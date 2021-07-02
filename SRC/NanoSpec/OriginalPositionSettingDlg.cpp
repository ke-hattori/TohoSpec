// OriginalPositionSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "../../INC/NSStage.hxx"
#include "OverlayDialog.h"
// 2009.09.18 bagus SE --{--
#include "ManualStageDlg.h"
// 2009.09.18 bagus SE --}--
#include "OriginalPositionSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// #########################################################################
// COriginalPositionSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
COriginalPositionSettingDlg::COriginalPositionSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COriginalPositionSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COriginalPositionSettingDlg)
	//}}AFX_DATA_INIT
	m_bBaseHeadEnable = TRUE;
	memset(&m_HeadType, 0, sizeof(m_HeadType));
	memset(&m_OriginalPosition, 0, sizeof(m_OriginalPosition));

// 2009.09.17 bagus SE --{--
	memset(&m_BasePos, 0, sizeof(m_BasePos));
// 2009.09.17 bagus SE --}--
}

// =========================================================================
//
void COriginalPositionSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COriginalPositionSettingDlg)
	//}}AFX_DATA_MAP
// 2009.09.17 bagus SE --{--
	DDX_Control(pDX, IDC_POS_X, m_stcPos_X);
	DDX_Control(pDX, IDC_POS_Y, m_stcPos_Y);
	DDX_Control(pDX, IDC_POS_Z, m_stcPos_Z);
// 2009.09.17 bagus SE --}--
	DDX_Control(pDX, IDC_SR_X, m_stcSR_X);
	DDX_Control(pDX, IDC_SR_Y, m_stcSR_Y);
	DDX_Control(pDX, IDC_SR_Z, m_stcSR_Z);
	DDX_Control(pDX, IDC_SE_X, m_stcSE_X);
	DDX_Control(pDX, IDC_SE_Y, m_stcSE_Y);
	DDX_Control(pDX, IDC_SE_Z, m_stcSE_Z);
	DDX_Control(pDX, IDC_4PP_X, m_stc4PP_X);
	DDX_Control(pDX, IDC_4PP_Y, m_stc4PP_Y);
	DDX_Control(pDX, IDC_4PP_Z, m_stc4PP_Z);
	DDX_Control(pDX, IDC_CONTACT_ANGLE_X, m_stcCONTACT_ANGLE_X);
	DDX_Control(pDX, IDC_CONTACT_ANGLE_Y, m_stcCONTACT_ANGLE_Y);
	DDX_Control(pDX, IDC_CONTACT_ANGLE_Z, m_stcCONTACT_ANGLE_Z);
	DDX_Control(pDX, IDC_STRESS_X, m_stcSTRESS_X);
	DDX_Control(pDX, IDC_STRESS_Y, m_stcSTRESS_Y);
	DDX_Control(pDX, IDC_STRESS_Z, m_stcSTRESS_Z);
	DDX_Control(pDX, IDC_MS_X, m_stcMS_X);
	DDX_Control(pDX, IDC_MS_Y, m_stcMS_Y);
	DDX_Control(pDX, IDC_MS_Z, m_stcMS_Z);
	DDX_Control(pDX, IDC_SR_SETUP_BUTTON, m_SrSetupButton);
	DDX_Control(pDX, IDC_SE_SETUP_BUTTON, m_SeSetupButton);
	DDX_Control(pDX, IDC_MS_SETUP_BUTTON, m_IrseSetupButton);
	DDX_Control(pDX, IDC_4PP_SETUP_BUTTON, m_4PPSetupButton);
	DDX_Control(pDX, IDC_CONTACT_ANGLE_SETUP_BUTTON, m_ContactAngleSetupButton);
	DDX_Control(pDX, IDC_STRESS_SETUP_BUTTON, m_StressSetupButton);
	DDX_Control(pDX, IDC_ALL_CLEAR, m_AllClearButton);
// 2009.09.17 bagus SE --{--
	DDX_Control(pDX, IDC_CHECK, m_CheckButton);
	DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
