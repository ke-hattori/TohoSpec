// ConfirmDlg.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg dialog

class CConfirmDlg : public CDialog
{
// Construction
public:
	CConfirmDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfirmDlg();

// Construction
	//{{AFX_DATA(CConfirmDlg)
	enum { IDD = IDD_CONFIRM_DLG };
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// implementation
	public:
	void SetButtonStyle(UINT nButtonStyle);
    void SetMessageText(LPCTSTR pszText, LPCTSTR pszCaption, UINT nIconType);

protected:
	void FontCreation();

	// Generated message map functions
	//{{AFX_MSG(CConfirmDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CStatic	m_MessageIcon;
	CStatic	m_MessageText;
	CFont m_Font;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	CString m_strMessageCaption;
	CString m_strMessageText;
	HICON m_hMessageIcon;
	UINT m_nButtonStyle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
