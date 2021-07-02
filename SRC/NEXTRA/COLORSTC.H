#if !defined(AFX_COLORSTATIC_H__FA06847D_DC9A_43A0_8838_F8EC9A5018A6__INCLUDED_)
#define AFX_COLORSTATIC_H__FA06847D_DC9A_43A0_8838_F8EC9A5018A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : header file
//

// ---------------------------------------------------------------------------
// CColorStatic window

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();
	enum {
			ST_FLS_SLOW,
			ST_FLS_NORMAL,
			ST_FLS_FAST
		};
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorStatic();

	BOOL SetIcon(int nIcon, BOOL bRepaint = TRUE, HINSTANCE hInstance = NULL);
	BOOL SetIcon(HICON hIcon, BOOL bRepaint = TRUE);

	void SetTextColor(COLORREF crTextColor = 0xFFFFFFFF, BOOL bReapaint = TRUE);

	void SetBkColor(COLORREF crBkColor = 0xFFFFFFFF, BOOL bRepaint = TRUE);

	void SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2);
	void StartTextBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

	void SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2, BOOL bRepaint = TRUE);
	void StartBkBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

	void EnableNotify(CWnd* pParent = NULL, UINT nMsg = WM_USER);

	// Generated message map functions
protected:
	virtual HRESULT OnDrawBackground(CDC* pDC, LPCRECT pRect);

	//{{AFX_MSG(CColorStatic)
	afx_msg void OnPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void FreeResources(BOOL bCheckForNULL = TRUE);
	void DrawItem(CDC* pDC);
	void DrawTheIcon(CDC* pDC, CRect* rpCtrl, BOOL bText);
	void DrawTheText(CDC* pDC, CRect* rpCtrl, CString& sText, DWORD dwStyle, DWORD dwExStyle);

	HICON m_hIcon;
	BYTE m_cxIcon;
	BYTE m_cyIcon;

	UINT m_nTimerId;

	COLORREF m_crTextColor;
	COLORREF m_crBlinkTextColors[2];
	BOOL m_bTextBlink;
	int m_nTextBlinkStep;

	COLORREF m_crBkColor;
	COLORREF m_crBlinkBkColors[2];
	BOOL m_bBkBlink;
	int m_nBkBlinkStep;

	CBrush m_brBkgnd;
	CBrush m_brBlinkBkgnd[2];

	CWnd* m_pParent;
	UINT m_nMsg;
};

// ---------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATIC_H__FA06847D_DC9A_43A0_8838_F8EC9A5018A6__INCLUDED_)
