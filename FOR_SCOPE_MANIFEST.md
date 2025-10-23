# For-Variable Hoisting TODO Manifest

This document tracks all files that require proper for-variable hoisting after removing `/Zc:forScope-` from MEASYS, NanoSpec, and MotTsk projects.

## Background

The `/Zc:forScope-` compiler flag was temporarily applied to MEASYS, NanoSpec, and MotTsk projects (Debug|Win32 only) to enable legacy VC6 for-loop scoping behavior. This allows for-loop variables to be visible after the loop ends, which is non-standard C++ behavior.

Once the solution compiles cleanly, we need to:
1. Remove `/Zc:forScope-` from all three projects
2. For each file listed below, perform minimal mechanical fixes:
   - If a loop index is used after the loop, HOIST its declaration above the loop
   - Restore braces/close comments if needed
   - Avoid refactors and behavior changes

## Files Requiring For-Variable Hoisting

### MEASYS Project
- **SRC/MeaSys/Xmp.cpp** - Multiple for-loop variables used after loops
- **SRC/MeaSys/SrRefFile.cpp** - Multiple for-loop variables used after loops
- **SRC/MeaSys/SrAnalysis.cpp** - Multiple for-loop variables used after loops
- **SRC/MeaSys/RadiusOfCurvature.cpp** - Multiple for-loop variables used after loops

### NanoSpec Project
Files that were fixed manually (already hoisted):
- SRC/NanoSpec/MotTsk/WIN.CPP (already fixed - iteration 26)
- SRC/NanoSpec/TestMeasurementDlg.cpp (already fixed - iteration 26)
- SRC/NanoSpec/SrVideoCalibrationSettingDlg.cpp (already fixed - iteration 26)
- SRC/NanoSpec/SrCofigurationSettingDlg.cpp (already fixed - iteration 27)

**Note**: The `/Zc:forScope-` flag was applied to NanoSpec in iteration 28, which eliminated ~240 for-scope errors. These files likely have for-loop variables used after loops but were not individually tracked. A comprehensive search will be needed when removing the flag.

### MotTsk Project
Files that were fixed manually (already hoisted):
- SRC/MotTsk/WIN.CPP (already fixed - iteration 26)

**Note**: The `/Zc:forScope-` flag was applied to MotTsk in iteration 28, which eliminated ~48 for-scope errors. These files likely have for-loop variables used after loops but were not individually tracked. A comprehensive search will be needed when removing the flag.

## Fix Pattern

For each file, apply this mechanical transformation:

```cpp
// BEFORE (non-standard, requires /Zc:forScope-)
for (int i = 0; i < n; ++i) {
    // loop body
}
use(i);  // ERROR without /Zc:forScope-

// AFTER (standard C++, no flag needed)
int i;
for (i = 0; i < n; ++i) {
    // loop body
}
use(i);  // OK
```

## Verification Steps

1. Remove `/Zc:forScope-` from MEASYS.vcxproj (Debug|Win32)
2. Remove `/Zc:forScope-` from NanoSpec.vcxproj (Debug|Win32)
3. Remove `/Zc:forScope-` from MOTTSK.vcxproj (Debug|Win32)
4. Build Debug|Win32 and collect all C2065 errors (undeclared identifier)
5. For each error, apply the hoisting pattern above
6. Rebuild until clean
7. Provide a short diff summary per file

## Status

- [x] Manifest created
- [ ] /Zc:forScope- removed from projects
- [ ] MEASYS files fixed
- [ ] NanoSpec files identified and fixed
- [ ] MotTsk files identified and fixed
- [ ] Final verification build passed
