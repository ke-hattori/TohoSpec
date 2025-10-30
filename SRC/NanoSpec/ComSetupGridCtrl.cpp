#include "stdafx.h"
#include "System.h"
#include "ComSetupGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ##########################################################################
// RecipeStageProgramMeasurementPointMap
// ##########################################################################

// ==========================================================================
//
BOOL CComSetupGridCtrl::GridInit(int iRows)
{
	///// Grid Initialize /////
	GV_ITEM Item;
	int Cols = 6;
	int Rows = iRows >= 5 ? iRows+1 : 5+1 ;//NumScanPoint + Header + AddCell;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;

	char* ColItem[]={
		"",
		"ComNo",
		"BaudRate",
		"Parity",
		"ByteSize",
		"StopBits",
	};

	///// Grid Initialize /////
	SetListMode(FALSE);
	EnableDragAndDrop(FALSE);
	SetTextBkColor(WHITE_COLOR);
	SetRowResize(0);
	SetEditable(TRUE);				// <--- When Edit is TRUE
	EnableSelection(TRUE);			// <--- When Edit is TRUE
	SetTrackFocusCell(FALSE);		// <--- When Edit is TRUE
	SetFrameFocusCell(FALSE);		// <--- When Edit is TRUE
	SetFixedColumnSelection(FALSE);
	SetFixedRowSelection(TRUE);

	TRY {
		SetRowCount(Rows);
		SetColumnCount(Cols);
		SetFixedRowCount(FixRows);
		SetFixedColumnCount(FixCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	END_CATCH

	///// Grid Size Initialize /////
	SetRowHeight(0, 20);
	SetColumnWidth(0, 60);

	for (row = 1; row < GetRowCount(); row++){
		SetRowHeight(row, 18);
	}

	SetColumnWidth(1, 60);
	SetColumnWidth(2, 60);
	SetColumnWidth(3, 60);
	SetColumnWidth(4, 60);
	SetColumnWidth(5, 60);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);

	for (row = 0; row < GetRowCount(); row++){
		for (col = 0; col < GetColumnCount(); col++){
			SetItemFont(row,col,&LogFont);
		}
	}

	///// Title Column /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s", ColItem[col]);
		SetItem(&Item);
	}

	///// Title Row /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

//	LPCTSTR pszHeadCom[] = { "SR", "RS", "CA", "Stress", "" };
//
//	for(row = 1; row < GetRowCount(); row++){
//		Item.mask = GVIF_TEXT|GVIF_FORMAT;
//		Item.row = row;
//		Item.col = 0;
//		Item.nFormat = dwTextStyle;
//		Item.strText.Format("%s", pszHeadCom[row - 1]);
//		SetItem(&Item);
//	}

	///// Grid Alignment /////
	dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < GetRowCount(); row++){
		for(col = 1; col < GetColumnCount(); col++){
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("");
			SetItem(&Item);
		}
	}

	return TRUE;
}

// ==========================================================================
//
void CComSetupGridCtrl::GridDisp(int row, int col, LPCTSTR pszText, DWORD TextStyle)
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
	SetItem(&Item);

	Invalidate();
}

// ==========================================================================
//
void CComSetupGridCtrl::GridEditable(BOOL Editable/*=TRUE*/)
{
	SetEditable(Editable);
	EnableSelection(Editable);
	SetTrackFocusCell(FALSE);
	SetFrameFocusCell(FALSE);
	SetSingleRowSelection(Editable);
	SetSingleColSelection(Editable);
	SetFixedColumnSelection(Editable);
	SetFixedRowSelection(Editable);
}
