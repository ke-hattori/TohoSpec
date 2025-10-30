// RecipeMeasurementSrReflectanceProgramView.cpp : ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "TestMeasurementDlg.h"
// 2010.01.07 bagus Gantry --{--
#include "TestMeasurementGantryDlg.h"
// 2010.01.07 bagus Gantry --}--
#include "RecipeMeasurementSrReflectanceProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementSrReflectanceProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementSrReflectanceProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMeasurementSrReflectanceProgramView::CRecipeMeasurementSrReflectanceProgramView()
	: CNanoRecipeUI(CRecipeMeasurementSrReflectanceProgramView::IDD)
{
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SR_REFLECTANCE, l_strBuffer, "SR REFLECTANCE");
	m_strCaption = l_strBuffer;

	m_strLens = _T("");

// 2010.01.07 bagus Gantry --{--
	m_iNewHeadType = HEAD_TYPE_SR;
	m_iNewScanType = MEAS_PROG_TYPE_SR_REFLECTANCE;
// 2010.01.07 bagus Gantry --}--

	//{{AFX_DATA_INIT(CRecipeMeasurementSrReflectanceProgramView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementSrReflectanceProgramView::~CRecipeMeasurementSrReflectanceProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementSrReflectanceProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementSrReflectanceProgramView)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_REFLECTANCE_PROG_NAME, m_ReflecMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_CBStringExact(pDX, IDC_MEAS_REFL_LENS, m_strLens);
	DDX_CBStringExact(pDX, IDC_MEAS_REFL_COMBO_OPT_FILTER, m_strOptFilter);
	DDX_Text(pDX, IDC_MEAS_REFL_INTEGRATION_TIME, m_ReflecMeas.ScanParams._SR.dIntegTime);
	DDX_Check(pDX, IDC_ENHANCED_UV, m_ReflecMeas.ScanParams._SR.bEnhancedUv);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG1, m_strSpecificWavelen[0]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG2, m_strSpecificWavelen[1]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG3, m_strSpecificWavelen[2]);
	DDX_Check(pDX, IDC_MEAS_REFL_CHK_WARN_LIFETIME, m_ReflecMeas.Ref.hdr.bWarnFlag);
	DDX_Text(pDX, IDC_MEAS_REFL_LIFETIME, m_ReflecMeas.Ref.hdr.dLifeTime);
	DDX_Text(pDX, IDC_MEAS_REFL_REF_MATERIAL, m_ReflecMeas.Ref.OptDesc.szRefMaterial, _MAX_PATH + 1);
	DDX_Text(pDX, IDC_REFLECTANCE_COMMENT, m_ReflecMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
	DDX_Control(pDX, IDC_TEST_BUTTON, m_TestButton);
}

BEGIN_MESSAGE_MAP(CRecipeMeasurementSrReflectanceProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementSrReflectanceProgramView)
	ON_BN_CLICKED(IDC_MEAS_REFL_CHK_WARN_LIFETIME, OnCheckReferenceLifeTime)
	ON_CBN_SELCHANGE(IDC_MEAS_REFL_COMBO_OPT_FILTER, OnSelchangeSrMeasReflOptFilter)
	ON_CBN_SELCHANGE(IDC_MEAS_REFL_LENS, OnSelchangeMeasReflLens)
	ON_BN_CLICKED(IDC_TEST_BUTTON, OnTestButton)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// =========================================================================
// CRecipeMeasurementSrReflectanceProgramView ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½

// =========================================================================
//
void CRecipeMeasurementSrReflectanceProgramView::OnInitialUpdate ()
{
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	ConfigFile_GetNanoSpecIni(m_srFilter, CONFIG_FILE_SR_FILTER);
	LoadRecipeData();

	m_strLens = m_SrTurret[m_ReflecMeas.ScanParams._SR.iLens].szName;


	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// ï¿½ì�¬ï¿½É�ï¿½ï¿½s
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, 20, SWP_NOMOVE);

	///// Static Control /////
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_REFLECTANCE_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// Control Initialize /////
	GetDlgItem(IDC_ABSOLUTE_MEASUREMENT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RELATIVE_MEASUREMENT)->ShowWindow(SW_HIDE);
	if ( !m_SrConfig.bUv )
		GetDlgItem(IDC_ENHANCED_UV)->ShowWindow(SW_HIDE);

	/// Test Button ///
	m_TestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TestButton.DrawFlatFocus(TRUE);

	///// ï¿½ï¿½ï¿½è”½ï¿½Ë—ï¿½ï¿½gï¿½ï¿½ï¿½İ’ï¿½ /////
	int IDC_MEAS_REFL_WAVE_LENG[] = {IDC_MEAS_REFL_WAVE_LENG1, IDC_MEAS_REFL_WAVE_LENG2, IDC_MEAS_REFL_WAVE_LENG3};
	for ( int i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
		if (m_ReflecMeas.ScanParams._SR.iSpecificWavelen[i] != 0)
			m_strSpecificWavelen[i].Format("%d", m_ReflecMeas.ScanParams._SR.iSpecificWavelen[i]);
		else
			m_strSpecificWavelen[i].Empty();

		SetDlgItemText(IDC_MEAS_REFL_WAVE_LENG[i], m_strSpecificWavelen[i]);
	}

	///// Combo Box Initialize /////
	InitCombo_SrReflOpticalFilter();
	InitCombo_Lens();
	InitCombo_MeasReflReferenceMaterial();
// 2015.03.16 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
//	OnCheckReferenceLifeTime();
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_MEAS_REFL_CHK_WARN_LIFETIME)->ShowWindow(SW_HIDE);
	}
	else{
		OnCheckReferenceLifeTime();
	}
// 2015.03.16 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--

	// ï¿½Rï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	((CEdit*)GetDlgItem(IDC_REFLECTANCE_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REFLECTANCE_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REFLECTANCE_COMMENT)->ShowWindow(SW_HIDE);
		//Saiki 20090410 Add ----->
		GetDlgItem(IDC_TEST_BUTTON)->ShowWindow(SW_HIDE);
		//Saiki 20090410 Add <-----

	}
