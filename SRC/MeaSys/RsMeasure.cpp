#include "stdafx.h"
#include "../../INC/Common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/SharedMemory.h"
#include "../../INC/nexiobase.hxx"
#include "../../INC/MeaSys.hxx"
#include "LogFile.h"
#include "RsMeasure.h"

extern CLogFile* pLogFile;
extern BOOL bHwSimulation;
extern TCHAR g_tszProcDir[_MAX_PATH];		/* ďovZX̃fBNg('\'t)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* fBNg('\'t)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
extern void AddAbsPath(LPTSTR ptszPath);
extern CSharedMemory<RESISTRESULT> smResistResultDataBase;	/* RS 茋 DB LGA	*/


LPCTSTR CRsMeasure::m_pszRange[] = {"-3", "-2", "-1", "+0", "+1", "+2", "+3", "+4", "+5", "+6", "+7",}; // Vsʂ̃R{{bNXƓł邱
LPCTSTR CRsMeasure::m_pszThickUnit[] = {"MM", "UM", "NM", "AM",};	// Vsʂ̃R{{bNXƓł邱

// --------------------------------------------------------------------------
// CRsMeasure
CRsMeasure::CRsMeasure()
{
	m_pRsHead = NULL;
}

// --------------------------------------------------------------------------
// ~CRsMeasure
CRsMeasure::~CRsMeasure()
{
}

// --------------------------------------------------------------------------
// InitInstance
BOOL CRsMeasure::InitInstance()
{
	TRACE(_T("CRsMeasure::InitInstance()\n"));

	ConfigFile_GetNanoSpecIni(&m_RsConfig, CONFIG_FILE_RS_CONFIG);

	// RSwbh̃C^tF[XI
	if ( bHwSimulation ) {
		m_pRsHead = new CRsHeadDesktop();
		pLogFile->Logging("Rs Head Type : CRsHeadDesktop");
	}
	else {
		m_pRsHead = new CRsHeadMCPT610();
		pLogFile->Logging("Rs Head Type : CRsHeadMCPT610");
	}

	// C^[bNiv[uwbhޔʒuֈړj
	if ( !MoveProbeHeadLowerPos(FALSE) )
		return FALSE;

	// RSwbh̏
	if ( !m_pRsHead->InitInstance() ) {
		pLogFile->Logging("Rs Head Initialize Error");
		pLogFile->Logging(m_pRsHead->GetLastError());
		return FALSE;
	}

	// v[u^Cv̐ݒ
	if ( !m_pRsHead->SetProbe(m_RsConfig.nRsProbeType) ) {
		pLogFile->Logging("Rs Head Set Probe Error");
		return FALSE;
	}

	return TRUE;
}

// --------------------------------------------------------------------------
// ExitInstance
void CRsMeasure::ExitInstance()
{
	TRACE(_T("CRsMeasure::ExitInstance()\n"));

	if ( m_pRsHead ) {
		m_pRsHead->ExitInstance();
		delete m_pRsHead;
		m_pRsHead = NULL;
	}
}

