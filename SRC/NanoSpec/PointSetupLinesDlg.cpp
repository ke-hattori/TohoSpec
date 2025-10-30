// PointSetupLinesDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "PointSetupLinesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CPointSetupLinesDlg ダイアログ
// #########################################################################

// =========================================================================
//
CPointSetupLinesDlg::CPointSetupLinesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointSetupLinesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointSetupLinesDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CPointSetupLinesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointSetupLinesDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_LINES_POINT_LIST_GRID, m_PointListGrid);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CPointSetupLinesDlg, CDialog)
	//{{AFX_MSG_MAP(CPointSetupLinesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CPointSetupLinesDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CPointSetupLinesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// Point List Grid /////
	PointListGrid_Init(&m_PointListGrid);
	GridEditable(&m_PointListGrid, TRUE);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CPointSetupLinesDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	CDialog::OnOK();
}

// =========================================================================
//
void CPointSetupLinesDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

// =========================================================================
//
void CPointSetupLinesDlg::GridDisp(CGridCtrl* pGrid, int row, int col, char* text, DWORD TextStyle)
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
void CPointSetupLinesDlg::GridEditable(CGridCtrl* pGrid, BOOL Editable)
{
	pGrid->SetEditable(Editable);
	pGrid->EnableSelection(Editable);
	pGrid->SetTrackFocusCell(FALSE);
	pGrid->SetFrameFocusCell(FALSE);
	pGrid->SetSingleRowSelection(FALSE);
	pGrid->SetSingleColSelection(FALSE);
}

// =========================================================================
//
BOOL CPointSetupLinesDlg::PointListGrid_Init(CGridCtrl* pGrid)
{
	///// Grid Initialize /////
	GV_ITEM Item;
	int Cols = 3;
	int Rows = 2+1;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;

	// Kojika 20090601 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090601 Add End

	char* ColItem[]={
		"Point",
		"X(mm)",
		"Y(mm)",
	};

	// Kojika 20090601 Change
//	  char* RowItem[]={
////	  "Start",
////	  "End",
//		  "スタート",
//		  "エンド",
//	  };
	char* RowItem_JPN[]={
		"スタート",
		"エンド",
	};
	char* RowItem_ENU[]={
		"Start",
		"End",
	};
	// Kojika 20090601 Change End

	///// Grid Initialize /////
	pGrid->SetListMode(TRUE);
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
	pGrid->SetRowHeight(0, 18);
	pGrid->SetColumnWidth(0, 60);

	for (row = 1; row < pGrid->GetRowCount(); row++){
		pGrid->SetRowHeight(row, 18);
	}

	pGrid->SetColumnWidth(1, 90);
	pGrid->SetColumnWidth(2, 90);

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
		Item.strText.Format("%s",ColItem[col]);
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
		// Kojika 20090601 Change
//		Item.strText.Format("%s", RowItem[row - 1]);
		if(l_SystemConfig.nLanguage == 0){
			Item.strText.Format("%s", RowItem_ENU[row - 1]);
		}
		else{
			Item.strText.Format("%s", RowItem_JPN[row - 1]);
		}
		// Kojika 20090601 Change End
		pGrid->SetItem(&Item);
	}

	return TRUE;
}
