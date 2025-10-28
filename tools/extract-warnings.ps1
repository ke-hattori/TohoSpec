# Extract and analyze warnings from MSBuild log
# Focus on uninitialized variables and bounds checking warnings

param(
    [Parameter(Mandatory=$true)]
    [string]$LogFile,
    
    [Parameter(Mandatory=$true)]
    [string]$OutputDir
)

# Target warning codes for uninitialized/bounds issues
$targetWarnings = @(
    'C4700',  # uninitialized local variable used
    'C4701',  # potentially uninitialized local variable used
    'C4703',  # potentially uninitialized local pointer variable used
    'C6001',  # using uninitialized memory
    'C6201',  # buffer overrun for stack allocated buffer
    'C6385',  # reading invalid data from buffer
    'C6386',  # buffer overrun while writing to buffer
    'C6387'   # invalid parameter value
)

# Unicode-related warnings to exclude from general count
$unicodeWarnings = @('C4133', 'C4244', 'C4267')

Write-Host "Extracting warnings from: $LogFile"

if (-not (Test-Path $LogFile)) {
    Write-Error "Log file not found: $LogFile"
    exit 1
}

# Read all lines from log
$lines = Get-Content $LogFile

# Parse warnings
$warnings = @()
$warningPattern = ':\s+(warning|error)\s+([A-Z]+\d+):\s+(.+)$'

foreach ($line in $lines) {
    if ($line -match $warningPattern) {
        $type = $matches[1]
        $code = $matches[2]
        $message = $matches[3]
        
        # Extract file path
        $filePath = ''
        if ($line -match '^(.+?)\((\d+)\)') {
            $filePath = $matches[1]
        }
        
        $warnings += [PSCustomObject]@{
            Type = $type
            Code = $code
            Message = $message
            File = $filePath
            Line = $line
        }
    }
}

Write-Host "Total warnings parsed: $($warnings.Count)"

# Filter out Unicode warnings for general count
$nonUnicodeWarnings = $warnings | Where-Object { $_.Code -notin $unicodeWarnings }
Write-Host "Non-Unicode warnings: $($nonUnicodeWarnings.Count)"

# Count target warnings (uninitialized/bounds)
$targetWarningInstances = $warnings | Where-Object { $_.Code -in $targetWarnings }
Write-Host "Uninitialized/Bounds warnings: $($targetWarningInstances.Count)"

# Group by code
$byCode = $nonUnicodeWarnings | Group-Object -Property Code | 
    Select-Object @{N='code';E={$_.Name}}, @{N='count';E={$_.Count}} |
    Sort-Object -Property count -Descending

# Group by file
$byFile = $nonUnicodeWarnings | Where-Object { $_.File } | 
    Group-Object -Property File |
    Select-Object @{N='file';E={$_.Name}}, @{N='count';E={$_.Count}} |
    Sort-Object -Property count -Descending

# Group target warnings by code
$targetByCode = $targetWarningInstances | Group-Object -Property Code |
    Select-Object @{N='code';E={$_.Name}}, @{N='count';E={$_.Count}} |
    Sort-Object -Property count -Descending

# Group target warnings by file
$targetByFile = $targetWarningInstances | Where-Object { $_.File } |
    Group-Object -Property File |
    Select-Object @{N='file';E={$_.Name}}, @{N='count';E={$_.Count}} |
    Sort-Object -Property count -Descending

# Create output object
$output = @{
    total = $nonUnicodeWarnings.Count
    totalWithUnicode = $warnings.Count
    targetWarnings = $targetWarningInstances.Count
    byCode = $byCode
    byFile = $byFile
    targetByCode = $targetByCode
    targetByFile = $targetByFile
}

# Save JSON
$jsonPath = Join-Path $OutputDir "warnings.json"
$output | ConvertTo-Json -Depth 10 | Out-File -FilePath $jsonPath -Encoding UTF8
Write-Host "Saved JSON to: $jsonPath"

# Save CSV for target warnings
$csvPath = Join-Path $OutputDir "target_warnings.csv"
$targetWarningInstances | Select-Object Code, File, Message | 
    Export-Csv -Path $csvPath -NoTypeInformation -Encoding UTF8
Write-Host "Saved target warnings CSV to: $csvPath"

# Save summary text
$summaryPath = Join-Path $OutputDir "warnings_summary.txt"
$summary = @"
Warning Analysis Summary
========================

Total Warnings (excluding Unicode): $($output.total)
Total Warnings (including Unicode): $($output.totalWithUnicode)

CRITICAL: Uninitialized/Bounds Warnings: $($output.targetWarnings)

Target Warning Codes:
$($targetByCode | ForEach-Object { "  $($_.code): $($_.count)" } | Out-String)

Top 10 Warning Codes (all):
$($byCode | Select-Object -First 10 | ForEach-Object { "  $($_.code): $($_.count)" } | Out-String)

Top 10 Files with Target Warnings:
$($targetByFile | Select-Object -First 10 | ForEach-Object { "  $($_.file): $($_.count)" } | Out-String)

Top 10 Files (all warnings):
$($byFile | Select-Object -First 10 | ForEach-Object { "  $($_.file): $($_.count)" } | Out-String)
"@

$summary | Out-File -FilePath $summaryPath -Encoding UTF8
Write-Host "Saved summary to: $summaryPath"

Write-Host "Extraction complete!"
