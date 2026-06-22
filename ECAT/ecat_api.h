/*
 * ecat_api.h — EtherCAT 从站 API 接口层
 *
 * 应用层与 SSC 的桥梁，定义 PDO 数据布局、命令接口、状态查询。
 *
 *   DI(n) = TxPDO (从站→主站), DO(n) = RxPDO (主站→从站)，n 为 UINT16 字索引(2字节)。
 *
 * PDO 布局 (各 510 个 UINT16 = 1020 字节):
 *   TxPDO: DI(0)=心跳, DI(1..34/35..68/69..102)=天线1/2/3(RSSI,EPC_LEN,NEW,EPC×31字),
 *          DI(103..138)=命令响应(ECHO,STATUS,RESULT,LEN,DATA×32字)
 *   RxPDO: DO(0)=命令码, DO(1)=天线, DO(2)=地址/参数, DO(3)=字数/参数, DO(4..33)=写入数据(30字)
 */
#ifndef _ECAT_API_H_
#define _ECAT_API_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PDO 大小 (ESC 最大 0x400=1024 字节) */
#define PDO_TX_UINT16    510
#define PDO_RX_UINT16    510
#define PDO_TX_MAX       0x400
#define PDO_RX_MAX       0x400

/* 应用层看门狗超时(ms)。主站停发 PDO 超此时间调安全输出回调。
 * 0=禁用(依赖 ESC SM 看门狗 + ECAT_IGNORE_SM_WD_ERROR)。
 * 当前 APPL_SafeOutput 为空实现，保持禁用。 */
#define ECAT_WD_TIMEOUT_MS    0U

/* 统一 PDO 访问宏。直接访问 DIUnit/DOUnit 内存，避开 ObjDict 索引查找。 */
#define _DI_BASE         ((uint16_t*)(&DIUnit10x6000.u16SubIndex0 + 1))
#define _DO_BASE         ((uint16_t*)(&DOUnit20x7000.u16SubIndex0 + 1))
#define DI(n)            (_DI_BASE[(n)])
#define DO(n)            (_DO_BASE[(n)])

/* ---- TxPDO (DI) 布局 — 从站→主站 ---- */

#define TX_HEARTBEAT     0   /* 心跳，每 PDO 周期 +1 */

/* 天线1: DI(1..34) */
#define TX_RFID1_RSSI    1
#define TX_RFID1_EPC_LEN 2
#define TX_RFID1_NEW     3   /* 1=有新标签 */
#define TX_RFID1_EPC     4   /* EPC 起始, 31 字 */

/* 天线2: DI(35..68) */
#define TX_RFID2_RSSI    35
#define TX_RFID2_EPC_LEN 36
#define TX_RFID2_NEW     37
#define TX_RFID2_EPC     38

/* 天线3: DI(69..102) */
#define TX_RFID3_RSSI    69
#define TX_RFID3_EPC_LEN 70
#define TX_RFID3_NEW     71
#define TX_RFID3_EPC     72

/* 命令响应: DI(103..138) */
#define TX_RFID_CMD_ECHO     103 /* 回显命令码 */
#define TX_RFID_CMD_STATUS   104 /* 0=空闲 1=执行中 2=成功 3=错误 */
#define TX_RFID_CMD_RESULT   105 /* 0=成功, 非零=错误码 */
#define TX_RFID_CMD_DATA_LEN 106 /* 响应数据长度(字节) */
#define TX_RFID_CMD_DATA     107 /* 响应数据, 32 字 */

/* ---- RxPDO (DO) 布局 — 主站→从站 ---- */

#define RX_RFID_CMD      0   /* 命令码: 0=空闲, 1..31=PLC命令, 100=原始命令 */
#define RX_RFID_ANT      1   /* 天线: 1/2/3, 0=保持当前 */
#define RX_RFID_ADDR     2   /* 字地址或命令参数 */
#define RX_RFID_WORDS    3   /* 读写字数或命令参数 */
#define RX_RFID_DATA     4   /* 写入数据, 30 字 */

/* ---- PLC 命令码 — 写 DO(0) 触发，边沿触发(见 Application.c 文件头) ---- */

