// RecipeMeasurementSrTransmittanceProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "RecipeMeasurementSrTransmittanceProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementSrTransmittanceProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementSrTransmittanceProgramView, CRecipeMeasurementSrReflectanceProgramView)

// =========================================================================
//
CRecipeMeasurementSrTransmittanceProgramView::CRecipeMeasurementSrTransmittanceProgramView()
{
// Kojika 20090527 Change
//	m_strCaption = "SR TRANSMITTANCE";
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SR_TRANSMITTANCE, l_strBuffer, "SR TRANSMITTANCE");
	m_strCaption = l_strBuffer;
// Kojika 20090527 Change End

	m_strLens = _T("");
}

// =========================================================================
//
CRecipeMeasurementSrTransmittanceProgramView::~CRecipeMeasurementSrTransmittanceProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementSrTransmittanceProgramView::LoadRecipeData()
{
// Kojika 20090529 Add
	CString l_strBuffer;
// Kojika 20090529 Add End

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ReflecMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_TRANSMITTANCE) ) {
			m_ReflecMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
			m_ReflecMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ReflecMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) { 		// RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ReflecMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_TRANSMITTANCE) ) {
				m_ReflecMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
				m_ReflecMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE;
			}
		}
		break;
	}
	::CopyMemory(&m_OldReflecMeas, &m_ReflecMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
// Kojika 20090529 Change
//		strcpy(m_ReflecMeas.hdr.szName, "Untitled");
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_ReflecMeas.hdr.szName, l_strBuffer);
// Kojika 20090529 Change End
	}
	//2009.12.21 bagus 修正 --{--
	//暫定対策
// 2010.01.06 bagus Gantry --{--
//	GetDlgItem(IDC_TEST_BUTTON)->ShowWindow(SW_HIDE);
// 2010.01.06 bagus Gantry --}--
	//2009.12.21 bagus 修正 --}--
}

// =========================================================================
//
BOOL CRecipeMeasurementSrTransmittanceProgramView::SaveRecipeData()
{
// 2009.10.09 K.Matsuo Delete -->
// 2009.09.19 bagus SE --{--
//	SR_CONFIG	SrConfig;
//
//	memset(&SrConfig, 0, sizeof(SrConfig));
//	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);
//
//	m_ReflecMeas.ScanParams._SR.iLens = SrConfig.iDefaultMeasLensNo;
//	m_ReflecMeas.ScanParams._SR.wOpticsFilterType = SrConfig.wDefaultMeasFilter;
// 2009.09.19 bagus SE --}--
// 2009.10.09 K.Matsuo Delete -->

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_ReflecMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_TRANSMITTANCE) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_ReflecMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ReflecMeas, m_szRecipeName, RECIPE_FILE_SR_TRANSMITTANCE) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldReflecMeas, &m_ReflecMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

