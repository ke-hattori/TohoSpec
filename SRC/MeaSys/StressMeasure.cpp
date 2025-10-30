#include "stdafx.h"
#include <math.h>
#include <process.h>	/* _beginthread, _endthread */
#include "../../INC/common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/nexiobase.hxx"
#include "../../INC/MeaSys.hxx"
#include "LogFile.h"
#include "StressMeasure.h"

#define STRESS_MEAS_TIMEOUT			(120)

extern CLogFile* pLogFile;
extern BOOL bHwSimulation;
extern HWND g_hNotifyWnd;
extern THICKNESSFORSTRESS thicknessForStress[STRESS_LINES_MAX];

// --------------------------------------------------------------------------
// CStressMeasure
CStressMeasure::CStressMeasure()
{
	TRACE(_T("CStressMeasure::CStressMeasure()\n"));

	m_pStressHead = NULL;
	m_pStressAnalysis = NULL;
	m_pMeasProgInfo = NULL;
	m_pStageProgStress = NULL;
	m_iDataNum = 0;
	m_pdDataList = NULL;
	m_bRun = FALSE;
	m_iStressMode = 0;
	m_iLineIndex = 0;
	m_bInitData = FALSE;
}

// --------------------------------------------------------------------------
// ~CStressMeasure
CStressMeasure::~CStressMeasure()
{
	TRACE(_T("CStressMeasure::~CStressMeasure()\n"));
}

// --------------------------------------------------------------------------
// InitInstance
BOOL CStressMeasure::InitInstance()
{
	TRACE(_T("CStressMeasure::InitInstance()\n"));

	if ( bHwSimulation ) {
		m_pStressHead = new CStressHeadDesktop();
	}
	else {
		m_pStressHead = new CStressHeadLKG5000();
	}

	if ( !m_pStressHead->InitInstance() ) {
		pLogFile->Logging("Stress Head Initialize Error");
		pLogFile->Logging(m_pStressHead->GetLastError());
		return FALSE;
	}

	return TRUE;
}

// --------------------------------------------------------------------------
// ExitInstance
void CStressMeasure::ExitInstance()
{
	TRACE(_T("CStressMeasure::ExitInstance()\n"));

	if ( m_pStressHead ) {
		m_pStressHead->ExitInstance();
		delete m_pStressHead;
		m_pStressHead = NULL;
	}

	if ( m_pdDataList ) {
		delete [] m_pdDataList;
		m_pdDataList = NULL;
	}
}

// --------------------------------------------------------------------------
// SetStressAnalysis
void CStressMeasure::SetStressAnalysisPtr(CStressAnalysis* pStressAnalysis)
{
	TRACE(_T("CStressMeasure::SetStressAnalysisPtr()\n"));

	m_pStressAnalysis = pStressAnalysis;
}

// --------------------------------------------------------------------------
// SetMeasProgInfoPtr
void CStressMeasure::SetMeasProgInfoPtr(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("CStressMeasure::SetMeasProgInfoPtr()\n"));

	m_pMeasProgInfo = pMeasProgInfo;
}

// --------------------------------------------------------------------------
// SetStageProgStressPtr
void CStressMeasure::SetStageProgStressPtr(const STAGE_PROG_STRESS* pStageProgStress)
{
	TRACE(_T("CStressMeasure::SetStageProgStressPtr()\n"));

	m_pStageProgStress = pStageProgStress;
}

// --------------------------------------------------------------------------
// Measure
void CStressMeasure::Measure(int iStressMode, int iLineIndex)
{
	HANDLE hThread;
	UINT nThreadId;

	m_iStressMode = iStressMode;
	m_iLineIndex = iLineIndex;
	m_bInitData = InitDataStrage();
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, LPVOID(this), 0, &nThreadId);
//	::WaitForSingleObject(hThread, INFINITE);
	::CloseHandle(hThread);
	hThread = NULL;
}

//---------------------------------------------------------------------------
// ThreadProc
UINT WINAPI CStressMeasure::ThreadProc(LPVOID lpData)
{
	TRACE(_T("CStressMeasure::ThreadProc(LPVOID lpData)\n"));

	return (reinterpret_cast<CStressMeasure*>(lpData))->ThreadProc();
}

