// SrCofigurationSettingDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SrCofigurationSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2009.09.19 bagus SE --{--
#define	MILLI_SEC		(1000.0)
// 2009.09.19 bagus SE --}--

// #########################################################################
// CSrCofigurationSettingDlg �_�C�A���O
// #########################################################################

// =========================================================================
//
CSrCofigurationSettingDlg::CSrCofigurationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrCofigurationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrCofigurationSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSrCofigurationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrCofigurationSettingDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrCofigurationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrCofigurationSettingDlg)
	ON_BN_CLICKED(IDC_CHECK_UV, OnCheckUv)
	ON_BN_CLICKED(IDC_CHECK_OD_MEASUREMENT, OnCheckOdMeasurement)
	ON_BN_CLICKED(IDC_CHECK_AUTO_FOCUS, OnCheckAutoFocus)
	ON_BN_CLICKED(IDC_CHECK_AUTO_FILTER, OnCheckAutoFilter)
	ON_BN_CLICKED(IDC_CHECK_TRANSMITTANCE, OnCheckTransmittance)
	ON_BN_CLICKED(IDC_CHECK_GANTRY_TYPE, OnCheckGantryType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSrCofigurationSettingDlg ���b�Z�[�W �n���h��

// =========================================================================
//
BOOL CSrCofigurationSettingDlg::OnInitDialog()
{
// 2009.09.19 bagus SE --{--
#if 0
	//���Ή��@�\disable
	GetDlgItem(IDC_CHECK_TRANSMITTANCE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_CIE_MEASUREMENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_UV)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_OD_MEASUREMENT)->EnableWindow(FALSE);
#endif
// 2009.09.19 bagus SE --}--

	memset(&m_SrConfig, 0, sizeof(m_SrConfig));
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);

// 2009.09.19 bagus SE --{--
	SR_TURRET SrTurret[SR_LENS_MAX];
	SR_FILTER SrFilter[SR_FILTER_MAX];

	memset(&SrTurret, 0, sizeof(SrTurret));
	ConfigFile_GetNanoSpecIni(SrTurret, CONFIG_FILE_SR_TURRET);

	memset(&SrFilter, 0, sizeof(SrFilter));
	ConfigFile_GetNanoSpecIni(SrFilter, CONFIG_FILE_SR_FILTER);

	int i;
	TCHAR szTemp[256];
	CComboBox* pCombo;

	pCombo = (CComboBox *)GetDlgItem(IDC_DEFAULT_MEASUREMENT_TURRET_NO);
	pCombo->ResetContent();
	for(i = 0; i < SR_LENS_MAX; i++){
		if ((!SrTurret[i].bEnable) || (!SrTurret[i].bAnalysis))
			continue;
		_stprintf(szTemp, "%d", i + 1);
		pCombo->AddString(szTemp);
	}
	if (pCombo->GetCount() > 0)
		pCombo->SetCurSel(0);

	pCombo = (CComboBox *)GetDlgItem(IDC_DEFAULT_MEASUREMENT_FILTER_NO);
	pCombo->ResetContent();
	_stprintf(szTemp, "%d", 1);
	pCombo->AddString(szTemp);
	for(i = 0; i < SR_FILTER_MAX; i++){
		if (!SrFilter[i].bEnable)
			continue;
		_stprintf(szTemp, "%d", i + 3);
		pCombo->AddString(szTemp);
	}
	if (pCombo->GetCount() > 0)
		pCombo->SetCurSel(0);

// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//	pCombo = (CComboBox *)GetDlgItem(IDC_USE_TURRET_NO);
//	pCombo->ResetContent();
//	for(i = 0; i < SR_LENS_MAX; i++){
//		if ((!SrTurret[i].bEnable) || (!SrTurret[i].bAnalysis))
//			continue;
//		_stprintf(szTemp, "%d", i + 1);
//		pCombo->AddString(szTemp);
//	}
//	if (pCombo->GetCount() > 0)
//		pCombo->SetCurSel(0);
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_USE_TURRET_NO)->ShowWindow(SW_HIDE);
	}
	else{
		pCombo = (CComboBox *)GetDlgItem(IDC_USE_TURRET_NO);
		pCombo->ResetContent();
		for(i = 0; i < SR_LENS_MAX; i++){
			if ((!SrTurret[i].bEnable) || (!SrTurret[i].bAnalysis))
				continue;
			_stprintf(szTemp, "%d", i + 1);
			pCombo->AddString(szTemp);
		}
		if (pCombo->GetCount() > 0)
			pCombo->SetCurSel(0);
	}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
// 2009.09.19 bagus SE --}--

// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_STATIC_OPTION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TURRET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TURRET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_AUTO_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHOOSE_AF_FUNC_IN_REFERENCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_AUTO_FILTER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHK_USEOPTFILTERAF)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_BUILT_IN_REFERENCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_UV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_UV_AUTO_SHUTTER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_UV_GAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_UV_GAIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_TRANSMITTANCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GROUP_TRANSMITTANCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_USE_TURRET_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_WAIT_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WAIT_TIME_FOR_SHUTTER_MOVING)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_UNIT_SEC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_GANTRY_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MEASUREMENT_POINT_NUMBER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NUMBER_OF_A_MEASUREMENT_POINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_UNIT_POINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_DISTANCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_CIE_MEASUREMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_OD_MEASUREMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_INTEGRATION_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SECOND_INTEGRATION_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TIME_UNIT_SEC)->ShowWindow(SW_HIDE);
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Set Data /////
	UpDate(FALSE);
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- { ---------- */
	OnCheckAutoFilter();
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- } ---------- */

	OnCheckUv();
