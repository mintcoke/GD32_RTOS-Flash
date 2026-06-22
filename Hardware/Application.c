/*
 * Application.c — PLC 命令分发 + PDO 数据映射
 *
 * 本文件是 EtherCAT 从站应用层的核心，负责：
 *   1. 解析 PLC 通过 RxPDO 发来的 RFID 命令并执行
 *   2. 将 RFID 天线数据（RSSI/EPC/NEW）映射到 TxPDO
 *
 * PLC 命令交互流程（边沿触发）：
 *
 *   PLC 端:                          从站端:
 *   ┌──────────────────┐            ┌───────────────────────┐
 *   │ DO(CMD) = 命令码  │ ──RxPDO──>│ s_Armed=1 → 检测到命令  │
 *   │                  │            │ s_Armed=0 → 锁定       │
 *   │                  │            │ STATUS = BUSY          │
 *   │                  │            │ 执行 RFID 命令          │
 *   │ DO(CMD) = 0      │ ──RxPDO──>│ 检测到 CMD=0           │
 *   │                  │            │ s_Armed=1 → 重新武装    │
 *   │                  │ <─TxPDO── │ STATUS = OK/ERROR       │
 *   │ 读取响应数据      │            │ RESULT + DATA 已就绪    │
 *   └──────────────────┘            └───────────────────────┘
 *
 * s_Armed 机制说明：
 *   PLC 每个周期都发送 RxPDO，CMD 字段在命令执行期间保持不变。
 *   如果没有 s_Armed，同一个命令会被重复执行。
 *   s_Armed 确保：CMD 从 0 变为非零时只执行一次，
 *   直到 PLC 将 CMD 清零后才能触发下一个命令（下降沿重新武装）。
 */

#define _Application_ 1
#include <string.h>
#include "ecat_api.h"
#include "Application.h"
#include "rfid_ecat.h"
#undef _Application_

#include "SSC-Device.h"
#include "ecatslv.h"
#include "GD32Evb.h"
#include <stdio.h>

/* ============================================================
 * 内部常量定义
 * ============================================================ */

/* 单个天线的 PDO 内部偏移（相对于 pdo_base） */
#define RFID_PDO_RSSI        0   /* RSSI 信号强度偏移 */
#define RFID_PDO_EPC_LEN     1   /* EPC 长度偏移 */
#define RFID_PDO_NEW         2   /* 新标签标志偏移 */
#define RFID_PDO_EPC         3   /* EPC 数据起始偏移 (31 个 UINT16) */

/* 命令参数限制 */
#define RFID_ANT_SETTLE_MS   5U  /* 天线切换后等待稳定时间 (ms) */
#define RFID_CMD_DATA_WORDS  32  /* 命令响应数据最大字数 (32字 = 64字节) */
#define RFID_REQ_DATA_WORDS  30  /* 请求数据最大字数 (30字 = 60字节) */
#define RFID_CMD_DATA_BYTES  (RFID_CMD_DATA_WORDS * 2)  /* 64 字节 */
#define RFID_REQ_DATA_BYTES  (RFID_REQ_DATA_WORDS * 2)  /* 60 字节 */
#define RFID_WRITE_MAX_WORDS 15U /* RFID 模块单次写操作最大字数限制 (15字 = 30字节) */

/* EPC 存储区地址映射
 * EPC 存储区布局：[PC字(1word)] [CRC(1word)] [EPC数据(N words)]
 * PC 字地址 = 1, EPC 数据起始地址 = 2 */
#define RFID_EPC_PC_ADDR     1U           /* PC 字在 EPC 区的地址 */
#define RFID_EPC_DATA_ADDR   2U           /* EPC 数据在 EPC 区的起始地址 */

/* 原始命令超时 */
#define RFID_RAW_TIMEOUT_MS  500U         /* RAW 命令最大等待时间 */

/* 标签选择参数 */
#define RFID_EPC_SELECT_PTR  0x00000020UL /* EPC 选择掩码起始位 (第32位 = EPC数据起始) */
#define RFID_SELECT_MODE_OFF 0x01U        /* 选择模式关闭命令 */

/* ============================================================
 * 数据转换辅助函数
 * ============================================================ */

/*
 * 将字节数组打包为 UINT16（大端序）
 *
 * 从 data[byte_idx] 开始取 2 字节，高字节在前。
 * 如果 byte_idx 超出 len 范围，补零。
 * 用于将 uint8_t 数组写入 DI() PDO（PDO 按 UINT16 对齐）。
 */
