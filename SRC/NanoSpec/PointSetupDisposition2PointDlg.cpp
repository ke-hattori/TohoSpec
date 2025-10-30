// PointSetupDisposition2PointDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <math.h>

#include "System.h"
#include "NanoSpec.h"
#include "PointSetupDisposition2PointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPointSetupDisposition2PointDlg ダイアログ


CPointSetupDisposition2PointDlg::CPointSetupDisposition2PointDlg(int iNumScans, double dSampleSizeX, double dSampleSizeY, int nOrgPos,CWnd* pParent /*=NULL*/)
	: CDialog(CPointSetupDisposition2PointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointSetupDisposition2PointDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_lStartPointX1 = m_lStartPointY1 =0;
	m_lPitchX1 = m_lPitchY1 = 0;
	m_lStartPointX2 = m_lStartPointY2 =0;
	m_lPitchX2 = m_lPitchY2 = 0;
	m_iCountX = m_iCountY = 0;
	m_dSampleSizeX = dSampleSizeX;
	m_dSampleSizeY = dSampleSizeY;
	m_nOrgPos = nOrgPos;
	m_iNumScans = iNumScans;
}


void CPointSetupDisposition2PointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointSetupDisposition2PointDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_DISPOSITON_GRID, m_DispositionGrid);
	DDX_Control(pDX, IDC_DISPOSITON2_GRID, m_DispositionGrid2);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CPointSetupDisposition2PointDlg, CDialog)
	//{{AFX_MSG_MAP(CPointSetupDisposition2PointDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointSetupDisposition2PointDlg メッセージ ハンドラ

BOOL CPointSetupDisposition2PointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox* pCombo;

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// Disposition Grid /////
	DispositionGrid_Init();
	GridEditable(&m_DispositionGrid, TRUE);

	DispositionGrid2_Init();
	GridEditable(&m_DispositionGrid2, TRUE);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Draw Shaped /////
	pCombo = (CComboBox*)GetDlgItem(IDC_DRAW_SHAPED);
	for(int i = 0 ; i < STAGE_DRAW_SHAPED_MAX; i++){
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(STAGE_DRAW_SHAPED_ITEM_ENU[i]);
		}
		else{
			pCombo->AddString(STAGE_DRAW_SHAPED_ITEM_JPN[i]);
		}
	}
	pCombo->SetCurSel(0);

	///// SetCur Cell //////
	m_DispositionGrid.SetItemState(1, 1, GVIS_SELECTED);
	m_DispositionGrid.SetFocusCell(1, 1);
	m_DispositionGrid2.SetItemState(1, 1, GVIS_SELECTED);
	m_DispositionGrid2.SetFocusCell(1, 1);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CPointSetupDisposition2PointDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DRAW_SHAPED);

	if(!CheckData()){
		return;
	}

	m_iDrawShaped = pCombo->GetCurSel();
	CDialog::OnOK();
}

// =========================================================================
//
void CPointSetupDisposition2PointDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

// =========================================================================
//
void CPointSetupDisposition2PointDlg::GridDisp(CGridCtrl* pGrid, int row, int col, char* text, DWORD TextStyle)
{
	GV_ITEM Item;
	DWORD dwTextStyle;

	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	dwTextStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	//dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	dwTextStyle &= ~DT_LEFT;
	dwTextStyle |= TextStyle;

	Item.nFormat = dwTextStyle;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = row;
	Item.col = col;
	Item.nFormat = dwTextStyle;
	Item.strText.Format("%s", text);
	pGrid->SetItem(&Item);

	pGrid->Invalidate();
}

// =========================================================================
//
void CPointSetupDisposition2PointDlg::GridEditable(CGridCtrl* pGrid, BOOL Editable)
{
	pGrid->SetEditable(Editable);
	pGrid->EnableSelection(Editable);
	pGrid->SetTrackFocusCell(FALSE);
	pGrid->SetFrameFocusCell(FALSE);
	pGrid->SetSingleRowSelection(Editable);
	pGrid->SetSingleColSelection(Editable);
}

