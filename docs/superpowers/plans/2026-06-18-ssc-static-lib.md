# SSC 协议栈静态库（ECAT_Core.lib）实现计划

> **面向 AI 代理的工作者：** 必需子技能：使用 superpowers:subagent-driven-development（推荐）或 superpowers:executing-plans 逐任务实现此计划。步骤使用复选框（`- [ ]`）语法来跟踪进度。

**目标：** 把 SSC 协议栈 5 个纯协议栈文件（mailbox/sdoserv/ecatcoe/ecatslv/ecatappl）增量链接为 `ECAT_Core.lib`，应用工程引用它，保证烧录后 EtherCAT 正常进 OP、RFID 业务全功能。

**架构：** 两个独立 Keil 工程——`ECAT_Core.uvprojx`（库工程，CreateLib=1，输出到 `Lib/`）和 `EtherCAT_RFID.uvprojx`（应用工程，把 `ECAT_Core.lib` 当 FileType=4 文件加入工程树）。从最安全的 mailbox.c 开始，每次加一个文件、烧录验证 OP，定位 0x0014 根因后修复，直到 5 文件全入库。

**技术栈：** Keil MDK-ARM 5.x（ARMCC）、GD32F103CB、Beckhoff SSC v5.12、J-Link 烧录。命令行编译用 `D:\Keil_V5\UV4\UV4.exe`。

---

## 重要说明：硬件在环验证

本计划每个任务都需**烧录到 GD32 板子上板验证**（非纯软件测试）。每任务的"验证"步骤包含：
1. 命令行编译（自动）
2. 烧录 hex（需人工用 J-Link 烧到板子）
3. 上板验证 EtherCAT 连接 + RFID 读卡（需人工观察串口日志 + PLC 连接状态）

执行者若无法烧录上板，应在编译验证通过后暂停，把 hex 路径告知用户由其烧录验证，再继续。

## 工具脚本

整个计划用一个 Python 脚本 `MDK-ARM/_gen_proj.py` 生成/修改两个 .uvprojx。它从 git 干净版（`git show HEAD:MDK-ARM/EtherCAT_RFID.uvprojx`）提取 TargetOption 段和文件路径映射，按参数生成库工程和应用工程。所有任务复用此脚本，只改参数。

**关键设计**：脚本用纯字符串拼接（不用 ElementTree，避免子树共享 bug），从 git 干净版读取（避免工作区污染），写入前用 ElementTree 校验 XML 合法性。

### 任务 0：创建工具脚本

**文件：**
- 创建：`MDK-ARM/_gen_proj.py`

- [ ] **步骤 1：编写工具脚本**

创建 `MDK-ARM/_gen_proj.py`，内容如下。此脚本接受 `--lib-files` 和 `--app-ssc-files` 参数，生成 `ECAT_Core.uvprojx` 和 `EtherCAT_RFID.uvprojx`：

