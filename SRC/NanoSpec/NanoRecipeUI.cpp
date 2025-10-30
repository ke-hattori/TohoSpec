// NanoRecipeUI.cpp : �C���v�������e�[�V���� �t�@�C��
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

	if (!IsDataChanged() ) return TRUE; // �ύX�Ȃ�

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
		m_nOpenMode = modeNew;			// �ꎞ�I�ɐV�K�쐬�̃��[�h�Ƃ���
		strcpy(m_szRecipeName, dlg.m_strName);
		if ( !SaveRecipeData() ) {
			m_nOpenMode = nOldOpenMode;
			return FALSE;
		}
		m_nOpenMode = modeNormal;		// �ۑ�����������A�m�[�}�����[�h��
		((CMainFrame*)AfxGetMainWnd())->SetSelectListName(m_szRecipeName);
	}
	return FALSE; // OK,CANCEL			// ���V�s�ꗗ��ʂւ͖߂�Ȃ�
}

// =========================================================================
// �f�[�^�j������CleanUp����
void CNanoRecipeUI::Discard()
{
}

// =========================================================================//
//// TRUE	: �G���[�Ȃ�
//// FALSE : �G���[����
//BOOL CNanoRecipeUI::CheckData()
//{
//	// �T�u�N���X�Œ�`
//	return TRUE;
//}
//
// =========================================================================//
//// TRUE	: �ύX����
//// FALSE : �ύX�Ȃ�
//BOOL CNanoRecipeUI::IsDataChanged()
//{
//	// �T�u�N���X�Œ�`
//	return FALSE;
//}
//
// =========================================================================//
//// TRUE	: �ۑ�����
//// FALSE : �ۑ����s
//BOOL CNanoRecipeUI::SaveRecipeData()
//{
//	// �T�u�N���X�Œ�`
//	return FALSE;
//}
//
// =========================================================================//
//// TRUE	: �A�N�Z�X��
//// FALSE : �A�N�Z�X�s��
//BOOL CNanoRecipeUI::IsAccessPrivilege()
//{
//	// �T�u�N���X�Œ�`
//	return FALSE;
//}
//

// =========================================================================
//
BOOL CNanoRecipeUI::Exit()
{
	///// User Access Privilege /////
	if ( !IsAccessPrivilege() )
		return TRUE;	// �ύX�Ȃ�����

	if ( !IsDataChanged() )
		return TRUE;	// �ύX�Ȃ�

// 2010.01.13 bagus �C�� --{--
	CString strBuffer;

	LoadStringML(IDS_DEFAULT_VALUE, strBuffer, " DEFAULT VALUE");
// 2010.01.13 bagus �C�� --}--

	CSaveModifiedDlg dlg;
	dlg.m_strModified = m_strCaption;
	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
// 2010.01.13 bagus �C�� --{--
//		  dlg.m_strModified += " DEFAULT VALUE";
		dlg.m_strModified += strBuffer;
// 2010.01.13 bagus �C�� --}--
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

/* added 2015.04.24 hmenjo Ref File �폜1 ---------- { ---------- */
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
				/* �V�X�e�����p���V�s�͔�Y��	*/
				if (FALSE != RecipeFile_LoadRecipe(&l_MainRcpInfo, (LPCSTR) l_FindMainRcp.GetFileTitle(), RECIPE_FILE_MAIN_RECIPE)) {
				if (0 == _tcsicmp(ptszMeasPgmName, l_MainRcpInfo.MainRcpParam.hdr.szMeas)) {
					/* �g���Ă��܂����D	*/
					_stprintf(l_tszRefPath, _T("%s%s%s"), g_szData_Ref_Dir, (LPCTSTR)l_FindMainRcp.GetFileTitle(), DAT_EXT);
						l_iRcRemove = _tremove(l_tszRefPath);
						if (0 != l_iRcRemove) {
							/* �폜�G���[	*/
							/* ���ݏ����͂���܂���	*/
						}
					}
				}
			}
		}
	}
}
/* added 2015.04.24 hmenjo Ref File �폜1 ---------- } ---------- */
