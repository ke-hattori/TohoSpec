#if !defined( _OVERLAY_HXX_ )
#define _OVERLAY_HXX_

#pragma once

#include <globals.hxx>

#define __MYGRAYSCALE

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _OVERLAYDLL_ )
	#define OVERLAYAPI __declspec(dllimport)
#else
	#define OVERLAYAPI __declspec(dllexport)
#endif

typedef struct _tagOVLYMULTIWND
{
	HWND hwnd;
	RECT rc;
} OVLYMULTIWND, *LPOVLYMULTIWND;

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
//OVERLAYAPI void 	__stdcall OverlayInitInstance(void);
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ----------              */
OVERLAYAPI void 	__stdcall OverlayInitInstance(char* pszVideoFmtSize);
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
OVERLAYAPI void 	__stdcall OverlayExitInstance(void);
OVERLAYAPI void 	__stdcall OverlayAttach(HWND hwndOverlay);
OVERLAYAPI void 	__stdcall OverlayDetach(void);
OVERLAYAPI BOOL 	__stdcall OverlayPreview(bool bEnable);
OVERLAYAPI void 	__stdcall OverlayImageGrab(LPVOID lpBuffer);
OVERLAYAPI void 	__stdcall OverlaySaveImage(PCSTR pszFilePath);
OVERLAYAPI void 	__stdcall OverlaySetMultipleWindow(CONST OVLYMULTIWND* lpOvlyMultiWnd);
OVERLAYAPI void 	__stdcall OverlayDrawCenter(void);
OVERLAYAPI void 	__stdcall OverlayDrawRectangle(CONST RECT* lpRect, BOOL bDrawColor = TRUE);
OVERLAYAPI BOOL 	__stdcall OverlayDrawLine(COLORREF crColor, int nXStart, int nYStart, int nXEnd, int nYEnd);
OVERLAYAPI BOOL 	__stdcall OverlayFill(COLORREF crColor);
OVERLAYAPI BOOL 	__stdcall OverlaySetBkColor(COLORREF crColor);
OVERLAYAPI COLORREF __stdcall OverlayGetBkColor(void);
OVERLAYAPI void 	__stdcall OverlaySetWindowPos(int X, int Y);
OVERLAYAPI void 	__stdcall OverlayGetWindowRect(LPRECT lpRect);
OVERLAYAPI BOOL 	__stdcall OverlaySelectChannel(DWORD dwSelect);

/*
 for multiple window
*/
OVERLAYAPI void 	__stdcall OverlayDrawFailedDeskewSite(BOOL bDraw = FALSE);
OVERLAYAPI void 	__stdcall OverlayDrawGoodDeskewSite(BOOL bDraw = FALSE, LPDESKEW_RESULT lpDeskewResult = NULL);

#ifdef __cplusplus
}
#endif

#endif // _OVERLAY_HXX_
