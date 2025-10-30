// PatternFile.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include <afxdllx.h>

#define	_PATTERNFILEDLL_
#include "..\\..\\INC\\PatternFile.hxx"

#include "Pattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static CRITICAL_SECTION criticalSection;

static AFX_EXTENSION_MODULE PatternFileDLL = { NULL, NULL };

/* added 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- { ---------- */
TCHAR g_tszProcDir[_MAX_PATH];			/* 呼出しプロセスのディレクトリ('\'付き)*/
TCHAR g_tszBaseDir[_MAX_PATH];			/* 基準ディレクトリ('\'付き)*/
/* added 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- } ---------- */

/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("PatternFile"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		TRACE0("PATTERNFILE.DLL Initializing!\n");

		// 拡張 DLL を１回だけ初期化します。
		if (!AfxInitExtensionModule(PatternFileDLL, hInstance))
			return 0;

		// この DLL をリソース チェインへ挿入します。
		// メモ: 拡張 DLL が MFC アプリケーションではなく
		//	 MFC 標準 DLL (ActiveX コントロールのような)
		//	 に暗黙的にリンクされる場合、この行を DllMain
		//	 から削除して、この拡張 DLL からエクスポート
		//	 された別の関数内へ追加してください。
		//	 この拡張 DLL を使用する標準 DLL はこの拡張 DLL
		//	 を初期化するために明示的にその関数を呼び出します。
		//	 それ以外の場合は、CDynLinkLibrary オブジェクトは
		//	 標準 DLL のリソース チェインへアタッチされず、
		//	 その結果重大な問題となります。

/* added 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- { ---------- */
		TCHAR l_tszProcessFName[_MAX_PATH];	/* 呼出しプロセスのフルパス*/
		::GetModuleFileName(0, l_tszProcessFName, sizeof(l_tszProcessFName));
		TCHAR l_tszDrive[_MAX_DRIVE];
		TCHAR l_tszDir[_MAX_DIR];
		_tsplitpath(l_tszProcessFName, l_tszDrive, l_tszDir, 0, 0);
		_stprintf(g_tszProcDir, _T("%s%s"), l_tszDrive, l_tszDir);
		if (0 != _tcslen(g_tszProcDir)) {
			if (_T('\\') != g_tszProcDir[_tcslen(g_tszProcDir) - 1]) {
				_tcscat(g_tszProcDir, _T("\\"));
			}
		}
		TCHAR l_tszProcDirUp[_MAX_PATH];
		_stprintf(l_tszProcDirUp, _T("%s.."), g_tszProcDir);
		_tfullpath(g_tszBaseDir, l_tszProcDirUp, sizeof(g_tszBaseDir));
		if ((0 != _tcslen(g_tszBaseDir)) && (_T('\\') != g_tszBaseDir[_tcslen(g_tszBaseDir) - 1])) {
			_tcscat(g_tszBaseDir, _T("\\"));
		}
/* added 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- } ---------- */

		new CDynLinkLibrary(PatternFileDLL);

		::InitializeCriticalSection(&criticalSection);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("PATTERNFILE.DLL Terminating!\n");
		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(PatternFileDLL);
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
	}
	return 1;	// ok
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : PatternFile_Lock
// Purpose	  : PatternFileの排他制御（ロック）
void PATTERNFILEAPI PatternFile_Lock()
{
	::EnterCriticalSection(&criticalSection);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : PatternFile_Unlock
// Purpose	  : PatternFileの排他制御（アンロック）
void PATTERNFILEAPI PatternFile_Unlock()
{
	::LeaveCriticalSection(&criticalSection);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : PatternFile_LoadPointList
// Purpose	  : PatternFileのパターンリストの読み込み
// Parameters : pPoint		---> SITE_PATTERNの配列
//				pNumScans	---> 読み込む測定ポイント数
//				pszFileName ---> ファイル名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL PATTERNFILEAPI PatternFile_LoadPointList(SITE_PATTERN* pPoint, WORD* pNumScans,
	LPCSTR pszFileName)
{
	BOOL bRet;
	PatternFile_Lock();
	bRet = LoadPointList(pPoint, pNumScans, pszFileName);
	PatternFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : PatternFile_SavePointList
// Purpose	  : PatternFileのパターンリストの保存
// Parameters : pPoint		---> SITE_PATTERNの配列
//				wNumScans	---> 保存する測定ポイント数
//				pszFilePath ---> ファイル名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL PATTERNFILEAPI PatternFile_SavePointList(const SITE_PATTERN* pPoint, WORD wNumScans,
	LPCSTR pszFilePath)
{
	BOOL bRet;
	PatternFile_Lock();
	bRet = SavePointList(pPoint, wNumScans, pszFilePath);
	PatternFile_Unlock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : PatternFile_MakePatternFilePath
// Purpose	  : Patternファイルのファイルパス名を生成
// Parameters : lpszName ---> ファイル名
//
void PATTERNFILEAPI PatternFile_MakePatternFilePath(LPSTR pszFilePath, LPCSTR pszFileName)
{
	MakePatternFilePath(pszFilePath, pszFileName);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : PatternFile_MakePatternImageFilePath
// Purpose	  : Patternイメージファイルのファイルパス名を生成
// Parameters : lpszName ---> ファイル名
//
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ---------- { ---------- */
//void PATTERNFILEAPI PatternFile_MakePatternImageFilePath(LPSTR pszFilePath, LPCSTR pszFileName, int iLens)
//{
//	MakePatternImageFilePath(pszFilePath, pszFileName, iLens);
//}
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ----------			  */
void PATTERNFILEAPI PatternFile_MakePatternImageFilePath(LPTSTR ptszFilePath, LPCTSTR ptszFileName)
{
	MakePatternImageFilePath(ptszFilePath, ptszFileName);
}
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
// Name 	  : PatternFile_GetPatternFileInfo
// Purpose	  : Patternイメージファイルのファイルパス名とレンズ倍率を生成
// Parameters : pszFilePath ---> ファイル名
//
void PATTERNFILEAPI PatternFile_GetPatternFileInfo(LPCSTR pszFilePath, LPSTR pszFileName, int* iLens)
{
	GetPatternFileInfo(pszFilePath, pszFileName, iLens);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : PatternFile_ExistPatternFilePath
// Purpose	  : ファイルパス名が存在するかを確認
// Parameters : pszFilePath ---> ファイルパス名
//				lpLastWriteSystemTime ---> ファイルの最終更新日時
//
// Returns	  : TRUE  ---> 存在する
//				FALSE ---> 存在しない
BOOL PATTERNFILEAPI PatternFile_ExistPatternFilePath(LPCSTR pszFilePath, SYSTEMTIME* lpLastWriteSystemTime)
{
	return ExistPatternFilePath(pszFilePath, lpLastWriteSystemTime);
}

/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- { ---------- */
/////////////////////////////////////////////////////////////////////////////
/* Name 	  : PatternFile_SetSubInfo
// Purpose	  : SPR パターン情報をファイルに書込みます．
// Parameters : LPCTSTR ptszPattern			---> パターン名
//				LPCSPR_SUB_INFO pPatSubInfo	---> サブ情報
//
// Returns	  : TRUE  ---> 書込み成功
//				FALSE ---> 書込み失敗									*/
BOOL PATTERNFILEAPI PatternFile_SetSubInfo(LPCTSTR ptszPatName, LPCSPR_SUB_INFO pPatSubInfo)
{
	return SetSubInfo(ptszPatName, pPatSubInfo);
}
/////////////////////////////////////////////////////////////////////////////
/* Name 	  : PatternFile_GetSubInfo
// Purpose	  : SPR パターン情報をファイルに書込みます．
// Parameters : LPCTSTR ptszPattern		---> パターン名
//				LPCSPR_SUB_INFO pPatSubInfo	---> サブ情報
//
// Returns	  : TRUE  ---> 読込み成功
//				FALSE ---> 読込み失敗									*/
BOOL PATTERNFILEAPI PatternFile_GetSubInfo(LPCTSTR ptszPatName, LPSPR_SUB_INFO pPatSubInfo)
{
	return GetSubInfo(ptszPatName, pPatSubInfo);
}
/////////////////////////////////////////////////////////////////////////////
/* Name 	  : PatternFile_RemoveSubInfo
// Purpose	  : SPR パターン情報ファイルを削除します．
// Parameters : LPCTSTR ptszPattern ---> パターン名
//
// Returns	  : TRUE  ---> 削除成功
//				FALSE ---> 削除失敗									*/
BOOL PATTERNFILEAPI PatternFile_RemoveSubInfo(LPCTSTR ptszPatName)
{
	return RemoveSubInfo(ptszPatName);
}
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- } ---------- */
// 2009.12.10 bagus Recipe Backup --{--
BOOL PATTERNFILEAPI PatternFile_SetBackupPath(LPCSTR lpszBackupPath,BOOL bUse)
{
	BOOL bRet;
	PatternFile_Lock();
	bRet = SetBackupPath(lpszBackupPath,bUse);
	PatternFile_Unlock();
	return bRet;
}
// 2009.12.10 bagus Recipe Backup --}--
