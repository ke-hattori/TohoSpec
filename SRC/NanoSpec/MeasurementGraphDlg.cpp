// MeasurementGraphDlg.cpp : ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "NanoSpecDoc.h"
#include "MultiLangHelper.h"
#include "MeasurementTabView.h"
#include "MeasurementGraphDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Measurement Graph
//2009.09.09 bagus stress --{--
// 2009.09.09 K.Matsuo -->
#define MAX_GRAPH_DEFL_Y				(1.0)
#define MIN_GRAPH_DEFL_Y				(-5.0)
#define MAX_GRAPH_DXDY_Y				(15.0)
#define MIN_GRAPH_DXDY_Y				(-15.0)
#define MAX_GRAPH_STRESS_Y				(500.0)
#define MIN_GRAPH_STRESS_Y				(-500.0)
// 2009.09.09 K.Matsuo <--
//2009.09.09 bagus stress --}--

#define DEF_MEASUREMENT_GRID_ROWLINE_COUNT	(6+1)	//MeasurementListGridï¿½É‚ï¿½ï¿½å‚¤ï¿½ÇƒXï¿½Nï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½
#define DEF_LITENSITY_GRID_ROWLINE_COUNT	(31+1)	//IntensityGridï¿½É‚ï¿½ï¿½å‚¤ï¿½ÇƒXï¿½Nï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½


#define MEASUREMENTGRID_GRAPH_COL_MIN	(15)


// #########################################################################
// CMeasurementGraphDlg ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O
// #########################################################################

// ==========================================================================
//
CMeasurementGraphDlg::CMeasurementGraphDlg(int m_mode, UINT uResourceID /*= IDD_MEASUREMENT_GRAPH_DLG*/, CWnd* pParent /*=NULL*/)
	: CDialog(uResourceID, pParent)
{
	//{{AFX_DATA_INIT(CMeasurementGraphDlg)
	//}}AFX_DATA_INIT

	FontCreation();

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;
	m_iGraphDlgMode = m_mode;

	//2009.09.13 bagus stress --{--
	m_nGraphRow = -1;
	m_nGraphTab = -1;
	//2009.09.13 bagus stress --}--

	m_iModeStatus = 0;
	m_iDispPointNo = 0;
	m_nDispLine = -1;
	m_nDispSection = 0;
	m_nDataIndex = 0;

	m_dAnalysisData = new double[SCANDATA_POINT_MAX];
	m_dAnalysisGenData = new double[SCANDATA_POINT_MAX];
	m_seRawData = new SE_RAW_ONE_DATA[SCANDATA_POINT_MAX];

	::ZeroMemory(m_dAnalysisData, sizeof(double) * SCANDATA_POINT_MAX);
	::ZeroMemory(m_dAnalysisGenData, sizeof(double) * SCANDATA_POINT_MAX);
	::ZeroMemory(m_seRawData, sizeof(SE_RAW_ONE_DATA) * SCANDATA_POINT_MAX);

	m_LightIntensityListGrid.SetVirtualMode(TRUE);

}

// =========================================================================
//
CMeasurementGraphDlg::~CMeasurementGraphDlg()
{
	m_pDoc->SetShowCompPointGraph(0);

/* added 2015.04.12 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ASSERT ---------- { ---------- */
	if (m_pDoc->GetMeasMode() == MEASMODE_TEST) {
		m_LightIntensityListGrid.EnableSelection(FALSE);
	}
/* added 2015.04.12 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ASSERT ---------- } ---------- */
	m_Font.DeleteObject();

	if ( m_dAnalysisData ) {
		delete [] m_dAnalysisData;
		m_dAnalysisData = NULL;
	}
	if ( m_dAnalysisGenData ) {
		delete [] m_dAnalysisGenData;
		m_dAnalysisGenData = NULL;
	}
	if ( m_seRawData ) {
		delete [] m_seRawData;
		m_seRawData = NULL;
	}
}

// =========================================================================
//
void CMeasurementGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasurementGraphDlg)
	DDX_Control(pDX, IDC_MEASERE_GRAPH, m_MeasureGraph);
	DDX_Control(pDX, IDC_MEASUREMENT_LIST_GRID, m_MeasurementListGrid);
	DDX_Control(pDX, IDC_LIGHT_INTENSITY_LIST_GRID, m_LightIntensityListGrid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MEASURE_GRAPH_TAB, m_MeasureGraphTab);
	//2009.09.03 bagus se ï¿½Iï¿½[ï¿½gï¿½Xï¿½Pï¿½[ï¿½ï¿½ --{--
	DDX_Check(pDX, IDC_GRAPH_AUTO_SCALE, m_rcp_data.MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale);
	//2009.09.03 bagus se ï¿½Iï¿½[ï¿½gï¿½Xï¿½Pï¿½[ï¿½ï¿½ --}--
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CMeasurementGraphDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasurementGraphDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MEASURE_GRAPH_TAB, OnSelchangeMeasureGraphTab)
	ON_BN_CLICKED(IDC_GRAPH_AUTO_SCALE, OnGraphAutoScale)
	//}}AFX_MSG_MAP
	//ON_NOTIFY(NM_CLICK, IDC_MEASUREMENT_LIST_GRID, OnGridClick)
	ON_NOTIFY(GVN_SELCHANGING, IDC_MEASUREMENT_LIST_GRID, OnSelChangeing)
	ON_NOTIFY(GVN_GETDISPINFO, IDC_LIGHT_INTENSITY_LIST_GRID, OnDispInfo)
//	ON_MESSAGE(WM_MEAS_COMP_EVENT, SetMeasurementData)

	ON_MESSAGE(WM_TESTMODE_ONEPOINT_END, OnSetMeasurementData_TestMode)
	ON_MESSAGE(WM_TESTMODE_REF_END, OnSetRefData_TestMode)


END_MESSAGE_MAP()

// =========================================================================
// CMeasurementGraphDlg ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½

// =========================================================================
//
BOOL CMeasurementGraphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
	//ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½tï¿½Bï¿½Oï¿½İ’ï¿½Ç‚İ�ï¿½ï¿½ï¿½
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--

	///// Get Selected Recipe Name /////
//	((CMainFrame*)AfxGetMainWnd())->GetSelectListName(m_szName);

// 2009.05.15 k-matsuo -->
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
// 2009.05.15 k-matsuo <--


	///// Load Data /////
	LoadData();

	///// Set Data /////
	SetData();

	//2009.08.13 bagus stress --{--
	Tab_Init();
	//2009.08.13 bagus stress --}--

	///// Static Control /////
	if( m_iGraphDlgMode != GRAPHDLG_MODE_TEST_NOMAL &&
		m_iGraphDlgMode != GRAPHDLG_MODE_TEST_RAW ){
		m_stcMainRecipeName.SubclassWindow(GetDlgItem(IDC_MAIN_RECIPE_NAME)->GetSafeHwnd());
		m_stcMainRecipeName.SetBkColor(WATER_COLOR);
		m_stcHeadType.SubclassWindow(GetDlgItem(IDC_HEAD_TYPE)->GetSafeHwnd());
		m_stcHeadType.SetBkColor(WATER_COLOR);
	}
	m_stcMeasurementItem.SubclassWindow(GetDlgItem(IDC_MEASUREMENT_ITEM)->GetSafeHwnd());
	m_stcMeasurementItem.SetBkColor(WATER_COLOR);
	m_stcMeasurementPoint.SubclassWindow(GetDlgItem(IDC_MEASUREMENT_POINT)->GetSafeHwnd());
	m_stcMeasurementPoint.SetBkColor(WATER_COLOR);

	//2009.07.30 bagus stress ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½@ï¿½\ï¿½Ç‰ï¿½ --{--
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // ï¿½wï¿½bï¿½hï¿½^ï¿½Cï¿½v
	m_stcMeasurementLS.SubclassWindow(GetDlgItem(IDC_MEASUREMENT_LS)->GetSafeHwnd());
	m_stcMeasurementLS.SetBkColor(WATER_COLOR);
	if(wHeadType != HEAD_TYPE_STRESS){
		//2009.09.08 bagus stress --{--
		//ï¿½ï¿½ï¿½Í‘ï¿½ï¿½ï¿½ÈŠOï¿½ï¿½LSï¿½Ì•\ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Ì‚Åƒ|ï¿½Cï¿½ï¿½ï¿½gï¿½\ï¿½ï¿½ï¿½Ì‘å‚«ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç‚ï¿½
		RECT tRect1,tRect2;
		m_stcMeasurementPoint.GetWindowRect(&tRect1);
		m_stcMeasurementLS.GetWindowRect(&tRect2);
		this->ScreenToClient(&tRect1);
		this->ScreenToClient(&tRect2);
// 2010.01.13 bagus Stress --{--
//		m_stcMeasurementPoint.MoveWindow(tRect1.left ,tRect1.top,tRect2.right - (tRect1.left ),tRect2.bottom - tRect2.top);
		m_stcMeasurementPoint.MoveWindow(tRect1.left ,tRect1.top, 122, tRect2.bottom - tRect2.top);
// 2010.01.13 bagus Stress --}--
		//2009.09.08 bagus stress --}--
		m_stcMeasurementLS.ShowWindow(SW_HIDE);
	}else{
		CString strBuffer;
		LoadStringML(IDS_STRESS_MEAS_POINT_LABEL,strBuffer,"Meas Point&LS");
		SetDlgItemText(IDC_MEASUREMENT_POINT_LABEL,strBuffer);
	}
	//2009.07.30 bagus stress ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½@ï¿½\ï¿½Ç‰ï¿½ --}--

	//2010.01.11 bagus GTR --{--
	//ï¿½Kï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ß—ï¿½ï¿½Ì�ê�‡ï¿½É‚Í‚È‚É‚ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
	&& m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW
	){
		m_stcMeasurementPoint.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEASUREMENT_POINT_LABEL)->ShowWindow(SW_HIDE);
	}
	//2010.01.11 bagus GTR --}--

	///// Grid Control Initialize /////
	MeasurementListGrid_Init();
	SetBlankLightIntensityListGrid();
	{
		CNanoMeasureGraph::GraphConfig tConfig;
		memset(&tConfig,NULL,sizeof(tConfig));
		tConfig.CanvasBackColor = RGB(0xFF,0xFF,0xFF);
		tConfig.GraphBackColor = RGB(0xFF,0xFF,0xFF);
		m_MeasureGraph .SetGraphConfig (tConfig);
		m_MeasureGraph.Invalidate();
	}
	//2009.09.14 bagus stress --}--
	MeasurementListGrid_HeaderSet();

	///// Draw Measuremd Data /////
	///// ï¿½Lï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ÍˆÍ“Ç‚İ�ï¿½ï¿½ï¿½ /////
	//2009.08.31 bagus stress --{--
	if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_STRESS){
		m_iStartWaveLength = m_rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wStart;
		m_iEndWaveLength = m_rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd;
	//2009.09.20 bagus stress --{--
	}else if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SE){
	//2009.09.20 bagus stress --}--
		m_iStartWaveLength = m_rcp_data.MeasProgInfo.ScanParams._SE.WavelenRange.wStart;
		m_iEndWaveLength = m_rcp_data.MeasProgInfo.ScanParams._SE.WavelenRange.wEnd;

	}else{
		if( m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE ||
	// 2009.10.26 k-matsuo -->
			m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE ||
			m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G )
	// 2009.10.26 k-matsuo <--
		{
			m_iStartWaveLength = MIN_TARGET_WAVELENGTH;
	// 2009.05.15 k-matsuo -->
	//		m_iEndWaveLength = MAX_TARGET_WAVELENGTH;
			if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
				m_iEndWaveLength = MAX_TARGET_WAVELENGTH;
			else
				m_iEndWaveLength = MAX_TARGET_WAVELENGTH_800;
	// 2009.05.15 k-matsuo <--
		}
		else //if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
		{
			m_iStartWaveLength = m_rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wStart;
			m_iEndWaveLength = m_rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd;
		}
	}
	//2009.08.31 bagus stress --}--

	//2009.09.30 bagus sr --{--
	//ï¿½ß‚ï¿½
	//2009.09.19 bagus stress --{--
	DrawLightIntensityGraph_Init();
	//2009.09.19 bagus stress --}--
	//2009.09.30 bagus sr --{--


	//2009.09.19 bagus stress --{--
	//ï¿½Oï¿½ï¿½ï¿½tï¿½Iï¿½[ï¿½gï¿½Xï¿½Pï¿½[ï¿½ï¿½
	if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale){
		//m_MeasureGraph.SetAutoScaleMode(TRUE);
		((CButton *)GetDlgItem(IDC_GRAPH_AUTO_SCALE))->SetCheck(1);
	}else{
		//m_MeasureGraph.SetAutoScaleMode(FALSE);
		((CButton *)GetDlgItem(IDC_GRAPH_AUTO_SCALE))->SetCheck(0);
	}


	if(m_pDoc->GetMeasMode() != MEASMODE_TEST) //ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
	{
		SetTimer(SHOW_GRAPH_TIMER, 500, NULL);
	}


	m_bCellSelectedFlg = FALSE; //ï¿½Iï¿½ï¿½ï¿½sï¿½Ì—Lï¿½ï¿½ï¿½tï¿½ï¿½ï¿½O

/* added 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- { ---------- */
	if (m_pDoc->GetMeasMode() != MEASMODE_TEST) {
		CWnd* l_pcwndParent = this->GetParent();			// CTabCtrl
		RECT l_rectParentAR;	l_pcwndParent->GetWindowRect(&l_rectParentAR);
		/* ï¿½eï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½^ï¿½uï¿½ÆŒï¿½ï¿½ß•tï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å‚ï¿½ï¿½D	*/
		((CTabCtrl*) l_pcwndParent)->AdjustRect(FALSE, &l_rectParentAR);
		RECT l_rectThisDlg;	this->GetWindowRect(&l_rectThisDlg);
		if ((g_lModelType == MODEL_T3100) &&
			((l_rectParentAR.bottom - l_rectParentAR.top) < (l_rectThisDlg.bottom - l_rectThisDlg.top))) {
			/* ï¿½Iï¿½tï¿½Zï¿½bï¿½gï¿½vï¿½Z	*/
			long l_lDeltaY = (l_rectThisDlg.bottom - l_rectThisDlg.top) - (l_rectParentAR.bottom - l_rectParentAR.top);
			l_lDeltaY += 1;
			this->ChgSizeCtrl(-l_lDeltaY);
		}
	}
/* added 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- } ---------- */

	return TRUE;	// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Éƒtï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½İ’è‚µï¿½È‚ï¿½ï¿½Æ‚ï¿½ï¿½Aï¿½ß‚ï¿½lï¿½ï¿½ TRUE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
					// ï¿½ï¿½O: OCX ï¿½vï¿½ï¿½ï¿½pï¿½eï¿½B ï¿½yï¿½[ï¿½Wï¿½Ì–ß‚ï¿½lï¿½ï¿½ FALSE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
}

// =========================================================================
//
void CMeasurementGraphDlg::FontCreation()
{
	int sts;

	sts = m_Font.CreateFont(
			11, 						// Height
			0,							// Width
			0,							// Escapement
			90, 						// Orientation
			FW_NORMAL,					// Weight
										//	FW_FW_DONTCARE	0
										//	FW_THIN 	100
										//	FW_EXTRALIGHT	200
										//	FW_ULTRALIGHT	200
										//	FW_LIGHT		300
										//	FW_NORMAL		400
										//	FW_REGULAR		400
										//	FW_MEDIUM		500
										//	FW_SEMIBOLD 600
										//	FW_DEMIBOLD 600
										//	FW_BOLD 	700
										//	FW_EXTRABOLD	800
										//	FW_ULTRABOLD	800
										//	FW_BLACK		900
										//	FW_HEAVY		900
			FALSE,						// Italic
			FALSE,						// Underline
			0,							// StrikeOut
			DEFAULT_CHARSET,			// CharSet
										//	ANSI_CHARSET		0
										//	DEFAULT_CHARSET 1
										//	SYMBOL_CHARSET		2
										//	SHIFTJIS_CHARSET 128
										//	OEM_CHARSET 	255
										//	HANGUL_CHARSET
			OUT_DEFAULT_PRECIS, 		// OutPrecision
										//	OUT_CHARACTER_PRECIS
										//	OUT_STRING_PRECIS
										//	OUT_DEFAULT_PRECIS
										//	OUT_STROKE_PRECIS
										//	OUT_DEVICE_PRECIS
										//	OUT_TT_PRECIS
										//	OUT_RASTER_PRECIS
										//	DEFAULT_QUALITY
			CLIP_DEFAULT_PRECIS,		// ClipPrecision
										//	CLIP_CHARACTER_PRECIS
										//	CLIP_MASK
										//	CLIP_DEFAULT_PRECIS
										//	CLIP_STROKE_PRECIS
										//	CLIP_ENCAPSULATE
										//	CLIP_TT_ALWAYS
										//	CLIP_LH_ANGLES
			DEFAULT_QUALITY,			// Quality
										//	DEFAULT_QUALITY
										//	DRAFT_QUALITY
										//	PROOF_QUALITY
			FIXED_PITCH | FF_SCRIPT,	// PitchAndFamily
										//	(BYTE Low2bit)
										//		DEFAULT_PITCH
										//		FIXED_PITCH
										//		VARIABLE_PITCH
										//	(HIGH4bit)
										//		FF_DECORATIVE
										//		FF_DONTCARE
										//		FF_SCRIPT
										//		FF_MODERN
										//		FF_ROMAN
										//		FF_SWISS
			"MS UI Gothic"				// Facename
										//	"Terminal"
										//	"MS Sans Serif"
										//	"Arial"
										//	"batangche" (Hangul)
										//	"MS ï¿½Sï¿½Vï¿½bï¿½N"
										//	"MS Pï¿½Sï¿½Vï¿½bï¿½N"
										//	"MS UI Gothic"
										//	"Times New Roman"
	);
}

// =========================================================================
//
void CMeasurementGraphDlg::MeasurementListGrid_Init()
{

	int iDispCol = 0; //ï¿½Lï¿½ï¿½Colï¿½ï¿½
	//2009.08.21 bagus stress --{--
	//iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // ï¿½wï¿½bï¿½hï¿½^ï¿½Cï¿½v
	switch(wHeadType){
	case HEAD_TYPE_SR:
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
// 2009.06.09 K.Matsuo -->
	//2010.01.11 bagus GTR --{--
	// ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Í�Aï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½Ú�uTEST_MODE_CON_ITEMï¿½vï¿½ï¿½ï¿½Aï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ğ‘�‚â‚·
	if( (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_NOMAL) || (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) ) {
		if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
			//Lens,Integï¿½Ì‚ï¿½
			iDispCol += 2;
		}else{
			//Lens,Filter,Integ
			iDispCol += 3;
		}
	}
	//2010.01.11 bagus GTR --}--
// 2009.06.09 K.Matsuo <--
		break;
	case HEAD_TYPE_STRESS:
//		iDispCol = m_pDoc->GetDispLabelCount() + 4 + 3 + 1; //4: LS+STRESS, 3:XYZ, 1:AF
//		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1 + 1; //4: LS+STRESS, 3:XYZ, 1:AF
		iDispCol = m_pDoc->GetDispLabelCount() + 4 + 3 + 1; //4: LS+STRESS, 3:XYZ, 1:AF
		break;
	//2009.09.20 bagus se --{--
	case HEAD_TYPE_SE:
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
// 2009.06.09 K.Matsuo -->
		// ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Í�Aï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½Ú�uTEST_MODE_CON_ITEMï¿½vï¿½ï¿½ï¿½Aï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ğ‘�‚â‚·
		if( (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_NOMAL) || (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) ) {
			iDispCol += 3;
		}
// 2009.06.09 K.Matsuo <--
		break;
	//2009.09.20 bagus se --}--
	default:
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
// 2009.06.09 K.Matsuo -->
	// ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Í�Aï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½Ú�uTEST_MODE_CON_ITEMï¿½vï¿½ï¿½ï¿½Aï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ğ‘�‚â‚·
	if( (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_NOMAL) || (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) ) {
		iDispCol += 3;
	}
// 2009.06.09 K.Matsuo <--
		break;
	}
	//2009.08.21 bagus stress --{--


	if(iDispCol < MEASUREMENTGRID_GRAPH_COL_MIN)
	{
		iDispCol = MEASUREMENTGRID_GRAPH_COL_MIN;
	}
//	int Cols = 18 + 1;
//	int Cols = MEASUREMENTGRID_GRAPH_COL_MIN + 1;
	int Cols = iDispCol + 1;
	int Rows = DEF_MEASUREMENT_GRID_ROWLINE_COUNT; //50+1;//2000 + 1;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	int ItemWidth = 68;

	///// Grid Initialize /////
	m_MeasurementListGrid.SetEditable(FALSE);
	m_MeasurementListGrid.EnableDragAndDrop(TRUE);
	m_MeasurementListGrid.SetTextBkColor(WHITE_COLOR);
	m_MeasurementListGrid.SetRowResize(0);
