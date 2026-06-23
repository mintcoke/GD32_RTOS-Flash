# EtherCAT RFID 从站固件 — 业务工程师交付说明

本项目已将稳定底层代码固化为静态库 `ECAT_Core.lib`，业务工程师只需修改应用层源码。
本文件说明：哪些文件给你、哪些不能动、怎么打开工程、改了什么要做什么。

---

## 1. 文件分级

### 1.1 库文件（不能改、不用重编）

| 文件 | 说明 |
|------|------|
| `MDK-ARM/Lib/ECAT_Core.lib` | 底层库，25 个文件编译而成：<br>• SSC 协议栈 7 个：mailbox / sdoserv / ecatcoe / ecatslv / ecatappl / objdef / coeappl<br>• GD32 外设驱动 14 个：gd32f10x_adc/crc/dma/exti/fwdgt/i2c/gpio/rtc/spi/timer/usart/misc/rcu/fmc<br>• Hardware 底层 4 个：spi_ecat / timer_ecat / gpio_ecat / wdg_ecat |

**注意**：库内 `.c` 源码不提供。除非底层接口（`GD32Evb.h` / `ecat_def.h`）发生变化，否则无需重新发版 lib。

### 1.2 业务源码（可改）

| 文件 | 作用 | 典型改动场景 |
|------|------|-------------|
| `ECAT/ecat_api.c` + `.h` | PDO 布局 + 命令码 + 协议栈接口 | 改 PDO 字段布局、加新命令码 |
| `Hardware/Application.c` + `.h` | PLC 命令分发 + PDO 映射 | 加新 PLC 命令、改命令处理逻辑 |
| `Hardware/rfid_ecat.c` + `.h` | RFID 驱动 + 扫描 + 看门狗 | 改扫描策略、看门狗阈值 |
| `Hardware/uart.c` + `.h` | 调试串口（USART2） | 改调试口配置 |
| `Hardware/App_Flash.c` + `.h` | Flash 持久化参数 | 改参数结构 |
| `User/main.c` | 主循环 | 改任务调度顺序 |
| `SSC/GD32Evb.c` + `.h` | 平台移植（SPI/中断回调） | 改 ESC 硬件相关、中断引脚 |
| `SSC/SSC-Device.c` + `.h` | 设备回调（PDO 地址/大小） | 改对象字典条目 |
| `User/gd32f10x_it.c` + `.h` | 中断向量处理 | 加/改中断 |
| `User/systick.c` + `.h` | 1ms 心跳 | 一般不动 |

### 1.3 头文件（全部提供）

业务源码 `#include` 需要的所有 `.h`，包括库内的接口声明：

- `SSC/`：`esc.h`、`ecat_def.h`、`applInterface.h`、`SSC-DeviceObjects.h`、`ecatslv.h`、`ecatappl.h`、`ecatcoe.h`、`mailbox.h`、`sdoserv.h`、`objdef.h`、`coeappl.h`
- `ECAT/`：`ecat_debug.h`、`ecat_hw_if.h`、`ecat_pdo_types.h`
- `User/`：`gd32f10x_libopt.h`、`FreeRTOSConfig.h`

### 1.4 固件/启动

| 文件 | 说明 |
|------|------|
| `User/startup_gd32f10x_md.s` | 启动文件 + 中断向量表（向量表只能一份，留应用层） |
| `Firmware/CMSIS/GD/GD32F10x/Source/system_gd32f10x.c` | 系统时钟配置 |
| `Firmware/CMSIS/` 全部 `.h` | CMSIS 核心头 + GD32 设备头 |
| `Firmware/GD32F10x_standard_peripheral/Include/` 全部 `.h` | 外设驱动头文件（`.c` 已在 lib 里，但 `.h` 业务层要 include） |

**不给**：`Firmware/.../Source/` 下的 14 个 `gd32f10x_*.c`（已在 lib 里）

### 1.5 工程文件

| 文件 | 说明 |
|------|------|
| `MDK-ARM/EtherCAT_RFID.uvprojx` | 应用工程，已配好引用 `ECAT_Core.lib`（FileType=4） |
| `MDK-ARM/EtherCAT_RFID.uvoptx` | 调试器/Target 状态，打开直接能调 |

