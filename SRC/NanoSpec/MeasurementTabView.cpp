// MeasurementTabView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MeasurementListDlg.h"
#include "MeasurementGraphDlg.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MeasurementTabView.h"
#include "NanoSpecDoc.h"
#include "SaveNameOverlapDlg.h"
#include "shlwapi.h"
#include "ManualMeasurementFormView.h"
#include "..\\..\\INC\\NSStage.hxx"
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
#include <Dlgs.h>
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
//2009.12.23 bagus --{--
#include "MeasurementProcessView.h"
//2009.12.23 bagus --}--
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
#include "ManuReMeasReCalcStatDlg.h"
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- } ---------- */


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2009.10.23 bagus Stress 修正 --{--
#define	COLUMN_LENGTH		(12)
// 2009.10.23 bagus Stress 修正 --}--

// #########################################################################
// CMeasurementTabView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CMeasurementTabView, CNanoUI)

// =========================================================================
//
CMeasurementTabView::CMeasurementTabView()
	: CNanoUI(CMeasurementTabView::IDD)
{
	//{{AFX_DATA_INIT(CMeasurementTabView)
	//}}AFX_DATA_INIT
	m_pMeasurementListDlg = NULL;
	m_pMeasurementGraphDlg = NULL;

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;

}

// =========================================================================
//
CMeasurementTabView::~CMeasurementTabView()
{


	if ( m_pMeasurementListDlg != NULL ) {
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pMeasurementListDlg->DestroyWindow();
		delete m_pMeasurementListDlg;
		m_pMeasurementListDlg = NULL;
	}

	if ( m_pMeasurementGraphDlg != NULL ) {
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pMeasurementGraphDlg->DestroyWindow();
		delete m_pMeasurementGraphDlg;
		m_pMeasurementGraphDlg = NULL;
	}
}

// =========================================================================
//
void CMeasurementTabView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasurementTabView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MEASUREMENT_TAB, m_MeasurementTab);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CMeasurementTabView, CNanoUI)
	//{{AFX_MSG_MAP(CMeasurementTabView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MEASUREMENT_TAB, OnSelchangeMeasurementTab)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_RESULT_DATA, OnUpdateSaveResultData)
	ON_COMMAND(IDM_SAVE_RESULT_DATA, OnSaveResultData)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_SAVE_AS_RESULT_DATA, OnSaveAs)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_AS_RESULT_DATA, OnUpdateSaveAs)
	//}}AFX_MSG_MAP

//	ON_MESSAGE(WM_MEAS_COMP_EVENT, SetMeasurementData)
	ON_MESSAGE(WM_MEAS_START_EVENT, MeasurementStart)
	ON_MESSAGE(WM_MEAS_ERROR_EVENT, MeasurementPointError)
	ON_MESSAGE(WM_MANUAL_START_EVENT, ManualMeasurementStart)

END_MESSAGE_MAP()

// =========================================================================
// CMeasurementTabView メッセージ ハンドラ

// =========================================================================
//
void CMeasurementTabView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

// 2014.01.17 Bagus Add (Stage None対応) -->
	//システムコンフィグ設定読み込み
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2014.01.17 Bagus Add (Stage None対応) <--

	///// Edit Tool Bar /////
	if(!m_wndMeasurementBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndMeasurementBar.LoadToolBar(IDR_MEASUREMENT_BAR)){
		TRACE0("Failed to create toolbar\n");
		return;		// 作成に失敗
	}

	RECT rect;
	GetClientRect(&rect);
	m_wndMeasurementBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	///// Get Selected Recipe Name /////
	//2009.09.29 bagus se --{-- del
	//m_pMainFrame->GetSelectListName(m_szName);
	//2009.09.29 bagus se --{--

/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
	this->MRCS_Initialize();
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- } ---------- */

	///// Load Data /////
	LoadData();

	///// Tab	/////
	Tab_Init();

/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	RECT l_rectThisView;	this->GetWindowRect(&l_rectThisView);
	RECT l_rectMeasTab;		m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
	if ((g_lModelType == MODEL_T3100) && (l_rectThisView.bottom < l_rectMeasTab.bottom)) {
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
		/* タブの底辺を移動	*/
		m_MeasurementTab.SetWindowPos(	0,
							0, 0,
							l_rectMeasTab.right - l_rectMeasTab.left,
							l_rectThisView.bottom - l_rectMeasTab.top,
							SWP_NOZORDER | SWP_NOMOVE);
	}
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */

	///// List	/////
	int dx = 5;
	int dy = 24;
	m_pMeasurementListDlg = new CMeasurementListDlg();
	m_pMeasurementListDlg->Create(CMeasurementListDlg::IDD, &m_MeasurementTab);
	m_pMeasurementListDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Graph /////
//	m_pMeasurementGraphDlg = new CMeasurementGraphDlg();
	if( m_pDoc->GetDispStatus() == MAIN_MENU_MANUAL_MEASUREMENT ){
		// マニュアル測定
		m_pMeasurementGraphDlg = new CMeasurementGraphDlg(GRAPHDLG_MODE_MANUAL);
	}
	else{
		// 通常測定
		m_pMeasurementGraphDlg = new CMeasurementGraphDlg(GRAPHDLG_MODE_SEQ);
	}
	m_pMeasurementGraphDlg->Create(CMeasurementGraphDlg::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Set Current Selection Item (Default Select Tab) ////
	int Index;
	Index = m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay;
	//2009.10.13 bagus CTA --{--
	//グラフタブのないレシピは念のために強制的にLISTタブをデフォルトに変更する
	if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
	//2009.11.03 bagus MS --{--
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS
	//2009.11.03 bagus MS --}--
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR && m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE)
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE)
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	){
		Index = 0;
	}
	//2009.10.13 bagus CTA --}--
	TabChange(Index);
	m_MeasurementTab.SetCurSel(Index);


	m_bToolbarSaveCompFlg = TRUE; //測定が行われなかった場合を考慮し、初期値はTRUE(保存済)とする。
	m_bExitSaveCompFlg = TRUE; //測定が行われなかった場合を考慮し、初期値はTRUE(保存済)とする。
	m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
	m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

	m_iProcessStatusOld = -1;
	m_iHostModeOld = -1;
	m_iManuMeasModeOld = -1;

	SetTimer(TABVIEW_ENABLE_CHANGE_TIMER, 50, NULL);
	if(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE)  //通常測定・リメジャー
	{
		ButtonEnableChange(MEAS_TAB_VIEW_CREATE);
	}
	m_pMainFrame->m_pMeaTabViewObj = this;

	::SetEvent(m_pMainFrame->m_hEvTabView);


	//2009.09.29 bagus se --{--
	//2009.11.30 bagus 修正 --{--
	//SRでなくとも手動測定の場合にはここでProcessingに移行する
	//if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR){
	//2009.11.30 bagus 修正 --}--
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)  //マニュアル測定
		{
			m_pDoc->SetProcessStatus(PROCESS_PROC);
			//2009.10.30 bagus 2point-distance
			//2点間距離の場合にはリファレンスを使用しないので取得しない
			if(
			//2009.11.30 bagus 修正 --{--
			//m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_DISTANCE
			(m_MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR &&
				m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_DISTANCE)
			//2009.11.30 bagus 修正 --}--
			//2009.11.03 bagus MS --{--
			&& m_MainRcpInfo.MainRcpParam.hdr.wHeadType != HEAD_TYPE_MS
			//2009.11.03 bagus MS --}--
			//2010.01.15 bagus GTR --{--
			//ガントリ透過率の場合にはリファレンスをこのタイミングで処理しない
			&& (m_MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR &&
				m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
			//2010.01.15 bagus GTR --}--
			){
				m_pMainFrame->m_pManuMeaFormViewObj->GetReferenceManuStart();
			}else{
				//2009.12.29 bagus 修正 --{--
				//手動測定は元々GetReferenceManuStart()で測定開始日時をセットするが
				//ここに来る場合にはリファレンスを取得しないのでここでセットする。
				///// 測定開始日時取得 /////
				SYSTEMTIME syStartDateTime;
				GetLocalTime(&syStartDateTime);
				m_pDoc->SetStartDateTime(&syStartDateTime);
				//2009.12.29 bagus 修正 --}--
			}
			//2点間距離の場合にはリファレンスを使用しないので取得しない
		}
	//}
	//2009.09.29 bagus se --}--

}

// =========================================================================
//
void CMeasurementTabView::OnSelchangeMeasurementTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index = m_MeasurementTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

// =========================================================================
//
void CMeasurementTabView::Tab_Init()
{
//	m_MeasurementTab.InsertItem(0, "List");
//	m_MeasurementTab.InsertItem(1, "Graph");
	//Saiki 20090529 Change ----->
	//m_MeasurementTab.InsertItem(0, "リスト");
	//m_MeasurementTab.InsertItem(1, "グラフ");
	CString strBuffer;
	LoadStringML(IDS_TAB_LIST, strBuffer, "List");
	m_MeasurementTab.InsertItem(0, strBuffer);
	//2009.10.13 bagus CTA --{--
	//グラフタブのないレシピに対応
	if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
	//2009.11.03 bagus MS --{--
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS
	//2009.11.03 bagus MS --}--
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR && m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE)
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE)
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	){
	}else{
		LoadStringML(IDS_TAB_GRAPH, strBuffer, "Graph");
		m_MeasurementTab.InsertItem(1, strBuffer);
	}
	//2009.10.13 bagus CTA --}--
	//Saiki 20090529 Change <-----
}

// =========================================================================
//
void CMeasurementTabView::TabChange(int Index)
{
	m_MeasurementTab.HighlightItem(0, FALSE);
	m_MeasurementTab.HighlightItem(1, FALSE);
	m_MeasurementTab.HighlightItem(Index, TRUE);

	///// Hide All Dialog /////
	m_pMeasurementListDlg->ShowWindow(SW_HIDE);
	m_pMeasurementGraphDlg->ShowWindow(SW_HIDE);

	///// Show Dialog /////
	switch(Index){
	case 0:
		m_pMeasurementListDlg->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pMeasurementGraphDlg->ShowWindow(SW_SHOW);
		break;
	}
}

// =========================================================================
//
void CMeasurementTabView::LoadData()
{
	//2009.09.29 bagus se --{--
	RCP_DATA rcp_data;

	m_pDoc->GetRcpData(&rcp_data);
	m_MainRcpInfo = rcp_data.MainRcpInfo;
	m_MeasProgInfo = rcp_data.MeasProgInfo;
	m_StageProgInfoHdr = rcp_data.StageProgInfoHdr;
	memcpy(m_ScanPoint,rcp_data.ScanPoint,sizeof(m_ScanPoint));
	strcpy(m_szName,m_MainRcpInfo.hdr.szName);
	//RecipeFile_LoadRecipe(&m_MainRcpInfo, m_szName, RECIPE_FILE_MAIN_RECIPE);
	//RecipeFile_LoadRecipe(&m_MeasProgInfo, m_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
	//RecipeFile_LoadRecipe(&m_StageProgInfoHdr, m_MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
	//RecipeFile_LoadPointList(m_ScanPoint, m_StageProgInfoHdr.wNumScans, m_MainRcpInfo.MainRcpParam.hdr.szStage);
	//2009.09.29 bagus se --}--

}

// =========================================================================
//
LRESULT CMeasurementTabView::SetMeasurementData(WPARAM wparam, LPARAM lparam)
{
//未使用
//	m_pMeasurementListDlg->PostMessage(WM_MEAS_COMP_EVENT, wparam, lparam);
//	m_pMeasurementGraphDlg->PostMessage(WM_MEAS_COMP_EVENT, wparam, lparam);
	return 0L;
}

// =========================================================================
//
LRESULT CMeasurementTabView::MeasurementStart(WPARAM wparam/*Point Number*/, LPARAM lparam)
{
	m_pMeasurementListDlg->StartMeasurementInit(wparam);
	return 0L;
}

// =========================================================================
//
LRESULT CMeasurementTabView::MeasurementPointError(WPARAM wparam/*Point Number*/, LPARAM lparam)
{
	m_pMeasurementListDlg->MeasurementError(wparam);
	return 0L;
}

// =========================================================================
//
LRESULT CMeasurementTabView::ManualMeasurementStart(WPARAM wparam, LPARAM lparam)
{
	STAGE_COORD pPosition;
	STAGE_COORD_XYZ CurrPos;

	NS_StageGetPos(&CurrPos);

	pPosition.lX = CurrPos.lX;
	pPosition.lY = CurrPos.lY;

	//現在のステージ座標を取得
	//マニュアル時は測定毎に座標をポイントグラフへ追加する必要がある。
	//m_pMeasurementListDlg->DrawPointMarker(pPosition.lX, pPosition.lY, MEASUREMENT_STATES_START);
	return 0L;
}





//===========================================================================================
//===========================================================================================
//===========================================================================================
/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConAddRevi
// Purpose		: 測定条件などの保存(追加・修正)
// 戻り値:	1:成功
//			2:失敗(保存しない)
//			3:失敗(保存キャンセル)
//　　　　 0:失敗
int CMeasurementTabView::SaveMeasConAddRevi(LPCTSTR pszMainRcpName, int iSaveWay)
{
	char szSampleId[SAMPLE_ID_LEN + 1];
	//2009.10.13 bagus RS --{--
	//ロットIDの追加
	char szLotId[LOT_ID_LEN + 1];
	//2009.10.13 bagus RS --}--
	char szSaveFullFilePath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szSaveFullDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szRefFilePath[MAX_PATH];
	char szToFilePath[MAX_PATH];
	char szFullPathFrom[SAVE_FILE_PATH_NAME_LEN+1]; //絶対パスに変換したパス
	char szFullPathTo[SAVE_FILE_PATH_NAME_LEN+1];//絶対パスに変換したパス
	char szCopyFrom[SAVE_FILE_PATH_NAME_LEN+1];
	char szCopyTo[SAVE_FILE_PATH_NAME_LEN+1];
	static char szMoveFrom[SAVE_FILE_PATH_NAME_LEN+1];
	static char szMoveTo[SAVE_FILE_PATH_NAME_LEN+1];
	char szDeleteDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDeleteFilePath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDirName[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDirName2[SAVE_FILE_PATH_NAME_LEN + 1]; //ディレクトリ名(ファイル名から拡張子を除いたもの)
	char szReNameDir[SAVE_FILE_PATH_NAME_LEN + 1];
	char szMessage[255+1];
	RCP_DATA rcp_data;
	BOOL bSampleID;
	BOOL bSaveMeasResult;
	BOOL bSaveOverwrite;
	BOOL bFileNameExistFlg = FALSE; //保存しようとしたファイル名が存在するかのフラグ(TRUE:存在、FALSE:なし)
	int iRtn=0; //メッセージボックス戻り値用
	//2009.10.17 bagus stress --{--
	char szCheckLetters[] = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//2009.10.17 bagus stress --{--

	memset(szSampleId, 0, sizeof(szSampleId));
	//2009.10.13 bagus RS --{--
	//ロットIDの追加
	memset(szLotId, 0, sizeof(szLotId));
	//2009.10.13 bagus RS --}--
	memset(szSaveFullFilePath, 0, sizeof(szSaveFullFilePath));
	memset(szSaveFullDirPath, 0, sizeof(szSaveFullDirPath));
	memset(szRefFilePath, 0, sizeof(szRefFilePath));
	memset(szToFilePath, 0, sizeof(szToFilePath));
	memset(szFullPathFrom, 0, sizeof(szFullPathFrom));
	memset(szFullPathTo, 0, sizeof(szFullPathTo));
	memset(szCopyFrom, 0, sizeof(szCopyFrom));
	memset(szCopyTo, 0, sizeof(szCopyTo));
	memset(szMoveTo, 0, sizeof(szMoveTo));
	memset(szMoveTo, 0, sizeof(szMoveTo));
	memset(szDeleteDirPath, 0, sizeof(szDeleteDirPath));
	memset(szDeleteFilePath, 0, sizeof(szDeleteFilePath));
	memset(szDirName, 0, sizeof(szDirName));
	memset(szDirName2, 0, sizeof(szDirName2));
	memset(szReNameDir, 0, sizeof(szReNameDir));
	memset(szMessage, 0, sizeof(szMessage));
	memset(&rcp_data, 0, sizeof(rcp_data));

	m_pDoc->GetSampleId(szSampleId); //サンプルIDの取得
	//2009.10.13 bagus RS --{--
	//ロットIDの追加
	m_pDoc->GetLotId(szLotId); 	//ロットIDの取得
	//2009.10.13 bagus RS --}--
	m_pDoc->GetRcpData(&rcp_data);

	bSampleID = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSampleID; //サンプルIDを使用？
	bSaveMeasResult = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult; //セーブファイル名にサンプルIDを使用?(bSampleID==TRUE時のみ有効)
	bSaveOverwrite = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite; //セーブファイルが同じファイル名だった場合、上書き可？
	//2009.12.17 bagus 保存ファイル名の変更 --{--
	BOOL bSaveNameWithDate = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate; //ファイルに日時を追加する
	//2009.12.17 bagus 保存ファイル名の変更 --}--
	//2009.12.17 bagus 保存ファイル名の変更 --{--
	if(m_pDoc->GetHostMode() == HOST_REMOTE){
		//リモート時は常に以下のオプションを有効にする
		bSampleID = TRUE;
		bSaveMeasResult = TRUE;
		bSaveOverwrite = TRUE;
		bSaveNameWithDate = TRUE;	//但しストレス測定の場合には使用としてTRUEであっても日時は付かない
	}
	//2009.12.17 bagus 保存ファイル名の変更 --}--


	//2009.08.28 bagus stress --{--
	//次に保存すべき連番の検索
	//zまで行ったらZを上書き
	int nNextSaveNo = strlen(szCheckLetters)-1;
	//2009.12.21 bagus 修正 --}--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		if(!m_pDoc->IsStressRefereneMeasuremnt()){
			char szCheckDir[MAX_PATH+1];
			//2009.10.17 bagus stress --{--
			//for(int nCheckNo = 1;nCheckNo < INT_MAX;nCheckNo++){
			//	sprintf(szCheckDir,"%s\\%s\\%s_%d%s",g_szResult_Dir,szSampleId,pszMainRcpName,nCheckNo,DAT_EXT);
//2009.12.21 bagus 修正 --{--
//			for(int nCheckNo = 0;nCheckNo < strlen(szCheckDir);nCheckNo++){
			for(int nCheckNo = 0;nCheckNo < strlen(szCheckLetters);nCheckNo++){
//2009.12.21 bagus 修正 --}--
				sprintf(szCheckDir,"%s\\%s\\%s_%c%s",g_szResult_Dir,szSampleId,pszMainRcpName,szCheckLetters[nCheckNo],DAT_EXT);
			//2009.10.17 bagus stress --}--
				if(!FileOrDirExists(szCheckDir)){
					nNextSaveNo = nCheckNo;
					break;
				}
			}
		}else{
			//2009.09.17 bagus stress --{--
			//pszMainRcpName = STRESS_REFERENCE_NAME;
			//2009.09.17 bagus stress --}--
		}
		if(iSaveWay == MEASUREMENT_TOOLBAR_SAVE
		|| iSaveWay == MEASUREMENT_TOOLBAR_SAVE_AS){
			iSaveWay = MEASUREMENT_EXIT_SAVE;
		}
		//ストレスの場合はファイル名を聞く必要がない
		bSaveMeasResult = TRUE;
		//2009.09.15 bagus stress --{--
		//ストレスのリファレンスは常に上書き
		if(m_pDoc->IsStressRefereneMeasuremnt()){
			bSaveOverwrite = TRUE;
		}
		//2009.09.15 bagus stress --}--
	}
	//2009.08.28 bagus stress --}--

