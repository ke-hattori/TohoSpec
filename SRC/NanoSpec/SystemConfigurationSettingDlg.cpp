// SystemConfigurationSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SystemConfigurationSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSystemConfigurationSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSystemConfigurationSettingDlg::CSystemConfigurationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemConfigurationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemConfigurationSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemConfigurationSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_HEAD_TYPE_SR, m_SystemConfig.HeadType.bSR);
	DDX_Check(pDX, IDC_CHECK_HEAD_TYPE_SE, m_SystemConfig.HeadType.bSE);
	DDX_Check(pDX, IDC_CHECK_HEAD_TYPE_COMPEASE, m_SystemConfig.HeadType.bCompEASE);
	DDX_Check(pDX, IDC_CHECK_HEAD_TYPE_4PP, m_SystemConfig.HeadType.bResist);
	DDX_Check(pDX, IDC_CHECK_HEAD_TYPE_CONTACT_ANGLE, m_SystemConfig.HeadType.bCTA);
	DDX_Check(pDX, IDC_CHECK_HEAD_TYPE_STRESS, m_SystemConfig.HeadType.bStress);
// 2009.10.17 bagus MS 追加 --{--
//	DDX_Check(pDX, IDC_CHECK_HEAD_TYPE_IRSE, m_SystemConfig.HeadType.bIRSE);
	DDX_Check(pDX, IDC_CHECK_HEAD_TYPE_MS, m_SystemConfig.HeadType.bMS);
// 2009.10.17 bagus MS 追加 --}--
	DDX_Check(pDX, IDC_CHECK_VIBRATION_DOWN_SENSOR, m_SystemConfig.bVibrationDownSensor);
	DDX_Check(pDX, IDC_CHECK_GLASS_EXISTENCE, m_SystemConfig.bSampleExistenceSensor);
	DDX_Check(pDX, IDC_CHECK_SAMPLE_PATTERN_REC, m_SystemConfig.bSamplePatternRec);
	DDX_Check(pDX, IDC_CHECK_SITE_PATTERN_REC, m_SystemConfig.bSitePatternRec);
	DDX_Check(pDX, IDC_CHECK_HOST, m_SystemConfig.bHost);
	DDX_Check(pDX, IDC_CHECK_SIGNAL_TOWER, m_SystemConfig.bSignalTower);
	DDX_Text(pDX, IDC_VACUUM_TIMEOUT, m_SystemConfig.iVacuumTimeout);
	DDX_Check(pDX, IDC_CHECK_MULTI_RECIPE, m_SystemConfig.bMultiRecipe);
	DDX_Check(pDX, IDC_CHECK_EXTRA_VIEW_CAMERRA, m_SystemConfig.bExtraViewCamera);
	DDX_Check(pDX, IDC_CHECK_SAMPLE_ALIGNER, m_SystemConfig.bSampleAligner);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
/* added 2009.06.16 hmenjo Loading Shutter 使用有無追加 ---------- { ---------- */
	DDX_Check(pDX, IDC_CHECK_LOADING_SHUTTER, m_SystemConfig.bUseLoadingShutter);
/* added 2009.06.16 hmenjo Loading Shutter 使用有無追加 ---------- } ---------- */
/* deleted 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- { ---------- */
///* added 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- { ---------- */
//	DDX_Check(pDX, IDC_CHK_USEOPTFILTERAF, m_SystemConfig.bUseOptFilterMeasPgmAF);
///* added 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- } ---------- */
/* deleted 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- } ---------- */
// 2009.12.10 bagus Recipe Backup --{--
	DDX_Check(pDX, IDC_CHECK_BACKUP, m_SystemConfig.bBackup);
