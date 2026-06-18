/*
 * rfid_ecat.c — RFID 模块驱动实现
 *
 * 通过 USART0 + DMA 与 RFID 模块 (EC-UHF-B 协议) 通信，
 * 实现 UHF RFID 标签的盘点、读写等操作。
 * 注意：此处的 RFID 模块走 USART0，与 EtherCAT 侧的 TR8253(协议转换，
 * 走 SPI)是两个独立器件，勿混淆。
 *
 * 硬件连接：
 *   USART0 (PA9=TX, PA10=RX) — 115200bps 8N1
 *   PA12 — 模块使能 (HIGH=启用)
 *   PB14/PB13/PB12 — 天线选择 (S1/S2/S3)
 *
 * DMA 收发机制：
 *   发送: DMA0_CH3, 单次模式，每次发送前重新配置
 *   接收: DMA0_CH4, 循环模式，DMA 持续将 USART 数据写入环形缓冲区
 *         软件通过比较 DMA 写位置和读位置来检测新数据
 *         帧检测在 rfid_rx_dma_service() 中完成
 *
 * 帧协议 (EC-UHF-B)：
 *   [0xAA] [Dir] [CmdID] [DataLen_H] [DataLen_L] [Data...] [Checksum] [0xDD]
 *   校验和 = Sum(Dir + CmdID + DataLen + Data) & 0xFF
 *
 * 重要：所有阻塞等待函数 (rfid_wait_frame, RFID_Inventory 等)
 *       在等待期间调用 ECAT_KeepAlive()，确保 EtherCAT 通信不中断。
 */
#include <stdio.h>
#include "rfid_ecat.h"
#include "systick.h"
#include <string.h>

/* 调试日志开关：1=启用串口打印, 0=关闭 */
#ifndef RFID_DEBUG_LOG
#define RFID_DEBUG_LOG 0
#endif

/* ============================================================
 * 全局状态变量
 * ============================================================ */

volatile uint8_t  rfid_rx_buf[RFID_RX_BUF_SIZE];  /* 帧解析缓冲区（从 DMA 缓冲区拷贝过来） */
volatile uint16_t rfid_rx_len = 0;                 /* 当前帧已接收的字节数 */
volatile int16_t  rfid_last_result = RFID_RET_TIMEOUT; /* 最后一次操作返回码 */
volatile uint8_t  rfid_last_cmd = 0;               /* 最后响应的命令码 */
volatile uint8_t  rfid_last_error = 0;             /* 模块返回的错误码 */

/* 软件看门狗状态 */
volatile uint32_t g_RfidLastAliveMs = 0;   /* 最后一次成功通信的时间戳 (ms) */
volatile uint8_t  g_RfidWdTriggered = 0;   /* 看门狗触发标志：1=正在重置, 0=正常 */

/* ============================================================
 * DMA 配置
 * ============================================================ */

#define RFID_DMA                DMA0            /* DMA0 外设 */
#define RFID_DMA_CLK            RCU_DMA0        /* DMA0 时钟 */
#define RFID_TX_DMA_CH          DMA_CH3         /* USART0_TX = DMA0_CH3 */
#define RFID_RX_DMA_CH          DMA_CH4         /* USART0_RX = DMA0_CH4 */
#define RFID_RX_DMA_SIZE        RFID_RX_BUF_SIZE /* DMA 接收缓冲区大小 */

static uint8_t s_TxBuf[RFID_TX_BUF_SIZE];               /* 发送缓冲区 */
static volatile uint8_t s_RxDmaBuf[RFID_RX_DMA_SIZE];   /* DMA 循环接收缓冲区 */
static uint16_t s_RxDmaRdPos = 0;                        /* DMA 缓冲区读位置（软件跟踪） */

static void rfid_send_cmd_data(uint8_t cmd_id, const uint8_t *data, uint16_t len);

/* ============================================================
 * 帧校验和计算 — 累加和取低 8 位
 * ============================================================ */
static uint8_t rfid_cs(const uint8_t *d, uint16_t len)
{
    uint16_t sum = 0;

    for (uint16_t i = 0; i < len; i++) {
        sum += d[i];
    }
    return (uint8_t)(sum & 0xFFU);
}

/* ============================================================
 * 清除 USART 错误标志
 *
 * 当 USART 出现溢出(ORERR)、噪声(NERR)、帧错误(FERR)、
 * 校验错误(PERR)时，必须先读 STAT 再读 DATA 才能清除标志。
 * 否则 DMA 接收会被卡住。
 * ============================================================ */
static void rfid_clear_usart_errors(void)
{
    if (RESET != usart_flag_get(RFID_USART, USART_FLAG_ORERR) ||
        RESET != usart_flag_get(RFID_USART, USART_FLAG_NERR) ||
        RESET != usart_flag_get(RFID_USART, USART_FLAG_FERR) ||
        RESET != usart_flag_get(RFID_USART, USART_FLAG_PERR)) {
        (void)USART_STAT(RFID_USART);
        (void)USART_DATA(RFID_USART);
    }
}

/* ============================================================
 * 启动 DMA 循环接收
 *
 * 配置 DMA0_CH4 为循环模式，将 USART0 接收数据持续写入
 * s_RxDmaBuf 环形缓冲区。每次发送新命令前都需要重新启动，
 * 以清除残留数据和重置读写位置。
 * ============================================================ */