// 2013.11.13 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	m_MeasurementListGrid.SetColumnResize(0);
// 2013.11.13 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	m_MeasurementListGrid.EnableSelection(TRUE);
	m_MeasurementListGrid.SetTrackFocusCell(FALSE);
	m_MeasurementListGrid.SetFrameFocusCell(FALSE);
	m_MeasurementListGrid.SetFixedColumnSelection(FALSE);
	m_MeasurementListGrid.SetFixedRowSelection(TRUE);
	m_MeasurementListGrid.SetListMode(TRUE);

	TRY{
		m_MeasurementListGrid.SetRowCount(Rows);
		m_MeasurementListGrid.SetColumnCount(Cols);
		m_MeasurementListGrid.SetFixedRowCount(FixRows);
		m_MeasurementListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	// Row 0 Height
	m_MeasurementListGrid.SetRowHeight(0, 36);

	// Row 1 -> RowCount Height
	for(row = 1 ; row < m_MeasurementListGrid.GetRowCount() ; row++){
		m_MeasurementListGrid.SetRowHeight(row, 18);
	}

	// Col 0 Width
	m_MeasurementListGrid.SetColumnWidth(0, 40);	// No.


	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_MeasurementListGrid.GetColumnCount(); col++)
	{
		m_MeasurementListGrid.SetColumnWidth(col, ItemWidth);
		if(strstr(m_MeasurementListGrid.GetItemText(0, col), TEST_MODE_CON_ITEM[2]) != NULL) //"Integration Time[sec]"
		{
			m_MeasurementListGrid.SetColumnWidth(col, 75);
		}
	}
// 2013.11.13 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		if(m_SrConfig.bAutoFocus){
			for (col = 1 ; col < 5; col++)	// XYZ, AF
			{
				m_MeasurementListGrid.SetColumnWidth(col, 0);
			}
		}
		else{
			for (col = 1 ; col < 4; col++)	// XYZ
			{
				m_MeasurementListGrid.SetColumnWidth(col, 0);
			}
		}
	}
// 2013.11.13 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	///// Font Initialize /////
	DWORD dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	LOGFONT LogFont;
	m_Font.GetLogFont(&LogFont);


	for(row = 0; row < m_MeasurementListGrid.GetRowCount(); row++)
	{
		if(row == 0)
		{
			LogFont.lfHeight = 12;
		}
		else
		{
			LogFont.lfHeight = 11;
		}
		for(col = 0; col < m_MeasurementListGrid.GetColumnCount(); col++)
		{
			m_MeasurementListGrid.SetItemFont(row, col, &LogFont);
			m_MeasurementListGrid.SetItemFormat(row, col, dwTextStyle);
		}
	}
}

// =========================================================================
//
void CMeasurementGraphDlg::MeasurementListGrid_HeaderSet()
{
	DWORD dwTextStyle;
	char szXyz[255+1];
	memset(szXyz, 0, sizeof(szXyz));
	WORD& wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType;
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;

	dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;


//2009.08.25 bagus stress --{--
#if 0

	///// Title Row /////
	dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	for ( int iRow = 1 ; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ ) {
		m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);
		m_MeasurementListGrid.SetItemFormat(iRow, 0, dwTextStyle);
	}

	///// Title Column /////
	//dwTextStyle = DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_END_ELLIPSIS;
	dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

	for( int iCol = 0 ; iCol < MEAS_LIST_POSITION_MAX; iCol++) {						// ï¿½Xï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½|ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½
		if(iCol == 0)
		{
			sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
		}
		else
		{
		sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
		}
		m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);

		m_MeasurementListGrid.SetItemFormat(0, iCol, dwTextStyle);
	}


	if ( wHeadType == HEAD_TYPE_SR ) {
		//// ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½è—“ ////
// 2009.05.15 k-matsuo -->
//		ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
// 2009.05.15 k-matsuo <--
		if(m_SrConfig.bAutoFocus){
			m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_AF_CHECK_ITEM[0]);
			m_MeasurementListGrid.SetItemFormat(0, iCol, dwTextStyle);
			iCol++;
		}

		int i;
		char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
		m_pDoc->GetDispLabel(szDispLabel);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel[i]);
			m_MeasurementListGrid.SetItemFormat(0, iCol, dwTextStyle);
			iCol++;
		}

		if( (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_NOMAL) || (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) )
		{
			for(i = 0 ; i < 3; i++)
			{
				m_MeasurementListGrid.SetItemText(0, iCol, TEST_MODE_CON_ITEM[i]);
				m_MeasurementListGrid.SetItemFormat(0, iCol, dwTextStyle);
				if(i==2)
					m_MeasurementListGrid.SetColumnWidth(iCol, 75);
				iCol++;
			}
		}
	}
#else
	int iCol = 0;
	int iRow = 0;
	int i;
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];


	switch( wHeadType ){
	case HEAD_TYPE_SR:
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		///// Title Column /////
		for  (iCol = 0; iCol < MEAS_LIST_POSITION_MAX; iCol++ ) 			   // ï¿½Xï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½|ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½
		{
			if(iCol == 0)
			{
				sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
			}
			else
			{
				sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
			}
			m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
	//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
		}
		//// ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½è—“ ////
		ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
		if(m_SrConfig.bAutoFocus){
			m_MeasurementListGrid.SetItemText(0, iCol++, MEAS_LIST_AF_CHECK_ITEM[0]);
		}

		if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
		&& ( (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_NOMAL) || (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) )){
			//ï¿½Kï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ß—ï¿½ï¿½Ìƒeï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½É‘ï¿½ï¿½è�€ï¿½Ú‚Í‚È‚ï¿½
		}else{
			m_pDoc->GetDispLabel(szDispLabel);
			for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
			{
				m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel[i]);
				iCol++;
			}
		}
		if( (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_NOMAL) || (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) )
		{
			//2010.01.11 bagus GTR --{--
			if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
				for(i = 0 ; i < 2; i++)
				{
					m_MeasurementListGrid.SetItemText(0, iCol, TEST_MODE_SR_GANTRY_CON_ITEM[i]);
					m_MeasurementListGrid.SetItemFormat(0, iCol, dwTextStyle);
					if(i==1)
						m_MeasurementListGrid.SetColumnWidth(iCol, 75);
					iCol++;
				}
			}else{
				for(i = 0 ; i < 3; i++)
				{
					m_MeasurementListGrid.SetItemText(0, iCol, TEST_MODE_CON_ITEM[i]);
					m_MeasurementListGrid.SetItemFormat(0, iCol, dwTextStyle);
					if(i==2)
						m_MeasurementListGrid.SetColumnWidth(iCol, 75);
					iCol++;
				}
			}
			//2010.01.11 bagus GTR --}--
		}
		break;
	//2009.09.15 bagus se --{--
	case HEAD_TYPE_SE:
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		///// Title Column /////
		for  (iCol = 0; iCol < MEAS_LIST_POSITION_MAX; iCol++ ) 			   // ï¿½Xï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½|ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½
		{
			if(iCol == 0)
			{
				sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
			}
			else
			{
				sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
			}
			m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
	//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
		}
		//// ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½è—“ ////
		ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
		if(m_SrConfig.bAutoFocus){
			m_MeasurementListGrid.SetItemText(0, iCol++, MEAS_LIST_AF_CHECK_ITEM[0]);
		}


		m_pDoc->GetDispLabel(szDispLabel);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel[i]);
			iCol++;
		}
		if( (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_NOMAL) || (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) )
		{
			for(i = 0 ; i < 3; i++)
			{
				m_MeasurementListGrid.SetItemText(0, iCol, TEST_MODE_CON_ITEM[i]);
				m_MeasurementListGrid.SetItemFormat(0, iCol, dwTextStyle);
				if(i==2)
					m_MeasurementListGrid.SetColumnWidth(iCol, 75);
				iCol++;
			}
		}
		break;
	//2009.09.15 bagus se --}--

	case HEAD_TYPE_STRESS:
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		///// Title Column /////
		for  (iCol = 0; iCol < MEAS_LIST_STRESS_POSITION_MAX; iCol++ )				  // ï¿½Xï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½|ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½
		{
			if(iCol == 0)
			{
				sprintf(szXyz, "%s", MEAS_LIST_STRESS_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
			}
			else if(iCol > 4)
			{
				sprintf(szXyz, "%s%s", MEAS_LIST_STRESS_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
			}
			else {
				sprintf(szXyz, "%s", MEAS_LIST_STRESS_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]ï¿½ï¿½mm
			}
			m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
	//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
		}
		//// ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½è—“ ////
		m_MeasurementListGrid.SetItemText(0, iCol++, MEAS_LIST_AF_CHECK_ITEM[0]);


		char szDispLabel2[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
		m_pDoc->GetDispLabel(szDispLabel2);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel2[i]);
			iCol++;
		}
		break;
// 2009.10.19 bagus MS ï¿½Ç‰ï¿½ --{--
//	case HEAD_TYPE_IRSE:
	case HEAD_TYPE_MS:
// 2009.10.19 bagus MS ï¿½Ç‰ï¿½ --}--
	case HEAD_TYPE_4PP:
	case HEAD_TYPE_CTA:
	default:
		break;
	}

#endif
//2009.08.25 bagus stress --}--
}

// =========================================================================
//
void CMeasurementGraphDlg::AddMeasurementListGrid_Data(int iRow, int iCol, LPCTSTR pszValue)
{
	DWORD dwTextStyle;

	//2009.09.10 bagus stress --{--
	if(iCol >= m_MeasurementListGrid.GetColumnCount()){
		return;
	}
	//2009.09.10 bagus stress --}--
	if (iRow==0)
		dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	else{
	 //   dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
		dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	}

	if(iRow>m_MeasurementListGrid.GetRowCount()-1){
//2009.08.25 bagus stress --{--
//2ï¿½È�ï¿½ï¿½Ñ‰zï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½ï¿½ï¿½lï¿½ï¿½
		while(m_MeasurementListGrid.GetRowCount() <= iRow){
			DWORD dwTextStyleColIndex = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
			CString strBuffer;
			strBuffer.Format("%d", m_MeasurementListGrid.GetRowCount());
			m_MeasurementListGrid.InsertRow(strBuffer);
			//m_MeasurementListGrid.SetItemFormat(iRow, 0, dwTextStyleColIndex);
			m_MeasurementListGrid.SetRowHeight (m_MeasurementListGrid.GetRowCount()-1,18);
		}
//2009.08.25 bagus stress --}--
	}
	m_MeasurementListGrid.SetItemText(iRow, iCol, pszValue);
	m_MeasurementListGrid.SetItemFormat(iRow, iCol, dwTextStyle);
/* added 2014.10.27 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- { ---------- */
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- { ---------- */
//	if ((0 != m_pMainFrame->m_pTestMeasDlgObj) && (MODEL_T3100 == g_lModelType) && (1 == iCol)) {
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ----------              */
	if (
		(
			(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) ||
			(m_pDoc->GetMeasMode() == MEASMODE_TEST)
			) &&
		(1 == iCol)
		) {
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- } ---------- */
		this->SetFocus();
		m_MeasurementListGrid.EnsureVisible(iRow, TRUE);
	}
/* added 2014.10.27 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- } ---------- */

	for(int i = 1;i < m_MeasurementListGrid.GetRowCount();i++){
		m_MeasurementListGrid.SetItemFormat(i, 0, dwTextStyle);
	}
	//ï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Tï¿½Cï¿½Yï¿½Ï�X
	LOGFONT LogFont;
	m_Font.GetLogFont(&LogFont);
	for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ ) {
			LogFont.lfHeight = 11;
		for ( iCol = 0; iCol < m_MeasurementListGrid.GetColumnCount(); iCol++ ) {
			m_MeasurementListGrid.SetItemFont(iRow, iCol, &LogFont);
		}
	}



	m_MeasurementListGrid.Invalidate();
}

// =========================================================================
//
void CMeasurementGraphDlg::LightIntensityListGrid_Init()
{
	int Rows = DEF_LITENSITY_GRID_ROWLINE_COUNT;//Gridï¿½É‚ï¿½ï¿½å‚¤ï¿½ÇƒXï¿½Nï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½oï¿½[ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	int ItemWidth = 68;

	m_strHeader.GetHeader(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType, m_iGraphDlgMode, GetCurrentTab(),m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType);

	///// Grid Initialize /////
	m_LightIntensityListGrid.SetEditable(FALSE);
	m_LightIntensityListGrid.EnableDragAndDrop(TRUE);
	m_LightIntensityListGrid.SetTextBkColor(WHITE_COLOR);
	m_LightIntensityListGrid.SetRowResize(0);
// 2013.11.13 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	m_LightIntensityListGrid.SetColumnResize(0);
// 2013.11.13 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	m_LightIntensityListGrid.EnableSelection(TRUE);
	m_LightIntensityListGrid.SetTrackFocusCell(FALSE);
	m_LightIntensityListGrid.SetFrameFocusCell(FALSE);
	m_LightIntensityListGrid.SetFixedColumnSelection(FALSE);
	m_LightIntensityListGrid.SetFixedRowSelection(TRUE);
	m_LightIntensityListGrid.SetListMode(TRUE);
	TRY{
		m_LightIntensityListGrid.SetRowCount(Rows);
		m_LightIntensityListGrid.SetColumnCount(m_strHeader.GetSize());
		m_LightIntensityListGrid.SetFixedRowCount(FixRows);
		m_LightIntensityListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH


	// Row 0 Height
	m_LightIntensityListGrid.SetRowHeight(0, 36);//Header Height

	// Row 1 - RowCount Height
	for(row = 1; row < m_LightIntensityListGrid.GetRowCount(); row++)
		m_LightIntensityListGrid.SetRowHeight(row, 18);

// 2010.01.21 bagus Gantry --{--
#if 0
	// Col 0 Width
	m_LightIntensityListGrid.SetColumnWidth(0, 40); // Wavelength Width

	// Col 1 - ColumnCount Width
	for (col = 1; col < m_LightIntensityListGrid.GetColumnCount(); col++)
		m_LightIntensityListGrid.SetColumnWidth(col, ItemWidth);
#else
	int nColNum = m_LightIntensityListGrid.GetColumnCount();

	if ((m_iGraphDlgMode != GRAPHDLG_MODE_TEST_RAW)
	 && (m_rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR)
	 && ((m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE)
	  || (m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))) {
		// Col 0 Width
		m_LightIntensityListGrid.SetColumnWidth(0, 90); // Wavelength Width
		m_LightIntensityListGrid.SetColumnWidth(1, 91); // Wavelength Width
	} else {
		// Col 0 Width
		m_LightIntensityListGrid.SetColumnWidth(0, 40); // Wavelength Width

		// Col 1 - ColumnCount Width
		for (col = 1; col < m_LightIntensityListGrid.GetColumnCount(); col++)
			m_LightIntensityListGrid.SetColumnWidth(col, ItemWidth);
	}
#endif
// 2010.01.21 bagus Gantry --}--
}

// =========================================================================
//
BOOL CMeasurementGraphDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉŒÅ—Lï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚©ï¿½Aï¿½Ü‚ï¿½ï¿½ÍŠï¿½{ï¿½Nï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	switch(pMsg->message){
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
	case WM_RBUTTONDBLCLK:
		if (pMsg->hwnd == m_MeasurementListGrid.m_hWnd) {
			POINT l_ptlParam;
			l_ptlParam.x = LOWORD(pMsg->lParam);
			l_ptlParam.y = HIWORD(pMsg->lParam);
			if (FALSE != m_pMainFrame->m_pMeaTabViewObj->MRCS_RDblClk(l_ptlParam, &m_MeasurementListGrid)) {
				return TRUE;
			}
		}
		break;
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */
	case WM_KEYDOWN:
/* modified 2015.04.12 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½Ê‰ï¿½ÊƒNï¿½ï¿½ï¿½Aï¿½Î�ï¿½ ---------- { ---------- */
//		if (pMsg->wParam == VK_ESCAPE)
//			return TRUE;
/* modified 2015.04.12 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½Ê‰ï¿½ÊƒNï¿½ï¿½ï¿½Aï¿½Î�ï¿½ ----------              */
		switch (pMsg->wParam) {
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
			break;
		}
/* modified 2015.04.12 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½Ê‰ï¿½ÊƒNï¿½ï¿½ï¿½Aï¿½Î�ï¿½ ---------- } ---------- */
		break;
	case WM_SYSKEYDOWN:
		if (pMsg->wParam == VK_F4)
			return TRUE;
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// =========================================================================
//
void CMeasurementGraphDlg::LoadData()
{
	m_pDoc->GetRcpData(&m_rcp_data);
}

// =========================================================================
//
void CMeasurementGraphDlg::SetData()
{
	CString strBuffer;
	int iHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType;

	int iMeasureNum=0;			 //ï¿½ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½
	iMeasureNum = m_pDoc->GetMeasurePointCount();


	if( m_iGraphDlgMode != GRAPHDLG_MODE_TEST_NOMAL &&
		m_iGraphDlgMode != GRAPHDLG_MODE_TEST_RAW ){
		///// Set Recipe Name /////
		SetDlgItemText(IDC_MAIN_RECIPE_NAME, m_rcp_data.szRecipeName);

		///// Set Recipe Head /////
		SetDlgItemText(IDC_HEAD_TYPE, HEAD_TYPE_ITEM[iHeadType]);
	}

	///// Set Scan Type /////
	CMLScanType mlScanTypeItem(iHeadType);
	SetDlgItemText(IDC_MEASUREMENT_ITEM, mlScanTypeItem[m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType]);

	///// Set Stage Point /////
	//2010.01.01.15 bagus 2point-Distance --{--
	if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
		iMeasureNum /= 2;
	}
	//2010.01.01.15 bagus 2point-Distance --}--
	strBuffer.Format("%5d / %5d ", 0, iMeasureNum);
	SetDlgItemText(IDC_MEASUREMENT_POINT, strBuffer);

	//2009.09.10 bagus stress --{--
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // ï¿½wï¿½bï¿½hï¿½^ï¿½Cï¿½v
	if(wHeadType == HEAD_TYPE_STRESS){
		int nTotalNum = 0;
		STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;
		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			if(stressStage.Line[nLine].bValidLine){
				for(int nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
					if(stressStage.Line[nLine].bScanValid[nSection]){
						nTotalNum++;
					}
				}
			}
		}
		strBuffer.Format("%5d / %5d ", 0, nTotalNum);
		SetDlgItemText(IDC_MEASUREMENT_LS, strBuffer);
	}
	//2009.09.10 bagus stress --}--

}

// =========================================================================
//
void CMeasurementGraphDlg::OnPaint()
{
	CPaintDC dc(this); // ï¿½`ï¿½ï¿½pï¿½Ìƒfï¿½oï¿½Cï¿½X ï¿½Rï¿½ï¿½ï¿½eï¿½Lï¿½Xï¿½g

	// ï¿½`ï¿½ï¿½pï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½Æ‚ï¿½ï¿½ï¿½ CDialog::OnPaint() ï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚Í‚ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½
}

// =========================================================================
// OnSelChangeing MeasurementListGridï¿½Iï¿½ï¿½ï¿½sï¿½Ì‘Iï¿½ï¿½Ï�Xï¿½ï¿½ï¿½É•\ï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½
void CMeasurementGraphDlg::OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID CellID;
	CellID.row=pItem->iRow;
	CellID.col=pItem->iColumn;
	if ( !m_MeasurementListGrid.IsValid(CellID) )
		return;

	// ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Ö‚ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½ï¿½Ì‚ÅˆÈ�~ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_nGraphRow == pItem->iRow
	&& m_nGraphTab == GetCurrentTab()){
		return;
	}
	m_nGraphRow = pItem->iRow;
	m_nGraphTab = GetCurrentTab();

	switch(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType){
	case HEAD_TYPE_SR:
	case HEAD_TYPE_SE:
	default:
		break;
	case HEAD_TYPE_STRESS:
		if(CellID.row <= m_pDoc->GetStoreCount()){
			//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì•ï¿½ï¿½ï¿½ï¿½Å‚ï¿½Thickï¿½Ì‚İ‚ï¿½ï¿½\ï¿½ï¿½ï¿½Å‚ï¿½ï¿½ï¿½
			m_nDispLine = -1;
		}else{
			//LSï¿½Ì•ï¿½ï¿½ï¿½ï¿½Å‚ï¿½Thickï¿½Ìƒ^ï¿½uï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½Å‚ï¿½ï¿½È‚ï¿½
			if(m_MeasureGraphTab.GetCurSel() == 0){
				m_iDispPointNo = -1;
			}
		}
		break;
	}

	//ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Xï¿½gï¿½Ô�ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½ê�‡ï¿½Aï¿½Oï¿½ï¿½ï¿½tï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Ä”ï¿½ï¿½ï¿½ï¿½ï¿½
	if (m_MeasurementListGrid.GetItemText(CellID.row, 1)==""){
		m_MeasureGraph.ClearGraphData();
		m_iDispPointNo = -1;
		m_nDispLine = -1;
		m_LightIntensityListGrid.Invalidate();
		m_MeasureGraph.Invalidate();
		return;
	}


	if( (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_NOMAL)
		||(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) )
	{ //ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½h
		TEST_MODE_DATA TestModeData;
		memset(&TestModeData, 0, sizeof(TestModeData));
		m_pDoc->GetTestModeData(&TestModeData, CellID.row);

		if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
		{
			m_iStartWaveLength = TestModeData.MeasRcpCon.wStart;
			m_iEndWaveLength = TestModeData.MeasRcpCon.wEnd;
		}
// 2009.05.20 K.Matsuo -->
//		MeasurementListGrid_Init();
		LightIntensityListGrid_Init();
// 2009.05.20 K.Matsuo <--
		DrawLightIntensityGraph_Init();
	}


