#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""主字库解析工具"""
import struct
import sys
import re
import os

def parse_vlw_font(file_path):
    """解析VLW格式字体文件"""
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 提取十六进制数据
    hex_values = re.findall(r'0x([0-9A-Fa-f]{2})', content)
    if not hex_values:
        return set()
    
    data = bytes([int(h, 16) for h in hex_values])
    characters = set()
    
    # 策略1: Unicode码点扫描(小端序) - 包含ASCII数字和中文
    for i in range(len(data) - 1):
        code = struct.unpack('<H', data[i:i+2])[0]
        # 中文范围 + ASCII可见字符
        if (0x0020 <= code <= 0x007E) or (0x4E00 <= code <= 0x9FFF):
            try:
                characters.add(chr(code))
            except:
                pass
    
    # 策略2: 大端序扫描
    for i in range(len(data) - 1):
        code = struct.unpack('>H', data[i:i+2])[0]
        if (0x0020 <= code <= 0x007E) or (0x4E00 <= code <= 0x9FFF):
            try:
                characters.add(chr(code))
            except:
                pass
    
    # 策略3: 32位小端序扫描(高位为0的填充)
    for i in range(len(data) - 3):
        if data[i+2] == 0x00 and data[i+3] == 0x00:
            code = struct.unpack('<H', data[i:i+2])[0]
            if (0x0020 <= code <= 0x007E) or (0x4E00 <= code <= 0x9FFF):
                try:
                    characters.add(chr(code))
                except:
                    pass
    
    # 策略4: UTF-8序列检测
    i = 0
    while i < len(data) - 2:
        if 0xE4 <= data[i] <= 0xE9:
            if 0x80 <= data[i+1] <= 0xBF and 0x80 <= data[i+2] <= 0xBF:
                try:
                    char = bytes([data[i], data[i+1], data[i+2]]).decode('utf-8')
                    if '\u4e00' <= char <= '\u9fff':
                        characters.add(char)
                except:
                    pass
        i += 1
    
    # 策略5: 字形表结构解析(完整32位码点)
    for entry_size in [12, 16, 20, 24, 28]:
        for i in range(0, min(10000, len(data) - entry_size), entry_size):
            try:
                code = struct.unpack('<I', data[i:i+4])[0]
                if (0x0020 <= code <= 0x007E) or (0x4E00 <= code <= 0x9FFF):
                    characters.add(chr(code))
            except:
                pass
    
    return characters

def scan_project_chars(project_root):
    """扫描项目代码中实际使用的中文字符和字符串"""
    used_chars = set()
    chinese_strings = []
    extensions = ['.ino', '.cpp', '.h', '.c']
    
    for root, dirs, files in os.walk(project_root):
        # 跳过备份目录
        if 'backup' in root.lower() or '__vm' in root:
            continue
            
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                        content = f.read()
                        
                        # 查找字符串字面量中的中文
                        # 匹配 "..." 和 '...' 中的内容
                        string_pattern = r'["\']([^"\']*[\u4e00-\u9fff][^"\']*)["\']'
                        matches = re.findall(string_pattern, content)
                        
                        for match in matches:
                            has_chinese = False
                            for char in match:
                                if '\u4e00' <= char <= '\u9fff':
                                    used_chars.add(char)
                                    has_chinese = True
                            if has_chinese:
                                chinese_strings.append((match, os.path.basename(file_path)))
                        
                except:
                    pass
    
    return used_chars, chinese_strings

def main():
    # 测试字符集
    test_chars = "贪吃蛇游戏历史记录2048局数打砖块飞机大战俄罗斯方块总开始退出继续暂停0123456789"
    
    # 解析主字体库
    font_file = 'src/ControllerSystem/chinese_32.h'
    project_root = 'src/ControllerSystem'
    
    print(f"解析字体文件: {font_file}")
    print()
    
    chars = parse_vlw_font(font_file)
    
    # 分类统计
    chinese_chars = [c for c in chars if '\u4e00' <= c <= '\u9fff']
    ascii_chars = [c for c in chars if '\u0020' <= c <= '\u007e']
    digits = [c for c in ascii_chars if c.isdigit()]
    
    chars_list = sorted(chars)
    
    print(f"✅ 找到 {len(chars_list)} 个字符 (中文: {len(chinese_chars)}, ASCII: {len(ascii_chars)})")
    if digits:
        print(f"   数字字符: {''.join(sorted(digits))}")
    print()
    
    # 显示前100个字符
    print("前100个字符:")
    for i in range(0, min(100, len(chars_list)), 20):
        print('  ' + ' '.join(chars_list[i:i+20]))
    print()
    
    # 扫描项目使用的字符
    print("=" * 70)
    print("扫描项目代码中的中文字符串...")
    print("=" * 70)
    print()
    
    project_chars, chinese_strings = scan_project_chars(project_root)
    project_chars_list = sorted(project_chars)
    
    print(f"✅ 项目代码中实际使用了 {len(project_chars_list)} 个中文字符")
    print()
    
    # 显示所有中文字符串
    print("=" * 70)
    print(f"找到 {len(chinese_strings)} 个包含中文的字符串:")
    print("=" * 70)
    print()
    
    for idx, (string, filename) in enumerate(chinese_strings, 1):
        print(f"{idx:3d}. [{filename:30s}] {string}")
    
    print()
    
    # 提取所有唯一的中文字符
    print("=" * 70)
    print("代码中使用的所有中文字符:")
    print("=" * 70)
    print()
    for i in range(0, len(project_chars_list), 50):
        print('  ' + ''.join(project_chars_list[i:i+50]))
    print()
    
    # 对比分析
    font_set = set(chinese_chars)
    in_both = [c for c in project_chars_list if c in font_set]
    missing = [c for c in project_chars_list if c not in font_set]
    
    print("=" * 70)
    print("对比分析:")
    print("=" * 70)
    print()
    print(f"✅ 字库已包含: {len(in_both)}/{len(project_chars_list)} ({len(in_both)*100//len(project_chars_list)}%)")
    if in_both:
        print("   " + ''.join(in_both[:100]))
        if len(in_both) > 100:
            print(f"   ... 还有 {len(in_both)-100} 个字符")
    print()
    
    if missing:
        print(f"❌ 字库缺失: {len(missing)} 个字符")
        print("   " + ''.join(missing))
        print()
    else:
        print("✅ 字库完整！项目所需字符全部包含。")
        print()
    
    # 测试字符覆盖
    print("=" * 70)
    print("测试字符覆盖:")
    print("=" * 70)
    print()
    
    found = []
    test_missing = []
    for char in test_chars:
        if char in chars:
            found.append(char)
        else:
            test_missing.append(char)
    
    print(f"覆盖率: {len(found)}/{len(test_chars)} ({len(found)*100//len(test_chars)}%)")
    print()
    
    if found:
        print(f"✅ 包含的字符 ({len(found)}个):")
        print(''.join(found))
        print()
    
    if test_missing:
        print(f"❌ 缺失的字符 ({len(test_missing)}个):")
        print(''.join(test_missing))
        print()
    
    return 0 if not missing else 1

if __name__ == '__main__':
    sys.exit(main())
