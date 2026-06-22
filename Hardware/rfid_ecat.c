/*
 * rfid_ecat.c — RFID 模块驱动 (EC-UHF-B 协议)
 *
 * USART0 + DMA 与 RFID 模块通信。该模块独立于 EtherCAT 侧的 TR8253(走 SPI)。
 *   USART0 (PA9=TX, PA10=RX) 115200bps 8N1
 *   PA12  模块使能 (HIGH=启用)
 *   PB14/PB13/PB12 天线选择 (S1/S2/S3)
 *
 * 收发：TX 走 DMA0_CH3 单次；RX 走 DMA0_CH4 循环，软件比较读写位置取帧。
 * 帧格式: [0xAA][Dir][Cmd][LenH][LenL][Data...][CS][0xDD]，CS=Sum(Dir..Data)&0xFF
 *
 * 所有阻塞等待均调 ECAT_KeepAlive() 保活，防止 RFID 命令(最长 500ms)
 * 阻塞期间 EtherCAT 掉线或 FWDGT 超时。
 */
#include <stdio.h>
#include "rfid_ecat.h"
#include "systick.h"
#include <string.h>

#ifndef RFID_DEBUG_LOG
#define RFID_DEBUG_LOG 0
#endif

/* ---- 全局状态 ---- */
volatile uint8_t  rfid_rx_buf[RFID_RX_BUF_SIZE];  /* 帧解析缓冲区 */
volatile uint16_t rfid_rx_len = 0;                 /* 当前帧已接收字节数 */
volatile int16_t  rfid_last_result = RFID_RET_TIMEOUT;
volatile uint8_t  rfid_last_cmd = 0;
volatile uint8_t  rfid_last_error = 0;

/* 软件看门狗状态 */
volatile uint32_t g_RfidLastAliveMs = 0;   /* 最后一次收到模块响应的时间戳 */
volatile uint8_t  g_RfidWdTriggered = 0;   /* 1=正在复位恢复, 0=正常 */

/* ---- DMA 配置 ---- */
#define RFID_DMA                DMA0
#define RFID_DMA_CLK            RCU_DMA0
#define RFID_TX_DMA_CH          DMA_CH3
#define RFID_RX_DMA_CH          DMA_CH4
#define RFID_RX_DMA_SIZE        RFID_RX_BUF_SIZE

static uint8_t s_TxBuf[RFID_TX_BUF_SIZE];
static volatile uint8_t s_RxDmaBuf[RFID_RX_DMA_SIZE];
static uint16_t s_RxDmaRdPos = 0;   /* 软件读位置 */

static void rfid_send_cmd_data(uint8_t cmd_id, const uint8_t *data, uint16_t len);

/* 帧校验和 = Sum(d) & 0xFF */
static uint8_t rfid_cs(const uint8_t *d, uint16_t len)
{
    uint16_t sum = 0;
    for (uint16_t i = 0; i < len; i++) {
        sum += d[i];
    }
    return (uint8_t)(sum & 0xFFU);
}

/* 清 USART 错误标志 (ORERR/NERR/FERR/PERR)。不清则 DMA 接收卡死。
 * GD32 要求先读 STAT 再读 DATA 才能清除。 */
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

/* 启动 DMA 循环接收。每次发命令前重启动，清残留数据。
 * drain 循环不能无界等待，否则模块持续输出噪声会撞 SM 看门狗。 */
static void rfid_rx_dma_start(void)
{
    extern void ECAT_KeepAlive(void);
    dma_parameter_struct dma_init_struct;

    usart_dma_receive_config(RFID_USART, USART_RECEIVE_DMA_DISABLE);
    dma_channel_disable(RFID_DMA, RFID_RX_DMA_CH);
    dma_deinit(RFID_DMA, RFID_RX_DMA_CH);
    dma_flag_clear(RFID_DMA, RFID_RX_DMA_CH,
                   DMA_FLAG_G | DMA_FLAG_FTF | DMA_FLAG_HTF | DMA_FLAG_ERR);

    rfid_clear_usart_errors();
    for (uint16_t drain = 0U;
         drain < RFID_RX_DMA_SIZE && RESET != usart_flag_get(RFID_USART, USART_FLAG_RBNE);
         drain++) {
        (void)usart_data_receive(RFID_USART);
        ECAT_KeepAlive();
    }

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

    dma_circulation_enable(RFID_DMA, RFID_RX_DMA_CH);

    s_RxDmaRdPos = 0;
    rfid_rx_len = 0;

    dma_channel_enable(RFID_DMA, RFID_RX_DMA_CH);
    usart_dma_receive_config(RFID_USART, USART_RECEIVE_DMA_ENABLE);
}

