// CRecipeMeasurementSrCieReflectanceProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "RecipeMeasurementSrCieReflectanceProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementSrCieReflectanceProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementSrCieReflectanceProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMeasurementSrCieReflectanceProgramView::CRecipeMeasurementSrCieReflectanceProgramView()
	: CNanoRecipeUI(CRecipeMeasurementSrCieReflectanceProgramView::IDD)
{
// Kojika 20090527 Change
//	m_strCaption = "SR CHROMATICITY REFLECTANCE";
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SR_REFLECT, l_strBuffer, "SR CHROMATICITY REFLECTANCE");
	m_strCaption = l_strBuffer;
//Kojika 20090527 Change End

	m_strLens = _T("");

	//{{AFX_DATA_INIT(CRecipeMeasurementSrCieReflectanceProgramView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementSrCieReflectanceProgramView::~CRecipeMeasurementSrCieReflectanceProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementSrCieReflectanceProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementSrCieReflectanceProgramView)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_REFLECT_CIE_PROG_NAME, m_ChroMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_CBStringExact(pDX, IDC_CIE_LENS, m_strLens);
	DDX_Text(pDX, IDC_MEAS_CIE_INTEGRATION_TIME, m_ChroMeas.ScanParams._SR.dIntegTime);
	DDX_Text(pDX, IDC_GAMMA, m_ChroMeas.ScanParams._SR.CIEOptDesc.dGammaCorrectFactor);
	DDX_CBIndex(pDX, IDC_COLOR_SYSTEM, (int&)m_ChroMeas.ScanParams._SR.CIEOptDesc.wColorSystem);
	DDX_CBIndex(pDX, IDC_ILLUMINANT_TABLE, (int&)m_ChroMeas.ScanParams._SR.CIEOptDesc.wIlluminant);
	DDX_Text(pDX, IDC_CIE_REFERENCE_MATERIAL, m_ChroMeas.Ref.OptDesc.szRefMaterial, _MAX_PATH + 1);
	DDX_Text(pDX, IDC_CIE_COMMENT, m_ChroMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CRecipeMeasurementSrCieReflectanceProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementSrCieReflectanceProgramView)
	ON_CBN_SELCHANGE(IDC_CIE_LENS, OnSelchangeCieLens)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// =========================================================================
// CRecipeMeasurementSrCieReflectanceProgramView メッセージ ハンドラ

// =========================================================================
//
void CRecipeMeasurementSrCieReflectanceProgramView::OnInitialUpdate()
{
	///// Load File /////
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	LoadRecipeData();

	m_strLens = m_SrTurret[m_ChroMeas.ScanParams._SR.iLens].szName;



	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// 作成に失敗
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, 20, SWP_NOMOVE);

	///// Control Initialize /////
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_REFLECT_CIE_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// Combo Box Initialize /////
	InitCombo_ColorSystem();
	InitCombo_IlluminantTable();
	InitCombo_ReferenceMaterial();
	InitCombo_Lens();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_CIE_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting ) {
		GetDlgItem(IDC_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REFLECT_CIE_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CIE_COMMENT)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================/
//
void CRecipeMeasurementSrCieReflectanceProgramView::LoadRecipeData()
{
	// Kojika 20090529 Add
	CString strBuffer;
	// Kojika 20090529 Add End

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ChroMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_REFLECTANCE_CIE) ) {
			m_ChroMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
			m_ChroMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_REFLECTANCE_CIE;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ChroMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {	// RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ChroMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_REFLECTANCE_CIE) ) {
				m_ChroMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
				m_ChroMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_REFLECTANCE_CIE;
			}
		}
		break;
	}
	::CopyMemory(&m_OldChroMeas, &m_ChroMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		// Kojika 20090529 Change
		//strcpy(m_ChroMeas.hdr.szName, "Untitled");
		LoadStringML(IDS_UNTITLE, strBuffer, "Untitled");
		strcpy(m_ChroMeas.hdr.szName, strBuffer);
		// Kojika 20090529 Change End
	}

}

