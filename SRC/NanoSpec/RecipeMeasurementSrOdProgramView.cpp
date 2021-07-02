// RecipeMeasurementSrOdProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "RecipeMeasurementSrOdProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementSrOdProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementSrOdProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMeasurementSrOdProgramView::CRecipeMeasurementSrOdProgramView()
	: CNanoRecipeUI(CRecipeMeasurementSrOdProgramView::IDD)
{
// Kojika 20090527 Change
//	m_strCaption = "SR OPTICAL DENSITY";
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SR_DENSITY, l_strBuffer, "SR OPTICAL DENSITY");
	m_strCaption = l_strBuffer;
// Kojika 20090527 Change End

	m_strLens = _T("");


	//{{AFX_DATA_INIT(CRecipeMeasurementSrOdProgramView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementSrOdProgramView::~CRecipeMeasurementSrOdProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementSrOdProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementSrOdProgramView)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_OD_PROG_NAME, m_ODMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_CBStringExact(pDX, IDC_MEAS_REFL_LENS, m_strLens);
	DDX_Text(pDX, IDC_MEAS_OD_INTEGRATION_TIME, m_ODMeas.ScanParams._SR.dIntegTime);
	DDX_Text(pDX, IDC_SR_OD_FIRST_REF_MATERIAL, m_ODMeas.Ref.OptDesc.szRefMaterial, _MAX_PATH + 1);
	//DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG1, m_ODMeas.ScanParams._SR.iSpecificWavelen[0]);
	//DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG2, m_ODMeas.ScanParams._SR.iSpecificWavelen[1]);
	//DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG3, m_ODMeas.ScanParams._SR.iSpecificWavelen[2]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG1, m_strSpecificWavelen[0]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG2, m_strSpecificWavelen[1]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG3, m_strSpecificWavelen[2]);
	DDX_Check(pDX, IDC_SR_OD_CHK_SECOND_REF, m_ODMeas.Ref2nd.bMeasure);
	DDX_Text(pDX, IDC_SR_OD_SECOND_INTE_TIME, m_ODMeas.Ref2nd.dIntegTime);
	DDX_Text(pDX, IDC_SR_OD_SECOND_REF_MATERIAL, m_ODMeas.Ref2nd.szRefMaterial, _MAX_PATH + 1);
	DDX_Text(pDX, IDC_OD_COMMENT, m_ODMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

BEGIN_MESSAGE_MAP(CRecipeMeasurementSrOdProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementSrOdProgramView)
	ON_BN_CLICKED(IDC_SR_OD_CHK_SECOND_REF, OnCheckSeconReferenceMeasurement)
	ON_CBN_SELCHANGE(IDC_MEAS_REFL_LENS, OnSelchangeMeasReflLens)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// =========================================================================
// CRecipeMeasurementSrOdProgramView メッセージ ハンドラ

// =========================================================================
//
void CRecipeMeasurementSrOdProgramView::OnInitialUpdate()
{
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SrXmp, CONFIG_FILE_SR_XMP);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	LoadRecipeData();

	m_strLens = m_SrTurret[m_ODMeas.ScanParams._SR.iLens].szName;


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

	///// Control Initalize /////
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_OD_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// 測定反射率波長設定 /////
	if (m_ODMeas.ScanParams._SR.iSpecificWavelen[0] != 0){
		m_strSpecificWavelen[0].Format("%d",m_ODMeas.ScanParams._SR.iSpecificWavelen[0]);
	}
	else{
		m_strSpecificWavelen[0].Format("%s","");
	}
	if (m_ODMeas.ScanParams._SR.iSpecificWavelen[1] != 0){
		m_strSpecificWavelen[1].Format("%d",m_ODMeas.ScanParams._SR.iSpecificWavelen[1]);
	}
	else{
		m_strSpecificWavelen[1].Format("%s","");
	}
	if (m_ODMeas.ScanParams._SR.iSpecificWavelen[2] != 0){
		m_strSpecificWavelen[2].Format("%d",m_ODMeas.ScanParams._SR.iSpecificWavelen[2]);
	}
	else{
		m_strSpecificWavelen[2].Format("%s","");
	}
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG1)->SetWindowText(m_strSpecificWavelen[0]);
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG2)->SetWindowText(m_strSpecificWavelen[1]);
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG3)->SetWindowText(m_strSpecificWavelen[2]);

	///// Combo Box Initialize /////
	InitCombo_ReferenceMaterial((CComboBox*)GetDlgItem(IDC_SR_OD_FIRST_REF_MATERIAL), m_ODMeas.Ref.OptDesc.szRefMaterial);
	InitCombo_ReferenceMaterial((CComboBox*)GetDlgItem(IDC_SR_OD_SECOND_REF_MATERIAL), m_ODMeas.Ref2nd.szRefMaterial);
	InitCombo_Lens();

	OnCheckSeconReferenceMeasurement();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_OD_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OD_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OD_COMMENT)->ShowWindow(SW_HIDE);
	}

}