static uint16_t RFID_PackBytes(const uint8_t *data, uint16_t len, uint16_t byte_idx)
{
    uint16_t w = 0;

    if (byte_idx < len) {
        w = (uint16_t)data[byte_idx] << 8;
        if ((uint16_t)(byte_idx + 1U) < len) {
            w |= data[byte_idx + 1U];
        } else {
            w >>= 8;  /* 仅 1 字节：放到低位，如 0x01 → 0x0001 */
        }
    }
    return w;
}

/* 读取大端序 UINT16 */
static uint16_t RFID_ReadU16BE(const uint8_t *data)
{
    return ((uint16_t)data[0] << 8) | data[1];
}

/* 写入大端序 UINT16 */
static void RFID_WriteU16BE(uint8_t *data, uint16_t value)
{
    data[0] = (uint8_t)(value >> 8);
    data[1] = (uint8_t)value;
}

/* 写入大端序 UINT32 */
static void RFID_WriteU32BE(uint8_t *data, uint32_t value)
{
    data[0] = (uint8_t)(value >> 24);
    data[1] = (uint8_t)(value >> 16);
    data[2] = (uint8_t)(value >> 8);
    data[3] = (uint8_t)value;
}

/*
 * 从 RxPDO 解包字节数据
 *
 * 将 DO(pdo_base + 0), DO(pdo_base + 1), ... 中的 UINT16
 * 拆分为字节（高字节在前），写入 data 数组。
 * 最多解包 RFID_REQ_DATA_WORDS 个字 = 60 字节。
 */
static void RFID_UnpackPdoBytes(uint16_t pdo_base, uint16_t byte_len, uint8_t *data)
{
    uint16_t out = 0;

    for (uint16_t i = 0; i < RFID_REQ_DATA_WORDS && out < byte_len; i++) {
        uint16_t w = DO(pdo_base + i);
        data[out++] = (uint8_t)(w >> 8);
        if (out < byte_len) {
            data[out++] = (uint8_t)w;
        }
    }
}

/* ============================================================
 * PDO 数据写入辅助
 * ============================================================ */

/*
 * 将命令响应数据写入 TxPDO
 *
 * 写入 DI(TX_RFID_CMD_DATA_LEN) = 数据长度
 * 写入 DI(TX_RFID_CMD_DATA + 0..31) = 数据内容（UINT16 大端对齐）
 */
static void RFID_WriteCmdDataToPdo(const uint8_t *data, uint16_t byte_len)
{
    if (byte_len > RFID_CMD_DATA_BYTES) {
        byte_len = RFID_CMD_DATA_BYTES;
    }

    DI(TX_RFID_CMD_DATA_LEN) = byte_len;
    for (uint16_t i = 0; i < RFID_CMD_DATA_WORDS; i++) {
        DI(TX_RFID_CMD_DATA + i) = RFID_PackBytes(data, byte_len, (uint16_t)(i * 2U));
    }
}

/* 清除命令响应数据区 */
static void RFID_ClearCmdDataPdo(void)
{
    DI(TX_RFID_CMD_DATA_LEN) = 0;
    for (uint16_t i = 0; i < RFID_CMD_DATA_WORDS; i++) {
        DI(TX_RFID_CMD_DATA + i) = 0;
    }
}

/* ============================================================
 * PLC 命令包装函数 — 简化 RFID_Command 的调用
 * ============================================================ */

/* 通用 PLC 命令：发送命令 + 数据，接收响应，成功时写入 TxPDO */
static int RFID_PlcCommand(uint8_t cmd_id,
                           const uint8_t *tx_data,
                           uint16_t tx_len,
                           uint8_t *rx_data,
                           uint16_t *rx_len)
{
    int ret = RFID_Command(cmd_id, tx_data, tx_len, rx_data, rx_len, RFID_RAW_TIMEOUT_MS);

    if (ret == RFID_RET_OK) {
        RFID_WriteCmdDataToPdo(rx_data, *rx_len);
    }
    return ret;
}

/* 无参数命令（如 GET_POWER, GET_REGION） */
static int RFID_PlcCommandNoParam(uint8_t cmd_id, uint8_t *buf)
{
    uint16_t rsp_len = RFID_CMD_DATA_BYTES;

    return RFID_PlcCommand(cmd_id, NULL, 0U, buf, &rsp_len);
}

