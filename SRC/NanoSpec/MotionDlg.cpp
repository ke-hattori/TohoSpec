// MotionDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "MotionDlg.h"
#include "Alarm.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "..\\..\\INC\\StageInt.hxx"
#include "..\\..\\INC\\measys.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define JOG_WATCH_TIMER_ID				(1)
#define JOG_WATCH_TIMER_INTERVAL		(10)
#define JOG_MODE_LAMP_TIMER_ID			(2)
#define JOG_MODE_LAMP_TIMER_INTERVAL	(1000)

// ##########################################################################
// CMotionDlg ダイアログ
// ##########################################################################

// =========================================================================
//
CMotionDlg::CMotionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotionDlg)
	//}}AFX_DATA_INIT
// 2009.11.09 bagus MS 修正 --{--
	m_nSelLens = 0;
	m_bMSModeLampFilcker = FALSE;
// 2009.11.09 bagus MS 修正 --}--
}

// =========================================================================
//
void CMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotionDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_INIT_STAGE_BUTTON, m_InitStageButton);
	DDX_Control(pDX, IDC_INIT_AUTO_FOCUS_BUTTON, m_InitAutoFocusButton);
	DDX_Control(pDX, IDC_INIT_AUTO_TURRET_BUTTON, m_InitAutoTurretButton);
	DDX_Control(pDX, IDC_FILTER_OPEN_BUTTON, m_FilterOpenButton);
	DDX_Control(pDX, IDC_FILTER_CLOSE_BUTTON, m_FilterCloseButton);
	DDX_Control(pDX, IDC_FILTER1_BUTTON, m_Filter1Button);
	DDX_Control(pDX, IDC_FILTER2_BUTTON, m_Filter2Button);
	DDX_Control(pDX, IDC_FILTER3_BUTTON, m_Filter3Button);
	DDX_Control(pDX, IDC_VACUUM_ON_BUTTON, m_VacuumOnButton);
	DDX_Control(pDX, IDC_VACUUM_OFF_BUTTON, m_VacuumOffButton);
	DDX_Control(pDX, IDC_COMPEASE_SHUTTER_OPEN_BUTTON, m_CompEASEShutterOpenButton);
	DDX_Control(pDX, IDC_COMPEASE_SHUTTER_CLOSE_BUTTON, m_CompEASEShutterCloseButton);
	DDX_Control(pDX, IDC_MAINTENANCE_POSITION1_BUTTON, m_MaintenancePosition1Button);
	DDX_Control(pDX, IDC_MAINTENANCE_POSITION2_BUTTON, m_MaintenancePosition2Button);
	DDX_Control(pDX, IDC_MAINTENANCE_POSITION3_BUTTON, m_MaintenancePosition3Button);
	DDX_Control(pDX, IDC_MAINTENANCE_POSITION4_BUTTON, m_MaintenancePosition4Button);
	DDX_Control(pDX, IDC_SAMPLE_LOADING_SHUTTER_OPEN_BUTTON, m_SampleLoadingShutterOpenButton);
	DDX_Control(pDX, IDC_SAMPLE_LOADING_SHUTTER_CLOSE_BUTTON, m_SampleLoadingShutterCloseButton);
	DDX_Control(pDX, IDC_SAMPLE_LIFTER_UP_BUTTON, m_SampleLifterUpButton);
	DDX_Control(pDX, IDC_SAMPLE_LIFTER_ALIGNMENT_BUTTON, m_SampleLifterAlignmentButton);
	DDX_Control(pDX, IDC_SAMPLE_LIFTER_DOWN_BUTTON, m_SampleLifterDownButton);
	DDX_Control(pDX, IDC_SAMPLE_LOAD_SEQ_LOAD_BUTTON, m_SampleLoadSequenceLoadButton);
	DDX_Control(pDX, IDC_SAMPLE_LOAD_SEQ_UNLOAD_BUTTON, m_SampleLoadSequenceUnloadButton);
	DDX_Control(pDX, IDC_SAMPLE_CLAMP_SEQ_OPEN_BUTTON, m_SampleClampSequenceOpenButton);
	DDX_Control(pDX, IDC_SAMPLE_CLAMP_SEQ_CLOSE_BUTTON, m_SampleClampSequenceCloseButton);
	DDX_Control(pDX, IDC_LOAD_SEQUENCE_CANCEL_BUTTON, m_LoadSequenceCancelButton);
	DDX_Control(pDX, IDC_CLAMP_SEQUENCE_CANCEL_BUTTON, m_ClampSequenceCancelButton);
	DDX_Control(pDX, IDC_WARNING_MESSAGE, m_ctlWarningMessage);
	DDX_Control(pDX, IDC_CLOSE, m_CloseButton);
// 2009.10.29 bagus MS 追加修正 --{--
	DDX_Control(pDX, IDC_MICRO_SCOPE_MOVE, m_MSMoveToPosButton);
	DDX_Control(pDX, IDC_MICRO_SCOPE_STD_MODE, m_MSSTDModeButton);
	DDX_Control(pDX, IDC_MICRO_SCOPE_JOG_INCING, m_MSJogInchingButton);
// 2009.10.29 bagus MS 追加修正 --}--
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CMotionDlg, CDialog)
	//{{AFX_MSG_MAP(CMotionDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_INIT_STAGE_BUTTON, OnInitStageButton)
	ON_BN_CLICKED(IDC_INIT_AUTO_FOCUS_BUTTON, OnInitAutoFocusButton)
	ON_BN_CLICKED(IDC_INIT_AUTO_TURRET_BUTTON, OnInitAutoTurretButton)
	ON_BN_CLICKED(IDC_FILTER_OPEN_BUTTON, OnFilterOpenButton)
	ON_BN_CLICKED(IDC_FILTER_CLOSE_BUTTON, OnFilterCloseButton)
	ON_BN_CLICKED(IDC_FILTER1_BUTTON, OnFilter1Button)
	ON_BN_CLICKED(IDC_FILTER2_BUTTON, OnFilter2Button)
	ON_BN_CLICKED(IDC_FILTER3_BUTTON, OnFilter3Button)
	ON_BN_CLICKED(IDC_VACUUM_ON_BUTTON, OnVacuumOnButton)
	ON_BN_CLICKED(IDC_VACUUM_OFF_BUTTON, OnVacuumOffButton)
	ON_BN_CLICKED(IDC_COMPEASE_SHUTTER_OPEN_BUTTON, OnCompEASEShutterOpenButton)
	ON_BN_CLICKED(IDC_COMPEASE_SHUTTER_CLOSE_BUTTON, OnCompEASEShutterCloseButton)
	ON_BN_CLICKED(IDC_MAINTENANCE_POSITION1_BUTTON, OnMaintenancePosition1Button)
	ON_BN_CLICKED(IDC_MAINTENANCE_POSITION2_BUTTON, OnMaintenancePosition2Button)
	ON_BN_CLICKED(IDC_MAINTENANCE_POSITION3_BUTTON, OnMaintenancePosition3Button)
	ON_BN_CLICKED(IDC_MAINTENANCE_POSITION4_BUTTON, OnMaintenancePosition4Button)
	ON_BN_CLICKED(IDC_SAMPLE_LIFTER_UP_BUTTON, OnSampleLifterUpButton)
	ON_BN_CLICKED(IDC_SAMPLE_LIFTER_ALIGNMENT_BUTTON, OnSampleLifterAlignmentButton)
	ON_BN_CLICKED(IDC_SAMPLE_LIFTER_DOWN_BUTTON, OnSampleLifterDownButton)
	ON_BN_CLICKED(IDC_SAMPLE_LOAD_SEQ_LOAD_BUTTON, OnSampleLoadSequenceLoadButton)
	ON_BN_CLICKED(IDC_SAMPLE_LOAD_SEQ_UNLOAD_BUTTON, OnSampleLoadSequenceUnloadButton)
	ON_BN_CLICKED(IDC_SAMPLE_CLAMP_SEQ_OPEN_BUTTON, OnSampleClampSequenceOpenButton)
	ON_BN_CLICKED(IDC_SAMPLE_CLAMP_SEQ_CLOSE_BUTTON, OnSampleClampSequenceCloseButton)
	ON_BN_CLICKED(IDC_LOAD_SEQUENCE_CANCEL_BUTTON, OnLoadSequenceCancelButton)
	ON_BN_CLICKED(IDC_CLAMP_SEQUENCE_CANCEL_BUTTON, OnClampSequenceCancelButton)
	ON_BN_CLICKED(IDC_CLOSE, OnCloseButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SAMPLE_LOADING_SHUTTER_OPEN_BUTTON, OnSampleLoadingShutterOpenButton)
	ON_BN_CLICKED(IDC_SAMPLE_LOADING_SHUTTER_CLOSE_BUTTON, OnSampleLoadingShutterCloseButton)
	ON_BN_CLICKED(IDC_MICRO_SCOPE_MOVE, OnMicroScopeMove)
	ON_BN_CLICKED(IDC_MICRO_SCOPE_STD_MODE, OnMicroScopeStdMode)
	ON_BN_CLICKED(IDC_MICRO_SCOPE_JOG_INCING, OnMicroScopeJogIncing)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEX_RESPONSE, OnNextraResponse)

