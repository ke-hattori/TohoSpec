#if !defined( _STAGEINT_HXX_ )
#define _STAGEINT_HXX_

#pragma once

#if !defined( _STAGEDLL_ )
	#define STAGEAPI __declspec(dllimport)
#else
	#define STAGEAPI __declspec(dllexport)
#endif

//#define _DISABLETHREADSPOS

// ====================================================================

//	Virtual stage axes for the Nanospec line of machines
const WORD X = 0;	// X
const WORD Y = 1;	// Y
const WORD Z = 2;	// Z (Elevator)

// ====================================================================
// EEPROM DEFINITIONS
#define EEPROM_BASE 			0x9125
#define EEPROM_MAX_BYTES		(256)

// Auto focus
#define AF_ROUGH_SPEED			0x0000
#define AF_FINE_SPEED			0x0008
#define AF_ROUGH_WIDTH			0x0010
#define AF_FINE_WIDTH			0x0018
#define AF_LONG_SCAN_POS		0x0020
#define AF_INIT_POS 			0x0030
#define AF_TURRET_POS			0x0034
#define AF_OFFSET				0x0038
#define AF_FINE_OFFSET			0x0048
#define AF_LONG_SCAN_WIDTH		0x0050
#define AF_MAX_POS_Z			0x0058
#define AF_TURRET_HIGH_SPEED	0x005C
#define AF_TURRET_LOW_SPEED 	0x005E
#define AF_SPEED_RANGE			0x0060
#define AF_ACCDEC_RATE			0x0062
#define AF_ENC_RATIO			0x0064
#define AF_NOISE_LEVEL			0x0066
#define AF_MODE1				0x0068
#define AF_TURRET_BRAKE_PULSE	0x0070
#define AF_MODE2				0x0072
#define AF_PULSE_PER_UM 		0x0074
#define AF_VIDEO				0x0078

// Auto stage
#define AS_MODE1				0x0080
#define AS_MODE2				0x0082
#define AS_RANGE1				0x0088
#define AS_RANGE2				0x008C
#define AS_RATE 				0x0090
#define AS_S_RATE				0x0094
#define AS_LOW_SPEED			0x0098
#define AS_HIGH_SPEED			0x00A0
#define AS_CONST_SPEED			0x00A8
#define AS_PLS_PER_10MM 		0x00B0
#define AS_FEDBK_PLS			0x00B4
#define AS_MAX_SIZE 			0x00B8
#define AS_FEDBK_DELAY			0x00BC
#define AS_SYSTEM_CFG			0x00BE
#define AS_ORG					0x00C0
#define AS_EDGE 				0x00C8
#define AS_OFFSET				0x00D0


// ====================================================================
// DEFINITIONS etc.
#define STAGE_LOG_FILE_NAME 	TEXT("stage")
#define MICROMETRE				(1000L)

typedef void (__stdcall *LPFNCSTAGENOTIFY)(void*);

typedef struct _STAGE_SPEED
{
	double dx;
	double dy;
} STAGE_SPEED, *PSTAGE_SPEED;

typedef union _MOTPARAMSEX
{
	struct
	{
		STAGE_COORD pos;
		STAGE_SPEED speed;
	} stage;
	struct
	{
		long   lZ;
		double dz;
	} elevator;
	_MOTPARAMSEX::_MOTPARAMSEX()
	{::ZeroMemory(this, sizeof(MOTPARAMSEX));}
} MOTPARAMSEX, *LPMOTPARAMSEX;

typedef struct _tagEEPROMDATA {
	struct {
		long InitPos;
		long MaxPos;
		WORD ppu;
	} AutoFocus;
	struct {
		struct {
			STAGE_SPEED Low;
			STAGE_SPEED Max;
			STAGE_SPEED Const;
		} Speed[2];
		STAGE_COORD Origin;
		struct {
			long x;
			long y;
		} Size;
		struct {
			long x;
			long y;
		} Edge;
	} AutoStage;
} EEPROMDATA;

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================

// --------------------------------------------------------------------
// ステージタイプ選択
// 下記関数のための仮定義です hmenjo
#define	STAGETYPE_NONE	0	// 未設定
#define	STAGETYPE_STD	1	// 標準 or NTN ステージ(すべてサブコン)
#define	STAGETYPE_SPT	2	// ソディックステージ(XY ステージのみソディック)
STAGEAPI int __stdcall StageSelector(int iStageType);