//2009.8.25 bagus swada -->
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // ï¿½wï¿½bï¿½hï¿½^ï¿½Cï¿½v
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;
	int iMeasureNum = m_pDoc->GetStoreCount();//m_pDoc->GetMeasurePointCount();
	switch(wHeadType){
	case HEAD_TYPE_SR:
	default:
		AddLightIntensityListAndGraph(CellID.row);
		break;
	//2009.09.07 bagus se --{--
	case HEAD_TYPE_SE:
		AddPsiAndDeltaListAndGraph(CellID.row);
		break;
	//2009.09.07 bagus se --}--
	case HEAD_TYPE_STRESS:
		LightIntensityListGrid_Init();
		DrawLightIntensityGraph_Init();
		//ï¿½fï¿½[ï¿½^ï¿½ÍƒNï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
		m_MeasureGraph.ClearGraphData();

		if(CellID.row <= iMeasureNum){
			if(GetCurrentTab() == 0){
				AddLightIntensityListAndGraph(CellID.row);
			}else{
				//2009.09.19 bagus stress --{--
//				m_LightIntensityListGrid.SetRowCount(1);
				SetBlankLightIntensityListGrid();
				//2009.09.19 bagus stress --}--
			}
		}else{
			//ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½Ó�ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½ï¿½ï¿½ï¿½LSï¿½ï¿½Tï¿½ï¿½ï¿½ÄŒï¿½ï¿½Â‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			if(GetCurrentTab() != 0){
				int nLS = 0;
				for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
					if(!m_rcp_data.StressStageProgInfo.Line[nLine].bValidLine){
						continue;
					}
					for(int nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
						if(m_rcp_data.StressStageProgInfo.Line[nLine].bScanValid[nSection]){
							nLS++;
							if(nLS == CellID.row - iMeasureNum){
								//ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ê‚½LSï¿½pï¿½ÉƒOï¿½ï¿½ï¿½tï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
								SetBlankLightIntensityListGrid();
								DrawLightIntensityGraph_Init_Stress(nLine,nSection);
								AddStressDataListAndGraph(nLine,nSection,GetCurrentTab());
								break;
							}
						}
					}
				}
			}else{
				//2009.09.19 bagus stress --{--
//				m_LightIntensityListGrid.SetRowCount(1);
				SetBlankLightIntensityListGrid();
				//2009.09.19 bagus stress --}--
			}
		}
		break;
	}
//2009.8.25 bagus swada <--

}

// =========================================================================
//

//LRESULT CMeasurementGraphDlg::SetMeasurementData(WPARAM wparam, LPARAM lparam)
//{
//MessageBox("ï¿½ï¿½ï¿½ï¿½ï¿½É‚Í—ï¿½ï¿½È‚ï¿½ï¿½Í‚ï¿½ hibino", "WARNING", MB_OK);
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½SetMeasurementData2ï¿½ÖˆÚ“ï¿½ hibino
//
//	return 0L;
//}


// =========================================================================
//
//[ï¿½ß‚ï¿½l]
//-1:ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s
// 0:ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
// 1:ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½)
// 2:ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½Å�Vï¿½}ï¿½ï¿½ï¿½Â�ï¿½ï¿½Ö‚ÌˆÚ“ï¿½ï¿½È‚ï¿½))
int CMeasurementGraphDlg::SetMeasurementData2()
{

	char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1]; 					  //ï¿½Oï¿½ï¿½ï¿½bï¿½hï¿½Ìƒ^ï¿½Cï¿½gï¿½ï¿½(X,Y,Z)
	char szTitleAf[AF_HEADER_LEN + 1];							 //ï¿½Oï¿½ï¿½ï¿½bï¿½hï¿½Ìƒ^ï¿½Cï¿½gï¿½ï¿½(AF)
	char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];	  //ï¿½Oï¿½ï¿½ï¿½bï¿½hï¿½Ìƒ^ï¿½Cï¿½gï¿½ï¿½(ï¿½æ“ªï¿½ï¿½Thick1?) ï¿½ï¿½X,Y,Z,AFï¿½ÍŠÜ‚Ü‚ï¿½
	int iLabelCount=0;								   //ï¿½Oï¿½ï¿½ï¿½bï¿½hï¿½Ìƒ^ï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½(ï¿½æ“ªï¿½ï¿½Thick1?) ï¿½ï¿½X,Y,Z,AFï¿½ÍŠÜ‚Ü‚ï¿½
	int iCurrentSetPoint = 0; //ï¿½ï¿½Ô�ÅŒï¿½Éƒfï¿½[ï¿½^ï¿½ï¿½ï¿½iï¿½[(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)ï¿½ï¿½ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ô�ï¿½
	int iDispLabelCount=0;
	ONE_POINT_DATA OnePointData; //1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Ì‘ï¿½ï¿½ï¿½fï¿½[ï¿½^
	STATISTICS Statistics[ADAPRESULT_COLS_MAX]; 					  //ï¿½ï¿½ï¿½vï¿½fï¿½[ï¿½^
	int iPoint/* = wparam*/;	// ï¿½ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Æ‚Ì‰ï¿½
	int iMeasureNum=0;			 //ï¿½ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½
	int iCount =0; //ï¿½ï¿½ï¿½[ï¿½vï¿½ï¿½ï¿½Ì“Yï¿½ï¿½ï¿½ï¿½(ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½g)
	int iShowCompletionPointGraph; //ï¿½ï¿½Ê‚É•\ï¿½ï¿½ï¿½Ï‚İ‚Ìƒ|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];

/* added 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
	int l_iUnitIndexThickInt = -1;		/* -1 ï¿½Í�ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½	*/
	SR_FIGURE_FORMAT l_SrFigureFormat;
	ConfigFile_GetNanoSpecIni(&l_SrFigureFormat,
					CONFIG_FILE_SR_FIGURE_FORMAT);
	if (0 != l_SrFigureFormat.iThicknessIntOnly) {
		l_iUnitIndexThickInt = 3;		/* -1 ï¿½ÈŠOï¿½ÍƒCï¿½ï¿½ï¿½fï¿½bï¿½Nï¿½Xï¿½l	*/
	}
/* added 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */

	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // ï¿½wï¿½bï¿½hï¿½^ï¿½Cï¿½v
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;

	// 2009.12.26 K.Matsuo Resistance -->
	if ( wHeadType == HEAD_TYPE_4PP )
		return 2;
	// 2009.12.26 K.Matsuo Resistance <--

	memset(szTitleXyz, 0, sizeof(szTitleXyz));
	memset(szTitleAf, 0, sizeof(szTitleAf));
	memset(szLabel, 0, sizeof(szLabel));
	memset(&OnePointData, 0, sizeof(OnePointData));
	memset(Statistics, 0, sizeof(Statistics));
	memset(szDispLabel, 0, sizeof(szDispLabel));

	int i;
	CString strBuffer;

	m_pDoc->GetDispLabel(szDispLabel);
	iDispLabelCount = m_pDoc->GetDispLabelCount();
	iMeasureNum = m_pDoc->GetMeasurePointCount();

	iShowCompletionPointGraph = m_pDoc->GetShowCompPointGraph();


	m_pDoc->GetLabel(szTitleXyz, szTitleAf, szLabel,&iLabelCount);
	iMeasureNum = m_pDoc->GetMeasurePointCount();
	iPoint = iCurrentSetPoint = m_pDoc->GetCurrentSetPoint();
//m_pDoc->SetMeasurePointCount(iMeasureNum); //ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ì‘ï¿½ï¿½è��ï¿½ÉƒCï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½é‚±ï¿½Æ�I


//ï¿½Iï¿½ï¿½ï¿½sï¿½Ì—Lï¿½ï¿½ï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½ï¿½
	for(i=0; i < iShowCompletionPointGraph; i++)
	{
		int iFocus = (m_MeasurementListGrid.GetItemState(i+1, 1) & GVIS_SELECTED);
		if((iFocus != 0)&&(m_bCellSelectedFlg == FALSE)) //ï¿½sï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½ï¿½ï¿½ÆˆÈ�~ï¿½Í�Å�Vï¿½sï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½)
		{
			m_bCellSelectedFlg = TRUE;
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- { ---------- */
//			m_MeasurementListGrid.EnsureVisible((i+1), TRUE);
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ----------              */
			if (
				(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) &&
				(m_pDoc->GetMeasMode() != MEASMODE_TEST)
				) {
				m_MeasurementListGrid.EnsureVisible((i + 1), TRUE);
			}
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- } ---------- */
			break;
		}
	}


	iCount = 0;
	if(iCurrentSetPoint == 0)
	{
		return 1; //
	}
	if(iShowCompletionPointGraph < iCurrentSetPoint)
	{
		iPoint = iCurrentSetPoint;	//ï¿½wï¿½ï¿½ï¿½[ï¿½vï¿½ï¿½ï¿½Ì�Iï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½xï¿½ï¿½ï¿½wï¿½iï¿½[ï¿½Ï‚İƒ|ï¿½Cï¿½ï¿½ï¿½gï¿½xï¿½lï¿½É�İ’ï¿½B
		iCount = /*iSetPointCount -*/ iShowCompletionPointGraph;	//ï¿½ï¿½ï¿½[ï¿½vï¿½Jï¿½nï¿½_ï¿½ï¿½İ’ï¿½B
	}
	else
	{
		//ï¿½ï¿½ï¿½İ’Ç‰ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½È‚ï¿½
		return 1;	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½)
	}

/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
	WORD l_wIncludeColorSetNo = 0;
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */

	while ( ++iCount <= iPoint ) { //ï¿½ï¿½Ê‚ï¿½Ø‚ï¿½Ö‚ï¿½ï¿½ï¿½ï¿½Æ‚ï¿½ï¿½É�Aï¿½ï¿½ï¿½İ�iï¿½sï¿½ï¿½ï¿½Ì‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ü‚Å‚Ìƒfï¿½[ï¿½^ï¿½ï¿½ï¿½Ä•`ï¿½æ‚·ï¿½é‚½ï¿½ß‚ï¿½whileï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½é�”
		CCellID CellID;
		CellID.row = iCount;

		if (m_MeasurementListGrid.IsValid(CellID) )
			break;

		int iIndex=0;

		m_pDoc->GetOnePointData(&OnePointData, iCount);

		if(OnePointData.Valid == FALSE)
		{
			break;
		}


		char szXyzFormat[255+1];
		memset(szXyzFormat, 0, sizeof(szXyzFormat));
		sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
		///// Point Position X //////
		iIndex++;
		double dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
		strBuffer.Format(szXyzFormat, dPositionX);
		AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

		///// Point Position Y //////
		double dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
		strBuffer.Format(szXyzFormat, dPositionY);
		AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

		/// ///// Point Position Z //////
		double dPositionZ = (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0]) / 10;
		strBuffer.Format(szXyzFormat, dPositionZ);
		AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

		char szDecimalPointBuff[255+1];
		int iCol;

		///// ScanTypeï¿½ï¿½Paramaeter /////
		int iHeadType /*=iHeadType*/ = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType;
		//2009.08.16 bagus stress --{--
		//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Åƒwï¿½bï¿½hï¿½^ï¿½Cï¿½vï¿½ï¿½ï¿½Æ‚É�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		//if(iHeadType == HEAD_TYPE_SR){
		switch(iHeadType){
		case HEAD_TYPE_SR:
// 2009.11.12 K.Matsuo SR,SEï¿½ñ��‚Í�Aï¿½ï¿½ï¿½ï¿½ï¿½@ï¿½\ï¿½É“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ -->
		case HEAD_TYPE_SE:
// 2009.11.12 K.Matsuo SR,SEï¿½ñ��‚Í�Aï¿½ï¿½ï¿½ï¿½ï¿½@ï¿½\ï¿½É“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ <--
			//// ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½è—“ ////
			ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
			if(m_SrConfig.bAutoFocus){
				strBuffer.Format("%s", OnePointData.szShowAfValid);
				AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);
			}

			iCol = 0;

			for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+3]:XYZ, [+1]:AF
			{
				//MeasurementData
				m_pDoc->GetOnePointAndStatistics(&OnePointData, Statistics, iCount); //(1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ú�ï¿½1)

				int iUnitIndex = 0;
				int iUnit = m_pDoc->GetUnitIndex();
				if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
				else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
				if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
					iUnitIndex = 0;
				}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

				char szDecimalPointBuff[255+1];
				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
				{
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ----------              */
					if (l_iUnitIndexThickInt < 0) {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "f");
					}
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
				}
				else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,Kï¿½lï¿½n
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]ï¿½Å’ï¿½
				}
// 2009.11.12 K.Matsuo ADAPï¿½Ì•ñ���ï¿½ï¿½Ú‚Ì�ï¿½ï¿½ï¿½Î‰ï¿½ï¿½B2ï¿½Â–Ú‚ï¿½N1[XXX],K1[XXX]ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½Æ‚ï¿½Aï¿½Ê�ï¿½ï¿½Ú‚Ì’lï¿½ï¿½ï¿½ -->
				else if(strncmp(szDispLabel[i], "Const", strlen("Const")) == 0 ||
						strncmp(szDispLabel[i], "Eg", strlen("Eg")) == 0 ||
						strncmp(szDispLabel[i], "Fraction", strlen("Fraction")) == 0)
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]ï¿½Å’ï¿½
				}
// 2009.11.12 K.Matsuo ADAPï¿½Ì•ñ���ï¿½ï¿½Ú‚Ì�ï¿½ï¿½ï¿½Î‰ï¿½ï¿½B2ï¿½Â–Ú‚ï¿½N1[XXX],K1[XXX]ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½Æ‚ï¿½Aï¿½Ê�ï¿½ï¿½Ú‚Ì’lï¿½ï¿½ï¿½ <--
				else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%ï¿½n, Fit
				{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]ï¿½Å’ï¿½
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
					if ((0 != strstr(szDispLabel[i], "Fit")) &&
						(1 == g_lXmpType)) {
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- { ---------- */
////						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f"); //[.##]ï¿½Å’ï¿½
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ----------              */
//						strcpy(szDecimalPointBuff, "%.3e");
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
#if FIT_DISP_SCOUT == 3
						strcpy(szDecimalPointBuff, "");
#elif FIT_DISP_SCOUT == 2
						strcpy(szDecimalPointBuff, "%.2e");
#elif FIT_DISP_SCOUT == 1
						strcpy(szDecimalPointBuff, "%.3e");
#else
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f");
#endif
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]ï¿½Å’ï¿½
					}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
				}
				else //ï¿½ï¿½ï¿½Ì‘ï¿½
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
				}

				if(strstr(szDispLabel[i], "Thick") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Thick*
				{

					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * g_dUnitConvert[iUnitIndex]);
				}
				else if(strstr(szDispLabel[i], "%") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½%ï¿½n
				{
					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * 100);
				}
				else //
				{
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
//					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
					if (0 == strlen(szDecimalPointBuff)) {
						long l_lExp = m_pDoc->GetExpDouble(OnePointData.dData[i]);
						strBuffer.Format("%.2fe%+03d", OnePointData.dData[i] / pow(10, l_lExp), l_lExp);
					} else {
						strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
					}
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
				}

				AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
				iCol++;
			}

			//LightIntensityListGrid and Graph
			if (iCount == iPoint){
				if ( iHeadType == HEAD_TYPE_SR ) {
					AddLightIntensityListAndGraph(iCount);
				}
				else if ( iHeadType == HEAD_TYPE_SE ) {
					AddPsiAndDeltaListAndGraph(iCount);
				} else {
					// ï¿½ï¿½ï¿½ï¿½ï¿½É‚Í�Aï¿½ï¿½ï¿½È‚ï¿½
					;
				}
				m_nGraphRow = iCount;
			}
			break;
		case HEAD_TYPE_STRESS:
		//2009.08.25 bagus stress --{--
			//Stress ï¿½wï¿½bï¿½hï¿½Ìƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			memset(szXyzFormat, 0, sizeof(szXyzFormat));
			sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
			///// X-Start/X-End/Stress
			iIndex = 0;
			AddMeasurementListGrid_Data(iCount, iIndex++ +1, _T("--"));
			AddMeasurementListGrid_Data(iCount, iIndex++ +1, _T("--"));
			AddMeasurementListGrid_Data(iCount, iIndex++ +1, _T("--"));
			AddMeasurementListGrid_Data(iCount, iIndex++ +1, _T("--"));
			///// Point Position X //////
			iIndex++;
			dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionX);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			///// Point Position Y //////
			dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionY);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			/// ///// Point Position Z //////
			dPositionZ = (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0]) / 10;
			strBuffer.Format(szXyzFormat, dPositionZ);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			//// ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½è—“ ////
			strBuffer.Format("%s", OnePointData.szShowAfValid);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			iCol = 0;
			for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + 4 + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+4]LS+STRESS, [+3]:XYZ, [+1]:AF
			{
				int iUnitIndex = 0;
				int iUnit = m_pDoc->GetUnitIndex();
				if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
				else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
				if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
					iUnitIndex = 0;
				}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
				{
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ----------              */
					if (l_iUnitIndexThickInt < 0) {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "f");
					}
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
				}
				else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,Kï¿½lï¿½n
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]ï¿½Å’ï¿½
				}
// 2009.11.12 K.Matsuo ADAPï¿½Ì•ñ���ï¿½ï¿½Ú‚Ì�ï¿½ï¿½ï¿½Î‰ï¿½ï¿½B2ï¿½Â–Ú‚ï¿½N1[XXX],K1[XXX]ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½Æ‚ï¿½Aï¿½Ê�ï¿½ï¿½Ú‚Ì’lï¿½ï¿½ï¿½ -->
				else if(strncmp(szDispLabel[i], "Const", strlen("Const")) == 0 ||
						strncmp(szDispLabel[i], "Eg", strlen("Eg")) == 0 ||
						strncmp(szDispLabel[i], "Fraction", strlen("Fraction")) == 0)
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]ï¿½Å’ï¿½
				}
// 2009.11.12 K.Matsuo ADAPï¿½Ì•ñ���ï¿½ï¿½Ú‚Ì�ï¿½ï¿½ï¿½Î‰ï¿½ï¿½B2ï¿½Â–Ú‚ï¿½N1[XXX],K1[XXX]ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½Æ‚ï¿½Aï¿½Ê�ï¿½ï¿½Ú‚Ì’lï¿½ï¿½ï¿½ <--
				else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%ï¿½n, Fit
				{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]ï¿½Å’ï¿½
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
					if ((0 != strstr(szDispLabel[i], "Fit")) &&
						(1 == g_lXmpType)) {
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- { ---------- */
////						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f"); //[.##]ï¿½Å’ï¿½
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ----------              */
//						strcpy(szDecimalPointBuff, "%.3e");
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
#if FIT_DISP_SCOUT == 3
						strcpy(szDecimalPointBuff, "");
#elif FIT_DISP_SCOUT == 2
						strcpy(szDecimalPointBuff, "%.2e");
#elif FIT_DISP_SCOUT == 1
						strcpy(szDecimalPointBuff, "%.3e");
#else
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f");
#endif
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]ï¿½Å’ï¿½
					}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
				}
				else //ï¿½ï¿½ï¿½Ì‘ï¿½
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
				}

				if(strstr(szDispLabel[i], "Thick") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Thick*
				{

					//MeasurementData
					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * g_dUnitConvert[iUnitIndex]);  //
				}
				else if(strstr(szDispLabel[i], "%") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½%ï¿½n
				{
					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * 100);
				}
				else //
				{
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
//					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
					if (0 == strlen(szDecimalPointBuff)) {
						long l_lExp = m_pDoc->GetExpDouble(OnePointData.dData[i]);
						strBuffer.Format("%.2fe%+03d", OnePointData.dData[i] / pow(10, l_lExp), l_lExp);
					} else {
						strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
					}
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
				}
				AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
				iCol++;
			}
		//2009.08.25 bagus stress --}--
		//2009.09.10 bagus stres  --{--
			//LightIntensityListGrid and Graph
			if (iCount == iPoint){
				//2009.09.13 bagus stress --{--
				//ï¿½ï¿½ï¿½İ‚ï¿½Thickï¿½Ìƒ^ï¿½uï¿½Å‚È‚ï¿½ï¿½ï¿½Î�ï¿½ï¿½sï¿½ï¿½ï¿½È‚ï¿½
				if(GetCurrentTab() == 0){
					AddLightIntensityListAndGraph(iCount);
				}
				//2009.10.02 bagus se --{--
				m_nGraphRow = iCount;
				//2009.10.02 bagus se --}--
			}
		//2009.09.10 bagus stres  --}--
			break;
		}
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
		if (0 < m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSetNoGet()) {
			if (iCount == m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSetNoGet()) {
				l_wIncludeColorSetNo = iCount;
			}
		}
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */
	}

	//2010.01.01.15 bagus 2point-Distance --{--
	//2ï¿½_ï¿½Ô‹ï¿½ï¿½ï¿½ï¿½Í”ï¿½ï¿½ï¿½
	int iDiv = 1;
	if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		iDiv = 2;
	}

	if(m_pDoc->GetSeqMeasMode() == SEQ_ON_NORMALMEASURE) //ï¿½Ê�í‘ªï¿½è’†
	{
		strBuffer.Format("%5d / %5d ", iPoint / iDiv, iMeasureNum / iDiv);
	}
	else if(m_pDoc->GetSeqMeasMode() == SEQ_ON_REMEASURE) //ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½
	{
		strBuffer.Format("%5d / %5d ", m_pDoc->GetReMeasurePointCnt() / iDiv, m_pDoc->GetReMeasNumScans() / iDiv/*iMeasureNum*/);
	}
	else //ï¿½ï¿½ï¿½è’†ï¿½Å‚È‚ï¿½
	{
		strBuffer.Format("%5d / %5d ", m_pDoc->GetStoreCount() / iDiv, iMeasureNum / iDiv);
	}
	//2010.01.01.15 bagus 2point-Distance --}--

	//2009.12.03 bagus --{--
	//SetDlgItemText(IDC_MEASUREMENT_POINT, strBuffer);
	CString strTmp;
	GetDlgItemText(IDC_MEASUREMENT_POINT, strTmp);
	if(strBuffer != strTmp){
		SetDlgItemText(IDC_MEASUREMENT_POINT, strBuffer);
	}
	//2009.12.03 bagus --}--


	iShowCompletionPointGraph = iCurrentSetPoint; //ï¿½ï¿½Ê•\ï¿½ï¿½ï¿½Ï‚İ�ï¿½ï¿½ï¿½ï¿½Xï¿½V
