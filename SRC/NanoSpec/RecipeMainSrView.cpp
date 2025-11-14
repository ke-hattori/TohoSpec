// RecipeMainSrView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
// 2009.10.09 bagus StagePGM 共通化 --{--
#include "MainFrm.h"
// 2009.10.09 bagus StagePGM 共通化 --}--
#include "RecipeMainSrView.h"
#include "..\\..\\INC\\PifComm.hxx"
#include "RecipeThicknessSettingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//2009.09.02 bagus se 機能拡張 --{--
#define STAGE_OFFSET		15
#define RADIUS				1
#define ELLIPSE_RADIUS		2//4

//Saiki 20110207 Delete ----->
//#define	PIN_INTERVAL		7
//Saiki 20110207 Delete <-----
//2009.09.02 bagus se 機能拡張 --}--

// 2009.12.08 bagus --{--
#if 1
const LPCSTR m_szScanTypeItem[] = {
	"", 							// MEAS_PROG_TYPE_SR_THICKNESS
	"", 							// MEAS_PROG_TYPE_SR_REFLECTANCE
	"(TR)", 						// MEAS_PROG_TYPE_SR_TRANSMITTANCE
	"(GTR)",						// MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
// 2009.10.13 bagus Distance 追加 --{--
	"(DIS)",						// MEAS_PROG_TYPE_SR_DISTANCE
// 2009.10.13 bagus Distance 追加 --}--
	"", 							// MEAS_PROG_TYPE_SR_REFLECTANCE_CIE
	"", 							// MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE
	"", 							// MEAS_PROG_TYPE_SR_OPTICAL_DENSITY
};
#else
const LPCSTR m_szScanTypeItem[] = {
	"", 							// MEAS_PROG_TYPE_SR_THICKNESS
	"", 							// MEAS_PROG_TYPE_SR_REFLECTANCE
	"(TRANCEMITTANCE)", 			// MEAS_PROG_TYPE_SR_TRANSMITTANCE
	"(GANTRY TRANCEMITTANCE)",		// MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
	"(DISTANCE)",					// MEAS_PROG_TYPE_SR_DISTANCE
	"", 							// MEAS_PROG_TYPE_SR_REFLECTANCE_CIE
	"", 							// MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE
	"", 							// MEAS_PROG_TYPE_SR_OPTICAL_DENSITY
};
#endif
// 2009.12.08 bagus --}--


// 2009.10.09 bagus StagePGM 共通化 --{--
static struct {
	int nHead;
	int nScan;
	LPCSTR	szStagePGMItem;
} s_StagePGMTable[] = {
	// HeadType,				ScanType,					StagePGMItem
// 2009.12.08 bagus --{--
#if 1
	{ STAGE_PGM_TYPE_STANDARD,	SCAN_TYPE_STANDARD, 		"STD"		},
	{ STAGE_PGM_TYPE_STRESS,	SCAN_TYPE_STRESS,			"STRESS"	},
	{ HEAD_TYPE_SR, 			SCAN_TYPE_SR_GANTRY,		"GTR"		},
	{ HEAD_TYPE_SR, 			SCAN_TYPE_SR_DISTANCE,		"DIS"		},
#else
	{ STAGE_PGM_TYPE_STANDARD,	SCAN_TYPE_STANDARD, 		"STD"							},
	{ STAGE_PGM_TYPE_STRESS,	SCAN_TYPE_STRESS,			"STRESS"						},
	{ HEAD_TYPE_SR, 			SCAN_TYPE_SR_GANTRY,		"SR (GANTRY TRANCEMITTANCE)"	},
	{ HEAD_TYPE_SR, 			SCAN_TYPE_SR_DISTANCE,		"SR (DISTANCE)" 				},
#endif
// 2009.12.08 bagus --}--
};
// 2009.10.09 bagus StagePGM 共通化 --}--

// 2009.11.04 bagus RS 追加 --{--
// 2009.12.08 bagus --{--
//#define	SCAN_TYPE_ITEM_RS		"RS"
// 2009.12.08 bagus --}--
// 2009.11.04 bagus RS 追加 --}--

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

// #########################################################################
// CRecipeMainSrView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CRecipeMainSrView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMainSrView::CRecipeMainSrView() : CNanoRecipeUI(CRecipeMainSrView::IDD)
{
	//m_strCaption = "SR MAIN RECIPE";
//	m_strCaption = "MAIN RECIPE";
	//Saiki 20090526 Change ----->
	//m_strCaption = "メイン レシピ";
	CString strBuffer;
	LoadStringML(IDS_CAPTION_MAIN_RECIPE, strBuffer, "MAIN RECIPE");
	m_strCaption = strBuffer;
	//Saiki 20090526 Change <-----
	//2009.08.28 bagus stress --{--
	m_iNewHeadType = HEAD_TYPE_SR;
	//2009.08.28 bagus stress --}--

	//{{AFX_DATA_INIT(CRecipeMainSrView)
	m_strHeadType = _T("");
	m_strMeasHeadType = _T("");
	m_strPointDeskewHeadType = _T("");
	m_strPointDeskewLens = _T("");
	m_strRecalib1HeadType = _T("");
	m_strRecalib2HeadType = _T("");
	m_strRecalib3HeadType = _T("");
	//}}AFX_DATA_INIT

	//2009.09.02 bagus se 機能拡張 --{--
	::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
	//2009.09.02 bagus se 機能拡張 --}--
	//2009.09.03 bagus se 機能拡張 --{--
	::ZeroMemory(&m_StageStressProg, sizeof(m_StageStressProg));
	//2009.09.03 bagus se 機能拡張 --{--

	// 2009.10.02 bagus Gantry --{--
	m_SRScanType = MEAS_PROG_TYPE_SR_THICKNESS;

	m_bChage = FALSE;
	// 2009.10.02 bagus Gantry --}--
}

// =========================================================================
//
CRecipeMainSrView::~CRecipeMainSrView()
{
}

// =========================================================================
//
void CRecipeMainSrView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMainSrView)
	DDX_Text(pDX, IDC_HEAD_TYPE, m_strHeadType);
	DDX_Text(pDX, IDC_MEAS_HEAD_TYPE, m_strMeasHeadType);
	DDX_Text(pDX, IDC_POINT_DESKEW_HEAD_TYPE, m_strPointDeskewHeadType);
	DDX_Text(pDX, IDC_POINT_DESKEW_LENS, m_strPointDeskewLens);
	DDX_Text(pDX, IDC_RECALIB_1_HEAD_TYPE, m_strRecalib1HeadType);
	DDX_Text(pDX, IDC_RECALIB_2_HEAD_TYPE, m_strRecalib2HeadType);
	DDX_Text(pDX, IDC_RECALIB_3_HEAD_TYPE, m_strRecalib3HeadType);
	//2009.09.02 bagus se 機能拡張 --{--
	DDX_Control(pDX, IDC_MEASUREMENT_STAGE_MAP, m_SamplePointGraph);
	//2009.09.02 bagus se 機能拡張 --}--
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MAIN_RECIPE_NAME, m_MainRcpInfo.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_MEASUREMENT_PROGRAM, m_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_STAGE_PROGRAM, m_MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_POINT_DESKEW_PROGRAM, m_MainRcpInfo.MainRcpParam.hdr.szPointDeskew, RECIPE_NAME_LEN + 1);

	DDX_CBIndex(pDX, IDC_RECALIBRATION_ITEM1, m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1]);
	DDX_CBIndex(pDX, IDC_RECALIBRATION_ITEM2, m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2]);
	DDX_CBIndex(pDX, IDC_RECALIBRATION_ITEM3, m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]);

	DDX_Text(pDX, IDC_RECALIBRATION_1, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1], RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_RECALIBRATION_2, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_RECALIBRATION_3, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], RECIPE_NAME_LEN + 1);
	DDX_Check(pDX, IDC_SAMPLE_ID, m_MainRcpInfo.MainRcpParam.hdr.bSampleID);
	DDX_Check(pDX, IDC_MEASUREMENT_RESULT_AUTO_SAVE, m_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult);
	DDX_Check(pDX, IDC_SAVE_OVERWRITE, m_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite);
	DDX_Check(pDX, IDC_MEASUREMENT_REFERENCE, m_MainRcpInfo.MainRcpParam._SR.bMeasRef);
	DDX_Check(pDX, IDC_REFERENCE_WITH_AF, m_MainRcpInfo.MainRcpParam._SR.bRefWithAF);
	DDX_Text(pDX, IDC_COMMENT, m_MainRcpInfo.hdr.szComment, RECIPE_COMMENT_LEN + 1);
	//2009.09.02 bagus se 機能拡張 --{--
	DDX_Text(pDX, IDC_STAGE_HEAD_TYPE, m_strStageHeadType);
	DDX_Check(pDX, IDC_GRAPH_AUTO_SCALE, m_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale);
	//2009.09.02 bagus se 機能拡張 --}--
	//2009.12.20 bagus 保存ファイル名変更 --{--
	DDX_Check(pDX, IDC_MEASUREMENT_RESULT_SAVE_WITH_DATE, m_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate);
	//2009.12.20 bagus 保存ファイル名変更 --}--
	//Saiki 20110207 Add ----->
	DDX_Control(pDX, IDC_THICK_SETTING_BUTTON, m_ThickSettingButton);
	//Saiki 20110207 Add <-----
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CRecipeMainSrView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMainSrView)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_MEASUREMENT_PROGRAM, OnSelchangeMeasurementProgram)
	ON_CBN_SELCHANGE(IDC_POINT_DESKEW_PROGRAM, OnSelchangePointDeskewProgram)
	ON_CBN_SELCHANGE(IDC_RECALIBRATION_1, OnSelchangeRecalibration1)
	ON_CBN_SELCHANGE(IDC_RECALIBRATION_2, OnSelchangeRecalibration2)
	ON_CBN_SELCHANGE(IDC_RECALIBRATION_3, OnSelchangeRecalibration3)
	ON_CBN_SELCHANGE(IDC_POINT_MANUAL_ADJUSTMENT, OnSelchangePointManualAdjustment)
	ON_CBN_SELCHANGE(IDC_FOCUS, OnSelchangeFocus)
	ON_CBN_SELCHANGE(IDC_FOCUS_IF_FAIL, OnSelchangeFocusIfFail)
	ON_CBN_SELCHANGE(IDC_DEFAULT_DISPLAY, OnSelchangeDefaultDisplay)
	ON_CBN_EDITCHANGE(IDC_SAMPLE_ID, OnSelchangeSampleId)
	ON_CBN_SELCHANGE(IDC_RECALIBRATION_ITEM1, OnSelchangeRecalibrationItem1)
	ON_CBN_SELCHANGE(IDC_RECALIBRATION_ITEM2, OnSelchangeRecalibrationItem2)
	ON_CBN_SELCHANGE(IDC_RECALIBRATION_ITEM3, OnSelchangeRecalibrationItem3)
	ON_CBN_SELCHANGE(IDC_FINAL_FOCUS, OnSelchangeFinalFocus)
	ON_CBN_SELCHANGE(IDC_STAGE_PROGRAM, OnSelchangeStageProgram)
	ON_BN_CLICKED(IDC_MEASUREMENT_RESULT_AUTO_SAVE, OnMeasurementResultAutoSave)
	ON_BN_CLICKED(IDC_THICK_SETTING_BUTTON, OnThickSettingButton)
	ON_BN_CLICKED(IDC_RADIO_ACTUAL_VALUE, OnRadioThicknessTypeForStressValue)
	ON_BN_CLICKED(IDC_RADIO_FIXED_VALUE, OnRadioThicknessTypeForStressValue)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// =========================================================================
// CRecipeMainSrView メッセージ ハンドラ

// =========================================================================
//

void CRecipeMainSrView::OnInitialUpdate()
{
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);

	memset(&m_StageConfig,NULL,sizeof(m_StageConfig));
	memset(&m_StressConfig,NULL,sizeof(m_StressConfig));
//Saiki 20110303 Add ----->
	memset(&m_StressParamInfo,NULL,sizeof(STRESS_PARAM_INFO));
	memset(&m_OldStressParamInfo,NULL,sizeof(STRESS_PARAM_INFO));
//Saiki 20110303 Add <-----
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);

	LoadRecipeData();

// 2009.12.08 bagus --{--
#if 0
// 2009.11.04 bagus RS 追加 --{--
//	m_strHeadType = HEAD_TYPE_ITEM[m_MainRcpInfo.MainRcpParam.hdr.wHeadType];					// SRしか入らないと思います．．．
	switch (m_MainRcpInfo.MainRcpParam.hdr.wHeadType) {
	case HEAD_TYPE_4PP:
		m_strHeadType = SCAN_TYPE_ITEM_RS;
		break;
	default:
		m_strHeadType = HEAD_TYPE_ITEM[m_MainRcpInfo.MainRcpParam.hdr.wHeadType];
		break;
	}
// 2009.11.04 bagus RS 追加 --}--
#else
// 2009.12.09 bagus --{--
//	m_strHeadType = HEAD_TYPE_ITEM[m_MainRcpInfo.MainRcpParam.hdr.wHeadType];
	m_strHeadType = HEAD_TYPE_ITEM_SHORT[m_MainRcpInfo.MainRcpParam.hdr.wHeadType];
// 2009.12.09 bagus --}--
// 2009.11.04 bagus RS 追加 --}--
#endif
// 2009.12.08 bagus --}--

	CNanoRecipeUI::OnInitialUpdate();

// 2013.11.12 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
/* added 2014.05.22 hmenjo 手動時 lot ID 不要 ---------- { ---------- */
		CString l_strTmp;
		LoadStringML(IDS_MAINRCP_SMPID_TXT, l_strTmp, "Sample ID");
		this->SetDlgItemText(IDC_SAMPLE_ID, l_strTmp);
/* added 2014.05.22 hmenjo 手動時 lot ID 不要 ---------- } ---------- */
		GetDlgItem(IDC_STAGE_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_PROGRAM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STR_POINT_DESKEW_HEAD_TYPE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_HEAD_TYPE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_FOCUS_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_IF_FAIL_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_IF_FAIL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINAL_FOCUS_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINAL_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ADJUSTMENT_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SAMPLE_ID_COMMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_RESULT_AUTO_SAVE_LABEL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STAGE_MAP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_STAGE_MAP)->ShowWindow(SW_HIDE);
	}
// 2013.11.12 Bagus Add (TohoSpec対応) <--
// 2014.01.17 Bagus Add (Stage None対応) -->
	else{
		if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
			GetDlgItem(IDC_STAGE_GROUP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STAGE_PROGRAM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STR_POINT_DESKEW_HEAD_TYPE2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STAGE_HEAD_TYPE)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STAGE_MAP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_MEASUREMENT_STAGE_MAP)->ShowWindow(SW_HIDE);
		}
	}
// 2014.01.17 Bagus Add (Stage None対応) <--
	///// Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// 作成に失敗
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	///// Static Control /////
	m_stcMainRecipeName.SubclassWindow(GetDlgItem(IDC_MAIN_RECIPE_NAME)->GetSafeHwnd());
	m_stcMainRecipeName.SetBkColor(WATER_COLOR);
	m_stcHeadType.SubclassWindow(GetDlgItem(IDC_HEAD_TYPE)->GetSafeHwnd());
	m_stcHeadType.SetBkColor(WATER_COLOR);
	m_stcMeasHeadType.SubclassWindow(GetDlgItem(IDC_MEAS_HEAD_TYPE)->GetSafeHwnd());
	m_stcMeasHeadType.SetBkColor(WATER_COLOR);
	m_stcPointDeskewLens.SubclassWindow(GetDlgItem(IDC_POINT_DESKEW_LENS)->GetSafeHwnd());
	m_stcPointDeskewLens.SetBkColor(WATER_COLOR);
	m_stcPointDeskewHeadType.SubclassWindow(GetDlgItem(IDC_POINT_DESKEW_HEAD_TYPE)->GetSafeHwnd());
	m_stcPointDeskewHeadType.SetBkColor(WATER_COLOR);
	m_stcRecalib1HeadType.SubclassWindow(GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->GetSafeHwnd());
	m_stcRecalib1HeadType.SetBkColor(WATER_COLOR);
	m_stcRecalib2HeadType.SubclassWindow(GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->GetSafeHwnd());
	m_stcRecalib2HeadType.SetBkColor(WATER_COLOR);
	m_stcRecalib3HeadType.SubclassWindow(GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->GetSafeHwnd());
	m_stcRecalib3HeadType.SetBkColor(WATER_COLOR);
	//2009.09.02 bagus se 機能拡張 --{--
	m_stcStageHeadType.SubclassWindow(GetDlgItem(IDC_STAGE_HEAD_TYPE)->GetSafeHwnd());
	m_stcStageHeadType.SetBkColor(WATER_COLOR);
	//2009.09.02 bagus se 機能拡張 --}--

	//2009.10.20 bagus MS 追加 --{--
	CString strBuf;
	LoadStringML(IDS_POINT_NAME_NONE, strBuf, "None");
	//2009.10.20 bagus MS 追加 --}--

//Saiki 20110207 Add ----->
	/// Thick Setting Button ///
	m_ThickSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ThickSettingButton.DrawFlatFocus(TRUE);
//Saiki 20110207 Add <-----

// 2009.09.29 bagus SE --{--
	switch(m_iNewHeadType){
	case HEAD_TYPE_SR:
		GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_SHOW);
// 2009.10.22 bagus Stress 修正 --{--
//		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
// 2009.10.22 bagus Stress 修正 --}--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 2009.12.11 bagus 修正 --{--
			GetDlgItem(IDC_FOCUS_IF_FAIL_LABEL)->ShowWindow(SW_NORMAL);
// 2009.12.11 bagus 修正 --}--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		break;
	case HEAD_TYPE_SE:
		GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
// 2009.10.22 bagus Stress 修正 --{--
//		m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = FALSE;
		m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
// 2009.10.22 bagus Stress 修正 --}--
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DEFAULT_DISPLAY_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DEFAULT_DISPLAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRAPH_AUTO_SCALE)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
		m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	case HEAD_TYPE_STRESS:
		GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_SHOW);
// 2009.10.22 bagus Stress 修正 --{--
//		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
		GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] = 0;
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3] = 0;
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], "%s", (LPCTSTR)strBuf);
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], "%s", (LPCTSTR)strBuf);
		GetDlgItem(IDC_SAMPLE_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_MEASUREMENT_RESULT_SAVE_WITH_DATE)->EnableWindow(FALSE);
		m_MainRcpInfo.MainRcpParam.hdr.bSampleID = TRUE;
		GetDlgItem(IDC_MEASUREMENT_RESULT_AUTO_SAVE)->EnableWindow(FALSE);
		m_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult = TRUE;
// 2009.10.22 bagus Stress 修正 --}--
// 2009.12.21 bagus stress 修正 --{--
//ストレス測定の場合には使用しない
		m_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate = FALSE;
		GetDlgItem(IDC_MEASUREMENT_RESULT_SAVE_WITH_DATE)->ShowWindow(SW_HIDE);
// 2009.12.21 bagus stress 修正 --}--
		break;
	// 2009.10.08 bagus CTA 追加 --{--
	case HEAD_TYPE_CTA:
		GetDlgItem(IDC_FOCUS_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_IF_FAIL_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_IF_FAIL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINAL_FOCUS_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINAL_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
		m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption = 0;
		m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption = 0;
		m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
		m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
// 2009.10.28 bagus CTA 追加修正 --{--
		GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3] = 0;
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], "%s", (LPCTSTR)strBuf);
// 2009.10.28 bagus CTA 追加修正 --}--
//2009.12.01 bagus CTA 修正 --{--
// サンプルIDを使用するを必ずチェックする。かつ変更不可
		m_MainRcpInfo.MainRcpParam.hdr.bSampleID = TRUE;
		GetDlgItem(IDC_SAMPLE_ID)->EnableWindow(FALSE);
//2009.12.01 bagus CTA 修正 --}--
//2009.12.07 bagus CTA 修正 --{--
		GetDlgItem(IDC_GRAPH_AUTO_SCALE)->ShowWindow(SW_HIDE);
//2009.12.07 bagus CTA 修正 --{--
		break;
	// 2009.10.08 bagus CTA 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case HEAD_TYPE_MS:
		GetDlgItem(IDC_RECALIB_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_ITEM_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_ITEM1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_IF_FAIL_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_IF_FAIL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINAL_FOCUS_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINAL_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ADJUSTMENT_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SAMPLE_ID)->ShowWindow(SW_HIDE);
		//2009.12.20 bagus 2009.12.20 bagus 保存ファイル名修正--{--
		GetDlgItem(IDC_MEASUREMENT_RESULT_SAVE_WITH_DATE)->ShowWindow(SW_HIDE);
		//2009.12.20 bagus 2009.12.20 bagus 保存ファイル名修正--}--
		GetDlgItem(IDC_MEASUREMENT_RESULT_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_RESULT_AUTO_SAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_RESULT_AUTO_SAVE_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_RESULT_ALLOW_OVERWRITE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRAPH_AUTO_SCALE)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] = 0;
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] = 0;
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3] = 0;
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1], "%s", (LPCTSTR)strBuf);
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], "%s", (LPCTSTR)strBuf);
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], "%s", (LPCTSTR)strBuf);
		m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
		m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption = 0;
		m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption = 0;
		m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = 0;
		m_MainRcpInfo.MainRcpParam.hdr.bSampleID = FALSE;
		m_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult = FALSE;
		m_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite = FALSE;
		m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay = 0;
		m_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale = TRUE;
		m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
		m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
		break;
	//2009.10.20 bagus MS 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	case HEAD_TYPE_4PP:
		GetDlgItem(IDC_RECALIB_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_ITEM_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_ITEM1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_IF_FAIL_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FOCUS_IF_FAIL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINAL_FOCUS_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FINAL_FOCUS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GRAPH_AUTO_SCALE)->ShowWindow(SW_HIDE);
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] = 0;
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] = 0;
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3] = 0;
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1], "%s", (LPCTSTR)strBuf);
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], "%s", (LPCTSTR)strBuf);
		sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], "%s", (LPCTSTR)strBuf);
		m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
		m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption = 0;
		m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption = 0;
		m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay = 0;
		m_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale = TRUE;
		m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
		m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
		break;
	// 2009.11.04 bagus RS 追加 --}--
	default:
		break;
	}
