// MeasurementTabView.cpp : ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½tï¿½@ï¿½Cï¿½ï¿½
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
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
#include <Dlgs.h>
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
//2009.12.23 bagus --{--
#include "MeasurementProcessView.h"
//2009.12.23 bagus --}--
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
#include "ManuReMeasReCalcStatDlg.h"
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
#define	COLUMN_LENGTH		(12)
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

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
// ï¿½fï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½gï¿½Ì’ï¿½ï¿½ï¿½DestroyWindowï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½È‚ï¿½
//		m_pMeasurementListDlg->DestroyWindow();
		delete m_pMeasurementListDlg;
		m_pMeasurementListDlg = NULL;
	}

	if ( m_pMeasurementGraphDlg != NULL ) {
// ï¿½fï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½gï¿½Ì’ï¿½ï¿½ï¿½DestroyWindowï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½È‚ï¿½
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
// CMeasurementTabView ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½

// =========================================================================
//
void CMeasurementTabView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
	//ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½tï¿½Bï¿½Oï¿½İ’ï¿½Ç‚İ�ï¿½ï¿½ï¿½
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--

	///// Edit Tool Bar /////
	if(!m_wndMeasurementBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndMeasurementBar.LoadToolBar(IDR_MEASUREMENT_BAR)){
		TRACE0("Failed to create toolbar\n");
		return;		// ï¿½ì�¬ï¿½É�ï¿½ï¿½s
	}

	RECT rect;
	GetClientRect(&rect);
	m_wndMeasurementBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	///// Get Selected Recipe Name /////
	//2009.09.29 bagus se --{-- del
	//m_pMainFrame->GetSelectListName(m_szName);
	//2009.09.29 bagus se --{--

/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
	this->MRCS_Initialize();
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */

	///// Load Data /////
	LoadData();

	///// Tab	/////
	Tab_Init();

/* added 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- { ---------- */
	RECT l_rectThisView;	this->GetWindowRect(&l_rectThisView);
	RECT l_rectMeasTab;		m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
	if ((g_lModelType == MODEL_T3100) && (l_rectThisView.bottom < l_rectMeasTab.bottom)) {
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
		/* ï¿½^ï¿½uï¿½Ì’ï¿½Ó‚ï¿½ï¿½Ú“ï¿½	*/
		m_MeasurementTab.SetWindowPos(	0,
							0, 0,
							l_rectMeasTab.right - l_rectMeasTab.left,
							l_rectThisView.bottom - l_rectMeasTab.top,
							SWP_NOZORDER | SWP_NOMOVE);
	}
/* added 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- } ---------- */

	///// List	/////
	int dx = 5;
	int dy = 24;
	m_pMeasurementListDlg = new CMeasurementListDlg();
	m_pMeasurementListDlg->Create(CMeasurementListDlg::IDD, &m_MeasurementTab);
	m_pMeasurementListDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Graph /////
//	m_pMeasurementGraphDlg = new CMeasurementGraphDlg();
	if( m_pDoc->GetDispStatus() == MAIN_MENU_MANUAL_MEASUREMENT ){
		// ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pMeasurementGraphDlg = new CMeasurementGraphDlg(GRAPHDLG_MODE_MANUAL);
	}
	else{
		// ï¿½Ê�í‘ªï¿½ï¿½
		m_pMeasurementGraphDlg = new CMeasurementGraphDlg(GRAPHDLG_MODE_SEQ);
	}
	m_pMeasurementGraphDlg->Create(CMeasurementGraphDlg::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Set Current Selection Item (Default Select Tab) ////
	int Index;
	Index = m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay;
	//2009.10.13 bagus CTA --{--
	//ï¿½Oï¿½ï¿½ï¿½tï¿½^ï¿½uï¿½Ì‚È‚ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½Í”Oï¿½Ì‚ï¿½ï¿½ß‚É‹ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½LISTï¿½^ï¿½uï¿½ï¿½ï¿½fï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½É•Ï�Xï¿½ï¿½ï¿½ï¿½
	if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
	//2009.11.03 bagus MS --{--
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS
	//2009.11.03 bagus MS --}--
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR && m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE)
	// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ -->
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE)
	// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ <--
	){
		Index = 0;
	}
	//2009.10.13 bagus CTA --}--
	TabChange(Index);
	m_MeasurementTab.SetCurSel(Index);


	m_bToolbarSaveCompFlg = TRUE; //ï¿½ï¿½ï¿½è‚ªï¿½sï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½ï¿½ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½lï¿½ï¿½TRUE(ï¿½Û‘ï¿½ï¿½ï¿½)ï¿½Æ‚ï¿½ï¿½ï¿½B
	m_bExitSaveCompFlg = TRUE; //ï¿½ï¿½ï¿½è‚ªï¿½sï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½ï¿½ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½lï¿½ï¿½TRUE(ï¿½Û‘ï¿½ï¿½ï¿½)ï¿½Æ‚ï¿½ï¿½ï¿½B
	m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
	m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

	m_iProcessStatusOld = -1;
	m_iHostModeOld = -1;
	m_iManuMeasModeOld = -1;

	SetTimer(TABVIEW_ENABLE_CHANGE_TIMER, 50, NULL);
	if(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE)  //ï¿½Ê�í‘ªï¿½ï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[
	{
		ButtonEnableChange(MEAS_TAB_VIEW_CREATE);
	}
	m_pMainFrame->m_pMeaTabViewObj = this;

	::SetEvent(m_pMainFrame->m_hEvTabView);


	//2009.09.29 bagus se --{--
	//2009.11.30 bagus ï¿½Cï¿½ï¿½ --{--
	//SRï¿½Å‚È‚ï¿½ï¿½Æ‚ï¿½ï¿½è“®ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Í‚ï¿½ï¿½ï¿½ï¿½ï¿½Processingï¿½ÉˆÚ�sï¿½ï¿½ï¿½ï¿½
	//if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR){
	//2009.11.30 bagus ï¿½Cï¿½ï¿½ --}--
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)  //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		{
			m_pDoc->SetProcessStatus(PROCESS_PROC);
			//2009.10.30 bagus 2point-distance
			//2ï¿½_ï¿½Ô‹ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Íƒï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½gï¿½pï¿½ï¿½ï¿½È‚ï¿½ï¿½Ì‚Å�æ“¾ï¿½ï¿½ï¿½È‚ï¿½
			if(
			//2009.11.30 bagus ï¿½Cï¿½ï¿½ --{--
			//m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_DISTANCE
			(m_MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR &&
				m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_DISTANCE)
			//2009.11.30 bagus ï¿½Cï¿½ï¿½ --}--
			//2009.11.03 bagus MS --{--
			&& m_MainRcpInfo.MainRcpParam.hdr.wHeadType != HEAD_TYPE_MS
			//2009.11.03 bagus MS --}--
			//2010.01.15 bagus GTR --{--
			//ï¿½Kï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ß—ï¿½ï¿½Ì�ê�‡ï¿½É‚Íƒï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½Ìƒ^ï¿½Cï¿½~ï¿½ï¿½ï¿½Oï¿½Å�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
			&& (m_MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR &&
				m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
			//2010.01.15 bagus GTR --}--
			){
				m_pMainFrame->m_pManuMeaFormViewObj->GetReferenceManuStart();
			}else{
				//2009.12.29 bagus ï¿½Cï¿½ï¿½ --{--
				//ï¿½è“®ï¿½ï¿½ï¿½ï¿½ÍŒï¿½ï¿½XGetReferenceManuStart()ï¿½Å‘ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½ï¿½ï¿½é‚ª
				//ï¿½ï¿½ï¿½ï¿½ï¿½É—ï¿½ï¿½ï¿½ê�‡ï¿½É‚Íƒï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½È‚ï¿½ï¿½Ì‚Å‚ï¿½ï¿½ï¿½ï¿½ÅƒZï¿½bï¿½gï¿½ï¿½ï¿½ï¿½B
				///// ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½ï¿½æ“¾ /////
				SYSTEMTIME syStartDateTime;
				GetLocalTime(&syStartDateTime);
				m_pDoc->SetStartDateTime(&syStartDateTime);
				//2009.12.29 bagus ï¿½Cï¿½ï¿½ --}--
			}
			//2ï¿½_ï¿½Ô‹ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Íƒï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½gï¿½pï¿½ï¿½ï¿½È‚ï¿½ï¿½Ì‚Å�æ“¾ï¿½ï¿½ï¿½È‚ï¿½
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
	//m_MeasurementTab.InsertItem(0, "ï¿½ï¿½ï¿½Xï¿½g");
	//m_MeasurementTab.InsertItem(1, "ï¿½Oï¿½ï¿½ï¿½t");
	CString strBuffer;
	LoadStringML(IDS_TAB_LIST, strBuffer, "List");
	m_MeasurementTab.InsertItem(0, strBuffer);
	//2009.10.13 bagus CTA --{--
	//ï¿½Oï¿½ï¿½ï¿½tï¿½^ï¿½uï¿½Ì‚È‚ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½É‘Î‰ï¿½
	if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
	//2009.11.03 bagus MS --{--
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS
	//2009.11.03 bagus MS --}--
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR && m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE)
	// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ -->
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE)
	// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ <--
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
//ï¿½ï¿½ï¿½gï¿½p
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

	//ï¿½ï¿½ï¿½İ‚ÌƒXï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½æ“¾
	//ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½Í‘ï¿½ï¿½è–ˆï¿½É�ï¿½ï¿½Wï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Oï¿½ï¿½ï¿½tï¿½Ö’Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B
	//m_pMeasurementListDlg->DrawPointMarker(pPosition.lX, pPosition.lY, MEASUREMENT_STATES_START);
	return 0L;
}





