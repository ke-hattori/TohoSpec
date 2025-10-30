// DeleteDataPeriodSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "DeleteDataPeriodSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CDeleteDataPeriodSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CDeleteDataPeriodSettingDlg::CDeleteDataPeriodSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteDataPeriodSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteDataPeriodSettingDlg)
	m_strDeleteDataDate = _T("");
	//}}AFX_DATA_INIT
	m_iDeleteDataPeriod = 0;
}

// =========================================================================
//
void CDeleteDataPeriodSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteDataPeriodSettingDlg)
	DDX_Text(pDX, IDC_DELETE_DATA_DATE, m_strDeleteDataDate);
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_DELETE_DATA_PERIOD, m_iDeleteDataPeriod);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_DATA_DELETE, m_DataDelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CDeleteDataPeriodSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CDeleteDataPeriodSettingDlg)
	ON_CBN_SELCHANGE(IDC_DELETE_DATA_PERIOD, OnSelchangeDeleteDataPeriod)
	ON_BN_CLICKED(IDC_DATA_DELETE, OnDataDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CDeleteDataPeriodSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CDeleteDataPeriodSettingDlg::OnInitDialog()
{
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Load Data /////
	ConfigFile_GetNanoSpecIni(&m_iDeleteDataPeriod, CONFIG_FILE_DELETE_DATA_PERIOD);

	CDialog::OnInitDialog();

	///// Static Control /////
	m_stcDeleteDataDate.SubclassWindow(GetDlgItem(IDC_DELETE_DATA_DATE)->GetSafeHwnd());
	m_stcDeleteDataDate.SetBkColor(REFERENCE_COLOR);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// 手動削除 Button /////
	m_DataDelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DataDelButton.DrawFlatFocus(TRUE);


	///// Control Initialize /////
	InitCombo_DeleteDataPeriod();

	OnSelchangeDeleteDataPeriod();

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CDeleteDataPeriodSettingDlg::OnOK()
{
	///// Save /////
	ConfigFile_SetNanoSpecIni(&m_iDeleteDataPeriod, CONFIG_FILE_DELETE_DATA_PERIOD);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CDeleteDataPeriodSettingDlg::OnSelchangeDeleteDataPeriod()
{
	UpdateData(TRUE);

	// 選択内容から年月取得
	typedef struct _PERIODSEARCH {
		int nSelect;
		int iYear;
		int iMonth;
	} PERIODSEARCH, *PPERIODSEARCH;

	PERIODSEARCH PeriodSearchTable[] =
	{
		{ DELETE_DATA_PERIOD_NONE,				0,	0	},
		{ DELETE_DATA_PERIOD_ONE_MONTH,			0,	1	},
		{ DELETE_DATA_PERIOD_THREE_MONTHS,		0,	3	},
		{ DELETE_DATA_PERIOD_SIX_MONTHS,		0,	6	},
		{ DELETE_DATA_PERIOD_ONE_YEAR,			1,	0	},
		{ DELETE_DATA_PERIOD_ONE_HALF_YEARS,	1,	6	},
		{ DELETE_DATA_PERIOD_TWO_YEARS,			2,	0	},
		{ DELETE_DATA_PERIOD_TWO_HALF_YEARS,	2,	6	},
		{ DELETE_DATA_PERIOD_THREE_YEARS,		3,	0	},
		{ DELETE_DATA_PERIOD_THREE_HALF_YEARS,	3,	6	},
		{ DELETE_DATA_PERIOD_FOUR_YEARS,		4,	0	},
		{ DELETE_DATA_PERIOD_FOUR_HALF_YEARS,	4,	6	},
		{ DELETE_DATA_PERIOD_FIVE_YEARS,		5,	0	},
	};

	int iYear = 0;
	int iMonth = 0;

	for ( int i = 0; i < DELETE_DATA_PERIOD_MAX; i++ ) {
		if ( m_iDeleteDataPeriod == PeriodSearchTable[0].nSelect ) {
			m_strDeleteDataDate = "None";
			goto ExitFunc;
		}

		if ( m_iDeleteDataPeriod == PeriodSearchTable[i].nSelect ) {
			iYear = PeriodSearchTable[i].iYear;
			iMonth = PeriodSearchTable[i].iMonth;
			break;
		}
	}

	// 現在日付からの削除年月
	int iDeleteYear;
	int iDeleteMonth;

	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	if ( (int)systemTime.wMonth > iMonth ) {
		iDeleteYear = (int)systemTime.wYear - iYear;
		iDeleteMonth = (int)systemTime.wMonth - iMonth;
	}
	else {
		iDeleteYear = ((int)systemTime.wYear - 1) - iYear;
		iDeleteMonth = ((int)systemTime.wMonth + 12) - iMonth;
	}


	int iDeleteDay;
	iDeleteDay = systemTime.wDay;
	//2,4,6,9,11月の日付調整
	if((iDeleteMonth == 4)||(iDeleteMonth == 6)||(iDeleteMonth == 9)||(iDeleteMonth == 11))
	//4,6,9,11月
	{
		if(iDeleteDay == 31)
		{
			iDeleteDay = 30;
		}
	}
	else if(iDeleteMonth == 2) //2月
	{
		if(iDeleteDay > 28)
		{
			if((((iDeleteYear%4)==0)&&((iDeleteYear%100)!=0)) //うるう年
			||((iDeleteYear%400)==0))
			{
				iDeleteDay = 29;
			}
			else //非うるう年
			{
				iDeleteDay = 28;
			}
		}
	}
	else //1,3,5,7,8,10,12月
	{
		//処理なし
	}

	m_strDeleteDataDate.Format("%04d/%02d/%2d", iDeleteYear, iDeleteMonth, iDeleteDay);

ExitFunc:
	UpdateData(FALSE);

}

// =========================================================================
//
void CDeleteDataPeriodSettingDlg::InitCombo_DeleteDataPeriod()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DELETE_DATA_PERIOD);
	pCombo->ResetContent();

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	for ( int i = 0; i < DELETE_DATA_PERIOD_MAX; i++ ){
		// Kojika 20090528 Change
		//pCombo->AddString(g_lpszDeleteDataPeriod[i]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszDeleteDataPeriod_ENU[i]);
		}
		else{
			pCombo->AddString(g_lpszDeleteDataPeriod_JPN[i]);
		}
		// Kojika 20090528 Change End
	}

	pCombo->SetCurSel(m_iDeleteDataPeriod);
}