END_MESSAGE_MAP()

// =========================================================================
// CMotionDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CMotionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

/* added 2009.06.16 hmenjo Loading Shutter 使用有無追加 ---------- { ---------- */
	///// Load System Config File /////
	/* システムコンフィグ設定を読出し */
	//2009.11.10 bagus MS --{--
	//メンバ変数化
	//SYSTEM_CONFIG l_SystemConfig;
	//ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//2009.11.10 bagus MS --}--

/* added 2009.06.16 hmenjo Loading Shutter 使用有無追加 ---------- } ---------- */
	////// Load File /////
	ConfigFile_GetNanoSpecIni(m_TempMaintenancePosition, CONFIG_FILE_MAINTENANCE_POSITION);
// 2009.11.09 bagus MS 修正 --{--
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
// 2009.11.09 bagus MS 修正 --}--

	///// Initialize Stage Button /////
	m_InitStageButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_InitStageButton.DrawFlatFocus(TRUE);

	///// Initialize Auto Focus Button /////
	m_InitAutoFocusButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_InitAutoFocusButton.DrawFlatFocus(TRUE);

	///// Initialize Auto Turret Button /////
	m_InitAutoTurretButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_InitAutoTurretButton.DrawFlatFocus(TRUE);

	///// Filter Open Button /////
	m_FilterOpenButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_FilterOpenButton.DrawFlatFocus(TRUE);

	///// Filter Close Button /////
	m_FilterCloseButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_FilterCloseButton.DrawFlatFocus(TRUE);

	///// Filter1 Button /////
	m_Filter1Button.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Filter1Button.DrawFlatFocus(TRUE);

	///// Filter2 Button /////
	m_Filter2Button.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Filter2Button.DrawFlatFocus(TRUE);

	///// Filter3 Button /////
	m_Filter3Button.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Filter3Button.DrawFlatFocus(TRUE);

	///// Vacuum On Button /////
	m_VacuumOnButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_VacuumOnButton.DrawFlatFocus(TRUE);

	///// Vacuum Off Button /////
	m_VacuumOffButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_VacuumOffButton.DrawFlatFocus(TRUE);

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	///// CompleteEASE Shutter Open Button /////
	m_CompEASEShutterOpenButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CompEASEShutterOpenButton.DrawFlatFocus(TRUE);

	///// CompleteEASE Shutter Close Button /////
	m_CompEASEShutterCloseButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CompEASEShutterCloseButton.DrawFlatFocus(TRUE);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

	///// Maintenance Position1 Button /////
	if( strlen(m_TempMaintenancePosition[0].szComment) > 0 ){
		GetDlgItem(IDC_MAINTENANCE_POSITION1_BUTTON)->SetWindowText(m_TempMaintenancePosition[0].szComment);
// 2009.12.08 bagus --{--
	} else {
		if (m_SystemConfig.nLanguage == 0) {
			GetDlgItem(IDC_MAINTENANCE_POSITION1_BUTTON)->SetWindowText(g_lpszMotionPos_ENU[0]);
		} else {
			GetDlgItem(IDC_MAINTENANCE_POSITION1_BUTTON)->SetWindowText(g_lpszMotionPos_JPN[0]);
		}
// 2009.12.08 bagus --}--
	}
	m_MaintenancePosition1Button.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MaintenancePosition1Button.DrawFlatFocus(TRUE);

	///// Maintenance Position2 Button /////
	if( strlen(m_TempMaintenancePosition[1].szComment) > 0 ){
		GetDlgItem(IDC_MAINTENANCE_POSITION2_BUTTON)->SetWindowText(m_TempMaintenancePosition[1].szComment);
// 2009.12.08 bagus --{--
	} else {
		if (m_SystemConfig.nLanguage == 0) {
			GetDlgItem(IDC_MAINTENANCE_POSITION2_BUTTON)->SetWindowText(g_lpszMotionPos_ENU[1]);
		} else {
			GetDlgItem(IDC_MAINTENANCE_POSITION2_BUTTON)->SetWindowText(g_lpszMotionPos_JPN[1]);
		}
// 2009.12.08 bagus --}--
	}
	m_MaintenancePosition2Button.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MaintenancePosition2Button.DrawFlatFocus(TRUE);

	///// Maintenance Position3 Button /////
	if( strlen(m_TempMaintenancePosition[2].szComment) > 0 ){
		GetDlgItem(IDC_MAINTENANCE_POSITION3_BUTTON)->SetWindowText(m_TempMaintenancePosition[2].szComment);
// 2009.12.08 bagus --{--
	} else {
		if (m_SystemConfig.nLanguage == 0) {
			GetDlgItem(IDC_MAINTENANCE_POSITION3_BUTTON)->SetWindowText(g_lpszMotionPos_ENU[2]);
		} else {
			GetDlgItem(IDC_MAINTENANCE_POSITION3_BUTTON)->SetWindowText(g_lpszMotionPos_JPN[2]);
		}
// 2009.12.08 bagus --}--
	}
	m_MaintenancePosition3Button.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MaintenancePosition3Button.DrawFlatFocus(TRUE);

	///// Maintenance Position4 Button /////
	if( strlen(m_TempMaintenancePosition[3].szComment) > 0 ){
		GetDlgItem(IDC_MAINTENANCE_POSITION4_BUTTON)->SetWindowText(m_TempMaintenancePosition[3].szComment);
// 2009.12.08 bagus --{--
	} else {
		if (m_SystemConfig.nLanguage == 0) {
			GetDlgItem(IDC_MAINTENANCE_POSITION4_BUTTON)->SetWindowText(g_lpszMotionPos_ENU[3]);
		} else {
			GetDlgItem(IDC_MAINTENANCE_POSITION4_BUTTON)->SetWindowText(g_lpszMotionPos_JPN[3]);
		}
// 2009.12.08 bagus --}--
	}
	m_MaintenancePosition4Button.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MaintenancePosition4Button.DrawFlatFocus(TRUE);

	///// Sample Loading Shutter Open Button /////
	m_SampleLoadingShutterOpenButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleLoadingShutterOpenButton.DrawFlatFocus(TRUE);

	///// Sample Loading Shutter Close Button /////
	m_SampleLoadingShutterCloseButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleLoadingShutterCloseButton.DrawFlatFocus(TRUE);
/* added 2009.06.16 hmenjo Loading Shutter 使用有無追加 ---------- { ---------- */
	/* シャッタ系の表示する/しない*/
	//2009.11.10 bagus MS --{--
	//システムコンフィグはメンバ変数化された
	//if (0 == l_SystemConfig.bUseLoadingShutter) {
	if (0 == m_SystemConfig.bUseLoadingShutter) {
	//2009.11.10 bagus MS --}--
		this->GetDlgItem(IDC_STATIC_SAMPLE_LOAD_SHUTTER)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_SAMPLE_LOADING_SHUTTER_OPEN_BUTTON)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_SAMPLE_LOADING_SHUTTER_CLOSE_BUTTON)->ShowWindow(SW_HIDE);
	}
