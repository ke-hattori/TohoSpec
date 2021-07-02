// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "common.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// Construction/Destruction
// ===========================================================================
CColorStatic::CColorStatic()
{
	FreeResources(FALSE);

	m_pParent	= NULL;
	m_nMsg		= WM_USER;

	// Default Color
	m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);
	m_crBkColor   = ::GetSysColor(COLOR_BTNFACE);
	m_brBkgnd.CreateSolidBrush(m_crBkColor);

	// For Blink
	m_crBlinkTextColors[0] = m_crTextColor;
	m_crBlinkTextColors[1] = m_crTextColor;
	m_crBlinkBkColors[0]   = m_crBkColor;
	m_crBlinkBkColors[1]   = m_crBkColor;
	m_bTextBlink		   = FALSE;
	m_nTextBlinkStep	   = 0;
	m_bBkBlink			   = FALSE;
	m_nBkBlinkStep		   = 0;
	m_nTimerId			   = 0;
	m_brBlinkBkgnd[0].CreateSolidBrush(m_crBkColor);
	m_brBlinkBkgnd[1].CreateSolidBrush(m_crBkColor);
}

CColorStatic::~CColorStatic()
{
	FreeResources(FALSE);
}

BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ===========================================================================
// CColorStatic message handlers
// ===========================================================================

// ---------------------------------------------------------------------------
HRESULT CColorStatic::OnDrawBackground(CDC* pDC, LPCRECT pRect)
{
	CBrush br(m_crBkColor);
	pDC->FillRect(pRect, &br);

	return 0L;
}

// ---------------------------------------------------------------------------
void CColorStatic::OnPaint()
{
	CPaintDC dc(this);

	DrawItem(&dc);
}

// ---------------------------------------------------------------------------
HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// Text Blink ?
	if ( m_bTextBlink )
		pDC->SetTextColor(m_crBlinkTextColors[m_nTextBlinkStep]);
	else
		pDC->SetTextColor(m_crTextColor);

	// Background Blink ?
	if ( m_bBkBlink ) {
		pDC->SetBkColor(m_crBlinkBkColors[m_nBkBlinkStep]);
		return (HBRUSH)m_brBlinkBkgnd[m_nBkBlinkStep];
	}

	pDC->SetBkColor(m_crBkColor);

	return (HBRUSH)m_brBkgnd;
}

// ---------------------------------------------------------------------------
void CColorStatic::OnDestroy()
{
	CStatic::OnDestroy();

	if ( m_nTimerId > 0 )
		KillTimer(m_nTimerId);

	m_brBkgnd.DeleteObject();
	m_brBlinkBkgnd[0].DeleteObject();
	m_brBlinkBkgnd[1].DeleteObject();
}

// ---------------------------------------------------------------------------
void CColorStatic::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent == m_nTimerId ) {
		// Text Blink
		if ( m_bTextBlink )
			m_nTextBlinkStep = !m_nTextBlinkStep;

		// Background Blink
		if ( m_bBkBlink )
			m_nBkBlinkStep = !m_nBkBlinkStep;

		if ( m_bBkBlink || m_bTextBlink ) {
			Invalidate();
			if ( m_pParent != NULL && (m_nBkBlinkStep == 1 || m_nTextBlinkStep == 1) )
				m_pParent->PostMessage(m_nMsg, GetDlgCtrlID(), 0);
		}
	}
	else
		CStatic::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------
void CColorStatic::FreeResources(BOOL bCheckForNULL)
{
	if ( bCheckForNULL ) {
		if ( m_hIcon != NULL )
			::DestroyIcon(m_hIcon);
	}

	m_hIcon = NULL;
	m_cxIcon = 0;
	m_cyIcon = 0;
}

