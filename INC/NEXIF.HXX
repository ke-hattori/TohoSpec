#if !defined( _NEXIF_HXX_ )
#define _NEXIF_HXX_

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _NEXIFDLL_ )
	#define NEXIFAPI __declspec(dllimport)
#else
	#define NEXIFAPI __declspec(dllexport)
#endif

enum NextraEventsEnum
{
	nexInitialize,
	nexCancel,
	nexReturnToOrigin,
	nexLoad,
	nexUnload,
	nexMoveToUpper,
	nexMoveToAlignment,
	nexMoveToLower,
	nexOpenWorkGuide,
	nexCloseWorkGuide,
	nexOpenShutter,
	nexCloseShutter,
	nexVacuumOn,
	nexVacuumOff,
	nexAirFloatOn,
	nexAirFloatOff,
	nexSetSampleSize,
	nexAlarmAllReset,
	nexRC_MoveToTeachPos,
	nexRC_JogPlus,
	nexRC_JogMinus,
	nexRC_JogStop,
	nexRC_SelectMode,
};

enum RCOPMODE
{
	RCOPMODE_UNKNOWN = 0,
	RCOPMODE_NORMAL,
	RCOPMODE_JOG,
	RCOPMODE_INCHING,
};

// ===========================================================================
// EXPORTED FUNCTION PROTOTYPES
// ===========================================================================
NEXIFAPI BOOL __stdcall nexifCancel(HWND client);
NEXIFAPI BOOL __stdcall nexifInitialize(HWND client);

NEXIFAPI BOOL __stdcall nexifReturnToOrigin(HWND client);
NEXIFAPI BOOL __stdcall nexifLoad(HWND client);
NEXIFAPI BOOL __stdcall nexifUnload(HWND client);
NEXIFAPI BOOL __stdcall nexifMoveToUpper(HWND client);
NEXIFAPI BOOL __stdcall nexifMoveToAlignment(HWND client);
NEXIFAPI BOOL __stdcall nexifMoveToLower(HWND client);
NEXIFAPI BOOL __stdcall nexifOpenWorkGuide(HWND client);
NEXIFAPI BOOL __stdcall nexifCloseWorkGuide(HWND client);
NEXIFAPI BOOL __stdcall nexifOpenShutter(HWND client);
NEXIFAPI BOOL __stdcall nexifCloseShutter(HWND client);
NEXIFAPI BOOL __stdcall nexifVacuumOn(HWND client);
NEXIFAPI BOOL __stdcall nexifVacuumOff(HWND client);
NEXIFAPI BOOL __stdcall nexifAirFloatOn(HWND client);
NEXIFAPI BOOL __stdcall nexifAirFloatOff(HWND client);

NEXIFAPI BOOL __stdcall nexifSetSampleSize(double x, double y);
NEXIFAPI BOOL __stdcall nexifGetSampleSize(double* x, double* y);

NEXIFAPI BOOL __stdcall nexifAlarmAllReset(HWND client);

NEXIFAPI BOOL __stdcall nexifRC_MoveToTeachPos(HWND client, unsigned char num);
NEXIFAPI BOOL __stdcall nexifRC_JogPlus(HWND client);
NEXIFAPI BOOL __stdcall nexifRC_JogMinus(HWND client);
NEXIFAPI BOOL __stdcall nexifRC_JogStop(HWND client);
NEXIFAPI BOOL __stdcall nexifRC_SelectMode(HWND client, unsigned char mode);

// Nextra Application BASE
#define  WM_USER_NEX						(WM_APP + 0x0200)

// ---------------------------------------------------------------------------
// nexifInitialize, nexifReturnToOrigin, nexifLoad, nexifUnload,
// nexifMoveToUpper, nexifMoveToAlignment, nexifMoveToLower,
// nexifOpenWorkGuide, nexifCloseWorkGuide, nexifOpenShutter,
// nexifClooseShutter, nexifVacuumOn, nexifVacuumOff
// completed
// ---------------------------------------------------------------------------
// wParam: bOk
// lParam: not used
// ---------------------------------------------------------------------------
#define WM_NEX_RESPONSE 					(WM_USER_NEX + 0x00)

// ---------------------------------------------------------------------------
// Window name
// ---------------------------------------------------------------------------
#define NEXTRA_WINDOW_NAME					"NextraWindow"
#define NEXTRA_EXE_NAME 					"Nextra.exe"

// ===========================================================================
// Internal Messages (nexif.dll, nextra.exe)
// ===========================================================================
#ifdef __NEXTRA_INTERNAL

