
#include "stage.h"
#include "StageSPT.h"

#define	MotMsgON
#include "..\\..\\inc\\MotsysMsg.h"
#include "..\\..\\inc\\MotSys.h"

#define	TIMEOUT_CREATEPROC		5000L	// [ms]ï¿½Fï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g
#define	TIMEOUT_MOTSYS_INIT		60000L	// [ms]ï¿½FMotSys ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g
#define	TIMEOUT_MOTSYS_SRVON	5000L	// [ms]ï¿½FMotSys ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÌŒï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½Å‚ÌƒTï¿½[ï¿½{ï¿½Iï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g
#define	TIMEOUT_MOTSYS_ORG		600		// [s] ï¿½FMotSys ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½ï¿½ï¿½@ï¿½Å’ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½Å‚ï¿½)
/* modified 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- { ---------- */
//#define	TIMEOUT_MOTSYS_MOTION	300		// [s] ï¿½FMotSys ï¿½Ê�ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½ï¿½ï¿½@ï¿½Å’ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½Å‚ï¿½)
/* modified 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ----------				*/
#define	TIMEOUT_MOTSYS_MOTION	600		// [s] ï¿½FMotSys ï¿½Ê�ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½ï¿½ï¿½@ï¿½Å’ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½Å‚ï¿½)
/* modified 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- } ---------- */

#define	MOTTSK_WINDOW_NAME	_T("Motion Task")
#define	MOTSYS_WINDOW_NAME	_T("Motion System Driver")
#define	MSGBOX_TITLE_STAGE_DLL	_T("Stage.dll")

/* ===========================================================================
 *	ï¿½Pï¿½ï¿½ï¿½Ö�ï¿½ï¿½ï¿½`ï¿½Å‚ï¿½ï¿½D
 */
//	ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½`ï¿½Fï¿½bï¿½Nï¿½Ö�ï¿½
BOOL CheckIsTimeOut(
		DWORD dwStartTime,	// ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½[ms]
		DWORD dwTimeOut		// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½ï¿½[ms]
	)
{
	DWORD l_dwEndTime = ::GetTickCount();
	DWORD l_dwElpsTime;
	if (l_dwEndTime < dwStartTime) {
		l_dwElpsTime = l_dwEndTime + (ULONG_MAX - dwStartTime) + 1;
	} else {
		l_dwElpsTime = l_dwEndTime - dwStartTime;
	}
	if (dwTimeOut <= l_dwElpsTime) {
		// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
		return TRUE;
	} else {
		return FALSE;
	}
}

//	MotSys ï¿½Ö�ï¿½ï¿½Ô�ï¿½ï¿½ï¿½ÏŠï¿½
WORD ConvAxisTo(WORD wAxis)
{
	WORD l_wAxis;

	switch (wAxis) {
	case X:
		l_wAxis = AXIS_BIG_X;
		break;
	case Y:
		l_wAxis = AXIS_BIG_Y;
		break;
	default:
		l_wAxis = AXIS_NUM + 1;
		break;
	}

	return l_wAxis;
}
//	MotSys ï¿½ï¿½ï¿½ç�²ï¿½Ô�ï¿½ï¿½ï¿½ÏŠï¿½
WORD ConvAxisFrom(WORD wAxis)
{
	WORD l_wAxis;

	switch (wAxis) {
	case AXIS_BIG_X:
		l_wAxis = X;
		break;
	case AXIS_BIG_Y:
		l_wAxis = Y;
		break;
	default:
		l_wAxis = AXIS_NUM + 1;
		break;
	}

	return l_wAxis;
}

//	ï¿½wï¿½è‘¬ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½é‚¢ï¿½Í�Å�ï¿½ï¿½ï¿½ï¿½xï¿½ğ’´‚ï¿½ï¿½È‚ï¿½ï¿½æ‚¤ï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
long SpeedAdjustLimit(long lSpeed, long lLocalSpeed, long lMaxSpeed)
{
	long l_lSpeed = lSpeed;
	long l_lLocalSpeed = lLocalSpeed;
	long l_lMaxSpeed = lMaxSpeed;

	long l_lDir;
	if (l_lSpeed < 0) {
		l_lSpeed *= -1L;
		l_lDir = -1L;
	} else {
		l_lDir = 1L;
	}
	if (l_lLocalSpeed < 0L) {
		l_lLocalSpeed *= -1;
	}
	if (l_lMaxSpeed < 0L) {
		l_lMaxSpeed *= -1L;
	}
	if (0 == l_lSpeed) {
		// ï¿½wï¿½è‘¬ï¿½xï¿½ï¿½ï¿½Oï¿½Ì�ê�‡ï¿½Cï¿½ï¿½ï¿½[ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½xï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
		l_lSpeed = l_lLocalSpeed;
		if (0 == l_lSpeed) {
			// ï¿½ï¿½ï¿½[ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½Oï¿½Ì�ê�‡ï¿½Cï¿½Å�ï¿½ï¿½ï¿½ï¿½xï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
			l_lSpeed = l_lMaxSpeed;
			if (0 == l_lSpeed) {
				// ï¿½Å�ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½Oï¿½Ì�ê�‡ï¿½Cï¿½ï¿½ï¿½xï¿½lï¿½ï¿½ï¿½Pï¿½Oï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D(ï¿½ï¿½ï¿½ï¿½ï¿½É‚Í—ï¿½ï¿½È‚ï¿½ï¿½Í‚ï¿½ï¿½Å‚ï¿½)
				l_lSpeed = 10;
			}
		} else {
			if (l_lMaxSpeed < l_lSpeed) {
				l_lSpeed = l_lMaxSpeed;
			}
		}
	} else {
		if (l_lLocalSpeed < l_lSpeed) {
			l_lSpeed = l_lLocalSpeed;
		}
		if (l_lMaxSpeed < l_lSpeed) {
			l_lSpeed = l_lMaxSpeed;
		}
	}


	return l_lSpeed * l_lDir;
}

//	ï¿½wï¿½ß’lï¿½ï¿½ï¿½ï¿½Jï¿½Eï¿½ï¿½ï¿½gï¿½Ö•ÏŠï¿½
long ConvCountFrom(long lValue, long lFactor)
{
	long l_lCount = lValue * lFactor;

	return l_lCount;
}
//	ï¿½wï¿½ß’lï¿½ÖƒJï¿½Eï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ÏŠï¿½
long ConvCountTo(long lCount, long lFactor)
{
	LONG64 l_l64Count = lCount;
	LONG64 l_l64Factor = lFactor;
	LONG64 l_l64Value1;
	LONG64 l_l64Value2;
	LONG64 l_l64_1000 = 1000;
//	LONG64 l_l64_500 = (lCount < 0)? -500 : 500;

	l_l64Value1 = (l_l64Count * l_l64_1000) / l_l64Factor;
	LONG64 l_l64_500 = (l_l64Value1 < 0)? -500 : 500;
	l_l64Value2 = (l_l64Value1 + l_l64_500) / l_l64_1000;	// ï¿½lï¿½ÌŒÜ“ï¿½

	return static_cast<long>(l_l64Value2);
}

// =========================================================================
// Class Name :  CStageSPT
//
// Descriptionï¿½F Sodick ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½bï¿½pï¿½Nï¿½ï¿½ï¿½X
//					Sodick ï¿½ï¿½ XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
//
// Author:		 hmenjo
// =========================================================================