// --------------------------------------------------------------------------
// Measure
BOOL CRsMeasure::Measure(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("CRsMeasure::Measure()\n"));

	int iRange;
	int iMeasTime;

	// p[^`FbN
	if ( !CheckParams(pMeasProgInfo) )
		return FALSE;

	// p[^ݒ
	if ( !SetParams(pMeasProgInfo) )
		return FALSE;

	// v[uwbhɉt邽߉~삳iʒuֈړj
	if ( !MoveProbeHeadLowerPos() )
		return FALSE;

	// Jn
	iRange = atoi(m_pszRange[pMeasProgInfo->ScanParams._RS.lStartIdx]);
	iMeasTime = pMeasProgInfo->ScanParams._RS.dScanTime * 1000;

	if ( !m_pRsHead->MeasStart() ) {
		MoveProbeHeadLowerPos(FALSE);
		return FALSE;
	}

	const int MAXTIMES = 20;		/* Wɏ~Ȃꍇi+0,+1,+0,+1...Ȃǁj̖[v */
	int iTry;

	for ( iTry = 0; iTry < MAXTIMES; iTry++ ) {

		Sleep(iMeasTime);

		RSMEASDATA rsMeasData;
		if ( !m_pRsHead->GetMeasData(&rsMeasData) ) {
			::ZeroMemory(smResistResultDataBase.GetSharedMemoryPtr(), sizeof(RESISTRESULT));
			m_pRsHead->MeasEnd();
			MoveProbeHeadLowerPos(FALSE);
			return FALSE;
		}

		// 茋ʃf[^
		// i㑱̃I[o[WEI[o[[hEA_[W`FbNŃgC㏑ꍇj
		smResistResultDataBase.GetSharedMemoryPtr()->chDataValid = rsMeasData.chDataValid;
		strcpy(smResistResultDataBase.GetSharedMemoryPtr()->szResistance,			rsMeasData.szResistance);
		strcpy(smResistResultDataBase.GetSharedMemoryPtr()->szSurfaceResistivity,	rsMeasData.szSurfaceResistivity);
		strcpy(smResistResultDataBase.GetSharedMemoryPtr()->szVolumeResistivity,	rsMeasData.szVolumeResistivity);
		strcpy(smResistResultDataBase.GetSharedMemoryPtr()->szConductivity, 		rsMeasData.szConductivity);
		smResistResultDataBase.GetSharedMemoryPtr()->dResistance			= rsMeasData.dResistance;
		smResistResultDataBase.GetSharedMemoryPtr()->dSurfaceResistivity	= rsMeasData.dSurfaceResistivity;
		smResistResultDataBase.GetSharedMemoryPtr()->dVolumeResistivity 	= rsMeasData.dVolumeResistivity;
		smResistResultDataBase.GetSharedMemoryPtr()->dConductivity			= rsMeasData.dConductivity;

		// Check OverRange/OverLoad
		if ( rsMeasData.chDataValid == 'O' || rsMeasData.chDataValid == 'L' ) {
			// LEVEL UP
			iRange++;
			if ( iRange > 7 ) {
				m_pRsHead->MeasEnd();
				MoveProbeHeadLowerPos(FALSE);
				return TRUE;
			}
			if ( !m_pRsHead->SetRange(iRange) ) {
				m_pRsHead->MeasEnd();
				MoveProbeHeadLowerPos(FALSE);
				return FALSE;
			}
			continue;	// OverRange/OverLoad -> Retry
		}

		// Check UnderRange
		CString strBuffer;
		strBuffer = rsMeasData.szResistance;
		if ( atof(strBuffer.Mid(0, 5)) <= 0.8 ) {
			// LEVEL DOWN
			iRange--;
			if ( iRange < -3 ) {
				m_pRsHead->MeasEnd();
				MoveProbeHeadLowerPos(FALSE);
				return TRUE;
			}
			if ( !m_pRsHead->SetRange(iRange) ) {
				m_pRsHead->MeasEnd();
				MoveProbeHeadLowerPos(FALSE);
				return FALSE;
			}
			continue;	// UnderRange -> Retry
		}

		m_pRsHead->MeasEnd();	// Normal End
		MoveProbeHeadLowerPos(FALSE);
		return TRUE;
	}

// 2009.12.26 K.Matsuo -->
// ::ZeroMemory(smResistResultDataBase.GetSharedMemoryPtr(), sizeof(RESISTRESULT));
// 2009.12.26 K.Matsuo <--

	pLogFile->Logging("Resist Measure Retry Max Count Over");

	m_pRsHead->MeasEnd();
	MoveProbeHeadLowerPos(FALSE);
// 2009.12.26 K.Matsuo -->
//	return FALSE;
	return TRUE;
// 2009.12.26 K.Matsuo <--
}

