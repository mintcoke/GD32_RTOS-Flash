/*
 * wdg_ecat.h — 硬件看门狗 (FWDGT) 接口
 *
 * GD32F103 的独立看门狗 (Free Watchdog Timer)。
 * 一旦启动就无法停止，属于硬件级安全机制。
 *
 * 与软件看门狗 (rfid_ecat.c 中 RFID_WatchdogCheck) 的区别：
 *   - FWDGT：监控整个程序是否存活，5s 无喂狗 → 芯片硬复位
 *   - 软件看门狗：仅监控 RFID 模块是否响应，2s 无响应 → 软重置 RFID
 *
 * 超时计算：
 *   时钟源 = IRC40K (40kHz 内部 RC，精度 ±3%)
 *   预分频 = DIV64 → 40000/64 = 625 Hz
 *   重载值 = 3125 → 3125/625 = 5.0 秒
 */
#ifndef __WDG_ECAT_H__
#define __WDG_ECAT_H__

#include "gd32f10x.h"

/* 初始化 FWDGT：5 秒超时，启动后不可停止 */
void WDG_Init(void);

/* 喂狗：重载计数器，必须在 5 秒内至少调用一次
 * 在主循环和 RFID 阻塞等待中都会调用 */
void WDG_Feed(void);

#endif /* __WDG_ECAT_H__ */
