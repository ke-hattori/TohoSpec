// MotSysDlg.cpp : インプリメンテーション ファイル
//

#include <io.h>
#include "stdafx.h"
#include "Sio.h"
#include "LogFile.h"
#include "MotSysDef.h"
#define MotMsgON
#include "..\\..\\inc\\MotsysMsg.h"
#include "MotSysDlg.h"
#include "..\..\INC\ts_secure_crt.h"
#include "CtrlCmd.h"
#include "MotionDef.h"
#include "Motion.h"
#include "FileNameDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT MOTDRV_SHOWALARM = RegisterWindowMessage("MOTDRV_SHOWALARM");	// アラーム表示用メッセージ定義
UINT MOTDRV_SRVINITCOMP = RegisterWindowMessage("MOTDRV_SRVINITCOMP");	// サーボ初期化完了メッセージ定義

CSio *g_pCSio;
CLogFile *g_pComLogFile;
CWinThread* g_pCCThread;	// Thread pointer

#define	ID_MSD_BASE_TIMER	101	// OnTimer ID
#define	MSD_BASE_TIME		200	// [ms]


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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnBtnComLogClear();
	afx_msg void OnBtnComLogEnable();
	virtual BOOL OnInitDialog();
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

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	// このダイアログ用のアイコンを設定
	SetIcon(((CMotSysDlg*) (this->GetParent()))->m_hMainIcon, TRUE);	// 大きいアイコンを設定

	if (g_pCSio != NULL) {
		if (g_pCSio->m_LogEnable == FALSE) {
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Disable"));
		} else {
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Enable"));
		}
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ログクリアボタン
void CAboutDlg::OnBtnComLogClear()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (g_pCSio != NULL) {
		GetDlgItem(IDC_BTN_COMLOGENA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_COMLOGCLR)->EnableWindow(FALSE);
		g_pCSio->ClearLog();
		GetDlgItem(IDC_BTN_COMLOGCLR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_COMLOGENA)->EnableWindow(TRUE);
	}
}

