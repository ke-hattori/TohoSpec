// StressReferenceListDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "Mainfrm.h"
#include "StressReferenceListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStressReferenceListDlg ダイアログ


CStressReferenceListDlg::CStressReferenceListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStressReferenceListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStressReferenceListDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

// 2009.10.22 bagus Stress 修正 --{--
	m_bListMode = FALSE;
	m_strSampleID = "";
// 2009.10.22 bagus Stress 修正 --}--
}


void CStressReferenceListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressReferenceListDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_DELETE_REFERENCE_BUTTON, m_DeleteReferenceButton);
// 2009.10.22 bagus Stress 修正 --{--
	DDX_Control(pDX, IDOK, m_OkButton);
// 2009.10.22 bagus Stress 修正 --}--

	DDX_Control(pDX, IDC_REFERENCE_LIST, m_StressReferenceListCtrl);
}


BEGIN_MESSAGE_MAP(CStressReferenceListDlg, CDialog)
	//{{AFX_MSG_MAP(CStressReferenceListDlg)
	ON_BN_CLICKED(IDC_DELETE_REFERENCE_BUTTON, OnDeleteReferenceButton)
	ON_NOTIFY(NM_DBLCLK, IDC_REFERENCE_LIST, OnDblclkReferenceList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStressReferenceListDlg メッセージ ハンドラ

BOOL CStressReferenceListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Delete Button /////
	m_DeleteReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeleteReferenceButton.DrawFlatFocus(TRUE);

	///// Stress Reference List /////
	StressReferenceList_Init();
	SetStressReferenceListData();

// 2009.10.22 bagus Stress 修正 --{--
	///// Ok Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	CString	strBuffer;

	if (m_bListMode) {
		GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DELETE_REFERENCE_BUTTON)->ShowWindow(SW_HIDE);
		LoadStringML(IDS_REFERENCE_LIST_CANCEL, strBuffer, "CANCEL");
		SetDlgItemText(IDCANCEL, strBuffer);
	} else {
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DELETE_REFERENCE_BUTTON)->ShowWindow(SW_SHOW);
		LoadStringML(IDS_REFERENCE_LIST_CLOSE, strBuffer, "CLOSE");
		SetDlgItemText(IDCANCEL, strBuffer);
	}
// 2009.10.22 bagus Stress 修正 --}--

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CStressReferenceListDlg::OnDeleteReferenceButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (!DeleteStressReference()) {
		// Err:Delete
	}

	SetStressReferenceListData();
}

void CStressReferenceListDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

void CStressReferenceListDlg::StressReferenceList_Init()
{
	LV_COLUMN listcol;
	int ItemMax;

// 2009.10.28 bagus Stress 追加修正 --{--
#if 0
	int Width[] = {
		198,
		198,
		198,
	};

	LPTSTR pszItemJPN[] = {
		"サンプルID",
		"レシピ名",
		"リファレンス取得日時",
	};
	LPTSTR pszItemENU[] = {
		"Sample ID",
		"Main Reference Name",
		"Reference Set Date",
	};
#else
	int Width[] = {
		297,
		297,
	};

	LPTSTR pszItemJPN[] = {
		"サンプルID",
		"リファレンス取得日時",
	};
	LPTSTR pszItemENU[] = {
		"Sample ID",
		"Reference Set Date",
	};
#endif
// 2009.10.28 bagus Stress 追加修正 --}--

	int Fmt[] = { LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER };


	ItemMax = sizeof(Width) / sizeof(int);

	m_StressReferenceListCtrl.SetViewStyle();

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		if(m_SystemConfig.nLanguage == 0){
			listcol.pszText = pszItemENU[i];
		}else{
			listcol.pszText = pszItemJPN[i];
		}
		m_StressReferenceListCtrl.InsertColumn(i,&listcol);
	}

	m_StressReferenceListCtrl.DeleteAllItems();
}

