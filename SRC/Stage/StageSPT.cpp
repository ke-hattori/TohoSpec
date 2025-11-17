
#include "stage.h"
#include "StageSPT.h"

#define	MotMsgON
#include "..\\..\\inc\\MotsysMsg.h"
#include "..\\..\\inc\\MotSys.h"

#define	TIMEOUT_CREATEPROC		5000L	// [ms]：プロセス起動待ち タイムアウト
#define	TIMEOUT_MOTSYS_INIT		60000L	// [ms]：MotSys 初期化完了待ち タイムアウト
#define	TIMEOUT_MOTSYS_SRVON	5000L	// [ms]：MotSys 初期化時の原点復帰でのサーボオン待ち タイムアウト
#define	TIMEOUT_MOTSYS_ORG		600		// [s] ：MotSys 原点復帰完了待ち タイムアウト(実機で調整が必要です)
/* modified 2009.11.11 hmenjo MotSys 軸タイムアウト ---------- { ---------- */
//#define	TIMEOUT_MOTSYS_MOTION	300		// [s] ：MotSys 通常移動完了待ち タイムアウト(実機で調整が必要です)
/* modified 2009.11.11 hmenjo MotSys 軸タイムアウト ----------				*/
#define	TIMEOUT_MOTSYS_MOTION	600		// [s] ：MotSys 通常移動完了待ち タイムアウト(実機で調整が必要です)
/* modified 2009.11.11 hmenjo MotSys 軸タイムアウト ---------- } ---------- */

#define	MOTTSK_WINDOW_NAME	_T("Motion Task")
#define	MOTSYS_WINDOW_NAME	_T("Motion System Driver")
#define	MSGBOX_TITLE_STAGE_DLL	_T("Stage.dll")

/* ===========================================================================
 *	単純関数定義です．
 */
//	タイムアウトチェック関数
BOOL CheckIsTimeOut(
		DWORD dwStartTime,	// 開始時刻[ms]
		DWORD dwTimeOut		// タイムアウト時間[ms]
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
		// タイムアウトにします．
		return TRUE;
	} else {
		return FALSE;
	}
}

//	MotSys へ軸番号を変換
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
//	MotSys から軸番号を変換
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

