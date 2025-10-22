#if !defined( _STAGE_H_ )
#define _STAGE_H_

#pragma once

#pragma warning (disable : 4786)

#include <windows.h>
#include <process.h>
#include <crtdbg.h>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include "..\\..\\INC\\utility.hxx"
#include "..\\..\\INC\\globals.hxx"
#include "..\\..\\INC\\iobase.hxx"
#include "..\\..\\INC\\stageerr.hxx"
#include "..\\..\\INC\\stageint.hxx"
#include "..\\..\\INC\\configfile.hxx"
#include "..\\..\\inc\\nexmisc.hxx"

#define DEGREE0 								(0.0)
#define DEGREE90								(90.0)
#define DEGREE180								(180.0)
#define DEGREE270								(270.0)

#define MAX_NEAREST_NEIGHBORS					(4)

#if !defined( PI )
#define PI (4 * atan(1))
#endif
#ifndef rad
	#define rad(deg) ((double)deg * PI / DEGREE180)
#endif
#ifndef offsetofcoord
	#define offsetofcoord(p, a) *(long*)((PBYTE)p + (offsetof(STAGE_COORD_XYZ, lX) + (a) * sizeof(long)))
#endif
#ifndef offsetofspeed
	#define offsetofspeed(p, a) *(double*)((PBYTE)p + (offsetof(STAGE_SPEED, dx) + (a) * sizeof(double)))
#endif
#ifndef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

/* added 2009.07.07 hmenjo dll ���΃p�X�Ή� Stage.dll ---------- { ---------- */
extern TCHAR g_tszProcDir[_MAX_PATH];			/* �ďo���v���Z�X�̃f�B���N�g��('\'�t��)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];			/* ��f�B���N�g��('\'�t��)*/
extern TCHAR g_tszLogDir[_MAX_PATH];			/* ���O �f�B���N�g��('\'�t��)*/
extern TCHAR g_tszMapFilePath[_MAX_PATH];		/* �}�b�v�t�@�C����*/
/* added 2009.07.07 hmenjo dll ���΃p�X�Ή� Stage.dll ---------- } ---------- */
/*
 *	Stage.dll ���L�f�[�^ -----------------------------------------------------
 */
#pragma comment(linker, "/section:StageData,rws")
#ifdef _STAGE_MASTER_C_
	#pragma data_seg("StageData")
	#define	STAGE_C_EXT
#else
	#define	STAGE_C_EXT	extern
#endif

STAGE_C_EXT	int	volatile gc_iStageType			/* �X�e�[�W�^�C�v				*/
#ifdef _STAGE_MASTER_C_
= STAGETYPE_NONE
#endif
;
STAGE_C_EXT	BOOL volatile gc_bInMotionMS[8]		/* �����쒆�t���O				*/
#ifdef _STAGE_MASTER_C_
= {0}
#endif
;
STAGE_C_EXT	DWORD volatile gc_dwMotionStop[8]	/* ����~���R					*/
#ifdef _STAGE_MASTER_C_
= {0}
#endif
;
STAGE_C_EXT	DWORD volatile gc_dwMotSysComErr	/* MotSys �ʐM �G���[�R�[�h		*/
#ifdef _STAGE_MASTER_C_
= {0}
#endif
;
STAGE_C_EXT	DWORD volatile gc_dwMotSysErr		/* MotSys �V�X�e�� �G���[�R�[�h	*/
#ifdef _STAGE_MASTER_C_
= {0}
#endif
;

#ifdef _STAGE_MASTER_C_
	#pragma data_seg()
#endif	// -------------------------------------------------------------------

// =========================================================================
// class : CStageMap
// =========================================================================
class CStageMap
{
public:
	CStageMap() : m_bValid(ReadMapFile()) {}
	virtual ~CStageMap() {}
	inline BOOL IsValid(void) { return m_bValid; }
	double InterpolateFromNeighbors(STAGE_COORD* pos) {
		if ( !m_bValid ) return 0.0;

		int index = 0;
		std::multimap<double, int> neighbors;
		for ( std::vector<stage_map>::iterator first(m_entries.begin()), last(m_entries.end()); first != last; ++first ) {
			double dx = (*first).dx - (pos->lX / 1000.0);
			double dy = (*first).dy - (pos->lY / 1000.0);
			double distance = (dx * dx) + (dy * dy);
			neighbors.insert(std::make_pair(distance, index++));
		}

		double weighted;
		double weighted_sum = 0, sum = 0;
		std::multimap<double, int>::iterator iter(neighbors.begin());
		for ( int count = 0; count < MAX_NEAREST_NEIGHBORS; count++ ) {
			if ( (*iter).first == 0 ) {
				weighted_sum = m_entries[(*iter).second].height;
				sum = 1.0;
				break;
			}
			else {
				weighted = 1 / (*iter).first;
				weighted_sum += m_entries[(*iter).second].height * weighted;
				sum += weighted;
			}
			iter++;
		}
		return (double) (weighted_sum / sum - m_min);
	}
private:
	class stage_map
	{
	public:
		double dx;
		double dy;
		double height;
	};
	std::vector<stage_map> m_entries;
	BOOL m_bValid;
	double m_min;

