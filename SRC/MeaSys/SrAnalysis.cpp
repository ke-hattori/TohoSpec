#include "stdafx.h"
#include <math.h>
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/DdeClient.hxx"
#include "SrAnalysis.h"
#include "ScanRange.h"
#include "resource.h"
#include "Mojiretsu.h"
#include "Xmp.h"
/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
#include <direct.h>
#include "System.h"
/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */

#if 0
#define szPathIll			 _T("..\\DATA\\IlluminantTable\\")
#define szCIECMFFilename	 _T("CIE1931_2degXYZ_CMF.dat")
#endif // 0

#define REFERENCEMATERIALPATH				_T("..\\CFG\\SYSTEM\\SR_ReferenceMaterial\\")

/* added 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ MeaSys.dll ---------- { ---------- */
extern TCHAR g_tszProcDir[_MAX_PATH];		/* ï¿½Ä�oï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½('\'ï¿½tï¿½ï¿½)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* ï¿½î�€ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½('\'ï¿½tï¿½ï¿½)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
extern void AddAbsPath(LPTSTR ptszPath);
/* added 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ MeaSys.dll ---------- } ---------- */

extern CXmp* pXmp;
extern CMojiretsu* pMojiretsu;

// --------------------------------------------------------------------
// CSrAnalysis
CSrAnalysis::CSrAnalysis()
{
	TRACE(_T("CSrAnalysis::CSrAnalysis()\n"));

	::ZeroMemory(m_RelRefl, sizeof(m_RelRefl));
}

// --------------------------------------------------------------------
// ~CSrAnalysis
CSrAnalysis::~CSrAnalysis()
{
	TRACE(_T("CSrAnalysis::~CSrAnalysis()\n"));
}

// --------------------------------------------------------------------
// InitInstance
BOOL CSrAnalysis::InitInstance()
{
	TRACE(_T("CSrAnalysis::InitInstance()\n"));

	return TRUE;
}

// --------------------------------------------------------------------
// ExitInstance
void CSrAnalysis::ExitInstance()
{
	TRACE(_T("CSrAnalysis::ExitInstance()\n"));
}

// // --------------------------------------------------------------------
// // PrintDebug
// void PrintDebug(const SCANDATA scanData[])
// {
// 	CString strBuffer;
// 	CStdioFile stdioFile;
// 	stdioFile.Open("_Dump.txt", CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);
//
// 	strBuffer.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
// 					 "Dark_T1",
// 					 "StdR_T1",
// 					 "ExtR_T1",
// 					 "Dark_T2",
// 					 "ExtR_T2",
// 					 "Samp",
// 					 "Analysys");
// 	stdioFile.WriteString(strBuffer);
//
// 	for(int i = 0; i < SCANDATA_POINT_MAX; i++) {
// 		strBuffer.Format("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
// 						 scanData[0].dScanData[i],
// 						 scanData[1].dScanData[i],
// 						 scanData[2].dScanData[i],
// 						 scanData[3].dScanData[i],
// 						 scanData[4].dScanData[i],
// 						 scanData[5].dScanData[i],
// 						 scanData[6].dScanData[i]);
// 		stdioFile.WriteString(strBuffer);
// 	}
// 	stdioFile.Close();
// }

// --------------------------------------------------------------------
// ComputeRate
BOOL CSrAnalysis::ComputeRate(ANALYSISDATA* pAnalysisData, const SCANDATA scanData[], const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("CSrAnalysis::ComputeRate()\n"));

	double* ResultData			= pAnalysisData->dAnalysisReflectanceData;

	const double* SampleData	= scanData[MEASYS_SAMP].dScanData;
	const double* ReferenceData = scanData[MEASYS_STDR_T1].dScanData;
	const double* DarkData		= scanData[MEASYS_DARK_T1].dScanData;

	const double* Samp_T2_Data	= scanData[MEASYS_SAMP].dScanData;
	const double* ExtR_T2_Data	= scanData[MEASYS_EXTR_T2].dScanData;
	const double* Dark_T2_Data	= scanData[MEASYS_DARK_T2].dScanData;
	const double* ExtR_T1_Data	= scanData[MEASYS_EXTR_T1].dScanData;
	const double* StdR_T1_Data	= scanData[MEASYS_STDR_T1].dScanData;
	const double* Dark_T1_Data	= scanData[MEASYS_DARK_T1].dScanData;

	const WORD& wScanType = pMeasProgInfo->ScanParams.hdr.wScanType;

	double dWavelengthStep = pAnalysisData->dWavelengthStep;

//	PrintDebug(scanData);

// 2009.10.26 K.Matsuo TR -->
	/* ï¿½ï¿½ï¿½ï¿½ï¿½Eï¿½ï¿½ï¿½Ë—ï¿½ ï¿½ï¿½Î’lï¿½ÏŠï¿½ï¿½Wï¿½ï¿½ */
	if ( wScanType == MEAS_PROG_TYPE_SR_THICKNESS		||
		 wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE 	||
		 wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE_CIE )
	{
		::ZeroMemory(m_RelRefl, sizeof(m_RelRefl));

		if ( !Readsio2File(pMeasProgInfo) )
			return FALSE;
	}
