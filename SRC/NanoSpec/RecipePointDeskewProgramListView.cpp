// RecipePointDeskewProgramListView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "SelectHeadTypeDlg.h"
#include "RecipePointDeskewProgramListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipePointDeskewProgramListView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipePointDeskewProgramListView, CNanoRecipeListUI)

// =========================================================================
//
CRecipePointDeskewProgramListView::CRecipePointDeskewProgramListView()
{
}

// =========================================================================
//
CRecipePointDeskewProgramListView::~CRecipePointDeskewProgramListView()
{
}

BEGIN_MESSAGE_MAP(CRecipePointDeskewProgramListView, CNanoRecipeListUI)
	//{{AFX_MSG_MAP(CRecipePointDeskewProgramListView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
//
void CRecipePointDeskewProgramListView::List_HeaderSet()
{
	LV_COLUMN listcol;
	DWORD dwStyle = 0;
	int ItemMax;

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };

	int Width[] = {
		0,		// (0) Index
// 2009.12.09 bagus --{--
//		  240,	  // (1) Name
//		  70,	  // (2) Head Type
		200,	// (1) Name
		110,	 // (2) Head Type
// 2009.12.09 bagus --}--
		140,	// (3) Date
		605,	// (4) Comment
	};

/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//// Kojika 20090527 Add
//	  SYSTEM_CONFIG l_SystemConfig;
//	  ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
//// Kojika 20090527 Add End
*/
/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//// Kojika 20090527 Change
////  char* Item[] = {
//////	  "",
//////	  "Name",
//////	  "Head",
//////	  "Date",
//////	  "Comment",
////	  "",
////	  "名前",
////	  "ヘッド",
////	  "データ",
////	  "コメント",
////  };
//	  char* ItemJPN[] = {
//		  "",
//		  "名前",
//		  "ヘッド",
//		  "データ",
//		  "コメント",
//	  };
//	  char* ItemENU[] = {
//		"",
//		"Name",
//		"Head",
//		"Date",
//		"Comment",
//	  };
//// Kojika 20090527 Change End
*/
/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

	ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);
/* added 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
	CStringArray l_straHdr;
	l_straHdr.SetSize(ItemMax);
	l_straHdr.SetAt(0, _T(""));
	LoadStringML(IDS_LSTHDR_NAME,		l_straHdr.ElementAt(1), _T("Name"));
	LoadStringML(IDS_LSTHDR_HEAD,		l_straHdr.ElementAt(2), _T("Head"));
	LoadStringML(IDS_LSTHDR_DATE,		l_straHdr.ElementAt(3), _T("Date"));
	LoadStringML(IDS_LSTHDR_COMMENT,	l_straHdr.ElementAt(4), _T("Comment"));
/* added 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//// Kojika 20090527 Change
//		  //listcol.pszText = Item[i];
//		  if(l_SystemConfig.nLanguage == 0){
//			  listcol.pszText = ItemENU[i];
//		  }
//		  else{
//			  listcol.pszText = ItemJPN[i];
//		  }
//// Kojika 20090527 Change End
*/
/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ----------			   */
		listcol.pszText = ((LPTSTR) (LPCTSTR) l_straHdr.GetAt(i));
/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */
		CNanoListUI::rfListCtrl().InsertColumn(i,&listcol);
	}
}

// =========================================================================
//
void CRecipePointDeskewProgramListView::List_DataSet()
{
	CFileFind Finder;
	CString strBuffer;
	char szDirPath[_MAX_PATH];
	char szName[RECIPE_NAME_LEN + 1];
	char szDate[256];
	BOOL bLoop;
	CTime time;
	POINT_DESKEW_PROG_INFO PointDeskewProgInfo;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- { ---------- */
	DWORD l_dwDataCount = 0;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- } ---------- */

	CNanoListUI::rfListCtrl().DeleteAllItems();
	sprintf(szDirPath, "%s*%s", g_szDb_Point_Deskew_Program_Dir, POINTDESKEWPGM_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		memset(&PointDeskewProgInfo, 0, sizeof(PointDeskewProgInfo));
		// ファイル名を取得
		strBuffer = Finder.GetFileTitle();
		if ( strBuffer[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		sprintf(szName, "%s", strBuffer);
		// 最終変更日時を取得
		Finder.GetLastWriteTime(time);
		sprintf(szDate, "%s", time.Format("%Y.%m.%d %H:%M:%S"));

		if(RecipeFile_LoadRecipe(&PointDeskewProgInfo, szName, RECIPE_FILE_POINT_DESKEW_PROGRAM)){
			List_Add(szName, (int)PointDeskewProgInfo.wHeadType,
				szDate, PointDeskewProgInfo.hdr.szComment);
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
void CRecipePointDeskewProgramListView::List_Add(char* szName, int iHeadType, char* szDate, char* szComment)
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

	///// Date /////
	listitem.iSubItem = 3;
	listitem.pszText = szDate;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Comment /////
	listitem.iSubItem = 4;
	csComment.Replace("\n", " ");
	listitem.pszText = csComment.GetBuffer(0);
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	CNanoListUI::rfListCtrl().SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

// =========================================================================
//
void CRecipePointDeskewProgramListView::RecipeEntry(BOOL bNew)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_POINT_DESKEW_PROGRAM_LIST);
	CSelectHeadTypeDlg dlg;
	char szName[RECIPE_NAME_LEN + 1];
	int iIndex;

	memset(szName, 0, sizeof(szName));
	pMainFrame->m_bNew = bNew;
	pMainFrame->SetSelectListName("");

	if(bNew){
		if(dlg.DoModal() == IDOK){
			pMainFrame->SetSelectHeadType(dlg.m_iHeadType);
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

		CNanoListUI::rfListCtrl().GetItemText(iIndex, 1, szName, sizeof(szName));

		if(szName[0] == '\0'){
			return;
		}
	}

	pMainFrame->SetSelectListName(szName);

	pMainFrame->ChangeExeWnd(RECIPE_POINT_DESKEW_WND);
}

// =========================================================================
//
BOOL CRecipePointDeskewProgramListView::DeleteRecipeData(LPCTSTR pszRecipeName)
{
	return RecipeFile_DeleteRecipe(pszRecipeName, RECIPE_FILE_POINT_DESKEW_PROGRAM);
}

// =========================================================================
//
BOOL CRecipePointDeskewProgramListView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_PROGRAM);
}