static void rfid_rx_dma_start(void)
{
    extern void ECAT_KeepAlive(void);
    dma_parameter_struct dma_init_struct;

    /* 先停止 DMA 和 USART DMA 接收 */
    usart_dma_receive_config(RFID_USART, USART_RECEIVE_DMA_DISABLE);
    dma_channel_disable(RFID_DMA, RFID_RX_DMA_CH);
    dma_deinit(RFID_DMA, RFID_RX_DMA_CH);
    dma_flag_clear(RFID_DMA, RFID_RX_DMA_CH,
                   DMA_FLAG_G | DMA_FLAG_FTF | DMA_FLAG_HTF | DMA_FLAG_ERR);

    /* 清除 USART 接收缓冲区中的残留数据。
     * 这里不能无界等待：如果模块持续输出噪声/残留字节，
     * 会长时间停在 WAIT 路径并触发 EtherCAT SM 看门狗。 */
    rfid_clear_usart_errors();
    for (uint16_t drain = 0U;
         drain < RFID_RX_DMA_SIZE && RESET != usart_flag_get(RFID_USART, USART_FLAG_RBNE);
         drain++) {
        (void)usart_data_receive(RFID_USART);
        ECAT_KeepAlive();
    }

    /* 配置 DMA：外设→内存，8位，循环模式，内存地址递增 */
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.periph_addr  = (uint32_t)&USART_DATA(RFID_USART);
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_addr  = (uint32_t)s_RxDmaBuf;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.number       = RFID_RX_DMA_SIZE;
    dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_MEDIUM;
    dma_init(RFID_DMA, RFID_RX_DMA_CH, &dma_init_struct);

    /* 启用循环模式 — DMA 写到末尾后自动回到起始位置 */
    dma_circulation_enable(RFID_DMA, RFID_RX_DMA_CH);

    /* 重置读写位置 */
    s_RxDmaRdPos = 0;
    rfid_rx_len = 0;

    /* 启动 DMA 和 USART DMA 接收 */
    dma_channel_enable(RFID_DMA, RFID_RX_DMA_CH);
    usart_dma_receive_config(RFID_USART, USART_RECEIVE_DMA_ENABLE);
}

/* ============================================================
 * 获取 DMA 当前写位置
 *
 * 通过读取 DMA 剩余传输数量，计算出 DMA 已经写到了哪里。
 * 返回值范围 [0, RFID_RX_DMA_SIZE)，是环形缓冲区的写位置。
 * ============================================================ */
static uint16_t rfid_rx_dma_write_pos(void)
{
    uint16_t left = (uint16_t)dma_transfer_number_get(RFID_DMA, RFID_RX_DMA_CH);
    return (uint16_t)((RFID_RX_DMA_SIZE - left) % RFID_RX_DMA_SIZE);
}

/* ============================================================
 * 将一个字节压入帧解析缓冲区
 *
 * 帧检测逻辑：
 *   - 第一个字节必须是帧头 0xAA，否则丢弃
 *   - 后续字节依次存入 rfid_rx_buf
 *   - 缓冲区满时重置（防止溢出）
 *   - 完整帧的验证在 rfid_frame_status() 中完成
 * ============================================================ */
static void rfid_rx_push(uint8_t ch)
{
    if (rfid_rx_len == 0U) {
        /* 等待帧头 */
        if (ch != RFID_HEADER) {
            return;
        }
        rfid_rx_buf[rfid_rx_len++] = ch;
        return;
    }

    /* 缓冲区溢出保护 */
    if (rfid_rx_len >= RFID_RX_BUF_SIZE) {
        rfid_rx_len = 0;
        return;
    }

    rfid_rx_buf[rfid_rx_len++] = ch;
}

/* ============================================================
 * DMA 接收服务 — 从环形缓冲区拷贝新数据到帧解析缓冲区
 *
 * 比较 DMA 写位置和软件读位置，将中间的新数据逐字节
 * 通过 rfid_rx_push() 送入帧解析缓冲区。
 * ============================================================ */
static void rfid_rx_dma_service(void)
{
    uint16_t wr = rfid_rx_dma_write_pos();

    while (s_RxDmaRdPos != wr) {
        rfid_rx_push(s_RxDmaBuf[s_RxDmaRdPos]);
        s_RxDmaRdPos++;
        if (s_RxDmaRdPos >= RFID_RX_DMA_SIZE) {
            s_RxDmaRdPos = 0;  /* 环形缓冲区回绕 */
        }
    }
}

/* ============================================================
 * 帧状态检测 — 检查帧解析缓冲区中是否有完整的有效帧
 *
 * 返回值：
 *   >0 = 帧长度（完整帧已就绪）
 *    0 = 帧未完成（继续等待）
 *   -1 = 帧错误（校验和/帧尾不对，已重置缓冲区）
 *
 * 帧格式：[0xAA] [Dir] [CmdID] [Len_H] [Len_L] [Data...] [CS] [0xDD]
 * 帧长度 = DataLen + 7 (帧头1 + 方向1 + 命令1 + 长度2 + 校验1 + 帧尾1)
 * ============================================================ */
static int rfid_frame_status(void)
{
    uint16_t data_len;
    uint16_t frame_len;
    uint8_t cs;

    /* 最短帧为 7 字节（无数据） */
    if (rfid_rx_len < 7U) {
        return 0;
    }

    /* 帧头检查 */
    if (rfid_rx_buf[0] != RFID_HEADER) {
        rfid_rx_len = 0;
        return -1;
    }

    /* 计算期望帧长度 */
    data_len = ((uint16_t)rfid_rx_buf[3] << 8) | rfid_rx_buf[4];
    frame_len = (uint16_t)(data_len + 7U);

    /* 帧长度溢出检查 */
    if (frame_len > RFID_RX_BUF_SIZE) {
        rfid_rx_len = 0;
        return -1;
    }

    /* 数据尚未收全 */
    if (rfid_rx_len < frame_len) {
        return 0;
    }

    /* 帧尾检查 */
    if (rfid_rx_buf[frame_len - 1U] != RFID_FOOTER) {
        rfid_rx_len = 0;
        return -1;
    }

    /* 校验和验证：Sum(Dir..Data) & 0xFF 应等于 Checksum 字节 */
    cs = rfid_cs((const uint8_t *)rfid_rx_buf + 1, (uint16_t)(frame_len - 3U));
    if (cs != rfid_rx_buf[frame_len - 2U]) {
        rfid_rx_len = 0;
        return -1;
    }

    /* 完整有效帧 */
    rfid_rx_len = frame_len;
    return frame_len;
}