// =========================================================================
// ２ＮＤリファレンス チェック選択時
//
void CRecipeMeasurementSrOdProgramView::OnCheckSeconReferenceMeasurement()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_SR_OD_SECOND_INTE_TIME)->EnableWindow(m_ODMeas.Ref2nd.bMeasure);
	GetDlgItem(IDC_SR_OD_SECOND_REF_MATERIAL)->EnableWindow(m_ODMeas.Ref2nd.bMeasure);
}

// =========================================================================
//
void CRecipeMeasurementSrOdProgramView::LoadRecipeData()
{
	// Kojika 20090528 Add
	CString l_strBuffer;
	// Kojika 20090528 Add End

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ODMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_OPTICAL_DENSITY) ) {
			m_ODMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
			m_ODMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_OPTICAL_DENSITY;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ODMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) { 			// RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ODMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_OPTICAL_DENSITY) ) {
				m_ODMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
				m_ODMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_OPTICAL_DENSITY;
			}
		}
		break;
	}
	::CopyMemory(&m_OldODMeas, &m_ODMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		// Kojika 20090528 Change
		//strcpy(m_ODMeas.hdr.szName, "Untitled");
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_ODMeas.hdr.szName, l_strBuffer);
		// Kojika 20090528 Change End
	}
}

// =========================================================================
//
void CRecipeMeasurementSrOdProgramView::InitCombo_ReferenceMaterial(CComboBox* pCombo, LPCTSTR pszMaterial)
{
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

	int nIndex = pCombo->FindStringExact(0, pszMaterial);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
BOOL CRecipeMeasurementSrOdProgramView::CheckData()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	UpdateData(TRUE);

	CString strBuffer;
	CString strCaption;
	int iValue;
	BOOL bValidWaveLen = FALSE;

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
		GetDlgItem(IDC_MEAS_REFL_LENS)->SetFocus();
		return FALSE;
	}

	///// Integration Time /////
	if ( m_ODMeas.ScanParams._SR.dIntegTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_ODMeas.ScanParams._SR.dIntegTime )
	{
// Kojika 20090527 Change
//		strBuffer.Format("Integration Time is out of range.\n(%.1f - %.1f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		LoadStringML(IDS_INTEGTIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.1f - %.1f)");
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
// Kojika 20090527 Change End
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_MEAS_OD_INTEGRATION_TIME)->SetFocus();
		return FALSE;
	}

	///// First Reference Strategy /////
	if ( strlen(m_ODMeas.Ref.OptDesc.szRefMaterial) == 0 )
	{
// Kojika 20090527 Change
//		MessageBox("First Reference Strategy is empty.", strCaption, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_FIRST_REFER_STRATEGY_EMPTY, l_strBuffer, "First Reference Strategy is empty.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
		return FALSE;
	}

	///// 測定反射率波長設定 /////
	//３つの設定値の上位が未設定の場合は上位に詰める。
	m_strSpecificWavelen[0].TrimLeft();
	m_strSpecificWavelen[0].TrimRight();
	m_strSpecificWavelen[1].TrimLeft();
	m_strSpecificWavelen[1].TrimRight();
	m_strSpecificWavelen[2].TrimLeft();
	m_strSpecificWavelen[2].TrimRight();
	//Target1(Null),Target2(Is),Target3(Null)
	if((m_strSpecificWavelen[0].IsEmpty() != 0) && (m_strSpecificWavelen[1].IsEmpty() == 0) && (m_strSpecificWavelen[2].IsEmpty() != 0))
	{
		m_strSpecificWavelen[0]=m_strSpecificWavelen[1];
		m_strSpecificWavelen[1]="";
	}
	//Target1(Null),Target2(Is),Target3(Is)
	if((m_strSpecificWavelen[0].IsEmpty() != 0) && (m_strSpecificWavelen[1].IsEmpty() == 0) && (m_strSpecificWavelen[2].IsEmpty() == 0))
	{
		m_strSpecificWavelen[0]=m_strSpecificWavelen[1];
		m_strSpecificWavelen[1]=m_strSpecificWavelen[2];
		m_strSpecificWavelen[2]="";
	}
	//Target1(Null),Target2(Null),Target3(Is)
	if((m_strSpecificWavelen[0].IsEmpty() != 0) && (m_strSpecificWavelen[1].IsEmpty() != 0) && (m_strSpecificWavelen[2].IsEmpty() == 0))
	{
		m_strSpecificWavelen[0]=m_strSpecificWavelen[2];
		m_strSpecificWavelen[2]="";
	}
	//Target1(Is),Target2(Null),Target3(Is)
	if((m_strSpecificWavelen[0].IsEmpty() == 0) && (m_strSpecificWavelen[1].IsEmpty() != 0) && (m_strSpecificWavelen[2].IsEmpty() == 0))
	{
		m_strSpecificWavelen[1]=m_strSpecificWavelen[2];
		m_strSpecificWavelen[2]="";
	}
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG1)->SetWindowText(m_strSpecificWavelen[0]);
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG2)->SetWindowText(m_strSpecificWavelen[1]);
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG3)->SetWindowText(m_strSpecificWavelen[2]);

	///// 1st Target WaveLength /////
	if (m_strSpecificWavelen[0].IsEmpty() == 0)
	{
		iValue = atoi(m_strSpecificWavelen[0]);
		if ((!CheckTransData(iValue,m_strSpecificWavelen[0])) || (iValue < MIN_TARGET_WAVELENGTH) || (MAX_TARGET_WAVELENGTH < iValue ))/**整数で、ターゲットが有効範囲に設定されているか？*/
		{
// Kojika 20090527 Change
//			strBuffer.Format("1st Target WaveLength is out of range.\n(%d - %d)", MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
			LoadStringML(IDS_FIRST_WAVELENGTH_OUT_RANGE, l_strBuffer, "1st Target WaveLength is out of range.\n(%d - %d)");
			strBuffer.Format(l_strBuffer, MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
// Kojika 20090527 Change End
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_WAVE_LENG1)->SetFocus();
			return FALSE;
		}
		else {
			bValidWaveLen = TRUE;
		}
	}

	///// 2nd Target WaveLength /////
	if (m_strSpecificWavelen[1].IsEmpty() == 0)/**ターゲットが入力されているか？*/
	{
		iValue = atoi(m_strSpecificWavelen[1]);
		if ((!CheckTransData(iValue,m_strSpecificWavelen[1])) || (iValue < MIN_TARGET_WAVELENGTH) || (MAX_TARGET_WAVELENGTH < iValue ))/**整数で、ターゲットが有効範囲に設定されているか？*/
		{
// Kojika 20090527 Change
//			strBuffer.Format("2nd Target WaveLength is out of range.\n(%d - %d)", MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
			LoadStringML(IDS_SECOND_WAVELENGTH_OUT_RANGE, l_strBuffer, "2nd Target WaveLength is out of range.\n(%d - %d)");
			strBuffer.Format(l_strBuffer, MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
// Kojika 20090527 Change End
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_WAVE_LENG2)->SetFocus();
			return FALSE;
		}
		else {
			bValidWaveLen = TRUE;
		}
	}

	///// 3rd Target WaveLength //////
	if (m_strSpecificWavelen[2].IsEmpty() == 0)
	{
		iValue = atoi(m_strSpecificWavelen[2]);
		if ((!CheckTransData(iValue,m_strSpecificWavelen[2])) || (iValue < MIN_TARGET_WAVELENGTH) || (MAX_TARGET_WAVELENGTH < iValue ))/**整数で、ターゲットが有効範囲に設定されているか？*/
		{
// Kojika 20090527 Change
//			strBuffer.Format("3rd Target WaveLength is out of range.\n(%d - %d)", MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
			LoadStringML(IDS_THIRD_WAVELENGTH_OUT_RANGE, l_strBuffer, "3rd Target WaveLength is out of range.\n(%d - %d)");
			strBuffer.Format(l_strBuffer, MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
// Kojika 20090527 Change End
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_WAVE_LENG3)->SetFocus();
			return FALSE;
		}
		else {
			bValidWaveLen = TRUE;
		}
	}

	if ( !bValidWaveLen )
	{
		// Kojika 20090528 Change
		//MessageBox("Please Input at Least One or More Wavelength.", strCaption, MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_INPUT_ONE_WAVELENGTH, l_strBuffer, "Please Input at Least One or More Wavelength.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
		// Kojika 20090528 Change End
		return FALSE;
	}
	else{
		m_ODMeas.ScanParams._SR.iSpecificWavelen[0] = atoi(m_strSpecificWavelen[0]);
		m_ODMeas.ScanParams._SR.iSpecificWavelen[1] = atoi(m_strSpecificWavelen[1]);
		m_ODMeas.ScanParams._SR.iSpecificWavelen[2] = atoi(m_strSpecificWavelen[2]);
	}

