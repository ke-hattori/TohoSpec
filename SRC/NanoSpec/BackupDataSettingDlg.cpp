// BackupDataSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "RecipeFile.hxx"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "BackupDataSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackupDataSettingDlg ダイアログ


CBackupDataSettingDlg::CBackupDataSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackupDataSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBackupDataSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CBackupDataSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackupDataSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Check(pDX,IDC_AUTO_BACKUP_CHECK,m_BackupSetting.bAutoBackup);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_BACKUP_NOW_BUTTON, m_BackupNowButton);
	DDX_Control(pDX, IDC_RESTORE_BUTTON, m_RestoreButton);
	DDX_Control(pDX, IDC_XMP_BACKUP_BUTTON, m_XMPBackupButton);
	DDX_Control(pDX, IDC_REF_BACKUP_PATH, m_RefButton);

}


BEGIN_MESSAGE_MAP(CBackupDataSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CBackupDataSettingDlg)
	ON_BN_CLICKED(IDC_BACKUP_NOW_BUTTON, OnBackupNowButton)
	ON_BN_CLICKED(IDC_RESTORE_BUTTON, OnRestoreButton)
	ON_BN_CLICKED(IDC_REF_BACKUP_PATH, OnRefBackupPath)
	ON_BN_CLICKED(IDC_XMP_BACKUP_BUTTON, OnXmpBackupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackupDataSettingDlg メッセージ ハンドラ

void CBackupDataSettingDlg::OnBackupNowButton()
{
	CString l_strBuffer;
	CString l_strTitle;
	CString strPath;
	char szDir[100];

	LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");
	GetDlgItemText(IDC_BACKUP_PATH,strPath);

	//バックアップ先が未指定なのでなにもしない
	if(strPath.GetLength() == 0){
	//2009.12.22 bagus 修正 --{--
		LoadStringML(IDS_BACKUP_NO_BACKUP_PATH,l_strBuffer,"Please input backup path.");
		MessageBox(l_strBuffer, l_strTitle, MB_OK| MB_ICONINFORMATION);
	//2009.12.22 bagus 修正 --}--
		return;
	}

	//2009.12.21 bagus Recipe Backup --{--
	//保存先ドライブチェック
	_splitpath(strPath.GetBuffer(0),szDir,NULL,NULL,NULL);
	strncat(szDir,"\\",sizeof(szDir)-1);
	DWORD dwRet;

	dwRet = GetFileAttributes(szDir);
	if(dwRet == (DWORD)-1 || (dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0){
		LoadStringML(IDS_BACKUP_DRIVE_NOT_EXIST,l_strBuffer,"Backup drive does not exist.");
		MessageBox(l_strBuffer,l_strTitle,MB_OK | MB_ICONWARNING);
		return;
	}
	//2009.12.21 bagus Recipe Backup --}--


	LoadStringML(IDS_BACKUP_NOW_CONFIRM, l_strBuffer, "Would you backup recipe data");
	if ( MessageBox(l_strBuffer, l_strTitle, MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL) {
		return;
	}


	RecipeFile_SetBackupPath(strPath.GetBuffer(0),m_OldBackupSetting.bAutoBackup);
	BOOL bRet = RecipeFile_BackupAllRecipe();
	RecipeFile_SetBackupPath(m_OldBackupSetting.BackupPath,m_OldBackupSetting.bAutoBackup);

	if(!bRet){
		LoadStringML(IDS_BACKUP_NOW_ERROR, l_strBuffer, "Can't backup recipe data");
		LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");
		if ( MessageBox(l_strBuffer, l_strTitle, MB_OK| MB_ICONWARNING) == IDOK) {
			return;
		}
	}
}

void CBackupDataSettingDlg::OnRestoreButton()
{
	CString l_strBuffer;
	CString l_strTitle;
	CString strPath;
	DWORD dwRet;

	GetDlgItemText(IDC_BACKUP_PATH,strPath);
	dwRet = GetFileAttributes(strPath.GetBuffer(0));
	if(dwRet == (DWORD)-1 || !(dwRet & FILE_ATTRIBUTE_DIRECTORY)){
		//バックアップディレクトリが存在しない
		LoadStringML(IDS_RESTORE_NO_BACKUP_PATH, l_strBuffer, "No Backup path.");
		LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK| MB_ICONWARNING);
		return;
	}

	LoadStringML(IDS_RESTORE_CONFIRM, l_strBuffer, "Would you restore recipe data?");
	LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");
	if ( MessageBox(l_strBuffer, l_strTitle, MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL) {
		return;
	}
	RecipeFile_SetBackupPath(strPath.GetBuffer(0),m_OldBackupSetting.bAutoBackup);
	BOOL bRet = RecipeFile_RestoreAllRecipe();
	RecipeFile_SetBackupPath(m_OldBackupSetting.BackupPath,m_OldBackupSetting.bAutoBackup);

	if(!bRet){
		LoadStringML(IDS_RESTORE_ERROR, l_strBuffer, "Can't restore recipe data");
		LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");
		if ( MessageBox(l_strBuffer, l_strTitle, MB_OK| MB_ICONWARNING) == IDOK) {
			return;
		}
	}
}

void CBackupDataSettingDlg::OnRefBackupPath()
{
	TCHAR tszPath[MAX_PATH+1];

	GetDlgItemText(IDC_BACKUP_PATH,tszPath,MAX_PATH);

	BROWSEINFO	tBrowseInfo;
	tBrowseInfo.hwndOwner = this->GetSafeHwnd();
	tBrowseInfo.pidlRoot = NULL;
	tBrowseInfo.pszDisplayName = NULL;
	tBrowseInfo.lpszTitle = NULL;
	tBrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS ;/*| BIF_NEWDIALOGSTYLE;*/
	tBrowseInfo.lpfn = NULL;
	tBrowseInfo.lParam = NULL; //(long)tszPath;
	tBrowseInfo.iImage = 0;
	LPITEMIDLIST	pidl = SHBrowseForFolder(&tBrowseInfo);
	if(pidl){
		TCHAR			szNewPath[MAX_PATH];
		SHGetPathFromIDList(pidl, szNewPath);
		CoTaskMemFree(pidl);

		SetDlgItemText(IDC_BACKUP_PATH,szNewPath);
	}

}

void CBackupDataSettingDlg::OnOK()
{
	CString strBackupPath;
	USER_DATA l_UserData;
	CString l_strBuffer;
	CString l_strTitle;

	UpdateData();

	GetDlgItemText(IDC_BACKUP_PATH,strBackupPath);

	//バックアップ先が未指定なのでなにもしない
	//2009.12.25 bagus Recipe backup --{--
	strBackupPath.TrimLeft();
	strBackupPath.TrimRight();
	//2009.12.25 bagus Recipe backup --}--
	if(strBackupPath.GetLength() == 0 && m_BackupSetting.bAutoBackup){
	//2009.12.22 bagus 修正 --{--
		LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");
		LoadStringML(IDS_BACKUP_NO_BACKUP_PATH,l_strBuffer,"Please input backup path.");
		MessageBox(l_strBuffer, l_strTitle, MB_OK| MB_ICONINFORMATION);
	//2009.12.22 bagus 修正 --}--
		return;
	}

	_tcscpy(m_BackupSetting.BackupPath,strBackupPath.GetBuffer(0));

	ConfigFile_GetNanoSpecIni(&l_UserData,CONFIG_FILE_USER_SETTING);
	l_UserData.BackupData = m_BackupSetting;
	ConfigFile_SetNanoSpecIni(&l_UserData,CONFIG_FILE_USER_SETTING);
	ConfigFile_SaveNanoSpecIni(CONFIG_FILE_USER_SETTING);
	//2009.12.21 bagus Recipe Backup --{--
	RecipeFile_SetBackupPath(m_BackupSetting.BackupPath,m_BackupSetting.bAutoBackup);
	//2009.12.21 bagus Recipe Backup --}--

	CDialog::OnOK();
}

void CBackupDataSettingDlg::OnCancel()
{

	CDialog::OnCancel();
}

BOOL CBackupDataSettingDlg::OnInitDialog()
{
	USER_DATA	l_UserData;

	CDialog::OnInitDialog();

/* added 2014.12.24 hmenjo Nano 名称修正残り ---------- { ---------- */
	if (g_lAppNameType != APP_NAME_NANO) {
		CString l_strCaption;
		this->GetDlgItemText(IDC_STATIC_NANODATA, l_strCaption);
		l_strCaption.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
		this->SetDlgItemText(IDC_STATIC_NANODATA, l_strCaption);
	}
/* added 2014.12.24 hmenjo Nano 名称修正残り ---------- } ---------- */

	ConfigFile_GetNanoSpecIni(&l_UserData,CONFIG_FILE_USER_SETTING);
	m_BackupSetting = l_UserData.BackupData;
	memcpy(&m_OldBackupSetting,&m_BackupSetting,sizeof(m_BackupSetting));

	SetDlgItemText(IDC_BACKUP_PATH,m_BackupSetting.BackupPath);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Backup Button /////
	m_BackupNowButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_BackupNowButton.DrawFlatFocus(TRUE);

	///// Restore Button /////
	m_RestoreButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RestoreButton.DrawFlatFocus(TRUE);

	///// XMP Backup Button /////
	m_XMPBackupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_XMPBackupButton.DrawFlatFocus(TRUE);

	///// Folder Reference Backup Button /////
	m_RefButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RefButton.DrawFlatFocus(TRUE);

	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CBackupDataSettingDlg::OnXmpBackupButton()
{
	SR_XMP l_SrXmp;
	char szDrive[MAX_PATH];
	char szDir[MAX_PATH];
	char szFile[MAX_PATH];
	char szExt[MAX_PATH];
	CString strFrom;
	CString strTo;
	SHFILEOPSTRUCT tSHFile;
	CString l_strBuffer;
	CString l_strTitle;

	ZeroMemory(&tSHFile, sizeof(SHFILEOPSTRUCT));

	ConfigFile_GetNanoSpecIni(&l_SrXmp,CONFIG_FILE_SR_XMP);
	_splitpath(l_SrXmp.szAdapExePath,szDrive,szDir,szFile,szExt);
	GetDlgItemText(IDC_BACKUP_PATH,strTo);
	LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");

	//バックアップ先が未指定なのでなにもしない
	if(strTo.GetLength() == 0){
	//2009.12.22 bagus 修正 --{--
		LoadStringML(IDS_BACKUP_NO_BACKUP_PATH,l_strBuffer,"Please input backup path.");
		MessageBox(l_strBuffer, l_strTitle, MB_OK| MB_ICONINFORMATION);
	//2009.12.22 bagus 修正 --}--
		return;
	}

	LoadStringML(IDS_XMP_BACKUP_CONFIRM, l_strBuffer, "XMP Data backup?");
	if ( MessageBox(l_strBuffer, l_strTitle, MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL) {
		return;
	}

	//フォルダコピー
	strFrom.Format("%s%s%s\\*",szDrive,szDir,"analysis");
	strTo += "\\analysis";

	//コピー先のディレクトリを作成する
	if(!MakeDirectory(strTo.GetBuffer(0))){
		LoadStringML(IDS_XMP_BACKUP_ERROR, l_strBuffer, "Can't backup XMP Data");
		LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK| MB_ICONWARNING);
		return;
	}

	tSHFile.hwnd = this->GetSafeHwnd();
	tSHFile.wFunc = FO_COPY;
	//2009.12.17 bagus 修正 --{--
	//tSHFile.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SIMPLEPROGRESS ;
	//tSHFile.fFlags = FOF_NOCONFIRMMKDIR | FOF_NOERRORUI ;
	tSHFile.fFlags = FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SIMPLEPROGRESS;
	//2009.12.17 bagus 修正 --}--

	tSHFile.fAnyOperationsAborted = TRUE;
	tSHFile.hNameMappings = NULL;
	//2009.12.17 bagus 修正 --{--
	//tSHFile.lpszProgressTitle = NULL;
	tSHFile.lpszProgressTitle = "XMP Backup";
	//2009.12.17 bagus 修正 --}--

	strFrom += "0";
	strTo += "0";
	strFrom.SetAt( strFrom.GetLength()-1, NULL );
	strTo.SetAt( strTo.GetLength()-1, NULL );

	tSHFile.pFrom = (LPCTSTR)strFrom.GetBuffer(0);
	tSHFile.pTo = (LPCTSTR)strTo.GetBuffer(0);

	if(SHFileOperation( &tSHFile ) != 0){
		LoadStringML(IDS_XMP_BACKUP_ERROR, l_strBuffer, "Can't backup XMP Data");
		LoadStringML(IDS_TITLE_BACKUP_SETTING, l_strTitle, "BACKUP SETTING");
		MessageBox(l_strBuffer, l_strTitle, MB_OK| MB_ICONWARNING);
	}
}
int CBackupDataSettingDlg::MakeDirectory(char* lpszDir)
{
	char szBuff[256];
	int i;
	int iRet;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = NULL;

	if((iRet = CreateDirectory(lpszDir, &sa)) == TRUE)
		return 1;
	else{
		//既に存在している場合にはそこでOKとする
		if(GetLastError() == ERROR_ALREADY_EXISTS){
			return 1;
		}
	}

	for(i = strlen(lpszDir); i > 0; i--){
		if (lpszDir[i] == '\\' || lpszDir[i] == '/')
			break;
	}

	if(i <= 0)
		return 0;

	strcpy(szBuff, lpszDir);
	szBuff[i] = NULL;
	//if(iRet = MakeDirectory(szBuff))
	//	return (iRet = CreateDirectory(lpszDir, &sa)) == TRUE ? 1 : 0;
	if(iRet = MakeDirectory(szBuff)){
		iRet = CreateDirectory(lpszDir, &sa);
		if(iRet){
			return 1;
		}else{
			//既に存在している場合にはそこでOKとする
			if(GetLastError() == ERROR_ALREADY_EXISTS){
				return 1;
			}
		}
	}

	return 0;
}
