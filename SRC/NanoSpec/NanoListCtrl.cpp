// CNanoListCtrl.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "NanoSpec.h"
#include "NanoListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ##########################################################################
// CNanoListCtrl
// ##########################################################################

CNanoListCtrl::CNanoListCtrl()
{
	FontCreation();
}

CNanoListCtrl::~CNanoListCtrl()
{
}


BEGIN_MESSAGE_MAP(CNanoListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CNanoListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CNanoListCtrl::SetViewStyle()
{
	///// Font Style /////
	SetFont(&m_listFont);

	///// Modify Style /////
	DWORD dwStyle = 0;
	dwStyle |= LVS_SHOWSELALWAYS;
	dwStyle |= LVS_SINGLESEL;
	ModifyStyle(0, dwStyle);

	///// Extended Style /////
	dwStyle = 0;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;
	SetExtendedStyle(dwStyle);
}

// =========================================================================
//
void CNanoListCtrl::FontCreation()
{
	m_listFont.CreateFont(
			12,							// Height
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