/* 带 1 字节参数的命令（如 SET_REGION, SET_MODE） */
static int RFID_PlcCommandU8(uint8_t cmd_id, uint8_t value, uint8_t *buf)
{
    uint16_t rsp_len = RFID_CMD_DATA_BYTES;

    buf[0] = value;
    return RFID_PlcCommand(cmd_id, buf, 1U, buf, &rsp_len);
}

/*
 * 仅检查状态码的命令
 *
 * 许多 RFID 模块命令的响应第一个字节是状态码：0=成功, 非0=错误。
 * 本函数自动检查状态码，非零时返回 RFID_RET_MODULE_ERR。
 * 成功时将完整响应写入 TxPDO。
 */
static int RFID_PlcCommandStatusOnly(uint8_t cmd_id, const uint8_t *tx_data, uint16_t tx_len, uint8_t *buf)
{
    uint16_t rsp_len = RFID_CMD_DATA_BYTES;
    int ret;

    ret = RFID_Command(cmd_id, tx_data, tx_len, buf, &rsp_len, RFID_RAW_TIMEOUT_MS);
    if (ret == RFID_RET_OK) {
        RFID_WriteCmdDataToPdo(buf, rsp_len);
        if (rsp_len < 1U) {
            rfid_last_result = RFID_RET_FRAME_ERR;
            return RFID_RET_FRAME_ERR;
        }
        if (buf[0] != 0U) {
            rfid_last_error = buf[0];
            rfid_last_result = RFID_RET_MODULE_ERR;
            return RFID_RET_MODULE_ERR;
        }
    }
    return ret;
}

/* 带 1 字节参数的状态命令（如 SET_HOP, SET_CARRIER） */
static int RFID_PlcCommandStatusU8(uint8_t cmd_id, uint8_t value, uint8_t *buf)
{
    buf[0] = value;
    return RFID_PlcCommandStatusOnly(cmd_id, buf, 1U, buf);
}

/* 带 2 字节参数的状态命令（如 SET_POWER） */
static int RFID_PlcCommandStatusU16(uint8_t cmd_id, uint16_t value, uint8_t *buf)
{
    RFID_WriteU16BE(buf, value);
    return RFID_PlcCommandStatusOnly(cmd_id, buf, 2U, buf);
}

/*
 * 功率参数转换
 *
 * PLC 传入的功率值有两种格式：
 *   1~33: 表示 dBm 值，转换为模块内部单位 (×100，如 30 → 3000)
 *   >33: 直接使用（已经是模块内部单位）
 */
static uint16_t RFID_PlcPowerParam(uint16_t value)
{
    if (value <= 33U) {
        return (uint16_t)(value * 100U);
    }
    return value;
}

/* ============================================================
 * 标签选择函数 — 写操作前需要先选择目标标签
 * ============================================================ */

/*
 * 选择指定 EPC 的标签
 *
 * 构造 Select 命令参数：
 *   [Target/Action/MemBank] [Ptr(4B)] [MaskLen(bits)] [MaskLen(bytes)] [MaskData...]
 *   选择 EPC 存储区从第 32 位开始、长度为 byte_len*8 位的标签
 */
static int RFID_PlcSelectEpcBytes(const uint8_t *epc, uint16_t byte_len, uint8_t *buf)
{
    if (epc == NULL || byte_len == 0U || byte_len > 31U) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    buf[0] = 0x01U; /* Target=0, Action=0, MemBank=EPC (0x01) */
    RFID_WriteU32BE(&buf[1], RFID_EPC_SELECT_PTR); /* 选择掩码起始位 = 32 */
    buf[5] = (uint8_t)(byte_len * 8U);             /* 掩码长度 (位) */
    buf[6] = 0x00U;                                  /* 掩码长度 (字节, 填充) */
    memcpy(&buf[7], epc, byte_len);                  /* 掩码数据 = EPC 码 */
    return RFID_PlcCommandStatusOnly(RFID_CMD_SET_SELECT,
                                     buf,
                                     (uint16_t)(7U + byte_len),
                                     buf);
}

/* 从 RxPDO 解包 EPC 数据后选择标签 */
static int RFID_PlcSelectEpcFromPdo(uint16_t byte_len, uint8_t *buf)
{
    uint8_t epc[RFID_REQ_DATA_BYTES];

    RFID_UnpackPdoBytes(RX_RFID_DATA, byte_len, epc);
    return RFID_PlcSelectEpcBytes(epc, byte_len, buf);
}

