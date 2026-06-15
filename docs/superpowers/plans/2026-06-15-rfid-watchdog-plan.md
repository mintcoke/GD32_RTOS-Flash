# RFID 看门狗超时自动复位 实现计划

> **面向 AI 代理的工作者：** 必需子技能：使用 superpowers:subagent-driven-development（推荐）或 superpowers:executing-plans 逐任务实现此计划。步骤使用复选框（`- [ ]`）语法来跟踪进度。

**目标：** 为 RFID 从站添加两层看门狗——软件看门狗监控 RFID 模块死机并软复位，硬件看门狗 (FWDGT) 兜底复位 MCU

**架构：** 软件看门狗在 `rfid_ecat.c` 中追踪 `g_RfidLastAliveMs`，超时 2s 后软复位 RFID 模块；硬件看门狗使用 GD32 FWDGT 外设，5s 超时，主循环喂狗，卡死时复位 MCU

**技术栈：** GD32F103 标准外设库 (FWDGT)、EtherCAT SSC 框架、EC-UHF-B RFID 协议

---

## 文件结构

| 文件 | 职责 |
|------|------|
| `Hardware/wdg_ecat.h` | FWDGT 接口声明：`WDG_Init()` / `WDG_Feed()` |
| `Hardware/wdg_ecat.c` | FWDGT 初始化（5s 超时）和喂狗实现 |
| `Hardware/rfid_ecat.h` | 新增看门狗常量、全局变量、`RFID_WatchdogCheck()` 原型 |
| `Hardware/rfid_ecat.c` | 更新 `g_RfidLastAliveMs`；新增 `RFID_WatchdogCheck()` 实现 |
| `Hardware/Application.c` | `DO_LED_Ctrl()` 中调用 `RFID_WatchdogCheck()` |
| `ECAT/ecat_api.h` | 新增 `RFID_ERR_WD_TIMEOUT` 错误码 |
| `User/main.c` | 添加 `WDG_Init()` 和 `WDG_Feed()` 调用 |

---

### 任务 1：新增 FWDGT 硬件看门狗模块

**文件：**
- 创建：`Hardware/wdg_ecat.h`
- 创建：`Hardware/wdg_ecat.c`

- [ ] **步骤 1：创建 wdg_ecat.h**

```c
/*
 * wdg_ecat.h — GD32F103 FWDGT (Independent Watchdog) interface
 */
#ifndef __WDG_ECAT_H__
#define __WDG_ECAT_H__

#include "gd32f10x.h"

void WDG_Init(void);   /* Initialize FWDGT, 5s timeout */
void WDG_Feed(void);   /* Reload FWDGT counter */

#endif /* __WDG_ECAT_H__ */
```

- [ ] **步骤 2：创建 wdg_ecat.c**

```c
/*
 * wdg_ecat.c — GD32F103 FWDGT (Independent Watchdog) implementation
 *
 * Clock: IRC40K (40kHz internal RC)
 * Prescaler: DIV64 → 40kHz/64 = 625 Hz
 * Reload: 3125 → timeout = 3125/625 = 5.0s
 *
 * FWDGT cannot be stopped once started — hardware guarantee.
 */
#include "wdg_ecat.h"
#include "gd32f10x_fwdgt.h"

#define WDG_PRESCALER   FWDGT_PSC_DIV64
#define WDG_RELOAD      3125U   /* 5.0s at 625 Hz */

void WDG_Init(void)
{
    /* Enable write access to FWDGT registers */
    fwdgt_write_enable();

    /* Configure prescaler and reload value */
    fwdgt_prescaler_value_config(WDG_PRESCALER);
    fwdgt_reload_value_config(WDG_RELOAD);

    /* Reload the counter now */
    fwdgt_counter_reload();

    /* Start FWDGT — cannot be stopped after this */
    fwdgt_enable();
}

void WDG_Feed(void)
{
    fwdgt_counter_reload();
}
```

- [ ] **步骤 3：将 wdg_ecat.c 添加到 Keil 工程**

在 Keil 项目文件 `MDK-ARM/EtherCAT_RFID.uvprojx` 中，将 `Hardware/wdg_ecat.c` 添加到 Hardware 分组。在 Keil IDE 中右键 Hardware 组 → Add Existing Files → 选择 `wdg_ecat.c`。

- [ ] **步骤 4：Commit**

```bash
git add Hardware/wdg_ecat.h Hardware/wdg_ecat.c
git commit -m "feat: add FWDGT hardware watchdog module (5s timeout)"
```

---

### 任务 2：在 rfid_ecat 中添加软件看门狗监控

**文件：**
- 修改：`Hardware/rfid_ecat.h`
- 修改：`Hardware/rfid_ecat.c`

- [ ] **步骤 1：在 rfid_ecat.h 中添加看门狗声明**

