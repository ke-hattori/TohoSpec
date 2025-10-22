#include "stdafx.h"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/SharedMemory.h"
#include "../../INC/MeaSys.hxx"
#include "LogFile.h"
#include "StressAnalysis.h"

#define UNITCONV_UM_TO_MM					(0.001)			// 10^(-3)
#define UNITCONV_MM_TO_M					(0.001)			// 10^(-3)
#define UNITCONV_MICRON_TO_M				(0.000001)		// 10^(-6)
#define UNITCONV_NM_TO_M					(0.000000001)	// 10^(-9)

extern CSharedMemory<STRESSRESULT> smStressResultDataBase;
extern CLogFile* pLogFile;

extern TCHAR g_tszProcDir[_MAX_PATH];		/* ďovZX̃fBNg('\'t)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* fBNg('\'t)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
extern void AddAbsPath(LPTSTR ptszPath);

extern BOOL MEAS_RecalibMinusEntry(LPCTSTR szRecalib, double& dData);

// --------------------------------------------------------------------
// CStressAnalysis
CStressAnalysis::CStressAnalysis()
{
	TRACE(_T("CStressAnalysis::CStressAnalysis()\n"));

	m_pMainPcpInfo = NULL;
	m_pMeasProgInfo = NULL;
	m_pStageProgInfoHdr = NULL;
	m_pStageProgStress = NULL;

	int i;


	for ( i = 0; i < STRESS_LINES_MAX; i++ ) {
		m_pFilmThicknessFitting[FIRST][i] = NULL;
		m_pFilmThicknessFitting[SINGLE][i] = new CCurveFitting(CCurveFitting::COEF7);	// 6
	}

	m_dwLineNum = 0;
	::ZeroMemory(m_dwSectionNum, sizeof(m_dwSectionNum));
}

// --------------------------------------------------------------------
// ~CStressAnalysis
CStressAnalysis::~CStressAnalysis()
{
	TRACE(_T("CStressAnalysis::~CStressAnalysis()\n"));

	int i;


	for ( i = 0; i < SUBSTRATE_MAX; i++ ) {
		int j;

		for ( j = 0; j < STRESS_LINES_MAX; j++ ) {
			if ( m_pFilmThicknessFitting[i][j] ) {
				delete m_pFilmThicknessFitting[i][j];
				m_pFilmThicknessFitting[i][j] = NULL;
			}
		}
	}
}

// --------------------------------------------------------------------
// InitInstance
BOOL CStressAnalysis::InitInstance()
{
	TRACE(_T("CStressAnalysis::InitInstance()\n"));

	ConfigFile_GetNanoSpecIni(&m_stressConfig, CONFIG_FILE_STRESS_CONFIG);

	m_dwLineNum = min(m_stressConfig.dwLiftPinNumberOfLine, STRESS_LINES_MAX);
	int i;

	for ( i = 0; i < (int)m_dwLineNum; i++)
		m_dwSectionNum[i] = min(m_stressConfig.Line[i].dwSectionNum, STRESS_SECTIONS_MAX);

	for ( i = 0; i < SUBSTRATE_MAX; i++ ) {
		int j;

		for ( j = 0; j < (int)m_dwLineNum; j++ ) {
			m_substrateR[i][j].SetStageSpeed((double)m_stressConfig.dwScanSpeed);
		}
	}

	return TRUE;
}

// --------------------------------------------------------------------
// ExitInstance
void CStressAnalysis::ExitInstance()
{
	TRACE(_T("CStressAnalysis::ExitInstance()\n"));

}

// --------------------------------------------------------------------------
// SetMainRcpInfoPtr
void CStressAnalysis::SetMainRcpInfoPtr(const MAIN_RCP_INFO* pMainPcpInfo)
{
	m_pMainPcpInfo = pMainPcpInfo;
}

// --------------------------------------------------------------------------
// SetMeasProgInfoPtr
void CStressAnalysis::SetMeasProgInfoPtr(const MEAS_PROG_INFO* pMeasProgInfo)
{

	m_pMeasProgInfo = pMeasProgInfo;
}

// --------------------------------------------------------------------------
// SetStageProgInfoHdrPtr
void CStressAnalysis::SetStageProgInfoHdrPtr(const STAGE_PROG_INFO_HDR* pStageProgInfoHdr)
{
	m_pStageProgInfoHdr = pStageProgInfoHdr;
}

// --------------------------------------------------------------------------
// SetStageProgStressPtr
void CStressAnalysis::SetStageProgStressPtr(const STAGE_PROG_STRESS* pStageProgStress)
{
	m_pStageProgStress = pStageProgStress;
}

// --------------------------------------------------------------------
// SetDeflection
void CStressAnalysis::SetDeflection(int iSubstrate, int iLine, const double* pDeflection, int nOccurence)
{
	TRACE(_T("CStressAnalysis::SetDeflection()\n"));

	ASSERT(iSubstrate < SUBSTRATE_MAX);
	ASSERT(iLine < STRESS_LINES_MAX);

	/* ψʗʂo^܂ */
	m_substrateR[iSubstrate][iLine].SetDeflection(pDeflection, nOccurence);
}