/* ============================================================
 * 提取模块错误码 — 如果帧是错误响应 (Dir=0x01, Cmd=0xFF)
 *
 * 返回值：
 *   >0 = 模块错误码
 *    0 = 不是错误响应
 *   <0 = 帧未就绪或帧错误
 * ============================================================ */
static int rfid_module_error_code(void)
{
    int r = rfid_frame_status();

    if (r <= 0) {
        return r;
    }
    /* 模块错误响应：Dir=MODULE, Cmd=ERROR, 且帧足够长 */
    if (rfid_rx_buf[1] == RFID_DIR_MODULE &&
        rfid_rx_buf[2] == RFID_CMD_ERROR &&
        r >= 8) {
        return rfid_rx_buf[5];
    }
    return 0;
}

/* ============================================================
 * DMA 发送 — 通过 DMA0_CH3 发送数据到 USART0
 *
 * 单次模式，发送完成后自动停止。
 * 等待 DMA 传输完成 + USART 发送完成，超时 10ms。
 * ============================================================ */
static int rfid_tx_dma_send(const uint8_t *buf, uint16_t len)
{
    dma_parameter_struct dma_init_struct;
    uint32_t deadline;

    /* 停止上一次发送 */
    dma_channel_disable(RFID_DMA, RFID_TX_DMA_CH);
    usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_DISABLE);
    dma_deinit(RFID_DMA, RFID_TX_DMA_CH);
    dma_flag_clear(RFID_DMA, RFID_TX_DMA_CH,
                   DMA_FLAG_G | DMA_FLAG_FTF | DMA_FLAG_HTF | DMA_FLAG_ERR);

    /* 配置 DMA：内存→外设，8位，单次模式 */
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.periph_addr  = (uint32_t)&USART_DATA(RFID_USART);
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_addr  = (uint32_t)buf;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.number       = len;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.priority     = DMA_PRIORITY_LOW;
    dma_init(RFID_DMA, RFID_TX_DMA_CH, &dma_init_struct);

    /* 启动发送 */
    usart_flag_clear(RFID_USART, USART_FLAG_TC);
    usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_ENABLE);
    dma_channel_enable(RFID_DMA, RFID_TX_DMA_CH);

    /* 等待 DMA 传输完成（10ms 超时）
     * 115200bps 下 20 字节帧约需 1.7ms，期间必须调 ECAT_KeepAlive
     * 防止 SM 看门狗超时 */
    deadline = g_SysTickCnt + 10U;
    while (RESET == dma_flag_get(RFID_DMA, RFID_TX_DMA_CH, DMA_FLAG_FTF)) {
        extern void ECAT_KeepAlive(void);
        ECAT_KeepAlive();
        if ((int32_t)(g_SysTickCnt - deadline) > 0) {
            dma_channel_disable(RFID_DMA, RFID_TX_DMA_CH);
            usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_DISABLE);
            return -1;
        }
    }

    /* 等待 USART 发送完成（最后一字节移出移位寄存器）
     * 同样需要 KeepAlive 保活 */
    deadline = g_SysTickCnt + 10U;
    while (RESET == usart_flag_get(RFID_USART, USART_FLAG_TC)) {
        extern void ECAT_KeepAlive(void);
        ECAT_KeepAlive();
        if ((int32_t)(g_SysTickCnt - deadline) > 0) {
            dma_channel_disable(RFID_DMA, RFID_TX_DMA_CH);
            usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_DISABLE);
            return -1;
        }
    }

    /* 清理 */
    dma_channel_disable(RFID_DMA, RFID_TX_DMA_CH);
    usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_DISABLE);
    dma_flag_clear(RFID_DMA, RFID_TX_DMA_CH,
                   DMA_FLAG_G | DMA_FLAG_FTF | DMA_FLAG_HTF | DMA_FLAG_ERR);
    return 0;
}

/* ============================================================
 * 发送命令（无参数）
 * ============================================================ */
void rfid_send_cmd(uint8_t cmd_id)
{
    rfid_send_cmd_data(cmd_id, NULL, 0);
}

/* ============================================================
 * 发送命令（带参数）
 *
 * 组装帧格式：[0xAA] [Dir=0x00] [CmdID] [Len_H] [Len_L] [Data] [CS] [0xDD]
 * 发送前确保模块使能，重启 DMA 接收，并通过 DMA 发送。
 * ============================================================ */
