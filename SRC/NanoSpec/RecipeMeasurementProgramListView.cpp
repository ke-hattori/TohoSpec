// RecipeMeasurementProgramListView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MultiLangHelper.h"
#include "SelectMeasurementTypeDlg.h"
#include "RecipeMeasurementProgramListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementProgramListView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementProgramListView, CNanoRecipeListUI)

// =========================================================================
//
CRecipeMeasurementProgramListView::CRecipeMeasurementProgramListView()
{
}

// =========================================================================
//
CRecipeMeasurementProgramListView::~CRecipeMeasurementProgramListView()
{
}

BEGIN_MESSAGE_MAP(CRecipeMeasurementProgramListView, CNanoRecipeListUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementProgramListView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
//
void CRecipeMeasurementProgramListView::List_HeaderSet()
{
	LV_COLUMN listcol;
	DWORD dwStyle = 0;
	int ItemMax;
/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//	  //Saiki 20090527 Add ----->
//	  SYSTEM_CONFIG l_SystemConfig;
//	  ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
//	  //Saiki 20090527 Add <-----
*/
/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };

// 2009.11.17 bagus RS 修正 --{--
#if 0
	int Width[] = {
		0,		// (0) Index
		240,	// (1) Name
		70, 	// (2) Head Type
		170,	// (3) Measurement Item
		140,	// (4) Date
		435,	// (5) Comment
	};
#else
	int Width[] = {
		0,		// (0) Index
// 2009.12.09 bagus --{--
//		240,	// (1) Name
//		105,	// (2) Head Type
		220,	// (1) Name
		125,	// (2) Head Type
// 2009.12.09 bagus --}--
		170,	// (3) Measurement Item
		140,	// (4) Date
		400,	// (5) Comment
	};
#endif
// 2009.11.17 bagus RS 修正 --}--

/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//	  //Saiki 20090527 Change ----->
////	char* Item[] = {
//////		"",
//////		"Name",
//////		"Head",
//////		"Meas Item",
//////		"Date",
//////		"Comment"
////	  "",
////		"名前",
////		"ヘッド",
////		"測定種別",
////		"データ",
////		"コメント"
////	};
//	  char* Item_ENU[] = {
//		"",
//		"Name",
//		"Head",
//		"Meas Item",
//		"Date",
//		"Comment"
//	  };
//	  char* Item_JPN[] = {
//		  "",
//		  "名前",
//		  "ヘッド",
//		  "測定種別",
//		  "データ",
//		  "コメント"
//	  };
//	  //Saiki 20090527 Change <-----
*/
/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

	ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);
/* added 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
	CStringArray l_straHdr;
	l_straHdr.SetSize(ItemMax);
	l_straHdr.SetAt(0, _T(""));
	LoadStringML(IDS_LSTHDR_NAME,		l_straHdr.ElementAt(1), _T("Name"));
	LoadStringML(IDS_LSTHDR_HEAD,		l_straHdr.ElementAt(2), _T("Head"));
	LoadStringML(IDS_LSTHDR_MEASITEM,	l_straHdr.ElementAt(3), _T("Meas Item"));
	LoadStringML(IDS_LSTHDR_DATE,		l_straHdr.ElementAt(4), _T("Date"));
	LoadStringML(IDS_LSTHDR_COMMENT,	l_straHdr.ElementAt(5), _T("Comment"));
/* added 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//		  //Saiki 20090527 Change ----->
//		  //listcol.pszText = Item[i];
//		  if(l_SystemConfig.nLanguage == 0){
//			  listcol.pszText = Item_ENU[i];
//		  }
//		  else{
//			  listcol.pszText = Item_JPN[i];
//		  }
//		  //Saiki 20090527 Change <-----
*/
/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ----------			   */
		listcol.pszText = ((LPTSTR) (LPCTSTR) l_straHdr.GetAt(i));
/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */
		CNanoListUI::rfListCtrl().InsertColumn(i,&listcol);
	}
}

// =========================================================================
//
void CRecipeMeasurementProgramListView::List_DataSet()
{
	CFileFind Finder;
	CString strBuffer;
	char szDirPath[_MAX_PATH];
	char szName[RECIPE_NAME_LEN + 1];
	char szDate[256];
	BOOL bLoop;
	CTime time;
	MEAS_PROG_INFO MeasProgInfo;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- { ---------- */
	DWORD l_dwDataCount = 0;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- } ---------- */

	CNanoListUI::rfListCtrl().DeleteAllItems();
	sprintf(szDirPath, "%s*%s", g_szDb_Measurement_Program_Dir, MEASUREMENTPGM_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		memset(&MeasProgInfo, 0, sizeof(MeasProgInfo));
		// ファイル名を取得
		strBuffer = Finder.GetFileTitle();
		if ( strBuffer[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		sprintf(szName, "%s", (LPCTSTR)strBuffer);
		// 最終変更日時を取得
		Finder.GetLastWriteTime(time);
		sprintf(szDate, "%s", (LPCTSTR)time.Format("%Y.%m.%d %H:%M:%S"));

		if(RecipeFile_LoadRecipe(&MeasProgInfo, szName, RECIPE_FILE_MEASUREMENT_PROGRAM)){
			List_Add(szName, (int)MeasProgInfo.ScanParams.hdr.wHeadType,
				(int)MeasProgInfo.ScanParams.hdr.wScanType, szDate, MeasProgInfo.hdr.szComment);
		}
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- { ---------- */
		l_dwDataCount++;
		if (RCP_PGM_NUM_MAX <= l_dwDataCount) {
			break;
		}
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- } ---------- */
	}
}

// =========================================================================
//
void CRecipeMeasurementProgramListView::List_Add(char* szName,int iHeadType, int iMeasType, char* szDate, char* szComment)
{
	LV_ITEM listitem;
	int item;
	CString csComment(szComment);

	listitem.iItem = CNanoListUI::rfListCtrl().GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	item = CNanoListUI::rfListCtrl().InsertItem(&listitem);

	///// Name /////
	listitem.iSubItem = 1;
	listitem.pszText = szName;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Head /////
	listitem.iSubItem = 2;
	listitem.pszText = (char*)HEAD_TYPE_ITEM[iHeadType];
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Measurement Item /////
	listitem.iSubItem = 3;

	CMLScanType mlScanTypeItem(iHeadType);
	listitem.pszText = (LPTSTR)((LPCTSTR)mlScanTypeItem[iMeasType]);
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Date /////
	listitem.iSubItem = 4;
	listitem.pszText = szDate;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Comment /////
	listitem.iSubItem = 5;
	csComment.Replace("\n", " ");
	listitem.pszText = csComment.GetBuffer(0);
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	CNanoListUI::rfListCtrl().SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

// =========================================================================
//
void CRecipeMeasurementProgramListView::RecipeEntry(BOOL bNew)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CSelectMeasurementTypeDlg dlg;

	int iHeadType;
	int iMeasType;
	int iIndex;
	char szName[RECIPE_NAME_LEN + 1];
	MEAS_PROG_INFO MeasProgInfo;

	memset(szName, 0, sizeof(szName));
	pMainFrame->m_bNew = bNew;
	pMainFrame->SetSelectListName("");

	if(bNew){
		if(dlg.DoModal() == IDOK){
			iHeadType = dlg.m_iHeadType;
			iMeasType = dlg.m_iMeasType;
		}
		else{
			return;
		}
	}
	else{
		iIndex = CNanoListUI::rfListCtrl().GetSelectionMark();

		if(iIndex <= -1){
			return;
		}

		CNanoListUI::rfListCtrl().GetItemText(iIndex, 1, szName, RECIPE_NAME_LEN + 1);

		if(szName[0] == '\0'){
			return;
		}

		if(RecipeFile_LoadRecipe(&MeasProgInfo, szName, RECIPE_FILE_MEASUREMENT_PROGRAM)){
			iHeadType = MeasProgInfo.ScanParams.hdr.wHeadType;
			iMeasType = MeasProgInfo.ScanParams.hdr.wScanType;
		}
		else{
			return;
		}
	}

	pMainFrame->SetSelectListName(szName);

	switch(iHeadType){
	case HEAD_TYPE_SR:
		switch(iMeasType){
		case MEAS_PROG_TYPE_SR_THICKNESS:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SR_THICKNESS_WND);
			break;
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SR_REFLECTANCE_WND);
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.10.27 bagus Gantry 追加修正 --{--
// 2010.01.07 bagus Gantry --{--
//		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2010.01.07 bagus Gantry --}--
// 2009.10.27 bagus Gantry 追加修正 --}--
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SR_TRANSMITTANCE_WND);
			break;
// 2010.01.07 bagus Gantry --{--
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SR_GANTRY_WND);
			break;
// 2010.01.07 bagus Gantry --}--
		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SR_CIE_REFLECTANCE_WND);
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND);
			break;
		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SR_OD_WND);
			break;
// 2009.10.13 bagus Distance 追加 --{--
		case MEAS_PROG_TYPE_SR_DISTANCE:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SR_DISTANCE_WND);
			break;
// 2009.10.13 bagus Distance 追加 --}--
		}
		break;
	case HEAD_TYPE_SE:
		switch(iMeasType){
		case MEAS_PROG_TYPE_SE_THICKNESS:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_SE_THICKNESS_WND);
			break;
		}
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		switch(iMeasType){
		case MEAS_PROG_TYPE_COMPEASE_THICKNESS:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_COMPEASE_THICKNESS_WND);
			break;
		}
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

// 2009.10.19 bagus MS 追加 --{--
#if 0
	case HEAD_TYPE_IRSE:
		break;
#else
	case HEAD_TYPE_MS:
		switch(iMeasType){
		case MEAS_PROG_TYPE_MS_MEAS:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_MS_WND);
			break;
		}
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:
		switch(iMeasType){
		case MEAS_PROG_TYPE_4PP_MEAS:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_4PP_WND);
			break;
		}
		break;
	case HEAD_TYPE_CTA:
// 2009.10.07 bagus CTA 追加 --{--
		switch(iMeasType){
		case MEAS_PROG_TYPE_CTA_MEAS:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_CTA_WND);
			break;
		}
// 2009.10.07 bagus CTA 追加 --}--
		break;
	case HEAD_TYPE_STRESS:
		switch(iMeasType){
		case MEAS_PROG_TYPE_STRESS_MEAS:
			pMainFrame->ChangeExeWnd(RECIPE_MEAS_STRESS_WND);
			break;
		}
		break;
	default:
		break;
	}
}

// =========================================================================
//
BOOL CRecipeMeasurementProgramListView::DeleteRecipeData(LPCTSTR pszRecipeName)
{
	return RecipeFile_DeleteRecipe(pszRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM);
}

// =========================================================================
//
BOOL CRecipeMeasurementProgramListView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_PROGRAM);
}
