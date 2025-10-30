// RSCofigurationSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "RSCofigurationSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRSCofigurationSettingDlg ダイアログ


CRSCofigurationSettingDlg::CRSCofigurationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRSCofigurationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRSCofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CRSCofigurationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRSCofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CRSCofigurationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CRSCofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSCofigurationSettingDlg メッセージ ハンドラ

BOOL CRSCofigurationSettingDlg::OnInitDialog()
{
	memset(&m_RsConfig, 0, sizeof(m_RsConfig));
	ConfigFile_GetNanoSpecIni(&m_RsConfig, CONFIG_FILE_RS_CONFIG);

	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Probe Type Combo /////
	InitCombo_ProbeType();

	///// Set Data /////
	UpDate(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CRSCofigurationSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);

	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_RsConfig, CONFIG_FILE_RS_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_RS_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CRSCofigurationSettingDlg::UpDate(BOOL bValid)
{
	CString	strBuffer;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_PROBE_TYPE);
	int nIndex = 0;

	if ( bValid ) {
		///// RsStdZAxisRetractPos /////
		GetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);
		m_RsConfig.lRsStdZAxisRetractPos = strtol(strBuffer, NULL, 10) * 10;

		///// Probe Type /////
		int nSelect = pCombo->GetCurSel();
		m_RsConfig.nRsProbeType = pCombo->GetItemData(nSelect);
	} else {
		///// RsStdZAxisRetractPos /////
		strBuffer.Format("%ld", m_RsConfig.lRsStdZAxisRetractPos / 10);
		SetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);

		///// Probe Type /////
		if ((m_RsConfig.nRsProbeType < 0) || (m_RsConfig.nRsProbeType >= PROBE_TYPE_MAX)) {
			nIndex = CB_ERR;
		} else {
			strBuffer = PROBE_TYPE_ITEM[m_RsConfig.nRsProbeType];
			nIndex = pCombo->FindStringExact(-1, strBuffer);
		}
		if(!(nIndex == CB_ERR)){
			pCombo->SetCurSel(nIndex);
		} else {
			pCombo->SetCurSel(0);
		}
	}
}

// =========================================================================
//
BOOL CRSCofigurationSettingDlg::CheckData()
{

	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_PROBE_TYPE);

	CString strBuffer;

	///// RsStdZAxisRetractPos /////
	GetDlgItemText(IDC_Z_AXIS_POSITION, strBuffer);
	if ( CheckMinMaxLong(strBuffer, MIN_ZAXIS_POSITION, MAX_ZAXIS_POSITION) != CHECK_DATA_OK ) {
		GetDlgItem(IDC_Z_AXIS_POSITION)->SetFocus();
		return FALSE;
	}

	///// Probe Type /////
	if (pCombo->GetCurSel() < 0) {
		pCombo->SetFocus();
		LoadStringML(IDS_SELECT_PROBE_TYPE, strBuffer, "Please select probe type");
		AfxMessageBox(strBuffer);
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
//
void CRSCofigurationSettingDlg::InitCombo_ProbeType()
{
	CString strBuffer;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_PROBE_TYPE);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < PROBE_TYPE_MAX; i++ ) {
		strBuffer = PROBE_TYPE_ITEM[i];
		if (strBuffer.GetLength() < 1)
			continue;
		nIndex = pCombo->AddString(strBuffer);
		pCombo->SetItemData(nIndex, i);
	}
}