//=========================================================
//[保存先パス取得]==========================================
	switch(iSaveWay)
	{
		case MEASUREMENT_EXIT_SAVE:

			if((m_pDoc->GetHostMode() != HOST_LOCAL)&&(bSaveOverwrite == FALSE)) //リモート かつ 上書き確認が必要な場合
			{
				//2009.08.28 bagus stress --{--
				//ストレスのディレクトリ構成に対応
				//リモート時はファイル重複を先行判別
				if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
					if(m_pDoc->IsStressRefereneMeasuremnt()){
						sprintf(szSaveFullFilePath, "%s%s\\%s%s", g_szResult_Dir , szSampleId ,STRESS_REFERENCE_NAME, DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s\\%s", g_szResult_Dir , szSampleId,STRESS_REFERENCE_NAME  );
					}else{
						//2009.10.17 bagus stress --{--
						//sprintf(szSaveFullFilePath, "%s%s\\%s_%d%s", g_szResult_Dir , szSampleId ,pszMainRcpName, nNextSaveNo, DAT_EXT); //
						//sprintf(szSaveFullDirPath, "%s%s\\%s_%d", g_szResult_Dir , szSampleId,pszMainRcpName, nNextSaveNo );
						sprintf(szSaveFullFilePath, "%s%s\\%s_%c%s", g_szResult_Dir , szSampleId ,pszMainRcpName, szCheckLetters[nNextSaveNo], DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s\\%s_%c", g_szResult_Dir , szSampleId,pszMainRcpName, szCheckLetters[nNextSaveNo] );
						//2009.10.17 bagus stress --{--
					}
				}else{
					//2009.12.17 bagus 保存ファイル名の変更 --{--
					if(!bSaveNameWithDate){
						//メインレシピの設定により保存時のディレクトリと結果ファイルに測定開始日時を付け加える
						sprintf(szSaveFullFilePath, "%s%s%s", g_szResult_Dir , szSampleId , DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s", g_szResult_Dir , szSampleId);
					}else{
						//YYYYMMDD_hhmmss => 15
						char szNameWithDate[15+1];
						SYSTEMTIME syStartDateTime;
						m_pDoc->GetStartDateTime(&syStartDateTime);
						memset(szNameWithDate,NULL,sizeof(szNameWithDate));
						_snprintf(szNameWithDate,15,"%04d%02d%02d_%02d%02d%02d",
								syStartDateTime.wYear,
								syStartDateTime.wMonth,
								syStartDateTime.wDay,
								syStartDateTime.wHour,
								syStartDateTime.wMinute,
								syStartDateTime.wSecond
							);
						sprintf(szSaveFullFilePath, "%s%s_%s%s", g_szResult_Dir , szSampleId , szNameWithDate , DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s_%s", g_szResult_Dir , szSampleId , szNameWithDate);
					}
					//2009.12.17 bagus 保存ファイル名の変更 --}--
				}
				//2009.08.28 bagus stress --}--
				if(( FileOrDirExists(szSaveFullFilePath)==TRUE ) //ファイル重複有り
				||( FileOrDirExists(szSaveFullDirPath)==TRUE )) //ディレクトリ重複有り
				{
					AlarmIf_Set(ALID_MeasurementResultNameDuplication); //測定結果保存名重複(リモート時)

					int iRtnOverlap = SaveOverlap(szSaveFullFilePath);
					if(iRtnOverlap != -1) //-1は処理続行
					{
//						return iRtnOverlap;
						return 2; //リモート時は測定終了後に測定画面にとどまるとマズイので、測定キャンセルでも保存せずにレシピ選択画面に抜ける。
					}
				}
			}
// 2009.12.17 bagus Stress --{--
#if 0
			else if(((bSampleID == TRUE)&&(bSaveMeasResult == TRUE))
					||((m_pDoc->GetHostMode() != HOST_LOCAL)&&(bSaveOverwrite == TRUE)))
#else
			else if(((bSampleID == TRUE)&&(bSaveMeasResult == TRUE))
					||((m_pDoc->GetHostMode() != HOST_LOCAL)))
#endif
// 2009.12.17 bagus Stress --}--
			{
				//2009.08.28 bagus stress --{--
				//sprintf(szSaveFullFilePath, "%s%s%s", g_szResult_Dir , szSampleId , DAT_EXT); //
				if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
					if(m_pDoc->IsStressRefereneMeasuremnt()){
						sprintf(szSaveFullFilePath, "%s%s\\%s%s", g_szResult_Dir , szSampleId ,STRESS_REFERENCE_NAME, DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s\\%s", g_szResult_Dir , szSampleId,STRESS_REFERENCE_NAME  );
					}else{
						//2009.10.17 bagus stress --{--
						//sprintf(szSaveFullFilePath, "%s%s\\%s_%d%s", g_szResult_Dir , szSampleId ,pszMainRcpName, nNextSaveNo, DAT_EXT); //
						//sprintf(szSaveFullDirPath, "%s%s\\%s_%d", g_szResult_Dir , szSampleId,pszMainRcpName, nNextSaveNo );
						sprintf(szSaveFullFilePath, "%s%s\\%s_%c%s", g_szResult_Dir , szSampleId ,pszMainRcpName, szCheckLetters[nNextSaveNo], DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s\\%s_%c", g_szResult_Dir , szSampleId,pszMainRcpName, szCheckLetters[nNextSaveNo] );
						//2009.10.17 bagus stress --{--
					}
				}else{
					//2009.12.17 bagus 保存ファイル名の変更 --{--
					if(!bSaveNameWithDate){
						//メインレシピの設定により保存時のディレクトリと結果ファイルに測定開始日時を付け加える
						sprintf(szSaveFullFilePath, "%s%s%s", g_szResult_Dir , szSampleId , DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s", g_szResult_Dir , szSampleId);
					}else{
						//YYYYMMDD_hhmmss => 15
						char szNameWithDate[15+1];
						SYSTEMTIME syStartDateTime;
						m_pDoc->GetStartDateTime(&syStartDateTime);
						memset(szNameWithDate,NULL,sizeof(szNameWithDate));
						_snprintf(szNameWithDate,15,"%04d%02d%02d_%02d%02d%02d",
								syStartDateTime.wYear,
								syStartDateTime.wMonth,
								syStartDateTime.wDay,
								syStartDateTime.wHour,
								syStartDateTime.wMinute,
								syStartDateTime.wSecond
							);
						sprintf(szSaveFullFilePath, "%s%s_%s%s", g_szResult_Dir , szSampleId , szNameWithDate , DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s_%s", g_szResult_Dir , szSampleId , szNameWithDate);
					}
					//2009.12.17 bagus 保存ファイル名の変更 --}--
				}
				//2009.08.28 bagus stress --}--
			}
			else
			{
				if(!GetMeasSaveFileName(szSaveFullFilePath))
				{
					return 3;

				}
			}
			break;

		case MEASUREMENT_TOOLBAR_SAVE:
			if(!GetMeasSaveFileName(szSaveFullFilePath))
			{
				return 3;

			}
			break;
		//Saiki 20090401 Add ----->
		case MEASUREMENT_TOOLBAR_SAVE_AS:
			if(bSaveOverwrite)
				bSaveOverwrite = FALSE;
			if(!GetMeasSaveFileName(szSaveFullFilePath))
			{
				return 3;

			}
			break;
		//Saiki 20090401 Add <-----
		default:
			//処理なし
			break;
	}


//=========================================================================
//[保存先ファイル名が重複していた場合、条件により対象ファイル(フォルダ)を削除]==
	//保存先ファイル名が既に存在していた場合の処理
	memset(szSaveFullDirPath, 0, sizeof(szSaveFullDirPath));
	_tsplitpath(szSaveFullFilePath, NULL, szDirName, szDirName2, NULL);
	sprintf(szSaveFullDirPath, "%s%s", szDirName, szDirName2); //
	if(( FileOrDirExists(szSaveFullFilePath)==TRUE ) //ファイル重複有り
	||( FileOrDirExists(szSaveFullDirPath)==TRUE )) //ディレクトリ重複有り
	{
		bFileNameExistFlg = TRUE; //保存予定ファイル名重複
		sprintf(szDeleteFilePath, "%s", szSaveFullFilePath); //削除対象ファイルパス名を作成
		sprintf(szDeleteDirPath, "%s", szSaveFullDirPath); //削除対象フォルダパス名を作成
	}
	else
	{
		//重複ナシ
		bFileNameExistFlg = FALSE; //
	}


	//保存ファイルが重複していた場合の上書き(正確には削除+移動)の下準備
	if(bFileNameExistFlg == TRUE) //保存名に重複あり
	{
/* modified 2016.08.31 hmenjo 結果データ勝手に削除 ---------- { ---------- */
//		if((m_pDoc->GetHostMode() == HOST_LOCAL)
//		&&(bSampleID == TRUE)&&(bSaveMeasResult == TRUE)&&(bSaveOverwrite == FALSE))
//		{
//			int iRtnOverlap = SaveOverlap(szSaveFullFilePath);
//			if(iRtnOverlap != -1) //-1は処理続行
//			{
//				return iRtnOverlap;
//			}
//		}
//
//		//上書きの準備として、対象ファイル(フォルダを削除)
//		remove(szDeleteFilePath); //対象ファイル削除
//		//★この時点では、重複はファイル名のみで判断しているので、ディレクトリ削除は直前で存在を確認する必要がある。
//		if(FileOrDirExists(szDeleteDirPath)==TRUE)
//		{
//			m_pMainFrame->DeleteDirectory(_T(szDeleteDirPath)); //対象ディレクトリを再帰的に削除
//		}
/* modified 2016.08.31 hmenjo 結果データ勝手に削除 ----------              */
		int l_iRtnOverlap = 0;
		if (
			(m_pDoc->GetHostMode() == HOST_LOCAL) &&	/* ローカルであること	*/
			(bSampleID == TRUE) &&						/* メインレシピで「サンプルIDを使用」がオン	*/
			(bSaveMeasResult == TRUE) &&				/* メインレシピで「サンプルID名で自動保存する」がオン	*/
			(bSaveOverwrite == FALSE)					/* メインレシピで「上書き保存する」がオフ	*/
		) {
			/* 上書き確認ダイアログ	*/
			l_iRtnOverlap = SaveOverlap(szSaveFullFilePath);
			if (-1 == l_iRtnOverlap) {
				/* 上書き保存(SAVE)で処理続行	*/
				/*	上書き前の削除処理は次の if (-1 == l_iRtnOverlap) ブロックです．	*/
			} else if (-2 == l_iRtnOverlap) {
				/* 名前を付けて保存(SAVE AS)で処理続行	*/
				/*	szSaveFullFilePath で指定された結果データが存在する場合がありますので，
					存在する場合は削除します．
					この場合，szSaveFullFilePath から対象ディレクトリを生成します．
					対象が結果データ用のディレクトリですので，他のフォーマットの
					データの存在は仕様上ありえないからです．	*/
				if (0xffffffff != ::GetFileAttributes(szSaveFullFilePath)) {
					/* 存在したので削除しておきます	*/
					remove(szSaveFullFilePath);
					char l_szDeleteDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
					strcpy(l_szDeleteDirPath, szSaveFullFilePath);
					if (0 != strrchr(l_szDeleteDirPath, _T('.'))) {
						*(strrchr(l_szDeleteDirPath, _T('.'))) = 0x00;
						if (FALSE != FileOrDirExists(l_szDeleteDirPath)) {
							/* 対象ディレクトリを再帰的に削除	*/
							m_pMainFrame->DeleteDirectory(_T(l_szDeleteDirPath));
						}
					}
				}
			} else {
				/* 保存しない(CANCEL)	*/
				return l_iRtnOverlap;
			}
		} else {
			l_iRtnOverlap = -1;
		}
		if (-1 == l_iRtnOverlap)  {
			/* 上書きの準備として，対象ファイル(フォルダを削除)	*/
			remove(szDeleteFilePath); /* 対象ファイル削除	*/
			/*	★この時点では，重複はファイル名のみで判断しているので，
				ディレクトリ削除は直前で存在を確認する必要がある．	*/
			if (FileOrDirExists(szDeleteDirPath) == TRUE) {
				m_pMainFrame->DeleteDirectory(_T(szDeleteDirPath));	/* 対象ディレクトリを再帰的に削除	*/
			}
		}
/* modified 2016.08.31 hmenjo 結果データ勝手に削除 ---------- } ---------- */
	}

//==========================================================
//[仮フォルダに一時保存]=====================================
	//リファレンスファイルコピー
	sprintf(szRefFilePath, "%s%s%s", g_szData_Ref_Dir, /*szRecipeName*/pszMainRcpName, DAT_EXT);
	sprintf(szToFilePath, "%s%s%s", g_szData_Measured_Data_Temp_Dir, pszMainRcpName, REFERENCE_EXT); //hibino [仮]測定結果保存時にリファレンスファイル名の拡張子を.refに変更(コピー元は.datのまま)
	CopyFile(szRefFilePath, szToFilePath, FALSE); 						// 既存ファイルがあっても、上書きする

	//2009.09.01 bagus stress --{--
	//ストレスのリファレンスがあった場合にそれをコピーする
	//StressRefRaw.dat -> ref\StressRef\SampleID.dat
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		sprintf(szRefFilePath, "%s%s%s",g_szData_Measured_Data_Temp_Dir ,_T("StressRefRawData"), DAT_EXT);
		sprintf(szToFilePath, "%s%s%s", g_szData_StressRef_Dir,szSampleId , DAT_EXT);
		CopyFile(szRefFilePath, szToFilePath, FALSE);
	}

	//2009.09.01 bagus stress --}--

	//レシピデータをファイルに保存(測定前に分かるもののみ)
	if(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) //マニュアル測定以外
	{
		RecipeFile_SaveCurrRecipeInfo(pszMainRcpName);
	}
	else //マニュアル測定
	{
		RecipeFile_SaveCurrRecipeInfo(pszMainRcpName, 1);
	}

	//データ書換え
	SaveMeasConRevi(pszMainRcpName);

	//データ追記
	SaveMeasConAdd(pszMainRcpName); //測定データなど追記

//==========================================================
//[ファイルrename＆move(正規の場所へ格納)]====================
	//ファイルrename＆move(正規の場所へ格納)

	_tsplitpath(szSaveFullFilePath, NULL, szDirName, szReNameDir, NULL); //パスを分解


	//ファイルmove&rename
	sprintf(szMoveFrom, "%s%s%s", g_szData_Measured_Data_Temp_Dir, pszMainRcpName, DAT_EXT);
	sprintf(szMoveTo, "%s", szSaveFullFilePath);
	MoveFile(szMoveFrom, szMoveTo ); //Move(Copyではない)

	//2010.01.13 bagus 2point --{--
	//2点間距離の場合にはフォルダを作る必要がないのでここでReturn
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		return 1;
	}
	//2010.01.13 bagus 2point --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE){
		return 1;
	}
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--

	//※RecipeFile_SaveCurrRecipeInfo()を2回実行すると[Point]が追記されてしまうので、先にmoveが必要
	//※フォルダは残しておかないと2回目のセーブの時に困るからcopyにする。
	//フォルダcopy&rename
	sprintf(szCopyFrom, "%s%s", g_szData_Measured_Data_Temp_Dir, "*");
	sprintf(szCopyTo, "%s%s%s", szDirName, szReNameDir, "\\");
	_fullpath(szFullPathFrom, (const char *)szCopyFrom, sizeof(szFullPathFrom)); //絶対パスに変換
	_fullpath(szFullPathTo, (const char *)szCopyTo, sizeof(szFullPathTo)); //絶対パスに変換
	MakeDirectory(szFullPathTo); //受け側ディレクトリ作成
	//2009.09.17 bagus stress --{--
	//ストレスのリファレンスファイルをコピーする
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		if(m_pDoc->IsStressRefereneMeasuremnt()){
			sprintf(szRefFilePath, "%s%s%s",g_szData_Measured_Data_Temp_Dir,pszMainRcpName, DAT_EXT);
			//sprintf(szToFilePath, "%s%s%s", g_szData_Measured_Data_Temp_Dir,STRESS_REFERENCE_NAME, DAT_EXT);
			sprintf(szToFilePath, "%s%s\\%s%s",g_szResult_Dir , szSampleId ,STRESS_REFERENCE_NAME, DAT_EXT);
			MoveFileEx(szRefFilePath, szToFilePath,MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
		}
	}
	//2009.09.17 bagus stress --}--
	int iCpChk = FolderCopy(szFullPathFrom, szFullPathTo); //コピー(Moveではない)


	//最終測定結果保存情報を｢DATA｣フォルダに格納
	// write NanoSpec ini file
	char szLastSaveInfFilePath[_MAX_PATH];
	memset(szLastSaveInfFilePath, 0, sizeof(szLastSaveInfFilePath));
	char szMoveToFullPath[SAVE_FILE_PATH_NAME_LEN+1];
	memset(szMoveToFullPath, 0, sizeof(szMoveToFullPath));
	_fullpath(szMoveToFullPath, (const char *)szMoveTo, sizeof(szMoveToFullPath)); //絶対パスに変換

/* modified 2009.07.07 hmenjo dll 相対パス対応 NanoSpec.exe ---------- { ----------	*/
//	sprintf(szLastSaveInfFilePath, "%s%s%s", DATA_DIR, "LastSaveInf", INF_EXT);
/* modified 2009.07.07 hmenjo dll 相対パス対応 NanoSpec.exe ----------				*/
	_stprintf(szLastSaveInfFilePath, _T("%sLastSaveInf") _T(INF_EXT), g_szData_Dir);
/* modified 2009.07.07 hmenjo dll 相対パス対応 NanoSpec.exe ---------- } ----------	*/
	::WritePrivateProfileString("LASTSAVEINF",
								"MeasuredDataPath",
/* modified 2009.07.07 hmenjo dll 相対パス対応 NanoSpec.exe ---------- { ----------	*/
//								szMoveTo,
/* modified 2009.07.07 hmenjo dll 相対パス対応 NanoSpec.exe ----------				*/
								szMoveToFullPath,
/* modified 2009.07.07 hmenjo dll 相対パス対応 NanoSpec.exe ---------- } ----------	*/
								szLastSaveInfFilePath);


	return (int)((0 == iCpChk)? 1 : 0);
}


/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConRevi
// Purpose		: 書換えが必要な場合使用
// Parameters	: pszMainRcpName  ---> 追記対象ファイル名(パス)
void CMeasurementTabView::SaveMeasConRevi(LPCTSTR pszMainRcpName)
{

//データ書換え
	int iDummyStartWaveLen;
	int iDummyEndWaveLen;
	int iDummyOpticalFilter;
	CString strBuffer;
	CStringArray arrayStringItem;
	int iItemNo[3];
	char szFilePath[MAX_PATH + 1];
	RCP_DATA rcp_data;
	TCHAR szDdeTextItem[1024];
	char *token;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	int iRecalibMax;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

/* added 2015.06.01 hmenjo 反射率 NONE バグ ---------- { ---------- */
	sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);
/* added 2015.06.01 hmenjo 反射率 NONE バグ ---------- } ---------- */
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	if ( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS ) {
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if ( rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_COMPEASE ) {
			iRecalibMax = sizeof(g_lpszCompEASERecalibItem_ENU) / sizeof(g_lpszCompEASERecalibItem_ENU[0]);
			for ( int i = 0; i < iRecalibMax; i++ ) {
				arrayStringItem.Add(g_lpszCompEASERecalibItem_ENU[i]);
			}
		}
		else {
	// ストラテジー毎の測定項目インデックスを追加
		arrayStringItem.Add("None");
		if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, rcp_data.MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry) ){
			token = strtok(szDdeTextItem, _T(","));
			while ( token ) {
				arrayStringItem.Add(token);
				token = strtok(NULL, _T(","));
			}
		}

			iRecalibMax = arrayStringItem.GetSize();
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

		// SRのMainRecipeReviファイルのセクション，キー，デフォルト，変数の設定
		LPCTSTR pszKey[] =	{ "FirstRecalItem", "SecondRecalItem",	"ThirdRecalItem"};
		iItemNo[0] = rcp_data.MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1];
		iItemNo[1] = rcp_data.MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2];
		iItemNo[2] = rcp_data.MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3];

		for ( int i = 0; i < 3; i++ ) {
			if ( iItemNo[i] < iRecalibMax )
				strBuffer = arrayStringItem.GetAt(iItemNo[i]);
			else
				strBuffer.Format("%d ;*****RecalibItem IndexOver", iItemNo[i]);

			// write NanoSpec ini file
/* deleted 2015.06.01 hmenjo 反射率 NONE バグ ---------- { ---------- */
//			sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);
/* deleted 2015.06.01 hmenjo 反射率 NONE バグ ---------- } ---------- */
			::WritePrivateProfileString("MainRecipe",
										pszKey[i],
										strBuffer,
										szFilePath);
		}
	}


