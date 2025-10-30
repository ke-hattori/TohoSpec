// RecipeGantryStageProgramView.cpp : ÔøΩCÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩeÔøΩ[ÔøΩVÔøΩÔøΩÔøΩÔøΩ ÔøΩtÔøΩ@ÔøΩCÔøΩÔøΩ
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
//#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "RecipeGantryStageProgramView.h"
#include "OverlayDialog.h"
#include "PointConfirmationDlg.h"
#include "BitmapStatic.h"
#include "DeskewDlg.h"
#include "GridCellCheck.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "../../INC/NSStage.hxx"

#undef SubclassWindow	// ÔøΩoÔøΩO: C4003: SubclassWindow ÔøΩ}ÔøΩNÔøΩÔøΩÔøΩÃÉpÔøΩÔøΩÔøΩÔøΩÔøΩ[ÔøΩ^ÔøΩ[ÔøΩÃïsÔøΩÔøΩ http://support.microsoft.com/kb/150076/ja

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STAGE_OFFSET		15
#define RADIUS				1
#define ELLIPSE_RADIUS		2//4

#define	DEFAULT_GRID_ROWS			50

// 2009.10.09 bagus StagePGM ÔøΩÔøΩÔøΩ âÔøΩ --{--
#define	STAGE_PROGRAM_HEAD_TYPE		(0)
// 2009.10.09 bagus StagePGM ÔøΩÔøΩÔøΩ âÔøΩ --}--

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

/////////////////////////////////////////////////////////////////////////////
// CRecipeGantryStageProgramView ÔøΩ_ÔøΩCÔøΩAÔøΩÔøΩÔøΩO


// ==========================================================================
//
IMPLEMENT_DYNCREATE(CRecipeGantryStageProgramView, CNanoRecipeUI)

// ==========================================================================
//
CRecipeGantryStageProgramView::CRecipeGantryStageProgramView()
	: CNanoRecipeUI(CRecipeGantryStageProgramView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeGantryStageProgramView)
		// ÔøΩÔøΩÔøΩÔøΩ - ClassWizard ÔøΩÕÇÔøΩÔøΩÃà íuÔøΩ…É}ÔøΩbÔøΩsÔøΩÔøΩÔøΩOÔøΩpÔøΩÃÉ}ÔøΩNÔøΩÔøΩÔøΩÔøΩ«âÔøΩÔøΩ‹ÇÔøΩÔøΩÕçÌèúÔøΩÔøΩÔøΩ‹ÇÔøΩÔøΩB
	//}}AFX_DATA_INIT

	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_STAGE_PROGRAM, l_strBuffer, "STAGE PROGRAM");
	m_strCaption = l_strBuffer;

	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
	::ZeroMemory(m_OldScanPoint, sizeof(m_OldScanPoint));

	memset(&m_ValidPoint, FALSE, sizeof(m_ValidPoint));
	memset(&m_OldValidPoint, FALSE, sizeof(m_OldValidPoint));

	m_nSysVScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
	m_iNumScans = 0;
}

CRecipeGantryStageProgramView::~CRecipeGantryStageProgramView()
{
}

// ==========================================================================
//
void CRecipeGantryStageProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeGantryStageProgramView)
	DDX_Control(pDX, IDC_SAMPLE_POINT_MAP, m_SamplePointGraph);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STAGE_PROGRAM_NAME, m_StageProgInfoHdr.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Control(pDX, IDC_STAGE_PROGRAM_POINT_NUMBER, m_stcStageProgramNumScan);
	DDX_Control(pDX, IDC_SAMPLE_NAME, m_stcSampleName);
	DDX_Control(pDX, IDC_SAMPLE_SHAPE, m_stcSampleShape);
	DDX_Control(pDX, IDC_SAMPLE_THICKNESS, m_stcSampleThickness);
	DDX_Control(pDX, IDC_SAMPLE_SIZE_X, m_stcSampleSizeX);
	DDX_Control(pDX, IDC_SAMPLE_SIZE_Y, m_stcSampleSizeY);
	DDX_Control(pDX, IDC_SAMPLE_ORIGIN_X, m_stcSampleOriginX);
	DDX_Control(pDX, IDC_SAMPLE_ORIGIN_Y, m_stcSampleOriginY);
	DDX_Control(pDX, IDC_MEASUREMENT_POINT_GRID, m_MeasurementPointGrid);
	DDX_Control(pDX, IDC_TEACHING_BUTTON, m_TeachingButton);
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CRecipeGantryStageProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeGantryStageProgramView)
	ON_BN_CLICKED(IDC_TEACHING_BUTTON, OnTeachingButton)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
// 2010.01.29 bagus Gantry ÔøΩCÔøΩÔøΩ --{--
	ON_NOTIFY(GVN_SELCHANGED, IDC_MEASUREMENT_POINT_GRID, OnSelChanged)
	ON_NOTIFY(GVN_SELCHANGING, IDC_MEASUREMENT_POINT_GRID, OnSelChangeing)
// 2010.01.29 bagus Gantry ÔøΩCÔøΩÔøΩ --}--
END_MESSAGE_MAP()

// ==========================================================================
// CRecipeGantryStageProgramView ÔøΩfÔøΩf

#ifdef _DEBUG
// ==========================================================================
//
void CRecipeGantryStageProgramView::AssertValid() const
{
	CNanoRecipeUI::AssertValid();
}

// ==========================================================================
//
void CRecipeGantryStageProgramView::Dump(CDumpContext& dc) const
{
	CNanoRecipeUI::Dump(dc);
}
#endif //_DEBUG

// ==========================================================================
// CRecipeGantryStageProgramView ÔøΩÔøΩÔøΩbÔøΩZÔøΩ[ÔøΩW ÔøΩnÔøΩÔøΩÔøΩhÔøΩÔøΩ

// ==========================================================================
//
void CRecipeGantryStageProgramView::OnInitialUpdate()
{
	CString l_strBuffer;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);

	memset(&m_SrReferencePosition, 0, sizeof(m_SrReferencePosition));
	ConfigFile_GetNanoSpecIni(&m_SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	memset(&m_SrConfig, 0, sizeof(m_SrConfig));
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);

	if ( !LoadRecipeData() ) {
		UpDate();
		::CopyMemory(&m_OldStageProgInfoHdr, &m_StageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
		::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
		CheckValidPoint();
		::CopyMemory(m_OldValidPoint, m_ValidPoint, sizeof(m_ValidPoint));
		LoadStringML(IDS_NOT_OPEN_SAMPLE_TYPE, l_strBuffer, "Can not open the sample type file.\nReturn to Stage Program List.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		m_pMainFrame->PostMessage(WM_EXE_WND_EXIT, 0, 0);
		return;
	}

	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndEditBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndEditBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// ÔøΩÏê¨ÔøΩ…éÔøΩÔøΩs
	}

	GetClientRect(&rect);
	m_wndEditBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	///// Static Control /////
	m_stcStageProgramName.SubclassWindow(GetDlgItem(IDC_STAGE_PROGRAM_NAME)->GetSafeHwnd());
	m_stcStageProgramName.SetBkColor(WATER_COLOR);
	m_stcStageProgramNumScan.SetBkColor(WATER_COLOR);
	m_stcSampleName.SetBkColor(WATER_COLOR);
	m_stcSampleShape.SetBkColor(WATER_COLOR);
	m_stcSampleThickness.SetBkColor(WATER_COLOR);
	m_stcSampleSizeX.SetBkColor(WATER_COLOR);
	m_stcSampleSizeY.SetBkColor(WATER_COLOR);
	m_stcSampleOriginX.SetBkColor(WATER_COLOR);
	m_stcSampleOriginY.SetBkColor(WATER_COLOR);

	m_TeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TeachingButton.DrawFlatFocus(TRUE);

// 2009.10.14 bagus Gantry ÔøΩCÔøΩÔøΩ --{--
	///// Axis Synchronous /////
//	CheckDlgButton(IDC_AXIS_SYNCHRONOUS_CHECK, BST_UNCHECKED);
// 2009.10.14 bagus Gantry ÔøΩCÔøΩÔøΩ --}--

	///// Get Current Execute Window /////
	m_iWnd = m_pMainFrame->GetCurrExeWnd();

	///// Measurement Point Grid /////
	m_SamplePointGraph.SetLineStatusMode(FALSE);
	SamplePointGraph_Init();

	///// Set Data /////
	SetData();

	// ÔøΩRÔøΩÔøΩÔøΩÔøΩÔøΩgÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	char szFilePath[MAX_PATH + 1];
	char szFilePathBackUp[MAX_PATH + 1];
	char szFilePathUntitled[MAX_PATH + 1];

	// ÔøΩpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÊëúÔøΩiÔøΩVÔøΩKÔøΩÏê¨ÔøΩjÔøΩÃèÍçáÔøΩÃÇÔøΩÔøΩ›çÌèúÔøΩiUntitledÔøΩAÔøΩ^ÔøΩCÔøΩgÔøΩÔøΩÔøΩ»ÇÔøΩÔøΩÃñÔøΩÔøΩOÔøΩ≈ï€ëÔøΩÔøΩÔøΩÔøΩÍÇΩÔøΩfÔøΩ[ÔøΩ^ÔøΩj
	if ( m_nOpenMode == modeNew ) {
		_stprintf(szFilePathUntitled, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_StageProgInfoHdr.hdr.szName);
		::DeleteFile(szFilePathUntitled);
		_stprintf(szFilePathUntitled, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_StageProgInfoHdr.hdr.szName);
		::DeleteFile(szFilePathUntitled);
	}

	// ÔøΩfÔøΩoÔøΩbÔøΩOÔøΩÔøΩÔøΩ…ägÔøΩÔøΩÔøΩqÔøΩÃÇ›ÇÃÇÔøΩÔøΩ›ÉtÔøΩ@ÔøΩCÔøΩÔøΩÔøΩÔøΩÔøΩ≈ÇÔøΩÔøΩƒÇÔøΩÔøΩ‹ÇÔøΩÔøΩƒÇÔøΩÔøΩÔøΩÃÇ≈ÅAÔøΩOÔøΩÃÇÔøΩÔøΩﬂÇ…çÌèúÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩƒÇÔøΩÔøΩÔøΩ
	_stprintf(szFilePathUntitled, "%s.D1", g_szDb_Deskew_Img_Dir);
	::DeleteFile(szFilePathUntitled);
	_stprintf(szFilePathUntitled, "%s.D2", g_szDb_Deskew_Img_Dir);
	::DeleteFile(szFilePathUntitled);

	// ÔøΩpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÊëúÔøΩiÔøΩÔøΩÔøΩ[ÔøΩNÔøΩjÔøΩÃÇÔøΩÔøΩ›çÌèú
	_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
	::DeleteFile(szFilePathBackUp);
	_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
	::DeleteFile(szFilePathBackUp);

	// ÔøΩpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÊëúÔøΩÃëﬁîÔøΩ
	_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_StageProgInfoHdr.hdr.szName);
	_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
	::CopyFile(szFilePath, szFilePathBackUp, FALSE);
	_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_StageProgInfoHdr.hdr.szName);
	_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
	::CopyFile(szFilePath, szFilePathBackUp, FALSE);

	SamplePointGraph_DataSet();

	CheckMeasurementPointData();

	SetTimer(1,100,NULL);
}

