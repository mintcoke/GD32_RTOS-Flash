/*
 * GD32Evb.h — GD32F103 Platform Port for TR8253 EtherCAT ESC via SPI
 *
 * This file replaces TR8253Evb.h for GD32F10x targets.
 * All pin assignments are centralized here — verify against actual hardware.
 */

#ifndef _GD32EVB_H_
#define _GD32EVB_H_

/*-----------------------------------------------------------------------------------------
------    Includes (gd32f10x.h MUST come BEFORE esc.h to avoid TRUE/FALSE macro conflict)
-----------------------------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "esc.h"

/*-----------------------------------------------------------------------------------------
------    ESC SPI Access Commands
-----------------------------------------------------------------------------------------*/
#define ESC_RD                    0x02 /**< \brief Read access to ESC */
#define ESC_WR                    0x04 /**< \brief Write access to ESC */

/*=======================================================================
 *  Hardware Pin Mapping — VERIFY AGAINST ACTUAL PCB
 *=======================================================================*/

/* ── SPI0 (APB2, PA5/6/7) — GD32 SPI0 = STM32 SPI1 ── */
#define ESC_SPI              SPI0
#define ESC_SPI_CLK          RCU_SPI0
#define ESC_SPI_GPIO         GPIOA
#define ESC_SPI_GPIO_CLK     RCU_GPIOA
#define ESC_SPI_SCK_PIN      GPIO_PIN_5
#define ESC_SPI_MISO_PIN     GPIO_PIN_6
#define ESC_SPI_MOSI_PIN     GPIO_PIN_7

/* ── SPI Chip Select ── */
#define ESC_CS_GPIO          GPIOA
#define ESC_CS_PIN           GPIO_PIN_4

/* ── ESC Interrupt (AL Event): PB15 → TR8253 pin 48 ── */
#define ESC_INT_GPIO         GPIOB
#define ESC_INT_PIN          GPIO_PIN_15
#define ESC_INT_EXTI_LINE    EXTI_15
#define ESC_INT_IRQn         EXTI10_15_IRQn

/* ── SYNC0 (DC Sync): PA0 → TR8253 pin xx (verify connection) ── */
#define ESC_SYNC0_GPIO       GPIOA
#define ESC_SYNC0_PIN        GPIO_PIN_0
#define ESC_SYNC0_EXTI_LINE  EXTI_0
#define ESC_SYNC0_IRQn       EXTI0_IRQn

/* ── SYNC1 (DC Sync) ── */
#define ESC_SYNC1_GPIO       GPIOA
#define ESC_SYNC1_PIN        GPIO_PIN_1
#define ESC_SYNC1_EXTI_LINE  EXTI_1
#define ESC_SYNC1_IRQn       EXTI1_IRQn

/* ── ECAT 1ms Timer ── */
#define ECAT_TIMER           TIMER2
#define ECAT_TIMER_CLK       RCU_TIMER2

/*=======================================================================
 *  SPI Chip Select Macros
 *=======================================================================*/
#define SELECT_SPI          gpio_bit_reset(ESC_CS_GPIO, ESC_CS_PIN);  /* baked-in ';' — matching original TR8253Evb.h pattern */
#define DESELECT_SPI        gpio_bit_set(ESC_CS_GPIO, ESC_CS_PIN);    /* baked-in ';' — matching original TR8253Evb.h pattern */

/*=======================================================================
 *  Global Interrupt Macros (CMSIS — same across ARM Cortex-M)
 *=======================================================================*/
#define DISABLE_GLOBAL_INT            __disable_irq()
#define ENABLE_GLOBAL_INT             __enable_irq()
#define DISABLE_AL_EVENT_INT          DISABLE_GLOBAL_INT
#define ENABLE_AL_EVENT_INT           ENABLE_GLOBAL_INT

/*=======================================================================
 *  ESC Interrupt Control
 *=======================================================================*/
#define INIT_ESC_INT                  {ESC_INT_Init();}
#define ACK_ESC_INT                   exti_flag_clear(ESC_INT_EXTI_LINE)

#ifndef DISABLE_ESC_INT
#define DISABLE_ESC_INT()             nvic_irq_disable(ESC_INT_IRQn)
#endif
#ifndef ENABLE_ESC_INT
#define ENABLE_ESC_INT()              nvic_irq_enable(ESC_INT_IRQn, 0, 0)
#endif