// =========================================================================
//	ï¿½Rï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^ / ï¿½fï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^
CStageSPT::CStageSPT()
{
	UINT i;
	m_bEnableStage = FALSE;
	m_hwndMotSys = 0;
	m_hwndMotTsk = 0;
	m_bMotSysInitialized = FALSE;
	m_bIsInitializedStageSTD = FALSE;
	memset(m_lMaxSpeed, 0, sizeof(m_lMaxSpeed));
	memset(m_lSoftHP, 0, sizeof(m_lSoftHP));
	memset(m_lLocalSpeed, 0, sizeof(m_lLocalSpeed));
	memset(&m_CurrPosSPT, 0, sizeof(m_CurrPosSPT));
	for (i = 0; i < 8; i++) {
		m_lFactor[i] = 1;	// ï¿½ï¿½ï¿½Wï¿½ÏŠï¿½ï¿½Wï¿½ï¿½ï¿½Í‚Pï¿½É‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
	}
	m_SoftLimitX.Positive = m_SoftLimitX.Negative = 0;
	m_SoftLimitY.Positive = m_SoftLimitY.Negative = 0;

	::InitializeCriticalSection(&m_csInquirePos);

	BOOL l_bResult;
	// MotTsk ï¿½ï¿½ï¿½Nï¿½ï¿½
	l_bResult = MotTskExe(EXE_START);
	if (0 == l_bResult) {
		// ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½s
		::MessageBox(0, _T("CStageSPT\nFailed to start MotTsk.exe."), MSGBOX_TITLE_STAGE_DLL, MB_OK | MB_SYSTEMMODAL);
	} else {
		l_bResult = MotSysExe(EXE_START);
		if (0 == l_bResult) {
			// ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½s
			::MessageBox(0, _T("CStageSPT\nFailed to start MotSys.exe."), MSGBOX_TITLE_STAGE_DLL, MB_OK | MB_SYSTEMMODAL);
		} else {
			m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME);
			m_hwndMotTsk = ::FindWindow(0, MOTTSK_WINDOW_NAME);
#if 0	// ï¿½ï¿½ï¿½ï¿½(ï¿½È‰ï¿½)ï¿½Í•sï¿½vï¿½É‚È‚ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D
			/*
			 *	XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½ÈŠOï¿½ï¿½ STAGETYPE_STD ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
			 *	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä�CCStageNTN ï¿½ï¿½ï¿½Kï¿½vï¿½Å‚ï¿½ï¿½D
			 */
			m_pStageSTD = new CStageNTN();	/*
									 			ï¿½ï¿½ï¿½ÌƒNï¿½ï¿½ï¿½Xï¿½ÌƒRï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^ï¿½Å‚ï¿½
									 			ï¿½Tï¿½uï¿½Rï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒfï¿½[ï¿½^ï¿½Ì“Ç�oï¿½ï¿½ï¿½ï¿½
									 			ï¿½sï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½Dï¿½ï¿½ï¿½Ô‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
											 */
			if (0 == m_pStageSTD) {
				// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s
				::MessageBox(0, _T("CStageSPT\nFailed to create(new) CStageNTN."), MSGBOX_TITLE_STAGE_DLL, MB_OK | MB_SYSTEMMODAL);
			}
#endif
		}
	}

}
CStageSPT::~CStageSPT()
{
	// MotSys ï¿½ï¿½ï¿½Iï¿½ï¿½
	m_hwndMotSys = 0;
	MotSysExe(EXE_QUIT);

	// MotTsk ï¿½ï¿½ï¿½Iï¿½ï¿½
	m_hwndMotTsk = 0;
	MotTskExe(EXE_QUIT);

	::DeleteCriticalSection(&m_csInquirePos);

#if 0
	if (0 != m_pStageSTD) {
		delete m_pStageSTD;
		m_pStageSTD = 0;
	}
#endif
}

/* ===========================================================================
 *	ï¿½È‰ï¿½ï¿½Í�ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½Ì‘ï¿½ï¿½ÌŠÖ�ï¿½ï¿½Å‚ï¿½ï¿½D
 */
//	MotTsk ï¿½Ì‹Nï¿½ï¿½/ï¿½Iï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½
BOOL CStageSPT::MotTskExe(EXE_SWITCH ExeSwitch)
{
	HWND l_hWnd;
	BOOL l_bRet = FALSE;

	l_hWnd = ::FindWindow(0, MOTTSK_WINDOW_NAME);

	switch (ExeSwitch) {
	case EXE_START:
		if (l_hWnd == NULL) {
			STARTUPINFO StartInfo;
			PROCESS_INFORMATION ProcessInfo;
			ZeroMemory(&StartInfo,sizeof(StartInfo));
			ZeroMemory(&ProcessInfo,sizeof(ProcessInfo));
#ifdef _DEBUG
			StartInfo.wShowWindow = SW_SHOWNORMAL;
#else
			StartInfo.wShowWindow = SW_HIDE;
#endif
			StartInfo.cb = sizeof(StartInfo);
/* added 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- { ---------- */
			TCHAR l_tszExePath[_MAX_PATH];
			_stprintf(l_tszExePath, _T("%s") BIN_DIR _T("MotTsk.exe"), g_tszProcDir);
/* added 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- } ---------- */
			l_bRet = ::CreateProcess(
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- { ---------- */
//							BIN_DIR _T("\\MotTsk.exe"),
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- 			 */
							l_tszExePath,
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- } ---------- */
							_T(""),
							NULL,
							NULL,
							FALSE,
							0,
							NULL,
							".",
							&StartInfo,
							&ProcessInfo
						);
			if (0 != l_bRet) {
				DWORD l_dwStartTime = ::GetTickCount();
				while (0 == ::FindWindow(0, MOTTSK_WINDOW_NAME)) {
					// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‘Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D
					//	(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ‹ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Å‚ï¿½)
					::Sleep(100);
					// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½
					if (TRUE == CheckIsTimeOut(l_dwStartTime, TIMEOUT_CREATEPROC)) {
						// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
						l_bRet = FALSE;
						break;
					}
				}
			}
		} else {
			// ï¿½ï¿½ï¿½Å‚É‹Nï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D
			l_bRet = TRUE;
		}
		break;
#if 0
	case EXE_SHOW:
		if (l_hWnd != NULL) {
			l_bRet = ::ShowWindow(l_hWnd, SW_SHOWNORMAL);
		} else {
			// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D
			l_bRet = FALSE;
		}
		break;
	case EXE_HIDE:
		if (l_hWnd != NULL) {
			l_bRet = ::ShowWindow(l_hWnd, SW_HIDE);
		} else {
			// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D
			l_bRet = FALSE;
		}
		break;
#endif
	case EXE_QUIT:
		if (l_hWnd != NULL) {
			l_bRet = ::PostMessage(l_hWnd, WM_CLOSE, NULL, NULL);
		} else {
			// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D
			l_bRet = FALSE;
		}
		break;
	default:
		l_bRet = FALSE;
		break;
	}

	return l_bRet;
}

