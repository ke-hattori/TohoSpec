#include "stdafx.h"
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/DdeClient.hxx"
#include "resource.h"
#include "System.h"
#include "Mojiretsu.h"
#include "Xmp.h"

// XMP用
#define DDE_SERVICE 			 		_T("jaw_adap")
#define DDE_TOPIC				 		_T("dde")
#define DDE_ITEM				 		_T("results")
#define ADAPTIMEOUT 					(180)
#define ADAP_EXE_PATH			 		_T("C:\\WVASE32\\Adap\\jaw_adap.exe")

extern CMojiretsu* pMojiretsu;

// 2014.03.08 bagus SCOUT modified -->
static BOOL g_bScoutAppExist;
static char g_szWindowTitle[MAX_PATH];

static char g_szMsgBoxCaption[256];

// --------------------------------------------------------------------
// EnumWindowsProc
static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	char szWindowName[MAX_PATH];
	char szClassName[MAX_PATH];
	const char* pszName = g_szWindowTitle;
	char* pszWindowName;
	char* pszClassName;

	GetWindowText(hWnd, szWindowName, MAX_PATH);
	GetClassName(hWnd, szClassName, MAX_PATH);

	pszWindowName = strstr(szWindowName, pszName);
	if(pszWindowName == NULL){
		return TRUE;
	}

	pszClassName = strstr(szClassName, pszName);
	if(pszClassName == NULL){
		return TRUE;
	}

	// found SCOUT App.
	g_bScoutAppExist = TRUE;

	return FALSE;
}
// 2014.03.08 bagus SCOUT modified <--

// --------------------------------------------------------------------
// CXmp
CXmp::CXmp()
{
}

// --------------------------------------------------------------------
// ~CXmp
CXmp::~CXmp()
{
}

// --------------------------------------------------------------------
// InitInstance
BOOL CXmp::InitInstance()
{
	TRACE(_T("CXmp::InitInstance()\n"));

	strcpy(g_szMsgBoxCaption, "NanoSpec");
	if(g_lAppNameType != APP_NAME_NANO){
		strncpy(g_szMsgBoxCaption, g_lpszAppPrefix4[g_lAppNameType], 4);
	}

	if ( !RunXmp() )
		return FALSE;

	return TRUE;
}

// --------------------------------------------------------------------
// ExitInstance
void CXmp::ExitInstance()
{
	TRACE(_T("CXmp::ExitInstance()\n"));

	ShutDownXmp();
}

