// MeasurementOperationView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
// 2009.08.28 bagus stress --{--
#include "SelectStressReferenceTypeDlg.h"
#include "StressReferenceListDlg.h"
// 2009.08.28 bagus stress --}--
#include "ReferenceMenuDlg.h"
#include "RecipeMainListView.h"
#include "MeasurementOperationView.h"
#include "NanoSpecDoc.h"
#include "SampleIdDlg.h"
#include "ChiefExports.h"
#include "OverlayDialog.h"
#include "ManualStageDlg.h"
#include "VacuumChangeDlg.h"
#include "ExecutionDlg.h"
#include "NEXIOBASE.HXX"
#include "..\\..\\INC\\PifComm.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MEAOPE_BUTTON_ENABLE_CHANGE_TIMER 		(1)
#define MEAOPE_MAINTE_WATCH_TIMER 				(2)


// ##########################################################################
// CMeasurementOperationView
// ##########################################################################

// ==========================================================================
//
IMPLEMENT_DYNCREATE(CMeasurementOperationView, CNanoUI)

// ==========================================================================
//
CMeasurementOperationView::CMeasurementOperationView()
	: CNanoUI(CMeasurementOperationView::IDD)
{
	//{{AFX_DATA_INIT(CMeasurementOperationView)
	//}}AFX_DATA_INIT

	m_CurrRecipe = MEASUREMENT_MAIN_RECIPE;

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;

}

// ==========================================================================
//
CMeasurementOperationView::~CMeasurementOperationView()
{
}

// ==========================================================================
//
void CMeasurementOperationView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasurementOperationView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_MAIN_RECIPE_BUTTON, m_MainRecipeButton);
	DDX_Control(pDX, IDC_MULTI_RECIPE_BUTTON, m_MultiRecipeButton);
	DDX_Control(pDX, IDC_START_BUTTON, m_StartButton);
	DDX_Control(pDX, IDC_HANDLER_LOAD_POSITION_BUTTON, m_HandlerLoadPositionButton);
	DDX_Control(pDX, IDC_MANUAL_LOAD_POSITION_BUTTON, m_ManualLoadPositionButton);
	DDX_Control(pDX, IDC_VACUUM_BUTTON, m_VacuumButton);
	DDX_Control(pDX, IDC_GLASS_LIFTER_BUTTON, m_GlassLifterButton);
	DDX_Control(pDX, IDC_REFERENCE_BUTTON, m_ReferenceButton);
	DDX_Control(pDX, IDC_MANUAL_STAGE_BUTTON, m_ManualStageButton);
	DDX_Control(pDX, IDC_MANUAL_MEASUREMENT_BUTTON, m_ManualMeasurementButton);
	DDX_Control(pDX, IDC_SAMPLE_LOAD_BUTTON, m_SampleLoadButton);
	DDX_Control(pDX, IDC_SAMPLE_UNLOAD_BUTTON, m_SampleUnLoadButton);
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CMeasurementOperationView, CNanoUI)
	//{{AFX_MSG_MAP(CMeasurementOperationView)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	ON_BN_CLICKED(IDC_HANDLER_LOAD_POSITION_BUTTON, OnHandlerLoadPositionButton)
	ON_BN_CLICKED(IDC_MANUAL_LOAD_POSITION_BUTTON, OnManualLoadPositionButton)
	ON_BN_CLICKED(IDC_VACUUM_BUTTON, OnVacuumButton)
	ON_BN_CLICKED(IDC_GLASS_LIFTER_BUTTON, OnGlassLifterButton)
	ON_BN_CLICKED(IDC_REFERENCE_BUTTON, OnReferenceButton)
	ON_BN_CLICKED(IDC_MANUAL_MEASUREMENT_BUTTON, OnManualMeasurementButton)
	ON_BN_CLICKED(IDC_MAIN_RECIPE_BUTTON, OnMainRecipeButton)
	ON_BN_CLICKED(IDC_MULTI_RECIPE_BUTTON, OnMultiRecipeButton)
	ON_BN_CLICKED(IDC_SAMPLE_LOAD_BUTTON, OnSampleLoadButton)
	ON_BN_CLICKED(IDC_SAMPLE_UNLOAD_BUTTON, OnSampleUnloadButton)
	ON_BN_CLICKED(IDC_MANUAL_STAGE_BUTTON, OnManualStageButton)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_CHIF_MOVE_SETPOS_END, OnMoveSetposEnd)
	ON_MESSAGE(WM_DISPMENU_MEASUREMENT_START, OnMeaOpeFuncCall)
	ON_MESSAGE(WM_DISPMENU_MANUAL_MEASUREMENT, OnMeaOpeFuncCall)
	ON_MESSAGE(WM_DISPMENU_HANDLER_LOAD_POSITION, OnMeaOpeFuncCall)
	ON_MESSAGE(WM_DISPMENU_MANUAL_STAGE, OnMeaOpeFuncCall)
	ON_MESSAGE(WM_DISPMENU_VACUUM, OnMeaOpeFuncCall)
	ON_MESSAGE(WM_DISPMENU_REFERENCE, OnMeaOpeFuncCall)

END_MESSAGE_MAP()

// ==========================================================================
// CMeasurementOperationView 診断

#ifdef _DEBUG
// ==========================================================================
//
void CMeasurementOperationView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// ==========================================================================
//
void CMeasurementOperationView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// ==========================================================================
// CMeasurementOperationView メッセージ ハンドラ

// ==========================================================================
//
void CMeasurementOperationView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	m_pMainFrame->m_pMeaOpeViewObj = this;


	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	//システムコンフィグ設定読み込み
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);

	///// Multi Recipeが設定されていない場合は、ボタン類の動的レイアウト変更 /////
	if(!m_SystemConfig.bMultiRecipe){
		int iTopOffSet = 2; //1=1/8高さシステムフォント
		int idTop;
		WINDOWPLACEMENT placement;

		GetDlgItem(IDC_RECIPE_GROUP)->ShowWindow(SW_HIDE);
		m_MainRecipeButton.ShowWindow(SW_HIDE);
		m_MultiRecipeButton.ShowWindow(SW_HIDE);

		m_StartButton.GetWindowPlacement(&placement);
		idTop = placement.rcNormalPosition.top - iTopOffSet;

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
			placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
			m_StartButton.SetWindowPlacement(&placement);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		m_ManualMeasurementButton.GetWindowPlacement(&placement);
		placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
		m_ManualMeasurementButton.SetWindowPlacement(&placement);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			m_HandlerLoadPositionButton.GetWindowPlacement(&placement);
			placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
			placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
			m_HandlerLoadPositionButton.SetWindowPlacement(&placement);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		m_ManualLoadPositionButton.GetWindowPlacement(&placement);
		placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
		m_ManualLoadPositionButton.SetWindowPlacement(&placement);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			m_VacuumButton.GetWindowPlacement(&placement);
			placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
			placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
			m_VacuumButton.SetWindowPlacement(&placement);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		m_GlassLifterButton.GetWindowPlacement(&placement);
		placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
		m_GlassLifterButton.SetWindowPlacement(&placement);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			m_ReferenceButton.GetWindowPlacement(&placement);
			placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
			placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
			m_ReferenceButton.SetWindowPlacement(&placement);

			m_ManualStageButton.GetWindowPlacement(&placement);
			placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
			placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
			m_ManualStageButton.SetWindowPlacement(&placement);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		m_SampleLoadButton.GetWindowPlacement(&placement);
		placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
		m_SampleLoadButton.SetWindowPlacement(&placement);

		m_SampleUnLoadButton.GetWindowPlacement(&placement);
		placement.rcNormalPosition.top	= placement.rcNormalPosition.top - idTop;
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - idTop;
		m_SampleUnLoadButton.SetWindowPlacement(&placement);
	}

// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
		m_StartButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		///// Start Button /////
		m_StartButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_START_COLOR);
		m_StartButton.DrawFlatFocus(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	///// Manual Measurement Button /////
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
		CString str;
		GetDlgItem(IDC_START_BUTTON)->GetWindowText(str);
		GetDlgItem(IDC_MANUAL_MEASUREMENT_BUTTON)->SetWindowText(str);
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	m_ManualMeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ManualMeasurementButton.DrawFlatFocus(TRUE);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
		m_HandlerLoadPositionButton.ShowWindow(SW_HIDE);
		m_ReferenceButton.ShowWindow(SW_HIDE);
		m_VacuumButton.ShowWindow(SW_HIDE);
		m_ManualStageButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		///// Hander Load Position Button /////
		if(m_SystemConfig.bHost){
			m_HandlerLoadPositionButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
				BUTTON_COLORING, BUTTON_NORMAL_COLOR);
			m_HandlerLoadPositionButton.DrawFlatFocus(TRUE);
		}
		else{
			m_HandlerLoadPositionButton.ShowWindow(SW_HIDE);
		}
	
		///// Reference Button /////
		m_ReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_ReferenceButton.DrawFlatFocus(TRUE);

		///// Vacuum Button /////
		if(m_SystemConfig.nVacuumType > 0){
			m_VacuumButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
				BUTTON_COLORING, BUTTON_NORMAL_COLOR);
			m_VacuumButton.DrawFlatFocus(TRUE);
		}
		else{
			m_VacuumButton.ShowWindow(SW_HIDE);
		}

		///// Manual Stage Button /////
		m_ManualStageButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_ManualStageButton.DrawFlatFocus(TRUE);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	///// Manual Load Position Button /////
	m_ManualLoadPositionButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ManualLoadPositionButton.DrawFlatFocus(TRUE);
	// 非表示
	m_ManualLoadPositionButton.ShowWindow(SW_HIDE);

	///// Glass Lifter Button /////
	if(m_SystemConfig.nSampleLifter > 0){
		m_GlassLifterButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_GlassLifterButton.DrawFlatFocus(TRUE);
	}
	else{
		m_GlassLifterButton.ShowWindow(SW_HIDE);
	}
	// 非表示
	m_GlassLifterButton.ShowWindow(SW_HIDE);

	///// Sample Load Button /////
	m_SampleLoadButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleLoadButton.DrawFlatFocus(TRUE);
	// 非表示
	m_SampleLoadButton.ShowWindow(SW_HIDE);

	///// Sample UnLoad Button /////
	m_SampleUnLoadButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SampleUnLoadButton.DrawFlatFocus(TRUE);
	// 非表示
	m_SampleUnLoadButton.ShowWindow(SW_HIDE);

	///// Select Main Recipe /////
	SelectRecipeButton(MEASUREMENT_MAIN_RECIPE);

	if(m_pDoc->GetHostMode() == HOST_REMOTE) //リモート時
	{
		ButtonEnableChange(MEAS_OPE_ALL_DISABLE);
	}

	SetTimer(MEAOPE_BUTTON_ENABLE_CHANGE_TIMER, 50, NULL);
	SetTimer(MEAOPE_MAINTE_WATCH_TIMER, 50, NULL);
}

// ==========================================================================
//
void CMeasurementOperationView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CNanoUI::OnShowWindow(bShow, nStatus);

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

}

// ==========================================================================
//
void CMeasurementOperationView::OnMainRecipeButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if(m_pDoc->GetInitFinished() == FALSE) return;

	if(m_CurrRecipe == MEASUREMENT_MAIN_RECIPE) return;

	pMainFrame->ChangeExeWnd(RECIPE_MAIN_LIST_WND);

	SelectRecipeButton(MEASUREMENT_MAIN_RECIPE);
}

// ==========================================================================
//
void CMeasurementOperationView::OnMultiRecipeButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if(m_pDoc->GetInitFinished() == FALSE) return;

	if(m_CurrRecipe == MEASUREMENT_MULTI_RECIPE) return;

	pMainFrame->ChangeExeWnd(RECIPE_MULTI_LIST_WND);

	SelectRecipeButton(MEASUREMENT_MULTI_RECIPE);
}

// ==========================================================================
//
void CMeasurementOperationView::OnStartButton()
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2, strMsg;
// Kojika 20090526 Add End

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	RCP_DATA rcp_data;
	BOOL bMain,bMeas,bStage,bPList,bStageStress;
//	int iMeasureNum;				//測定ポイント数
//	char szUserId[MAX_PATH +1 ];	//ユーザID
	static char szRecipeName[RECIPE_NAME_LEN + 1];
//	char* pszCaption = "MEASUREMENT START";
//	char* pszCaption = "測定開始";
	int ChiefRet = 0;
	BOOL bResult = FALSE;
//	int iDispLabelCount;


	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
		  この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //メンテナンス・スイッチ・オン
	}

	// 各I/Oチェック
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* エア圧力低下 */
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	if(m_pDoc->GetInitFinished() == FALSE) return;

	m_pDoc->OperationLogging("Measurement Operation - START Button was pushed.");

	///// Check Host Mode /////
	if(m_pDoc->GetHostMode() != HOST_LOCAL){
// Kojika 20090526 Change
//		MessageBox("HOST MODE is not LOCAL.", pszCaption, MB_OK | MB_ICONWARNING);
//		MessageBox("ホストモードがローカルではありません", pszCaption, MB_OK | MB_ICONWARNING);
		LoadStringML(IDS_HOST_MODE_NOT_LOCAL, strBuffer1, "HOST MODE is not LOCAL.");
		LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer2, "MEASUREMENT START");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONWARNING);