//===================================================
	// write NanoSpec ini file
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)  //マニュアル測定
	{
		::WritePrivateProfileString("MainRecipe",
									"StageProgram",
									"None",
									szFilePath);
	}

}

/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConRevi2
// Purpose		: iniファイル書き出し
// Parameters	: pDesc 		  --->
//				  iDescCount	  ---> 書き込みキー数
//				  pszMainRcpName  ---> 追記対象ファイル名(パス)
void CMeasurementTabView::SaveMeasConRevi2(RECIPE_FILE* pDesc, int iDescCount, LPCTSTR pszMainRcpName)
{
	int iIndex/*, iDescCount*/;
	LPCSTR section;
//	char buff[RECIPE_COMMENT_LEN + 1];
	char buff[1024 + 1];
	char szFilePath[_MAX_PATH];


	sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);


	for(iIndex = 0; iIndex < iDescCount; iIndex++){
		// get section name
		if (strlen((pDesc + iIndex)->section) > 0)
			section = (pDesc + iIndex)->section;

		// set file data
		memset(buff, 0, sizeof(buff));
		switch((pDesc + iIndex)->type){
		case 'b':
			strcpy(buff, (*(BOOL*)(pDesc + iIndex)->data)? "TRUE" : "FALSE");
			break;
		case 'B':
			_itoa(*(BYTE*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'S':
			_itoa(*(short*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'I':
			_itoa(*(int*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'L':
			_ltoa(*(long*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'F':
			sprintf(buff, "%.3f", *(float*)(pDesc + iIndex)->data);
			break;
		case 'D':
			sprintf(buff, "%.3lf", *(double*)(pDesc + iIndex)->data);
			break;
		case 'W':
			sprintf(buff, "%d", *(WORD*)(pDesc + iIndex)->data);
			break;
		case 'C':
//			strncpy(buff, (char*)(pDesc + iIndex)->data, RECIPE_COMMENT_LEN);
			strncpy(buff, (char*)(pDesc + iIndex)->data, sizeof(buff));
			break;
		default:
			break;
		}

		// write NanoSpec ini file
		::WritePrivateProfileString(section,
									(pDesc + iIndex)->key,
									buff,
									/*p*/szFilePath);
	}

	return;
}



/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConAdd
// Purpose		: 追記が必要な場合使用(主に測定データ用)
// Parameters	: pDesc				--->
//				  iDescCount		---> 書き込みキー数
//				  pszMainRcpName	---> 追記対象ファイル名(パス)
void CMeasurementTabView::SaveMeasConAdd(LPCTSTR pszMainRcpName)
{

//
	BOOL bCheck = FALSE;

	char szUserId[MAX_PATH + 1];									//ユーザID
	SYSTEMTIME syStartDateTime;										//測定開始日時
	SYSTEMTIME syEndDateTime;										//測定終了日時
	char szSampleId[SAMPLE_ID_LEN + 1];								//	サンプルID
	//2009.10.13 bagus RS --{--
	char szLotId[LOT_ID_LEN + 1];									//	ロットID
	//2009.10.13 bagus RS --}--
	char szUnit[DISP_UNIT_NAME_LEN + 1];							//	測定結果単位名
	int iSetPointCount;												//格納済ポイント数
	char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1] = {0};			//ラベル(X,Y,Z) …未使用(関数の引数に必要なのであるだけ)
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	double dData[ADAPRESULT_COLS_MAX];								//Colデータ(AFよりも後ろのデータ)
	char szFilePath[_MAX_PATH];
	RCP_DATA rcp_data;

// 2009.10.23 bagus Stress 修正 --{--
	int	nColumnLen;
	char szTmp[255+1];
	char szTmp2[255+1];
	int	nKeyLen;
// 2009.10.23 bagus Stress 修正 --}--

/* added 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
	int l_iUnitIndexThickInt = -1;		/* -1 は整数表示化しない	*/
	SR_FIGURE_FORMAT l_SrFigureFormat;
	ConfigFile_GetNanoSpecIni(&l_SrFigureFormat,
					CONFIG_FILE_SR_FIGURE_FORMAT);
	if (0 != l_SrFigureFormat.iThicknessIntOnly) {
		l_iUnitIndexThickInt = 3;		/* -1 以外はインデックス値	*/
	}
/* added 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */

//初期化
	memset(szUserId, 0, sizeof(szUserId));
	memset(&syStartDateTime, 0, sizeof(syStartDateTime));
	memset(&syEndDateTime, 0 , sizeof(syEndDateTime));
	memset(szSampleId, 0, sizeof(szSampleId));
	//2009.10.13 bagus RS --{--
	//ロットIDの追加
	memset(szLotId, 0, sizeof(szLotId));
	//2009.10.13 bagus RS --}--
	memset(szUnit, 0, sizeof(szUnit));
	iSetPointCount = 0;
	memset(szDispLabel, 0, sizeof(szDispLabel));
	memset(dData ,0 ,sizeof(dData));
	memset(szFilePath, 0, sizeof(szFilePath));


//データ収集
	m_pDoc->GetUserId(szUserId);	//ユーザIDの取得
	m_pDoc->GetStartDateTime(&syStartDateTime);	//	測定開始日時の取得
	m_pDoc->GetEndDateTime(&syEndDateTime);	//	測定終了日時の取得
	m_pDoc->GetSampleId(szSampleId);	//	サンプルIDの取得
	//2009.10.13 bagus RS --{--
	//ロットIDの追加
	m_pDoc->GetLotId(szLotId);		//	ロットIDの取得
	//2009.10.13 bagus RS --}--
	m_pDoc->GetUnitName(szUnit);	//	測定結果単位名の取得
	iSetPointCount = m_pDoc->GetStoreCount();	//格納済件数の取得
	m_pDoc->GetDispLabel(szDispLabel);
	sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);

	//2009.09.01 bagus stress --{--
	m_pDoc->GetRcpData(&rcp_data);
	//2009.09.01 bagus stress --}--

	//2009.10.28 bagus 2point-distance --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		iSetPointCount /= 2;
	}
	//2009.10.28 bagus 2point-distance --}--
//ファイルに追記

	char szSection[16 + 1];
	char szStartDate[255 + 1];
	char szStartTime[255 + 1];
	char szEndDate[255 + 1];
	char szEndTime[255 + 1];
	//2009.09.01 bagus stress --{--
	//項目が増えるのでバッファを増やす
	//char szFullLabel[1024+1];
	char szFullLabel[2048+1];
	//2009.09.01 bagus stress --}--
	char szPointNum[255 + 1]; //ポイント数
	char szPointKey[255 + 1]; //ポイント数キー名
	char szPointData[1024+1];
	char szTempFormat[32+1];
/* added 2017.04.29 hmenjo 整数表示化 SD 除外 ---------- { ---------- */
	char szTempFormatThickSD[32+1];
/* added 2017.04.29 hmenjo 整数表示化 SD 除外 ---------- } ---------- */
	ONE_POINT_DATA OnePointData;
	int iWritePoint = 0;
	//2009.09.01 bagus stress --{--
	int iLSScansNum = 0;
	char szStressUnit[32+1];
	//2009.09.01 bagus stress --}--
	int i,j;

	memset(szSection, 0, sizeof(szSection));
	memset(szStartDate, 0 ,sizeof(szStartDate));
	memset(szStartTime, 0, sizeof(szStartTime));
	memset(szEndDate, 0, sizeof(szEndDate));
	memset(szEndTime, 0, sizeof(szEndTime));
	memset(szFullLabel, 0, sizeof(szFullLabel));
	memset(szPointNum, 0, sizeof(szPointNum));
	memset(szPointKey, 0, sizeof(szPointKey));
	memset(szPointData, 0, sizeof(szPointData));
	memset(szTempFormat, 0, sizeof(szTempFormat));
	memset(&OnePointData, 0, sizeof(OnePointData));



//測定開始(終了)時間
	sprintf(szStartDate, "%04d/%02d/%02d", syStartDateTime.wYear,syStartDateTime.wMonth,syStartDateTime.wDay); //測定開始日付
	sprintf(szStartTime, "%02d:%02d:%02d.%03d", syStartDateTime.wHour,syStartDateTime.wMinute,syStartDateTime.wSecond,syStartDateTime.wMilliseconds); //測定開始時刻
	sprintf(szEndDate, "%04d/%02d/%02d", syEndDateTime.wYear,syEndDateTime.wMonth,syEndDateTime.wDay); //測定終了日付
	sprintf(szEndTime, "%02d:%02d:%02d.%03d", syEndDateTime.wHour,syEndDateTime.wMinute,syEndDateTime.wSecond,syEndDateTime.wMilliseconds); //測定終了時刻

	iWritePoint = 0;
	iWritePoint += sprintf(szFullLabel + iWritePoint, "%s",  "    "); ////ブランク*4
//2009.09.01 bagus stress --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		//SR項目
	//ヘッダ1(X,Y,Z部)
		for(i=1; i<4+4; i++)
		{
			char szXyz[255+1];
			memset(szXyz, 0, sizeof(szXyz));

//2009.10.20 bagus Stress 修正 --{--
#if 0
			if(i == 4)
				sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "MPa"); 		//ストレスはMPa
			else
				sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]はmm
#else
			if (i == 4) {
				// 2009.10.23 bagus Stress 修正 --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "[MPa]"); 		//ストレスはMPa
				sprintf(szTmp, "%s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "[MPa]"); 		//ストレスはMPa
				// 2009.10.23 bagus Stress 修正 --}--
			} else if (i > 4) {
				// 2009.10.23 bagus Stress 修正 --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]はmm
				sprintf(szTmp, "%s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]はmm
				// 2009.10.23 bagus Stress 修正 --}--
			} else {
				// 2009.10.23 bagus Stress 修正 --{--
				//sprintf(szXyz, "     %s", MEAS_LIST_STRESS_POSITION_ITEM[i]); //g_lpszXyzUnit[0]はmm
				sprintf(szTmp, "%s", MEAS_LIST_STRESS_POSITION_ITEM[i]); //g_lpszXyzUnit[0]はmm
				// 2009.10.23 bagus Stress 修正 --}--
			}
			// 2009.10.23 bagus Stress 修正 --{--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp);
			sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
			// 2009.10.23 bagus Stress 修正 --}--
#endif
//2009.10.20 bagus Stress 修正 --}--
			if(i==1)
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szFullLabel + iWritePoint, "\"%s\"",  szXyz); ////X,Y,Z
				iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
			else
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szXyz); ////X,Y,Z
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
		}
	//ヘッダ2(AF部追記)
		// 2009.10.23 bagus Stress 修正 --{--
		//iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",	MEAS_LIST_AF_CHECK_ITEM[0]);
		nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
		iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress 修正 --}--
	}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		for(i=0; i<4; i++)
		{
			char szXyz[255+1];
			memset(szXyz, 0, sizeof(szXyz));
			sprintf(szTmp, "%s%s", MEAS_LIST_POSITION_ITEM[i % 2 + 1], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]はmm
			nColumnLen = COLUMN_LENGTH - strlen(szTmp);
			sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
//2009.12.10 bagus 修正 --{--
//			if(i==1)
			if(i==0)
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szFullLabel + iWritePoint, "\"%s\"",  szXyz); ////X,Y,Z
				iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
			else
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szXyz); ////X,Y,Z
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
//2009.12.10 bagus 修正 --{--
		}

	// 2009.10.23 bagus Stress 修正 --}--
	//2009.10.28 bagus 2point-Distance --{--
	}else{
		//SRなど
// 2013.12.19 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2013.12.19 Bagus Add (TohoSpec対応) <--
			//ヘッダ1(X,Y,Z部)
			for(i=1; i<4; i++)
			{
				char szXyz[255+1];
				memset(szXyz, 0, sizeof(szXyz));
				// 2009.10.23 bagus Stress 修正 --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]はmm
				sprintf(szTmp, "%s%s", MEAS_LIST_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]はmm
				nColumnLen = COLUMN_LENGTH - strlen(szTmp);
				sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
				// 2009.10.23 bagus Stress 修正 --}--
				if(i==1)
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szFullLabel + iWritePoint, "\"%s\"",  szXyz); ////X,Y,Z
					iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
				else
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szXyz); ////X,Y,Z
					iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
			}
// 2013.12.19 Bagus Add (TohoSpec対応) -->
		}
// 2013.12.19 Bagus Add (TohoSpec対応) <--
//ヘッダ2(AF部追記)
// 2009.12.17 bagus CA --{--
#if 0
		// 2009.10.23 bagus Stress 修正 --{--
		//iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",	MEAS_LIST_AF_CHECK_ITEM[0]);
		nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
		iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress 修正 --}--

#else
		if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP) {
			// RSは出力しない
		} else if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA) {
			// CAは出力しない
		} else {
// 2013.12.19 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//			if(g_lModelType != MODEL_T3100){
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
// 2013.12.19 Bagus Add (TohoSpec対応) <--
				nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2013.12.19 Bagus Add (TohoSpec対応) -->
			}
// 2013.12.19 Bagus Add (TohoSpec対応) <--
		}
#endif
// 2009.12.17 bagus CA --}--
	}
//2009.09.01 bagus stress --}--
//ヘッダ3(ストラテジ部追記)

	//2009.11.17 bagus RS --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP){
		nColumnLen = COLUMN_LENGTH - strlen(_T("DATA"));
		sprintf(szTmp, "%*s%s", nColumnLen, " ", _T("DATA"));
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
		iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
	}
	//2009.11.17 bagus RS --}--

	for(i=0; szDispLabel[i][0] != '\0'; i++)
	{
// 2009.10.23 bagus Stress 修正 --{--
#if 0
		if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 10.10s\"",	szDispLabel[i]); //
		}
		else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,K値系
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 8.8s\"",  szDispLabel[i]); //
		}
		else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%系, Fit
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 8.8s\"",  szDispLabel[i]); //
		}
		else //その他
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 9.9s\"",  szDispLabel[i]); //
		}
#else
		nColumnLen = COLUMN_LENGTH - strlen(szDispLabel[i]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", szDispLabel[i]);
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.12.19 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
	// 2009.12.15 bagus NANOMAP Mod -->
	//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
	// 2009.12.15 bagus NANOMAP Mod <--
// 2013.12.19 Bagus Add (TohoSpec対応) -->
		}
		else{
			if(i == 0){
				iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szTmp);
			}
			else{
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
			}
		}
// 2013.12.19 Bagus Add (TohoSpec対応) <--
#endif
// 2009.10.23 bagus Stress 修正 --}--
	}

	//DATファイル書き込み処理
	int iDescCount;
	RECIPE_FILE* pDesc = NULL;
	BOOL bSaveJudge = 0;

	//2009.09.01 bagus stress --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		iLSScansNum = m_pDoc->GetMeasuredStressLSNum();
		strcpy(szStressUnit,"MPa");
	}else{
		iLSScansNum = 0;
		strcpy(szStressUnit,"");
	}
	//2009.09.01 bagus stress --}--

