#ifndef _ECAT_API_H_
#define _ECAT_API_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Sonar configuration */
#define SONAR_BAUD       115200
#define SONAR_TRIG_MS    50
#define SONAR_NO_DATA    0xFFFF

/* PDO size */
#define PDO_TX_UINT16    510
#define PDO_RX_UINT16    510
#define PDO_TX_MAX       0x400
#define PDO_RX_MAX       0x400

/* Unified PDO access: DI(n) / DO(n), indexes are UINT16 words. */
#define _DI_BASE         ((uint16_t*)(&DIUnit10x6000.u16SubIndex0 + 1))
#define _DO_BASE         ((uint16_t*)(&DOUnit20x7000.u16SubIndex0 + 1))
#define DI(n)            (_DI_BASE[(n)])
#define DO(n)            (_DO_BASE[(n)])

/* TxPDO (DI): slave -> master */
#define TX_HEARTBEAT     0   /* heartbeat counter */

/* RFID ANT1: DI(1..34), EPC has 31 UINT16 = 62 bytes */
#define TX_RFID1_RSSI    1   /* RFID tag RSSI */
#define TX_RFID1_EPC_LEN 2   /* RFID EPC length in bytes */
#define TX_RFID1_NEW     3   /* new/valid tag flag */
#define TX_RFID1_EPC     4   /* RFID EPC data start */

/* RFID ANT2: DI(35..68), same format as ANT1 */
#define TX_RFID2_RSSI    35  /* RFID tag RSSI */
#define TX_RFID2_EPC_LEN 36  /* RFID EPC length in bytes */
#define TX_RFID2_NEW     37  /* new/valid tag flag */
#define TX_RFID2_EPC     38  /* RFID EPC data start */

/* RFID ANT3: DI(69..102), same format as ANT1 */
#define TX_RFID3_RSSI    69  /* RFID tag RSSI */
#define TX_RFID3_EPC_LEN 70  /* RFID EPC length in bytes */
#define TX_RFID3_NEW     71  /* new/valid tag flag */
#define TX_RFID3_EPC     72  /* RFID EPC data start */

/* RFID command response: DI(103..138), slave -> master */
#define TX_RFID_CMD_ECHO     103 /* echoed command code */
#define TX_RFID_CMD_STATUS   104 /* 0=idle, 1=busy, 2=ok, 3=error */
#define TX_RFID_CMD_RESULT   105 /* driver return code or module error */
#define TX_RFID_CMD_DATA_LEN 106 /* response data length in bytes */
#define TX_RFID_CMD_DATA     107 /* response data start, 32 UINT16 = 64 bytes */

/* Backward-compatible aliases for ANT1. */
#define TX_RFID_RSSI     TX_RFID1_RSSI
#define TX_RFID_EPC_LEN  TX_RFID1_EPC_LEN
#define TX_RFID_NEW      TX_RFID1_NEW
#define TX_RFID_EPC      TX_RFID1_EPC

/* RxPDO (DO): master -> slave */
#define RX_RFID_CMD      0   /* 0=idle, 1..31=simple RFID commands, 100=raw EC-UHF-B command */
#define RX_RFID_ANT      1   /* antenna 1..3, 0=keep current */
#define RX_RFID_ADDR     2   /* word address or simple command value */
#define RX_RFID_WORDS    3   /* word count or simple command value */
#define RX_RFID_DATA     4   /* write data start, 30 UINT16 = 60 bytes */

#define RFID_PLC_CMD_NONE       0
#define RFID_PLC_CMD_GET_INFO   1
#define RFID_PLC_CMD_READ_TID   2
#define RFID_PLC_CMD_READ_USER  3
#define RFID_PLC_CMD_WRITE_USER 4
#define RFID_PLC_CMD_WRITE_EPC  5
#define RFID_PLC_CMD_SET_EPC_LEN 6
#define RFID_PLC_CMD_SET_POWER  7
#define RFID_PLC_CMD_GET_POWER  8
#define RFID_PLC_CMD_SET_REGION 9
#define RFID_PLC_CMD_GET_REGION 10
#define RFID_PLC_CMD_SET_CHANNEL 11
#define RFID_PLC_CMD_GET_CHANNEL 12
#define RFID_PLC_CMD_SET_HOP    13
#define RFID_PLC_CMD_GET_HOP    14
#define RFID_PLC_CMD_SET_MODE   15
#define RFID_PLC_CMD_GET_MODE   16
#define RFID_PLC_CMD_INVENTORY  17
#define RFID_PLC_CMD_STOP_POLL  18
#define RFID_PLC_CMD_SET_CH_LIST 19
#define RFID_PLC_CMD_GET_CH_LIST 20
#define RFID_PLC_CMD_SET_RX     21
#define RFID_PLC_CMD_GET_RX     22
#define RFID_PLC_CMD_TEST_RSSI  23
#define RFID_PLC_CMD_SET_CARRIER 24
#define RFID_PLC_CMD_RESET      25
#define RFID_PLC_CMD_SELECT_EPC 30
#define RFID_PLC_CMD_CLEAR_SELECT 31
#define RFID_PLC_CMD_RAW        100

#define RFID_PLC_STATUS_IDLE    0
#define RFID_PLC_STATUS_BUSY    1
#define RFID_PLC_STATUS_OK      2
#define RFID_PLC_STATUS_ERROR   3

#define RFID_ERR_WD_TIMEOUT    0xE001U

typedef struct {
    float    sonar_zero_offset;
    float    sonar_scale_factor;
    uint16_t modbus_slave_addr;
    uint16_t baudrate_code;
    uint32_t reserved[8];
    uint32_t checksum;
} PersistentParams_t;

extern PersistentParams_t g_PersistentParams;

void ECAT_Stack_Init(void);
void ECAT_Stack_MainLoop(void);

typedef void (*ecat_periodic_cb_t)(void);
void ECAT_RegisterPeriodicTask(ecat_periodic_cb_t cb);

typedef void (*ecat_safe_output_cb_t)(void);
void ECAT_RegisterSafeOutput(ecat_safe_output_cb_t cb);

typedef void (*sonar_fail_cb_t)(void);
void ECAT_RegisterSonarFailCallback(sonar_fail_cb_t cb);
void ECAT_CheckSonarFail(void);

uint32_t ECAT_GetParamSize(void);
void*    ECAT_GetParamPtr(void);
void     ECAT_SetParamDirty(void);

uint8_t ECAT_IsOpRunning(void);
uint8_t ECAT_GetAlState(void);
uint16_t ECAT_GetAlStatusCode(void);
uint16_t ECAT_GetLocalErrorCode(void);
uint16_t ECAT_GetLastAlEvent(void);
uint16_t ECAT_GetLastAlControl(void);
uint32_t ECAT_GetAlControlCount(void);
uint32_t ECAT_GetEscIntCount(void);
uint16_t ECAT_ReadEscAlStatus(void);
uint16_t ECAT_ReadEscEventMask(void);
uint16_t ECAT_ReadEscEepromStatus(void);
uint16_t ECAT_GetSyncType(void);

/* SSC bridge */
extern void (*g_pfnPeriodicTask)(void);
extern void (*g_pfnSafeOutput)(void);
extern void (*g_pfnTxPdoMapping)(uint16_t* pData);
extern void (*g_pfnRxPdoMapping)(uint16_t* pData);
void APPL_CoeTxPdoMapping(uint16_t* pData);
void APPL_CoeRxPdoMapping(uint16_t* pData);

#ifdef __cplusplus
}
#endif

#endif /* _ECAT_API_H_ */
