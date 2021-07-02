// NanoRecipeUI.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "SaveAsDlg.h"
#include "SaveModifiedDlg.h"
#include "NanoRecipeUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CNanoRecipeUI
// #########################################################################

IMPLEMENT_DYNAMIC(CNanoRecipeUI, CNanoUI)

// =========================================================================
//
CNanoRecipeUI::CNanoRecipeUI(LPCTSTR lpszTemplateName) : CNanoUI(lpszTemplateName), m_strCaption("")
{
	if ( ((CMainFrame*)AfxGetMainWnd())->m_bDefaultSetting ) {
		m_nOpenMode = modeDefaultSetting;
		return;
	}

	if ( ((CMainFrame*)AfxGetMainWnd())->m_bNew )
		m_nOpenMode = modeNew;
	else
		m_nOpenMode = modeNormal;

	///// Get Selected Recipe Name /////
	((CMainFrame*)AfxGetMainWnd())->GetSelectListName(m_szRecipeName);
}

// =========================================================================
//
CNanoRecipeUI::CNanoRecipeUI(UINT nIDTemplate) : CNanoUI(nIDTemplate)
{
	if ( ((CMainFrame*)AfxGetMainWnd())->m_bDefaultSetting ) {
		m_nOpenMode = modeDefaultSetting;
		return;
	}

	if ( ((CMainFrame*)AfxGetMainWnd())->m_bNew )
		m_nOpenMode = modeNew;
	else
		m_nOpenMode = modeNormal;

	///// Get Selected Recipe Name /////
	((CMainFrame*)AfxGetMainWnd())->GetSelectListName(m_szRecipeName);
}

// =========================================================================
//
CNanoRecipeUI::~CNanoRecipeUI()
{
}

BEGIN_MESSAGE_MAP(CNanoRecipeUI, CNanoUI)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(IDM_SAVE, OnSave)
	ON_COMMAND(IDM_SAVE_AS, OnSaveAs)
	ON_UPDATE_COMMAND_UI(IDM_SAVE, OnUpdateSave)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_AS, OnUpdateSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
//
void CNanoRecipeUI::OnSave()
{
	Save();
}

// =========================================================================
//
void CNanoRecipeUI::OnSaveAs()
{
	SaveAs();
}

// =========================================================================
//
BOOL CNanoRecipeUI::Save()
{
	//Saiki 20090528 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090528 Add <-----

	///// User Access Privilege /////
	if ( !IsAccessPrivilege() ) {
//Saiki 20090128 Change ----->
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_USER_ACCESS_STOP_TEXT, strBuffer, "You don't have Access Privilege");
		LoadStringML(IDS_USER_ACCESS_STOP_CAPTION, strTitle, "ACCESS PRIVILEGE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
//Saiki 20090128 Change <-----
		return FALSE;
	}

	///// Save /////
	if(m_nOpenMode == modeNew) return SaveAs();

	if (!CheckData() ) return FALSE;

	if (!IsDataChanged() ) return TRUE; // 変更なし

	return SaveRecipeData();
}

// =========================================================================
//
BOOL CNanoRecipeUI::SaveAs()
{
	UINT nOldOpenMode;
	//Saiki 20090528 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090528 Add <-----
	///// User Access Privilege /////
	if ( !IsAccessPrivilege() ) {
//Saiki 20090128 Change ----->
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_USER_ACCESS_STOP_TEXT, strBuffer, "You don't have Access Privilege");
		LoadStringML(IDS_USER_ACCESS_STOP_CAPTION, strTitle, "ACCESS PRIVILEGE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
//Saiki 20090128 Change <-----
		return FALSE;
	}

	///// Save As /////
	if ( !CheckData() ) return FALSE;

	CSaveAsDlg dlg(SaveAsDlgInfo());
	if ( dlg.DoModal() == IDOK ) {
		nOldOpenMode = m_nOpenMode;
		m_nOpenMode = modeNew;			// 一時的に新規作成のモードとする
		strcpy(m_szRecipeName, dlg.m_strName);
		if ( !SaveRecipeData() ) {
			m_nOpenMode = nOldOpenMode;
			return FALSE;
		}
		m_nOpenMode = modeNormal;		// 保存成功したら、ノーマルモードへ
		((CMainFrame*)AfxGetMainWnd())->SetSelectListName(m_szRecipeName);
	}
	return FALSE; // OK,CANCEL			// レシピ一覧画面へは戻らない
}

// =========================================================================
// データ破棄時のCleanUp処理
void CNanoRecipeUI::Discard()
{
}