#define RFID_PLC_CMD_NONE           0
#define RFID_PLC_CMD_GET_INFO       1   /* 读模块信息(固件版本) */
#define RFID_PLC_CMD_READ_TID       2   /* 读 TID 区 */
#define RFID_PLC_CMD_READ_USER      3   /* 读用户区 */
#define RFID_PLC_CMD_WRITE_USER     4   /* 写用户区 */
#define RFID_PLC_CMD_WRITE_EPC      5   /* 写 EPC 区(自动选标签)。addr>=2，前两字 PC/CRC 禁写；单次<=15字 */
#define RFID_PLC_CMD_SET_EPC_LEN    6   /* 改 EPC 长度(改 PC 字) */
#define RFID_PLC_CMD_SET_POWER      7
#define RFID_PLC_CMD_GET_POWER      8
#define RFID_PLC_CMD_SET_REGION     9
#define RFID_PLC_CMD_GET_REGION     10
#define RFID_PLC_CMD_SET_CHANNEL    11
#define RFID_PLC_CMD_GET_CHANNEL    12
#define RFID_PLC_CMD_SET_HOP        13  /* 0=关, 非0=开 */
#define RFID_PLC_CMD_GET_HOP        14
#define RFID_PLC_CMD_SET_MODE       15  /* 0=密集读取, 1=正常 */
#define RFID_PLC_CMD_GET_MODE       16
#define RFID_PLC_CMD_INVENTORY      17  /* 单次盘点(返回 RSSI+PC+EPC) */
#define RFID_PLC_CMD_STOP_POLL      18
#define RFID_PLC_CMD_SET_CH_LIST    19
#define RFID_PLC_CMD_GET_CH_LIST    20
#define RFID_PLC_CMD_SET_RX         21
#define RFID_PLC_CMD_GET_RX         22
#define RFID_PLC_CMD_TEST_RSSI      23
#define RFID_PLC_CMD_SET_CARRIER    24  /* 0=关, 非0=开 */
#define RFID_PLC_CMD_RESET          25  /* 软重置 RFID 模块 */
#define RFID_PLC_CMD_SELECT_EPC     30  /* 选择指定 EPC 标签 */
#define RFID_PLC_CMD_CLEAR_SELECT   31
#define RFID_PLC_CMD_RAW            100 /* 原始命令直发模块 */

/* 命令状态码 — DI(TX_RFID_CMD_STATUS) */
#define RFID_PLC_STATUS_IDLE    0
#define RFID_PLC_STATUS_BUSY    1
#define RFID_PLC_STATUS_OK      2
#define RFID_PLC_STATUS_ERROR   3

#define RFID_ERR_WD_TIMEOUT    0xE001U  /* RFID 模块 2s 无响应，看门狗触发 */

/* 持久化参数 — 保存到 Flash，主站可经 CoE 读写 */
typedef struct {
    uint16_t modbus_slave_addr;    /* 历史遗留，未使用 */
    uint16_t baudrate_code;        /* 历史遗留，未使用 */
    uint32_t reserved[8];
    uint32_t checksum;
} PersistentParams_t;

extern PersistentParams_t g_PersistentParams;

/* ---- 协议栈核心 ---- */

void ECAT_Stack_Init(void);       /* 初始化协议栈(ESC、SSC、对象字典) */
void ECAT_Stack_MainLoop(void);   /* 驱动主循环，须高频调用，否则通信中断 */
void ECAT_KeepAlive(void);        /* = MainLoop + WDG_Feed，RFID 阻塞期间保活 */

/* ---- 回调注册 — main() 中调用 ---- */

typedef void (*ecat_periodic_cb_t)(void);
void ECAT_RegisterPeriodicTask(ecat_periodic_cb_t cb);  /* 每 PDO 周期调一次 */

typedef void (*ecat_safe_output_cb_t)(void);
void ECAT_RegisterSafeOutput(ecat_safe_output_cb_t cb);  /* 进入 SAFE-OP 时调 */

/* ---- 持久化参数 ---- */

uint32_t ECAT_GetParamSize(void);
void*    ECAT_GetParamPtr(void);
void     ECAT_SetParamDirty(void);   /* 标记待写入 Flash */

/* ---- 状态查询 — 调试监控用 ---- */

uint8_t  ECAT_GetAlState(void);        /* 0x01=INIT 0x02=PRE-OP 0x04=SAFE-OP 0x08=OP */
uint16_t ECAT_GetAlStatusCode(void);   /* 错误码, 0=无错误 */
uint16_t ECAT_GetLocalErrorCode(void);

/* ---- SSC 桥接 — SSC 经函数指针调用应用层 ---- */

extern void (*g_pfnPeriodicTask)(void);
extern void (*g_pfnSafeOutput)(void);
extern void (*g_pfnTxPdoMapping)(uint16_t* pData);
extern void (*g_pfnRxPdoMapping)(uint16_t* pData);

/* CoE PDO 映射 — SSC 在 PDO 交换时调用 */
void APPL_CoeTxPdoMapping(uint16_t* pData);  /* TxPDO: 应用 → ESC */
void APPL_CoeRxPdoMapping(uint16_t* pData);  /* RxPDO: ESC → 应用 */

#ifdef __cplusplus
}
#endif

#endif /* _ECAT_API_H_ */
