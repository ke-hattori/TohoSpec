// RecipeFile.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#define	_RECIPEFILEDLL_
#include "..\\..\\INC\\RecipeFile.hxx"

#include "Recipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
TCHAR g_tszProcDir[_MAX_PATH] = _T("");		/* 呼出しプロセスのディレクトリ('\'付き)*/
TCHAR g_tszBaseDir[_MAX_PATH] = _T("");		/* 基準ディレクトリ('\'付き)*/
void GetProcBaseDir(
		LPTSTR ptszProcDir,
		LPTSTR ptszBaseDir
	)
{
	if (0 == _tcscmp(ptszProcDir, _T(""))) {
		TCHAR l_tszProcessFName[_MAX_PATH];	/* 呼出しプロセスのフルパス*/
		::GetModuleFileName(0, l_tszProcessFName, sizeof(l_tszProcessFName));
		TCHAR l_tszDrive[_MAX_DRIVE];
		TCHAR l_tszDir[_MAX_DIR];
		_tsplitpath(l_tszProcessFName, l_tszDrive, l_tszDir, 0, 0);
		_stprintf(ptszProcDir, _T("%s%s"), l_tszDrive, l_tszDir);
		if (0 != _tcslen(ptszProcDir)) {
			if (_T('\\') != ptszProcDir[_tcslen(ptszProcDir) - 1]) {
				_tcscat(ptszProcDir, _T("\\"));
			}
		}
	}
	if ((0 != _tcscmp(ptszProcDir, _T("")))
	 && (0 == _tcscmp(ptszBaseDir, _T("")))) {
		TCHAR l_tszProcDirUp[_MAX_PATH];
		_stprintf(l_tszProcDirUp, _T("%s.."), ptszProcDir);
		_tfullpath(ptszBaseDir, l_tszProcDirUp, sizeof(g_tszBaseDir));
		if ((0 != _tcslen(ptszBaseDir)) && (_T('\\') != ptszBaseDir[_tcslen(ptszBaseDir) - 1])) {
			_tcscat(ptszBaseDir, _T("\\"));
		}
	}
}
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */

static CRITICAL_SECTION criticalSection;

static AFX_EXTENSION_MODULE RecipeFileDLL = { NULL, NULL };

// --------------------------------------------------------------------
// DllMain
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("RecipeFile"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		TRACE0("RECIPEFILE.DLL Initializing!\n");

		if (!AfxInitExtensionModule(RecipeFileDLL, hInstance))
			return 0;

/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
		GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */
		new CDynLinkLibrary(RecipeFileDLL);
//2009.12.10 bagus Recipe Backup --{--
		SetBackupPath("",FALSE);
//2009.12.10 bagus Recipe Backup --}--

		::InitializeCriticalSection(&criticalSection);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("RECIPEFILE.DLL Terminating!\n");
		AfxTermExtensionModule(RecipeFileDLL);

		::EnterCriticalSection(&criticalSection);
		::DeleteCriticalSection(&criticalSection);
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_Lock
// Purpose	  : Recipeの排他制御（ロック）
void RECIPEFILEAPI RecipeFile_Lock()
{
	::EnterCriticalSection(&criticalSection);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_Unlock
// Purpose	  : Recipeの排他制御（アンロック）
void RECIPEFILEAPI RecipeFile_Unlock()
{
	::LeaveCriticalSection(&criticalSection);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_LoadRecipe
// Purpose	  : Recipe，Program，UserAccount，Sampleの読み込み
// Parameters : pVoid	 ---> データを保存する構造体のポインタ
//				lpszName ---> ファイル名
//				iType	 ---> どのファイル(MainRecipe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL RECIPEFILEAPI RecipeFile_LoadRecipe(LPVOID pVoid, LPCSTR lpszName, int iType)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = LoadRecipe(pVoid, lpszName, iType);
	RecipeFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_SaveRecipe
// Purpose	  : Recipe，Program，UserAccount，Sampleの保存
// Parameters : pVoid	 ---> データを保存する構造体のポインタ
//				lpszName ---> ファイル名
//				iType	 ---> どのファイル(MainRecipe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL RECIPEFILEAPI RecipeFile_SaveRecipe(LPCVOID pVoid, LPCSTR lpszName, int iType)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = SaveRecipe(pVoid, lpszName, iType);
	RecipeFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_LoadMainRecipeList
// Purpose	  : MultiRecipeのMainRecipeListの読み込み
// Parameters : pMainRcpList   ---> MainRecipeListの配列
//				wNumMainRecipe ---> 読み込むMainRecipe数
//				lpszName	   ---> ファイル名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL RECIPEFILEAPI RecipeFile_LoadMainRecipeList(MULTI_RCP_MAIN_RCP_LIST* pMainRcpList,
	WORD wNumMainRecipe, LPCSTR lpszName)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = LoadMainRecipeList(pMainRcpList, wNumMainRecipe, lpszName);
	RecipeFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_SaveMainRecipeList
// Purpose	  : MultiRecipeのMainRecipeListの保存
// Parameters : pMainRcpList   ---> MainRecipeListの配列
//				wNumMainRecipe ---> 保存するMainRecipe数
//				lpszName	   ---> ファイル名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL RECIPEFILEAPI RecipeFile_SaveMainRecipeList(const MULTI_RCP_MAIN_RCP_LIST* pMainRcpList,
	WORD wNumMainRecipe, LPCSTR lpszName)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = SaveMainRecipeList(pMainRcpList, wNumMainRecipe, lpszName);
	RecipeFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_LoadPointList
// Purpose	  : StageProgramの測定ポイントリストの読み込み
// Parameters : pPoint		---> STAGE_COORDの配列
//				wNumScans	---> 読み込む測定ポイント数
//				pszFileName ---> ファイル名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL RECIPEFILEAPI RecipeFile_LoadPointList(STAGE_COORD* pPoint, WORD wNumScans,
	LPCSTR pszFileName)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = LoadPointList(pPoint, wNumScans, pszFileName);
	RecipeFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_SavePointList
