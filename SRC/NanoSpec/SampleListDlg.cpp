// SampleListDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SampleSettingDlg.h"
#include "SampleListDlg.h"

#define MICROMETRE				(1000L)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CALLBACK CSampleListDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam3)
{
	CSampleListDlg* pDlg = (CSampleListDlg*)lParam3;
	CListCtrl* pList = (CListCtrl*)pDlg->GetDlgItem(IDC_SAMPLE_LIST);
	int iCol = pDlg->m_iCol;
	BOOL bOrder = pDlg->m_bOrder[iCol];
	CString strBuffer1, strBuffer2;

	strBuffer1 = pList->GetItemText(lParam1, iCol);
	strBuffer2 = pList->GetItemText(lParam2, iCol);

	return bOrder ? strcmp(strBuffer1, strBuffer2) : strcmp(strBuffer2, strBuffer1);
}


// #########################################################################
// CSampleListDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSampleListDlg::CSampleListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSampleListDlg)
	//}}AFX_DATA_INIT
	memset(m_bOrder, 0, sizeof(m_bOrder));
}

// =========================================================================
//
CSampleListDlg::~CSampleListDlg()
{
}

// =========================================================================
//
void CSampleListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleListDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_NEW_BUTTON, m_NewButton);
	DDX_Control(pDX, IDC_EDIT_BUTTON, m_EditButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSampleListDlg, CDialog)
	//{{AFX_MSG_MAP(CSampleListDlg)
	ON_BN_CLICKED(IDC_NEW_BUTTON, OnNewButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_NOTIFY(NM_DBLCLK, IDC_SAMPLE_LIST, OnDblclkSampleList)
	ON_NOTIFY(NM_CLICK, IDC_SAMPLE_LIST, OnClickSampleList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SAMPLE_LIST, OnColumnclickSampleList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSampleListDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSampleListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	// 2009.08.26 bagus stress --{--
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// 2009.08.26 bagus stress --}--

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// New Button /////
	m_NewButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NewButton.DrawFlatFocus(TRUE);

	///// Edit Button /////
	m_EditButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_EditButton.DrawFlatFocus(TRUE);

	///// Delete Button /////
	m_DeleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeleteButton.DrawFlatFocus(TRUE);

	// リストコントロールの初期化
	m_lstCtrl.SubclassWindow(GetDlgItem(IDC_SAMPLE_LIST)->GetSafeHwnd());
	m_lstCtrl.SetViewStyle();

	SampleList_HeaderSet();
	SampleList_DataSet();

	Sort(1);

	if ( m_lstCtrl.GetItemCount() > 0 ) {
		m_lstCtrl.SetSelectionMark(0);
		m_lstCtrl.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSampleListDlg::SampleList_HeaderSet()
{
	LV_COLUMN listcol;
	int ItemMax;

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };
	//int Fmt[] = { LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER };
	// 2009.08.26 bagus stress --{--
	int FmtStress[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };
	// 2009.08.26 bagus stress --}--

	// 2009.08.26 bagus stress --{--
	//int Width[] = {
	//	0,		// (0) Index
	//	160,//240	// (1) Name
	//	65, 	// (2) Sample Shape
	//	80, 	// (2) Sample Size_X
	//	80, 	// (2) Sample Size_Y
	//	90,//65,		// (3) Sample Thickness
	//	142,	// (4) Date
	//};
	int Width[] = {
		0,		// (0) Index
		220,	// (1) Name
		65,		// (2) Sample Shape
		80,		// (2) Sample Size_X
		80,		// (2) Sample Size_Y
		90,		// (3) Sample Thickness
		152,	// (4) Date
	};
	int WidthStress[] = {
		0,		// (0) Index
		150,	// (1) Name
		65,		// (2) Sample Shape
		80,		// (2) Sample Size_X
		80,		// (2) Sample Size_Y
		90,		// (3) Sample Thickness
		100,	// (4) Elastic Modulus
		122,	// (5) Date
	};
	// 2009.08.26 bagus stress --}--

/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//	// Kojika 20090529 Add
//	  SYSTEM_CONFIG l_SystemConfig;
//	  ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
//	// Kojika 20090529 Add End
*/
/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//	// Kojika 20090529 Change
//	  //char* Item[] = {
//	//	//"",
//	//	//"Name",
//	//	//"Shape",
//	//	//"Size X[um]",
//	//	//"Size Y[um]",
//	//	//"Thickness[um]",
//	//	//"Date",
//		//"",
//	  //	"名前",
//	  //	"形状",
//	  //	"サイズ X[um]",
//	  //	"サイズ Y[um]",
//	  //	"膜厚[um]",
//	  //	"データ",
//	  //};
//	  char* Item_JPN[] = {
//		  "",
//		  "名前",
//		  "形状",
//		  "サイズ X[um]",
//		  "サイズ Y[um]",
//		  "膜厚[um]",
//		  "データ",
//	  };
//	  char* Item_ENU[] = {
//		"",
//		"Name",
//		"Shape",
//		"Size X[um]",
//		"Size Y[um]",
//		"Thickness[um]",
//		"Date",
//	  };
//	// Kojika 20090529 Change End
*/
/* deleted 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

	// 2009.08.26 bagus stress --{--
	//ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);
	if (m_SystemConfig.HeadType.bStress) {
		ItemMax = sizeof(FmtStress) / sizeof(FmtStress[0]);
	} else {
		ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);
	}
	// 2009.08.26 bagus stress --}--
/* added 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
	CStringArray l_straHdr;
	l_straHdr.SetSize(ItemMax);
	l_straHdr.SetAt(0, _T(""));
	LoadStringML(IDS_LSTHDR_NAME,		l_straHdr.ElementAt(1), _T("Name"));
	LoadStringML(IDS_LSTHDR_SHAPE,		l_straHdr.ElementAt(2), _T("Shape"));
	LoadStringML(IDS_LSTHDR_SIZEX,		l_straHdr.ElementAt(3), _T("Size X[um]"));
	LoadStringML(IDS_LSTHDR_SIZEY,		l_straHdr.ElementAt(4), _T("Size Y[um]"));
	LoadStringML(IDS_LSTHDR_THICKNESS,	l_straHdr.ElementAt(5), _T("Thickness[um]"));
	// 2009.08.26 bagus stress --{--
	//LoadStringML(IDS_LSTHDR_DATE,		l_straHdr.ElementAt(6), _T("Date"));
	if (m_SystemConfig.HeadType.bStress) {
		LoadStringML(IDS_LSTHDR_ELASTICMODULUS,		l_straHdr.ElementAt(6), _T("Elastic Modulus"));
		LoadStringML(IDS_LSTHDR_DATE,				l_straHdr.ElementAt(7), _T("Date"));
	} else {
		LoadStringML(IDS_LSTHDR_DATE,		l_straHdr.ElementAt(6), _T("Date"));
	}
	// 2009.08.26 bagus stress --}--
/* added 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		// 2009.08.26 bagus stress --{--
		//listcol.fmt = Fmt[i];
		//listcol.cx = Width[i];
		if (m_SystemConfig.HeadType.bStress) {
			listcol.fmt = FmtStress[i];
			listcol.cx = WidthStress[i];
		} else {
			listcol.fmt = Fmt[i];
			listcol.cx = Width[i];
		}
		// 2009.08.26 bagus stress --}--

/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- { ---------- */
/*
//		// Kojika 20090529 Change
//		  //listcol.pszText = Item[i];
//		  if(l_SystemConfig.nLanguage == 0){
//			  listcol.pszText = Item_ENU[i];
//		  }
//		  else{
//			  listcol.pszText = Item_JPN[i];
//		  }
//		// Kojika 20090529 Change End
*/
/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ----------			   */
		listcol.pszText = ((LPTSTR) (LPCTSTR) l_straHdr.GetAt(i));
/* modified 2009.07.06 hmenjo リストヘッダ文字リソース登録 ---------- } ---------- */
		m_lstCtrl.InsertColumn(i,&listcol);
	}
}

// =========================================================================
//
void CSampleListDlg::SampleList_DataSet()
{
	CFileFind Finder;
	char szDirPath[_MAX_PATH];
	char szName[RECIPE_NAME_LEN + 1];
	char szDate[256];
	BOOL bLoop;
	CTime time;
	SAMPLEINFO SampleInfo;

	m_lstCtrl.DeleteAllItems();
	sprintf_s(szDirPath, _countof(szDirPath), "%s*%s", g_szCfg_System_Sample_Dir, DAT_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		// ファイル名を取得
		sprintf_s(szName, _countof(szName), "%s", Finder.GetFileTitle());
		// 最終変更日時を取得
		Finder.GetLastWriteTime(time);
		sprintf_s(szDate, _countof(szDate), "%s", time.Format("%Y.%m.%d %H:%M:%S"));

		// 2009.08.26 bagus stress --{--
		//if ( RecipeFile_LoadRecipe(&SampleInfo, szName, RECIPE_FILE_SAMPLE) )
		//	SampleList_Add( SampleInfo.szName,
		//					SampleInfo.nShape,
		//					(SampleInfo.Size.dx),
		//					(SampleInfo.Size.dy),
		//					(SampleInfo.dThickness),
		//					szDate);
		if (m_SystemConfig.HeadType.bStress) {
			if ( RecipeFile_LoadRecipe(&SampleInfo, szName, RECIPE_FILE_SAMPLE) )
				SampleList_Add( SampleInfo.szName,
								SampleInfo.nShape,
								(SampleInfo.Size.dx),
								(SampleInfo.Size.dy),
								(SampleInfo.dThickness),
								(SampleInfo.dElasticModulusValue),
								szDate);
		} else {
			if ( RecipeFile_LoadRecipe(&SampleInfo, szName, RECIPE_FILE_SAMPLE) )
				SampleList_Add( SampleInfo.szName,
								SampleInfo.nShape,
								(SampleInfo.Size.dx),
								(SampleInfo.Size.dy),
								(SampleInfo.dThickness),
								0,
								szDate);
		}
	}
}

// =========================================================================
//
// 2009.08.26 bagus stress --{--
//void CSampleListDlg::SampleList_Add(char* lpszName, int iShape, double dSizeX, double dSizeY, double dThickness, char* lpszDate)
void CSampleListDlg::SampleList_Add(char* lpszName, int iShape, double dSizeX, double dSizeY, double dThickness, double dElasticModulusValue, char* lpszDate)
// 2009.08.26 bagus stress --}--
{
	LV_ITEM listitem;
	int item;
	char Buff[256];

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	listitem.iItem = m_lstCtrl.GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	item = m_lstCtrl.InsertItem(&listitem);

	///// Name /////
	listitem.iSubItem = 1;
	listitem.pszText = lpszName;
	m_lstCtrl.SetItem(&listitem);

	///// Shape /////
	listitem.iSubItem = 2;
	memset(Buff, 0, sizeof(Buff));
	iShape = iShape >= SAMPLE_SHAPE_MAX ? 0 : iShape;
	// Kojika 20090528 Change
	//memcpy(Buff, g_lpszSampleShape[iShape], sizeof(Buff));
	if(l_SystemConfig.nLanguage == 0){
		memcpy(Buff, g_lpszSampleShape_ENU[iShape], sizeof(Buff));
	}
	else{
		memcpy(Buff, g_lpszSampleShape_JPN[iShape], sizeof(Buff));
	}
	// Kojika 20090528 Change End
	listitem.pszText = Buff;
	m_lstCtrl.SetItem(&listitem);

	///// Size_X /////
	listitem.iSubItem = 3;
	sprintf_s(Buff, _countof(Buff), "%.0lf", dSizeX);
	listitem.pszText = Buff;
	m_lstCtrl.SetItem(&listitem);

	///// Size_Y /////
	listitem.iSubItem = 4;
	sprintf_s(Buff, _countof(Buff), "%.0lf", dSizeY);
	listitem.pszText = Buff;
	m_lstCtrl.SetItem(&listitem);

	///// Thickness /////
	listitem.iSubItem = 5;
	sprintf_s(Buff, _countof(Buff), "%.0lf", dThickness);
	listitem.pszText = Buff;
	m_lstCtrl.SetItem(&listitem);

	///// Date /////
	// 2009.08.26 bagus stress --{--
	//listitem.iSubItem = 6;
	//listitem.pszText = lpszDate;
	//m_lstCtrl.SetItem(&listitem);
	if (m_SystemConfig.HeadType.bStress) {
		listitem.iSubItem = 6;
		sprintf_s(Buff, _countof(Buff), "%e", dElasticModulusValue);
		listitem.pszText = Buff;
		m_lstCtrl.SetItem(&listitem);

		listitem.iSubItem = 7;
		listitem.pszText = lpszDate;
		m_lstCtrl.SetItem(&listitem);
	} else {
		listitem.iSubItem = 6;
		listitem.pszText = lpszDate;
		m_lstCtrl.SetItem(&listitem);
	}

	m_lstCtrl.SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

// =========================================================================
//
void CSampleListDlg::OnNewButton()
{
	EditSample(TRUE);
}

// =========================================================================
//
void CSampleListDlg::OnEditButton()
{
	EditSample(FALSE);
}

// =========================================================================
//
void CSampleListDlg::OnDeleteButton()
{
	int iListCount;
	char szMessage[256], szName[RECIPE_NAME_LEN + 1];
	int iSelIndex = m_lstCtrl.GetSelectionMark();

	// Kojika 20090529 Add
	CString l_strBuffer;
	// Kojika 20090529 Add End

	// 選択されていない場合は何もしない
	if ( iSelIndex == -1 )
		return;

	m_lstCtrl.GetItemText(iSelIndex, 1, szName, sizeof(szName));
	// Kojika 20090529 Change
	//sprintf_s(szMessage, _countof(szMessage), "Do you want to Delete \"%s\"?", szName);
	//sprintf_s(szMessage, _countof(szMessage), "\"%s\"を削除しますか?", szName);
	LoadStringML(IDS_DELETE_SAMPLE_LIST, l_strBuffer, "Do you want to Delete ""%s""?");
	sprintf_s(szMessage, _countof(szMessage), l_strBuffer, szName);
	//if ( MessageBox(szMessage, "SAMPLE LIST", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDCANCEL )
	LoadStringML(IDS_TITLE_SAMPLE_LIST, l_strBuffer, "SAMPLE LIST");
	if ( MessageBox(szMessage, l_strBuffer, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDCANCEL )
	// Kojika 20090529 Change End
		return;

	if(RecipeFile_DeleteRecipe(szName, RECIPE_FILE_SAMPLE)){
		iListCount = m_lstCtrl.GetItemCount();
		if(iListCount){
			SampleList_DataSet();

			if(iSelIndex == iListCount - 1){
				iSelIndex--;
			}

			m_lstCtrl.SetSelectionMark(iSelIndex);
			m_lstCtrl.SetItemState(iSelIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

// =========================================================================
//
void CSampleListDlg::OnClickSampleList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}

// =========================================================================
//
void CSampleListDlg::OnDblclkSampleList(NMHDR* pNMHDR, LRESULT* pResult)
{
	EditSample(FALSE);

	*pResult = 0;
}

// =========================================================================
//
void CSampleListDlg::EditSample(BOOL bNew)
{
	char szName[RECIPE_NAME_LEN + 1];

	int iSelIndex = m_lstCtrl.GetSelectionMark();

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	if(!bNew && iSelIndex == -1){
		// Kojika 20090529 Change
		//MessageBox("Please Select Sample", "SAMPLE LIST", MB_OK|MB_ICONSTOP);
		//MessageBox("サンプルを選択してください", "SAMPLE LIST", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_SELECT_SAMPLE, l_strBuffer, "Please Select Sample");
		LoadStringML(IDS_TITLE_SAMPLE_LIST, l_strBuffer, "SAMPLE LIST");
		// Kojika 20090529 Change End
		return;
	}

	memset(szName, 0, sizeof(szName));
	if( !bNew ){
		m_lstCtrl.GetItemText(iSelIndex, 1, szName, sizeof(szName));
	}

	CSampleSettingDlg dlg(bNew, szName);
	if ( dlg.DoModal() == IDOK ) {
		SampleList_DataSet();

		///// Cursor /////
		iSelIndex = List_SetCursorPosition(&m_lstCtrl, 1, dlg.m_szName);
	}

	m_bOrder[1] = 0;
	Sort(1);
}

// =========================================================================
//
void CSampleListDlg::OnColumnclickSampleList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int iCol = pNMListView->iSubItem;
	Sort(iCol);

	*pResult = 0;
}

// =========================================================================
//
void CSampleListDlg::Sort(int iCol)
{
	m_iCol = iCol;
	m_bOrder[iCol] = !m_bOrder[iCol];
	RenumberItem();
	m_lstCtrl.SortItems(CompareFunc, (unsigned long)this);
}

// =========================================================================
//
void CSampleListDlg::RenumberItem()
{
	LV_ITEM listitem;

	for ( int i = 0; i < m_lstCtrl.GetItemCount(); i++ ) {
		listitem.iItem = i;
		listitem.iSubItem = 0;
		listitem.mask = LVIF_PARAM;
		listitem.lParam = i;			// ここで番号をアイテムに指定する
		m_lstCtrl.SetItem(&listitem);
	}
}
