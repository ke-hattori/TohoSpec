// AccessLevelSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "Misc.h"
#include "AccessLevelSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CAccessLevelSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CAccessLevelSettingDlg::CAccessLevelSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccessLevelSettingDlg::IDD, pParent), m_iAccountLevel(ACCOUNT_LEVEL_ADMINISTRATOR)
{
	//{{AFX_DATA_INIT(CAccessLevelSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CAccessLevelSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessLevelSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_MEASUREMENT, m_AccessLevel[m_iAccountLevel].bMeasurement);
	DDX_Check(pDX, IDC_CHECK_RECIPE, m_AccessLevel[m_iAccountLevel].bRecipe);
	DDX_Check(pDX, IDC_CHECK_PROGRAM, m_AccessLevel[m_iAccountLevel].bProgram);
	DDX_Check(pDX, IDC_CHECK_DATA, m_AccessLevel[m_iAccountLevel].bData);
	DDX_Check(pDX, IDC_CHECK_USER_SETTING, m_AccessLevel[m_iAccountLevel].bUserSetting);
	DDX_Check(pDX, IDC_CHECK_MAINTENANCE, m_AccessLevel[m_iAccountLevel].bMaintenance);
	DDX_CBIndex(pDX, IDC_ACCOUNT_LEVEL, m_iAccountLevel);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CAccessLevelSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessLevelSettingDlg)
	ON_CBN_SELCHANGE(IDC_ACCOUNT_LEVEL, OnSelchangeAccountLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CAccessLevelSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CAccessLevelSettingDlg::OnInitDialog()
{
	ConfigFile_GetNanoSpecIni(m_AccessLevel, CONFIG_FILE_ACCESS_LEVEL);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	// Kojika 20090528 Change
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//NanoAddComboBoxString(IDC_ACCOUNT_LEVEL, ACCOUNT_LEVEL_STR_INIT, ACCOUNT_LEVEL_MAX, m_iAccountLevel);
	if(l_SystemConfig.nLanguage == 0){
		NanoAddComboBoxString(IDC_ACCOUNT_LEVEL, ACCOUNT_LEVEL_STR_INIT_ENU, ACCOUNT_LEVEL_MAX, m_iAccountLevel);
	}
	else{
		NanoAddComboBoxString(IDC_ACCOUNT_LEVEL, ACCOUNT_LEVEL_STR_INIT_JPN, ACCOUNT_LEVEL_MAX, m_iAccountLevel);
	}
	// Kojika 20090528 Change End


	UpdateCheckAccessLevel(m_iAccountLevel);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CAccessLevelSettingDlg::OnSelchangeAccountLevel()
{
	UpdateData();

	UpdateCheckAccessLevel(m_iAccountLevel);

	UpdateData(FALSE);
}

// =========================================================================
//
void CAccessLevelSettingDlg::OnOK()
{
	///// Save /////
	if ( !UpdateData() )
		return;

	ConfigFile_SetNanoSpecIni(m_AccessLevel, CONFIG_FILE_ACCESS_LEVEL);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CAccessLevelSettingDlg::UpdateCheckAccessLevel(int iAccountLevel)
{
	BOOL bEnable;
	bEnable = ( iAccountLevel != ACCOUNT_LEVEL_ADMINISTRATOR );

	// 測定は、固定とする
	GetDlgItem(IDC_CHECK_MEASUREMENT)->EnableWindow(FALSE);

	// Administratorだけは変更できないようにする
	GetDlgItem(IDC_CHECK_RECIPE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_PROGRAM)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_DATA)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_USER_SETTING)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_MAINTENANCE)->EnableWindow(bEnable);
}
