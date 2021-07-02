#include "stdafx.h"
#include "System.h"
#include "RecipeStageProgramMeasurementPointGrid.h"

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
BOOL CRecipeStageProgramMeasurementPointGrid::GridInit(int iRows)
{
	///// Grid Initialize /////
	GV_ITEM Item;
	int Cols = 3;
	int Rows = iRows >= 1001 ? iRows+1+1 : 1000+1 ;//NumScanPoint + Header + AddCell;
	//int Rows = iRows >= 31 ? iRows+1+1 : 38+1+1 ;//NumScanPoint + Header + AddCell//ジャストグリット高さ数;
	//int Rows = SCAN_POINT_MAX + 1;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;

// Kojika 20090527 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// Kojika 20090527 Add End

// Kojika 20090527 Change
//	char* ColItem[]={
//		"Point",
//		"X (mm)",
//		"Y (mm)",
//	};
	char* ColItemJPN[]={
		"ポイント",
		"X (mm)",
		"Y (mm)",
	};
	char* ColItemENU[]={
		"Point",
		"X (mm)",
		"Y (mm)",
	};
// Kojika 20090527 Change End

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
	SetRowHeight(0, 18);
	SetColumnWidth(0, 50);

	for (row = 1; row < GetRowCount(); row++){
		SetRowHeight(row, 18);
	}

	SetColumnWidth(1, 70);
	SetColumnWidth(2, 70);

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
// Kojika 20090527 Change
		//Item.strText.Format("%s", ColItem[col]);
		if(l_SystemConfig.nLanguage == 0){
			Item.strText.Format("%s", ColItemENU[col]);
		}
		else{
			Item.strText.Format("%s", ColItemJPN[col]);
		}
// Kojika 20090527 Change End
		SetItem(&Item);
	}

	///// Title Row /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%d", row);
		SetItem(&Item);
	}

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
void CRecipeStageProgramMeasurementPointGrid::GridDisp(int row, int col, LPCTSTR pszText, DWORD TextStyle)
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
void CRecipeStageProgramMeasurementPointGrid::GridEditable(BOOL Editable/*=TRUE*/)
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
