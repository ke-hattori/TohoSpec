// SitePatternSetDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "GridCellNumeric.h"
#include "BitmapStatic.h"
#include "PatRecDeskewDlg.h"
#include "SaveAsDlg.h"
#include "SitePatternEditDlg.h"
#include "SitePatternSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PATTERN_NO_SELECT	(0)
#define PATTERN_ALL_NEW 	(1)
#define PATTERN_MODIFY		(2)

/////////////////////////////////////////////////////////////////////////////
// CSitePatternSetDlg ダイアログ


CSitePatternSetDlg::CSitePatternSetDlg(STAGE_PROG_INFO_HDR* pStageProgInfoHdr, CWnd* pParent /*=NULL*/)
	:	m_pStageProgInfoHdr(pStageProgInfoHdr) , CDialog(CSitePatternSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSitePatternDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

CSitePatternSetDlg::~CSitePatternSetDlg()
{
	::DeleteFile(PATTERN_IMG_TEMP);
	::DeleteFile(PATTERN_IMG_1);
	::DeleteFile(PATTERN_IMG_2);
}

void CSitePatternSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSitePatternSetDlg)
	DDX_Control(pDX, IDC_PATTERN1_SETUP_BUTTON, m_Pattern1SetButton);
	DDX_Control(pDX, IDC_PATTERN2_SETUP_BUTTON, m_Pattern2SetButton);
	DDX_Control(pDX, IDC_PATTERN_CHG_BUTTON, m_PatternChgButton);
	DDX_Control(pDX, IDC_PATTERN_ALL_CHG_BUTTON, m_PatternAllChgButton);
	DDX_Control(pDX, IDC_PATTERN_DEL_BUTTON, m_PatternDelButton);
	DDX_Control(pDX, IDC_PATTERN_ALL_DEL_BUTTON, m_PatternAllDelButton);
	DDX_Control(pDX, IDC_ALL_ENABLE_BUTTON, m_AllEnableButton);
	DDX_Control(pDX, IDC_ALL_DISABLE_BUTTON, m_AllDisableButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_MEASUREMENT_POINT_GRID, m_MeasurementPointGrid);
	DDX_Text(pDX, IDC_LENS1, m_szLens1);
	DDX_Text(pDX, IDC_OFFSET_X1, m_szOffsetX1);
	DDX_Text(pDX, IDC_OFFSET_Y1, m_szOffsetY1);
	DDX_Text(pDX, IDC_LENS2, m_szLens2);
	DDX_Text(pDX, IDC_OFFSET_X2, m_szOffsetX2);
	DDX_Text(pDX, IDC_OFFSET_Y2, m_szOffsetY2);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STAGE_PROGRAM_NAME, m_pStageProgInfoHdr->hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Control(pDX, IDC_SITE_PR_IMG1, m_stcSitePrImg[0]);
	DDX_Control(pDX, IDC_SITE_PR_IMG2, m_stcSitePrImg[1]);
}


BEGIN_MESSAGE_MAP(CSitePatternSetDlg, CDialog)
	//{{AFX_MSG_MAP(CSitePatternSetDlg)
	ON_BN_CLICKED(IDC_PATTERN1_SETUP_BUTTON, OnPattern1SetupButton)
	ON_BN_CLICKED(IDC_PATTERN2_SETUP_BUTTON, OnPattern2SetupButton)
	ON_CBN_SELCHANGE(IDC_SITE_PR_EXECUTION_MODE, OnSelchangeSitePrExecutionMode)
	ON_CBN_SELCHANGE(IDC_SITE_PR_FAIL_OPTION, OnSelchangeSitePrFailOption)
	ON_BN_CLICKED(IDC_PATTERN_CHG_BUTTON, OnPatternChgButton)
	ON_BN_CLICKED(IDC_PATTERN_ALL_CHG_BUTTON, OnPatternAllChgButton)
	ON_BN_CLICKED(IDC_PATTERN_DEL_BUTTON, OnPatternDelButton)
	ON_BN_CLICKED(IDC_PATTERN_ALL_DEL_BUTTON, OnPatternAllDelButton)
	ON_BN_CLICKED(IDC_ALL_ENABLE_BUTTON, OnAllEnableButton)
	ON_BN_CLICKED(IDC_ALL_DISABLE_BUTTON, OnAllDisableButton)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_SELCHANGING, IDC_MEASUREMENT_POINT_GRID, OnSelChangeing)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSitePatternSetDlg メッセージ ハンドラ

