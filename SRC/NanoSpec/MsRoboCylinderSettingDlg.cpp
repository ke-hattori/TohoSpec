// MsRoboCylinderSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MsRoboCylinderSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static const LPSTR LENS_TYPE_SENSOR[] = {
	_T("OFF"),
	_T("ON"),
} ;


/////////////////////////////////////////////////////////////////////////////
// CMsRoboCylinderSettingDlg ダイアログ


CMsRoboCylinderSettingDlg::CMsRoboCylinderSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsRoboCylinderSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsRoboCylinderSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_iCurrNo = 0;
	m_strName = "";
}


void CMsRoboCylinderSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsRoboCylinderSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MS_ROBO_CYLINDER_NAME_LEN);
}


BEGIN_MESSAGE_MAP(CMsRoboCylinderSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CMsRoboCylinderSettingDlg)
	ON_CBN_SELCHANGE(IDC_INDEX, OnSelchangeIndex)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, OnCheckEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsRoboCylinderSettingDlg メッセージ ハンドラ

BOOL CMsRoboCylinderSettingDlg::OnInitDialog()
{
	memset(&m_MsConfig, 0, sizeof(m_MsConfig));
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);

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

	///// Index ComboBox Initialize /////
	pCombo = (CComboBox *)GetDlgItem(IDC_INDEX);
	pCombo->ResetContent();
	for(i = 0; i < MAX_MS_ROBO_CYLINDER_NUM; i++){
		_stprintf(szTemp, "%d", i + 1);
		pCombo->AddString(szTemp);
	}
	pCombo->SetCurSel(0);
	m_iCurrNo = 0;

	///// Position ComboBox Initialize /////
	pCombo = (CComboBox *)GetDlgItem(IDC_POSITION_NO);
	pCombo->ResetContent();
	for(i = 0; i < MAX_MS_ROBO_CYLINDER_POSITION_NUM; i++){
		_stprintf(szTemp, "%d", i );
		pCombo->AddString(szTemp);
	}
	pCombo->SetCurSel(0);

	///// Sensor ComboBox Initialize /////
	pCombo = (CComboBox *)GetDlgItem(IDC_LENS_TYPE_SENSOR);
	pCombo->ResetContent();
	for(i = 0; i < sizeof(LENS_TYPE_SENSOR) / sizeof(LENS_TYPE_SENSOR[0]); i++){
		pCombo->AddString(LENS_TYPE_SENSOR[i]);
	}
	pCombo->SetCurSel(0);

	UpDate(FALSE);
	OnCheckEnable();

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CMsRoboCylinderSettingDlg::OnOK()
{
	///// Save /////
	UpDate(TRUE);

	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_MS_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CMsRoboCylinderSettingDlg::OnSelchangeIndex()
{
	CComboBox* pCombo;
	pCombo = (CComboBox *)GetDlgItem(IDC_INDEX);
	// 現在の設定を保存
	UpDate(TRUE);
	if(!CheckData()){
		pCombo->SetCurSel(m_iCurrNo);
		return;
	}

	// 新しい番号のターレットの設定を表示
	m_iCurrNo = pCombo->GetCurSel();
	UpDate(FALSE);
	OnCheckEnable();
}

// =========================================================================
//
void CMsRoboCylinderSettingDlg::OnCheckEnable()
{
	BOOL bEnable = IsDlgButtonChecked(IDC_CHECK_ENABLE) == BST_CHECKED;

	GetDlgItem(IDC_NAME)->EnableWindow(bEnable);
	GetDlgItem(IDC_POSITION_NO)->EnableWindow(bEnable);
	GetDlgItem(IDC_LENS_TYPE_SENSOR)->EnableWindow(bEnable);
}

// =========================================================================
//
void CMsRoboCylinderSettingDlg::UpDate(BOOL bValid)
{
	CString strBuffer;

	CComboBox* pComboPos = (CComboBox *)GetDlgItem(IDC_POSITION_NO);
	CComboBox* pComboSensor = (CComboBox *)GetDlgItem(IDC_LENS_TYPE_SENSOR);
	int		nIndex;
	UINT	uCheck;

	if ( bValid ) {
		///// Enable /////
		m_MsConfig.RoboCylinder[m_iCurrNo].bEnable = IsDlgButtonChecked(IDC_CHECK_ENABLE) == BST_CHECKED;

		///// Name /////
		GetDlgItemText(IDC_NAME, strBuffer);
		_stprintf(m_MsConfig.RoboCylinder[m_iCurrNo].szName, "%s", (PCSTR) strBuffer);

		///// Position No. /////
		nIndex = pComboPos->GetCurSel() ;
		m_MsConfig.RoboCylinder[m_iCurrNo].nPosition = nIndex;

		///// Lens Type Sensor /////
		nIndex = pComboSensor->GetCurSel();
		if (nIndex == 0) {
			m_MsConfig.RoboCylinder[m_iCurrNo].bSensor = FALSE;
		} else {
			m_MsConfig.RoboCylinder[m_iCurrNo].bSensor = TRUE;
		}
	}
	else {
		///// Enable /////
		uCheck = m_MsConfig.RoboCylinder[m_iCurrNo].bEnable ? BST_CHECKED : BST_UNCHECKED;
		CheckDlgButton(IDC_CHECK_ENABLE, uCheck);

		///// Name /////
		strBuffer = m_MsConfig.RoboCylinder[m_iCurrNo].szName;
		SetDlgItemText(IDC_NAME, strBuffer);

		///// Position No. /////
		if ((m_MsConfig.RoboCylinder[m_iCurrNo].nPosition <= 0) || (m_MsConfig.RoboCylinder[m_iCurrNo].nPosition > MAX_MS_ROBO_CYLINDER_POSITION_NUM)) {
			//NG
			pComboPos->SetCurSel(0);
		} else {
			pComboPos->SetCurSel(m_MsConfig.RoboCylinder[m_iCurrNo].nPosition);
		}

		///// Lens Type Sensor /////
		if (m_MsConfig.RoboCylinder[m_iCurrNo].bSensor) {
			pComboSensor->SetCurSel(1);
		} else {
			pComboSensor->SetCurSel(0);
		}
	}
}

// =========================================================================
//
BOOL CMsRoboCylinderSettingDlg::CheckData()
{
	CString strBuffer;

	CString l_strBuffer, l_strTitle;

	CComboBox* pComboPos = (CComboBox *)GetDlgItem(IDC_POSITION_NO);
	CComboBox* pComboSensor = (CComboBox *)GetDlgItem(IDC_LENS_TYPE_SENSOR);

	if ( !m_MsConfig.RoboCylinder[m_iCurrNo].bEnable ) {
		return TRUE;
	}

	// Name
	if ( _tcslen(m_MsConfig.RoboCylinder[m_iCurrNo].szName) == 0 ) {
		LoadStringML(IDS_ENTER_NAME, l_strBuffer, "Please enter a name");
		LoadStringML(IDS_TITLE_ROBO_CYLINDER_SETTING, l_strTitle, "Lens Name & Position Setting");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_NAME)->SetFocus();
		return FALSE;
	}

	// Position No.
	if (pComboPos->GetCurSel() < 0) {
		pComboPos->SetFocus();
		LoadStringML(IDS_SELECT_ROBO_CYLINDER_POSITION, strBuffer, "Please select position no.");
		AfxMessageBox(strBuffer);
		return FALSE;
	}

	// Lens Type Sensor
	if (pComboSensor->GetCurSel() < 0) {
		pComboSensor->SetFocus();
		LoadStringML(IDS_SELECT_ROBO_CYLINDER_SENSOR, strBuffer, "Please select lens typee sensor");
		AfxMessageBox(strBuffer);
		return FALSE;
	}

	return TRUE;
}
