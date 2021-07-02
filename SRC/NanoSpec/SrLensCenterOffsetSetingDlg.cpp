// SrLensCenterOffsetSetingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "ManualStageDlg.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "SrLensCenterOffsetSetingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// LensのXのオフセット値を入力するテキストボックスのID
int LensOffsetX_IDs[] = {
	IDC_LENS1_X,
	IDC_LENS2_X,
	IDC_LENS3_X,
	IDC_LENS4_X,
};

// LensのYのオフセット値を入力するテキストボックスのID
int LensOffsetY_IDs[] = {
	IDC_LENS1_Y,
	IDC_LENS2_Y,
	IDC_LENS3_Y,
	IDC_LENS4_Y,
};

// LensのZのオフセット値を入力するテキストボックスのID
int LensOffsetZ_IDs[] = {
	IDC_LENS1_Z,
	IDC_LENS2_Z,
	IDC_LENS3_Z,
	IDC_LENS4_Z,
};

// Lensの名前を入力するテキストボックスのID
int LensName_IDs[] = {
	IDC_LENS1_NAME,
	IDC_LENS2_NAME,
	IDC_LENS3_NAME,
	IDC_LENS4_NAME,
};

// LensのSetupボタンのID
int LensSetupButton_IDs[] = {
	IDC_LENS1_SETUP_BUTTON,
	IDC_LENS2_SETUP_BUTTON,
	IDC_LENS3_SETUP_BUTTON,
	IDC_LENS4_SETUP_BUTTON,
};

// ==========================================================================
// Define																   //
//																		   //
// ==========================================================================
//#define LENS_CENTER_OFFSET_SETTING_CAPTION			"LENS CENTER OFFSET SETTING"
//#define LENS_CENTER_OFFSET_SETTING_MESSAGE			"LENS CENTER OFFSET SETTING"

