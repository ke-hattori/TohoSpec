// NanoSpecTitleDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpecTitle.h"
#include "NanoSpecTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleDlg ダイアログ                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CNanoSpecTitleDlg::CNanoSpecTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNanoSpecTitleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNanoSpecTitleDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	FontCreation();
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNanoSpecTitleDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_wndProgress);
	DDX_Control(pDX, IDC_MESSAGE2, m_stcMessage2);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CNanoSpecTitleDlg, CDialog)
	//{{AFX_MSG_MAP(CNanoSpecTitleDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SET_PROGRESS, OnSetProgressPos)
	ON_MESSAGE(WM_SET_MESSAGE2, OnSetMessage2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleDlg メッセージ ハンドラ                                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CNanoSpecTitleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
/* added 2014.11.21 hmenjo 3100 アイコン(TITLE) ---------- { ---------- */
	CString l_strTmp = AfxGetApp()->m_pszHelpFilePath;
	CString l_strDirProc = l_strTmp.Left(l_strTmp.ReverseFind(_T('\\')) + 1);
	CString l_strPathNanoIni;
	if (g_lAppNameType == APP_NAME_TOHO) {
		l_strPathNanoIni.Format(_T("%s%s%sSpec.ini"), l_strDirProc, CFG_DIR, g_lpszAppPrefix4[APP_NAME_TOHO]);
	} else {
		l_strPathNanoIni.Format(_T("%s%s%sSpec.ini"), l_strDirProc, CFG_DIR, g_lpszAppPrefix4[APP_NAME_NANO]);
	}
	long l_lModelType = ::GetPrivateProfileInt(INISECTION_MODELTYPE, INIKEY_MODELTYPE_TYPE, MODEL_M6500, l_strPathNanoIni);
/* added 2014.11.21 hmenjo 3100 アイコン(TITLE) ---------- } ---------- */
/* modified 2014.11.21 hmenjo 3100 アイコン(TITLE) ---------- { ---------- */
//	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
//	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
/* modified 2014.11.21 hmenjo 3100 アイコン(TITLE) ----------              */
	BOOL l_bIconNormal = TRUE;
	if (l_lModelType == MODEL_T3100) {
#if 0
		/* リソースへのアイコンの登録が必要です．	*/
		HICON l_hIcon = 0;
		l_hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_3100));
		if (0 != l_hIcon) {
			l_bIconNormal = FALSE;
			this->SetIcon(l_hIcon, TRUE);			/* 大きいアイコンを設定	*/
			/* 小さいアイコンは，システムが自動で探してくれます．	*/
		}
#else
		/* アイコンをファイルから読込みます．	*/
		CString l_strPathIcon = l_strDirProc + _T("3100.ico");
		HICON l_hIcon32 = 0;
		HICON l_hIcon16 = 0;
		l_hIcon32 = (HICON) ::LoadImage(
								NULL,
								l_strPathIcon,
								IMAGE_ICON,
								32, 32,
								LR_LOADFROMFILE
							);
		l_hIcon16 = (HICON) ::LoadImage(
								NULL,
								l_strPathIcon,
								IMAGE_ICON,
								16, 16,
								LR_LOADFROMFILE
							);
		if ((0 != l_hIcon32) && (0 != l_hIcon16)) {
			l_bIconNormal = FALSE;
			this->SetIcon(l_hIcon32, TRUE);			/* 大きいアイコンを設定	*/
			this->SetIcon(l_hIcon16, FALSE);		/* 小さいアイコンを設定	*/
		}
#endif
	}
	if (FALSE != l_bIconNormal) {
		this->SetIcon(m_hIcon, TRUE);		/* 大きいアイコンを設定	*/
		this->SetIcon(m_hIcon, FALSE);		/* 小さいアイコンを設定	*/
	}
/* modified 2014.11.21 hmenjo 3100 アイコン(TITLE) ---------- } ---------- */

	///// Caption /////
	SetWindowText(NANOSPEC_TITLE_WINDOW_NAME);