/* DMA 当前写位置 [0, RFID_RX_DMA_SIZE) */
static uint16_t rfid_rx_dma_write_pos(void)
{
    uint16_t left = (uint16_t)dma_transfer_number_get(RFID_DMA, RFID_RX_DMA_CH);
    return (uint16_t)((RFID_RX_DMA_SIZE - left) % RFID_RX_DMA_SIZE);
}

/* 压入一字节到帧缓冲。首字节须为 0xAA，否则丢弃；满则重置防溢出。
 * 完整帧校验在 rfid_frame_status() 完成。 */
static void rfid_rx_push(uint8_t ch)
{
    if (rfid_rx_len == 0U) {
        if (ch != RFID_HEADER) {
            return;
        }
        rfid_rx_buf[rfid_rx_len++] = ch;
        return;
    }

    if (rfid_rx_len >= RFID_RX_BUF_SIZE) {
        rfid_rx_len = 0;
        return;
    }

    rfid_rx_buf[rfid_rx_len++] = ch;
}

/* 把 DMA 环形缓冲区中的新数据搬到帧缓冲区 */
static void rfid_rx_dma_service(void)
{
    uint16_t wr = rfid_rx_dma_write_pos();

    while (s_RxDmaRdPos != wr) {
        rfid_rx_push(s_RxDmaBuf[s_RxDmaRdPos]);
        s_RxDmaRdPos++;
        if (s_RxDmaRdPos >= RFID_RX_DMA_SIZE) {
            s_RxDmaRdPos = 0;
        }
    }
}

/* 检查帧缓冲是否有完整有效帧。
 * 返回 >0=帧长度, 0=未完成, -1=错误(已重置)。 */
static int rfid_frame_status(void)
{
    uint16_t data_len;
    uint16_t frame_len;
    uint8_t cs;

    if (rfid_rx_len < 7U) {
        return 0;
    }

    if (rfid_rx_buf[0] != RFID_HEADER) {
        rfid_rx_len = 0;
        return -1;
    }

    data_len = ((uint16_t)rfid_rx_buf[3] << 8) | rfid_rx_buf[4];
    frame_len = (uint16_t)(data_len + 7U);

    if (frame_len > RFID_RX_BUF_SIZE) {
        rfid_rx_len = 0;
        return -1;
    }

    if (rfid_rx_len < frame_len) {
        return 0;
    }

    if (rfid_rx_buf[frame_len - 1U] != RFID_FOOTER) {
        rfid_rx_len = 0;
        return -1;
    }

    cs = rfid_cs((const uint8_t *)rfid_rx_buf + 1, (uint16_t)(frame_len - 3U));
    if (cs != rfid_rx_buf[frame_len - 2U]) {
        rfid_rx_len = 0;
        return -1;
    }

    rfid_rx_len = frame_len;
    return frame_len;
}

/* 提取模块错误码 (Dir=MODULE, Cmd=ERROR)。
 * 返回 >0=错误码, 0=非错误响应, <0=帧未就绪/错误。 */
static int rfid_module_error_code(void)
{
    int r = rfid_frame_status();

    if (r <= 0) {
        return r;
    }
    if (rfid_rx_buf[1] == RFID_DIR_MODULE &&
        rfid_rx_buf[2] == RFID_CMD_ERROR &&
        r >= 8) {
        return rfid_rx_buf[5];
    }
    return 0;
}

/* DMA 发送。单次模式，等 DMA+USART 发送完成，10ms 超时。
 * 等待期间调 ECAT_KeepAlive 防 SM 看门狗超时。 */
static int rfid_tx_dma_send(const uint8_t *buf, uint16_t len)
{
    dma_parameter_struct dma_init_struct;
    uint32_t deadline;

    dma_channel_disable(RFID_DMA, RFID_TX_DMA_CH);
    usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_DISABLE);
    dma_deinit(RFID_DMA, RFID_TX_DMA_CH);
    dma_flag_clear(RFID_DMA, RFID_TX_DMA_CH,
                   DMA_FLAG_G | DMA_FLAG_FTF | DMA_FLAG_HTF | DMA_FLAG_ERR);

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

    usart_flag_clear(RFID_USART, USART_FLAG_TC);
    usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_ENABLE);
    dma_channel_enable(RFID_DMA, RFID_TX_DMA_CH);

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

    dma_channel_disable(RFID_DMA, RFID_TX_DMA_CH);
    usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_DISABLE);
    dma_flag_clear(RFID_DMA, RFID_TX_DMA_CH,
                   DMA_FLAG_G | DMA_FLAG_FTF | DMA_FLAG_HTF | DMA_FLAG_ERR);
    return 0;
}