// --------------------------------------------------------------------
// Initialization/Shutdown
STAGEAPI BOOL __stdcall StageInitialize(void);
STAGEAPI BOOL __stdcall StageInitializeStage(void);
STAGEAPI BOOL __stdcall StageInitializeAutoFocus(void);
STAGEAPI BOOL __stdcall StageInitializeTurret(void);

// --------------------------------------------------------------------
// Status
STAGEAPI BOOL __stdcall StageIsIdle(void);

// --------------------------------------------------------------------
// EEProm
STAGEAPI BOOL __stdcall StageGetStageSize(long*, long*);
STAGEAPI BOOL __stdcall StageGetOriginPos(STAGE_COORD*);
STAGEAPI void __stdcall StageGetEEPromData(EEPROMDATA* pEEPromData);

// --------------------------------------------------------------------
// Configuration
STAGEAPI BOOL __stdcall StageEnableJoyStick(BOOL bEnable = TRUE);
STAGEAPI BOOL __stdcall StageEnableMotion(BOOL bEnable = TRUE);

// --------------------------------------------------------------------
// Absolute Moves
STAGEAPI BOOL __stdcall StageMoveAbsolute(WORD, long);
STAGEAPI BOOL __stdcall StageMoveAbsolute(STAGE_COORD*);
STAGEAPI BOOL __stdcall StageMoveAbsoluteEx(WORD, long, BOOL bWait = TRUE);
STAGEAPI BOOL __stdcall StageMoveAbsoluteEx(STAGE_COORD*, BOOL bWait = TRUE);

// --------------------------------------------------------------------
// Stop
STAGEAPI BOOL __stdcall StageStop(void);

// --------------------------------------------------------------------
// Relative Moves
STAGEAPI BOOL __stdcall StageMoveRelative(WORD, long, BOOL bWait = TRUE);
STAGEAPI BOOL __stdcall StageMoveRelative(STAGE_COORD*, BOOL bWait = TRUE);

// --------------------------------------------------------------------
// Speed Moves
STAGEAPI BOOL __stdcall StageMoveAtSpeed(WORD, double);

STAGEAPI void __stdcall StageSetLocalSpeed(double dx, double dy);

// 2013.02.22 bagus Substrate thickness setting -->
// --------------------------------------------------------------------
// Substrate Thickness
STAGEAPI void __stdcall StageSetZLensOffset(long lZOffset);

STAGEAPI void __stdcall StageSetZSubThickOffset(long lZOffset);

STAGEAPI void __stdcall StageSetAFInvalidRegion(double dVal);
// 2013.02.22 bagus Substrate thickness setting <--

// --------------------------------------------------------------------
// Inquiries
STAGEAPI void __stdcall StageGetPos(STAGE_COORD*);
STAGEAPI void __stdcall StageGetPos(STAGE_COORD_XYZ*);
#ifndef _DISABLETHREADSPOS
STAGEAPI long __stdcall StageGetPos(WORD wAxis);
STAGEAPI long __stdcall StageGetElevatorPos(void);
#else
STAGEAPI BOOL __stdcall StageGetPos(WORD, long*);
STAGEAPI BOOL __stdcall StageGetElevatorPos(long*);
#endif //_DISABLETHREADSPOS
STAGEAPI UINT __stdcall StageGetElevatorSpeed(double**);

// --------------------------------------------------------------------
// Notification

// --------------------------------------------------------------------
// Vacuum
STAGEAPI BOOL __stdcall StageVacuumCtrl(BOOL);
STAGEAPI int  __stdcall StageGetVacuumStatus(void);

STAGEAPI BOOL __stdcall StageSelectLens(UINT);
STAGEAPI int  __stdcall StageGetCurLens(void);
STAGEAPI BOOL __stdcall StageDoAutoFocus(void);
STAGEAPI BOOL __stdcall StageAbortAutoFocus(void);

STAGEAPI int  __stdcall StageGetMaxStageSpeed(double*, double*);
STAGEAPI void __stdcall StageSetLocalSpeed(double x, double y);

STAGEAPI BOOL __stdcall StageElevatorMove(long);
STAGEAPI BOOL __stdcall StageElevatorMoveAtSpeed(double);
STAGEAPI BOOL __stdcall StageElevatorMoveAtSpeedEx(double);
STAGEAPI BOOL __stdcall StageElevatorMoveRelative(long);

STAGEAPI BOOL __stdcall StageElevatorStop(void);

STAGEAPI void __stdcall StageEnableSpeedLimit(void);
STAGEAPI void __stdcall StageDisableSpeedLimit(void);