/*=======================================================================
 *  SYNC0 Interrupt Control (DC mode)
 *=======================================================================*/
#ifdef DC_SUPPORTED

#define INIT_SYNC0_INT                {SYNC0_INT_Init();}
#define Sync0Isr                      EXTI0_IRQHandler
#define SYNC0_IRQn                    EXTI0_IRQn
#define DISABLE_SYNC0_INT             nvic_irq_disable(SYNC0_IRQn)
#define ENABLE_SYNC0_INT              nvic_irq_enable(SYNC0_IRQn, 0, 0)
#define ACK_SYNC0_INT                 exti_flag_clear(ESC_SYNC0_EXTI_LINE)

#define INIT_SYNC1_INT                {SYNC1_INT_Init();}
#define Sync1Isr
#define SYNC1_IRQn                    EXTI1_IRQn
#define DISABLE_SYNC1_INT             nvic_irq_disable(SYNC1_IRQn)
#define ENABLE_SYNC1_INT              nvic_irq_enable(SYNC1_IRQn, 0, 0)
#define ACK_SYNC1_INT

#endif /* DC_SUPPORTED */

/*=======================================================================
 *  Hardware Timer
 *=======================================================================*/
#define ECAT_TIMER_INC_P_MS                1000 /**< \brief 1000 ticks per ms at 1MHz timer clock*/
#define TIMER_INTERVAL      1 /* ms */
#define INIT_ECAT_TIMER     {ECAT_TIM_Init();}
#define STOP_ECAT_TIMER     timer_disable(ECAT_TIMER)
#define START_ECAT_TIMER    timer_enable(ECAT_TIMER)

#ifndef HW_GetTimer
#define HW_GetTimer()       PDI_GetTimer()
#endif

#ifndef HW_ClearTimer
#define HW_ClearTimer()     PDI_ClearTimer()
#endif

/*=======================================================================
 *  ESC Access Macros (16/32 bit, Normal + ISR variants)
 *=======================================================================*/
#define HW_EscReadWord(WordValue, Address) \
    HW_EscRead(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2)
#define HW_EscReadDWord(DWordValue, Address) \
    HW_EscRead(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4)
#define HW_EscReadMbxMem(pData,Address,Len) \
    HW_EscRead(((MEM_ADDR *)(pData)),((UINT16)(Address)),(Len))

#define HW_EscReadWordIsr(WordValue, Address) \
    HW_EscReadIsr(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2)
#define HW_EscReadDWordIsr(DWordValue, Address) \
    HW_EscReadIsr(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4)

#define HW_EscWriteWord(WordValue, Address) \
    HW_EscWrite(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2)
#define HW_EscWriteDWord(DWordValue, Address) \
    HW_EscWrite(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4)
#define HW_EscWriteMbxMem(pData,Address,Len) \
    HW_EscWrite(((MEM_ADDR *)(pData)),((UINT16)(Address)),(Len))

#define HW_EscWriteWordIsr(WordValue, Address) \
    HW_EscWriteIsr(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2)
#define HW_EscWriteDWordIsr(DWordValue, Address) \
    HW_EscWriteIsr(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4)

#endif /* _GD32EVB_H_ */

/*=======================================================================
 *  PROTO macro for function declarations
 *=======================================================================*/
#if defined(_GD32EVB_) && (_GD32EVB_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif

PROTO UINT8  HW_Init(void);
PROTO void   HW_Release(void);
PROTO UINT16 HW_GetALEventRegister(void);
PROTO UINT16 HW_GetALEventRegister_Isr(void);
PROTO void   HW_SetLed(UINT8 RunLed, UINT8 ErrLed);

PROTO void HW_EscRead(MEM_ADDR *pData, UINT16 Address, UINT16 Len);
PROTO void HW_EscReadIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len);
PROTO void HW_EscWrite(MEM_ADDR *pData, UINT16 Address, UINT16 Len);
PROTO void HW_EscWriteIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len);

PROTO UINT16 PDI_GetTimer(void);
PROTO void   PDI_ClearTimer(void);

/* ── Interrupt handler functions ── */
PROTO void ESC_INT_Callback(void);
PROTO void SYNC0_INT_Callback(void);

PROTO volatile uint32_t g_EscIntCount;

#undef PROTO

#include "ecat_hw_if.h"
