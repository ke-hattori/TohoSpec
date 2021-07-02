#if !defined( _NEXPROC_HXX_ )
#define _NEXPROC_HXX_

#pragma once

#include <iostream>

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _NEXPROCDLL_ )
	#define NEXPROCAPI __declspec(dllimport)
#else
	#define NEXPROCAPI __declspec(dllexport)
#endif

// ピン上昇に伴うイベント時にエラーとしない
// date : 2009.10.06
#define __NEXPROC_NO_PIN_UP_CHECK

#ifdef __NEXTRA_INTERNAL

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================

NEXPROCAPI void __stdcall nexprocCancel (void);

NEXPROCAPI LRESULT __stdcall nexprocInitialize (void);
NEXPROCAPI void __stdcall nexprocFinalize(void);
NEXPROCAPI void __stdcall nexprocReturnToOrigin (HWND);
NEXPROCAPI void __stdcall nexprocLoad (HWND);
NEXPROCAPI void __stdcall nexprocUnload (HWND);
NEXPROCAPI void __stdcall nexprocMoveToUpper (HWND);
NEXPROCAPI void __stdcall nexprocMoveToAlignment (HWND);
NEXPROCAPI void __stdcall nexprocMoveToLower (HWND);
NEXPROCAPI void __stdcall nexprocOpenWorkGuide (HWND);
NEXPROCAPI void __stdcall nexprocCloseWorkGuide (HWND);
NEXPROCAPI void __stdcall nexprocOpenShutter (HWND);
NEXPROCAPI void __stdcall nexprocCloseShutter (HWND);
NEXPROCAPI void __stdcall nexprocVacuumOn (HWND);
NEXPROCAPI void __stdcall nexprocVacuumOff (HWND);
NEXPROCAPI void __stdcall nexprocAirFloatOn (HWND);
NEXPROCAPI void __stdcall nexprocAirFloatOff (HWND);

NEXPROCAPI void __stdcall nexprocRC_MoveToTeachPos(HWND, unsigned char);
NEXPROCAPI void __stdcall nexprocRC_JogPlus(HWND);
NEXPROCAPI void __stdcall nexprocRC_JogMinus(HWND);
NEXPROCAPI void __stdcall nexprocRC_JogStop(HWND);
NEXPROCAPI void __stdcall nexprocRC_SelectMode (HWND, unsigned char);
NEXPROCAPI void __stdcall nexprocRC_AlarmReset (void);

NEXPROCAPI BOOL __stdcall nexprocIsIdle (void);

#endif // __NEXTRA_INTERNAL

#ifdef __cplusplus
}
#endif

#endif // _NEXPROC_HXX_
