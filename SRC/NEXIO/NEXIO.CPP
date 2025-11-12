#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#define _NEXIODLL_

#include <windows.h>
#include <commctrl.h>
#include <process.h>
#include <crtdbg.h>
#define  __NEXTRA_INTERNAL
#include <NEXIOBASE.HXX>
#include <NEXRC.HXX>
#include <NEXUI.HXX>
#include "nexio.h"
#include "nexiotypea.h"
#include "nexiotypeb.h"
// matsuhisa 2011.09.13 add >>>
#include "nexiotypec.h"
// matsuhisa 2011.09.13 add <<<
//Saiki 20130812 Add ----->
#include "nexiotyped.h"
//Saiki 20130812 Add <-----
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
#include "nexiotypee.h"
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
#pragma comment(lib, "apidio.lib")
#ifdef _DEBUG
#pragma comment(lib, "dtnsnexui.lib")
#else
#pragma comment(lib, "tnsnexui.lib")
#endif

using namespace nano;

#pragma data_seg("Shared")
HINSTANCE	   hInstance   = NULL;
HWND		   hwndMonitor = NULL;
DWORD		   dwProcessId = 0L;
HWND		   hwndOwner   = NULL;
HWND		   hwndTabPages[NEXIO_MAX_SLOTS_NUM] = { NULL, NULL, NULL };
UINT		   nCurrentPage   = 0;
UINT		   nNumberOfPages = 0;
#pragma data_seg()

#pragma comment(linker, "/Section:Shared,RWS")

#define NEXIO_DINP_START_CTRLIDS		IDC_CN1_IN000
#define NEXIO_DINP_END_CTRLIDS			IDC_CN1_IN077
#define NEXIO_DOUT_START_CTRLIDS		IDC_CN1_OUT000
#define NEXIO_DOUT_END_CTRLIDS			IDC_CN1_OUT077
#define REFRESH_TIMER_ID				(1)
#define REFRESH_TIMER_ELAPSE			(100L)

// Foward declarations of functions included in this code module:
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK TabProc(HWND, UINT, WPARAM, LPARAM);
static void InitializeTabCtrl(HWND);
static UINT UpdateData(UINT nID, BOOL bInput, BOOL bSaveAndValidate =TRUE);
static void CheckDioButton(UINT nIDFirst, UINT nIDLast, BOOL bInput);

cDioBaseThread	Dio;
cDioInterface*	pInterface = NULL;
NEXINIPARAMS	params;

static UINT idd_TYPE[NEXTRA_TYPE_MAX] =
{
	IDD_DIO_TYPEA_DIALOG,
	IDD_DIO_TYPEB_DIALOG,
};