/*
	///// 1st Target WaveLength /////
	iValue = m_ODMeas.ScanParams._SR.iSpecificWavelen[0];
	if ( iValue != 0 )
	{
		if ( iValue < MIN_TARGET_WAVELENGTH || MAX_TARGET_WAVELENGTH < iValue )
		{
			strBuffer.Format("1st Target WaveLength is out of range.\n(%d ~ %d)", MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_WAVE_LENG1)->SetFocus();
			return FALSE;
		}
		else {
			bValidWaveLen = TRUE;
		}
	}

	///// 2nd Target WaveLength /////
	iValue = m_ODMeas.ScanParams._SR.iSpecificWavelen[1];
	if ( iValue != 0 )
	{
		if ( iValue < MIN_TARGET_WAVELENGTH || MAX_TARGET_WAVELENGTH < iValue )
		{
			strBuffer.Format("2nd Target WaveLength is out of range.\n(%d ~ %d)", MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_WAVE_LENG2)->SetFocus();
			return FALSE;
		}
		else {
			bValidWaveLen = TRUE;
		}
	}

	///// 3rd Target WaveLength //////
	iValue = m_ODMeas.ScanParams._SR.iSpecificWavelen[2];
	if ( iValue != 0 )
	{
		if ( iValue < MIN_TARGET_WAVELENGTH || MAX_TARGET_WAVELENGTH < iValue )
		{
			strBuffer.Format("3rd Target WaveLength is out of range.\n(%d ~ %d)", MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_WAVE_LENG3)->SetFocus();
			return FALSE;
		}
		else {
			bValidWaveLen = TRUE;
		}
	}

	if( !bValidWaveLen )
	{
		MessageBox("Please Input at Least One or More Wavelength.", strCaption, MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
*/
	if ( m_ODMeas.Ref2nd.bMeasure )
	{
		///// Second Integration Time /////
		if ( m_ODMeas.Ref2nd.dIntegTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_ODMeas.Ref2nd.dIntegTime )
		{
// Kojika 20090527 Change
//			strBuffer.Format("2nd Integration Time is out of range.\n(%.1f - %.1f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
			LoadStringML(IDS_SECOND_INTEGTIME_OUT_RANGE, l_strBuffer, "2nd Integration Time is out of range.\n(%.1f - %.1f)");
			strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
// Kojika 20090527 Change End
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_SR_OD_SECOND_INTE_TIME)->SetFocus();
			return FALSE;
		}

		///// Second Reference Strategy /////
		if ( strlen(m_ODMeas.Ref2nd.szRefMaterial) == 0 )
		{
// Kojika 20090527 Change
//			MessageBox("Second Reference Strategy is empty.", strCaption, MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_SECOND_REFER_STRATEGY_EMPTY, l_strBuffer, "Second Reference Strategy is empty.");
			MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
			return FALSE;
		}
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrOdProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldODMeas, &m_ODMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrOdProgramView::SaveRecipeData()
{
// 2009.09.19 bagus SE --{--
	SR_CONFIG	SrConfig;

	memset(&SrConfig, 0, sizeof(SrConfig));
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	m_ODMeas.ScanParams._SR.iLens = SrConfig.iDefaultMeasLensNo;
	m_ODMeas.ScanParams._SR.wOpticsFilterType = SrConfig.wDefaultMeasFilter;
// 2009.09.19 bagus SE --}--

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_ODMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_OPTICAL_DENSITY) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_ODMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ODMeas, m_szRecipeName, RECIPE_FILE_SR_OPTICAL_DENSITY) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldODMeas, &m_ODMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrOdProgramView::IsAccessPrivilege()
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
int CRecipeMeasurementSrOdProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

BOOL CRecipeMeasurementSrOdProgramView::CheckTransData(int iData , CString strData)
{
	CString strTransiData;

	strTransiData.Format("%d",iData);
	return strTransiData == strData;
}

// =========================================================================
// レンズ コンボ選択時
//
void CRecipeMeasurementSrOdProgramView::OnSelchangeMeasReflLens()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_ODMeas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	UpdateData(FALSE);
}


// =========================================================================
//レンズ コンボイニシャル処理
//
void CRecipeMeasurementSrOdProgramView::InitCombo_Lens()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_LENS);
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
			m_ODMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.14 bagus SR --}--
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
			m_ODMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
			SaveRecipeData();
			LoadStringML(IDS_LENS_ZERO, l_strBuffer, "Invalid for the specified item, Lens is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
// 2009.12.09 bagus SR --}--
}