// 2009.10.26 K.Matsuo TR <--

	double numer;
	double denom;
	CScanRange scanRange(pMeasProgInfo);

	int iIndex;
	double dScanStart = (double)scanRange.GetScanStartRange();
	double dScanEnd = (double)scanRange.GetScanEndRange();

	double p = dScanStart;
	while(p <= dScanEnd){
		if(dWavelengthStep == 0.5){
			iIndex = (int)((p - dScanStart) * 2);
		}
		else{
			iIndex = (int)(p - dScanStart);
		}

		if ( !pMeasProgInfo->Ref2nd.bMeasure ) {
			numer = SampleData[iIndex] - DarkData[iIndex];
			denom = ReferenceData[iIndex] - DarkData[iIndex];
		}
		else {
			numer = (Samp_T2_Data[iIndex] - Dark_T2_Data[iIndex]) * (ExtR_T1_Data[iIndex] - Dark_T1_Data[iIndex]);
			denom = (ExtR_T2_Data[iIndex] - Dark_T2_Data[iIndex]) * (StdR_T1_Data[iIndex] - Dark_T1_Data[iIndex]);
		}

		if ( numer <= 0.0 )
		{
			ResultData[iIndex] = 0.0;
		}
		else if ( denom <= 0.0 )
		{
			ResultData[iIndex] = 0.0;
		}
		else
		{
			// ï¿½ï¿½ï¿½Î”ï¿½ï¿½Ë—ï¿½ï¿½Ì�Zï¿½o
			ResultData[iIndex] = numer / denom;
// 2009.05.18 K.Matsuo ï¿½oï¿½Oï¿½Î‰ï¿½ ï¿½ï¿½Î”ï¿½ï¿½Ë—ï¿½ï¿½ï¿½100%ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½A100%ï¿½É‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Î‰ï¿½ -->
			// ï¿½ï¿½ï¿½Ì�ï¿½ï¿½_ï¿½Í�Aï¿½Ü‚ï¿½ï¿½ï¿½ï¿½Î”ï¿½ï¿½Ë—ï¿½ï¿½Å‚ï¿½ï¿½ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½Ì‚ï¿½ï¿½ß�í�œï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½Bï¿½Bï¿½B
//			if ( ResultData[i] > 1.0 )
//				ResultData[i] = 1.0;
// 2009.05.18 K.Matsuo ï¿½oï¿½Oï¿½Î‰ï¿½ ï¿½ï¿½Î”ï¿½ï¿½Ë—ï¿½ï¿½ï¿½100%ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½A100%ï¿½É‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Î‰ï¿½ <--
		}
		/* ï¿½ï¿½ï¿½ï¿½ï¿½Eï¿½ï¿½ï¿½Ë—ï¿½ ï¿½ï¿½Î’lï¿½ÏŠï¿½ï¿½Wï¿½ï¿½ */
		if ( wScanType == MEAS_PROG_TYPE_SR_THICKNESS		||
			 wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE 	||
			 wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE_CIE )
		{
			// ï¿½ï¿½Î”ï¿½ï¿½Ë—ï¿½ï¿½Ì�Zï¿½o
			ResultData[iIndex] = ResultData[iIndex] * m_RelRefl[(int)p - 1];
// 2009.05.18 K.Matsuo ï¿½oï¿½Oï¿½Î‰ï¿½ ï¿½ï¿½Î”ï¿½ï¿½Ë—ï¿½ï¿½ï¿½100%ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½A100%ï¿½É‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Î‰ï¿½ -->
			if ( ResultData[iIndex] > 1.0 )
				ResultData[iIndex] = 1.0;
// 2009.05.18 K.Matsuo ï¿½oï¿½Oï¿½Î‰ï¿½ ï¿½ï¿½Î”ï¿½ï¿½Ë—ï¿½ï¿½ï¿½100%ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½A100%ï¿½É‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Î‰ï¿½ <--
		}
#if 0
//		/* ODï¿½lï¿½vï¿½Z ï¿½ï¿½ï¿½ß—ï¿½ï¿½ï¿½ï¿½ï¿½ODï¿½lï¿½ï¿½ï¿½vï¿½Zï¿½ï¿½ï¿½ï¿½ */
//		if ( wScanType == MEAS_PROG_TYPE_SR_OPTICAL_DENSITY ) {
//			if ( ResultData[iIndex] == 0.0 )
//				ResultData[iIndex] = 6.0;
//			else
//				ResultData[iIndex] = log10(1.0 / ResultData[iIndex]);
//		}
//		*/
#endif // 0

		p += dWavelengthStep;
	}

	return TRUE;
}

// --------------------------------------------------------------------
// ComputeCIEpart1
void CSrAnalysis::ComputeCIEpart1(const MEAS_PROG_INFO* pMeasProgInfo, const SCANDATA scanData[])
{
#if 0
///*
//'====================================================
//'
//'   ï¿½Fï¿½xï¿½ï¿½ï¿½è�€ï¿½ÚŠeï¿½ï¿½vï¿½ZPart1ï¿½iXYZ,XnYnZnï¿½j
//'   COD_SdataModiï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½Fï¿½xï¿½ï¿½ï¿½vï¿½Z
//'   ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½@ï¿½\ï¿½Ì‚ï¿½ï¿½ß‚É‚Qï¿½Â‚É•ï¿½ï¿½ï¿½
//'   Part1ï¿½Å‚Í�uXYZï¿½vï¿½ï¿½ï¿½ß�Aï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½APart2ï¿½É‚Ä•â�³ï¿½ï¿½ï¿½XYZï¿½É‚ÄŒvï¿½Z
//'
//'====================================================
//*/
//
//	TRACE(_T("CSrAnalysis::ComputeCIEpart1()\n"));
//
//	if ( !ReadIllumiFile(pMeasProgInfo->ScanParams._SR.CIEOptDesc.wIlluminant) ) {
//		::MessageBox(NULL, _T("CIE ï¿½Cï¿½ï¿½ï¿½~ï¿½iï¿½ï¿½ï¿½gï¿½vï¿½Zï¿½ï¿½ï¿½s ReadIllumiFile"), _T("NanoSpec"), MB_OK | MB_ICONERROR);
//		return;
//	}
//	if ( !ReadCIECMFFile() ) {
//		::MessageBox(NULL, _T("CIE ï¿½Cï¿½ï¿½ï¿½~ï¿½iï¿½ï¿½ï¿½gï¿½vï¿½Zï¿½ï¿½ï¿½s ReadCIECMFFile"), _T("NanoSpec"), MB_OK | MB_ICONERROR);
//		return;
//	}
//
//	const double* ResultData = scanData[MEASYS_ANALYSIS].dScanData;
//
//	  double sumX, sumY, sumZ, SumSY;
//	  double sumXn, sumYn, sumZn;
//	  double LK;
//
//	  sumX = sumY = sumZ = SumSY = 0.0;
//	  sumXn = sumYn = sumZn = 0.0;
//
//	  for ( int i = m_scanStart-1; i < m_scanEnd; i++ )
//	  {
//		  SumSY += m_Illumi[i] * m_CMFdefine[i].ybar;
//
//		  sumX += m_Illumi[i] * m_CMFdefine[i].xbar * ResultData[i];
//		  sumY += m_Illumi[i] * m_CMFdefine[i].ybar * ResultData[i];
//		  sumZ += m_Illumi[i] * m_CMFdefine[i].zbar * ResultData[i];
//
//		  sumXn += m_Illumi[i] * m_CMFdefine[i].xbar;
//		  sumYn += m_Illumi[i] * m_CMFdefine[i].ybar;
//		  sumZn += m_Illumi[i] * m_CMFdefine[i].zbar;
//	  }
//
//	  LK = 100.0 / SumSY;
//
//	  //'X,Y,Z
//	  CIE_Result.cieLargeX = sumX * LK;
//	  CIE_Result.cieLargeY = sumY * LK;
//	  CIE_Result.cieLargeZ = sumZ * LK;
//
//	  //'Xn,Yn,Zn (Complete Reflection)
//	  CIE_Result.cieTempXn = sumXn * LK;
//	  CIE_Result.cieTempYn = sumYn * LK;
//	  CIE_Result.cieTempZn = sumZn * LK;
//
///*
//'ï¿½Fï¿½xï¿½ï¿½ï¿½Ú‘ï¿½ï¿½èŒ‹ï¿½ï¿½
//Type CIE_RESULT_ITEMS
//	  cieLargeX As Double
//	  cieLargeY As Double
//	  cieLargeZ As Double
//	  cieSmallx As Double
//	  cieSmally As Double
//	  cieSmallz As Double
//	  cieNormalL As Double
//	  cieNormalA As Double
//	  cieNormalB As Double
//	  cieStarL As Double
//	  cieStarA As Double
//	  cieStarB As Double
//	  cieDashU As Double
//	  cieDashV As Double
//	  cieStarU As Double
//	  cieStarV As Double
//
//	  cieTempXn As Double	 'ï¿½vï¿½Ztempï¿½iï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½ï¿½tempï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½[ï¿½oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½j
//	  cieTempYn As Double	 'ï¿½vï¿½Ztempï¿½iï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½ï¿½tempï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½[ï¿½oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½j
//	  cieTempZn As Double	 'ï¿½vï¿½Ztempï¿½iï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½ï¿½tempï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½[ï¿½oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½j
//End Type
//Public CIE_Result As CIE_RESULT_ITEMS
//
//
//'====================================================
//'
//'   ï¿½Fï¿½xï¿½ï¿½ï¿½è�€ï¿½ÚŠeï¿½ï¿½vï¿½ZPart1ï¿½iXYZ,XnYnZnï¿½j
//'   COD_SdataModiï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½Fï¿½xï¿½ï¿½ï¿½vï¿½Z
//'   ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½@ï¿½\ï¿½Ì‚ï¿½ï¿½ß‚É‚Qï¿½Â‚É•ï¿½ï¿½ï¿½
//'   Part1ï¿½Å‚Í�uXYZï¿½vï¿½ï¿½ï¿½ß�Aï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½APart2ï¿½É‚Ä•â�³ï¿½ï¿½ï¿½XYZï¿½É‚ÄŒvï¿½Z
//'
//'====================================================
//Public Sub ComputeCIEpart1()
//
//Dim sumX As Double, sumY As Double, sumZ As Double, SumSY As Double
//Dim sumXn As Double, sumYn As Double, sumZn As Double
//Dim LK As Double
//Dim uDn As Double, vDn As Double
//Dim i As Integer
//
//SumSY = 0
//
//sumX = 0
//sumY = 0
//sumZ = 0
//
//sumXn = 0
//sumYn = 0
//sumZn = 0
//
//For i = CIE_MINWAVE To CIE_MAXWAVE
//	  With CIE_CMFtable.CMFdefine(i%)
//		  SumSY = SumSY + CIE_ILLUMItable.ILLUMIdefine(i%) * .ybar
//
//		  sumX = sumX + CIE_ILLUMItable.ILLUMIdefine(i%) * .xbar * COD_SdataModi!(i%)
//		  sumY = sumY + CIE_ILLUMItable.ILLUMIdefine(i%) * .ybar * COD_SdataModi!(i%)
//		  sumZ = sumZ + CIE_ILLUMItable.ILLUMIdefine(i%) * .zbar * COD_SdataModi!(i%)
//
//		  sumXn = sumXn + CIE_ILLUMItable.ILLUMIdefine(i%) * .xbar
//		  sumYn = sumYn + CIE_ILLUMItable.ILLUMIdefine(i%) * .ybar
//		  sumZn = sumZn + CIE_ILLUMItable.ILLUMIdefine(i%) * .zbar
//	  End With
//Next i
//
//LK = 100# / SumSY
//
//On Error Resume Next
//
//With CIE_Result
//	  'X,Y,Z
//	  .cieLargeX = sumX * LK
//	  .cieLargeY = sumY * LK
//	  .cieLargeZ = sumZ * LK
//
//	  'Xn,Yn,Zn (Complete Reflection)
//	  .cieTempXn = sumXn * LK
//	  .cieTempYn = sumYn * LK
//	  .cieTempZn = sumZn * LK
//End With
//
//End Sub
//*/
#endif // 0
}

