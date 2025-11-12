#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#define __NEXTRA_INTERNAL
#include "..\\..\\inc\\nexiobase.hxx"

#define	_STAGE_MASTER_C_
#include "stage.h"
#include <crtdbg.h>

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\lib\\dtnsutility.lib")
#pragma comment(lib, "..\\..\\lib\\dtnsnexio.lib")
#pragma comment(lib, "..\\..\\lib\\dTnsConfigFile.lib")
#else
#pragma comment(lib, "..\\..\\lib\\tnsutility.lib")
#pragma comment(lib, "..\\..\\lib\\tnsnexio.lib")
#pragma comment(lib, "..\\..\\lib\\TnsConfigFile.lib")
#endif

#define PARALEL_INTERFACE

/*
 サブコントローラーコマンド
*/
static const TCHAR szMoveStageEx		  = TCHAR('G');
static const PSTR  szMoveElevatorEx 	  = TEXT("GZ");
static const PSTR  szLimitSpeedEx		  = TEXT("GL");
static const TCHAR szGetPosXyz			  = TCHAR('H');
static const TCHAR szEEPromParam		  = TCHAR('I');
static const TCHAR szJoystickStatusXy	  = TCHAR('K'); 		// ジョイスティック有効・無効
static const TCHAR szMoveStageXy		  = TCHAR('M'); 		// XY軸移動
static const TCHAR szMoveStageZ 		  = TCHAR('N'); 		// Z軸移動
static const TCHAR szDoOffsetZ			  = TCHAR('O'); 		// Z軸オフセット
static const TCHAR szGetPosXy			  = TCHAR('P'); 		// ステージ現在位置取得
static const TCHAR szEncoderStatusZ 	  = TCHAR('Q'); 		// Z軸エンコーダ状態
static const TCHAR szSetExtCounter		  = TCHAR('S'); 		// 外部カウンタ設定
static const TCHAR szGetExtCounter		  = TCHAR('T'); 		// 外部カウンタ取得
// 2013.02.15 bagus Substrate thickness setting -->
static const TCHAR szAFInvalidRegion	  = TCHAR('U'); 		// AF無効領域
// 2013.02.15 bagus Substrate thickness setting <--
static const TCHAR szFirmwareVersion	  = TCHAR('V'); 		// ファームウェアーバージョン取得
static const TCHAR szSetPosX			  = TCHAR('X'); 		// X軸位置設定
static const TCHAR szSetPosY			  = TCHAR('Y'); 		// Y軸位置設定
static const TCHAR szSetPosZ			  = TCHAR('Z'); 		// Z軸位置設定
static const TCHAR szJoystickStatusZ	  = TCHAR('a'); 		// Z軸ジョイスティック有効・無効
static const TCHAR szInputPort			  = TCHAR('b'); 		// 入力ポート読込
static const TCHAR szOutputPort 		  = TCHAR('c'); 		// 出力ポートセット・リセット
static const TCHAR szShutterCtrl		  = TCHAR('e'); 		// 電磁弁ON・OFF
static const TCHAR szUpDownZ			  = TCHAR('g'); 		// Z軸UP・DOWN
static const TCHAR szTurretPowerOFF 	  = TCHAR('h'); 		// ターレット電源OFF
static const TCHAR szVacuumCtrl 		  = TCHAR('i'); 		// ステージ用バルブ開閉
//static const TCHAR sz 					TCHAR('o'); 		// ステージ位置移動フラグ
static const TCHAR szGetPosZ			  = TCHAR('p'); 		// Z軸現在位置取得
static const TCHAR szAutoFocusStatus	  = TCHAR('q'); 		// オートフォーカス有効・無効
static const TCHAR szStageStatus		  = TCHAR('r'); 		// ステージ有効・無効
static const TCHAR szHardwareStatus 	  = TCHAR('s'); 		// ハードウェアステータス
static const TCHAR szSelectLens 		  = TCHAR('t'); 		// レンズ選択
static const TCHAR szDoAutoFocus		  = TCHAR('u'); 		// オートフォーカス
static const TCHAR szInitAutoFocusTurret  = TCHAR('v'); 		// オートフォーカス・ターレット初期化
static const TCHAR szInitStage			  = TCHAR('x'); 		// ステージ初期化
static const TCHAR szInitTurret 		  = TCHAR('y'); 		// ターレット初期化
static const TCHAR szInitAutoFocus		  = TCHAR('z'); 		// オートフォーカス初期化