//	指定速度がローカル速度あるいは最高速度を超えないようにします．
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
		// 指定速度が０の場合，ローカル速度にします．
		l_lSpeed = l_lLocalSpeed;
		if (0 == l_lSpeed) {
			// ローカル速度が０の場合，最高速度にします．
			l_lSpeed = l_lMaxSpeed;
			if (0 == l_lSpeed) {
				// 最高速度が０の場合，速度値を１０にします．(ここには来ないはずです)
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

//	指令値からカウントへ変換
long ConvCountFrom(long lValue, long lFactor)
{
	long l_lCount = lValue * lFactor;

	return l_lCount;
}
//	指令値へカウントから変換
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
	l_l64Value2 = (l_l64Value1 + l_l64_500) / l_l64_1000;	// 四捨五入

	return l_l64Value2;
}

// =========================================================================
// Class Name :  CStageSPT
//
// Description： Sodick ステージラッパクラス
//					Sodick の XY ステージの制御を行います．
//
// Author:		 hmenjo
// =========================================================================

// =========================================================================
//	コンストラクタ / デストラクタ
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
		m_lFactor[i] = 1;	// 座標変換係数は１にしておきます．
	}
	m_SoftLimitX.Positive = m_SoftLimitX.Negative = 0;
	m_SoftLimitY.Positive = m_SoftLimitY.Negative = 0;

	::InitializeCriticalSection(&m_csInquirePos);

	BOOL l_bResult;
	// MotTsk を起動
	l_bResult = MotTskExe(EXE_START);
	if (0 == l_bResult) {
		// 起動失敗
		::MessageBox(0, _T("CStageSPT\nFailed to start MotTsk.exe."), MSGBOX_TITLE_STAGE_DLL, MB_OK | MB_SYSTEMMODAL);
	} else {
		l_bResult = MotSysExe(EXE_START);
		if (0 == l_bResult) {
			// 起動失敗
			::MessageBox(0, _T("CStageSPT\nFailed to start MotSys.exe."), MSGBOX_TITLE_STAGE_DLL, MB_OK | MB_SYSTEMMODAL);
		} else {
			m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME);
			m_hwndMotTsk = ::FindWindow(0, MOTTSK_WINDOW_NAME);
#if 0	// ここ(以下)は不要になりました．
			/*
			 *	XY ステージ以外は STAGETYPE_STD を使います．
			 *	したがって，CStageNTN が必要です．
			 */
			m_pStageSTD = new CStageNTN();	/*
									 			このクラスのコンストラクタでは
									 			サブコンからのデータの読出しを
									 			行っています．時間がかかります．
											 */
			if (0 == m_pStageSTD) {
				// 生成失敗
				::MessageBox(0, _T("CStageSPT\nFailed to create(new) CStageNTN."), MSGBOX_TITLE_STAGE_DLL, MB_OK | MB_SYSTEMMODAL);
			}
#endif
		}
	}

}
CStageSPT::~CStageSPT()
{
	// MotSys を終了
	m_hwndMotSys = 0;
	MotSysExe(EXE_QUIT);

	// MotTsk を終了
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
 *	以下は制御，その他の関数です．
 */
//	MotTsk の起動/終了の制御
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
/* added 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- { ---------- */
			TCHAR l_tszExePath[_MAX_PATH];
			_stprintf(l_tszExePath, _T("%s") BIN_DIR _T("MotTsk.exe"), g_tszProcDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- } ---------- */
			l_bRet = ::CreateProcess(
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- { ---------- */
//							BIN_DIR _T("\\MotTsk.exe"),
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- 			 */
							l_tszExePath,
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- } ---------- */
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
					// ウィンドウが生成されるまで待ちます．
					//	(ちょっと強引なやり方です)
					::Sleep(100);
					// タイムアウト監視
					if (TRUE == CheckIsTimeOut(l_dwStartTime, TIMEOUT_CREATEPROC)) {
						// タイムアウトにします．
						l_bRet = FALSE;
						break;
					}
				}
			}
		} else {
			// すでに起動していました．
			l_bRet = TRUE;
		}
		break;
#if 0
	case EXE_SHOW:
		if (l_hWnd != NULL) {
			l_bRet = ::ShowWindow(l_hWnd, SW_SHOWNORMAL);
		} else {
			// ウィンドウがなかった．
			l_bRet = FALSE;
		}
		break;
	case EXE_HIDE:
		if (l_hWnd != NULL) {
			l_bRet = ::ShowWindow(l_hWnd, SW_HIDE);
		} else {
			// ウィンドウがなかった．
			l_bRet = FALSE;
		}
		break;
#endif
	case EXE_QUIT:
		if (l_hWnd != NULL) {
			l_bRet = ::PostMessage(l_hWnd, WM_CLOSE, NULL, NULL);
		} else {
			// ウィンドウがなかった．
			l_bRet = FALSE;
		}
		break;
	default:
		l_bRet = FALSE;
		break;
	}

	return l_bRet;
}

//	MotSys の起動/終了の制御
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
/* added 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- { ---------- */
			TCHAR l_tszExePath[_MAX_PATH];
			_stprintf(l_tszExePath, _T("%s") BIN_DIR _T("MotSysNSPT.exe"), g_tszProcDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- } ---------- */
			l_bRet = ::CreateProcess(
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- { ---------- */
//							BIN_DIR _T("\\MotSysNSPT.exe"),
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- 			 */
							l_tszExePath,
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- } ---------- */
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
					// ウィンドウが生成されるまで待ちます．
					//	(ちょっと強引なやり方です)
					::Sleep(100);
					// タイムアウト監視
					if (TRUE == CheckIsTimeOut(l_dwStartTime, TIMEOUT_CREATEPROC)) {
						// タイムアウトにします．
						l_bRet = FALSE;
						break;
					}
				}
			}
		} else {
			// すでに起動していました．
			l_bRet = TRUE;
		}
		break;
	case EXE_QUIT:
		if (l_hWnd != NULL) {
			l_bRet = ::PostMessage(l_hWnd, WM_CLOSE, NULL, NULL);
		} else {
			// ウィンドウがなかった．
			l_bRet = FALSE;
		}
		break;
	default:
		l_bRet = FALSE;
		break;
	}

	return l_bRet;
}