/* added 2009.06.16 hmenjo Loading Shutter 使用有無追加 ---------- } ---------- */

	///// Sample Lifter Up Button /////
	m_SampleLifterUpButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleLifterUpButton.DrawFlatFocus(TRUE);

	///// Sample Lifter Alignment Button /////
	m_SampleLifterAlignmentButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleLifterAlignmentButton.DrawFlatFocus(TRUE);

	///// Sample Lifter Down Button /////
	m_SampleLifterDownButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleLifterDownButton.DrawFlatFocus(TRUE);

	///// Sample Load Sequence Load Button /////
	m_SampleLoadSequenceLoadButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleLoadSequenceLoadButton.DrawFlatFocus(TRUE);

	///// Sample Load Sequence Unload Button /////
	m_SampleLoadSequenceUnloadButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleLoadSequenceUnloadButton.DrawFlatFocus(TRUE);

	///// Load Sequence Cancel Button /////
	m_LoadSequenceCancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_LoadSequenceCancelButton.DrawFlatFocus(TRUE);

	///// Sample Clamp Sequence Open Button /////
	m_SampleClampSequenceOpenButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleClampSequenceOpenButton.DrawFlatFocus(TRUE);

	///// Sample Clamp Sequence Close Button /////
	m_SampleClampSequenceCloseButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleClampSequenceCloseButton.DrawFlatFocus(TRUE);

	///// Clamp Sequence Cancel Button /////
	m_ClampSequenceCancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ClampSequenceCancelButton.DrawFlatFocus(TRUE);

	///// Close Button /////
	m_CloseButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CloseButton.DrawFlatFocus(TRUE);

	///// Warning Message /////
	m_ctlWarningMessage.SetWindowText("");

	m_bSampleLoadingShutterOpen = FALSE;
	m_bSampleLoadingShutterClose = FALSE;

// 2009.10.29 bagus MS 追加修正 --{--
	///// Move To Pos Button /////
	m_MSMoveToPosButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MSMoveToPosButton.DrawFlatFocus(TRUE);

	///// STD Mode Button /////
	m_MSSTDModeButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MSSTDModeButton.DrawFlatFocus(TRUE);

	///// Jog / Inching Button /////
	m_MSJogInchingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MSJogInchingButton.DrawFlatFocus(TRUE);

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	if (m_SystemConfig.HeadType.bSE && m_SystemConfig.HeadType.bCompEASE) {
		GetDlgItem(IDC_STATIC_COMPEASE_SHUTTER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMPEASE_SHUTTER_OPEN_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMPEASE_SHUTTER_CLOSE_BUTTON)->ShowWindow(SW_SHOW);
	}
	else {
		GetDlgItem(IDC_STATIC_COMPEASE_SHUTTER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMPEASE_SHUTTER_OPEN_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMPEASE_SHUTTER_CLOSE_BUTTON)->ShowWindow(SW_HIDE);
	}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

	if (m_SystemConfig.HeadType.bMS) {
		GetDlgItem(IDC_MICRO_SCOPE_Z_AXIS_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MICRO_SCOPE_LENS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MICRO_SCOPE_MOVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MICRO_SCOPE_STD_MODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MICRO_SCOPE_JOG_INCING)->ShowWindow(SW_SHOW);
		InitCombo_Lens();
		//2009.11.10 bagus MS --{--
		//Jogボタン監視タイマ
		//起動時は通常モードに変更しておく
		nexifRC_SelectMode(this->m_hWnd,RCOPMODE_NORMAL);
		SetTimer(JOG_MODE_LAMP_TIMER_ID,JOG_MODE_LAMP_TIMER_INTERVAL,NULL);
		SetTimer(JOG_WATCH_TIMER_ID,JOG_WATCH_TIMER_INTERVAL,NULL);
		//2009.11.10 bagus MS --}--
	} else {
		GetDlgItem(IDC_MICRO_SCOPE_Z_AXIS_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MICRO_SCOPE_LENS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MICRO_SCOPE_MOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MICRO_SCOPE_STD_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MICRO_SCOPE_JOG_INCING)->ShowWindow(SW_HIDE);
	}
// 2009.10.29 bagus MS 追加修正 --}--


	// Kawa 2009.05.30 ----->
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	// Kawa 2009.05.30 <-----

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CMotionDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

//	CDialog::OnCancel();
}

// =========================================================================
//
void CMotionDlg::OnClose()
{

//	CDialog::OnClose();
}

// =========================================================================
//
void CMotionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
BOOL result;

	if( m_SampleLoadingShutterOpenButton.m_bPush ){

		// ▼インターロック条件▼
		if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
			if( nexioIsMaintenanceSwitch() != OFF ){
				// Kojika 20090602 Change
				//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
				SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
				return;
			}
		}
		else{												// 通常動作
		}
		// 各I/Oチェック
		if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
			SetWarningMessageText( IDS_CHECK_EMO_SW );
			return;
		}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//		if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
		if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
			SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
			return;
		}
		if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
			SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
			return;
		}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//		if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
		if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
			SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
			return;
		}
		if(	m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER) ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("動作条件が不足しています");
			SetWarningMessageText( IDS_WORKING_CONDITION_INSUFFICIENT );
			return;
		}
		// ▲インターロック条件▲

		m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, TRUE);

		// Sample Loading Shutter OPEN
		nexioShutterOpenEx(ON);

		// Sample Loading Shutter OPEN Button was Down
		m_bSampleLoadingShutterOpen = TRUE;

		SetOperationLog("Sample Loading Shutter - OPEN Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");

	}
	if( m_SampleLoadingShutterCloseButton.m_bPush ){

		// ▼インターロック条件▼
		if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
			if( nexioIsMaintenanceSwitch() != OFF ){
				// Kojika 20090602 Change
				//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
				SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
				return;
			}
		}
		else{												// 通常動作
		}
		// 各I/Oチェック
		if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
			SetWarningMessageText( IDS_CHECK_EMO_SW );
			return;
		}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//		if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
		if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
			SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
			return;
		}
		if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
			SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
			return;
		}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//		if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
		if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
			SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
			return;
		}
		if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
			SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
			return;
		}
		if(	m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER) ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("シャッターが動作しています");
			SetWarningMessageText( IDS_SHUTTER_WORKING );
			return;
		}
// 2009.11.09 bagus MS 修正 --{--
		if( m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) ){
			SetWarningMessageText( IDS_MICROSCOPE_WORKING );
			return;
		}
// 2009.11.09 bagus MS 修正 --}--
		// ▲インターロック条件▲

		m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, TRUE);

		// Sample Loading Shutter CLOSE
		nexioShutterCloseEx(ON);

		// Sample Loading Shutter CLOSE Button was Down
		m_bSampleLoadingShutterClose = TRUE;

		SetOperationLog("Sample Loading Shutter - CLOSE Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");

	}
}

// =========================================================================
//
void CMotionDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_bSampleLoadingShutterOpen ){
		// Sample Loading Shutter OPEN Button was Up
		m_bSampleLoadingShutterOpen = FALSE;
		nexioShutterOpenEx(OFF);
		m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, FALSE);
	}
	if( m_bSampleLoadingShutterClose ){
		// Sample Loading Shutter CLOSE Button was Up
		m_bSampleLoadingShutterClose = FALSE;
		nexioShutterCloseEx(OFF);
		m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, FALSE);
	}
}