// --------------------------------------------------------------------
// SetDeflectionAtZero
void CStressAnalysis::SetDeflectionAtZero(int iSubstrate, int iLine, double** pDeflection, int nOccurence)
{
	TRACE(_T("CStressAnalysis::SetDeflectionAtZero()\n"));

	ASSERT(iSubstrate < SUBSTRATE_MAX);
	ASSERT(iLine < STRESS_LINES_MAX);

	/* ψʗʂo^܂ */
	m_substrateR[iSubstrate][iLine].SetDeflectionAtZero(pDeflection, nOccurence);
}

// --------------------------------------------------------------------
// SetFilmThickness
void CStressAnalysis::SetFilmThickness(int iLine, CURVEFITTINGST* pCf)
{
	TRACE(_T("CStressAnalysis::SetFilmThickness()\n"));

	const int DATANUM = 7;

//	ASSERT(pCf->datanum == DATANUM);
	if ( pCf->datanum != DATANUM )
		pLogFile->LoggingV("Stress SetFilmThickness Error, DataNum = %d", pCf->datanum);

	ASSERT(iLine < STRESS_LINES_MAX);

	/* ̃C}bsO */
	m_pFilmThicknessFitting[SINGLE][iLine]->SetParamData(*pCf);
}

// --------------------------------------------------------------------
// ReferenceData
void CStressAnalysis::ReferenceData(int iLine)
{
	const double dStartX = (double)m_stressConfig.lScanStartPosX * UNITCONV_UM_TO_MM;
	const double dEndX = (double)m_stressConfig.lScanEndPosX * UNITCONV_UM_TO_MM;
	const double dStepX = (double)m_stressConfig.dwSamplingInterval * UNITCONV_UM_TO_MM;

	CalcDeflectionStartAndEndAtZero(FIRST, iLine);

//	MakeScanDataLog();

	// ʃf[^̕ҏW
	double key;
	double value;

	::ZeroMemory(smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw, sizeof(smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw));

	// ʃf[^iψʗʁj
	smStressResultDataBase.GetSharedMemoryPtr()->nDeflectionOccurence[FIRST] = m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.GetCount();
	key = dStartX;
	int i;

	for ( i = 0; i < m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.GetCount(); i++ ) {
		if ( !m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.Lookup(key, value) )
			ASSERT(FALSE);
		smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw[FIRST][i].dPosum = key * 1000;
		smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw[FIRST][i].dDefl = value;
		key += dStepX;
	}
}

// --------------------------------------------------------------------
// CalcDeflectionStartAndEndAtZero
void CStressAnalysis::CalcDeflectionStartAndEndAtZero(int iSubstrate, int iLine)
{
	const double dStartX = (double)m_stressConfig.lScanStartPosX * UNITCONV_UM_TO_MM;
	const double dEndX = (double)m_stressConfig.lScanEndPosX * UNITCONV_UM_TO_MM;
	const double dStepX = (double)m_stressConfig.dwSamplingInterval * UNITCONV_UM_TO_MM;

	const int iFirstSectionIndex = 0;
	const int iLastSectionIndex = (int)m_dwSectionNum[iLine] - 1;

	BOOL bFirstSection;
	BOOL bLastSection;
	double dSectionStartX;
	double dSectionEndX;

	m_substrateR[iSubstrate][iLine].CalcDeflectionData(dStartX, dEndX, dStepX);

	int i;


	for ( i = iFirstSectionIndex; i <= iLastSectionIndex; i++ ) {
		bFirstSection = (i == iFirstSectionIndex);
		bLastSection = (i == iLastSectionIndex);
		dSectionStartX = m_stressConfig.Line[iLine].PinPos[i].lX * UNITCONV_UM_TO_MM;
		dSectionEndX = m_stressConfig.Line[iLine].PinPos[i + 1].lX * UNITCONV_UM_TO_MM;

		m_substrateR[iSubstrate][iLine].CalcDeflectionStartAndEndAtZero(dSectionStartX, dSectionEndX, dStepX, dStartX, dEndX, bFirstSection, bLastSection);
	}
}