//	StageSTD の初期化確認
BOOL CStageSPT::IsInitializedStageSTD(void)
{
	return m_bIsInitializedStageSTD;
}

//	現在位置 取得 スレッド(未使用)
UINT CStageSPT::MonitorThreadProcSPT(void)
{
	while (0 != m_bMonitorRun) {
		InquirePosSPT();

		::Sleep(100);
	}
	return 0L;
}

//	現在位置 取得
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
	case MS_GETPOS_PARAMERROR:		// パラメタエラー(MOTDRV_GETPOSITION 用)
		return STAGE_ERR_MS_PARAMETER_ERROR;
		break;
	case MS_GETPOS_AXIS_UNINIT:		// 未初期化(MOTDRV_GETPOSITION 用)
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
		break;
	case MS_GETPOS_AXIS_NOTHOME:	// 未原点復帰(MOTDRV_GETPOSITION 用)
		return STAGE_ERR_MS_AXIS_NOT_HOME;
		break;
	default:
		switch (l_GetPos.lY) {
		case MS_GETPOS_PARAMERROR:		// パラメタエラー(MOTDRV_GETPOSITION 用)
			return STAGE_ERR_MS_PARAMETER_ERROR;
			break;
		case MS_GETPOS_AXIS_UNINIT:		// 未初期化(MOTDRV_GETPOSITION 用)
			return STAGE_ERR_MS_AXIS_UNINITIALIZED;
			break;
		case MS_GETPOS_AXIS_NOTHOME:	// 未原点復帰(MOTDRV_GETPOSITION 用)
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
				// SPT では，MotSys 座標(絶対座標)で管理します．
				//		UI の際に論理座標と変換します．

				::LeaveCriticalSection(&m_csInquirePos);
			}
			break;
		}
		break;
	}


	return STAGE_ERR_NONE;
}

//	MotSys X/Y 軸の移動完了待ち スレッド
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
		// タイムアウト監視
		if (TRUE == CheckIsTimeOut(l_dwStartTime, l_dwTimuOut)) {
			// タイムアウトにします．
			l_iRc = STAGE_ERR_MS_MOTION_TIMEOUT;
			break;
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// 停止していた場合，正常停止かどうか確認します．
		switch (gc_dwMotionStop[X]) {
		case STAGE_STOP_OK:							// X 軸 正常停止
			switch (gc_dwMotionStop[Y]) {
			case STAGE_STOP_OK:							// Y 軸 正常停止
				break;
			case STAGE_STOP_FORCE:
				l_iRc = STAGE_ERR_MS_MOTION_FORCE;
				break;									// Y 軸 強制停止:
			case STAGE_STOP_FAIL:
			default:
				l_iRc = STAGE_ERR_MS_MOTION_FAIL;
				break;									// Y 軸 異常停止:
			}
			break;
		case STAGE_STOP_FORCE:						// X 軸 強制停止
			l_iRc = STAGE_ERR_MS_MOTION_FORCE;
			break;
		case STAGE_STOP_FAIL:						// X 軸 異常停止
		default:
			l_iRc = STAGE_ERR_MS_MOTION_FAIL;
			break;
		}
	}
	// 位置取得
	InquirePosSPT();

	return l_iRc;
}

//	MotSys 指定軸の移動完了待ち スレッド
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
		// タイムアウト監視
		if (TRUE == CheckIsTimeOut(l_dwStartTime, l_dwTimuOut)) {
			// タイムアウトにします．
			l_iRc = STAGE_ERR_MS_MOTION_TIMEOUT;
			break;
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// 停止していた場合，正常停止かどうか確認します．
		switch (gc_dwMotionStop[l_wAxis]) {
		case STAGE_STOP_OK:					// 正常停止
			break;
		case STAGE_STOP_FORCE:				// 強制停止
			l_iRc = STAGE_ERR_MS_MOTION_FORCE;
			break;
		case STAGE_STOP_FAIL:				// 異常停止
		default:
			l_iRc = STAGE_ERR_MS_MOTION_FAIL;
			break;
		}
	}
	// 位置取得
	InquirePosSPT();

	return l_iRc;
}

