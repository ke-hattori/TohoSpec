#if !defined( _PATREC_HXX_ )
#define _PATREC_HXX_

#pragma once

#include "..\\..\\INC\\globals.hxx"

// pattern recognition deskew images
#define DESKEW_IMG_TEMP 		DB_DESKEW_IMG_DIR "temp.img"
#define DESKEW_IMG_1			DB_DESKEW_IMG_DIR "dskw_1.img"
#define DESKEW_IMG_2			DB_DESKEW_IMG_DIR "dskw_2.img"

#ifdef __cplusplus
extern "C"
{
#endif


#if !defined( _PATRECDLL_ )
	#define PATRECAPI __declspec(dllimport)
#else
	#define PATRECAPI __declspec(dllexport)
#endif

#define IMAGE_WIDTH 			(640L)
#define IMAGE_HEIGHT			(480L)
#define DESKEW_SITE_MAX 		(2)

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================

PATRECAPI BOOL __stdcall PatRecInit(void);
PATRECAPI BOOL __stdcall PatRecEnd(void);
PATRECAPI BOOL __stdcall PatRecTeach(LPRECT lpRect);
PATRECAPI BOOL __stdcall PatRecMatch(LPDESKEW_RESULT lpDeskewResult);
PATRECAPI BOOL __stdcall PatRecSaveModel(PSTR pszModelName);
PATRECAPI BOOL __stdcall PatRecDrawModel(HWND hWnd, WORD wSite, PSTR pszModelName);
PATRECAPI BOOL __stdcall PatRecRestoreModel(PSTR pszModelName);

#ifdef __cplusplus
}
#endif

#endif // _PATREC_HXX_
