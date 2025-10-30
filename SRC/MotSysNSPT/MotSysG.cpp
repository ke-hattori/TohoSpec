// MotSys.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "MotSysDef.h"
#include "..\\..\\inc\\MotsysMsg.h"
#include "MotSysDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotSysApp

BEGIN_MESSAGE_MAP(CMotSysApp, CWinApp)
	//{{AFX_MSG_MAP(CMotSysApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotSysApp �N���X�̍\�z

CMotSysApp::CMotSysApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CMotSysApp �I�u�W�F�N�g

CMotSysApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMotSysApp �N���X�̏�����

#define PATTERN	2	// ��d�N���̖h�~�@�\�̃p�^�[���I��
					/*	1 : FindWindow ���g���ꍇ
						2 : Mutex ���g���ꍇ
					 */
BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam);

BOOL CMotSysApp::InitInstance()
{
	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������������
	//  ��Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ��̂��폜����
	//  ���������B

#ifdef _AFXDLL
	// Enable3dControls();			// No longer needed in modern MFC
#else
	// Enable3dControlsStatic();	// No longer needed in modern MFC
#endif

	// ��d�N���̖h�~����
#if PATTERN == 1
	if (FindWindow(NULL, "Motion System Driver") != NULL) {
		return FALSE;
	}
#elif PATTERN == 2
	m_hMutex_Run = ::CreateMutex(NULL, TRUE, IDENT_CODE);	// exe �̃��l�[���ł����s�o���Ȃ��悤�ɂ��邽�ߓ���̖��O��ݒ�
	if ((m_hMutex_Run == NULL) || (::GetLastError() == ERROR_ALREADY_EXISTS)) {
		// ���łɋN�����Ă���v���Z�X��O�ʂɂ���
		::EnumWindows(&FindWindowProc, NULL);
		return	FALSE;
	}
#endif


	CMotSysDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <��ݾ�> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}

int CMotSysApp::ExitInstance()
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	// ��d�N���̖h�~�̂��߂� Mutex �J��
#if PATTERN == 2
	if (m_hMutex_Run != NULL) {
		::ReleaseMutex(m_hMutex_Run);
		::CloseHandle(m_hMutex_Run);
	}
#endif

	return CWinApp::ExitInstance();
}

/* EnumWindowsProc�R�[���o�b�N�֐� */
BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam)
{
	/* ���������E�B���h�E���A���łɋN�����Ă���A�v���P�[�V�����̃E�B���h�E�����ׂ� */
	if(::GetProp(hwnd, IDENT_CODE)) {
		/* �A�C�R��������Ă���̂ł���΁A���̃T�C�Y�ɖ߂� */
		if (IsIconic(hwnd)) {
			::ShowWindow(hwnd, SW_RESTORE);
		} else {
			::ShowWindow(hwnd, SW_SHOWNORMAL);
		}
		/* ���������E�B���h�E���t�H�A�O���E���h�E�B���h�E�ɂ��� */
		::SetForegroundWindow(::GetLastActivePopup(hwnd));
		// �\������(����(SetForegroundWindow�̌�)����Ȃ��ƂP��� SW_SHOW �ŕ\������Ȃ�����)
		return FALSE;	/* �񋓂𒆒f */
	}
	return TRUE;	/* �񋓂𑱂��� */
}