```python
# -*- coding: utf-8 -*-
"""生成 ECAT_Core.uvprojx（库工程）和 EtherCAT_RFID.uvprojx（应用工程）。
从 git 干净版提取 TargetOption 和文件路径，纯字符串拼接，避免子树共享。
用法：python _gen_proj.py --lib-files mailbox.c sdoserv.c --with-lib
  --lib-files: 进库的 SSC 文件名（不含路径）
  --with-lib:  应用工程是否引用 ECAT_Core.lib（阶段1起加此参数）
"""
import re, subprocess, sys, os

def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument('--lib-files', nargs='*', default=[], help='进库的 SSC 文件名')
    ap.add_argument('--with-lib', action='store_true', help='应用工程引用 ECAT_Core.lib')
    args = ap.parse_args()

    # 从 git 干净版读取
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    clean = subprocess.check_output(
        ['git', 'show', 'HEAD:MDK-ARM/EtherCAT_RFID.uvprojx'],
        cwd=repo_root, text=True, encoding='utf-8')

    topt = re.search(r'<TargetOption>.*?</TargetOption>', clean, re.DOTALL).group(0)
    old_od = re.search(r'<OutputDirectory>[^<]*</OutputDirectory>', topt).group(0)
    before = clean[:clean.index('  <Targets>')]
    after = clean[clean.index('  </Targets>') + len('  </Targets>'):]

    # 文件路径映射（从干净版提取所有文件）
    all_files = {}
    for m in re.finditer(
        r'<FileName>([^<]+)</FileName>\s*<FileType>(\d+)</FileType>\s*<FilePath>([^<]+)</FilePath>',
        clean):
        all_files[m.group(1)] = (m.group(2), m.group(3))

    # 全部 9 个 SSC 文件
    ALL_SSC = ['GD32Evb.c', 'ecatappl.c', 'ecatcoe.c', 'ecatslv.c', 'mailbox.c',
               'objdef.c', 'sdoserv.c', 'SSC-Device.c', 'coeappl.c']
    lib_set = set(args.lib_files)
    # 应用层 SSC = 全部 - 进库的
    app_ssc = [f for f in ALL_SSC if f not in lib_set]

    # 应用工程固定包含的 Group（不变部分）
    APP_GROUPS = {
        'User': ['main.c', 'gd32f10x_it.c', 'systick.c'],
        'Hardware': ['uart.c', 'spi_ecat.c', 'timer_ecat.c', 'gpio_ecat.c',
                     'rfid_ecat.c', 'Application.c', 'App_Flash.c', 'wdg_ecat.c'],
        'ECAT': ['ecat_api.c'],
        'SSC': app_ssc,
        'Firmware/CMSIS': ['startup_gd32f10x_md.s', 'system_gd32f10x.c'],
    }
    # Firmware/GD32_StdPeriph 14 个外设驱动（从干净版提取顺序）
    FW_FILES = [m.group(1) for m in re.finditer(
        r'<FileName>(gd32f10x_\w+\.c)</FileName>', clean)]
    APP_GROUPS['Firmware/GD32_StdPeriph'] = FW_FILES

    # 库工程 Group：只含进库的 SSC 文件
    LIB_GROUPS = {'SSC': args.lib_files}

    def groups_xml(fm, add_lib=False):
        out = ['      <Groups>']
        for gn, fns in fm.items():
            if not fns:
                continue
            out.append('        <Group>')
            out.append(f'          <GroupName>{gn}</GroupName>')
            out.append('          <Files>')
            for fn in fns:
                ft, fp = all_files[fn]
                out.append('            <File>')
                out.append(f'              <FileName>{fn}</FileName>')
                out.append(f'              <FileType>{ft}</FileType>')
                out.append(f'              <FilePath>{fp}</FilePath>')
                out.append('            </File>')
            if add_lib and gn == 'ECAT':
                out.append('            <File>')
                out.append('              <FileName>ECAT_Core.lib</FileName>')
                out.append('              <FileType>4</FileType>')
                out.append('              <FilePath>.\\Lib\\ECAT_Core.lib</FilePath>')
                out.append('            </File>')
            out.append('          </Files>')
            out.append('        </Group>')
        out.append('      </Groups>')
        return '\n'.join(out)

    od_app = '.\\Objects\\'
    od_lib = '.\\Lib\\'

    # 应用工程
    t = topt.replace(old_od, f'<OutputDirectory>{od_app}</OutputDirectory>')
    app = (before + '  <Targets>\n    <Target>\n'
           + '      <TargetName>Target 1</TargetName>\n'
           + '      <ToolsetNumber>0x4</ToolsetNumber>\n'
           + '      <ToolsetName>ARM-ADS</ToolsetName>\n'
           + t + '\n' + groups_xml(APP_GROUPS, add_lib=args.with_lib) + '\n'
           + '    </Target>\n  </Targets>' + after)
    with open('EtherCAT_RFID.uvprojx', 'w', encoding='utf-8', newline='') as f:
        f.write(app)

    # 库工程
    tl = topt.replace(old_od, f'<OutputDirectory>{od_lib}</OutputDirectory>')
    tl = tl.replace('<OutputName>EtherCAT_RFID</OutputName>', '<OutputName>ECAT_Core</OutputName>')
    tl = tl.replace('<CreateExecutable>1</CreateExecutable>', '<CreateExecutable>0</CreateExecutable>')
    tl = tl.replace('<CreateLib>0</CreateLib>', '<CreateLib>1</CreateLib>')
    tl = tl.replace('<CreateHexFile>1</CreateHexFile>', '<CreateHexFile>0</CreateHexFile>')
    lib_proj = (before + '  <Targets>\n    <Target>\n'
                + '      <TargetName>ECAT_Core</TargetName>\n'
                + '      <ToolsetNumber>0x4</ToolsetNumber>\n'
                + '      <ToolsetName>ARM-ADS</ToolsetName>\n'
                + tl + '\n' + groups_xml(LIB_GROUPS) + '\n'
                + '    </Target>\n  </Targets>' + after)
    with open('ECAT_Core.uvprojx', 'w', encoding='utf-8', newline='') as f:
        f.write(lib_proj)

    # XML 合法性校验
    import xml.etree.ElementTree as ET
    for fn in ['EtherCAT_RFID.uvprojx', 'ECAT_Core.uvprojx']:
        t = ET.parse(fn)
        for tg in t.getroot().find('Targets').findall('Target'):
            name = tg.find('TargetName').text
            nfs = sum(len(g.findall('Files/File')) for g in tg.findall('Groups/Group'))
            print(f'[{fn}] {name}: {nfs} files')

if __name__ == '__main__':
    main()
```