// ---------------------------------------------------------------------------
// nexifInitialize
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_INITIALIZE					(WM_USER_NEX + 0x01)

// ---------------------------------------------------------------------------
// nexifCancel
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_CANCEL						(WM_USER_NEX + 0x02)

// ---------------------------------------------------------------------------
// nexifReturnToOrigin
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_RETURN_TO_ORIGIN 			(WM_USER_NEX + 0x03)

// ---------------------------------------------------------------------------
// nexifLoad
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_LOAD 						(WM_USER_NEX + 0x04)

// ---------------------------------------------------------------------------
// nexifUnload
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_UNLOAD						(WM_USER_NEX + 0x05)

// ---------------------------------------------------------------------------
// nexifMoveToUpper
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_MOVE_TO_UPPER				(WM_USER_NEX + 0x06)

// ---------------------------------------------------------------------------
// nexifMoveToAlignment
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_MOVE_TO_ALIGNMENT			(WM_USER_NEX + 0x07)

// ---------------------------------------------------------------------------
// nexifMoveToLower
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_MOVE_TO_LOWER				(WM_USER_NEX + 0x08)

// ---------------------------------------------------------------------------
// nexifOpenWorkGuide
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_OPEN_WORK_GUIDE				(WM_USER_NEX + 0x09)

// ---------------------------------------------------------------------------
// nexifCloseWorkGuide
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_CLOSE_WORK_GUIDE 			(WM_USER_NEX + 0x0A)

// ---------------------------------------------------------------------------
// nexifOpenShutter
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_OPEN_SHUTTER 				(WM_USER_NEX + 0x0B)

// ---------------------------------------------------------------------------
// nexifClooseShutter
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_CLOSE_SHUTTER				(WM_USER_NEX + 0x0C)

// ---------------------------------------------------------------------------
// nexifVacuumOn
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_VACUUM_ON					(WM_USER_NEX + 0x0D)

// ---------------------------------------------------------------------------
// nexifVacuumOff
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_VACUUM_OFF					(WM_USER_NEX + 0x0E)

// ---------------------------------------------------------------------------
// nexifAirFloatOn
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_AIR_FLOAT_ON 				(WM_USER_NEX + 0x0F)

// ---------------------------------------------------------------------------
// nexifAirFloatOn
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_AIR_FLOAT_OFF				(WM_USER_NEX + 0x10)

// ---------------------------------------------------------------------------
// nexifAlarmReset
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_ALARM_ALL_RESET				(WM_USER_NEX + 0x11)

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES FOR NEXTRA
// ====================================================================
NEXIFAPI void __stdcall nexifAlarmSet(unsigned short alid);
NEXIFAPI void __stdcall nexifAlarmReset(unsigned short alid);

// ---------------------------------------------------------------------------
// nexifAlarmSet
// ---------------------------------------------------------------------------
// wParam: unsigned short alarm id
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_ALARM_SET					(WM_USER_NEX + 0x20)

// ---------------------------------------------------------------------------
// nexifAlarmReset
// ---------------------------------------------------------------------------
// wParam: unused
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_ALARM_RESET					(WM_USER_NEX + 0x21)

// ---------------------------------------------------------------------------
// nexifRC_MoveToTeachPos
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unsigned char num
// ---------------------------------------------------------------------------
#define WM_NEX_RC_MOVE_TO_TEACH_POS 		(WM_USER_NEX + 0x30)

// ---------------------------------------------------------------------------
// nexifRC_JogPlus
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_RC_JOG_PLUS					(WM_USER_NEX + 0x31)

// ---------------------------------------------------------------------------
// nexifRC_JogMinus
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_RC_JOG_MINUS 				(WM_USER_NEX + 0x32)

// ---------------------------------------------------------------------------
// nexifRC_JogStop
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unused
// ---------------------------------------------------------------------------
#define WM_NEX_RC_JOG_STOP					(WM_USER_NEX + 0x33)

// ---------------------------------------------------------------------------
// nexifRC_SelectMode
// ---------------------------------------------------------------------------
// wParam: HWND client
// lParam: unsigned char mode
// ---------------------------------------------------------------------------
#define WM_NEX_RC_SELECT_MODE				(WM_USER_NEX + 0x34)

#endif // __NEXTRA_INTERNAL

// all is fine
#define NEXIF_OK							(1)

#ifdef __cplusplus
}
#endif

#endif // _NEXIF_HXX_
