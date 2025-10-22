/****************************************************************
 *		Copyright (C) 2003 by Tencor Instruments
 *
 *		Source Upgrade From VC1.52 to VC6.0
 *
 *		Updated by Ouyang-jc
 *
 *		Date:	2003/03/26
 ****************************************************************/

// ===========================================================================
// Include files
// ===========================================================================

#include <afxwin.h>

#include "..\\..\\inc\\MotSysMsg.h"

#include "app.h"
#include "win.h"




// ===========================================================================
// Create instance of application (equivalent to WinMain)
// ===========================================================================

static CMotTskApp ThisApp;



/*---------------------------------------------------------------------------

   CMotTskApp

   InitInstance:
   Called once when the application begins.  All we need to do is to create
   a new CMainWnd object which can handle itself once made visible.

---------------------------------------------------------------------------*/
BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam);

BOOL CMotTskApp::InitInstance ()
{
	// allow only one instance of this app (handled by Mutex below)

#ifdef _DEBUG			   // Enable trace flags, output debug Window
	afxTraceFlags	= 0;
	afxTraceEnabled = 1;
#endif

   TRACE ("MOTION SYSTEM Task -- Started \n");

   	// ��d�N����h�~
//	m_hMutex_Run = ::CreateMutex(NULL, TRUE, m_pszExeName);
	m_hMutex_Run = ::CreateMutex(NULL, TRUE, IDENT_CODE);	// exe �̃��l�[���ł����s�o���Ȃ��悤�ɂ��邽�ߓ���̖��O��ݒ�
	if ((m_hMutex_Run == NULL) || (::GetLastError() == ERROR_ALREADY_EXISTS)) {
		// ���łɋN�����Ă���v���Z�X��O�ʂɂ���
		::EnumWindows(&FindWindowProc, NULL);
		return	FALSE;
	}

	m_pMainWnd = new cMotTskWnd();
	m_pMainWnd->ShowWindow(m_nCmdShow);
/* added 2009.06.01 hmenjo Servo.ini �ŏ����ŏ�����ݒ� ---------- { ---------- */
	TCHAR	l_tszText[32];
	::GetPrivateProfileString(_T("Window"), _T("AutoHide"), _T("2"), l_tszText, sizeof(l_tszText), _T("..\\cfg\\Servo.ini"));
	int l_iAutoHide = _ttoi(l_tszText);
	if ((l_iAutoHide < 0) || (2 < l_iAutoHide)) {
		l_iAutoHide = 2;
	}
	switch (l_iAutoHide) {
	case 1:
		m_pMainWnd->ShowWindow(SW_MINIMIZE);
	case 2:
		m_pMainWnd->ShowWindow(SW_HIDE);
	case 0:
	default:
		break;
	}
/* added 2009.06.01 hmenjo Servo.ini �ŏ����ŏ�����ݒ� ---------- } ---------- */
	m_pMainWnd->UpdateWindow();

	return TRUE;
}
int CMotTskApp::ExitInstance()
{
	AfxTrace("CMotTskApp::ExitInstance\n");
	if (m_pMainWnd) {
		delete m_pMainWnd;
	}

	// ��d�N���̖h�~�̂��߂� Mutex �J��
	if (m_hMutex_Run != NULL) {
		::ReleaseMutex(m_hMutex_Run);
		::CloseHandle(m_hMutex_Run);
	}

	return true;
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
