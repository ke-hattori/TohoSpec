// PointSetupDispositionDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <math.h>

#include "System.h"
#include "NanoSpec.h"
#include "PointSetupDispositionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CPointSetupDispositionDlg ダイアログ
// #########################################################################

// =========================================================================
//
CPointSetupDispositionDlg::CPointSetupDispositionDlg(int iNumScans, double dSampleSizeX, double dSampleSizeY, int nOrgPos,CWnd* pParent /*=NULL*/)
	: CDialog(CPointSetupDispositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointSetupDispositionDlg)
	//}}AFX_DATA_INIT
	m_lStartPointX = m_lStartPointY =0;
	m_lPitchX = m_lPitchY = 0;
	m_iCountX = m_iCountY = 0;
	m_dSampleSizeX = dSampleSizeX;
	m_dSampleSizeY = dSampleSizeY;
	//2009.09.30 bagus se --{--
	m_nOrgPos = nOrgPos;
	//2009.09.30 bagus se --}--
	m_iNumScans = iNumScans;
}

// =========================================================================
//
void CPointSetupDispositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointSetupDispositionDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_DISPOSITON_GRID, m_DispositionGrid);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CPointSetupDispositionDlg, CDialog)
	//{{AFX_MSG_MAP(CPointSetupDispositionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CPointSetupDispositionDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CPointSetupDispositionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox* pCombo;
	CGridCtrl* pGrid = &m_DispositionGrid;

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	///// Disposition Grid /////
	DispositionGrid_Init(&m_DispositionGrid);
	GridEditable(&m_DispositionGrid, TRUE);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Draw Shaped /////
	pCombo = (CComboBox*)GetDlgItem(IDC_DRAW_SHAPED);
	for(int i = 0 ; i < STAGE_DRAW_SHAPED_MAX; i++){
		// Kojika 20090528 Change
		//pCombo->AddString(STAGE_DRAW_SHAPED_ITEM[i]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(STAGE_DRAW_SHAPED_ITEM_ENU[i]);
		}
		else{
			pCombo->AddString(STAGE_DRAW_SHAPED_ITEM_JPN[i]);
		}
		// Kojika 20090528 Change End
	}
	pCombo->SetCurSel(0);

	///// SetCur Cell //////
	pGrid->SetItemState(1, 1, GVIS_SELECTED);
	pGrid->SetFocusCell(1, 1);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CPointSetupDispositionDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DRAW_SHAPED);

	if(!CheckData(&m_DispositionGrid)){
		return;
	}

	m_iDrawShaped = pCombo->GetCurSel();
	CDialog::OnOK();
}

// =========================================================================
//
void CPointSetupDispositionDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

// =========================================================================
//
void CPointSetupDispositionDlg::GridDisp(CGridCtrl* pGrid, int row, int col, char* text, DWORD TextStyle)
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
void CPointSetupDispositionDlg::GridEditable(CGridCtrl* pGrid, BOOL Editable)
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
BOOL CPointSetupDispositionDlg::DispositionGrid_Init(CGridCtrl* pGrid)
{
	///// Grid Initialize /////
	GV_ITEM Item;
	int Cols = 4;
	int Rows = 2+1;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;
	//Saiki 20090529 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090529 Add <-----
	//Saiki 20090529 Change ----->
//	  char* ColItem[]={
////	  "Coord",
////	  "Start (mm)",
////	  "Pitch (mm)",
////	  "Count",
//		  "軸",
//		  "開始位置 (mm)",
//		  "間隔 (mm)",
//		  "ポイント数",
//	  };
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

	//Saiki 20090529 Change <-----

	char* RowItem[]={
		"X",
		"Y",
	};

	///// Grid Initialize /////
	pGrid->SetListMode(FALSE);
	pGrid->EnableDragAndDrop(FALSE);
	pGrid->SetTextBkColor(WHITE_COLOR);
	pGrid->SetRowResize(0);

	pGrid->SetEditable(FALSE);				// <--- When Edit is TRUE
	pGrid->EnableSelection(FALSE);			// <--- When Edit is TRUE
	pGrid->SetTrackFocusCell(FALSE);		// <--- When Edit is TRUE
	pGrid->SetFrameFocusCell(FALSE);		// <--- When Edit is TRUE

	TRY {
		pGrid->SetRowCount(Rows);
		pGrid->SetColumnCount(Cols);
		pGrid->SetFixedRowCount(FixRows);
		pGrid->SetFixedColumnCount(FixCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	END_CATCH

	// Make cell 1,1 read-only
	//pGrid->SetItemState(1,1, pGrid->GetItemState(1,1) | GVIS_READONLY);

	//BOOL TitleTips = TRUE;
	//pGrid->EnableTitleTips(TitleTips);

	///// Grid Size Initialize /////
	//Saiki 20090129 Change ----->
 //   pGrid->SetRowHeight(0, 18);
	pGrid->SetRowHeight(0, 20);
	//Saiki 20090129 Change <-----
	pGrid->SetColumnWidth(0, 60);

	for (row = 1; row < pGrid->GetRowCount(); row++){
		pGrid->SetRowHeight(row, 18);
	}

	pGrid->SetColumnWidth(1, 90);
	pGrid->SetColumnWidth(2, 90);
	pGrid->SetColumnWidth(3, 90);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);

	for (row = 0; row < pGrid->GetRowCount(); row++){
		for (col = 0; col < pGrid->GetColumnCount(); col++){
			pGrid->SetItemFont(row,col,&LogFont);
		}
	}

	///// Title Column /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < pGrid->GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		//Saiki 20090529 Change ----->
		//Item.strText.Format("%s",ColItem[col]);
		if(l_SystemConfig.nLanguage == 0){
			Item.strText.Format("%s",ColItem_ENU[col]);
		}
		else{
			Item.strText.Format("%s",ColItem_JPN[col]);
		}
		//Saiki 20090529 Change <-----
		pGrid->SetItem(&Item);
	}

	///// Title Row /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < pGrid->GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s", RowItem[row - 1]);
		pGrid->SetItem(&Item);
	}

	///// Grid Alignment /////
	dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < pGrid->GetRowCount(); row++){
		for(col = 1; col < pGrid->GetColumnCount(); col++){
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("");
			pGrid->SetItem(&Item);
		}
	}

	return TRUE;
}