// --------------------------------------------------------------------
// ComputeCIEpart2
void CSrAnalysis::ComputeCIEpart2()
{
#if 0
///*
//'====================================================
//'
//'   ï¿½Fï¿½xï¿½ï¿½ï¿½è�€ï¿½ÚŠeï¿½ï¿½vï¿½ZPart2ï¿½ixyz,Lab,a*b*,u'v',u*v*ï¿½j
//'   COD_SdataModiï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½Fï¿½xï¿½ï¿½ï¿½vï¿½Z
//'   ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½@ï¿½\ï¿½Ì‚ï¿½ï¿½ß‚É‚Qï¿½Â‚É•ï¿½ï¿½ï¿½
//'  Part1ï¿½Å‚Í�uXYZï¿½vï¿½ï¿½ï¿½ß�Aï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½APart2ï¿½É‚Ä•â�³ï¿½ï¿½ï¿½XYZï¿½É‚ÄŒvï¿½Z
//'
//'====================================================
//*/
//
//	TRACE(_T("CSrAnalysis::ComputeCIEpart2()\n"));
//
//	  double uDn, vDn;
//
//	  //'[x,y,z]
//	  if ( CIE_Result.cieLargeX + CIE_Result.cieLargeY + CIE_Result.cieLargeZ != 0.0 )
//	  {
//		  CIE_Result.cieSmallx = CIE_Result.cieLargeX / (CIE_Result.cieLargeX + CIE_Result.cieLargeY + CIE_Result.cieLargeZ);
//		  CIE_Result.cieSmally = CIE_Result.cieLargeY / (CIE_Result.cieLargeX + CIE_Result.cieLargeY + CIE_Result.cieLargeZ);
//		  CIE_Result.cieSmallz = CIE_Result.cieLargeZ / (CIE_Result.cieLargeX + CIE_Result.cieLargeY + CIE_Result.cieLargeZ); // Smallz = 1 - Smallx - Smally ï¿½Æ“ï¿½ï¿½ï¿½
//	  }
//
//	  //'[L,a,b]
//	  CIE_Result.cieNormalL = 10.0 * sqrt(CIE_Result.cieLargeY);
//	  if ( sqrt(CIE_Result.cieLargeY) != 0.0 )
//	  {
//		  CIE_Result.cieNormalA = 17.5 * (1.02 * CIE_Result.cieLargeX - CIE_Result.cieLargeY) / sqrt(CIE_Result.cieLargeY);
//		  CIE_Result.cieNormalB = 7.0 * (CIE_Result.cieLargeY - 0.847 * CIE_Result.cieLargeZ) / sqrt(CIE_Result.cieLargeY);
//	  }
//
//	  //'[L*,a*,b*]
//	  if ( CIE_Result.cieTempYn != 0.0 )
//	  {
//		  CIE_Result.cieStarL = 116.0 * pow(CIE_Result.cieLargeY / CIE_Result.cieTempYn, 1.0 / 3.0) - 16.0;
//	  }
//	  if ( CIE_Result.cieTempXn != 0.0 && CIE_Result.cieTempYn != 0.0 )
//	  {
//		  CIE_Result.cieStarA = 500.0 * (pow(CIE_Result.cieLargeX / CIE_Result.cieTempXn, 1.0 / 3.0) - pow(CIE_Result.cieLargeY / CIE_Result.cieTempYn, 1.0 / 3.0));
//	  }
//	  if ( CIE_Result.cieTempYn != 0.0 && CIE_Result.cieTempZn != 0.0 )
//	  {
//		  CIE_Result.cieStarB = 200.0 * (pow(CIE_Result.cieLargeY / CIE_Result.cieTempYn, 1.0 / 3.0) - pow(CIE_Result.cieLargeZ / CIE_Result.cieTempZn, 1.0 / 3.0));
//	  }
//
//	  //'[u',v']
//	  if ( CIE_Result.cieLargeX + 15.0 * CIE_Result.cieLargeY + 3.0 * CIE_Result.cieLargeZ != 0 )
//	  {
//		  CIE_Result.cieDashU = 4.0 * CIE_Result.cieLargeX / (CIE_Result.cieLargeX + 15.0 * CIE_Result.cieLargeY + 3.0 * CIE_Result.cieLargeZ);
//		  CIE_Result.cieDashV = 9.0 * CIE_Result.cieLargeY / (CIE_Result.cieLargeX + 15.0 * CIE_Result.cieLargeY + 3.0 * CIE_Result.cieLargeZ);
//	  }
//
//	  //'[u'n,v'n (Complete refrection)]ï¿½\ï¿½ï¿½ï¿½È‚ï¿½    // ï¿½ï¿½ï¿½Sï¿½gï¿½Uï¿½Ê‚ÅŒvï¿½Zï¿½ï¿½ï¿½ï¿½u',v'ï¿½l
//	  if ( CIE_Result.cieTempXn + 15.0 * CIE_Result.cieTempYn + 3.0 * CIE_Result.cieTempZn != 0 )
//	  {
//		  uDn = 4.0 * CIE_Result.cieTempXn / (CIE_Result.cieTempXn + 15.0 * CIE_Result.cieTempYn + 3.0 * CIE_Result.cieTempZn);
//		  vDn = 9.0 * CIE_Result.cieTempYn / (CIE_Result.cieTempXn + 15.0 * CIE_Result.cieTempYn + 3.0 * CIE_Result.cieTempZn);
//	  }
//
//	  //'[u*,v*]
//	  CIE_Result.cieStarU = 13.0 * CIE_Result.cieStarL * (CIE_Result.cieDashU - uDn);
//	  CIE_Result.cieStarV = 13.0 * CIE_Result.cieStarL * (CIE_Result.cieDashV - vDn);
//
///*
//'====================================================
//'
//'   ï¿½Fï¿½xï¿½ï¿½ï¿½è�€ï¿½ÚŠeï¿½ï¿½vï¿½ZPart2ï¿½ixyz,Lab,a*b*,u'v',u*v*ï¿½j
//'   COD_SdataModiï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½Fï¿½xï¿½ï¿½ï¿½vï¿½Z
//'   ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½@ï¿½\ï¿½Ì‚ï¿½ï¿½ß‚É‚Qï¿½Â‚É•ï¿½ï¿½ï¿½
//'  Part1ï¿½Å‚Í�uXYZï¿½vï¿½ï¿½ï¿½ß�Aï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½APart2ï¿½É‚Ä•â�³ï¿½ï¿½ï¿½XYZï¿½É‚ÄŒvï¿½Z
//'
//'====================================================
//Public Sub ComputeCIEpart2()
//
//Dim uDn As Double, vDn As Double
//
//
//On Error Resume Next
//
//With CIE_Result
//	   '[x,y,z]
//	   If (.cieLargeX + .cieLargeY + .cieLargeZ) <> 0 Then
//		  .cieSmallx = .cieLargeX / (.cieLargeX + .cieLargeY + .cieLargeZ)
//		  .cieSmally = .cieLargeY / (.cieLargeX + .cieLargeY + .cieLargeZ)
//		  .cieSmallz = .cieLargeZ / (.cieLargeX + .cieLargeY + .cieLargeZ)
//	  End If
//
//	  '[L,a,b]
//	  .cieNormalL = 10# * Sqr(.cieLargeY)
//	  If Sqr(.cieLargeY) <> 0 Then
//		  .cieNormalA = 17.5 * (1.02 * .cieLargeX - .cieLargeY) / Sqr(.cieLargeY)
//		  .cieNormalB = 7# * (.cieLargeY - 0.847 * .cieLargeZ) / Sqr(.cieLargeY)
//	  End If
//
//	  '[L*,a*,b*]
//	  If .cieTempYn <> 0 Then
//		  .cieStarL = 116# * (.cieLargeY / .cieTempYn) ^ (1# / 3#) - 16#
//	  End If
//	  If .cieTempXn <> 0 And .cieTempYn <> 0 Then
//		  .cieStarA = 500# * ((.cieLargeX / .cieTempXn) ^ (1# / 3#) - (.cieLargeY / .cieTempYn) ^ (1# / 3#))
//	  End If
//	  If .cieTempYn <> 0 And .cieTempZn <> 0 Then
//		  .cieStarB = 200# * ((.cieLargeY / .cieTempYn) ^ (1# / 3#) - (.cieLargeZ / .cieTempZn) ^ (1# / 3#))
//	  End If
//
//	  '[u',v']
//	   If (.cieLargeX + 15# * .cieLargeY + 3# * .cieLargeZ) <> 0 Then
//		  .cieDashU = 4# * .cieLargeX / (.cieLargeX + 15# * .cieLargeY + 3# * .cieLargeZ)
//		  .cieDashV = 9# * .cieLargeY / (.cieLargeX + 15# * .cieLargeY + 3# * .cieLargeZ)
//	  End If
//
//	  '[u'n,v'n (Complete refrection)]ï¿½\ï¿½ï¿½ï¿½È‚ï¿½
//	   If (.cieTempXn + 15# * .cieTempYn + 3# * .cieTempZn) <> 0 Then
//		  uDn = 4# * .cieTempXn / (.cieTempXn + 15# * .cieTempYn + 3# * .cieTempZn)
//		  vDn = 9# * .cieTempYn / (.cieTempXn + 15# * .cieTempYn + 3# * .cieTempZn)
//	  End If
//
//	  '[u*,v*]
//	  .cieStarU = 13 * .cieStarL * (.cieDashU - uDn)
//	  .cieStarV = 13 * .cieStarL * (.cieDashV - vDn)
//End With
//
//End Sub
//*/
#endif // 0
}