/* added 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- { ---------- */
	RECT l_rectThisView;	this->GetWindowRect(&l_rectThisView);
	RECT l_rectComment;	this->GetDlgItem(IDC_REFLECTANCE_COMMENT)->GetWindowRect(&l_rectComment);
	RECT l_rectThisDlg = {0, 0, 605, 551};
	((CDialog*) this)->MapDialogRect(&l_rectThisDlg);
	long l_lViewBottom = l_rectThisView.top + (l_rectThisDlg.bottom - l_rectThisDlg.top + 1);
	long l_lSpaceCommentBottom = l_lViewBottom - l_rectComment.bottom;
	if ((g_lModelType == MODEL_T3100) && (l_rectThisView.bottom <= (l_rectComment.bottom + l_lSpaceCommentBottom))) {
		long l_ldeltaY = l_rectThisView.bottom - l_lSpaceCommentBottom - (l_rectComment.bottom - l_rectComment.top);
		l_ldeltaY = l_rectComment.top - l_ldeltaY;
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
		POINT l_ptStart = {0, 0};
		/* ï¿½Rï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½	*/
		RECT l_rectCommentStatic;	this->GetDlgItem(IDC_COMMENT_STATIC)->GetWindowRect(&l_rectCommentStatic);
		l_ptStart.x = l_rectCommentStatic.left;
		l_ptStart.y = l_rectCommentStatic.top - l_ldeltaY;
		this->ScreenToClient(&l_ptStart);
		this->GetDlgItem(IDC_COMMENT_STATIC)->SetWindowPos(0, l_ptStart.x, l_ptStart.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		/* ï¿½Rï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½	*/
		l_ptStart.x = l_rectComment.left;
		l_ptStart.y = l_rectComment.top - l_ldeltaY;
		this->ScreenToClient(&l_ptStart);
		this->GetDlgItem(IDC_REFLECTANCE_COMMENT)->SetWindowPos(0, l_ptStart.x, l_ptStart.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
/* added 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- } ---------- */
}

// =========================================================================
// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½^ï¿½Cï¿½ï¿½ ï¿½`ï¿½Fï¿½bï¿½Nï¿½Iï¿½ï¿½ï¿½ï¿½
//
void CRecipeMeasurementSrReflectanceProgramView::OnCheckReferenceLifeTime()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_MEAS_REFL_LIFETIME)->EnableWindow(m_ReflecMeas.Ref.hdr.bWarnFlag);
}

// =========================================================================
// ï¿½Iï¿½vï¿½eï¿½Bï¿½Jï¿½ï¿½ï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½[ ï¿½Rï¿½ï¿½ï¿½{ï¿½Iï¿½ï¿½ï¿½ï¿½
//
void CRecipeMeasurementSrReflectanceProgramView::OnSelchangeSrMeasReflOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_ReflecMeas.ScanParams._SR.wOpticsFilterType = static_cast<WORD>(pCombo->GetItemData(nSelect));
}

// =========================================================================
//
void CRecipeMeasurementSrReflectanceProgramView::LoadRecipeData()
{
	CString l_strBuffer;

	switch ( m_nOpenMode )
	{
	case modeNew:				// ï¿½ï¿½ï¿½Vï¿½sï¿½Vï¿½Kï¿½ì�¬
	case modeDefaultSetting:	// ï¿½Rï¿½ï¿½ï¿½tï¿½Bï¿½Oï¿½ï¿½Ê‚ï¿½ï¿½ï¿½Jï¿½Ú‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Pï¿½[ï¿½X
		if ( !RecipeFile_LoadRecipe(&m_ReflecMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_REFLECTANCE) ) {
			m_ReflecMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
			m_ReflecMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_REFLECTANCE;
		}
		break;
	default:					// ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½Ê‚ï¿½ï¿½ï¿½Jï¿½Ú‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Pï¿½[ï¿½X
		if ( !RecipeFile_LoadRecipe(&m_ReflecMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) { 		// RECIPE_FILE_MEASUREMENT_PROGRAM ï¿½Å�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ç‚µï¿½ï¿½...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ReflecMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_REFLECTANCE) ) {
				m_ReflecMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
				m_ReflecMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_REFLECTANCE;
			}
		}
		break;
	}
	::CopyMemory(&m_OldReflecMeas, &m_ReflecMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_ReflecMeas.hdr.szName, l_strBuffer);
	}
}

