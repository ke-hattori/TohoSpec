#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#include <windows.h>
#include "..\\..\\INC\\globals.hxx"
#include "..\\..\\INC\\stageint.hxx"
#include "..\\..\\INC\\overlay.hxx"
#include <tchar.h>
#include <string>
#include <algorithm>
#include <math.h>
#include "tisudshl.h"
#include "overlay.h"

#ifdef _DEBUG
#pragma comment(lib, "tis_udshl07_vc6d.lib")
#else
#pragma comment(lib, "tis_udshl07_vc6.lib")
#endif

const WORD VIDEO_CALIB_FACTOR_DISP_DIGITS	= 3;

static double  Convert_RoundForDisplay( double Val, WORD NumDecimDigts )
{
	if ( NumDecimDigts > 20 )
		return 0;

	double Fct	= pow(10, NumDecimDigts);
	long   Temp = (long) (Val * Fct + 0.5);

	return Temp / Fct;
}

extern HWND CreateDummyWindow(void);

// =========================================================================
// Class Name :  CImageCtrl
//
// Description： オーバレイラッパクラス
//
// Author:		 T.Matsuhisa
//
// =========================================================================

// =========================================================================
// Construction/Destruction
CImageCtrl::CImageCtrl()
	: m_pSink(DShowLib::FrameHandlerSink::create(DShowLib::eRGB24, 5)), COverlay()
{
	if ( m_bValid = DShowLib::InitLibrary("IP-0000105987") ) {
		m_pSink->setSnapMode(false);
		m_Grabber.setSinkType(m_pSink);

		m_Grabber.addListener(this, DShowLib::GrabberListener::eFRAMEREADY);
	}
}

CImageCtrl::~CImageCtrl()
{
	// ライブラリを開放する
	DShowLib::ExitLibrary();

	if ( ::IsWindow(m_hwndDummy) ) {
		::DestroyWindow(m_hwndDummy);
		m_hwndDummy = NULL;
	}
}

