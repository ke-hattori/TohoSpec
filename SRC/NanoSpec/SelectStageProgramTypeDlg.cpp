// SelectStageProgramTypeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "SelectStageProgramTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2009.10.09 bagus StagePGM 共通化 --{--
#define	STAGE_PROGRAM_HEAD_TYPE_STANDARD	(0)
#define	STAGE_PROGRAM_HEAD_TYPE_STRESS		(1)
#define	STAGE_PROGRAM_HEAD_TYPE_GANTRY		(2)
#define	STAGE_PROGRAM_HEAD_TYPE_DISTANCE	(3)
// 2009.10.09 bagus StagePGM 共通化 --}--

/////////////////////////////////////////////////////////////////////////////
// CSelectStageProgramTypeDlg ダイアログ


CSelectStageProgramTypeDlg::CSelectStageProgramTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectStageProgramTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectStageProgramTypeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

// 2009.10.09 bagus StagePGM 共通化 --{--
//	m_iHeadType = HEAD_TYPE_SR;
	m_iHeadType = STAGE_PGM_TYPE_STANDARD;
	m_iScanType = SCAN_TYPE_STANDARD;
// 2009.10.09 bagus StagePGM 共通化 --}--
}


void CSelectStageProgramTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectStageProgramTypeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CSelectStageProgramTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectStageProgramTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectStageProgramTypeDlg メッセージ ハンドラ

BOOL CSelectStageProgramTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
	SR_CONFIG SrConfig;
	memset(&SrConfig, 0, sizeof(SrConfig));
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);
	//2009.12.07
	SYSTEM_CONFIG SystemConfig;
	memset(&SystemConfig, 0, sizeof(SystemConfig));
	ConfigFile_GetNanoSpecIni(&SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Head Type Combo /////
	HEAD_TYPE_CONFIG HeadType;
	DEFAULT_HEAD_TYPE DefaultHeadType;

	ConfigFile_GetNanoSpecIni(&HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);
	ConfigFile_GetNanoSpecIni(&DefaultHeadType, CONFIG_FILE_DEFAULT_HEAD_TYPE);
	m_iHeadType = DefaultHeadType.nHeadType;

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_HEAD_TYPE);
	pCombo->ResetContent();

	int iSelect = 0;
	CString strBuffer, strTitle;

	// 選択されているヘッドのみ表示
// 2009.10.09 bagus StagePGM 共通化 --{--
#if 0
	if(HeadType.bSR){
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SR]);
		if(m_iHeadType == HEAD_TYPE_SR){
			iSelect = pCombo->GetCount() - 1;
		}
	}
	if(HeadType.bSE){
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SE]);
		if(m_iHeadType == HEAD_TYPE_SE){
			iSelect = pCombo->GetCount() - 1;
		}
	}
	if(HeadType.bIRSE){
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_IRSE]);
		if(m_iHeadType == HEAD_TYPE_IRSE){
			iSelect = pCombo->GetCount() - 1;
		}
	}
	if(HeadType.bResist){
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_4PP]);
		if(m_iHeadType == HEAD_TYPE_4PP){
			iSelect = pCombo->GetCount() - 1;
		}
	}
	if(HeadType.bCTA){
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_CTA]);
		if(m_iHeadType == HEAD_TYPE_CTA){
			iSelect = pCombo->GetCount() - 1;
		}
	}
	if(HeadType.bStress){
		pCombo->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_STRESS]);
		if(m_iHeadType == HEAD_TYPE_STRESS){
			iSelect = pCombo->GetCount() - 1;
		}
	}
	if(HeadType.bSR){
		if(SrConfig.bTransmittance && SrConfig.bGantryType){
//
			pCombo->AddString("SR Gantry Transmittance");
			if(m_iHeadType == HEAD_TYPE_SR){
				iSelect = pCombo->GetCount() - 1;
			}
		}
	}
#else
	const LPCSTR *STAGE_PGM_TYPE_ITEM;
	if(SystemConfig.nLanguage == 0){
		STAGE_PGM_TYPE_ITEM = &STAGE_PGM_TYPE_ITEM_ENU[0];
	}else{
		STAGE_PGM_TYPE_ITEM = STAGE_PGM_TYPE_ITEM_JPN;
	}