// =========================================================================//
//// TRUE	: エラーなし
//// FALSE : エラーあり
//BOOL CNanoRecipeUI::CheckData()
//{
//	// サブクラスで定義
//	return TRUE;
//}
//
// =========================================================================//
//// TRUE	: 変更あり
//// FALSE : 変更なし
//BOOL CNanoRecipeUI::IsDataChanged()
//{
//	// サブクラスで定義
//	return FALSE;
//}
//
// =========================================================================//
//// TRUE	: 保存成功
//// FALSE : 保存失敗
//BOOL CNanoRecipeUI::SaveRecipeData()
//{
//	// サブクラスで定義
//	return FALSE;
//}
//
// =========================================================================//
//// TRUE	: アクセス可
//// FALSE : アクセス不可
//BOOL CNanoRecipeUI::IsAccessPrivilege()
//{
//	// サブクラスで定義
//	return FALSE;
//}
//

// =========================================================================
//
BOOL CNanoRecipeUI::Exit()
{
	///// User Access Privilege /////
	if ( !IsAccessPrivilege() )
		return TRUE;	// 変更なし扱い

	if ( !IsDataChanged() )
		return TRUE;	// 変更なし

// 2010.01.13 bagus 修正 --{--
	CString strBuffer;

	LoadStringML(IDS_DEFAULT_VALUE, strBuffer, " DEFAULT VALUE");
// 2010.01.13 bagus 修正 --}--

	CSaveModifiedDlg dlg;
	dlg.m_strModified = m_strCaption;
	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
// 2010.01.13 bagus 修正 --{--
//		  dlg.m_strModified += " DEFAULT VALUE";
		dlg.m_strModified += strBuffer;
// 2010.01.13 bagus 修正 --}--
		break;
	default:
		break;
	}

	if ( dlg.DoModal() == IDOK ) {
		if ( dlg.m_iModified == MODIFIED_SAVE ) {
			return Save();
		}
		else {
			Discard();
		}
	}
	else {
		return FALSE; // CANCEL
	}
	return TRUE;
}

// =========================================================================
//
void CNanoRecipeUI::OnUpdateSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsAccessPrivilege());

}

// =========================================================================
//
void CNanoRecipeUI::OnUpdateSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsAccessPrivilege());
}

/* added 2015.04.24 hmenjo Ref File 削除1 ---------- { ---------- */
void CNanoRecipeUI::DelRefFileByUseMeasPgm(LPCTSTR ptszMeasPgmName)
{
	CString l_strSearchPath;
	l_strSearchPath.Format(_T("%s*%s"), g_szDb_Main_Recipe_Dir, MAINRECIPE_EXT);
	CFileFind l_FindMainRcp;
	BOOL l_bFindExist = l_FindMainRcp.FindFile(l_strSearchPath);
	MAIN_RCP_INFO l_MainRcpInfo;
	TCHAR l_tszRefPath[MAX_PATH];
	int l_iRcRemove = 0;
	while (FALSE != l_bFindExist) {
		l_bFindExist = l_FindMainRcp.FindNextFile();
		if (
			(FALSE == l_FindMainRcp.IsTemporary()) &&
			(FALSE == l_FindMainRcp.IsSystem()) &&
			(FALSE == l_FindMainRcp.IsHidden()) &&
			(FALSE == l_FindMainRcp.IsReadOnly()) &&
			(FALSE == l_FindMainRcp.IsDirectory()) &&
			(FALSE == l_FindMainRcp.IsDots())
		) {
			if (SYSTEM_RECIPE_BEGINNING_CHAR != l_FindMainRcp.GetFileName().GetAt(0)) {
				/* システム利用レシピは非該当	*/
				if (FALSE != RecipeFile_LoadRecipe(&l_MainRcpInfo, (LPCSTR) l_FindMainRcp.GetFileTitle(), RECIPE_FILE_MAIN_RECIPE)) {
					if (0 == _tcsicmp(ptszMeasPgmName, l_MainRcpInfo.MainRcpParam.hdr.szMeas)) {
						/* 使っていました．	*/
						_stprintf(l_tszRefPath, _T("%s%s%s"), g_szData_Ref_Dir, l_FindMainRcp.GetFileTitle(), DAT_EXT);
						l_iRcRemove = _tremove(l_tszRefPath);
						if (0 != l_iRcRemove) {
							/* 削除エラー	*/
							/* 現在処理はありません	*/
						}
					}
				}
			}
		}
	}
}
/* added 2015.04.24 hmenjo Ref File 削除1 ---------- } ---------- */
