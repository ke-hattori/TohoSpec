// CcdCalibrationDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "system.h"
#include "nanospec.h"
#include "ccdcalibrationdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CCcdCalibrationDlg ダイアログ
// #########################################################################

// =========================================================================
//
CCcdCalibrationDlg::CCcdCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCcdCalibrationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCcdCalibrationDlg)
	//}}AFX_DATA_INIT
	m_iSelect = 0;
}

// =========================================================================
//
void CCcdCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCcdCalibrationDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CCcdCalibrationDlg, CDialog)
	//{{AFX_MSG_MAP(CCcdCalibrationDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CCcdCalibrationDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CCcdCalibrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Combo Box Initialize /////
	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_WAVELENGTH);
	pCombo->ResetContent();
	for(int i = 0; i < SR_CCD_ARRAY_ITEM_MAX; i++){
		pCombo->AddString(g_lpszSrCcdArrayItem[i]);
	}
	pCombo->SetCurSel(0);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CCcdCalibrationDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	m_iSelect = ((CComboBox *)GetDlgItem(IDC_WAVELENGTH))->GetCurSel();

	CDialog::OnOK();
}