static void rfid_send_cmd_data(uint8_t cmd_id, const uint8_t *data, uint16_t len)
{
    uint16_t idx = 0;

    if (len > (uint16_t)(RFID_TX_BUF_SIZE - 7U)) {
        len = (uint16_t)(RFID_TX_BUF_SIZE - 7U);
    }

    /* 组装帧头 */
    s_TxBuf[idx++] = RFID_HEADER;      /* 0xAA */
    s_TxBuf[idx++] = RFID_DIR_HOST;    /* 0x00 = 主机→模块 */
    s_TxBuf[idx++] = cmd_id;
    s_TxBuf[idx++] = (uint8_t)(len >> 8);   /* 数据长度高字节 */
    s_TxBuf[idx++] = (uint8_t)len;          /* 数据长度低字节 */

    /* 填入负载数据 */
    if (data != NULL && len > 0U) {
        memcpy(&s_TxBuf[idx], data, len);
        idx = (uint16_t)(idx + len);
    }

    /* 校验和 + 帧尾 */
    s_TxBuf[idx++] = rfid_cs(&s_TxBuf[1], (uint16_t)(idx - 1U)); /* Sum(Dir..Data) */
    s_TxBuf[idx++] = RFID_FOOTER;     /* 0xDD */

    /* 确保模块使能 */
    gpio_bit_set(RFID_EN_PORT, RFID_EN_PIN);
    /* 短延时等待模块就绪（约 2~3us） */
    for (volatile int d = 0; d < 200; d++) {
        __NOP();
    }

    /* 重启 DMA 接收（清除上次残留），然后发送 */
    rfid_rx_dma_start();
    rfid_last_result = RFID_RET_TIMEOUT;
    rfid_last_cmd = cmd_id;
    rfid_last_error = 0;
    (void)rfid_tx_dma_send(s_TxBuf, idx);
}

/* ============================================================
 * 非阻塞轮询 — 检查是否收到完整帧
 *
 * 返回值：0=等待中, >0=帧长度, <0=错误
 * 在主循环或阻塞等待函数中调用。
 * ============================================================ */
int rfid_poll(void)
{
    rfid_clear_usart_errors();
    rfid_rx_dma_service();       /* 从 DMA 缓冲区拷贝新数据 */
    return rfid_frame_status();  /* 检查帧完整性 */
}

/* ============================================================
 * 阻塞等待帧 — 等待 RFID 模块响应
 *
 * 在等待期间持续调用 ECAT_KeepAlive()，确保：
 *   1. SSC 协议栈持续运行（PDO 映射正常 → SM 看门狗不触发）
 *   2. FWDGT 被喂狗（5s 硬件看门狗不触发）
 *
 * 这是最关键的保活点：RFID 命令最长可阻塞 500ms，
 * 如果不调 ECAT_KeepAlive()，EtherCAT 通信会掉线。
 * ============================================================ */
static int rfid_wait_frame(uint32_t timeout_ms)
{
    uint32_t deadline = g_SysTickCnt + timeout_ms;
    int r;

    while (1) {
        r = rfid_poll();
        if (r != 0) {
            return r;  /* 收到完整帧 (>0) 或帧错误 (<0) */
        }
        if ((int32_t)(g_SysTickCnt - deadline) > 0) {
            rfid_last_result = RFID_RET_TIMEOUT;
            rfid_rx_dma_start();  /* 超时后重启 DMA 接收 */
            return RFID_RET_TIMEOUT;
        }
        /* 阻塞期间保活 — 驱动 EtherCAT + 喂硬件看门狗 */
        extern void ECAT_KeepAlive(void);
        ECAT_KeepAlive();
    }
}

/* ============================================================
 * RFID_Init — 初始化 RFID 模块
 *
 * 配置 USART0 (115200bps 8N1)、DMA0_CH3/CH4、GPIO，
 * 启动 DMA 循环接收，初始化软件看门狗时间戳。
 * ============================================================ */
void RFID_Init(void)
{
    /* 使能外设时钟 */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RFID_USART_CLK);
    rcu_periph_clock_enable(RFID_DMA_CLK);

    /* 配置 GPIO */
    gpio_init(RFID_TX_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, RFID_TX_PIN);   /* PA9: USART0_TX */
    gpio_init(RFID_RX_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, RFID_RX_PIN); /* PA10: USART0_RX */
    gpio_init(RFID_EN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RFID_EN_PIN);  /* PA12: 模块使能 */
    gpio_bit_set(RFID_EN_PORT, RFID_EN_PIN);  /* 默认启用模块 */

    /* 配置天线选择 GPIO，默认选择天线1 (全部低电平 = 000 = 天线1) */
    gpio_init(RFID_ANT_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,
              RFID_ANT_S1 | RFID_ANT_S2 | RFID_ANT_S3);
    gpio_bit_reset(RFID_ANT_PORT, RFID_ANT_S1 | RFID_ANT_S2 | RFID_ANT_S3);

    /* 配置 USART0: 115200bps, 8N1 */
    usart_deinit(RFID_USART);
    usart_baudrate_set(RFID_USART, 115200U);
    usart_word_length_set(RFID_USART, USART_WL_8BIT);
    usart_stop_bit_set(RFID_USART, USART_STB_1BIT);
    usart_parity_config(RFID_USART, USART_PM_NONE);
    usart_transmit_config(RFID_USART, USART_TRANSMIT_ENABLE);
    usart_receive_config(RFID_USART, USART_RECEIVE_ENABLE);
    usart_enable(RFID_USART);

    /* 启动 DMA 循环接收 */
    rfid_rx_dma_start();

    /* 初始化软件看门狗时间戳 */
    extern volatile uint32_t g_SysTickCnt;
    g_RfidLastAliveMs = g_SysTickCnt;
}

/* ============================================================
 * RFID_SetAntenna — 切换天线
 *
 * 天线选择编码 (PB14=S1, PB13=S2, PB12=S3)：
 *   S1=1, S2=0, S3=0 → 天线1
 *   S1=0, S2=1, S3=0 → 天线2
 *   S1=0, S2=0, S3=1 → 天线3
 *
 * 先全部拉低，再根据 ant 参数拉高对应引脚。
 * ============================================================ */
void RFID_SetAntenna(uint8_t ant)
{
    /* 先全部拉低 */
    gpio_bit_reset(RFID_ANT_PORT, RFID_ANT_S1 | RFID_ANT_S2 | RFID_ANT_S3);

    switch (ant) {
    case 1:
        gpio_bit_set(RFID_ANT_PORT, RFID_ANT_S1);
        break;
    case 2:
        gpio_bit_set(RFID_ANT_PORT, RFID_ANT_S2);
        break;
    case 3:
        gpio_bit_set(RFID_ANT_PORT, RFID_ANT_S3);
        break;
    default:
        break;  /* 其他值：全部低 = 天线1 */
    }
}