// 2009.09.29 bagus SE --}--
//2009.12.09 bagus 修正 --{--
// 測定PGMとStagePGMのComboBoxのInitで値が変更されるので
// 一旦リキャブリレーションの値を退避しておく。
	int 	RecalibItem[RECALIB_MAX];
	TCHAR	szRecalib[RECALIB_MAX][RECIPE_NAME_LEN + 1];
	memcpy(RecalibItem,m_MainRcpInfo.MainRcpParam._SR.RecalibItem,sizeof(int) * RECALIB_MAX);
	memcpy(szRecalib,m_MainRcpInfo.MainRcpParam._SR.szRecalib,sizeof(TCHAR) * RECALIB_MAX * (RECIPE_NAME_LEN + 1));
//2009.12.09 bagus 修正 --}--

	///// Control Initialize /////
	InitCombo_MeasurementProgram();
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		InitCombo_StageProgram();
		//2009.12.09 bagus 修正 --{--
		OnSelchangeStageProgram();
		//2009.12.09 bagus 修正 --}--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	InitCombo_PointDeskewProgram();
//2009.12.09 bagus 修正 --{--
	memcpy(m_MainRcpInfo.MainRcpParam._SR.RecalibItem,RecalibItem,sizeof(int) * RECALIB_MAX);
	memcpy(m_MainRcpInfo.MainRcpParam._SR.szRecalib,szRecalib,sizeof(TCHAR) * RECALIB_MAX * (RECIPE_NAME_LEN + 1));
	InitCombo_RecalibrationProgram((CComboBox*)GetDlgItem(IDC_RECALIBRATION_1), m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1]);
	InitCombo_RecalibrationProgram((CComboBox*)GetDlgItem(IDC_RECALIBRATION_2), m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2]);
	InitCombo_RecalibrationProgram((CComboBox*)GetDlgItem(IDC_RECALIBRATION_3), m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3]);
	((CComboBox *)GetDlgItem(IDC_RECALIBRATION_ITEM1))->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1]);
	((CComboBox *)GetDlgItem(IDC_RECALIBRATION_ITEM2))->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2]);
	((CComboBox *)GetDlgItem(IDC_RECALIBRATION_ITEM3))->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]);
	OnSelchangeRecalibration1();
	OnSelchangeRecalibration2();
	OnSelchangeRecalibration3();
//2009.12.09 bagus 修正 --}--
	InitCombo_PointManualAdjustment();
	InitCombo_Focus();
	InitCombo_DefaultDisplay();
//Saiki 20110315 Add ----->
	InitThickOptionsForStress(m_iNewHeadType == HEAD_TYPE_STRESS ? TRUE : FALSE);
//Saiki 20110315 Add <-----

	// 各種ヘッドタイプの編集
	//OnSelchangeMeasurementProgram();
	//OnSelchangePointDeskewProgram();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		DefaultSetting_Init();
	}

// 2009.10.22 bagus Stress 修正 --{--
#if 0
	// 2009.08.28 bagus stress --{--
	//ストレス測定はSAMPLEID固定
	CButton *pButton = (CButton *)GetDlgItem(IDC_SAMPLE_ID);
	if (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS) {
		pButton->SetCheck(1);
		pButton->EnableWindow(FALSE);
		m_MainRcpInfo.MainRcpParam.hdr.bSampleID = TRUE;
	} else {
		//pButton->SetCheck(1);
		pButton->EnableWindow(TRUE);
	}
	// 2009.08.28 bagus stress --}--
#endif
// 2009.10.22 bagus Stress 修正 --}--

	//2009.09.02 bagus se 機能拡張 --{--
	///// Stage Map /////
	switch(m_iNewHeadType){
	case HEAD_TYPE_STRESS:
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			m_SamplePointGraph.SetLineStatusMode(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		break;
	case HEAD_TYPE_SR:
	//2009.09.07 bagus se --{--
	case HEAD_TYPE_SE:
	//2009.09.07 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.08 bagus CTA 追加 --{--
	case HEAD_TYPE_CTA:
	// 2009.10.08 bagus CTA 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case HEAD_TYPE_MS:
	//2009.10.20 bagus MS 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	case HEAD_TYPE_4PP:
	// 2009.11.04 bagus RS 追加 --}--
	default:
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			m_SamplePointGraph.SetLineStatusMode(FALSE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		break;
	}

// 2014.01.07 bagus Add(Stage None対応) -->
	if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
			m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
			UpdateData(FALSE);
			GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
		}
	}
// 2014.01.07 bagus Add(Stage None対応) <--

	//2009.12.09 bagus 修正 --{--
	//OnSelchangeStageProgram();
	//2009.12.09 bagus 修正 --}--
	//2009.12.21 bagus 修正 --{--
	OnMeasurementResultAutoSave();
	//2009.12.21 bagus 修正 --}--
	//2009.09.02 bagus se 機能拡張 --}--
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	RECT l_rectThisView;	this->GetWindowRect(&l_rectThisView);
	RECT l_rectComment;	this->GetDlgItem(IDC_COMMENT)->GetWindowRect(&l_rectComment);
	RECT l_rectThisDlg = {0, 0, 605, 551};
	((CDialog*) this)->MapDialogRect(&l_rectThisDlg);
	long l_lViewBottom = l_rectThisView.top + (l_rectThisDlg.bottom - l_rectThisDlg.top + 1);
	long l_lSpaceCommentBottom = l_lViewBottom - l_rectComment.bottom;
	if ((g_lModelType == MODEL_T3100) && (l_rectThisView.bottom <= (l_rectComment.bottom + l_lSpaceCommentBottom))) {
		long l_ldeltaY = l_rectThisView.bottom - l_lSpaceCommentBottom - (l_rectComment.bottom - l_rectComment.top);
		l_ldeltaY = l_rectComment.top - l_ldeltaY;
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
		POINT l_ptStart = {0, 0};
		/* コメント文字	*/
		RECT l_rectCommentStatic;	this->GetDlgItem(IDC_COMMENT_STATIC)->GetWindowRect(&l_rectCommentStatic);
		l_ptStart.x = l_rectCommentStatic.left;
		l_ptStart.y = l_rectCommentStatic.top - l_ldeltaY;
		this->ScreenToClient(&l_ptStart);
		this->GetDlgItem(IDC_COMMENT_STATIC)->SetWindowPos(0, l_ptStart.x, l_ptStart.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		/* コメント欄	*/
		l_ptStart.x = l_rectComment.left;
		l_ptStart.y = l_rectComment.top - l_ldeltaY;
		this->ScreenToClient(&l_ptStart);
		this->GetDlgItem(IDC_COMMENT)->SetWindowPos(0, l_ptStart.x, l_ptStart.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		/* コントロールを移動	*/
		RECT l_rectDelta = {10, 337 - 259 + 5, 87, 12};
		((CDialog*) this)->MapDialogRect(&l_rectDelta);
		struct {
			RECT	rectCnvPos;
			int		iID;
		} l_CnvPosId[] = {
				{{ 10, 337,  87,  12}, IDC_SAMPLE_ID},
				{{ 10, 352, 335,  72}, IDC_MEASUREMENT_RESULT_GROUP},
				{{ 20, 364, 136,  12}, IDC_MEASUREMENT_RESULT_AUTO_SAVE},
				{{ 28, 380, 300,  12}, IDC_MEASUREMENT_RESULT_SAVE_WITH_DATE},
				{{ 20, 396,  80,  12}, IDC_MEASUREMENT_RESULT_ALLOW_OVERWRITE},
				{{ 10, 432, 110,  12}, IDC_MEASUREMENT_REFERENCE},
				{{ 10, 447,  60,  12}, IDC_DEFAULT_DISPLAY_LABEL},
				{{ 95, 447,  50,  12}, IDC_DEFAULT_DISPLAY},
				{{170, 446,  74,  12}, IDC_GRAPH_AUTO_SCALE},
				{{0, 0, 0, 0}, 0},
			};
		for (int i = 0; 0 != l_CnvPosId[i].iID; i++) {
			switch (l_CnvPosId[i].iID) {
			case IDC_MEASUREMENT_REFERENCE:
				l_CnvPosId[i].rectCnvPos.top -= 7;
				break;
			case IDC_DEFAULT_DISPLAY_LABEL:
			case IDC_DEFAULT_DISPLAY:
			case IDC_GRAPH_AUTO_SCALE:
				l_CnvPosId[i].rectCnvPos.top -= 9;
				break;
			}
			((CDialog*) this)->MapDialogRect(&l_CnvPosId[i].rectCnvPos);
			this->GetDlgItem(l_CnvPosId[i].iID)->SetWindowPos(0,
							l_CnvPosId[i].rectCnvPos.left,
							l_CnvPosId[i].rectCnvPos.top - l_rectDelta.top,
							0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
	}
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */
}

// =========================================================================
// メジャメントプログラム コンボ選択時 ヘッドタイプの編集
//
void CRecipeMainSrView::OnSelchangeMeasurementProgram()
{
// 2009.10.02 bagus Gantry --{--
//	UpdateData(TRUE);
	if (!m_bChage) {UpdateData(TRUE); }
// 2009.10.02 bagus Gantry --}--

// 2009.10.02 bagus Gantry --{--
	CComboBox* pComboFocus = (CComboBox*)GetDlgItem(IDC_FOCUS);
	CComboBox* pComboAdjust = (CComboBox*)GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT);
// 2009.10.02 bagus Gantry --}--

// 2009.10.28 bagus 2点間 --{--
	CString strBuf;
// 2009.10.28 bagus 2点間 --}--

	for(int i=0; i < RECALIB_MAX; i++)
	{
//		if(m_MainRcpInfo.MainRcpParam._SR.RecalibItem[i] == -1)
			//『UpdateData(TRUE)』で、空コンボボックスから、-1を代入されてしまった変数に0を入れる
			m_MainRcpInfo.MainRcpParam._SR.RecalibItem[i] = 0;
	}

	// コンボボックス選択された名称の取得
	CString strBuffer = m_MainRcpInfo.MainRcpParam.hdr.szMeas;
	///// Measurement Program - Head Type /////
	if ( strBuffer.GetLength() ) {
		if ( !RecipeFile_LoadRecipe(&m_MeasProgInfo, strBuffer, RECIPE_FILE_MEASUREMENT_PROGRAM) )
			m_strMeasHeadType.Empty();
		else
// 2009.11.04 bagus RS 追加 --{--
#if 0
// 2009.09.30 bagus Gantry --{--
//			m_strMeasHeadType = HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType];
			if (m_MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR) {
				m_strMeasHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType], m_szScanTypeItem[m_MeasProgInfo.ScanParams.hdr.wScanType]);
			} else {
				m_strMeasHeadType = HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType];
			}
// 2009.09.30 bagus Gantry --}--
#else
// 2009.12.08 bagus --{--
#if 0
			switch (m_MeasProgInfo.ScanParams.hdr.wHeadType) {
			case HEAD_TYPE_SR:
				m_strMeasHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType], m_szScanTypeItem[m_MeasProgInfo.ScanParams.hdr.wScanType]);
				break;
			case HEAD_TYPE_4PP:
				m_strMeasHeadType = SCAN_TYPE_ITEM_RS;
				break;
			default:
				m_strMeasHeadType = HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType];
				break;
			}
#else
			switch (m_MeasProgInfo.ScanParams.hdr.wHeadType) {
			case HEAD_TYPE_SR:
// 2009.12.09 bagus --{--
//				m_strMeasHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType], m_szScanTypeItem[m_MeasProgInfo.ScanParams.hdr.wScanType]);
				m_strMeasHeadType.Format("%s%s", HEAD_TYPE_ITEM_SHORT[m_MeasProgInfo.ScanParams.hdr.wHeadType], m_szScanTypeItem[m_MeasProgInfo.ScanParams.hdr.wScanType]);
// 2009.12.09 bagus --}--
				break;
			default:
// 2009.12.09 bagus --{--
//				m_strMeasHeadType = HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType];
				m_strMeasHeadType = HEAD_TYPE_ITEM_SHORT[m_MeasProgInfo.ScanParams.hdr.wHeadType];
// 2009.12.09 bagus --}--
				break;
			}
#endif
// 2009.12.08 bagus --}--
#endif
// 2009.11.04 bagus RS 追加 --}--
			InitCombo_RecalibItemSetting();
// 2009.10.02 bagus Gantry --{--
			m_SRScanType = m_MeasProgInfo.ScanParams.hdr.wScanType;

// 2013.11.07 Bagus Add (TohoSpec対応) -->
			if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
				InitCombo_StageProgram();
				OnSelchangeStageProgram();
// 2013.11.07 Bagus Add (TohoSpec対応) -->
			}
// 2014.01.14 Bagus Add (TohoSpec対応 メインレシピ新規作成時にRecalib Noneを表示するようにする) -->
			else{
				UpdateData(TRUE);
			}
// 2014.01.14 Bagus Add (TohoSpec対応 メインレシピ新規作成時にRecalib Noneを表示するようにする) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			InitCombo_RecalibrationProgram((CComboBox*)GetDlgItem(IDC_RECALIBRATION_1), m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1]);
			OnSelchangeRecalibration1();
			InitCombo_RecalibrationProgram((CComboBox*)GetDlgItem(IDC_RECALIBRATION_2), m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2]);
			OnSelchangeRecalibration2();
			InitCombo_RecalibrationProgram((CComboBox*)GetDlgItem(IDC_RECALIBRATION_3), m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3]);
			OnSelchangeRecalibration3();
// 2009.10.28 bagus 2点間 --{--
			InitCombo_DefaultDisplay();
// 2009.10.28 bagus 2点間 --}--

// 2009.10.08 bagus CTA 追加 --{--
#if 0
			if (m_SRScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) {
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
				m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
				m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;

				pComboFocus->EnableWindow(FALSE);
				pComboFocus->SetCurSel(0);
				m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_FOCUS_IF_FAIL)->EnableWindow(FALSE);
					GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(FALSE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
				pComboAdjust->EnableWindow(FALSE);
				pComboAdjust->SetCurSel(0);
				m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = 0;
			} else {
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_SHOW);
// 2009.10.09 K.Matsuo 既存のデータ処理のデグレードバグ <--
//				m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
//				m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = FALSE;
// 2009.10.09 K.Matsuo 既存のデータ処理のデグレードバグ <--

				pComboFocus->EnableWindow(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_FOCUS_IF_FAIL)->EnableWindow(TRUE);
					GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
				pComboAdjust->EnableWindow(TRUE);
			}
#else
			switch ( m_iNewHeadType ) {
			case HEAD_TYPE_SR:
				switch ( m_SRScanType ) {
				case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.10.27 bagus Gantry 追加修正 --{--
				case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2009.10.27 bagus Gantry 追加修正 --}--
					GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
// 2009.10.22 bagus Stress 修正 --{--
//					GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
// 2009.10.22 bagus Stress 修正 --}--
// 2009.10.09 K.Matsuo 既存のデータ処理のデグレードバグ <--
//					m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
//					m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
// 2009.10.09 K.Matsuo 既存のデータ処理のデグレードバグ <--

					pComboFocus->EnableWindow(FALSE);
					pComboFocus->SetCurSel(0);
					m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
						GetDlgItem(IDC_FOCUS_IF_FAIL)->EnableWindow(FALSE);
						//2009.12.09 bagus --{--
						GetDlgItem(IDC_FINAL_FOCUS_LABEL)->EnableWindow(FALSE);
						//2009.12.09 bagus --}--
						GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(FALSE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					pComboAdjust->EnableWindow(FALSE);
					pComboAdjust->SetCurSel(0);
					m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = 0;
// 2009.10.28 bagus 2点間 --{--
					GetDlgItem(IDC_RECALIB_GROUP)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_ITEM_STATIC)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_ITEM1)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_1)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_SHOW);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
					if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
						GetDlgItem(IDC_ADJUSTMENT_LABEL)->ShowWindow(SW_SHOW);
						GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT)->ShowWindow(SW_SHOW);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 2009.10.28 bagus 2点間 --}--
// 2009.12.07 bagus 2点間 --{--
					GetDlgItem(IDC_GRAPH_AUTO_SCALE)->ShowWindow(SW_SHOW);
// 2009.12.07 bagus 2点間 --}--
					//Saiki 20110215 Add ----->
					OnSelectThickOptionsForStress();
					//Saiki 20110215 Add <-----
					break;
// 2009.10.28 bagus 2点間 --{--
				case MEAS_PROG_TYPE_SR_DISTANCE:
					GetDlgItem(IDC_RECALIB_GROUP)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIB_ITEM_STATIC)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIBRATION_ITEM1)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIBRATION_1)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_HIDE);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
					if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
						GetDlgItem(IDC_ADJUSTMENT_LABEL)->ShowWindow(SW_HIDE);
						GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT)->ShowWindow(SW_HIDE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

					GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);

					LoadStringML(IDS_POINT_NAME_NONE, strBuf, "None");

					m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] = 0;
					m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] = 0;
					m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3] = 0;
					sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1], "%s", (LPCTSTR)strBuf);
					sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], "%s", (LPCTSTR)strBuf);
					sprintf(m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], "%s", (LPCTSTR)strBuf);

					m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = 0;

					m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;

// 2009.12.07 bagus 2点間 --{--
					m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
						GetDlgItem(IDC_FINAL_FOCUS)->ShowWindow(SW_HIDE);
						//2009.12.09 bagus --{--
						GetDlgItem(IDC_FINAL_FOCUS_LABEL)->ShowWindow(SW_HIDE);
						//2009.12.09 bagus --}--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_GRAPH_AUTO_SCALE)->ShowWindow(SW_HIDE);
// 2009.12.07 bagus 2点間 --}--
					OnSelectThickOptionsForStress();
					break;
// 2009.10.28 bagus 2点間 --}--
				default:
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
						GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_SHOW);
					}
// 2009.10.22 bagus Stress 修正 --{--
//					GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_SHOW);
// 2009.10.22 bagus Stress 修正 --}--
//2009.11.27 bagus 修正 --{--
					//m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
//2009.11.27 bagus 修正 --}--
// 2009.10.22 bagus Stress 修正 --{--
//					m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = FALSE;
// 2009.10.22 bagus Stress 修正 --}--

					pComboFocus->EnableWindow(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
						GetDlgItem(IDC_FOCUS_IF_FAIL)->EnableWindow(TRUE);
						GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(TRUE);
						GetDlgItem(IDC_FOCUS_IF_FAIL_LABEL)->ShowWindow(SW_NORMAL);
// 2009.12.14 bagus SR 追加 --{--
						GetDlgItem(IDC_FINAL_FOCUS)->ShowWindow(SW_NORMAL);
						GetDlgItem(IDC_FINAL_FOCUS_LABEL)->EnableWindow(TRUE);
// 2009.12.14 bagus SR 追加 --}--
						GetDlgItem(IDC_FINAL_FOCUS_LABEL)->ShowWindow(SW_NORMAL);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					pComboAdjust->EnableWindow(TRUE);
// 2009.10.28 bagus 2点間 --{--
					GetDlgItem(IDC_RECALIB_GROUP)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_ITEM_STATIC)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_ITEM1)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_1)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_SHOW);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
					if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
						GetDlgItem(IDC_ADJUSTMENT_LABEL)->ShowWindow(SW_SHOW);
						GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT)->ShowWindow(SW_SHOW);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 2009.10.28 bagus 2点間 --}--
// 2009.12.07 bagus 2点間 --{--
					GetDlgItem(IDC_GRAPH_AUTO_SCALE)->ShowWindow(SW_SHOW);
// 2009.12.07 bagus 2点間 --}--
//Saiki 20110215 Add ----->
					OnSelectThickOptionsForStress();
//Saiki 20110215 Add <-----
					break;
				}
				break;
			case HEAD_TYPE_CTA:
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
// 2009.10.22 bagus Stress 修正 --{--
//				GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
// 2009.10.22 bagus Stress 修正 --}--
				m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
