/*
 * rfid_ecat.h — RFID 模块驱动接口
 *
 * USART0 (PA9=TX, PA10=RX) 115200bps 8N1 与 RFID 模块通信。
 *   PA12  模块使能 (HIGH=启用)
 *   PB14/PB13/PB12 天线选择 (S1/S2/S3)
 *
 * 协议 (EC-UHF-B): 0xAA + Dir(1B) + Cmd(1B) + Len(2B,大端) + Data(NB) + CS(1B) + 0xDD
 *   Dir: 0x00=主机→模块, 0x01=模块→主机, 0x02=模块主动通知
 *
 * 收发: TX 走 DMA0_CH3 单次; RX 走 DMA0_CH4 循环 + 软件帧检测。
 *
 * 两级看门狗: 软件看门狗(本文件,2s 无响应→软重置) + 硬件看门狗(wdg_ecat.c,5s→芯片复位)。
 */
#ifndef __RFID_ECAT_H
#define __RFID_ECAT_H

#include "gd32f10x.h"
#include <stdint.h>

/* ---- 硬件引脚 ---- */

#define RFID_USART           USART0
#define RFID_USART_CLK       RCU_USART0
#define RFID_TX_PORT         GPIOA
#define RFID_TX_PIN          GPIO_PIN_9
#define RFID_RX_PORT         GPIOA
#define RFID_RX_PIN          GPIO_PIN_10

#define RFID_EN_PORT         GPIOA
#define RFID_EN_PIN          GPIO_PIN_12
#define RFID_EN_ON()         gpio_bit_set(RFID_EN_PORT, RFID_EN_PIN)
#define RFID_EN_OFF()        gpio_bit_reset(RFID_EN_PORT, RFID_EN_PIN)

/* 天线选择 PB14=S1(LSB) PB13=S2 PB12=S3, 000=天线1..111=天线8, 当前用 ANT1~3 */
#define RFID_ANT_PORT        GPIOB
#define RFID_ANT_S1          GPIO_PIN_14
#define RFID_ANT_S2          GPIO_PIN_13
#define RFID_ANT_S3          GPIO_PIN_12

/* ---- 帧协议常量 ---- */

#define RFID_HEADER          0xAA
#define RFID_FOOTER          0xDD
#define RFID_DIR_HOST         0x00   /* 主机→模块 */
#define RFID_DIR_MODULE       0x01   /* 模块→主机(响应) */
#define RFID_DIR_NOTIFY       0x02   /* 模块主动通知 */

/* ---- RFID 模块命令码 ---- */

#define RFID_CMD_READ_INFO    0x03
#define RFID_CMD_RESET        0x19
#define RFID_CMD_INVENTORY    0x22
#define RFID_CMD_MULTI_POLL   0x27
#define RFID_CMD_STOP_POLL    0x28

#define RFID_CMD_SET_SELECT   0x0C
#define RFID_CMD_GET_SELECT   0x0B
#define RFID_CMD_GET_QUERY    0x0D
#define RFID_CMD_SET_QUERY    0x0E
#define RFID_CMD_SET_SEL_MODE 0x12

#define RFID_CMD_SET_REGION   0x07
#define RFID_CMD_GET_REGION   0x08
#define RFID_CMD_GET_CHANNEL  0xAA
#define RFID_CMD_SET_CHANNEL  0xAB
#define RFID_CMD_GET_HOP      0xAC
#define RFID_CMD_SET_HOP      0xAD
#define RFID_CMD_GET_CH_LIST  0xA8
#define RFID_CMD_SET_CH_LIST  0xA9
#define RFID_CMD_SET_CARRIER  0xB0

#define RFID_CMD_SET_POWER    0xB6   /* 单位 10dBm */
#define RFID_CMD_GET_POWER    0xB7
#define RFID_CMD_SET_RX       0xF0
#define RFID_CMD_GET_RX       0xF1
#define RFID_CMD_TEST_RSSI    0xF3
#define RFID_CMD_SET_MODE     0xF5   /* 0=密集, 1=正常 */
#define RFID_CMD_GET_MODE     0xF6

#define RFID_CMD_READ_TAG     0x39
#define RFID_CMD_WRITE_TAG    0x49
#define RFID_CMD_SET_IO       0x1A
#define RFID_CMD_ERROR        0xFF   /* 模块错误响应 */

/* ---- 驱动返回码 ---- */

#define RFID_RET_OK            0
#define RFID_RET_TIMEOUT      -1     /* 模块无响应 */
#define RFID_RET_MODULE_ERR   -2     /* 模块返回错误码(见 rfid_last_error) */
#define RFID_RET_FRAME_ERR    -3     /* 帧格式错误(校验和/长度/方向) */
#define RFID_RET_MODULE_RSP   -4     /* 模块返回非预期响应(命令码不匹配) */