// =========================================================================
//
void CRecipeMeasurementSrCieReflectanceProgramView::InitCombo_ColorSystem()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COLOR_SYSTEM);
	pCombo->ResetContent();

	for (int i = 0; i < COLORSYS_MAX; i++ )
		pCombo->AddString(COLORSYSTEMS_TYPE_ITEM[i]);

	pCombo->SetCurSel(m_ChroMeas.ScanParams._SR.CIEOptDesc.wColorSystem);
}

// =========================================================================
//
void CRecipeMeasurementSrCieReflectanceProgramView::InitCombo_IlluminantTable()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_ILLUMINANT_TABLE);
	pCombo->ResetContent();

	for (int i = 0; i < ILLUMINANT_MAX; i++ )
		pCombo->AddString(ILLUMINANT_TABLE_ITEM[i]);

	pCombo->SetCurSel(m_ChroMeas.ScanParams._SR.CIEOptDesc.wIlluminant);
}

// =========================================================================
//
void CRecipeMeasurementSrCieReflectanceProgramView::InitCombo_ReferenceMaterial()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CIE_REFERENCE_MATERIAL);
	pCombo->ResetContent();

	CString strDirPath;
	strDirPath.Format("%s*%s", g_szCfg_System_Sr_Reference_Material_Dir, DAT_EXT);
	CFileFind Finder;
	if ( !Finder.FindFile(strDirPath) )
		return;

	//Saiki 20090410 Change ----->
	//BOOL bLoop;
	BOOL bLoop = TRUE;
	//Saiki 20090410 Change <-----
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		pCombo->AddString(Finder.GetFileTitle());
	}

	int nIndex = pCombo->FindStringExact(0, m_ChroMeas.Ref.OptDesc.szRefMaterial);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
BOOL CRecipeMeasurementSrCieReflectanceProgramView::CheckData()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	UpdateData(TRUE);

	CString strBuffer;
	CString strCaption;

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
// Kojika 20090527 Change
//		strCaption = "DEFAULT SETTING - ";
		LoadStringML(IDS_CAPTION_DEF_SETTING, l_strBuffer, "DEFAULT SETTING - ");
		strCaption = l_strBuffer;
// Kojika 20090527 Change End
		break;
	default:
// Kojika 20090527 Change
//		strCaption = "MEASUREMET PROGRAM - ";
		LoadStringML(IDS_CAPTION_MEASURE_PROG, l_strBuffer, "MEASUREMET PROGRAM - ");
		strCaption = l_strBuffer;