BOOL CSitePatternSetDlg::OnInitDialog()
{
	int i;
	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	CDialog::OnInitDialog();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	//// Button Control /////
	m_Pattern1SetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Pattern1SetButton.DrawFlatFocus(TRUE);
	m_Pattern2SetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Pattern2SetButton.DrawFlatFocus(TRUE);
	m_PatternChgButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PatternChgButton.DrawFlatFocus(TRUE);
	m_PatternAllChgButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PatternAllChgButton.DrawFlatFocus(TRUE);
	m_PatternDelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PatternDelButton.DrawFlatFocus(TRUE);
	m_PatternAllDelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PatternAllDelButton.DrawFlatFocus(TRUE);
	m_AllEnableButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AllEnableButton.DrawFlatFocus(TRUE);
	m_AllDisableButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AllDisableButton.DrawFlatFocus(TRUE);
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Static Control /////
	m_stcStageProgramName.SubclassWindow(GetDlgItem(IDC_STAGE_PROGRAM_NAME)->GetSafeHwnd());
	m_stcStageProgramName.SetBkColor(WATER_COLOR);
	m_stcLens1.SubclassWindow(GetDlgItem(IDC_LENS1)->GetSafeHwnd());
	m_stcLens1.SetBkColor(WATER_COLOR);
	m_stcOffsetX1.SubclassWindow(GetDlgItem(IDC_OFFSET_X1)->GetSafeHwnd());
	m_stcOffsetX1.SetBkColor(WATER_COLOR);
	m_stcOffsetY1.SubclassWindow(GetDlgItem(IDC_OFFSET_Y1)->GetSafeHwnd());
	m_stcOffsetY1.SetBkColor(WATER_COLOR);
	m_stcLens2.SubclassWindow(GetDlgItem(IDC_LENS2)->GetSafeHwnd());
	m_stcLens2.SetBkColor(WATER_COLOR);
	m_stcOffsetX2.SubclassWindow(GetDlgItem(IDC_OFFSET_X2)->GetSafeHwnd());
	m_stcOffsetX2.SetBkColor(WATER_COLOR);
	m_stcOffsetY2.SubclassWindow(GetDlgItem(IDC_OFFSET_Y2)->GetSafeHwnd());
	m_stcOffsetY2.SetBkColor(WATER_COLOR);

	///// SitePR Execution Mode /////
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_SITE_PR_EXECUTION_MODE);
	pCombo->ResetContent();
	for ( i = 0; i < SITE_PR_MODE_MAX; i++ ){
		// Kojika 20090528 Change
		//pCombo->AddString(g_lpszSitePatternMode[i]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSitePatternMode_ENU[i]);
		}
		else{
			pCombo->AddString(g_lpszSitePatternMode_JPN[i]);
		}
		// Kojika 20090528 Change End
	}
	m_SelectMode = m_pStageProgInfoHdr->nSitePatternMode;
	pCombo->SetCurSel(m_SelectMode);

	///// SitePR Fail Option /////
	CComboBox* pFailCombo;
	pFailCombo = (CComboBox*)GetDlgItem(IDC_SITE_PR_FAIL_OPTION);
	pFailCombo->ResetContent();
	for ( i = 0; i < SITE_PR_FAIL_OPTION_MAX; i++ ){
		// Kojika 20090528 Change
		//pFailCombo->AddString(g_lpszSitePatternFailOption[i]);
		if(l_SystemConfig.nLanguage == 0){
			pFailCombo->AddString(g_lpszSitePatternFailOption_ENU[i]);
		}
		else{
			pFailCombo->AddString(g_lpszSitePatternFailOption_JPN[i]);
		}
		// Kojika 20090528 Change End
	}
	m_FailMode = m_pStageProgInfoHdr->nSitePrFailOption;
	pFailCombo->SetCurSel(m_FailMode);

	///// Measurement Point Grid /////
	MeasurementPointGrid_Init(m_iNumScans);
	GridEditable();

	///// Set Data /////
	SetData();

	///// Pattern Data /////
	DispPattern();

	///// Operation Enable /////
	BOOL bFlg;
	bFlg = m_pStageProgInfoHdr->nSitePatternMode;
	OperationEnable(bFlg);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ==========================================================================
//
BOOL CSitePatternSetDlg::MeasurementPointGrid_Init(int iRows)
{
	///// Grid Initialize /////
	GV_ITEM Item;
	int Cols = 6;
//	int Rows = iRows >= 1001 ? iRows+1+1 : 1000+1 ;//NumScanPoint + Header + AddCell;
	int Rows = iRows+1; 						//NumScanPoint + Header;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;
	//Saiki 20090601 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090601 Add <-----

	//Saiki 20090601 Change ----->
	//char* ColItem[]={
	//	"Point",
	//	"X (mm)",
	//	"Y (mm)",
	//	"Pattern1",
	//	"Pattern2",
	//	"ENABLE",
	//};
	char* ColItem_ENU[]={
		"Point",
		"X (mm)",
		"Y (mm)",
		"Pattern1",
		"Pattern2",
		"Enable",
	};
	char* ColItem_JPN[]={
		"ポイント",
		"X (mm)",
		"Y (mm)",
		"パターン1",
		"パターン2",
		"有効",
	};
	//Saiki 20090601 Change <-----

	///// Grid Initialize /////
	m_MeasurementPointGrid.SetListMode(FALSE);
	m_MeasurementPointGrid.EnableDragAndDrop(FALSE);
	m_MeasurementPointGrid.SetTextBkColor(WHITE_COLOR);
	m_MeasurementPointGrid.SetRowResize(0);
	m_MeasurementPointGrid.SetEditable(TRUE);				// <--- When Edit is TRUE
	m_MeasurementPointGrid.EnableSelection(TRUE);			// <--- When Edit is TRUE
	m_MeasurementPointGrid.SetTrackFocusCell(FALSE);		// <--- When Edit is TRUE
	m_MeasurementPointGrid.SetFrameFocusCell(FALSE);		// <--- When Edit is TRUE
	m_MeasurementPointGrid.SetFixedColumnSelection(FALSE);
	m_MeasurementPointGrid.SetFixedRowSelection(TRUE);
//	m_MeasurementPointGrid.SetScrollInfo();

	TRY {
		m_MeasurementPointGrid.SetRowCount(Rows);
		m_MeasurementPointGrid.SetColumnCount(Cols);
		m_MeasurementPointGrid.SetFixedRowCount(FixRows);
		m_MeasurementPointGrid.SetFixedColumnCount(FixCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	END_CATCH

	///// Grid Size Initialize /////
	m_MeasurementPointGrid.SetRowHeight(0, 18);
	m_MeasurementPointGrid.SetColumnWidth(0, 50);

	for (row = 1; row < m_MeasurementPointGrid.GetRowCount(); row++){
		m_MeasurementPointGrid.SetRowHeight(row, 18);
	}

	m_MeasurementPointGrid.SetColumnWidth(1, 70);
	m_MeasurementPointGrid.SetColumnWidth(2, 70);
	m_MeasurementPointGrid.SetColumnWidth(3, 240);
	m_MeasurementPointGrid.SetColumnWidth(4, 240);
	m_MeasurementPointGrid.SetColumnWidth(5, 60);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);

	for (row = 0; row < m_MeasurementPointGrid.GetRowCount(); row++){
		for (col = 0; col < m_MeasurementPointGrid.GetColumnCount(); col++){
			m_MeasurementPointGrid.SetItemFont(row,col,&LogFont);
		}
	}

	///// Title Column /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_MeasurementPointGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		//Saiki 20090601 Change ----->
		//Item.strText.Format("%s", ColItem[col]);
		if(l_SystemConfig.nLanguage == 0){
			Item.strText.Format("%s", ColItem_ENU[col]);
		}
		else{
			Item.strText.Format("%s", ColItem_JPN[col]);
		}
		//Saiki 20090601 Change <-----
		m_MeasurementPointGrid.SetItem(&Item);
	}

	///// Title Row /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_MeasurementPointGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%d", row);
		m_MeasurementPointGrid.SetItem(&Item);
	}

	///// Grid Alignment /////
	dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 1; col < m_MeasurementPointGrid.GetColumnCount()-1; col++){
		for(row = 1; row < m_MeasurementPointGrid.GetRowCount(); row++){
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("");
			m_MeasurementPointGrid.SetItem(&Item);
		}
	}
	// 最後はCheckBox
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;
	col = m_MeasurementPointGrid.GetColumnCount()-1;
	for(row = 1; row < m_MeasurementPointGrid.GetRowCount(); row++){
		m_MeasurementPointGrid.SetCellType(row, col, RUNTIME_CLASS(CGridCellCheck));

//		Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_STATE;
//		Item.row = row;
//		Item.col = col;
//		Item.nFormat = dwTextStyle;
//		Item.strText.Format("");
//		m_MeasurementPointGrid.SetItem(&Item);
	}

	// Cellの初期位置
	CCellID CellID;
	if( iRows > 0 ){
		CellID.col = 3;
		CellID.row = 1;
	}
	else{
		CellID.col = 3;
		CellID.row = 0;
	}
	m_CurrentCellID = CellID;

	return TRUE;
}

