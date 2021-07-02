// SrVideoCalibrationSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <math.h>
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "VideoCalibDlg.h"
#include "SrVideoCalibrationSettingDlg.h"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSrVideoCalibrationSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSrVideoCalibrationSettingDlg::CSrVideoCalibrationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrVideoCalibrationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrVideoCalibrationSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrVideoCalibrationSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LENS1_X, m_stcLens1_X);
	DDX_Control(pDX, IDC_LENS1_Y, m_stcLens1_Y);
	DDX_Control(pDX, IDC_LENS2_X, m_stcLens2_X);
	DDX_Control(pDX, IDC_LENS2_Y, m_stcLens2_Y);
	DDX_Control(pDX, IDC_LENS3_X, m_stcLens3_X);
	DDX_Control(pDX, IDC_LENS3_Y, m_stcLens3_Y);
	DDX_Control(pDX, IDC_LENS4_X, m_stcLens4_X);
	DDX_Control(pDX, IDC_LENS4_Y, m_stcLens4_Y);
	DDX_Control(pDX, IDC_LENS1_SETUP_BUTTON, m_Lens1SetupButton);
	DDX_Control(pDX, IDC_LENS2_SETUP_BUTTON, m_Lens2SetupButton);
	DDX_Control(pDX, IDC_LENS3_SETUP_BUTTON, m_Lens3SetupButton);
	DDX_Control(pDX, IDC_LENS4_SETUP_BUTTON, m_Lens4SetupButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_CustomLensNameText(pDX, IDC_LENS1_NAME, IDC_LENS1_SETUP_BUTTON, m_SrTurret[0].bEnable, m_SrTurret[0].szName);
	DDX_CustomLensNameText(pDX, IDC_LENS2_NAME, IDC_LENS2_SETUP_BUTTON, m_SrTurret[1].bEnable, m_SrTurret[1].szName);
	DDX_CustomLensNameText(pDX, IDC_LENS3_NAME, IDC_LENS3_SETUP_BUTTON, m_SrTurret[2].bEnable, m_SrTurret[2].szName);
	DDX_CustomLensNameText(pDX, IDC_LENS4_NAME, IDC_LENS4_SETUP_BUTTON, m_SrTurret[3].bEnable, m_SrTurret[3].szName);
	DDX_CustomLensXyText(pDX, IDC_LENS1_X, m_SrVideoCalib[0].dXPixelToMicron);
	DDX_CustomLensXyText(pDX, IDC_LENS1_Y, m_SrVideoCalib[0].dYPixelToMicron);
	DDX_CustomLensXyText(pDX, IDC_LENS2_X, m_SrVideoCalib[1].dXPixelToMicron);
	DDX_CustomLensXyText(pDX, IDC_LENS2_Y, m_SrVideoCalib[1].dYPixelToMicron);
	DDX_CustomLensXyText(pDX, IDC_LENS3_X, m_SrVideoCalib[2].dXPixelToMicron);
	DDX_CustomLensXyText(pDX, IDC_LENS3_Y, m_SrVideoCalib[2].dYPixelToMicron);
	DDX_CustomLensXyText(pDX, IDC_LENS4_X, m_SrVideoCalib[3].dXPixelToMicron);
	DDX_CustomLensXyText(pDX, IDC_LENS4_Y, m_SrVideoCalib[3].dYPixelToMicron);
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::DDX_CustomLensNameText(CDataExchange* pDX, int nIdcName, int nIdcSetupButton, BOOL bTurretEnable, LPCTSTR pszTurretName)
{
	if ( !pDX->m_bSaveAndValidate ) {
		if ( bTurretEnable ) {
			GetDlgItem(nIdcName)->SetWindowText(pszTurretName);
			GetDlgItem(nIdcSetupButton)->EnableWindow();
		}
		else {
			GetDlgItem(nIdcName)->SetWindowText("Unused");
			GetDlgItem(nIdcSetupButton)->EnableWindow(FALSE);
		}
	}
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::DDX_CustomLensXyText(CDataExchange* pDX, int nIDC, double value)
{
	char szBuff[256];

	if ( !pDX->m_bSaveAndValidate ) {
		double temp = CVideoCalibDlg::Convert_RoundForDisplay(value, VIDEO_CALIB_FACTOR_DISP_DIGITS);
		sprintf(szBuff, "%G", temp);
		GetDlgItem(nIDC)->SetWindowText(szBuff);
	}
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrVideoCalibrationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrVideoCalibrationSettingDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_LENS1_SETUP_BUTTON, OnLens1SetupButton)
	ON_BN_CLICKED(IDC_LENS2_SETUP_BUTTON, OnLens2SetupButton)
	ON_BN_CLICKED(IDC_LENS3_SETUP_BUTTON, OnLens3SetupButton)
	ON_BN_CLICKED(IDC_LENS4_SETUP_BUTTON, OnLens4SetupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSrVideoCalibrationSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSrVideoCalibrationSettingDlg::OnInitDialog()
{
	///// Load ini File /////
	ConfigFile_GetNanoSpecIni(m_SrVideoCalib, CONFIG_FILE_SR_VIDEO_CALIBRATION);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);

	CDialog::OnInitDialog();

	///// Static Control /////
	m_stcLens1_X.SetBkColor(REFERENCE_COLOR);
	m_stcLens1_Y.SetBkColor(REFERENCE_COLOR);
	m_stcLens2_X.SetBkColor(REFERENCE_COLOR);
	m_stcLens2_Y.SetBkColor(REFERENCE_COLOR);
	m_stcLens3_X.SetBkColor(REFERENCE_COLOR);
	m_stcLens3_Y.SetBkColor(REFERENCE_COLOR);
	m_stcLens4_X.SetBkColor(REFERENCE_COLOR);
	m_stcLens4_Y.SetBkColor(REFERENCE_COLOR);

	m_Lens1SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Lens1SetupButton.DrawFlatFocus(TRUE);
	m_Lens2SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Lens2SetupButton.DrawFlatFocus(TRUE);
	m_Lens3SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Lens3SetupButton.DrawFlatFocus(TRUE);
	m_Lens4SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Lens4SetupButton.DrawFlatFocus(TRUE);
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::OnOK()
{
	ConfigFile_SetNanoSpecIni(m_SrVideoCalib, CONFIG_FILE_SR_VIDEO_CALIBRATION);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::OnLens1SetupButton()
{
	LensSetupButton(IDC_LENS1_SETUP_BUTTON);
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::OnLens2SetupButton()
{
	LensSetupButton(IDC_LENS2_SETUP_BUTTON);
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::OnLens3SetupButton()
{
	LensSetupButton(IDC_LENS3_SETUP_BUTTON);
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::OnLens4SetupButton()
{
	LensSetupButton(IDC_LENS4_SETUP_BUTTON);
}

// =========================================================================
//
void CSrVideoCalibrationSettingDlg::LensSetupButton(UINT nID)
{
	// LensのSetupボタンのID
	UINT Video_LensSetupButton_IDs[] = {
		IDC_LENS1_SETUP_BUTTON,
		IDC_LENS2_SETUP_BUTTON,
		IDC_LENS3_SETUP_BUTTON,
		IDC_LENS4_SETUP_BUTTON,
	};

	int nMaxCnt = sizeof(Video_LensSetupButton_IDs) / sizeof(Video_LensSetupButton_IDs[0]);
	for ( UINT nIndex = 0; nIndex < nMaxCnt; nIndex++ )
		if ( nID == Video_LensSetupButton_IDs[nIndex] )
			break;

	ASSERT( nIndex < nMaxCnt );

	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(nIndex);
	pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	CVideoCalibDlg dlg;
	dlg.SetXPixelToMicron(m_SrVideoCalib[nIndex].dXPixelToMicron);
	dlg.SetYPixelToMicron(m_SrVideoCalib[nIndex].dYPixelToMicron);

	if ( dlg.DoModal() == IDOK ) {
		m_SrVideoCalib[nIndex].dXPixelToMicron = dlg.GetXPixelToMicron();
		m_SrVideoCalib[nIndex].dYPixelToMicron = dlg.GetYPixelToMicron();
	}

	UpdateData(FALSE);
}


