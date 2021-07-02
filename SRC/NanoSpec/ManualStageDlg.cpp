// ManualStageDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "ManualStageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualStageDlg ダイアログ											   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CManualStageDlg::CManualStageDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CManualStageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualStageDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

// 2009.10.05 bagus SE --{--
	::ZeroMemory(m_szRecipeName, sizeof(m_szRecipeName));
	m_nSampleMode = 0;
// 2009.10.05 bagus SE --}--
}

/////////////////////////////////////////////////////////////////////////////
//
void CManualStageDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualStageDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CManualStageDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CManualStageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualStageDlg メッセージ ハンドラ									   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CManualStageDlg::OnInitDialog()
{
// 2009.10.05 bagus SE --{--
	//Sampleセット
	if (m_nSampleMode == 1) {
		SetFileName(m_szRecipeName);
	}
	SetSampleMode(m_nSampleMode);
// 2009.10.05 bagus SE --}--

	COverlayDialog::OnInitDialog();

	// Cancelボタン非表示 と Okボタン移動
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	CRect rect;
	GetDlgItem(IDCANCEL)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDOK)->MoveWindow(rect);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CManualStageDlg::OnOK()
{
	COverlayDialog::OnOK();
}

void CManualStageDlg::OnCancel()
{
	COverlayDialog::OnCancel();
}