//---------------------------------------------------------------------------
// ThreadProc
UINT CStressMeasure::ThreadProc()
{
	TRACE(_T("CStressMeasure::ThreadProc()\n"));

	enum STATE {
		STATE_INIT = 0,
		STATE_START,
		STATE_STOP,
		STATE_MEAS,
		STATE_END,
		STATE_ABEND,
		STATE_MAX,
	};

	// ステージ移動や信号検出がが正常でない場合、タイムアウトエラー
	CTimer timer;
	timer.Restart(STRESS_MEAS_TIMEOUT);

	m_bRun = TRUE;
	int iStep = STATE_INIT;

	// シミュレーション
	if ( bHwSimulation ) {
		iStep = STATE_MEAS;
	}

	while ( m_bRun ) {
		if ( timer.IsTimeout() )
			break;

		switch ( iStep ) {
		case STATE_INIT:
//			InitDataStrage();
			if ( !m_bInitData ) {
				pLogFile->Logging("Detect Stress Unit Initialize Error.");
				iStep = STATE_ABEND;
				break;
			}
			if ( !nexioIsStressUnitAlarm() ) {	// B接点 ref.3-68 出力はN.C.接点です。OFF時に短絡、ON時に開放となります。
				pLogFile->Logging("Detect Stress Unit Alarm.");
				iStep = STATE_ABEND;
				break;
			}
			iStep++;
			break;
		case STATE_START:
			if ( nexioIsStressScanStartPosX() ) {
				StartDataStrage();
				iStep++;
			}
			break;
		case STATE_STOP:
			if ( nexioIsStressScanEndPosX() ) {
				StopDataStrage();
				iStep++;
			}
			break;
		case STATE_MEAS:
			if ( !OutputDataStrage() ) {
				pLogFile->Logging("Stress Unit OutputDataStrage Error.");
				iStep = STATE_ABEND;
				break;
			}
			m_pStressAnalysis->SetDeflection(m_iStressMode, m_iLineIndex, m_pdDataList, m_iDataNum);
			if ( m_iStressMode == 0 ) {
				m_pStressAnalysis->ReferenceData(m_iLineIndex);
			}
			else {
				m_pStressAnalysis->SetFilmThickness(m_iLineIndex, &thicknessForStress[m_iLineIndex].cf);
				if ( !m_pStressAnalysis->CalcStress(m_iLineIndex) ) {
					pLogFile->Logging("Stress CalcStress Error.");
					iStep = STATE_ABEND;
					break;
				}
			}
			iStep++;
			break;
		case STATE_END:
			m_bRun = FALSE;
			break;
		case STATE_ABEND:
			m_bRun = FALSE;
			break;
		default:
			ASSERT(FALSE);
			break;
		}

//		::Sleep(10);
		::Sleep(1);
	}

// 2009.10.07 K.Matsuo Bug Fix -->
// InitDataStrage()でリターンFALSEされてしまう対応。
// StartDataStrage()をCallしたら、必ずStopDataStrage()をCallすること！
	if ( iStep == STATE_STOP )
		StopDataStrage();
// 2009.10.07 K.Matsuo Bug Fix -->

	long lRet;
	lRet = iStep == STATE_END ? 0L : 1L;
	if ( g_hNotifyWnd )
		::PostMessage(g_hNotifyWnd, WM_MEAS_LINE_END, lRet, 0L);

	return 0;
}

// --------------------------------------------------------------------------
// Cancel
void CStressMeasure::Cancel()
{
	TRACE(_T("CStressMeasure::Cancel()\n"));

	m_bRun = FALSE;
}

// --------------------------------------------------------------------------
// InitDataStrage
BOOL CStressMeasure::InitDataStrage()
{
	TRACE(_T("CStressMeasure::InitDataStrage()\n"));

	return m_pStressHead->InitDataStrage();
}

// --------------------------------------------------------------------------
// StartDataStrage
BOOL CStressMeasure::StartDataStrage()
{
	TRACE(_T("CStressMeasure::StartDataStrage()\n"));

	return m_pStressHead->StartDataStrage();
}

// --------------------------------------------------------------------------
// StopDataStrage
BOOL CStressMeasure::StopDataStrage()
{
	TRACE(_T("CStressMeasure::StopDataStrage()\n"));

	return m_pStressHead->StopDataStrage();
}

// --------------------------------------------------------------------------
// OutputDataStrage
BOOL CStressMeasure::OutputDataStrage()
{
	TRACE(_T("CStressMeasure::OutputDataStrage()\n"));

	int iState = 0;
	int iData = 0;

	// 少し待たないと、ストレスユニットのデータ取得数が少ない
	::Sleep(1000);

	if ( !StatusInfoDataStrage(iState, iData) ) {
		return FALSE;
	}

	m_iDataNum = 0;
	if ( m_pdDataList ) {
		delete [] m_pdDataList;
		m_pdDataList = NULL;
	}

	if ( iData == 0 )
		return TRUE;

	m_pdDataList = new double[iData];
	if ( !OutputDataStrage(1, m_pdDataList, iData) )
		return FALSE;

	m_iDataNum = iData;

	pLogFile->LoggingV("Stress Data : %d", m_iDataNum);

	return TRUE;
}





// --------------------------------------------------------------------
// ChangeCommMode
BOOL CStressMeasure::ChangeCommMode()
{
	return m_pStressHead->ChangeCommMode();
}

// --------------------------------------------------------------------
// ChangeNormalMode
BOOL CStressMeasure::ChangeNormalMode()
{
	return m_pStressHead->ChangeNormalMode();
}

// --------------------------------------------------------------------
// StatusInfoDataStrage
BOOL CStressMeasure::StatusInfoDataStrage(int& iState, int& iData)
{
	return m_pStressHead->StatusInfoDataStrage(iState, iData);
}

// --------------------------------------------------------------------
// OutputDataStrage
BOOL CStressMeasure::OutputDataStrage(int iChannel, double dDataList[], int nOccurence)
{
	return m_pStressHead->OutputDataStrage(iChannel, dDataList, nOccurence);
}

// --------------------------------------------------------------------
// OutputDataStrage
BOOL CStressMeasure::OutputDataStrage(int iChannel, CStringArray& rStringList)
{
	return m_pStressHead->OutputDataStrage(iChannel, rStringList);
}

// --------------------------------------------------------------------
// AutoZeroOn
BOOL CStressMeasure::AutoZeroOn(int iChannel)
{
	return m_pStressHead->AutoZeroOn(iChannel);
}

// --------------------------------------------------------------------
// AutoZeroOff
BOOL CStressMeasure::AutoZeroOff(int iChannel)
{
	return m_pStressHead->AutoZeroOff(iChannel);
}

// --------------------------------------------------------------------
// PanelLockOn
BOOL CStressMeasure::PanelLockOn()
{
	return m_pStressHead->PanelLockOn();
}

// --------------------------------------------------------------------
// PanelLockOff
BOOL CStressMeasure::PanelLockOff()
{
	return m_pStressHead->PanelLockOff();
}

// --------------------------------------------------------------------
// Send
BOOL CStressMeasure::Send(LPCTSTR pszCmd)			// ※汎用コマンド
{
	return m_pStressHead->Send(pszCmd);
}