/* modified 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
//	m_pDoc->SetShowCompPointGraph(iShowCompletionPointGraph);
/* modified 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ----------              */
	WORD l_wShowCompletionPointGraph = 0;
	m_pMainFrame->m_pMeaTabViewObj->MRCS_ShowCompletionPointSnap(0, &l_wShowCompletionPointGraph);
	if (0 == l_wShowCompletionPointGraph) {
		m_pDoc->SetShowCompPointGraph(iShowCompletionPointGraph);
	} else {
		/* ï¿½\ï¿½ï¿½ï¿½Ï‚İƒ|ï¿½Cï¿½ï¿½ï¿½gï¿½Ì‹Lï¿½ï¿½ï¿½İ‚ï¿½(ï¿½Â‚Ü‚ï¿½Ä‘ï¿½ï¿½ï¿½)	*/
		m_pDoc->SetShowCompPointGraph(l_wShowCompletionPointGraph);
		/* ï¿½ï¿½ï¿½ï¿½Ô�ï¿½ï¿½ï¿½ï¿½ß‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
		if (l_wShowCompletionPointGraph == m_pDoc->GetShowCompPointList()) {
			m_pDoc->SetCurrentSetPoint(l_wShowCompletionPointGraph);
		}
	}
	if (0 < l_wIncludeColorSetNo) {
		/* ï¿½wï¿½ï¿½sï¿½Fï¿½Ô�ï¿½ï¿½İ‚ï¿½	*/
		m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSet(l_wIncludeColorSetNo, FALSE, TRUE);
	}
/* modified 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */


	if(m_bCellSelectedFlg == FALSE) //ï¿½Iï¿½ï¿½ï¿½sï¿½È‚ï¿½
	{
		//ï¿½Ê‰ï¿½Ê�ï¿½ï¿½Í�Aï¿½Ç‰ï¿½ï¿½sï¿½ÉˆÚ“ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½æ‚¤ï¿½ï¿½ï¿½ï¿½(ï¿½\ï¿½tï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½)
		CWnd* pFocusWnd = GetFocus();
		if(pFocusWnd == NULL)
		{
			return 2;
		}
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- { ---------- */
//		m_MeasurementListGrid.EnsureVisible(iShowCompletionPointGraph, TRUE);
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ----------              */
			if (
				(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) &&
				(m_pDoc->GetMeasMode() != MEASMODE_TEST)
				) {
				m_MeasurementListGrid.EnsureVisible(iShowCompletionPointGraph, TRUE);
			}
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- } ---------- */
	}


	return 0;
}


//2009.08.25 bagus stress --{--
int CMeasurementGraphDlg::SetMeasurementLSData_One(int nStressLine,int nRow,STRESSRESULT *pOneData)
{
	STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;
	CString strBuffer;
	int nSection;
	int nValidCount = 0;

	if(nStressLine < 1 || nStressLine > STRESS_LINES_MAX){
		return 0;
	}

	//ï¿½ï¿½ï¿½Vï¿½sï¿½Æ‚ï¿½ï¿½Ä—Lï¿½ï¿½ï¿½Èƒï¿½ï¿½Cï¿½ï¿½ï¿½Å‚È‚ï¿½ï¿½ï¿½Î•\ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(!stressStage.Line[nStressLine-1].bValidLine){
		return 0;
	}

	for(nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
		//ï¿½Lï¿½ï¿½ï¿½Å‚È‚ï¿½ï¿½Zï¿½Nï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Í‚È‚É‚ï¿½ï¿½ï¿½ï¿½È‚ï¿½
		if(!stressStage.Line[nStressLine-1].bScanValid[nSection]){
			continue;
		}
		//LmSn
		strBuffer.Format("L%dS%d",nStressLine,nSection+1);
		AddMeasurementListGrid_Data(nRow + nValidCount, 0, strBuffer);
		//X-Start[mm]
		//2009.09.09 bagus stres --{--
		//um -> mm
		//strBuffer.Format("%ld",stressStage.Line[nStressLine-1].SectPos[nSection].lScanStartPosX);
		strBuffer.Format("%.3lf",stressStage.Line[nStressLine-1].SectPos[nSection].lScanStartPosX / (double)MICROMETRE);
		AddMeasurementListGrid_Data(nRow + nValidCount, 1, strBuffer);
		//X-End[mm]
		//strBuffer.Format("%ld",stressStage.Line[nStressLine-1].SectPos[nSection].lScanEndPosX);
		strBuffer.Format("%.3lf",stressStage.Line[nStressLine-1].SectPos[nSection].lScanEndPosX / (double)MICROMETRE);
		AddMeasurementListGrid_Data(nRow + nValidCount, 2, strBuffer);
		//Y[mm]
		//strBuffer.Format("%ld",stressStage.Line[nStressLine-1].SectPos[nSection].lScanPosY);
		strBuffer.Format("%.3lf",stressStage.Line[nStressLine-1].SectPos[nSection].lScanPosY / (double)MICROMETRE);
		AddMeasurementListGrid_Data(nRow + nValidCount, 3, strBuffer);
		//2009.09.09 bagus stres --}--
		//Stress
		strBuffer.Format("%.3f",pOneData->dStressSection[nSection]);
		AddMeasurementListGrid_Data(nRow + nValidCount, 4, strBuffer);
		//
		strBuffer.Format("---");
		for(int nRestCol = 5;nRestCol < m_MeasurementListGrid.GetColumnCount() -5;nRestCol++){
			AddMeasurementListGrid_Data(nRow + nValidCount, nRestCol, strBuffer);
		}
		nValidCount++;
	}
	return nValidCount;
}
//2009.08.25 bagus stress --}--
//2009.08.22 bagus stress --{--
// LSï¿½fï¿½[ï¿½^ï¿½ï¿½Listï¿½Oï¿½ï¿½ï¿½bï¿½hï¿½ÖƒZï¿½bï¿½gï¿½ï¿½ï¿½ï¿½×‚ÌŠÖ�ï¿½
int CMeasurementGraphDlg::SetMeasurementLSData(int nStartRow)
{
	int nShowStressData = 0;
	STRESSRESULT tResult;
	int nShowRowNum;
	int nShowCompLine = 0;
	STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;
	CString strBuffer;

	if(m_pDoc->GetCurrentMeasureStressLine() <= m_pDoc->GetShowCompStressGraph()){
		return 0;
	}
	int nTotalNum = 0;
	nShowRowNum = 0;
	for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
		if(m_pDoc->GetOneStressLineDataValid(nLine+1)){
			m_pDoc->GetOneStressLineData(&tResult,nLine+1);
			nShowRowNum += SetMeasurementLSData_One(nLine+1,nShowRowNum+nStartRow+1,&tResult);
			nShowCompLine = nLine+1;
		}
		if(stressStage.Line[nLine].bValidLine){
			for(int nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
				if(stressStage.Line[nLine].bScanValid[nSection]){
					nTotalNum++;
				}
			}
		}

	}

	//2009.09.10 bagus stress --{--
	BOOL bCellSelectedFlg = FALSE;
	int i;
	for(i=0; i < nStartRow + nShowRowNum; i++)
	{
		int iFocus = (m_MeasurementListGrid.GetItemState(i+1, 1) & GVIS_SELECTED);
		if((iFocus != 0)) //ï¿½sï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½ï¿½ï¿½ÆˆÈ�~ï¿½Í�Å�Vï¿½sï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½)
		{
			bCellSelectedFlg = TRUE;
			break;
		}
	}
	if(!bCellSelectedFlg){
		//ï¿½Ê‰ï¿½Ê�ï¿½ï¿½Í�Aï¿½Ç‰ï¿½ï¿½sï¿½ÉˆÚ“ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½æ‚¤ï¿½ï¿½ï¿½ï¿½(ï¿½\ï¿½tï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½)
		CWnd* pFocusWnd = GetFocus();
		if(pFocusWnd == NULL)
		{
		}else{
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- { ---------- */
//			m_MeasurementListGrid.EnsureVisible(i, TRUE);
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ----------              */
			if (
				(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) &&
				(m_pDoc->GetMeasMode() != MEASMODE_TEST)
				) {
				m_MeasurementListGrid.EnsureVisible(i, TRUE);
			}
/* modified 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- } ---------- */
			NM_GRIDVIEW dDummy;
			//ï¿½Å�Vï¿½Ìƒfï¿½[ï¿½^ï¿½ÌƒOï¿½ï¿½ï¿½tï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½é‚½ï¿½ß‚Ì�ï¿½ï¿½ï¿½
			dDummy.iColumn = 0;
			dDummy.iRow = i;
			if(nShowCompLine > 0){
				OnSelChangeing((NMHDR *)&dDummy, NULL);
			}
		}
	}
	//2009.09.10 bagus stress --}--

	strBuffer.Format("%5d / %5d",nShowRowNum,nTotalNum);
	//2009.12.03 bagus --{--
	//SetDlgItemText(IDC_MEASUREMENT_LS,strBuffer);
	CString strTmp;
	GetDlgItemText(IDC_MEASUREMENT_LS,strTmp);
	if(strTmp != strBuffer){
		SetDlgItemText(IDC_MEASUREMENT_LS,strBuffer);
	}
	//2009.12.03 bagus --}--

	m_pDoc->SetShowCompStressGraph(nShowCompLine);

	return 1;
}
//2009.08.22 bagus stress --}--




// =========================================================================
//ï¿½ï¿½ï¿½èŒ‹ï¿½Ê•\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½p)
LRESULT CMeasurementGraphDlg::OnSetMeasurementData_TestMode(WPARAM wparam, LPARAM lparam)
{
	m_iModeStatus = 2;

	ONE_POINT_DATA OnePointData; //1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Ì‘ï¿½ï¿½ï¿½fï¿½[ï¿½^
	int iNowPoint = 0;
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	CString strBuffer;
	int iDispLabelCount = 0;
	int i;

		// Kojika 20090528 Add
		SYSTEM_CONFIG l_SystemConfig;
		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
		// Kojika 20090528 Add End

	SR_TURRET l_SrTurret[SR_LENS_MAX];
	SR_FILTER l_srFilter[SR_FILTER_MAX];

	memset(&OnePointData, 0, sizeof(OnePointData));
	memset(szDispLabel, 0, sizeof(szDispLabel));

/* added 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
	int l_iUnitIndexThickInt = -1;		/* -1 ï¿½Í�ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½	*/
	SR_FIGURE_FORMAT l_SrFigureFormat;
	ConfigFile_GetNanoSpecIni(&l_SrFigureFormat,
					CONFIG_FILE_SR_FIGURE_FORMAT);
	if (0 != l_SrFigureFormat.iThicknessIntOnly) {
		l_iUnitIndexThickInt = 3;		/* -1 ï¿½ÈŠOï¿½ÍƒCï¿½ï¿½ï¿½fï¿½bï¿½Nï¿½Xï¿½l	*/
	}
/* added 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */

	iNowPoint = m_pDoc->GetStoreCount();
	iDispLabelCount = m_pDoc->GetDispLabelCount();
	m_pDoc->GetDispLabel(szDispLabel);
	m_pDoc->GetOnePointData(&OnePointData, iNowPoint);


	//ï¿½Oï¿½ï¿½ï¿½tï¿½\ï¿½ï¿½ï¿½ÍˆÍ�Ä’ï¿½ï¿½ï¿½
	MAIN_RCP_INFO l_MainRcpInfo;
	MEAS_PROG_INFO l_Meas;
	RCP_DATA rcp_data;
	memset(&l_MainRcpInfo, 0, sizeof(l_MainRcpInfo));
	memset(&l_Meas, 0, sizeof(l_Meas));
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	RecipeFile_LoadRecipe(&l_MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE); //
	RecipeFile_LoadRecipe(&l_Meas, l_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);

	if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
	{
		m_iEndWaveLength = rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd = l_Meas.ScanParams._SR.WavelenRange.wEnd;
		m_iStartWaveLength = rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wStart = l_Meas.ScanParams._SR.WavelenRange.wStart;
		m_pDoc->SetRcpData(&rcp_data);
// 2009.05.20 K.Matsuo -->
//		MeasurementListGrid_Init();
		LightIntensityListGrid_Init();
// 2009.05.20 K.Matsuo <--
		DrawLightIntensityGraph_Init();
	}

	int iIndex=0;

	char szXyzFormat[255+1];
	memset(szXyzFormat, 0, sizeof(szXyzFormat));
	sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
	///// Point Position X //////
	iIndex++;
	double dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
	strBuffer.Format(szXyzFormat, dPositionX);
	AddMeasurementListGrid_Data(iNowPoint, iIndex++, strBuffer);

	///// Point Position Y //////
	double dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
	strBuffer.Format(szXyzFormat, dPositionY);
	AddMeasurementListGrid_Data(iNowPoint, iIndex++, strBuffer);

	///	///// Point Position Z //////
	double dPositionZ = (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0]) / 10;
	strBuffer.Format(szXyzFormat, dPositionZ);
	AddMeasurementListGrid_Data(iNowPoint, iIndex++, strBuffer);



	///// ScanTypeï¿½ï¿½Paramaeter /////
	int iHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType;
	if(iHeadType == HEAD_TYPE_SR)
	{
		//// ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½è—“	////
		ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
		if(m_SrConfig.bAutoFocus){
			strBuffer.Format("%s", OnePointData.szShowAfValid);
			AddMeasurementListGrid_Data(iNowPoint, iIndex++, strBuffer);
		}

		switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
		{
			case MEAS_PROG_TYPE_SR_THICKNESS:
			case MEAS_PROG_TYPE_SR_REFLECTANCE:
// 2009.10.26 K.Matsuo TR -->
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.10.26 K.Matsuo TR <--
// 2010.01.11 bagus  GTR --{--
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2010.01.11 bagus  GTR --}--

			int iCol;
			iCol = 0;
			for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+3]:XYZ, [+1]:AF
			{
				int iUnitIndex = 0;
				int iUnit = m_pDoc->GetUnitIndex();
				if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
				else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
				if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
					iUnitIndex = 0;
				}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

				char szDecimalPointBuff[255+1];
				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
				{
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ----------              */
					if (l_iUnitIndexThickInt < 0) {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "f");
					}
/* modified 2017.04.15 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
				}
				else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,Kï¿½lï¿½n
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]ï¿½Å’ï¿½
				}
// 2009.11.12 K.Matsuo ADAPï¿½Ì•ñ���ï¿½ï¿½Ú‚Ì�ï¿½ï¿½ï¿½Î‰ï¿½ï¿½B2ï¿½Â–Ú‚ï¿½N1[XXX],K1[XXX]ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½Æ‚ï¿½Aï¿½Ê�ï¿½ï¿½Ú‚Ì’lï¿½ï¿½ï¿½ -->
				else if(strncmp(szDispLabel[i], "Const", strlen("Const")) == 0 ||
						strncmp(szDispLabel[i], "Eg", strlen("Eg")) == 0 ||
						strncmp(szDispLabel[i], "Fraction", strlen("Fraction")) == 0)
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]ï¿½Å’ï¿½
				}
// 2009.11.12 K.Matsuo ADAPï¿½Ì•ñ���ï¿½ï¿½Ú‚Ì�ï¿½ï¿½ï¿½Î‰ï¿½ï¿½B2ï¿½Â–Ú‚ï¿½N1[XXX],K1[XXX]ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½Æ‚ï¿½Aï¿½Ê�ï¿½ï¿½Ú‚Ì’lï¿½ï¿½ï¿½ <--
				else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%ï¿½n, Fit
				{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]ï¿½Å’ï¿½
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
					if ((0 != strstr(szDispLabel[i], "Fit")) &&
						(1 == g_lXmpType)) {
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- { ---------- */
////						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f"); //[.##]ï¿½Å’ï¿½
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ----------              */
//						strcpy(szDecimalPointBuff, "%.3e");
///* modified 2014.10.27 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½ %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
#if FIT_DISP_SCOUT == 3
						strcpy(szDecimalPointBuff, "");
#elif FIT_DISP_SCOUT == 2
						strcpy(szDecimalPointBuff, "%.2e");
#elif FIT_DISP_SCOUT == 1
						strcpy(szDecimalPointBuff, "%.3e");
#else
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f");
#endif
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]ï¿½Å’ï¿½
					}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
				}
				else //ï¿½ï¿½ï¿½Ì‘ï¿½
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //ï¿½Iï¿½ï¿½Unitï¿½Å•Ï“ï¿½
				}

				if(strstr(szDispLabel[i], "Thick") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Thick*
				{

					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * g_dUnitConvert[iUnitIndex]);

				}
//Saiki 20130123 add --->
				else if(strstr(szDispLabel[i], "%") != NULL) //ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½%ï¿½n
				{

					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * 100);

				}
//Saiki 20130123 add <---
				else //
				{
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- { ---------- */
//					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ----------              */
					if (0 == strlen(szDecimalPointBuff)) {
						long l_lExp = m_pDoc->GetExpDouble(OnePointData.dData[i]);
						strBuffer.Format("%.2fe%+03d", OnePointData.dData[i] / pow(10, l_lExp), l_lExp);
					} else {
						strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
					}
/* modified 2014.11.22 hmenjo Fit ï¿½wï¿½ï¿½ï¿½\ï¿½ï¿½(2) ---------- } ---------- */
				}

				AddMeasurementListGrid_Data(iNowPoint, iCol+iIndex, strBuffer );
				iCol++;
			}


			//ï¿½ï¿½ï¿½mï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½tï¿½É’Ç‰ï¿½
			TEST_MODE_MEASRCP_CONDITION MeasRcpCon;
			memset(&MeasRcpCon, 0, sizeof(MeasRcpCon));
			m_pDoc->GetMeasRcpCon(iNowPoint, &MeasRcpCon);


			//ï¿½ï¿½ï¿½èƒŒï¿½ï¿½ï¿½Y
			ConfigFile_GetNanoSpecIni(&l_SrTurret, CONFIG_FILE_SR_TURRET);
			strBuffer.Format("%s", l_SrTurret[MeasRcpCon.iLens].szName);
			AddMeasurementListGrid_Data(iNowPoint, iCol+iIndex, strBuffer );
			iCol++;

			//ï¿½ï¿½ï¿½wï¿½tï¿½Bï¿½ï¿½ï¿½^

			ConfigFile_GetNanoSpecIni(l_srFilter, CONFIG_FILE_SR_FILTER);
			int iFilterIndex;
			int iOptItem;
			iOptItem = MeasRcpCon.wOpticsFilterType;
			switch ( iOptItem )
			{
				case OPT_FILTER_UNKNOWN:
					break;
				case OPT_FILTER_OPEN:
					// Kojika 20090528 Change
					//strBuffer.Format("%s", OPTICAL_FILTER_TYPE_ITEM[iOptItem]);
					if(l_SystemConfig.nLanguage == 0){
							strBuffer.Format("%s", OPTICAL_FILTER_TYPE_ITEM_ENU[iOptItem]);
					}
					else{
							strBuffer.Format("%s", OPTICAL_FILTER_TYPE_ITEM_JPN[iOptItem]);
					}
					// Kojika 20090528 Change End
					break;
				case OPT_FILTER_DARK:
					break;
				case OPT_FILTER_POS1:
				case OPT_FILTER_POS2:
				case OPT_FILTER_POS3:
					iFilterIndex = iOptItem - 3;
					if ( l_srFilter[iFilterIndex].bEnable )
					{
						strBuffer.Format("%s", l_srFilter[iFilterIndex].szName);
					}
					break;
				default:
					break;
			}

			//2010.01.11 bagus GTR --{--
			//ï¿½Kï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ß—ï¿½ï¿½É‚Íƒï¿½ï¿½ï¿½ï¿½Yï¿½ï¿½ï¿½Ú‚Í‚È‚ï¿½
			if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
			}else{
				AddMeasurementListGrid_Data(iNowPoint, iCol+iIndex, strBuffer );
				iCol++;
			}
			//2010.01.11 bagus GTR --}--

			//ï¿½ï¿½ï¿½è��ï¿½ï¿½
			strBuffer.Format("%.3lf", /*TestModeData.*/MeasRcpCon.dIntegTime);
			AddMeasurementListGrid_Data(iNowPoint, iCol+iIndex, strBuffer );
			iCol++;


			AddLightIntensityListAndGraph(iNowPoint);


				break;
			default:
				break;
		}
		//2010.01.01.15 bagus 2point-Distance --{--
		//2ï¿½_ï¿½Ô‹ï¿½ï¿½ï¿½ï¿½Í”ï¿½ï¿½ï¿½
		if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			iNowPoint /= 2;
		}
		//2010.01.01.15 bagus 2point-Distance --}--
		strBuffer.Format("%5d / %5d ", iNowPoint, iNowPoint);
		SetDlgItemText(IDC_MEASUREMENT_POINT, strBuffer);
	}

	return 0L;
}





