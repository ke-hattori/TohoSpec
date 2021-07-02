// ComSetupDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "nanospec.h"
#include "ComSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComSetupDlg ダイアログ


CComSetupDlg::CComSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSetupDlg)
	//}}AFX_DATA_INIT
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	LoadStringML(IDS_TITLE_NANOSPEC, m_strMsgBoxCaption,"NanoSpec");
// 2013.11.07 Bagus Add (TohoSpec対応) <--
}

CComSetupDlg::~CComSetupDlg()
{
}

void CComSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMSETUP_GRID_CTRL, m_ComGridCtrl);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);

	//{{AFX_DATA_MAP(CComSetupDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CComSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CComSetupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComSetupDlg メッセージ ハンドラ

//---------------------------------------------------------------------------
// OnInitDialog
BOOL CComSetupDlg::OnInitDialog()
{
	int iRow;

	CDialog::OnInitDialog();

	///// Get Data /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	ConfigFile_GetNanoSpecIni(&m_ComSetting, CONFIG_FILE_COM);
	m_OldComSetting = m_ComSetting;

	const int iHeadComMax = 4;
	m_ComGridCtrl.GridInit(iHeadComMax);

	iRow = 1;
	// SR
	if ( m_SystemConfig.HeadType.bSR) {
		UpdateComPortData(iRow++, "SR", &m_ComSetting.SrComPort, FALSE);
	}
	// RS
	if ( m_SystemConfig.HeadType.bResist) {
		UpdateComPortData(iRow++, "RS", &m_ComSetting.RsComPort, FALSE);
	}
	// CA
	if ( m_SystemConfig.HeadType.bCTA ) {
		UpdateComPortData(iRow++, "CA", &m_ComSetting.CaComPort, FALSE);
	}
	// Stress
	if ( m_SystemConfig.HeadType.bStress ) {
		UpdateComPortData(iRow++, "Stress", &m_ComSetting.StressComPort, FALSE);
	}

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;
}

//---------------------------------------------------------------------------
// OnOK
void CComSetupDlg::OnOK()
{
	int iRow;

	iRow = 1;
	// SR
	if ( m_SystemConfig.HeadType.bSR) {
		if ( !UpdateComPortData(iRow++, NULL, &m_ComSetting.SrComPort) )
			return;
	}
	// RS
	if ( m_SystemConfig.HeadType.bResist) {
		if ( !UpdateComPortData(iRow++, NULL, &m_ComSetting.RsComPort) )
			return;
	}
	// CA
	if ( m_SystemConfig.HeadType.bCTA ) {
		if ( !UpdateComPortData(iRow++, NULL, &m_ComSetting.CaComPort) )
			return;
	}
	// Stress
	if ( m_SystemConfig.HeadType.bStress ) {
		if ( !UpdateComPortData(iRow++, NULL, &m_ComSetting.StressComPort) )
			return;
	}

	if ( memcmp(&m_OldComSetting, &m_ComSetting, sizeof(COM_SETTING)) != 0 ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//		int iRtn = MessageBox("OverWrite?", "NanoSpec", MB_YESNOCANCEL | MB_ICONINFORMATION);
		int iRtn = MessageBox("OverWrite?", m_strMsgBoxCaption, MB_YESNOCANCEL | MB_ICONINFORMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		switch ( iRtn ) {
		case IDYES:
			ConfigFile_SetNanoSpecIni(&m_ComSetting, CONFIG_FILE_COM);
			ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);
			break;
		case IDNO:
			break;
		case IDCANCEL:
			return;
			break;
		}
	}

	CDialog::OnOK();
}

//---------------------------------------------------------------------------
// UpdateComPortData
BOOL CComSetupDlg::UpdateComPortData(int iRow, LPCTSTR pszHeadName, COM_PORT* pComPort, BOOL bSaveAndValidate/* = TRUE*/)
{
	int iCol;
	CString strBuffer;

	if ( !bSaveAndValidate ) {
		iCol = 0;
		m_ComGridCtrl.GridDisp(iRow, iCol++, pszHeadName, DT_LEFT);
		m_ComGridCtrl.GridDisp(iRow, iCol++, pComPort->szCommNo, DT_LEFT);
		m_ComGridCtrl.GridDisp(iRow, iCol++, pComPort->szBaudRate, DT_LEFT);
		m_ComGridCtrl.GridDisp(iRow, iCol++, pComPort->szParity, DT_LEFT);
		m_ComGridCtrl.GridDisp(iRow, iCol++, pComPort->szByteSize, DT_LEFT);
		m_ComGridCtrl.GridDisp(iRow, iCol++, pComPort->szStopBits, DT_LEFT);
	}
	else {
		iCol = 1;
		// ComNo
		strBuffer = m_ComGridCtrl.GetItemText(iRow, iCol);
		if ( strBuffer.GetLength() > 5 ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Input Text Too Long.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Input Text Too Long.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		if ( strBuffer.Mid(0, 3) != "COM" ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Invalid Text.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Invalid Text.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		int iComNo = atoi(strBuffer.Mid(3));
		if ( iComNo < 1 ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Invalid Com No.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Invalid Com No.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		strcpy(pComPort->szCommNo, m_ComGridCtrl.GetItemText(iRow, iCol++));

		// BaudRate
		strBuffer = m_ComGridCtrl.GetItemText(iRow, iCol);
		if ( strBuffer.GetLength() > 6 ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Input Text Too Long.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Input Text Too Long.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		strcpy(pComPort->szBaudRate, m_ComGridCtrl.GetItemText(iRow, iCol++));

		// Parity
		strBuffer = m_ComGridCtrl.GetItemText(iRow, iCol);
		if ( strBuffer.GetLength() > 1 ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Input Text Too Long.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Input Text Too Long.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		strcpy(pComPort->szParity, m_ComGridCtrl.GetItemText(iRow, iCol++));

		// ByteSize
		strBuffer = m_ComGridCtrl.GetItemText(iRow, iCol);
		if ( strBuffer.GetLength() > 1 ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Input Text Too Long.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Input Text Too Long.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		int iByteSize = atoi(strBuffer);
		if ( iByteSize < 5 || 8 < iByteSize ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Invalid ByteSize.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Invalid ByteSize.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		strcpy(pComPort->szByteSize, m_ComGridCtrl.GetItemText(iRow, iCol++));

		// StopBits
		strBuffer = m_ComGridCtrl.GetItemText(iRow, iCol);
		if ( strBuffer.GetLength() > 3 ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Input Text Too Long.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Input Text Too Long.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		if ( strBuffer != "1" && strBuffer != "1.5" && strBuffer != "2" ) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//			MessageBox("Invalid StopBits.", "NanoSpec", MB_OK | MB_ICONEXCLAMATION);
			MessageBox("Invalid StopBits.", m_strMsgBoxCaption, MB_OK | MB_ICONEXCLAMATION);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			m_ComGridCtrl.EnableSelection();
			m_ComGridCtrl.SetItemState(iRow, iCol, m_ComGridCtrl.GetItemState(iRow, iCol) | GVIS_SELECTED);
			m_ComGridCtrl.SetFocusCell(iRow, iCol);
			return FALSE;
		}
		strcpy(pComPort->szStopBits, m_ComGridCtrl.GetItemText(iRow, iCol++));
	}

	return TRUE;
}