// =========================================================================
// Name：	   Craete
// Purpose：
// Parameters: None
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::Attach(HWND hwndOverlay)
{
	if ( m_Grabber.isDevOpen() ) {
		Detach();
	}

	m_Grabber.setHWND(hwndOverlay);

	Grabber::tVidCapDevListPtr pVidCapDevList =
		m_Grabber.getAvailableVideoCaptureDevices();
	if ( pVidCapDevList == 0 || pVidCapDevList->empty() ) {
		return;
	}

	BOOL bRet = m_Grabber.openDev(pVidCapDevList->at(0));
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
//	m_Grabber.setVideoFormat("UYVY (640x480)");
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ----------              */
	m_Grabber.setVideoFormat(g_szVideoFmtSize);
/* modified 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
	m_Grabber.setInputChannel(1);

	COverlay::Attach(hwndOverlay);
}

// =========================================================================
// Name：	   Destroy
// Purpose：
// Parameters: None
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::Detach(void)
{
	PreviewStop();

	if ( m_Grabber.isDevOpen() ) {
		m_Grabber.closeDev();
	}

	COverlay::Detach();
}

// =========================================================================
// Name：	   Preview
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CImageCtrl::Preview(bool bEnable)
{
	return bEnable ? PreviewStart() : PreviewStop();
}

// =========================================================================
// Name：	   SelectChannel
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CImageCtrl::SelectChannel(DWORD dwSelect)
{
	COverlay::SelectChannel(dwSelect);

	return SelectChannel();
}

// =========================================================================
// Name：	   ImageGrab
// Purpose：
// Parameters: pszFilePath - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::ImageGrab(LPVOID lpBuffer)
{
	_ASSERT(m_pSink != NULL);

#if !defined( __MYGRAYSCALE )
	smart_ptr<MemBuffer> pBuffer = m_pSink->getLastAcqMemBuffer();
	if ( pBuffer != NULL ) {
		pBuffer->lock();

		::CopyMemory(lpBuffer, pBuffer->getPtr(), pBuffer->getBufferSize());

		pBuffer->unlock();
	}
#else
	smart_ptr<MemBuffer>  pBuffer = m_pSink->getLastAcqMemBuffer();
	MemBuffer::tBmInfHPtr pbi	  = pBuffer->getBitmapInfoHeader();
	if ( pBuffer != NULL ) {
		pBuffer->lock();

		unsigned char *buffer = NULL, *rgb = NULL, *src;
		int height, width;

		buffer = (unsigned char*) malloc(pBuffer->getBufferSize());
		if ( buffer != NULL ) {
			memcpy(buffer, pBuffer->getPtr(), pBuffer->getBufferSize());
		}
		rgb = (unsigned char*) lpBuffer;
		for ( height = 0; height < pbi->biHeight; height++ ) {
			for ( width = 0; width < pbi->biWidth; width++ ) {
				src = buffer+ 3 * ( ( pbi->biHeight - 1 - height ) * pbi->biWidth + width );
				*(rgb++) = *(src + 2);
				*(rgb++) = *(src + 1);
				*(rgb++) = *(src + 0);
			}
		}
		free(buffer);
		pBuffer->unlock();
	}
#endif
}

// =========================================================================
// Name：	   SaveImage
// Purpose：
// Parameters: pszFilePath - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::SaveImage(PCSTR pszFilePath)
{
	_ASSERT(m_pSink != NULL);

	smart_ptr<MemBuffer> pBuffer = m_pSink->getLastAcqMemBuffer();
	if ( pBuffer != NULL ) {
		pBuffer->lock();

		pBuffer->save(pszFilePath);

		pBuffer->unlock();
	}
}

// =========================================================================
// Name：	   SetMultipleWindow
// Purpose：
// Parameters: pszFilePath - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::SetMultipleWindow(CONST OVLYMULTIWND* lpOvlyMultiWnd)
{
	if ( !m_Grabber.isDevOpen() ) {
		if ( !::IsWindow(m_hwndDummy) ) {
			m_hwndDummy = CreateDummyWindow();
		}
		Attach(m_hwndDummy);
		PreviewStart();
	}

	COverlay::SetMultipleWindow(lpOvlyMultiWnd);
}

// =========================================================================
// Name：	   frameReady
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::frameReady(
	Grabber&			 caller,
	smart_ptr<MemBuffer> pBuffer,
	DWORD				 dwFrameNumber
	)
{
	smart_ptr<BITMAPINFOHEADER> pBmInfHdr = pBuffer->getBitmapInfoHeader();
	for ( std::list<OVLYMULTIWND>::iterator first(m_list.begin()), last(m_list.end()); first != last; ++first ) {
		if ( ::IsWindow( (*first).hwnd ) ) {
			HDC hDC = ::GetDC( (*first).hwnd );
			if ( hDC ) {
				int nDestWidth	= (*first).rc.right  - (*first).rc.left;
				int nDestHeight = (*first).rc.bottom - (*first).rc.top;
				int nSrcWidth	= pBmInfHdr->biWidth;
				int nSrcHeight	= abs(pBmInfHdr->biHeight);
				::SetStretchBltMode(hDC, COLORONCOLOR);
				int nLines = StretchDIBits(
								 hDC,
								 (*first).rc.left,
								 (*first).rc.top,
								 nDestWidth,
								 nDestHeight,
								 0,
								 0,
								 nSrcWidth,
								 nSrcHeight,
								 pBuffer->getPtr(),
								 reinterpret_cast<LPBITMAPINFO>(pBmInfHdr.get()),
								 DIB_RGB_COLORS,
								 SRCCOPY
								 );
				HPEN hOldPen, hNewPen;
				double dXRatio = (double)(double(nDestWidth)  / double(nSrcWidth));
				double dYRatio = (double)(double(nDestHeight) / double(nSrcHeight));
				if ( m_bDrawGoodDeskewSite ) {
					RECT rc;
					rc.left   = (nDestWidth  / 2) + static_cast<LONG>((m_DeskewResult.XPos - 15.0) * dXRatio);
					rc.top	  = (nDestHeight / 2) + static_cast<LONG>((m_DeskewResult.YPos - 15.0) * dYRatio);
					rc.right  = (nDestWidth  / 2) + static_cast<LONG>((m_DeskewResult.XPos + 15.0) * dXRatio);
					rc.bottom = (nDestHeight / 2) + static_cast<LONG>((m_DeskewResult.YPos + 15.0) * dYRatio);
					hNewPen = ::CreatePen(PS_SOLID, 0, RGB (0, 255, 0));
					if ( hNewPen ) {
						hOldPen = (HPEN) ::SelectObject(hDC, (HGDIOBJ) hNewPen);
						::Rectangle(
							hDC,
							rc.left,
							rc.top,
							rc.right,
							rc.bottom
							);
						::SelectObject(hDC, hOldPen);
						::DeleteObject(hNewPen);
					}
				}
				else
				if ( m_bDrawFailedDeskewSite ) {
					hNewPen = ::CreatePen(PS_SOLID, 3, RGB (255, 0, 0));
					if ( hNewPen ) {
						hOldPen = (HPEN) ::SelectObject(hDC, (HGDIOBJ) hNewPen);
						::MoveToEx(hDC, (nDestWidth / 2) - static_cast<int>(10 * dXRatio), (nDestHeight / 2) - static_cast<int>(10 * dYRatio), NULL);
						::LineTo(hDC, (nDestWidth / 2) + static_cast<int>(10 * dXRatio), (nDestHeight / 2) + static_cast<int>(10 * dYRatio));
						::MoveToEx(hDC, (nDestWidth / 2) + static_cast<int>(10 * dXRatio), (nDestHeight / 2) - static_cast<int>(10 * dYRatio), NULL);
						::LineTo(hDC, (nDestWidth / 2) - static_cast<int>(10 * dXRatio), (nDestHeight / 2) + static_cast<int>(10 * dYRatio));
						::SelectObject(hDC, hOldPen);
						::DeleteObject(hNewPen);
					}
				}
				::ReleaseDC((*first).hwnd, hDC);
			}
		}
	}
}

// =========================================================================
// Name：	   DrawCenter
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::DrawCenter(void)
{
	static BOOL bInit = FALSE;
	if ( !bInit ) {
		bInit = TRUE;
		GetWindowRect(&m_rcOverlay);
		m_ptOverlayCenter.x = (m_rcOverlay.right - m_rcOverlay.left) / 2;
		m_ptOverlayCenter.y = (m_rcOverlay.bottom - m_rcOverlay.top) / 2;

	}
	double m_XPixelToMicron = 0.74757575757575757;
	double m_YPixelToMicron = 0.7518796;

	Convert_RoundForDisplay( m_XPixelToMicron, VIDEO_CALIB_FACTOR_DISP_DIGITS );
	Convert_RoundForDisplay( m_YPixelToMicron, VIDEO_CALIB_FACTOR_DISP_DIGITS );

	//	Determine the size of the whole screen. The video overlay
	//	start at (0,0) and of size 512 * 488. So the center of the
	//	video overlay is at (255, 243). The center is drawn only
	//	if the width of the view is wider than 256.
	//
	if ( m_Grabber.isLive() ) {
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap = m_Grabber.getOverlay(ePP_DISPLAY);
		COLORREF crColor = RGB(0, 255, 0);
		pOverlayBitmap->setEnable(true);
		// get the video calibration factor
//		VideoCalib_T VideoCalib;
//		CalibCom_GetVideoCalibFactor( &VideoCalib );

		// define a quarter length between every big tick marker to be 50um
		int iMarkSpc = int ( 50 / m_XPixelToMicron + 0.5 );

		// Draw two cross dashed lines on the overly screen with markers on them
		DrawDashLine( X, m_ptOverlayCenter.x, +1, iMarkSpc, m_rcOverlay.right, m_ptOverlayCenter.y, crColor, pOverlayBitmap );
		DrawDashLine( X, m_ptOverlayCenter.x, -1, iMarkSpc, m_rcOverlay.left,  m_ptOverlayCenter.y, crColor, pOverlayBitmap );

		// draw the markers on each line
		DrawMarkerOnDashLine( X, m_ptOverlayCenter.x, +1, iMarkSpc * 2, m_rcOverlay.right, m_ptOverlayCenter.y, crColor, pOverlayBitmap );
		DrawMarkerOnDashLine( X, m_ptOverlayCenter.x, -1, iMarkSpc * 2, m_rcOverlay.left,  m_ptOverlayCenter.y, crColor, pOverlayBitmap );

		// Now do the y axis
		iMarkSpc = int ( 50 / m_YPixelToMicron + 0.5 );

		DrawDashLine( Y, m_ptOverlayCenter.y, +1, iMarkSpc, m_rcOverlay.bottom, m_ptOverlayCenter.x, crColor, pOverlayBitmap );
		DrawDashLine( Y, m_ptOverlayCenter.y, -1, iMarkSpc, m_rcOverlay.top,   m_ptOverlayCenter.x,  crColor, pOverlayBitmap );

		DrawMarkerOnDashLine( Y, m_ptOverlayCenter.y, +1, iMarkSpc * 2, m_rcOverlay.bottom, m_ptOverlayCenter.x, crColor, pOverlayBitmap );
		DrawMarkerOnDashLine( Y, m_ptOverlayCenter.y, -1, iMarkSpc * 2, m_rcOverlay.top,	m_ptOverlayCenter.x, crColor, pOverlayBitmap );
	}
}

// =========================================================================
// Name：	   DrawMarkerOnDashLine
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::DrawMarkerOnDashLine(
	WORD							   wAxis,
	int 							   iStart,
	int 							   iInc,
	int 							   iLength,
	int 							   iBound,
	int 							   iOtherAxisVal,
	COLORREF						   crColor,
	smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap
	)
{
#define SmallMarkerSize 7
#define BigMarkerSize	10
	bool bret;

	iStart += iLength * iInc;

	BOOL bContinue = iInc > 0 ? iStart <= iBound : iStart >= iBound;
	while( bContinue ) {
		if ( wAxis == X ) { // draw the line along x axis
			bret = pOverlayBitmap->drawLine(crColor, iStart, iOtherAxisVal, iStart, iOtherAxisVal + SmallMarkerSize);
			iStart += iLength * iInc;
			bret = pOverlayBitmap->drawLine(crColor, iStart, iOtherAxisVal, iStart, iOtherAxisVal - BigMarkerSize);
		}
		else {				// draw the line along y axis
			bret = pOverlayBitmap->drawLine(crColor, iOtherAxisVal, iStart, iOtherAxisVal - SmallMarkerSize, iStart);
			iStart += iLength * iInc;
			if ( iStart < m_rcOverlay.bottom ) {
				bret = pOverlayBitmap->drawLine(crColor, iOtherAxisVal, iStart, iOtherAxisVal + BigMarkerSize, iStart);
			}
		}

		iStart += iLength * iInc;
		bContinue = iInc > 0 ? iStart <= iBound : iStart >= iBound;
	}

#undef SmallMarkerSize
#undef BigMarkerSize
}

// =========================================================================
// Name：	   DrawDashLine
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::DrawDashLine(
		WORD							   wAxis,
		int 							   iStart,
		int 							   iInc,
		int 							   iLength,
		int 							   iBound,
		int 							   iOtherAxisVal,
		COLORREF						   crColor,
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap
		)
{
	int iEmptySpc = 4; // spaces between lines

	int iEnd = iStart + (iLength - iEmptySpc / 2) * iInc;

	DrawOneLine(wAxis, iStart, iEnd, iOtherAxisVal, crColor, pOverlayBitmap);
	iStart = iEnd + iEmptySpc * iInc;

	BOOL bContinue = iInc > 0 ? iStart <= iBound : iStart >= iBound;
	while ( bContinue ) {
		iEnd = iStart + (iLength * 2 - iEmptySpc) * iInc;
		DrawOneLine(wAxis, iStart, iEnd, iOtherAxisVal, crColor, pOverlayBitmap);
		iStart = iEnd + iEmptySpc * iInc;

		bContinue = iInc > 0 ? iStart <= iBound : iStart >= iBound;
	}
}

// =========================================================================
// Name：	   DrawOneLine
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::DrawOneLine(
	WORD							   wAxis,
	int 							   iStart,
	int 							   iEnd,
	int 							   iOtherAxisVal,
	COLORREF						   crColor,
	smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap
	)
{
	if ( wAxis == X ) { // draw the line along x axis
		pOverlayBitmap->drawLine(crColor, iStart, iOtherAxisVal, iEnd, iOtherAxisVal);
	}
	else {				// draw the line along y axis
		iStart = min(iStart, m_rcOverlay.bottom - 1);
		iEnd   = min(iEnd, m_rcOverlay.bottom - 1);
		pOverlayBitmap->drawLine(crColor, iOtherAxisVal, iStart, iOtherAxisVal, iEnd);
	}
}

BOOL CImageCtrl::DrawLine(
	COLORREF crColor,
	int 	 nXStart,
	int 	 nYStart,
	int 	 nXEnd,
	int 	 nYEnd
	)
{
	BOOL bRet = FALSE;

	if ( m_Grabber.isLive() ) {
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap = m_Grabber.getOverlay(ePP_DISPLAY);

		bRet = pOverlayBitmap->drawLine(crColor, nXStart, nYStart, nXEnd, nYEnd);
	}

	return bRet;
}

// =========================================================================
// Name：	   DrawRectangle
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::DrawRectangle(CONST RECT* lpRect, BOOL bDrawColor)
{
	if ( m_Grabber.isLive() ) {
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap = m_Grabber.getOverlay(ePP_DISPLAY);
		COLORREF crColor = bDrawColor ? RGB(0, 0, 255) : pOverlayBitmap->getDropOutColor();

		pOverlayBitmap->drawFrameRect(crColor, *lpRect);
		int x = lpRect->left + (lpRect->right  - lpRect->left) / 2;
		int y = lpRect->top  + (lpRect->bottom - lpRect->top)  / 2;
#define D 2
		RECT rect = { x - D, y - D, x + D, y + D };
		if ( rect.bottom + D < m_rcOverlay.bottom )
			pOverlayBitmap->drawSolidEllipse(crColor, rect);
#undef D
	}
}

// =========================================================================
// Name：	   Fill
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CImageCtrl::Fill(COLORREF crColor)
{
	BOOL bRet = FALSE;

	if ( m_Grabber.isLive() ) {
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap = m_Grabber.getOverlay(ePP_DISPLAY);
		bRet = pOverlayBitmap->fill(crColor);
	}

	return bRet;
}

// =========================================================================
// Name：	   SetBkColor
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CImageCtrl::SetBkColor(COLORREF crColor)
{
	BOOL bRet = FALSE;

	if ( m_Grabber.isLive() ) {
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap = m_Grabber.getOverlay(ePP_DISPLAY);
		bRet = pOverlayBitmap->setDropOutColor(crColor);
	}

	return bRet;
}

// =========================================================================
// Name：	   GetBkColor
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
COLORREF CImageCtrl::GetBkColor(void) const
{
	COLORREF crColor = CLR_INVALID;

	if ( m_Grabber.isLive() ) {
		smart_ptr<DShowLib::OverlayBitmap> pOverlayBitmap = m_Grabber.getOverlay(ePP_DISPLAY);
		crColor = pOverlayBitmap->getDropOutColor();
	}

	return crColor;
}

// =========================================================================
// Name：	   SetWindowPos
// Purpose：
// Parameters: bEnable - Specify TRUE for this parameter to enable preview
//						 mode of FALSE to disable it.
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
void CImageCtrl::SetWindowPos(int X, int Y)
{
	if ( m_Grabber.isDevValid() ) {
		m_Grabber.setWindowPosition(X, Y);
	}
}
