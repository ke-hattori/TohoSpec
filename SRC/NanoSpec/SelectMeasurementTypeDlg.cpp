// SelectMeasurementTypeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "RecipeMeasurementProgramListView.h"
#include "MultiLangHelper.h"
#include "SelectMeasurementTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSelectMeasurementTypeDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSelectMeasurementTypeDlg::CSelectMeasurementTypeDlg(BOOL bRecipeDefaultValueSetting /* = FALSE */,
	int iDefaultValueSettingHeadType /* = HEAD_TYPE_SR */, CWnd* pParent /*=NULL*/)
	: CDialog(CSelectMeasurementTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectMeasurementTypeDlg)
	//}}AFX_DATA_INIT
	m_iHeadType = HEAD_TYPE_SR;
	m_iMeasType = MEAS_PROG_TYPE_SR_THICKNESS;
	m_bRecipeDefaultValueSetting = bRecipeDefaultValueSetting;
	m_iDefaultValueSettingHeadType = iDefaultValueSettingHeadType;

// 2009.10.29 bagus 2点間 修正 --{--
	m_bRecalibration = FALSE;
// 2009.10.29 bagus 2点間 修正 --}--
}

// =========================================================================
//
void CSelectMeasurementTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectMeasurementTypeDlg)
	DDX_Control(pDX, IDC_MEASUREMENT_TYPE_LIST, m_lbMeasTypeList);
	DDX_Control(pDX, IDC_HEAD_TYPE, m_cbHeadType);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSelectMeasurementTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectMeasurementTypeDlg)
	ON_LBN_DBLCLK(IDC_MEASUREMENT_TYPE_LIST, OnDblclkMeasurementTypeList)
	ON_CBN_SELCHANGE(IDC_HEAD_TYPE, OnSelchangeHeadType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSelectMeasurementTypeDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSelectMeasurementTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	ConfigFile_GetNanoSpecIni(&m_HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_DefaultHeadType, CONFIG_FILE_DEFAULT_HEAD_TYPE);
	m_iHeadType = m_DefaultHeadType.nHeadType;
	m_iMeasType = m_DefaultHeadType.nMeasType;

	m_cbHeadType.ResetContent();

// 2009.10.29 bagus 2点間 修正 --{--
	int	iMinHeadType = HEAD_TYPE_SR;
// 2009.10.29 bagus 2点間 修正 --}--

	// Configで設定されているヘッドのみ表示
	int nIndex;
	if ( m_HeadType.bSR ) {
		nIndex = m_cbHeadType.AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SR]);
		m_cbHeadType.SetItemData(nIndex, HEAD_TYPE_SR);
	}
// 2009.10.29 bagus 2点間 修正 --{--
	else {
		iMinHeadType = HEAD_TYPE_SE;
	}
// 2009.10.29 bagus 2点間 修正 --}--
	if ( m_HeadType.bSE ) {
		// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if ( m_HeadType.bCompEASE ) {
			nIndex = m_cbHeadType.AddString(HEAD_TYPE_ITEM[HEAD_TYPE_COMPEASE]);
			m_cbHeadType.SetItemData(nIndex, HEAD_TYPE_COMPEASE);
		}
		else {
		nIndex = m_cbHeadType.AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SE]);
		m_cbHeadType.SetItemData(nIndex, HEAD_TYPE_SE);
	}
		// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	}
// 2009.10.29 bagus 2点間 修正 --{--
	else {
		if (iMinHeadType == HEAD_TYPE_SE)
			iMinHeadType = HEAD_TYPE_4PP;
	}
// 2009.10.29 bagus 2点間 修正 --}--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	if ( m_HeadType.bIRSE ) {
		nIndex = m_cbHeadType.AddString(HEAD_TYPE_ITEM[HEAD_TYPE_IRSE]);
		m_cbHeadType.SetItemData(nIndex, HEAD_TYPE_IRSE);
	}
#endif
// 2009.10.19 bagus MS 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
//	if ( m_HeadType.bResist ) {
	if (( m_HeadType.bResist ) && (!m_bRecalibration)) {
// 2009.11.04 bagus RS 追加 --}--
		nIndex = m_cbHeadType.AddString(HEAD_TYPE_ITEM[HEAD_TYPE_4PP]);
		m_cbHeadType.SetItemData(nIndex, HEAD_TYPE_4PP);
	}
// 2009.10.29 bagus 2点間 修正 --{--
	else {
		if (iMinHeadType == HEAD_TYPE_4PP)
			iMinHeadType = HEAD_TYPE_CTA;
	}
// 2009.10.29 bagus 2点間 修正 --}--
	if ( m_HeadType.bCTA ) {
		nIndex = m_cbHeadType.AddString(HEAD_TYPE_ITEM[HEAD_TYPE_CTA]);
		m_cbHeadType.SetItemData(nIndex, HEAD_TYPE_CTA);
	}
