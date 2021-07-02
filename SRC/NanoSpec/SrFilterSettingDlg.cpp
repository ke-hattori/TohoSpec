// SrFilterSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SrFilterSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// フィルタを使用するかどうかのチェックボックスのID
int CheckFilterEnable_IDs[] = {
	IDC_CHECK_FILTER1_ENABLE,
	IDC_CHECK_FILTER2_ENABLE,
	IDC_CHECK_FILTER3_ENABLE,
};

// フィルタの名前を入力するエディットボックスのID
int EditFilterName_IDs[] = {
	IDC_FILTER1_NAME,
	IDC_FILTER2_NAME,
	IDC_FILTER3_NAME,
};

// フィルタの波長範囲の開始波長を入力するエディットボックスのID
int EditFilterWavelengthStart_IDs[] = {
	IDC_FILTER1_WAVELENGTH_START,
	IDC_FILTER2_WAVELENGTH_START,
	IDC_FILTER3_WAVELENGTH_START,
};

// フィルタの波長範囲の終了波長を入力するエディットボックスのID
int EditFilterWavelengthEnd_IDs[] = {
	IDC_FILTER1_WAVELENGTH_END,
	IDC_FILTER2_WAVELENGTH_END,
	IDC_FILTER3_WAVELENGTH_END,
};

// #########################################################################
// CSrFilterSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSrFilterSettingDlg::CSrFilterSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrFilterSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrFilterSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSrFilterSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrFilterSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrFilterSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrFilterSettingDlg)
	ON_BN_CLICKED(IDC_CHECK_FILTER1_ENABLE, OnCheckFilterEnable)
	ON_BN_CLICKED(IDC_CHECK_FILTER2_ENABLE, OnCheckFilterEnable)
	ON_BN_CLICKED(IDC_CHECK_FILTER3_ENABLE, OnCheckFilterEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSrFilterSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSrFilterSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	memset(&m_SrFilter, 0, sizeof(m_SrFilter));
	ConfigFile_GetNanoSpecIni(m_SrFilter, CONFIG_FILE_SR_FILTER);

	///// Data Set /////
	UpDate(FALSE);

	OnCheckFilterEnable();

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSrFilterSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);

	if(!CheckData()) return;

// 2009.09.19 bagus SE --{--
	SR_CONFIG	SrConfig;
	int			nIndex;

	memset(&SrConfig, 0, sizeof(SrConfig));
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	nIndex = SrConfig.wDefaultMeasFilter - 1;
	if (nIndex < 0) {
		// OPEN
	} else if (nIndex >= SR_FILTER_MAX) {
		// そもそもフィルタ数が異常
	} else {
		// 未使用フィルタを選択しているなら設定変更
		if (!m_SrFilter[nIndex].bEnable) {
			SrConfig.wDefaultMeasFilter = 1;
			ConfigFile_SetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);
			ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);
		}
	}
// 2009.09.19 bagus SE --}--

	ConfigFile_SetNanoSpecIni(m_SrFilter, CONFIG_FILE_SR_FILTER);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);
	CDialog::OnOK();
}

// =========================================================================
//
void CSrFilterSettingDlg::OnCheckFilterEnable()
{
	BOOL bCheck;
	for ( int iFilter = 0; iFilter < SR_FILTER_MAX; iFilter++ ) {
		bCheck = IsDlgButtonChecked(CheckFilterEnable_IDs[iFilter]) == BST_CHECKED;
		GetDlgItem(EditFilterName_IDs[iFilter])->EnableWindow(bCheck);
		GetDlgItem(EditFilterWavelengthStart_IDs[iFilter])->EnableWindow(bCheck);
		GetDlgItem(EditFilterWavelengthEnd_IDs[iFilter])->EnableWindow(bCheck);
	}
}

