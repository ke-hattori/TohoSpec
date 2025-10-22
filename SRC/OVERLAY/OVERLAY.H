#if !defined( _OVERLAY_H_ )
#define _OVERLAY_H_

#pragma once

#pragma warning (disable : 4786)

#include <windows.h>
#include <process.h>
#include <crtdbg.h>
#include <tchar.h>
#include <string>
#include <list>
#include "tisudshl.h"

class COverlay
{
public:
	COverlay()
		: m_hwndOverlay(NULL), m_hwndMultiple(NULL), m_hwndDummy(NULL), m_dwSelectChannel(0L),
		m_bDrawFailedDeskewSite(FALSE), m_bDrawGoodDeskewSite(FALSE) {
		::ZeroMemory(&m_DeskewResult, sizeof(m_DeskewResult));
	}
	virtual ~COverlay() {};

public:
	/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
	virtual void SetVideoFmtSize(char* pszVideoFmtSize) {
		strncpy(g_szVideoFmtSize,
			pszVideoFmtSize,
			(sizeof(g_szVideoFmtSize) / sizeof(char)) - 1);
		g_szVideoFmtSize[(sizeof(g_szVideoFmtSize) / sizeof(char)) - 1] = 0;
	}
	/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
	virtual void Attach(HWND hwndOverlay) { m_hwndOverlay = hwndOverlay; }
	virtual void Detach(void) { m_hwndOverlay = NULL; }

	virtual BOOL Preview(bool bEnable) { return FALSE; }
	virtual BOOL SelectChannel(DWORD dwSelect) {
		m_dwSelectChannel = dwSelect;
		return TRUE;
	}
	virtual void SaveImage(PCSTR pszFileName) {}
	virtual void ImageGrab(LPVOID lpBuffer) {}
	virtual void SetMultipleWindow(CONST OVLYMULTIWND* lpOvlyMultiWnd) { m_list.push_back(*lpOvlyMultiWnd); }
	virtual void DrawCenter(void) {}
	virtual void GetWindowRect(LPRECT lpRect) {}
	virtual BOOL DrawLine(
		COLORREF crColor,
		int 	 nXStart,
		int 	 nYStart,
		int 	 nXEnd,
		int 	 nYEnd
	) {
		return FALSE;
	}
	virtual void DrawRectangle(CONST RECT* lpRect, BOOL bDrawColor) {}
	virtual BOOL Fill(COLORREF crColor) { return FALSE; }
	virtual BOOL SetBkColor(COLORREF crColor) { return FALSE; }
	virtual COLORREF GetBkColor(void) const { return CLR_INVALID; }
	virtual void SetWindowPos(int X, int Y) {}

	// for multiple windows
	virtual void DrawFailedDeskewSite(BOOL bDrawFailedDeskewSite)
	{
		if (bDrawFailedDeskewSite) {
			m_bDrawFailedDeskewSite = bDrawFailedDeskewSite;
			m_bDrawGoodDeskewSite = FALSE;
		}
		else {
			m_bDrawFailedDeskewSite = FALSE;
		}
	}

	virtual void DrawGoodDeskewSite(BOOL bDrawGoodDeskewSite, LPDESKEW_RESULT lpDeskewResult)
	{
		if (bDrawGoodDeskewSite) {
			if (lpDeskewResult != NULL) {
				m_DeskewResult = *lpDeskewResult;
				m_bDrawGoodDeskewSite = bDrawGoodDeskewSite;
				m_bDrawFailedDeskewSite = FALSE;
			}
			else {
				m_bDrawGoodDeskewSite = FALSE;
			}
		}
	}

protected:
	virtual BOOL SelectChannel(void) { return FALSE; }
	inline BOOL IsValid(void) { return m_bValid; }

protected:
	/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
	char					g_szVideoFmtSize[128];
	/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
	BOOL					m_bValid;
	HWND					m_hwndOverlay;
	HWND					m_hwndMultiple;
	std::list<OVLYMULTIWND> m_list;
	HWND					m_hwndDummy;
	DWORD					m_dwSelectChannel;
	RECT					m_rcOverlay;
	POINT					m_ptOverlayCenter;
	BOOL					m_bDrawFailedDeskewSite;
	BOOL					m_bDrawGoodDeskewSite;
	DESKEW_RESULT			m_DeskewResult;
};

using namespace _DSHOWLIB_NAMESPACE;

class CImageCtrl : public COverlay, DShowLib::GrabberListener
{
public:
	CImageCtrl();
	virtual ~CImageCtrl();

public:
	virtual void Attach(HWND hwndOverlay);
	virtual void Detach(void);

	virtual BOOL Preview(bool bEnable);
	virtual BOOL SelectChannel(DWORD dwSelect);
	virtual void ImageGrab(LPVOID lpBuffer);
	virtual void SaveImage(PCSTR pszFileName);
	virtual void SetMultipleWindow(CONST OVLYMULTIWND* lpOvlyMultiWnd);
	virtual void DrawCenter(void);
	virtual BOOL DrawLine(
		COLORREF crColor,
		int 	 nXStart,
		int 	 nYStart,
		int 	 nXEnd,
		int 	 nYEnd
	);
	virtual void DrawRectangle(CONST RECT* lpRect, BOOL bDrawColor);
	virtual BOOL Fill(COLORREF crColor);
	virtual BOOL SetBkColor(COLORREF crColor);
	virtual COLORREF GetBkColor(void) const;
	virtual void SetWindowPos(int X, int Y);
	virtual void GetWindowRect(LPRECT lpRect) {
		long cx, cy;

		if (m_Grabber.isDevValid()) {
			m_Grabber.getWindowPosition(lpRect->left, lpRect->top, cx, cy);
			lpRect->right = cx - lpRect->left;
			lpRect->bottom = cy - lpRect->top;
		}
	}

protected:
	virtual BOOL SelectChannel(void) {
		return m_Grabber.setInputChannel(m_dwSelectChannel);
	}

private:
	inline BOOL OpenDevice(void) {
		return TRUE;
	}
	inline BOOL PreviewStart(DWORD dwPathPosition = ePP_DISPLAY) {
		if (m_hwndOverlay == NULL || m_Grabber.isLive()) return FALSE;

		m_Grabber.setOverlayBitmapPathPosition(dwPathPosition);
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap = m_Grabber.getOverlay(ePP_DISPLAY);
		pOverlayBitmap->fill(pOverlayBitmap->getDropOutColor());

		return m_Grabber.startLive();
	}
	inline BOOL PreviewStop(void) {
		if (!m_Grabber.isLive()) return FALSE;
		return m_Grabber.stopLive();
	}
	virtual void frameReady(
		Grabber& caller,
		smart_ptr<MemBuffer> pBuffer,
		DWORD				 dwFrameNumber
	);
	void DrawDashLine(
		WORD							   wAxis,
		int 							   iStart,
		int 							   iInc,
		int 							   iLength,
		int 							   iBound,
		int 							   iOtherAxisVal,
		COLORREF						   crColor,
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap
	);
	void DrawOneLine(
		WORD							   wAxis,
		int 							   iStart,
		int 							   iEnd,
		int 							   iOtherAxisVal,
		COLORREF						   crColor,
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap
	);
	void DrawMarkerOnDashLine(
		WORD							   wAxis,
		int 							   iStart,
		int 							   iInc,
		int 							   iLength,
		int 							   iBound,
		int 							   iOtherAxisVal,
		COLORREF						   crColor,
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap
	);

private:
	DShowLib::Grabber					m_Grabber;
	DShowLib::FrameHandlerSink::tFHSPtr m_pSink;
};

#endif // _OVERLAY_H_
