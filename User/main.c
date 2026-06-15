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

/*───────────────────────────────────────────────────────────
 * ECAT Monitor — only prints AL state changes
 *───────────────────────────────────────────────────────────*/
static void ECAT_Monitor(void)
{
    static uint8_t last_al = 0xFFU;
    uint8_t al = ECAT_GetAlState();

    if (al != last_al) {
        printf("[ECAT] AL=0x%02X\r\n", al);
        last_al = al;
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
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_TIM2_Init();

    ECAT_Stack_Init();
    ECAT_RegisterPeriodicTask(DO_LED_Ctrl);
    ECAT_RegisterSafeOutput(DO_LED_Off);

    RFID_Init();

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
