// SYSCFG.H : header file
//

#if !defined(_SYSCFGDLG_H_)
#define _SYSCFGDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ===========================================================================
// CSystemConfigurationDlg dialog
// ===========================================================================

class CSystemConfigurationDlg : public CDialog
{
// Construction
public:
	CSystemConfigurationDlg(CWnd* pParent = NULL);	 // standard constructor

// Dialog Data
	//{{AFX_DATA(CSystemConfigurationDlg)
	enum { IDD = IDD_SYSTEM_CFG_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemConfigurationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSystemConfigurationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	NEXINIPARAMS m_params;
};

#endif // !defined(_SYSCFGDLG_H_)