// 2009.09.17 bagus SE --}--
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(COriginalPositionSettingDlg, CDialog)
	//{{AFX_MSG_MAP(COriginalPositionSettingDlg)
	ON_BN_CLICKED(IDC_SR_SETUP_BUTTON, OnSrSetupButton)
	ON_BN_CLICKED(IDC_SE_SETUP_BUTTON, OnSeSetupButton)
	ON_BN_CLICKED(IDC_MS_SETUP_BUTTON, OnMsSetupButton)
	ON_BN_CLICKED(IDC_4PP_SETUP_BUTTON, On4PPSetupButton)
	ON_BN_CLICKED(IDC_CONTACT_ANGLE_SETUP_BUTTON, OnContactAngleSetupButton)
	ON_BN_CLICKED(IDC_STRESS_SETUP_BUTTON, OnStressSetupButton)
	ON_BN_CLICKED(IDC_ALL_CLEAR, OnAllClear)
	ON_CBN_SELCHANGE(IDC_BASE_HEAD_TYPE, OnSelchangeBaseHeadType)
	ON_BN_CLICKED(IDC_CHECK, OnCheck)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// COriginalPositionSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL COriginalPositionSettingDlg::OnInitDialog()
{
// 2009.09.17 bagus SE --{--
	//未対応機能disable
//	GetDlgItem(IDC_ALL_CLEAR)->EnableWindow(FALSE);
// 2009.09.17 bagus SE --}--

	CDialog::OnInitDialog();

	///// Static Control /////
// 2009.09.17 bagus SE --{--
	m_stcPos_X.SetBkColor(REFERENCE_COLOR);
	m_stcPos_Y.SetBkColor(REFERENCE_COLOR);
	m_stcPos_Z.SetBkColor(REFERENCE_COLOR);
// 2009.09.17 bagus SE --}--
	m_stcSR_X.SetBkColor(REFERENCE_COLOR);
	m_stcSR_Y.SetBkColor(REFERENCE_COLOR);
	m_stcSR_Z.SetBkColor(REFERENCE_COLOR);
	m_stcSE_X.SetBkColor(REFERENCE_COLOR);
	m_stcSE_Y.SetBkColor(REFERENCE_COLOR);
	m_stcSE_Z.SetBkColor(REFERENCE_COLOR);
// 2009.10.19 bagus MS 追加 --{--
#if 0
	m_stcIRSE_X.SetBkColor(REFERENCE_COLOR);
	m_stcIRSE_Y.SetBkColor(REFERENCE_COLOR);
	m_stcIRSE_Z.SetBkColor(REFERENCE_COLOR);
#else
	m_stcMS_X.SetBkColor(REFERENCE_COLOR);
	m_stcMS_Y.SetBkColor(REFERENCE_COLOR);
	m_stcMS_Z.SetBkColor(REFERENCE_COLOR);
#endif
// 2009.10.19 bagus MS 追加 --}--
	m_stc4PP_X.SetBkColor(REFERENCE_COLOR);
	m_stc4PP_Y.SetBkColor(REFERENCE_COLOR);
	m_stc4PP_Z.SetBkColor(REFERENCE_COLOR);
	m_stcCONTACT_ANGLE_X.SetBkColor(REFERENCE_COLOR);
	m_stcCONTACT_ANGLE_Y.SetBkColor(REFERENCE_COLOR);
	m_stcCONTACT_ANGLE_Z.SetBkColor(REFERENCE_COLOR);
	m_stcSTRESS_X.SetBkColor(REFERENCE_COLOR);
	m_stcSTRESS_Y.SetBkColor(REFERENCE_COLOR);
	m_stcSTRESS_Z.SetBkColor(REFERENCE_COLOR);

	m_SrSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SrSetupButton.DrawFlatFocus(TRUE);
	m_SeSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SeSetupButton.DrawFlatFocus(TRUE);
	m_IrseSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_IrseSetupButton.DrawFlatFocus(TRUE);
	m_4PPSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_4PPSetupButton.DrawFlatFocus(TRUE);
	m_ContactAngleSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ContactAngleSetupButton.DrawFlatFocus(TRUE);
	m_StressSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_StressSetupButton.DrawFlatFocus(TRUE);

	m_AllClearButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AllClearButton.DrawFlatFocus(TRUE);
	m_CheckButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CheckButton.DrawFlatFocus(TRUE);
	m_ApplyButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ApplyButton.DrawFlatFocus(TRUE);

	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	////// Load Data /////
	ConfigFile_GetNanoSpecIni(&m_HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_OriginalPosition, CONFIG_FILE_ORIGINAL_POSITION);

	///// Get Base Head Type /////
	m_iBaseHeadType = m_OriginalPosition.wBaseHeadType;


	///// ComboBox Initilize /////
	if(!Init()){
		CDialog::OnCancel();
		return FALSE;
	}

	///// Set Data /////
	UpDate(FALSE);

	/////Control Initilize /////
	GetDlgItem(IDC_BASE_HEAD_TYPE)->EnableWindow(FALSE);
// 2009.09.17 bagus SE --{--
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
// 2009.09.17 bagus SE --}--
	GetDlgItem(IDC_SR_SETUP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_SE_SETUP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_MS_SETUP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_4PP_SETUP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONTACT_ANGLE_SETUP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_STRESS_SETUP_BUTTON)->EnableWindow(FALSE);

// 2009.11.20 K.Matsuo ReEdit -->
	EnableSetButtons(FALSE);
	GetDlgItem(IDC_BASE_HEAD_TYPE)->EnableWindow(FALSE);
// 2009.11.20 K.Matsuo ReEdit <--

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void COriginalPositionSettingDlg::OnOK()
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

	///// Save /////
	m_OriginalPosition.wBaseHeadType = m_iBaseHeadType;

	if(!m_bBaseHeadEnable){
// Kojika 20090526 Change
//		MessageBox("Re-Set Up with an Effective Base Head.", "ORIGINAL POSITION SETTING", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_RESET_EFFECTIVE_BASE_HEAD, strBuffer1, "Re-Set Up with an Effective Base Head.");
		LoadStringML(IDS_TITLE_ORG_PO_SETTING, strBuffer2, "ORIGINAL POSITION SETTING");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONSTOP);
// Kojika 20090526 Change End
		return;
	}

	UpDate(TRUE);

	ConfigFile_SetNanoSpecIni(&m_OriginalPosition, CONFIG_FILE_ORIGINAL_POSITION);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
BOOL COriginalPositionSettingDlg::Init()
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_BASE_HEAD_TYPE);

	if(m_HeadType.bSR)
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SR]);
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// 	if(m_HeadType.bSE)
// 		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SE]);
	if(m_HeadType.bSE) {
		if(m_HeadType.bCompEASE) {
			pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_COMPEASE]);
		}
		else {
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SE]);
		}
	}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	if(m_HeadType.bMS)
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_IRSE]);
#endif
// 2009.10.19 bagus MS 追加 --}--
	if(m_HeadType.bResist)
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_4PP]);
	if(m_HeadType.bCTA)
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_CTA]);
	if(m_HeadType.bStress)
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_STRESS]);
// 2009.10.19 bagus MS 追加 --{--
	if(m_HeadType.bMS)
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_MS]);
// 2009.10.19 bagus MS 追加 --}--

	if(pCombo->GetCount() <= 0){
// Kojika 20090526 Change
//		MessageBox("Head Setting has not been set up.", "ORIGINAL POSITION SETTING", MB_OK);
		LoadStringML(IDS_HEAD_SETTING_NOT_SETUP, strBuffer1, "Head Setting has not been set up.");
		LoadStringML(IDS_TITLE_ORG_PO_SETTING, strBuffer2, "ORIGINAL POSITION SETTING");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change End
		return FALSE;
	}

	///// Base Headが使用不可のとき /////
	switch (m_iBaseHeadType){
	case HEAD_TYPE_SR:
		if(!m_HeadType.bSR)
			m_bBaseHeadEnable = FALSE;
		break;
	case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//		if (!m_HeadType.bSE)
		if (!m_HeadType.bSE ||
			m_HeadType.bSE && m_HeadType.bCompEASE)
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			m_bBaseHeadEnable = FALSE;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		if(!m_HeadType.bCompEASE)
			m_bBaseHeadEnable = FALSE;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case HEAD_TYPE_IRSE:
		if(!m_HeadType.bMS)
			m_bBaseHeadEnable = FALSE;
		break;
#else
	case HEAD_TYPE_MS:
		if(!m_HeadType.bMS)
			m_bBaseHeadEnable = FALSE;
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:
		if(!m_HeadType.bResist)
			m_bBaseHeadEnable = FALSE;
		break;
	case HEAD_TYPE_CTA:
		if(!m_HeadType.bCTA)
			m_bBaseHeadEnable = FALSE;
		break;
	case HEAD_TYPE_STRESS:
		if(!m_HeadType.bStress)
			m_bBaseHeadEnable = FALSE;
		break;
	default:
		break;
	}
	if(!m_bBaseHeadEnable){
// Kojika 20090526 Change
//		MessageBox("A Base Head cannot be Used.\nPlease Make a Base Head Usable or Newly Specify a Base Head.", "ORIGINAL POSITION SETTING", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_BASE_NOT_USED, strBuffer1, "A Base Head cannot be Used.\nPlease Make a Base Head Usable or Newly Specify a Base Head.");
		LoadStringML(IDS_TITLE_ORG_PO_SETTING, strBuffer2, "ORIGINAL POSITION SETTING");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONSTOP);
// Kojika 20090526 Change End
		SetDlgItemText(IDC_GUIDANCE, "A Base Head cannot be Used. Make a Base Head Usable or Newly Specify a Base Head.");
	}
// 2009.09.17 bagus SE --{--
#if 1
	pCombo->SetCurSel(0);
#else
	CString strBuffer;

	if (pCombo->GetCount() > 0) {
		pCombo->SetCurSel(m_iBaseHeadType);

		pCombo->GetLBText(m_iBaseHeadType, strBuffer);
		for ( int iHead = 0; iHead < HEAD_TYPE_MAX; iHead++ ) {
			if ( strBuffer.Compare(HEAD_TYPE_ITEM[iHead]) == 0 )
				break;
		}
		if ( iHead != m_iBaseHeadType ) {
			m_iBaseHeadType = iHead;
			EnableSetButtons(FALSE);
		}
	}