	BOOL ReadMapFile(void) {
/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� Stage.dll ---------- { ---------- */
//		  std::ifstream ifs(CFG_DIR STAGE_MAP_FILE_NAME DAT_EXT);
/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� Stage.dll ---------- 			 */
		std::ifstream ifs(g_tszMapFilePath);
/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� Stage.dll ---------- } ---------- */
		if ( !ifs.is_open() )
			return FALSE;

		TCHAR szBuffer[256];
		double* xyz;
		stage_map data;
		while ( ifs.getline(szBuffer, sizeof(szBuffer)) ) {
			xyz = reinterpret_cast<double*>(&data);
			for ( PCSTR pszToken = _tcstok(szBuffer, SPACE TAB LF); pszToken; pszToken = _tcstok(NULL, SPACE TAB LF) ) {
#if 1
				*xyz++ = atof(pszToken);
#else
				*xyz++ = atol(pszToken) / 1000.0;
#endif
			}
			m_min = m_entries.empty() ? data.height : min(m_min, data.height);
			m_entries.push_back(data);
		}
		return m_entries.size() < MAX_NEAREST_NEIGHBORS ? FALSE : TRUE;
	}
};

//===========================================================================
// class : CAbstractStage
//===========================================================================
class CAbstractStage
{
public:
	CAbstractStage();
	virtual ~CAbstractStage();

	// �X�e�[�^�X�擾
	inline BOOL IsValid(void) { return m_bValid; }
#if 0	// modified hmenjo 20090507 ---------- { -----
	inline BOOL IsIdle(void) { return m_bIdle; }
#else	// modified hmenjo 20090507 ----------
	virtual BOOL IsIdle(void) { return m_bIdle; }
#endif	// modified hmenjo 20090507 ---------- } -----
	inline BOOL IsAutoFocusInProgress(void) { return m_bAutoFocusInProgress; }
	inline BOOL IsEnableStage(void) { return m_bEnableStage; }
	inline BOOL IsEnableAutoFocus(void) { return m_bEnableAutoFocus; }

	// ���x���~�b�g����
#if 0	// modified hmenjo 20090507 ---------- { -----
	inline void EnableSpeedLimit(void)
#else	// modified hmenjo 20090507 ----------
	virtual void EnableSpeedLimit(void)
#endif	// modified hmenjo 20090507 ---------- } -----
	{
		m_bDisableSpeedLimit = FALSE;
		m_bEnableSpeedLimit  = TRUE;
	}
#if 0	// modified hmenjo 20090507 ---------- { -----
	inline void DisableSpeedLimit(void)
#else	// modified hmenjo 20090507 ----------
	virtual void DisableSpeedLimit(void)
#endif	// modified hmenjo 20090507 ---------- } -----
	{
		m_bEnableSpeedLimit  = FALSE;
		m_bDisableSpeedLimit = TRUE;
	}

#ifndef _DISABLETHREADSPOS
	inline void GetPos(STAGE_COORD* pPos) { ::CopyMemory(pPos, &m_CurrPos, sizeof(STAGE_COORD)); }
	inline void GetPos(STAGE_COORD_XYZ* pPos) { ::CopyMemory(pPos, &m_CurrPos, sizeof(STAGE_COORD_XYZ)); }
	inline long GetPos(WORD wAxis) { return offsetofcoord(&m_CurrPos, wAxis); }
	inline long GetElevatorPos(void) { return m_CurrPos.lZ; }
#else
	virtual int GetPos(STAGE_COORD* pPos) { return 0; }
	virtual int GetPos(STAGE_COORD_XYZ* pPos) { return 0; }
	virtual int GetPos(WORD wAxis, long* pPos) { return 0; }
	virtual int GetElevatorPos(long* pPos) { return 0; }
#endif
	inline UINT GetElevatorSpeed(double** ppElevatorSpeeds) {
		if ( m_pElevatorSpeeds != NULL ) {
			*ppElevatorSpeeds = m_pElevatorSpeeds;
			return m_nTotalElevatorSpeeds;
		}
		else {
			*ppElevatorSpeeds = NULL;
			return 0L;
		}
	}

public:
	virtual BOOL Initialize(void) { return FALSE; }
	virtual int InitializeStage(void) { return 0; }
	virtual int InitializeAutoFocus(void) { return 0; }
	virtual int InitializeTurret(void) { return 0; }

	virtual BOOL IsStageIdle(void) { return FALSE; }
	virtual BOOL IsElevatorIdle(void) { return FALSE; }
	virtual int StageMove(STAGE_COORD* pPos, UINT nMode) { return 0; }
	virtual int StageMoveEx(LPMOTPARAMSEX lpmpx, BOOL bWait = TRUE) { return 0; }
	virtual int StageStop(void) { return 0; }

	virtual int AdjustElevator(BOOL bZero, STAGE_COORD* pPos) { return 0; }
	virtual int AdjustElevator(long lOffset = 0L) { return 0; }
	virtual int ElevatorMove(long lPos) { return 0; }
	virtual int ElevatorMoveEx(LPMOTPARAMSEX lpmpx, BOOL bWait = TRUE) { return 0; }
	virtual int ElevatorMoveAtSpeed(double dSpeed) { return 0; }
	virtual int ElevatorMoveAtSpeedEx(double dSpeed) { return 0; }
	virtual int ElevatorStop(void) { return 0; }
	virtual int ElevatorMoveRelative(long lPos) { return 0; }

	virtual int DoAutoFocus(void) { return 0; }
	virtual int AbortAutoFocus(void) { return 0; }