//===========================================================================================
//===========================================================================================
//===========================================================================================
/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConAddRevi
// Purpose		: ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚Ç‚Ì•Û‘ï¿½(ï¿½Ç‰ï¿½ï¿½Eï¿½Cï¿½ï¿½)
// ï¿½ß‚ï¿½l:	1:ï¿½ï¿½ï¿½ï¿½
//			2:ï¿½ï¿½ï¿½s(ï¿½Û‘ï¿½ï¿½ï¿½ï¿½È‚ï¿½)
//			3:ï¿½ï¿½ï¿½s(ï¿½Û‘ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½)
//ï¿½@ï¿½@ï¿½@ï¿½@ 0:ï¿½ï¿½ï¿½s
int CMeasurementTabView::SaveMeasConAddRevi(LPCTSTR pszMainRcpName, int iSaveWay)
{
	char szSampleId[SAMPLE_ID_LEN + 1];
	//2009.10.13 bagus RS --{--
	//ï¿½ï¿½ï¿½bï¿½gIDï¿½Ì’Ç‰ï¿½
	char szLotId[LOT_ID_LEN + 1];
	//2009.10.13 bagus RS --}--
	char szSaveFullFilePath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szSaveFullDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szRefFilePath[MAX_PATH];
	char szToFilePath[MAX_PATH];
	char szFullPathFrom[SAVE_FILE_PATH_NAME_LEN+1]; //ï¿½ï¿½Îƒpï¿½Xï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½ï¿½ï¿½pï¿½X
	char szFullPathTo[SAVE_FILE_PATH_NAME_LEN+1];//ï¿½ï¿½Îƒpï¿½Xï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½ï¿½ï¿½pï¿½X
	char szCopyFrom[SAVE_FILE_PATH_NAME_LEN+1];
	char szCopyTo[SAVE_FILE_PATH_NAME_LEN+1];
	static char szMoveFrom[SAVE_FILE_PATH_NAME_LEN+1];
	static char szMoveTo[SAVE_FILE_PATH_NAME_LEN+1];
	char szDeleteDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDeleteFilePath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDirName[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDirName2[SAVE_FILE_PATH_NAME_LEN + 1]; //ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½(ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½qï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
	char szReNameDir[SAVE_FILE_PATH_NAME_LEN + 1];
	char szMessage[255+1];
	RCP_DATA rcp_data;
	BOOL bSampleID;
	BOOL bSaveMeasResult;
	BOOL bSaveOverwrite;
	BOOL bFileNameExistFlg = FALSE; //ï¿½Û‘ï¿½ï¿½ï¿½ï¿½æ‚¤ï¿½Æ‚ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½é‚©ï¿½Ìƒtï¿½ï¿½ï¿½O(TRUE:ï¿½ï¿½ï¿½İ�AFALSE:ï¿½È‚ï¿½)
	int iRtn=0; //ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½{ï¿½bï¿½Nï¿½Xï¿½ß‚ï¿½lï¿½p
	//2009.10.17 bagus stress --{--
	char szCheckLetters[] = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//2009.10.17 bagus stress --{--

	memset(szSampleId, 0, sizeof(szSampleId));
	//2009.10.13 bagus RS --{--
	//ï¿½ï¿½ï¿½bï¿½gIDï¿½Ì’Ç‰ï¿½
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

	m_pDoc->GetSampleId(szSampleId); //ï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½IDï¿½Ì�æ“¾
	//2009.10.13 bagus RS --{--
	//ï¿½ï¿½ï¿½bï¿½gIDï¿½Ì’Ç‰ï¿½
	m_pDoc->GetLotId(szLotId); 	//ï¿½ï¿½ï¿½bï¿½gIDï¿½Ì�æ“¾
	//2009.10.13 bagus RS --}--
	m_pDoc->GetRcpData(&rcp_data);

	bSampleID = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSampleID; //ï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½IDï¿½ï¿½ï¿½gï¿½pï¿½H
	bSaveMeasResult = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult; //ï¿½Zï¿½[ï¿½uï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ÉƒTï¿½ï¿½ï¿½vï¿½ï¿½IDï¿½ï¿½ï¿½gï¿½p?(bSampleID==TRUEï¿½ï¿½ï¿½Ì‚İ—Lï¿½ï¿½)
	bSaveOverwrite = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite; //ï¿½Zï¿½[ï¿½uï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Aï¿½ã�‘ï¿½ï¿½ï¿½Â�H
	//2009.12.17 bagus ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì•Ï�X --{--
	BOOL bSaveNameWithDate = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate; //ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½É“ï¿½ï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½
	//2009.12.17 bagus ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì•Ï�X --}--
	//2009.12.17 bagus ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì•Ï�X --{--
	if(m_pDoc->GetHostMode() == HOST_REMOTE){
		//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í�ï¿½ÉˆÈ‰ï¿½ï¿½ÌƒIï¿½vï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½
		bSampleID = TRUE;
		bSaveMeasResult = TRUE;
		bSaveOverwrite = TRUE;
		bSaveNameWithDate = TRUE;	//ï¿½Aï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Í�gï¿½pï¿½Æ‚ï¿½ï¿½ï¿½TRUEï¿½Å‚ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í•tï¿½ï¿½ï¿½È‚ï¿½
	}
	//2009.12.17 bagus ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì•Ï�X --}--


	//2009.08.28 bagus stress --{--
	//ï¿½ï¿½ï¿½É•Û‘ï¿½ï¿½ï¿½ï¿½×‚ï¿½ï¿½Aï¿½Ô‚ÌŒï¿½ï¿½ï¿½
	//zï¿½Ü‚Å�sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½ã�‘ï¿½ï¿½
	int nNextSaveNo = strlen(szCheckLetters)-1;
	//2009.12.21 bagus ï¿½Cï¿½ï¿½ --}--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		if(!m_pDoc->IsStressRefereneMeasuremnt()){
			char szCheckDir[MAX_PATH+1];
			//2009.10.17 bagus stress --{--
			//for(int nCheckNo = 1;nCheckNo < INT_MAX;nCheckNo++){
			//	sprintf(szCheckDir,"%s\\%s\\%s_%d%s",g_szResult_Dir,szSampleId,pszMainRcpName,nCheckNo,DAT_EXT);
//2009.12.21 bagus ï¿½Cï¿½ï¿½ --{--
//			for(int nCheckNo = 0;nCheckNo < strlen(szCheckDir);nCheckNo++){
			for(int nCheckNo = 0;nCheckNo < static_cast<int>(strlen(szCheckLetters));nCheckNo++){
//2009.12.21 bagus ï¿½Cï¿½ï¿½ --}--
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
		//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ì�ê�‡ï¿½Íƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ğ•·‚ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½È‚ï¿½
		bSaveMeasResult = TRUE;
		//2009.09.15 bagus stress --{--
		//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ìƒï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½Í�ï¿½É�ã�‘ï¿½ï¿½
		if(m_pDoc->IsStressRefereneMeasuremnt()){
			bSaveOverwrite = TRUE;
		}
		//2009.09.15 bagus stress --}--
	}
	//2009.08.28 bagus stress --}--

//=========================================================
//[ï¿½Û‘ï¿½ï¿½ï¿½pï¿½Xï¿½æ“¾]==========================================
	switch(iSaveWay)
	{
		case MEASUREMENT_EXIT_SAVE:

			if((m_pDoc->GetHostMode() != HOST_LOCAL)&&(bSaveOverwrite == FALSE)) //ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½g ï¿½ï¿½ï¿½ï¿½ ï¿½ã�‘ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Kï¿½vï¿½È�ê�‡
			{
				//2009.08.28 bagus stress --{--
				//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½É‘Î‰ï¿½
				//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Íƒtï¿½@ï¿½Cï¿½ï¿½ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½
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
					//2009.12.17 bagus ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì•Ï�X --{--
					if(!bSaveNameWithDate){
						//ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½Ì�İ’ï¿½É‚ï¿½ï¿½Û‘ï¿½ï¿½ï¿½ï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ÆŒï¿½ï¿½Êƒtï¿½@ï¿½Cï¿½ï¿½ï¿½É‘ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
					//2009.12.17 bagus ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì•Ï�X --}--
				}
				//2009.08.28 bagus stress --}--
				if(( FileOrDirExists(szSaveFullFilePath)==TRUE ) //ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½dï¿½ï¿½ï¿½Lï¿½ï¿½
				||( FileOrDirExists(szSaveFullDirPath)==TRUE )) //ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½dï¿½ï¿½ï¿½Lï¿½ï¿½
				{
					AlarmIf_Set(ALID_MeasurementResultNameDuplication); //ï¿½ï¿½ï¿½èŒ‹ï¿½Ê•Û‘ï¿½ï¿½ï¿½ï¿½dï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½)

					int iRtnOverlap = SaveOverlap(szSaveFullFilePath);
					if(iRtnOverlap != -1) //-1ï¿½Í�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s
					{
//						return iRtnOverlap;
						return 2; //ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‘ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½É‘ï¿½ï¿½ï¿½ï¿½Ê‚É‚Æ‚Ç‚Ü‚ï¿½Æƒ}ï¿½Yï¿½Cï¿½Ì‚Å�Aï¿½ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½Å‚ï¿½ï¿½Û‘ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Éƒï¿½ï¿½Vï¿½sï¿½Iï¿½ï¿½ï¿½ï¿½Ê‚É”ï¿½ï¿½ï¿½ï¿½ï¿½B
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
					//2009.12.17 bagus ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì•Ï�X --{--
					if(!bSaveNameWithDate){
						//ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½Ì�İ’ï¿½É‚ï¿½ï¿½Û‘ï¿½ï¿½ï¿½ï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ÆŒï¿½ï¿½Êƒtï¿½@ï¿½Cï¿½ï¿½ï¿½É‘ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
					//2009.12.17 bagus ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì•Ï�X --}--
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
			//ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
			break;
	}


//=========================================================================
//[ï¿½Û‘ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½É‚ï¿½ï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½(ï¿½tï¿½Hï¿½ï¿½ï¿½_)ï¿½ï¿½ï¿½í�œ]==
	//ï¿½Û‘ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É‘ï¿½ï¿½İ‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Ì�ï¿½ï¿½ï¿½
	memset(szSaveFullDirPath, 0, sizeof(szSaveFullDirPath));
	_tsplitpath(szSaveFullFilePath, NULL, szDirName, szDirName2, NULL);
	sprintf(szSaveFullDirPath, "%s%s", szDirName, szDirName2); //
	if(( FileOrDirExists(szSaveFullFilePath)==TRUE ) //ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½dï¿½ï¿½ï¿½Lï¿½ï¿½
	||( FileOrDirExists(szSaveFullDirPath)==TRUE )) //ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½dï¿½ï¿½ï¿½Lï¿½ï¿½
	{
		bFileNameExistFlg = TRUE; //ï¿½Û‘ï¿½ï¿½\ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½dï¿½ï¿½
		sprintf(szDeleteFilePath, "%s", szSaveFullFilePath); //ï¿½í�œï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½ï¿½pï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ì�¬
		sprintf(szDeleteDirPath, "%s", szSaveFullDirPath); //ï¿½í�œï¿½Î�Ûƒtï¿½Hï¿½ï¿½ï¿½_ï¿½pï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ì�¬
	}
	else
	{
		//ï¿½dï¿½ï¿½ï¿½iï¿½V
		bFileNameExistFlg = FALSE; //
	}


	//ï¿½Û‘ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Ì�ã�‘ï¿½ï¿½(ï¿½ï¿½ï¿½mï¿½É‚Í�í�œ+ï¿½Ú“ï¿½)ï¿½Ì‰ï¿½ï¿½ï¿½ï¿½ï¿½
	if(bFileNameExistFlg == TRUE) //ï¿½Û‘ï¿½ï¿½ï¿½ï¿½É�dï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	{
/* modified 2016.08.31 hmenjo ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½É�í�œ ---------- { ---------- */
//		if((m_pDoc->GetHostMode() == HOST_LOCAL)
//		&&(bSampleID == TRUE)&&(bSaveMeasResult == TRUE)&&(bSaveOverwrite == FALSE))
//		{
//			int iRtnOverlap = SaveOverlap(szSaveFullFilePath);
//			if(iRtnOverlap != -1) //-1ï¿½Í�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s
//			{
//				return iRtnOverlap;
//			}
//		}
//
//		//ï¿½ã�‘ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½Æ‚ï¿½ï¿½Ä�Aï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½(ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½í�œ)
//		remove(szDeleteFilePath); //ï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½ï¿½í�œ
//		//ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ï¿½ï¿½_ï¿½Å‚Í�Aï¿½dï¿½ï¿½ï¿½Íƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì‚İ‚Å”ï¿½ï¿½fï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Ì‚Å�Aï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½í�œï¿½Í’ï¿½ï¿½Oï¿½Å‘ï¿½ï¿½İ‚ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B
//		if(FileOrDirExists(szDeleteDirPath)==TRUE)
//		{
//			m_pMainFrame->DeleteDirectory(_T(szDeleteDirPath)); //ï¿½Î�Ûƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ä‹Aï¿½Iï¿½É�í�œ
//		}
/* modified 2016.08.31 hmenjo ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½É�í�œ ----------              */
		int l_iRtnOverlap = 0;
		if (
			(m_pDoc->GetHostMode() == HOST_LOCAL) &&	/* ï¿½ï¿½ï¿½[ï¿½Jï¿½ï¿½ï¿½Å‚ï¿½ï¿½é‚±ï¿½ï¿½	*/
			(bSampleID == TRUE) &&						/* ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½Å�uï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½IDï¿½ï¿½ï¿½gï¿½pï¿½vï¿½ï¿½ï¿½Iï¿½ï¿½	*/
			(bSaveMeasResult == TRUE) &&				/* ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½Å�uï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½IDï¿½ï¿½ï¿½Å�ï¿½ï¿½ï¿½ï¿½Û‘ï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Iï¿½ï¿½	*/
			(bSaveOverwrite == FALSE)					/* ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½Å�uï¿½ã�‘ï¿½ï¿½ï¿½Û‘ï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Iï¿½t	*/
		) {
			/* ï¿½ã�‘ï¿½ï¿½ï¿½mï¿½Fï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O	*/
			l_iRtnOverlap = SaveOverlap(szSaveFullFilePath);
			if (-1 == l_iRtnOverlap) {
				/* ï¿½ã�‘ï¿½ï¿½ï¿½Û‘ï¿½(SAVE)ï¿½Å�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s	*/
				/*	ï¿½ã�‘ï¿½ï¿½ï¿½Oï¿½Ì�í�œï¿½ï¿½ï¿½ï¿½ï¿½Í�ï¿½ï¿½ï¿½ if (-1 == l_iRtnOverlap) ï¿½uï¿½ï¿½ï¿½bï¿½Nï¿½Å‚ï¿½ï¿½D	*/
			} else if (-2 == l_iRtnOverlap) {
				/* ï¿½ï¿½ï¿½Oï¿½ï¿½tï¿½ï¿½ï¿½Ä•Û‘ï¿½(SAVE AS)ï¿½Å�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s	*/
				/*	szSaveFullFilePath ï¿½Å�wï¿½è‚³ï¿½ê‚½ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½ï¿½ê�‡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½Ì‚Å�C
					ï¿½ï¿½ï¿½İ‚ï¿½ï¿½ï¿½ê�‡ï¿½Í�í�œï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
					ï¿½ï¿½ï¿½Ì�ê�‡ï¿½CszSaveFullFilePath ï¿½ï¿½ï¿½ï¿½Î�Ûƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ğ�¶�ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
					ï¿½Î�Û‚ï¿½ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½pï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Å‚ï¿½ï¿½Ì‚Å�Cï¿½ï¿½ï¿½Ìƒtï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½ï¿½
					ï¿½fï¿½[ï¿½^ï¿½Ì‘ï¿½ï¿½İ‚Í�dï¿½lï¿½ã‚ ï¿½è‚¦ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½Å‚ï¿½ï¿½D	*/
				if (0xffffffff != ::GetFileAttributes(szSaveFullFilePath)) {
					/* ï¿½ï¿½ï¿½İ‚ï¿½ï¿½ï¿½ï¿½Ì‚Å�í�œï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½	*/
					remove(szSaveFullFilePath);
					char l_szDeleteDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
					strcpy(l_szDeleteDirPath, szSaveFullFilePath);
					if (0 != strrchr(l_szDeleteDirPath, _T('.'))) {
						*(strrchr(l_szDeleteDirPath, _T('.'))) = 0x00;
						if (FALSE != FileOrDirExists(l_szDeleteDirPath)) {
							/* ï¿½Î�Ûƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ä‹Aï¿½Iï¿½É�í�œ	*/
							m_pMainFrame->DeleteDirectory(_T(l_szDeleteDirPath));
						}
					}
				}
			} else {
				/* ï¿½Û‘ï¿½ï¿½ï¿½ï¿½È‚ï¿½(CANCEL)	*/
				return l_iRtnOverlap;
			}
		} else {
			l_iRtnOverlap = -1;
		}
		if (-1 == l_iRtnOverlap)  {
			/* ï¿½ã�‘ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½Æ‚ï¿½ï¿½Ä�Cï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½(ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½í�œ)	*/
			remove(szDeleteFilePath); /* ï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½ï¿½í�œ	*/
			/*	ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ï¿½ï¿½_ï¿½Å‚Í�Cï¿½dï¿½ï¿½ï¿½Íƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ì‚İ‚Å”ï¿½ï¿½fï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Ì‚Å�C
				ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½í�œï¿½Í’ï¿½ï¿½Oï¿½Å‘ï¿½ï¿½İ‚ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			if (FileOrDirExists(szDeleteDirPath) == TRUE) {
				m_pMainFrame->DeleteDirectory(_T(szDeleteDirPath));	/* ï¿½Î�Ûƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ä‹Aï¿½Iï¿½É�í�œ	*/
			}
		}
/* modified 2016.08.31 hmenjo ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½É�í�œ ---------- } ---------- */
	}

//==========================================================
//[ï¿½ï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½Éˆê��ï¿½Û‘ï¿½]=====================================
	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Rï¿½sï¿½[
	sprintf(szRefFilePath, "%s%s%s", g_szData_Ref_Dir, /*szRecipeName*/pszMainRcpName, DAT_EXT);
	sprintf(szToFilePath, "%s%s%s", g_szData_Measured_Data_Temp_Dir, pszMainRcpName, REFERENCE_EXT); //hibino [ï¿½ï¿½]ï¿½ï¿½ï¿½èŒ‹ï¿½Ê•Û‘ï¿½ï¿½ï¿½ï¿½Éƒï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ÌŠgï¿½ï¿½ï¿½qï¿½ï¿½.refï¿½É•Ï�X(ï¿½Rï¿½sï¿½[ï¿½ï¿½ï¿½ï¿½.datï¿½Ì‚Ü‚ï¿½)
	CopyFile(szRefFilePath, szToFilePath, FALSE); 						// ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Aï¿½ã�‘ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	//2009.09.01 bagus stress --{--
	//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ìƒï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½É‚ï¿½ï¿½ï¿½ï¿½ï¿½Rï¿½sï¿½[ï¿½ï¿½ï¿½ï¿½
	//StressRefRaw.dat -> ref\StressRef\SampleID.dat
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		sprintf(szRefFilePath, "%s%s%s",g_szData_Measured_Data_Temp_Dir ,_T("StressRefRawData"), DAT_EXT);
		sprintf(szToFilePath, "%s%s%s", g_szData_StressRef_Dir,szSampleId , DAT_EXT);
		CopyFile(szRefFilePath, szToFilePath, FALSE);
	}

	//2009.09.01 bagus stress --}--

	//ï¿½ï¿½ï¿½Vï¿½sï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½É•Û‘ï¿½(ï¿½ï¿½ï¿½ï¿½Oï¿½É•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Ì‚ï¿½)
	if(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÈŠO
	{
		RecipeFile_SaveCurrRecipeInfo(pszMainRcpName);
	}
	else //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	{
		RecipeFile_SaveCurrRecipeInfo(pszMainRcpName, 1);
	}

	//ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	SaveMeasConRevi(pszMainRcpName);

	//ï¿½fï¿½[ï¿½^ï¿½Ç‹L
	SaveMeasConAdd(pszMainRcpName); //ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½È‚Ç’Ç‹L

//==========================================================
//[ï¿½tï¿½@ï¿½Cï¿½ï¿½renameï¿½ï¿½move(ï¿½ï¿½ï¿½Kï¿½Ì�ê�Šï¿½ÖŠiï¿½[)]====================
	//ï¿½tï¿½@ï¿½Cï¿½ï¿½renameï¿½ï¿½move(ï¿½ï¿½ï¿½Kï¿½Ì�ê�Šï¿½ÖŠiï¿½[)

	_tsplitpath(szSaveFullFilePath, NULL, szDirName, szReNameDir, NULL); //ï¿½pï¿½Xï¿½ğ•ª‰ï¿½


	//ï¿½tï¿½@ï¿½Cï¿½ï¿½move&rename
	sprintf(szMoveFrom, "%s%s%s", g_szData_Measured_Data_Temp_Dir, pszMainRcpName, DAT_EXT);
	sprintf(szMoveTo, "%s", szSaveFullFilePath);
	MoveFile(szMoveFrom, szMoveTo ); //Move(Copyï¿½Å‚Í‚È‚ï¿½)

	//2010.01.13 bagus 2point --{--
	//2ï¿½_ï¿½Ô‹ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Íƒtï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½È‚ï¿½ï¿½Ì‚Å‚ï¿½ï¿½ï¿½ï¿½ï¿½Return
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		return 1;
	}
	//2010.01.13 bagus 2point --}--
	// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ -->
	else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE){
		return 1;
	}
	// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ <--

	//ï¿½ï¿½RecipeFile_SaveCurrRecipeInfo()ï¿½ï¿½2ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½[Point]ï¿½ï¿½ï¿½Ç‹Lï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½moveï¿½ï¿½ï¿½Kï¿½v
	//ï¿½ï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½Í�cï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½2ï¿½ï¿½Ú‚ÌƒZï¿½[ï¿½uï¿½Ì�ï¿½ï¿½É�ï¿½ï¿½é‚©ï¿½ï¿½copyï¿½É‚ï¿½ï¿½ï¿½B
	//ï¿½tï¿½Hï¿½ï¿½ï¿½_copy&rename
	sprintf(szCopyFrom, "%s%s", g_szData_Measured_Data_Temp_Dir, "*");
	sprintf(szCopyTo, "%s%s%s", szDirName, szReNameDir, "\\");
	_fullpath(szFullPathFrom, (const char *)szCopyFrom, sizeof(szFullPathFrom)); //ï¿½ï¿½Îƒpï¿½Xï¿½É•ÏŠï¿½
	_fullpath(szFullPathTo, (const char *)szCopyTo, sizeof(szFullPathTo)); //ï¿½ï¿½Îƒpï¿½Xï¿½É•ÏŠï¿½
	MakeDirectory(szFullPathTo); //ï¿½ó‚¯‘ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ì�¬
	//2009.09.17 bagus stress --{--
	//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ìƒï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Rï¿½sï¿½[ï¿½ï¿½ï¿½ï¿½
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		if(m_pDoc->IsStressRefereneMeasuremnt()){
			sprintf(szRefFilePath, "%s%s%s",g_szData_Measured_Data_Temp_Dir,pszMainRcpName, DAT_EXT);
			//sprintf(szToFilePath, "%s%s%s", g_szData_Measured_Data_Temp_Dir,STRESS_REFERENCE_NAME, DAT_EXT);
			sprintf(szToFilePath, "%s%s\\%s%s",g_szResult_Dir , szSampleId ,STRESS_REFERENCE_NAME, DAT_EXT);
			MoveFileEx(szRefFilePath, szToFilePath,MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
		}
	}
	//2009.09.17 bagus stress --}--
	int iCpChk = FolderCopy(szFullPathFrom, szFullPathTo); //ï¿½Rï¿½sï¿½[(Moveï¿½Å‚Í‚È‚ï¿½)


	//ï¿½Å�Iï¿½ï¿½ï¿½èŒ‹ï¿½Ê•Û‘ï¿½ï¿½ï¿½ï¿½ï¿½DATAï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ÉŠiï¿½[
	// write NanoSpec ini file
	char szLastSaveInfFilePath[_MAX_PATH];
	memset(szLastSaveInfFilePath, 0, sizeof(szLastSaveInfFilePath));
	char szMoveToFullPath[SAVE_FILE_PATH_NAME_LEN+1];
	memset(szMoveToFullPath, 0, sizeof(szMoveToFullPath));
	_fullpath(szMoveToFullPath, (const char *)szMoveTo, sizeof(szMoveToFullPath)); //ï¿½ï¿½Îƒpï¿½Xï¿½É•ÏŠï¿½

/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ NanoSpec.exe ---------- { ----------	*/
//	sprintf(szLastSaveInfFilePath, "%s%s%s", DATA_DIR, "LastSaveInf", INF_EXT);
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ NanoSpec.exe ----------				*/
	_stprintf(szLastSaveInfFilePath, _T("%sLastSaveInf") _T(INF_EXT), g_szData_Dir);
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ NanoSpec.exe ---------- } ----------	*/
	::WritePrivateProfileString("LASTSAVEINF",
								"MeasuredDataPath",
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ NanoSpec.exe ---------- { ----------	*/
//								szMoveTo,
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ NanoSpec.exe ----------				*/
								szMoveToFullPath,
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ NanoSpec.exe ---------- } ----------	*/
								szLastSaveInfFilePath);


	return (int)((0 == iCpChk)? 1 : 0);
}


/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConRevi
// Purpose		: ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½È�ê�‡ï¿½gï¿½p
// Parameters	: pszMainRcpName  ---> ï¿½Ç‹Lï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½(ï¿½pï¿½X)
void CMeasurementTabView::SaveMeasConRevi(LPCTSTR pszMainRcpName)
{

//ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ -->
	int iRecalibMax;
// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ <--

/* added 2015.06.01 hmenjo ï¿½ï¿½ï¿½Ë—ï¿½ NONE ï¿½oï¿½O ---------- { ---------- */
	sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);
/* added 2015.06.01 hmenjo ï¿½ï¿½ï¿½Ë—ï¿½ NONE ï¿½oï¿½O ---------- } ---------- */
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	if ( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS ) {
// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ -->
		if ( rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_COMPEASE ) {
			iRecalibMax = sizeof(g_lpszCompEASERecalibItem_ENU) / sizeof(g_lpszCompEASERecalibItem_ENU[0]);
			for ( int i = 0; i < iRecalibMax; i++ ) {
				arrayStringItem.Add(g_lpszCompEASERecalibItem_ENU[i]);
			}
		}
		else {
	// ï¿½Xï¿½gï¿½ï¿½ï¿½eï¿½Wï¿½[ï¿½ï¿½ï¿½Ì‘ï¿½ï¿½è�€ï¿½ÚƒCï¿½ï¿½ï¿½fï¿½bï¿½Nï¿½Xï¿½ï¿½Ç‰ï¿½
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
// 2013.02.01 bagus CompleteEASEï¿½wï¿½bï¿½hï¿½Ç‰ï¿½ <--

		// SRï¿½ï¿½MainRecipeReviï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ÌƒZï¿½Nï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½Lï¿½[ï¿½Cï¿½fï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Cï¿½Ï�ï¿½ï¿½Ì�İ’ï¿½
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
/* deleted 2015.06.01 hmenjo ï¿½ï¿½ï¿½Ë—ï¿½ NONE ï¿½oï¿½O ---------- { ---------- */
//			sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);
/* deleted 2015.06.01 hmenjo ï¿½ï¿½ï¿½Ë—ï¿½ NONE ï¿½oï¿½O ---------- } ---------- */
			::WritePrivateProfileString("MainRecipe",
										pszKey[i],
										strBuffer,
										szFilePath);
		}
	}


//===================================================
	// write NanoSpec ini file
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)  //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	{
		::WritePrivateProfileString("MainRecipe",
									"StageProgram",
									"None",
									szFilePath);
	}

}

