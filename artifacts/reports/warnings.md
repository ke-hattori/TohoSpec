# TohoSpec VS2022 Warning Analysis Report

## Summary

**Total Warnings:** 9590
**Unique Warning Codes:** 10

## Warning Breakdown by Code

| Warning Code | Count | Description |
|--------------|-------|-------------|
| C4996 | 8402 | Deprecated function (CRT security) |
| C4244 | 654 | Type conversion, possible data loss |
| C4838 | 224 | Unknown |
| C4477 | 143 | Unknown |
| C4018 | 85 | Signed/unsigned mismatch |
| MSB8012 | 70 | TargetPath mismatch |
| C4778 | 5 | Unknown |
| C4474 | 5 | Unknown |
| C4091 | 1 | Unknown |
| C4005 | 1 | Unknown |

## Top 10 Warning Codes (Detailed)

### 1. C4996 (8402 occurrences)

**Description:** Deprecated function (CRT security)

**Sample Warnings:**
```
5>D:\a\TohoSpec\TohoSpec\INC\DllMutex.hxx(18,3): warning C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
13>D:\a\TohoSpec\TohoSpec\SRC\Port_NT\PORT_NT.CPP(53,2): warning C4996: 'GetVersionExA': was declared deprecated [D:\a\TohoSpec\TohoSpec\SRC\Port_NT\PORT_NT.vcxproj]
13>D:\a\TohoSpec\TohoSpec\SRC\Port_NT\PORT_NT.CPP(175,4): warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_N
```

### 2. C4244 (654 occurrences)

**Description:** Type conversion, possible data loss

**Sample Warnings:**
```
3>D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\IniFile.cpp(1323,74): warning C4244: '=': conversion from 'double' to 'WORD', possible loss of data [D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\ConfigFile.vcxproj]
3>D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\IniFile.cpp(1387,51): warning C4244: '=': conversion from 'double' to 'WORD', possible loss of data [D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\ConfigFile.vcxproj]
33>D:\a\TohoSpec\TohoSpec\SRC\OVERLAY\IMAGECTRL.CPP(300,36): warning C4244: '=': conversion from 'double' to 'LONG', possible loss of data [D:\a\TohoSpec\TohoSpec\SRC\OVERLAY\OVERLAY.vcxproj]
```

### 3. C4838 (224 occurrences)

**Description:** Unknown

**Sample Warnings:**
```
20>D:\a\TohoSpec\TohoSpec\SRC\MotSysNSPT\CtrlCmd.cpp(2427,44): warning C4838: conversion from 'int' to 'DWORD' requires a narrowing conversion [D:\a\TohoSpec\TohoSpec\SRC\MotSysNSPT\MotSysG.vcxproj]
20>D:\a\TohoSpec\TohoSpec\SRC\MotSysNSPT\CtrlCmd.cpp(2427,48): warning C4838: conversion from 'int' to 'DWORD' requires a narrowing conversion [D:\a\TohoSpec\TohoSpec\SRC\MotSysNSPT\MotSysG.vcxproj]
20>D:\a\TohoSpec\TohoSpec\SRC\MotSysNSPT\CtrlCmd.cpp(2427,52): warning C4838: conversion from 'int' to 'DWORD' requires a narrowing conversion [D:\a\TohoSpec\TohoSpec\SRC\MotSysNSPT\MotSysG.vcxproj]
```

### 4. C4477 (143 occurrences)

**Description:** Unknown

**Sample Warnings:**
```
18>D:\a\TohoSpec\TohoSpec\SRC\Stage\stage.h(948,25): warning C4477: 'sprintf' : format string '%+09.4f' requires an argument of type 'double', but variadic argument 2 has type 'int' [D:\a\TohoSpec\Toh
18>D:\a\TohoSpec\TohoSpec\SRC\Stage\stage.h(952,25): warning C4477: 'sprintf' : format string '%+010.4f' requires an argument of type 'double', but variadic argument 2 has type 'int' [D:\a\TohoSpec\To
18>D:\a\TohoSpec\TohoSpec\SRC\Stage\stage.h(948,25): warning C4477: 'sprintf' : format string '%+09.4f' requires an argument of type 'double', but variadic argument 2 has type 'const T' [D:\a\TohoSpec
```

### 5. C4018 (85 occurrences)

