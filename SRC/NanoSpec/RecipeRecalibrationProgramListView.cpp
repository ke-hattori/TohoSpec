// RecipeRecalibrationProgramListView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SelectMeasurementTypeDlg.h"
#include "MultiLangHelper.h"
#include "RecipeRecalibrationProgramListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeRecalibrationProgramListView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeRecalibrationProgramListView, CNanoRecipeListUI)

// =========================================================================
//
CRecipeRecalibrationProgramListView::CRecipeRecalibrationProgramListView()
{
	//{{AFX_DATA_INIT(CRecipeRecalibrationProgramListView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeRecalibrationProgramListView::~CRecipeRecalibrationProgramListView()
{
}

BEGIN_MESSAGE_MAP(CRecipeRecalibrationProgramListView, CNanoRecipeListUI)
	//{{AFX_MSG_MAP(CRecipeRecalibrationProgramListView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
//
void CRecipeRecalibrationProgramListView::List_HeaderSet()
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
////Saiki 20090527 Change ----->
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
//
////Saiki 20090527 Change <-----
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
void CRecipeRecalibrationProgramListView::List_DataSet()
{
	CFileFind Finder;
	CString strBuffer;
	char szDirPath[_MAX_PATH];
	char szName[RECIPE_NAME_LEN + 1];
	char szDate[256];
	BOOL bLoop;
	CTime time;
	RECALIB_PROG_INFO RecalibProgInfo;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- { ---------- */
	DWORD l_dwDataCount = 0;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- } ---------- */

	CNanoListUI::rfListCtrl().DeleteAllItems();
	sprintf(szDirPath, "%s*%s", g_szDb_Recalibration_Program_Dir, RECALIBPGM_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		memset(&RecalibProgInfo, 0, sizeof(RecalibProgInfo));
		// ファイル名を取得
		strBuffer = Finder.GetFileTitle();
		if ( strBuffer[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		sprintf(szName, "%s", strBuffer);
		// 最終変更日時を取得
		Finder.GetLastWriteTime(time);
		sprintf(szDate, "%s", time.Format("%Y.%m.%d %H:%M:%S"));

		if(RecipeFile_LoadRecipe(&RecalibProgInfo, szName, RECIPE_FILE_RECALIBRATION_PROGRAM)){
			List_Add(szName, (int)RecalibProgInfo.wHeadType,
				(int)RecalibProgInfo.wScanType, szDate, RecalibProgInfo.hdr.szComment);
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
void CRecipeRecalibrationProgramListView::List_Add(char* szName, int iHeadType, int iMeasType, char* szDate, char* szComment)
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
void CRecipeRecalibrationProgramListView::RecipeEntry(BOOL bNew)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_RECALIBRATION_PROGRAM_LIST);
	CSelectMeasurementTypeDlg dlg;
	char szName[RECIPE_NAME_LEN + 1];
	int iIndex;

	memset(szName, 0, sizeof(szName));
	pMainFrame->m_bNew = bNew;
	pMainFrame->SetSelectListName("");

// 2009.10.29 bagus 2点間 修正 --{--
	dlg.m_bRecalibration = TRUE;
// 2009.10.29 bagus 2点間 修正 --}--

	if(bNew){
		if(dlg.DoModal() == IDOK){
			// 2009.10.20 bagus MS 追加 --{--
			if (dlg.m_iHeadType == HEAD_TYPE_MS) {
				// MSにリキャリブレーションはない
				return;
			}
			// 2009.10.20 bagus MS 追加 --}--
			// 2009.10.24 bagus 2点間 修正 --{--
			if ((dlg.m_iHeadType == HEAD_TYPE_SR) && (dlg.m_iMeasType == MEAS_PROG_TYPE_SR_DISTANCE)) {
				// Distanceにリキャリブレーションはない
				return;
			}
			// 2009.10.24 bagus 2点間 修正 --}--
			pMainFrame->SetSelectHeadType(dlg.m_iHeadType);
			pMainFrame->SetSelectMeasurementType(dlg.m_iMeasType);
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
	}

	pMainFrame->SetSelectListName(szName);

	pMainFrame->ChangeExeWnd(RECIPE_RECALIBRATION_WND);
}

// =========================================================================
//
BOOL CRecipeRecalibrationProgramListView::DeleteRecipeData(LPCTSTR pszRecipeName)
{
	return RecipeFile_DeleteRecipe(pszRecipeName, RECIPE_FILE_RECALIBRATION_PROGRAM);
}


// =========================================================================
//
BOOL CRecipeRecalibrationProgramListView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_PROGRAM);
}
