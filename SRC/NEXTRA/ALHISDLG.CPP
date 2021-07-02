// ALARMHISDLG.CPP : implementation file
//

#include "stdafx.h"
#include "common.hpp"
#include "alhisdlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// CAlarmHistoryDlg dialog
// ===========================================================================

CAlarmHistoryDlg::CAlarmHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarmHistoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarmHistoryDlg)
	//}}AFX_DATA_INIT
}

void CAlarmHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmHistoryDlg)
	DDX_Control(pDX, IDC_ALARM_LIST, m_listAlarm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarmHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CAlarmHistoryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ===========================================================================
// CAlarmHistoryDlg message handlers
// ===========================================================================

BOOL CAlarmHistoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listAlarm.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listAlarm.GetHeaderCtrl()->EnableWindow(FALSE);

	InitializeList();
	UpdateList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ---------------------------------------------------------------------------
void CAlarmHistoryDlg::InitializeList(void)
{
	int nWidth[] = { 0, 120, 40, 50, 300 };
	int nFormat[] = { LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT };

	int nCount = sizeof(nWidth) / sizeof(nWidth[0]);
	int nTotal = 0;
	for ( int index = 0; index < nCount; index++ ) nTotal += nWidth[index];

	RECT rect;
	m_listAlarm.GetClientRect(&rect);
	int cxScroll = ::GetSystemMetrics(SM_CXVSCROLL);

	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_WIDTH | LVCF_FMT;
	for ( int nColumn = 0; nColumn < nCount; nColumn++ ) {
		lvColumn.cx = (int)((rect.right - cxScroll) * nWidth[nColumn] / nTotal + 0.5);
		lvColumn.fmt = nFormat[nColumn];
		m_listAlarm.InsertColumn(nColumn, &lvColumn);
	}
}

// ---------------------------------------------------------------------------
void CAlarmHistoryDlg::UpdateList(void)
{
	m_listAlarm.DeleteAllItems();

	TCHAR szFilePath[_MAX_PATH];
	TCHAR szDir   [_MAX_DIR];
	TCHAR szFname [_MAX_FNAME];
	TCHAR szExt   [_MAX_EXT];
	_tsplitpath(NEXALARM_LOGFILE_NAME, NULL, szDir, szFname, szExt);
	_stprintf(szFilePath, "%s%s001%s", szDir, szFname, szExt);

	std::ifstream ifs(szFilePath);

/*
<<<<< NEXALARMXXX.LOG >>>>>
0                                                                                                   1                                                                                                   2                                                                                                   3
0         1         2         3         4         5         6         7         8         9         0         1         2         3         4         5         6         7         8         9         0         1         2         3         4         5         6         7         8         9         0         1         2         3         4         5
01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
2009.03.03 13:00:53:081 1053 [W] <SET> ADAXサーボ通信失敗
YYYY.MM.DD HH:MM:SS:FFF #### [#] <###> ##################
*/
	std::string line;
	std::list<string> list;
	while ( std::getline(ifs, line) ) {
		if ( line.find("<SET>", 33) != 33 ) continue;
		list.push_back(line);
		if ( list.size() >= 1000 ) break;
	}

	std::list<string>::reverse_iterator iter(list.rbegin());
	for ( ; iter != list.rend(); iter++ ) {
		// DUMMY
		m_listAlarm.InsertItem(0, "");
		// DATE/TIME
		m_listAlarm.SetItemText(0, 1, iter->substr(0, 23).c_str());
		// ALARM ID
		m_listAlarm.SetItemText(0, 2, iter->substr(24, 4).c_str());
		// LEVEL
		m_listAlarm.SetItemText(0, 3, iter->substr(30, 1) == "A" ? "Alarm" : "Warning");
		// ALARM TEXT
		m_listAlarm.SetItemText(0, 4, iter->substr(39).c_str());
	}

	m_listAlarm.EnsureVisible(list.size() - 1, FALSE);
}
