#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>

#define _STAGEDLL_

#include "..\\..\\inc\\nexmisc.hxx"
#include "..\\..\\INC\\globals.hxx"
#include "stage.h"
#include "StageSPT.h"

extern WORD ConvAxisTo(WORD wAxis);		//	MotSys へ軸番号を変換
extern WORD ConvAxisFrom(WORD wAxis);	//	MotSys から軸番号を変換

typedef struct tagSTAGECALLBACK
{
	LPFNCSTAGENOTIFY lpfn;
	LPVOID			 lparg;
	void notify(void)
	{lpfn(lparg);}
	tagSTAGECALLBACK::tagSTAGECALLBACK()
	{::ZeroMemory(this, sizeof(tagSTAGECALLBACK));}
} STAGECALLBACK, *PSTAGECALLBACK;
STAGECALLBACK stageCallback;

CAbstractStage* pStage = 0;
/* added 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- { ---------- */
TCHAR g_tszProcDir[_MAX_PATH];			/* 呼出しプロセスのディレクトリ('\'付き)*/
TCHAR g_tszBaseDir[_MAX_PATH];			/* 基準ディレクトリ('\'付き)*/
TCHAR g_tszLogDir[_MAX_PATH];			/* ログ ディレクトリ('\'付き)*/
TCHAR g_tszMapFilePath[_MAX_PATH];		/* マップファイル名*/
/* added 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- } ---------- */