// =========================================================================
//
void CMotionDlg::OnInitStageButton()
{
	BOOL bRet;

	// ステージ初期化
	if (0 != nexioIsMaintenanceSwitch()) {
		// メンテナンスモードでなかった
		// 速度制限しない
		StageDisableSpeedLimit();
	} else {
		// メンテナンスモードだった
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- { ----------
//		// 速度制限する
//		StageEnableSpeedLimit();
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ----------
		if (TRUE == g_bIL_SpeedDown) {
			// 速度制限する
			StageEnableSpeedLimit();
		}
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- } ----------
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);

	if( (bRet=StageInitializeStage())==TRUE ){
		SetOperationLog("STAGE Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージのイニシャライズが失敗しました");
		SetWarningMessageText( IDS_FAILED_INITIAL_STAGE );
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnInitAutoFocusButton()
{
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);

	// オートフォーカス初期化
	if( StageInitializeAutoFocus()==TRUE ){
		SetOperationLog("AUTO FOCUS Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Z軸のイニシャライズが失敗しました");
		SetWarningMessageText( IDS_FAILED_INITIAL_Z_AXIS );
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnInitAutoTurretButton()
{
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);

	// ターレット初期化
	if( StageInitializeTurret()==TRUE ){
		SetOperationLog("AUTO TURRET Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットのイニシャライズが失敗しました");
		SetWarningMessageText( IDS_FAILED_INITIAL_TURRET );
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnFilterOpenButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	// ▲インターロック条件▲

	// Optical Filter OPEN
	if( MEAS_SrHead_ChangeCcdShutter(OPT_FILTER_OPEN)==TRUE ){
		SetOperationLog("Optical Filter - OPEN Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("光学フィルタの動作に失敗しました");
		SetWarningMessageText( IDS_FAILED_OPTICAL_FILTER );
	}
}

// =========================================================================
//
void CMotionDlg::OnFilterCloseButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	// ▲インターロック条件▲

	// Optical Filter CLOSE
	if( MEAS_SrHead_ChangeCcdShutter(OPT_FILTER_DARK)==TRUE ){
		SetOperationLog("Optical Filter - CLOSE Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("光学フィルタの動作に失敗しました");
		SetWarningMessageText( IDS_FAILED_OPTICAL_FILTER );
	}
}

// =========================================================================
//
void CMotionDlg::OnFilter1Button()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	// ▲インターロック条件▲

	// Optical Filter FILTER 1
	if( MEAS_SrHead_ChangeCcdShutter(OPT_FILTER_POS1)==TRUE ){
		SetOperationLog("Optical Filter - FILTER 1 Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("光学フィルタの動作に失敗しました");
		SetWarningMessageText( IDS_FAILED_OPTICAL_FILTER );
	}
}

// =========================================================================
//
void CMotionDlg::OnFilter2Button()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	// ▲インターロック条件▲

	// Optical Filter FILTER 2
	if( MEAS_SrHead_ChangeCcdShutter(OPT_FILTER_POS2)==TRUE ){
		SetOperationLog("Optical Filter - FILTER 2 Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("光学フィルタの動作に失敗しました");
		SetWarningMessageText( IDS_FAILED_OPTICAL_FILTER );
	}
}

// =========================================================================
//
void CMotionDlg::OnFilter3Button()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	// ▲インターロック条件▲

	// Optical Filter FILTER 3
	if( MEAS_SrHead_ChangeCcdShutter(OPT_FILTER_POS3)==TRUE ){
		SetOperationLog("Optical Filter - FILTER 3 Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("光学フィルタの動作に失敗しました");
		SetWarningMessageText( IDS_FAILED_OPTICAL_FILTER );
	}
}

// =========================================================================
//
void CMotionDlg::OnVacuumOnButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= ON ){		// ピン下端						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	// ▲インターロック条件▲

	// Vacuum ON
	if( nexifVacuumOn(this->m_hWnd)==TRUE ){
		SetOperationLog("Vacuum ON Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("バキュームONに失敗しました");
		SetWarningMessageText( IDS_FAILED_VACCUM_ON );
	}
}

// =========================================================================
//
void CMotionDlg::OnVacuumOffButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	// ▲インターロック条件▲

	// Vacuum OFF
	if( nexifVacuumOff(this->m_hWnd)==TRUE ){
		SetOperationLog("Vacuum OFF Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("バキュームOFFに失敗しました");
		SetWarningMessageText( IDS_FAILED_VACCUM_OFF );
	}
}

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// =========================================================================
//
void CMotionDlg::OnCompEASEShutterOpenButton()
{
	BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	// ▲インターロック条件▲

	// CompleteEASE Shutter Open
	if( MEAS_CompEASEHead_OpenLampShutter() == TRUE ){
		SetOperationLog("CompleteEASE Shutter - OPEN Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		//m_ctlWarningMessage.SetWindowText("EASE光源シャッター開の動作に失敗しました");
		SetWarningMessageText( IDS_FAILED_COMPEASE_SHUTTER_OPEN );
	}
}

// =========================================================================
//
void CMotionDlg::OnCompEASEShutterCloseButton()
{
	BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	// ▲インターロック条件▲

	// CompleteEASE Shutter Close
	if( MEAS_CompEASEHead_CloseLampShutter() == TRUE ){
		SetOperationLog("CompleteEASE Shutter - CLOSE Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		//m_ctlWarningMessage.SetWindowText("EASE光源シャッター閉の動作に失敗しました");
		SetWarningMessageText( IDS_FAILED_COMPEASE_SHUTTER_CLOSE );
	}
}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

// =========================================================================
//
void CMotionDlg::OnMaintenancePosition1Button()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
		if( nexioIsMaintenanceSwitch() != ON ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsMovo2Alarm())	!= OFF){		// MOVOアラーム					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("MOVOアラームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_MOVO_ALARM );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= ON ){		// ピン下端						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	STAGE_COORD Position;							// current stage position

	Position.lX = m_TempMaintenancePosition[0].Loc.lX;
	Position.lY = m_TempMaintenancePosition[0].Loc.lY;

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);

	if( NS_StageMoveAbsolute(&Position) == TRUE ){
		SetOperationLog("Maintenance Position Move - POSITION 1 Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ポジション１の位置移動に失敗しました");
		SetWarningMessageText( IDS_FAILED_POSITION_MOVEMENT1 );
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnMaintenancePosition2Button()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
		if( nexioIsMaintenanceSwitch() != ON ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsMovo2Alarm())	!= OFF){		// MOVOアラーム					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("MOVOアラームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_MOVO_ALARM );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= ON ){		// ピン下端						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	STAGE_COORD Position;							// current stage position

	Position.lX = m_TempMaintenancePosition[1].Loc.lX;
	Position.lY = m_TempMaintenancePosition[1].Loc.lY;

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);

	if( NS_StageMoveAbsolute(&Position) == TRUE ){
		SetOperationLog("Maintenance Position Move - POSITION 2 Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ポジション２の位置移動に失敗しました");
		SetWarningMessageText( IDS_FAILED_POSITION_MOVEMENT2 );
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnMaintenancePosition3Button()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
		if( nexioIsMaintenanceSwitch() != ON ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsMovo2Alarm())	!= OFF){		// MOVOアラーム					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("MOVOアラームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_MOVO_ALARM );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= ON ){		// ピン下端						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	STAGE_COORD Position;							// current stage position

	Position.lX = m_TempMaintenancePosition[2].Loc.lX;
	Position.lY = m_TempMaintenancePosition[2].Loc.lY;

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);

	if( NS_StageMoveAbsolute(&Position) == TRUE ){
		SetOperationLog("Maintenance Position Move - POSITION 3 Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ポジション３の位置移動に失敗しました");
		SetWarningMessageText( IDS_FAILED_POSITION_MOVEMENT3 );
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnMaintenancePosition4Button()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
		if( nexioIsMaintenanceSwitch() != ON ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsMovo2Alarm())	!= OFF){		// MOVOアラーム					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("MOVOアラームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_MOVO_ALARM );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= ON ){		// ピン下端						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	STAGE_COORD Position;							// current stage position

	Position.lX = m_TempMaintenancePosition[3].Loc.lX;
	Position.lY = m_TempMaintenancePosition[3].Loc.lY;

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);

	if( NS_StageMoveAbsolute(&Position) == TRUE ){
		SetOperationLog("Maintenance Position Move - POSITION 4 Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ポジション４の位置移動に失敗しました");
		SetWarningMessageText( IDS_FAILED_POSITION_MOVEMENT4 );
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnSampleLoadingShutterOpenButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
		SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
		return;
	}
	if(	m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("シャッターが動作しています");
		SetWarningMessageText( IDS_SHUTTER_WORKING );
		return;
	}
// 2009.11.09 bagus MS 修正 --{--
	if( m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) ){
		SetWarningMessageText( IDS_MICROSCOPE_WORKING );
		return;
	}
// 2009.11.09 bagus MS 修正 --}--
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, TRUE);

	// Sample Loading Shutter OPEN
	if( nexifOpenShutter(this->m_hWnd)==TRUE ){

		// 操作ログの取得
		m_pDoc->OperationLogging("MOTION - Sample Loading Shutter OPEN Button was pushed.");
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnSampleLoadingShutterCloseButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
// 	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
		SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if(	m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("シャッターが動作しています");
		SetWarningMessageText( IDS_SHUTTER_WORKING );
		return;
	}
// 2009.11.09 bagus MS 修正 --{--
	if( m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) ){
		SetWarningMessageText( IDS_MICROSCOPE_WORKING );
		return;
	}
// 2009.11.09 bagus MS 修正 --}--
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, TRUE);

	// Sample Loading Shutter CLOSE
	if( nexifCloseShutter(this->m_hWnd)==TRUE ){

		// 操作ログの取得
		m_pDoc->OperationLogging("MOTION - Sample Loading Shutter CLOSE Button was pushed.");
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnSampleLifterUpButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( (result=nexioIsGlassExist())	== ON ){		// ガラス在荷有り				＝Ｈ
// matsuhisa 20090522 fix >>>
//		if( (result=nexioIsVacuumStatus())	!= ON ){	// バキューム					＝Ｈ
		if( (result=nexioIsVacuumOff())	!= ON ){	// バキューム					＝Ｈ
// matsuhisa 20090522 fix <<<
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("バキュームを確認して下さい");
			SetWarningMessageText( IDS_CHECK_VACCUM );
			return;
		}
		if( (result=nexioIsPinDownPos())	!= OFF){	// ピン下端						＝Ｌ
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
			SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
			return;
		}
	}
// matsuhisa 20090522 fix >>>
//	else{												// ガラス在荷無し				＝Ｌ
//		if( (result=nexioIsPinDownPos())	!= ON ){	// ピン下端						＝Ｈ
//			m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
//			return;
//		}
//	}
// matsuhisa 20090522 fix <<<
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, TRUE);

	// Sample Lifter UPPER
	if( nexifMoveToUpper(this->m_hWnd)==TRUE ){
		SetOperationLog("Sample Lifter - UPPER Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("サンプルリフターの上端に失敗しました");
		SetWarningMessageText( IDS_FAILED_RISE_SAMPLE_LIFTER );
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnSampleLifterAlignmentButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
// matsuhisa 20090522 fix >>>
//	if( (result=nexioIsVacuumStatus())	!= ON ){		// バキューム					＝Ｈ
	if( (result=nexioIsVacuumOff())	!= ON ){		// バキューム					＝Ｈ
// matsuhisa 20090522 fix <<<
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("バキュームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_VACCUM );
		return;
	}
	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
		return;
	}
// matsuhisa 20090522 fix >>>
//	if( (result=nexioIsPinDownPos())	!= OFF){		// ピン下端						＝Ｌ
//		m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
//		return;
//	}
// matsuhisa 20090522 fix <<<
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, TRUE);

	// Sample Lifter ALIGNMENT
	if( nexifMoveToAlignment(this->m_hWnd)==TRUE ){
		SetOperationLog("Sample Lifter - ALIGNMENT Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("アライメント位置移動に失敗しました");
		SetWarningMessageText( IDS_FAILED_ALIGNMENT_POSITION_MOVE );
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnSampleLifterDownButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
// 2009.08.24 K.Matsuo delete -->
// ロードポジションへ移動しようとすると、PIN下端でないと言われた
// PINを下げようとするとロードポジションでないと言われて、デットロック状態になったため、削除
//	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
//		// Kojika 20090602 Change
//		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
//		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
//		return;
//	}
// 2009.08.24 K.Matsuo delete <--
	if( (result=nexioIsPinDownPos())	!= OFF){		// ピン下端						＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, TRUE);

	// Sample Lifter LOWER
	if( nexifMoveToLower(this->m_hWnd)==TRUE ){
		SetOperationLog("Sample Lifter - LOWER Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("サンプルリフターの下降に失敗しました");
		SetWarningMessageText( IDS_FAILED_DESCENT_SAMPLE_LIFTER );
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnSampleLoadSequenceLoadButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
		SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= OFF){		// ピン下端						＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( (result=nexioIsPinUpperPos())!=ON ){			// ピン上端						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター上端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_UPPER );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, TRUE);
	m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, TRUE);

	if( nexifLoad(this->m_hWnd)==TRUE ){
		SetOperationLog("Sequence - Load/Unload - LOAD Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("シーケンスのロードに失敗しました");
		SetWarningMessageText( IDS_FAILED_LOAD_SEQUENCE );
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, FALSE);
//	m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnSampleLoadSequenceUnloadButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
		SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= ON ){		// ピン下端						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, TRUE);
	m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, TRUE);

	if( nexifUnload(this->m_hWnd)==TRUE ){
		SetOperationLog("Sequence - Load/Unload - UNLOAD Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("シーケンスのアンロードに失敗しました");
		SetWarningMessageText( IDS_FAILED_UNLOAD_SEQUENCE );
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_PIN, FALSE);
//	m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnLoadSequenceCancelButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
		SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	if( nexifCancel(this->m_hWnd)==TRUE ){
		SetOperationLog("Sequence - Load/Unload - CANCE Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("シーケンスのキャンセルに失敗しました");
		SetWarningMessageText( IDS_FAILED_CANCEL_SEQUENCE );
	}
}

// =========================================================================
//
void CMotionDlg::OnSampleClampSequenceOpenButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
		SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= OFF){		// ピン下端						＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= ON ){		// ワークガイドCLOSE位置(FWD)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= OFF){		// ワークガイドOPEN位置(REV)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( (result=nexioIsPinAlignmentPos())!=ON ){		// ピン中間						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター中間を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_MIDDLE );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, TRUE);

	if( nexifOpenWorkGuide(this->m_hWnd)==TRUE ){
		SetOperationLog("Sequence - Clamp - OPEN Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドのオープンに失敗しました");
		SetWarningMessageText( IDS_FAILED_OPEN_WORKGUIDE );
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnSampleClampSequenceCloseButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
		SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= OFF){		// ピン下端						＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsWorkGuideClose())!= OFF){		// ワークガイドCLOSE位置(FWD)	＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドCLOSEを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_CLOSE );
		return;
	}
	if( (result=nexioIsWorkGuideOpen())	!= ON ){		// ワークガイドOPEN位置(REV)	＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドOPENを確認して下さい");
		SetWarningMessageText( IDS_CHECK_WORKGUIDE_OPEN );
		return;
	}
	if( (result=nexioIsPinAlignmentPos())!=ON ){		// ピン中間						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター中間を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_MIDDLE );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, TRUE);

	if( nexifCloseWorkGuide(this->m_hWnd)==TRUE ){
		SetOperationLog("Sequence - Clamp - CLOSE Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドのクローズに失敗しました");
		SetWarningMessageText( IDS_FAILED_CLOSE_WORKGUIDE );
	}
// ActuateFlagは応答を待って落とす
//	m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, FALSE);
}

// =========================================================================
//
void CMotionDlg::OnClampSequenceCancelButton()
{
BOOL result;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if( (result=nexioIsEmergencyStop()) != ON ){		// 緊急停止						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("EMOスイッチを確認して下さい");
		SetWarningMessageText( IDS_CHECK_EMO_SW );
		return;
	}
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
//	if( (result=nexioIsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 --------			 */
	if( (result = m_pDoc->Rap_IsDoorInterlock()) != ON ){		/* ドアインターロック			＝Ｈ */
/* modified hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ドアインターロックを確認して下さい");
		SetWarningMessageText( IDS_CHECK_DOOR_INTERLOCK );
		return;
	}
	if( (result=nexioIsEquipmentPower())!= ON ){		// 装置電源						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("装置電源を確認して下さい");
		SetWarningMessageText( IDS_CHECK_EQUIPMENT_POWER );
		return;
	}
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	if( (result=nexioIsAirPressureLevelLow())!=OFF ){	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	if ((result = m_pDoc->Rap_IsAirPressureLowON(1)) != OFF) {	/* 供給CDA圧力低下アラーム		＝Ｌ */
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("エア圧力を確認して下さい");
		SetWarningMessageText( IDS_CHECK_AIR_PRESSURE );
		return;
	}
	if( (result=nexioIsShutterClose())	!= ON ){		// シャッター閉					＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ローディングシャッター閉を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOADING_SHUTTER );
		return;
	}
	if( (result=nexioIsRobotArmDetect())!= ON ){		// ロボットアーム干渉外			＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロボットアームを確認して下さい");
		SetWarningMessageText( IDS_CHECK_ROBOT_ARM );
		return;
	}
	if( (result=nexioIsStageLoadPos())	!= ON ){		// ロードポジション				＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ロードポジションを確認して下さい");
		SetWarningMessageText( IDS_CHECK_LOAD_POSITION );
		return;
	}
	if( (result=nexioIsPinDownPos())	!= OFF){		// ピン下端						＝Ｌ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター下端を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_LOWER );
		return;
	}
	if( (result=nexioIsPinAlignmentPos())!=ON ){		// ピン中間						＝Ｈ
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフター中間を確認して下さい");
		SetWarningMessageText( IDS_CHECK_LIFTER_MIDDLE );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	// ▲インターロック条件▲

	if( nexifCancel(this->m_hWnd)==TRUE ){
		SetOperationLog("Sequence - Clamp - CANCE Button was pushed.");
		m_ctlWarningMessage.SetWindowText("");
	}
	else{
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドのキャンセルに失敗しました");
		SetWarningMessageText( IDS_FAILED_CANCEL_WORKGUIDE );
	}
}

// =========================================================================
//
void CMotionDlg::OnCloseButton()
{
	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){
			// Kojika 20090602 Change
			//m_ctlWarningMessage.SetWindowText("メンテナンスＳＷを確認して下さい");
			SetWarningMessageText( IDS_CHECK_MAINTENANCE_SW );
			return;
		}
	}
	else{												// 通常動作
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ステージが動作しています");
		SetWarningMessageText( IDS_STAGE_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("Ｚ軸が動作しています");
		SetWarningMessageText( IDS_Z_AXIS_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_PIN) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("リフターが動作しています");
		SetWarningMessageText( IDS_LIFTER_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ワークガイドが動作しています");
		SetWarningMessageText( IDS_CLAMP_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_TURRET) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("ターレットが動作しています");
		SetWarningMessageText( IDS_TURRET_WORKING );
		return;
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER) ){
		// Kojika 20090602 Change
		//m_ctlWarningMessage.SetWindowText("シャッターが動作しています");
		SetWarningMessageText( IDS_SHUTTER_WORKING );
		return;
	}
// 2009.11.09 bagus MS 修正 --{--
	if( m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) ){
		SetWarningMessageText( IDS_MICROSCOPE_WORKING );
		return;
	}
	if ( nexioRC_GetJOGP() || nexioRC_GetJOGM() ) {
		if(nexifRC_JogStop(this->m_hWnd)){
			MSG 	msg;

			//応答を処理するまで待つ
			while (1){
				if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
					break;
				}else{
					Sleep(10);
				}
			}
		}
	}
	nexioMS_ModeLamp(FALSE);
	nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
// 2009.11.09 bagus MS 修正 --}--
	// ▲インターロック条件▲

	SetOperationLog("CLOSE Button was pushed.");
	EndDialog(IDOK);
}

// =========================================================================
//
void CMotionDlg::SetOperationLog(LPCTSTR msg)
{
	char strmsg[1024];

	// 操作ログを取得
	memset( strmsg, 0, sizeof(strmsg));
	strcpy( strmsg, "[");
	::GetWindowText(this->m_hWnd, &strmsg[1], 1000);
	_tcscat( strmsg, "] ");
	_tcscat( strmsg, msg);

	m_pDoc->OperationLogging(strmsg);
}
/*
	*	Nextra からの応答(完了)メッセージジハンドラ
	*/
LRESULT CMotionDlg::OnNextraResponse(WPARAM wparam, LPARAM lparam)
{
	DWORD	l_dwDeviceCode = (DWORD) wparam;
	int		l_iResult = (int) ((0 == lparam)? 1 : 0);

	if ((nexMoveToUpper == l_dwDeviceCode) || (nexMoveToLower == l_dwDeviceCode) ||
		(nexMoveToAlignment == l_dwDeviceCode)) {
		m_pDoc->ActuateFlagsSet(ACTUATE_PIN, FALSE);			// 動作中フラグ(ピン)をオフ
	}
	if ((nexOpenShutter == l_dwDeviceCode) || (nexCloseShutter == l_dwDeviceCode)) {
		m_pDoc->ActuateFlagsSet(ACTUATE_SHUTTER, FALSE);		// 動作中フラグ(シャッタ)をオフ
	}
	if ((nexLoad == l_dwDeviceCode) || (nexUnload == l_dwDeviceCode)) {
		m_pDoc->ActuateFlagsSet(ACTUATE_PIN, FALSE);			// 動作中フラグ(ピン)をオフ
		m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, FALSE);	// 動作中フラグ(ワークガイド)をオフ
	}
	if ((nexOpenWorkGuide == l_dwDeviceCode) || (nexCloseWorkGuide == l_dwDeviceCode)) {
		m_pDoc->ActuateFlagsSet(ACTUATE_WORKGUIDE, FALSE);	// 動作中フラグ(ワークガイド)をオフ
	}
// 2009.11.09 bagus MS 修正 --{--
	if ((nexRC_MoveToTeachPos == l_dwDeviceCode)
	 || (nexRC_JogPlus == l_dwDeviceCode)
	 || (nexRC_JogMinus == l_dwDeviceCode)
	 || (nexRC_JogStop == l_dwDeviceCode)
	 || (nexRC_SelectMode == l_dwDeviceCode)) {
		m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);	// 動作中フラグ(顕微鏡)をオフ
	}
// 2009.11.09 bagus MS 修正 --}--

	return 0L;
}

/*	2009.06.02 Kojika
	*  m_ctlWarningMessage Language Change
	*/
void CMotionDlg::SetWarningMessageText(int idsno)
{
	CString l_strBuffer;

	switch (idsno) {
	case IDS_CHECK_MAINTENANCE_SW:
		LoadStringML(IDS_CHECK_MAINTENANCE_SW, l_strBuffer, "Please check the maintenance SW.");
		break;
	case IDS_CHECK_EMO_SW:
		LoadStringML(IDS_CHECK_EMO_SW, l_strBuffer, "Please check the EMO SW.");
		break;
	case IDS_CHECK_DOOR_INTERLOCK:
		LoadStringML(IDS_CHECK_DOOR_INTERLOCK, l_strBuffer, "Please check the door interlock.");
		break;
	case IDS_CHECK_EQUIPMENT_POWER:
		LoadStringML(IDS_CHECK_EQUIPMENT_POWER, l_strBuffer, "Please check the equipment power.");
		break;
	case IDS_CHECK_AIR_PRESSURE:
		LoadStringML(IDS_CHECK_AIR_PRESSURE, l_strBuffer, "Please check the air pressure.");
		break;
	case IDS_WORKING_CONDITION_INSUFFICIENT:
		LoadStringML(IDS_WORKING_CONDITION_INSUFFICIENT, l_strBuffer, "The working condition is insufficient. ");
		break;
	case IDS_CHECK_ROBOT_ARM:
		LoadStringML(IDS_CHECK_ROBOT_ARM, l_strBuffer, "Please check the robot arm.");
		break;
	case IDS_CHECK_LIFTER_LOWER:
		LoadStringML(IDS_CHECK_LIFTER_LOWER, l_strBuffer, "Please check the lifter lower.");
		break;
	case IDS_CHECK_WORKGUIDE_CLOSE:
		LoadStringML(IDS_CHECK_WORKGUIDE_CLOSE, l_strBuffer, "Please check the workguide CLOSE.");
		break;
	case IDS_CHECK_WORKGUIDE_OPEN:
		LoadStringML(IDS_CHECK_WORKGUIDE_OPEN, l_strBuffer, "Please check the workguide OPEN.");
		break;
	case IDS_CHECK_MOVO_ALARM:
		LoadStringML(IDS_CHECK_MOVO_ALARM, l_strBuffer, "Please check the MOVO Alarm.");
		break;
	case IDS_CHECK_LOADING_SHUTTER:
		LoadStringML(IDS_CHECK_LOADING_SHUTTER, l_strBuffer, "Please check the Loading shutter CLOSE");
		break;
	case IDS_CHECK_VACCUM:
		LoadStringML(IDS_CHECK_VACCUM, l_strBuffer, "Please check the vaccum");
		break;
	case IDS_CHECK_LOAD_POSITION:
		LoadStringML(IDS_CHECK_LOAD_POSITION, l_strBuffer, "Please check the Load position");
		break;
	case IDS_CHECK_LIFTER_UPPER:
		LoadStringML(IDS_CHECK_LIFTER_UPPER, l_strBuffer, "Please check the lifter upper.");
		break;
	case IDS_CHECK_LIFTER_MIDDLE:
		LoadStringML(IDS_CHECK_LIFTER_MIDDLE, l_strBuffer, "Please check the lifter middle.");
		break;
	case IDS_STAGE_WORKING:
		LoadStringML(IDS_STAGE_WORKING, l_strBuffer, "Stage is moving.");
		break;
	case IDS_Z_AXIS_WORKING:
		LoadStringML(IDS_Z_AXIS_WORKING, l_strBuffer, "Z Axis is Working");
		break;
	case IDS_LIFTER_WORKING:
		LoadStringML(IDS_LIFTER_WORKING, l_strBuffer, "Lifters is Working");
		break;
	case IDS_CLAMP_WORKING:
		LoadStringML(IDS_CLAMP_WORKING, l_strBuffer, "Work Guide is moving.");
		break;
	case IDS_TURRET_WORKING:
		LoadStringML(IDS_TURRET_WORKING, l_strBuffer, "Turret is moving.");
		break;
	case IDS_SHUTTER_WORKING:
		LoadStringML(IDS_SHUTTER_WORKING, l_strBuffer, "Shutter is Working");
		break;
// 2009.11.09 bagus MS 修正 --{--
	case IDS_MICROSCOPE_WORKING:
		LoadStringML(IDS_MICROSCOPE_WORKING, l_strBuffer, "Micro Scope is Working");
		break;
// 2009.11.09 bagus MS 修正 --}--
	case IDS_FAILED_INITIAL_STAGE:
		LoadStringML(IDS_FAILED_INITIAL_STAGE, l_strBuffer, "The initialization of the stage failed.");
		break;
	case IDS_FAILED_INITIAL_Z_AXIS:
		LoadStringML(IDS_FAILED_INITIAL_Z_AXIS, l_strBuffer, "The initialization of the Z axis failed.");
		break;
	case IDS_FAILED_INITIAL_TURRET:
		LoadStringML(IDS_FAILED_INITIAL_TURRET, l_strBuffer, "The initialization of the turret failed.");
		break;
	case IDS_FAILED_OPTICAL_FILTER:
		LoadStringML(IDS_FAILED_OPTICAL_FILTER, l_strBuffer, "The operation of the optical filter failed.");
		break;
	case IDS_FAILED_VACCUM_ON:
		LoadStringML(IDS_FAILED_VACCUM_ON, l_strBuffer, "It failed in vacuum ON.");
		break;
	case IDS_FAILED_VACCUM_OFF:
		LoadStringML(IDS_FAILED_VACCUM_OFF, l_strBuffer, "It failed in vacuum OFF");
		break;
	case IDS_FAILED_POSITION_MOVEMENT1:
		LoadStringML(IDS_FAILED_POSITION_MOVEMENT1, l_strBuffer, "It failed in the position the movement of position 1.");
		break;
	case IDS_FAILED_POSITION_MOVEMENT2:
		LoadStringML(IDS_FAILED_POSITION_MOVEMENT2, l_strBuffer, "It failed in the position the movement of position 2.");
		break;
	case IDS_FAILED_POSITION_MOVEMENT3:
		LoadStringML(IDS_FAILED_POSITION_MOVEMENT3, l_strBuffer, "It failed in the position the movement of position 3.");
		break;
	case IDS_FAILED_POSITION_MOVEMENT4:
		LoadStringML(IDS_FAILED_POSITION_MOVEMENT4, l_strBuffer, "It failed in the position the movement of position 4.");
		break;
	case IDS_FAILED_RISE_SAMPLE_LIFTER:
		LoadStringML(IDS_FAILED_RISE_SAMPLE_LIFTER, l_strBuffer, "It failed in the rise of the sample lifter.");
		break;
	case IDS_FAILED_ALIGNMENT_POSITION_MOVE:
		LoadStringML(IDS_FAILED_ALIGNMENT_POSITION_MOVE, l_strBuffer, "It failed in the alignment position movement. ");
		break;
	case IDS_FAILED_DESCENT_SAMPLE_LIFTER:
		LoadStringML(IDS_FAILED_DESCENT_SAMPLE_LIFTER, l_strBuffer, "It failed in the descent of the sample lifter.");
		break;
	case IDS_FAILED_LOAD_SEQUENCE:
		LoadStringML(IDS_FAILED_LOAD_SEQUENCE, l_strBuffer, "It failed in loading the sequence. ");
		break;
	case IDS_FAILED_UNLOAD_SEQUENCE:
		LoadStringML(IDS_FAILED_UNLOAD_SEQUENCE, l_strBuffer, "It failed in unloading the sequence.");
		break;
	case IDS_FAILED_CANCEL_SEQUENCE:
		LoadStringML(IDS_FAILED_CANCEL_SEQUENCE, l_strBuffer, "It failed in the cancellation of the sequence.");
		break;
	case IDS_FAILED_OPEN_WORKGUIDE:
		LoadStringML(IDS_FAILED_OPEN_WORKGUIDE, l_strBuffer, "It failed in the opening of the work guide. ");
		break;
	case IDS_FAILED_CLOSE_WORKGUIDE:
		LoadStringML(IDS_FAILED_CLOSE_WORKGUIDE, l_strBuffer, "It failed in the close of the work guide. ");
		break;
	case IDS_FAILED_CANCEL_WORKGUIDE:
		LoadStringML(IDS_FAILED_CANCEL_WORKGUIDE, l_strBuffer, "It failed in the cancellation of the work guide.");
		break;
	default:
		break;
	}
	m_ctlWarningMessage.SetWindowText(l_strBuffer);
}

// 2009.10.29 bagus MS 追加修正 --{--
// =========================================================================
//レンズ コンボイニシャル処理
//
void CMotionDlg::InitCombo_Lens()
{
	CString l_strBuffer, l_strTitle;
// 2009.11.09 bagus MS 修正 --{--
//	MS_CONFIG MsConfig;

//	ConfigFile_GetNanoSpecIni(&MsConfig, CONFIG_FILE_MS_CONFIG);
// 2009.11.09 bagus MS 修正 --}--

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < MAX_MS_ROBO_CYLINDER_NUM; i++ ) {
// 2009.11.09 bagus MS 修正 --{--
//		if (MsConfig.RoboCylinder[i].bEnable ) {
//			nIndex = pCombo->AddString(MsConfig.RoboCylinder[i].szName);
		if (m_MsConfig.RoboCylinder[i].bEnable ) {
			nIndex = pCombo->AddString(m_MsConfig.RoboCylinder[i].szName);
// 2009.11.09 bagus MS 修正 --}--
			pCombo->SetItemData(nIndex, i);
// matsuhisa 2009.12.26 added >>>
			if (m_MsConfig.RoboCylinder[i].bSensor) {
				m_nLensSensorItem = i;
			}
// matsuhisa 2009.12.26 added <<<
		}
	}
	pCombo->SetCurSel(0);
}

void CMotionDlg::OnMicroScopeMove()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

// 2009.11.09 bagus MS 修正 --{--
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_LENS);
	int	nIndex = pCombo->GetCurSel();
	int	nItemData = pCombo->GetItemData(nIndex);

	int	nPosNo = m_MsConfig.RoboCylinder[nItemData].nPosition;

	CString l_strBuffer, l_strTitle, strMsg;
// matsuhisa 2009.12.26 added >>>
	if ( !nexioIsMS_LensKind() ) {
		if ( nPosNo == m_MsConfig.RoboCylinder[m_nLensSensorItem].nPosition ) {
			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
			m_pDoc->MessageStringIf_Set(strMsg);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
	}
// matsuhisa 2009.12.26 added <<<

//	if ( CCursor::m_bWaitCursor ) {
//		pCombo->SetCurSel(m_nSelLens);
//		return;
//	}

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	if (m_pDoc->GetHostMode() == HOST_REMOTE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if ( nexioIsMaintenanceSwitch() != OFF ) {
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

	// 各I/Oチェック
	if (m_pDoc->IsInterLock() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
	if (m_pDoc->CheckUnitStatus() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		pCombo->SetCurSel(m_nSelLens);
		return;
	} //ステージが動作中です
	if (m_pDoc->CheckActiveFlag() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
	// ▲インターロック条件▲

	if (!m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
// matsuhisa 2009.12.26 deleted >>>
//		if (m_MsConfig.RoboCylinder[nItemData].bSensor == nexioIsMS_LensKind()) {
// matsuhisa 2009.12.26 deleted <<<
			// 他の移動入力の受付を禁止する
			if( !m_pDoc->JoyStickChangeDisable() ){
				LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
				LoadStringML(IDS_TITLE_WARNING, l_strTitle, "Warning");
				MessageBox(strMsg, l_strTitle, MB_OK);
				// 必ずJoyStickの状態を戻すこと
				m_pDoc->JoyStickStatusRestore();
				pCombo->SetCurSel(m_nSelLens);
				return;
			}

			// ActuateFlagをセットする
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);

			// Mode切り替え
			unsigned char oldMode = nexioRC_GetCurrentMode();
			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
				MSG 	msg;

				//応答を処理するまで待つ
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}

			// 位置決め
			if(nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo)){
				// ActuateFlagをセットする
				m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);
				MSG 	msg;

				//応答を処理するまで待つ
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}

			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, oldMode);
				MSG 	msg;

				//応答を処理するまで待つ
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);
// matsuhisa 2009.12.26 deleted >>>
//		} else {
//			// 顕微鏡のレンズの設定が一致しない
//			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
//			m_pDoc->MessageStringIf_Set(strMsg);
//			pCombo->SetCurSel(m_nSelLens);
//		}
// matsuhisa 2009.12.26 deleted <<<
	}

	m_nSelLens = nIndex;
// 2009.11.09 bagus MS 修正 --}--
}

