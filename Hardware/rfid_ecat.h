/*
 * rfid_ecat.h -- UHF RFID module driver (USART0, PA9/PA10)
 *
 * Protocol: EC-UHF-B, 115200bps, 8N1
 * Frame: 0xAA + Dir + CmdID + DataLen(2B) + Data(N) + Checksum + 0xDD
 */

#ifndef __RFID_ECAT_H
#define __RFID_ECAT_H

#include "gd32f10x.h"
#include <stdint.h>

/* ------ Hardware pins ------ */
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
/* Compatibility aliases: PA12 is module enable, not TX/RX direction. */
#define RFID_EN_TX()         RFID_EN_ON()
#define RFID_EN_RX()         RFID_EN_ON()

/* Antenna selection: PB12=S3, PB13=S2, PB14=S1 */
#define RFID_ANT_PORT        GPIOB
#define RFID_ANT_S1          GPIO_PIN_14
#define RFID_ANT_S2          GPIO_PIN_13
#define RFID_ANT_S3          GPIO_PIN_12

/* ------ Frame constants ------ */
#define RFID_HEADER          0xAA
#define RFID_FOOTER          0xDD
#define RFID_DIR_HOST         0x00
#define RFID_DIR_MODULE       0x01
#define RFID_DIR_NOTIFY       0x02

/* ------ Command IDs ------ */
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
#define RFID_CMD_SET_POWER    0xB6
#define RFID_CMD_GET_POWER    0xB7
#define RFID_CMD_SET_RX       0xF0
#define RFID_CMD_GET_RX       0xF1
#define RFID_CMD_TEST_RSSI    0xF3
#define RFID_CMD_SET_MODE     0xF5
#define RFID_CMD_GET_MODE     0xF6
#define RFID_CMD_READ_TAG     0x39
#define RFID_CMD_WRITE_TAG    0x49
#define RFID_CMD_SET_IO       0x1A
#define RFID_CMD_ERROR        0xFF

/* ------ Driver return codes ------ */
#define RFID_RET_OK            0
#define RFID_RET_TIMEOUT      -1
#define RFID_RET_MODULE_ERR   -2
#define RFID_RET_FRAME_ERR    -3
#define RFID_RET_MODULE_RSP   -4

/* Common module error codes */
#define RFID_ERR_NO_TAG        0x15
#define RFID_RSP_NO_TAG        0xF5

/* ------ Memory banks ------ */
#define RFID_BANK_RFU         0x00
#define RFID_BANK_EPC         0x01
#define RFID_BANK_TID         0x02
#define RFID_BANK_USER        0x03

/* ------ Max frame sizes ------ */
#define RFID_RX_BUF_SIZE      256
#define RFID_TX_BUF_SIZE      256
#define RFID_EPC_MAX_LEN      62

/* ------ Tag info ------ */
typedef struct {
    uint8_t  epc_len;
    uint8_t  epc[RFID_EPC_MAX_LEN];
    uint8_t  rssi;
    uint16_t pc;
} rfid_tag_t;

/* ------ Public API ------ */
void RFID_Init(void);
void RFID_SetAntenna(uint8_t ant);
int  RFID_Reset(void);
int  RFID_Inventory(rfid_tag_t *tag);
int  RFID_ReadTag(uint8_t bank, uint16_t addr, uint16_t len_words, uint8_t *data);
int  RFID_WriteTag(uint8_t bank, uint16_t addr, uint16_t len_words, const uint8_t *data);
int  RFID_Command(uint8_t cmd_id,
                  const uint8_t *tx_data,
                  uint16_t tx_len,
                  uint8_t *rx_data,
                  uint16_t *rx_len,
                  uint32_t timeout_ms);

/* Non-blocking API (for use inside ECAT main loop) */
void RFID_StartCmd(uint8_t cmd_id);
int  RFID_Poll(void);        /* returns 0=busy, >0=ok, <0=err */
int  RFID_ParseTag(rfid_tag_t *tag);

void RFID_UART_IRQHandler(void);
void RFID_Scan(void);

/* Tag data globals (per-antenna) */
#define RFID_ANT_COUNT    3U
#define RFID_EPC_BYTES    62U
extern uint8_t g_RfidRssi  [RFID_ANT_COUNT];
extern uint8_t g_RfidEpcLen[RFID_ANT_COUNT];
extern uint8_t g_RfidNew   [RFID_ANT_COUNT];
extern uint8_t g_RfidEpc   [RFID_ANT_COUNT][RFID_EPC_BYTES];

/* ------ Software Watchdog ------ */
#define RFID_WD_TIMEOUT_MS     2000U   /* Trigger soft reset after 2s no response */

extern volatile uint32_t g_RfidLastAliveMs;  /* Last successful RFID communication timestamp */
extern volatile uint8_t  g_RfidWdTriggered;  /* Watchdog soft-reset triggered flag */
void RFID_WatchdogCheck(void);              /* Check RFID module liveness, soft-reset if needed */

/* Debug helpers */
extern volatile uint8_t  rfid_rx_buf[RFID_RX_BUF_SIZE];
extern volatile uint16_t rfid_rx_len;
extern volatile int16_t  rfid_last_result;
extern volatile uint8_t  rfid_last_cmd;
extern volatile uint8_t  rfid_last_error;

#endif /* __RFID_ECAT_H */
