/*
 * ecat_api.h — EtherCAT 从站 API 接口层
 *
 * 本文件是应用层与 SSC (EtherCAT Slave Stack Code) 之间的桥梁。
 * 定义了 PDO 数据布局、命令接口、状态查询函数等。
 *
 * 核心概念：
 *   - DI(n) = TxPDO，从站→主站的数据（Data Input，站在主站角度是输入）
 *   - DO(n) = RxPDO，主站→从站的数据（Data Output，站在主站角度是输出）
 *   - n 是 UINT16 字索引，每个索引对应 2 字节
 *
 * PDO 布局总览：
 *   TxPDO = 510 个 UINT16 = 1020 字节
 *   RxPDO = 510 个 UINT16 = 1020 字节
 *
 *   TxPDO (DI) — 从站→主站:
 *     DI(0)        : 心跳计数器
 *     DI(1..34)    : 天线1 — RSSI, EPC长度, 新标签标志, EPC数据(31字=62字节)
 *     DI(35..68)   : 天线2 — 同上格式
 *     DI(69..102)  : 天线3 — 同上格式
 *     DI(103..138) : 命令响应 — 回显命令码, 状态, 结果, 数据长度, 数据(32字=64字节)
 *
 *   RxPDO (DO) — 主站→从站:
 *     DO(0)        : 命令码 (0=空闲, 1..31=RFID命令, 100=原始命令)
 *     DO(1)        : 天线编号 (1..3, 0=保持当前)
 *     DO(2)        : 字地址 / 命令参数
 *     DO(3)        : 字数 / 命令参数
 *     DO(4..33)    : 写入数据 (30字=60字节)
 */
#ifndef _ECAT_API_H_
#define _ECAT_API_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PDO 大小配置
 * TxPDO = 510 个 UINT16 = 1020 字节
 * RxPDO = 510 个 UINT16 = 1020 字节
 * 实际 ESC 支持最大 0x400 (1024) 字节 */
#define PDO_TX_UINT16    510     /* TxPDO 字数 (UINT16) */
#define PDO_RX_UINT16    510     /* RxPDO 字数 (UINT16) */
#define PDO_TX_MAX       0x400   /* ESC TxPDO 最大字节数 */
#define PDO_RX_MAX       0x400   /* ESC RxPDO 最大字节数 */

/*
 * 统一 PDO 访问宏
 *
 * DI(n) — 读/写 TxPDO (从站→主站)，n 为 UINT16 字索引
 * DO(n) — 读   RxPDO (主站→从站)，n 为 UINT16 字索引
 *
 * 内部实现：通过 DIUnit/DOUnit 结构体的内存地址直接访问，
 * 避开了 SSC 的 ObjDict 索引查找，速度快、使用简单。
 */
#define _DI_BASE         ((uint16_t*)(&DIUnit10x6000.u16SubIndex0 + 1))
#define _DO_BASE         ((uint16_t*)(&DOUnit20x7000.u16SubIndex0 + 1))
#define DI(n)            (_DI_BASE[(n)])
#define DO(n)            (_DO_BASE[(n)])

/* ============================================================
 * TxPDO (DI) 布局 — 从站发送给主站的数据
 * ============================================================ */

#define TX_HEARTBEAT     0   /* 心跳计数器，每个 PDO 周期 +1，主站用来检测通信是否正常 */

/* ---------- 天线1: DI(1..34) ---------- */
#define TX_RFID1_RSSI    1   /* 标签信号强度 (0~255) */
#define TX_RFID1_EPC_LEN 2   /* EPC 码长度（字节数） */
#define TX_RFID1_NEW     3   /* 新标签标志: 1=有新标签, 0=无变化 */
#define TX_RFID1_EPC     4   /* EPC 数据起始，占 31 个 UINT16 = 62 字节 */

/* ---------- 天线2: DI(35..68) ---------- */
#define TX_RFID2_RSSI    35  /* 标签信号强度 */
#define TX_RFID2_EPC_LEN 36  /* EPC 码长度 */
#define TX_RFID2_NEW     37  /* 新标签标志 */
#define TX_RFID2_EPC     38  /* EPC 数据起始 */

/* ---------- 天线3: DI(69..102) ---------- */
#define TX_RFID3_RSSI    69  /* 标签信号强度 */
#define TX_RFID3_EPC_LEN 70  /* EPC 码长度 */
#define TX_RFID3_NEW     71  /* 新标签标志 */
#define TX_RFID3_EPC     72  /* EPC 数据起始 */

