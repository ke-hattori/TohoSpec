// MaintenanceNoticeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "MaintenanceNoticeDlg.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceNoticeDlg ダイアログ


CMaintenanceNoticeDlg::CMaintenanceNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaintenanceNoticeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaintenanceNoticeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CMaintenanceNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaintenanceNoticeDlg)
	DDX_Control(pDX, IDC_MAINTENANCE_NOTICE_TEXT, m_stcMntNotice);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaintenanceNoticeDlg, CDialog)
	//{{AFX_MSG_MAP(CMaintenanceNoticeDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceNoticeDlg メッセージ ハンドラ

BOOL CMaintenanceNoticeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

//	FontCreation();

	LOGFONT lf;
	m_stcMntNotice.GetFont()->GetLogFont(&lf);
	lf.lfHeight = 34 * 10;
	lf.lfWeight = FW_BOLD;
	m_Font.CreatePointFontIndirect(&lf);
	m_stcMntNotice.SetFont(&m_Font);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}



/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceNoticeDlg::FontCreation()
{
	int sts;

	sts = m_Font.CreateFont(
			64,							// Height
			0,							// Width
			0,							// Escapement
			0,							// Orientation
			FW_HEAVY,					// Weight
										//	FW_FW_DONTCARE	0
										//	FW_THIN		100
										//	FW_EXTRALIGHT	200
										//	FW_ULTRALIGHT	200
										//	FW_LIGHT		300
										//	FW_NORMAL		400
										//	FW_REGULAR		400
										//	FW_MEDIUM		500
										//	FW_SEMIBOLD	600
										//	FW_DEMIBOLD	600
										//	FW_BOLD		700
										//	FW_EXTRABOLD	800
										//	FW_ULTRABOLD	800
										//	FW_BLACK		900
										//	FW_HEAVY		900
			FALSE,						// Italic
			FALSE,						// Underline
			0,							// StrikeOut
			DEFAULT_CHARSET,			// CharSet
										//	ANSI_CHARSET		0
										//	DEFAULT_CHARSET	1
										//	SYMBOL_CHARSET		2
										//	SHIFTJIS_CHARSET 128
										//	OEM_CHARSET		255
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



void CMaintenanceNoticeDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	//Saiki 20090529 Add ----->
	CString strBuffer;
	//Saiki 20090529 Add <-----
	if ( bShow ) {
		// 中央に表示
		CenterWindow();

		// 巨大化して表示
		CRect rect;
		AfxGetApp()->m_pMainWnd->GetWindowRect(&rect);
		rect.left += 100;
		rect.top += 100;
		rect.right -= 100;
		rect.bottom -= 100;
		MoveWindow(&rect);
		GetClientRect(&rect);

//		rect.bottom -= 20;
		m_stcMntNotice.MoveWindow(&rect);
/* 2009.04.30 文言変更（リスクアセスメント後の指摘対応）
		m_stcMntNotice.SetWindowText("装置内に人がいます"); */
		//Saiki 20090529 Change ----->
		//m_stcMntNotice.SetWindowText("装置メンテナンスモード中。\n人がいる可能性があります。\n確認してください。");
		LoadStringML(IDS_EQ_MAINTE_MODE_CHECK, strBuffer, "Equipment Maintenance Mode. \nSome people may be in the Equipment. \nPlease check.");
		m_stcMntNotice.SetWindowText(strBuffer);
		//Saiki 20090529 Change <-----
		m_stcMntNotice.SetBkColor(RED_COLOR, TRUE);
	}
}



void CMaintenanceNoticeDlg::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

//タイマー処理なし


	CDialog::OnTimer(nIDEvent);
}

int CMaintenanceNoticeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください


	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMaintenanceNoticeDlg::Create(UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/)
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::Create(nIDTemplate, pParentWnd);

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// NanoSpec終了のMessageBoxが隠れてしまうのを回避する為に使用
void CMaintenanceNoticeDlg::ChangeTopMost(BOOL bTopMost)
{
	if(bTopMost == TRUE)
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else
	{
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}



// =========================================================================
//
void CMaintenanceNoticeDlg::OnOK()
{
	return;
	CDialog::OnOK();
}

// =========================================================================
//
void CMaintenanceNoticeDlg::OnCancel()
{
	CDialog::OnCancel();
}

// =========================================================================
//
void CMaintenanceNoticeDlg::OnClose()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	CDialog::OnClose();
}

// =========================================================================
//
BOOL CMaintenanceNoticeDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}