// #########################################################################
// CSrLensCenterOffsetSetingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSrLensCenterOffsetSetingDlg::CSrLensCenterOffsetSetingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrLensCenterOffsetSetingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrLensCenterOffsetSetingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrLensCenterOffsetSetingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LENS1_X, m_stcLens1_X);
	DDX_Control(pDX, IDC_LENS1_Y, m_stcLens1_Y);
	DDX_Control(pDX, IDC_LENS1_Z, m_stcLens1_Z);
	DDX_Control(pDX, IDC_LENS2_X, m_stcLens2_X);
	DDX_Control(pDX, IDC_LENS2_Y, m_stcLens2_Y);
	DDX_Control(pDX, IDC_LENS2_Z, m_stcLens2_Z);
	DDX_Control(pDX, IDC_LENS3_X, m_stcLens3_X);
	DDX_Control(pDX, IDC_LENS3_Y, m_stcLens3_Y);
	DDX_Control(pDX, IDC_LENS3_Z, m_stcLens3_Z);
	DDX_Control(pDX, IDC_LENS4_X, m_stcLens4_X);
	DDX_Control(pDX, IDC_LENS4_Y, m_stcLens4_Y);
	DDX_Control(pDX, IDC_LENS4_Z, m_stcLens4_Z);
	DDX_Control(pDX, IDC_LENS1_SETUP_BUTTON, m_Lens1SetupButton);
	DDX_Control(pDX, IDC_LENS2_SETUP_BUTTON, m_Lens2SetupButton);
	DDX_Control(pDX, IDC_LENS3_SETUP_BUTTON, m_Lens3SetupButton);
	DDX_Control(pDX, IDC_LENS4_SETUP_BUTTON, m_Lens4SetupButton);
	DDX_Control(pDX, IDC_ALL_CLEAR, m_AllClearButton);
	DDX_Control(pDX, IDC_CHECK_BUTTON, m_CheckButton);
	DDX_Control(pDX, IDC_APPLY_BUTTON, m_ApplyButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_BASEPOS_X, m_basePos_X);
	DDX_Control(pDX, IDC_BASEPOS_Y, m_basePos_Y);
	DDX_Control(pDX, IDC_BASEPOS_Z, m_basePos_Z);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrLensCenterOffsetSetingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrLensCenterOffsetSetingDlg)
	ON_BN_CLICKED(IDC_ALL_CLEAR, OnAllClear)
	ON_BN_CLICKED(IDC_CHECK_BUTTON, OnCheck)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, OnApply)
	ON_CBN_SELCHANGE(IDC_BASE_LENS, OnSelchangeBaseLens)
	ON_BN_CLICKED(IDC_LENS1_SETUP_BUTTON, OnLens1SetupButton)
	ON_BN_CLICKED(IDC_LENS2_SETUP_BUTTON, OnLens2SetupButton)
	ON_BN_CLICKED(IDC_LENS3_SETUP_BUTTON, OnLens3SetupButton)
	ON_BN_CLICKED(IDC_LENS4_SETUP_BUTTON, OnLens4SetupButton)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSrLensCenterOffsetSetingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSrLensCenterOffsetSetingDlg::OnInitDialog()
{
	ConfigFile_GetNanoSpecIni(&m_LensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);

	m_LensOffsetBackUp = m_LensOffset;

	// 現在のレンズOffset情報消去
	SR_LENS_CENTER_OFFSET lensOffset;
	::ZeroMemory(&lensOffset, sizeof(lensOffset));
	ConfigFile_SetNanoSpecIni(&lensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
//	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);

	CDialog::OnInitDialog();

	CComboBox* pCombo;
	int i;
	char szBuff[256];
	CString csBuff;
	BOOL bLens = FALSE;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Static Control /////
	m_basePos_X.SetBkColor(REFERENCE_COLOR);
	m_basePos_Y.SetBkColor(REFERENCE_COLOR);
	m_basePos_Z.SetBkColor(REFERENCE_COLOR);
	m_stcLens1_X.SetBkColor(REFERENCE_COLOR);
	m_stcLens1_Y.SetBkColor(REFERENCE_COLOR);
	m_stcLens1_Z.SetBkColor(REFERENCE_COLOR);
	m_stcLens2_X.SetBkColor(REFERENCE_COLOR);
	m_stcLens2_Y.SetBkColor(REFERENCE_COLOR);
	m_stcLens2_Z.SetBkColor(REFERENCE_COLOR);
	m_stcLens3_X.SetBkColor(REFERENCE_COLOR);
	m_stcLens3_Y.SetBkColor(REFERENCE_COLOR);
	m_stcLens3_Z.SetBkColor(REFERENCE_COLOR);
	m_stcLens4_X.SetBkColor(REFERENCE_COLOR);
	m_stcLens4_Y.SetBkColor(REFERENCE_COLOR);
	m_stcLens4_Z.SetBkColor(REFERENCE_COLOR);

	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);
	m_AllClearButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AllClearButton.DrawFlatFocus(TRUE);
	m_CheckButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CheckButton.DrawFlatFocus(TRUE);
	m_ApplyButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ApplyButton.DrawFlatFocus(TRUE);
	m_Lens1SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Lens1SetupButton.DrawFlatFocus(TRUE);
	m_Lens2SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Lens2SetupButton.DrawFlatFocus(TRUE);
	m_Lens3SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Lens3SetupButton.DrawFlatFocus(TRUE);
	m_Lens4SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Lens4SetupButton.DrawFlatFocus(TRUE);


	///// Get Base Lens Number /////
	m_iBaseLens = m_LensOffset.iBaseLens;
	///// Base LensがUnusedの時 /////
	if(!m_SrTurret[m_iBaseLens].bEnable){
		// Kojika 20090529 Change
		//MessageBox("A Base Lens cannot be Used.\nPlease Make a Base Lens Usable or Newly Specify a Base Lens.", "Lens Center Offset Setting", MB_OK | MB_ICONSTOP);
		//SetDlgItemText(IDC_GUIDANCE, "A Base Lens cannot be Used. Please Make a Base Lens Usable or Newly Specify a Base Lens.");
		LoadStringML(IDS_BASE_LENS_UNUSED_CR, l_strBuffer, "A Base Lens cannot be Used.\nPlease Make a Base Lens Usable or Newly Specify a Base Lens.");
		LoadStringML(IDS_TITLE_LENS_OFFSET_SETTING, l_strTitle, "Lens Center Offset Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_BASE_LENS_UNUSED, l_strBuffer, "A Base Lens cannot be Used. Please Make a Base Lens Usable or Newly Specify a Base Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090529 Change End
	}

	// オフセット値の入力
	long BaseOffsetX = m_LensOffset.Lens[m_iBaseLens].lX;
	long BaseOffsetY = m_LensOffset.Lens[m_iBaseLens].lY;
	long BaseOffsetZ = m_LensOffset.Lens[m_iBaseLens].lZ;

	for(i = 0; i < SR_LENS_MAX; i++){
		///// ComboBox Initilize /////
		pCombo = (CComboBox *)GetDlgItem(IDC_BASE_LENS);
		if(m_SrTurret[i].bEnable){
			sprintf(szBuff, "%d", i+1);
			pCombo->AddString(szBuff);
		}

		// 名前の入力
		// Kojika 20090529 Change
		//SetDlgItemText(LensName_IDs[i], m_SrTurret[i].bEnable ? szBuff : "Unused");
		LoadStringML(IDS_UNUSED, l_strBuffer, "Unused");
		SetDlgItemText(LensName_IDs[i], m_SrTurret[i].bEnable ? szBuff : l_strBuffer);
		// Kojika 20090529 Change End

		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", m_LensOffset.Lens[i].lX - BaseOffsetX);
		SetDlgItemText(LensOffsetX_IDs[i], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", m_LensOffset.Lens[i].lY - BaseOffsetY);
		SetDlgItemText(LensOffsetY_IDs[i], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", m_LensOffset.Lens[i].lZ - BaseOffsetZ);
		SetDlgItemText(LensOffsetZ_IDs[i], szBuff);

		// Setupボタンの有効無効
		GetDlgItem(LensSetupButton_IDs[i])->EnableWindow(m_SrTurret[i].bEnable);

	}

	///// Control Initilize /////
	pCombo = (CComboBox *)GetDlgItem(IDC_BASE_LENS);
	for(i = 0; i < pCombo->GetCount(); i ++){
		pCombo->GetLBText(i, csBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%d", m_iBaseLens + 1);
		if(strcmp(szBuff, csBuff) == 0){
			pCombo->SetCurSel(i);
			bLens = TRUE;
		}
	}
	if(!bLens){
		pCombo->SetCurSel(0);
	}

	GetDlgItem(IDC_BASE_LENS)->EnableWindow(FALSE);
	GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_LENS1_SETUP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_LENS2_SETUP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_LENS3_SETUP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_LENS4_SETUP_BUTTON)->EnableWindow(FALSE);

// 2009.12.26 K.Matsuo ReEdit -->
	EnableButtons(FALSE);
	GetDlgItem(IDC_BASE_LENS)->EnableWindow(FALSE);
// 2009.12.26 K.Matsuo ReEdit <--

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnOK()
{
	int i;
	CString csBuff;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	SetOperationLog("OK Button was push.");

	///// Save /////
	m_LensOffset.iBaseLens = m_iBaseLens;

	if(!m_SrTurret[m_LensOffset.iBaseLens].bEnable){
		// Kojika 20090529 Change
		//MessageBox("Re-Set Up with an Effective Base Lens.", "Lens Center Offset Setting", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_RESET_BASE_LENS, l_strBuffer, "Re-Set Up with an Effective Base Lens.");
		LoadStringML(IDS_TITLE_LENS_OFFSET_SETTING, l_strTitle, "Lens Center Offset Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090529 Change End
		return;
	}

	for(i = 0; i < SR_LENS_MAX; i++){
		GetDlgItemText(LensOffsetX_IDs[i], csBuff);
		m_LensOffset.Lens[i].lX = atol(csBuff);
		GetDlgItemText(LensOffsetY_IDs[i], csBuff);
		m_LensOffset.Lens[i].lY = atol(csBuff);
		GetDlgItemText(LensOffsetZ_IDs[i], csBuff);
		m_LensOffset.Lens[i].lZ = atol(csBuff);
	}

	ConfigFile_SetNanoSpecIni(&m_LensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnCancel()
{
	ConfigFile_SetNanoSpecIni(&m_LensOffsetBackUp, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
//	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);

	CDialog::OnCancel();
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnAllClear()
{
	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	// Kojika 20090529 Change
	//if(MessageBox("Do You Want All Clear?", "Lens Center Offset", MB_OKCANCEL|MB_ICONQUESTION) == IDOK){
	LoadStringML(IDS_ALL_CLEAR, l_strBuffer, "Do You Want All Clear?");
	LoadStringML(IDS_TITLE_LENS_OFFSET, l_strTitle, "Lens Center Offset");
	if(MessageBox(l_strBuffer, l_strTitle, MB_OKCANCEL|MB_ICONQUESTION) == IDOK){
	// Kojika 20090529 Change End
		char szBuff[256];

		SetOperationLog("ALLCLEAR Button was push.");

		memset(szBuff, 0, sizeof(szBuff));
		//メモリの0クリアと表示の変更
		memset(&m_LensOffset, 0, sizeof(SR_LENS_CENTER_OFFSET));

		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", 0);
		SetDlgItemText(IDC_BASEPOS_X, szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", 0);
		SetDlgItemText(IDC_BASEPOS_Y, szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", 0);
		SetDlgItemText(IDC_BASEPOS_Z, szBuff);

		for(int i = 0; i < SR_LENS_MAX; i++){
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld",	m_LensOffset.Lens[i].lX);
			SetDlgItemText(LensOffsetX_IDs[i], szBuff);

			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld",	m_LensOffset.Lens[i].lY);
			SetDlgItemText(LensOffsetY_IDs[i], szBuff);

			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld",	m_LensOffset.Lens[i].lZ);
			SetDlgItemText(LensOffsetZ_IDs[i], szBuff);
		}
		// Kojika 20090529 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Specify the Position of a Base Lens.");
		LoadStringML(IDS_SPECIFY_POSITION_BASE_LENS, l_strBuffer, "Please Specify the Position of a Base Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090529 Change End

		EnableButtons(FALSE);
	}

}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnCheck()
{
	// Kojika 20090529 Add
	CString l_strCap, l_strMsg;
	// Kojika 20090529 Add End
	SR_LENS_CENTER_OFFSET lensOffset;
	CManualStageDlg dlg;

	SetOperationLog("CHECK Button was push.");

	// Kojika 20090529 Change
	//dlg.SetCaption(LENS_CENTER_OFFSET_SETTING_CAPTION);
	//dlg.SetMessage(LENS_CENTER_OFFSET_SETTING_MESSAGE);
	LoadStringML(IDS_LENS_OFFSET_SETTING_CAPTION, l_strCap, "LENS CENTER OFFSET SETTING");
	LoadStringML(IDS_LENS_OFFSET_SETTING_MESSAGE, l_strMsg, "LENS CENTER OFFSET SETTING");
	dlg.SetCaption(l_strCap);
	dlg.SetMessage(l_strMsg);
	// Kojika 20090529 Change End

// INS 2009.07.06 by kawamura
	int		i;
	CString csBuff;

	// 画面の設定値を書き込む
	lensOffset.iBaseLens = m_iBaseLens;
	for(i = 0; i < SR_LENS_MAX; i++){
		GetDlgItemText(LensOffsetX_IDs[i], csBuff);
		lensOffset.Lens[i].lX = atol(csBuff);
		GetDlgItemText(LensOffsetY_IDs[i], csBuff);
		lensOffset.Lens[i].lY = atol(csBuff);
		GetDlgItemText(LensOffsetZ_IDs[i], csBuff);
		lensOffset.Lens[i].lZ = atol(csBuff);
	}
	ConfigFile_SetNanoSpecIni(&lensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
//	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);
// INS 2009.07.06 by kawamura

	dlg.DoModal();

	// カレントのレンズOffset消去
	::ZeroMemory(&lensOffset, sizeof(lensOffset));
	ConfigFile_SetNanoSpecIni(&lensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
//	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnApply()
{
	int i;
	CString csBuff;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	SetOperationLog("APPLY Button was push.");

	///// Save /////
	m_LensOffset.iBaseLens = m_iBaseLens;

	if(!m_SrTurret[m_LensOffset.iBaseLens].bEnable){
		// Kojika 20090529 Change
		//MessageBox("Re-Set Up with an Effective Base Lens.", "Lens Center Offset Setting", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_RESET_BASE_LENS, l_strBuffer, "Re-Set Up with an Effective Base Lens.");
		LoadStringML(IDS_TITLE_LENS_OFFSET_SETTING, l_strTitle, "Lens Center Offset Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090529 Change End
		return;
	}

	for(i = 0; i < SR_LENS_MAX; i++){
		GetDlgItemText(LensOffsetX_IDs[i], csBuff);
		m_LensOffset.Lens[i].lX = atol(csBuff);
		GetDlgItemText(LensOffsetY_IDs[i], csBuff);
		m_LensOffset.Lens[i].lY = atol(csBuff);
		GetDlgItemText(LensOffsetZ_IDs[i], csBuff);
		m_LensOffset.Lens[i].lZ = atol(csBuff);
	}

	ConfigFile_SetNanoSpecIni(&m_LensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
	ConfigFile_SaveNanoSpecIni(SYSTEM_SR_INI_FILE);

	m_LensOffsetBackUp = m_LensOffset;

	GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(FALSE);
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnSelchangeBaseLens()
{
	CComboBox* pCombo;
	CString csBuff;
	char szBuff[256];
	int iLens;

	// Kojika 20090529 Add
	CString l_strBuffer;
	// Kojika 20090529 Add End

	memset(szBuff, 0, sizeof(szBuff));
	sprintf(szBuff, "%ld", 0);
	SetDlgItemText(IDC_BASEPOS_X, szBuff);
	memset(szBuff, 0, sizeof(szBuff));
	sprintf(szBuff, "%ld", 0);
	SetDlgItemText(IDC_BASEPOS_Y, szBuff);
	memset(szBuff, 0, sizeof(szBuff));
	sprintf(szBuff, "%ld", 0);
	SetDlgItemText(IDC_BASEPOS_Z, szBuff);

	pCombo = (CComboBox *)GetDlgItem(IDC_BASE_LENS);
	pCombo->GetLBText(pCombo->GetCurSel(), csBuff);

	for(int i = 0; i < SR_LENS_MAX; i ++){
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%d", i + 1);
		if(strcmp(szBuff, csBuff) == 0)
			iLens = i;
	}
	if(!(iLens == m_iBaseLens)){
		m_iBaseLens = iLens;
		EnableButtons(FALSE);
	}
	GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(FALSE);
	// Kojika 20090529 Change
	//SetDlgItemText(IDC_GUIDANCE, "Please Specify the Position of a Base Lens.");
	LoadStringML(IDS_SPECIFY_POSITION_BASE_LENS, l_strBuffer, "Please Specify the Position of a Base Lens.");
	SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
	// Kojika 20090529 Change End
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnLens1SetupButton()
{
	COverlayDialog dlgOverlay;
	long lX, lY, lZ;
	char szBuff[256];
	CString csBuff;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strCap, l_strMsg;
	// Kojika 20090529 Add End

	SetOperationLog("Lens1 SETUP Button was push.");

	// １つ目のレンズを選択
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(0);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	// X
	GetDlgItemText(LensOffsetX_IDs[0], csBuff);
	dlgOverlay.m_lX = atol(csBuff);
	// Y
	GetDlgItemText(LensOffsetY_IDs[0], csBuff);
	dlgOverlay.m_lY = atol(csBuff);
	// Z
	GetDlgItemText(LensOffsetZ_IDs[0], csBuff);
	dlgOverlay.m_lZ = atol(csBuff);

	// Kojika 20090529 Change
	//dlgOverlay.SetCaption(LENS_CENTER_OFFSET_SETTING_CAPTION);
	//dlgOverlay.SetMessage(LENS_CENTER_OFFSET_SETTING_MESSAGE);
	LoadStringML(IDS_LENS_OFFSET_SETTING_CAPTION, l_strCap, "LENS CENTER OFFSET SETTING");
	LoadStringML(IDS_LENS_OFFSET_SETTING_MESSAGE, l_strMsg, "LENS CENTER OFFSET SETTING");
	dlgOverlay.SetCaption(l_strCap);
	dlgOverlay.SetMessage(l_strMsg);
	// Kojika 20090529 Change End
	if(dlgOverlay.DoModal() == IDOK){
		if(m_iBaseLens == 0){
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lX);
			SetDlgItemText(IDC_BASEPOS_X, szBuff);
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lY);
			SetDlgItemText(IDC_BASEPOS_Y, szBuff);
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lZ);
			SetDlgItemText(IDC_BASEPOS_Z, szBuff);

			m_LensOffset.Lens[m_iBaseLens].lX = dlgOverlay.m_lX;
			m_LensOffset.Lens[m_iBaseLens].lY = dlgOverlay.m_lY;
			m_LensOffset.Lens[m_iBaseLens].lZ = dlgOverlay.m_lZ;
		}

		lX = dlgOverlay.m_lX - m_LensOffset.Lens[m_iBaseLens].lX;
		lY = dlgOverlay.m_lY - m_LensOffset.Lens[m_iBaseLens].lY;
		lZ = dlgOverlay.m_lZ - m_LensOffset.Lens[m_iBaseLens].lZ;

		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lX);
		SetDlgItemText(LensOffsetX_IDs[0], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lY);
		SetDlgItemText(LensOffsetY_IDs[0], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lZ);
		SetDlgItemText(LensOffsetZ_IDs[0], szBuff);

		// Kojika 20090529 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090529 Change End
		EnableButtons(TRUE);

		GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(TRUE);
	}
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnLens2SetupButton()
{
	COverlayDialog dlgOverlay;
	long lX, lY, lZ;
	char szBuff[256];
	CString csBuff;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strCap, l_strMsg;
	// Kojika 20090529 Add End

	SetOperationLog("Lens2 SETUP Button was push.");

	// ２つ目のレンズを選択
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(1);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	// X
	GetDlgItemText(LensOffsetX_IDs[1], csBuff);
	dlgOverlay.m_lX = atol(csBuff);
	// Y
	GetDlgItemText(LensOffsetY_IDs[1], csBuff);
	dlgOverlay.m_lY = atol(csBuff);
	// Z
	GetDlgItemText(LensOffsetZ_IDs[1], csBuff);
	dlgOverlay.m_lZ = atol(csBuff);

	// Kojika 20090529 Change
	//dlgOverlay.SetCaption(LENS_CENTER_OFFSET_SETTING_CAPTION);
	//dlgOverlay.SetMessage(LENS_CENTER_OFFSET_SETTING_MESSAGE);
	LoadStringML(IDS_LENS_OFFSET_SETTING_CAPTION, l_strCap, "LENS CENTER OFFSET SETTING");
	LoadStringML(IDS_LENS_OFFSET_SETTING_MESSAGE, l_strMsg, "LENS CENTER OFFSET SETTING");
	dlgOverlay.SetCaption(l_strCap);
	dlgOverlay.SetMessage(l_strMsg);
	// Kojika 20090529 Change End
	if(dlgOverlay.DoModal() == IDOK){
		if(m_iBaseLens == 1){
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lX);
			SetDlgItemText(IDC_BASEPOS_X, szBuff);
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lY);
			SetDlgItemText(IDC_BASEPOS_Y, szBuff);
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lZ);
			SetDlgItemText(IDC_BASEPOS_Z, szBuff);

			m_LensOffset.Lens[m_iBaseLens].lX = dlgOverlay.m_lX;
			m_LensOffset.Lens[m_iBaseLens].lY = dlgOverlay.m_lY;
			m_LensOffset.Lens[m_iBaseLens].lZ = dlgOverlay.m_lZ;
		}

		lX = dlgOverlay.m_lX - m_LensOffset.Lens[m_iBaseLens].lX;
		lY = dlgOverlay.m_lY - m_LensOffset.Lens[m_iBaseLens].lY;
		lZ = dlgOverlay.m_lZ - m_LensOffset.Lens[m_iBaseLens].lZ;

		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lX);
		SetDlgItemText(LensOffsetX_IDs[1], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lY);
		SetDlgItemText(LensOffsetY_IDs[1], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lZ);
		SetDlgItemText(LensOffsetZ_IDs[1], szBuff);

		// Kojika 20090529 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090529 Change End
		EnableButtons(TRUE);

		GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(TRUE);
	}
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnLens3SetupButton()
{
	COverlayDialog dlgOverlay;
	long lX, lY, lZ;
	char szBuff[256];
	CString csBuff;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strCap, l_strMsg;
	// Kojika 20090529 Add End

	SetOperationLog("Lens3 SETUP Button was push.");

	// ３つ目のレンズを選択
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(2);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	// X
	GetDlgItemText(LensOffsetX_IDs[2], csBuff);
	dlgOverlay.m_lX = atol(csBuff);
	// Y
	GetDlgItemText(LensOffsetY_IDs[2], csBuff);
	dlgOverlay.m_lY = atol(csBuff);
	// Z
	GetDlgItemText(LensOffsetZ_IDs[2], csBuff);
	dlgOverlay.m_lZ = atol(csBuff);

	// Kojika 20090529 Change
	//dlgOverlay.SetCaption(LENS_CENTER_OFFSET_SETTING_CAPTION);
	//dlgOverlay.SetMessage(LENS_CENTER_OFFSET_SETTING_MESSAGE);
	LoadStringML(IDS_LENS_OFFSET_SETTING_CAPTION, l_strCap, "LENS CENTER OFFSET SETTING");
	LoadStringML(IDS_LENS_OFFSET_SETTING_MESSAGE, l_strMsg, "LENS CENTER OFFSET SETTING");
	dlgOverlay.SetCaption(l_strCap);
	dlgOverlay.SetMessage(l_strMsg);
	// Kojika 20090529 Change End
	if(dlgOverlay.DoModal() == IDOK){
		if(m_iBaseLens == 2){
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lX);
			SetDlgItemText(IDC_BASEPOS_X, szBuff);
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lY);
			SetDlgItemText(IDC_BASEPOS_Y, szBuff);
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lZ);
			SetDlgItemText(IDC_BASEPOS_Z, szBuff);

			m_LensOffset.Lens[m_iBaseLens].lX = dlgOverlay.m_lX;
			m_LensOffset.Lens[m_iBaseLens].lY = dlgOverlay.m_lY;
			m_LensOffset.Lens[m_iBaseLens].lZ = dlgOverlay.m_lZ;
		}

		lX = dlgOverlay.m_lX - m_LensOffset.Lens[m_iBaseLens].lX;
		lY = dlgOverlay.m_lY - m_LensOffset.Lens[m_iBaseLens].lY;
		lZ = dlgOverlay.m_lZ - m_LensOffset.Lens[m_iBaseLens].lZ;

		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lX);
		SetDlgItemText(LensOffsetX_IDs[2], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lY);
		SetDlgItemText(LensOffsetY_IDs[2], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lZ);
		SetDlgItemText(LensOffsetZ_IDs[2], szBuff);

		// Kojika 20090529 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090529 Change End
		EnableButtons(TRUE);

		GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(TRUE);
	}
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::OnLens4SetupButton()
{
	COverlayDialog dlgOverlay;
	long lX, lY, lZ;
	char szBuff[256];
	CString csBuff;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strCap, l_strMsg;
	// Kojika 20090529 Add End

	SetOperationLog("Lens4 SETUP Button was push.");

	// ４つ目のレンズを選択
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(3);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	// X
	GetDlgItemText(LensOffsetX_IDs[3], csBuff);
	dlgOverlay.m_lX = atol(csBuff);
	// Y
	GetDlgItemText(LensOffsetY_IDs[3], csBuff);
	dlgOverlay.m_lY = atol(csBuff);
	// Z
	GetDlgItemText(LensOffsetZ_IDs[3], csBuff);
	dlgOverlay.m_lZ = atol(csBuff);

	// Kojika 20090529 Change
	//dlgOverlay.SetCaption(LENS_CENTER_OFFSET_SETTING_CAPTION);
	//dlgOverlay.SetMessage(LENS_CENTER_OFFSET_SETTING_MESSAGE);
	LoadStringML(IDS_LENS_OFFSET_SETTING_CAPTION, l_strCap, "LENS CENTER OFFSET SETTING");
	LoadStringML(IDS_LENS_OFFSET_SETTING_MESSAGE, l_strMsg, "LENS CENTER OFFSET SETTING");
	dlgOverlay.SetCaption(l_strCap);
	dlgOverlay.SetMessage(l_strMsg);
	// Kojika 20090529 Change End
	if(dlgOverlay.DoModal() == IDOK){
		if(m_iBaseLens == 3){
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lX);
			SetDlgItemText(IDC_BASEPOS_X, szBuff);
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lY);
			SetDlgItemText(IDC_BASEPOS_Y, szBuff);
			memset(szBuff, 0, sizeof(szBuff));
			sprintf(szBuff, "%ld", dlgOverlay.m_lZ);
			SetDlgItemText(IDC_BASEPOS_Z, szBuff);

			m_LensOffset.Lens[m_iBaseLens].lX = dlgOverlay.m_lX;
			m_LensOffset.Lens[m_iBaseLens].lY = dlgOverlay.m_lY;
			m_LensOffset.Lens[m_iBaseLens].lZ = dlgOverlay.m_lZ;
		}

		lX = dlgOverlay.m_lX - m_LensOffset.Lens[m_iBaseLens].lX;
		lY = dlgOverlay.m_lY - m_LensOffset.Lens[m_iBaseLens].lY;
		lZ = dlgOverlay.m_lZ - m_LensOffset.Lens[m_iBaseLens].lZ;

		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lX);
		SetDlgItemText(LensOffsetX_IDs[3], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lY);
		SetDlgItemText(LensOffsetY_IDs[3], szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%ld", lZ);
		SetDlgItemText(LensOffsetZ_IDs[3], szBuff);

		// Kojika 20090529 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090529 Change End
		EnableButtons(TRUE);

		GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(TRUE);
	}
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::EnableButtons(BOOL flag)
{
	GetDlgItem(IDC_BASE_LENS)->EnableWindow(!flag);

	switch(m_iBaseLens){
	case 0:
		GetDlgItem(IDC_LENS1_SETUP_BUTTON)->EnableWindow(m_SrTurret[0].bEnable ? !flag : FALSE);
		GetDlgItem(IDC_LENS2_SETUP_BUTTON)->EnableWindow(m_SrTurret[1].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS3_SETUP_BUTTON)->EnableWindow(m_SrTurret[2].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS4_SETUP_BUTTON)->EnableWindow(m_SrTurret[3].bEnable ? flag : FALSE);
		break;
	case 1:
		GetDlgItem(IDC_LENS1_SETUP_BUTTON)->EnableWindow(m_SrTurret[0].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS2_SETUP_BUTTON)->EnableWindow(m_SrTurret[1].bEnable ? !flag : FALSE);
		GetDlgItem(IDC_LENS3_SETUP_BUTTON)->EnableWindow(m_SrTurret[2].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS4_SETUP_BUTTON)->EnableWindow(m_SrTurret[3].bEnable ? flag : FALSE);
		break;
	case 2:
		GetDlgItem(IDC_LENS1_SETUP_BUTTON)->EnableWindow(m_SrTurret[0].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS2_SETUP_BUTTON)->EnableWindow(m_SrTurret[1].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS3_SETUP_BUTTON)->EnableWindow(m_SrTurret[2].bEnable ? !flag : FALSE);
		GetDlgItem(IDC_LENS4_SETUP_BUTTON)->EnableWindow(m_SrTurret[3].bEnable ? flag : FALSE);
		break;
	case 3:
		GetDlgItem(IDC_LENS1_SETUP_BUTTON)->EnableWindow(m_SrTurret[0].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS2_SETUP_BUTTON)->EnableWindow(m_SrTurret[1].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS3_SETUP_BUTTON)->EnableWindow(m_SrTurret[2].bEnable ? flag : FALSE);
		GetDlgItem(IDC_LENS4_SETUP_BUTTON)->EnableWindow(m_SrTurret[3].bEnable ? !flag : FALSE);
		break;
	default:
		break;
	}
}

// =========================================================================
//
void CSrLensCenterOffsetSetingDlg::SetOperationLog(LPCTSTR pszLog)
{
	char strmsg[1024];

	// 操作ログを取得
	memset( strmsg, 0, sizeof(strmsg));
	strcpy( strmsg, "[");
	::GetWindowText(this->m_hWnd, &strmsg[1], 1000);
	_tcscat( strmsg, "] ");
	_tcscat( strmsg, pszLog);

	m_pDoc->OperationLogging(strmsg);
}