/* modified 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- { ---------- */
//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
/* modified 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- 			 */
	int l_iDesktopSizeX = GetSystemMetrics(SM_CXFULLSCREEN);
	int l_iDesktopSizeY = GetSystemMetrics(SM_CYFULLSCREEN);
	RECT l_rectSize; GetWindowRect(&l_rectSize);
	int l_lPosX, l_lPosY;
	if (l_iDesktopSizeX < (l_rectSize.right - l_rectSize.left)) {
		l_lPosX = 0;
	} else {
		l_lPosX = (l_iDesktopSizeX - (l_rectSize.right - l_rectSize.left)) / 2;
	}
	if (((l_iDesktopSizeY / 2) - 100) < (l_rectSize.bottom - l_rectSize.top)) {
		l_lPosY = 0;
	} else {
		l_lPosY = (l_iDesktopSizeY / 2) - 100 - (l_rectSize.bottom - l_rectSize.top);
	}
	SetWindowPos(&wndTopMost, l_lPosX, l_lPosY, 0, 0, SWP_NOSIZE);
/* modified 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- } ---------- */

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
	CString strBuffer;
	if(g_lAppNameType != APP_NAME_NANO){
		GetDlgItemText(IDC_MESSAGE, strBuffer);
		strBuffer.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
		SetDlgItemText(IDC_MESSAGE, strBuffer);
	}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

	///// Font /////
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_MESSAGE);
	pStatic->SetFont(&m_Font);

	GetDlgItem(IDOK)->ShowWindow(FALSE);

/* added 2009.07.01 hmenjo タイトルウィンドウなにがなんでも最前面 ---------- { ---------- */
	this->SetTimer(110, 300, 0);
/* added 2009.07.01 hmenjo タイトルウィンドウなにがなんでも最前面 ---------- } ---------- */

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::OnPaint()
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

