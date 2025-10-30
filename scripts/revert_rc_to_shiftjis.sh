#!/bin/bash

cd /home/ubuntu/repos/TohoSpec

find . -name "*.rc" -o -name "*.rc2" | grep -v "reports\|_logs\|scripts" | while read file; do
    echo "Reverting $file to Shift-JIS..."
    git show a57cb79:"${file#./}" > "$file" 2>/dev/null || echo "  Warning: Could not restore $file"
done

echo "Done! All RC files reverted to original Shift-JIS encoding."