// 2009.10.22 bagus Stress 修正 --{--
//				m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
// 2009.10.22 bagus Stress 修正 --}--

				pComboFocus->EnableWindow(FALSE);
				pComboFocus->SetCurSel(0);
				m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_FOCUS_IF_FAIL)->EnableWindow(FALSE);
					GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(FALSE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
				//2009.12.07 bagus CTA 位置補正項目は有効にする--{--
				//pComboAdjust->EnableWindow(FALSE);
				pComboAdjust->SetCurSel(0);
				//2009.12.07 bagus CTA --}--
				m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = 0;
				//Saiki 20110215 Add ----->
				OnSelectThickOptionsForStress();
				//Saiki 20110215 Add <-----
				break;
//2009.10.20 bagus MS 追加 --{--
			case HEAD_TYPE_MS:
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
// 2009.10.22 bagus Stress 修正 --{--
//				GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
// 2009.10.22 bagus Stress 修正 --}--
				m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
// 2009.10.22 bagus Stress 修正 --{--
//				m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
// 2009.10.22 bagus Stress 修正 --}--

				pComboFocus->EnableWindow(FALSE);
				pComboFocus->SetCurSel(0);
				m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_FOCUS_IF_FAIL)->EnableWindow(FALSE);
					GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(FALSE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
				pComboAdjust->EnableWindow(FALSE);
				pComboAdjust->SetCurSel(0);
				m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = 0;
// 2009.10.28 bagus 2点間 --{--
				GetDlgItem(IDC_RECALIB_GROUP)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_ITEM_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_ITEM1)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_1)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_HIDE);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_ADJUSTMENT_LABEL)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT)->ShowWindow(SW_HIDE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 2009.10.28 bagus 2点間 --}--
				//Saiki 20110215 Add ----->
				OnSelectThickOptionsForStress();
				//Saiki 20110215 Add <-----
				break;
//2009.10.20 bagus MS 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
			case HEAD_TYPE_4PP:
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
				m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;

				pComboFocus->EnableWindow(FALSE);
				pComboFocus->SetCurSel(0);
				m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_FOCUS_IF_FAIL)->EnableWindow(FALSE);
					GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(FALSE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
				GetDlgItem(IDC_RECALIB_GROUP)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_ITEM_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_ITEM1)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_1)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_HIDE);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_ADJUSTMENT_LABEL)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT)->ShowWindow(SW_SHOW);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
				//Saiki 20110215 Add ----->
				OnSelectThickOptionsForStress();
				//Saiki 20110215 Add <-----
				break;
// 2009.11.04 bagus RS 追加 --}--
// 2009.12.08 bagus Stress --{--
			case HEAD_TYPE_STRESS:
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_SHOW);

				GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_HIDE);
				//Saiki 20110215 Add ----->
				OnSelectThickOptionsForStress(TRUE);
				//Saiki 20110215 Add <-----
				break;
// 2009.12.09 bagus SE --{--
			case HEAD_TYPE_SE:
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
				m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
				m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
				//Saiki 20110215 Add ----->
				OnSelectThickOptionsForStress();
				//Saiki 20110215 Add <-----
				break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			case HEAD_TYPE_COMPEASE:
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_DEFAULT_DISPLAY_LABEL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_DEFAULT_DISPLAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_GRAPH_AUTO_SCALE)->ShowWindow(SW_HIDE);
				m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
				m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = TRUE;
				OnSelectThickOptionsForStress();
				break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.12.09 bagus SE --}--
// 2009.12.08 bagus Stress --}--
			default:
				GetDlgItem(IDC_MEASUREMENT_REFERENCE)->ShowWindow(SW_SHOW);
// 2009.10.22 bagus Stress 修正 --{--
//				GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_SHOW);
// 2009.10.22 bagus Stress 修正 --}--
				m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
// 2009.10.22 bagus Stress 修正 --{--
//				m_MainRcpInfo.MainRcpParam._SR.bRefWithAF = FALSE;
// 2009.10.22 bagus Stress 修正 --}--

				pComboFocus->EnableWindow(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_FOCUS_IF_FAIL)->EnableWindow(TRUE);
					GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
				pComboAdjust->EnableWindow(TRUE);
// 2009.10.28 bagus 2点間 --{--
				GetDlgItem(IDC_RECALIB_GROUP)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIB_ITEM_STATIC)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIBRATION_ITEM1)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIBRATION_ITEM2)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIBRATION_ITEM3)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIBRATION_1)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIBRATION_2)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIBRATION_3)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIB_2_HEAD_STATIC)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIB_3_HEAD_STATIC)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIB_2_HEAD_TYPE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RECALIB_3_HEAD_TYPE)->ShowWindow(SW_SHOW);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
				if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
					GetDlgItem(IDC_ADJUSTMENT_LABEL)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT)->ShowWindow(SW_SHOW);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
				}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 2009.10.28 bagus 2点間 --}--
				//Saiki 20110215 Add ----->
				OnSelectThickOptionsForStress();
				//Saiki 20110215 Add <-----
				break;
			}
#endif
// 2009.10.08 bagus CTA 追加 --}--
// 2009.10.02 bagus Gantry --}--
	}
	else {
		m_strMeasHeadType.Empty();
	}

	UpdateData(FALSE);
}

// =========================================================================
// ポイントデスキュー コンボ選択時 ヘッドタイプ・レンズの編集
//
void CRecipeMainSrView::OnSelchangePointDeskewProgram()
{
	UpdateData(TRUE);

	POINT_DESKEW_PROG_INFO PointDeskewProgInfo;
	// コンボボックス選択された名称の取得
	CString strBuffer = m_MainRcpInfo.MainRcpParam.hdr.szPointDeskew;
	///// Point Deskew - Head Type /////
	///// Point Deskew - Lens /////
//	if ( strBuffer.GetLength() && strBuffer != "None" ) {
	//Saiki 20090526 Change ----->
	//if ( strBuffer.GetLength() && strBuffer != "無し" ) {
	CString strBuf;
	LoadStringML(IDS_POINT_NAME_NONE, strBuf, "None");
	if ( strBuffer.GetLength() && strBuffer != strBuf ) {
	//Saiki 20090526 Change <-----
		if ( !RecipeFile_LoadRecipe(&PointDeskewProgInfo, strBuffer, RECIPE_FILE_POINT_DESKEW_PROGRAM) )
		{
			m_strPointDeskewHeadType.Empty();
			m_strPointDeskewLens.Empty();
		}
		else
		{
// 2009.12.09 bagus --{--
//			m_strPointDeskewHeadType = HEAD_TYPE_ITEM[PointDeskewProgInfo.wHeadType];
			m_strPointDeskewHeadType = HEAD_TYPE_ITEM_SHORT[PointDeskewProgInfo.wHeadType];
// 2009.12.09 bagus --}--
			m_strPointDeskewLens = m_SrTurret[PointDeskewProgInfo.iLens].szName;
		}
	}
	else {
		m_strPointDeskewHeadType.Empty();
		m_strPointDeskewLens.Empty();
	}

	UpdateData(FALSE);
}

// =========================================================================
// リキャリブレーション１ コンボ選択時 ヘッドタイプの編集
//
void CRecipeMainSrView::OnSelchangeRecalibration1()
{
	UpdateData(TRUE);

	RECALIB_PROG_INFO RecalibProgInfo;
	// コンボボックス選択された名称の取得
	CString strBuffer = m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1];
	///// Recalibration 1 - Head Type /////
//	if ( strBuffer.GetLength() && strBuffer != "None" ) {
	//Saiki 20090526 Change ----->
	//if ( strBuffer.GetLength() && strBuffer != "無し" ) {
	CString strBuf;
	LoadStringML(IDS_POINT_NAME_NONE, strBuf, "None");
	if ( strBuffer.GetLength() && strBuffer != strBuf ) {
	//Saiki 20090526 Change <-----
		if ( !RecipeFile_LoadRecipe(&RecalibProgInfo, strBuffer, RECIPE_FILE_RECALIBRATION_PROGRAM) )
			m_strRecalib1HeadType.Empty();
		else
// 2009.11.04 bagus RS 追加 --{--
#if 0
// 2009.09.30 bagus Gantry --{--
//			m_strRecalib1HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
			if (RecalibProgInfo.wHeadType == HEAD_TYPE_SR) {
				m_strRecalib1HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
			} else {
				m_strRecalib1HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
			}
// 2009.09.30 bagus Gantry --}--
#else
// 2009.12.08 bagus --{--
#if 0
			switch (RecalibProgInfo.wHeadType) {
			case HEAD_TYPE_SR:
				m_strRecalib1HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
				break;
			case HEAD_TYPE_4PP:
				m_strRecalib1HeadType = SCAN_TYPE_ITEM_RS;
				break;
			default:
				m_strRecalib1HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
				break;
			}
#else
			switch (RecalibProgInfo.wHeadType) {
			case HEAD_TYPE_SR:
// 2009.12.09 bagus --{--
//				m_strRecalib1HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
				m_strRecalib1HeadType.Format("%s%s", HEAD_TYPE_ITEM_SHORT[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
// 2009.12.09 bagus --}--
				break;
			default:
// 2009.12.09 bagus --{--
//				m_strRecalib1HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
				m_strRecalib1HeadType = HEAD_TYPE_ITEM_SHORT[RecalibProgInfo.wHeadType];
// 2009.12.09 bagus --}--
				break;
			}
#endif
// 2009.12.08 bagus --}--
#endif
// 2009.11.04 bagus RS 追加 --}--
	}
	else {
		m_strRecalib1HeadType.Empty();
	}

	UpdateData(FALSE);
}

// =========================================================================
// リキャリブレーション２ コンボ選択時 ヘッドタイプの編集
//
void CRecipeMainSrView::OnSelchangeRecalibration2()
{
	UpdateData(TRUE);

	RECALIB_PROG_INFO RecalibProgInfo;
	// コンボボックス選択された名称の取得
	CString strBuffer = m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2];
	///// Recalibration 2 - Head Type /////
//	if ( strBuffer.GetLength() && strBuffer != "None" ) {
	//Saiki 20090526 Change ----->
	//if ( strBuffer.GetLength() && strBuffer != "無し" ) {
	CString strBuf;
	LoadStringML(IDS_POINT_NAME_NONE, strBuf, "None");
	if ( strBuffer.GetLength() && strBuffer != strBuf ) {
	//Saiki 20090526 Change <-----
		if ( !RecipeFile_LoadRecipe(&RecalibProgInfo, strBuffer, RECIPE_FILE_RECALIBRATION_PROGRAM) )
			m_strRecalib2HeadType.Empty();
		else
// 2009.11.04 bagus RS 追加 --{--
#if 0
// 2009.09.30 bagus Gantry --{--
//			m_strRecalib2HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
			if (RecalibProgInfo.wHeadType == HEAD_TYPE_SR) {
				m_strRecalib2HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
			} else {
				m_strRecalib2HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
			}
// 2009.09.30 bagus Gantry --}--
#else
// 2009.12.08 bagus --{--
#if 0
			switch (RecalibProgInfo.wHeadType) {
			case HEAD_TYPE_SR:
				m_strRecalib2HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
				break;
			case HEAD_TYPE_4PP:
				m_strRecalib2HeadType = SCAN_TYPE_ITEM_RS;
				break;
			default:
				m_strRecalib2HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
				break;
			}
#else
			switch (RecalibProgInfo.wHeadType) {
			case HEAD_TYPE_SR:
// 2009.12.09 bagus --{--
//				m_strRecalib2HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
				m_strRecalib2HeadType.Format("%s%s", HEAD_TYPE_ITEM_SHORT[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
// 2009.12.09 bagus --}--
				break;
			default:
// 2009.12.09 bagus --{--
//				m_strRecalib2HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
				m_strRecalib2HeadType = HEAD_TYPE_ITEM_SHORT[RecalibProgInfo.wHeadType];
// 2009.12.09 bagus --}--
				break;
			}
#endif
// 2009.12.08 bagus --}--
#endif
// 2009.11.04 bagus RS 追加 --}--
	}
	else {
		m_strRecalib2HeadType.Empty();
	}

	UpdateData(FALSE);
}

// =========================================================================
// リキャリブレーション３ コンボ選択時 ヘッドタイプの編集
//
void CRecipeMainSrView::OnSelchangeRecalibration3()
{
	UpdateData(TRUE);

	RECALIB_PROG_INFO RecalibProgInfo;
	// コンボボックス選択された名称の取得
	CString strBuffer = m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3];
	///// Recalibration 3 - Head Type /////
//	if ( strBuffer.GetLength() && strBuffer != "None" ) {
	//Saiki 20090526 Change ----->
	CString strBuf;
	//if ( strBuffer.GetLength() && strBuffer != "無し" ) {
	LoadStringML(IDS_POINT_NAME_NONE, strBuf, "None");
	if ( strBuffer.GetLength() && strBuffer != strBuf ) {
	//Saiki 20090526 Change <-----
		if ( !RecipeFile_LoadRecipe(&RecalibProgInfo, strBuffer, RECIPE_FILE_RECALIBRATION_PROGRAM) )
			m_strRecalib3HeadType.Empty();
		else
// 2009.11.04 bagus RS 追加 --{--
#if 0
// 2009.09.30 bagus Gantry --{--
//			m_strRecalib3HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
			if (RecalibProgInfo.wHeadType == HEAD_TYPE_SR) {
				m_strRecalib3HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
			} else {
				m_strRecalib3HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
			}
// 2009.09.30 bagus Gantry --}--
#else
// 2009.12.08 bagus --{--
#if 0
			switch (RecalibProgInfo.wHeadType) {
			case HEAD_TYPE_SR:
				m_strRecalib3HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
				break;
			case HEAD_TYPE_4PP:
				m_strRecalib3HeadType = SCAN_TYPE_ITEM_RS;
				break;
			default:
				m_strRecalib3HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
				break;
			}
#else
			switch (RecalibProgInfo.wHeadType) {
			case HEAD_TYPE_SR:
// 2009.12.09 bagus --{--
//				m_strRecalib3HeadType.Format("%s%s", HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
				m_strRecalib3HeadType.Format("%s%s", HEAD_TYPE_ITEM_SHORT[RecalibProgInfo.wHeadType], m_szScanTypeItem[RecalibProgInfo.wScanType]);
// 2009.12.09 bagus --}--
				break;
			default:
// 2009.12.09 bagus --{--
//				m_strRecalib3HeadType = HEAD_TYPE_ITEM[RecalibProgInfo.wHeadType];
				m_strRecalib3HeadType = HEAD_TYPE_ITEM_SHORT[RecalibProgInfo.wHeadType];
// 2009.12.09 bagus --}--
				break;
			}
#endif
// 2009.12.08 bagus --}--
#endif
// 2009.11.04 bagus RS 追加 --}--
	}
	else {
		m_strRecalib3HeadType.Empty();
	}

	UpdateData(FALSE);
}

// =========================================================================
// ポイントマニュアルアジャストメント コンボ選択時
//
void CRecipeMainSrView::OnSelchangePointManualAdjustment()
{
//	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = nSelect;

//	UpdateData(FALSE);
}

// =========================================================================
// サンプルID コンボ選択時
//
void CRecipeMainSrView::OnSelchangeSampleId()
{
//	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SAMPLE_ID);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_MainRcpInfo.MainRcpParam.hdr.bSampleID = nSelect;

//	UpdateData(FALSE);
}

// =========================================================================
// フォーカス コンボ選択時
//
void CRecipeMainSrView::OnSelchangeFocus()
{
//	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_FOCUS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_MainRcpInfo.MainRcpParam._SR.nFocus = nSelect;

//	UpdateData(FALSE);
}

// =========================================================================
// フォーカス失敗時動作 コンボ選択時
//
void CRecipeMainSrView::OnSelchangeFocusIfFail()
{
//	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_FOCUS_IF_FAIL);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption = nSelect;
	if(nSelect == AFOCUS_FAIL_OPTION_AUTO_RETRY_CANCEL_RECIPE || nSelect == AFOCUS_FAIL_OPTION_CANCEL_RECIPE){
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(FALSE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			GetDlgItem(IDC_FINAL_FOCUS)->EnableWindow(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	}

//	UpdateData(FALSE);
}

// =========================================================================
// デフォルトディスプレイ コンボ選択時
//
void CRecipeMainSrView::OnSelchangeDefaultDisplay()
{
//	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DEFAULT_DISPLAY);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay = nSelect;

//	UpdateData(FALSE);
}

// =========================================================================
//
void CRecipeMainSrView::LoadRecipeData()
{
	//Saiki 20090526 Add ----->
	CString strBuffer;
	//Saiki 20090526 Add <-----
	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		//2009.08.28 bagus strsss --{--
		//SR/STRESS画面共用の為、変更。
		//if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_MAIN_RECIPE) ) {
		//	m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
		//}
		switch(m_iNewHeadType){
		case HEAD_TYPE_SR:
		default:
			if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_MAIN_RECIPE) ) {
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
			}
			break;
		//2009.09.07 bagus se --{--
		case HEAD_TYPE_SE:
			if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, SE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SE_MAIN_RECIPE) ) {
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SE;
			}
			break;
		//2009.09.07 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
			if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, COMPEASE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE) ) {
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_COMPEASE;
				// デフォルト値設定
				m_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult = TRUE;
				m_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate = TRUE;
			}
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		case HEAD_TYPE_STRESS:
//Saiki 20110303 Change ----->
			//if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS_MAIN_RECIPE) ) {
			//	  m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_STRESS;
			//}
			if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS_MAIN_RECIPE) ) {
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_STRESS;
			}
			if(!RecipeFile_LoadStressParam(&m_StressParamInfo, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS_MAIN_RECIPE)){
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_STRESS;
			}
//Saiki 20110303 Change <-----
			break;
		// 2009.10.08 bagus CTA 追加 --{--
		case HEAD_TYPE_CTA:
			if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, CTA_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_CTA_MAIN_RECIPE) ) {
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_CTA;
			}
			break;
		// 2009.10.08 bagus CTA 追加 --}--
		//2009.10.20 bagus MS 追加 --{--
		case HEAD_TYPE_MS:
			if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, MS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_MS_MAIN_RECIPE) ) {
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_MS;
			}
			break;
		//2009.10.20 bagus MS 追加 --}--
		// 2009.11.04 bagus RS 追加 --{--
		case HEAD_TYPE_4PP:
			if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, RES_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_4PP_MAIN_RECIPE) ) {
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_4PP;
			}
			break;
		// 2009.11.04 bagus RS 追加 --}--
		}
		//2009.08.28 bagus strsss --}--
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, m_szRecipeName, RECIPE_FILE_MAIN_RECIPE) ) {
			m_nOpenMode = modeNew;
			//2009.08.28 bagus stress --{--
			//SR/STRESS画面共用のために変更
			//if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_MAIN_RECIPE) ) {
			//	m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
			//}
			switch(m_iNewHeadType){
			case HEAD_TYPE_SR:
			default:
				if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_MAIN_RECIPE) ) {
					m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
				}
				break;
			//2009.09.07 bagus se --{--
			case HEAD_TYPE_SE:
				if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, SE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SE_MAIN_RECIPE) ) {
					m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SE;
				}
				break;
			//2009.09.07 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			case HEAD_TYPE_COMPEASE:
				if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, COMPEASE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE) ) {
					m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_COMPEASE;
				}
				break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			case HEAD_TYPE_STRESS:
				if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS_MAIN_RECIPE) ) {
					//Saiki 20110215 Change ----->
					//m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_STRESS;
					if ( !RecipeFile_LoadRecipe(&m_StressParamInfo, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS_MAIN_RECIPE) ) {
						m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_STRESS;
					}
					//Saiki 20110215 Change <-----
				}
				break;
			// 2009.10.08 bagus CTA 追加 --{--
			case HEAD_TYPE_CTA:
				if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, CTA_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_CTA_MAIN_RECIPE) ) {
					m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_CTA;
				}
				break;
			// 2009.10.08 bagus CTA 追加 --}--
			//2009.10.20 bagus MS 追加 --{--
			case HEAD_TYPE_MS:
				if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, MS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_MS_MAIN_RECIPE) ) {
					m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_MS;
				}
				break;
			//2009.10.20 bagus MS 追加 --}--
			// 2009.11.04 bagus RS 追加 --{--
			case HEAD_TYPE_4PP:
				if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, RES_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_4PP_MAIN_RECIPE) ) {
					m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_4PP;
				}
				break;
			// 2009.11.04 bagus RS 追加 --}--
			}
			//2009.08.28 bagus strsss --}--
		}
		//Saiki 20110304 Add ----->
		//if(m_iNewHeadType == HEAD_TYPE_STRESS){
			if(!RecipeFile_LoadStressParam(&m_StressParamInfo, m_szRecipeName, RECIPE_FILE_STRESS_MAIN_RECIPE)){
				m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_STRESS;
			}
		//}
		//Saiki 20110304 Add <-----
		break;
	}
	::CopyMemory(&m_OldMainRcpInfo, &m_MainRcpInfo, sizeof(MAIN_RCP_INFO));
	if(m_iNewHeadType == HEAD_TYPE_STRESS){
		::CopyMemory(&m_OldStressParamInfo, &m_StressParamInfo, sizeof(STRESS_PARAM_INFO));
	}


	if ( m_nOpenMode == modeNew ) {
//		strcpy(m_MainRcpInfo.hdr.szName, "Untitled");
		//Saiki 20090526 Change ----->
		//strcpy(m_MainRcpInfo.hdr.szName, "タイトルなし");
		LoadStringML(IDS_UNTITLE, strBuffer, "Untitled");
		strcpy(m_MainRcpInfo.hdr.szName, strBuffer);
		//Saiki 20090526 Change <-----

	}
}