	virtual int FocosMoveAtSpeed(double* pSpeed) { return 0; }
	virtual int MoveAbsolute(WORD wAxis, long lPos) { return 0; }
	virtual int MoveAbsolute(STAGE_COORD* pPos) { return 0; }
	virtual int MoveAbsoluteEx(WORD wAxis, long lPos, BOOL bWait = TRUE) { return 0; }
	virtual int MoveAbsoluteEx(STAGE_COORD* pPos, BOOL bWait = TRUE) { return 0; }
	virtual int MoveRelative(WORD wAxis, long lPos, BOOL bWait = TRUE) { return 0; }
	virtual int MoveRelative(STAGE_COORD* pPos, BOOL bWait = TRUE) { return 0; }
	virtual int MoveAtSpeed(WORD, double) { return FALSE; }
#if 1	// modified hmenjo 20090415 ---------- { -----
	virtual void ShowMotSys(BOOL bShow) {}
	virtual int EnableJoystickSPT(BOOL bEnable = TRUE, int iSpeedSel = 1) {return 0;}
	virtual int StageStopAxis(WORD wAxis) {return 0;}
	virtual BOOL IsAxisIdle(WORD wAxis) {return FALSE;}
	virtual int MoveAbsoluteAtSpeedEx(WORD wAxis, long lPos, double, BOOL bWait = TRUE) {return 0;}
	virtual int MoveAbsoluteAtSpeedEx(STAGE_COORD* pPos, double, BOOL bWait = TRUE) {return 0;}
	virtual int MoveRelativeAtSpeed(WORD wAxis, long lPos, double, BOOL bWait = TRUE) {return 0;}
	virtual int MoveRelativeAtSpeed(STAGE_COORD* pPos, double, BOOL bWait = TRUE) {return 0;}
	virtual BOOL GetJoyStickSPT(void) {return 0;}
	virtual int MoveAtSpeedEx(WORD, double, BOOL bWait = TRUE) {return 0;}
	virtual int GetElectronicGear(WORD wAxis, long* plEleGearAB) {return 0;}
	virtual int SetFactor(WORD wAxis, long lFactor) {return FALSE;}
	virtual int GetOrgLoc(long* plStageOrgLocX, long* plStageOrgLocY) {return FALSE;}
	virtual int SetJoyStickSpeedSPT(WORD wAxis, int iSpeedSel, double dSpeed) {return FALSE;}
#endif	// modified hmenjo 20090415 ---------- } -----

	virtual BOOL VacuumCtrl(BOOL) { return FALSE; }
	virtual int DisableAutoFocus(void) { return 0; }
	virtual int DisableStage(void) { return 0; }
	virtual int DisableJoystick(BOOL bDisable = TRUE) { return 0; }
	virtual int SelectLens(UINT) { return 0; }
	virtual int GetCurLens(void) { return m_nSelectLens; }
	// �X�e�[�^�X�ύX
	virtual int EnableJoystickMoveElevator(BOOL bEnable = TRUE) { return 0; }
	virtual void SetLocalSpeed(double x, double y) {}
// 2013.02.15 bagus Substrate thickness setting -->
	virtual void SetZLensOffset(long lZOffset) {}
	virtual void SetZSubThickOffset(long lZOffset) {}
	virtual void SetAFInvalidRegion(double dVal) {}
// 2013.02.15 bagus Substrate thickness setting <--

	// �X�e�[�^�X�擾
	virtual void GetEEPromData(EEPROMDATA* pEEPromData) {}
	virtual int GetStageSize(long* pStageSizeX, long* pStageSizeY) { return 0; }
	virtual int GetOriginPos(STAGE_COORD* pPos) { return 0; }
	virtual int GetMaxStageSpeed(double* x, double* y) { return 0; }

	virtual int GetVacuumStatus(void) { return 0; }

protected:
	void LogicalToPhysical(STAGE_COORD* pSrc, STAGE_COORD* pDst);
	void LogicalToPhysical(STAGE_COORD* pPos);
	void PhysicalToLogical(STAGE_COORD* pSrc, STAGE_COORD* pDst);
	void PhysicalToLogical(STAGE_COORD_XYZ*, STAGE_COORD_XYZ*);

	inline long round(double val) {
		return (long) (val > 0 ? val + 0.5 : val - 0.5);
	}

	struct PARAMS
	{
//		  STAGE_CONFIG Stage;
		SYSTEM_CONFIG SystemConfig;
		EDGE_ALIGNER EdgeAligner;
	} m_Params;
	BOOL m_bValid;
	BOOL m_bIdle;
	BOOL m_bAutoFocusInProgress;
	BOOL m_bAbortAutoFocus;
	BOOL m_bEnableStage;
	BOOL m_bEnableAutoFocus;
	BOOL m_bEnableTurret;
	BOOL m_bEnableJoystick;

	int m_nSelectLens;

	double m_dCurrSpeeds[MAX_AXIS];
	UINT m_nTotalElevatorSpeeds;
	double* m_pElevatorSpeeds;
	STAGE_SPEED m_MaxStageSpeed;
	STAGE_SPEED m_LocalStageSpeed;
	STAGE_COORD m_EdgePos[2];
// 2013.02.15 bagus Substrate thickness setting -->
	long m_lZLensOffSet;
	long m_lZSubThickOffSet;
// 2013.02.15 bagus Substrate thickness setting <--

