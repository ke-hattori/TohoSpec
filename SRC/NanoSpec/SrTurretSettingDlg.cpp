// SrTurretSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SrTurretSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// #########################################################################
// CSrTurretSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSrTurretSettingDlg::CSrTurretSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrTurretSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrTurretSettingDlg)
	//}}AFX_DATA_INIT
	m_iCurrNo = 0;
	m_strName = "";
}

// =========================================================================
//
void CSrTurretSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrTurretSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, RECIPE_NAME_LEN);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrTurretSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrTurretSettingDlg)
	ON_CBN_SELCHANGE(IDC_NUM, OnSelchangeNo)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, OnCheckEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSrTurretSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSrTurretSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	int i;
	TCHAR szTemp[256];
	CComboBox* pCombo;

	///// ComboBox Initialize /////
	pCombo = (CComboBox *)GetDlgItem(IDC_NUM);
	pCombo->ResetContent();
	for(i = 0; i < SR_LENS_MAX; i++){
		_stprintf(szTemp, "%d", i + 1);
		pCombo->AddString(szTemp);
	}
	pCombo->SetCurSel(0);
	m_iCurrNo = 0;

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_STATIC_TURRET_NO)->ShowWindow(SW_HIDE);
	}
	else{
		GetDlgItem(IDC_STATIC_TURRET_NO)->ShowWindow(SW_SHOW);
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

// 2009.09.18 bagus SE --{--
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//	pCombo = (CComboBox *)GetDlgItem(IDC_AUTOFOCUS_TURRET_NO);
//	pCombo->ResetContent();
//	for(i = 0; i < SR_LENS_MAX; i++){
//		_stprintf(szTemp, "%d", i + 1);
//		pCombo->AddString(szTemp);
//	}
//	pCombo->SetCurSel(0);
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_AUTOFOCUS_TURRET_NO)->ShowWindow(SW_HIDE);
	}
	else{
		pCombo = (CComboBox *)GetDlgItem(IDC_AUTOFOCUS_TURRET_NO);
		pCombo->ResetContent();
		for(i = 0; i < SR_LENS_MAX; i++){
			_stprintf(szTemp, "%d", i + 1);
			pCombo->AddString(szTemp);
		}
		pCombo->SetCurSel(0);
	}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
// 2009.09.18 bagus SE --}--

	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	UpDate(FALSE);
	UpdateXmpAngle();
	OnCheckEnable();

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}
// =========================================================================
//
void CSrTurretSettingDlg::OnOK()
{
	SR_LENS_CENTER_OFFSET LensOffset;
	TCHAR szBuff[256];

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	///// Save /////
	UpDate(TRUE);

	if(!CheckData()) return;

// 2009.09.19 bagus SE --{--
	int		i;
	BOOL	bCheck = FALSE;
	CString strBuffer;

	for (i=0; i<SR_LENS_MAX; i++) {
		if (m_SrTurret[i].bEnable && m_SrTurret[i].bAnalysis) {
			bCheck = TRUE;
			break;
		}
	}
	if (!bCheck) {
		LoadStringML(IDS_CHECK_VALID_MEAS_LENS, strBuffer, "Valid as a measurement lens is necessary by one.");
		AfxMessageBox(strBuffer);
		return;
	}
// 2009.09.19 bagus SE --}--

	///// Lens Center Offset SettingでReferenceとして使用しているLensを無効にしたとき /////
	ConfigFile_GetNanoSpecIni(&LensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
	if ( !m_SrTurret[LensOffset.iBaseLens].bEnable ) {
		int iLens = LensOffset.iBaseLens + 1;
		// Kojika 20090529 Change
		//_stprintf(szBuff, "Since Turret No.%d is Specified as Base Lens, It Cannot Save.\nConfirm Turret No.%d or Change Base Lens in LENS CENTER OFFSET.", iLens, iLens);
		LoadStringML(IDS_BASE_LENS_CANNOT_SAVE, l_strBuffer, "Since Turret No.%d is Specified as Base Lens, It Cannot Save.\nConfirm Turret No.%d or Change Base Lens in LENS CENTER OFFSET.");
		_stprintf(szBuff, l_strBuffer, iLens, iLens);
		//if ( MessageBox(szBuff, "Turret Setting",MB_OK | MB_ICONQUESTION) != IDOK )
		LoadStringML(IDS_TITLE_TURRET_SETTING, l_strTitle, "Turret Setting");
		if ( MessageBox(szBuff, l_strTitle, MB_OK | MB_ICONQUESTION) != IDOK ){
		// Kojika 20090529 Change End
			return;
		}
	}

// 2009.09.19 bagus SE --{--
	SR_CONFIG	SrConfig;
	int			nIndex;

	memset(&SrConfig, 0, sizeof(SrConfig));
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	nIndex = SrConfig.iTrUseLensNo - 1;
	if (nIndex < 0) {
		// OPEN
	} else if (nIndex >= SR_LENS_MAX) {
		// そもそもターレット数が異常
	} else {
		// 未使用ターレットを選択しているなら設定変更
		if (!m_SrTurret[nIndex].bEnable || !m_SrTurret[nIndex].bAnalysis) {
			for (i=0; i<SR_LENS_MAX; i++) {
				if (m_SrTurret[i].bEnable && m_SrTurret[i].bAnalysis) {
					SrConfig.iTrUseLensNo = i + 1;
					ConfigFile_SetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);
					ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);
					break;
				}
			}
		}
	}