void CMotionDlg::OnMicroScopeStdMode()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

// 2009.11.09 bagus MS 修正 --{--
	CString l_strBuffer, l_strTitle, strMsg;

//	if ( CCursor::m_bWaitCursor ) {
//		return;
//	}

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	if (m_pDoc->GetHostMode() == HOST_REMOTE) {
		return;
	}

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if ( nexioIsMaintenanceSwitch() != OFF ) {
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

	// 各I/Oチェック
	if (m_pDoc->IsInterLock() == TRUE) {
		return;
	}
	if (m_pDoc->CheckUnitStatus() == TRUE) {
		return;
	}

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	if (m_pDoc->CheckActiveFlag() == TRUE) {
		return;
	}
	// ▲インターロック条件▲

	if (!m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
		// 他の移動入力の受付を禁止する
		if( !m_pDoc->JoyStickChangeDisable() ){
			LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
			LoadStringML(IDS_TITLE_WARNING, l_strTitle, "Warning");
			MessageBox(strMsg, l_strTitle, MB_OK);
			// 必ずJoyStickの状態を戻すこと
			m_pDoc->JoyStickStatusRestore();
			return;
		}

		// ActuateFlagをセットする
		m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);
		// Mode切り替え
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);

	}
// 2009.11.09 bagus MS 修正 --}--
}