/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
BOOL APIENTRY DllMain( HANDLE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch ( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("Stage"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
//		if ( pStage == NULL ) {			StageSelector(...) で設定しています．
//			pStage = new CStageNTN();
//		}
/* added 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- { ---------- */
		TCHAR l_tszProcessFName[_MAX_PATH];	/* 呼出しプロセスのフルパス*/
		::GetModuleFileName(0, l_tszProcessFName, sizeof(l_tszProcessFName));
		TCHAR l_tszDrive[_MAX_DRIVE];
		TCHAR l_tszDir[_MAX_DIR];
		_tsplitpath(l_tszProcessFName, l_tszDrive, l_tszDir, 0, 0);
		_stprintf(g_tszProcDir, _T("%s%s"), l_tszDrive, l_tszDir);
		if (0 != _tcslen(g_tszProcDir)) {
			if (_T('\\') != g_tszProcDir[_tcslen(g_tszProcDir) - 1]) {
				_tcscat(g_tszProcDir, _T("\\"));
			}
		}
		TCHAR l_tszProcDirUp[_MAX_PATH];
		_stprintf(l_tszProcDirUp, _T("%s.."), g_tszProcDir);
		_tfullpath(g_tszBaseDir, l_tszProcDirUp, sizeof(g_tszBaseDir));
		if ((0 != _tcslen(g_tszBaseDir)) && (_T('\\') != g_tszBaseDir[_tcslen(g_tszBaseDir) - 1])) {
			_tcscat(g_tszBaseDir, _T("\\"));
		}
		_stprintf(g_tszLogDir, _T("%s") LOG_DIR, g_tszProcDir);
		if ((0 != _tcslen(g_tszLogDir)) && (_T('\\') != g_tszLogDir[_tcslen(g_tszLogDir) - 1])) {
			_tcscat(g_tszLogDir, _T("\\"));
		}
		_stprintf(g_tszMapFilePath, _T("%s") CFG_DIR STAGE_MAP_FILE_NAME DAT_EXT, g_tszProcDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- } ---------- */
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if ( pStage != NULL ) {
			delete pStage;
			pStage = NULL;
		}
		gc_iStageType = STAGETYPE_NONE;
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		break;
	}
	return TRUE;
}

// --------------------------------------------------------------------
// ステージタイプ選択
int __stdcall StageSelector(int iStageType)
{
	int l_iRc;

	if (0 != pStage) {
		l_iRc = gc_iStageType;		// すでに選択済です．
	} else {
		switch (iStageType) {
		case STAGETYPE_STD:
			gc_iStageType = iStageType;
			pStage = new CStageNTN();
			l_iRc = iStageType;
			break;
		case STAGETYPE_SPT:
			gc_iStageType = iStageType;
			pStage = new CStageSPT();
			l_iRc = iStageType;
			break;
		default:
			gc_iStageType = iStageType;
			l_iRc = STAGETYPE_NONE;
			break;
		}
	}

	return l_iRc;
}

// --------------------------------------------------------------------
// StageInitialize
BOOL __stdcall StageInitialize(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->Initialize() == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageInitializeStage
BOOL __stdcall StageInitializeStage(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->InitializeStage() == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageIsIdle
BOOL __stdcall StageIsIdle(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->IsIdle();
}

// --------------------------------------------------------------------
// StageInitializeAutoFocus
BOOL __stdcall StageInitializeAutoFocus(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->InitializeAutoFocus() == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageInitializeTurret
BOOL __stdcall StageInitializeTurret(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->InitializeTurret() == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageEnableJoyStick
BOOL __stdcall StageEnableJoyStick(BOOL bEnable/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->DisableJoystick(!bEnable) == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageEnableMotion
BOOL __stdcall StageEnableMotion(BOOL bEnable/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return TRUE;
}

// --------------------------------------------------------------------
// StageMoveAbsolute
BOOL __stdcall StageMoveAbsolute(WORD wAxis, long lPos)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	if (STAGETYPE_SPT == gc_iStageType) {
		return pStage->MoveAbsolute(wAxis, lPos) == STAGE_ERR_NONE;
	} else {
		return pStage->MoveAbsolute(wAxis, lPos);
	}
}

// --------------------------------------------------------------------
// StageMoveAbsolute
BOOL __stdcall StageMoveAbsolute(STAGE_COORD* pPos)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveAbsolute(pPos) == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageMoveAbsolute
BOOL __stdcall StageMoveAbsoluteEx(WORD wAxis, long lPos, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	if (STAGETYPE_SPT == gc_iStageType) {
		return pStage->MoveAbsoluteEx(wAxis, lPos, bWait) == STAGE_ERR_NONE;
	} else {
		return pStage->MoveAbsoluteEx(wAxis, lPos, bWait);
	}
}

// --------------------------------------------------------------------
// StageMoveAbsolute
BOOL __stdcall StageMoveAbsoluteEx(STAGE_COORD* pPos, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveAbsoluteEx(pPos, bWait) == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageMoveRelative
BOOL __stdcall StageMoveRelative(WORD wAxis, long lPos, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveRelative(wAxis, lPos, bWait) == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageMoveRelative
BOOL __stdcall StageMoveRelative(STAGE_COORD* pPos, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveRelative(pPos, bWait) == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// StageStop
BOOL __stdcall StageStop(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->StageStop() == STAGE_ERR_NONE;
}

// --------------------------------------------------------------------
// GetPos
void __stdcall StageGetPos(STAGE_COORD* pPos)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		pPos->lX = 0x80000000;
		pPos->lY = 0x80000000;
		return;
	}

	 pStage->GetPos(pPos);
}

void __stdcall StageGetPos(STAGE_COORD_XYZ* pPos)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		pPos->lX = 0x80000000;
		pPos->lY = 0x80000000;
		pPos->lZ = 0x80000000;
		return;
	}

	pStage->GetPos(pPos);
}

#ifndef _DISABLETHREADSPOS		// -----------------------------------------------------
long __stdcall StageGetPos(WORD wAxis)
{
	if (0 == pStage) {return 0x80000000;}	// ステージタイプが未設定です

	return pStage->GetPos(wAxis);
}

long __stdcall StageGetElevatorPos(void)
{
	if (0 == pStage) {return 0x80000000;}	// ステージタイプが未設定です

	return pStage->GetElevatorPos();
}
#else							// -----------------------------------------------------
BOOL __stdcall StageGetPos(WORD wAxis, long* pPos)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		*pPos = 0x80000000;
		return FALSE;
	}

	return pStage->GetPos(wAxis, pPos) == STAGE_ERR_NONE;
}

BOOL __stdcall StageGetElevatorPos(long* pPos)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		*pPos = 0x80000000;
		return FALSE;
	}

	return pStage->GetElevatorPos(pPos) == STAGE_ERR_NONE;
}
#endif //_DISABLETHREADSPOS		// -----------------------------------------------------