/*
 * 写入后更新 EPC 缓存
 *
 * 写 EPC 成功后，更新对应天线的 g_RfidEpc 缓存，
 * 避免下次扫描时读到旧数据。
 */
static void RFID_UpdateEpcCacheFromWrite(uint8_t ant,
                                         uint16_t actual_addr,
                                         const uint8_t *data,
                                         uint16_t byte_len)
{
    uint16_t offset;
    uint16_t copy_len;
    uint8_t idx;

    if (ant < 1U || ant > RFID_ANT_COUNT ||
        actual_addr < RFID_EPC_DATA_ADDR ||
        data == 0 || byte_len == 0U) {
        return;
    }

    /* 计算写入位置在缓存中的偏移 */
    offset = (uint16_t)((actual_addr - RFID_EPC_DATA_ADDR) * 2U);
    if (offset >= RFID_EPC_BYTES) {
        return;
    }

    /* 截断超出缓存的部分 */
    copy_len = byte_len;
    if ((uint16_t)(offset + copy_len) > RFID_EPC_BYTES) {
        copy_len = (uint16_t)(RFID_EPC_BYTES - offset);
    }

    idx = (uint8_t)(ant - 1U);
    memcpy(&g_RfidEpc[idx][offset], data, copy_len);
    if (g_RfidEpcLen[idx] < (uint8_t)(offset + copy_len)) {
        g_RfidEpcLen[idx] = (uint8_t)(offset + copy_len);
    }
    g_RfidNew[idx] = 1U;
}

/*
 * 确保目标标签已被选择
 *
 * 写标签前需要先选择标签。如果缓存中没有 EPC 数据，
 * 先执行一次 Inventory 获取标签信息，然后再发送 Select 命令。
 */
static int RFID_PlcEnsureSelected(uint8_t ant, uint8_t *buf)
{
    rfid_tag_t tag;
    uint8_t idx;
    int ret;

    if (ant < 1U || ant > RFID_ANT_COUNT) {
        return RFID_RET_OK;
    }

    idx = (uint8_t)(ant - 1U);
    if (g_RfidEpcLen[idx] == 0U) {
        /* 缓存中没有标签数据，先盘点获取 */
        ret = RFID_Inventory(&tag);
        if (ret != RFID_RET_OK) {
            return ret;
        }

        g_RfidRssi[idx] = tag.rssi;
        g_RfidEpcLen[idx] = tag.epc_len;
        memset(g_RfidEpc[idx], 0, RFID_EPC_BYTES);
        memcpy(g_RfidEpc[idx], tag.epc, tag.epc_len);
        g_RfidNew[idx] = 1U;
    }

    /* 用缓存的 EPC 数据选择标签 */
    return RFID_PlcSelectEpcBytes(g_RfidEpc[idx], g_RfidEpcLen[idx], buf);
}


/*
 * 分段写入标签存储区 — RFID 模块单次写最大 15 words
 *
 * RFID 模块的写命令 (0x49) 单次最多写入 15 words (30 字节)，
 * 超过此限制模块会返回错误。本函数自动将大数据拆分为多段写入，
 * 每段最多 RFID_WRITE_MAX_WORDS (15) words。
 */
static int RFID_WriteTagChunked(uint8_t bank, uint16_t addr, uint16_t words, const uint8_t *data)
{
    uint16_t offset_words = 0U;

    while (offset_words < words) {
        uint16_t chunk = (uint16_t)(words - offset_words);
        uint16_t chunk_addr = (uint16_t)(addr + offset_words);
        int ret;

        if (chunk > RFID_WRITE_MAX_WORDS) {
            chunk = RFID_WRITE_MAX_WORDS;
        }

        ret = RFID_WriteTag(bank, chunk_addr, chunk, &data[offset_words * 2U]);
        if (ret != RFID_RET_OK) {
            return ret;
        }

        offset_words = (uint16_t)(offset_words + chunk);
    }

    return RFID_RET_OK;
}

/* ============================================================
 * 阻塞延时 — 天线切换后等待稳定
 *
 * 使用 ECAT_KeepAlive() 保活，确保 EtherCAT 通信不中断。
 * ============================================================ */