// =========================================================================
//
LRESULT CMeasurementGraphDlg::OnSetRefData_TestMode(WPARAM wparam, LPARAM lparam)
{
	m_iModeStatus = 1;

	//2010.01.13 bagus --{--
	//ï¿½Kï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ß—ï¿½ï¿½Å�ï¿½sï¿½\ï¿½ï¿½ï¿½Í‚È‚ï¿½
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
		return 0L;
	}
	//2010.01.13 bagus --}--

	//ï¿½Oï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
	m_MeasureGraph.ClearGraphData();

	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½Eï¿½_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½\ï¿½ï¿½
	int iPoint = m_pDoc->GetStoreCount();
	AddLightIntensityListAndGraph_Ref(iPoint + 1); //ï¿½ï¿½sï¿½\ï¿½ï¿½(ï¿½ã‚©ï¿½ï¿½OnSetMeasurementData_TestMode()ï¿½Å“ï¿½ï¿½lï¿½ï¿½ï¿½ã�‘ï¿½ï¿½)

	return 0L;
}




// =========================================================================
//
void CMeasurementGraphDlg::DrawLightIntensityGraph_Init()
{

//2009.08.31 bagus stress --{--
//ï¿½wï¿½bï¿½hï¿½^ï¿½Cï¿½vï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É‚Í’ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ÅŠÖ�ï¿½ï¿½É•ï¿½ï¿½ï¿½ï¿½ï¿½B
	switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType){
	case HEAD_TYPE_SR:
	default:
		DrawLightIntensityGraph_Init_SR();
		break;
	//2009.09.15 bagus se --{--
	case HEAD_TYPE_SE:
		DrawLightIntensityGraph_Init_SE();
		break;
	//2009.09.15 bagus se --}--
	case HEAD_TYPE_STRESS:
		DrawLightIntensityGraph_Init_Stress(-1,-1);
		break;
	}

	//2009.09.03 bagus se ï¿½Iï¿½[ï¿½gï¿½Xï¿½Pï¿½[ï¿½ï¿½ --{--
	m_MeasureGraph.SetAutoScaleMode(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale);
	//2009.09.03 bagus se ï¿½Iï¿½[ï¿½gï¿½Xï¿½Pï¿½[ï¿½ï¿½ --}--
}

void CMeasurementGraphDlg::DrawLightIntensityGraph_Init_SR()
{
	CNanoMeasureGraph::GraphConfig tGraphConfig;
	CNanoMeasureGraph::LineConfig tLineConfig;
	memset(&tGraphConfig,NULL,sizeof(tGraphConfig));
	tGraphConfig.UseLineSelect = TRUE;
	strcpy(tGraphConfig.UpperTitle,"SPECTRUM GRAPH");
	strcpy(tGraphConfig.LowerTitle,"");
	tGraphConfig.CanvasMargin = 7;	//ï¿½Oï¿½ï¿½ï¿½tï¿½`ï¿½ï¿½]ï¿½ï¿½

	///// ï¿½gï¿½ï¿½ï¿½ÍˆÍ‚Ì�İ’ï¿½ /////
	switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType){
	case HEAD_TYPE_SR:
	default:
		switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType){
		case MEAS_PROG_TYPE_SR_THICKNESS:
			//Saiki 20090530 Delete ----->
			//strcpy(tGraphConfig.Y_AxisTitle,"Intensity[%]");
			//Saiki 20090530 Delete <-----
			strcpy(tGraphConfig.X_AxisTitle,"Wavelength [nm]");
			tGraphConfig.X_Max = m_iEndWaveLength;
			if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW)
			{
				//Saiki 20090530 Add ----->
				strcpy(tGraphConfig.Y_AxisTitle,"Reflective Light Intensity");
				//Saiki 20090530 Add <-----
				tGraphConfig.Y_Max= 65535;
			}
			else
			{
				//Saiki 20090530 Add ----->
				strcpy(tGraphConfig.Y_AxisTitle,"Reflective Light Intensity [%]");
				//Saiki 20090530 Add <-----
				tGraphConfig.Y_Max= 100;
			}
			tGraphConfig.X_Origin = m_iStartWaveLength;
			tGraphConfig.Y_Origin = 0;
			tGraphConfig.X_Step = 50;

			if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW)
			{
				tGraphConfig.Y_Step = 10000;
			}
			else
			{
				tGraphConfig.Y_Step = 10;
			}
			break;

		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			strcpy(tGraphConfig.Y_AxisTitle,"Reflective Light Intensity [%]");
			strcpy(tGraphConfig.X_AxisTitle,"Wavelength [nm]");
			strcpy(tGraphConfig.UpperTitle,"SPECTRUM GRAPH");
			tGraphConfig.X_Max = m_iEndWaveLength;
			if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW)
			{
				tGraphConfig.Y_Max= 65535;
			}
			else
			{
				tGraphConfig.Y_Max= 100;
			}
			tGraphConfig.X_Origin = m_iStartWaveLength;
			tGraphConfig.Y_Origin = 0;
			tGraphConfig.X_Step = 50;

			if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW)
			{
				tGraphConfig.Y_Step = 10000;
			}
			else
			{
				tGraphConfig.Y_Step = 10;
			}
			break;

		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.09.03 K.Matsuo -->
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2009.09.03 K.Matsuo <--
			//2010.01.11 bagus GTR --{--
			if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW)
			{
				strcpy(tGraphConfig.Y_AxisTitle,"Transmissive Light Intensity");
				tGraphConfig.Y_Max= 65535;
				tGraphConfig.Y_Step = 10000;
			}
			else
			{
				strcpy(tGraphConfig.Y_AxisTitle,"Transmissive Light Intensity [%]");
				tGraphConfig.Y_Max= 100;
				tGraphConfig.Y_Step = 10;
			}
			//2010.01.11 bagus GTR --}--
			strcpy(tGraphConfig.X_AxisTitle,"Wavelength [nm]");
			strcpy(tGraphConfig.UpperTitle,"SPECTRUM GRAPH");
			tGraphConfig.X_Max = m_iEndWaveLength;
			tGraphConfig.X_Origin = m_iStartWaveLength;
			tGraphConfig.Y_Origin = 0;
			tGraphConfig.X_Step = 50;
			break;

		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
			break;
		}
		break;
	case HEAD_TYPE_STRESS:
		//Saiki 20090530 Delete ----->
		//strcpy(tGraphConfig.Y_AxisTitle,"Intensity[%]");
		//Saiki 20090530 Delete <-----
		strcpy(tGraphConfig.X_AxisTitle,"Wavelength [nm]");
		tGraphConfig.X_Max = m_iEndWaveLength;
		if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW)
		{
			//Saiki 20090530 Add ----->
			strcpy(tGraphConfig.Y_AxisTitle,"Reflective Light Intensity");
			//Saiki 20090530 Add <-----
			tGraphConfig.Y_Max= 65535;
		}
		else
		{
			//Saiki 20090530 Add ----->
			strcpy(tGraphConfig.Y_AxisTitle,"Reflective Light Intensity [%]");
			//Saiki 20090530 Add <-----
			tGraphConfig.Y_Max= 100;
		}
		tGraphConfig.X_Origin = m_iStartWaveLength;
		tGraphConfig.Y_Origin = 0;
		tGraphConfig.X_Step = 50;

		tGraphConfig.Y_Step = 10;
		break;
	}

// 2014.04.02 bagus wavelength step modified -->
//	tGraphConfig.PointNum = (tGraphConfig.X_Max)-(tGraphConfig.X_Origin)+1;//200;//NANO_MEASURE_MAX_POINT_DATA;
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	switch(m_SrConfig.nWavelengthStep){
	case SR_WAVELENGTH_STEP_05_NM:
		tGraphConfig.PointNum = ((tGraphConfig.X_Max)-(tGraphConfig.X_Origin))*2+1;//200;//NANO_MEASURE_MAX_POINT_DATA;
		break;
	case SR_WAVELENGTH_STEP_1_NM:
	default:
		tGraphConfig.PointNum = (tGraphConfig.X_Max)-(tGraphConfig.X_Origin)+1;//200;//NANO_MEASURE_MAX_POINT_DATA;
		break;
	}
// 2014.04.02 bagus wavelength step modified <--
	tGraphConfig.CanvasBackColor = WHITE_COLOR;
	tGraphConfig.GraphBackColor = WHITE_COLOR;//RGB(0x00,0xAA,0xAA);
	tGraphConfig.GraphBorderColor = RGB(0x00,0x00,0x00);
	tGraphConfig.GraphStepLineColor = RGB(0xAA,0xAA,0xAA);
	tGraphConfig.TitleTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.UnitTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.SelectingCalibrationColor= RED_COLOR;
	tGraphConfig.SelectedCalibrationColor = BLUE_COLOR;
	tGraphConfig.UseCalibration = FALSE;
	//tGraphConfig.CalibrationLineNum = 7;
	m_MeasureGraph.SetGraphConfig(tGraphConfig);


	//GetData and SetData
// 2014.04.02 bagus wavelength step modified -->
//	int Rows=(m_iEndWaveLength)-(m_iStartWaveLength)+2;
	int Rows;
	switch(m_SrConfig.nWavelengthStep){
	case SR_WAVELENGTH_STEP_05_NM:
		Rows=((m_iEndWaveLength)-(m_iStartWaveLength))*2+2;
		break;
	case SR_WAVELENGTH_STEP_1_NM:
	default:
		Rows=(m_iEndWaveLength)-(m_iStartWaveLength)+2;
		break;
	}
// 2014.04.02 bagus wavelength step modified <--
	if(Rows<=DEF_LITENSITY_GRID_ROWLINE_COUNT) Rows=DEF_LITENSITY_GRID_ROWLINE_COUNT;
	TRY{
		m_LightIntensityListGrid.SetRowCount(Rows);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	////SetLine Condition ////
	for(int i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
		memset(&tLineConfig,NULL,sizeof(tLineConfig));

// 2010.01.21 bagus Gantry --{--
#if 0
		if (((m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS) && (i<=2) )
		||( (m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE) && (i<=2) )
		//2010.01.11 bagus GTR --{--
		||( (m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G) && (i<=1) )
		//2010.01.11 bagus GTR --}--
		 || (i==0))
		{
			tLineConfig.UseFlag = TRUE;
			tLineConfig.Visible = TRUE;
		}
		else{
			tLineConfig.UseFlag = FALSE;
			tLineConfig.Visible = FALSE;
		}
#else
		if (m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) {
			if (((m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS) && (i<=2))
			 || ((m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE) && (i<=2))
			 || ((m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G) && (i<=1))
			 || (i==0)
			) {
				tLineConfig.UseFlag = TRUE;
				tLineConfig.Visible = TRUE;
			} else {
				tLineConfig.UseFlag = FALSE;
				tLineConfig.Visible = FALSE;
			}
		} else {
			if (((m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS) && (i<=2))
			 || (i==0)
			) {
				tLineConfig.UseFlag = TRUE;
				tLineConfig.Visible = TRUE;
			} else {
				tLineConfig.UseFlag = FALSE;
				tLineConfig.Visible = FALSE;
			}
		}
#endif
// 2010.01.21 bagus Gantry --}--

		tLineConfig.DataIndex = i;
		if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW)
		{
			//2010.01.11 bagus GTR --{--
			if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
				if (i==0) {
					tLineConfig.Color = RED_COLOR;
					tLineConfig.PenStyle = PS_SOLID;
					wsprintf(tLineConfig.LineName,"Reference Data ");
				} else if (i==1) {
					tLineConfig.Color = GREEN_COLOR;
					tLineConfig.PenStyle = PS_SOLID;//PS_DOT;
					wsprintf(tLineConfig.LineName,"Dark Data ");
				} else {
					tLineConfig.PenStyle = PS_SOLID;
					wsprintf(tLineConfig.LineName,"null%02d",i);
				}
			}else{
			//2010.01.11 bagus GTR --}--
				if (i==0) {
					tLineConfig.Color = BLUE_COLOR;
					// =PS_SOLID:ï¿½ï¿½ï¿½ï¿½
					// PS_DASH:ï¿½jï¿½ï¿½
					// PS_DOT:ï¿½_ï¿½ï¿½
					// PS_DASHDOT:ï¿½Pï¿½_ï¿½ï¿½ï¿½ï¿½
					// PS_DASHDOTDOT:ï¿½Qï¿½_ï¿½ï¿½ï¿½ï¿½
					tLineConfig.PenStyle = PS_SOLID;
					wsprintf(tLineConfig.LineName,"Sample Data ");
				} else if (i==1) {
					tLineConfig.Color = RED_COLOR;
					tLineConfig.PenStyle = PS_SOLID;//PS_DOT;
					wsprintf(tLineConfig.LineName,"Reference Data ");
				} else if (i==2) {
					tLineConfig.Color = GREEN_COLOR;
					tLineConfig.PenStyle = PS_SOLID;//PS_DASHDOTDOT;
					wsprintf(tLineConfig.LineName,"Dark Reference ");
				} else {
					tLineConfig.PenStyle = PS_SOLID;
					wsprintf(tLineConfig.LineName,"null%02d",i);
				}
			//2010.01.11 bagus GTR --{--
			}
			//2010.01.11 bagus GTR --}--
		}
		else
		{
			if (i==0) {
				tLineConfig.Color = BLUE_COLOR;
				// =PS_SOLID:ï¿½ï¿½ï¿½ï¿½
				// PS_DASH:ï¿½jï¿½ï¿½
				// PS_DOT:ï¿½_ï¿½ï¿½
				// PS_DASHDOT:ï¿½Pï¿½_ï¿½ï¿½ï¿½ï¿½
				// PS_DASHDOTDOT:ï¿½Qï¿½_ï¿½ï¿½ï¿½ï¿½
				tLineConfig.PenStyle = PS_SOLID;
				wsprintf(tLineConfig.LineName,"Measured Data ");
			} else if (i==1) {
				tLineConfig.Color = RED_COLOR;
				tLineConfig.PenStyle = PS_SOLID;//PS_DOT;
				wsprintf(tLineConfig.LineName,"Analysis Data ");
			} else if (i==2) {
				tLineConfig.Color = GREEN_COLOR;
				tLineConfig.PenStyle = PS_SOLID;//PS_DASHDOTDOT;
				wsprintf(tLineConfig.LineName,"Fit Data    ");
			} else {
				tLineConfig.PenStyle = PS_SOLID;
				wsprintf(tLineConfig.LineName,"null%02d",i);
			}
		}
		m_MeasureGraph.SetLineConfig(i,tLineConfig);
		m_MeasureGraph.ClearGraphData();
	}
}
//2009.09.15 bagus se --{--
void CMeasurementGraphDlg::DrawLightIntensityGraph_Init_SE()
{

	CNanoMeasureGraph::GraphConfig tGraphConfig;
	CNanoMeasureGraph::LineConfig tLineConfig;
	memset(&tGraphConfig,NULL,sizeof(tGraphConfig));
	tGraphConfig.UseLineSelect = TRUE;
	strcpy(tGraphConfig.UpperTitle,"SPECTRUM GRAPH");
	strcpy(tGraphConfig.LowerTitle,"");
	tGraphConfig.CanvasMargin = 7;	//ï¿½Oï¿½ï¿½ï¿½tï¿½`ï¿½ï¿½]ï¿½ï¿½

	///// ï¿½gï¿½ï¿½ï¿½ÍˆÍ‚Ì�İ’ï¿½ /////
	switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType){
	case HEAD_TYPE_SE:
			strcpy(tGraphConfig.X_AxisTitle,"Wave Length [nm]");
			strcpy(tGraphConfig.Y_AxisTitle,"Psi [deg]");
			tGraphConfig.Y_Max= 180;
			tGraphConfig.X_Origin = m_rcp_data.MeasProgInfo.ScanParams._SE.WavelenRange.wStart;
			tGraphConfig.X_Max = m_rcp_data.MeasProgInfo.ScanParams._SE.WavelenRange.wEnd;
			tGraphConfig.Y_Origin = -180;
			tGraphConfig.X_Step = 100;
			tGraphConfig.Y_Step = 30;
			break;
	default:
		return;
		break;
	}

	tGraphConfig.PointNum = (tGraphConfig.X_Max)-(tGraphConfig.X_Origin)+1;//200;//NANO_MEASURE_MAX_POINT_DATA;
	tGraphConfig.CanvasBackColor = WHITE_COLOR;
	tGraphConfig.GraphBackColor = WHITE_COLOR;//RGB(0x00,0xAA,0xAA);
	tGraphConfig.GraphBorderColor = RGB(0x00,0x00,0x00);
	tGraphConfig.GraphStepLineColor = RGB(0xAA,0xAA,0xAA);
	tGraphConfig.TitleTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.UnitTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.SelectingCalibrationColor= RED_COLOR;
	tGraphConfig.SelectedCalibrationColor = BLUE_COLOR;
	tGraphConfig.UseCalibration = FALSE;
	m_MeasureGraph.SetGraphConfig(tGraphConfig);


	//GetData and SetData
	int Rows=(m_iEndWaveLength)-(m_iStartWaveLength)+2;
	if(Rows<=DEF_LITENSITY_GRID_ROWLINE_COUNT) Rows=DEF_LITENSITY_GRID_ROWLINE_COUNT;
	TRY{
		m_LightIntensityListGrid.SetRowCount(Rows);
		m_LightIntensityListGrid.SetColumnCount(4);
		m_LightIntensityListGrid.SetFixedColumnCount(1);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH


	////SetLine Condition ////
	for(int i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
		memset(&tLineConfig,NULL,sizeof(tLineConfig));

		tLineConfig.DataIndex = i;
		if (i==0) {
			tLineConfig.Color = BLUE_COLOR;
			// =PS_SOLID:ï¿½ï¿½ï¿½ï¿½
			// PS_DASH:ï¿½jï¿½ï¿½
			// PS_DOT:ï¿½_ï¿½ï¿½
			// PS_DASHDOT:ï¿½Pï¿½_ï¿½ï¿½ï¿½ï¿½
			// PS_DASHDOTDOT:ï¿½Qï¿½_ï¿½ï¿½ï¿½ï¿½
			tLineConfig.PenStyle = PS_SOLID;
			wsprintf(tLineConfig.LineName,"Measured Data ");
			tLineConfig.UseFlag = TRUE;
			tLineConfig.Visible = TRUE;
		} else if (i==1) {
			tLineConfig.Color = RED_COLOR;
			tLineConfig.PenStyle = PS_SOLID;//PS_DOT;
			wsprintf(tLineConfig.LineName,"Model Data ");
			tLineConfig.UseFlag = TRUE;
			tLineConfig.Visible = TRUE;
		} else if (i==2) {
			tLineConfig.Color = GREEN_COLOR;
			tLineConfig.PenStyle = PS_SOLID;//PS_DASHDOTDOT;
			wsprintf(tLineConfig.LineName,"Fit Data ");
			tLineConfig.UseFlag = TRUE;
			tLineConfig.Visible = TRUE;
		} else {
			tLineConfig.PenStyle = PS_SOLID;
			wsprintf(tLineConfig.LineName,"null%02d",i);
			tLineConfig.UseFlag = FALSE;
			tLineConfig.Visible = FALSE;
		}
		m_MeasureGraph.SetLineConfig(i,tLineConfig);
		m_MeasureGraph.ClearGraphData();
	}
}
//2009.09.15 bagus se --}--
void CMeasurementGraphDlg::DrawLightIntensityGraph_Init_Stress(int nLine,int nSection)
{
	CNanoMeasureGraph::GraphConfig tGraphConfig;
	CNanoMeasureGraph::LineConfig tLineConfig;
	memset(&tGraphConfig,NULL,sizeof(tGraphConfig));
	tGraphConfig.UseLineSelect = TRUE;
	strcpy(tGraphConfig.UpperTitle,"STRESS GRAPH");
	strcpy(tGraphConfig.LowerTitle,"");
	tGraphConfig.CanvasMargin = 7;	//ï¿½Oï¿½ï¿½ï¿½tï¿½`ï¿½ï¿½]ï¿½ï¿½

	tGraphConfig.PointNum = 100;	//ï¿½ï¿½ï¿½Ì’lï¿½Í�bï¿½ï¿½iï¿½ï¿½ï¿½Û‚Ì•\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å’lï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½j
	tGraphConfig.CanvasBackColor = WHITE_COLOR;
	tGraphConfig.GraphBackColor = WHITE_COLOR;//RGB(0x00,0xAA,0xAA);
	tGraphConfig.GraphBorderColor = RGB(0x00,0x00,0x00);
	tGraphConfig.GraphStepLineColor = RGB(0xAA,0xAA,0xAA);
	tGraphConfig.TitleTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.UnitTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.SelectingCalibrationColor= RED_COLOR;
	tGraphConfig.SelectedCalibrationColor = BLUE_COLOR;
	long lMax = -999999;
	long lMin = 999999;
	int n;

	STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;

	lMax = stressStage.Line[nLine].SectPos[nSection].lScanEndPosX;
	lMin = stressStage.Line[nLine].SectPos[nSection].lScanStartPosX;
	switch(GetCurrentTab()){
	case 0:
		DrawLightIntensityGraph_Init_SR();
		break;
	case 1:
		for(n = 0;n < STRESS_SECTIONS_MAX;n++){
//2009.10.13 bagus stress --{--
#if 0
			if(lMax < stressStage.Line[nLine].SectPos[nSection].lScanEndPosX){
				lMax = stressStage.Line[nLine].SectPos[nSection].lScanEndPosX;
			}
			if(lMin > stressStage.Line[nLine].SectPos[nSection].lScanEndPosX){
				lMin = stressStage.Line[nLine].SectPos[nSection].lScanEndPosX;
			}
			if(lMax < stressStage.Line[nLine].SectPos[nSection].lScanStartPosX){
				lMax = stressStage.Line[nLine].SectPos[nSection].lScanStartPosX;
			}
			if(lMin > stressStage.Line[nLine].SectPos[nSection].lScanStartPosX){
				lMin = stressStage.Line[nLine].SectPos[nSection].lScanStartPosX;
			}
#else
			if(!stressStage.Line[nLine].bScanValid[n]){
				continue;
			}
			if(lMax < stressStage.Line[nLine].SectPos[n].lScanEndPosX){
				lMax = stressStage.Line[nLine].SectPos[n].lScanEndPosX;
			}
			if(lMin > stressStage.Line[nLine].SectPos[n].lScanEndPosX){
				lMin = stressStage.Line[nLine].SectPos[n].lScanEndPosX;
			}
			if(lMax < stressStage.Line[nLine].SectPos[n].lScanStartPosX){
				lMax = stressStage.Line[nLine].SectPos[n].lScanStartPosX;
			}
			if(lMin > stressStage.Line[nLine].SectPos[n].lScanStartPosX){
				lMin = stressStage.Line[nLine].SectPos[n].lScanStartPosX;
			}
#endif
//2009.10.13 bagus stress --}--
		}
	case 2:
		strcpy(tGraphConfig.X_AxisTitle,"Location [mm]");
		strcpy(tGraphConfig.Y_AxisTitle,"LS Line [mm]");
		if(nLine < 0 || nSection < 0){
			//ï¿½ï¿½ï¿½ï¿½ï¿½Å‚Í�bï¿½ï¿½Iï¿½È’lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½iLSï¿½É‚ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ì‚Å�j
			tGraphConfig.X_Max = 10000;
			tGraphConfig.Y_Max = 10000;
			tGraphConfig.X_Origin = 0;
			tGraphConfig.Y_Origin = 0;
			tGraphConfig.X_Step = 50000;
		}else{
			tGraphConfig.X_Max = static_cast<long>(lMax / (double)MICROMETRE);
			tGraphConfig.Y_Max = static_cast<long>(MAX_GRAPH_DEFL_Y);
			tGraphConfig.X_Origin = static_cast<long>(lMin / (double)MICROMETRE);
			tGraphConfig.Y_Origin = static_cast<long>(MIN_GRAPH_DEFL_Y);
			//tGraphConfig.X_Step = 50000;
			tGraphConfig.X_Step = (tGraphConfig.X_Max - tGraphConfig.X_Origin) / 5;
			if(tGraphConfig.X_Step > 20){
				tGraphConfig.X_Step = tGraphConfig.X_Step - (tGraphConfig.X_Step % 10);
			}
			tGraphConfig.Y_Step = static_cast<long>((MAX_GRAPH_DEFL_Y - MIN_GRAPH_DEFL_Y) / 10);
		}
		m_MeasureGraph.SetGraphConfig(tGraphConfig);
		break;
	case 3:
		strcpy(tGraphConfig.X_AxisTitle,"Location [mm]");
		strcpy(tGraphConfig.Y_AxisTitle,"Stress [MPa]");
		//ï¿½ï¿½ï¿½ï¿½ï¿½Å‚Í�bï¿½ï¿½Iï¿½È’lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½iLSï¿½É‚ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ì‚Å�j
		if(nLine < 0 || nSection < 0){
			//ï¿½ï¿½ï¿½ï¿½ï¿½Å‚Í�bï¿½ï¿½Iï¿½È’lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½iLSï¿½É‚ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ì‚Å�j
			tGraphConfig.X_Max = 10000;
			tGraphConfig.Y_Max = 10000;
			tGraphConfig.X_Origin = 0;
			tGraphConfig.Y_Origin = 0;
			tGraphConfig.X_Step = 50;
		}else{
		tGraphConfig.X_Max = static_cast<long>(stressStage.Line[nLine].SectPos[nSection].lScanEndPosX / (double)MICROMETRE);
		tGraphConfig.Y_Max = static_cast<long>(MAX_GRAPH_STRESS_Y);		//@@@
			tGraphConfig.X_Origin = static_cast<long>(stressStage.Line[nLine].SectPos[nSection].lScanStartPosX / (double)MICROMETRE);
			tGraphConfig.Y_Origin = static_cast<long>(MIN_GRAPH_STRESS_Y);
			tGraphConfig.X_Step = (tGraphConfig.X_Max - tGraphConfig.X_Origin) / 5;
			if(tGraphConfig.X_Step > 20){
				tGraphConfig.X_Step = tGraphConfig.X_Step - (tGraphConfig.X_Step % 10);
			}
			tGraphConfig.Y_Step = static_cast<long>((MAX_GRAPH_STRESS_Y - MIN_GRAPH_STRESS_Y) / 10);
		}
		m_MeasureGraph.SetGraphConfig(tGraphConfig);
		break;
	}



	////SetLine Condition ////
	for(int i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
		memset(&tLineConfig,NULL,sizeof(tLineConfig));
		tLineConfig.DataIndex = i;
		switch(GetCurrentTab()){
		case 0:
			switch(i){
			case 0:
				tLineConfig.UseFlag = TRUE;
				tLineConfig.Visible = TRUE;
				tLineConfig.Color = BLUE_COLOR;
				tLineConfig.PenStyle = PS_SOLID;
				wsprintf(tLineConfig.LineName,"Measured Data ");
				break;
			case 1:
				tLineConfig.UseFlag = TRUE;
				tLineConfig.Visible = TRUE;
				tLineConfig.PenStyle = PS_SOLID;
				tLineConfig.Color = RED_COLOR;
				wsprintf(tLineConfig.LineName,"Model Data ");
				break;
			case 2:
				tLineConfig.UseFlag = TRUE;
				tLineConfig.Visible = TRUE;
				tLineConfig.PenStyle = PS_SOLID;
				tLineConfig.Color = GREEN_COLOR;
				wsprintf(tLineConfig.LineName,"Fit Data ");
				break;
			default:
				tLineConfig.UseFlag = FALSE;
				tLineConfig.Visible = FALSE;
			}
			break;
		case 1:
		case 2:
			switch(i){
			case 0:
				tLineConfig.UseFlag = TRUE;
				tLineConfig.Visible = TRUE;
				tLineConfig.PenStyle = PS_SOLID;
				tLineConfig.Color = BLUE_COLOR;
// 2009.10.09 K.Matsuo -->
//				wsprintf(tLineConfig.LineName,"Measured Data ");
				wsprintf(tLineConfig.LineName,"Reference Data ");
// 2009.10.09 K.Matsuo <--
				break;
			case 1:
				tLineConfig.UseFlag = TRUE;
				tLineConfig.Visible = TRUE;
				tLineConfig.PenStyle = PS_SOLID;
				tLineConfig.Color = RED_COLOR;
// 2009.10.09 K.Matsuo -->
//				wsprintf(tLineConfig.LineName,"Reference Data ");
				wsprintf(tLineConfig.LineName,"Measured Data ");
// 2009.10.09 K.Matsuo <--
				break;
			case 2:
				tLineConfig.UseFlag = TRUE;
// 2009.10.09 K.Matsuo -->
// ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Eï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½Í�Aï¿½`ï¿½Fï¿½bï¿½Nï¿½{ï¿½bï¿½Nï¿½Xï¿½Éƒ`ï¿½Fï¿½bï¿½Nï¿½È‚ï¿½
//				tLineConfig.Visible = TRUE;
				if ( m_pDoc->GetStoreCount() > 0 )
					tLineConfig.Visible = TRUE;
// 2009.10.09 K.Matsuo <--
				tLineConfig.PenStyle = PS_SOLID;
				tLineConfig.Color = GREEN_COLOR;
				wsprintf(tLineConfig.LineName,"Difference Data ");
				break;
			default:
				tLineConfig.UseFlag = FALSE;
				tLineConfig.Visible = FALSE;
			}
			break;
		case 3:
			switch(i){
			case 0:
				tLineConfig.UseFlag = TRUE;
				tLineConfig.Visible = TRUE;
				tLineConfig.PenStyle = PS_SOLID;
				tLineConfig.Color = BLUE_COLOR;
				wsprintf(tLineConfig.LineName,"Stress Data ");
				break;
			default:
				tLineConfig.UseFlag = FALSE;
				tLineConfig.Visible = FALSE;
			}
			break;
		}
		m_MeasureGraph.SetLineConfig(i,tLineConfig);
		m_MeasureGraph.ClearGraphData();
	}
}