- [ ] **步骤 2：验证脚本能从干净版生成全源码应用工程（不含 lib）**

运行（不带 `--with-lib`、`--lib-files` 为空，应用工程应含全部 9 个 SSC 文件）：
```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
python _gen_proj.py
```
预期输出：
```
[EtherCAT_RFID.uvprojx] Target 1: 37 files
[ECAT_Core.uvprojx] ECAT_Core: 0 files
```
（应用工程 37 个文件 = 3 User + 8 Hardware + 1 ECAT + 9 SSC + 14 Firmware + 2 CMSIS；库工程 0 文件，因为没传 --lib-files）

- [ ] **步骤 3：编译验证生成的应用工程与基线一致**

```bash
rm -f Objects/*.o Objects/*.axf Objects/*.hex
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o build_log.txt
grep -E "Error|Warning|Program Size" build_log.txt
```
预期：`Code=24706 RO-data=21242 RW-data=7536 ZI-data=4856`，0 Error 0 Warning（与基线一致，证明脚本生成的应用工程等价于全源码）

- [ ] **步骤 4：恢复 git 干净版（脚本生成的应用工程验证完即丢弃，保持工作区干净）**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32"
git checkout HEAD -- MDK-ARM/EtherCAT_RFID.uvprojx
rm -f MDK-ARM/ECAT_Core.uvprojx MDK-ARM/build_log.txt
```

- [ ] **步骤 5：Commit 工具脚本**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32"
git add MDK-ARM/_gen_proj.py
git commit -m "chore: 添加 .uvprojx 生成脚本（SSC 静态库工具）"
```

---

### 任务 1：基线准备 + mailbox.c 探针验证（阶段 0 + 阶段 1）

**目的**：验证静态库链接机制本身是否 OK。mailbox.c 全局变量少、不涉及 PDO/状态机核心，是最安全的探针。

**文件：**
- 修改：`MDK-ARM/EtherCAT_RFID.uvprojx`（应用工程，SSC Group 移除 mailbox.c，ECAT Group 加 ECAT_Core.lib）
- 修改：`MDK-ARM/ECAT_Core.uvprojx`（库工程，只含 mailbox.c）
- 创建：`MDK-ARM/Lib/`（库产物目录）

