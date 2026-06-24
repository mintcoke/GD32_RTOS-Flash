/*
 * Application.c — PLC 命令分发 + PDO 数据映射
 *
 * 应用层核心：解析 PLC 通过 RxPDO 发来的 RFID 命令并执行，
 * 将 RFID 天线数据(RSSI/EPC/NEW)映射到 TxPDO。
 *
 * 命令边沿触发 (s_Armed)：
 *   PLC 每周期都发 RxPDO，CMD 在执行期间保持不变。s_Armed 确保
 *   CMD 从 0 变非零时只执行一次，直到 PLC 清零 CMD(下降沿)才重新武装。
 *   不加此机制同一命令会被 PDO 周期重复执行。
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

/* ---- 内部常量 ---- */

/* 单天线 PDO 内部偏移 (相对 pdo_base) */
#define RFID_PDO_RSSI        0
#define RFID_PDO_EPC_LEN     1
#define RFID_PDO_NEW         2
#define RFID_PDO_EPC         3   /* EPC 数据起始 (31 个 UINT16) */

#define RFID_ANT_SETTLE_MS   5U  /* 天线切换稳定时间 */
#define RFID_CMD_DATA_WORDS  128 /* 命令响应最大字数 (256 字节, 受 TxPDO 数据区限制) */
#define RFID_REQ_DATA_WORDS  128 /* 请求数据最大字数 (256 字节, 受 RxPDO 数据区限制) */
#define RFID_CMD_DATA_BYTES  (RFID_CMD_DATA_WORDS * 2)
#define RFID_REQ_DATA_BYTES  (RFID_REQ_DATA_WORDS * 2)

/* 各存储区读写上限(字)。标准上限放行，越界写由标签返回错误码如实上报。
 * RFU: 标准固定 64 位=4 字，写禁止。
 * EPC: 标准上限 31 字(PC 5 位编码)，前两字 PC/CRC 禁写。
 * TID/USER: 标准未限定，固件按 PDO 数据区 128 字限(单次命令传输上限)。 */
#define RFID_RFU_MAX_WORDS     4U
#define RFID_EPC_MAX_WORDS     31U
#define RFID_TID_MAX_WORDS     128U
#define RFID_USER_MAX_WORDS    128U
#define RFID_EPC_FIXED_WORDS   8U   /* READ_EPC_USER_ALL 每通道 EPC 固定读 8 字(不足补0, 超出截断), 保证每通道固定长度 */

/* EPC 区布局: [PC(1word,addr=1)][CRC(1word)][EPC数据(addr>=2)] */
#define RFID_EPC_PC_ADDR     1U
#define RFID_EPC_DATA_ADDR   2U

#define RFID_RAW_TIMEOUT_MS  500U

#define RFID_EPC_SELECT_PTR  0x00000020UL /* EPC 选择掩码起始位 (第32位) */
#define RFID_SELECT_MODE_OFF 0x01U

/* ---- 数据转换辅助 ---- */

/* 字节数组打包为 UINT16(大端)。byte_idx 越界补零。
 * 用于把 uint8_t 写入 DI() (PDO 按 UINT16 对齐)。 */
static uint16_t RFID_PackBytes(const uint8_t *data, uint16_t len, uint16_t byte_idx)
{
    uint16_t w = 0;

    if (byte_idx < len) {
        w = (uint16_t)data[byte_idx] << 8;
        if ((uint16_t)(byte_idx + 1U) < len) {
            w |= data[byte_idx + 1U];
        } else {
            w >>= 8;  /* 仅 1 字节放低位: 0x01 → 0x0001 */
        }
    }
    return w;
}

static uint16_t RFID_ReadU16BE(const uint8_t *data)
{
    return ((uint16_t)data[0] << 8) | data[1];
}

static void RFID_WriteU16BE(uint8_t *data, uint16_t value)
{
    data[0] = (uint8_t)(value >> 8);
    data[1] = (uint8_t)value;
}

static void RFID_WriteU32BE(uint8_t *data, uint32_t value)
{
    data[0] = (uint8_t)(value >> 24);
    data[1] = (uint8_t)(value >> 16);
    data[2] = (uint8_t)(value >> 8);
    data[3] = (uint8_t)value;
}

