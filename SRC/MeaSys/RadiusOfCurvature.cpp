#include "stdafx.h"
#include <FLOAT.H>
#include "RadiusOfCurvature.h"

#define UNITCONV_UM_TO_MM					(0.001)			// 10^(-3)
#define UNITCONV_MM_TO_UM					(1000)			// 10^3

// --------------------------------------------------------------------
// CRadiusOfCurvature
CRadiusOfCurvature::CRadiusOfCurvature()
{
//	TRACE(_T("CRadiusOfCurvature::CRadiusOfCurvature()\n"));

	m_dStageSpeed = 0.0;

	m_pDeflectionThetaFitting = new CCurveFitting(CCurveFitting::COEF2);		// 1A
}

// --------------------------------------------------------------------
// ~CRadiusOfCurvature
CRadiusOfCurvature::~CRadiusOfCurvature()
{
//	TRACE(_T("CRadiusOfCurvature::~CRadiusOfCurvature()\n"));

	if ( m_pDeflectionThetaFitting ) {
		delete m_pDeflectionThetaFitting;
		m_pDeflectionThetaFitting = NULL;
	}
}

// --------------------------------------------------------------------
// SetStageSpeed
void CRadiusOfCurvature::SetStageSpeed(double dStageSpeed)
{
	TRACE(_T("CRadiusOfCurvature::SetStageSpeed()\n"));

	/* Xe[Wx(mm/s)o^܂ */
	m_dStageSpeed = dStageSpeed;
}

// --------------------------------------------------------------------
// SetDeflection
void CRadiusOfCurvature::SetDeflection(const double* pDeflection, int nOccurence)
{
	TRACE(_T("CRadiusOfCurvature::SetDeflection()\n"));

	/* ψʗʁi݁jo^܂ */
	m_DeflectionArray.RemoveAll();
	m_DeflectionArray.SetSize(nOccurence);
	int i;

	for ( i = 0; i < nOccurence; i++ )
		m_DeflectionArray.SetAt(i, pDeflection[i] * UNITCONV_MM_TO_UM);
}

// --------------------------------------------------------------------
// SetDeflectionAtZero
void CRadiusOfCurvature::SetDeflectionAtZero(double** pDeflection, int nOccurence)
{
	TRACE(_T("CRadiusOfCurvature::SetDeflectionAtZero()\n"));

	/* ψʗʁi݁jo^܂ */
	m_DeflectionAtZeroData.RemoveAll();
	int i;

	for ( i = 0; i < nOccurence; i++ )
		m_DeflectionAtZeroData.SetAt(pDeflection[i][0] * UNITCONV_UM_TO_MM, pDeflection[i][1]);
}

// --------------------------------------------------------------------
// Calc
//void CRadiusOfCurvature::Calc(double dDeflStartmm, double dDeflEndmm, double dDeflStepmm)
//{
//	TRACE(_T("CRadiusOfCurvature::Calc()\n"));
//
//// 2009.09.15 K.Matsuo t@XƉ͑莞̗LCsvP[Xւ̑Ή -->
////	if ( m_DeflectionArray.GetSize() == 0 )
////		return FALSE;
//// 2009.09.15 K.Matsuo t@XƉ͑莞̗LCsvP[Xւ̑Ή <--
//
//	// ψʗʁi݁j̎擾
//	CalcDeflectionData(dDeflStartmm, dDeflEndmm, dDeflStepmm);
//	// \ʂ̌X΁i(=dy/dx)j
//	CalcDeflectionTheta(dDeflStartmm, dDeflEndmm, dDeflStepmm);
//	// \ʂ̌X΂ȗa߂邽߁A1AvZ
//	CalcDeflectionThetaLinest(dDeflStartmm, dDeflEndmm, dDeflStepmm);
//}

// --------------------------------------------------------------------
// GetDthetaDx
double CRadiusOfCurvature::GetDthetaDx(double dPosmm)
{
//	TRACE(_T("CRadiusOfCurvature::GetDthetaDx()\n"));

	/* d/dx擾܂ */
	double dRet;

	if ( !m_DeflectionThetaLineStMap.Lookup(dPosmm, dRet) )
		ASSERT(FALSE);

	return dRet;
}

// --------------------------------------------------------------------
// GetRadius
double CRadiusOfCurvature::GetRadius(double dPosmm)
{
	TRACE(_T("CRadiusOfCurvature::GetRadius()\n"));

	/* Radius ߂܂ */
//	return 1.0 / GetDthetaDx(dPosmm);
	if( GetDthetaDx(dPosmm) == 0.0 )
		return DBL_MAX;
	else
		return 1.0 / GetDthetaDx(dPosmm);
}

// --------------------------------------------------------------------
// CalcDeflectionData
void CRadiusOfCurvature::CalcDeflectionData(double dDeflStartmm, double dDeflEndmm, double dDeflStepmm)
{
	TRACE(_T("CRadiusOfCurvature::CalcDeflectionData()\n"));

	m_DeflectionData.RemoveAll();
	m_DeflectionAtZeroData.RemoveAll();

	double dDefl;
	double d = dDeflStartmm;
	while ( d <= dDeflEndmm ) {
		dDefl = GetDeflectionByPos(d, dDeflStartmm);
		m_DeflectionData.SetAt(d, dDefl);
		d += dDeflStepmm;
	}
}

// --------------------------------------------------------------------
// CalcDeflectionStartAndEndAtZero
void CRadiusOfCurvature::CalcDeflectionStartAndEndAtZero(double dSectionStartX, double dSectionEndX, double dDeflStepmm, double dLineStartX, double dLineEndX, BOOL bFirstSection, BOOL bLastSection)
{
	TRACE(_T("CRadiusOfCurvature::CalcDeflectionStartAndEndAtZero()\n"));

	double dStartData;
	double dEndData;

	if ( !m_DeflectionData.Lookup(dSectionStartX, dStartData) )
		ASSERT(FALSE);
	if ( !m_DeflectionData.Lookup(dSectionEndX, dEndData) )
		ASSERT(FALSE);

	double dDefl;
	double dDeflAtZero;
	double d = dSectionStartX;
	while ( d <= dSectionEndX ) {
		if ( !m_DeflectionData.Lookup(d, dDefl) )
			ASSERT(FALSE);
		dDeflAtZero = dDefl - dStartData - ((dEndData - dStartData) / (dSectionEndX - dSectionStartX)) * ((d - dSectionStartX));
		m_DeflectionAtZeroData.SetAt(d, dDeflAtZero);
		d += dDeflStepmm;
	}

	if ( bFirstSection ) {
		d = dLineStartX;
		while ( d < dSectionStartX ) {
			if ( !m_DeflectionData.Lookup(d, dDefl) )
				ASSERT(FALSE);
			dDeflAtZero = dDefl - dStartData - ((dEndData - dStartData) / (dSectionEndX - dSectionStartX)) * ((d - dSectionStartX));
			m_DeflectionAtZeroData.SetAt(d, dDeflAtZero);
			d += dDeflStepmm;
		}
	}

	if ( bLastSection ) {
		d = dSectionEndX + dDeflStepmm;
		while ( d <= dLineEndX ) {
			if ( !m_DeflectionData.Lookup(d, dDefl) )
				ASSERT(FALSE);
			dDeflAtZero = dDefl - dStartData - ((dEndData - dStartData) / (dSectionEndX - dSectionStartX)) * ((d - dSectionStartX));
			m_DeflectionAtZeroData.SetAt(d, dDeflAtZero);
			d += dDeflStepmm;
		}
	}
}

// --------------------------------------------------------------------
// CalcDeflectionTheta
void CRadiusOfCurvature::CalcDeflectionTheta(double dThetaStartmm, double dThetaEndmm, double dThetaStepmm)
{
	TRACE(_T("CRadiusOfCurvature::CalcDeflectionTheta()\n"));

	m_DeflectionThetaMap.RemoveAll();
	m_DeflectionThetaLineStMap.RemoveAll();

	double dTheta;
	double dTemp[2];
	double d = dThetaStartmm;
	while ( d + dThetaStepmm * 2 <= dThetaEndmm ) {
		if ( !m_DeflectionAtZeroData.Lookup(d + dThetaStepmm * 2, dTemp[1]) )
			ASSERT(FALSE);
		if ( !m_DeflectionAtZeroData.Lookup(d, dTemp[0]) )
			ASSERT(FALSE);
		dTheta = dTemp[1] - dTemp[0];
		m_DeflectionThetaMap.SetAt(d + dThetaStepmm, dTheta / (dThetaStepmm * 2) );
		d += dThetaStepmm;
	}
}

