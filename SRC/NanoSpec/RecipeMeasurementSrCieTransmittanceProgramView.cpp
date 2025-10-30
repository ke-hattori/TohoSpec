// RecipeMeasurementSrCieTransmittanceProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "RecipeMeasurementSrCieTransmittanceProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementSrCieTransmittanceProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementSrCieTransmittanceProgramView, CRecipeMeasurementSrCieReflectanceProgramView)

// =========================================================================
//
CRecipeMeasurementSrCieTransmittanceProgramView::CRecipeMeasurementSrCieTransmittanceProgramView()
{
// Kojika 20090527 Change
//	m_strCaption = "SR CHROMATICITY TRANSMITTANCE";
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SR_TRANSMIT, l_strBuffer, "SR CHROMATICITY TRANSMITTANCE");
	m_strCaption = l_strBuffer;
//Kojika 20090527 Change End
}

// =========================================================================
//
CRecipeMeasurementSrCieTransmittanceProgramView::~CRecipeMeasurementSrCieTransmittanceProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementSrCieTransmittanceProgramView::LoadRecipeData()
{
	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ChroMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE) ) {
			m_ChroMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
			m_ChroMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ChroMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {			// RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ChroMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE) ) {
				m_ChroMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
				m_ChroMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE;
			}
		}
		break;
	}
	::CopyMemory(&m_OldChroMeas, &m_ChroMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		strcpy(m_ChroMeas.hdr.szName, "Untitled");
	}
}

// =========================================================================
//
BOOL CRecipeMeasurementSrCieTransmittanceProgramView::SaveRecipeData()
{
// 2009.09.19 bagus SE --{--
	SR_CONFIG	SrConfig;

	memset(&SrConfig, 0, sizeof(SrConfig));
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	m_ChroMeas.ScanParams._SR.iLens = SrConfig.iDefaultMeasLensNo;
	m_ChroMeas.ScanParams._SR.wOpticsFilterType = SrConfig.wDefaultMeasFilter;
// 2009.09.19 bagus SE --}--

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_ChroMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_ChroMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ChroMeas, m_szRecipeName, RECIPE_FILE_SR_TRANSMITTANCE_CIE) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldChroMeas, &m_ChroMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