#endif
// 2009.09.17 bagus SE --}--
	return TRUE;
}

// =========================================================================
//
void COriginalPositionSettingDlg::OnAllClear()
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

// Kojika 20090526 Change
//	if ( MessageBox("Do You Want All Clear?", "Lens Center Offset", MB_OKCANCEL|MB_ICONQUESTION) == IDOK ) {
	LoadStringML(IDS_ALL_CLEAR, strBuffer1, "Do You Want All Clear?");
	LoadStringML(IDS_TITLE_LENS_CENTER_OFFSET, strBuffer2, "Lens Center Offset");
	if ( MessageBox(strBuffer1, strBuffer2, MB_OKCANCEL|MB_ICONQUESTION) == IDOK ) {
// Kojika 20090526 Change End
		::ZeroMemory(&m_OriginalPosition, sizeof(ORIGINAL_POSITION));

		SetDlgItemInt(IDC_SR_X, 0L);
		SetDlgItemInt(IDC_SR_Y, 0L);
		SetDlgItemInt(IDC_SR_Z, 0L);
		SetDlgItemInt(IDC_SE_X, 0L);
		SetDlgItemInt(IDC_SE_Y, 0L);
		SetDlgItemInt(IDC_SE_Z, 0L);
		SetDlgItemInt(IDC_MS_X, 0L);
		SetDlgItemInt(IDC_MS_Y, 0L);
		SetDlgItemInt(IDC_MS_Z, 0L);
		SetDlgItemInt(IDC_4PP_X, 0L);
		SetDlgItemInt(IDC_4PP_Y, 0L);
		SetDlgItemInt(IDC_4PP_Z, 0L);
		SetDlgItemInt(IDC_CONTACT_ANGLE_X, 0L);
		SetDlgItemInt(IDC_CONTACT_ANGLE_Y, 0L);
		SetDlgItemInt(IDC_CONTACT_ANGLE_Z, 0L);
		SetDlgItemInt(IDC_STRESS_X, 0L);
		SetDlgItemInt(IDC_STRESS_Y, 0L);
		SetDlgItemInt(IDC_STRESS_Z, 0L);
// 2009.09.17 bagus SE --{--
		memset(&m_BasePos, 0, sizeof(m_BasePos));
		UpDateBasePos();
// 2009.09.17 bagus SE --}--

		// Kojika 20090603 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Specify the Position of a Base Head.");
		LoadStringML(IDS_SPECIFY_POSITION_BASE_HEAD, strBuffer1, "Please Specify the Position of a Base Head.");
		SetDlgItemText(IDC_GUIDANCE, strBuffer1);
		// Kojika 20090603 Change End

		EnableSetButtons(FALSE);
	}
}

// =========================================================================
//
void COriginalPositionSettingDlg::OnSelchangeBaseHeadType()
{
	CString strBuffer;

	CComboBox* pCombo = (CComboBox *) GetDlgItem(IDC_BASE_HEAD_TYPE);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect != CB_ERR ) {
		pCombo->GetLBText(pCombo->GetCurSel(), strBuffer);
		for ( int iHead = 0; iHead < HEAD_TYPE_MAX; iHead++ ) {
			if ( strBuffer.Compare(HEAD_TYPE_ITEM[iHead]) == 0 )
				break;
		}
		if ( iHead != m_iBaseHeadType ) {
			m_iBaseHeadType = iHead;
			EnableSetButtons(FALSE);
		}
	}

// 2009.09.17 bagus SE --{--
	UpDateBasePos();
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
// 2009.09.17 bagus SE --}--
}

// =========================================================================
//
void COriginalPositionSettingDlg::OnSrSetupButton()
{
	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	COverlayDialog dlgOverlay;
	CString strBuffer;
	long lX, lY, lZ;

	// X
	GetDlgItemText(IDC_SR_X, strBuffer);
	dlgOverlay.m_lX = atol(strBuffer);
	// Y
	GetDlgItemText(IDC_SR_Y, strBuffer);
	dlgOverlay.m_lY = atol(strBuffer);
	// Z
	GetDlgItemText(IDC_SR_Z, strBuffer);
	dlgOverlay.m_lZ = atol(strBuffer);

	if ( dlgOverlay.DoModal() == IDOK ) {
		if ( m_iBaseHeadType == HEAD_TYPE_SR ) {
			m_OriginalPosition.LocSr.lX = dlgOverlay.m_lX;
			m_OriginalPosition.LocSr.lY = dlgOverlay.m_lY;
			m_OriginalPosition.LocSr.lZ = dlgOverlay.m_lZ;
// 2009.09.17 bagus SE --{--
			m_BasePos.lX = m_OriginalPosition.LocSr.lX;
			m_BasePos.lY = m_OriginalPosition.LocSr.lY;
			m_BasePos.lZ = m_OriginalPosition.LocSr.lZ;
			UpDateBasePos();
// 2009.09.17 bagus SE --}--
		}
		switch ( m_iBaseHeadType ) {
		case HEAD_TYPE_SR:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSr.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SR_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSr.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SR_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSr.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SR_Z, strBuffer);
			break;
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSe.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SR_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSe.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SR_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSe.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SR_Z, strBuffer);
			break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocIrse.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SR_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocIrse.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SR_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocIrse.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SR_Z, strBuffer);
			break;
#else
		case HEAD_TYPE_MS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocMs.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SR_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocMs.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SR_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocMs.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SR_Z, strBuffer);
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			lX = dlgOverlay.m_lX - m_OriginalPosition.Loc4PP.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SR_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.Loc4PP.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SR_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.Loc4PP.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SR_Z, strBuffer);
			break;
		case HEAD_TYPE_CTA:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocContactAngle.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SR_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocContactAngle.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SR_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocContactAngle.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SR_Z, strBuffer);
			break;
		case HEAD_TYPE_STRESS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocStress.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SR_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocStress.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SR_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocStress.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SR_Z, strBuffer);
			break;
		default:
			break;
		}

		// Kojika 20090603 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090603 Change End
		EnableSetButtons(TRUE);
// 2009.09.18 bagus SE --{--
		GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
// 2009.09.18 bagus SE --}--
	}
}