	CLogFile m_LogFile;
	CStageMap m_StageMap;
	long m_lElevatorSoftHomePos;

#ifndef _DISABLETHREADSPOS
	// ���݈ʒu��񃂃j�^
	void BeginMonitor(void) {
		if ( m_hMonitorThread == NULL ) {
			m_bMonitorRun = TRUE;
			m_hMonitorThread = (HANDLE)_beginthreadex(NULL, 0, MonitorThreadProc, LPVOID(this), 0, &m_nMonitorThreadID);
			if ( m_hMonitorThread == NULL ) {
				_RPTF0(_CRT_WARN, _TEXT("Unable to create monitor thread\n"));
				return;
			}
		}
	}

	static UINT WINAPI MonitorThreadProc(LPVOID lpData) {
		CAbstractStage* pThis = reinterpret_cast<CAbstractStage*>(lpData);

		return pThis->MonitorThreadProc();
	}
	virtual UINT MonitorThreadProc(void) { return 0L; }
	STAGE_COORD_XYZ m_CurrPos;
	inline void EndMonitor(void) {
		m_bMonitorRun = FALSE;
		::WaitForSingleObject(m_hMonitorThread, INFINITE);
		::CloseHandle(m_hMonitorThread);
		m_hMonitorThread = NULL;
	}
#endif // _DISABLETHREADSPOS
	BOOL m_bMonitorRun;
	HANDLE m_hMonitorThread;
	UINT m_nMonitorThreadID;
	BOOL m_bEnableSpeedLimit;
	BOOL m_bDisableSpeedLimit;
};

//===========================================================================
// class : CStageNTN
//===========================================================================
#define AUX_STDSTRLEN							(256)
#define AUX_MINSTRLEN							(3)

#define AUX_XYFORMATLEN 						(10)
#define AUX_ZFORMATLEN							(9)

#define AUX_PORT								(1)

#define MIN_ENC 								(0.25)

#define HWSTCMD_PARAMS_AUTOFOCUS				0x00
#define HWSTCMD_PARAMS_LENS 					0x01
#define HWSTCMD_PARAMS_STAGE					0x02
#define HWSTCMD_PARAMS_KINDOFSTAGE				0x03
#define HWSTCMD_PARAMS_VACUUM					0x08
#define HWSTCMD_PARAMS_COVER					0x0D
#define HWSTCMD_PARAMS_VIBISOL					0x0E

#define HWSTCMD_RESULT_RUN						0x00
#define HWSTCMD_RESULT_AVAILABLE				0x01
#define HWSTCMD_RESULT_SENSORERROR				0x02
#define HWSTCMD_RESULT_PCBERROR 				0x03

#define HW_AUTOFOCUS							0x0001
#define HW_TURRET								0x0002

#define TURRET_INIT_FAILURE_VALUE				(4)
#define TURRET_MOTION_FAILURE_VALUE 			(4)
#define XSENSOR_ERROR_VALUE 					(10)
#define YSENSOR_ERROR_VALUE 					(11)
#define STAGE_INIT_FAILURE_VALUE				(12)
#define STAGE_MOTION_FAILURE_VALUE				(13)
#define STAGE_MOTION_ABORTED_VALUE				(14)
#define ZAXIS_INIT_FAILURE_VALUE				(21)
#define AUTOFOCUS_MOTION_FAILURE				(22)
#define ZAXIS_HARDWARE_LIMIT					(23)
#define AUTOFOCUS_MOTION_ABORTED_VALUE			(24)
#define ZAXIS_MOTION_FAILURE_VALUE				(25)
#define COVER_OPENED_VALUE						(30)

#define DINP_PROBE_DOWN 						0x01
#define DINP_PROBE_UP							0x02
#define DOUT_TRANSMISSIONSHUTTER				0x20
#define DOUT_PROBE_UPDOWN						0x40
#define DOUT_PROBE_CHANGE						0x80

#define MS_MOVE_XY								(0)
#define MS_MOVE_XY_AND_AUTOFOCUS				(1)
#define MS_MOVE_SLOW							(2)
#define MS_SCAN_XSENSOR 						(3)
#define MS_SCAN_YSENSOR 						(4)
#define MS_STORE_POS1							(5)
#define MS_STORE_POS2							(6)
#define MS_MOVE_XYZ 							(7)

#define MSEX_MOVE_XY							(2001)
#define MSEX_STOP_XY							(2002)
#define MSEX_STATUS_XY							(2003)
#define MSEX_RS232C_ON							(2320)
#define MSEX_RS232C_OFF 						(2321)
#define MSEX_SPEEDS_Z							(2500)
#define MSEX_STATUS_Z							(2520)
#define MSEX_STOP_Z 							(2560)
#define MSEX_MOVE_Z 							(2561)

class CStageNTN : public CAbstractStage
{
public:
	CStageNTN();
	~CStageNTN();

	virtual BOOL IsIdle(void) { return !m_bInMotion && IsStageIdle(); }

#if 0	// modified hmenjo 20090415 ---------- { -----
	virtual BOOL Initialize(void);
	virtual BOOL Initialize(LPVOID);
#else	// modified hmenjo 20090415 ----------
	virtual int Initialize(void);
	virtual int Initialize(LPVOID);
#endif	// modified hmenjo 20090415 ---------- } -----
	virtual int InitializeStage(void);
	virtual int InitializeAutoFocus(void);
	virtual int InitializeTurret(void);
	virtual int InitializeAutoFocusTurret(void);
	virtual int StageMove(STAGE_COORD* pPos, UINT nMode);
	virtual int StageMove(UINT nMode);
	virtual int StageMove(long lX, long lY, UINT nMode);
	virtual int StageMoveEx(LPMOTPARAMSEX lpmpx, BOOL bWait = TRUE);
	virtual int StageStop(void);
	virtual int MoveAbsolute(WORD wAxis, long lPos);
	virtual int MoveAbsolute(STAGE_COORD* pPos);
	virtual int MoveAbsoluteEx(WORD wAxis, long lPos, BOOL bWait = TRUE);
	virtual int MoveAbsoluteEx(STAGE_COORD* pPos, BOOL bWait = TRUE);