/* ============================================================
 * RFID_Inventory — 单次盘点
 *
 * 发送 Inventory 命令，等待模块返回标签信息。
 * 超时 100ms（盘点响应通常在 30ms 内）。
 *
 * 返回值：RFID_RET_OK=成功, 其他=失败
 * 成功时 tag 中填充 RSSI/PC/EPC 数据。
 *
 * 注意：等待循环中调用 ECAT_KeepAlive() 保活。
 * ============================================================ */
int RFID_Inventory(rfid_tag_t *tag)
{
    uint32_t deadline;
    int r;
    int epc_bytes;

    rfid_send_cmd(RFID_CMD_INVENTORY);
    deadline = g_SysTickCnt + 100U;  /* 100ms 超时 */

    /* 等待响应 — 使用 ECAT_KeepAlive 保活 */
    while (1) {
        r = rfid_poll();
        if (r != 0) {
            break;
        }
        if ((int32_t)(g_SysTickCnt - deadline) > 0) {
            break;
        }
        extern void ECAT_KeepAlive(void);
        ECAT_KeepAlive();
    }

    /* 超时处理 */
    if (r == 0) {
        rfid_last_result = RFID_RET_TIMEOUT;
        rfid_rx_dma_start();
        return RFID_RET_TIMEOUT;
    }

    /* 帧错误 */
    if (r < 0) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    /* 模块返回错误 (Dir=MODULE, Cmd=ERROR) */
    if (rfid_rx_buf[1] == RFID_DIR_MODULE && rfid_rx_buf[2] == RFID_CMD_ERROR && r >= 8) {
        rfid_last_error = rfid_rx_buf[5];
        rfid_last_result = RFID_RET_MODULE_ERR;
        return RFID_RET_MODULE_ERR;
    }

    /* 模块返回非 Inventory 通知 */
    if (rfid_rx_buf[1] == RFID_DIR_MODULE) {
        rfid_last_cmd = rfid_rx_buf[2];
        rfid_last_error = (r >= 8) ? rfid_rx_buf[5] : 0;
        rfid_last_result = RFID_RET_MODULE_RSP;
        return RFID_RET_MODULE_RSP;
    }

    /* 验证通知帧格式 (Dir=NOTIFY, 至少12字节) */
    if (r < 12 || rfid_rx_buf[1] != RFID_DIR_NOTIFY) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    /* 解析标签数据 */
    tag->rssi = rfid_rx_buf[5];                         /* RSSI 信号强度 */
    tag->pc = ((uint16_t)rfid_rx_buf[6] << 8) | rfid_rx_buf[7]; /* PC 字 (大端) */
    epc_bytes = r - 12;                                  /* EPC 数据长度 */
    if (epc_bytes < 0 || epc_bytes > RFID_EPC_MAX_LEN) {
        epc_bytes = 0;
    }
    tag->epc_len = (uint8_t)epc_bytes;
    memcpy(tag->epc, (const uint8_t *)rfid_rx_buf + 8, (uint32_t)epc_bytes);

    rfid_last_result = RFID_RET_OK;
    g_RfidLastAliveMs = g_SysTickCnt;  /* 更新看门狗时间戳 */
    return RFID_RET_OK;
}

/* ============================================================
 * RFID_Reset — 软重置 RFID 模块
 *
 * 仅发送 RESET 命令，不等待响应（模块重置后无法回复）。
 * 看门狗触发后的恢复流程在 RFID_WatchdogCheck() 中处理。
 * ============================================================ */
int RFID_Reset(void)
{
    rfid_send_cmd(RFID_CMD_RESET);
    return 0;
}

/* ============================================================
 * 非阻塞 API（备用，当前未使用）
 * ============================================================ */

/* 轮询 DMA 接收状态 */
int RFID_Poll(void)
{
    return rfid_poll();
}

/* 发起命令（不等待响应） */
void RFID_StartCmd(uint8_t cmd_id)
{
    rfid_send_cmd(cmd_id);
}

/* 解析当前帧中的标签数据（非阻塞，需先 StartCmd + Poll） */
int RFID_ParseTag(rfid_tag_t *tag)
{
    int r = rfid_frame_status();
    int module_error;
    int epc_bytes;

    if (r < 0) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }
    module_error = rfid_module_error_code();
    if (module_error > 0) {
        rfid_last_error = (uint8_t)module_error;
        rfid_last_result = RFID_RET_MODULE_ERR;
        return RFID_RET_MODULE_ERR;
    }
    if (r < 12 || rfid_rx_buf[1] != RFID_DIR_NOTIFY) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    tag->rssi = rfid_rx_buf[5];
    tag->pc = ((uint16_t)rfid_rx_buf[6] << 8) | rfid_rx_buf[7];
    epc_bytes = r - 12;
    if (epc_bytes < 0 || epc_bytes > RFID_EPC_MAX_LEN) {
        epc_bytes = 0;
    }
    tag->epc_len = (uint8_t)epc_bytes;
    memcpy(tag->epc, (const uint8_t *)rfid_rx_buf + 8, (uint32_t)epc_bytes);
    rfid_last_result = RFID_RET_OK;
    g_RfidLastAliveMs = g_SysTickCnt;
    return RFID_RET_OK;
}