在 `rfid_ecat.h` 的 `/* Debug helpers */` 注释块之前，添加：

```c
/* ------ Software Watchdog ------ */
#define RFID_WD_TIMEOUT_MS     2000U   /* Trigger soft reset after 2s no response */
#define RFID_ERR_WD_TIMEOUT    0xE001U /* Watchdog timeout error code for PLC */

extern volatile uint32_t g_RfidLastAliveMs;  /* Last successful RFID communication timestamp */
extern volatile uint8_t  g_RfidWdTriggered;  /* Watchdog soft-reset triggered flag */
void RFID_WatchdogCheck(void);              /* Check RFID module liveness, soft-reset if needed */
```

- [ ] **步骤 2：在 rfid_ecat.c 中定义看门狗全局变量**

在 `rfid_ecat.c` 中，紧跟在 `volatile uint8_t rfid_last_error = 0;` (第20行) 之后，添加：

```c
volatile uint32_t g_RfidLastAliveMs = 0;
volatile uint8_t  g_RfidWdTriggered = 0;
```

- [ ] **步骤 3：在 RFID_Inventory() 成功时更新 g_RfidLastAliveMs**

在 `rfid_ecat.c` 的 `RFID_Inventory()` 函数中，找到 `rfid_last_result = RFID_RET_OK;` (约第400行)，在其后添加：

```c
    g_RfidLastAliveMs = g_SysTickCnt;
```

同样在 `rfid_rx_dma_start()` 被调用的超时返回前（第369行 `return RFID_RET_TIMEOUT;`），在 `rfid_last_result = RFID_RET_TIMEOUT;` 之后不更新（因为超时不算成功）。

- [ ] **步骤 4：在 RFID_ReadTag() 成功时更新 g_RfidLastAliveMs**

在 `RFID_ReadTag()` 函数中，找到最后的 `rfid_last_result = RFID_RET_OK;` (约第509行)，在其后添加：

```c
    g_RfidLastAliveMs = g_SysTickCnt;
```

- [ ] **步骤 5：在 RFID_WriteTag() 成功时更新 g_RfidLastAliveMs**

在 `RFID_WriteTag()` 函数中，找到最后的 `rfid_last_result = RFID_RET_OK;` (约第570行)，在其后添加：

```c
    g_RfidLastAliveMs = g_SysTickCnt;
```

- [ ] **步骤 6：在 RFID_Command() 成功时更新 g_RfidLastAliveMs**

在 `RFID_Command()` 函数中，找到最后的 `rfid_last_result = RFID_RET_OK;` (约第626行)，在其后添加：

```c
    g_RfidLastAliveMs = g_SysTickCnt;
```

- [ ] **步骤 7：在 RFID_Init() 末尾初始化看门狗时间戳**

在 `RFID_Init()` 函数末尾（第324行 `rfid_rx_dma_start();` 之后），添加：

```c
    extern volatile uint32_t g_SysTickCnt;
    g_RfidLastAliveMs = g_SysTickCnt;
```

- [ ] **步骤 8：在 rfid_ecat.c 中实现 RFID_WatchdogCheck()**

在 `RFID_Scan()` 函数之前（约第630行之前），添加：

```c
void RFID_WatchdogCheck(void)
{
    extern volatile uint32_t g_SysTickCnt;
    extern void ECAT_Stack_MainLoop(void);

    if (g_RfidWdTriggered != 0U) {
        return;  /* Already triggered, waiting for hardware WD to recover or MCU reset */
    }

    if ((uint32_t)(g_SysTickCnt - g_RfidLastAliveMs) <= RFID_WD_TIMEOUT_MS) {
        return;  /* RFID module is alive */
    }

    /* RFID module not responding for > 2s — trigger soft reset */
    g_RfidWdTriggered = 1U;

    /* Notify PLC via PDO */
    DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_ERROR;
    DI(TX_RFID_CMD_RESULT) = RFID_ERR_WD_TIMEOUT;

    /* Soft reset RFID module */
    RFID_Reset();

    /* Wait 100ms for module reboot, keep EtherCAT alive */
    {
        uint32_t deadline = g_SysTickCnt + 100U;
        while ((int32_t)(g_SysTickCnt - deadline) < 0) {
            ECAT_Stack_MainLoop();
        }
    }

    /* Reinitialize USART + DMA */
    rfid_rx_dma_start();

    /* Reset watchdog timer (starts 2s cooldown) */
    g_RfidLastAliveMs = g_SysTickCnt;
}
```

注意：`DI()` 宏和 `TX_RFID_CMD_STATUS` / `TX_RFID_CMD_RESULT` / `RFID_PLC_STATUS_ERROR` 定义在 `ecat_api.h` 中，`rfid_ecat.c` 已通过 `rfid_ecat.h` 间接包含。需在 `rfid_ecat.c` 顶部添加 `#include "ecat_api.h"`。