// =========================================================================
//
void CRecipeMainSrView::InitCombo_MeasurementProgram()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MEASUREMENT_PROGRAM);
	pCombo->ResetContent();

	CFileFind Finder;
	CString strName;
	CString strDirPath;
	strDirPath.Format("%s*%s", g_szDb_Measurement_Program_Dir, MEASUREMENTPGM_EXT);
	if ( !Finder.FindFile(strDirPath) )
		return;

	//Saiki 20090410 Change ----->
	//BOOL bLoop;
	BOOL bLoop = TRUE;
	//Saiki 20090410 Change <-----
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		strName = Finder.GetFileTitle();
		if ( strName[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		if ( !RecipeFile_LoadRecipe(&m_MeasProgInfo, strName, RECIPE_FILE_MEASUREMENT_PROGRAM) )
			continue;
		// 2009.08.28 bagus stress --{--
		// メインレシピのヘッドタイプと同一のヘッドタイプの測定PGMのみを表示する
		//if ( m_MeasProgInfo.ScanParams.hdr.wHeadType != HEAD_TYPE_SR )
		if (m_MeasProgInfo.ScanParams.hdr.wHeadType != m_MainRcpInfo.MainRcpParam.hdr.wHeadType)
		// 2009.08.28 bagus stress --}--
			continue;
		pCombo->AddString(strName);
	}

	int nIndex = pCombo->FindStringExact(-1, m_MainRcpInfo.MainRcpParam.hdr.szMeas);
	pCombo->SetCurSel(nIndex);

// 2009.10.02 bagus Gantry --{--
	m_bChage = TRUE;
	OnSelchangeMeasurementProgram();
	m_bChage = FALSE;
// 2009.10.02 bagus Gantry --}--

	// コンボボックス選択された名称の取得
	CString strBuffer = m_MainRcpInfo.MainRcpParam.hdr.szMeas;
	///// Measurement Program - Head Type /////
	if ( strBuffer.GetLength() ) {
		if ( !RecipeFile_LoadRecipe(&m_MeasProgInfo, strBuffer, RECIPE_FILE_MEASUREMENT_PROGRAM) )
			m_strMeasHeadType.Empty();
		else
// 2009.11.04 bagus RS 追加 --{--
#if 0
// 2009.09.30 bagus Gantry --{--
//			m_strMeasHeadType = HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType];
			if (m_MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR) {
				m_strMeasHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType], m_szScanTypeItem[m_MeasProgInfo.ScanParams.hdr.wScanType]);
			} else {
				m_strMeasHeadType = HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType];
			}
// 2009.09.30 bagus Gantry --}--
#else
// 2009.12.08 bagus --{--
#if 0
			switch (m_MeasProgInfo.ScanParams.hdr.wHeadType) {
			case HEAD_TYPE_SR:
				m_strMeasHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType], m_szScanTypeItem[m_MeasProgInfo.ScanParams.hdr.wScanType]);
				break;
			case HEAD_TYPE_4PP:
				m_strMeasHeadType = SCAN_TYPE_ITEM_RS;
				break;
			default:
				m_strMeasHeadType = HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType];
				break;
			}
#else
			switch (m_MeasProgInfo.ScanParams.hdr.wHeadType) {
			case HEAD_TYPE_SR:
// 2009.12.09 bagus --{--
//				m_strMeasHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType], m_szScanTypeItem[m_MeasProgInfo.ScanParams.hdr.wScanType]);
				m_strMeasHeadType.Format("%s%s", HEAD_TYPE_ITEM_SHORT[m_MeasProgInfo.ScanParams.hdr.wHeadType], m_szScanTypeItem[m_MeasProgInfo.ScanParams.hdr.wScanType]);
// 2009.12.09 bagus --}--
				break;
			default:
// 2009.12.09 bagus --{--
//				m_strMeasHeadType = HEAD_TYPE_ITEM[m_MeasProgInfo.ScanParams.hdr.wHeadType];
				m_strMeasHeadType = HEAD_TYPE_ITEM_SHORT[m_MeasProgInfo.ScanParams.hdr.wHeadType];
// 2009.12.09 bagus --}--
				break;
			}
#endif
// 2009.12.08 bagus --}--
#endif
// 2009.11.04 bagus RS 追加 --}--
			InitCombo_RecalibItemSetting(m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1],m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2],m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]);
	}
	else {
		m_strMeasHeadType.Empty();
	}
}

// =========================================================================
//
void CRecipeMainSrView::InitCombo_StageProgram()
{
// 2009.10.09 bagus StagePGM 共通化 --{--
#if 0
	//Saiki 20090526 Add ----->
	CString strBuffer;
	//Saiki 20090526 Add <-----
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STAGE_PROGRAM);
	pCombo->ResetContent();
//	pCombo->AddString("None");
	//Saiki 20090526 Change ----->
	//pCombo->AddString("無し");
	LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
	pCombo->AddString(strBuffer);
	//Saiki 20090526 Change <-----

	//2009.09.02 bagus se ステージ種絞込み --{--
	STAGE_PROG_INFO_HDR StageProgInfoHdr;
	//2009.09.02 bagus se ステージ種絞込み --}--

	CString strName;
	CString strDirPath;
	strDirPath.Format("%s*%s", g_szDb_Stage_Program_Dir, STAGEPGM_EXT);
	CFileFind Finder;
	if ( !Finder.FindFile(strDirPath) )
		return;

	//Saiki 20090410 Change ----->
	//BOOL bLoop;
	BOOL bLoop = TRUE;
	//Saiki 20090410 Change <-----
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		// ファイル名を取得
		strName = Finder.GetFileTitle();
		if ( strName[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;

		//2009.09.02 bagus stress ステージ種絞込み --{--
		//メインレシピと同一のヘッドタイプを持つステージPGMのみを表示する
		//2009.09.07 bagus se 但しSRとSEは共用
		if ( !RecipeFile_LoadRecipe(&StageProgInfoHdr, strName.GetBuffer(0), RECIPE_FILE_STAGE_PROGRAM) )
			continue;
		if (StageProgInfoHdr.wHeadType != m_MainRcpInfo.MainRcpParam.hdr.wHeadType){
			//2009.09.07 bagus se --{--
			//SR/SE は共用
			if((m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR || m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SE)
				&& (StageProgInfoHdr.wHeadType == HEAD_TYPE_SR || StageProgInfoHdr.wHeadType == HEAD_TYPE_SE)){
				//SR/SEは共用なのでOK
			}else{
				continue;
			}
			//2009.09.07 bagus se --}--
		}
		//2009.09.02 bagus se ステージ種絞込み --}--

// 2009.10.02 bagus Gantry --{--
// 2009.10.08 bagus CTA 追加 --{--
#if 0
		switch (m_SRScanType){
		case MEAS_PROG_TYPE_SR_THICKNESS:
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			if (StageProgInfoHdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
				continue;
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
			if ((!m_SrConfig.bGantryType) && (StageProgInfoHdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))
				continue;
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			if (StageProgInfoHdr.wScanType != MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
				continue;
			if (!m_SrConfig.bGantryType)
				continue;
			break;
		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
		default:
			if (StageProgInfoHdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
				continue;
			break;
		}
#else
		switch ( m_iNewHeadType ) {
		case HEAD_TYPE_SR:
			if (StageProgInfoHdr.wHeadType != m_iNewHeadType)
				continue;

		switch (m_SRScanType){
		case MEAS_PROG_TYPE_SR_THICKNESS:
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			if (StageProgInfoHdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
				continue;
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
			if ((!m_SrConfig.bGantryType) && (StageProgInfoHdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))
				continue;
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			if (StageProgInfoHdr.wScanType != MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
				continue;
			if (!m_SrConfig.bGantryType)
				continue;
			break;
// 2009.10.28 bagus 2点間 --{--
		case MEAS_PROG_TYPE_SR_DISTANCE:
			if (StageProgInfoHdr.wScanType != MEAS_PROG_TYPE_SR_DISTANCE)
				continue;
			break;
// 2009.10.28 bagus 2点間 --}--
		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
		default:
			if (StageProgInfoHdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
				continue;
			break;
		}
		case HEAD_TYPE_STRESS:
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		case HEAD_TYPE_CTA:
		//2009.10.20 bagus MS 追加 --{--
		case HEAD_TYPE_MS:
		//2009.10.20 bagus MS 追加 --}--
		// 2009.11.04 bagus RS 追加 --{--
		case HEAD_TYPE_4PP:
		// 2009.11.04 bagus RS 追加 --}--
		default:
			if (StageProgInfoHdr.wHeadType != m_iNewHeadType)
				continue;

			break;
		}
#endif
// 2009.10.08 bagus CTA 追加 --}--
// 2009.10.02 bagus Gantry --}--

		pCombo->AddString(strName);
	}
	int nIndex = pCombo->FindStringExact(-1, m_MainRcpInfo.MainRcpParam.hdr.szStage);

	if((m_nOpenMode == modeNew )&&(nIndex == -1))
	{
		nIndex = 0;
	}

	pCombo->SetCurSel(nIndex);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
//2009.09.02 bagus se 機能拡張 --{--
		OnSelchangeStageProgram();
//2009.09.02 bagus se 機能拡張 --}--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
#else
	CString strBuffer;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STAGE_PROGRAM);
	pCombo->ResetContent();
	LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
	pCombo->AddString(strBuffer);

	CMainFrame* 	pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CNanoSpecDoc*	pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	int nStagePGM;

	STAGE_PROG_INFO_HDR StageProgInfoHdr;

	CString strName;
	CString strDirPath;
	strDirPath.Format("%s*%s", g_szDb_Stage_Program_Dir, STAGEPGM_EXT);
	CFileFind Finder;
	if ( !Finder.FindFile(strDirPath) )
		return;

	BOOL bLoop = TRUE;
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		// ファイル名を取得
		strName = Finder.GetFileTitle();
		if ( strName[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;

		//メインレシピと同一のヘッドタイプを持つステージPGMのみを表示する
		if ( !RecipeFile_LoadRecipe(&StageProgInfoHdr, strName.GetBuffer(0), RECIPE_FILE_STAGE_PROGRAM) )
			continue;

		// 旧型→新型へのヘッドタイプ変換
		nStagePGM = pDoc->GetStagePGM(m_MainRcpInfo.MainRcpParam.hdr.wHeadType, m_SRScanType);

		switch ( m_iNewHeadType ) {
		case HEAD_TYPE_SR:
			switch (m_SRScanType){
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
				if ((m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR) && (StageProgInfoHdr.wScanType == SCAN_TYPE_SR_GANTRY)) {
					// OK
				} else if (StageProgInfoHdr.wHeadType == nStagePGM) {
					// OK
				} else {
					// NG
					continue;
				}
				break;
// 2009.10.28 bagus 2点間 --{--
			case MEAS_PROG_TYPE_SR_DISTANCE:
				if ((m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR) && (StageProgInfoHdr.wScanType == SCAN_TYPE_SR_DISTANCE)) {
					// OK
				} else if (StageProgInfoHdr.wHeadType == nStagePGM) {
					// OK
				} else {
					// NG
					continue;
				}
				break;
// 2009.10.28 bagus 2点間 --}--
			case MEAS_PROG_TYPE_SR_THICKNESS:
			case MEAS_PROG_TYPE_SR_REFLECTANCE:
			case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
			case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
			default:
// 2009.10.23 bagus Stress 修正 --{--
// 2009.10.23 bagus Stress 修正 --}--
				if (StageProgInfoHdr.wHeadType != nStagePGM)
					continue;
				break;
			}
			break;
		case HEAD_TYPE_STRESS:
// 2009.10.23 bagus Stress 修正 --{--
			if (StageProgInfoHdr.wHeadType != nStagePGM)
				continue;
			break;
// 2009.10.23 bagus Stress 修正 --}--
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		case HEAD_TYPE_CTA:
		//2009.10.20 bagus MS 追加 --{--
		case HEAD_TYPE_MS:
		//2009.10.20 bagus MS 追加 --}--
// 2009.10.23 bagus Stress 修正 --{--
#if 0
		default:
			if (StageProgInfoHdr.wHeadType != nStagePGM)
				continue;
			break;
#else
		case HEAD_TYPE_4PP:
		default:
			if ((StageProgInfoHdr.wHeadType != nStagePGM) && (StageProgInfoHdr.wHeadType != m_iNewHeadType))
				continue;
			break;
#endif
// 2009.10.23 bagus Stress 修正 --}--
		}

		pCombo->AddString(strName);
	}
	int nIndex = pCombo->FindStringExact(-1, m_MainRcpInfo.MainRcpParam.hdr.szStage);

	if((m_nOpenMode == modeNew )&&(nIndex == -1))
	{
		nIndex = 0;
	}

	pCombo->SetCurSel(nIndex);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		OnSelchangeStageProgram();
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
#endif
// 2009.10.09 bagus StagePGM 共通化 --}--
}

// =========================================================================
//
void CRecipeMainSrView::InitCombo_PointDeskewProgram()
{
	//Saiki 20090526 Add ----->
	CString strBuffer;
	//Saiki 20090526 Add <-----
	//// Point Deskew の設定項目の表示/非表示
//★暫定対応　忘れるな！
#if 0
	if ( m_SystemConfig.nPRMethod <=0 || m_SystemConfig.bSitePatternRec==FALSE){
		GetDlgItem(IDC_POINT_DESKEW_PROGRAM_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_DESKEW_PROGRAM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STR_POINT_DESKEW_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STR_POINT_DESKEW_LENS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_DESKEW_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_DESKEW_LENS)->ShowWindow(SW_HIDE);
		return;
	}
#else
		GetDlgItem(IDC_POINT_DESKEW_PROGRAM_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_DESKEW_PROGRAM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STR_POINT_DESKEW_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STR_POINT_DESKEW_LENS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_DESKEW_HEAD_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_DESKEW_LENS)->ShowWindow(SW_HIDE);
		return;
#endif

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_POINT_DESKEW_PROGRAM);

	pCombo->ResetContent();
//	pCombo->AddString("None");
	//Saiki 20090526 Change ----->
	//pCombo->AddString("無し");
	LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
	pCombo->AddString(strBuffer);
	//Saiki 20090526 Change <-----

	POINT_DESKEW_PROG_INFO PointDeskewProgInfo;
	CFileFind Finder;
	CString strName;
	CString strDirPath;
	strDirPath.Format("%s*%s", g_szDb_Point_Deskew_Program_Dir, POINTDESKEWPGM_EXT);
	if ( !Finder.FindFile(strDirPath) )
		return;

	//Saiki 20090410 Change ----->
	//BOOL bLoop;
	BOOL bLoop = TRUE;
	//Saiki 20090410 Change <-----
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		// ファイル名を取得
		strName = Finder.GetFileTitle();
		if ( strName[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		if ( !RecipeFile_LoadRecipe(&PointDeskewProgInfo, strName, RECIPE_FILE_POINT_DESKEW_PROGRAM) )
			continue;
		// 2009.08.28 bagus stress --{--
		//if ( PointDeskewProgInfo.wHeadType != HEAD_TYPE_SR )
		if ( PointDeskewProgInfo.wHeadType != m_MainRcpInfo.MainRcpParam.hdr.wHeadType )
		// 2009.08.28 bagus stress --}--
			continue;
		pCombo->AddString(strName);
	}

	int nIndex = pCombo->FindStringExact(-1, m_MainRcpInfo.MainRcpParam.hdr.szPointDeskew);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
void CRecipeMainSrView::InitCombo_RecalibrationProgram(CComboBox* pCombo, LPCTSTR pszRecalib)
{
//	// メジャメントレシピの取得
//	LPCTSTR pszMeas = m_MainRcpInfo.MainRcpParam.hdr.szMeas;
//	if ( !RecipeFile_LoadRecipe(&m_MeasProgInfo, pszMeas, RECIPE_FILE_MEASUREMENT_PROGRAM) )
//		return;

	//Saiki 20090526 Add ----->
	CString strBuffer;
	//Saiki 20090526 Add <-----
	// コンボボックスの編集
	pCombo->ResetContent();
//	pCombo->AddString("None");
	//Saiki 20090526 Change ----->
	//pCombo->AddString("無し");
	LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
	pCombo->AddString(strBuffer);
	//Saiki 20090526 Change <-----

	RECALIB_PROG_INFO RecalibProgInfo;
	CFileFind Finder;
	CString strName;
	CString strDirPath;
	strDirPath.Format("%s*%s", g_szDb_Recalibration_Program_Dir, RECALIBPGM_EXT);

//OkabeChange20090525--->
	if ( Finder.FindFile(strDirPath) )
	{
		//Saiki 20090410 Change ----->
		//BOOL bLoop;
		BOOL bLoop = TRUE;
		//Saiki 20090410 Change <-----
		while ( bLoop ) {
			bLoop = Finder.FindNextFile();
			// ファイル名を取得
			strName = Finder.GetFileTitle();
			if ( strName[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
				continue;
			if ( !RecipeFile_LoadRecipe(&RecalibProgInfo, strName, RECIPE_FILE_RECALIBRATION_PROGRAM) )
				continue;
			// 2009.08.28 bagus stress --{--
			//if ( RecalibProgInfo.wHeadType != HEAD_TYPE_SR )
			if ( RecalibProgInfo.wHeadType != m_MainRcpInfo.MainRcpParam.hdr.wHeadType )
			// 2009.08.28 bagus stress --}--
				continue;

			//2009.12.07 bagus SR 修正 --{--
			if( RecalibProgInfo.wHeadType == HEAD_TYPE_SR){
				if( (m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE || m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
					if(RecalibProgInfo.wScanType != MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
					&& RecalibProgInfo.wScanType != MEAS_PROG_TYPE_SR_TRANSMITTANCE){
						continue;
					}
				}else{
					if(RecalibProgInfo.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
					|| RecalibProgInfo.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE){
						continue;
					}
				}
			}
			//2009.12.07 bagus SR 修正 --}--

// 2009.10.27 bagus Gantry 追加修正 --{--
#if 0
// 2009.10.02 bagus Gantry --{--
			if (RecalibProgInfo.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
				continue;
// 2009.10.02 bagus Gantry --}--
#endif
// 2009.10.27 bagus Gantry 追加修正 --}--

			pCombo->AddString(strName);
		}
	}

/*
	if ( !Finder.FindFile(strDirPath) )
		return;
	//Saiki 20090410 Change ----->
	//BOOL bLoop;
	BOOL bLoop = TRUE;
	//Saiki 20090410 Change <-----
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		// ファイル名を取得
		strName = Finder.GetFileTitle();
		if ( strName[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		if ( !RecipeFile_LoadRecipe(&RecalibProgInfo, strName, RECIPE_FILE_RECALIBRATION_PROGRAM) )
			continue;
		if ( RecalibProgInfo.wHeadType != HEAD_TYPE_SR )
			continue;
//		if ( RecalibProgInfo.wScanType != m_MeasProgInfo.ScanParams.hdr.wScanType )
//			continue;
		pCombo->AddString(strName);
	}
*/
//OkabeChange20090525<---

	int nIndex = pCombo->FindStringExact(-1, pszRecalib);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
void CRecipeMainSrView::InitCombo_PointManualAdjustment()
{
	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_POINT_MANUAL_ADJUSTMENT);
	pCombo->ResetContent();
	//Saiki 20090526 Add ----->
	CString strBuffer, strTitle;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090526 Add <-----

	//Saiki 20090527 Change ----->
	//for ( int i = 0; i < POINT_MANUAL_ADJUSTMENT_MAX; i++ )
	//	  pCombo->AddString(g_lpszPointManualAdjustment[i]);
	for ( int i = 0; i < POINT_MANUAL_ADJUSTMENT_MAX; i++ ){
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszPointManualAdjustment_ENU[i]);
		}
		else{
			pCombo->AddString(g_lpszPointManualAdjustment_JPN[i]);
		}
	}

	//Saiki 20090527 Change <-----
	//Saiki 20090406 Change ----->
	//pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment);
	if(!(m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment >= POINT_MANUAL_ADJUSTMENT_MAX)){
		pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment);
	}
	else{
		if(m_nOpenMode == modeNew){
			m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = 0;
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment);
		}
		else{
			m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = 0;
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment);
			SaveRecipeData();
			//Saiki 20090526 Change ----->
			//MessageBox("指定した項目が無効のため、測定ポジションの補正 はINDEXを0として保存しました。", "NanoSpec", MB_OK);
			LoadStringML(IDS_POINT_ADJUSTMENT_ZERO, strBuffer, "Invalid for the specified item, Correction of the measurement position is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK);
			//Saiki 20090526 Change <-----
		}
	}
	//Saiki 20090406 Change <-----
}


// =========================================================================
//
void CRecipeMainSrView::InitCombo_Focus()
{
	//Saiki 20090526 Add ----->
	CString strBuffer, strTitle;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090526 Add <-----
	if (m_SrConfig.bAutoFocus){
		CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_FOCUS);
		pCombo->ResetContent();

		//Saiki 20090527 Change ----->
		//for ( int i = 0; i < SR_FOCUS_MAX; i++ )
		//	  pCombo->AddString(g_lpszSrFocus[i]);
		for ( int i = 0; i < SR_FOCUS_MAX; i++ ){
			if(l_SystemConfig.nLanguage == 0){
				pCombo->AddString(g_lpszSrFocus_ENU[i]);
			}
			else{
				pCombo->AddString(g_lpszSrFocus_JPN[i]);
			}
		}
		//Saiki 20090527 Change <-----
		//Saiki 20090406 Change ----->
		//pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nFocus);
		if(!(m_MainRcpInfo.MainRcpParam._SR.nFocus >= SR_FOCUS_MAX)){
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nFocus);
		}
		else{
			if(m_nOpenMode == modeNew){
				m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
				pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nFocus);
			}
			else{
				m_MainRcpInfo.MainRcpParam._SR.nFocus = 0;
				pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nFocus);
				SaveRecipeData();
				//Saiki 20090526 Change ----->
				//MessageBox("指定した項目が無効のため、フォーカス はINDEXを0として保存しました。", "NanoSpec", MB_OK);
				LoadStringML(IDS_FOCUS_ZERO, strBuffer, "Invalid for the specified item, Focus is saved as an INDEX to 0.");
				LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
				MessageBox(strBuffer, strTitle, MB_OK);
				//Saiki 20090526 Change <-----
			}
		}
		//Saiki 20090406 Change <------

		InitCombo_FocusIfFail();

// 2009.10.22 bagus Stress 修正 --{--
//		if (!m_SrConfig.bChooseAfFuncInReference){
//			GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
//		}
// 2009.10.22 bagus Stress 修正 --}--
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			GetDlgItem(IDC_FOCUS_LABEL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_FOCUS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_FOCUS_IF_FAIL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_FOCUS_IF_FAIL_LABEL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_FINAL_FOCUS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_FINAL_FOCUS_LABEL)->ShowWindow(SW_HIDE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 2009.10.22 bagus Stress 修正 --{--
//		GetDlgItem(IDC_REFERENCE_WITH_AF)->ShowWindow(SW_HIDE);
// 2009.10.22 bagus Stress 修正 --}--
	}

}

// =========================================================================
//
void CRecipeMainSrView::InitCombo_FocusIfFail()
{
	//Saiki 20090526 Add ----->
	CString strBuffer, strTitle;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090526 Add <-----
	CComboBox* pCombo;
	pCombo= (CComboBox *)GetDlgItem(IDC_FOCUS_IF_FAIL);
	pCombo->ResetContent();

	//Saiki 20090527 Change ----->
	//for ( int i = 0; i < AFOCUS_FAIL_OPTION_MAX; i++ )
	//	  pCombo->AddString(g_lpszSrFocusIfFail[i]);
	for ( int i = 0; i < AFOCUS_FAIL_OPTION_MAX; i++ ){
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSrFocusIfFail_ENU[i]);
		}
		else{
			pCombo->AddString(g_lpszSrFocusIfFail_JPN[i]);
		}
	}
	//Saiki 20090527 Change <-----
	//Saiki 20090406 Change ----->
	//pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption);
	if(!(m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption >= AFOCUS_FAIL_OPTION_MAX))
		pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption);
	else{
		if(m_nOpenMode == modeNew){
			m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption = 0;
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption);
		}
		else{
			m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption = 0;
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption);
			SaveRecipeData();
			//Saiki 20090526 Change ----->
			//MessageBox("指定した項目が無効のため、オートフォーカス失敗時 はINDEXを0として保存しました。", "NanoSpec", MB_OK);
			LoadStringML(IDS_AUTOFOCUS_ZERO, strBuffer, "Invalid for the specified item, <AutoFocus Fail>is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK);
			//Saiki 20090526 Change <-----
		}
	}
	//Saiki 20090406 Change <-----

	pCombo = (CComboBox *)GetDlgItem(IDC_FINAL_FOCUS);
	pCombo->ResetContent();
	//Saiki 20090527 Change ----->
	//for ( i = 0; i < FANAL_FOCUS_FAIL_DATA_VALID_OPTION_MAX; i++ )
	//	  pCombo->AddString(g_lpszSrFocusIfFailDataValid[i]);
	for ( i = 0; i < FANAL_FOCUS_FAIL_DATA_VALID_OPTION_MAX; i++ ){
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSrFocusIfFailDataValid_ENU[i]);
		}
		else{
			pCombo->AddString(g_lpszSrFocusIfFailDataValid_JPN[i]);
		}
	}
	//Saiki 20090527 Change <-----
	//Saiki 20090406 Change ----->
	//pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption);
	if(!(m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption >= FANAL_FOCUS_FAIL_DATA_VALID_OPTION_MAX))
		pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption);
	else{
		if(m_nOpenMode == modeNew){
			m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption = 0;
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption);
		}
		else{
			m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption = 0;
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption);
			SaveRecipeData();
			//Saiki 20090526 Change ----->
			//MessageBox("指定した項目が無効のため、INDEXを0として保存しました。", "NanoSpec", MB_OK);
			LoadStringML(IDS_INDEX_ZERO, strBuffer, "Invalid for the specified item,  saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK);
			//Saiki 20090526 Change <-----
		}
	}
	//Saiki 20090406 Change <-----
}