/* ============================================================
 * RFID_ReadTag — 读标签存储区
 *
 * 读取指定存储区 (EPC/TID/USER) 的数据。
 * 负载格式：[Reserved(4B)] [Bank(1B)] [Addr(2B)] [Len(2B)]
 * 响应格式：[DataLen] [PC_EPC_Len] [PC+EPC] [Data...] [CS]
 *
 * 阻塞等待 200ms，期间调用 ECAT_KeepAlive() 保活。
 * ============================================================ */
int RFID_ReadTag(uint8_t bank, uint16_t addr, uint16_t len_words, uint8_t *data)
{
    uint8_t payload[9];
    uint16_t data_len;
    uint16_t pc_epc_len;
    uint16_t rsp_data_offset;
    uint16_t rsp_data_bytes;
    int r;

    if (data == NULL || bank > RFID_BANK_USER || len_words == 0U ||
        len_words > (uint16_t)((RFID_RX_BUF_SIZE - 11U) / 2U)) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    /* 组装读命令负载 */
    payload[0] = 0;                           /* 保留 */
    payload[1] = 0;                           /* 保留 */
    payload[2] = 0;                           /* 保留 */
    payload[3] = 0;                           /* 保留 */
    payload[4] = bank;                        /* 存储区: EPC/TID/USER */
    payload[5] = (uint8_t)(addr >> 8);        /* 起始地址高字节 */
    payload[6] = (uint8_t)addr;               /* 起始地址低字节 */
    payload[7] = (uint8_t)(len_words >> 8);   /* 读取字数高字节 */
    payload[8] = (uint8_t)len_words;          /* 读取字数低字节 */

    rfid_send_cmd_data(RFID_CMD_READ_TAG, payload, sizeof(payload));
    r = rfid_wait_frame(200U);  /* 阻塞等待 200ms */
    if (r < 0) {
        return r;
    }

    /* 验证响应帧 */
    if (rfid_rx_buf[1] != RFID_DIR_MODULE) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }
    if (rfid_rx_buf[2] == RFID_CMD_ERROR && r >= 8) {
        rfid_last_error = rfid_rx_buf[5];
        rfid_last_result = RFID_RET_MODULE_ERR;
        return RFID_RET_MODULE_ERR;
    }
    if (rfid_rx_buf[2] != RFID_CMD_READ_TAG || r < 11) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    /* 解析响应数据 — 跳过 PC+EPC 头部，提取用户数据 */
    data_len = ((uint16_t)rfid_rx_buf[3] << 8) | rfid_rx_buf[4];
    pc_epc_len = rfid_rx_buf[5];                    /* PC+EPC 占用字节数 */
    rsp_data_offset = (uint16_t)(6U + pc_epc_len);  /* 用户数据起始偏移 */
    rsp_data_bytes = (uint16_t)(len_words * 2U);     /* 期望读取字节数 */

    if (data_len < (uint16_t)(1U + pc_epc_len + rsp_data_bytes) ||
        r < (int)(rsp_data_offset + rsp_data_bytes + 2U)) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    memcpy(data, (const uint8_t *)rfid_rx_buf + rsp_data_offset, rsp_data_bytes);
    rfid_last_result = RFID_RET_OK;
    g_RfidLastAliveMs = g_SysTickCnt;
    return RFID_RET_OK;
}

/* ============================================================
 * RFID_WriteTag — 写标签存储区
 *
 * 向指定存储区写入数据。
 * 负载格式：[Reserved(4B)] [Bank(1B)] [Addr(2B)] [Len(2B)] [Data...]
 * 响应格式：[DataLen] [Status(1B, 0=成功)] [CS]
 *
 * 阻塞等待 300ms（写操作比读慢），期间调用 ECAT_KeepAlive() 保活。
 * ============================================================ */
int RFID_WriteTag(uint8_t bank, uint16_t addr, uint16_t len_words, const uint8_t *data)
{
    uint8_t payload[9 + RFID_EPC_MAX_LEN];
    uint16_t data_bytes;
    uint16_t rsp_len;
    uint16_t status_idx;
    int r;

    if (data == NULL || bank > RFID_BANK_USER || len_words == 0U ||
        len_words > (uint16_t)(RFID_EPC_MAX_LEN / 2U)) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    /* 组装写命令负载 */
    data_bytes = (uint16_t)(len_words * 2U);
    payload[0] = 0;
    payload[1] = 0;
    payload[2] = 0;
    payload[3] = 0;
    payload[4] = bank;
    payload[5] = (uint8_t)(addr >> 8);
    payload[6] = (uint8_t)addr;
    payload[7] = (uint8_t)(len_words >> 8);
    payload[8] = (uint8_t)len_words;
    memcpy(&payload[9], data, data_bytes);

    rfid_send_cmd_data(RFID_CMD_WRITE_TAG, payload, (uint16_t)(9U + data_bytes));
    r = rfid_wait_frame(300U);  /* 阻塞等待 300ms */
    if (r < 0) {
        return r;
    }

    /* 验证响应帧 */
    if (rfid_rx_buf[1] != RFID_DIR_MODULE) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }
    if (rfid_rx_buf[2] == RFID_CMD_ERROR && r >= 8) {
        rfid_last_error = rfid_rx_buf[5];
        rfid_last_result = RFID_RET_MODULE_ERR;
        return RFID_RET_MODULE_ERR;
    }
    if (rfid_rx_buf[2] != RFID_CMD_WRITE_TAG || r < 9) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    /* 检查写入状态 — 最后一个数据字节为状态码，0=成功 */
    rsp_len = ((uint16_t)rfid_rx_buf[3] << 8) | rfid_rx_buf[4];
    status_idx = (uint16_t)(5U + rsp_len - 1U);
    if (rsp_len < 1U || r < (int)(status_idx + 2U)) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }
    if (rfid_rx_buf[status_idx] != 0U) {
        rfid_last_error = rfid_rx_buf[status_idx];
        rfid_last_result = RFID_RET_MODULE_ERR;
        return RFID_RET_MODULE_ERR;
    }

    rfid_last_result = RFID_RET_OK;
    g_RfidLastAliveMs = g_SysTickCnt;
    return RFID_RET_OK;
}