// Kojika 20090526 Change End

		m_pDoc->OperationLogging("Measurement Operation - START BUTTON - ABORT : HOST MODE is not LOCAL.");
		return;
	}

	///// メモリー初期化 /////
	m_pDoc->InitMeasData();



	///// 現在選択されているメインレシピをロード /////
	if(m_CurrRecipe == MEASUREMENT_MAIN_RECIPE)
		((CRecipeMainListView *)pMainFrame->m_wndSplitter2.GetPane(0,0))->RecipeStart(szRecipeName);


	///// Recipe 取得 /////
	strcpy(rcp_data.szRecipeName, szRecipeName);
	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bMeas = RecipeFile_LoadRecipe(&rcp_data.MeasProgInfo, rcp_data.MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
	bStage = RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
	//2009.10.28 bagus 2point-distance
	if(rcp_data.StageProgInfoHdr.wScanType == SCAN_TYPE_SR_DISTANCE
	&& rcp_data.StageProgInfoHdr.wHeadType == HEAD_TYPE_SR
	){
		//2点間距離用に座標を取り込む
		bPList = RecipeFile_Load2PointList(rcp_data.ScanPoint, rcp_data.StageProgInfoHdr.wNumScans, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage);
	}else{
	bPList = RecipeFile_LoadPointList(rcp_data.ScanPoint, rcp_data.StageProgInfoHdr.wNumScans, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage);
	}
	//2009.10.28 bagus 2point-distance
	//2009.08.21 bagus stress --{--
	//ストレスヘッダなのでストレス用の情報を別途取得する
	bStageStress = FALSE;
	if(bStage){
		bStageStress = TRUE;
		if(rcp_data.StageProgInfoHdr.wHeadType == HEAD_TYPE_STRESS){
			bStageStress = RecipeFile_LoadRecipe(&rcp_data.StressStageProgInfo,rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM_STRESS);
		}else{
			memset(&rcp_data.StressStageProgInfo,NULL,sizeof(rcp_data.StressStageProgInfo));
		}
	}
	//2009.08.21 bagus stress --}--
	//Saiki 20091226 Add ----->/*レンズ種別が違うときは測定開始時にはねる*/
	if(rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_MS){
		int nItemData;
		if(0 <= rcp_data.MeasProgInfo.ScanParams._MScope.iMagLensIndex){
			nItemData = rcp_data.MeasProgInfo.ScanParams._MScope.iMagLensIndex;
		}else{
			nItemData = 0;
		}

		if (m_MsConfig.RoboCylinder[nItemData].bSensor != nexioIsMS_LensKind()) {
			// 顕微鏡のレンズの設定が一致しない
			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}
	}
	//Saiki 20091226 Add <-----


	if((bMain == FALSE) || (bMeas == FALSE) || (bStage == FALSE) || (bPList == FALSE) || (bStageStress == FALSE)){
// Kojika 20090526 Change
//		MessageBox("Recipe Abnormal", pszCaption, MB_OK|MB_ICONWARNING);
//		MessageBox("このレシピは異常です。レシピを確認してください", pszCaption, MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_RECIPE_ABNORMAL, strBuffer1, "Recipe Abnormal");
		LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer2, "MEASUREMENT START");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONWARNING);
// Kojika 20090526 Change End
		m_pDoc->OperationLogging("Measurement Operation - START BUTTON - ABORT : Recipe Abnormal.");
		return;
	}

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	m_pDoc->SetSampleIdFromUI(FALSE);
	m_pDoc->SetSampleId("");
	m_pDoc->SetLotId("");
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	///// Sample ID 取得 /////
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.bSampleID == TRUE)
	{
		CSampleIdDlg sampdlg;
		// 2009.10.22 bagus Stress 修正 --{--
		if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS) {
			sampdlg.m_bUseReferenceList = TRUE;
		} else {
			sampdlg.m_bUseReferenceList = FALSE;
		}
		// 2009.10.22 bagus Stress 修正 --}--
		//2009.11.04 bagus RS,CA --{--
		//ロットID（カセットID）が必要なヘッドに対してそれを指示する
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
			sampdlg.m_bUseLotID = TRUE;
		}
		//2009.11.04 bagus RS,CA --}--

		if(sampdlg.DoModal() == IDCANCEL){
			m_pDoc->OperationLogging("Measurement Operation - ABORT : Sample ID Input is canceled.");
			return;
		}

	}


	///// レシピセット /////
	m_pDoc->SetRcpData(&rcp_data);

	///// ローカル/リモート共通処理 /////
	MeasStart(rcp_data.StageProgInfoHdr.wNumScans); //レシピセット後にやる必要アリ



	///// チーフへ測定開始メッセージを送信 /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementStartFailed);
		return;
	}

	memset(m_pDoc->m_bDoPointMeasFlag, FALSE, sizeof(m_pDoc->m_bDoPointMeasFlag));
	//2009.10.28 bagus 2point-Distance --{--
	WORD wNumScans = rcp_data.StageProgInfoHdr.wNumScans;
	if(rcp_data.StageProgInfoHdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.StageProgInfoHdr.wScanType == SCAN_TYPE_SR_DISTANCE){
		wNumScans *= 2;
	}
	//for(int i=0; i<rcp_data.StageProgInfoHdr.wNumScans; i++)
	for(int i=0; i<wNumScans; i++)
	{
		m_pDoc->m_bDoPointMeasFlag[i] = TRUE; //測定実行フラグを立てる
	}
	//2009.10.28 bagus 2point-Distance --}--

	m_pDoc->SetLastMeasPoint(rcp_data.StageProgInfoHdr.wNumScans); //最終測定ポイントを格納
	TRACE("%s%d", "《CMeasurementOperationView::OnStartButton》 iLastMeasPoint:", rcp_data.StageProgInfoHdr.wNumScans);

	LPARAM lparam = (LPARAM)MAKELONG(0, 0);
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SEQ, (WPARAM)szRecipeName, lparam); //通常測定

	///// チーフの結果 /////
	// Kojika 20090604 Change
	//bResult = pMainFrame->JudgeChiefResult(ChiefRet, pszCaption);
	LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer1, "MEASUREMENT START");
	bResult = pMainFrame->JudgeChiefResult(ChiefRet, strBuffer1);
	// Kojika 20090604 Change End

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - START BUTTON - ABORT : Chief Result is Measurement Cancel.");
		return;
	}

	//シーケンス測定モードフラグ設定
	m_pDoc->SetSeqMeasMode(SEQ_ON_NORMALMEASURE); //通常測定中に設定


	//測定モードフラグ設定
	m_pDoc->SetMeasMode(MEASMODE_SEQ_OR_REMEASURE); //通常測定orリメジャー


	m_pDoc->OperationLogging("Measurement Operation - START BUTTON - Measurement was initiated.");

	///// 測定画面へ切替 /////
	pMainFrame->ChangeOperationPanel(OP_MEASUREMENT_PROCESS);
	pMainFrame->ChangeExeWnd(MEAS_WND);

	return;
}

// ==========================================================================
//
void CMeasurementOperationView::OnManualMeasurementButton()
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2, strMsg;
// Kojika 20090526 Add End

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();


	RCP_DATA rcp_data;
	BOOL bMain, bMeas;
	static char szRecipeName[RECIPE_NAME_LEN + 1];