// =========================================================================
//
void CRecipeMeasurementSrReflectanceProgramView::InitCombo_SrReflOpticalFilter()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER);

	pCombo->ResetContent();

	int nIndex;
	int iFilterIndex;

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	LPCTSTR pszFilterName[LANGUAGE_MAX][OPT_FILTER_MAX] = {
		{OPTICAL_FILTER_TYPE_ITEM_ENU[0], OPTICAL_FILTER_TYPE_ITEM_ENU[1], OPTICAL_FILTER_TYPE_ITEM_ENU[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Enu
		{OPTICAL_FILTER_TYPE_ITEM_JPN[0], OPTICAL_FILTER_TYPE_ITEM_JPN[1], OPTICAL_FILTER_TYPE_ITEM_JPN[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Jpn
	};

	for ( int iOptItem = 0; iOptItem < OPT_FILTER_MAX; iOptItem++ ) {
		switch ( iOptItem ) {
		case OPT_FILTER_UNKNOWN:
			break;
		case OPT_FILTER_OPEN:
			nIndex = pCombo->AddString(pszFilterName[l_SystemConfig.nLanguage][iOptItem]);
			pCombo->SetItemData(nIndex, iOptItem);
			break;
		case OPT_FILTER_DARK:
			break;
		case OPT_FILTER_POS1:
		case OPT_FILTER_POS2:
		case OPT_FILTER_POS3:
			iFilterIndex = iOptItem - 3;
			if ( m_srFilter[iFilterIndex].bEnable ) {
				nIndex = pCombo->AddString(m_srFilter[iFilterIndex].szName);
				pCombo->SetItemData(nIndex, iOptItem);
			}
			break;
		default:
			break;
		}
	}

	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½Å”ï¿½ï¿½Ë—ï¿½ï¿½ï¿½ï¿½ï¿½PGMï¿½Ì�ê�‡
	LoadStringML(IDS_CAPTION_SR_REFLECTANCE, l_strBuffer, "SR REFLECTANCE");
// 2015.03.16 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
//	if (m_SrConfig.bAutoFilter && m_strCaption == l_strBuffer){
	if ((g_lModelType == MODEL_T3100) ||
	    (m_SrConfig.bAutoFilter && m_strCaption == l_strBuffer)){
// 2015.03.16 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
		if ((m_ReflecMeas.ScanParams._SR.wOpticsFilterType < 0) || (m_ReflecMeas.ScanParams._SR.wOpticsFilterType >= OPT_FILTER_MAX)) {
			nIndex = 0;
		} else {
			nIndex = pCombo->FindStringExact(-1, pszFilterName[l_SystemConfig.nLanguage][m_ReflecMeas.ScanParams._SR.wOpticsFilterType]);
		}

		if(!(nIndex == CB_ERR)){
			pCombo->SetCurSel(nIndex);
		}
		else{
			if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
				nIndex = 0;
				pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus SR --{--
				m_ReflecMeas.ScanParams._SR.wOpticsFilterType = static_cast<WORD>(pCombo->GetItemData(0));
// 2009.12.14 bagus SR --}--
			}
			else{
				nIndex = 0;
				pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus SR --{--
				m_ReflecMeas.ScanParams._SR.wOpticsFilterType = static_cast<WORD>(pCombo->GetItemData(0));
// 2009.12.09 bagus SR --}--
				SaveRecipeData();
				LoadStringML(IDS_OPTICAL_FILTER_ZERO, l_strBuffer, "Invalid for the specified item, Optical filter is saved as an INDEX to 0.");
				LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
				MessageBox(l_strBuffer, l_strTitle, MB_OK);
			}
		}
	}
	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½È‚ï¿½ï¿½Aï¿½Ü‚ï¿½ï¿½Í“ï¿½ï¿½ß—ï¿½ï¿½ï¿½ï¿½ï¿½PGMï¿½Ì�ê�‡
	else{
		//Saiki 20091015 Add ----->
		pCombo->SetCurSel(0);
		//Saiki 20091015 Add <-----
		m_ReflecMeas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER)->ShowWindow(SW_HIDE);
// 2009.10.02 bagus Gantry --{--
		GetDlgItem(IDC_ENHANCED_UV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_REFL_REF_MATERIAL_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_REFL_REF_MATERIAL)->ShowWindow(SW_HIDE);
// 2009.10.02 bagus Gantry --}--
	}
}

// =========================================================================
//
BOOL CRecipeMeasurementSrReflectanceProgramView::CheckData()
{
	CString l_strBuffer, l_strTitle;

	//2010.01.15 bagus --{--
	//ï¿½`ï¿½Fï¿½bï¿½Nï¿½Å‚Í‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½É‚Í�ï¿½ï¿½É�iï¿½Ü‚È‚ï¿½
	//UpdateData(TRUE);
	if(!UpdateData(TRUE)){
		return FALSE;
	}
	//2010.01.15 bagus --}--

	CString strBuffer;
	CString strCaption;
	int MinWaveLength;
// 2009.05.15 k-matsuo -->
	int MaxWaveLength;
// 2009.05.15 k-matsuo <--
	int iValue;
	BOOL bValidWaveLen = FALSE;
	int i;
	int j;

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		LoadStringML(IDS_CAPTION_DEF_SETTING, l_strBuffer, "DEFAULT SETTING - ");
		strCaption = l_strBuffer;
		break;
	default:
		LoadStringML(IDS_CAPTION_MEASURE_PROG, l_strBuffer, "MEASUREMET PROGRAM - ");
		strCaption = l_strBuffer;
		break;
	}
	strCaption += m_strCaption;

	///// Lens /////
	if ( m_strLens.IsEmpty() )
	{
		LoadStringML(IDS_SELECT_LENS, l_strBuffer, "Please Select Lens.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_MEAS_REFL_LENS)->SetFocus();
		return FALSE;
	}

	///// Optical Filter /////
	
	if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH512 ) {
		// ï¿½Wï¿½ï¿½ï¿½wï¿½bï¿½hï¿½wï¿½è��ï¿½Í�ANDFilterï¿½ï¿½ï¿½pï¿½ï¿½ï¿½ï¿½Æ�Aï¿½ï¿½ï¿½Ê‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚½ï¿½ß�Aï¿½gï¿½pï¿½ï¿½ï¿½È‚ï¿½ï¿½æ‚¤ï¿½xï¿½ï¿½
		if ( m_ReflecMeas.ScanParams._SR.wOpticsFilterType == OPT_FILTER_POS2 ||
			  m_ReflecMeas.ScanParams._SR.wOpticsFilterType == OPT_FILTER_POS3)
		{
			LoadStringML(IDS_NOMALHEAD_FILTER, l_strBuffer, "If LAH512 head, select Yellow Filter or Open Necessary.");
			MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER)->SetFocus();
			return FALSE;
		}
	} else if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS ) {
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½wï¿½bï¿½hï¿½Íƒï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½É‚ï¿½ï¿½Aï¿½gï¿½pï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½ğ�§Œï¿½ï¿½ï¿½ï¿½ï¿½æ‚¤ï¿½xï¿½ï¿½
		if ( m_strLens == "5 X" || m_strLens == "10 X" ) {
			if ( m_ReflecMeas.ScanParams._SR.wOpticsFilterType != OPT_FILTER_POS2 &&
				 m_ReflecMeas.ScanParams._SR.wOpticsFilterType != OPT_FILTER_POS3)
			{
				LoadStringML(IDS_HIGHRESO_HEAD_LOWMAGNI_LENS_FILTER, l_strBuffer, "If Select 5X or 10X Lens, select ND30 Filter or ND + Yellow Filter Necessary.");
				MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
				GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER)->SetFocus();
				return FALSE;
			}
		} else {
			if ( m_ReflecMeas.ScanParams._SR.wOpticsFilterType == OPT_FILTER_POS2 ||
				 m_ReflecMeas.ScanParams._SR.wOpticsFilterType == OPT_FILTER_POS3)
			{
				LoadStringML(IDS_HIGHRESO_HEAD_HIGHMAGNI_LENS_FILTER, l_strBuffer, "If Select 50X or 100X Lens, select Yellow Filter or Open Necessary.");
				MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
				GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER)->SetFocus();
				return FALSE;
			}
		}
	}

	///// Integration Time /////
	if ( m_ReflecMeas.ScanParams._SR.dIntegTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_ReflecMeas.ScanParams._SR.dIntegTime )
	{
		//2010.01.13 bagus ï¿½Cï¿½ï¿½ --{--
		//LoadStringML(IDS_INTEGTIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.1f - %.1f)");
		LoadStringML(IDS_INTEGTIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.2f - %.2f)");
		//2010.01.13 bagus ï¿½Cï¿½ï¿½ --}--
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_MEAS_REFL_INTEGRATION_TIME)->SetFocus();
		return FALSE;
	}

	if ( m_ReflecMeas.ScanParams._SR.bEnhancedUv )
		MinWaveLength = MIN_ENHANCED_UV_WAVELENGTH;
	else
		MinWaveLength = MIN_TARGET_WAVELENGTH;

