/*!
    \file    gd32f10x_it.c
    \brief   interrupt service routines for EtherCAT slave
*/

#include "gd32f10x_it.h"
#include "systick.h"
#include "GD32Evb.h"
#include "rfid_ecat.h"
#include "rfid_ecat.h"

/* ══════════════════════════════════════════════════════════
 *  Cortex-M3 Exception Handlers
 * ══════════════════════════════════════════════════════════ */

void NMI_Handler(void)
{
    while(1) {
    }
}

void HardFault_Handler(void)
{
    while(1) {
    }
}

void MemManage_Handler(void)
{
    while(1) {
    }
}

void BusFault_Handler(void)
{
    while(1) {
    }
}

void UsageFault_Handler(void)
{
    while(1) {
    }
}

void DebugMon_Handler(void)
{
    while(1) {
    }
}

/* ══════════════════════════════════════════════════════════
 *  SysTick — 1ms system tick
 *
 *  - Increments global tick counter (g_SysTickCnt)
 *  - Decrements delay counter for delay_1ms()
 * ══════════════════════════════════════════════════════════ */
void SysTick_Handler(void)
{
    g_SysTickCnt++;
    delay_decrement();
}

/* ══════════════════════════════════════════════════════════
 *  EXTI0 — SYNC0 interrupt (DC distributed clock sync)
 * ══════════════════════════════════════════════════════════ */
void EXTI0_IRQHandler(void)
{
    SYNC0_INT_Callback();
}

/* ══════════════════════════════════════════════════════════
 *  EXTI10_15 — ESC AL Event interrupt (PB15 EXTI15)
 * ══════════════════════════════════════════════════════════ */
void USART0_IRQHandler(void)
{
    RFID_UART_IRQHandler();
}

void EXTI10_15_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(ESC_INT_EXTI_LINE)) {
        ESC_INT_Callback();
    }
}

