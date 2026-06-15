/*
 * main.c — EtherCAT + RFID Slave (GD32F103CBT6 + TR8253 + EC-UHF-B)
 */
#include "gd32f10x.h"
#include "systick.h"
#include "spi_ecat.h"
#include "timer_ecat.h"
#include "gpio_ecat.h"
#include "ecat_def.h"
#include "SSC-Device.h"
#include "applInterface.h"
#include "GD32Evb.h"
#include "Application.h"
#include "ecat_api.h"
#include "uart.h"
#include "rfid_ecat.h"

#ifndef ECAT_DEBUG_LOG
#define ECAT_DEBUG_LOG 0
#endif

/*───────────────────────────────────────────────────────────
 * ECAT Monitor
 *───────────────────────────────────────────────────────────*/
static void ECAT_Monitor(void)
{
    static uint8_t last_al = 0xFFU;
    static uint16_t last_code = 0xFFFFU;
    static uint16_t last_local = 0xFFFFU;
    static uint32_t last_pdo_ms = 0U;
    uint8_t al = ECAT_GetAlState();

    if (al != last_al
#if ECAT_DEBUG_LOG
        || (g_SysTickCnt - last_diag_ms) >= 1000U
#endif
    ) {
        uint16_t code = ECAT_GetAlStatusCode();
        uint16_t local = ECAT_GetLocalErrorCode();

        if (al != last_al || code != last_code || local != last_local) {
            printf("[ECAT] AL=0x%02X code=0x%04X local=0x%04X\r\n", al, code, local);
            if ((al & 0x0FU) == 0x08U) {
                printf("[HB] OP!\r\n");
            }
            last_al = al;
            last_code = code;
            last_local = local;
        }

#if ECAT_DEBUG_LOG
        if ((al & 0x0FU) != 0x08U && (g_SysTickCnt - last_diag_ms) >= 1000U) {
            uint16_t esc_al = ECAT_ReadEscAlStatus();
            uint16_t mask = ECAT_ReadEscEventMask();
            uint16_t eeprom = ECAT_ReadEscEepromStatus();

            printf("[ECATDBG] escAL=0x%04X evt=0x%04X lastCtl=0x%04X ctlCnt=%lu irq=%lu mask=0x%04X ee=0x%04X\r\n",
                   esc_al,
                   ECAT_GetLastAlEvent(),
                   ECAT_GetLastAlControl(),
                   (unsigned long)ECAT_GetAlControlCount(),
                   (unsigned long)ECAT_GetEscIntCount(),
                   mask,
                   eeprom);
            last_diag_ms = g_SysTickCnt;
        }
#endif
    }

    /* Periodic PDO diagnostic: print heartbeat and key values every 2s in OP */
    if ((al & 0x0FU) == 0x08U && (g_SysTickCnt - last_pdo_ms) >= 2000U) {
        printf("[PDO] DI(0)=0x%04X DI(1)=0x%04X sync=0x%04X intCnt=%lu\r\n",
               DI(0), DI(1),
               (unsigned)ECAT_GetSyncType(),
               (unsigned long)ECAT_GetEscIntCount());
        last_pdo_ms = g_SysTickCnt;
    }
}

/*───────────────────────────────────────────────────────────
 * main
 *───────────────────────────────────────────────────────────*/
int main(void)
{
    SystemInit();
    systick_config();
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    UART_Init();
    printf("\r\n=== ECAT Slave Start ===\r\n");
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_TIM2_Init();

    printf("[ECAT] Stack init... ");
    ECAT_Stack_Init();
    printf("OK (AL=0x%02X)\r\n", ECAT_GetAlState());
    ECAT_RegisterPeriodicTask(DO_LED_Ctrl);
    ECAT_RegisterSafeOutput(DO_LED_Off);

    RFID_Init();
    printf("[ECAT] Running, waiting for master...\r\n");

    while (1) {
        ECAT_Stack_MainLoop();
        if ((ECAT_GetAlState() & 0x0FU) == 0x08U) {
            if (!RFID_EcatCmdTask()) {
                RFID_Scan();
            }
        }
        ECAT_Monitor();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}