/* ===========================================================================
 *	以下は外部関数の実体です．
 */
//	初期化[StageInitialize()] --------------------------------------------------------------------
int CStageSPT::Initialize(void)
{
	int l_iRc = STAGE_ERR_NONE;
	if (STAGE_ERR_NONE == l_iRc) {
		// サブコン の初期化＆完了待ち
		l_iRc = CStageNTN::Execute(&CStageNTN::Initialize);
		m_bIsInitializedStageSTD = (STAGE_ERR_NONE == l_iRc)? TRUE : FALSE;
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// XY ステージ初期化＆完了待ち
		l_iRc = InitializeStage();
	}

	return l_iRc;
}

//	XY ステージ初期化[StageInitializeStage()] ----------------------------------------------------
int CStageSPT::InitializeStage(void)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}

	int l_iRc = STAGE_ERR_NONE;
	if (STAGE_ERR_NONE == l_iRc) {
		// MotSys の初期化完了待ち
		if (STAGE_ERR_NONE == (l_iRc = CStageNTN::Execute(&CStageNTN::InitializeStage_WaitMotSysInit))) {
			// MotSys の初期化が完了
			m_bMotSysInitialized = TRUE;
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// X 軸の原点復帰を起動
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
				// タイムアウトにします．
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
		// Y 軸の原点復帰を起動
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
		// X/Y 軸の原点復帰の完了待ち
		l_iRc = CStageNTN::Execute(&CStageNTN::MotSys_WaitMotionXY, (LPVOID) MAKELONG(0, TIMEOUT_MOTSYS_ORG));
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// X/Y 軸の最高速度を取得(内部変数にのみ取得のためパラメタを指定していません)
		l_iRc = GetMaxStageSpeed(0, 0);
	}
	if (STAGE_ERR_NONE == l_iRc) {
		// X 軸の SoftHP を設定します．
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
		// Y 軸の SoftHP を設定します．
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
		// X 軸のソフトリミットを取得
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
		// Y 軸のソフトリミットを取得
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
		// X/Y の位置取得のポーリングを開始します．
		BeginMonitor();
/* added 2009.05.26 hmenjo XY 現在値取得 ---------- { ---------- */
		::Sleep(1000);
		InquirePosSPT();
/* added 2009.05.26 hmenjo XY 現在値取得 ---------- } ---------- */
	}

	return l_iRc;
}
//	初期化[StageInitializeStage()]用 MotSys の初期化完了待ち スレッド
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
		// タイムアウト監視
		if (TRUE == CheckIsTimeOut(l_dwStartTime, TIMEOUT_MOTSYS_INIT)) {
			// タイムアウトにします．
			break;
		}

#ifndef MS_INIT_ALLAXIS
		// X 軸の初期化チェック
		if (TRUE != l_bMotSysInitialized_X) {
			if (MS_NO_ERROR == MotsysSetWindow(m_hwndMotSys, ConvAxisTo(X), m_hwndMotTsk)) {
				l_bMotSysInitialized_X = TRUE;
			}
		}
		// Y 軸の初期化チェック
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

//	XY ステージ動作中ステータス[StageIsIdle()] ---------------------------------------------------
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

//	絶対位置移動(指定軸)[StageMoveAbsolute()] ----------------------------------------------------
int CStageSPT::MoveAbsolute(WORD wAxis, long lPos)
{
	return MoveAbsoluteEx(wAxis, lPos, TRUE);
}

//	絶対位置移動(XY軸)[StageMoveAbsolute()] ------------------------------------------------------
int CStageSPT::MoveAbsolute(STAGE_COORD* pPos)
{
	return MoveAbsoluteEx(pPos, TRUE);
}

//	絶対位置移動(指定軸：ウェイト制御付き)[StageMoveAbsoluteEx()] --------------------------------
int CStageSPT::MoveAbsoluteEx(WORD wAxis, long lPos, BOOL bWait/*=TRUE*/)
{
	return MoveAbsoluteAtSpeedEx(wAxis, lPos, 0.0, bWait);
}