// =========================================================================
//
void CMeasurementGraphDlg::AddLightIntensityListAndGraph(int iPointIndex)
{
	double dAnaData, dGenData, dFitData;
	double dDarkT1Data, d1stRefData, dSampData;
	CString strBuffer;

	char szDecimalpoint[255+1];
	memset(szDecimalpoint, 0, sizeof(szDecimalpoint));
	sprintf(szDecimalpoint, "%s%s%s", "%", g_lpszDecimalPoint[2], "f");

	m_iDispPointNo = iPointIndex;

	::ZeroMemory(m_dAnalysisData, sizeof(double) * SCANDATA_POINT_MAX);
	::ZeroMemory(m_dAnalysisGenData, sizeof(double) * SCANDATA_POINT_MAX);

	//2009.09.10 bagus stress --{--
	//if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_STRESS){
		LightIntensityListGrid_Init();
		DrawLightIntensityGraph_Init();
	//}
	//2009.09.10 bagus stress --}--

	//// Graph Data All Clear ////
	m_MeasureGraph.ClearGraphData();

	//// GenData and SetData ////
	if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) { //RAWï¿½fï¿½[ï¿½^
		double dWave = (double)m_iStartWaveLength;
		while(dWave <= m_iEndWaveLength){
			//2010.01.11 bagus GTR --{--
			if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
				///// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
				d1stRefData = m_pDoc->NanoSpecDoc_GetScanSampleData(iPointIndex, dWave);
				strBuffer.Format(szDecimalpoint, d1stRefData);
				m_MeasureGraph.AddLineData(0, (double)atof(strBuffer));

				///// ï¿½_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
				//dDarkT1Data = m_pDoc->NanoSpecDoc_GetScanDark_T1Data(iPointIndex, dWave);
				dDarkT1Data = MEAS_GetScanGTrReferenceData(0, dWave);
				strBuffer.Format(szDecimalpoint, dDarkT1Data);
				m_MeasureGraph.AddLineData(1, (double)atof(strBuffer));
			}
			else{
				///// ï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½ï¿½fï¿½[ï¿½^ /////
				dSampData = m_pDoc->NanoSpecDoc_GetScanSampleData(iPointIndex, dWave);
				strBuffer.Format(szDecimalpoint, dSampData);
				m_MeasureGraph.AddLineData(0, (double)atof(strBuffer));

				///// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
				d1stRefData = m_pDoc->NanoSpecDoc_GetScan1stReferenceData(iPointIndex, dWave);
				strBuffer.Format(szDecimalpoint, d1stRefData);
				m_MeasureGraph.AddLineData(1, (double)atof(strBuffer));

				///// ï¿½_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
				dDarkT1Data = m_pDoc->NanoSpecDoc_GetScanDark_T1Data(iPointIndex, dWave);
				strBuffer.Format(szDecimalpoint, dDarkT1Data);
				m_MeasureGraph.AddLineData(2, (double)atof(strBuffer));
			}
			//2010.01.11 bagus GTR --}--

			switch(m_SrConfig.nWavelengthStep){
			case SR_WAVELENGTH_STEP_05_NM:
				dWave += 0.5;
				break;
			case SR_WAVELENGTH_STEP_1_NM:
			default:
				dWave += 1.0;
				break;
			}
		}
	}
	else {
// 2009.09.30 K.Matsuo Add -->
		char szBuff[256];
		LPTSTR token;
// 2014.04.03 bagus wavelength step modified -->
//		int iWaveLength;
		double dWaveLen;
		int iIndex;
// 2014.04.03 bagus wavelength step modified <--

		SR_XMP SrXmp;
		ConfigFile_GetNanoSpecIni(&SrXmp, CONFIG_FILE_SR_XMP);

		//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½pï¿½Xï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½æ“¾
		char szXmpDataFileName[_MAX_FNAME]; 		// Dataï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½
		char szXmpDataExt[_MAX_EXT];				// Dataï¿½gï¿½ï¿½ï¿½q
		char szDataFilePath[MAX_PATH];				// ï¿½Rï¿½sï¿½[ï¿½ï¿½ï¿½Dataï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½iï¿½Aï¿½Ô•tï¿½ï¿½ï¿½j
		char szXmpFittingDataFileName[_MAX_FNAME]; 	// FittingDataï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½
		char szXmpFittingDataExt[_MAX_EXT]; 		// FittingDataï¿½gï¿½ï¿½ï¿½q
		char szFittingDataFilePath[MAX_PATH];		// ï¿½Rï¿½sï¿½[ï¿½ï¿½ï¿½FittingDataï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½iï¿½Aï¿½Ô•tï¿½ï¿½ï¿½j

		_splitpath(SrXmp.szDataPath, NULL, NULL, szXmpDataFileName, szXmpDataExt);
		_splitpath(SrXmp.szFittingDataPath, NULL, NULL, szXmpFittingDataFileName, szXmpFittingDataExt);

		//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½É‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ô�ï¿½ï¿½ï¿½Ç‰ï¿½
		sprintf(szDataFilePath, "%s%s_%05d%s", g_szData_Meas_Dat_Temp_Copy_Dir, szXmpDataFileName, iPointIndex, szXmpDataExt);
		sprintf(szFittingDataFilePath, "%s%s_%05d%s", g_szData_Meas_Dat_Temp_Copy_Dir, szXmpFittingDataFileName, iPointIndex, szXmpFittingDataExt);

		TRY
		{
			CStdioFile stdioFile(szDataFilePath, CFile::modeRead);
			//ï¿½Å�ï¿½ï¿½Ìƒwï¿½bï¿½_ï¿½ÍƒXï¿½Lï¿½bï¿½vï¿½ï¿½ï¿½ï¿½
			stdioFile.ReadString(strBuffer);
			stdioFile.ReadString(strBuffer);

			while ( stdioFile.ReadString(strBuffer) ) {
				strcpy(szBuff, strBuffer);
				token = strtok(szBuff, " \t\n");			// uR
				token = strtok(NULL, " \t\n");				// WaveLength (ex. 380nm)
// 2014.04.03 bagus wavelength step modified -->
//				iWaveLength = atoi(token);
				switch(m_SrConfig.nWavelengthStep){
				case SR_WAVELENGTH_STEP_05_NM:
					dWaveLen = atof(token);
					iIndex = (int)((dWaveLen - (double)m_iStartWaveLength)*2);
					break;
				case SR_WAVELENGTH_STEP_1_NM:
				default:
					iIndex = atoi(token) - m_iStartWaveLength;
					break;
				}
// 2014.04.03 bagus wavelength step modified <--
				token = strtok(NULL, " \t\n");				// XMP angle
				token = strtok(NULL, " \t\n");				// data
// 2014.04.03 bagus wavelength step modified -->
//				m_dAnalysisData[iWaveLength - 1] = atof(token);
				if(iIndex >= 0){
					m_dAnalysisData[iIndex] = atof(token);
				}
// 2014.04.03 bagus wavelength step modified <--
			}

			stdioFile.Close();
		}
		CATCH(CFileException, e)
		{
		}
		END_CATCH

		if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)	{
			TRY
			{
				CStdioFile stdioFile(szFittingDataFilePath, CFile::modeRead);
				//ï¿½Å�ï¿½ï¿½Ìƒwï¿½bï¿½_ï¿½ÍƒXï¿½Lï¿½bï¿½vï¿½ï¿½ï¿½ï¿½
				stdioFile.ReadString(strBuffer);
				stdioFile.ReadString(strBuffer);
				for ( int i = 0; i < 2; i++ ) {
					if ( strBuffer == _T("nm") || strBuffer == _T("1/cm") )
						break;
					stdioFile.ReadString(strBuffer);
				}

				while ( stdioFile.ReadString(strBuffer) ) {
					strcpy(szBuff, strBuffer);
					token = strtok(szBuff, " \t\n");			// uR
					token = strtok(NULL, " \t\n");				// WaveLength (ex. 380nm)
// 2014.04.03 bagus wavelength step modified -->
//					iWaveLength = atoi(token);
					switch(m_SrConfig.nWavelengthStep){
					case SR_WAVELENGTH_STEP_05_NM:
						dWaveLen = atof(token);
						iIndex = (int)((dWaveLen - (double)m_iStartWaveLength)*2);
						break;
					case SR_WAVELENGTH_STEP_1_NM:
					default:
						iIndex = atoi(token) - m_iStartWaveLength;
						break;
					}
// 2014.04.03 bagus wavelength step modified <--
					token = strtok(NULL, " \t\n");				// XMP angle?
					token = strtok(NULL, " \t\n");				// data
// 2014.04.03 bagus wavelength step modified -->
//					m_dAnalysisGenData[iWaveLength - 1] = atof(token);
					if(iIndex >= 0){
						m_dAnalysisGenData[iIndex] = atof(token);
					}
// 2014.04.03 bagus wavelength step modified <--
				}

				stdioFile.Close();
			}
			CATCH(CFileException, e)
			{
			}
			END_CATCH
		}

// 2014.04.03 bagus wavelength step added -->
		if(m_SrConfig.nWavelengthStep != SR_WAVELENGTH_STEP_05_NM){
// 2014.04.03 bagus wavelength step added <--
// 2014.04.03 bagus wavelength step modified -->
//			for(int iWave = m_iStartWaveLength; iWave <= m_iEndWaveLength; iWave++){
			int iIndex;
			for(int iWave = m_iStartWaveLength; iWave <= m_iEndWaveLength; iWave++){
				iIndex = iWave - m_iStartWaveLength;
// 2014.04.03 bagus wavelength step modified <--
				///// Set Measured Data Value at Cell /////
				dAnaData = m_dAnalysisData[iIndex] * 100;
				strBuffer.Format(szDecimalpoint, dAnaData);
				m_MeasureGraph.AddLineData(0, (double)atof(strBuffer));

				if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
				{
					///// Set XMP Gen Data Value at Cell /////
					dGenData = m_dAnalysisGenData[iIndex] * 100;
					strBuffer.Format(szDecimalpoint, dGenData);
					m_MeasureGraph.AddLineData(1, (double)atof(strBuffer));

					///// Set Fit Data((Measured Data)-(Gen Data)) Graph Data /////
					dFitData = fabs(dAnaData - dGenData);
					strBuffer.Format(szDecimalpoint, dFitData);
					m_MeasureGraph.AddLineData(2, (double)atof(strBuffer));
				}
			}
// 2014.04.03 bagus wavelength step added -->
		}
		else{
			int iIndex;
			for(int iWave = m_iStartWaveLength; iWave <= m_iEndWaveLength; iWave++){
				iIndex = (iWave - m_iStartWaveLength)*2;
				///// Set Measured Data Value at Cell /////
				dAnaData = m_dAnalysisData[iIndex] * 100;
				strBuffer.Format(szDecimalpoint, dAnaData);
				m_MeasureGraph.AddLineData(0, (double)atof(strBuffer));

				if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
				{
					///// Set XMP Gen Data Value at Cell /////
					dGenData = m_dAnalysisGenData[iIndex] * 100;
					strBuffer.Format(szDecimalpoint, dGenData);
					m_MeasureGraph.AddLineData(1, (double)atof(strBuffer));

					///// Set Fit Data((Measured Data)-(Gen Data)) Graph Data /////
					dFitData = fabs(dAnaData - dGenData);
					strBuffer.Format(szDecimalpoint, dFitData);
					m_MeasureGraph.AddLineData(2, (double)atof(strBuffer));
				}

				if(iWave == m_iEndWaveLength){
					break;
				}

				iIndex++;
				///// Set Measured Data Value at Cell /////
				dAnaData = m_dAnalysisData[iIndex] * 100;
				strBuffer.Format(szDecimalpoint, dAnaData);
				m_MeasureGraph.AddLineData(0, (double)atof(strBuffer));

				if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
				{
					///// Set XMP Gen Data Value at Cell /////
					dGenData = m_dAnalysisGenData[iIndex] * 100;
					strBuffer.Format(szDecimalpoint, dGenData);
					m_MeasureGraph.AddLineData(1, (double)atof(strBuffer));

					///// Set Fit Data((Measured Data)-(Gen Data)) Graph Data /////
					dFitData = fabs(dAnaData - dGenData);
					strBuffer.Format(szDecimalpoint, dFitData);
					m_MeasureGraph.AddLineData(2, (double)atof(strBuffer));
				}
			}
		}
// 2014.04.03 bagus wavelength step added <--
// 2009.09.30 K.Matsuo Add <--
	}

	m_MeasureGraph.Invalidate();
}


