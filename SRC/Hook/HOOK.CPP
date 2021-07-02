#define WIN32_LEAN_AND_MEAN

#define _HOOKDLL_

#include <windows.h>
#include "..\\..\\INC\\hook.hxx"
#include <tchar.h>
#include <string>
#include <crtdbg.h>
//Saiki 20090604 Add ----->
#include <ConfigFile.hxx>
//Saiki 20090604 Add <-----

HINSTANCE hInstance;
#pragma data_seg("SHARE_DATA")
HHOOK hKeyHook = NULL;
HHOOK hMouseHook = NULL;
#pragma data_seg()
//Saiki 20090604 Change ----->
//PCSTR pszWindowTable[] =
//{
//	  "NanoSpec",
//	  "マニュアルステージ操作",
//	  "POINT SETUP - TEACHING",
//	  "DESKEW SETUP",
//	  "Teach Location",
//	  "Jog Operation",
//	  "位置決め",
//	  "ジョグ操作",
//	  NULL,
//};
PCSTR pszWindowTable_JPN[] =
{
//	"NanoSpec",
	"マニュアルステージ操作",
	"POINT SETUP - TEACHING",
	"DESKEW SETUP",
	"Teach Location",
	"Jog Operation",
	"位置決め",
	"ジョグ操作",
// 2009.06.10 K.Matsuo -->
	"オートフォーカス",
	"オートフォーカス設定 & ポジション補正",
	"オートフォーカス設定",
	"ダークリファレンス位置設定",
	"デスキュー設定",
	"GENERAL POSITION SETTING CAPTION",
	"ハンドラー・ロードポジション セッティング",
	"LENS CENTER OFFSET SETTING",
	"メンテナンスポジション設定",
	"マニュアル デスキュー 設定",
	"手動測定 - ポイントティーチング",
	"マニュアル サイトパターン設定",
	"マニュアルロードポジション設定",
	"ポイント設定 - ティーチング",
	"ポジション補正",
	"反射絶対リファレンス位置設定",
	"サンプル設定",
	"サイトパターン設定",
	"第１透過リファレンス位置設定",
	"第２透過リファレンス位置設定",
	"透過絶対リファレンス位置設定",
	"VIDEO CALIBRATION SETUP",
// 2009.06.10 K.Matsuo <--
	"２点間距離測定位置設定",
	"ガントリー透過リファレンス位置設定",
	"ステージオフセット - ティーチング",
	"測定ポイント確認",
	NULL,
};
PCSTR pszWindowTable_ENU[] =
{
//	"NanoSpec",
	"MANUAL STAGE",
	"POINT SETUP - TEACHING",
	"DESKEW SETUP",
	"Teach Location",
	"Jog Operation",
	"Teach Location",
	"Jog Operation",
// 2009.06.10 K.Matsuo -->
	"AUTO FOCUS",
	"AUTOFOCUS SETTING & POSITION ADJUST",
	"AUTOFOCUS SETTING",
	"DARK REFERENCE POSITION SETTING",
	"Deskew Setting",
	"GENERAL POSITION SETTING CAPTION",
	"HANDLER LOAD POSITION SETTING",
	"LENS CENTER OFFSET SETTING",
	"MAINTENANCE POSITION SETTING",
	"MANUAL DESKEW SETUP",
	"MANUAL MEASUREMENT - POINT TEACHING",
	"MANUAL SITE PATTERN SETUP",
	"MANUAL LOAD POSITION SETTING",
	"POINT SETUP - TEACHING",
	"POSITION ADJUST",
	"REFLECT ABSOLUTE REFERENCE POSITION SETTING",
	"SAMPLE SETTING",
	"Site Pattern Setting",
	"TRANSMIT 1ST REFERENCE POSITION SETTING",
	"TRANSMIT 2ND REFERENCE POSITION SETTING",
	"TRANSMIT ABSOLUTE REFERENCE POSITION SETTING",
	"VIDEO CALIBRATION SETUP",
// 2009.06.10 K.Matsuo <--
	"2POINT DISTANCE MEASUREMENT POSITION SETTING",
	"GANTRY TRANSMIT REFERENCE POSITION SETTING",
	"STAGE OFFSET - TEACHING",
	"MEASUREMENT POINT - CONFIRMATION",
	NULL,
};

