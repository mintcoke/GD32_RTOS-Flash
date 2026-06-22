/*
 * main.c — EtherCAT + RFID 从站主程序
 *
 * GD32F103CBT6 + TR8253(ESC, SPI1) + RFID 模块(USART0+DMA)。
 *
 * 主循环: ECAT_Stack_MainLoop → RFID_WatchdogCheck(全状态) →
 *         OP 下 RFID_EcatCmdTask/RFID_Scan → ECAT_Monitor → WDG_Feed
 * RFID 看门狗放 OP 门控外: 模块任何状态都可能卡死，进 OP 前就要保活。
 * RFID 命令阻塞期间由 ECAT_KeepAlive() 保活，不掉线。
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
#include "wdg_ecat.h"

/* AL 状态变化时打印一条，避免刷屏。
 * AL=状态 SC=错误码 LE=本地错误码 */
static void ECAT_Monitor(void)
{
    static uint8_t last_al = 0xFFU;
    uint8_t al = ECAT_GetAlState();

    if (al != last_al) {
        uint16_t status_code = ECAT_GetAlStatusCode();
        uint16_t local_err  = ECAT_GetLocalErrorCode();

        printf("[ECAT] AL=0x%02X SC=0x%04X LE=0x%04X\r\n",
               al, status_code, local_err);

        last_al = al;
    }
}

int main(void)
{
    SystemInit();                           /* 时钟 72MHz */
    systick_config();                       /* SysTick 1ms */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    UART_Init();                            /* 调试串口 USART2 */
    MX_GPIO_Init();                         /* ESC 中断引脚等 */
    MX_SPI1_Init();                         /* SPI1 → ESC */
    MX_TIM2_Init();

    ECAT_Stack_Init();                      /* ESC + SSC 状态机(内部挂接 APPL_* 回调) */

    RFID_Init();                            /* USART0 + DMA + 天线 GPIO */
    WDG_Init();                             /* FWDGT 5s 超时 */

    /* 主循环 — 顺序不可调换，ECAT_Stack_MainLoop 须最高频调用 */
    while (1) {
        ECAT_Stack_MainLoop();

        /* RFID 软件看门狗(全状态)：模块卡死及时恢复 */
        if (RFID_WatchdogCheck()) {
            DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_ERROR;
            DI(TX_RFID_CMD_RESULT) = RFID_ERR_WD_TIMEOUT;
        }

        /* OP 状态：优先处理 PLC 命令，无命令才自动扫描 */
        if ((ECAT_GetAlState() & 0x0FU) == 0x08U) {
            if (!RFID_EcatCmdTask()) {
                RFID_Scan();
            }
        }

        ECAT_Monitor();
        WDG_Feed();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}