- [ ] **步骤 1：用脚本生成阶段 1 配置（mailbox.c 进库 + 应用引用 lib）**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
mkdir -p Lib
python _gen_proj.py --lib-files mailbox.c --with-lib
```
预期输出：
```
[EtherCAT_RFID.uvprojx] Target 1: 37 files   # 36 源文件 + 1 lib 文件
[ECAT_Core.uvprojx] ECAT_Core: 1 files
```
（应用工程 SSC Group 从 9 个减到 8 个，但 ECAT Group 多了 ECAT_Core.lib，总数仍 37）

- [ ] **步骤 2：编译库工程 → 生成 Lib/ECAT_Core.lib**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Lib/*.lib Lib/*.o
"D:/Keil_V5/UV4/UV4.exe" -j0 -b ECAT_Core.uvprojx -o build_lib.txt
ls -la Lib/ECAT_Core.lib
tail -2 build_lib.txt
```
预期：`Lib/ECAT_Core.lib` 存在，日志末尾 `0 Error(s), 0 Warning(s)`，含 `creating Library...`

- [ ] **步骤 3：编译应用工程 → 生成 hex**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Objects/*.o Objects/*.axf Objects/*.hex
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o build_app.txt
grep -E "Error|Warning|Program Size" build_app.txt
```
预期：0 Error 0 Warning，`Code=` 值记录下来（mailbox.c 进库后应接近 24706）

- [ ] **步骤 4：烧录上板验证（需人工）**

烧录 `MDK-ARM/Objects/EtherCAT_RFID.hex` 到 GD32 板子（J-Link）。上电后观察：
- 串口（USART2，PB10/PB11，115200）应输出 `[ECAT] AL=0x08`（进 OP）
- 不应出现 `[ECAT] AL=0x11 SC=0x0014`（INIT+错误）
- PLC（KV-X310）应能连接成功
- 3 路天线放标签能读到 EPC

**若失败（报 0x0014 或连不上）**：问题在静态库机制本身。执行：
1. 生成 map 对比：
```bash
# 阶段1版 map
python -c "x=open('EtherCAT_RFID.uvprojx',encoding='utf-8').read();x=x.replace('<ldLst>0</ldLst>','<ldLst>1</ldLst>');open('EtherCAT_RFID.uvprojx','w',encoding='utf-8',newline='').write(x)"
rm -f Objects/*.o Objects/*.axf Objects/*.hex Listings/*.map
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o /dev/null
cp Listings/EtherCAT_RFID.map _stage1.map
git checkout HEAD -- MDK-ARM/EtherCAT_RFID.uvprojx
# 基线 map
python -c "x=open('EtherCAT_RFID.uvprojx',encoding='utf-8').read();x=x.replace('<ldLst>0</ldLst>','<ldLst>1</ldLst>');open('EtherCAT_RFID.uvprojx','w',encoding='utf-8',newline='').write(x)"
rm -f Objects/*.o Objects/*.axf Objects/*.hex Listings/*.map
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o /dev/null
cp Listings/EtherCAT_RFID.map _baseline.map
git checkout HEAD -- MDK-ARM/EtherCAT_RFID.uvprojx
```
2. 对比 mailbox.c 相关符号地址：`diff _baseline.map _stage1.map | grep -i mailbox`
3. 暂停，把对比结果告知用户，由用户决定修复方向（scatter file / 强引用）。**不继续后续任务。**

**若成功（进 OP，RFID 正常）**：静态库机制 OK，进任务 2。

- [ ] **步骤 5：Commit 阶段 1 配置**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32"
# .gitignore 已忽略 Lib/ 和 build_*.txt
git add MDK-ARM/EtherCAT_RFID.uvprojx MDK-ARM/ECAT_Core.uvprojx
git commit -m "feat: mailbox.c 进 ECAT_Core.lib 验证静态库机制（阶段1）"
```

---

### 任务 2：加 sdoserv.c（阶段 2 第 1 步）

**文件：**
- 修改：`MDK-ARM/ECAT_Core.uvprojx`（加 sdoserv.c）
- 修改：`MDK-ARM/EtherCAT_RFID.uvprojx`（SSC Group 移除 sdoserv.c）

- [ ] **步骤 1：用脚本生成阶段 2.1 配置（mailbox + sdoserv 进库）**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
python _gen_proj.py --lib-files mailbox.c sdoserv.c --with-lib
```
预期：
```
[EtherCAT_RFID.uvprojx] Target 1: 36 files   # 又少1个源文件
[ECAT_Core.uvprojx] ECAT_Core: 2 files
```

- [ ] **步骤 2：重编库**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Lib/*.lib Lib/*.o
"D:/Keil_V5/UV4/UV4.exe" -j0 -b ECAT_Core.uvprojx -o build_lib.txt
tail -2 build_lib.txt
```
预期：`0 Error(s), 0 Warning(s)`，`creating Library...`

- [ ] **步骤 3：重编应用**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Objects/*.o Objects/*.axf Objects/*.hex
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o build_app.txt
grep -E "Error|Warning|Program Size" build_app.txt
```
预期：0 Error 0 Warning

- [ ] **步骤 4：烧录上板验证（需人工）**

同任务 1 步骤 4 的验证清单。sdoserv.c 含 ~15 个 VARMEM 段处理变量，若报错优先查这些变量的 RAM 地址。

**若失败**：记录失败现象（AL 状态码），保留 `build_app.txt` 和 map，暂停告知用户。
**若成功**：进任务 3。

- [ ] **步骤 5：Commit**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32"
git add MDK-ARM/EtherCAT_RFID.uvprojx MDK-ARM/ECAT_Core.uvprojx
git commit -m "feat: sdoserv.c 进 ECAT_Core.lib（阶段2.1）"
```

---

### 任务 3：加 ecatcoe.c（阶段 2 第 2 步）

**文件：**
- 修改：`MDK-ARM/ECAT_Core.uvprojx`（加 ecatcoe.c）
- 修改：`MDK-ARM/EtherCAT_RFID.uvprojx`（SSC Group 移除 ecatcoe.c）

- [ ] **步骤 1：用脚本生成阶段 2.2 配置**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
python _gen_proj.py --lib-files mailbox.c sdoserv.c ecatcoe.c --with-lib
```
预期：
```
[EtherCAT_RFID.uvprojx] Target 1: 35 files
[ECAT_Core.uvprojx] ECAT_Core: 3 files
```

- [ ] **步骤 2：重编库**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Lib/*.lib Lib/*.o
"D:/Keil_V5/UV4/UV4.exe" -j0 -b ECAT_Core.uvprojx -o build_lib.txt
tail -2 build_lib.txt
```
预期：0 Error 0 Warning

- [ ] **步骤 3：重编应用**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Objects/*.o Objects/*.axf Objects/*.hex
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o build_app.txt
grep -E "Error|Warning|Program Size" build_app.txt
```
预期：0 Error 0 Warning

- [ ] **步骤 4：烧录上板验证（需人工）**

同验证清单。ecatcoe.c 几乎全局部变量，风险低，预期通过。

**若失败**：暂停告知用户。
**若成功**：进任务 4。

- [ ] **步骤 5：Commit**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32"
git add MDK-ARM/EtherCAT_RFID.uvprojx MDK-ARM/ECAT_Core.uvprojx
git commit -m "feat: ecatcoe.c 进 ECAT_Core.lib（阶段2.2）"
```

---

### 任务 4：加 ecatslv.c（阶段 2 第 3 步，0x0014 高风险文件）

**目的**：ecatslv.c 含 `nMaxEscAddress`（0x0014 直接相关）+ 大量状态机全局变量。这是最可能复现 0x0014 的文件。

**文件：**
- 修改：`MDK-ARM/ECAT_Core.uvprojx`（加 ecatslv.c）
- 修改：`MDK-ARM/EtherCAT_RFID.uvprojx`（SSC Group 移除 ecatslv.c）

- [ ] **步骤 1：用脚本生成阶段 2.3 配置**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
python _gen_proj.py --lib-files mailbox.c sdoserv.c ecatcoe.c ecatslv.c --with-lib
```
预期：
```
[EtherCAT_RFID.uvprojx] Target 1: 34 files
[ECAT_Core.uvprojx] ECAT_Core: 4 files
```

- [ ] **步骤 2：重编库**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Lib/*.lib Lib/*.o
"D:/Keil_V5/UV4/UV4.exe" -j0 -b ECAT_Core.uvprojx -o build_lib.txt
tail -2 build_lib.txt
```
预期：0 Error 0 Warning

- [ ] **步骤 3：重编应用**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Objects/*.o Objects/*.axf Objects/*.hex
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o build_app.txt
grep -E "Error|Warning|Program Size" build_app.txt
```
预期：0 Error 0 Warning。记录 Code 值。

- [ ] **步骤 4：烧录上板验证（需人工）**

同验证清单。**这是 0x0014 最可能复现的点**。

**若报 0x0014（AL=0x11 SC=0x0014）**：进入阶段 3 根因定位，执行任务 4A。**不 commit 此任务。**

**若成功**：进任务 5。

- [ ] **步骤 5：Commit（仅成功时执行）**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32"
git add MDK-ARM/EtherCAT_RFID.uvprojx MDK-ARM/ECAT_Core.uvprojx
git commit -m "feat: ecatslv.c 进 ECAT_Core.lib（阶段2.3）"
```

---

### 任务 4A：ecatslv.c 进库报 0x0014 的根因定位与修复（阶段 3）

**触发条件**：任务 4 步骤 4 报 0x0014。此任务仅在那时执行。

**目的**：对比 map 定位 `nMaxEscAddress` 等变量地址差异，用 scatter file 或强引用修复。

**文件：**
- 可能修改：`MDK-ARM/EtherCAT_RFID.uvprojx`（加 scatter file 或 Misc 参数）
- 可能创建：`MDK-ARM/ECAT_RFID.sct`（scatter file，若用此方案）
- 可能修改：`User/main.c` 或 `SSC/SSC-Device.c`（加强引用，若用此方案）

- [ ] **步骤 1：生成问题版 map 和基线 map**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
# 问题版 map（当前配置：ecatslv 进库）
python -c "x=open('EtherCAT_RFID.uvprojx',encoding='utf-8').read();x=x.replace('<ldLst>0</ldLst>','<ldLst>1</ldLst>');open('EtherCAT_RFID.uvprojx','w',encoding='utf-8',newline='').write(x)"
rm -f Objects/*.o Objects/*.axf Objects/*.hex Listings/*.map
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o /dev/null
cp Listings/EtherCAT_RFID.map _ecatslv_in_lib.map
# 恢复并生成基线 map
git checkout HEAD -- MDK-ARM/EtherCAT_RFID.uvprojx
python -c "x=open('EtherCAT_RFID.uvprojx',encoding='utf-8').read();x=x.replace('<ldLst>0</ldLst>','<ldLst>1</ldLst>');open('EtherCAT_RFID.uvprojx','w',encoding='utf-8',newline='').write(x)"
rm -f Objects/*.o Objects/*.axf Objects/*.hex Listings/*.map
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o /dev/null
cp Listings/EtherCAT_RFID.map _baseline.map
git checkout HEAD -- MDK-ARM/EtherCAT_RFID.uvprojx
```

- [ ] **步骤 2：对比关键变量地址**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
echo "=== nMaxEscAddress ==="
grep -n "nMaxEscAddress" _baseline.map _ecatslv_in_lib.map
echo "=== aPdOutputData / aPdInputData ==="
grep -n "aPdOutputData\|aPdInputData" _baseline.map _ecatslv_in_lib.map
echo "=== bInitFinished / SyncManInfo ==="
grep -n "bInitFinished\|SyncManInfo" _baseline.map _ecatslv_in_lib.map
echo "=== ecatslv.o 段布局差异 ==="
diff _baseline.map _ecatslv_in_lib.map | grep -i "ecatslv" | head
```
分析：若变量地址不同 → RAM 重排；若符号在问题版缺失 → 符号剔除。

- [ ] **步骤 3：根据分析结果选择修复方案**

根据步骤 2 结果，三选一：

**方案 A：RAM 地址重排** → 用 scatter file 固定段顺序。创建 `MDK-ARM/ECAT_RFID.sct`，把 .bss 段按基线顺序排列，在应用工程 Linker 选项加 scatter file（修改 .uvprojx 的 `<ScatterFile>`）。此方案复杂，需参考基线 map 的段顺序。

**方案 B：const 数据 Flash 重排** → 检查对象字典指针。若 `OBJ_GetObjectHandle` 等返回的指针指向库内 const 表，但表地址变了，需在应用层显式引用这些表（extern + 取地址强制链入）。

**方案 C：符号被误剔除** → 在 `SSC/SSC-Device.c` 顶部加强引用：
```c
/* 强制链接器保留 ecatslv.c 的关键符号，防止静态库剔除 */
extern UINT16 nMaxEscAddress;
static void * const _keep_ecatslv[] = { (void*)&nMaxEscAddress, (void*)MainInit, (void*)CheckIfEcatError };
```

- [ ] **步骤 4：重新生成阶段 2.3 配置并应用修复**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
python _gen_proj.py --lib-files mailbox.c sdoserv.c ecatcoe.c ecatslv.c --with-lib
# 应用步骤3的修复（手动编辑相应文件）
```

