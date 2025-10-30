// SrViewRangeSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SrViewRangeSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSrViewRangeSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSrViewRangeSettingDlg::CSrViewRangeSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrViewRangeSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrViewRangeSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSrViewRangeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrViewRangeSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_LEFT, m_SrViewRange.lLeft);
	DDX_Text(pDX, IDC_TOP, m_SrViewRange.lTop);
	DDX_Text(pDX, IDC_WIDTH, m_SrViewRange.lWidth);
	DDX_Text(pDX, IDC_HEIGHT, m_SrViewRange.lHeight);
	DDV_MinMaxLong(pDX, m_SrViewRange.lLeft, MIN_VIEW_RANGE, MAX_VIEW_RANGE);
	DDV_MinMaxLong(pDX, m_SrViewRange.lTop, MIN_VIEW_RANGE, MAX_VIEW_RANGE);
	DDV_MinMaxLong(pDX, m_SrViewRange.lWidth, MIN_VIEW_RANGE, MAX_VIEW_RANGE);
	DDV_MinMaxLong(pDX, m_SrViewRange.lHeight, MIN_VIEW_RANGE, MAX_VIEW_RANGE);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrViewRangeSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrViewRangeSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSrViewRangeSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSrViewRangeSettingDlg::OnInitDialog()
{
	////// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SrViewRange, CONFIG_FILE_SR_VIEW_RANGE);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSrViewRangeSettingDlg::OnOK()
{
	///// Save /////
	if ( !UpdateData() )
		return;

	ConfigFile_SetNanoSpecIni(&m_SrViewRange, CONFIG_FILE_SR_VIEW_RANGE);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);

	CDialog::OnOK();
}
