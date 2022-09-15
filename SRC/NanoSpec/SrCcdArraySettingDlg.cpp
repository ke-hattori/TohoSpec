// SrCcdArraySettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "CcdCalibrationDlg.h"
#include "SrCcdArraySettingDlg.h"
#include "..\\..\\INC\\NSStage.hxx"
//Saiki 20090526 Add ----->
#include "resource.h"
//Saiki 20090526 Add <-----
// 2013.11.07 Bagus Add (TohoSpec対応) -->
#include "ConfirmDlg.h"
// 2013.11.07 Bagus Add (TohoSpec対応) <--
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEF_LITENSITY_GRID_ROWLINE_COUNT	(31+1)	//IntensityGridにちょうどスクロールバーが表示される行数

// #########################################################################
// CSrCcdArraySettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSrCcdArraySettingDlg::CSrCcdArraySettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrCcdArraySettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrCcdArraySettingDlg)
	m_CCDCalibIntegrationTime = 0.3;
	//}}AFX_DATA_INIT

	m_strLens = _T("");
// 2009.09.30 K.Matsuo スタックオーバーフロー対策 -->
	m_pCCDCalibGraph = new CNanoMeasureGraph();
// 2009.09.30 K.Matsuo スタックオーバーフロー対策 <--
//	FontCreation();
}

// =========================================================================
//
CSrCcdArraySettingDlg::~CSrCcdArraySettingDlg()
{
//	m_Font.DeleteObject();
// 2009.09.30 K.Matsuo スタックオーバーフロー対策 -->
	if ( m_pCCDCalibGraph ) {
		delete m_pCCDCalibGraph;
		m_pCCDCalibGraph = NULL;
	}
// 2009.09.30 K.Matsuo スタックオーバーフロー対策 <--
}

// =========================================================================
//
void CSrCcdArraySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrCcdArraySettingDlg)
	DDX_Control(pDX, IDC_CCD_CALIB_GRAPH, *m_pCCDCalibGraph);
	DDX_Text(pDX, IDC_CCD_CALIB_INTEGRATION_TIME, m_CCDCalibIntegrationTime);
	DDV_MinMaxDouble(pDX, m_CCDCalibIntegrationTime, 0., 30.);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CCD_INTENSITY_LIST_GRID, m_CCDIntensityListGrid);
	DDX_Control(pDX, IDC_CCD_CALIB_LIST_GRID, m_CCDCalibListGrid);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_SCAN_BUTTON, m_ScanButton);
	DDX_Control(pDX, IDC_CALIB_CLEAR_BUTTON, m_ClearCalibrationButton);
	DDX_CBStringExact(pDX, IDC_CCD_CALIB_LENS, m_strLens);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrCcdArraySettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrCcdArraySettingDlg)
	ON_BN_CLICKED(IDC_MANUAL_SCAN_BUTTON, OnScanButton)
	ON_BN_CLICKED(IDC_CALIB_CLEAR_BUTTON, OnCalibClearButton)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NANO_MEASURE_GRAPH_CALIBRATION_SET,CalibrationSet)
	ON_MESSAGE(WM_NANO_MEASURE_GRAPH_CALIBRATION_COMP,CalibrationComplete)

END_MESSAGE_MAP()