// --------------------------------------------------------------------
// CalcStress
BOOL CStressAnalysis::CalcStress(int iLine)
{
	TRACE(_T("CStressAnalysis::CalcStress()\n"));

	const double dStartX = (double)m_stressConfig.lScanStartPosX * UNITCONV_UM_TO_MM;
	const double dEndX = (double)m_stressConfig.lScanEndPosX * UNITCONV_UM_TO_MM;
	const double dStepX = (double)m_stressConfig.dwSamplingInterval * UNITCONV_UM_TO_MM;

	m_stressMap[iLine].RemoveAll();

	// YC̃t@Xf[^Ȃ
	if ( m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.GetCount() == 0 ) {
		pLogFile->LoggingV("Stress No Reference Data, Line = %d", iLine);
		::ZeroMemory(smStressResultDataBase.GetSharedMemoryPtr(), sizeof(STRESSRESULT));
		// ʏBCp邽߂TRUE
		return TRUE;
	}

	/* FirstifKXjSingleitTvj̋ȗa߂܂ */
	double dSectionStartX;
	double dSectionEndX;

//	CalcDeflectionStartAndEndAtZero(FIRST, iLine);					// FIRST莞ReferenceData()Ŏ{ς
	CalcDeflectionStartAndEndAtZero(SINGLE, iLine);

	m_substrateR[FIRST][iLine].CalcDeflectionTheta(dStartX, dEndX, dStepX);
	m_substrateR[SINGLE][iLine].CalcDeflectionTheta(dStartX, dEndX, dStepX);

	int i;


	for ( i = 0; i < (int)m_dwSectionNum[iLine]; i++ ) {
		if ( m_pStageProgStress->Line[iLine].bScanValid[i] ) {
			dSectionStartX = (double)m_pStageProgStress->Line[iLine].SectPos[i].lScanStartPosX * UNITCONV_UM_TO_MM;
			dSectionEndX = (double)m_pStageProgStress->Line[iLine].SectPos[i].lScanEndPosX * UNITCONV_UM_TO_MM;

			m_substrateR[FIRST][iLine].CalcDeflectionThetaLinest(dSectionStartX, dSectionEndX, dStepX);
			m_substrateR[SINGLE][iLine].CalcDeflectionThetaLinest(dSectionStartX, dSectionEndX, dStepX);
		}
	}

//	MakeScanDataLog();

	/* Stoney̌ɂAXgX߂܂ */
	double dStressStartmm;
	double dStressEndmm;
//	double dStressStepmm;
	double dStress;
	double d;
	for ( i = 0; i < (int)m_dwSectionNum[iLine]; i++ ) {
		if ( m_pStageProgStress->Line[iLine].bScanValid[i] ) {
			dStressStartmm = (double)m_pStageProgStress->Line[iLine].SectPos[i].lScanStartPosX * UNITCONV_UM_TO_MM;
			dStressEndmm = (double)m_pStageProgStress->Line[iLine].SectPos[i].lScanEndPosX * UNITCONV_UM_TO_MM;
//			dStressStepmm = dStepX;
//			d = dStressStartmm;
//			while ( d <= dStressEndmm ) {
//				dStress = StoneyFormula(iLine, d);
//				m_stressMap[iLine].SetAt(d, dStress);
//				d += dStressStepmm;
//			}

// 2009.12.16 K.Matsuo 2Ŋ؂Ȃ̂ŁA_ȉ؎̂ -->
			d = (int)((dStressStartmm + dStressEndmm) / 2);			// PINԂ̒
// 2009.12.16 K.Matsuo 2Ŋ؂Ȃ̂ŁA_ȉ؎̂ <--
			dStress = StoneyFormula(iLine, d);
// 2009.11.16 K.Matsuo Lu[VΉ -->
			if ( !MEAS_RecalibMinusEntry(m_pMainPcpInfo->MainRcpParam._SR.szRecalib[0], dStress) ) {
				pLogFile->Logging("Stress MEAS_RecalibMinusEntry Error");
				::ZeroMemory(smStressResultDataBase.GetSharedMemoryPtr(), sizeof(STRESSRESULT));
				return FALSE;
			}
// 2009.11.16 K.Matsuo Lu[VΉ <--
			m_stressMap[iLine].SetAt(d, dStress);
		}
	}

// ʃf[^̕ҏW
	double key;
	double value;

	::ZeroMemory(smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw, sizeof(smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw));

	// ʃf[^iψʗʁj
	smStressResultDataBase.GetSharedMemoryPtr()->nDeflectionOccurence[FIRST] = m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.GetCount();
	key = dStartX;
	for ( i = 0; i < m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.GetCount(); i++ ) {
		if ( !m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.Lookup(key, value) )
			ASSERT(FALSE);
		smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw[FIRST][i].dPosum = key * 1000;
		smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw[FIRST][i].dDefl = value;
		key += dStepX;
	}

	// ʃf[^iψʗʁj
	smStressResultDataBase.GetSharedMemoryPtr()->nDeflectionOccurence[SINGLE] = m_substrateR[SINGLE][iLine].m_DeflectionAtZeroData.GetCount();
	ASSERT(smStressResultDataBase.GetSharedMemoryPtr()->nDeflectionOccurence[SINGLE] <= DEFL_CNT_MAX);

	key = dStartX;
	for ( i = 0; i < m_substrateR[SINGLE][iLine].m_DeflectionAtZeroData.GetCount(); i++ ) {
		if ( !m_substrateR[SINGLE][iLine].m_DeflectionAtZeroData.Lookup(key, value) )
			ASSERT(FALSE);
		smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw[SINGLE][i].dPosum = key * 1000;
		smStressResultDataBase.GetSharedMemoryPtr()->DeflectionRaw[SINGLE][i].dDefl = value;
		key += dStepX;
	}

	// ʃf[^iXj
	smStressResultDataBase.GetSharedMemoryPtr()->nDyDxOccurence = m_substrateR[SINGLE][iLine].m_DeflectionThetaMap.GetCount();
	ASSERT(smStressResultDataBase.GetSharedMemoryPtr()->nDyDxOccurence <= DYDX_CNT_MAX);

	key = dStartX + dStepX;
	for ( i = 0; i < m_substrateR[SINGLE][iLine].m_DeflectionThetaMap.GetCount(); i++ ) {
		if ( !m_substrateR[SINGLE][iLine].m_DeflectionThetaMap.Lookup(key, value) )
			ASSERT(FALSE);
		smStressResultDataBase.GetSharedMemoryPtr()->DyDxRaw[i].dPosum = key * 1000;
		smStressResultDataBase.GetSharedMemoryPtr()->DyDxRaw[i].dTheta = value;
		key += dStepX;
	}

	// ʃf[^íj
	smStressResultDataBase.GetSharedMemoryPtr()->nStressLineOccurrence = m_stressMap[iLine].GetCount();
	ASSERT(smStressResultDataBase.GetSharedMemoryPtr()->nStressLineOccurrence <= STRESS_CNT_MAX);

	int iCnt;
	iCnt = 0;
	key = dStartX + (dStepX * 5);
	while ( iCnt < m_stressMap[iLine].GetCount() ) {
		if ( !m_stressMap[iLine].Lookup(key, value) ) {
			//ASSERT(FALSE);
			key += dStepX;
			continue;
		}
		smStressResultDataBase.GetSharedMemoryPtr()->StressRaw[iCnt].dPosum = key * 1000;
		smStressResultDataBase.GetSharedMemoryPtr()->StressRaw[iCnt].dStress = value;
		iCnt++;
		key += dStepX;
	}

	// ʃf[^íiXgXjiZNVƁjj
	double dStressSection;
	for ( i = 0; i < (int)m_dwSectionNum[iLine]; i++ ) {
		if ( m_pStageProgStress->Line[iLine].bScanValid[i] ) {
			iCnt = 0;
			dStressSection = 0.0;
			dSectionStartX = (double)m_pStageProgStress->Line[iLine].SectPos[i].lScanStartPosX * UNITCONV_UM_TO_MM;
			dSectionEndX = (double)m_pStageProgStress->Line[iLine].SectPos[i].lScanEndPosX * UNITCONV_UM_TO_MM;
			int j;

			for ( j = 0; j < smStressResultDataBase.GetSharedMemoryPtr()->nStressLineOccurrence; j++ ) {
				if ( dSectionStartX <= smStressResultDataBase.GetSharedMemoryPtr()->StressRaw[j].dPosum * UNITCONV_UM_TO_MM &&
					smStressResultDataBase.GetSharedMemoryPtr()->StressRaw[j].dPosum * UNITCONV_UM_TO_MM <= dSectionEndX ) {
					dStressSection += GetStress(iLine, smStressResultDataBase.GetSharedMemoryPtr()->StressRaw[j].dPosum * UNITCONV_UM_TO_MM);
					iCnt++;
				}
			}

			if ( iCnt > 0 ) {
				smStressResultDataBase.GetSharedMemoryPtr()->nSectionOccurrence = i + 1;
				smStressResultDataBase.GetSharedMemoryPtr()->dStressSection[i] = dStressSection / iCnt;
			}

		}
	}

	return TRUE;
}

// --------------------------------------------------------------------
// GetStress
double CStressAnalysis::GetStress(int iLine, double dPosmm)
{
	TRACE(_T("CStressAnalysis::GetStress()\n"));

	/* Stress擾܂ */
	double dStress;

	if ( !m_stressMap[iLine].Lookup(dPosmm, dStress) )
		ASSERT(FALSE);

	return dStress;
}

// --------------------------------------------------------------------
// StoneyFormula
double CStressAnalysis::StoneyFormula(int iLine, double dPosmm)
{
//	TRACE(_T("CStressAnalysis::StoneyFormula()\n"));

	double dElasticModulus = m_pStageProgStress->dElasticModulusValue;				/* E11 Pa */
	double dSubstrateThickness;			/* MicroMeter */
	double dFilmThickness;				/* NanoMeter */
	double dStress;

	dSubstrateThickness = m_pStageProgInfoHdr->SampleInfo.dThickness;
	dFilmThickness = m_pFilmThicknessFitting[SINGLE][iLine]->GetFitValue(dPosmm);

	TRACE("Posmm : %lf\n", dPosmm);
	TRACE("dElasticModulus : %lf\n", dElasticModulus);
	TRACE("dSubstrateThickness : %lf\n", dSubstrateThickness);
	TRACE("dFilmThickness : %lf\n", dFilmThickness);
	TRACE("m_substrateR SINGLE : %lf\n", m_substrateR[SINGLE][iLine].GetDthetaDx(dPosmm));
	TRACE("m_substrateR FIRST : %lf\n", m_substrateR[FIRST][iLine].GetDthetaDx(dPosmm));

	if ( 0.0 ==	((m_substrateR[SINGLE][iLine].GetDthetaDx(dPosmm))	/* d/dx(=1/R) */ -
				 (m_substrateR[FIRST][iLine].GetDthetaDx(dPosmm)))) {
		// Zero Devide
		dStress = 0.0;
	}
	else {
		dStress = (
			   dElasticModulus *
			   (dSubstrateThickness * UNITCONV_MICRON_TO_M) * (dSubstrateThickness * UNITCONV_MICRON_TO_M)
			  )
			  /
			  (
			   6.0 * (dFilmThickness * UNITCONV_NM_TO_M) *
			   (1 / ((m_substrateR[SINGLE][iLine].GetDthetaDx(dPosmm))	/* d/dx(=1/R) */ -
					 (m_substrateR[FIRST][iLine].GetDthetaDx(dPosmm))))
			  );
	}

	return dStress / 1000000.0;	//MPa
}