//	絶対位置移動(XY軸：ウェイト制御付き)[StageMoveAbsoluteEx()] ----------------------------------
int CStageSPT::MoveAbsoluteEx(STAGE_COORD* pPos, BOOL bWait/*=TRUE*/)
{
	return MoveAbsoluteAtSpeedEx(pPos, 0.0, bWait);
}

//	相対位置移動(指定軸：ウェイト制御付き)[StageMoveRelative()] ----------------------------------
int CStageSPT::MoveRelative(WORD wAxis, long lPos, BOOL bWait/*=TRUE*/)
{
	return MoveRelativeAtSpeed(wAxis, lPos, 0.0, bWait);
}

//	相対位置移動(XY軸：ウェイト制御付き)[StageMoveRelative()] ------------------------------------
int CStageSPT::MoveRelative(STAGE_COORD* pPos, BOOL bWait/*=TRUE*/)
{
	return MoveRelativeAtSpeed(pPos, 0.0, bWait);
}

//	XY ステージを停止[StageStop()] ---------------------------------------------------------------
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
//	現在位置取得(XY軸)[StageGetPos()] ------------------------------------------------------------
int CStageSPT::GetPos(STAGE_COORD* pPos)
{
	// 論理座標に変換します．
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

//	現在位置取得(XYZ軸)[StageGetPos()] -----------------------------------------------------------
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

//	現在位置取得(指定(XY)軸)[StageGetPos()] ------------------------------------------------------
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

//	速度移動(指定速度でリミットまで移動)[StageMoveAtSpeed()] -------------------------------------
int CStageSPT::MoveAtSpeed(WORD wAxis, double dSpeed)
{
	return MoveAtSpeedEx(wAxis, dSpeed, TRUE);
}

//	ステージのサイズを取得[StageGetStageSize()] --------------------------------------------------
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

//	原点位置を取得[StageGetOriginPos()] ----------------------------------------------------------
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

//	XY ステージの最高速度を取得[StageGetMaxStageSpeed()] -----------------------------------------
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
		case MS_GETPOS_PARAMERROR:	l_iRc = static_cast<long>(STAGE_ERR_MS_GETPOS_PARAMERROR;		break);
		case MS_GETPOS_AXIS_UNINIT:	l_iRc = static_cast<long>(STAGE_ERR_MS_GETPOS_AXIS_UNINIT;	break);
		default:
			m_lMaxSpeed[X] = static_cast<long>(l_iMaxSpeedX);
			if (0 != x) {
				*x = static_cast<long>(ConvCountTo(l_iMaxSpeedX, m_lFactor[X]));
			}
			m_lMaxSpeed[Y] = static_cast<long>(l_iMaxSpeedY);
			if (0 != y) {
				*y = ConvCountTo(l_iMaxSpeedY, m_lFactor[Y]);
			}
			break;
		}
		break;
	}

	return l_iRc;
}

//	XY ステージの速度を設定[StageSetLocalSpeed()] ------------------------------------------------
void CStageSPT::SetLocalSpeed(double x, double y)
{
	long l_LocalSpeedX = static_cast<long>(static_cast<long>(x));
	long l_LocalSpeedY = static_cast<long>(static_cast<long>(y));

	m_lLocalSpeed[X] = static_cast<long>(ConvCountFrom(l_LocalSpeedX, m_lFactor[X]));
	m_lLocalSpeed[Y] = static_cast<long>(ConvCountFrom(l_LocalSpeedY, m_lFactor[Y]));

	return;
}

//	速度リミッタ設定[StageEnableSpeedLimit()] ----------------------------------------------------
void CStageSPT::EnableSpeedLimit(void)
{
	// まだ未実装です．???????????????????????????????????????????????????????????????????????????
	//		工数と納期の問題で未実装のままとします．[2009.04.20(Mon)]

	return;
}

//	速度リミッタ解除[StageDisableSpeedLimit()] ---------------------------------------------------
void CStageSPT::DisableSpeedLimit(void)
{
	// まだ未実装です．???????????????????????????????????????????????????????????????????????????
	//		工数と納期の問題で未実装のままとします．[2009.04.20(Mon)]

	return;
}

