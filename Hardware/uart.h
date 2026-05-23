#ifndef __UART_H
#define __UART_H

#include "gd32f10x.h"
#include <stdio.h>

/* RS485 收发控制引脚 PA5 */
#define RS485_DE_PORT   GPIOA
#define RS485_DE_PIN    GPIO_PIN_5
#define RS485_TX_EN()   gpio_bit_set(RS485_DE_PORT, RS485_DE_PIN)
#define RS485_RX_EN()   gpio_bit_reset(RS485_DE_PORT, RS485_DE_PIN)

/* RS485 接收缓冲区大小 */
#define RS485_RX_BUF_SIZE 64

/* 外部声明接收缓冲区和状态 */
extern uint8_t  rs485_rx_buf[RS485_RX_BUF_SIZE];
extern volatile uint16_t rs485_rx_len;

/* 函数声明 */
void UART_Init(void);
void RS485_SendData(uint8_t *data, uint16_t len);

#endif /* __UART_H */
