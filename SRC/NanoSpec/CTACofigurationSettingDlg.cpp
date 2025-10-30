// CTACofigurationSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "CTACofigurationSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCTACofigurationSettingDlg ダイアログ


CCTACofigurationSettingDlg::CCTACofigurationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCTACofigurationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCTACofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CCTACofigurationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCTACofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CCTACofigurationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CCTACofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCTACofigurationSettingDlg メッセージ ハンドラ

BOOL CCTACofigurationSettingDlg::OnInitDialog()
{
	memset(&m_CaConfig, 0, sizeof(m_CaConfig));
	ConfigFile_GetNanoSpecIni(&m_CaConfig, CONFIG_FILE_CA_CONFIG);

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

// =========================================================================
//
void CCTACofigurationSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);

	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_CaConfig, CONFIG_FILE_CA_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_CA_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CCTACofigurationSettingDlg::UpDate(BOOL bValid)
{
	CString	strBuffer;

	if ( bValid ) {
		///// CaStdZAxisRetractPos /////
		GetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);
		m_CaConfig.lCaStdZAxisRetractPos = strtol(strBuffer, NULL, 10) * 10;
	}
	else{
		///// CaStdZAxisRetractPos /////
		strBuffer.Format("%ld", m_CaConfig.lCaStdZAxisRetractPos / 10);
		SetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);
	}
}

// =========================================================================
//
BOOL CCTACofigurationSettingDlg::CheckData()
{

	UpdateData(TRUE);

	CString strBuffer;

	///// CaStdZAxisRetractPos /////
	GetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);
	if ( CheckMinMaxLong(strBuffer, MIN_ZAXIS_POSITION, MAX_ZAXIS_POSITION) != CHECK_DATA_OK ) {
		GetDlgItem(IDC_Z_AXIS_POSITION)->SetFocus();
		return FALSE;
	}

	return TRUE;
}