// =========================================================================
//
BOOL CPointSetupDispositionDlg::CheckData(CGridCtrl* pGrid)
{
	CComboBox* pCombo;
	int iMin = 1, iMax = SCAN_POINT_MAX;
	long lMax;
	CString strBuff;
	CString strBuffSub;
	CString csBuff;
	char szBuff[256];
	double dValueBuff;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	///// Draw Shaped /////
	pCombo = (CComboBox*)GetDlgItem(IDC_DRAW_SHAPED);
	if(pCombo->GetCurSel() == CB_ERR){
		// Kojika 20090601 Change
//		MessageBox("Draw Shaped is empty.", "POINT SETUP", MB_OK | MB_ICONSTOP);
//		MessageBox("描画図形が選ばれていません", "POINT SETUP", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_DRAW_SHAPED_EMPTY, l_strBuffer, "Draw Shaped is empty.");
		LoadStringML(IDS_TITLE_POINT_SETUP, l_strTitle, "POINT SETUP");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090601 Change End
		return FALSE;
	}

	// グリッドの状態を取得
//OkabeAdd20090319--->
	// X軸の始点が「空」の時は抜ける
	strBuff = pGrid->GetItemText(1, 1);
	if (strlen(strBuff)==0){
		return FALSE;
	}
	// Y軸の始点が「空」の時は抜ける
	strBuff = pGrid->GetItemText(2, 1);
	if (strlen(strBuff)==0){
		return FALSE;
	}
	// X軸の間隔が「空」の時は抜ける
	strBuff = pGrid->GetItemText(1, 2);
	if (strlen(strBuff)==0) {
		return FALSE;
	}
	// Y軸の間隔が「空」の時は抜ける
	strBuff = pGrid->GetItemText(2, 2);
	if (strlen(strBuff)==0){
		return FALSE;
	}
	// X軸のポイント数が「空」または「0以下」の時は抜ける
	//OkabeCorrection20090612--->
	strBuff = pGrid->GetItemText(1, 3);
	if ((strlen(strBuff)==0) || (atoi(strBuff) <=0)){
		return FALSE;
	}
	//OkabeCorrection20090612<---
	// Y軸のポイント数が四角配置で「空」または「0以下」の時は抜ける
	strBuff = pGrid->GetItemText(2, 3);
	//OkabeCorrection20090612--->
	if (pCombo->GetCurSel() == STAGE_DRAW_SHAPED_RECTANGLE){
		if ((strlen(strBuff)==0)  || (atoi(strBuff) <=0)) {
			return FALSE;
		}
	}
	//OkabeCorrection20090612<---
//OkabeAdd20090319<---

	// StartPoint X
	dValueBuff = atof(pGrid->GetItemText(1, 1)) * MICROMETRE ;
	//2009.09.30 bagus se --{--
	//原点位置対応
	//if(fabs(dValueBuff) > (m_dSampleSizeX/2)){
	if(!IsPosInStageSizeX(dValueBuff)){
	//2009.09.30 bagus se --}--
		// Kojika 20090601 Change
//		sprintf(szBuff, "Out of the Size Location of X.\n(%.3lf - %.3lf)", -(m_dSampleSizeX/2) / MICROMETRE, (m_dSampleSizeX/2) / MICROMETRE);
//		sprintf(szBuff, "Xの位置座標がサイズの外にあります.\n(%.3lf - %.3lf)", -(m_dSampleSizeX/2) / MICROMETRE, (m_dSampleSizeX/2) / MICROMETRE);
//		MessageBox(szBuff, "POINT SETUP DISPOSITION", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_OUT_SIZE_LOCATION_X, l_strBuffer, "Out of the Size Location of X.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//2009.09.30 bagus se --{--
		//原点位置対応
		long lMin,lMax;
		GetPosRangeX(&lMin,&lMax);
		//sprintf(szBuff, l_strBuffer, -(m_dSampleSizeX/2) / MICROMETRE, (m_dSampleSizeX/2) / MICROMETRE);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		//2009.09.30 bagus se --}--
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090601 Change End
		return FALSE;
	}
	m_lStartPointX =(long)dValueBuff;

	// StartPoint Y
	dValueBuff = atof(pGrid->GetItemText(2, 1)) * MICROMETRE ;
	//2009.09.30 bagus se --{--
	//原点位置対応
	//if(fabs(dValueBuff) > (m_dSampleSizeY/2)){
	if(!IsPosInStageSizeY(dValueBuff)){
	//2009.09.30 bagus se --}--
		// Kojika 20090601 Change
//		sprintf(szBuff, "Out of the Size Location of Y.\n(%.3lf - %.3lf)", -(m_dSampleSizeY/2) / MICROMETRE, (m_dSampleSizeY/2) / MICROMETRE );
//		sprintf(szBuff, "Ｙの位置座標がサイズの外にあります.\n(%.3lf - %.3lf)", -(m_dSampleSizeY/2) / MICROMETRE, (m_dSampleSizeY/2) / MICROMETRE );
//		MessageBox(szBuff, "POINT SETUP DISPOSITION", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_OUT_SIZE_LOCATION_Y, l_strBuffer, "Out of the Size Location of Y.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//2009.09.30 bagus se --{--
		//原点位置対応
		long lMin,lMax;
		GetPosRangeY(&lMin,&lMax);
		//sprintf(szBuff, l_strBuffer, -(m_dSampleSizeY/2) / MICROMETRE, (m_dSampleSizeY/2) / MICROMETRE );
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		//2009.09.30 bagus se --}--

		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090601 Change End
		return FALSE;
	}
	m_lStartPointY =(long)dValueBuff;

	// Pitch X
	dValueBuff = atof(pGrid->GetItemText(1, 2)) * MICROMETRE ;

//同一座標の繰返し設定可能にした
//OkabeDelete20090612--->
//	//OkabeAdd20090319--->
//		if(dValueBuff == 0){
//			return FALSE;
//		}
//	//OkabeAdd20090319<---
//OkabeDelete20090612<---

	//2009.09.30 bagus se --{--
	//原点位置対応
	//if(fabs(dValueBuff) > (m_dSampleSizeX/2)){
	if(!IsPosInStageSizeX(dValueBuff)){
	//2009.09.30 bagus se --}--
		// Kojika 20090601 Change
//		sprintf(szBuff, "Out of the Pitch Range of X.\n(%.3lf - %.3lf)", -(m_dSampleSizeX/2) / MICROMETRE,(m_dSampleSizeX/2) / MICROMETRE);
//		sprintf(szBuff, "Ｘの範囲が、ピッチの外にあります.\n(%.3lf - %.3lf)", -(m_dSampleSizeX/2) / MICROMETRE,(m_dSampleSizeX/2) / MICROMETRE);
//		MessageBox(szBuff, "POINT SETUP DISPOSITION", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_OUT_PITCH_RANGE_X, l_strBuffer, "Out of the Pitch Range of X.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//2009.09.30 bagus se --{--
		//原点位置対応
		long lMin,lMax;
		GetPosRangeX(&lMin,&lMax);
		//sprintf(szBuff, l_strBuffer, -(m_dSampleSizeX/2) / MICROMETRE, (m_dSampleSizeX/2) / MICROMETRE);
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		//2009.09.30 bagus se --}--
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090601 Change End
		return FALSE;
	}
	m_lPitchX =(long)dValueBuff;

	// Pitch Y
	dValueBuff = atof(pGrid->GetItemText(2, 2)) * MICROMETRE ;

//OkabeDelete20090612--->
//同一座標の繰返し設定可能にした
//	//OkabeAdd20090319--->
//		if(dValueBuff == 0){
//			return FALSE;
//		}
//	//OkabeAdd20090319<---
//OkabeDelete20090612<---

	//2009.09.30 bagus se --{--
	//原点位置対応
	//if(fabs(dValueBuff) > (m_dSampleSizeY/2)){
	if(!IsPosInStageSizeY(dValueBuff)){
	//2009.09.30 bagus se --}--
		// Kojika 20090601 Change
//		sprintf(szBuff, "Out of the Pitch Range of Y.\n(%.3lf - %.3lf)", -(m_dSampleSizeY/2) / MICROMETRE, (m_dSampleSizeY/2) / MICROMETRE);
//		sprintf(szBuff, "Ｙの範囲が、ピッチの外にあります..\n(%.3lf - %.3lf)", -(m_dSampleSizeY/2) / MICROMETRE, (m_dSampleSizeY/2) / MICROMETRE);
//		MessageBox(szBuff, "POINT SETUP DISPOSITION", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_OUT_PITCH_RANGE_Y, l_strBuffer, "Out of the Pitch Range of Y.\n(%.3lf - %.3lf)");
		LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
		//2009.09.30 bagus se --{--
		//原点位置対応
		long lMin,lMax;
		GetPosRangeY(&lMin,&lMax);
		//sprintf(szBuff, l_strBuffer, -(m_dSampleSizeY/2) / MICROMETRE, (m_dSampleSizeY/2) / MICROMETRE );
		sprintf(szBuff, l_strBuffer, lMin, lMax);
		//2009.09.30 bagus se --}--
		MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090601 Change End
		return FALSE;
	}
	m_lPitchY =(long)dValueBuff;

	// Count X
	dValueBuff = atof(pGrid->GetItemText(1, 1)) * MICROMETRE ;
	strBuff = pGrid->GetItemText(1, 3);

//OkabeChange20090612--->
//同一座標の繰返し設定可能にした
	if ( m_lPitchX != 0 ){
		lMax=0;
		//2009.09.30 bagus sr --{--
		//原点位置対応
		//while ( fabs((dValueBuff + (lMax * m_lPitchX))) <= m_dSampleSizeX/2 )
		while ( IsPosInStageSizeX((dValueBuff + (lMax * m_lPitchX))))
			lMax++;
		//2009.09.30 bagus sr --}--
		if(CheckMinMaxInt(strBuff, 1, lMax) != CHECK_DATA_OK){
			return FALSE;
		}
	}
	m_iCountX = atoi(strBuff);

	// Count Y
	//Lineモードは m_iCountXで計算（追加ポイント数計算のため"1"固定とする）
	dValueBuff = atof(pGrid->GetItemText(2, 1)) * MICROMETRE ;
	if(pCombo->GetCurSel() == STAGE_DRAW_SHAPED_RECTANGLE){
		strBuff = pGrid->GetItemText(2, 3);

		if ( m_lPitchY != 0 ){
			lMax=0;
			//2009.09.30 bagus sr --{--
			//原点位置対応
			//while ( fabs((dValueBuff + (lMax * m_lPitchY))) <= m_dSampleSizeY/2 )
			while ( IsPosInStageSizeY((dValueBuff + (lMax * m_lPitchY))))
				lMax++;
			//2009.09.30 bagus sr --{--
			if(CheckMinMaxInt(strBuff, 1, lMax) != CHECK_DATA_OK){
				return FALSE;
			}
		}
		m_iCountY = atoi(strBuff);
	}
	else{
		m_iCountY = 1;
		if ( m_lPitchY != 0 ){
			lMax=0;
			//2009.09.30 bagus sr --{--
			//原点位置対応
			//while ( fabs((dValueBuff + (lMax * m_lPitchY))) <= m_dSampleSizeY/2 )
			while ( IsPosInStageSizeY((dValueBuff + (lMax * m_lPitchY))))
				lMax++;
			//2009.09.30 bagus sr --{--

			char czTemp[5+1];
			_itoa( m_iCountX, czTemp, 10 );
			if(CheckMinMaxInt(czTemp, 1, lMax) != CHECK_DATA_OK){
				return FALSE;
			}
		}
	}
//OkabeAdd20090612<---

	//Lineモードの場合 カウント数はXが有効、Yは無効
	if(pCombo->GetCurSel() ==	STAGE_DRAW_SHAPED_LINE){
		/*if(m_iCountX != m_iCountY){
			MessageBox("Please enter a same integer in count", "POINT SETUP DISPOSITION", MB_OK|MB_ICONSTOP);
			return FALSE;
		}*/
		if(m_iCountX > (SCAN_POINT_MAX - m_iNumScans)){
			// Kojika 20090601 Change
//			sprintf(szBuff, "The Maximum Number which can be Registered is %d.", SCAN_POINT_MAX);
//			sprintf(szBuff, "最大登録数は%dです.", SCAN_POINT_MAX);
//			MessageBox(szBuff, "POINT SETUP DISPOSITION", MB_OK|MB_ICONSTOP);
			LoadStringML(IDS_MAXIMUM_NUMBER_CAN_REGISTERED, l_strBuffer, "The Maximum Number which can be Registered is %d.");
			LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
			sprintf(szBuff, l_strBuffer, SCAN_POINT_MAX);
			MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
			// Kojika 20090601 Change End
			return FALSE;
		}
	}

	if(pCombo->GetCurSel() == STAGE_DRAW_SHAPED_RECTANGLE){
	//RectAngleモードの場合、総ポイント数が最大を越えないようにする
		if(m_iCountX * m_iCountY > (SCAN_POINT_MAX - m_iNumScans)){
			// Kojika 20090601 Change
//			sprintf(szBuff, "The Maximum Number which can be Registered is %d.", SCAN_POINT_MAX);
//			sprintf(szBuff, "最大登録数は%dです.", SCAN_POINT_MAX);
//			MessageBox(szBuff, "POINT SETUP DISPOSITION", MB_OK|MB_ICONSTOP);
			LoadStringML(IDS_MAXIMUM_NUMBER_CAN_REGISTERED, l_strBuffer, "The Maximum Number which can be Registered is %d.");
			LoadStringML(IDS_TITLE_POINT_SETUP_DISPOSITION, l_strTitle, "POINT SETUP DISPOSITION");
			sprintf(szBuff, l_strBuffer, SCAN_POINT_MAX);
			MessageBox(szBuff, l_strTitle, MB_OK|MB_ICONSTOP);
			// Kojika 20090601 Change End
			return FALSE;
		}
	}
	return TRUE;

}

