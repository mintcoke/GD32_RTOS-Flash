/*
 * rfid_ecat.h — RFID 模块驱动接口
 *
 * 硬件连接：
 *   USART0 (PA9=TX, PA10=RX) — 与 RFID 模块通信，115200bps 8N1
 *   PA12 — 模块使能引脚 (HIGH=启用, LOW=待机)
 *   PB14/PB13/PB12 — 天线选择 (S1/S2/S3，3 位编码选 1~8 天线)
 *
 * 通信协议 (EC-UHF-B)：
 *   帧格式: 0xAA + Dir(1B) + CmdID(1B) + DataLen(2B,大端) + Data(NB) + Checksum(1B) + 0xDD
 *   方向码: 0x00=主机→模块, 0x01=模块→主机, 0x02=模块主动通知
 *
 * DMA 收发：
 *   发送: DMA0_CH3 (USART0_TX)，单次模式
 *   接收: DMA0_CH4 (USART0_RX)，循环模式
 *   接收采用循环模式 + 软件帧检测，无需每帧重新启动 DMA
 *
 * 两级看门狗保护：
 *   1. 软件看门狗（本文件）：2s 无响应 → 软重置 RFID 模块
 *   2. 硬件看门狗（wdg_ecat.c）：5s 主循环卡死 → 芯片硬复位
 */

#ifndef __RFID_ECAT_H
#define __RFID_ECAT_H

#include "gd32f10x.h"
#include <stdint.h>

/* ============================================================
 * 硬件引脚定义
 * ============================================================ */

/* USART0 与 RFID 模块通信 */
#define RFID_USART           USART0          /* USART 外设 */
#define RFID_USART_CLK       RCU_USART0      /* USART0 时钟 */
#define RFID_TX_PORT         GPIOA           /* TX 引脚端口 */
#define RFID_TX_PIN          GPIO_PIN_9      /* TX = PA9 */
#define RFID_RX_PORT         GPIOA           /* RX 引脚端口 */
#define RFID_RX_PIN          GPIO_PIN_10     /* RX = PA10 */

/* RFID 模块使能 — PA12, 高电平启用 */
#define RFID_EN_PORT         GPIOA
#define RFID_EN_PIN          GPIO_PIN_12
#define RFID_EN_ON()         gpio_bit_set(RFID_EN_PORT, RFID_EN_PIN)    /* 启用模块 */
#define RFID_EN_OFF()        gpio_bit_reset(RFID_EN_PORT, RFID_EN_PIN)  /* 待机模式 */

/* 兼容别名：PA12 是模块使能，不是 RS485 方向控制
 * (旧代码中可能有 RFID_EN_TX/RX 的调用，保持兼容) */
#define RFID_EN_TX()         RFID_EN_ON()
#define RFID_EN_RX()         RFID_EN_ON()

/* 天线选择 — PB14=S1, PB13=S2, PB12=S3
 * 3 位二进制编码：000=天线1, 001=天线2, ..., 111=天线8
 * 当前板子使用 3 个天线 (ANT1~ANT3) */
#define RFID_ANT_PORT        GPIOB
#define RFID_ANT_S1          GPIO_PIN_14     /* S1 (LSB) */
#define RFID_ANT_S2          GPIO_PIN_13     /* S2 */
#define RFID_ANT_S3          GPIO_PIN_12     /* S3 (MSB) */

/* ============================================================
 * 帧协议常量
 * ============================================================ */

#define RFID_HEADER          0xAA    /* 帧头 */
#define RFID_FOOTER          0xDD    /* 帧尾 */
#define RFID_DIR_HOST         0x00   /* 方向：主机→模块 */
#define RFID_DIR_MODULE       0x01   /* 方向：模块→主机（命令响应） */
#define RFID_DIR_NOTIFY       0x02   /* 方向：模块主动通知（如盘点结果） */

/* ============================================================
 * RFID 模块命令码 — 发送给 RFID 模块的指令
 * ============================================================ */

#define RFID_CMD_READ_INFO    0x03   /* 读取模块信息（固件版本等） */
#define RFID_CMD_RESET        0x19   /* 软重置模块 */
#define RFID_CMD_INVENTORY    0x22   /* 单次盘点（返回一个标签） */
#define RFID_CMD_MULTI_POLL   0x27   /* 连续盘点（模块主动推送） */
#define RFID_CMD_STOP_POLL    0x28   /* 停止连续盘点 */

/* 标签选择相关 */
#define RFID_CMD_SET_SELECT   0x0C   /* 设置选择条件（用于后续读/写指定标签） */
#define RFID_CMD_GET_SELECT   0x0B   /* 读取当前选择条件 */
#define RFID_CMD_GET_QUERY    0x0D   /* 读取 Query 参数 */
#define RFID_CMD_SET_QUERY    0x0E   /* 设置 Query 参数 */
#define RFID_CMD_SET_SEL_MODE 0x12   /* 设置选择模式 (0x01=关闭选择) */