// =========================================================================
//
BOOL CPointSetupDisposition2PointDlg::DispositionGrid_Init()
{
	///// Grid Initialize /////
	GV_ITEM Item;
	int Cols = 4;
	int Rows = 2+1;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	char* ColItem_ENU[]={
		"Coord",
		"Start (mm)",
		"Pitch (mm)",
		"Count",
	};
	char* ColItem_JPN[]={
		"軸",
		"開始位置 (mm)",
		"間隔 (mm)",
		"ポイント数",
	};

	char* RowItem[]={
		"X1",
		"Y1",
	};

	///// Grid Initialize /////
	m_DispositionGrid.SetListMode(FALSE);
	m_DispositionGrid.EnableDragAndDrop(FALSE);
	m_DispositionGrid.SetTextBkColor(WHITE_COLOR);
	m_DispositionGrid.SetRowResize(0);

	m_DispositionGrid.SetEditable(FALSE);			   // <--- When Edit is TRUE
	m_DispositionGrid.EnableSelection(FALSE);		   // <--- When Edit is TRUE
	m_DispositionGrid.SetTrackFocusCell(FALSE); 	   // <--- When Edit is TRUE
	m_DispositionGrid.SetFrameFocusCell(FALSE); 	   // <--- When Edit is TRUE

	TRY {
		m_DispositionGrid.SetRowCount(Rows);
		m_DispositionGrid.SetColumnCount(Cols);
		m_DispositionGrid.SetFixedRowCount(FixRows);
		m_DispositionGrid.SetFixedColumnCount(FixCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	END_CATCH

	// Make cell 1,1 read-only
	//m_DispositionGrid.SetItemState(1,1, m_DispositionGrid.GetItemState(1,1) | GVIS_READONLY);

	//BOOL TitleTips = TRUE;
	//m_DispositionGrid.EnableTitleTips(TitleTips);

	///// Grid Size Initialize /////
	m_DispositionGrid.SetRowHeight(0, 20);
	m_DispositionGrid.SetColumnWidth(0, 60);

	for (row = 1; row < m_DispositionGrid.GetRowCount(); row++){
		m_DispositionGrid.SetRowHeight(row, 18);
	}

	m_DispositionGrid.SetColumnWidth(1, 90);
	m_DispositionGrid.SetColumnWidth(2, 90);
	m_DispositionGrid.SetColumnWidth(3, 90);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);

	for (row = 0; row < m_DispositionGrid.GetRowCount(); row++){
		for (col = 0; col < m_DispositionGrid.GetColumnCount(); col++){
			m_DispositionGrid.SetItemFont(row,col,&LogFont);
		}
	}

	///// Title Column /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_DispositionGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		if(l_SystemConfig.nLanguage == 0){
			Item.strText.Format("%s",ColItem_ENU[col]);
		}
		else{
			Item.strText.Format("%s",ColItem_JPN[col]);
		}
		m_DispositionGrid.SetItem(&Item);
	}

	///// Title Row /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_DispositionGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s", RowItem[row - 1]);
		m_DispositionGrid.SetItem(&Item);
	}

	///// Grid Alignment /////
	dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_DispositionGrid.GetRowCount(); row++){
		for(col = 1; col < m_DispositionGrid.GetColumnCount(); col++){
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("");
			m_DispositionGrid.SetItem(&Item);
		}
	}

	return TRUE;
}