//	char* pszCaption = "MANUAL MEASUREMENT START";
//	  char* pszCaption = "手動測定開始";


	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
		  この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //メンテナンス・スイッチ・オン
	}

	// 各I/Oチェック
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* エア圧力低下 */
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//2009.11.11 bagus MS --{--
	//ロボシリンダー位置が安全な場所にいない可能性がある
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲



	if(m_pDoc->GetInitFinished() == FALSE) return;

	if(m_CurrRecipe != MEASUREMENT_MAIN_RECIPE) return;


	///// メモリー初期化 /////
	m_pDoc->InitMeasData();


	///// 現在選択されているメインレシピをロード /////
	if(m_CurrRecipe == MEASUREMENT_MAIN_RECIPE)
		((CRecipeMainListView *)pMainFrame->m_wndSplitter2.GetPane(0,0))->RecipeStart(szRecipeName);


	///// Recipe 取得 /////
	strcpy(rcp_data.szRecipeName, szRecipeName);
	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bMeas = RecipeFile_LoadRecipe(&rcp_data.MeasProgInfo, rcp_data.MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
//	bStage = RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
//	bPList = RecipeFile_LoadPointList(rcp_data.ScanPoint, rcp_data.StageProgInfoHdr.wNumScans, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage);

	if((bMain == FALSE) || (bMeas == FALSE) /*|| (bStage == FALSE) || (bPList == FALSE)*/){ //ステージプログラムはなくても可
// Kojika 20090526 Change
//		MessageBox("Recipe Abnormal", pszCaption, MB_OK|MB_ICONWARNING);
//		MessageBox("このレシピは異常です。レシピを確認してください", pszCaption, MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_RECIPE_ABNORMAL, strBuffer1, "Recipe Abnormal");
		LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer2, "MEASUREMENT START");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONWARNING);
// Kojika 20090526 Change End
		m_pDoc->OperationLogging("Measurement Operation - START BUTTON - ABORT : Recipe Abnormal.");
		return;
	}

	//2009.09.04 bagus se --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		//2009.09.08 bagus stress --{--
		LoadStringML(IDS_STRESS_MANUAL_MEAS_ERR,strBuffer1,"STRESS: Manual Measurement is not supportted.");
		LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer2, "MEASUREMENT START");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONWARNING);
		//2009.09.08 bagus stress --}--
		m_pDoc->OperationLogging("Measurement Operation - START BUTTON - ABORT : Stress Manual Measurement.");
		return;
	}
	//2009.09.04 bagus se --}--

	//Saiki 20091226 Add ----->/*レンズ種別が違うときは測定開始時にはねる*/
	if(rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_MS){
		int nItemData;
		if(0 <= rcp_data.MeasProgInfo.ScanParams._MScope.iMagLensIndex){
			nItemData = rcp_data.MeasProgInfo.ScanParams._MScope.iMagLensIndex;
		}else{
			nItemData = 0;
		}

		if (m_MsConfig.RoboCylinder[nItemData].bSensor != nexioIsMS_LensKind()) {
			// 顕微鏡のレンズの設定が一致しない
			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}
	}
	//Saiki 20091226 Add <-----

	//2010.01.15 bagus GTR --{--
	//リファレンスの有効期限チェック
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR){
		double l_dLifeTime = rcp_data.MeasProgInfo.Ref.hdr.dLifeTime;			// リファレンス更新間隔[ms](有効期限)
		int l_iRet = MEAS_CheckRefFileElapsedTimeOut(rcp_data.szRecipeName, l_dLifeTime);
		
		if(1 != l_iRet){
			if(FALSE == MEAS_ReadRefFile(rcp_data.szRecipeName))
			{
				//リファレンス読み込み異常
				LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer2, "MEASUREMENT START");
				MessageBox("Reference data is not exist", strBuffer2, MB_OK | MB_ICONWARNING);
				return;
			}
		}
		if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){

			if(l_iRet != 0){
				//リファレンス異常
				LoadStringML(IDS_REFFILE_ELAPSED_TIMEOUT, strBuffer1, "Reference data is not exist or timeout.");
				LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer2, "MEASUREMENT START");
				MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONWARNING);
				return;
			}
		}
	}
	//2010.01.15 bagus GTR --}--

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	m_pDoc->SetSampleIdFromUI(FALSE);
	m_pDoc->SetSampleId("");
	m_pDoc->SetLotId("");
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	///// Sample ID 取得 /////
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.bSampleID == TRUE)
	{
		CSampleIdDlg sampdlg;
		// 2009.10.22 bagus Stress 修正 --{--
		if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS) {
			sampdlg.m_bUseReferenceList = TRUE;
		} else {
			sampdlg.m_bUseReferenceList = FALSE;
		}
		// 2009.10.22 bagus Stress 修正 --}--
		//2009.11.04 bagus RS,CA --{--
		//ロットID（カセットID）が必要なヘッドに対してそれを指示する
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
		|| rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
			sampdlg.m_bUseLotID = TRUE;
		}
		//2009.11.04 bagus RS,CA --}--
		if(sampdlg.DoModal() == IDCANCEL){
			m_pDoc->OperationLogging("Measurement Operation - ABORT : Sample ID Input is canceled.");
			return;
		}

	}


	///// レシピセット /////
	m_pDoc->SetRcpData(&rcp_data);

	//Saiki 20090413 Add ----->
	///// レンズ切替 /////
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(rcp_data.MeasProgInfo.ScanParams._SR.iLens);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);
	//Saiki 20090413 Add <-----

// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType != MODEL_T3100){
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 2013.02.22 bagus Substrate thickness setting -->
		BOOL bSelected;
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
		bSelected = SelectSubThickNotify(rcp_data.StageProgInfoHdr.SampleInfo.dThickness);
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

		if ( !bSelected ) {
			AlarmIf_Set(ALID_SubstrateThicknessError);
		}
// 2013.02.22 bagus Substrate thickness setting <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	///// ローカル/リモート共通処理 /////
	MeasStart(0); //レシピセット後にやる必要アリ

	//2009.11.04 bagus CA --{--
	//マニュアル測定の場合には前処理が必要
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
		if(!m_pDoc->PrepareManualCAMeasure()){
			m_pDoc->OperationLogging("Measurement Operation - ABORT : Failed to prepare manual Contact Angle measurement.");
			//2009.09.08 bagus stress --{--
			LoadStringML(IDS_CA_MANUAL_MEAS_ERR,strBuffer1,"Contace Angle: Can't start manual CTA measurement.");
			LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer2, "MEASUREMENT START");
			MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONWARNING);
			//2009.09.08 bagus stress --}--
			return;
		}
	}

	m_pDoc->SetLastMeasPoint(0); //最終測定ポイントを格納



	//シーケンス測定モードフラグ設定
	m_pDoc->SetSeqMeasMode(SEQ_ON_NORMALMEASURE); //通常測定中に設定


	//測定モードフラグ設定
	m_pDoc->SetMeasMode(MEASMODE_MANUALMEASURE); //マニュアル測定


	//マニュアル測定モード設定
	m_pDoc->SetManuMeasMode(MANUMODE_INIT); //マニュアル測定モードでないに設定


	m_pMainFrame->m_iNowPoint = 0;



	m_pDoc->OperationLogging("Measurement Operation - MANUAL MEASUREMENT Button was pushed.");



