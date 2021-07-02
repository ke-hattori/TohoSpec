// ConfirmDlg.cpp : implementation
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "ConfirmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg dialog


CConfirmDlg::CConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfirmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfirmDlg)
	//}}AFX_DATA_INIT
	m_strMessageCaption = _T("");
	m_strMessageText = _T("");
	m_hMessageIcon = NULL;
	m_nButtonStyle = MB_OKCANCEL;

	FontCreation();
}

/////////////////////////////////////////////////////////////////////////////
//
CConfirmDlg::~CConfirmDlg()
{
	m_Font.DeleteObject();
}

void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmDlg)
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_MESSAGE_ICON, m_MessageIcon);
	DDX_Control(pDX, IDC_MESSAGE_TEXT_STATIC, m_MessageText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(CConfirmDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// OnInitDialog
BOOL CConfirmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);

	///// Font /////
	m_MessageText.SetFont(&m_Font);

	// Text
	m_MessageText.SetWindowText(m_strMessageText);

	// Caption
	this->SetWindowText(m_strMessageCaption);

	// Icon
	m_MessageIcon.SetIcon(m_hMessageIcon);

	// Button Style
	CString strBuffer1;
	CString strBuffer2;

	if(m_nButtonStyle == MB_OK){
		LoadStringML(IDS_OK, strBuffer1, "OK");
		LoadStringML(IDS_CANCEL, strBuffer2, "Cancel");
		m_OkButton.SetWindowText(strBuffer1);
		m_CancelButton.SetWindowText(strBuffer2);

		CRect rcDlg;
		CRect rcBtn;
		CRect rcMoveCenter;
		this->GetWindowRect(&rcDlg);
		m_OkButton.GetWindowRect(&rcBtn);
		this->ScreenToClient(&rcBtn);

		// center of a dialog.
		rcMoveCenter = rcBtn;
		rcMoveCenter.left = (rcDlg.Width() - rcBtn.Width()) / 2;
		rcMoveCenter.right = rcMoveCenter.left + rcBtn.Width();

		m_OkButton.MoveWindow(rcMoveCenter);
		m_CancelButton.ShowWindow(SW_HIDE);
	}
	else if(m_nButtonStyle == MB_YESNO){
		LoadStringML(IDS_YES, strBuffer1, "YES");
		LoadStringML(IDS_NO, strBuffer2, "NO");
		m_OkButton.SetWindowText(strBuffer1);
		m_CancelButton.SetWindowText(strBuffer2);
	}
	else{
		// MB_OKCANCEL
		LoadStringML(IDS_OK, strBuffer1, "OK");
		LoadStringML(IDS_CANCEL, strBuffer2, "Cancel");
		m_OkButton.SetWindowText(strBuffer1);
		m_CancelButton.SetWindowText(strBuffer2);
	}

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
//
void CConfirmDlg::OnOK()
{
	int nResult = IDOK;

	if(m_nButtonStyle == MB_YESNO){
		nResult = IDYES;
	}
	else{
		// MB_OK or MB_OKCANCEL
	}

	CDialog::EndDialog(nResult);
}

/////////////////////////////////////////////////////////////////////////////
//
void CConfirmDlg::OnCancel()
{
	int nResult = IDCANCEL;

	if(m_nButtonStyle == MB_YESNO){
		nResult = IDNO;
	}
	else{
		// MB_OKCANCEL
	}

	CDialog::EndDialog(nResult);
}

/////////////////////////////////////////////////////////////////////////////
// SetButtonStyle
void CConfirmDlg::SetButtonStyle(UINT nButtonStyle)
{
	m_nButtonStyle = nButtonStyle;
}

/////////////////////////////////////////////////////////////////////////////
// SetMessageText
void CConfirmDlg::SetMessageText(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nIconType)
{
	LPCTSTR lpIconName;

	// Text
	m_strMessageText = lpszText;

	// Caption
	m_strMessageCaption = lpszCaption;

	// Icon
	switch( nIconType ) {
	case MB_ICONEXCLAMATION:
		lpIconName = IDI_EXCLAMATION;
		break;
	case MB_ICONINFORMATION:
		lpIconName = IDI_ASTERISK;
		break;
	case MB_ICONQUESTION:
		lpIconName = IDI_QUESTION;
		break;
	case MB_ICONSTOP:
		lpIconName = IDI_HAND;
		break;
	default:
		lpIconName = IDI_EXCLAMATION;
		break;
	}

	m_hMessageIcon = ::LoadIcon(NULL, lpIconName);
}

/////////////////////////////////////////////////////////////////////////////
//
void CConfirmDlg::FontCreation()
{
	int sts;

	sts = m_Font.CreateFont(
			14,							// Height
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
