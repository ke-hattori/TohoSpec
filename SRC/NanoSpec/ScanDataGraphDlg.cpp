// ScanDataGraphDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "ScanDataGraphDlg.h"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScanDataGraphDlg ダイアログ


CScanDataGraphDlg::CScanDataGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanDataGraphDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanDataGraphDlg)
	m_pScanDataGraph = new CNanoMeasureGraph();
	//}}AFX_DATA_INIT

// 2010.01.08 bagus Gantry --{--
	m_nSysVScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
// 2010.01.08 bagus Gantry --}--
}

// =========================================================================
//
CScanDataGraphDlg::~CScanDataGraphDlg()
{
	if ( m_pScanDataGraph ) {
		delete m_pScanDataGraph;
		m_pScanDataGraph = NULL;
	}
}

void CScanDataGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanDataGraphDlg)
	DDX_Control(pDX, IDC_SCAN_DATA_GRAPH, *m_pScanDataGraph);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SCAN_DATA_GRID, m_ScanDataGrid);
}


BEGIN_MESSAGE_MAP(CScanDataGraphDlg, CDialog)
	//{{AFX_MSG_MAP(CScanDataGraphDlg)
	ON_BN_CLICKED(IDC_GRAPH_AUTO_SCALE, OnGraphAutoScale)
// 2010.01.29 bagus Gantry --{--
	ON_WM_KILLFOCUS()
// 2010.01.29 bagus Gantry --}--
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanDataGraphDlg メッセージ ハンドラ
// =========================================================================
//
BOOL CScanDataGraphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_srConfig, CONFIG_FILE_SR_CONFIG);

	///// Option Auto Focus /////
	//CheckDlgButton(IDC_GRAPH_AUTO_SCALE, 1);
	CheckDlgButton(IDC_GRAPH_AUTO_SCALE, 0);

	///// Grid Control Initialize /////
//	ScanDataGrid_Init();
//	SetData();

	///// Draw Graph Frame /////
// 2010.01.21 bagus Gantry --{--
//	ScanDataGraph_Init();
//	ScanDataGrid_Init();
	InitDialogData();
// 2010.01.21 bagus Gantry --}--

	///// Do Scan /////