/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	if (HEAD_TYPE_MS == rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		/* MS ヘッドの場合のみ，MS ヘッドにオフセット移動します．	*/
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		BOOL l_bRc = SelectHeadTypeNotify(HEAD_TYPE_MS, TRUE);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
		/* SR フィルタを CLOSE します．	*/
		MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	}
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */


	///// 画面へ切替 /////
	pMainFrame->ChangeOperationPanel(OP_MANUAL_MEASUREMENT_FORM);
	pMainFrame->ChangeExeWnd(MANUAL_MEAS_FORM_WND);

	return;
}

// ==========================================================================
//
void CMeasurementOperationView::OnHandlerLoadPositionButton()
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2, strMsg;
// Kojika 20090526 Add End
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	static char szMessage[MAX_PATH];
	int PosId = 0;			// Handler Load Position


	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
		  この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //メンテナンス・スイッチ・オン
	}

	// 各I/Oチェック
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* エア圧力低下 */
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	if(m_pDoc->GetInitFinished() == FALSE) return;

// Kojika 20090526 Change
//	if(MessageBox(	"Do You want to execute Handler Load Position?", "HANDLER LOAD POSITION", MB_OKCANCEL|MB_ICONQUESTION) != IDOK) return;
//	if(MessageBox(	"ハンドラー ロード・ポジションへ移動しますか", "HANDLER LOAD POSITION", MB_OKCANCEL|MB_ICONQUESTION) != IDOK) return;
	LoadStringML(IDS_EXECUTE_HANDLER_LOAD_POS, strBuffer1, "Do You want to execute Handler Load Position?");
	LoadStringML(IDS_TITLE_HANDLER_LOAD_POS, strBuffer2, "LOAD POSITION");
	if(MessageBox(strBuffer1, strBuffer2, MB_OKCANCEL|MB_ICONQUESTION) != IDOK) return;
// Kojika 20090526 Change End

	///// チーフ存在確認 /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementStartFailed);
		return;
	}

	///// チーフへ移動開始メッセージを送信 /////
	::PostMessage(l_hChiefDlg, WM_DISP_MOVE_SETPOS, PosId, 0);

	m_pDoc->OperationLogging("Measurement Operation - HANDLER LOAD POSITION Button was pushed.");
}

// ==========================================================================
//
void CMeasurementOperationView::OnSampleLoadButton()
{
	if(m_pDoc->GetInitFinished() == FALSE) return;

	m_pDoc->OperationLogging("Measurement Operation - SAMPLE LOAD Button was pushed.");
}


// ==========================================================================
//
void CMeasurementOperationView::OnSampleUnloadButton()
{
	if(m_pDoc->GetInitFinished() == FALSE) return;

	m_pDoc->OperationLogging("Measurement Operation - SAMPLE UNLOAD Button was pushed.");
}

// ==========================================================================
//
void CMeasurementOperationView::OnManualLoadPositionButton()
{
	if(m_pDoc->GetInitFinished() == FALSE) return;

	m_pDoc->OperationLogging("Measurement Operation - MANUAL LOAD Button was pushed.");
}

// ==========================================================================
//
void CMeasurementOperationView::OnVacuumButton()
{
	if(m_pDoc->GetInitFinished() == FALSE) return;

	CVacuumChangeDlg dlg;

	dlg.DoModal();

	m_pDoc->OperationLogging("Measurement Operation - VACUUM Button was pushed.");
}

// ==========================================================================
//
void CMeasurementOperationView::OnGlassLifterButton()
{
	CString str;
	//Saiki 20090603 Add ----->
	CString strMsgUp, strMsgDw, strMsg;
	//Saiki 20090603 Add <-----

	if(m_pDoc->GetInitFinished() == FALSE) return;

	GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->GetWindowText(str);
	//Saiki 20090603 Change ----->
////  if ( str == "GLASS LIFT UP"){
////	  GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->SetWindowText("GLASS LIFT DOWN");
//	  if ( str == "ガラス リフト上昇"){
//		  GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->SetWindowText("ガラス リフト下降");
//	  }
//	  else{
////	  GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->SetWindowText("GLASS LIFT UP");
//		  GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->SetWindowText("ガラス リフト上昇");
//	  }

	LoadStringML(IDS_GLASS_LIFT_DOWN, strMsgDw, "GLASS LIFT DOWN");
	LoadStringML(IDS_GLASS_LIFT_UP, strMsgUp, "GLASS LIFT UP");
//	if ( str == "GLASS LIFT UP"){
//		GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->SetWindowText("GLASS LIFT DOWN");
	if ( str == strMsgUp){
		GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->SetWindowText(strMsgDw);
	}
	else{
//		GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->SetWindowText("GLASS LIFT UP");
		GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->SetWindowText(strMsgUp);
	}

	//Saiki 20090603 Change <-----
	GetDlgItem(IDC_GLASS_LIFTER_BUTTON)->RedrawWindow();

	m_pDoc->OperationLogging("Measurement Operation - SAMPLE LIFTER Button was pushed.");
}

// ==========================================================================
//
void CMeasurementOperationView::OnReferenceButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if(m_pDoc->GetInitFinished() == FALSE) return;

	if(m_CurrRecipe != MEASUREMENT_MAIN_RECIPE) return;

	// レシピが選択されていないときはダイアログをださない
	if(!pMainFrame->CheckSelectMainRecipe())
		return;

	CReferenceMenuDlg dlg;
// 2009.08.28 bagus stress --{--
//	dlg.DoModal();
	CSelectStressReferenceTypeDlg	dlg2;
	CStressReferenceListDlg			dlg3;

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	CString	strBuffer1, strBuffer2;

	///// 選択中のメインレシピ名を表示 /////
	char szRecipeName[RECIPE_NAME_LEN + 1];
	memset(szRecipeName, 0, sizeof(szRecipeName));
	((CRecipeMainListView *)pMainFrame->m_wndSplitter2.GetPane(0,0))->RecipeStart(szRecipeName);

	///// Recipe 取得 /////
	strcpy(rcp_data.szRecipeName, szRecipeName);
	if (!RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE)) {
		LoadStringML(IDS_RECIPE_ABNORMAL, strBuffer1, "Recipe Abnormal");
		LoadStringML(IDS_TITLE_MEASUREMENT_START, strBuffer2, "REFERENCE");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONWARNING);
	}

	if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS) {
		if ( dlg2.DoModal() == IDCANCEL )
			return;

		switch ( dlg2.m_SelectedIndex ) {
		case 0:
			dlg.DoModal();
			break;
		case 1:
			dlg3.DoModal();
			break;
		default:
			return;
			break;
		}
	} else {
		dlg.DoModal();
	}
// 2009.08.28 bagus stress --}--
}

