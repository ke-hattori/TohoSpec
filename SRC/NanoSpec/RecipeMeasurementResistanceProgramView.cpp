// RecipeMeasurementResistanceProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "RecipeMeasurementResistanceProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MIN_RS_MEASUREMENT_TIME			(2)
#define MAX_RS_MEASUREMENT_TIME			(10)
#define MIN_RS_FILM_THICKNESS			(0.001)
#define MAX_RS_FILM_THICKNESS			(9999.0)
#define MIN_RS_RCF 						(0.001)
#define MAX_RS_RCF 						(9999.0)


// #########################################################################
// CRecipeMeasurementResistanceProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementResistanceProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMeasurementResistanceProgramView::CRecipeMeasurementResistanceProgramView()
	: CNanoRecipeUI(CRecipeMeasurementResistanceProgramView::IDD)
{
// Kojika 20090527 Change
//	m_strCaption = "RESISTANCE";
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_RESISTANCE, l_strBuffer, "RESISTANCE");
	m_strCaption = l_strBuffer;
//Kojika 20090527 Change End

	//{{AFX_DATA_INIT(CRecipeMeasurementResistanceProgramView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementResistanceProgramView::~CRecipeMeasurementResistanceProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementResistanceProgramView)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_RESISTANCE_PROG_NAME, m_ResisMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_FILM_THICKNESS, m_ResisMeas.ScanParams._RS.dThickness);
	DDX_Text(pDX, IDC_RCF, m_ResisMeas.ScanParams._RS.dRsCorrectFactor);
	DDX_Text(pDX, IDC_MEASUREMENT_TIME, m_ResisMeas.ScanParams._RS.dScanTime);					// m_ResisMeas.ScanParams._RS.dScanTime 要検討。ComboBoxのインデックスより、実の値をもつべきでは？
	DDX_Text(pDX, IDC_RESISTANCE_COMMENT, m_ResisMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

BEGIN_MESSAGE_MAP(CRecipeMeasurementResistanceProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementResistanceProgramView)
	ON_CBN_SELCHANGE(IDC_THICKNESS_UNIT, OnSelchangeThicknessUnit)
	ON_CBN_SELCHANGE(IDC_LIMIT_VOLTAGE, OnSelchangeLimitVoltage)
	ON_CBN_SELCHANGE(IDC_START_RANGE, OnSelchangeStartRange)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// =========================================================================
// CRecipeMeasurementResistanceProgramView メッセージ ハンドラ
//
// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::OnInitialUpdate()
{
	///// Load File /////
	LoadRecipeData();

	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return;		// 作成に失敗
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, 20, SWP_NOMOVE);

	///// Static Control /////
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_RESISTANCE_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// Control Initialize /////
	InitCombo_ThicknessUnit();
	InitCombo_LimitVoltage();
	InitCombo_StartRange();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_RESISTANCE_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RESISTANCE_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RESISTANCE_COMMENT)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::OnSelchangeThicknessUnit()
{
	UpdateData(TRUE);

	m_ResisMeas.ScanParams._RS.wUnits = ((CComboBox*)GetDlgItem(IDC_THICKNESS_UNIT))->GetCurSel();
}

// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::OnSelchangeLimitVoltage()
{
	UpdateData(TRUE);

	m_ResisMeas.ScanParams._RS.wVoltageLimit = ((CComboBox*)GetDlgItem(IDC_LIMIT_VOLTAGE))->GetCurSel();
}

// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::OnSelchangeStartRange()
{
	UpdateData(TRUE);

	m_ResisMeas.ScanParams._RS.lStartIdx = ((CComboBox*)GetDlgItem(IDC_START_RANGE))->GetCurSel();
}

// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::LoadRecipeData()
{
	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ResisMeas, RES_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_4PP) ) {
			m_ResisMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_4PP;
			m_ResisMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_4PP_MEAS;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ResisMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {			// RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ResisMeas, RES_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_4PP) ) {
				m_ResisMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_4PP;
				m_ResisMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_4PP_MEAS;
			}
		}
		break;
	}
	::CopyMemory(&m_OldResisMeas, &m_ResisMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		strcpy(m_ResisMeas.hdr.szName, "Untitled");
	}
}

// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::InitCombo_ThicknessUnit()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_THICKNESS_UNIT);
	pCombo->ResetContent();

	for ( int i = 0; i < RESISTANCE_UNIT_MAX; i++ )
		pCombo->AddString(RESISTANCE_UNIT_ITEM[i]);

// 2009.12.09 bagus --{--
	CString l_strBuffer, l_strTitle;

	if ((m_ResisMeas.ScanParams._RS.wUnits < 0) || (m_ResisMeas.ScanParams._RS.wUnits > RESISTANCE_UNIT_MAX)) {
		m_ResisMeas.ScanParams._RS.wUnits = 0;
		SaveRecipeData();
		LoadStringML(IDS_THICKNESS_UNIT_ZERO, l_strBuffer, "Invalid for the specified item, Thickness Unit is saved as an INDEX to 0.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
	}
// 2009.12.09 bagus --}--
	pCombo->SetCurSel(m_ResisMeas.ScanParams._RS.wUnits);
}

// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::InitCombo_LimitVoltage()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_LIMIT_VOLTAGE);
	pCombo->ResetContent();

	for ( int i = 0; i < RESISTANCE_LIMIT_VOLTAGE_MAX; i++)
		pCombo->AddString(RESISTANCE_LIMIT_VOLTAGE_ITEM[i]);

// 2009.12.09 bagus --{--
	CString l_strBuffer, l_strTitle;

	if ((m_ResisMeas.ScanParams._RS.wVoltageLimit < 0) || (m_ResisMeas.ScanParams._RS.wVoltageLimit > RESISTANCE_LIMIT_VOLTAGE_MAX)) {
		m_ResisMeas.ScanParams._RS.wVoltageLimit = 0;
		SaveRecipeData();
		LoadStringML(IDS_LIMIT_VOLTAGE_ZERO, l_strBuffer, "Invalid for the specified item, Limit Voltage is saved as an INDEX to 0.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
	}
// 2009.12.09 bagus --}--
	pCombo->SetCurSel(m_ResisMeas.ScanParams._RS.wVoltageLimit);
}

// =========================================================================
//
void CRecipeMeasurementResistanceProgramView::InitCombo_StartRange()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_START_RANGE);
	pCombo->ResetContent();

	CString strBuffer;
// 2009.11.04 bagus RS 追加 --{--
//	for ( int i = 7; i > (-4); i-- ) {
//		strBuffer.Format("%d", i);
	for ( int i = -3; i < 8; i++ ) {
		strBuffer.Format("%+d", i);
// 2009.11.04 bagus RS 追加 --}--
		pCombo->AddString(strBuffer);
	}

// 2009.12.09 bagus --{--
	CString l_strBuffer, l_strTitle;

	if ((m_ResisMeas.ScanParams._RS.lStartIdx - 3 < -3) || (m_ResisMeas.ScanParams._RS.lStartIdx - 3 > 8)) {
		m_ResisMeas.ScanParams._RS.lStartIdx = 0;
		SaveRecipeData();
		LoadStringML(IDS_STRAT_RANGE_ZERO, l_strBuffer, "Invalid for the specified item, Start Range is saved as an INDEX to 0.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
	}
// 2009.12.09 bagus --}--
	pCombo->SetCurSel(m_ResisMeas.ScanParams._RS.lStartIdx);
}

