/**
 * @file    ecat_debug.h
 * @brief   EtherCAT 调试与日志接口 (可选)
 *
 * 提供可选的调试输出功能，便于开发阶段排查问题。
 * 正式发布时可通过宏关闭所有调试输出。
 *
 * 使用方法:
 *   1. 在 Keil MDK Define 中添加 ECAT_DEBUG_ENABLE=1
 *   2. 实现 ECAT_DebugPutChar(ch) 宏用于字符输出
 *      (例如: 重定向到串口、SWO、或 RTT)
 *   3. 在代码中调用 ECAT_DEBUG(...) 输出调试信息
 *
 * 默认实现:
 *   如果未定义 ECAT_DebugPutChar，则所有调试宏展开为空。
 */

#ifndef _ECAT_DEBUG_H_
#define _ECAT_DEBUG_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 调试开关 */
#ifndef ECAT_DEBUG_ENABLE
#define ECAT_DEBUG_ENABLE   0
#endif

#ifndef ECAT_DebugPutChar
#define ECAT_DebugPutChar(ch)   ((void)0)
#endif

#if ECAT_DEBUG_ENABLE
static inline void ECAT_Debug_Puts(const char *str)
{
    while (*str) ECAT_DebugPutChar(*str++);
}

static inline void ECAT_Debug_PutHex8(uint8_t val)
{
    const char hex[] = "0123456789ABCDEF";
    ECAT_DebugPutChar(hex[(val >> 4) & 0x0F]);
    ECAT_DebugPutChar(hex[val & 0x0F]);
}

static inline void ECAT_Debug_PutHex16(uint16_t val)
{
    ECAT_Debug_PutHex8((uint8_t)(val >> 8));
    ECAT_Debug_PutHex8((uint8_t)(val & 0xFF));
}

static inline void ECAT_Debug_PutHex32(uint32_t val)
{
    ECAT_Debug_PutHex16((uint16_t)(val >> 16));
    ECAT_Debug_PutHex16((uint16_t)(val & 0xFFFF));
}

static inline void ECAT_Debug_PutDec(uint32_t val)
{
    char buf[12];
    int i = 0;
    if (val == 0) { ECAT_DebugPutChar('0'); return; }
    while (val > 0) { buf[i++] = (char)('0' + (val % 10)); val /= 10; }
    while (i > 0) ECAT_DebugPutChar(buf[--i]);
}
#else
#define ECAT_Debug_Puts(s)           ((void)0)
#define ECAT_Debug_PutHex8(v)        ((void)0)
#define ECAT_Debug_PutHex16(v)       ((void)0)
#define ECAT_Debug_PutHex32(v)       ((void)0)
#define ECAT_Debug_PutDec(v)         ((void)0)
#endif

#if ECAT_DEBUG_ENABLE

#define ECAT_DEBUG(fmt, ...) do { \
    ECAT_Debug_Puts("[ECAT] "); \
    ECAT_Debug_Puts(fmt); \
    ECAT_DebugPutChar('\n'); \
} while(0)

#define ECAT_DEBUG_WARN(msg) do { \
    ECAT_Debug_Puts("[ECAT WARN] "); \
    ECAT_Debug_Puts(msg); \
    ECAT_DebugPutChar('\n'); \
} while(0)

#define ECAT_DEBUG_ERR(msg) do { \
    ECAT_Debug_Puts("[ECAT ERR]  "); \
    ECAT_Debug_Puts(msg); \
    ECAT_DebugPutChar('\n'); \
} while(0)

#define ECAT_DEBUG_HEX(label, data, len) do { \
    ECAT_Debug_Puts("[ECAT] "); \
    ECAT_Debug_Puts(label); \
    ECAT_Debug_Puts(": "); \
    for (uint32_t _i = 0; _i < (len); _i++) { \
        ECAT_Debug_PutHex8(((const uint8_t*)(data))[_i]); \
        ECAT_DebugPutChar(' '); \
    } \
    ECAT_DebugPutChar('\n'); \
} while(0)

#define ECAT_DEBUG_ALSTATE(old_state, new_state) do { \
    static const char * const _al_names[] = { \
        "", "INIT", "PREOP", "BOOT", "SAFEOP", "", "", "", "OP" }; \
    ECAT_Debug_Puts("[ECAT] AL state: "); \
    if ((old_state) <= 8 && _al_names[old_state][0]) \
        { ECAT_Debug_Puts(_al_names[old_state]); } \
    else \
        { ECAT_Debug_PutDec(old_state); } \
    ECAT_Debug_Puts(" -> "); \
    if ((new_state) <= 8 && _al_names[new_state][0]) \
        { ECAT_Debug_Puts(_al_names[new_state]); } \
    else \
        { ECAT_Debug_PutDec(new_state); } \
    ECAT_DebugPutChar('\n'); \
} while(0)

#else
#define ECAT_DEBUG(fmt, ...)        ((void)0)
#define ECAT_DEBUG_WARN(msg)        ((void)0)
#define ECAT_DEBUG_ERR(msg)         ((void)0)
#define ECAT_DEBUG_HEX(l, d, n)     ((void)0)
#define ECAT_DEBUG_ALSTATE(o, n)    ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _ECAT_DEBUG_H_ */