// =========================================================================
//
void CRecipeMainSrView::InitCombo_DefaultDisplay()
{
	//Kojika 20090527 Add ----->
	CString strBuffer, strTitle;
	//Kojika 20090526 Add <-----

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_DEFAULT_DISPLAY);
	pCombo->ResetContent();

	//Saiki 20090527 Change ----->
	//for ( int i = 0; i < SR_DEFAULT_DISPLAY_MAX; i++ )
	//	  pCombo->AddString(g_lpszSrDefaultDisplay[i]);
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

//2009.10.20 bagus MS 追加 --{--
	switch (m_iNewHeadType) {
// 2009.10.28 bagus 2点間 --{--
	case HEAD_TYPE_SR:
		switch (m_SRScanType) {
		case MEAS_PROG_TYPE_SR_DISTANCE:
			if(l_SystemConfig.nLanguage == 0){
				pCombo->AddString(g_lpszSrDefaultDisplay_ENU[0]);
			} else {
				pCombo->AddString(g_lpszSrDefaultDisplay_JPN[0]);
			}
			pCombo->SetCurSel(0);
			return;
			break;
		default:
			break;
		}
		break;
// 2009.10.28 bagus 2点間 --}--
// 2009.10.28 bagus CTA 追加修正 --{--
	case HEAD_TYPE_CTA:
// 2009.10.28 bagus CTA 追加修正 --}--
	case HEAD_TYPE_MS:
// 2009.11.04 bagus RS 追加 --{--
	case HEAD_TYPE_4PP:
// 2009.11.04 bagus RS 追加 --}--
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSrDefaultDisplay_ENU[0]);
		} else {
			pCombo->AddString(g_lpszSrDefaultDisplay_JPN[0]);
		}
		pCombo->SetCurSel(0);
		return;
		break;
	default:
		break;
	}
//2009.10.20 bagus MS 追加 --}--

	for ( int i = 0; i < SR_DEFAULT_DISPLAY_MAX; i++ ){
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSrDefaultDisplay_ENU[i]);
		}
		else{
			pCombo->AddString(g_lpszSrDefaultDisplay_JPN[i]);
		}

	}
	//Saiki 20090527 Change <-----
	//Saiki 20090406 Change ----->
	//pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay);
	if(!(m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay >= SR_DEFAULT_DISPLAY_MAX))
		pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay);
	else{
		if(m_nOpenMode == modeNew){
			m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay = 0;
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay);
		}
		else{
			m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay = 0;
			pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay);
			SaveRecipeData();
			//Kojika 20090527 Change
			//MessageBox("指定した項目が無効のため、初期表示画面はINDEXを0として保存しました。", "NanoSpec", MB_OK);
			LoadStringML(IDS_INIT_DISPLAY_ZERO, strBuffer, "Invalid for the specified item, Initial display screen is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK);
			//Kojika 20090527 Change End
		}
	}
	//Saiki 20090406 Change <-----
}

// =========================================================================
//
void CRecipeMainSrView::DefaultSetting_Init()
{
	CComboBox* pCombo;

	///// Name /////
	GetDlgItem(IDC_MAIN_RECIPE_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MAIN_RECIPE_NAME_STATIC)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_MAIN_RECIPE_NAME, "");

	///// Head Type /////
	GetDlgItem(IDC_HEAD_TYPE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HEAD_TYPE_STATIC)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_HEAD_TYPE, "");

	///// Measurement Program /////
	pCombo = (CComboBox*)GetDlgItem(IDC_MEASUREMENT_PROGRAM);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();

	///// Stage Program /////
	pCombo = (CComboBox*)GetDlgItem(IDC_STAGE_PROGRAM);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();

	///// Point Deskew Program /////
	pCombo = (CComboBox*)GetDlgItem(IDC_POINT_DESKEW_PROGRAM);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();

	///// Recalibration /////
	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();

	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();

	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();

	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();

	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();

	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();
/*
	///// Lens /////
	pCombo = (CComboBox*)GetDlgItem(IDC_LENS);
	pCombo->EnableWindow(FALSE);
	pCombo->ResetContent();
*/

// 2010.01.06 bagus RS --{--
#if 0
// 2009.12.21 bagus 修正 --{--
// デフォルトレシピの設定の場合には変更不可
	m_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate = FALSE;
	GetDlgItem(IDC_MEASUREMENT_RESULT_SAVE_WITH_DATE)->ShowWindow(SW_HIDE);
// 2009.12.21 bagus 修正 --}--
#endif
// 2010.01.06 bagus RS --}--
	///// Comment /////
	GetDlgItem(IDC_COMMENT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
	SetDlgItemText(IDC_COMMENT, "");
}

// =========================================================================
//
BOOL CRecipeMainSrView::CheckData()
{
// Kojika 20090527 Add
	CString strBuffer, strTitle;
// Kojika 20090527 Add End

	UpdateData(TRUE);

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		break;
	default:
		///// Measurement Program /////
		if ( strlen(m_MainRcpInfo.MainRcpParam.hdr.szMeas) == 0 )
		{
// Kojika 20090527 Change
//			MessageBox("Please Select Measurement Program.", m_strCaption, MB_OK | MB_ICONSTOP);
//			MessageBox("測定プログラムを選択してください", m_strCaption, MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_SELECT_MEASERE_PROG, strBuffer, "Please Select Measurement Program.");
			LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
			MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
			GetDlgItem(IDC_MEASUREMENT_PROGRAM)->SetFocus();
			return FALSE;
		}

// 2013.11.07 Bagus Add (TohoSpec対応) -->
		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			///// Stage Program /////
			if ( strlen(m_MainRcpInfo.MainRcpParam.hdr.szStage) == 0 )
			{
// Kojika 20090527 Change
//				MessageBox("Please Select Stage Program.", m_strCaption, MB_OK | MB_ICONSTOP);
//				MessageBox("ステージプログラムを選択してください", m_strCaption, MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_SELECT_STAGE_PROG, strBuffer, "Please Select Stage Program.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
				GetDlgItem(IDC_STAGE_PROGRAM)->SetFocus();
				return FALSE;
			}
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		///// Recalibration /////
		//設定が正常に行われていない場合は、修正が必要な項目へフォーカスを戻す（SetFocus）
		if (m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS){
			if ((m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] !=0) && (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] == m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2])){
// Kojika 20090527 Change
//				MessageBox("Item can be selected only once.\n\rPlease select it again.", m_strCaption, MB_OK | MB_ICONSTOP);
//				MessageBox("アイテムは一つだけしか選択できません\n\r再度選択してください", m_strCaption, MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_ITEM_SELECT_ONCE, strBuffer, "Item can be selected only once.\n\rPlease select it again.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
				GetDlgItem(IDC_RECALIBRATION_ITEM2)->SetFocus();
				return FALSE;
			}
			if ((m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] !=0) && (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] == m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3])){
// Kojika 20090527 Change
//				MessageBox("Item can be selected only once.\n\rPlease select it again.", m_strCaption, MB_OK | MB_ICONSTOP);
//				MessageBox("アイテムは一つだけしか選択できません\n\r再度選択してください", m_strCaption, MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_ITEM_SELECT_ONCE, strBuffer, "Item can be selected only once.\n\rPlease select it again.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
				GetDlgItem(IDC_RECALIBRATION_ITEM3)->SetFocus();
				return FALSE;
			}
			if ((m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] !=0) && (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] == m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3])){
// Kojika 20090527 Change
//				MessageBox("Item can be selected only once.\n\rPlease select it again.", m_strCaption, MB_OK | MB_ICONSTOP);
//				MessageBox("アイテムは一つだけしか選択できません\n\r再度選択してください", m_strCaption, MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_ITEM_SELECT_ONCE, strBuffer, "Item can be selected only once.\n\rPlease select it again.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
				GetDlgItem(IDC_RECALIBRATION_ITEM3)->SetFocus();
				return FALSE;
			}
		}

		CComboBox* pCombo;
		//if (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] == -1) {
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		//if (pCombo->GetCurSel() == 0) {
		if (pCombo->GetCurSel() < 0){
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
			//pCombo->EnableWindow(FALSE);
			pCombo->SetCurSel(0);
			//m_strRecalib1HeadType.Empty();
			m_strRecalib1HeadType = "";
		}
		//if (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] == -1) {
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		//if (pCombo->GetCurSel() == 0) {
		if (pCombo->GetCurSel() < 0){
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
			//pCombo->EnableWindow(FALSE);
			pCombo->SetCurSel(0);
			//m_strRecalib2HeadType.Empty();
			m_strRecalib2HeadType = "";
		}
		//if (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3] == -1) {
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		//if (pCombo->GetCurSel() == 0) {
		if (pCombo->GetCurSel() < 0){
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
			//pCombo->EnableWindow(FALSE);
			pCombo->SetCurSel(0);
			//m_strRecalib3HeadType.Empty();
			m_strRecalib3HeadType = "";
		}

// 2009.12.08 bagus --{--
		RECALIB_PROG_INFO RecalibProgInfo;
		CString strNone;

		LoadStringML(IDS_POINT_NAME_NONE, strNone, "None");

		if (strcmp(strNone, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1]) != 0) {
			if ( !RecipeFile_LoadRecipe(&RecalibProgInfo, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1], RECIPE_FILE_RECALIBRATION_PROGRAM) ) {
				// 存在しないリキャリブレーション設定
				LoadStringML(IDS_RECALIB_NOT_EXIST, strBuffer, "The Recalib is not exist.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
			if ( RecalibProgInfo.wHeadType != m_MainRcpInfo.MainRcpParam.hdr.wHeadType ) {
				// ヘッドタイプが異なる
				LoadStringML(IDS_DIFFERENT_HEADTYPE, strBuffer, "The HeadType is not corresponding.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
		if (strcmp(strNone, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2]) != 0) {
			if ( !RecipeFile_LoadRecipe(&RecalibProgInfo, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], RECIPE_FILE_RECALIBRATION_PROGRAM) ) {
				// 存在しないリキャリブレーション設定
				LoadStringML(IDS_RECALIB_NOT_EXIST, strBuffer, "The Recalib is not exist.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
			if ( RecalibProgInfo.wHeadType != m_MainRcpInfo.MainRcpParam.hdr.wHeadType ) {
				// ヘッドタイプが異なる
				LoadStringML(IDS_DIFFERENT_HEADTYPE, strBuffer, "The HeadType is not corresponding.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
		if (strcmp(strNone, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3]) != 0) {
			if ( !RecipeFile_LoadRecipe(&RecalibProgInfo, m_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], RECIPE_FILE_RECALIBRATION_PROGRAM) ) {
				// 存在しないリキャリブレーション設定
				LoadStringML(IDS_RECALIB_NOT_EXIST, strBuffer, "The Recalib is not exist.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
			if ( RecalibProgInfo.wHeadType != m_MainRcpInfo.MainRcpParam.hdr.wHeadType ) {
				// ヘッドタイプが異なる
				LoadStringML(IDS_DIFFERENT_HEADTYPE, strBuffer, "The HeadType is not corresponding.");
				LoadStringML(IDS_TITLE_MAINRECIPE, strTitle, "MAIN RECIPE");
				MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
// 2009.12.08 bagus --}--
		break;
	}
//Saiki 20110223 Add ----->
	///// Check Thickness for Stress /////
	if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){

		CButton *pCheckActual = (CButton *)GetDlgItem(IDC_RADIO_ACTUAL_VALUE);
		CButton *pCheckFixed = (CButton *)GetDlgItem(IDC_RADIO_FIXED_VALUE);
		if(pCheckActual->GetCheck() == 0 && pCheckFixed->GetCheck() == 1)//Fixed Value Mode
			m_StressParamInfo.bPriority = TRUE;
		else
			m_StressParamInfo.bPriority = FALSE;

		if( CheckStressThicknessData(m_MainRcpInfo) == FALSE){
			return FALSE;
		}
	}
//Saiki 20110223 Add <-----

	UpdateData(FALSE);

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMainSrView::IsDataChanged()
{
	UpdateData(TRUE);
//Saiki 20110304 Change ----->
//	return memcmp(&m_OldMainRcpInfo, &m_MainRcpInfo, sizeof(MAIN_RCP_INFO)) != 0;
	BOOL bRet = FALSE;

	if(memcmp(&m_OldMainRcpInfo, &m_MainRcpInfo, sizeof(MAIN_RCP_INFO)) == 0){
		if(m_iNewHeadType == HEAD_TYPE_STRESS){
			return memcmp(&m_OldStressParamInfo, &m_StressParamInfo, sizeof(STRESS_PARAM_INFO)) != 0;
		}
	}
	else{
		bRet = TRUE;
	}

	return bRet;
//Saiki 20110304 Change <-----
}

// =========================================================================
//
BOOL CRecipeMainSrView::SaveRecipeData()
{
	RECIPENAMEDATESET recipeNameDataSet;

// 2013.11.15 Bagus Add (TohoSpec対応) -->
// 2014.01.07 bagus Add(Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(g_lModelType != MODEL_T3100){
		if (m_SystemConfig.nStageType == STAGE_TYPE_NONE){
			m_MainRcpInfo.MainRcpParam._SR.bMeasRef = FALSE;
		}
	}
	else{
// 2014.01.07 bagus Add(Stage None対応) <--
		m_MainRcpInfo.MainRcpParam._SR.nFocus = SR_FOCUS_NONE;
		m_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_CANCEL_RECIPE;
		m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption = FANAL_FOCUS_FAIL_DATA_VALID_OPTION_INVALID;
		m_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_NONE;
	}
// 2013.11.15 Bagus Add (TohoSpec対応) <--

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
// 2009.09.29 bagus SE --{--
#if 0
		if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_MAIN_RECIPE) )
			return FALSE;
		break;
#else
		switch(m_iNewHeadType){
		case HEAD_TYPE_SR:
			if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_MAIN_RECIPE) )
				return FALSE;
			break;
		case HEAD_TYPE_STRESS:
			//Saiki 20110303 Change ----->
			//if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS_MAIN_RECIPE) ){
			//	  return FALSE;
			//}
			if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS_MAIN_RECIPE) ){
				return FALSE;
			}
			if( !RecipeFile_SetStressParam(&m_StressParamInfo, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS_MAIN_RECIPE) ){
				return FALSE;
			}
			//Saiki 20110303 Change <-----
			break;
		case HEAD_TYPE_SE:
			if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, SE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SE_MAIN_RECIPE) )
				return FALSE;
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
			if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, COMPEASE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE) )
				return FALSE;
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		// 2009.10.08 bagus CTA 追加 --{--
		case HEAD_TYPE_CTA:
			if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, CTA_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_CTA_MAIN_RECIPE) )
				return FALSE;
			break;
		// 2009.10.08 bagus CTA 追加 --}--
		//2009.10.20 bagus MS 追加 --{--
		case HEAD_TYPE_MS:
			if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, MS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_MS_MAIN_RECIPE) )
				return FALSE;
			break;
		//2009.10.20 bagus MS 追加 --}--
		// 2009.11.04 bagus RS 追加 --{--
		case HEAD_TYPE_4PP:
			if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, RES_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_4PP_MAIN_RECIPE) )
				return FALSE;
			break;
		// 2009.11.04 bagus RS 追加 --}--
		default:
			return FALSE;
			break;
		}

		break;
#endif
// 2009.09.29 bagus SE --}--
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_MainRcpInfo.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, m_szRecipeName, RECIPE_FILE_SR_MAIN_RECIPE) )
			return FALSE;
//Saiki 20110304 Add ----->
		if( !RecipeFile_SetStressParam(&m_StressParamInfo, m_szRecipeName, RECIPE_FILE_STRESS_MAIN_RECIPE) ){
			return FALSE;
		}