// ==========================================================================
//
void CSitePatternSetDlg::GridDisp(int row, int col, LPCTSTR pszText, DWORD TextStyle)
{
	GV_ITEM Item;
	DWORD dwTextStyle;

	Item.mask = GVIF_PARAM | GVIF_FORMAT;
	dwTextStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	dwTextStyle &= ~DT_LEFT;
	dwTextStyle |= TextStyle;

	Item.nFormat = dwTextStyle;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = row;
	Item.col = col;
	Item.nFormat = dwTextStyle;
	Item.strText.Format("%s", pszText);
	m_MeasurementPointGrid.SetItem(&Item);

	m_MeasurementPointGrid.Invalidate();
}

// ==========================================================================
//
void CSitePatternSetDlg::GridEditable(BOOL Editable/*=TRUE*/)
{
//	m_MeasurementPointGrid.SetEditable(Editable);
	m_MeasurementPointGrid.SetEditable(FALSE);
	m_MeasurementPointGrid.EnableSelection(Editable);
	m_MeasurementPointGrid.SetTrackFocusCell(FALSE);
	m_MeasurementPointGrid.SetFrameFocusCell(FALSE);
	m_MeasurementPointGrid.SetSingleRowSelection(Editable);
	m_MeasurementPointGrid.SetSingleColSelection(Editable);
	m_MeasurementPointGrid.SetFixedColumnSelection(Editable);
	m_MeasurementPointGrid.SetFixedRowSelection(Editable);
}

// ==========================================================================
//
void CSitePatternSetDlg::SetData()
{
	TCHAR szBuff[256];

	///// Point Coord /////
	// SAMPLE原点からの相対座標とする
	for ( int i = 0 ; i < m_iNumScans ; i++ ) {
		// X
		sprintf(szBuff, "%.3lf", ((double)m_ScanPoint[i].lX) / MICROMETRE );
		GridDisp(i + 1, 1, szBuff, DT_RIGHT);
		// Y
		sprintf(szBuff, "%.3lf", ((double)m_ScanPoint[i].lY) / MICROMETRE );
		GridDisp(i + 1, 2, szBuff, DT_RIGHT);
		// Pattern1
		if( m_SitePoint[i].PatternInfo[0].bDefined ){
			GridDisp(i + 1, 3, m_SitePoint[i].PatternInfo[0].szSitePatternName, DT_LEFT);
		}
		else{
			GridDisp(i + 1, 3, "", DT_LEFT);
		}
		// Pattern2
		if( m_SitePoint[i].PatternInfo[1].bDefined ){
			GridDisp(i + 1, 4, m_SitePoint[i].PatternInfo[1].szSitePatternName, DT_LEFT);
		}
		else{
			GridDisp(i + 1, 4, "", DT_LEFT);
		}
		// ENABLE
		((CGridCellCheck*) m_MeasurementPointGrid.GetCell(i + 1, 5))->SetCheck(m_SitePoint[i].bEnable);

	}
}