UINT __stdcall StageGetElevatorSpeed(double** ppElevatorSpeeds)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		*ppElevatorSpeeds = NULL;
		return 0;
	}

	return pStage->GetElevatorSpeed(ppElevatorSpeeds);
}

BOOL __stdcall StageSelectLens(UINT nSelect)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

/* added 2009.05.28 hmenjo Turret Backlash 除去 ---------- { ---------- */
	int l_iCurSel = pStage->GetCurLens();
	switch (nSelect) {
	case 1:
		if ((2 == l_iCurSel) || (3 == l_iCurSel)) {
			if (STAGE_ERR_NONE != pStage->SelectLens(0)) {
				return FALSE;
			}
		}
		break;
	case 2:
		if (3 == l_iCurSel) {
			if (STAGE_ERR_NONE != pStage->SelectLens(1)) {
				return FALSE;
			}
		}
		break;
	case 0:
	case 3:
	default:
		break;
	}
/* added 2009.05.28 hmenjo Turret Backlash 除去 ---------- } ---------- */
	return pStage->SelectLens(nSelect) == STAGE_ERR_NONE;
}

int __stdcall StageGetCurLens(void)
{
	if (0 == pStage) {return 0x80000000;}	// ステージタイプが未設定です

	return pStage->GetCurLens();
}

BOOL __stdcall StageDoAutoFocus(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->DoAutoFocus() == STAGE_ERR_NONE;
}

BOOL __stdcall StageAbortAutoFocus(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->AbortAutoFocus() == STAGE_ERR_NONE;
}

BOOL __stdcall StageMoveAtSpeed(WORD wAxis, double dSpeed)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveAtSpeed(wAxis, dSpeed) == STAGE_ERR_NONE;
}

void __stdcall StageGetEEPromData(EEPROMDATA* pEEPromData)
{
	if (0 == pStage) {return;}	// ステージタイプが未設定です

	pStage->GetEEPromData(pEEPromData);
}

BOOL __stdcall StageGetStageSize(long* pStageSizeX, long* pStageSizeY)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		*pStageSizeX = 0x80000000;
		*pStageSizeY = 0x80000000;
		return FALSE;
	}

	return pStage->GetStageSize(pStageSizeX, pStageSizeY) == STAGE_ERR_NONE;
}

BOOL __stdcall StageGetOriginPos(STAGE_COORD* pPos)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		pPos->lX = 0x80000000;
		pPos->lY = 0x80000000;
		return FALSE;
	}

	return pStage->GetOriginPos(pPos);
}

int  __stdcall StageGetMaxStageSpeed(double* x, double* y)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		*x = 0;
		*y = 0;
		return 0x8000000;
	}

	return pStage->GetMaxStageSpeed(x, y);
}

void __stdcall StageSetLocalSpeed(double x, double y)
{
	if (0 == pStage) {return;}	// ステージタイプが未設定です

	pStage->SetLocalSpeed(x, y);
}

// 2013.02.15 bagus Substrate thickness setting -->
void __stdcall StageSetZLensOffset(long lZOffset)
{
	if (0 == pStage) {return;}	// ステージタイプが未設定です

	pStage->SetZLensOffset(lZOffset);
}

void __stdcall StageSetZSubThickOffset(long lZOffset)
{
	if (0 == pStage) {return;}	// ステージタイプが未設定です

	pStage->SetZSubThickOffset(lZOffset);
}

void __stdcall StageSetAFInvalidRegion(double dVal)
{
	if (0 == pStage) {return;}	// ステージタイプが未設定です

	pStage->SetAFInvalidRegion(dVal);
}
// 2013.02.15 bagus Substrate thickness setting <--

BOOL __stdcall StageElevatorMove(long lPos)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->ElevatorMove(lPos) == STAGE_ERR_NONE;
}

BOOL __stdcall StageElevatorMoveAtSpeed(double dSpeed)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->ElevatorMoveAtSpeed(dSpeed) == STAGE_ERR_NONE;
}