- [ ] **步骤 9：在 rfid_ecat.c 顶部添加 ecat_api.h 包含**

在 `rfid_ecat.c` 的 `#include "rfid_ecat.h"` 之后，添加：

```c
#include "ecat_api.h"
```

- [ ] **步骤 10：Commit**

```bash
git add Hardware/rfid_ecat.h Hardware/rfid_ecat.c
git commit -m "feat: add RFID software watchdog with 2s timeout and soft-reset"
```

---

### 任务 3：在 DO_LED_Ctrl() 中调用看门狗检查

**文件：**
- 修改：`Hardware/Application.c`

- [ ] **步骤 1：在 DO_LED_Ctrl() 末尾调用 RFID_WatchdogCheck()**

在 `Application.c` 的 `DO_LED_Ctrl()` 函数中，在最后的 `}` 之前（第577行之前），添加：

```c
    RFID_WatchdogCheck();
```

- [ ] **步骤 2：Commit**

```bash
git add Hardware/Application.c
git commit -m "feat: call RFID_WatchdogCheck from DO_LED_Ctrl periodic task"
```

---

### 任务 4：在 ecat_api.h 中添加看门狗错误码

**文件：**
- 修改：`ECAT/ecat_api.h`

- [ ] **步骤 1：添加 RFID_ERR_WD_TIMEOUT 定义**

在 `ecat_api.h` 中，找到 `#define RFID_PLC_STATUS_ERROR   3` (约第101行)，在其后添加：

```c
#define RFID_ERR_WD_TIMEOUT    0xE001U
```

注意：`RFID_ERR_WD_TIMEOUT` 也在 `rfid_ecat.h` 中定义了。为避免重复定义冲突，删除 `rfid_ecat.h` 中的定义（任务2步骤1中添加的），只保留 `ecat_api.h` 中的定义。`rfid_ecat.h` 已包含 `ecat_api.h` 间接路径，或者 `rfid_ecat.c` 直接 `#include "ecat_api.h"` 可访问该宏。

- [ ] **步骤 2：删除 rfid_ecat.h 中的重复定义**

从 `rfid_ecat.h` 中删除任务2步骤1中添加的 `#define RFID_ERR_WD_TIMEOUT 0xE001U` 行。保留 `RFID_WD_TIMEOUT_MS`、全局变量声明和 `RFID_WatchdogCheck()` 原型。

- [ ] **步骤 3：Commit**

```bash
git add ECAT/ecat_api.h Hardware/rfid_ecat.h
git commit -m "feat: add RFID_ERR_WD_TIMEOUT error code to ecat_api.h"
```

---

### 任务 5：在 main.c 中集成硬件看门狗

**文件：**
- 修改：`User/main.c`

- [ ] **步骤 1：添加 wdg_ecat.h 包含**

在 `main.c` 的 `#include "rfid_ecat.h"` 之后，添加：

```c
#include "wdg_ecat.h"
```

- [ ] **步骤 2：在 RFID_Init() 之后添加 WDG_Init()**

在 `main()` 函数中，`RFID_Init();` 之后，添加：

```c
    WDG_Init();
```

- [ ] **步骤 3：在主循环末尾添加 WDG_Feed()**

在 `main()` 的 `while(1)` 循环中，`ECAT_Monitor();` 之后，`}` 之前，添加：

```c
        WDG_Feed();
```

- [ ] **步骤 4：Commit**

```bash
git add User/main.c
git commit -m "feat: integrate FWDGT hardware watchdog in main loop"
```

---

### 任务 6：编译验证与功能测试

**文件：** 无新文件

- [ ] **步骤 1：Keil 编译**

在 Keil 中 Build (F7)，确认 0 Error, 0 Warning。

- [ ] **步骤 2：烧录并观察正常启动**

烧录后串口应输出 `[ECAT] AL=0x08`，正常进入 OP。确认 RFID 数据正常，`g_RfidLastAliveMs` 持续更新。

- [ ] **步骤 3：测试 RFID 死机软复位**

断开 RFID 模块串口连接（拔掉 PA9/PA10 排线），等待 2s+。预期：
- PLC 端 `TX_RFID_CMD_STATUS` 变为 `3` (ERROR)
- PLC 端 `TX_RFID_CMD_RESULT` 变为 `0xE001`
- 重新连接排线后，RFID 恢复正常通信

- [ ] **步骤 4：测试硬件看门狗兜底**

如果软复位后 RFID 仍不恢复（保持排线断开），主循环继续运行 → FWDGT 持续喂狗 → MCU 不会复位。这符合设计：硬件看门狗只在主循环本身卡死时才触发。

- [ ] **步骤 5：Final commit**

```bash
git add -A
git commit -m "feat: RFID watchdog timeout auto-reset — two-layer (soft + FWDGT)"
```