/* ============================================================
 * RFID_Command — 通用命令接口
 *
 * 发送任意命令 ID 和数据，等待响应（可自定义超时）。
 * 用于 PLC_RAW 命令和特殊命令。
 *
 * 默认超时 500ms，阻塞期间调用 ECAT_KeepAlive() 保活。
 * ============================================================ */
int RFID_Command(uint8_t cmd_id,
                 const uint8_t *tx_data,
                 uint16_t tx_len,
                 uint8_t *rx_data,
                 uint16_t *rx_len,
                 uint32_t timeout_ms)
{
    uint16_t data_len;
    uint16_t copy_len;
    int r;

    if (rx_len == NULL ||
        (tx_len > 0U && tx_data == NULL) ||
        (tx_len > (uint16_t)(RFID_TX_BUF_SIZE - 7U)) ||
        (*rx_len > 0U && rx_data == NULL)) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    if (timeout_ms == 0U) {
        timeout_ms = 500U;  /* 默认 500ms 超时 */
    }

    rfid_send_cmd_data(cmd_id, tx_data, tx_len);
    r = rfid_wait_frame(timeout_ms);
    if (r < 0) {
        return r;
    }

    /* 验证响应帧 */
    if (r < 7 || rfid_rx_buf[0] != RFID_HEADER ||
        (rfid_rx_buf[2] != cmd_id && rfid_rx_buf[2] != RFID_CMD_ERROR)) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    /* 提取响应数据 */
    data_len = ((uint16_t)rfid_rx_buf[3] << 8) | rfid_rx_buf[4];
    copy_len = data_len;
    if (copy_len > *rx_len) {
        copy_len = *rx_len;  /* 不超过调用者缓冲区 */
    }
    if (copy_len > 0U && rx_data != NULL) {
        memcpy(rx_data, (const uint8_t *)rfid_rx_buf + 5, copy_len);
    }
    *rx_len = copy_len;

    /* 检查模块错误响应 */
    if (rfid_rx_buf[1] == RFID_DIR_MODULE &&
        rfid_rx_buf[2] == RFID_CMD_ERROR &&
        data_len > 0U) {
        rfid_last_error = rfid_rx_buf[5];
        rfid_last_result = RFID_RET_MODULE_ERR;
        return RFID_RET_MODULE_ERR;
    }

    rfid_last_result = RFID_RET_OK;
    g_RfidLastAliveMs = g_SysTickCnt;
    return RFID_RET_OK;
}

/* ============================================================
 * RFID_UART_IRQHandler — USART0 中断处理
 *
 * 仅清除错误标志。正常数据接收由 DMA 完成，不使用中断。
 * ============================================================ */
void RFID_UART_IRQHandler(void)
{
    rfid_clear_usart_errors();
}

/* ============================================================
 * RFID_WatchdogCheck — RFID 软件看门狗
 *
 * 检测 RFID 模块是否卡死，2 秒无响应则触发软重置。
 *
 * 工作流程：
 *   1. 如果 g_RfidWdTriggered != 0，检查 RFID_WD_POWER_OFF_MS 恢复时间是否到期
 *   2. 到期后 PA12 重新上电，重启 USART+DMA 接收，重置看门狗时间戳
 *   3. 未在恢复中时，检查 (当前时间 - 最后存活时间) > 2s
 *   4. 超时后 PA12 断电 + 发送 RESET 命令，立即返回，不在这里忙等
 *   5. 返回 1，调用方设置 PDO 错误状态
 *
 * 注意：本函数在所有 EtherCAT 状态下都运行（不在 OP 门控内），
 *       因为 RFID 模块在任何状态下都可能卡死。
 * ============================================================ */
uint8_t RFID_WatchdogCheck(void)
{
#if RFID_SOFT_WD_ENABLE
    extern volatile uint32_t g_SysTickCnt;
    static uint32_t s_ResetReadyMs = 0U;

    /* Reset recovery is intentionally non-blocking. A 100ms busy wait here
     * can exceed the 62.5ms EtherCAT SM watchdog budget and drop OP. */
    if (g_RfidWdTriggered != 0U) {
        if ((int32_t)(g_SysTickCnt - s_ResetReadyMs) < 0) {
            return 0U;
        }

        /* 硬复位完成：PA12 重新上电，重启接收链路并重新开始存活计时。
         * 双保险：软复位(命令)救软卡死，PA12 断电再上电能救硬死亡
         * (模块固件崩溃/串口无响应)。等待 RFID_WD_POWER_ON_MS 让模块
         * 上电稳定后再开接收。 */
        RFID_EN_ON();
        rfid_rx_dma_start();
        g_RfidLastAliveMs = g_SysTickCnt;
        g_RfidWdTriggered = 0U;
        return 0U;
    }

    /* 未超时，正常 */
    if ((uint32_t)(g_SysTickCnt - g_RfidLastAliveMs) <= RFID_WD_TIMEOUT_MS) {
        return 0U;
    }

    /* RFID 模块 2 秒无响应 — 触发复位 */
    g_RfidWdTriggered = 1U;

    /* 双保险复位：
     *   1. PA12 拉低断电 — 即使模块彻底死掉(软复位命令进不去)也能强制重启
     *   2. RFID_Reset() 发软复位命令 — 模块串口还活着时走标准复位路径
     * 断电时间 RFID_WD_POWER_OFF_MS 后由上面的恢复分支重新上电。 */
    RFID_EN_OFF();
    RFID_Reset();

    /* 模块断电+重启需要时间；不要在这里等待，后续主循环再完成恢复。 */
    s_ResetReadyMs = g_SysTickCnt + RFID_WD_POWER_OFF_MS;

    return 1U;  /* 通知调用方：看门狗触发了 */
#else
    return 0U;
#endif
}

