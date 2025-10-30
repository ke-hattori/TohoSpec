// UserAccountListDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "UserAccountListDlg.h"
#include "UserAccountSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CUserAccountListDlg �_�C�A���O
// #########################################################################

// =========================================================================
//
CUserAccountListDlg::CUserAccountListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserAccountListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserAccountListDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CUserAccountListDlg::~CUserAccountListDlg()
{
}

// =========================================================================
//
void CUserAccountListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserAccountListDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_NEW_BUTTON, m_NewButton);
	DDX_Control(pDX, IDC_EDIT_BUTTON, m_EditButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);
	DDX_Control(pDX, IDC_SET_BUTTON, m_SetButton);
	DDX_Control(pDX, IDC_RESET_BUTTON, m_ResetButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CUserAccountListDlg, CDialog)
	//{{AFX_MSG_MAP(CUserAccountListDlg)
	ON_BN_CLICKED(IDC_NEW_BUTTON, OnNewButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_SET_BUTTON, OnSetButton)
	ON_BN_CLICKED(IDC_RESET_BUTTON, OnResetButton)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_ACCOUNT_LIST, OnDblclkUserAccountList)
	ON_NOTIFY(NM_CLICK, IDC_USER_ACCOUNT_LIST, OnClickUserAccountList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CUserAccountListDlg ���b�Z�[�W �n���h��

// =========================================================================
//
BOOL CUserAccountListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// New Button /////
	m_NewButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NewButton.DrawFlatFocus(TRUE);

	///// Edit Button /////
	m_EditButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_EditButton.DrawFlatFocus(TRUE);

	///// Delete Button /////
	m_DeleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeleteButton.DrawFlatFocus(TRUE);

	///// Set Button /////
	m_SetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SetButton.DrawFlatFocus(TRUE);

	///// Reset Button /////
	m_ResetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ResetButton.DrawFlatFocus(TRUE);

	///// Static Control /////
	m_stcUserName.SubclassWindow(GetDlgItem(IDC_AUTO_LOGON_NAME)->GetSafeHwnd());
	m_stcUserName.SetBkColor(WATER_COLOR);

	// ���X�g�R���g���[���̏�����
	m_lstCtrl.SubclassWindow(GetDlgItem(IDC_USER_ACCOUNT_LIST)->GetSafeHwnd());
	m_lstCtrl.SetViewStyle();

	UserAccountList_HeaderSet();
	UserAccountList_DataSet();

	// Auto Logon�̏�����
	AutoLogon_Init();

	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
					// ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// =========================================================================
//
void CUserAccountListDlg::OnOK()
{
	char szAutoLogonName[RECIPE_NAME_LEN + 1];

	::ZeroMemory(szAutoLogonName, sizeof(szAutoLogonName));
	GetDlgItemText(IDC_AUTO_LOGON_NAME, szAutoLogonName, sizeof(szAutoLogonName));
	ConfigFile_SetNanoSpecIni(szAutoLogonName, CONFIG_FILE_AUTO_LOGON_NAME);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CUserAccountListDlg::UserAccountList_HeaderSet()
{
	LV_COLUMN listcol;
	int ItemMax;

	// Kojika 20090529 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090529 Add End

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_LEFT };

	int Width[] = {
		0,		// (0) Index
		240,	// (1) Name
		100,	// (2) Access Level
		170,	// (3) Date
	};

	// Kojika 20090529 Change
	//LPTSTR pszItem[] = {
	//	//"",
	//	//"Name",
	//	//"Access Level",
	//	//"Date",
		//"",
	//	  "���O",
	//	  "�A�J�E���g���x��",
	//	  "���t",
	//};
	LPTSTR pszItem_JPN[] = {
		"",
		"���O",
		"�A�J�E���g���x��",
		"���t",
	};
	LPTSTR pszItem_ENU[] = {
		"",
		"Name",
		"Access Level",
		"Date",
	};
	// Kojika 20090529 Change End

	ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		// Kojika 20090529 Change
		//listcol.pszText = pszItem[i];
		if(l_SystemConfig.nLanguage == 0){
			listcol.pszText = pszItem_ENU[i];
		}
		else{
			listcol.pszText = pszItem_JPN[i];
		}
		// Kojika 20090529 Change End
		m_lstCtrl.InsertColumn(i,&listcol);
	}
}

// =========================================================================
//
void CUserAccountListDlg::UserAccountList_DataSet()
{
	CFileFind Finder;
	char szDirPath[_MAX_PATH];
	char szName[MAX_PATH];
	char szDate[256];
	BOOL bLoop;
	CTime time;
	USER_ACCOUNT UserAccount;

	m_lstCtrl.DeleteAllItems();
	sprintf(szDirPath, "%s*%s", g_szCfg_User_User_Account_Dir, DAT_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		::ZeroMemory(&UserAccount, sizeof(UserAccount));
		::ZeroMemory(szName, sizeof(szName));

	// �t�@�C�������擾
	sprintf(szName, "%s", (LPCSTR)Finder.GetFileTitle());
		// �ŏI�ύX�������擾
		Finder.GetLastWriteTime(time);
		strcpy(szDate, time.Format("%Y.%m.%d %H:%M:%S"));

		if ( RecipeFile_LoadRecipe(&UserAccount, szName, RECIPE_FILE_USER_ACCOUNT) )
			UserAccountList_Add(UserAccount.szName, UserAccount.nAccountLevel, szDate);
	}

	if ( m_lstCtrl.GetItemCount() > 0 ) {
		m_lstCtrl.SetSelectionMark(0);
		m_lstCtrl.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}

}

// =========================================================================
//
void CUserAccountListDlg::UserAccountList_Add(char* lpszName, int iAccountLevel, char* lpszDate)
{
	LV_ITEM listitem;
	int item;

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	listitem.iItem = m_lstCtrl.GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	item = m_lstCtrl.InsertItem(&listitem);

	///// Name /////
	listitem.iSubItem = 1;
	listitem.pszText = lpszName;
	m_lstCtrl.SetItem(&listitem);

	///// Access Level /////
	listitem.iSubItem = 2;
//	listitem.pszText = g_lpszAccountLevel[iAccountLevel];
	// Kojika 20090528 Change
	//listitem.pszText = const_cast<PSTR>(g_lpszAccountLevel[iAccountLevel]);
	if(l_SystemConfig.nLanguage == 0){
		listitem.pszText = const_cast<PSTR>(g_lpszAccountLevel_ENU[iAccountLevel]);
	}else{
		listitem.pszText = const_cast<PSTR>(g_lpszAccountLevel_JPN[iAccountLevel]);
	}
	// Kojika 20090528 Change End
	m_lstCtrl.SetItem(&listitem);

	///// Date /////
	listitem.iSubItem = 3;
	listitem.pszText = lpszDate;
	m_lstCtrl.SetItem(&listitem);

	m_lstCtrl.SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

// =========================================================================
//
void CUserAccountListDlg::OnClickUserAccountList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}

// =========================================================================
//
void CUserAccountListDlg::OnDblclkUserAccountList(NMHDR* pNMHDR, LRESULT* pResult)
{
	EditUserAccount(FALSE);

	*pResult = 0;
}

// =========================================================================
//
void CUserAccountListDlg::OnNewButton()
{
	EditUserAccount(TRUE);
}

// =========================================================================
//
void CUserAccountListDlg::OnEditButton()
{
	EditUserAccount(FALSE);
}

// =========================================================================
//
void CUserAccountListDlg::OnDeleteButton()
{
	int iListCount, iSelect;
	char szMessage[256], szName[RECIPE_NAME_LEN + 1];

// Kojika 20090529 Add
	CString l_strBuffer;
// Kojika 20090529 Add End

	///// Get Select User Name /////
	iSelect = m_lstCtrl.GetSelectionMark();
	// �I������Ă��Ȃ��ꍇ�͉������Ȃ�
	if ( iSelect == -1 )
		return;

	m_lstCtrl.GetItemText(iSelect, 1, szName, sizeof(szName));
	// Kojika 20090529 Change
	//sprintf(szMessage, "Do you want to Delete \"%s\"", szName);
	//sprintf(szMessage, "\"%s\"���폜���܂���", szName);
	LoadStringML(IDS_DELETE_ACCOUNT_LIST, l_strBuffer, "Do you want to Delete ""%s""");
	sprintf(szMessage, (LPCSTR)l_strBuffer, szName);
	//if ( MessageBox(szMessage, "USER ACCOUNT LIST", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDCANCEL )
	LoadStringML(IDS_TITLE_USER_ACCOUNT_LIST, l_strBuffer, "USER ACCOUNT LIST");
	if ( MessageBox(szMessage, l_strBuffer, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDCANCEL )
		return;

	///// Delete User /////
	if ( RecipeFile_DeleteRecipe(szName, RECIPE_FILE_USER_ACCOUNT) ) {
		iListCount = m_lstCtrl.GetItemCount();
		UserAccountList_DataSet();

		if(iSelect == iListCount - 1){
			iSelect--;
		}
		m_lstCtrl.SetSelectionMark(iSelect);
		if(iSelect != -1){
			m_lstCtrl.SetItemState(iSelect, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

// =========================================================================
//
void CUserAccountListDlg::OnSetButton()
{
	int iSelect;
	char szName[RECIPE_NAME_LEN + 1];

	iSelect = m_lstCtrl.GetSelectionMark();

	// �I������Ă��Ȃ��ꍇ�͉������Ȃ�
	if ( iSelect == -1 )
		return;

	m_lstCtrl.GetItemText(iSelect, 1, szName, sizeof(szName));
	SetDlgItemText(IDC_AUTO_LOGON_NAME, szName);
}

// =========================================================================
//
void CUserAccountListDlg::OnResetButton()
{
	SetDlgItemText(IDC_AUTO_LOGON_NAME, "");
}

// =========================================================================
//
void CUserAccountListDlg::EditUserAccount(BOOL bNew)
{
	int iSelect;
	char szName[RECIPE_NAME_LEN + 1];
	iSelect = m_lstCtrl.GetSelectionMark();

// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
// Kojika 20090529 Add End

	// ���ڂ��I������Ă��Ȃ��ꍇ�C���b�Z�[�W��\��
	if ( !bNew && iSelect == -1 ) {
		//MessageBox("Please Select User", "USER ACCOUNT LIST", MB_OK | MB_ICONSTOP);
		//MessageBox("���[�U��I�����Ă�������", "USER ACCOUNT LIST", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_SELECT_USER, l_strBuffer, "Please Select User");
		LoadStringML(IDS_TITLE_USER_ACCOUNT_LIST, l_strTitle, "USER ACCOUNT LIST");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	// ���O��ǂݍ���
	m_lstCtrl.GetItemText(iSelect, 1, szName, sizeof(szName));

	CUserAccountSettingDlg dlg(bNew, szName);
	if ( dlg.DoModal() == IDOK )
		UserAccountList_DataSet();
}

// =========================================================================
//
void CUserAccountListDlg::AutoLogon_Init()
{
	char szAutoLogonName[RECIPE_NAME_LEN + 1];

	memset(szAutoLogonName, 0, sizeof(szAutoLogonName));
	ConfigFile_GetNanoSpecIni(szAutoLogonName, CONFIG_FILE_AUTO_LOGON_NAME);
	if ( strcmp(szAutoLogonName, "") != 0 ) {
		SetDlgItemText(IDC_AUTO_LOGON_NAME, szAutoLogonName);
	}
}