// ==========================================================================
//
void CSitePatternSetDlg::DispPattern()
{
	TCHAR	szFileName[MAX_PATH];
	TCHAR	szFilePath[MAX_PATH];
	CFileFind Finder;
	BOOL	bRet;
	CString szBuf;
	SYSTEMTIME lastWriteSystemTime;
	SPR_SUB_INFO m_pPatSubInfo;

	UpdateData();

	::DeleteFile(PATTERN_IMG_TEMP);
	::DeleteFile(PATTERN_IMG_1);
	::DeleteFile(PATTERN_IMG_2);

	m_stcSitePrImg[0].Invalidate(TRUE);
	m_stcSitePrImg[1].Invalidate(TRUE);

	::ZeroMemory(szFileName, sizeof(szFileName));
	::ZeroMemory(szFilePath, sizeof(szFilePath));

	m_szLens1 = "";
	m_szOffsetX1 = "";
	m_szOffsetY1 = "";
	m_szLens2 = "";
	m_szOffsetX2 = "";
	m_szOffsetY2 = "";

	///// Lens Magnification Combo /////
	SR_TURRET srTurret[SR_LENS_MAX];
	ConfigFile_GetNanoSpecIni(srTurret, CONFIG_FILE_SR_TURRET);

	// Pattern表示
	if( m_CurrentCellID.row > 0  && m_CurrentCellID.row <= m_iNumScans ){
		// Pattern1
		if( m_SitePoint[m_CurrentCellID.row-1].PatternInfo[0].bDefined ){
//			PatternFile_MakePatternImageFilePath(szFilePath, m_SitePoint[m_CurrentCellID.row-1].PatternInfo[0].szSitePatternName, m_SitePoint[m_CurrentCellID.row-1].PatternInfo[0].iLens);
			sprintf(szFileName, "%s", m_SitePoint[m_CurrentCellID.row-1].PatternInfo[0].szSitePatternName);
			sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Img_Dir, szFileName, PATTERN_IMAGE_EXT);
			bRet = PatternFile_ExistPatternFilePath(szFilePath, &lastWriteSystemTime);
			if( bRet ){
				// イメージを取得
				::CopyFile(szFilePath, PATTERN_IMG_1, FALSE);
				// オフセット情報を取得
				bRet = PatternFile_GetSubInfo(szFileName, &m_pPatSubInfo);
				if( bRet ){
					m_szOffsetX1.Format("%.3lf", (double)(m_pPatSubInfo.ptOffset.x)/MICROMETRE);
					m_szOffsetY1.Format("%.3lf", (double)(m_pPatSubInfo.ptOffset.y)/MICROMETRE);
					m_szLens1 = srTurret[m_pPatSubInfo.iLens].szName;
				}
			}
		}
		// Pattern2
		if( m_SitePoint[m_CurrentCellID.row-1].PatternInfo[1].bDefined ){
//			PatternFile_MakePatternImageFilePath(szFilePath, m_SitePoint[m_CurrentCellID.row-1].PatternInfo[1].szSitePatternName, m_SitePoint[m_CurrentCellID.row-1].PatternInfo[1].iLens);
			sprintf(szFileName, "%s", m_SitePoint[m_CurrentCellID.row-1].PatternInfo[1].szSitePatternName);
			sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Img_Dir, szFileName, PATTERN_IMAGE_EXT);
			bRet = PatternFile_ExistPatternFilePath(szFilePath, &lastWriteSystemTime);
			if( bRet ){
				// イメージを取得
				::CopyFile(szFilePath, PATTERN_IMG_2, FALSE);
				// オフセット情報を取得
				bRet = PatternFile_GetSubInfo(szFileName, &m_pPatSubInfo);
				if( bRet ){
					m_szOffsetX2.Format("%.3lf", (double)(m_pPatSubInfo.ptOffset.x)/MICROMETRE);
					m_szOffsetY2.Format("%.3lf", (double)(m_pPatSubInfo.ptOffset.y)/MICROMETRE);
					m_szLens2 = srTurret[m_pPatSubInfo.iLens].szName;
				}
			}
		}
	}

	m_stcSitePrImg[0].SetBitmap(PATTERN_IMG_1);
	m_stcSitePrImg[1].SetBitmap(PATTERN_IMG_2);

	// Cursor表示
	SetCursor(m_CurrentCellID);

	UpdateData(FALSE);
}

// =========================================================================
// GRID選択行を取得する
void CSitePatternSetDlg::OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	///// Draw SelectPoint /////
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID CellID;
	CellID.row=pItem->iRow;
	CellID.col=pItem->iColumn;

	if ( !m_MeasurementPointGrid.IsValid(CellID) )
		return;

	if( CellID.col == 5 ){
		// Enableの状態を反転させる
		m_SitePoint[CellID.row-1].bEnable ? m_SitePoint[CellID.row-1].bEnable = 0 : m_SitePoint[CellID.row-1].bEnable = 1;
	}

	// 現在の選択中セル
	m_CurrentCellID = CellID;

	// Pattern表示
	DispPattern();
}

// ==========================================================================
//
void CSitePatternSetDlg::OnOK()
{
	m_pDoc->OperationLogging("CSitePatternSetDlg::OK Button was push.");

	m_pStageProgInfoHdr->nSitePatternMode = m_SelectMode;
	m_pStageProgInfoHdr->nSitePrFailOption = m_FailMode;

	///// Point Coord /////
	for ( int i = 0 ; i < m_iNumScans ; i++ ) {
		m_SitePoint[i].bEnable = ((CGridCellCheck*) m_MeasurementPointGrid.GetCell(i + 1, 5))->GetCheck();
	}

	CDialog::OnOK();
}

// ==========================================================================
//
void CSitePatternSetDlg::OnCancel()
{
	m_pDoc->OperationLogging("CSitePatternSetDlg::Cancel Button was push.");

	CDialog::OnCancel();
}

// ==========================================================================
//
void CSitePatternSetDlg::OnSelchangeSitePrExecutionMode()
{
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_SITE_PR_EXECUTION_MODE);

	m_pDoc->OperationLogging("CSitePatternSetDlg::ExecutionMode ComboBox was select.");

	m_SelectMode = pCombo->GetCurSel();

	OperationEnable(m_SelectMode);
}

// ==========================================================================
//
void CSitePatternSetDlg::OnSelchangeSitePrFailOption()
{
	CComboBox* pFailCombo;
	pFailCombo = (CComboBox*)GetDlgItem(IDC_SITE_PR_FAIL_OPTION);

	m_pDoc->OperationLogging("CSitePatternSetDlg::FailOption ComboBox was select.");

	m_FailMode = pFailCombo->GetCurSel();
}

// ==========================================================================
// 設定１
void CSitePatternSetDlg::OnPattern1SetupButton()
{
	m_pDoc->OperationLogging("CSitePatternSetDlg::Setup1 Button was push.");

	PatternSetUp(0);
}

// ==========================================================================
// 設定２
void CSitePatternSetDlg::OnPattern2SetupButton()
{
	m_pDoc->OperationLogging("CSitePatternSetDlg::Setup2 Button was push.");

	PatternSetUp(1);
}

// ==========================================================================
// パターン設定
void CSitePatternSetDlg::PatternSetUp(int nSite)
{
	TCHAR	szFilePath[MAX_PATH];
	SYSTEMTIME lastWriteSystemTime;
	DESKEW_SITE selectPoint;
	int 	iLens = 0;
	CString strBuffer, strTitle;

// 2009.12.02 K.Matsuo SampleOrigin -->
	SAMPLEINFO sampleInfo;
	::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
	RecipeFile_LoadRecipe(&sampleInfo, m_pStageProgInfoHdr->SampleInfo.szName, RECIPE_FILE_SAMPLE);
// 2009.12.02 K.Matsuo SampleOrigin <--

	::ZeroMemory(&selectPoint, sizeof(selectPoint));
	::DeleteFile(DESKEW_IMG_TEMP);

	if( m_SitePoint[m_CurrentCellID.row-1].PatternInfo[nSite].bDefined ){
		selectPoint.bDefined = TRUE;
// 2009.12.02 K.Matsuo SampleOrigin -->
//		selectPoint.Loc.lX = (m_ScanPoint[m_CurrentCellID.row - 1].lX) + (m_pStageProgInfoHdr->SampleInfo.Origin.lX);
//		selectPoint.Loc.lY = (m_ScanPoint[m_CurrentCellID.row - 1].lY) + (m_pStageProgInfoHdr->SampleInfo.Origin.lY);
		selectPoint.Loc.lX = (m_ScanPoint[m_CurrentCellID.row - 1].lX) + (sampleInfo.Origin.lX);
		selectPoint.Loc.lY = (m_ScanPoint[m_CurrentCellID.row - 1].lY) + (sampleInfo.Origin.lY);
// 2009.12.02 K.Matsuo SampleOrigin <--
		iLens = m_SitePoint[m_CurrentCellID.row-1].PatternInfo[nSite].iLens;

//		PatternFile_MakePatternImageFilePath(szFilePath,
//											m_SitePoint[m_CurrentCellID.row-1].PatternInfo[nSite].szSitePatternName,
//											m_SitePoint[m_CurrentCellID.row-1].PatternInfo[nSite].iLens);
		sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Img_Dir, m_SitePoint[m_CurrentCellID.row-1].PatternInfo[nSite].szSitePatternName, PATTERN_IMAGE_EXT);

		if( PatternFile_ExistPatternFilePath(szFilePath, &lastWriteSystemTime) ){
			// Pattern有り
			::CopyFile(szFilePath, DESKEW_IMG_TEMP, FALSE);
		}
	}
	else{
		LoadStringML(IDS_GO_TO_POINT, strBuffer, "Would you move to the Scan Point?");
		LoadStringML(IDS_MOVE_CONFIRM, strTitle, "MOVE CONFIRM");
		if ( MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL) == IDOK ){
			selectPoint.bDefined = TRUE;
// 2009.12.02 K.Matsuo SampleOrigin -->
//			selectPoint.Loc.lX = (m_ScanPoint[m_CurrentCellID.row - 1].lX) + (m_pStageProgInfoHdr->SampleInfo.Origin.lX);
//			selectPoint.Loc.lY = (m_ScanPoint[m_CurrentCellID.row - 1].lY) + (m_pStageProgInfoHdr->SampleInfo.Origin.lY);
			selectPoint.Loc.lX = (m_ScanPoint[m_CurrentCellID.row - 1].lX) + (sampleInfo.Origin.lX);
			selectPoint.Loc.lY = (m_ScanPoint[m_CurrentCellID.row - 1].lY) + (sampleInfo.Origin.lY);
// 2009.12.02 K.Matsuo SampleOrigin <--
		}
	}

	CPatRecDeskewDlg dlg(SITEPR_MODE, iLens, &selectPoint);
	//Sampleセット
	dlg.SetFileName(m_pStageProgInfoHdr->SampleInfo.szName);
	dlg.SetSampleMode(1);

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(m_pStageProgInfoHdr->SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	if ( dlg.DoModal() == IDOK ) {

		CSaveAsDlg SaveDlg(SAVE_AS_SITE_PATTERN_IMG);

		SPR_SUB_INFO m_pPatSubInfo;

		// レンズ倍率を引き渡す
		SaveDlg.m_iLens = dlg.m_iLens;
		m_pPatSubInfo.iLens = dlg.m_iLens;

		// オフセットを引き渡す
		m_pPatSubInfo.ptOffset.x = dlg.m_OffsetPos.lX;
		m_pPatSubInfo.ptOffset.y = dlg.m_OffsetPos.lY;

		if ( SaveDlg.DoModal() == IDOK ) {
			// ファイルの保存
			::CopyFile(DESKEW_IMG_TEMP, SaveDlg.m_szFilePath, FALSE);

			// オフセットの保存
			PatternFile_SetSubInfo(SaveDlg.m_strName, &m_pPatSubInfo);

			m_SitePoint[m_CurrentCellID.row-1].PatternInfo[nSite].bDefined = TRUE;
			strcpy(m_SitePoint[m_CurrentCellID.row-1].PatternInfo[nSite].szSitePatternName, SaveDlg.m_strName);
			m_SitePoint[m_CurrentCellID.row-1].PatternInfo[nSite].iLens = SaveDlg.m_iLens;

			SetData();
			DispPattern();
		}
	}
}