/* ---------- 命令响应: DI(103..138) ---------- */
#define TX_RFID_CMD_ECHO     103 /* 回显的命令码，与 RxPDO 中的命令码一致 */
#define TX_RFID_CMD_STATUS   104 /* 命令执行状态: 0=空闲, 1=执行中, 2=成功, 3=错误 */
#define TX_RFID_CMD_RESULT   105 /* 命令结果码: 0=成功, 非零=驱动错误码或模块错误码 */
#define TX_RFID_CMD_DATA_LEN 106 /* 响应数据长度（字节数） */
#define TX_RFID_CMD_DATA     107 /* 响应数据起始，占 32 个 UINT16 = 64 字节 */

/* ============================================================
 * RxPDO (DO) 布局 — 主站发送给从站的数据
 * ============================================================ */

#define RX_RFID_CMD      0   /* RFID 命令码: 0=无命令, 1..31=PLC命令, 100=原始EC-UHF-B命令 */
#define RX_RFID_ANT      1   /* 天线选择: 1/2/3, 0=保持当前天线 */
#define RX_RFID_ADDR     2   /* 字地址或命令参数（如设置功率值） */
#define RX_RFID_WORDS    3   /* 读写字数或命令参数 */
#define RX_RFID_DATA     4   /* 写入数据起始，占 30 个 UINT16 = 60 字节 */

/* ============================================================
 * PLC 命令码定义 — 写入 RxPDO DO(0) 触发命令
 *
 * 命令流程（边沿触发）：
 *   1. PLC 把命令码写入 DO(RX_RFID_CMD)（从 0 变为非零）
 *   2. 从站检测到命令后执行，TX_RFID_CMD_STATUS 设为 BUSY
 *   3. 执行完毕后 STATUS 变为 OK 或 ERROR，RESULT 写入结果码
 *   4. PLC 把 DO(RX_RFID_CMD) 清零（下降沿），从站重新武装
 *   5. PLC 可以发送下一个命令
 * ============================================================ */
#define RFID_PLC_CMD_NONE           0   /* 无命令 / 空闲 */
#define RFID_PLC_CMD_GET_INFO       1   /* 读取模块信息 (固件版本等) */
#define RFID_PLC_CMD_READ_TID       2   /* 读标签 TID 区 */
#define RFID_PLC_CMD_READ_USER      3   /* 读标签用户区 */
#define RFID_PLC_CMD_WRITE_USER     4   /* 写标签用户区 */
#define RFID_PLC_CMD_WRITE_EPC      5   /* 写标签 EPC 区（自动选标签）。
                                         * addr 必须 >= 2（EPC 数据起始字），
                                         * EPC 区前两字为 PC(1)/CRC(1) 禁止写；
                                         * addr<2 视为参数错误。单次最多 15 字。 */
#define RFID_PLC_CMD_SET_EPC_LEN    6   /* 设置 EPC 长度（修改 PC 字） */
#define RFID_PLC_CMD_SET_POWER      7   /* 设置发射功率 */
#define RFID_PLC_CMD_GET_POWER      8   /* 读取当前发射功率 */
#define RFID_PLC_CMD_SET_REGION     9   /* 设置工作区域 (0=中国, 1=美国, ...) */
#define RFID_PLC_CMD_GET_REGION     10  /* 读取当前工作区域 */
#define RFID_PLC_CMD_SET_CHANNEL    11  /* 设置固定信道 */
#define RFID_PLC_CMD_GET_CHANNEL    12  /* 读取当前信道 */
#define RFID_PLC_CMD_SET_HOP        13  /* 设置跳频模式: 0=关闭, 非0=开启 */
#define RFID_PLC_CMD_GET_HOP        14  /* 读取跳频模式 */
#define RFID_PLC_CMD_SET_MODE       15  /* 设置工作模式: 0=密集读取, 1=正常 */
#define RFID_PLC_CMD_GET_MODE       16  /* 读取工作模式 */
#define RFID_PLC_CMD_INVENTORY      17  /* 单次盘点（返回 RSSI+PC+EPC） */
#define RFID_PLC_CMD_STOP_POLL      18  /* 停止自动轮询 */
#define RFID_PLC_CMD_SET_CH_LIST    19  /* 设置跳频信道列表 */
#define RFID_PLC_CMD_GET_CH_LIST    20  /* 读取跳频信道列表 */
#define RFID_PLC_CMD_SET_RX         21  /* 设置接收灵敏度参数 */
#define RFID_PLC_CMD_GET_RX         22  /* 读取接收灵敏度参数 */
#define RFID_PLC_CMD_TEST_RSSI      23  /* RSSI 测试 */
#define RFID_PLC_CMD_SET_CARRIER    24  /* 设置载波: 0=关闭, 非0=开启 */
#define RFID_PLC_CMD_RESET          25  /* 软重置 RFID 模块 */
#define RFID_PLC_CMD_SELECT_EPC     30  /* 选择指定 EPC 的标签（用于后续读/写） */
#define RFID_PLC_CMD_CLEAR_SELECT   31  /* 清除标签选择 */
#define RFID_PLC_CMD_RAW            100 /* 原始命令：直接发送自定义指令给 RFID 模块 */

