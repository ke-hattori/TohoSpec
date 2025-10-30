#pragma once

#include "../../INC/Globals.hxx"
#include "CurveFitting.hpp"
#include "RadiusOfCurvature.h"

class CStressAnalysis
{
public:
	enum SUBSTRATES {
		FIRST = 0,
		SINGLE,
		SUBSTRATE_MAX,
	};

	CStressAnalysis();
	~CStressAnalysis();

	BOOL InitInstance();
	void ExitInstance();

	void SetMainRcpInfoPtr(const MAIN_RCP_INFO* pMainPcpInfo);
	void SetMeasProgInfoPtr(const MEAS_PROG_INFO* pMeasProgInfo);
	void SetStageProgInfoHdrPtr(const STAGE_PROG_INFO_HDR* pStageProgInfoHdr);
	void SetStageProgStressPtr(const STAGE_PROG_STRESS* pStageProgStress);

	void SetDeflection(int iSubstrate, int iLine, const double* pDeflection, int nOccurence);
	void SetDeflectionAtZero(int iSubstrate, int iLine, double** pDeflection, int nOccurence);
	void SetFilmThickness(int iLine, CURVEFITTINGST* pCf);
	void ReferenceData(int iLine);
	BOOL CalcStress(int iLine);
	double GetStress(int iLine, double dPosmm);

	void MakeScanDataLog();
	void GetScanDataFilename(LPTSTR pszFilename);

private:
	void CalcDeflectionStartAndEndAtZero(int iSubstrate, int iLine);
	double StoneyFormula(int iLine, double dPosmm);

private:
	const MAIN_RCP_INFO* m_pMainPcpInfo;
	const MEAS_PROG_INFO* m_pMeasProgInfo;
	const STAGE_PROG_INFO_HDR* m_pStageProgInfoHdr;
	const STAGE_PROG_STRESS* m_pStageProgStress;
	CCurveFitting* m_pFilmThicknessFitting[SUBSTRATE_MAX][STRESS_LINES_MAX];
	CRadiusOfCurvature m_substrateR[SUBSTRATE_MAX][STRESS_LINES_MAX];
	CMap<double, const double&, double, const double&> m_stressMap[STRESS_LINES_MAX];

	STRESS_CONFIG m_stressConfig;
	DWORD m_dwLineNum;
	DWORD m_dwSectionNum[STRESS_LINES_MAX];
};


typedef struct _THICKNESSFORSTRESS {
	BOOL bValidLine;
	long lScanPosY;
	CURVEFITTINGST cf;
} THICKNESSFORSTRESS;

