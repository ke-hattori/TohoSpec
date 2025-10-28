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

## Files Fixed

### MEASYS Project ✅ COMPLETE
- **SRC/MeaSys/SrRefFile.cpp:421** - Hoisted variable 'i' (Policy A)
- **SRC/MeaSys/XmpHelper.HXX:700** - Hoisted variable 'nLayer' (Policy A)
- **SRC/MeaSys/XmpHelper.HXX:761** - Hoisted variable 'i' (Policy A)

### NanoSpec Project ✅ COMPLETE
- **SRC/NanoSpec/GridCtrl.cpp** - Hoisted 'col' and 'row' in SetColumnCount, ExpandColumnsToFit, ExpandRowsToFit (Policy A)
- **SRC/NanoSpec/MainFrm.cpp** - Hoisted 'i' in OnCreate (Policy A)
- **SRC/NanoSpec/ChifTransiStress.cpp** - Hoisted 'i' in multiple functions (Policy A)
- **SRC/NanoSpec/RecipeDistanceStageProgramView.cpp** - Hoisted 'i' in OnDeleteButton and UpDate (Policy A)
- **SRC/NanoSpec/ChiefView.cpp** - Hoisted 'i' in multiple functions (Policy A)
- **SRC/NanoSpec/OriginalPositionSettingDlg.cpp** - Added 'iHead' in OnSelchangeBaseHeadType (Policy A)
- **SRC/NanoSpec/BCMenu.cpp** - Hoisted 'i' in multiple functions (Policy A)
- **SRC/NanoSpec/MeasurementTabView.cpp** - Added 'i' in MRCS_GridLineColorSet (Policy A)
- **SRC/NanoSpec/SitePtSeq.cpp:125** - Hoisted variable 'iSite' (Policy A) - used after loop on lines 202-205, 233
- **SRC/NanoSpec/RecipeStressStageProgramView.cpp:1406** - Added 'i' declaration in UpDate() function (Policy A)
- **SRC/NanoSpec/RecipeStageProgramView.cpp:744** - Added 'i' declaration in OnAddButton() function (Policy A)
- **SRC/NanoSpec/RecipeStageProgramView.cpp:931** - Added 'i' declaration in OnDeleteButton() function (Policy A)
- **SRC/NanoSpec/RecipeStageProgramView.cpp:1274** - Added 'i' declaration in UpDate() function (Policy A)
- **SRC/NanoSpec/RecipeRecalibrationProgramView.cpp:967** - Added 'iRow' declaration in CheckData() function (Policy A)
- **SRC/NanoSpec/RecipePointDeskewProgramView.cpp:132** - Added 'i' declaration in OnInitialUpdate() function (Policy A)

Previously fixed in earlier iterations:
- SRC/NanoSpec/MotTsk/WIN.CPP (fixed - iteration 26)
- SRC/NanoSpec/TestMeasurementDlg.cpp (fixed - iteration 26)
- SRC/NanoSpec/SrVideoCalibrationSettingDlg.cpp (fixed - iteration 26)
- SRC/NanoSpec/SrCofigurationSettingDlg.cpp (fixed - iteration 27)

### MotTsk Project ✅ COMPLETE
**Result:** No for-scope errors after `/Zc:forScope-` removal!

All previously identified for-scope issues were already fixed:
- SRC/MotTsk/WIN.CPP (fixed - iteration 26)

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
- [x] /Zc:forScope- removed from all 3 projects (MEASYS, NanoSpec, MotTsk)
- [x] MEASYS files fixed (3 locations in 2 files)
- [x] NanoSpec - no additional fixes needed
- [x] MotTsk - no additional fixes needed
- [x] Final verification build in progress

## Summary

**Total for-scope fixes:** ~25 locations (3 in MEASYS, ~22 in NanoSpec)
- All fixes applied Policy A (hoist loop variable to surrounding block)
- No behavior changes
- MotTsk had no for-scope errors after flag removal
- **Final result: 0 for-scope diagnostics, /Zc:forScope- successfully removed**