// 2009.12.10 bagus Recipe Backup --}--
	DDX_Check(pDX, IDC_CHECK_DISABLE_NEXTRA, m_SystemConfig.bDisableNextra);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSystemConfigurationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemConfigurationSettingDlg)
	ON_CBN_SELCHANGE(IDC_CAPTURE_BOARD, OnSelchangeCaptureBoard)
	ON_CBN_SELCHANGE(IDC_PR_METHOD, OnSelchangePRMethod)
	ON_CBN_SELCHANGE(IDC_LANGUAGE, OnSelchangeLanguage)
	ON_CBN_SELCHANGE(IDC_STAGE_TYPE, OnSelchangeStageType)
	ON_CBN_SELCHANGE(IDC_VIBRATION_CONTROL_UNIT, OnSelchangeVibrationControlUnit)
	ON_CBN_SELCHANGE(IDC_SAMPLE_LIFTER, OnSelchangeSampleLifter)
	ON_CBN_SELCHANGE(IDC_VACUUM_TYPE, OnSelchangeVacuumType)
	ON_CBN_SELCHANGE(IDC_CORRECT_ANGLE, OnSelchangeCorrectAngle)
	ON_CBN_SELCHANGE(IDC_JOYSTICK_TYPE, OnSelchangeJoystickType)
	ON_BN_CLICKED(IDC_CHECK_HEAD_TYPE_SE, OnCheckHeadTypeSe)
	ON_BN_CLICKED(IDC_CHECK_HEAD_TYPE_COMPEASE, OnCheckHeadTypeCompEASE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSystemConfigurationSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSystemConfigurationSettingDlg::OnInitDialog()
{
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		m_SystemConfig.HeadType.bSR = TRUE;
		GetDlgItem(IDC_CHECK_HEAD_TYPE_SR)->EnableWindow(FALSE);
	}
// 2013.11.08 Bagus Add (TohoSpec対応) -->

	//未対応機能disable
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_CHECK_HEAD_TYPE_SE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_COMPLETE_EASE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_COMPEASE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_4PP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_CONTACT_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_STRESS)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_CAPTURE_BOARD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CAPTURE_BOARD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PR_METHOD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PR_METHOD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_SAMPLE_PATTERN_REC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_SITE_PATTERN_REC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_HOST)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CHECK_DISABLE_NEXTRA)->ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		//2009.09.02 bagus se SE機能追加 --{--
		//GetDlgItem(IDC_CHECK_HEAD_TYPE_SE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_SE)->EnableWindow(TRUE);
		//2009.09.02 bagus se SE機能追加 --}--
		// 2009.11.04 bagus RS 追加 --{--
		//GetDlgItem(IDC_CHECK_HEAD_TYPE_4PP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_4PP)->EnableWindow(TRUE);
		// 2009.11.04 bagus RS 追加 --}--
		// 2009.10.07 bagus CTA --{--
		//GetDlgItem(IDC_CHECK_HEAD_TYPE_CONTACT_ANGLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_CONTACT_ANGLE)->EnableWindow(TRUE);
		// 2009.10.07 bagus CTA --}--
		//@@@ bagus stress ストレス機能追加 --{--
		GetDlgItem(IDC_CHECK_HEAD_TYPE_STRESS)->EnableWindow(TRUE);
		//@@@ bagus stress ストレス機能追加 --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	// Okabe Change 2009.05.30 ----->
	//GetDlgItem(IDC_CHECK_SITE_PATTERN_REC)->EnableWindow(FALSE);
	// Okabe Change 2009.05.30 <-----
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_CHECK_HEAD_TYPE_MS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_SIGNAL_TOWER)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_STAGE_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_JOYSTICK_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK_TYPE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CHECK_MULTI_RECIPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_EXTRA_VIEW_CAMERRA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_LOADING_SHUTTER)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_CORRECT_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CORRECT_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_VIBRATION_DOWN_SENSOR)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_VIBRATION_CONTROL_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VIBRATION_CONTROL_UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_VIBRATION_DOWN_SENSOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_GLASS_EXISTENCE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_SAMPLE_LIFTER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SAMPLE_LIFTER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_SAMPLE_ALIGNER)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_VACUUM_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VACUUM_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VACUUM_TIMEOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VACUUM_TIMEOUT)->ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
// 2009.10.17 bagus MS 追加 --{--
//		GetDlgItem(IDC_CHECK_HEAD_TYPE_IRSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_MS)->EnableWindow(TRUE);
// 2009.10.17 bagus MS 追加 --}--
		GetDlgItem(IDC_CHECK_SIGNAL_TOWER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MULTI_RECIPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CORRECT_ANGLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_VIBRATION_DOWN_SENSOR)->EnableWindow(FALSE);

		GetDlgItem(IDC_VIBRATION_CONTROL_UNIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_VIBRATION_DOWN_SENSOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_GLASS_EXISTENCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SAMPLE_LIFTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAMPLE_ALIGNER)->EnableWindow(FALSE);
		GetDlgItem(IDC_VACUUM_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_VACUUM_TIMEOUT)->EnableWindow(FALSE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--


	///// Load System Config File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	CDialog::OnInitDialog();

	///// Control Initialize /////
	InitCombo_CaptureBoard();
	InitCombo_PRMethod();
	InitCombo_Language();
	InitCombo_StageType();
	InitCombo_StageVibrationControlUnit();
	InitCombo_StageSampleLifter();
	InitCombo_StageVacuumType();
	InitCombo_StageCorrectAngle();
// 2009.10.29 bagus Jog 追加修正 --{--
	InitCombo_StageJoystickType();
// 2009.10.29 bagus Jog 追加修正 --}--

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnOK()
{
	// Kojika 20090603 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090603 Add End

	///// Save /////
	if(!CheckData()) return;

// 2009.10.17 bagus MS 追加 --{--
#if 0
	if ( !m_SystemConfig.HeadType.bCTA && !m_SystemConfig.HeadType.bStress &&
			!m_SystemConfig.HeadType.bSR && !m_SystemConfig.HeadType.bSE				&&
			!m_SystemConfig.HeadType.bIRSE && !m_SystemConfig.HeadType.bResist ) {
#else
	if ( !m_SystemConfig.HeadType.bCTA && !m_SystemConfig.HeadType.bStress &&
			!m_SystemConfig.HeadType.bSR && !m_SystemConfig.HeadType.bSE &&
			!m_SystemConfig.HeadType.bMS && !m_SystemConfig.HeadType.bResist ) {
#endif
// 2009.10.17 bagus MS 追加 --}--
		// Kojika 20090603 Change
		//if ( MessageBox("Although the Head Type is not Select, do it Save ?",
		//			"SYSTEM CONFIG SETTING", MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL) {
		LoadStringML(IDS_HEAD_TYPE_NOT_SELECT, l_strBuffer, "Although the Head Type is not Select, do it Save ?");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "SYSTEM CONFIG SETTING");
		if ( MessageBox(l_strBuffer, l_strTitle, MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL) {
			return;
		}
		// Kojika 20090603 Change End
	}

	ConfigFile_SetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnSelchangeCaptureBoard()
{
	m_SystemConfig.nCaptureBoard = ((CComboBox*)GetDlgItem(IDC_CAPTURE_BOARD))->GetCurSel();
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnSelchangePRMethod()
{
	m_SystemConfig.nPRMethod = ((CComboBox*)GetDlgItem(IDC_PR_METHOD))->GetCurSel();
	if (m_SystemConfig.nPRMethod != 0){
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
			GetDlgItem(IDC_CHECK_SAMPLE_PATTERN_REC)->EnableWindow(TRUE);
			// Okabe Change 2009.05.30 ----->
			GetDlgItem(IDC_CHECK_SITE_PATTERN_REC)->EnableWindow(TRUE);
			// Okabe Change 2009.05.30 <-----
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
			GetDlgItem(IDC_CHECK_SAMPLE_PATTERN_REC)->EnableWindow(FALSE);
			// Okabe Change 2009.05.30 ----->
			GetDlgItem(IDC_CHECK_SITE_PATTERN_REC)->EnableWindow(FALSE);
			// Okabe Change 2009.05.30 <-----
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	}
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnSelchangeLanguage()
{
	m_SystemConfig.nLanguage = ((CComboBox*)GetDlgItem(IDC_LANGUAGE))->GetCurSel();
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnSelchangeStageType()
{
	m_SystemConfig.nStageType = ((CComboBox*)GetDlgItem(IDC_STAGE_TYPE))->GetCurSel();
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnSelchangeVibrationControlUnit()
{
	m_SystemConfig.nVibrationControlUnit = ((CComboBox*)GetDlgItem(IDC_VIBRATION_CONTROL_UNIT))->GetCurSel();
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnSelchangeSampleLifter()
{
	m_SystemConfig.nSampleLifter = ((CComboBox*)GetDlgItem(IDC_SAMPLE_LIFTER))->GetCurSel();
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnSelchangeVacuumType()
{
	m_SystemConfig.nVacuumType = ((CComboBox*)GetDlgItem(IDC_VACUUM_TYPE))->GetCurSel();
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnSelchangeCorrectAngle()
{
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_CORRECT_ANGLE);
	switch(pCombo->GetCurSel()){
	case CORRECT_ANGLE_0:
		m_SystemConfig.dCorrectAngle = 0;
		break;
	case CORRECT_ANGLE_90:
		m_SystemConfig.dCorrectAngle = 90;
		break;
	case CORRECT_ANGLE_MINUS_90:
		m_SystemConfig.dCorrectAngle = -90;
		break;
	}
}

// 2009.10.29 bagus Jog 追加修正 --{--
void CSystemConfigurationSettingDlg::OnSelchangeJoystickType()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_SystemConfig.nJoystickType = ((CComboBox*)GetDlgItem(IDC_JOYSTICK_TYPE))->GetCurSel();
}
// 2009.10.29 bagus Jog 追加修正 --}--

// =========================================================================
//
void CSystemConfigurationSettingDlg::InitCombo_CaptureBoard()
{
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		CComboBox* pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_CAPTURE_BOARD);
		pCombo->ResetContent();
		for ( int i = 0; i < CAPTURE_BOARD_MAX; i++ ) {
			pCombo->AddString(g_lpszCaptureBoard[i]);
		}
		pCombo->SetCurSel(m_SystemConfig.nCaptureBoard);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::InitCombo_PRMethod()
{
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		CComboBox* pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_PR_METHOD);
		pCombo->ResetContent();
		for ( int i = 0; i < PR_METHOD_MAX; i++ ) {
			pCombo->AddString(g_lpszPRMethod[i]);
		}
		pCombo->SetCurSel(m_SystemConfig.nPRMethod);

		BOOL bEnable = ( m_SystemConfig.nPRMethod != PR_METHOD_NONE );
		GetDlgItem(IDC_CHECK_SAMPLE_PATTERN_REC)->EnableWindow(bEnable);
		GetDlgItem(IDC_CHECK_SITE_PATTERN_REC)->EnableWindow(bEnable);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::InitCombo_Language()
{
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_LANGUAGE);
	pCombo->ResetContent();
	for ( int i = 0; i < LANGUAGE_MAX; i++ ) {
		pCombo->AddString(g_lpszLanguage[i]);
	}
	pCombo->SetCurSel(m_SystemConfig.nLanguage);
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::InitCombo_StageType()
{
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		CComboBox* pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_STAGE_TYPE);
		pCombo->ResetContent();
		for ( int i = 0; i < STAGE_TYPE_MAX; i++ ) {
			pCombo->AddString(g_lpszStageType[i]);
		}
		pCombo->SetCurSel(m_SystemConfig.nStageType);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::InitCombo_StageVibrationControlUnit()
{
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_VIBRATION_CONTROL_UNIT);
	pCombo->ResetContent();
	for ( int i = 0; i < VIBRATION_CONTROL_UNIT_MAX; i++ ) {
		pCombo->AddString(g_lpszVibrationControlUnit[i]);
	}
	pCombo->SetCurSel(m_SystemConfig.nVibrationControlUnit);
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::InitCombo_StageSampleLifter()
{
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_SAMPLE_LIFTER);
	pCombo->ResetContent();
	for ( int i = 0; i < SAMPLE_LIFTER_MAX; i++ ) {
		pCombo->AddString(g_lpszSampleLifter[i]);
	}
	pCombo->SetCurSel(m_SystemConfig.nSampleLifter);
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::InitCombo_StageVacuumType()
{
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_VACUUM_TYPE);
	pCombo->ResetContent();
	for ( int i = 0; i < VACUUM_TYPE_MAX; i++ ) {
		pCombo->AddString(g_lpszVacuumType[i]);
	}
	pCombo->SetCurSel(m_SystemConfig.nVacuumType);
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::InitCombo_StageCorrectAngle()
{
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_CORRECT_ANGLE);
	pCombo->ResetContent();
	for ( int i = 0; i < CORRECT_ANGLE_MAX; i++ ) {
		pCombo->AddString(g_lpszCorrectAngle[i]);
	}
	int iTemp = (int)(m_SystemConfig.dCorrectAngle / 90);
	if ( iTemp < 0 ) {
		pCombo->SetCurSel(CORRECT_ANGLE_MINUS_90);
	}
	else {
		switch ( iTemp ) {
		case CORRECT_ANGLE_90:
			pCombo->SetCurSel(CORRECT_ANGLE_90);
			break;
		case CORRECT_ANGLE_0:
		default:
			pCombo->SetCurSel(CORRECT_ANGLE_0);
			break;
		}
	}
}

// 2009.10.29 bagus Jog 追加修正 --{--
void CSystemConfigurationSettingDlg::InitCombo_StageJoystickType()
{
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		CComboBox* pCombo;
		pCombo = (CComboBox*)GetDlgItem(IDC_JOYSTICK_TYPE);
		pCombo->ResetContent();
		for ( int i = 0; i < JOYSTICK_TYPE_MAX; i++ ) {
			pCombo->AddString(g_lpszJoystickType[i]);
		}
		pCombo->SetCurSel(m_SystemConfig.nJoystickType);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
}
// 2009.10.29 bagus Jog 追加修正 --}--

// =========================================================================
//
BOOL CSystemConfigurationSettingDlg::CheckData()
{
	// Kojika 20090603 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090603 Add End

	if(UpdateData(TRUE) == FALSE) return FALSE;

	int iValue;
	double dValue;
	CString strBuffer;

	///// Stage Vacuum Timeout /////
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		iValue = m_SystemConfig.iVacuumTimeout;
		if(!(iValue >= MIN_VACCUM_TIMEOUT && iValue <= MAX_VACCUM_TIMEOUT)){
			// Kojika 20090603 Change
			//strBuffer.Format("Stage Vacuum Timeout is out of range.\n(%d - %d)", MIN_VACCUM_TIMEOUT, MAX_VACCUM_TIMEOUT);
			//MessageBox(strBuffer, "System Configuration Setting", MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_STAGE_VACUUM_TIMEOUT_OUT_RANGE, l_strBuffer, "Stage Vacuum Timeout is out of range.\n(%d - %d)");
			LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
			strBuffer.Format(l_strBuffer, MIN_VACCUM_TIMEOUT, MAX_VACCUM_TIMEOUT);
			MessageBox(strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
			// Kojika 20090603 Change End
			GetDlgItem(IDC_VACUUM_TIMEOUT)->SetFocus();
			return FALSE;
		}
	}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--

	///// Capture Board /////
	iValue = m_SystemConfig.nCaptureBoard;
	if(iValue == -1){
		// Kojika 20090603 Change
		//MessageBox("Please Select an Capture Board", "System Configuration Setting", MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_CAPTURE_BOARD, l_strBuffer, "Please Select an Capture Board.");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONINFORMATION);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Image Board /////
	iValue = m_SystemConfig.nPRMethod;
	if(iValue == -1){
		// Kojika 20090603 Change
		//MessageBox("Please Select an Image Board", "System Configuration Setting", MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_IMAGE_BOARD, l_strBuffer, "Please Select an Image Board.");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONINFORMATION);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Language /////
	iValue = m_SystemConfig.nLanguage;
	if(iValue == -1){
		// Kojika 20090603 Change
		//MessageBox("Please Select an Language", "System Configuration Setting", MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_LANGUAGE, l_strBuffer, "Please Select an Language.");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONINFORMATION);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Stage Type /////
	iValue = m_SystemConfig.nStageType;
	if(iValue == -1){
		// Kojika 20090603 Change
		//MessageBox("Please Select an Stage Type", "System Configuration Setting", MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_STAGE_TYPE, l_strBuffer, "Please Select an Stage Type.");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONINFORMATION);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Stage Vibration Control Unit /////
	iValue = m_SystemConfig.nVibrationControlUnit;
	if(iValue == -1){
		// Kojika 20090603 Change
		//MessageBox("Please Select an Stage Vibration Control Unit", "System Configuration Setting", MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_STAGE_VIBRATION_CONTROL, l_strBuffer, "Please Select an Stage Vibration Control Unit.");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONINFORMATION);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Stage Sample Lifter /////
	iValue = m_SystemConfig.nSampleLifter;
	if(iValue == -1){
		// Kojika 20090603 Change
		//MessageBox("Please Select an Stage Sample Lifter", "System Configuration Setting", MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_STAGE_SAMPLE_LIFTER, l_strBuffer, "Please Select an Stage Sample Lifter.");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONINFORMATION);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Stage Vacuum Type /////
	iValue = m_SystemConfig.nVacuumType;
	if(iValue == -1){
		// Kojika 20090603 Change
		//MessageBox("Please Select an Stage Vacuum Type", "System Configuration Setting", MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_STAGE_VACUUM_TYPE, l_strBuffer, "Please Select an Stage Vacuum Type.");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONINFORMATION);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Correct Angle /////
	dValue = m_SystemConfig.dCorrectAngle;
	if(!(dValue == 90 || dValue == 0 || dValue == (-90))){
		// Kojika 20090603 Change
		//strBuffer.Format("Correct Angle is out of range.\n(%d or %d or %d)", 90, 0, -90);
		//MessageBox(strBuffer, "System Configuration Setting", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_CORRECT_ANGLE_OUT_RANGE, l_strBuffer, "Correct Angle is out of range.\n(%d or %d or %d)");
		LoadStringML(IDS_TITLE_SYSTEM_CONFIG_SETTING, l_strTitle, "System Configuration Setting");
		strBuffer.Format(l_strBuffer, 90, 0, -90);
		MessageBox(strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	return TRUE;
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnCheckHeadTypeSe()
{
	int iCheck;

	iCheck = ((CButton*)GetDlgItem(IDC_CHECK_HEAD_TYPE_SE))->GetCheck();
	if (iCheck == BST_UNCHECKED){
		((CButton*)GetDlgItem(IDC_CHECK_HEAD_TYPE_COMPEASE))->SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_CHECK_HEAD_TYPE_COMPEASE)->EnableWindow(FALSE);
	}
	else{
		GetDlgItem(IDC_CHECK_HEAD_TYPE_COMPEASE)->EnableWindow(TRUE);
	}
}

// =========================================================================
//
void CSystemConfigurationSettingDlg::OnCheckHeadTypeCompEASE()
{
	int iCheck;

	iCheck = ((CButton*)GetDlgItem(IDC_CHECK_HEAD_TYPE_COMPEASE))->GetCheck();
	if (iCheck == BST_CHECKED){
		((CButton*)GetDlgItem(IDC_CHECK_HEAD_TYPE_SE))->SetCheck(BST_CHECKED);
	}
}