//	MotSys ï¿½Ì‹Nï¿½ï¿½/ï¿½Iï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½
BOOL CStageSPT::MotSysExe(EXE_SWITCH ExeSwitch)
{
	HWND l_hWnd;
	BOOL l_bRet = FALSE;

	l_hWnd = ::FindWindow(0, MOTSYS_WINDOW_NAME);

	switch (ExeSwitch) {
	case EXE_START:
		if (l_hWnd == NULL) {
			STARTUPINFO StartInfo;
			PROCESS_INFORMATION ProcessInfo;
			ZeroMemory(&StartInfo,sizeof(StartInfo));
			ZeroMemory(&ProcessInfo,sizeof(ProcessInfo));
			StartInfo.wShowWindow = SW_SHOWNORMAL;
			StartInfo.cb = sizeof(StartInfo);
/* added 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- { ---------- */
			TCHAR l_tszExePath[_MAX_PATH];
			_stprintf(l_tszExePath, _T("%s") BIN_DIR _T("MotSysNSPT.exe"), g_tszProcDir);
/* added 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- } ---------- */
			l_bRet = ::CreateProcess(
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- { ---------- */
//							BIN_DIR _T("\\MotSysNSPT.exe"),
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- 			 */
							l_tszExePath,
/* modified 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ Stage.dll ---------- } ---------- */
							_T(""),
							NULL,
							NULL,
							FALSE,
							0,
							NULL,
							".",
							&StartInfo,
							&ProcessInfo
						);
			if (0 != l_bRet) {
				DWORD l_dwStartTime = ::GetTickCount();
				while (0 == ::FindWindow(0, MOTSYS_WINDOW_NAME)) {
					// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‘Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D
					//	(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ‹ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Å‚ï¿½)
					::Sleep(100);
					// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½
					if (TRUE == CheckIsTimeOut(l_dwStartTime, TIMEOUT_CREATEPROC)) {
						// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
						l_bRet = FALSE;
						break;
					}
				}
			}
		} else {
			// ï¿½ï¿½ï¿½Å‚É‹Nï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D
			l_bRet = TRUE;
		}
		break;
	case EXE_QUIT:
		if (l_hWnd != NULL) {
			l_bRet = ::PostMessage(l_hWnd, WM_CLOSE, NULL, NULL);
		} else {
			// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D
			l_bRet = FALSE;
		}
		break;
	default:
		l_bRet = FALSE;
		break;
	}

	return l_bRet;
}

//	StageSTD ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½mï¿½F
BOOL CStageSPT::IsInitializedStageSTD(void)
{
	return m_bIsInitializedStageSTD;
}

//	ï¿½ï¿½ï¿½İˆÊ’u ï¿½æ“¾ ï¿½Xï¿½ï¿½ï¿½bï¿½h(ï¿½ï¿½ï¿½gï¿½p)
UINT CStageSPT::MonitorThreadProcSPT(void)
{
	while (0 != m_bMonitorRun) {
		InquirePosSPT();

		::Sleep(100);
	}
	return 0L;
}

//	ï¿½ï¿½ï¿½İˆÊ’u ï¿½æ“¾
int CStageSPT::InquirePosSPT(void)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	STAGE_COORD l_GetPos;
	l_GetPos.lX = MotsysGetPosition(m_hwndMotSys, ConvAxisTo(X));
	l_GetPos.lY = MotsysGetPosition(m_hwndMotSys, ConvAxisTo(Y));
	switch (l_GetPos.lX) {
	case MS_GETPOS_PARAMERROR:		// ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[(MOTDRV_GETPOSITION ï¿½p)
		return STAGE_ERR_MS_PARAMETER_ERROR;
		break;
	case MS_GETPOS_AXIS_UNINIT:		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(MOTDRV_GETPOSITION ï¿½p)
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
		break;
	case MS_GETPOS_AXIS_NOTHOME:	// ï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A(MOTDRV_GETPOSITION ï¿½p)
		return STAGE_ERR_MS_AXIS_NOT_HOME;
		break;
	default:
		switch (l_GetPos.lY) {
		case MS_GETPOS_PARAMERROR:		// ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[(MOTDRV_GETPOSITION ï¿½p)
			return STAGE_ERR_MS_PARAMETER_ERROR;
			break;
		case MS_GETPOS_AXIS_UNINIT:		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(MOTDRV_GETPOSITION ï¿½p)
			return STAGE_ERR_MS_AXIS_UNINITIALIZED;
			break;
		case MS_GETPOS_AXIS_NOTHOME:	// ï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A(MOTDRV_GETPOSITION ï¿½p)
			return STAGE_ERR_MS_AXIS_NOT_HOME;
			break;
		default:
			{
				::EnterCriticalSection(&m_csInquirePos);

				m_CurrPosSPT.lX = l_GetPos.lX;
				m_CurrPosSPT.lY = l_GetPos.lY;
				STAGE_COORD l_CurrPos;
				l_CurrPos.lX = ConvCountTo(m_CurrPosSPT.lX, m_lFactor[X]);
				l_CurrPos.lY = ConvCountTo(m_CurrPosSPT.lY, m_lFactor[Y]);
				PhysicalToLogical(&l_CurrPos, (STAGE_COORD*) &m_CurrPos);
//				PhysicalToLogical(&l_GetPos, (STAGE_COORD*) &m_CurrPos);
				// SPT ï¿½Å‚Í�CMotSys ï¿½ï¿½ï¿½W(ï¿½ï¿½Î�ï¿½ï¿½W)ï¿½ÅŠÇ—ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
				//		UI ï¿½Ì�Û‚É˜_ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½Æ•ÏŠï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D

				::LeaveCriticalSection(&m_csInquirePos);
			}
			break;
		}
		break;
	}


	return STAGE_ERR_NONE;
}

//	MotSys X/Y ï¿½ï¿½ï¿½ÌˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½Xï¿½ï¿½ï¿½bï¿½h
int CStageSPT::MotSys_WaitMotionXY(LPVOID pParams)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}

	int l_iRc = STAGE_ERR_NONE;
	DWORD l_dwTimuOut = HIWORD(pParams) * 1000L;
	DWORD l_dwStartTime = ::GetTickCount();
	while ((TRUE == gc_bInMotionMS[X]) || (TRUE == gc_bInMotionMS[Y])) {
		::Sleep(100);
		// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½
		if (TRUE == CheckIsTimeOut(l_dwStartTime, l_dwTimuOut)) {
			// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
			l_iRc = STAGE_ERR_MS_MOTION_TIMEOUT;
			break;
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// ï¿½ï¿½~ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½ï¿½ï¿½Ç‚ï¿½ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
		switch (gc_dwMotionStop[X]) {
		case STAGE_STOP_OK:							// X ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½~
			switch (gc_dwMotionStop[Y]) {
			case STAGE_STOP_OK:							// Y ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½~
				break;
			case STAGE_STOP_FORCE:
				l_iRc = STAGE_ERR_MS_MOTION_FORCE;
				break;									// Y ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½~:
			case STAGE_STOP_FAIL:
			default:
				l_iRc = STAGE_ERR_MS_MOTION_FAIL;
				break;									// Y ï¿½ï¿½ ï¿½Ù�ï¿½ï¿½~:
			}
			break;
		case STAGE_STOP_FORCE:						// X ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½~
			l_iRc = STAGE_ERR_MS_MOTION_FORCE;
			break;
		case STAGE_STOP_FAIL:						// X ï¿½ï¿½ ï¿½Ù�ï¿½ï¿½~
		default:
			l_iRc = STAGE_ERR_MS_MOTION_FAIL;
			break;
		}
	}
	// ï¿½Ê’uï¿½æ“¾
	InquirePosSPT();

	return l_iRc;
}

//	MotSys ï¿½wï¿½è�²ï¿½ÌˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½Xï¿½ï¿½ï¿½bï¿½h
int CStageSPT::MotSys_WaitMotionAxis(LPVOID pParams)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}

	int l_iRc = STAGE_ERR_NONE;
	WORD l_wAxis = LOBYTE(LOWORD(pParams));
	DWORD l_dwTimuOut = HIWORD(pParams) * 1000L;
	DWORD l_dwStartTime = ::GetTickCount();
	while (TRUE == gc_bInMotionMS[l_wAxis]) {
		::Sleep(100);
		// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½
		if (TRUE == CheckIsTimeOut(l_dwStartTime, l_dwTimuOut)) {
			// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
			l_iRc = STAGE_ERR_MS_MOTION_TIMEOUT;
			break;
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// ï¿½ï¿½~ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½ï¿½ï¿½Ç‚ï¿½ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
		switch (gc_dwMotionStop[l_wAxis]) {
		case STAGE_STOP_OK:					// ï¿½ï¿½ï¿½ï¿½ï¿½~
			break;
		case STAGE_STOP_FORCE:				// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½~
			l_iRc = STAGE_ERR_MS_MOTION_FORCE;
			break;
		case STAGE_STOP_FAIL:				// ï¿½Ù�ï¿½ï¿½~
		default:
			l_iRc = STAGE_ERR_MS_MOTION_FAIL;
			break;
		}
	}
	// ï¿½Ê’uï¿½æ“¾
	InquirePosSPT();

	return l_iRc;
}

/* ===========================================================================
 *	ï¿½È‰ï¿½ï¿½ÍŠOï¿½ï¿½ï¿½Ö�ï¿½ï¿½Ì�ï¿½ï¿½Ì‚Å‚ï¿½ï¿½D
 */
//	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[StageInitialize()] --------------------------------------------------------------------
int CStageSPT::Initialize(void)
{
	int l_iRc = STAGE_ERR_NONE;
	if (STAGE_ERR_NONE == l_iRc) {
		// ï¿½Tï¿½uï¿½Rï¿½ï¿½ ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½
		l_iRc = CStageNTN::Execute(&CStageNTN::Initialize);
		m_bIsInitializedStageSTD = (STAGE_ERR_NONE == l_iRc)? TRUE : FALSE;
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½
		l_iRc = InitializeStage();
	}

	return l_iRc;
}

