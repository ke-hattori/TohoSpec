#!/usr/bin/env python3
"""Add #pragma code_page(65001) to .rc and .rc2 files."""

import os
import sys
from pathlib import Path

def fix_rc_file(file_path):
    """Add #pragma code_page(65001) to RC file if not present."""
    try:
        with open(file_path, 'r', encoding='utf-8-sig') as f:
            content = f.read()
        
        if '#pragma code_page(65001)' in content or 'code_page(65001)' in content:
            return False, "Already has pragma"
        
        new_content = '#pragma code_page(65001)\n' + content
        
        with open(file_path, 'w', encoding='utf-8-sig') as f:
            f.write(new_content)
        
        return True, "Added pragma"
    
    except Exception as e:
        return False, str(e)

def main():
    base_dir = Path('/home/ubuntu/repos/TohoSpec')
    extensions = ['.rc', '.rc2']
    
    results = []
    success_count = 0
    skip_count = 0
    error_count = 0
    
    for ext in extensions:
        for file_path in base_dir.rglob(f'*{ext}'):
            if 'reports' in str(file_path) or '_logs' in str(file_path) or 'scripts' in str(file_path):
                continue
            
            rel_path = file_path.relative_to(base_dir)
            success, message = fix_rc_file(file_path)
            
            if success:
                success_count += 1
                print(f"✓ {rel_path}: {message}")
            elif "Already has pragma" in message:
                skip_count += 1
            else:
                error_count += 1
                print(f"✗ {rel_path}: {message}")
    
    print(f"\n{'='*80}")
    print(f"RC file pragma addition complete!")
    print(f"Added pragma: {success_count}")
    print(f"Already had pragma: {skip_count}")
    print(f"Errors: {error_count}")
    
    return 0 if error_count == 0 else 1

if __name__ == '__main__':
    sys.exit(main())