//	MotSys の表示/非表示[StageShowMotSys()] ------------------------------------------------------
void CStageSPT::ShowMotSys(BOOL bShow)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return;
	}
	if (0 == bShow) {
		::PostMessage(m_hwndMotSys, MOTDRV_HIDEINDOW, 0, 0);	// 非表示
	} else {
		::PostMessage(m_hwndMotSys, MOTDRV_SHOWINDOW, 0, 0);	// 表示
	}
}

//	ジョイスティック 有効・無効[StageEnableJoyStickSPT()] ----------------------------------------
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

//	X/Y 軸を停止[StageStopAxis()] ----------------------------------------------------------------
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

//	XY 軸動作中ステータス[StageIsAxisIdle()] -----------------------------------------------------
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

//	絶対位置指定速度移動(指定軸：ウェイト制御付き)[StageMoveAbsoluteAtSpeedEx()] -----------------
int CStageSPT::MoveAbsoluteAtSpeedEx(WORD wAxis, long lPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	STAGE_COORD l_AdjEvPos;		// Z 補正用
	int l_iRc = STAGE_ERR_NONE;
	switch (wAxis) {
	case X:
	case Y:
		{
			// 指定位置を絶対座標に変換
			l_iRc = InquirePosSPT();	// 座標を(改めて)取得
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
				LogicalToPhysical(&l_LogicalPos, &l_PhysicalPos);				// 絶対座標に変換
				l_AdjEvPos.lX = l_PhysicalPos.lX; l_AdjEvPos.lY = l_PhysicalPos.lY;		// Z 補正用
				long l_lPhysicalPos;
				if (X == wAxis) {
					l_lPhysicalPos = l_PhysicalPos.lX;
				} else {
					l_lPhysicalPos = l_PhysicalPos.lY;
				}
				l_lPhysicalPos = ConvCountFrom(l_lPhysicalPos, m_lFactor[wAxis]);
				// 速度の設定
				long l_lSpeed = ConvCountFrom(dSpeed, m_lFactor[wAxis]);
				l_lSpeed = SpeedAdjustLimit(l_lSpeed, m_lLocalSpeed[wAxis], m_lMaxSpeed[wAxis]);
				// 移動指令
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
		AdjustElevator(FALSE, &l_AdjEvPos);		// Z 補正
		if (TRUE == bWait) {
			// 移動完了を待ちます．
			l_iRc = CStageNTN::Execute(&CStageNTN::MotSys_WaitMotionAxis, (LPVOID) MAKELONG(MAKEWORD(wAxis, 1), TIMEOUT_MOTSYS_MOTION));
		}
	}

	return l_iRc;
}

//	絶対位置指定速度移動(XY軸：ウェイト制御付き)[StageMoveAbsoluteAtSpeedEx()] -------------------
int CStageSPT::MoveAbsoluteAtSpeedEx(STAGE_COORD* pPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	// 指定位置を絶対座標に変換
	STAGE_COORD l_AdjEvPos;		// Z 補正用
	STAGE_COORD l_PhysicalPos;
	LogicalToPhysical(pPos, &l_PhysicalPos);				// 絶対座標に変換
	l_AdjEvPos.lX = l_PhysicalPos.lX; l_AdjEvPos.lY = l_PhysicalPos.lY;		// Z 補正用
	l_PhysicalPos.lX = ConvCountFrom(l_PhysicalPos.lX, m_lFactor[X]);
	l_PhysicalPos.lY = ConvCountFrom(l_PhysicalPos.lY, m_lFactor[Y]);
	// 速度の設定
	long l_lSpeedX = ConvCountFrom(dSpeed, m_lFactor[X]);
	long l_lSpeedY = ConvCountFrom(dSpeed, m_lFactor[Y]);
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
		AdjustElevator(FALSE, &l_AdjEvPos);		// Z 補正
		if (TRUE == bWait) {
			// 移動完了を待ちます．
			l_iRc = CStageNTN::Execute(&CStageNTN::MotSys_WaitMotionXY, (LPVOID) MAKELONG(MAKEWORD(0, 0), TIMEOUT_MOTSYS_MOTION));
		}
	}

	return l_iRc;
}

