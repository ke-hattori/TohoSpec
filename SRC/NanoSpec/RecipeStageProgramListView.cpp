// RecipeStageProgramListView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SelectSampleTypeDlg.h"
#include "SelectHeadTypeDlg.h"
//2009.09.25 bagus gantry --{--
#include "SelectStageProgramTypeDlg.h"
//2009.09.25 bagus gantry --}--
#include "RecipeStageProgramListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeStageProgramListView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeStageProgramListView, CNanoRecipeListUI)

// =========================================================================
//
CRecipeStageProgramListView::CRecipeStageProgramListView()
{
}

// =========================================================================
//
CRecipeStageProgramListView::~CRecipeStageProgramListView()
{
}

BEGIN_MESSAGE_MAP(CRecipeStageProgramListView, CNanoRecipeListUI)
	//{{AFX_MSG_MAP(CRecipeStageProgramListView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
//
void CRecipeStageProgramListView::List_HeaderSet()
{
	LV_COLUMN listcol;
	DWORD dwStyle = 0;
	int ItemMax;

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_LEFT };

	int Width[] = {
		0,		// (0) Index
		240,	// (1) Name
		70, 	// (2) Point
		160,	// (3) Sample
		140,	// (7) Date
		425,	// (8) Comment
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
////		//"",
////		//"Name",
////		//"Point",
////		//"Sample",
////		//"Date",
////		//"Comment",
//		//"",
////	  "名前",
////	  "ポイント",
////	  "サンプル",
////	  "データ",
////	  "コメント",
////  };
//	  char* ItemJPN[] = {
//		  "",
//		  "名前",
//		  "ポイント",
//		  "サンプル",
//		  "データ",
//		  "コメント",
//	  };
//	  char* ItemENU[] = {
//		"",
//		"Name",
//		"Point",
//		"Sample",
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
	LoadStringML(IDS_LSTHDR_POINT,		l_straHdr.ElementAt(2), _T("Point"));
	LoadStringML(IDS_LSTHDR_SAMPLE,		l_straHdr.ElementAt(3), _T("Sample"));
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
void CRecipeStageProgramListView::List_DataSet()
{
	CFileFind Finder;
	CString strBuffer;
	char szDirPath[_MAX_PATH];
	char szName[RECIPE_NAME_LEN + 1];
	char szDate[256];
	BOOL bLoop;
	CTime time;
	STAGE_PROG_INFO_HDR StageProgInfoHdr;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- { ---------- */
	DWORD l_dwDataCount = 0;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- } ---------- */

	CNanoListUI::rfListCtrl().DeleteAllItems();
	sprintf(szDirPath, "%s*%s", g_szDb_Stage_Program_Dir, STAGEPGM_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		memset(&StageProgInfoHdr, 0, sizeof(StageProgInfoHdr));
		// ファイル名を取得
		strBuffer = Finder.GetFileTitle();
		if ( strBuffer[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		sprintf(szName, "%s", strBuffer);
		// 最終変更日時を取得
		Finder.GetLastWriteTime(time);
		sprintf(szDate, "%s", time.Format("%Y.%m.%d %H:%M:%S"));

		if(RecipeFile_LoadRecipe(&StageProgInfoHdr, szName, RECIPE_FILE_STAGE_PROGRAM)){
			List_Add(szName, (int)StageProgInfoHdr.wNumScans, StageProgInfoHdr.SampleInfo.szName, szDate, StageProgInfoHdr.hdr.szComment);
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
void CRecipeStageProgramListView::List_Add(char* szName, int iPoint, char *szSample, char* szDate, char* szComment)
{
	LV_ITEM listitem;
	int item;
	char Buff[10];
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

	///// Point /////
	listitem.iSubItem = 2;
	sprintf(Buff, "%d", iPoint);
	listitem.pszText = Buff;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Sample Name /////
	listitem.iSubItem = 3;
	listitem.pszText = szSample;
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
void CRecipeStageProgramListView::RecipeEntry(BOOL bNew)
{
// 2009.10.09 bagus StagePGM 共通化 --{--
#if 0
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_STAGE_PROGRAM_LIST);
	CSelectSampleTypeDlg dlg;
	char szName[RECIPE_NAME_LEN + 1];
	char szSampleTypeName[RECIPE_NAME_LEN + 1];
	int iIndex;
	//2009.08.28 bagus stress --{--
	// ヘッドタイプ種別
	int iHeadType;
	//2009.08.28 bagus stress --}--
	// 2009.10.02 bagus Gantry --{--
	int	iScanType;
	// 2009.10.02 bagus Gantry --}--

	memset(szName, 0, sizeof(szName));
	memset(szSampleTypeName, 0, sizeof(szSampleTypeName));
	pMainFrame->m_bNew = bNew;
	pMainFrame->SetSelectListName("");

	if(bNew){
		//Select Head Type Dialog
		CSelectHeadTypeDlg dlgHeadType;
//2009.09.25 bagus gantry --{--
		CSelectStageProgramTypeDlg dlgStagePGMType;
//2009.09.25 bagus gantry --}--

// 2009.10.02 bagus Gantry --{--
#if 0
		if(dlgHeadType.DoModal() != IDOK){
			return ;
		}
		iHeadType = dlgHeadType.m_iHeadType;
#else
		if(dlgStagePGMType.DoModal() != IDOK){
			return ;
		}
		iHeadType = dlgStagePGMType.m_iHeadType;
#endif
// 2009.10.02 bagus Gantry --}--

		//設定されているサンプルが1種類のみの場合は選択画面を表示しない
		CFileFind Finder;
		char szDirPath[_MAX_PATH];
		CString csName;
		BOOL bLoop;
		int iItemCount=0;

		sprintf(szDirPath, "%s*%s", g_szCfg_System_Sample_Dir, DAT_EXT);
		bLoop = Finder.FindFile(szDirPath);

		while(bLoop){
			bLoop = Finder.FindNextFile();

			if(Finder.IsDots()){
				continue;
			}

			csName = Finder.GetFileTitle();
			iItemCount++;
		}
		//2009.08.28 bagus stress --{--
		//ストレスの時にはチェックがあるので自動選択させない
		dlg.m_iHeadType = iHeadType;
		//if (iItemCount==1){
		if(iItemCount == 1 && iHeadType != HEAD_TYPE_STRESS){
		//2009.08.28 bagus stress --}--
			csName.Insert(RECIPE_NAME_LEN, '\0');

			pMainFrame->SetSelectSampleTypeName(csName);
		}
		//選択画面を表示
		else{
			if(dlg.DoModal() == IDCANCEL){
				return;
			}
			pMainFrame->SetSelectSampleTypeName(dlg.m_szName);
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
		//2009.08.28 bagus stress --{--
		//ヘッドタイプの取得用
		STAGE_PROG_INFO_HDR l_StageProgInfoHdr;
		if ( !RecipeFile_LoadRecipe(&l_StageProgInfoHdr, szName, RECIPE_FILE_STAGE_PROGRAM) ) {
			return;
		}
		iHeadType = l_StageProgInfoHdr.wHeadType;
		//2009.08.28 bagus stress --}--

		// 2009.10.02 bagus Gantry --{--
		iScanType = l_StageProgInfoHdr.wScanType;
		if (iScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G) {
			iHeadType = HEAD_TYPE_MAX;
		}
		// 2009.10.02 bagus Gantry --}--
	}

	pMainFrame->SetSelectListName(szName);

// 2009.09.29 bagus SE --{--
	pMainFrame->SetSelectHeadType(iHeadType);
// 2009.09.29 bagus SE --}--

	switch(iHeadType){
	case HEAD_TYPE_SR:
	default:
		pMainFrame->ChangeExeWnd(RECIPE_STAGE_WND);
		break;
	case HEAD_TYPE_STRESS:
		pMainFrame->ChangeExeWnd(RECIPE_STAGE_STRESS_WND);
		break;
// 2009.10.02 bagus Gantry --{--
	case HEAD_TYPE_MAX:
		pMainFrame->ChangeExeWnd(RECIPE_STAGE_GANTRY_WND);
		break;
// 2009.10.02 bagus Gantry --}--
	}
#else
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_STAGE_PROGRAM_LIST);
	CSelectSampleTypeDlg dlg;
	char szName[RECIPE_NAME_LEN + 1];
	char szSampleTypeName[RECIPE_NAME_LEN + 1];
	int iIndex;
	int iStagePGMType;
	int	iScanType;

	memset(szName, 0, sizeof(szName));
	memset(szSampleTypeName, 0, sizeof(szSampleTypeName));
	pMainFrame->m_bNew = bNew;
	pMainFrame->SetSelectListName("");

	if(bNew){
		//Select Head Type Dialog
		CSelectStageProgramTypeDlg dlgStagePGMType;

		if(dlgStagePGMType.DoModal() != IDOK){
			return ;
		}
		iStagePGMType = dlgStagePGMType.m_iHeadType;

		//設定されているサンプルが1種類のみの場合は選択画面を表示しない
		CFileFind Finder;
		char szDirPath[_MAX_PATH];
		CString csName;
		BOOL bLoop;
		int iItemCount=0;

		sprintf(szDirPath, "%s*%s", g_szCfg_System_Sample_Dir, DAT_EXT);
		bLoop = Finder.FindFile(szDirPath);

		while(bLoop){
			bLoop = Finder.FindNextFile();

			if(Finder.IsDots()){
				continue;
			}

			csName = Finder.GetFileTitle();
			iItemCount++;
		}
		//ストレスの時にはチェックがあるので自動選択させない
		dlg.m_iHeadType = iStagePGMType;
		if(iItemCount == 1 && iStagePGMType != STAGE_PGM_TYPE_STRESS){
			csName.Insert(RECIPE_NAME_LEN, '\0');

			pMainFrame->SetSelectSampleTypeName(csName);
		}
		//選択画面を表示
		else{
			//2009.12.24 bagus stress 修正 --{--
			if(iStagePGMType != STAGE_PGM_TYPE_STRESS){
				if(dlg.DoModal() == IDCANCEL){
					return;
				}
				pMainFrame->SetSelectSampleTypeName(dlg.m_szName);
			}else{
				//2009.12.24 bagus stress 修正 --}--
				//2009.12.23 bagus stress 修正 --{--
				//if(dlg.DoModal() == IDCANCEL){
				//	return;
				//}
				//pMainFrame->SetSelectSampleTypeName(dlg.m_szName);
				STRESS_CONFIG l_StressConfig;
				SAMPLEINFO l_SampleInfo;
				ConfigFile_GetNanoSpecIni(&l_StressConfig,CONFIG_FILE_STRESS_CONFIG);
				//pMainFrame->SetSelectSampleTypeName(dlg.m_szName);
				if(!RecipeFile_LoadRecipe(&l_SampleInfo,l_StressConfig.szSampleName,RECIPE_FILE_SAMPLE)){
					//ストレスラインセクション設定の中のサンプル名が向こう
					CString l_strBuffer = "";
					LoadStringML(IDS_SAMPLE_NAME_INVALID,l_strBuffer,"Sample Name is invalid.");
					MessageBox(l_strBuffer,"STAGE PGM",MB_OK|MB_ICONWARNING);
					return;
				}
				pMainFrame->SetSelectSampleTypeName(l_StressConfig.szSampleName);
			}
			//2009.12.23 bagus stress 修正 --}--
			//2009.12.24 bagus stress 修正 --}--
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
		//ヘッドタイプの取得用
		STAGE_PROG_INFO_HDR l_StageProgInfoHdr;
		if ( !RecipeFile_LoadRecipe(&l_StageProgInfoHdr, szName, RECIPE_FILE_STAGE_PROGRAM) ) {
			return;
		}
		iStagePGMType = l_StageProgInfoHdr.wHeadType;

		iScanType = l_StageProgInfoHdr.wScanType;
		if (iStagePGMType == 0) {
			switch (iScanType) {
			case SCAN_TYPE_SR_GANTRY:
				iStagePGMType = STAGE_PGM_TYPE_SR_GANTRY;
				break;
			case SCAN_TYPE_SR_DISTANCE:
				iStagePGMType = STAGE_PGM_TYPE_SR_DISTANCE;
				break;
			default:
				iStagePGMType = STAGE_PGM_TYPE_STANDARD;
				break;
			}
		}
	}

	pMainFrame->SetSelectListName(szName);

	pMainFrame->SetSelectHeadType(iStagePGMType);

	switch(iStagePGMType){
	case STAGE_PGM_TYPE_STANDARD:
	default:
		pMainFrame->ChangeExeWnd(RECIPE_STAGE_WND);
		break;
	case STAGE_PGM_TYPE_STRESS:
		pMainFrame->ChangeExeWnd(RECIPE_STAGE_STRESS_WND);
		break;
	case STAGE_PGM_TYPE_SR_GANTRY:
		pMainFrame->ChangeExeWnd(RECIPE_STAGE_GANTRY_WND);
		break;
	case STAGE_PGM_TYPE_SR_DISTANCE:
		pMainFrame->ChangeExeWnd(RECIPE_STAGE_SR_DISTANCE_WND);
		break;
	}
#endif
// 2009.10.09 bagus StagePGM 共通化 --}--
}

// =========================================================================
//
BOOL CRecipeStageProgramListView::DeleteRecipeData(LPCTSTR pszRecipeName)
{
	return RecipeFile_DeleteRecipe(pszRecipeName, RECIPE_FILE_STAGE_PROGRAM);
}

// =========================================================================
//
BOOL CRecipeStageProgramListView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_PROGRAM);
}
