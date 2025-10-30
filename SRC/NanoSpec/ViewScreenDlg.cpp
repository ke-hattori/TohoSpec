// ViewScreenDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "ViewScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ##########################################################################
// CViewScreenDlg ダイアログ
// ##########################################################################

// ==========================================================================
//
CViewScreenDlg::CViewScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewScreenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewScreenDlg)
	//}}AFX_DATA_INIT
}

// ==========================================================================
//
void CViewScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewScreenDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CViewScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CViewScreenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ==========================================================================
// CViewScreenDlg メッセージ ハンドラ

// ==========================================================================
//
BOOL CViewScreenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OVLYMULTIWND ovlymw = { m_hWnd };
	GetDlgItem(IDC_VIDEO_SCREEN)->GetClientRect(&ovlymw.rc);
	// クライアントエッジ境界線使用時のロジック
	OffsetRect(&ovlymw.rc, GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE));
	OverlaySetMultipleWindow(&ovlymw);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ==========================================================================
void CViewScreenDlg::OnOK()
{
	CDialog::OnOK();
}