	virtual int MoveRelative(WORD wAxis, long lPos, BOOL bWait = TRUE);
	virtual int MoveRelative(STAGE_COORD* pPos, BOOL bWait = TRUE);
	virtual int MoveAtSpeed(WORD wAxis, double dSpeed);

	virtual int AdjustElevator(BOOL bZero, STAGE_COORD* pPos);
	virtual int AdjustElevator(long lOffset = 0L);
	virtual int ElevatorMove(long lPos);
	virtual int ElevatorMoveEx(LPMOTPARAMSEX lpmpx, BOOL bWait = TRUE);
	virtual int ElevatorMoveAtSpeed(double dSpeed);
	virtual int ElevatorMoveAtSpeedEx(double dSpeed);
	virtual int ElevatorStop(void);
	virtual int ElevatorMoveRelative(long lPos);

	virtual int DoAutoFocus(void);
	virtual int DoAutoFocus(LPVOID);
	virtual int AbortAutoFocus(void);

	virtual BOOL IsStageIdle(void);
	virtual BOOL IsElevatorIdle(void);
	virtual BOOL IsValidZAxisEncoder(void);
	virtual BOOL VacuumCtrl(BOOL);
	virtual int DoEdgeAligner(STAGE_COORD* pOffset);
	virtual int SelectLens(UINT);
	virtual int SelectLens(LPVOID);
	virtual int AutoFocusInquire(BOOL bDisable = TRUE);
	virtual int HardwareStatusInquire(UINT);
	virtual int StageInquire(BOOL bDisable = TRUE);
	virtual int TurretInquire(BOOL bDisable = TRUE);
	virtual int DisableAutoFocus(void);
	virtual int DisableStage(void);
	virtual int DisableJoystick(BOOL bDisable = TRUE);
	virtual int DisableTurret(void);

	virtual int EnableJoystickMoveElevator(BOOL bEnable = TRUE);
	virtual void SetLocalSpeed(double x, double y);
// 2013.02.15 bagus Substrate thickness setting -->
	virtual void SetZLensOffset(long lZOffset);
	virtual void SetZSubThickOffset(long lZOffset);
	virtual void SetAFInvalidRegion(double dVal);
// 2013.02.15 bagus Substrate thickness setting <--
	virtual int GetVacuumStatus(void);

	virtual void GetEEPromData(EEPROMDATA* pEEPromData);
	virtual int GetStageSize(long* pStageSizeX, long* pStageSizeY);
	virtual int GetOriginPos(STAGE_COORD* pPos);
	virtual int GetMaxStageSpeed(double* x, double* y);

#ifdef _DISABLETHREADSPOS
	virtual int GetPos(STAGE_COORD* pPos);
	virtual int GetPos(STAGE_COORD_XYZ* pPos);
	virtual int GetPos(WORD wAxis, long* pPos);
	virtual int GetElevatorPos(long* pPos);
#endif

protected:
private:
#ifndef _DISABLETHREADSPOS
	int  InquirePos(void);
#endif
	int GetStageSpeed(unsigned short Type, unsigned short Range, double* x, double* y);
	void SetPos(WORD wAxis, long lPos);
	void SetPos(STAGE_COORD* pPos);
	void SetSpeed(WORD wAxis, double dSpeed);
	void GetEEPromData(LPWORD lpData, DWORD dwOffset, SIZE_T Length);
	void SerialInterfaceCtrl(BOOL bOn);
	void CtrlSpeedLimit(void);

