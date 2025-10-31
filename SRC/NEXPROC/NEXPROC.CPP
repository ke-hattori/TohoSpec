#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#define _NEXPROCDLL_

#include <windows.h>
#include <process.h>
#include <crtdbg.h>
#define  __NEXTRA_INTERNAL
#include <NEXIOBASE.HXX>
#include <NEXAX.HXX>
#include <NEXRC.HXX>
#include <NEXPROC.HXX>
/* added 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- { ---------- */
#define	NEXPROC_MAS
/* added 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- } ---------- */
#include "coord.hpp"
#include "typea.hpp"
#include "typeb.hpp"

// matsuhisa 2011.09.13 add >>>
typedef cTypeB cTypeC;
// matsuhias 2011.09.13 add <<<
//Saiki 20130812 Add ----->
typedef cTypeC cTypeD;
//Saiki 20130812 Add <-----
cCoord* 		pCoord = NULL;
NEXINIPARAMS	params;

/* added 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- { ---------- */
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
/* added 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- } ---------- */
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
		if (FALSE == DllMutexCreate(_T("NEXPROC"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		_RPTF0(_CRT_WARN, "NEXPROC - DLL_PROCESS_ATTACH\n");

		if ( pCoord == NULL ) {
			nexiniGetParameter(&params);
			switch ( params.System.Type ) {
			case NEXTRA_TYPEA:
				pCoord = (cCoord*) new cTypeA();
				break;
			case NEXTRA_TYPEB:
				pCoord = (cCoord*) new cTypeB();
				break;
// matsuhisa 2011.09.13 add >>>
			case NEXTRA_TYPEC:
				pCoord = (cCoord*) new cTypeC();
				break;
// matsuhisa 2011.09.13 add <<<
//Saiki 20130812 Add ----->
			case NEXTRA_TYPED:
				pCoord = (cCoord*) new cTypeD();
				break;
//Saiki 20130812 Add <-----
// 2013.11.07 Bagus Add (TohoSpec対応) -->
			case NEXTRA_TYPEE:
				::MessageBox(NULL, "Cannot call this program by a setting contents of T3100.", "NEXTRA", MB_OK | MB_ICONWARNING);
				break;
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			default:
				_ASSERT(FALSE);
				break;
			}
/* added 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- { ---------- */
			if (0 != pCoord) {
				GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
				_stprintf(g_tszLogPath, _T("%s") _T(LOG_DIR) _T("NEXCOORD.LOG"), g_tszProcDir);
			}
/* added 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- } ---------- */
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if ( pCoord != NULL ) {
			delete pCoord;
			pCoord = NULL;
		}
		_RPTF0(_CRT_WARN, "NEXPROC - DLL_PROCESS_DETACH\n");
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		break;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Cancel
// ---------------------------------------------------------------------------
void __stdcall nexprocCancel(void)
{
	_ASSERT(pCoord != NULL);

	pCoord->Cancel();
}

// ---------------------------------------------------------------------------
// Initialize
// ---------------------------------------------------------------------------
LRESULT __stdcall nexprocInitialize(void)
{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if ( params.System.Type == NEXTRA_TYPEE ) {
		return 0L;
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	_ASSERT(pCoord != NULL);

	return pCoord->Initialize();
}

// ---------------------------------------------------------------------------
// Finalize
// ---------------------------------------------------------------------------
void __stdcall nexprocFinalize(void)
{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if ( params.System.Type == NEXTRA_TYPEE ) {
		return;
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	_ASSERT(pCoord != NULL);

	pCoord->Finalize();
}

// ---------------------------------------------------------------------------
// Require Idle status
// ---------------------------------------------------------------------------
BOOL __stdcall nexprocIsIdle(void)
{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if ( params.System.Type == NEXTRA_TYPEE ) {
		return TRUE;
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	_ASSERT(pCoord != NULL);

	return pCoord->IsIdle();
}

// ---------------------------------------------------------------------------
// Return To Origin
// ---------------------------------------------------------------------------
void __stdcall nexprocReturnToOrigin(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->ReturnToOrigin(hwnd);
}

// ---------------------------------------------------------------------------
// Load
// ---------------------------------------------------------------------------
void __stdcall nexprocLoad(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->Load(hwnd);
}

// ---------------------------------------------------------------------------
// Unload
// ---------------------------------------------------------------------------
void __stdcall nexprocUnload(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->Unload(hwnd);
}

// ---------------------------------------------------------------------------
// Move To Upper
// ---------------------------------------------------------------------------
void __stdcall nexprocMoveToUpper(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->MoveToUpper(hwnd);
}

// ---------------------------------------------------------------------------
// Move To Alignment
// ---------------------------------------------------------------------------
void __stdcall nexprocMoveToAlignment(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->MoveToAlignment(hwnd);
}

// ---------------------------------------------------------------------------
// Move To Lower
// ---------------------------------------------------------------------------
void __stdcall nexprocMoveToLower(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->MoveToLower(hwnd);
}

// ---------------------------------------------------------------------------
// Open Work Guide
// ---------------------------------------------------------------------------
void __stdcall nexprocOpenWorkGuide(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->OpenWorkGuide(hwnd);
}

// ---------------------------------------------------------------------------
// Close Work Guide
// ---------------------------------------------------------------------------
void __stdcall nexprocCloseWorkGuide(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->CloseWorkGuide(hwnd);
}

// ---------------------------------------------------------------------------
// Open Shutter
// ---------------------------------------------------------------------------
void __stdcall nexprocOpenShutter(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->OpenShutter(hwnd);
}

// ---------------------------------------------------------------------------
// Close Shutter
// ---------------------------------------------------------------------------
void __stdcall nexprocCloseShutter(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->CloseShutter(hwnd);
}

// ---------------------------------------------------------------------------
// Vacuum On
// ---------------------------------------------------------------------------
void __stdcall nexprocVacuumOn(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->VacuumOn(hwnd);
}

// ---------------------------------------------------------------------------
// Vacuum Off
// ---------------------------------------------------------------------------
void __stdcall nexprocVacuumOff(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->VacuumOff(hwnd);
}

// ---------------------------------------------------------------------------
// Air Float On
// ---------------------------------------------------------------------------
void __stdcall nexprocAirFloatOn(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->AirFloatOn(hwnd);
}

// ---------------------------------------------------------------------------
// Air Float Off
// ---------------------------------------------------------------------------
void __stdcall nexprocAirFloatOff(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->AirFloatOff(hwnd);
}

// ---------------------------------------------------------------------------
// Robo Cylinder Move To Teaching Position
// ---------------------------------------------------------------------------
void __stdcall nexprocRC_MoveToTeachPos(HWND hwnd, unsigned char num)
{
	_ASSERT(pCoord != NULL);

	pCoord->RC_MoveToTeachPos(hwnd, num);
}

// ---------------------------------------------------------------------------
// Robo Cylinder Jogging Plus
// ---------------------------------------------------------------------------
void __stdcall nexprocRC_JogPlus(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->RC_JogPlus(hwnd);
}

// ---------------------------------------------------------------------------
// Robo Cylinder Jogging Minus
// ---------------------------------------------------------------------------
void __stdcall nexprocRC_JogMinus(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->RC_JogMinus(hwnd);
}

// ---------------------------------------------------------------------------
// Robo Cylinder Jogging Stop
// ---------------------------------------------------------------------------
void __stdcall nexprocRC_JogStop(HWND hwnd)
{
	_ASSERT(pCoord != NULL);

	pCoord->RC_JogStop(hwnd);
}

// ---------------------------------------------------------------------------
// Robo Cylinder Select Mode
// ---------------------------------------------------------------------------
void __stdcall nexprocRC_SelectMode(HWND hwnd, unsigned char mode)
{
	_ASSERT(pCoord != NULL);

	pCoord->RC_SelectMode(hwnd, mode);
}