//=============================================================================

///MeasuredData書き出し1(ポイント毎データを除く)
	const RECIPE_FILE SrMeasuredDataDesc[] =
	{
		//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
		{"MeasuredData",	"UserID",		'C',		"0",		szUserId			},
		{"",				"StartDate",	'C',		"0",		szStartDate 		},
		{"",				"StartTime",	'C',		"0",		szStartTime 		},
		{"",				"EndDate",		'C',		"0",		szEndDate			},
		{"",				"EndTime",		'C',		"0",		szEndTime			},
		{"",				"SampleID", 	'C',		"0",		szSampleId			},
//2009.10.13 bagus RS --{--
//ロットIDの追加
		{"",				"LotID", 		'C',		"0",		szLotId			  	},
//2009.10.13 bagus RS --}--
		{"",				"Unit", 		'C',		"0",		szUnit				},
		{"",				"MeasuredNum",	'I',		"0",		&iSetPointCount 	},
//2009.09.01 bagus stress --{--
//ストレス用の項目の追加
		{"",				"MeasuredLSScanNum",	'I',  "0",		&iLSScansNum 		},
		{"",				"StressUnit",			'C',  "",		szStressUnit 		},
//2009.09.01 bagus stress --}--
		{"",				"Header",		'C',		"0",		szFullLabel 		},
	};
	//SrMeasuredDataDesc[]の書き込み
	iDescCount = sizeof(SrMeasuredDataDesc) / sizeof(SrMeasuredDataDesc[0]);
	pDesc = new RECIPE_FILE [iDescCount];
	memcpy(pDesc, SrMeasuredDataDesc, sizeof(SrMeasuredDataDesc));

	SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

	if ( pDesc ) {
		delete [] pDesc;
		pDesc = NULL;
	}

//=============================================================================


///MeasuredData書き出し2(ポイント毎データのみ)
	for(i=0; i<iSetPointCount; i++)
	{
		ONE_POINT_DATA OnePointData2;
		double dDistance;
	//2009.10.28 bagus 2point-Distance --{--
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			m_pDoc->GetOnePointData(&OnePointData, 2 * i+1);		//始点
			m_pDoc->GetOnePointData(&OnePointData2, 2 * i+2);		//終点
			ZeroBungUp(szPointNum, i+1);
			sprintf(szPointKey, "%s%s", "Point", szPointNum);
			nKeyLen = strlen(szPointKey);
			iWritePoint = 0;
			//Start X
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//Start Y
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//Start X
			sprintf(szTmp2, "%+.3lf",  OnePointData2.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//Start Y
			sprintf(szTmp2, "%+.3lf",  OnePointData2.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//Distance
			dDistance = m_pMainFrame->Calc2PointDistance(OnePointData.xyz.lX,OnePointData.xyz.lY,OnePointData2.xyz.lX,OnePointData2.xyz.lY);
			sprintf(szTmp2, "%+.3lf",  dDistance * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//2009.11.26 bagus 2point-distance --{--
			//dx,dy項目の追加
			//dx
			//2009.11.29 bagus 2point-distance --{--
			//sprintf(szTmp2, "%+.3lf",  (OnePointData.xyz.lX - OnePointData2.xyz.lX) * g_dXyzUnitConvertCoefficient[0]); //X
			sprintf(szTmp2, "%+.3lf",  fabs((OnePointData.xyz.lX - OnePointData2.xyz.lX) * g_dXyzUnitConvertCoefficient[0])); //X
			//2009.11.29 bagus 2point-distance --}--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//dy
			//2009.11.29 bagus 2point-distance --{--
			//sprintf(szTmp2, "%+.3lf", (OnePointData.xyz.lY - OnePointData2.xyz.lY) * g_dXyzUnitConvertCoefficient[0]); //X
			sprintf(szTmp2, "%+.3lf", fabs((OnePointData.xyz.lY - OnePointData2.xyz.lY) * g_dXyzUnitConvertCoefficient[0])); //X
			//2009.11.29 bagus 2point-distance --}--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//2009.11.26 bagus 2point-distance --}--


	//2009.10.28 bagus 2point-Distance --}--
	//2009.11.17 bagus RS --{--
		}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP){
			m_pDoc->GetOnePointData(&OnePointData, i+1);
			m_pDoc->GetOnepointDdata(i+1, dData);
			//キー
			ZeroBungUp(szPointNum, i+1);
			sprintf(szPointKey, "%s%s", "Point", szPointNum);
			// 2009.10.23 bagus Stress 修正 --{--
			nKeyLen = strlen(szPointKey);
			// 2009.10.23 bagus Stress 修正 --}--
			iWritePoint = 0;

			//X,Y,Z部
			// 2009.10.23 bagus Stress 修正 --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, "\"% +10.3lf\"",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress 修正 --}--

			// 2009.10.23 bagus Stress 修正 --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress 修正 --}--

			// 2009.10.23 bagus Stress 修正 --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10 ); //Z
			sprintf(szTmp2, "%+.3lf",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10); //Z
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress 修正 --}--

// 2009.12.17 bagus RS --{--
#if 0
			//AF部追記
			// 2009.10.23 bagus Stress 修正 --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",	OnePointData.szShowAfValid); //AF
			sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress 修正 --}--
#endif
// 2009.12.17 bagus RS --}--

			{
				int nRSDataCol;
				RESISTRESULT ResistOnePointData;
				BOOL bResitValidData = FALSE;
				CString strBuffer;
				m_pDoc->GetOneResistData(&ResistOnePointData,i+1);

				switch(ResistOnePointData.chDataValid){
				case _T('N'):
					bResitValidData = TRUE;
					strBuffer.Format(_T("%s"),_T("VALID"));
					break;
				case _T('O'):
					bResitValidData = FALSE;
					strBuffer.Format(_T("%s"),_T("OV.RG"));
					break;
				case _T('L'):
					bResitValidData = FALSE;
					strBuffer.Format(_T("%s"),_T("OV.LD"));
					break;
				default:
					bResitValidData = FALSE;
					strBuffer.Format(_T("%s"),_T("UNKNOWN"));
					break;
				}
				nColumnLen = COLUMN_LENGTH - strBuffer.GetLength();
				sprintf(szTmp, "%*s%s", nColumnLen, " ", strBuffer.GetBuffer(0));
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--

				for(nRSDataCol = 0;nRSDataCol < 4;nRSDataCol++){
					//MeasurementData
					switch(nRSDataCol){
					//2009.11.17 RS --{--
					case 0:	strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szResistance : _T("")); 		break;
					case 1:	strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szSurfaceResistivity : _T(""));	break;
					case 2:	strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szVolumeResistivity : _T(""));	break;
					case 3:	strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szConductivity : _T(""));		break;
					//2009.11.17 RS --}--
					}
					nColumnLen = COLUMN_LENGTH - strBuffer.GetLength();
					sprintf(szTmp, "%*s%s", nColumnLen, " ", strBuffer.GetBuffer(0));
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				}
			}


	//2009.11.17 bagus RS --}--
		}else{
			m_pDoc->GetOnePointData(&OnePointData, i+1);
			m_pDoc->GetOnepointDdata(i+1, dData);
		//キー
			ZeroBungUp(szPointNum, i+1);
			sprintf(szPointKey, "%s%s", "Point", szPointNum);
			// 2009.10.23 bagus Stress 修正 --{--
			nKeyLen = strlen(szPointKey);
			// 2009.10.23 bagus Stress 修正 --}--
			iWritePoint = 0;
		//2009.09.01 bagus stress --{--
		//ストレス用に項目を埋める
			if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
				for(int nSpacer = 0;nSpacer < 4;nSpacer++){
// 2009.10.22 bagus Stress 修正 --{--
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"        --\"");	//
					if (nSpacer == 0) {
						// 2009.10.23 bagus Stress 修正 --{--
						//iWritePoint += sprintf(szPointData + iWritePoint, "\"       ---\"");	//
						sprintf(szTmp2, "---");
						nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
						sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//						iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
						iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
						// 2009.10.23 bagus Stress 修正 --}--
					} else {
						// 2009.10.23 bagus Stress 修正 --{--
						//iWritePoint += sprintf(szPointData + iWritePoint, ",\"       ---\"");	//
						sprintf(szTmp2, "---");
						nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
						sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//						iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
						iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
						// 2009.10.23 bagus Stress 修正 --}--
					}
// 2009.10.22 bagus Stress 修正 --}--
				}
				iWritePoint += sprintf(szPointData + iWritePoint, ",");	//
			}
	//2009.09.01 bagus stress --}--

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.12.19 Bagus Add (TohoSpec対応) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpec対応) <--
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
				//X,Y,Z部
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, "\"% +10.3lf\"",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
				sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--

				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
				sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--

				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10 ); //Z
				sprintf(szTmp2, "%+.3lf",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10); //Z
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--

				//AF部追記
// 2009.12.17 bagus CA --{--
#if 0
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",	OnePointData.szShowAfValid); //AF
				sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--
#else
				if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType != HEAD_TYPE_CTA) {
					sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
					nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
					sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				}
#endif
// 2009.12.17 bagus CA --}--
// 2013.12.19 Bagus Add (TohoSpec対応) -->
			}
// 2013.12.19 Bagus Add (TohoSpec対応) <--

			int iUnitIndex = 0;
			int iUnit = m_pDoc->GetUnitIndex();
			if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
			else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
			if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
				iUnitIndex = 0;
			}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

			//ストラテジ部
			for(j=0; szDispLabel[j][0] != '\0'; j++)
			{
				char szDecimalPointBuff[255+1];
				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				if(strstr(szDispLabel[j], "Thick") != NULL) //Thick*
				{
/* modified 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
//					// 2009.10.23 bagus Stress 修正 --{--
//					//sprintf(szTempFormat, ",%s 10%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //選択Unitで変動
//					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //選択Unitで変動
//					// 2009.10.23 bagus Stress 修正 --}--
/* modified 2017.04.15 hmenjo 整数表示化 ----------              */
					if (l_iUnitIndexThickInt < 0) {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //選択Unitで変動
					} else {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "lf");
					}
/* modified 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */
				}
				else if((szDispLabel[j][0]=='N')||(szDispLabel[j][0]=='K')) //N,K値系
				{
					// 2009.10.23 bagus Stress 修正 --{--
					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[4], "lf\""); //[.####]固定
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[4], "lf"); //[.####]固定
					// 2009.10.23 bagus Stress 修正 --}--
				}
				else if(strstr(szDispLabel[j], "%") != NULL) //%系
				{
					// 2009.10.23 bagus Stress 修正 --{--
					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]固定
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]固定
					// 2009.10.23 bagus Stress 修正 --}--
				}
				else if(strstr(szDispLabel[j], "Fit") != NULL) //Fit
				{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//					// 2009.10.23 bagus Stress 修正 --{--
//					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]固定
//					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]固定
//					// 2009.10.23 bagus Stress 修正 --}--
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
					if (1 == g_lXmpType) {
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit 指数表示 %.3e ---------- { ---------- */
////						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf"); //[.##]固定
///* modified 2014.10.27 hmenjo Fit 指数表示 %.3e ----------              */
//						strcpy(szTempFormat, "%.3e");
///* modified 2014.10.27 hmenjo Fit 指数表示 %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ----------              */
#if FIT_DISP_SCOUT == 3
						strcpy(szTempFormat, "");
#elif FIT_DISP_SCOUT == 2
						strcpy(szTempFormat, "%.2e");
#elif FIT_DISP_SCOUT == 1
						strcpy(szTempFormat, "%.3e");
#else
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf"); //[.##]固定
#endif
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ---------- } ---------- */
					} else {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]固定
					}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
				}
				else //その他
				{
					// 2009.10.23 bagus Stress 修正 --{--
					//sprintf(szTempFormat, ",%s 9%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //選択Unitで変動
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //選択Unitで変動
					// 2009.10.23 bagus Stress 修正 --}--
				}

				if(strstr(szDispLabel[j], "Thick") != NULL) //ラベル名がThick*
				{
					// 2009.10.23 bagus Stress 修正 --{--
					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j] * g_dUnitConvert[iUnitIndex]); //
					sprintf(szTmp2, szTempFormat,  dData[j] * g_dUnitConvert[iUnitIndex]);
					// 2009.10.23 bagus Stress 修正 --}--
				}
				else if(strstr(szDispLabel[j], "%") != NULL) //ラベル名が%系
				{
					// 2009.10.23 bagus Stress 修正 --{--
					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j] * 100); //
					sprintf(szTmp2, szTempFormat,  dData[j] * 100);
					// 2009.10.23 bagus Stress 修正 --}--
				}
				else //
				{
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ---------- { ---------- */
//					// 2009.10.23 bagus Stress 修正 --{--
//					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j]); //
//					sprintf(szTmp2, szTempFormat,  dData[j]);
//					// 2009.10.23 bagus Stress 修正 --}--
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ----------              */
					if (0 == strlen(szTempFormat)) {
						long l_lExp = m_pDoc->GetExpDouble(dData[j]);
						sprintf(szTmp2, "%.2fe%+03d",  dData[j] / pow(10, l_lExp), l_lExp);
					} else {
						sprintf(szTmp2, szTempFormat,  dData[j]);
					}
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ---------- } ---------- */
				}

				// 2009.10.23 bagus Stress 修正 --{--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.12.19 Bagus Add (TohoSpec対応) -->
//				if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpec対応) <--
				if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
// 2013.12.19 Bagus Add (TohoSpec対応) -->
				}
				else{
					if(j == 0){
						iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
					}
					else{
						iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
					}
				}
// 2013.12.19 Bagus Add (TohoSpec対応) <--
				// 2009.10.23 bagus Stress 修正 --}--
			}
		//2009.10.28 bagus 2point-Distance --{--
		}
		//2009.10.28 bagus 2point-Distance --}--

		const RECIPE_FILE SrPointDataDesc[] =
		{
			//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
			{"MeasuredData",	szPointKey, 	'C',		"0",		szPointData 	},
		};

		//SrPointDataDesc[]の書き込み
		iDescCount = sizeof(SrPointDataDesc) / sizeof(SrPointDataDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrPointDataDesc, sizeof(SrPointDataDesc));

		SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

		if ( pDesc ) {
			delete [] pDesc;
			pDesc = NULL;
		}

	}

	//2009.09.01 bagus stress --{--
	//ストレスの場合のLSのデータを出力する
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		char szLS[16+1];
		STAGE_PROG_STRESS& stressStage = rcp_data.StressStageProgInfo;

		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			if(!m_pDoc->GetOneStressLineDataValid(nLine+1)){
				continue;
			}

			for(int nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
				memset(szLS,NULL,sizeof(szLS));
				memset(szPointData,NULL,sizeof(szPointData));
				iWritePoint = 0;
				//有効でないセクションはなにもしない
				if(!stressStage.Line[nLine].bScanValid[nSection]){
					continue;
				}
			//LmSn
				sprintf(szLS,"L%dS%d",nLine+1,nSection+1);
			//X-Start
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%ld\"",stressStage.Line[nLine].SectPos[nSection].lScanStartPosX);
				nKeyLen = 10 - strlen(szLS);
				if (nKeyLen < 0)
					nKeyLen = 0;
				//2009.12.24 bagus stress --{--
				//sprintf(szTmp2, "%ld", stressStage.Line[nLine].SectPos[nSection].lScanStartPosX);
				sprintf(szTmp2, "%.4f", stressStage.Line[nLine].SectPos[nSection].lScanStartPosX / (double)MICROMETRE);
				//2009.12.24 bagus stress --}--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, "%*s\"%s\"", nKeyLen, " ", szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, "%*s\"%s\"", nKeyLen+1, " ", szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--
			//X-End
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%ld\"",stressStage.Line[nLine].SectPos[nSection].lScanEndPosX);
				//2009.12.24 bagus stress --{--
				//sprintf(szTmp2, "%ld", stressStage.Line[nLine].SectPos[nSection].lScanEndPosX);
				sprintf(szTmp2, "%.4f", stressStage.Line[nLine].SectPos[nSection].lScanEndPosX / (double)MICROMETRE);
				//2009.12.24 bagus stress --}--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--
			//Y
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%ld\"",stressStage.Line[nLine].SectPos[nSection].lScanPosY);
				//2009.12.24 bagus stress --{--
				//sprintf(szTmp2, "%ld", stressStage.Line[nLine].SectPos[nSection].lScanPosY);
				sprintf(szTmp2, "%.4f", stressStage.Line[nLine].SectPos[nSection].lScanPosY / (double)MICROMETRE);
				//2009.12.24 bagus stress --}--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--
			//Stress
				STRESSRESULT tStressResult;
				m_pDoc->GetOneStressLineData(&tStressResult,nLine+1);
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint,",\"%.3lf\"",tStressResult.dStressSection[nSection]);
				sprintf(szTmp2, "%.3lf", tStressResult.dStressSection[nSection]);
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--
				//ストレス項目に無関係な項目を埋める
// 2009.10.23 bagus Stress 修正 --{--
#if 0
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				// 2009.10.23 bagus Stress 修正 --}--
#else
				sprintf(szTmp2, "---");
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
	#if 0
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
// 2009.12.14 bagus Stress 修正 --{--
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
// 2009.12.14 bagus Stress 修正 --}--
	#else
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
	#endif
// 2009.12.15 bagus NANOMAP Mod <--
#endif
// 2009.10.23 bagus Stress 修正 --}--
				//
				const RECIPE_FILE StressLSDataDesc[] =
				{
					//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
					{"MeasuredData",	szLS,	  'C',		  "0",		  szPointData	  },
				};

				iDescCount = sizeof(StressLSDataDesc) / sizeof(StressLSDataDesc[0]);
				pDesc = new RECIPE_FILE [iDescCount];
				memcpy(pDesc, StressLSDataDesc, sizeof(StressLSDataDesc));

				SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

				if ( pDesc ) {
					delete [] pDesc;
					pDesc = NULL;
				}
			}

		}
	}
	//2009.09.01 bagus stress --}--


	//改行(Point*****の項目後に1行改行を追加)
	FILE *fp;
	if ((fp = fopen(szFilePath, "a")) == NULL)
	{
		//ファイルオープンが失敗した場合、改行を入れず続行
	}
	else
	{
		putc('\n', fp);
		//2009.08.28 bagus stress --{--
		fclose(fp);
		//2009.08.28 bagus stress --}--
	}
	//2009.08.28 bagus stress --{--
	//失敗したのであればfcloseしない
	//fclose(fp);
	//2009.08.28 bagus stress --}--