//==============================================================================================
//	  NexI/O module. (nexio...).
//==============================================================================================
DEFINE_NEXIOPROCTYPE(cDioInterface*, nexioGetDioInterface, (void))

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
		if (FALSE == DllMutexCreate(_T("NEXIO"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		_RPTF0(_CRT_WARN, "NEXIO - DLL_PROCESS_ATTACH\n");
		if ( hInstance == NULL ) {
			hInstance = (HINSTANCE) hModule;
		}
		nexiniGetParameter(&params);
		switch ( params.System.Type ) {
		case NEXTRA_TYPEA:
			pInterface = new cDioTypeA();
			break;
		case NEXTRA_TYPEB:
			pInterface = new cDioTypeB();
			break;
// matsuhisa 2011.09.13 add >>>
		case NEXTRA_TYPEC:
			pInterface = new cDioTypeC();
			break;
// matsuhisa 2011.09.13 add <<<
//Saiki 20130812 Add ----->
//Saiki 20130812 Add ----->
		case NEXTRA_TYPED:
			pInterface = new cDioTypeD();
			break;
//Saiki 20130812 Add <-----
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
		case NEXTRA_TYPEE:
			pInterface = new cDioTypeE();
			break;
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		default:
			_ASSERT(FALSE);
			break;
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if ( pInterface != NULL ) {
			delete pInterface;
			pInterface = NULL;
		}
		_RPTF0(_CRT_WARN, "NEXIO - DLL_PROCESS_DETACH\n");
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		break;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Initialize
// ---------------------------------------------------------------------------
BOOL Initialize(HWND hwnd)
{
	if ( hwndOwner == NULL ) {
		hwndOwner = hwnd;

		Dio.Initialize();

		if ( hwndMonitor == NULL ) {
			if ( Dio.IsValidModule() ) {
				// メイン画面作成
				HRSRC hRsrc  = FindResource(Dio.GetModuleHandle(), MAKEINTRESOURCE(IDD_DIO_MONITOR_DIALOG), RT_DIALOG);
				HGLOBAL hGlobal = LoadResource(Dio.GetModuleHandle(), hRsrc);
				LPDLGTEMPLATE lpDlgTmp = (LPDLGTEMPLATE) LockResource(hGlobal);
				hwndMonitor = CreateDialogIndirect(hInstance, lpDlgTmp, NULL, (DLGPROC) DlgProc);
				UnlockResource(hRsrc);
				FreeResource(hRsrc);
				// DI/Oポート子画面作成
				InitializeTabCtrl(GetDlgItem(hwndMonitor, IDC_TABCTRL));
				dwProcessId = GetCurrentProcessId();
			}
		}
		if ( hwndMonitor == NULL || !IsWindow(hwndMonitor) ) return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// InitializeTabCtrl
// ---------------------------------------------------------------------------
void InitializeTabCtrl(HWND hwndTab)
{
	TCHAR			szPath[_MAX_PATH];
	TCHAR			szBuffer[256], szAppName[256], szKeyName[256];
	HRSRC			hRsrc;
	HGLOBAL 		hGlobal;
	LPDLGTEMPLATE	lpDlgTmp;
	TCITEM			tcItem;
	UINT			nID;

	// 相対パス設定
	// これが相対パスが設定されたNEXIO.TXTファイルだ！
	process_directory::make(szPath, NEXIO_TXTFILE_NAME);

	for ( nNumberOfPages = 0; nNumberOfPages < Dio.GetNumSlots(); nNumberOfPages++ ) {
		hRsrc  = FindResource(Dio.GetModuleHandle(), MAKEINTRESOURCE(IDD_DIO_MONITOR_TAB), RT_DIALOG);
		hGlobal = LoadResource(Dio.GetModuleHandle(), hRsrc);
		lpDlgTmp = (LPDLGTEMPLATE) LockResource(hGlobal);
		hwndTabPages[nNumberOfPages] = CreateDialogIndirect(hInstance, lpDlgTmp, hwndMonitor, (DLGPROC) TabProc);

		// タブへ挿入
		_stprintf(szAppName, "CN%d", nNumberOfPages + 2);
		GetPrivateProfileString(
			szAppName,
			"TabTitle",
			"",
			szBuffer,
			sizeof(szBuffer),
			szPath
			);
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = szBuffer;
		TabCtrl_InsertItem(hwndTab, nNumberOfPages, &tcItem);

		UnlockResource(hRsrc);
		FreeResource(hRsrc);

		// NEXIO.TXTファイルから読み込んだテキストを各タブのボタンテキストに更新
		// ファイルが無かった場合は想定外！
		// 絶対に用意する事
		for ( nID = 0L; nID <= NEXIO_DINP_END_CTRLIDS - NEXIO_DINP_START_CTRLIDS; nID++ ) {
			_stprintf(szKeyName, "CN%d_IN_%02o", nNumberOfPages + 2, nID);
			GetPrivateProfileString(
				szAppName,
				szKeyName,
				"",
				szBuffer,
				sizeof(szBuffer),
				szPath
				);
			SetDlgItemText(hwndTabPages[nNumberOfPages], nID + NEXIO_DINP_START_CTRLIDS, szBuffer);
		}
		for ( nID = 0L; nID <= NEXIO_DOUT_END_CTRLIDS - NEXIO_DOUT_START_CTRLIDS; nID++ ) {
			_stprintf(szKeyName, "CN%d_OUT_%02o", nNumberOfPages + 2, nID);
			GetPrivateProfileString(
				szAppName,
				szKeyName,
				"",
				szBuffer,
				sizeof(szBuffer),
				szPath
				);
			SetDlgItemText(hwndTabPages[nNumberOfPages], nID + NEXIO_DOUT_START_CTRLIDS, szBuffer);
		}
	}
	// タブ子画面の位置調整
	RECT tabRect, itemRect;
	GetClientRect(hwndTab, &tabRect);
	TabCtrl_GetItemRect(hwndTab, 0, &itemRect);
	int nX	= itemRect.left;
	int nY	= itemRect.bottom + 1;
	int nXc = tabRect.right - itemRect.left - 1;
	int nYc = tabRect.bottom - nY - 1;

	// タブ１のみ表示し後は非表示
	SetWindowPos(hwndTabPages[0], HWND_TOP, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for ( int index = 1; (size_t)index < nNumberOfPages; index++ ) {
		SetWindowPos(hwndTabPages[index], HWND_TOP, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

// ---------------------------------------------------------------------------
// Finalize
// ---------------------------------------------------------------------------
void Finalize(void)
{
// matsuhisa 2013.06.11 modified >>>
//	if ( dwProcessId == GetCurrentProcessId() ) {
//		while ( nNumberOfPages > 0 ) {
//			if ( hwndTabPages[nNumberOfPages - 1] != NULL )
//			DestroyWindow(hwndTabPages[nNumberOfPages - 1]);
//			nNumberOfPages--;
//		}
//		if ( hwndMonitor != NULL ) {
//			DestroyWindow(hwndMonitor);
//			hwndMonitor = NULL;
//		}
//	}
	if ( hwndOwner != NULL ) {
		hwndOwner = NULL;
		while ( nNumberOfPages > 0 ) {
			if ( hwndTabPages[nNumberOfPages - 1] != NULL )
			DestroyWindow(hwndTabPages[nNumberOfPages - 1]);
			nNumberOfPages--;
		}
		if ( hwndMonitor != NULL ) {
			DestroyWindow(hwndMonitor);
			hwndMonitor = NULL;
		}
		Dio.Finalize();
	}
// matsuhisa 2013.06.11 modified <<<
}

// ===========================================================================
// Request functions
// ===========================================================================
//
//	FUNCTION: TabProc(HWND, unsigned, WORD, LONG)
//
//	PURPOSE:  Processes messages for the main window.
//
//	WM_INITDIALOG  -  message for window/dialog
//
//
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch ( message ) {
	case WM_INITDIALOG:
		return (FALSE);
	case WM_ACTIVATE:
		if ( wParam == WA_INACTIVE ) {
			SetFocus(GetDlgItem(hwnd, IDOK));
		}
		return (FALSE);
	case WM_COMMAND:
		switch ( GET_WM_COMMAND_ID(wParam, lParam) ) {
		case IDC_MANUAL_INPUT:
			if ( Button_GetCheck(GetDlgItem(hwnd, IDC_MANUAL_INPUT)) ) {
				if ( !nexuiPasswordConfirm(hwnd) ) {
					Button_SetCheck(GetDlgItem(hwnd, IDC_MANUAL_INPUT), FALSE);
					break;
				}
			}
			Dio.manual.input.flip();
			break;
		case IDC_MANUAL_OUTPUT:
			if ( Button_GetCheck(GetDlgItem(hwnd, IDC_MANUAL_OUTPUT)) ) {
				if ( !nexuiPasswordConfirm(hwnd) ) {
					Button_SetCheck(GetDlgItem(hwnd, IDC_MANUAL_OUTPUT), FALSE);
					break;
				}
			}
			Dio.manual.output.flip();
			break;
		case IDOK:
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		return (FALSE);
	case WM_TIMER:
		return (FALSE);
	case WM_NOTIFY:
		switch ( ((NMHDR*) lParam)->code ) {
		case TCN_SELCHANGE:
			// タブがクリックされ現在と違う場合に切り替える
			if ( nCurrentPage != TabCtrl_GetCurSel(GetDlgItem(hwndMonitor, IDC_TABCTRL)) ) {
				ShowWindow(hwndTabPages[nCurrentPage], SW_HIDE);
				nCurrentPage = TabCtrl_GetCurSel(GetDlgItem(hwndMonitor, IDC_TABCTRL));
				ShowWindow(hwndTabPages[nCurrentPage], SW_SHOW);
			}
			break;
		}
		return (FALSE);
	}
	return (FALSE);
}

// ===========================================================================
// Request functions
// ===========================================================================
//
//	FUNCTION: TabProc(HWND, unsigned, WORD, LONG)
//
//	PURPOSE:  Processes messages for the main window.
//
//	WM_INITDIALOG  -  message for window/dialog
//
//
INT_PTR CALLBACK TabProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch ( message ) {
	case WM_INITDIALOG:
		SetTimer(hwnd, REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
		return (FALSE);
	case WM_ACTIVATE:
		if ( wParam == WA_INACTIVE ) {
			SetFocus(GetDlgItem(hwnd, IDOK));
		}
		break;
	case WM_COMMAND:
		switch ( GET_WM_COMMAND_ID(wParam, lParam) ) {
		case IDC_CN1_IN000: case IDC_CN1_IN001: case IDC_CN1_IN002: case IDC_CN1_IN003: case IDC_CN1_IN004: case IDC_CN1_IN005: case IDC_CN1_IN006: case IDC_CN1_IN007:
		case IDC_CN1_IN010: case IDC_CN1_IN011: case IDC_CN1_IN012: case IDC_CN1_IN013: case IDC_CN1_IN014: case IDC_CN1_IN015: case IDC_CN1_IN016: case IDC_CN1_IN017:
		case IDC_CN1_IN020: case IDC_CN1_IN021: case IDC_CN1_IN022: case IDC_CN1_IN023: case IDC_CN1_IN024: case IDC_CN1_IN025: case IDC_CN1_IN026: case IDC_CN1_IN027:
		case IDC_CN1_IN030: case IDC_CN1_IN031: case IDC_CN1_IN032: case IDC_CN1_IN033: case IDC_CN1_IN034: case IDC_CN1_IN035: case IDC_CN1_IN036: case IDC_CN1_IN037:
		case IDC_CN1_IN040: case IDC_CN1_IN041: case IDC_CN1_IN042: case IDC_CN1_IN043: case IDC_CN1_IN044: case IDC_CN1_IN045: case IDC_CN1_IN046: case IDC_CN1_IN047:
		case IDC_CN1_IN050: case IDC_CN1_IN051: case IDC_CN1_IN052: case IDC_CN1_IN053: case IDC_CN1_IN054: case IDC_CN1_IN055: case IDC_CN1_IN056: case IDC_CN1_IN057:
		case IDC_CN1_IN060: case IDC_CN1_IN061: case IDC_CN1_IN062: case IDC_CN1_IN063: case IDC_CN1_IN064: case IDC_CN1_IN065: case IDC_CN1_IN066: case IDC_CN1_IN067:
		case IDC_CN1_IN070: case IDC_CN1_IN071: case IDC_CN1_IN072: case IDC_CN1_IN073: case IDC_CN1_IN074: case IDC_CN1_IN075: case IDC_CN1_IN076: case IDC_CN1_IN077:
			if ( Dio.manual.input.is_enable() )
				UpdateData(GET_WM_COMMAND_ID(wParam, lParam) - NEXIO_DINP_START_CTRLIDS, TRUE);
			break;
		case IDC_CN1_OUT000: case IDC_CN1_OUT001: case IDC_CN1_OUT002: case IDC_CN1_OUT003: case IDC_CN1_OUT004: case IDC_CN1_OUT005: case IDC_CN1_OUT006: case IDC_CN1_OUT007:
		case IDC_CN1_OUT010: case IDC_CN1_OUT011: case IDC_CN1_OUT012: case IDC_CN1_OUT013: case IDC_CN1_OUT014: case IDC_CN1_OUT015: case IDC_CN1_OUT016: case IDC_CN1_OUT017:
		case IDC_CN1_OUT020: case IDC_CN1_OUT021: case IDC_CN1_OUT022: case IDC_CN1_OUT023: case IDC_CN1_OUT024: case IDC_CN1_OUT025: case IDC_CN1_OUT026: case IDC_CN1_OUT027:
		case IDC_CN1_OUT030: case IDC_CN1_OUT031: case IDC_CN1_OUT032: case IDC_CN1_OUT033: case IDC_CN1_OUT034: case IDC_CN1_OUT035: case IDC_CN1_OUT036: case IDC_CN1_OUT037:
		case IDC_CN1_OUT040: case IDC_CN1_OUT041: case IDC_CN1_OUT042: case IDC_CN1_OUT043: case IDC_CN1_OUT044: case IDC_CN1_OUT045: case IDC_CN1_OUT046: case IDC_CN1_OUT047:
		case IDC_CN1_OUT050: case IDC_CN1_OUT051: case IDC_CN1_OUT052: case IDC_CN1_OUT053: case IDC_CN1_OUT054: case IDC_CN1_OUT055: case IDC_CN1_OUT056: case IDC_CN1_OUT057:
		case IDC_CN1_OUT060: case IDC_CN1_OUT061: case IDC_CN1_OUT062: case IDC_CN1_OUT063: case IDC_CN1_OUT064: case IDC_CN1_OUT065: case IDC_CN1_OUT066: case IDC_CN1_OUT067:
		case IDC_CN1_OUT070: case IDC_CN1_OUT071: case IDC_CN1_OUT072: case IDC_CN1_OUT073: case IDC_CN1_OUT074: case IDC_CN1_OUT075: case IDC_CN1_OUT076: case IDC_CN1_OUT077:
			if ( Dio.manual.output.is_enable() )
				UpdateData(GET_WM_COMMAND_ID(wParam, lParam) - NEXIO_DOUT_START_CTRLIDS, FALSE);
			break;
		case IDOK:
			break;
		case IDCANCEL:
			break;
		  }
		return (FALSE);
	case WM_TIMER:
		if ( wParam == REFRESH_TIMER_ID ) {
			CheckDioButton(NEXIO_DINP_START_CTRLIDS, NEXIO_DINP_END_CTRLIDS, TRUE);
			CheckDioButton(NEXIO_DOUT_START_CTRLIDS, NEXIO_DOUT_END_CTRLIDS, FALSE);
			KillTimer(hwnd, REFRESH_TIMER_ID);
			SetTimer(hwnd, REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
		}
		return (FALSE);
	}
	return (FALSE);
}

// ---------------------------------------------------------------------------
// UpdateData
// ---------------------------------------------------------------------------
UINT UpdateData(UINT nID, BOOL bInput, BOOL bSaveAndValidate/*=TRUE*/)
{
	UINT uRet = BST_UNCHECKED;

	if ( pInterface == NULL) return 0L;

	basic_device* ptr = bInput ? Dio.GetInpDataPtr() : Dio.GetOutDataPtr();

	div_t div_result = div(nID, 8);
	unsigned long port	 = (unsigned long) div_result.quot + nCurrentPage * 8;
	unsigned long bit	 = (unsigned long) div_result.rem;

	unsigned long mask = 0x00000001;
	mask <<= bit;
	if ( bSaveAndValidate ) {
		if ( ptr[port].byte & mask ) {
			ptr[port].byte &= ~mask;
		}
		else {
			ptr[port].byte |= mask;
			uRet = BST_CHECKED;
		}
	}
	else {
		uRet = ptr[port].byte & mask ? BST_CHECKED : BST_UNCHECKED;
	}

	return uRet;
}
// ---------------------------------------------------------------------------
// CheckDioButton
// ---------------------------------------------------------------------------
void CheckDioButton(UINT nIDFirst, UINT nIDLast, BOOL bInput)
{
	HWND hwnd;
	UINT uCheck;

	for ( UINT nIDButton = nIDFirst; nIDButton < nIDLast + 1; nIDButton++ ) {
		hwnd = GetDlgItem(hwndTabPages[nCurrentPage], nIDButton);

		if ( hwnd != NULL ) {
			uCheck = UpdateData(nIDButton - nIDFirst, bInput, FALSE/*bSaveAndValidate*/);
			CheckDlgButton(hwndTabPages[nCurrentPage], nIDButton, uCheck);
		}
	}
}

// ---------------------------------------------------------------------------
// Initialize module
// ---------------------------------------------------------------------------
BOOL __stdcall nexioInitialize(HWND owner)
{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if(params.System.Type == NEXTRA_TYPEE){
		return TRUE;
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	return Initialize(owner);
}

// ---------------------------------------------------------------------------
// Finalize module
// ---------------------------------------------------------------------------
NEXIOAPI void __stdcall nexioFinalize(void)
{
	Finalize();
}

// ---------------------------------------------------------------------------
// I/O monitor
// ---------------------------------------------------------------------------
BOOL __stdcall nexioMonitor(int X, int Y)
{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if(params.System.Type == NEXTRA_TYPEE){
		return TRUE;
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	if ( hwndMonitor != NULL && IsWindow(hwndMonitor) ) {
		if ( !IsWindowVisible(hwndMonitor) )
			SetWindowPos(hwndMonitor, NULL, X, Y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
		SetForegroundWindow(hwndMonitor);
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Inquire EMO status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsEmergencyStop(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsEmergencyStop();
}

// ---------------------------------------------------------------------------
// Inquire door interlock status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsDoorInterlock(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsDoorInterlock();
}

// ---------------------------------------------------------------------------
// Inquire equipment power on status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsEquipmentPower(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsEquipmentPower();
}

// ---------------------------------------------------------------------------
// Inquire safety plug status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsSafetyPlug(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsSafetyPlug();
}

// ---------------------------------------------------------------------------
// Inquire hepa power on status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsHepaPower(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsHepaPower();
}

// ---------------------------------------------------------------------------
// Inquire air pressure level low status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsAirPressureLevelLow(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsAirPressureLevelLow();
}

// ---------------------------------------------------------------------------
// Inquire move2 alarm status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsMovo2Alarm(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMovo2Alarm();
}
// ---------------------------------------------------------------------------
// Inquire maintenance switch on status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsMaintenanceSwitch(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMaintenanceSwitch();
}
// ---------------------------------------------------------------------------
// Inquire toho engineer maintenance switch on status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsEngineerMaintenanceSwitch(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsEngineerMaintenanceSwitch();
}

// ---------------------------------------------------------------------------
// Inquire stress scan start position X status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsStressScanStartPosX(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsStressScanStartPosX();
}

// ---------------------------------------------------------------------------
// Inquire stress scan end position X status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsStressScanEndPosX(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsStressScanEndPosX();
}

// ---------------------------------------------------------------------------
// Inquire shutter open status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsShutterOpen(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsShutterOpen();
}

// ---------------------------------------------------------------------------
// Inquire shutter close status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsShutterClose(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsShutterClose();
}

// ---------------------------------------------------------------------------
// Inquire robot arm detection status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRobotArmDetect(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRobotArmDetect();
}

// ---------------------------------------------------------------------------
// Inquire stress unit alarm status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsStressUnitAlarm(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsStressUnitAlarm();
}

// ---------------------------------------------------------------------------
// Inquire pin upper position status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsPinUpperPos(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsPinUpperPos();
}

// ---------------------------------------------------------------------------
// Inquire pin alignment position status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsPinAlignmentPos(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsPinAlignmentPos();
}

