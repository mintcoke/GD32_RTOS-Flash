# RFID 从站看门狗超时自动复位设计

## 背景

EtherCAT RFID 从站（GD32F103C8T6 + TR8253 ESC + EC-UHF-B RFID 模块）在运行过程中，RFID 模块可能因通信协议卡死、射频干扰等原因停止响应。当前代码没有检测和恢复机制，一旦 RFID 模块死机，PLC 端读到的 RFID 数据将永久停留在最后有效值或清零，需要人工断电重启。

## 需求

- 监控 RFID 模块是否连续无响应超过 2 秒
- 超时后先尝试软复位 RFID 模块（快速恢复）
- 如果软复位无效或主循环本身卡死，硬件看门狗兜底复位整个 MCU
- 通过 PDO 通知 PLC 看门狗超时事件

## 架构

两层看门狗：

```
┌─────────────────────────────────────────┐
│  软件看门狗（RFID 模块级别）              │
│  超时: 2s                               │
│  动作: RFID_Reset() + 重新初始化 USART   │
│  冷却: 2s 内不重复触发                    │
│  通知: PDO TX_RFID_CMD_STATUS/RESULT     │
└──────────────┬──────────────────────────┘
               │ 软复位失败 / 主循环卡死
               ▼
┌─────────────────────────────────────────┐
│  硬件看门狗（MCU 级别）                  │
│  FWDGT (IRC40K), 超时: 5s               │
│  动作: MCU 硬件复位                      │
│  喂狗: 主循环每次迭代末尾                 │
└─────────────────────────────────────────┘
```

## 详细设计

### 1. 软件看门狗 — RFID 模块监控

#### 监控指标

新增全局变量 `g_RfidLastAliveMs`，记录任意天线最后一次成功与 RFID 模块通信的时间戳。在以下位置更新：

- `RFID_Inventory()` 返回 `RFID_RET_OK`
- `RFID_Command()` 返回 `RFID_RET_OK`
- `RFID_ReadTag()` / `RFID_WriteTag()` 返回 `RFID_RET_OK`

判断逻辑：

```c
if (g_SysTickCnt - g_RfidLastAliveMs > RFID_WD_TIMEOUT_MS) {
    // 触发软复位
}
```

#### 软复位流程

```
RFID 连续无响应 > 2s
  → g_RfidWdTriggered = 1
  → 通知 PLC: DI(TX_RFID_CMD_STATUS) = RFID_PLC_STATUS_ERROR
               DI(TX_RFID_CMD_RESULT) = 0xE001 (看门狗超时)
  → RFID_Reset() 软复位模块
  → 延时 100ms (RFID 模块重启时间，期间继续调用 ECAT_Stack_MainLoop)
  → 重新初始化 RFID USART + DMA (rfid_rx_dma_start())
  → 设置冷却期: g_RfidLastAliveMs = g_SysTickCnt (重置计时器)
  → 冷却期内不重复触发，避免反复复位
```

#### 防抖与冷却

- 冷却期 2s：软复位后 `g_RfidLastAliveMs` 重置为当前时间，下次检查在 2s 后
- 软复位最多触发一次，如果复位后 RFID 仍然无响应，不再重复软复位，等待硬件看门狗兜底
- 通过 `g_RfidWdTriggered` 标志防止重入

#### 调用位置

`RFID_WatchdogCheck()` 在 `DO_LED_Ctrl()` 中调用，该函数通过 `ECAT_Application()` → `g_pfnPeriodicTask()` 路径在每次 PDO 周期执行，确保只在 OP 状态下检查。

### 2. 硬件看门狗 — FWDGT 兜底

#### 配置

| 参数 | 值 | 说明 |
|------|---|------|
| 时钟源 | IRC40K (40kHz) | GD32F103 内部 RC，不受主时钟影响 |
| 预分频器 | FWDGT_PSC_DIV64 | 40kHz / 64 = 625 Hz |
| 重载值 | 3125 | 3125 / 625 = 5.0s |
| 超时 | 5.0s | 给软件看门狗 2s 软复位 + 额外余量 |

#### 喂狗策略

- 喂狗位置：`main()` 的 `while(1)` 循环末尾，`ECAT_Monitor()` 之后
- 无条件喂狗：不区分 RFID 是否正常，因为即使 RFID 死了 EtherCAT 通信仍需运行
- 只有主循环本身卡死（如 SPI 总线锁死、死循环）才应触发硬件复位

#### 初始化时机

在 `ECAT_Stack_Init()` 和 `RFID_Init()` 完成后、进入主循环之前。FWDGT 一旦启动就无法软件停止。

#### 启动流程

```c
main()
  SystemInit / GPIO / SPI / Timer / UART
  ECAT_Stack_Init()
  RFID_Init()
  WDG_Init()            // FWDGT 初始化，5s 超时
  while(1) {
    ECAT_Stack_MainLoop()
    RFID_Scan / RFID_EcatCmdTask
    ECAT_Monitor()
    WDG_Feed()           // 喂狗
  }
```

### 3. PLC 通知

超时时通过 TxPDO 通知 PLC：

| PDO 变量 | 值 | 含义 |
|----------|---|------|
| `DI(TX_RFID_CMD_STATUS)` | `RFID_PLC_STATUS_ERROR` (3) | 命令状态为错误 |
| `DI(TX_RFID_CMD_RESULT)` | `0xE001` | 看门狗超时错误码 |

错误码定义在 `ecat_api.h` 中：
- `RFID_ERR_WD_TIMEOUT = 0xE001`

## 文件改动

| 文件 | 改动类型 | 改动内容 |
|------|---------|---------|
| `rfid_ecat.h` | 修改 | 新增 `g_RfidLastAliveMs`、`g_RfidWdTriggered`、`RFID_WD_TIMEOUT_MS`、`RFID_WD_COOLDOWN_MS` 声明；新增 `RFID_WatchdogCheck()` 原型 |
| `rfid_ecat.c` | 修改 | `RFID_Command()`/`RFID_Inventory()` 等成功时更新 `g_RfidLastAliveMs`；新增 `RFID_WatchdogCheck()` 实现 |
| `Application.c` | 修改 | `DO_LED_Ctrl()` 中调用 `RFID_WatchdogCheck()` |
| `ecat_api.h` | 修改 | 新增 `RFID_ERR_WD_TIMEOUT = 0xE001` |
| `Hardware/wdg_ecat.c` | 新建 | FWDGT 初始化 (`WDG_Init`) 和喂狗 (`WDG_Feed`) |
| `Hardware/wdg_ecat.h` | 新建 | FWDGT 接口声明 |
| `User/main.c` | 修改 | 主循环中添加 `WDG_Init()` 和 `WDG_Feed()` |

## 测试验证

1. **正常场景**：RFID 模块正常工作，看门狗不触发，`g_RfidLastAliveMs` 持续更新
2. **RFID 死机**：断开 RFID 模块串口连接，2s 后触发软复位，PDO 报 `0xE001` 错误码
3. **软复位恢复**：重新连接 RFID 模块，100ms 后恢复正常通信
4. **主循环卡死**：在调试中人为插入死循环，5s 后 FWDGT 触发 MCU 复位
5. **冷却期**：连续触发后不会反复复位 RFID 模块