// =========================================================================
//
void CSrFilterSettingDlg::UpDate(BOOL bValid)
{
	CString strBuffer;
	int		iFilter;

	if ( bValid ) {

		for ( iFilter = 0; iFilter < SR_FILTER_MAX; iFilter++ ) {
			///// Filter Enable /////
			m_SrFilter[iFilter].bEnable = IsDlgButtonChecked(CheckFilterEnable_IDs[iFilter]) == BST_CHECKED;

			///// Filter Name /////
			GetDlgItemText(EditFilterName_IDs[iFilter], strBuffer);
			_stprintf(m_SrFilter[iFilter].szName, "%s", (PCSTR) strBuffer);

			if ( m_SrFilter[iFilter].bEnable ) {
				///// Filter Wavelength Start /////
				GetDlgItemText(EditFilterWavelengthStart_IDs[iFilter], strBuffer);
				m_SrFilter[iFilter].Wavelength.wStart = (WORD) atoi((PCSTR) strBuffer);

				///// Filter Wavelength End /////
				GetDlgItemText(EditFilterWavelengthEnd_IDs[iFilter], strBuffer);
				m_SrFilter[iFilter].Wavelength.wEnd = (WORD) atoi((PCSTR) strBuffer);
			}
		}
	}
	else {
		for ( iFilter = 0; iFilter < SR_FILTER_MAX; iFilter++ ) {
			///// Filter Enable /////
			UINT uCheck = m_SrFilter[iFilter].bEnable ? BST_CHECKED : BST_UNCHECKED;
			CheckDlgButton(CheckFilterEnable_IDs[iFilter], uCheck);

			///// Filter Name /////
			strBuffer = m_SrFilter[iFilter].szName;
			SetDlgItemText(EditFilterName_IDs[iFilter], (PCSTR) strBuffer);

			///// Filter Wavelength Start /////
			strBuffer.Format("%d", m_SrFilter[iFilter].Wavelength.wStart);
			SetDlgItemText(EditFilterWavelengthStart_IDs[iFilter], (PCSTR) strBuffer);

			///// Filter Wavelength End /////
			strBuffer.Format("%d", m_SrFilter[iFilter].Wavelength.wEnd);
			SetDlgItemText(EditFilterWavelengthEnd_IDs[iFilter], (PCSTR) strBuffer);
		}
	}
}
// =========================================================================
//
BOOL CSrFilterSettingDlg::CheckData()
{
	CString strBufferStart;
	CString strBufferEnd;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	for ( int iFilter = 0; iFilter < SR_FILTER_MAX; iFilter++){
		if ( !m_SrFilter[iFilter].bEnable ) {
			continue;
		}

		// Name
		if ( _tcslen(m_SrFilter[iFilter].szName) <= 0 ) {
			// Kojika 20090529 Change
			//MessageBox("Please enter a name", "SR FILTER SETTING", MB_OK|MB_ICONSTOP);
			LoadStringML(IDS_ENTER_NAME, l_strBuffer, "Please enter a name");
			LoadStringML(IDS_TITLE_SR_FILTER_SETTING, l_strTitle, "SR FILTER SETTING");
			MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
			// Kojika 20090529 Change End
			GetDlgItem(EditFilterName_IDs[iFilter])->SetFocus();
			return FALSE;
		}

		// Start
		GetDlgItemText(EditFilterWavelengthStart_IDs[iFilter], strBufferStart);
		if ( CheckMinMaxInt(strBufferStart, MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH) != CHECK_DATA_OK ) {
			GetDlgItem(EditFilterWavelengthStart_IDs[iFilter])->SetFocus();
			return FALSE;
		}

		// End
		GetDlgItemText(EditFilterWavelengthEnd_IDs[iFilter], strBufferEnd);
		if ( CheckMinMaxInt(strBufferEnd, MIN_TARGET_WAVELENGTH, MAX_TARGET_WAVELENGTH) != CHECK_DATA_OK ) {
			GetDlgItem(EditFilterWavelengthEnd_IDs[iFilter])->SetFocus();
			return FALSE;
		}

		// Check Start < End
		if ( atoi(strBufferStart) >= atoi(strBufferEnd) ) {
			// Kojika 20090529 Change
			//strBufferEnd.Format("Please enter End Filter WaveLength bigger value than Start Filter WaveLength.");
			//MessageBox(strBufferEnd, "SR FILTER SETTING", MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_WAVELENGTH_END_BIGGER, l_strBuffer, "Please enter End Filter WaveLength bigger value than Start Filter WaveLength.");
			LoadStringML(IDS_TITLE_SR_FILTER_SETTING, l_strTitle, "SR FILTER SETTING");
			strBufferEnd.Format(l_strBuffer);
			MessageBox(strBufferEnd, l_strTitle, MB_OK | MB_ICONSTOP);
			// Kojika 20090529 Change End
			GetDlgItem(EditFilterWavelengthEnd_IDs[iFilter])->SetFocus();
			return FALSE;
		}

	}
	return TRUE;
}