// Purpose	  : StageProgramの測定ポイントリストの保存
// Parameters : pPoint		---> STAGE_COORDの配列
//				wNumScans	---> 保存する測定ポイント数
//				pszFilePath ---> ファイル名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL RECIPEFILEAPI RecipeFile_SavePointList(const STAGE_COORD* pPoint, WORD wNumScans,
	LPCSTR pszFilePath)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = SavePointList(pPoint, wNumScans, pszFilePath);
	RecipeFile_Unlock();
	return bRet;
}

// 2009.10.15 bagus Distance 追加 --{--
BOOL RECIPEFILEAPI RecipeFile_Load2PointList(STAGE_COORD* pPoint, WORD wNumScans,
	LPCSTR pszFileName)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = Load2PointList(pPoint, wNumScans, pszFileName);
	RecipeFile_Unlock();
	return bRet;
}

BOOL RECIPEFILEAPI RecipeFile_Save2PointList(const STAGE_COORD* pPoint, WORD wNumScans,
	LPCSTR pszFilePath)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = Save2PointList(pPoint, wNumScans, pszFilePath);
	RecipeFile_Unlock();
	return bRet;
}
// 2009.10.15 bagus Distance 追加 --}--

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_SaveCurrRecipeInfo
// Purpose	  : 現在のRecipeデータの保存
// Parameters : pszMainRcpName ---> メインレシピ名
//			  : iMeasType(デフォルト引数) ---> 測定タイプ(マニュアル測定かそうでないかを識別するためのもの) (デフォルト値は0)
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL RECIPEFILEAPI RecipeFile_SaveCurrRecipeInfo(LPCSTR pszMainRcpName, int iMeasType)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = SaveCurrRecipeInfo(pszMainRcpName, iMeasType);
	RecipeFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_DeleteRecipe
// Purpose	  : Recipe，Program，UserAccount，Sampleの削除
// Parameters : lpszName ---> ファイル名
//				iType	 ---> どのファイル(MainReicpe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 削除成功
//				FALSE ---> 削除失敗
BOOL RECIPEFILEAPI RecipeFile_DeleteRecipe(LPCSTR pszFileName, int iType)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = DeleteRecipe(pszFileName, iType);
	RecipeFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_ExistRecipe
// Purpose	  : Recipe，Program，UserAccount，Sampleが存在するかを確認
// Parameters : lpszName ---> ファイル名
//				lpLastWriteSystemTime ---> ファイルの最終更新日時
//				iType	 ---> どのファイル(MainRecipe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 存在する
//				FALSE ---> 存在しない
BOOL RECIPEFILEAPI RecipeFile_ExistRecipe(LPCSTR pszFileName, SYSTEMTIME* lpLastWriteSystemTime, int iType)
{
	return ExistRecipe(pszFileName, lpLastWriteSystemTime, iType);
}

//2009.12.10 bagus Recipe Backup --{--
/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_SetBackupPath
// Purpose	  : オートBackupするための設定をセットする
// Parameters : lpszBackupPath ---> バックアップ用パス
//				bUse 		   ---> 有効/無効
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL RECIPEFILEAPI RecipeFile_SetBackupPath(LPCSTR lpszBackupPath,BOOL bUse)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = SetBackupPath(lpszBackupPath,bUse);
	RecipeFile_Unlock();
	return bRet;
}
BOOL RECIPEFILEAPI RecipeFile_BackupAllRecipe()
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = BackupAllRecipe();
	RecipeFile_Unlock();
	return bRet;
}
BOOL RECIPEFILEAPI RecipeFile_RestoreAllRecipe()
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = RestoreAllRecipe();
	RecipeFile_Unlock();
	return bRet;
}
//2009.12.10 bagus Recipe Backup --}--

//Saiki 20110208 Add ----->
/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_SetStressParam
// Purpose	  : 応力測定用の設定をセットする
// Parameters : pVoid	 ---> データを保存する構造体のポインタ
//				lpszName ---> ファイル名
//				iType	 ---> どのファイル(MainRecipe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL RECIPEFILEAPI RecipeFile_SetStressParam(LPVOID pVoid, LPCSTR lpszName, int iType)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = SetStressParam(pVoid, lpszName, iType);
	RecipeFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : RecipeFile_LoadStressParam
// Purpose	  : 応力測定用の設定をセットする
// Parameters : pVoid	 ---> データを保存する構造体のポインタ
//				lpszName ---> ファイル名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL RECIPEFILEAPI RecipeFile_LoadStressParam(LPVOID pVoid, LPCSTR lpszName, int iType)
{
	BOOL bRet;
	RecipeFile_Lock();
	bRet = LoadStressParam(pVoid, lpszName, iType);
	RecipeFile_Unlock();
	return bRet;
}
//Saiki 20110208 Add <-----