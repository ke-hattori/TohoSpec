// TerminatorDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "tlhelp32.h"
#include "Terminator.h"
#include "TerminatorDlg.h"
//#include "GLOBCUST.HXX"
#include "RestartDlg.h"
// Goto add 2007.0409 --->
//#include <LogManager.hxx>
// Goto add 2007.0409 <---

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int     m_Mode;
/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// ダイアログ データ
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard は仮想関数のオーバーライドを生成します
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
    //}}AFX_VIRTUAL

// インプリメンテーション
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // メッセージ ハンドラがありません。
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerminatorDlg ダイアログ

CTerminatorDlg::CTerminatorDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTerminatorDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTerminatorDlg)
        // メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
	GetRootDirPath();
	MakeProcessResetPath();
	MakeBinPath();
// 2013.11.07 Bagus Add (TohoSpec対応) <--

    MaxNum = GetList();
}

void CTerminatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTerminatorDlg)
    DDX_Control(pDX, IDC_LIST1, m_List);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTerminatorDlg, CDialog)
    //{{AFX_MSG_MAP(CTerminatorDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, OnForceRestart)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON2, OnForceKill)
    ON_BN_CLICKED(IDC_BUTTON3, OnReAgentCD)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
    ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerminatorDlg メッセージ ハンドラ

BOOL CTerminatorDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // "バージョン情報..." メニュー項目をシステム メニューへ追加します。

    // IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);         // 大きいアイコンを設定
    SetIcon(m_hIcon, FALSE);        // 小さいアイコンを設定

    // *********************************
    m_Mode = 3;
    for(int i=0;i<100;i++){DDD[i]=-1; SaveDDD[i]=-1;}

    WIN32_FIND_DATA FindData;

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
    #ifdef DEBUG