// =========================================================================
//
void COriginalPositionSettingDlg::OnSeSetupButton()
{
	COverlayDialog dlgOverlay;
	CString csTemp;
	CString strBuffer;
	long lX, lY, lZ;

	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	// X
	GetDlgItemText(IDC_SE_X, csTemp);
	dlgOverlay.m_lX = atol(csTemp);
	// Y
	GetDlgItemText(IDC_SE_Y, csTemp);
	dlgOverlay.m_lY = atol(csTemp);
	// Z
	GetDlgItemText(IDC_SE_Z, csTemp);
	dlgOverlay.m_lZ = atol(csTemp);

	if(dlgOverlay.DoModal() == IDOK){
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//		if(m_iBaseHeadType == HEAD_TYPE_SE){
		if(m_iBaseHeadType == HEAD_TYPE_SE ||
		   m_iBaseHeadType == HEAD_TYPE_COMPEASE){
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			m_OriginalPosition.LocSe.lX = dlgOverlay.m_lX;
			m_OriginalPosition.LocSe.lY = dlgOverlay.m_lY;
			m_OriginalPosition.LocSe.lZ = dlgOverlay.m_lZ;
// 2009.09.17 bagus SE --{--
			m_BasePos.lX = m_OriginalPosition.LocSe.lX;
			m_BasePos.lY = m_OriginalPosition.LocSe.lY;
			m_BasePos.lZ = m_OriginalPosition.LocSe.lZ;
			UpDateBasePos();
// 2009.09.17 bagus SE --}--
		}
		switch (m_iBaseHeadType){
		case HEAD_TYPE_SR:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSr.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSr.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSr.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SE_Z, strBuffer);
			break;
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSe.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSe.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSe.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SE_Z, strBuffer);
			break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocIrse.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocIrse.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocIrse.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SE_Z, strBuffer);
			break;
#else
		case HEAD_TYPE_MS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocMs.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocMs.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocMs.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SE_Z, strBuffer);
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			lX = dlgOverlay.m_lX - m_OriginalPosition.Loc4PP.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.Loc4PP.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.Loc4PP.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SE_Z, strBuffer);
			break;
		case HEAD_TYPE_CTA:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocContactAngle.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocContactAngle.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocContactAngle.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SE_Z, strBuffer);
			break;
		case HEAD_TYPE_STRESS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocStress.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_SE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocStress.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_SE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocStress.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_SE_Z, strBuffer);
			break;
		default:
			break;
		}

		// Kojika 20090603 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090603 Change End
		EnableSetButtons(TRUE);
// 2009.09.18 bagus SE --{--
		GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
// 2009.09.18 bagus SE --}--
	}
}

// =========================================================================
//
void COriginalPositionSettingDlg::OnMsSetupButton()
{
	COverlayDialog dlgOverlay;
	CString csTemp;
	CString strBuffer;
	long lX, lY, lZ;

	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	// X
	GetDlgItemText(IDC_MS_X, csTemp);
	dlgOverlay.m_lX = atol(csTemp);
	// Y
	GetDlgItemText(IDC_MS_Y, csTemp);
	dlgOverlay.m_lY = atol(csTemp);
	// Z
	GetDlgItemText(IDC_MS_Z, csTemp);
	dlgOverlay.m_lZ = atol(csTemp);

	if(dlgOverlay.DoModal() == IDOK){
// 2009.10.19 bagus MS 追加 --{--
#if 0
		if(m_iBaseHeadType == HEAD_TYPE_IRSE){
			m_OriginalPosition.LocIrse.lX = dlgOverlay.m_lX;
			m_OriginalPosition.LocIrse.lY = dlgOverlay.m_lY;
			m_OriginalPosition.LocIrse.lZ = dlgOverlay.m_lZ;
// 2009.09.17 bagus SE --{--
			m_BasePos.lX = m_OriginalPosition.LocIrse.lX;
			m_BasePos.lY = m_OriginalPosition.LocIrse.lY;
			m_BasePos.lZ = m_OriginalPosition.LocIrse.lZ;
			UpDateBasePos();
// 2009.09.17 bagus SE --}--
		}
#else
		if(m_iBaseHeadType == HEAD_TYPE_MS){
			m_OriginalPosition.LocMs.lX = dlgOverlay.m_lX;
			m_OriginalPosition.LocMs.lY = dlgOverlay.m_lY;
			m_OriginalPosition.LocMs.lZ = dlgOverlay.m_lZ;
			m_BasePos.lX = m_OriginalPosition.LocMs.lX;
			m_BasePos.lY = m_OriginalPosition.LocMs.lY;
			m_BasePos.lZ = m_OriginalPosition.LocMs.lZ;
			UpDateBasePos();
		}
#endif
// 2009.10.19 bagus MS 追加 --}--
		switch (m_iBaseHeadType){
		case HEAD_TYPE_SR:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSr.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_MS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSr.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_MS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSr.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_MS_Z, strBuffer);
			break;
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSe.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_MS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSe.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_MS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSe.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_MS_Z, strBuffer);
			break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocIrse.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_MS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocIrse.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_MS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocIrse.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_MS_Z, strBuffer);
			break;
#else
		case HEAD_TYPE_MS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocMs.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_MS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocMs.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_MS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocMs.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_MS_Z, strBuffer);
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			lX = dlgOverlay.m_lX - m_OriginalPosition.Loc4PP.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_MS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.Loc4PP.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_MS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.Loc4PP.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_MS_Z, strBuffer);
			break;
		case HEAD_TYPE_CTA:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocContactAngle.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_MS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocContactAngle.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_MS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocContactAngle.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_MS_Z, strBuffer);
			break;
		case HEAD_TYPE_STRESS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocStress.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_MS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocStress.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_MS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocStress.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_MS_Z, strBuffer);
			break;
		default:
			break;
		}

		// Kojika 20090603 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090603 Change End
		EnableSetButtons(TRUE);
// 2009.09.18 bagus SE --{--
		GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
// 2009.09.18 bagus SE --}--
	}
}

// =========================================================================
//
void COriginalPositionSettingDlg::On4PPSetupButton()
{
	COverlayDialog dlgOverlay;
	CString csTemp;
	CString strBuffer;
	long lX, lY, lZ;

	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	// X
	GetDlgItemText(IDC_4PP_X, csTemp);
	dlgOverlay.m_lX = atol(csTemp);
	// Y
	GetDlgItemText(IDC_4PP_Y, csTemp);
	dlgOverlay.m_lY = atol(csTemp);
	// Z
	GetDlgItemText(IDC_4PP_Z, csTemp);
	dlgOverlay.m_lZ = atol(csTemp);

	if(dlgOverlay.DoModal() == IDOK){
		if(m_iBaseHeadType == HEAD_TYPE_4PP){
			m_OriginalPosition.Loc4PP.lX = dlgOverlay.m_lX;
			m_OriginalPosition.Loc4PP.lY = dlgOverlay.m_lY;
			m_OriginalPosition.Loc4PP.lZ = dlgOverlay.m_lZ;
// 2009.09.17 bagus SE --{--
			m_BasePos.lX = m_OriginalPosition.Loc4PP.lX;
			m_BasePos.lY = m_OriginalPosition.Loc4PP.lY;
			m_BasePos.lZ = m_OriginalPosition.Loc4PP.lZ;
			UpDateBasePos();
// 2009.09.17 bagus SE --}--
		}
		switch (m_iBaseHeadType){
		case HEAD_TYPE_SR:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSr.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_4PP_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSr.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_4PP_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSr.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_4PP_Z, strBuffer);
			break;
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSe.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_4PP_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSe.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_4PP_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSe.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_4PP_Z, strBuffer);
			break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocIrse.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_4PP_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocIrse.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_4PP_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocIrse.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_4PP_Z, strBuffer);
			break;
