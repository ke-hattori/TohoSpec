#pragma once

BOOL LoadRecipe(LPVOID pVoid, LPCSTR lpszName, int iType);
BOOL SaveRecipe(LPCVOID pVoid, LPCSTR lpszName, int iType);
BOOL LoadMainRecipeList(MULTI_RCP_MAIN_RCP_LIST* pMainRcpList, WORD wNumMainRecipe, LPCSTR lpszName);
BOOL SaveMainRecipeList(const MULTI_RCP_MAIN_RCP_LIST* pMainRcpList, WORD wNumMainRecipe, LPCSTR lpszName);
BOOL LoadPointList(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName);
BOOL LoadPointList2(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath);		// 入力：ファイルパス用
// 2009.10.15 bagus Distance 追加 --{--
BOOL Load2PointList(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName);
BOOL Load2PointList2(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath);		// 入力：ファイルパス用
// 2009.10.15 bagus Distance 追加 --}--
BOOL SavePointList(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName);
// 2010.03.09 bagus Recipe Backup 修正 --{--
//BOOL SavePointList2(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath);		// 入力：ファイルパス用
BOOL SavePointList2(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath, BOOL bDoBackup = TRUE);		// 入力：ファイルパス用
// 2010.03.09 bagus Recipe Backup 修正 --}--
// 2009.10.15 bagus Distance 追加 --{--
BOOL Save2PointList(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName);
// 2010.03.09 bagus Recipe Backup 修正 --{--
//BOOL Save2PointList2(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath);		// 入力：ファイルパス用
BOOL Save2PointList2(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath, BOOL bDoBackup = TRUE);		// 入力：ファイルパス用
// 2010.03.09 bagus Recipe Backup 修正 --}--
// 2009.10.15 bagus Distance 追加 --}--
BOOL MakeRecipeFilePath(LPSTR pszFilePath, LPCSTR pszFileName, int iType);
BOOL DeleteRecipe(LPCSTR pszFileName, int iType);
BOOL ExistRecipe(LPCSTR pszFileName, SYSTEMTIME* lpLastWriteSystemTime, int iType);
BOOL SaveCurrRecipeInfo(LPCSTR pszMainRcpName, int = 0);

BOOL LoadHeadTypeAndScanType(int* piType, LPCSTR pszFilePathName);
BOOL LoadHeadTypeAndScanType2(int* piType, LPCSTR pszFilePath);
BOOL SaveHeadTypeAndScanType(int iType, LPCSTR pszFileName);
BOOL SaveHeadTypeAndScanType2(int iType, LPCSTR pszFilePath);						// 入力：ファイルパス用
void ClearMemory();
BOOL SaveCurrRecipeInfo(LPCSTR pszRcpName, LPCSTR pszFilePath, int iType);

//2009.12.10 bagus Recipe Backup --{--
BOOL SetBackupPath(LPCTSTR lpszBackupPath,BOOL bUse);
BOOL BackupRecipe(LPCTSTR lpszSourcePath);
//2009.12.25 bagus Recipe Backup --{--
BOOL DeleteBackupRecipe(LPCTSTR lpszSourcePath);
//2009.12.25 bagus Recipe Backup --}--
BOOL BackupAllRecipe();
BOOL RestoreAllRecipe();
//2009.12.10 bagus Recipe Backup --}--
//Saiki 20110209 Add ----->
BOOL SetStressParam(LPVOID pVoid, LPCSTR lpszName, int iType);
BOOL LoadStressParam(LPVOID pVoid, LPCSTR lpszName, int iType);
//Saiki 20110209 Add <-----