// 2009.05.15 k-matsuo -->
	if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
		MaxWaveLength = MAX_TARGET_WAVELENGTH;
	else
		MaxWaveLength = MAX_TARGET_WAVELENGTH_800;
// 2009.05.15 k-matsuo <--


	///// ï¿½ï¿½ï¿½è”½ï¿½Ë—ï¿½ï¿½gï¿½ï¿½ï¿½İ’ï¿½ /////
	///// Reflectance Measurement /////
	///// 1st Target WaveLength /////
	///// 2nd Target WaveLength /////
	///// 3rd Target WaveLength /////
	int IDC_MEAS_REFL_WAVE_LENG[] = {IDC_MEAS_REFL_WAVE_LENG1, IDC_MEAS_REFL_WAVE_LENG2, IDC_MEAS_REFL_WAVE_LENG3};
	int IDS_WAVELENGTH[] = {IDS_FIRST_WAVE_OUT_RANGE, IDS_SECOND_WAVE_OUT_RANGE, IDS_THIRD_WAVE_OUT_RANGE};
	LPCTSTR pszWaveLengthString[] = {"1st Target WaveLength is out of range.\n(%d - %d)", "2nd Target WaveLength is out of range.\n(%d - %d)", "3rd Target WaveLength is out of range.\n(%d - %d)"};

	//ï¿½Rï¿½Â‚Ì�İ’ï¿½lï¿½Ì�ï¿½Ê‚ï¿½ï¿½ï¿½ï¿½İ’ï¿½Ì�ê�‡ï¿½Í�ï¿½Ê‚É‹lï¿½ß‚ï¿½B
	CStringArray strSpecificWavelen;
	strSpecificWavelen.SetSize(SPECIFIED_WAVELENGTH_MAX);
	for ( i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
		strSpecificWavelen[i] = m_strSpecificWavelen[i];
		strSpecificWavelen[i].TrimLeft();
		strSpecificWavelen[i].TrimRight();
	}

	// ï¿½ï¿½Ê‚É‹lï¿½ß‚ï¿½
	j = 0;
	for ( i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
		if ( !strSpecificWavelen[i].IsEmpty() )
			m_strSpecificWavelen[j++] = strSpecificWavelen[i];
	}
	// CleanUp
	for ( ; j < SPECIFIED_WAVELENGTH_MAX; j++ ) {
		m_strSpecificWavelen[j].Empty();
	}
	for ( i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
		SetDlgItemText(IDC_MEAS_REFL_WAVE_LENG[i], m_strSpecificWavelen[i]);
	}

	// Check
	for ( i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
		if ( !m_strSpecificWavelen[i].IsEmpty() )	{
			iValue = atoi(m_strSpecificWavelen[i]);
			if ((!CheckTransData(iValue,m_strSpecificWavelen[i])) || (iValue < MinWaveLength) || (MaxWaveLength < iValue )) { /**ï¿½ï¿½ï¿½ï¿½ï¿½Å�Aï¿½^ï¿½[ï¿½Qï¿½bï¿½gï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ÍˆÍ‚É�İ’è‚³ï¿½ï¿½Ä‚ï¿½ï¿½é‚©ï¿½H*/
				if(CheckTransData(iValue,m_strSpecificWavelen[i])) {
					LoadStringML(IDS_WAVELENGTH[i], l_strBuffer, pszWaveLengthString[i]);
					strBuffer.Format(l_strBuffer, MinWaveLength, MaxWaveLength );
				}
				else {
					LoadStringML(IDS_SET_INTERVAL_NM, l_strBuffer, "Please set it at intervals of 1 nm.");
					strBuffer.Format(l_strBuffer);
				}
				MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
				GetDlgItem(IDC_MEAS_REFL_WAVE_LENG[i])->SetFocus();
				return FALSE;
			}
			else {
				bValidWaveLen = TRUE;
			}
		}
	}

	if ( !bValidWaveLen ) {
		LoadStringML(IDS_INPUT_WAVELENGTH, l_strBuffer, "Please Input at Least One or More Wavelength.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	else{
		for ( int i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
			m_ReflecMeas.ScanParams._SR.iSpecificWavelen[i] = atoi(m_strSpecificWavelen[i]);
		}
	}

	///// Life Time /////
// 2015.03.16 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	if(g_lModelType == MODEL_T3100){
		if ( m_ReflecMeas.Ref.hdr.dLifeTime < MIN_LIFE_TIME || MAX_LIFE_TIME < m_ReflecMeas.Ref.hdr.dLifeTime ) 	{
			LoadStringML(IDS_LIFETIME_OUT_RANGE, l_strBuffer, "Life Time is out of range.""\n(%.1f - %.1f)");
			strBuffer.Format(l_strBuffer, MIN_LIFE_TIME, MAX_LIFE_TIME);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_LIFETIME)->SetFocus();
			return FALSE;
		}
		if((m_ReflecMeas.Ref.hdr.dLifeTime - (int)m_ReflecMeas.Ref.hdr.dLifeTime) != 0) //ï¿½ï¿½ï¿½ï¿½ï¿½Å‚È‚ï¿½
		{
			LoadStringML(IDS_SET_INTERVAL_MIN, l_strBuffer, "Please set it at intervals of 1 min.");
			strBuffer.Format(l_strBuffer);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_LIFETIME)->SetFocus();
			return FALSE;
		}
	}
	else {
// 2015.03.16 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if ( m_ReflecMeas.Ref.hdr.bWarnFlag)	{
		if ( m_ReflecMeas.Ref.hdr.dLifeTime < MIN_LIFE_TIME || MAX_LIFE_TIME < m_ReflecMeas.Ref.hdr.dLifeTime ) 	{
			LoadStringML(IDS_LIFETIME_OUT_RANGE, l_strBuffer, "Life Time is out of range.""\n(%.1f - %.1f)");
			strBuffer.Format(l_strBuffer, MIN_LIFE_TIME, MAX_LIFE_TIME);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_LIFETIME)->SetFocus();
			return FALSE;
		}
		if((m_ReflecMeas.Ref.hdr.dLifeTime - (int)m_ReflecMeas.Ref.hdr.dLifeTime) != 0) //ï¿½ï¿½ï¿½ï¿½ï¿½Å‚È‚ï¿½
		{
			LoadStringML(IDS_SET_INTERVAL_MIN, l_strBuffer, "Please set it at intervals of 1 min.");
			strBuffer.Format(l_strBuffer);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_MEAS_REFL_LIFETIME)->SetFocus();
			return FALSE;
		}
	}