#else
		case HEAD_TYPE_MS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocMs.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_4PP_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocMs.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_4PP_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocMs.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_4PP_Z, strBuffer);
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			lX = dlgOverlay.m_lX - m_OriginalPosition.Loc4PP.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_4PP_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.Loc4PP.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_4PP_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.Loc4PP.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_4PP_Z, strBuffer);
			break;
		case HEAD_TYPE_CTA:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocContactAngle.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_4PP_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocContactAngle.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_4PP_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocContactAngle.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_4PP_Z, strBuffer);
			break;
		case HEAD_TYPE_STRESS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocStress.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_4PP_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocStress.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_4PP_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocStress.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_4PP_Z, strBuffer);
			break;
		default:
			break;
		}

		// Kojika 20090603 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090603 Change End
// 2009.09.18 bagus SE --{--
		GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
// 2009.09.18 bagus SE --}--
		EnableSetButtons(TRUE);
	}
}

// =========================================================================
//
void COriginalPositionSettingDlg::OnContactAngleSetupButton()
{
	COverlayDialog dlgOverlay;
	CString csTemp;
	CString strBuffer;
	long lX, lY, lZ;

	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	// X
	GetDlgItemText(IDC_CONTACT_ANGLE_X, csTemp);
	dlgOverlay.m_lX = atol(csTemp);
	// Y
	GetDlgItemText(IDC_CONTACT_ANGLE_Y, csTemp);
	dlgOverlay.m_lY = atol(csTemp);
	// Z
	GetDlgItemText(IDC_CONTACT_ANGLE_Z, csTemp);
	dlgOverlay.m_lZ = atol(csTemp);

	if(dlgOverlay.DoModal() == IDOK){
		if(m_iBaseHeadType == HEAD_TYPE_CTA){
			m_OriginalPosition.LocContactAngle.lX = dlgOverlay.m_lX;
			m_OriginalPosition.LocContactAngle.lY = dlgOverlay.m_lY;
			m_OriginalPosition.LocContactAngle.lZ = dlgOverlay.m_lZ;
// 2009.09.17 bagus SE --{--
			m_BasePos.lX = m_OriginalPosition.LocContactAngle.lX;
			m_BasePos.lY = m_OriginalPosition.LocContactAngle.lY;
			m_BasePos.lZ = m_OriginalPosition.LocContactAngle.lZ;
			UpDateBasePos();
// 2009.09.17 bagus SE --}--
		}
		switch (m_iBaseHeadType){
		case HEAD_TYPE_SR:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSr.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_CONTACT_ANGLE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSr.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_CONTACT_ANGLE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSr.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_CONTACT_ANGLE_Z, strBuffer);
			break;
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSe.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_CONTACT_ANGLE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSe.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_CONTACT_ANGLE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSe.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_CONTACT_ANGLE_Z, strBuffer);
			break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocIrse.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_CONTACT_ANGLE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocIrse.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_CONTACT_ANGLE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocIrse.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_CONTACT_ANGLE_Z, strBuffer);
			break;
#else
		case HEAD_TYPE_MS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocMs.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_CONTACT_ANGLE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocMs.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_CONTACT_ANGLE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocMs.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_CONTACT_ANGLE_Z, strBuffer);
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			lX = dlgOverlay.m_lX - m_OriginalPosition.Loc4PP.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_CONTACT_ANGLE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.Loc4PP.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_CONTACT_ANGLE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.Loc4PP.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_CONTACT_ANGLE_Z, strBuffer);
			break;
		case HEAD_TYPE_CTA:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocContactAngle.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_CONTACT_ANGLE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocContactAngle.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_CONTACT_ANGLE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocContactAngle.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_CONTACT_ANGLE_Z, strBuffer);
			break;
		case HEAD_TYPE_STRESS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocStress.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_CONTACT_ANGLE_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocStress.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_CONTACT_ANGLE_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocStress.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_CONTACT_ANGLE_Z, strBuffer);
			break;
		default:
			break;
		}

		// Kojika 20090603 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090603 Change End
		EnableSetButtons(TRUE);
// 2009.09.18 bagus SE --{--
		GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
// 2009.09.18 bagus SE --}--
	}
}

// =========================================================================
//
void COriginalPositionSettingDlg::OnStressSetupButton()
{
	COverlayDialog dlgOverlay;
	CString csTemp;
	CString strBuffer;
	long lX, lY, lZ;

	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	// X
	GetDlgItemText(IDC_STRESS_X, csTemp);
	dlgOverlay.m_lX = atol(csTemp);
	// Y
	GetDlgItemText(IDC_STRESS_Y, csTemp);
	dlgOverlay.m_lY = atol(csTemp);
	// Z
	GetDlgItemText(IDC_STRESS_Z, csTemp);
	dlgOverlay.m_lZ = atol(csTemp);


	if(dlgOverlay.DoModal() == IDOK){
		if(m_iBaseHeadType == HEAD_TYPE_STRESS){
			m_OriginalPosition.LocStress.lX = dlgOverlay.m_lX;
			m_OriginalPosition.LocStress.lY = dlgOverlay.m_lY;
			m_OriginalPosition.LocStress.lZ = dlgOverlay.m_lZ;
// 2009.09.17 bagus SE --{--
			m_BasePos.lX = m_OriginalPosition.LocStress.lX;
			m_BasePos.lY = m_OriginalPosition.LocStress.lY;
			m_BasePos.lZ = m_OriginalPosition.LocStress.lZ;
			UpDateBasePos();
// 2009.09.17 bagus SE --}--
		}
		switch (m_iBaseHeadType){
		case HEAD_TYPE_SR:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSr.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_STRESS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSr.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_STRESS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSr.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_STRESS_Z, strBuffer);
			break;
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocSe.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_STRESS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocSe.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_STRESS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocSe.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_STRESS_Z, strBuffer);
			break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocIrse.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_STRESS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocIrse.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_STRESS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocIrse.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_STRESS_Z, strBuffer);
			break;
#else
		case HEAD_TYPE_MS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocMs.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_STRESS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocMs.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_STRESS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocMs.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_STRESS_Z, strBuffer);
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			lX = dlgOverlay.m_lX - m_OriginalPosition.Loc4PP.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_STRESS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.Loc4PP.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_STRESS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.Loc4PP.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_STRESS_Z, strBuffer);
			break;
		case HEAD_TYPE_CTA:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocContactAngle.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_STRESS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocContactAngle.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_STRESS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocContactAngle.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_STRESS_Z, strBuffer);
			break;
		case HEAD_TYPE_STRESS:
			lX = dlgOverlay.m_lX - m_OriginalPosition.LocStress.lX;
			strBuffer.Format("%ld", lX);
			SetDlgItemText(IDC_STRESS_X, strBuffer);

			lY = dlgOverlay.m_lY - m_OriginalPosition.LocStress.lY;
			strBuffer.Format("%ld", lY);
			SetDlgItemText(IDC_STRESS_Y, strBuffer);

			lZ = dlgOverlay.m_lZ - m_OriginalPosition.LocStress.lZ;
			strBuffer.Format("%ld", lZ);
			SetDlgItemText(IDC_STRESS_Z, strBuffer);
			break;
		default:
			break;
		}

		// Kojika 20090603 Change
		//SetDlgItemText(IDC_GUIDANCE, "Please Set Up Offset of Each Lens.");
		LoadStringML(IDS_SETUP_OFFSET_EACH_LENS, l_strBuffer, "Please Set Up Offset of Each Lens.");
		SetDlgItemText(IDC_GUIDANCE, l_strBuffer);
		// Kojika 20090603 Change End
		EnableSetButtons(TRUE);