//========================================
///MeasuredData書き出し3(統計データのみ)


	char szStrategyLabel[1024+1];
	char szValidMesuredNum[1024+1];
	char szHeader[1024+1];
	char szMax[1024+1];
	char szMin[1024+1];
	char szRange[1024+1];
	char szMean[1024+1];
	char szSd[1024+1];
	memset(szStrategyLabel, 0, sizeof(szStrategyLabel));
	memset(szValidMesuredNum, 0, sizeof(szValidMesuredNum));
	memset(szHeader, 0, sizeof(szHeader));
	memset(szMax, 0, sizeof(szMax));
	memset(szMin, 0, sizeof(szMin));
	memset(szRange, 0, sizeof(szRange));
	memset(szMean, 0, sizeof(szMean));
	memset(szSd, 0, sizeof(szSd));

// 2009.10.23 bagus Stress 修正 --{--
	BOOL bWriteHead = FALSE;
// 2009.10.23 bagus Stress 修正 --}--

	//統計データ有効Point数, ストラテジィヘッダ
	iWritePoint = 0;
// 2009.10.23 bagus Stress 修正 --{--
	nKeyLen = 10 - strlen("Header");
	if (nKeyLen < 0) {
		nKeyLen = 0;
	} else {
		iWritePoint += sprintf(szStrategyLabel + iWritePoint, "%*s", nKeyLen, " ");
	}
// 2009.10.23 bagus Stress 修正 --}--
	//2009.09.01 bagus stress --{--
	//ストレスの統計情報用ヘッド
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		// 2009.10.23 bagus Stress 修正 --{--
		//iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"Stress[MPa]\"");
		sprintf(szTmp2, "%s", "Stress[MPa]");
		nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"%s\"",  szTmp);
		iWritePoint += sprintf(szStrategyLabel + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress 修正 --}--
		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			STGPGM_STRESS_LINE& LineInfo = rcp_data.StressStageProgInfo.Line[nLine];
			if(LineInfo.bValidLine){
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szStrategyLabel + iWritePoint,",\"Stress L%d\"",nLine+1);
				sprintf(szTmp2, "Stress L%d", nLine+1);
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szStrategyLabel + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szStrategyLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress 修正 --}--
			}
		}
// 2009.10.23 bagus Stress 修正 --{--
		bWriteHead = TRUE;
// 2009.10.23 bagus Stress 修正 --}--
	}
	//2009.09.01 bagus stress --}--
	for(i=0; szDispLabel[i][0] != '\0'; i++)
	{
// 2009.10.23 bagus Stress 修正 --{--
#if 0
		//2009.09.01 bagus stress --{--
		//if(i != 0)
		if(iWritePoint != 0)
		//2009.09.01 bagus stress --}--
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, ",");
		}
#else
		if (bWriteHead) {
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, ",");
		} else {
			//
		}
#endif
// 2009.10.23 bagus Stress 修正 --}--
// 2009.10.23 bagus Stress 修正 --{--
#if 0
		if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 10.10s\"",  szDispLabel[i]); //
		}
		else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,K値系
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 8.8s\"",  szDispLabel[i]); //
		}
		else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%系, Fit
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 8.8s\"",  szDispLabel[i]); //
		}
		else //その他
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 9.9s\"",  szDispLabel[i]); //
		}
#else
		nColumnLen = COLUMN_LENGTH - strlen(szDispLabel[i]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", szDispLabel[i]);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"%s\"",  szTmp);
		iWritePoint += sprintf(szStrategyLabel + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
#endif
// 2009.10.23 bagus Stress 修正 --}--
// 2009.10.23 bagus Stress 修正 --{--
		bWriteHead = TRUE;
// 2009.10.23 bagus Stress 修正 --}--
	}

	int iStatisticsValidPointCount = m_pDoc->GetStatisticsValidPointCount();
	const RECIPE_FILE SrStatisticsHeaderDesc[] = //統計データ有効数、ストラテジィヘッダ
	{
		//Section Name		Key Name					Data Type	Defaule 	Variable Pointer
		{"STATISTICS",		"ValidMesuredNum",			'I',		"0",		&iStatisticsValidPointCount },
		{"",				"Header",					'C',		"0",		szStrategyLabel 							},
	};

	//SrStatisticsHeaderDesc[]の書き込み
	iDescCount = sizeof(SrStatisticsHeaderDesc) / sizeof(SrStatisticsHeaderDesc[0]);
	pDesc = new RECIPE_FILE [iDescCount];
	memcpy(pDesc, SrStatisticsHeaderDesc, sizeof(SrStatisticsHeaderDesc));

	SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

	if ( pDesc ) {
		delete [] pDesc;
		pDesc = NULL;
	}

	int iUnitIndex = 0;
	int iUnit = m_pDoc->GetUnitIndex();
	if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
	else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
	if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
		iUnitIndex = 0;
	}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */
	memset(szTempFormat, 0, sizeof(szTempFormat));
/* added 2017.04.29 hmenjo 整数表示化 SD 除外 ---------- { ---------- */
	memset(szTempFormatThickSD, 0, sizeof(szTempFormatThickSD));
/* added 2017.04.29 hmenjo 整数表示化 SD 除外 ---------- } ---------- */

// 2009.10.23 bagus Stress 修正 --{--
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.10.23 bagus Stress 修正 --}--

	for(i=0; i<5; i++)
	{
		iWritePoint = 0;
// 2009.10.23 bagus Stress 修正 --{--
		bWriteHead = FALSE;
		if(l_SystemConfig.nLanguage == 0){
			nKeyLen = 10 - strlen(MEAS_LIST_PARAM_ITEM_ENU[i]);
		} else {
			nKeyLen = 10 - strlen(MEAS_LIST_PARAM_ITEM_JPN[i]);
		}

		if (nKeyLen < 0) {
			nKeyLen = 0;
		} else {
			iWritePoint += sprintf(szPointData + iWritePoint, "%*s", nKeyLen, " ");
		}
// 2009.10.23 bagus Stress 修正 --}--
		int iWritePointSpaceBung;
		iWritePointSpaceBung=0;

		//2009.09.01 bagus stress --{--
		//ストレスの場合にはストレス用の項目の統計情報を追加する
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
			STATISTICS l_StressStat;
			m_pDoc->GetStressStatisticsData(&l_StressStat);
			double dStressData;
			if(i==0)		dStressData = l_StressStat.dMaximum;
			else if(i==1)	dStressData = l_StressStat.dMinimum;
			else if(i==2)	dStressData = l_StressStat.dRange;
			else if(i==3)	dStressData = l_StressStat.dAverage;
			else/*if(i==4)*/dStressData = l_StressStat.dStandardDeviation;
			// 2009.10.23 bagus Stress 修正 --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, "\"%.3lf\"",dStressData);
			sprintf(szTmp2, "%.3lf", dStressData);
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress 修正 --}--

			for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
				STGPGM_STRESS_LINE& LineInfo = rcp_data.StressStageProgInfo.Line[nLine];
				if(LineInfo.bValidLine){
					m_pDoc->GetStressLineStatisticsData(nLine+1,&l_StressStat);
					double dStressData;
					if(i==0)		dStressData = l_StressStat.dMaximum;
					else if(i==1)	dStressData = l_StressStat.dMinimum;
					else if(i==2)	dStressData = l_StressStat.dRange;
					else if(i==3)	dStressData = l_StressStat.dAverage;
					else/*if(i==4)*/dStressData = l_StressStat.dStandardDeviation;
					// 2009.10.23 bagus Stress 修正 --{--
					//iWritePoint += sprintf(szPointData + iWritePoint,",\"%.3lf\"",dStressData);
					sprintf(szTmp2, "%.3lf", dStressData);
					nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
					sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
					// 2009.10.23 bagus Stress 修正 --}--
				}
			}
// 2009.10.23 bagus Stress 修正 --{--
			bWriteHead = TRUE;
// 2009.10.23 bagus Stress 修正 --}--
		}
		//2009.09.01 bagus stress --}--

		//Max,Min,Range,Mean,S.D.
		//ストラテジ部
		for(j=0; szDispLabel[j][0] != '\0'; j++)
		{
			if(strstr(szDispLabel[j], "Thick") != NULL) //Thick*
			{
/* modified 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
//				// 2009.10.23 bagus Stress 修正 --{--
//				//sprintf(szTempFormat, "%s 10%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //選択Unitで変動
//				sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //選択Unitで変動
//				// 2009.10.23 bagus Stress 修正 --}--
/* modified 2017.04.15 hmenjo 整数表示化 ----------              */
				if (l_iUnitIndexThickInt < 0) {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //選択Unitで変動
				} else {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "lf");
				}
/* added 2017.04.29 hmenjo 整数表示化 SD 除外 ---------- { ---------- */
				sprintf(szTempFormatThickSD, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //選択Unitで変動
/* added 2017.04.29 hmenjo 整数表示化 SD 除外 ---------- } ---------- */
/* modified 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */
			}
			else if((szDispLabel[j][0]=='N')||(szDispLabel[j][0]=='K')) //N,K値系
			{
				// 2009.10.23 bagus Stress 修正 --{--
				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[4], "lf\""); //[.####]固定
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[4], "lf");
				// 2009.10.23 bagus Stress 修正 --}--
			}
			else if(strstr(szDispLabel[j], "%") != NULL) //%系
			{
				// 2009.10.23 bagus Stress 修正 --{--
				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]固定
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
				// 2009.10.23 bagus Stress 修正 --}--
			}
			else if(strstr(szDispLabel[j], "Fit") != NULL) //Fit
			{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//				// 2009.10.23 bagus Stress 修正 --{--
//				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]固定
//				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
//				// 2009.10.23 bagus Stress 修正 --}--
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
				if (1 == g_lXmpType) {
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit 指数表示 %.3e ---------- { ---------- */
////					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf");
///* modified 2014.10.27 hmenjo Fit 指数表示 %.3e ----------              */
//					strcpy(szTempFormat, "%.3e");
///* modified 2014.10.27 hmenjo Fit 指数表示 %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ----------              */
#if FIT_DISP_SCOUT == 3
					strcpy(szTempFormat, "");
#elif FIT_DISP_SCOUT == 2
					strcpy(szTempFormat, "%.2e");
#elif FIT_DISP_SCOUT == 1
					strcpy(szTempFormat, "%.3e");
#else
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf");
#endif
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ---------- } ---------- */
				} else {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
				}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
			}
			else //その他
			{
				// 2009.10.23 bagus Stress 修正 --{--
				//sprintf(szTempFormat, "%s 9%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //選択Unitで変動
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //選択Unitで変動
				// 2009.10.23 bagus Stress 修正 --}--
			}

			STATISTICS Statistics[ADAPRESULT_COLS_MAX];
			memset(Statistics, 0, sizeof(Statistics));
			m_pDoc->GetStatisticsData(Statistics);
			double dStatisticsData;
			if(i==0)		dStatisticsData = Statistics[j].dMaximum;
			else if(i==1)	dStatisticsData = Statistics[j].dMinimum;
			else if(i==2)	dStatisticsData = Statistics[j].dRange;
			else if(i==3)	dStatisticsData = Statistics[j].dAverage;
			else/*if(i==4)*/dStatisticsData = Statistics[j].dStandardDeviation;

// 2009.10.23 bagus Stress 修正 --{--
#if 0
			//詰め物
			char szSpaceBung[16+1];
			memset(szSpaceBung, 0, sizeof(szSpaceBung));
			//2009.09.01 bagus stress --{--
			//if(j == 0)
			if(iWritePoint == 0)
			//2009.09.01 bagus stress --}--
			{
				//Saiki 20090603 Change ----->
				//int iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM[i]); //6は"Header"の文字数(STATISTICSのキーの最長文字)
				int iSpaceBungCnt;
				SYSTEM_CONFIG l_SystemConfig;
				ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
				if(l_SystemConfig.nLanguage == 0){
						iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM_ENU[i]); //6は"Header"の文字数(STATISTICSのキーの最長文字)
				}
				else{
					iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM_JPN[i]); //6は"Header"の文字数(STATISTICSのキーの最長文字)
				}
				//Saiki 20090603 Change <-----
				for(int k=0; k < iSpaceBungCnt; k++)
				{
					iWritePointSpaceBung += sprintf(szSpaceBung + iWritePointSpaceBung, " ");
				}
				iWritePoint += sprintf(szPointData + iWritePoint, szSpaceBung); //
			}
#endif
// 2009.10.23 bagus Stress 修正 --}--
// 2009.10.23 bagus Stress 修正 --{--
#if 0
			//2009.09.01 bagus stress --{--
			//if(j != 0)
			if(iWritePoint != 0)
			//2009.09.01 bagus stress --}--
			{
				iWritePoint += sprintf(szPointData + iWritePoint, ",");
			}
#else
			if (bWriteHead) {
				iWritePoint += sprintf(szPointData + iWritePoint, ",");
			} else {
				//
			}
#endif
// 2009.10.23 bagus Stress 修正 --}--
			if(strstr(szDispLabel[j], "Thick") != NULL) //ラベル名がThick*
			{
/* modified 2017.04.29 hmenjo 整数表示化 SD 除外 ---------- { ---------- */
//				// 2009.10.23 bagus Stress 修正 --{--
//				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData/*dData[j]*/ * g_dUnitConvert[iUnitIndex]); //
//				sprintf(szTmp2, szTempFormat,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
//				// 2009.10.23 bagus Stress 修正 --}--
/* modified 2017.04.29 hmenjo 整数表示化 SD 除外 ----------              */
				if (i == 4) {
					/* SD 値	*/
					sprintf(szTmp2, szTempFormatThickSD,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
				} else {
					/* SD 値以外	*/
					sprintf(szTmp2, szTempFormat,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
				}
/* modified 2017.04.29 hmenjo 整数表示化 SD 除外 ---------- } ---------- */
			}
			else if(strstr(szDispLabel[j], "%") != NULL) //ラベル名が%系
			{
				// 2009.10.23 bagus Stress 修正 --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData * 100); //
				sprintf(szTmp2, szTempFormat,  dStatisticsData * 100);
				// 2009.10.23 bagus Stress 修正 --}--
			}
			else //
			{
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ---------- { ---------- */
//				// 2009.10.23 bagus Stress 修正 --{--
//				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData/*dData[j]*/); //
//				sprintf(szTmp2, szTempFormat,  dStatisticsData);
//				// 2009.10.23 bagus Stress 修正 --}--
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ----------              */
				if (0 == strlen(szTempFormat)) {
					long l_lExp = m_pDoc->GetExpDouble(dStatisticsData);
					sprintf(szTmp2, "%.2fe%+03d",  dStatisticsData / pow(10, l_lExp), l_lExp);
				} else {
					sprintf(szTmp2, szTempFormat,  dStatisticsData);
				}
/* modified 2014.11.22 hmenjo Fit 指数表示(2) ---------- } ---------- */
			}

			// 2009.10.23 bagus Stress 修正 --{--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress 修正 --}--

// 2009.10.23 bagus Stress 修正 --{--
			bWriteHead = TRUE;
// 2009.10.23 bagus Stress 修正 --}--
		}

//Saiki 20090603 Change ----->
// 2009.10.23 bagus Stress 修正 --{--
//		SYSTEM_CONFIG l_SystemConfig;
//		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.10.23 bagus Stress 修正 --}--
		if(l_SystemConfig.nLanguage == 0){

			const RECIPE_FILE SrStatisticsDataDesc_ENU[] =
			{
				//Section Name		Key Name					Data Type	Defaule 	Variable Pointer
				{"STATISTICS",		/*szMeasListParamItem*/MEAS_LIST_PARAM_ITEM_ENU[i], 'C',		"0",		szPointData 	},
			};
			//SrStatisticsDataDesc[]の書き込み
			iDescCount = sizeof(SrStatisticsDataDesc_ENU) / sizeof(SrStatisticsDataDesc_ENU[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SrStatisticsDataDesc_ENU, sizeof(SrStatisticsDataDesc_ENU));
		}
		else{
			const RECIPE_FILE SrStatisticsDataDesc_JPN[] =
		{
			//Section Name		Key Name					Data Type	Defaule 	Variable Pointer
				{"STATISTICS",		/*szMeasListParamItem*/MEAS_LIST_PARAM_ITEM_JPN[i], 'C',		"0",		szPointData 	},
		};
		//SrStatisticsDataDesc[]の書き込み
			iDescCount = sizeof(SrStatisticsDataDesc_JPN) / sizeof(SrStatisticsDataDesc_JPN[0]);
		pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SrStatisticsDataDesc_JPN, sizeof(SrStatisticsDataDesc_JPN));
		}
//Saiki 20090603 Change <-----

		SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

		if(pDesc != NULL){
			delete [] pDesc;
			pDesc = NULL;
		}

	}

/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
	/* 使用結果データ出力*/
	if ((0 == this->MRCS_IncludeCountGet()) &&
		(0 == this->MRCS_DeleteCountGet())) {
		/* 指定行，削除済み行がともに０の場合は従来の仕様です．	*/
	} else {
		CString l_strPoints = _T("");
		CString l_strTmp;
		BOOL l_bUse = FALSE;
		int l_iprevUse = 0;
		for (i = 1; i <= iSetPointCount; i++) {
			if (0 == (MRCS_SettingStatusGet(i) & 0x4000)) {
				/* Valid = FALSE でない	*/
				if (0 < this->MRCS_IncludeCountGet()) {
					/* 指定行在り	*/
					if (1 == this->MRCS_SettingGet(i)) {
						l_bUse = TRUE;
					}
				} else {
					/* 指定行無し	*/
					if (2 != this->MRCS_SettingGet(i)) {
						l_bUse = TRUE;
					}
				}
			}
			if (FALSE != l_bUse) {
				/* 使用	*/
				l_bUse = FALSE;
				if (0 < l_strPoints.GetLength()) {
					/* ２回目以降	*/
					if (l_iprevUse == i - 1) {
						/* 連番	*/
						if (_T('-') != l_strPoints.GetAt(l_strPoints.GetLength() - 1)) {
							l_strPoints += _T("-");
						}
						if (i == iSetPointCount) {
							/* 最終データ	*/
							l_strTmp.Format(_T("%d"), iSetPointCount);
							l_strPoints += l_strTmp;
						}
					} else {
						/* 飛び番	*/
						if (_T('-') == l_strPoints.GetAt(l_strPoints.GetLength() - 1)) {
							l_strTmp.Format(_T("%d,%d"), l_iprevUse, i);
						} else {
							l_strTmp.Format(_T(",%d"), i);
						}
						l_strPoints += l_strTmp;
					}
				} else {
					/* 初回	*/
					l_strPoints.Format(_T("%d"), i);
				}
				l_iprevUse = i;
			} else {
				/* 未使用	*/
				if (i == iSetPointCount) {
					/* 最終データ	*/
					if (_T('-') == l_strPoints.GetAt(l_strPoints.GetLength() - 1)) {
						l_strTmp.Format(_T("%d"), l_iprevUse);
						l_strPoints += l_strTmp;
					}
				}
			}
		}
		TCHAR l_tszFilePath[_MAX_PATH];
		_stprintf(l_tszFilePath, _T("%s%s%s"), g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);
		::WritePrivateProfileString(_T("STATISTICS"), _T("Points"), l_strPoints, l_tszFilePath);
	}
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- } ---------- */

	return;
}



