/*
 * wdg_ecat.h — GD32F103 FWDGT (Independent Watchdog) interface
 */
#ifndef __WDG_ECAT_H__
#define __WDG_ECAT_H__

#include "gd32f10x.h"

void WDG_Init(void);   /* Initialize FWDGT, 5s timeout */
void WDG_Feed(void);   /* Reload FWDGT counter */

#endif /* __WDG_ECAT_H__ */
