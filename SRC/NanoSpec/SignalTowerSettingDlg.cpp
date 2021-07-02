// SignalTowerSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SignalTowerSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSignalTowerSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSignalTowerSettingDlg::CSignalTowerSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignalTowerSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSignalTowerSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSignalTowerSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSignalTowerSettingDlg)
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_INITIALIZE_RED, m_SignalTower.Initialize.iRed);
	DDX_CBIndex(pDX, IDC_INITIALIZE_YELLOW, m_SignalTower.Initialize.iYellow);
	DDX_CBIndex(pDX, IDC_INITIALIZE_GREEN, m_SignalTower.Initialize.iGreen);
	DDX_CBIndex(pDX, IDC_INITIALIZE_BUZZER, m_SignalTower.Initialize.iBuzzer);
	DDX_CBIndex(pDX, IDC_IDLE_RED, m_SignalTower.Idle.iRed);
	DDX_CBIndex(pDX, IDC_IDLE_YELLOW, m_SignalTower.Idle.iYellow);
	DDX_CBIndex(pDX, IDC_IDLE_GREEN, m_SignalTower.Idle.iGreen);
	DDX_CBIndex(pDX, IDC_IDLE_BUZZER, m_SignalTower.Idle.iBuzzer);
	DDX_CBIndex(pDX, IDC_RUN_RED, m_SignalTower.Run.iRed);
	DDX_CBIndex(pDX, IDC_RUN_YELLOW, m_SignalTower.Run.iYellow);
	DDX_CBIndex(pDX, IDC_RUN_GREEN, m_SignalTower.Run.iGreen);
	DDX_CBIndex(pDX, IDC_RUN_BUZZER, m_SignalTower.Run.iBuzzer);
	DDX_CBIndex(pDX, IDC_ALARM_RED, m_SignalTower.Alarm.iRed);
	DDX_CBIndex(pDX, IDC_ALARM_YELLOW, m_SignalTower.Alarm.iYellow);
	DDX_CBIndex(pDX, IDC_ALARM_GREEN, m_SignalTower.Alarm.iGreen);
	DDX_CBIndex(pDX, IDC_ALARM_BUZZER, m_SignalTower.Alarm.iBuzzer);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_INITIALIZE_TEST_BUTTON, m_InitializeTestButton);
	DDX_Control(pDX, IDC_IDLE_TEST_BUTTON, m_IdleTestButton);
	DDX_Control(pDX, IDC_RUN_TEST_BUTTON, m_RunTestButton);
	DDX_Control(pDX, IDC_ALARM_TEST_BUTTON, m_AlarmTestButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSignalTowerSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSignalTowerSettingDlg)
	ON_BN_CLICKED(IDC_INITIALIZE_TEST_BUTTON, OnInitializeTestButton)
	ON_BN_CLICKED(IDC_IDLE_TEST_BUTTON, OnIdleTestButton)
	ON_BN_CLICKED(IDC_RUN_TEST_BUTTON, OnRunTestButton)
	ON_BN_CLICKED(IDC_ALARM_TEST_BUTTON, OnAlarmTestButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSignalTowerSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSignalTowerSettingDlg::OnInitDialog()
{
	////// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SignalTower, CONFIG_FILE_SIGNAL_TOWER);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Initialize Test Button /////
	m_InitializeTestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_InitializeTestButton.DrawFlatFocus(TRUE);

	///// Idle Test Button /////
	m_IdleTestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_IdleTestButton.DrawFlatFocus(TRUE);

	///// Run Test Button /////
	m_RunTestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RunTestButton.DrawFlatFocus(TRUE);

	///// Alarm Test Button /////
	m_AlarmTestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AlarmTestButton.DrawFlatFocus(TRUE);

	///// Control Initialize /////
	InitCombo_SignalTowerLight(IDC_INITIALIZE_RED, m_SignalTower.Initialize.iRed);
	InitCombo_SignalTowerLight(IDC_INITIALIZE_YELLOW, m_SignalTower.Initialize.iYellow);
	InitCombo_SignalTowerLight(IDC_INITIALIZE_GREEN, m_SignalTower.Initialize.iGreen);
	InitCombo_SignalTowerBuzzer(IDC_INITIALIZE_BUZZER, m_SignalTower.Initialize.iBuzzer);
	InitCombo_SignalTowerLight(IDC_IDLE_RED, m_SignalTower.Idle.iRed);
	InitCombo_SignalTowerLight(IDC_IDLE_YELLOW, m_SignalTower.Idle.iYellow);
	InitCombo_SignalTowerLight(IDC_IDLE_GREEN, m_SignalTower.Idle.iGreen);
	InitCombo_SignalTowerBuzzer(IDC_IDLE_BUZZER, m_SignalTower.Idle.iBuzzer);
	InitCombo_SignalTowerLight(IDC_RUN_RED, m_SignalTower.Run.iRed);
	InitCombo_SignalTowerLight(IDC_RUN_YELLOW, m_SignalTower.Run.iYellow);
	InitCombo_SignalTowerLight(IDC_RUN_GREEN, m_SignalTower.Run.iGreen);
	InitCombo_SignalTowerBuzzer(IDC_RUN_BUZZER, m_SignalTower.Run.iBuzzer);
	InitCombo_SignalTowerLight(IDC_ALARM_RED, m_SignalTower.Alarm.iRed);
	InitCombo_SignalTowerLight(IDC_ALARM_YELLOW, m_SignalTower.Alarm.iYellow);
	InitCombo_SignalTowerLight(IDC_ALARM_GREEN, m_SignalTower.Alarm.iGreen);
	InitCombo_SignalTowerBuzzer(IDC_ALARM_BUZZER, m_SignalTower.Alarm.iBuzzer);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSignalTowerSettingDlg::OnOK()
{
	///// Save /////
	if ( !UpdateData() )
		return;

	ConfigFile_SetNanoSpecIni(&m_SignalTower, CONFIG_FILE_SIGNAL_TOWER);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CSignalTowerSettingDlg::OnInitializeTestButton()
{
	UpdateData();
	// signal tower の呼び出し
}

// =========================================================================
//
void CSignalTowerSettingDlg::OnIdleTestButton()
{
	UpdateData();
	// signal tower の呼び出し
}

// =========================================================================
//
void CSignalTowerSettingDlg::OnRunTestButton()
{
	UpdateData();
	// signal tower の呼び出し
}

// =========================================================================
//
void CSignalTowerSettingDlg::OnAlarmTestButton()
{
	UpdateData();
	// signal tower の呼び出し
}

// =========================================================================
//
void CSignalTowerSettingDlg::InitCombo_SignalTowerLight(UINT nID, int iSelect)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nID);
	pCombo->ResetContent();
	for ( int i = 0; i < SIGNAL_TOWER_LIGHT_MAX; i++ )
		pCombo->AddString(g_lpszSignalTowerLight[i]);
	pCombo->SetCurSel(iSelect);
}

// =========================================================================
//
void CSignalTowerSettingDlg::InitCombo_SignalTowerBuzzer(UINT nID, int iSelect)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nID);
	pCombo->ResetContent();
	for ( int i = 0; i < SIGNAL_TOWER_BUZZER_MAX; i++ )
		pCombo->AddString(g_lpszSignalTowerBuzzer[i]);
	pCombo->SetCurSel(iSelect);
}