/////////////////////////////////////////////////////////////////////////////
//
HCURSOR CNanoSpecTitleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
/* deleted 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- { ---------- */
//	CRect rect;
//	AfxGetApp()->m_pMainWnd->GetWindowRect(&rect);
//	rect.top -= 200;
//	rect.bottom -= 200;
//	MoveWindow(&rect);
/* deleted 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- } ---------- */
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::OnClose()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	CDialog::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::FontCreation()
{
	int sts;

	sts = m_Font.CreateFont(
			36,							// Height
			0,							// Width
			0,							// Escapement
			0,							// Orientation
			FW_BOLD,					// Weight
										//	FW_FW_DONTCARE	0
										//	FW_THIN			100
										//	FW_EXTRALIGHT	200
										//	FW_ULTRALIGHT	200
										//	FW_LIGHT		300
										//	FW_NORMAL		400
										//	FW_REGULAR		400
										//	FW_MEDIUM		500
										//	FW_SEMIBOLD		600
										//	FW_DEMIBOLD		600
										//	FW_BOLD			700
										//	FW_EXTRABOLD	800
										//	FW_ULTRABOLD	800
										//	FW_BLACK		900
										//	FW_HEAVY		900
			FALSE,						// Italic
			FALSE,						// Underline
			0,							// StrikeOut
			DEFAULT_CHARSET,			// CharSet
										//	ANSI_CHARSET		0
										//	DEFAULT_CHARSET		1
										//	SYMBOL_CHARSET		2
										//	SHIFTJIS_CHARSET	128
										//	OEM_CHARSET			255
										//	HANGUL_CHARSET
			OUT_DEFAULT_PRECIS,			// OutPrecision
										//	OUT_CHARACTER_PRECIS
										//	OUT_STRING_PRECIS
										//	OUT_DEFAULT_PRECIS
										//	OUT_STROKE_PRECIS
										//	OUT_DEVICE_PRECIS
										//	OUT_TT_PRECIS
										//	OUT_RASTER_PRECIS
										//	DEFAULT_QUALITY
			CLIP_DEFAULT_PRECIS,		// ClipPrecision
										//	CLIP_CHARACTER_PRECIS
										//	CLIP_MASK
										//	CLIP_DEFAULT_PRECIS
										//	CLIP_STROKE_PRECIS
										//	CLIP_ENCAPSULATE
										//	CLIP_TT_ALWAYS
										//	CLIP_LH_ANGLES
			DEFAULT_QUALITY,			// Quality
										//	DEFAULT_QUALITY
										//	DRAFT_QUALITY
										//	PROOF_QUALITY
			FIXED_PITCH | FF_SCRIPT,	// PitchAndFamily
										//	(BYTE Low2bit)
										//		DEFAULT_PITCH
										//		FIXED_PITCH
										//		VARIABLE_PITCH
										//	(HIGH4bit)
										//		FF_DECORATIVE
										//		FF_DONTCARE
										//		FF_SCRIPT
										//		FF_MODERN
										//		FF_ROMAN
										//		FF_SWISS
//			"Arial Black"				// Facename
			"MS UI Gothic"				// Facename
										//	"Terminal"
										//	"MS Sans Serif"
										//	"Arial"
										//	"batangche" (Hangul)
										//	"MS ゴシック"
										//	"MS Pゴシック"
										//	"MS UI Gothic"
										//	"Times New Roman"
	);
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CNanoSpecTitleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		///// Message /////
//		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_MESSAGE)->GetSafeHwnd()){
//			ControlColor(pDC, &m_Brush_Message, RED);
//			return m_Brush_Message;
//		}
		break;
	}

	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color)
{
	pBrush->DeleteObject();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(Color);
	pBrush->CreateSolidBrush(Color);
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecTitleDlg::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

/* added 2009.07.01 hmenjo タイトルウィンドウなにがなんでも最前面 ---------- { ---------- */
	switch (nIDEvent) {
	case 110:
		{
/* deleted 2016.08.26 hmenjo 起動時ダンマリ対策 ---------- { ---------- */
//			static int l_iPrc = 0;
/* deleted 2016.08.26 hmenjo 起動時ダンマリ対策 ---------- } ---------- */
			HWND l_hCurrWnd;
/* deleted 2016.08.26 hmenjo 起動時ダンマリ対策 ---------- { ---------- */
//			switch (l_iPrc) {
//			case 0:
/* deleted 2016.08.26 hmenjo 起動時ダンマリ対策 ---------- } ---------- */
				l_hCurrWnd = ::GetForegroundWindow();
				if ((0 != l_hCurrWnd) && (l_hCurrWnd != this->m_hWnd)) {
					DWORD l_dwForeProcID = ::GetWindowThreadProcessId(l_hCurrWnd, 0);
					if (0 != l_dwForeProcID) {
						if (0 != ::AttachThreadInput(::GetCurrentThreadId(), l_dwForeProcID, TRUE)) {
							if (0 != ::SetForegroundWindow(this->m_hWnd)) {
								this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
/* deleted 2016.08.26 hmenjo 起動時ダンマリ対策 ---------- { ---------- */
//								l_iPrc = 1;
/* deleted 2016.08.26 hmenjo 起動時ダンマリ対策 ---------- } ---------- */
							}
							::AttachThreadInput(::GetCurrentThreadId(), l_dwForeProcID, FALSE);
						}
					}
				}
/* deleted 2016.08.26 hmenjo 起動時ダンマリ対策 ---------- { ---------- */
//				break;
//			case 1:
//				break;
//			}
/* deleted 2016.08.26 hmenjo 起動時ダンマリ対策 ---------- } ---------- */
		}
		break;
	default:
		break;
	}
/* added 2009.07.01 hmenjo タイトルウィンドウなにがなんでも最前面 ---------- } ---------- */

	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CNanoSpecTitleDlg::OnSetProgressPos(WPARAM wparam, LPARAM lparam)
{
	m_wndProgress.SetPos(int(wparam));

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CNanoSpecTitleDlg::OnSetMessage2(WPARAM wparam, LPARAM lparam)
{
	CString strBuffer;

	strBuffer.LoadString(int(wparam));
	m_stcMessage2.SetWindowText(strBuffer);

	return 0L;
}
