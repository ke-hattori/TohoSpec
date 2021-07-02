// INFORMDLG.CPP : implementation file
//

#include "stdafx.h"
#include "common.hpp"
#include "informdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------
// CInformationDlg dialog


CInformationDlg::CInformationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInformationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInformationDlg)
	//}}AFX_DATA_INIT
}

void CInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInformationDlg)
	DDX_Text(pDX, IDC_TEXT, m_strText);
	//}}AFX_DATA_MAP
	m_nTimeout = 100;
}

BEGIN_MESSAGE_MAP(CInformationDlg, CDialog)
	//{{AFX_MSG_MAP(CInformationDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------
// CInformationDlg message handlers
BOOL CInformationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText((PCSTR) m_strTitle);
	SetDlgItemText(IDC_TEXT, (PCSTR) m_strText);

	return TRUE;
}

// ---------------------------------------------------------------------------
// OnCancel
void CInformationDlg::OnCancel()
{
	CDialog::OnCancel();
}

// ---------------------------------------------------------------------------
// OnOK
void CInformationDlg::OnOK()
{
	CDialog::OnOK();
}