// =========================================================================
//
BOOL CPointSetupDisposition2PointDlg::DispositionGrid2_Init()
{
	///// Grid Initialize /////
	GV_ITEM Item;
	int Cols = 3;
	int Rows = 2+1;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	char* ColItem_ENU[]={
		"Coord",
		"Start (mm)",
		"Pitch (mm)",
	};
	char* ColItem_JPN[]={
		"軸",
		"開始位置 (mm)",
		"間隔 (mm)",
	};

	char* RowItem[]={
		"X2",
		"Y2",
	};

	///// Grid Initialize /////
	m_DispositionGrid2.SetListMode(FALSE);
	m_DispositionGrid2.EnableDragAndDrop(FALSE);
	m_DispositionGrid2.SetTextBkColor(WHITE_COLOR);
	m_DispositionGrid2.SetRowResize(0);

	m_DispositionGrid2.SetEditable(FALSE);				// <--- When Edit is TRUE
	m_DispositionGrid2.EnableSelection(FALSE);			// <--- When Edit is TRUE
	m_DispositionGrid2.SetTrackFocusCell(FALSE);		// <--- When Edit is TRUE
	m_DispositionGrid2.SetFrameFocusCell(FALSE);		// <--- When Edit is TRUE

	TRY {
		m_DispositionGrid2.SetRowCount(Rows);
		m_DispositionGrid2.SetColumnCount(Cols);
		m_DispositionGrid2.SetFixedRowCount(FixRows);
		m_DispositionGrid2.SetFixedColumnCount(FixCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	END_CATCH

	// Make cell 1,1 read-only
	//m_DispositionGrid2.SetItemState(1,1, m_DispositionGrid2.GetItemState(1,1) | GVIS_READONLY);

	//BOOL TitleTips = TRUE;
	//m_DispositionGrid2.EnableTitleTips(TitleTips);

	///// Grid Size Initialize /////
	m_DispositionGrid2.SetRowHeight(0, 20);
	m_DispositionGrid2.SetColumnWidth(0, 60);

	for (row = 1; row < m_DispositionGrid2.GetRowCount(); row++){
		m_DispositionGrid2.SetRowHeight(row, 18);
	}

	m_DispositionGrid2.SetColumnWidth(1, 90);
	m_DispositionGrid2.SetColumnWidth(2, 90);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);

	for (row = 0; row < m_DispositionGrid2.GetRowCount(); row++){
		for (col = 0; col < m_DispositionGrid2.GetColumnCount(); col++){
			m_DispositionGrid2.SetItemFont(row,col,&LogFont);
		}
	}

	///// Title Column /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_DispositionGrid2.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		if(l_SystemConfig.nLanguage == 0){
			Item.strText.Format("%s",ColItem_ENU[col]);
		}
		else{
			Item.strText.Format("%s",ColItem_JPN[col]);
		}
		m_DispositionGrid2.SetItem(&Item);
	}

	///// Title Row /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_DispositionGrid2.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s", RowItem[row - 1]);
		m_DispositionGrid2.SetItem(&Item);
	}

	///// Grid Alignment /////
	dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_DispositionGrid2.GetRowCount(); row++){
		for(col = 1; col < m_DispositionGrid2.GetColumnCount(); col++){
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("");
			m_DispositionGrid2.SetItem(&Item);
		}
	}

	return TRUE;
}