/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConRevi2
// Purpose		: iniï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½oï¿½ï¿½
// Parameters	: pDesc 		  --->
//				  iDescCount	  ---> ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İƒLï¿½[ï¿½ï¿½
//				  pszMainRcpName  ---> ï¿½Ç‹Lï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½(ï¿½pï¿½X)
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
// Purpose		: ï¿½Ç‹Lï¿½ï¿½ï¿½Kï¿½vï¿½È�ê�‡ï¿½gï¿½p(ï¿½ï¿½É‘ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½p)
// Parameters	: pDesc				--->
//				  iDescCount		---> ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İƒLï¿½[ï¿½ï¿½
//				  pszMainRcpName	---> ï¿½Ç‹Lï¿½Î�Ûƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½(ï¿½pï¿½X)
void CMeasurementTabView::SaveMeasConAdd(LPCTSTR pszMainRcpName)
{

//
	BOOL bCheck = FALSE;

	char szUserId[MAX_PATH + 1];									//ï¿½ï¿½ï¿½[ï¿½UID
	SYSTEMTIME syStartDateTime;										//ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½
	SYSTEMTIME syEndDateTime;										//ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	char szSampleId[SAMPLE_ID_LEN + 1];								//	ï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½ID
	//2009.10.13 bagus RS --{--
	char szLotId[LOT_ID_LEN + 1];									//	ï¿½ï¿½ï¿½bï¿½gID
	//2009.10.13 bagus RS --}--
	char szUnit[DISP_UNIT_NAME_LEN + 1];							//	ï¿½ï¿½ï¿½èŒ‹ï¿½Ê’Pï¿½Ê–ï¿½
	int iSetPointCount;												//ï¿½iï¿½[ï¿½Ïƒ|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½
	char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1] = {0};			//ï¿½ï¿½ï¿½xï¿½ï¿½(X,Y,Z) ï¿½cï¿½ï¿½ï¿½gï¿½p(ï¿½Ö�ï¿½ï¿½Ìˆï¿½ï¿½ï¿½ï¿½É•Kï¿½vï¿½È‚Ì‚Å‚ï¿½ï¿½é‚¾ï¿½ï¿½)
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	double dData[ADAPRESULT_COLS_MAX];								//Colï¿½fï¿½[ï¿½^(AFï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒfï¿½[ï¿½^)
	char szFilePath[_MAX_PATH];
	RCP_DATA rcp_data;

// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
	int	nColumnLen;
	char szTmp[255+1];
	char szTmp2[255+1];
	int	nKeyLen;
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

/* added 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
	int l_iUnitIndexThickInt = -1;		/* -1 ï¿½Í�ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½	*/
	SR_FIGURE_FORMAT l_SrFigureFormat;
	ConfigFile_GetNanoSpecIni(&l_SrFigureFormat,
					CONFIG_FILE_SR_FIGURE_FORMAT);
	if (0 != l_SrFigureFormat.iThicknessIntOnly) {
		l_iUnitIndexThickInt = 3;		/* -1 ï¿½ÈŠOï¿½ÍƒCï¿½ï¿½ï¿½fï¿½bï¿½Nï¿½Xï¿½l	*/
	}
/* added 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */

//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	memset(szUserId, 0, sizeof(szUserId));
	memset(&syStartDateTime, 0, sizeof(syStartDateTime));
	memset(&syEndDateTime, 0 , sizeof(syEndDateTime));
	memset(szSampleId, 0, sizeof(szSampleId));
	//2009.10.13 bagus RS --{--
	//ï¿½ï¿½ï¿½bï¿½gIDï¿½Ì’Ç‰ï¿½
	memset(szLotId, 0, sizeof(szLotId));
	//2009.10.13 bagus RS --}--
	memset(szUnit, 0, sizeof(szUnit));
	iSetPointCount = 0;
	memset(szDispLabel, 0, sizeof(szDispLabel));
	memset(dData ,0 ,sizeof(dData));
	memset(szFilePath, 0, sizeof(szFilePath));


//ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½W
	m_pDoc->GetUserId(szUserId);	//ï¿½ï¿½ï¿½[ï¿½UIDï¿½Ì�æ“¾
	m_pDoc->GetStartDateTime(&syStartDateTime);	//	ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½ï¿½Ì�æ“¾
	m_pDoc->GetEndDateTime(&syEndDateTime);	//	ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�æ“¾
	m_pDoc->GetSampleId(szSampleId);	//	ï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½IDï¿½Ì�æ“¾
	//2009.10.13 bagus RS --{--
	//ï¿½ï¿½ï¿½bï¿½gIDï¿½Ì’Ç‰ï¿½
	m_pDoc->GetLotId(szLotId);		//	ï¿½ï¿½ï¿½bï¿½gIDï¿½Ì�æ“¾
	//2009.10.13 bagus RS --}--
	m_pDoc->GetUnitName(szUnit);	//	ï¿½ï¿½ï¿½èŒ‹ï¿½Ê’Pï¿½Ê–ï¿½ï¿½Ì�æ“¾
	iSetPointCount = m_pDoc->GetStoreCount();	//ï¿½iï¿½[ï¿½ÏŒï¿½ï¿½ï¿½ï¿½Ì�æ“¾
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
//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½É’Ç‹L

	char szSection[16 + 1];
	char szStartDate[255 + 1];
	char szStartTime[255 + 1];
	char szEndDate[255 + 1];
	char szEndTime[255 + 1];
	//2009.09.01 bagus stress --{--
	//ï¿½ï¿½ï¿½Ú‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Åƒoï¿½bï¿½tï¿½@ï¿½ğ‘�‚â‚·
	//char szFullLabel[1024+1];
	char szFullLabel[2048+1];
	//2009.09.01 bagus stress --}--
	char szPointNum[255 + 1]; //ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½
	char szPointKey[255 + 1]; //ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Lï¿½[ï¿½ï¿½
	char szPointData[1024+1];
	char szTempFormat[32+1];
/* added 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ---------- { ---------- */
	char szTempFormatThickSD[32+1];
/* added 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ---------- } ---------- */
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



//ï¿½ï¿½ï¿½ï¿½Jï¿½n(ï¿½Iï¿½ï¿½)ï¿½ï¿½ï¿½ï¿½
	sprintf(szStartDate, "%04d/%02d/%02d", syStartDateTime.wYear,syStartDateTime.wMonth,syStartDateTime.wDay); //ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½t
	sprintf(szStartTime, "%02d:%02d:%02d.%03d", syStartDateTime.wHour,syStartDateTime.wMinute,syStartDateTime.wSecond,syStartDateTime.wMilliseconds); //ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½
	sprintf(szEndDate, "%04d/%02d/%02d", syEndDateTime.wYear,syEndDateTime.wMonth,syEndDateTime.wDay); //ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½t
	sprintf(szEndTime, "%02d:%02d:%02d.%03d", syEndDateTime.wHour,syEndDateTime.wMinute,syEndDateTime.wSecond,syEndDateTime.wMilliseconds); //ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	iWritePoint = 0;
	iWritePoint += sprintf(szFullLabel + iWritePoint, "%s",  "    "); ////ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½N*4
//2009.09.01 bagus stress --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		//SRï¿½ï¿½ï¿½ï¿½
	//ï¿½wï¿½bï¿½_1(X,Y,Zï¿½ï¿½)
		for(i=1; i<4+4; i++)
		{
			char szXyz[255+1];
			memset(szXyz, 0, sizeof(szXyz));

//2009.10.20 bagus Stress ï¿½Cï¿½ï¿½ --{--
#if 0
			if(i == 4)
				sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "MPa"); 		//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½ï¿½MPa
			else
				sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
#else
			if (i == 4) {
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "[MPa]"); 		//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½ï¿½MPa
				sprintf(szTmp, "%s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "[MPa]"); 		//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½ï¿½MPa
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			} else if (i > 4) {
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
				sprintf(szTmp, "%s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			} else {
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//sprintf(szXyz, "     %s", MEAS_LIST_STRESS_POSITION_ITEM[i]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
				sprintf(szTmp, "%s", MEAS_LIST_STRESS_POSITION_ITEM[i]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			}
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp);
			sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
#endif
//2009.10.20 bagus Stress ï¿½Cï¿½ï¿½ --}--
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
	//ï¿½wï¿½bï¿½_2(AFï¿½ï¿½ï¿½Ç‹L)
		// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
		//iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",	MEAS_LIST_AF_CHECK_ITEM[0]);
		nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
		iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
	}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		for(i=0; i<4; i++)
		{
			char szXyz[255+1];
			memset(szXyz, 0, sizeof(szXyz));
			sprintf(szTmp, "%s%s", MEAS_LIST_POSITION_ITEM[i % 2 + 1], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
			nColumnLen = COLUMN_LENGTH - strlen(szTmp);
			sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
//2009.12.10 bagus ï¿½Cï¿½ï¿½ --{--
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
//2009.12.10 bagus ï¿½Cï¿½ï¿½ --{--
		}

	// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
	//2009.10.28 bagus 2point-Distance --{--
	}else{
		//SRï¿½È‚ï¿½
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
			//ï¿½wï¿½bï¿½_1(X,Y,Zï¿½ï¿½)
			for(i=1; i<4; i++)
			{
				char szXyz[255+1];
				memset(szXyz, 0, sizeof(szXyz));
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
				sprintf(szTmp, "%s%s", MEAS_LIST_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
				nColumnLen = COLUMN_LENGTH - strlen(szTmp);
				sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
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
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
//ï¿½wï¿½bï¿½_2(AFï¿½ï¿½ï¿½Ç‹L)
// 2009.12.17 bagus CA --{--
#if 0
		// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
		//iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",	MEAS_LIST_AF_CHECK_ITEM[0]);
		nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
		iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

#else
		if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP) {
			// RSï¿½Í�oï¿½Í‚ï¿½ï¿½È‚ï¿½
		} else if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA) {
			// CAï¿½Í�oï¿½Í‚ï¿½ï¿½È‚ï¿½
		} else {
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
//			if(g_lModelType != MODEL_T3100){
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
				nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
			}
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		}
#endif
// 2009.12.17 bagus CA --}--
	}
//2009.09.01 bagus stress --}--
//ï¿½wï¿½bï¿½_3(ï¿½Xï¿½gï¿½ï¿½ï¿½eï¿½Wï¿½ï¿½ï¿½Ç‹L)

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
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
#if 0
		if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 10.10s\"",	szDispLabel[i]); //
		}
		else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,Kï¿½lï¿½n
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 8.8s\"",  szDispLabel[i]); //
		}
		else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%ï¿½n, Fit
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 8.8s\"",  szDispLabel[i]); //
		}
		else //ï¿½ï¿½ï¿½Ì‘ï¿½
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 9.9s\"",  szDispLabel[i]); //
		}
#else
		nColumnLen = COLUMN_LENGTH - strlen(szDispLabel[i]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", szDispLabel[i]);
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
	// 2009.12.15 bagus NANOMAP Mod -->
	//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
	// 2009.12.15 bagus NANOMAP Mod <--
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
		else{
			if(i == 0){
				iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szTmp);
			}
			else{
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
			}
		}
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
#endif
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
	}

	//DATï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ�ï¿½ï¿½ï¿½
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

///MeasuredDataï¿½ï¿½ï¿½ï¿½ï¿½oï¿½ï¿½1(ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
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
//ï¿½ï¿½ï¿½bï¿½gIDï¿½Ì’Ç‰ï¿½
		{"",				"LotID", 		'C',		"0",		szLotId			  	},
//2009.10.13 bagus RS --}--
		{"",				"Unit", 		'C',		"0",		szUnit				},
		{"",				"MeasuredNum",	'I',		"0",		&iSetPointCount 	},
//2009.09.01 bagus stress --{--
//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½pï¿½Ì�ï¿½ï¿½Ú‚Ì’Ç‰ï¿½
		{"",				"MeasuredLSScanNum",	'I',  "0",		&iLSScansNum 		},
		{"",				"StressUnit",			'C',  "",		szStressUnit 		},
