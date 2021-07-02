#if !defined( _STAGESPT_H_ )
#define _STAGESPT_H_

enum EXE_SWITCH {
	EXE_START = 0,
	EXE_SHOW,
	EXE_HIDE,
	EXE_QUIT
};

typedef struct {
	long Positive;
	long Negative;
} SOFTLIMIT;

//===========================================================================
// class : CStageSPT ソディックステージクラス
//===========================================================================
class CStageSPT : public CStageNTN
{
public:
	CStageSPT();
	~CStageSPT();
	int InquirePosSPT(void);
	BOOL IsInitializedStageSTD(void);

private:
	HWND m_hwndMotSys;
	HWND m_hwndMotTsk;
	BOOL m_bMotSysInitialized;
	long m_lMaxSpeed[8];
	long m_lSoftHP[8];
	long m_lLocalSpeed[8];
	STAGE_COORD m_CurrPosSPT;
	long m_lFactor[8];
	SOFTLIMIT m_SoftLimitX;
	SOFTLIMIT m_SoftLimitY;

private:
	BOOL MotTskExe(EXE_SWITCH ExeSwitch);
	BOOL MotSysExe(EXE_SWITCH ExeSwitch);
	int InitializeStage_WaitMotSysInit(LPVOID);
	int MotSys_WaitMotionXY(LPVOID pParams);
	int MotSys_WaitMotionAxis(LPVOID pParams);
	UINT MonitorThreadProcSPT(void);

private:		// 外部関数の処理定義
	BOOL m_bIsInitializedStageSTD;
	CRITICAL_SECTION m_csInquirePos;
	virtual BOOL Initialize(void);
	virtual int InitializeStage(void);
	virtual int IsIdle(void);
	virtual int MoveAbsolute(WORD wAxis, long lPos);
	virtual int MoveAbsolute(STAGE_COORD* pPos);
	virtual int MoveAbsoluteEx(WORD wAxis, long lPos, BOOL bWait = TRUE);
	virtual int MoveAbsoluteEx(STAGE_COORD* pPos, BOOL bWait = TRUE);
	virtual int MoveRelative(WORD wAxis, long lPos, BOOL bWait = TRUE);
	virtual int MoveRelative(STAGE_COORD* pPos, BOOL bWait = TRUE);
	virtual int StageStop(void);
#ifdef _DISABLETHREADSPOS
	virtual int GetPos(STAGE_COORD* pPos);
	virtual int GetPos(STAGE_COORD_XYZ* pPos);
	virtual int GetPos(WORD wAxis, long* pPos);
#endif
	virtual int MoveAtSpeed(WORD wAxis, double dSpeed);
	virtual int GetStageSize(long* pStageSizeX, long* pStageSizeY);
	virtual int GetOriginPos(STAGE_COORD* pPos);
	virtual int GetMaxStageSpeed(double* x, double* y);
	virtual void SetLocalSpeed(double x, double y);
	virtual void EnableSpeedLimit(void);
	virtual void DisableSpeedLimit(void);
	// 以下は SPT 用に新規追加です．
	virtual void ShowMotSys(BOOL bShow);
	virtual int EnableJoystickSPT(BOOL bEnable = TRUE, int iSpeedSel = 1);
	virtual int StageStopAxis(WORD wAxis);
	virtual BOOL IsAxisIdle(WORD wAxis);
	virtual int MoveAbsoluteAtSpeedEx(WORD wAxis, long lPos, double, BOOL bWait = TRUE);
	virtual int MoveAbsoluteAtSpeedEx(STAGE_COORD* pPos, double, BOOL bWait = TRUE);
	virtual int MoveRelativeAtSpeed(WORD wAxis, long lPos, double, BOOL bWait = TRUE);
	virtual int MoveRelativeAtSpeed(STAGE_COORD* pPos, double, BOOL bWait = TRUE);
	virtual BOOL GetJoyStickSPT(void);
	virtual int MoveAtSpeedEx(WORD, double, BOOL bWait = TRUE);
	virtual int GetElectronicGear(WORD wAxis, long* plEleGearAB);
	virtual int SetFactor(WORD wAxis, long lFactor);
	virtual int GetOrgLoc(long* plStageOrgLocX, long* plStageOrgLocY);
	virtual int SetJoyStickSpeedSPT(WORD wAxis, int iSpeedSel, double dSpeed);
};

#endif // _STAGESPT_H_
