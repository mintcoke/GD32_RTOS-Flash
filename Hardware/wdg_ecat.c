/*
 * wdg_ecat.c — GD32F103 FWDGT (Independent Watchdog) implementation
 *
 * Clock: IRC40K (40kHz internal RC)
 * Prescaler: DIV64 → 40kHz/64 = 625 Hz
 * Reload: 3125 → timeout = 3125/625 = 5.0s
 *
 * FWDGT cannot be stopped once started — hardware guarantee.
 */
#include "wdg_ecat.h"
#include "gd32f10x_fwdgt.h"

#define WDG_PRESCALER   FWDGT_PSC_DIV64
#define WDG_RELOAD      3125U   /* 5.0s at 625 Hz */

void WDG_Init(void)
{
    /* Enable write access to FWDGT registers */
    fwdgt_write_enable();

    /* Configure prescaler and reload value */
    fwdgt_prescaler_value_config(WDG_PRESCALER);
    fwdgt_reload_value_config(WDG_RELOAD);

    /* Reload the counter now */
    fwdgt_counter_reload();

    /* Start FWDGT — cannot be stopped after this */
    fwdgt_enable();
}

void WDG_Feed(void)
{
    fwdgt_counter_reload();
}