void CMotionDlg::OnMicroScopeJogIncing()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

// 2009.11.09 bagus MS 修正 --{--
	CString l_strBuffer, l_strTitle, strMsg;

//	if ( CCursor::m_bWaitCursor ) {
//		return;
//	}

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	if (m_pDoc->GetHostMode() == HOST_REMOTE) {
		return;
	}

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if ( nexioIsMaintenanceSwitch() != OFF ) {
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

	// 各I/Oチェック
	if (m_pDoc->IsInterLock() == TRUE) {
		return;
	}
	if (m_pDoc->CheckUnitStatus() == TRUE) {
		return;
	}

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	if (m_pDoc->CheckActiveFlag() == TRUE) {
		return;
	}
	// ▲インターロック条件▲

	if (!m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
		// 他の移動入力の受付を禁止する
		if( !m_pDoc->JoyStickChangeDisable() ){
			LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
			LoadStringML(IDS_TITLE_WARNING, l_strTitle, "Warning");
			MessageBox(strMsg, l_strTitle, MB_OK);
			// 必ずJoyStickの状態を戻すこと
			m_pDoc->JoyStickStatusRestore();
			return;
		}

		// ActuateFlagをセットする
		m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);

		// Mode切り替え
		int	nMode = nexioRC_GetCurrentMode();

		if (nMode == RCOPMODE_JOG) {
			nexifRC_SelectMode(this->m_hWnd, RCOPMODE_INCHING);
		} else if (nMode == RCOPMODE_INCHING) {
			nexifRC_SelectMode(this->m_hWnd, RCOPMODE_JOG);
		} else {
			nexifRC_SelectMode(this->m_hWnd, RCOPMODE_JOG);
		}
	}
