/*
 * sonar_ecat.h — Bare-metal sonar driver for ECAT integration
 *
 * Protocol: 4-byte frames over UART (USART2 / RS485)
 *   Frame: [0xFF] [DistH] [DistL] [Checksum]
 *   Checksum: (FF + DistH + DistL) & 0xFF
 *
 * Replaces the FreeRTOS-based Modbus sonar.c.
 */

#ifndef __SONAR_ECAT_H__
#define __SONAR_ECAT_H__

#include "gd32f10x.h"

/* Sonar baud rate (configurable in ecat_api.h) */
#define SONAR_BAUD       115200

/* Sonar trigger interval in ms (>33ms) */
#define SONAR_TRIG_MS    50

/* Magic value indicating no valid data / timeout */
#define SONAR_NO_DATA    0xFFFF

/* Sonar UART receive buffer size */
#define SONAR_RX_BUF_SIZE 64

/* Global sonar state */
extern uint16_t g_SonarDist;
extern uint8_t  g_SonarBuf[4];

/* Sonar receive buffer */
extern uint8_t  sonar_rx_buf[SONAR_RX_BUF_SIZE];
extern volatile uint16_t sonar_rx_len;

/* API */
void Sensor_UART_Init(uint32_t baud);
void Sonar_Task(void);
void Sonar_RecvTick(void);

/* Interrupt handler (called from gd32f10x_it.c) */
void Sonar_UART_IRQHandler(void);

#endif /* __SONAR_ECAT_H__ */