// 2009.10.29 bagus 2点間 修正 --{--
	else {
		if (iMinHeadType == HEAD_TYPE_CTA)
			iMinHeadType = HEAD_TYPE_STRESS;
	}
// 2009.10.29 bagus 2点間 修正 --}--
	if ( m_HeadType.bStress ) {
		nIndex = m_cbHeadType.AddString(HEAD_TYPE_ITEM[HEAD_TYPE_STRESS]);
		m_cbHeadType.SetItemData(nIndex, HEAD_TYPE_STRESS);
	}
// 2009.10.19 bagus MS 追加 --{--
// 2009.10.29 bagus 2点間 修正 --{--
	else {
		if (iMinHeadType == HEAD_TYPE_STRESS)
			iMinHeadType = HEAD_TYPE_MS;
	}
// 2009.10.29 bagus 2点間 修正 --}--
//	if ( m_HeadType.bMS ) {
	if (( m_HeadType.bMS ) && (!m_bRecalibration)) {
		nIndex = m_cbHeadType.AddString(HEAD_TYPE_ITEM[HEAD_TYPE_MS]);
		m_cbHeadType.SetItemData(nIndex, HEAD_TYPE_MS);
	}
// 2009.10.19 bagus MS 追加 --}--

	// Kojika 20090601 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090601 Add End

// 2009.10.24 bagus 2点間 修正 --{--
//	if(m_cbHeadType.GetCount() <= 0){
	if ((m_cbHeadType.GetCount() <= 0) || (iMinHeadType < 0)) {
// 2009.10.24 bagus 2点間 修正 --}--
		// Kojika 20090601 Change
		//MessageBox("Head Setting has not been set up.", "SELECT MEASUREMENT TYPE", MB_OK);
		//MessageBox("ヘッドタイプが設定されていません", "SELECT MEASUREMENT TYPE", MB_OK);
		LoadStringML(IDS_HEAD_SETTING_NOT_SETUP, l_strBuffer, "Head Setting has not been set up.");
		LoadStringML(IDS_TITLE_SELECT_MEASUREMENT_TYPE, l_strTitle, "SELECT MEASUREMENT TYPE");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		// Kojika 20090601 Change End
		CDialog::OnCancel();
		return FALSE;
	}

	int iDefaultHeadType = m_bRecipeDefaultValueSetting ? m_iDefaultValueSettingHeadType : m_iHeadType;
// 2009.10.24 bagus 2点間 修正 --{--
	switch ( iDefaultHeadType ) {
	case HEAD_TYPE_SR:
		if (!m_HeadType.bSR) {
			iDefaultHeadType = iMinHeadType;
			m_iHeadType = iMinHeadType;
			m_iMeasType = 0;
		}
		break;
	case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//		if (!m_HeadType.bSE) {
		if (!m_HeadType.bSE ||
			m_HeadType.bSE && m_HeadType.bCompEASE) {
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			iDefaultHeadType = iMinHeadType;
			m_iHeadType = iMinHeadType;
			m_iMeasType = 0;
		}
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		if (!m_HeadType.bCompEASE) {
			iDefaultHeadType = iMinHeadType;
			m_iHeadType = iMinHeadType;
			m_iMeasType = 0;
		}
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	case HEAD_TYPE_4PP:
// 2009.11.04 bagus RS 追加 --{--
//		if (!m_HeadType.bResist) {
		if ((!m_HeadType.bResist) || (m_bRecalibration)) {
// 2009.11.04 bagus RS 追加 --}--
			iDefaultHeadType = iMinHeadType;
			m_iHeadType = iMinHeadType;
			m_iMeasType = 0;
		}
		break;
	case HEAD_TYPE_CTA:
		if (!m_HeadType.bCTA) {
			iDefaultHeadType = iMinHeadType;
			m_iHeadType = iMinHeadType;
			m_iMeasType = 0;
		}
		break;
	case HEAD_TYPE_STRESS:
		if (!m_HeadType.bStress) {
			iDefaultHeadType = iMinHeadType;
			m_iHeadType = iMinHeadType;
			m_iMeasType = 0;
		}
		break;
	case HEAD_TYPE_MS:
//		if (!m_HeadType.bMS) {
		if ((!m_HeadType.bMS) || (m_bRecalibration)) {
			iDefaultHeadType = iMinHeadType;
			m_iHeadType = iMinHeadType;
			m_iMeasType = 0;
		}
		break;
	default:
		return FALSE;
		break;
	}
