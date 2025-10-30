// BitmapStatic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "BitmapStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CBitmapStatic
// #########################################################################
CBitmapStatic::CBitmapStatic()
	: m_ptImage(0, 0)
{
}

CBitmapStatic::~CBitmapStatic()
{
}

BEGIN_MESSAGE_MAP(CBitmapStatic, CStatic)
	//{{AFX_MSG_MAP(CBitmapStatic)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CBitmapStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CBitmapStatic::OnPaint()
{
	CPaintDC dc(this);

	DrawBitmap(&dc);
}

void CBitmapStatic::PreSubclassWindow()
{
	ShowScrollBar(SB_BOTH, TRUE);

	GetClientRect(&m_rcClient);

	ShowScrollBar(SB_BOTH, FALSE);

	CStatic::PreSubclassWindow();
}

void CBitmapStatic::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int iScrollBarPos = GetScrollPos(SB_VERT);

	SCROLLINFO si;
	switch (nSBCode) {
	case SB_LINEUP:
		iScrollBarPos = max(iScrollBarPos - 1, 0);
		break;
	case SB_LINEDOWN:
		iScrollBarPos = min(iScrollBarPos + 1, GetScrollLimit(SB_VERT));
		break;
	case SB_PAGEUP:
		iScrollBarPos = max(iScrollBarPos - 10, 0);
		break;
	case SB_PAGEDOWN:
		iScrollBarPos = min(iScrollBarPos + 10, GetScrollLimit(SB_VERT));
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask	= SIF_TRACKPOS;
		if( GetScrollInfo(SB_VERT, &si, SIF_TRACKPOS) )
			iScrollBarPos = si.nTrackPos;
		else
			iScrollBarPos = (UINT) nPos;
		break;
	}
	m_ptImage.y = iScrollBarPos;
	SetScrollPos(SB_VERT, iScrollBarPos);

	Invalidate();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBitmapStatic::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int iScrollBarPos = GetScrollPos(SB_HORZ);

	SCROLLINFO si;
	switch ( nSBCode ) {
	case SB_LINELEFT:
		iScrollBarPos = max(iScrollBarPos - 1, 0);
		break;
	case SB_LINERIGHT:
		iScrollBarPos = min(iScrollBarPos + 1, GetScrollLimit(SB_HORZ));
		break;
	case SB_PAGELEFT:
		iScrollBarPos = max(iScrollBarPos - 10, 0);
		break;
	case SB_PAGERIGHT:
		iScrollBarPos = min(iScrollBarPos + 10, GetScrollLimit(SB_HORZ));
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask	= SIF_TRACKPOS;

		if ( GetScrollInfo(SB_HORZ, &si, SIF_TRACKPOS) )
			iScrollBarPos = si.nTrackPos;
		else
			iScrollBarPos = (UINT) nPos;
		break;
	}
	m_ptImage.x = iScrollBarPos;
	SetScrollPos(SB_HORZ, iScrollBarPos);

	Invalidate();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CBitmapStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( message == WM_NCHITTEST || message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK )
		return ::DefWindowProc( m_hWnd, message, wParam, lParam );

	return CStatic::WindowProc(message, wParam, lParam);
}

// =========================================================================
// SetBitmap
BOOL CBitmapStatic::SetBitmap(PCSTR pszFilePath, CScrollBar*)
{
	BOOL bRet = TRUE;

	DeleteObject();

	HBITMAP hBmp = (HBITMAP) ::LoadImage(0,
		pszFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if ( hBmp == NULL ) return FALSE;

	m_bmImage.Attach(hBmp);

	BITMAP bitmap;
	m_bmImage.GetBitmap(&bitmap);

	m_sizeBitmap.cx = bitmap.bmWidth; m_sizeBitmap.cy = bitmap.bmHeight;

	if ( bitmap.bmHeight > m_rcClient.Height() ) {
		SCROLLINFO si = { sizeof(SCROLLINFO) };
		si.fMask = SIF_RANGE;
		si.nMax = bitmap.bmHeight - m_rcClient.Height();
		si.nMin = 0 ;

		SetScrollInfo(SB_VERT, &si);
		EnableScrollBarCtrl(SB_VERT, TRUE);
	}
	else {
		EnableScrollBarCtrl(SB_VERT, FALSE);
		SetScrollPos(SB_VERT, 0);
	}
	if ( bitmap.bmWidth > m_rcClient.Width() ) {
		SCROLLINFO si = { sizeof(SCROLLINFO) };
		si.fMask = SIF_RANGE;
		si.nMax = bitmap.bmWidth - m_rcClient.Width();
		si.nMin = 0 ;

		SetScrollInfo(SB_HORZ, &si);
		EnableScrollBarCtrl(SB_HORZ, TRUE);
	}
	else {
		SetScrollPos(SB_HORZ, 0);
		EnableScrollBarCtrl(SB_HORZ, FALSE);
	}

	Invalidate();

	return bRet;
}

// =========================================================================
// DrawBitmap
void CBitmapStatic::DrawBitmap(CDC* pDC)
{
	CBrush brBackground(RGB(0, 0, 0));
	if ( IsValid() ) {
		CRgn rgnClient, rgnBitmap;
		rgnClient.CreateRectRgnIndirect(&m_rcClient);
		rgnBitmap.CreateRectRgn(0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy);
		rgnClient.CombineRgn(&rgnClient, &rgnBitmap, RGN_DIFF);
		pDC->FillRgn(&rgnClient,	&brBackground);

		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap* oldBitmap = (CBitmap*) memDC.SelectObject(&m_bmImage);
		pDC->BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &memDC, m_ptImage.x, m_ptImage.y, SRCCOPY);
		memDC.SelectObject(oldBitmap);
	}
	else {
		pDC->FillRect(m_rcClient, &brBackground);
	}
	ReleaseDC(pDC);
}