static void RFID_EcatDelayMs(uint32_t ms)
{
    extern volatile uint32_t g_SysTickCnt;
    uint32_t deadline = g_SysTickCnt + ms;

    while ((int32_t)(g_SysTickCnt - deadline) < 0) {
        ECAT_KeepAlive();
    }
}

/* ============================================================
 * 天线数据 → TxPDO 映射
 *
 * 将每个天线的 RSSI/EPC_LEN/NEW/EPC 写入 TxPDO 对应位置。
 * 在 APPL_UpdateTxPdo() 中被调用（每个 PDO 周期一次）。
 * ============================================================ */
static void RFID_MapAntennaToPdo(uint8_t ant_idx, uint16_t pdo_base)
{
    extern uint8_t  g_RfidRssi[RFID_ANT_COUNT];
    extern uint8_t  g_RfidEpcLen[RFID_ANT_COUNT];
    extern uint8_t  g_RfidNew[RFID_ANT_COUNT];
    extern uint8_t  g_RfidEpc[RFID_ANT_COUNT][RFID_EPC_BYTES];

    DI(pdo_base + RFID_PDO_RSSI) = g_RfidRssi[ant_idx];
    DI(pdo_base + RFID_PDO_EPC_LEN) = g_RfidEpcLen[ant_idx];
    DI(pdo_base + RFID_PDO_NEW) = g_RfidNew[ant_idx];

    /* EPC 数据打包为 UINT16（大端序），共 31 个字 = 62 字节 */
    for (int i = 0; i < 31; i++) {
        uint16_t byte_idx = (uint16_t)(i * 2);
        DI(pdo_base + RFID_PDO_EPC + i) =
            RFID_PackBytes(g_RfidEpc[ant_idx], g_RfidEpcLen[ant_idx], byte_idx);
    }
}

/* ============================================================
 * RFID_EcatCmdTask — PLC 命令分发核心
 *
 * 从 RxPDO 读取命令参数，执行对应的 RFID 操作，
 * 将结果写入 TxPDO。
 *
 * 返回值：
 *   1 = 正在处理命令（主循环跳过 RFID_Scan）
 *   0 = 无命令（主循环继续 RFID_Scan）
 *
 * 边沿触发机制 (s_Armed)：
 *   - CMD 从 0 变为非零 → s_Armed=1 → 执行命令 → s_Armed=0
 *   - CMD 保持非零期间 → s_Armed=0 → 跳过（避免重复执行）
 *   - CMD 变为 0 → s_Armed=1（重新武装，准备下一次命令）
 * ============================================================ */
