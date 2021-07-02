// ZAxisOffsetSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "ZAxisOffsetSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CZAxisOffsetSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CZAxisOffsetSettingDlg::CZAxisOffsetSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZAxisOffsetSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZAxisOffsetSettingDlg)
	m_strLens = _T("");
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CZAxisOffsetSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZAxisOffsetSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_START_BUTTON, m_StartButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Text(pDX, IDC_STAGE_PROGRAM, m_MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_NAME_LEN + 1);
	DDX_CBStringExact(pDX, IDC_LENS, m_strLens);
	DDX_Text(pDX, IDC_COMMENT, m_MainRcpInfo.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CZAxisOffsetSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CZAxisOffsetSettingDlg)
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	ON_CBN_SELCHANGE(IDC_LENS, OnSelchangeLens)
	ON_CBN_SELCHANGE(IDC_DEFAULT_DISPLAY, OnSelchangeDefaultDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CZAxisOffsetSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CZAxisOffsetSettingDlg::OnInitDialog()
{
	///// Load File /////
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	LoadRecipeData();

	//m_strLens = m_SrTurret[m_MainRcpInfo.MainRcpParam._SR.iLens].szName;
	m_strLens = m_SrTurret[m_MeasProgInfo.ScanParams._SR.iLens].szName;

	CDialog::OnInitDialog();

	///// Start Button /////
	m_StartButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_StartButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Control Initialize /////
	InitCombo_StageProgram();
	InitCombo_Lens();
	InitCombo_DefaultDisplay();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CZAxisOffsetSettingDlg::OnStartButton()
{
	///// Save /////
	if(!CheckData()) return;

	if ( !RecipeFile_SaveRecipe(&m_MainRcpInfo, STAGE_LEVEL_CALIBRATION_FILE_NAME, RECIPE_FILE_Z_AXIS_OFFSET) )
		return;

	EndDialog(IDOK);
}

// =========================================================================
// レンズ コンボ選択時
//
void CZAxisOffsetSettingDlg::OnSelchangeLens()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;

	m_MeasProgInfo.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	UpdateData(FALSE);
}

// =========================================================================
// デフォルトディスプレイ コンボ選択時
//
void CZAxisOffsetSettingDlg::OnSelchangeDefaultDisplay()
{
//	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DEFAULT_DISPLAY);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay = nSelect;

//	UpdateData(FALSE);
}

// =========================================================================
//
void CZAxisOffsetSettingDlg::LoadRecipeData()
{
	if ( !RecipeFile_LoadRecipe(&m_MainRcpInfo, STAGE_LEVEL_CALIBRATION_FILE_NAME, RECIPE_FILE_Z_AXIS_OFFSET) ) {
		m_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
		strcpy(m_MainRcpInfo.hdr.szName, STAGE_LEVEL_CALIBRATION_FILE_NAME);
	}
}

// =========================================================================
//
void CZAxisOffsetSettingDlg::InitCombo_StageProgram()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STAGE_PROGRAM);
	pCombo->ResetContent();
//	pCombo->AddString("None");

	CString strDirPath;
	CString strBuffer;
	strDirPath.Format("%s*%s", g_szDb_Stage_Program_Dir, STAGEPGM_EXT);
	CFileFind Finder;
	if ( !Finder.FindFile(strDirPath) )
		return;

	//Saiki 20090410 Change ----->
	//BOOL bLoop;
	BOOL bLoop = TRUE;
	//Saiki 20090410 Change <-----
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		// ファイル名を取得
		strBuffer = Finder.GetFileTitle();
		if ( strBuffer[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		pCombo->AddString(strBuffer);
	}

	int nIndex = pCombo->FindStringExact(-1, m_MainRcpInfo.MainRcpParam.hdr.szStage);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
void CZAxisOffsetSettingDlg::InitCombo_Lens()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
		if ( m_SrTurret[i].bEnable ) {
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	nIndex = pCombo->FindStringExact(-1, m_strLens);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
void CZAxisOffsetSettingDlg::InitCombo_DefaultDisplay()
{
	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_DEFAULT_DISPLAY);
	pCombo->ResetContent();

	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	//Saiki 20090527 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090527 Add <-----
	for ( int i = 0; i < SR_DEFAULT_DISPLAY_MAX; i++ ) {
		//Saiki 20090527 Change ----->
		//pCombo->AddString(g_lpszSrDefaultDisplay[i]);
		//if ( strcmp(g_lpszSrDefaultDisplay[i], "Disable") == 0 )
		//	  continue;

		// Kojika 20090603 Change
		LoadStringML(IDS_DISABLE, l_strBuffer, "Disable");
		if(l_SystemConfig.nLanguage == 0){
			//if ( strcmp(g_lpszSrDefaultDisplay_ENU[i], "Disable") == 0 )
			if ( strcmp(g_lpszSrDefaultDisplay_ENU[i], l_strBuffer) == 0 )
			continue;
			pCombo->AddString(g_lpszSrDefaultDisplay_ENU[i]);
		}
		else{
			//if ( strcmp(g_lpszSrDefaultDisplay_JPN[i], "Disable") == 0 )
			if ( strcmp(g_lpszSrDefaultDisplay_JPN[i], l_strBuffer) == 0 )
				continue;
			pCombo->AddString(g_lpszSrDefaultDisplay_JPN[i]);
		}
		// Kojika 20090603 Change End
		//Saiki 20090527 Change <-----
	}
	pCombo->SetCurSel(m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay);
}

// =========================================================================
//
BOOL CZAxisOffsetSettingDlg::CheckData()
{
	// Kojika 20090603 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090603 Add End

	UpdateData(TRUE);

	///// Stage /////
//	if ( ((CComboBox*)GetDlgItem(IDC_STAGE_PROGRAM))->GetCurSel() == CB_ERR || strcmp(m_MainRcpInfo.MainRcpParam.hdr.szStage, "None") == 0 ) {
	if ( ((CComboBox*)GetDlgItem(IDC_STAGE_PROGRAM))->GetCurSel() == CB_ERR ) {
		// Kojika 20090603 Change
		//MessageBox("Please Select Stage", "Z AXIS OFFSET SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_SELECT_STAGE, l_strBuffer, "Please Select Stage.");
		LoadStringML(IDS_Z_AXIS_OFFSET_SETTING, l_strTitle, "Z AXIS OFFSET SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Lens /////
	if ( m_strLens.IsEmpty() ) {
		// Kojika 20090603 Change
		//MessageBox("Please Select Lens", "Z AXIS OFFSET SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_SELECT_LENS, l_strBuffer, "Please Select Lens.");
		LoadStringML(IDS_Z_AXIS_OFFSET_SETTING, l_strTitle, "Z AXIS OFFSET SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090603 Change End
		GetDlgItem(IDC_LENS)->SetFocus();
		return FALSE;
	}

	///// Default Display /////
	if ( ((CComboBox*)GetDlgItem(IDC_DEFAULT_DISPLAY))->GetCurSel() == CB_ERR ) {
		// Kojika 20090603 Change
		//MessageBox("Please Select Default Display", "Z AXIS OFFSET SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_SELECT_DEFAULT_DISPLAY, l_strBuffer, "Please Select Default Display.");
		LoadStringML(IDS_Z_AXIS_OFFSET_SETTING, l_strTitle, "Z AXIS OFFSET SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090603 Change End
		return FALSE;
	}

	return TRUE;
}
