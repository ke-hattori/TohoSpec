// NanoSpecTitle.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#define _MASTER_
#include "System.h"
#include "NanoSpecTitle.h"
#include "NanoSpecTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleApp

BEGIN_MESSAGE_MAP(CNanoSpecTitleApp, CWinApp)
	//{{AFX_MSG_MAP(CNanoSpecTitleApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleApp クラスの構築

CNanoSpecTitleApp::CNanoSpecTitleApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CNanoSpecTitleApp オブジェクト

CNanoSpecTitleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleApp クラスの初期化

BOOL CNanoSpecTitleApp::InitInstance()
{
	AfxEnableControlContainer();

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//	れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//	ください。

#ifdef _AFXDLL
	Enable3dControls(); 		// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif
/* added 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- { ---------- */
	// 二重起動の防止処理
	m_hMutex_Run = ::CreateMutex(NULL, TRUE, m_pszExeName);
	if ((m_hMutex_Run == NULL) || (::GetLastError() == ERROR_ALREADY_EXISTS)) {
		return	FALSE;
	}
/* added 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- } ---------- */

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
	g_lAppNameType = APP_NAME_NANO; // default application name
	for(int i = 0; i < APP_NAME_MAX; i++){
		if(_tcsncmp(m_pszExeName, g_lpszAppPrefix4[i], 4) == 0){
			g_lAppNameType = i;
			break;
		}
	}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

	//Saiki 20090525 Add ----->
	CString strBuffer;
	SYSTEM_CONFIG l_SystemConfig;
	///// NanoSpec.iniのグローバル変数の初期化 /////
	ConfigFile_LoadAllNanoSpecIni();
	//Saiki 20090723 Delete ----->
	//ConfigFile_SaveAllNanoSpecIni();
	//Saiki 20090723 Delete <-----
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	HANDLE m_hModule;
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
	#ifdef _DEBUG
	strBuffer.Format("dtns%sspecTitle", g_lpszAppPrefix4[g_lAppNameType]);
	#else
	strBuffer.Format("tns%sspecTitle", g_lpszAppPrefix4[g_lAppNameType]);
	#endif
	m_hModule = ResourceLoadLibrary(l_SystemConfig.nLanguage, strBuffer);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	AfxSetResourceHandle((HINSTANCE) m_hModule);
	//Saiki 20090525 Add <-----

	CNanoSpecTitleDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//		 記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <ｷｬﾝｾﾙ> で消された時のコードを
		//		 記述してください。
	}

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

/* added 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- { ---------- */
int CNanoSpecTitleApp::ExitInstance()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	// 二重起動の防止のための Mutex 開放
	if (m_hMutex_Run != NULL) {
		::ReleaseMutex(m_hMutex_Run);
		::CloseHandle(m_hMutex_Run);
	}

	return CWinApp::ExitInstance();
}
/* added 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- } ---------- */
