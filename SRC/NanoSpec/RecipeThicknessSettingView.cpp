// RecipeThicknessSettingView.cpp : implementation file
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "Math.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SharedMemory.h"
#include "NanoSpecDoc.h"
#include "RecipeThicknessSettingView.h"
#include "RecipeThicknessCopyForm.h"
#include "RecipeMainSrView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FIXROWS 1
#define FIXCOLS 3

#define STAGE_OFFSET		15
#define RADIUS				1
#define ELLIPSE_RADIUS		2//4

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

/////////////////////////////////////////////////////////////////////////////
// CRecipeThicknessSettingView dialog


CRecipeThicknessSettingView::CRecipeThicknessSettingView(CWnd* pParent /*=NULL*/)
	: CDialog(CRecipeThicknessSettingView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecipeThicknessSettingView)
	//}}AFX_DATA_INIT

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;
	m_CurrentCellID.col = -1;
	m_CurrentCellID.row = -1;

}


void CRecipeThicknessSettingView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeThicknessSettingView)
	DDX_Control(pDX, IDC_MEASUREMENT_STAGE_MAP, m_StageMapGraph);
	DDX_Control(pDX, IDC_MEASUREMENT_POINT_FOR_STRESS_GRID, m_MeasurementPointForStressListGrid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_THICKNESS_COPY, m_ThicknessCopyButton);
	DDX_Control(pDX, IDC_BUTTON_THICKNESS_DELETE, m_ThicknessClearButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CRecipeThicknessSettingView, CDialog)
	//{{AFX_MSG_MAP(CRecipeThicknessSettingView)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_THICKNESS_COPY, OnButtonThicknessCopy)
	ON_BN_CLICKED(IDC_BUTTON_THICKNESS_DELETE, OnButtonThicknessClear)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_SELCHANGING, IDC_MEASUREMENT_POINT_FOR_STRESS_GRID, OnSelChangeing)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecipeThicknessSettingView message handlers

