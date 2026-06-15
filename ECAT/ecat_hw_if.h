/*
 * ecat_hw_if.h — EtherCAT Hardware Abstraction Interface
 *
 * 校验平台必须提供的宏，并为未定义的 HW_Esc* 宏提供默认实现。
 * 由 TR8253Evb.h 包含，不在 SSC 自有文件之列，不会因 SSC Tool 再生被覆盖。
 *
 * 包含链: ecatslv.h → TR8253Evb.h → ecat_hw_if.h
 */

#ifndef _ECAT_HW_IF_H_
#define _ECAT_HW_IF_H_

#include "ecat_def.h"
#include "esc.h"

/* ============================================================
 * 平台必须提供的宏 (若无则编译报错)
 * ============================================================ */

#ifndef DISABLE_ESC_INT
#error "DISABLE_ESC_INT() must be defined by the platform port"
#endif

#ifndef ENABLE_ESC_INT
#error "ENABLE_ESC_INT() must be defined by the platform port"
#endif

#ifndef HW_GetTimer
#error "HW_GetTimer() must be defined by the platform port"
#endif

#ifndef HW_ClearTimer
#error "HW_ClearTimer() must be defined by the platform port"
#endif

/* ============================================================
 * 带默认值的宏 (平台可选择覆盖)
 * ============================================================ */

#ifndef HW_EscReadWord
#define HW_EscReadWord(WordValue, Address) \
    HW_EscRead(((MEM_ADDR *)&(WordValue)), ((UINT16)(Address)), 2)
#endif

#ifndef HW_EscReadDWord
#define HW_EscReadDWord(DWordValue, Address) \
    HW_EscRead(((MEM_ADDR *)&(DWordValue)), ((UINT16)(Address)), 4)
#endif

#ifndef HW_EscReadMbxMem
#define HW_EscReadMbxMem(pData, Address, Len) \
    HW_EscRead(((MEM_ADDR *)(pData)), ((UINT16)(Address)), (Len))
#endif

#ifndef HW_EscReadWordIsr
#define HW_EscReadWordIsr(WordValue, Address) \
    HW_EscReadIsr(((MEM_ADDR *)&(WordValue)), ((UINT16)(Address)), 2)
#endif

#ifndef HW_EscReadDWordIsr
#define HW_EscReadDWordIsr(DWordValue, Address) \
    HW_EscReadIsr(((MEM_ADDR *)&(DWordValue)), ((UINT16)(Address)), 4)
#endif

#ifndef HW_EscWriteWord
#define HW_EscWriteWord(WordValue, Address) \
    HW_EscWrite(((MEM_ADDR *)&(WordValue)), ((UINT16)(Address)), 2)
#endif

#ifndef HW_EscWriteDWord
#define HW_EscWriteDWord(DWordValue, Address) \
    HW_EscWrite(((MEM_ADDR *)&(DWordValue)), ((UINT16)(Address)), 4)
#endif

#ifndef HW_EscWriteMbxMem
#define HW_EscWriteMbxMem(pData, Address, Len) \
    HW_EscWrite(((MEM_ADDR *)(pData)), ((UINT16)(Address)), (Len))
#endif

#ifndef HW_EscWriteWordIsr
#define HW_EscWriteWordIsr(WordValue, Address) \
    HW_EscWriteIsr(((MEM_ADDR *)&(WordValue)), ((UINT16)(Address)), 2)
#endif

#ifndef HW_EscWriteDWordIsr
#define HW_EscWriteDWordIsr(DWordValue, Address) \
    HW_EscWriteIsr(((MEM_ADDR *)&(DWordValue)), ((UINT16)(Address)), 4)
#endif

#endif /* _ECAT_HW_IF_H_ */