void CDeleteDataPeriodSettingDlg::OnDataDelete()
{
// Kojika 20090526 Add
	CString strBuffer, strTitle;
// Kojika 20090526 Add End
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DELETE_DATA_PERIOD);
	int iCurSel = pCombo->GetCurSel();
	if((iCurSel == 0)||(iCurSel == CB_ERR))
	{
// Kojika 20090526 Change
//		MessageBox("有効年月日の指定がありません。", "NanoSpec", MB_OK);
		LoadStringML(IDS_NO_SPECIFI_EFFECTIVE_DATE, strBuffer, "There is no specification at an effective date.");
		LoadStringML(IDS_TITLE_NANOSPEC, strTitle,"NanoSpec");
		MessageBox(strBuffer, strTitle, MB_OK);
// Kojika 20090526 Change End
		return;
	}
// Kojika 20090526 Change
//	int iRtn = MessageBox("測定データを削除しますか？\n(削除後、復旧できません)", "NanoSpec", MB_OKCANCEL);
	LoadStringML(IDS_MEASURE_DATA_DELETE, strBuffer, "Is the measured data deleted?\n(After deletion, it isn't possible to be restored it)");
	LoadStringML(IDS_TITLE_NANOSPEC, strTitle,"NanoSpec");
	int iRtn = MessageBox(strBuffer, strTitle, MB_OKCANCEL);
// Kojika 20090526 Change End

	if(iRtn == IDOK)
	{
		ConfigFile_SetNanoSpecIni(&m_iDeleteDataPeriod, CONFIG_FILE_DELETE_DATA_PERIOD);
		ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

		m_pMainFrame->OldMeasDataDelete(); //古い測定データファイルを削除
	}

}
