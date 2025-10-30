// AlarmIf.cpp : DLL 用の初期化処理の定義を行います。
//

#include "stdafx.h"
#include <afxdllx.h>

#define _ALARMIFDLL_

#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\AlarmIf.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE AlarmIfDLL = { NULL, NULL };

#pragma data_seg("Shared")		// ここはセクション名(任意)
HWND g_hWnd = 0;
#pragma data_seg()

#pragma comment(linker, "/Section:Shared,RWS") // リンカオプション設定

/////////////////////////////////////////////////////////////////////////////
// Variable                                                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//CAlarm* g_pAlarm;
//HWND g_hWnd = 0;

/////////////////////////////////////////////////////////////////////////////
// DLL                                                                     //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// DllMain
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
		if (FALSE == DllMutexCreate(_T("AlarmIf"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		TRACE0("ALARMIF.DLL Initializing!\n");

		// 拡張 DLL を１回だけ初期化します。
		if (!AfxInitExtensionModule(AlarmIfDLL, hInstance))
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

		new CDynLinkLibrary(AlarmIfDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ALARMIF.DLL Terminating!\n");
		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(AlarmIfDLL);
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
	}
	return 1;	// ok
}

/////////////////////////////////////////////////////////////////////////////
//
void __stdcall AlarmIf_Init(HWND hWnd)
{
	g_hWnd = hWnd;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL __stdcall AlarmIf_Set(int Id)
{
	SendMessage(g_hWnd, WM_ALARMIF_SET, Id, 0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL __stdcall AlarmIf_Reset(int Id)
{
	SendMessage(g_hWnd, WM_ALARMIF_RESET, Id, 0);

	return TRUE;
}
