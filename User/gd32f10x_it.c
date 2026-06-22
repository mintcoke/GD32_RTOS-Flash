/*
 * gd32f10x_it.c — 中断服务程序
 */

#include "gd32f10x_it.h"
#include "systick.h"
#include "GD32Evb.h"
#include "rfid_ecat.h"

/* ---- Cortex-M3 异常处理 ---- */

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

/* ---- SysTick 1ms — 递增 g_SysTickCnt + delay_1ms 计数 ---- */
void SysTick_Handler(void)
{
    g_SysTickCnt++;
    delay_decrement();
}

/* ---- EXTI0 — SYNC0 中断 (DC 分布式时钟同步) ---- */
void EXTI0_IRQHandler(void)
{
    SYNC0_INT_Callback();
}

/* ---- USART0 — RFID 模块接收错误标志清除 ---- */
void USART0_IRQHandler(void)
{
    RFID_UART_IRQHandler();
}

/* ---- EXTI10_15 — ESC AL 事件中断 (PB15) ---- */
void EXTI10_15_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(ESC_INT_EXTI_LINE)) {
        ESC_INT_Callback();
    }
}