// --------------------------------------------------------------------
// 軸移動完了
//		MotTsk のみ使用可能です
STAGEAPI void __stdcall StageMTAxisMotionComplete(short wAxis, DWORD dwStatus);
// MotSys 通信エラー セット
//		MotTsk のみ使用可能です
STAGEAPI void __stdcall StageMTSetMotSysComErr(DWORD dwErrCode);
// MotSys 通信エラー 取得
//		MotTsk のみ使用可能です
STAGEAPI DWORD __stdcall StageMTGetMotSysComErr(void);
// MotSys システムエラー セット
//		MotTsk のみ使用可能です
STAGEAPI void __stdcall StageMTSetMotSysErr(DWORD dwErrCode);
// MotSys システムエラー 取得
//		MotTsk のみ使用可能です
STAGEAPI DWORD __stdcall StageMTGetMotSysErr(void);
// 軸停止理由取得
#define	STAGE_STOP_OK		0	// 正常停止
#define	STAGE_STOP_FORCE	1	// 強制停止
#define	STAGE_STOP_FAIL		2	// 異常停止
STAGEAPI DWORD __stdcall StageGetAxisStopReason(short wAxis);
// MotSys エラー クリア
STAGEAPI void __stdcall StageClrMotSysErr(void);
// MotSys エラー 取得(０：エラー無し)
STAGEAPI int __stdcall StageGetMotSysErr(BOOL bAutoClr = TRUE);
// Stage.dll が生きているか(初期化済か)です
STAGEAPI int __stdcall StageDllIsLive(void);
// MotSys.exe の表示/非表示
STAGEAPI void __stdcall StageShowMotSys(BOOL bShow);
// SPT 用 ジョイスティック 有効/無効
STAGEAPI BOOL __stdcall StageEnableJoyStickSPT(BOOL bEnable = TRUE, int iSpeedSel = 1);
// SPT 用 ジョイスティック状態取得(TRUE：ジョイスティック中)
STAGEAPI BOOL __stdcall StageGetJoyStickSPT(void);
// X/Y 軸を停止
STAGEAPI BOOL __stdcall StageStopAxis(WORD wAxis);
// XY 軸動作中ステータス
STAGEAPI BOOL __stdcall StageIsAxisIdle(WORD wAxis);
// 絶対位置指定速度移動(指定軸：ウェイト制御付き)
STAGEAPI BOOL __stdcall StageMoveAbsoluteAtSpeedEx(WORD wAxis, long lPos, double dSpeed, BOOL bWait = TRUE);
// 絶対位置指定速度移動(XY軸：ウェイト制御付き)
STAGEAPI BOOL __stdcall StageMoveAbsoluteAtSpeedEx(STAGE_COORD* pPos, double dSpeed, BOOL bWait = TRUE);
// 相対位置指定速度移動(指定軸：ウェイト制御付き)
STAGEAPI BOOL __stdcall StageMoveRelativeAtSpeed(WORD wAxis, long lPos, double dSpeed, BOOL bWait = TRUE);
// 相対位置指定速度移動(XY軸：ウェイト制御付き)
STAGEAPI BOOL __stdcall StageMoveRelativeAtSpeed(STAGE_COORD* pPos, double dSpeed, BOOL bWait = TRUE);
// 指定速度移動(指定軸：ウェイト制御付き)
STAGEAPI BOOL __stdcall StageMoveAtSpeedEx(WORD wAxis, double dSpeed, BOOL bWait = TRUE);
// 軸の電子ギアを取得(分母：LOWORD(*plEleGearAB)，分子：HIWORD(*plEleGearAB)
//		初期化データ()で設定された内容です．
//		ここでは設定出来ませんが，設定範囲は：
//			上限：規定無し
//			下限：1/32 まで
STAGEAPI BOOL __stdcall StageGetElectronicGear(WORD wAxis, long* plEleGearAB);
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
STAGEAPI BOOL __stdcall StageSetFactor(WORD wAxis, long lFactor);
/* ステージの原点位置オフセットを取得
		ステージの－側方向端から原点(０点)までの距離です． */
STAGEAPI BOOL __stdcall StageGetOrgLoc(long* plStageOrgLocX, long* plStageOrgLocY);
/* ジョイスティックの速度を設定 */
/*		wAxis		対象軸：X あるいは Y
		iSpeedSel	速度番号：1,2,3 のどれか
		dSpeed		速度値：*/
STAGEAPI BOOL __stdcall StageSetJoyStickSpeedSPT(WORD wAxis, int iSpeedSel, double dSpeed);

#endif // _STAGEINT_HXX_
