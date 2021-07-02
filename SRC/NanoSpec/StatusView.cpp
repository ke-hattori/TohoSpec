// StatusView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "StatusView.h"
#include "Alarm.h"
#include "ChiefExports.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusView                                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CStatusView, CNanoUI)

/////////////////////////////////////////////////////////////////////////////
//
CStatusView::CStatusView()
	: CNanoUI(CStatusView::IDD)
{
	//{{AFX_DATA_INIT(CStatusView)
	//}}AFX_DATA_INIT

	FontCreation();
}

/////////////////////////////////////////////////////////////////////////////
//
CStatusView::~CStatusView()
{
	m_Font.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//
void CStatusView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_ALARM_RESET_BUTTON, m_AlarmResetButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CStatusView, CNanoUI)
	//{{AFX_MSG_MAP(CStatusView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ALARM_RESET_BUTTON, OnAlarmResetButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusView 診断

#ifdef _DEBUG

/////////////////////////////////////////////////////////////////////////////
//
void CStatusView::AssertValid() const
{
	CNanoUI::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
//
void CStatusView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStatusView メッセージ ハンドラ                                         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
int CStatusView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNanoUI::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CStatusView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	///// Alarm Reset Button /////
	m_AlarmResetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AlarmResetButton.DrawFlatFocus(TRUE);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_ALARM_LIST);
	pList->GetWindowRect(&m_ListRect);

	InitAlarmList();
}

/////////////////////////////////////////////////////////////////////////////
//
void CStatusView::InitAlarmList()
{
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_ALARM_LIST);
	LV_COLUMN listcol;
	DWORD dwStyle = 0;
	int ItemMax;
	//Saiki 20090527 Add ----->
	CString strBuffer;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090527 Add <-----

	int Width[] = {
		0,		// (0) Index
		70, 	// (1) Date
		60, 	// (2) Time
		60, 	// (3) Level (Alarm / Warning)
		50, 	// (4) ID
		700,	// (5) Text
	};

//Saiki 20090527 Change ----->
	char* Item_ENU[] = {
		"",
		"Date",
		"Time",
		"Level",
		"ID",
		"Alarm",
	};
	char* Item_JPN[] = {
		"",
		"日付",
		"時間",
		"レベル",
		"ID",
		"アラーム内容",
	};
//Saiki 20090527 Change <-----

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };

	ItemMax = sizeof(Width) / sizeof(int);

	///// Font /////
	pList->SetFont(&m_Font);

	///// Modify Style /////
	dwStyle = 0;
//	dwStyle |= LVS_EDITLABELS;
	dwStyle |= LVS_SHOWSELALWAYS;
	dwStyle |= LVS_SINGLESEL;
	pList->ModifyStyle(0, dwStyle);

	///// Extended Style /////
//	dwStyle = pList->GetExtendedStyle();
	dwStyle = 0;
//	dwStyle |= LVS_EDITLABELS;
//	dwStyle |= LVS_EX_CHECKBOXES;
//	dwStyle |= LVS_EX_ONECLICKACTIVATE;
//	dwStyle |= LVS_EX_HEADERDRAGDROP;
//	dwStyle |= LVIS_FOCUSED;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;
	pList->SetExtendedStyle(dwStyle);

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		//Saiki 20090527 Change ----->
		//listcol.pszText = Item[i];
		if(l_SystemConfig.nLanguage == 0){
			listcol.pszText = Item_ENU[i];
		}
		else{
			listcol.pszText = Item_JPN[i];
		}
		//Saiki 20090527 Change <-----
		pList->InsertColumn(i,&listcol);
	}

	pList->DeleteAllItems();
}

/////////////////////////////////////////////////////////////////////////////
//
void CStatusView::AddAlarmList(int Index, ALARM_HIS* pAlarmHis)
{
	CListCtrl *pList = (CListCtrl*)GetDlgItem(IDC_ALARM_LIST);
	LV_ITEM listitem;
	char Buff[256];
	int Item;

	// Write List Box
	if(!pList) return;

	// Insert Alarm
	listitem.mask = LVIF_TEXT;

	///// Index /////
	sprintf(Buff,"%d",Index);
	listitem.iItem = pList->GetItemCount();
	listitem.iSubItem = 0;
	listitem.pszText = Buff;
	Item = pList->InsertItem(&listitem);

	///// Date /////
	listitem.iItem = Item;
	listitem.iSubItem = 1;
	listitem.pszText = pAlarmHis->Date;
	pList->SetItem(&listitem);

	///// Time /////
	listitem.iItem = Item;
	listitem.iSubItem = 2;
	listitem.pszText = pAlarmHis->Time;
	pList->SetItem(&listitem);

	///// Level /////
	listitem.iItem = Item;
	listitem.iSubItem = 3;
	listitem.pszText = pAlarmHis->Level;
	pList->SetItem(&listitem);

	///// Alarm Text /////
	listitem.iItem = Item;
	listitem.iSubItem = 4;
	listitem.pszText = pAlarmHis->Id;
	pList->SetItem(&listitem);

	///// Alarm Text /////
	listitem.iItem = Item;
	listitem.iSubItem = 5;
	listitem.pszText = pAlarmHis->Text;
	pList->SetItem(&listitem);
}

/////////////////////////////////////////////////////////////////////////////
//
void CStatusView::OnAlarmResetButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)GetDocument();


	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
		  この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}
	// ▲インターロック条件▲


	///// Send to Alarm Thread /////
	pMainFrame->m_pAlarm->AlarmAllReset();

	///// Send to Chief /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg != NULL){
		::PostMessage(l_hChiefDlg, WM_CHIF_RESET_ALARM, 0, 0);
	}

	pDoc->OperationLogging("ALARM RESET Button was pushed.");
}
