/*
 * sonar_ecat.c — Bare-metal sonar driver for ECAT integration
 *
 * Adapted from source project usart.c (STM32 USART6 DMA) to
 * GD32 USART1 interrupt-based receive with RS485 direction control.
 *
 * Protocol: Simple 4-byte checksum frame
 *   Byte0: 0xFF (header)
 *   Byte1: Distance high byte
 *   Byte2: Distance low byte
 *   Byte3: (Byte0 + Byte1 + Byte2) & 0xFF (checksum)
 */

#include "sonar_ecat.h"
#include "uart.h"
#include "systick.h"

/* ═══════════════════════════════════════════════════════════
 *  Global State
 * ═══════════════════════════════════════════════════════════ */
uint16_t g_SonarDist = SONAR_NO_DATA;
uint8_t  g_SonarBuf[4] = {0};

uint8_t  sonar_rx_buf[SONAR_RX_BUF_SIZE];
volatile uint16_t sonar_rx_len = 0;

/* ═══════════════════════════════════════════════════════════
 *  Sensor_UART_Init
 *
 *  Initialize USART1 for sonar communication.
 *  Reuses existing RS485 hardware from uart.c (PB10=TX, PB11=RX, PA5=DE).
 *  Configures interrupt-based receive.
 * ═══════════════════════════════════════════════════════════ */
void Sensor_UART_Init(uint32_t baud)
{
    /* Reuse UART_Init for the basic setup, then reconfigure baud if needed */
    /* For now: USART1 is already configured by UART_Init() in the main init path.
     * If the existing UART_Init has already run, USART1 is ready at 115200.
     * We just need to ensure the interrupt is enabled. */

    /* Enable USART1 receive interrupt (RBNE = RX buffer not empty) */
    nvic_irq_enable(USART1_IRQn, 0, 0);
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    usart_enable(USART1);

    /* Clear any stale data */
    sonar_rx_len = 0;
}

/* ═══════════════════════════════════════════════════════════
 *  Sonar_Task — Called from main loop every cycle
 *
 *  Implements a 3-state trigger state machine:
 *    State 0: Send break → transition to state 1
 *    State 1: Wait 1ms, clear break → transition to state 2
 *    State 2: Wait SONAR_TRIG_MS, → transition to state 0
 *
 *  Also calls Sonar_RecvTick() to process received data.
 * ═══════════════════════════════════════════════════════════ */
void Sonar_Task(void)
{
    static uint32_t s_Tick = 0;
    static uint8_t  s_State = 0;
    uint32_t now = g_SysTickCnt;

    switch (s_State) {
    case 0:
        /* Send break condition to trigger sonar sensor */
        USART_CTL0(USART1) |= USART_CTL0_SBKCMD;
        s_Tick = now;
        s_State = 1;
        break;

    case 1:
        if (now - s_Tick < 1) break;
        /* Clear break condition */
        USART_CTL0(USART1) &= ~USART_CTL0_SBKCMD;
        /* Clear any garbage received during break */
        (void)usart_data_receive(USART1);
        s_State = 2;
        break;

    case 2:
        if (now - s_Tick >= SONAR_TRIG_MS) s_State = 0;
        break;
    }

    Sonar_RecvTick();
}

/* ═══════════════════════════════════════════════════════════
 *  Sonar_RecvTick — Parse received sonar data
 *
 *  Scans the interrupt-filled buffer for valid 4-byte frames.
 *  Frame format: [0xFF] [DistH] [DistL] [Checksum]
 *    Checksum = (0xFF + DistH + DistL) & 0xFF
 *
 *  On valid frame: updates g_SonarDist, toggles DOCH15 indicator.
 * ═══════════════════════════════════════════════════════════ */
void Sonar_RecvTick(void)
{
    static uint8_t  s_Buf[4];
    static uint32_t s_Pos = 0;
    uint32_t pos = sonar_rx_len;

    while (s_Pos != pos) {
        uint8_t ch = sonar_rx_buf[s_Pos];
        s_Pos = (s_Pos + 1) & (SONAR_RX_BUF_SIZE - 1);

        /* Shift buffer: sliding window of 4 bytes */
        s_Buf[0] = s_Buf[1];
        s_Buf[1] = s_Buf[2];
        s_Buf[2] = s_Buf[3];
        s_Buf[3] = ch;

        /* Copy to global debug buffer */
        g_SonarBuf[0] = s_Buf[0];
        g_SonarBuf[1] = s_Buf[1];
        g_SonarBuf[2] = s_Buf[2];
        g_SonarBuf[3] = s_Buf[3];

        /* Validate frame: header=0xFF, checksum matches */
        if (s_Buf[0] == 0xFF && ((s_Buf[0] + s_Buf[1] + s_Buf[2]) & 0xFF) == s_Buf[3]) {
            g_SonarDist = ((uint16_t)s_Buf[1] << 8) | s_Buf[2];
            /* Toggle sonar indicator — DOCH15 (PD10 on source, TODO for GD32) */
        }
    }
}

/* ═══════════════════════════════════════════════════════════
 *  Sonar_UART_IRQHandler — USART1 interrupt service routine
 *  Called from USART1_IRQHandler in gd32f10x_it.c
 * ═══════════════════════════════════════════════════════════ */
void Sonar_UART_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)) {
        uint8_t data = (uint8_t)usart_data_receive(USART1);

        if (sonar_rx_len < SONAR_RX_BUF_SIZE) {
            sonar_rx_buf[sonar_rx_len++] = data;
        } else {
            /* Buffer full — wrap around */
            sonar_rx_len = 0;
        }
    }

    /* Clear overrun error if present */
    if (RESET != usart_flag_get(USART1, USART_FLAG_ORERR)) {
        (void)usart_data_receive(USART1);  /* Read to clear */
    }
}
