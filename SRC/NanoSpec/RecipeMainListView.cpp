// RecipeMainListView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SelectHeadTypeDlg.h"
#include "RecipeMainListView.h"
#include "MultiLangHelper.h"
#include "..\\..\\INC\\PifComm.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMainListView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMainListView, CNanoRecipeListUI)

// =========================================================================
//
CRecipeMainListView::CRecipeMainListView()
{
}

// =========================================================================
//
CRecipeMainListView::~CRecipeMainListView()
{
}

BEGIN_MESSAGE_MAP(CRecipeMainListView, CNanoRecipeListUI)
	//{{AFX_MSG_MAP(CRecipeMainListView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
//
void CRecipeMainListView::List_HeaderSet()
{

/* deleted 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ---------- { ---------- */
//	  //Saiki 20090527 Add ----->
//	  SYSTEM_CONFIG l_SystemConfig;
//	  ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
//	  //Saiki 20090527 Add <-----
/* deleted 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ---------- } ---------- */
	LV_COLUMN listcol;
	DWORD dwStyle = 0;
	int ItemMax;

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };

// 2009.11.17 bagus RS �C�� --{--
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
		380,	// (5) Comment
	};
#endif
// 2009.11.17 bagus RS �C�� --}--

/* deleted 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ---------- { ---------- */
/*
////Saiki 20090526 Change ----->
////	char* Item[] = {
//////		"",
//////		"Name",
//////		"Head",
//////		"Meas Item",
//////		"Date",
//////		"Comment",
////	  "",
////		"���O",
////		"�w�b�h",
////		"������",
////		"�f�[�^",
////		"�R�����g",
////	};
//	  char* ItemJPN[] = {
//		  "",
//		  "���O",
//		  "�w�b�h",
//		  "������",
//		  "�f�[�^",
//		  "�R�����g",
//	  };
//	  char* ItemENU[] = {
//		"",
//		"Name",
//		"Head",
//		"Meas Item",
//		"Date",
//		"Comment",
//	  };
//
////Saiki 20090526 Change <-----
*/
/* deleted 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ---------- } ---------- */

	ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);
/* added 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ---------- { ---------- */
	CStringArray l_straHdr;
	l_straHdr.SetSize(ItemMax);
	l_straHdr.SetAt(0, _T(""));
	LoadStringML(IDS_LSTHDR_NAME,		l_straHdr.ElementAt(1), _T("Name"));
	LoadStringML(IDS_LSTHDR_HEAD,		l_straHdr.ElementAt(2), _T("Head"));
	LoadStringML(IDS_LSTHDR_MEASITEM,	l_straHdr.ElementAt(3), _T("Meas Item"));
	LoadStringML(IDS_LSTHDR_DATE,		l_straHdr.ElementAt(4), _T("Date"));
	LoadStringML(IDS_LSTHDR_COMMENT,	l_straHdr.ElementAt(5), _T("Comment"));
/* added 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ---------- } ---------- */

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
/* modified 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ---------- { ---------- */
/*
//		  //Saiki 20090527 Change ----->
//		  //listcol.pszText = Item[i];
//		  if(l_SystemConfig.nLanguage == 0){
//			  listcol.pszText = ItemENU[i];
//		  }
//		  else{
//			  listcol.pszText = ItemJPN[i];
//		  }
*/
/* modified 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ----------			   */
		listcol.pszText = ((LPTSTR) (LPCTSTR) l_straHdr.GetAt(i));
/* modified 2009.07.06 hmenjo ���X�g�w�b�_�������\�[�X�o�^ ---------- } ---------- */
		CNanoListUI::rfListCtrl().InsertColumn(i,&listcol);
	}

}