/* 区域和信道配置 */
#define RFID_CMD_SET_REGION   0x07   /* 设置工作区域 */
#define RFID_CMD_GET_REGION   0x08   /* 读取工作区域 */
#define RFID_CMD_GET_CHANNEL  0xAA   /* 读取当前信道 */
#define RFID_CMD_SET_CHANNEL  0xAB   /* 设置固定信道 */
#define RFID_CMD_GET_HOP      0xAC   /* 读取跳频模式 */
#define RFID_CMD_SET_HOP      0xAD   /* 设置跳频模式 */
#define RFID_CMD_GET_CH_LIST  0xA8   /* 读取跳频信道列表 */
#define RFID_CMD_SET_CH_LIST  0xA9   /* 设置跳频信道列表 */
#define RFID_CMD_SET_CARRIER  0xB0   /* 设置载波开关 */

/* 功率和接收配置 */
#define RFID_CMD_SET_POWER    0xB6   /* 设置发射功率 (单位: 10dBm) */
#define RFID_CMD_GET_POWER    0xB7   /* 读取当前发射功率 */
#define RFID_CMD_SET_RX       0xF0   /* 设置接收灵敏度参数 */
#define RFID_CMD_GET_RX       0xF1   /* 读取接收灵敏度参数 */
#define RFID_CMD_TEST_RSSI    0xF3   /* RSSI 测试 */
#define RFID_CMD_SET_MODE     0xF5   /* 设置工作模式 (0=密集, 1=正常) */
#define RFID_CMD_GET_MODE     0xF6   /* 读取工作模式 */

/* 标签读写 */
#define RFID_CMD_READ_TAG     0x39   /* 读标签指定存储区 */
#define RFID_CMD_WRITE_TAG    0x49   /* 写标签指定存储区 */
#define RFID_CMD_SET_IO       0x1A   /* 设置 IO 控制 */
#define RFID_CMD_ERROR        0xFF   /* 模块返回的错误响应 */

/* ============================================================
 * 驱动返回码 — RFID 函数的返回值
 * ============================================================ */

#define RFID_RET_OK            0     /* 成功 */
#define RFID_RET_TIMEOUT      -1     /* 超时：模块无响应 */
#define RFID_RET_MODULE_ERR   -2     /* 模块返回错误码（见 rfid_last_error） */
#define RFID_RET_FRAME_ERR    -3     /* 帧格式错误（校验和/长度/方向码不对） */
#define RFID_RET_MODULE_RSP   -4     /* 模块返回非预期响应（命令码不匹配） */

/* 常见模块错误码 */
#define RFID_ERR_NO_TAG        0x15  /* 无标签 */
#define RFID_RSP_NO_TAG        0xF5  /* 盘点无标签（旧版协议） */

/* ============================================================
 * 标签存储区定义 — ISO 18000-6C 标准
 * ============================================================ */

#define RFID_BANK_RFU         0x00   /* 保留区 */
#define RFID_BANK_EPC         0x01   /* EPC 区（包含 PC + EPC 码） */
#define RFID_BANK_TID         0x02   /* TID 区（标签唯一标识，只读） */
#define RFID_BANK_USER        0x03   /* 用户区（可读可写） */

/* ============================================================
 * 缓冲区大小
 * ============================================================ */

#define RFID_RX_BUF_SIZE      256   /* DMA 接收缓冲区大小（字节） */
#define RFID_TX_BUF_SIZE      256   /* 发送缓冲区大小（字节） */
#define RFID_EPC_MAX_LEN      62    /* EPC 码最大长度（字节） */

/* ============================================================
 * 标签信息结构体 — Inventory 命令返回的数据
 * ============================================================ */
typedef struct {
    uint8_t  epc_len;                        /* EPC 码长度（字节数） */
    uint8_t  epc[RFID_EPC_MAX_LEN];          /* EPC 码数据 */
    uint8_t  rssi;                           /* 信号强度 (0~255) */
    uint16_t pc;                             /* PC (Protocol Control) 字 */
} rfid_tag_t;

/* ============================================================
 * 公共 API — 阻塞式调用
 *
 * 这些函数会阻塞等待 RFID 模块响应（最长 500ms），
 * 阻塞期间内部调用 ECAT_KeepAlive() 保持 EtherCAT 通信。
 * ============================================================ */

/* 初始化 RFID 模块（USART0 + DMA + GPIO） */
void RFID_Init(void);

/* 切换天线 (1~3)，控制 PB14/PB13/PB12 */
void RFID_SetAntenna(uint8_t ant);

/* 软重置 RFID 模块（非阻塞，发送命令后立即返回） */
int  RFID_Reset(void);

/* 单次盘点：发送 Inventory 命令，等待标签响应（100ms 超时） */
int  RFID_Inventory(rfid_tag_t *tag);

/* 读标签存储区：指定存储区、起始地址、字数 */
int  RFID_ReadTag(uint8_t bank, uint16_t addr, uint16_t len_words, uint8_t *data);