// 2009.11.09 bagus MS 修正 --}--
}
// 2009.10.29 bagus MS 追加修正 --}--

void CMotionDlg::OnTimer(UINT nIDEvent)
{
	if (m_SystemConfig.HeadType.bMS) {
		switch(nIDEvent){
		case JOG_WATCH_TIMER_ID:
			if(m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)){
				//すでに顕微鏡用シリンダが動作しているのでスイッチをみない
			}else{
				switch(nexioRC_GetCurrentMode()){
				case RCOPMODE_JOG:
				case RCOPMODE_INCHING:
					if(nexioIsMS_DownSwitch() && nexioIsMS_UpSwitch()){
						//同時押し
					//2009.12.25 bagus MS --{--
					//}else if(nexioIsMS_UpSwitch() && nexioIsMS_UpperPos()){
					}else if(nexioIsMS_UpSwitch() && !nexioIsMS_UpperPos()){
					//2009.12.25 bagus MS --}--
						if(nexifRC_JogMinus(this->m_hWnd)){
							m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
						}
					}else if(nexioIsMS_DownSwitch() ){
						if(!nexioIsMS_LowerPos1() && !nexioIsMS_LensKind()){
							//下降端１でレンズ種別が1-50x
						}else if(!nexioIsMS_LowerPos2()){
							//一番下
						}else{
							//上昇
							if(nexifRC_JogPlus(this->m_hWnd)){
								m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
							}
						}
					}else{
						//ボタンが離されたのでジョグを止める
						if ( nexioRC_GetJOGP() || nexioRC_GetJOGM() ) {
							if(nexifRC_JogStop(this->m_hWnd)){
								m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
							}
						}
					}

					break;
				}

				if(nexioIsMS_ModeSwitch()){
					switch(nexioRC_GetCurrentMode()){
					case RCOPMODE_JOG:
						m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
						nexifRC_SelectMode(this->m_hWnd,RCOPMODE_INCHING);
						break;
					case RCOPMODE_INCHING:
						m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
						nexifRC_SelectMode(this->m_hWnd,RCOPMODE_JOG);
						break;
					}
				}else{
				}
			}
			break;
		case JOG_MODE_LAMP_TIMER_ID:
			switch(nexioRC_GetCurrentMode()){
			case RCOPMODE_JOG:
				nexioMS_ModeLamp(TRUE);
				break;
			case RCOPMODE_INCHING:
				if(m_bMSModeLampFilcker)
					nexioMS_ModeLamp(FALSE);
				else
					nexioMS_ModeLamp(TRUE);
				m_bMSModeLampFilcker = !m_bMSModeLampFilcker;
				break;
			default:
				nexioMS_ModeLamp(FALSE);
				break;
			}
			break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

