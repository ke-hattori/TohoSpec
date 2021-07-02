// Pattern.h
//
//////////////////////////////////////////////////////////////////////
#pragma once

BOOL LoadPointList(SITE_PATTERN* pPoint, WORD* wNumScans, LPCSTR pszFileName);
BOOL LoadPointList2(SITE_PATTERN* pPoint, WORD* wNumScans, LPCSTR pszFilePath);		// 入力：ファイルパス用
BOOL SavePointList(const SITE_PATTERN* pPoint, WORD wNumScans, LPCSTR pszFileName);
BOOL SavePointList2(const SITE_PATTERN* pPoint, WORD wNumScans, LPCSTR pszFilePath);		// 入力：ファイルパス用

void MakePatternFilePath(LPSTR pszFilePath, LPCSTR pszFileName);
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ---------- { ---------- */
//void MakePatternImageFilePath(LPSTR pszFilePath, LPCSTR pszFileName, int iLens);
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ----------			  */
void MakePatternImageFilePath(LPTSTR ptszFilePath, LPCTSTR ptszFileName);
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ---------- } ---------- */
void GetPatternFileInfo(LPCSTR pszFilePath, LPSTR pszFileName, int* iLens);
BOOL ExistPatternFilePath(LPCSTR pszFilePath, SYSTEMTIME* lpLastWriteSystemTime);
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- { ---------- */
BOOL SetSubInfo(LPCTSTR ptszPatName, LPCSPR_SUB_INFO pPatSubInfo);
BOOL GetSubInfo(LPCTSTR ptszPatName, LPSPR_SUB_INFO pPatSubInfo);
BOOL RemoveSubInfo(LPCTSTR ptszPatName);
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- } ---------- */

/* added 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- { ---------- */
extern TCHAR g_tszProcDir[_MAX_PATH];			/* 呼出しプロセスのディレクトリ('\'付き)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];			/* 基準ディレクトリ('\'付き)*/
/* added 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- } ---------- */

//2009.12.10 bagus Recipe Backup --{--
BOOL SetBackupPath(LPCTSTR lpszBackupPath,BOOL bUse);
BOOL BackupRecipe(LPCTSTR lpszSourcePath);
BOOL DeleteBackupRecipe(LPCTSTR lpszSourcePath);
//2009.12.10 bagus Recipe Backup --}--
