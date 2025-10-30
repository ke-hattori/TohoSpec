#!/usr/bin/env python3
"""Scan source files for encoding and non-ASCII characters."""

import os
import sys
from pathlib import Path
import chardet

def detect_encoding(file_path):
    """Detect file encoding."""
    try:
        with open(file_path, 'rb') as f:
            raw_data = f.read()
            result = chardet.detect(raw_data)
            return result['encoding'], result['confidence']
    except Exception as e:
        return None, 0

def scan_file_for_non_ascii(file_path):
    """Scan file for non-ASCII characters."""
    encoding, confidence = detect_encoding(file_path)
    
    if not encoding:
        return None
    
    try:
        with open(file_path, 'r', encoding=encoding, errors='replace') as f:
            lines = f.readlines()
            
        non_ascii_found = []
        for line_num, line in enumerate(lines, 1):
            for col_num, char in enumerate(line, 1):
                if ord(char) > 127:
                    non_ascii_found.append({
                        'line': line_num,
                        'col': col_num,
                        'char': char,
                        'codepoint': f"U+{ord(char):04X}",
                        'context': line.strip()[:80]
                    })
        
        return {
            'encoding': encoding,
            'confidence': confidence,
            'non_ascii_count': len(non_ascii_found),
            'non_ascii': non_ascii_found[:10]  # First 10 occurrences
        }
    except Exception as e:
        return None

def main():
    base_dir = Path('/home/ubuntu/repos/TohoSpec')
    extensions = ['.c', '.cpp', '.cc', '.cxx', '.h', '.hpp', '.inl', '.rc', '.rc2', '.def', '.idl']
    
    results = []
    
    for ext in extensions:
        for file_path in base_dir.rglob(f'*{ext}'):
            if 'reports' in str(file_path) or '_logs' in str(file_path):
                continue
                
            result = scan_file_for_non_ascii(file_path)
            if result and result['non_ascii_count'] > 0:
                results.append({
                    'file': str(file_path.relative_to(base_dir)),
                    **result
                })
    
    results.sort(key=lambda x: x['non_ascii_count'], reverse=True)
    
    print(f"Total files with non-ASCII characters: {len(results)}")
    print(f"\nTop 20 files by non-ASCII character count:\n")
    
    for i, result in enumerate(results[:20], 1):
        print(f"{i}. {result['file']}")
        print(f"   Encoding: {result['encoding']} (confidence: {result['confidence']:.2f})")
        print(f"   Non-ASCII count: {result['non_ascii_count']}")
        if result['non_ascii']:
            print(f"   First occurrence: Line {result['non_ascii'][0]['line']}, {result['non_ascii'][0]['codepoint']}")
        print()
    
    report_path = base_dir / 'reports' / 'cp932_scan.md'
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write("# CP932 / Non-ASCII Character Scan\n\n")
        f.write(f"Total files scanned: {len(results)}\n\n")
        f.write("## Files with Non-ASCII Characters\n\n")
        
        for result in results:
            f.write(f"### {result['file']}\n\n")
            f.write(f"- **Encoding**: {result['encoding']} (confidence: {result['confidence']:.2f})\n")
            f.write(f"- **Non-ASCII count**: {result['non_ascii_count']}\n\n")
            
            if result['non_ascii']:
                f.write("| Line | Col | Codepoint | Context |\n")
                f.write("|------|-----|-----------|----------|\n")
                for item in result['non_ascii']:
                    context = item['context'].replace('|', '\\|')
                    f.write(f"| {item['line']} | {item['col']} | {item['codepoint']} | `{context}` |\n")
                f.write("\n")
    
    print(f"\nDetailed report saved to: {report_path}")

if __name__ == '__main__':
    main()