// ログの許可/禁止
void CAboutDlg::OnBtnComLogEnable()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (g_pCSio != NULL) {
		if (g_pCSio->m_LogEnable == FALSE) {
			g_pCSio->m_LogEnable = TRUE;
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Enable"));
			// Servo.ini に記憶しておく
			::WritePrivateProfileString(_T("Log"), _T("ComLog"), _T("1"), SERVO_INI);
		} else {
			g_pCSio->m_LogEnable = FALSE;
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Disable"));
			// Servo.ini に記憶しておく
			::WritePrivateProfileString(_T("Log"), _T("ComLog"), _T("0"), SERVO_INI);
		}
	}
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_BTN_COMLOGCLR, OnBtnComLogClear)
	ON_BN_CLICKED(IDC_BTN_COMLOGENA, OnBtnComLogEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotSysDlg ダイアログ

CMotSysDlg::CMotSysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotSysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotSysDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMotSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotSysDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMotSysDlg, CDialog)
	//{{AFX_MSG_MAP(CMotSysDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SERVO_OFF, OnBTNServoOFF)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ORGtest, OnBTNORGtest)
	ON_BN_CLICKED(IDC_BTN_SERVO_ON, OnBTNServoON)
	ON_BN_CLICKED(IDC_BTN_ORGtestY, OnBTNORGtestY)
	ON_BN_CLICKED(IDC_BTN_ORGtestZ, OnBTNORGtestZ)
	ON_BN_CLICKED(IDC_BTN_ORGtestT, OnBTNORGtestT)
	ON_BN_CLICKED(IDC_BTN_ABStestX, OnBTNABStestX)
	ON_BN_CLICKED(IDC_BTN_ABStestY, OnBTNABStestY)
	ON_BN_CLICKED(IDC_BTN_ABStestZ, OnBTNABStestZ)
	ON_BN_CLICKED(IDC_BTN_ABStestT, OnBTNABStestT)
	ON_BN_CLICKED(IDC_BTN_INCtestX, OnBTNINCtestX)
	ON_BN_CLICKED(IDC_BTN_INCtestY, OnBTNINCtestY)
	ON_BN_CLICKED(IDC_BTN_INCtestZ, OnBTNINCtestZ)
	ON_BN_CLICKED(IDC_BTN_INCtestT, OnBTNINCtestT)
	ON_BN_CLICKED(IDC_BTN_HPtestX, OnBTNHPtestX)
	ON_BN_CLICKED(IDC_BTN_HPtestY, OnBTNHPtestY)
	ON_BN_CLICKED(IDC_BTN_HPtestZ, OnBTNHPtestZ)
	ON_BN_CLICKED(IDC_BTN_HPtestT, OnBTNHPtestT)
	ON_BN_CLICKED(IDC_BTN_ABStestX2, OnBTNABStestX2)
	ON_BN_CLICKED(IDC_BTN_ABStestY2, OnBTNABStestY2)
	ON_BN_CLICKED(IDC_BTN_ABStestZ2, OnBTNABStestZ2)
	ON_BN_CLICKED(IDC_BTN_ABStestT2, OnBTNABStestT2)
	ON_BN_CLICKED(IDC_BTN_INCtestX2, OnBTNINCtestX2)
	ON_BN_CLICKED(IDC_BTN_INCtestY2, OnBTNINCtestY2)
	ON_BN_CLICKED(IDC_BTN_INCtestZ2, OnBTNINCtestZ2)
	ON_BN_CLICKED(IDC_BTN_INCtestT2, OnBTNINCtestT2)
	ON_BN_CLICKED(IDC_BTN_SERVO_PARAM, OnBtnServoParam)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_X, OnBtnDPosPeakClearX)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_YM, OnBtnDPosPeakClearYM)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_YS, OnBtnDPosPeakClearYS)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_Z, OnBtnDPosPeakClearZ)
	ON_BN_CLICKED(IDC_BTN_ORGtestX, OnBTNORGtest)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_T, OnBtnDPosPeakClearT)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(MOTDRV_SHOWINDOW, OnMotsysShowWindow)
	ON_REGISTERED_MESSAGE(MOTDRV_HIDEINDOW, OnBtnHideMsg)
	ON_REGISTERED_MESSAGE(MOTDRV_SETWINDOW, OnMotdrv_Setwindow)
	ON_REGISTERED_MESSAGE(MOTDRV_SETSOFTHOME, OnMotdrv_Setsofthome)
	ON_REGISTERED_MESSAGE(MOTDRV_EMERGENCYSTOP, OnMotdrv_Emergencystop)
	ON_REGISTERED_MESSAGE(MOTDRV_MOVEATSPEED, OnMotdrv_Moveatspeed)
	ON_REGISTERED_MESSAGE(MOTDRV_MOVETOPOSITION, OnMotdrv_Movetoposition)
	ON_REGISTERED_MESSAGE(MOTDRV_GETPOSITION, OnMotdrv_Getposition)
	ON_REGISTERED_MESSAGE(MOTDRV_GOHOME, OnMotdrv_Gohome)
	ON_REGISTERED_MESSAGE(MOTDRV_MOVEATSPEEDTODEST, OnMotdrv_Moveatspeedtodest)
	ON_REGISTERED_MESSAGE(MOTDRV_ISSTOPPED, OnMotdrv_Isstopped)
	ON_REGISTERED_MESSAGE(MOTDRV_ISJOYMODE, OnMotdrv_IsJoyMode)
	ON_REGISTERED_MESSAGE(MOTDRV_SETJOYMODE, OnMotdrv_SetJoyMode)
	ON_REGISTERED_MESSAGE(MOTDRV_GETMAXSPEED, OnMotdrv_GetMaxSpeed)
	ON_REGISTERED_MESSAGE(MOTDRV_GETMOTIONSIZE, OnMotdrv_GetMotionSize)
	ON_REGISTERED_MESSAGE(MOTDRV_GETELECTRONICGEAR, OnMotdrv_GetElectronicGear)
	ON_REGISTERED_MESSAGE(MOTDRV_GETSOFTLIMIT, OnMotdrv_GetSoftLimit)
	ON_REGISTERED_MESSAGE(MOTDRV_SETJOYSPEED, OnMotdrv_SetJoySpeed)
	ON_REGISTERED_MESSAGE(MOTDRV_SHOWALARM, OnShowAlarmMsg)
	ON_REGISTERED_MESSAGE(MOTDRV_SRVINITCOMP, OnServoInitCompMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotSysDlg メッセージ ハンドラ

BOOL CMotSysDlg::OnInitDialog()
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

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
//	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	// この行を消すとシステムが勝手に16x16アイコンを探してくれる．
	// ただし，16x16アイコンは予め作成しておく必要がある．

	// TODO: 特別な初期化を行う時はこの場所に追加してください。

	// ウィンドウのプロパティリストに識別情報を追加
	::SetProp(m_hWnd, IDENT_CODE, (HANDLE) 1);

	//変数の初期化
	m_bOnClosing = FALSE;
	m_bDIO_Opned = FALSE;
	m_pComLogFile = 0;
	m_pSio = 0;
	g_pCCThread = 0;
	m_bInitFailed = FALSE;
	m_hMainIcon = m_hIcon;
	m_OnClose = FALSE;
	g_CC_bInitialize = FALSE;
	memset(m_dPos, 0, sizeof(m_dPos));
	InitMotionData();

	// コントロールのイネーブル設定
	EnableDlgCtrl();

	TCHAR	l_tszText[32];
	::GetPrivateProfileString(_T("Window"), _T("AutoHide"), _T("2"), l_tszText, sizeof(l_tszText), SERVO_INI);
	m_AutoHide = _ttoi(l_tszText);
	if ((m_AutoHide < 0) || (2 < m_AutoHide)) {
		m_AutoHide = 2;
	}
/* added 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- { ---------- */
	/* DIO ログ する/しない設定読込み	*/
	::GetPrivateProfileString(_T("DIO"), _T("Log"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	if (0 == _tcscmp(_T("1"), l_tszText)) {
		m_bDioLog = TRUE;
	} else {
		m_bDioLog = FALSE;
	}
	/* DIO ログ ファイル設定	*/
	if (TRUE == m_bDioLog) {
		TCHAR l_tszDrive[_MAX_PATH], l_tszDir[_MAX_DIR], l_tszFName[_MAX_FNAME], l_tszExt[_MAX_EXT];
		TCHAR l_tszFilePath[_MAX_PATH];
		_tsplitpath(MS_DIO_LOG, l_tszDrive, l_tszDir, l_tszFName, l_tszExt);
		_stprintf_s(l_tszFilePath, _countof(l_tszFilePath), _T("%s%s"), l_tszDrive, l_tszDir);
		m_pcDioLog = new CLogFile(l_tszFName);
		if (0 != m_pcDioLog) {
			_tcscpy(m_pcDioLog->m_tszLogFilePath, l_tszFilePath);
		} else {
			::MessageBox(NULL, _T("Failed to create DIO Log class"), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	} else {
		m_pcDioLog = 0;
	}
/* added 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- } ---------- */

	SetDlgItemText(IDC_STATIC_STATUS, _T("Initializing..."));

	// アラームログの初期化
	{
		TCHAR l_tszDrive[_MAX_PATH], l_tszDir[_MAX_PATH], l_tszFName[_MAX_FNAME], l_tszExt[8];
		TCHAR	l_tszFilePath[_MAX_PATH];
		_tsplitpath(ALARM_LOG, l_tszDrive, l_tszDir, l_tszFName, l_tszExt);
		// パスとファイル名を作成
		_stprintf_s(l_tszFilePath, _countof(l_tszFilePath), _T("%s%s"), l_tszDrive, l_tszDir);
		m_pAlarmLogFile = new CLogFile(l_tszFName);
		if (0 != m_pAlarmLogFile) {
			_tcscpy(m_pAlarmLogFile->m_tszLogFilePath, l_tszFilePath);
		} else {
			::MessageBox(NULL, _T("Failed to create Alarm Log class"), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	}

	// DIO オープン
	if (0 == m_bInitFailed) {
		if (CC_DioOpen() != 0) {
			// オープン失敗
			m_bInitFailed = TRUE;
			SetDlgItemText(IDC_STATIC_STATUS, _T("Failed to DIO opened"));
			SetDlgItemText(IDC_STATIC_COMSTAT, _T("Uninitialized"));
			::MessageBox(NULL, _T("Failed to open DIO Driver."), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		} else {
			m_bDIO_Opned = TRUE;
			// シリアル通信のログの初期化
			if (InitializeCommLog() == FALSE) {
				m_bInitFailed = TRUE;
				SetDlgItemText(IDC_STATIC_STATUS, _T("Uninitialized"));
				SetDlgItemText(IDC_STATIC_COMSTAT, _T("Failed to create Comm. Log"));
				::MessageBox(NULL, _T("Failed to create Comm Log class"), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			} else {
				// シリアル通信の初期化
				if (InitializeComm() == TRUE) {
					char l_tszText[80], l_tszText2[8], l_tszText3[8];
					switch (m_pSio->m_Parity) {
					case 0: strcpy(l_tszText2, _T("N")); break;
					case 1: strcpy(l_tszText2, _T("O")); break;
					case 2: strcpy(l_tszText2, _T("E")); break;
					case 3: strcpy(l_tszText2, _T("M")); break;
					case 4: strcpy(l_tszText2, _T("S")); break;
					}
					switch (m_pSio->m_StopBits) {
					case 0: strcpy(l_tszText3, _T("1")); break;
					case 1: strcpy(l_tszText3, _T("1.5")); break;
					case 2: strcpy(l_tszText3, _T("2")); break;
					}
					_stprintf_s(l_tszText, _countof(l_tszText), _T("%s,%d,%s,%s"), m_pSio->m_sChannelNo, m_pSio->m_BaudRate, l_tszText2, l_tszText3);
					SetDlgItemText(IDC_STATIC_COMSTAT, l_tszText);
				} else {
					m_bInitFailed = TRUE;
					SetDlgItemText(IDC_STATIC_COMSTAT, _T("Uninitialized"));
					SetDlgItemText(IDC_STATIC_STATUS, _T("Uninitialized"));
				}
			}
		}
	} else {
		SetDlgItemText(IDC_STATIC_STATUS, _T("Failed to open DIO"));
		SetDlgItemText(IDC_STATIC_COMSTAT, _T("Uninitialized"));
	}

	// アバウトボタンに ICON を設定
	((CButton*) GetDlgItem(IDC_BTN_ABOUT))->SetIcon(m_hIcon);

	// ウィンドウを最前面にする
#ifndef _DEBUG
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#endif

	// このプロセスの優先度を設定する
	HANDLE l_hCurProc = ::GetCurrentProcess();
	// 前の優先度を取得
	DWORD l_oldPriority = ::GetPriorityClass(l_hCurProc);
	ChgToPriorityMsg(l_oldPriority, l_tszText);
	SetDlgItemText(IDC_STATIC_PROCPRI2, l_tszText);
#if 1
//	BOOL l_err = SetPriorityClass(l_hCurProc, REALTIME_PRIORITY_CLASS);
	BOOL l_err = SetPriorityClass(l_hCurProc, HIGH_PRIORITY_CLASS);
	if (l_err == 0) {
		// 優先度の切替え失敗
		DWORD l_errcode = GetLastError();
		TCHAR l_tszErrMsg[255]; ShowLastError(l_errcode, l_tszText);
		_stprintf_s(l_tszErrMsg, _countof(l_tszErrMsg), _T("Failed to change process priority.\n%s(0x%08x)"), l_tszText, l_errcode);
		if (0 == m_bOnClosing) {
			::MessageBox(NULL, l_tszErrMsg, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	}
	// 新しい優先度を取得
	DWORD l_newPriority = GetPriorityClass(l_hCurProc);
	ChgToPriorityMsg(l_newPriority, l_tszText);
	SetDlgItemText(IDC_STATIC_PROCPRI, l_tszText);
#endif

	// タイマ(OnTimer)を起動
	SetTimer(ID_MSD_BASE_TIMER, MSD_BASE_TIME, NULL);

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CMotSysDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMotSysDlg::OnPaint()
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

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CMotSysDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMotSysDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	// "リターン"キーで終了するのを回避するため
//	CDialog::OnOK();
}

void CMotSysDlg::OnBtnAbout()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// アバウトを表示
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CMotSysDlg::OnBtnHide()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// ウィンドウ(ダイアログベース)を非表示にする
	ShowWindow(SW_HIDE);
}

LRESULT CMotSysDlg::OnBtnHideMsg(WPARAM wParam, LPARAM lParam)
{
	OnBtnHide();
	return 0;
}

LRESULT CMotSysDlg::OnMotsysShowWindow(WPARAM wParam, LPARAM lParam)
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	// ウィンドウ(ダイアログベース)を表示にする
	ShowWindow(SW_SHOWNORMAL);
	return 0;
}

void CMotSysDlg::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	switch (nIDEvent) {
	case ID_MSD_BASE_TIMER:
		{
			static BOOL l_bRunOnce = TRUE;
			if ((0 != l_bRunOnce) && (0 == m_bInitFailed)) {
				l_bRunOnce = FALSE;
				// コントローラ監視スレッドを起動(コントローラ制御の定周期処理)
				g_pCCThread = AfxBeginThread(CC_Main_Loop, (CWnd*) this, THREAD_PRIORITY_NORMAL);
				g_pCCThread->m_bAutoDelete = FALSE;
			}
		}
		if (0 == m_bInitFailed) {
			// 送信/受信ランプの制御
			static BOOL ls_IndicateS = FALSE;
			static BOOL ls_IndicateR = FALSE;
			if (m_pSio != NULL) {
				if (ls_IndicateS == TRUE) {
					ls_IndicateS = FALSE;
					if (m_pSio->m_Indicate_Send == TRUE) {
						SetDlgItemText(IDC_STATIC_SEND, _T("S"));
						m_pSio->m_Indicate_Send = FALSE;
					}
				} else {
					ls_IndicateS = TRUE;
					SetDlgItemText(IDC_STATIC_SEND, _T("s"));
				}
				if (ls_IndicateR == TRUE) {
					ls_IndicateR = FALSE;
					if (m_pSio->m_Indicate_Recv == TRUE) {
						SetDlgItemText(IDC_STATIC_RECV, _T("R"));
						m_pSio->m_Indicate_Recv = FALSE;
					}
				} else {
					ls_IndicateR = TRUE;
					SetDlgItemText(IDC_STATIC_RECV, _T("r"));
				}
			}
		}
		{
			CC_DispOthers();
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

/*
 *	モーションメッセージ処理
 */
LRESULT CMotSysDlg::OnMotdrv_Setwindow(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_SETWINDOW, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Setsofthome(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_SETSOFTHOME, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Emergencystop(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_EMERGENCYSTOP, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Moveatspeed(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_MOVEATSPEED, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Movetoposition(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_MOVETOPOSITION, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Getposition(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETPOSITION, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Gohome(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GOHOME, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Moveatspeedtodest(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_MOVEATSPEEDTODEST, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Isstopped(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_ISSTOPPED, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_IsJoyMode(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_ISJOYMODE, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_SetJoyMode(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_SETJOYMODE, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_GetMaxSpeed(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETMAXSPEED, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_GetMotionSize(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETMOTIONSIZE, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_GetElectronicGear(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETELECTRONICGEAR, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_GetSoftLimit(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETSOFTLIMIT, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_SetJoySpeed(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_SETJOYSPEED, wParam, lParam);
}

// メイン関数
LRESULT CMotSysDlg::MotsysDriverProc(UINT uMsg, WPARAM lParam1, LPARAM lParam2)
{
	LRESULT	lRes = 0L;

	if (uMsg == 0) {
		lRes = -1L;  // return 0L to FAIL load
	} else {
		lRes = MotsysProc(uMsg, lParam1, lParam2);
	}

	return lRes;
}
// モーション関数
LRESULT CMotSysDlg::MotsysProc(UINT uMsg, WPARAM lParam1, LPARAM lParam2)
{
	LRESULT	lRes = 0L;
	short axis;

	if		  (uMsg == MOTDRV_SETWINDOW) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_set_response_window(axis, (HWND) lParam2);
	} else if (uMsg == MOTDRV_SETSOFTHOME) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_set_soft_home(axis, lParam2);
	} else if (uMsg == MOTDRV_EMERGENCYSTOP) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_emergency_stop(axis);
	} else if (uMsg == MOTDRV_MOVEATSPEED) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_move_at_speed(axis, HIWORD(lParam1));
	} else if (uMsg == MOTDRV_MOVETOPOSITION) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_move_to_position(axis, lParam2, HIWORD(lParam1));
	} else if (uMsg == MOTDRV_GETPOSITION) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_position(axis);
	} else if (uMsg == MOTDRV_GOHOME) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_go_home(axis);
//	} else if (uMsg == MOTDRV_MOVEATSPEEDTODEST) {	これは MOTDRV_MOVETOPOSITION と同じなので使わないことにします．
//		axis = ConvAxis(LOWORD(lParam1));
//		lRes = motion_move_at_speed_to_dest(axis, lParam2, HIWORD(lParam1));
	} else if (uMsg == MOTDRV_ISSTOPPED) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_is_stopped(axis);
	} else if (uMsg == MOTDRV_ISJOYMODE) {
		lRes = motion_is_joymode();
	} else if (uMsg == MOTDRV_SETJOYMODE) {
		lRes = motion_set_joymode(lParam1, lParam2);
	} else if (uMsg == MOTDRV_GETMAXSPEED) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_maxspeed(axis);
	} else if (uMsg == MOTDRV_GETMOTIONSIZE) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_motionsize(axis);
	} else if (uMsg == MOTDRV_GETELECTRONICGEAR) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_electronicgear(axis);
	} else if (uMsg == MOTDRV_GETSOFTLIMIT) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_softlimit(axis, HIWORD(lParam1));
	} else if (uMsg == MOTDRV_SETJOYSPEED) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_set_joyspeed(axis, lParam2, HIWORD(lParam1));
	} else {
		lRes = MS_NO_FUNCTION;
	}

	return lRes;
}

void CMotSysDlg::OnCancel()
{
	if (m_OnClose == TRUE) {
		CDialog::OnCancel();
	}
}

void CMotSysDlg::OnClose()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	m_bOnClosing = TRUE;

	// サーボ OFF，全軸停止
	if (0 != g_pCCThread) {
		CC_ServoOFF_RunOFF();
	}

	/*	下記の処理で GetExitCodeThread によりスレッドの終了の確認が出来なかったため，
		このような処理にしている．
		したがって，メモリリークが発生していたら，リブートが必要かも．*/

	// 表示したままの MessageBox があればクローズさせる
	HWND m_hwnd_MsgBox;
	do {
		if ((m_hwnd_MsgBox = ::FindWindow(NULL, MSD_MESSAGEBOX_TITLE)) != NULL) {
			::PostMessage(m_hwnd_MsgBox, WM_CLOSE, 0, 0);
			// メッセージを処理させるために再度 WM_CLOSE を発行し，OS に制御を渡す
			PostMessage(WM_CLOSE, 0, 0);
			return;
		}
	} while (m_hwnd_MsgBox != NULL);

	KillTimer(ID_MSD_BASE_TIMER);

	if (0 != g_pCCThread) {
		m_pSio->EndComm();
		g_CC_ThreadLoop = FALSE;
		DWORD l_dwRc = WaitForSingleObject(g_pCCThread->m_hThread, 100);
		switch (l_dwRc) {
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			PostMessage(WM_CLOSE, 0, 0);
			return;
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
		default:
			TerminateThread(g_pCCThread, 0);
			break;
		}
		delete g_pCCThread;
		g_pCCThread = NULL;
	}

	CloseObjects();		// 各種オブジェクトを開放する

	CDialog::OnClose();
}
// 各種オブジェクトを開放する
void CMotSysDlg::CloseObjects()
{
	m_OnClose = TRUE;

	// DIO クローズ
	if (0 != m_bDIO_Opned) {
		m_bDIO_Opned = FALSE;
		if (CC_DioClose() != 0) {
			;	// クローズ失敗
		}
	}

	// シリアル通信の開放
	if (m_pSio) {
		delete m_pSio;
		m_pSio = NULL;
	}
	if (m_pComLogFile) {
		delete m_pComLogFile;
		m_pComLogFile = NULL;
	}
	if (m_pAlarmLogFile) {
		delete m_pAlarmLogFile;
		m_pAlarmLogFile = NULL;
	}

}

// 優先度コードを文字列に変換する
void CMotSysDlg::ChgToPriorityMsg(DWORD code, TCHAR *ptszmsg)
{
	switch (code) {
	case HIGH_PRIORITY_CLASS:
		_stprintf_s(ptszmsg, _countof(ptszmsg), _T("HIGH_PRIORITY_CLASS(0x%08x)"), code);
		break;
	case IDLE_PRIORITY_CLASS:
		_stprintf_s(ptszmsg, _countof(ptszmsg), _T("IDLE_PRIORITY_CLASS(0x%08x)"), code);
		break;
	case NORMAL_PRIORITY_CLASS:
		_stprintf_s(ptszmsg, _countof(ptszmsg), _T("NORMAL_PRIORITY_CLASS(0x%08x)"), code);
		break;
	case REALTIME_PRIORITY_CLASS:
		_stprintf_s(ptszmsg, _countof(ptszmsg), _T("REALTIME_PRIORITY_CLASS(0x%08x)"), code);
		break;
	default:
		_stprintf_s(ptszmsg, _countof(ptszmsg), _T("Undefined(0x%08x)"), code);
		break;
	}
}

// エラーコードからシステムのエラーメッセージを取得する
void CMotSysDlg::ShowLastError(DWORD code, TCHAR *ptszErrMsg)
{
	LPVOID lpMsgBuf;
	::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
		);
	_tcscpy(ptszErrMsg, (char*) lpMsgBuf);
	::LocalFree(lpMsgBuf);

	TCHAR *l_ptszText;
	l_ptszText = ptszErrMsg;
	while (1) {
		if ((*l_ptszText == 0x7f) || ((0x00 <= *l_ptszText) && (*l_ptszText <= 0x1f))) {
			*l_ptszText = 0x00;
			break;
		}
		l_ptszText++;
	}
}

// シリアル通信の初期化
BOOL CMotSysDlg::InitializeComm()
{
	TCHAR	l_tszText[255];
	::GetPrivateProfileString(_T("COM"), _T("No"), _T("3"), l_tszText, sizeof(l_tszText), MOT_INI);	// ポート番号読出し
	int l_PortCh = _ttoi(l_tszText);
	m_pSio = g_pCSio = new CSio(l_PortCh);	// 通信クラス作成
	if (m_pSio == NULL) {
		return FALSE;
	}
	::GetPrivateProfileString(_T("Log"), _T("ComLog"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);	// 通信ログ on/off
	m_pSio->m_LogEnable = _ttoi(l_tszText);
	if (m_pSio->m_LogEnable != 0) {
		m_pSio->m_LogEnable = TRUE;
	} else {
		m_pSio->m_LogEnable = FALSE;
	}
	::GetPrivateProfileString(_T("COM"), _T("BaudRate"), _T("38400"), l_tszText, sizeof(l_tszText), MOT_INI);	// ボーレート読出し
	int l_BaudRate = _ttoi(l_tszText);
	::GetPrivateProfileString(_T("COM"), _T("ByteSize"), _T("8"), l_tszText, sizeof(l_tszText), MOT_INI);		// データ長読出し
	int l_ByteSize = _ttoi(l_tszText);
	::GetPrivateProfileString(_T("COM"), _T("Parity"), _T("0"), l_tszText, sizeof(l_tszText), MOT_INI);		// パリティ読出し
	int l_Parity = _ttoi(l_tszText);
	switch (l_Parity) {
	case 0:	l_Parity = NOPARITY; break;
	case 1:	l_Parity = ODDPARITY; break;
	case 2:	l_Parity = EVENPARITY; break;
	case 3:	l_Parity = MARKPARITY; break;
	case 4:	l_Parity = SPACEPARITY; break;
	}
	::GetPrivateProfileString(_T("COM"), _T("StopBits"), _T("0"), l_tszText, sizeof(l_tszText), MOT_INI);		// ストップビット読出し
	int l_StopBits = _ttoi(l_tszText);
	switch (l_StopBits) {
	case 0:	l_StopBits = ONESTOPBIT; break;
	case 1:	l_StopBits = ONE5STOPBITS; break;
	case 2:	l_StopBits = TWOSTOPBITS; break;
	}
	if (m_pSio->Initialize(l_BaudRate, l_ByteSize, l_Parity, l_StopBits) != TRUE) {
		_stprintf_s(l_tszText, _countof(l_tszText), _T("Communication port COM%01d is invalid."), l_PortCh);
		if (0 == m_bOnClosing) {
			::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
		return FALSE;
	}

	return TRUE;
}

// シリアル通信のログの初期化
BOOL CMotSysDlg::InitializeCommLog()
{
	TCHAR	l_tszText[_MAX_PATH] = _T("");
	FILE	*fp;
	TCHAR	l_tsztmpfile[_MAX_FNAME] = _T("DCXXXXXX");
	BOOL	l_NoFile = TRUE;
	TCHAR	l_tszFilePath[_MAX_PATH];

	// ログディレクトリを取得
	::GetPrivateProfileString(_T("ComLog"), _T("Path"), _T("None"), l_tszText, sizeof(l_tszText), MOT_INI);
	if ((_tcscmp(l_tszText, "None") == 0) || (l_tszText[0] == 0x00)) {
		// 設定がなかった
		l_NoFile = FALSE;
	} else {
		if (_tmktemp(l_tsztmpfile) != NULL) {
			_tcscpy_s(l_tszFilePath, _countof(l_tszFilePath), l_tszText);
			_tcscat_s(l_tszText, _countof(l_tszText), l_tsztmpfile);
			if ((fp = _tfopen(l_tszText, "w")) != NULL) {
				fclose(fp);
				_tremove(l_tszText);
			} else {
				// オープン失敗
				l_NoFile = FALSE;
			}
		} else {
			// temp ファイル作成失敗
			l_NoFile = FALSE;
		}
	}
	if (l_NoFile == FALSE) {
		// MOT_INI のディレクトリ位置に作成
		TCHAR l_tszDrive[8], l_tszDir[_MAX_PATH], l_tszFName[_MAX_FNAME], l_tszExt[8];
		_tsplitpath(MOT_INI, l_tszDrive, l_tszDir, l_tszFName, l_tszExt);
		// パスとファイル名を作成
		_stprintf_s(l_tszFilePath, _countof(l_tszFilePath), _T("%s%s"), l_tszDrive, l_tszDir);
	}

	// ログファイル名を取得
	::GetPrivateProfileString(_T("ComLog"), _T("FileName"), COM_LOG_FILE, l_tszText, sizeof(l_tszText), MOT_INI);
	if (_tcscmp(l_tszText, COM_LOG_FILE) != 0) {
		_tcscpy_s(l_tsztmpfile, _countof(l_tsztmpfile), l_tszFilePath);
		_tcscat_s(l_tsztmpfile, _countof(l_tsztmpfile), l_tszText);
		if ((fp = _tfopen(l_tsztmpfile, "w")) != NULL) {
			fclose(fp);
			_tremove(l_tsztmpfile);
		} else {
			// オープン失敗
			l_NoFile = FALSE;
			_tcscpy_s(l_tsztmpfile, _countof(l_tsztmpfile), l_tszFilePath);
			_tcscat_s(l_tsztmpfile, _countof(l_tsztmpfile), COM_LOG_FILE);
		}
	}

	// 通信ログクラス作成
	m_pComLogFile = g_pComLogFile = new CLogFile(l_tszText);
	if (m_pComLogFile == NULL) {
		return FALSE;
	}
	_tcscpy(m_pComLogFile->m_tszLogFilePath, l_tszFilePath);

	return TRUE;
}

// サーボ初期化完了ハンドラ
void CMotSysDlg::OnServoInitComp(WPARAM wParam)
{
	BOOL l_bRc = (BOOL) wParam;

	if (0 == wParam) {
		// 失敗
		;	// 何もしません．
	} else {
		// 成功
		switch (m_AutoHide) {
		case 0:
			break;
		case 1:
			ShowWindow(SW_MINIMIZE);
			break;
		case 2:
		default:
			ShowWindow(SW_MINIMIZE);
			ShowWindow(SW_HIDE);
			break;
		}
	}
}

LRESULT CMotSysDlg::OnServoInitCompMsg(WPARAM wParam, LPARAM lParam)
{
	OnServoInitComp(wParam);
	return 0;
}

// アラーム表示処理
void CMotSysDlg::ShowAlarm(WORD Mode, WORD Axis, WORD Code1, WORD Code2)
{
	WPARAM	l_WParam;
	LPARAM	l_LParam;

	l_WParam = MAKEWPARAM(Axis, Mode);
	l_LParam = MAKELPARAM(Code2, Code1);

	// 異常を通知します．
	send_motion_message(-1, Mode);

	PostMessage(MOTDRV_SHOWALARM, l_WParam, l_LParam);
}
// アラーム表示メッセージ処理
void CMotSysDlg::OnShowAlarm(WPARAM wParam, LPARAM lParam)
{
	//							 ----Motion System Driver Error----
	TCHAR	l_tszText0[] = "Alarm\n----------------------------------\n";
	TCHAR	l_tszText1[255];
	TCHAR	l_tszText2[32];
	TCHAR	l_tszText3[32];
/* added 2010.02.24 hmenjo MotSys Y 軸アラームログ修正 ---------- { ---------- */
	TCHAR	l_tszText4[32];
/* added 2010.02.24 hmenjo MotSys Y 軸アラームログ修正 ---------- } ---------- */
	TCHAR	l_tszText5[255];
	WORD	l_Mode	= HIWORD(wParam);
	WORD	l_Axis	= LOWORD(wParam);
	WORD	l_Code1 = HIWORD(lParam);
	WORD	l_Code2 = LOWORD(lParam);
	int		l_MsgID;
	int		l_ret;
	TCHAR	l_tszHomeErrorMessage[255] = _T("Axis can not go home.");
	TCHAR	tszAlarmLog[255] = _T("Motion System Driver Alarm.");

	switch (l_Mode) {
	case 1:		// ステージコントローラの WD エラーが発生
		_tcscpy_s(l_tszText1, _countof(l_tszText1), l_tszText0);
		_tcscat(l_tszText1, _T("W.D. Error on Controller.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" W.D. Error on Controller."));
		break;
	case 2:		// システムエラー色々
		_tcscpy_s(l_tszText1, _countof(l_tszText1), l_tszText0);
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" System Error."));
		break;
	case 3:		// 軸アラーム色々
		switch (l_Axis) {
		case AXIS_BIG_X:		_tcscpy(l_tszText2, _T("Big X"));			break;	// Big X
		case AXIS_BIG_Y:		_tcscpy(l_tszText2, _T("Big YM"));			break;	// Big Y
		case AXIS_Z:			_tcscpy(l_tszText2, _T("Z(Elevator)"));		break;	// Z(Elevator)
		case AXIS_SLOW_X:		_tcscpy(l_tszText2, _T("X slow"));			break;	// X slow
		case AXIS_T:			_tcscpy(l_tszText2, _T("Theta"));			break;	// Theta
		case AXIS_L:			_tcscpy(l_tszText2, _T("Level"));			break;	// Level
		case AXIS_FAST_X:		_tcscpy(l_tszText2, _T("X fast"));			break;	// X fast
		default:	_tcscpy(l_tszText2, _T("Undefine"));		break;	// Undefine
		}
		CnvWordToBin(l_Code1, l_tszText3, 2);
		_stprintf_s(l_tszText1, _countof(l_tszText1), _T("%sAlarm on Axis[%d] (%s).\nAlarm code : %s"), l_tszText0, l_Axis, l_tszText2, l_tszText3);
		if (l_Axis == AXIS_BIG_Y) {
			// Y 軸の場合は YS も表示
/* modified 2010.02.24 hmenjo MotSys Y 軸アラームログ修正 ---------- { ---------- */
//			CnvWordToBin(l_Code2, l_tszText3, 2);
//			_stprintf(l_tszText5, _T("\nAlarm on Axis[%d] (%s).\nAlarm code : %s"), l_Axis, _T("Big YS"), l_tszText3);
/* modified 2010.02.24 hmenjo MotSys Y 軸アラームログ修正 ----------			  */
			CnvWordToBin(l_Code2, l_tszText4, 2);
			_stprintf(l_tszText5, _T("\nAlarm on Axis[%d] (%s).\nAlarm code : %s"), l_Axis, _T("Big YS"), l_tszText4);
/* modified 2010.02.24 hmenjo MotSys Y 軸アラームログ修正 ---------- } ---------- */
			_tcscat_s(l_tszText1, _countof(l_tszText1), l_tszText5);
		}
		l_MsgID = 1;
		// AlarmLog Message
		TCHAR tszTemp[256];
/* modified 2010.02.24 hmenjo MotSys Y 軸アラームログ修正 ---------- { ---------- */
//		  _stprintf_s(tszTemp, _countof(tszTemp), _T(" Alarm on Axis (%s). Alarm code : %s."), l_tszText2, l_tszText3);
/* modified 2010.02.24 hmenjo MotSys Y 軸アラームログ修正 ----------			  */
		_stprintf_s(tszTemp, _countof(tszTemp), _T(" Alarm on Axis (%sYS). Alarm code : %s    %s."), l_tszText2, l_tszText3, l_tszText4);
/* modified 2010.02.24 hmenjo MotSys Y 軸アラームログ修正 ---------- } ---------- */
		_tcscat_s(tszAlarmLog, _countof(tszAlarmLog), tszTemp);
		break;
	case 4:		// 初期化完了フラグが０にならなかった
		_tcscpy_s(l_tszText1, _countof(l_tszText1), l_tszText0);
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("Not be initialized on Controller.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" System Error. Not be Initialized on Controller."));
		break;
	case 5:		// DIO エラー
		_tcscpy(l_tszText1, _T("DIO error.\n----------------------------------\n"));
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("Not work DIO board.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" DIO error."));
		break;
	case 6:		// サーボ ON/OFF エラー
		_tcscpy(l_tszText1, _T("Servo ON/OFF timeout.\n----------------------------------\n"));
		_tcscat(l_tszText1, _T("System Error.\n"));
		if (l_Code1 == 0) {
			_tcscat(l_tszText1, _T("Not Servo OFF on Controller.\n"));
		} else {
			_tcscat(l_tszText1, _T("Not Servo ON on Controller.\n"));
		}
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" Servo ON/OFF timeout."));
		break;
	case 7:		// 非常停止 エラー
		_tcscpy(l_tszText1, _T("EMG error.\n----------------------------------\n"));
		_tcscat(l_tszText1, _T("EMG button.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" EMG error."));
		break;
	case 8: 	// 原点復帰 エラー	 原点復帰が行われなかった
		switch(l_Code1){
		case MS_PARAMETER_ERROR:
			_tcscat(l_tszHomeErrorMessage, _T(" Parameter error. "));
			break;
		case MS_AXIS_LOCKED:
			_tcscat(l_tszHomeErrorMessage, _T(" Axis locked. "));
			break;
		case MS_AXIS_UNINITIALIZED:
			_tcscat(l_tszHomeErrorMessage, _T(" Uninitialized. "));
			break;
		case MS_FUNCTION_BUSY:
			_tcscat(l_tszHomeErrorMessage, _T(" Function busy. "));
			break;
		default:
			_tcscat(l_tszHomeErrorMessage, _T(" Error. "));
			break;
		}
		_stprintf_s(l_tszText1, _countof(l_tszText1), _T("%sAxis=%d."), l_tszHomeErrorMessage, l_Axis);
		l_MsgID = 2;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" Axis can not go home."));
		break;
	default:
		break;
	}
	AlarmLogging(tszAlarmLog);

	switch (l_MsgID) {
	case 0:
		if (0 == m_bOnClosing) {
			static int ls_iMsgID_0 = 0;
			if (0 == ls_iMsgID_0) {ls_iMsgID_0 = 1;} else {break;}
			g_CC_CommAbort = TRUE;
			l_ret = ::MessageBox(NULL, l_tszText1, MSD_MESSAGEBOX_TITLE,
																MB_OK
															  | MB_ICONQUESTION
															  | MB_DEFBUTTON1
															  | MB_SYSTEMMODAL);
			g_CC_CommAbort = FALSE;
			if (l_ret == IDOK) {
				PostMessage(WM_CLOSE, 0, 0);
			} else {
				if (l_Mode == 6) {
					g_CC_ServoONOFFerr = FALSE;
				}
			}
			ls_iMsgID_0 = 0;
		}
		break;
	case 1:
		if (0 == m_bOnClosing) {
/* deleted 2009.06.02 hmenjo 軸アラームでは COM 通信中断せず ---------- { ---------- */
//			static ls_iMsgID_1 = 0;
//			if (0 == ls_iMsgID_1) {ls_iMsgID_1 = 1;} else {break;}
//			g_CC_CommAbort = TRUE;
/* deleted 2009.06.02 hmenjo 軸アラームでは COM 通信中断せず ---------- } ---------- */
			l_ret = ::MessageBox(NULL, l_tszText1, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
/* deleted 2009.06.02 hmenjo 軸アラームでは COM 通信中断せず ---------- { ---------- */
//			g_CC_CommAbort = FALSE;
/* deleted 2009.06.02 hmenjo 軸アラームでは COM 通信中断せず ---------- } ---------- */
			if (l_Mode == 3) {
				g_CC_bClearAlarm[CC_CnvAxisToSPT(l_Axis)] = TRUE;
			}
/* deleted 2009.06.02 hmenjo 軸アラームでは COM 通信中断せず ---------- { ---------- */
//			ls_iMsgID_1 = 0;
/* deleted 2009.06.02 hmenjo 軸アラームでは COM 通信中断せず ---------- } ---------- */
		}
		break;
	default:
		if (0 == m_bOnClosing) {
			static int ls_iMsgID_def = 0;
			if (0 == ls_iMsgID_def) {ls_iMsgID_def = 1;} else {break;}
			g_CC_CommAbort = TRUE;
			l_ret = ::MessageBox(NULL, l_tszText1, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			g_CC_CommAbort = FALSE;
			ls_iMsgID_def = 0;
		}
		break;
	}


}

LRESULT CMotSysDlg::OnShowAlarmMsg(WPARAM wParam, LPARAM lParam)
{
	OnShowAlarm(wParam, lParam);
	return 0;
}

// ワードデータを２進数表記の文字列に変換する
//	mode により分割できる．
//		=0 : 分割なし
//		=1 : 8 ビット毎に分割
//		=2 : 4 ビット毎に分割
void CMotSysDlg::CnvWordToBin(
		WORD WordData,		// 16 bit データ
		TCHAR *ptszBinary,	// 変換後データ文字列
		WORD mode			// モード(上記参照)
	)
{
	TCHAR	l_tszText1[32];
	TCHAR	l_tszText2[32];

	_ltot(WordData, l_tszText1, 2);
	memset(l_tszText2, 0, sizeof(l_tszText2));
	memcpy(l_tszText2, _T("0000000000000000"), 16 - strlen(l_tszText1));
	_tcscat_s(l_tszText2, _countof(l_tszText2), l_tszText1);
	switch (mode) {
	case 1:
		_tcscpy(l_tszText1, _T("00000000 00000000"));
		memcpy(&l_tszText1[0], &l_tszText2[0], 8);
		memcpy(&l_tszText1[9], &l_tszText2[8], 8);
		break;
	case 2:
		_tcscpy(l_tszText1, _T("0000 0000 0000 0000"));
		memcpy(&l_tszText1[0],	&l_tszText2[0],  4);
		memcpy(&l_tszText1[5],	&l_tszText2[4],  4);
		memcpy(&l_tszText1[10], &l_tszText2[8],  4);
		memcpy(&l_tszText1[15], &l_tszText2[12], 4);
		break;
	case 0:
	default:
		_tcscpy_s(l_tszText1, _countof(l_tszText1), l_tszText2);
		break;
	}
	_tcscpy_s(ptszBinary, _countof(ptszBinary), l_tszText1);
}

BOOL CMotSysDlg::DestroyWindow()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	// ウィンドウのプロパティリストから識別情報を削除
	::RemoveProp(m_hWnd, IDENT_CODE);

	return CDialog::DestroyWindow();
}

// サーボ OFF
void CMotSysDlg::OnBTNServoOFF()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	g_CC_ServoOFFReq = TRUE;
}

// サーボ ON
void CMotSysDlg::OnBTNServoON()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	g_CC_ServoONReq = TRUE;
}

// サーボパラメタ読込み
void CMotSysDlg::OnBtnServoParam()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CC_LoadServoParam();

}

// X 軸 位置偏差ピーク値をクリア
void CMotSysDlg::OnBtnDPosPeakClearX()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_BIG_X], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_BIG_X], _T("0"));
	m_dPos[1][AXIS_BIG_X] = m_dPos[2][AXIS_BIG_X] = 0;
}
// YM 軸 位置偏差ピーク値をクリア
void CMotSysDlg::OnBtnDPosPeakClearYM()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_BIG_Y], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_BIG_Y], _T("0"));
	m_dPos[1][AXIS_BIG_Y] = m_dPos[2][AXIS_BIG_Y] = 0;
}
// YS 軸 位置偏差ピーク値をクリア
void CMotSysDlg::OnBtnDPosPeakClearYS()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_NUM], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_NUM], _T("0"));
	m_dPos[1][AXIS_NUM] = m_dPos[2][AXIS_NUM] = 0;
}
// Z 軸 位置偏差ピーク値をクリア
void CMotSysDlg::OnBtnDPosPeakClearZ()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_Z], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_Z], _T("0"));
	m_dPos[1][AXIS_Z] = m_dPos[2][AXIS_Z] = 0;
}
// T 軸 位置偏差ピーク値をクリア
void CMotSysDlg::OnBtnDPosPeakClearT()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_T], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_T], _T("0"));
	m_dPos[1][AXIS_T] = m_dPos[2][AXIS_T] = 0;
}
// 各軸位置偏差を表示
void CMotSysDlg::DispdPos(WORD wAxis, long d_Pos)
{
	TCHAR	l_tszText[32];
//	short	l_sAxis = ConvAxis(wAxis);		// 不要のはず
	short	l_sAxis = wAxis;

	// リアルタイム値を更新
	m_dPos[0][l_sAxis] = d_Pos;
	_stprintf_s(l_tszText, _countof(l_tszText), _T("%11d"), m_dPos[0][l_sAxis]);
	SetDlgItemText(g_iDlgCtrlID[17][l_sAxis], l_tszText);
	// －側ピーク値を更新
	if (m_dPos[0][l_sAxis] < m_dPos[1][l_sAxis]) {
		m_dPos[1][l_sAxis] = m_dPos[0][l_sAxis];
		_stprintf_s(l_tszText, _countof(l_tszText), _T("%11d"), m_dPos[1][l_sAxis]);
		SetDlgItemText(g_iDlgCtrlID[19][l_sAxis], l_tszText);
	}
	// ＋側ピーク値を更新
	if (m_dPos[2][l_sAxis] < m_dPos[0][l_sAxis]) {
		m_dPos[2][l_sAxis] = m_dPos[0][l_sAxis];
		_stprintf_s(l_tszText, _countof(l_tszText), _T("%11d"), m_dPos[2][l_sAxis]);
		SetDlgItemText(g_iDlgCtrlID[20][l_sAxis], l_tszText);
	}
}