// =========================================================================
//
void CRecipeMainListView::List_DataSet()
{
	CFileFind Finder;
	CString strBuffer;
	char szDirPath[_MAX_PATH];
	char szName[RECIPE_NAME_LEN + 1];
	char szDate[256];
	BOOL bLoop;
	CTime time;
	MAIN_RCP_INFO MainRcpInfo;
	MEAS_PROG_INFO MeasProgInfo;
/* added 2016.08.11 hmenjo �ő僌�V�s������ ---------- { ---------- */
	DWORD l_dwDataCount = 0;
/* added 2016.08.11 hmenjo �ő僌�V�s������ ---------- } ---------- */

	CNanoListUI::rfListCtrl().DeleteAllItems();
	sprintf(szDirPath, "%s*%s", g_szDb_Main_Recipe_Dir, MAINRECIPE_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		memset(&MainRcpInfo, 0, sizeof(MainRcpInfo));
		memset(&MeasProgInfo, 0, sizeof(MeasProgInfo));
		// �t�@�C�������擾
	strBuffer = Finder.GetFileTitle();
	if ( strBuffer[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
		continue;
	sprintf(szName, "%s", (LPCSTR)strBuffer);
	// �ŏI�ύX�������擾
	Finder.GetLastWriteTime(time);
	sprintf(szDate, "%s", (LPCSTR)time.Format("%Y.%m.%d %H:%M:%S"));

		if(RecipeFile_LoadRecipe(&MainRcpInfo, szName, RECIPE_FILE_MAIN_RECIPE)){
			// MeasurementProgram��Load�ł��Ȃ��Ƃ���MeasItem�͋󔒂Ƃ���
			if(RecipeFile_LoadRecipe(&MeasProgInfo, MainRcpInfo.MainRcpParam.hdr.szMeas,
				RECIPE_FILE_MEASUREMENT_PROGRAM)){
				List_Add(szName, (int)MainRcpInfo.MainRcpParam.hdr.wHeadType,
					(int)MeasProgInfo.ScanParams.hdr.wScanType, szDate, MainRcpInfo.hdr.szComment);
			}
			else{
				List_Add(szName, (int)MainRcpInfo.MainRcpParam.hdr.wHeadType,
					-1, szDate, MainRcpInfo.hdr.szComment);
			}
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
void CRecipeMainListView::List_Add(char* szName, int iHeadType, int iMeasType, char* szDate, char* szComment)
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
	if(iMeasType == -1){
		listitem.pszText = "";
		CNanoListUI::rfListCtrl().SetItem(&listitem);
	}
	else{
		CMLScanType mlScanTypeItem(iHeadType);
		listitem.pszText = (LPTSTR)((LPCTSTR)mlScanTypeItem[iMeasType]);
		CNanoListUI::rfListCtrl().SetItem(&listitem);
	}

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
//	CNanoListUI::rfListCtrl().EnsureVisible(item, FALSE);
}

// =========================================================================
//
void CRecipeMainListView::RecipeEntry(BOOL bNew)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CSelectHeadTypeDlg dlg;

	int iHeadType = 0;
	int iIndex;
	char szName[RECIPE_NAME_LEN + 1];
	MAIN_RCP_INFO MainRcpInfo;

	memset(szName, 0, sizeof(szName));
	pMainFrame->m_bNew = bNew;
	pMainFrame->SetSelectListName("");

	if(bNew){
		///// Load System Config File /////
		HEAD_TYPE_CONFIG HeadType;
		ConfigFile_GetNanoSpecIni(&HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);

		//�ݒ�w�b�h���P�@��݂̂̏ꍇ�̓w�b�h�I����ʂ͕\�����Ȃ�
// 2009.10.19 bagus MS �ǉ� --{--
#if 0
		if((HeadType.bSR && !HeadType.bSE && !HeadType.bIRSE && !HeadType.bResist && !HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && HeadType.bSE && !HeadType.bIRSE && !HeadType.bResist && !HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && !HeadType.bSE && HeadType.bIRSE && !HeadType.bResist && !HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && !HeadType.bSE && !HeadType.bIRSE && HeadType.bResist && !HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && !HeadType.bSE && !HeadType.bIRSE && !HeadType.bResist && HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && !HeadType.bSE && !HeadType.bIRSE && !HeadType.bResist && !HeadType.bCTA && HeadType.bStress) ){
#else
		if((HeadType.bSR && !HeadType.bSE && !HeadType.bMS && !HeadType.bResist && !HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && HeadType.bSE && !HeadType.bMS && !HeadType.bResist && !HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && !HeadType.bSE && HeadType.bMS && !HeadType.bResist && !HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && !HeadType.bSE && !HeadType.bMS && HeadType.bResist && !HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && !HeadType.bSE && !HeadType.bMS && !HeadType.bResist && HeadType.bCTA && !HeadType.bStress) ||
			(!HeadType.bSR && !HeadType.bSE && !HeadType.bMS && !HeadType.bResist && !HeadType.bCTA && HeadType.bStress) ){
#endif
// 2009.10.19 bagus MS �ǉ� --}--

				// �ݒ肳��Ă���B��̃w�b�h��ݒ�
				if(HeadType.bSR)
					iHeadType=HEAD_TYPE_SR;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
// 				if(HeadType.bSE)
// 					iHeadType=HEAD_TYPE_SE;
				if(HeadType.bSE) {
					if(HeadType.bCompEASE) {
						iHeadType=HEAD_TYPE_COMPEASE;
					}
					else {
					iHeadType=HEAD_TYPE_SE;
					}
				}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.19 bagus MS �ǉ� --{--
#if 0
				if(HeadType.bIRSE)
					iHeadType=HEAD_TYPE_IRSE;
#else
				if(HeadType.bMS)
					iHeadType=HEAD_TYPE_MS;
#endif
// 2009.10.19 bagus MS �ǉ� --}--
				if(HeadType.bResist)
					iHeadType=HEAD_TYPE_4PP;
				if(HeadType.bCTA)
					iHeadType=HEAD_TYPE_CTA;
				if(HeadType.bStress)
					iHeadType=HEAD_TYPE_STRESS;
		}
		else{
			//�w�b�h�I����ʕ\��
			if(dlg.DoModal() == IDOK){
				iHeadType = dlg.m_iHeadType;
			}
			else{
				return;
			}
		}
	}
	else{
		iIndex = CNanoListUI::rfListCtrl().GetSelectionMark();

		if ( iIndex == -1 ){
			return;
		}

		CNanoListUI::rfListCtrl().GetItemText(iIndex, 1, szName, RECIPE_NAME_LEN + 1);

		if ( strlen(szName) == 0 )
			return;

		if ( RecipeFile_LoadRecipe(&MainRcpInfo, szName, RECIPE_FILE_MAIN_RECIPE) )
			iHeadType = (int)MainRcpInfo.MainRcpParam.hdr.wHeadType;
		else
			return;
	}

	pMainFrame->SetSelectListName(szName);

	switch(iHeadType){
	case HEAD_TYPE_SR:
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_SR_WND);
		break;
	case HEAD_TYPE_SE:
// 2009.09.19 bagus SE --{--
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_SE_WND);
// 2009.09.19 bagus SE --}--
		break;
// 2009.10.19 bagus MS �ǉ� --{--
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case HEAD_TYPE_COMPEASE:
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_COMPEASE_WND);
		break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
#if 0
	case HEAD_TYPE_IRSE:
		break;
#else
	case HEAD_TYPE_MS:
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_MS_WND);
		break;
#endif
// 2009.10.19 bagus MS �ǉ� --}--
	case HEAD_TYPE_4PP:
// 2009.11.04 bagus RS �ǉ� --{--
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_4PP_WND);
// 2009.11.04 bagus RS �ǉ� --}--
		break;
	case HEAD_TYPE_CTA:
// 2009.10.08 bagus CTA �ǉ� --{--
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_CTA_WND);
// 2009.10.08 bagus CTA �ǉ� --}--
		break;
	case HEAD_TYPE_STRESS:
		//2009.08.16 bagus stress --{--
		//���̌�ŕ��򂷂邽�߂ɕʂ�ID��n���i���̂�SR�Ɠ����j
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_STRESS_WND);
		//2009.08.16 bagus stress --}--
		break;
	default:
