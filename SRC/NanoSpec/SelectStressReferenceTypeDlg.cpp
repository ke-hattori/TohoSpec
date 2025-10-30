// SelectStressReferenceTypeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "MultiLangHelper.h"
#include "SelectStressReferenceTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectStressReferenceTypeDlg ダイアログ


CSelectStressReferenceTypeDlg::CSelectStressReferenceTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectStressReferenceTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectStressReferenceTypeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_SelectedIndex = 0;
}


void CSelectStressReferenceTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectStressReferenceTypeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CSelectStressReferenceTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectStressReferenceTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectStressReferenceTypeDlg メッセージ ハンドラ

BOOL CSelectStressReferenceTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Referencr Type Combo /////
	ReferenceTypeCombo_Init();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSelectStressReferenceTypeDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_REFERENCE_TYPE);
	m_SelectedIndex = pCombo->GetCurSel();

	CDialog::OnOK();
}

void CSelectStressReferenceTypeDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

void CSelectStressReferenceTypeDlg::ReferenceTypeCombo_Init()
{
	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_REFERENCE_TYPE);

	pCombo->ResetContent();

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	CMLScanType mlSrHeadScanTypeItem(HEAD_TYPE_SR);
	CMLScanType mlStressHeadScanTypeItem(HEAD_TYPE_STRESS);

	pCombo->AddString(mlSrHeadScanTypeItem[0]);
	pCombo->AddString(mlStressHeadScanTypeItem[0]);

	pCombo->SetCurSel(0);

	return;
}