// 2010.01.21 bagus Gantry --{--
//	ScanData();
// 2010.01.21 bagus Gantry --}--

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CScanDataGraphDlg::ScanDataGraph_Init()
{
	CNanoMeasureGraph::GraphConfig tGraphConfig;
	CNanoMeasureGraph::LineConfig tLineConfig;

	CString l_strBuffer;

	memset(&tGraphConfig,NULL,sizeof(tGraphConfig));
	tGraphConfig.UseLineSelect = TRUE;
//	LoadStringML(IDS_CCD_ARRAY_SPECTRUM_GRAPH, l_strBuffer, "CCD ARRAY SPECTRUM GRAPH");
	l_strBuffer = "SPECTRUM GRAPH";
	strcpy(tGraphConfig.UpperTitle, l_strBuffer);
	strcpy(tGraphConfig.LowerTitle,"");
	tGraphConfig.CanvasMargin = 7;	//グラフ描画余白

///// 波長範囲の設定 /////
//	LoadStringML(IDS_LIGHT_INTENSITY_TITLE, l_strBuffer, "Light Intensity");
// 2010.01.29 bagus Gantry 修正 --{--
//	l_strBuffer = "Transmittance Light Intensity";
	l_strBuffer = "Light Intensity";
// 2010.01.29 bagus Gantry 修正 --}--
	strcpy(tGraphConfig.Y_AxisTitle, l_strBuffer);
//	LoadStringML(IDS_ARRAY_PIXEL, l_strBuffer, "Array Pixel [dot]");
	l_strBuffer = "Wavelength [nm]";
	strcpy(tGraphConfig.X_AxisTitle, l_strBuffer);
	tGraphConfig.X_Max = m_iEndWaveLength;
	tGraphConfig.X_Origin = m_iStartWaveLength;
	tGraphConfig.X_Step = 50;

	if (IsDlgButtonChecked(IDC_GRAPH_AUTO_SCALE)==TRUE){
		double dLength;
		CString strBuffer;
		int iIntensityMax = 0;
		int iTemp_Step;
		for(int iPixel = m_iStartWaveLength; iPixel <= m_iEndWaveLength; iPixel++){
			dLength = sin(3.14*iPixel / 180) *10000+30000;
			strBuffer.Format("%lf", dLength);
			if (iIntensityMax < (double)atof(strBuffer))
				iIntensityMax = (double)atof(strBuffer);
		}
		if(iIntensityMax < 1000)
			iIntensityMax = 1000;

		tGraphConfig.Y_Max= iIntensityMax;
		iTemp_Step = (int)(iIntensityMax / 1000);
		iTemp_Step = (int)(iTemp_Step * 1000 / 10);
		tGraphConfig.Y_Step = iTemp_Step;
	}
	else{
		tGraphConfig.Y_Max= 65535;
		tGraphConfig.Y_Step = 5000;
	}
	tGraphConfig.Y_Origin = 0;

	tGraphConfig.PointNum = (tGraphConfig.X_Max)-(tGraphConfig.X_Origin)+1;//200;//NANO_MEASURE_MAX_POINT_DATA;
	tGraphConfig.CanvasBackColor = WHITE_COLOR;
	tGraphConfig.GraphBackColor = WHITE_COLOR;//RGB(0x00,0xAA,0xAA);
	tGraphConfig.GraphBorderColor = RGB(0x00,0x00,0x00);
	tGraphConfig.GraphStepLineColor = RGB(0xAA,0xAA,0xAA);
	tGraphConfig.TitleTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.UnitTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.SelectingCalibrationColor= RGB(0xFF, 0x00, 0x00);
	tGraphConfig.SelectedCalibrationColor = RGB(0x00, 0xFF, 0x00);
	//2010.02.01 bagus 修正 --{--
	//tGraphConfig.UseCalibration = TRUE;
	//tGraphConfig.CalibrationLineNum = 7;
	tGraphConfig.UseCalibration = FALSE;
	tGraphConfig.CalibrationLineNum = 0;
	//2010.02.01 bagus 修正 --}--
	m_pScanDataGraph->SetGraphConfig(tGraphConfig);


	//GetData and SetData
#if 0 // 2014.04.04 bagus wavelength step modified -->
//	int Rows=(m_iEndWaveLength)-(m_iStartWaveLength)+2;
#else
	int Rows;

	switch(m_srConfig.nWavelengthStep){
	case SR_WAVELENGTH_STEP_05_NM:
		Rows = (m_iEndWaveLength - m_iStartWaveLength + 1) * 2 + 1;
		break;
	case SR_WAVELENGTH_STEP_1_NM:
	default:
		Rows = m_iEndWaveLength - m_iStartWaveLength + 2;
		break;
	}
#endif // 2014.04.04 bagus wavelength step modified <--
//	if(Rows<=DEF_LITENSITY_GRID_ROWLINE_COUNT) Rows=DEF_LITENSITY_GRID_ROWLINE_COUNT;
	TRY{
		m_ScanDataGrid.SetRowCount(Rows);
		//m_ScanDataGrid.SetColumnCount(Cols);
		//m_ScanDataGrid.SetFixedRowCount(FixRows);
		//m_ScanDataGrid.SetFixedColumnCount(FixCols);
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

		if(i<=0){
			tLineConfig.UseFlag = TRUE;
			tLineConfig.Visible = TRUE;
		}
		else{
			tLineConfig.UseFlag = FALSE;
			tLineConfig.Visible = FALSE;
		}

		tLineConfig.DataIndex = i;
		if (i==0) {
			tLineConfig.Color = BLUE_COLOR;
			// =PS_SOLID:実線
			// PS_DASH:破線
			// PS_DOT:点線
			// PS_DASHDOT:１点鎖線
			// PS_DASHDOTDOT:２点鎖線
			tLineConfig.PenStyle = PS_SOLID;
		}
		else {
			tLineConfig.PenStyle = PS_SOLID;
			wsprintf(tLineConfig.LineName,"null%02d",i);
		}
		m_pScanDataGraph->SetLineConfig(i,tLineConfig);
		m_pScanDataGraph->ClearGraphData();
	}
}
void CScanDataGraphDlg::ScanDataGrid_Init()
{
	int Cols = 1 + 1;
	int Rows = m_iEndWaveLength - m_iStartWaveLength +2;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;
	int Index;
	int ItemWidth = 106;//68;//136

	LPCTSTR pszGraphSetting_Item[] = {
		"Wave\r\n[nm]",
		"Scan\r\nData"
	};

	///// Grid Initialize /////
	m_ScanDataGrid.SetEditable(FALSE);
	m_ScanDataGrid.EnableDragAndDrop(TRUE);
	m_ScanDataGrid.SetTextBkColor(WHITE_COLOR);
	m_ScanDataGrid.SetRowResize(0);

	m_ScanDataGrid.EnableSelection(TRUE);
	m_ScanDataGrid.SetTrackFocusCell(FALSE);
	m_ScanDataGrid.SetFrameFocusCell(FALSE);
	m_ScanDataGrid.SetFixedColumnSelection(TRUE);
	m_ScanDataGrid.SetFixedRowSelection(TRUE);
	m_ScanDataGrid.SetListMode(TRUE);
	TRY{
		m_ScanDataGrid.SetRowCount(Rows);
		m_ScanDataGrid.SetColumnCount(Cols);
		m_ScanDataGrid.SetFixedRowCount(FixRows);
		m_ScanDataGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2010.01.08 bagus Gantry --{--
	CWnd* GridBox = GetDlgItem(IDC_SCAN_DATA_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

//	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nCellsWidth;
	if (Rows > 30) {
		nCellsWidth = GridRect.Width();
	} else {
		nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	}
//	int	nColWidth = nCellsWidth / Cols;
//	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);
	ItemWidth = nCellsWidth - 70;
// 2010.01.08 bagus Gantry --}--

	// Row 0 Height
	m_ScanDataGrid.SetRowHeight(0, 36);//Header Height

	// Row 1 - RowCount Height
	for(row = 1; row < m_ScanDataGrid.GetRowCount(); row++)
		m_ScanDataGrid.SetRowHeight(row, 18);

	// Col 0 Width
	m_ScanDataGrid.SetColumnWidth(0,70);//(0, 40);	// Wavelength Width

	// Col 1 - ColumnCount Width
	for (col = 1; col < m_ScanDataGrid.GetColumnCount(); col++)
		m_ScanDataGrid.SetColumnWidth(col, ItemWidth);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);//ダイアログのフォント取得
	//m_Font.GetLogFont(&LogFont);//ビューのフォント取得

	for ( row = 0; row < m_ScanDataGrid.GetRowCount(); row++ )
		for ( col = 0; col < m_ScanDataGrid.GetColumnCount(); col++ )
			m_ScanDataGrid.SetItemFont(row, col, &LogFont);

	///// Title Row /////
	//// Current Graph Data ////
	dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	Index = 0;
	row = 1;
	//for ( Index = 0; Index <= Rows ; Index++ ) {
	for ( Index = 0; Index < m_ScanDataGrid.GetRowCount() ; Index++ ) {
		///// Wavelength /////
		if (Index != 0)
			m_ScanDataGrid.SetItemTextFmt(Index, 0, "%d", Index + m_iStartWaveLength - 1);

		m_ScanDataGrid.SetItemFormat(Index, 0, dwTextStyle);
	}

	//// Title Column /////
	//dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	dwTextStyle = DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS;
	Index = 0;
	for ( col = 0; col < m_ScanDataGrid.GetColumnCount(); col++ ) {
		m_ScanDataGrid.SetItemText(0, col, pszGraphSetting_Item[Index]);
		m_ScanDataGrid.SetItemFormat(0, col, dwTextStyle);
		Index++;
	}
}

BOOL CScanDataGraphDlg::ScanData()
{
	CNanoSpecDoc* l_pDoc = (CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument();
	//2010.01.13 bagus GTR --{--
	//MEAS_SrHead_CcdScan((double)m_nIntegrationTime);
	MEAS_SrHead_CcdScan(m_dIntegrationTime);
	//2010.01.13 bagus GTR --}--

	AddLightIntensityListAndGraph();

	return TRUE;
}
void CScanDataGraphDlg::AddLightIntensityListAndGraph()
{
#if 0 // 2014.04.04 bagus wavelength step modified -->
//	double dLength;
#endif // 2014.04.04 bagus wavelength step modified <--
	CString strBuffer;
	DWORD dwTextStyle;

	//// Graph Data All Clear ////
	m_pScanDataGraph->ClearGraphData();

#if 0 // 2014.04.04 bagus wavelength step modified -->
// 	//// Gen CCD Data and Set CCD Data ////
// 	for(int iWave = m_iStartWaveLength; iWave <= m_iEndWaveLength; iWave++){
// 		//LightIntensityListGrid
// 		dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
//
// 		///// Set Measured CCD Data Value at Pixel /////
// 		dLength = MEAS_GetScan1stReferenceData(iWave);
// 		strBuffer.Format("%.2lf", dLength);
// 		m_ScanDataGrid.SetItemText(iWave - m_iStartWaveLength + 1, 1, strBuffer);
// 		m_ScanDataGrid.SetItemFormat(iWave - m_iStartWaveLength + 1, 1, dwTextStyle);
// 		m_pScanDataGraph->AddLineData(0, dLength);
//
// 	}
#else
	//// Gen CCD Data and Set CCD Data ////
	double dWavelengthStep;
	double dStartWave;
	double dEndWave;
	double dTargetWave;
	double dData;
	int iIndex;

	switch(m_srConfig.nWavelengthStep){
	case SR_WAVELENGTH_STEP_05_NM:
		dWavelengthStep = 0.5;
		break;
	case SR_WAVELENGTH_STEP_1_NM:
	default:
		dWavelengthStep = 1.0;
		break;
	}

	dStartWave = (double)m_iStartWaveLength;
	dEndWave = (double)m_iEndWaveLength;
	dTargetWave = dStartWave;
	while(dTargetWave <= dEndWave)
	{
		//LightIntensityListGrid
		dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

		///// Set Measured CCD Data Value at Pixel /////
		dData = MEAS_GetScan1stReferenceData(dTargetWave);
		strBuffer.Format("%.2lf", dData);

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dTargetWave - dStartWave) * 2.0);
		}
		else{
			iIndex = (int)((dTargetWave - dStartWave));
		}

		m_ScanDataGrid.SetItemText(iIndex + 1, 1, strBuffer);
		m_ScanDataGrid.SetItemFormat(iIndex + 1, 1, dwTextStyle);
		m_pScanDataGraph->AddLineData(0, dData);

		dTargetWave += dWavelengthStep;
	}
#endif // 2014.04.04 bagus wavelength step modified <--

	m_ScanDataGrid.Invalidate();
	m_pScanDataGraph->Invalidate();

}
void CScanDataGraphDlg::OnGraphAutoScale()
{
	BOOL bAutoScale = m_pScanDataGraph->GetAutoScaleMode();
	m_pScanDataGraph->SetAutoScaleMode(!bAutoScale);
	m_pScanDataGraph->Invalidate();
}

// 2010.01.21 bagus Gantry --{--
void CScanDataGraphDlg::InitDialogData()
{
	ScanDataGraph_Init();
	ScanDataGrid_Init();
}
// 2010.01.21 bagus Gantry --}--

// 2010.01.29 bagus Gantry --{--
void CScanDataGraphDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);

}
// 2010.01.29 bagus Gantry --}--