uint8_t RFID_EcatCmdTask(void)
{
    static uint8_t s_Armed = 1U;  /* 命令武装标志：1=等待新命令, 0=命令锁定中 */
    uint8_t buf[RFID_CMD_DATA_BYTES];
    uint16_t cmd = DO(RX_RFID_CMD);       /* 读取命令码 */
    uint16_t ant = DO(RX_RFID_ANT);       /* 读取天线编号 */
    uint16_t addr = DO(RX_RFID_ADDR);     /* 读取地址/参数 */
    uint16_t words = DO(RX_RFID_WORDS);   /* 读取字数/参数 */
    int ret = RFID_RET_FRAME_ERR;


    /* RFID soft reset is in progress. Do not start any blocking RFID command,
     * and report "busy" so the caller also skips automatic scanning. */
    if (g_RfidWdTriggered != 0U) {
        if (cmd != RFID_PLC_CMD_NONE) {
            DI(TX_RFID_CMD_ECHO) = cmd;
            DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_BUSY;
            DI(TX_RFID_CMD_RESULT) = 0U;
        }
        return 1U;
    }

    /* 无命令：保持 IDLE 状态，重新武装 */
    if (cmd == RFID_PLC_CMD_NONE) {
        if (DI(TX_RFID_CMD_STATUS) != RFID_PLC_STATUS_BUSY) {
            DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_IDLE;
        }
        s_Armed = 1U;
        return 0U;
    }

    /* 命令锁定中（同一命令不重复执行） */
    if (s_Armed == 0U) {
        return 0U;
    }
    s_Armed = 0U;  /* 锁定，直到 CMD 清零才重新武装 */

    /* 设置响应头 */
    DI(TX_RFID_CMD_ECHO) = cmd;
    DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_BUSY;
    DI(TX_RFID_CMD_RESULT) = 0;
    RFID_ClearCmdDataPdo();

    /* 切换天线（如果指定了天线编号） */
    if (ant >= 1U && ant <= RFID_ANT_COUNT) {
        RFID_SetAntenna((uint8_t)ant);
        RFID_EcatDelayMs(RFID_ANT_SETTLE_MS);  /* 等待 5ms 天线稳定 */
    }

    /* ---- 命令分发 ---- */
    switch (cmd) {

    case RFID_PLC_CMD_GET_INFO:       /* 读取模块信息 */
        if (addr > 1U) {
            addr = 1U;
        }
        ret = RFID_PlcCommandU8(RFID_CMD_READ_INFO, (uint8_t)addr, buf);
        break;

    case RFID_PLC_CMD_READ_TID:       /* 读标签 TID 区 */
        if (words == 0U) {
            words = 6U;  /* 默认读 6 个字 */
        }
        if (words > (uint16_t)(RFID_CMD_DATA_BYTES / 2U)) {
            words = (uint16_t)(RFID_CMD_DATA_BYTES / 2U);
        }
        ret = RFID_ReadTag(RFID_BANK_TID, addr, words, buf);
        if (ret == RFID_RET_OK) {
            RFID_WriteCmdDataToPdo(buf, (uint16_t)(words * 2U));
        }
        break;

    case RFID_PLC_CMD_READ_USER:      /* 读标签用户区 */
        if (words == 0U) {
            words = 6U;
        }
        if (words > (uint16_t)(RFID_CMD_DATA_BYTES / 2U)) {
            words = (uint16_t)(RFID_CMD_DATA_BYTES / 2U);
        }
        ret = RFID_PlcEnsureSelected((uint8_t)ant, buf);
        if (ret == RFID_RET_OK) {
            ret = RFID_ReadTag(RFID_BANK_USER, addr, words, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteCmdDataToPdo(buf, (uint16_t)(words * 2U));
            }
        }
        break;

    case RFID_PLC_CMD_WRITE_USER:     /* 写标签用户区 */
        if (words == 0U || words > (uint16_t)(RFID_REQ_DATA_BYTES / 2U)) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint16_t write_bytes = (uint16_t)(words * 2U);
            ret = RFID_PlcEnsureSelected((uint8_t)ant, buf);
            if (ret == RFID_RET_OK) {
                RFID_UnpackPdoBytes(RX_RFID_DATA, write_bytes, buf);
                RFID_WriteCmdDataToPdo(buf, write_bytes);
                ret = RFID_WriteTagChunked(RFID_BANK_USER, addr, words, buf);
                {
                    uint8_t rd_buf[RFID_REQ_DATA_BYTES];
                    int rd_ret = RFID_ReadTag(RFID_BANK_USER, addr, words, rd_buf);
                    if (rd_ret == RFID_RET_OK && memcmp(rd_buf, buf, write_bytes) == 0) {
                        ret = RFID_RET_OK;
                    } else {
                        ret = RFID_RET_FRAME_ERR;
                    }
                }
            }
        }
        break;

    case RFID_PLC_CMD_WRITE_EPC:      /* 写标签 EPC 区（自动选标签） */
        /* EPC 存储区前两个字为 PC/CRC（addr=1 为 PC 字），禁止写入。
         * 写 EPC 数据必须从 addr >= RFID_EPC_DATA_ADDR(2) 开始；
         * addr < 2 视为参数错误，避免误写 PC/CRC。 */
        if (words == 0U || words > RFID_WRITE_MAX_WORDS
            || addr < RFID_EPC_DATA_ADDR) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t write_buf[RFID_REQ_DATA_BYTES];
            uint16_t actual_addr = addr;
            uint16_t write_bytes = (uint16_t)(words * 2U);
            ret = RFID_PlcEnsureSelected((uint8_t)ant, buf);
            if (ret == RFID_RET_OK) {
                RFID_UnpackPdoBytes(RX_RFID_DATA, write_bytes, write_buf);
                RFID_WriteCmdDataToPdo(write_buf, write_bytes);
                ret = RFID_WriteTag(RFID_BANK_EPC, actual_addr, words, write_buf);
                if (ret == RFID_RET_OK) {
                    RFID_UpdateEpcCacheFromWrite((uint8_t)ant, actual_addr, write_buf, write_bytes);
                    if (ant >= 1U && ant <= RFID_ANT_COUNT && g_RfidEpcLen[ant - 1U] > 0U) {
                        (void)RFID_PlcSelectEpcBytes(g_RfidEpc[ant - 1U], g_RfidEpcLen[ant - 1U], buf);
                    }
                    RFID_WriteCmdDataToPdo(write_buf, write_bytes);
                    rfid_last_error = 0U;
                    rfid_last_result = RFID_RET_OK;
                }
            }
        }
        break;

    case RFID_PLC_CMD_SET_EPC_LEN:    /* 修改 EPC 长度（PC 字的高 5 位） */
        if (words == 0U || words > RFID_WRITE_MAX_WORDS) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            ret = RFID_PlcEnsureSelected((uint8_t)ant, buf);
            if (ret == RFID_RET_OK) {
                ret = RFID_ReadTag(RFID_BANK_EPC, RFID_EPC_PC_ADDR, 1U, buf);
                if (ret == RFID_RET_OK) {
                    uint16_t pc = RFID_ReadU16BE(buf);
                    pc = (uint16_t)((pc & 0x07FFU) | ((words & 0x1FU) << 11));
                    RFID_WriteU16BE(buf, pc);
                    RFID_WriteCmdDataToPdo(buf, 2U);
                    ret = RFID_WriteTag(RFID_BANK_EPC, RFID_EPC_PC_ADDR, 1U, buf);
                }
            }
        }
        break;

    case RFID_PLC_CMD_SET_POWER:      /* 设置发射功率 */
        {
            uint16_t pwr = RFID_PlcPowerParam(words);
            ret = RFID_PlcCommandStatusU16(RFID_CMD_SET_POWER, pwr, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, pwr);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_POWER:      /* 读取发射功率 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_POWER, buf);
        break;

    case RFID_PLC_CMD_SET_REGION:     /* 设置工作区域 */
        if (addr > 0xFFU) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t region = (uint8_t)addr;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_REGION, region, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)region);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_REGION:     /* 读取工作区域 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_REGION, buf);
        break;

    case RFID_PLC_CMD_SET_CHANNEL:    /* 设置固定信道 */
        if (addr > 0xFFU) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t ch = (uint8_t)addr;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_CHANNEL, ch, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)ch);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_CHANNEL:    /* 读取当前信道 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_CHANNEL, buf);
        break;

    case RFID_PLC_CMD_SET_HOP:        /* 设置跳频模式 */
        {
            uint8_t hop = (addr == 0U) ? 0x00U : 0xFFU;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_HOP, hop, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)hop);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_HOP:        /* 读取跳频模式 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_HOP, buf);
        break;

    case RFID_PLC_CMD_SET_MODE:       /* 设置工作模式 */
        if (addr > 1U) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t mode = (uint8_t)addr;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_MODE, mode, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)mode);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_MODE:       /* 读取工作模式 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_MODE, buf);
        break;

    case RFID_PLC_CMD_INVENTORY:      /* 单次盘点 */
        {
            rfid_tag_t tag;
            ret = RFID_Inventory(&tag);
            if (ret == RFID_RET_OK) {
                buf[0] = tag.rssi;
                RFID_WriteU16BE(&buf[1], tag.pc);
                buf[3] = tag.epc_len;
                memcpy(&buf[4], tag.epc, tag.epc_len);
                RFID_WriteCmdDataToPdo(buf, (uint16_t)(4U + tag.epc_len));
            }
        }
        break;

    case RFID_PLC_CMD_STOP_POLL:      /* 停止连续盘点 */
        ret = RFID_PlcCommandStatusOnly(RFID_CMD_STOP_POLL, NULL, 0U, buf);
        break;

    case RFID_PLC_CMD_SET_CH_LIST:    /* 设置跳频信道列表 */
        if (words > RFID_REQ_DATA_BYTES) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t cnt = (uint8_t)words;
            buf[0] = cnt;
            RFID_UnpackPdoBytes(RX_RFID_DATA, cnt, &buf[1]);
            ret = RFID_PlcCommandStatusOnly(RFID_CMD_SET_CH_LIST, buf, (uint16_t)(cnt + 1U), buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)cnt);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_CH_LIST:    /* 读取跳频信道列表 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_CH_LIST, buf);
        break;

    case RFID_PLC_CMD_SET_RX:         /* 设置接收灵敏度 */
        {
            uint8_t rx_params[4];
            RFID_UnpackPdoBytes(RX_RFID_DATA, 4U, rx_params);
            ret = RFID_PlcCommandStatusOnly(RFID_CMD_SET_RX, rx_params, 4U, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteCmdDataToPdo(rx_params, 4U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_RX:         /* 读取接收灵敏度 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_RX, buf);
        break;

    case RFID_PLC_CMD_TEST_RSSI:      /* RSSI 测试 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_TEST_RSSI, buf);
        break;

    case RFID_PLC_CMD_SET_CARRIER:    /* 设置载波开关 */
        {
            uint8_t carrier = (addr == 0U) ? 0x00U : 0xFFU;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_CARRIER, carrier, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)carrier);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_RESET:          /* 软重置 RFID 模块 */
        RFID_Reset();
        ret = RFID_RET_OK;
        break;

    case RFID_PLC_CMD_SELECT_EPC:     /* 选择指定 EPC 的标签 */
        if (words == 0U) {
            words = (ant >= 1U && ant <= RFID_ANT_COUNT) ? g_RfidEpcLen[ant - 1U] : 0U;
            if (words > 0U) {
                ret = RFID_PlcSelectEpcBytes(g_RfidEpc[ant - 1U], words, buf);
            } else {
                ret = RFID_RET_FRAME_ERR;
            }
        } else {
            ret = RFID_PlcSelectEpcFromPdo(words, buf);
        }
        break;

    case RFID_PLC_CMD_CLEAR_SELECT:   /* 清除标签选择 */
        ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_SEL_MODE, RFID_SELECT_MODE_OFF, buf);
        break;

    case RFID_PLC_CMD_RAW:            /* 原始命令：直接发指令给 RFID 模块 */
        if (addr > 0xFFU || words > RFID_REQ_DATA_BYTES) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint16_t rsp_len = RFID_CMD_DATA_BYTES;
            RFID_UnpackPdoBytes(RX_RFID_DATA, words, buf);
            ret = RFID_Command((uint8_t)addr, buf, words, buf, &rsp_len, RFID_RAW_TIMEOUT_MS);
            RFID_WriteCmdDataToPdo(buf, rsp_len);
        }
        break;

    default:                           /* 未知命令 */
        ret = RFID_RET_FRAME_ERR;
        break;
    }

    /* ---- 写入命令执行结果到 TxPDO ---- */
    if (ret == RFID_RET_OK) {
        DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_OK;
        DI(TX_RFID_CMD_RESULT) = 0;
    } else {
        DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_ERROR;
        DI(TX_RFID_CMD_RESULT) = (uint16_t)((rfid_last_error != 0U) ? rfid_last_error : (uint16_t)(-ret));
    }
    return 1U;  /* 命令处理中，主循环跳过 RFID_Scan */
}

