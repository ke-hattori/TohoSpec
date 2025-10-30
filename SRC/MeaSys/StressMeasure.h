#pragma once

#include "../../INC/Globals.hxx"
#include "../../INC/ConfigFile.hxx"
#include "StressHead.h"
#include "StressAnalysis.h"

class CStressMeasure
{
public:
	CStressMeasure();
	~CStressMeasure();

	BOOL InitInstance();
	void ExitInstance();

	void SetStressAnalysisPtr(CStressAnalysis* pStressAnalysis);
	void SetMeasProgInfoPtr(const MEAS_PROG_INFO* pMeasProgInfo);
	void SetStageProgStressPtr(const STAGE_PROG_STRESS* pStageProgStress);

	static UINT WINAPI ThreadProc(LPVOID lpData);
	UINT ThreadProc();
	void Measure(int iStressMode, int iLineIndex);
	void Cancel();

	BOOL InitDataStrage();
	BOOL StartDataStrage();
	BOOL StopDataStrage();
	BOOL OutputDataStrage();

	BOOL ChangeCommMode();
	BOOL ChangeNormalMode();
	BOOL StatusInfoDataStrage(int& iState, int& iData);
	BOOL OutputDataStrage(int iChannel, double dDataList[], int nOccurence);
	BOOL OutputDataStrage(int iChannel, CStringArray& rStringList);
	BOOL AutoZeroOn(int iChannel);
	BOOL AutoZeroOff(int iChannel);
	BOOL PanelLockOn();
	BOOL PanelLockOff();
	BOOL Send(LPCTSTR pszCmd);			// ※汎用コマンド

private:
	CStressHead* m_pStressHead;
	CStressAnalysis* m_pStressAnalysis;
	const MEAS_PROG_INFO* m_pMeasProgInfo;
	const STAGE_PROG_STRESS* m_pStageProgStress;
	int m_iDataNum;
	double* m_pdDataList;
	BOOL m_bRun;
	int m_iStressMode;
	int m_iLineIndex;
	BOOL m_bInitData;
};