// =========================================================================
// CSrCcdArraySettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSrCcdArraySettingDlg::OnInitDialog()
{
	//未対応機能HIDE
	GetDlgItem(IDC_CHECK_AUTO_SCALE)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHECK_CONTINOUS_SCAN)->ShowWindow(SW_HIDE);

	char szBuff[1];
	sprintf(szBuff, "%d", 1);
	SetDlgItemText(IDC_CCD_CALIB_SCAN_TIMES, szBuff);

	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	ConfigFile_GetNanoSpecIni(m_SrCcdArray, CONFIG_FILE_SR_CCD_ARRAY);

	CDialog::OnInitDialog();

	///// Option Auto Focus /////
	//CheckDlgButton(IDC_CHECK_AUTO_SCALE, 1);
	CheckDlgButton(IDC_CHECK_AUTO_SCALE, 0);

	///// Option Continious Scan /////
	CheckDlgButton(IDC_CHECK_CONTINOUS_SCAN, 0);

	//SRヘッドの種類を選択する
	m_iStartWaveLength = 1;
	if (m_SrConfig.nHeadType <=3)
		m_iEndWaveLength = 512;		//標準ヘッド、ＵＶヘッド、厚膜ヘッド
	else
		m_iEndWaveLength = 1024;	//厚膜ヘッド、ＳＳヘッド

	InitCombo_Lens();


	///// Grid Control Initialize /////
	LightIntensityListGrid_Init();
	CalibPixelListGrid_Init();
	SetData();

	///// Draw Graph Frame /////
	DrawLightIntensityGraph_Init();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Manual Scan Button /////
	m_ScanButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ScanButton.DrawFlatFocus(TRUE);

	///// Calibration Button /////
	m_ClearCalibrationButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ClearCalibrationButton.DrawFlatFocus(TRUE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSrCcdArraySettingDlg::DrawLightIntensityGraph_Init()
{
	CNanoMeasureGraph::GraphConfig tGraphConfig;
	CNanoMeasureGraph::LineConfig tLineConfig;

	// Kojika 20090529 Add
	CString l_strBuffer;
	// Kojika 20090529 Add End

	memset(&tGraphConfig,NULL,sizeof(tGraphConfig));
	tGraphConfig.UseLineSelect = TRUE;
	// Kojika 20090529 Change
	//strcpy(tGraphConfig.UpperTitle,"CCD ARRAY SPECTRUM GRAPH");
	LoadStringML(IDS_CCD_ARRAY_SPECTRUM_GRAPH, l_strBuffer, "CCD ARRAY SPECTRUM GRAPH");
	strcpy(tGraphConfig.UpperTitle, l_strBuffer);
	// Kojika 20090529 Change End
	strcpy(tGraphConfig.LowerTitle,"");
	tGraphConfig.CanvasMargin = 7;	//グラフ描画余白

///// 波長範囲の設定 /////
	// Kojika 20090529 Change
	//strcpy(tGraphConfig.Y_AxisTitle,"Light Intensity");
	LoadStringML(IDS_LIGHT_INTENSITY_TITLE, l_strBuffer, "Light Intensity");
	strcpy(tGraphConfig.Y_AxisTitle, l_strBuffer);
	//strcpy(tGraphConfig.X_AxisTitle,"Array Pixel [dot]");
	LoadStringML(IDS_ARRAY_PIXEL, l_strBuffer, "Array Pixel [dot]");
	strcpy(tGraphConfig.X_AxisTitle, l_strBuffer);
	// Kojika 20090529 Change End
	tGraphConfig.X_Max = m_iEndWaveLength;
	tGraphConfig.X_Origin = m_iStartWaveLength;
	tGraphConfig.X_Step = 50;

	if (IsDlgButtonChecked(IDC_CHECK_AUTO_SCALE)==TRUE){
		double dLength;
		CString strBuffer;
		int iIntensityMax = 0;
		int iTemp_Step;
		// Gen CCD Data and Set CCD Data ////
		for(int iPixel = m_iStartWaveLength; iPixel <= m_iEndWaveLength; iPixel++){
			//dLength = MEAS_GetCcdData(1, iPixel);
			//dLength = MEAS_SrHead_GetCcdData(iPixel);
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
	tGraphConfig.UseCalibration = TRUE;
	tGraphConfig.CalibrationLineNum = 7;
	m_pCCDCalibGraph->SetGraphConfig(tGraphConfig);


	//GetData and SetData
	int Rows=(m_iEndWaveLength)-(m_iStartWaveLength)+2;
	if(Rows<=DEF_LITENSITY_GRID_ROWLINE_COUNT) Rows=DEF_LITENSITY_GRID_ROWLINE_COUNT;
	TRY{
		m_CCDIntensityListGrid.SetRowCount(Rows);
		//m_CCDIntensityListGrid.SetColumnCount(Cols);
		//m_CCDIntensityListGrid.SetFixedRowCount(FixRows);
		//m_CCDIntensityListGrid.SetFixedColumnCount(FixCols);
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
		m_pCCDCalibGraph->SetLineConfig(i,tLineConfig);
		m_pCCDCalibGraph->ClearGraphData();
	}
}

// =========================================================================
//
void CSrCcdArraySettingDlg::AddLightIntensityListAndGraph(int iPointIndex)
{
	double dLength;
	CString strBuffer;
	DWORD dwTextStyle;

	//// Graph Data All Clear ////
	m_pCCDCalibGraph->ClearGraphData();

	//// Gen CCD Data and Set CCD Data ////
	for(int iPixel = m_iStartWaveLength; iPixel <= m_iEndWaveLength; iPixel++){
		//LightIntensityListGrid
		dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

		///// Set Measured CCD Data Value at Pixel /////
		dLength = MEAS_SrHead_GetCcdData(iPixel);
		strBuffer.Format("%.0lf", dLength);
		m_CCDIntensityListGrid.SetItemText(iPixel, 1, strBuffer);
		m_CCDIntensityListGrid.SetItemFormat(iPixel, 1, dwTextStyle);
		m_pCCDCalibGraph->AddLineData(0, (double)atof(strBuffer));

	}
	m_CCDIntensityListGrid.Invalidate();
	m_pCCDCalibGraph->Invalidate();
}

// =========================================================================
//光強度表示グリッド
void CSrCcdArraySettingDlg::LightIntensityListGrid_Init()
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
		"Pixel [dot]",
		"Intensity Data"
	};

	///// Grid Initialize /////
	m_CCDIntensityListGrid.SetEditable(FALSE);
	m_CCDIntensityListGrid.EnableDragAndDrop(TRUE);
	m_CCDIntensityListGrid.SetTextBkColor(WHITE_COLOR);
	m_CCDIntensityListGrid.SetRowResize(0);

	m_CCDIntensityListGrid.EnableSelection(TRUE);
	m_CCDIntensityListGrid.SetTrackFocusCell(FALSE);
	m_CCDIntensityListGrid.SetFrameFocusCell(FALSE);
	m_CCDIntensityListGrid.SetFixedColumnSelection(TRUE);
	m_CCDIntensityListGrid.SetFixedRowSelection(TRUE);
	m_CCDIntensityListGrid.SetListMode(TRUE);
	TRY{
		m_CCDIntensityListGrid.SetRowCount(Rows);
		m_CCDIntensityListGrid.SetColumnCount(Cols);
		m_CCDIntensityListGrid.SetFixedRowCount(FixRows);
		m_CCDIntensityListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH


	// Row 0 Height
	m_CCDIntensityListGrid.SetRowHeight(0, 36);//Header Height

	// Row 1 - RowCount Height
	for(row = 1; row < m_CCDIntensityListGrid.GetRowCount(); row++)
		m_CCDIntensityListGrid.SetRowHeight(row, 18);

	// Col 0 Width
	m_CCDIntensityListGrid.SetColumnWidth(0,70);//(0, 40);	// Wavelength Width

	// Col 1 - ColumnCount Width
	for (col = 1; col < m_CCDIntensityListGrid.GetColumnCount(); col++)
		m_CCDIntensityListGrid.SetColumnWidth(col, ItemWidth);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);//ダイアログのフォント取得
	//m_Font.GetLogFont(&LogFont);//ビューのフォント取得

	for ( row = 0; row < m_CCDIntensityListGrid.GetRowCount(); row++ )
		for ( col = 0; col < m_CCDIntensityListGrid.GetColumnCount(); col++ )
			m_CCDIntensityListGrid.SetItemFont(row, col, &LogFont);

	///// Title Row /////
	//// Current Graph Data ////
	dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	Index = 0;
	row = 1;
	//for ( Index = 0; Index <= Rows ; Index++ ) {
	for ( Index = 0; Index < m_CCDIntensityListGrid.GetRowCount() ; Index++ ) {
		///// Wavelength /////
		if (Index != 0)
			m_CCDIntensityListGrid.SetItemTextFmt(Index, 0, "%d", Index);

		m_CCDIntensityListGrid.SetItemFormat(Index, 0, dwTextStyle);
	}

	//// Title Column /////
	//dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	dwTextStyle = DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS;
	Index = 0;
	for ( col = 0; col < m_CCDIntensityListGrid.GetColumnCount(); col++ ) {
		m_CCDIntensityListGrid.SetItemText(0, col, pszGraphSetting_Item[Index]);
		m_CCDIntensityListGrid.SetItemFormat(0, col, dwTextStyle);
		Index++;
	}
}

// =========================================================================
//
void CSrCcdArraySettingDlg::SetData()
{
	CString strBuffer;

	for ( int iIndex = 0; iIndex < SR_CCD_ARRAY_ITEM_MAX; iIndex++ ) {
		if(m_SrCcdArray[iIndex].dData <= 0 || m_SrCcdArray[iIndex].dData > m_iEndWaveLength ){
			strBuffer.Format("%s", "");
		}
		else{
			strBuffer.Format("%.10lf", m_SrCcdArray[iIndex].dData);
		}
		m_CCDCalibListGrid.SetItemText(iIndex+1, 2, strBuffer);
	}
	m_CCDCalibListGrid.Invalidate();
}

// =========================================================================
//

LRESULT CSrCcdArraySettingDlg::SetMeasurementData(WPARAM wparam, LPARAM lparam)
{
	CString strBuffer;
	DWORD dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

	int iPoint = wparam;	// 測定ポイントごとの回数

	for(int iPixel = 0; iPixel < m_iEndWaveLength; iPixel++){
		//CCD Intensity Data
//		strBuffer.Format("%lf", m_pCCDDataBase->GetSharedMemoryPtr()->ccdData[0][iPixel]);
		strBuffer.Format("%lf", (double)MEAS_SrHead_GetCcdData(iPixel));
		m_CCDIntensityListGrid.SetItemText(1, iPixel+1, strBuffer);
		m_CCDIntensityListGrid.SetItemFormat(1, iPixel+1, dwTextStyle);
		m_CCDIntensityListGrid.Invalidate();
	}

	//LightIntensityListGrid and Graph
	AddLightIntensityListAndGraph(0);

	return 0L;
}

// =========================================================================
//CCDスキャン
//
void CSrCcdArraySettingDlg::OnScanButton()
{
	CString strBuffer;
	int iScanTimes;
	int iScanCount;

	UpdateData(TRUE);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CComboBox* pCombo;
	pCombo = (CComboBox *)GetDlgItem(IDC_CCD_CALIB_LENS);
	int iLens = pCombo->GetCurSel();

// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
	SYSTEM_CONFIG l_SystemConfig;

	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
//	if(g_lModelType == MODEL_T3100){
	if(l_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		CConfirmDlg dlg;
		CString l_strCaption;
		CString l_strMessageText;
		CString l_strFormat;
		SR_TURRET l_srTurret[SR_LENS_MAX];
		SR_FILTER l_srFilter[SR_FILTER_MAX];

		LoadStringML(IDS_SET_LENS_CAPTION, l_strCaption, "Set Lens and filter");
		LoadStringML(IDS_SET_LENS_MESSAGE, l_strFormat, "Please set lens and filter.\nLens:%s\nFilter:%s\nReference:%s");

		ConfigFile_GetNanoSpecIni(l_srTurret, CONFIG_FILE_SR_TURRET);
		ConfigFile_GetNanoSpecIni(l_srFilter, CONFIG_FILE_SR_FILTER);
		LPCTSTR pszFilterName[LANGUAGE_MAX][OPT_FILTER_MAX] = {
			{OPTICAL_FILTER_TYPE_ITEM_ENU[0], OPTICAL_FILTER_TYPE_ITEM_ENU[1], OPTICAL_FILTER_TYPE_ITEM_ENU[2], l_srFilter[0].szName, l_srFilter[1].szName, l_srFilter[2].szName},	// Enu
			{OPTICAL_FILTER_TYPE_ITEM_JPN[0], OPTICAL_FILTER_TYPE_ITEM_JPN[1], OPTICAL_FILTER_TYPE_ITEM_JPN[2], l_srFilter[0].szName, l_srFilter[1].szName, l_srFilter[2].szName},	// Jpn
		};

		l_strMessageText.Format(l_strFormat, l_srTurret[iLens].szName, pszFilterName[l_SystemConfig.nLanguage][OPT_FILTER_OPEN],"-");

		dlg.SetButtonStyle(MB_OK);
		dlg.SetMessageText(l_strMessageText, l_strCaption, MB_ICONINFORMATION);
		dlg.DoModal();
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	//繰返し測定判定（繰返し：チェック時）
	m_bScan_StopFlg =0;

	//繰返し測定判定（繰返し：チェック時）
	GetDlgItemText(IDC_CCD_CALIB_SCAN_TIMES, strBuffer);
	iScanTimes = atof((PCSTR) strBuffer);
	iScanCount =0;
	do
	{
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			CNanoSpecDoc* l_pDoc = (CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument();
			l_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
			StageSelectLensNotify(iLens);
			l_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);
			MEAS_SrHead_ChangeCcdShutter(OPT_FILTER_OPEN);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		MEAS_SrHead_CcdScan((double)m_CCDCalibIntegrationTime);

		///// Draw Graph Frame /////
		DrawLightIntensityGraph_Init();

		AddLightIntensityListAndGraph(0);
		if (IsDlgButtonChecked(IDC_CHECK_CONTINOUS_SCAN)==TRUE)
			Sleep(100);

		iScanCount ++ ;
	}while ((IsDlgButtonChecked(IDC_CHECK_CONTINOUS_SCAN)==TRUE) || (iScanCount < iScanTimes) || (m_bScan_StopFlg ==1));
}

// =========================================================================
//レンズ コンボイニシャル処理
//
void CSrCcdArraySettingDlg::InitCombo_Lens()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CCD_CALIB_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
		if ( m_SrTurret[i].bEnable ) {
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	//カレントのレンズを設定したい
	//nIndex = pCombo->FindStringExact(-1, m_strLens);
	//pCombo->SetCurSel(nIndex);
	pCombo->SetCurSel(0);
}

// =========================================================================
//キャリブレーションピクセル表示グリッド
void CSrCcdArraySettingDlg::CalibPixelListGrid_Init()
{
	int Cols = 2 + 1;
	int Rows = 7 + 1;
	int FixRows = 1;
	int FixCols = 2;
	int row, col;
	DWORD dwTextStyle1,dwTextStyle2;
	int Index;
	int ItemWidth = 76;

	LPCTSTR pszGridSetting_Item[] = {
		"No.",
		"Calibration\r\nWave[nm]",
		"Pixel\r\n[dot]"
	};

	///// Grid Initialize /////
	m_CCDCalibListGrid.SetEditable(FALSE);
	m_CCDCalibListGrid.EnableDragAndDrop(TRUE);
	m_CCDCalibListGrid.SetTextBkColor(WHITE_COLOR);
	m_CCDCalibListGrid.SetRowResize(0);

	m_CCDCalibListGrid.SetEditable(TRUE);				// <--- When Edit is TRUE
	m_CCDCalibListGrid.EnableSelection(TRUE);
	m_CCDCalibListGrid.SetTrackFocusCell(FALSE);
	m_CCDCalibListGrid.SetFrameFocusCell(FALSE);
	m_CCDCalibListGrid.SetFixedColumnSelection(FALSE);
	m_CCDCalibListGrid.SetFixedRowSelection(TRUE);
	m_CCDCalibListGrid.SetListMode(TRUE);
	TRY{
		m_CCDCalibListGrid.SetRowCount(Rows);
		m_CCDCalibListGrid.SetColumnCount(Cols);
		m_CCDCalibListGrid.SetFixedRowCount(FixRows);
		m_CCDCalibListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH


	// Row 0 Height
	m_CCDCalibListGrid.SetRowHeight(0, 36);//Header Height

	// Row 1 - RowCount Height
	for(row = 1; row < m_CCDCalibListGrid.GetRowCount(); row++)
		m_CCDCalibListGrid.SetRowHeight(row, 18);

	// Col 0 Width
	m_CCDCalibListGrid.SetColumnWidth(0, 40);	// Wavelength Width

	// Col 1 - ColumnCount Width
	for (col = 1; col < m_CCDCalibListGrid.GetColumnCount(); col++)
		m_CCDCalibListGrid.SetColumnWidth(col, ItemWidth);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);//ダイアログのフォント取得
	//m_Font.GetLogFont(&LogFont);//ビューのフォント取得

	for ( row = 0; row < m_CCDCalibListGrid.GetRowCount(); row++ )
		for ( col = 0; col < m_CCDCalibListGrid.GetColumnCount(); col++ )
			m_CCDCalibListGrid.SetItemFont(row, col, &LogFont);

	///// Title Row /////
	//// Current Graph Data ////
	dwTextStyle1 = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	dwTextStyle2 = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	Index = 0;
	row = 1;
	//for ( Index = 0; Index <= Rows ; Index++ ) {
	for ( Index = 0; Index < m_CCDCalibListGrid.GetRowCount() ; Index++ ) {
		///// Wavelength /////
		if (Index != 0) {
			m_CCDCalibListGrid.SetItemTextFmt(Index, 0, "%d", Index);
			m_CCDCalibListGrid.SetItemTextFmt(Index, 1, g_lpszSrCcdArrayItem[Index-1]);
		}

		m_CCDCalibListGrid.SetItemFormat(Index, 0, dwTextStyle1);
		m_CCDCalibListGrid.SetItemFormat(Index, 1, dwTextStyle2);
		m_CCDCalibListGrid.SetItemFormat(Index, 2, dwTextStyle2);
	}

	//// Title Column /////
	//dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	dwTextStyle1 = DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS;
	Index = 0;
	for ( col = 0; col < m_CCDCalibListGrid.GetColumnCount(); col++ ) {
		m_CCDCalibListGrid.SetItemText(0, col, pszGridSetting_Item[Index]);
		m_CCDCalibListGrid.SetItemFormat(0, col, dwTextStyle1);
		Index++;
	}
}


LRESULT CSrCcdArraySettingDlg::CalibrationSet(WPARAM wparam, LPARAM lparam)
{
	//Kojika 20090525 Add
	CString strbuf;
	//Kojika 20090525 Add End
	CString strBuffer;
	DWORD dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

	int iPoint = wparam;	// 測定ポイントごとの回数
	CCellID cell;

	CNanoMeasureGraph::CalibrationData tCalibData;

	int i,j;
	BOOL bCheckSelectCell = FALSE;

	for(i=1; i<=7; i++){
		for(j=1; j<=2; j++){
			if(m_CCDCalibListGrid.IsCellSelected(i,j) == TRUE)
				bCheckSelectCell=TRUE;
		}
	}
	if(bCheckSelectCell==FALSE){
		for(i=1; i<=7; i++){
			if(m_CCDCalibListGrid.GetItemText(i,2) == ""){
				m_CCDCalibListGrid.SetSelectedRange(i, 0, i, 2 ,TRUE, TRUE);
				m_CCDCalibListGrid.SetFocusCell(i,2);
				break;
			}
			//if((i==7) && (m_CCDCalibListGrid.GetItemText(i,2) == "")){
			if(i==7){
				//AfxMessageBox("Already Set Calibration Value");
				//Kojika 20090525 Change
				LoadStringML(IDS_ALREADY_SET_CALIB_VALUE , strbuf, "Already Set Calibration Value");
				AfxMessageBox(strbuf);
				//Kojika 20090525 Change End
				m_pCCDCalibGraph->ResetCalibrationData();
				m_pCCDCalibGraph->Invalidate();
				return 0L;
			}
		}
	}

	tCalibData = m_pCCDCalibGraph->GetCalibrationData();
	strBuffer.Format("%d",tCalibData.PointIndex[iPoint-1]);
	cell = m_CCDCalibListGrid.GetFocusCell();
	int iCount=0;
	while(iCount <=7){
		if(m_CCDCalibListGrid.GetItemText(cell.row,2) != ""){
			if (cell.row<7)
				cell.row++;
			else
				cell.row =1;
		}
		else{
			m_CCDCalibListGrid.SetSelectedRange(cell.row, 0, cell.row, 2 ,TRUE, TRUE);
			m_CCDCalibListGrid.SetFocusCell(cell.row,2);
			break;
		}
		if(iCount==7) {
			//AfxMessageBox("Already Set Calibration Value");
			//Kojika 20090525 Change
			LoadStringML(IDS_ALREADY_SET_CALIB_VALUE , strbuf, "Already Set Calibration Value");
			AfxMessageBox(strbuf);
			//Kojika 20090525 Change End
			break;
		}
		iCount++;
	}
	m_CCDCalibListGrid.SetItemText(cell.row, 2, strBuffer);
	m_CCDCalibListGrid.Invalidate();
	return 0L;
}


LRESULT CSrCcdArraySettingDlg::CalibrationComplete(WPARAM wparam, LPARAM lparam)
{
	//Kojika 20090525 Add
	CString strBuffer;
	//Kojika 20090525 Add End

	//AfxMessageBox("Complete Calibration Setting");
	//Kojika 20090525 Change
	LoadStringML(IDS_COMPLETE_CALIB_SETTING , strBuffer, "Complete Calibration Setting");
	AfxMessageBox(strBuffer);
	//Kojika 20090525 Change End

	return 0L;
}


BOOL CSrCcdArraySettingDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_ESCAPE)
		{
			m_bScan_StopFlg=1;
			return TRUE;
		}
	//	if(pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_RETURN){
		if( m_pCCDCalibGraph->GetFocus() ){
			//return TRUE;
			return FALSE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CSrCcdArraySettingDlg::OnCalibClearButton()
{
	//for ( Index = 0; Index <= Rows ; Index++ ) {
	for ( int Index = 1; Index < m_CCDCalibListGrid.GetRowCount() ; Index++ ) {
		///// Wavelength /////
		m_CCDCalibListGrid.SetItemTextFmt(Index, 2, "");
	}
	m_pCCDCalibGraph->ResetCalibrationData();
	m_pCCDCalibGraph->Invalidate();
	m_CCDCalibListGrid.Invalidate();
}

void CSrCcdArraySettingDlg::OnOK()
{
	///// Save /////
	if(!CheckData())
		return;

	ConfigFile_SetNanoSpecIni(m_SrCcdArray, CONFIG_FILE_SR_CCD_ARRAY);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
BOOL CSrCcdArraySettingDlg::CheckData()
{
//Kojika 20090525 Add
	CString strBuffer;
//Kojika 20090525 Add End

	for(int i=1; i<=7; i++){
		if(m_CCDCalibListGrid.GetItemText(i,2) == ""){
//			int iRet=AfxMessageBox("There are wave value not set.\n\rWould you save them?", MB_OKCANCEL);
//Kojika 20090525 Change
			LoadStringML(IDS_WAVE_VALUE_NOT_SET , strBuffer, "There are wave value not set.\n\rWould you save them?");
			int iRet=AfxMessageBox(strBuffer, MB_OKCANCEL);
//Kojika 20090525 Change End
			if( iRet != IDOK )
				return FALSE;
			else
				break;
		}
	}

	CString buffer;
	for ( int iIndex = 0; iIndex < SR_CCD_ARRAY_ITEM_MAX; iIndex++ ) {
		strcpy( m_SrCcdArray[iIndex].szItem , g_lpszSrCcdArrayItem[iIndex] );
		buffer = m_CCDCalibListGrid.GetItemText(iIndex+1, 2);
		m_SrCcdArray[iIndex].dData = atof(buffer);
	}
	return TRUE;
}
