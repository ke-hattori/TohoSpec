#pragma once

#define NMMAX					   (1000)

#include "DdeClient.hxx"

#if 0
// CIE定義データ
typedef struct _CIE_xyzdata{
	//'cieLambda  As Double    '380nm to 780 nm
	double xbar;
	double ybar;
	double zbar;
	_CIE_xyzdata::_CIE_xyzdata(){ZeroMemory(this, sizeof(_CIE_xyzdata));}
} CIE_xyzdata, *PCIE_xyzdata;

typedef struct _CIE_RESULT_ITEMS{
	double cieLargeX;
	double cieLargeY;
	double cieLargeZ;
	double cieSmallx;
	double cieSmally;
	double cieSmallz;
	double cieNormalL;
	double cieNormalA;
	double cieNormalB;
	double cieStarL;
	double cieStarA;
	double cieStarB;
	double cieDashU;
	double cieDashV;
	double cieStarU;
	double cieStarV;

	double cieTempXn;	 //'計算temp（リキャリブレーションのためtempもグローバル化が必要）
	double cieTempYn;	 //'計算temp（リキャリブレーションのためtempもグローバル化が必要）
	double cieTempZn;	 //'計算temp（リキャリブレーションのためtempもグローバル化が必要）

	_CIE_RESULT_ITEMS::_CIE_RESULT_ITEMS(){ZeroMemory(this, sizeof(_CIE_RESULT_ITEMS));}
} CIE_RESULT_ITEMS, *PCIE_RESULT_ITEMS;
#endif // 0

class CSrAnalysis
{
public:
	CSrAnalysis();
	~CSrAnalysis();
	BOOL InitInstance();
	void ExitInstance();
	BOOL RunXmp();
	void ShutDownXmp();
	BOOL DoXmp(LPTSTR pszXmpDdeText, ANALYSISDATA* pAnalysisData, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, CDdeClient* pXmpDde);
	BOOL WriteAft2AdapFile(const ANALYSISDATA* pAnalysisData, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, LPCTSTR pszFullPath = _T("C:\\WVASE32\\Adap\\aft2adap.dat"));
	BOOL ComputeRate(ANALYSISDATA* pAnalysisData, const SCANDATA scanData[], const MEAS_PROG_INFO* pMeasProgInfo);
	void ComputeCIEpart1(const MEAS_PROG_INFO* pMeasProgInfo, const SCANDATA scanData[]);
	void ComputeCIEpart2();
/* added 2016.02.24 hmenjo バッチ処理 ---------- { ---------- */
	BOOL CopyAft2AdapFileSpecified(double dWavelengthStep, double dXmpAngle, LPCTSTR ptszFullPath = _T("C:\\WVASE32\\Adap\\aft2adap.dat"));
/* added 2016.02.24 hmenjo バッチ処理 ---------- } ---------- */

protected:
	BOOL Readsio2File(const MEAS_PROG_INFO* pMeasProgInfo);
	BOOL ReadCIECMFFile();
	BOOL ReadGenDataFile(ANALYSISDATA* pAnalysisData, LPCTSTR pszFullPath = _T("C:\\WVASE32\\Adap\\gendata.dat"));
	BOOL ReadIllumiFile(WORD wIlluminant);

	double m_RelRefl[NMMAX];

#if 0
//	double m_Illumi[NMMAX];
//	CIE_xyzdata m_CMFdefine[NMMAX];
//	CIE_RESULT_ITEMS CIE_Result;
#endif // 0
};