BOOL CRecipeThicknessSettingView::OnInitDialog()
{
	CDialog::OnInitDialog();

	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	memset(&m_StageConfig,NULL,sizeof(m_StageConfig));
	memset(&m_StressConfig,NULL,sizeof(m_StressConfig));
	m_MeasurementPointForStressListGrid.DeleteAllItems();

	///// Load Stage Config & Stress Config /////
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);
//	  RecipeFile_LoadRecipe(&m_SampleInfo, m_StressConfig.szSampleName, RECIPE_FILE_SAMPLE);
	::CopyMemory(&m_OldStressParamInfo, &m_StressParamInfo, sizeof(STRESS_PARAM_INFO));

	///// Load Stage Program /////
	if( strlen(m_MainRcpInfo.MainRcpParam.hdr.szStage) != 0 ){//Stage Program 未選択
        if ( !RecipeFile_LoadRecipe(&m_StageStressProg, m_MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM_STRESS) ) {
            ::ZeroMemory(&m_StageStressProg, sizeof(m_StageStressProg));
        }
	}

	///// Copy Button /////
	m_ThicknessCopyButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ThicknessCopyButton.DrawFlatFocus(TRUE);

	///// Clear Button /////
	m_ThicknessClearButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ThicknessClearButton.DrawFlatFocus(TRUE);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	MeasurementPointForStressListGrid_Init();
	LoadThicknessData();


	///// Measurement Point Graph /////
	m_StageMapGraph.SetLineStatusMode(TRUE);
	SamplePointGraph_Init();

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecipeThicknessSettingView::MeasurementPointForStressListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 1; //有効Col数
	int iDispRow;//有効Row数
	int FixRows = FIXROWS;
	int FixCols = FIXCOLS;
	int Cols = iDispCol + FixCols;
	int Rows;
	DWORD dwTextStyle;
	int col, Row;
	int iCount, jCount;

	SYSTEM_CONFIG l_SystemConfig;

	char* ColItem_ENU[]={
		"No.",
		"X",
		"Y",
		"Thick.",
	};
	char* ColItem_JPN[]={
		"No.",
		"X",
		"Y",
		"膜厚",
	};

	///// Get System Config /////
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// Row Initialize /////
	iDispRow = m_StressConfig.dwLiftPinNumberOfLine * PIN_INTERVAL;
	Rows = FIXROWS + iDispRow;
	Row = FIXROWS;


	///// Grid Initialize /////
	m_MeasurementPointForStressListGrid.SetEditable(TRUE);
	m_MeasurementPointForStressListGrid.EnableDragAndDrop(FALSE);
	m_MeasurementPointForStressListGrid.SetTextBkColor(WHITE_COLOR);
	m_MeasurementPointForStressListGrid.SetRowResize(0);

	m_MeasurementPointForStressListGrid.EnableSelection(TRUE);
	m_MeasurementPointForStressListGrid.SetTrackFocusCell(FALSE);
	m_MeasurementPointForStressListGrid.SetFrameFocusCell(FALSE);
	m_MeasurementPointForStressListGrid.SetFixedColumnSelection(FALSE);
	m_MeasurementPointForStressListGrid.SetFixedRowSelection(TRUE);
	m_MeasurementPointForStressListGrid.SetListMode(TRUE);

	TRY{
		m_MeasurementPointForStressListGrid.SetRowCount(Rows);
		m_MeasurementPointForStressListGrid.SetColumnCount(Cols);
		m_MeasurementPointForStressListGrid.SetFixedRowCount(FixRows);
		m_MeasurementPointForStressListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	///// Grid Size Initialize /////
	// Height
	int iRow = 0;
	m_MeasurementPointForStressListGrid.SetRowHeight(iRow++, 40);		// Row 0
	while ( iRow < m_MeasurementPointForStressListGrid.GetRowCount() )	// Row 1 ～
		m_MeasurementPointForStressListGrid.SetRowHeight(iRow++, 20);

	// Width
	int iCol = 0;
	m_MeasurementPointForStressListGrid.SetColumnWidth(iCol++, 60); 	// Col 0
	while ( iCol < m_MeasurementPointForStressListGrid.GetColumnCount() ) // Col 1 ～
		m_MeasurementPointForStressListGrid.SetColumnWidth(iCol++, 75);

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < Cols; col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		if(l_SystemConfig.nLanguage == 0)
			Item.strText.Format("%s",ColItem_ENU[col]);
		else
			Item.strText.Format("%s",ColItem_JPN[col]);
		m_MeasurementPointForStressListGrid.SetItem(&Item);
	}

	///// Number Row /////
	for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
		if (!m_StressConfig.Line[iCount].bValidLine) {
			continue;
		}
		for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = Row;
			Item.col = 0;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("L%dP%d", iCount+1, jCount+1);
			m_MeasurementPointForStressListGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = Row;
			Item.col = 1;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[iCount].MeasPos[jCount].lX / MICROMETRE)); 	// 単位変換(um->mm)
			m_MeasurementPointForStressListGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = Row;
			Item.col = 2;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[iCount].MeasPos[jCount].lY / MICROMETRE)); 	// 単位変換(um->mm)
			m_MeasurementPointForStressListGrid.SetItem(&Item);

			Row++;
		}
	}
	return;
}


void CRecipeThicknessSettingView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	long lDrawStageSizeX, lDrawStageSizeY;
	CRect StageRect;
//	  CRect SampleRect;

	///// Region Map /////
	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
	CDC* pDC = pPictBox->GetDC();
	pPictBox->GetClientRect(&StageRect);

	///// Draw Stage Color /////
	CBrush BrushStage(BLACK_COLOR);
	m_oldBrush = pDC->SelectObject(&BrushStage);
	CPen PenStage(PS_SOLID, 1, BLACK_COLOR);
	m_oldPen = pDC->SelectObject(&PenStage);

//	///// Stage Rect Offset /////
  long StageRectLeft = StageRect.left + STAGE_OFFSET;
  long StageRectRight = StageRect.right - STAGE_OFFSET;
  long StageRectTop = StageRect.top + STAGE_OFFSET;
  long StageRectBottom = StageRect.bottom - STAGE_OFFSET;

//	///// ステージ有効範囲 /////
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

	SamplePointGraph_DataSet();

	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
	ReleaseDC(pDC);
}