// 2015.03.16 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2015.03.16 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	///// Reference Strategy /////
	if ( strlen(m_ReflecMeas.Ref.OptDesc.szRefMaterial) == 0)
	{
		LoadStringML(IDS_NOT_SELECT_REFER_STRATEGY, l_strBuffer, "The reference strategy is not selected");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_MEAS_REFL_REF_MATERIAL)->SetFocus();
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrReflectanceProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldReflecMeas, &m_ReflecMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrReflectanceProgramView::SaveRecipeData()
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
// 2009.10.09 K.Matsuo Delete <--

// 2015.03.16 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	if(g_lModelType == MODEL_T3100){
		m_ReflecMeas.Ref.hdr.bWarnFlag = FALSE;
	}
// 2015.03.16 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_ReflecMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_REFLECTANCE) )
			return FALSE;
		break;
	default:
		// ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½İ’ï¿½iSave, SaveAsï¿½ï¿½ï¿½pï¿½j
		strcpy(m_ReflecMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ReflecMeas, m_szRecipeName, RECIPE_FILE_SR_REFLECTANCE) )
			return FALSE;
/* added 2015.04.24 hmenjo Ref File ï¿½í�œ1 ---------- { ---------- */
		if ((g_lModelType == MODEL_T3100) &&
			(
				(m_nOpenMode == modeNew) ||
				((m_nOpenMode == modeNormal) && (
							(m_ReflecMeas.ScanParams._SR.iLens      != m_OldReflecMeas.ScanParams._SR.iLens) ||
							(m_ReflecMeas.ScanParams._SR.dIntegTime != m_OldReflecMeas.ScanParams._SR.dIntegTime)
						))
			)
		) {
			/*	ï¿½Vï¿½K(modeNew)ï¿½Å‚Í–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É�í�œ
				ï¿½Ê�ï¿½(modeNormal)ï¿½Å‚Í�Cï¿½ï¿½ï¿½ï¿½ï¿½Y or ï¿½~ï¿½Ï�ï¿½ï¿½ï¿½(ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)ï¿½ï¿½ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½	*/
			CNanoRecipeUI::DelRefFileByUseMeasPgm(m_ReflecMeas.hdr.szName);
		}
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldReflecMeas, &m_ReflecMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrReflectanceProgramView::IsAccessPrivilege()
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
int CRecipeMeasurementSrReflectanceProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrReflectanceProgramView::CheckTransData(int iData , CString strData)
{
	CString strTransiData;

	strTransiData.Format("%d",iData);
	return strTransiData == strData;
}

// =========================================================================
// ï¿½ï¿½ï¿½ï¿½ï¿½Y ï¿½Rï¿½ï¿½ï¿½{ï¿½Iï¿½ï¿½ï¿½ï¿½
//
void CRecipeMeasurementSrReflectanceProgramView::OnSelchangeMeasReflLens()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_ReflecMeas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	UpdateData(FALSE);
}