// 2009.09.18 bagus SE --{--
		GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
// 2009.09.18 bagus SE --}--
	}
}

// =========================================================================
//
void COriginalPositionSettingDlg::UpDate(BOOL bValid)
{
	CComboBox* pCombo;
	CString csTemp;
	CString csHeadType;
	int iIndex;
	int i;
	long BaseOffsetX;
	long BaseOffsetY;
	long BaseOffsetZ;

	if(bValid){
		// Base Head Type
		pCombo =	(CComboBox*)GetDlgItem(IDC_BASE_HEAD_TYPE);
		iIndex = pCombo->GetCurSel();
		pCombo->GetLBText(iIndex, csHeadType);
		for(i = 0 ; i < HEAD_TYPE_MAX ; i++){
			if(strcmp(csHeadType, HEAD_TYPE_ITEM[i]) == 0){
				break;
			}
		}
		if(i < HEAD_TYPE_MAX){
			m_OriginalPosition.wBaseHeadType = i;
		}
		else{
			m_OriginalPosition.wBaseHeadType = 0;
		}

		// SR Original Position
		// X
		GetDlgItemText(IDC_SR_X, csTemp);
		m_OriginalPosition.LocSr.lX = atol(csTemp);
		// Y
		GetDlgItemText(IDC_SR_Y, csTemp);
		m_OriginalPosition.LocSr.lY = atol(csTemp);
		// Z
		GetDlgItemText(IDC_SR_Z, csTemp);
		m_OriginalPosition.LocSr.lZ = atol(csTemp);

		// SE Original Position
		// X
		GetDlgItemText(IDC_SE_X, csTemp);
		m_OriginalPosition.LocSe.lX = atol(csTemp);
		// Y
		GetDlgItemText(IDC_SE_Y, csTemp);
		m_OriginalPosition.LocSe.lY = atol(csTemp);
		// Z
		GetDlgItemText(IDC_SE_Z, csTemp);
		m_OriginalPosition.LocSe.lZ = atol(csTemp);

// 2009.10.19 bagus MS 追加 --{--
#if 0
		// IRSE Original Position
		// X
		GetDlgItemText(IDC_MS_X, csTemp);
		m_OriginalPosition.LocIrse.lX = atol(csTemp);
		// Y
		GetDlgItemText(IDC_MS_Y, csTemp);
		m_OriginalPosition.LocIrse.lY = atol(csTemp);
		// Z
		GetDlgItemText(IDC_MS_Z, csTemp);
		m_OriginalPosition.LocIrse.lZ = atol(csTemp);
#else
		// MS Original Position
		// X
		GetDlgItemText(IDC_MS_X, csTemp);
		m_OriginalPosition.LocMs.lX = atol(csTemp);
		// Y
		GetDlgItemText(IDC_MS_Y, csTemp);
		m_OriginalPosition.LocMs.lY = atol(csTemp);
		// Z
		GetDlgItemText(IDC_MS_Z, csTemp);
		m_OriginalPosition.LocMs.lZ = atol(csTemp);
#endif
// 2009.10.19 bagus MS 追加 --}--

		// 4PP Original Position
		// X
		GetDlgItemText(IDC_4PP_X, csTemp);
		m_OriginalPosition.Loc4PP.lX = atol(csTemp);
		// Y
		GetDlgItemText(IDC_4PP_Y, csTemp);
		m_OriginalPosition.Loc4PP.lY = atol(csTemp);
		// Z
		GetDlgItemText(IDC_4PP_Z, csTemp);
		m_OriginalPosition.Loc4PP.lZ = atol(csTemp);

		// Contact Angle Original Position
		// X
		GetDlgItemText(IDC_CONTACT_ANGLE_X, csTemp);
		m_OriginalPosition.LocContactAngle.lX = atol(csTemp);
		// Y
		GetDlgItemText(IDC_CONTACT_ANGLE_Y, csTemp);
		m_OriginalPosition.LocContactAngle.lY = atol(csTemp);
		// Z
		GetDlgItemText(IDC_CONTACT_ANGLE_Z, csTemp);
		m_OriginalPosition.LocContactAngle.lZ = atol(csTemp);

		// Stress Original Position
		// X
		GetDlgItemText(IDC_STRESS_X, csTemp);
		m_OriginalPosition.LocStress.lX = atol(csTemp);
		// Y
		GetDlgItemText(IDC_STRESS_Y, csTemp);
		m_OriginalPosition.LocStress.lY = atol(csTemp);
		// Z
		GetDlgItemText(IDC_STRESS_Z, csTemp);
		m_OriginalPosition.LocStress.lZ = atol(csTemp);

	}
	else{
		// Base Head Type
		pCombo =	(CComboBox*)GetDlgItem(IDC_BASE_HEAD_TYPE);
		iIndex = pCombo->FindString(0, HEAD_TYPE_ITEM[m_OriginalPosition.wBaseHeadType]);
		if(iIndex == CB_ERR) iIndex = 0;
		pCombo->SetCurSel(iIndex);

		switch (iIndex){
		case HEAD_TYPE_SR:
			BaseOffsetX = m_OriginalPosition.LocSr.lX;
			BaseOffsetY = m_OriginalPosition.LocSr.lY;
			BaseOffsetZ = m_OriginalPosition.LocSr.lZ;
			break;
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			BaseOffsetX = m_OriginalPosition.LocSe.lX;
			BaseOffsetY = m_OriginalPosition.LocSe.lY;
			BaseOffsetZ = m_OriginalPosition.LocSe.lZ;
			break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			BaseOffsetX = m_OriginalPosition.LocIrse.lX;
			BaseOffsetY = m_OriginalPosition.LocIrse.lY;
			BaseOffsetZ = m_OriginalPosition.LocIrse.lZ;
			break;
#else
		case HEAD_TYPE_MS:
			BaseOffsetX = m_OriginalPosition.LocMs.lX;
			BaseOffsetY = m_OriginalPosition.LocMs.lY;
			BaseOffsetZ = m_OriginalPosition.LocMs.lZ;
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			BaseOffsetX = m_OriginalPosition.Loc4PP.lX;
			BaseOffsetY = m_OriginalPosition.Loc4PP.lY;
			BaseOffsetZ = m_OriginalPosition.Loc4PP.lZ;
			break;
		case HEAD_TYPE_CTA:
			BaseOffsetX = m_OriginalPosition.LocContactAngle.lX;
			BaseOffsetY = m_OriginalPosition.LocContactAngle.lY;
			BaseOffsetZ = m_OriginalPosition.LocContactAngle.lZ;
			break;
		case HEAD_TYPE_STRESS:
			BaseOffsetX = m_OriginalPosition.LocStress.lX;
			BaseOffsetY = m_OriginalPosition.LocStress.lY;
			BaseOffsetZ = m_OriginalPosition.LocStress.lZ;
			break;
		default:
			break;
		}

		// SR Original Position
		// X
		csTemp.Format("%ld", m_OriginalPosition.LocSr.lX - BaseOffsetX);
		SetDlgItemText(IDC_SR_X, csTemp);
		// Y
		csTemp.Format("%ld", m_OriginalPosition.LocSr.lY - BaseOffsetY);
		SetDlgItemText(IDC_SR_Y, csTemp);
		// Z
		csTemp.Format("%ld", m_OriginalPosition.LocSr.lZ - BaseOffsetZ);
		SetDlgItemText(IDC_SR_Z, csTemp);

		// SE Original Position
		// X
		csTemp.Format("%ld", m_OriginalPosition.LocSe.lX - BaseOffsetX);
		SetDlgItemText(IDC_SE_X, csTemp);
		// Y
		csTemp.Format("%ld", m_OriginalPosition.LocSe.lY - BaseOffsetY);
		SetDlgItemText(IDC_SE_Y, csTemp);
		// Z
		csTemp.Format("%ld", m_OriginalPosition.LocSe.lZ -BaseOffsetZ);
		SetDlgItemText(IDC_SE_Z, csTemp);

// 2009.10.19 bagus MS 追加 --{--
#if 0
		// IRSE Original Position
		// X
		csTemp.Format("%ld", m_OriginalPosition.LocIrse.lX - BaseOffsetX);
		SetDlgItemText(IDC_MS_X, csTemp);
		// Y
		csTemp.Format("%ld", m_OriginalPosition.LocIrse.lY - BaseOffsetY);
		SetDlgItemText(IDC_MS_Y, csTemp);
		// Z
		csTemp.Format("%ld", m_OriginalPosition.LocIrse.lZ - BaseOffsetZ);
		SetDlgItemText(IDC_MS_Z, csTemp);
#else
		// MS Original Position
		// X
		csTemp.Format("%ld", m_OriginalPosition.LocMs.lX - BaseOffsetX);
		SetDlgItemText(IDC_MS_X, csTemp);
		// Y
		csTemp.Format("%ld", m_OriginalPosition.LocMs.lY - BaseOffsetY);
		SetDlgItemText(IDC_MS_Y, csTemp);
		// Z
		csTemp.Format("%ld", m_OriginalPosition.LocMs.lZ - BaseOffsetZ);
		SetDlgItemText(IDC_MS_Z, csTemp);
#endif
// 2009.10.19 bagus MS 追加 --}--

		// 4PP Original Position
		// X
		csTemp.Format("%ld", m_OriginalPosition.Loc4PP.lX - BaseOffsetX);
		SetDlgItemText(IDC_4PP_X, csTemp);
		// Y
		csTemp.Format("%ld", m_OriginalPosition.Loc4PP.lY - BaseOffsetY);
		SetDlgItemText(IDC_4PP_Y, csTemp);
		// Z
		csTemp.Format("%ld", m_OriginalPosition.Loc4PP.lZ - BaseOffsetZ);
		SetDlgItemText(IDC_4PP_Z, csTemp);

		// Contact Angle Original Position
		// X
		csTemp.Format("%ld", m_OriginalPosition.LocContactAngle.lX - BaseOffsetX);
		SetDlgItemText(IDC_CONTACT_ANGLE_X, csTemp);
		// Y
		csTemp.Format("%ld", m_OriginalPosition.LocContactAngle.lY - BaseOffsetY);
		SetDlgItemText(IDC_CONTACT_ANGLE_Y, csTemp);
		// Z
		csTemp.Format("%ld", m_OriginalPosition.LocContactAngle.lZ - BaseOffsetZ);
		SetDlgItemText(IDC_CONTACT_ANGLE_Z, csTemp);

		// Stress Original Position
		// X
		csTemp.Format("%ld", m_OriginalPosition.LocStress.lX - BaseOffsetX);
		SetDlgItemText(IDC_STRESS_X, csTemp);
		// Y
		csTemp.Format("%ld", m_OriginalPosition.LocStress.lY - BaseOffsetY);
		SetDlgItemText(IDC_STRESS_Y, csTemp);
		// Z
		csTemp.Format("%ld", m_OriginalPosition.LocStress.lZ - BaseOffsetZ);
		SetDlgItemText(IDC_STRESS_Z, csTemp);
	}
}