// ---------------------------------------------------------------------------
// Inquire pin down position status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsPinDownPos(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsPinDownPos();
}

// ---------------------------------------------------------------------------
// Inquire work guide open status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsWorkGuideOpen(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsWorkGuideOpen();
}

// ---------------------------------------------------------------------------
// Inquire work guide close status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsWorkGuideClose(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsWorkGuideClose();
}

// ---------------------------------------------------------------------------
// Inquire stress retract position Z
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRetractPosZ(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRetractPosZ();
}

// ---------------------------------------------------------------------------
// Inquire stage load posisiton status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsStageLoadPos(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsStageLoadPos();
}

// ---------------------------------------------------------------------------
// Inquire vacuum on status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsVacuumOn(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsVacuumOn();
}

// ---------------------------------------------------------------------------
// Inquire vacuum off status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsVacuumOff(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsVacuumOff();
}

// ---------------------------------------------------------------------------
// Inquire glass existence status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsGlassExist(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsGlassExist();
}

// ---------------------------------------------------------------------------
// Inquire vacuum status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsVacuumStatus(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->VacuumStatus();
}

// ---------------------------------------------------------------------------
// Inquire air float status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsAirFloatStatus(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->AirFloatStatus();
}

// ---------------------------------------------------------------------------
// Inquire hepa status
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsHepaStatus(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->HepaStatus();
}
// ---------------------------------------------------------------------------
// Execute equipment status run
// ---------------------------------------------------------------------------
void __stdcall nexioEquipmentStatusRun(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->EquipmentStatusRun(bOn);
}