/* 从 RxPDO 解包字节数据。DO(pdo_base+i) 拆为高字节在前，最多 60 字节。 */
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

/* ---- PDO 数据写入辅助 ---- */

/* 命令响应写入 TxPDO: DI(LEN)=长度, DI(DATA+0..31)=内容(大端) */
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

static void RFID_ClearCmdDataPdo(void)
{
    DI(TX_RFID_CMD_DATA_LEN) = 0;
    for (uint16_t i = 0; i < RFID_CMD_DATA_WORDS; i++) {
        DI(TX_RFID_CMD_DATA + i) = 0;
    }
}

/* ---- PLC 命令包装 (简化 RFID_Command 调用) ---- */

/* 通用：发命令+数据，收响应，成功时写入 TxPDO */
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

static int RFID_PlcCommandNoParam(uint8_t cmd_id, uint8_t *buf)
{
    uint16_t rsp_len = RFID_CMD_DATA_BYTES;
    return RFID_PlcCommand(cmd_id, NULL, 0U, buf, &rsp_len);
}

static int RFID_PlcCommandU8(uint8_t cmd_id, uint8_t value, uint8_t *buf)
{
    uint16_t rsp_len = RFID_CMD_DATA_BYTES;
    buf[0] = value;
    return RFID_PlcCommand(cmd_id, buf, 1U, buf, &rsp_len);
}

/* 检查响应首字节状态码 (0=成功, 非0=模块错误)，成功时写 TxPDO */
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

static int RFID_PlcCommandStatusU8(uint8_t cmd_id, uint8_t value, uint8_t *buf)
{
    buf[0] = value;
    return RFID_PlcCommandStatusOnly(cmd_id, buf, 1U, buf);
}

static int RFID_PlcCommandStatusU16(uint8_t cmd_id, uint16_t value, uint8_t *buf)
{
    RFID_WriteU16BE(buf, value);
    return RFID_PlcCommandStatusOnly(cmd_id, buf, 2U, buf);
}

/* 功率参数转换: 1~33 视为 dBm(×100, 如 30→3000); >33 直接用(已是内部单位) */
static uint16_t RFID_PlcPowerParam(uint16_t value)
{
    if (value <= 33U) {
        return (uint16_t)(value * 100U);
    }
    return value;
}

/* ---- 标签选择 (写操作前需先选目标标签) ---- */

/* 选择指定 EPC 的标签。
 * Select 参数: [Target/Action/MemBank][Ptr(4B)][MaskLen(bits)][MaskLen(bytes)][MaskData]
 * 选 EPC 区从第 32 位起、长 byte_len*8 位的标签。 */
static int RFID_PlcSelectEpcBytes(const uint8_t *epc, uint16_t byte_len, uint8_t *buf)
{
    if (epc == NULL || byte_len == 0U || byte_len > 31U) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    buf[0] = 0x01U; /* MemBank=EPC */
    RFID_WriteU32BE(&buf[1], RFID_EPC_SELECT_PTR);
    buf[5] = (uint8_t)(byte_len * 8U);
    buf[6] = 0x00U;
    memcpy(&buf[7], epc, byte_len);
    return RFID_PlcCommandStatusOnly(RFID_CMD_SET_SELECT,
                                     buf,
                                     (uint16_t)(7U + byte_len),
                                     buf);
}

static int RFID_PlcSelectEpcFromPdo(uint16_t byte_len, uint8_t *buf)
{
    uint8_t epc[RFID_REQ_DATA_BYTES];
    RFID_UnpackPdoBytes(RX_RFID_DATA, byte_len, epc);
    return RFID_PlcSelectEpcBytes(epc, byte_len, buf);
}

/* 写 EPC 成功后更新对应天线缓存，避免下次扫描读到旧数据 */
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

    offset = (uint16_t)((actual_addr - RFID_EPC_DATA_ADDR) * 2U);
    if (offset >= RFID_EPC_BYTES) {
        return;
    }

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

/* 确保目标标签已选。缓存无 EPC 时先 Inventory 获取，再发 Select。 */
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

    return RFID_PlcSelectEpcBytes(g_RfidEpc[idx], g_RfidEpcLen[idx], buf);
}

