// SpeedSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "SpeedSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpeedSettingDlg ダイアログ

/////////////////////////////////////////////////////////////////////////////
//
CSpeedSettingDlg::CSpeedSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpeedSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpeedSettingDlg)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CSpeedSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpeedSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_CustomSpeedDataText(pDX, IDC_LOW_X, m_StageMotionSpeedData[X].Speed[STAGE_MOTION_LOW]);
	DDV_CustomXYSpeedDataCheck(pDX, m_StageMotionSpeedData[X].Speed[STAGE_MOTION_LOW]);
	DDX_CustomSpeedDataText(pDX, IDC_LOW_Y, m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_LOW]);
	DDV_CustomXYSpeedDataCheck(pDX, m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_LOW]);
	DDX_CustomSpeedDataText(pDX, IDC_LOW_Z, m_StageMotionSpeedData[Z].Speed[STAGE_MOTION_LOW]);
	DDV_CustomZSpeedDataCheck(pDX, m_StageMotionSpeedData[Z].Speed[STAGE_MOTION_LOW]);
	DDX_CustomSpeedDataText(pDX, IDC_MIDDLE_X, m_StageMotionSpeedData[X].Speed[STAGE_MOTION_MID]);
	DDV_CustomXYSpeedDataCheck(pDX, m_StageMotionSpeedData[X].Speed[STAGE_MOTION_MID]);
	DDX_CustomSpeedDataText(pDX, IDC_MIDDLE_Y, m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_MID]);
	DDV_CustomXYSpeedDataCheck(pDX, m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_MID]);
	DDX_CustomSpeedDataText(pDX, IDC_MIDDLE_Z, m_StageMotionSpeedData[Z].Speed[STAGE_MOTION_MID]);
	DDV_CustomZSpeedDataCheck(pDX, m_StageMotionSpeedData[Z].Speed[STAGE_MOTION_MID]);
	DDX_CustomSpeedDataText(pDX, IDC_HIGH_X, m_StageMotionSpeedData[X].Speed[STAGE_MOTION_HIGH]);
	DDV_CustomXYSpeedDataCheck(pDX, m_StageMotionSpeedData[X].Speed[STAGE_MOTION_HIGH]);
	DDX_CustomSpeedDataText(pDX, IDC_HIGH_Y, m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_HIGH]);
	DDV_CustomXYSpeedDataCheck(pDX, m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_HIGH]);
	DDX_CustomSpeedDataText(pDX, IDC_HIGH_Z, m_StageMotionSpeedData[Z].Speed[STAGE_MOTION_HIGH]);
	DDV_CustomZSpeedDataCheck(pDX, m_StageMotionSpeedData[Z].Speed[STAGE_MOTION_HIGH]);
	DDX_CustomSpeedDataText(pDX, IDC_JOG_X, m_StageMotionSpeedData[X].Jog);
	DDV_CustomJogDataCheck(pDX, m_StageMotionSpeedData[X].Jog);
	DDX_CustomSpeedDataText(pDX, IDC_JOG_Y, m_StageMotionSpeedData[Y].Jog);
	DDV_CustomJogDataCheck(pDX, m_StageMotionSpeedData[Y].Jog);
	DDX_CustomSpeedDataText(pDX, IDC_JOG_Z, m_StageMotionSpeedData[Z].Jog);
	DDV_CustomJogDataCheck(pDX, m_StageMotionSpeedData[Z].Jog);
}

/////////////////////////////////////////////////////////////////////////////
//
void CSpeedSettingDlg::DDX_CustomSpeedDataText(CDataExchange* pDX, int nIDC, int& value)
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
void CSpeedSettingDlg::DDV_CustomXYSpeedDataCheck(CDataExchange* pDX, int& value)
{
	// for the future
}

/////////////////////////////////////////////////////////////////////////////
//
void CSpeedSettingDlg::DDV_CustomZSpeedDataCheck(CDataExchange* pDX, int& value)
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
void CSpeedSettingDlg::DDV_CustomJogDataCheck(CDataExchange* pDX, int& value)
{
	// for the future
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CSpeedSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSpeedSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeedSettingDlg メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSpeedSettingDlg::OnInitDialog()
{
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Get Data /////
	ConfigFile_GetNanoSpecIni(&m_StageMotionSpeedData[0], CONFIG_FILE_STAGE_MOTION_SPEED);

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
void CSpeedSettingDlg::OnOK()
{
	if ( !UpdateData() )
		return;

	SetOperationLog("OK Button was push.");

	ConfigFile_SetNanoSpecIni(&m_StageMotionSpeedData[0], CONFIG_FILE_STAGE_MOTION_SPEED);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
void CSpeedSettingDlg::OnCancel()
{
	SetOperationLog("CANCEL Button was push.");

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//
void CSpeedSettingDlg::SetOperationLog(LPCTSTR pszLog)
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