// 2009.09.18 bagus SE --{--
	OnCheckTransmittance();
	OnCheckGantryType();
// 2009.09.18 bagus SE --}--
	OnCheckOdMeasurement();

	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
					// ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// =========================================================================
//
void CSrCofigurationSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);

	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- { ---------- */
	((CMainFrame*) AfxGetMainWnd())->m_srConfig.bUseOptFilterMeasPgmAF = m_SrConfig.bUseOptFilterMeasPgmAF;
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- } ---------- */
	CDialog::OnOK();
}

// =========================================================================
//
void CSrCofigurationSettingDlg::OnCheckUv()
{
	// UV Gain�̃G�f�B�b�g�{�b�N�X�̗L�������̐؂�ւ�
	GetDlgItem(IDC_UV_GAIN)->EnableWindow(IsDlgButtonChecked(IDC_CHECK_UV) == BST_CHECKED);
	// UV Auto Shutter�̃`�F�b�N�{�b�N�X�̗L�������̐؂�ւ�
	GetDlgItem(IDC_CHECK_UV_AUTO_SHUTTER)->EnableWindow(IsDlgButtonChecked(IDC_CHECK_UV) == BST_CHECKED);
}

// =========================================================================
//
void CSrCofigurationSettingDlg::UpDate(BOOL bValid)
{
	CString	strBuffer;
	UINT		uCheck;
	CComboBox* pCombo;
// 2009.09.19 bagus SE --{--
	int	nIndex;
// 2009.09.19 bagus SE --}--
	int i;

	if ( bValid ) {
		///// Option Auto Focus
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bAutoFocus = IsDlgButtonChecked(IDC_CHECK_AUTO_FOCUS);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bAutoFocus = FALSE;
		}
		else{
			m_SrConfig.bAutoFocus = IsDlgButtonChecked(IDC_CHECK_AUTO_FOCUS);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option Choose AF Function in Reference
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bChooseAfFuncInReference = IsDlgButtonChecked(IDC_CHOOSE_AF_FUNC_IN_REFERENCE);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bChooseAfFuncInReference = FALSE;
		}
		else{
			m_SrConfig.bChooseAfFuncInReference = IsDlgButtonChecked(IDC_CHOOSE_AF_FUNC_IN_REFERENCE);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option Auto Filter
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bAutoFilter = IsDlgButtonChecked(IDC_CHECK_AUTO_FILTER);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bAutoFilter = FALSE;
		}
		else{
			m_SrConfig.bAutoFilter = IsDlgButtonChecked(IDC_CHECK_AUTO_FILTER);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option Transmittance
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bTransmittance = IsDlgButtonChecked(IDC_CHECK_TRANSMITTANCE);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bTransmittance = FALSE;
		}
		else{
			m_SrConfig.bTransmittance = IsDlgButtonChecked(IDC_CHECK_TRANSMITTANCE);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option Built-in Reference
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bBuiltInReference = IsDlgButtonChecked(IDC_CHECK_BUILT_IN_REFERENCE);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bBuiltInReference = FALSE;
		}
		else{
			m_SrConfig.bBuiltInReference = IsDlgButtonChecked(IDC_CHECK_BUILT_IN_REFERENCE);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option CIE Measurement
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bCieMeasurement = IsDlgButtonChecked(IDC_CHECK_CIE_MEASUREMENT);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bCieMeasurement = FALSE;
		}
		else{
			m_SrConfig.bCieMeasurement = IsDlgButtonChecked(IDC_CHECK_CIE_MEASUREMENT);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option OD Measurement
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bOdMeasurement = IsDlgButtonChecked(IDC_CHECK_OD_MEASUREMENT);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bOdMeasurement = FALSE;
		}
		else{
			m_SrConfig.bOdMeasurement = IsDlgButtonChecked(IDC_CHECK_OD_MEASUREMENT);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option UV
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bUv = IsDlgButtonChecked(IDC_CHECK_UV);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bUv = FALSE;
		}
		else{
			m_SrConfig.bUv = IsDlgButtonChecked(IDC_CHECK_UV);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option Uv Auto Shutter
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bUvAutoShutter = IsDlgButtonChecked(IDC_CHECK_UV_AUTO_SHUTTER);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bUvAutoShutter = FALSE;
		}
		else{
			m_SrConfig.bUvAutoShutter = IsDlgButtonChecked(IDC_CHECK_UV_AUTO_SHUTTER);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Head Type /////
		m_SrConfig.nHeadType = ((CComboBox *) GetDlgItem(IDC_HEAD_TYPE))->GetCurSel();

		///// Head Interface /////
		m_SrConfig.nHeadInterface = ((CComboBox *) GetDlgItem(IDC_HEAD_INTERFACE))->GetCurSel();

		///// Option Turret /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.nTurret = ((CComboBox *) GetDlgItem(IDC_TURRET))->GetCurSel();
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.nTurret = 0;
		}
		else{
			m_SrConfig.nTurret = ((CComboBox *) GetDlgItem(IDC_TURRET))->GetCurSel();
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

// 2014.04.04 bagus wavelength step added -->
		///// Wavelength Step /////
		m_SrConfig.nWavelengthStep = ((CComboBox *) GetDlgItem(IDC_WAVELENGTH_STEP))->GetCurSel();
// 2014.04.04 bagus wavelength step added <--

		///// Head Default Integration Time /////
		GetDlgItemText(IDC_INTEGRATION_TIME, strBuffer);
		m_SrConfig.dIntegrationTime = atof((PCSTR) strBuffer);

		///// Head Default 2nd(OD) Reference Integration Time /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		GetDlgItemText(IDC_SECOND_INTEGRATION_TIME, strBuffer);
//		m_SrConfig.dSecondIntegrationTime = atof((PCSTR) strBuffer);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.dSecondIntegrationTime = 0.0;
		}
		else{
			GetDlgItemText(IDC_SECOND_INTEGRATION_TIME, strBuffer);
			m_SrConfig.dSecondIntegrationTime = atof((PCSTR) strBuffer);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option UV Gain /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		GetDlgItemText(IDC_UV_GAIN, strBuffer);
//		m_SrConfig.dUvGain = atof((PCSTR) strBuffer);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.dUvGain = 0.0;
		}
		else{
			GetDlgItemText(IDC_UV_GAIN, strBuffer);
			m_SrConfig.dUvGain = atof((PCSTR) strBuffer);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- { ---------- */
		/* AF ���ɑ���v���O�����̃t�B���^�ݒ���g�����ݒ�	*/
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bUseOptFilterMeasPgmAF = this->IsDlgButtonChecked(IDC_CHK_USEOPTFILTERAF);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bUseOptFilterMeasPgmAF = FALSE;
		}
		else{
			m_SrConfig.bUseOptFilterMeasPgmAF = this->IsDlgButtonChecked(IDC_CHK_USEOPTFILTERAF);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- } ---------- */
// 2009.09.19 bagus SE --{--
		///// DefaultMeasureTurretNo /////
		GetDlgItemText(IDC_DEFAULT_MEASUREMENT_TURRET_NO, strBuffer);
		m_SrConfig.iDefaultMeasLensNo = strtol(strBuffer, NULL, 10);
// 2009.09.29 bagus SE --{--
		m_SrConfig.iDefaultMeasLensNo -= 1;
// 2009.09.29 bagus SE --}--

		///// DefaultMeasureFilter /////
		GetDlgItemText(IDC_DEFAULT_MEASUREMENT_FILTER_NO, strBuffer);
		m_SrConfig.wDefaultMeasFilter = static_cast<long>(strtol(strBuffer, NULL, 10));

//2009.09.30 bagus SE --{--
// 2010.01.06 bagus Gantry --{--
//		if(m_SrConfig.bTransmittance){
// 2010.01.06 bagus Gantry --}--
			///// TrUseTurretNo /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//			GetDlgItemText(IDC_USE_TURRET_NO, strBuffer);
//			m_SrConfig.iTrUseLensNo = strtol(strBuffer, NULL, 10);
			if(g_lModelType == MODEL_T3100){
				m_SrConfig.iTrUseLensNo = 0;
			}
			else{
				GetDlgItemText(IDC_USE_TURRET_NO, strBuffer);
				m_SrConfig.iTrUseLensNo = strtol(strBuffer, NULL, 10);
// 2009.09.29 bagus SE --{--
				m_SrConfig.iTrUseLensNo -= 1;
// 2009.09.29 bagus SE --}--
			}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

			///// TrShutterMoveWaitTime /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//			GetDlgItemText(IDC_WAIT_TIME_FOR_SHUTTER_MOVING, strBuffer);
//			m_SrConfig.wTrShutterMoveWaitTime = (double)(atof((PCSTR) strBuffer)) * MILLI_SEC;
			if(g_lModelType == MODEL_T3100){
				m_SrConfig.wTrShutterMoveWaitTime = static_cast<long>(0.0);
			}
			else{
				GetDlgItemText(IDC_WAIT_TIME_FOR_SHUTTER_MOVING, strBuffer);
				m_SrConfig.wTrShutterMoveWaitTime = static_cast<long>((double)(atof((PCSTR) strBuffer)) * MILLI_SEC);
			}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

			///// GantryType /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//			m_SrConfig.bGantryType = IsDlgButtonChecked(IDC_CHECK_GANTRY_TYPE);
			if(g_lModelType == MODEL_T3100){
				m_SrConfig.bGantryType = FALSE;
			}
			else{
				m_SrConfig.bGantryType = IsDlgButtonChecked(IDC_CHECK_GANTRY_TYPE);
			}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

			///// GantryNumberOfMeasurePoint /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//			GetDlgItemText(IDC_NUMBER_OF_A_MEASUREMENT_POINT, strBuffer);
//			m_SrConfig.wGantryNumOfMeasPoint = strtol(strBuffer, NULL, 10);
			if(g_lModelType == MODEL_T3100){
				m_SrConfig.wGantryNumOfMeasPoint = 1;
			}
			else{
				GetDlgItemText(IDC_NUMBER_OF_A_MEASUREMENT_POINT, strBuffer);
				m_SrConfig.wGantryNumOfMeasPoint = static_cast<int>(strtol(strBuffer, NULL, 10));
			}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
// 2010.01.06 bagus Gantry --{--
//		}
// 2010.01.06 bagus Gantry --}--
//2009.09.30 bagus SE --}--
// 2009.09.19 bagus SE --}--
// 2009.10.13 bagus Distance �ǉ� --{--
		///// Distance /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		m_SrConfig.bDistance = IsDlgButtonChecked(IDC_CHECK_DISTANCE);
		if(g_lModelType == MODEL_T3100){
			m_SrConfig.bDistance = FALSE;
		}
		else{
			m_SrConfig.bDistance = IsDlgButtonChecked(IDC_CHECK_DISTANCE);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
// 2009.10.13 bagus Distance �ǉ� --}--
	}
	else{
		///// Option Auto Focus /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = m_SrConfig.bAutoFocus ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_AUTO_FOCUS, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bAutoFocus ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_AUTO_FOCUS, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

// 2009.09.19 bagus SE --{--
		///// Option Choose AF Function in Reference
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = m_SrConfig.bChooseAfFuncInReference ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHOOSE_AF_FUNC_IN_REFERENCE, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bChooseAfFuncInReference ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHOOSE_AF_FUNC_IN_REFERENCE, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
// 2009.09.19 bagus SE --}--

		///// Option Auto Filter /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = m_SrConfig.bAutoFilter ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_AUTO_FILTER, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bAutoFilter ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_AUTO_FILTER, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option Transmittance /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = m_SrConfig.bTransmittance ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_TRANSMITTANCE, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bTransmittance ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_TRANSMITTANCE, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option Built-in Reference /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = m_SrConfig.bBuiltInReference ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_BUILT_IN_REFERENCE, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bBuiltInReference ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_BUILT_IN_REFERENCE, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option CIE Measurement /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = m_SrConfig.bCieMeasurement ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_CIE_MEASUREMENT, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bCieMeasurement ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_CIE_MEASUREMENT, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option OD Measurement /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = m_SrConfig.bOdMeasurement ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_OD_MEASUREMENT, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bOdMeasurement ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_OD_MEASUREMENT, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option UV /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
//		uCheck = m_SrConfig.bUv ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_UV, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bUv ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_UV, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option Uv Auto Shutter /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = m_SrConfig.bUvAutoShutter ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_UV_AUTO_SHUTTER, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bUvAutoShutter ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_UV_AUTO_SHUTTER, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

	///// Head Type /////
	pCombo = (CComboBox *) GetDlgItem(IDC_HEAD_TYPE);
	pCombo->ResetContent();
	for ( i = 0; i < SR_HEAD_TYPE_MAX; i++ ) {
		pCombo->AddString(g_lpszSrHeadType[i]);
	}
	pCombo->SetCurSel(m_SrConfig.nHeadType);

		///// Head Interface /////
		pCombo = (CComboBox *) GetDlgItem(IDC_HEAD_INTERFACE);
		pCombo->ResetContent();
		for(i = 0; i < SR_HEAD_INTERFACE_MAX; i++){
			pCombo->AddString(g_lpszSrHeadInterface[i]);
		}
		pCombo->SetCurSel(m_SrConfig.nHeadInterface);

		///// Option Turret /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		pCombo = (CComboBox *) GetDlgItem(IDC_TURRET);
//		pCombo->ResetContent();
//		for ( i = 0; i < SR_TURRET_MAX; i++ ) {
//			pCombo->AddString(g_lpszSrTurret[i]);
//		}
//		pCombo->SetCurSel(m_SrConfig.nTurret);
		if(g_lModelType != MODEL_T3100){
			pCombo = (CComboBox *) GetDlgItem(IDC_TURRET);
			pCombo->ResetContent();
			for ( i = 0; i < SR_TURRET_MAX; i++ ) {
				pCombo->AddString(g_lpszSrTurret[i]);
			}
			pCombo->SetCurSel(m_SrConfig.nTurret);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// UV Gain�̗L�������̐ݒ� /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		GetDlgItem(IDC_UV_GAIN)->EnableWindow(m_SrConfig.bUv ? TRUE : FALSE);
		if(g_lModelType != MODEL_T3100){
			GetDlgItem(IDC_UV_GAIN)->EnableWindow(m_SrConfig.bUv ? TRUE : FALSE);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

// 2014.04.04 bagus wavelength step added -->
		///// Wavelength Step /////
		pCombo = (CComboBox *) GetDlgItem(IDC_WAVELENGTH_STEP);
		pCombo->ResetContent();
		for(i = 0; i < SR_WAVELENGTH_STEP_MAX; i++){
			pCombo->AddString(g_lpszSrWavelengthStep[i]);
		}
		pCombo->SetCurSel(m_SrConfig.nWavelengthStep);
// 2014.04.04 bagus wavelength step added <--

		///// Head Default Integration Time /////
		strBuffer.Format("%.1lf", m_SrConfig.dIntegrationTime);
		SetDlgItemText(IDC_INTEGRATION_TIME, strBuffer);

		///// Head Default 2nd(OD) Reference Integration Time /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		strBuffer.Format("%.1lf", m_SrConfig.dSecondIntegrationTime);
//		SetDlgItemText(IDC_SECOND_INTEGRATION_TIME, strBuffer);
		if(g_lModelType != MODEL_T3100){
			strBuffer.Format("%.1lf", m_SrConfig.dSecondIntegrationTime);
			SetDlgItemText(IDC_SECOND_INTEGRATION_TIME, strBuffer);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// Option UV Gain /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		strBuffer.Format("%.3lf", m_SrConfig.dUvGain);
//		SetDlgItemText(IDC_UV_GAIN, strBuffer);
		if(g_lModelType != MODEL_T3100){
			strBuffer.Format("%.3lf", m_SrConfig.dUvGain);
			SetDlgItemText(IDC_UV_GAIN, strBuffer);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- { ---------- */
		/* AF ���ɑ���v���O�����̃t�B���^�ݒ���g�����ݒ�	*/
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//		uCheck = (0 != m_SrConfig.bUseOptFilterMeasPgmAF)? BST_CHECKED : BST_UNCHECKED;
//		this->CheckDlgButton(IDC_CHK_USEOPTFILTERAF, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = (0 != m_SrConfig.bUseOptFilterMeasPgmAF)? BST_CHECKED : BST_UNCHECKED;
			this->CheckDlgButton(IDC_CHK_USEOPTFILTERAF, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- } ---------- */
// 2009.09.19 bagus SE --{--
		///// DefaultMeasureTurretNo /////
		pCombo = (CComboBox *) GetDlgItem(IDC_DEFAULT_MEASUREMENT_TURRET_NO);
// 2009.09.29 bagus SE --{--
//		strBuffer.Format("%d", m_SrConfig.iDefaultMeasLensNo);
		strBuffer.Format("%d", m_SrConfig.iDefaultMeasLensNo + 1);
// 2009.09.29 bagus SE --}--
		nIndex = pCombo->FindStringExact(-1, strBuffer);
		pCombo->SetCurSel(nIndex);

		///// DefaultMeasureFilter /////
		pCombo = (CComboBox *) GetDlgItem(IDC_DEFAULT_MEASUREMENT_FILTER_NO);
		strBuffer.Format("%d", m_SrConfig.wDefaultMeasFilter);
		nIndex = pCombo->FindStringExact(-1, strBuffer);
		pCombo->SetCurSel(nIndex);

		///// TrUseTurretNo /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
//		pCombo = (CComboBox *) GetDlgItem(IDC_USE_TURRET_NO);
//// 2009.09.29 bagus SE --{--
////		strBuffer.Format("%d", m_SrConfig.iTrUseLensNo);
//		strBuffer.Format("%d", m_SrConfig.iTrUseLensNo + 1);
//// 2009.09.29 bagus SE --}--
//		nIndex = pCombo->FindStringExact(-1, strBuffer);
//		pCombo->SetCurSel(nIndex);
		if(g_lModelType != MODEL_T3100){
			pCombo = (CComboBox *) GetDlgItem(IDC_USE_TURRET_NO);
			strBuffer.Format("%d", m_SrConfig.iTrUseLensNo + 1);
			nIndex = pCombo->FindStringExact(-1, strBuffer);
			pCombo->SetCurSel(nIndex);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// TrShutterMoveWaitTime /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
//		strBuffer.Format("%.3lf", (double)(m_SrConfig.wTrShutterMoveWaitTime) / MILLI_SEC);
//		SetDlgItemText(IDC_WAIT_TIME_FOR_SHUTTER_MOVING, strBuffer);
		if(g_lModelType != MODEL_T3100){
			strBuffer.Format("%.3lf", (double)(m_SrConfig.wTrShutterMoveWaitTime) / MILLI_SEC);
			SetDlgItemText(IDC_WAIT_TIME_FOR_SHUTTER_MOVING, strBuffer);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// GantryType /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
//		uCheck = (0 != m_SrConfig.bGantryType)? BST_CHECKED : BST_UNCHECKED;
//		this->CheckDlgButton(IDC_CHECK_GANTRY_TYPE, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = (0 != m_SrConfig.bGantryType)? BST_CHECKED : BST_UNCHECKED;
			this->CheckDlgButton(IDC_CHECK_GANTRY_TYPE, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

		///// GantryNumberOfMeasurePoint /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
//		strBuffer.Format("%d", m_SrConfig.wGantryNumOfMeasPoint);
//		SetDlgItemText(IDC_NUMBER_OF_A_MEASUREMENT_POINT, strBuffer);
		if(g_lModelType != MODEL_T3100){
			strBuffer.Format("%d", m_SrConfig.wGantryNumOfMeasPoint);
			SetDlgItemText(IDC_NUMBER_OF_A_MEASUREMENT_POINT, strBuffer);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
// 2009.09.19 bagus SE --}--
// 2009.10.13 bagus Distance �ǉ� --{--
		///// Distance /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
//		uCheck = m_SrConfig.bDistance ? BST_CHECKED : BST_UNCHECKED;
//		CheckDlgButton(IDC_CHECK_DISTANCE, uCheck);
		if(g_lModelType != MODEL_T3100){
			uCheck = m_SrConfig.bDistance ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(IDC_CHECK_DISTANCE, uCheck);
		}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
// 2009.10.13 bagus Distance �ǉ� --}--
	}
}

// =========================================================================
//
BOOL CSrCofigurationSettingDlg::CheckData()
{

	UpdateData(TRUE);

	CString strBuffer;

	///// Head Default Integration Time /////
	GetDlgItemText(IDC_INTEGRATION_TIME, strBuffer);
	if ( CheckMinMaxDouble(strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME) != CHECK_DATA_OK ) {
		GetDlgItem(IDC_INTEGRATION_TIME)->SetFocus();
		return FALSE;
	}

	///// Head Default 2nd(OD) Reference Integration Time /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//	GetDlgItemText(IDC_SECOND_INTEGRATION_TIME, strBuffer);
//	if ( CheckMinMaxDouble(strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME) != CHECK_DATA_OK ) {
//		GetDlgItem(IDC_SECOND_INTEGRATION_TIME)->SetFocus();
//		return FALSE;
//	}
	if(g_lModelType != MODEL_T3100){
		GetDlgItemText(IDC_SECOND_INTEGRATION_TIME, strBuffer);
		if ( CheckMinMaxDouble(strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME) != CHECK_DATA_OK ) {
			GetDlgItem(IDC_SECOND_INTEGRATION_TIME)->SetFocus();
			return FALSE;
		}
	}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

	///// Option UV Gain /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//	if ( m_SrConfig.bUv ) {
//		GetDlgItemText(IDC_UV_GAIN, strBuffer);
//		if ( CheckMinMaxDouble(strBuffer, MIN_UV_GAIN, MAX_UV_GAIN) != CHECK_DATA_OK ) {
//			GetDlgItem(IDC_UV_GAIN)->SetFocus();
//			return FALSE;
//		}
//	}
	if(g_lModelType != MODEL_T3100){
		if ( m_SrConfig.bUv ) {
			GetDlgItemText(IDC_UV_GAIN, strBuffer);
			if ( CheckMinMaxDouble(strBuffer, MIN_UV_GAIN, MAX_UV_GAIN) != CHECK_DATA_OK ) {
				GetDlgItem(IDC_UV_GAIN)->SetFocus();
				return FALSE;
			}
		}
	}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

// 2009.09.19 bagus SE --{--
	CComboBox* pCombo;
	int	nIndex;

	///// DefaultMeasureTurretNo /////
	pCombo = (CComboBox *) GetDlgItem(IDC_DEFAULT_MEASUREMENT_TURRET_NO);
	nIndex = pCombo->GetCurSel();
	if (nIndex == CB_ERR) {
		// ���I����NG
		pCombo->SetFocus();
		LoadStringML(IDS_SELECT_DEF_MEAS_TURRET_NO, strBuffer, "Please select default measurement turret no.");
		AfxMessageBox(strBuffer);
		return FALSE;
	}

	///// DefaultMeasureFilter /////
	pCombo = (CComboBox *) GetDlgItem(IDC_DEFAULT_MEASUREMENT_FILTER_NO);
	nIndex = pCombo->GetCurSel();
	if (nIndex == CB_ERR) {
		// ���I����NG
		pCombo->SetFocus();
		LoadStringML(IDS_SELECT_DEF_MEAS_FILTER_NO, strBuffer, "Please select default measurement filter no.");
		AfxMessageBox(strBuffer);
		return FALSE;
	}

	///// TrShutterMoveWaitTime /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//	if ( m_SrConfig.bTransmittance ) {
//		///// TrUseTurretNo /////
//		pCombo = (CComboBox *) GetDlgItem(IDC_USE_TURRET_NO);
//		nIndex = pCombo->GetCurSel();
//		if (nIndex == CB_ERR) {
//			// ���I����NG
//			pCombo->SetFocus();
//			LoadStringML(IDS_SELECT_USE_MEAS_TURRET_NO, strBuffer, "Please select default use turret no.");
//			AfxMessageBox(strBuffer);
//			return FALSE;
//		}
//
//		GetDlgItemText(IDC_WAIT_TIME_FOR_SHUTTER_MOVING, strBuffer);
//		//2009.09.30 bagus sr --{--
//		//�����_�ȉ�4���ȏ���͂���Ă�����G���[�Ƃ���
//		strBuffer.TrimLeft();
//		strBuffer.TrimRight();
//		int nPos = strBuffer.FindOneOf(_T("."));
//		if(nPos >= 0){
//			if(strBuffer.GetLength() > nPos + 4){
//				CString strMsg;
//				LoadStringML(IDS_TRANSMITTANCE_SHUTTER_MOVING_ERROR, strMsg, "Please input number less than 3 Digits for X.nnn");
//				AfxMessageBox(strMsg);
//				return FALSE;
//			}
//		}
//		//2009.09.30 bagus sr --}--
//		if ( CheckMinMaxDouble(strBuffer, MIN_WAITTIME_SHUTTER_MOVING, MAX_WAITTIME_SHUTTER_MOVING) != CHECK_DATA_OK ) {
//			GetDlgItem(IDC_WAIT_TIME_FOR_SHUTTER_MOVING)->SetFocus();
//			return FALSE;
//		}
//	}
	if(g_lModelType != MODEL_T3100){
		if ( m_SrConfig.bTransmittance ) {
			///// TrUseTurretNo /////
			pCombo = (CComboBox *) GetDlgItem(IDC_USE_TURRET_NO);
			nIndex = pCombo->GetCurSel();
			if (nIndex == CB_ERR) {
				// ���I����NG
				pCombo->SetFocus();
				LoadStringML(IDS_SELECT_USE_MEAS_TURRET_NO, strBuffer, "Please select default use turret no.");
				AfxMessageBox(strBuffer);
				return FALSE;
			}

			GetDlgItemText(IDC_WAIT_TIME_FOR_SHUTTER_MOVING, strBuffer);
			//2009.09.30 bagus sr --{--
			//�����_�ȉ�4���ȏ���͂���Ă�����G���[�Ƃ���
			strBuffer.TrimLeft();
			strBuffer.TrimRight();
			int nPos = strBuffer.FindOneOf(_T("."));
			if(nPos >= 0){
				if(strBuffer.GetLength() > nPos + 4){
					CString strMsg;
					LoadStringML(IDS_TRANSMITTANCE_SHUTTER_MOVING_ERROR, strMsg, "Please input number less than 3 Digits for X.nnn");
					AfxMessageBox(strMsg);
					return FALSE;
				}
			}
			//2009.09.30 bagus sr --}--
			if ( CheckMinMaxDouble(strBuffer, MIN_WAITTIME_SHUTTER_MOVING, MAX_WAITTIME_SHUTTER_MOVING) != CHECK_DATA_OK ) {
				GetDlgItem(IDC_WAIT_TIME_FOR_SHUTTER_MOVING)->SetFocus();
				return FALSE;
			}
		}
	}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--

	///// GantryNumberOfMeasurePoint /////
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) -->
//	if ( m_SrConfig.bTransmittance && m_SrConfig.bGantryType ) {
//		GetDlgItemText(IDC_NUMBER_OF_A_MEASUREMENT_POINT, strBuffer);
//		//2009.12.07 bagus GTR �C�� --{--
//		//if ( CheckMinMaxDouble(strBuffer, MIN_GANTRY_MEAS_POINT_NUM, MAX_GANTRY_MEAS_POINT_NUM) != CHECK_DATA_OK ) {
//		if ( CheckMinMaxLong(strBuffer, MIN_GANTRY_MEAS_POINT_NUM, MAX_GANTRY_MEAS_POINT_NUM) != CHECK_DATA_OK ) {
//		//2009.12.07 bagus GTR �C�� --}--
//			GetDlgItem(IDC_NUMBER_OF_A_MEASUREMENT_POINT)->SetFocus();
//			return FALSE;
//		}
//	}
	if(g_lModelType != MODEL_T3100){
		if ( m_SrConfig.bTransmittance && m_SrConfig.bGantryType ) {
			GetDlgItemText(IDC_NUMBER_OF_A_MEASUREMENT_POINT, strBuffer);
			//2009.12.07 bagus GTR �C�� --{--
			//if ( CheckMinMaxDouble(strBuffer, MIN_GANTRY_MEAS_POINT_NUM, MAX_GANTRY_MEAS_POINT_NUM) != CHECK_DATA_OK ) {
			if ( CheckMinMaxLong(strBuffer, MIN_GANTRY_MEAS_POINT_NUM, MAX_GANTRY_MEAS_POINT_NUM) != CHECK_DATA_OK ) {
			//2009.12.07 bagus GTR �C�� --}--
				GetDlgItem(IDC_NUMBER_OF_A_MEASUREMENT_POINT)->SetFocus();
				return FALSE;
			}
		}
	}
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
// 2009.09.19 bagus SE --}--

	return TRUE;
}

void CSrCofigurationSettingDlg::OnCheckOdMeasurement()
{
	// UV Gain�̃G�f�B�b�g�{�b�N�X�̗L�������̐؂�ւ�
	GetDlgItem(IDC_SECOND_INTEGRATION_TIME)->EnableWindow(IsDlgButtonChecked(IDC_CHECK_OD_MEASUREMENT) == BST_CHECKED);
}

/*
void CSrCofigurationSettingDlg::OnReferenceDataFolderPathButton()
{
	//�n���h���擾�Q�l�T���v��
	//CWnd* pWnd;
	//pWnd = GetDlgItem(IDD_SR_CONFIGURATION_SETTING_DLG);

	��1
	HWND hWnd = this->GetSafeHwnd();
	HWND hWndButton1 = ::GetDlgItem(hWnd, IDC_BUTTON1);
	::ShowWindow(hWndButton1, SW_HIDE);

	��2
	CWnd* pButton1 = this->GetDlgItem(IDC_BUTTON1);
	HWND hWndButton1 = pButton1->GetSafeHwnd();
	::ShowWindow(hWndButton1, SW_HIDE);


	// ���b�Z�[�W�{�b�N�X�̃I�[�i�[�ɂ������B���h�E�n���h�����w�肷��
	HWND hWnd = this->GetSafeHwnd();

	BROWSEINFO		bi;
	LPSTR			lpBuffer;
	LPITEMIDLIST	pidlRoot;	   // �u���E�Y�̃��[�gPIDL
	LPITEMIDLIST	pidlBrowse;    // ���[�U�[���I������PIDL
	LPMALLOC		lpMalloc = NULL;

	HRESULT hr = SHGetMalloc(&lpMalloc);
	if(FAILED(hr)) return;

	// �u���E�Y���󂯎��o�b�t�@�̈�̊m��
	if ((lpBuffer = (LPSTR) lpMalloc->Alloc(_MAX_PATH)) == NULL) {
		return;
	}
	// �_�C�A���O�\�����̃��[�g�t�H���_��PIDL���擾
	// ���ȉ��̓f�X�N�g�b�v�����[�g�Ƃ��Ă���B�f�X�N�g�b�v�����[�g�Ƃ���
	//	 �ꍇ�́A�P�� bi.pidlRoot �ɂO��ݒ肷�邾���ł��悢�B���̑��̓�
	//	 ��t�H���_�����[�g�Ƃ��鎖���ł���B�ڍׂ�SHGetSpecialFolderLoca
	//	 tion�̃w���v���Q�Ƃ̎��B
	if (!SUCCEEDED(SHGetSpecialFolderLocation(	hWnd, CSIDL_DESKTOP, &pidlRoot))) {
		lpMalloc->Free(lpBuffer);
		return;
	}
	// BROWSEINFO�\���̂̏����l�ݒ�
	// ��BROWSEINFO�\���̂̊e�����o�̏ڍא������w���v���Q��
	bi.hwndOwner = hWnd;
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = lpBuffer;
	bi.lpszTitle = "Please select the reference data folder.";
	bi.ulFlags = 0;
	bi.lpfn = 0;
	bi.lParam = 0;
	// �t�H���_�I���_�C�A���O�̕\��
	pidlBrowse = SHBrowseForFolder(&bi);
	if (pidlBrowse != NULL) {
		// PIDL�`���̖߂�l�̃t�@�C���V�X�e���̃p�X�ɕϊ�
		if (SHGetPathFromIDList(pidlBrowse, lpBuffer)) {
			// �擾����
			SetDlgItemText(IDC_REFERENCE_DATA_FOLDER_PATH,lpBuffer);
		}
		// SHBrowseForFolder�̖߂�lPIDL�����
		lpMalloc->Free(pidlBrowse);
	}
	// �N���[���A�b�v����
	lpMalloc->Free(pidlRoot);
	lpMalloc->Free(lpBuffer);
	lpMalloc->Release();
}
*/

void CSrCofigurationSettingDlg::OnCheckAutoFocus()
{
	// Choose AF Function in Reference�̃`�F�b�N�{�b�N�X�̗L�������̐؂�ւ�
	GetDlgItem(IDC_CHOOSE_AF_FUNC_IN_REFERENCE)->EnableWindow(IsDlgButtonChecked(IDC_CHECK_AUTO_FOCUS) == BST_CHECKED);
}

/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- { ---------- */
void CSrCofigurationSettingDlg::OnCheckAutoFilter()
{
	/* Use Optical Filter of Meas.-PGM at AF �̃`�F�b�N�{�b�N�X�̗L�������̐؂�ւ�	*/
	this->GetDlgItem(IDC_CHK_USEOPTFILTERAF)->EnableWindow(BST_CHECKED == this->IsDlgButtonChecked(IDC_CHECK_AUTO_FILTER));
	if (0 == this->IsDlgButtonChecked(IDC_CHECK_AUTO_FILTER)) {
		this->CheckDlgButton(IDC_CHK_USEOPTFILTERAF, 0);
	}
}
/* added 2009.07.03 hmenjo Seq ���莞�t�B���^�Œ�@�\ ���� ---------- } ---------- */

// 2009.09.18 bagus SE --{--
void CSrCofigurationSettingDlg::OnCheckTransmittance()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	GetDlgItem(IDC_USE_TURRET_NO)->EnableWindow(IsDlgButtonChecked(IDC_CHECK_TRANSMITTANCE) == BST_CHECKED);
	GetDlgItem(IDC_WAIT_TIME_FOR_SHUTTER_MOVING)->EnableWindow(IsDlgButtonChecked(IDC_CHECK_TRANSMITTANCE) == BST_CHECKED);
	GetDlgItem(IDC_CHECK_GANTRY_TYPE)->EnableWindow(IsDlgButtonChecked(IDC_CHECK_TRANSMITTANCE) == BST_CHECKED);
	GetDlgItem(IDC_NUMBER_OF_A_MEASUREMENT_POINT)->EnableWindow((IsDlgButtonChecked(IDC_CHECK_GANTRY_TYPE) == BST_CHECKED) && (IsDlgButtonChecked(IDC_CHECK_TRANSMITTANCE) == BST_CHECKED));
}

void CSrCofigurationSettingDlg::OnCheckGantryType()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	GetDlgItem(IDC_NUMBER_OF_A_MEASUREMENT_POINT)->EnableWindow((IsDlgButtonChecked(IDC_CHECK_GANTRY_TYPE) == BST_CHECKED) && (IsDlgButtonChecked(IDC_CHECK_TRANSMITTANCE) == BST_CHECKED));
}
// 2009.09.18 bagus SE --}--