// =========================================================================
//ï¿½ï¿½ï¿½ï¿½ï¿½Y ï¿½Rï¿½ï¿½ï¿½{ï¿½Cï¿½jï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//
void CRecipeMeasurementSrReflectanceProgramView::InitCombo_Lens()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
// 2009.11.24 bagus SR ï¿½Cï¿½ï¿½ --{--
//		if ( m_SrTurret[i].bEnable ) {
		if ( m_SrTurret[i].bEnable && m_SrTurret[i].bAnalysis ) {
// 2009.11.24 bagus SR ï¿½Cï¿½ï¿½ --}--
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	nIndex = pCombo->FindStringExact(-1, m_strLens);
	//Saiki 20090406 Change ----->
	//pCombo->SetCurSel(nIndex);
	if(!(nIndex == CB_ERR)){
		pCombo->SetCurSel(nIndex);
	}
	else{
		if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus SR --{--
			m_ReflecMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.14 bagus SR --}--
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus SR --{--
			m_ReflecMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.09 bagus SR --}--
			SaveRecipeData();
			LoadStringML(IDS_LENS_ZERO, l_strBuffer, "Invalid for the specified item, Lens is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
	//Saiki 20090406 Change <-----
}


// =========================================================================
//
void CRecipeMeasurementSrReflectanceProgramView::InitCombo_MeasReflReferenceMaterial()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_REF_MATERIAL);
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

	int nIndex = pCombo->FindStringExact(0, m_ReflecMeas.Ref.OptDesc.szRefMaterial);
	//Saiki 20090406Change ----->
	//pCombo->SetCurSel(nIndex);
	if(!(nIndex == CB_ERR)){
		pCombo->SetCurSel(nIndex);
	}
	else{
		if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
			SaveRecipeData();
			LoadStringML(IDS_REFER_STRATEGY_ZERO, l_strBuffer, "Invalid for the specified item, Reference strategy is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
	//Saiki 20090406 Change <----
}

// =========================================================================
//
void CRecipeMeasurementSrReflectanceProgramView::OnTestButton()
{

	//ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ì�¬
	if(CheckData() == FALSE) //ï¿½ï¿½ï¿½Vï¿½sï¿½fï¿½[ï¿½^ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ‚ï¿½Ä‚ï¿½ï¿½È‚ï¿½
	{
		//ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½CheckData()ï¿½ï¿½ï¿½Å�oï¿½ï¿½ï¿½B
		return; //ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½
	}
	char szRecipeNameMain[RECIPE_NAME_LEN + 1];
	char szRecipeNameMeas[RECIPE_NAME_LEN + 1];
	char szRecipeNameStage[RECIPE_NAME_LEN + 1];

	if ( !GetTestModeRecipeName(szRecipeNameMain, SAVE_AS_MAIN_RECIPE) )
		return;
	if ( !GetTestModeRecipeName(szRecipeNameMeas, SAVE_AS_MEASUREMENT_PROGRAM) )
		return;
	//2010.01.11 bagus Gantry --{--
	if(m_iNewScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
		if ( !GetTestModeRecipeName(szRecipeNameStage, SAVE_AS_STAGE_PROGRAM) )
			return;
	}else{
		memset(szRecipeNameStage,NULL,sizeof(szRecipeNameStage));
	}
	//2010.01.11 bagus Gantry --}--

	//2010.01.11 bagus Gantry --{--
	//BOOL bMainRcpMakeFlg = MakeTempSrMainRecipe(szRecipeNameMain, szRecipeNameMeas); //ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ì�¬
	BOOL bMainRcpMakeFlg = MakeTempSrMainRecipe(szRecipeNameMain, szRecipeNameMeas, szRecipeNameStage); //ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ì�¬
	//2010.01.11 bagus Gantry --}--
	BOOL bMeasRcpMakeFlg = MakeTempSrMeasurementRecipe(szRecipeNameMeas); //ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ì�¬
	//2010.01.11 bagus Gantry --{--
	BOOL bStageRcpMakeFlg = TRUE;
	if(m_iNewScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
		bStageRcpMakeFlg = MakeTempSrGantryStageRecipe(szRecipeNameStage); //ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ì�¬
	}
	//2010.01.11 bagus Gantry --}--

	//ï¿½wï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½xï¿½wï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½xï¿½wï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Vï¿½sï¿½xï¿½wï¿½\ï¿½ï¿½ï¿½pï¿½ï¿½ï¿½xï¿½ï¿½ï¿½xï¿½wï¿½Pï¿½Ê–ï¿½ï¿½xï¿½Ì‚İŠiï¿½[
	//ï¿½ï¿½ï¿½ï¿½Ñ‰ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Waveï¿½Íˆï¿½
	TempRecipeDocSet(szRecipeNameMain);

	//ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½O
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	pDoc->OperationLogging("Measurement Operation - TEST Button was pushed.");

	pMainFrame->m_iNowPoint = 0;
	pDoc->SetMeasMode(MEASMODE_TEST); //ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½h

	CTestMeasurementDlg dlg;
	CComboBox* pCombo;
	int nIndex;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	LPCTSTR pszFilterName[LANGUAGE_MAX][OPT_FILTER_MAX] = {
		{OPTICAL_FILTER_TYPE_ITEM_ENU[0], OPTICAL_FILTER_TYPE_ITEM_ENU[1], OPTICAL_FILTER_TYPE_ITEM_ENU[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Enu
		{OPTICAL_FILTER_TYPE_ITEM_JPN[0], OPTICAL_FILTER_TYPE_ITEM_JPN[1], OPTICAL_FILTER_TYPE_ITEM_JPN[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Jpn
	};

// 2010.01.07 bagus Gantry --{--
	CTestMeasurementGantryDlg dlg2;

	if ((m_iNewHeadType == HEAD_TYPE_SR) && (m_iNewScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)) {
		// Optical Filter
		pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER);
		nIndex = pCombo->GetCurSel();
		dlg2.m_strOpticalFilter = pszFilterName[l_SystemConfig.nLanguage][pCombo->GetItemData(nIndex)];

		// Lens
		pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_LENS);
		nIndex = pCombo->GetCurSel();
		pCombo->GetLBText(nIndex, dlg2.m_strLens);
		// Integration Time
		dlg2.m_dIntegTime = m_ReflecMeas.ScanParams._SR.dIntegTime;
		dlg2.m_wStart = m_ReflecMeas.ScanParams._SR.WavelenRange.wStart;
		dlg2.m_wEnd = m_ReflecMeas.ScanParams._SR.WavelenRange.wEnd;

		if( dlg2.DoModal() == IDOK ){
			UpdateData(TRUE);

			m_strLens = dlg2.m_strLens;
			m_strOptFilter = dlg2.m_strOpticalFilter;
			m_ReflecMeas.ScanParams._SR.dIntegTime = dlg2.m_dIntegTime;
			m_ReflecMeas.ScanParams._SR.WavelenRange.wStart = dlg2.m_wStart;
			m_ReflecMeas.ScanParams._SR.WavelenRange.wEnd = dlg2.m_wEnd;

			UpdateData(FALSE);
		}
	} else {
// 2010.01.07 bagus Gantry --}--
	// Optical Filter
	pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_COMBO_OPT_FILTER);
	nIndex = pCombo->GetCurSel();
	dlg.m_strOpticalFilter = pszFilterName[l_SystemConfig.nLanguage][pCombo->GetItemData(nIndex)];

	// Lens
	pCombo = (CComboBox*)GetDlgItem(IDC_MEAS_REFL_LENS);
	nIndex = pCombo->GetCurSel();
//	  dlg.m_strLens = m_SrTurret[nIndex].szName; //ï¿½ï¿½ï¿½~
	pCombo->GetLBText(nIndex, dlg.m_strLens);
	// Integration Time
	dlg.m_dIntegTime = m_ReflecMeas.ScanParams._SR.dIntegTime;
	dlg.m_wStart = m_ReflecMeas.ScanParams._SR.WavelenRange.wStart;
	dlg.m_wEnd = m_ReflecMeas.ScanParams._SR.WavelenRange.wEnd;

	if( dlg.DoModal() == IDOK ){
		UpdateData(TRUE);

		m_strLens = dlg.m_strLens;
		m_strOptFilter = dlg.m_strOpticalFilter;
		m_ReflecMeas.ScanParams._SR.dIntegTime = dlg.m_dIntegTime;
		m_ReflecMeas.ScanParams._SR.WavelenRange.wStart = dlg.m_wStart;
		m_ReflecMeas.ScanParams._SR.WavelenRange.wEnd = dlg.m_wEnd;

		UpdateData(FALSE);
	}
// 2010.01.07 bagus Gantry --{--
	}
// 2010.01.07 bagus Gantry --}--
}

// =========================================================================
//ï¿½_ï¿½~ï¿½[ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�¬
BOOL CRecipeMeasurementSrReflectanceProgramView::MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName)
{
	return RecipeFile_SaveRecipe(&m_ReflecMeas, pszRecipeName, RECIPE_FILE_SR_REFLECTANCE);
}

// =========================================================================
//ï¿½_ï¿½~ï¿½[ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�¬
BOOL CRecipeMeasurementSrReflectanceProgramView::MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas, LPCTSTR pszRecipeNameStage)
{
	MAIN_RCP_INFO MainRcpInfo_Temp;
	memset(&MainRcpInfo_Temp, 0, sizeof(MainRcpInfo_Temp));


#if 0 //ï¿½İ’è‚ªï¿½Kï¿½vï¿½È‚ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚±ï¿½ï¿½
	MainRcpInfo_Temp.hdr.szComment;
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szStage, "---");
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szPointDeskew, "---");
	MainRcpInfo_Temp.MainRcpParam.hdr.nPointManualAdjustment;
	MainRcpInfo_Temp.MainRcpParam.hdr.bSampleID = FALSE;
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szSampleID, "---");
	MainRcpInfo_Temp.MainRcpParam.hdr.bSaveMeasResult = FALSE;
	MainRcpInfo_Temp.MainRcpParam.hdr.bSaveOverwrite = FALSE;
	MainRcpInfo_Temp.MainRcpParam._SR.RecalibItem[0] = 0; //
	strcpy(MainRcpInfo_Temp.MainRcpParam._SR.szRecalib[0], "---"); //
	MainRcpInfo_Temp.MainRcpParam._SR.nAutoFocusFailOption = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.nFinalFocusFailDataValidOption = 0;

	MainRcpInfo_Temp.MainRcpParam._SR.nDefaultDisplay = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.bMeasRef = FALSE;