//Saiki 20110304 Add <-----
		sprintf(recipeNameDataSet.szRecipeNameExt, "%s%s", m_szRecipeName, MAINRECIPE_EXT);
		RecipeFile_ExistRecipe(m_szRecipeName, &recipeNameDataSet.recipeDateTime, RECIPE_FILE_MAIN_RECIPE);
		if ( m_nOpenMode == modeNew )
		{
			PifComm_ChangeRecipeInsertReport(0, &recipeNameDataSet);
		}
		else
		{
			PifComm_ChangeRecipeReplaceReport(0, &recipeNameDataSet);

			//メインレシピ編集と同時にリファレンスデータも削除する(modeNormalのみ)
			char szFilePath[MAX_PATH];
			memset(szFilePath, 0, sizeof(szFilePath));
			sprintf(szFilePath, "%s%s%s", g_szData_Ref_Dir, m_szRecipeName, DAT_EXT);
			DeleteFile(szFilePath);
		}
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldMainRcpInfo, &m_MainRcpInfo, sizeof(MAIN_RCP_INFO));
	if(m_iNewHeadType == HEAD_TYPE_STRESS){
		::CopyMemory(&m_OldStressParamInfo, &m_StressParamInfo, sizeof(STRESS_PARAM_INFO));
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMainSrView::IsAccessPrivilege()
{
	int iAccessItem;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( m_nOpenMode == modeDefaultSetting )
		iAccessItem = ACCESS_USERSETTING;
	else
		iAccessItem = ACCESS_RECIPE;

	return pDoc->User_Access(iAccessItem);
}

// =========================================================================
//
int CRecipeMainSrView::SaveAsDlgInfo()
{
	return SAVE_AS_MAIN_RECIPE;
}

// =========================================================================
//
void CRecipeMainSrView::OnSelchangeRecalibrationItem1()
{
	UpdateData(TRUE);

	CComboBox* pCombo;

	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);

	if (pCombo->GetCurSel() != -1){
	//if (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] == -1) {
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

// =========================================================================
//
void CRecipeMainSrView::OnSelchangeRecalibrationItem2()
{
	UpdateData(TRUE);

	CComboBox* pCombo;

	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);

	if (pCombo->GetCurSel() != -1){
	//if (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] == -1) {
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

// =========================================================================
//
void CRecipeMainSrView::OnSelchangeRecalibrationItem3()
{
	UpdateData(TRUE);

	CComboBox* pCombo;

	pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);

	if (pCombo->GetCurSel() != -1){
	//if (m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3] == -1) {
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

// =========================================================================
//リキャリブ項目表示の設定
void CRecipeMainSrView::InitCombo_RecalibItemSetting(int iRicalibSetIndex1/*=0*/,int iRicalibSetIndex2/*=0*/,int iRicalibSetIndex3/*=0*/)
{
// Kojika 20090527 Add
	CString l_strBuffer, strBuffer;
// Kojika 20090527 Add End

	//UpdateData(TRUE);

	CComboBox* pCombo;
	int iDummyStartWaveLen;
	int iDummyEndWaveLen;
	int iDummyOpticalFilter;


// 2009.10.22 bagus Stress 修正 --{--
#if 0
	// コンボボックスの編集
	switch(m_MeasProgInfo.ScanParams.hdr.wScanType){
	case MEAS_PROG_TYPE_SR_THICKNESS:
		TCHAR szDdeTextItem[1024];
		char *token;

	//1st Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		// ストラテジー毎の測定項目インデックスを追加
//		pCombo->AddString("None");
		//Saiki 20090527 Change ----->
		//pCombo->AddString("無し");
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
		//Saiki 20090527 Change <-----
		if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry) ){
			token = strtok(szDdeTextItem, _T(","));
			while ( token != NULL  ) {
				pCombo->AddString(token);
				token = strtok(NULL, _T(","));
			}
		}
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(iRicalibSetIndex1);
		//m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1]=0;

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		//pCombo->EnableWindow(FALSE);
		pCombo->SetCurSel(0);

	//2nd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		// ストラテジー毎の測定項目インデックスを追加
//		pCombo->AddString("None");
		//Saiki 20090527 Change ----->
		//pCombo->AddString("無し");
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
		//Saiki 20090527 Change <-----
		if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry) ){
			token = strtok(szDdeTextItem, _T(","));
			while ( token != NULL  ) {
				pCombo->AddString(token);
				token = strtok(NULL, _T(","));
			}
		}
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(iRicalibSetIndex2);
		//m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2]=0;

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		//pCombo->EnableWindow(FALSE);
		pCombo->SetCurSel(0);

	//3rd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		// ストラテジー毎の測定項目インデックスを追加
//		pCombo->AddString("None");
		//Saiki 20090527 Change ----->
		//pCombo->AddString("無し");
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
		//Saiki 20090527 Change <-----
		if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry) ){
			token = strtok(szDdeTextItem, _T(","));
			while ( token != NULL  ) {
				pCombo->AddString(token);
				token = strtok(NULL, _T(","));
			}
		}
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(iRicalibSetIndex3);
		//m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]=0;

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		//pCombo->EnableWindow(FALSE);
		pCombo->SetCurSel(0);

		break;

	case MEAS_PROG_TYPE_SR_REFLECTANCE:
	case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.09.03 K.Matsuo -->
	case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2009.09.03 K.Matsuo <--
	case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		//pCombo->EnableWindow(FALSE);
// Kojika 20090527 Change
//		pCombo->AddString("1st Wave");
//		pCombo->AddString("第１波長");
		LoadStringML(IDS_INDEX_ONE_WAVE, l_strBuffer, "1st Wave");
		pCombo->AddString(l_strBuffer);
// Kojika 20090527 Change End
		//m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1]=0;
		pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		//pCombo->EnableWindow(FALSE);
// Kojika 20090527 Change
//		pCombo->AddString("2nd Wave");
//		pCombo->AddString("第２波長");
		LoadStringML(IDS_INDEX_TWO_WAVE, l_strBuffer, "2nd Wave");
		pCombo->AddString(l_strBuffer);
// Kojika 20090527 Change End
		//m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2]=0;
		pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		//pCombo->EnableWindow(FALSE);
// Kojika 20090527 Change
//		pCombo->AddString("3rd Wave");
//		pCombo->AddString("第３波長");
		LoadStringML(IDS_INDEX_THR_WAVE, l_strBuffer, "3rd Wave");
		pCombo->AddString(l_strBuffer);
// Kojika 20090527 Change End
		//m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]=0;
		pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		break;

	case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
	case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		//pCombo->EnableWindow(FALSE);
		pCombo->AddString("CIE X");
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1]=0;
		//pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		//pCombo->EnableWindow(FALSE);
		pCombo->AddString("CIE Y");
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2]=0;
		//pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		//pCombo->EnableWindow(FALSE);
		pCombo->AddString("CIE Z");
		m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]=0;
		//pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		break;
	}
#else
	TCHAR szDdeTextItem[1024];
	char *token;

	switch (m_MeasProgInfo.ScanParams.hdr.wHeadType) {
	case HEAD_TYPE_SR:
		switch (m_MeasProgInfo.ScanParams.hdr.wScanType) {
		case MEAS_PROG_TYPE_SR_THICKNESS:
		//1st Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
			pCombo->ResetContent();
			// ストラテジー毎の測定項目インデックスを追加
			LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
			pCombo->AddString(strBuffer);
			if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry) ){
				token = strtok(szDdeTextItem, _T(","));
				while ( token != NULL  ) {
					//2009.12.07 bagus SR MSE -> FIT --{--
					//pCombo->AddString(token);
					if(strcmp(token, "MSE") == 0){
						pCombo->AddString("Fit");
					}else{
						pCombo->AddString(token);
					}
					//2009.12.07 bagus SR MSE -> FIT --{--
					token = strtok(NULL, _T(","));
				}
			}
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(iRicalibSetIndex1);
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
			pCombo->SetCurSel(0);

		//2nd Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
			pCombo->ResetContent();
			// ストラテジー毎の測定項目インデックスを追加
			LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
			pCombo->AddString(strBuffer);
			if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry) ){
				token = strtok(szDdeTextItem, _T(","));
				while ( token != NULL  ) {
					//2009.12.07 bagus SR MSE -> FIT --{--
					//pCombo->AddString(token);
					if(strcmp(token, "MSE") == 0){
						pCombo->AddString("Fit");
					}else{
						pCombo->AddString(token);
					}
					//2009.12.07 bagus SR MSE -> FIT --{--
					token = strtok(NULL, _T(","));
				}
			}
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(iRicalibSetIndex2);
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
			pCombo->SetCurSel(0);

		//3rd Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
			pCombo->ResetContent();
			// ストラテジー毎の測定項目インデックスを追加
			LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
			pCombo->AddString(strBuffer);
			if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry) ){
				token = strtok(szDdeTextItem, _T(","));
				while ( token != NULL  ) {
					//2009.12.07 bagus SR MSE -> FIT --{--
					//pCombo->AddString(token);
					if(strcmp(token, "MSE") == 0){
						pCombo->AddString("Fit");
					}else{
						pCombo->AddString(token);
					}
					//2009.12.07 bagus SR MSE -> FIT --{--
					token = strtok(NULL, _T(","));
				}
			}
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(iRicalibSetIndex3);
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
			pCombo->SetCurSel(0);

			break;
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
		case MEAS_PROG_TYPE_SR_DISTANCE:
		//1st Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
			pCombo->ResetContent();
			LoadStringML(IDS_INDEX_ONE_WAVE, l_strBuffer, "1st Wave");
			pCombo->AddString(l_strBuffer);
			pCombo->SetCurSel(0);
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(0);

		//2nd Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
			pCombo->ResetContent();
			LoadStringML(IDS_INDEX_TWO_WAVE, l_strBuffer, "2nd Wave");
			pCombo->AddString(l_strBuffer);
			pCombo->SetCurSel(0);
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(0);

		//3rd Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
			pCombo->ResetContent();
			LoadStringML(IDS_INDEX_THR_WAVE, l_strBuffer, "3rd Wave");
			pCombo->AddString(l_strBuffer);
			pCombo->SetCurSel(0);
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(0);

			break;
		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
		//1st Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
			pCombo->ResetContent();
			pCombo->AddString("CIE X");
			m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1]=0;
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(0);

		//2nd Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
			pCombo->ResetContent();
			pCombo->AddString("CIE Y");
			m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2]=0;
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(0);

		//3rd Recalib Combo
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
			pCombo->ResetContent();
			pCombo->AddString("CIE Z");
			m_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]=0;
			pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
			pCombo->EnableWindow(TRUE);
			pCombo->SetCurSel(0);

			break;
		}
		break;
	case HEAD_TYPE_SE:
// 2009.12.08 bagus SE --{--
		//1st Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		// ストラテジー毎の測定項目インデックスを追加
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
		if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SE.szStrategyEntry) ){
			token = strtok(szDdeTextItem, _T(","));
			while ( token != NULL  ) {
				//2009.12.07 bagus SR MSE -> FIT --{--
				//pCombo->AddString(token);
				if(strcmp(token, "MSE") == 0){
					pCombo->AddString("Fit");
				}else{
					pCombo->AddString(token);
				}
				//2009.12.07 bagus SR MSE -> FIT --{--
				token = strtok(NULL, _T(","));
			}
		}
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(iRicalibSetIndex1);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->SetCurSel(0);

		//2nd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		// ストラテジー毎の測定項目インデックスを追加
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
		if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SE.szStrategyEntry) ){
			token = strtok(szDdeTextItem, _T(","));
			while ( token != NULL  ) {
				//2009.12.07 bagus SR MSE -> FIT --{--
				//pCombo->AddString(token);
				if(strcmp(token, "MSE") == 0){
					pCombo->AddString("Fit");
				}else{
					pCombo->AddString(token);
				}
				//2009.12.07 bagus SR MSE -> FIT --{--
				token = strtok(NULL, _T(","));
			}
		}
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(iRicalibSetIndex2);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->SetCurSel(0);

		//3rd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		// ストラテジー毎の測定項目インデックスを追加
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
		if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, m_MeasProgInfo.ScanParams._SE.szStrategyEntry) ){
			token = strtok(szDdeTextItem, _T(","));
			while ( token != NULL  ) {
				//2009.12.07 bagus SR MSE -> FIT --{--
				//pCombo->AddString(token);
				if(strcmp(token, "MSE") == 0){
					pCombo->AddString("Fit");
				}else{
					pCombo->AddString(token);
				}
				//2009.12.07 bagus SR MSE -> FIT --{--
				token = strtok(NULL, _T(","));
			}
		}
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(iRicalibSetIndex3);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->SetCurSel(0);

		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		{
			int nItemIDs[] = {IDC_RECALIBRATION_ITEM1, IDC_RECALIBRATION_ITEM2, IDC_RECALIBRATION_ITEM3};
			int iCurSels[] = {iRicalibSetIndex1, iRicalibSetIndex2, iRicalibSetIndex3};
			int nRecalibRecipeIDs[] = {IDC_RECALIBRATION_1, IDC_RECALIBRATION_2, IDC_RECALIBRATION_3};
			int nRecalibItemCount;

			nRecalibItemCount = sizeof(g_lpszCompEASERecalibItem_ENU) / sizeof(g_lpszCompEASERecalibItem_ENU[0]);

			// Recalib Combo
			for( int i = 0; i < 3; i++) {
				pCombo = (CComboBox*)GetDlgItem(nItemIDs[i]);
				pCombo->ResetContent();

				for( int iItem = 0; iItem < nRecalibItemCount; iItem++) {
					if (m_SystemConfig.nLanguage == 0) {
						pCombo->AddString(g_lpszCompEASERecalibItem_ENU[iItem]);
					}
					else {
						pCombo->AddString(g_lpszCompEASERecalibItem_JPN[iItem]);
					}
				}

				pCombo->EnableWindow(TRUE);
				pCombo->SetCurSel(iCurSels[i]);

				pCombo = (CComboBox*)GetDlgItem(nRecalibRecipeIDs[i]);
				pCombo->SetCurSel(0);
			}
		}
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.12.08 bagus SE --}--
	case HEAD_TYPE_IRSE:
// 2009.11.04 bagus RS 追加 --{--
//	case HEAD_TYPE_4PP:
// 2009.11.04 bagus RS 追加 --}--
// 2009.10.28 bagus CTA 追加修正 --{--
//	case HEAD_TYPE_CTA:
// 2009.10.28 bagus CTA 追加修正 --}--
	//1st Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_ONE_WAVE, l_strBuffer, "1st Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//2nd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_TWO_WAVE, l_strBuffer, "2nd Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//3rd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_THR_WAVE, l_strBuffer, "3rd Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		break;
	case HEAD_TYPE_STRESS:
	//1st Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
// 2009.12.08 bagus --{--
#if 0
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
#endif
// 2009.12.08 bagus --}--
		LoadStringML(IDS_POINT_NAME_STRESS, strBuffer, "STRESS");
		pCombo->AddString(strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//2nd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
// 2009.12.08 bagus --{--
#if 0
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
#endif
// 2009.12.08 bagus --}--
		LoadStringML(IDS_POINT_NAME_STRESS, strBuffer, "STRESS");
		pCombo->AddString(strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//3rd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
// 2009.12.08 bagus --{--
#if 0
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");
		pCombo->AddString(strBuffer);
#endif
// 2009.12.08 bagus --}--
		LoadStringML(IDS_POINT_NAME_STRESS, strBuffer, "STRESS");
		pCombo->AddString(strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		break;
	case HEAD_TYPE_MS:
	//1st Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_ONE_WAVE, l_strBuffer, "1st Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//2nd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_TWO_WAVE, l_strBuffer, "2nd Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//3rd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_THR_WAVE, l_strBuffer, "3rd Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		break;
// 2009.10.28 bagus CTA 追加修正 --{--
	case HEAD_TYPE_CTA:
	//1st Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		LoadStringML(IDS_POINT_NAME_CA, l_strBuffer, "CA");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//2nd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		LoadStringML(IDS_POINT_NAME_RAD, l_strBuffer, "Rad");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//3rd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		LoadStringML(IDS_POINT_NAME_NONE, l_strBuffer, "None");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		break;
// 2009.10.28 bagus CTA 追加修正 --}--
// 2009.11.04 bagus RS 追加 --{--
	case HEAD_TYPE_4PP:
	//1st Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_ONE_WAVE, l_strBuffer, "1st Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//2nd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_TWO_WAVE, l_strBuffer, "2nd Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//3rd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_THR_WAVE, l_strBuffer, "3rd Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		break;
// 2009.11.04 bagus RS 追加 --}--
	default:
	//1st Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_ONE_WAVE, l_strBuffer, "1st Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//2nd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM2);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_TWO_WAVE, l_strBuffer, "2nd Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_2);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

	//3rd Recalib Combo
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM3);
		pCombo->ResetContent();
		LoadStringML(IDS_INDEX_THR_WAVE, l_strBuffer, "3rd Wave");
		pCombo->AddString(l_strBuffer);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_3);
		pCombo->EnableWindow(TRUE);
		pCombo->SetCurSel(0);

		break;
	}
#endif
// 2009.10.22 bagus Stress 修正 --}--

	UpdateData(FALSE);
	//UpdateData(TRUE);

}

// =========================================================================
// フォーカス失敗時動作 コンボ選択時
//
void CRecipeMainSrView::OnSelchangeFinalFocus()
{
//	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_FINAL_FOCUS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption = nSelect;

//	UpdateData(FALSE);
}

//2009.09.02 bagus se 機能拡張 --{--
void CRecipeMainSrView::OnPaint()
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	//2009.12.21 bagus 修正 --{--
	long lDrawStageSizeX, lDrawStageSizeY;
	CRect StageRect;
//	CRect SampleRect;

	///// Region Map /////
	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
	CDC* pDC = pPictBox->GetDC();
	pPictBox->GetClientRect(&StageRect);

	//2009.12.21 bagus 修正 --{--
	//ステージ部は表示しない。サンプルのみ表示する
	///// Draw Stage Color /////
	CBrush BrushStage(BLACK_COLOR);
	m_oldBrush = pDC->SelectObject(&BrushStage);
	CPen PenStage(PS_SOLID, 1, BLACK_COLOR);
	m_oldPen = pDC->SelectObject(&PenStage);

	///// Stage Rect Offset /////
	long StageRectLeft = StageRect.left + STAGE_OFFSET;
	long StageRectRight = StageRect.right - STAGE_OFFSET;
	long StageRectTop = StageRect.top + STAGE_OFFSET;
	long StageRectBottom = StageRect.bottom - STAGE_OFFSET;

	///// ステージ有効範囲 /////
// 	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
// 	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
	double dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);

	if((dStageMaxSizeX / dStageMaxSizeY) > ((StageRectRight - StageRectLeft) / (StageRectBottom - StageRectTop))) {
		lDrawStageSizeX =(StageRectRight - StageRectLeft);
		lDrawStageSizeY = static_cast<long>((StageRectRight - StageRectLeft) * dStageMaxSizeY / dStageMaxSizeX);

		m_dDrawStartStagePosX = StageRectLeft;
		m_dDrawStartStagePosY = StageRectTop + ((StageRectBottom - StageRectTop - lDrawStageSizeY) / 2);
		m_dDrawEndStagePosX = StageRectRight;
		m_dDrawEndStagePosY = m_dDrawStartStagePosY + lDrawStageSizeY;
	}
	else{
		lDrawStageSizeX = static_cast<long>((StageRectBottom - StageRectTop) * dStageMaxSizeX / dStageMaxSizeY);
		lDrawStageSizeY = (StageRectBottom - StageRectTop);

		m_dDrawStartStagePosX = StageRectLeft + ((StageRectRight - StageRectLeft - lDrawStageSizeX) / 2);
		m_dDrawStartStagePosY = StageRectTop;
		m_dDrawEndStagePosX = m_dDrawStartStagePosX + lDrawStageSizeX;
		m_dDrawEndStagePosY = StageRectBottom;
	}

//	pDC->Rectangle(CRect(m_dDrawStartStagePosX, m_dDrawStartStagePosY, m_dDrawEndStagePosX, m_dDrawEndStagePosY));
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);
	//2009.12.21 bagus 修正 --}--

	///// Draw Sample /////
	DrawSample(pDC, m_StageConfig.OrgPos, m_StageConfig.Dir.X, m_StageConfig.Dir.Y);

// 2013.01.09 bagus -->
// 	///// Draw Origin Point /////
// 	DrawOriginMaker(m_StageConfig.OrgPos);
// 2013.01.09 bagus <--

// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		SamplePointGraph_DataSet();
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
	ReleaseDC(pDC);
}

// 2013.01.09 bagus -->
// 軸座標（STAGEAPI用）に変換
void CRecipeMainSrView::ToXyAxisCoord(STAGE_COORD* lpPoint)
{
	// X: RIGHT
	if ( m_StageConfig.Dir.X == DIR_RIGHT ) {
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: LEFT
	if ( m_StageConfig.Dir.X == DIR_LEFT ) {
		lpPoint->lX *= -1;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: TOP
	if ( m_StageConfig.Dir.X == DIR_TOP ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lY = ptTemp.lX;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_RIGHT:
			lpPoint->lX = ptTemp.lY;
			break;
		case DIR_LEFT:
			lpPoint->lX = ptTemp.lY * -1;
			break;
		default:
			break;
		}
	}

	// X: BOTTOM
	if ( m_StageConfig.Dir.X == DIR_BOTTOM ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lY = ptTemp.lX * -1;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_RIGHT:
			lpPoint->lX = ptTemp.lY;
			break;
		case DIR_LEFT:
			lpPoint->lX = ptTemp.lY * -1;
			break;
		default:
			break;
		}
	}
}
// 2013.01.09 bagus <--

void CRecipeMainSrView::DrawSample(CDC* pDC, int iOrg, int DirX, int DirY)
{
	WINDOWPLACEMENT Stageplacement;

	///// Region Map /////
	GetDlgItem(IDC_STAGE_MAP)->GetWindowPlacement(&Stageplacement);

	///// ステージ有効範囲(黒い四角はこのサイズで描画されている) /////
// 2009.12.14 bagus 修正 --{--
//	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
//	double dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
// 2013.01.17 bagus rotate xy view -->
//	double dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX);
//	double dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY);

	double dStageMaxSizeX;
	double dStageMaxSizeY;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
		dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
		break;
	case 90:
	case 270:
		dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
		dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
		break;
	}