//		pMainFrame->ChangeExeWnd(RECIPE_MAIN_SR_WND);
		break;
	}
}

// =========================================================================
//
BOOL CRecipeMainListView::DeleteRecipeData(LPCTSTR pszRecipeName)
{
	RECIPENAMEDATESET recipeNameDataSet;

	if ( !RecipeFile_DeleteRecipe(pszRecipeName, RECIPE_FILE_MAIN_RECIPE))
		return FALSE;

	sprintf(recipeNameDataSet.szRecipeNameExt, "%s%s", pszRecipeName, MAINRECIPE_EXT);
	::GetLocalTime(&recipeNameDataSet.recipeDateTime);
	PifComm_ChangeRecipeDeleteReport(0, &recipeNameDataSet);


	//���C�����V�s�폜�Ɠ����Ƀ��t�@�����X�f�[�^���폜����
	char szFilePath[MAX_PATH];
	memset(szFilePath, 0, sizeof(szFilePath));
	sprintf(szFilePath, "%s%s%s", g_szData_Ref_Dir, pszRecipeName, DAT_EXT);
	DeleteFile(szFilePath);


	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMainListView::RecipeStart(LPTSTR pszRecipeName)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	char szName[RECIPE_NAME_LEN + 1];

	pMainFrame->SetSelectListName("");

	// ���X�g�̑I���s�擾
	int iIndex = ((CListCtrl*) GetDlgItem(IDC_MAIN_RECIPE_LIST))->GetSelectionMark();
	if ( iIndex == -1 )
		return FALSE;

	if ( !((CListCtrl*) GetDlgItem(IDC_MAIN_RECIPE_LIST))->GetItemText(iIndex, 1, szName, sizeof(szName)) )
		return FALSE;

	pMainFrame->SetSelectListName(szName);
	lstrcpy(pszRecipeName, szName);
	return TRUE;
}


BOOL CRecipeMainListView::RecipeHead(LPTSTR pszRecipeHead, LPTSTR pszRecipeScan)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
//	char szName[RECIPE_NAME_LEN + 1];
	char szHead[255+1];
	char szScan[255+1];
	memset(szHead, 0, sizeof(szHead));
	memset(szScan, 0, sizeof(szScan));

//	pMainFrame->SetSelectListName("");

	// ���X�g�̑I���s�擾
	int iIndex = ((CListCtrl*) GetDlgItem(IDC_MAIN_RECIPE_LIST))->GetSelectionMark();
	if ( iIndex == -1 )
		return FALSE;

	if ( !((CListCtrl*) GetDlgItem(IDC_MAIN_RECIPE_LIST))->GetItemText(iIndex, 2, szHead, sizeof(szHead)) )
		return FALSE;

	if ( !((CListCtrl*) GetDlgItem(IDC_MAIN_RECIPE_LIST))->GetItemText(iIndex, 3, szScan, sizeof(szScan)) )
		return FALSE;

//	pMainFrame->SetSelectListName(szName);
//	lstrcpy(pszRecipeName, szName);
	lstrcpy(pszRecipeHead, szHead);
	lstrcpy(pszRecipeScan, szScan);

	return TRUE;
}



// =========================================================================
//
BOOL CRecipeMainListView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_RECIPE);
}
