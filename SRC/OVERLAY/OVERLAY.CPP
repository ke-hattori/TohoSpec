#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#define _OVERLAYDLL_

#include <windows.h>
#include "..\\..\\INC\\overlay.hxx"
#include <tchar.h>
#include <string>
#include "overlay.h"

// ####################################################################
// GLOBAL VARIABLES
// ####################################################################
static COverlay *pOverlay = NULL;
HINSTANCE hInstance = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch ( message ) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HWND CreateDummyWindow(void)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			   = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	   = (WNDPROC)WndProc;
	wcex.cbClsExtra 	   = 0;
	wcex.cbWndExtra 	   = 0;
	wcex.hInstance		   = hInstance;
	wcex.hIcon			   = LoadIcon(hInstance, NULL);
	wcex.hCursor		   = NULL;
	wcex.hbrBackground	   = NULL;
	wcex.lpszMenuName	   = NULL;
	wcex.lpszClassName	   = TEXT("HOGE");
	wcex.hIconSm		   = LoadIcon(wcex.hInstance, NULL);

	RegisterClassEx(&wcex);

	// create the dummy window
	return ::CreateWindow("HOGE", TEXT("Hoge Display"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

}

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
		if (FALSE == DllMutexCreate(_T("OVERLAY"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		hInstance = (HINSTANCE) hModule;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		break;
	}
	return TRUE;
}

// ====================================================================
// OverlayAttach
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
//void __stdcall OverlayInitInstance(void)
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ----------              */
void __stdcall OverlayInitInstance(char* pszVideoFmtSize)
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
{
	if ( pOverlay == NULL ) {
		pOverlay = new CImageCtrl();
/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
		if (0 != pOverlay) {
			pOverlay->SetVideoFmtSize(pszVideoFmtSize);
		}
/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
	}
}

// ====================================================================
// OverlayAttach
void __stdcall OverlayExitInstance(void)
{
	if ( pOverlay != NULL ) {
		delete pOverlay;
		pOverlay = NULL;
	}
}

// ====================================================================
// OverlayAttach
void __stdcall OverlayAttach(HWND hwndOverlay)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->Attach(hwndOverlay);
}

// ====================================================================
// OverlayDetach
void __stdcall OverlayDetach(void)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->Detach();
}

// ====================================================================
// OverlayPreview
BOOL __stdcall OverlayPreview(bool bEnable)
{
	_ASSERT(pOverlay != NULL);

	return pOverlay->Preview(bEnable);
}

// ====================================================================
// OverlayImageGrab
void __stdcall OverlayImageGrab(LPVOID lpBuffer)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->ImageGrab(lpBuffer);
}

// ====================================================================
// OverlaySaveImage
void __stdcall OverlaySaveImage(PCSTR pszFilePath)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->SaveImage(pszFilePath);
}

// ====================================================================
// OverlaySetMultipleWindow
void __stdcall OverlaySetMultipleWindow(CONST OVLYMULTIWND* lpOvlyMultiWnd)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->SetMultipleWindow(lpOvlyMultiWnd);
}

// ====================================================================
// OverlayDrawCenter
void __stdcall OverlayDrawCenter(void)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->DrawCenter();
}
BOOL __stdcall OverlayDrawLine(
	COLORREF crColor,
	int 	 nXStart,
	int 	 nYStart,
	int 	 nXEnd,
	int 	 nYEnd
	)
{
	_ASSERT(pOverlay != NULL);

	return pOverlay->DrawLine(crColor, nXStart, nYStart, nXEnd, nYEnd);
}

// ====================================================================
// OverlayDrawRectangle
void __stdcall OverlayDrawRectangle(CONST RECT* lpRect, BOOL bDrawColor)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->DrawRectangle(lpRect, bDrawColor);
}

// ====================================================================
// OverlayFill
BOOL __stdcall OverlayFill(COLORREF crColor)
{
	_ASSERT(pOverlay != NULL);

	return pOverlay->Fill(crColor);
}

// ====================================================================
// OverlaySetBkColor
BOOL __stdcall OverlaySetBkColor(COLORREF crColor)
{
	_ASSERT(pOverlay != NULL);

	return pOverlay->SetBkColor(crColor);
}

// ====================================================================
// OverlayGetBkColor
COLORREF __stdcall OverlayGetBkColor(void)
{
	_ASSERT(pOverlay != NULL);

	return pOverlay->GetBkColor();
}

// ====================================================================
// OverlaySetWindowPos
void __stdcall OverlaySetWindowPos(int X, int Y)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->SetWindowPos(X, Y);
}

// ====================================================================
// OverlayGetWindowRect
void __stdcall OverlayGetWindowRect(LPRECT lpRect)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->GetWindowRect(lpRect);
}

// ====================================================================
// OverlaySelectChannel
BOOL __stdcall OverlaySelectChannel(DWORD dwSelect)
{
	_ASSERT(pOverlay != NULL);

	return pOverlay->SelectChannel(dwSelect);
}

void __stdcall OverlayDrawFailedDeskewSite(BOOL bDraw/*=FALSE*/)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->DrawFailedDeskewSite(bDraw);
}

void __stdcall OverlayDrawGoodDeskewSite(BOOL bDraw/*=FALSE*/, LPDESKEW_RESULT lpDeskewResult/*=NULL*/)
{
	_ASSERT(pOverlay != NULL);

	pOverlay->DrawGoodDeskewSite(bDraw, lpDeskewResult);
}
