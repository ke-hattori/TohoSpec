#if !defined(AFX_RESTARTDLG_H__89BDA6DB_185D_4AA2_917E_4FE4CB326CFB__INCLUDED_)
#define AFX_RESTARTDLG_H__89BDA6DB_185D_4AA2_917E_4FE4CB326CFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RestartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRestartDlg dialog

class CRestartDlg : public CDialog
{
// Construction
public:
	CRestartDlg(CWnd* pParent = NULL);   // standard constructor
	void DoColorText();
	// Dialog Data
	//{{AFX_DATA(CRestartDlg)
	enum { IDD = IDD_RESTART };
	CComboBox	m_Combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRestartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRestartDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESTARTDLG_H__89BDA6DB_185D_4AA2_917E_4FE4CB326CFB__INCLUDED_)