// --------------------------------------------------------------------
// Readsio2File
BOOL CSrAnalysis::Readsio2File(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("CSrAnalysis::Readsio2File()\n"));

// (* sample *)
// "sio2(20a)_si-jaw(200-900nm).dat"
//SiO2(20A)/Si-JAW
//VASEmethod[Normal Fit Generation for sio2(20)_si-jaw.dat and unsaved model, EllipsometerType=0, PolRT=45.00, WVASE=3.474, Wed Sep 19 09:06:43 2007]
//Original[sio2(20a)_si-jaw(200-900nm).dat]
//nm
//uR	200.000000	0.000000	0.65107	0.0001
//uR	201.000000	0.000000	0.651471	0.0001
//uR	202.000000	0.000000	0.651919	0.0001
//uR	203.000000	0.000000	0.652406	0.0001

	TCHAR szFilename[_MAX_PATH + 1];
	const int nMaxSize = 256;
	TCHAR szLine[nMaxSize];

	FILE* fp;

	if ( !pMeasProgInfo->Ref2nd.bMeasure ) {
		_stprintf(szFilename, _T("%s%s%s"), REFERENCEMATERIALPATH, pMeasProgInfo->Ref.OptDesc.szRefMaterial, ".dat");
	}
	else {
		_stprintf(szFilename, _T("%s%s%s"), REFERENCEMATERIALPATH, pMeasProgInfo->Ref2nd.szRefMaterial, ".dat");
	}
/* added 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ MeaSys.dll ---------- { ---------- */
	AddAbsPath(szFilename);
/* added 2009.07.07 hmenjo dll ï¿½ï¿½ï¿½Îƒpï¿½Xï¿½Î‰ï¿½ MeaSys.dll ---------- } ---------- */

	if ( (fp = fopen(szFilename, _T("r"))) == NULL ) {
		TCHAR szBuff[256];
		_stprintf(szBuff, _T("%s%s"), szFilename, _T("ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½"));
//		::MessageBox(NULL, szBuff, _T("NanoSpec"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	const int HEADERLINENUM = 4;
	for ( int i = 0; i < HEADERLINENUM; i++ ) {
		_fgetts(szLine, nMaxSize, fp);
	}

	int posnm;
	double wave;
	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
	{
		_tcstok(szLine, _T("\t"));
		posnm = atoi(_tcstok(NULL, _T("\t")));
		_tcstok(NULL, _T("\t"));
		wave = atof(_tcstok(NULL, _T("\t")));
		m_RelRefl[posnm-1] = wave;
	}
	fclose(fp);

	return TRUE;
}