/////////////////////////////////////////////////////////////////////////////
//数値の前に0を補完し、5桁の数字にする。
BOOL CMeasurementTabView::ZeroBungUp(char* czPoint, int iPoint)
{
	char czTemp[5+1];
	memset(czTemp, 0, sizeof(czTemp));

	int i=0;
	int iCnt= 5; //前に'0'を補完する数(5で初期化)

	if((iPoint <= 99999) && (iPoint > 0)) //5桁まで可
	{
		_itoa(iPoint, czTemp, 10);

		for(i=0; czTemp[i]!='\0'; i++)
		{iCnt--;}

		for(i=0; i<5+1; i++)
		{
			if(i < iCnt){czPoint[i]='0';}
			else		{czPoint[i] = czTemp[i-iCnt];}
		}
		return TRUE;
	}

	czPoint[0] = '\0';
	return FALSE; //ここまで到達したら処理失敗

}

/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
TCHAR lg_tszDirSelect[_MAX_PATH * 2];	/* 選択されているディレクトリ */
TCHAR lg_tszEditText[_MAX_PATH];	/* 入力中のファイル名の一時記憶 */
TCHAR lg_tszErrMsg[256];	/* エラーメッセージ文字列 */
TCHAR lg_tszErrMsgTitle[256];	/* エラーメッセージタイトル文字列 */
UINT CALLBACK GetSaveFNHookProc(
		HWND hwndDlg,	/* コモンダイアログのコントロールのハンドル */
		UINT uiMsg,		/* メッセージ */
		WPARAM wParam,	/*	*/
		LPARAM lParam	/*	*/
	)
{
	static int ls_iDirChgPrc = 0;

	switch (uiMsg) {
	case WM_NOTIFY:
		{
			LPNMHDR l_pNMHdr = (LPNMHDR) lParam;
			LPOFNOTIFY l_pOpenFileNameNotify = (LPOFNOTIFY) lParam;
			TCHAR l_tszInvalid[64] = _T("!\"'*+,./:;<=>?@[]^`|~$&()\\");	/* 使用禁止文字 */
			TCHAR l_tszInvalid2[64] = _T("!\"'*+,./:;<=>?@[]^`|~$&()");	/* 使用禁止文字'\'は許可します */
			switch (l_pNMHdr->code) {
			case CDN_INITDONE:
				::EnableWindow(::GetDlgItem(l_pNMHdr->hwndFrom, cmb1), FALSE);	/* 場所コンボをディセーブル */
				::EnableWindow(::GetDlgItem(l_pNMHdr->hwndFrom, cmb2), FALSE);	/* 種類コンボをディセーブル */
				_tcscpy(lg_tszDirSelect, l_pOpenFileNameNotify->lpOFN->lpstrInitialDir);
				/* デスクトップの中央に表示します． */
				{
					int l_iLeft = ::GetSystemMetrics(SM_CXFULLSCREEN);
					int l_iTop = ::GetSystemMetrics(SM_CYFULLSCREEN);
					RECT l_rectDlg = {0};
					::GetWindowRect(::GetParent(hwndDlg), &l_rectDlg);
					l_iLeft = (l_iLeft / 2) - ((l_rectDlg.right - l_rectDlg.left) / 2);
					l_iTop = (l_iTop / 2) - ((l_rectDlg.bottom - l_rectDlg.top) / 2);
					::SetWindowPos(::GetParent(hwndDlg), 0, l_iLeft, l_iTop, 0, 0, SWP_NOSIZE);
				}
				break;
			case CDN_FOLDERCHANGE:	/* ディレクトリが変更されました． */
				{
					/* 変更されたディレクトリを取得 */
					TCHAR l_tszDirChanged[_MAX_DIR * 2];
					memset(l_tszDirChanged, 0, sizeof(l_tszDirChanged));
					CommDlg_OpenSave_GetFolderPath(l_pNMHdr->hwndFrom, l_tszDirChanged, sizeof(l_tszDirChanged));
					switch (ls_iDirChgPrc) {
					case 0:		/* 初期化直後 */
						ls_iDirChgPrc = 1;
						break;
					case 1:		/* ディレクトリ名に無効な文字が無いかチェック */
								/* と結果ディレクトリ以下かどうかをチェック */
								/*	(無効なディレクトリではなくて無効な文字のチェックのみです)*/
						{
							TCHAR l_tszDirChgd[_MAX_PATH * 2];
							_tcscpy(l_tszDirChgd, l_tszDirChanged);
							l_tszDirChgd[_tcslen(l_pOpenFileNameNotify->lpOFN->lpstrInitialDir)] = 0;
							if (
								(0 != _tcsicmp(l_tszDirChgd, l_pOpenFileNameNotify->lpOFN->lpstrInitialDir))
								|| (0 != _tcspbrk(&(l_tszDirChanged[_tcslen(l_pOpenFileNameNotify->lpOFN->lpstrInitialDir) + 1]), l_tszInvalid2))
								) {
								/* 無効なディレクトリなので，前のディレクトリに戻します．*/
								/* 入力中のファイル名を一時記憶 */
								CommDlg_OpenSave_GetSpec(l_pNMHdr->hwndFrom, lg_tszEditText, sizeof(lg_tszEditText));
								/* 前のディレクトリを EDIT コントロールにセット */
								::SendDlgItemMessage(l_pNMHdr->hwndFrom, edt1, WM_SETTEXT, 0, (LPARAM) lg_tszDirSelect);
								/* OK ボタンをクリックする */
								::PostMessage(l_pNMHdr->hwndFrom, WM_COMMAND, (WPARAM) IDOK, (LPARAM) 0);
								ls_iDirChgPrc = 2;
							} else {
								_tcscpy(lg_tszDirSelect, l_tszDirChanged);
							}
						}
						break;
					default:	/* ディレクトリ戻し待ち */
						if (0 == _tcsicmp(l_tszDirChanged, lg_tszDirSelect)) {
							/* 入力中のファイル名を EDIT コントロールに戻します */
							::SendDlgItemMessage(l_pNMHdr->hwndFrom, edt1, WM_SETTEXT, 0, (LPARAM) lg_tszEditText);
							ls_iDirChgPrc = 1;
						}
						break;
					}
				}
				break;
			case CDN_FILEOK:	/* OK ボタンが押されました． */
				{				/*		EDIT コントロールが空のときはココに来ません． */
					/* ファイル名を(パス無しで)取得 */
					TCHAR l_tszFile[_MAX_FNAME * 2];
					CommDlg_OpenSave_GetSpec(l_pNMHdr->hwndFrom, l_tszFile, sizeof(l_tszFile));
					/* 拡張子の有無を判断してあれば削除します． */
					TCHAR l_tszExt[_MAX_EXT] = _T(".");
					_tcscat(l_tszExt, l_pOpenFileNameNotify->lpOFN->lpstrDefExt);
					if (0 == _tcsicmp(&(l_tszFile[_tcslen(l_tszFile) - _tcslen(l_tszExt)]), l_tszExt)) {
						/* 指定された拡張子が入力されていたので，削除します． */
						l_tszFile[_tcslen(l_tszFile) - _tcslen(l_tszExt)] = 0;
					}
					if (0 != _tcspbrk(l_tszFile, l_tszInvalid)) {
						/* 無効文字発見 */
						/* エラーメッセージボックスを表示します． */
						::MessageBox(0, lg_tszErrMsg, lg_tszErrMsgTitle, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
						/* ダイアログを閉じないようにします． */
						::SetWindowLong(hwndDlg, DWL_MSGRESULT, 1);
						return 1;
					}
				}
				break;
			}
		}
		break;
	}

	return 0;
}
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */

//ファイル名をつけて保存
BOOL CMeasurementTabView::GetMeasSaveFileName(char* szThisInputFliePass)
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

//	char szThisInputFliePass[SAVE_FILE_PATH_NAME_LEN+1];
//	memset(szThisInputFliePass, 0, sizeof(szThisInputFliePass));

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	BOOL bSaveOverwrite = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite;

/* modified 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
	CString l_strErrMsg = _T("結果データ名に無効な文字があります．");
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	CString l_strErrMsgTitle = _T("NanoSpec");
	CString l_strErrMsgTitle;
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	LoadStringML(IDS_RSLTNM_ERRMSG, l_strErrMsg, _T("Invalid character(s) in Result data name."));
	LoadStringML(IDS_TITLE_NANOSPEC, l_strErrMsgTitle, _T("NanoSpec"));
	_tcscpy(lg_tszErrMsg, l_strErrMsg);
	_tcscpy(lg_tszErrMsgTitle, l_strErrMsgTitle);
	/* コモンファイルダイアログの準備 */
	TCHAR l_tszResultFile[_MAX_PATH * 2] = _T(SAVE_DEFAULT_NAME);	/* 結果ディレクトリ＆ファイル名 */
	TCHAR l_tszResultFileTitle[_MAX_FNAME];	/* 結果ファイル名(パス無し) */
	TCHAR l_tszDefaultDir[_MAX_PATH * 2];	/* 初期(デフォルト)ディレクトリ */
	_tcscpy(l_tszDefaultDir, g_szResult_Dir);
	if (_T('\\') == l_tszDefaultDir[_tcslen(g_szResult_Dir) - 1]) {
		/* 最後に'\'があれば削除します． */
		l_tszDefaultDir[_tcslen(g_szResult_Dir) - 1] = 0;
	}
	OPENFILENAME l_ofn;
	memset(&l_ofn, 0, sizeof(OPENFILENAME));
	l_ofn.lStructSize = sizeof(OPENFILENAME);
	l_ofn.hwndOwner = this->m_hWnd;
	l_ofn.lpstrFilter = _T("DAT Files (*.dat)\0*.dat\0\0");
	l_ofn.nFilterIndex = 0;
	l_ofn.lpstrFile = l_tszResultFile;
	l_ofn.nMaxFile = sizeof(l_tszResultFile);
	l_ofn.lpstrFileTitle = l_tszResultFileTitle;
	l_ofn.nMaxFileTitle = sizeof(l_tszResultFileTitle);
	l_ofn.lpstrInitialDir = l_tszDefaultDir;
	l_ofn.Flags = 0
				| OFN_NOCHANGEDIR
				| OFN_EXPLORER
				| OFN_ENABLEHOOK
				| OFN_HIDEREADONLY
				| 0;
	if ((m_pDoc->GetHostMode() == HOST_LOCAL) && (bSaveOverwrite == FALSE)) {
		/* ローカル時 かつ 上書き確認が必要 */
		l_ofn.Flags |= OFN_OVERWRITEPROMPT; /* 上書き確認 */
	}
	l_ofn.lpstrDefExt = _T("dat");
	l_ofn.lpfnHook = GetSaveFNHookProc;
	BOOL l_bRet = ::GetSaveFileName(&l_ofn);
	if (0 == l_bRet) {
		return FALSE;
	}
/* modified 2009.06.16 hmenjo 使用禁止文字チェック ----------			   */
//	  CFileDialog dlg( FALSE,
//					  "*.*",
//					  SAVE_DEFAULT_NAME,
//					  OFN_HIDEREADONLY | OFN_NOCHANGEDIR,	  // OFN_NOCHANGEDIR カレントディレクトリがズレる対策。CFileDialogの直前のカレントディレクトリに戻す
//					  "DAT File (*.dat)|*.dat||",
//					  this);
//
//	  if((m_pDoc->GetHostMode() == HOST_LOCAL)&&(bSaveOverwrite == FALSE)) //ローカル時 かつ 上書き確認が必要
//	  {
//		  dlg.m_ofn.Flags |= OFN_OVERWRITEPROMPT; //上書き確認
//	  }
//
//	  dlg.m_ofn.lpstrInitialDir = g_szResult_Dir;
//
//	  if(dlg.DoModal() != IDOK)
//	  {
//		  return FALSE;
//	  }
/* modified 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */

/* modified 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
//	  if(dlg.GetFileTitle().GetLength() > SAMPLE_ID_LEN)
/* modified 2009.06.16 hmenjo 使用禁止文字チェック ----------			   */
	TCHAR l_tszResultFName[_MAX_FNAME];
	_tcscpy(l_tszResultFName, l_tszResultFileTitle);
	TCHAR *l_pDotPos = _tcsrchr(l_tszResultFName, _T('.'));
	if (0 != l_pDotPos) {
		*l_pDotPos = 0;
	}
	if(_tcslen(l_tszResultFName) > SAMPLE_ID_LEN)
/* modified 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
	{
// Kojika 20090526 Change
//		MessageBox("保存名が長過ぎます", "測定データ保存", MB_OK);
		LoadStringML(IDS_NAME_TOO_LONG, strBuffer1, "Name is Too Long.");
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change
		return FALSE;
	}

/* modified 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
//	  if(dlg.GetPathName().GetLength() > SAVE_FILE_PATH_NAME_LEN)
/* modified 2009.06.16 hmenjo 使用禁止文字チェック ----------			   */
	if(_tcslen(l_tszResultFile) > SAVE_FILE_PATH_NAME_LEN)
/* modified 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
	{
// Kojika 20090526 Change
//		MessageBox("Path is Too Long.", "Measurement Data Save", MB_OK);
//		MessageBox("保存先パス名長過ぎます", "測定データ保存", MB_OK);
		LoadStringML(IDS_PATH_TOO_LONG, strBuffer1, "Path is Too Long.");
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change
		return FALSE;
	}

/* modified 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
//	  strcpy(szThisInputFliePass, dlg.GetPathName());
/* modified 2009.06.16 hmenjo 使用禁止文字チェック ----------			   */
	_tcscpy(szThisInputFliePass, l_tszResultFile);
/* modified 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */


	char szFullPath[MAX_PATH];
//	memset(szFullPath, 0, sizeof(szFullPath));
//	_fullpath(szFullPath, (const char *)RESULT_DIR, sizeof(szFullPath));
	strcpy(szFullPath, g_szResult_Dir);
	if(StrStrI(szThisInputFliePass, szFullPath)==NULL)
	{
		char szMessage[1024+1];
		memset(szMessage, 0, sizeof(szMessage));
// Kojika 20090526 Change
//		sprintf(szMessage, "%s%s", szFullPath, " 配下以外には保存することができません。\n再度、保存しなおしてください。");
//		MessageBox(szMessage, "測定データ保存", MB_OK);
//		sprintf(szMessage, " %s 配下以外には保存することができません。\n再度、保存しなおしてください。", szFullPath);
		LoadStringML(IDS_NOT_POSSIBLE_SAVE_BESIDE_BELOW, strBuffer1, "It isn't possible to save it besides below %s. \nPlease save it again.");
		sprintf(szMessage, strBuffer1, szFullPath);
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(szMessage, strBuffer2, MB_OK);
// Kojika 20090526 Change End
		return FALSE;
	}

	return TRUE; //ここまで来たなら正常
}



/////////////////////////////////////////////////////////////////////////////
//上書き確認ダイアログを出す
int CMeasurementTabView::SaveOverlap(char* szSaveFullFilePath)
{
	int iSaveNameOverlap;
	CSaveNameOverlapDlg SaveDlg;
	strcpy(SaveDlg.m_szSpecifiedFileName, szSaveFullFilePath); //重複ファイル名をダイアログに渡す
	iSaveNameOverlap = SaveDlg.DoModal();

	if(iSaveNameOverlap == SAVEOVERLAP_SAVE)		   //上書き保存(SAVE)
	{
		//処理続行
	}
	else if(iSaveNameOverlap == SAVEOVERLAP_NOSAVE)    //保存しない(CANCEL)
	{
		//
		return 2;	//2:失敗(保存しない)
	}
	else if(iSaveNameOverlap == SAVEOVERLAP_SAVEAS)    //名前を付けて保存(SAVE AS)
	{
		//名前を付けて保存(内部の選択によっては『保存しない』)
		if(!GetMeasSaveFileName(szSaveFullFilePath))
		{
			return 3;	//3:失敗(保存キャンセル)
/* added 2016.08.31 hmenjo 結果データ勝手に削除 ---------- { ---------- */
		} else {
			/* 名前を付けて保存(SAVE AS)で処理続行	*/
			return -2;
/* added 2016.08.31 hmenjo 結果データ勝手に削除 ---------- } ---------- */
		}
	}

	return -1; //処理続行
}



/////////////////////////////////////////////////////////////////////////////
//ファイル存在チェック
BOOL CMeasurementTabView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}



/////////////////////////////////////////////////////////////////////////////
//パス名から、ファイル名の開始位置を取得する
//パス名の末尾からパス名最後の '\' までを、ファイル名と判断する
char * CMeasurementTabView::GetFileName( const char *PathName )
{
	for( char *p = strtail(PathName); p >= PathName; p-- ) // 文字列の最後から最初まで
	{
		if ( ('\\'==*p) && !isJMS(PathName,p-PathName) )
			return p+1;
	}
	return (char *)PathName; // 文字列に '\' は含まれていなかった
}//GetFileName


/////////////////////////////////////////////////////////////////////////////
//文字列末尾の文字 '\0' の位置を返す
char * CMeasurementTabView::strtail( const char *string )
{
	return (char*)strchr( string, '\0' );
}//strtail


