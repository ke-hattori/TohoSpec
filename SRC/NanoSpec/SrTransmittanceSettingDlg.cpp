// SrTransmittanceSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "SrTransmittanceSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrTransmittanceSettingDlg ダイアログ


CSrTransmittanceSettingDlg::CSrTransmittanceSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrTransmittanceSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrTransmittanceSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CSrTransmittanceSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrTransmittanceSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CSrTransmittanceSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrTransmittanceSettingDlg)
	ON_BN_CLICKED(IDC_CHK_DONOT_CHECK_MEAS_POINT, OnChkDonotCheckMeasPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrTransmittanceSettingDlg メッセージ ハンドラ

BOOL CSrTransmittanceSettingDlg::OnInitDialog()
{
	memset(&m_SrTransmittance, 0, sizeof(m_SrTransmittance));
	ConfigFile_GetNanoSpecIni(&m_SrTransmittance, CONFIG_FILE_SR_TRANSMIT);

	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Set Data /////
	UpDate(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSrTransmittanceSettingDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	///// Save /////
	UpDate(TRUE);

	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_SrTransmittance, CONFIG_FILE_SR_TRANSMIT);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_SR_INI_FILE);

	CDialog::OnOK();
}

void CSrTransmittanceSettingDlg::OnChkDonotCheckMeasPoint()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

}

// =========================================================================
//
void CSrTransmittanceSettingDlg::UpDate(BOOL bValid)
{
	UINT	uCheck;

	if ( bValid ) {
		///// Option Auto Focus
		m_SrTransmittance.bDoNotCheckMeasPoint = IsDlgButtonChecked(IDC_CHK_DONOT_CHECK_MEAS_POINT);
	}
	else{
		///// Option Auto Focus /////
		uCheck = m_SrTransmittance.bDoNotCheckMeasPoint ? BST_CHECKED : BST_UNCHECKED;
		CheckDlgButton(IDC_CHK_DONOT_CHECK_MEAS_POINT, uCheck);
	}
}

// =========================================================================
//
BOOL CSrTransmittanceSettingDlg::CheckData()
{
	UpdateData(TRUE);

	return TRUE;
}

