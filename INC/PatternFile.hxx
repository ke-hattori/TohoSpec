#pragma once

#include <Globals.hxx>

// dllimport / dllexport
#if !defined(_PATTERNFILEDLL_)
#define PATTERNFILEAPI __declspec(dllimport)
#else
#define PATTERNFILEAPI __declspec(dllexport)
#endif

extern "C" void PATTERNFILEAPI PatternFile_Lock();
extern "C" void PATTERNFILEAPI PatternFile_Unlock();

extern "C" BOOL PATTERNFILEAPI PatternFile_LoadPointList(SITE_PATTERN* pPoint, WORD* wNumScans, LPCSTR pszFileName);
extern "C" BOOL PATTERNFILEAPI PatternFile_SavePointList(const SITE_PATTERN* pPoint, WORD wNumScans, LPCSTR pszFileName);

extern "C" void PATTERNFILEAPI PatternFile_MakePatternFilePath(LPSTR pszFilePath, LPCSTR pszFileName);
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ---------- { ---------- */
//extern "C" void PATTERNFILEAPI PatternFile_MakePatternImageFilePath(LPSTR pszFilePath, LPCSTR pszFileName, int iLens);
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ----------			  */
extern "C" void PATTERNFILEAPI PatternFile_MakePatternImageFilePath(LPTSTR ptszFilePath, LPCTSTR ptszFileName);
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ---------- } ---------- */

extern "C" void PATTERNFILEAPI PatternFile_GetPatternFileInfo(LPCSTR pszFilePath, LPSTR pszFileName, int* iLens);

extern "C" BOOL PATTERNFILEAPI PatternFile_ExistPatternFilePath(LPCSTR pszFilePath, SYSTEMTIME* lpLastWriteSystemTime);

/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- { ---------- */
extern "C" BOOL PATTERNFILEAPI PatternFile_SetSubInfo(LPCTSTR ptszPatName, LPCSPR_SUB_INFO pPatSubInfo);
extern "C" BOOL PATTERNFILEAPI PatternFile_GetSubInfo(LPCTSTR ptszPatName, LPSPR_SUB_INFO pPatSubInfo);
extern "C" BOOL PATTERNFILEAPI PatternFile_RemoveSubInfo(LPCTSTR ptszPatName);
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- } ---------- */
// 2009.12.10 bagus Recipe backup --{--
extern "C" BOOL PATTERNFILEAPI PatternFile_SetBackupPath(LPCSTR lpszBackupPath,BOOL bUse);
// 2009.12.10 bagus Recipe backup --}--
