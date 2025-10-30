// MotSysCfgDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MotSysCfg.h"
#include "MotSysCfgDlg.h"
#include "MotCfg.h"
#include "AxisDlg.h"
#include "ServoParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CMotSysCfgDlg ダイアログ

CMotSysCfgDlg::CMotSysCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotSysCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotSysCfgDlg)
	m_chkMotCfg = FALSE;
	m_chkBigX = FALSE;
	m_chkBigY = FALSE;
	m_chkLevel = FALSE;
	m_chkTheta = FALSE;
	m_chkXFast = FALSE;
	m_chkXSlow = FALSE;
	m_chkY = FALSE;
	m_chkZ = FALSE;
	m_chkServo = FALSE;
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMotSysCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotSysCfgDlg)
	DDX_Check(pDX, IDC_CHK_MOTCFG, m_chkMotCfg);
	DDX_Check(pDX, IDC_CHK_BIGX, m_chkBigX);
	DDX_Check(pDX, IDC_CHK_BIGY, m_chkBigY);
	DDX_Check(pDX, IDC_CHK_LEVEL, m_chkLevel);
	DDX_Check(pDX, IDC_CHK_THETA, m_chkTheta);
	DDX_Check(pDX, IDC_CHK_XFAST, m_chkXFast);
	DDX_Check(pDX, IDC_CHK_XSLOW, m_chkXSlow);
	DDX_Check(pDX, IDC_CHK_Y, m_chkY);
	DDX_Check(pDX, IDC_CHK_Z, m_chkZ);
	DDX_Check(pDX, IDC_CHK_SERVO, m_chkServo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMotSysCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CMotSysCfgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	ON_BN_CLICKED(IDC_CHK_MOTCFG, OnChkMotcfg)
	ON_BN_CLICKED(IDC_CHK_BIGX, OnChkBigx)
	ON_BN_CLICKED(IDC_CHK_BIGY, OnChkBigy)
	ON_BN_CLICKED(IDC_CHK_Z, OnChkZ)
	ON_BN_CLICKED(IDC_CHK_XSLOW, OnChkXslow)
	ON_BN_CLICKED(IDC_CHK_THETA, OnChkTheta)
	ON_BN_CLICKED(IDC_CHK_LEVEL, OnChkLevel)
	ON_BN_CLICKED(IDC_CHK_XFAST, OnChkXfast)
	ON_BN_CLICKED(IDC_CHK_Y, OnChkY)
	ON_BN_CLICKED(IDC_CHK_SERVO, OnChkServo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotSysCfgDlg メッセージ ハンドラ

BOOL CMotSysCfgDlg::OnInitDialog()
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
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定

	// TODO: 特別な初期化を行う時はこの場所に追加してください。

	// ウィンドウのプロパティリストに識別情報を追加
	::SetProp(m_hWnd, IDENT_CODE, (HANDLE) 1);

	// 変数の初期化
	m_OnClose = FALSE;
	m_hMainIcon = m_hIcon;
	m_pCMotCfg = NULL;
	m_pCServoParam = NULL;
	m_pCAxisDlgBigX = NULL;
	m_pCAxisDlgBigY = NULL;
	m_pCAxisDlgZ = NULL;
	m_pCAxisDlgXslow = NULL;
	m_pCAxisDlgT = NULL;
	m_pCAxisDlgL = NULL;
	m_pCAxisDlgXfast = NULL;
	m_pCAxisDlgY = NULL;

	// ボタンに ICON を設定
	((CButton*) GetDlgItem(IDC_BTN_ABOUT))->SetIcon(m_hIcon);

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CMotSysCfgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMotSysCfgDlg::OnPaint()
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
HCURSOR CMotSysCfgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMotSysCfgDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	// "リターン"キーで終了するのを回避するため削除
//	CDialog::OnOK();
}

void CMotSysCfgDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	if (m_OnClose == TRUE) {
		CDialog::OnCancel();
	}
}

void CMotSysCfgDlg::OnClose()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	m_OnClose = TRUE;

	CDialog::OnClose();
}

void CMotSysCfgDlg::OnBtnAbout()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	// アバウトを表示
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CMotSysCfgDlg::OnChkMotcfg()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkMotCfg == FALSE) {
		m_chkMotCfg = TRUE;
		if (m_pCMotCfg == NULL) {
			m_pCMotCfg = new CMotCfg;
			m_pCMotCfg->Create(this);
		} else {
			m_pCMotCfg->ShowWindow(SW_SHOW);
			m_pCMotCfg->BringWindowToTop();
		}
	} else {
		m_chkMotCfg = FALSE;
		if (m_pCMotCfg != NULL) {
			m_pCMotCfg->ShowWindow(SW_HIDE);
		}
	}
}
void CMotSysCfgDlg::EndMotCfgDlg()
{
	m_pCMotCfg = NULL;
}