// --------------------------------------------------------------------
// RunXmp
BOOL CXmp::RunXmp()
{
	TRACE(_T("CXmp::RunXmp()\n"));

	// XMP 起動（Adap 起動）
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char szAdapExePath[MAX_PATH];

	::ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.wShowWindow = SW_MINIMIZE;
	::ZeroMemory( &pi, sizeof(pi) );

	///// ADAP Exe Path /////
	SR_XMP srXmp;
	ConfigFile_GetNanoSpecIni(&srXmp, CONFIG_FILE_SR_XMP);
	memset(szAdapExePath, 0, sizeof(szAdapExePath));
	if(srXmp.szAdapExePath[0] != '\0'){
		strcpy(szAdapExePath, srXmp.szAdapExePath);
	}
	else{
		strcpy(szAdapExePath, ADAP_EXE_PATH);
	}
/* added 2014.05.22 hmenjo SC_adap timeout 対策 ---------- { ---------- */
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
//	BOOL bSC_Adap = FALSE;
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ----------              */
	BOOL m_bSC_Adap = FALSE;
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */
	char szLowerAdapExePath[MAX_PATH];
	memset(szLowerAdapExePath, 0, sizeof(szLowerAdapExePath));
	for(int i = 0; i < strlen(srXmp.szAdapExePath); i++){
		szLowerAdapExePath[i] = tolower(srXmp.szAdapExePath[i]);
	}
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
//	bSC_Adap = (strstr(szLowerAdapExePath, "sc_adap.exe") != NULL);
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ----------              */
	m_bSC_Adap = (strstr(szLowerAdapExePath, "sc_adap.exe") != NULL);
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */

	DWORD l_dwSC2cnt = 5;	/* default	*/
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
//	if (FALSE != bSC_Adap) {
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ----------              */
	if (FALSE != m_bSC_Adap) {
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */
		/* ストラテジ(ディレクトリ)をカウント	*/
		CString l_strStrategyListPath = srXmp.szStrategyListPath;
		CString l_strStrategyDir;
		if (-1 != l_strStrategyListPath.ReverseFind('\\')) {
			l_strStrategyDir = l_strStrategyListPath.Left(l_strStrategyListPath.ReverseFind('\\') + 1);
		}
		CString l_strSearchPath = l_strStrategyDir + "as*";
		CFileFind l_clsFindDirAS;
		if (FALSE != l_clsFindDirAS.FindFile(l_strSearchPath, 0)) {
			DWORD l_dwAScnt = 0;
			TCHAR l_tszFileTitle[MAX_PATH];
			int i;
			BOOL l_bLast = FALSE;
			do {
				if (FALSE == l_clsFindDirAS.FindNextFile()) {
					l_bLast = TRUE;
				}
				if (FALSE == l_clsFindDirAS.IsDirectory()) {
					continue;
				}
				_tcscpy(l_tszFileTitle, l_clsFindDirAS.GetFileTitle());
				if ((_tcslen(l_tszFileTitle) < 3) || (5 < _tcslen(l_tszFileTitle))) {
					continue;
				}
				if ((_T('A') != l_tszFileTitle[0]) && (_T('a') != l_tszFileTitle[0]) &&
					(_T('S') != l_tszFileTitle[1]) && (_T('s') != l_tszFileTitle[1])) {
					continue;
				}
				for (i = 2; i < _tcslen(l_tszFileTitle); i++) {
					if ((l_tszFileTitle[i] < _T('0')) || (_T('9') < l_tszFileTitle[i])) {
						continue;
					}
				}
				l_dwAScnt++;
			} while (FALSE == l_bLast);
			if (0 < l_dwAScnt) {
				l_dwSC2cnt = l_dwAScnt;
			}
		}
	}
/* added 2014.05.22 hmenjo SC_adap timeout 対策 ---------- } ---------- */

	// Start the child process.
	if ( !::CreateProcess(NULL, szAdapExePath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) )
	{
		::MessageBox(NULL, pMojiretsu->LoadString(IDS_STRING21), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// Close process and thread handles.
	::CloseHandle( pi.hProcess );
	::CloseHandle( pi.hThread );

	HWND hWnd;
	CTimer timer;

// 2014.03.08 bagus SCOUT modified -->
// 	// Wvase32が正常起動したかチェック
// 	timer.Restart(10);
// 	while ( (hWnd = ::FindWindow(NULL, srXmp.szWvaseWindowTitle)) == NULL )	{
// 		if ( timer.IsTimeout() ) {
// 			// WVASE32応答タイムアウトエラー
// 			::MessageBox(NULL, pMojiretsu->LoadString(IDS_STRING22), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
// 			return FALSE;
// 		}
// 		::Sleep(10);
// 	}

/* deleted 2014.05.22 hmenjo SC_adap timeout 対策 ---------- { ---------- */
//	BOOL bSC_Adap = FALSE;
//
//	char szLowerAdapExePath[MAX_PATH];
//	memset(szLowerAdapExePath, 0, sizeof(szLowerAdapExePath));
//	for(int i = 0; i < strlen(srXmp.szAdapExePath); i++){
//		szLowerAdapExePath[i] = tolower(srXmp.szAdapExePath[i]);
//	}
//	bSC_Adap = (strstr(szLowerAdapExePath, "sc_adap.exe") != NULL);
/* deleted 2014.05.22 hmenjo SC_adap timeout 対策 ---------- } ---------- */

	// Wvase32が正常起動したかチェック
	timer.Restart(60);

/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
//	if(bSC_Adap){
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ----------              */
	if (FALSE != m_bSC_Adap) {
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */
		g_bScoutAppExist = FALSE;
		strcpy(g_szWindowTitle, srXmp.szWvaseWindowTitle);
		while(TRUE){
			EnumWindows(EnumWindowsProc, 0);
			if(!g_bScoutAppExist){
				if ( timer.IsTimeout() ) {
					// SCOUT応答タイムアウトエラー
					::MessageBox(NULL, pMojiretsu->LoadString(IDS_STRING24), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
					return FALSE;
				}
			}
			else{
				break;
			}
			::Sleep(10);
		}
	}
	else{
		while ( (hWnd = ::FindWindow(NULL, srXmp.szWvaseWindowTitle)) == NULL )	{
			if ( timer.IsTimeout() ) {
				// WVASE32応答タイムアウトエラー
				::MessageBox(NULL, pMojiretsu->LoadString(IDS_STRING22), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
			::Sleep(10);
		}
	}
// 2014.03.08 bagus SCOUT modified <--

// 2014.03.08 bagus SCOUT modified -->
//	::ShowWindow(hWnd, SW_HIDE);
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
//	if(!bSC_Adap){
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ----------              */
	if (FALSE == m_bSC_Adap) {
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */
		::ShowWindow(hWnd, SW_HIDE);
	}
// 2014.03.08 bagus SCOUT modified <--

	// XMP DDE 通信開始
	CDdeClient xmpDde;
	xmpDde.Init();
/* modified 2014.05.22 hmenjo SC_adap timeout 対策 ---------- { ---------- */
//	timer.Restart(10);
/* modified 2014.05.22 hmenjo SC_adap timeout 対策 ----------              */
	double l_dAdapTimeout = 10.0;
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
//	if (FALSE != bSC_Adap) {
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ----------              */
	if (FALSE != m_bSC_Adap) {
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */
		double l_dTO_ScoutStart = 30.0;		/* [s]	*/
		double l_dTO_ReadSC2 = 12.0;		/* [s]	*/
		l_dAdapTimeout = l_dTO_ScoutStart + (l_dTO_ReadSC2 * l_dwSC2cnt);
	}
	timer.Restart(l_dAdapTimeout);
/* modified 2014.05.22 hmenjo SC_adap timeout 対策 ---------- } ---------- */
	while ( !xmpDde.Connect(DDE_SERVICE, DDE_TOPIC) ) {
		if ( timer.IsTimeout() ) {
			// DDE Communication タイムアウトエラー
			::MessageBox(NULL, pMojiretsu->LoadString(IDS_STRING23), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
			return FALSE;
		}
		else
			::Sleep(10);
	};
	xmpDde.Execute(_T("WindowState(1)"));		// XMP unvisibleにする
	xmpDde.Disconnect();
	xmpDde.Uninit();

// 2014.03.08 bagus SCOUT modified -->
//	::ShowWindow(hWnd, SW_HIDE);
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
//	if(!bSC_Adap){
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ----------              */
	if (FALSE == m_bSC_Adap) {
/* modified 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */
		::ShowWindow(hWnd, SW_HIDE);
	}
// 2014.03.08 bagus SCOUT modified <--

	return TRUE;
}

// --------------------------------------------------------------------
// ShutDownXmp
void CXmp::ShutDownXmp()
{
	TRACE(_T("CXmp::ShutDownXmp()\n"));

	// XMP DDE 通信終了
	CDdeClient xmpDde;
	xmpDde.Init();
	xmpDde.Connect(DDE_SERVICE, DDE_TOPIC);
	xmpDde.Execute(_T("Quit()"));
	xmpDde.Disconnect();
	xmpDde.Uninit();
}

// --------------------------------------------------------------------
// DoXmp
BOOL CXmp::DoXmp(LPTSTR pszXmpDdeText, LPCTSTR pszCmd, CDdeClient* pXmpDde)
{
	TRACE(_T("CXmp::DoXmp()\n"));

	TCHAR szBuff[4096];
	CTimer timer;

	pXmpDde->Execute(pszCmd);

	// DDE通信でADAPから結果取得
	if ( !pXmpDde->DataCopy(szBuff, DDE_ITEM) )	{
		// DDE通信エラー
		return FALSE;
	}
	// WVASE32計算中のため結果待ち。ADAPは、WVASE32.EXEのFIT値を取得するまでの間、""を返却してくる
	timer.Restart(ADAPTIMEOUT);
	while ( _tcscmp(szBuff, _T("")) == 0 ) {
		if ( timer.IsTimeout() ) {
			// WVASE32応答タイムアウトエラー
			return FALSE;
		}
		::Sleep(10);
		if ( !pXmpDde->DataCopy(szBuff, DDE_ITEM) )	{
			// DDE通信エラー
			return FALSE;
		}
	}

	_tcscpy(pszXmpDdeText, szBuff);

	if ( _tcslen(szBuff) > 511 )
		szBuff[511] = _TCHAR('\0');
	TRACE1(_T("%s\n"), szBuff);

	return TRUE;
}

/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
BOOL CXmp::ShowXmp(int iShow)
{
	TRACE(_T("CXmp::ShowXmp()\n"));

	BOOL l_bRc = FALSE;

	HWND l_hWnd = ::FindWindow(0, ADAP_WINDOW_NAME);
	if (0 != l_hWnd) {
		switch (iShow) {
		case 1:
		case 2:
			{
				/* xmp に表示/非表示	*/
				int l_iCmd = 1;
				switch (iShow) {
				case 1:		l_iCmd = 1;	break;
				case 2:		l_iCmd = 2;	break;
				}
				TCHAR l_tszCmd[64];
				_stprintf(l_tszCmd, _T("WindowState(%d)"), l_iCmd);
				CDdeClient l_xmpDde;
				l_xmpDde.Init();
				l_xmpDde.Connect(DDE_SERVICE, DDE_TOPIC);
				l_xmpDde.Execute(l_tszCmd);
				l_xmpDde.Disconnect();
				l_xmpDde.Uninit();
				/* ADAP に表示/非表示	*/
				::ShowWindow(l_hWnd, (1 == l_iCmd)? SW_HIDE : SW_SHOWNORMAL);
			}
			break;
		default:
			/* 何もしない	*/
			break;
		}
	}

	return l_bRc;
}
/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */
