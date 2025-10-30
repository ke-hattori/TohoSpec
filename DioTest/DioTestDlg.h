// DioTestDlg.h : header file
//

#if !defined(AFX_DIOTESTDLG_H__54BCD9D9_9EDF_4232_B861_7261D3B10873__INCLUDED_)
#define AFX_DIOTESTDLG_H__54BCD9D9_9EDF_4232_B861_7261D3B10873__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDioTestDlg dialog

class CDioTestDlg : public CDialog
{
// Construction
public:
	CDioTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDioTestDlg)
	enum { IDD = IDD_DIOTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDioTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDioTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInitial();
	afx_msg void OnUpper();
	afx_msg void OnShutterOpen();
	afx_msg void OnShutterClose();
	afx_msg void OnLoad();
	afx_msg void OnUnload();
	afx_msg void OnAlignment();
	afx_msg void OnClamp();
	afx_msg void OnUnclamp();
	afx_msg void OnVacuumOn();
	afx_msg void OnVacuumOff();
	afx_msg void OnDown();
	afx_msg void OnButton1();
	//}}AFX_MSG
    LRESULT OnNexResponse(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIOTESTDLG_H__54BCD9D9_9EDF_4232_B861_7261D3B10873__INCLUDED_)