	inline int ConvertErrorCode(int iErrorCode, LPBOOL lpbEnable = NULL) {
		int iStageErrorCode = STAGE_ERR_NONE;

		switch ( iErrorCode ) {
		case 0:
			iStageErrorCode = STAGE_ERR_NONE;
			if ( lpbEnable != NULL )
				*lpbEnable = TRUE;
			break;
		case XSENSOR_ERROR_VALUE:
			iStageErrorCode = STAGE_ERR_XSENSOR;
			break;
		case YSENSOR_ERROR_VALUE:
			iStageErrorCode = STAGE_ERR_YSENSOR;
			break;
		case STAGE_MOTION_FAILURE_VALUE:
			iStageErrorCode = STAGE_ERR_MOTION_FAILURE;
			break;
		case STAGE_MOTION_ABORTED_VALUE:
			iStageErrorCode = STAGE_ERR_MOTION_ABORTED;
			break;
		case STAGE_INIT_FAILURE_VALUE:
			iStageErrorCode = STAGE_ERR_STAGE_INIT_FAILURE;
			break;
		case COVER_OPENED_VALUE:
			iStageErrorCode = STAGE_ERR_COVER_OPENED;
			break;
		case ZAXIS_INIT_FAILURE_VALUE:
			iStageErrorCode = STAGE_ERR_AUTOFOCUS_INIT_FAILURE;
			break;
		case AUTOFOCUS_MOTION_FAILURE:
			iStageErrorCode = STAGE_ERR_AUTOFOCUS_MOTION_FAILURE;
			break;
		case ZAXIS_HARDWARE_LIMIT:
		case ZAXIS_MOTION_FAILURE_VALUE:
			iStageErrorCode = STAGE_ERR_ZAXIS_MOTION_FAILURE;
			break;
		case AUTOFOCUS_MOTION_ABORTED_VALUE:
//			  iStageErrorCode = STAGE_ERR_AUTOFOCUS_MOTION;
			break;
		default:
			break;
		}
		return iStageErrorCode;
	}
	template <class T>
	class aux_result
	{
	public:
		typedef T* pointer;
	public:
		aux_result<T>(PCSTR result_buffer) : buffer(result_buffer) {};
		virtual pointer convert(void) = 0;
		virtual BOOL is_valid (void) { return FALSE; }
	protected:
		int length(void) const {
			return _tcslen(buffer);
		}
		PCSTR buffer;
		T value;
	};
	class aux_int : public aux_result<int>
	{
	public:
		aux_int(PCSTR result_buffer) : aux_result<int>(result_buffer) {};
		pointer convert(void) {
			return &value;
		}
	};
	class aux_xy : public aux_result<STAGE_COORD>
	{
	public:
		aux_xy(PCSTR result_buffer) : aux_result<STAGE_COORD>(result_buffer) {};
		virtual BOOL is_valid (void) { return _tcsnccmp(buffer, "PX", 2) == 0; }
		pointer convert(void) {
			// messege format
            // 0         1         2         3         4
            // 0123456789012345678901234567890
            // PX+0000.0000Y+0000.0000
			if ( length() == 23 ) {
				value.lX = (long) (atof(std::string(buffer).substr(2, AUX_XYFORMATLEN).c_str()) * MICROMETRE);
				value.lY = (long) (atof(std::string(buffer).substr(13, AUX_XYFORMATLEN).c_str()) * MICROMETRE);
			}
            // messege format
            // 0         1         2         3         4
            // 0123456789012345678901234567890
            // PX+000.0000Y+0000.000
			else {
				value.lX = (long) (atof(std::string(buffer).substr(2, AUX_XYFORMATLEN - 1).c_str()) * MICROMETRE);
				value.lY = (long) (atof(std::string(buffer).substr(12, AUX_XYFORMATLEN - 1).c_str()) * MICROMETRE);
			}
			return &value;
		}
		void convert(pointer obj) {
			::CopyMemory(obj, &value, sizeof(STAGE_COORD));
		}
	};
	class aux_xyz : public aux_result<STAGE_COORD_XYZ>
	{
	public:
		aux_xyz(PCSTR result_buffer) : aux_result<STAGE_COORD_XYZ>(result_buffer) {};
		virtual BOOL is_valid (void) { return _tcsnccmp(buffer, "HX", 2) == 0; }
		pointer convert(void) {
			// messege format
            // 0         1         2         3         4
            // 01234567890123456789012345678901234567890
            // HX+0000.0000Y+0000.0000Z+000.0000
			if ( length() == 33 ) {
				value.lX = (long) (atof(std::string(buffer).substr(2, AUX_XYFORMATLEN).c_str()) * MICROMETRE);
				value.lY = (long) (atof(std::string(buffer).substr(13, AUX_XYFORMATLEN).c_str()) * MICROMETRE);
				value.lZ = (long) (atof(std::string(buffer).substr(24, AUX_ZFORMATLEN).c_str()) * MICROMETRE * 10);
			}
            // messege format
            // 0         1         2         3         4
            // 01234567890123456789012345678901234567890
            // HX+000.0000Y+000.0000Z+000.000
			else {
				value.lX = (long) (atof(std::string(buffer).substr(2, AUX_XYFORMATLEN - 1).c_str()) * MICROMETRE);
				value.lY = (long) (atof(std::string(buffer).substr(12, AUX_XYFORMATLEN - 1).c_str()) * MICROMETRE);
				value.lZ = (long) (atof(std::string(buffer).substr(22, AUX_ZFORMATLEN).c_str()) * MICROMETRE * 10);
			}
			return &value;
		}
		void convert(pointer obj) {
			::CopyMemory(obj, &value, sizeof(STAGE_COORD_XYZ));
		}
	};
	class aux_z : public aux_result<long>
	{
	public:
		aux_z(PCSTR result_buffer) : aux_result<long>(result_buffer) {};
		virtual BOOL is_valid (void) { return length() > AUX_ZFORMATLEN; }
		pointer convert(void) {
			// messege format
            // 0         1         2         3         4
            // 0123456789012345678901234567890
            //   +000.0000
#if 1
			value = (long) (atof(std::string(buffer).substr(2, AUX_ZFORMATLEN).c_str()) * MICROMETRE * 10);
#else
			value = (long) (atof(std::string(buffer).substr(2, length() - 3).c_str()) * MICROMETRE * 10);
#endif
			return &value;
		}
	};
	template <class T>
	class CAuxResult
	{
	public:
		CAuxResult() : result(m_szBuffer) {
			clear();
		}
		~CAuxResult() {}
		void clear(void) {
			::ZeroMemory(m_szBuffer, sizeof(m_szBuffer));
		}
		operator BYTE() {
			return (BYTE) operator int();
		}
		operator short() {
			return (short) operator int();
		}
		operator WORD() {
			return (WORD) operator int();
		}
		operator int() {
			return _ttoi(&m_szBuffer[1]);
		}
		operator double() {
			return atof(&m_szBuffer[1]);
		}
		operator PCSTR() {
			return (PCSTR) m_szBuffer;
		}
		operator PTSTR() {
			return (PTSTR) m_szBuffer;
		}
	BOOL IsValid(void) { return result.is_valid(); }
	typename T::pointer convert() {
		return result.convert();
	}
		T* operator->() {
			return &result;
		}
	private:
		TCHAR m_szBuffer[AUX_STDSTRLEN + 1];
		T result;
	};
	typedef CAuxResult<aux_int> CAuxInt;
	typedef CAuxResult<aux_xy>	CAuxXy;
	typedef CAuxResult<aux_xyz> CAuxXyz;
	typedef CAuxResult<aux_z>	CAuxZ;

#if 1	// added hmenjo 20090415 ---------- { -----
		//		StageSPT �p ���z�֐���`
public:
	virtual int InitializeStage_WaitMotSysInit(LPVOID) {return 0;}
	virtual int MotSys_WaitMotionXY(LPVOID pParams) {return 0;}
	virtual int MotSys_WaitMotionAxis(LPVOID pParams) {return 0;}
	virtual int InquirePosSPT(void) {return STAGE_ERR_NONE;}
	virtual BOOL IsInitializedStageSTD(void) {return TRUE;}
	virtual int GetOrgLoc(long* plStageOrgLocX, long* plStageOrgLocY);
#endif	// added hmenjo 20090415 ---------- { -----

protected:
#ifndef _DISABLETHREADSPOS
	// ���݈ʒu��񃂃j�^
		virtual UINT MonitorThreadProc(void);
#endif // _DISABLETHREADSPOS