// Kojika 20090527 Change End
		break;
	}
	strCaption += m_strCaption;

	///// Lens /////
	if ( m_strLens.IsEmpty() )
	{
// Kojika 20090527 Change
//		MessageBox("Please Select Lens.", m_strCaption, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_SELECT_LENS, l_strBuffer, "Please Select Lens.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
		GetDlgItem(IDC_CIE_LENS)->SetFocus();
		return FALSE;
	}

	///// Integration Time /////
	if ( m_ChroMeas.ScanParams._SR.dIntegTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_ChroMeas.ScanParams._SR.dIntegTime ) {
// Kojika 20090527 Change
//		strBuffer.Format("Integration Time is out of range.\n(%.1f - %.1f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		LoadStringML(IDS_INTEGTIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.1f - %.1f)");
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
// Kojika 20090527 Change End
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_MEAS_CIE_INTEGRATION_TIME)->SetFocus();
		return FALSE;
	}

	///// Gamma /////
	if ( m_ChroMeas.ScanParams._SR.CIEOptDesc.dGammaCorrectFactor < MIN_GAMMA || MAX_GAMMA < m_ChroMeas.ScanParams._SR.CIEOptDesc.dGammaCorrectFactor ) {
// Kojika 20090527 Change
//		strBuffer.Format("Gamma Correct Factor is out of range.\n(%.1f - %.1f)", MIN_GAMMA, MAX_GAMMA);
		LoadStringML(IDS_GAMMA_OUT_RANGE, l_strBuffer, "Gamma Correct Factor is out of range.\n(%.1f - %.1f)");
		strBuffer.Format(l_strBuffer, MIN_GAMMA, MAX_GAMMA);
// Kojika 20090527 Change End
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_GAMMA)->SetFocus();
		return FALSE;
	}

	///// Color Systems /////
	if ( ((CComboBox *)GetDlgItem(IDC_COLOR_SYSTEM))->GetCurSel() == CB_ERR ) {
// Kojika 20090527 Change
//		MessageBox("Please Select a Color Systems", strCaption, MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_COLOR_SYSTEM, l_strBuffer, "Please Select a Color Systems");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
// Kojika 20090527 Change End
		return FALSE;
	}

	///// Illuminant Table /////
	if ( ((CComboBox *)GetDlgItem(IDC_ILLUMINANT_TABLE))->GetCurSel() == CB_ERR ) {
// Kojika 20090527 Change
//		MessageBox("Please Select an Illuminant Table", strCaption, MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_ILLUMINANT_TABLE, l_strBuffer, "Please Select an Illuminant Table");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
// Kojika 20090527 Change End
		return FALSE;
	}

	///// Reference Strategy /////
	if ( strlen(m_ChroMeas.Ref.OptDesc.szRefMaterial) == 0 ) {
// Kojika 20090527 Change
//		MessageBox("Reference Strategy is empty.", strCaption, MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_REFER_STRATEGY_EMPTY, l_strBuffer, "Reference Strategy is empty.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
// Kojika 20090527 Change End
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrCieReflectanceProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldChroMeas, &m_ChroMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrCieReflectanceProgramView::SaveRecipeData()
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
		if ( !RecipeFile_SaveRecipe(&m_ChroMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_REFLECTANCE_CIE) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_ChroMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ChroMeas, m_szRecipeName, RECIPE_FILE_SR_REFLECTANCE_CIE) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldChroMeas, &m_ChroMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrCieReflectanceProgramView::IsAccessPrivilege()
{
	int iAccessItem;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( m_nOpenMode == modeDefaultSetting )
		iAccessItem = ACCESS_USERSETTING;
	else
		iAccessItem = ACCESS_PROGRAM;

	return pDoc->User_Access(iAccessItem);
}

// =========================================================================
//
int CRecipeMeasurementSrCieReflectanceProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

// =========================================================================
// レンズ コンボ選択時
//
void CRecipeMeasurementSrCieReflectanceProgramView::OnSelchangeCieLens()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CIE_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_ChroMeas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	UpdateData(FALSE);
}

// =========================================================================
//レンズ コンボイニシャル処理
//
void CRecipeMeasurementSrCieReflectanceProgramView::InitCombo_Lens()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CIE_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
// 2009.11.24 bagus SR 修正 --{--
//		  if ( m_SrTurret[i].bEnable ) {
		if ( m_SrTurret[i].bEnable && m_SrTurret[i].bAnalysis ) {
// 2009.11.24 bagus SR 修正 --}--
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	nIndex = pCombo->FindStringExact(-1, m_strLens);
// 2009.12.09 bagus SR --{--
//	pCombo->SetCurSel(nIndex);
	CString l_strBuffer, l_strTitle;

	if(!(nIndex == CB_ERR)){
		pCombo->SetCurSel(nIndex);
	}
	else{
		if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus SR --{--
			m_ChroMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.14 bagus SR --}--
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
			m_ChroMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
			SaveRecipeData();
			LoadStringML(IDS_LENS_ZERO, l_strBuffer, "Invalid for the specified item, Lens is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
// 2009.12.09 bagus SR --}--
}

