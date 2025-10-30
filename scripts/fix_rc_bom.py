#!/usr/bin/env python3
"""Remove BOM from RC files but keep UTF-8 encoding with pragma."""

import os
import sys
from pathlib import Path

def fix_rc_bom(file_path):
    """Remove BOM from RC file but keep UTF-8 encoding."""
    try:
        with open(file_path, 'r', encoding='utf-8-sig') as f:
            content = f.read()
        
        with open(file_path, 'w', encoding='utf-8', newline='') as f:
            f.write(content)
        
        return True, "Removed BOM"
    
    except Exception as e:
        return False, str(e)

def main():
    base_dir = Path('/home/ubuntu/repos/TohoSpec')
    extensions = ['.rc', '.rc2']
    
    success_count = 0
    error_count = 0
    
    for ext in extensions:
        for file_path in base_dir.rglob(f'*{ext}'):
            if 'reports' in str(file_path) or '_logs' in str(file_path) or 'scripts' in str(file_path):
                continue
            
            rel_path = file_path.relative_to(base_dir)
            success, message = fix_rc_bom(file_path)
            
            if success:
                success_count += 1
                print(f"✓ {rel_path}: {message}")
            else:
                error_count += 1
                print(f"✗ {rel_path}: {message}")
    
    print(f"\n{'='*80}")
    print(f"RC BOM removal complete!")
    print(f"Success: {success_count}")
    print(f"Errors: {error_count}")
    
    return 0 if error_count == 0 else 1

if __name__ == '__main__':
    sys.exit(main())