//	XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[StageInitializeStage()] ----------------------------------------------------
int CStageSPT::InitializeStage(void)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}

	int l_iRc = STAGE_ERR_NONE;
	if (STAGE_ERR_NONE == l_iRc) {
		// MotSys ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½
		if (STAGE_ERR_NONE == (l_iRc = CStageNTN::Execute(&CStageNTN::InitializeStage_WaitMotSysInit))) {
			// MotSys ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			m_bMotSysInitialized = TRUE;
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// X ï¿½ï¿½ï¿½ÌŒï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Nï¿½ï¿½
#if 0
		int l_iMSRet;
		gc_bInMotionMS[X] = TRUE;
		if (MS_NO_ERROR != (l_iMSRet = MotsysGoHome(m_hwndMotSys, ConvAxisTo(X)))) {
			gc_bInMotionMS[X] = FALSE;
			switch (l_iMSRet) {
			case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
			case MS_NO_HARDWARE:		l_iRc = STAGE_ERR_MS_NO_HARDWARE;			break;
			case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
			case MS_FUNCTION_BUSY:		l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;			break;
			case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
			case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
			default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
			}
		}
#else
		BOOL l_bServoOn = FALSE;
		DWORD l_dwStartTime = ::GetTickCount();
		while ((FALSE == l_bServoOn) && (STAGE_ERR_NONE == l_iRc)) {
			if (TRUE == CheckIsTimeOut(l_dwStartTime, TIMEOUT_MOTSYS_SRVON)) {
				// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
				l_iRc = STAGE_ERR_MS_NOT_SERVO_ON;
				break;
			}
			gc_bInMotionMS[X] = TRUE;
			int l_iMSRet = MotsysGoHome(m_hwndMotSys, ConvAxisTo(X));
			switch (l_iMSRet) {
			case MS_NO_ERROR:
				l_bServoOn = TRUE;
				break;
			case MS_NO_HARDWARE:
				gc_bInMotionMS[X] = FALSE;
				::Sleep(100);
				break;
			default:
				gc_bInMotionMS[X] = FALSE;
				switch (l_iMSRet) {
				case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
				case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
				case MS_FUNCTION_BUSY:		l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;			break;
				case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
				case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
				default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
				}
				break;
			}
		}
#endif
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// Y ï¿½ï¿½ï¿½ÌŒï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Nï¿½ï¿½
		int l_iMSRet;
		gc_bInMotionMS[Y] = TRUE;
		if (MS_NO_ERROR != (l_iMSRet = MotsysGoHome(m_hwndMotSys, ConvAxisTo(Y)))) {
			gc_bInMotionMS[Y] = FALSE;
			switch (l_iMSRet) {
			case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
			case MS_NO_HARDWARE:		l_iRc = STAGE_ERR_MS_NOT_SERVO_ON;			break;
			case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
			case MS_FUNCTION_BUSY:		l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;			break;
			case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
			case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
			default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
			}
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// X/Y ï¿½ï¿½ï¿½ÌŒï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ÌŠï¿½ï¿½ï¿½ï¿½Ò‚ï¿½
		l_iRc = CStageNTN::Execute(&CStageNTN::MotSys_WaitMotionXY, (LPVOID) MAKELONG(0, TIMEOUT_MOTSYS_ORG));
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// X/Y ï¿½ï¿½ï¿½Ì�Å�ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½æ“¾(ï¿½ï¿½ï¿½ï¿½ï¿½Ï�ï¿½ï¿½É‚Ì‚İ�æ“¾ï¿½Ì‚ï¿½ï¿½ßƒpï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½wï¿½è‚µï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½)
		l_iRc = GetMaxStageSpeed(0, 0);
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// X ï¿½ï¿½ï¿½ï¿½ SoftHP ï¿½ï¿½İ’è‚µï¿½Ü‚ï¿½ï¿½D
		int l_iMSRet;
		if (MS_NO_ERROR != (l_iMSRet = MotsysSetSoftHome(m_hwndMotSys, ConvAxisTo(X), m_lSoftHP[X]))) {
			switch(l_iMSRet) {
			case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
			case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
			case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
			case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
			default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
			}
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// Y ï¿½ï¿½ï¿½ï¿½ SoftHP ï¿½ï¿½İ’è‚µï¿½Ü‚ï¿½ï¿½D
		int l_iMSRet;
		if (MS_NO_ERROR != (l_iMSRet = MotsysSetSoftHome(m_hwndMotSys, ConvAxisTo(Y), m_lSoftHP[Y]))) {
			switch(l_iMSRet) {
			case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
			case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
			case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
			case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
			default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
			}
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// X ï¿½ï¿½ï¿½Ìƒ\ï¿½tï¿½gï¿½ï¿½ï¿½~ï¿½bï¿½gï¿½ï¿½ï¿½æ“¾
		long l_lSoftLimitXP = MotsysGetSoftLimit(m_hwndMotSys, ConvAxisTo(X), 1);
		long l_lSoftLimitXN = MotsysGetSoftLimit(m_hwndMotSys, ConvAxisTo(X), -1);
		switch (l_lSoftLimitXP) {
		case MS_GETPOS_PARAMERROR:	l_iRc = STAGE_ERR_MS_GETPOS_PARAMERROR;		break;
		case MS_GETPOS_AXIS_UNINIT:	l_iRc = STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break;
		default:
			switch (l_lSoftLimitXN) {
			case MS_GETPOS_PARAMERROR:	l_iRc = STAGE_ERR_MS_GETPOS_PARAMERROR;		break;
			case MS_GETPOS_AXIS_UNINIT:	l_iRc = STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break;
			default:
				m_SoftLimitX.Positive = l_lSoftLimitXP;
				m_SoftLimitX.Negative = l_lSoftLimitXN;
				break;
			}
			break;
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// Y ï¿½ï¿½ï¿½Ìƒ\ï¿½tï¿½gï¿½ï¿½ï¿½~ï¿½bï¿½gï¿½ï¿½ï¿½æ“¾
		long l_lSoftLimitYP = MotsysGetSoftLimit(m_hwndMotSys, ConvAxisTo(Y), 1);
		long l_lSoftLimitYN = MotsysGetSoftLimit(m_hwndMotSys, ConvAxisTo(Y), -1);
		switch (l_lSoftLimitYP) {
		case MS_GETPOS_PARAMERROR:	l_iRc = STAGE_ERR_MS_GETPOS_PARAMERROR;		break;
		case MS_GETPOS_AXIS_UNINIT:	l_iRc = STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break;
		default:
			switch (l_lSoftLimitYN) {
			case MS_GETPOS_PARAMERROR:	l_iRc = STAGE_ERR_MS_GETPOS_PARAMERROR;		break;
			case MS_GETPOS_AXIS_UNINIT:	l_iRc = STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break;
			default:
				m_SoftLimitY.Positive = l_lSoftLimitYP;
				m_SoftLimitY.Negative = l_lSoftLimitYN;
				break;
			}
			break;
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		m_bEnableStage = TRUE;
		// X/Y ï¿½ÌˆÊ’uï¿½æ“¾ï¿½Ìƒ|ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
		BeginMonitor();
/* added 2009.05.26 hmenjo XY ï¿½ï¿½ï¿½İ’lï¿½æ“¾ ---------- { ---------- */
		::Sleep(1000);
		InquirePosSPT();
/* added 2009.05.26 hmenjo XY ï¿½ï¿½ï¿½İ’lï¿½æ“¾ ---------- } ---------- */
	}

	return l_iRc;
}
//	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[StageInitializeStage()]ï¿½p MotSys ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½Xï¿½ï¿½ï¿½bï¿½h
#define	MS_INIT_ALLAXIS
int CStageSPT::InitializeStage_WaitMotSysInit(LPVOID)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}

#ifndef MS_INIT_ALLAXIS
	BOOL l_bMotSysInitialized_X = FALSE;
	BOOL l_bMotSysInitialized_Y = FALSE;
#else
	BOOL l_bMotSysInitialized = FALSE;
#endif
	DWORD l_dwStartTime = ::GetTickCount();
#ifndef MS_INIT_ALLAXIS
	while ((TRUE != l_bMotSysInitialized_X) || (TRUE != l_bMotSysInitialized_Y)) {
#else
	while (TRUE != l_bMotSysInitialized) {
#endif
		// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½
		if (TRUE == CheckIsTimeOut(l_dwStartTime, TIMEOUT_MOTSYS_INIT)) {
			// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
			break;
		}

#ifndef MS_INIT_ALLAXIS
		// X ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
		if (TRUE != l_bMotSysInitialized_X) {
			if (MS_NO_ERROR == MotsysSetWindow(m_hwndMotSys, ConvAxisTo(X), m_hwndMotTsk)) {
				l_bMotSysInitialized_X = TRUE;
			}
		}
		// Y ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
		if (TRUE != l_bMotSysInitialized_Y) {
			if (MS_NO_ERROR == MotsysSetWindow(m_hwndMotSys, ConvAxisTo(Y), m_hwndMotTsk)) {
				l_bMotSysInitialized_Y = TRUE;
			}
		}
#else
		if (MS_NO_ERROR == MotsysSetWindow(m_hwndMotSys, AXIS_ALL, m_hwndMotTsk)) {
			l_bMotSysInitialized = TRUE;
		}
#endif
	}

	int l_iRc;
#ifndef MS_INIT_ALLAXIS
	if ((TRUE == l_bMotSysInitialized_X) && (TRUE == l_bMotSysInitialized_Y)) {
#else
	if (TRUE == l_bMotSysInitialized) {
#endif
		l_iRc = STAGE_ERR_NONE;
	} else {
		l_iRc = STAGE_ERR_MS_INIT_TIMEOUT;
	}

	return l_iRc;
}

//	XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ì’†ï¿½Xï¿½eï¿½[ï¿½^ï¿½X[StageIsIdle()] ---------------------------------------------------
int CStageSPT::IsIdle(void)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return FALSE;
	}
	if (TRUE == m_bMotSysInitialized) {
		int l_iResultX = MotsysIsStopped(m_hwndMotSys, ConvAxisTo(X));
		int l_iResultY = MotsysIsStopped(m_hwndMotSys, ConvAxisTo(Y));
		if ((MS_NO_ERROR == l_iResultX) && (MS_NO_ERROR == l_iResultY)) {
			return TRUE;
		}
	}

	return FALSE;
}

//	ï¿½ï¿½ÎˆÊ’uï¿½Ú“ï¿½(ï¿½wï¿½è�²)[StageMoveAbsolute()] ----------------------------------------------------
int CStageSPT::MoveAbsolute(WORD wAxis, long lPos)
{
	return MoveAbsoluteEx(wAxis, lPos, TRUE);
}

//	ï¿½ï¿½ÎˆÊ’uï¿½Ú“ï¿½(XYï¿½ï¿½)[StageMoveAbsolute()] ------------------------------------------------------
int CStageSPT::MoveAbsolute(STAGE_COORD* pPos)
{
	return MoveAbsoluteEx(pPos, TRUE);
}

//	ï¿½ï¿½ÎˆÊ’uï¿½Ú“ï¿½(ï¿½wï¿½è�²ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveAbsoluteEx()] --------------------------------
int CStageSPT::MoveAbsoluteEx(WORD wAxis, long lPos, BOOL bWait/*=TRUE*/)
{
	return MoveAbsoluteAtSpeedEx(wAxis, lPos, 0.0, bWait);
}

//	ï¿½ï¿½ÎˆÊ’uï¿½Ú“ï¿½(XYï¿½ï¿½ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveAbsoluteEx()] ----------------------------------
int CStageSPT::MoveAbsoluteEx(STAGE_COORD* pPos, BOOL bWait/*=TRUE*/)
{
	return MoveAbsoluteAtSpeedEx(pPos, 0.0, bWait);
}

//	ï¿½ï¿½ï¿½ÎˆÊ’uï¿½Ú“ï¿½(ï¿½wï¿½è�²ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveRelative()] ----------------------------------
int CStageSPT::MoveRelative(WORD wAxis, long lPos, BOOL bWait/*=TRUE*/)
{
	return MoveRelativeAtSpeed(wAxis, lPos, 0.0, bWait);
}

//	ï¿½ï¿½ï¿½ÎˆÊ’uï¿½Ú“ï¿½(XYï¿½ï¿½ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveRelative()] ------------------------------------
int CStageSPT::MoveRelative(STAGE_COORD* pPos, BOOL bWait/*=TRUE*/)
{
	return MoveRelativeAtSpeed(pPos, 0.0, bWait);
}

//	XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½~[StageStop()] ---------------------------------------------------------------
int CStageSPT::StageStop(void)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;
	int l_iMSRet = MotsysEmergencyStop(m_hwndMotSys, AXIS_ALL);
	if (MS_NO_ERROR != l_iMSRet) {
		switch(l_iMSRet) {
		case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;		break;
		case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;	break;
		default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;		break;
		}
	}

	return l_iRc;
}

#ifdef _DISABLETHREADSPOS
//	ï¿½ï¿½ï¿½İˆÊ’uï¿½æ“¾(XYï¿½ï¿½)[StageGetPos()] ------------------------------------------------------------
int CStageSPT::GetPos(STAGE_COORD* pPos)
{
	// ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
#if 0
	STAGE_COORD l_Pos;
	PhysicalToLogical(&m_CurrPos, &l_Pos);

	*pPos.lX = l_Pos.lX;
	*pPos.lY = l_Pos.lY;
#else
	*pPos.lX = m_CurrPos.lX;
	*pPos.lY = m_CurrPos.lY;
#endif

	return STAGE_ERR_NONE;
}

//	ï¿½ï¿½ï¿½İˆÊ’uï¿½æ“¾(XYZï¿½ï¿½)[StageGetPos()] -----------------------------------------------------------
int CStageSPT::GetPos(STAGE_COORD_XYZ* pPos)
{
	int l_iRc = STAGE_ERR_NONE;
	STAGE_COORD l_Pos;
	if (STAGE_ERR_NONE == (l_iRc = GetPos(&l_Pos))) {
		long l_PosZ;
		GetElevatorPos(&l_PosZ);
		pPos->lX = l_Pos.lX;
		pPos->lY = l_Pos.lY;
		pPos->lZ = l_PosZ;
	}

	return l_iRc;
}

//	ï¿½ï¿½ï¿½İˆÊ’uï¿½æ“¾(ï¿½wï¿½ï¿½(XY)ï¿½ï¿½)[StageGetPos()] ------------------------------------------------------
int CStageSPT::GetPos(WORD wAxis, long* pPos)
{
	int l_iRc = STAGE_ERR_NONE;
	switch (wAxis) {
	case X:
	case Y:
		{
			STAGE_COORD l_Pos;
			if (STAGE_ERR_NONE == (l_iRc = GetPos(&l_Pos))) {
				*pPos = (X == wAxis)? l_Pos.lX : l_Pos.lY;
			}
		}
		break;
	case Z:
		GetElevatorPos(pPos);
		break;
	default:
		l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;
		break;
	}

	return l_iRc;
}
#endif

//	ï¿½ï¿½ï¿½xï¿½Ú“ï¿½(ï¿½wï¿½è‘¬ï¿½xï¿½Åƒï¿½ï¿½~ï¿½bï¿½gï¿½Ü‚ÅˆÚ“ï¿½)[StageMoveAtSpeed()] -------------------------------------
int CStageSPT::MoveAtSpeed(WORD wAxis, double dSpeed)
{
	return MoveAtSpeedEx(wAxis, dSpeed, TRUE);
}

//	ï¿½Xï¿½eï¿½[ï¿½Wï¿½ÌƒTï¿½Cï¿½Yï¿½ï¿½ï¿½æ“¾[StageGetStageSize()] --------------------------------------------------
int CStageSPT::GetStageSize(long* pStageSizeX, long* pStageSizeY)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;
	long l_iSizeX = MotsysGetMotionSize(m_hwndMotSys, ConvAxisTo(X));
	long l_iSizeY = MotsysGetMotionSize(m_hwndMotSys, ConvAxisTo(Y));
	switch (l_iSizeX) {
	case MS_GETPOS_PARAMERROR:	l_iRc = STAGE_ERR_MS_GETPOS_PARAMERROR;		break;
	case MS_GETPOS_AXIS_UNINIT:	l_iRc = STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break;
	default:
		switch (l_iSizeY) {
		case MS_GETPOS_PARAMERROR:	l_iRc = STAGE_ERR_MS_GETPOS_PARAMERROR;		break;
		case MS_GETPOS_AXIS_UNINIT:	l_iRc = STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break;
		default:
			if (0 != pStageSizeX) {
				*pStageSizeX = ConvCountTo(l_iSizeX, m_lFactor[X]);
				if (*pStageSizeX < 0) {*pStageSizeX *= -1;}
			}
			if (0 != pStageSizeY) {
				*pStageSizeY = ConvCountTo(l_iSizeY, m_lFactor[Y]);
				if (*pStageSizeY < 0) {*pStageSizeY *= -1;}
			}
			break;
		}
		break;
	}

	return l_iRc;
}

//	ï¿½ï¿½ï¿½_ï¿½Ê’uï¿½ï¿½ï¿½æ“¾[StageGetOriginPos()] ----------------------------------------------------------
int CStageSPT::GetOriginPos(STAGE_COORD* pPos)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	pPos->lX = ConvCountTo(m_SoftLimitX.Negative, m_lFactor[X]);
	if (pPos->lX < 0) {pPos->lX *= -1;}
	pPos->lY = ConvCountTo(m_SoftLimitY.Negative, m_lFactor[Y]);
	if (pPos->lY < 0) {pPos->lY *= -1;}

	return STAGE_ERR_NONE;
}

