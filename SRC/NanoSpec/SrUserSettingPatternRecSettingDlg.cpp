// SrUserSettingPatternRecSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SrUserSettingPatternRecSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSrUserSettingPatternRecSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSrUserSettingPatternRecSettingDlg::CSrUserSettingPatternRecSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrUserSettingPatternRecSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrUserSettingPatternRecSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSrUserSettingPatternRecSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrUserSettingPatternRecSettingDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrUserSettingPatternRecSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrUserSettingPatternRecSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSrUserSettingPatternRecSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSrUserSettingPatternRecSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	ConfigFile_GetNanoSpecIni(&m_SrUserSettingPatternRec, CONFIG_FILE_SR_PATTERN_REC);
	UpDate(FALSE);


	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSrUserSettingPatternRecSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);
	if ( !UpdateData() )
		return;

	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_SrUserSettingPatternRec, CONFIG_FILE_SR_PATTERN_REC);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_SR_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CSrUserSettingPatternRecSettingDlg::UpDate(BOOL bValid)
{
	char szBuff[256];
	CString csBuff;

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	if(bValid)
	{
		///// Lowest Match Score /////
		GetDlgItemText(IDC_LOWEST_MATCH_SCORE, csBuff);
		m_SrUserSettingPatternRec.dLowestMatchScore = atof(csBuff);

		///// Max.Score to Stop Groping /////
		GetDlgItemText(IDC_STOP_GROPING_SCORE, csBuff);
		m_SrUserSettingPatternRec.dStopGropingScore = atof(csBuff);

		///// No. of Groping Retry Layers /////
		CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_NUM_LAYERS);
		m_SrUserSettingPatternRec.nNumLayers = pCombo->GetCurSel();
	}
	else{
		///// Lowest Match Score /////
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%.1lf", m_SrUserSettingPatternRec.dLowestMatchScore);
		SetDlgItemText(IDC_LOWEST_MATCH_SCORE, szBuff);

		///// Max.Score to Stop Groping /////
		memset(szBuff, 0, sizeof(szBuff));
		sprintf(szBuff, "%.1lf", m_SrUserSettingPatternRec.dStopGropingScore);
		SetDlgItemText(IDC_STOP_GROPING_SCORE, szBuff);

		// No. of Groping Retry Layers
		CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_NUM_LAYERS);
		pCombo->ResetContent();
		for(int i = 0; i < SR_USER_SETTING_PATTERN_REC_NUM_LAYERS_MAX; i++){
			// Kojika 20090528 Change
			//pCombo->AddString(g_lpszSrUserSettingPatternRecNumLayers[i]);
			if(l_SystemConfig.nLanguage == 0){
				pCombo->AddString(g_lpszSrUserSettingPatternRecNumLayers_ENU[i]);
			}else{
				pCombo->AddString(g_lpszSrUserSettingPatternRecNumLayers_JPN[i]);
			}
			// Kojika 20090528 Change End
		}
		pCombo->SetCurSel(m_SrUserSettingPatternRec.nNumLayers);

	}
}

// =========================================================================
//
BOOL CSrUserSettingPatternRecSettingDlg::CheckData()
{
	CString csBuff;

	///// Lowest Match Score /////
	GetDlgItemText(IDC_LOWEST_MATCH_SCORE, csBuff);
	if(CheckMinMaxDouble(csBuff, MIN_MATCH_SCORE, MAX_MATCH_SCORE) != CHECK_DATA_OK){
		GetDlgItem(IDC_LOWEST_MATCH_SCORE)->SetFocus();
		return FALSE;
	}

	///// Max.Score to Stop Groping /////
	GetDlgItemText(IDC_STOP_GROPING_SCORE, csBuff);
	if(CheckMinMaxDouble(csBuff, MIN_MATCH_SCORE, MAX_MATCH_SCORE) != CHECK_DATA_OK){
		GetDlgItem(IDC_STOP_GROPING_SCORE)->SetFocus();
		return FALSE;
	}
	return TRUE;
}