// ---------------------------------------------------------------------------
void CColorStatic::DrawItem(CDC* pDC)
{
	CString sText;
	CRect rCtrl;

	GetClientRect(rCtrl);

	DWORD dwStyle	= GetStyle();
	DWORD dwExStyle = GetExStyle();

	OnDrawBackground(pDC, &rCtrl);

	if ( dwStyle & SS_SUNKEN ) {
		pDC->Draw3dRect(rCtrl, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
		rCtrl.DeflateRect(1, 1);
	}

	if ( dwExStyle & WS_EX_CLIENTEDGE ) {
		pDC->Draw3dRect(rCtrl, RGB(0,0,0), RGB(0,0,0));
		rCtrl.DeflateRect(1, 1);
	}

	GetWindowText(sText);

	// Draw icon
	if ( m_hIcon ) DrawTheIcon(pDC, &rCtrl, (BOOL)(sText.IsEmpty() == 0));

	// Draw text
	DrawTheText(pDC, &rCtrl, sText, dwStyle, dwExStyle);
}

// ---------------------------------------------------------------------------
void CColorStatic::DrawTheIcon(CDC* pDC, CRect* rpCtrl, BOOL bText)
{
	int 	nX = 0, nY = 0;
	CRect	rIcon;

	rIcon.CopyRect(rpCtrl);

	// If there is text
	if ( bText ) {
		rIcon.right = rIcon.left + (4+m_cxIcon+4);
		rpCtrl->left = rIcon.right;
	}

	nX = rIcon.left;
	nY = rIcon.top;

	nX += (rIcon.Width() - m_cxIcon) / 2;

	nY += (rIcon.Height() - m_cyIcon) / 2;

	pDC->DrawState( CPoint(nX, nY),
					CSize(m_cxIcon, m_cyIcon),
					m_hIcon,
					DSS_NORMAL,
					(CBrush *)NULL);
}

// ---------------------------------------------------------------------------
void CColorStatic::DrawTheText(CDC* pDC, CRect* rpCtrl, CString& sText, DWORD dwStyle, DWORD dwExStyle)
{
	CRect	rText;
	int 	nX = 0, nY = 0;
	UINT	nFormat = DT_EXPANDTABS;

	// Transparent background
	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(m_crTextColor);

	// Set same font of the window
	CFont* oldFont = pDC->SelectObject(GetFont());

	if ( (dwStyle & SS_NOPREFIX) == SS_NOPREFIX ) nFormat |= DT_NOPREFIX;
	if ( (dwStyle & SS_LEFTNOWORDWRAP) != SS_LEFTNOWORDWRAP ) nFormat |= DT_WORDBREAK;

	rText = rpCtrl;
	pDC->DrawText(sText, -1, rText, nFormat | DT_CALCRECT);

	// Center horizontally
	if ( dwStyle & SS_CENTER )
		nX += (rpCtrl->Width() - rText.Width())/2;
	else if ( dwStyle & SS_RIGHT )
		nX += rpCtrl->Width() - rText.Width();

	// Center vertically
	if ( dwStyle & SS_CENTERIMAGE )
		nY += (rpCtrl->Height() - rText.Height())/2;

	rText.OffsetRect(nX, nY);

	pDC->DrawText(sText, -1, rText, nFormat);

	// Restore old font
	pDC->SelectObject(oldFont);
}

// ---------------------------------------------------------------------------
BOOL CColorStatic::SetIcon(int nIcon, BOOL bRepaint, HINSTANCE hInstance)
{
	HINSTANCE hInstResource;
	HICON	   hIcon;

	if ( hInstance == NULL )
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIcon), RT_GROUP_ICON);
	else
		hInstResource = hInstance;

	// Load icon from resource
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIcon), IMAGE_ICON, 0, 0, 0);

	return SetIcon(hIcon, bRepaint);
}