// --------------------------------------------------------------------------
// CheckParams
BOOL CRsMeasure::CheckParams(const MEAS_PROG_INFO* pMeasProgInfo)
{
	if ( pMeasProgInfo->ScanParams._RS.lStartIdx < 0 || (sizeof(m_pszRange) / sizeof(m_pszRange[0])) <= pMeasProgInfo->ScanParams._RS.lStartIdx ) {
		pLogFile->LoggingV("Resist Measure Illegal Range : %ld", pMeasProgInfo->ScanParams._RS.lStartIdx);
		return FALSE;
	}

	if ( pMeasProgInfo->ScanParams._RS.dThickness < 0.001 || 9999.0 < pMeasProgInfo->ScanParams._RS.dThickness ) {
		pLogFile->LoggingV("Resist Measure Illegal Thickness : %lf", pMeasProgInfo->ScanParams._RS.dThickness);
		return FALSE;
	}

	if ( pMeasProgInfo->ScanParams._RS.wUnits < 0 || (sizeof(m_pszThickUnit) / sizeof(m_pszThickUnit[0])) <= pMeasProgInfo->ScanParams._RS.wUnits ) {
		pLogFile->LoggingV("Resist Measure Illegal Thick Unit : %d", pMeasProgInfo->ScanParams._RS.wUnits);
		return FALSE;
	}

	if ( pMeasProgInfo->ScanParams._RS.dRsCorrectFactor < 0.001 || 9999.0 < pMeasProgInfo->ScanParams._RS.dRsCorrectFactor ) {
		pLogFile->LoggingV("Resist Measure Illegal RCF : %lf", pMeasProgInfo->ScanParams._RS.dRsCorrectFactor);
		return FALSE;
	}

	return TRUE;
}

// --------------------------------------------------------------------------
// SetParams
BOOL CRsMeasure::SetParams(const MEAS_PROG_INFO* pMeasProgInfo)
{
	if ( !m_pRsHead->SetLimitV(pMeasProgInfo->ScanParams._RS.wVoltageLimit) )
		return FALSE;

	if ( !m_pRsHead->SetRange(atoi(m_pszRange[pMeasProgInfo->ScanParams._RS.lStartIdx])) )
		return FALSE;

	if ( !m_pRsHead->SetThickData(pMeasProgInfo->ScanParams._RS.dThickness, m_pszThickUnit[pMeasProgInfo->ScanParams._RS.wUnits]) )
		return FALSE;

	if ( !m_pRsHead->SetRCF(pMeasProgInfo->ScanParams._RS.dRsCorrectFactor) )
		return FALSE;

	return TRUE;
}

// --------------------------------------------------------------------------
// MoveProbeHeadLowerPos
BOOL CRsMeasure::MoveProbeHeadLowerPos(BOOL bLowerPos/*=TRUE*/)
{
	// V~[V
	if ( bHwSimulation ) {
		return TRUE;
	}

	// v[uwbh
	if ( bLowerPos ) {
		// ~
		nexioRS_ProbeHeadUp(FALSE);
		nexioRS_ProbeHeadDown(TRUE);
	}
	else {
		// ㏸
		nexioRS_ProbeHeadDown(FALSE);
		nexioRS_ProbeHeadUp(TRUE);
	}

	// C^[bNĎiv[uwbhʒu~[ɂȂĂ邩j
	const int iTimeoutSec = 30;

	CTimer timer;
	timer.Restart(iTimeoutSec);

	while ( 1 ) {
		if ( bLowerPos ) {
			// ~[̃ZT[IɂȂ܂IH
			if ( !nexioIsRS_ProbeHeadUpperPos() && nexioIsRS_ProbeHeadLowerPos() )
				break;
		}
		else {
			// ㏸[̃ZT[IɂȂ܂IH
			if ( nexioIsRS_ProbeHeadUpperPos() && !nexioIsRS_ProbeHeadLowerPos() )
				break;
		}

		if ( timer.IsTimeout() ) {
			pLogFile->LoggingV("Resist Measure Probe Head Move To %s Position Timeout Error.", (bLowerPos ? "Lower" : "Upper"));
			return FALSE;
		}

		::Sleep(10);
	}

	return TRUE;
}
