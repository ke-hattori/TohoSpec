// SeFigureFormatSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SeFigureFormatSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// SeFigureFormatSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
SeFigureFormatSettingDlg::SeFigureFormatSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SeFigureFormatSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SeFigureFormatSettingDlg)
	//}}AFX_DATA_INIT
	m_iHeadType = HEAD_TYPE_SE;
}

// =========================================================================
//
void SeFigureFormatSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SeFigureFormatSettingDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(SeFigureFormatSettingDlg, CDialog)
	//{{AFX_MSG_MAP(SeFigureFormatSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// SeFigureFormatSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL SeFigureFormatSettingDlg::OnInitDialog()
{
	//未対応機能disable
	GetDlgItem(IDC_THICKNESS_FORMAT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_REFLECTANCE_FORMAT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMITTANCE_FORMAT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHROMATICITY_FORMAT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OPTICAL_DENSITY_FORMAT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FORMAT_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_REFLECTANCE_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_REFLECTANCE2_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMITTANCE_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMITTANCE2_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHROMATICITY_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_OPTICALDENSITY_LABEL)->ShowWindow(SW_HIDE);

	///// 動的サイズ・配置変更 /////
	GetDlgItem(IDC_UNIT_LABEL)->SetWindowPos(&wndTop,150,10,0,0,SWP_NOSIZE);
	GetDlgItem(IDC_THICKNESS_UNIT)->SetWindowPos(&wndTop,150,30,0,0,SWP_NOSIZE);
	GetDlgItem(IDOK)->SetWindowPos(&wndTop,15,65,0,0,SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop,115,65,0,0,SWP_NOSIZE);
	SeFigureFormatSettingDlg::SetWindowPos(&wndTop,0,0,230,125,SWP_NOMOVE);

	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// SetData /////
// 2009.09.19 bagus SE --{--
//	ConfigFile_GetNanoSpecIni(&m_SeFigureFormat, CONFIG_FILE_SR_FIGURE_FORMAT);

	CString l_strBuffer;

	switch (m_iHeadType) {
	case HEAD_TYPE_SE:
	default:
		LoadStringML(IDS_TITLE_SE_FIGURE_FORMAT_SETTING, l_strBuffer, "SE FIGURE FORMAT SETTING");
		m_strCaption = l_strBuffer;
	ConfigFile_GetNanoSpecIni(&m_SeFigureFormat, CONFIG_FILE_SE_FIGURE_FORMAT);
		break;
	case HEAD_TYPE_COMPEASE:
		LoadStringML(IDS_TITLE_COMPEASE_FIGURE_FORMAT_SETTING, l_strBuffer, "EASE FIGURE FORMAT SETTING");
		m_strCaption = l_strBuffer;
		ConfigFile_GetNanoSpecIni(&m_SeFigureFormat, CONFIG_FILE_COMPEASE_FIGURE_FORMAT);
		break;
	}

	this->SetWindowText(m_strCaption);
// 2009.09.19 bagus SE --}--
	UpDate(FALSE);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void SeFigureFormatSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);
	if ( !UpdateData() )
		return;

// 2009.09.19 bagus SE --{--
//	ConfigFile_SetNanoSpecIni(&m_SeFigureFormat, CONFIG_FILE_SR_FIGURE_FORMAT);
//	ConfigFile_SaveNanoSpecIni(USER_SETTING_SR_INI_FILE);
	switch (m_iHeadType) {
	case HEAD_TYPE_SE:
	default:
	ConfigFile_SetNanoSpecIni(&m_SeFigureFormat, CONFIG_FILE_SE_FIGURE_FORMAT);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_SE_INI_FILE);
		break;
	case HEAD_TYPE_COMPEASE:
		ConfigFile_SetNanoSpecIni(&m_SeFigureFormat, CONFIG_FILE_COMPEASE_FIGURE_FORMAT);
		ConfigFile_SaveNanoSpecIni(USER_SETTING_COMPEASE_INI_FILE);
		break;
	}
// 2009.09.19 bagus SE --}--

	CDialog::OnOK();
}

// =========================================================================
//
void SeFigureFormatSettingDlg::UpDate(BOOL bValid)
{
	int i;
	CComboBox* pCombo;

	if(bValid){
		// Thickness Format
		pCombo = (CComboBox *)GetDlgItem(IDC_THICKNESS_FORMAT);
		m_SeFigureFormat.nThicknessFormat = pCombo->GetCurSel();

		// Thickness Unit
		pCombo = (CComboBox *)GetDlgItem(IDC_THICKNESS_UNIT);
		m_SeFigureFormat.nThicknessUnit = pCombo->GetCurSel();

		// Reflectance Format
		pCombo = (CComboBox *)GetDlgItem(IDC_REFLECTANCE_FORMAT);
		m_SeFigureFormat.nReflectanceFormat = pCombo->GetCurSel();

		// Transmittance Format
		pCombo = (CComboBox *)GetDlgItem(IDC_TRANSMITTANCE_FORMAT);
		m_SeFigureFormat.nTransmittanceFormat = pCombo->GetCurSel();

		// Chromaticity Format
		pCombo = (CComboBox *)GetDlgItem(IDC_CHROMATICITY_FORMAT);
		m_SeFigureFormat.nChromaticityFormat = pCombo->GetCurSel();

		// Optical Density Format
		pCombo = (CComboBox *)GetDlgItem(IDC_OPTICAL_DENSITY_FORMAT);
		m_SeFigureFormat.nOpticalDensityFormat = pCombo->GetCurSel();
	}
	else{

		// Thickness Format
		pCombo = (CComboBox *)GetDlgItem(IDC_THICKNESS_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SeFigureFormat.nThicknessFormat);

		// Thickness Unit
		pCombo = (CComboBox *)GetDlgItem(IDC_THICKNESS_UNIT);
		pCombo->ResetContent();
// 2009.09.19 bagus SE --{--
//		for(i = 0; i < FIGURE_UNIT_SR_THICKNESS_MAX; i++){
		for(i = 0; i < FIGURE_UNIT_SE_THICKNESS_MAX; i++){
// 2009.09.19 bagus SE --}--
			pCombo->AddString(g_lpszFigureUnitSrThickness[i]);
		}
		pCombo->SetCurSel(m_SeFigureFormat.nThicknessUnit);

		// Reflectance Format
		pCombo = (CComboBox *)GetDlgItem(IDC_REFLECTANCE_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SeFigureFormat.nReflectanceFormat);

		// Transmittance Format
		pCombo = (CComboBox *)GetDlgItem(IDC_TRANSMITTANCE_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SeFigureFormat.nTransmittanceFormat);

		// Chromaticity Format
		pCombo = (CComboBox *)GetDlgItem(IDC_CHROMATICITY_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SeFigureFormat.nChromaticityFormat);

		// Optical Density Format
		pCombo = (CComboBox *)GetDlgItem(IDC_OPTICAL_DENSITY_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SeFigureFormat.nOpticalDensityFormat);
	}
}
