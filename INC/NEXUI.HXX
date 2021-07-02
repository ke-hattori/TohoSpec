#if !defined( _NEXUI_HXX_ )
#define _NEXUI_HXX_

#pragma once

#include <globals.hxx>

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _NEXUIDLL_ )
	#define NEXUIAPI __declspec(dllimport)
#else
	#define NEXUIAPI __declspec(dllexport)
#endif

#define NEXPASS_STRING_LENGTH			(12)
#define NEXPASS_INIFILE_NAME			CFG_SYSTEM_DIR "password.ini"
#define NEXPASS_DEBUG					"toho"

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================
NEXUIAPI int  __stdcall nexuiMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

NEXUIAPI BOOL __stdcall nexuiPasswordConfirm(HWND client);
NEXUIAPI BOOL __stdcall nexuiPasswordConfig(HWND client);

#ifdef __cplusplus
}
#endif

#endif // _NEXPASS_HXX_
