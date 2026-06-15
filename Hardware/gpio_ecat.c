/*
 * gpio_ecat.c — GD32 GPIO & EXTI initialization for EtherCAT ESC interface
 *
 * GD32 EXTI requires: gpio_exti_source_select() then exti_init()
 */

#include "gpio_ecat.h"

void MX_GPIO_Init(void)
{
    /* Enable GPIO + AF clocks */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);

    /* ── ESC_INT (PB15): EXTI15, falling edge ── */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_15);
    exti_init(EXTI_15, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_enable(EXTI_15);
    exti_flag_clear(EXTI_15);
    nvic_irq_enable(EXTI10_15_IRQn, 1, 0);  /* lower than USART0 */

    /* ── SYNC0 (PA0): EXTI0, falling edge ── */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_enable(EXTI_0);
    exti_flag_clear(EXTI_0);
    nvic_irq_enable(EXTI0_IRQn, 0, 1);
}