// ==========================================================================
//
void CRecipeGantryStageProgramView::OnTeachingButton()
{
	CString l_strBuffer;
	CString strMsg;

// 2010.02.02 K.Matsuo PIN UPPER ÔøΩCÔøΩÔøΩÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩbÔøΩNÔøΩ`ÔøΩFÔøΩbÔøΩNÔøΩRÔøΩÔøΩŒâÔøΩ -->
	// ÔøΩÔøΩÔøΩCÔøΩÔøΩÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩbÔøΩNÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩeÔøΩiÔøΩÔøΩÔøΩX
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // ÔøΩÔøΩÔøΩÔøΩÔøΩeÔøΩiÔøΩÔøΩÔøΩXÔøΩŸèÔøΩ
	}
	else{												// ÔøΩ èÌìÆÔøΩÔøΩ
	}
	// ÔøΩeI/OÔøΩ`ÔøΩFÔøΩbÔøΩN
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVOÔøΩAÔøΩÔøΩÔøΩ[ÔøΩÔøΩ 				ÔøΩÔøΩÔøΩk
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// ÔøΩVÔøΩÔøΩÔøΩbÔøΩ^ÔøΩ[ÔøΩ»äOÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩƒÇÔøΩÔøΩÔøΩÔøΩÁìÆÔøΩÔøΩ÷é~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩÔøΩÔøΩÏíÜ
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ÔøΩÔøΩÔøΩCÔøΩÔøΩÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩbÔøΩNÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
// 2010.02.02 K.Matsuo PIN UPPER ÔøΩCÔøΩÔøΩÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩbÔøΩNÔøΩ`ÔøΩFÔøΩbÔøΩNÔøΩRÔøΩÔøΩŒâÔøΩ <--

	CPointConfirmationDlg dlg;

	UpDate();
	dlg.m_iNumScans = m_SrConfig.wGantryNumOfMeasPoint;
	memcpy(&dlg.m_StageProgInfoHdr, &m_StageProgInfoHdr, sizeof(dlg.m_StageProgInfoHdr));
//	memcpy(dlg.m_ScanPoint, m_ScanPoint, sizeof(dlg.m_ScanPoint));
// 2009.11.20 K.Matsuo Bug Fix STAGE_COORDÔøΩÔøΩSTAGE_COORD_XYZÔøΩÔøΩsÔøΩÔøΩmemcpy -->
//	memcpy(dlg.m_ScanPoint, m_SrReferencePosition.LocGantryTransmitReference, sizeof(dlg.m_ScanPoint));
	for ( int i = 0; i < m_SrConfig.wGantryNumOfMeasPoint; i++ ) {
		dlg.m_ScanPoint[i].lX = m_SrReferencePosition.LocGantryTransmitReference[i].lX;
		dlg.m_ScanPoint[i].lY = m_SrReferencePosition.LocGantryTransmitReference[i].lY;
	}
// 2009.11.20 K.Matsuo Bug Fix STAGE_COORDÔøΩÔøΩSTAGE_COORD_XYZÔøΩÔøΩsÔøΩÔøΩmemcpy <--
	memcpy(dlg.m_ValidPoint, m_ValidPoint, sizeof(dlg.m_ValidPoint));

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(m_StageProgInfoHdr.SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	//dlg.SetFileName(&m_StageProgInfoHdr);
	LoadStringML(IDS_POINT_CONFIRMATION_CAPTION, l_strBuffer, "MEASUREMENT POINT - CONFIRMATION");
	dlg.SetCaption(l_strBuffer);

	LoadStringML(IDS_POINT_CONFIRMATION_MESSAGE, l_strBuffer, "MEASUREMENT POINT - CONFIRMATION");
	dlg.SetMessage(l_strBuffer);

	if(dlg.DoModal() == IDOK){
		memcpy(m_ValidPoint, dlg.m_ValidPoint, sizeof(m_ValidPoint));

		SetMeasurementPointData();
		SamplePointGraph_DataSet();

		m_SamplePointGraph.Invalidate();
		Invalidate();

//		OnPaint();
	}
}

// ==========================================================================
//
BOOL CRecipeGantryStageProgramView::LoadRecipeData()
{
	TCHAR szSampleTypeName[RECIPE_NAME_LEN + 1];
	SAMPLEINFO SampleInfo;


	// ÔøΩVÔøΩKÔøΩÏê¨ÔøΩÃèÍçá
	if ( m_nOpenMode == modeNew ) {
		///// Get Selected Sample Type	Name /////
		m_pMainFrame->GetSelectSampleTypeName(szSampleTypeName);

		//	///// Sample /////
		if ( !RecipeFile_LoadRecipe(&SampleInfo, szSampleTypeName, RECIPE_FILE_SAMPLE) ) {
			memset(&m_StageProgInfoHdr, 0, sizeof(m_StageProgInfoHdr));
			memset(m_ScanPoint, 0, sizeof(m_ScanPoint));
			return FALSE;
		}
		else {
			strcpy(m_StageProgInfoHdr.SampleInfo.szName, SampleInfo.szName);
			m_StageProgInfoHdr.SampleInfo.nShape = SampleInfo.nShape;
			m_StageProgInfoHdr.SampleInfo.dThickness = SampleInfo.dThickness;
			m_StageProgInfoHdr.SampleInfo.Size.dx = SampleInfo.Size.dx;
			m_StageProgInfoHdr.SampleInfo.Size.dy = SampleInfo.Size.dy;
			m_StageProgInfoHdr.SampleInfo.Origin.lX = SampleInfo.Origin.lX;
			m_StageProgInfoHdr.SampleInfo.Origin.lY = SampleInfo.Origin.lY;
		}
// 2009.10.09 bagus StagePGM ÔøΩÔøΩÔøΩ âÔøΩ --{--
//		m_StageProgInfoHdr.wScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE_G;
// 2009.10.09 bagus StagePGM ÔøΩÔøΩÔøΩ âÔøΩ --}--
// 2009.12.14 bagus NANOMAP ÔøΩ«âÔøΩ --{--
		m_StageProgInfoHdr.SampleInfo.StageConfig = m_StageConfig;
// 2009.12.14 bagus NANOMAP ÔøΩ«âÔøΩ --}--
// 2010.01.15 bagus GTR --{--
		for (int i=0; i<m_SrConfig.wGantryNumOfMeasPoint; i++) {
			m_ValidPoint[i] = TRUE;
		}
// 2010.01.15 bagus GTR --}--
	}
	else{
		///// Stage Program Header /////
		if ( !RecipeFile_LoadRecipe(&m_StageProgInfoHdr, m_szRecipeName, RECIPE_FILE_STAGE_PROGRAM) ) {
			m_nOpenMode = modeNew;
			::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
			::ZeroMemory(&m_OldStageProgInfoHdr, sizeof(m_OldStageProgInfoHdr));
		}
		else{
// 2009.12.02 K.Matsuo SampleOrigin -->
			SAMPLEINFO sampleInfo;
			::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
			RecipeFile_LoadRecipe(&sampleInfo, m_StageProgInfoHdr.SampleInfo.szName, RECIPE_FILE_SAMPLE);
			m_StageProgInfoHdr.SampleInfo.Origin.lX = sampleInfo.Origin.lX;
			m_StageProgInfoHdr.SampleInfo.Origin.lY = sampleInfo.Origin.lY;
// 2009.12.02 K.Matsuo SampleOrigin <--
			::CopyMemory(&m_OldStageProgInfoHdr, &m_StageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
		}

		///// Measurement Point /////
		if ( !RecipeFile_LoadPointList(m_ScanPoint, m_StageProgInfoHdr.wNumScans, m_szRecipeName) ) {
			m_nOpenMode = modeNew;
			::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
			::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
		}
		else{
			::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
			CheckValidPoint();
			::CopyMemory(m_OldValidPoint, m_ValidPoint, sizeof(m_ValidPoint));
		}

		///// Sample Name /////
		strcpy(szSampleTypeName, m_StageProgInfoHdr.SampleInfo.szName);
	}

	if ( m_nOpenMode == modeNew ) {
		CString strBuffer;
		LoadStringML(IDS_UNTITLE_NAME, strBuffer, "Untitled");
		strcpy(m_StageProgInfoHdr.hdr.szName, strBuffer);
		strcpy(m_OldStageProgInfoHdr.hdr.szName, strBuffer);
	}

// 2009.10.09 bagus StagePGM ÔøΩÔøΩÔøΩ âÔøΩ --{--
	m_StageProgInfoHdr.wHeadType = STAGE_PROGRAM_HEAD_TYPE;
	m_StageProgInfoHdr.wScanType = SCAN_TYPE_SR_GANTRY;
// 2009.10.09 bagus StagePGM ÔøΩÔøΩÔøΩ âÔøΩ --}--
// 2009.10.13 bagus StagePGM ÔøΩCÔøΩÔøΩ --{--
	m_OldStageProgInfoHdr.wHeadType = STAGE_PROGRAM_HEAD_TYPE;
	m_OldStageProgInfoHdr.wScanType = SCAN_TYPE_SR_GANTRY;
// 2009.10.13 bagus StagePGM ÔøΩCÔøΩÔøΩ --}--

	return TRUE;
}

// ==========================================================================
//
void CRecipeGantryStageProgramView::SetData()
{
	char szBuff[256];

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// Name /////
	SetDlgItemText(IDC_STAGE_PROGRAM_NAME, m_StageProgInfoHdr.hdr.szName);

	///// Sample - Name /////
	SetDlgItemText(IDC_SAMPLE_NAME, m_StageProgInfoHdr.SampleInfo.szName);

	///// Sample - Shape /////
	m_StageProgInfoHdr.SampleInfo.nShape = m_StageProgInfoHdr.SampleInfo.nShape >= SAMPLE_SHAPE_MAX ? 0 : m_StageProgInfoHdr.SampleInfo.nShape;
	if(l_SystemConfig.nLanguage == 0){
		SetDlgItemText(IDC_SAMPLE_SHAPE, g_lpszSampleShape_ENU[m_StageProgInfoHdr.SampleInfo.nShape]);
	}
	else{
		SetDlgItemText(IDC_SAMPLE_SHAPE, g_lpszSampleShape_JPN[m_StageProgInfoHdr.SampleInfo.nShape]);
	}

	///// Sample - Thickness /////
	sprintf(szBuff, "%.3lf", m_StageProgInfoHdr.SampleInfo.dThickness / MICROMETRE);
	SetDlgItemText(IDC_SAMPLE_THICKNESS, szBuff);

	///// Sample - Size X /////
	sprintf(szBuff, "%.3lf", m_StageProgInfoHdr.SampleInfo.Size.dx / MICROMETRE);
	SetDlgItemText(IDC_SAMPLE_SIZE_X, szBuff);

	///// Sample - Size Y /////
	sprintf(szBuff, "%.3lf", m_StageProgInfoHdr.SampleInfo.Size.dy / MICROMETRE);
	SetDlgItemText(IDC_SAMPLE_SIZE_Y, szBuff);

	///// Sample - Origin X /////
	sprintf(szBuff, "%.3lf", ((double)m_StageProgInfoHdr.SampleInfo.Origin.lX / MICROMETRE));
	SetDlgItemText(IDC_SAMPLE_ORIGIN_X, szBuff);

	///// Sample - Origin Y /////
	sprintf(szBuff, "%.3lf", ((double)m_StageProgInfoHdr.SampleInfo.Origin.lY / MICROMETRE));
	SetDlgItemText(IDC_SAMPLE_ORIGIN_Y, szBuff);

	///// Point Number /////
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);

	///// Point Coord /////
	SetMeasurementPointData();

	///// Comment /////
	SetDlgItemText(IDC_COMMENT, m_StageProgInfoHdr.hdr.szComment);
}

// ==========================================================================
//
void CRecipeGantryStageProgramView::UpDate()
{
	CString strBuffer;
	BOOL bShape = FALSE;

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// Name /////
	GetDlgItemText(IDC_STAGE_PROGRAM_NAME, strBuffer);
	strBuffer.Insert(RECIPE_NAME_LEN, '\0');
	strcpy(m_StageProgInfoHdr.hdr.szName, strBuffer);

	///// Sample Name /////
	GetDlgItemText(IDC_SAMPLE_NAME, strBuffer);
	strBuffer.Insert(RECIPE_NAME_LEN, '\0');
	strcpy(m_StageProgInfoHdr.SampleInfo.szName, strBuffer);

	///// Sample -Shape /////
	GetDlgItemText(IDC_SAMPLE_SHAPE, strBuffer);
	for ( int i = 0 ; i < SAMPLE_SHAPE_MAX ; i ++ ) {
		if(l_SystemConfig.nLanguage == 0){
			if ( strBuffer == g_lpszSampleShape_ENU[i] ) {
				m_StageProgInfoHdr.SampleInfo.nShape = i;
				bShape = TRUE;
			}
		}
		else{
			if ( strBuffer == g_lpszSampleShape_JPN[i] ) {
				m_StageProgInfoHdr.SampleInfo.nShape = i;
				bShape = TRUE;
			}
		}
	}
	if ( !bShape ) {
		CString strBuffer;
		LoadStringML(IDS_SHAPE_NOT_EXIST_INP, strBuffer, "The Shape not Existing is Inputted.");
		MessageBox(strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
	}

	///// Thickness /////
	GetDlgItemText(IDC_SAMPLE_THICKNESS, strBuffer);
	m_StageProgInfoHdr.SampleInfo.dThickness = atof(strBuffer) * MICROMETRE;

	///// Sample - Size X /////
	GetDlgItemText(IDC_SAMPLE_SIZE_X, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Size.dx = atof(strBuffer) * MICROMETRE;

	///// Sample - Size Y /////
	GetDlgItemText(IDC_SAMPLE_SIZE_Y, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Size.dy = atof(strBuffer) * MICROMETRE;

	///// Sample - Origin X /////
	GetDlgItemText(IDC_SAMPLE_ORIGIN_X, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Origin.lX = static_cast<long>(atof(strBuffer) * MICROMETRE);

	///// Sample - Origin Y /////
	GetDlgItemText(IDC_SAMPLE_ORIGIN_Y, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Origin.lY = static_cast<long>(atof(strBuffer) * MICROMETRE);

	///// Point Count /////
	m_StageProgInfoHdr.wNumScans = (WORD)m_iNumScans;

	///// Comment /////
	GetDlgItemText(IDC_COMMENT, m_StageProgInfoHdr.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// ==========================================================================
//
BOOL CRecipeGantryStageProgramView::SaveRecipeData()
{
	char szFilePath[MAX_PATH + 1];
	char szFilePathBackUp[MAX_PATH + 1];
	char szOriginalFilePath[MAX_PATH + 1];
	char szNewFilePath[MAX_PATH + 1];

	// ÔøΩVÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩVÔøΩsÔøΩÔøΩÔøΩÔøΩ›íÔøΩiSave, SaveAsÔøΩÔøΩÔøΩpÔøΩj
	strcpy(m_StageProgInfoHdr.hdr.szName, m_szRecipeName);

	if ( strcmp(m_OldStageProgInfoHdr.hdr.szName, m_szRecipeName) != 0 ) {
		// ÔøΩVÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩVÔøΩsÔøΩÔøΩÔøΩÔøΩ›íÔøΩiSaveAsÔøΩÔøΩpÔøΩj

		// ÔøΩÔøΩÔøΩÔøΩÔøΩVÔøΩsÔøΩÃÉpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÊëúÔøΩÔøΩVÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩVÔøΩsÔøΩÔøΩÔøΩ≈çÏê¨
		// ÔøΩiÔøΩpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÔøΩÔøΩ“èWÔøΩrÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩmÔøΩÔøΩ»ÇÔøΩÔøΩÃÇ≈ÅAÔøΩÔøΩÔøΩVÔøΩsÔøΩ“èWÔøΩÔøΩÔøΩÃìÔøΩÔøΩeÔøΩ≈çÏê¨ÔøΩÔøΩÔøΩÔøΩj
		_stprintf(szOriginalFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		_stprintf(szNewFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_szRecipeName);
		::DeleteFile(szNewFilePath);
		::CopyFile(szOriginalFilePath, szNewFilePath, FALSE);
		_stprintf(szOriginalFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		_stprintf(szNewFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_szRecipeName);
		::DeleteFile(szNewFilePath);
		::CopyFile(szOriginalFilePath, szNewFilePath, FALSE);

		// ÔøΩÔøΩÔøΩÔøΩÔøΩVÔøΩsÔøΩÃÉpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÊëúÔøΩiÔøΩﬁîÔøΩÔøΩjÔøΩÃñﬂÇÔøΩ
		// ÔøΩiÔøΩpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÔøΩÔøΩ“èWÔøΩrÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩmÔøΩÔøΩ»ÇÔøΩÔøΩÃÇ≈ÅAÔøΩÔøΩÔøΩVÔøΩsÔøΩ“èWÔøΩOÔøΩÃèÔøΩ‘Ç‹Ç≈ñﬂÇÔøΩÔøΩj
		_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
		_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		::CopyFile(szFilePathBackUp, szFilePath, FALSE);
		_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
		_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		::CopyFile(szFilePathBackUp, szFilePath, FALSE);
	}

	// ÔøΩpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÊëúÔøΩÃÉoÔøΩbÔøΩNÔøΩAÔøΩbÔøΩvÔøΩÔøΩÔøΩÏê¨
	// ÔøΩiÔøΩÔøΩÔøΩÔøΩÔøΩAÔøΩÔøΩÔøΩÃåÔøΩÃëÔøΩÔøΩÔøΩ≈ÅAÔøΩpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩ“èWÔøΩÀï€ëÔøΩÔøΩLÔøΩÔøΩÔøΩÔøΩÔøΩZÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÍçáÔøΩAÔøΩÔøΩÔøΩÃÉoÔøΩbÔøΩNÔøΩAÔøΩbÔøΩvÔøΩÔøΩÔøΩ_ÔøΩÃìÔøΩÔøΩeÔøΩ≈èÔøΩÔøΩÔøΩÔøΩﬂÇÔøΩÔøΩj
	_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_szRecipeName);
	_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
	::CopyFile(szFilePath, szFilePathBackUp, FALSE);
	_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_szRecipeName);
	_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
	::CopyFile(szFilePath, szFilePathBackUp, FALSE);

	if ( !RecipeFile_SaveRecipe(&m_StageProgInfoHdr, m_StageProgInfoHdr.hdr.szName, RECIPE_FILE_STAGE_PROGRAM) )
		return FALSE;

	if ( !RecipeFile_SavePointList(m_ScanPoint, m_StageProgInfoHdr.wNumScans, m_StageProgInfoHdr.hdr.szName) )
		return FALSE;

	//2009.12.10 bagus Recipe Backup --{--
	USER_DATA l_UserData;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig,CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_GetNanoSpecIni(&l_UserData,CONFIG_FILE_USER_SETTING);
	if(l_SystemConfig.bBackup){
		if(l_UserData.BackupData.bAutoBackup && strlen(l_UserData.BackupData.BackupPath) > 0 ){
			//ÔøΩÔøΩÔøΩÔøΩÔøΩoÔøΩbÔøΩNÔøΩAÔøΩbÔøΩvÔøΩÔøΩÔøΩLÔøΩÔøΩÔøΩ»ÇÃÇ≈ÇÔøΩÔøΩÔøΩÔøΩ≈âÊëúÔøΩtÔøΩ@ÔøΩCÔøΩÔøΩÔøΩÔøΩÔøΩoÔøΩbÔøΩNÔøΩAÔøΩbÔøΩvÔøΩÔøΩÔøΩÔøΩ
			TCHAR fullPath[MAX_PATH];
			CString strSourceImage;
			CString strBackupPath;
			BOOL bCreateDir = TRUE;
			_fullpath(fullPath,l_UserData.BackupData.BackupPath,sizeof(fullPath));
			_tcscat(fullPath,"\\" DB_DIR_NAME "\\" DB_DESKEW_IMG_DIR_NAME "\\" );
			for(int n = 0;n < static_cast<int>(strlen(fullPath));n++){
				if(n > 0 && fullPath[n] == '\\' && fullPath[n-1] != ':'){
					fullPath[n] = '\0';
					bCreateDir = CreateDirectory(fullPath,NULL);
					if(!bCreateDir){
						if(GetLastError() == ERROR_ALREADY_EXISTS){
							bCreateDir = TRUE;
						}
					}
					fullPath[n] = '\\';
					if(!bCreateDir)
						break;
				}
			}
			if(bCreateDir){
				strSourceImage.Format("%s%s.D1",g_szDb_Deskew_Img_Dir,m_szRecipeName);
				strBackupPath.Format("%s%s.D1",fullPath,m_szRecipeName);
				CopyFile(szFilePath,strBackupPath.GetBuffer(0),FALSE);
				strSourceImage.Format("%s%s.D2",g_szDb_Deskew_Img_Dir,m_szRecipeName);
				strBackupPath.Format("%s%s.D2",fullPath,m_szRecipeName);
				CopyFile(szFilePath,strBackupPath.GetBuffer(0),FALSE);
			}
		}
	}
	//2009.12.10 bagus Recipe Backup --}--

	SetDlgItemText(IDC_STAGE_PROGRAM_NAME, m_StageProgInfoHdr.hdr.szName);

	///// Point Number /////
	TCHAR szBuff[256];
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);

	::CopyMemory(&m_OldStageProgInfoHdr, &m_StageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
	::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
	::CopyMemory(m_OldValidPoint, m_ValidPoint, sizeof(m_ValidPoint));

	CRect rect;
	GetDlgItem(IDC_MEASUREMENT_MAP)->GetClientRect(rect);
	InvalidateRect(rect);

	return TRUE;
}

// ==========================================================================
//
BOOL CRecipeGantryStageProgramView::CheckData()
{
//	TCHAR szBuff[256];
//	double dMinX, dMaxX, dMinY, dMaxY;
	STAGE_COORD coordLeftTop;
	STAGE_COORD coordRightBottom;
	CString strBuffer;

	UpDate();

	if ( m_iNumScans <= 0 ) {
		LoadStringML(IDS_NO_MEASUREMENT_POINT, strBuffer, "There is no specification measurement point.");
		MessageBox(strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩÃëÂÇ´ÔøΩÔøΩÔøΩ≈ÉÔøΩÔøΩ~ÔøΩbÔøΩgÔøΩlÔøΩÔøΩ›íÔøΩ /////
	switch ( m_StageConfig.OrgPos ) {
	case 1: // ÔøΩÔøΩÔøΩS
		coordLeftTop.lX = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dx / 2 * -1);
		coordLeftTop.lY = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dy / 2);
		coordRightBottom.lX = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dx / 2);
		coordRightBottom.lY = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dy / 2 * -1);
		break;
	case 2: // ÔøΩÔøΩÔøΩÔøΩ
		coordLeftTop.lX = static_cast<long>(0);
		coordLeftTop.lY = static_cast<long>(0);
		coordRightBottom.lX = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dx);
		coordRightBottom.lY = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dy * -1);
		break;
	case 3: // ÔøΩÔøΩÔøΩÔøΩ
		coordLeftTop.lX = static_cast<long>(0);
		coordLeftTop.lY = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dy);
		coordRightBottom.lX = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dx);
		coordRightBottom.lY = 0;
		break;
	case 4: // ÔøΩEÔøΩÔøΩ
		coordLeftTop.lX = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dx * -1);
		coordLeftTop.lY = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dy);
		coordRightBottom.lX = 0;
		coordRightBottom.lY = 0;
		break;
	case 5: // ÔøΩEÔøΩÔøΩ
		coordLeftTop.lX = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dx * -1);
		coordLeftTop.lY = static_cast<long>(0);
		coordRightBottom.lX = 0;
		coordRightBottom.lY = static_cast<long>(m_StageProgInfoHdr.SampleInfo.Size.dy * -1);
		break;
	}

	// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩRÔøΩÔøΩÔøΩtÔøΩBÔøΩOÔøΩÃéÔøΩÔøΩ›íÔøΩ∆çÔøΩÔøΩÌÇπÔøΩÔøΩ
	NS_ConvertToStageConfigXyAxis(&coordLeftTop);
	NS_ConvertToStageConfigXyAxis(&coordRightBottom);