// ---------------------------------------------------------------------------
// Execute equipment power off
// ---------------------------------------------------------------------------
void __stdcall nexioEquipmentPowerOFF(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->EquipmentPowerOFF(bOn);
}

// matsuhisa 2013.06.04 added >>>
// ---------------------------------------------------------------------------
// Sub-controller power off
// ---------------------------------------------------------------------------
void __stdcall nexioSubConPowerOFF(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->SubConPowerOFF(bOn);
}
// matsuhisa 2013.06.04 added <<<

// ---------------------------------------------------------------------------
// Execute hepa power off
// ---------------------------------------------------------------------------
void __stdcall nexioHepaPowerOFF(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->HepaPowerOFF(bOn);
}

// ---------------------------------------------------------------------------
// Execute stress measurement triger
// ---------------------------------------------------------------------------
void __stdcall nexioStressMeasTriger(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->StressMeasTriger(bOn);
}

// ---------------------------------------------------------------------------
// Inquire servo on status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsServoOnEx(unsigned short axis)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsServoOnEx(axis);
}

// ---------------------------------------------------------------------------
// Inquire servo ready status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsServoReadyEx(unsigned short axis)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsServoReadyEx(axis);
}

// ---------------------------------------------------------------------------
// Inquire servo alarm status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsServoAlarmEx(unsigned short axis)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsServoAlarmEx(axis);
}

