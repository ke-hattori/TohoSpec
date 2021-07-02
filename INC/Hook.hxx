#if !defined( _HOOK_HXX_ )
#define _HOOK_HXX_

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _HOOKDLL_ )
	#define HOOKAPI __declspec(dllimport)
#else
	#define HOOKAPI __declspec(dllexport)
#endif

// ====================================================================
// WINDOW MESSAGE DEFINITIONS
#define WM_KEYHOOK				(WM_APP + 0x0100)
#define WM_MOUSEHOOK			(WM_APP + 0x0101)

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================

// --------------------------------------------------------------------
// Set
HOOKAPI BOOL __stdcall HookKeySet(void);
HOOKAPI BOOL __stdcall HookMouseSet(void);

// --------------------------------------------------------------------
// Unhook
HOOKAPI BOOL __stdcall HookKeyUnhook(void);
HOOKAPI BOOL __stdcall HookMouseUnhook(void);

#ifdef __cplusplus
}
#endif

#endif // _HOOK_HXX_