//  if (FindFirstFile("C:\\Nanospec\\bin\\nanospec.exe", &FindData) != INVALID_HANDLE_VALUE
    if (FindFirstFile(m_strNanoSpecReleaseExeFilePath, &FindData) != INVALID_HANDLE_VALUE
        &&(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!= FILE_ATTRIBUTE_DIRECTORY)
    {
        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ListViewInit();
        ListViewInsert();
        SetTimer(0,500,NULL);

        if(OnShowcmdline())OnButton4(); //Show Restart Dialog
    }
//  else if (FindFirstFile("C:\\Nanospec\\bin_deb\\nanospec.exe", &FindData) != INVALID_HANDLE_VALUE
    else if (FindFirstFile(m_strNanoSpecDebugExeFilePath, &FindData) != INVALID_HANDLE_VALUE
        &&(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!= FILE_ATTRIBUTE_DIRECTORY)
    {
        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ListViewInit();
        ListViewInsert();
        SetTimer(0,500,NULL);

        if(OnShowcmdline())OnButton4(); //Show Restart Dialog
    }
    #else
//  if (FindFirstFile("C:\\Nanospec\\bin_deb\\nanospec.exe", &FindData) != INVALID_HANDLE_VALUE
    if (FindFirstFile(m_strNanoSpecDebugExeFilePath, &FindData) != INVALID_HANDLE_VALUE
        &&(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!= FILE_ATTRIBUTE_DIRECTORY)
    {
        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ListViewInit();
        ListViewInsert();
        SetTimer(0,500,NULL);

        if(OnShowcmdline())OnButton4(); //Show Restart Dialog
    }
//  else if (FindFirstFile("C:\\Nanospec\\bin\\nanospec.exe", &FindData) != INVALID_HANDLE_VALUE
    else if (FindFirstFile(m_strNanoSpecReleaseExeFilePath, &FindData) != INVALID_HANDLE_VALUE
        &&(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!= FILE_ATTRIBUTE_DIRECTORY)
    {
        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ListViewInit();
        ListViewInsert();
        SetTimer(0,500,NULL);

        if(OnShowcmdline())OnButton4(); //Show Restart Dialog
    }
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

    #endif
    else
    {
        OnCancel();
    }
    return TRUE;
}

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
void CTerminatorDlg::GetRootDirPath()
{
	TCHAR szBuff[MAX_PATH];
	CString strDirectory;

	::GetModuleFileName(NULL, szBuff, sizeof(szBuff));

	strDirectory = szBuff;
	m_strCurrentDirectory = strDirectory.Left(strDirectory.ReverseFind(_T('\\')) + 1);		// current dir

	strDirectory = strDirectory.Left(strDirectory.ReverseFind(_T('\\')));
	m_strRootDirectory = strDirectory.Left(strDirectory.ReverseFind(_T('\\')) + 1);	// root_dir
}

void CTerminatorDlg::MakeProcessResetPath()
{
	m_strProcessResetDirPath = m_strRootDirectory + "CFG\\ProcessReset\\";

	m_strForceRestartReleaseBatFilePath = m_strProcessResetDirPath + "ForceRestart.bat";
	m_strForceRestartDebugBatFilePath = m_strProcessResetDirPath + "ForceRestart_Debug.bat";
	m_strProcessListTxtFilePath = m_strProcessResetDirPath + "ProcessList.txt";
}

void CTerminatorDlg::MakeBinPath()
{
	m_strNanoSpecReleaseExeFilePath = m_strCurrentDirectory + GetReleaseBinRelativePath();
	m_strNanoSpecDebugExeFilePath = m_strCurrentDirectory + GetDebugBinRelativePath();
}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

void CTerminatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CTerminatorDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 描画用のデバイス コンテキスト

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // クライアントの矩形領域内の中央
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // アイコンを描画します。
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

HCURSOR CTerminatorDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;

}

void CTerminatorDlg::OnForceRestart()
{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	CString strBatFilePath;
	CString strCmdParameters;
// 2013.11.07 Bagus Add (TohoSpec対応) <--

    for(int K=0;K<MaxNum;K++) KillProc(DDD[K]);
    char lpPath[255];
    sprintf(lpPath,"cmd.exe");
//Saiki 20090604 Change ----->
    WIN32_FIND_DATA FindData;

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
    #ifdef DEBUG
//      if (FindFirstFile("C:\\Nanospec\\bin\\nanospec.exe", &FindData) != INVALID_HANDLE_VALUE
        if (FindFirstFile(m_strNanoSpecReleaseExeFilePath, &FindData) != INVALID_HANDLE_VALUE
            &&(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!= FILE_ATTRIBUTE_DIRECTORY)
        {
//          HINSTANCE hInst = ShellExecute(NULL,"open", lpPath,"/C C:\\NanoSpec\\Cfg\\ProcessReset\\ForceRestart.bat", NULL, SW_SHOWNORMAL );
			strBatFilePath = m_strForceRestartReleaseBatFilePath;
        }else{
//          HINSTANCE hInst = ShellExecute(NULL,"open", lpPath,"/C C:\\NanoSpec\\Cfg\\ProcessReset\\ForceRestart_Debug.bat", NULL, SW_SHOWNORMAL );
			strBatFilePath = m_strForceRestartDebugBatFilePath;
        }
    #else
//      if (FindFirstFile("C:\\Nanospec\\bin_deb\\nanospec.exe", &FindData) != INVALID_HANDLE_VALUE
        if (FindFirstFile(m_strNanoSpecDebugExeFilePath, &FindData) != INVALID_HANDLE_VALUE
            &&(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!= FILE_ATTRIBUTE_DIRECTORY)
        {
//          HINSTANCE hInst = ShellExecute(NULL,"open", lpPath,"/C C:\\NanoSpec\\Cfg\\ProcessReset\\ForceRestart_Debug.bat", NULL, SW_SHOWNORMAL );
			strBatFilePath = m_strForceRestartDebugBatFilePath;
        }else{
//          HINSTANCE hInst = ShellExecute(NULL,"open", lpPath,"/C C:\\NanoSpec\\Cfg\\ProcessReset\\ForceRestart.bat", NULL, SW_SHOWNORMAL );
			strBatFilePath = m_strForceRestartReleaseBatFilePath;
        }
    #endif

	strCmdParameters.Format("/C %s", (LPCTSTR)strBatFilePath);
	HINSTANCE hInst = ShellExecute(NULL,"open", lpPath, strCmdParameters, NULL, SW_SHOWNORMAL );
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

//Saiki 20090604 Change <-----
    ShowWindow(SW_MINIMIZE);
}

void CTerminatorDlg::OnForceKill()
{
    for(int K=0;K<MaxNum;K++) KillProc(DDD[K]);
}

void CTerminatorDlg::OnReAgentCD()
{
}

void CTerminatorDlg::OnOK()
{
}


//***********************************
//  Find Process
//***********************************
BOOL CTerminatorDlg::FindAndKillApp()
{
    HANDLE         snapshot;
    PROCESSENTRY32 process;
    process.dwSize = sizeof( PROCESSENTRY32 );
    snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPALL, 0 );
    if( snapshot == NULL ) return FALSE;

    BOOL status = Process32First( snapshot, &process );     //Start Find

    DWORD index = 0;
    while( status )
    {
        for(int K=0;K<MaxNum;K++)
        {
            CString K1  = process.szExeFile;
            CString K2  = GGG[K];

            if(strcmp(CharLower(K1.GetBuffer(0)),CharLower(K2.GetBuffer(0)))==0)
                DDD[K] = process.th32ProcessID;             //KillProc(process.th32ProcessID);
        }
        status = Process32Next( snapshot, &process );       //Next Application
        index++;
    }

    CloseHandle (snapshot);
    return TRUE;
}

//***********************************
//  Kill Process
//***********************************
BOOL CTerminatorDlg::KillProc(DWORD Pid)
{
    BOOL  resuret;
    HANDLE hProcess;

    hProcess = OpenProcess( PROCESS_TERMINATE, 1, Pid );
    if( resuret = (hProcess != 0) )
    {
        // プロセスがKillできない場合にfalse
        resuret = TerminateProcess( hProcess, (UINT)0 );
        CloseHandle( hProcess );
    }
    return resuret;
}

// 2013.11.07 Bagus Add (TohoSpec対応) -->
//***********************************
//  Read ForceRestart.bat
//***********************************
CString CTerminatorDlg::GetReleaseBinRelativePath()
{
	CStdioFile file;
	CFileStatus status;
	CString strBuffer;

	if (CFile::GetStatus(m_strForceRestartReleaseBatFilePath, status) &&
		file.Open(m_strForceRestartReleaseBatFilePath, CFile::modeRead | CFile::typeText))
	{
		file.ReadString(strBuffer);
		file.Close();
	}

	return strBuffer;
}

//***********************************
//  Read ForceRestart_Debug.bat
//***********************************
CString CTerminatorDlg::GetDebugBinRelativePath()
{
	CStdioFile file;
	CFileStatus status;
	CString strBuffer;

	if (CFile::GetStatus(m_strForceRestartDebugBatFilePath, status) &&
		file.Open(m_strForceRestartDebugBatFilePath, CFile::modeRead | CFile::typeText))
	{
		file.ReadString(strBuffer);
		file.Close();
	}

	return strBuffer;
}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

//***********************************
//  Read ini file
//***********************************
int CTerminatorDlg::GetList()
{
    CStdioFile file;
    CFileStatus status;
    int Count=0;
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//  if (CFile::GetStatus("C:\\NanoSpec\\Cfg\\ProcessReset\\ProcessList.txt", status) &&
//      file.Open("C:\\NanoSpec\\Cfg\\ProcessReset\\ProcessList.txt", CFile::modeRead | CFile::typeText))
    if (CFile::GetStatus(m_strProcessListTxtFilePath, status) &&
        file.Open(m_strProcessListTxtFilePath, CFile::modeRead | CFile::typeText))
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
    {
        while(file.ReadString(GGG[Count])!=NULL) {Count++;}
        file.Close();
    }

    return Count;
}

//***********************************
//  Show ListView
//***********************************
void CTerminatorDlg::ListViewInit()
{
    HWND h_ListBox = GetDlgItem(IDC_LIST1)->GetSafeHwnd();
    if( !h_ListBox) return;

    RECT rect;
    GetDlgItem(IDC_LIST1)->GetClientRect(&rect );


    ListView_SetExtendedListViewStyle( h_ListBox, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

    LV_COLUMN ListCol;

    ListCol.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    ListCol.fmt     = LVCFMT_LEFT;
    ListCol.cx      = 180;
    ListCol.pszText = "AppName";
    ListView_InsertColumn( h_ListBox, 0, &ListCol );

    ListCol.cx      = rect.right -180 - 17;
    ListCol.pszText = "State";
    ListView_InsertColumn( h_ListBox, 1, &ListCol );

}


void CTerminatorDlg::ListViewInsert()
{
    HWND h_ListBox = GetDlgItem(IDC_LIST1)->GetSafeHwnd();
    if( !h_ListBox) return;
    ListView_DeleteAllItems(h_ListBox); //Clear list

    ListView_SetTextColor(h_ListBox, RGB(0,0,120));
    ListView_SetTextBkColor(h_ListBox, RGB(200,200,200));
    ListView_SetBkColor(h_ListBox,  RGB(200,200,200));

    LV_ITEM ListItem;
    int ItemNo=0;
    while(ItemNo < MaxNum)
    {
        ListItem.mask     = LVIF_TEXT;
        ListItem.iItem = ItemNo;
        ListItem.iSubItem = 0;
        ListItem.pszText = GGG[ItemNo].GetBuffer(0);
        ListView_InsertItem(h_ListBox,&ListItem);

        char txt[50]="";
        if(DDD[ItemNo]!=-1)sprintf(txt,"%s","Working");
        ListItem.pszText  = txt;
        ListItem.iSubItem = 1;
        ListView_SetItem( h_ListBox, &ListItem );

        ItemNo++;
    }
}

void CTerminatorDlg::OnTimer(UINT nIDEvent)
{
    KillTimer(0);
    for(int i=0;i<100;i++){DDD[i]=-1;}
    FindAndKillApp();                   //Get Current Value

    if(memcmp(&DDD,&SaveDDD,sizeof(DDD))!=0)
    {
        memcpy(&SaveDDD, &DDD,sizeof(DDD));
        ListViewInsert();
    }
    SetTimer(0,500,NULL);
    CDialog::OnTimer(nIDEvent);
}


void CTerminatorDlg::OnCancel()
{
    CDialog::OnCancel();
}


void CTerminatorDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
    int sa = m_List.GetSelectionMark();
    CString ka = m_List.GetItemText(sa,0);
    CString kaa = m_List.GetItemText(sa,1);
    int i=0;
    if (kaa=="") goto Out;

    if( AfxMessageBox(ka + " :\n\nYou want to say Bye-Bye for me ? "
                    , MB_OKCANCEL | MB_ICONWARNING) == 2)
                    goto Out;


    for(i=0;i<100;i++)
    {
        if(ka == GGG[i])
        {
            KillProc(DDD[i]);
            break;
        }
    }

Out:
    *pResult = 0;
}

void CTerminatorDlg::OnButton4()
{
    this->ShowWindow(SW_HIDE);
    CRestartDlg* Dlg = new CRestartDlg();

    this->ShowWindow(SW_HIDE);

    if(Dlg->DoModal() == IDOK)OnForceKill();    //kill all process

    switch(m_Mode)
    {
    case 0:
        RestartWin(EWX_REBOOT);
        break;
    case 1:
        RestartWin(EWX_POWEROFF);
        break;
    case 2:
        RestartWin(EWX_LOGOFF);
        break;
    case 3:
        if(Dlg)delete Dlg;
        OnCancel();
        break;
    }
}

void CTerminatorDlg::RestartWin(UINT param)
{

    HANDLE           hToken;
    TOKEN_PRIVILEGES tknPri;


    //////////////////////////
    OpenProcessToken( GetCurrentProcess(),
                       TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                       &hToken );

    //////////////////////////
    LookupPrivilegeValue( NULL, SE_SHUTDOWN_NAME, &(tknPri.Privileges[0].Luid) );
    tknPri.PrivilegeCount = 1;
    tknPri.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges( hToken,FALSE,&tknPri,0,NULL,NULL );

    ExitWindowsEx (param,0 ); /*EWX_POWEROFF*/

}


BOOL CTerminatorDlg::OnShowcmdline()
{
  int intArgC;
  intArgC = __argc;

  if(!(intArgC==2))
  {
    return FALSE;
  }
  CString DD = __argv[1];

  if(DD == "-SYS")  return TRUE;
  else              return false;
}