// =========================================================================
//
void COriginalPositionSettingDlg::EnableSetButtons(BOOL flag)
{
	GetDlgItem(IDC_BASE_HEAD_TYPE)->EnableWindow(!flag);

	switch(m_iBaseHeadType){
	case HEAD_TYPE_SR:
		GetDlgItem(IDC_SR_SETUP_BUTTON)->EnableWindow(m_HeadType.bSR ? !flag : FALSE);
		GetDlgItem(IDC_SE_SETUP_BUTTON)->EnableWindow(m_HeadType.bSE ? flag : FALSE);
		GetDlgItem(IDC_MS_SETUP_BUTTON)->EnableWindow(m_HeadType.bMS ? flag : FALSE);
		GetDlgItem(IDC_4PP_SETUP_BUTTON)->EnableWindow(m_HeadType.bResist ? flag : FALSE);
		GetDlgItem(IDC_CONTACT_ANGLE_SETUP_BUTTON)->EnableWindow(m_HeadType.bCTA ? flag : FALSE);
		GetDlgItem(IDC_STRESS_SETUP_BUTTON)->EnableWindow(m_HeadType.bStress ? flag : FALSE);
		break;
	case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		GetDlgItem(IDC_SR_SETUP_BUTTON)->EnableWindow(m_HeadType.bSR ? flag : FALSE);
		GetDlgItem(IDC_SE_SETUP_BUTTON)->EnableWindow(m_HeadType.bSE ? !flag : FALSE);
		GetDlgItem(IDC_MS_SETUP_BUTTON)->EnableWindow(m_HeadType.bMS ? flag : FALSE);
		GetDlgItem(IDC_4PP_SETUP_BUTTON)->EnableWindow(m_HeadType.bResist ? flag : FALSE);
		GetDlgItem(IDC_CONTACT_ANGLE_SETUP_BUTTON)->EnableWindow(m_HeadType.bCTA ? flag : FALSE);
		GetDlgItem(IDC_STRESS_SETUP_BUTTON)->EnableWindow(m_HeadType.bStress ? flag : FALSE);
		break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case HEAD_TYPE_IRSE:
		GetDlgItem(IDC_SR_SETUP_BUTTON)->EnableWindow(m_HeadType.bSR ? flag : FALSE);
		GetDlgItem(IDC_SE_SETUP_BUTTON)->EnableWindow(m_HeadType.bSE ? flag : FALSE);
		GetDlgItem(IDC_MS_SETUP_BUTTON)->EnableWindow(m_HeadType.bMS ? !flag : FALSE);
		GetDlgItem(IDC_4PP_SETUP_BUTTON)->EnableWindow(m_HeadType.bResist ? flag : FALSE);
		GetDlgItem(IDC_CONTACT_ANGLE_SETUP_BUTTON)->EnableWindow(m_HeadType.bCTA ? flag : FALSE);
		GetDlgItem(IDC_STRESS_SETUP_BUTTON)->EnableWindow(m_HeadType.bStress ? flag : FALSE);
		break;
#else
	case HEAD_TYPE_MS:
		GetDlgItem(IDC_SR_SETUP_BUTTON)->EnableWindow(m_HeadType.bSR ? flag : FALSE);
		GetDlgItem(IDC_SE_SETUP_BUTTON)->EnableWindow(m_HeadType.bSE ? flag : FALSE);
		GetDlgItem(IDC_MS_SETUP_BUTTON)->EnableWindow(m_HeadType.bMS ? !flag : FALSE);
		GetDlgItem(IDC_4PP_SETUP_BUTTON)->EnableWindow(m_HeadType.bResist ? flag : FALSE);
		GetDlgItem(IDC_CONTACT_ANGLE_SETUP_BUTTON)->EnableWindow(m_HeadType.bCTA ? flag : FALSE);
		GetDlgItem(IDC_STRESS_SETUP_BUTTON)->EnableWindow(m_HeadType.bStress ? flag : FALSE);
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:
		GetDlgItem(IDC_SR_SETUP_BUTTON)->EnableWindow(m_HeadType.bSR ? flag : FALSE);
		GetDlgItem(IDC_SE_SETUP_BUTTON)->EnableWindow(m_HeadType.bSE ? flag : FALSE);
		GetDlgItem(IDC_MS_SETUP_BUTTON)->EnableWindow(m_HeadType.bMS ? flag : FALSE);
		GetDlgItem(IDC_4PP_SETUP_BUTTON)->EnableWindow(m_HeadType.bResist ? !flag : FALSE);
		GetDlgItem(IDC_CONTACT_ANGLE_SETUP_BUTTON)->EnableWindow(m_HeadType.bCTA ? flag : FALSE);
		GetDlgItem(IDC_STRESS_SETUP_BUTTON)->EnableWindow(m_HeadType.bStress ? flag : FALSE);
		break;
	case HEAD_TYPE_CTA:
		GetDlgItem(IDC_SR_SETUP_BUTTON)->EnableWindow(m_HeadType.bSR ? flag : FALSE);
		GetDlgItem(IDC_SE_SETUP_BUTTON)->EnableWindow(m_HeadType.bSE ? flag : FALSE);
		GetDlgItem(IDC_MS_SETUP_BUTTON)->EnableWindow(m_HeadType.bMS ? flag : FALSE);
		GetDlgItem(IDC_4PP_SETUP_BUTTON)->EnableWindow(m_HeadType.bResist ? flag : FALSE);
		GetDlgItem(IDC_CONTACT_ANGLE_SETUP_BUTTON)->EnableWindow(m_HeadType.bCTA ? !flag : FALSE);
		GetDlgItem(IDC_STRESS_SETUP_BUTTON)->EnableWindow(m_HeadType.bStress ? flag : FALSE);
		break;
	case HEAD_TYPE_STRESS:
		GetDlgItem(IDC_SR_SETUP_BUTTON)->EnableWindow(m_HeadType.bSR ? flag : FALSE);
		GetDlgItem(IDC_SE_SETUP_BUTTON)->EnableWindow(m_HeadType.bSE ? flag : FALSE);
		GetDlgItem(IDC_MS_SETUP_BUTTON)->EnableWindow(m_HeadType.bMS ? flag : FALSE);
		GetDlgItem(IDC_4PP_SETUP_BUTTON)->EnableWindow(m_HeadType.bResist ? flag : FALSE);
		GetDlgItem(IDC_CONTACT_ANGLE_SETUP_BUTTON)->EnableWindow(m_HeadType.bCTA ? flag : FALSE);
		GetDlgItem(IDC_STRESS_SETUP_BUTTON)->EnableWindow(m_HeadType.bStress ? !flag : FALSE);
		break;
	default:
		break;
	}
}

