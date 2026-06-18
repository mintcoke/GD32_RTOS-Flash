# SSC 协议栈静态库（ECAT_Core.lib）设计规格

**日期**：2026-06-18
**目标**：把 SSC 协议栈中"后续一定不会改"的纯协议栈文件链接为 `ECAT_Core.lib`，应用工程引用它，保证 100% 运行正常（EtherCAT 正常 INIT→PREOP→SAFEOP→OP，RFID 业务全功能）。

## 背景

工程是 EtherCAT RFID 从站（GD32F103 + TR8253 ESC + RFID 模块）。SSC 协议栈 9 个 .c 文件散落在应用工程里，业务工程师容易误改。目标：把纯协议栈部分固化为 .lib，只留业务相关源码可改。

**之前尝试过且失败**：把 5 个 SSC 文件（ecatappl/ecatslv/ecatcoe/mailbox/sdoserv）+ 外设驱动一次性进库，gd32f10x_it.c/GD32Evb.c 留应用层。编译能过、Code=24706 与全源码一致，但烧录后 EtherCAT 报 **AL Status Code 0x0014**（NOVALIDFIRMWARE），从站卡在 INIT+Error，主站连接失败。

0x0014 抛出点在 [ecatslv.c CheckSmSettings](SSC/ecatslv.c#L404)：`nMaxEscAddress < MAX_PD_WRITE_ADDRESS(0x2FFF)`。`nMaxEscAddress` 在 [MainInit](SSC/ecatslv.c#L2436)（ecatappl.c）里通过 `HW_EscReadWord`（GD32Evb.h 宏 → GD32Evb.c SPI 函数）读 ESC 寄存器 0x0006 赋值。全源码能跑、进库报错，且 Code 完全一致 → 不是符号未链入（否则 Code 会少），根因待精确定位（最可能：静态库段合并导致全局变量/PDO 缓冲 RAM 地址重排，或 const 对象字典 Flash 地址重排）。

## 已确认的约束

1. **GD32Evb.c 留应用层**：含 ESC_INT_Callback/SYNC0_INT_Callback 中断回调，调用库内 PDI_Isr/Sync0_Isr。之前实验过 GD32Evb.c 进库 + 中断向量表留应用层 → 弱符号 ISR 被链接器丢弃 → 中断全丢。所以 GD32Evb.c 不入库。
2. **gd32f10x_it.c 留应用层**：含 EXTI0/EXTI15/USART0/SysTick 中断向量处理，理由同上。
3. **objdef.c / coeappl.c / SSC-Device.c 留应用层**：对象字典定义、CoE 应用回调、设备层状态——业务相关，随时会改。
4. **许可证限制**：MDK 不支持 `--library` 链接器参数（L3921U）、`IncludeLibs` XML 标签无效。唯一可行方式：把 .lib 当普通文件加入工程树（FileType=4），Keil 直接传给链接器。
5. **库工程与应用工程编译设置必须完全一致**：Optim=4、MicroLIB=1、OneElfS=1、useXO=0、Define=USE_STDPERIPH_DRIVER,GD32F10X_MD、IncludePath 相同。否则 ABI/段属性不匹配。

## 文件分级

### 入库区（ECAT_Core.lib，5 个纯协议栈文件，按风险从低到高）

| 文件 | 职责 | 全局变量风险 |
|------|------|------------|
| mailbox.c | 邮箱通信核心 | 低（少状态变量，内部用） |
| sdoserv.c | SDO 服务器底层 | 中（~15 个 VARMEM 段处理变量） |
| ecatcoe.c | CoE 协议核心 | 低（几乎全局部变量） |
| ecatslv.c | 核心状态机 | 高（nMaxEscAddress、u16ALEventMask、SyncManInfo、EepromLoaded 等） |
| ecatappl.c | 应用层轮询 MainLoop/MainInit | 高（aPdOutputData/aPdInputData 大数组、bInitFinished） |

### 业务区（留应用工程源码）

- `SSC/GD32Evb.c` — 平台移植（SPI/Timer/中断回调）
- `SSC/SSC-Device.c` — 设备回调（APPL_GenerateMapping 等设 PDO 地址/大小）
- `SSC/objdef.c` — 对象字典定义
- `SSC/coeappl.c` — CoE 应用回调
- `User/main.c`、`User/gd32f10x_it.c`、`User/systick.c`
- `Hardware/*`（rfid_ecat、Application、ecat_api、外设驱动等）
- `Firmware/CMSIS/startup_gd32f10x_md.s`、`system_gd32f10x.c`

## 方案：增量入库 + 每步验证

核心思想：不一次全进（之前失败），而是从最安全的文件开始，每次加一个、烧录验证 OP。哪步出问题就精确定位到那个文件，再针对性修复。每一步都必须烧录上板验证能进 OP + RFID 读卡正常，才进下一步。

### 阶段 0：基线准备

- 当前全源码版（commit 0cf5a3b，已验证能 OP）作为基线
- 备份 `MDK-ARM/EtherCAT_RFID.uvprojx`、`.uvoptx`
- 生成基线 map（开 ldLst）：`Objects/` 下留全源码 map 作对比基准
- 在工程目录建 `Lib/` 子目录存放库产物

### 阶段 1：单文件探针验证（mailbox.c）

**目的**：验证"静态库链接机制本身"是否 OK。mailbox.c 全局变量少、不涉及 PDO 缓冲/状态机核心，是最安全的探针。

- 库工程 `ECAT_Core.uvprojx`：CreateLib=1，OutputDirectory=`.\Lib\`，只含 mailbox.c
- 应用工程 `EtherCAT_RFID.uvprojx`：含其余 8 个 SSC 文件 + 全部业务文件，ECAT Group 加 `Lib\ECAT_Core.lib`（FileType=4）
- 编译库 → 编译应用 → 烧录 → 验证 OP
- **通过判据**：EtherCAT 正常进 OP，3 路天线 RFID 读卡正常
- 失败 → 问题在静态库机制本身（链接器设置），查 map 对比；成功 → 阶段 2

### 阶段 2：逐个加文件（按风险从低到高）

依次把 sdoserv → ecatcoe → ecatslv → ecatappl 加入库工程，同时从应用工程移除。每加一个：
- 重编库 → 重编应用 → 烧录 → 验证 OP + RFID
- 哪个文件加入后报 0x0014 或其他错误，该文件即元凶
- 记录每步的 map 文件备查

预期在 ecatslv.c 或 ecatappl.c 加入时可能复现 0x0014（这俩含 nMaxEscAddress / PDO 缓冲等高风险变量）。

### 阶段 3：锁定 0x0014 根因并修复

元凶文件确定后：
- 对比"该文件进库"与"全源码"两个 map，查 `nMaxEscAddress`、`aPdOutputData`/`aPdInputData`、相关 const 表的地址差异
- 若 RAM 地址重排 → 用 scatter file 或链接器选项固定段顺序
- 若 const 数据 Flash 地址重排 → 检查对象字典指针引用
- 若符号被误剔除 → 在应用层加显式强引用（extern + 取地址）
- 修复后该文件进库，验证 OP 通过

### 阶段 4：全部 5 文件入库

5 个纯协议栈文件都在库中，应用工程只剩业务源码 + lib。最终验证：
- 编译 0 错 0 警
- 烧录后 EtherCAT INIT→PREOP→SAFEOP→OP 全程正常
- 3 路天线 RFID 读卡正常
- PLC 命令（读 TID、写 EPC、设功率等）正常
- 拔插网线能重连

## 工程结构（最终态）

```
MDK-ARM/
├── EtherCAT_RFID.uvprojx    ← 应用工程（业务源码 + 引用 lib）
├── EtherCAT_RFID.uvoptx
├── ECAT_Core.uvprojx        ← 库工程（5 个纯协议栈文件）
├── Lib/
│   └── ECAT_Core.lib        ← 库产物
├── Objects/                 ← 应用编译产物
└── Listings/
```

应用工程 Group（最终态）：
- User: main.c, gd32f10x_it.c, systick.c
- Hardware: rfid_ecat.c, Application.c, uart.c, spi_ecat.c, timer_ecat.c, gpio_ecat.c, App_Flash.c, wdg_ecat.c
- ECAT: ecat_api.c, **ECAT_Core.lib**
- SSC: GD32Evb.c, SSC-Device.c, objdef.c, coeappl.c
- Firmware/GD32_StdPeriph: 14 个外设驱动
- Firmware/CMSIS: startup_gd32f10x_md.s, system_gd32f10x.c

库工程 Group：
- SSC: mailbox.c, sdoserv.c, ecatcoe.c, ecatslv.c, ecatappl.c
（编译设置与应用工程完全一致，CreateLib=1，输出到 Lib/）

**范围说明**：库工程只含这 5 个 SSC 文件。外设驱动（gd32f10x_*.c）、GD32Evb.c、业务文件全部留应用工程——本次只固化"纯协议栈"，不扩大范围（外设驱动进库是独立决策，不在本规格内）。这 5 个 SSC 文件不直接调用外设驱动（通过 GD32Evb.h 的宏间接调用，宏展开后在应用层解析），所以库工程无需外设驱动即可编译。

## 保符号机制（贯穿全程）

1. 库工程与应用工程编译/链接设置完全一致（Optim=4、MicroLIB=1、OneElfS=1、useXO=0、同 Define/IncludePath）
2. 库工程不单独做死代码剔除（依赖应用工程链接时统一处理）
3. 应用工程把 lib 当文件加入工程树（FileType=4），绕过 MDK 许可证对 `--library` 的限制
4. 链接器 Misc controls 保持空（不残留 `--library` 等违规参数）

## 风险与回退

- **每阶段都烧录验证**：任何一步失败，回退到上一步已知可用的配置（git checkout 或备份的 .uvprojx）
- **0x0014 复现**：阶段 3 专门处理，有 map 对比 + scatter file/强引用等修复手段
- **库工程设置污染**：之前多次失败尝试残留的 IncludeLibs/Misc/LinkerInputFile 必须清空，每次从 git 干净版重建
- **Keil 缓存**：改 .uvprojx 前必须关闭 Keil，改完再开（之前踩过：Keil 用内存版本覆盖磁盘文件）

## 验证清单

每个阶段烧录后必须全部通过：
- [ ] 串口有 `[ECAT] AL=0x08` 日志（进 OP）
- [ ] 不报 0x0014 或其他 AL 错误码
- [ ] 3 路天线能读到 EPC（放标签测试）
- [ ] PLC 发读 TID / 写 EPC 命令能正常执行
- [ ] 拔插网线能重连进 OP
