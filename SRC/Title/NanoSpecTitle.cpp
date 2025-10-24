// NanoSpecTitle.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#define _MASTER_
#include "System.h"
#include "NanoSpecTitle.h"
#include "NanoSpecTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleApp

BEGIN_MESSAGE_MAP(CNanoSpecTitleApp, CWinApp)
	//{{AFX_MSG_MAP(CNanoSpecTitleApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleApp �N���X�̍\�z

CNanoSpecTitleApp::CNanoSpecTitleApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CNanoSpecTitleApp �I�u�W�F�N�g

CNanoSpecTitleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleApp �N���X�̏�����

BOOL CNanoSpecTitleApp::InitInstance()
{
	AfxEnableControlContainer();

	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������������
	//	��Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ��̂��폜����
	//	���������B

#ifdef _AFXDLL
	// Enable3dControls(); 		// No longer needed in modern MFC
#else
	// Enable3dControlsStatic();	// No longer needed in modern MFC
#endif
/* added 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
	// ��d�N���̖h�~����
	m_hMutex_Run = ::CreateMutex(NULL, TRUE, m_pszExeName);
	if ((m_hMutex_Run == NULL) || (::GetLastError() == ERROR_ALREADY_EXISTS)) {
		return	FALSE;
	}
/* added 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */

// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
	g_lAppNameType = APP_NAME_NANO; // default application name
	for(int i = 0; i < APP_NAME_MAX; i++){
		if(_tcsncmp(m_pszExeName, g_lpszAppPrefix4[i], 4) == 0){
			g_lAppNameType = i;
			break;
		}
	}
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--

	//Saiki 20090525 Add ----->
	CString strBuffer;
	SYSTEM_CONFIG l_SystemConfig;
	///// NanoSpec.ini�̃O���[�o���ϐ��̏����� /////
	ConfigFile_LoadAllNanoSpecIni();
	//Saiki 20090723 Delete ----->
	//ConfigFile_SaveAllNanoSpecIni();
	//Saiki 20090723 Delete <-----
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	HANDLE m_hModule;
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
	#ifdef _DEBUG
	strBuffer.Format("dtns%sspecTitle", g_lpszAppPrefix4[g_lAppNameType]);
	#else
	strBuffer.Format("tns%sspecTitle", g_lpszAppPrefix4[g_lAppNameType]);
	#endif
	m_hModule = ResourceLoadLibrary(l_SystemConfig.nLanguage, strBuffer);
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--
	AfxSetResourceHandle((HINSTANCE) m_hModule);
	//Saiki 20090525 Add <-----

	CNanoSpecTitleDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//		 �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <��ݾ�> �ŏ����ꂽ���̃R�[�h��
		//		 �L�q���Ă��������B
	}

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

/* added 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
int CNanoSpecTitleApp::ExitInstance()
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

	// ��d�N���̖h�~�̂��߂� Mutex �J��
	if (m_hMutex_Run != NULL) {
		::ReleaseMutex(m_hMutex_Run);
		::CloseHandle(m_hMutex_Run);
	}

	return CWinApp::ExitInstance();
}
/* added 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
