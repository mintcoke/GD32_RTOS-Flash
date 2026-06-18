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
    # 注意：gd32f10x_it.c 属于 User Group（中断处理），不在外设驱动里，需排除
    FW_FILES = [m.group(1) for m in re.finditer(
        r'<FileName>(gd32f10x_\w+\.c)</FileName>', clean)
        if m.group(1) != 'gd32f10x_it.c']
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
