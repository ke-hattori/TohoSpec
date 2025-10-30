// AlarmLogDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "Alarm.h"
#include "AlarmLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Define                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#define ALARM_DIALOG_MAX 1000

/////////////////////////////////////////////////////////////////////////////
// CAlarmLogDlg ダイアログ                                                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CAlarmLogDlg::CAlarmLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarmLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarmLogDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmLogDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CAlarmLogDlg, CDialog)
	//{{AFX_MSG_MAP(CAlarmLogDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmLogDlg メッセージ ハンドラ                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CAlarmLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Init List /////
	InitList();

	/// Restore /////
	Restore(g_szLog_Dir, ALARM_LOG_NAME, ALARM_LOG_MAX, ALARM_DIALOG_MAX);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmLogDlg::InitList()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_ALARM_LIST);
	LV_COLUMN listcol;
	int i;
	DWORD dwStyle = 0;
	int ItemMax;
	//Saiki 20090527 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090527 Add <-----

	int Width[] = {
		0,		// (0) Index
		70, 	// (1) Date
		60, 	// (2) Time
		60, 	// (3) Level (Alarm / Warning)
		50, 	// (4) Set/Reset
		50, 	// (5) ID
		600,	// (6) Alarm
	};

//Saiki 20090527 Change ----->
//	  LPTSTR Item[] = {
////	  "",
////	  "Date",
////	  "Time",
////	  "Level",
////	  "Set",
////	  "ID",
////	  "Alarm",
//		"",
//		  "日付",
//		  "時間",
//		  "レベル",
//		  "セット",
//		"ID",
//		  "アラーム内容",
//	  };
	LPTSTR Item_ENU[] = {
	  "",
	  "Date",
	  "Time",
	  "Level",
	  "Set",
	  "ID",
	  "Alarm",
	};
	LPTSTR Item_JPN[] = {
		"",
		"日付",
		"時間",
		"レベル",
		"セット",
		"ID",
		"アラーム内容",
	};
//Saiki 20090527 Change <-----

	int Fmt[] = {
		LVCFMT_LEFT,
		LVCFMT_CENTER,
		LVCFMT_CENTER,
		LVCFMT_CENTER,
		LVCFMT_CENTER,
		LVCFMT_CENTER,
		LVCFMT_LEFT
	};

	ItemMax = sizeof(Width) / sizeof(int);

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

	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(i = 0 ; i < ItemMax ; i++){
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
void CAlarmLogDlg::AddAlarmHis(int Index, ALARM_HIS* pAlarmHis)
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_ALARM_LIST);
	LV_ITEM listitem;
	int item;
	int ListCount;

	while(ListCount = pList->GetItemCount() >= ALARM_DIALOG_MAX){
		pList->DeleteItem(0);
	}

	if(Index == -1){
		listitem.iItem = pList->GetItemCount();
	}
	else{
		listitem.iItem = Index;
	}

	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	item = pList->InsertItem(&listitem);

	///// Date /////
	listitem.iSubItem = 1;
	listitem.pszText = pAlarmHis->Date;
	pList->SetItem(&listitem);

	///// Time /////
	listitem.iSubItem = 2;
	listitem.pszText = pAlarmHis->Time;
	pList->SetItem(&listitem);

	///// Level /////
	listitem.iSubItem = 3;
	listitem.pszText = pAlarmHis->Level;
	pList->SetItem(&listitem);

	///// Set /////
	listitem.iSubItem = 4;
	listitem.pszText = pAlarmHis->Set;
	pList->SetItem(&listitem);

	///// ID /////
	listitem.iSubItem = 5;
	listitem.pszText = pAlarmHis->Id;
	pList->SetItem(&listitem);

	///// Alarm Text /////
	listitem.iSubItem = 6;
	listitem.pszText = pAlarmHis->Text;
	pList->SetItem(&listitem);

	SetCursorToLast();
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmLogDlg::SetCursorToLast()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_ALARM_LIST);
	int Index;

	Index = pList->GetItemCount() - 1;
	if(Index < 0) Index = 0;

	pList->SetItemState(Index, LVIS_SELECTED, LVIS_SELECTED);
	pList->EnsureVisible(Index, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmLogDlg::Restore(char* Path, char* filename,int LogMax,int HisMax)
{
	CFile file;
	char FileName[_MAX_PATH];
	DWORD FilePointer;
	char Character;
	char szBuff[1024];
	int Count;
	int ReadNum;
	int TerminatorNum;
	int i;
	int ListCount;
	int FileNo;
	ALARM_HIS AlarmHis;

	ListCount = 0;
	for(FileNo = 1 ; FileNo <= LogMax ; FileNo++){
		///// Log File Saving /////
		sprintf(FileName, "%s\\%s%03d" LOG_FILE_EXT, Path, filename, FileNo);
		if(!file.Open(FileName, CFile::modeRead | CFile::typeBinary)){
			continue;
		}

		file.SeekToEnd();
		FilePointer = static_cast<long>(file.GetPosition());

		///// check the number of terminator /////
		TerminatorNum = 0;
		FilePointer = static_cast<long>(file.Seek(-2 ,CFile::current));
		for(i = 0 ; i < 2 ; i++){
			file.Read(&Character, 1);
			if(Character == 0x0d || Character == 0x0a){
				TerminatorNum++;
			}
		}

		///// read data /////
		Count = 0;
		try{
			while(FilePointer != 0){
				FilePointer = static_cast<long>(file.Seek(-2 ,CFile::current));
				file.Read(&Character, 1);
				Count++;
				if(FilePointer == 0){
					Count = Count - (TerminatorNum - 1);
					FilePointer = static_cast<long>(file.Seek(-1 ,CFile::current));
					ReadNum = file.Read(szBuff, Count);
					FilePointer = static_cast<long>(file.Seek(-ReadNum ,CFile::current));
					szBuff[Count] = '\0';

					AlarmHis_StrToHis(szBuff, &AlarmHis);
					AddAlarmHis(0, &AlarmHis);

					Count = 0;
					if(++ListCount >= HisMax){
						break;
					}
					continue;
				}
				if(Character == 0x0a){
					Count = Count - TerminatorNum;
					ReadNum = file.Read(szBuff,Count);
					FilePointer = static_cast<long>(file.Seek(-ReadNum ,CFile::current));
					szBuff[Count] = '\0';

					AlarmHis_StrToHis(szBuff, &AlarmHis);
					AddAlarmHis(0, &AlarmHis);

					Count = 0;
					if(++ListCount >= HisMax){
						break;
					}
					continue;
				}
			}
		}
		catch(CFileException* fe){
			long ErrorCode = fe->m_lOsError;
		}

		file.Close();
	}

}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmLogDlg::AlarmHis_StrToHis(char* pszStr, ALARM_HIS* pAlarmHis)
{
	char szTemp[MAX_PATH];
	CString sBuff;

	memset(pAlarmHis, 0, sizeof(ALARM_HIS));

	///// Date /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[0], ALARM_LOG_DATE_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Date, sBuff.GetBuffer(0));

	///// Time /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[11], ALARM_LOG_TIME_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Time, sBuff.GetBuffer(0));

	///// Level /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[21], ALARM_LOG_LEVEN_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Level, sBuff.GetBuffer(0));

	///// Set /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[29], ALARM_LOG_SET_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Set, sBuff.GetBuffer(0));

	///// ID /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[34], ALARM_LOG_ID_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Id, sBuff.GetBuffer(0));

	///// Text /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[39], ALARM_LOG_TEXT_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Text, sBuff.GetBuffer(0));
}
