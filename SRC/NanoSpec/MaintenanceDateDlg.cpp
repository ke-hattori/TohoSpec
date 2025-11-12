// MaintenanceDateDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "MainFrm.h"
#include "NanoSpecDoc.h"
#include "MaintenanceDateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDateDlg ダイアログ


CMaintenanceDateDlg::CMaintenanceDateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaintenanceDateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaintenanceDateDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CMaintenanceDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaintenanceDateDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	DDX_Control(pDX, IDC_MAINTENANCE_DAYS_COMBO, m_DaysComboBox);
	DDX_Control(pDX, IDC_ALARM_MAINTE_DATE_COMBO, m_AlarmMainteComboBox);
	DDX_Control(pDX, IDC_MAINTENANCE_LIFETIME_COMBO, m_LifeTimeComboBox);
	DDX_Control(pDX, IDC_ALARM_LAMP_MAINTE_DATE_COMBO, m_AlarmLampComboBox);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MAINTENANCE_DATA_SET_BUTTON, m_MainteDataSetButton);
	DDX_Control(pDX, IDC_LAMP_MAINTENANCE_DATA_SET_BUTTON, m_LampMainteDataSetButton);
	DDX_Control(pDX, IDC_CLOSE, m_CloseButton);
}


BEGIN_MESSAGE_MAP(CMaintenanceDateDlg, CDialog)
	//{{AFX_MSG_MAP(CMaintenanceDateDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MAINTENANCE_DATA_SET_BUTTON, OnMaintenanceDataSetButton)
	ON_BN_CLICKED(IDC_LAMP_MAINTENANCE_DATA_SET_BUTTON, OnLampMaintenanceDataSetButton)
	ON_BN_CLICKED(IDC_CLOSE, OnCloseButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDateDlg メッセージ ハンドラ

BOOL CMaintenanceDateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
	int index;
	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Load File /////
//	ConfigFile_GetNanoSpecIni(&m_pMaintenanceDate, CONFIG_FILE_MAINTENANCE_DATE);
	m_pMaintenanceDate = m_pDoc->GetMaintenanceDate();

	///// Maintenance Date /////
	SetDlgItemText(IDC_EDIT_MAINTE_DATE, m_pMaintenanceDate.MaintenanceDate);
	SetDlgItemText(IDC_EDIT_MAINTE_DATE_NEXT, m_pMaintenanceDate.NextMaintenanceDate);

	m_DaysComboBox.ResetContent();
	// Kojika 20090528 Change
	//PCSTR pszYearString[] = MAINTENANCE_DAYS_STR;
	//for( index = 0; index < DAYS_MAX; index++ ){
	//	  m_DaysComboBox.AddString(pszYearString[index]);
	//}
	if(l_SystemConfig.nLanguage == 0){
		PCSTR pszYearString[] = MAINTENANCE_DAYS_STR_ENU;
		for( index = 0; index < DAYS_MAX; index++ ){
			m_DaysComboBox.AddString(pszYearString[index]);
		}
	}
	else{
		PCSTR pszYearString[] = MAINTENANCE_DAYS_STR_JPN;
		for( index = 0; index < DAYS_MAX; index++ ){
			m_DaysComboBox.AddString(pszYearString[index]);
		}
	}
	// Kojika 20090528 Change End
	m_DaysComboBox.SetCurSel(m_pMaintenanceDate.nDaysSpan);

	m_AlarmMainteComboBox.ResetContent();
	// Kojika 20090529 Add
	CString l_strBuffer;
	// Kojika 20090529 Change
	//m_AlarmMainteComboBox.AddString("NO USE");
	//m_AlarmMainteComboBox.AddString("USE");
	//m_AlarmMainteComboBox.AddString("発報しない");
	LoadStringML(IDS_ALARM_NO_USE, l_strBuffer, "NO USE");
	m_AlarmMainteComboBox.AddString(l_strBuffer);
	//m_AlarmMainteComboBox.AddString("発報する");
	LoadStringML(IDS_ALARM_USE, l_strBuffer, "USE");
	m_AlarmMainteComboBox.AddString(l_strBuffer);
	// Kojika 20090529 Change End
	m_AlarmMainteComboBox.SetCurSel(m_pMaintenanceDate.nAlarmFlag);

	///// Load File /////
//	ConfigFile_GetNanoSpecIni(&m_pMaintenanceLamp, CONFIG_FILE_MAINTENANCE_LAMP);
	m_pMaintenanceLamp = m_pDoc->GetMaintenanceLamp();

	///// SR Reflectance Lamp /////
	SetDlgItemText(IDC_EDIT_LAMP_MAINTE_DATE, m_pMaintenanceLamp.MaintenanceDate);
	m_pPassageHour.m_span = m_pMaintenanceLamp.MaintenanceHour;
	char	csBuff[20];
	MakeTotalTimeString( m_pPassageHour, csBuff );
	SetDlgItemText(IDC_EDIT_LAMP_MAINTE_DATE_NEXT, csBuff);

	m_LifeTimeComboBox.ResetContent();
	// Kojika 20090528 Change
	//PCSTR pszLifeTimeString[] = MAINTENANCE_LIFETIME_STR;
	//for( index = 0; index < HOURS_MAX; index++ ){
	//	  m_LifeTimeComboBox.AddString(pszLifeTimeString[index]);
	//}

	if(l_SystemConfig.nLanguage == 0){
		PCSTR pszLifeTimeString[] = MAINTENANCE_LIFETIME_STR_ENU;
		for( index = 0; index < HOURS_MAX; index++ ){
			m_LifeTimeComboBox.AddString(pszLifeTimeString[index]);
		}
	}
	else{
		PCSTR pszLifeTimeString[] = MAINTENANCE_LIFETIME_STR_JPN;
		for( index = 0; index < HOURS_MAX; index++ ){
			m_LifeTimeComboBox.AddString(pszLifeTimeString[index]);
		}
	}
	// Kojika 20090528 Change End
	m_LifeTimeComboBox.SetCurSel(m_pMaintenanceLamp.nHourSpan);

	m_AlarmLampComboBox.ResetContent();
	// Kojika 20090529 Change
	//m_AlarmLampComboBox.AddString("NO USE");
	//m_AlarmLampComboBox.AddString("USE");
	//m_AlarmLampComboBox.AddString("発報しない");
	LoadStringML(IDS_ALARM_NO_USE, l_strBuffer, "NO USE");
	m_AlarmLampComboBox.AddString(l_strBuffer);
	//m_AlarmLampComboBox.AddString("発報する");
	LoadStringML(IDS_ALARM_USE, l_strBuffer, "USE");
	m_AlarmLampComboBox.AddString(l_strBuffer);
	// Kojika 20090529 Change End

	m_AlarmLampComboBox.SetCurSel(m_pMaintenanceLamp.nAlarmFlag);


	/// 設定 Button (メンテナンス) ///
	m_MainteDataSetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MainteDataSetButton.DrawFlatFocus(TRUE);

	/// 設定 Button (SR反射用ランプ) ///
	m_LampMainteDataSetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_LampMainteDataSetButton.DrawFlatFocus(TRUE);

	/// 閉じる Button ///
	m_CloseButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CloseButton.DrawFlatFocus(TRUE);


	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ==========================================================================
//
void CMaintenanceDateDlg::OnTimer(UINT nIDEvent)
{
	char	csBuff[20];

//	m_pPassageHour.m_span = m_pDoc->GetPassageHour();
	m_pMaintenanceLamp = m_pDoc->GetMaintenanceLamp();
	m_pPassageHour.m_span = m_pMaintenanceLamp.MaintenanceHour;

	MakeTotalTimeString( m_pPassageHour, csBuff );

	SetDlgItemText(IDC_EDIT_LAMP_MAINTE_DATE_NEXT ,csBuff);

	CDialog::OnTimer(nIDEvent);
}

// ==========================================================================
//
void CMaintenanceDateDlg::MakeTotalTimeString( COleDateTimeSpan pstime, char* csBuff )
{
	char	strtmp[20];

	double	h=pstime.GetTotalHours();
	long	m=pstime.GetMinutes();
	long	s=pstime.GetSeconds();

	memset(csBuff,0,sizeof(csBuff));
	sprintf(csBuff,"%04d:",(long)h);
	memset(strtmp,0,sizeof(strtmp));
	sprintf(strtmp,"%02d:",m);
	strcat(csBuff,strtmp);
	memset(strtmp,0,sizeof(strtmp));
	sprintf(strtmp,"%02d",s);
	strcat(csBuff,strtmp);
}

void CMaintenanceDateDlg::OnMaintenanceDataSetButton()
{
// Kojika 20090529 Add
	CString strBuffer, strTitle;
// Kojika 20090529 Add End

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString csBuff;
	COleDateTime mDate;
	COleDateTime mNewDate;

	GetDlgItemText(IDC_EDIT_MAINTE_DATE, csBuff);
	if( mDate.ParseDateTime(csBuff, LOCALE_NOUSEROVERRIDE) == 0 ){
// Kojika 20090529 Change
//		MessageBox("正しい日付を設定して下さい.", "MAINTENANCE DATE", MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_SET_CORRECT_DATE, strBuffer, "Please set a correct date");
		LoadStringML(IDS_TITLE_MAINTENANCE_DATE, strTitle, "MAINTENANCE DATE");
		MessageBox(strBuffer, strTitle, MB_OK|MB_ICONWARNING);
// Kojika 20090529 Change End
		return;
	}

	SetOperationLog("Proofreading SET Button was push.");

	SetDlgItemText(IDC_EDIT_MAINTE_DATE ,mDate.Format("%Y/%m/%d %H:%M:%S"));
	mNewDate = mDate;

	int selDays = m_DaysComboBox.GetCurSel();

	COleDateTimeSpan mDays;
	switch(selDays){
	case DAYS_90:
		mDays.m_span = 90;
		break;
	case DAYS_180:
		mDays.m_span = 180;
		break;
	case DAYS_270:
		mDays.m_span = 270;
		break;
	case DAYS_365:
		mDays.m_span = 365;
		break;
	}
	mNewDate.operator +=(mDays);

	SetDlgItemText(IDC_EDIT_MAINTE_DATE_NEXT ,mNewDate.Format("%Y/%m/%d %H:%M:%S"));

	// データを取り込む
	GetDlgItemText(IDC_EDIT_MAINTE_DATE, csBuff);
	sprintf(m_pMaintenanceDate.MaintenanceDate, "%s", (LPCTSTR)csBuff);
	GetDlgItemText(IDC_EDIT_MAINTE_DATE_NEXT, csBuff);
	sprintf(m_pMaintenanceDate.NextMaintenanceDate, "%s", (LPCTSTR)csBuff);
	m_pMaintenanceDate.nDaysSpan = selDays;
	m_pMaintenanceDate.nAlarmFlag = m_AlarmMainteComboBox.GetCurSel();

	m_pDoc->SetMaintenanceDate( m_pMaintenanceDate );

	ConfigFile_SetNanoSpecIni(&m_pMaintenanceDate, CONFIG_FILE_MAINTENANCE_DATE);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

}

void CMaintenanceDateDlg::OnLampMaintenanceDataSetButton()
{
// Kojika 20090529 Add
	CString strBuffer, strTitle;
// Kojika 20090529 Add End

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString csBuff;
	COleDateTime mDate;

	GetDlgItemText(IDC_EDIT_LAMP_MAINTE_DATE, csBuff);
	if( mDate.ParseDateTime(csBuff, LOCALE_NOUSEROVERRIDE) == 0 ){
// Kojika 20090529 Change
//		MessageBox("正しい日付を設定して下さい.", "MAINTENANCE DATE", MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_SET_CORRECT_DATE, strBuffer, "Please set a correct date");
		LoadStringML(IDS_TITLE_MAINTENANCE_DATE, strTitle, "MAINTENANCE DATE");
		MessageBox(strBuffer, strTitle, MB_OK|MB_ICONWARNING);
// Kojika 20090529 Change End
		return;
	}

	SetOperationLog("SR Reflectance Lamp SET Button was push.");

	SetDlgItemText(IDC_EDIT_LAMP_MAINTE_DATE ,mDate.Format("%Y/%m/%d %H:%M:%S"));

	// データを取り込む
	GetDlgItemText(IDC_EDIT_LAMP_MAINTE_DATE, csBuff);
	sprintf(m_pMaintenanceLamp.MaintenanceDate, "%s", csBuff);
	GetDlgItemText(IDC_EDIT_LAMP_MAINTE_DATE_NEXT, csBuff);
	m_pPassageHour.m_span = 0;			// カウントをクリアする
	m_pDoc->SetPassageHour(m_pPassageHour.m_span);
	m_pMaintenanceLamp.MaintenanceHour = m_pPassageHour.m_span;
	m_pMaintenanceLamp.nHourSpan = m_LifeTimeComboBox.GetCurSel();
	m_pMaintenanceLamp.nAlarmFlag = m_AlarmLampComboBox.GetCurSel();

	m_pDoc->SetMaintenanceLamp( m_pMaintenanceLamp );

	ConfigFile_SetNanoSpecIni(&m_pMaintenanceLamp, CONFIG_FILE_MAINTENANCE_LAMP);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

}

// =========================================================================
//
void CMaintenanceDateDlg::OnCloseButton()
{
	SetOperationLog("CLOSE Button was pushed.");
	EndDialog(IDOK);
}

// =========================================================================
//
void CMaintenanceDateDlg::SetOperationLog(LPCTSTR pszLog)
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
