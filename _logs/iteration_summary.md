# TohoSpec VS2022 Migration - Iteration Summary

## Compile Error Progress

| Iteration | Compile Errors | Linker Errors | Total Errors | Key Changes |
|-----------|----------------|---------------|--------------|-------------|
| 12 | 3 (C1189) | 13 | 16 | Started with MFC runtime issues |
| 13 | 160+ (C2065) | 26 | 186 | Dynamic MFC exposed iLine scoping issues |
| 14 | 101 (C3867) | 14 | 115 | Fixed iLine, but member function pointer issues |
| 15 | 101 (C3867) | 18 | 119 | Fixed CPifMessage502 member function pointers |
| 16 | 101 (C3867) | 14 | 115 | Still had CPifMessage503 patterns |
| 17 | 0 | 13 | 13 | **All compile errors fixed!** |

## Remaining Errors (All Linker/Build Order)

### LNK1104 Errors (3)
- dTnsConfigFile.lib (HOOK project)
- dtnsnexini.lib (NEXUI project)
- TIS_UDSHL12d.lib (OVERLAY project)

### MSB3073 Errors (5)
- NANOSPECTITLERCENU.dll copy failure
- NANOSPECRCJPN.dll copy failure
- NANOSPECRCENU.dll copy failure
- NANOSPECTITLERCJPN.dll copy failure
- NanoSpecTitle.exe copy failure

## Key Fixes Applied

1. **MFC Configuration**: Changed RecipeFile, PatternFile, PifComm to Dynamic MFC with MultiThreadedDebugDLL runtime
2. **Variable Scoping**: Declared iLine outside for loops in Recipe.cpp (5 locations)
3. **Member Function Pointers**: Added & operator for 501 member function pointer assignments in PifComm.cpp
   - CPifMessage502: 120 fixes
   - CPifMessage503: 381 fixes

## Next Steps

1. Analyze project dependencies to understand build order
2. Add ProjectReference entries to resolve MSB3073 copy failures
3. Investigate missing .lib files (may need to build dependent projects first)
4. Rebuild with /m after dependencies are fixed
