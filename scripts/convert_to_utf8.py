#!/usr/bin/env python3
"""Convert source files to UTF-8 with BOM."""

import os
import sys
from pathlib import Path
import chardet
import codecs

def detect_encoding(file_path):
    """Detect file encoding."""
    try:
        with open(file_path, 'rb') as f:
            raw_data = f.read()
            
            if raw_data.startswith(codecs.BOM_UTF8):
                return 'utf-8-sig', 1.0
            
            if raw_data.startswith(codecs.BOM_UTF16_LE) or raw_data.startswith(codecs.BOM_UTF16_BE):
                return 'utf-16', 1.0
            
            result = chardet.detect(raw_data)
            return result['encoding'], result['confidence']
    except Exception as e:
        return None, 0

def convert_file_to_utf8_bom(file_path, dry_run=False):
    """Convert a single file to UTF-8 with BOM."""
    try:
        encoding, confidence = detect_encoding(file_path)
        
        if not encoding:
            return False, "Could not detect encoding"
        
        try:
            with open(file_path, 'r', encoding=encoding, errors='replace') as f:
                content = f.read()
        except Exception as e:
            for fallback_encoding in ['shift_jis', 'cp932', 'utf-8', 'latin-1']:
                try:
                    with open(file_path, 'r', encoding=fallback_encoding, errors='replace') as f:
                        content = f.read()
                    encoding = fallback_encoding
                    break
                except:
                    continue
            else:
                return False, f"Could not read file with any encoding"
        
        if not dry_run:
            with open(file_path, 'w', encoding='utf-8-sig', newline='') as f:
                f.write(content)
        
        return True, f"Converted from {encoding} (confidence: {confidence:.2f})"
    
    except Exception as e:
        return False, str(e)

def main():
    base_dir = Path('/home/ubuntu/repos/TohoSpec')
    extensions = ['.c', '.cpp', '.cc', '.cxx', '.h', '.hpp', '.inl', '.rc', '.rc2', '.def', '.idl']
    
    dry_run = '--dry-run' in sys.argv
    batch_size = 50  # Process in batches
    
    files_to_convert = []
    for ext in extensions:
        for file_path in base_dir.rglob(f'*{ext}'):
            if 'reports' in str(file_path) or '_logs' in str(file_path) or 'scripts' in str(file_path):
                continue
            files_to_convert.append(file_path)
    
    print(f"Found {len(files_to_convert)} files to process")
    print(f"Mode: {'DRY RUN' if dry_run else 'LIVE CONVERSION'}")
    print()
    
    success_count = 0
    error_count = 0
    errors = []
    
    for i, file_path in enumerate(files_to_convert, 1):
        rel_path = file_path.relative_to(base_dir)
        success, message = convert_file_to_utf8_bom(file_path, dry_run)
        
        if success:
            success_count += 1
            if i % 50 == 0:
                print(f"Processed {i}/{len(files_to_convert)}: {rel_path}")
        else:
            error_count += 1
            errors.append((str(rel_path), message))
            print(f"ERROR {i}/{len(files_to_convert)}: {rel_path} - {message}")
    
    print(f"\n{'='*80}")
    print(f"Conversion {'simulation' if dry_run else 'complete'}!")
    print(f"Success: {success_count}")
    print(f"Errors: {error_count}")
    
    if errors:
        print(f"\nFiles with errors:")
        for file_path, error in errors:
            print(f"  - {file_path}: {error}")
    
    return 0 if error_count == 0 else 1

if __name__ == '__main__':
    sys.exit(main())