void rfid_send_cmd(uint8_t cmd_id)
{
    rfid_send_cmd_data(cmd_id, NULL, 0);
}

/* 组装并发送一帧。帧: [0xAA][0x00][Cmd][LenH][LenL][Data][CS][0xDD]
 * 发送前确保模块使能，重启 DMA 接收清残留。 */
static void rfid_send_cmd_data(uint8_t cmd_id, const uint8_t *data, uint16_t len)
{
    uint16_t idx = 0;

    if (len > (uint16_t)(RFID_TX_BUF_SIZE - 7U)) {
        len = (uint16_t)(RFID_TX_BUF_SIZE - 7U);
    }

    s_TxBuf[idx++] = RFID_HEADER;
    s_TxBuf[idx++] = RFID_DIR_HOST;
    s_TxBuf[idx++] = cmd_id;
    s_TxBuf[idx++] = (uint8_t)(len >> 8);
    s_TxBuf[idx++] = (uint8_t)len;

    if (data != NULL && len > 0U) {
        memcpy(&s_TxBuf[idx], data, len);
        idx = (uint16_t)(idx + len);
    }

    s_TxBuf[idx++] = rfid_cs(&s_TxBuf[1], (uint16_t)(idx - 1U));
    s_TxBuf[idx++] = RFID_FOOTER;

    gpio_bit_set(RFID_EN_PORT, RFID_EN_PIN);
    for (volatile int d = 0; d < 200; d++) {
        __NOP();
    }

    rfid_rx_dma_start();
    rfid_last_result = RFID_RET_TIMEOUT;
    rfid_last_cmd = cmd_id;
    rfid_last_error = 0;
    (void)rfid_tx_dma_send(s_TxBuf, idx);
}

/* 轮询是否收到完整帧。返回 0=等待中, >0=帧长度, <0=错误。
 * 收到完整帧即刷新看门狗时间戳：模块回了话(含"无标签"错误码)即视为活着，
 * 只有真超时(模块不回话)才不刷新，由 RFID_WatchdogCheck 判定卡死。 */
int rfid_poll(void)
{
    int r;

    rfid_clear_usart_errors();
    rfid_rx_dma_service();
    r = rfid_frame_status();
    if (r > 0) {
        extern volatile uint32_t g_SysTickCnt;
        g_RfidLastAliveMs = g_SysTickCnt;
    }
    return r;
}

/* 阻塞等待帧。等待期间调 ECAT_KeepAlive 保活(SSC + FWDGT)。
 * RFID 命令最长阻塞 500ms，不保活会掉 EtherCAT。 */
static int rfid_wait_frame(uint32_t timeout_ms)
{
    uint32_t deadline = g_SysTickCnt + timeout_ms;
    int r;

    while (1) {
        r = rfid_poll();
        if (r != 0) {
            return r;
        }
        if ((int32_t)(g_SysTickCnt - deadline) > 0) {
            rfid_last_result = RFID_RET_TIMEOUT;
            rfid_rx_dma_start();
            return RFID_RET_TIMEOUT;
        }
        extern void ECAT_KeepAlive(void);
        ECAT_KeepAlive();
    }
}

void RFID_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RFID_USART_CLK);
    rcu_periph_clock_enable(RFID_DMA_CLK);

    gpio_init(RFID_TX_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, RFID_TX_PIN);
    gpio_init(RFID_RX_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, RFID_RX_PIN);
    gpio_init(RFID_EN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RFID_EN_PIN);
    gpio_bit_set(RFID_EN_PORT, RFID_EN_PIN);

    /* 天线选择 GPIO，默认天线1 (全低=000=天线1) */
    gpio_init(RFID_ANT_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,
              RFID_ANT_S1 | RFID_ANT_S2 | RFID_ANT_S3);
    gpio_bit_reset(RFID_ANT_PORT, RFID_ANT_S1 | RFID_ANT_S2 | RFID_ANT_S3);

    usart_deinit(RFID_USART);
    usart_baudrate_set(RFID_USART, 115200U);
    usart_word_length_set(RFID_USART, USART_WL_8BIT);
    usart_stop_bit_set(RFID_USART, USART_STB_1BIT);
    usart_parity_config(RFID_USART, USART_PM_NONE);
    usart_transmit_config(RFID_USART, USART_TRANSMIT_ENABLE);
    usart_receive_config(RFID_USART, USART_RECEIVE_ENABLE);
    usart_enable(RFID_USART);

    rfid_rx_dma_start();

    extern volatile uint32_t g_SysTickCnt;
    g_RfidLastAliveMs = g_SysTickCnt;
}