// =========================================================================
//
void CMeasurementGraphDlg::AddLightIntensityListAndGraph_Ref(int iPointIndex)
{
	double dDarkT1Data, d1stRefData/*, dSampData*/;
	CString strBuffer;

	char szDecimalpoint[255+1];
	memset(szDecimalpoint, 0, sizeof(szDecimalpoint));
	sprintf(szDecimalpoint, "%s%s%s", "%", g_lpszDecimalPoint[2], "f");

	m_iDispPointNo = iPointIndex;

	//// Graph Data All Clear ////
	m_MeasureGraph.ClearGraphData();

	//// GenData and SetData ////
// 2014.10.16 bagus wavelength step modified -->
	double dWave = (double)m_iStartWaveLength;
	while(dWave <= m_iEndWaveLength){
		if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) //RAWï¿½fï¿½[ï¿½^
		{
			///// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
			d1stRefData = m_pDoc->NanoSpecDoc_GetScan1stReferenceData(iPointIndex, dWave);
			strBuffer.Format(szDecimalpoint, d1stRefData);
			m_MeasureGraph.AddLineData(1, (double)atof(strBuffer));

			///// ï¿½_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
			dDarkT1Data = m_pDoc->NanoSpecDoc_GetScanDark_T1Data(iPointIndex, dWave);
			strBuffer.Format(szDecimalpoint, dDarkT1Data);
			m_MeasureGraph.AddLineData(2, (double)atof(strBuffer));
		}

		switch(m_SrConfig.nWavelengthStep){
		case SR_WAVELENGTH_STEP_05_NM:
			dWave += 0.5;
			break;
		case SR_WAVELENGTH_STEP_1_NM:
		default:
			dWave += 1.0;
			break;
		}
	}
// 2014.10.16 bagus wavelength step modified <--

	m_LightIntensityListGrid.Invalidate();	// 2014.10.16 bug fix.
	m_MeasureGraph.Invalidate();
}


//2009.09.15 bagus se --{--
void CMeasurementGraphDlg::AddPsiAndDeltaListAndGraph(int iPointIndex)
{
	CSERawDataList l_RawDataList;

	m_iDispPointNo = iPointIndex;

	::ZeroMemory(m_seRawData, sizeof(SE_RAW_ONE_DATA) * SCANDATA_POINT_MAX);

	LightIntensityListGrid_Init();
	DrawLightIntensityGraph_Init();

	//// Graph Data All Clear ////
	m_MeasureGraph.ClearGraphData();

	if(!m_pMainFrame->LoadSERawDataByPointNo(iPointIndex, &l_RawDataList)){
		//ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Ç‚İ�ï¿½ï¿½ß‚È‚ï¿½
		return ;
	}


	int nDataNum = l_RawDataList.GetCount();
	BOOL bPsiData = GetCurrentTab() == 0 ? TRUE : FALSE;

	//ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½È‚ï¿½ï¿½Ì‚Å‚È‚É‚ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(nDataNum == 0)
		return;

	int n;
	double dWaveLength;
	double dMeasured;
	double dModel;
	double dFit;

	CString strBuffer;
	double *aryMapKeys = new double[nDataNum];
	SE_RAW_ONE_DATA tRawData;
	int nCount = 0;;

	//CMapï¿½ÌƒLï¿½[ï¿½Ì�ï¿½ï¿½ï¿½ï¿½Í•sï¿½ï¿½È‚Ì‚ï¿½WaveLengthï¿½Ì�ï¿½ï¿½ï¿½ï¿½É•ï¿½ï¿½Ñ‘Ö‚ï¿½ï¿½ï¿½
	POSITION pos = l_RawDataList.GetStartPosition();
	if(pos == NULL)
		return;
	l_RawDataList.GetNextAssoc(pos,dWaveLength,tRawData);
	while(pos != NULL){
		aryMapKeys[nCount] = dWaveLength;
		for(int n = nCount;n > 0;n--){
			if(aryMapKeys[n-1] < aryMapKeys[n]){
				break;
			}else{
				double t = aryMapKeys[n-1];
				aryMapKeys[n-1] = aryMapKeys[n];
				aryMapKeys[n] = t;
			}
		}

		nCount++;
		l_RawDataList.GetNextAssoc(pos,dWaveLength,tRawData);
	}

	nDataNum = 0;
	m_LightIntensityListGrid.SetRowCount(nCount+1);
	m_MeasureGraph.ClearGraphData();
	for(n = 0;n < nCount;n++){
		dWaveLength = aryMapKeys[n];
		if(!l_RawDataList.Lookup(dWaveLength,tRawData)){
			//ï¿½ï¿½ï¿½ï¿½ï¿½É‚Í‚ï¿½ï¿½È‚ï¿½ï¿½Í‚ï¿½
			break;
		}
		if(dWaveLength < m_iStartWaveLength || dWaveLength > m_iEndWaveLength){
			//ï¿½ï¿½ï¿½Vï¿½sï¿½É�İ’è‚µï¿½ï¿½ï¿½ÍˆÍ‚Ì‚İ‚Ìƒfï¿½[ï¿½^ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			continue;
		}

		//// Graph ï¿½fï¿½[ï¿½^
		dMeasured = bPsiData ? tRawData.dPsi : tRawData.dDelta;
		dModel = bPsiData ? tRawData.dPsiModel : tRawData.dDeltaModel;

		//Measured Data
		m_MeasureGraph.AddLineData(0,dWaveLength,dMeasured);

		//Model Data
		m_MeasureGraph.AddLineData(1,dWaveLength,dModel);

		//Fit Data
		dFit = fabs(dMeasured - dModel);
		m_MeasureGraph.AddLineData(2,dWaveLength,dFit);

//		////ï¿½Oï¿½ï¿½ï¿½bï¿½hï¿½fï¿½[ï¿½^
//		/// Wave Length[nm]
//		dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
//		strBuffer.Format("%.1f",dWaveLength);
//		m_LightIntensityListGrid.SetItemText(nDataNum+1, 0, strBuffer);
//		m_LightIntensityListGrid.SetItemFormat(nDataNum+1, 0, dwTextStyle);
//		/// Measured Data
//		dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
//		strBuffer.Format("%.3f",dMeasured);
//		m_LightIntensityListGrid.SetItemText(nDataNum+1, 1, strBuffer);
//		m_LightIntensityListGrid.SetItemFormat(nDataNum+1, 1, dwTextStyle);
//		/// Model Data
//		strBuffer.Format("%.3f",dModel);
//		m_LightIntensityListGrid.SetItemText(nDataNum+1, 2, strBuffer);
//		m_LightIntensityListGrid.SetItemFormat(nDataNum+1, 2, dwTextStyle);
//		/// Fit Data
//		strBuffer.Format("%.3f",dFit);
//		m_LightIntensityListGrid.SetItemText(nDataNum+1, 3, strBuffer);
//		m_LightIntensityListGrid.SetItemFormat(nDataNum+1, 3, dwTextStyle);

		m_seRawData[nDataNum] = tRawData;

		nDataNum++;
	}

	m_LightIntensityListGrid.SetRowCount(nDataNum+1);

	CNanoMeasureGraph::GraphConfig tConfig;
	m_MeasureGraph.GetGraphConfig(&tConfig);
	tConfig.PointNum = nDataNum;
	m_MeasureGraph.SetGraphConfig(tConfig);

	delete [] aryMapKeys;
}
//2009.09.15 bagus se --}--

// =========================================================================
//
void CMeasurementGraphDlg::OnTimer(UINT nIDEvent)
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½Éƒï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚©ï¿½Ü‚ï¿½ï¿½Íƒfï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	if( m_pDoc->GetLastMeasPoint() == m_pDoc->GetShowCompPointGraph() ) //ï¿½ï¿½ [ï¿½ï¿½ï¿½ï¿½]ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½Wï¿½bï¿½Nï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Í�Aï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½æ‚¤ï¿½É�Cï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½H
	{
		//2009.08.31 bagus stess --{--
		//ï¿½vï¿½ï¿½ï¿½Zï¿½bï¿½Vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½Å‚ï¿½ï¿½ï¿½Î‚Ü‚ï¿½ï¿½^ï¿½Cï¿½}ï¿½ï¿½ï¿½~ï¿½ß‚È‚ï¿½ï¿½iï¿½ï¿½ï¿½ï¿½ï¿½Å�~ï¿½ß‚ï¿½ÆƒXï¿½gï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ÌŒï¿½ï¿½Ê‚ï¿½ï¿½\ï¿½ï¿½ï¿½Å‚ï¿½ï¿½È‚ï¿½ï¿½j
		if(m_pDoc->GetProcessStatus() != PROCESS_PROC){
			KillTimer(SHOW_GRAPH_TIMER);
			MeasurementListGrid_UnSelectAll();
		}
		//KillTimer(SHOW_LIST_TIMER);
		//MeasurementListGrid_UnSelectAll();
		//2009.08.31 bagus stess --}--
	}
	SetMeasurementData2();
	//2009.08.31 bagus stress --{--
	if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		SetMeasurementLSData(m_pDoc->GetStoreCount());
	}
	//2009.08.31 bagus stress --}--

	CDialog::OnTimer(nIDEvent);
}


// =========================================================================
// m_MeasurementListGridï¿½Ì‘Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½ï¿½Sï¿½Ä‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
void CMeasurementGraphDlg::MeasurementListGrid_UnSelectAll()
{
	int iRowCount = m_MeasurementListGrid.GetRowCount();
	int iColumnCount = m_MeasurementListGrid.GetColumnCount();

	for(int iRow=1; iRow<iRowCount; iRow++)
	{
		for(int iCol=1; iCol<iColumnCount; iCol++)
		{
			//ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			int iState = m_MeasurementListGrid.GetItemState(iRow, iCol);
			iState = (iState & GVIS_SELECTED) ^ iState;
			BOOL bRtnChk = m_MeasurementListGrid.SetItemState(iRow, iCol, iState);
			m_MeasurementListGrid.Invalidate();
		}
	}
}
void CMeasurementGraphDlg::Tab_Init()
{
	WORD& wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType;
	CString strBuffer;

	switch(wHeadType){
	case HEAD_TYPE_SR:
	default:
		m_MeasureGraphTab.InsertItem(0, "Thick");
		break;
	case HEAD_TYPE_STRESS:
		m_MeasureGraphTab.InsertItem(0, "Thick");
		m_MeasureGraphTab.InsertItem(1, "Line");
		m_MeasureGraphTab.InsertItem(2, "LS Line");
// 2009.10.22 bagus Stress ï¿½Cï¿½ï¿½ --{--
//		m_MeasureGraphTab.InsertItem(3, "LS Stress");
// 2009.10.22 bagus Stress ï¿½Cï¿½ï¿½ --}--
		break;
	case HEAD_TYPE_SE:
		m_MeasureGraphTab.InsertItem(0, "Psi");
		m_MeasureGraphTab.InsertItem(1, "Delta");
		break;
	}
}

void CMeasurementGraphDlg::TabChange(int Index)
{
	CCellID celSel = m_MeasurementListGrid.GetFocusCell();
	//2009.10.30 bagus se --{--
	if(celSel.row < 0){
		celSel.row = m_nGraphRow;
		celSel.col = 0;
	}
	//2009.10.30 bagus se --}--
	NM_GRIDVIEW dDummy;

	dDummy.iColumn = celSel.col;
	dDummy.iRow = celSel.row;
	OnSelChangeing((NMHDR *)&dDummy, NULL);
}

void CMeasurementGraphDlg::OnSelchangeMeasureGraphTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index = m_MeasureGraphTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

//2009.08.25 bagus stess --{--
void CMeasurementGraphDlg::AddStressDataListAndGraph(int nLine,int nSection,int nType)
{
	CString strBuffer;

	char szDecimalpoint[255+1];
	memset(szDecimalpoint, 0, sizeof(szDecimalpoint));
	sprintf(szDecimalpoint, "%s%s%s", "%", g_lpszDecimalPoint[2], "f");

	m_nDispLine = nLine;
	m_nDispSection = nSection;

	STRESSRESULT tResult;
	STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;

	//// Graph Data All Clear ////
	m_MeasureGraph.ClearGraphData();
	if((nLine < 0 || nLine >= STRESS_LINES_MAX)
	|| (nSection < 0 || nSection >= STRESS_SECTIONS_MAX)){
		return;
	}
	if(!stressStage.Line[nLine].bValidLine){
		return;
	}
	if(!stressStage.Line[nLine].bScanValid[nSection]){
		return;
	}

	m_pDoc->GetOneStressLineData(&tResult,nLine+1);
	double dGridData[4];
	int nGridDataNum;
	long lScanStartPosX = stressStage.Line[nLine].SectPos[nSection].lScanStartPosX;
	long lScanEndPosX = stressStage.Line[nLine].SectPos[nSection].lScanEndPosX;

	//// GenData and SetData ////
	BOOL bStressMeas = !m_pDoc->GetOneStressLineDataRef(nLine);
	BOOL bFirstData = FALSE;
	int nDataIndex;
	int i;
	long lIndex = 0;
	int nValidPointNum = 0;

	m_nDataIndex = 0;

	//ï¿½ï¿½Uï¿½Nï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½
	//m_LightIntensityListGrid.SetRowCount(1);
	switch(GetCurrentTab()){
	case 0:		//Thick
		//ï¿½È‚É‚ï¿½ï¿½ï¿½ï¿½È‚ï¿½
		nGridDataNum = 0;
		break;
	case 1:		//Lineï¿½Ïˆï¿½ DeflectionRaw Lineï¿½Sï¿½ï¿½
		if(tResult.nDeflectionOccurence[0] > 0){
			lScanStartPosX = static_cast<long>(tResult.DeflectionRaw[0][0].dPosum);
			lScanEndPosX = static_cast<long>(tResult.DeflectionRaw[0][tResult.nDeflectionOccurence[0]-1].dPosum);
		}else{
			lScanStartPosX = 0L;
			lScanEndPosX = 0L;
		}
		{
			CNanoMeasureGraph::GraphConfig tConfig;
			m_MeasureGraph.GetGraphConfig(&tConfig);
			tConfig.X_Origin = static_cast<long>(lScanStartPosX / (double)MICROMETRE);
			tConfig.X_Max = static_cast<long>(lScanEndPosX / (double)MICROMETRE);
			m_MeasureGraph.SetGraphConfig(tConfig);
		}
	case 2:		//Lineï¿½Ïˆï¿½ DeflectionRaw Sectionï¿½Pï¿½ï¿½
		for(nDataIndex = 0;nDataIndex < tResult.nDeflectionOccurence[0];nDataIndex++){
			if(lScanStartPosX <=  tResult.DeflectionRaw[0][nDataIndex].dPosum
			&& tResult.DeflectionRaw[0][nDataIndex].dPosum <= lScanEndPosX ){
				if ( !bFirstData ) {
					m_nDataIndex = nDataIndex;
					bFirstData = TRUE;
				}

				dGridData[0] = tResult.DeflectionRaw[0][nDataIndex].dPosum;
//2009.09.19 bagus stress --{--
//				if(bStressMeas){
					//ï¿½ï¿½ï¿½Í‘ï¿½ï¿½ï¿½
					dGridData[1] = tResult.DeflectionRaw[0][nDataIndex].dDefl;	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					dGridData[2] = tResult.DeflectionRaw[1][nDataIndex].dDefl;	//ï¿½ï¿½ï¿½ï¿½
					dGridData[3] = fabs(dGridData[1]-dGridData[2]);					//ï¿½ï¿½ï¿½ï¿½
					nGridDataNum = 4;
//				}else{
//					//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½
//					dGridData[1] = tResult.DeflectionRaw[bStressMeas][nDataIndex].dDefl;
//					nGridDataNum = 2;
//				}
//2009.09.19 bagus stress --}--
				//ï¿½fï¿½[ï¿½^ï¿½ÌŠiï¿½[
				for(i = 0;i < nGridDataNum;i++){
					if(i == 0){
						;
					}else{
						m_MeasureGraph.AddLineData(i-1, dGridData[i] / MICROMETRE);				// @@@
					}
				}
				nValidPointNum++;
				lIndex++;
			}
		}
		break;
	case 3:		//LSï¿½ï¿½ï¿½ï¿½StressRaw
		for(nDataIndex = 0;nDataIndex < tResult.nStressLineOccurrence;nDataIndex++){
			if(lScanStartPosX <=  tResult.StressRaw[nDataIndex].dPosum && tResult.StressRaw[nDataIndex].dPosum <= lScanEndPosX ){
				if ( !bFirstData ) {
					m_nDataIndex = nDataIndex;
					bFirstData = TRUE;
				}
				dGridData[0] = tResult.StressRaw[nDataIndex].dPosum;
				//ï¿½ï¿½ï¿½Í‘ï¿½ï¿½ï¿½
				dGridData[1] = tResult.StressRaw[nDataIndex].dStress;
				nGridDataNum = 2;
				//ï¿½fï¿½[ï¿½^ï¿½ÌŠiï¿½[
				for(i = 0;i < nGridDataNum;i++){
					if(i == 0){
						;
					}else{
						m_MeasureGraph.AddLineData(i-1, dGridData[i] / MICROMETRE);				// @@@
					}
				}
				nValidPointNum++;
				lIndex++;
			}
		}
		break;
	default:
		nGridDataNum = 0;
	}
	//2009.09.10 bagus stress --{--
	{
		CNanoMeasureGraph::GraphConfig tConfig;
		m_MeasureGraph.GetGraphConfig(&tConfig);
		tConfig.PointNum = nValidPointNum;
		m_MeasureGraph.SetGraphConfig(tConfig);
	}
	if(nValidPointNum + 1 < DEF_LITENSITY_GRID_ROWLINE_COUNT){
		m_LightIntensityListGrid.SetRowCount(DEF_LITENSITY_GRID_ROWLINE_COUNT);
	}else{
		m_LightIntensityListGrid.SetRowCount(nValidPointNum + 1);
	}
	//2009.09.10 bagus stress --}--
	m_LightIntensityListGrid.Invalidate();
	m_MeasureGraph.Invalidate();

}
int CMeasurementGraphDlg::GetCurrentTab()
{
	return m_MeasureGraphTab.GetCurSel();
}
//2009.08.25 bagus stess --}--

//2009.09.03 bagus se ï¿½Iï¿½[ï¿½gï¿½Xï¿½Pï¿½[ï¿½ï¿½ --{--
void CMeasurementGraphDlg::OnGraphAutoScale()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	UpdateData(TRUE);

	m_MeasureGraph.SetAutoScaleMode(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale);

	m_MeasureGraph.Invalidate(TRUE);
}
//2009.09.03 bagus se ï¿½Iï¿½[ï¿½gï¿½Xï¿½Pï¿½[ï¿½ï¿½ --}--
//2009.09.19 bagus stress --{--
void CMeasurementGraphDlg::SetBlankLightIntensityListGrid()
{
	LightIntensityListGrid_Init();

}
//2009.09.19 bagus stress --}--

void CMeasurementGraphDlg::OnDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
//	typedef struct tagGV_DISPINFO {
//		NMHDR	hdr;
//		GV_ITEM item;
//	} GV_DISPINFO;