// ==========================================================================
// パターン変更
void CSitePatternSetDlg::OnPatternChgButton()
{
	int m_bSeachFlg;
	//Saiki 20090601 Add ----->
	CString strBuffer, strTitle;
	LoadStringML(IDS_TITLE_CHANGE_PATTERN, strTitle, "SET PATTERN");
	//Saiki 20090601 Add <-----

	m_pDoc->OperationLogging("CSitePatternSetDlg::Change Button was push.");

	m_bSeachFlg = CheckSelectItem();

	if( m_bSeachFlg==PATTERN_NO_SELECT ){
		//Saiki 20090601 Change ----->
		//MessageBox(	"変更するパターンを選択して下さい",
		//			"パターン変更",
		//			MB_OK);
		LoadStringML(IDS_SELECT_PATTERN_CHANGE, strBuffer, "Please Select the Pattern to Set");
		MessageBox( strBuffer, strTitle, MB_OK);
		//Saiki 20090601 Change <-----
		return;
	}

	CSitePatternEditDlg dlg;
	g_pSitePatternEditDlg = &dlg;
	dlg.m_iMode = SITEPATTERN_MODE_EDIT;
	if( dlg.DoModal() == IDOK ){
		if( m_bSeachFlg==PATTERN_MODIFY ){
/* modified 2009.07.01 hmenjo パターン変更ボタンのバグ修正 ---------- { ---------- */
/*				//Saiki 20090601 Change ----->
//				//if(IDOK == MessageBox(	"選択されているパターンを変更してもよろしいですか？",
//				//						"パターン変更",
//				//						MB_ICONQUESTION | MB_OKCANCEL)){
//				LoadStringML(IDS_CHANGE_SELECTED_PAT, strBuffer, "Would you Set the Selected Pattern ?");
//				if(IDOK == MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)){
//				//Saiki 20090601 Change <-----
*/
/* modified 2009.07.01 hmenjo パターン変更ボタンのバグ修正 ----------			   */
			LoadStringML(IDS_CHANGE_SELECTED_PAT, strBuffer, "Would you Set the Selected Pattern ?");
			if (IDOK != MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)) {
/* modified 2009.07.01 hmenjo パターン変更ボタンのバグ修正 ---------- } ---------- */
				return;
			}
		}
		for ( int i = 0 ; i < m_iNumScans ; i++ ) {
			if( m_MeasurementPointGrid.GetItemState(i+1, 3) & GVIS_SELECTED ){
				// Pattern1
				m_SitePoint[i].PatternInfo[0].bDefined = TRUE;
				strcpy(m_SitePoint[i].PatternInfo[0].szSitePatternName, dlg.m_SelectName);
//				m_SitePoint[i].PatternInfo[0].iLens = dlg.m_SelectLens;
				m_SitePoint[i].PatternInfo[0].iLens = dlg.m_pPatSubInfo.iLens;
			}
			if( m_MeasurementPointGrid.GetItemState(i+1, 4) & GVIS_SELECTED ){
				// Pattern2
				m_SitePoint[i].PatternInfo[1].bDefined = TRUE;
				strcpy(m_SitePoint[i].PatternInfo[1].szSitePatternName, dlg.m_SelectName);
//				m_SitePoint[i].PatternInfo[1].iLens = dlg.m_SelectLens;
				m_SitePoint[i].PatternInfo[1].iLens = dlg.m_pPatSubInfo.iLens;
			}
		}
		SetData();
		DispPattern();
	}
}

// ==========================================================================
// パターン一括変更
void CSitePatternSetDlg::OnPatternAllChgButton()
{
	int i;
	//Saiki 20090601 Add ----->
	CString strBuffer, strTitle;
	LoadStringML(IDS_TITLE_CHANGE_PAT_COLLECTIVE, strTitle, "SET PATTERN COLLECTIVELY");
	//Saiki 20090601 Add <-----

	m_pDoc->OperationLogging("CSitePatternSetDlg::All Change Button was push.");

	//Saiki 20090609 Change ----->
	//if ( m_CurrentCellID.row <= 0 || (m_CurrentCellID.col!=3 && m_CurrentCellID.col!=4) ||
	//	  (m_CurrentCellID.col==3 && !m_SitePoint[m_CurrentCellID.row-1].PatternInfo[0].bDefined) ||
	//	  (m_CurrentCellID.col==4 && !m_SitePoint[m_CurrentCellID.row-1].PatternInfo[1].bDefined)){
	if ( m_CurrentCellID.row <= 0 || (m_CurrentCellID.col!=3 && m_CurrentCellID.col!=4)){
	//Saiki 20090609 Change <-----
		//Saiki 20090601 Change ----->
		//MessageBox(	"一括変更する変更元パターンを選択して下さい",
		//			"パターン一括変更",
		//			MB_OK);
		LoadStringML(IDS_SELECT_PATTERN_CHANGE_COLLECTIVE, strBuffer, "Please Select the Pattern to Set Collectively");
		MessageBox(strBuffer, strTitle, MB_OK);
		//Saiki 20090601 Change <-----
		return;
	}

	CSitePatternEditDlg dlg;
	g_pSitePatternEditDlg = &dlg;
	dlg.m_iMode = SITEPATTERN_MODE_EDIT;
	if( dlg.DoModal() == IDOK ){

		switch(m_CurrentCellID.col){
		case 3:
		//Saiki 20090601 Change ----->
		//if(IDOK == MessageBox(	"選択されているパターンで\nパターン１を一括変更してもよろしいですか？",
		//						"パターン一括変更",
		//						MB_ICONQUESTION | MB_OKCANCEL)){
		LoadStringML(IDS_CHANGE_PAT_COLLECTIVE_SELECT_PAT1, strBuffer, "Would you Set the Pattern1 in a Pattern that is Selected ?");
		if(IDOK == MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)){
		//Saiki 20090601 Change <-----
				for ( i = 0 ; i < m_iNumScans ; i++ ) {
					// Pattern1
					m_SitePoint[i].PatternInfo[0].bDefined = TRUE;
					strcpy(m_SitePoint[i].PatternInfo[0].szSitePatternName, dlg.m_SelectName);
//					m_SitePoint[i].PatternInfo[0].iLens = dlg.m_SelectLens;
					m_SitePoint[i].PatternInfo[0].iLens = dlg.m_pPatSubInfo.iLens;
				}
				SetData();
				DispPattern();
			}
			break;
		case 4:
		//Saiki 20090601 Change ----->
		//if(IDOK == MessageBox(	"選択されているパターンで\nパターン２を一括変更してもよろしいですか？",
		//						"パターン一括変更",
		//						MB_ICONQUESTION | MB_OKCANCEL)){
		LoadStringML(IDS_CHANGE_PAT_COLLECTIVE_SELECT_PAT2, strBuffer, "Would you Set the Pattern2 in a Pattern that is Selected ?");
		if(IDOK == MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)){
		//Saiki 20090601 Change <-----
				for ( i = 0 ; i < m_iNumScans ; i++ ) {
					// Pattern2
					m_SitePoint[i].PatternInfo[1].bDefined = TRUE;
					strcpy(m_SitePoint[i].PatternInfo[1].szSitePatternName, dlg.m_SelectName);
//					m_SitePoint[i].PatternInfo[1].iLens = dlg.m_SelectLens;
					m_SitePoint[i].PatternInfo[1].iLens = dlg.m_pPatSubInfo.iLens;
				}
				SetData();
				DispPattern();
			}
			break;
		}
	}
}