// =========================================================================
// Class Name :  CAbstractStage
//
// Description： ステージ抽象クラス
//
// Author:       T.Matsuhisa
//
// =========================================================================
CAbstractStage::CAbstractStage()
	: m_bValid(FALSE), m_hMonitorThread(NULL), m_bMonitorRun(FALSE),
	  m_bAutoFocusInProgress(FALSE), m_bAbortAutoFocus(FALSE),
	  m_bEnableSpeedLimit(FALSE), m_bDisableSpeedLimit(FALSE),
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- { ---------- */
//		m_LogFile(LOG_DIR, STAGE_LOG_FILE_NAME)
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- 			 */
	  m_LogFile(g_tszLogDir, STAGE_LOG_FILE_NAME)
/* modified 2009.07.07 hmenjo dll 相対パス対応 Stage.dll ---------- } ---------- */
{
// 2013.02.15 bagus Substrate thickness setting -->
	m_lZLensOffSet = 0;
	m_lZSubThickOffSet = 0;
// 2013.02.15 bagus Substrate thickness setting <--
	::ZeroMemory(&m_Params, sizeof(m_Params));
	ConfigFile_GetNanoSpecIni(&m_Params.SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
}

CAbstractStage::~CAbstractStage()
{
	gc_iStageType = STAGETYPE_NONE;
#ifndef _DISABLETHREADSPOS
	EndMonitor();
#endif // _DISABLETHREADSPOS
}

// =========================================================================
// Name：      LogicalToPhysical
// Purpose：   論理座標 to 物理座標
// Parameters: pSrc - 論理座標
//             pDst - 物理座標
// Returns:    None
//
// Notes:
//
void CAbstractStage::LogicalToPhysical(STAGE_COORD* pSrc, STAGE_COORD* pDst)
{
	pDst->lX = cos(rad(m_Params.SystemConfig.dCorrectAngle)) * pSrc->lX + sin(rad(m_Params.SystemConfig.dCorrectAngle)) * pSrc->lY;
	pDst->lY = cos(rad(m_Params.SystemConfig.dCorrectAngle)) * pSrc->lY - sin(rad(m_Params.SystemConfig.dCorrectAngle)) * pSrc->lX;
}

// =========================================================================
// Name：      LogicalToPhysical
// Purpose：   論理座標 to 物理座標
// Parameters: pPos - 論理座標 to 物理座標
//
// Returns:    None
//
// Notes:
//
void CAbstractStage::LogicalToPhysical(STAGE_COORD* pPos)
{
	STAGE_COORD Temp;
	Temp.lX = pPos->lX; Temp.lY = pPos->lY;
	LogicalToPhysical(&Temp, pPos);
}

// =========================================================================
// Name：      PhysicalToLogical
// Purpose：   物理座標 to 論理座標
// Parameters: pSrc - 物理座標
//             pDst - 論理座標
// Returns:    None
//
// Notes:
//
void CAbstractStage::PhysicalToLogical(STAGE_COORD* pSrc, STAGE_COORD* pDst)
{
	pDst->lX = cos(rad(m_Params.SystemConfig.dCorrectAngle)) * pSrc->lX - sin(rad(m_Params.SystemConfig.dCorrectAngle)) * pSrc->lY;
	pDst->lY = cos(rad(m_Params.SystemConfig.dCorrectAngle)) * pSrc->lY + sin(rad(m_Params.SystemConfig.dCorrectAngle)) * pSrc->lX;
}

// =========================================================================
// Name：      PhysicalToLogical
// Purpose：   物理座標 to 論理座標
// Parameters: pSrc - 物理座標
//             pDst - 論理座標
// Returns:    None
//
// Notes:
//
void CAbstractStage::PhysicalToLogical(STAGE_COORD_XYZ* pSrc, STAGE_COORD_XYZ* pDst)
{
	PhysicalToLogical((STAGE_COORD*) pSrc, (STAGE_COORD*) pDst);
	pDst->lZ = pSrc->lZ;
}

// =========================================================================
// Class Name :  CStageNTN
//
// Description： NTNステージラッパクラス
//
// Author:       T.Matsuhisa
// =========================================================================

// =========================================================================
// Construction/Destruction
CStageNTN::CStageNTN()
	: m_wPortNo(AUX_PORT), m_hThread(NULL), m_lpEEProm(NULL), pinterface(NULL), m_bInterrupt(FALSE), m_bInMotion(FALSE)
{
	::InitializeCriticalSection(&m_csWait);

	m_nTotalElevatorSpeeds = 10;
	m_pElevatorSpeeds = (double*) new double[m_nTotalElevatorSpeeds];

	if ( m_pElevatorSpeeds ) {
		m_pElevatorSpeeds[0] = 3.1;
		m_pElevatorSpeeds[1] = 6.2;
		m_pElevatorSpeeds[2] = 12.5;
		m_pElevatorSpeeds[3] = 18.7;
		m_pElevatorSpeeds[4] = 25.0;
		m_pElevatorSpeeds[5] = 50.0;
		m_pElevatorSpeeds[6] = 100.0;
		m_pElevatorSpeeds[7] = 200.0;
		m_pElevatorSpeeds[8] = 400.0;
		m_pElevatorSpeeds[9] = 2500.0;
	}
	m_lpEEProm = (LPBYTE) new BYTE[EEPROM_MAX_BYTES];

#if defined( PARALEL_INTERFACE )
	pinterface = new aux();
#else
	pinterface = new sio(NULL);
	pinterface->_create(TEXT("COM1"));
#endif

	if ( pinterface->_is_valid() ) {
		if ( pinterface->_init() == IOBASE_ERR_NONE ) {
#if defined( PARALEL_INTERFACE )
			m_wPortNo = pinterface->_access(AUX_PORT);
#else
			SerialInterfaceCtrl(TRUE);
#endif
		if ( m_wPortNo == AUX_PORT )
			m_bValid = TRUE;
		}

		CAuxInt result;
		DWORD dwOffset;
		for ( dwOffset = 0; dwOffset < EEPROM_MAX_BYTES; dwOffset++ ) {
			SendCommand(szEEPromParam, EEPROM_BASE + dwOffset, (PTSTR) result);
			m_lpEEProm[dwOffset] = static_cast<BYTE>(result);
		}

		GetMaxStageSpeed(&m_MaxStageSpeed.dx, &m_MaxStageSpeed.dy);
		m_LocalStageSpeed = m_MaxStageSpeed;
	}
}
CStageNTN::~CStageNTN()
{
	if ( pinterface != NULL ) {
		delete pinterface;
		pinterface = NULL;
	}
	if ( m_pElevatorSpeeds ) {
		delete [] m_pElevatorSpeeds;
		m_pElevatorSpeeds = NULL;
	}
	if ( m_lpEEProm != NULL ) {
		delete [] m_lpEEProm;
		m_lpEEProm = NULL;
	}

	::DeleteCriticalSection(&m_csWait);
}

// =========================================================================
// Name：      ThreadProc
// Purpose：
// Parameters: lpData - thisポインタ
//
// Returns:    UINT
//
// Notes:
//
UINT WINAPI CStageNTN::ThreadProc(LPVOID lpData)
{
	THREADPARAMS* pthrParams = reinterpret_cast<THREADPARAMS*>(lpData);
	CStageNTN*	  pThis 	 = reinterpret_cast<CStageNTN*>(pthrParams->pThis);

	return pThis->ThreadProc(pthrParams);
}

// =========================================================================
// Name：      ThreadProc
// Purpose：   絶対移動完了待ちスレッド
// Parameters: None
//
// Returns:    UINT
//
// Notes:
//
UINT CStageNTN::ThreadProc(PTHREADPARAMS pthrParams)
{
	pthrParams->iRet = (this->*(pthrParams->pStageFunc))(pthrParams->lpData);

	return 0L;
}

// =========================================================================
// Name：      IsStageIdle
// Purpose：   XYステージステータス
// Parameters: None
//
// Returns:    BOOL - TRUE = IDLE
//
// Notes:
//
BOOL CStageNTN::IsStageIdle(void)
{
	CAuxInt result;
	SendCommand(szMoveStageEx, MSEX_STATUS_XY, (PSTR) result);

	return static_cast<int>(result) == 0 ? TRUE : FALSE;
}

// =========================================================================
// Name：      IsElevatorIdle
// Purpose：   Elevatorテータス
// Parameters: None
//
// Returns:    BOOL - TRUE = IDLE
//
// Notes:
//
BOOL CStageNTN::IsElevatorIdle(void)
{
	CAuxInt result;
	SendCommand(szMoveStageEx, MSEX_STATUS_Z, (PSTR) result);

	return static_cast<int>(result) == 0 ? TRUE : FALSE;
}

// =========================================================================
// Name：      StageMove
// Purpose：   ステージ移動
// Parameters: pPos - STAGE_COORD構造体へのポインタ
//             nMode - モード - 0 =
//                            - 1 =
//                            - 2 =
//                            - 3 =
//                            - 4 =
//                            - 5 =
//                            - 6 =
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::StageMove(STAGE_COORD* pPos, UINT nMode)
{
	if ( !m_bEnableStage ) {
		return STAGE_ERR_INVALID_STAGE;
	}

	STAGE_COORD Pos;
	LogicalToPhysical(pPos, &Pos);

	AdjustElevator(FALSE, &Pos);

	SetPos(X, Pos.lX);
	SetPos(Y, Pos.lY);

	return StageMove(nMode);
}

int CStageNTN::StageMove(long lX, long lY, UINT nMode)
{
	STAGE_COORD Pos;
	Pos.lX = lX;
	Pos.lY = lY;

	return StageMove(&Pos, nMode);
}

int CStageNTN::StageMove(UINT nMode)
{
	CAuxInt result;
	if ( SendCommand(szMoveStageXy, nMode, (PSTR) result) != STAGE_ERR_NONE ||
		 static_cast<int>(result) != 0 ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}
// matsuhisa 2013.01.22 added >>>
	InquirePos();
// matsuhisa 2012.01.22 added <<<
	return ConvertErrorCode(static_cast<int>(result));
}

// =========================================================================
// Name：      StageMoveEx
// Purpose：   ステージ移動拡張版
// Parameters: lpmpx - MOTARAMSEX構造体へのポインタ
//             bWait - 完了待ちフラグ
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::StageMoveEx(LPMOTPARAMSEX lpmpx, BOOL bWait/*TRUE*/)
{
	int iRet = STAGE_ERR_NONE;

//	  m_wait.lock();

	m_bInMotion = TRUE;

	STAGE_COORD Pos;
	LogicalToPhysical(&lpmpx->stage.pos, &Pos);

	AdjustElevator(FALSE, NULL);


	SetPos(X, Pos.lX);
	SetPos(Y, Pos.lY);
	SetSpeed(X, lpmpx->stage.speed.dx);
	SetSpeed(Y, lpmpx->stage.speed.dy);

	CAuxInt result;
	if ( SendCommand(szMoveStageEx, MSEX_MOVE_XY, (PSTR) result) != STAGE_ERR_NONE ||
		 static_cast<int>(result) != 0 ) {
		m_bInMotion = FALSE;
//		   m_wait.unlock();
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

//	  m_wait.unlock();

	if ( bWait ) {
		while ( !IsStageIdle() ) {
			DoEvents();
		}
		StageStop();
	}
// matsuhisa 2013.01.22 added >>>
	InquirePos();
// matsuhisa 2012.01.22 added <<<
	return iRet;
}

// =========================================================================
// Name：      MoveAbsolute
// Purpose：   絶対位置移動(指定軸)
// Parameters: wAxis - 移動軸
//             lPos  - 目標位置
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::MoveAbsolute(WORD wAxis, long lPos)
{
	MOTPARAMSEX mpx;
	GetPos(&mpx.stage.pos);

	offsetofcoord(&mpx.stage.pos, wAxis) = lPos;

	::CopyMemory(&mpx.stage.speed, &m_LocalStageSpeed, sizeof(STAGE_SPEED));

	return StageMove(mpx.stage.pos.lX, mpx.stage.pos.lY, 0);
}

// =========================================================================
// Name：      MoveAbsolute
// Purpose：   絶対位置移動(指定軸)
// Parameters: wAxis - 移動軸
//             lPos  - 目標位置
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::MoveAbsoluteEx(WORD wAxis, long lPos, BOOL bWait/*=TRUE*/)
{
	MOTPARAMSEX mpx;
	GetPos(&mpx.stage.pos);
#if 1
	offsetofcoord(&mpx.stage.pos, wAxis) = lPos;
#else
	switch ( wAxis ) {
	case X:
		mpx.pos.lX = lPos;
		break;
	case Y:
		mpx.pos.lY = lPos;
		break;
	default:
		assert(FALSE);
	}
#endif

#if 0
	::CopyMemory(&mpx.stage.speed, &m_MaxStageSpeed, sizeof(STAGE_SPEED));
#else
	::CopyMemory(&mpx.stage.speed, &m_LocalStageSpeed, sizeof(STAGE_SPEED));
#endif

	return StageMoveEx(&mpx, bWait);
}

// =========================================================================
// Name：      MoveAbsolute
// Purpose：   絶対位置移動(XY軸)
// Parameters: pPos - 目標位置
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::MoveAbsolute(STAGE_COORD* pPos)
{
	MOTPARAMSEX mpx;

	::CopyMemory(&mpx.stage.pos, pPos, sizeof(STAGE_COORD));

	::CopyMemory(&mpx.stage.speed, &m_LocalStageSpeed, sizeof(STAGE_SPEED));

	return StageMove(pPos, 0);
}

// =========================================================================
// Name：      MoveAbsolute
// Purpose：   絶対位置移動(XY軸)
// Parameters: pPos - 目標位置
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::MoveAbsoluteEx(STAGE_COORD* pPos, BOOL bWait/*=TRUE*/)
{
	MOTPARAMSEX mpx;

	::CopyMemory(&mpx.stage.pos, pPos, sizeof(STAGE_COORD));
#if 0
	::CopyMemory(&mpx.stage.speed, &m_MaxStageSpeed, sizeof(STAGE_SPEED));
#else
	::CopyMemory(&mpx.stage.speed, &m_LocalStageSpeed, sizeof(STAGE_SPEED));
#endif

	return StageMoveEx(&mpx, bWait);
}

int CStageNTN::MoveRelative(WORD wAxis, long lPos, BOOL bWait/*=TRUE*/)
{
#ifndef _DISABLETHREADSPOS
	InquirePos();
#endif

	MOTPARAMSEX mpx;
	GetPos(&mpx.stage.pos);

#if 1
	offsetofcoord(&mpx.stage.pos, wAxis) += lPos;
#else
	switch ( wAxis ) {
	case X:
		mpx.pos.lX += lPos;
		break;
	case Y:
		mpx.pos.lY += lPos;
		break;
	default:
		assert(FALSE);
	}
#endif

#if 0
	::CopyMemory(&mpx.stage.speed, &m_MaxStageSpeed, sizeof(STAGE_SPEED));
#else
	::CopyMemory(&mpx.stage.speed, &m_LocalStageSpeed, sizeof(STAGE_SPEED));
#endif

	return StageMoveEx(&mpx, bWait);
}

int CStageNTN::MoveRelative(STAGE_COORD* pPos, BOOL bWait/*=TRUE*/)
{
#ifndef _DISABLETHREADSPOS
	InquirePos();
#endif

	MOTPARAMSEX mpx;
	GetPos(&mpx.stage.pos);

	mpx.stage.pos.lX += pPos->lX; mpx.stage.pos.lY += pPos->lY;

#if 0
	::CopyMemory(&mpx.stage.speed, &m_MaxStageSpeed, sizeof(STAGE_SPEED));
#else
	::CopyMemory(&mpx.stage.speed, &m_LocalStageSpeed, sizeof(STAGE_SPEED));
#endif

	return StageMoveEx(&mpx, bWait);
}

// =========================================================================
// Name：      MoveAtSpeed
// Purpose：   速度移動
// Parameters: wAxis  - 移動軸
//             dSpeed - 目標スピード
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::MoveAtSpeed(WORD wAxis, double dSpeed)
{
	if ( m_bInMotion ) return 0;
	m_wait.lock();
#ifndef _DISABLETHREADSPOS
	InquirePos();
#endif

	MOTPARAMSEX mpx;
	GetPos(&mpx.stage.pos);

#if 1
	offsetofcoord(&mpx.stage.pos, wAxis)   = dSpeed > 0 ? 9999999L : -9999999L;
	offsetofspeed(&mpx.stage.speed, wAxis) = fabs(dSpeed);
#else
	switch ( wAxis ) {
	case X:
		mpx.pos.lX = m_Params.StageConfig.StageSize.lX;
		mpx.speed.dx = dSpeed;
		break;
	case Y:
		mpx.pos.lY = m_Params.StageConfig.StageSize.lY;
		mpx.speed.dy = dSpeed;
		break;
	default:
		assert(FALSE);
	}
#endif

	int iRet = StageMoveEx(&mpx, FALSE);
	m_wait.unlock();
	return iRet;
}
int CStageNTN::StageStop(void)
{
	m_wait.lock();

	int iRet = SendCommand(szMoveStageEx, MSEX_STOP_XY);

	m_bInMotion = FALSE;

	m_wait.unlock();

	return iRet;
}

// =========================================================================
// Name：      ElevatorMove
// Purpose：   エレベーター移動
// Parameters: lPos - 目標位置
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::ElevatorMove(long lPos)
{
	int iRet = STAGE_ERR_NONE;

	AutoFocusInquire();

	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

//	  if ( SendCommand(szSetPosZ, (lPos / MICROMETRE * 100L)) != STAGE_ERR_NONE )
	if ( SendCommand(szSetPosZ, double(lPos / double(MICROMETRE * 10))) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;
	if ( SendCommand(szMoveStageZ, 0) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;
/* added 2009.10.19 hmenjo Z 軸用に位置取得追加 ---------- { ---------- */
	InquirePos();
/* added 2009.10.19 hmenjo Z 軸用に位置取得追加 ---------- } ---------- */

	return iRet;
}

// =========================================================================
// Name：      ElevatorMoveRelative
// Purpose：   エレベーター移動
// Parameters: lPos - 目標位置
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::ElevatorMoveRelative(long lPos)
{
	int iRet = STAGE_ERR_NONE;

	AutoFocusInquire();


	long lCurrPos;
#ifndef _DISABLETHREADSPOS
		lCurrPos = GetElevatorPos();
#else
		GetElevatorPos(&lCurrPos);
#endif

	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

	if ( SendCommand(szSetPosZ, double((lPos * 10.0 + lCurrPos) / (1000.0 * 10.0))) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;
	if ( SendCommand(szMoveStageZ, 0) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;
/* added 2009.10.19 hmenjo Z 軸用に位置取得追加 ---------- { ---------- */
	InquirePos();
/* added 2009.10.19 hmenjo Z 軸用に位置取得追加 ---------- } ---------- */

	return iRet;
}

// =========================================================================
// Name：      ElevatorMoveEx
// Purpose：   エレベーター移動拡張版
// Parameters: lpmpx - MOTARAMSEX構造体へのポインタ
//             bWait - 完了待ちフラグ
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::ElevatorMoveEx(LPMOTPARAMSEX lpmpx, BOOL bWait/*TRUE*/)
{
	int iRet = STAGE_ERR_NONE;

	static WORD speeds_table[] =
	{			// No.
		1,		// 00
		1,		// 01
		2,		// 02
		5,		// 03
		10, 	// 04
		20, 	// 05
		40, 	// 06
		60, 	// 07
		80, 	// 08
		160,	// 09
		320,	// 10
		640,	// 11
		1280,	// 12
		8000,	// 13
//		  8000,   // 14
//		  8000,   // 15
	};

	const double resolution = 0.05;
	const WORD	 min_range	= 1;
	const WORD	 max_range	= 8191;

	BOOL valid_data = FALSE;
	WORD range;
	int index;
	for ( index = 0; index < _countof(speeds_table); index++ ) {
		range = (resolution * speeds_table[index]) / lpmpx->elevator.dz + 0.5;
		if ( range < min_range ) {
			break;
		}
		else if ( range > max_range ) {
			continue;
		}
		else {
			valid_data = TRUE;
			break;
		}
	}

	assert(range < min_range || range > max_range);

	if ( valid_data == FALSE ) return STAGE_ERR_INVALID_DATA;

	AutoFocusInquire();

	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

	if ( SendCommand(szSetPosZ, lpmpx->elevator.lZ) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;

	CAuxInt result;
	if ( SendCommand(szMoveStageEx, MSEX_SPEEDS_Z + index, (PSTR) result) != STAGE_ERR_NONE ||
		 static_cast<int>(result) != 0 ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

	if ( SendCommand(szMoveElevatorEx, range, (PSTR) result) != STAGE_ERR_NONE ||
		 static_cast<int>(result) != 0 ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

	if ( bWait ) {
		while ( !IsElevatorIdle() ) {
			DoEvents();
		}
		ElevatorStop();
	}
/* added 2009.10.19 hmenjo Z 軸用に位置取得追加 ---------- { ---------- */
	InquirePos();
/* added 2009.10.19 hmenjo Z 軸用に位置取得追加 ---------- } ---------- */
	return iRet;
}

// =========================================================================
// Name：      ElevatorMoveAtSpeed
// Purpose：   フォーカス軸速度移動
// Parameters: dSpeed - 目標スピード
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::ElevatorMoveAtSpeed(double dSpeed)
{
	AutoFocusInquire();

	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

	for ( int index = 0; (size_t)index < m_nTotalElevatorSpeeds; index++ ) {
		if ( m_pElevatorSpeeds[index] == fabs(dSpeed) ) {
			index += MSEX_MOVE_Z + (dSpeed < 0 ? m_nTotalElevatorSpeeds : 0);
			CAuxInt result;
			if ( SendCommand(szMoveStageEx, index, (PSTR) result) != STAGE_ERR_NONE ||
				 static_cast<int>(result) != 0 ) {
				return STAGE_ERR_COMMUNICATION_FAILURE;
			}
/* added 2009.10.19 hmenjo Z 軸用に位置取得追加 ---------- { ---------- */
			InquirePos();
/* added 2009.10.19 hmenjo Z 軸用に位置取得追加 ---------- } ---------- */
			return ConvertErrorCode(static_cast<int>(result));
		}
	}
	return STAGE_ERR_INVALID_DATA;
}

// =========================================================================
// Name：      ElevatorMoveAtSpeedEx
// Purpose：   フォーカス軸速度移動
// Parameters: dSpeed - 目標スピード
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::ElevatorMoveAtSpeedEx(double dSpeed)
{
	AutoFocusInquire();

	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

	MOTPARAMSEX mpx;
	mpx.elevator.lZ = dSpeed > 0 ? 9999999L : -9999999L;
	mpx.elevator.dz = fabs(dSpeed);

	return ElevatorMoveEx(&mpx, FALSE);;
}

int CStageNTN::ElevatorStop(void)
{
	return SendCommand(szMoveStageEx, MSEX_STOP_Z);
}

// =========================================================================
// Name：      VacuumCtrl
// Purpose：   バキューム制御
// Parameters: bOn - ON/OFF
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::VacuumCtrl(BOOL bOn)
{
	return SendCommand(szVacuumCtrl, bOn);
}

#define ALIGNER_NONE 0
// =========================================================================
// Name：      DoAligner
// Purpose：   バキューム制御
// Parameters: bOn - ON/OFF
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::DoEdgeAligner(STAGE_COORD* pOffset)
{
	int iRet;

//	  if ( m_Params.Stage.nAlignerType == ALIGNER_NONE )
//		  return STAGE_ERR_ALIGNER_NOT_FOUND;

	DisableJoystick();

	AdjustElevator(TRUE, NULL);

//	long xsensor_y, xsensor_xstart, xsensor_xend, ysensor_x, ysensor_ystart, ysensor_yend;
	long xsensor_xstart = m_Params.EdgeAligner.Start.lX + pOffset->lX;
	long xsensor_xend	= m_Params.EdgeAligner.End.lX + pOffset->lX;
	long xsensor_y		= (m_Params.EdgeAligner.Start.lY + m_Params.EdgeAligner.End.lY) / 2L + pOffset->lY;
	long ysensor_x		= (m_Params.EdgeAligner.Start.lX + m_Params.EdgeAligner.End.lX) / 2L + pOffset->lX;
	long ysensor_ystart = m_Params.EdgeAligner.Start.lY + pOffset->lY;
	long ysensor_yend	= m_Params.EdgeAligner.End.lY + pOffset->lY;

	BOOL bSuccess = TRUE;
#if 0
	long lAngle = round(m_Params.SystemConfig.CorrectAngle.lY / DEGREE90);
	if ( abs(m_Params.SystemConfig.CorrectAngle.lX - m_Params.SystemConfig.CorrectAngle.lY) < 5.0 && abs(lAngle) == 1 ) {
#else
	if ( m_Params.SystemConfig.dCorrectAngle == DEGREE90 || m_Params.SystemConfig.dCorrectAngle == -DEGREE90 ) {
#endif
		if ( (iRet = StageMove(ysensor_x, ysensor_ystart, MS_MOVE_XY)) != STAGE_ERR_NONE )		// スキャン開始位置移動
			goto EXIT;
		if ( (iRet = StageMove(ysensor_x, ysensor_yend, MS_SCAN_XSENSOR)) != STAGE_ERR_NONE )	// X軸スキャン
			goto EXIT;
		if ( (iRet = StageMove(xsensor_xstart, xsensor_y, MS_MOVE_XY)) != STAGE_ERR_NONE )		// スキャン開始位置移動
			goto EXIT;
		if ( (iRet = StageMove(xsensor_xend, xsensor_y, MS_SCAN_YSENSOR)) != STAGE_ERR_NONE)	// Y軸スキャン
			goto EXIT;

		if ( (iRet = StageMove(MS_STORE_POS1)) != STAGE_ERR_NONE )
			goto EXIT;
		GetPos(&m_EdgePos[0]);

		if ( (iRet = StageMove(MS_STORE_POS2)) != STAGE_ERR_NONE )
			goto EXIT;
		GetPos(&m_EdgePos[1]);

		if ( (abs(xsensor_xstart - m_EdgePos[1].lX) < 0.1) ||
			 (abs(xsensor_xstart - m_EdgePos[0].lX) < 0.1) ||
			 (abs(ysensor_ystart - m_EdgePos[1].lY) < 0.1) ) {
			// サーチ失敗
			bSuccess = FALSE;
		}
	}
	else {
		if ( (iRet = StageMove(xsensor_xstart, xsensor_y, MS_MOVE_XY)) != STAGE_ERR_NONE )		// スキャン開始位置移動
			goto EXIT;
		if ( (iRet = StageMove(xsensor_xend, xsensor_y, MS_SCAN_XSENSOR)) != STAGE_ERR_NONE)	// X軸スキャン
			goto EXIT;
		if ( (iRet = StageMove(ysensor_x, ysensor_ystart, MS_MOVE_XY)) != STAGE_ERR_NONE )		// スキャン開始位置移動
			goto EXIT;
		if ( (iRet = StageMove(ysensor_x, ysensor_yend, MS_SCAN_YSENSOR)) != STAGE_ERR_NONE )	// Y軸スキャン
			goto EXIT;

		if ( (iRet = StageMove(MS_STORE_POS1)) != STAGE_ERR_NONE )
			goto EXIT;
		GetPos(&m_EdgePos[0]);

		if ( (iRet = StageMove(MS_STORE_POS2)) != STAGE_ERR_NONE )
			goto EXIT;
		GetPos(&m_EdgePos[1]);

		if ( (abs(xsensor_xstart - m_EdgePos[1].lX) < 0.1) ||
			 (abs(ysensor_ystart - m_EdgePos[1].lY) < 0.1) ||
			 (abs(ysensor_ystart - m_EdgePos[0].lY) < 0.1) ) {
			// サーチ失敗
			bSuccess = FALSE;
		}
	}
	if ( !bSuccess ) {
		iRet = STAGE_ERR_EDGE_ALIGNER_FAILURE;
		m_EdgePos[0].lX = m_Params.EdgeAligner.Sensor.X[0].lX; m_EdgePos[0].lY = m_Params.EdgeAligner.Sensor.Y[0].lY;
		m_EdgePos[1].lX = m_Params.EdgeAligner.Sensor.X[0].lX; m_EdgePos[1].lY = m_Params.EdgeAligner.Sensor.Y[1].lY;
	}

EXIT:
	DisableJoystick(FALSE);
	return iRet;
}

// =========================================================================
// Name：      DoAutoFocus
// Purpose：   オートフォーカス
// Parameters: None
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::DoAutoFocus(void)
{
	return Execute(&CStageNTN::DoAutoFocus);
}

int CStageNTN::DoAutoFocus(LPVOID)
{
	AutoFocusInquire();

	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

	m_bAutoFocusInProgress = TRUE;

	CAuxInt result;
	SendCommand(szDoAutoFocus, 0, (PTSTR) result);

//----- Kawa 2009.01.14 ----->
	InquirePos();
//<---------------------------

	m_bAbortAutoFocus	   = FALSE;
	m_bAutoFocusInProgress = FALSE;

	// 保留
	return static_cast<int>(result);
}

int CStageNTN::AbortAutoFocus(void)
{
	if ( IsAutoFocusInProgress() ) {
		pinterface->_abort();
		m_bAbortAutoFocus = TRUE;
	}

	return STAGE_ERR_NONE;
}

// =========================================================================
// Name：      HardwareStatusInquire
// Purpose：   ハートウェア状態確認
// Parameters: nID - サブコマンドID
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::HardwareStatusInquire(UINT nID)
{
	int iRet;

	CAuxInt result;
	if ( (iRet = SendCommand(szHardwareStatus, nID, (PTSTR) result)) != STAGE_ERR_NONE )
		return iRet;

	iRet = static_cast<int>(result);
	return iRet;
}
// =========================================================================
// Name：      AutoFocusInquire
// Purpose：   オートフォーカス状態確認
// Parameters: None
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::AutoFocusInquire(BOOL bDisable/*=TRUE*/)
{
	int iRet = HardwareStatusInquire(HWSTCMD_PARAMS_AUTOFOCUS);

	if ( iRet == HWSTCMD_RESULT_PCBERROR && bDisable ) {
		DisableAutoFocus();
	}
	return iRet;
}

// =========================================================================
// Name：      TurretInquire
// Purpose：   ターレット状態
// Parameters: None
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::TurretInquire(BOOL bDisable/*=TRUE*/)
{
	int iRet = HardwareStatusInquire(HWSTCMD_PARAMS_LENS);

	if ( iRet == HWSTCMD_RESULT_PCBERROR && bDisable ) {
		DisableTurret();
	}
	return iRet;
}

// =========================================================================
// Name：      StageInquire
// Purpose：   ステージ状態
// Parameters: None
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::StageInquire(BOOL bDisable/*=TRUE*/)
{
	int iRet = HardwareStatusInquire(HWSTCMD_PARAMS_STAGE);

	if ( iRet == HWSTCMD_RESULT_PCBERROR && bDisable ) {
		DisableStage();
	}
	return iRet;
}

// =========================================================================
// Name：      DisableTurret
// Purpose：   ターレット無効
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
int CStageNTN::DisableTurret(void)
{
	m_bEnableTurret = FALSE;

	if ( SendCommand(szTurretPowerOFF, 0) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;

	return STAGE_ERR_NONE;
}

// =========================================================================
// Name：      DisableAutoFocus
// Purpose：   オートフォーカス無効
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
int CStageNTN::DisableAutoFocus(void)
{
	m_bEnableAutoFocus = FALSE;

	if ( SendCommand(szAutoFocusStatus, 0) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;

	return STAGE_ERR_NONE;
}

// =========================================================================
// Name：      DisableStage
// Purpose：   ステージ無効
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
int CStageNTN::DisableStage(void)
{
	m_bEnableJoystick = m_bEnableStage = FALSE;

	if ( SendCommand(szStageStatus, 0) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;

	return STAGE_ERR_NONE;
}

// =========================================================================
// Name：      EnableJoystickMoveElevator
// Purpose：   ジョイスティックエレベーター移動 有効・無効
// Parameters: bEnable - TRUE = 有効
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::EnableJoystickMoveElevator(BOOL bEnable/*=FALSE*/)
{
	AutoFocusInquire();

	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

	SendCommand(szJoystickStatusZ, bEnable);

	return STAGE_ERR_NONE;
}

// =========================================================================
// Name：      DisableJoystick
// Purpose：   ジョイスティック 有効・無効
// Parameters: bDisable - TRUE = 無効
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::DisableJoystick(BOOL bDisable/*=TRUE*/)
{
	if ( !IsEnableStage() ) {
		m_bEnableJoystick = FALSE;
		return STAGE_ERR_INVALID_STAGE;
	}

	CAuxInt result;
	if ( SendCommand(szJoystickStatusXy, !bDisable, (PTSTR) result) != STAGE_ERR_NONE )
		return STAGE_ERR_COMMUNICATION_FAILURE;
	if ( !bDisable && (static_cast<int>(result) != 0) ) {
		DisableJoystick();
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}
	m_bEnableJoystick = !bDisable;

	return STAGE_ERR_NONE;
}

// =========================================================================
// Name：      IsDownVibIsol
// Purpose：   Z軸エンコーダ状態
// Parameters: None
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:      途中
//
/*
BOOL CStageNTN::IsDownVibIsol(void)
{
    int iRet = STAGE_ERR_INVALID_PORT;

    if ( !IsValid() ) return iRet;

    if ( (iRet = HardwareStatusInquire(HWSTCMD_PARAMS_STAGE)) == HWSTCMD_RESULT_SENSORERROR ) {

    return TRUE;
}
*/

// =========================================================================
// Name：      IsValidZAxisEncoder
// Purpose：   Z軸エンコーダ状態
// Parameters: None
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CStageNTN::IsValidZAxisEncoder(void)
{
/*
	if ( m_Params.ZAxis.Enc.bValid ) {
		CAuxZ result;
		// Z軸位置取得
		SendCommand(szGetPosZ, 0, (PTSTR) result);
		long lPos = *result.convert();

		// 受信用バッファクリア
		result.clear();

		// Z軸エンコーダ値取得
		SendCommand(szEncoderStatusZ, 0, (PTSTR) result);
		double dEnc = (*result.convert() - m_Params.ZAxis.Enc.dOffset) * m_Params.ZAxis.Enc.dDiv + m_Params.ZAxis.Enc.dOffset;
		if ( abs(lPos - dEnc) >= MIN_ENC ) return FALSE;
	}
*/
	return TRUE;
}

// =========================================================================
// Name：      AdjustElevator
// Purpose：   ステージ平坦度調整
// Parameters: bZero - 0位置
//             pPos  - ステージ座標
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
int CStageNTN::AdjustElevator(BOOL bZero, STAGE_COORD* pPos/*=NULL*/)
{
	int iRet = STAGE_ERR_NONE;
	static long lOffsetPrev = 0L;

	if ( pPos == NULL ) 		return STAGE_ERR_INVALID_DATA;
	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;
	if ( !IsEnableStage() ) 	return STAGE_ERR_INVALID_STAGE;

//	  if ( m_Params.ZAxis.bOffsetCtrl && m_StageMap.IsValid() ) {
	if ( m_StageMap.IsValid() ) {
		long lOffset = 0L;
		// 2013.02.15 bagus Substrate thickness setting -->
		long lTemp = 0L;
		// 2013.02.15 bagus Substrate thickness setting <--
		if ( !bZero ) {
			// 2013.02.15 bagus Substrate thickness setting -->
			//lOffset = long(m_StageMap.InterpolateFromNeighbors(pPos) * 100.0 + 0.5);	 //
			lTemp = m_StageMap.InterpolateFromNeighbors(pPos) + m_lZLensOffSet + m_lZSubThickOffSet;
			lOffset = long(lTemp * 100.0 + 0.5);	 //
			// 2013.02.15 bagus Substrate thickness setting <--
		}

		if ( lOffset != lOffsetPrev ) {
			if ( (iRet = AdjustElevator(lOffset)) != STAGE_ERR_NONE )
				lOffsetPrev = lOffset;
		}
	}
	return iRet;
}

// =========================================================================
// Name：      AdjustElevator
// Purpose：   ステージ平坦度調整
// Parameters: bZero - 0位置
//             pPos  - ステージ座標
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
int CStageNTN::AdjustElevator(long lOffset)
{
//	  return SendCommand(szDoOffsetZ, min(max((lOffset / MICROMETRE * 100L), 0L), 9999L));
	return SendCommand(szDoOffsetZ, min(max(lOffset, 0L), 9999L));
}

// =========================================================================
// Name：      SelectLens
// Purpose：   レンズ選択
// Parameters: nSelect - レンズ
//
// Returns:    int - 0 = 成功
//
// Notes:
//
int CStageNTN::SelectLens(UINT nSelect)
{
	if ( m_nSelectLens == nSelect ) return STAGE_ERR_NONE;

	return SelectLens(&nSelect);
}

int CStageNTN::SelectLens(LPVOID lpData)
{
	UINT nSelect = *((UINT*) lpData);
	if ( nSelect > 3 )
		return STAGE_ERR_INVALID_DATA;

	TurretInquire();

	m_nSelectLens = nSelect;
	return SendCommand(szSelectLens, m_nSelectLens);
}

// =========================================================================
// Name：      Initialize
// Purpose：   初期化
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
int CStageNTN::Initialize(void)
{
	return Execute(&CStageNTN::Initialize);
}

int CStageNTN::Initialize(LPVOID)
{
	int iRet;

	if ( (iRet = AdjustElevator(0L)) != STAGE_ERR_NONE )
		goto EXIT;
	if ( (iRet = InitializeAutoFocusTurret()) != STAGE_ERR_NONE )
		goto EXIT;

	CtrlSpeedLimit();

#if 0	// modified hmenjo 20090415 ---------- { -----
	if ( (iRet = InitializeStage()) != STAGE_ERR_NONE )
		goto EXIT;
#else	// modified hmenjo 20090415 ----------
	if (STAGETYPE_SPT != gc_iStageType) {
		if ( (iRet = InitializeStage()) != STAGE_ERR_NONE )
			goto EXIT;
	}
#endif	// modified hmenjo 20090415 ---------- } -----

#ifndef _DISABLETHREADSPOS
	BeginMonitor();
#else
	STAGE_COORD XYPos;
	GetPos(&XYPos);
	long		lZPos;
	GetElevatorPos(&lZPos);
#endif // _DISABLETHREADSPOS

EXIT:
	return iRet;
}

// =========================================================================
// Name：      InitializeStage
// Purpose：   ステージ初期化
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
int CStageNTN::InitializeStage(void)
{
	int iRet = STAGE_ERR_STAGE_INIT_FAILURE;

	m_bEnableStage = m_bEnableJoystick = FALSE;

	iRet = StageInquire(FALSE);
	if ( iRet < HWSTCMD_RESULT_RUN ) {
		return iRet;
	}
	else if ( iRet >= HWSTCMD_RESULT_RUN  &&
			  iRet < HWSTCMD_RESULT_SENSORERROR ) {
		CAuxInt result;
		if ( SendCommand(szInitStage, 0, (PTSTR) result) == STAGE_ERR_NONE ) {
			switch ( static_cast<int>(result) ) {
			case 0:
				m_bEnableStage = TRUE;
				iRet = STAGE_ERR_NONE;
				break;
			case XSENSOR_ERROR_VALUE:
				iRet = STAGE_ERR_XSENSOR;
				break;
			case YSENSOR_ERROR_VALUE:
				iRet = STAGE_ERR_YSENSOR;
				break;
			case STAGE_MOTION_FAILURE_VALUE:
				iRet = STAGE_ERR_MOTION_FAILURE;
				break;
			case STAGE_MOTION_ABORTED_VALUE:
				iRet = STAGE_ERR_MOTION_ABORTED;
				break;
			case COVER_OPENED_VALUE:
				iRet = STAGE_ERR_COVER_OPENED;
				break;
			case STAGE_INIT_FAILURE_VALUE:
			default:
				break;
			}
		}
		else {
			iRet = STAGE_ERR_STAGE_NOT_FOUND;
		}
	}
	return iRet;
}

// =========================================================================
// Name：      InitializeAutoFocus
// Purpose：   オートフォーカス初期化
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:      未だ解析不可能
//
int CStageNTN::InitializeAutoFocus(void)
{
	int iRet;

	m_bEnableAutoFocus = FALSE;

	if ( (iRet = AdjustElevator(0L)) != STAGE_ERR_NONE ) {
		return iRet;
	}

	// オートフォーカス確認
	if ( AutoFocusInquire(FALSE) > HWSTCMD_RESULT_AVAILABLE ) {
		DisableAutoFocus();
		return STAGE_ERR_AUTOFOCUS_NOT_FOUND;
	}

	CAuxInt result;
	if ( (iRet = SendCommand(szInitAutoFocus, 0, (PTSTR) result)) == STAGE_ERR_NONE ) {
		switch ( static_cast<int>(result) ) {
		case 0:
			m_bEnableAutoFocus = TRUE;
			iRet = STAGE_ERR_NONE;
			break;
		case ZAXIS_INIT_FAILURE_VALUE:
			iRet = STAGE_ERR_AUTOFOCUS_INIT_FAILURE;
			break;
		case AUTOFOCUS_MOTION_FAILURE:
			iRet = STAGE_ERR_AUTOFOCUS_MOTION_FAILURE;
			break;
		case ZAXIS_HARDWARE_LIMIT:
		case ZAXIS_MOTION_FAILURE_VALUE:
			iRet = STAGE_ERR_ZAXIS_MOTION_FAILURE;
			break;
		case AUTOFOCUS_MOTION_ABORTED_VALUE:
			iRet = STAGE_ERR_AUTOFOCUS_MOTION_ABORTED;
			break;
		default:
			iRet = STAGE_ERR_AUTOFOCUSTURRET_INIT_FAILURE;
			break;
		}
#ifndef _DISABLETHREADSPOS
		m_lElevatorSoftHomePos = GetElevatorPos();
#else
		GetElevatorPos(&m_lElevatorSoftHomePos);
#endif
		{
			// m_lElevatorSoftHomePos += ???
		}
	}
	return iRet;
}

// =========================================================================
// Name：      InitializeTurret
// Purpose：   ターレット初期化
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:      未だ解析不可能
//
int CStageNTN::InitializeTurret(void)
{
	int iRet;

	m_bEnableTurret = FALSE;

	if ( TurretInquire(FALSE) > HWSTCMD_RESULT_AVAILABLE ) {
		DisableTurret();
		return STAGE_ERR_TURRET_NOT_FOUND;
	}
	CAuxInt result;
	if ( (iRet = SendCommand(szInitTurret, 0, (PTSTR) result)) == STAGE_ERR_NONE ) {
		if ( static_cast<int>(result) == 0 ) {
			m_bEnableTurret = TRUE;
		}
		else {
			iRet = STAGE_ERR_TURRET_INIT_FAILURE;
		}
	}
	return iRet;
}

// =========================================================================
// Name：      InitializeAutoFocusTurret
// Purpose：   オートフォーカス及びターレット初期化
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:      未だ解析不可能
//
int CStageNTN::InitializeAutoFocusTurret(void)
{
	int iRet = STAGE_ERR_INVALID_AUTOFOCUSTURRET;

	m_bEnableAutoFocus = m_bEnableTurret = FALSE;
	m_nSelectLens = -1;

	if ( (iRet = AdjustElevator(0L)) != STAGE_ERR_NONE ) {
		return iRet;
	}

	UINT uFlags = 0;
	// オートフォーカス確認
	iRet = AutoFocusInquire(FALSE);
	if ( iRet >= HWSTCMD_RESULT_RUN  &&
		 iRet < HWSTCMD_RESULT_SENSORERROR ) {
		uFlags |= HW_AUTOFOCUS;
	}
	// ターレット確認
	iRet = TurretInquire(FALSE);
	if ( iRet >= HWSTCMD_RESULT_RUN  &&
		 iRet < HWSTCMD_RESULT_SENSORERROR ) {
		uFlags |= HW_TURRET;
	}

	CAuxInt result;
	switch ( uFlags ) {
	case (HW_TURRET):
#if 0
		if ( (iRet = SendCommand(szInitTurret, 0, (PTSTR) result)) == STAGE_ERR_NONE ) {
			if ( static_cast<int>(result) == 0 ) {
				m_bEnableTurret = TRUE;
			}
			else {
				iRet = STAGE_ERR_TURRET_INIT_FAILURE;
			}
			DisableAutoFocus();
		}
#else
		if ( InitializeTurret() == STAGE_ERR_NONE ) {
			DisableAutoFocus();
		}
		else {
			iRet = STAGE_ERR_TURRET_INIT_FAILURE;
		}
#endif
		break;
	case (HW_AUTOFOCUS):
		DisableTurret();
	case (HW_AUTOFOCUS | HW_TURRET):
		if ( (iRet = SendCommand(szInitAutoFocusTurret, 0, (PTSTR) result)) == STAGE_ERR_NONE ) {
			switch ( static_cast<int>(result) ) {
			case 0:
				m_bEnableAutoFocus = m_bEnableTurret = TRUE;
				break;
			case ZAXIS_INIT_FAILURE_VALUE:
			case AUTOFOCUS_MOTION_FAILURE:
			case ZAXIS_HARDWARE_LIMIT:
			case ZAXIS_MOTION_FAILURE_VALUE:
			case AUTOFOCUS_MOTION_ABORTED_VALUE:
				iRet = ConvertErrorCode(static_cast<int>(result));
				break;
			default:
				iRet = STAGE_ERR_AUTOFOCUSTURRET_INIT_FAILURE;
				break;
			}
		}
		break;
	default:
		DisableTurret();
		DisableAutoFocus();
		break;
	}
#ifndef _DISABLETHREADSPOS
	m_lElevatorSoftHomePos = GetElevatorPos();
#else
	GetElevatorPos(&m_lElevatorSoftHomePos);
#endif
	{
		// m_lElevatorSoftHomePos += ???
	}
	return iRet;
}

// =========================================================================
// Name：      GetVacuumStatus
// Purpose：   吸着確認
// Parameters: None
//
// Returns:    int - 0 = ON
//                   1 = OFF
//                以外 = エラー
// Notes:      未だ解析不可能
//
int CStageNTN::GetVacuumStatus(void)
{
	return HardwareStatusInquire(HWSTCMD_PARAMS_VACUUM);
}

#ifndef _DISABLETHREADSPOS
// =========================================================================
// Name：      GetXyPos
// Purpose：   現在値取得
// Parameters: None
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
UINT CStageNTN::MonitorThreadProc(void)
{
	static device_status emergency;

	while ( m_bMonitorRun ) {
		CtrlSpeedLimit();
#if 0
		if ( bFirstTime ) {
			bFirstTime = FALSE;
			// XY軸
			CAuxXy resultXy;
			if ( SendCommand(szGetPosXy, 0, (PTSTR) resultXy) != STAGE_ERR_NONE ) {
				return STAGE_ERR_COMMUNICATION_FAILURE;
			}
			PhysicalToLogical(resultXy.convert(), (STAGE_COORD*) &m_CurrPos);

			// Z軸
			AutoFocusInquire();

			if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

			CAuxZ result;
			if ( SendCommand(szGetPosZ, 0, (PSTR) result) != STAGE_ERR_NONE ) {
				return STAGE_ERR_COMMUNICATION_FAILURE;
			}
			m_CurrPos.lZ = *result.convert();
		}
		else {
			CAuxXyz resultXyz;
			if ( SendCommand(szGetPosXyz, 0, (PTSTR) resultXyz) != STAGE_ERR_NONE ) {
				return STAGE_ERR_COMMUNICATION_FAILURE;
			}
			PhysicalToLogical(resultXyz.convert(), &m_CurrPos);
		}
#else
		// 非常停止
		if ( emergency.detect(nexioIsEmergencyStop()).is_down() ) {
			m_bValid = FALSE;
		}

#if 0	// modified hmenjo 20090416 ---------- { -----
		InquirePos();
#else	// modified hmenjo 20090416 ----------
// matsuhisa 2013.01.21 added >>>
		if ( m_bEnableJoystick ) {
// matsuhisa 2013.01.21 added <<<
		if ((STAGETYPE_SPT != gc_iStageType)
		 || ((STAGETYPE_SPT == gc_iStageType) && (TRUE == IsInitializedStageSTD()))) {
			InquirePos();
		}
// matsuhisa 2013.01.21 added >>>
		}
// matsuhisa 2013.01.21 added <<<
#endif	// modified hmenjo 20090416 ---------- } -----
#endif
#if 1	// added hmenjo 20090416 ---------- { -----
		if (STAGETYPE_SPT == gc_iStageType) {
			InquirePosSPT();
		}
#endif	// added hmenjo 20090416 ---------- } -----
		Sleep(100);
	}
	return 0L;
}

int CStageNTN::InquirePos(void)
{
	static BOOL bFirstTime = TRUE;

	if ( bFirstTime ) {
		bFirstTime = FALSE;
#if 1	// added hmenjo 20090416 ---------- { -----
	if (STAGETYPE_SPT != gc_iStageType) {
#endif	// added hmenjo 20090416 ---------- } -----
		// XY軸
		CAuxXy resultXy;
		if ( SendCommand(szGetPosXy, 0, (PTSTR) resultXy) != STAGE_ERR_NONE ) {
			return STAGE_ERR_COMMUNICATION_FAILURE;
		}

		if ( !resultXy.IsValid() ) {
			return STAGE_ERR_COMMUNICATION_FAILURE;
		}

		PhysicalToLogical(resultXy.convert(), (STAGE_COORD*) &m_CurrPos);
#if 1	// added hmenjo 20090416 ---------- { -----
	}
#endif	// added hmenjo 20090416 ---------- } -----

		// Z軸
		AutoFocusInquire();

		if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

		CAuxZ result;
		if ( SendCommand(szGetPosZ, 0, (PSTR) result) != STAGE_ERR_NONE ) {
			return STAGE_ERR_COMMUNICATION_FAILURE;
		}

		if ( !result.IsValid() ) {
			return STAGE_ERR_COMMUNICATION_FAILURE;
		}

		m_CurrPos.lZ = *result.convert();
	}
	else {
		CAuxXyz resultXyz;
		if ( SendCommand(szGetPosXyz, 0, (PTSTR) resultXyz) != STAGE_ERR_NONE ) {
			return STAGE_ERR_COMMUNICATION_FAILURE;
		}

		if ( !resultXyz.IsValid() ) {
			return STAGE_ERR_COMMUNICATION_FAILURE;
		}

#if 0	// modified hmenjo 20090416 ---------- { -----
		PhysicalToLogical(resultXyz.convert(), &m_CurrPos);
#else	// modified hmenjo 20090416 ----------
		if (STAGETYPE_SPT != gc_iStageType) {
			PhysicalToLogical(resultXyz.convert(), &m_CurrPos);
		} else {
			STAGE_COORD_XYZ l_CurrPos;
			PhysicalToLogical(resultXyz.convert(), &l_CurrPos);
			m_CurrPos.lZ = l_CurrPos.lZ;
		}
#endif	// modified hmenjo 20090416 ---------- } -----
	}

	return 0L;
}

#else
// =========================================================================
// Name：      GetXyPos
// Purpose：   現在値取得
// Parameters: pPos - 位置取得バッファ
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
int CStageNTN::GetPos(STAGE_COORD* pPos)
{
	CAuxXy result;
	if ( SendCommand(szGetPosXy, 0, (PTSTR) result) != STAGE_ERR_NONE ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

	if ( !result.IsValid() ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

	PhysicalToLogical(result.convert(), pPos);

	return STAGE_ERR_NONE;
}

int CStageNTN::GetPos(STAGE_COORD_XYZ* pPos)
{
	CAuxXyz result;
	if ( SendCommand(szGetPosXyz, 0, (PTSTR) result) != STAGE_ERR_NONE ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

	if ( !result.IsValid() ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

	PhysicalToLogical((STAGE_COORD*) result.convert(), (STAGE_COORD*) pPos);

	return STAGE_ERR_NONE;
}

// =========================================================================
// Name：      GetPos
// Purpose：   現在値取得
// Parameters: wAxis - 軸
//             pPos  - 位置取得バッファ
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
int CStageNTN::GetPos(WORD wAxis, long* pPos)
{
	int iRet = STAGE_ERR_NONE;

	if ( wAxis == Z ) {
		GetElevatorPos(pPos);
	}
	else {
		STAGE_COORD Pos;
		if ( (iRet = GetPos(&Pos)) == STAGE_ERR_NONE ) {
			*pPos = wAxis == X ? Pos.lX : Pos.lY;
		}
	}

	return iRet;
}

// =========================================================================
// Name：      GetElevatorPos
// Purpose：   エレベーター現在値取得
// Parameters: pPos  - 位置取得バッファ
//
// Returns:    int - STAGE_ERR_NONE = 成功
//
// Notes:
//
int CStageNTN::GetElevatorPos(long* pPos)
{
	int iRet = STAGE_ERR_NONE;

	AutoFocusInquire();

	if ( !IsEnableAutoFocus() ) return STAGE_ERR_INVALID_AUTOFOCUS;

	CAuxZ result;
	if ( SendCommand(szGetPosZ, 0, (PSTR) result) != STAGE_ERR_NONE ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

	if ( !result.IsValid() ) {
		return STAGE_ERR_COMMUNICATION_FAILURE;
	}

	*pPos = *result.convert();

	return iRet;
}
#endif // _DISABLETHREADSPOS

// =========================================================================
// Name：      SetPos
// Purpose：   指令座標設定
// Parameters: wAxis - 軸
//             lPos  - 指令座標
//
// Returns:    None
//
// Notes:
//
void CStageNTN::SetPos(WORD wAxis, long lPos)
{
	switch ( wAxis ) {
	case X:
		SendCommand(szSetPosX, double(lPos / double(MICROMETRE)));
		break;
	case Y:
		SendCommand(szSetPosY, double(lPos / double(MICROMETRE)));
		break;
	case Z:
		SendCommand(szSetPosZ, double(lPos / double(MICROMETRE)));
		break;
	default:
		assert(FALSE);
		break;
	}
}

// =========================================================================
// Name：      SetPos
// Purpose：   指令座標設定
// Parameters: pPos - 指令座標
//
// Returns:    None
//
// Notes:
//
void CStageNTN::SetPos(STAGE_COORD* pPos)
{
	SetPos(X, pPos->lX);
	SetPos(Y, pPos->lY);
}

void CStageNTN::CtrlSpeedLimit(void)
{
	if ( m_bEnableSpeedLimit ) {
		::EnterCriticalSection(&m_csWait);
		SendCommand(szLimitSpeedEx, 1);
		::LeaveCriticalSection(&m_csWait);
		m_bEnableSpeedLimit = FALSE;
	}
	else
	if ( m_bDisableSpeedLimit ) {
		::EnterCriticalSection(&m_csWait);
		SendCommand(szLimitSpeedEx, 0);
		::LeaveCriticalSection(&m_csWait);
		m_bDisableSpeedLimit = FALSE;
	}
}

void CStageNTN::SetSpeed(WORD wAxis, double dSpeed)
{
	if ( dSpeed <= 0 ) return;

#if 0
	double rate = min(offsetofspeed(&m_MaxStageSpeed, wAxis), dSpeed + 0.05) / offsetofspeed(&m_MaxStageSpeed, wAxis) * 100;
#else
	double rate = min(offsetofspeed(&m_LocalStageSpeed, wAxis), dSpeed + 0.05) / offsetofspeed(&m_LocalStageSpeed, wAxis) * 100;
#endif

	switch ( wAxis ) {
	case X:
		SendCommand(szMoveStageEx, long(rate * 10.0));
		break;
	case Y:
		SendCommand(szMoveStageEx, long(rate * 10.0 + 1000.0));
		break;
	default:
		assert(FALSE);
		break;
	}
}

void CStageNTN::GetEEPromData(LPWORD lpData, DWORD dwOffset, SIZE_T Length)
{
	::CopyMemory(lpData, &m_lpEEProm[dwOffset], Length);
}

void CStageNTN::GetEEPromData(EEPROMDATA* pEEPromData)
{
	if ( !IsValid() ) {
		pEEPromData = NULL;
		return;
	}

	DWORD dwValue;

	// Auto focus initial position
	GetEEPromData((LPWORD) &pEEPromData->AutoFocus.InitPos, AF_INIT_POS, sizeof(pEEPromData->AutoFocus.InitPos));
	GetEEPromData((LPWORD) &pEEPromData->AutoFocus.ppu, AF_PULSE_PER_UM, sizeof(pEEPromData->AutoFocus.ppu));
	// Auto focus maximum position
	GetEEPromData((LPWORD) &pEEPromData->AutoFocus.MaxPos, AF_MAX_POS_Z, sizeof(pEEPromData->AutoFocus.MaxPos));
	// Auto stage low speed(range1, 2)
	GetStageSpeed(AS_LOW_SPEED, AS_RANGE1, &pEEPromData->AutoStage.Speed[0].Low.dx, &pEEPromData->AutoStage.Speed[0].Low.dy);
	GetStageSpeed(AS_LOW_SPEED, AS_RANGE2, &pEEPromData->AutoStage.Speed[1].Low.dx, &pEEPromData->AutoStage.Speed[1].Low.dy);
	// Auto stage high speed(range1, 2)
	GetStageSpeed(AS_HIGH_SPEED, AS_RANGE1, &pEEPromData->AutoStage.Speed[0].Max.dx, &pEEPromData->AutoStage.Speed[0].Max.dy);
	GetStageSpeed(AS_HIGH_SPEED, AS_RANGE2, &pEEPromData->AutoStage.Speed[1].Max.dx, &pEEPromData->AutoStage.Speed[1].Max.dy);
	// Auto stage const speed(range1, 2)
	GetStageSpeed(AS_CONST_SPEED, AS_RANGE1, &pEEPromData->AutoStage.Speed[0].Const.dx, &pEEPromData->AutoStage.Speed[0].Const.dy);
	GetStageSpeed(AS_CONST_SPEED, AS_RANGE2, &pEEPromData->AutoStage.Speed[1].Const.dx, &pEEPromData->AutoStage.Speed[1].Const.dy);
	GetOriginPos(&pEEPromData->AutoStage.Origin);
	GetStageSize(&pEEPromData->AutoStage.Size.x, &pEEPromData->AutoStage.Size.y);

	GetEEPromData((LPWORD) &dwValue, AS_EDGE, sizeof(dwValue));
	pEEPromData->AutoStage.Edge.x = (long) LOWORD(dwValue); pEEPromData->AutoStage.Edge.y = (long) HIWORD(dwValue);

}

int CStageNTN::GetStageSize(long* pStageSizeX, long* pStageSizeY)
{
	if ( !IsValid() ) return STAGE_ERR_INVALID_PORT;

	DWORD dwStageSize;
	GetEEPromData((LPWORD) &dwStageSize, AS_MAX_SIZE, sizeof(dwStageSize));
	*pStageSizeX = (long) LOWORD(dwStageSize); *pStageSizeY = (long) HIWORD(dwStageSize);

	return STAGE_ERR_NONE;
}

int CStageNTN::GetOriginPos(STAGE_COORD* pPos)
{
	if ( !IsValid() ) return STAGE_ERR_INVALID_PORT;

	DWORD dwOriginPos;
	GetEEPromData((LPWORD) &dwOriginPos, AS_ORG, sizeof(dwOriginPos));
	pPos->lX = (long) LOWORD(dwOriginPos) * (MICROMETRE / 10);
	pPos->lY = (long) HIWORD(dwOriginPos) * (MICROMETRE / 10);

	return STAGE_ERR_NONE;
}

/* added hmenjo 2009.05.25 SPT ステージ用設定対応 -------- { -------- */
/* ステージの原点位置オフセットを取得
		ステージの－側方向端から原点(０点)までの距離です． */
int CStageNTN::GetOrgLoc(long* plStageOrgLocX, long* plStageOrgLocY)
{
	int l_iRc = STAGE_ERR_NONE;

	STAGE_COORD l_OrgPos;
	if (STAGE_ERR_NONE == l_iRc) {
		l_iRc = GetOriginPos(&l_OrgPos);
	}
	STAGE_COORD l_StageSize;
	if (STAGE_ERR_NONE == l_iRc) {
		l_iRc = GetStageSize(&l_StageSize.lX, &l_StageSize.lY);
		if (STAGE_ERR_NONE == l_iRc) {
			l_StageSize.lX *= (MICROMETRE / 10);
			l_StageSize.lY *= (MICROMETRE / 10);
		}
	}
	if (STAGE_ERR_NONE == l_iRc) {
		WORD l_wXMode1_2;
		WORD l_wYMode1_2;
		GetEEPromData(&l_wXMode1_2, 64, sizeof(WORD));
		GetEEPromData(&l_wYMode1_2, 65, sizeof(WORD));
		BYTE l_ucXMode1 = LOBYTE(l_wXMode1_2);
		BYTE l_ucYMode1 = LOBYTE(l_wYMode1_2);
		if (0 == (l_ucXMode1 & 0x40)) {
			// 1パルス方式の場合
			if (0 == (l_ucXMode1 & 0x10)) {
				// 正転 (DIR信号がLで正転)
				*plStageOrgLocX = l_OrgPos.lX;
			} else {
				// 逆転 (DIR信号がLで逆転)
				*plStageOrgLocX = l_StageSize.lX - l_OrgPos.lX;
			}
		} else {
			// 2パルス方式の場合
			if (0 == (l_ucXMode1 & 0x20)) {
				// 正転 (DIR信号：CWパルス，PULSE信号：CCWパルス)
				*plStageOrgLocX = l_OrgPos.lX;
			} else {
				// 逆転 (DIR信号：CCWパルス，PULSE信号：CWパルス)
				*plStageOrgLocX = l_StageSize.lX - l_OrgPos.lX;
			}
		}
		if (0 == (l_ucYMode1 & 0x40)) {
			// 1パルス方式の場合
			if (0 == (l_ucYMode1 & 0x10)) {
				// 正転 (DIR信号がLで正転)
				*plStageOrgLocY = l_OrgPos.lY;
			} else {
				// 逆転 (DIR信号がLで逆転)
				*plStageOrgLocY = l_StageSize.lY - l_OrgPos.lY;
			}
		} else {
			// 2パルス方式の場合
			if (0 == (l_ucYMode1 & 0x20)) {
				// 正転 (DIR信号：CWパルス，PULSE信号：CCWパルス)
				*plStageOrgLocY = l_OrgPos.lY;
			} else {
				// 逆転 (DIR信号：CCWパルス，PULSE信号：CWパルス)
				*plStageOrgLocY = l_StageSize.lY - l_OrgPos.lY;
			}
		}
	}

	return l_iRc;
}
/* added hmenjo 2009.05.25 SPT ステージ用設定対応 -------- } -------- */

int CStageNTN::GetStageSpeed(unsigned short Type, unsigned short Range, double* x, double* y)
{
	if ( !IsValid() ) return STAGE_ERR_INVALID_PORT;

	DWORD dwRange[2];
	DWORD dwPulse;
	DWORD dwSpeed;
	WORD range;

	GetEEPromData((LPWORD) &dwRange, Range, sizeof(dwRange));
	GetEEPromData((LPWORD) &dwPulse, AS_PLS_PER_10MM, sizeof(dwPulse));
	GetEEPromData((LPWORD) &dwSpeed, Type, sizeof(dwSpeed));

	// x
	range = LOWORD(dwRange[1]) == 0 ? LOWORD(dwRange[0]) : LOWORD(dwRange[1]);
	*x = 500.0 / range * LOWORD(dwSpeed) / LOWORD(dwPulse) * 10.0;
	// y
	range = HIWORD(dwRange[1]) == 0 ? HIWORD(dwRange[0]) : HIWORD(dwRange[1]);
	*y = 500.0 / range * HIWORD(dwSpeed) / HIWORD(dwPulse) * 10.0;

	return STAGE_ERR_NONE;
}

int CStageNTN::GetMaxStageSpeed(double* x, double* y)
{
	return GetStageSpeed(AS_HIGH_SPEED, AS_RANGE1, x, y);
}

void CStageNTN::SerialInterfaceCtrl(BOOL bOn)
{
	pinterface->_flush(AUX_PORT);

	int iRetries = 2;
	while ( iRetries-- > 0 ) {
		if ( SendCommand(szMoveStageEx, bOn ? MSEX_RS232C_ON : MSEX_RS232C_OFF) == STAGE_ERR_NONE ) {
			return;
		}
		else {
			pinterface->_flush(AUX_PORT);
		}
	}
	m_wPortNo = STAGE_ERR_INVALID_ACCESS;
}

void CStageNTN::SetLocalSpeed(double x, double y)
{
	m_LocalStageSpeed.dx = m_MaxStageSpeed.dx * x;
	m_LocalStageSpeed.dy = m_MaxStageSpeed.dy * y;
}

// 2013.02.15 bagus Substrate thickness setting -->
// =========================================================================
// Name：      SetZSubThickOffset
// Purpose：   Z オフセット
// Parameters: lZOffset
//
// Returns:    None
//
// Notes:
//
void CStageNTN::SetZLensOffset(long lZOffset)
{
	m_lZLensOffSet = lZOffset;

	STAGE_COORD Pos;
	GetPos(&Pos);

	AdjustElevator(FALSE, &Pos);
}

// =========================================================================
// Name：      SetZSubThickOffset
// Purpose：   Z オフセット
// Parameters: lZOffset
//
// Returns:    None
//
// Notes:
//
void CStageNTN::SetZSubThickOffset(long lZOffset)
{
	m_lZSubThickOffSet = lZOffset;

	STAGE_COORD Pos;
	GetPos(&Pos);

	AdjustElevator(FALSE, &Pos);
}

// =========================================================================
// Name：      SetAFInvalidRegion
// Purpose：   オートフォーカス無効領域
// Parameters: dVal
//
// Returns:    None
//
// Notes:
//
void CStageNTN::SetAFInvalidRegion(double dVal)
{
	int iVal;

	iVal = (int)(dVal / 10.0);		// convert unit um -> 10um

	SendCommand(szAFInvalidRegion, iVal);
}
// 2013.02.15 bagus Substrate thickness setting <--