#if 0
	///// ÔøΩOÔøΩÔøΩÔøΩbÔøΩhÔøΩÃílÔøΩÔøΩÔøΩÊìæ /////
	for ( int i = 0; i < m_iNumScans; i++ ) {
		///// X /////
		if ( !(m_ScanPoint[i].lX >= min(coordLeftTop.lX, coordRightBottom.lX) && m_ScanPoint[i].lX <= max(coordLeftTop.lX, coordRightBottom.lX)) ) {
			LoadStringML(IDS_X_COORDINATE_OUT_LIMIT, strBuffer, "POINT = %d : X coordinate is out of limit.");
			sprintf(szBuff, strBuffer, i + 1);
			MessageBox(szBuff, m_strCaption, MB_OK | MB_ICONSTOP);
			return FALSE;
		}

		///// Y /////
		if ( !(m_ScanPoint[i].lY >= min(coordLeftTop.lY, coordRightBottom.lY) && m_ScanPoint[i].lY <= max(coordLeftTop.lY, coordRightBottom.lY)) ) {
			LoadStringML(IDS_Y_COORDINATE_OUT_LIMIT, strBuffer, "POINT = %d : Y coordinate is out of limit.");
			sprintf(szBuff, strBuffer, i + 1);
			MessageBox(szBuff, m_strCaption, MB_OK | MB_ICONSTOP);
			return FALSE;
		}
	}