/* ============================================================
 * APPL_UpdateTxPdo — 周期性任务（每个 PDO 周期调用一次）
 *
 * 由 ECAT_RegisterPeriodicTask() 注册，
 * 在 APPL_CoeTxPdoMapping() → Bridge_PeriodicTask() 中被调用。
 *
 * 功能：
 *   1. 递增心跳计数器 → DI(0)
 *   2. 将 3 个天线的标签数据映射到 TxPDO
 * ============================================================ */
void APPL_UpdateTxPdo(void)
{
    static uint16_t s_Counter = 0;
    static const uint16_t rfid_pdo_base[RFID_ANT_COUNT] = {
        TX_RFID1_RSSI,   /* 天线1: DI(1..34) */
        TX_RFID2_RSSI,   /* 天线2: DI(35..68) */
        TX_RFID3_RSSI    /* 天线3: DI(69..102) */
    };

    /* 心跳计数器 — 主站用来检测从站 PDO 交换是否正常 */
    s_Counter++;
    DI(0) = s_Counter;

    /* 将每个天线的 RSSI/EPC_LEN/NEW/EPC 写入 TxPDO */
    for (uint8_t ant = 0; ant < RFID_ANT_COUNT; ant++) {
        RFID_MapAntennaToPdo(ant, rfid_pdo_base[ant]);
    }
}

/* ============================================================
 * APPL_SafeOutput — 安全输出（进入 SAFE-OP 时调用）
 *
 * 当前无安全输出需求，保留为空函数。
 * ============================================================ */
void APPL_SafeOutput(void)
{
}
