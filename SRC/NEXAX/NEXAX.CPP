#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#define _NEXAXDLL_

#include <windows.h>
#include <vector>
#define __NEXTRA_INTERNAL
#include <NEXMISC.HXX>
#include <NEXIF.HXX>
#include <NEXIOBASE.HXX>
#include <NEXAX.HXX>
#include <NEXINI.HXX>
#include "sio.h"
#include "adax.h"

#ifdef _DEBUG
#pragma comment(lib, "dtnsnexif.lib")
#pragma comment(lib, "dtnsnexio.lib")
#pragma comment(lib, "dtnsnexini.lib")
#else
#pragma comment(lib, "tnsnexif.lib")
#pragma comment(lib, "tnsnexio.lib")
#pragma comment(lib, "tnsnexini.lib")
#endif

using namespace std;

std::vector<cAdaxServo*> adaxlist;
// ###########################################################################
// NEXADAX local helper function declarations
// ###########################################################################

/* added 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- { ---------- */
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
/* added 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- } ---------- */
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
		if (FALSE == DllMutexCreate(_T("NEXAX"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		_RPTF0(_CRT_WARN, "NEXAX - DLL_PROCESS_ATTACH\n");
/* added 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- { ---------- */
		GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- } ---------- */
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		for ( std::vector<cAdaxServo*>::iterator iter(adaxlist.begin()); iter != adaxlist.end(); iter++ ) {
			delete *iter;
			*iter = NULL;
		}
		_RPTF0(_CRT_WARN, "NEXAX - DLL_PROCESS_DETACH\n");
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		break;
	}
	return TRUE;
}

// ###########################################################################
// Request functions
// ###########################################################################

// ===========================================================================
// Initialize
// ===========================================================================
unsigned long __stdcall nexaxInitialize()
{
	if ( adaxlist.size() == 0 ) {
		for ( unsigned short axis = 0; axis < nexax_axis_max; axis++ ) {
			adaxlist.push_back(new cAdaxServo(axis));
			adaxlist[axis]->Initialize();
		}
	}
	return adaxlist.size() > 0 ? NEXAXERR_NONE : NEXAXERR_INITIALIZE_FAILURE;
}

// ===========================================================================
// Finalize
// ===========================================================================
unsigned long __stdcall nexaxFinalize(void)
{
	std::vector<cAdaxServo*>::iterator iter(adaxlist.begin());
	for ( ; iter != adaxlist.end(); iter++ ) {
		(*iter)->Finalize();
	}

	basic_timer wait;
	wait.delay(25);

	return NEXAXERR_NONE;
}

// ===========================================================================
// Cancel
// ===========================================================================
unsigned long __stdcall nexaxCancel(void)
{
	unsigned long retval = NEXAXERR_NONE;

	std::vector<cAdaxServo*>::iterator iter(adaxlist.begin());
	for ( ; iter != adaxlist.end(); iter++ ) {
		if ( !(*iter)->Cancel() ) {
			(*iter)->AlarmSet(NEXALID_CommunicationFailure);
			retval = NEXAXERR_COMMUNICATION_FAILURE;
		}
	}

	return retval;
}

// ===========================================================================
// WaitForMotionFinished
// ===========================================================================
unsigned long __stdcall nexaxWaitForMotionFinished(unsigned short axis)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->WaitForMotionFinished();
}

// ===========================================================================
// WaitForHomeMotionFinished
// ===========================================================================
unsigned long __stdcall nexaxWaitForHomeMotionFinished(unsigned short axis)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->WaitForHomeMotionFinished();
}

// ===========================================================================
// Move to upper position
// ===========================================================================
unsigned long __stdcall nexaxMoveToUpperPos(unsigned short axis, BOOL wait/*=FALSE*/)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->MoveToUpperPos(wait);
}

// ===========================================================================
// Move to alignment position
// ===========================================================================
unsigned long __stdcall nexaxMoveToAlignmentPos(unsigned short axis, BOOL wait/*=FALSE*/)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->MoveToAlignmentPos(wait);
}

// ===========================================================================
// Move to lower position
// ===========================================================================
unsigned long __stdcall nexaxMoveToLowerPos(unsigned short axis, BOOL wait/*=FALSE*/)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->MoveToLowerPos(wait);
}

// ===========================================================================
// Move to home position
// ===========================================================================
unsigned long __stdcall nexaxMoveToHomePos(unsigned short axis)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->MoveToHomePos();
}


// ===========================================================================
// Move to softhome position
// ===========================================================================
unsigned long __stdcall nexaxMoveToSoftHomePos(unsigned short axis)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->MoveToSoftHomePos();
}

// ===========================================================================
// Jogging normal
// ===========================================================================
unsigned long __stdcall nexaxNormalJog(unsigned short axis, short speed)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->NormalJog(speed);
}

// ===========================================================================
// Jogging pulse
// ===========================================================================
unsigned long __stdcall nexaxPulseJog(unsigned short axis, long pls)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->PulseJog(pls);
}

// ===========================================================================
// Motion stop
// ===========================================================================
unsigned long __stdcall nexaxMotionStop(unsigned short axis)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->MotionStop();
}

// ===========================================================================
// Get current position
// ===========================================================================
unsigned long __stdcall nexaxGetCurrPos(unsigned short axis, long& pos)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->GetCurrPos(pos);
}

// ===========================================================================
// Upload motion params
// ===========================================================================
unsigned long __stdcall nexaxUploadMotionParams(unsigned short axis, motion_params_t* params)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->UploadMotionParams(params);
}

// ===========================================================================
// Upload motion params
// ===========================================================================
unsigned long __stdcall nexaxDownloadMotionParams(unsigned short axis, motion_params_t* params)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->DownloadMotionParams(params);
}

// ===========================================================================
// Inquire status
// ===========================================================================

// ---------------------------------------------------------------------------
BOOL __stdcall nexaxIsInitialized(unsigned short axis)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->IsInitialized();
}

// ---------------------------------------------------------------------------
BOOL __stdcall nexaxIsIdle(unsigned short axis)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->IsIdle();
}

// ---------------------------------------------------------------------------
BOOL __stdcall nexaxIsReady(unsigned short axis)
{
	_ASSERT(adaxlist[axis] != NULL);

	return adaxlist[axis]->IsReady();
}