/* 命令执行状态码 — 写入 DI(TX_RFID_CMD_STATUS) */
#define RFID_PLC_STATUS_IDLE    0   /* 空闲：没有命令在执行 */
#define RFID_PLC_STATUS_BUSY    1   /* 执行中：命令正在处理 */
#define RFID_PLC_STATUS_OK      2   /* 成功：命令执行完成 */
#define RFID_PLC_STATUS_ERROR   3   /* 错误：命令执行失败 */

/* RFID 看门狗超时错误码 */
#define RFID_ERR_WD_TIMEOUT    0xE001U  /* RFID 模块 2 秒无响应，看门狗触发 */

/*
 * 持久化参数结构体 — 保存到 Flash 的配置
 * 主站可通过 CoE (CAN over EtherCAT) 读写这些参数
 */
typedef struct {
    uint16_t modbus_slave_addr;    /* Modbus 从站地址（历史遗留，未使用） */
    uint16_t baudrate_code;        /* 波特率编码（历史遗留，未使用） */
    uint32_t reserved[8];          /* 保留字段 */
    uint32_t checksum;             /* 校验和 */
} PersistentParams_t;

extern PersistentParams_t g_PersistentParams;

/* ============================================================
 * EtherCAT 协议栈核心函数
 * ============================================================ */

/* 初始化 EtherCAT 协议栈（ESC、SSC、对象字典） */
void ECAT_Stack_Init(void);

/* 驱动 SSC 协议栈主循环（处理 AL 状态机、PDO 映射、邮箱等）
 * 必须在主 while(1) 中高频调用，否则 EtherCAT 通信会中断 */
void ECAT_Stack_MainLoop(void);

/* 阻塞期间保活函数 = ECAT_Stack_MainLoop() + WDG_Feed()
 * 用于 RFID 命令阻塞等待期间，同时驱动协议栈和喂硬件看门狗，
 * 防止 SM 看门狗或 FWDGT 超时导致 EtherCAT 掉线 */
void ECAT_KeepAlive(void);

/* ============================================================
 * 回调注册 — 在 main() 中调用，注册应用层回调函数
 * ============================================================ */

typedef void (*ecat_periodic_cb_t)(void);
/* 注册周期性任务回调（如 LED 控制、PDO 数据更新）
 * 在每个 PDO 输入映射周期内被调用 */
void ECAT_RegisterPeriodicTask(ecat_periodic_cb_t cb);

typedef void (*ecat_safe_output_cb_t)(void);
/* 注册安全输出回调 — EtherCAT 进入 SAFE-OP 时调用，用于设置输出为安全值 */
void ECAT_RegisterSafeOutput(ecat_safe_output_cb_t cb);

/* ============================================================
 * 持久化参数 — Flash 读写
 * ============================================================ */

uint32_t ECAT_GetParamSize(void);    /* 获取持久化参数结构体大小 */
void*    ECAT_GetParamPtr(void);     /* 获取持久化参数结构体指针 */
void     ECAT_SetParamDirty(void);   /* 标记参数已修改，待写入 Flash */

/* ============================================================
 * 状态查询函数 — 用于调试和监控
 * ============================================================ */

uint8_t  ECAT_GetAlState(void);       /* 读取 AL 状态: 0x01=INIT, 0x02=PRE-OP, 0x04=SAFE-OP, 0x08=OP */
uint16_t ECAT_GetAlStatusCode(void);  /* 读取 AL 状态码（错误码），0=无错误 */
uint16_t ECAT_GetLocalErrorCode(void); /* 读取本地错误码 */

/* ============================================================
 * SSC 内部桥接函数 — SSC 协议栈通过函数指针调用
 * ============================================================ */

extern void (*g_pfnPeriodicTask)(void);           /* 周期性任务回调 */
extern void (*g_pfnSafeOutput)(void);             /* 安全输出回调 */
extern void (*g_pfnTxPdoMapping)(uint16_t* pData); /* TxPDO 映射回调 */
extern void (*g_pfnRxPdoMapping)(uint16_t* pData); /* RxPDO 映射回调 */

/* CoE PDO 映射函数 — SSC 在 PDO 交换时调用
 * 把 DIUnit/DOUnit 结构体数据拷贝到 ESC 的 PDO 缓冲区 */
void APPL_CoeTxPdoMapping(uint16_t* pData);  /* TxPDO: 应用数据 → ESC */
void APPL_CoeRxPdoMapping(uint16_t* pData);  /* RxPDO: ESC → 应用数据 */

#ifdef __cplusplus
}
#endif

#endif /* _ECAT_API_H_ */
