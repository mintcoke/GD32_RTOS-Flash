/*
 * timer_ecat.c — GD32 TIMER2 configured as 1ms tick for ECAT stack
 *
 * Source project equivalent: TIM3, Prescaler=84, Period=1999
 *   → 168MHz/84 = 2MHz, 2MHz/2000 = 1kHz (1ms)
 *
 * GD32 equivalent: TIMER2, Prescaler=108, Period=1000
 *   → 108MHz/108 = 1MHz, 1MHz/1000 = 1kHz (1ms)
 */

#include "timer_ecat.h"
#include "GD32Evb.h"

void MX_TIM2_Init(void)
{
    rcu_periph_clock_enable(ECAT_TIMER_CLK);

    /* Reset and configure TIMER2 */
    timer_disable(ECAT_TIMER);
    timer_deinit(ECAT_TIMER);

    timer_parameter_struct tim_init;
    timer_struct_para_init(&tim_init);

    tim_init.prescaler         = 108 - 1;      /* 108MHz / 108 = 1MHz */
    tim_init.period            = 1000 - 1;      /* 1MHz / 1000 = 1kHz → 1ms */
    tim_init.clockdivision     = TIMER_CKDIV_DIV1;
    tim_init.counterdirection  = TIMER_COUNTER_UP;
    tim_init.repetitioncounter = 0;

    timer_init(ECAT_TIMER, &tim_init);
    timer_enable(ECAT_TIMER);
}