// ==========================================================================
// パターン削除
void CSitePatternSetDlg::OnPatternDelButton()
{
	//Saiki 20090601 Add ----->
	CString strBuffer, strTitle;
//	LoadStringML(IDS_TITLE_DELETE_PATTERN, strTitle, "DELETE PATTERN");
	LoadStringML(IDS_TITLE_DELETE_PATTERN, strTitle, "CLEAR PATTERN");
	//Saiki 20090601 Add <-----

	m_pDoc->OperationLogging("CSitePatternSetDlg::Delete Button was push.");

	if( m_CurrentCellID.row <= 0 || (m_CurrentCellID.col!=3 && m_CurrentCellID.col!=4) ){
		//Saiki 20090601 Change ----->
		//MessageBox(	"削除するパターンを選択して下さい",
		//			"パターン削除",
		//			MB_OK);
		LoadStringML(IDS_SELECT_PATTERN_DELETE, strBuffer, "Please Select the Pattern to Clear");
		MessageBox( strBuffer, strTitle, MB_OK);
		//Saiki 20090601 Change <-----
		return;
	}

	//Saiki 20090601 Change ----->
	//if(IDOK == MessageBox(	"選択されているパターンを削除してもよろしいですか？",
	//						"パターン削除",
	//						MB_ICONQUESTION | MB_OKCANCEL)){
	LoadStringML(IDS_DELETE_SELECTED_PAT, strBuffer, "Would you Clear the Selected Pattern ?");
	if(IDOK == MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)){
	//Saiki 20090601 Change <-----
		switch(m_CurrentCellID.col){
		case 3:
			// Pattern1
			::ZeroMemory(&m_SitePoint[m_CurrentCellID.row-1].PatternInfo[0], sizeof(m_SitePoint[m_CurrentCellID.row-1].PatternInfo[0]));
			break;
		case 4:
			// Pattern2
			::ZeroMemory(&m_SitePoint[m_CurrentCellID.row-1].PatternInfo[1], sizeof(m_SitePoint[m_CurrentCellID.row-1].PatternInfo[1]));
			break;
		}

		SetData();
		DispPattern();
	}
}

// ==========================================================================
// パターン一括削除
void CSitePatternSetDlg::OnPatternAllDelButton()
{
	int i;
	//Saiki 20090601 Add ----->
	CString strBuffer, strTitle;
//	LoadStringML(IDS_TITLE_DELETE_PAT_COLLECTIVE, strTitle, "DELETE PATTERN COLLECTIVELY");
	LoadStringML(IDS_TITLE_DELETE_PAT_COLLECTIVE, strTitle, "CLEAR PATTERN COLLECTIVELY");
	//Saiki 20090601 Add <-----

	m_pDoc->OperationLogging("CSitePatternSetDlg::All Delete Button was push.");

	if( m_CurrentCellID.row <= 0 || (m_CurrentCellID.col!=3 && m_CurrentCellID.col!=4) ){
		//Saiki 20090601 Change ----->
		//MessageBox(	"削除するパターンの列を選択して下さい",
		//			"パターン一括削除",
		//			MB_OK);
		LoadStringML(IDS_SELECT_PATTERN_DELETE_COLLECTIVE, strBuffer, "Please Select the Pattern to Clear Column");
		MessageBox(strBuffer, strTitle, MB_OK);
		//Saiki 20090601 Change <-----
		return;
	}

	switch(m_CurrentCellID.col){
	case 3:
		//Saiki 20090601 Change ----->
		//if(IDOK == MessageBox(	"パターン１を一括削除してもよろしいですか？",
		//						"パターン一括削除",
		//						MB_ICONQUESTION | MB_OKCANCEL)){
		LoadStringML(IDS_DELETE_PAT_COLLECTIVE_SELECT_PAT1, strBuffer, "Would you Clear the Pattern1 in a Pattern that is Selected ?");
		if(IDOK == MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)){
		//Saiki 20090601 Change <-----
			for ( i = 0 ; i < m_iNumScans ; i++ ) {
				// Pattern1
				::ZeroMemory(&m_SitePoint[i].PatternInfo[0], sizeof(m_SitePoint[i].PatternInfo[0]));
			}
			SetData();
			DispPattern();
		}
		break;
	case 4:
		//Saiki 20090601 Change ----->
		//if(IDOK == MessageBox(	"パターン２を一括削除してもよろしいですか？",
		//						"パターン一括削除",
		//						MB_ICONQUESTION | MB_OKCANCEL)){
		LoadStringML(IDS_DELETE_PAT_COLLECTIVE_SELECT_PAT2, strBuffer, "Would you Clear the Pattern2 in a Pattern that is Selected ?");
		if(IDOK == MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)){
		//Saiki 20090601 Change <-----
			for ( i = 0 ; i < m_iNumScans ; i++ ) {
				// Pattern1
				::ZeroMemory(&m_SitePoint[i].PatternInfo[1], sizeof(m_SitePoint[i].PatternInfo[1]));
			}
			SetData();
			DispPattern();
		}
		break;
	}
}