/* ============================================================
 * RFID_Scan — 自动轮询扫描
 *
 * 在 OP 状态下被主循环调用，依次扫描 3 个天线。
 *
 * 扫描策略：
 *   - 每 30ms 扫描一个天线（RFID_SCAN_MS）
 *   - 3 个天线轮替：ANT1 → ANT2 → ANT3 → ANT1 → ...
 *   - 有标签时更新 RSSI/EPC/NEW 标志
 *   - 标签丢失时延迟清除（保留 180ms + 2 次连续丢失），
 *     避免偶尔一次读不到就清除标签数据
 *
 * 丢失延迟机制：
 *   s_RfidMissCount 记录连续丢失次数
 *   s_RfidLastSeen 记录最后一次看到标签的时间
 *   满足两个条件才清除：连续丢失 >= 2 次 AND 距最后看到 >= 180ms
 * ============================================================ */
#define RFID_SCAN_MS 30U            /* 每个天线扫描间隔 */
#define RFID_LOST_HOLD_MS 180U      /* 标签丢失保持时间 */
#define RFID_LOST_CLEAR_COUNT 2U    /* 连续丢失次数阈值 */

/* 每个天线的标签数据（由 DO_LED_Ctrl 映射到 TxPDO） */
uint8_t g_RfidRssi  [RFID_ANT_COUNT];
uint8_t g_RfidEpcLen[RFID_ANT_COUNT];
uint8_t g_RfidNew   [RFID_ANT_COUNT];
uint8_t g_RfidEpc   [RFID_ANT_COUNT][RFID_EPC_BYTES];

/* 标签丢失跟踪（内部使用） */
static uint32_t s_RfidLastSeen[RFID_ANT_COUNT];   /* 最后看到标签的时间戳 */
static uint8_t  s_RfidMissCount[RFID_ANT_COUNT];  /* 连续丢失计数 */

/* 清除指定天线的标签数据 */
static void RFID_ClearChannel(uint8_t idx)
{
    g_RfidRssi[idx] = 0;
    g_RfidEpcLen[idx] = 0;
    g_RfidNew[idx] = 0;
    memset(g_RfidEpc[idx], 0, RFID_EPC_BYTES);
}

void RFID_Scan(void)
{
    static uint8_t ant = 1;           /* 当前扫描的天线 (1~3) */
    static uint32_t next = 0;         /* 下次扫描的时间戳 */
    static uint8_t pending = 0U;
    static uint8_t pending_ant = 1U;
    static uint32_t deadline = 0U;
    extern volatile uint32_t g_SysTickCnt;
    rfid_tag_t tag;
    uint8_t idx;
    int ret;

    if (g_RfidWdTriggered != 0U) {
        return;
    }

    if (pending == 0U) {
        /* 时间未到，跳过 */
        if (g_SysTickCnt < next) {
            return;
        }

        RFID_SetAntenna(ant);
        RFID_StartCmd(RFID_CMD_INVENTORY);
        pending_ant = ant;
        deadline = g_SysTickCnt + 100U;
        pending = 1U;

        ant++;
        if (ant > RFID_ANT_COUNT) {
            ant = 1U;
        }
        return;
    }

    ret = RFID_Poll();
    if (ret == 0) {
        if ((int32_t)(g_SysTickCnt - deadline) <= 0) {
            return;
        }
        rfid_last_result = RFID_RET_TIMEOUT;
        rfid_rx_dma_start();
        ret = RFID_RET_TIMEOUT;
    } else if (ret > 0) {
        ret = RFID_ParseTag(&tag);
    } else {
        rfid_last_result = RFID_RET_FRAME_ERR;
        ret = RFID_RET_FRAME_ERR;
    }

    pending = 0U;
    idx = (uint8_t)(pending_ant - 1U);

    if (ret == RFID_RET_OK) {
        /* 读到标签 — 更新数据 */
        g_RfidRssi[idx] = tag.rssi;
        g_RfidEpcLen[idx] = tag.epc_len;
        memset(g_RfidEpc[idx], 0, RFID_EPC_BYTES);
        memcpy(g_RfidEpc[idx], tag.epc, tag.epc_len);
        g_RfidNew[idx] = 1;
        s_RfidLastSeen[idx] = g_SysTickCnt;
        s_RfidMissCount[idx] = 0;
    } else {
        /* 无标签或读取失败 */
        if (rfid_last_error == RFID_ERR_NO_TAG || rfid_last_cmd == RFID_RSP_NO_TAG) {
            /* 确实无标签 — 累计丢失次数 */
            if (s_RfidMissCount[idx] < 255U) {
                s_RfidMissCount[idx]++;
            }

            /* 延迟清除：连续丢失 >= 2 次 AND 距最后看到 >= 180ms */
            if (g_RfidNew[idx] != 0U &&
                s_RfidMissCount[idx] >= RFID_LOST_CLEAR_COUNT &&
                (uint32_t)(g_SysTickCnt - s_RfidLastSeen[idx]) >= RFID_LOST_HOLD_MS) {
                RFID_ClearChannel(idx);
            }
        } else {
            /* 其他错误（通信故障等），调试时打印 */
#if RFID_DEBUG_LOG
            printf("[RFID] ANT%d err ret=%d rx=%d\n", ant, ret, rfid_rx_len);
#endif
        }
    }

    next = g_SysTickCnt + RFID_SCAN_MS;
}
