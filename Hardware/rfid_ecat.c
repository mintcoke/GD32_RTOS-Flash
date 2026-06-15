#include <stdio.h>
/*
 * rfid_ecat.c -- EC-UHF-B RFID inventory driver over USART0 + DMA.
 *
 * PA12 is the RFID module ENABLE pin from the board pin table:
 * HIGH = module enabled, LOW = standby. It is not an RS485 direction pin.
 */
#include "rfid_ecat.h"
#include "ecat_api.h"
#include "systick.h"
#include <string.h>

#ifndef RFID_DEBUG_LOG
#define RFID_DEBUG_LOG 0
#endif

volatile uint8_t  rfid_rx_buf[RFID_RX_BUF_SIZE];
volatile uint16_t rfid_rx_len = 0;
volatile int16_t  rfid_last_result = RFID_RET_TIMEOUT;
volatile uint8_t  rfid_last_cmd = 0;
volatile uint8_t  rfid_last_error = 0;

volatile uint32_t g_RfidLastAliveMs = 0;
volatile uint8_t  g_RfidWdTriggered = 0;

#define RFID_DMA                DMA0
#define RFID_DMA_CLK            RCU_DMA0

/* GD32 channel enums are 0-based: USART0_TX=DMA0_CH3, USART0_RX=DMA0_CH4. */
#define RFID_TX_DMA_CH          DMA_CH3
#define RFID_RX_DMA_CH          DMA_CH4
#define RFID_RX_DMA_SIZE        RFID_RX_BUF_SIZE

static uint8_t s_TxBuf[RFID_TX_BUF_SIZE];
static volatile uint8_t s_RxDmaBuf[RFID_RX_DMA_SIZE];
static uint16_t s_RxDmaRdPos = 0;

static void rfid_send_cmd_data(uint8_t cmd_id, const uint8_t *data, uint16_t len);

static uint8_t rfid_cs(const uint8_t *d, uint16_t len)
{
    uint16_t sum = 0;

    for (uint16_t i = 0; i < len; i++) {
        sum += d[i];
    }
    return (uint8_t)(sum & 0xFFU);
}

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

static void rfid_rx_dma_start(void)
{
    dma_parameter_struct dma_init_struct;

    usart_dma_receive_config(RFID_USART, USART_RECEIVE_DMA_DISABLE);
    dma_channel_disable(RFID_DMA, RFID_RX_DMA_CH);
    dma_deinit(RFID_DMA, RFID_RX_DMA_CH);
    dma_flag_clear(RFID_DMA, RFID_RX_DMA_CH,
                   DMA_FLAG_G | DMA_FLAG_FTF | DMA_FLAG_HTF | DMA_FLAG_ERR);

    rfid_clear_usart_errors();
    while (RESET != usart_flag_get(RFID_USART, USART_FLAG_RBNE)) {
        (void)usart_data_receive(RFID_USART);
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

static uint16_t rfid_rx_dma_write_pos(void)
{
    uint16_t left = (uint16_t)dma_transfer_number_get(RFID_DMA, RFID_RX_DMA_CH);
    return (uint16_t)((RFID_RX_DMA_SIZE - left) % RFID_RX_DMA_SIZE);
}

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
        if ((int32_t)(g_SysTickCnt - deadline) > 0) {
            dma_channel_disable(RFID_DMA, RFID_TX_DMA_CH);
            usart_dma_transmit_config(RFID_USART, USART_TRANSMIT_DMA_DISABLE);
            return -1;
        }
    }

    deadline = g_SysTickCnt + 10U;
    while (RESET == usart_flag_get(RFID_USART, USART_FLAG_TC)) {
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

int rfid_poll(void)
{
    rfid_clear_usart_errors();
    rfid_rx_dma_service();
    return rfid_frame_status();
}

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
        extern void ECAT_Stack_MainLoop(void);
        ECAT_Stack_MainLoop();
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
        extern void ECAT_Stack_MainLoop(void);
        ECAT_Stack_MainLoop();
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

int RFID_Reset(void)
{
    rfid_send_cmd(RFID_CMD_RESET);
    return 0;
}

int RFID_Poll(void)
{
    return rfid_poll();
}

void RFID_StartCmd(uint8_t cmd_id)
{
    rfid_send_cmd(cmd_id);
}

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

void RFID_UART_IRQHandler(void)
{
    rfid_clear_usart_errors();
}

void RFID_WatchdogCheck(void)
{
    extern volatile uint32_t g_SysTickCnt;
    extern void ECAT_Stack_MainLoop(void);

    if (g_RfidWdTriggered != 0U) {
        return;  /* Already triggered, waiting for hardware WD to recover or MCU reset */
    }

    if ((uint32_t)(g_SysTickCnt - g_RfidLastAliveMs) <= RFID_WD_TIMEOUT_MS) {
        return;  /* RFID module is alive */
    }

    /* RFID module not responding for > 2s — trigger soft reset */
    g_RfidWdTriggered = 1U;

    /* Notify PLC via PDO */
    DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_ERROR;
    DI(TX_RFID_CMD_RESULT) = RFID_ERR_WD_TIMEOUT;

    /* Soft reset RFID module */
    RFID_Reset();

    /* Wait 100ms for module reboot, keep EtherCAT alive */
    {
        uint32_t deadline = g_SysTickCnt + 100U;
        while ((int32_t)(g_SysTickCnt - deadline) < 0) {
            ECAT_Stack_MainLoop();
        }
    }

    /* Reinitialize USART + DMA */
    rfid_rx_dma_start();

    /* Reset watchdog timer (starts 2s cooldown) */
    g_RfidLastAliveMs = g_SysTickCnt;
}

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
    extern volatile uint32_t g_SysTickCnt;

    if (g_SysTickCnt < next) {
        return;
    }

    rfid_tag_t tag;
    RFID_SetAntenna(ant);
    int ret = RFID_Inventory(&tag);
    uint8_t idx = (uint8_t)(ant - 1U);

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

    ant++;
    if (ant > RFID_ANT_COUNT) {
        ant = 1;
    }
    next = g_SysTickCnt + RFID_SCAN_MS;
}