//---------------------------------------------------------------------------
// MakeScanDataLog
void CStressAnalysis::MakeScanDataLog()
{
	TRACE(_T("CStressAnalysis::MakeScanDataLog()\n"));

	TCHAR szFilename[_MAX_PATH];
	FILE* fp;

	GetScanDataFilename(szFilename);
	if ( (fp = fopen(szFilename, _T("a"))) == NULL )
		return;

	POSITION pos;
	double dPosmm;
	double dValue;

	int iLine;


	for ( iLine = 0; iLine < 5; iLine++ ) {
		_ftprintf(fp, _T("%s LINE%02d\n"), "FIRST", iLine + 1);
		pos = m_substrateR[FIRST][iLine].m_DeflectionData.GetStartPosition();
		while ( pos ) {
			m_substrateR[FIRST][iLine].m_DeflectionData.GetNextAssoc(pos, dPosmm, dValue);
			_ftprintf(fp, _T("%lf\t%lf\n"), dPosmm, dValue);
		}
		_ftprintf(fp, _T("\n\n"));
	}

	_ftprintf(fp, _T("\n\n"));

	for ( iLine = 0; iLine < 5; iLine++ ) {
		_ftprintf(fp, _T("%s LINE%02d\n"), "FIRST AT ZERO", iLine + 1);
		pos = m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.GetStartPosition();
		while ( pos ) {
			m_substrateR[FIRST][iLine].m_DeflectionAtZeroData.GetNextAssoc(pos, dPosmm, dValue);
			_ftprintf(fp, _T("%lf\t%lf\n"), dPosmm, dValue);
		}
		_ftprintf(fp, _T("\n\n"));
	}

	_ftprintf(fp, _T("\n\n"));

	for ( iLine = 0; iLine < 5; iLine++ ) {
		_ftprintf(fp, _T("%s LINE%02d\n"), "SINGLE", iLine + 1);
		pos = m_substrateR[SINGLE][iLine].m_DeflectionData.GetStartPosition();
		while ( pos ) {
			m_substrateR[SINGLE][iLine].m_DeflectionData.GetNextAssoc(pos, dPosmm, dValue);
			_ftprintf(fp, _T("%lf\t%lf\n"), dPosmm, dValue);
		}
		_ftprintf(fp, _T("\n\n"));
	}

	_ftprintf(fp, _T("\n\n"));

	for ( iLine = 0; iLine < 5; iLine++ ) {
		_ftprintf(fp, _T("%s LINE%02d\n"), "SINGLE AT ZERO", iLine + 1);
		pos = m_substrateR[SINGLE][iLine].m_DeflectionData.GetStartPosition();
		while ( pos ) {
			m_substrateR[SINGLE][iLine].m_DeflectionData.GetNextAssoc(pos, dPosmm, dValue);
			_ftprintf(fp, _T("%lf\t%lf\n"), dPosmm, dValue);
		}
		_ftprintf(fp, _T("\n\n"));
	}

	for ( iLine = 0; iLine < 5; iLine++ ) {
		_ftprintf(fp, _T("%s LINE%02d\n"), "FIRST THETA", iLine + 1);
		pos = m_substrateR[FIRST][iLine].m_DeflectionThetaMap.GetStartPosition();
		while ( pos ) {
			m_substrateR[FIRST][iLine].m_DeflectionThetaMap.GetNextAssoc(pos, dPosmm, dValue);
			_ftprintf(fp, _T("%lf\t%lf\n"), dPosmm, dValue);
		}
		_ftprintf(fp, _T("\n\n"));
	}

	_ftprintf(fp, _T("\n\n"));

	for ( iLine = 0; iLine < 5; iLine++ ) {
		_ftprintf(fp, _T("%s LINE%02d\n"), "FIRST THETA ST", iLine + 1);
		pos = m_substrateR[FIRST][iLine].m_DeflectionThetaLineStMap.GetStartPosition();
		while ( pos ) {
			m_substrateR[FIRST][iLine].m_DeflectionThetaLineStMap.GetNextAssoc(pos, dPosmm, dValue);
			_ftprintf(fp, _T("%lf\t%lf\n"), dPosmm, dValue);
		}
		_ftprintf(fp, _T("\n\n"));
	}

	_ftprintf(fp, _T("\n\n"));

	fclose(fp);
}