/* 写标签：words 多少直接单次写多少，模块拒写由 RFID_WriteTag 如实返回错误码。 */
static int RFID_WriteTagChunked(uint8_t bank, uint16_t addr, uint16_t words, const uint8_t *data)
{
    return RFID_WriteTag(bank, addr, words, data);
}

/* 阻塞延时(天线切换稳定)。用 ECAT_KeepAlive 保活。 */
static void RFID_EcatDelayMs(uint32_t ms)
{
    extern volatile uint32_t g_SysTickCnt;
    uint32_t deadline = g_SysTickCnt + ms;

    while ((int32_t)(g_SysTickCnt - deadline) < 0) {
        ECAT_KeepAlive();
    }
}

/* 单天线数据 → TxPDO。在 APPL_UpdateTxPdo() 中调用。 */
static void RFID_MapAntennaToPdo(uint8_t ant_idx, uint16_t pdo_base)
{
    extern uint8_t  g_RfidRssi[RFID_ANT_COUNT];
    extern uint8_t  g_RfidEpcLen[RFID_ANT_COUNT];
    extern uint8_t  g_RfidNew[RFID_ANT_COUNT];
    extern uint8_t  g_RfidEpc[RFID_ANT_COUNT][RFID_EPC_BYTES];

    DI(pdo_base + RFID_PDO_RSSI) = g_RfidRssi[ant_idx];
    DI(pdo_base + RFID_PDO_EPC_LEN) = g_RfidEpcLen[ant_idx];
    DI(pdo_base + RFID_PDO_NEW) = g_RfidNew[ant_idx];

    for (int i = 0; i < 31; i++) {
        uint16_t byte_idx = (uint16_t)(i * 2);
        DI(pdo_base + RFID_PDO_EPC + i) =
            RFID_PackBytes(g_RfidEpc[ant_idx], g_RfidEpcLen[ant_idx], byte_idx);
    }
}

/* 一键读三通道 EPC+USER。
 * 每通道: [EPC状态][EPC N字][USER状态][USER WORDS字] 依次写入 TxPDO 响应区。
 * EPC 长度 N 取自标签 PC 字高 5 位(自动适配); USER 读 ADDR 起 WORDS 字。
 * 失败通道状态写错误码、数据填 0，继续下一通道。总长超 128 字截断。
 * USER 区读前用各通道缓存 EPC 选标签(无缓存先 Inventory)。 */
