// ExecutionDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "ExecutionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExecutionDlg ダイアログ

/////////////////////////////////////////////////////////////////////////////
//
CExecutionDlg::CExecutionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExecutionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExecutionDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	FontCreation();
	memset(m_szCaption, 0, sizeof(m_szCaption));
	memset(m_szMessage, 0, sizeof(m_szMessage));
}

/////////////////////////////////////////////////////////////////////////////
//
CExecutionDlg::~CExecutionDlg()
{
	m_Font.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExecutionDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CExecutionDlg, CDialog)
	//{{AFX_MSG_MAP(CExecutionDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExecutionDlg メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
BOOL CExecutionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Kojika 20090528 Add
	CString l_strBuffer;
	// Kojika 20090528 Add End

	///// Pointer /////
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);

	///// Font /////
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_EXECUTE_MESSAGE);
	pStatic->SetFont(&m_Font);

	// Kojika 20090528 Change
	//if(strcmp(m_szCaption, IDS_EXECUTION_EXIT_CAPTION) == 0)
	//{
	//	GetDlgItem(IDOK)->ShowWindow(FALSE);
	//}
	LoadStringML(IDS_QUIT_PROCESS , l_strBuffer, "QUIT PROCESS");
	if(strcmp(m_szCaption, l_strBuffer) == 0)
	{
		GetDlgItem(IDOK)->ShowWindow(FALSE);
	}
	// Kojika 20090528 Change End

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	///// Caption /////
	SetWindowText(m_szCaption);

	///// Message /////
	SetDlgItemText(IDC_EXECUTE_MESSAGE, m_szMessage);
}

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::OnOK()
{


	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::OnCancel()
{





	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color)
{
	pBrush->DeleteObject();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(Color);
	pBrush->CreateSolidBrush(Color);
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CExecutionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:

		break;
	}

	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::OnTimer(UINT nIDEvent)
{







	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::FontCreation()
{
	int sts;

	sts = m_Font.CreateFont(
			18,							// Height
			0,							// Width
			0,							// Escapement
			0,							// Orientation
			FW_NORMAL,					// Weight
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

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::SetCaption(LPCTSTR lpszCaption)
{
	memset(m_szCaption, 0, sizeof(m_szCaption));

	strncpy(m_szCaption, lpszCaption, MAX_PATH);
}

/////////////////////////////////////////////////////////////////////////////
//
void CExecutionDlg::SetMessage(LPCTSTR lpszMessage)
{
	memset(m_szMessage, 0, sizeof(m_szMessage));

	strncpy(m_szMessage, lpszMessage, MAX_PATH);
}