// 2009.10.28 bagus Stress 追加修正 --{--
#if 0
void CStressReferenceListDlg::SetStressReferenceListData()
{
	m_StressReferenceListCtrl.DeleteAllItems();

	LV_ITEM listitem;
	int item;

	CFileFind fnd1;
	CFileFind fnd2;

//	TCHAR drive[_MAX_DRIVE];		// ドライブ
//	TCHAR dir[_MAX_DIR];			// ディレクトリ名
//	TCHAR fname[_MAX_FNAME];		// ファイル名
//	TCHAR ext[_MAX_EXT];			// 拡張子

	CString	strSampleID;
	CString	strStressRef;
	CString	strStressRefPath;

#if 0
	CString strPathName = g_szResult_Dir;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.*");
	strStressRef.Format("%s%s", STRESS_REFERENCE_NAME, DAT_EXT);
#else
	CString strPathName = g_szData_StressRef_Dir;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.dat");
	//strStressRef.Format("%s%s", STRESS_REFERENCE_NAME, DAT_EXT);
#endif

	FILETIME	ftFileTime, ftLocalFileTime;
	SYSTEMTIME	stFileTime;

	CString	strBuffer;

	if(fnd1.FindFile(strPathName, 0)){
		int i = 1;
		while(i){
			i = fnd1.FindNextFile();

			// ファイル名が"."か".."の場合は次を検索
			if(fnd1.IsDots())
				continue;

#if 0
			// フォルダだった場合、サンプルIDとして取得
			if(fnd1.IsDirectory()){
				strStressRefPath = fnd1.GetFilePath();
				strStressRefPath.TrimRight('\\');
				strStressRefPath += _T("\\");
				strStressRefPath += strStressRef;

				if(fnd2.FindFile(strStressRefPath, 0)){
					int j = 1;
					j = fnd2.FindNextFile();

					_splitpath(fnd1.GetFileName(), drive, dir, fname, ext);

					listitem.iItem = m_StressReferenceListCtrl.GetItemCount();
					listitem.mask = LVIF_TEXT;

					///// Sample ID /////
					listitem.iSubItem = 0;
					listitem.pszText = fname;
					item = m_StressReferenceListCtrl.InsertItem(&listitem);

					///// Main Reference Name /////
					listitem.iSubItem = 1;
					listitem.pszText = "";
					m_StressReferenceListCtrl.SetItem(&listitem);

					///// Reference Set Date /////
					listitem.iSubItem = 2;
					if (fnd2.GetCreationTime(&ftFileTime)) {
						FileTimeToLocalFileTime(&ftFileTime , &ftLocalFileTime);
						FileTimeToSystemTime(&ftLocalFileTime , &stFileTime);
						strBuffer.Format("%d.%02d.%02d %02d:%02d:%02d", stFileTime.wYear, stFileTime.wMonth, stFileTime.wDay, stFileTime.wHour, stFileTime.wMinute, stFileTime.wSecond);
						listitem.pszText = strBuffer.GetBuffer(0);
					} else {
						listitem.pszText = "";
					}
					m_StressReferenceListCtrl.SetItem(&listitem);
				}
				fnd2.Close();
			}
#else
			if(fnd1.IsDirectory()){
				continue;
			}
			listitem.iItem = m_StressReferenceListCtrl.GetItemCount();
			listitem.mask = LVIF_TEXT;

			///// Sample ID /////
			TCHAR szName[MAX_PATH];
			_tcscpy(szName,fnd1.GetFileTitle().GetBuffer(0));
			listitem.iSubItem = 0;
			listitem.pszText = szName;
			item = m_StressReferenceListCtrl.InsertItem(&listitem);

			///// Main Reference Name /////
			//MAIN_RCP_INFO_HDR l_MainRcpInfo;
			//bMain = RecipeFile_LoadRecipe(&l_MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
			TCHAR tszRecipeName[RECIPE_NAME_LEN+1];
			GetPrivateProfileString(_T("MainRecipe"),_T("Name"),_T("???"),tszRecipeName,RECIPE_NAME_LEN,fnd1.GetFilePath().GetBuffer(0));
			listitem.iSubItem = 1;
			listitem.pszText = tszRecipeName;
			m_StressReferenceListCtrl.SetItem(&listitem);

			///// Reference Set Date /////
			listitem.iSubItem = 2;
			if (fnd1.GetCreationTime(&ftFileTime)) {
				FileTimeToLocalFileTime(&ftFileTime , &ftLocalFileTime);
				FileTimeToSystemTime(&ftLocalFileTime , &stFileTime);
				strBuffer.Format("%d.%02d.%02d %02d:%02d:%02d", stFileTime.wYear, stFileTime.wMonth, stFileTime.wDay, stFileTime.wHour, stFileTime.wMinute, stFileTime.wSecond);
				listitem.pszText = strBuffer.GetBuffer(0);
			} else {
				listitem.pszText = "";
			}
			m_StressReferenceListCtrl.SetItem(&listitem);
#endif
		}
		fnd1.Close();
	}
}
#else
void CStressReferenceListDlg::SetStressReferenceListData()
{
	m_StressReferenceListCtrl.DeleteAllItems();

	LV_ITEM listitem;
	int item;

	CFileFind fnd1;
	CFileFind fnd2;

	CString	strSampleID;
	CString	strStressRef;
	CString	strStressRefPath;

	CString strPathName = g_szData_StressRef_Dir;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.dat");

	FILETIME	ftFileTime, ftLocalFileTime;
	SYSTEMTIME	stFileTime;

	CString	strBuffer;

	if(fnd1.FindFile(strPathName, 0)){
		int i = 1;
		while(i){
			i = fnd1.FindNextFile();

			// ファイル名が"."か".."の場合は次を検索
			if(fnd1.IsDots())
				continue;

			if(fnd1.IsDirectory()){
				continue;
			}
			listitem.iItem = m_StressReferenceListCtrl.GetItemCount();
			listitem.mask = LVIF_TEXT;

			///// Sample ID /////
			TCHAR szName[MAX_PATH];
			_tcscpy(szName,fnd1.GetFileTitle().GetBuffer(0));
			listitem.iSubItem = 0;
			listitem.pszText = szName;
			item = m_StressReferenceListCtrl.InsertItem(&listitem);

			///// Reference Set Date /////
			listitem.iSubItem = 1;
			if (fnd1.GetCreationTime(&ftFileTime)) {
				FileTimeToLocalFileTime(&ftFileTime , &ftLocalFileTime);
				FileTimeToSystemTime(&ftLocalFileTime , &stFileTime);
				strBuffer.Format("%d.%02d.%02d %02d:%02d:%02d", stFileTime.wYear, stFileTime.wMonth, stFileTime.wDay, stFileTime.wHour, stFileTime.wMinute, stFileTime.wSecond);
				listitem.pszText = strBuffer.GetBuffer(0);
			} else {
				listitem.pszText = "";
			}
			m_StressReferenceListCtrl.SetItem(&listitem);
		}
		fnd1.Close();
	}
}
#endif
// 2009.10.28 bagus Stress 追加修正 --}--

