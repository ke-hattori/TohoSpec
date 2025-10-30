// SrFigureFormatSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SrFigureFormatSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// SrFigureFormatSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
SrFigureFormatSettingDlg::SrFigureFormatSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SrFigureFormatSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SrFigureFormatSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void SrFigureFormatSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SrFigureFormatSettingDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(SrFigureFormatSettingDlg, CDialog)
	//{{AFX_MSG_MAP(SrFigureFormatSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// SrFigureFormatSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL SrFigureFormatSettingDlg::OnInitDialog()
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
/* modified 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
//	GetDlgItem(IDOK)->SetWindowPos(&wndTop,15,65,0,0,SWP_NOSIZE);
//	GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop,115,65,0,0,SWP_NOSIZE);
//	SrFigureFormatSettingDlg::SetWindowPos(&wndTop,0,0,230,125,SWP_NOMOVE);
/* modified 2017.04.15 hmenjo 整数表示化 ----------              */
	GetDlgItem(IDOK)->SetWindowPos(&wndTop, 15, 95, 0, 0, SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop, 115, 95, 0, 0, SWP_NOSIZE);
	SrFigureFormatSettingDlg::SetWindowPos(&wndTop, 0, 0, 230, 155, SWP_NOMOVE);
/* modified 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */
/* added 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
	/* 「整数部のみ」の表示位置変更	*/
	RECT l_rectReflectanceLabel;
	this->GetDlgItem(IDC_REFLECTANCE_LABEL)->GetWindowRect(&l_rectReflectanceLabel);
	this->ScreenToClient(&l_rectReflectanceLabel);
	this->GetDlgItem(IDC_CHK_INT_ONLY)->SetWindowPos(&wndTop,
												l_rectReflectanceLabel.left,
												l_rectReflectanceLabel.top,
												0, 0, SWP_NOSIZE);
/* added 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */

	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// SetData /////
	ConfigFile_GetNanoSpecIni(&m_SrFigureFormat, CONFIG_FILE_SR_FIGURE_FORMAT);
	UpDate(FALSE);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void SrFigureFormatSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);
	if ( !UpdateData() )
		return;

	ConfigFile_SetNanoSpecIni(&m_SrFigureFormat, CONFIG_FILE_SR_FIGURE_FORMAT);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_SR_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void SrFigureFormatSettingDlg::UpDate(BOOL bValid)
{
	int i;
	CComboBox* pCombo;

	if(bValid){
		// Thickness Format
		pCombo = (CComboBox *)GetDlgItem(IDC_THICKNESS_FORMAT);
		m_SrFigureFormat.nThicknessFormat = pCombo->GetCurSel();

		// Thickness Unit
		pCombo = (CComboBox *)GetDlgItem(IDC_THICKNESS_UNIT);
		m_SrFigureFormat.nThicknessUnit = pCombo->GetCurSel();

		// Reflectance Format
		pCombo = (CComboBox *)GetDlgItem(IDC_REFLECTANCE_FORMAT);
		m_SrFigureFormat.nReflectanceFormat = pCombo->GetCurSel();

		// Transmittance Format
		pCombo = (CComboBox *)GetDlgItem(IDC_TRANSMITTANCE_FORMAT);
		m_SrFigureFormat.nTransmittanceFormat = pCombo->GetCurSel();

		// Chromaticity Format
		pCombo = (CComboBox *)GetDlgItem(IDC_CHROMATICITY_FORMAT);
		m_SrFigureFormat.nChromaticityFormat = pCombo->GetCurSel();

		// Optical Density Format
		pCombo = (CComboBox *)GetDlgItem(IDC_OPTICAL_DENSITY_FORMAT);
		m_SrFigureFormat.nOpticalDensityFormat = pCombo->GetCurSel();
/* added 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
		/* Thickness Integer Only	*/
		if (0 == ((CButton*) this->GetDlgItem(IDC_CHK_INT_ONLY))->GetCheck()) {
			m_SrFigureFormat.iThicknessIntOnly = 0;
		} else {
			m_SrFigureFormat.iThicknessIntOnly = 1;
		}
/* added 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */
	}
	else{

		// Thickness Format
		pCombo = (CComboBox *)GetDlgItem(IDC_THICKNESS_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SrFigureFormat.nThicknessFormat);

		// Thickness Unit
		pCombo = (CComboBox *)GetDlgItem(IDC_THICKNESS_UNIT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_UNIT_SR_THICKNESS_MAX; i++){
			pCombo->AddString(g_lpszFigureUnitSrThickness[i]);
		}
		pCombo->SetCurSel(m_SrFigureFormat.nThicknessUnit);

		// Reflectance Format
		pCombo = (CComboBox *)GetDlgItem(IDC_REFLECTANCE_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SrFigureFormat.nReflectanceFormat);

		// Transmittance Format
		pCombo = (CComboBox *)GetDlgItem(IDC_TRANSMITTANCE_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SrFigureFormat.nTransmittanceFormat);

		// Chromaticity Format
		pCombo = (CComboBox *)GetDlgItem(IDC_CHROMATICITY_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SrFigureFormat.nChromaticityFormat);

		// Optical Density Format
		pCombo = (CComboBox *)GetDlgItem(IDC_OPTICAL_DENSITY_FORMAT);
		pCombo->ResetContent();
		for(i = 0; i < FIGURE_FORMAT_MAX; i++){
			pCombo->AddString(g_lpszFigureFormat[i]);
		}
		pCombo->SetCurSel(m_SrFigureFormat.nOpticalDensityFormat);
/* added 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
		/* Thickness Integer Only	*/
		if (0 == m_SrFigureFormat.iThicknessIntOnly) {
			((CButton*) this->GetDlgItem(IDC_CHK_INT_ONLY))->SetCheck(0);
		} else {
			((CButton*) this->GetDlgItem(IDC_CHK_INT_ONLY))->SetCheck(1);
		}
/* added 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */
	}
}
