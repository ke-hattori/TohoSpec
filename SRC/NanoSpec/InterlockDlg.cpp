// CInterlockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "InterlockDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CInterlockDlg class implementation

CInterlockDlg::CInterlockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInterlockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInterlockDlg)
	//}}AFX_DATA_INIT
	m_InterlockMsg.LoadString(IDS_INTERLOCK_DEF_MSG);
}

void CInterlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterlockDlg)
	DDX_Text(pDX, IDC_INTERLOCK_MSG, m_InterlockMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInterlockDlg, CDialog)
	//{{AFX_MSG_MAP(CInterlockDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CInterlockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;	// return TRUE	unless you set the focus to a control
}