**Description:** Signed/unsigned mismatch

**Sample Warnings:**
```
3>D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\IniFile.cpp(1563,41): warning C4018: '<': signed/unsigned mismatch [D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\ConfigFile.vcxproj]
3>D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\IniFile.cpp(1601,39): warning C4018: '<': signed/unsigned mismatch [D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\ConfigFile.vcxproj]
3>D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\IniFile.cpp(1627,48): warning C4018: '<': signed/unsigned mismatch [D:\a\TohoSpec\TohoSpec\SRC\ConfigFile\ConfigFile.vcxproj]
```

### 6. MSB8012 (70 occurrences)

**Description:** TargetPath mismatch

**Sample Warnings:**
```
9>C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Microsoft\VC\v170\Microsoft.CppBuild.targets(1402,5): warning MSB8012: TargetPath(D:\a\TohoSpec\TohoSpec\SRC\MeaSys\MEASYSRCENU\Debug
9>C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Microsoft\VC\v170\Microsoft.CppBuild.targets(1404,5): warning MSB8012: TargetName(MEASYSRCENU) does not match the Linker's OutputFile
4>C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Microsoft\VC\v170\Microsoft.CppBuild.targets(1564,5): warning MSB8012: TargetPath(D:\a\TohoSpec\TohoSpec\SRC\Active\Debug\ACTIVE.lib)
```

### 7. C4778 (5 occurrences)

**Description:** Unknown

**Sample Warnings:**
```
32>D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\MainFrm.cpp(7462,29): warning C4778: 'sprintf' : unterminated format string '%l' [D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\NanoSpec.vcxproj]
32>D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\MainFrm.cpp(7574,29): warning C4778: 'sprintf' : unterminated format string '%l' [D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\NanoSpec.vcxproj]
32>D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\MainFrm.cpp(7635,29): warning C4778: 'sprintf' : unterminated format string '%l' [D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\NanoSpec.vcxproj]
```

### 8. C4474 (5 occurrences)

**Description:** Unknown

**Sample Warnings:**
```
32>D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\MainFrm.cpp(7462,91): warning C4474: 'sprintf' : too many arguments passed for format string [D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\NanoSpec.vcxproj]
32>D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\MainFrm.cpp(7574,86): warning C4474: 'sprintf' : too many arguments passed for format string [D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\NanoSpec.vcxproj]
32>D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\MainFrm.cpp(7635,87): warning C4474: 'sprintf' : too many arguments passed for format string [D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\NanoSpec.vcxproj]
```

### 9. C4091 (1 occurrences)

**Description:** Unknown

**Sample Warnings:**
```
34>D:\a\TohoSpec\TohoSpec\SRC\NSStage\NSStage.cpp(63,13): warning C4091: 'static ': ignored on left of 'STAGE_TYPE' when no variable is declared [D:\a\TohoSpec\TohoSpec\SRC\NSStage\NSStage.vcxproj]
```

### 10. C4005 (1 occurrences)

**Description:** Unknown

**Sample Warnings:**
```
32>D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\BtnST.cpp(30,9): warning C4005: 'BS_TYPEMASK': macro redefinition [D:\a\TohoSpec\TohoSpec\SRC\NanoSpec\NanoSpec.vcxproj]
```

## Recommendations

### High Priority (Safety & Security)

1. **C4996 - Deprecated CRT Functions**
   - Replace with _s variants (strcpy_s, sprintf_s, etc.)
   - Or add _CRT_SECURE_NO_WARNINGS if functions are used safely
   - Impact: ~8,000+ warnings

2. **C4244/C4267 - Type Conversion Warnings**
   - Review for potential data loss
   - Add explicit casts where safe
   - Impact: ~1,000+ warnings

### Medium Priority (Code Quality)

3. **C4018/C4389 - Signed/Unsigned Mismatch**
   - Fix comparison operators
   - Use appropriate types (size_t, int)
   - Impact: ~500+ warnings

4. **C4101/C4100/C4189 - Unused Variables**
   - Remove unused variables
   - Or use (void)param; to suppress
   - Impact: ~200+ warnings

### Low Priority (Build Configuration)

5. **MSB8012 - TargetPath Mismatch**
   - Fix project output paths
   - Ensure $(OutDir), $(TargetName) match linker settings
   - Impact: ~10 warnings

