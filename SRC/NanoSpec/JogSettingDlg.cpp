// JogSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "JogSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJogSettingDlg ダイアログ

/////////////////////////////////////////////////////////////////////////////
//
CJogSettingDlg::CJogSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJogSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJogSettingDlg)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJogSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_CustomJogDataText(pDX, IDC_LOW_X, m_StageMotionJogData[X].Jog[STAGE_MOTION_LOW]);
	DDV_CustomXYJogDataCheck(pDX, m_StageMotionJogData[X].Jog[STAGE_MOTION_LOW]);
	DDX_CustomJogDataText(pDX, IDC_LOW_Y, m_StageMotionJogData[Y].Jog[STAGE_MOTION_LOW]);
	DDV_CustomXYJogDataCheck(pDX, m_StageMotionJogData[Y].Jog[STAGE_MOTION_LOW]);
	DDX_CustomJogDataText(pDX, IDC_LOW_Z, m_StageMotionJogData[Z].Jog[STAGE_MOTION_LOW]);
	DDV_CustomZJogDataCheck(pDX, m_StageMotionJogData[Z].Jog[STAGE_MOTION_LOW]);
	DDX_CustomJogDataText(pDX, IDC_MIDDLE_X, m_StageMotionJogData[X].Jog[STAGE_MOTION_MID]);
	DDV_CustomXYJogDataCheck(pDX, m_StageMotionJogData[X].Jog[STAGE_MOTION_MID]);
	DDX_CustomJogDataText(pDX, IDC_MIDDLE_Y, m_StageMotionJogData[Y].Jog[STAGE_MOTION_MID]);
	DDV_CustomXYJogDataCheck(pDX, m_StageMotionJogData[Y].Jog[STAGE_MOTION_MID]);
	DDX_CustomJogDataText(pDX, IDC_MIDDLE_Z, m_StageMotionJogData[Z].Jog[STAGE_MOTION_MID]);
	DDV_CustomZJogDataCheck(pDX, m_StageMotionJogData[Z].Jog[STAGE_MOTION_MID]);
	DDX_CustomJogDataText(pDX, IDC_HIGH_X, m_StageMotionJogData[X].Jog[STAGE_MOTION_HIGH]);
	DDV_CustomXYJogDataCheck(pDX, m_StageMotionJogData[X].Jog[STAGE_MOTION_HIGH]);
	DDX_CustomJogDataText(pDX, IDC_HIGH_Y, m_StageMotionJogData[Y].Jog[STAGE_MOTION_HIGH]);
	DDV_CustomXYJogDataCheck(pDX, m_StageMotionJogData[Y].Jog[STAGE_MOTION_HIGH]);
	DDX_CustomJogDataText(pDX, IDC_HIGH_Z, m_StageMotionJogData[Z].Jog[STAGE_MOTION_HIGH]);
	DDV_CustomZJogDataCheck(pDX, m_StageMotionJogData[Z].Jog[STAGE_MOTION_HIGH]);
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogSettingDlg::DDX_CustomJogDataText(CDataExchange* pDX, int nIDC, int& value)
{
	TCHAR szBuff[256];

	pDX->PrepareEditCtrl(nIDC);

	if ( pDX->m_bSaveAndValidate ) {
		GetDlgItemText(nIDC, szBuff, sizeof(szBuff));
		value = (int)(atof(szBuff) * MICROMETRE);
	}
	else {
		_stprintf(szBuff, "%.3lf", ((double)value) / MICROMETRE);
		SetDlgItemText(nIDC, szBuff);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogSettingDlg::DDV_CustomXYJogDataCheck(CDataExchange* pDX, int& value)
{
	// for the future
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogSettingDlg::DDV_CustomZJogDataCheck(CDataExchange* pDX, int& value)
{
	const int iMin = 1;
	const int iMax = 10000;
	//Saiki 20090601 Change ----->
	CString strBuffer;
	//Saiki 20090601 Change <-----

	if ( pDX->m_bSaveAndValidate ) {
		if ( value < iMin || iMax < value ) {
			// AfxMessageBox("0.001 から 10.000 までの範囲で入力してください。", MB_ICONEXCLAMATION);
			//Saiki 20090601 Change ----->
			//AfxMessageBox("Input range 0.001 ~ 10.000", MB_ICONEXCLAMATION);
			LoadStringML(IDS_INPUT_RANGE, strBuffer, "Input range 0.001 ~ 10.000");
			AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
			//Saiki 20090601 Change <-----
			pDX->Fail();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CJogSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CJogSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJogSettingDlg メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
BOOL CJogSettingDlg::OnInitDialog()
{
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Get Data /////
	ConfigFile_GetNanoSpecIni(&m_StageMotionJogData[0], CONFIG_FILE_STAGE_MOTION_JOG);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogSettingDlg::OnOK()
{
	if ( !UpdateData() )
		return;

	SetOperationLog("OK Button was push.");

	ConfigFile_SetNanoSpecIni(&m_StageMotionJogData[0], CONFIG_FILE_STAGE_MOTION_JOG);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogSettingDlg::OnCancel()
{
	SetOperationLog("CANCEL Button was push.");

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogSettingDlg::SetOperationLog(LPCTSTR pszLog)
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