//	XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ì�Å�ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½æ“¾[StageGetMaxStageSpeed()] -----------------------------------------
int CStageSPT::GetMaxStageSpeed(double* x, double* y)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;
	long l_iMaxSpeedX = MotsysGetMaxSpeed(m_hwndMotSys, ConvAxisTo(X));
	long l_iMaxSpeedY = MotsysGetMaxSpeed(m_hwndMotSys, ConvAxisTo(Y));
	switch (l_iMaxSpeedX) {
	case MS_GETPOS_PARAMERROR:	l_iRc = STAGE_ERR_MS_GETPOS_PARAMERROR;		break;
	case MS_GETPOS_AXIS_UNINIT:	l_iRc = STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break;
	default:
		switch (l_iMaxSpeedY) {
		case MS_GETPOS_PARAMERROR:	l_iRc = STAGE_ERR_MS_GETPOS_PARAMERROR;		break;
		case MS_GETPOS_AXIS_UNINIT:	l_iRc = STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break;
		default:
			m_lMaxSpeed[X] = l_iMaxSpeedX;
			if (0 != x) {
				*x = ConvCountTo(l_iMaxSpeedX, m_lFactor[X]);
			}
			m_lMaxSpeed[Y] = l_iMaxSpeedY;
			if (0 != y) {
				*y = ConvCountTo(l_iMaxSpeedY, m_lFactor[Y]);
			}
			break;
		}
		break;
	}

	return l_iRc;
}