/////////////////////////////////////////////////////////////////////////////
//文字列中のある１バイトについて、１バイト文字か２バイト文字の一部であるかを判定する。
//*********************************************************
// 文字列 str の str[nPos] について、
//	 ０ …… １バイト文字
//	 １ …… ２バイト文字の一部（第１バイト）
//	 ２ …… ２バイト文字の一部（第２バイト）
// のいずれかを返す。
//*********************************************************
#define jms1(c) (((0x81 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0x9F)) || ((0xE0 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xFC) ))
#define jms2(c) ((0x7F != (unsigned char)(c)) && (0x40 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xFC))
int CMeasurementTabView::isJMS( const char *str, int nPos )
{
	int i;
	int state; // { 0, 1, 2 }

	// 0 = １バイト文字
	// 1 = ２バイト文字の第１バイト
	// 2 = ２バイト文字の第２バイト
	state = 0;
	for( i = 0; str[i] != '\0'; i++ )
	{
		if		( ( state == 0 ) && ( jms1( str[i] ) ) ) state = 1; // 0 -> 1
		else if ( ( state == 1 ) && ( jms2( str[i] ) ) ) state = 2; // 1 -> 2
		else if ( ( state == 2 ) && ( jms1( str[i] ) ) ) state = 1; // 2 -> 1
		else											 state = 0; // 2 -> 0, その他

		// str[nPos] での状態を返す。
		if ( i == nPos ) return state;
	}
	return 0;
}//isJMS



