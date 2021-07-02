#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#define _NEXIFDLL_

#include <windows.h>
#include <alarmif.hxx>
#include <nexmisc.hxx>
#define  __NEXTRA_INTERNAL
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) -->
#include <nexini.hxx>
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) <--
#include <nexif.hxx>

#pragma data_seg("Shared")
double	SampleSizeX = 0.0;
double	SampleSizeY = 0.0;
#pragma data_seg()

#pragma comment(linker, "/Section:Shared,RWS")

// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) -->
#ifdef _DEBUG
#pragma comment(lib, "dtnsnexini.lib")
#else
#pragma comment(lib, "tnsnexini.lib")
#endif
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) <--

HINSTANCE hInstance = NULL;
HHOOK	  hHook 	= NULL;
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) -->
NEXINIPARAMS	params;
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) <--

using namespace nano;

// ===========================================================================
// NEXIF local helper function declarations
// ===========================================================================

static BOOL nexifPostMessage(UINT msg, WPARAM wp = 0L, LPARAM lp = 0L);
static BOOL nexifSendMessage(UINT msg, WPARAM wp = 0L, LPARAM lp = 0L);

/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- } ---------- */
BOOL APIENTRY DllMain( HANDLE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch ( ul_reason_for_call ) {
	case DLL_PROCESS_ATTACH:
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("NEXIF"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- } ---------- */
		hInstance = (HINSTANCE) hModule;
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) -->
		nexiniGetParameter(&params);
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) <--
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- } ---------- */
		break;
	}
	return TRUE;
}

// ===========================================================================
// Request functions
// ===========================================================================

// ---------------------------------------------------------------------------
// Initialize (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifInitialize(HWND client)
{
	// send message to nextra interface application
	return nexifSendMessage(WM_NEX_INITIALIZE, (WPARAM) client);
}

// ---------------------------------------------------------------------------
//	Return to origin (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifReturnToOrigin(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_RETURN_TO_ORIGIN, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Load (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifLoad(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_LOAD, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Unload (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifUnload(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_UNLOAD, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Move to upper (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifMoveToUpper(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_MOVE_TO_UPPER, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Move to alignment (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifMoveToAlignment(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_MOVE_TO_ALIGNMENT, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Move to lower (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifMoveToLower(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_MOVE_TO_LOWER, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Open work guide (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifOpenWorkGuide(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_OPEN_WORK_GUIDE, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Close work guide (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifCloseWorkGuide(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_CLOSE_WORK_GUIDE, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Open shutter (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifOpenShutter(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_OPEN_SHUTTER, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Close shutter (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifCloseShutter(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_CLOSE_SHUTTER, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Vacuum on (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifVacuumOn(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_VACUUM_ON, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Vacuum off (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifVacuumOff(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_VACUUM_OFF, (WPARAM) client);

	return TRUE;
}

// ---------------------------------------------------------------------------
// Air float on (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifAirFloatOn(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_AIR_FLOAT_ON, (WPARAM) client);
}

// ---------------------------------------------------------------------------
// Air float off (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifAirFloatOff(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_AIR_FLOAT_OFF, (WPARAM) client);

	return TRUE;
}

// ---------------------------------------------------------------------------
// Set sample size
// ---------------------------------------------------------------------------
BOOL __stdcall nexifSetSampleSize(double x, double y)
{
	SampleSizeX = x; SampleSizeY = y;

	return TRUE;
}

// ---------------------------------------------------------------------------
// Get sample size
// ---------------------------------------------------------------------------
BOOL __stdcall nexifGetSampleSize(double* x, double* y)
{
	*x = SampleSizeX; *y = SampleSizeY;

	return TRUE;
}

// ---------------------------------------------------------------------------
// Cancel (client HWND required for reply message(s)
// ---------------------------------------------------------------------------
BOOL __stdcall nexifCancel(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_CANCEL, (WPARAM) client);
}

// --------------------------------------------------------------------
// nexifAlarmAllReset
BOOL __stdcall nexifAlarmAllReset(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_ALARM_ALL_RESET, (WPARAM) client);
}

// --------------------------------------------------------------------
// nexifRC_MoveToTeachPos
BOOL __stdcall nexifRC_MoveToTeachPos(HWND client, unsigned char num)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_RC_MOVE_TO_TEACH_POS, (WPARAM) client, (LPARAM) num);
}

// --------------------------------------------------------------------
// nexifRC_JogPlus
BOOL __stdcall nexifRC_JogPlus(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_RC_JOG_PLUS, (WPARAM) client);
}

// --------------------------------------------------------------------
// nexifRC_JogMinus
BOOL __stdcall nexifRC_JogMinus(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_RC_JOG_MINUS, (WPARAM) client);
}

// --------------------------------------------------------------------
// nexifRC_JogStop
BOOL __stdcall nexifRC_JogStop(HWND client)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_RC_JOG_STOP, (WPARAM) client);
}

// --------------------------------------------------------------------
// nexifRC_SelectMode
BOOL __stdcall nexifRC_SelectMode(HWND client, unsigned char mode)
{
	// send message to nextra interface application
	return nexifPostMessage(WM_NEX_RC_SELECT_MODE, (WPARAM) client, (LPARAM) mode);
}

// ===========================================================================
// FOR NEXTRA
// ===========================================================================

// ---------------------------------------------------------------------------
// Alarm set
// ---------------------------------------------------------------------------
void __stdcall nexifAlarmSet(unsigned short alid)
{
	// send message to nextra interface application
	nexifPostMessage(WM_NEX_ALARM_SET, (WPARAM) alid);
}

// ---------------------------------------------------------------------------
// Alarm reset
// ---------------------------------------------------------------------------
void __stdcall nexifAlarmReset(unsigned short alid)
{
	// send message to nextra interface application
	nexifPostMessage(WM_NEX_ALARM_RESET, (WPARAM) alid);
}

// ===========================================================================
// NEXIF local helper function declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// Post a message to the resistor task, check for errors
// ---------------------------------------------------------------------------
static BOOL nexifPostMessage(UINT msg, WPARAM wp, LPARAM lp)
{
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) -->
	if(params.System.Type == NEXTRA_TYPEE){
		return TRUE;
	}
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) <--

	HWND hwnd = ::FindWindow(NULL, NEXTRA_WINDOW_NAME);
	if ( hwnd != NULL ) {
		// send it
		return ::PostMessage(hwnd, msg, wp, lp);
	}
	else {
		return FALSE;
	}
}

// ---------------------------------------------------------------------------
// Send a message to the resistor task, check for errors
// ---------------------------------------------------------------------------
static BOOL nexifSendMessage(UINT msg, WPARAM wp, LPARAM lp)
{
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) -->
	if(params.System.Type == NEXTRA_TYPEE){
		return TRUE;
	}
// 2013.11.14 Bagus Add (TohoSpec‘Î‰ž) <--

	HWND hwnd = ::FindWindow(NULL, NEXTRA_WINDOW_NAME);
	if ( hwnd != NULL ) {
		// send it
		return ::SendMessage(hwnd, msg, wp, lp);
	}
	else {
		return FALSE;
	}
}
