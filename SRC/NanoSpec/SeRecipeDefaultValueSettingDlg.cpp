// SeRecipeDefaultValueSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SelectMeasurementTypeDlg.h"
#include "SeRecipeDefaultValueSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSeRecipeDefaultValueSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSeRecipeDefaultValueSettingDlg::CSeRecipeDefaultValueSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeRecipeDefaultValueSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeRecipeDefaultValueSettingDlg)
	//}}AFX_DATA_INIT
	m_DefaultHeadType = HEAD_TYPE_SE;
}

// =========================================================================
//
void CSeRecipeDefaultValueSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeRecipeDefaultValueSettingDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_MAIN_RECIPE_BUTTON, m_MainRecipeButton);
	DDX_Control(pDX, IDC_MEASUREMENT_PROGRAM_BUTTON, m_MeasurementProgramButton);
	//Saiki 20090410 Add ----->
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	//Saiki 20090410 Add <-----
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSeRecipeDefaultValueSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSeRecipeDefaultValueSettingDlg)
	ON_BN_CLICKED(IDC_MAIN_RECIPE_BUTTON, OnMainRecipeButton)
	ON_BN_CLICKED(IDC_MEASUREMENT_PROGRAM_BUTTON, OnMeasurementProgramButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSeRecipeDefaultValueSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSeRecipeDefaultValueSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Main Recipe Button /////
	m_MainRecipeButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MainRecipeButton.DrawFlatFocus(TRUE);

	///// Measurement Program Button /////
	m_MeasurementProgramButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementProgramButton.DrawFlatFocus(TRUE);

	pMainFrame->m_bNew = FALSE;
	pMainFrame->SetSelectListName("");

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSeRecipeDefaultValueSettingDlg::OnMainRecipeButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->m_bDefaultSetting = TRUE;
// 2009.09.29 bagus SE --{--
//	pMainFrame->ChangeExeWnd(USERCONF_DEF_MAIN_RECIPE_WND);
	switch ( m_DefaultHeadType ) {
	case HEAD_TYPE_SE:
	default:
	pMainFrame->ChangeExeWnd(USERCONF_DEF_SE_MAIN_RECIPE_WND);
		break;
	case HEAD_TYPE_COMPEASE:
		pMainFrame->ChangeExeWnd(USERCONF_DEF_COMPEASE_MAIN_RECIPE_WND);
		break;
	}
// 2009.09.29 bagus SE --}--
	pMainFrame->m_bDefaultSetting = FALSE;
	EndDialog(IDOK);
}

// =========================================================================
//
void CSeRecipeDefaultValueSettingDlg::OnMeasurementProgramButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CSelectMeasurementTypeDlg dlg(TRUE);

	if(dlg.DoModal() != IDOK){
		return;
	}

	pMainFrame->m_bDefaultSetting = TRUE;

	switch(dlg.m_iHeadType){
	case HEAD_TYPE_SR:
		switch(dlg.m_iMeasType){
			case MEAS_PROG_TYPE_SR_THICKNESS:
				pMainFrame->ChangeExeWnd(USERCONF_DEF_SR_THICKNESS_WND);
				break;
			case MEAS_PROG_TYPE_SR_REFLECTANCE:
				pMainFrame->ChangeExeWnd(USERCONF_DEF_SR_REFLECTANCE_WND);
				break;
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.10.27 bagus Gantry 追加修正 --{--
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2009.10.27 bagus Gantry 追加修正 --}--
				pMainFrame->ChangeExeWnd(USERCONF_DEF_SR_TRANSMITTANCE_WND);
				break;
			case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
				pMainFrame->ChangeExeWnd(USERCONF_DEF_SR_CIE_REFLECTANCE_WND);
				break;
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
				pMainFrame->ChangeExeWnd(USERCONF_DEF_SR_CIE_TRANSMITTANCE_WND);
				break;
			case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
				pMainFrame->ChangeExeWnd(USERCONF_DEF_SR_OD_WND);
				break;
			// 2009.10.13 bagus Distance 追加 --{--
			case MEAS_PROG_TYPE_SR_DISTANCE:
				pMainFrame->ChangeExeWnd(USERCONF_DEF_SR_DISTANCE_WND);
				break;
			// 2009.10.13 bagus Distance 追加 --}--
		}
		break;
	case HEAD_TYPE_SE:
		//2009.09.07 bagus se --{--
		switch(dlg.m_iMeasType){
		case MEAS_PROG_TYPE_SE_THICKNESS:
			pMainFrame->ChangeExeWnd(USERCONF_DEF_SE_THICKNESS_WND);
			break;
		}
		//2009.09.07 bagus se --}--
		break;
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		switch(dlg.m_iMeasType){
		case MEAS_PROG_TYPE_COMPEASE_THICKNESS:
			pMainFrame->ChangeExeWnd(USERCONF_DEF_COMPEASE_THICKNESS_WND);
			break;
		}
		break;
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case HEAD_TYPE_IRSE:
		break;
#else
	case HEAD_TYPE_MS:
		switch(dlg.m_iMeasType){
		case MEAS_PROG_TYPE_MS_MEAS:
			pMainFrame->ChangeExeWnd(USERCONF_DEF_MS_WND);
			break;
		}
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:
		pMainFrame->ChangeExeWnd(USERCONF_DEF_SR_4PP_WND);
		break;
	case HEAD_TYPE_CTA:
		// 2009.10.08 bagus CTA 追加 --{--
		switch(dlg.m_iMeasType){
		case MEAS_PROG_TYPE_CTA_MEAS:
			pMainFrame->ChangeExeWnd(USERCONF_DEF_CTA_WND);
			break;
		}
		// 2009.10.08 bagus CTA 追加 --}--
		break;
	case HEAD_TYPE_STRESS:
		//2009.10.21 bagus Stress 追加 --{--
		switch(dlg.m_iMeasType){
		case MEAS_PROG_TYPE_STRESS_MEAS:
			pMainFrame->ChangeExeWnd(USERCONF_DEF_STRESS_WND);
			break;
		}
		//2009.10.21 bagus Stress 追加 --}--
		break;
	}

	pMainFrame->m_bDefaultSetting = FALSE;

	EndDialog(IDOK);
}