- [ ] **步骤 5：重编 + 烧录验证**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Lib/*.lib Lib/*.o && "D:/Keil_V5/UV4/UV4.exe" -j0 -b ECAT_Core.uvprojx -o /dev/null
rm -f Objects/*.o Objects/*.axf Objects/*.hex && "D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o build_app.txt
grep -E "Error|Warning|Program Size" build_app.txt
```
烧录验证 OP。**若仍失败**：暂停告知用户，把 map 对比和已试方案告知，由用户决定下一步。

- [ ] **步骤 6：清理临时 map + Commit 修复**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f _baseline.map _ecatslv_in_lib.map
cd ..
git add -A
git commit -m "fix: 修复 ecatslv.c 进库 0x0014（根因：<填步骤2分析结论，如 nMaxEscAddress RAM 地址重排/符号剔除>，修复：<填步骤3实际选用方案 A/B/C>）"
```

---

### 任务 5：加 ecatappl.c（阶段 2 第 4 步，含 PDO 缓冲）

**目的**：ecatappl.c 含 `aPdOutputData`/`aPdInputData` 大数组 + MainLoop/MainInit。最后一个高风险文件。

**文件：**
- 修改：`MDK-ARM/ECAT_Core.uvprojx`（加 ecatappl.c）
- 修改：`MDK-ARM/EtherCAT_RFID.uvprojx`（SSC Group 移除 ecatappl.c）

- [ ] **步骤 1：用脚本生成阶段 2.4 配置（5 文件全进库）**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
python _gen_proj.py --lib-files mailbox.c sdoserv.c ecatcoe.c ecatslv.c ecatappl.c --with-lib
```
预期：
```
[EtherCAT_RFID.uvprojx] Target 1: 33 files   # SSC Group 只剩 4 个业务文件
[ECAT_Core.uvprojx] ECAT_Core: 5 files
```
应用工程 SSC Group 应只剩：GD32Evb.c, objdef.c, SSC-Device.c, coeappl.c

- [ ] **步骤 2：重编库**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Lib/*.lib Lib/*.o
"D:/Keil_V5/UV4/UV4.exe" -j0 -b ECAT_Core.uvprojx -o build_lib.txt
tail -2 build_lib.txt
```
预期：0 Error 0 Warning

- [ ] **步骤 3：重编应用**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Objects/*.o Objects/*.axf Objects/*.hex
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o build_app.txt
grep -E "Error|Warning|Program Size" build_app.txt
```
预期：0 Error 0 Warning

- [ ] **步骤 4：烧录上板验证（需人工）**

完整验证清单：
- [ ] 串口有 `[ECAT] AL=0x08`（进 OP）
- [ ] 不报 0x0014 或其他 AL 错误码
- [ ] 3 路天线放标签能读到 EPC
- [ ] PLC 发读 TID / 写 EPC 命令正常执行
- [ ] 拔插网线能重连进 OP

**若失败**：执行任务 4A 同样的根因定位流程（生成 map 对比 ecatappl.c 相关符号 `aPdOutputData`/`aPdInputData`/`bInitFinished`），暂停告知用户。
**若全部通过**：进任务 6。

- [ ] **步骤 5：Commit**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32"
git add MDK-ARM/EtherCAT_RFID.uvprojx MDK-ARM/ECAT_Core.uvprojx
git commit -m "feat: ecatappl.c 进 ECAT_Core.lib，5 文件全入库（阶段4）"
```

---

### 任务 6：清理与最终验证（阶段 4 收尾）

**目的**：清理临时文件，确认最终态，更新 .gitignore。

**文件：**
- 修改：`.gitignore`（确保 Lib/、build_*.txt、_*.map 忽略）

- [ ] **步骤 1：清理临时文件**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f _gen_proj.py build_lib.txt build_app.txt build_log.txt _*.map 2>/dev/null
rm -f Objects/*.o Objects/*.crf Objects/*.d Objects/*.htm Objects/*.lnp Objects/*.dep 2>/dev/null
rm -rf Lib/*.o 2>/dev/null
```
注意：保留 `Lib/ECAT_Core.lib`（交付物）、`Objects/EtherCAT_RFID.hex`（烧录用）。`_gen_proj.py` 在任务全部完成后删除（未来改库结构时重新写）。

- [ ] **步骤 2：确认 .gitignore 覆盖编译产物**

检查 `.gitignore` 包含：
```
MDK-ARM/Lib/
MDK-ARM/Objects/*.o
MDK-ARM/Objects/*.axf
MDK-ARM/build_*.txt
MDK-ARM/_*.map
MDK-ARM/_gen_proj.py
```
若缺则补充。

- [ ] **步骤 3：最终全量 Rebuild 验证**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32/MDK-ARM"
rm -f Lib/*.lib Lib/*.o Objects/*.o Objects/*.axf Objects/*.hex
"D:/Keil_V5/UV4/UV4.exe" -j0 -b ECAT_Core.uvprojx -o /dev/null
"D:/Keil_V5/UV4/UV4.exe" -j0 -b EtherCAT_RFID.uvprojx -o build_final.txt
grep -E "Error|Warning|Program Size" build_final.txt
ls -la Lib/ECAT_Core.lib Objects/EtherCAT_RFID.hex
```
预期：0 Error 0 Warning，lib 和 hex 都存在。

- [ ] **步骤 4：最终烧录验证（需人工）**

烧录 `Objects/EtherCAT_RFID.hex`，执行完整验证清单（同任务 5 步骤 4）。

- [ ] **步骤 5：Commit 清理**

```bash
cd "d:/YPC/Desktop/Keil5 Project/EtherCAT_GD32"
git add .gitignore
git commit -m "chore: SSC 静态库完成，清理临时文件与 gitignore"
```

---

## 自检结果

**规格覆盖度**：
- 阶段 0（基线准备）→ 任务 0（脚本）+ 任务 1 步骤 1（建 Lib/）
- 阶段 1（mailbox 探针）→ 任务 1
- 阶段 2（逐个加文件）→ 任务 2/3/4/5
- 阶段 3（0x0014 修复）→ 任务 4A
- 阶段 4（全入库+最终验证）→ 任务 5 + 任务 6
- 保符号机制（编译设置一致、FileType=4、Misc 空）→ 任务 0 脚本保证（从干净版复制 TargetOption）
- 验证清单 → 每个任务的步骤 4 + 任务 6 步骤 4
全部覆盖。

**占位符扫描**：任务 4A 步骤 3 的"方案 A/B/C"是条件分支（根据 map 对比结果选），每个方案都给了具体做法，不是占位符。任务 4A 步骤 6 的 commit message 有 `<方案描述>` 占位 → 修复为：执行时填实际选用的方案。

**类型一致性**：脚本参数 `--lib-files`、`--with-lib` 在所有任务一致；文件名列表（ALL_SSC）与规格的 9 个文件一致；FileType=4（lib）在任务 0 脚本和规格一致。
