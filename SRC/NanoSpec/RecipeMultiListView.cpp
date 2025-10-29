// RecipeMultiListView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "RecipeMultiListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMultiListView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMultiListView, CNanoRecipeListUI)

// =========================================================================
//
CRecipeMultiListView::CRecipeMultiListView()
{
}

// =========================================================================
//
CRecipeMultiListView::~CRecipeMultiListView()
{
}

BEGIN_MESSAGE_MAP(CRecipeMultiListView, CNanoRecipeListUI)
	//{{AFX_MSG_MAP(CRecipeMultiListView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
//
void CRecipeMultiListView::List_HeaderSet()
{
	LV_COLUMN listcol;
	DWORD dwStyle = 0;
	int ItemMax;

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_LEFT };

	int Width[] = {
		0,		// (0) Index
		240,	// (1) Name
		140,	// (2) Date
		660,	// (3) Comment
	};

	char* Item[] = {
		"",
		"Name",
		"Date",
		"Comment",
	};

	ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		listcol.pszText = Item[i];
		CNanoListUI::rfListCtrl().InsertColumn(i,&listcol);
	}
}

// =========================================================================
//
void CRecipeMultiListView::List_DataSet()
{
	CFileFind Finder;
	CString strBuffer;
	char szDirPath[_MAX_PATH];
	char szName[RECIPE_NAME_LEN + 1];
	char szDate[256];
	BOOL bLoop;
	CTime time;
	MULTI_RCP_INFO_HDR MultiRcpInfoHdr;
/* added 2016.08.11 hmenjo �ő僌�V�s������ ---------- { ---------- */
	DWORD l_dwDataCount = 0;
/* added 2016.08.11 hmenjo �ő僌�V�s������ ---------- } ---------- */

	CNanoListUI::rfListCtrl().DeleteAllItems();
	sprintf(szDirPath, "%s*%s", g_szDb_Multi_Recipe_Dir, MULTIRECIPE_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		memset(&MultiRcpInfoHdr, 0, sizeof(MultiRcpInfoHdr));
		// �t�@�C�������擾
	strBuffer = Finder.GetFileTitle();
	if ( strBuffer[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
		continue;
	sprintf(szName, "%s", (LPCSTR)strBuffer);
	// �ŏI�ύX�������擾
	Finder.GetLastWriteTime(time);
	sprintf(szDate, "%s", (LPCSTR)time.Format("%Y.%m.%d %H:%M:%S"));

		if(RecipeFile_LoadRecipe(&MultiRcpInfoHdr, szName, RECIPE_FILE_MULTI_RECIPE)){
			List_Add(szName, szDate, MultiRcpInfoHdr.hdr.szComment);
		}
/* added 2016.08.11 hmenjo �ő僌�V�s������ ---------- { ---------- */
		l_dwDataCount++;
		if (RCP_PGM_NUM_MAX <= l_dwDataCount) {
			break;
		}
/* added 2016.08.11 hmenjo �ő僌�V�s������ ---------- } ---------- */
	}
}

// =========================================================================
//
void CRecipeMultiListView::List_Add(char* szName, char* szDate, char* szComment)
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

	///// Date /////
	listitem.iSubItem = 2;
	listitem.pszText = szDate;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Comment /////
	listitem.iSubItem = 3;
	csComment.Replace("\n", " ");
	listitem.pszText = csComment.GetBuffer(0);
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	CNanoListUI::rfListCtrl().SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

// =========================================================================
//
void CRecipeMultiListView::RecipeEntry(BOOL bNew)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	char szName[RECIPE_NAME_LEN + 1];
	int iIndex;

	pMainFrame->m_bNew = bNew;
	pMainFrame->SetSelectListName("");

	if(bNew){
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

	pMainFrame->ChangeExeWnd(RECIPE_MULTI_WND);
}


// =========================================================================
//
BOOL CRecipeMultiListView::DeleteRecipeData(LPCTSTR pszRecipeName)
{
	return RecipeFile_DeleteRecipe(pszRecipeName, RECIPE_FILE_MULTI_RECIPE);
}

// =========================================================================
//
BOOL CRecipeMultiListView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_RECIPE);
}