#define RFID_ERR_NO_TAG        0x15  /* 无标签 */
#define RFID_RSP_NO_TAG        0xF5  /* 盘点无标签(旧版协议) */

/* ---- 标签存储区 (ISO 18000-6C) ---- */

#define RFID_BANK_RFU         0x00
#define RFID_BANK_EPC         0x01   /* EPC 区(含 PC + EPC) */
#define RFID_BANK_TID         0x02   /* TID 区(唯一标识,只读) */
#define RFID_BANK_USER        0x03   /* 用户区(可读可写) */

/* ---- 缓冲区大小 ---- */

#define RFID_RX_BUF_SIZE      256
#define RFID_TX_BUF_SIZE      256
#define RFID_EPC_MAX_LEN      62
#define RFID_TAG_MAX_WORDS    128U   /* 单次读写最大字数 (受 PDO 数据区限制, 128字=256字节) */

/* ---- 标签信息 (Inventory 返回) ---- */
typedef struct {
    uint8_t  epc_len;
    uint8_t  epc[RFID_EPC_MAX_LEN];
    uint8_t  rssi;
    uint16_t pc;
} rfid_tag_t;

/* ---- 阻塞式 API ----
 * 这些函数阻塞等待模块响应(最长 500ms)，期间内部调 ECAT_KeepAlive 保活。 */

void RFID_Init(void);                                   /* 初始化(USART0+DMA+GPIO) */
void RFID_SetAntenna(uint8_t ant);                      /* 切天线 1~3 */
int  RFID_Reset(void);                                  /* 软重置(非阻塞) */
int  RFID_Inventory(rfid_tag_t *tag);                   /* 单次盘点, 100ms 超时 */
int  RFID_ReadTag(uint8_t bank, uint16_t addr, uint16_t len_words, uint8_t *data);
int  RFID_WriteTag(uint8_t bank, uint16_t addr, uint16_t len_words, const uint8_t *data);
int  RFID_Command(uint8_t cmd_id, const uint8_t *tx_data, uint16_t tx_len,
                  uint8_t *rx_data, uint16_t *rx_len, uint32_t timeout_ms);

/* ---- 非阻塞 API (备用) ----
 * StartCmd 发起，Poll 轮询，ParseTag 解析。当前主用阻塞式。 */

void RFID_StartCmd(uint8_t cmd_id);
int  RFID_Poll(void);                /* 0=等待中, >0=收到帧, <0=错误 */
int  RFID_ParseTag(rfid_tag_t *tag);
void RFID_UART_IRQHandler(void);     /* USART0 中断: 清错误标志 */

void RFID_Scan(void);                /* OP 状态下主循环调用, 轮扫 3 天线 */

/* ---- 天线标签数据 (RFID_Scan 更新, APPL_UpdateTxPdo 映射到 TxPDO) ---- */

#define RFID_ANT_COUNT    3U
#define RFID_EPC_BYTES    62U

extern uint8_t g_RfidRssi  [RFID_ANT_COUNT];
extern uint8_t g_RfidEpcLen[RFID_ANT_COUNT];
extern uint8_t g_RfidNew   [RFID_ANT_COUNT];
extern uint8_t g_RfidEpc   [RFID_ANT_COUNT][RFID_EPC_BYTES];

/* ---- 软件看门狗 ----
 * g_RfidLastAliveMs 记录最后一次收到模块响应(完整帧)的时间戳。
 * 模块回了话(含"无标签"错误码)即视为活着；超过 2s 完全无响应则触发复位。
 * 复位分两步(非阻塞): 触发(PA12 断电+RESET 命令) → 到时恢复(上电+重启接收)。
 * 必须在 OP 门控外调用，模块随时可能卡死。 */

#ifndef RFID_SOFT_WD_ENABLE
#define RFID_SOFT_WD_ENABLE    1U
#endif

#define RFID_WD_TIMEOUT_MS     2000U   /* 超时阈值 */
#define RFID_WD_POWER_OFF_MS   200U    /* PA12 断电时长 */

extern volatile uint32_t g_RfidLastAliveMs;
extern volatile uint8_t  g_RfidWdTriggered;   /* 1=正在复位恢复 */

uint8_t RFID_WatchdogCheck(void);   /* 返回 1=本次触发复位 */

/* ---- 调试辅助变量 ---- */

extern volatile uint8_t  rfid_rx_buf[RFID_RX_BUF_SIZE];
extern volatile uint16_t rfid_rx_len;
extern volatile int16_t  rfid_last_result;
extern volatile uint8_t  rfid_last_cmd;
extern volatile uint8_t  rfid_last_error;

#endif /* __RFID_ECAT_H */