void CMotSysCfgDlg::OnChkBigx()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkBigX == FALSE) {
		m_chkBigX = TRUE;
		if (m_pCAxisDlgBigX == NULL) {
			m_pCAxisDlgBigX = new CAxisDlg;
			m_pCAxisDlgBigX->Create(this, 0);
		} else {
			m_pCAxisDlgBigX->ShowWindow(SW_SHOW);
			m_pCAxisDlgBigX->BringWindowToTop();
		}
	} else {
		m_chkBigX = FALSE;
		if (m_pCAxisDlgBigX != NULL) {
			m_pCAxisDlgBigX->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::OnChkBigy()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkBigY == FALSE) {
		m_chkBigY = TRUE;
		if (m_pCAxisDlgBigY == NULL) {
			m_pCAxisDlgBigY = new CAxisDlg;
			m_pCAxisDlgBigY->Create(this, 1);
		} else {
			m_pCAxisDlgBigY->ShowWindow(SW_SHOW);
			m_pCAxisDlgBigY->BringWindowToTop();
		}
	} else {
		m_chkBigY = FALSE;
		if (m_pCAxisDlgBigY != NULL) {
			m_pCAxisDlgBigY->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::OnChkZ()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkZ == FALSE) {
		m_chkZ = TRUE;
		if (m_pCAxisDlgZ == NULL) {
			m_pCAxisDlgZ = new CAxisDlg;
			m_pCAxisDlgZ->Create(this, 2);
		} else {
			m_pCAxisDlgZ->ShowWindow(SW_SHOW);
			m_pCAxisDlgZ->BringWindowToTop();
		}
	} else {
		m_chkZ = FALSE;
		if (m_pCAxisDlgZ != NULL) {
			m_pCAxisDlgZ->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::OnChkXslow()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkXSlow == FALSE) {
		m_chkXSlow = TRUE;
		if (m_pCAxisDlgXslow == NULL) {
			m_pCAxisDlgXslow = new CAxisDlg;
			m_pCAxisDlgXslow->Create(this, 3);
		} else {
			m_pCAxisDlgXslow->ShowWindow(SW_SHOW);
			m_pCAxisDlgXslow->BringWindowToTop();
		}
	} else {
		m_chkXSlow = FALSE;
		if (m_pCAxisDlgXslow != NULL) {
			m_pCAxisDlgXslow->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::OnChkTheta()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkTheta == FALSE) {
		m_chkTheta = TRUE;
		if (m_pCAxisDlgT == NULL) {
			m_pCAxisDlgT = new CAxisDlg;
			m_pCAxisDlgT->Create(this, 4);
		} else {
			m_pCAxisDlgT->ShowWindow(SW_SHOW);
			m_pCAxisDlgT->BringWindowToTop();
		}
	} else {
		m_chkTheta = FALSE;
		if (m_pCAxisDlgT != NULL) {
			m_pCAxisDlgT->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::OnChkLevel()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkLevel == FALSE) {
		m_chkLevel = TRUE;
		if (m_pCAxisDlgL == NULL) {
			m_pCAxisDlgL = new CAxisDlg;
			m_pCAxisDlgL->Create(this, 5);
		} else {
			m_pCAxisDlgL->ShowWindow(SW_SHOW);
			m_pCAxisDlgL->BringWindowToTop();
		}
	} else {
		m_chkLevel = FALSE;
		if (m_pCAxisDlgL != NULL) {
			m_pCAxisDlgL->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::OnChkXfast()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkXFast == FALSE) {
		m_chkXFast = TRUE;
		if (m_pCAxisDlgXfast == NULL) {
			m_pCAxisDlgXfast = new CAxisDlg;
			m_pCAxisDlgXfast->Create(this, 6);
		} else {
			m_pCAxisDlgXfast->ShowWindow(SW_SHOW);
			m_pCAxisDlgXfast->BringWindowToTop();
		}
	} else {
		m_chkXFast = FALSE;
		if (m_pCAxisDlgXfast != NULL) {
			m_pCAxisDlgXfast->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::OnChkY()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkY == FALSE) {
		m_chkY = TRUE;
		if (m_pCAxisDlgY == NULL) {
			m_pCAxisDlgY = new CAxisDlg;
			m_pCAxisDlgY->Create(this, 7);
		} else {
			m_pCAxisDlgY->ShowWindow(SW_SHOW);
			m_pCAxisDlgY->BringWindowToTop();
		}
	} else {
		m_chkY = FALSE;
		if (m_pCAxisDlgY != NULL) {
			m_pCAxisDlgY->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::EndAxisDlg(int AxisNo)
{
	switch (AxisNo) {
	case 0:	m_pCAxisDlgBigX = NULL; break;	// BigX
	case 1:	m_pCAxisDlgBigY = NULL; break;	// BigY
	case 2:	m_pCAxisDlgZ = NULL; break;	// Z
	case 3:	m_pCAxisDlgXslow = NULL; break;	// X slow
	case 4:	m_pCAxisDlgT = NULL; break;	// Theta
	case 5:	m_pCAxisDlgL = NULL; break;	// Level
	case 6:	m_pCAxisDlgXfast = NULL; break;	// X fast
	case 7:	m_pCAxisDlgY = NULL; break;	// Y(なし)
	default:	// なし
		break;
	}
}

void CMotSysCfgDlg::OnChkServo()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (m_chkServo == FALSE) {
		m_chkServo = TRUE;
		if (m_pCServoParam == NULL) {
			m_pCServoParam = new CServoParam;
			m_pCServoParam->Create(this);
		} else {
			m_pCServoParam->ShowWindow(SW_SHOW);
			m_pCServoParam->BringWindowToTop();
		}
	} else {
		m_chkServo = FALSE;
		if (m_pCServoParam != NULL) {
			m_pCServoParam->ShowWindow(SW_HIDE);
		}
	}
}

void CMotSysCfgDlg::EndServoParam()
{
	m_pCServoParam = NULL;
}

BOOL CMotSysCfgDlg::DestroyWindow()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	// ウィンドウのプロパティリストから識別情報を削除
	::RemoveProp(m_hWnd, IDENT_CODE);

	return CDialog::DestroyWindow();
}