// =========================================================================
//
BOOL CRecipeMeasurementResistanceProgramView::CheckData()
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

	///// Film Thickness /////
	if ( m_ResisMeas.ScanParams._RS.dThickness < MIN_RS_FILM_THICKNESS || MAX_RS_FILM_THICKNESS < m_ResisMeas.ScanParams._RS.dThickness ) {
		strBuffer.Format("Film Thickness is out of range.\n(%.3lf - %.3lf)", MIN_RS_FILM_THICKNESS, MAX_RS_FILM_THICKNESS);
// Kojika 20090527 Change
//		strBuffer.Format("Film Thickness is out of range.\n(%.1f - %.1f)", MIN_FILM_THICKNESS, MAX_FILM_THICKNESS);
		LoadStringML(IDS_FILM_OUT_RANGE, l_strBuffer, "Film Thickness is out of range.\n(%.3lf - %.3lf)");
		strBuffer.Format(l_strBuffer, MIN_RS_FILM_THICKNESS, MAX_RS_FILM_THICKNESS);
// Kojika 20090527 Change End
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_FILM_THICKNESS)->SetFocus();
		return FALSE;
	}

	///// Thickness Unit /////
	if( ((CComboBox *)GetDlgItem(IDC_THICKNESS_UNIT))->GetCurSel() == CB_ERR ) {
// Kojika 20090527 Change
//		MessageBox("Please Select a Thickness Unit", strCaption, MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_THICK_UNIT, l_strBuffer, "Please Select a Thickness Unit");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
// Kojika 20090527 Change End
		return FALSE;
	}

	///// Limit Voltage /////
	if ( ((CComboBox *)GetDlgItem(IDC_LIMIT_VOLTAGE))->GetCurSel() == CB_ERR ) {
// Kojika 20090527 Change
//		MessageBox("Please Select a Limit Voltage", strCaption, MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_LIMIT_VOLT, l_strBuffer, "Please Select a Limit Voltage");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
// Kojika 20090527 Change End
		return FALSE;
	}

	///// Start Range /////
	if ( ((CComboBox*)GetDlgItem(IDC_START_RANGE))->GetCurSel() == CB_ERR ) {
// Kojika 20090527 Change
//		MessageBox("Please Select a Start Range", strCaption, MB_OK | MB_ICONINFORMATION);
		LoadStringML(IDS_SELECT_START_RANGE, l_strBuffer, "Please Select a Start Range");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
// Kojika 20090527 Change End
		return FALSE;
	}

	///// RCF /////
	if ( m_ResisMeas.ScanParams._RS.dRsCorrectFactor < MIN_RS_RCF || MAX_RS_RCF < m_ResisMeas.ScanParams._RS.dRsCorrectFactor ) {
// Kojika 20090527 Change
//		strBuffer.Format("Rs Correct Factor is out of range.\n(%.1f - %.1f)", MIN_RS_RCF, MAX_RS_RCF);
		LoadStringML(IDS_RS_OUT_RANGE, l_strBuffer, "Rs Correct Factor is out of range.\n(%.3lf - %.3lf)");
		strBuffer.Format(l_strBuffer, MIN_RS_RCF, MAX_RS_RCF);
// Kojika 20090527 Change End
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_RCF)->SetFocus();
		return FALSE;
	}

	///// Scan Time /////
	if ( m_ResisMeas.ScanParams._RS.dScanTime < MIN_RS_MEASUREMENT_TIME || MAX_RS_MEASUREMENT_TIME < m_ResisMeas.ScanParams._RS.dScanTime ) {
// Kojika 20090527 Change
//		strBuffer.Format("Scan Time is out of range.\n(%.1f - %.1f)", MIN_FILM_THICKNESS, MAX_FILM_THICKNESS);
		LoadStringML(IDS_SCANTIME_OUT_RANGE, l_strBuffer, "Scan Time is out of range.\n(%d - %d)");
		strBuffer.Format(l_strBuffer, MIN_RS_MEASUREMENT_TIME, MAX_RS_MEASUREMENT_TIME);
// Kojika 20090527 Change End
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_MEASUREMENT_TIME)->SetFocus();
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementResistanceProgramView::IsDataChanged()
{
	UpdateData(TRUE);

// 2009.11.05 bagus RS 追加 --{--
//	return memcpy(&m_OldResisMeas, &m_ResisMeas, sizeof(MEAS_PROG_INFO)) != 0;
	return memcmp(&m_OldResisMeas, &m_ResisMeas, sizeof(MEAS_PROG_INFO)) != 0;
// 2009.11.05 bagus RS 追加 --}--
}

// =========================================================================
//
BOOL CRecipeMeasurementResistanceProgramView::SaveRecipeData()
{
// 2009.09.19 bagus SE --{--
	SR_CONFIG	SrConfig;

	memset(&SrConfig, 0, sizeof(SrConfig));
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	m_ResisMeas.ScanParams._RS.iLens = SrConfig.iDefaultMeasLensNo;
	m_ResisMeas.ScanParams._RS.wOpticsFilterType = SrConfig.wDefaultMeasFilter;
// 2009.09.19 bagus SE --}--

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_ResisMeas, RES_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_4PP) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_ResisMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ResisMeas, m_szRecipeName, RECIPE_FILE_4PP) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldResisMeas, &m_ResisMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementResistanceProgramView::IsAccessPrivilege()
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
int CRecipeMeasurementResistanceProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}