static int RFID_ReadEpcUserAll(uint16_t user_addr, uint16_t user_words, uint8_t *buf)
{
    uint16_t out_word = 0;   /* TxPDO 响应区当前写入字偏移(相对 TX_RFID_CMD_DATA) */
    uint16_t user_bytes = (uint16_t)(user_words * 2U);

    for (uint8_t ant = 1U; ant <= RFID_ANT_COUNT; ant++) {
        uint16_t epc_words = 0;
        uint8_t epc_status = 0;
        uint8_t user_status = 0;
        uint8_t epc_buf[RFID_EPC_BYTES + 2];

        RFID_SetAntenna(ant);
        RFID_EcatDelayMs(RFID_ANT_SETTLE_MS);

        /* --- 读 EPC: 固定读 8 字, 失败填 0 (保证每通道固定长度好解析) --- */
        {
            uint8_t epc_len = RFID_EPC_FIXED_WORDS;
            memset(epc_buf, 0, sizeof(epc_buf));
            if (RFID_ReadTag(RFID_BANK_EPC, RFID_EPC_PC_ADDR, 1U, epc_buf) == RFID_RET_OK) {
                uint16_t pc = RFID_ReadU16BE(epc_buf);
                epc_words = (uint16_t)((pc >> 11) & 0x1FU);
                if (epc_words == 0U || epc_words > RFID_EPC_MAX_WORDS) {
                    epc_words = 0;
                }
                memset(epc_buf, 0, epc_len * 2U);
                if (epc_words > 0U) {
                    if (RFID_ReadTag(RFID_BANK_EPC, RFID_EPC_DATA_ADDR, epc_words, epc_buf) != RFID_RET_OK) {
                        epc_status = (rfid_last_error != 0U) ? rfid_last_error : 0xFFU;
                    }
                }
            } else {
                epc_status = (rfid_last_error != 0U) ? rfid_last_error : 0xFFU;
            }
        }

        /* --- 读 USER: 先选标签(用缓存 EPC), 再读 --- */
        if (RFID_PlcEnsureSelected(ant, buf) == RFID_RET_OK) {
            if (RFID_ReadTag(RFID_BANK_USER, user_addr, user_words, buf) != RFID_RET_OK) {
                user_status = (rfid_last_error != 0U) ? rfid_last_error : 0xFFU;
            }
        } else {
            user_status = (rfid_last_error != 0U) ? rfid_last_error : 0xFFU;
        }

        /* --- 写入 TxPDO: [EPC状态][EPC 固定8字][USER状态][USER WORDS字] --- */
        /* 每通道固定长度 = 1 + RFID_EPC_FIXED_WORDS + 1 + user_words, PLC 按固定偏移读 */
        /* EPC 状态字 */
        if (out_word < RFID_CMD_DATA_WORDS) {
            DI(TX_RFID_CMD_DATA + out_word) = epc_status;
            out_word++;
        }
        /* EPC 数据(固定 RFID_EPC_FIXED_WORDS 字, 不足补 0) */
        {
            uint16_t copy_w = epc_words;
            if (copy_w > RFID_EPC_FIXED_WORDS) {
                copy_w = RFID_EPC_FIXED_WORDS;
            }
            for (uint16_t i = 0; i < RFID_EPC_FIXED_WORDS && out_word < RFID_CMD_DATA_WORDS; i++) {
                DI(TX_RFID_CMD_DATA + out_word) = (i < copy_w)
                    ? RFID_PackBytes(epc_buf, (uint16_t)(epc_words * 2U), (uint16_t)(i * 2U))
                    : 0;
                out_word++;
            }
        }
        /* USER 状态字 */
        if (out_word < RFID_CMD_DATA_WORDS) {
            DI(TX_RFID_CMD_DATA + out_word) = user_status;
            out_word++;
        }
        /* USER 数据(成功才有数据, 失败填 0) */
        for (uint16_t i = 0; i < user_words && out_word < RFID_CMD_DATA_WORDS; i++) {
            DI(TX_RFID_CMD_DATA + out_word) = (user_status == 0U)
                ? RFID_PackBytes(buf, user_bytes, (uint16_t)(i * 2U))
                : 0;
            out_word++;
        }
    }

    DI(TX_RFID_CMD_DATA_LEN) = (uint16_t)(out_word * 2U);
    return RFID_RET_OK;
}

/* PLC 命令分发核心。从 RxPDO 读命令执行，结果写 TxPDO。
 * 返回 1=处理中(主循环跳过 RFID_Scan), 0=无命令(继续 Scan)。
 * 边沿触发见文件头说明。
 *
 * s_Armed 状态机(防重复执行):
 *   - CMD 从 0 变非0 且 s_Armed=1 → 执行命令, s_Armed 置 0(锁定)
 *   - CMD 保持非0 且 s_Armed=0   → 跳过(同一命令不重复执行)
 *   - CMD 变回 0                  → s_Armed 置 1(重新武装, 准备下一条)
 * 这样 PLC 每周期都发 RxPDO, 但同一命令只执行一次, 直到 PLC 清零 CMD。 */
