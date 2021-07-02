// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "common.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// CColorButton
// ===========================================================================

CColorButton::CColorButton()
{
	m_pParent		 = NULL;
	m_nMsg			 = WM_USER;
	m_bTextBlink	 = FALSE;
	m_nTextBlinkStep = 0;
	m_bBkBlink		 = FALSE;
	m_nBkBlinkStep	 = 0;
	m_nTimerId		 = 0;

	// Set Default Text Colors
	m_crTextColor		   = ::GetSysColor(COLOR_BTNTEXT);
	m_crBlinkTextColors[0] = m_crTextColor;
	m_crBlinkTextColors[1] = m_crTextColor;

	// Set Default Backgroud Colors
	m_crBkColor 		 = ::GetSysColor(COLOR_BTNFACE);
	m_crBlinkBkColors[0] = m_crBkColor;
	m_crBlinkBkColors[1] = m_crBkColor;
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ENABLE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ===========================================================================
// CColorButton message handlers
// ===========================================================================

// ---------------------------------------------------------------------------
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC*	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	m_bIsPressed  = lpDrawItemStruct->itemState & ODS_SELECTED;
	m_bIsFocused  = lpDrawItemStruct->itemState & ODS_FOCUS;
	m_bIsDefaut   = lpDrawItemStruct->itemState & ODS_DEFAULT;
	m_bIsDisabled = lpDrawItemStruct->itemState & ODS_DISABLED;

	CRect itemRect = lpDrawItemStruct->rcItem;

	// Read the button's titile
	CString strTitle;
	GetWindowText(strTitle);

	CRect captionRect = lpDrawItemStruct->rcItem;

	if ( m_bIsDisabled )
		DrawFilledRect(pDC, itemRect, ::GetSysColor(COLOR_BTNFACE));
	else
		DrawFilledRect(pDC, itemRect, GetBkColor());

	DrawBorder(pDC, &itemRect);

	CRect textRect = captionRect;
	textRect.OffsetRect(-1, 0);

	CRect offsetRect = textRect;
	offsetRect.OffsetRect(1, 1);

	if ( m_bIsDisabled ) {
		DrawTheText(pDC, offsetRect, strTitle, GetSysColor(COLOR_BTNHILIGHT));
		DrawTheText(pDC, textRect, strTitle, GetSysColor(COLOR_BTNSHADOW));
	}
	else {
		if ( m_bIsPressed ) {
			DrawTheText(pDC, offsetRect, strTitle, GetTextColor());
		}
		else {
			  DrawTheText(pDC, textRect, strTitle, GetTextColor());
		}
	}

	if ( m_bIsFocused ) {
		CRect focusRect = itemRect;
		focusRect.DeflateRect(3, 3);
		pDC->DrawFocusRect(&focusRect);
	}
}

// ---------------------------------------------------------------------------
BOOL CColorButton::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_LBUTTONDBLCLK )
		pMsg->message = WM_LBUTTONDOWN;

	return CButton::PreTranslateMessage(pMsg);
}

// ---------------------------------------------------------------------------
void CColorButton::OnDestroy()
{
	CButton::OnDestroy();
	// Destroy timer (if any)
	if ( m_nTimerId > 0 ) KillTimer(m_nTimerId);
}

// ---------------------------------------------------------------------------
void CColorButton::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent == m_nTimerId ) {
		// If control is blinking (text) switch its color
		if ( m_bTextBlink ) m_nTextBlinkStep = !m_nTextBlinkStep;
		else m_nTextBlinkStep =0;

		// If control is blinking (background) switch its color
		if ( m_bBkBlink ) m_nBkBlinkStep = !m_nBkBlinkStep;
		else m_nBkBlinkStep = 0;
		// If there is any blinking in action then repaint the control
		// and send the notification message (if any)
		if ( m_bBkBlink || m_bTextBlink ) {
			Invalidate();
			// Send notification message only on rising blink
			if ( m_pParent != NULL && ( m_nBkBlinkStep == 1 || m_nTextBlinkStep == 1 ) )
				m_pParent->PostMessage(m_nMsg, GetDlgCtrlID(), 0);
		}
	}
	else
	CButton::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------
void CColorButton::EnableNotify(CWnd* pParent, UINT nMsg)
{
	m_pParent = pParent;
	m_nMsg = nMsg;
}

// ---------------------------------------------------------------------------
void CColorButton::SetTextColor(COLORREF crTextColor/*=::GetSysColor(COLOR_BTNTEXT)*/, BOOL bRepaint/*= TRUE*/)
{
	m_crTextColor = crTextColor;
	if ( bRepaint )
		Invalidate();
}

// ---------------------------------------------------------------------------
COLORREF CColorButton::GetTextColor()
{
	return m_bTextBlink ? m_crBlinkTextColors[m_nBkBlinkStep] : m_crTextColor;
}

// ---------------------------------------------------------------------------
void CColorButton::SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2)
{
	// Set new blink text colors
	m_crBlinkTextColors[0] = crBlinkTextColor1;
	m_crBlinkTextColors[1] = crBlinkTextColor2;
}