// 2009.09.19 bagus SE --}--

	ConfigFile_SetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);
	CDialog::OnOK();
}

// =========================================================================
//
void CSrTurretSettingDlg::OnSelchangeNo()
{
	CComboBox* pCombo;
	pCombo = (CComboBox *)GetDlgItem(IDC_NUM);
	// 現在の設定を保存
	UpDate(TRUE);
	if(!CheckData()){
		pCombo->SetCurSel(m_iCurrNo);
		return;
	}

	// 新しい番号のターレットの設定を表示
	m_iCurrNo = pCombo->GetCurSel();
	UpDate(FALSE);
	UpdateXmpAngle();
	OnCheckEnable();
}

// =========================================================================
//
void CSrTurretSettingDlg::OnCheckEnable()
{
	BOOL bEnable = IsDlgButtonChecked(IDC_CHECK_ENABLE) == BST_CHECKED;

	GetDlgItem(IDC_NAME)->EnableWindow(bEnable);
	GetDlgItem(IDC_XMP_ANGLE)->EnableWindow(bEnable);

// 2009.09.18 bagus SE --{--
	GetDlgItem(IDC_CHECK_VALID_AS_A_MEASUREMENT_LENS)->EnableWindow(bEnable);
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//	GetDlgItem(IDC_AUTOFOCUS_TURRET_NO)->EnableWindow(bEnable);
	if(g_lModelType != MODEL_T3100){
		GetDlgItem(IDC_AUTOFOCUS_TURRET_NO)->EnableWindow(bEnable);
	}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
// 2009.09.18 bagus SE --}--
}

// =========================================================================
//
void CSrTurretSettingDlg::GridDisp(CGridCtrl* pGrid, int row, int col, char* text, DWORD TextStyle)
{
	GV_ITEM gvItem;
	DWORD dwTextStyle;

	gvItem.mask = GVIF_TEXT | GVIF_FORMAT;
	dwTextStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	//dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	dwTextStyle &= ~DT_LEFT;
	dwTextStyle |= TextStyle;

	gvItem.nFormat = dwTextStyle;
	gvItem.mask = GVIF_TEXT|GVIF_FORMAT;
	gvItem.row = row;
	gvItem.col = col;
	gvItem.nFormat = dwTextStyle;
	gvItem.strText.Format("%s", text);
	pGrid->SetItem(&gvItem);

	pGrid->Invalidate();
}