//typedef struct _GV_ITEM {
//	  int	   row,col; 	// Row and Column of item
//	  UINT	   mask;		// Mask for use in getting/setting cell data
//	  UINT	   nState;		// cell state (focus/hilighted etc)
//	  DWORD    nFormat; 	// Format of cell
//	  int	   iImage;		// index of the list view itemï¿½s icon
//	  COLORREF crBkClr; 	// Background colour (or CLR_DEFAULT)
//	  COLORREF crFgClr; 	// Forground colour (or CLR_DEFAULT)
//	  LPARAM   lParam;		// 32-bit value to associate with item
//	  LOGFONT  lfFont;		// Cell font
//	  UINT	   nMargin; 	// Internal cell margin
//	  CString  strText; 	// Text in cell
//} GV_ITEM;

	GV_DISPINFO* pDispInfo = (GV_DISPINFO*)pNMHDR;
	GV_ITEM* pItem= &pDispInfo->item;

	char szDecimalpoint[255+1];
	long lScanStartPosX;
	long lScanEndPosX;
	STRESSRESULT tResult;
	STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;

	int nDataIndex;

	// Format(Style)
	if ( pItem->mask & GVIF_FORMAT ) {
		if ( pItem->row == 0 ) {
			pItem->nFormat = DT_CENTER | DT_VCENTER | DT_WORDBREAK;
		}
		else {
			pItem->nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
		}
	}

	// Font
	if ( pItem->mask & GVIF_FONT ) {
		switch ( GetCurrentTab() ) {
		case 0:	// Thick
			if ( pItem->row == 0 ) {
				;
			}
			else {
				LOGFONT LogFont;
				//ï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Tï¿½Cï¿½Yï¿½Ï�X
				LogFont.lfHeight = 12;
				m_Font.GetLogFont(&LogFont);
				pItem->lfFont = LogFont;
			}
			break;
		case 1:
		case 2:
		case 3:
			if ( pItem->row == 0 ) {
				;
			}
			else {
				LOGFONT LogFont;
				m_Font.GetLogFont(&LogFont);
				//ï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Tï¿½Cï¿½Yï¿½Ï�X
				LogFont.lfHeight = 11;
				pItem->lfFont = LogFont;
				break;
			}
		}
	}

	// Text
	if ( pItem->mask & GVIF_TEXT ) {
		if ( pItem->row == 0 ) {
			// Header
			//2010.01.11 bagus GTR --{--
			//m_strHeader.GetHeader(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType, m_iGraphDlgMode, GetCurrentTab());
			m_strHeader.GetHeader(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType, m_iGraphDlgMode, GetCurrentTab(),m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType);
			//2010.01.11 bagus GTR --}--

			if ( pItem->col < m_strHeader.GetSize() ) {
				pItem->strText = m_strHeader[pItem->col];
			}
		}
		else {
			// Body
			if ( m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR ||
				 m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS ) {
				switch ( GetCurrentTab() ) {
				case 0:	// Thick
					sprintf(szDecimalpoint, "%s%s%s", "%", g_lpszDecimalPoint[2], "f");

// 2014.04.02 bagus wavelength step modified -->
//					if ( m_iDispPointNo > 0 ) {
//						if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) { //RAWï¿½fï¿½[ï¿½^
//
//							int iWave = m_iStartWaveLength + pItem->row - 1;
//							if ( iWave <= m_iEndWaveLength ) {
//								//2010.01.11 bagus GTR --{--
//								if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
//									switch ( pItem->col ) {
//									case 0:
//										///// Set Wavelength at Cell /////
//										pItem->strText.Format("%d", iWave);
//										break;
//									case 1:
//										///// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
//										//pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScan1stReferenceData(m_iDispPointNo, iWave));
//										pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScanSampleData(m_iDispPointNo, iWave));
//										break;
//									case 2:
//										///// ï¿½_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
//										//pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScan1stReferenceData(m_iDispPointNo, iWave));
//										pItem->strText.Format(szDecimalpoint, MEAS_GetScanGTrReferenceData(0, iWave));
//										//pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScanDark_T1Data(m_iDispPointNo, iWave));
//										break;
//									}
//								}else{
//									switch ( pItem->col ) {
//									case 0:
//										///// Set Wavelength at Cell /////
//										pItem->strText.Format("%d", iWave);
//										break;
//									case 1:
//										///// ï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½ï¿½fï¿½[ï¿½^ /////
//										if ( m_iModeStatus != 1/* TestMode Reference */ )
//											pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScanSampleData(m_iDispPointNo, iWave));
//										else
//											pItem->strText = "";
//										break;
//									case 2:
//										///// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
//										pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScan1stReferenceData(m_iDispPointNo, iWave));
//										break;
//									case 3:
//										///// ï¿½_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
//										pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScanDark_T1Data(m_iDispPointNo, iWave));
//										break;
//									}
//								}
//								//2010.01.11 bagus GTR --}--
//							}
//						}
//						else {
//							int iWave = m_iStartWaveLength + pItem->row - 1;
//							if ( iWave <= m_iEndWaveLength ) {
//								switch ( pItem->col ) {
//								case 0:
//									///// Set Wavelength at Cell /////
//									pItem->strText.Format("%d", iWave);
//									break;
//								case 1:
//									///// Set XMP Gen Data Value at Cell /////
//									if ( m_iModeStatus != 1/* TestMode Reference */ ) {
//										//2010.02.01 bagus ï¿½Cï¿½ï¿½ --{--
//										//ï¿½È‰ï¿½ï¿½Ì‚ï¿½ï¿½Ì‚ï¿½100ï¿½{ï¿½ï¿½ï¿½ï¿½%ï¿½\ï¿½Lï¿½Å�oï¿½ï¿½
//										switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType){
//										case MEAS_PROG_TYPE_SR_REFLECTANCE:
//										case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
//										case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
//										case MEAS_PROG_TYPE_SR_THICKNESS:
//											pItem->strText.Format(szDecimalpoint, m_dAnalysisData[iWave - 1] * 100.0);
//											break;
//										default:
//											pItem->strText.Format(szDecimalpoint, m_dAnalysisData[iWave - 1]);
//										}
//										//2010.02.01 bagus ï¿½Cï¿½ï¿½ --}--
//									}
//									else {
//										pItem->strText = "";
//									}
//									break;
//								case 2:
//									///// Set Fit Data((Measured Data)-(Gen Data)) Graph Data /////
//									if ( m_iModeStatus != 1/* TestMode Reference */ ) {
//										if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS) {
//											//2010.02.01 bagus ï¿½Cï¿½ï¿½ --{--
//											//ï¿½È‰ï¿½ï¿½Ì‚ï¿½ï¿½Ì‚ï¿½100ï¿½{ï¿½ï¿½ï¿½ï¿½%ï¿½\ï¿½Lï¿½Å�oï¿½ï¿½
//											switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType){
//											case MEAS_PROG_TYPE_SR_REFLECTANCE:
//											case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
//											case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
//											case MEAS_PROG_TYPE_SR_THICKNESS:
//												pItem->strText.Format(szDecimalpoint, m_dAnalysisGenData[iWave - 1] * 100.0);
//												break;
//											default:
//												pItem->strText.Format(szDecimalpoint, m_dAnalysisGenData[iWave - 1]);
//											}
//											//2010.02.01 bagus ï¿½Cï¿½ï¿½ --}--
//										}
//										else {
//											pItem->strText = "";
//										}
//									}
//									else {
//										pItem->strText = "";
//									}
//									break;
//								}
//							}
//						}
//					}
					if ( m_iDispPointNo > 0 ) {
						if(m_iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) { //RAWï¿½fï¿½[ï¿½^

							double dWave;
							switch(m_SrConfig.nWavelengthStep){
							case SR_WAVELENGTH_STEP_05_NM:
								dWave = (double)m_iStartWaveLength + (pItem->row - 1) * 0.5;
								break;
							case SR_WAVELENGTH_STEP_1_NM:
							default:
								dWave = (double)(m_iStartWaveLength + pItem->row - 1);
								break;
							}

							if ( dWave <= (double)m_iEndWaveLength ) {
								//2010.01.11 bagus GTR --{--
								if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
									switch ( pItem->col ) {
									case 0:
										///// Set Wavelength at Cell /////
										switch(m_SrConfig.nWavelengthStep){
										case SR_WAVELENGTH_STEP_05_NM:
											pItem->strText.Format("%.1f", dWave);
											break;
										case SR_WAVELENGTH_STEP_1_NM:
										default:
											pItem->strText.Format("%d", (int)dWave);
										}
										break;
									case 1:
										///// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
										pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScanSampleData(m_iDispPointNo, dWave));
										break;
									case 2:
										///// ï¿½_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
										pItem->strText.Format(szDecimalpoint, MEAS_GetScanGTrReferenceData(0, dWave));
										break;
									}
								}else{
									switch ( pItem->col ) {
									case 0:
										///// Set Wavelength at Cell /////
										switch(m_SrConfig.nWavelengthStep){
										case SR_WAVELENGTH_STEP_05_NM:
											pItem->strText.Format("%.1f", dWave);
											break;
										case SR_WAVELENGTH_STEP_1_NM:
										default:
											pItem->strText.Format("%d", (int)dWave);
										}
										break;
									case 1:
										///// ï¿½Tï¿½ï¿½ï¿½vï¿½ï¿½ï¿½fï¿½[ï¿½^ /////
										if ( m_iModeStatus != 1/* TestMode Reference */ )
											pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScanSampleData(m_iDispPointNo, dWave));
										else
											pItem->strText = "";
										break;
									case 2:
										///// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
										pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScan1stReferenceData(m_iDispPointNo, dWave));
										break;
									case 3:
										///// ï¿½_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X /////
										pItem->strText.Format(szDecimalpoint, m_pDoc->NanoSpecDoc_GetScanDark_T1Data(m_iDispPointNo, dWave));
										break;
									}
								}
								//2010.01.11 bagus GTR --}--
							}
						}
						else {
							double dWave;
							switch(m_SrConfig.nWavelengthStep){
							case SR_WAVELENGTH_STEP_05_NM:
								dWave = (double)m_iStartWaveLength + (pItem->row - 1) * 0.5;
								break;
							case SR_WAVELENGTH_STEP_1_NM:
							default:
								dWave = (double)(m_iStartWaveLength + pItem->row - 1);
								break;
							}

							int iIndex = pItem->row - 1;
							if ( dWave <= (double)m_iEndWaveLength ) {
								switch ( pItem->col ) {
								case 0:
									///// Set Wavelength at Cell /////
									switch(m_SrConfig.nWavelengthStep){
									case SR_WAVELENGTH_STEP_05_NM:
										pItem->strText.Format("%.1f", dWave);
										break;
									case SR_WAVELENGTH_STEP_1_NM:
									default:
										pItem->strText.Format("%d", (int)dWave);
									}
									break;
								case 1:
									///// Set XMP Gen Data Value at Cell /////
									if ( m_iModeStatus != 1/* TestMode Reference */ ) {
										//2010.02.01 bagus ï¿½Cï¿½ï¿½ --{--
										//ï¿½È‰ï¿½ï¿½Ì‚ï¿½ï¿½Ì‚ï¿½100ï¿½{ï¿½ï¿½ï¿½ï¿½%ï¿½\ï¿½Lï¿½Å�oï¿½ï¿½
										switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType){
										case MEAS_PROG_TYPE_SR_REFLECTANCE:
										case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
										case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
										case MEAS_PROG_TYPE_SR_THICKNESS:
											pItem->strText.Format(szDecimalpoint, m_dAnalysisData[iIndex] * 100.0);
											break;
										default:
											pItem->strText.Format(szDecimalpoint, m_dAnalysisData[iIndex]);
										}
										//2010.02.01 bagus ï¿½Cï¿½ï¿½ --}--
									}
									else {
										pItem->strText = "";
									}
									break;
								case 2:
									///// Set Fit Data((Measured Data)-(Gen Data)) Graph Data /////
									if ( m_iModeStatus != 1/* TestMode Reference */ ) {
										if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS) {
											//2010.02.01 bagus ï¿½Cï¿½ï¿½ --{--
											//ï¿½È‰ï¿½ï¿½Ì‚ï¿½ï¿½Ì‚ï¿½100ï¿½{ï¿½ï¿½ï¿½ï¿½%ï¿½\ï¿½Lï¿½Å�oï¿½ï¿½
											switch(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType){
											case MEAS_PROG_TYPE_SR_REFLECTANCE:
											case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
											case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
											case MEAS_PROG_TYPE_SR_THICKNESS:
												pItem->strText.Format(szDecimalpoint, m_dAnalysisGenData[iIndex] * 100.0);
												break;
											default:
												pItem->strText.Format(szDecimalpoint, m_dAnalysisGenData[iIndex]);
											}
											//2010.02.01 bagus ï¿½Cï¿½ï¿½ --}--
										}
										else {
											pItem->strText = "";
										}
									}
									else {
										pItem->strText = "";
									}
									break;
								}
							}
						}
					}
// 2014.04.02 bagus wavelength step modified <--
					break;
				case 1:	//Lineï¿½Ïˆï¿½ DeflectionRaw Lineï¿½Sï¿½ï¿½
				case 2:	//Lineï¿½Ïˆï¿½ DeflectionRaw Sectionï¿½Pï¿½ï¿½
				case 3:	//LSï¿½ï¿½ï¿½ï¿½StressRaw
					//// Graph Data All Clear ////
					if((m_nDispLine < 0 || m_nDispLine >= STRESS_LINES_MAX)
					|| (m_nDispSection < 0 || m_nDispSection >= STRESS_SECTIONS_MAX)){
						return;
					}
					if(!stressStage.Line[m_nDispLine].bValidLine){
						return;
					}
					if(!stressStage.Line[m_nDispLine].bScanValid[m_nDispSection]){
						return;
					}

					m_pDoc->GetOneStressLineData(&tResult,m_nDispLine+1);

					lScanStartPosX = stressStage.Line[m_nDispLine].SectPos[m_nDispSection].lScanStartPosX;
					lScanEndPosX = stressStage.Line[m_nDispLine].SectPos[m_nDispSection].lScanEndPosX;

					//// GenData and SetData ////
					m_pDoc->GetOneStressLineDataRef(m_nDispLine);

					if ( GetCurrentTab() == 1 ) {
						//Lineï¿½Ïˆï¿½ DeflectionRaw Lineï¿½Sï¿½ï¿½
						if(tResult.nDeflectionOccurence[0] > 0){
							lScanStartPosX = static_cast<long>(tResult.DeflectionRaw[0][0].dPosum);
							lScanEndPosX = static_cast<long>(tResult.DeflectionRaw[0][tResult.nDeflectionOccurence[0]-1].dPosum);
						}else{
							lScanStartPosX = 0L;
							lScanEndPosX = 0L;
						}
					}

					nDataIndex = pItem->row - 1 + m_nDataIndex;

					if ( GetCurrentTab() == 1 || GetCurrentTab() == 2 ) {
						//Lineï¿½Ïˆï¿½ DeflectionRaw Lineï¿½Sï¿½ï¿½
						//Lineï¿½Ïˆï¿½ DeflectionRaw Sectionï¿½Pï¿½ï¿½
						switch ( pItem->col ) {
						case 0:	pItem->strText.Format("%.1f", tResult.DeflectionRaw[0][nDataIndex].dPosum / MICROMETRE);	break;
						//ï¿½ï¿½ï¿½Í‘ï¿½ï¿½ï¿½
						case 1:	pItem->strText.Format("%.3f", tResult.DeflectionRaw[0][nDataIndex].dDefl / MICROMETRE);		break;	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
						case 2:	if ( m_pDoc->GetStoreCount() == 0 ) break;	pItem->strText.Format("%.3f", tResult.DeflectionRaw[1][nDataIndex].dDefl / MICROMETRE);		break;	//ï¿½ï¿½ï¿½ï¿½
						case 3:	if ( m_pDoc->GetStoreCount() == 0 ) break;	pItem->strText.Format("%.3f", fabs(tResult.DeflectionRaw[0][nDataIndex].dDefl - tResult.DeflectionRaw[1][nDataIndex].dDefl) / MICROMETRE);				break;	//ï¿½ï¿½ï¿½ï¿½
						}
					}
					else {
					 	//LSï¿½ï¿½ï¿½ï¿½StressRaw
						switch ( pItem->col ) {
						case 0:	if ( m_pDoc->GetStoreCount() == 0 ) break;	pItem->strText.Format("%.1f", tResult.StressRaw[nDataIndex].dPosum / MICROMETRE);	break;
						//ï¿½ï¿½ï¿½Í‘ï¿½ï¿½ï¿½
						case 1:	if ( m_pDoc->GetStoreCount() == 0 ) break;	pItem->strText.Format("%.3f", tResult.StressRaw[nDataIndex].dStress);	break;
						}
					}
					break;
				}
			}
			else if ( m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SE ) {
				if ( m_iDispPointNo > 0 ) {
					switch ( GetCurrentTab() ) {
					case 0:	// Psi
						switch ( pItem->col ) {
						/// Wave Length[nm]
						case 0: pItem->strText.Format("%.1f", m_seRawData[pItem->row - 1].dWaveLength);	break;
						/// Measured Data
						case 1:	pItem->strText.Format("%.3f", m_seRawData[pItem->row - 1].dPsi);		break;
						/// Model Data
						case 2:	pItem->strText.Format("%.3f", m_seRawData[pItem->row - 1].dPsiModel);	break;
						/// Fit Data
						case 3:	pItem->strText.Format("%.3f", fabs(m_seRawData[pItem->row - 1].dPsi - m_seRawData[pItem->row - 1].dPsiModel));	break;
						}
						break;
					case 1:	// Delta
						switch ( pItem->col ) {
						/// Wave Length[nm]
						case 0: pItem->strText.Format("%.1f", m_seRawData[pItem->row - 1].dWaveLength);	break;
						/// Measured Data
						case 1:	pItem->strText.Format("%.3f", m_seRawData[pItem->row - 1].dDelta);		break;
						/// Model Data
						case 2:	pItem->strText.Format("%.3f", m_seRawData[pItem->row - 1].dDeltaModel);	break;
						/// Fit Data
						case 3:	pItem->strText.Format("%.3f", fabs(m_seRawData[pItem->row - 1].dDelta - m_seRawData[pItem->row - 1].dDeltaModel));	break;
						}
						break;
					}
				}
			}
		}
	}

	*pResult = 0;
}

// ï¿½Oï¿½ï¿½ï¿½bï¿½hï¿½ÌƒJï¿½ï¿½ï¿½ï¿½ï¿½Ì�æ“¾
void CMeasurementGraphDlg::CLightIntensityListGridHeader::GetHeader(WORD wHeadType, int iGraphDlgMode, int iTab, WORD wScanType)
{
	RemoveAll();

	switch(wHeadType){
	case HEAD_TYPE_SR:
	default:
		if(iGraphDlgMode == GRAPHDLG_MODE_TEST_RAW) //ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½h(RAWï¿½fï¿½[ï¿½^)
		{
			//2010.01.11 bagus GTR --{--
			if(wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G){
				Add("Wave [nm]");
				Add("Reference\nData");
				Add("Dark\nData");
			}else{
				Add("Wave [nm]");
				Add("Sample Data");
				Add("Reference Data");
				Add("Dark Reference");
			}
			//2010.01.11 bagus GTR --}--
		}
		else {
// 2010.01.21 bagus Gantry --{--
#if 0
			Add("Wave [nm]");
			Add("Measured Data[%]");
			Add("Model Data[%]");
#else
			switch ( wScanType ) {
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			case MEAS_PROG_TYPE_SR_REFLECTANCE:
				Add("Wave [nm]");
				Add("Measured Data[%]");
				break;
			default:
				Add("Wave [nm]");
				Add("Measured Data[%]");
				Add("Model Data[%]");
				break;
			}
#endif
// 2010.01.21 bagus Gantry --}--
		}
		break;
	case HEAD_TYPE_SE:
		Add("Wave [nm]");
		Add("Measured Data[%]");
		Add("Model Data[%]");
		Add("Fit Data[%]");
		break;
	case HEAD_TYPE_STRESS:
		switch(iTab){
		case 0: //Thick
			Add("Wave [nm]");
			Add("Meas");
			Add("Model");
			Add("Fit");
			break;
		case 1:	//Lineï¿½Ïˆï¿½
			Add("Loc [mm]");
			Add("Ref ");
			Add("Meas ");
			Add("Diff");
			break;
		case 2:	//LSï¿½Ïˆï¿½
			Add("Loc [mm]");
			Add("Ref ");
			Add("Meas ");
			Add("Diff");
			break;
		case 3:	//LSï¿½ï¿½ï¿½ï¿½
			Add("Loc [mm]");
			Add("Stress");
			Add("");
			break;
		}
		break;
	}
}

/* added 2015.03.19 hmenjo FWXGA ï¿½Î‰ï¿½ ---------- { ---------- */
void CMeasurementGraphDlg::ChgSizeCtrl(long ldY)
{
	/* ï¿½^ï¿½u	*/
	RECT l_rectMeasGraphTab;	m_MeasureGraphTab.GetWindowRect(&l_rectMeasGraphTab);
	m_MeasureGraphTab.SetWindowPos(	0,
						0, 0,
						l_rectMeasGraphTab.right - l_rectMeasGraphTab.left,
						l_rectMeasGraphTab.bottom - l_rectMeasGraphTab.top + ldY,
						SWP_NOZORDER | SWP_NOMOVE);
	/* ï¿½Oï¿½ï¿½ï¿½bï¿½h	*/
	RECT l_rectIntGrid;	m_LightIntensityListGrid.GetWindowRect(&l_rectIntGrid);
	m_LightIntensityListGrid.SetWindowPos(	0,
						0, 0,
						l_rectIntGrid.right - l_rectIntGrid.left,
						l_rectIntGrid.bottom - l_rectIntGrid.top + ldY,
						SWP_NOZORDER | SWP_NOMOVE);
	/* ï¿½`ï¿½Fï¿½bï¿½Nï¿½{ï¿½bï¿½Nï¿½Xï¿½Fï¿½Iï¿½[ï¿½gï¿½Xï¿½Pï¿½[ï¿½ï¿½	*/
	RECT l_rectAutoScaleChk;	this->GetDlgItem(IDC_GRAPH_AUTO_SCALE)->GetWindowRect(&l_rectAutoScaleChk);
	POINT l_ptAutoScaleChk = {l_rectAutoScaleChk.left, l_rectAutoScaleChk.top + ldY};
	this->ScreenToClient(&l_ptAutoScaleChk);
	this->GetDlgItem(IDC_GRAPH_AUTO_SCALE)->SetWindowPos(0, l_ptAutoScaleChk.x, l_ptAutoScaleChk.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	/* ï¿½Oï¿½ï¿½ï¿½t	*/
	RECT l_rectMeasGraph;	m_MeasureGraph.GetWindowRect(&l_rectMeasGraph);
	m_MeasureGraph.SetWindowPos(	0,
						0, 0,
						l_rectMeasGraph.right - l_rectMeasGraph.left,
						l_rectMeasGraph.bottom - l_rectMeasGraph.top + ldY,
						SWP_NOZORDER | SWP_NOMOVE);
	/* ï¿½{ï¿½Ìƒ_ï¿½Cï¿½Aï¿½ï¿½ï¿½O	*/
	RECT l_rectDlg;	this->GetWindowRect(&l_rectDlg);
	this->SetWindowPos(	0,
						0, 0,
						l_rectDlg.right - l_rectDlg.left,
						l_rectDlg.bottom - l_rectDlg.top + ldY,
						SWP_NOZORDER | SWP_NOMOVE);
}
/* added 2015.03.19 hmenjo FWXGA ï¿½Î‰ï¿½ ---------- } ---------- */