// ==========================================================================
//
void CMeasurementOperationView::OnManualStageButton()
{
	//Saiki 20090529 Add ----->
	CString strBuffer, strMsg;
	//Saiki 20090529 Add <-----

	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
		  この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
//		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return 0L;} //メンテナンス・スイッチ・オン
	}

	// 各I/Oチェック
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* エア圧力低下 */
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	if(m_pDoc->GetInitFinished() == FALSE) return;

	CManualStageDlg dlg;

// 2009.10.05 bagus SE --{--
#if 1
	TCHAR szSampleTypeName[RECIPE_NAME_LEN + 1];

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->GetSelectSampleTypeName(szSampleTypeName);

// 2009.10.15 bagus Stress ﾏﾆｭｱﾙｽﾃｰｼﾞ修正 --{--
	dlg.m_nSampleMode = 0;
// 2009.10.15 bagus Stress ﾏﾆｭｱﾙｽﾃｰｼﾞ修正 --}--
#else
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	RCP_DATA rcp_data;
	BOOL bMain,bStage;
	static char szRecipeName[RECIPE_NAME_LEN + 1];

	///// 現在選択されているメインレシピをロード /////
	if(m_CurrRecipe == MEASUREMENT_MAIN_RECIPE)
		((CRecipeMainListView *)pMainFrame->m_wndSplitter2.GetPane(0,0))->RecipeStart(szRecipeName);

	///// Recipe 取得 /////
	strcpy(rcp_data.szRecipeName, szRecipeName);
	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bStage = RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);

	if((bMain == FALSE) || (bStage == FALSE)){
		//レシピが読めなかったのでサンプル表示はなし
		dlg.m_nSampleMode = 0;
	}else{
		dlg.m_nSampleMode = 1;
	}

	strcpy(dlg.m_szRecipeName, rcp_data.StageProgInfoHdr.SampleInfo.szName);
#endif
// 2009.10.05 bagus SE --}--

	//Saiki 20090529 Change ----->
	//dlg.SetCaption(MANUALSTAGE_CAPTION);
	//dlg.SetMessage(MANUALSTAGE_MESSAGE);
	LoadStringML(IDS_MANUALSTAGE_CAPTION, strBuffer, "MANUAL STAGE");
	LoadStringML(IDS_MANUALSTAGE_MESSAGE, strMsg, "MANUAL STAGE");
	dlg.SetCaption(strBuffer);
	dlg.SetMessage(strMsg);
	//Saiki 20090529 Change <-----
	dlg.DoModal();
}

// ==========================================================================
//
void CMeasurementOperationView::SelectRecipeButton(int Select)
{
	CShadeButtonST* pButton;

	m_MainRecipeButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);
	m_MultiRecipeButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);

	switch(Select){
	case MEASUREMENT_MAIN_RECIPE:
		m_CurrRecipe = MEASUREMENT_MAIN_RECIPE;
		pButton = &m_MainRecipeButton;
		break;

	case MEASUREMENT_MULTI_RECIPE:
		m_CurrRecipe = MEASUREMENT_MULTI_RECIPE;
		pButton = &m_MultiRecipeButton;
		break;
	}

	pButton->SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_RADIO_ON_COLOR);

	InvalidateRect(NULL, TRUE);
}

// ==========================================================================
// MeasurementOperationView内のボタン有効/無効の切替え
void CMeasurementOperationView::ButtonEnableChange(int Select)
{
	CButton *button;
	CComboBox *combobox;

	switch(Select){
	case MEAS_OPE_ALL_DISABLE:
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			button = (CButton *)GetDlgItem(IDC_START_BUTTON);
			button->EnableWindow(FALSE); //[測定開始]ボタン無効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		button = (CButton *)GetDlgItem(IDC_MANUAL_MEASUREMENT_BUTTON);
		button->EnableWindow(FALSE); //[手動測定]ボタン無効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			button = (CButton *)GetDlgItem(IDC_HANDLER_LOAD_POSITION_BUTTON);
			button->EnableWindow(FALSE); //[ハンドラー・ロードポジション]ボタン無効
			button = (CButton *)GetDlgItem(IDC_REFERENCE_BUTTON);
			button->EnableWindow(FALSE); //[リファレンス]ボタン無効
			button = (CButton *)GetDlgItem(IDC_VACUUM_BUTTON);
			button->EnableWindow(FALSE); //[バキューム]ボタン無効
			button = (CButton *)GetDlgItem(IDC_MANUAL_STAGE_BUTTON);
			button->EnableWindow(FALSE); //[マニュアル・ステージ]ボタン無効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		//IDD_MAIN_MENU_FORM内コントロールの無効化
		button = (CButton *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON);
		button->EnableWindow(FALSE); //[MEASUREMENT]ボタン無効
		button = (CButton *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON);
		button->EnableWindow(FALSE); //[RECIPE]ボタン無効
		button = (CButton *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON);
		button->EnableWindow(FALSE); //[LOG ON]ボタン無効

		//IDD_VIEW_WINDOW_FORM内コントロールの無効化
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			combobox = (CComboBox*)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO);
			combobox->EnableWindow(FALSE); //レンズ倍率選択無効
			combobox = (CComboBox*)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO);
			combobox->EnableWindow(FALSE); //…無効
			button = (CButton *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON);
			button->EnableWindow(FALSE); //[AUTO FOCUS]ボタン無効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		//メニューバー内(IDR_MEASUREMENT_MENU)の有効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
		m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

		break;

	case MEAS_OPE_ALL_ENABLE:
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			button = (CButton *)GetDlgItem(IDC_START_BUTTON);
			button->EnableWindow(TRUE); //[測定開始]ボタン有効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		button = (CButton *)GetDlgItem(IDC_MANUAL_MEASUREMENT_BUTTON);
		button->EnableWindow(TRUE); //[手動測定]ボタン有効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			button = (CButton *)GetDlgItem(IDC_HANDLER_LOAD_POSITION_BUTTON);
			button->EnableWindow(TRUE); //[ハンドラー・ロードポジション]ボタン有効
			button = (CButton *)GetDlgItem(IDC_REFERENCE_BUTTON);
			button->EnableWindow(TRUE); //[リファレンス]ボタン有効
			button = (CButton *)GetDlgItem(IDC_VACUUM_BUTTON);
			button->EnableWindow(TRUE); //[バキューム]ボタン有効
			button = (CButton *)GetDlgItem(IDC_MANUAL_STAGE_BUTTON);
			button->EnableWindow(TRUE); //[マニュアル・ステージ]ボタン有効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		//IDD_MAIN_MENU_FORM内コントロールの無効化
		button = (CButton *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON);
		button->EnableWindow(TRUE); //[MEASUREMENT]ボタン有効
		button = (CButton *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON);
		button->EnableWindow(TRUE); //[RECIPE]ボタン有効
		button = (CButton *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON);
		button->EnableWindow(TRUE); //[LOG ON]ボタン有効

		//IDD_VIEW_WINDOW_FORM内コントロールの有効化
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			combobox = (CComboBox*)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO);
			combobox->EnableWindow(TRUE); //レンズ倍率選択有効
			combobox = (CComboBox*)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO);
			combobox->EnableWindow(TRUE); //…有効
			button = (CButton *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON);
			button->EnableWindow(TRUE); //[AUTO FOCUS]ボタン有効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		//メニューバー内(IDR_MEASUREMENT_MENU)の有効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;
		m_pMainFrame->m_MeasMenuEnableFlg2 = TRUE;

		break;

	case HANDLER_LOAD_POSITION_S:
	case SAMPLE_LOAD_S:
	case SAMPLE_UNLOAD_S:
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			button = (CButton *)GetDlgItem(IDC_HANDLER_LOAD_POSITION_BUTTON);
			button->EnableWindow(FALSE); //[HANDLER_LOAD_POSITION]ボタン無効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		button = (CButton *)GetDlgItem(IDC_SAMPLE_LOAD_BUTTON);
		button->EnableWindow(FALSE); //[SAMPLE_LOAD]ボタン無効
		button = (CButton *)GetDlgItem(IDC_SAMPLE_UNLOAD_BUTTON);
		button->EnableWindow(FALSE); //[SAMPLE_UNLOAD]ボタン無効
		break;

	case HANDLER_LOAD_POSITION_E:
	case SAMPLE_LOAD_E:
	case SAMPLE_UNLOAD_E:
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			button = (CButton *)GetDlgItem(IDC_HANDLER_LOAD_POSITION_BUTTON);
			button->EnableWindow(TRUE); //[HANDLER_LOAD_POSITION]ボタン有効
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		button = (CButton *)GetDlgItem(IDC_SAMPLE_LOAD_BUTTON);
		button->EnableWindow(TRUE); //[SAMPLE_LOAD]ボタン有効
		button = (CButton *)GetDlgItem(IDC_SAMPLE_UNLOAD_BUTTON);
		button->EnableWindow(TRUE); //[SAMPLE_UNLOAD]ボタン有効
		break;
	}

}