// 以下のコードはデバッグ用

// 原点復帰
void CMotSysDlg::OnBTNORGtest()		// 「X ORG」ボタン
{
	mmtestORGX(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestY()		// 「Y ORG」ボタン
{
	mmtestORGY(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestZ()		// 「Z ORG」ボタン
{
	mmtestORGZ(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestT()		// 「T ORG」ボタン
{
	mmtestORGT(0);	//debdeb
}
// ABS 移動
void CMotSysDlg::OnBTNABStestX()		// 「X ABS1」ボタン
{
	mmtestORGX(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestX2()		// 「X ABS2」ボタン
{
	mmtestORGX(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestY()		// 「Y ABS1」ボタン
{
	mmtestORGY(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestY2()		// 「Y ABS2」ボタン
{
	mmtestORGY(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestZ()		// 「Z ABS1」ボタン
{
	mmtestORGZ(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestZ2()		// 「Z ABS2」ボタン
{
	mmtestORGZ(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestT()		// 「T ABS1」ボタン
{
	mmtestORGT(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestT2()		// 「T ABS2」ボタン
{
	mmtestORGT(21);	//debdeb
}
// INC 移動
void CMotSysDlg::OnBTNINCtestX()		// 「X INC+」ボタン
{
	mmtestORGX(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestX2()		// 「X INC-」ボタン
{
	mmtestORGX(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestY()		// 「Y INC+」ボタン
{
	mmtestORGY(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestY2()		// 「Y INC-」ボタン
{
	mmtestORGY(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestZ()		// 「Z INC+」ボタン
{
	mmtestORGZ(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestZ2()		// 「Z INC-」ボタン
{
	mmtestORGZ(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestT()		// 「T INC+」ボタン
{
	mmtestORGT(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestT2()		// 「T INC-」ボタン
{
	mmtestORGT(31);	//debdeb
}
// HP 移動
void CMotSysDlg::OnBTNHPtestX()		// 「X HP」ボタン
{
	mmtestORGX(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestY()		// 「Y HP」ボタン
{
	mmtestORGY(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestZ()		// 「Z HP」ボタン
{
	mmtestORGZ(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestT()		// 「T HP」ボタン
{
	mmtestORGT(1);	//debdeb
}

// コントロールの Enable処理
void CMotSysDlg::EnableDlgCtrl()
{
	int l_iCtrlID[][9] = {	// 各軸ごとに最大項目数は５０個です．(g_iDlgCtrlID[][]で定義)
	/*00*/	IDC_STATIC_LBL_STS_X,	IDC_STATIC_LBL_STS_YM,	IDC_STATIC_LBL_STS_Z,	0,	IDC_STATIC_LBL_STS_T,	0,	0,	0,	IDC_STATIC_LBL_STS_YS,
	/*01*/	IDC_STATIC_AXIS_X,		IDC_STATIC_AXIS_YM,		IDC_STATIC_AXIS_Z,		0,	IDC_STATIC_AXIS_T,		0,	0,	0,	IDC_STATIC_AXIS_YS,
	/*02*/	IDC_STATIC_MODE_X,		IDC_STATIC_MODE_YM,		IDC_STATIC_MODE_Z,		0,	IDC_STATIC_MODE_T,		0,	0,	0,	0,
	/*03*/	IDC_STATIC_POS_X,		IDC_STATIC_POS_YM,		IDC_STATIC_POS_Z,		0,	IDC_STATIC_POS_T,		0,	0,	0,	IDC_STATIC_POS_YS,
	/*04*/	IDC_BTN_HPtestX,		IDC_BTN_HPtestY,		IDC_BTN_HPtestZ,		0,	IDC_BTN_HPtestT,		0,	0,	0,	0,
	/*05*/	IDC_BTN_INCtestX2,		IDC_BTN_INCtestY2,		IDC_BTN_INCtestZ2,		0,	IDC_BTN_INCtestT2,		0,	0,	0,	0,
	/*06*/	IDC_BTN_INCtestX,		IDC_BTN_INCtestY,		IDC_BTN_INCtestZ,		0,	IDC_BTN_INCtestT,		0,	0,	0,	0,
	/*07*/	IDC_BTN_ABStestX2,		IDC_BTN_ABStestY2,		IDC_BTN_ABStestZ2,		0,	IDC_BTN_ABStestT2,		0,	0,	0,	0,
	/*08*/	IDC_BTN_ABStestX,		IDC_BTN_ABStestY,		IDC_BTN_ABStestZ,		0,	IDC_BTN_ABStestT,		0,	0,	0,	0,
	/*09*/	IDC_BTN_ORGtestX,		IDC_BTN_ORGtestY,		IDC_BTN_ORGtestZ,		0,	IDC_BTN_ORGtestT,		0,	0,	0,	0,
	/*10*/	IDC_STATIC_LBL_PTN_X,	IDC_STATIC_LBL_PTN_Y,	IDC_STATIC_LBL_PTN_Z,	0,	IDC_STATIC_LBL_PTN_T,	0,	0,	0,	0,
	/*11*/	IDC_STATIC_PTN_X,		IDC_STATIC_PTN_Y,		IDC_STATIC_PTN_Z,		0,	IDC_STATIC_PTN_T,		0,	0,	0,	0,
	/*12*/	IDC_STATIC_PRC_X,		IDC_STATIC_PRC_Y,		IDC_STATIC_PRC_Z,		0,	IDC_STATIC_PRC_T,		0,	0,	0,	0,
	/*13*/	IDC_STATIC_SOFTHP_X,	IDC_STATIC_SOFTHP_Y,	IDC_STATIC_SOFTHP_Z,	0,	IDC_STATIC_SOFTHP_T,	0,	0,	0,	0,
	/*14*/	IDC_STATIC_SPEED_X,		IDC_STATIC_SPEED_Y,		IDC_STATIC_SPEED_Z,		0,	IDC_STATIC_SPEED_T,		0,	0,	0,	0,
	/*15*/	IDC_STATIC_DESTPOS_X,	IDC_STATIC_DESTPOS_Y,	IDC_STATIC_DESTPOS_Z,	0,	IDC_STATIC_DESTPOS_T,	0,	0,	0,	0,
	/*16*/	IDC_STATIC_LBL_POS_X,	IDC_STATIC_LBL_POS_YM,	IDC_STATIC_LBL_POS_Z,	0,	IDC_STATIC_LBL_POS_T,	0,	0,	0,	IDC_STATIC_LBL_POS_YS,
	/*17*/	IDC_STATIC_DPOSR_X,		IDC_STATIC_DPOSR_YM,	IDC_STATIC_DPOSR_Z,		0,	IDC_STATIC_DPOSR_T,		0,	0,	0,	IDC_STATIC_DPOSR_YS,
	/*18*/	IDC_BTN_DPOSP_CLR_X,	IDC_BTN_DPOSP_CLR_YM,	IDC_BTN_DPOSP_CLR_Z,	0,	IDC_BTN_DPOSP_CLR_T,	0,	0,	0,	IDC_BTN_DPOSP_CLR_YS,
	/*19*/	IDC_STATIC_DPOSPM_X,	IDC_STATIC_DPOSPM_YM,	IDC_STATIC_DPOSPM_Z,	0,	IDC_STATIC_DPOSPM_T,	0,	0,	0,	IDC_STATIC_DPOSPM_YS,
	/*20*/	IDC_STATIC_DPOSPP_X,	IDC_STATIC_DPOSPP_YM,	IDC_STATIC_DPOSPP_Z,	0,	IDC_STATIC_DPOSPP_T,	0,	0,	0,	IDC_STATIC_DPOSPP_YS,
			-1,						-1,						-1,						-1,	-1,						-1,	-1,	-1,	-1,
		};

	int i, j;

	// コントロール ID テーブルを設定
	memset(g_iDlgCtrlID, 0, sizeof(g_iDlgCtrlID));
	i = 0;
	while (-1 != l_iCtrlID[i][0]) {
#if CC_DEB_BUTTON==0
		if ((4 <= i) && (i <= 9)) {
			i++;
			continue;
		}
#endif
		g_iDlgCtrlID[i][AXIS_BIG_X]	= l_iCtrlID[i][0];
		g_iDlgCtrlID[i][AXIS_BIG_Y]	= l_iCtrlID[i][1];
		g_iDlgCtrlID[i][AXIS_Z]		= l_iCtrlID[i][2];
		g_iDlgCtrlID[i][AXIS_T]		= l_iCtrlID[i][4];
		g_iDlgCtrlID[i][AXIS_NUM]	= l_iCtrlID[i][8];
		i++;
	}
	for (j = 0; j <= AXIS_NUM; j++) {
		g_iDlgCtrlID[i][j] = -1;
	}

	// Enable 処理
	if (0 != motion_data[AXIS_BIG_X].bEnable) {
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_BIG_X]) {
			if (0 != g_iDlgCtrlID[i][AXIS_BIG_X]) {
				GetDlgItem(g_iDlgCtrlID[i][AXIS_BIG_X])->EnableWindow(TRUE);
			}
			i++;
		}
	}
	if (0 != motion_data[AXIS_BIG_Y].bEnable) {
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_BIG_Y]) {
			if (0 != g_iDlgCtrlID[i][AXIS_BIG_Y]) {
				GetDlgItem(g_iDlgCtrlID[i][AXIS_BIG_Y])->EnableWindow(TRUE);
			}
			i++;
		}
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_NUM]) {
			if (0 != g_iDlgCtrlID[i][AXIS_NUM]) {
				GetDlgItem(g_iDlgCtrlID[i][AXIS_NUM])->EnableWindow(TRUE);
			}
			i++;
		}
	}
	if (0 != motion_data[AXIS_Z].bEnable) {
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_Z]) {
			if (0 != g_iDlgCtrlID[i][AXIS_Z]) {
				GetDlgItem(g_iDlgCtrlID[i][2])->EnableWindow(TRUE);
			}
			i++;
		}
	}
	if (0 != motion_data[AXIS_T].bEnable) {
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_T]) {
			if (0 != g_iDlgCtrlID[i][AXIS_T]) {
				GetDlgItem(g_iDlgCtrlID[i][4])->EnableWindow(TRUE);
			}
			i++;
		}
	}
}

//	アラーム用ログ
void CMotSysDlg::AlarmLogging(TCHAR *ptszAlarmLog)
{
	if (0 == m_pAlarmLogFile) {
		return;
	}

	TCHAR	l_tszLogBuff[1024] = _T("");

	// 日付，時刻を設定
	m_pAlarmLogFile->AddTime(l_tszLogBuff, 2);
	_tcscat(l_tszLogBuff, _T(" "));
	_tcscat_s(l_tszLogBuff, _countof(l_tszLogBuff), ptszAlarmLog);

	m_pAlarmLogFile->Logging(l_tszLogBuff);
}

/* added 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- { ---------- */
void CMotSysDlg::DioLogging(DWORD dwDi, DWORD dwDo)
{
	if (0 == m_pcDioLog) {
		return;
	}

	TCHAR l_tszLogBuff[1024] = _T("");
	WORD l_wDiLData = LOWORD(dwDi);
	WORD l_wDiHData = HIWORD(dwDi);
	WORD l_wDoLData = LOWORD(dwDo);
	WORD l_wDoHData = HIWORD(dwDo);
	TCHAR l_tszDiLData[32];
	TCHAR l_tszDiHData[32];
	TCHAR l_tszDoLData[32];
	TCHAR l_tszDoHData[32];
	this->CnvWordToBin(l_wDiLData, l_tszDiLData, 1);
	this->CnvWordToBin(l_wDiHData, l_tszDiHData, 1);
	this->CnvWordToBin(l_wDoLData, l_tszDoLData, 1);
	this->CnvWordToBin(l_wDoHData, l_tszDoHData, 1);

	/* 日付，時刻を設定	*/
	m_pcDioLog->AddTime(l_tszLogBuff, 4);
	_tcscat(l_tszLogBuff, _T(" "));

	_tcscat_s(l_tszLogBuff, _countof(l_tszLogBuff), l_tszDiHData);
	_tcscat(l_tszLogBuff, _T(" "));
	_tcscat_s(l_tszLogBuff, _countof(l_tszLogBuff), l_tszDiLData);
	_tcscat(l_tszLogBuff, _T("  "));
	_tcscat_s(l_tszLogBuff, _countof(l_tszLogBuff), l_tszDoHData);
	_tcscat(l_tszLogBuff, _T(" "));
	_tcscat_s(l_tszLogBuff, _countof(l_tszLogBuff), l_tszDoLData);

	m_pcDioLog->Logging(l_tszLogBuff);
}
/* added 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- } ---------- */