**关键**：`Lib/ECAT_Core.lib` 的相对路径 `.\Lib\` 已写死在 `.uvprojx` 里，解压后不要移动 lib 位置。

### 1.6 ESI 设备描述文件（PLC 导入用）

| 文件 | 说明 |
|------|------|
| `DOC/TRI_SPI_DC.xml` | 从站 EtherCAT ESI 描述。导入 KV-Studio / TwinCAT 识别从站 PDO 布局用。**没它主站认不出从站** |

### 1.7 文档

| 文件 | 说明 |
|------|------|
| `DOC/EtherCAT_GD32_Project_Doc.docx` | 项目说明（9 章节） |
| `DOC/操作说明_PLC命令_v4.bak.docx` | PLC 命令操作说明 |
| `DOC/EC-UHF-B 通讯说明的副本.pdf` | RFID 模块协议手册 |
| `DOC/GD32F103CBT6管脚使用说明.xlsx` | GD32 管脚分配 |
| `DOC/E-CAT GD32F103C8T6_PE管脚使用说明.xlsx` | EtherCAT 管脚 |
| `DOC/GD32F103xx Datasheet_Rev3.3.pdf` | GD32 数据手册 |
| `DOC/GD32F10x_用户手册_Rev2.9.pdf` | GD32 用户手册 |

### 1.8 参考固件

| 文件 | 说明 |
|------|------|
| `MDK-ARM/Objects/EtherCAT_RFID.hex` | 参考固件，先烧这个验证硬件 OK，再改自己的代码 |

---

## 2. 快速上手

### 2.1 编译环境要求

- **Keil MDK-ARM 5.x** + **ARMCC 编译器**（传统编译器，**不要切 AC6/armclang**，ABI 不兼容会链接失败）
- MDK 许可证需支持 GD32F103（Cortex-M3），Lite 版 32KB 限制内可用（本工程 Code ≈ 24KB）

### 2.2 打开工程

1. 解压交付包，保持目录结构不变（`MDK-ARM/Lib/ECAT_Core.lib` 路径不能动）
2. 双击 `MDK-ARM/EtherCAT_RFID.uvprojx`
3. 确认 Target 选 `EtherCAT_RFID`

### 2.3 编译

- `Project → Build Target`（F7）
- 预期 0 错 0 警，生成 `MDK-ARM/Objects/EtherCAT_RFID.hex`
- **改业务源码后直接 Build，lib 不用动**

### 2.4 烧录

- 烧录 `EtherCAT_RFID.hex`
- 勾选 **Reset and Run**（否则烧完不自动运行，串口无输出）
- 调试串口：USART2 (PB10/PB11)，115200bps 8N1

### 2.5 验证

- 调试串口看到 `[ECAT] AL=0x08` 表示进 OP
- 3 路天线放 UHF 标签能读到 EPC
- PLC 发读 TID / 写 EPC 命令正常

---

## 3. 改动指引

### 3.1 改业务逻辑（命令处理、扫描策略等）

直接改 `Application.c` / `rfid_ecat.c` / `main.c` → Build → 烧录。lib 不用动。

### 3.2 加新 PLC 命令

1. `ecat_api.h`：在 `RFID_PLC_CMD_*` 宏里加新命令码（避免占用已用值）
2. `Application.c`：在 `RFID_EcatCmdTask()` 的 `switch(cmd)` 加新 `case`
3. Build → 烧录

### 3.3 改 PDO 布局（重要）

改 `ecat_api.h` 的 PDO 字段定义后，**必须同步改 ESI XML** 并重新导入 PLC：

1. 改 `ecat_api.h` 里 `TX_RFID*` / `RX_RFID*` 宏
2. 同步改 `DOC/TRI_SPI_DC.xml` 的 PDO 条目（0x6000/0x7000 对象字典）
3. 在 KV-Studio 重新导入 XML，重新映射 PDO
4. Build → 烧录

**不同步改 XML 会导致主站识别的 PDO 布局与从站实际不一致，通信错乱。**

### 3.4 改持久化参数

1. `ecat_api.h`：在 `PersistentParams_t` 结构体加字段
2. 业务代码直接读写 `g_PersistentParams.新字段`
3. 改后调用 `ECAT_SetParamDirty()`
4. 结构体大小变化后，旧 Flash 数据 checksum 校验失败会自动清零用默认值（安全，不会崩）

### 3.5 改了底层接口（GD32Evb.h / ecat_def.h）

这种情况**需要重新编译 lib**，联系底层维护者发新版本 `ECAT_Core.lib`，替换 `MDK-ARM/Lib/ECAT_Core.lib` 即可。

---

## 4. 不要做的事

- ❌ 不要改 `Lib/` 下的任何文件（lib 是二进制，改不动）
- ❌ 不要把库内 `.c` 源码加回工程（会造成符号重复定义）
- ❌ 不要改 `startup_gd32f10x_md.s` 的中断向量表（除非明确要加中断）
- ❌ 不要改 `Firmware/CMSIS/` 和外设驱动头文件（库依赖它们）
- ❌ 不要切 AC6 编译器
- ❌ 不要移动 `Lib/ECAT_Core.lib` 的位置

---

## 5. 命令交互协议（速查）

### 5.1 发命令（PLC → 从站，写 RxPDO）

```
DO(0)=CMD   DO(1)=ANT   DO(2)=ADDR   DO(3)=WORDS   DO(4..67)=DATA
```

字段用法（统一规则）：
- **ADDR**（DO(2)）：起始字地址，**仅读写类命令用**（READ/WRITE/RAW），其他命令不用保持 0
- **WORDS**（DO(3)）：读写字数（读写类）或参数值（参数类命令）
- **DATA**（DO(4..67)）：写入数据，64 字 = 128 字节

### 5.2 收结果（从站 → PLC，读 TxPDO）

```
DI(103)=ECHO  DI(104)=STATUS  DI(105)=RESULT  DI(106)=LEN  DI(107..170)=DATA
```

STATUS：0=IDLE / 1=BUSY / **2=OK** / 3=ERROR（注意 2 才是成功，别和习惯混淆）

### 5.3 命令时序（边沿触发）

1. PLC 写 `DO(0)=CMD` 触发命令
2. 从站 STATUS 变 BUSY → 完成后变 OK 或 ERROR
3. PLC 读结果
4. **PLC 写 `DO(0)=0` 清零**（下降沿，从站重新武装，否则下一条命令发不进）

### 5.4 常用命令码

| CMD | 名称 | 说明 |
|-----|------|------|
| 1 | GET_INFO | 读模块信息 |
| 2 | READ_TID | 读 TID 区（默认 6 字，最多 64 字） |
| 3 | READ_USER | 读 USER 区（最多 64 字） |
| 4 | WRITE_USER | 写 USER 区（单次 ≤64 字，单段 ≤15 字可分段） |
| 5 | WRITE_EPC | 写 EPC 区（addr≥2，前两字 PC/CRC 禁写；≤31 字，单次写不分段）⚠️超过标签容量会报废 |
| 6 | SET_EPC_LEN | 改 EPC 长度（1~31 字，改 PC 字高 5 位） |
| 7/8 | SET/GET_POWER | 设/读功率 |
| 17 | INVENTORY | 单次盘点 |
| 32 | READ_RFU | 读 RFU 区密码（最多 4 字，写禁止） |
| 100 | RAW | 原始命令直发模块 |

完整命令码见 `ecat_api.h` `RFID_PLC_CMD_*` 宏定义。

### 5.5 各存储区读写上限规则

固件透传不预判：写入字数由 PLC 的 WORDS 决定，固件不挡，模块/标签拒写就如实返回错误码。
单次命令传输量受 PDO 数据区限制（读响应 64 字、写请求 64 字）。

| 存储区 | Bank | 读上限 | 写上限 | 说明 |
|--------|------|--------|--------|------|
| RFU（保留/密码） | 0 | 4 字（8 字节） | **禁止** | 含 Kill/Access 密码，误写可能报废标签 |
| EPC | 1 | 31 字（62 字节） | ≤31 字，addr≥2 | 前两字 PC/CRC 禁写 |
| TID | 2 | 64 字（128 字节） | 禁止（只读） | 出厂固化，标签本身拒写 |
| USER | 3 | 64 字（128 字节） | ≤64 字 | 标签容量看芯片型号 |

**⚠️ 写 EPC 报废标签警告**：
- 不同标签 EPC 区实际容量不同（常见 6/8/16 字节），固件无法预知
- **写 EPC 前必须先盘点确认标签最大 EPC 长度**：盘点返回的 PC 字（DI(108) 高字节+DI(109) 高字节组合）高 5 位 = 标签声明的 EPC 字数。写入字数不得超过此值
- `SET_EPC_LEN` 改大长度 + `WRITE_EPC` 写超过标签容量的数据，会把数据写到 EPC 区之外，**可能永久报废标签**
- 建议保守使用：写入字数 ≤ 标签盘点返回的 PC 字长度，且不要随意 SET_EPC_LEN 改大

**说明**：
- 单次命令读写受 PDO 数据区 64 字限制（响应数据 DI(107..170)、写入数据 DO(4..67)）
- EPC 受标准上限 31 字限制（PC 字 5 位编码）
- `SET_EPC_LEN` 的 `words` 是字数（1~31），不是字节数
- 写已不再分段，words 多少直接单次写多少

---

## 6. 目录结构

```
ECAT_交付包/
├── MDK-ARM/
│   ├── EtherCAT_RFID.uvprojx       ← 应用工程
│   ├── EtherCAT_RFID.uvoptx
│   ├── Lib/ECAT_Core.lib          ← 库本体（路径勿动）
│   └── Objects/EtherCAT_RFID.hex  ← 参考固件
├── ECAT/                            ← 业务源码 + 头
├── Hardware/                        ← 业务源码 + 头
├── SSC/                             ← GD32Evb.c + SSC-Device.c + 所有 .h
├── User/                            ← main.c + it.c + systick + startup.s
├── Firmware/                        ← CMSIS + 外设 .h
├── DOC/                             ← 文档 + ESI XML
└── README_交付.md                   ← 本文件
```
