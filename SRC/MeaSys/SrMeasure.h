#pragma once

#include "../../INC/Globals.hxx"
#include "../../INC/ConfigFile.hxx"
#include "SrHead.h"
#include "CurveFitting.hpp"

#define CALIBFILEMAXLINE				  MAXDATANUM
#define CCD_WCAL_MAX					  (6)

class CSrMeasure
{
public:
	CSrMeasure();
	virtual ~CSrMeasure();
	virtual BOOL InitInstance();
	virtual void ExitInstance();
	virtual BOOL Measure(SCANDATA scanData[], int ccdData[], int iMeaSysStatus, const MEAS_PROG_INFO* pMeasProgInfo);
	virtual BOOL GTrReferecneMeasure(SCANDATA scanData[], int ccdData[], int iMeaSysStatus, const MEAS_PROG_INFO* pMeasProgInfo);

	BOOL CcdScan(int iCcdDataArray[], double dIntegTime, BOOL bProhibitNotify = FALSE);
	BOOL ChangeCcdShutter(WORD wFilter);
	BOOL OpenUvShutter();
	BOOL CloseUvShutter();
	BOOL OpenTransShutter();
	BOOL CloseTransShutter(BOOL bWait = TRUE);
	BOOL IsIdle() {return m_bRun == FALSE;}
	void GetScanData(double* DstArray, int MinTRWave, int MaxTRWave, const int iCcdDataArray[]);
	
	int GetWaveLengthStepFromConfig();

private:
// 2014.04.04 bagus wavelength step added -->
	void GetScanData_Step_1nm(double* DstArray, int MinTRWave, int MaxTRWave, const int iCcdDataArray[]);
	void GetScanData_Step_05nm(double* DstArray, int MinTRWave, int MaxTRWave, const int iCcdDataArray[]);
// 2014.04.04 bagus wavelength step added <--

	BOOL ReadCcdCalibDataFile(CCDCALIB*);

	void MakeScanDataLog(const int iCcdDataArray[], const double* DstArray, WORD wScanType, int iMeaSysStatus);
	void GetScanDataFilename(LPTSTR lpszFilename);

	BOOL IsDiscrete(WORD wScanType);

	double GetIntegTime(int iMeaSysStatus, const MEAS_PROG_INFO* pMeasProgInfo);
	void UvFinally();
	void VisFinally();

private:
	HANDLE m_hThread;
	UINT m_nThreadId;
	BOOL m_bRun;
	BOOL m_bSaveScanDataLog;

	CSrHead* m_pSrHead;
	CCurveFitting* m_pCcdPosFitting;
	const MEAS_PROG_INFO* m_pMeasProgInfo;
	int m_iCcdPixels;
	SR_CONFIG m_SrConfig;
};