// 2013.01.17 bagus rotate xy view <--
// 2009.12.14 bagus 修正 --}--

// 2013.01.09 bagus -->
	double dSampleSizeX;
	double dSampleSizeY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	}
	else {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
	}
// 2013.01.17 bagus rotate xy view <--
	double dTemp;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		break;
	case 90:
	case 270:
		dTemp = dSampleSizeX;
		dSampleSizeX = dSampleSizeY;
		dSampleSizeY = dTemp;
		break;
	}
// 2013.01.17 bagus rotate xy view <--

	///// 　サンプルサイズがステージの大きさとり小さいかチェック /////
// 	if ((m_StageProgInfoHdr.SampleInfo.Size.dx > dStageMaxSizeX) || (m_StageProgInfoHdr.SampleInfo.Size.dy > dStageMaxSizeY))
// 		return;
	if ((dSampleSizeX > dStageMaxSizeX) || (dSampleSizeY > dStageMaxSizeY))
		return;

	///// サンプル描画サイズの計算（ステージ描画サイズ対するサンプル描画サイズを計算） /////
// 	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX) * m_StageProgInfoHdr.SampleInfo.Size.dx / dStageMaxSizeX;
// 	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY) * m_StageProgInfoHdr.SampleInfo.Size.dy / dStageMaxSizeY;
// 	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX) * dSampleSizeX / dStageMaxSizeX;
// 	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY) * dSampleSizeY / dStageMaxSizeY;
	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX); // ステージの黒い部分は比率計算に加味しない
	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY); // ステージの黒い部分は比率計算に加味しない
// 2013.01.09 bagus <--

	///// ステージとサンプルの原点オフセットの合計値 /////
	double dTotalOrgOffsetPosX, dTotalOrgOffsetPosY;

	///// サンプルの描画位置 /////
	double dtop, dbottom, dright, dleft;

// 2009.12.02 K.Matsuo SampleOrigin -->
	SAMPLEINFO sampleInfo;
	::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
	RecipeFile_LoadRecipe(&sampleInfo, m_StageProgInfoHdr.SampleInfo.szName, RECIPE_FILE_SAMPLE);
// 2009.12.02 K.Matsuo SampleOrigin <--

// 2013.01.09 bagus -->
	STAGE_COORD XyAxisCoord;

	XyAxisCoord.lX = m_StageProgInfoHdr.SampleInfo.Origin.lX;
	XyAxisCoord.lY = m_StageProgInfoHdr.SampleInfo.Origin.lY;
	ToXyAxisCoord(&XyAxisCoord);
// 2013.01.09 bagus <--

	//Sample描画位置設定
	//四角形状サンプルのみ対応
	switch(m_StageProgInfoHdr.SampleInfo.nShape){
	case SAMPLE_SHAPE_RECTANGLE:		//Sample shaped RectAngle
		switch(iOrg){
		case 1: ///// センター基準	//X軸：→+　Y軸：↑+のみ対応
		default:
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2013.01.09 bagus -->
// 2009.12.02 K.Matsuo SampleOrigin -->
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.14 bagus 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (sampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (sampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX) - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY) - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.14 bagus 修正 --}--
// 2009.12.02 K.Matsuo SampleOrigin <--
// 2013.01.17 bagus rotate xy view <--
// 			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX) + (XyAxisCoord.lX) - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY) + (XyAxisCoord.lY) - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
				break;
			case 90:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
				break;
			case 180:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
				break;
			case 270:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
				break;
			}
// 2013.01.09 bagus -->

			///// サンプル描画位置を計算 /////
			//左基準
			dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX / 2);
			dright	= dleft + dDrawSampleSizeX;
			//下基準
			dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + (dDrawSampleSizeY / 2);
			dtop	= dbottom - dDrawSampleSizeY;
			break;

		case 2: /////LeftTop
// 2013.01.09 bagus -->
// 2009.09.07 bagus stress 原点位置対応 --{--
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2009.09.11 bagus stress 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.02 K.Matsuo SampleOrigin -->
//			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = (sampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = (sampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.02 K.Matsuo SampleOrigin <--
// 2009.09.11 bagus stress 修正 --}--
			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus -->

			///// サンプル描画位置を計算 /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom = dtop + dDrawSampleSizeY;
			break;
// 2009.09.07 bagus stress 原点位置対応 --}--
		case 3: /////LeftBottom
// 2013.01.09 bagus -->
// 2009.09.07 bagus stress 原点位置対応 --{--
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2009.09.11 bagus stress 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.02 K.Matsuo SampleOrigin -->
//			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.14 bagus 修正 --{--
//			dTotalOrgOffsetPosX = (sampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (sampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = ((m_StageProgInfoHdr.SampleInfo.Origin.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX);
// 			dTotalOrgOffsetPosY = ((m_StageProgInfoHdr.SampleInfo.Origin.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY);
// 2009.12.14 bagus 修正 --}--
// 2009.12.02 K.Matsuo SampleOrigin <--
// 2009.09.11 bagus stress 修正 --}--
			dTotalOrgOffsetPosX = ((XyAxisCoord.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX);
			dTotalOrgOffsetPosY = ((XyAxisCoord.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY);
// 2013.01.09 bagus -->

			///// サンプル描画位置を計算 /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
			break;
// 2009.09.07 bagus stress 原点位置対応 --}--
		case 4: /////RightBottom
// 2013.01.09 bagus -->
// 2009.09.07 bagus stress 原点位置対応 --{--
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2009.09.11 bagus stress 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.02 K.Matsuo SampleOrigin -->
//			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = (sampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = (sampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.02 K.Matsuo SampleOrigin <--
// 2009.09.11 bagus stress 修正 --}--
			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus -->

			///// サンプル描画位置を計算 /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
			break;
// 2009.09.07 bagus stress 原点位置対応 --}--
		case 5: /////RightTop
// 2013.01.09 bagus -->
// 2009.09.07 bagus stress 原点位置対応 --{--
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2009.09.11 bagus stress 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.02 K.Matsuo SampleOrigin -->
//			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = (sampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = (sampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.02 K.Matsuo SampleOrigin <--
// 2009.09.11 bagus stress 修正 --}--
			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus -->

			///// サンプル描画位置を計算 /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom = dtop + dDrawSampleSizeY;
// 2009.09.07 bagus stress 原点位置対応 --}--
			break;
		}

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
// 2013.01.09 bagus -->
			////DLG上の外形コントロールサイズ(Sample Size)
			WINDOWPLACEMENT placement;
			m_SamplePointGraph.GetWindowPlacement(&placement);
			placement = Stageplacement;
			placement.rcNormalPosition.bottom -= 30;
			placement.rcNormalPosition.top += 30;
			placement.rcNormalPosition.left += 8;
			placement.rcNormalPosition.right -= 8;

			//グラフコントロールグラフ領域サイズ
			double dGraphSize_x = (placement.rcNormalPosition.right - placement.rcNormalPosition.left);
			double dGraphSize_y = (placement.rcNormalPosition.bottom - placement.rcNormalPosition.top);
			if ((dGraphSize_y / dGraphSize_x) < (dSampleSizeY / dSampleSizeX)){
				//Xサイズ設定
				double dDeltaGraphSize_x = dSampleSizeX * (dGraphSize_y / dSampleSizeY);
				//スペースのセンターあわせ用座標計算
				double dGraphCenterOffset_x = (dGraphSize_x - dDeltaGraphSize_x)/2;
				placement.rcNormalPosition.left = placement.rcNormalPosition.left + static_cast<LONG>(dGraphCenterOffset_x);
				placement.rcNormalPosition.right = placement.rcNormalPosition.left + static_cast<LONG>(dDeltaGraphSize_x);
			}
			else{
				//Yサイズ設定
				double dDeltaGraphSize_y = dSampleSizeY * (dGraphSize_x / dSampleSizeX);
				placement.rcNormalPosition.bottom = placement.rcNormalPosition.top + static_cast<LONG>(dDeltaGraphSize_y);
			}

			//Y方向もセンターに合わせる
			long sh = Stageplacement.rcNormalPosition.bottom - Stageplacement.rcNormalPosition.top;
			long ph = placement.rcNormalPosition.bottom - placement.rcNormalPosition.top;
			if(sh > ph){
				placement.rcNormalPosition.top = Stageplacement.rcNormalPosition.top + (sh - ph) / 2;
				placement.rcNormalPosition.bottom = Stageplacement.rcNormalPosition.bottom - (sh - ph) / 2;
			}

			//位置指定
			m_SamplePointGraph.SetWindowPlacement(&placement);
			m_SamplePointGraph.ShowWindow(SW_SHOW);
// 2013.01.09 bagus <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	}
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);
}

// 2013.01.09 bagus -->
// void CRecipeMainSrView::DrawOriginMaker(int Origin)
// {
// 	long OriginX, OriginY;
//
// 	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
// 	CDC* pDC = pPictBox->GetDC();
// 	CRect StageRect;
// 	pPictBox->GetClientRect(&StageRect);
//
// 	CBrush BrushRed(GREEN_COLOR);
// 	CPen PenRed(PS_SOLID, 1, GREEN_COLOR);
// 	pDC->SelectObject(&BrushRed);
// 	pDC->SelectObject(&PenRed);
//
// 	///// ステージ有効範囲 /////
// 	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
// 	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
//
// 	///// Draw point /////
// 	switch(Origin){
// 	case 1:
// 	default:
// 		///// Center Position /////
// 		OriginX = m_dDrawStartStagePosX + (m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 		OriginY = m_dDrawStartStagePosY + (m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 		break;
//
// 	case 2: /////LeftTop
// 	case 3: /////LeftBottom
// 	case 4: /////RightBottom
// 	case 5: /////RightTop
// 		break;
// 	}
// 	//Draw Point
// 	pDC->Ellipse((OriginX - ELLIPSE_RADIUS), (OriginY - ELLIPSE_RADIUS), (OriginX + ELLIPSE_RADIUS), (OriginY + ELLIPSE_RADIUS));
// 	pDC->SelectObject(m_oldPen);
// 	pDC->SelectObject(m_oldBrush);
//
// 	ReleaseDC(pDC);
// }
// 2013.01.09 bagus <--

void CRecipeMainSrView::SamplePointGraph_Init()
{
	CString strBuffer;
	CNanoPointGraph::GraphConfig tConfig;

	m_SamplePointGraph.ShowWindow(SW_HIDE);

	////Point Graph Data Reset
	m_SamplePointGraph.Reset();
	m_SamplePointGraph.ResetSection();

	////Back Color
	tConfig.BackColor = WAFER_COLOR;
	////Edge Color
	tConfig.EdgeColor[CNanoPointGraph::EDGE_LOWER] = WHITE_COLOR;
	tConfig.EdgeColor[CNanoPointGraph::EDGE_UPPER] = RGB(0x00,0x00,0x00);
	////Status Color
	tConfig.StatusColor[CNanoPointGraph::NOT_MEASURED] = RGB(0x00,0x00,0xFF);
	tConfig.StatusColor[CNanoPointGraph::MEASURING] = RGB(0x00,0xFF,0x55);
	tConfig.StatusColor[CNanoPointGraph::SELECTED] = RGB(0xFF,0x00,0x00);
	tConfig.StatusColor[CNanoPointGraph::MEASURED] = RGB(0x77,0x77,0x77);
	////Section Color
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_NOT_MEASURED] = RGB(0xFF,0xD7,0x00);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_MEASURING] = RGB(0x00,0xFF,0x55);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_SELECTED] = RGB(0xFF,0x00,0x00);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_MEASURED] = RGB(0x77,0x77,0x77);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_NOT_USED] = WAFER_COLOR;

	// 2013.01.09 bagus -->
	tConfig.OriginColor = GREEN_COLOR;
	// 2013.01.09 bagus -->

	////CNanoPointGraphクラス内サイズ(Sample Size)
// 2009.09.07 bagus stress 原点位置対応 --{--
//	tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
//	tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
// 		// X<->Y入れ替え
// 		tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
// 		tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
// 		// X-Yそのまま
// 		tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
// 		tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 	} else {
// 		// 矢印設定がおかしい
// 	}

// 2013.01.17 bagus rotate xy view -->
	double dSampleSizeX;
	double dSampleSizeY;

	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
		// X<->Y入れ替え
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
		// X-Yそのまま
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	} else {
		// 矢印設定がおかしい
	}

	double dTemp;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		break;
	case 90:
	case 270:
		dTemp = dSampleSizeX;
		dSampleSizeX = dSampleSizeY;
		dSampleSizeY = dTemp;
		break;
	}

	tConfig.SampleSizeX = dSampleSizeX;
	tConfig.SampleSizeY = dSampleSizeY;
// 2013.01.17 bagus rotate xy view <--

// 2009.09.07 bagus stress 原点位置対応 --}--
	m_SamplePointGraph.SetConfig(&tConfig);

	SamplePointGraph_DataSet();
}

// 2013.01.17 bagus rotate xy view -->
// ==========================================================================
//
long CRecipeMainSrView::RotateXyAxisDir(long lDir)
{
	long lRet;

	lRet = lDir;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
		break;
	case 90:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_TOP:
			lRet = DIR_RIGHT;
			break;
		case DIR_LEFT:
			lRet = DIR_TOP;
			break;
		case DIR_BOTTOM:
			lRet = DIR_LEFT;
			break;
		default:
			break;
		}
		break;
	case 180:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_LEFT;
			break;
		case DIR_TOP:
			lRet = DIR_BOTTOM;
			break;
		case DIR_LEFT:
			lRet = DIR_RIGHT;
			break;
		case DIR_BOTTOM:
			lRet = DIR_TOP;
			break;
		default:
			break;
		}
		break;
	case 270:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_TOP;
			break;
		case DIR_TOP:
			lRet = DIR_LEFT;
			break;
		case DIR_LEFT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_BOTTOM:
			lRet = DIR_RIGHT;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return lRet;
}
// 2013.01.17 bagus rotate xy view <--

void CRecipeMainSrView::SamplePointGraph_DataSet()
{
	////Point Graph Data Reset
	m_SamplePointGraph.Reset();
	m_SamplePointGraph.ResetSection();

	CNanoPointGraph::LSData tLineData;

	int i, j;

	////Point Data
	//センター原点の場合
// 2009.09.07 bagus stress 原点位置対応 --{--
//	double dOffSetValueX = (m_StageProgInfoHdr.SampleInfo.Size.dx/2);
//	double dOffSetValueY = (m_StageProgInfoHdr.SampleInfo.Size.dy/2);
	double dOffSetValueX;
	double dOffSetValueY;
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
		dOffSetValueX = (m_StageProgInfoHdr.SampleInfo.Size.dx/2);
		dOffSetValueY = (m_StageProgInfoHdr.SampleInfo.Size.dy/2);
// 2013.01.17 bagus rotate xy view -->
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			break;
		case 90:
			break;
		case 180:
			break;
		case 270:
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 2:
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = 0;
// // 2009.09.08 bagus stress 原点位置対応 --{--
// //		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 		dOffSetValueY = 0;
// // 2009.09.08 bagus stress 原点位置対応 --{--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			break;
		case 90:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 180:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 270:
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 3:
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = 0;
// // 2009.09.08 bagus stress 原点位置対応 --{--
// //		dOffSetValueY = 0;
// 		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// // 2009.09.08 bagus stress 原点位置対応 --{--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 90:
			break;
		case 180:
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 270:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 4:
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = m_StageProgInfoHdr.SampleInfo.Size.dx;
// // 2009.09.08 bagus stress 原点位置対応 --{--
// //		dOffSetValueY = 0;
// 		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// // 2009.09.08 bagus stress 原点位置対応 --{--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 90:
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 180:
			break;
		case 270:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 5:
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = m_StageProgInfoHdr.SampleInfo.Size.dx;
// // 2009.09.08 bagus stress 原点位置対応 --{--
// //		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 		dOffSetValueY = 0;
// // 2009.09.08 bagus stress 原点位置対応 --{--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
 			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 90:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 180:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 270:
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	}
// 2009.09.07 bagus stress 原点位置対応 --}--

// 2013.01.09 bagus -->
	double dOriginPointDataX;
	double dOriginPointDataY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dOriginPointDataX = dOffSetValueX;
		dOriginPointDataY = dOffSetValueY;
	}
	else {
		dOriginPointDataX = dOffSetValueY;
		dOriginPointDataY = dOffSetValueX;
	}
// 2013.01.17 bagus rotate xy view <--
//	m_SamplePointGraph.SetOriginPointData(dOffSetValueX, dOffSetValueY);
	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOriginPointDataX), static_cast<long>(dOriginPointDataY));
		break;
	case 90:
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOriginPointDataY), static_cast<long>(dOriginPointDataX));
		dOffSetValueX = dOriginPointDataY;
		dOffSetValueY = dOriginPointDataX;
		break;
	case 180:
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOriginPointDataX), static_cast<long>(dOriginPointDataY));
		break;
	case 270:
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOriginPointDataY), static_cast<long>(dOriginPointDataX));
		dOffSetValueX = dOriginPointDataY;
		dOffSetValueY = dOriginPointDataX;
		break;
	}
// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus <--

	double Scan_PointIncOffsetX;
	double Scan_PointIncOffsetY;

// 2013.01.17 bagus rotate xy view -->
	long lDirX = RotateXyAxisDir(m_StageConfig.Dir.X);
	long lDirY = RotateXyAxisDir(m_StageConfig.Dir.Y);
// 2013.01.17 bagus rotate xy view <--

// 2009.10.28 bagus 2点間 --{--
#if 0
	for(int iPoint= 0; iPoint<m_StageProgInfoHdr.wNumScans; iPoint++){
// 2009.09.07 bagus stress 原点位置対応 --{--
//		Scan_PointIncOffsetX = m_ScanPoint[iPoint].lX + dOffSetValueX;
//		Scan_PointIncOffsetY = -(m_ScanPoint[iPoint].lY - dOffSetValueY);
		switch (lDirX) {
		case 1: 	// X:→
		default:
			Scan_PointIncOffsetX = dOffSetValueX + m_ScanPoint[iPoint].lX;
			break;
		case 2: 	// X:↑
			Scan_PointIncOffsetY = dOffSetValueX - m_ScanPoint[iPoint].lX;
			break;
		case 3: 	// X:←
			Scan_PointIncOffsetX = dOffSetValueX - m_ScanPoint[iPoint].lX;
			break;
		case 4: 	// X:↓
			Scan_PointIncOffsetY = dOffSetValueX + m_ScanPoint[iPoint].lX;
			break;
		}

		switch (lDirY) {
		case 1: 	// Y:→
			Scan_PointIncOffsetX = dOffSetValueY + m_ScanPoint[iPoint].lY;
			break;
		case 2: 	// Y:↑
		default:
			Scan_PointIncOffsetY = dOffSetValueY - m_ScanPoint[iPoint].lY;
			break;
		case 3: 	// Y:←
			Scan_PointIncOffsetX = dOffSetValueY - m_ScanPoint[iPoint].lY;
			break;
		case 4: 	// Y:↓
			Scan_PointIncOffsetY = dOffSetValueY + m_ScanPoint[iPoint].lY;
			break;
		}
// 2009.09.07 bagus stress 原点位置対応 --}--
		m_SamplePointGraph.AddPoint(Scan_PointIncOffsetX, Scan_PointIncOffsetY);
	}
#else

	int nCount;
	if ((m_iNewHeadType == HEAD_TYPE_SR) && (m_SRScanType == MEAS_PROG_TYPE_SR_DISTANCE)) {
		nCount = m_StageProgInfoHdr.wNumScans * 2;
	} else {
		nCount = m_StageProgInfoHdr.wNumScans;
	}

	for(int iPoint= 0; iPoint<nCount; iPoint++){
		switch (lDirX) {
		case 1: 	// X:→
		default:
			Scan_PointIncOffsetX = dOffSetValueX + m_ScanPoint[iPoint].lX;
			break;
		case 2: 	// X:↑
			Scan_PointIncOffsetY = dOffSetValueX - m_ScanPoint[iPoint].lX;
			break;
		case 3: 	// X:←
			Scan_PointIncOffsetX = dOffSetValueX - m_ScanPoint[iPoint].lX;
			break;
		case 4: 	// X:↓
			Scan_PointIncOffsetY = dOffSetValueX + m_ScanPoint[iPoint].lX;
			break;
		}

		switch (lDirY) {
		case 1: 	// Y:→
			Scan_PointIncOffsetX = dOffSetValueY + m_ScanPoint[iPoint].lY;
			break;
		case 2: 	// Y:↑
		default:
			Scan_PointIncOffsetY = dOffSetValueY - m_ScanPoint[iPoint].lY;
			break;
		case 3: 	// Y:←
			Scan_PointIncOffsetX = dOffSetValueY - m_ScanPoint[iPoint].lY;
			break;
		case 4: 	// Y:↓
			Scan_PointIncOffsetY = dOffSetValueY + m_ScanPoint[iPoint].lY;
			break;
		}
		m_SamplePointGraph.AddPoint(static_cast<long>(Scan_PointIncOffsetX), static_cast<long>(Scan_PointIncOffsetY));
	}
