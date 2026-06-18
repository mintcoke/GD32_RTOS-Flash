# -*- coding: utf-8 -*-
"""生成 ECAT_Core.uvprojx（库工程）和 EtherCAT_RFID.uvprojx（应用工程）。
从全源码基线 commit 0cf5a3b 提取 TargetOption 和文件路径，纯字符串拼接。
通用版：--lib-files 可指定任意文件进库（SSC / 外设驱动 / system 等），
脚本按文件原始所属 Group 自动归类。

用法：
  python _gen_proj.py --lib-files mailbox.c sdoserv.c --with-lib
  python _gen_proj.py --lib-files mailbox.c ... gd32f10x_spi.c --with-lib
"""
import re, subprocess, sys, os

# 全源码基线 commit（含全部 37 文件，固定不变）
BASELINE_COMMIT = '0cf5a3b'

def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument('--lib-files', nargs='*', default=[], help='进库的文件名（不含路径）')
    ap.add_argument('--with-lib', action='store_true', help='应用工程引用 ECAT_Core.lib')
    args = ap.parse_args()

    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    clean = subprocess.check_output(
        ['git', 'show', f'{BASELINE_COMMIT}:MDK-ARM/EtherCAT_RFID.uvprojx'],
        cwd=repo_root, text=True, encoding='utf-8')

    topt = re.search(r'<TargetOption>.*?</TargetOption>', clean, re.DOTALL).group(0)
    old_od = re.search(r'<OutputDirectory>[^<]*</OutputDirectory>', topt).group(0)
    before = clean[:clean.index('  <Targets>')]
    after = clean[clean.index('  </Targets>') + len('  </Targets>'):]

    # 从基线提取「文件 → (FileType, FilePath, 所属 Group)」映射，保持原始 Group 顺序
    all_files = {}      # fn -> (filetype, filepath)
    file_group = {}     # fn -> groupname
    group_order = []    # Group 出现顺序
    # 用正则按 Group 块解析
    for gm in re.finditer(r'<GroupName>([^<]+)</GroupName>\s*<Files>(.*?)</Files>', clean, re.DOTALL):
        gn = gm.group(1)
        if gn not in group_order:
            group_order.append(gn)
        for fm in re.finditer(r'<FileName>([^<]+)</FileName>\s*<FileType>(\d+)</FileType>\s*<FilePath>([^<]+)</FilePath>', gm.group(2)):
            fn = fm.group(1)
            all_files[fn] = (fm.group(2), fm.group(3))
            file_group[fn] = gn

    lib_set = set(args.lib_files)

    # 应用工程 Group：每个 Group 保留「未进库」的文件，保持原始顺序
    APP_GROUPS = {}
    for gn in group_order:
        files_in_group = [fn for fn in all_files if file_group[fn] == gn and fn not in lib_set]
        if files_in_group:
            APP_GROUPS[gn] = files_in_group

    # 库工程 Group：进库文件按原始 Group 归类
    LIB_GROUPS = {}
    for fn in args.lib_files:
        if fn not in all_files:
            print(f'!! 警告: {fn} 不在基线工程中，跳过', file=sys.stderr)
            continue
        gn = file_group[fn]
        LIB_GROUPS.setdefault(gn, []).append(fn)

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

    # XML 合法性校验 + 打印
    import xml.etree.ElementTree as ET
    for fn in ['EtherCAT_RFID.uvprojx', 'ECAT_Core.uvprojx']:
        t = ET.parse(fn)
        for tg in t.getroot().find('Targets').findall('Target'):
            name = tg.find('TargetName').text
            print(f'[{fn}] {name}:')
            for g in tg.findall('Groups/Group'):
                fns = [f.find('FileName').text for f in g.findall('Files/File')]
                print(f'    {g.find("GroupName").text}: {len(fns)} files')

if __name__ == '__main__':
    main()
