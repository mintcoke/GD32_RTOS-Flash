/*
 * gpio_ecat.h — GPIO + EXTI init for ESC interface
 *
 * ESC pins (verify against actual hardware):
 *   PA4 = SPI CS
 *   PA2 = ESC_INT (EXTI2, falling edge)
 *   PA0 = SYNC0   (EXTI0, falling edge)
 *   PA1 = SYNC1   (EXTI1, falling edge)
 */

#ifndef __GPIO_ECAT_H__
#define __GPIO_ECAT_H__

#include "gd32f10x.h"
#include "GD32Evb.h"

void MX_GPIO_Init(void);

#endif /* __GPIO_ECAT_H__ */