#endif

	return TRUE;
}

// ==========================================================================
//
BOOL CRecipeGantryStageProgramView::IsDataChanged()
{
	UpDate();

	//ÔøΩÔøΩÔøΩÔøΩ|ÔøΩCÔøΩÔøΩÔøΩgÔøΩÔøΩÔøΩ‹Ç≈Ç≈ïœâÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÍçáÔøΩÕçXÔøΩVÔøΩÔøΩÔøΩÍÇΩÔøΩ∆Ç›Ç»ÇÔøΩ
	return ( memcmp(&m_StageProgInfoHdr, &m_OldStageProgInfoHdr, sizeof(m_StageProgInfoHdr)) != 0 ||
				memcmp(m_ScanPoint, m_OldScanPoint, (sizeof(m_ScanPoint) / 10000 * m_StageProgInfoHdr.wNumScans)) != 0 ||
				memcmp(&m_ValidPoint, &m_OldValidPoint, sizeof(m_ValidPoint)) != 0 );
}

// ==========================================================================
//
void CRecipeGantryStageProgramView::OnPaint()
{
	CPaintDC dc(this); // ÔøΩ`ÔøΩÔøΩpÔøΩÃÉfÔøΩoÔøΩCÔøΩX ÔøΩRÔøΩÔøΩÔøΩeÔøΩLÔøΩXÔøΩg
	long lDrawStageSizeX, lDrawStageSizeY;
	CRect StageRect;
	CRect SampleRect;

	///// Region Map /////
	CWnd* pPictBox = GetDlgItem(IDC_MEASUREMENT_MAP);
	CDC* pDC = pPictBox->GetDC();
	pPictBox->GetClientRect(&StageRect);

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

	///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩLÔøΩÔøΩÔøΩÕàÔøΩ /////
// 2009.12.09 bagus --{--
//	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
	double dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
// 2009.12.09 bagus --}--

	if((dStageMaxSizeX / dStageMaxSizeY) > ((StageRectRight - StageRectLeft) / (StageRectBottom - StageRectTop))) {
		lDrawStageSizeX =(StageRectRight - StageRectLeft);
		lDrawStageSizeY = static_cast<long>(((StageRectRight - StageRectLeft) * dStageMaxSizeY / dStageMaxSizeX));

		m_dDrawStartStagePosX = StageRectLeft;
		m_dDrawStartStagePosY = StageRectTop + ((StageRectBottom - StageRectTop - lDrawStageSizeY) / 2);
		m_dDrawEndStagePosX = StageRectRight;
		m_dDrawEndStagePosY = m_dDrawStartStagePosY + lDrawStageSizeY;
	}
	else{
		lDrawStageSizeX = static_cast<long>(((StageRectBottom - StageRectTop) * dStageMaxSizeX / dStageMaxSizeY));
		lDrawStageSizeY = static_cast<long>((StageRectBottom - StageRectTop));

		m_dDrawStartStagePosX = StageRectLeft + ((StageRectRight - StageRectLeft - lDrawStageSizeX) / 2);
		m_dDrawStartStagePosY = StageRectTop;
		m_dDrawEndStagePosX = m_dDrawStartStagePosX + lDrawStageSizeX;
		m_dDrawEndStagePosY = StageRectBottom;
	}

	pDC->Rectangle(CRect(static_cast<int>(m_dDrawStartStagePosX), static_cast<int>(m_dDrawStartStagePosY), static_cast<int>(m_dDrawEndStagePosX), static_cast<int>(m_dDrawEndStagePosY)));
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);

	///// Draw Sample /////
	DrawSample(pDC, m_StageConfig.OrgPos, m_StageConfig.Dir.X, m_StageConfig.Dir.Y);

// 2013.01.09 bagus -->
// 	///// Draw Origin Point /////
// 	DrawOriginMaker(m_StageConfig.OrgPos);
// 2013.01.09 bagus <--

	///// Draw AxisCoordinate /////
	DrawAxisCoordinate(m_StageConfig.OrgPos, m_StageConfig.Dir.X, m_StageConfig.Dir.Y);

	SamplePointGraph_DataSet();

// ÔøΩ`ÔøΩÔøΩpÔøΩÔøΩÔøΩbÔøΩZÔøΩ[ÔøΩWÔøΩ∆ÇÔøΩÔøΩÔøΩ CNanoRecipeUI::OnPaint() ÔøΩÔøΩÔøΩƒÇ—èoÔøΩÔøΩÔøΩƒÇÕÇÔøΩÔøΩÔøΩÔøΩ‹ÇÔøΩÔøΩÔøΩ
	ReleaseDC(pDC);
}

// 2013.01.09 bagus -->
// ÔøΩÔøΩÔøΩÔøΩÔøΩWÔøΩiSTAGEAPIÔøΩpÔøΩjÔøΩ…ïœäÔøΩ
void CRecipeGantryStageProgramView::ToXyAxisCoord(STAGE_COORD* lpPoint)
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

// ==========================================================================
//
void CRecipeGantryStageProgramView::DrawSample(CDC* pDC, int iOrg, int DirX, int DirY)
{
	WINDOWPLACEMENT Stageplacement, Sampleplacement;

	///// Region Map /////
	GetDlgItem(IDC_MEASUREMENT_MAP)->GetWindowPlacement(&Stageplacement);

	///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩLÔøΩÔøΩÔøΩÕàÔøΩ(ÔøΩÔøΩÔøΩÔøΩÔøΩlÔøΩpÔøΩÕÇÔøΩÔøΩÃÉTÔøΩCÔøΩYÔøΩ≈ï`ÔøΩÊÇ≥ÔøΩÔøΩƒÇÔøΩÔøΩÔøΩ) /////
// 2009.12.09 bagus --{--
//	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
	double dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
// 2009.12.09 bagus --}--

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

	///// ÔøΩ@ÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩTÔøΩCÔøΩYÔøΩÔøΩÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩÃëÂÇ´ÔøΩÔøΩÔøΩ∆ÇËè¨ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ`ÔøΩFÔøΩbÔøΩN /////
// 	if ((m_StageProgInfoHdr.SampleInfo.Size.dx > dStageMaxSizeX) || (m_StageProgInfoHdr.SampleInfo.Size.dy > dStageMaxSizeY))
// 		return;
	if ((dSampleSizeX > dStageMaxSizeX) || (dSampleSizeY > dStageMaxSizeY))
		return;
// 2013.01.09 bagus <--

	//ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩÔøΩOrigin PositionÔøΩÔøΩÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩkÔøΩÔøΩÔøΩÔøΩÔøΩ…çÔøΩÔøΩÌÇπÔøΩÔøΩÔøΩÔøΩÔøΩWÔøΩ…ïœäÔøΩ

	///// ÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩ`ÔøΩÔøΩTÔøΩCÔøΩYÔøΩÃåvÔøΩZÔøΩiÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩ`ÔøΩÔøΩTÔøΩCÔøΩYÔøΩŒÇÔøΩÔøΩÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩ`ÔøΩÔøΩTÔøΩCÔøΩYÔøΩÔøΩÔøΩvÔøΩZÔøΩj /////
	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX) * m_StageProgInfoHdr.SampleInfo.Size.dx / dStageMaxSizeX;
	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY) * m_StageProgInfoHdr.SampleInfo.Size.dy / dStageMaxSizeY;

	///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩ∆ÉTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÃåÔøΩÔøΩ_ÔøΩIÔøΩtÔøΩZÔøΩbÔøΩgÔøΩÃçÔøΩÔøΩvÔøΩl /////
	double dTotalOrgOffsetPosX, dTotalOrgOffsetPosY;

	///// ÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÃï`ÔøΩÔøΩ íu /////
	double dtop, dbottom, dright, dleft;

// 2013.01.09 bagus -->
	STAGE_COORD XyAxisCoord;

	XyAxisCoord.lX = m_StageProgInfoHdr.SampleInfo.Origin.lX;
	XyAxisCoord.lY = m_StageProgInfoHdr.SampleInfo.Origin.lY;
	ToXyAxisCoord(&XyAxisCoord);
// 2013.01.09 bagus <--

	//SampleÔøΩ`ÔøΩÔøΩ íuÔøΩ›íÔøΩ
	//ÔøΩlÔøΩpÔøΩ`ÔøΩÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÃÇ›ëŒâÔøΩ
	switch(m_StageProgInfoHdr.SampleInfo.nShape){
	case SAMPLE_SHAPE_RECTANGLE:		//Sample shaped RectAngle
		switch(iOrg){
		case 1: ///// ÔøΩZÔøΩÔøΩÔøΩ^ÔøΩ[ÔøΩÓèÄ	//XÔøΩÔøΩÔøΩFÔøΩÔøΩ+ÔøΩ@YÔøΩÔøΩÔøΩFÔøΩÔøΩ+ÔøΩÃÇ›ëŒâÔøΩ
		default:
			///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩ∆ÉTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÃåÔøΩÔøΩ_ÔøΩIÔøΩtÔøΩZÔøΩbÔøΩgÔøΩÃçÔøΩÔøΩvÔøΩlÔøΩÔøΩÔøΩvÔøΩZ /////
// 2013.01.09 bagus -->
// 2009.12.09 bagus --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX) - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY) - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.09 bagus --}--
			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX) + (XyAxisCoord.lX) - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY) + (XyAxisCoord.lY) - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus <--

			///// ÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩ`ÔøΩÔøΩ íuÔøΩÔøΩÔøΩvÔøΩZ /////
			//ÔøΩÔøΩÔøΩÓèÄ
			dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX / 2);
			dright	= dleft + dDrawSampleSizeX;
			//ÔøΩÔøΩÔøΩÓèÄ
			dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + (dDrawSampleSizeY / 2);
			dtop	= dbottom - dDrawSampleSizeY;
			break;

		case 2: /////LeftTop
			///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩ∆ÉTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÃåÔøΩÔøΩ_ÔøΩIÔøΩtÔøΩZÔøΩbÔøΩgÔøΩÃçÔøΩÔøΩvÔøΩlÔøΩÔøΩÔøΩvÔøΩZ /////
// 2013.01.09 bagus -->
// 2009.10.05 bagus SE --{--
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --}--
			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus <--

			///// ÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩ`ÔøΩÔøΩ íuÔøΩÔøΩÔøΩvÔøΩZ /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom	= dtop + dDrawSampleSizeY;
			break;
		case 3: /////LeftBottom
			///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩ∆ÉTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÃåÔøΩÔøΩ_ÔøΩIÔøΩtÔøΩZÔøΩbÔøΩgÔøΩÃçÔøΩÔøΩvÔøΩlÔøΩÔøΩÔøΩvÔøΩZ /////
// 2013.01.09 bagus -->
// 2009.10.05 bagus SE --{--
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.09 bagus --{--
//			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = ((m_StageProgInfoHdr.SampleInfo.Origin.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX);
// 			dTotalOrgOffsetPosY = ((m_StageProgInfoHdr.SampleInfo.Origin.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY);
// 2009.12.09 bagus --}--
// 2009.10.05 bagus SE --}--
			dTotalOrgOffsetPosX = ((XyAxisCoord.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX);
			dTotalOrgOffsetPosY = ((XyAxisCoord.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY);
// 2013.01.09 bagus <--

			///// ÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩ`ÔøΩÔøΩ íuÔøΩÔøΩÔøΩvÔøΩZ /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
			break;
		case 4: /////RightBottom
			///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩ∆ÉTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÃåÔøΩÔøΩ_ÔøΩIÔøΩtÔøΩZÔøΩbÔøΩgÔøΩÃçÔøΩÔøΩvÔøΩlÔøΩÔøΩÔøΩvÔøΩZ /////
// 2013.01.09 bagus -->
// 2009.10.05 bagus SE --{--
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --}--
			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus <--

			///// ÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩ`ÔøΩÔøΩ íuÔøΩÔøΩÔøΩvÔøΩZ /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
			break;
		case 5: /////RightTop
			///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩ∆ÉTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩÃåÔøΩÔøΩ_ÔøΩIÔøΩtÔøΩZÔøΩbÔøΩgÔøΩÃçÔøΩÔøΩvÔøΩlÔøΩÔøΩÔøΩvÔøΩZ /////
// 2013.01.09 bagus -->
// 2009.10.05 bagus SE --{--
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --}--
			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus <-->

			///// ÔøΩTÔøΩÔøΩÔøΩvÔøΩÔøΩÔøΩ`ÔøΩÔøΩ íuÔøΩÔøΩÔøΩvÔøΩZ /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom	= dtop + dDrawSampleSizeY;
			break;
		}

		if ((m_dDrawStartStagePosX <= dleft) && (dright <= m_dDrawEndStagePosX) && (m_dDrawStartStagePosY <= dtop) && (dbottom <= m_dDrawEndStagePosY)){
			Sampleplacement.rcNormalPosition.left = static_cast<long>(Stageplacement.rcNormalPosition.left + dleft);
			Sampleplacement.rcNormalPosition.right = static_cast<long>(Stageplacement.rcNormalPosition.left + dright);
			Sampleplacement.rcNormalPosition.top = static_cast<long>(Stageplacement.rcNormalPosition.top  + dtop);
			Sampleplacement.rcNormalPosition.bottom = static_cast<long>(Stageplacement.rcNormalPosition.top  + dbottom);
			//ÔøΩlÔøΩpSampleÔøΩ`ÔøΩÔøΩ
			m_SamplePointGraph.SetWindowPlacement(&Sampleplacement);
			m_SamplePointGraph.ShowWindow(SW_SHOW);
		}
		else{
			m_SamplePointGraph.ShowWindow(SW_HIDE);
		}

	}
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);
}

// ==========================================================================
//
void CRecipeGantryStageProgramView::DrawAxisCoordinate(int iOrgPos, int DirX, int DirY)
{
#define ARROW_LENGTH	18
#define ARROW_SIDE	6

	POINT PT[3];
	CString csBuff;
	CString strBuffer;

	CWnd* CoordBox = GetDlgItem(IDC_COORDINATE);
	CDC* pDC = CoordBox->GetDC();
	CRect StageRect;
	CoordBox->GetClientRect(&StageRect);

	CBrush BrushStage(WAFER_COLOR);
	CBrush BrushArrow(BLACK_COLOR);
	CBrush BrushOrg(GREEN_COLOR);
	CPen PenStage(PS_SOLID, 1, WAFER_COLOR);
	CPen PenArrow(PS_SOLID, 1, BLACK_COLOR);
	CPen PenOrg(PS_SOLID, 1, GREEN_COLOR);

	CBrush* oldBrush = pDC->SelectObject(&BrushStage);
	CPen* oldPen = pDC->SelectObject(&PenStage);

	long StageRectLeft = StageRect.left + STAGE_OFFSET;
	long StageRectRight = StageRect.right - STAGE_OFFSET;
	long StageRectTop = StageRect.top + STAGE_OFFSET;
	long StageRectBottom = StageRect.bottom - STAGE_OFFSET;

	///// Draw Stage /////
	pDC->Rectangle(CRect(StageRectLeft, StageRectTop, StageRectRight , StageRectBottom));

	///// Font Setting /////
	TEXTMETRIC tm;
	GetTextMetrics(pDC->GetSafeHdc(), &tm);
	if( pDC->SelectObject( CFont::FromHandle((HFONT) GetStockObject(ANSI_FIXED_FONT) ) ) == NULL ){
		LoadStringML(IDS_ERROR_SELECT_OBJECT, strBuffer, "Error in SelectObject");
		MessageBox(strBuffer);
	}

	///// ÔøΩwÔøΩiÔøΩFÔøΩìßñÔøΩÔøΩ…ê›íÔøΩ /////
	pDC->SetBkMode(TRANSPARENT);

	///// Draw Center Position /////
	long CenterPosX = StageRectLeft + ((StageRectRight - StageRectLeft) / 2);
	long CenterPosY = StageRectTop + ((StageRectBottom - StageRectTop) / 2);

	pDC->SelectObject(&BrushArrow);
	pDC->SelectObject(&PenArrow);
	///// Draw Arrows /////
	if(iOrgPos == 1){ //ÔøΩ\ÔøΩÔøΩÔøΩÔøΩ`ÔøΩÔøΩ
		pDC->MoveTo(CenterPosX - ARROW_LENGTH, CenterPosY);
		pDC->LineTo(CenterPosX + ARROW_LENGTH, CenterPosY);
		pDC->MoveTo(CenterPosX, CenterPosY + ARROW_LENGTH);
		pDC->LineTo(CenterPosX, CenterPosY - ARROW_LENGTH);
	}

	if(DirX == 1 || DirY == 1){ //ÔøΩEÔøΩÔøΩÔøΩÔøΩ
		pDC->MoveTo(CenterPosX, CenterPosY);
		pDC->LineTo(CenterPosX + ARROW_LENGTH, CenterPosY);
		PT[0].x = CenterPosX + ARROW_LENGTH;
		PT[0].y = CenterPosY;
		PT[1].x = CenterPosX + ARROW_LENGTH - ARROW_SIDE;
		PT[1].y = CenterPosY + ARROW_SIDE/2;
		PT[2].x = CenterPosX + ARROW_LENGTH - ARROW_SIDE;
		PT[2].y = CenterPosY - ARROW_SIDE/2;
		pDC->SetPolyFillMode(WINDING );
		pDC->Polygon( PT, 3 );
		if(DirX == 1)
			csBuff.Format("%s", "+X");
		else
			csBuff.Format("%s", "+Y");
		pDC->TextOut(PT[0].x + 5, PT[0].y - ARROW_SIDE, csBuff);
	}

	if(DirX == 2 || DirY == 2){ //ÔøΩÔøΩÔøΩÔøΩÔøΩ
		pDC->MoveTo(CenterPosX, CenterPosY);
		pDC->LineTo(CenterPosX, CenterPosY - ARROW_LENGTH);
		PT[0].x = CenterPosX;
		PT[0].y = CenterPosY - ARROW_LENGTH;
		PT[1].x = CenterPosX + ARROW_SIDE/2;
		PT[1].y = CenterPosY - ARROW_LENGTH + ARROW_SIDE;
		PT[2].x = CenterPosX - ARROW_SIDE/2;
		PT[2].y = CenterPosY - ARROW_LENGTH + ARROW_SIDE;
		pDC->SetPolyFillMode(WINDING );
		pDC->Polygon( PT, 3 );
		if(DirX == 2)
			csBuff.Format("%s", "+X");
		else
			csBuff.Format("%s", "+Y");
		pDC->TextOut(PT[0].x - ARROW_SIDE, PT[0].y - 15, csBuff);
	}

	if(DirX == 3 || DirY == 3){ //ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
		pDC->MoveTo(CenterPosX, CenterPosY);
		pDC->LineTo(CenterPosX - ARROW_LENGTH, CenterPosY);
		PT[0].x = CenterPosX - ARROW_LENGTH;
		PT[0].y = CenterPosY;
		PT[1].x = CenterPosX - ARROW_LENGTH + ARROW_SIDE;
		PT[1].y = CenterPosY - ARROW_SIDE/2;
		PT[2].x = CenterPosX - ARROW_LENGTH + ARROW_SIDE;
		PT[2].y = CenterPosY + ARROW_SIDE/2;
		pDC->SetPolyFillMode(WINDING );
		pDC->Polygon( PT, 3 );
		if(DirX == 3)
			csBuff.Format("%s", "+X");
		else
			csBuff.Format("%s", "+Y");
		pDC->TextOut(PT[0].x - 20, PT[0].y - ARROW_SIDE, csBuff);
	}
	if(DirX == 4 || DirY == 4){ //ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
		pDC->MoveTo(CenterPosX, CenterPosY);
		pDC->LineTo(CenterPosX, CenterPosY + ARROW_LENGTH);
		PT[0].x = CenterPosX;
		PT[0].y = CenterPosY + ARROW_LENGTH;
		PT[1].x = CenterPosX - ARROW_SIDE/2;
		PT[1].y = CenterPosY + ARROW_LENGTH - ARROW_SIDE;
		PT[2].x = CenterPosX + ARROW_SIDE/2;
		PT[2].y = CenterPosY + ARROW_LENGTH - ARROW_SIDE;
		pDC->SetPolyFillMode(WINDING );
		pDC->Polygon( PT, 3 );
		if(DirX == 4)
			csBuff.Format("%s", "+X");
		else
			csBuff.Format("%s", "+Y");
		pDC->TextOut(PT[0].x - ARROW_SIDE, PT[0].y + 5, csBuff);
	}

	pDC->SelectObject(&BrushOrg);
	pDC->SelectObject(&PenOrg);
	pDC->Ellipse((CenterPosX - ELLIPSE_RADIUS), (CenterPosY - ELLIPSE_RADIUS), (CenterPosX + ELLIPSE_RADIUS), (CenterPosY + ELLIPSE_RADIUS));

	///// Put back color /////
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);

	ReleaseDC(pDC);
}

// 2013.01.09 bagus -->
// // ==========================================================================
// //
// void CRecipeGantryStageProgramView::DrawOriginMaker(int Origin)
// {
// 	long OriginX, OriginY;
//
// 	CWnd* pPictBox = GetDlgItem(IDC_MEASUREMENT_MAP);
// 	CDC* pDC = pPictBox->GetDC();
// 	CRect StageRect;
// 	pPictBox->GetClientRect(&StageRect);
//
// 	CBrush BrushRed(GREEN_COLOR);
// 	CPen PenRed(PS_SOLID, 1, GREEN_COLOR);
// 	pDC->SelectObject(&BrushRed);
// 	pDC->SelectObject(&PenRed);
//
// 	///// ÔøΩXÔøΩeÔøΩ[ÔøΩWÔøΩLÔøΩÔøΩÔøΩÕàÔøΩ /////
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

// =========================================================================
//
BOOL CRecipeGantryStageProgramView::IsAccessPrivilege()
{
	///// User Access Privilege /////
	return m_pDoc->User_Access(ACCESS_PROGRAM);
}

// =========================================================================
// ÔøΩfÔøΩ[ÔøΩ^ÔøΩjÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩCleanUpÔøΩÔøΩÔøΩÔøΩ
void CRecipeGantryStageProgramView::Discard()
{
	char szFilePath[MAX_PATH + 1];
	char szFilePathBackUp[MAX_PATH + 1];

	// ÔøΩpÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩFÔøΩÔøΩÔøΩÊëúÔøΩiÔøΩﬁîÔøΩÔøΩjÔøΩÃñﬂÇÔøΩ
	_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
	_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
	::CopyFile(szFilePathBackUp, szFilePath, FALSE);
	_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
	_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
	::CopyFile(szFilePathBackUp, szFilePath, FALSE);
}

// =========================================================================
//
int CRecipeGantryStageProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_STAGE_PROGRAM;
}

// =========================================================================
//
void CRecipeGantryStageProgramView::SamplePointGraph_Init()
{
	CString strBuffer;
	CNanoPointGraph::GraphConfig tConfig;

	m_SamplePointGraph.ShowWindow(SW_HIDE);

	////Point Graph Data Reset
	m_SamplePointGraph.Reset();

	////Back Color
	tConfig.BackColor = WAFER_COLOR;//WATER_COLOR;//WHITE_COLOR;
	////Edge Color
	tConfig.EdgeColor[CNanoPointGraph::EDGE_LOWER] = WHITE_COLOR;
	tConfig.EdgeColor[CNanoPointGraph::EDGE_UPPER] = RGB(0x00,0x00,0x00);
	////Status Color
	tConfig.StatusColor[CNanoPointGraph::NOT_MEASURED] = RGB(0x00,0x00,0xFF);
	tConfig.StatusColor[CNanoPointGraph::MEASURING] = RGB(0x00,0xFF,0x55);
	tConfig.StatusColor[CNanoPointGraph::SELECTED] = RGB(0xFF,0x00,0x00);
	tConfig.StatusColor[CNanoPointGraph::MEASURED] = RGB(0x77,0x77,0x77);

	// 2013.01.09 bagus -->
	tConfig.OriginColor = GREEN_COLOR;
	// 2013.01.09 bagus -->

	////CNanoPointGraphÔøΩNÔøΩÔøΩÔøΩXÔøΩÔøΩÔøΩTÔøΩCÔøΩY(Sample Size)
	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
		// X<->YÔøΩÔøΩÔøΩÔøΩ÷ÇÔøΩ
		tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
		tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
		// X-YÔøΩÔøΩÔøΩÃÇ‹ÇÔøΩ
		tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	} else {
		// ÔøΩÔøΩÔøΩ›íËÇ™ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
	}
	m_SamplePointGraph.SetConfig(&tConfig);

	SamplePointGraph_DataSet();
}

// =========================================================================
//
void CRecipeGantryStageProgramView::SamplePointGraph_DataSet()
{
	////Point Graph Data Reset
	m_SamplePointGraph.Reset();

	////Point Data
	//ÔøΩZÔøΩÔøΩÔøΩ^ÔøΩ[ÔøΩÔøΩÔøΩ_ÔøΩÃèÍçá
	double dOffSetValueX;
	double dOffSetValueY;
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
		dOffSetValueX = (m_StageProgInfoHdr.SampleInfo.Size.dx/2);
		dOffSetValueY = (m_StageProgInfoHdr.SampleInfo.Size.dy/2);
		break;
	case 2:
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		break;
	case 3:
		dOffSetValueX = 0;
		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
		break;
	case 4:
		dOffSetValueX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
		break;
	case 5:
		dOffSetValueX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dOffSetValueY = 0;
		break;
	}

// 2013.01.09 bagus -->
	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOffSetValueX), static_cast<long>(dOffSetValueY));
	}
	else {
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOffSetValueY), static_cast<long>(dOffSetValueX));
	}
// 2013.01.09 bagus <--

	double Scan_PointIncOffsetX,  Scan_PointIncOffsetY;
	for(int iPoint= 0; iPoint<m_StageProgInfoHdr.wNumScans; iPoint++){
		switch (m_StageConfig.Dir.X) {
		case 1:		// X:ÔøΩÔøΩ
		default:
			Scan_PointIncOffsetX = dOffSetValueX + m_ScanPoint[iPoint].lX;
			break;
		case 2:		// X:ÔøΩÔøΩ
			Scan_PointIncOffsetY = dOffSetValueX - m_ScanPoint[iPoint].lX;
			break;
		case 3:		// X:ÔøΩÔøΩ
			Scan_PointIncOffsetX = dOffSetValueX - m_ScanPoint[iPoint].lX;
			break;
		case 4:		// X:ÔøΩÔøΩ
			Scan_PointIncOffsetY = dOffSetValueX + m_ScanPoint[iPoint].lX;
			break;
		}

		switch (m_StageConfig.Dir.Y) {
		case 1:		// Y:ÔøΩÔøΩ
			Scan_PointIncOffsetX = dOffSetValueY + m_ScanPoint[iPoint].lY;
			break;
		case 2:		// Y:ÔøΩÔøΩ
		default:
			Scan_PointIncOffsetY = dOffSetValueY - m_ScanPoint[iPoint].lY;
			break;
		case 3:		// Y:ÔøΩÔøΩ
			Scan_PointIncOffsetX = dOffSetValueY - m_ScanPoint[iPoint].lY;
			break;
		case 4:		// Y:ÔøΩÔøΩ
			Scan_PointIncOffsetY = dOffSetValueY + m_ScanPoint[iPoint].lY;
			break;
		}
		m_SamplePointGraph.AddPoint(static_cast<long>(Scan_PointIncOffsetX), static_cast<long>(Scan_PointIncOffsetY));
	}

	///// Point Number /////
	TCHAR szBuff[256];
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);
}

void CRecipeGantryStageProgramView::OnTimer(UINT nIDEvent)
{
	// TODO: ÔøΩÔøΩÔøΩÃà íuÔøΩ…ÉÔøΩÔøΩbÔøΩZÔøΩ[ÔøΩW ÔøΩnÔøΩÔøΩÔøΩhÔøΩÔøΩÔøΩpÔøΩÃÉRÔøΩ[ÔøΩhÔøΩÔøΩ«âÔøΩÔøΩÔøΩÔøΩÈÇ©ÔøΩ‹ÇÔøΩÔøΩÕÉfÔøΩtÔøΩHÔøΩÔøΩÔøΩgÔøΩÃèÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩƒÇ—èoÔøΩÔøΩÔøΩƒÇÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ

	CheckMeasurementPointData();

	CNanoRecipeUI::OnTimer(nIDEvent);
}

BOOL CRecipeGantryStageProgramView::CheckMeasurementPointData()
{
	GV_ITEM Item;

	int	i;

	int FixRows = 1;
	int FixCols = 1;
	long	lLineValid;

	TCHAR szBuff[256];

	long	lValid = 0;

	BOOL	bCheckChanged = FALSE;

	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));

	for (i=0; i<m_SrConfig.wGantryNumOfMeasPoint; i++) {
		Item.mask = GVIF_TEXT;
		Item.row = i+FixRows;
		Item.col = 3;
		m_MeasurementPointGrid.GetItem(&Item);
		lLineValid = ((CGridCellCheck *)m_MeasurementPointGrid.GetCell(i+FixRows, 3))->GetCheck();
		if (lLineValid <= 0) {
			if(m_ValidPoint[i]){	bCheckChanged = TRUE; }
			m_ValidPoint[i] = FALSE;
		} else {
			if(!m_ValidPoint[i]){	bCheckChanged = TRUE; }
			m_ValidPoint[i] = TRUE;

			m_ScanPoint[lValid].lX = m_SrReferencePosition.LocGantryTransmitReference[i].lX;
			m_ScanPoint[lValid].lY = m_SrReferencePosition.LocGantryTransmitReference[i].lY;
			lValid++;
		}
	}

	m_iNumScans = lValid;
	m_StageProgInfoHdr.wNumScans = m_iNumScans;
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);

	if(bCheckChanged){
		m_SamplePointGraph.Invalidate();
		Invalidate(TRUE);
	}

	return ( TRUE );
}

void CRecipeGantryStageProgramView::MeasurementPointListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 3; //ÔøΩLÔøΩÔøΩColÔøΩÔøΩ

	int FixRows = 1;
	int FixCols = 1;
	int Cols = iDispCol + FixCols;
	int Rows = FixRows;
	int	col;
	DWORD dwTextStyle;

	char* ColItem_ENU[]={
		"Point",
		"X (mm)",
		"Y (mm)",
		"Valid",
	};
	char* ColItem_JPN[]={
		"Point",
		"X (mm)",
		"Y (mm)",
		"Valid",
	};

	///// Grid Initialize /////
	m_MeasurementPointGrid.SetEditable(FALSE);
	m_MeasurementPointGrid.EnableDragAndDrop(FALSE);
	m_MeasurementPointGrid.SetTextBkColor(WHITE_COLOR);
	m_MeasurementPointGrid.SetRowResize(0);

	m_MeasurementPointGrid.EnableSelection(TRUE);
	m_MeasurementPointGrid.SetListMode(FALSE);
	m_MeasurementPointGrid.SetTrackFocusCell(FALSE);
	m_MeasurementPointGrid.SetFrameFocusCell(FALSE);
	m_MeasurementPointGrid.SetFixedColumnSelection(TRUE);
	m_MeasurementPointGrid.SetFixedRowSelection(TRUE);

	TRY{
		m_MeasurementPointGrid.SetRowCount(Rows);
		m_MeasurementPointGrid.SetColumnCount(Cols);
		m_MeasurementPointGrid.SetFixedRowCount(FixRows);
		m_MeasurementPointGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	CWnd* GridBox = GetDlgItem(IDC_MEASUREMENT_POINT_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);

	// Row 0 Height
	m_MeasurementPointGrid.SetRowHeight(0, 18);

	// Col 0 Width
	m_MeasurementPointGrid.SetColumnWidth(0, nFixColWidth);	// No.

	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_MeasurementPointGrid.GetColumnCount(); col++)
	{
		m_MeasurementPointGrid.SetColumnWidth(col, nColWidth);
	}

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_MeasurementPointGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s",ColItem_ENU[col]);
		m_MeasurementPointGrid.SetItem(&Item);
	}
}

void CRecipeGantryStageProgramView::SetMeasurementPointData()
{
	MeasurementPointListGrid_Init();

	GV_ITEM Item;

//	int	i ,j;
	int	i;
	BOOL	bExist;

	int FixRows = 1;
	int FixCols = 1;
	int	row;
	DWORD	dwTextStyle;
	int nRowCount = m_SrConfig.wGantryNumOfMeasPoint;

	TRY{
		if ((nRowCount + FixRows) < DEFAULT_GRID_ROWS) {
			m_MeasurementPointGrid.SetRowCount(DEFAULT_GRID_ROWS);
		} else {
			m_MeasurementPointGrid.SetRowCount(nRowCount + FixRows);
		}
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	row = FixRows;
	for (i=0; i<m_SrConfig.wGantryNumOfMeasPoint; i++) {
		dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = i+FixRows;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%d", i+1);
		m_MeasurementPointGrid.SetItem(&Item);

		dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = i+FixRows;
		Item.col = 1;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%.3f", (double)(m_SrReferencePosition.LocGantryTransmitReference[i].lX) / (double)(MICROMETRE));			// ÔøΩPÔøΩ ïœäÔøΩ(um->mm)
		m_MeasurementPointGrid.SetItem(&Item);

		dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = i+FixRows;
		Item.col = 2;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%.3f", (double)(m_SrReferencePosition.LocGantryTransmitReference[i].lY) / (double)(MICROMETRE));			// ÔøΩPÔøΩ ïœäÔøΩ(um->mm)
		m_MeasurementPointGrid.SetItem(&Item);

		dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = i+FixRows;
		Item.col = 3;
		Item.nFormat = dwTextStyle;
		m_MeasurementPointGrid.SetCellType(i+1, 3, RUNTIME_CLASS(CGridCellCheck));
#if 0
		bExist = FALSE;
		for (j=0; j<m_SrConfig.wGantryNumOfMeasPoint; j++) {
			if ((m_SrReferencePosition.LocGantryTransmitReference[i].lX == m_ScanPoint[j].lX)
			 && (m_SrReferencePosition.LocGantryTransmitReference[i].lY == m_ScanPoint[j].lY)) {
				bExist = TRUE;
				break;
			}
		}
#else
		bExist = m_ValidPoint[i];
#endif
		((CGridCellCheck *)m_MeasurementPointGrid.GetCell(i+1, 3))->SetCheck(bExist);
	}

	SamplePointGraph_DataSet();
}

void CRecipeGantryStageProgramView::CheckValidPoint()
{
	int	i, j;
	BOOL	bExist;

	for (i=0; i<m_SrConfig.wGantryNumOfMeasPoint; i++) {
		bExist = FALSE;
// 2009.10.13 bagus StagePGM ÔøΩCÔøΩÔøΩ --{--
//		for (j=0; j<m_SrConfig.wGantryNumOfMeasPoint; j++) {
		for (j=0; j<m_StageProgInfoHdr.wNumScans; j++) {
// 2009.10.13 bagus StagePGM ÔøΩCÔøΩÔøΩ --}--
			if ((m_SrReferencePosition.LocGantryTransmitReference[i].lX == m_ScanPoint[j].lX)
			 && (m_SrReferencePosition.LocGantryTransmitReference[i].lY == m_ScanPoint[j].lY)) {
				bExist = TRUE;
				break;
			}
		}
		m_ValidPoint[i] = bExist;
	}
}

// 2010.01.29 bagus Gantry ÔøΩCÔøΩÔøΩ --{--
// =========================================================================
// OnSelChangeing IDC_MEASUREMENT_POINT_GRIDÔøΩIÔøΩÔøΩÔøΩsÔøΩÃÉXÔøΩLÔøΩÔøΩÔøΩÔøΩÔøΩ|ÔøΩCÔøΩÔøΩÔøΩgÔøΩ…Ç¬ÇÔøΩÔøΩƒÅAÔøΩ}ÔøΩbÔøΩvÔøΩÃÉXÔøΩLÔøΩÔøΩÔøΩÔøΩÔøΩ|ÔøΩCÔøΩÔøΩÔøΩgÔøΩÔøΩIÔøΩÔøΩFÔøΩ…ê›íËÇ∑ÔøΩÔøΩ
void CRecipeGantryStageProgramView::OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	///// Draw SelectPoint /////
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID CellID;
	CellID.row=pItem->iRow;
	CellID.col=pItem->iColumn;

	if ( !m_MeasurementPointGrid.IsValid(CellID) )
		return;

	//PointGraphÔøΩIÔøΩÔøΩÔøΩ|ÔøΩCÔøΩÔøΩÔøΩgÔøΩFÔøΩœçX
	m_SamplePointGraph.Select(CellID.row-1);
}

// ==========================================================================
// OnSelChanged IDC_MEASUREMENT_POINT_GRIDÔøΩIÔøΩÔøΩÔøΩsÔøΩÃÉXÔøΩLÔøΩÔøΩÔøΩÔøΩÔøΩ“èWÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
void CRecipeGantryStageProgramView::OnSelChanged(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	/*
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID CellID;
	CellID.row=pItem->iRow;
	CellID.col=pItem->iColumn;

	if ( !m_MeasurementPointGrid.IsValid(CellID) )
		return;
	*/
}
// 2010.01.29 bagus Gantry ÔøΩCÔøΩÔøΩ --}--