//	相対位置指定速度移動(指定軸：ウェイト制御付き)[StageMoveRelativeAtSpeed()] -------------------
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
			// 絶対座標を論理座標に変換してオフセットを加算
			l_iRc = InquirePosSPT();	// 座標を(改めて)取得
			if (STAGE_ERR_NONE == l_iRc) {
				long l_lLogicalPos;
				if (X == wAxis) {
					l_lLogicalPos = m_CurrPos.lX + lPos;
				} else {
					l_lLogicalPos = m_CurrPos.lY + lPos;
				}
				// 移動指令
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

//	相対位置指定速度移動(XY軸：ウェイト制御付き)[StageMoveRelativeAtSpeed()] ---------------------
int CStageSPT::MoveRelativeAtSpeed(STAGE_COORD* pPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == (m_hwndMotSys = ::FindWindow(0, MOTSYS_WINDOW_NAME))) {
		m_bMotSysInitialized = FALSE;
		return STAGE_ERR_MS_NONE_MOTSYS;
	}
	if (TRUE != m_bMotSysInitialized) {
		return STAGE_ERR_MS_AXIS_UNINITIALIZED;
	}

	// 絶対座標を論理座標に変換してオフセットを加算
	int l_iRc = InquirePosSPT();	// 座標を(改めて)取得
	if (STAGE_ERR_NONE == l_iRc) {
		STAGE_COORD l_LogicalPos;
		l_LogicalPos.lX = m_CurrPos.lX + pPos->lX;
		l_LogicalPos.lY = m_CurrPos.lY + pPos->lY;
		l_iRc = MoveAbsoluteAtSpeedEx(&l_LogicalPos, dSpeed, bWait);
	}

	return l_iRc;
}

//	SPT 用 ジョイスティック状態取得[StageGetJoystickSPT()] ----------------------------------------
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

//	指定速度移動(指定軸：ウェイト制御付き)[StageMoveAtSpeedEx()] ----------------------------------
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
			// 速度の設定
			long l_lSpeed = ConvCountFrom(dSpeed, m_lFactor[wAxis]);
			l_lSpeed = SpeedAdjustLimit(l_lSpeed, m_lLocalSpeed[wAxis], m_lMaxSpeed[wAxis]);
			// 移動指令
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
			// 移動完了を待ちます．
			l_iRc = CStageNTN::Execute(&CStageNTN::MotSys_WaitMotionAxis, (LPVOID) MAKELONG(MAKEWORD(wAxis, 1), TIMEOUT_MOTSYS_MOTION));
		}
	}

	return l_iRc;
}

//	SPT 用 軸の電子ギアを取得[StageGetElectronicGear()] -------------------------------------------
//		(分母：LOWORD(*plEleGearAB)，分子：HIWORD(*plEleGearAB)
//		初期化データ()で設定された内容です．
//		ここでは設定出来ませんが，設定範囲は：
//			上限：規定無し
//			下限：1/32 まで
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

/* 軸の単位変換係数を設定
 *		lFactor[count/指令単位]
 *			距離は SPT ステージの電子ギア比で決まります．
 *			SPT ステージの実フィードバック分解能が 156.25[nm]なので，
 *			電子ギア比が 8/25 の場合，下記の指令分解能になります．
 *				0.050[um/count]
 *			電子ギア比は SPT ステージの初期化データで設定されます．
 *			考え方：
 *				指令分解能が 0.050[um/count]の場合：
 *					指令単位を[um]にする場合，20[count/指令単位]です．
 *					指令単位を[0.1um]にする場合，2[count/指令単位]です．*/
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

/* ステージの原点位置オフセットを取得
		ステージの－側方向端から原点(０点)までの距離です． */
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
			// 逆転
			*plStageOrgLocX = l_StageSize.lX - l_OrgPos.lX;
		} else {
			// 正転
			*plStageOrgLocX = l_OrgPos.lX;
		}
		if (m_lFactor[Y] < 0) {
			// 逆転
			*plStageOrgLocY = l_StageSize.lY - l_OrgPos.lY;
		} else {
			// 正転
			*plStageOrgLocY = l_OrgPos.lY;
		}
	}

	return STAGE_ERR_NONE;
}

/* ジョイスティックの速度を設定 */
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
			// 速度の設定
			long l_lSpeed = ConvCountFrom(dSpeed, m_lFactor[wAxis]);
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