void CRecipeThicknessSettingView::OnButtonThicknessCopy()
{
	int iCount;
	int jCount;
	int Row = FIXROWS;
	DWORD dwTextStyle;
	GV_ITEM Item;

	CRecipeThicknessCopyForm RcpThickCpyForm;
	if(RcpThickCpyForm.DoModal() == IDOK){
		for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
			if (!m_StressConfig.Line[iCount].bValidLine) {
				continue;
			}
			for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
				dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
				dwTextStyle |= DT_END_ELLIPSIS;
				Item.mask = GVIF_TEXT|GVIF_FORMAT;
				Item.row = Row;
				Item.col = 3;
				Item.nFormat = dwTextStyle;
				Item.strText.Format("%.3f", RcpThickCpyForm.m_dEditThicknessValue);
				m_MeasurementPointForStressListGrid.SetItem(&Item);
				m_MeasurementPointForStressListGrid.Refresh();
				Row++;
			}
		}
	}
}

void CRecipeThicknessSettingView::OnButtonThicknessClear()
{
	int iCount;
	int jCount;
	int Row = FIXROWS;
	DWORD dwTextStyle;
	GV_ITEM Item;

	for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
		if (!m_StressConfig.Line[iCount].bValidLine) {
			continue;
		}
		for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = Row;
			Item.col = 3;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("");
			m_MeasurementPointForStressListGrid.SetItem(&Item);
			m_MeasurementPointForStressListGrid.Refresh();
			Row++;
		}
	}
}

void CRecipeThicknessSettingView::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(CheckData() == FALSE){
		return;
	}

	CDialog::OnOK();
}

void CRecipeThicknessSettingView::OnCancel()
{
//          MessageBox("応力測定用膜厚は修正されています。\n変更を破棄しますか？", m_strCaption, MB_OK | MB_ICONSTOP);
	CString strBuffer, strTitle;
	STRESS_PARAM_INFO tempStressInfo;
	::CopyMemory(&tempStressInfo, &m_OldStressParamInfo, sizeof(STRESS_PARAM_INFO));
	Update(&tempStressInfo);

	//if(memcmp(&m_OldStressParamInfo, &m_StressParamInfo, sizeof(STRESS_PARAM_INFO)) != 0){
	if(memcmp(&m_OldStressParamInfo, &tempStressInfo, sizeof(STRESS_PARAM_INFO)) != 0){
	    LoadStringML(IDS_STRESS_THICKNESS_MODLFIED, strBuffer, "Thickness for Stress has been modified.\nDo you Discard the Changeds?");
	    LoadStringML(IDS_TITLE_THICKNESS_SETTING, strTitle, "THICKNESS SETTING");
	    if(MessageBox(strBuffer, strTitle, MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL){ return; }
	}
	CDialog::OnCancel();
}

BOOL CRecipeThicknessSettingView::CheckData()
{
	GV_ITEM Item;
	double dtmpVal;
	int Row = FIXROWS;
	int iCount, jCount;

	CString	strBuffer;
	char szMessage[256];
	STRESS_PARAM_INFO	tempSTParamInfo;

	memset(&tempSTParamInfo, 0, sizeof(STRESS_PARAM_INFO));

	switch(m_nOpenMode)
	{
		case modeDefaultSetting:
			for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
				for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
					Item.mask = GVIF_TEXT|GVIF_FORMAT;
					Item.row = Row;
					Item.col = 3;
					m_MeasurementPointForStressListGrid.GetItem(&Item);
					if (CheckInputValueDouble(Item.strText, &dtmpVal)){
						if(dtmpVal > 0){
							tempSTParamInfo.Line[iCount].dPtThick[jCount] = dtmpVal;
							Row++;
							continue;
						}
						Invalidate( FALSE );
						LoadStringML(IDS_STRESS_INPUT_THICKNESS, strBuffer, "Please input L%dP%d Thickenss.");
						sprintf(szMessage, strBuffer, iCount+1, jCount+1);
						AfxMessageBox(szMessage);
						return FALSE;
					}
					else{
						if(Item.strText.IsEmpty() != 0){
							LoadStringML(IDS_STRESS_INPUT_THICKNESS, strBuffer, "Please input L%dP%d Thickenss.");
						}
						else{
							LoadStringML(IDS_STRESS_ENTER_NUM_THICK, strBuffer, "Please Enter the Numbers L%dP%d Thickenss.");
						}
						Invalidate( FALSE );
						sprintf(szMessage, strBuffer, iCount+1, jCount+1);
						AfxMessageBox(szMessage);
						return FALSE;
					}
				}
			}
		break;
		default:
		LoadStringML(IDS_POINT_NAME_NONE, strBuffer, "None");

		if( strlen(m_MainRcpInfo.MainRcpParam.hdr.szStage) == 0 || (strcmp(m_MainRcpInfo.MainRcpParam.hdr.szStage, strBuffer) == 0)){//Stage Program 未選択
			for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
				for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
					Item.mask = GVIF_TEXT|GVIF_FORMAT;
					Item.row = Row;
					Item.col = 3;
					m_MeasurementPointForStressListGrid.GetItem(&Item);
					if (CheckInputValueDouble(Item.strText, &dtmpVal)){
						if(dtmpVal > 0){
							tempSTParamInfo.Line[iCount].dPtThick[jCount] = dtmpVal;
							Row++;
							continue;
						}
						Invalidate( FALSE );
						LoadStringML(IDS_STRESS_INPUT_THICKNESS, strBuffer, "Please input L%dP%d Thickenss.");
						sprintf(szMessage, strBuffer, iCount+1, jCount+1);
						AfxMessageBox(szMessage);
						return FALSE;
					}
					else{
						if(Item.strText.IsEmpty() != 0){
							LoadStringML(IDS_STRESS_INPUT_THICKNESS, strBuffer, "Please input L%dP%d Thickenss.");
						}
						else{
							LoadStringML(IDS_STRESS_ENTER_NUM_THICK, strBuffer, "Please Enter the Numbers L%dP%d Thickenss.");
						}
						Invalidate( FALSE );
						sprintf(szMessage, strBuffer, iCount+1, jCount+1);
						AfxMessageBox(szMessage);
						return FALSE;
					}
				}
			}
		}
		else{//StageProgram選択済み
			for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
				for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
					Item.mask = GVIF_TEXT|GVIF_FORMAT;
					Item.row = Row;
					Item.col = 3;
					m_MeasurementPointForStressListGrid.GetItem(&Item);

					if (!m_StageStressProg.Line[iCount].bValidLine) {// 無効Lineの場合
						if(CheckInputValueDouble(Item.strText, &dtmpVal)){
							tempSTParamInfo.Line[iCount].dPtThick[jCount] = dtmpVal;
							Row++;
							continue;
						}
						else{
							if(Item.strText.IsEmpty() != 0){
								tempSTParamInfo.Line[iCount].dPtThick[jCount] = 0.0;
								Row++;
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
						if (CheckInputValueDouble(Item.strText, &dtmpVal)){// 有効Lineの場合
							if(dtmpVal > 0){
								tempSTParamInfo.Line[iCount].dPtThick[jCount] = dtmpVal;
								Row++;
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

	::CopyMemory(&m_StressParamInfo, &tempSTParamInfo, sizeof(STRESS_PARAM_INFO));

	return TRUE;
}

void CRecipeThicknessSettingView::Update(STRESS_PARAM_INFO* tempStressInfo)
{
	GV_ITEM Item;
	double dtmpVal;
	int Row = FIXROWS;
	int iCount, jCount;

	for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
		for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = Row;
			Item.col = 3;
			m_MeasurementPointForStressListGrid.GetItem(&Item);
			if (CheckInputValueDouble(Item.strText, &dtmpVal)){
				tempStressInfo->Line[iCount].dPtThick[jCount] = dtmpVal;
				Row++;
				continue;
			}
			else{
				tempStressInfo->Line[iCount].dPtThick[jCount] = -1;
				Row++;
				continue;
			}
		}
	}
}

void CRecipeThicknessSettingView::SamplePointGraph_Init()
{
	CString strBuffer;
	CNanoPointGraph::GraphConfig tConfig;

	m_StageMapGraph.ShowWindow(SW_HIDE);

	////Point Graph Data Reset
	m_StageMapGraph.ResetAll();

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
	tConfig.PinColor = RGB(0xFF,0x00,0xFF);

	// 2013.01.09 bagus -->
	tConfig.OriginColor = GREEN_COLOR;
	// 2013.01.09 bagus -->

	////CNanoPointGraphクラス内サイズ(Sample Size)
	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
		// X<->Y入れ替え
		tConfig.SampleSizeY = m_StressConfig.Size.dx;//m_SampleInfo.Size.dx;
		tConfig.SampleSizeX = m_StressConfig.Size.dy;//m_SampleInfo.Size.dy;
	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
		// X-Yそのまま
		tConfig.SampleSizeX = m_StressConfig.Size.dx;
		tConfig.SampleSizeY = m_StressConfig.Size.dy;
	} else {
		// 矢印設定がおかしい
	}
	m_StageMapGraph.SetConfig(&tConfig);

	SamplePointGraph_DataSet();
}

void CRecipeThicknessSettingView::SamplePointGraph_DataSet()
{
	////Point Graph Data Reset
	m_StageMapGraph.ResetAll();

	CNanoPointGraph::LSData tLineData;

	int i, j;

	////Point Data
	double dOffSetValueX;
	double dOffSetValueY;
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
		dOffSetValueX = (m_StressConfig.Size.dx/2);
		dOffSetValueY = (m_StressConfig.Size.dy/2);
		break;
	case 2:
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		break;
	case 3:
		dOffSetValueX = 0;
		dOffSetValueY = m_StressConfig.Size.dy;
		break;
	case 4:
		dOffSetValueX = m_StressConfig.Size.dx;
		dOffSetValueY = m_StressConfig.Size.dy;
		break;
	case 5:
		dOffSetValueX = m_StressConfig.Size.dx;
		dOffSetValueY = 0;
		break;
	}

// 2013.01.09 bagus -->
	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		m_StageMapGraph.SetOriginPointData(dOffSetValueX, dOffSetValueY);
	}
	else {
		m_StageMapGraph.SetOriginPointData(dOffSetValueY, dOffSetValueX);
	}
// 2013.01.09 bagus <--

	for (i=0; (DWORD)i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
		if(i >= STRESS_PINS_MAX){
			break;
		}
/*
// Pin Data
		for(j = 0; j<m_StressConfig.Line[i].dwLiftPinNumberOfLn;j++){
			long lPinX,lPinY;
			lPinX = lPinY = 0L;
			switch (m_StageConfig.Dir.X) {
			case 1: 	// X:→
			default:
				lPinX = dOffSetValueX + m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 2: 	// X:↑
				lPinY = dOffSetValueX - m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 3: 	// X:←
				lPinX = dOffSetValueX - m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 4: 	// X:↓
				lPinY = dOffSetValueX + m_StressConfig.Line[i].PinPos[j].lX;
				break;
			}

			switch (m_StageConfig.Dir.Y) {
			case 1: 	// Y:→
				lPinX = dOffSetValueY + m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 2: 	// Y:↑
			default:
				lPinY = dOffSetValueY - m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 3: 	// Y:←
				lPinX = dOffSetValueY - m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 4: 	// Y:↓
				lPinY = dOffSetValueY + m_StressConfig.Line[i].PinPos[j].lY;
				break;
			}
			m_StageMapGraph.AddPinData(lPinX,lPinY);
		}*/
		  for (j=0; (DWORD)j<m_StressConfig.Line[i].dwSectionNum; j++) {

			if((DWORD)i >= m_StressConfig.dwLiftPinNumberOfLine){
				continue;
			}
			if(j >= STRESS_SECTIONS_MAX){
				break;
			}

			tLineData.Status = CNanoPointGraph::SECTION_NOT_MEASURED;

			switch (m_StageConfig.Dir.X) {
			case 1: 	// X:→
			default:
				tLineData.StartX = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndX = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 2: 	// X:↑
				tLineData.StartY = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndY = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 3: 	// X:←
				tLineData.StartX = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndX = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 4: 	// X:↓
				tLineData.StartY = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndY = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			}

			switch (m_StageConfig.Dir.Y) {
			case 1: 	// Y:→
				tLineData.StartX = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndX = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 2: 	// Y:↑
			default:
				tLineData.StartY = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndY = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 3: 	// Y:←
				tLineData.StartX = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndX = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 4: 	// Y:↓
				tLineData.StartY = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndY = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			}

			m_StageMapGraph.AddLineData(&tLineData);
		}
	}

	double	dPointX, dPointY;
	for (i=0; (DWORD)i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
		if (!m_StressConfig.Line[i].bValidLine) {
			continue;
		}

		for(int iPoint= 0; iPoint<PIN_INTERVAL; iPoint++){
			switch (m_StageConfig.Dir.X) {
			case 1: 	// X:→
			default:
				dPointX = dOffSetValueX + m_StressConfig.Line[i].MeasPos[iPoint].lX;
				break;
			case 2: 	// X:↑
				dPointY = dOffSetValueX - m_StressConfig.Line[i].MeasPos[iPoint].lX;
				break;
			case 3: 	// X:←
				dPointX = dOffSetValueX - m_StressConfig.Line[i].MeasPos[iPoint].lX;
				break;
			case 4: 	// X:↓
				dPointY = dOffSetValueX + m_StressConfig.Line[i].MeasPos[iPoint].lX;
				break;
			}

			switch (m_StageConfig.Dir.Y) {
			case 1: 	// Y:→
				dPointX = dOffSetValueY + m_StressConfig.Line[i].MeasPos[iPoint].lY;
				break;
			case 2: 	// Y:↑
			default:
				dPointY = dOffSetValueY - m_StressConfig.Line[i].MeasPos[iPoint].lY;
				break;
			case 3: 	// Y:←
				dPointX = dOffSetValueY - m_StressConfig.Line[i].MeasPos[iPoint].lY;
				break;
			case 4: 	// Y:↓
				dPointY = dOffSetValueY + m_StressConfig.Line[i].MeasPos[iPoint].lY;
				break;
			}
			m_StageMapGraph.AddPoint(dPointX, dPointY);
		}
	}
}

