#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <crtdbg.h>
#include "..\\..\\INC\\globals.hxx"

#include "mil.h"
#include "patrec.h"
/* added 2009.07.07 hmenjo dll 相対パス対応 PatRec.dll ---------- { ---------- */
#include <stdlib.h>
/* added 2009.07.07 hmenjo dll 相対パス対応 PatRec.dll ---------- } ---------- */

#pragma warning (disable:4786)

#define _PATRECDLL_


/* added 2009.07.07 hmenjo dll 相対パス対応 PatRec.dll ---------- { ---------- */
TCHAR g_tszProcDir[_MAX_PATH];			/* 呼出しプロセスのディレクトリ('\'付き)*/
TCHAR g_tszBaseDir[_MAX_PATH];			/* 基準ディレクトリ('\'付き)*/
/* added 2009.07.07 hmenjo dll 相対パス対応 PatRec.dll ---------- } ---------- */

// ####################################################################
// GLOBAL VARIABLES
// ####################################################################
static CPatRec* pPatRec = NULL;

/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
BOOL APIENTRY DllMain( HANDLE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
// matsuhisa 2009.12.09 added >>>
	static SYSTEM_CONFIG sysConfig;
// matsuhisa 2009.12.09 added <<<

	switch ( ul_reason_for_call ) {
	case DLL_PROCESS_ATTACH:
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("PatRec"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		if ( pPatRec == NULL ) {
// matsuhisa 2009.12.09 modified >>>
//			  pPatRec = new CPatRecMil80();
			ConfigFile_GetNanoSpecIni(&sysConfig, CONFIG_FILE_SYSTEM_CONFIG);
			switch ( sysConfig.nPRMethod ) {
			case PR_METHOD_MIL8_G:
				pPatRec = new GeometricModelFinder();
				break;
			case PR_METHOD_MIL8_M:
				pPatRec = new CPatMatchRec();
				break;
			case PR_METHOD_NONE:
			default:
			pPatRec = new CPatRecMil80();
				break;
			}
// matsuhisa 2009.12.09 added <<<
/* added 2009.07.07 hmenjo dll 相対パス対応 PatRec.dll ---------- { ---------- */
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
/* added 2009.07.07 hmenjo dll 相対パス対応 PatRec.dll ---------- } ---------- */
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if ( pPatRec != NULL ) {
			delete pPatRec;
			pPatRec = NULL;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		break;
	}
	return TRUE;
}

// ====================================================================
// PatRecInit
BOOL __stdcall PatRecInit(void)
{
	return pPatRec->Init();
}

// ====================================================================
// PatRecEnd
BOOL __stdcall PatRecEnd(void)
{
	return pPatRec->End();
}

// ====================================================================
// PatRecTeach
BOOL __stdcall PatRecTeach(LPRECT lpRect)
{
	return pPatRec->Teach(lpRect);
}

// ====================================================================
// PatRecMatch
BOOL __stdcall PatRecMatch(LPDESKEW_RESULT lpDeskewResult)
{
	return pPatRec->Match(lpDeskewResult, TRUE, 1);
}

// ====================================================================
// PatRecSaveModel
BOOL __stdcall PatRecSaveModel(PSTR pszModelName)
{
	return pPatRec->SaveModel(pszModelName);
}

// ====================================================================
// PatRecDrawModel
BOOL __stdcall PatRecDrawModel(HWND hWnd, WORD wSite, PSTR pszModelName)
{
	return pPatRec->DrawModel(hWnd, wSite, pszModelName);
}

// ====================================================================
// PatRecRestoreModel
BOOL __stdcall PatRecRestoreModel(PSTR pszModelName)
{
	return pPatRec->RestoreModel(pszModelName);
}