#endif
// 2009.10.28 bagus 2点間 --}--

	switch(m_iNewHeadType){
	case HEAD_TYPE_STRESS:
		break;
	default:
		return;
	}

	for (i=0; (DWORD)i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
		if(i >= STRESS_PINS_MAX){
			break;
		}

		for (j=0; (DWORD)j<m_StressConfig.Line[i].dwSectionNum; j++) {
			if (m_StageStressProg.Line[i].bValidLine) {
				tLineData.Status = CNanoPointGraph::SECTION_NOT_MEASURED;
			} else {
				tLineData.Status = CNanoPointGraph::SECTION_NOT_USED;
			}
			if(j >= STRESS_SECTIONS_MAX){
				break;
			}
			if(!m_StageStressProg.Line[i].bScanValid[j]){
				continue;
			}

// 2009.09.07 bagus stress 原点位置対応 --{--
//			tLineData.StartX = m_StressConfig.Line[i].SectPos[j].lScanStartPosX + dOffSetValueX;
//			tLineData.StartY = -(m_StressConfig.Line[i].SectPos[j].lScanPosY - dOffSetValueY);
//			tLineData.EndX = m_StressConfig.Line[i].SectPos[j].lScanEndPosX + dOffSetValueX;
//			tLineData.EndY = -(m_StressConfig.Line[i].SectPos[j].lScanPosY - dOffSetValueY);
			switch (lDirX) {
			case 1: 	// X:→
			default:
				tLineData.StartX = static_cast<long>(dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanStartPosX);
				tLineData.EndX = static_cast<long>(dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanEndPosX);
				break;
			case 2: 	// X:↑
				tLineData.StartY = static_cast<long>(dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX);
				tLineData.EndY = static_cast<long>(dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanEndPosX);
				break;
			case 3: 	// X:←
				tLineData.StartX = static_cast<long>(dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX);
				tLineData.EndX = static_cast<long>(dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanEndPosX);
				break;
			case 4: 	// X:↓
				tLineData.StartY = static_cast<long>(dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanStartPosX);
				tLineData.EndY = static_cast<long>(dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanEndPosX);
				break;
			}

			switch (lDirY) {
			case 1: 	// Y:→
				tLineData.StartX = static_cast<float>(dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY);
				tLineData.EndX = static_cast<long>(dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY);
				break;
			case 2: 	// Y:↑
			default:
				tLineData.StartY = static_cast<float>(dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY);
				tLineData.EndY = static_cast<float>(dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY);
				break;
			case 3: 	// Y:←
				tLineData.StartX = static_cast<float>(dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY);
				tLineData.EndX = static_cast<float>(dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY);
				break;
			case 4: 	// Y:↓
				tLineData.StartY = static_cast<float>(dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY);
				tLineData.EndY = static_cast<float>(dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY);
				break;
			}
// 2009.09.07 bagus stress 原点位置対応 --}--

			m_SamplePointGraph.AddLineData(&tLineData);
		}
	}
}

void CRecipeMainSrView::OnSelchangeStageProgram()
{
	UpdateData(TRUE);

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STAGE_PROGRAM);

	CString strBuffer;
	int 	nIndex;

	int 	iNumScans;

	nIndex = pCombo->GetCurSel();

	if (nIndex <= 0) {
		// -1:Not Selected
		//	0:None
		::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
		m_strStageHeadType = "";
		iNumScans = 0;
		::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
		//2009.09.03 bagus se 機能拡張 --{--
		::ZeroMemory(&m_StageStressProg, sizeof(m_StageStressProg));
		//2009.09.03 bagus se 機能拡張 --{--
	} else {
		///// Stage Program Header /////
// 2009.10.09 bagus StagePGM 共通化 --{--
#if 0
		if ( !RecipeFile_LoadRecipe(&m_StageProgInfoHdr, m_MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM) ) {
			::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
// 2009.09.30 bagus Gantry --{--
			m_strStageHeadType = HEAD_TYPE_ITEM[m_MainRcpInfo.MainRcpParam.hdr.wHeadType];
//			if (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR) {
//				m_strStageHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_MainRcpInfo.MainRcpParam.hdr.wHeadType], m_szScanTypeItem[m_MainRcpInfo.MainRcpParam.hdr.wScanType]);
//			} else {
//				m_strStageHeadType = HEAD_TYPE_ITEM[m_MainRcpInfo.MainRcpParam.hdr.wHeadType];
//			}
// 2009.09.30 bagus Gantry --}--
			iNumScans = 0;
		} else {
// 2009.11.04 bagus RS 追加 --{--
#if 0
// 2009.09.30 bagus Gantry --{--
//			m_strStageHeadType = HEAD_TYPE_ITEM[m_StageProgInfoHdr.wHeadType];
			if (m_StageProgInfoHdr.wHeadType == HEAD_TYPE_SR) {
				m_strStageHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_StageProgInfoHdr.wHeadType], m_szScanTypeItem[m_StageProgInfoHdr.wScanType]);
			} else {
				m_strStageHeadType = HEAD_TYPE_ITEM[m_StageProgInfoHdr.wHeadType];
			}
// 2009.09.30 bagus Gantry --}--
#else
// 2009.12.08 bagus --{--
#if 0
			switch (m_StageProgInfoHdr.wHeadType) {
			case HEAD_TYPE_SR:
				m_strStageHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_StageProgInfoHdr.wHeadType], m_szScanTypeItem[m_StageProgInfoHdr.wScanType]);
				break;
			case HEAD_TYPE_4PP:
				m_strStageHeadType = SCAN_TYPE_ITEM_RS;
				break;
			default:
				m_strStageHeadType = HEAD_TYPE_ITEM[m_StageProgInfoHdr.wHeadType];
				break;
			}
#else
			switch (m_StageProgInfoHdr.wHeadType) {
			case HEAD_TYPE_SR:
// 2009.12.09 bagus --{--
//				m_strStageHeadType.Format("%s%s", HEAD_TYPE_ITEM[m_StageProgInfoHdr.wHeadType], m_szScanTypeItem[m_StageProgInfoHdr.wScanType]);
				m_strStageHeadType.Format("%s%s", HEAD_TYPE_ITEM_SHORT[m_StageProgInfoHdr.wHeadType], m_szScanTypeItem[m_StageProgInfoHdr.wScanType]);
// 2009.12.09 bagus --}--
				break;
			default:
// 2009.12.09 bagus --{--
//				m_strStageHeadType = HEAD_TYPE_ITEM[m_StageProgInfoHdr.wHeadType];
				m_strStageHeadType = HEAD_TYPE_ITEM_SHORT[m_StageProgInfoHdr.wHeadType];
// 2009.12.09 bagus --}--
				break;
			}
#endif
// 2009.12.08 bagus --}--
#endif
// 2009.11.04 bagus RS 追加 --}--
			iNumScans = (int)m_StageProgInfoHdr.wNumScans;
		}
#else
		int nHeadType = STAGE_PGM_TYPE_STANDARD;
		int nScanType = SCAN_TYPE_STANDARD;

		int i;

		m_strStageHeadType = "";

		if ( !RecipeFile_LoadRecipe(&m_StageProgInfoHdr, m_MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM) ) {
			::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
			iNumScans = 0;
		} else {
			nHeadType = m_StageProgInfoHdr.wHeadType;
			nScanType = m_StageProgInfoHdr.wScanType;
			iNumScans = (int)m_StageProgInfoHdr.wNumScans;
		}

// 2009.10.23 bagus Stress 修正 --{--
#if 0
		for(i=0; i<sizeof(s_StagePGMTable)/sizeof(s_StagePGMTable[0]); i++) {
			if ((s_StagePGMTable[i].nHead == nHeadType) && (s_StagePGMTable[i].nScan == nScanType)) {
				m_strStageHeadType.Format("%s", s_StagePGMTable[i].szStagePGMItem);
				break;
			}
		}
#else
		BOOL bOld = FALSE;

		// 旧型仕様
		switch (nHeadType) {
		case HEAD_TYPE_SR:
// 2009.12.09 bagus --{--
//			m_strStageHeadType.Format("%s%s", HEAD_TYPE_ITEM[nHeadType], m_szScanTypeItem[nScanType]);
			m_strStageHeadType.Format("%s%s", HEAD_TYPE_ITEM_SHORT[nHeadType], m_szScanTypeItem[nScanType]);
// 2009.12.09 bagus --}--
			bOld = TRUE;
			break;
		case HEAD_TYPE_SE:
// 2009.12.09 bagus --{--
//			m_strStageHeadType = HEAD_TYPE_ITEM[nHeadType];
			m_strStageHeadType = HEAD_TYPE_ITEM_SHORT[nHeadType];
// 2009.12.09 bagus --}--
			bOld = TRUE;
			break;
		case HEAD_TYPE_4PP:
// 2009.12.08 bagus --{--
#if 0
// 2009.11.04 bagus RS 追加 --{--
//			m_strStageHeadType = HEAD_TYPE_ITEM[nHeadType];
			m_strStageHeadType = SCAN_TYPE_ITEM_RS;
// 2009.11.04 bagus RS 追加 --}--
#else
// 2009.12.09 bagus --{--
//			m_strStageHeadType = HEAD_TYPE_ITEM[nHeadType];
			m_strStageHeadType = HEAD_TYPE_ITEM_SHORT[nHeadType];
// 2009.12.09 bagus --}--
#endif
// 2009.12.08 bagus --}--
			bOld = TRUE;
			break;
		case HEAD_TYPE_CTA:
// 2009.12.09 bagus --{--
//			m_strStageHeadType = HEAD_TYPE_ITEM[nHeadType];
			m_strStageHeadType = HEAD_TYPE_ITEM_SHORT[nHeadType];
// 2009.12.09 bagus --}--
			bOld = TRUE;
			break;
		case HEAD_TYPE_MS:
// 2009.12.09 bagus --{--
//			m_strStageHeadType = HEAD_TYPE_ITEM[nHeadType];
			m_strStageHeadType = HEAD_TYPE_ITEM_SHORT[nHeadType];
// 2009.12.09 bagus --}--
			bOld = TRUE;
			break;
		default:
			break;
		}

		// 新型仕様
		if (!bOld) {
			for(i=0; i<sizeof(s_StagePGMTable)/sizeof(s_StagePGMTable[0]); i++) {
				if ((s_StagePGMTable[i].nHead == nHeadType) && (s_StagePGMTable[i].nScan == nScanType)) {
					m_strStageHeadType.Format("%s", s_StagePGMTable[i].szStagePGMItem);
					break;
				}
			}
		}
#endif
// 2009.10.23 bagus Stress 修正 --}--
#endif
// 2009.10.09 bagus StagePGM 共通化 --}--

// 2009.10.28 bagus 2点間 --{--
#if 0
		///// Scan Points /////
		if ( !RecipeFile_LoadPointList(m_ScanPoint, iNumScans, m_MainRcpInfo.MainRcpParam.hdr.szStage) ) {
			::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
		}
#else
		///// Scan Points /////
		if ((nHeadType == HEAD_TYPE_SR) && (nScanType == SCAN_TYPE_SR_DISTANCE)) {
			if ( !RecipeFile_Load2PointList(m_ScanPoint, iNumScans, m_MainRcpInfo.MainRcpParam.hdr.szStage) ) {
				::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
			}
		} else {
			if ( !RecipeFile_LoadPointList(m_ScanPoint, iNumScans, m_MainRcpInfo.MainRcpParam.hdr.szStage) ) {
				::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
			}
		}
#endif
// 2009.10.28 bagus 2点間 --}--

		//2009.09.03 bagus se 機能拡張 --{--
		if ( !RecipeFile_LoadRecipe(&m_StageStressProg, m_MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM_STRESS) ) {
			::ZeroMemory(&m_StageStressProg, sizeof(m_StageStressProg));
		}
		//2009.09.03 bagus se 機能拡張 --{--
	}

	UpdateData(FALSE);

	SamplePointGraph_Init();

	Invalidate(TRUE);
}
//2009.09.02 bagus se 機能拡張 --}--

//2009.12.21 bagus --{--
void CRecipeMainSrView::OnMeasurementResultAutoSave()
{
	CButton *pCheck = (CButton *)GetDlgItem(IDC_MEASUREMENT_RESULT_AUTO_SAVE);

	if(!pCheck)
		return;

	if(pCheck->GetCheck()){
		GetDlgItem(IDC_MEASUREMENT_RESULT_SAVE_WITH_DATE)->EnableWindow(TRUE);
	}else{
		GetDlgItem(IDC_MEASUREMENT_RESULT_SAVE_WITH_DATE)->EnableWindow(FALSE);
	}

}
//2009.12.21 bagus --}--


//Saiki 20110201 Add ----->
// =========================================================================
//bEnable : (Default)->FALSE
//FALSE : 応力測定以外, TRUE : 応力測定
// =========================================================================
void CRecipeMainSrView::InitThickOptionsForStress(BOOL bEnable)
{

	GetDlgItem(IDC_STATIC_STRESS_MEASURE_OPTION)->ShowWindow(bEnable == TRUE ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_RADIO_ACTUAL_VALUE)->ShowWindow(bEnable == TRUE ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_RADIO_FIXED_VALUE)->ShowWindow(bEnable == TRUE ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_THICK_SETTING_BUTTON)->ShowWindow(bEnable == TRUE ? SW_SHOW : SW_HIDE);

	if(bEnable == TRUE){
		CButton *pCheckActual = (CButton *)GetDlgItem(IDC_RADIO_ACTUAL_VALUE);
		CButton *pCheckFixed = (CButton *)GetDlgItem(IDC_RADIO_FIXED_VALUE);
		if(m_StressParamInfo.bPriority){//Fixed Value Mode
			GetDlgItem(IDC_THICK_SETTING_BUTTON)->EnableWindow(TRUE);
			pCheckActual->SetCheck(0);
			pCheckFixed->SetCheck(1);

			EnableRecalibrationForStress(FALSE);
		}
		else{
			GetDlgItem(IDC_THICK_SETTING_BUTTON)->EnableWindow(FALSE);
			pCheckActual->SetCheck(1);
			pCheckFixed->SetCheck(0);

			EnableRecalibrationForStress(TRUE);
		}
	}
}

void CRecipeMainSrView::OnSelectThickOptionsForStress(BOOL bEnable)
{

	GetDlgItem(IDC_STATIC_STRESS_MEASURE_OPTION)->ShowWindow(bEnable == TRUE ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_RADIO_ACTUAL_VALUE)->ShowWindow(bEnable == TRUE ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_RADIO_FIXED_VALUE)->ShowWindow(bEnable == TRUE ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_THICK_SETTING_BUTTON)->ShowWindow(bEnable == TRUE ? SW_SHOW : SW_HIDE);

	if(bEnable == TRUE){
		CButton *pCheckActual = (CButton *)GetDlgItem(IDC_RADIO_ACTUAL_VALUE);
		CButton *pCheckFixed = (CButton *)GetDlgItem(IDC_RADIO_FIXED_VALUE);
		if(pCheckActual->GetCheck()==0 && pCheckFixed->GetCheck()==1){//Fixed Value Mode
			GetDlgItem(IDC_THICK_SETTING_BUTTON)->EnableWindow(TRUE);
			pCheckActual->SetCheck(0);
			pCheckFixed->SetCheck(1);

			EnableRecalibrationForStress(FALSE);
		}
		else{
			GetDlgItem(IDC_THICK_SETTING_BUTTON)->EnableWindow(FALSE);
			pCheckActual->SetCheck(1);
			pCheckFixed->SetCheck(0);

		EnableRecalibrationForStress(TRUE);
		}
	}
}

void CRecipeMainSrView::OnThickSettingButton()
{
	CRecipeThicknessSettingView dlg;

	UpdateData(TRUE);

	dlg.m_nOpenMode = m_nOpenMode;
	::CopyMemory(&dlg.m_StressParamInfo, &m_StressParamInfo, sizeof(STRESS_PARAM_INFO));
	::CopyMemory(&dlg.m_MainRcpInfo, &m_MainRcpInfo, sizeof(MAIN_RCP_INFO));

	if(dlg.DoModal()==IDOK){
		::CopyMemory(&m_StressParamInfo, &dlg.m_StressParamInfo, sizeof(STRESS_PARAM_INFO));
	}
}
void CRecipeMainSrView::OnRadioThicknessTypeForStressValue()
{
	CButton *pCheckActual = (CButton *)GetDlgItem(IDC_RADIO_ACTUAL_VALUE);
	CButton *pCheckFixed = (CButton *)GetDlgItem(IDC_RADIO_FIXED_VALUE);
	if(pCheckActual->GetCheck() == 0 && pCheckFixed->GetCheck() == 1){//Fixed Value Mode

		GetDlgItem(IDC_THICK_SETTING_BUTTON)->EnableWindow(TRUE);
		EnableRecalibrationForStress(FALSE);
		m_StressParamInfo.bPriority = TRUE;
	}
	else{
		GetDlgItem(IDC_THICK_SETTING_BUTTON)->EnableWindow(FALSE);
		EnableRecalibrationForStress(TRUE);
		m_StressParamInfo.bPriority = FALSE;
	}
}

void CRecipeMainSrView::EnableRecalibrationForStress(BOOL bEnable)
{
	CComboBox* pCombo;
	if(!bEnable){
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_ITEM1);
		pCombo->SetCurSel(0);
		pCombo = (CComboBox*)GetDlgItem(IDC_RECALIBRATION_1);
		pCombo->SetCurSel(0);
	}

	GetDlgItem(IDC_RECALIBRATION_ITEM1)->EnableWindow(bEnable);
	GetDlgItem(IDC_RECALIBRATION_1)->EnableWindow(bEnable);
	GetDlgItem(IDC_RECALIB_1_HEAD_STATIC)->EnableWindow(bEnable);
	GetDlgItem(IDC_RECALIB_1_HEAD_TYPE)->EnableWindow(bEnable);

}

BOOL CRecipeMainSrView::CheckStressThicknessData(MAIN_RCP_INFO MainRcpInfo)
{
	double dtmpVal;
	int iCount, jCount;

	CString	strBuffer;
	CString strThick;
	char szMessage[256];

	if(m_StressParamInfo.bPriority == FALSE) {return TRUE;}

	switch(m_nOpenMode)
	{
		case modeDefaultSetting:
		break;
		default:
		if( strlen(MainRcpInfo.MainRcpParam.hdr.szStage) == 0 ){//Stage Program 未選択
			return FALSE;
		}
		else{//StageProgram選択済み
			for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
				for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
					strThick.Format("%f", m_StressParamInfo.Line[iCount].dPtThick[jCount]);
					if (!m_StageStressProg.Line[iCount].bValidLine) {// 無効Lineの場合
						if(CheckInputValueDouble(strThick, &dtmpVal)){
							continue;
						}
						else{
							if(strThick.IsEmpty() != 0){
								m_StressParamInfo.Line[iCount].dPtThick[jCount] = 0.0;
								continue;
							}
							else{
								Invalidate( FALSE );
								LoadStringML(IDS_STRESS_ENTER_NUM_THICK, strBuffer, "Please Enter the Numbers L%dP%d Thickenss.");
								sprintf(szMessage, strBuffer, iCount+1, jCount+1);
								AfxMessageBox(szMessage);
								return FALSE;
							}
						}
					}
					else{
						if (CheckInputValueDouble(strThick, &dtmpVal)){// 有効Lineの場合
							if(dtmpVal > 0){
								continue;
							}
							Invalidate( FALSE );
							LoadStringML(IDS_STRESS_INPUT_THICKNESS, strBuffer, "Please input L%dP%d Thickenss.");
							sprintf(szMessage, strBuffer, iCount+1, jCount+1);
							AfxMessageBox(szMessage);
							return FALSE;
						}
					}
				}
			}
		}
		break;
	}
	return TRUE;
}

BOOL CRecipeMainSrView::CheckInputValueLong(CString strVal, long *lValue)
{
	long	lChk;
	char	*pChk = new char[strVal.GetLength()+1];
	char	*pEnd = NULL;

	strcpy(pChk, strVal);

	lChk = strtol(strVal, &pEnd, 10);
	errno = 0;
	if (errno == ERANGE) {
		delete [] pChk;
		return ( FALSE );
	}

	if ((pEnd != NULL) && (*pEnd != '\0')) {
		if (&pChk[strlen(pChk)-1] != pEnd) {
			delete [] pChk;
			return ( FALSE );
		}
	}

	delete [] pChk;

	*lValue = lChk;

	return ( TRUE );
}

BOOL CRecipeMainSrView::CheckInputValueDouble(CString strVal, double *dValue)
{
	double	dChk;
	char	*pChk = new char[strVal.GetLength()+1];
	char	*pEnd = NULL;

	strcpy(pChk, strVal);

	dChk = strtod(strVal, &pEnd);
	errno = 0;
	if (errno == ERANGE) {
		delete [] pChk;
		return ( FALSE );
	}

	if ((pEnd != NULL) && (*pEnd != '\0')) {
		if (&pChk[strlen(pChk)-1] != pEnd) {
			delete [] pChk;
			return ( FALSE );
		}
	}

	delete [] pChk;

	*dValue = dChk;

	return ( TRUE );
}

//Saiki 20110201 Add <-----