/* 切换天线。先全拉低，再拉高对应引脚。其他值=天线1。 */
void RFID_SetAntenna(uint8_t ant)
{
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
        break;
    }
}

/* 单次盘点，100ms 超时。成功时 tag 填充 RSSI/PC/EPC。 */
int RFID_Inventory(rfid_tag_t *tag)
{
    uint32_t deadline;
    int r;
    int epc_bytes;

    rfid_send_cmd(RFID_CMD_INVENTORY);
    deadline = g_SysTickCnt + 100U;

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

    if (r == 0) {
        rfid_last_result = RFID_RET_TIMEOUT;
        rfid_rx_dma_start();
        return RFID_RET_TIMEOUT;
    }

    if (r < 0) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    if (rfid_rx_buf[1] == RFID_DIR_MODULE && rfid_rx_buf[2] == RFID_CMD_ERROR && r >= 8) {
        rfid_last_error = rfid_rx_buf[5];
        rfid_last_result = RFID_RET_MODULE_ERR;
        return RFID_RET_MODULE_ERR;
    }

    if (rfid_rx_buf[1] == RFID_DIR_MODULE) {
        rfid_last_cmd = rfid_rx_buf[2];
        rfid_last_error = (r >= 8) ? rfid_rx_buf[5] : 0;
        rfid_last_result = RFID_RET_MODULE_RSP;
        return RFID_RET_MODULE_RSP;
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

/* 软重置。仅发命令不等响应(模块复位后无法回复)。
 * 看门狗恢复流程在 RFID_WatchdogCheck()。 */
int RFID_Reset(void)
{
    rfid_send_cmd(RFID_CMD_RESET);
    return 0;
}

/* ---- 非阻塞 API (备用) ---- */

int RFID_Poll(void)
{
    return rfid_poll();
}

void RFID_StartCmd(uint8_t cmd_id)
{
    rfid_send_cmd(cmd_id);
}

/* 解析当前帧标签数据，需先 StartCmd + Poll */
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

/* 读标签存储区 (EPC/TID/USER)。阻塞 200ms。
 * 负载: [Reserved(4B)][Bank(1B)][Addr(2B)][Len(2B)]
 * 响应: [DataLen][PC_EPC_Len][PC+EPC][Data...][CS]，跳过 PC+EPC 头取用户数据 */
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

    payload[0] = 0;
    payload[1] = 0;
    payload[2] = 0;
    payload[3] = 0;
    payload[4] = bank;
    payload[5] = (uint8_t)(addr >> 8);
    payload[6] = (uint8_t)addr;
    payload[7] = (uint8_t)(len_words >> 8);
    payload[8] = (uint8_t)len_words;

    rfid_send_cmd_data(RFID_CMD_READ_TAG, payload, sizeof(payload));
    r = rfid_wait_frame(200U);
    if (r < 0) {
        return r;
    }

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

    data_len = ((uint16_t)rfid_rx_buf[3] << 8) | rfid_rx_buf[4];
    pc_epc_len = rfid_rx_buf[5];
    rsp_data_offset = (uint16_t)(6U + pc_epc_len);
    rsp_data_bytes = (uint16_t)(len_words * 2U);

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

/* 写标签存储区。阻塞 300ms (写比读慢)。
 * 负载: [Reserved(4B)][Bank(1B)][Addr(2B)][Len(2B)][Data...]
 * 响应: [DataLen][Status(1B, 0=成功)][CS] */
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
    r = rfid_wait_frame(300U);
    if (r < 0) {
        return r;
    }

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

    /* 写状态码在响应数据最后一字节，0=成功 */
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

/* 通用命令接口，用于 PLC_RAW。默认超时 500ms。 */
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
        timeout_ms = 500U;
    }

    rfid_send_cmd_data(cmd_id, tx_data, tx_len);
    r = rfid_wait_frame(timeout_ms);
    if (r < 0) {
        return r;
    }

    if (r < 7 || rfid_rx_buf[0] != RFID_HEADER ||
        (rfid_rx_buf[2] != cmd_id && rfid_rx_buf[2] != RFID_CMD_ERROR)) {
        rfid_last_result = RFID_RET_FRAME_ERR;
        return RFID_RET_FRAME_ERR;
    }

    data_len = ((uint16_t)rfid_rx_buf[3] << 8) | rfid_rx_buf[4];
    copy_len = data_len;
    if (copy_len > *rx_len) {
        copy_len = *rx_len;
    }
    if (copy_len > 0U && rx_data != NULL) {
        memcpy(rx_data, (const uint8_t *)rfid_rx_buf + 5, copy_len);
    }
    *rx_len = copy_len;

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

/* USART0 中断：仅清错误标志，正常接收由 DMA 完成 */
void RFID_UART_IRQHandler(void)
{
    rfid_clear_usart_errors();
}

/* RFID 软件看门狗：模块 2s 无响应则触发复位。
 * 复位分两步(非阻塞，避免忙等撞 62.5ms SM 看门狗)：
 *   触发：PA12 断电 + 发 RESET 命令，设恢复时间戳，返回 1
 *   恢复：到时后 PA12 上电 + 重启接收 + 清标志
 * 双保险：软复位(命令)救软卡死，PA12 断电再上电救硬死亡(固件崩溃)。
 * 本函数在所有 EtherCAT 状态下都运行(OP 门控外)，因模块随时可能卡死。 */
uint8_t RFID_WatchdogCheck(void)
{
#if RFID_SOFT_WD_ENABLE
    extern volatile uint32_t g_SysTickCnt;
    static uint32_t s_ResetReadyMs = 0U;

    if (g_RfidWdTriggered != 0U) {
        if ((int32_t)(g_SysTickCnt - s_ResetReadyMs) < 0) {
            return 0U;
        }

        /* 恢复：重新上电 + 重启接收 + 重新计时 */
        RFID_EN_ON();
        rfid_rx_dma_start();
        g_RfidLastAliveMs = g_SysTickCnt;
        g_RfidWdTriggered = 0U;
        return 0U;
    }

    if ((uint32_t)(g_SysTickCnt - g_RfidLastAliveMs) <= RFID_WD_TIMEOUT_MS) {
        return 0U;
    }

    /* 2s 无响应 — 触发复位 */
    g_RfidWdTriggered = 1U;
    RFID_EN_OFF();
    RFID_Reset();
    s_ResetReadyMs = g_SysTickCnt + RFID_WD_POWER_OFF_MS;

    return 1U;
#else
    return 0U;
#endif
}

/* ---- 自动轮询扫描 ----
 * OP 状态下被主循环调用，3 天线轮替 (每 RFID_SCAN_MS 切一个)。
 * 标签丢失延迟清除：连续丢失 >= 2 次 AND 距最后看到 >= 180ms，
 * 避免偶尔读不到就清数据。 */
#define RFID_SCAN_MS 30U
#define RFID_LOST_HOLD_MS 180U
#define RFID_LOST_CLEAR_COUNT 2U

uint8_t g_RfidRssi  [RFID_ANT_COUNT];
uint8_t g_RfidEpcLen[RFID_ANT_COUNT];
uint8_t g_RfidNew   [RFID_ANT_COUNT];
uint8_t g_RfidEpc   [RFID_ANT_COUNT][RFID_EPC_BYTES];

static uint32_t s_RfidLastSeen[RFID_ANT_COUNT];
static uint8_t  s_RfidMissCount[RFID_ANT_COUNT];

static void RFID_ClearChannel(uint8_t idx)
{
    g_RfidRssi[idx] = 0;
    g_RfidEpcLen[idx] = 0;
    g_RfidNew[idx] = 0;
    memset(g_RfidEpc[idx], 0, RFID_EPC_BYTES);
}

void RFID_Scan(void)
{
    static uint8_t ant = 1;
    static uint32_t next = 0;
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
        g_RfidRssi[idx] = tag.rssi;
        g_RfidEpcLen[idx] = tag.epc_len;
        memset(g_RfidEpc[idx], 0, RFID_EPC_BYTES);
        memcpy(g_RfidEpc[idx], tag.epc, tag.epc_len);
        g_RfidNew[idx] = 1;
        s_RfidLastSeen[idx] = g_SysTickCnt;
        s_RfidMissCount[idx] = 0;
    } else {
        if (rfid_last_error == RFID_ERR_NO_TAG || rfid_last_cmd == RFID_RSP_NO_TAG) {
            if (s_RfidMissCount[idx] < 255U) {
                s_RfidMissCount[idx]++;
            }

            if (g_RfidNew[idx] != 0U &&
                s_RfidMissCount[idx] >= RFID_LOST_CLEAR_COUNT &&
                (uint32_t)(g_SysTickCnt - s_RfidLastSeen[idx]) >= RFID_LOST_HOLD_MS) {
                RFID_ClearChannel(idx);
            }
        } else {
#if RFID_DEBUG_LOG
            printf("[RFID] ANT%d err ret=%d rx=%d\n", ant, ret, rfid_rx_len);
#endif
        }
    }

    next = g_SysTickCnt + RFID_SCAN_MS;
}