// ==========================================================================
//
LRESULT CMeasurementOperationView::OnMoveSetposEnd(WPARAM wparam, LPARAM lparam)
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	int ChiefRet = wparam;
	BOOL bResult = FALSE;
	HWND hWnd;
	BOOL bRet;
//	char* pCaption = "STAGE MOVE";
//	  char* pCaption = "ステージ移動";

	//Saiki 20090128 Change ----->
//	hWnd = ::FindWindow(NULL, EXECUTION_CAPTION);
	LoadStringML(IDS_EXECUTION_CAPTION, strBuffer1, "EXECUTION");
	hWnd = ::FindWindow(NULL, strBuffer1);
	//Saiki 20090128 Change <-----
	if(hWnd != NULL){
		bRet = ::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
	}

	///// チーフの結果 /////
	//Saiki 20090527 Change ----->
	//bResult = pMainFrame->JudgeChiefResult(ChiefRet, pCaption);
	LoadStringML(IDS_TITLE_STAGE_MOVE, strBuffer2, "STAGE MOVE");
	bResult = pMainFrame->JudgeChiefResult(ChiefRet, strBuffer2);
	//Saiki 20090527 Change <-----

#ifndef NO_COMPLETE_MSG	/* deleted 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	if(bResult == TRUE){
// Kojika 20090526 Change
//		MessageBox("Stage Movement Complete.", pCaption, MB_OK);
//		MessageBox("ステージ移動完了", pCaption, MB_OK);
		LoadStringML(IDS_STAGE_MOVE_COMP, strBuffer1, "Stage Movement Complete.");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
	}
#endif					/* deleted 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	ButtonEnableChange(HANDLER_LOAD_POSITION_E);

	return 0L;
}


// ==========================================================================
//
void CMeasurementOperationView::MeasStart(WORD wNumScans)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	RCP_DATA rcp_data;
//	BOOL bMain,bMeas,bStage,bPList;
	int iMeasureNum;				//測定ポイント数
	char szUserId[MAX_PATH +1 ];	//ユーザID
	static char szRecipeName[RECIPE_NAME_LEN + 1];