// ==========================================================================
// 有効一括ON
void CSitePatternSetDlg::OnAllEnableButton()
{
	//Saiki 20090601 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090601 Add <-----

	m_pDoc->OperationLogging("CSitePatternSetDlg::All Enable Button was push.");

	//Saiki 20090601 Change ----->
	//if(IDOK == MessageBox(	"有効フラグを一括でONしてもよろしいですか？",
	//						"有効一括ON",
	//						MB_ICONQUESTION | MB_OKCANCEL)){
	LoadStringML(IDS_BATCH_FLAG_ON, strBuffer, "Would you do Once All the Active Flag ON?");
	LoadStringML(IDS_EFFECTIVE_BATCH_ON, strTitle, "Effective Batch ON");
	if(IDOK == MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)){
	//Saiki 20090601 Change <-----
		for ( int i = 0 ; i < m_iNumScans ; i++ ) {
			// ENABLE
			m_SitePoint[i].bEnable = 1;
		}

		SetData();
		DispPattern();
	}
}

// ==========================================================================
// 有効一括OFF
void CSitePatternSetDlg::OnAllDisableButton()
{
	//Saiki 20090601 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090601 Add <-----

	m_pDoc->OperationLogging("CSitePatternSetDlg::All Disable Button was push.");

	//Saiki 20090601 Change ----->
	//if(IDOK == MessageBox(	"有効フラグを一括でOFFしてもよろしいですか？",
	//						"有効一括OFF",
	//						MB_ICONQUESTION | MB_OKCANCEL)){
	LoadStringML(IDS_BATCH_FLAG_OFF, strBuffer, "Would you do Once All the Active Flag OFF?");
	LoadStringML(IDS_EFFECTIVE_BATCH_OFF, strTitle, "Effective Batch OFF");
	if(IDOK == MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL)){
	//Saiki 20090601 Change <-----
		for ( int i = 0 ; i < m_iNumScans ; i++ ) {
			// ENABLE
			m_SitePoint[i].bEnable = 0;
		}

		SetData();
		DispPattern();
	}
}

// ==========================================================================
//
void CSitePatternSetDlg::OperationEnable(BOOL bFlg)
{
	CButton* pBtn;

	// 設定１
	pBtn = (CButton*)GetDlgItem(IDC_PATTERN1_SETUP_BUTTON);
	pBtn->EnableWindow(bFlg);
	// 設定２
	pBtn = (CButton*)GetDlgItem(IDC_PATTERN2_SETUP_BUTTON);
	pBtn->EnableWindow(bFlg);
	// パターン変更
	pBtn = (CButton*)GetDlgItem(IDC_PATTERN_CHG_BUTTON);
	pBtn->EnableWindow(bFlg);
	// パターン一括変更
	pBtn = (CButton*)GetDlgItem(IDC_PATTERN_ALL_CHG_BUTTON);
	pBtn->EnableWindow(bFlg);
	// パターン削除
	pBtn = (CButton*)GetDlgItem(IDC_PATTERN_DEL_BUTTON);
	pBtn->EnableWindow(bFlg);
	// パターン一括削除
	pBtn = (CButton*)GetDlgItem(IDC_PATTERN_ALL_DEL_BUTTON);
	pBtn->EnableWindow(bFlg);
	// 有効一括ON
	pBtn = (CButton*)GetDlgItem(IDC_ALL_ENABLE_BUTTON);
	pBtn->EnableWindow(bFlg);
	// 有効一括OFF
	pBtn = (CButton*)GetDlgItem(IDC_ALL_DISABLE_BUTTON);
	pBtn->EnableWindow(bFlg);
	// 失敗時
	pBtn = (CButton*)GetDlgItem(IDC_SITE_PR_FAIL_OPTION);
	pBtn->EnableWindow(bFlg);
	// グリッド
	m_MeasurementPointGrid.EnableWindow(bFlg);
}

// ==========================================================================
//
void CSitePatternSetDlg::SetCursor(CCellID cell)
{
	if( cell.row > 0 && (cell.col==3 || cell.col==4)){
		m_MeasurementPointGrid.SetItemState(cell.row, cell.col, GVIS_SELECTED);
	}
}

// ==========================================================================
// 選択されているセルがあるか確認する
int CSitePatternSetDlg::CheckSelectItem()
{
	BOOL m_bSeachFlg = FALSE;

	for ( int i = 0 ; i < m_iNumScans ; i++ ) {
		if( m_MeasurementPointGrid.GetItemState(i+1, 3) & GVIS_SELECTED ){
			m_bSeachFlg = TRUE;
			if( m_SitePoint[i].PatternInfo[0].bDefined )	return PATTERN_MODIFY;
		}
		if( m_MeasurementPointGrid.GetItemState(i+1, 4) & GVIS_SELECTED ){
			m_bSeachFlg = TRUE;
			if( m_SitePoint[i].PatternInfo[1].bDefined )	return PATTERN_MODIFY;
		}
	}

	return m_bSeachFlg == TRUE ? PATTERN_ALL_NEW : PATTERN_NO_SELECT ;
}
