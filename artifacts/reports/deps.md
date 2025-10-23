# TohoSpec Dependency Graph Report

## Solution Overview

**Solution:** NANOSPEC.sln
**Total Projects:** 37

## Project List

| # | Project Name | Path | References |
|---|--------------|------|------------|
| 1 | ACTIVE | SRC\Active\ACTIVE.vcxproj | 0 |
| 2 | AlarmIf | SRC\AlarmIf\AlarmIf.vcxproj | 0 |
| 3 | ConfigFile | SRC\ConfigFile\ConfigFile.vcxproj | 0 |
| 4 | DESKEW | SRC\Deskew\DESKEW.vcxproj | 0 |
| 5 | HOOK | SRC\Hook\HOOK.vcxproj | 0 |
| 6 | MEASYS | SRC\MeaSys\MEASYS.vcxproj | 0 |
| 7 | MEASYSRCENU | SRC\MeaSys\MEASYSRCENU\MEASYSRCENU.vcxproj | 0 |
| 8 | MEASYSRCJPN | SRC\MeaSys\MEASYSRCJPN\MEASYSRCJPN.vcxproj | 0 |
| 9 | MOTTSK | SRC\MotTsk\MOTTSK.vcxproj | 0 |
| 10 | MotSysCfg | SRC\MotSysCfgN\MotSysCfg.vcxproj | 0 |
| 11 | MotSysG | SRC\MotSysNSPT\MotSysG.vcxproj | 0 |
| 12 | NANOSPECRCENU | SRC\NanoSpec\NANOSPECRCENU\NANOSPECRCENU.vcxproj | 0 |
| 13 | NANOSPECRCJPN | SRC\NanoSpec\NANOSPECRCJPN\NANOSPECRCJPN.vcxproj | 0 |
| 14 | NANOSPECTITLERCENU | SRC\Title\NANOSPECTITLERCENU\NANOSPECTITLERCENU.vcxproj | 0 |
| 15 | NANOSPECTITLERCJPN | SRC\Title\NANOSPECTITLERCJPN\NANOSPECTITLERCJPN.vcxproj | 0 |
| 16 | NEXAX | SRC\NEXAX\NEXAX.vcxproj | 0 |
| 17 | NEXIF | SRC\NEXIF\NEXIF.vcxproj | 0 |
| 18 | NEXINI | SRC\NEXINI\NEXINI.vcxproj | 0 |
| 19 | NEXIO | SRC\NEXIO\NEXIO.vcxproj | 0 |
| 20 | NEXPROC | SRC\NEXPROC\NEXPROC.vcxproj | 0 |
| 21 | NEXRCENU | SRC\NEXRCENU\NEXRCENU.vcxproj | 0 |
| 22 | NEXRCJPN | SRC\NEXRCJPN\NEXRCJPN.vcxproj | 0 |
| 23 | NEXTRA | SRC\NEXTRA\NEXTRA.vcxproj | 0 |
| 24 | NEXUI | SRC\NEXUI\NEXUI.vcxproj | 0 |
| 25 | NSSTAGERCENU | SRC\NSStage\NSSTAGERCENU\NSSTAGERCENU.vcxproj | 0 |
| 26 | NSSTAGERCJPN | SRC\NSStage\NSSTAGERCJPN\NSSTAGERCJPN.vcxproj | 0 |
| 27 | NSStage | SRC\NSStage\NSStage.vcxproj | 0 |
| 28 | NanoSpec | SRC\NanoSpec\NanoSpec.vcxproj | 0 |
| 29 | NanoSpecTitle | SRC\Title\NanoSpecTitle.vcxproj | 0 |
| 30 | OVERLAY | SRC\OVERLAY\OVERLAY.vcxproj | 0 |
| 31 | PATREC | SRC\PatRec\PATREC.vcxproj | 0 |
| 32 | PORT_NT | SRC\Port_NT\PORT_NT.vcxproj | 0 |
| 33 | PatternFile | SRC\PatternFile\PatternFile.vcxproj | 0 |
| 34 | PifComm | SRC\PifComm\PifComm.vcxproj | 0 |
| 35 | RecipeFile | SRC\RecipeFile\RecipeFile.vcxproj | 0 |
| 36 | STAGE | SRC\Stage\STAGE.vcxproj | 0 |
| 37 | UTILITY | SRC\Utility\UTILITY.vcxproj | 0 |

## Dependency Details

## Build Order Analysis

Projects are built in dependency order. Projects with no dependencies build first.

**Leaf Projects (no dependencies):** 37

- ACTIVE
- AlarmIf
- ConfigFile
- DESKEW
- HOOK
- MEASYS
- MEASYSRCENU
- MEASYSRCJPN
- MOTTSK
- MotSysCfg
- MotSysG
- NANOSPECRCENU
- NANOSPECRCJPN
- NANOSPECTITLERCENU
- NANOSPECTITLERCJPN
- NEXAX
- NEXIF
- NEXINI
- NEXIO
- NEXPROC
- NEXRCENU
- NEXRCJPN
- NEXTRA
- NEXUI
- NSSTAGERCENU
- NSSTAGERCJPN
- NSStage
- NanoSpec
- NanoSpecTitle
- OVERLAY
- PATREC
- PORT_NT
- PatternFile
- PifComm
- RecipeFile
- STAGE
- UTILITY

**Projects with Most Dependencies (Top 10):**


## Link Order Considerations

### Static vs Dynamic Linking

- **Static MFC Projects:** Link MFC statically, use static CRT
- **Dynamic MFC Projects:** Link MFC dynamically, use DLL CRT
- **Non-MFC Projects:** Use static CRT by default

### Potential Issues

1. **Circular Dependencies:** None detected (MSBuild would fail)
2. **Link Order:** MSBuild handles automatically via ProjectReference
3. **Duplicate Symbols:** Resolved (CMemDC namespace isolation)

## Recommendations

1. **Maintain ProjectReference:** Keep using ProjectReference instead of manual .lib links
2. **Avoid Circular Dependencies:** Design interfaces to break cycles if they appear
3. **Consistent CRT Usage:** Ensure all projects in a dependency chain use compatible CRT
4. **Build Order:** Use /m:1 for deterministic builds during development