// --------------------------------------------------------------------
// DoXmp
BOOL CSrAnalysis::DoXmp(LPTSTR pszXmpDdeText, ANALYSISDATA* pAnalysisData, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, CDdeClient* pXmpDde)
{
	TRACE(_T("CSrAnalysis::DoXmp()\n"));

	TCHAR szBuff[256];
	SR_XMP srXmp;

	ConfigFile_GetNanoSpecIni(&srXmp, CONFIG_FILE_SR_XMP);

	// AFT2ADAP
	if ( !WriteAft2AdapFile(pAnalysisData, pMainRcpInfo, pMeasProgInfo, srXmp.szDataPath) )
		return FALSE;

	// FITDATAï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½M
// 2009.09.14 K.Matsuo NanoSpec6500,9300ï¿½Rï¿½}ï¿½ï¿½ï¿½h ï¿½İŠï¿½ï¿½Î‰ï¿½ -->
//		6500 : strategy
//		9300 : StratNum
//	_stprintf(szBuff, _T("FitData(strategy=%02d,)"), atoi(pMeasProgInfo->ScanParams)._SR.XMPDesc.szStrategyEntry);
	_stprintf(szBuff, _T("FitData(strategy=%02d,StratNum=%02d)"),
			atoi(pMeasProgInfo->ScanParams._SR.XMPDesc.szStrategyEntry), atoi(pMeasProgInfo->ScanParams._SR.XMPDesc.szStrategyEntry));
// 2009.09.14 K.Matsuo NanoSpec6500,9300ï¿½Rï¿½}ï¿½ï¿½ï¿½h ï¿½İŠï¿½ï¿½Î‰ï¿½ <--

	// DoXmp
	if ( !pXmp->DoXmp(pszXmpDdeText, szBuff, pXmpDde) )
		return FALSE;

	// GENDATA
	if ( !ReadGenDataFile(pAnalysisData, srXmp.szFittingDataPath) )
		return FALSE;

	return TRUE;
}

// --------------------------------------------------------------------
// WriteAft2AdapFile
BOOL CSrAnalysis::WriteAft2AdapFile(const ANALYSISDATA* pAnalysisData, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, LPCTSTR pszFullPath/*= _T("C:\\WVASE32\\Adap\\aft2adap.dat")*/)
{
	TRACE(_T("CSrAnalysis::WriteAft2AdapFile()\n"));

	//const int& iLens = pMainRcpInfo->MainRcpParam._SR.iLens;
	const int& iLens = pMeasProgInfo->ScanParams._SR.iLens;

	const double* dDstArray = pAnalysisData->dAnalysisReflectanceData;

	TCHAR szBuff[256];
	SR_TURRET srTurret[SR_LENS_MAX];
	double dXmpAngle;
	FILE* fp;
	CTimer timer;
// 2014.04.03 bagus wavelength step added -->
	int iIndex;
	double dWavelengthStep = pAnalysisData->dWavelengthStep;
// 2014.04.03 bagus wavelength step added <--

	// Configï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½xmpAngleï¿½æ“¾
	ConfigFile_GetNanoSpecIni(srTurret, CONFIG_FILE_SR_TURRET);
	dXmpAngle = srTurret[iLens].dXmpAngle;

/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
	if (2 == g_lBatchFlag) {
		BOOL l_bRslt = FALSE;
		if (FALSE != this->CopyAft2AdapFileSpecified(dWavelengthStep, dXmpAngle, pszFullPath)) {
			l_bRslt = TRUE;
/* added 2016.07.08 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½2 ---------- { ---------- */
			if ((pMeasProgInfo->ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE) ||
				(pMeasProgInfo->ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE_CIE)) {
				/* ï¿½ï¿½ï¿½Ë—ï¿½ï¿½Ì�ê�‡ï¿½Í“ï¿½ï¿½ï¿½ï¿½Ï�ï¿½(pAnalysisData)ï¿½ÉƒZï¿½bï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
				memset((void*) pAnalysisData->dAnalysisReflectanceData, 0, sizeof(pAnalysisData->dAnalysisReflectanceData));
				if (0 == (fp = fopen(pszFullPath, _T("r")))) {
					l_bRslt = FALSE;
				} else {
					char l_szLine[256];
					double* ll_pdDstArray = (double*) dDstArray;
					double ll_dScanStart = -1.0;
					double ll_dScanEnd = -1.0;
					char ll_sz_uR[32];
					double ll_dWave;
					long ll_lWave;
					double ll_dXmpAngle;
					double ll_dDstArray;
					double ll_dDummy;
					while (0 == feof(fp)) {
						fgets(l_szLine, sizeof(l_szLine) - 1, fp);
						if (0 != ferror(fp)) {
							/* ï¿½Gï¿½ï¿½ï¿½[	*/
							l_bRslt = FALSE;
							break;
						}
						if (0 == strncmp(l_szLine, _T("uR"), 2)) {
							if (dWavelengthStep == 0.5) {
								sscanf(l_szLine, _T("%s %lf %lf %lf %lf"), ll_sz_uR, &ll_dWave, &ll_dXmpAngle, &ll_dDstArray, &ll_dDummy);
								if (ll_dScanStart < 0) {
									ll_dScanStart = ll_dWave;
								}
								ll_dScanEnd = ll_dWave;
								iIndex = (int) ((ll_dScanEnd - ll_dScanStart) * 2);
							} else {
								sscanf(l_szLine, _T("%s %d %lf %lf %lf"), ll_sz_uR, &ll_lWave, &ll_dXmpAngle, &ll_dDstArray, &ll_dDummy);
								if (ll_dScanStart < 0) {
									ll_dScanStart = ll_lWave;
								}
								ll_dScanEnd = ll_lWave;
								iIndex = (int) (ll_dScanEnd - ll_dScanStart);
							}
							ll_pdDstArray[iIndex] = ll_dDstArray;
						}
					}
					fclose(fp);
					if ((ll_dScanStart < 0) || (ll_dScanEnd < 0)) {
						l_bRslt = FALSE;
				} else {
					int* ll_piScanStart = (int*) &(pAnalysisData->iScanStart);
					int* ll_piScanEnd = (int*) &(pAnalysisData->iScanEnd);
					*ll_piScanStart = static_cast<int>(ll_dScanStart);
					*ll_piScanEnd = static_cast<int>(ll_dScanEnd);
				}
				}
			}
/* added 2016.07.08 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½2 ---------- } ---------- */
		}
		/* ï¿½oï¿½bï¿½`ï¿½vï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A	*/
		g_lBatchFlag = 0;
		return l_bRslt;
	} else {
		/* ï¿½oï¿½bï¿½`ï¿½vï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A	*/
		g_lBatchFlag = 0;
	}
