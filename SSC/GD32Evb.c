/*
 * GD32Evb.c — GD32F103 Platform Implementation for TR8253 ESC via SPI
 *
 * This file replaces TR8253Evb.c for GD32F10x targets.
 * Translates all STM32 HAL calls to GD32 Standard Peripheral Library.
 */

/*--------------------------------------------------------------------------------------
------    Includes
--------------------------------------------------------------------------------------*/
#include "gd32f10x.h"          /* BEFORE SSC headers: avoid TRUE/FALSE macro conflict */
#include "ecat_def.h"
#include "ecatslv.h"

#define _GD32EVB_ 1
#include "GD32Evb.h"
#undef _GD32EVB_

#include "ecatappl.h"
#include "ecat_api.h"
#include "spi_ecat.h"
#include "timer_ecat.h"

/*--------------------------------------------------------------------------------------
------    Internal Types and Defines
--------------------------------------------------------------------------------------*/
typedef union
{
    unsigned short    Word;
    unsigned char     Byte[2];
} UBYTETOWORD;

typedef union
{
    UINT8             Byte[2];
    UINT16            Word;
} UALEVENT;

/*--------------------------------------------------------------------------------------
------    Internal Variables
--------------------------------------------------------------------------------------*/
UALEVENT         EscALEvent;  /* AL Event register (0x220) content, updated on each ESC access */

/*--------------------------------------------------------------------------------------
------    Local Function: RxTxSpiData (single byte SPI exchange)
--------------------------------------------------------------------------------------*/
static UINT8 RxTxSpiData(UINT8 MosiByte)
{
    uint32_t timeout;

    /* Ensure SPI is enabled */
    spi_enable(ESC_SPI);

    /* Wait for TX buffer empty */
    timeout = 100000;
    while (spi_i2s_flag_get(ESC_SPI, SPI_FLAG_TBE) == RESET)
    {
        if (--timeout == 0) return 0; /* SPI hardware fault, timeout */
    }
    spi_i2s_data_transmit(ESC_SPI, MosiByte);

    /* Wait for RX buffer not empty */
    timeout = 100000;
    while (spi_i2s_flag_get(ESC_SPI, SPI_FLAG_RBNE) == RESET)
    {
        if (--timeout == 0) return 0; /* ESC not responding, timeout */
    }
    return (UINT8)spi_i2s_data_receive(ESC_SPI);
}

/*--------------------------------------------------------------------------------------
------    Local Function: AddressingEsc
--------------------------------------------------------------------------------------*/
static void AddressingEsc(UINT16 Address, UINT8 Command)
{
    VARVOLATILE UBYTETOWORD tmp;
    tmp.Word = (Address << 3) | Command;

    SELECT_SPI;

    /* Send first address/command byte, receive first AL Event byte */
    EscALEvent.Byte[0] = RxTxSpiData(tmp.Byte[1]);
    EscALEvent.Byte[1] = RxTxSpiData(tmp.Byte[0]);
}

/*--------------------------------------------------------------------------------------
------    Local Function: GetInterruptRegister
--------------------------------------------------------------------------------------*/
static void GetInterruptRegister(void)
{
    VARVOLATILE UINT8 dummy;
    HW_EscRead((MEM_ADDR *)&dummy, 0, 1);
}

/*--------------------------------------------------------------------------------------
------    Local Function: ISR_GetInterruptRegister
--------------------------------------------------------------------------------------*/
static void ISR_GetInterruptRegister(void)
{
    VARVOLATILE UINT8 dummy;
    HW_EscReadIsr((MEM_ADDR *)&dummy, 0, 1);
}

/*======================================================================================
      Interrupt Initialization Functions
======================================================================================*/

void ESC_INT_Init(void)
{
    /* GPIO and EXTI configured in MX_GPIO_Init() */
}

#ifdef DC_SUPPORTED

void SYNC0_INT_Init(void)
{
    /* GPIO and EXTI configured in MX_GPIO_Init() */
}

void SYNC1_INT_Init(void)
{
    /* Placeholder — not used in current hardware */
}

#endif /* DC_SUPPORTED */

/*======================================================================================
      SPI Init (called by HW_Init, wraps MX_SPI1_Init)
======================================================================================*/

static void SPI_Init(void)
{
    MX_SPI1_Init();
}

/*======================================================================================
      ECAT Timer Init (GD32 TIMER2, 1ms tick)
======================================================================================*/