// 2009.10.19 bagus MS 追加 --{--
//	if ((HeadType.bSR) || (HeadType.bSE) || (HeadType.bCTA) || (HeadType.bIRSE) || (HeadType.bResist)) {
	if ((HeadType.bSR) || (HeadType.bSE) || (HeadType.bCTA) || (HeadType.bMS) || (HeadType.bResist)) {
// 2009.10.19 bagus MS 追加 --}--
		pCombo->AddString(STAGE_PGM_TYPE_ITEM[STAGE_PROGRAM_HEAD_TYPE_STANDARD]);
	}
	if(HeadType.bStress){
		pCombo->AddString(STAGE_PGM_TYPE_ITEM[STAGE_PROGRAM_HEAD_TYPE_STRESS]);
	}
	if(HeadType.bSR){
		if(SrConfig.bTransmittance && SrConfig.bGantryType){
			pCombo->AddString(STAGE_PGM_TYPE_ITEM[STAGE_PROGRAM_HEAD_TYPE_GANTRY]);
		}
	}
// 2009.10.13 bagus Distance 追加 --{--
	if(HeadType.bSR){
		if(SrConfig.bDistance){
			pCombo->AddString(STAGE_PGM_TYPE_ITEM[STAGE_PROGRAM_HEAD_TYPE_DISTANCE]);
		}
	}
// 2009.10.13 bagus Distance 追加 --}--
#endif
// 2009.10.09 bagus StagePGM 共通化 --}--

	if(pCombo->GetCount() <= 0){
		LoadStringML(IDS_SAMPLE_SETTING, strBuffer, "Please Sample Setting");
		LoadStringML(IDS_SELECT_SAMPLE_TYPE, strTitle, "SELECT SAMPLE TYPE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	pCombo->SetCurSel(0);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSelectStageProgramTypeDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_HEAD_TYPE);
	CString	strBuffer;
	int i;

	if(pCombo->GetCurSel() == CB_ERR){
		return;
	}

	GetDlgItemText(IDC_HEAD_TYPE, strBuffer);

// 2009.10.09 bagus StagePGM 共通化 --{--
#if 0
	m_iHeadType = HEAD_TYPE_MAX;

	for(i = 0; i < HEAD_TYPE_MAX; i++){
		if(strcmp(strBuffer.GetBuffer(0), HEAD_TYPE_ITEM[i]) == 0){
			m_iHeadType = i;
			break;
		}
	}
#else
	int	iStagePGM;

	iStagePGM = STAGE_PGM_TYPE_STANDARD;


	//2009.12.07 bagus 修正 --{--
	SYSTEM_CONFIG SystemConfig;
	memset(&SystemConfig, 0, sizeof(SystemConfig));
	ConfigFile_GetNanoSpecIni(&SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	const LPCSTR *STAGE_PGM_TYPE_ITEM;
	if(SystemConfig.nLanguage == 0){
		STAGE_PGM_TYPE_ITEM = &STAGE_PGM_TYPE_ITEM_ENU[0];
	}else{
		STAGE_PGM_TYPE_ITEM = STAGE_PGM_TYPE_ITEM_JPN;
	}
	//2009.12.07 bagus 修正 --}--

	for(i = 0; i < STAGE_PGM_TYPE_MAX; i++){
		if(strcmp(strBuffer.GetBuffer(0), STAGE_PGM_TYPE_ITEM[i]) == 0){
			iStagePGM = i;
			break;
		}
	}

	switch ( iStagePGM ) {
	case STAGE_PROGRAM_HEAD_TYPE_STANDARD:
		m_iHeadType = STAGE_PGM_TYPE_STANDARD;
		m_iScanType = SCAN_TYPE_STANDARD;
		break;
	case STAGE_PROGRAM_HEAD_TYPE_STRESS:
		m_iHeadType = STAGE_PGM_TYPE_STRESS;
		m_iScanType = SCAN_TYPE_STRESS;
		break;
	case STAGE_PROGRAM_HEAD_TYPE_GANTRY:
		m_iHeadType = STAGE_PGM_TYPE_SR_GANTRY;
		m_iScanType = SCAN_TYPE_SR_GANTRY;
		break;
	case STAGE_PROGRAM_HEAD_TYPE_DISTANCE:
		m_iHeadType = STAGE_PGM_TYPE_SR_DISTANCE;
		m_iScanType = SCAN_TYPE_SR_DISTANCE;
		break;
	default:
		m_iHeadType = STAGE_PGM_TYPE_STANDARD;
		m_iScanType = SCAN_TYPE_STANDARD;
		break;
	}
#endif
// 2009.10.09 bagus StagePGM 共通化 --}--

	CDialog::OnOK();
}

void CSelectStageProgramTypeDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}