// =========================================================================
//
void CSrTurretSettingDlg::UpDate(BOOL bValid)
{
	CString strBuffer;
	GV_ITEM gvItem;

// 2009.09.18 bagus SE --{--
	CComboBox* pCombo;
	pCombo = (CComboBox *)GetDlgItem(IDC_AUTOFOCUS_TURRET_NO);
// 2009.09.18 bagus SE --}--

	if ( bValid ) {
		///// Enable /////
		m_SrTurret[m_iCurrNo].bEnable = IsDlgButtonChecked(IDC_CHECK_ENABLE) == BST_CHECKED;

		///// Name /////
		GetDlgItemText(IDC_NAME, strBuffer);
		_stprintf(m_SrTurret[m_iCurrNo].szName, "%s", (PCSTR) strBuffer);

		///// XMP Angle /////
		GetDlgItemText(IDC_XMP_ANGLE, strBuffer);
		m_SrTurret[m_iCurrNo].dXmpAngle = atof((PCSTR) strBuffer);

// 2009.09.18 bagus SE --{--
// 2009.09.29 bagus SE --{--
#if 0
		///// Valid as a Measurement Lens /////
		m_SrTurret[m_iCurrNo].bAnalysis = IsDlgButtonChecked(IDC_CHECK_VALID_AS_A_MEASUREMENT_LENS) == BST_CHECKED;

		///// Auto Focus Turret No. /////
		if (m_SrTurret[m_iCurrNo].bEnable) {
			m_SrTurret[m_iCurrNo].iAFLens = pCombo->GetCurSel() + 1;
		} else {
			m_SrTurret[m_iCurrNo].iAFLens = m_iCurrNo + 1;
		}
#else
		///// Valid as a Measurement Lens /////
		m_SrTurret[m_iCurrNo].bAnalysis = IsDlgButtonChecked(IDC_CHECK_VALID_AS_A_MEASUREMENT_LENS) == BST_CHECKED;

		///// Auto Focus Turret No. /////
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		if (m_SrTurret[m_iCurrNo].bEnable) {
//			m_SrTurret[m_iCurrNo].iAFLens = pCombo->GetCurSel();
//		} else {
//			m_SrTurret[m_iCurrNo].iAFLens = m_iCurrNo;
//		}
		if(g_lModelType == MODEL_T3100){
			m_SrTurret[m_iCurrNo].iAFLens = 0;
		}
		else{
			if (m_SrTurret[m_iCurrNo].bEnable) {
				m_SrTurret[m_iCurrNo].iAFLens = pCombo->GetCurSel();
			} else {
				m_SrTurret[m_iCurrNo].iAFLens = m_iCurrNo;
			}
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
#endif
// 2009.09.29 bagus SE --}--
// 2009.09.18 bagus SE --}--
	}
	else {
		///// Enable /////
		UINT uCheck = m_SrTurret[m_iCurrNo].bEnable ? BST_CHECKED : BST_UNCHECKED;
		CheckDlgButton(IDC_CHECK_ENABLE, uCheck);

		///// Name /////
		strBuffer =	m_SrTurret[m_iCurrNo].szName;
		SetDlgItemText(IDC_NAME, strBuffer);

		///// XMP Angle /////
		strBuffer.Format("%.3lf", m_SrTurret[m_iCurrNo].dXmpAngle);
		SetDlgItemText(IDC_XMP_ANGLE, strBuffer);

// 2009.09.18 bagus SE --{--
		///// Valid as a Measurement Lens /////
		UINT uCheck2 = m_SrTurret[m_iCurrNo].bAnalysis ? BST_CHECKED : BST_UNCHECKED;
		CheckDlgButton(IDC_CHECK_VALID_AS_A_MEASUREMENT_LENS, uCheck2);

// 2009.09.29 bagus SE --{--
#if 0
		///// Auto Focus Turret No. /////
		if ((m_SrTurret[m_iCurrNo].iAFLens < 1) || (m_SrTurret[m_iCurrNo].iAFLens > SR_LENS_MAX)) {
			//NG
			pCombo->SetCurSel(m_iCurrNo);
		} else {
			pCombo->SetCurSel(m_SrTurret[m_iCurrNo].iAFLens - 1);
		}
#else
		///// Auto Focus Turret No. /////
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		if ((m_SrTurret[m_iCurrNo].iAFLens < 0) || (m_SrTurret[m_iCurrNo].iAFLens >= SR_LENS_MAX)) {
//			//NG
//			pCombo->SetCurSel(m_iCurrNo);
//		} else {
//			pCombo->SetCurSel(m_SrTurret[m_iCurrNo].iAFLens);
//		}
		if(g_lModelType != MODEL_T3100){
			if ((m_SrTurret[m_iCurrNo].iAFLens < 0) || (m_SrTurret[m_iCurrNo].iAFLens >= SR_LENS_MAX)) {
				//NG
				pCombo->SetCurSel(m_iCurrNo);
			} else {
				pCombo->SetCurSel(m_SrTurret[m_iCurrNo].iAFLens);
			}
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
#endif
// 2009.09.29 bagus SE --}--
// 2009.09.18 bagus SE --}--
	}
}
// =========================================================================
//
BOOL CSrTurretSettingDlg::CheckData()
{
	CString strBuffer;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

// 2009.09.18 bagus SE --{--
	CComboBox* pCombo;
	pCombo = (CComboBox *)GetDlgItem(IDC_AUTOFOCUS_TURRET_NO);
// 2009.09.18 bagus SE --}--

	if ( !m_SrTurret[m_iCurrNo].bEnable ) {
		return TRUE;
	}

	// Name
	if ( _tcslen(m_SrTurret[m_iCurrNo].szName) == 0 ) {
		// Kojika 20090529 Change
		//MessageBox("Please enter a name", "Turret Setting", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_ENTER_NAME, l_strBuffer, "Please enter a name");
		LoadStringML(IDS_TITLE_TURRET_SETTING, l_strTitle, "Turret Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_NAME)->SetFocus();
		return FALSE;
	}

	// XMP Angle
	GetDlgItemText(IDC_XMP_ANGLE, strBuffer);
	if ( CheckMinMaxDouble(strBuffer) != CHECK_DATA_OK ) {
		GetDlgItem(IDC_XMP_ANGLE)->SetFocus();
		return FALSE;
	}

// 2009.09.18 bagus SE --{--
	// Auto Focus Turret No.
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//	if (pCombo->GetCurSel() < 0) {
//		pCombo->SetFocus();
//		LoadStringML(IDS_SELECT_AUTO_FOCUS_TURRET_NO, strBuffer, "Please select auto focus turret no.");
//		AfxMessageBox(strBuffer);
//		return FALSE;
//	}
	if(g_lModelType != MODEL_T3100){
		if (pCombo->GetCurSel() < 0) {
			pCombo->SetFocus();
			LoadStringML(IDS_SELECT_AUTO_FOCUS_TURRET_NO, strBuffer, "Please select auto focus turret no.");
			AfxMessageBox(strBuffer);
			return FALSE;
		}
	}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
// 2009.09.18 bagus SE --}--

	return TRUE;
}
// =========================================================================
//
void CSrTurretSettingDlg::UpdateXmpAngle()
{
	TCHAR szBuff[256];

	// XMP Angle
	_stprintf(szBuff, "%.3lf", m_SrTurret[m_iCurrNo].dXmpAngle);
	SetDlgItemText(IDC_XMP_ANGLE, szBuff);

}