//	XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ì‘ï¿½ï¿½xï¿½ï¿½İ’ï¿½[StageSetLocalSpeed()] ------------------------------------------------
void CStageSPT::SetLocalSpeed(double x, double y)
{
	long l_LocalSpeedX = static_cast<long>(x);
	long l_LocalSpeedY = static_cast<long>(y);

	m_lLocalSpeed[X] = ConvCountFrom(l_LocalSpeedX, m_lFactor[X]);
	m_lLocalSpeed[Y] = ConvCountFrom(l_LocalSpeedY, m_lFactor[Y]);

	return;
}

//	ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½~ï¿½bï¿½^ï¿½İ’ï¿½[StageEnableSpeedLimit()] ----------------------------------------------------
void CStageSPT::EnableSpeedLimit(void)
{
	// ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å‚ï¿½ï¿½D???????????????????????????????????????????????????????????????????????????
	//		ï¿½Hï¿½ï¿½ï¿½Æ”[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½Å–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Ü‚Ü‚Æ‚ï¿½ï¿½Ü‚ï¿½ï¿½D[2009.04.20(Mon)]

	return;
}

//	ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½~ï¿½bï¿½^ï¿½ï¿½ï¿½ï¿½[StageDisableSpeedLimit()] ---------------------------------------------------
void CStageSPT::DisableSpeedLimit(void)
{
	// ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å‚ï¿½ï¿½D???????????????????????????????????????????????????????????????????????????
	//		ï¿½Hï¿½ï¿½ï¿½Æ”[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½Å–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Ü‚Ü‚Æ‚ï¿½ï¿½Ü‚ï¿½ï¿½D[2009.04.20(Mon)]

	return;
}

//	MotSys ï¿½Ì•\ï¿½ï¿½/ï¿½ï¿½\ï¿½ï¿½[StageShowMotSys()] ------------------------------------------------------
void CStageSPT::ShowMotSys(BOOL bShow)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return;
	}
	if (0 == bShow) {
		::PostMessage(m_hwndMotSys, MOTDRV_HIDEINDOW, 0, 0);	// ï¿½ï¿½\ï¿½ï¿½
	} else {
		::PostMessage(m_hwndMotSys, MOTDRV_SHOWINDOW, 0, 0);	// ï¿½\ï¿½ï¿½
	}
}

