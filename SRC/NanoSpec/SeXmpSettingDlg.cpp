// SeXmpSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SeXmpSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSeXmpSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSeXmpSettingDlg::CSeXmpSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeXmpSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeXmpSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSeXmpSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeXmpSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STRATEGY_LIST_PATH, m_SrXmp.szStrategyListPath, 255 + 1);
	DDX_Text(pDX, IDC_DATA_PATH, m_SrXmp.szDataPath, 255 + 1);
	DDX_Text(pDX, IDC_FITTING_DATA_PATH, m_SrXmp.szFittingDataPath, 255 + 1);
	DDX_Text(pDX, IDC_WVASE_EXE_PATH, m_SrXmp.szAdapExePath, 255 + 1);
	DDX_Text(pDX, IDC_WVASE_WINDOW_TITLE, m_SrXmp.szWvaseWindowTitle, 255 + 1);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_STRATEGY_LIST_PATH_BUTTON, m_StrategyListPathButton);
	DDX_Control(pDX, IDC_DATA_PATH_BUTTON, m_DataPathButton);
	DDX_Control(pDX, IDC_FITTING_DATA_PATH_BUTTON, m_FittingDataPathButton);
	DDX_Control(pDX, IDC_WVASE_EXE_PATH_BUTTON, m_WvaseExePathButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSeXmpSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSeXmpSettingDlg)
	ON_BN_CLICKED(IDC_STRATEGY_LIST_PATH_BUTTON, OnStrategyListPathButton)
	ON_BN_CLICKED(IDC_DATA_PATH_BUTTON, OnDataPathButton)
	ON_BN_CLICKED(IDC_FITTING_DATA_PATH_BUTTON, OnFittingDataPathButton)
	ON_BN_CLICKED(IDC_WVASE_EXE_PATH_BUTTON, OnWvaseExePathButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSeXmpSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSeXmpSettingDlg::OnInitDialog()
{
	////// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SrXmp, CONFIG_FILE_SE_XMP);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Strategy List Paht Button /////
	m_StrategyListPathButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_StrategyListPathButton.DrawFlatFocus(TRUE);

	///// Data Path Button /////
	m_DataPathButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DataPathButton.DrawFlatFocus(TRUE);

	///// Fitting Data Path Button /////
	m_FittingDataPathButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_FittingDataPathButton.DrawFlatFocus(TRUE);

	///// WVASE Exe Path Button /////
	m_WvaseExePathButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_WvaseExePathButton.DrawFlatFocus(TRUE);

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_STRATEGY_LIST_PATH))->SetLimitText(255);
	((CEdit*)GetDlgItem(IDC_DATA_PATH))->SetLimitText(255);
	((CEdit*)GetDlgItem(IDC_FITTING_DATA_PATH))->SetLimitText(255);
	((CEdit*)GetDlgItem(IDC_WVASE_EXE_PATH))->SetLimitText(255);
	((CEdit*)GetDlgItem(IDC_WVASE_WINDOW_TITLE))->SetLimitText(255);

	// ADAP使用時　：ADAP経由でWVASE32を利用するケース（ADAP＋WVASE32）の対処
	// Word資料『NanoSpec本体のXMP SETTING画面について（20071130）.doc』参照
	GetDlgItem(IDC_WVASE_EXE_PATH)->EnableWindow(FALSE);
	GetDlgItem(IDC_WVASE_EXE_PATH_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_WVASE_WINDOW_TITLE)->EnableWindow(FALSE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSeXmpSettingDlg::OnOK()
{
	///// Save /////
	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_SrXmp, CONFIG_FILE_SE_XMP);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SE_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CSeXmpSettingDlg::OnStrategyListPathButton()
{
	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	CFileDialog dlg(TRUE, "lst",	NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
		"Strategy List Path (*.lst)|*.lst||");
	dlg.m_ofn.lpstrTitle = "Strategy List Path";

	if ( dlg.DoModal() == IDOK ) {
		if ( dlg.GetPathName().GetLength() > 255 ) {
			// Kojika 20090529 Change
			//MessageBox("Path is Too Long", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
			LoadStringML(IDS_PATH_TOO_LONG, l_strBuffer, "Path is Too Long");
			LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
			MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
			// Kojika 20090529 Change End
			return;
		}
		strcpy(m_SrXmp.szStrategyListPath, dlg.GetPathName());
		UpdateData(FALSE);
	}
}

// =========================================================================
//
void CSeXmpSettingDlg::OnDataPathButton()
{
	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	CFileDialog dlg(TRUE, "dat",	NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
		"Data Path (*.dat)|*.dat||");
	dlg.m_ofn.lpstrTitle = "Data Path";

	if ( dlg.DoModal() == IDOK ) {
		if ( dlg.GetPathName().GetLength() > 255 ) {
			// Kojika 20090529 Change
			//MessageBox("Path is Too Long", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
			LoadStringML(IDS_PATH_TOO_LONG, l_strBuffer, "Path is Too Long");
			LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
			MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
			// Kojika 20090529 Change End
			return;
		}
		strcpy(m_SrXmp.szDataPath, dlg.GetPathName());
		UpdateData(FALSE);
	}
}

// =========================================================================
//
void CSeXmpSettingDlg::OnFittingDataPathButton()
{
	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	CFileDialog dlg(TRUE, "dat",	NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
		"Fitting Data Path (*.dat)|*.dat||");
	dlg.m_ofn.lpstrTitle = "Fitting Data Path";

	if ( dlg.DoModal() == IDOK ) {
		if ( dlg.GetPathName().GetLength() > 255 ) {
			// Kojika 20090529 Change
			//MessageBox("Path is Too Long", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
			LoadStringML(IDS_PATH_TOO_LONG, l_strBuffer, "Path is Too Long");
			LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
			MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
			// Kojika 20090529 Change End
			return;
		}
		strcpy(m_SrXmp.szFittingDataPath, dlg.GetPathName());
		UpdateData(FALSE);
	}
}

// =========================================================================
//
void CSeXmpSettingDlg::OnWvaseExePathButton()
{
	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	CFileDialog dlg(TRUE, "exe",	NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
		"WVASE Exe Path (*.exe)|*.exe||");
	dlg.m_ofn.lpstrTitle = "WVASE Exe Path";

	if ( dlg.DoModal() == IDOK ) {
		if ( dlg.GetPathName().GetLength() > 255 ) {
			// Kojika 20090529 Change
			//MessageBox("Path is Too Long", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
			LoadStringML(IDS_PATH_TOO_LONG, l_strBuffer, "Path is Too Long");
			LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
			MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
			// Kojika 20090529 Change End
			return;
		}
		strcpy(m_SrXmp.szAdapExePath, dlg.GetPathName());
		UpdateData(FALSE);
	}
}

// =========================================================================
//
BOOL CSeXmpSettingDlg::CheckData()
{
	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	UpdateData(TRUE);

	///// Strategy List Path /////
	if ( strlen(m_SrXmp.szStrategyListPath) == 0 ) {
		// Kojika 20090529 Change
		//MessageBox("Please enter a Strategy List Path", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_ENTER_STRATEGY_PATH, l_strBuffer, "Please enter a Strategy List Path");
		LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_STRATEGY_LIST_PATH)->SetFocus();
		return FALSE;
	}

	///// Data Path /////
	if ( strlen(m_SrXmp.szDataPath) == 0 ) {
		// Kojika 20090529 Change
		//MessageBox("Please enter a Data Path", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_ENTER_DATA_PATH, l_strBuffer, "Please enter a Data Path");
		LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_DATA_PATH)->SetFocus();
		return FALSE;
	}

	///// Fitting Data Path /////
	if ( strlen(m_SrXmp.szFittingDataPath) == 0 ) {
		// Kojika 20090529 Change
		//MessageBox("Please enter a Fitting Data Path", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_ENTER_FITTING_DATA_PATH, l_strBuffer, "Please enter a Fitting Data Path");
		LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_FITTING_DATA_PATH)->SetFocus();
		return FALSE;
	}

	///// ADAP Exe Path /////
	if ( strlen(m_SrXmp.szAdapExePath) == 0 ) {
		// Kojika 20090529 Change
		//MessageBox("Please enter a WVASE Exe Path", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_ENTER_WVASE_EXE_PATH, l_strBuffer, "Please enter a WVASE Exe Path");
		LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_WVASE_EXE_PATH)->SetFocus();
		return FALSE;
	}

	///// WVASE Window Title /////
	if ( strlen(m_SrXmp.szWvaseWindowTitle) == 0 ) {
		// Kojika 20090529 Change
		//MessageBox("Please enter a WVASE Window Title", "SE XMP SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_ENTER_WVASE_WINDOW_TITLE, l_strBuffer, "Please enter a WVASE Window Title");
		LoadStringML(IDS_TITLE_SR_XMP_SETTING, l_strTitle, "SE XMP SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_WVASE_WINDOW_TITLE)->SetFocus();
		return FALSE;
	}
	return TRUE;
}