// --------------------------------------------------------------------
// CalcDeflectionThetaLinest
void CRadiusOfCurvature::CalcDeflectionThetaLinest(double dThetaStartmm, double dThetaEndmm, double dThetaStepmm)
{
	TRACE(_T("CRadiusOfCurvature::CalcDeflectionThetaLinest()\n"));

	/* d/dx߂܂ */
//	m_DeflectionThetaLineStMap.RemoveAll();

// 2009.10.09 K.Matsuo ZNVԂd/dx(=1/R)vZ -->
//	CURVEFITTINGST cf;
//	const int DATANUM = 7;
//
//	double dCoef;
//	double dTemp;
//	double d = dThetaStartmm + dThetaStepmm;
//	while ( d + dThetaStepmm * 6 <= dThetaEndmm - dThetaStepmm ) {
//		cf.datanum = DATANUM;
//		int i;
		for ( i = 0; i < DATANUM; i++ ) {
//			cf.point[i] = d + dThetaStepmm * i;
//			if ( !m_DeflectionThetaMap.Lookup(d + dThetaStepmm * i, dTemp) )
//				ASSERT(FALSE);
//			cf.value[i] = dTemp;
//		}
//		m_pDeflectionThetaFitting->SetParamData(cf);
//		dCoef = m_pDeflectionThetaFitting->GetCoefficient(1);				// Wa̎擾iy = ax + bj
//		m_DeflectionThetaLineStMap.SetAt(d + dThetaStepmm * 3, dCoef);
//		d += dThetaStepmm;
//	}

	CURVEFITTINGST cf;

	double dCoef;
	double dTemp;
	double d = dThetaStartmm;
	cf.datanum = 0;
	while ( d <= dThetaEndmm ) {
		cf.point[cf.datanum] = d;
		if ( !m_DeflectionThetaMap.Lookup(d, dTemp) )
			ASSERT(FALSE);
		cf.value[cf.datanum] = dTemp;
		cf.datanum++;
		d += dThetaStepmm;
	}
	m_pDeflectionThetaFitting->SetParamData(cf);
	dCoef = m_pDeflectionThetaFitting->GetCoefficient(1);				// Wa̎擾iy = ax + bj

	d = dThetaStartmm;
	while ( d <= dThetaEndmm ) {
		m_DeflectionThetaLineStMap.SetAt(d, dCoef);
		d += dThetaStepmm;
	}
// 2009.10.09 K.Matsuo <--
}

// --------------------------------------------------------------------
// GetDeflectionByPos
double CRadiusOfCurvature::GetDeflectionByPos(double dPosmm, double dOffsetmm)
{
//	TRACE(_T("CRadiusOfCurvature::GetDeflectionByPos()\n"));

	int iIndex;
	double dIndex;
	double ddIndex;
	double ddm;
	double ddp;
	double dData;

	ASSERT(m_dStageSpeed > 0.0);

	dIndex = dPosmm - dOffsetmm;
	dIndex = dIndex * ((1.0 / m_dStageSpeed) / (10.0 / 1000.0));	// (1.0 / m_dStageSpeed) / (TvOC^[o10.0 / f[^~ϊԊu1000.0)

	iIndex = (int)dIndex;
	ddIndex = dIndex - (double)iIndex;

	// dόvZ̊֌WŁACfbNXQƂsȈʒuɂȂ̂ŁA[̂P܂łpȂ悤ɂ
	if ( iIndex <= 1 ) {
		dData = m_DeflectionArray[1];
	}
	else if ( iIndex >= m_DeflectionArray.GetSize() -1 ) {
		dData = m_DeflectionArray[m_DeflectionArray.GetSize()-2];
	}
	else {
		ddIndex = dIndex - (double)iIndex;
		ddm = m_DeflectionArray[iIndex-1] * 0.5 + (m_DeflectionArray[iIndex] + m_DeflectionArray[iIndex-2]) * 0.25;
		ddp = m_DeflectionArray[iIndex] * 0.5 + (m_DeflectionArray[iIndex-1] + m_DeflectionArray[iIndex+1]) * 0.25;
		dData = ddm * (1.0 - ddIndex) + ddp * ddIndex;
	}

	return dData;
}