//2009.09.01 bagus stress --}--
		{"",				"Header",		'C',		"0",		szFullLabel 		},
	};
	//SrMeasuredDataDesc[]ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	iDescCount = sizeof(SrMeasuredDataDesc) / sizeof(SrMeasuredDataDesc[0]);
	pDesc = new RECIPE_FILE [iDescCount];
	memcpy(pDesc, SrMeasuredDataDesc, sizeof(SrMeasuredDataDesc));

	SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

	if ( pDesc ) {
		delete [] pDesc;
		pDesc = NULL;
	}

//=============================================================================


///MeasuredDataï¿½ï¿½ï¿½ï¿½ï¿½oï¿½ï¿½2(ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Ì‚ï¿½)
	for(i=0; i<iSetPointCount; i++)
	{
		ONE_POINT_DATA OnePointData2;
		double dDistance;
	//2009.10.28 bagus 2point-Distance --{--
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			m_pDoc->GetOnePointData(&OnePointData, 2 * i+1);		//ï¿½nï¿½_
			m_pDoc->GetOnePointData(&OnePointData2, 2 * i+2);		//ï¿½Iï¿½_
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
			//dx,dyï¿½ï¿½ï¿½Ú‚Ì’Ç‰ï¿½
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
			//ï¿½Lï¿½[
			ZeroBungUp(szPointNum, i+1);
			sprintf(szPointKey, "%s%s", "Point", szPointNum);
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			nKeyLen = strlen(szPointKey);
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			iWritePoint = 0;

			//X,Y,Zï¿½ï¿½
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, "\"% +10.3lf\"",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10 ); //Z
			sprintf(szTmp2, "%+.3lf",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10); //Z
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

// 2009.12.17 bagus RS --{--
#if 0
			//AFï¿½ï¿½ï¿½Ç‹L
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",	OnePointData.szShowAfValid); //AF
			sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
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
		//ï¿½Lï¿½[
			ZeroBungUp(szPointNum, i+1);
			sprintf(szPointKey, "%s%s", "Point", szPointNum);
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			nKeyLen = strlen(szPointKey);
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			iWritePoint = 0;
		//2009.09.01 bagus stress --{--
		//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½pï¿½É�ï¿½ï¿½Ú‚ğ–„‚ß‚ï¿½
			if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
				for(int nSpacer = 0;nSpacer < 4;nSpacer++){
// 2009.10.22 bagus Stress ï¿½Cï¿½ï¿½ --{--
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"        --\"");	//
					if (nSpacer == 0) {
						// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
						//iWritePoint += sprintf(szPointData + iWritePoint, "\"       ---\"");	//
						sprintf(szTmp2, "---");
						nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
						sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//						iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
						iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
						// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
					} else {
						// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
						//iWritePoint += sprintf(szPointData + iWritePoint, ",\"       ---\"");	//
						sprintf(szTmp2, "---");
						nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
						sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//						iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
						iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
						// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
					}
// 2009.10.22 bagus Stress ï¿½Cï¿½ï¿½ --}--
				}
				iWritePoint += sprintf(szPointData + iWritePoint, ",");	//
			}
	//2009.09.01 bagus stress --}--

// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
				//X,Y,Zï¿½ï¿½
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, "\"% +10.3lf\"",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
				sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
				sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10 ); //Z
				sprintf(szTmp2, "%+.3lf",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10); //Z
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

				//AFï¿½ï¿½ï¿½Ç‹L
// 2009.12.17 bagus CA --{--
#if 0
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",	OnePointData.szShowAfValid); //AF
				sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
#else
				if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType != HEAD_TYPE_CTA) {
					sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
					nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
					sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				}
#endif
// 2009.12.17 bagus CA --}--
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
			}
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

			int iUnitIndex = 0;
			int iUnit = m_pDoc->GetUnitIndex();
			if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
			else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
			if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
				iUnitIndex = 0;
			}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

			//ï¿½Xï¿½gï¿½ï¿½ï¿½eï¿½Wï¿½ï¿½
			for(j=0; szDispLabel[j][0] != '\0'; j++)
			{
				char szDecimalPointBuff[255+1];
				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				if(strstr(szDispLabel[j], "Thick") != NULL) //Thick*
				{
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
//					//sprintf(szTempFormat, ",%s 10%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
//					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
//					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ----------              */
					if (l_iUnitIndexThickInt < 0) {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
					} else {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "lf");
					}
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
				}
				else if((szDispLabel[j][0]=='N')||(szDispLabel[j][0]=='K')) //N,Kï¿½lï¿½n
				{
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[4], "lf\""); //[.####]ï¿½Å’ï¿½
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[4], "lf"); //[.####]ï¿½Å’ï¿½
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
				}
				else if(strstr(szDispLabel[j], "%") != NULL) //%ï¿½n
				{
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]ï¿½Å’ï¿½
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]ï¿½Å’ï¿½
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
				}
				else if(strstr(szDispLabel[j], "Fit") != NULL) //Fit
				{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
//					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]ï¿½Å’ï¿½
//					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]ï¿½Å’ï¿½
//					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
					if (1 == g_lXmpType) {
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- { ---------- */
////						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf"); //[.##]ï¿½Å’ï¿½
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ----------              */
//						strcpy(szTempFormat, "%.3e");
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
#if FIT_DISP_SCOUT == 3
						strcpy(szTempFormat, "");
#elif FIT_DISP_SCOUT == 2
						strcpy(szTempFormat, "%.2e");
#elif FIT_DISP_SCOUT == 1
						strcpy(szTempFormat, "%.3e");
#else
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf"); //[.##]ï¿½Å’ï¿½
#endif
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
					} else {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]ï¿½Å’ï¿½
					}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
				}
				else //ï¿½ï¿½ï¿½Ì‘ï¿½
				{
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
					//sprintf(szTempFormat, ",%s 9%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
				}

				if(strstr(szDispLabel[j], "Thick") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Thick*
				{
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j] * g_dUnitConvert[iUnitIndex]); //
					sprintf(szTmp2, szTempFormat,  dData[j] * g_dUnitConvert[iUnitIndex]);
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
				}
				else if(strstr(szDispLabel[j], "%") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½%ï¿½n
				{
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j] * 100); //
					sprintf(szTmp2, szTempFormat,  dData[j] * 100);
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
				}
				else //
				{
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
//					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
//					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j]); //
//					sprintf(szTmp2, szTempFormat,  dData[j]);
//					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
					if (0 == strlen(szTempFormat)) {
						long l_lExp = m_pDoc->GetExpDouble(dData[j]);
						sprintf(szTmp2, "%.2fe%+03d",  dData[j] / pow(10, l_lExp), l_lExp);
					} else {
						sprintf(szTmp2, szTempFormat,  dData[j]);
					}
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
				}

				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//				if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
				if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
				}
				else{
					if(j == 0){
						iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
					}
					else{
						iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
					}
				}
// 2013.12.19 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			}
		//2009.10.28 bagus 2point-Distance --{--
		}
		//2009.10.28 bagus 2point-Distance --}--

		const RECIPE_FILE SrPointDataDesc[] =
		{
			//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
			{"MeasuredData",	szPointKey, 	'C',		"0",		szPointData 	},
		};

		//SrPointDataDesc[]ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
	//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ì�ê�‡ï¿½ï¿½LSï¿½Ìƒfï¿½[ï¿½^ï¿½ï¿½ï¿½oï¿½Í‚ï¿½ï¿½ï¿½
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
				//ï¿½Lï¿½ï¿½ï¿½Å‚È‚ï¿½ï¿½Zï¿½Nï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Í‚È‚É‚ï¿½ï¿½ï¿½ï¿½È‚ï¿½
				if(!stressStage.Line[nLine].bScanValid[nSection]){
					continue;
				}
			//LmSn
				sprintf(szLS,"L%dS%d",nLine+1,nSection+1);
			//X-Start
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
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
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			//X-End
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
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
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			//Y
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
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
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			//Stress
				STRESSRESULT tStressResult;
				m_pDoc->GetOneStressLineData(&tStressResult,nLine+1);
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//iWritePoint += sprintf(szPointData + iWritePoint,",\"%.3lf\"",tStressResult.dStressSection[nSection]);
				sprintf(szTmp2, "%.3lf", tStressResult.dStressSection[nSection]);
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
				//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½Ú‚É–ï¿½ï¿½ÖŒWï¿½È�ï¿½ï¿½Ú‚ğ–„‚ß‚ï¿½
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
#if 0
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
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
// 2009.12.14 bagus Stress ï¿½Cï¿½ï¿½ --{--
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
// 2009.12.14 bagus Stress ï¿½Cï¿½ï¿½ --}--
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
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
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


	//ï¿½ï¿½ï¿½s(Point*****ï¿½Ì�ï¿½ï¿½ÚŒï¿½ï¿½1ï¿½sï¿½ï¿½ï¿½sï¿½ï¿½Ç‰ï¿½)
	FILE *fp;
	if ((fp = fopen(szFilePath, "a")) == NULL)
	{
		//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Aï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ê‚¸ï¿½ï¿½ï¿½s
	}
	else
	{
		putc('\n', fp);
		//2009.08.28 bagus stress --{--
		fclose(fp);
		//2009.08.28 bagus stress --}--
	}
	//2009.08.28 bagus stress --{--
	//ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å‚ï¿½ï¿½ï¿½ï¿½fcloseï¿½ï¿½ï¿½È‚ï¿½
	//fclose(fp);
	//2009.08.28 bagus stress --}--


//========================================
///MeasuredDataï¿½ï¿½ï¿½ï¿½ï¿½oï¿½ï¿½3(ï¿½ï¿½ï¿½vï¿½fï¿½[ï¿½^ï¿½Ì‚ï¿½)


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

// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
	BOOL bWriteHead = FALSE;
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

	//ï¿½ï¿½ï¿½vï¿½fï¿½[ï¿½^ï¿½Lï¿½ï¿½Pointï¿½ï¿½, ï¿½Xï¿½gï¿½ï¿½ï¿½eï¿½Wï¿½Bï¿½wï¿½bï¿½_
	iWritePoint = 0;
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
	nKeyLen = 10 - strlen("Header");
	if (nKeyLen < 0) {
		nKeyLen = 0;
	} else {
		iWritePoint += sprintf(szStrategyLabel + iWritePoint, "%*s", nKeyLen, " ");
	}
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
	//2009.09.01 bagus stress --{--
	//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ì“ï¿½ï¿½vï¿½ï¿½ï¿½pï¿½wï¿½bï¿½h
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
		//iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"Stress[MPa]\"");
		sprintf(szTmp2, "%s", "Stress[MPa]");
		nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"%s\"",  szTmp);
		iWritePoint += sprintf(szStrategyLabel + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			STGPGM_STRESS_LINE& LineInfo = rcp_data.StressStageProgInfo.Line[nLine];
			if(LineInfo.bValidLine){
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//iWritePoint += sprintf(szStrategyLabel + iWritePoint,",\"Stress L%d\"",nLine+1);
				sprintf(szTmp2, "Stress L%d", nLine+1);
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szStrategyLabel + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szStrategyLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			}
		}
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
		bWriteHead = TRUE;
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
	}
	//2009.09.01 bagus stress --}--
	for(i=0; szDispLabel[i][0] != '\0'; i++)
	{
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
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
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
#if 0
		if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 10.10s\"",  szDispLabel[i]); //
		}
		else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,Kï¿½lï¿½n
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 8.8s\"",  szDispLabel[i]); //
		}
		else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%ï¿½n, Fit
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 8.8s\"",  szDispLabel[i]); //
		}
		else //ï¿½ï¿½ï¿½Ì‘ï¿½
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
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
		bWriteHead = TRUE;
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
	}

	int iStatisticsValidPointCount = m_pDoc->GetStatisticsValidPointCount();
	const RECIPE_FILE SrStatisticsHeaderDesc[] = //ï¿½ï¿½ï¿½vï¿½fï¿½[ï¿½^ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½Xï¿½gï¿½ï¿½ï¿½eï¿½Wï¿½Bï¿½wï¿½bï¿½_
	{
		//Section Name		Key Name					Data Type	Defaule 	Variable Pointer
		{"STATISTICS",		"ValidMesuredNum",			'I',		"0",		&iStatisticsValidPointCount },
		{"",				"Header",					'C',		"0",		szStrategyLabel 							},
	};

	//SrStatisticsHeaderDesc[]ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
/* added 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ---------- { ---------- */
	memset(szTempFormatThickSD, 0, sizeof(szTempFormatThickSD));
/* added 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ---------- } ---------- */

// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

	for(i=0; i<5; i++)
	{
		iWritePoint = 0;
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
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
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
		int iWritePointSpaceBung;
		iWritePointSpaceBung=0;

		//2009.09.01 bagus stress --{--
		//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ì�ê�‡ï¿½É‚ÍƒXï¿½gï¿½ï¿½ï¿½Xï¿½pï¿½Ì�ï¿½ï¿½Ú‚Ì“ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
			STATISTICS l_StressStat;
			m_pDoc->GetStressStatisticsData(&l_StressStat);
			double dStressData;
			if(i==0)		dStressData = l_StressStat.dMaximum;
			else if(i==1)	dStressData = l_StressStat.dMinimum;
			else if(i==2)	dStressData = l_StressStat.dRange;
			else if(i==3)	dStressData = l_StressStat.dAverage;
			else/*if(i==4)*/dStressData = l_StressStat.dStandardDeviation;
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, "\"%.3lf\"",dStressData);
			sprintf(szTmp2, "%.3lf", dStressData);
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

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
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
					//iWritePoint += sprintf(szPointData + iWritePoint,",\"%.3lf\"",dStressData);
					sprintf(szTmp2, "%.3lf", dStressData);
					nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
					sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
					// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
				}
			}
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			bWriteHead = TRUE;
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
		}
		//2009.09.01 bagus stress --}--

		//Max,Min,Range,Mean,S.D.
		//ï¿½Xï¿½gï¿½ï¿½ï¿½eï¿½Wï¿½ï¿½
		for(j=0; szDispLabel[j][0] != '\0'; j++)
		{
			if(strstr(szDispLabel[j], "Thick") != NULL) //Thick*
			{
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
//				//sprintf(szTempFormat, "%s 10%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
//				sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
//				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ----------              */
				if (l_iUnitIndexThickInt < 0) {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
				} else {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "lf");
				}
/* added 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ---------- { ---------- */
				sprintf(szTempFormatThickSD, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
/* added 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ---------- } ---------- */
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
			}
			else if((szDispLabel[j][0]=='N')||(szDispLabel[j][0]=='K')) //N,Kï¿½lï¿½n
			{
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[4], "lf\""); //[.####]ï¿½Å’ï¿½
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[4], "lf");
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			}
			else if(strstr(szDispLabel[j], "%") != NULL) //%ï¿½n
			{
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]ï¿½Å’ï¿½
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			}
			else if(strstr(szDispLabel[j], "Fit") != NULL) //Fit
			{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
//				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]ï¿½Å’ï¿½
//				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
//				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
				if (1 == g_lXmpType) {
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- { ---------- */
////					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf");
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ----------              */
//					strcpy(szTempFormat, "%.3e");
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
#if FIT_DISP_SCOUT == 3
					strcpy(szTempFormat, "");
#elif FIT_DISP_SCOUT == 2
					strcpy(szTempFormat, "%.2e");
#elif FIT_DISP_SCOUT == 1
					strcpy(szTempFormat, "%.3e");
#else
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf");
#endif
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
				} else {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
				}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
			}
			else //ï¿½ï¿½ï¿½Ì‘ï¿½
			{
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//sprintf(szTempFormat, "%s 9%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
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

// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
#if 0
			//ï¿½lï¿½ß•ï¿½
			char szSpaceBung[16+1];
			memset(szSpaceBung, 0, sizeof(szSpaceBung));
			//2009.09.01 bagus stress --{--
			//if(j == 0)
			if(iWritePoint == 0)
			//2009.09.01 bagus stress --}--
			{
				//Saiki 20090603 Change ----->
				//int iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM[i]); //6ï¿½ï¿½"Header"ï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½(STATISTICSï¿½ÌƒLï¿½[ï¿½Ì�Å’ï¿½ï¿½ï¿½ï¿½ï¿½)
				int iSpaceBungCnt;
				SYSTEM_CONFIG l_SystemConfig;
				ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
				if(l_SystemConfig.nLanguage == 0){
						iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM_ENU[i]); //6ï¿½ï¿½"Header"ï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½(STATISTICSï¿½ÌƒLï¿½[ï¿½Ì�Å’ï¿½ï¿½ï¿½ï¿½ï¿½)
				}
				else{
					iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM_JPN[i]); //6ï¿½ï¿½"Header"ï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½(STATISTICSï¿½ÌƒLï¿½[ï¿½Ì�Å’ï¿½ï¿½ï¿½ï¿½ï¿½)
				}
				//Saiki 20090603 Change <-----
				for(int k=0; k < iSpaceBungCnt; k++)
				{
					iWritePointSpaceBung += sprintf(szSpaceBung + iWritePointSpaceBung, " ");
				}
				iWritePoint += sprintf(szPointData + iWritePoint, szSpaceBung); //
			}
