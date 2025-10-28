# Release|Win32 Smoke Test Checklist

## Overview

**Purpose:** Validate that Release|Win32 binaries function correctly after SAFESEH fixes and build stabilization.  
**Configuration:** Release|Win32 (v143 toolset)  
**Date:** 2025-10-22

## Test Environment

- **OS:** Windows 10/11 (x86 or x64)
- **Build:** Release|Win32
- **Binaries Location:** `bin/` directory
- **Test Data:** Sample measurement files (if available)

## Pre-Test Checklist

- [ ] Verify all binaries built successfully (0 errors)
- [ ] Check that main executable exists: `bin/NanoSpec.exe`
- [ ] Verify DLLs are present in `dll/` directory
- [ ] Ensure no Debug artifacts are mixed with Release build

## Smoke Test Cases

### 1. Application Launch

**Objective:** Verify the main application launches without crashes

- [ ] **Test 1.1:** Launch `bin/NanoSpec.exe`
  - **Expected:** Application window appears
  - **Expected:** No crash dialogs or error messages
  - **Expected:** Main menu and toolbar visible
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

- [ ] **Test 1.2:** Check application title and version
  - **Expected:** Correct application name in title bar
  - **Expected:** About dialog shows correct version
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

### 2. Main Window Functionality

**Objective:** Verify basic UI interactions work correctly

- [ ] **Test 2.1:** Open main menu items
  - **Expected:** File, Edit, View, Tools, Help menus open without errors
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

- [ ] **Test 2.2:** Toolbar buttons respond
  - **Expected:** Clicking toolbar buttons triggers expected actions
  - **Expected:** No crashes or hangs
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

### 3. Key Dialog Windows

**Objective:** Verify critical dialogs open and close properly

- [ ] **Test 3.1:** Open Configuration/Settings dialog
  - **Action:** File → Settings (or equivalent)
  - **Expected:** Settings dialog opens
  - **Expected:** Can navigate tabs/sections
  - **Expected:** Can close dialog without crash
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

- [ ] **Test 3.2:** Open Measurement dialog
  - **Action:** Open measurement configuration
  - **Expected:** Dialog opens with controls visible
  - **Expected:** Can interact with controls
  - **Expected:** Can close dialog
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

- [ ] **Test 3.3:** Open Stage Program dialog
  - **Action:** Open stage program configuration
  - **Expected:** Dialog opens correctly
  - **Expected:** Grid/list controls render properly
  - **Expected:** Can close dialog
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

### 4. File Operations

**Objective:** Verify file I/O operations work correctly

- [ ] **Test 4.1:** Open existing file
  - **Action:** File → Open → Select sample file
  - **Expected:** File loads without errors
  - **Expected:** Data displays correctly
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

- [ ] **Test 4.2:** Save file
  - **Action:** File → Save or Save As
  - **Expected:** File saves successfully
  - **Expected:** No error dialogs
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

- [ ] **Test 4.3:** Recent files list
  - **Action:** File → Recent Files
  - **Expected:** Recent files list populates
  - **Expected:** Can open file from recent list
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

### 5. Hardware Integration (If Available)

**Objective:** Verify hardware communication works (if test hardware available)

- [ ] **Test 5.1:** Connect to measurement hardware
  - **Action:** Initialize hardware connection
  - **Expected:** Connection succeeds or shows appropriate error
  - **Expected:** No crashes
  - **Result:** ⬜ PASS / ⬜ FAIL / ⬜ N/A
  - **Notes:** _______________________________________

- [ ] **Test 5.2:** Stage control (if applicable)
  - **Action:** Send stage movement command
  - **Expected:** Stage responds or shows appropriate error
  - **Expected:** No crashes
  - **Result:** ⬜ PASS / ⬜ FAIL / ⬜ N/A
  - **Notes:** _______________________________________

### 6. Application Exit

**Objective:** Verify clean shutdown

- [ ] **Test 6.1:** Close application via File → Exit
  - **Expected:** Application closes cleanly
  - **Expected:** No crash dialogs
  - **Expected:** No orphaned processes
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

- [ ] **Test 6.2:** Close application via window X button
  - **Expected:** Application closes cleanly
  - **Expected:** Prompts to save if needed
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

## Memory and Stability Tests (Optional)

- [ ] **Test 7.1:** Run application for 5 minutes
  - **Expected:** No memory leaks (check Task Manager)
  - **Expected:** No crashes or hangs
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

- [ ] **Test 7.2:** Open/close dialogs 10 times
  - **Expected:** No degradation in performance
  - **Expected:** No crashes
  - **Result:** ⬜ PASS / ⬜ FAIL
  - **Notes:** _______________________________________

## Known Issues / Limitations

- **SAFESEH Disabled:** MotSysG and NEXIO projects have SAFESEH disabled due to apidio.lib compatibility. This is expected and does not affect functionality.
- **Warnings:** 867 warnings present (mostly type conversion warnings). These do not affect Release build functionality.

## Test Results Summary

**Date Tested:** _______________  
**Tester:** _______________  
**Build Version:** _______________

**Overall Result:** ⬜ PASS / ⬜ FAIL

**Total Tests:** ___ / ___  
**Passed:** ___  
**Failed:** ___  
**Skipped/N/A:** ___

## Critical Issues Found

_List any critical issues that would block release:_

1. _______________________________________
2. _______________________________________
3. _______________________________________

## Non-Critical Issues Found

_List any minor issues or observations:_

1. _______________________________________
2. _______________________________________
3. _______________________________________

## Recommendations

- [ ] Release build is stable and ready for deployment
- [ ] Release build needs fixes before deployment
- [ ] Additional testing required in areas: _______________

## Sign-Off

**Tested By:** _______________  
**Date:** _______________  
**Signature:** _______________

---

## Notes for Automated Testing

If implementing automated smoke tests, consider:

1. **UI Automation:** Use Windows UI Automation or similar framework
2. **Process Monitoring:** Monitor for crashes, hangs, memory leaks
3. **Log Analysis:** Parse application logs for errors/warnings
4. **Performance Metrics:** Track startup time, dialog open times
5. **Regression Detection:** Compare against baseline metrics
