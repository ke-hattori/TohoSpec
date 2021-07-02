#if !defined(_INFORMDLG_H_)
#define _INFORMDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InformationDlg.h : header file
//

// ---------------------------------------------------------------------------
// CInformationDlg dialog

class CInformationDlg : public CDialog
{
// Construction
public:
	CInformationDlg(CWnd* pParent = NULL);	 // standard constructor

// Dialog Data
	//{{AFX_DATA(CInformationDlg)
	enum { IDD = IDD_INFORMATION_DIALOG };
	CString m_strText;
	//}}AFX_DATA
	CString m_strTitle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInformationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CInformationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UINT m_nTimeout;
};

#endif // !defined(_INFORMDLG_H_)