BOOL CStressReferenceListDlg::DeleteStressReference()
{
	// 選択されているインデックス取得
	int	nIndex;
	nIndex = m_StressReferenceListCtrl.GetSelectionMark();
	if ( nIndex == -1 )
		return ( FALSE );

	// サンプルID名称取得
	CString	strSampleID;
	strSampleID = m_StressReferenceListCtrl.GetItemText(nIndex, 0);
	strSampleID.TrimLeft();
	strSampleID.TrimRight();
	if (strSampleID.IsEmpty())
		return ( FALSE );

	//削除の確認
	////@@
	CString strMessage;
	CString strTitle;
	//実際はりソースからメッセージを持ってくる
	//Saiki 20091214 Change ---->
//	strMessage = "リファレンスを削除してよいですか？";
//	strTitle = "REFERENCE";
//	if(MessageBox(strMessage,strTitle,MB_YESNO ) != IDYES){
//		return FALSE;
//	}
	LoadStringML(IDS_REF_AND_RESULT_DELETE_MESSAGE , strMessage, "Reference Data And Result Data together will be deleted Are you sure?");
	LoadStringML(IDS_TITLE_REFERENCE, strTitle,  "REFERENCE");

	if(MessageBox(strMessage,strTitle,MB_YESNO ) != IDYES){
		return FALSE;
	}
	//Saiki 20091214 Change <-----



	CString strPathName;
	////Data\Ref\StressRef内のリファレンスを削除
	strPathName = g_szData_StressRef_Dir;
	strPathName.TrimRight('\\');
	strPathName += _T("\\");
	strPathName += strSampleID;
	strPathName += DAT_EXT;
	DeleteFile(strPathName.GetBuffer(0));

	////RESULTディレクトリ内のリファレンスを全削除
	// サンプルIDまでのパス作成
	strPathName = g_szResult_Dir;
	strPathName.TrimRight('\\');
	strPathName += _T("\\");
	strPathName += strSampleID;
//Saiki 20091217 Delete ----->/*サンプルID名ディレクトリー毎消す*/
//	strPathName += _T("\\");
//
//	// リファレンスDirとFileのパス作成
//	CString strStressRefFile;
//	CString strStressRefDir;
//	strStressRefDir = strPathName;
//	strStressRefDir += STRESS_REFERENCE_NAME;
//	strStressRefFile = strStressRefDir;
//	strStressRefFile += DAT_EXT;
//Saiki 20091217 Delete <-----

	// Dir削除のための準備
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// 削除を行う
//Saiki 20091217 Change ----->
//	int nRetFile, nRetDir;
//
//	nRetFile = ::DeleteFile(strStressRefFile);
//	nRetDir = pMainFrame->DeleteDirectory(strStressRefDir);
//	if ((nRetFile != 0) || (nRetDir != 0))
//		return ( FALSE );

	int  nRetDir;
	nRetDir = pMainFrame->DeleteDirectory(strPathName);
	if (nRetDir != 0)
		return ( FALSE );
//Saiki 20091217 Change <-----

	return ( TRUE );
}

// 2009.10.22 bagus Stress 修正 --{--
void CStressReferenceListDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	// 選択されているインデックス取得
	int	nIndex;
	nIndex = m_StressReferenceListCtrl.GetSelectionMark();
	if ( nIndex == -1 )
		return;

	// サンプルID名称取得
	CString	strSampleID;
	strSampleID = m_StressReferenceListCtrl.GetItemText(nIndex, 0);
	strSampleID.TrimLeft();
	strSampleID.TrimRight();
	if (strSampleID.IsEmpty())
		return;

	m_strSampleID = strSampleID;

	CDialog::OnOK();
}
// 2009.10.22 bagus Stress 修正 --}--

// 2009.12.03 K.Matsuo -->
void CStressReferenceListDlg::OnDblclkReferenceList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnOK();

	*pResult = 0;
}
// 2009.12.03 K.Matsuo <--