//	char* pszCaption = "MEASUREMENT START";
	char* pszCaption = "測定開始";
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	int iDispLabelCount;

	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	///// 指定フォルダ存在確認 /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// 測定データ仮フォルダが残っていたら削除 /////
	if(bDirExist == TRUE){
		m_pMainFrame->DeleteDirectory(_T(szDelDir));
	}

	///// 表示ポインター初期化 /////
	m_pDoc->SetShowCompPointList(0);
	m_pDoc->SetShowCompPointGraph(0);

	//2009.10.28 bagus 2point-Distance --{--
	//2点間距離測定の場合には始点、終点があるので2倍に
	if(rcp_data.StageProgInfoHdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.StageProgInfoHdr.wScanType == SCAN_TYPE_SR_DISTANCE){
		wNumScans *= 2;
	}
	//2009.10.28 bagus 2point-Distance --}--

	//2009.12.03 bagus stress --{--
	//リファレンス測定の場合にはiMeasureNum=0として実行する
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		TCHAR l_RefFilePath[_MAX_PATH];
		TCHAR l_tszSampleID[SAMPLE_ID_LEN+1];
		m_pDoc->GetSampleId(l_tszSampleID);
		_stprintf(l_RefFilePath, _T("%s%s.dat"), g_szData_StressRef_Dir, l_tszSampleID);
		if (-1 == ::GetFileAttributes(l_RefFilePath)) {
			/*
				(ファイルが無かったので)リファレンス測定
				リファレンス測定の場合にはLSのみなので膜厚測定は行わない
			*/
			wNumScans = 0;
		}
	}
	//2009.12.03 bagus stress --}--

	///// 測定ポイント数 ///// (マニュアル測定時、テストモード時は初期値を0とする)
	iMeasureNum = /*rcp_data.StageProgInfoHdr.*/wNumScans;
	m_pDoc->SetMeasurePointCount(iMeasureNum);

	///// ユーザID取得 /////
	strcpy(szUserId, m_pDoc->User_GetName());
	m_pDoc->SetUserId(szUserId);


	///// 単位名取得 /////
	SR_FIGURE_FORMAT SrFigureFormat;
	SE_FIGURE_FORMAT SeFigureFormat;
	COMPEASE_FIGURE_FORMAT CompEASEFigureFormat;
	char szUnit[MAX_PATH];
	int nUnit;

	memset(szUnit, 0, sizeof(szUnit));

	switch ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType ) {
	case HEAD_TYPE_SR:
	default:
	ConfigFile_GetNanoSpecIni(&SrFigureFormat, CONFIG_FILE_SR_FIGURE_FORMAT);
		nUnit = SrFigureFormat.nThicknessUnit;
		break;
	case HEAD_TYPE_SE:
		ConfigFile_GetNanoSpecIni(&SeFigureFormat, CONFIG_FILE_SE_FIGURE_FORMAT);
		nUnit = SeFigureFormat.nThicknessUnit;
		break;
	case HEAD_TYPE_COMPEASE:
		ConfigFile_GetNanoSpecIni(&CompEASEFigureFormat, CONFIG_FILE_COMPEASE_FIGURE_FORMAT);
		nUnit = CompEASEFigureFormat.nThicknessUnit;
		break;
	}

	if(nUnit >= 0 && nUnit < FIGURE_UNIT_SR_THICKNESS_MAX){
		strcpy(szUnit, g_lpszFigureUnitSrThickness[nUnit]);
	}
	else{
		strcpy(szUnit, " ");
	}

	m_pDoc->SetUnitName(szUnit);
	m_pDoc->SetUnitIndex(nUnit);



	///// 画面表示(データ保存)用タイトル /////
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];		//画面表示(データ保存)用タイトル ←X,Y,Z,AFは含まず
	memset(szDispLabel, 0, sizeof(szDispLabel)); //
	//2009.09.24 bagus se --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SE){
		m_pMainFrame->GetStrategyhead(rcp_data.MeasProgInfo.ScanParams._SE.szStrategyEntry, szDispLabel, &iDispLabelCount);
	//2009.11.04 bagus CA --{--
	}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
		iDispLabelCount = 0;
		//接触角（必ずTRUEになっているはず）
		if(rcp_data.MeasProgInfo.ScanParams._CA.bContactAngle){
			strncpy(szDispLabel[iDispLabelCount],MEAS_LIST_CA_ITEM[0],ADAPRESULTSTRINGLENMAX);
			iDispLabelCount++;
		}
		//半径
		if(rcp_data.MeasProgInfo.ScanParams._CA.bRadius){
			strncpy(szDispLabel[iDispLabelCount],MEAS_LIST_CA_ITEM[1],ADAPRESULTSTRINGLENMAX);
			iDispLabelCount++;
		}
		//液体体積
		if(rcp_data.MeasProgInfo.ScanParams._CA.bLiquidVolume){
			strncpy(szDispLabel[iDispLabelCount],MEAS_LIST_CA_ITEM[2],ADAPRESULTSTRINGLENMAX);
			iDispLabelCount++;
		}

	//2009.11.04 bagus CA --}--
	//2009.11.04 bagus RS --{--
	}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP){
		iDispLabelCount = 4;
		strncpy(szDispLabel[0],MEAS_LIST_RS_ITEM[0],ADAPRESULTSTRINGLENMAX);
		strncpy(szDispLabel[1],MEAS_LIST_RS_ITEM[1],ADAPRESULTSTRINGLENMAX);
		strncpy(szDispLabel[2],MEAS_LIST_RS_ITEM[2],ADAPRESULTSTRINGLENMAX);
		strncpy(szDispLabel[3],MEAS_LIST_RS_ITEM[3],ADAPRESULTSTRINGLENMAX);
	//2009.11.04 bagus RS --}--
	//2009.11.07 bagus MS --{--
	//顕微鏡測定には測定項目はない
	}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		iDispLabelCount = 0;
	//2009.11.07 bagus MS --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE){
		// １ポイント目の測定結果取得タイミングで動的にラベル作成する
		iDispLabelCount = 0;
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	}else{
		//2009.10.28 bagus 2point-Distance --{--
		//2点間距離測定の場合には始点、終点があるので2倍に
		if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			//2点間距離の場合にはストラテジーから取得しない
			iDispLabelCount = 1;
			strncpy(szDispLabel[0],"Distance",ADAPRESULTSTRINGLENMAX);
			//2009.11.26 bagus 2point-distance --{--
			strncpy(szDispLabel[1],"dx[mm]",ADAPRESULTSTRINGLENMAX);
			strncpy(szDispLabel[2],"dy[mm]",ADAPRESULTSTRINGLENMAX);
			//2009.11.26 bagus 2point-distance --}--
		}else{
			//通常測定
		m_pMainFrame->GetStrategyhead(rcp_data.MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry, szDispLabel, &iDispLabelCount);
		}
		//2009.10.28 bagus 2point-Distance --}--
	}
	//2009.09.24 bagus se --}--
	m_pDoc->SetDispLabel(szDispLabel);
	m_pDoc->SetDispLabelCount(iDispLabelCount);


}


//メニューボタンからの関数コール
LRESULT CMeasurementOperationView::OnMeaOpeFuncCall(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
		case WM_DISPMENU_MEASUREMENT_START:
			OnStartButton();
			break;
		case WM_DISPMENU_MANUAL_MEASUREMENT:
			OnManualMeasurementButton();
			break;
		case WM_DISPMENU_HANDLER_LOAD_POSITION:
			OnHandlerLoadPositionButton();
			break;
		case WM_DISPMENU_MANUAL_STAGE:
			OnManualStageButton();
			break;
		case WM_DISPMENU_VACUUM:
			OnVacuumButton();
			break;
		case WM_DISPMENU_REFERENCE:
			OnReferenceButton();
			break;
	}
	return 0L;
}


void CMeasurementOperationView::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	switch(nIDEvent)
	{
		case MEAOPE_BUTTON_ENABLE_CHANGE_TIMER:

			static BOOL sbHostMode = m_pDoc->GetHostMode();
			static BOOL sbHostModeOld = sbHostMode; //前回値

			sbHostMode = m_pDoc->GetHostMode();

			if(sbHostMode != sbHostModeOld)
			{
				if(sbHostMode ==HOST_REMOTE) //リモート時
				{
					ButtonEnableChange(MEAS_OPE_ALL_DISABLE);
				}
				else //ローカル時
				{
					ButtonEnableChange(MEAS_OPE_ALL_ENABLE);
				}
			}
			sbHostModeOld = sbHostMode; //前回値を更新

			break;

		case MEAOPE_MAINTE_WATCH_TIMER:

			if(m_pDoc->GetProcessStatus() == PROCESS_INIT) return;

			static BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
			static BOOL sbNexMntSwOld = TRUE;
			static BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();
			static BOOL sbNexEngiMntSwOld = FALSE;

			sbNexMntSw = nexioIsMaintenanceSwitch();
			sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

			if((sbNexMntSw != sbNexMntSwOld)||(sbNexEngiMntSw != sbNexEngiMntSwOld))
			{
				Invalidate(TRUE); //OnCtlColor()の処理をさせる為にCall
			}
			sbNexMntSwOld = sbNexMntSw; //前回値書換え
			sbNexEngiMntSwOld = sbNexEngiMntSw; //前回値書換え

			break;
	}




	CNanoUI::OnTimer(nIDEvent);
}


/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CMeasurementOperationView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

//	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if(m_pDoc->GetProcessStatus() == PROCESS_INIT) return hbr;

	BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if( pWnd == this ) // 自分自身を描画するときは
	{
		if((sbNexMntSw == OFF)||(sbNexEngiMntSw == ON))
		{
			hbr = ::CreateSolidBrush( RGB( 255, 0, 0 ) );
		}
	}

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}