BOOL __stdcall StageElevatorMoveAtSpeedEx(double dSpeed)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->ElevatorMoveAtSpeedEx(dSpeed) == STAGE_ERR_NONE;
}

BOOL __stdcall StageElevatorMoveRelative(long lPos)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->ElevatorMoveRelative(lPos) == STAGE_ERR_NONE;
}

BOOL __stdcall StageElevatorStop(void)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->ElevatorStop() == STAGE_ERR_NONE;
}

BOOL __stdcall StageVacuumCtrl(BOOL bOn)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->VacuumCtrl(bOn) == STAGE_ERR_NONE;
}

int  __stdcall StageGetVacuumStatus(void)
{
	if (0 == pStage) {return 0x80000000;}	// ステージタイプが未設定です

	return pStage->GetVacuumStatus();
}

void __stdcall StageEnableSpeedLimit(void)
{
	if (0 == pStage) {return;}	// ステージタイプが未設定です

	pStage->EnableSpeedLimit();
}

void __stdcall StageDisableSpeedLimit(void)
{
	if (0 == pStage) {return;}	// ステージタイプが未設定です

	pStage->DisableSpeedLimit();
}

// 軸移動完了
//		MotTsk のみ使用可能です
void __stdcall StageMTAxisMotionComplete(short wAxis, DWORD dwStatus)
{
	gc_bInMotionMS[ConvAxisFrom(wAxis)] = FALSE;
	gc_dwMotionStop[ConvAxisFrom(wAxis)] = dwStatus;
}

// MotSys 通信エラー セット
//		MotTsk のみ使用可能です
void __stdcall StageMTSetMotSysComErr(DWORD dwErrCode)
{
	gc_dwMotSysComErr = dwErrCode;
}

// MotSys 通信エラー 取得
//		MotTsk のみ使用可能です
DWORD __stdcall StageMTGetMotSysComErr(void)
{
	return gc_dwMotSysComErr;
}

// MotSys システムエラー セット
//		MotTsk のみ使用可能です
void __stdcall StageMTSetMotSysErr(DWORD dwErrCode)
{
	gc_dwMotSysErr = dwErrCode;
}

// MotSys システムエラー 取得
//		MotTsk のみ使用可能です
DWORD __stdcall StageMTGetMotSysErr(void)
{
	return gc_dwMotSysErr;
}

// 軸停止理由取得(0：正常停止，1：強制停止，2：異常停止)
DWORD __stdcall StageGetAxisStopReason(short wAxis)
{
	if ((X != wAxis) && (Y != wAxis)) {
		return -1;
	}

	return gc_dwMotionStop[wAxis];
}

// MotSys エラー クリア
void __stdcall StageClrMotSysErr(void)
{
	gc_dwMotSysComErr = 0;
	gc_dwMotSysErr = 0;
}

// MotSys エラー 取得(０：エラー無し)
int __stdcall StageGetMotSysErr(BOOL bAutoClr/*=TRUE*/)
{
	int l_iError = 0;

	if (0 != gc_dwMotSysErr) {
		l_iError = gc_dwMotSysErr;
	} else {
		l_iError = gc_dwMotSysComErr;
	}
	if (TRUE == bAutoClr) {
		gc_dwMotSysComErr = 0;
		gc_dwMotSysErr = 0;
	}

	return l_iError;
}

// Stage.dll が生きているか(初期化済かどうか)です
int __stdcall StageDllIsLive(void)
{
	return gc_iStageType;
}

// MotSys.exe の表示/非表示
void __stdcall StageShowMotSys(BOOL bShow)
{
	if (0 == pStage) {return;}	// ステージタイプが未設定です

	pStage->ShowMotSys(bShow);
}