void CMeasurementTabView::OnUpdateSaveResultData(CCmdUI* pCmdUI)
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

	//2009.11.03 bagus MS --{--
	//顕微鏡測定は保存させない
	{
		RCP_DATA rcp;
		m_pDoc->GetRcpData(&rcp);
		if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
			pCmdUI->Enable(FALSE);
			return ;
		}
	}
	//2009.11.03 bagus MS --}--


	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //マニュアル測定モード
	{
		if(m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP)
		{
			pCmdUI->Enable(TRUE);
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}
	else //通常測定モード(リメジャー含む)
	{

		if(m_pDoc->GetProcessStatus() == PROCESS_WAIT)
		{
			pCmdUI->Enable(TRUE);
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

}


void CMeasurementTabView::OnSaveResultData()
{
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
	OnSaveAsMain(MEASUREMENT_TOOLBAR_SAVE);
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
/* deleted 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
//	  // TODO: この位置にコマンド ハンドラ用のコードを追加してください
//
////ツールバーのセーブボタン押下時の処理
//
//// Kojika 20090526 Add
//	  CString strBuffer, strTitle;
//// Kojika 20090526 Add End
//
////  BOOL bCheck;
//int iCheck=0;
//	  RCP_DATA rcp_data;
//	  memset(&rcp_data, 0, sizeof(rcp_data));
//	  m_pDoc->GetRcpData(&rcp_data);
//
//	  iCheck = SaveMeasConAddRevi(rcp_data.szRecipeName, (int)MEASUREMENT_TOOLBAR_SAVE);
//
//	  //Saiki 20090406 Change ----->
////	if(iCheck != 1)
////	{
//////		MessageBox("Save UnCompleted.", "", MB_OK); //セーブ失敗
////		MessageBox("保存失敗", "NanoSpec", MB_OK); //セーブ失敗
////	}
////  else
////  {
////	  m_bToolbarSaveCompFlg = TRUE; //ツールバーで保存済
////  }
//	  if(iCheck != 1 && iCheck != 3){ //保存失敗
//		  AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //測定結果保存失敗
//		  if( m_pDoc->GetHostMode() == HOST_LOCAL ){
//// Kojika 20090526 Change
////		  MessageBox("Save UnCompleted.", "NanoSpec", MB_OK); //セーブ失敗
////		  MessageBox("保存に失敗しました", "NanoSpec", MB_OK); //セーブ失敗
//			  LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
//			  LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
//			  MessageBox(strBuffer, strTitle, MB_OK); //セーブ失敗
//// Kojika 20090526 Change END
//		  }
//	  }
//	  else if(iCheck == 3){//保存キャンセル
//		   //保存キャンセルなので何もしない
//	  }
//	  else{//保存成功
//		  m_bToolbarSaveCompFlg = TRUE; //ツールバーで保存済
//	  }
//	  //Saiki 20090406 Change <-----
//
/* deleted 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
}





// ==========================================================================
//測定中⇔測定終了 時のボタン有効/無効の切替え
//通常測定・リメジャー専用。 マニュアル測定モードでの切替えはManualMeasurementFormView.cppに記述
void CMeasurementTabView::ButtonEnableChange(int Select)
{
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	WORD wNumScans;


	//リモート時は測定状態にかかわらず、ボタン押下制限をかける
	if(m_pDoc->GetHostMode() == HOST_REMOTE) //リモート時
	{
		if(m_pDoc->GetMeasMode() == MEASMODE_SEQ_OR_REMEASURE) //通常測定orリメジャー
		{
			//IDD_MAIN_MENU_FORM内コントロールの無効化
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE); //[LOG ON]ボタン無効

			//IDD_MEASUREMENT_PROCESS_FORMコントロールの無効化
//			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_CANCEL_BUTTON)->EnableWindow(FALSE); 	//[測定中止]ボタン無効
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->EnableWindow(FALSE);		//[測定中断(再開)]ボタン無効
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[REMEASUREMENT]ボタン無効
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);	//[位置補正後再測定]ボタン無効
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]ボタン無効

			//IDD_VIEW_WINDOW_FORM内コントロールの無効化
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//レンズ倍率選択無効
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//カメラ選択無効
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ボタン無効
			//Saiki 20090410 Add ----->
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ヘッド切替無効
			//Saiki 20090410 Add <-----
			//メニューバー内(IDR_MEASUREMENT_MENU)の無効化フラグ操作
			m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
		}
		return;
	}


	switch(Select){
	case MEASUREMENT_SEQ_S:

		//IDD_MEASUREMENT_PROCESS_FORMコントロールの無効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[REMEASUREMENT]ボタン無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);	//[位置補正後再測定]ボタン無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]ボタン無効

		//IDD_VIEW_WINDOW_FORM内コントロールの無効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//レンズ倍率選択無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//カメラ選択無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ボタン無効
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ヘッド切替無効
		//Saiki 20090410 Add <-----

		//メニューバー内(IDR_MEASUREMENT_MENU)の無効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg = FALSE;

		break;


	case MEASUREMENT_SEQ_E:

		//IDD_MEASUREMENT_PROCESS_FORMコントロールの有効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_CANCEL_BUTTON)->EnableWindow(TRUE);	//[測定中止]ボタン有効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->EnableWindow(TRUE);	//[測定中断(再開)]ボタン有効
		//2009.10.28 bagus 2point-distance --{--
		wNumScans = rcp_data.StageProgInfoHdr.wNumScans;
		if(rcp_data.StageProgInfoHdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.StageProgInfoHdr.wScanType == SCAN_TYPE_SR_DISTANCE){
			wNumScans *= 2;
		}
		//if(rcp_data.StageProgInfoHdr.wNumScans == m_pDoc->GetStoreCount()) //測定数が予定測定に到達している
		if(wNumScans == m_pDoc->GetStoreCount()) //測定数が予定測定に到達している
		//2009.10.28 bagus 2point-distance --}--
		{
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(TRUE);		//[REMEASUREMENT]ボタン有効
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(TRUE);//[位置補正後再測定]ボタン有効
		}
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);				//[EXIT]ボタン有効
		//2009.12.20 bagus 修正 --{--
		((CMeasurementProcessView *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0)))->ChangeOperationEnable(FALSE);
		//2009.12.20 bagus 修正 --}--

		//IDD_VIEW_WINDOW_FORM内コントロールの有効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//レンズ倍率選択有効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//カメラ選択有効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]ボタン有効
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE);	//ヘッド切替有効
		//Saiki 20090410 Add <-----

		//メニューバー内(IDR_MEASUREMENT_MENU)の有効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;


//	  case MEASUREMENT_TEST_S:
//		  break;

//	  case MEASUREMENT_TEST_E:
//		  break;


	case MEAS_TAB_VIEW_CREATE:
		//IDD_MAIN_MENU_FORM内コントロールの無効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(FALSE);		//[MEASUREMENT]ボタン無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(FALSE);	//[RECIPE]ボタン無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE);		//[LOG ON]ボタン無効

		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //マニュアル測定
		{
			//IDD_VIEW_WINDOW_FORM内コントロールの無効化
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//カメラ選択無効
		}


		//メニューバー内(IDR_MEASUREMENT_MENU)の無効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

		break;


	case MEAS_TAB_VIEW_DELETE:
		//IDD_MAIN_MENU_FORM内コントロールの有効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//[MEASUREMENT]ボタン有効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(TRUE);//[RECIPE]ボタン有効

		//メニューバー内(IDR_MEASUREMENT_MENU)の有効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg2 = TRUE;
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;
	}

}



void CMeasurementTabView::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	int iProcessStatus = m_pDoc->GetProcessStatus();
	int iHostMode = m_pDoc->GetHostMode();
	int iMeasMode = m_pDoc->GetMeasMode();	//測定モード取得
	int iManuMeasMode = m_pDoc->GetManuMeasMode();

	if((iProcessStatus != m_iProcessStatusOld)||(iHostMode != m_iHostModeOld))
	{
		switch (nIDEvent)
		{
			case TABVIEW_ENABLE_CHANGE_TIMER:		// 50ms 定周期タイマ

//				int iMeasMode = m_pDoc->GetMeasMode();	//測定モード取得
				if(iMeasMode == MEASMODE_SEQ_OR_REMEASURE) //通常測定orリメジャー
				{

					if((iProcessStatus == PROCESS_WAIT)||(iProcessStatus == PROCESS_DOWN))
					{
						ButtonEnableChange(MEASUREMENT_SEQ_E);
					}
					else
					{
						ButtonEnableChange(MEASUREMENT_SEQ_S);
					}
				}
				else if(iMeasMode == MEASMODE_MANUALMEASURE) //マニュアル測定
				{
					//マニュアル測定のボタン制御は、ManualMeasurementFormView.cppに記述

				}
//			  else if(iMeasMode == MEASMODE_TEST) //テストモード
//			  {
//
//			  }
				else
				{
					//処理なし
				}

				break;
		}
	}

	//前回値更新
	m_iProcessStatusOld = iProcessStatus;
	m_iHostModeOld = iHostMode;



	//前回値更新
	m_iManuMeasModeOld = iManuMeasMode;


	CNanoUI::OnTimer(nIDEvent);
}



int CMeasurementTabView::FolderCopy(LPCTSTR szFrom, LPCTSTR szTo )
{
	SHFILEOPSTRUCT tSHFile;

	ZeroMemory(&tSHFile, sizeof(SHFILEOPSTRUCT));

	CString from = szFrom;
	CString to = szTo;

	tSHFile.hwnd = m_hWnd;
	tSHFile.wFunc = FO_COPY;
	tSHFile.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SIMPLEPROGRESS ;

	tSHFile.fAnyOperationsAborted = TRUE;
	tSHFile.hNameMappings = NULL;
	tSHFile.lpszProgressTitle = NULL;

	from += "0";
	to += "0";
	from.SetAt( from.GetLength()-1, NULL );
	to.SetAt( to.GetLength()-1, NULL );

	tSHFile.pFrom = (LPCTSTR)from;
	tSHFile.pTo = (LPCTSTR)to;

	return SHFileOperation( &tSHFile ); // 実行

}



/////////////////////////////////////////////////////////////////////////////
// Make Directory
int CMeasurementTabView::MakeDirectory(char* dir)
{
	char buff[256];
	int i;
	SECURITY_ATTRIBUTES sa;
	int ret;

	sa.lpSecurityDescriptor = NULL;

	if((ret = CreateDirectory(dir,&sa)) == TRUE) return 1;
	for(i=strlen(dir);i>0;i--){
		if(dir[i] == '\\' || dir[i] == '/'){
			break;
		}
	}
	if(i <= 0) return 0;
	strcpy(buff,dir);
	buff[i]=NULL;
	if(ret = MakeDirectory(buff)){
		return (ret = CreateDirectory(dir,&sa)) == TRUE ? 1 : 0;
	}
	return 0;
}



void CMeasurementTabView::OnDestroy()
{
	CNanoUI::OnDestroy();

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	//ステータスバー文字クリア
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(0, "");

}

/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
void CMeasurementTabView::OnSaveAsMain(int iSaveWay)
{
	CString strBuffer, strTitle;

	int iCheck=0;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	//2009.11.03 bagus MS --{--
	//顕微鏡測定の場合には保存させない
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		iCheck = 1;
	}else{
		iCheck = SaveMeasConAddRevi(rcp_data.szRecipeName, iSaveWay);
	}
	//2009.11.03 bagus MS --}--


	if(iCheck != 1 && iCheck != 3){ //保存失敗
		AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //測定結果保存失敗
		if( m_pDoc->GetHostMode() == HOST_LOCAL ){
			LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK); //セーブ失敗
		}
	}
	else if(iCheck == 3){//保存キャンセル
			//保存キャンセルなので何もしない
	}
	else{//保存成功
		m_bToolbarSaveCompFlg = TRUE; //ツールバーで保存済
	}
}
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
void CMeasurementTabView::OnSaveAs()
{
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
	OnSaveAsMain(MEASUREMENT_TOOLBAR_SAVE_AS);
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
/* deleted 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
//	  //ツールバーのセーブボタン押下時と同様の処理
//
//// Kojika 20090526 Add
//	  CString strBuffer, strTitle;
//// Kojika 20090526 Add End
//
//	  int iCheck=0;
//	  RCP_DATA rcp_data;
//	  memset(&rcp_data, 0, sizeof(rcp_data));
//	  m_pDoc->GetRcpData(&rcp_data);
//
//	  iCheck = SaveMeasConAddRevi(rcp_data.szRecipeName, (int)MEASUREMENT_TOOLBAR_SAVE_AS);
//
//	  //Saiki 20090406 Change ----->
////	if(iCheck != 1){
////		MessageBox("保存失敗", "NanoSpec", MB_OK); //セーブ失敗
////	}
////	else{
////	  m_bToolbarSaveCompFlg = TRUE; //ツールバーで保存済
////  }
////  }
//	  if(iCheck != 1 && iCheck != 3){ //保存失敗
//		  AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //測定結果保存失敗
//		  if( m_pDoc->GetHostMode() == HOST_LOCAL ){
//// Kojika 20090526 Change
////				  MessageBox("Save UnCompleted.", "NanoSpec", MB_OK); //セーブ失敗
////		  MessageBox("保存に失敗しました", "NanoSpec", MB_OK); //セーブ失敗
//			  LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
//			  LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec")
//			  MessageBox(strBuffer, strTitle, MB_OK); //セーブ失敗
//// Kojika 20090526 Change END
//		  }
//	  }
//	  else if(iCheck == 3){//保存キャンセル
//		   //保存キャンセルなので何もしない
//	  }
//	  else{//保存成功
//		  m_bToolbarSaveCompFlg = TRUE; //ツールバーで保存済
//	  }
//	  //Saiki 20090406 Change <-----
/* deleted 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
}

void CMeasurementTabView::OnUpdateSaveAs(CCmdUI* pCmdUI)
{
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){ //マニュアル測定モード
		if(m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP){
			pCmdUI->Enable(TRUE);
		}
		else{
			pCmdUI->Enable(FALSE);
		}
	}
	else{ //通常測定モード(リメジャー含む)

		if(m_pDoc->GetProcessStatus() == PROCESS_WAIT){
			pCmdUI->Enable(TRUE);
		}
		else{
			pCmdUI->Enable(FALSE);
		}
	}
}

/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
/*
 *	初期化
 */
void CMeasurementTabView::MRCS_Initialize()
{
	memset(m_MRCS_dwSettings, 0, sizeof(m_MRCS_dwSettings));
	m_MRCS_wReMeasureNo = 0;
	m_MRCS_wIncludeColorSetNo = 0;
	m_MRCS_bIncludeColorSetList = FALSE;
	m_MRCS_bIncludeColorSetGraph = FALSE;
	m_MRCS_wShowCompletionPointSnapL = 0;
	m_MRCS_wShowCompletionPointSnapG = 0;
}
/*
 *	機能の実行可能条件をチェックします．
 *		DWORD dwMode
 *				＝０：全チェック
 *				＝１：再測定時用チェック
 *				上記以外：＝０と同じ
 */
BOOL CMeasurementTabView::MRCS_CheckPermission(DWORD dwMode/* = 0*/)
{
	BOOL l_bRc = FALSE;

	/* 手動モード以外は除外	*/
	if (m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) {
		return FALSE;
	}

	/* ストレス測定は除外	*/
	if (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS) {
		return FALSE;
	}

	/* 顕微鏡測定は除外	*/
	if (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		return FALSE;
	}

	/* SR_DISTANCE は除外	*/
	if (m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE) {
		return FALSE;
	}

	/* GTR は再測定のみ除外	*/
	if ((dwMode == 1) &&
		(m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)) {
		return FALSE;
	}

	return TRUE;
}
/*
 *	機能の右ダブルクリックの実行可能条件をチェックします．
 */
BOOL CMeasurementTabView::MRCS_CheckRunPermission()
{
	BOOL l_bRc = FALSE;

	/* 手動モード以外は除外	*/
	if (m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) {
		return FALSE;
	}
/* added 2016.06.02 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
	/* 手動モードで測定完了は除外	*/
	if (m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP) {
		return FALSE;
	}
/* added 2016.06.02 hmenjo 手動 再測定/指定統計 ---------- } ---------- */

	if (FALSE != m_pMainFrame->m_ManualMeasMenuEnableFlg) {
		l_bRc = TRUE;
	}

	return l_bRc;
}
/*
 *	右ダブルクリック処理
 */
BOOL CMeasurementTabView::MRCS_RDblClk(POINT ptGrid, CGridCtrl* pclsGrid/* = 0*/)
{
	BOOL l_bRc = FALSE;

	if (0 == pclsGrid) {
		return FALSE;
	}

	CGridCtrl* l_pclsGrid = pclsGrid;

	if (FALSE != this->MRCS_CheckRunPermission()) {
		if (FALSE != this->MRCS_CheckPermission()) {
			CCellID l_clsCellID = l_pclsGrid->GetCellFromPt(ptGrid, FALSE);
			if (FALSE != l_clsCellID.IsValid()) {
				/* クリックしたセルは有効	*/
				int l_iMeasCount = m_pDoc->GetMeasurePointCount();
				if (l_clsCellID.row <= l_iMeasCount) {
					/* クリック行 No. が測定数以内	*/
					WORD l_wClickLine = l_clsCellID.row;
					/* 有効な行での選択状態の行数を取得	*/
//	テスト用		int l_iSelCnt = m_MeasurementListGrid.GetSelectedCount();
//	テスト用		if (FALSE != m_MeasurementListGrid.IsCellSelected(l_clsCellID)) {
					int l_iSelCount = 0;
					BOOL l_bIncludedInSelected = FALSE;	/* 選択行中に指定行が在るフラグ	*/
					for (int i = 1; i <= l_iMeasCount; i++) {
						if (FALSE != l_pclsGrid->IsCellSelected(i, l_clsCellID.col)) {
							/* 選択行	*/
							l_iSelCount++;
							if (1 == this->MRCS_SettingGet(i)) {
								l_bIncludedInSelected = TRUE;
							}
							m_MRCS_dwSettings[i] |= 0x80000000;
						} else {
							/* 非選択行	*/
							m_MRCS_dwSettings[i] &= 0x7fffffff;
						}
					}
					/* 可能な操作の判定	*/
					DWORD l_dwSecondCheckID = 0;	/* 条件判定 ID	*/
					switch (l_iSelCount) {
					case 0:		/* 選択状態無し	*/
						l_dwSecondCheckID = 2;
						break;
					case 1:		/* １行選択	*/
						if (FALSE != l_pclsGrid->IsCellSelected(l_clsCellID)) {
							/* クリック行が選択状態	*/
							l_dwSecondCheckID = 3;
						} else {
							/* クリック行が非選択状態	*/
							l_dwSecondCheckID = 2;
						}
						break;
					default:	/* 複数行選択	*/
						if (FALSE != l_pclsGrid->IsCellSelected(l_clsCellID)) {
							/* クリック行が選択状態	*/
							l_dwSecondCheckID = 1;
						} else {
							/* クリック行が非選択状態	*/
							l_dwSecondCheckID = 2;
						}
						break;
					}
					WORD l_wEnableSelect = 0;
					switch (l_dwSecondCheckID) {
					case 1:
						if (FALSE != l_bIncludedInSelected) {
							/* 選択状態の行に指定行が在る	*/
							l_wEnableSelect |= 0x0001;	/* 指定行	*/
							l_wEnableSelect |= 0x0002;	/* 指定解除	*/
						} else {
							/* 選択状態の行に指定行が無い	*/
							l_wEnableSelect |= 0x0001;	/* 指定行	*/
						}
						break;
					case 2:
						if (2 != this->MRCS_SettingGet(l_clsCellID.row)) {
							/* クリック行が削除済み行でない	*/
							if (1 < (l_iMeasCount - MRCS_DeleteCountGet())) {
								/* 非削除済み行が２以上の場合のみ削除可能です．	*/
								l_wEnableSelect |= 0x0004;	/* 削除	*/
							}
							if (FALSE != this->MRCS_CheckPermission(1)) {
								l_wEnableSelect |= 0x0010;	/* 再測定	*/
							}
						} else {
							/* クリック行が削除済み行である	*/
							l_wEnableSelect |= 0x0008;	/* 削除復活	*/
							if (FALSE != this->MRCS_CheckPermission(1)) {
								l_wEnableSelect |= 0x0010;	/* 再測定	*/
							}
						}
						break;
					case 3:
						if ((1 != this->MRCS_SettingGet(l_clsCellID.row)) &&
							(2 != this->MRCS_SettingGet(l_clsCellID.row))) {
							/* クリック行が指定行でなく，削除済み行でもない	*/
							l_wEnableSelect |= 0x0001;	/* 指定行	*/
						} else if (1 == this->MRCS_SettingGet(l_clsCellID.row)) {
							/* クリック行が指定行である	*/
							l_wEnableSelect |= 0x0002;	/* 指定解除	*/
						}
						break;
					default:
						/* 何もしない	*/
						break;
					}
					if (0 < MRCS_DeleteCountGet()) {
						/* 削除済み行が在る	*/
						l_wEnableSelect |= 0x1000;	/* 削除済み行在り	*/
					}
					if (0 < MRCS_IncludeCountGet()) {
						/* 指定行が在る	*/
						l_wEnableSelect |= 0x2000;	/* 指定行在り	*/
					}
					if (0 != l_wEnableSelect) {
						/* 操作画面を表示	*/
						l_bRc = TRUE;
						CManuReMeasReCalcStatDlg l_dlgMRCS;
						l_dlgMRCS.ParamSet(MAKELPARAM(l_wEnableSelect, l_wClickLine));
						if (IDOK == l_dlgMRCS.DoModal()) {
							/* OK	*/
							DWORD l_dwRslt;
							l_dlgMRCS.ParamGet((LPARAM*) &l_dwRslt);
							WORD l_wSelectNo = LOWORD(l_dwRslt);
							BOOL l_bCalcStat = FALSE;	/* 統計計算必要	*/
							switch (l_wSelectNo) {
							case 1:		/* 指定行	*/
							case 2:		/* 指定解除	*/
								l_bCalcStat = TRUE;
								for (i = 1; i <= l_iMeasCount; i++) {
									if (0 != (m_MRCS_dwSettings[i] & 0x80000000)) {
										/* 選択行	*/
										switch (l_wSelectNo) {
										case 1:		/* 指定行	*/
											if (2 != this->MRCS_SettingGet(i)) {
												/* 削除済み行以外	*/
												this->MRCS_SettingSet(i, 1);
												this->MRCS_GridLineColorSet(i, CYAN_COLOR);
												this->MRCS_IncludeCountInc();
											}
											break;
										case 2:		/* 指定解除	*/
											if (1 == this->MRCS_SettingGet(i)) {
												/* 指定行	*/
												this->MRCS_SettingSet(i, 0);
												this->MRCS_GridLineColorSet(i, CLR_DEFAULT);
												this->MRCS_IncludeCountDec();
											}
											break;
										}
									}
								}
								break;
							case 3:		/* 削除	*/
								l_bCalcStat = TRUE;
								if (1 == this->MRCS_SettingGet(l_wClickLine)) {
									/* 指定行だった場合はデクリメントします．	*/
									this->MRCS_IncludeCountDec();
								}
								this->MRCS_SettingSet(l_wClickLine, 2);
								this->MRCS_GridLineColorSet(l_wClickLine, MAGENTA_COLOR);
								this->MRCS_DeleteCountInc();
								break;
							case 4:		/* 削除復活	*/
								if (0 == MRCS_IncludeCountGet()) {
									/* 指定行が無い場合のみ統計計算します．	*/
									l_bCalcStat = TRUE;
								}
								this->MRCS_SettingSet(l_wClickLine, 0);
								this->MRCS_GridLineColorSet(l_wClickLine, CLR_DEFAULT);
								this->MRCS_DeleteCountDec();
								break;
							case 5:		/* 再測定	*/
								m_MRCS_wReMeasureNo = l_wClickLine;
								if (2 == this->MRCS_SettingGet(m_MRCS_wReMeasureNo)) {
									/* 削除済み行でした	*/
									this->MRCS_DeleteCountDec();
									this->MRCS_SettingSet(m_MRCS_wReMeasureNo, 0);
									this->MRCS_GridLineColorSet(m_MRCS_wReMeasureNo, CLR_DEFAULT);
								}
								m_pMainFrame->m_pManuMeaFormViewObj->ManuMeas();
								break;
							case 13:	/* すべての削除データを復活	*/
							case 14:	/* すべての指定行を解除	*/
								{
									WORD l_wDiffPrm;
									switch (l_wSelectNo) {
									case 13:	/* すべての削除データを復活	*/
										if (0 == MRCS_IncludeCountGet()) {
											/* 指定行が無い場合のみ統計計算します．	*/
											l_bCalcStat = TRUE;
										}
										l_wDiffPrm = 2;
										m_MRCS_dwSettings[0] &= 0xffff0000;
										break;
									case 14:	/* すべての指定行を解除	*/
										l_bCalcStat = TRUE;
										l_wDiffPrm = 1;
										m_MRCS_dwSettings[0] &= 0x0000ffff;
										break;
									}
									for (i = 1; i <= l_iMeasCount; i++) {
										if (l_wDiffPrm == this->MRCS_SettingGet(i)) {
											this->MRCS_SettingSet(i, 0);
											this->MRCS_GridLineColorSet(i, CLR_DEFAULT);
										}
									}
								}
								break;
							default:
								break;
							}
							/* 選択状態を解除	*/
							for (i = 1; i <= SCAN_POINT_MAX; i++) {
								m_MRCS_dwSettings[i] &= 0x7fffffff;
							}
							m_pMeasurementListDlg->MeasurementListGrid_UnSelectAll();
							m_pMeasurementGraphDlg->MeasurementListGrid_UnSelectAll();
//	選択状態にはしないことにします．
//							if (5 == l_wSelectNo) {
//								/* 再測定後はその行を選択状態にします．	*/
//								m_MRCS_dwSettings[l_wClickLine] |= 0x80000000;
//							}
							/* 統計計算	*/
							if (FALSE != l_bCalcStat) {
								STATISTICS l_Statistics[ADAPRESULT_COLS_MAX];	/* 統計データ	*/
								memset(l_Statistics, 0, sizeof(l_Statistics));
								/* 統計計算	*/
								m_pMainFrame->GlassDataStatistics(
										l_Statistics,
										m_pDoc->GetDispLabelCount(),
										m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption
									);
								/* 統計データ保存	*/
								m_pDoc->SetStatisticsData(l_Statistics);
								/* 統計データ表示	*/
//	不要です．							m_pDoc->SetShowCompPointGraph(l_iMeasCount - 1);
								/*		統計データの表示は CMeasurementListDlg のみで行っているため
										CMeasurementGraphDlg 側は不要です．	*/
								m_pDoc->SetShowCompPointList(l_iMeasCount - 1);
							}
						} else {
							/* Cancel	*/
							/*	処理は何もありません．	*/
						}
					}
				}
			}
		}
	}

	return l_bRc;
}
/*
 *	設定バッファから設定のみ取り出す
 */
WORD CMeasurementTabView::MRCS_SettingGet(long lIndex)
{
	return LOWORD(m_MRCS_dwSettings[lIndex]);
}
/*
 *	設定バッファに設定を書込む
 */
void CMeasurementTabView::MRCS_SettingSet(long lIndex, WORD wSetting)
{
	WORD l_wStatus = HIWORD(m_MRCS_dwSettings[lIndex]);
	m_MRCS_dwSettings[lIndex] = (DWORD) MAKELONG(wSetting, l_wStatus);
}
/*
 *	設定バッファから状態のみ取り出す
 */
WORD CMeasurementTabView::MRCS_SettingStatusGet(long lIndex)
{
	return HIWORD(m_MRCS_dwSettings[lIndex]);
}
/*
 *	設定バッファに状態のみ書込む
 */
void CMeasurementTabView::MRCS_SettingStatusSet(long lIndex, WORD wStatus)
{
	WORD l_wSetting = LOWORD(m_MRCS_dwSettings[lIndex]);
	m_MRCS_dwSettings[lIndex] = (DWORD) MAKELONG(l_wSetting, wStatus);
}
/*
 *	設定バッファから全情報を取り出す
 */
DWORD CMeasurementTabView::MRCS_SettingAllGet(long lIndex)
{
	return m_MRCS_dwSettings[lIndex];
}
/*
 *	指定リスト行の色設定
 */
void CMeasurementTabView::MRCS_GridLineColorSet(long lIndex, COLORREF cr/* = CLR_DEFAULT*/)
{
	/* CMeasurementListDlg	*/
	for (int i = 1; i < m_pMeasurementListDlg->m_MeasurementListGrid.GetColumnCount(); i++) {
		m_pMeasurementListDlg->m_MeasurementListGrid.SetItemBkColour(lIndex, i, cr);
	}
	/* CMeasurementGraphDlg	*/
	for (i = 1; i < m_pMeasurementGraphDlg->m_MeasurementListGrid.GetColumnCount(); i++) {
		m_pMeasurementGraphDlg->m_MeasurementListGrid.SetItemBkColour(lIndex, i, cr);
	}
}
/*
 *	削除済み行数カウンタを取得
 */
WORD CMeasurementTabView::MRCS_DeleteCountGet()
{
	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	削除済み行数カウンタを加算
 */
WORD CMeasurementTabView::MRCS_DeleteCountInc(WORD wCount/* = 1*/)
{
	WORD l_wCount = LOWORD(m_MRCS_dwSettings[0]);
	l_wCount += wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(l_wCount, HIWORD(m_MRCS_dwSettings[0]));

	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	削除済み行数カウンタを減算
 */
WORD CMeasurementTabView::MRCS_DeleteCountDec(WORD wCount/* = 1*/)
{
	WORD l_wCount = LOWORD(m_MRCS_dwSettings[0]);
	l_wCount -= wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(l_wCount, HIWORD(m_MRCS_dwSettings[0]));

	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	指定行数カウンタを取得
 */
WORD CMeasurementTabView::MRCS_IncludeCountGet()
{
	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	指定行数カウンタを加算
 */
WORD CMeasurementTabView::MRCS_IncludeCountInc(WORD wCount/* = 1*/)
{
	WORD l_wCount = HIWORD(m_MRCS_dwSettings[0]);
	l_wCount += wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(LOWORD(m_MRCS_dwSettings[0]), l_wCount);

	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	指定行数カウンタを減算
 */
WORD CMeasurementTabView::MRCS_IncludeCountDec(WORD wCount/* = 1*/)
{
	WORD l_wCount = HIWORD(m_MRCS_dwSettings[0]);
	l_wCount -= wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(LOWORD(m_MRCS_dwSettings[0]), l_wCount);

	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	指定行を追加
 */
WORD CMeasurementTabView::MRCS_IncludeAdd(long lIndex)
{
	WORD l_wRc = this->MRCS_IncludeCountGet();

	if (0 == this->MRCS_SettingGet(lIndex)) {
		/* 指定が何も無い場合のみ設定します．	*/
		this->MRCS_SettingSet(lIndex, 1);
		l_wRc = this->MRCS_IncludeCountInc();
	}

	return l_wRc;
}
/*
 *	再測定 No. の取得
 */
WORD CMeasurementTabView::MRCS_ReMeasureNoGet()
{
	return m_MRCS_wReMeasureNo;
}
/*
 *	再測定 No. をクリア
 */
void CMeasurementTabView::MRCS_ReMeasureNoClr()
{
	m_MRCS_wReMeasureNo = 0;
}
/*
 *	指定行色番号の設定
 */
void CMeasurementTabView::MRCS_IncludeColorSetNoSet(WORD wNo)
{
	m_MRCS_wIncludeColorSetNo = wNo;
}
/*
 *	指定行色番号の取得
 */
WORD CMeasurementTabView::MRCS_IncludeColorSetNoGet()
{
	return m_MRCS_wIncludeColorSetNo;
}
/*
 *	指定行色設定
 */
#define	MRCSREDRAWMETHOD	1	/* 再描画方法	*/
void CMeasurementTabView::MRCS_IncludeColorSet(WORD wNo, BOOL bList, BOOL bGraph)
{
	if (0 == wNo) {
		m_MRCS_wIncludeColorSetNo = 0;
	}
	if ((FALSE == bList) && (FALSE == bGraph)) {
		m_MRCS_bIncludeColorSetList = FALSE;
		m_MRCS_bIncludeColorSetGraph = FALSE;
	}
	if ((0 == wNo) ||
		((FALSE == bList) && (FALSE == bGraph))) {
		return;
	}

	if (FALSE != bList) {
		m_MRCS_bIncludeColorSetList = TRUE;
	}
	if (FALSE != bGraph) {
		m_MRCS_bIncludeColorSetGraph = TRUE;
	}
	if ((FALSE != m_MRCS_bIncludeColorSetList) &&
		(FALSE != m_MRCS_bIncludeColorSetGraph)) {
		/* 両方設定完了	*/
		WORD l_wSetNo = m_MRCS_wIncludeColorSetNo;
		m_MRCS_bIncludeColorSetList = FALSE;
		m_MRCS_bIncludeColorSetGraph = FALSE;
		m_MRCS_wIncludeColorSetNo = 0;
		this->MRCS_GridLineColorSet(l_wSetNo, CYAN_COLOR);
		/*	色設定後の反映が，すぐにされませんでしたので，
			再描画(？)処理を入れています．	*/
		/* グリッド表示更新	*/
#if		MRCSREDRAWMETHOD == 1
		m_pMeasurementListDlg->m_MeasurementListGrid.Refresh();
		m_pMeasurementGraphDlg->m_MeasurementListGrid.Refresh();
#elif	MRCSREDRAWMETHOD == 2
		m_pMeasurementListDlg->m_MeasurementListGrid.RedrawRow(lIndex);
		m_pMeasurementGraphDlg->m_MeasurementListGrid.RedrawRow(lIndex);
#elif	MRCSREDRAWMETHOD == 3
		m_pMeasurementListDlg->m_MeasurementListGrid.EnsureVisible(lIndex, 1);
		m_pMeasurementGraphDlg->m_MeasurementListGrid.EnsureVisible(lIndex, 1);
#endif
	}
}
/*
 *	結果リストの表示済みポイントの記憶，取得，クリア
 */
BOOL CMeasurementTabView::MRCS_ShowCompletionPointSnap(LPWORD pwList/* = 0*/, LPWORD pwGraph/* = 0*/)
{
	BOOL l_bRc = FALSE;

	if ((0 == pwList) && (0 == pwGraph)) {
		/* クリア	*/
		m_MRCS_wShowCompletionPointSnapL = 0;
		m_MRCS_wShowCompletionPointSnapG = 0;
	} else
	if ((0 != pwList) && (0 != pwGraph)) {
		/* 記憶	*/
		m_MRCS_wShowCompletionPointSnapL = m_pDoc->GetShowCompPointList();
		m_MRCS_wShowCompletionPointSnapG = m_pDoc->GetShowCompPointGraph();
		*pwList = m_MRCS_wShowCompletionPointSnapL;
		*pwGraph = m_MRCS_wShowCompletionPointSnapG;
		l_bRc = TRUE;
	} else
	if (0 != pwList) {
		/* CMeasurementListDlg 側取得	*/
		*pwList = m_MRCS_wShowCompletionPointSnapL;
		m_MRCS_wShowCompletionPointSnapL = 0;
		l_bRc = TRUE;
	} else
	if (0 != pwGraph) {
		/* CMeasurementGraphDlg 側取得	*/
		*pwGraph = m_MRCS_wShowCompletionPointSnapG;
		m_MRCS_wShowCompletionPointSnapG = 0;
		l_bRc = TRUE;
	}

	return l_bRc;
}
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- } ---------- */