/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */

	if ( (fp = fopen(pszFullPath, _T("w"))) == NULL )  {
		_stprintf(szBuff, _T("%s%s"), pszFullPath, _T("ï¿½Ì�ì�¬ï¿½É�ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½"));
//		::MessageBox(NULL, szBuff, _T("NanoSpec"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	_ftprintf(fp, _T("%s\n"), timer.ToChar());
	_ftprintf(fp, _T("nm\n"));

// 2014.04.03 bagus wavelength step modified -->
//	for ( int i = pAnalysisData->iScanStart - 1; i < pAnalysisData->iScanEnd; i++ ) {
//		_ftprintf(fp, _T("uR %d %lf %lf 0.0001 \n"), i+1, dXmpAngle, dDstArray[i]);
//	}
	double dScanStart = (double)pAnalysisData->iScanStart;
	double dScanEnd = (double)pAnalysisData->iScanEnd;

	double p = dScanStart;
	while(p <= dScanEnd){
		if(dWavelengthStep == 0.5){
			iIndex = (int)((p - dScanStart) * 2);
			_ftprintf(fp, _T("uR %.1lf %lf %lf 0.0001 \n"), p, dXmpAngle, dDstArray[iIndex]);
		}
		else{
			iIndex = (int)(p - dScanStart);
			_ftprintf(fp, _T("uR %d %lf %lf 0.0001 \n"), (int)p, dXmpAngle, dDstArray[iIndex]);
		}

		p += dWavelengthStep;
	}
// 2014.04.03 bagus wavelength step modified <--
	fclose(fp);

	return TRUE;
}