// ---------------------------------------------------------------------------
// Inquire pin position completion status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsPinPosCompEx(unsigned short axis)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsPinPosCompEx(axis);
}

// ---------------------------------------------------------------------------
// Inquire pin upper position status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsPinUpperPosEx(unsigned short axis)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsPinUpperPosEx(axis);
}

// ---------------------------------------------------------------------------
// Inquire pin alignment position status for nextra function
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsPinAlignmentPosEx(unsigned short axis)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsPinAlignmentPosEx(axis);
}

// ---------------------------------------------------------------------------
// Inquire pin lower position status for nextra function
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsPinLowerPosEx(unsigned short axis)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsPinLowerPosEx(axis);
}

// ---------------------------------------------------------------------------
// Inquire pin down position status for nextra function
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsPinDownPosEx(unsigned short axis)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsPinDownPosEx(axis);
}

// ---------------------------------------------------------------------------
// Inquire work guide open status for nextra function
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsWorkGuideOpenEx(unsigned short num)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsWorkGuideOpenEx(num);
}

// ---------------------------------------------------------------------------
// Inquire work guide close status for nextra function
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsWorkGuideCloseEx(unsigned short num)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsWorkGuideCloseEx(num);
}

// ---------------------------------------------------------------------------
// Inquire vacuum on status for nextra function
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsVacuumOnEx(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsVacuumOnEx();
}

