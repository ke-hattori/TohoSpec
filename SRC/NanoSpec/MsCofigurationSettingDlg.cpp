// MsCofigurationSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MsCofigurationSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsCofigurationSettingDlg ダイアログ


CMsCofigurationSettingDlg::CMsCofigurationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsCofigurationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsCofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CMsCofigurationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsCofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CMsCofigurationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CMsCofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsCofigurationSettingDlg メッセージ ハンドラ

BOOL CMsCofigurationSettingDlg::OnInitDialog()
{
	memset(&m_MsConfig, 0, sizeof(m_MsConfig));
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);

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
void CMsCofigurationSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);

	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_MS_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CMsCofigurationSettingDlg::UpDate(BOOL bValid)
{
	CString	strBuffer;

	if ( bValid ) {
		///// StdZAxisRetractPos /////
		GetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);
		m_MsConfig.lStdZAxisRetractPos = strtol(strBuffer, NULL, 10) * 10;
	}
	else{
		///// StdZAxisRetractPos /////
		strBuffer.Format("%ld", m_MsConfig.lStdZAxisRetractPos / 10);
		SetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);
	}
}

// =========================================================================
//
BOOL CMsCofigurationSettingDlg::CheckData()
{

	UpdateData(TRUE);

	CString strBuffer;

	///// StdZAxisRetractPos /////
	GetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);
	if ( CheckMinMaxLong(strBuffer, MIN_STD_ZAXIS_RETRACT_POS, MAX_STD_ZAXIS_RETRACT_POS) != CHECK_DATA_OK ) {
		GetDlgItem(IDC_Z_AXIS_POSITION)->SetFocus();
		return FALSE;
	}

	return TRUE;
}