// ---------------------------------------------------------------------------
void CColorButton::StartTextBlink(BOOL bStart, UINT nElapse)
{
	UINT nCount;

	// Destroy any previous timer
	if ( m_nTimerId > 0 ) {
		KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}

	m_bTextBlink = bStart;
	m_nTextBlinkStep = 0;

	// Start timer
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
void CColorButton::SetBkColor(COLORREF crBkColor/*=::GetSysColor(COLOR_BTNFACE)*/, BOOL bRepaint/*= TRUE*/)
{
	m_crBkColor = crBkColor;
	if ( bRepaint )
		Invalidate();
}

// ---------------------------------------------------------------------------
COLORREF CColorButton::GetBkColor()
{
	return m_bBkBlink ? m_crBlinkBkColors[m_nBkBlinkStep] : m_crBkColor;
}

// ---------------------------------------------------------------------------
void CColorButton::SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2)
{
	// Set new blink text colors
	m_crBlinkBkColors[0] = crBlinkBkColor1;
	m_crBlinkBkColors[1] = crBlinkBkColor2;
}

// ---------------------------------------------------------------------------
void CColorButton::StartBkBlink(BOOL bStart, UINT nElapse)
{
	UINT nCount;

	// Destroy any previous timer
	if ( m_nTimerId > 0 ) {
		KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}

	m_bBkBlink = bStart;
	m_nBkBlinkStep = 0;

	// Start timer
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
void CColorButton::DrawFilledRect(CDC* pDC, CRect rect, COLORREF crBkColor)
{
	CBrush brush;

	brush.CreateSolidBrush(crBkColor);
	pDC->FillRect(rect, &brush);
}

// ---------------------------------------------------------------------------
void CColorButton::DrawBorder(CDC* pDC, CRect* pRect)
{
	if ( m_bIsPressed ) {
			CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
			pDC->FrameRect(pRect, &brBtnShadow);
	}
	else {
		CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
		CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));		// Light gray
		CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));	// Dark gray
		CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black

		// Draw top-left borders
		// White line
		CPen* pOldPen = pDC->SelectObject(&penBtnHiLight);
		pDC->MoveTo(pRect->left, pRect->bottom-1);
		pDC->LineTo(pRect->left, pRect->top);
		pDC->LineTo(pRect->right, pRect->top);
		// Light gray line
		pDC->SelectObject(pen3DLight);
		pDC->MoveTo(pRect->left+1, pRect->bottom-1);
		pDC->LineTo(pRect->left+1, pRect->top+1);
		pDC->LineTo(pRect->right, pRect->top+1);
		// Draw bottom-right borders
		// Black line
		pDC->SelectObject(pen3DDKShadow);
		pDC->MoveTo(pRect->left, pRect->bottom-1);
		pDC->LineTo(pRect->right-1, pRect->bottom-1);
		pDC->LineTo(pRect->right-1, pRect->top-1);
		// Dark gray line
		pDC->SelectObject(penBtnShadow);
		pDC->MoveTo(pRect->left+1, pRect->bottom-2);
		pDC->LineTo(pRect->right-2, pRect->bottom-2);
		pDC->LineTo(pRect->right-2, pRect->top);

		pDC->SelectObject(pOldPen);
	}
}

// ---------------------------------------------------------------------------
void CColorButton::DrawTheText(CDC* pDC, CRect rect, LPCSTR lpszText, COLORREF crTextColor)
{
	COLORREF crPreviousColour;
	crPreviousColour = pDC->SetTextColor(crTextColor);

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(lpszText, -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SetTextColor(crPreviousColour);
}

// ---------------------------------------------------------------------------
void CColorButton::OnEnable(BOOL bEnable)
{
	CButton::OnEnable(bEnable);

	if ( bEnable ) {
		CWnd* pWnd = GetParent()->GetNextDlgTabItem(this);
		if ( pWnd )
			pWnd->SetFocus();
		else
			GetParent()->SetFocus();
	}
}

// ---------------------------------------------------------------------------
void CColorButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if ( pParent != NULL )
		pParent->SendMessage(WM_BUTTON_EVENT, (WPARAM)EV_LBUTTONDOWN, (LPARAM)CWnd::GetDlgCtrlID());

	CButton::OnLButtonDown(nFlags, point);
}

// ---------------------------------------------------------------------------
void CColorButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if ( pParent != NULL )
		pParent->SendMessage(WM_BUTTON_EVENT, (WPARAM)EV_LBUTTONUP, (LPARAM)CWnd::GetDlgCtrlID());

	CButton::OnLButtonUp(nFlags, point);
}

// ---------------------------------------------------------------------------
void CColorButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if ( pParent != NULL )
		pParent->PostMessage(WM_BUTTON_EVENT, (WPARAM)EV_LBUTTONDBLCLK, (LPARAM)CWnd::GetDlgCtrlID());

	CButton::OnLButtonDblClk(nFlags, point);
}

// ---------------------------------------------------------------------------
void CColorButton::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if ( pParent != NULL )
		pParent->PostMessage(WM_BUTTON_EVENT, (WPARAM)EV_RBUTTONDOWN, (LPARAM)CWnd::GetDlgCtrlID());

	CButton::OnRButtonDown(nFlags, point);
}

// ---------------------------------------------------------------------------
void CColorButton::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if ( pParent != NULL )
		pParent->PostMessage(WM_BUTTON_EVENT, (WPARAM)EV_RBUTTONUP, (LPARAM)CWnd::GetDlgCtrlID());

	CButton::OnRButtonUp(nFlags, point);
}

// ---------------------------------------------------------------------------
void CColorButton::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd *pParent = GetParent();

	if ( pParent != NULL )
		pParent->PostMessage(WM_BUTTON_EVENT, (WPARAM)EV_RBUTTONDBLCLK, (LPARAM)CWnd::GetDlgCtrlID());

	CButton::OnRButtonDblClk(nFlags, point);
}