// ---------------------------------------------------------------------------
// Inquire vacuum off status for nextra function
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsVacuumOffEx(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsVacuumOffEx();
}

// ---------------------------------------------------------------------------
// Execute shutter open for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioShutterOpenEx(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->ShutterOpenEx(bOn);
}

// ---------------------------------------------------------------------------
// Execute shutter close for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioShutterCloseEx(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->ShutterCloseEx(bOn);
}

// ---------------------------------------------------------------------------
// Execute vacuum on for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioVacuumOnEx(unsigned short valve, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->VacuumOnEx(valve, bOn);
}

// ---------------------------------------------------------------------------
// Execute vacuum destruct for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioVacuumDestructEx(unsigned short valve, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->VacuumDestructEx(valve, bOn);
}

// ---------------------------------------------------------------------------
// Execute air float for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioAirFloatEx(unsigned short valve, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->AirFloatEx(valve, bOn);
}

// ---------------------------------------------------------------------------
// Execute servo on for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioServoOnEx(unsigned short axis, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->ServoOnEx(axis, bOn);
}

// ---------------------------------------------------------------------------
// Execute servo alarm reset for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioServoAlarmResetEx(unsigned short axis, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->ServoAlarmResetEx(axis, bOn);
}

// ---------------------------------------------------------------------------
// Execute servo brake for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioServoBrakeEx(unsigned short axis, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->ServoBrakeEx(axis, bOn);
}

// ---------------------------------------------------------------------------
// Execute pin up for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioPinUpEx(unsigned short num, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->PinUpEx(num, bOn);
}

// ---------------------------------------------------------------------------
// Execute pin alignment for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioPinAlignmentEx(unsigned short num, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->PinAlignmentEx(num, bOn);
}

// ---------------------------------------------------------------------------
// Execute pin down for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioPinDownEx(unsigned short num, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->PinDownEx(num, bOn);
}

// ---------------------------------------------------------------------------
// Execute work guide open for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioWorkGuideOpenEx(unsigned short num, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->WorkGuideOpenEx(num, bOn);
}

// ---------------------------------------------------------------------------
// Execute work guide close for nextra function
// ---------------------------------------------------------------------------
void __stdcall nexioWorkGuideCloseEx(unsigned short num, BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->WorkGuideCloseEx(num, bOn);
}

