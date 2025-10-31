#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#define _NEXUIDLL_

#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <tchar.h>
#include <NEXINI.HXX>
#include <NEXRC.HXX>
#include <NEXUI.HXX>

#ifdef _DEBUG
#pragma comment(lib, "dtnsnexini.lib")
#else
#pragma comment(lib, "tnsnexini.lib")
#endif

HINSTANCE hInstance   = NULL;
HMODULE   hModuleRsrc = NULL;
HHOOK	  hHook 	  = NULL;
NEXINIPARAMS params;

// Foward declarations of functions included in this code module:
static BOOL InitInstance(void);
static void ExitInstance(void);
LRESULT CALLBACK CBTProc(int, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProcConfirm(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProcConfig(HWND, UINT, WPARAM, LPARAM);
/* added 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- { ---------- */
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
/* added 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- } ---------- */

/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
BOOL APIENTRY DllMain( HANDLE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch ( ul_reason_for_call ) {
	case DLL_PROCESS_ATTACH:
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("NEXUI"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
			hInstance = (HINSTANCE) hModule;
			nexiniGetParameter(&params);
/* added 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- { ---------- */
		GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- } ---------- */
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		break;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// nexuiPasswordConfirm
// ---------------------------------------------------------------------------
int __stdcall nexuiMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	hHook = SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());

	int iRet = MessageBox(hWnd, lpText, lpCaption, uType);

	if ( hHook != NULL )
		UnhookWindowsHookEx(hHook);

	return iRet;
}

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd;
	if ( nCode >= 0 ) {
		switch ( nCode ) {
		case HCBT_ACTIVATE:
			hwnd = GetDlgItem((HWND) wParam, IDOK);
			if ( hwnd != NULL )
				SetWindowText(hwnd, "OK");
			hwnd = GetDlgItem((HWND) wParam, IDCANCEL);
			if ( hwnd != NULL )
				SetWindowText(hwnd, params.System.Language == LANGUAGE_ENGLISH ? "Cancel" : "キャンセル");
			hwnd = GetDlgItem((HWND) wParam, IDRETRY);
			if ( hwnd != NULL )
				SetWindowText(hwnd, params.System.Language == LANGUAGE_ENGLISH ? "Retry(&R)" : "再試行(&R)");
			UnhookWindowsHookEx(hHook);
			hHook = NULL;
			return 0;
		}
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// ---------------------------------------------------------------------------
// nexuiPasswordConfirm
// ---------------------------------------------------------------------------
BOOL __stdcall nexuiPasswordConfirm(HWND hwndParent)
{
	BOOL bRet = FALSE;

/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- { ---------- */
//	  if ( !GetPrivateProfileInt (
//				"PASSWORD",
//				"ENABLE",
//				INT(TRUE),
//				NEXPASS_INIFILE_NAME ) ) return TRUE;
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- 			 */
	TCHAR l_tszIniPath[_MAX_PATH];
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
	_stprintf(l_tszIniPath, _T("%s") NEXPASS_INIFILE_NAME, g_tszProcDir);
	if ( !GetPrivateProfileInt (
			  "PASSWORD",
			  "ENABLE",
			  INT(TRUE),
			  l_tszIniPath ) ) return TRUE;
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- } ---------- */

	hModuleRsrc = NexResourceLoadLibrary(params.System.Language);
	if ( hModuleRsrc != NULL ) {
		HRSRC hRsrc  = FindResource(hModuleRsrc, MAKEINTRESOURCE(IDD_PASSWORD_DIALOG), RT_DIALOG);
		HGLOBAL hGlobal = LoadResource(hModuleRsrc, hRsrc);
		LPDLGTEMPLATE lpDlgTmp = (LPDLGTEMPLATE) LockResource(hGlobal);
		bRet = DialogBoxIndirect(hInstance, lpDlgTmp, hwndParent, (DLGPROC) DlgProcConfirm) == IDOK ? TRUE : FALSE;
		UnlockResource(hRsrc);
		FreeResource(hRsrc);
		FreeLibrary(hModuleRsrc);
	}
	return bRet;
}

// ---------------------------------------------------------------------------
// nexuiPasswordConfig
// ---------------------------------------------------------------------------
BOOL __stdcall nexuiPasswordConfig(HWND hwndParent)
{
	BOOL bRet = FALSE;

	hModuleRsrc = NexResourceLoadLibrary(params.System.Language);
	if ( hModuleRsrc != NULL ) {
		HRSRC hRsrc  = FindResource(hModuleRsrc, MAKEINTRESOURCE(IDD_PASSWORD_CFG_DIALOG), RT_DIALOG);
		HGLOBAL hGlobal = LoadResource(hModuleRsrc, hRsrc);
		LPDLGTEMPLATE lpDlgTmp = (LPDLGTEMPLATE) LockResource(hGlobal);
		bRet = DialogBoxIndirect(hInstance, lpDlgTmp, hwndParent, (DLGPROC) DlgProcConfig) == IDOK ? TRUE : FALSE;
		UnlockResource(hRsrc);
		FreeResource(hRsrc);
		FreeLibrary(hModuleRsrc);
	}
	return bRet;
}

INT_PTR CALLBACK DlgProcConfirm(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR szCurrPassword[NEXPASS_STRING_LENGTH + 1];
	TCHAR szPassword[NEXPASS_STRING_LENGTH + 1];
	TCHAR szTitle[256] = { 0 };
	TCHAR szMessage[256] = { 0 };

	switch ( message ) {
	case WM_INITDIALOG:
		Edit_LimitText(GetDlgItem(hwnd, IDC_PASSWORD), NEXPASS_STRING_LENGTH);
		return (TRUE);
	case WM_COMMAND:
		switch ( GET_WM_COMMAND_ID(wParam, lParam) ) {
		case IDOK:
#ifdef _DEBUG
			_tcscpy(szCurrPassword, NEXPASS_DEBUG);
#else
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- { ---------- */
//			  GetPrivateProfileString (
//				  "PASSWORD",
//				  "CURRENT",
//				  NEXPASS_DEBUG,
//				  szCurrPassword,
//				  sizeof(szCurrPassword),
//				  NEXPASS_INIFILE_NAME );
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- 			 */
			TCHAR l_tszIniPath[_MAX_PATH];
			GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
			_stprintf(l_tszIniPath, _T("%s") NEXPASS_INIFILE_NAME, g_tszProcDir);
			GetPrivateProfileString (
				"PASSWORD",
				"CURRENT",
				NEXPASS_DEBUG,
				szCurrPassword,
				sizeof(szCurrPassword),
				l_tszIniPath );
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- } ---------- */
#endif
			Edit_GetText(GetDlgItem(hwnd, IDC_PASSWORD), szPassword, sizeof(szCurrPassword));
			if ( _tcsicmp(szCurrPassword, szPassword) != 0 ) {
				LoadString(hModuleRsrc, IDS_PASSWORD_CONFIRM_TITLE, szTitle, sizeof(szTitle));
				LoadString(hModuleRsrc, IDS_PASSWORD_DEFFERENT, szMessage, sizeof(szMessage));
				nexuiMessageBox(hwnd, szMessage, szTitle, MB_OK | MB_ICONWARNING);
				Edit_SetText(GetDlgItem(hwnd, IDC_PASSWORD), "");
				break;
			}
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		  }
		return (TRUE);
	}
	return (FALSE);
}

INT_PTR CALLBACK DlgProcConfig(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bEnable;
	TCHAR szNewPassword[NEXPASS_STRING_LENGTH + 1];
	TCHAR szConfirmPassword[NEXPASS_STRING_LENGTH + 1];
	TCHAR szBuffer[256] = { 0 };
	TCHAR szTitle[256] = { 0 };
	TCHAR szMessage[256] = { 0 };
/* added 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- { ---------- */
	TCHAR l_tszIniPath[_MAX_PATH];
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
	_stprintf(l_tszIniPath, _T("%s") NEXPASS_INIFILE_NAME, g_tszProcDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- } ---------- */

	switch ( message ) {
	case WM_INITDIALOG:
		bEnable = GetPrivateProfileInt (
					  "PASSWORD",
					  "ENABLE",
					  INT(TRUE),
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- { ---------- */
//						NEXPASS_INIFILE_NAME );
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- 			 */
					  l_tszIniPath );
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- } ---------- */
		Button_SetCheck(GetDlgItem(hwnd, IDC_PASSWORD_EANABLE), bEnable);
		Edit_LimitText(GetDlgItem(hwnd, IDC_NEW_PASSWORD), NEXPASS_STRING_LENGTH);
		Edit_LimitText(GetDlgItem(hwnd, IDC_CONFIRM_NEW_PASSWORD), NEXPASS_STRING_LENGTH);
		return (TRUE);
	case WM_COMMAND:
		switch ( GET_WM_COMMAND_ID(wParam, lParam) ) {
		case IDOK:
			_itot(Button_GetCheck(GetDlgItem(hwnd, IDC_PASSWORD_EANABLE)), szBuffer, 10);
			WritePrivateProfileString (
				"PASSWORD",
				"ENABLE",
				szBuffer,
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- { ---------- */
//				  NEXPASS_INIFILE_NAME );
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- 			 */
				l_tszIniPath );
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- } ---------- */
			Edit_GetText(GetDlgItem(hwnd, IDC_NEW_PASSWORD), szNewPassword, sizeof(szNewPassword));
			Edit_GetText(GetDlgItem(hwnd, IDC_CONFIRM_NEW_PASSWORD), szConfirmPassword, sizeof(szConfirmPassword));
			if ( _tcscmp(szNewPassword, szConfirmPassword) != 0 ) {
				LoadString(hModuleRsrc, IDS_PASSWORD_CONFIG_TITLE, szTitle, sizeof(szTitle));
				LoadString(hModuleRsrc, IDS_PASSWROD_CONFIRM_ERR, szMessage, sizeof(szMessage));
				nexuiMessageBox(hwnd, szMessage, szTitle, MB_OK | MB_ICONWARNING);
				EndDialog(hwnd, IDCANCEL);
				break;
			}

			LoadString(hModuleRsrc, IDS_PASSWORD_CONFIG_TITLE, szTitle, sizeof(szTitle));
			LoadString(hModuleRsrc, IDS_PASSWORD_SAVE, szMessage, sizeof(szMessage));
			if ( nexuiMessageBox(hwnd, szMessage, szTitle, MB_YESNO | MB_ICONWARNING) == IDNO ) break;

			WritePrivateProfileString (
				"PASSWORD",
				"CURRENT",
				szNewPassword,
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- { ---------- */
//				  NEXPASS_INIFILE_NAME );
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- 			 */
				l_tszIniPath );
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexui.dll ---------- } ---------- */
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		  }
		return (TRUE);
	}
	return (FALSE);
}
