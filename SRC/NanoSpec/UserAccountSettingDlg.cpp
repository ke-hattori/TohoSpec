// UserAccountSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SaveAsDlg.h"
#include "UserAccountSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserAccountSettingDlg ダイアログ                                       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CUserAccountSettingDlg::CUserAccountSettingDlg(BOOL bNew, char* lpszName /* = NULL */, CWnd* pParent /*=NULL*/)
	: CDialog(CUserAccountSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserAccountSettingDlg)
	//}}AFX_DATA_INIT
	m_bNew = bNew;
	if(!bNew && lpszName != NULL)
		memcpy(m_UserAccount.szName, lpszName, sizeof(m_UserAccount.szName));
}

/////////////////////////////////////////////////////////////////////////////
//
void CUserAccountSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserAccountSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_USER_NAME, m_UserAccount.szName, USER_NAME_LEN + 1);
	DDX_Check(pDX, IDC_CHECK_MEASUREMENT, m_AccessLevel[m_UserAccount.nAccountLevel].bMeasurement);
	DDX_Check(pDX, IDC_CHECK_RECIPE, m_AccessLevel[m_UserAccount.nAccountLevel].bRecipe);
	DDX_Check(pDX, IDC_CHECK_PROGRAM, m_AccessLevel[m_UserAccount.nAccountLevel].bProgram);
	DDX_Check(pDX, IDC_CHECK_DATA, m_AccessLevel[m_UserAccount.nAccountLevel].bData);
	DDX_Check(pDX, IDC_CHECK_USER_SETTING, m_AccessLevel[m_UserAccount.nAccountLevel].bUserSetting);
	DDX_Check(pDX, IDC_CHECK_MAINTENANCE, m_AccessLevel[m_UserAccount.nAccountLevel].bMaintenance);
	DDX_Check(pDX, IDC_CHECK_ENABLE_PASSWORD, m_UserAccount.bEnablePassword);
	DDX_Text(pDX, IDC_PASSWORD, m_UserAccount.szPassword, PASSWORD_LEN + 1);
	DDX_Text(pDX, IDC_CONFIRM_PASSWORD, m_strConfirmPassword);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CUserAccountSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CUserAccountSettingDlg)
	ON_CBN_SELCHANGE(IDC_ACCOUNT_LEVEL, OnSelchangeAccountLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserAccountSettingDlg メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
BOOL CUserAccountSettingDlg::OnInitDialog()
{
	//// Load Data /////
	ConfigFile_GetNanoSpecIni(m_AccessLevel, CONFIG_FILE_ACCESS_LEVEL); 	// アクセスレベルは参照情報として使用する（更新なし）

	///// Read User Acount /////
	// 新規作成
	if(m_bNew){
		m_UserAccount.nAccountLevel = ACCOUNT_LEVEL_ADMINISTRATOR;
	}

	// 変更
	else{
		if(!RecipeFile_LoadRecipe(&m_UserAccount, m_UserAccount.szName, RECIPE_FILE_USER_ACCOUNT)){
			m_bNew = TRUE;
			m_UserAccount.nAccountLevel = ACCOUNT_LEVEL_OPERATOR;
		}
	}

	///// Check User Level /////
	if(m_UserAccount.nAccountLevel < ACCOUNT_LEVEL_ADMINISTRATOR || m_UserAccount.nAccountLevel >= ACCOUNT_LEVEL_MAX){
		m_UserAccount.nAccountLevel = ACCOUNT_LEVEL_OPERATOR;
	}

	///// Password /////
	m_strConfirmPassword = m_UserAccount.szPassword;

	CDialog::OnInitDialog();

	///// Ok Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Control Initialize /////
	InitCombo_AccountLevel();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_USER_NAME))->SetLimitText(/*RECIPE_NAME_LEN*/USER_NAME_LEN);
	((CEdit*)GetDlgItem(IDC_PASSWORD))->SetLimitText(PASSWORD_LEN);
	((CEdit*)GetDlgItem(IDC_CONFIRM_PASSWORD))->SetLimitText(PASSWORD_LEN);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CUserAccountSettingDlg::OnSelchangeAccountLevel()
{
	UpdateData();

	m_UserAccount.nAccountLevel = ((CComboBox*)GetDlgItem(IDC_ACCOUNT_LEVEL))->GetCurSel();

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//
void CUserAccountSettingDlg::OnOK()
{
//	CMainFrame* pMainFrame = (CMainFrame *) AfxGetMainWnd();
//	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
//	BOOL bAccess;
//
//	///// User Access Privilege /////
//	if((bAccess = pDoc->User_Access(ACCESS_USERSETTING)) == FALSE){
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
//		return;
//	}

	///// Check /////
	if(!CheckData()) return;

	///// Save /////
	RecipeFile_SaveRecipe(&m_UserAccount, m_UserAccount.szName, RECIPE_FILE_USER_ACCOUNT);

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
void CUserAccountSettingDlg::InitCombo_AccountLevel()
{
	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End
	///// Account Level /////
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_ACCOUNT_LEVEL);
	pCombo->ResetContent();
	for ( int i = 0; i < ACCOUNT_LEVEL_MAX; i++ )
	// Kojika 20090528 Change
	//pCombo->AddString(g_lpszAccountLevel[i]);
	if(l_SystemConfig.nLanguage == 0){
		pCombo->AddString(g_lpszAccountLevel_ENU[i]);
	}else{
		pCombo->AddString(g_lpszAccountLevel_JPN[i]);
	}
	// Kojika 20090528 Change End
	pCombo->SetCurSel(m_UserAccount.nAccountLevel);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CUserAccountSettingDlg::CheckData()
{
	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	CString l_strBuffer1, l_strBuffer2, l_strBuffer3;
	// Kojika 20090529 Add End

	UpdateData();

	///// Name Check /////
	if ( strcmp(m_UserAccount.szName, "TOHO") == 0 ) {
		// Kojika 20090529 Change
		//MessageBox("This name cannot be entered. Please enter a different name", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("この名前は入力できません. 違う名前を入力してください", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_NAME_NOT_ENTER, l_strBuffer, "This name cannot be entered. Please enter a different name");
		LoadStringML(IDS_TITLE_USER_ACCOUNT_SETTING, l_strTitle, "USER ACCOUNT SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_USER_NAME)->SetFocus();
		return FALSE;
	}

	///// Name Check /////
	if ( strlen(m_UserAccount.szName) == 0 ) {
		// Kojika 20090529 Change
		//MessageBox("Please enter a user name", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("ユーザ名を入力してください", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_ENTER_USER_NAME, l_strBuffer, "Please enter a user name");
		LoadStringML(IDS_TITLE_USER_ACCOUNT_SETTING, l_strTitle, "USER ACCOUNT SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_USER_NAME)->SetFocus();
		return FALSE;
	}
	if ( strlen(m_UserAccount.szName) > USER_NAME_LEN ) {
		char szMsg[256+1];
		memset(szMsg, 0, sizeof(szMsg));
		// Kojika 20090529 Change
		//sprintf(szMsg, "%s%d%s%d%s", "ユーザ名は半角", USER_NAME_LEN, "文字(全角", USER_NAME_LEN/2, "文字)以内で入力してください");
		LoadStringML(IDS_ENTER_USER_NAME_WITHIN, l_strBuffer1, "Please enter a user name within normal-width ");
		LoadStringML(IDS_ENTER_USER_NAME_NORMAL, l_strBuffer2, " characters (em-size ");
		LoadStringML(IDS_ENTER_USER_NAME_EMSIZE, l_strBuffer3, " characters)");
		sprintf(szMsg, "%s%d%s%d%s", l_strBuffer1, USER_NAME_LEN, l_strBuffer2, USER_NAME_LEN/2, l_strBuffer3);
		//MessageBox(szMsg, "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_TITLE_USER_ACCOUNT_SETTING, l_strTitle, "USER ACCOUNT SETTING");
		MessageBox(szMsg, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_USER_NAME)->SetFocus();
		return FALSE;
	}
	if ( !CheckName(m_UserAccount.szName, USER_NAME_LEN) ) {
		// Kojika 20090529 Change
		//MessageBox("User Name can not contain spaces or punctuation characters", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("ユーザ名にスペースや句読点を含むことは出来ません", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_NOT_CONTAIN_SPACE, l_strBuffer, "User Name can not contain spaces or punctuation characters");
		LoadStringML(IDS_TITLE_USER_ACCOUNT_SETTING, l_strTitle, "USER ACCOUNT SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_USER_NAME)->SetFocus();
		return FALSE;
	}

	///// Account Level /////
	if ( ((CComboBox*)GetDlgItem(IDC_ACCOUNT_LEVEL))->GetCurSel() == CB_ERR ) {
		// Kojika 20090529 Change
		//MessageBox("Please Select an Account Level", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("アカウントレベルを選択してください", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_SELECT_ACCOUNT_LEVEL, l_strBuffer, "Please Select an Account Level");
		LoadStringML(IDS_TITLE_USER_ACCOUNT_SETTING, l_strTitle, "USER ACCOUNT SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_ACCOUNT_LEVEL)->SetFocus();
		return FALSE;
	}

	///// Password ////
	if ( m_UserAccount.bEnablePassword ) {
		if ( strlen(m_UserAccount.szPassword) == 0 ) {
			// Kojika 20090529 Change
			//MessageBox("Please enter a password", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
			//MessageBox("パスワードを入力してください", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
			LoadStringML(IDS_ENTER_PASSWORD, l_strBuffer, "Please enter a password");
			LoadStringML(IDS_TITLE_USER_ACCOUNT_SETTING, l_strTitle, "USER ACCOUNT SETTING");
			MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
			// Kojika 20090529 Change End
			GetDlgItem(IDC_PASSWORD)->SetFocus();
			return FALSE;
		}
	}

	if ( m_strConfirmPassword != m_UserAccount.szPassword ) {
		// Kojika 20090529 Change
		//MessageBox("Password is different", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("パスワードが違います", "USER ACCOUNT SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_PASSWORD_DIFFRENT, l_strBuffer, "Password is different");
		LoadStringML(IDS_TITLE_USER_ACCOUNT_SETTING, l_strTitle, "USER ACCOUNT SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_CONFIRM_PASSWORD)->SetFocus();
		return FALSE;
	}

	return TRUE;
}