/* 写标签存储区：指定存储区、起始地址、字数、数据 */
int  RFID_WriteTag(uint8_t bank, uint16_t addr, uint16_t len_words, const uint8_t *data);

/* 通用命令接口：发送任意命令，等待响应（可自定义超时） */
int  RFID_Command(uint8_t cmd_id,
                  const uint8_t *tx_data,
                  uint16_t tx_len,
                  uint8_t *rx_data,
                  uint16_t *rx_len,
                  uint32_t timeout_ms);

/* ============================================================
 * 非阻塞 API — 用于在主循环中分步执行
 *
 * StartCmd 发起命令，Poll 轮询状态，ParseTag 解析结果。
 * 当前代码主要使用阻塞式 API，非阻塞 API 保留备用。
 * ============================================================ */

/* 发送命令（不等待响应） */
void RFID_StartCmd(uint8_t cmd_id);

/* 轮询 DMA 接收状态：0=等待中, >0=收到帧, <0=错误 */
int  RFID_Poll(void);

/* 解析 DMA 缓冲区中的标签数据 */
int  RFID_ParseTag(rfid_tag_t *tag);

/* USART0 中断处理（清除错误标志） */
void RFID_UART_IRQHandler(void);

/* 自动轮询扫描 — 在 OP 状态下被主循环调用
 * 依次扫描 3 个天线，更新全局标签数据 */
void RFID_Scan(void);

/* ============================================================
 * 天线标签数据 — 全局变量，每个天线一组
 *
 * 由 RFID_Scan() 更新，由 APPL_UpdateTxPdo() 映射到 TxPDO
 * ============================================================ */

#define RFID_ANT_COUNT    3U      /* 天线数量 */
#define RFID_EPC_BYTES    62U     /* EPC 数据字节数（与 RFID_EPC_MAX_LEN 一致） */

extern uint8_t g_RfidRssi  [RFID_ANT_COUNT];   /* 每个天线的标签信号强度 */
extern uint8_t g_RfidEpcLen[RFID_ANT_COUNT];   /* 每个天线的 EPC 长度 */
extern uint8_t g_RfidNew   [RFID_ANT_COUNT];   /* 每个天线的新标签标志 (1=有更新) */
extern uint8_t g_RfidEpc   [RFID_ANT_COUNT][RFID_EPC_BYTES]; /* 每个天线的 EPC 数据 */

/* ============================================================
 * 软件看门狗 — 监控 RFID 模块是否响应
 *
 * 工作原理：
 *   g_RfidLastAliveMs 记录最后一次收到模块响应（完整帧）的时间戳，
 *   只要模块回了话（含"无标签"错误码）即视为活着；
 *   如果 (当前时间 - 最后存活时间) > 2s（模块完全无响应），触发软重置。
 *
 * 重置流程：
 *   1. 设置 g_RfidWdTriggered = 1（防止重入）
 *   2. 发送 RESET 命令
 *   3. 立即返回，100ms 恢复等待由后续主循环分步完成
 *   4. 到时后重新初始化 USART + DMA
 *   5. 清除 g_RfidWdTriggered，允许再次触发
 *   6. 首次触发返回 1，调用方设置 PDO 错误状态
 *
 * 注意：RFID_WatchdogCheck() 必须在 OP 门控之外调用，
 *       因为 RFID 模块在任何状态下都可能卡死。
 * ============================================================ */

#ifndef RFID_SOFT_WD_ENABLE
#define RFID_SOFT_WD_ENABLE    1U      /* 启用 RFID 软件看门狗：模块 2s 无响应自动软复位自愈 */
#endif

#define RFID_WD_TIMEOUT_MS     2000U   /* 超时阈值：2 秒无响应则触发重置 */
#define RFID_WD_POWER_OFF_MS   200U    /* 硬复位：PA12 断电时长，到期后重新上电并开接收 */

extern volatile uint32_t g_RfidLastAliveMs;  /* 最后一次 RFID 通信成功的时间戳 (ms) */
extern volatile uint8_t  g_RfidWdTriggered;  /* 看门狗触发标志：1=正在重置, 0=正常 */

/* 检查 RFID 看门狗，返回 1 表示本次触发了重置 */
uint8_t RFID_WatchdogCheck(void);

/* ============================================================
 * 调试辅助变量
 * ============================================================ */

extern volatile uint8_t  rfid_rx_buf[RFID_RX_BUF_SIZE]; /* DMA 接收缓冲区（循环模式） */
extern volatile uint16_t rfid_rx_len;   /* 当前接收到的帧长度 */
extern volatile int16_t  rfid_last_result; /* 最后一次操作的返回码 */
extern volatile uint8_t  rfid_last_cmd;    /* 最后响应的命令码 */
extern volatile uint8_t  rfid_last_error;  /* 模块返回的错误码 */

#endif /* __RFID_ECAT_H */