//---------------------------------------------------------------------------
// GetScanDataFilename
void CStressAnalysis::GetScanDataFilename(LPTSTR pszFilename)
{
	TRACE(_T("CStressAnalysis::GetScanDataFilename()\n"));

	TCHAR szDate[] = _T("yyyymmdd");
	TCHAR szTime[] = _T("hhmmss");
	TCHAR szDateDirPath[_MAX_PATH];

	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
	TCHAR l_tszMeasDataDir[_MAX_PATH];
	_stprintf(l_tszMeasDataDir, _T("%s") DATA_MEASUREMENTDATA_DIR, g_tszProcDir);
	SECURITY_ATTRIBUTES sa;
	sa.lpSecurityDescriptor = NULL;
	::CreateDirectory(l_tszMeasDataDir, &sa);

	SYSTEMTIME systime;
	::GetLocalTime(&systime);

	_stprintf(szDate, _T("%04d%02d%02d"), systime.wYear, systime.wMonth, systime.wDay);
	_stprintf(szTime, _T("%02d%02d%02d"), systime.wHour, systime.wMinute, systime.wSecond);

	_stprintf(szDateDirPath, _T("%s%s%s"), DATA_MEASUREMENTDATA_DIR, szDate, _T("\\"));
	AddAbsPath(szDateDirPath);
	::CreateDirectory(szDateDirPath, &sa);

	_stprintf(pszFilename, _T("%s%s%s"), szDateDirPath, szTime, TXT_EXT);
}