void CRecipeThicknessSettingView::DrawSample(CDC* pDC, int iOrg, int DirX, int DirY)
{
	WINDOWPLACEMENT Stageplacement, Sampleplacement;

	///// Region Map /////
	GetDlgItem(IDC_STAGE_MAP)->GetWindowPlacement(&Stageplacement);

	///// ステージ有効範囲(黒い四角はこのサイズで描画されている) /////
	double dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);

// 2013.01.09 bagus -->
	double dSampleSizeX;
	double dSampleSizeY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSizeX = m_StressConfig.Size.dx;
		dSampleSizeY = m_StressConfig.Size.dy;
	}
	else {
		dSampleSizeX = m_StressConfig.Size.dy;
		dSampleSizeY = m_StressConfig.Size.dx;
	}

	///// 　サンプルサイズがステージの大きさとり小さいかチェック /////
// 	if ((m_StressConfig.Size.dx > dStageSizeX) || (m_StressConfig.Size.dy > dStageSizeY))
// 		return;
	if ((dSampleSizeX > dStageSizeX) || (dSampleSizeY > dStageSizeY))
		return;
// 2013.01.09 bagus <--

	///// サンプル描画サイズの計算（ステージ描画サイズ対するサンプル描画サイズを計算） /////
	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX) * m_StressConfig.Size.dx / dStageSizeX;
	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY) * m_StressConfig.Size.dy / dStageSizeY;

	///// ステージとサンプルの原点オフセットの合計値 /////
	double dTotalOrgOffsetPosX, dTotalOrgOffsetPosY;

	///// サンプルの描画位置 /////
	double dtop, dbottom, dright, dleft;

	//Sample描画位置設定
	//四角形状サンプルのみ対応
	switch(m_StressConfig.dwShape){
	case SAMPLE_SHAPE_RECTANGLE:		//Sample shaped RectAngle
		switch(iOrg){
		case 1: ///// センター基準	//X軸：→+　Y軸：↑+のみ対応
		default:
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StressConfig.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StressConfig.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;

			///// サンプル描画位置を計算 /////
			//左基準
			dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX / 2);
			dright	= dleft + dDrawSampleSizeX;
			//下基準
			dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + (dDrawSampleSizeY / 2);
			dtop	= dbottom - dDrawSampleSizeY;
			break;

		case 2: /////LeftTop
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
			dTotalOrgOffsetPosX = (m_StressConfig.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			dTotalOrgOffsetPosY = (m_StressConfig.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;

			///// サンプル描画位置を計算 /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom = dtop + dDrawSampleSizeY;
			break;
		case 3: /////LeftBottom
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
			dTotalOrgOffsetPosX = ((m_StressConfig.Origin.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX);
			dTotalOrgOffsetPosY = ((m_StressConfig.Origin.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY);

			///// サンプル描画位置を計算 /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
			break;
		case 4: /////RightBottom
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
			dTotalOrgOffsetPosX = (m_StressConfig.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			dTotalOrgOffsetPosY = (m_StressConfig.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;

			///// サンプル描画位置を計算 /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
			break;
		case 5: /////RightTop
			///// ステージとサンプルの原点オフセットの合計値を計算 /////
			dTotalOrgOffsetPosX = (m_StressConfig.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			dTotalOrgOffsetPosY = (m_StressConfig.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;

			///// サンプル描画位置を計算 /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom = dtop + dDrawSampleSizeY;
			break;
		}

	  if ((m_dDrawStartStagePosX <= dleft) && (dright <= m_dDrawEndStagePosX) && (m_dDrawStartStagePosY <= dtop) && (dbottom <= m_dDrawEndStagePosY)){
		  Sampleplacement.rcNormalPosition.left   = Stageplacement.rcNormalPosition.left + dleft;
		  Sampleplacement.rcNormalPosition.right  = Stageplacement.rcNormalPosition.left + dright;
		  Sampleplacement.rcNormalPosition.top	  = Stageplacement.rcNormalPosition.top  + dtop;
		  Sampleplacement.rcNormalPosition.bottom = Stageplacement.rcNormalPosition.top  + dbottom;
		  //四角Sample描画
		  m_StageMapGraph.SetWindowPlacement(&Sampleplacement);
		  m_StageMapGraph.ShowWindow(SW_SHOW);
	  }
	  else{
		  m_StageMapGraph.ShowWindow(SW_HIDE);
	  }

	}
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);
}

// =========================================================================
// OnSelChangeing IDC_MEASUREMENT_POINT_FOR_STRESS_GRID選択行のスキャンポイントについて、マップのスキャンポイントを選択色に設定する
void CRecipeThicknessSettingView::OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	///// Draw SelectPoint /////
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID CellID;
	CellID.row=pItem->iRow;
	CellID.col=pItem->iColumn;

	if ( !m_MeasurementPointForStressListGrid.IsValid(CellID) )
		return;

	//PointGraph選択ポイント色変更
	m_StageMapGraph.Select(CellID.row-1);

	//現在の選択中セル
	m_CurrentCellID = CellID;
}

void CRecipeThicknessSettingView::LoadThicknessData()
{
	int DefultValue = 0;
	int iCount, jCount;
	int Row = FIXROWS;
	GV_ITEM Item;
	DWORD dwTextStyle;

//	if(m_nOpenMode == modeNew){
	for(iCount=0; (DWORD)iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
		if (!m_StressConfig.Line[iCount].bValidLine) {
			Row = Row + PIN_INTERVAL;
			continue;
		}
		for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = Row;
			Item.col = 3;
			Item.nFormat = dwTextStyle;
			//Saiki 20110304 Change ----->
			//if(m_nOpenMode != modeNew && m_nOpenMode != modeDefaultSetting){
			//	Item.strText.Format("%.3f", m_StressParamInfo.Line[iCount].dPtThick[jCount]);
			//}
			//else if(m_nOpenMode == modeDefaultSetting){
			//	Item.strText.Format("%.3f", m_StressParamInfo.Line[iCount].dPtThick[jCount]);
			//}
			//else{
			//	Item.strText.Format("%.3f", DefultValue);
			//}
			Item.strText.Format("%.3f", m_StressParamInfo.Line[iCount].dPtThick[jCount]);
			//Saiki 20110304 Change <-----
			m_MeasurementPointForStressListGrid.SetItem(&Item);
			m_MeasurementPointForStressListGrid.Refresh();
			Row++;
		}
	}
//	}
//	else{
//		for(iCount=0; iCount<m_StressConfig.dwLiftPinNumberOfLine; iCount++){
//			if (!m_StressConfig.Line[iCount].bValidLine) {
//				continue;
//			}
//			for (jCount=0; jCount<PIN_INTERVAL; jCount++) {
//				dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
//				dwTextStyle |= DT_END_ELLIPSIS;
//				Item.mask = GVIF_TEXT|GVIF_FORMAT;
//				Item.row = Row;
//				Item.col = 3;
//				Item.nFormat = dwTextStyle;
//				Item.strText.Format("%.3f", m_StressParamInfo.Line[iCount].dPtThick[jCount]);
//				m_MeasurementPointForStressListGrid.SetItem(&Item);
//				m_MeasurementPointForStressListGrid.Refresh();
//				Row++;
//			}
//		}
//	}
}

BOOL CRecipeThicknessSettingView::CheckInputValueLong(CString strVal, long *lValue)
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

BOOL CRecipeThicknessSettingView::CheckInputValueDouble(CString strVal, double *dValue)
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