// SPT 用 ジョイスティック 有効/無効
BOOL __stdcall StageEnableJoyStickSPT(BOOL bEnable/*=TRUE*/, int iSpeedSel/*=1*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->EnableJoystickSPT(bEnable, iSpeedSel) == STAGE_ERR_NONE;
}

// X/Y 軸を停止
BOOL __stdcall StageStopAxis(WORD wAxis)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->StageStopAxis(wAxis) == STAGE_ERR_NONE;
}

// XY 軸動作中ステータス
BOOL __stdcall StageIsAxisIdle(WORD wAxis)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->IsAxisIdle(wAxis);
}

// 絶対位置指定速度移動(指定軸：ウェイト制御付き)
BOOL __stdcall StageMoveAbsoluteAtSpeedEx(WORD wAxis, long lPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveAbsoluteAtSpeedEx(wAxis, lPos, dSpeed, bWait) == STAGE_ERR_NONE;
}

// 絶対位置指定速度移動(XY軸：ウェイト制御付き)
BOOL __stdcall StageMoveAbsoluteAtSpeedEx(STAGE_COORD* pPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveAbsoluteAtSpeedEx(pPos, dSpeed, bWait) == STAGE_ERR_NONE;
}

// 相対位置指定速度移動(指定軸：ウェイト制御付き)
BOOL __stdcall StageMoveRelativeAtSpeed(WORD wAxis, long lPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveRelativeAtSpeed(wAxis, lPos, dSpeed, bWait) == STAGE_ERR_NONE;
}

// 相対位置指定速度移動(XY軸：ウェイト制御付き)
BOOL __stdcall StageMoveRelativeAtSpeed(STAGE_COORD* pPos, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveRelativeAtSpeed(pPos, dSpeed, bWait) == STAGE_ERR_NONE;
}

// SPT 用 ジョイスティック状態取得
BOOL __stdcall StageGetJoyStickSPT(void)
{
	if (0 == pStage) {return TRUE;}	// ステージタイプが未設定です

	return pStage->GetJoyStickSPT();
}

// 指定速度移動(指定軸：ウェイト制御付き)
BOOL __stdcall StageMoveAtSpeedEx(WORD wAxis, double dSpeed, BOOL bWait/*=TRUE*/)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->MoveAtSpeedEx(wAxis, dSpeed, bWait) == STAGE_ERR_NONE;
}

// 軸の電子ギアを取得(分母：LOWORD(*plEleGearAB)，分子：HIWORD(*plEleGearAB)
//		初期化データ()で設定された内容です．
//		ここでは設定出来ませんが，設定範囲は：
//			上限：規定無し
//			下限：1/32 まで
BOOL __stdcall StageGetElectronicGear(WORD wAxis, long* plEleGearAB)
{
	if (0 == pStage) {
		// ステージタイプが未設定です
		*plEleGearAB = MAKELONG(1, 1);
		return FALSE;
	}

	return pStage->GetElectronicGear(wAxis, plEleGearAB);
}

// 軸の単位変換係数を設定
//		lFactor[count/指令単位]
//			距離は SPT ステージの電子ギア比で決まります．
//			SPT ステージの実フィードバック分解能が 156.25[nm]なので，
//			電子ギア比が 8/25 の場合，下記の指令分解能になります．
//				0.050[um/count]
//			電子ギア比は SPT ステージの初期化データで設定されます．
//			考え方：
//				指令分解能が 0.050[um/count]の場合：
//					指令単位を[um]にする場合，20[count/指令単位]です．
//					指令単位を[0.1um]にする場合，2[count/指令単位]です．
BOOL __stdcall StageSetFactor(WORD wAxis, long lFactor)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->SetFactor(wAxis, lFactor);
}

/* ステージの原点位置オフセットを取得
		ステージの－側方向端から原点(０点)までの距離です． */
BOOL __stdcall StageGetOrgLoc(long* plStageOrgLocX, long* plStageOrgLocY)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->GetOrgLoc(plStageOrgLocX, plStageOrgLocY) == STAGE_ERR_NONE;
}

/* ジョイスティックの速度を設定 */
BOOL __stdcall StageSetJoyStickSpeedSPT(WORD wAxis, int iSpeedSel, double dSpeed)
{
	if (0 == pStage) {return FALSE;}	// ステージタイプが未設定です

	return pStage->SetJoyStickSpeedSPT(wAxis, iSpeedSel, dSpeed) == STAGE_ERR_NONE;
}
