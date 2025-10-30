// RestartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Terminator.h"
#include "RestartDlg.h"
#include "TerminatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int m_Mode;
DWORD m_Color;
DWORD RED_COLOR		RGB(255,0,0);
DWORD BLUE_COLOR	RGB(0,0,255);
/////////////////////////////////////////////////////////////////////////////
// CRestartDlg dialog


CRestartDlg::CRestartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRestartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRestartDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CRestartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRestartDlg)
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRestartDlg, CDialog)
	//{{AFX_MSG_MAP(CRestartDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRestartDlg message handlers

void CRestartDlg::OnOK() 
{
	CString SA;
	GetDlgItem(IDC_COMBO1)->GetWindowText(SA);

	if (SA == "Restart Windows")
				m_Mode = 0;//(EWX_REBOOT);

	else if (SA == "Shut Down Windows")
				m_Mode = 1;//(EWX_POWEROFF);

	else if (SA == "Log Off")
				m_Mode = 2;//(EWX_LOGOFF);

	else if (SA == "Terminal orca application")
				m_Mode = 3;//(Terminal orca application)

	CDialog::OnOK();
}


BOOL CRestartDlg::OnInitDialog() 
{ 
// Goto add 2006.1226 --->
    CButton* pBtn = (CButton *)GetDlgItem(IDCANCEL);
    pBtn->ShowWindow(SW_HIDE);
// Goto add 2006.1226 <---
	::SetForegroundWindow(this->GetSafeHwnd());
	CDialog::OnInitDialog();
	CRect box;
	GetWindowRect(&box);
	SetWindowPos(NULL,
				 (GetSystemMetrics(SM_CXSCREEN) - box.Width())/ 2,
				 (GetSystemMetrics(SM_CYSCREEN) - box.Height()) / 2,
				 300, 100, SWP_NOSIZE | SWP_NOZORDER);
	m_Combo.SetFocus();
	m_Combo.SetCurSel(0);
	m_Color = (COLORREF)BLUE_COLOR;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRestartDlg::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	DoColorText();

}

void CRestartDlg::DoColorText()
{
	HDC hdc;
	PAINTSTRUCT ps;
	static CFont hFont;
	static HFONT hFontOld;

	hdc = ::BeginPaint(GetDlgItem(IDC_STATIC_MSG)->GetSafeHwnd(),&ps);

	hFont.CreateFont(18,6,0,0,
					FW_SEMIBOLD,
					0,0,0,
					ANSI_CHARSET,				//SHIFTJIS_CHARSET
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					ANTIALIASED_QUALITY,
					FIXED_PITCH,
					"Times New Roman");			//Arial Narrow//Times New Roman//Century//Arial

	hFontOld = (HFONT)SelectObject(hdc, hFont);

	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc, (COLORREF)m_Color);
	TextOut(hdc, 5, 0, "A Fatal System Error Occurred.", 30);

	SelectObject(hdc, hFontOld);		// フォントオブジェクトを元に戻す
	DeleteObject(hFont);				// フォントオブジェクトの解放
	::EndPaint(GetDlgItem(IDC_STATIC_MSG)->GetSafeHwnd(), &ps);


}
