#if !defined(AFX_COLORBUTTON_H__CBE89EEF_7971_4A8D_BC13_20A06C858567__INCLUDED_)
#define AFX_COLORBUTTON_H__CBE89EEF_7971_4A8D_BC13_20A06C858567__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorButton.h : header file
//

const LPCTSTR lpszEventText[] =
	{
		"LBUTTON DOWN",
		"LBUTTON UP",
		"LBUTTON DBLCLK",
		"RBUTTON DOWN",
		"RBUTTON UP",
		"RBUTTON DBLCLK",
	};

// ---------------------------------------------------------------------------
// CColorButton window

class CColorButton : public CButton
{
// Construction
public:
	CColorButton();
	enum
	{
		ST_FLS_SLOW,
		ST_FLS_NORMAL,
		ST_FLS_FAST
	};

	enum
	{
		EV_LBUTTONDOWN,
		EV_LBUTTONUP,
		EV_LBUTTONDBLCLK,
		EV_RBUTTONDOWN,
		EV_RBUTTONUP,
		EV_RBUTTONDBLCLK
	};
	static LPCTSTR GetEventText(UINT nEvent) { return lpszEventText[nEvent]; };

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorButton();
	void EnableNotify(CWnd* pParent, UINT nMsg);

	void SetTextColor(COLORREF crTextColor = ::GetSysColor(COLOR_BTNTEXT), BOOL bRepaint = TRUE);
	COLORREF GetTextColor();
	void SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2);
	void StartTextBlink(BOOL bStat = TRUE, UINT nElapse = ST_FLS_NORMAL);

	void SetBkColor(COLORREF crBkCOlor = ::GetSysColor(COLOR_BTNFACE), BOOL bRepaint = TRUE);
	COLORREF GetBkColor();
	void SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2);
	void StartBkBlink(BOOL bStat = TRUE, UINT nElapse = ST_FLS_NORMAL);

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT m_nMsg;
	CWnd* m_pParent;

	int m_nBkBlinkStep;
	BOOL m_bBkBlink;
	COLORREF m_crBlinkBkColors[2];
	COLORREF m_crBkColor;

	int m_nTextBlinkStep;
	BOOL m_bTextBlink;
	COLORREF m_crBlinkTextColors[2];
	COLORREF m_crTextColor;

	UINT m_nTimerId;

	BOOL m_bIsDisabled;
	BOOL m_bIsDefaut;
	BOOL m_bIsFocused;
	BOOL m_bIsPressed;

	void DrawFilledRect(CDC* pDC, CRect rect, COLORREF crBkColor);
	void DrawBorder(CDC* pDC, CRect* pRect);
	void DrawTheText(CDC* pDC, CRect rect, LPCSTR lpszText, COLORREF crTextColor);
};

// ---------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__CBE89EEF_7971_4A8D_BC13_20A06C858567__INCLUDED_)