// 2009.09.17 bagus SE --{--
void COriginalPositionSettingDlg::OnCheck()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString			l_strCap, l_strMsg;
	CManualStageDlg	dlg;

	LoadStringML(IDS_ORIGINAL_POSITION_SETTING_CAPTION, l_strCap, "ORIGINAL POSITION SETTING");
	LoadStringML(IDS_ORIGINAL_POSITION_SETTING_MESSAGE, l_strMsg, "ORIGINAL POSITION SETTING");
	dlg.SetCaption(l_strCap);
	dlg.SetMessage(l_strMsg);

	ORIGINAL_POSITION	OldOriginalPosition;

	// 現在の設定値を読み出す
	ConfigFile_GetNanoSpecIni(&OldOriginalPosition, CONFIG_FILE_ORIGINAL_POSITION);

	// 画面の設定値を書き込む
	ConfigFile_SetNanoSpecIni(&m_OriginalPosition, CONFIG_FILE_ORIGINAL_POSITION);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	dlg.m_bOriginalPositionSetting = TRUE;
	dlg.DoModal();
	// ヘッドタイプをSRに戻しておく
	SelectHeadTypeNotify(HEAD_TYPE_SR, FALSE);

	// バックアップしたデータを戻す
	ConfigFile_SetNanoSpecIni(&OldOriginalPosition, CONFIG_FILE_ORIGINAL_POSITION);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);
}

void COriginalPositionSettingDlg::OnApply()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString strBuffer1, strBuffer2;

	///// Save /////
	m_OriginalPosition.wBaseHeadType = m_iBaseHeadType;

	if(!m_bBaseHeadEnable){
		LoadStringML(IDS_RESET_EFFECTIVE_BASE_HEAD, strBuffer1, "Re-Set Up with an Effective Base Head.");
		LoadStringML(IDS_TITLE_ORG_PO_SETTING, strBuffer2, "ORIGINAL POSITION SETTING");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONSTOP);
		return;
	}

	UpDate(TRUE);

	ConfigFile_SetNanoSpecIni(&m_OriginalPosition, CONFIG_FILE_ORIGINAL_POSITION);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

void COriginalPositionSettingDlg::UpDateBasePos()
{
	CString			csTemp;

	// X
	csTemp.Format("%ld", m_BasePos.lX);
	SetDlgItemText(IDC_POS_X, csTemp);
	// Y
	csTemp.Format("%ld", m_BasePos.lY);
	SetDlgItemText(IDC_POS_Y, csTemp);
	// Z
	csTemp.Format("%ld", m_BasePos.lZ);
	SetDlgItemText(IDC_POS_Z, csTemp);
}
// 2009.09.17 bagus SE --}--