// ---------------------------------------------------------------------------
BOOL CColorStatic::SetIcon(HICON hIcon, BOOL bRepaint)
{
	BOOL		bRetValue;
	ICONINFO	ii;

	// Free any loaded resource
	FreeResources();

	if ( hIcon ) {
		m_hIcon = hIcon;

		// Get icon dimension
		ZeroMemory(&ii, sizeof(ICONINFO));
		bRetValue = ::GetIconInfo(m_hIcon, &ii);
		if ( !bRetValue ) {
			FreeResources();
			return bRetValue;
		}

		m_cxIcon = (BYTE)(ii.xHotspot * 2);
		m_cyIcon = (BYTE)(ii.yHotspot * 2);
		::DeleteObject(ii.hbmMask);
		::DeleteObject(ii.hbmColor);
	}

	if ( bRepaint ) Invalidate();

	return TRUE;
}

// ---------------------------------------------------------------------------
void CColorStatic::SetTextColor(COLORREF crTextColor/*=0xFFFFFFFF*/, BOOL bRepaint/*=TRUE*/)
{
	if ( crTextColor != 0xFFFFFFFF )
		m_crTextColor = crTextColor;
	else
		m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);

	if ( bRepaint )
		Invalidate();
}

// ---------------------------------------------------------------------------
void CColorStatic::SetBkColor(COLORREF crBkColor/*=0xFFFFFFFF*/, BOOL bRepaint/*=TRUE*/)
{
	if ( crBkColor != 0XFFFFFFFF )
		m_crBkColor = crBkColor;
	else
		m_crBkColor = ::GetSysColor(COLOR_BTNFACE);

	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(m_crBkColor);

	if ( bRepaint )
		Invalidate();
}

// ---------------------------------------------------------------------------
void CColorStatic::SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2)
{
	m_crBlinkTextColors[0] = crBlinkTextColor1;
	m_crBlinkTextColors[1] = crBlinkTextColor2;
}

// ---------------------------------------------------------------------------
void CColorStatic::StartTextBlink(BOOL bStart/*=TRUE*/, UINT nElapse/*=ST_FLS_NORMAL*/)
{
	UINT nCount;

	if ( m_nTimerId > 0 ) {
		KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}

	m_bTextBlink = bStart;
	m_nTextBlinkStep = 0;

	if ( m_bTextBlink ) {
		switch ( nElapse ) {
		case ST_FLS_SLOW:
			nCount = 2000;
			break;
		case ST_FLS_NORMAL:
			nCount = 1000;
			break;
		case ST_FLS_FAST:
			nCount = 500;
			break;
		default:
			nCount = nElapse;
			break;
		}
		m_nTimerId = SetTimer(1, nCount, NULL);
	}
}

// ---------------------------------------------------------------------------
void CColorStatic::SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2, BOOL bRepaint/*=TRUE*/)
{
	m_crBlinkBkColors[0] = crBlinkBkColor1;
	m_crBlinkBkColors[1] = crBlinkBkColor2;

	m_brBlinkBkgnd[0].DeleteObject();
	m_brBlinkBkgnd[0].CreateSolidBrush(m_crBlinkBkColors[0]);
	m_brBlinkBkgnd[1].DeleteObject();
	m_brBlinkBkgnd[1].CreateSolidBrush(m_crBlinkBkColors[1]);

	if ( bRepaint )
		Invalidate();
}

// ---------------------------------------------------------------------------
void CColorStatic::StartBkBlink(BOOL bStart, UINT nElapse)
{
	UINT nCount;

	if ( m_nTimerId > 0 ) {
		KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}

	m_bBkBlink = bStart;
	m_nBkBlinkStep = 0;

	if ( m_bBkBlink ) {
		switch ( nElapse ) {
		case ST_FLS_SLOW:
			nCount = 2000;
			break;
		case ST_FLS_NORMAL:
			nCount = 1000;
			break;
		case ST_FLS_FAST:
			nCount = 500;
			break;
		default:
			nCount = nElapse;
			break;
		}
		m_nTimerId = SetTimer(1, nCount, NULL);
	}
}

// ---------------------------------------------------------------------------
void CColorStatic::EnableNotify(CWnd* pParent, UINT nMsg)
{
	m_pParent = pParent;
	m_nMsg = nMsg;
}