//Saiki 20090604 Change <-----

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
		if (FALSE == DllMutexCreate(_T("Hook"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		hInstance = (HINSTANCE) hModule;
		//Saiki 20090604 Add ----->
		ConfigFile_LoadAllNanoSpecIni();
		//Saiki 20090604 Add <-----
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

// --------------------------------------------------------------------
// KeyboardProc
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//Saiki 20090604 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090604 Add <-----
	if ( nCode == HC_ACTION ) {
		HWND hWndNotify;
		//Saiki 20090604 Change ----->
//		  for ( int index = 0; pszWindowTable[index] != NULL; index++ ) {
//			  hWndNotify = ::FindWindow(NULL, pszWindowTable[index]);
//			  if ( hWndNotify != NULL ) {
//				  ::SendMessage(hWndNotify, WM_KEYHOOK, (WPARAM) wParam, (LPARAM) lParam);
//			  }
//		  }
		if(l_SystemConfig.nLanguage == 0){
			for ( int index = 0; pszWindowTable_ENU[index] != NULL; index++ ) {
				hWndNotify = ::FindWindow(NULL, pszWindowTable_ENU[index]);
				if ( hWndNotify != NULL ) {
					::SendMessage(hWndNotify, WM_KEYHOOK, (WPARAM) wParam, (LPARAM) lParam);
				}
			}
		}
		else{
			for ( int index = 0; pszWindowTable_JPN[index] != NULL; index++ ) {
				hWndNotify = ::FindWindow(NULL, pszWindowTable_JPN[index]);
				if ( hWndNotify != NULL ) {
					::SendMessage(hWndNotify, WM_KEYHOOK, (WPARAM) wParam, (LPARAM) lParam);
				}
			}
		}

		//Saiki 20090604 Change <-----
	}
	return (CallNextHookEx(NULL, nCode, wParam, lParam));
}

// --------------------------------------------------------------------
// HookKeySet
BOOL __stdcall HookKeySet(void)
{
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD,
				(HOOKPROC) KeyboardProc, hInstance, 0);
	return hKeyHook != NULL;
}

// --------------------------------------------------------------------
// HookKeyUnhook
BOOL __stdcall HookKeyUnhook(void)
{
	return UnhookWindowsHookEx(hKeyHook);
}

// --------------------------------------------------------------------
// MouseProc
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//Saiki 20090604 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090604 Add <-----
	if ( nCode >= 0 ) {
		HWND hWndNotify;
		//Saiki 20090604 Change ----->
//		  for ( int index = 0; pszWindowTable[index] != NULL; index++ ) {
//			  hWndNotify = ::FindWindow(NULL, pszWindowTable[index]);
//			  if ( hWndNotify != NULL ) {
//				  ::SendMessage(hWndNotify, WM_MOUSEHOOK, (WPARAM) wParam, (LPARAM) lParam);
//				  break;
//			  }
//		  }
		if(l_SystemConfig.nLanguage == 0){
			for ( int index = 0; pszWindowTable_ENU[index] != NULL; index++ ) {
				hWndNotify = ::FindWindow(NULL, pszWindowTable_ENU[index]);
				if ( hWndNotify != NULL ) {
					::SendMessage(hWndNotify, WM_MOUSEHOOK, (WPARAM) wParam, (LPARAM) lParam);
					break;
				}
			}
		}
		else{
			for ( int index = 0; pszWindowTable_JPN[index] != NULL; index++ ) {
				hWndNotify = ::FindWindow(NULL, pszWindowTable_JPN[index]);
				if ( hWndNotify != NULL ) {
					::SendMessage(hWndNotify, WM_MOUSEHOOK, (WPARAM) wParam, (LPARAM) lParam);
					break;
				}
			}
		}

		//Saiki 20090604 Change <-----
	}
	return (CallNextHookEx(NULL, nCode, wParam, lParam));
}

// --------------------------------------------------------------------
// HookMouseSet
BOOL __stdcall HookMouseSet(void)
{
	hMouseHook = SetWindowsHookEx(WH_MOUSE,
				(HOOKPROC) MouseProc, hInstance, 0);
	return hMouseHook != NULL;
}

// --------------------------------------------------------------------
// HookMouseUnhook
BOOL __stdcall HookMouseUnhook(void)
{
	return UnhookWindowsHookEx(hMouseHook);
}