//	ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½N ï¿½Lï¿½ï¿½ï¿½Eï¿½ï¿½ï¿½ï¿½[StageEnableJoyStickSPT()] ----------------------------------------
int CStageSPT::EnableJoystickSPT(BOOL bEnable/*=TRUE*/, int iSpeedSel/*=1*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;
	int l_iMSRet = MotsysSetJoyMode(m_hwndMotSys, bEnable, iSpeedSel);
	if (MS_NO_ERROR != l_iMSRet) {
		switch(l_iMSRet) {
		case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
		case MS_NO_HARDWARE:		l_iRc = STAGE_ERR_MS_NO_HARDWARE;			break;
		case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
		case MS_FUNCTION_BUSY:		l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;			break;
		case MS_AXIS_NOT_HOME:		l_iRc = STAGE_ERR_MS_AXIS_NOT_HOME;			break;
		case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
		default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
		}
	}

	return l_iRc;
}

//	X/Y ï¿½ï¿½ï¿½ï¿½ï¿½~[StageStopAxis()] ----------------------------------------------------------------
int CStageSPT::StageStopAxis(WORD wAxis)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;
	switch (wAxis) {
	case X:
	case Y:
		{
			int l_iMSRet = MotsysEmergencyStop(m_hwndMotSys, ConvAxisTo(wAxis));
			if (MS_NO_ERROR != l_iMSRet) {
				switch(l_iMSRet) {
				case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;		break;
				case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;	break;
				default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;		break;
				}
			}
		}
		break;
	default:
		l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;
		break;
	}

	return l_iRc;
}

//	XY ï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Xï¿½eï¿½[ï¿½^ï¿½X[StageIsAxisIdle()] -----------------------------------------------------
BOOL CStageSPT::IsAxisIdle(WORD wAxis)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return FALSE;
	}
	if (TRUE == m_bMotSysInitialized) {
		if (MS_NO_ERROR == MotsysIsStopped(m_hwndMotSys, ConvAxisTo(wAxis))) {
			return TRUE;
		}
	}

	return FALSE;
}

//	ï¿½ï¿½ÎˆÊ’uï¿½wï¿½è‘¬ï¿½xï¿½Ú“ï¿½(ï¿½wï¿½è�²ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveAbsoluteAtSpeedEx()] -----------------
int CStageSPT::MoveAbsoluteAtSpeedEx(WORD wAxis, long lPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	STAGE_COORD l_AdjEvPos;		// Z ï¿½â�³ï¿½p
	int l_iRc = STAGE_ERR_NONE;
	switch (wAxis) {
	case X:
	case Y:
		{
			// ï¿½wï¿½ï¿½Ê’uï¿½ï¿½ï¿½Î�ï¿½ï¿½Wï¿½É•ÏŠï¿½
			l_iRc = InquirePosSPT();	// ï¿½ï¿½ï¿½Wï¿½ï¿½(ï¿½ï¿½ï¿½ß‚ï¿½)ï¿½æ“¾
			if (STAGE_ERR_NONE == l_iRc) {
				STAGE_COORD l_LogicalPos;
				if (X == wAxis) {
					l_LogicalPos.lX = lPos;
					l_LogicalPos.lY = m_CurrPos.lY;
				} else {
					l_LogicalPos.lX = m_CurrPos.lX;
					l_LogicalPos.lY = lPos;
				}
				STAGE_COORD l_PhysicalPos;
				LogicalToPhysical(&l_LogicalPos, &l_PhysicalPos);				// ï¿½ï¿½Î�ï¿½ï¿½Wï¿½É•ÏŠï¿½
				l_AdjEvPos.lX = l_PhysicalPos.lX; l_AdjEvPos.lY = l_PhysicalPos.lY;		// Z ï¿½â�³ï¿½p
				long l_lPhysicalPos;
				if (X == wAxis) {
					l_lPhysicalPos = l_PhysicalPos.lX;
				} else {
					l_lPhysicalPos = l_PhysicalPos.lY;
				}
			l_lPhysicalPos = ConvCountFrom(l_lPhysicalPos, m_lFactor[wAxis]);
			// ï¿½ï¿½ï¿½xï¿½Ì�İ’ï¿½
			long l_lSpeed = ConvCountFrom(static_cast<long>(dSpeed), m_lFactor[wAxis]);
				l_lSpeed = SpeedAdjustLimit(l_lSpeed, m_lLocalSpeed[wAxis], m_lMaxSpeed[wAxis]);
				// ï¿½Ú“ï¿½ï¿½wï¿½ï¿½
				gc_bInMotionMS[wAxis] = TRUE;
				int l_iMSRet = MotsysMoveToPosition(m_hwndMotSys, ConvAxisTo(wAxis), l_lPhysicalPos, l_lSpeed);
				if (MS_NO_ERROR != l_iMSRet) {
					if (MS_FUNCTION_BUSY != l_iMSRet) {
						gc_bInMotionMS[wAxis] = FALSE;
					}
					switch (l_iMSRet) {
					case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
					case MS_NO_HARDWARE:		l_iRc = STAGE_ERR_MS_NO_HARDWARE;			break;
					case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
					case MS_FUNCTION_BUSY:		l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;			break;
					case MS_AXIS_NOT_HOME:		l_iRc = STAGE_ERR_MS_AXIS_NOT_HOME;			break;
					case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
					case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
					default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
					}
				}
			}
		}
		break;
	default:
		l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;
		break;
	}
	if (STAGE_ERR_NONE == l_iRc) {
		AdjustElevator(FALSE, &l_AdjEvPos);		// Z ï¿½â�³
		if (TRUE == bWait) {
			// ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D
			l_iRc = CStageNTN::Execute(&CStageNTN::MotSys_WaitMotionAxis, (LPVOID) MAKELONG(MAKEWORD(wAxis, 1), TIMEOUT_MOTSYS_MOTION));
		}
	}

	return l_iRc;
}

//	ï¿½ï¿½ÎˆÊ’uï¿½wï¿½è‘¬ï¿½xï¿½Ú“ï¿½(XYï¿½ï¿½ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveAbsoluteAtSpeedEx()] -------------------
int CStageSPT::MoveAbsoluteAtSpeedEx(STAGE_COORD* pPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	// ï¿½wï¿½ï¿½Ê’uï¿½ï¿½ï¿½Î�ï¿½ï¿½Wï¿½É•ÏŠï¿½
	STAGE_COORD l_AdjEvPos;		// Z ï¿½â�³ï¿½p
	STAGE_COORD l_PhysicalPos;
	LogicalToPhysical(pPos, &l_PhysicalPos);				// ï¿½ï¿½Î�ï¿½ï¿½Wï¿½É•ÏŠï¿½
	l_AdjEvPos.lX = l_PhysicalPos.lX; l_AdjEvPos.lY = l_PhysicalPos.lY;		// Z ï¿½â�³ï¿½p
	l_PhysicalPos.lX = ConvCountFrom(l_PhysicalPos.lX, m_lFactor[X]);
	l_PhysicalPos.lY = ConvCountFrom(l_PhysicalPos.lY, m_lFactor[Y]);
	// ï¿½ï¿½ï¿½xï¿½Ì�İ’ï¿½
	long l_lSpeedX = ConvCountFrom(static_cast<long>(dSpeed), m_lFactor[X]);
	long l_lSpeedY = ConvCountFrom(static_cast<long>(dSpeed), m_lFactor[Y]);
	l_lSpeedX = SpeedAdjustLimit(l_lSpeedX, m_lLocalSpeed[X], m_lMaxSpeed[X]);
	l_lSpeedY = SpeedAdjustLimit(l_lSpeedY, m_lLocalSpeed[Y], m_lMaxSpeed[Y]);
	int l_iRc = STAGE_ERR_NONE;
	if (STAGE_ERR_NONE == l_iRc) {
		if ((TRUE == gc_bInMotionMS[X]) || (TRUE == gc_bInMotionMS[Y])) {
			l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;
		} else {
			gc_bInMotionMS[X] = TRUE;
			int l_iMSRetX = MotsysMoveToPosition(m_hwndMotSys, ConvAxisTo(X), l_PhysicalPos.lX, l_lSpeedX);
			if (MS_NO_ERROR != l_iMSRetX) {
				gc_bInMotionMS[X] = FALSE;
				switch (l_iMSRetX) {
				case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
				case MS_NO_HARDWARE:		l_iRc = STAGE_ERR_MS_NO_HARDWARE;			break;
				case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
				case MS_FUNCTION_BUSY:		l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;			break;
				case MS_AXIS_NOT_HOME:		l_iRc = STAGE_ERR_MS_AXIS_NOT_HOME;			break;
				case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
				case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
				default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
				}
			}
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		gc_bInMotionMS[Y] = TRUE;
		int l_iMSRetY = MotsysMoveToPosition(m_hwndMotSys, ConvAxisTo(Y), l_PhysicalPos.lY, l_lSpeedY);
		if (MS_NO_ERROR != l_iMSRetY) {
			gc_bInMotionMS[Y] = FALSE;
			switch (l_iMSRetY) {
			case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
			case MS_NO_HARDWARE:		l_iRc = STAGE_ERR_MS_NO_HARDWARE;			break;
			case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
			case MS_FUNCTION_BUSY:		l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;			break;
			case MS_AXIS_NOT_HOME:		l_iRc = STAGE_ERR_MS_AXIS_NOT_HOME;			break;
			case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
			case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
			default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
			}
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		AdjustElevator(FALSE, &l_AdjEvPos);		// Z ï¿½â�³
		if (TRUE == bWait) {
			// ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D
			l_iRc = CStageNTN::Execute(&CStageNTN::MotSys_WaitMotionXY, (LPVOID) MAKELONG(MAKEWORD(0, 0), TIMEOUT_MOTSYS_MOTION));
		}
	}

	return l_iRc;
}

//	ï¿½ï¿½ï¿½ÎˆÊ’uï¿½wï¿½è‘¬ï¿½xï¿½Ú“ï¿½(ï¿½wï¿½è�²ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveRelativeAtSpeed()] -------------------
int CStageSPT::MoveRelativeAtSpeed(WORD wAxis, long lPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;
	switch (wAxis) {
	case X:
	case Y:
		{
			// ï¿½ï¿½Î�ï¿½ï¿½Wï¿½ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½ÄƒIï¿½tï¿½Zï¿½bï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Z
			l_iRc = InquirePosSPT();	// ï¿½ï¿½ï¿½Wï¿½ï¿½(ï¿½ï¿½ï¿½ß‚ï¿½)ï¿½æ“¾
			if (STAGE_ERR_NONE == l_iRc) {
				long l_lLogicalPos;
				if (X == wAxis) {
					l_lLogicalPos = m_CurrPos.lX + lPos;
				} else {
					l_lLogicalPos = m_CurrPos.lY + lPos;
				}
				// ï¿½Ú“ï¿½ï¿½wï¿½ï¿½
				l_iRc = MoveAbsoluteAtSpeedEx(wAxis, l_lLogicalPos, dSpeed, bWait);
			}
		}
		break;
	default:
		l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;
		break;
	}

	return l_iRc;
}

//	ï¿½ï¿½ï¿½ÎˆÊ’uï¿½wï¿½è‘¬ï¿½xï¿½Ú“ï¿½(XYï¿½ï¿½ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveRelativeAtSpeed()] ---------------------
int CStageSPT::MoveRelativeAtSpeed(STAGE_COORD* pPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	// ï¿½ï¿½Î�ï¿½ï¿½Wï¿½ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½ÄƒIï¿½tï¿½Zï¿½bï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Z
	int l_iRc = InquirePosSPT();	// ï¿½ï¿½ï¿½Wï¿½ï¿½(ï¿½ï¿½ï¿½ß‚ï¿½)ï¿½æ“¾
	if (STAGE_ERR_NONE == l_iRc) {
		STAGE_COORD l_LogicalPos;
		l_LogicalPos.lX = m_CurrPos.lX + pPos->lX;
		l_LogicalPos.lY = m_CurrPos.lY + pPos->lY;
		l_iRc = MoveAbsoluteAtSpeedEx(&l_LogicalPos, dSpeed, bWait);
	}

	return l_iRc;
}

//	SPT ï¿½p ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½Ô�æ“¾[StageGetJoystickSPT()] ----------------------------------------
BOOL CStageSPT::GetJoyStickSPT(void)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return TRUE;
	}

	BOOL l_bRet = TRUE;
	int l_iRc = MotsysIsJoyMode(m_hwndMotSys);
	switch (l_iRc) {
	case MS_NO_ERROR:
		l_bRet = FALSE;
		break;
	case MS_JOYMODE:
	case MS_AXIS_UNINITIALIZED:
	case MS_NO_FUNCTION:
	default:
		l_bRet = TRUE;
		break;
	}

	return l_bRet;
}

//	ï¿½wï¿½è‘¬ï¿½xï¿½Ú“ï¿½(ï¿½wï¿½è�²ï¿½Fï¿½Eï¿½Fï¿½Cï¿½gï¿½ï¿½ï¿½ï¿½tï¿½ï¿½)[StageMoveAtSpeedEx()] ----------------------------------
int CStageSPT::MoveAtSpeedEx(WORD wAxis, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;
	switch (wAxis) {
	case X:
	case Y:
		{
		// ï¿½ï¿½ï¿½xï¿½Ì�İ’ï¿½
		long l_lSpeed = ConvCountFrom(static_cast<long>(dSpeed), m_lFactor[wAxis]);
			l_lSpeed = SpeedAdjustLimit(l_lSpeed, m_lLocalSpeed[wAxis], m_lMaxSpeed[wAxis]);
			// ï¿½Ú“ï¿½ï¿½wï¿½ï¿½
			gc_bInMotionMS[wAxis] = TRUE;
			int l_iMSRet = MotsysMoveAtSpeed(m_hwndMotSys, ConvAxisTo(wAxis), l_lSpeed, 0);
			if (MS_NO_ERROR != l_iMSRet) {
				if (MS_FUNCTION_BUSY != l_iMSRet) {
					gc_bInMotionMS[wAxis] = FALSE;
				}
				switch (l_iMSRet) {
				case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
				case MS_NO_HARDWARE:		l_iRc = STAGE_ERR_MS_NO_HARDWARE;			break;
				case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
				case MS_FUNCTION_BUSY:		l_iRc = STAGE_ERR_MS_FUNCTION_BUSY;			break;
				case MS_AXIS_NOT_HOME:		l_iRc = STAGE_ERR_MS_AXIS_NOT_HOME;			break;
				case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
				case MS_JOYMODE:			l_iRc = STAGE_ERR_MS_JOYMODE;				break;
				default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
				}
			}
		}
		break;
	default:
		l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;
		break;
	}
	if (STAGE_ERR_NONE == l_iRc) {
		if (TRUE == bWait) {
			// ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D
			l_iRc = CStageNTN::Execute(&CStageNTN::MotSys_WaitMotionAxis, (LPVOID) MAKELONG(MAKEWORD(wAxis, 1), TIMEOUT_MOTSYS_MOTION));
		}
	}

	return l_iRc;
}

//	SPT ï¿½p ï¿½ï¿½ï¿½Ì“dï¿½qï¿½Mï¿½Aï¿½ï¿½ï¿½æ“¾[StageGetElectronicGear()] -------------------------------------------
//		(ï¿½ï¿½ï¿½ï¿½FLOWORD(*plEleGearAB)ï¿½Cï¿½ï¿½ï¿½qï¿½FHIWORD(*plEleGearAB)
//		ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^()ï¿½Å�İ’è‚³ï¿½ê‚½ï¿½ï¿½ï¿½eï¿½Å‚ï¿½ï¿½D
//		ï¿½ï¿½ï¿½ï¿½ï¿½Å‚Í�İ’ï¿½oï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ñ‚ª�Cï¿½İ’ï¿½ÍˆÍ‚Í�F
//			ï¿½ï¿½ï¿½ï¿½Fï¿½Kï¿½è–³ï¿½ï¿½
//			ï¿½ï¿½ï¿½ï¿½ï¿½F1/32 ï¿½Ü‚ï¿½
BOOL CStageSPT::GetElectronicGear(WORD wAxis, long* plEleGearAB)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return FALSE;
	}

	BOOL l_bRet;
	int l_iRc = MotsysGetElectronicGear(m_hwndMotSys, ConvAxisTo(wAxis));
	switch (l_iRc) {
	case MS_GETPOS_PARAMERROR:
		l_bRet = FALSE;
		break;
	default:
		*plEleGearAB = l_iRc;
		l_bRet = TRUE;
		break;
	}

	return l_bRet;
}

/* ï¿½ï¿½ï¿½Ì’Pï¿½Ê•ÏŠï¿½ï¿½Wï¿½ï¿½ï¿½ï¿½İ’ï¿½
 *		lFactor[count/ï¿½wï¿½ß’Pï¿½ï¿½]
 *			ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ SPT ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ì“dï¿½qï¿½Mï¿½Aï¿½ï¿½ÅŒï¿½ï¿½Ü‚ï¿½Ü‚ï¿½ï¿½D
 *			SPT ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ì�ï¿½ï¿½tï¿½Bï¿½[ï¿½hï¿½oï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ 156.25[nm]ï¿½È‚Ì‚Å�C
 *			ï¿½dï¿½qï¿½Mï¿½Aï¿½ä‚ª 8/25 ï¿½Ì�ê�‡ï¿½Cï¿½ï¿½ï¿½Lï¿½Ì�wï¿½ß•ï¿½ï¿½ï¿½\ï¿½É‚È‚ï¿½Ü‚ï¿½ï¿½D
 *				0.050[um/count]
 *			ï¿½dï¿½qï¿½Mï¿½Aï¿½ï¿½ï¿½ SPT ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Å�İ’è‚³ï¿½ï¿½Ü‚ï¿½ï¿½D
 *			ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½F
 *				ï¿½wï¿½ß•ï¿½ï¿½ï¿½\ï¿½ï¿½ 0.050[um/count]ï¿½Ì�ê�‡ï¿½F
 *					ï¿½wï¿½ß’Pï¿½Ê‚ï¿½[um]ï¿½É‚ï¿½ï¿½ï¿½ê�‡ï¿½C20[count/ï¿½wï¿½ß’Pï¿½ï¿½]ï¿½Å‚ï¿½ï¿½D
 *					ï¿½wï¿½ß’Pï¿½Ê‚ï¿½[0.1um]ï¿½É‚ï¿½ï¿½ï¿½ê�‡ï¿½C2[count/ï¿½wï¿½ß’Pï¿½ï¿½]ï¿½Å‚ï¿½ï¿½D*/
BOOL CStageSPT::SetFactor(WORD wAxis, long lFactor)
{
	BOOL l_bRc = TRUE;
	switch (wAxis) {
	case X:
	case Y:
		m_lFactor[wAxis] = lFactor;
		break;
	default:
		l_bRc = FALSE;
		break;
	}

	return l_bRc;
}

/* ï¿½Xï¿½eï¿½[ï¿½Wï¿½ÌŒï¿½ï¿½_ï¿½Ê’uï¿½Iï¿½tï¿½Zï¿½bï¿½gï¿½ï¿½ï¿½æ“¾
		ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ì�|ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½çŒ´ï¿½_(ï¿½Oï¿½_)ï¿½Ü‚Å‚Ì‹ï¿½ï¿½ï¿½ï¿½Å‚ï¿½ï¿½D */
int CStageSPT::GetOrgLoc(long* plStageOrgLocX, long* plStageOrgLocY)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;

	STAGE_COORD l_OrgPos;
	if (STAGE_ERR_NONE == l_iRc) {
		l_iRc = GetOriginPos(&l_OrgPos);
	}
	STAGE_COORD l_StageSize;
	if (STAGE_ERR_NONE == l_iRc) {
		l_iRc = GetStageSize(&l_StageSize.lX, &l_StageSize.lY);
	}
	if (STAGE_ERR_NONE == l_iRc) {
		if (m_lFactor[X] < 0) {
			// ï¿½tï¿½]
			*plStageOrgLocX = l_StageSize.lX - l_OrgPos.lX;
		} else {
			// ï¿½ï¿½ï¿½]
			*plStageOrgLocX = l_OrgPos.lX;
		}
		if (m_lFactor[Y] < 0) {
			// ï¿½tï¿½]
			*plStageOrgLocY = l_StageSize.lY - l_OrgPos.lY;
		} else {
			// ï¿½ï¿½ï¿½]
			*plStageOrgLocY = l_OrgPos.lY;
		}
	}

	return STAGE_ERR_NONE;
}

/* ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ì‘ï¿½ï¿½xï¿½ï¿½İ’ï¿½ */
int CStageSPT::SetJoyStickSpeedSPT(WORD wAxis, int iSpeedSel, double dSpeed)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	int l_iRc = STAGE_ERR_NONE;
	switch (wAxis) {
	case X:
	case Y:
		{
		// ï¿½ï¿½ï¿½xï¿½Ì�İ’ï¿½
		long l_lSpeed = ConvCountFrom(static_cast<long>(dSpeed), m_lFactor[wAxis]);
			l_lSpeed = SpeedAdjustLimit(l_lSpeed, m_lLocalSpeed[wAxis], m_lMaxSpeed[wAxis]);
			int l_iMSRet = MotsysSetJoySpeed(m_hwndMotSys, ConvAxisTo(wAxis), iSpeedSel, l_lSpeed);
			if (MS_NO_ERROR != l_iMSRet) {
				switch (l_iMSRet) {
				case MS_NO_FUNCTION:		l_iRc = STAGE_ERR_MS_NO_FUNCTION;			break;
				case MS_NO_HARDWARE:		l_iRc = STAGE_ERR_MS_NO_HARDWARE;			break;
				case MS_PARAMETER_ERROR:	l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;		break;
				case MS_AXIS_UNINITIALIZED:	l_iRc = STAGE_ERR_MS_AXIS_UNINITIALIZED;	break;
				default:					l_iRc = STAGE_ERR_MS_UNDEFINE_ERR;			break;
				}
			}
		}
		break;
	default:
		l_iRc = STAGE_ERR_MS_PARAMETER_ERROR;
		break;
	}
	return l_iRc;
}