// =========================================================================
//
BOOL CPointSetupDisposition2PointDlg::CheckData()
{
	CComboBox* pCombo;
	int iMin = 1, iMax = SCAN_POINT_MAX;
	long lMax;
	CString strBuff;
	CString strBuffSub;
	CString csBuff;
	char szBuff[256];
	double dValueBuff;

	CString l_strBuffer, l_strTitle;

	///// Draw Shaped /////
	pCombo = (CComboBox*)GetDlgItem(IDC_DRAW_SHAPED);
	if(pCombo->GetCurSel() == CB_ERR){
		LoadStringML(IDS_DRAW_SHAPED_EMPTY, l_strBuffer, "Draw Shaped is empty.");
		LoadStringML(IDS_TITLE_POINT_SETUP, l_strTitle, "POINT SETUP");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// グリッドの状態を取得
	// X軸の始点が「空」の時は抜ける
	strBuff = m_DispositionGrid.GetItemText(1, 1);
	if (strlen(strBuff)==0){
		return FALSE;
	}
	// Y軸の始点が「空」の時は抜ける
	strBuff = m_DispositionGrid.GetItemText(2, 1);
	if (strlen(strBuff)==0){
		return FALSE;
	}
	// X軸の間隔が「空」の時は抜ける
	strBuff = m_DispositionGrid.GetItemText(1, 2);
	if (strlen(strBuff)==0) {
		return FALSE;
	}
	// Y軸の間隔が「空」の時は抜ける
	strBuff = m_DispositionGrid.GetItemText(2, 2);
	if (strlen(strBuff)==0){
		return FALSE;
	}
	// X軸のポイント数が「空」または「0以下」の時は抜ける
	strBuff = m_DispositionGrid.GetItemText(1, 3);
	if ((strlen(strBuff)==0) || (atoi(strBuff) <=0)){
		return FALSE;
	}
	// Y軸のポイント数が四角配置で「空」または「0以下」の時は抜ける
	strBuff = m_DispositionGrid.GetItemText(2, 3);
	if (pCombo->GetCurSel() == STAGE_DRAW_SHAPED_RECTANGLE){
		if ((strlen(strBuff)==0)  || (atoi(strBuff) <=0)) {
			return FALSE;
		}
	}

	// X軸の始点が「空」の時は抜ける
	strBuff = m_DispositionGrid2.GetItemText(1, 1);
	if (strlen(strBuff)==0){
		return FALSE;
	}
	// Y軸の始点が「空」の時は抜ける
	strBuff = m_DispositionGrid2.GetItemText(2, 1);
	if (strlen(strBuff)==0){
		return FALSE;
	}
	// X軸の間隔が「空」の時は抜ける
	strBuff = m_DispositionGrid2.GetItemText(1, 2);
	if (strlen(strBuff)==0) {
		return FALSE;
	}
	// Y軸の間隔が「空」の時は抜ける
	strBuff = m_DispositionGrid2.GetItemText(2, 2);
	if (strlen(strBuff)==0){
		return FALSE;
	}
#if 0
	// X軸のポイント数が「空」または「0以下」の時は抜ける
	strBuff = m_DispositionGrid2.GetItemText(1, 3);
	if ((strlen(strBuff)==0) || (atoi(strBuff) <=0)){
		return FALSE;
	}
	// Y軸のポイント数が四角配置で「空」または「0以下」の時は抜ける
	strBuff = m_DispositionGrid2.GetItemText(2, 3);
	if (pCombo->GetCurSel() == STAGE_DRAW_SHAPED_RECTANGLE){
		if ((strlen(strBuff)==0)  || (atoi(strBuff) <=0)) {
			return FALSE;
		}
	}
#endif

	// StartPoint X1
	dValueBuff = atof(m_DispositionGrid.GetItemText(1, 1)) * MICROMETRE ;
	//原点位置対応
	if(!IsPosInStageSizeX(dValueBuff)){
		LoadStringML(IDS_OUT_SIZE_LOCATION_X, l_strBuffer, "Out of the Size Location of X.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//原点位置対応
		long lMin,lMax;
		GetPosRangeX(&lMin,&lMax);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	m_lStartPointX1 =(long)dValueBuff;

	// StartPoint Y1
	dValueBuff = atof(m_DispositionGrid.GetItemText(2, 1)) * MICROMETRE ;
	//原点位置対応
	if(!IsPosInStageSizeY(dValueBuff)){
		LoadStringML(IDS_OUT_SIZE_LOCATION_Y, l_strBuffer, "Out of the Size Location of Y.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//原点位置対応
		long lMin,lMax;
		GetPosRangeY(&lMin,&lMax);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	m_lStartPointY1 =(long)dValueBuff;

	// StartPoint X2
	dValueBuff = atof(m_DispositionGrid2.GetItemText(1, 1)) * MICROMETRE ;
	//原点位置対応
	if(!IsPosInStageSizeX(dValueBuff)){
		LoadStringML(IDS_OUT_SIZE_LOCATION_X, l_strBuffer, "Out of the Size Location of X.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//原点位置対応
		long lMin,lMax;
		GetPosRangeX(&lMin,&lMax);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	m_lStartPointX2 =(long)dValueBuff;

	// StartPoint Y2
	dValueBuff = atof(m_DispositionGrid2.GetItemText(2, 1)) * MICROMETRE ;
	//原点位置対応
	if(!IsPosInStageSizeY(dValueBuff)){
		LoadStringML(IDS_OUT_SIZE_LOCATION_Y, l_strBuffer, "Out of the Size Location of Y.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//原点位置対応
		long lMin,lMax;
		GetPosRangeY(&lMin,&lMax);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	m_lStartPointY2 =(long)dValueBuff;

	// Pitch X1
	dValueBuff = atof(m_DispositionGrid.GetItemText(1, 2)) * MICROMETRE ;
	//原点位置対応
	if(!IsPosInStageSizeX(dValueBuff)){
		LoadStringML(IDS_OUT_PITCH_RANGE_X, l_strBuffer, "Out of the Pitch Range of X.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//原点位置対応
		long lMin,lMax;
		GetPosRangeX(&lMin,&lMax);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	m_lPitchX1 =(long)dValueBuff;

	// Pitch Y1
	dValueBuff = atof(m_DispositionGrid.GetItemText(2, 2)) * MICROMETRE ;
	//原点位置対応
	if(!IsPosInStageSizeY(dValueBuff)){
		LoadStringML(IDS_OUT_PITCH_RANGE_Y, l_strBuffer, "Out of the Pitch Range of Y.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//原点位置対応
		long lMin,lMax;
		GetPosRangeY(&lMin,&lMax);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	m_lPitchY1 =(long)dValueBuff;

	// Pitch X2
	dValueBuff = atof(m_DispositionGrid2.GetItemText(1, 2)) * MICROMETRE ;
	//原点位置対応
	if(!IsPosInStageSizeX(dValueBuff)){
		LoadStringML(IDS_OUT_PITCH_RANGE_X, l_strBuffer, "Out of the Pitch Range of X.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//原点位置対応
		long lMin,lMax;
		GetPosRangeX(&lMin,&lMax);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	m_lPitchX2 =(long)dValueBuff;

	// Pitch Y2
	dValueBuff = atof(m_DispositionGrid2.GetItemText(2, 2)) * MICROMETRE ;
	//原点位置対応
	if(!IsPosInStageSizeY(dValueBuff)){
		LoadStringML(IDS_OUT_PITCH_RANGE_Y, l_strBuffer, "Out of the Pitch Range of Y.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//原点位置対応
		long lMin,lMax;
		GetPosRangeY(&lMin,&lMax);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	m_lPitchY2 =(long)dValueBuff;

	// Count X
	dValueBuff = atof(m_DispositionGrid.GetItemText(1, 1)) * MICROMETRE ;
	strBuff = m_DispositionGrid.GetItemText(1, 3);
	//同一座標の繰返し設定可能にした
	if ( m_lPitchX1 != 0 ){
		lMax=0;
		//原点位置対応
		while ( IsPosInStageSizeX((dValueBuff + (lMax * m_lPitchX1))))
			lMax++;
		if(CheckMinMaxInt(strBuff, 1, lMax) != CHECK_DATA_OK){
			return FALSE;
		}
	}
	if ( m_lPitchX2 != 0 ){
		lMax=0;
		//原点位置対応
		while ( IsPosInStageSizeX((dValueBuff + (lMax * m_lPitchX2))))
			lMax++;
		if(CheckMinMaxInt(strBuff, 1, lMax) != CHECK_DATA_OK){
			return FALSE;
		}
	}
	m_iCountX = atoi(strBuff);

	// Count Y
	//Lineモードは m_iCountXで計算（追加ポイント数計算のため"1"固定とする）
	dValueBuff = atof(m_DispositionGrid.GetItemText(2, 1)) * MICROMETRE ;
	if(pCombo->GetCurSel() == STAGE_DRAW_SHAPED_RECTANGLE){
		strBuff = m_DispositionGrid.GetItemText(2, 3);

		if ( m_lPitchY1 != 0 ){
			lMax=0;
			//原点位置対応
			while ( IsPosInStageSizeY((dValueBuff + (lMax * m_lPitchY1))))
				lMax++;
			if(CheckMinMaxInt(strBuff, 1, lMax) != CHECK_DATA_OK){
				return FALSE;
			}
		}
		if ( m_lPitchY2 != 0 ){
			lMax=0;
			//原点位置対応
			while ( IsPosInStageSizeY((dValueBuff + (lMax * m_lPitchY2))))
				lMax++;
			if(CheckMinMaxInt(strBuff, 1, lMax) != CHECK_DATA_OK){
				return FALSE;
			}
		}
		m_iCountY = atoi(strBuff);
	}
	else{
		m_iCountY = 1;
		if ( m_lPitchY1 != 0 ){
			lMax=0;
			//原点位置対応
			while ( IsPosInStageSizeY((dValueBuff + (lMax * m_lPitchY1))))
				lMax++;

			char czTemp[5+1];
			_itoa( m_iCountX, czTemp, 10 );
			if(CheckMinMaxInt(czTemp, 1, lMax) != CHECK_DATA_OK){
				return FALSE;
			}
		}
		if ( m_lPitchY2 != 0 ){
			lMax=0;
			//原点位置対応
			while ( IsPosInStageSizeY((dValueBuff + (lMax * m_lPitchY2))))
				lMax++;

			char czTemp[5+1];
			_itoa( m_iCountX, czTemp, 10 );
			if(CheckMinMaxInt(czTemp, 1, lMax) != CHECK_DATA_OK){
				return FALSE;
			}
		}
	}

	//Lineモードの場合 カウント数はXが有効、Yは無効
	if(pCombo->GetCurSel() ==	STAGE_DRAW_SHAPED_LINE){
		if(m_iCountX > (SCAN_POINT_MAX - m_iNumScans)){
			LoadStringML(IDS_MAXIMUM_NUMBER_CAN_REGISTERED, l_strBuffer, "The Maximum Number which can be Registered is %d.");
			LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
			sprintf(szBuff, l_strBuffer, SCAN_POINT_MAX);
			MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}

	if(pCombo->GetCurSel() == STAGE_DRAW_SHAPED_RECTANGLE){
	//RectAngleモードの場合、総ポイント数が最大を越えないようにする
		if(m_iCountX * m_iCountY > (SCAN_POINT_MAX - m_iNumScans)){
			LoadStringML(IDS_MAXIMUM_NUMBER_CAN_REGISTERED, l_strBuffer, "The Maximum Number which can be Registered is %d.");
			LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
			sprintf(szBuff, l_strBuffer, SCAN_POINT_MAX);
			MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}
	return TRUE;

}

//原点位置対応
BOOL CPointSetupDisposition2PointDlg::IsPosInStageSizeX(double dPos)
{
//	int		OrgPos;		// 1:Center, 2:Top Left, 3:Botton Left, 4:Botton Right, 5:Top Right
	BOOL bRet = FALSE;
	long lMin,lMax;

	GetPosRangeX(&lMin,&lMax);
	if(lMin <= dPos && dPos <= lMax)
		bRet = TRUE;


	return bRet;
}
BOOL CPointSetupDisposition2PointDlg::IsPosInStageSizeY(double dPos)
{
//	int		OrgPos;		// 1:Center, 2:Top Left, 3:Botton Left, 4:Botton Right, 5:Top Right
	BOOL bRet = FALSE;

	long lMin,lMax;

	GetPosRangeY(&lMin,&lMax);
	if(lMin <= dPos && dPos <= lMax)
		bRet = TRUE;

	return bRet;
}
void CPointSetupDisposition2PointDlg::GetPosRangeX(long *pMin,long *pMax)
{
	switch(m_nOrgPos){
	case 1:
		*pMin = static_cast<long>(-(m_dSampleSizeX / 2));
		*pMax = static_cast<long>(m_dSampleSizeX / 2);
		break;
	case 2:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeX);
		break;
	case 3:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeX);
		break;
	case 4:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeX);
		break;
	case 5:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeX);
		break;
	default:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeX);
		break;
	}

}
void CPointSetupDisposition2PointDlg::GetPosRangeY(long *pMin,long *pMax)
{
	switch(m_nOrgPos){
	case 1:
		*pMin = static_cast<long>(-(m_dSampleSizeY / 2));
		*pMax = static_cast<long>(m_dSampleSizeY / 2);
		break;
	case 2:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeY);
		break;
	case 3:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeY);
		break;
	case 4:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeY);
		break;
	case 5:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeY);
		break;
	default:
		*pMin = 0;
		*pMax = static_cast<long>(m_dSampleSizeY);
		break;
	}
}