// 2013.01.10 bagus stage driver alarm io -->
// ---------------------------------------------------------------------------
// Inquire stage alarm status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsStageAlarm(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsStageAlarm();
}

// ---------------------------------------------------------------------------
// Inquire stage driver X1 axis alarm status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsStageDriverX1AxisAlarm(unsigned short num)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsStageDriverX1AxisAlarm(num);
}

// ---------------------------------------------------------------------------
// Inquire stage driver X2 axis alarm status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsStageDriverX2AxisAlarm(unsigned short num)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsStageDriverX2AxisAlarm(num);
}

// ---------------------------------------------------------------------------
// Inquire stage driver Y axis alarm status for nextra fuction
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsStageDriverYAxisAlarm(unsigned short num)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsStageDriverYAxisAlarm(num);
}
// 2013.01.10 bagus stage driver alarm io <--

//==============================================================================================
//	  NexI/O Contact Angle module. (nexioRC_...).
//==============================================================================================

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsCA_Interlock(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsCA_Interlock();
}

// ---------------------------------------------------------------------------
NEXIOAPI void __stdcall nexioCA_Interlock(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->CA_Interlock(bOn);
}

//==============================================================================================
//	  NexI/O Resistance Sheet(4PP) module. (nexioRC_...).
//==============================================================================================

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsRS_ProbeHeadUpperPos(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRS_ProbeHeadUpperPos();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsRS_ProbeHeadLowerPos(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRS_ProbeHeadLowerPos();
}

// ---------------------------------------------------------------------------
NEXIOAPI void __stdcall nexioRS_ProbeHeadUp(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RS_ProbeHeadUp(bOn);
}

// ---------------------------------------------------------------------------
NEXIOAPI void __stdcall nexioRS_ProbeHeadDown(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RS_ProbeHeadDown(bOn);
}

//==============================================================================================
//	  NexI/O Transmittance module. (nexioRC_...).
//==============================================================================================

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsTransmitLightSourcePower(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsTransmitLightSourcePower();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsTransmitLight1Condition(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsTransmitLight1Condition();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsTransmitLight2Condition(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsTransmitLight2Condition();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsTransmitLight3Condition(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsTransmitLight3Condition();
}

// ---------------------------------------------------------------------------
NEXIOAPI void __stdcall nexioTransmitLightSource1Shutter(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->TransmitLightSource1Shutter(bOn);
}

// ---------------------------------------------------------------------------
NEXIOAPI void __stdcall nexioTransmitLightSource2Shutter(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->TransmitLightSource2Shutter(bOn);
}
// ---------------------------------------------------------------------------
NEXIOAPI void __stdcall nexioTransmitLightSource3Shutter(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->TransmitLightSource3Shutter(bOn);
}

//==============================================================================================
//	  NexI/O Micro Scope module. (nexioRC_...).
//==============================================================================================

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsMS_ModeSwitch(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMS_ModeSwitch();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsMS_UpSwitch(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMS_UpSwitch();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsMS_DownSwitch(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMS_DownSwitch();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsMS_UpperPos(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMS_UpperPos();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsMS_LowerPos1(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMS_LowerPos1();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsMS_LowerPos2(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMS_LowerPos2();
}

// ---------------------------------------------------------------------------
NEXIOAPI BOOL __stdcall nexioIsMS_LensKind(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsMS_LensKind();
}

// ---------------------------------------------------------------------------
NEXIOAPI void __stdcall nexioMS_ModeLamp(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->MS_ModeLamp(bOn);
}

//==============================================================================================
//	  NexI/O Robo Cylinder module. (nexioRC_...).
//==============================================================================================

unsigned char __stdcall nexioRC_GetCurrentMode(void)
{
	if ( pInterface == NULL ) return RCOPMODE_UNKNOWN;

	return pInterface->RC_GetCurrentMode();
}

BOOL __stdcall nexioRC_GetJISL(void)
{
	if ( pInterface == NULL ) return RCOPMODE_UNKNOWN;

	return pInterface->RC_GetJISL();
}

BOOL __stdcall nexioRC_GetJOGP(void)
{
	if ( pInterface == NULL ) return RCOPMODE_UNKNOWN;

	return pInterface->RC_GetJOGP();
}

BOOL __stdcall nexioRC_GetJOGM(void)
{
	if ( pInterface == NULL ) return RCOPMODE_UNKNOWN;

	return pInterface->RC_GetJOGM();
}

// ---------------------------------------------------------------------------
// The relevant position number is output when positioning has
// completed.
// The signal will turn OFF when the next start signal is received.
// It is used by the PLC to check if the commanded position has
// definitively been reached, and also to provide a position
// interlock, etc.
// ---------------------------------------------------------------------------
unsigned char __stdcall nexioRC_PM(void)
{
	if ( pInterface == NULL ) return 0;

	return pInterface->RC_PM();
}

// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_PM1(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_PM1();
}

// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_PM2(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_PM2();
}

// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_PM4(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_PM4();
}

// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_PM8(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_PM8();
}

// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_PM16(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_PM16();
}

// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_PM32(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_PM32();
}

// ---------------------------------------------------------------------------
// This signal will remain ON while the actuator is moving, and
// OFF while the actuator is standing still.
// It is used to determine whether the actuator is moving or
// paused.
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_MOVE(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_MOVE();
}

// ---------------------------------------------------------------------------
// ON: Teaching mode, OFF: Normal mode
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_MODES(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_MODES();
}

// ---------------------------------------------------------------------------
// This signal becomes effective after a position movement
// command is input. It will turn ON when the current actuator
// position enters the range specified in the position table and
// remain ON until the actuator exits the range.
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_PZONE(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_PZONE();
}

// ---------------------------------------------------------------------------
// This signal will remain OFF during the AUTO mode, and ON
// during the MANU mode.
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_RMDS(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_RMDS();
}

// ---------------------------------------------------------------------------
// This signal is OFF immediately after the power is input, and
// turns ON when home return has completed.
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_HEND(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_HEND();
}

// ---------------------------------------------------------------------------
// This signal turns ON when the target position was reached
// and the actuator has entered the specified in-position range.
// It is used to determine whether positioning has completed.
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_PEND(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_PEND();
}

// ---------------------------------------------------------------------------
// This signal is output upon completion of writing to the
// nonvolatile memory in response to a current-position write
// command (PWRT).
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_WEND(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_WEND();
}

// ---------------------------------------------------------------------------
// This signal is always output once the servo is turned ON and
// the controller is ready to operate.
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_SV(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_SV();
}

// ---------------------------------------------------------------------------
// OFF: Emergency stop has been actuated
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_EMGS(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_EMGS();
}
// ---------------------------------------------------------------------------
// This signal remains ON in normal conditions of use and turns
// OFF when an alarm generates.
// ---------------------------------------------------------------------------
BOOL __stdcall nexioIsRC_ALM(void)
{
	if ( pInterface == NULL ) return FALSE;

	return pInterface->IsRC_ALM();
}

// ---------------------------------------------------------------------------
// The target position number is input.
// A command position number must be specified by 6 ms before
// the start signal (CSTR) turns ON.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_PC(unsigned char val)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_PC(val);
}

// ---------------------------------------------------------------------------
void __stdcall nexioRC_PC1(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_PC1(bOn);
}

// ---------------------------------------------------------------------------
void __stdcall nexioRC_PC2(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_PC2(bOn);
}

// ---------------------------------------------------------------------------
void __stdcall nexioRC_PC4(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_PC4(bOn);
}

// ---------------------------------------------------------------------------
void __stdcall nexioRC_PC8(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_PC8(bOn);
}

// ---------------------------------------------------------------------------
void __stdcall nexioRC_PC16(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_PC16(bOn);
}

// ---------------------------------------------------------------------------
void __stdcall nexioRC_PC32(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_PC32(bOn);
}

// ---------------------------------------------------------------------------
// Mode selection (ON: Teaching mode, OFF: Normal mode)
// ---------------------------------------------------------------------------
void __stdcall nexioRC_MODE(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_MODE(bOn);
}

// ---------------------------------------------------------------------------
// OFF: Jog, ON: Inching
// ---------------------------------------------------------------------------
void __stdcall nexioRC_JISL(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_JISL(bOn);
}

// ---------------------------------------------------------------------------
// The actuator will start jogging or inching in the positive
// direction at an ON edge of this signal.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_JOGP(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_JOGP(bOn);
}

// ---------------------------------------------------------------------------
// The actuator will start jogging or inching in the negative
// direction at an ON edge of this signal.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_JOGM(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_JOGM(bOn);
}