#endif

	strcpy(MainRcpInfo_Temp.hdr.szName, pszRecipeNameMain);
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szMeas, pszRecipeNameMeas);
	//2010.01.11 bagus Gantry --{--
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szStage, pszRecipeNameStage);
	//2010.01.11 bagus Gantry --}--
	MainRcpInfo_Temp.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
	MainRcpInfo_Temp.MainRcpParam._SR.nFocus = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.bRefWithAF = TRUE;

	return RecipeFile_SaveRecipe(&MainRcpInfo_Temp, pszRecipeNameMain, RECIPE_FILE_SR_MAIN_RECIPE);
}
//2010.01.11 bagus Gantry --{--
BOOL CRecipeMeasurementSrReflectanceProgramView::MakeTempSrGantryStageRecipe(LPCTSTR pszRecipeName)
{
	STAGE_PROG_INFO_HDR StageInfo_Temp;
	BOOL bRet;
	SR_REFERENCE_POSITION l_SrReferencePosition;
	SR_CONFIG	l_SrConfig;
	STAGE_COORD		l_ScanPoint[SCAN_POINT_MAX];

	memset(&StageInfo_Temp,NULL,sizeof(StageInfo_Temp));
	memset(&l_SrReferencePosition, 0, sizeof(l_SrReferencePosition));
	memset(&l_SrConfig, 0, sizeof(l_SrConfig));
	memset(l_ScanPoint,NULL,sizeof(l_ScanPoint));
	ConfigFile_GetNanoSpecIni(&l_SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);
	ConfigFile_GetNanoSpecIni(&l_SrConfig, CONFIG_FILE_SR_CONFIG);

	StageInfo_Temp.wHeadType = STAGE_PGM_TYPE_STANDARD;
	StageInfo_Temp.wScanType = SCAN_TYPE_SR_GANTRY;
	StageInfo_Temp.wNumScans = l_SrConfig.wGantryNumOfMeasPoint;
	strcpy(StageInfo_Temp.hdr.szName,pszRecipeName);
	for(int n = 0;n < l_SrConfig.wGantryNumOfMeasPoint;n++){
		l_ScanPoint[n].lX = l_SrReferencePosition.LocGantryTransmitReference[n].lX;
		l_ScanPoint[n].lY = l_SrReferencePosition.LocGantryTransmitReference[n].lY;
	}

	bRet = RecipeFile_SaveRecipe(&StageInfo_Temp, pszRecipeName, RECIPE_FILE_STAGE_PROGRAM);
	RecipeFile_SavePointList(l_ScanPoint, StageInfo_Temp.wNumScans, pszRecipeName);
	return TRUE;
}
//2010.01.11 bagus Gantry --}--

