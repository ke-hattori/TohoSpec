// NanoUI.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "NanoUI.h"

IMPLEMENT_DYNAMIC(CNanoUI, CFormView)

CNanoUI::CNanoUI(LPCTSTR lpszTemplateName) : CFormView(lpszTemplateName)
{
}

CNanoUI::CNanoUI(UINT nIDTemplate) : CFormView(nIDTemplate)
{
}

CNanoUI::~CNanoUI()
{
}
void CNanoUI::FontCreation(int nHeight /*= 12*/)
{
	int sts;

	sts = m_Font.CreateFont(
			nHeight,					// Height
			0,							// Width
			0,							// Escapement
			0,							// Orientation
			FW_NORMAL,					// Weight
										//	FW_FW_DONTCARE	0
										//	FW_THIN 	100
										//	FW_EXTRALIGHT	200
										//	FW_ULTRALIGHT	200
										//	FW_LIGHT		300
										//	FW_NORMAL		400
										//	FW_REGULAR		400
										//	FW_MEDIUM		500
										//	FW_SEMIBOLD 600
										//	FW_DEMIBOLD 600
										//	FW_BOLD 	700
										//	FW_EXTRABOLD	800
										//	FW_ULTRABOLD	800
										//	FW_BLACK		900
										//	FW_HEAVY		900
			FALSE,						// Italic
			FALSE,						// Underline
			0,							// StrikeOut
			DEFAULT_CHARSET,			// CharSet
										//	ANSI_CHARSET		0
										//	DEFAULT_CHARSET 1
										//	SYMBOL_CHARSET		2
										//	SHIFTJIS_CHARSET 128
										//	OEM_CHARSET 	255
										//	HANGUL_CHARSET
			OUT_DEFAULT_PRECIS, 		// OutPrecision
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
										//	"Arial"
										//	"Times New Roman"
	);
}

BOOL CNanoUI::Exit()
{
	return TRUE;
}