void ECAT_TIM_Init(void)
{
    MX_TIM2_Init();
}

UINT16 PDI_GetTimer(void)
{
    return (UINT16)timer_counter_read(ECAT_TIMER);
}

void PDI_ClearTimer(void)
{
    timer_counter_value_config(ECAT_TIMER, 0);
}

/*======================================================================================
      ESC Interrupt Callback (called from gd32f10x_it.c EXTI handlers)
======================================================================================*/

void ESC_INT_Callback(void)
{
    PDI_Isr();
    exti_flag_clear(ESC_INT_EXTI_LINE);
}

void SYNC0_INT_Callback(void)
{
    Sync0_Isr();
    exti_flag_clear(ESC_SYNC0_EXTI_LINE);
}

/*======================================================================================
      Exported Hardware Access Functions
======================================================================================*/

UINT8 HW_Init(void)
{
    UINT32 intMask;

    SPI_Init();
    DESELECT_SPI;  /* Ensure CS initially high */

    /* Verify SPI communication by writing and reading back AL_EVENTMASK */
    {
        int retry = 0;
        do
        {
            intMask = 0x93;
            HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
            intMask = 0;
            HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
            if (++retry > 1000) return 1;   /* Timeout */
        } while (intMask != 0x93);
    }

    intMask = 0x00;
    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

    INIT_ESC_INT
    ENABLE_ESC_INT();

    INIT_SYNC0_INT
    INIT_SYNC1_INT

    ENABLE_SYNC0_INT;
    ENABLE_SYNC1_INT;

    INIT_ECAT_TIMER;
    START_ECAT_TIMER;

    ENABLE_GLOBAL_INT;

    return 0;
}

void HW_Release(void)
{
    /* Release hardware resources if needed */
}

UINT16 HW_GetALEventRegister(void)
{
    GetInterruptRegister();
    return EscALEvent.Word;
}

UINT16 HW_GetALEventRegister_Isr(void)
{
    ISR_GetInterruptRegister();
    return EscALEvent.Word;
}

/*======================================================================================
      SPI Read Functions
======================================================================================*/

void HW_EscRead(MEM_ADDR *pData, UINT16 Address, UINT16 Len)
{
    UINT16 i = Len;
    UINT8 data = 0U;
    UINT8 *pTmpData = (UINT8 *)pData;

    DISABLE_GLOBAL_INT;

    /* Keep one ESC SPI transaction per block. Re-addressing every byte makes
       1020-byte PDO transfers take tens of ms and can trip the SM watchdog. */
    AddressingEsc(Address, ESC_RD);

    while (i-- > 0)
    {
        if (i == 0U)
        {
            /* Last byte: DI pin must be 1 */
            data = 0xFFU;
        }
        *pTmpData = RxTxSpiData(data);
        pTmpData++;
    }

    DESELECT_SPI

    ENABLE_GLOBAL_INT;
}

void HW_EscReadIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len)
{
    UINT16 i = Len;
    UINT8 data = 0;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* Send address and command to ESC */
    AddressingEsc(Address, ESC_RD);

    while (i-- > 0)
    {
        if (i == 0)
        {
            /* Last byte: DI pin must be 1 */
            data = 0xFF;
        }

        *pTmpData = RxTxSpiData(data);
        pTmpData++;
    }

    DESELECT_SPI
}

/*======================================================================================
      SPI Write Functions
======================================================================================*/

void HW_EscWrite(MEM_ADDR *pData, UINT16 Address, UINT16 Len)
{
    UINT16 i = Len;
    VARVOLATILE UINT8 dummy;
    UINT8 *pTmpData = (UINT8 *)pData;

    DISABLE_GLOBAL_INT;

    /* Keep one ESC SPI transaction per block, same as ISR path. */
    AddressingEsc(Address, ESC_WR);

    while (i-- > 0)
    {
        dummy = RxTxSpiData(*pTmpData);
        pTmpData++;
    }

    DESELECT_SPI

    ENABLE_GLOBAL_INT;
}

void HW_EscWriteIsr(MEM_ADDR *pData, UINT16 Address, UINT16 Len)
{
    UINT16 i = Len;
    VARVOLATILE UINT16 dummy;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* Send address and command to ESC */
    AddressingEsc(Address, ESC_WR);

    while (i-- > 0)
    {
        dummy = RxTxSpiData(*pTmpData);
        pTmpData++;
    }

    DESELECT_SPI
}
