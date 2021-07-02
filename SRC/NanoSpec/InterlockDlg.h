#pragma once

/////////////////////////////////////////////////////////////////////////////
// CInerlockDlg class definition

#include "resource.h"

class CInterlockDlg : public CDialog
{
// Construction
public:
	CInterlockDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInterlockDlg)
	enum { IDD = IDD_INTERLOCK };
	CString m_InterlockMsg;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// Generated message map functions
	//{{AFX_MSG(CInterlockDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
