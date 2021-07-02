#if !defined( _NANOMISC_HXX_ )
#define _NANOMISC_HXX_

#pragma once

#include <globals.hxx>

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _NANOMISCDLL_ )
	#define NANOMISCAPI __declspec(dllimport)
#else
	#define NANOMISCAPI __declspec(dllexport)
#endif

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================
NANOMISCAPI BOOL __stdcall nanoInformation(int nCmdShow);

#ifdef __cplusplus
}
#endif
_NANOMISC_HXX_
#endif // _NEXPASS_HXX_