uint8_t RFID_EcatCmdTask(void)
{
    static uint8_t s_Armed = 1U;   /* 1=等待新命令, 0=命令锁定中 */
    uint8_t buf[RFID_CMD_DATA_BYTES];
    uint16_t cmd = DO(RX_RFID_CMD);       /* 读取命令码 */
    uint16_t ant = DO(RX_RFID_ANT);       /* 读取天线号 */
    uint16_t addr = DO(RX_RFID_ADDR);     /* 读取地址/参数 */
    uint16_t words = DO(RX_RFID_WORDS);   /* 读取字数/参数 */
    int ret = RFID_RET_FRAME_ERR;

    /* RFID 软复位进行中：模块正在重启, 不发任何阻塞命令, 返回 BUSY 让主循环也跳过扫描。
     * 若此时 PLC 发了命令, 回显命令码并报 BUSY, 让 PLC 知道暂不可用。 */
    if (g_RfidWdTriggered != 0U) {
        if (cmd != RFID_PLC_CMD_NONE) {
            DI(TX_RFID_CMD_ECHO) = cmd;
            DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_BUSY;
            DI(TX_RFID_CMD_RESULT) = 0U;
        }
        return 1U;
    }

    /* 无命令: 保持 IDLE 状态, 重新武装 s_Armed, 主循环继续做自动扫描。
     * 注意: 若上条命令还在 BUSY(理论上不该出现), 不强行覆盖成 IDLE。 */
    if (cmd == RFID_PLC_CMD_NONE) {
        if (DI(TX_RFID_CMD_STATUS) != RFID_PLC_STATUS_BUSY) {
            DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_IDLE;
        }
        s_Armed = 1U;
        return 0U;
    }

    /* 命令锁定中(s_Armed=0): CMD 还没被 PLC 清零, 同一命令不重复执行, 直接返回 */
    if (s_Armed == 0U) {
        return 0U;
    }
    s_Armed = 0U;   /* 锁定, 直到 CMD 清零才重新武装 */

    /* 设置响应头: 回显命令码, 置 BUSY, 清结果码和数据区。
     * PLC 据此知道命令已被接收, 轮询 STATUS 等待 OK/ERROR。 */
    DI(TX_RFID_CMD_ECHO) = cmd;
    DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_BUSY;
    DI(TX_RFID_CMD_RESULT) = 0;
    RFID_ClearCmdDataPdo();

    /* 切换天线(若指定了合法天线号), 等待稳定后再执行命令 */
    if (ant >= 1U && ant <= RFID_ANT_COUNT) {
        RFID_SetAntenna((uint8_t)ant);
        RFID_EcatDelayMs(RFID_ANT_SETTLE_MS);
    }

    /* ---- 命令分发 ---- */
    switch (cmd) {

    case RFID_PLC_CMD_GET_INFO:        /* 读模块信息(硬件/固件版本), WORDS=0硬件/1固件 */
        if (words > 1U) {
            words = 1U;
        }
        ret = RFID_PlcCommandU8(RFID_CMD_READ_INFO, (uint8_t)words, buf);
        break;

    case RFID_PLC_CMD_READ_TID:        /* 读 TID 区(只读), 默认 6 字, 最多 122 字 */
        if (words == 0U) {
            words = 6U;
        }
        if (words > RFID_TID_MAX_WORDS) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            ret = RFID_ReadTag(RFID_BANK_TID, addr, words, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteCmdDataToPdo(buf, (uint16_t)(words * 2U));
            }
        }
        break;

    case RFID_PLC_CMD_READ_USER:       /* 读 USER 区(自动选标签), 默认 6 字, 最多 122 字 */
        if (words == 0U) {
            words = 6U;
        }
        if (words > RFID_USER_MAX_WORDS) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            ret = RFID_PlcEnsureSelected((uint8_t)ant, buf);
            if (ret == RFID_RET_OK) {
                ret = RFID_ReadTag(RFID_BANK_USER, addr, words, buf);
                if (ret == RFID_RET_OK) {
                    RFID_WriteCmdDataToPdo(buf, (uint16_t)(words * 2U));
                }
            }
        }
        break;

    case RFID_PLC_CMD_READ_RFU:     /* 读 RFU 区(密码)，最多 4 字，写禁止 */
        if (words == 0U || words > RFID_RFU_MAX_WORDS || addr >= RFID_RFU_MAX_WORDS) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            ret = RFID_ReadTag(RFID_BANK_RFU, addr, words, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteCmdDataToPdo(buf, (uint16_t)(words * 2U));
            }
        }
        break;

    case RFID_PLC_CMD_READ_EPC_USER_ALL:  /* 一键读三通道 EPC+USER */
        if (words == 0U || words > 32U) {
            ret = RFID_RET_FRAME_ERR;   /* USER 字数 1~32, 保证三通道≤128 字 */
        } else {
            ret = RFID_ReadEpcUserAll(addr, words, buf);
        }
        break;

    case RFID_PLC_CMD_WRITE_USER:      /* 写 USER 区(自动选标签+写后回读校验) */
        if (words == 0U || words > RFID_USER_MAX_WORDS) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint16_t write_bytes = (uint16_t)(words * 2U);
            ret = RFID_PlcEnsureSelected((uint8_t)ant, buf);
            if (ret == RFID_RET_OK) {
                RFID_UnpackPdoBytes(RX_RFID_DATA, write_bytes, buf);
                RFID_WriteCmdDataToPdo(buf, write_bytes);
                ret = RFID_WriteTagChunked(RFID_BANK_USER, addr, words, buf);
                /* 回读校验 */
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

    case RFID_PLC_CMD_WRITE_EPC:
        /* EPC 区前两字 PC/CRC 禁写，addr>=2。总分段<=31字(标准上限 PC 5 位编码)。
         * 单次命令受 RxPDO 数据区 64 字限制。越界(addr+words>33)报错，
         * 标签容量不足由标签返回错误码如实上报。 */
        if (words == 0U || words > RFID_REQ_DATA_WORDS
            || addr < RFID_EPC_DATA_ADDR
            || (uint16_t)(addr + words) > (RFID_EPC_DATA_ADDR + RFID_EPC_MAX_WORDS)) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t write_buf[RFID_REQ_DATA_BYTES];
            uint16_t actual_addr = addr;
            uint16_t write_bytes = (uint16_t)(words * 2U);
            ret = RFID_PlcEnsureSelected((uint8_t)ant, buf);
            if (ret == RFID_RET_OK) {
                RFID_UnpackPdoBytes(RX_RFID_DATA, write_bytes, write_buf);
                RFID_WriteCmdDataToPdo(write_buf, write_bytes);
                ret = RFID_WriteTagChunked(RFID_BANK_EPC, actual_addr, words, write_buf);
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

    case RFID_PLC_CMD_SET_EPC_LEN:    /* 改 EPC 长度 = 改 PC 字高 5 位，1~31 字 */
        if (words == 0U || words > RFID_EPC_MAX_WORDS) {
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

    case RFID_PLC_CMD_SET_POWER:       /* 设置发射功率, WORDS=1~33 dBm 或 >33 原始值 */
        {
            uint16_t pwr = RFID_PlcPowerParam(words);
            ret = RFID_PlcCommandStatusU16(RFID_CMD_SET_POWER, pwr, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, pwr);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_POWER:       /* 读取发射功率 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_POWER, buf);
        break;

    case RFID_PLC_CMD_SET_REGION:      /* 设置工作区域, WORDS=区域编码 */
        if (words > 0xFFU) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t region = (uint8_t)words;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_REGION, region, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)region);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_REGION:      /* 读取工作区域 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_REGION, buf);
        break;

    case RFID_PLC_CMD_SET_CHANNEL:     /* 设置固定信道, WORDS=信道号 */
        if (words > 0xFFU) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t ch = (uint8_t)words;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_CHANNEL, ch, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)ch);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_CHANNEL:     /* 读取当前信道 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_CHANNEL, buf);
        break;

    case RFID_PLC_CMD_SET_HOP:         /* 设置跳频模式, WORDS=0关/非0开 */
        {
            uint8_t hop = (words == 0U) ? 0x00U : 0xFFU;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_HOP, hop, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)hop);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_HOP:         /* 读取跳频模式 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_HOP, buf);
        break;

    case RFID_PLC_CMD_SET_MODE:        /* 设置工作模式, WORDS=0正常/1密集 */
        if (words > 1U) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint8_t mode = (uint8_t)words;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_MODE, mode, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)mode);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_MODE:        /* 读取工作模式 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_MODE, buf);
        break;

    case RFID_PLC_CMD_INVENTORY:       /* 单次盘点, 返回 RSSI+PC+EPC */
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

    case RFID_PLC_CMD_STOP_POLL:       /* 停止连续盘点 */
        ret = RFID_PlcCommandStatusOnly(RFID_CMD_STOP_POLL, NULL, 0U, buf);
        break;

    case RFID_PLC_CMD_SET_CH_LIST:     /* 设置跳频信道列表, WORDS=数量+WRITE_DATA=信道 */
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

    case RFID_PLC_CMD_GET_CH_LIST:     /* 读取跳频信道列表 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_CH_LIST, buf);
        break;

    case RFID_PLC_CMD_SET_RX:          /* 设置接收灵敏度, WRITE_DATA=4字节参数 */
        {
            uint8_t rx_params[4];
            RFID_UnpackPdoBytes(RX_RFID_DATA, 4U, rx_params);
            ret = RFID_PlcCommandStatusOnly(RFID_CMD_SET_RX, rx_params, 4U, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteCmdDataToPdo(rx_params, 4U);
            }
        }
        break;

    case RFID_PLC_CMD_GET_RX:          /* 读取接收灵敏度 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_GET_RX, buf);
        break;

    case RFID_PLC_CMD_TEST_RSSI:       /* RSSI 测试 */
        ret = RFID_PlcCommandNoParam(RFID_CMD_TEST_RSSI, buf);
        break;

    case RFID_PLC_CMD_SET_CARRIER:     /* 载波开关, WORDS=0关/非0开 */
        {
            uint8_t carrier = (words == 0U) ? 0x00U : 0xFFU;
            ret = RFID_PlcCommandStatusU8(RFID_CMD_SET_CARRIER, carrier, buf);
            if (ret == RFID_RET_OK) {
                RFID_WriteU16BE(buf, (uint16_t)carrier);
                RFID_WriteCmdDataToPdo(buf, 2U);
            }
        }
        break;

    case RFID_PLC_CMD_RESET:           /* 软重置 RFID 模块(非阻塞, 不等响应) */
        RFID_Reset();
        ret = RFID_RET_OK;
        break;

    case RFID_PLC_CMD_SELECT_EPC:      /* 选择指定 EPC 标签, WORDS=0用缓存/>0用WRITE_DATA */
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

    case RFID_PLC_CMD_RAW:            /* 原始命令直发模块, ADDR=指令码, WORDS=payload字节 */
        if (addr > 0xFFU || words > RFID_REQ_DATA_BYTES) {
            ret = RFID_RET_FRAME_ERR;
        } else {
            uint16_t rsp_len = RFID_CMD_DATA_BYTES;
            RFID_UnpackPdoBytes(RX_RFID_DATA, words, buf);
            ret = RFID_Command((uint8_t)addr, buf, words, buf, &rsp_len, RFID_RAW_TIMEOUT_MS);
            RFID_WriteCmdDataToPdo(buf, rsp_len);
        }
        break;

    default:                           /* 未知命令码 */
        ret = RFID_RET_FRAME_ERR;
        break;
    }

    /* ---- 结果写 TxPDO ----
     * 命令执行完毕, 根据 ret 更新 STATUS/RESULT:
     *   成功: STATUS=OK, RESULT=0
     *   失败: STATUS=ERROR, RESULT 优先用模块错误码(rfid_last_error),
     *         没有模块错误码时用驱动错误码取反(-ret, 如 FRAME_ERR=-3 → 3) */
    if (ret == RFID_RET_OK) {
        DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_OK;
        DI(TX_RFID_CMD_RESULT) = 0;
    } else {
        DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_ERROR;
        DI(TX_RFID_CMD_RESULT) = (uint16_t)((rfid_last_error != 0U) ? rfid_last_error : (uint16_t)(-ret));
    }
    return 1U;   /* 命令处理中, 主循环跳过 RFID_Scan */
}

/* 周期任务(每 PDO 周期一次)。递增心跳 → DI(0)，映射 3 天线数据到 TxPDO。 */
void APPL_UpdateTxPdo(void)
{
    static uint16_t s_Counter = 0;
    static const uint16_t rfid_pdo_base[RFID_ANT_COUNT] = {
        TX_RFID1_RSSI,
        TX_RFID2_RSSI,
        TX_RFID3_RSSI
    };

    s_Counter++;
    DI(0) = s_Counter;

    for (uint8_t ant = 0; ant < RFID_ANT_COUNT; ant++) {
        RFID_MapAntennaToPdo(ant, rfid_pdo_base[ant]);
    }
}

/* 安全输出 (进入 SAFE-OP 时调用)。当前无需求，保留空实现。 */
void APPL_SafeOutput(void)
{
}