	// move absolute wait for done thread
	typedef int (CStageNTN::*PSTAGEFUNC)(LPVOID);
	typedef struct _THREADPARAMS
	{
		CStageNTN*		pThis;
		PSTAGEFUNC		pStageFunc;
		LPVOID			lpData;
		int				iRet;
	} THREADPARAMS, *PTHREADPARAMS;
	static UINT WINAPI ThreadProc(LPVOID);
	UINT ThreadProc(PTHREADPARAMS);
	HANDLE m_hThread;
	UINT m_nThreadID;

	int Execute(PSTAGEFUNC pStageFunc, LPVOID lpData = NULL) {
		DWORD dwRet;
		THREADPARAMS thrParams;
#if 1	// modified hmenjo 20090507 ---------- { -----
		HANDLE l_hThread = 0;
		UINT l_uiThreadID = 0;
		BOOL l_bThreadMode = (0 != HIBYTE(LOWORD(lpData)))? TRUE : FALSE;
#endif	// modified hmenjo 20090507 ---------- } -----

		thrParams.pThis 	  = this;
		thrParams.pStageFunc  = pStageFunc;
		thrParams.lpData	  = lpData;
		thrParams.iRet		  = -1;
#if 1	// modified hmenjo 20090507 ---------- { -----
		if (((FALSE != l_bThreadMode) && (0 == l_hThread))
		 || ((FALSE == l_bThreadMode) && (0 == m_hThread))) {
			if (FALSE != l_bThreadMode) {
				l_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, LPVOID(&thrParams), 0, &l_uiThreadID);
			} else {
				m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, LPVOID(&thrParams), 0, &m_nThreadID);
			}
			if (((FALSE != l_bThreadMode) && (0 == l_hThread))
			 || ((FALSE == l_bThreadMode) && (0 == m_hThread))) {
				;
			} else {
				while (1) {
					if (FALSE != l_bThreadMode) {
						dwRet = MsgWaitForMultipleObjects(1, &l_hThread, FALSE, INFINITE, QS_ALLINPUT);
					} else {
						dwRet = MsgWaitForMultipleObjects(1, &m_hThread, FALSE, INFINITE, QS_ALLINPUT);
					}
#else	// modified hmenjo 20090507 ----------
		if ( m_hThread == NULL ) {
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, LPVOID(&thrParams), 0, &m_nThreadID);
			if ( m_hThread == NULL ) {
			}
			else {
				while ( 1 ) {
					dwRet = MsgWaitForMultipleObjects(1, &m_hThread, FALSE, INFINITE, QS_ALLINPUT);
#endif	// modified hmenjo 20090507 ---------- } -----
					if ( dwRet == WAIT_OBJECT_0 ) {
						// event done
						break;
					}
					else if ( dwRet == WAIT_OBJECT_0 + 1 ) {
						// process window message
						DoEvents();
						Sleep(10);
					}
					else {
						// timeout !?
						;
					}
				}
#if 0	// modified hmenjo 20090507 ---------- { -----
				if (FALSE != l_bThreadMode) {
					::CloseHandle(l_hThread);
					l_hThread = NULL;
				} else {
					::CloseHandle(m_hThread);
					m_hThread = NULL;
				}
#else	// modified hmenjo 20090507 ----------
				::CloseHandle(m_hThread);
				m_hThread = NULL;
#endif	// modified hmenjo 20090507 ---------- } -----
			}
		}
		return thrParams.iRet;
	}
	CRITICAL_SECTION m_csWait;
	nano::critical_section		m_wait;
public:
	// aux module...
	BOOL ReceiveCommand(PTSTR pszBuffer) {
		int iRet = STAGE_ERR_NONE;
		short answer;
		long lLength = 0;
		PTSTR psz = pszBuffer;
		BOOL bAbortAutoFocus = FALSE;

		do {
			answer = pinterface->_recv(m_wPortNo);
			lLength++;
			if ( m_bAbortAutoFocus ) {
				pinterface->_send(m_wPortNo, '/');
				answer = TCHAR('s');
				bAbortAutoFocus = TRUE;
				m_bAbortAutoFocus = FALSE;
				break;
			}
			if ( IsAutoFocusInProgress() )
				DoEvents();
		} while ( lLength < 18 && answer == -1 );
		if ( lLength == 18 ) {
			return STAGE_ERR_INVALID_ACCESS;
		}
		if	( pszBuffer != NULL )
			*psz = (TCHAR) answer;

		while ( (answer = pinterface->_recv(m_wPortNo)) != CRCHAR ) {
			if ( answer == -1 ) {
				iRet = STAGE_ERR_INVALID_ACCESS;
				break;
			}
			if ( pszBuffer != NULL ) {
				psz = _tcsinc(psz);
				*psz = (TCHAR) answer;
				if ( _tcslen(psz) == AUX_STDSTRLEN ) {
					iRet = STAGE_ERR_INVALID_ACCESS;
					break;
				}
			}
		}
		if ( bAbortAutoFocus ) {
			pinterface->_recv(m_wPortNo);
		}
		return iRet;
	}
	template <class T>
	BOOL SendCommand(const TCHAR szCmdID, const T& Val, PTSTR pszBuffer = NULL) {

		::EnterCriticalSection(&m_csWait);

		BOOL bRet = SendCommand(std::string(1, szCmdID).c_str(), Val, pszBuffer);

		::LeaveCriticalSection(&m_csWait);

		return bRet;
	}
	template <class T>
	BOOL SendCommand(PCSTR pszCmdID, const T& Val, PTSTR pszBuffer = NULL) {
		int iRet = STAGE_ERR_NONE;
		TCHAR szBuffer[AUX_STDSTRLEN + 1];

		assert(pszCmdID != NULL);

		if ( !IsValid() ) {
			m_bEnableStage = m_bEnableAutoFocus = m_bEnableTurret = m_bEnableJoystick = FALSE;
			return STAGE_ERR_INVALID_PORT;
		}

		if ( _tcscmp(pszCmdID, szLimitSpeedEx) == 0 ) {
			_stprintf(szBuffer, "%s%04d%c", pszCmdID, Val, CRCHAR);
		}
		else {
			switch ( pszCmdID[0] ) {
			case szMoveStageEx:
			case szDoOffsetZ:
				_stprintf(szBuffer, "%s%04d%c", pszCmdID, Val, CRCHAR);
				break;
			case szOutputPort:
				// �����̔�r���Ӗ��s��
				_stprintf(szBuffer, "%s%X%c", pszCmdID, Val, CRCHAR);
				break;
			case szJoystickStatusXy:
			case szSelectLens:
			case szAutoFocusStatus:
			case szStageStatus:
			case szVacuumCtrl:
			case szJoystickStatusZ:
			case szInputPort:
			case szShutterCtrl:
			case szHardwareStatus:
			case szGetExtCounter:
			case szMoveStageXy:
			case szEEPromParam:
				_stprintf(szBuffer, "%s%X" CR, pszCmdID, Val);
				break;
// 2013.02.15 bagus Substrate thickness setting -->
			case szAFInvalidRegion:
				_stprintf(szBuffer, "%s%04X" CR, pszCmdID, Val);
				break;
// 2013.02.15 bagus Substrate thickness setting <--
			case szUpDownZ:
				_stprintf(szBuffer, "%s%02X" CR, pszCmdID, Val);
				break;
			case szSetPosZ:
				_stprintf(szBuffer, "%s%+09.4f" CR, pszCmdID, Val);
				break;
			case szSetPosX:
			case szSetPosY:
				_stprintf(szBuffer, "%s%+010.4f" CR, pszCmdID, Val);
				break;
			case szSetExtCounter:
				_stprintf(szBuffer, "%s%+010d" CR, pszCmdID, Val);
				break;
			case szDoAutoFocus:
				_stprintf(szBuffer, "%s" CR, pszCmdID);
				break;
			case szGetPosXy:
				_stprintf(szBuffer, "%s1" CR, pszCmdID);
				break;
			default:
				_stprintf(szBuffer, "%s" CR, pszCmdID);
				break;
			}
		}

		for ( PCSTR psz = szBuffer; *psz != NULLCHAR; psz = _tcsinc(psz) ) {
			if ( pinterface->_send(m_wPortNo, *psz) != 0 ) {
				pinterface->_send(m_wPortNo, CRCHAR);
				iRet = STAGE_ERR_INVALID_ACCESS;
				break;
			}
		}
#if 0
		if ( iRet == STAGE_ERR_NONE ) {
			iRet = ReceiveCommand(pszBuffer);
		}
#else
		iRet = ReceiveCommand(pszBuffer);
#endif
		if ( iRet != STAGE_ERR_NONE ) {
			pinterface->_flush(m_wPortNo);
		}

		return iRet;
	}

private:
	iobase* pinterface;
	WORD m_wPortNo;
	LPBYTE m_lpEEProm;
	BOOL m_bInterrupt;
	BOOL m_bInMotion;
};

#endif // _STAGE_H_