// =========================================================================
//ï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½NanoSpecDocï¿½ÖŠiï¿½[
BOOL CRecipeMeasurementSrReflectanceProgramView::TempRecipeDocSet(LPCTSTR pszRecipeName)
{
//ï¿½wï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½xï¿½wï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½xï¿½wï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Vï¿½sï¿½xï¿½wï¿½\ï¿½ï¿½ï¿½pï¿½ï¿½ï¿½xï¿½ï¿½ï¿½xï¿½wï¿½Pï¿½Ê–ï¿½ï¿½xï¿½Ì‚İŠiï¿½[
//ï¿½ï¿½ï¿½ï¿½Ñ�wï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Waveï¿½ÍˆÍ�xï¿½Aï¿½wï¿½ï¿½ï¿½ï¿½ï¿½Ê�x

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	///// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ /////
	pDoc->InitMeasData();


	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	rcp_data.MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_REFLECTANCE;


	//Waveï¿½ÍˆÍƒZï¿½bï¿½g
	rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wStart = MIN_TARGET_WAVELENGTH;
// 2009.05.15 k-matsuo -->
//	rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd = MAX_TARGET_WAVELENGTH;
	if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
		rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd = MAX_TARGET_WAVELENGTH;
	else
		rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd = MAX_TARGET_WAVELENGTH_800;
// 2009.05.15 k-matsuo <--


	strcpy(rcp_data.szRecipeName, pszRecipeName);
	//ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½NanoSpecDocï¿½Ö‚ÌŠiï¿½[
	//2010.01.11 bagus Gantry --{--
	BOOL bMain = FALSE;
	BOOL bMeas = FALSE;
	BOOL bStage = FALSE;
	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bMeas = RecipeFile_LoadRecipe(&rcp_data.MeasProgInfo, rcp_data.MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
	if((bMain == FALSE) || (bMeas == FALSE))
	{
		return FALSE;
	}
	if(m_iNewScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
		bStage = RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
	}
	//2010.01.11 bagus Gantry --}--
	pDoc->SetRcpData(&rcp_data);


	///// ï¿½Pï¿½Ê–ï¿½ï¿½æ“¾ /////
	SR_FIGURE_FORMAT SrFigureFormat;
	char szUnit[MAX_PATH];
	memset(szUnit, 0, sizeof(szUnit));

	ConfigFile_GetNanoSpecIni(&SrFigureFormat, CONFIG_FILE_SR_FIGURE_FORMAT);

	int nUnit = SrFigureFormat.nThicknessUnit;
	if(nUnit >= 0 && nUnit < FIGURE_UNIT_SR_THICKNESS_MAX){
		strcpy(szUnit, g_lpszFigureUnitSrThickness[nUnit]);
	}
	else{
		strcpy(szUnit, " ");
	}
	pDoc->SetUnitName(szUnit);
	pDoc->SetUnitIndex(nUnit);


	///// ï¿½ï¿½Ê•\ï¿½ï¿½(ï¿½fï¿½[ï¿½^ï¿½Û‘ï¿½)ï¿½pï¿½^ï¿½Cï¿½gï¿½ï¿½ /////
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];		//ï¿½ï¿½Ê•\ï¿½ï¿½(ï¿½fï¿½[ï¿½^ï¿½Û‘ï¿½)ï¿½pï¿½^ï¿½Cï¿½gï¿½ï¿½ ï¿½ï¿½X,Y,Z,AFï¿½ÍŠÜ‚Ü‚ï¿½
	int iDispLabelCount;
	memset(szDispLabel, 0, sizeof(szDispLabel)); //

	//2010.01.11 bagus GTR --{--
	if(m_iNewScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
		//ï¿½Kï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ß—ï¿½ï¿½Ìƒeï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½Í‘ï¿½ï¿½è�€ï¿½Ú‚ï¿½ï¿½È‚ï¿½
		iDispLabelCount = 0;
	}else{
		pMainFrame->GetStrategyhead(m_ReflecMeas.ScanParams._SR.XMPDesc.szStrategyEntry, szDispLabel, &iDispLabelCount);
	}
	//2010.01.11 bagus GTR --}--
	pDoc->SetDispLabel(szDispLabel);
	pDoc->SetDispLabelCount(iDispLabelCount);


	return TRUE;
}

// =========================================================================
//ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ì�¬
BOOL CRecipeMeasurementSrReflectanceProgramView::GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType)
{
	CString l_strBuffer, l_strTitle;

	char szRcpName[RECIPE_NAME_LEN + 1];
	char szRcpNamePath[MAX_PATH + 1];

	sprintf(szRcpName, "%c%s", SYSTEM_RECIPE_BEGINNING_CHAR, TESTMODE_TEMP_RECIPE_NAME); //ï¿½ì�¬ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½

	//ï¿½ì�¬ï¿½ï¿½ï¿½éƒŒï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½×‚ï¿½×‚Ìƒpï¿½Xï¿½ï¿½ï¿½ì�¬
	if ( iRcpType == SAVE_AS_MAIN_RECIPE ) { //ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½s
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Main_Recipe_Dir, szRcpName, MAINRECIPE_EXT); //ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½p
	}
	else if(iRcpType == SAVE_AS_MEASUREMENT_PROGRAM) { //ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Measurement_Program_Dir, szRcpName, MEASUREMENTPGM_EXT); //ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½p
	}
	//2010.01.10 bagus Gantry --{--
	else if(iRcpType == SAVE_AS_STAGE_PROGRAM){	//ï¿½Xï¿½eï¿½[ï¿½Wï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Stage_Program_Dir, szRcpName, STAGEPGM_EXT); //ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½p
	}
	//2010.01.10 bagus Gantry --}--

	//ï¿½ì�¬ï¿½ï¿½ï¿½æ‚¤ï¿½Æ‚ï¿½ï¿½éƒŒï¿½Vï¿½sï¿½Ì�dï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
	if ( FileOrDirExists(szRcpNamePath)==TRUE ) { //ï¿½ì�¬ï¿½ï¿½ï¿½æ‚¤ï¿½Æ‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½É�dï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		//ï¿½\ï¿½tï¿½gï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½É‰ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½ï¿½ï¿½É‚Í—ï¿½ï¿½È‚ï¿½ï¿½Í‚ï¿½ï¿½B
		LoadStringML(IDS_CAPTION_SR_TRANSMIT, l_strBuffer, "Temporary recipe for the test mode remains, \nthe test mode cannot be begun.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		return FALSE; //ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½dï¿½ï¿½(ï¿½\ï¿½tï¿½gï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½X)
	}

	strcpy(pszFileName, szRcpName);
	return TRUE;
}

// =========================================================================
//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½İƒ`ï¿½Fï¿½bï¿½N(ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½İ�ï¿½ï¿½ï¿½TRUEï¿½ï¿½Ô‚ï¿½)
BOOL CRecipeMeasurementSrReflectanceProgramView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}