// 2009.10.24 bagus 2点間 修正 --}--
	m_cbHeadType.SetCurSel(m_cbHeadType.FindStringExact(0, HEAD_TYPE_ITEM[iDefaultHeadType]));

	OnSelchangeHeadType();

	// デフォルト値の設定の場合は指定されたヘッドタイプで固定
	m_cbHeadType.EnableWindow(!m_bRecipeDefaultValueSetting);


	// Measurement Item
	if ( !m_bRecipeDefaultValueSetting ) {
		CMLScanType mlScanTypeItem(m_iHeadType);
		m_lbMeasTypeList.SetCurSel(m_lbMeasTypeList.FindStringExact(0, mlScanTypeItem[m_iMeasType]));
	}

	// タイトルの変更
	switch( ((CMainFrame*)AfxGetMainWnd())->GetCurrExeWnd() ) {
	case USERCONF_WND:
		if ( m_bRecipeDefaultValueSetting ) {
			LoadStringML(IDS_RECIPE_DEFAULT_SETTING, l_strBuffer, "RECIPE DEFAULT VALUE SETTING");
		}
		else{
			LoadStringML(IDS_HEAD_MEASERE_TYPE_SETTING, l_strBuffer, "DEFAULT HEAD & MEASUREMENT TYPE SETTING");
		}
		SetWindowText(l_strBuffer);
		break;
	}

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSelectMeasurementTypeDlg::OnOK()
{
	if ( m_cbHeadType.GetCurSel() == CB_ERR 	||
			m_lbMeasTypeList.GetCurSel() == CB_ERR ) {
		return;
	}

	m_iHeadType = m_cbHeadType.GetItemData(m_cbHeadType.GetCurSel());
	m_iMeasType = m_lbMeasTypeList.GetItemData(m_lbMeasTypeList.GetCurSel());

	///// Select Measurement Type /////
	if ( !m_bRecipeDefaultValueSetting ) {
		SaveMeasurementType();
	}

	CDialog::OnOK();
}

// =========================================================================
//
void CSelectMeasurementTypeDlg::OnDblclkMeasurementTypeList()
{
	OnOK();
}

// =========================================================================
//
void CSelectMeasurementTypeDlg::OnSelchangeHeadType()
{
	// コンボボックスで選択された該当のヘッドタイプについて
	// スキャンタイプをリスト表示する

	m_iHeadType = m_cbHeadType.GetItemData(m_cbHeadType.GetCurSel());

	SR_CONFIG SrConfig;
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	int i;
	int nIndex;

	m_lbMeasTypeList.ResetContent();

	CMLScanType mlScanTypeItem(m_iHeadType);
	switch(m_iHeadType){
	case HEAD_TYPE_SR:
		for ( i = 0; i < mlScanTypeItem.GetItemCount(); i++ ) {
			// 設定されていない項目は表示しない
			switch(i){
// 2009.10.27 bagus Gantry 追加修正 --{--
#if 0
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.10.22 bagus Stress 修正 --{--
//			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2009.10.22 bagus Stress 修正 --}--
#else
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
#endif
// 2009.10.27 bagus Gantry 追加修正 --}--
				if(!SrConfig.bTransmittance){
					continue;
				}
				break;
// 2009.10.29 bagus 2点間 修正 --{--
			case MEAS_PROG_TYPE_SR_DISTANCE:
				//2009.11.16 bagus 2point-distance --{--
				//if (m_bRecalibration) {
				// 2009.11.17 bagus 2点間 修正 --{--
				//if (!SrConfig.bDistance){
				if (!SrConfig.bDistance || m_bRecalibration) {
				// 2009.11.17 bagus 2点間 修正 --}--
				//2009.11.16 bagus 2point-distance --}--
					continue;
				}
				break;
// 2009.10.29 bagus 2点間 修正 --}--
			case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
				if(!SrConfig.bCieMeasurement){
					continue;
				}
				break;
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
				if(!SrConfig.bTransmittance || !SrConfig.bCieMeasurement){
					continue;
				}
				break;
			case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
				if(!SrConfig.bOdMeasurement){
					continue;
				}
				break;
// 2009.10.22 bagus Stress 修正 --{--
// 2009.10.27 bagus Gantry 追加修正 --{--
//			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.10.27 bagus Gantry 追加修正 --}--
				continue;
				break;
// 2009.10.22 bagus Stress 修正 --}--
			default:
				break;
			}

			nIndex = m_lbMeasTypeList.AddString(mlScanTypeItem[i]);
			m_lbMeasTypeList.SetItemData(nIndex, i);
		}
		break;
	case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	case HEAD_TYPE_IRSE:
	case HEAD_TYPE_MS:
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_CTA:
	case HEAD_TYPE_4PP:
	case HEAD_TYPE_STRESS:
		for ( i = 0; i < mlScanTypeItem.GetItemCount(); i++ ) {
			nIndex = m_lbMeasTypeList.AddString(mlScanTypeItem[i]);
			m_lbMeasTypeList.SetItemData(nIndex, i);
		}
		break;
	default:
		break;
	}
}

// =========================================================================
//
void CSelectMeasurementTypeDlg::SaveMeasurementType()
{
	m_DefaultHeadType.nHeadType = m_iHeadType;
	m_DefaultHeadType.nMeasType = m_iMeasType;

	ConfigFile_SetNanoSpecIni(&m_DefaultHeadType, CONFIG_FILE_DEFAULT_HEAD_TYPE);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);
}