// ---------------------------------------------------------------------------
// This signal switches the operating mode between AUTO
// and MANU.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_RMOD(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_RMOD(bOn);
}

// ---------------------------------------------------------------------------
// Home return operation is started at a rise edge of this signal.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_HOME(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_HOME(bOn);
}

// ---------------------------------------------------------------------------
// ON: Actuator can be moved, OFF: Actuator decelerates to a
// stop
// ---------------------------------------------------------------------------
void __stdcall nexioRC_STP(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_STP(bOn);
}

// ---------------------------------------------------------------------------
// The actuator will start moving at a rise edge of this signal.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_CSTR(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_CSTR(bOn);
}

// ---------------------------------------------------------------------------
// When this signal has remained ON for 20 msec or longer, the
// current position will be stored under the position number
// selected by PC1 to PC32.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_PWRT(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_PWRT(bOn);
}

// ---------------------------------------------------------------------------
// An alarm is reset at a rise edge of this signal.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_RES(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_RES(bOn);
}

// ---------------------------------------------------------------------------
// The servo remains ON while this signal is ON.
// The servo remains OFF while this signal is OFF.
// ---------------------------------------------------------------------------
void __stdcall nexioRC_SON(BOOL bOn)
{
	if ( pInterface == NULL ) return;

	pInterface->RC_SON(bOn);
}