// --------------------------------------------------------------------
// ReadGenDataFile
BOOL CSrAnalysis::ReadGenDataFile(ANALYSISDATA* pAnalysisData, LPCTSTR pszFullPath/* = _T("C:\\WVASE32\\Adap\\gendata.dat")*/)
{
	TRACE(_T("CSrAnalysis::ReadGenDataFile()\n"));

	double* dDstArray = pAnalysisData->dAnalysisFitData;

	TCHAR szBuff[256];
	CStdioFile file;
	TCHAR* token;
// 2014.04.03 bagus wavelength step modified -->
//	int iWave;
	double dWaveLen;
	int iIndex;
	double dWavelengthStep = pAnalysisData->dWavelengthStep;
	double dScanStart = (double)pAnalysisData->iScanStart;
// 2014.04.03 bagus wavelength step modified <--

	if ( !file.Open(pszFullPath, CFile::modeRead | CFile::typeText | CFile::shareDenyNone) )  {
		_stprintf(szBuff, _T("%s%s"), pszFullPath, _T("ï¿½Ì“Ç�ï¿½ï¿½İ‚É�ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½"));
//		::MessageBox(NULL, szBuff, _T("NanoSpec"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	file.ReadString(szBuff, sizeof(szBuff) - 1);
	file.ReadString(szBuff, sizeof(szBuff) - 1);
	for ( int i = 0; i < 2; i++ ) {
		if ( _tcscmp(szBuff, _T("nm")) == 0 || _tcscmp(szBuff, _T("1/cm")) == 0 )
			break;
		file.ReadString(szBuff, sizeof(szBuff) - 1);
	}

	while ( file.ReadString(szBuff, sizeof(szBuff) - 1) ) {
		token = _tcstok(szBuff, _T(" \t"));
		token = _tcstok(NULL, _T(" \t"));
// 2014.04.03 bagus wavelength step modified -->
//		iWave = _ttoi(token);
		if(dWavelengthStep == 0.5){
			dWaveLen = atof(token);
			iIndex = (int)((dWaveLen - dScanStart)*2);
		}
		else{
			iIndex = (int)(atof(token) - dScanStart);
		}
// 2014.04.03 bagus wavelength step modified <--
		token = _tcstok(NULL, _T(" \t"));
		token = _tcstok(NULL, _T(" \t"));
// 2014.04.03 bagus wavelength step modified -->
//		dDstArray[iWave - 1] = atof(token);
		dDstArray[iIndex] = atof(token);
// 2014.04.03 bagus wavelength step modified <--
	}

	return TRUE;
}

// --------------------------------------------------------------------
// ReadIllumiFile
BOOL CSrAnalysis::ReadIllumiFile(WORD wIlluminant)
{
#if 0
//	TRACE(_T("CSrAnalysis::ReadIllumiFile()\n"));
//// (* sample *)
////[Illuminant_D65]
////300, 0.034100
////301, 0.360140
////302, 0.686180
////303, 1.012220
//
//	  TCHAR szFilename[_MAX_PATH];
//	  const int nMaxSize = 256;
//	  TCHAR szLine[nMaxSize];
//																						   //ï¿½bï¿½ï¿½
//	  FILE* fp;
//
//	  _stprintf(szFilename, _T("%s%s%s"), szPathIll, ILLUMINANT_TABLE_ITEM[wIlluminant], CIE_EXT);
//	  if ( (fp = fopen(szFilename, _T("r"))) == NULL ) {
//		TCHAR szBuff[256];
//		_stprintf(szBuff, _T("%s%s"), szFilename, _T("ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½"));
//		::MessageBox(NULL, szBuff, _T("NanoSpec"), MB_OK | MB_ICONERROR);
//		  return FALSE;
//	  }
//
//	  const int HEADERLINENUM = 1;
//	for ( int i = 0; i < HEADERLINENUM; i++ )
//	  {
//		  _fgetts(szLine, nMaxSize, fp);
//	  }
//
//	  int wave;
//	  double illu;
//	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
//	  {
//		  wave = atoi(_tcstok(szLine, _T(",")));
//		  if ( m_scanStart <= wave && wave <= m_scanEnd )
//		  {
//		  	illu = atof(_tcstok(NULL, _T(",")));
//			  m_Illumi[wave] = illu;
//		  }
//	  }
//	  fclose(fp);
#endif // 0
	return TRUE;
}

// --------------------------------------------------------------------
// ReadCIECMFFile
BOOL CSrAnalysis::ReadCIECMFFile()
{
#if 0
///*
// CIE Color Matching Functions
//
//'CIE 1931 2-deg XYZ Color Matching Functions for wavelenghts in 1 nm
//'increments from 380 nm to 780 nm.
//
//'CIE 1931 2-deg XYZ Color Matching Functionsï¿½ï¿½`ï¿½lï¿½zï¿½ï¿½
//'CIE_CMFdefine(CIE_MINWAVE To CIE_MAXWAVE)
//'CIE_CMFtable
//
//*/
//
//	TRACE(_T("CSrAnalysis::ReadCIECMFFile()\n"));
//
//	// (* sample *)
////CIE 1931 2-deg XYZ Color Matching Functions
////[database]
////360,  0.000129900000,  0.000003917000,	0.000606100000
////361,  0.000145847000,  0.000004393581,	0.000680879200
////362,  0.000163802100,  0.000004929604,	0.000765145600
//
//	  TCHAR szFilename[_MAX_PATH];
//	  const int nMaxSize = 256;
//	  TCHAR szLine[nMaxSize];
//
//	  FILE* fp;
//
//	  _stprintf(szFilename, _T("%s%s"),szPathIll, szCIECMFFilename);
//	  if ( (fp = fopen(szFilename, _T("r"))) == NULL )
//	  {
//		TCHAR szBuff[256];
//		_stprintf(szBuff, _T("%s%s"), szFilename, _T("ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½"));
//		::MessageBox(NULL, szBuff, _T("NanoSpec"), MB_OK | MB_ICONERROR);
//		  return FALSE;
//	  }
//
//	  const int HEADERLINENUM = 2;
//	for ( int i = 0; i < HEADERLINENUM; i++ )
//	  {
//		  _fgetts(szLine, nMaxSize, fp);
//	  }
//
//	  int wave;
//	  double dBuff;
//	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
//	  {
//		  wave = atoi(_tcstok(szLine, _T(",")));
//		  if ( m_scanStart <= wave && wave <= m_scanEnd )
//		  {
//		  	dBuff = atof(_tcstok(NULL, _T(",")));
//			  m_CMFdefine[wave].xbar = dBuff;
//		  	dBuff = atof(_tcstok(NULL, _T(",")));
//			  m_CMFdefine[wave].ybar = dBuff;
//		  	dBuff = atof(_tcstok(NULL, _T(",")));
//			  m_CMFdefine[wave].zbar = dBuff;
//		  }
//	  }
//	  fclose(fp);
#endif // 0
	return TRUE;
}

/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
/* ï¿½eï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½p	*/
BOOL CALLBACK EnumMainWindowProc(HWND hWnd, LPARAM lParam)
{
	DWORD l_dwProcID;
	LPDWORD l_dwProcIDsrc = (LPDWORD) lParam;

	::GetWindowThreadProcessId(hWnd, &l_dwProcID);
	if (l_dwProcID == *l_dwProcIDsrc) {
		*l_dwProcIDsrc = (DWORD) hWnd;
		return FALSE;
	}

	return TRUE;
}
/* MFC ï¿½Æ“ï¿½ï¿½ï¿½ï¿½^ï¿½Cï¿½vï¿½Ìƒtï¿½@ï¿½Cï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½\ï¿½ï¿½ï¿½p	*/
UINT CALLBACK OFNHookProcOldStyle(
	HWND hdlg,		/* ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½{ï¿½bï¿½Nï¿½Xï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½Ìƒnï¿½ï¿½ï¿½hï¿½ï¿½	*/
	UINT uiMsg,		/* ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½ï¿½Ê�q	*/
	WPARAM wParam,	/* ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½pï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½^	*/
	LPARAM lParam	/* ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½pï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½^	*/
)
{
	switch (uiMsg) {
	case WM_INITDIALOG:
		{
			/* ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½fï¿½Xï¿½Nï¿½gï¿½bï¿½vï¿½Ì’ï¿½ï¿½ï¿½ï¿½É•\ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
			HWND l_hwndDesktop = ::GetDesktopWindow();
			RECT l_rectDesktop;
			::GetWindowRect(l_hwndDesktop, &l_rectDesktop);
			RECT l_rectOfn;
			::GetWindowRect(::GetParent(hdlg), &l_rectOfn);
			int l_iXnew = ((l_rectDesktop.right - l_rectDesktop.left) - (l_rectOfn.right - l_rectOfn.left)) / 2;
			int l_Ynew = ((l_rectDesktop.bottom - l_rectDesktop.top) - (l_rectOfn.bottom - l_rectOfn.top)) / 2;
			::SetWindowPos(::GetParent(hdlg), 0, l_iXnew, l_Ynew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
		break;
	}

	return 0;
}
/* ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½\ï¿½ï¿½ï¿½Æ‘Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
BOOL CSrAnalysis::CopyAft2AdapFileSpecified(double dWavelengthStep, double dXmpAngle, LPCTSTR ptszFullPath/* = _T("C:\\WVASE32\\Adap\\aft2adap.dat")*/)
{
	BOOL l_bRc = FALSE;
	static TCHAR ls_tszAft2AdapPathSrc[MAX_PATH] = _T("");	/* ï¿½Oï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Ìƒtï¿½ï¿½ï¿½pï¿½X	*/

	/* ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½æ“¾	*/
	HWND l_hwndMainWnd = 0;
	DWORD l_dwProcID = ::GetCurrentProcessId();
	if (FALSE == ::EnumWindows(EnumMainWindowProc, (LPARAM) &l_dwProcID)) {
		if (0 == ::GetLastError()) {
			l_hwndMainWnd = (HWND) l_dwProcID;
		}
	}

	TCHAR l_tszAft2AdapPathSrc[MAX_PATH] = _T("");
	TCHAR l_tszAft2AdapDir[MAX_PATH] = _T("");
	/* ï¿½Oï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Í‘ï¿½ï¿½İ‚ï¿½ï¿½é‚©ï¿½Hï¿½H	*/
	if (-1 != ::GetFileAttributes(ls_tszAft2AdapPathSrc)) {
		/* ï¿½ï¿½ï¿½ï¿½	*/
		_tcscpy(l_tszAft2AdapPathSrc, ls_tszAft2AdapPathSrc);
	} else {
		/* ï¿½ï¿½ï¿½È‚ï¿½	*/
		_tcscpy(l_tszAft2AdapPathSrc, _T(""));
		/* ï¿½Oï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Í‘ï¿½ï¿½İ‚ï¿½ï¿½é‚©ï¿½Hï¿½H	*/
		BOOL l_bDef = FALSE;
		TCHAR *l_ptszNull;
		_tcscpy(l_tszAft2AdapDir, ls_tszAft2AdapPathSrc);
		l_ptszNull = _tcsrchr(l_tszAft2AdapDir, _T('\\'));
		if (0 == l_ptszNull) {
			/* ï¿½pï¿½Xï¿½ï¿½'\'ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
			l_bDef = TRUE;
		} else {
			*(l_ptszNull + 1) = 0;
			if (-1 == ::GetFileAttributes(l_tszAft2AdapDir)) {
				/* ï¿½ï¿½ï¿½İ‚ï¿½ï¿½È‚ï¿½	*/
				l_bDef = TRUE;
			}
		}
		if (FALSE != l_bDef) {
			l_bDef = FALSE;
			/* ptszFullPath ï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
			_tcscpy(l_tszAft2AdapDir, ptszFullPath);
			l_ptszNull = _tcsrchr(l_tszAft2AdapDir, _T('\\'));
			/* ptszFullPath ï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Í‘ï¿½ï¿½İ‚ï¿½ï¿½é‚©ï¿½Hï¿½H	*/
			if (0 == l_ptszNull) {
				/* ï¿½pï¿½Xï¿½ï¿½'\'ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				l_bDef = TRUE;
			} else {
				*(l_ptszNull + 1) = 0;
				if (-1 == ::GetFileAttributes(l_tszAft2AdapDir)) {
					/* ï¿½ï¿½ï¿½È‚ï¿½	*/
					l_bDef = TRUE;
				}
			}
			if (FALSE != l_bDef) {
				_tcscpy(l_tszAft2AdapDir, _T("C:\\"));
			}
		}
	}

	/* ï¿½Rï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½É‚ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Iï¿½ï¿½	*/
	OPENFILENAME l_OpenFileName;
	memset(&l_OpenFileName, 0, sizeof(l_OpenFileName));
	l_OpenFileName.lStructSize = sizeof(OPENFILENAME);
	l_OpenFileName.hwndOwner = l_hwndMainWnd;
	l_OpenFileName.lpstrFilter = _T("dat Files (*.dat)\0*.dat\0\0");
	l_OpenFileName.nFilterIndex = 1;
	l_OpenFileName.lpstrFile = l_tszAft2AdapPathSrc;
	l_OpenFileName.nMaxFile = MAX_PATH;
	l_OpenFileName.lpstrInitialDir = l_tszAft2AdapDir;
	l_OpenFileName.lpfnHook = OFNHookProcOldStyle;	/* ï¿½tï¿½bï¿½Nï¿½vï¿½ï¿½ï¿½Vï¿½[ï¿½Wï¿½ï¿½	*/
	l_OpenFileName.Flags =
						OFN_FILEMUSTEXIST |		/* ï¿½ï¿½ï¿½İ‚ï¿½ï¿½ï¿½pï¿½Xï¿½Ì‚İ—Lï¿½ï¿½	*/
						OFN_PATHMUSTEXIST |		/* ï¿½ï¿½ï¿½İ‚ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Ì‚İ—Lï¿½ï¿½	*/
						OFN_NONETWORKBUTTON |	/* ï¿½lï¿½bï¿½gï¿½ï¿½ï¿½[ï¿½Nï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½	*/
						OFN_ENABLEHOOK |		/* ï¿½tï¿½bï¿½Nï¿½vï¿½ï¿½ï¿½Vï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½Lï¿½ï¿½	*/
						OFN_EXPLORER |			/* ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Gï¿½Nï¿½Xï¿½vï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½^ï¿½Cï¿½ï¿½	*/
						0;
	/* ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½gï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½	*/
	TCHAR l_tszCurrDir[MAX_PATH];
	_tgetcwd(l_tszCurrDir, MAX_PATH - 1);
	if (FALSE != ::GetOpenFileName(&l_OpenFileName)) {
		/* ï¿½mï¿½ï¿½	*/
//		::MessageBox(l_hwndMainWnd, l_tszAft2AdapPathSrc, l_tszAft2AdapPathSrc, MB_OK);
		/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½`ï¿½Fï¿½bï¿½N	*/
		TCHAR l_tszTargetPath[MAX_PATH];
		BOOL l_bTempFile = FALSE;
		if (0 == _tcsicmp(ptszFullPath, l_tszAft2AdapPathSrc)) {
			/* ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½Íƒeï¿½ï¿½ï¿½|ï¿½ï¿½ï¿½ï¿½ï¿½ÉƒRï¿½sï¿½[ï¿½ï¿½ï¿½Ü‚ï¿½(_mktemp ï¿½Í�gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½)ï¿½D	*/
			_tcscpy(l_tszTargetPath, ptszFullPath);
			_tcscat(l_tszTargetPath, _T("batibati"));
			::CopyFile(l_tszAft2AdapPathSrc, l_tszTargetPath, FALSE);
			l_bTempFile = TRUE;
		} else {
			_tcscpy(l_tszTargetPath, l_tszAft2AdapPathSrc);
		}
		long l_lReadErr = 0;
		FILE *l_pfp;
		if (0 == (l_pfp = _tfopen(l_tszTargetPath, _T("rt")))) {
			/* ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[	*/
			l_lReadErr = 1;
		} else {
			TCHAR l_tszLine[256];
			/* ï¿½Pï¿½sï¿½Ú�Fï¿½ï¿½Ç‚ï¿½	*/
			if (0 == _fgetts(l_tszLine, 255, l_pfp)) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½Cï¿½ï¿½ï¿½é‚¢ï¿½Í�Cï¿½Iï¿½[	*/
				l_lReadErr = 2;
			}
			/* ï¿½Qï¿½sï¿½Ú�F"nm"ï¿½Ì‚Í‚ï¿½	*/
			else if (0 == _fgetts(l_tszLine, 255, l_pfp)) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½Cï¿½ï¿½ï¿½é‚¢ï¿½Í�Cï¿½Iï¿½[	*/
				l_lReadErr = 3;
			}
			else if (0 != _tcscmp(l_tszLine, _T("nm\n"))) {
				/* "nm"ï¿½Å‚Í‚È‚ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				l_lReadErr = 4;
			}
			/* ï¿½Rï¿½sï¿½Ú�Fï¿½Å�ï¿½ï¿½Ìƒfï¿½[ï¿½^	*/
			else if (0 == _fgetts(l_tszLine, 255, l_pfp)) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½Cï¿½ï¿½ï¿½é‚¢ï¿½Í�Cï¿½Iï¿½[	*/
				l_lReadErr = 5;
			} else {
				/* ï¿½fï¿½[ï¿½^ï¿½sï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½	*/
				TCHAR l_tszSymbol[128];
				TCHAR l_tszWaveLen[128];
				TCHAR l_tszAngle[128];
				TCHAR l_tszReflectance[128];
				_stscanf(l_tszLine, _T("%s %s %s %s"), l_tszSymbol, l_tszWaveLen, l_tszAngle, l_tszReflectance);
				if (0 != _tcscmp(l_tszSymbol, _T("uR"))) {
					/* ï¿½æ“ªï¿½ï¿½"uR"ï¿½Å‚È‚ï¿½ï¿½D	*/
					l_lReadErr = 6;
				} else {
					BOOL l_bWaveLenStep0_5 = FALSE;
					if (0 != _tcsrchr(l_tszWaveLen, _T('.'))) {
						l_bWaveLenStep0_5 = TRUE;
					}
					if ((0.5 == dWavelengthStep) && (FALSE == l_bWaveLenStep0_5)) {
						/* ï¿½Xï¿½eï¿½bï¿½vï¿½Gï¿½ï¿½ï¿½[	*/
						l_lReadErr = 7;
					}
					if ((1.0 == dWavelengthStep) && (FALSE != l_bWaveLenStep0_5)) {
						/* ï¿½Xï¿½eï¿½bï¿½vï¿½Gï¿½ï¿½ï¿½[	*/
						l_lReadErr = 8;
					}
				}
			}
			fclose(l_pfp);
			if (0 == l_lReadErr) {
				/* ï¿½ï¿½ï¿½ï¿½	*/
				_tcscpy(ls_tszAft2AdapPathSrc, l_tszAft2AdapPathSrc);
				::CopyFile(l_tszAft2AdapPathSrc, ptszFullPath, FALSE);
				l_bRc = TRUE;
			} else {
				TCHAR l_tszMsg[128];
				switch (l_lReadErr) {
				case 1:
					/* ï¿½Iï¿½[ï¿½vï¿½ï¿½ ï¿½Gï¿½ï¿½ï¿½[	*/
					_tcscpy(l_tszMsg, _T("file open error."));
					break;
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½g ï¿½Gï¿½ï¿½ï¿½[	*/
					_tcscpy(l_tszMsg, _T("file format error."));
					break;
				case 7:
				case 8:
					/* ï¿½Xï¿½eï¿½bï¿½v ï¿½Gï¿½ï¿½ï¿½[	*/
					_tcscpy(l_tszMsg, _T("wave length step error."));
					break;
				default:
					/* ï¿½ï¿½ï¿½ï¿½` ï¿½Gï¿½ï¿½ï¿½[	*/
					_tcscpy(l_tszMsg, _T("undefined error."));
					break;
				}
				::MessageBox(l_hwndMainWnd, l_tszMsg, _T("Batch"), MB_OK);
			}
		}
		if (FALSE != l_bTempFile) {
			/* ï¿½eï¿½ï¿½ï¿½|ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½í�œ	*/
			_tremove(l_tszTargetPath);
		}
	} else {
		/* ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½Cï¿½Gï¿½ï¿½ï¿½[	*/
		DWORD l_dwErr = ::CommDlgExtendedError();
		if (0 == l_dwErr) {
			::MessageBox(l_hwndMainWnd, _T("Batch cancel."), _T("Batch"), MB_OK);
		} else {
			TCHAR l_tszMsg[128];
			_stprintf(l_tszMsg, _T("Error (=0x%08x)"), l_dwErr);
			::MessageBox(l_hwndMainWnd, l_tszMsg, _T("Batch"), MB_OK);
		}
	}
	/* ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½gï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ğ•œ‹A	*/
	_tchdir(l_tszCurrDir);

	return l_bRc;
}
/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