#endif
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
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
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			if(strstr(szDispLabel[j], "Thick") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Thick*
			{
/* modified 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ---------- { ---------- */
//				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
//				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData/*dData[j]*/ * g_dUnitConvert[iUnitIndex]); //
//				sprintf(szTmp2, szTempFormat,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
//				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
/* modified 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ----------              */
				if (i == 4) {
					/* SD ï¿½l	*/
					sprintf(szTmp2, szTempFormatThickSD,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
				} else {
					/* SD ï¿½lï¿½ÈŠO	*/
					sprintf(szTmp2, szTempFormat,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
				}
/* modified 2017.04.29 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ SD ï¿½ï¿½ï¿½O ---------- } ---------- */
			}
			else if(strstr(szDispLabel[j], "%") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½%ï¿½n
			{
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData * 100); //
				sprintf(szTmp2, szTempFormat,  dStatisticsData * 100);
				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
			}
			else //
			{
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
//				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
//				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData/*dData[j]*/); //
//				sprintf(szTmp2, szTempFormat,  dStatisticsData);
//				// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
				if (0 == strlen(szTempFormat)) {
					long l_lExp = m_pDoc->GetExpDouble(dStatisticsData);
					sprintf(szTmp2, "%.2fe%+03d",  dStatisticsData / pow(10, l_lExp), l_lExp);
				} else {
					sprintf(szTmp2, szTempFormat,  dStatisticsData);
				}
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
			}

			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--

// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
			bWriteHead = TRUE;
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
		}

//Saiki 20090603 Change ----->
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --{--
//		SYSTEM_CONFIG l_SystemConfig;
//		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.10.23 bagus Stress ï¿½Cï¿½ï¿½ --}--
		if(l_SystemConfig.nLanguage == 0){

			const RECIPE_FILE SrStatisticsDataDesc_ENU[] =
			{
				//Section Name		Key Name					Data Type	Defaule 	Variable Pointer
				{"STATISTICS",		/*szMeasListParamItem*/MEAS_LIST_PARAM_ITEM_ENU[i], 'C',		"0",		szPointData 	},
			};
			//SrStatisticsDataDesc[]ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
		//SrStatisticsDataDesc[]ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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

/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
	/* ï¿½gï¿½pï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½oï¿½ï¿½*/
	if ((0 == this->MRCS_IncludeCountGet()) &&
		(0 == this->MRCS_DeleteCountGet())) {
		/* ï¿½wï¿½ï¿½sï¿½Cï¿½í�œï¿½Ï‚İ�sï¿½ï¿½ï¿½Æ‚ï¿½ï¿½É‚Oï¿½Ì�ê�‡ï¿½Í�]ï¿½ï¿½ï¿½Ì�dï¿½lï¿½Å‚ï¿½ï¿½D	*/
	} else {
		CString l_strPoints = _T("");
		CString l_strTmp;
		BOOL l_bUse = FALSE;
		int l_iprevUse = 0;
		for (i = 1; i <= iSetPointCount; i++) {
			if (0 == (MRCS_SettingStatusGet(i) & 0x4000)) {
				/* Valid = FALSE ï¿½Å‚È‚ï¿½	*/
				if (0 < this->MRCS_IncludeCountGet()) {
					/* ï¿½wï¿½ï¿½sï¿½İ‚ï¿½	*/
					if (1 == this->MRCS_SettingGet(i)) {
						l_bUse = TRUE;
					}
				} else {
					/* ï¿½wï¿½ï¿½sï¿½ï¿½ï¿½ï¿½	*/
					if (2 != this->MRCS_SettingGet(i)) {
						l_bUse = TRUE;
					}
				}
			}
			if (FALSE != l_bUse) {
				/* ï¿½gï¿½p	*/
				l_bUse = FALSE;
				if (0 < l_strPoints.GetLength()) {
					/* ï¿½Qï¿½ï¿½ÚˆÈ�~	*/
					if (l_iprevUse == i - 1) {
						/* ï¿½Aï¿½ï¿½	*/
						if (_T('-') != l_strPoints.GetAt(l_strPoints.GetLength() - 1)) {
							l_strPoints += _T("-");
						}
						if (i == iSetPointCount) {
							/* ï¿½Å�Iï¿½fï¿½[ï¿½^	*/
							l_strTmp.Format(_T("%d"), iSetPointCount);
							l_strPoints += l_strTmp;
						}
					} else {
						/* ï¿½ï¿½Ñ”ï¿½	*/
						if (_T('-') == l_strPoints.GetAt(l_strPoints.GetLength() - 1)) {
							l_strTmp.Format(_T("%d,%d"), l_iprevUse, i);
						} else {
							l_strTmp.Format(_T(",%d"), i);
						}
						l_strPoints += l_strTmp;
					}
				} else {
					/* ï¿½ï¿½ï¿½ï¿½	*/
					l_strPoints.Format(_T("%d"), i);
				}
				l_iprevUse = i;
			} else {
				/* ï¿½ï¿½ï¿½gï¿½p	*/
				if (i == iSetPointCount) {
					/* ï¿½Å�Iï¿½fï¿½[ï¿½^	*/
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
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */

	return;
}



/////////////////////////////////////////////////////////////////////////////
//ï¿½ï¿½ï¿½lï¿½Ì‘Oï¿½ï¿½0ï¿½ï¿½âŠ®ï¿½ï¿½ï¿½A5ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½B
BOOL CMeasurementTabView::ZeroBungUp(char* czPoint, int iPoint)
{
	char czTemp[5+1];
	memset(czTemp, 0, sizeof(czTemp));

	int i=0;
	int iCnt= 5; //ï¿½Oï¿½ï¿½'0'ï¿½ï¿½âŠ®ï¿½ï¿½ï¿½é�”(5ï¿½Å�ï¿½ï¿½ï¿½ï¿½ï¿½)

	if((iPoint <= 99999) && (iPoint > 0)) //5ï¿½ï¿½ï¿½Ü‚Å‰ï¿½
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
	return FALSE; //ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å“ï¿½ï¿½Bï¿½ï¿½ï¿½ï¿½ï¿½ç�ˆï¿½ï¿½ï¿½ï¿½ï¿½s

}

/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
TCHAR lg_tszDirSelect[_MAX_PATH * 2];	/* ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ */
TCHAR lg_tszEditText[_MAX_PATH];	/* ï¿½ï¿½ï¿½Í’ï¿½ï¿½Ìƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Ìˆê��ï¿½Lï¿½ï¿½ */
TCHAR lg_tszErrMsg[256];	/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
TCHAR lg_tszErrMsgTitle[256];	/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½^ï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
UINT CALLBACK GetSaveFNHookProc(
		HWND hwndDlg,	/* ï¿½Rï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ÌƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ìƒnï¿½ï¿½ï¿½hï¿½ï¿½ */
		UINT uiMsg,		/* ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W */
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
			TCHAR l_tszInvalid[64] = _T("!\"'*+,./:;<=>?@[]^`|~$&()\\");	/* ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ */
			TCHAR l_tszInvalid2[64] = _T("!\"'*+,./:;<=>?@[]^`|~$&()");	/* ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½'\'ï¿½Í‹ï¿½ï¿½Â‚ï¿½ï¿½Ü‚ï¿½ */
			switch (l_pNMHdr->code) {
			case CDN_INITDONE:
				::EnableWindow(::GetDlgItem(l_pNMHdr->hwndFrom, cmb1), FALSE);	/* ï¿½ê�Šï¿½Rï¿½ï¿½ï¿½{ï¿½ï¿½ï¿½fï¿½Bï¿½Zï¿½[ï¿½uï¿½ï¿½ */
				::EnableWindow(::GetDlgItem(l_pNMHdr->hwndFrom, cmb2), FALSE);	/* ï¿½ï¿½ŞƒRï¿½ï¿½ï¿½{ï¿½ï¿½ï¿½fï¿½Bï¿½Zï¿½[ï¿½uï¿½ï¿½ */
				_tcscpy(lg_tszDirSelect, l_pOpenFileNameNotify->lpOFN->lpstrInitialDir);
				/* ï¿½fï¿½Xï¿½Nï¿½gï¿½bï¿½vï¿½Ì’ï¿½ï¿½ï¿½ï¿½É•\ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D */
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
			case CDN_FOLDERCHANGE:	/* ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D */
				{
					/* ï¿½Ï�Xï¿½ï¿½ï¿½ê‚½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½æ“¾ */
					TCHAR l_tszDirChanged[_MAX_DIR * 2];
					memset(l_tszDirChanged, 0, sizeof(l_tszDirChanged));
					CommDlg_OpenSave_GetFolderPath(l_pNMHdr->hwndFrom, l_tszDirChanged, sizeof(l_tszDirChanged));
					switch (ls_iDirChgPrc) {
					case 0:		/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
						ls_iDirChgPrc = 1;
						break;
					case 1:		/* ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½É–ï¿½ï¿½ï¿½ï¿½È•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N */
								/* ï¿½ÆŒï¿½ï¿½Êƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½È‰ï¿½ï¿½ï¿½ï¿½Ç‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N */
								/*	(ï¿½ï¿½ï¿½ï¿½ï¿½Èƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Å‚Í‚È‚ï¿½ï¿½Ä–ï¿½ï¿½ï¿½ï¿½È•ï¿½ï¿½ï¿½ï¿½Ìƒ`ï¿½Fï¿½bï¿½Nï¿½Ì‚İ‚Å‚ï¿½)*/
						{
							TCHAR l_tszDirChgd[_MAX_PATH * 2];
							_tcscpy(l_tszDirChgd, l_tszDirChanged);
							l_tszDirChgd[_tcslen(l_pOpenFileNameNotify->lpOFN->lpstrInitialDir)] = 0;
							if (
								(0 != _tcsicmp(l_tszDirChgd, l_pOpenFileNameNotify->lpOFN->lpstrInitialDir))
								|| (0 != _tcspbrk(&(l_tszDirChanged[_tcslen(l_pOpenFileNameNotify->lpOFN->lpstrInitialDir) + 1]), l_tszInvalid2))
								) {
								/* ï¿½ï¿½ï¿½ï¿½ï¿½Èƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½È‚Ì‚Å�Cï¿½Oï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½É–ß‚ï¿½ï¿½Ü‚ï¿½ï¿½D*/
								/* ï¿½ï¿½ï¿½Í’ï¿½ï¿½Ìƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê��ï¿½Lï¿½ï¿½ */
								CommDlg_OpenSave_GetSpec(l_pNMHdr->hwndFrom, lg_tszEditText, sizeof(lg_tszEditText));
								/* ï¿½Oï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ EDIT ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ÉƒZï¿½bï¿½g */
								::SendDlgItemMessage(l_pNMHdr->hwndFrom, edt1, WM_SETTEXT, 0, (LPARAM) lg_tszDirSelect);
								/* OK ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ */
								::PostMessage(l_pNMHdr->hwndFrom, WM_COMMAND, (WPARAM) IDOK, (LPARAM) 0);
								ls_iDirChgPrc = 2;
							} else {
								_tcscpy(lg_tszDirSelect, l_tszDirChanged);
							}
						}
						break;
					default:	/* ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ß‚ï¿½ï¿½Ò‚ï¿½ */
						if (0 == _tcsicmp(l_tszDirChanged, lg_tszDirSelect)) {
							/* ï¿½ï¿½ï¿½Í’ï¿½ï¿½Ìƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ EDIT ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½É–ß‚ï¿½ï¿½Ü‚ï¿½ */
							::SendDlgItemMessage(l_pNMHdr->hwndFrom, edt1, WM_SETTEXT, 0, (LPARAM) lg_tszEditText);
							ls_iDirChgPrc = 1;
						}
						break;
					}
				}
				break;
			case CDN_FILEOK:	/* OK ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D */
				{				/*		EDIT ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Æ‚ï¿½ï¿½ÍƒRï¿½Rï¿½É—ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½D */
					/* ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½pï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)ï¿½æ“¾ */
					TCHAR l_tszFile[_MAX_FNAME * 2];
					CommDlg_OpenSave_GetSpec(l_pNMHdr->hwndFrom, l_tszFile, sizeof(l_tszFile));
					/* ï¿½gï¿½ï¿½ï¿½qï¿½Ì—Lï¿½ï¿½ï¿½ğ”»’fï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Î�í�œï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D */
					TCHAR l_tszExt[_MAX_EXT] = _T(".");
					_tcscat(l_tszExt, l_pOpenFileNameNotify->lpOFN->lpstrDefExt);
					if (0 == _tcsicmp(&(l_tszFile[_tcslen(l_tszFile) - _tcslen(l_tszExt)]), l_tszExt)) {
						/* ï¿½wï¿½è‚³ï¿½ê‚½ï¿½gï¿½ï¿½ï¿½qï¿½ï¿½ï¿½ï¿½ï¿½Í‚ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ì‚Å�Cï¿½í�œï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D */
						l_tszFile[_tcslen(l_tszFile) - _tcslen(l_tszExt)] = 0;
					}
					if (0 != _tcspbrk(l_tszFile, l_tszInvalid)) {
						/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
						/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½{ï¿½bï¿½Nï¿½Xï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D */
						::MessageBox(0, lg_tszErrMsg, lg_tszErrMsgTitle, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
						/* ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½Â‚ï¿½ï¿½È‚ï¿½ï¿½æ‚¤ï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D */
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
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */

//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â‚ï¿½ï¿½Ä•Û‘ï¿½
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

/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
	CString l_strErrMsg = _T("ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½ï¿½ï¿½É–ï¿½ï¿½ï¿½ï¿½È•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D");
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
//	CString l_strErrMsgTitle = _T("NanoSpec");
	CString l_strErrMsgTitle;
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
	LoadStringML(IDS_RSLTNM_ERRMSG, l_strErrMsg, _T("Invalid character(s) in Result data name."));
	LoadStringML(IDS_TITLE_NANOSPEC, l_strErrMsgTitle, _T("NanoSpec"));
	_tcscpy(lg_tszErrMsg, l_strErrMsg);
	_tcscpy(lg_tszErrMsgTitle, l_strErrMsgTitle);
	/* ï¿½Rï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½Ì�ï¿½ï¿½ï¿½ */
	TCHAR l_tszResultFile[_MAX_PATH * 2] = _T(SAVE_DEFAULT_NAME);	/* ï¿½ï¿½ï¿½Êƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ */
	TCHAR l_tszResultFileTitle[_MAX_FNAME];	/* ï¿½ï¿½ï¿½Êƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½(ï¿½pï¿½Xï¿½ï¿½ï¿½ï¿½) */
	TCHAR l_tszDefaultDir[_MAX_PATH * 2];	/* ï¿½ï¿½ï¿½ï¿½(ï¿½fï¿½tï¿½Hï¿½ï¿½ï¿½g)ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ */
	_tcscpy(l_tszDefaultDir, g_szResult_Dir);
	if (_T('\\') == l_tszDefaultDir[_tcslen(g_szResult_Dir) - 1]) {
		/* ï¿½ÅŒï¿½ï¿½'\'ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î�í�œï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D */
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
		/* ï¿½ï¿½ï¿½[ï¿½Jï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ã�‘ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Kï¿½v */
		l_ofn.Flags |= OFN_OVERWRITEPROMPT; /* ï¿½ã�‘ï¿½ï¿½ï¿½mï¿½F */
	}
	l_ofn.lpstrDefExt = _T("dat");
	l_ofn.lpfnHook = GetSaveFNHookProc;
	BOOL l_bRet = ::GetSaveFileName(&l_ofn);
	if (0 == l_bRet) {
		return FALSE;
	}
/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ----------			   */
//	  CFileDialog dlg( FALSE,
//					  "*.*",
//					  SAVE_DEFAULT_NAME,
//					  OFN_HIDEREADONLY | OFN_NOCHANGEDIR,	  // OFN_NOCHANGEDIR ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½gï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½ï¿½ï¿½ï¿½Î�ï¿½BCFileDialogï¿½Ì’ï¿½ï¿½Oï¿½ÌƒJï¿½ï¿½ï¿½ï¿½ï¿½gï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½É–ß‚ï¿½
//					  "DAT File (*.dat)|*.dat||",
//					  this);
//
//	  if((m_pDoc->GetHostMode() == HOST_LOCAL)&&(bSaveOverwrite == FALSE)) //ï¿½ï¿½ï¿½[ï¿½Jï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ã�‘ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Kï¿½v
//	  {
//		  dlg.m_ofn.Flags |= OFN_OVERWRITEPROMPT; //ï¿½ã�‘ï¿½ï¿½ï¿½mï¿½F
//	  }
//
//	  dlg.m_ofn.lpstrInitialDir = g_szResult_Dir;
//
//	  if(dlg.DoModal() != IDOK)
//	  {
//		  return FALSE;
//	  }
/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */

/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
//	  if(dlg.GetFileTitle().GetLength() > SAMPLE_ID_LEN)
/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ----------			   */
	TCHAR l_tszResultFName[_MAX_FNAME];
	_tcscpy(l_tszResultFName, l_tszResultFileTitle);
	TCHAR *l_pDotPos = _tcsrchr(l_tszResultFName, _T('.'));
	if (0 != l_pDotPos) {
		*l_pDotPos = 0;
	}
	if(_tcslen(l_tszResultFName) > SAMPLE_ID_LEN)
/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
	{
// Kojika 20090526 Change
//		MessageBox("ï¿½Û‘ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ß‚ï¿½ï¿½Ü‚ï¿½", "ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Û‘ï¿½", MB_OK);
		LoadStringML(IDS_NAME_TOO_LONG, strBuffer1, "Name is Too Long.");
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change
		return FALSE;
	}

/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
//	  if(dlg.GetPathName().GetLength() > SAVE_FILE_PATH_NAME_LEN)
/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ----------			   */
	if(_tcslen(l_tszResultFile) > SAVE_FILE_PATH_NAME_LEN)
/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
	{
// Kojika 20090526 Change
//		MessageBox("Path is Too Long.", "Measurement Data Save", MB_OK);
//		MessageBox("ï¿½Û‘ï¿½ï¿½ï¿½pï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ß‚ï¿½ï¿½Ü‚ï¿½", "ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Û‘ï¿½", MB_OK);
		LoadStringML(IDS_PATH_TOO_LONG, strBuffer1, "Path is Too Long.");
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change
		return FALSE;
	}

/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
//	  strcpy(szThisInputFliePass, dlg.GetPathName());
/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ----------			   */
	_tcscpy(szThisInputFliePass, l_tszResultFile);
/* modified 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */


	char szFullPath[MAX_PATH];
//	memset(szFullPath, 0, sizeof(szFullPath));
//	_fullpath(szFullPath, (const char *)RESULT_DIR, sizeof(szFullPath));
	strcpy(szFullPath, g_szResult_Dir);
	if(StrStrI(szThisInputFliePass, szFullPath)==NULL)
	{
		char szMessage[1024+1];
		memset(szMessage, 0, sizeof(szMessage));
// Kojika 20090526 Change
//		sprintf(szMessage, "%s%s", szFullPath, " ï¿½zï¿½ï¿½ï¿½ÈŠOï¿½É‚Í•Û‘ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ‚ï¿½ï¿½Å‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½B\nï¿½Ä“xï¿½Aï¿½Û‘ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B");
//		MessageBox(szMessage, "ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Û‘ï¿½", MB_OK);
//		sprintf(szMessage, " %s ï¿½zï¿½ï¿½ï¿½ÈŠOï¿½É‚Í•Û‘ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ‚ï¿½ï¿½Å‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½B\nï¿½Ä“xï¿½Aï¿½Û‘ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B", szFullPath);
		LoadStringML(IDS_NOT_POSSIBLE_SAVE_BESIDE_BELOW, strBuffer1, "It isn't possible to save it besides below %s. \nPlease save it again.");
		sprintf(szMessage, strBuffer1, szFullPath);
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(szMessage, strBuffer2, MB_OK);
// Kojika 20090526 Change End
		return FALSE;
	}

	return TRUE; //ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å—ï¿½ï¿½ï¿½ï¿½È‚ç�³ï¿½ï¿½
}



/////////////////////////////////////////////////////////////////////////////
//ï¿½ã�‘ï¿½ï¿½ï¿½mï¿½Fï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½oï¿½ï¿½
int CMeasurementTabView::SaveOverlap(char* szSaveFullFilePath)
{
	int iSaveNameOverlap;
	CSaveNameOverlapDlg SaveDlg;
	strcpy(SaveDlg.m_szSpecifiedFileName, szSaveFullFilePath); //ï¿½dï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½É“nï¿½ï¿½
	iSaveNameOverlap = SaveDlg.DoModal();

	if(iSaveNameOverlap == SAVEOVERLAP_SAVE)		   //ï¿½ã�‘ï¿½ï¿½ï¿½Û‘ï¿½(SAVE)
	{
		//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s
	}
	else if(iSaveNameOverlap == SAVEOVERLAP_NOSAVE)    //ï¿½Û‘ï¿½ï¿½ï¿½ï¿½È‚ï¿½(CANCEL)
	{
		//
		return 2;	//2:ï¿½ï¿½ï¿½s(ï¿½Û‘ï¿½ï¿½ï¿½ï¿½È‚ï¿½)
	}
	else if(iSaveNameOverlap == SAVEOVERLAP_SAVEAS)    //ï¿½ï¿½ï¿½Oï¿½ï¿½tï¿½ï¿½ï¿½Ä•Û‘ï¿½(SAVE AS)
	{
		//ï¿½ï¿½ï¿½Oï¿½ï¿½tï¿½ï¿½ï¿½Ä•Û‘ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½Ì‘Iï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½Ä‚Í�wï¿½Û‘ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½x)
		if(!GetMeasSaveFileName(szSaveFullFilePath))
		{
			return 3;	//3:ï¿½ï¿½ï¿½s(ï¿½Û‘ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½)
/* added 2016.08.31 hmenjo ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½É�í�œ ---------- { ---------- */
		} else {
			/* ï¿½ï¿½ï¿½Oï¿½ï¿½tï¿½ï¿½ï¿½Ä•Û‘ï¿½(SAVE AS)ï¿½Å�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s	*/
			return -2;
/* added 2016.08.31 hmenjo ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½É�í�œ ---------- } ---------- */
		}
	}

	return -1; //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s
}



/////////////////////////////////////////////////////////////////////////////
//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½İƒ`ï¿½Fï¿½bï¿½N
BOOL CMeasurementTabView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}



/////////////////////////////////////////////////////////////////////////////
//ï¿½pï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ÌŠJï¿½nï¿½Ê’uï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½ï¿½
//ï¿½pï¿½Xï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½pï¿½Xï¿½ï¿½ï¿½ÅŒï¿½ï¿½ '\' ï¿½Ü‚Å‚ï¿½ï¿½Aï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Æ”ï¿½ï¿½fï¿½ï¿½ï¿½ï¿½
char * CMeasurementTabView::GetFileName( const char *PathName )
{
	for( const char *p = strtail(PathName); p >= PathName; p-- ) // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ÅŒã‚©ï¿½ï¿½Å�ï¿½ï¿½Ü‚ï¿½
	{
		if ( ('\\'==*p) && !isJMS(PathName,p-PathName) )
			return (char*)(p+1);
	}
	return (char *)PathName; // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ '\' ï¿½ÍŠÜ‚Ü‚ï¿½Ä‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½
}//GetFileName


/////////////////////////////////////////////////////////////////////////////
//ï¿½ï¿½ï¿½ï¿½ï¿½ñ––”ï¿½ï¿½Ì•ï¿½ï¿½ï¿½ '\0' ï¿½ÌˆÊ’uï¿½ï¿½Ô‚ï¿½
const char * CMeasurementTabView::strtail( const char *string )
{
	return strchr( string, '\0' );
}//strtail


/////////////////////////////////////////////////////////////////////////////
//ï¿½ï¿½ï¿½ï¿½ï¿½ñ’†‚Ì‚ï¿½ï¿½ï¿½Pï¿½oï¿½Cï¿½gï¿½É‚Â‚ï¿½ï¿½Ä�Aï¿½Pï¿½oï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Qï¿½oï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ìˆê•”ï¿½Å‚ï¿½ï¿½é‚©ï¿½ğ”»’è‚·ï¿½ï¿½B
//*********************************************************
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ str ï¿½ï¿½ str[nPos] ï¿½É‚Â‚ï¿½ï¿½Ä�A
//	 ï¿½O ï¿½cï¿½c ï¿½Pï¿½oï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½
//	 ï¿½P ï¿½cï¿½c ï¿½Qï¿½oï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ìˆê•”ï¿½iï¿½ï¿½Pï¿½oï¿½Cï¿½gï¿½j
//	 ï¿½Q ï¿½cï¿½c ï¿½Qï¿½oï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ìˆê•”ï¿½iï¿½ï¿½Qï¿½oï¿½Cï¿½gï¿½j
// ï¿½Ì‚ï¿½ï¿½ï¿½ï¿½ê‚©ï¿½ï¿½Ô‚ï¿½ï¿½B
//*********************************************************
#define jms1(c) (((0x81 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0x9F)) || ((0xE0 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xFC) ))
#define jms2(c) ((0x7F != (unsigned char)(c)) && (0x40 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xFC))
int CMeasurementTabView::isJMS( const char *str, int nPos )
{
	int i;
	int state; // { 0, 1, 2 }

	// 0 = ï¿½Pï¿½oï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½
	// 1 = ï¿½Qï¿½oï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ì‘ï¿½Pï¿½oï¿½Cï¿½g
	// 2 = ï¿½Qï¿½oï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ì‘ï¿½Qï¿½oï¿½Cï¿½g
	state = 0;
	for( i = 0; str[i] != '\0'; i++ )
	{
		if		( ( state == 0 ) && ( jms1( str[i] ) ) ) state = 1; // 0 -> 1
		else if ( ( state == 1 ) && ( jms2( str[i] ) ) ) state = 2; // 1 -> 2
		else if ( ( state == 2 ) && ( jms1( str[i] ) ) ) state = 1; // 2 -> 1
		else											 state = 0; // 2 -> 0, ï¿½ï¿½ï¿½Ì‘ï¿½

		// str[nPos] ï¿½Å‚Ì�ï¿½Ô‚ï¿½Ô‚ï¿½ï¿½B
		if ( i == nPos ) return state;
	}
	return 0;
}//isJMS



void CMeasurementTabView::OnUpdateSaveResultData(CCmdUI* pCmdUI)
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ï¿½ command update UI ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	//2009.11.03 bagus MS --{--
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í•Û‘ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	{
		RCP_DATA rcp;
		m_pDoc->GetRcpData(&rcp);
		if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
			pCmdUI->Enable(FALSE);
			return ;
		}
	}
	//2009.11.03 bagus MS --}--


	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½h
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
	else //ï¿½Ê�í‘ªï¿½èƒ‚ï¿½[ï¿½h(ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½Ü‚ï¿½)
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
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
	OnSaveAsMain(MEASUREMENT_TOOLBAR_SAVE);
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
/* deleted 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
//	  // TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½}ï¿½ï¿½ï¿½h ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//
////ï¿½cï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½ÌƒZï¿½[ï¿½uï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½
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
//////		MessageBox("Save UnCompleted.", "", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
////		MessageBox("ï¿½Û‘ï¿½ï¿½ï¿½ï¿½s", "NanoSpec", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
////	}
////  else
////  {
////	  m_bToolbarSaveCompFlg = TRUE; //ï¿½cï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½Å•Û‘ï¿½ï¿½ï¿½
////  }
//	  if(iCheck != 1 && iCheck != 3){ //ï¿½Û‘ï¿½ï¿½ï¿½ï¿½s
//		  AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //ï¿½ï¿½ï¿½èŒ‹ï¿½Ê•Û‘ï¿½ï¿½ï¿½ï¿½s
//		  if( m_pDoc->GetHostMode() == HOST_LOCAL ){
//// Kojika 20090526 Change
////		  MessageBox("Save UnCompleted.", "NanoSpec", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
////		  MessageBox("ï¿½Û‘ï¿½ï¿½É�ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½", "NanoSpec", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
//			  LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
//			  LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
//			  MessageBox(strBuffer, strTitle, MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
//// Kojika 20090526 Change END
//		  }
//	  }
//	  else if(iCheck == 3){//ï¿½Û‘ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½
//		   //ï¿½Û‘ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½È‚Ì‚Å‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
//	  }
//	  else{//ï¿½Û‘ï¿½ï¿½ï¿½ï¿½ï¿½
//		  m_bToolbarSaveCompFlg = TRUE; //ï¿½cï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½Å•Û‘ï¿½ï¿½ï¿½
//	  }
//	  //Saiki 20090406 Change <-----
//
/* deleted 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
}





// ==========================================================================
//ï¿½ï¿½ï¿½è’†ï¿½Ì‘ï¿½ï¿½ï¿½Iï¿½ï¿½ ï¿½ï¿½ï¿½Ìƒ{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½/ï¿½ï¿½ï¿½ï¿½ï¿½Ì�Ø‘Ö‚ï¿½
//ï¿½Ê�í‘ªï¿½ï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½pï¿½B ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½Å‚Ì�Ø‘Ö‚ï¿½ï¿½ï¿½ManualMeasurementFormView.cppï¿½É‹Lï¿½q
void CMeasurementTabView::ButtonEnableChange(int Select)
{
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	WORD wNumScans;


	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‘ï¿½ï¿½ï¿½ï¿½Ô‚É‚ï¿½ï¿½ï¿½ï¿½ï¿½ç‚¸ï¿½Aï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) //ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½
	{
		if(m_pDoc->GetMeasMode() == MEASMODE_SEQ_OR_REMEASURE) //ï¿½Ê�í‘ªï¿½ï¿½orï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[
		{
			//IDD_MAIN_MENU_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE); //[LOG ON]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

			//IDD_MEASUREMENT_PROCESS_FORMï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
//			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_CANCEL_BUTTON)->EnableWindow(FALSE); 	//[ï¿½ï¿½ï¿½è’†ï¿½~]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->EnableWindow(FALSE);		//[ï¿½ï¿½ï¿½è’†ï¿½f(ï¿½ÄŠJ)]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[REMEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);	//[ï¿½Ê’uï¿½â�³ï¿½ï¿½Ä‘ï¿½ï¿½ï¿½]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

			//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Iï¿½ğ–³Œï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ğ–³Œï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			//Saiki 20090410 Add ----->
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö–ï¿½ï¿½ï¿½
			//Saiki 20090410 Add <-----
			//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
			m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
		}
		return;
	}


	switch(Select){
	case MEASUREMENT_SEQ_S:

		//IDD_MEASUREMENT_PROCESS_FORMï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[REMEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);	//[ï¿½Ê’uï¿½â�³ï¿½ï¿½Ä‘ï¿½ï¿½ï¿½]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

		//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Iï¿½ğ–³Œï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ğ–³Œï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö–ï¿½ï¿½ï¿½
		//Saiki 20090410 Add <-----

		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg = FALSE;

		break;


	case MEASUREMENT_SEQ_E:

		//IDD_MEASUREMENT_PROCESS_FORMï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_CANCEL_BUTTON)->EnableWindow(TRUE);	//[ï¿½ï¿½ï¿½è’†ï¿½~]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->EnableWindow(TRUE);	//[ï¿½ï¿½ï¿½è’†ï¿½f(ï¿½ÄŠJ)]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		//2009.10.28 bagus 2point-distance --{--
		wNumScans = rcp_data.StageProgInfoHdr.wNumScans;
		if(rcp_data.StageProgInfoHdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.StageProgInfoHdr.wScanType == SCAN_TYPE_SR_DISTANCE){
			wNumScans *= 2;
		}
		//if(rcp_data.StageProgInfoHdr.wNumScans == m_pDoc->GetStoreCount()) //ï¿½ï¿½ï¿½è�”ï¿½ï¿½ï¿½\ï¿½è‘ªï¿½ï¿½É“ï¿½ï¿½Bï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
		if(wNumScans == m_pDoc->GetStoreCount()) //ï¿½ï¿½ï¿½è�”ï¿½ï¿½ï¿½\ï¿½è‘ªï¿½ï¿½É“ï¿½ï¿½Bï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
		//2009.10.28 bagus 2point-distance --}--
		{
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(TRUE);		//[REMEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(TRUE);//[ï¿½Ê’uï¿½â�³ï¿½ï¿½Ä‘ï¿½ï¿½ï¿½]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		}
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);				//[EXIT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		//2009.12.20 bagus ï¿½Cï¿½ï¿½ --{--
		((CMeasurementProcessView *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0)))->ChangeOperationEnable(FALSE);
		//2009.12.20 bagus ï¿½Cï¿½ï¿½ --}--

		//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Iï¿½ï¿½Lï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½Lï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE);	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö—Lï¿½ï¿½
		//Saiki 20090410 Add <-----

		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;


//	  case MEASUREMENT_TEST_S:
//		  break;

//	  case MEASUREMENT_TEST_E:
//		  break;


	case MEAS_TAB_VIEW_CREATE:
		//IDD_MAIN_MENU_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(FALSE);		//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(FALSE);	//[RECIPE]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE);		//[LOG ON]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		{
			//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ğ–³Œï¿½
		}


		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

		break;


	case MEAS_TAB_VIEW_DELETE:
		//IDD_MAIN_MENU_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(TRUE);//[RECIPE]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½

		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg2 = TRUE;
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;
	}

}



void CMeasurementTabView::OnTimer(UINT nIDEvent)
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½Éƒï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚©ï¿½Ü‚ï¿½ï¿½Íƒfï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	int iProcessStatus = m_pDoc->GetProcessStatus();
	int iHostMode = m_pDoc->GetHostMode();
	int iMeasMode = m_pDoc->GetMeasMode();	//ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½æ“¾
	int iManuMeasMode = m_pDoc->GetManuMeasMode();

	if((iProcessStatus != m_iProcessStatusOld)||(iHostMode != m_iHostModeOld))
	{
		switch (nIDEvent)
		{
			case TABVIEW_ENABLE_CHANGE_TIMER:		// 50ms ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Cï¿½}

//				int iMeasMode = m_pDoc->GetMeasMode();	//ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½æ“¾
				if(iMeasMode == MEASMODE_SEQ_OR_REMEASURE) //ï¿½Ê�í‘ªï¿½ï¿½orï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[
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
				else if(iMeasMode == MEASMODE_MANUALMEASURE) //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
				{
					//ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒ{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í�AManualMeasurementFormView.cppï¿½É‹Lï¿½q

				}
//			  else if(iMeasMode == MEASMODE_TEST) //ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½h
//			  {
//
//			  }
				else
				{
					//ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
				}

				break;
		}
	}

	//ï¿½Oï¿½ï¿½lï¿½Xï¿½V
	m_iProcessStatusOld = iProcessStatus;
	m_iHostModeOld = iHostMode;



	//ï¿½Oï¿½ï¿½lï¿½Xï¿½V
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

	return SHFileOperation( &tSHFile ); // ï¿½ï¿½ï¿½s

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

	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½Éƒï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	//ï¿½Xï¿½eï¿½[ï¿½^ï¿½Xï¿½oï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(0, "");

}

/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
void CMeasurementTabView::OnSaveAsMain(int iSaveWay)
{
	CString strBuffer, strTitle;

	int iCheck=0;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	//2009.11.03 bagus MS --{--
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Í•Û‘ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		iCheck = 1;
	}else{
		iCheck = SaveMeasConAddRevi(rcp_data.szRecipeName, iSaveWay);
	}
	//2009.11.03 bagus MS --}--


	if(iCheck != 1 && iCheck != 3){ //ï¿½Û‘ï¿½ï¿½ï¿½ï¿½s
		AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //ï¿½ï¿½ï¿½èŒ‹ï¿½Ê•Û‘ï¿½ï¿½ï¿½ï¿½s
		if( m_pDoc->GetHostMode() == HOST_LOCAL ){
			LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
		}
	}
	else if(iCheck == 3){//ï¿½Û‘ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½
			//ï¿½Û‘ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½È‚Ì‚Å‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	}
	else{//ï¿½Û‘ï¿½ï¿½ï¿½ï¿½ï¿½
		m_bToolbarSaveCompFlg = TRUE; //ï¿½cï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½Å•Û‘ï¿½ï¿½ï¿½
	}
}
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
void CMeasurementTabView::OnSaveAs()
{
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
	OnSaveAsMain(MEASUREMENT_TOOLBAR_SAVE_AS);
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
/* deleted 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
//	  //ï¿½cï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½ÌƒZï¿½[ï¿½uï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ“ï¿½ï¿½lï¿½Ì�ï¿½ï¿½ï¿½
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
////		MessageBox("ï¿½Û‘ï¿½ï¿½ï¿½ï¿½s", "NanoSpec", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
////	}
////	else{
////	  m_bToolbarSaveCompFlg = TRUE; //ï¿½cï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½Å•Û‘ï¿½ï¿½ï¿½
////  }
////  }
//	  if(iCheck != 1 && iCheck != 3){ //ï¿½Û‘ï¿½ï¿½ï¿½ï¿½s
//		  AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //ï¿½ï¿½ï¿½èŒ‹ï¿½Ê•Û‘ï¿½ï¿½ï¿½ï¿½s
//		  if( m_pDoc->GetHostMode() == HOST_LOCAL ){
//// Kojika 20090526 Change
////				  MessageBox("Save UnCompleted.", "NanoSpec", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
////		  MessageBox("ï¿½Û‘ï¿½ï¿½É�ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½", "NanoSpec", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
//			  LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
//			  LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec")
//			  MessageBox(strBuffer, strTitle, MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
//// Kojika 20090526 Change END
//		  }
//	  }
//	  else if(iCheck == 3){//ï¿½Û‘ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½
//		   //ï¿½Û‘ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½È‚Ì‚Å‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
//	  }
//	  else{//ï¿½Û‘ï¿½ï¿½ï¿½ï¿½ï¿½
//		  m_bToolbarSaveCompFlg = TRUE; //ï¿½cï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½Å•Û‘ï¿½ï¿½ï¿½
//	  }
//	  //Saiki 20090406 Change <-----
/* deleted 2009.06.16 hmenjo ï¿½gï¿½pï¿½Ö�~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
}

void CMeasurementTabView::OnUpdateSaveAs(CCmdUI* pCmdUI)
{
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){ //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½h
		if(m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP){
			pCmdUI->Enable(TRUE);
		}
		else{
			pCmdUI->Enable(FALSE);
		}
	}
	else{ //ï¿½Ê�í‘ªï¿½èƒ‚ï¿½[ï¿½h(ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½Ü‚ï¿½)

		if(m_pDoc->GetProcessStatus() == PROCESS_WAIT){
			pCmdUI->Enable(TRUE);
		}
		else{
			pCmdUI->Enable(FALSE);
		}
	}
}

/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
/*
 *	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
 *	ï¿½@ï¿½\ï¿½Ì�ï¿½ï¿½sï¿½Â”\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½Nï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
 *		DWORD dwMode
 *				ï¿½ï¿½ï¿½Oï¿½Fï¿½Sï¿½`ï¿½Fï¿½bï¿½N
 *				ï¿½ï¿½ï¿½Pï¿½Fï¿½Ä‘ï¿½ï¿½è��ï¿½pï¿½`ï¿½Fï¿½bï¿½N
 *				ï¿½ï¿½Lï¿½ÈŠOï¿½Fï¿½ï¿½ï¿½Oï¿½Æ“ï¿½ï¿½ï¿½
 */
BOOL CMeasurementTabView::MRCS_CheckPermission(DWORD dwMode/* = 0*/)
{
	BOOL l_bRc = FALSE;

	/* ï¿½è“®ï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠOï¿½Í�ï¿½ï¿½O	*/
	if (m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) {
		return FALSE;
	}

	/* ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½Í�ï¿½ï¿½O	*/
	if (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS) {
		return FALSE;
	}

	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í�ï¿½ï¿½O	*/
	if (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		return FALSE;
	}

	/* SR_DISTANCE ï¿½Í�ï¿½ï¿½O	*/
	if (m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE) {
		return FALSE;
	}

	/* GTR ï¿½Í�Ä‘ï¿½ï¿½ï¿½Ì‚İ�ï¿½ï¿½O	*/
	if ((dwMode == 1) &&
		(m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)) {
		return FALSE;
	}

	return TRUE;
}
/*
 *	ï¿½@ï¿½\ï¿½Ì‰Eï¿½_ï¿½uï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½Ì�ï¿½ï¿½sï¿½Â”\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½Nï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
 */
BOOL CMeasurementTabView::MRCS_CheckRunPermission()
{
	BOOL l_bRc = FALSE;

	/* ï¿½è“®ï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠOï¿½Í�ï¿½ï¿½O	*/
	if (m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) {
		return FALSE;
	}
/* added 2016.06.02 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
	/* ï¿½è“®ï¿½ï¿½ï¿½[ï¿½hï¿½Å‘ï¿½ï¿½èŠ®ï¿½ï¿½ï¿½Í�ï¿½ï¿½O	*/
	if (m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP) {
		return FALSE;
	}
/* added 2016.06.02 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */

	if (FALSE != m_pMainFrame->m_ManualMeasMenuEnableFlg) {
		l_bRc = TRUE;
	}

	return l_bRc;
}
/*
 *	ï¿½Eï¿½_ï¿½uï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
 */
BOOL CMeasurementTabView::MRCS_RDblClk(POINT ptGrid, CGridCtrl* pclsGrid/* = 0*/)
{
	BOOL l_bRc = FALSE;
	int i;

	if (0 == pclsGrid) {
		return FALSE;
	}

	CGridCtrl* l_pclsGrid = pclsGrid;

	if (FALSE != this->MRCS_CheckRunPermission()) {
		if (FALSE != this->MRCS_CheckPermission()) {
			CCellID l_clsCellID = l_pclsGrid->GetCellFromPt(ptGrid, FALSE);
			if (FALSE != l_clsCellID.IsValid()) {
				/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½Í—Lï¿½ï¿½	*/
				int l_iMeasCount = m_pDoc->GetMeasurePointCount();
				if (l_clsCellID.row <= l_iMeasCount) {
					/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½s No. ï¿½ï¿½ï¿½ï¿½ï¿½è�”ï¿½È“ï¿½	*/
					WORD l_wClickLine = l_clsCellID.row;
					/* ï¿½Lï¿½ï¿½ï¿½È�sï¿½Å‚Ì‘Iï¿½ï¿½ï¿½ï¿½Ô‚Ì�sï¿½ï¿½ï¿½ï¿½ï¿½æ“¾	*/
//	ï¿½eï¿½Xï¿½gï¿½p		int l_iSelCnt = m_MeasurementListGrid.GetSelectedCount();
//	ï¿½eï¿½Xï¿½gï¿½p		if (FALSE != m_MeasurementListGrid.IsCellSelected(l_clsCellID)) {
					int l_iSelCount = 0;
					BOOL l_bIncludedInSelected = FALSE;	/* ï¿½Iï¿½ï¿½ï¿½sï¿½ï¿½ï¿½É�wï¿½ï¿½sï¿½ï¿½ï¿½İ‚ï¿½tï¿½ï¿½ï¿½O	*/
					for (i = 1; i <= l_iMeasCount; i++) {
						if (FALSE != l_pclsGrid->IsCellSelected(i, l_clsCellID.col)) {
							/* ï¿½Iï¿½ï¿½ï¿½s	*/
							l_iSelCount++;
							if (1 == this->MRCS_SettingGet(i)) {
								l_bIncludedInSelected = TRUE;
							}
							m_MRCS_dwSettings[i] |= 0x80000000;
						} else {
							/* ï¿½ï¿½Iï¿½ï¿½ï¿½s	*/
							m_MRCS_dwSettings[i] &= 0x7fffffff;
						}
					}
					/* ï¿½Â”\ï¿½È‘ï¿½ï¿½ï¿½Ì”ï¿½ï¿½ï¿½	*/
					DWORD l_dwSecondCheckID = 0;	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ID	*/
					switch (l_iSelCount) {
					case 0:		/* ï¿½Iï¿½ï¿½ï¿½ï¿½Ô–ï¿½ï¿½ï¿½	*/
						l_dwSecondCheckID = 2;
						break;
					case 1:		/* ï¿½Pï¿½sï¿½Iï¿½ï¿½	*/
						if (FALSE != l_pclsGrid->IsCellSelected(l_clsCellID)) {
							/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½sï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½	*/
							l_dwSecondCheckID = 3;
						} else {
							/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½sï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½	*/
							l_dwSecondCheckID = 2;
						}
						break;
					default:	/* ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½Iï¿½ï¿½	*/
						if (FALSE != l_pclsGrid->IsCellSelected(l_clsCellID)) {
							/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½sï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½	*/
							l_dwSecondCheckID = 1;
						} else {
							/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½sï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½	*/
							l_dwSecondCheckID = 2;
						}
						break;
					}
					WORD l_wEnableSelect = 0;
					switch (l_dwSecondCheckID) {
					case 1:
						if (FALSE != l_bIncludedInSelected) {
							/* ï¿½Iï¿½ï¿½ï¿½ï¿½Ô‚Ì�sï¿½É�wï¿½ï¿½sï¿½ï¿½ï¿½İ‚ï¿½	*/
							l_wEnableSelect |= 0x0001;	/* ï¿½wï¿½ï¿½s	*/
							l_wEnableSelect |= 0x0002;	/* ï¿½wï¿½ï¿½ï¿½ï¿½ï¿½	*/
						} else {
							/* ï¿½Iï¿½ï¿½ï¿½ï¿½Ô‚Ì�sï¿½É�wï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
							l_wEnableSelect |= 0x0001;	/* ï¿½wï¿½ï¿½s	*/
						}
						break;
					case 2:
						if (2 != this->MRCS_SettingGet(l_clsCellID.row)) {
							/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½sï¿½ï¿½ï¿½í�œï¿½Ï‚İ�sï¿½Å‚È‚ï¿½	*/
							if (1 < (l_iMeasCount - MRCS_DeleteCountGet())) {
								/* ï¿½ï¿½í�œï¿½Ï‚İ�sï¿½ï¿½ï¿½Qï¿½È�ï¿½Ì�ê�‡ï¿½Ì‚İ�í�œï¿½Â”\ï¿½Å‚ï¿½ï¿½D	*/
								l_wEnableSelect |= 0x0004;	/* ï¿½í�œ	*/
							}
							if (FALSE != this->MRCS_CheckPermission(1)) {
								l_wEnableSelect |= 0x0010;	/* ï¿½Ä‘ï¿½ï¿½ï¿½	*/
							}
						} else {
							/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½sï¿½ï¿½ï¿½í�œï¿½Ï‚İ�sï¿½Å‚ï¿½ï¿½ï¿½	*/
							l_wEnableSelect |= 0x0008;	/* ï¿½í�œï¿½ï¿½ï¿½ï¿½	*/
							if (FALSE != this->MRCS_CheckPermission(1)) {
								l_wEnableSelect |= 0x0010;	/* ï¿½Ä‘ï¿½ï¿½ï¿½	*/
							}
						}
						break;
					case 3:
						if ((1 != this->MRCS_SettingGet(l_clsCellID.row)) &&
							(2 != this->MRCS_SettingGet(l_clsCellID.row))) {
							/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½sï¿½ï¿½ï¿½wï¿½ï¿½sï¿½Å‚È‚ï¿½ï¿½Cï¿½í�œï¿½Ï‚İ�sï¿½Å‚ï¿½ï¿½È‚ï¿½	*/
							l_wEnableSelect |= 0x0001;	/* ï¿½wï¿½ï¿½s	*/
						} else if (1 == this->MRCS_SettingGet(l_clsCellID.row)) {
							/* ï¿½Nï¿½ï¿½ï¿½bï¿½Nï¿½sï¿½ï¿½ï¿½wï¿½ï¿½sï¿½Å‚ï¿½ï¿½ï¿½	*/
							l_wEnableSelect |= 0x0002;	/* ï¿½wï¿½ï¿½ï¿½ï¿½ï¿½	*/
						}
						break;
					default:
						/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½	*/
						break;
					}
					if (0 < MRCS_DeleteCountGet()) {
						/* ï¿½í�œï¿½Ï‚İ�sï¿½ï¿½ï¿½İ‚ï¿½	*/
						l_wEnableSelect |= 0x1000;	/* ï¿½í�œï¿½Ï‚İ�sï¿½İ‚ï¿½	*/
					}
					if (0 < MRCS_IncludeCountGet()) {
						/* ï¿½wï¿½ï¿½sï¿½ï¿½ï¿½İ‚ï¿½	*/
						l_wEnableSelect |= 0x2000;	/* ï¿½wï¿½ï¿½sï¿½İ‚ï¿½	*/
					}
					if (0 != l_wEnableSelect) {
						/* ï¿½ï¿½ï¿½ï¿½ï¿½Ê‚ï¿½\ï¿½ï¿½	*/
						l_bRc = TRUE;
						CManuReMeasReCalcStatDlg l_dlgMRCS;
						l_dlgMRCS.ParamSet(MAKELPARAM(l_wEnableSelect, l_wClickLine));
						if (IDOK == l_dlgMRCS.DoModal()) {
							/* OK	*/
							DWORD l_dwRslt;
							l_dlgMRCS.ParamGet((LPARAM*) &l_dwRslt);
							WORD l_wSelectNo = LOWORD(l_dwRslt);
							BOOL l_bCalcStat = FALSE;	/* ï¿½ï¿½ï¿½vï¿½vï¿½Zï¿½Kï¿½v	*/
							switch (l_wSelectNo) {
							case 1:		/* ï¿½wï¿½ï¿½s	*/
							case 2:		/* ï¿½wï¿½ï¿½ï¿½ï¿½ï¿½	*/
								l_bCalcStat = TRUE;
								for (i = 1; i <= l_iMeasCount; i++) {
									if (0 != (m_MRCS_dwSettings[i] & 0x80000000)) {
										/* ï¿½Iï¿½ï¿½ï¿½s	*/
										switch (l_wSelectNo) {
										case 1:		/* ï¿½wï¿½ï¿½s	*/
											if (2 != this->MRCS_SettingGet(i)) {
												/* ï¿½í�œï¿½Ï‚İ�sï¿½ÈŠO	*/
												this->MRCS_SettingSet(i, 1);
												this->MRCS_GridLineColorSet(i, CYAN_COLOR);
												this->MRCS_IncludeCountInc();
											}
											break;
										case 2:		/* ï¿½wï¿½ï¿½ï¿½ï¿½ï¿½	*/
											if (1 == this->MRCS_SettingGet(i)) {
												/* ï¿½wï¿½ï¿½s	*/
												this->MRCS_SettingSet(i, 0);
												this->MRCS_GridLineColorSet(i, CLR_DEFAULT);
												this->MRCS_IncludeCountDec();
											}
											break;
										}
									}
								}
								break;
							case 3:		/* ï¿½í�œ	*/
								l_bCalcStat = TRUE;
								if (1 == this->MRCS_SettingGet(l_wClickLine)) {
									/* ï¿½wï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Íƒfï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
									this->MRCS_IncludeCountDec();
								}
								this->MRCS_SettingSet(l_wClickLine, 2);
								this->MRCS_GridLineColorSet(l_wClickLine, MAGENTA_COLOR);
								this->MRCS_DeleteCountInc();
								break;
							case 4:		/* ï¿½í�œï¿½ï¿½ï¿½ï¿½	*/
								if (0 == MRCS_IncludeCountGet()) {
									/* ï¿½wï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Ì‚İ“ï¿½ï¿½vï¿½vï¿½Zï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
									l_bCalcStat = TRUE;
								}
								this->MRCS_SettingSet(l_wClickLine, 0);
								this->MRCS_GridLineColorSet(l_wClickLine, CLR_DEFAULT);
								this->MRCS_DeleteCountDec();
								break;
							case 5:		/* ï¿½Ä‘ï¿½ï¿½ï¿½	*/
								m_MRCS_wReMeasureNo = l_wClickLine;
								if (2 == this->MRCS_SettingGet(m_MRCS_wReMeasureNo)) {
									/* ï¿½í�œï¿½Ï‚İ�sï¿½Å‚ï¿½ï¿½ï¿½	*/
									this->MRCS_DeleteCountDec();
									this->MRCS_SettingSet(m_MRCS_wReMeasureNo, 0);
									this->MRCS_GridLineColorSet(m_MRCS_wReMeasureNo, CLR_DEFAULT);
								}
								m_pMainFrame->m_pManuMeaFormViewObj->ManuMeas();
								break;
							case 13:	/* ï¿½ï¿½ï¿½×‚Ä‚Ì�í�œï¿½fï¿½[ï¿½^ï¿½ğ•œŠï¿½	*/
							case 14:	/* ï¿½ï¿½ï¿½×‚Ä‚Ì�wï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
								{
									WORD l_wDiffPrm;
									switch (l_wSelectNo) {
									case 13:	/* ï¿½ï¿½ï¿½×‚Ä‚Ì�í�œï¿½fï¿½[ï¿½^ï¿½ğ•œŠï¿½	*/
										if (0 == MRCS_IncludeCountGet()) {
											/* ï¿½wï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Ì‚İ“ï¿½ï¿½vï¿½vï¿½Zï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
											l_bCalcStat = TRUE;
										}
										l_wDiffPrm = 2;
										m_MRCS_dwSettings[0] &= 0xffff0000;
										break;
									case 14:	/* ï¿½ï¿½ï¿½×‚Ä‚Ì�wï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
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
							/* ï¿½Iï¿½ï¿½ï¿½ï¿½Ô‚ï¿½ï¿½ï¿½ï¿½ï¿½	*/
							for (i = 1; i <= SCAN_POINT_MAX; i++) {
								m_MRCS_dwSettings[i] &= 0x7fffffff;
							}
							m_pMeasurementListDlg->MeasurementListGrid_UnSelectAll();
							m_pMeasurementGraphDlg->MeasurementListGrid_UnSelectAll();
//	ï¿½Iï¿½ï¿½ï¿½ï¿½Ô‚É‚Í‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Æ‚É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
//							if (5 == l_wSelectNo) {
//								/* ï¿½Ä‘ï¿½ï¿½ï¿½ï¿½Í‚ï¿½ï¿½Ì�sï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½Ô‚É‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
//								m_MRCS_dwSettings[l_wClickLine] |= 0x80000000;
//							}
							/* ï¿½ï¿½ï¿½vï¿½vï¿½Z	*/
							if (FALSE != l_bCalcStat) {
								STATISTICS l_Statistics[ADAPRESULT_COLS_MAX];	/* ï¿½ï¿½ï¿½vï¿½fï¿½[ï¿½^	*/
								memset(l_Statistics, 0, sizeof(l_Statistics));
								/* ï¿½ï¿½ï¿½vï¿½vï¿½Z	*/
								m_pMainFrame->GlassDataStatistics(
										l_Statistics,
										m_pDoc->GetDispLabelCount(),
										m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption
									);
								/* ï¿½ï¿½ï¿½vï¿½fï¿½[ï¿½^ï¿½Û‘ï¿½	*/
								m_pDoc->SetStatisticsData(l_Statistics);
								/* ï¿½ï¿½ï¿½vï¿½fï¿½[ï¿½^ï¿½\ï¿½ï¿½	*/
//	ï¿½sï¿½vï¿½Å‚ï¿½ï¿½D							m_pDoc->SetShowCompPointGraph(l_iMeasCount - 1);
								/*		ï¿½ï¿½ï¿½vï¿½fï¿½[ï¿½^ï¿½Ì•\ï¿½ï¿½ï¿½ï¿½ CMeasurementListDlg ï¿½Ì‚İ‚Å�sï¿½ï¿½ï¿½Ä‚ï¿½ï¿½é‚½ï¿½ï¿½
										CMeasurementGraphDlg ï¿½ï¿½ï¿½Í•sï¿½vï¿½Å‚ï¿½ï¿½D	*/
								m_pDoc->SetShowCompPointList(l_iMeasCount - 1);
							}
						} else {
							/* Cancel	*/
							/*	ï¿½ï¿½ï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½D	*/
						}
					}
				}
			}
		}
	}

	return l_bRc;
}
/*
 *	ï¿½İ’ï¿½oï¿½bï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½İ’ï¿½Ì‚İ�ï¿½ï¿½oï¿½ï¿½
 */
WORD CMeasurementTabView::MRCS_SettingGet(long lIndex)
{
	return LOWORD(m_MRCS_dwSettings[lIndex]);
}
/*
 *	ï¿½İ’ï¿½oï¿½bï¿½tï¿½@ï¿½É�İ’ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
 */
void CMeasurementTabView::MRCS_SettingSet(long lIndex, WORD wSetting)
{
	WORD l_wStatus = HIWORD(m_MRCS_dwSettings[lIndex]);
	m_MRCS_dwSettings[lIndex] = (DWORD) MAKELONG(wSetting, l_wStatus);
}
/*
 *	ï¿½İ’ï¿½oï¿½bï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Ô‚Ì‚İ�ï¿½ï¿½oï¿½ï¿½
 */
WORD CMeasurementTabView::MRCS_SettingStatusGet(long lIndex)
{
	return HIWORD(m_MRCS_dwSettings[lIndex]);
}
/*
 *	ï¿½İ’ï¿½oï¿½bï¿½tï¿½@ï¿½É�ï¿½Ô‚Ì‚İ�ï¿½ï¿½ï¿½ï¿½ï¿½
 */
void CMeasurementTabView::MRCS_SettingStatusSet(long lIndex, WORD wStatus)
{
	WORD l_wSetting = LOWORD(m_MRCS_dwSettings[lIndex]);
	m_MRCS_dwSettings[lIndex] = (DWORD) MAKELONG(l_wSetting, wStatus);
}
/*
 *	ï¿½İ’ï¿½oï¿½bï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½Sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½oï¿½ï¿½
 */
DWORD CMeasurementTabView::MRCS_SettingAllGet(long lIndex)
{
	return m_MRCS_dwSettings[lIndex];
}
/*
 *	ï¿½wï¿½èƒŠï¿½Xï¿½gï¿½sï¿½Ì�Fï¿½İ’ï¿½
 */
void CMeasurementTabView::MRCS_GridLineColorSet(long lIndex, COLORREF cr/* = CLR_DEFAULT*/)
{
	int i;
	/* CMeasurementListDlg	*/
	for (i = 1; i < m_pMeasurementListDlg->m_MeasurementListGrid.GetColumnCount(); i++) {
		m_pMeasurementListDlg->m_MeasurementListGrid.SetItemBkColour(lIndex, i, cr);
	}
	/* CMeasurementGraphDlg	*/
	for (i = 1; i < m_pMeasurementGraphDlg->m_MeasurementListGrid.GetColumnCount(); i++) {
		m_pMeasurementGraphDlg->m_MeasurementListGrid.SetItemBkColour(lIndex, i, cr);
	}
}
/*
 *	ï¿½í�œï¿½Ï‚İ�sï¿½ï¿½ï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½æ“¾
 */
WORD CMeasurementTabView::MRCS_DeleteCountGet()
{
	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	ï¿½í�œï¿½Ï‚İ�sï¿½ï¿½ï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Z
 */
WORD CMeasurementTabView::MRCS_DeleteCountInc(WORD wCount/* = 1*/)
{
	WORD l_wCount = LOWORD(m_MRCS_dwSettings[0]);
	l_wCount += wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(l_wCount, HIWORD(m_MRCS_dwSettings[0]));

	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	ï¿½í�œï¿½Ï‚İ�sï¿½ï¿½ï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Z
 */
WORD CMeasurementTabView::MRCS_DeleteCountDec(WORD wCount/* = 1*/)
{
	WORD l_wCount = LOWORD(m_MRCS_dwSettings[0]);
	l_wCount -= wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(l_wCount, HIWORD(m_MRCS_dwSettings[0]));

	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	ï¿½wï¿½ï¿½sï¿½ï¿½ï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½æ“¾
 */
WORD CMeasurementTabView::MRCS_IncludeCountGet()
{
	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	ï¿½wï¿½ï¿½sï¿½ï¿½ï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Z
 */
WORD CMeasurementTabView::MRCS_IncludeCountInc(WORD wCount/* = 1*/)
{
	WORD l_wCount = HIWORD(m_MRCS_dwSettings[0]);
	l_wCount += wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(LOWORD(m_MRCS_dwSettings[0]), l_wCount);

	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	ï¿½wï¿½ï¿½sï¿½ï¿½ï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Z
 */
WORD CMeasurementTabView::MRCS_IncludeCountDec(WORD wCount/* = 1*/)
{
	WORD l_wCount = HIWORD(m_MRCS_dwSettings[0]);
	l_wCount -= wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(LOWORD(m_MRCS_dwSettings[0]), l_wCount);

	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	ï¿½wï¿½ï¿½sï¿½ï¿½Ç‰ï¿½
 */
WORD CMeasurementTabView::MRCS_IncludeAdd(long lIndex)
{
	WORD l_wRc = this->MRCS_IncludeCountGet();

	if (0 == this->MRCS_SettingGet(lIndex)) {
		/* ï¿½wï¿½è‚ªï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Ì‚İ�İ’è‚µï¿½Ü‚ï¿½ï¿½D	*/
		this->MRCS_SettingSet(lIndex, 1);
		l_wRc = this->MRCS_IncludeCountInc();
	}

	return l_wRc;
}
/*
 *	ï¿½Ä‘ï¿½ï¿½ï¿½ No. ï¿½Ì�æ“¾
 */
WORD CMeasurementTabView::MRCS_ReMeasureNoGet()
{
	return m_MRCS_wReMeasureNo;
}
/*
 *	ï¿½Ä‘ï¿½ï¿½ï¿½ No. ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
 */
void CMeasurementTabView::MRCS_ReMeasureNoClr()
{
	m_MRCS_wReMeasureNo = 0;
}
/*
 *	ï¿½wï¿½ï¿½sï¿½Fï¿½Ô�ï¿½ï¿½Ì�İ’ï¿½
 */
void CMeasurementTabView::MRCS_IncludeColorSetNoSet(WORD wNo)
{
	m_MRCS_wIncludeColorSetNo = wNo;
}
/*
 *	ï¿½wï¿½ï¿½sï¿½Fï¿½Ô�ï¿½ï¿½Ì�æ“¾
 */
WORD CMeasurementTabView::MRCS_IncludeColorSetNoGet()
{
	return m_MRCS_wIncludeColorSetNo;
}
/*
 *	ï¿½wï¿½ï¿½sï¿½Fï¿½İ’ï¿½
 */
#define	MRCSREDRAWMETHOD	1	/* ï¿½Ä•`ï¿½ï¿½ï¿½ï¿½@	*/
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
		/* ï¿½ï¿½ï¿½ï¿½ï¿½İ’èŠ®ï¿½ï¿½	*/
		WORD l_wSetNo = m_MRCS_wIncludeColorSetNo;
		m_MRCS_bIncludeColorSetList = FALSE;
		m_MRCS_bIncludeColorSetGraph = FALSE;
		m_MRCS_wIncludeColorSetNo = 0;
		this->MRCS_GridLineColorSet(l_wSetNo, CYAN_COLOR);
		/*	ï¿½Fï¿½İ’ï¿½ï¿½Ì”ï¿½ï¿½fï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½Å‚ï¿½ï¿½ï¿½ï¿½Ì‚Å�C
			ï¿½Ä•`ï¿½ï¿½(ï¿½H)ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
		/* ï¿½Oï¿½ï¿½ï¿½bï¿½hï¿½\ï¿½ï¿½ï¿½Xï¿½V	*/
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
 *	ï¿½ï¿½ï¿½Êƒï¿½ï¿½Xï¿½gï¿½Ì•\ï¿½ï¿½ï¿½Ï‚İƒ|ï¿½Cï¿½ï¿½ï¿½gï¿½Ì‹Lï¿½ï¿½ï¿½Cï¿½æ“¾ï¿½Cï¿½Nï¿½ï¿½ï¿½A
 */
BOOL CMeasurementTabView::MRCS_ShowCompletionPointSnap(LPWORD pwList/* = 0*/, LPWORD pwGraph/* = 0*/)
{
	BOOL l_bRc = FALSE;

	if ((0 == pwList) && (0 == pwGraph)) {
		/* ï¿½Nï¿½ï¿½ï¿½A	*/
		m_MRCS_wShowCompletionPointSnapL = 0;
		m_MRCS_wShowCompletionPointSnapG = 0;
	} else
	if ((0 != pwList) && (0 != pwGraph)) {
		/* ï¿½Lï¿½ï¿½	*/
		m_MRCS_wShowCompletionPointSnapL = m_pDoc->GetShowCompPointList();
		m_MRCS_wShowCompletionPointSnapG = m_pDoc->GetShowCompPointGraph();
		*pwList = m_MRCS_wShowCompletionPointSnapL;
		*pwGraph = m_MRCS_wShowCompletionPointSnapG;
		l_bRc = TRUE;
	} else
	if (0 != pwList) {
		/* CMeasurementListDlg ï¿½ï¿½ï¿½æ“¾	*/
		*pwList = m_MRCS_wShowCompletionPointSnapL;
		m_MRCS_wShowCompletionPointSnapL = 0;
		l_bRc = TRUE;
	} else
	if (0 != pwGraph) {
		/* CMeasurementGraphDlg ï¿½ï¿½ï¿½æ“¾	*/
		*pwGraph = m_MRCS_wShowCompletionPointSnapG;
		m_MRCS_wShowCompletionPointSnapG = 0;
		l_bRc = TRUE;
	}

	return l_bRc;
}
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */
