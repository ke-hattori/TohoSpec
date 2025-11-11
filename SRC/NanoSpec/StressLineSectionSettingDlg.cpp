// StressLineSectionSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "Math.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SharedMemory.h"
#include "NanoSpecDoc.h"
#include "StressLineSectionSettingDlg.h"
#include "BtnSt.h"
#include "GridCellNumeric.h"
#include "GridCellCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STAGE_OFFSET		15
#define RADIUS				1
#define ELLIPSE_RADIUS		2//4

//Saiki 20110207 Delete ----->
//#define	PIN_INTERVAL		7
//Saiki 20110207 Delete <-----

// 2009.09.17 bagus stress Grid修正 --{--
//2009.09.24 bagus stress 修正 --{--
//#define	DEFAULT_GRID_ROWS			30			// グリッドの基本行数
#define	GRID_ROW_HEIGHT			18					// グリッドの１行の高さ
//2009.09.24 bagus stress 修正 --}--
// 2009.09.17 bagus stress Grid修正 --}--

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

/////////////////////////////////////////////////////////////////////////////
// CStressLineSectionSettingDlg ダイアログ


CStressLineSectionSettingDlg::CStressLineSectionSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStressLineSectionSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStressLineSectionSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_strSampleShape = _T("");
	m_strSampleSizeX = _T("");
	m_strSampleSizeY = _T("");
	m_strSampleOriginX = _T("");
	m_strSampleOriginY = _T("");

	m_strStressValidRange = _T("");
	m_strInvalidRangeEdge = _T("");
	m_strMeasPointOffsetX = _T("");
	m_strMeasPointOffsetY = _T("");

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;

	memset(m_bOldLineValid,NULL,sizeof(m_bOldLineValid));

// 2009.09.17 bagus stress Grid修正 --{--
	m_nSysVScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
// 2009.09.17 bagus stress Grid修正 --}--
}

CStressLineSectionSettingDlg::~CStressLineSectionSettingDlg()
{
}

void CStressLineSectionSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressLineSectionSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		DDX_Control(pDX, IDC_LINE_LIST_GRID, m_LineListGrid);
		DDX_Control(pDX, IDC_PIN_LIST_GRID, m_PinListGrid);
		DDX_Control(pDX, IDC_SET_LINE_VALID_LIST_GRID, m_LineValidListGrid);
		DDX_Control(pDX, IDC_STRESS_MEASUREMENT_LINE_SECTION_LIST_GRID, m_StressMeasurementLineSectionListGrid);
		DDX_Control(pDX, IDC_NECESSARY_THICKNESS_MEASUREMENT_LIST_GRID, m_NecessaryThicknessMeasurementListGrid);
		DDX_Control(pDX, IDC_MEASUREMENT_POINT_MAP, m_SamplePointGraph);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_GRIDSET_BUTTON, m_GridSetButton);
	DDX_Control(pDX, IDC_SETLINESECTION_BUTTON, m_SetLineSectionButton);
// 2009.09.04 bagus stress ライン数設定ボタン追加 --{--
	DDX_Control(pDX, IDC_LINESET_BUTTON, m_LineSetButton);
// 2009.09.04 bagus stress ライン数設定ボタン追加 --}--

	DDX_Text(pDX, IDC_SAMPLE_SHAPE, m_strSampleShape);
	DDX_Text(pDX, IDC_SAMPLE_SIZE_X, m_strSampleSizeX);
	DDX_Text(pDX, IDC_SAMPLE_SIZE_Y, m_strSampleSizeY);
	DDX_Text(pDX, IDC_SAMPLE_ORIGIN_X, m_strSampleOriginX);
	DDX_Text(pDX, IDC_SAMPLE_ORIGIN_Y, m_strSampleOriginY);

	DDX_Text(pDX, IDC_STRESS_VALID_RANGE, m_strStressValidRange);
	DDX_Text(pDX, IDC_INVALID_RANGE_EDGE, m_strInvalidRangeEdge);
	DDX_Text(pDX, IDC_THICK_MEASUREMENT_POINT_OFFSET_X, m_strMeasPointOffsetX);
	DDX_Text(pDX, IDC_THICK_MEASUREMENT_POINT_OFFSET_Y, m_strMeasPointOffsetY);

}

BEGIN_MESSAGE_MAP(CStressLineSectionSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CStressLineSectionSettingDlg)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_NUMBER_OF_LINE, OnKillfocusNumberOfLine)
	ON_BN_CLICKED(IDC_GRIDSET_BUTTON, OnGridsetButton)
	ON_BN_CLICKED(IDC_SETLINESECTION_BUTTON, OnSetLineSectionButton)
	ON_EN_KILLFOCUS(IDC_INVALID_RANGE_EDGE, OnKillfocusInvalidRangeEdge)
	ON_EN_KILLFOCUS(IDC_THICK_MEASUREMENT_POINT_OFFSET_X, OnKillfocusThickMeasurementPointOffsetX)
	ON_BN_CLICKED(IDC_LINESET_BUTTON, OnLinesetButton)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStressLineSectionSettingDlg メッセージ ハンドラ

BOOL CStressLineSectionSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString	strBuffer;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	// TODO: この位置に初期化の補足処理を追加してください

	///// Load File /////
	memset(&m_StageConfig,NULL,sizeof(m_StageConfig));
	memset(&m_StressConfig,NULL,sizeof(m_StressConfig));
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);

	RecipeFile_LoadRecipe(&m_SampleInfo, m_szName, RECIPE_FILE_SAMPLE);
	//2009.12.23 bagus stress 修正 --{--
	_tcscpy(m_StressConfig.szSampleName,m_szName);
	//2009.12.23 bagus stress 修正 --}--

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Grid Set Button /////
	m_GridSetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_GridSetButton.DrawFlatFocus(TRUE);

	///// Set Line Section Button /////
	m_SetLineSectionButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SetLineSectionButton.DrawFlatFocus(TRUE);

	// 2009.09.04 bagus stress ライン数設定ボタン追加 --{--
	///// Grid Set Button /////
	m_LineSetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_LineSetButton.DrawFlatFocus(TRUE);
	// 2009.09.04 bagus stress ライン数設定ボタン追加 --}--

	///// Selected Sample /////
	m_StressConfig.Size.dx = m_SampleInfo.Size.dx;
	m_StressConfig.Size.dy = m_SampleInfo.Size.dy;
	m_StressConfig.Origin.lX = m_SampleInfo.Origin.lX;
	m_StressConfig.Origin.lY = m_SampleInfo.Origin.lY;

	m_stcSampleShape.SubclassWindow(GetDlgItem(IDC_SAMPLE_SHAPE)->GetSafeHwnd());
	m_stcSampleShape.SetBkColor(WATER_COLOR);
	if ((m_SampleInfo.nShape < 0) || (m_SampleInfo.nShape >= SAMPLE_SHAPE_MAX)) {
		m_strSampleShape.Format("");
	} else {
		m_strSampleShape.Format("%s", g_lpszSampleShape_ENU[m_SampleInfo.nShape]);
	}
	m_stcSampleSizeX.SubclassWindow(GetDlgItem(IDC_SAMPLE_SIZE_X)->GetSafeHwnd());
	m_stcSampleSizeX.SetBkColor(WATER_COLOR);
	m_strSampleSizeX.Format("%.3f", (double)(m_StressConfig.Size.dx / MICROMETRE));							// 単位変換(um->mm)
	m_stcSampleSizeY.SubclassWindow(GetDlgItem(IDC_SAMPLE_SIZE_Y)->GetSafeHwnd());
	m_stcSampleSizeY.SetBkColor(WATER_COLOR);
	m_strSampleSizeY.Format("%.3f", (double)(m_StressConfig.Size.dy / MICROMETRE));							// 単位変換(um->mm)
	m_stcSampleOriginX.SubclassWindow(GetDlgItem(IDC_SAMPLE_ORIGIN_X)->GetSafeHwnd());
	m_stcSampleOriginX.SetBkColor(WATER_COLOR);
	m_strSampleOriginX.Format("%.3f", (double)(m_StressConfig.Origin.lX / MICROMETRE));						// 単位変換(um->mm)
	m_stcSampleOriginY.SubclassWindow(GetDlgItem(IDC_SAMPLE_ORIGIN_Y)->GetSafeHwnd());
	m_stcSampleOriginY.SetBkColor(WATER_COLOR);
	m_strSampleOriginY.Format("%.3f", (double)(m_StressConfig.Origin.lY / MICROMETRE));						// 単位変換(um->mm)

	///// Grid Control Initialize /////
	PinListGrid_Init();
	LineListGrid_Init();
	LineValidListGrid_Init();
	StressMeasurementLineSectionListGrid_Init();
	NecessaryThicknessMeasurementListGrid_Init();

	///// Stress Valid Range /////
//	m_strStressValidRange.Format("%f.3", (double)(m_StressConfig.lStressValidRange / MICROMETRE));			// 単位変換(um->mm)
	m_strStressValidRange.Format("%d", m_StressConfig.lStressValidRange / MICROMETRE);						// 単位変換(um->mm)

	///// Invalid Range /////
//	m_strInvalidRangeEdge.Format("%f.3", (double)m_StressConfig.lInvalidRangeEdge / MICROMETRE));			// 単位変換(um->mm)
	m_strInvalidRangeEdge.Format("%d", m_StressConfig.lInvalidRangeEdge / MICROMETRE);					// 単位変換(um->mm)

	///// Thick Measurement Point Offset /////
	m_strMeasPointOffsetX.Format("%ld", m_StressConfig.MeasOffset.lX / MICROMETRE);				// 単位変換(um->mm)
	m_strMeasPointOffsetY.Format("%ld", m_StressConfig.MeasOffset.lY / MICROMETRE);				// 単位変換(um->mm)

	///// Number of Line /////
	if (CheckStressConfig_NumOfLineData()) {
		strBuffer.Format("%d", m_StressConfig.dwLiftPinNumberOfLine);
		SetDlgItemText(IDC_NUMBER_OF_LINE, strBuffer);

		///// Line Data /////
		if (CheckStressConfig_LineData()) {
			SetLineData();
			SetLineValidData();

			///// Pin Data /////
			if (CheckStressConfig_PinData()) {
				SetPinData();

				SetStressMeasurementLineSectionData();
				SetNecessaryThicknessMeasurementData();
			}
		}
	}

	///// Measurement Point Graph /////
	m_SamplePointGraph.SetLineStatusMode(TRUE);
	SamplePointGraph_Init();

	UpdateData(FALSE);

	SetTimer(1,100,NULL);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CStressLineSectionSettingDlg::LineListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 1; //有効Col数

	int FixRows = 1;
	int FixCols = 1;
	int Cols = iDispCol + FixCols;
//	int Rows = 100 + FixRows;
	int Rows = FixRows;
// 2009.09.17 bagus stress Grid修正 --{--
//	int row,col;
//	int ItemWidth = 51;
	int	col;
// 2009.09.17 bagus stress Grid修正 --}--
	DWORD dwTextStyle;

	char* ColItem_ENU[]={
		"No.",
		"Pin",
	};
	char* ColItem_JPN[]={
		"No.",
		"ピン",
	};

	///// Grid Initialize /////
	m_LineListGrid.SetEditable(TRUE);
	m_LineListGrid.EnableDragAndDrop(FALSE);
	m_LineListGrid.SetTextBkColor(WHITE_COLOR);
	m_LineListGrid.SetRowResize(0);

	//2009.09.15 bagus stress --{--
	//m_LineListGrid.EnableSelection(TRUE);
	//m_LineListGrid.SetTrackFocusCell(TRUE);
	//m_LineListGrid.SetFrameFocusCell(TRUE);
	//m_LineListGrid.SetFixedColumnSelection(FALSE);
	//m_LineListGrid.SetFixedRowSelection(FALSE);

	m_LineListGrid.EnableSelection(TRUE);
	m_LineListGrid.SetTrackFocusCell(FALSE);
	m_LineListGrid.SetFrameFocusCell(FALSE);
	m_LineListGrid.SetFixedColumnSelection(TRUE);
	m_LineListGrid.SetFixedRowSelection(TRUE);
	m_LineListGrid.SetListMode(FALSE);
	//2009.09.15 bagus stress --}--

	TRY{
		m_LineListGrid.SetRowCount(Rows);
		m_LineListGrid.SetColumnCount(Cols);
		m_LineListGrid.SetFixedRowCount(FixRows);
		m_LineListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2009.09.17 bagus stress Grid修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_LINE_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
//2009.09.24 bagus stress 修正 --{--
#if 0
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);
// 2009.09.17 bagus stress Grid修正 --{--
#else
	GridBox->GetClientRect(&GridRect);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);

	GridBox->GetWindowRect(&GridRect);
	this->ScreenToClient(&GridRect);

	GridBox->MoveWindow(GridRect.left, GridRect.top, GridRect.Width(), (GRID_ROW_HEIGHT * 5)+4);

	ReleaseDC(pDC);
#endif
//2009.09.24 bagus stress 修正 --}--

	// Row 0 Height
	m_LineListGrid.SetRowHeight(0, GRID_ROW_HEIGHT);

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	// Row 1 -> RowCount Height
	for(row = 1 ; row < m_LineListGrid.GetRowCount() ; row++){
		m_LineListGrid.SetRowHeight(row, GRID_ROW_HEIGHT);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 0 Width
// 2009.09.17 bagus stress Grid修正 --{--
//	m_LineListGrid.SetColumnWidth(0, 51);	// No.
	m_LineListGrid.SetColumnWidth(0, nFixColWidth);	// No.
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_LineListGrid.GetColumnCount(); col++)
	{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_LineListGrid.SetColumnWidth(col, ItemWidth);
		m_LineListGrid.SetColumnWidth(col, nColWidth);
// 2009.09.17 bagus stress Grid修正 --}--
	}

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_LineListGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s",ColItem_ENU[col]);
		m_LineListGrid.SetItem(&Item);
	}

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	///// Title Row /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_LineListGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("Line%d", row);
		m_LineListGrid.SetItem(&Item);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	return;
}

void CStressLineSectionSettingDlg::PinListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 2; //有効Col数

	int FixRows = 1;
	int FixCols = 1;
	int Cols = iDispCol + FixCols;
//	int Rows = 100 + FixRows;
	int Rows = FixRows;
// 2009.09.17 bagus stress Grid修正 --{--
//	int row,col;
//	int ItemWidth = 75;
	int	col;
// 2009.09.17 bagus stress Grid修正 --}--
	DWORD dwTextStyle;

	char* ColItem_ENU[]={
		"Point",
		"X (mm)",
		"Y (mm)",
	};
	char* ColItem_JPN[]={
		"Point",
		"X (mm)",
		"Y (mm)",
	};

	///// Grid Initialize /////
	m_PinListGrid.SetEditable(TRUE);
	m_PinListGrid.EnableDragAndDrop(FALSE);
	m_PinListGrid.SetTextBkColor(WHITE_COLOR);
	m_PinListGrid.SetRowResize(0);

	//2009.09.15 bagus stress --{--
	//m_PinListGrid.EnableSelection(TRUE);
	//m_PinListGrid.SetTrackFocusCell(TRUE);
	//m_PinListGrid.SetFrameFocusCell(TRUE);
	//m_PinListGrid.SetFixedColumnSelection(FALSE);
	//m_PinListGrid.SetFixedRowSelection(FALSE);
	//m_PinListGrid.SetListMode(TRUE);

	m_PinListGrid.EnableSelection(TRUE);
	m_PinListGrid.SetListMode(FALSE);
	m_PinListGrid.SetTrackFocusCell(FALSE);
	m_PinListGrid.SetFrameFocusCell(FALSE);
	m_PinListGrid.SetFixedColumnSelection(TRUE);
	m_PinListGrid.SetFixedRowSelection(TRUE);
	//2009.09.15 bagus stress --}--

	TRY{
		m_PinListGrid.SetRowCount(Rows);
		m_PinListGrid.SetColumnCount(Cols);
		m_PinListGrid.SetFixedRowCount(FixRows);
		m_PinListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2009.09.17 bagus stress Grid修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_PIN_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
//2009.09.24 bagus stress 修正 --{--
#if 0
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);
// 2009.09.17 bagus stress Grid修正 --{--
#else
	GridBox->GetClientRect(&GridRect);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);

	GridBox->GetWindowRect(&GridRect);
	this->ScreenToClient(&GridRect);

	GridBox->MoveWindow(GridRect.left, GridRect.top, GridRect.Width(), (GRID_ROW_HEIGHT * 19)+4);

	ReleaseDC(pDC);
#endif
//2009.09.24 bagus stress 修正 --}--

	// Row 0 Height
	m_PinListGrid.SetRowHeight(0, GRID_ROW_HEIGHT);

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	// Row 1 -> RowCount Height
	for(row = 1 ; row < m_PinListGrid.GetRowCount() ; row++){
		m_PinListGrid.SetRowHeight(row, GRID_ROW_HEIGHT);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 0 Width
// 2009.09.17 bagus stress Grid修正 --{--
//	m_PinListGrid.SetColumnWidth(0, 50);	// No.
	m_PinListGrid.SetColumnWidth(0, nFixColWidth);	// No.
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_PinListGrid.GetColumnCount(); col++)
	{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_PinListGrid.SetColumnWidth(col, ItemWidth);
		m_PinListGrid.SetColumnWidth(col, nColWidth);
// 2009.09.17 bagus stress Grid修正 --}--
	}

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_PinListGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s",ColItem_ENU[col]);
		m_PinListGrid.SetItem(&Item);
	}

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	///// Title Row /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_PinListGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("LxPin%d", row);
		m_PinListGrid.SetItem(&Item);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	return;
}

void CStressLineSectionSettingDlg::StressMeasurementLineSectionListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 3; //有効Col数

	int FixRows = 1;
	int FixCols = 1;
	int Cols = iDispCol + FixCols;
//	int Rows = 100 + FixRows;
	int Rows = FixRows;
// 2009.09.17 bagus stress Grid修正 --{--
//	int row,col;
//	int ItemWidth = 75;
	int	col;
// 2009.09.17 bagus stress Grid修正 --}--
	DWORD dwTextStyle;

	char* ColItem_ENU[]={
		"LS No.",
		"X Start",
		"X End",
		"Y",
	};
	char* ColItem_JPN[]={
		"LS No.",
		"X Start",
		"X End",
		"Y",
	};

	///// Grid Initialize /////
	m_StressMeasurementLineSectionListGrid.SetEditable(FALSE);
	m_StressMeasurementLineSectionListGrid.EnableDragAndDrop(FALSE);
	m_StressMeasurementLineSectionListGrid.SetTextBkColor(WHITE_COLOR);
	m_StressMeasurementLineSectionListGrid.SetRowResize(0);

	//2009.09.15 bagus stress --{--
//	m_StressMeasurementLineSectionListGrid.EnableSelection(FALSE);
	//m_StressMeasurementLineSectionListGrid.SetTrackFocusCell(TRUE);
	//m_StressMeasurementLineSectionListGrid.SetFrameFocusCell(TRUE);
	//m_StressMeasurementLineSectionListGrid.SetFixedColumnSelection(FALSE);
	//m_StressMeasurementLineSectionListGrid.SetFixedRowSelection(FALSE);

	m_StressMeasurementLineSectionListGrid.EnableSelection(TRUE);
	m_StressMeasurementLineSectionListGrid.SetTrackFocusCell(FALSE);
	m_StressMeasurementLineSectionListGrid.SetFrameFocusCell(FALSE);
	m_StressMeasurementLineSectionListGrid.SetFixedColumnSelection(TRUE);
	m_StressMeasurementLineSectionListGrid.SetFixedRowSelection(TRUE);
	m_StressMeasurementLineSectionListGrid.SetListMode(FALSE);
	//2009.09.15 bagus stress --}--

	TRY{
		m_StressMeasurementLineSectionListGrid.SetRowCount(Rows);
		m_StressMeasurementLineSectionListGrid.SetColumnCount(Cols);
		m_StressMeasurementLineSectionListGrid.SetFixedRowCount(FixRows);
		m_StressMeasurementLineSectionListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2009.09.17 bagus stress Grid修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_STRESS_MEASUREMENT_LINE_SECTION_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
//2009.09.24 bagus stress 修正 --{--
#if 0
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);
// 2009.09.17 bagus stress Grid修正 --{--
#else
	GridBox->GetClientRect(&GridRect);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);

	GridBox->GetWindowRect(&GridRect);
	this->ScreenToClient(&GridRect);

	GridBox->MoveWindow(GridRect.left, GridRect.top, GridRect.Width(), (GRID_ROW_HEIGHT * 18)+4);

	ReleaseDC(pDC);
#endif
//2009.09.24 bagus stress 修正 --}--

	// Row 0 Height
	m_StressMeasurementLineSectionListGrid.SetRowHeight(0, GRID_ROW_HEIGHT);

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	// Row 1 -> RowCount Height
	for(row = 1 ; row < m_StressMeasurementLineSectionListGrid.GetRowCount() ; row++){
		m_StressMeasurementLineSectionListGrid.SetRowHeight(row, GRID_ROW_HEIGHT);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 0 Width
// 2009.09.17 bagus stress Grid修正 --{--
//	m_StressMeasurementLineSectionListGrid.SetColumnWidth(0, 52);	// No.
	m_StressMeasurementLineSectionListGrid.SetColumnWidth(0, nFixColWidth);	// No.
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_StressMeasurementLineSectionListGrid.GetColumnCount(); col++)
	{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_StressMeasurementLineSectionListGrid.SetColumnWidth(col, ItemWidth);
		m_StressMeasurementLineSectionListGrid.SetColumnWidth(col, nColWidth);
// 2009.09.17 bagus stress Grid修正 --}--
	}

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_StressMeasurementLineSectionListGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s",ColItem_ENU[col]);
		m_StressMeasurementLineSectionListGrid.SetItem(&Item);
	}

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	///// Title Row /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_StressMeasurementLineSectionListGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("LxP%d", row);
		m_StressMeasurementLineSectionListGrid.SetItem(&Item);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	return;
}

void CStressLineSectionSettingDlg::NecessaryThicknessMeasurementListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 2; //有効Col数

	int FixRows = 1;
	int FixCols = 1;
	int Cols = iDispCol + FixCols;
//	int Rows = 100 + FixRows;
	int Rows = FixRows;
// 2009.09.17 bagus stress Grid修正 --{--
//	int row,col;
//	int ItemWidth = 85;
	int	col;
// 2009.09.17 bagus stress Grid修正 --}--
	DWORD dwTextStyle;

	char* ColItem_ENU[]={
		"No.",
		"X",
		"Y",
	};
	char* ColItem_JPN[]={
		"No.",
		"X",
		"Y",
	};

	///// Grid Initialize /////
	m_NecessaryThicknessMeasurementListGrid.SetEditable(FALSE);
	m_NecessaryThicknessMeasurementListGrid.EnableDragAndDrop(FALSE);
	m_NecessaryThicknessMeasurementListGrid.SetTextBkColor(WHITE_COLOR);
	m_NecessaryThicknessMeasurementListGrid.SetRowResize(0);

	//2009.09.15 bagus stress --{--
//	m_NecessaryThicknessMeasurementListGrid.EnableSelection(FALSE);
	//m_NecessaryThicknessMeasurementListGrid.SetTrackFocusCell(TRUE);
	//m_NecessaryThicknessMeasurementListGrid.SetFrameFocusCell(TRUE);
	//m_NecessaryThicknessMeasurementListGrid.SetFixedColumnSelection(FALSE);
	//m_NecessaryThicknessMeasurementListGrid.SetFixedRowSelection(FALSE);

	m_NecessaryThicknessMeasurementListGrid.EnableSelection(TRUE);
	m_NecessaryThicknessMeasurementListGrid.SetTrackFocusCell(FALSE);
	m_NecessaryThicknessMeasurementListGrid.SetFrameFocusCell(FALSE);
	m_NecessaryThicknessMeasurementListGrid.SetFixedColumnSelection(TRUE);
	m_NecessaryThicknessMeasurementListGrid.SetFixedRowSelection(TRUE);
	m_NecessaryThicknessMeasurementListGrid.SetListMode(FALSE);
	//2009.09.15 bagus stress --}--

	TRY{
		m_NecessaryThicknessMeasurementListGrid.SetRowCount(Rows);
		m_NecessaryThicknessMeasurementListGrid.SetColumnCount(Cols);
		m_NecessaryThicknessMeasurementListGrid.SetFixedRowCount(FixRows);
		m_NecessaryThicknessMeasurementListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2009.09.17 bagus stress Grid修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_NECESSARY_THICKNESS_MEASUREMENT_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
//2009.09.24 bagus stress 修正 --{--
#if 0
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);
// 2009.09.17 bagus stress Grid修正 --{--
#else
	GridBox->GetClientRect(&GridRect);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);

	GridBox->GetWindowRect(&GridRect);
	this->ScreenToClient(&GridRect);

	GridBox->MoveWindow(GridRect.left, GridRect.top, GridRect.Width(), (GRID_ROW_HEIGHT * 16)+4);

	ReleaseDC(pDC);
#endif
//2009.09.24 bagus stress 修正 --}--

	// Row 0 Height
	m_NecessaryThicknessMeasurementListGrid.SetRowHeight(0, GRID_ROW_HEIGHT);

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	// Row 1 -> RowCount Height
	for(row = 1 ; row < m_NecessaryThicknessMeasurementListGrid.GetRowCount() ; row++){
		m_NecessaryThicknessMeasurementListGrid.SetRowHeight(row, GRID_ROW_HEIGHT);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 0 Width
// 2009.09.17 bagus stress Grid修正 --{--
//	m_NecessaryThicknessMeasurementListGrid.SetColumnWidth(0, 52);	// No.
	m_NecessaryThicknessMeasurementListGrid.SetColumnWidth(0, nFixColWidth);	// No.
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_NecessaryThicknessMeasurementListGrid.GetColumnCount(); col++)
	{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_NecessaryThicknessMeasurementListGrid.SetColumnWidth(col, ItemWidth);
		m_NecessaryThicknessMeasurementListGrid.SetColumnWidth(col, nColWidth);
// 2009.09.17 bagus stress Grid修正 --}--
	}

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_NecessaryThicknessMeasurementListGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s",ColItem_ENU[col]);
		m_NecessaryThicknessMeasurementListGrid.SetItem(&Item);
	}

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	///// Title Row /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_NecessaryThicknessMeasurementListGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("LxP%d", row);
		m_NecessaryThicknessMeasurementListGrid.SetItem(&Item);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	return;
}

void CStressLineSectionSettingDlg::DrawAxisCoordinate(int iOrgPos, int DirX, int DirY)
{
#define ARROW_LENGTH	18
#define ARROW_SIDE	6

	POINT PT[3];
	CString csBuff;
	//Saiki 20090528 Add ----->
	CString strBuffer;
	//Saiki 20090528 Add <-----

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
	//Saiki 20090528 Change ----->
	//if( pDC->SelectObject( CFont::FromHandle((HFONT) GetStockObject(ANSI_FIXED_FONT) ) ) == NULL )
	//	//MessageBox("Error in SelectObject");
	//	MessageBox("選択したオブジェクトがエラーです");
	if( pDC->SelectObject( CFont::FromHandle((HFONT) GetStockObject(ANSI_FIXED_FONT) ) ) == NULL ){
		//MessageBox("Error in SelectObject");
		LoadStringML(IDS_ERROR_SELECT_OBJECT, strBuffer, "Error in SelectObject");
		MessageBox(strBuffer);
	}
	//Saiki 20090528 Change <-----

	///// 背景色を透明に設定 /////
	pDC->SetBkMode(TRANSPARENT);

	///// Draw Center Position /////
	long CenterPosX = StageRectLeft + ((StageRectRight - StageRectLeft) / 2);
	long CenterPosY = StageRectTop + ((StageRectBottom - StageRectTop) / 2);

	pDC->SelectObject(&BrushArrow);
	pDC->SelectObject(&PenArrow);
	///// Draw Arrows /////
	if(iOrgPos == 1){ //十字を描く
		pDC->MoveTo(CenterPosX - ARROW_LENGTH, CenterPosY);
		pDC->LineTo(CenterPosX + ARROW_LENGTH, CenterPosY);
		pDC->MoveTo(CenterPosX, CenterPosY + ARROW_LENGTH);
		pDC->LineTo(CenterPosX, CenterPosY - ARROW_LENGTH);
	}

	if(DirX == 1 || DirY == 1){ //右方向
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

	if(DirX == 2 || DirY == 2){ //上方向
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

	if(DirX == 3 || DirY == 3){ //左方向
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
	if(DirX == 4 || DirY == 4){ //下方向
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


void CStressLineSectionSettingDlg::OnPaint()
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

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

	///// ステージ有効範囲 /////
//	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
	///// ステージ有効範囲 /////
	double dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);

	if((dStageSizeX / dStageSizeX) > ((StageRectRight - StageRectLeft) / (StageRectBottom - StageRectTop))) {
		lDrawStageSizeX =(StageRectRight - StageRectLeft);
		lDrawStageSizeY = ((StageRectRight - StageRectLeft) * dStageSizeX / dStageSizeX);

		m_dDrawStartStagePosX = StageRectLeft;
		m_dDrawStartStagePosY = StageRectTop + ((StageRectBottom - StageRectTop - lDrawStageSizeY) / 2);
		m_dDrawEndStagePosX = StageRectRight;
		m_dDrawEndStagePosY = m_dDrawStartStagePosY + lDrawStageSizeY;
	}
	else{
		lDrawStageSizeX = ((StageRectBottom - StageRectTop) * dStageSizeX / dStageSizeX);
		lDrawStageSizeY = (StageRectBottom - StageRectTop);

		m_dDrawStartStagePosX = StageRectLeft + ((StageRectRight - StageRectLeft - lDrawStageSizeX) / 2);
		m_dDrawStartStagePosY = StageRectTop;
		m_dDrawEndStagePosX = m_dDrawStartStagePosX + lDrawStageSizeX;
		m_dDrawEndStagePosY = StageRectBottom;
	}

	pDC->Rectangle(CRect(m_dDrawStartStagePosX, m_dDrawStartStagePosY, m_dDrawEndStagePosX, m_dDrawEndStagePosY));
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

	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
	ReleaseDC(pDC);
}

void CStressLineSectionSettingDlg::OnKillfocusNumberOfLine()
{
// 2009.09.04 bagus stress ライン数設定ボタン追加 --{--
	return;
// 2009.09.04 bagus stress ライン数設定ボタン追加 --}--

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString	sNumOfLine;
	DWORD	NumOfLine;

	GetDlgItemText(IDC_NUMBER_OF_LINE, sNumOfLine);
	NumOfLine = strtol(sNumOfLine, NULL, 10);

	if (NumOfLine < 0) {
		NumOfLine = 0;

		sNumOfLine.Format("%d", NumOfLine);
		SetDlgItemText(IDC_NUMBER_OF_LINE, sNumOfLine);
	}
	if (NumOfLine > STRESS_LINES_MAX) {
		NumOfLine = STRESS_LINES_MAX;

		sNumOfLine.Format("%d", NumOfLine);
		SetDlgItemText(IDC_NUMBER_OF_LINE, sNumOfLine);
	}

	m_StressConfig.dwLiftPinNumberOfLine = NumOfLine;
// 2009.09.08 bagus stress 修正 --{--
	for (int i=0; i<STRESS_LINES_MAX; i++) {
		if (i<NumOfLine) {
//			m_StressConfig.Line[i].bValidLine = TRUE;
		} else {
			m_StressConfig.Line[i].bValidLine = FALSE;
		}
	}
// 2009.09.08 bagus stress 修正 --}--

	SetLineData();
	SetLineValidData();
}

void CStressLineSectionSettingDlg::SetLineData()
{
// 2009.09.17 bagus stress Grid修正 --{--
	LineListGrid_Init();
// 2009.09.17 bagus stress Grid修正 --}--

	GV_ITEM Item;

	int FixRows = 1;
	int FixCols = 1;
// 2009.09.17 bagus stress Grid修正 --{--
//	int	Cols, Rows;
// 2009.09.17 bagus stress Grid修正 --}--
	int	row;
	DWORD	dwTextStyle;

// 2009.09.17 bagus stress Grid修正 --{--
//	Cols = m_LineListGrid.GetColumnCount();
//	Rows = m_LineListGrid.GetRowCount();
// 2009.09.17 bagus stress Grid修正 --}--

//2009.09.24 bagus stress 修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_LINE_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	DEFAULT_GRID_ROWS;
	DEFAULT_GRID_ROWS = (int)(GridRect.Height() / GRID_ROW_HEIGHT);
	DEFAULT_GRID_ROWS += 1;
//2009.09.24 bagus stress 修正 --}--

	TRY{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_LineListGrid.SetRowCount(m_StressConfig.dwLiftPinNumberOfLine + FixRows);
		if ((m_StressConfig.dwLiftPinNumberOfLine + FixRows) < DEFAULT_GRID_ROWS) {
			m_LineListGrid.SetRowCount(DEFAULT_GRID_ROWS);
		} else {
			m_LineListGrid.SetRowCount(m_StressConfig.dwLiftPinNumberOfLine + FixRows);
		}
// 2009.09.17 bagus stress Grid修正 --}--
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	for (row=FixRows; row<m_StressConfig.dwLiftPinNumberOfLine+FixRows; row++) {
		dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("Line%d", row);
		m_LineListGrid.SetItem(&Item);

		dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 1;
		Item.nFormat = dwTextStyle;
//		Item.strText = "0";
		Item.strText.Format("%d", m_StressConfig.Line[row-FixRows].dwLiftPinNumberOfLn);
		m_LineListGrid.SetItem(&Item);

// 2009.09.17 bagus stress Grid修正 --{--
		m_LineListGrid.SetItemState(row,1,m_LineListGrid.GetItemState(row,1) & ~GVIS_READONLY);
// 2009.09.17 bagus stress Grid修正 --}--
	}

// 2009.09.17 bagus stress Grid修正 --{--
	int	Rows = m_LineListGrid.GetRowCount();

	for (row=(m_StressConfig.dwLiftPinNumberOfLine + FixRows); row<Rows; row++) {
		m_LineListGrid.SetItemState(row,1,m_LineListGrid.GetItemState(row,1) | GVIS_READONLY);
	}
// 2009.09.17 bagus stress Grid修正 --}--
}

void CStressLineSectionSettingDlg::OnGridsetButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (!CheckLineData()) {
// 2009.09.17 bagus stress Grid修正 --{--
		Invalidate( FALSE );
// 2009.09.17 bagus stress Grid修正 --}--
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckLineData");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_PIN_NUM, strBuffer, "Please input pin number.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}

	SetDefaultPinData();
	SetPinData();

// 2009.09.08 bagus stress 修正 --{--
	StressMeasurementLineSectionListGrid_Init();
	NecessaryThicknessMeasurementListGrid_Init();
// 2009.09.08 bagus stress 修正 --}--

// 2009.09.17 bagus stress Grid修正 --{--
	Invalidate( FALSE );
// 2009.09.17 bagus stress Grid修正 --}--
}

void CStressLineSectionSettingDlg::SetPinData()
{
	PinListGrid_Init();

	GV_ITEM Item;

	int	row;

	int	i ,j;

	int	NumOfAllPins = 0;

// 2009.09.08 bagus stress 修正 --{--
//	for (i=0; i<STRESS_LINES_MAX; i++) {
	for (i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
// 2009.09.08 bagus stress 修正 --}--
		NumOfAllPins += m_StressConfig.Line[i].dwLiftPinNumberOfLn;
	}

	int FixRows = 1;
	int FixCols = 1;
	int	Rows;
	DWORD	dwTextStyle;

//2009.09.24 bagus stress 修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_PIN_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	DEFAULT_GRID_ROWS;
	DEFAULT_GRID_ROWS = (int)(GridRect.Height() / GRID_ROW_HEIGHT);
	DEFAULT_GRID_ROWS += 1;
//2009.09.24 bagus stress 修正 --}--

	TRY{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_PinListGrid.SetRowCount(NumOfAllPins + FixRows);
		if ((NumOfAllPins + FixRows) < DEFAULT_GRID_ROWS) {
			m_PinListGrid.SetRowCount(DEFAULT_GRID_ROWS);
		} else {
			m_PinListGrid.SetRowCount(NumOfAllPins + FixRows);
		}
// 2009.09.17 bagus stress Grid修正 --}--
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	row = FixRows;
	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
		Rows = m_StressConfig.Line[i].dwLiftPinNumberOfLn;

		for (j=0; j<Rows; j++) {
			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = 0;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("L%dPin%d", i+1, j+1);
			m_PinListGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = 1;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i].PinPos[j].lX / MICROMETRE));		// 単位変換(um->mm)
			m_PinListGrid.SetItem(&Item);
// 2009.09.17 bagus stress Grid修正 --{--
			m_PinListGrid.SetItemState(row,1,m_PinListGrid.GetItemState(row,1) & ~GVIS_READONLY);
// 2009.09.17 bagus stress Grid修正 --}--

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = 2;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i].PinPos[j].lY / MICROMETRE));		// 単位変換(um->mm)
			m_PinListGrid.SetItem(&Item);
// 2009.09.17 bagus stress Grid修正 --{--
			m_PinListGrid.SetItemState(row,2,m_PinListGrid.GetItemState(row,2) & ~GVIS_READONLY);
// 2009.09.17 bagus stress Grid修正 --}--

			row++;
		}
	}

// 2009.09.17 bagus stress Grid修正 --{--
	Rows = m_PinListGrid.GetRowCount();

	for (row=(NumOfAllPins + FixRows); row<Rows; row++) {
		m_PinListGrid.SetItemState(row,1,m_PinListGrid.GetItemState(row,1) | GVIS_READONLY);
		m_PinListGrid.SetItemState(row,2,m_PinListGrid.GetItemState(row,2) | GVIS_READONLY);
	}
// 2009.09.17 bagus stress Grid修正 --}--
}

BOOL CStressLineSectionSettingDlg::CheckLineData()
{
// 2009.09.17 bagus stress Grid修正 --{--
//	int	LineCols, LineRows;
	int	LineRows;

//	LineCols = m_LineListGrid.GetColumnCount();
	LineRows = m_LineListGrid.GetRowCount();
// 2009.09.17 bagus stress Grid修正 --}--

	DWORD	NumOfPin[STRESS_LINES_MAX];
	memset(&NumOfPin, 0, sizeof(NumOfPin));

	GV_ITEM Item;

	int LineFixRows = 1;
	int LineFixCols = 1;
	int	row;

// 2009.09.15 bagus stress 修正 --{--
	long	PinNum;
// 2009.09.15 bagus stress 修正 --}--

	for (row=LineFixRows; row<LineRows; row++) {
		Item.mask = GVIF_TEXT;
		Item.row = row;
		Item.col = 1;

// 2009.09.17 bagus stress Grid修正 --{--
		if (row >= m_StressConfig.dwLiftPinNumberOfLine + LineFixRows)
			continue;
// 2009.09.17 bagus stress Grid修正 --}--

		m_LineListGrid.GetItem(&Item);

// 2009.09.15 bagus stress 修正 --{--
//		NumOfPin[row - LineFixRows] = strtol(Item.strText, NULL, 10);
		if (!CheckInputValueLong(Item.strText, &PinNum))
			return ( FALSE );

		NumOfPin[row - LineFixRows] = PinNum;
// 2009.09.15 bagus stress 修正 --}--

		if ((NumOfPin[row - LineFixRows] <= 1) || (NumOfPin[row - LineFixRows] > STRESS_PINS_MAX)) {
			return ( FALSE );
		}
	}

// 2009.09.17 bagus stress Grid修正 --{--
//	for (row=LineFixRows; row<LineRows; row++) {
	for (row=LineFixRows; row<m_StressConfig.dwLiftPinNumberOfLine + LineFixRows; row++) {
// 2009.09.17 bagus stress Grid修正 --}--
		m_StressConfig.Line[row - LineFixRows].dwLiftPinNumberOfLn = NumOfPin[row - LineFixRows];
		m_StressConfig.Line[row - LineFixRows].dwSectionNum = NumOfPin[row - LineFixRows] - 1;
	}

	return ( TRUE );
}

void CStressLineSectionSettingDlg::SetLineValidData()
{
//2009.09.24 bagus stress 修正 --{--
	LineValidListGrid_Init();
//2009.09.24 bagus stress 修正 --}--

	GV_ITEM Item;

	int FixRows = 1;
	int FixCols = 1;
	int	row;
	int	nLine;
	DWORD	dwTextStyle;

//2009.09.24 bagus stress 修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_SET_LINE_VALID_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	DEFAULT_GRID_ROWS;
	DEFAULT_GRID_ROWS = (int)(GridRect.Height() / GRID_ROW_HEIGHT);
	DEFAULT_GRID_ROWS += 1;
//2009.09.24 bagus stress 修正 --}--

	TRY{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_LineValidListGrid.SetRowCount(m_StressConfig.dwLiftPinNumberOfLine + FixRows);
		if ((m_StressConfig.dwLiftPinNumberOfLine + FixRows) < DEFAULT_GRID_ROWS) {
			m_LineValidListGrid.SetRowCount(DEFAULT_GRID_ROWS);
		} else {
			m_LineValidListGrid.SetRowCount(m_StressConfig.dwLiftPinNumberOfLine + FixRows);
		}
// 2009.09.17 bagus stress Grid修正 --}--
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	nLine = 1;
	for (row=FixRows; row<m_StressConfig.dwLiftPinNumberOfLine+FixRows; row++) {
		dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("Line%d", nLine);
		m_LineValidListGrid.SetItem(&Item);

		//2009.09.14 bagus stress --{--
		m_LineValidListGrid.SetCellType(row, 1, RUNTIME_CLASS(CGridCellCheck));
		if(m_StressConfig.Line[row - FixRows].bValidLine){
			((CGridCellCheck *)m_LineValidListGrid.GetCell(row,1))->SetCheck(TRUE);
		}else{
			((CGridCellCheck *)m_LineValidListGrid.GetCell(row,1))->SetCheck(FALSE);
		}
		/*
		dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 1;
		Item.nFormat = dwTextStyle;
//		Item.strText.Format("0");
		if (m_StressConfig.Line[row - FixRows].bValidLine) {
			Item.strText.Format("1");
		} else {
			Item.strText.Format("0");
		}
		m_LineValidListGrid.SetItem(&Item);
		*/
		//2009.09.14 bagus stress --}--

		nLine++;
	}
	//SetLineValidCheckBox();
}

void CStressLineSectionSettingDlg::LineValidListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 1; //有効Col数

	int FixRows = 1;
	int FixCols = 1;
	int Cols = iDispCol + FixCols;
	int Rows = FixRows;
// 2009.09.17 bagus stress Grid修正 --{--
//	int row,col;
//	int ItemWidth = 51;
	int	col;
// 2009.09.17 bagus stress Grid修正 --}--
	DWORD dwTextStyle;

	char* ColItem_ENU[]={
		"No.",
		"Valid",
	};
	char* ColItem_JPN[]={
		"No.",
		"Valid",
	};

	///// Grid Initialize /////
// 2009.09.17 bagus stress Grid修正 --{--
//	m_LineValidListGrid.SetEditable(TRUE);
	m_LineValidListGrid.SetEditable(FALSE);
// 2009.09.17 bagus stress Grid修正 --}--
	m_LineValidListGrid.EnableDragAndDrop(FALSE);
	m_LineValidListGrid.SetTextBkColor(WHITE_COLOR);
	m_LineValidListGrid.SetRowResize(0);

	//2009.09.15 bagus stress --{--
	//m_LineValidListGrid.EnableSelection(TRUE);
	//m_LineValidListGrid.SetTrackFocusCell(TRUE);
	//m_LineValidListGrid.SetFrameFocusCell(TRUE);
	//m_LineValidListGrid.SetFixedColumnSelection(FALSE);
	//m_LineValidListGrid.SetFixedRowSelection(FALSE);
	m_LineValidListGrid.SetListMode(FALSE);
	m_LineValidListGrid.EnableSelection(TRUE);
	m_LineValidListGrid.SetTrackFocusCell(FALSE);
	m_LineValidListGrid.SetFrameFocusCell(FALSE);
	m_LineValidListGrid.SetFixedColumnSelection(TRUE);
	//2009.09.15 bagus stress --}--

	TRY{
		m_LineValidListGrid.SetRowCount(Rows);
		m_LineValidListGrid.SetColumnCount(Cols);
		m_LineValidListGrid.SetFixedRowCount(FixRows);
		m_LineValidListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2009.09.17 bagus stress Grid修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_SET_LINE_VALID_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
//2009.09.24 bagus stress 修正 --{--
#if 0
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);
// 2009.09.17 bagus stress Grid修正 --{--
#else
	GridBox->GetClientRect(&GridRect);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);

	GridBox->GetWindowRect(&GridRect);
	this->ScreenToClient(&GridRect);

	GridBox->MoveWindow(GridRect.left, GridRect.top, GridRect.Width(), (GRID_ROW_HEIGHT * 8)+4);

	ReleaseDC(pDC);
#endif
//2009.09.24 bagus stress 修正 --}--

	// Row 0 Height
	m_LineValidListGrid.SetRowHeight(0, GRID_ROW_HEIGHT);

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	// Row 1 -> RowCount Height
	for(row = 1 ; row < m_LineValidListGrid.GetRowCount() ; row++){
		m_LineValidListGrid.SetRowHeight(row, GRID_ROW_HEIGHT);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 0 Width
// 2009.09.17 bagus stress Grid修正 --{--
//	m_LineValidListGrid.SetColumnWidth(0, 51);	// No.
	m_LineValidListGrid.SetColumnWidth(0, nFixColWidth);	// No.
// 2009.09.17 bagus stress Grid修正 --}--

	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_LineValidListGrid.GetColumnCount(); col++)
	{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_LineValidListGrid.SetColumnWidth(col, ItemWidth);
		m_LineValidListGrid.SetColumnWidth(col, nColWidth);
// 2009.09.17 bagus stress Grid修正 --}--
	}

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_LineValidListGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s",ColItem_ENU[col]);
		m_LineValidListGrid.SetItem(&Item);
	}

// 2009.09.17 bagus stress Grid修正 --{--
#if 0
	///// Title Row /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_LineValidListGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("Line%d", row);
		m_LineValidListGrid.SetItem(&Item);
	}
#endif
// 2009.09.17 bagus stress Grid修正 --}--

	return;
}

BOOL CStressLineSectionSettingDlg::IsValidLine(int nLine)
{
	return ( TRUE );
}

void CStressLineSectionSettingDlg::OnSetLineSectionButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (!CheckPinData()) {
// 2009.09.17 bagus stress Grid修正 --{--
		Invalidate( FALSE );
// 2009.09.17 bagus stress Grid修正 --}--
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckPinData");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_PIN_POS, strBuffer, "Please input pin position.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}

	CheckLineValidData();

// 2009.09.14 bagus stress 修正 --{--
	if (!CheckStressValidRangeData()) {
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_INTERVAL_RANGE, strBuffer, "Please input stress interval range.");
		AfxMessageBox(strBuffer);
		return;
	}

	if (!CheckInvalidRangeEdgeData()) {
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_INTERVAL_RANGE_EDGE, strBuffer, "Please input interval range edge.");
		AfxMessageBox(strBuffer);
		return;
	}

	if (!CheckThinkMeasurementPointOffsetXData()) {
		CString	strBuffer;
		LoadStringML(IDS_STRESS_THICK_MEAS_POINT_OFFSET_X, strBuffer, "Please input offset(X).");
		AfxMessageBox(strBuffer);
		return;
	}
// 2009.09.14 bagus stress 修正 --}--

	SetDefaultStressMeasurementLineSectionData();
	SetStressMeasurementLineSectionData();
	SetDefaultNecessaryThicknessMeasurementData();
	SetNecessaryThicknessMeasurementData();

// 2009.09.17 bagus stress Grid修正 --{--
//	Invalidate( FALSE );
// 2009.09.17 bagus stress Grid修正 --}--

// 2009.09.08 bagus stress 修正 --{--
//	OnPaint();
// 2009.09.08 bagus stress 修正 --}--
}

void CStressLineSectionSettingDlg::SetStressMeasurementLineSectionData()
{
	StressMeasurementLineSectionListGrid_Init();

	GV_ITEM Item;

	int	i ,j;

	int	NumOfSections = 0;

// 2009.09.08 bagus stress 修正 --{--
//	for (i=0; i<STRESS_LINES_MAX; i++) {
	for (i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
// 2009.09.08 bagus stress 修正 --}--
		if ((m_StressConfig.Line[i].dwLiftPinNumberOfLn > 0) && (IsValidLine(i))) {
//			NumOfSections += (m_StressConfig.Line[i].dwLiftPinNumberOfLn - 1);
			if (m_StressConfig.Line[i].bValidLine) {
				NumOfSections += m_StressConfig.Line[i].dwSectionNum;
			}
		}
	}

	int FixRows = 1;
	int FixCols = 1;
	int	row;
	int	Rows;
	DWORD	dwTextStyle;

//2009.09.24 bagus stress 修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_STRESS_MEASUREMENT_LINE_SECTION_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	DEFAULT_GRID_ROWS;
	DEFAULT_GRID_ROWS = (int)(GridRect.Height() / GRID_ROW_HEIGHT);
	DEFAULT_GRID_ROWS += 1;
//2009.09.24 bagus stress 修正 --}--

	TRY{
// 2009.09.17 bagus stress Grid修正 --{--
//		m_StressMeasurementLineSectionListGrid.SetRowCount(NumOfSections + FixRows);
		if ((NumOfSections + FixRows) < DEFAULT_GRID_ROWS) {
			m_StressMeasurementLineSectionListGrid.SetRowCount(DEFAULT_GRID_ROWS);
		} else {
			m_StressMeasurementLineSectionListGrid.SetRowCount(NumOfSections + FixRows);
		}
// 2009.09.17 bagus stress Grid修正 --}--
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	row = FixRows;
	for(i=FixRows; i<m_StressConfig.dwLiftPinNumberOfLine+FixRows; i++){
		if (!m_StressConfig.Line[i - FixRows].bValidLine) {
			continue;
		}
		Rows = m_StressConfig.Line[i - FixRows].dwSectionNum;

		for (j=row; j<row+Rows; j++) {
			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 0;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("L%dS%d", i, j - row + 1);
			m_StressMeasurementLineSectionListGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 1;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i - FixRows].SectPos[j - row].lScanStartPosX / MICROMETRE));			// 単位変換(um->mm)
			m_StressMeasurementLineSectionListGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 2;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i - FixRows].SectPos[j - row].lScanEndPosX / MICROMETRE));			// 単位変換(um->mm)
			m_StressMeasurementLineSectionListGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 3;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i - FixRows].SectPos[j - row].lScanPosY / MICROMETRE));			// 単位変換(um->mm)
			m_StressMeasurementLineSectionListGrid.SetItem(&Item);
		}

		row += Rows;
	}
}

void CStressLineSectionSettingDlg::SetNecessaryThicknessMeasurementData()
{
	NecessaryThicknessMeasurementListGrid_Init();

	GV_ITEM Item;

	int	i ,j;

// 2009.09.11 bagus stress 修正 --{--
	int	NumOfLines = 0;

	for (i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
		if (m_StressConfig.Line[i].bValidLine) {
			NumOfLines++;
		}
	}
// 2009.09.11 bagus stress 修正 --}--

	int FixRows = 1;
	int FixCols = 1;
	DWORD	dwTextStyle;

//2009.09.24 bagus stress 修正 --{--
	CWnd* GridBox = GetDlgItem(IDC_NECESSARY_THICKNESS_MEASUREMENT_LIST_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	DEFAULT_GRID_ROWS;
	DEFAULT_GRID_ROWS = (int)(GridRect.Height() / GRID_ROW_HEIGHT);
	DEFAULT_GRID_ROWS += 1;
//2009.09.24 bagus stress 修正 --}--

	TRY{
// 2009.09.11 bagus stress 修正 --{--
//		m_NecessaryThicknessMeasurementListGrid.SetRowCount((m_StressConfig.dwLiftPinNumberOfLine * PIN_INTERVAL) + FixRows);
// 2009.09.17 bagus stress Grid修正 --{--
//		m_NecessaryThicknessMeasurementListGrid.SetRowCount((NumOfLines * PIN_INTERVAL) + FixRows);
		if (((NumOfLines * PIN_INTERVAL) + FixRows) < DEFAULT_GRID_ROWS) {
			m_NecessaryThicknessMeasurementListGrid.SetRowCount(DEFAULT_GRID_ROWS);
		} else {
			m_NecessaryThicknessMeasurementListGrid.SetRowCount((NumOfLines * PIN_INTERVAL) + FixRows);
		}
// 2009.09.17 bagus stress Grid修正 --}--
// 2009.09.11 bagus stress 修正 --}--
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2009.09.11 bagus stress 修正 --{--
	int	Row = 0 + FixRows;
// 2009.09.11 bagus stress 修正 --}--
	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
// 2009.09.11 bagus stress 修正 --{--
		if (!m_StressConfig.Line[i].bValidLine) {
			continue;
		}
// 2009.09.11 bagus stress 修正 --}--
		for (j=0; j<PIN_INTERVAL; j++) {
			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
// 2009.09.11 bagus stress 修正 --{--
//			Item.row = j + (i * PIN_INTERVAL) + FixRows;
			Item.row = Row;
// 2009.09.11 bagus stress 修正 --}--
			Item.col = 0;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("L%dP%d", i+1, j+1);
			m_NecessaryThicknessMeasurementListGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
// 2009.09.11 bagus stress 修正 --{--
//			Item.row = j + (i * PIN_INTERVAL) + FixRows;
			Item.row = Row;
// 2009.09.11 bagus stress 修正 --}--
			Item.col = 1;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i].MeasPos[j].lX / MICROMETRE));		// 単位変換(um->mm)
			m_NecessaryThicknessMeasurementListGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
// 2009.09.11 bagus stress 修正 --{--
//			Item.row = j + (i * PIN_INTERVAL) + FixRows;
			Item.row = Row;
// 2009.09.11 bagus stress 修正 --}--
			Item.col = 2;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i].MeasPos[j].lY / MICROMETRE));		// 単位変換(um->mm)
			m_NecessaryThicknessMeasurementListGrid.SetItem(&Item);

// 2009.09.11 bagus stress 修正 --{--
			Row++;
// 2009.09.11 bagus stress 修正 --}--
		}
	}
}

void CStressLineSectionSettingDlg::OnKillfocusInvalidRangeEdge()
{
// 2009.09.11 bagus stress 修正 --{--
	return;
// 2009.09.11 bagus stress 修正 --}--

// 2009.09.04 bagus stress 数値チェック追加 --{--
	if (!CheckInvalidRangeEdgeData()) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckInvalidRangeEdgeData");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_INTERVAL_RANGE_EDGE, strBuffer, "Please input interval range edge.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}
// 2009.09.04 bagus stress 数値チェック追加 --}--

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString	strBuffer;

	GetDlgItemText(IDC_INVALID_RANGE_EDGE, strBuffer);
	m_StressConfig.lInvalidRangeEdge = (long)(strtod(strBuffer.GetBuffer(0), NULL) * MICROMETRE);	// 単位変換(mm->um)

// 2009.09.08 bagus stress 修正 --{--
	StressMeasurementLineSectionListGrid_Init();
	NecessaryThicknessMeasurementListGrid_Init();
// 2009.09.08 bagus stress 修正 --}--
}

void CStressLineSectionSettingDlg::OnKillfocusThickMeasurementPointOffsetX()
{
// 2009.09.11 bagus stress 修正 --{--
	return;
// 2009.09.11 bagus stress 修正 --}--

// 2009.09.04 bagus stress 数値チェック追加 --{--
	if (!CheckThinkMeasurementPointOffsetXData()) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckThinkMeasurementPointOffsetXData");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_THICK_MEAS_POINT_OFFSET_X, strBuffer, "Please input offset(X).");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}
// 2009.09.04 bagus stress 数値チェック追加 --}--

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString	strBuffer;

	GetDlgItemText(IDC_THICK_MEASUREMENT_POINT_OFFSET_X, strBuffer);
	m_StressConfig.MeasOffset.lX = (long)(strtod(strBuffer.GetBuffer(0), NULL) * MICROMETRE);	// 単位変換(mm->um)

// 2009.09.08 bagus stress 修正 --{--
	StressMeasurementLineSectionListGrid_Init();
	NecessaryThicknessMeasurementListGrid_Init();
// 2009.09.08 bagus stress 修正 --}--
}

void CStressLineSectionSettingDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

// 2009.09.16 bagus stress 修正 --{--
#if 1
	if(!SetAllInputData()){
		CString	strBuffer;
		LoadStringML(IDS_LINE_SECTION_SETTING_CONFIRM, strBuffer, "Is the input value good though it is not reflected?");

		if (IDOK == MessageBox(strBuffer, "", MB_ICONQUESTION|MB_OKCANCEL)) {
			ConfigFile_SetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);
			ConfigFile_SaveNanoSpecIni(SYSTEM_STRESS_INI_FILE);

			CDialog::OnOK();
		}

		return;
	}
#endif
// 2009.09.16 bagus stress 修正 --}--

// 2009.09.08 bagus stress チェック処理追加 --{--
	if(!CheckLineNum()) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckLineNum");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_LINE_NUM, strBuffer, "Please input line number.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}

	if(!CheckLineData()) {
// 2009.09.17 bagus stress Grid修正 --{--
		Invalidate( FALSE );
// 2009.09.17 bagus stress Grid修正 --}--
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckLineData");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_PIN_NUM, strBuffer, "Please input pin number.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}

// 2009.09.15 bagus stress 修正 --{--
	if (!CheckPinData()) {
// 2009.09.17 bagus stress Grid修正 --{--
		Invalidate( FALSE );
// 2009.09.17 bagus stress Grid修正 --}--
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_PIN_POS, strBuffer, "Please input pin position.");
		AfxMessageBox(strBuffer);
		return;
	}
// 2009.09.15 bagus stress 修正 --}--

	if (!CheckStressValidRangeData()) {
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_INTERVAL_RANGE, strBuffer, "Please input stress interval range.");
		AfxMessageBox(strBuffer);
		return;
	}

	if (!CheckInvalidRangeEdgeData()) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckInvalidRangeEdgeData");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_INTERVAL_RANGE_EDGE, strBuffer, "Please input interval range edge.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}

	if (!CheckThinkMeasurementPointOffsetXData()) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckThinkMeasurementPointOffsetXData");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_THICK_MEAS_POINT_OFFSET_X, strBuffer, "Please input offset(X).");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}
// 2009.09.08 bagus stress チェック処理追加 --}--

// 2009.09.11 bagus stress チェック追加 --{--
	if (!CheckStressMeasurementLineSectionData()) {
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_LINE_SECTION, strBuffer, "The distance of the line is too short.");
		AfxMessageBox(strBuffer);
		return;
	}

	if (!CheckNecessaryThicknessMeasurementData()) {
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_MEAS_POINT, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}
// 2009.09.11 bagus stress チェック追加 --}--

// 2009.09.16 bagus stress 修正 --{--
#if 0
	if(!SetAllInputData()){
		CString	strBuffer;
		LoadStringML(IDS_LINE_SECTION_SETTING_CONFIRM, strBuffer, "Is the input value good though it is not reflected?");

		if (IDOK != MessageBox(strBuffer, "", MB_ICONQUESTION|MB_OKCANCEL))
			return;
	}
#endif
// 2009.09.16 bagus stress 修正 --}--
	ConfigFile_SetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_STRESS_INI_FILE);

	CDialog::OnOK();
}

BOOL CStressLineSectionSettingDlg::CheckStressConfig_NumOfLineData()
{
	if (m_StressConfig.dwLiftPinNumberOfLine < 0) {
		return ( FALSE );
	}
	if (m_StressConfig.dwLiftPinNumberOfLine > STRESS_LINES_MAX) {
		return ( FALSE );
	}

	return ( TRUE );
}

BOOL CStressLineSectionSettingDlg::CheckStressConfig_LineData()
{
	int	i;

	for (i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
		if ((m_StressConfig.Line[i].dwLiftPinNumberOfLn <= 1) || (m_StressConfig.Line[i].dwLiftPinNumberOfLn > STRESS_PINS_MAX)) {
			return ( FALSE );
		}
	}

	return ( TRUE );
}

BOOL CStressLineSectionSettingDlg::CheckStressConfig_PinData()
{
	int	i, j;
	DWORD	Rows;

	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
		Rows = m_StressConfig.Line[i].dwLiftPinNumberOfLn;
		for (j=0; j<Rows; j++) {
// 2009.09.08 bagus stress 原点位置対応 --{--
//			if ((m_StressConfig.Line[i].PinPos[j].lX < ((m_StressConfig.Size.dx / 2) * -1)) || (m_StressConfig.Line[i].PinPos[j].lX > (m_StressConfig.Size.dx / 2))) {
//				return ( FALSE );
//			}
//			if ((m_StressConfig.Line[i].PinPos[j].lY < ((m_StressConfig.Size.dy / 2) * -1)) && (m_StressConfig.Line[i].PinPos[j].lY > (m_StressConfig.Size.dy / 2))) {
//				return ( FALSE );
//			}
			if (!CheckPinPos(m_StressConfig.Line[i].PinPos[j].lX, m_StressConfig.Line[i].PinPos[j].lY))
				return ( FALSE );
// 2009.09.08 bagus stress 原点位置対応 --}--
		}
	}

	return ( TRUE );
}

BOOL CStressLineSectionSettingDlg::CheckPinData()
{
	long	PinX[STRESS_LINES_MAX * STRESS_PINS_MAX];
	long	PinY[STRESS_LINES_MAX * STRESS_PINS_MAX];
	long	tmpPinX, tmpPinY;

	memset(&PinX, 0, sizeof(PinX));
	memset(&PinY, 0, sizeof(PinY));

// 2009.09.17 bagus stress Grid修正 --{--
//	int	PinCols, PinRows;
	int	PinRows;

//	PinCols = m_PinListGrid.GetColumnCount();
	PinRows = m_PinListGrid.GetRowCount();
// 2009.09.17 bagus stress Grid修正 --}--

	GV_ITEM Item;

	int	i ,j;

	int FixRows = 1;
	int FixCols = 1;
	int	Rows;
	int	Pin;

// 2009.09.15 bagus stress 修正 --{--
	double	tmpPin;
// 2009.09.15 bagus stress 修正 --}--

// 2009.09.17 bagus stress Grid修正 --{--
	int	NumOfAllPins = 0;

	for (i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
		NumOfAllPins += m_StressConfig.Line[i].dwLiftPinNumberOfLn;
	}
// 2009.09.17 bagus stress Grid修正 --}--

	Pin = 0;
	for(i=FixRows; i<PinRows; i++){
// 2009.09.17 bagus stress Grid修正 --{--
		if (i >= NumOfAllPins + FixRows)
			continue;
// 2009.09.17 bagus stress Grid修正 --}--
		Item.mask = GVIF_TEXT;
		Item.row = i;
		Item.col = 1;
		m_PinListGrid.GetItem(&Item);
// 2009.09.15 bagus stress 修正 --{--
//		tmpPinX = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)
		if (!CheckInputValueDouble(Item.strText, &tmpPin))
			return ( FALSE );

		tmpPinX = tmpPin * MICROMETRE;
// 2009.09.15 bagus stress 修正 --}--

		Item.mask = GVIF_TEXT;
		Item.row = i;
		Item.col = 2;
		m_PinListGrid.GetItem(&Item);
// 2009.09.15 bagus stress 修正 --{--
//		tmpPinY = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)
		if (!CheckInputValueDouble(Item.strText, &tmpPin))
			return ( FALSE );

		tmpPinY = tmpPin * MICROMETRE;
// 2009.09.15 bagus stress 修正 --}--

// 2009.09.08 bagus stress 原点位置対応 --{--
//		if ((tmpPinX >= ((m_StressConfig.Size.dx / 2) * -1)) && (tmpPinX <= (m_StressConfig.Size.dx / 2))) {
//			PinX[i - FixRows] = tmpPinX;
//		} else {
//			return ( FALSE );
//		}
//		if ((tmpPinY >= ((m_StressConfig.Size.dy / 2) * -1)) && (tmpPinY <= (m_StressConfig.Size.dy / 2))) {
//			PinY[i - FixRows] = tmpPinY;
//		} else {
//			return ( FALSE );
//		}
		if (!CheckPinPos(tmpPinX, tmpPinY))
			return ( FALSE );
		PinX[i - FixRows] = tmpPinX;
		PinY[i - FixRows] = tmpPinY;
// 2009.09.08 bagus stress 原点位置対応 --}--
	}

// 2009.09.14 bagus stress 修正 --{--
	long	SectionLength;

	Pin = 0;
	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
		Rows = m_StressConfig.Line[i].dwLiftPinNumberOfLn;
		for (j=0; j<Rows; j++) {
			if (j == 0) {
				Pin++;
				continue;
			}

			SectionLength = labs(PinX[Pin] - PinX[Pin - 1]);
			if (SectionLength < SECTION_MIN_RANGE)
				return ( FALSE );

			Pin++;
		}
	}
// 2009.09.14 bagus stress 修正 --}--

	Pin = 0;
	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
		Rows = m_StressConfig.Line[i].dwLiftPinNumberOfLn;
		for (j=0; j<Rows; j++) {
			m_StressConfig.Line[i].PinPos[j].lX = PinX[Pin];
			m_StressConfig.Line[i].PinPos[j].lY = PinY[Pin];

			Pin++;
		}
	}

	return ( TRUE );
}

void CStressLineSectionSettingDlg::SetDefaultPinData()
{
	PinListGrid_Init();

	double	LineInterval, PinInterval;
	double	PinX, PinY;
// 2009.09.08 bagus stress 原点位置対応 --{--
	double	PinOrgX, PinOrgY;
// 2009.09.08 bagus stress 原点位置対応 --}--

	if (m_StressConfig.dwLiftPinNumberOfLine <= 1) {
		LineInterval = 0;
	} else {
// 2009.09.16 bagus stress 修正 --{--
//		LineInterval = (((m_StressConfig.Size.dy - (m_StressConfig.lInvalidRangeEdge * 2)) / (m_StressConfig.dwLiftPinNumberOfLine - 1)) * 1000 + 500) / 1000;
		LineInterval = (long)((((m_StressConfig.Size.dy - (m_StressConfig.lInvalidRangeEdge * 2)) / (m_StressConfig.dwLiftPinNumberOfLine - 1)) * 1000 + 500) / 1000);
// 2009.09.16 bagus stress 修正 --}--
	}

	int	i, j;

	int	Rows;

// 2009.09.08 bagus stress 原点位置対応 --{--
//	PinY = (long)((((m_StressConfig.Size.dy / 2) - m_StressConfig.lInvalidRangeEdge) * 1000 + 500) / 1000);
//	PinY *= -1;
	CheckPinMinPos(&PinOrgX, &PinOrgY);
	if (PinOrgY == 0) {
		PinOrgY += m_StressConfig.lInvalidRangeEdge;
		PinY = PinOrgY;
	} else if (PinOrgY < 0) {
		PinOrgY += m_StressConfig.lInvalidRangeEdge;
		PinY = (long)((PinOrgY * 1000 - 500) / 1000);
	} else {
		PinOrgY -= m_StressConfig.lInvalidRangeEdge;
		PinY = (long)((PinOrgY * 1000 + 500) / 1000);
	}
// 2009.09.11 bagus stress 修正 --{--
//	if (PinOrgX == 0) {
//		PinOrgX += m_StressConfig.lInvalidRangeEdge;
//	} else if (PinOrgX < 0) {
//		PinOrgX += m_StressConfig.lInvalidRangeEdge;
//	} else {
//		PinOrgX -= m_StressConfig.lInvalidRangeEdge;
//	}
// 2009.09.11 bagus stress 修正 --}--
// 2009.09.08 bagus stress 原点位置対応 --}--
	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
		Rows = m_StressConfig.Line[i].dwLiftPinNumberOfLn;

		if (Rows <= 1) {
			PinInterval = 0;
		} else {
			PinInterval = (long)((((m_StressConfig.Size.dx - (m_StressConfig.lInvalidRangeEdge * 2)) / (Rows - 1)) * 1000 + 500) / 1000);
		}

// 2009.09.08 bagus stress 原点位置対応 --{--
//		PinX = (long)((((m_StressConfig.Size.dx / 2) - m_StressConfig.lInvalidRangeEdge) * 1000 + 500) / 1000);
//		PinX *= -1;
		if (PinOrgX == 0) {
			PinX = PinOrgX;
		} else if (PinOrgX < 0) {
			PinX = (long)((PinOrgX * 1000 - 500) / 1000);
		} else {
			PinX = (long)((PinOrgX * 1000 + 500) / 1000);
		}
// 2009.09.08 bagus stress 原点位置対応 --}--
		for (j=0; j<Rows; j++) {
// 2009.09.08 bagus stress 修正 --{--
#if 0
			m_StressConfig.Line[i].PinPos[j].lX = PinX;
			m_StressConfig.Line[i].PinPos[j].lY = PinY;
#else
			m_StressConfig.Line[i].PinPos[j].lX = 0;
			m_StressConfig.Line[i].PinPos[j].lY = 0;
#endif
// 2009.09.08 bagus stress 修正 --}--
			PinX += PinInterval;
		}

		PinY += LineInterval;
	}
}

void CStressLineSectionSettingDlg::SetDefaultStressMeasurementLineSectionData()
{
	long	PinXS, PinXE, PinY;

	int	i ,j;
	int	Rows;

	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
		Rows = m_StressConfig.Line[i].dwSectionNum;
		for (j=0; j<Rows; j++) {
			PinY = m_StressConfig.Line[i].PinPos[j].lY;
			PinXS = m_StressConfig.Line[i].PinPos[j].lX;
			PinXE = m_StressConfig.Line[i].PinPos[j + 1].lX;

// 2009.10.19 K.Matsuo 有効測定範囲の設定に変更 -->
//			m_StressConfig.Line[i].SectPos[j].lScanStartPosX = PinXS + m_StressConfig.lStressValidRange;
//			m_StressConfig.Line[i].SectPos[j].lScanEndPosX = PinXE - m_StressConfig.lStressValidRange;
			m_StressConfig.Line[i].SectPos[j].lScanStartPosX = ((PinXS + PinXE) / 1000 / 2 * 1000) - m_StressConfig.lStressValidRange;	// mm小数点以下は、切り落とし
			m_StressConfig.Line[i].SectPos[j].lScanEndPosX = ((PinXS + PinXE) / 1000 / 2 * 1000) + m_StressConfig.lStressValidRange;		// mm小数点以下は、切り落とし
// 2009.10.19 K.Matsuo 有効測定範囲の設定に変更 <--
			m_StressConfig.Line[i].SectPos[j].lScanPosY = PinY;
		}
	}
}

void CStressLineSectionSettingDlg::SetDefaultNecessaryThicknessMeasurementData()
{
	long	LineInterval, PinInterval;
	long	PinX, PinY;
// 2009.09.08 bagus stress 原点位置対応 --{--
	double	PinOrgX, PinOrgY;
// 2009.09.08 bagus stress 原点位置対応 --}--
// 2009.09.11 bagus stress 修正 --{--
	double	PinMaxX, PinMaxY;
// 2009.09.11 bagus stress 修正 --}--

	if (m_StressConfig.dwLiftPinNumberOfLine <= 1) {
		LineInterval = 0;
	} else {
		LineInterval = (long)((((m_StressConfig.Size.dy - (m_StressConfig.lInvalidRangeEdge * 2)) / (m_StressConfig.dwLiftPinNumberOfLine - 1)) * 1000 + 500) / 1000);
	}

// 2009.09.11 bagus stress 修正 --{--
//	PinInterval = (long)((((m_StressConfig.Size.dx - (m_StressConfig.lInvalidRangeEdge * 2)) / PIN_INTERVAL) * 1000 + 500) / 1000);
	PinInterval = (long)((((m_StressConfig.Size.dx - (m_StressConfig.lInvalidRangeEdge * 2)) / (PIN_INTERVAL - 1)) * 1000 + 500) / 1000);
// 2009.09.11 bagus stress 修正 --}--

	int	i ,j;

// 2009.09.08 bagus stress 原点位置対応 --{--
//	PinY = (long)((((m_StressConfig.Size.dy / 2) - m_StressConfig.lInvalidRangeEdge) * 1000 + 500) / 1000);
//	PinY *= -1;
	CheckPinMinPos(&PinOrgX, &PinOrgY);
// 2009.09.11 bagus stress 修正 --{--
	CheckPinMaxPos(&PinMaxX, &PinMaxY);
// 2009.09.11 bagus stress 修正 --}--
	if (PinOrgY == 0) {
		PinOrgY += m_StressConfig.lInvalidRangeEdge;
		PinY = PinOrgY;
	} else if (PinOrgY < 0) {
		PinOrgY += m_StressConfig.lInvalidRangeEdge;
		PinY = (long)((PinOrgY * 1000 - 500) / 1000);
	} else {
		PinOrgY -= m_StressConfig.lInvalidRangeEdge;
		PinY = (long)((PinOrgY * 1000 + 500) / 1000);
	}
// 2009.09.11 bagus stress 修正 --{--
//	if (PinOrgX == 0) {
//		PinOrgX += m_StressConfig.lInvalidRangeEdge;
//	} else if (PinOrgX < 0) {
//		PinOrgX += m_StressConfig.lInvalidRangeEdge;
//	} else {
//		PinOrgX -= m_StressConfig.lInvalidRangeEdge;
//	}
// 2009.09.11 bagus stress 修正 --}--
// 2009.09.08 bagus stress 原点位置対応 --}--
	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
// 2009.09.08 bagus stress 原点位置対応 --{--
//		PinX = (long)((((m_StressConfig.Size.dx / 2) - m_StressConfig.lInvalidRangeEdge) * 1000 + 500) / 1000);
//		PinX *= -1;
		if (PinOrgX == 0) {
			PinX = PinOrgX;
		} else if (PinOrgX < 0) {
			PinX = (long)((PinOrgX * 1000 - 500) / 1000);
		} else {
			PinX = (long)((PinOrgX * 1000 + 500) / 1000);
		}
// 2009.09.08 bagus stress 原点位置対応 --}--
		for (j=0; j<PIN_INTERVAL; j++) {
// 2009.09.11 bagus stress 修正 --{--
//			m_StressConfig.Line[i].MeasPos[j].lX = PinX;
//			m_StressConfig.Line[i].MeasPos[j].lY = PinY;
			if (j== PIN_INTERVAL-1) {
				m_StressConfig.Line[i].MeasPos[j].lX = PinMaxX;
//				m_StressConfig.Line[i].MeasPos[j].lY = PinMaxY;
			} else {
				m_StressConfig.Line[i].MeasPos[j].lX = PinX;
			}
// 2009.09.17 bagus Stress --{--
//			m_StressConfig.Line[i].MeasPos[j].lY = PinY;
			m_StressConfig.Line[i].MeasPos[j].lY = m_StressConfig.Line[i].SectPos[0].lScanPosY;
// 2009.09.17 bagus Stress --}--
// 2009.09.11 bagus stress 修正 --}--

// 2009.09.11 bagus stress 修正 --{--
//			PinX += PinInterval;
			PinX += PinInterval + m_StressConfig.MeasOffset.lX;
// 2009.09.11 bagus stress 修正 --}--
		}

		PinY += LineInterval;
	}
}

BOOL CStressLineSectionSettingDlg::CheckLineValidData()
{
// 2009.09.17 bagus stress Grid修正 --{--
//	int	LineCols, LineRows;

//	LineCols = m_LineValidListGrid.GetColumnCount();
//	LineRows = m_LineValidListGrid.GetRowCount();
// 2009.09.17 bagus stress Grid修正 --}--

	GV_ITEM Item;

	int FixRows = 1;
	int FixCols = 1;
	int	row;
//	DWORD	dwTextStyle;
	long	lLineValid;

// 2009.09.17 bagus stress Grid修正 --{--
//	for (row=FixRows; row<LineRows; row++) {
	for (row=FixRows; row<m_StressConfig.dwLiftPinNumberOfLine+FixRows; row++) {
// 2009.09.17 bagus stress Grid修正 --}--
		Item.mask = GVIF_TEXT;
		Item.row = row;
		Item.col = 1;
		//
		m_LineValidListGrid.GetItem(&Item);
#if 1
//		lLineValid = m_LineCheckBox.CheckBox[row-1]->GetCheck();
		lLineValid = ((CGridCellCheck *)m_LineValidListGrid.GetCell(row,1))->GetCheck();
#else
		lLineValid = strtol(Item.strText, NULL, 10);
#endif

		/*
		dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		dwTextStyle |= DT_END_ELLIPSIS;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 1;
		Item.nFormat = dwTextStyle;
		if (lLineValid <= 0) {
			m_StressConfig.Line[row - FixRows].bValidLine = FALSE;
			Item.strText.Format("0");
			m_LineValidListGrid.SetItem(&Item);
		} else {
			m_StressConfig.Line[row - FixRows].bValidLine = TRUE;
			Item.strText.Format("1");
			m_LineValidListGrid.SetItem(&Item);
		}
		*/
		if (lLineValid <= 0) {
			m_StressConfig.Line[row - FixRows].bValidLine = FALSE;
			//2009.09.14 bagus stress --{--
			m_bOldLineValid[row - FixRows] = FALSE;
			//2009.09.14 bagus stress --}--
		}else {
			m_StressConfig.Line[row - FixRows].bValidLine = TRUE;
			//2009.09.14 bagus stress --{--
			m_bOldLineValid[row - FixRows] = TRUE;
			//2009.09.14 bagus stress --}--
		}
	}

	return ( TRUE );
}

void CStressLineSectionSettingDlg::SamplePointGraph_Init()
{
	CString strBuffer;
	CNanoPointGraph::GraphConfig tConfig;

	m_SamplePointGraph.ShowWindow(SW_HIDE);

	////Point Graph Data Reset
// 2009.09.11 bagus stress 修正 --{--
//	m_SamplePointGraph.Reset();
//	m_SamplePointGraph.ResetSection();
	m_SamplePointGraph.ResetAll();
// 2009.09.11 bagus stress 修正 --}--

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
	//2009.09.10 bagus stress --{--
	tConfig.PinColor = RGB(0xFF,0x00,0xFF);
	//2009.09.10 bagus stress --}--

	// 2013.01.09 bagus -->
	tConfig.OriginColor = GREEN_COLOR;
	// 2013.01.09 bagus -->

	////CNanoPointGraphクラス内サイズ(Sample Size)
// 2009.09.07 bagus stress 原点位置対応 --{--
//	tConfig.SampleSizeX = m_SampleInfo.Size.dx;
//	tConfig.SampleSizeY = m_SampleInfo.Size.dy;
	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
		// X<->Y入れ替え
		tConfig.SampleSizeY = m_SampleInfo.Size.dx;
		tConfig.SampleSizeX = m_SampleInfo.Size.dy;
	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
		// X-Yそのまま
		tConfig.SampleSizeX = m_SampleInfo.Size.dx;
		tConfig.SampleSizeY = m_SampleInfo.Size.dy;
	} else {
		// 矢印設定がおかしい
	}
// 2009.09.07 bagus stress 原点位置対応 --}--
	m_SamplePointGraph.SetConfig(&tConfig);

	SamplePointGraph_DataSet();
}

void CStressLineSectionSettingDlg::SamplePointGraph_DataSet()
{
	////Point Graph Data Reset
// 2009.09.11 bagus stress 修正 --{--
//	m_SamplePointGraph.Reset();
//	m_SamplePointGraph.ResetSection();
	m_SamplePointGraph.ResetAll();
// 2009.09.11 bagus stress 修正 --}--

	CNanoPointGraph::LSData tLineData;

	int i, j;

	////Point Data
	//センター原点の場合
// 2009.09.07 bagus stress 原点位置対応 --{--
//	double dOffSetValueX = (m_SampleInfo.Size.dx/2);
//	double dOffSetValueY = (m_SampleInfo.Size.dy/2);
	double dOffSetValueX;
	double dOffSetValueY;
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
		dOffSetValueX = (m_SampleInfo.Size.dx/2);
		dOffSetValueY = (m_SampleInfo.Size.dy/2);
		break;
	case 2:
		dOffSetValueX = 0;
// 2009.09.08 bagus stress 原点位置対応 --{--
//		dOffSetValueY = m_SampleInfo.Size.dy;
		dOffSetValueY = 0;
// 2009.09.08 bagus stress 原点位置対応 --}--
		break;
	case 3:
		dOffSetValueX = 0;
// 2009.09.08 bagus stress 原点位置対応 --{--
//		dOffSetValueY = 0;
		dOffSetValueY = m_SampleInfo.Size.dy;
// 2009.09.08 bagus stress 原点位置対応 --}--
		break;
	case 4:
		dOffSetValueX = m_SampleInfo.Size.dx;
// 2009.09.08 bagus stress 原点位置対応 --{--
//		dOffSetValueY = 0;
		dOffSetValueY = m_SampleInfo.Size.dy;
// 2009.09.08 bagus stress 原点位置対応 --}--
		break;
	case 5:
		dOffSetValueX = m_SampleInfo.Size.dx;
// 2009.09.08 bagus stress 原点位置対応 --{--
//		dOffSetValueY = m_SampleInfo.Size.dy;
		dOffSetValueY = 0;
// 2009.09.08 bagus stress 原点位置対応 --}--
		break;
	}
// 2009.09.07 bagus stress 原点位置対応 --}--

// 2013.01.09 bagus -->
	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		m_SamplePointGraph.SetOriginPointData(dOffSetValueX, dOffSetValueY);
	}
	else {
		m_SamplePointGraph.SetOriginPointData(dOffSetValueY, dOffSetValueX);
	}
// 2013.01.09 bagus <--

	for (i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
		if(i >= STRESS_PINS_MAX){
			break;
		}

//2009.09.10 bagus stress --{--
// Pin Data
		for(j = 0; j<m_StressConfig.Line[i].dwLiftPinNumberOfLn;j++){
			long lPinX,lPinY;
			lPinX = lPinY = 0L;
			switch (m_StageConfig.Dir.X) {
			case 1:		// X:→
			default:
				lPinX = dOffSetValueX + m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 2:		// X:↑
				lPinY = dOffSetValueX - m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 3:		// X:←
				lPinX = dOffSetValueX - m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 4:		// X:↓
				lPinY = dOffSetValueX + m_StressConfig.Line[i].PinPos[j].lX;
				break;
			}

			switch (m_StageConfig.Dir.Y) {
			case 1:		// Y:→
				lPinX = dOffSetValueY + m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 2:		// Y:↑
			default:
				lPinY = dOffSetValueY - m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 3:		// Y:←
				lPinX = dOffSetValueY - m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 4:		// Y:↓
				lPinY = dOffSetValueY + m_StressConfig.Line[i].PinPos[j].lY;
				break;
			}
			m_SamplePointGraph.AddPinData(lPinX,lPinY);
		}
//2009.09.10 bagus stress --}--
		for (j=0; j<m_StressConfig.Line[i].dwSectionNum; j++) {
			//2009.09.14 bagus stress --{--
			if(i+1 >= m_LineListGrid.GetRowCount()){
				continue;
			}
			//if (m_StressConfig.Line[i].bValidLine) {
			if(((CGridCellCheck *)m_LineValidListGrid.GetCell(i+1,1))->GetCheck()){
			//2009.09.14 bagus stress --}--
				tLineData.Status = CNanoPointGraph::SECTION_NOT_MEASURED;
			} else {
				tLineData.Status = CNanoPointGraph::SECTION_NOT_USED;
			}
			if(j >= STRESS_SECTIONS_MAX){
				break;
			}

// 2009.09.07 bagus stress 原点位置対応 --{--
//			tLineData.StartX = m_StressConfig.Line[i].SectPos[j].lScanStartPosX + dOffSetValueX;
//			tLineData.StartY = -(m_StressConfig.Line[i].SectPos[j].lScanPosY - dOffSetValueY);
//			tLineData.EndX = m_StressConfig.Line[i].SectPos[j].lScanEndPosX + dOffSetValueX;
//			tLineData.EndY = -(m_StressConfig.Line[i].SectPos[j].lScanPosY - dOffSetValueY);
			switch (m_StageConfig.Dir.X) {
			case 1:		// X:→
			default:
				tLineData.StartX = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndX = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 2:		// X:↑
				tLineData.StartY = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndY = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 3:		// X:←
				tLineData.StartX = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndX = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 4:		// X:↓
				tLineData.StartY = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndY = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			}

			switch (m_StageConfig.Dir.Y) {
			case 1:		// Y:→
				tLineData.StartX = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndX = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 2:		// Y:↑
			default:
				tLineData.StartY = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndY = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 3:		// Y:←
				tLineData.StartX = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndX = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 4:		// Y:↓
				tLineData.StartY = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndY = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			}
// 2009.09.07 bagus stress 原点位置対応 --}--

			m_SamplePointGraph.AddLineData(&tLineData);
		}
	}

// 2009.09.07 bagus stress 原点位置対応 --{--
	double	dPointX, dPointY;
// 2009.09.07 bagus stress 原点位置対応 --}--
	for (i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
// 2009.09.11 bagus stress 修正 --{--
		if (!m_StressConfig.Line[i].bValidLine) {
			continue;
		}
// 2009.09.11 bagus stress 修正 --}--

		for (j=0; j<PIN_INTERVAL; j++) {
// 2009.09.07 bagus stress 原点位置対応 --{--
//			m_SamplePointGraph.AddPoint((m_StressConfig.Line[i].MeasPos[j].lX + dOffSetValueX), (-(m_StressConfig.Line[i].MeasPos[j].lY - dOffSetValueY)));
			switch (m_StageConfig.Dir.X) {
			case 1:		// X:→
			default:
				dPointX = dOffSetValueX + m_StressConfig.Line[i].MeasPos[j].lX;
				break;
			case 2:		// X:↑
				dPointY = dOffSetValueX - m_StressConfig.Line[i].MeasPos[j].lX;
				break;
			case 3:		// X:←
				dPointX = dOffSetValueX - m_StressConfig.Line[i].MeasPos[j].lX;
				break;
			case 4:		// X:↓
				dPointY = dOffSetValueX + m_StressConfig.Line[i].MeasPos[j].lX;
				break;
			}

			switch (m_StageConfig.Dir.Y) {
			case 1:		// Y:→
				dPointX = dOffSetValueY + m_StressConfig.Line[i].MeasPos[j].lY;
				break;
			case 2:		// Y:↑
			default:
				dPointY = dOffSetValueY - m_StressConfig.Line[i].MeasPos[j].lY;
				break;
			case 3:		// Y:←
				dPointX = dOffSetValueY - m_StressConfig.Line[i].MeasPos[j].lY;
				break;
			case 4:		// Y:↓
				dPointY = dOffSetValueY + m_StressConfig.Line[i].MeasPos[j].lY;
				break;
			}

			m_SamplePointGraph.AddPoint(dPointX, dPointY);
// 2009.09.07 bagus stress 原点位置対応 --}--
		}
	}
}

void CStressLineSectionSettingDlg::DrawSample(CDC* pDC, int iOrg, int DirX, int DirY)
{
	WINDOWPLACEMENT Stageplacement, Sampleplacement;

	///// Region Map /////
	GetDlgItem(IDC_MEASUREMENT_MAP)->GetWindowPlacement(&Stageplacement);

	///// ステージ有効範囲(黒い四角はこのサイズで描画されている) /////
	//double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
	//double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
	double dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);

// 2013.01.09 bagus -->
	double dSampleSizeX;
	double dSampleSizeY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSizeX = m_SampleInfo.Size.dx;
		dSampleSizeY = m_SampleInfo.Size.dy;
	}
	else {
		dSampleSizeX = m_SampleInfo.Size.dy;
		dSampleSizeY = m_SampleInfo.Size.dx;
	}

	///// 　サンプルサイズがステージの大きさとり小さいかチェック /////
// 	if ((m_SampleInfo.Size.dx > dStageSizeX) || (m_SampleInfo.Size.dy > dStageSizeY))
// 		return;
	if ((dSampleSizeX > dStageSizeX) || (dSampleSizeY > dStageSizeY))
		return;
// 2013.01.09 bagus <--

	///// サンプル描画サイズの計算（ステージ描画サイズ対するサンプル描画サイズを計算） /////
	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX) * m_SampleInfo.Size.dx / dStageSizeX;
	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY) * m_SampleInfo.Size.dy / dStageSizeY;

	///// ステージとサンプルの原点オフセットの合計値 /////
	double dTotalOrgOffsetPosX, dTotalOrgOffsetPosY;

	///// サンプルの描画位置 /////
	double dtop, dbottom, dright, dleft;

	//Sample描画位置設定
	//四角形状サンプルのみ対応
	switch(m_SampleInfo.nShape){
	case SAMPLE_SHAPE_RECTANGLE:		//Sample shaped RectAngle
		switch(iOrg){
		case 1: ///// センター基準	//X軸：→+　Y軸：↑+のみ対応
		default:
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
			//dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			//dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;

			///// サンプル描画位置を計算 /////
			//左基準
			dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX / 2);
			dright	= dleft + dDrawSampleSizeX;
			//下基準
			dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + (dDrawSampleSizeY / 2);
			dtop	= dbottom - dDrawSampleSizeY;
			break;

		case 2: /////LeftTop
// 2009.09.07 bagus stress 原点位置対応 --{--
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2009.09.11 bagus stress 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
// 2009.09.11 bagus stress 修正 --}--

			///// サンプル描画位置を計算 /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom	= dtop + dDrawSampleSizeY;
			break;
// 2009.09.07 bagus stress 原点位置対応 --}--
		case 3: /////LeftBottom
// 2009.09.07 bagus stress 原点位置対応 --{--
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2009.09.11 bagus stress 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			dTotalOrgOffsetPosX = ((m_SampleInfo.Origin.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX);
			dTotalOrgOffsetPosY = ((m_SampleInfo.Origin.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY);
// 2009.09.11 bagus stress 修正 --}--

			///// サンプル描画位置を計算 /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
			break;
// 2009.09.07 bagus stress 原点位置対応 --}--
		case 4: /////RightBottom
// 2009.09.07 bagus stress 原点位置対応 --{--
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2009.09.11 bagus stress 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
// 2009.09.11 bagus stress 修正 --}--

			///// サンプル描画位置を計算 /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
			break;
// 2009.09.07 bagus stress 原点位置対応 --}--
		case 5: /////RightTop
// 2009.09.07 bagus stress 原点位置対応 --{--
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
// 2009.09.11 bagus stress 修正 --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
// 2009.09.11 bagus stress 修正 --}--

			///// サンプル描画位置を計算 /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom	= dtop + dDrawSampleSizeY;
// 2009.09.07 bagus stress 原点位置対応 --}--
			break;
		}

		if ((m_dDrawStartStagePosX <= dleft) && (dright <= m_dDrawEndStagePosX) && (m_dDrawStartStagePosY <= dtop) && (dbottom <= m_dDrawEndStagePosY)){
			Sampleplacement.rcNormalPosition.left	= Stageplacement.rcNormalPosition.left + dleft;
			Sampleplacement.rcNormalPosition.right	= Stageplacement.rcNormalPosition.left + dright;
			Sampleplacement.rcNormalPosition.top	= Stageplacement.rcNormalPosition.top  + dtop;
			Sampleplacement.rcNormalPosition.bottom = Stageplacement.rcNormalPosition.top  + dbottom;
			//四角Sample描画
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

// 2013.01.09 bagus -->
// void CStressLineSectionSettingDlg::DrawOriginMaker(int Origin)
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


// 2009.09.04 bagus stress ライン数設定ボタン追加 --{--
void CStressLineSectionSettingDlg::OnLinesetButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
// 2009.09.08 bagus stress LineNumのチェック --{--
#if 0
	CString	sNumOfLine;
	DWORD	NumOfLine;

	GetDlgItemText(IDC_NUMBER_OF_LINE, sNumOfLine);
	NumOfLine = strtol(sNumOfLine, NULL, 10);

	if (NumOfLine < 0) {
		NumOfLine = 0;

		sNumOfLine.Format("%d", NumOfLine);
		SetDlgItemText(IDC_NUMBER_OF_LINE, sNumOfLine);
	}
	if (NumOfLine > STRESS_LINES_MAX) {
		NumOfLine = STRESS_LINES_MAX;

		sNumOfLine.Format("%d", NumOfLine);
		SetDlgItemText(IDC_NUMBER_OF_LINE, sNumOfLine);
	}

	m_StressConfig.dwLiftPinNumberOfLine = NumOfLine;
// 2009.09.08 bagus stress 修正 --{--
	for (int i=0; i<STRESS_LINES_MAX; i++) {
		if (i<NumOfLine) {
//			m_StressConfig.Line[i].bValidLine = TRUE;
		} else {
			m_StressConfig.Line[i].bValidLine = FALSE;
		}
	}
// 2009.09.08 bagus stress 修正 --}--
#else
	if (!CheckLineNum()) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:CheckLineNum");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_LINE_NUM, strBuffer, "Please input line number.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}
#endif
// 2009.09.08 bagus stress LineNumのチェック --}--

	SetLineData();
	SetLineValidData();

// 2009.09.08 bagus stress 修正 --{--
	PinListGrid_Init();
	StressMeasurementLineSectionListGrid_Init();
	NecessaryThicknessMeasurementListGrid_Init();
// 2009.09.08 bagus stress 修正 --}--
}
// 2009.09.04 bagus stress ライン数設定ボタン追加 --}--

// 2009.09.04 bagus stress 数値チェック追加 --{--
BOOL CStressLineSectionSettingDlg::CheckStressValidRangeData()
{
	CString	strBuffer;

	GetDlgItemText(IDC_STRESS_VALID_RANGE, strBuffer);
// 2009.09.15 bagus stress 修正 --{--
#if 0
	if(CheckMinMaxLong(strBuffer, 0, (m_StressConfig.Size.dx / MICROMETRE)) != CHECK_DATA_OK){
		GetDlgItem(IDC_STRESS_VALID_RANGE)->SetFocus();
		return FALSE;
	}
#else
	long	tmpVal;

	if (!CheckInputValueLong(strBuffer, &tmpVal))
		return FALSE;

	if ((tmpVal < 50) || ((m_StressConfig.Size.dx / MICROMETRE) < tmpVal))
		return FALSE;
#endif
// 2009.09.15 bagus stress 修正 --}--
	m_StressConfig.lStressValidRange = (long)(strtod(strBuffer.GetBuffer(0), NULL) * MICROMETRE);	// 単位変換(mm->um)

	return ( TRUE );
}

BOOL CStressLineSectionSettingDlg::CheckInvalidRangeEdgeData()
{
	CString	strBuffer;

	GetDlgItemText(IDC_INVALID_RANGE_EDGE, strBuffer);
// 2009.09.15 bagus stress 修正 --{--
#if 0
	if(CheckMinMaxLong(strBuffer, 0, (m_StressConfig.Size.dx / MICROMETRE)) != CHECK_DATA_OK){
		GetDlgItem(IDC_INVALID_RANGE_EDGE)->SetFocus();
		return FALSE;
	}
#else
	long	tmpVal;

	if (!CheckInputValueLong(strBuffer, &tmpVal))
		return FALSE;

	if ((tmpVal < 0) || ((m_StressConfig.Size.dx / MICROMETRE) < tmpVal))
		return FALSE;
#endif
// 2009.09.15 bagus stress 修正 --}--
	m_StressConfig.lInvalidRangeEdge = (long)(strtod(strBuffer.GetBuffer(0), NULL) * MICROMETRE);	// 単位変換(mm->um)

	return ( TRUE );
}

BOOL CStressLineSectionSettingDlg::CheckThinkMeasurementPointOffsetXData()
{
	CString	strBuffer;

	GetDlgItemText(IDC_THICK_MEASUREMENT_POINT_OFFSET_X, strBuffer);
// 2009.09.15 bagus stress 修正 --{--
#if 0
	if(CheckMinMaxLong(strBuffer, 0, (m_StressConfig.Size.dx / MICROMETRE)) != CHECK_DATA_OK){
		GetDlgItem(IDC_THICK_MEASUREMENT_POINT_OFFSET_X)->SetFocus();
		return FALSE;
	}
#else
	long	tmpVal;

	if (!CheckInputValueLong(strBuffer, &tmpVal))
		return FALSE;

// 2009.09.16 bagus stress 修正 --{--
//	if ((tmpVal < 0) || ((m_StressConfig.Size.dx / MICROMETRE) < tmpVal))
	if ((tmpVal < (-(m_StressConfig.Size.dx / MICROMETRE))) || ((m_StressConfig.Size.dx / MICROMETRE) < tmpVal))
// 2009.09.16 bagus stress 修正 --}--
		return FALSE;
#endif
// 2009.09.15 bagus stress 修正 --}--
	m_StressConfig.MeasOffset.lX = (long)(strtod(strBuffer.GetBuffer(0), NULL) * MICROMETRE);	// 単位変換(mm->um)

	return ( TRUE );
}
// 2009.09.04 bagus stress 数値チェック追加 --}--

// 2009.09.08 bagus stress 原点位置対応 --{--
BOOL CStressLineSectionSettingDlg::CheckPinPos(long lX, long lY)
{
	double	dXMax, dXMin;
	double	dYMax, dYMin;
//	double	dTmp;

	// X:→ , Y:↑ 基準で計算
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = m_StressConfig.Size.dx / 2;
//		dXMin = -(m_StressConfig.Size.dx / 2);
		dXMax = (m_StressConfig.Size.dx / 2) - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx / 2) + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = m_StressConfig.Size.dy / 2;
		dYMin = -(m_StressConfig.Size.dy / 2);
		break;
	case 2:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = m_StressConfig.Size.dx;
//		dXMin = 0;
		dXMax = m_StressConfig.Size.dx - m_StressConfig.lInvalidRangeEdge;
		dXMin = 0 + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = 0;
		dYMin = -(m_StressConfig.Size.dy);
		break;
	case 3:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = m_StressConfig.Size.dx;
//		dXMin = 0;
		dXMax = m_StressConfig.Size.dx - m_StressConfig.lInvalidRangeEdge;
		dXMin = 0 + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = m_StressConfig.Size.dy;
		dYMin = 0;
		break;
	case 4:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = 0;
//		dXMin = -(m_StressConfig.Size.dx);
		dXMax = 0 - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx) + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = m_StressConfig.Size.dy;
		dYMin = 0;
		break;
	case 5:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = 0;
//		dXMin = -(m_StressConfig.Size.dx);
		dXMax = 0 - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx) + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = 0;
		dYMin = -(m_StressConfig.Size.dy);
		break;
	}

/*
	switch (m_StageConfig.Dir.X) {
	case 1:		// X:→
	default:
		// 基準のまま
		break;
	case 2:		// X:↑
		dTmp = dXMax;
		dXMax = dYMax;
		dYMax = dTmp;
		dTmp = dXMin;
		dXMin = dYMin;
		dYMin = dTmp;
		break;
	case 3:		// X:←
		dXMax *= -1;
		dXMin *= -1;
		break;
	case 4:		// X:↓
		dTmp = dXMax;
		dXMax = dYMax;
		dYMax = dTmp;
		dTmp = dXMin;
		dXMin = dYMin;
		dYMin = dTmp;
		dYMax *= -1;
		dYMin *= -1;
		break;
	}

	switch (m_StageConfig.Dir.Y) {
	case 1:		// Y:→
		// Xが2or4なので入れ替え済み
		break;
	case 2:		// Y:↑
	default:
		// 基準のまま
		break;
	case 3:		// Y:←
		// Xが2or4なので入れ替え済み
		dXMax *= -1;
		dXMin *= -1;
		break;
	case 4:		// Y:↓
		dYMax *= -1;
		dYMin *= -1;
		break;
	}
*/

	// 範囲チェック
	if ((lX < dXMin) || (lX > dXMax)) {
		return ( FALSE );
	}
	if ((lY < dYMin) && (lY > dYMax)) {
		return ( FALSE );
	}

	return ( TRUE );
}

void CStressLineSectionSettingDlg::CheckPinMinPos(double *dX, double *dY)
{
	double	dXMax, dXMin;
	double	dYMax, dYMin;
//	double	dTmp;

	// X:→ , Y:↑ 基準で計算
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = m_StressConfig.Size.dx / 2;
//		dXMin = -(m_StressConfig.Size.dx / 2);
		dXMax = (m_StressConfig.Size.dx / 2) - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx / 2) + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = m_StressConfig.Size.dy / 2;
		dYMin = -(m_StressConfig.Size.dy / 2);
		break;
	case 2:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = m_StressConfig.Size.dx;
//		dXMin = 0;
		dXMax = m_StressConfig.Size.dx - m_StressConfig.lInvalidRangeEdge;
		dXMin = 0 + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = 0;
		dYMin = -(m_StressConfig.Size.dy);
		break;
	case 3:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = m_StressConfig.Size.dx;
//		dXMin = 0;
		dXMax = m_StressConfig.Size.dx - m_StressConfig.lInvalidRangeEdge;
		dXMin = 0 + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = m_StressConfig.Size.dy;
		dYMin = 0;
		break;
	case 4:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = 0;
//		dXMin = -(m_StressConfig.Size.dx);
		dXMax = 0 - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx) + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = m_StressConfig.Size.dy;
		dYMin = 0;
		break;
	case 5:
// 2009.09.11 bagus stress 修正 --{--
//		dXMax = 0;
//		dXMin = -(m_StressConfig.Size.dx);
		dXMax = 0 - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx) + m_StressConfig.lInvalidRangeEdge;
// 2009.09.11 bagus stress 修正 --}--
		dYMax = 0;
		dYMin = -(m_StressConfig.Size.dy);
		break;
	}

/*
	switch (m_StageConfig.Dir.X) {
	case 1:		// X:→
	default:
		// 基準のまま
		break;
	case 2:		// X:↑
		dTmp = dXMax;
		dXMax = dYMax;
		dYMax = dTmp;
		dTmp = dXMin;
		dXMin = dYMin;
		dYMin = dTmp;
		break;
	case 3:		// X:←
		dXMax *= -1;
		dXMin *= -1;
		break;
	case 4:		// X:↓
		dTmp = dXMax;
		dXMax = dYMax;
		dYMax = dTmp;
		dTmp = dXMin;
		dXMin = dYMin;
		dYMin = dTmp;
		dYMax *= -1;
		dYMin *= -1;
		break;
	}

	switch (m_StageConfig.Dir.Y) {
	case 1:		// Y:→
		// Xが2or4なので入れ替え済み
		break;
	case 2:		// Y:↑
	default:
		// 基準のまま
		break;
	case 3:		// Y:←
		// Xが2or4なので入れ替え済み
		dXMax *= -1;
		dXMin *= -1;
		break;
	case 4:		// Y:↓
		dYMax *= -1;
		dYMin *= -1;
		break;
	}
*/

	*dX = dXMin;
	*dY = dYMin;
}
// 2009.09.08 bagus stress 原点位置対応 --}--

// 2009.09.08 bagus stress LineNumのチェック --{--
BOOL CStressLineSectionSettingDlg::CheckLineNum()
{
	CString	sNumOfLine;
// 2009.09.15 bagus stress 修正 --{--
//	DWORD	NumOfLine;
	long	NumOfLine;
// 2009.09.15 bagus stress 修正 --}--

	GetDlgItemText(IDC_NUMBER_OF_LINE, sNumOfLine);
// 2009.09.15 bagus stress 修正 --{--
//	NumOfLine = strtol(sNumOfLine, NULL, 10);

	if (!CheckInputValueLong(sNumOfLine, &NumOfLine))
		return ( FALSE );
// 2009.09.15 bagus stress 修正 --}--

// 2009.09.15 bagus stress 修正 --{--
//	if (NumOfLine < 0) {
	if (NumOfLine <= 0) {
// 2009.09.15 bagus stress 修正 --}--
		return ( FALSE );
	}
	if (NumOfLine > STRESS_LINES_MAX) {
		return ( FALSE );
	}

	m_StressConfig.dwLiftPinNumberOfLine = NumOfLine;
// 2009.09.15 bagus stress 修正 --{--
	sNumOfLine.Format("%ld", NumOfLine);
	SetDlgItemText(IDC_NUMBER_OF_LINE, sNumOfLine);
// 2009.09.15 bagus stress 修正 --}--

// 2009.09.08 bagus stress 修正 --{--
	for (int i=0; i<STRESS_LINES_MAX; i++) {
		if (i<NumOfLine) {
//			m_StressConfig.Line[i].bValidLine = TRUE;
		} else {
			m_StressConfig.Line[i].bValidLine = FALSE;
		}
	}
// 2009.09.08 bagus stress 修正 --}--

	return ( TRUE );
}
// 2009.09.08 bagus stress LineNumのチェック --}--

BOOL CStressLineSectionSettingDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CDialog::OnNotify(wParam, lParam, pResult);
}


void CStressLineSectionSettingDlg::OnTimer(UINT nIDEvent)
{
//	AdjustValidCheckBoxPos();
//	CheckLineValidData();
	//2009.09.14 bagus stress --{--
	//チェックボックスの中身が変わったら表示を変更する
	BOOL bChanged = FALSE;
	for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
// 2009.09.17 bagus stress Grid修正 --{--
//		if(nLine + 1 < m_LineValidListGrid.GetRowCount()){
		if(nLine + 1 < m_StressConfig.dwLiftPinNumberOfLine){
// 2009.09.17 bagus stress Grid修正 --}--
			if(m_bOldLineValid[nLine] != ((CGridCellCheck *)m_LineValidListGrid.GetCell(nLine+1,1))->GetCheck()){
				m_bOldLineValid[nLine] = ((CGridCellCheck *)m_LineValidListGrid.GetCell(nLine+1,1))->GetCheck();
				bChanged = TRUE;
			}
		}else{
			m_bOldLineValid[nLine] = FALSE;
		}
	}
	if(bChanged)
		SamplePointGraph_DataSet();

	CDialog::OnTimer(nIDEvent);
}

// 2009.09.11 bagus stress 原点位置対応 --{--
void CStressLineSectionSettingDlg::CheckPinMaxPos(double *dX, double *dY)
{
	double	dXMax, dXMin;
	double	dYMax, dYMin;
//	double	dTmp;

	// X:→ , Y:↑ 基準で計算
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
		dXMax = (m_StressConfig.Size.dx / 2) - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx / 2) + m_StressConfig.lInvalidRangeEdge;
		dYMax = m_StressConfig.Size.dy / 2;
		dYMin = -(m_StressConfig.Size.dy / 2);
		break;
	case 2:
		dXMax = m_StressConfig.Size.dx - m_StressConfig.lInvalidRangeEdge;
		dXMin = 0 + m_StressConfig.lInvalidRangeEdge;
		dYMax = 0;
		dYMin = -(m_StressConfig.Size.dy);
		break;
	case 3:
		dXMax = m_StressConfig.Size.dx - m_StressConfig.lInvalidRangeEdge;
		dXMin = 0 + m_StressConfig.lInvalidRangeEdge;
		dYMax = m_StressConfig.Size.dy;
		dYMin = 0;
		break;
	case 4:
		dXMax = 0 - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx) + m_StressConfig.lInvalidRangeEdge;
		dYMax = m_StressConfig.Size.dy;
		dYMin = 0;
		break;
	case 5:
		dXMax = 0 - m_StressConfig.lInvalidRangeEdge;
		dXMin = -(m_StressConfig.Size.dx) + m_StressConfig.lInvalidRangeEdge;
		dYMax = 0;
		dYMin = -(m_StressConfig.Size.dy);
		break;
	}

/*
	switch (m_StageConfig.Dir.X) {
	case 1:		// X:→
	default:
		// 基準のまま
		break;
	case 2:		// X:↑
		dTmp = dXMax;
		dXMax = dYMax;
		dYMax = dTmp;
		dTmp = dXMin;
		dXMin = dYMin;
		dYMin = dTmp;
		break;
	case 3:		// X:←
		dXMax *= -1;
		dXMin *= -1;
		break;
	case 4:		// X:↓
		dTmp = dXMax;
		dXMax = dYMax;
		dYMax = dTmp;
		dTmp = dXMin;
		dXMin = dYMin;
		dYMin = dTmp;
		dYMax *= -1;
		dYMin *= -1;
		break;
	}

	switch (m_StageConfig.Dir.Y) {
	case 1:		// Y:→
		// Xが2or4なので入れ替え済み
		break;
	case 2:		// Y:↑
	default:
		// 基準のまま
		break;
	case 3:		// Y:←
		// Xが2or4なので入れ替え済み
		dXMax *= -1;
		dXMin *= -1;
		break;
	case 4:		// Y:↓
		dYMax *= -1;
		dYMin *= -1;
		break;
	}
*/

	*dX = dXMax;
	*dY = dYMax;
}
// 2009.09.11 bagus stress 原点位置対応 --}--

// 2009.09.11 bagus stress チェック追加 --{--
BOOL CStressLineSectionSettingDlg::CheckStressMeasurementLineSectionData()
{
	int	i, j;
	int Rows;

	long	SectionLength;

	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
		if (!m_StressConfig.Line[i].bValidLine) {
			continue;
		}
		Rows = m_StressConfig.Line[i].dwSectionNum;
		for (j=0; j<Rows; j++) {
			SectionLength = labs(m_StressConfig.Line[i].SectPos[j].lScanEndPosX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX);
			if (SectionLength < SECTION_MIN_RANGE)
				return ( FALSE );
		}
	}

	return ( TRUE );
}

BOOL CStressLineSectionSettingDlg::CheckNecessaryThicknessMeasurementData()
{
	int	i;

	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
		if (!m_StressConfig.Line[i].bValidLine) {
			continue;
		}

		if (m_StressConfig.Line[i].MeasPos[1].lX < m_StressConfig.Line[i].MeasPos[0].lX) {
			// X:→でのみ動作
			return ( FALSE );
		}

		if (m_StressConfig.Line[i].MeasPos[PIN_INTERVAL-2].lX > m_StressConfig.Line[i].MeasPos[PIN_INTERVAL-1].lX) {
			// X:→でのみ動作
			return ( FALSE );
		}
	}

	return ( TRUE );
}
// 2009.09.11 bagus stress チェック追加 --}--

// 2009.09.14 bagus stress 修正 --{--
BOOL CStressLineSectionSettingDlg::SetAllInputData()
{
	STRESS_CONFIG	CheckStressConfig;
	memset(&CheckStressConfig, NULL, sizeof(CheckStressConfig));
	//この画面に関係ない部分があるので一回すべてをコピーする
	memcpy(&CheckStressConfig,&m_StressConfig,sizeof(CheckStressConfig));

	CString	strBuffer;

	DWORD	NumOfLine;

// 2009.09.17 bagus stress Grid修正 --{--
//	int	LineCols, LineRows;
// 2009.09.17 bagus stress Grid修正 --}--

	DWORD	NumOfPin;

	GV_ITEM Item;

	int LineFixRows = 1;
	int	row;

	long	PinX[STRESS_LINES_MAX * STRESS_PINS_MAX];
	long	PinY[STRESS_LINES_MAX * STRESS_PINS_MAX];
	long	tmpPinX, tmpPinY;

// 2009.09.17 bagus stress Grid修正 --{--
//	int	PinCols, PinRows;
// 2009.09.17 bagus stress Grid修正 --}--

	int	i ,j;

	int PinFixRows = 1;
	int	Rows;
	int	Pin;

// 2009.09.17 bagus stress Grid修正 --{--
	int	AllPinNum = 0;
// 2009.09.17 bagus stress Grid修正 --{--

// 2009.09.17 bagus stress Grid修正 --{--
//	int	LineValidCols, LineValidRows;
// 2009.09.17 bagus stress Grid修正 --}--

	int		FixLineValidRows = 1;
	long	lLineValid;

// 2009.09.18 bagus SE --{--
#if 0
	long	PinXS, PinXE, PinYS;

	long	LineInterval, PinInterval;
	double	PinOrgX, PinOrgY;
	double	PinMaxX, PinMaxY;
#endif
// 2009.09.18 bagus SE --}--


	///// Selected Sample /////
	CheckStressConfig.Size.dx = m_SampleInfo.Size.dx;
	CheckStressConfig.Size.dy = m_SampleInfo.Size.dy;
	CheckStressConfig.Origin.lX = m_SampleInfo.Origin.lX;
	CheckStressConfig.Origin.lY = m_SampleInfo.Origin.lY;

	// LineNum
	GetDlgItemText(IDC_NUMBER_OF_LINE, strBuffer);
	NumOfLine = strtol(strBuffer, NULL, 10);
	CheckStressConfig.dwLiftPinNumberOfLine = NumOfLine;

	// Line
// 2009.09.17 bagus stress Grid修正 --{--
//	LineCols = m_LineListGrid.GetColumnCount();
//	LineRows = m_LineListGrid.GetRowCount();
// 2009.09.17 bagus stress Grid修正 --}--

// 2009.09.17 bagus stress Grid修正 --{--
//	for (row=LineFixRows; row<LineRows; row++) {
	for (row=LineFixRows; row<NumOfLine+LineFixRows; row++) {
// 2009.09.17 bagus stress Grid修正 --}--
		Item.mask = GVIF_TEXT;
		Item.row = row;
		Item.col = 1;
		m_LineListGrid.GetItem(&Item);
		NumOfPin = strtol(Item.strText, NULL, 10);

// 2009.09.17 bagus stress Grid修正 --{--
		AllPinNum += NumOfPin;
// 2009.09.17 bagus stress Grid修正 --}--

		CheckStressConfig.Line[row - LineFixRows].dwLiftPinNumberOfLn = NumOfPin;
		CheckStressConfig.Line[row - LineFixRows].dwSectionNum = NumOfPin - 1;
	}

	// Pin
	memset(&PinX, 0, sizeof(PinX));
	memset(&PinY, 0, sizeof(PinY));

// 2009.09.17 bagus stress Grid修正 --{--
//	PinCols = m_PinListGrid.GetColumnCount();
//	PinRows = m_PinListGrid.GetRowCount();
// 2009.09.17 bagus stress Grid修正 --}--

	Pin = 0;
// 2009.09.17 bagus stress Grid修正 --{--
//	for(i=PinFixRows; i<PinRows; i++){
	for(i=PinFixRows; i<AllPinNum+PinFixRows; i++){
// 2009.09.17 bagus stress Grid修正 --}--
		Item.mask = GVIF_TEXT;
		Item.row = i;
		Item.col = 1;
		m_PinListGrid.GetItem(&Item);
		tmpPinX = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)

		Item.mask = GVIF_TEXT;
		Item.row = i;
		Item.col = 2;
		m_PinListGrid.GetItem(&Item);
		tmpPinY = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)

		PinX[i - PinFixRows] = tmpPinX;
		PinY[i - PinFixRows] = tmpPinY;
	}

	Pin = 0;
	for(i=0; i<CheckStressConfig.dwLiftPinNumberOfLine; i++){
		Rows = CheckStressConfig.Line[i].dwLiftPinNumberOfLn;
		for (j=0; j<Rows; j++) {
			CheckStressConfig.Line[i].PinPos[j].lX = PinX[Pin];
			CheckStressConfig.Line[i].PinPos[j].lY = PinY[Pin];

			Pin++;
		}
	}

	// Line Valid
// 2009.09.17 bagus stress Grid修正 --{--
//	LineValidCols = m_LineValidListGrid.GetColumnCount();
//	LineValidRows = m_LineValidListGrid.GetRowCount();
// 2009.09.17 bagus stress Grid修正 --}--

// 2009.09.17 bagus stress Grid修正 --{--
//	for (row=FixLineValidRows; row<LineValidRows; row++) {
	for (row=FixLineValidRows; row<CheckStressConfig.dwLiftPinNumberOfLine+FixLineValidRows; row++) {
// 2009.09.17 bagus stress Grid修正 --}--
		Item.mask = GVIF_TEXT;
		Item.row = row;
		Item.col = 1;
		//
		m_LineValidListGrid.GetItem(&Item);
#if 1
		//lLineValid = m_LineCheckBox.CheckBox[row-1]->GetCheck();
		lLineValid = ((CGridCellCheck *)m_LineValidListGrid.GetCell(row,1))->GetCheck();
#else
		lLineValid = strtol(Item.strText, NULL, 10);
#endif
		if (lLineValid <= 0) {
			CheckStressConfig.Line[row - FixLineValidRows].bValidLine = FALSE;
		} else {
			CheckStressConfig.Line[row - FixLineValidRows].bValidLine = TRUE;
		}
	}

	// Range Pin
	GetDlgItemText(IDC_STRESS_VALID_RANGE, strBuffer);
	CheckStressConfig.lStressValidRange = (long)(strtod(strBuffer.GetBuffer(0), NULL) * MICROMETRE);	// 単位変換(mm->um)

	// Range Edge
	GetDlgItemText(IDC_INVALID_RANGE_EDGE, strBuffer);
	CheckStressConfig.lInvalidRangeEdge = (long)(strtod(strBuffer.GetBuffer(0), NULL) * MICROMETRE);	// 単位変換(mm->um)

	// Offset X
	GetDlgItemText(IDC_THICK_MEASUREMENT_POINT_OFFSET_X, strBuffer);
	CheckStressConfig.MeasOffset.lX = (long)(strtod(strBuffer.GetBuffer(0), NULL) * MICROMETRE);	// 単位変換(mm->um)

// 2009.09.18 bagus SE --{--
#if 0
	// Section
	for(i=0; i<CheckStressConfig.dwLiftPinNumberOfLine; i++){
		Rows = CheckStressConfig.Line[i].dwSectionNum;
		for (j=0; j<Rows; j++) {
			PinYS = CheckStressConfig.Line[i].PinPos[j].lY;
			PinXS = CheckStressConfig.Line[i].PinPos[j].lX;
			PinXE = CheckStressConfig.Line[i].PinPos[j + 1].lX;

			CheckStressConfig.Line[i].SectPos[j].lScanStartPosX = PinXS + CheckStressConfig.lStressValidRange;
			CheckStressConfig.Line[i].SectPos[j].lScanEndPosX = PinXE - CheckStressConfig.lStressValidRange;
			CheckStressConfig.Line[i].SectPos[j].lScanPosY = PinYS;
		}
	}

	// Measurement
	if (CheckStressConfig.dwLiftPinNumberOfLine <= 1) {
		LineInterval = 0;
	} else {
		LineInterval = (long)((((CheckStressConfig.Size.dy - (CheckStressConfig.lInvalidRangeEdge * 2)) / (CheckStressConfig.dwLiftPinNumberOfLine - 1)) * 1000 + 500) / 1000);
	}

	PinInterval = (long)((((CheckStressConfig.Size.dx - (CheckStressConfig.lInvalidRangeEdge * 2)) / (PIN_INTERVAL - 1)) * 1000 + 500) / 1000);

	CheckPinMinPos(&PinOrgX, &PinOrgY);
	CheckPinMaxPos(&PinMaxX, &PinMaxY);
	if (PinOrgY == 0) {
		PinOrgY += CheckStressConfig.lInvalidRangeEdge;
		PinYS = PinOrgY;
	} else if (PinOrgY < 0) {
		PinOrgY += CheckStressConfig.lInvalidRangeEdge;
		PinYS = (long)((PinOrgY * 1000 - 500) / 1000);
	} else {
		PinOrgY -= CheckStressConfig.lInvalidRangeEdge;
		PinYS = (long)((PinOrgY * 1000 + 500) / 1000);
	}

	for(i=0; i<CheckStressConfig.dwLiftPinNumberOfLine; i++){
		if (PinOrgX == 0) {
			PinXS = PinOrgX;
		} else if (PinOrgX < 0) {
			PinXS = (long)((PinOrgX * 1000 - 500) / 1000);
		} else {
			PinXS = (long)((PinOrgX * 1000 + 500) / 1000);
		}
		for (j=0; j<PIN_INTERVAL; j++) {
			if (j== PIN_INTERVAL-1) {
				CheckStressConfig.Line[i].MeasPos[j].lX = PinMaxX;
				CheckStressConfig.Line[i].MeasPos[j].lY = PinYS;
			} else {
				CheckStressConfig.Line[i].MeasPos[j].lX = PinXS;
				CheckStressConfig.Line[i].MeasPos[j].lY = PinYS;
			}

			PinXS += PinInterval + CheckStressConfig.MeasOffset.lX;
		}

		PinYS += LineInterval;
	}
//#else
	// Section
	for(i=0; i<CheckStressConfig.dwLiftPinNumberOfLine; i++){
		Rows = CheckStressConfig.Line[i].dwSectionNum;
		for (j=0; j<Rows; j++) {
			Item.mask = GVIF_TEXT;
			Item.row = i;
			Item.col = 1;
			m_StressMeasurementLineSectionListGrid.GetItem(&Item);
			PinXS = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)

			Item.mask = GVIF_TEXT;
			Item.row = i;
			Item.col = 2;
			m_StressMeasurementLineSectionListGrid.GetItem(&Item);
			PinXE = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)

			Item.mask = GVIF_TEXT;
			Item.row = i;
			Item.col = 3;
			m_StressMeasurementLineSectionListGrid.GetItem(&Item);
			PinYS = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)

			CheckStressConfig.Line[i].SectPos[j].lScanStartPosX = PinXS;
			CheckStressConfig.Line[i].SectPos[j].lScanEndPosX = PinXE;
			CheckStressConfig.Line[i].SectPos[j].lScanPosY = PinYS;
		}
	}

	for(i=0; i<CheckStressConfig.dwLiftPinNumberOfLine; i++){
		for (j=0; j<PIN_INTERVAL; j++) {
			Item.mask = GVIF_TEXT;
			Item.row = i;
			Item.col = 1;
			m_NecessaryThicknessMeasurementListGrid.GetItem(&Item);
			PinXS = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)

			Item.mask = GVIF_TEXT;
			Item.row = i;
			Item.col = 2;
			m_NecessaryThicknessMeasurementListGrid.GetItem(&Item);
			PinYS = strtod(Item.strText, NULL) * MICROMETRE;		// 単位変換(um->mm)

			CheckStressConfig.Line[i].MeasPos[j].lX = PinXS;
			CheckStressConfig.Line[i].MeasPos[j].lY = PinYS;
		}
	}
#else
	// SET LINE/SECTION を押した時点で決まるので、編集チェックはスルー
	for(i=0; i<STRESS_LINES_MAX; i++){
		for (j=0; j<STRESS_SECTIONS_MAX; j++) {
			CheckStressConfig.Line[i].SectPos[j].lScanStartPosX = m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
			CheckStressConfig.Line[i].SectPos[j].lScanEndPosX = m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
			CheckStressConfig.Line[i].SectPos[j].lScanPosY = m_StressConfig.Line[i].SectPos[j].lScanPosY;
		}
	}

	for(i=0; i<STRESS_LINES_MAX; i++){
		for (j=0; j<STRESS_SECTIONS_MAX; j++) {
			CheckStressConfig.Line[i].MeasPos[j].lX = m_StressConfig.Line[i].MeasPos[j].lX;
			CheckStressConfig.Line[i].MeasPos[j].lY = m_StressConfig.Line[i].MeasPos[j].lY;
		}
	}
#endif
// 2009.09.18 bagus SE --}--

	return memcmp(&CheckStressConfig, &m_StressConfig, sizeof(m_StressConfig)) == 0;

}
// 2009.09.14 bagus stress 修正 --}--

// 2009.09.15 bagus stress 修正 --{--
BOOL CStressLineSectionSettingDlg::CheckInputValueLong(CString strVal, long *lValue)
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

//	if ((pEnd != NULL) && (&pChk[strlen(pChk)-1] != pEnd)) {
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

BOOL CStressLineSectionSettingDlg::CheckInputValueDouble(CString strVal, double *dValue)
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

//	if ((pEnd != NULL) && (&pChk[strlen(pChk)-1] != pEnd)) {
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
// 2009.09.15 bagus stress 修正 --}--