//2009.09.30 bagus se --{--
//原点位置対応
BOOL CPointSetupDispositionDlg::IsPosInStageSizeX(double dPos)
{
//	int		OrgPos;		// 1:Center, 2:Top Left, 3:Botton Left, 4:Botton Right, 5:Top Right
	BOOL bRet = FALSE;
	long lMin,lMax;

	GetPosRangeX(&lMin,&lMax);
	if(lMin <= dPos && dPos <= lMax)
		bRet = TRUE;


	return bRet;
}
BOOL CPointSetupDispositionDlg::IsPosInStageSizeY(double dPos)
{
//	int		OrgPos;		// 1:Center, 2:Top Left, 3:Botton Left, 4:Botton Right, 5:Top Right
	BOOL bRet = FALSE;

	long lMin,lMax;

	GetPosRangeY(&lMin,&lMax);
	if(lMin <= dPos && dPos <= lMax)
		bRet = TRUE;

	return bRet;
}
void CPointSetupDispositionDlg::GetPosRangeX(long *pMin,long *pMax)
{
	switch(m_nOrgPos){
	case 1:
		*pMin = -(m_dSampleSizeX / 2);
		*pMax = m_dSampleSizeX / 2;
		break;
	case 2:
		*pMin = 0;
		*pMax = m_dSampleSizeX;
		break;
	case 3:
		*pMin = 0;
		*pMax = m_dSampleSizeX;
		break;
	case 4:
		*pMin = 0;
		*pMax = m_dSampleSizeX;
		break;
	case 5:
		*pMin = 0;
		*pMax = m_dSampleSizeX;
		break;
	default:
		*pMin = 0;
		*pMax = m_dSampleSizeX;
		break;
	}

}
void CPointSetupDispositionDlg::GetPosRangeY(long *pMin,long *pMax)
{
	switch(m_nOrgPos){
	case 1:
		*pMin = -(m_dSampleSizeY / 2);
		*pMax = m_dSampleSizeY / 2;
		break;
	case 2:
		*pMin = 0;
		*pMax = m_dSampleSizeY;
		break;
	case 3:
		*pMin = 0;
		*pMax = m_dSampleSizeY;
		break;
	case 4:
		*pMin = 0;
		*pMax = m_dSampleSizeY;
		break;
	case 5:
		*pMin = 0;
		*pMax = m_dSampleSizeY;
		break;
	default:
		*pMin = 0;
		*pMax = m_dSampleSizeY;
		break;
	}
}
//2009.09.30 bagus se --}--
