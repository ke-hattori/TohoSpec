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

	m_pDeflectionThetaFitting = new CCurveFitting(CCurveFitting::COEF2);		// 1次回帰
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

	/* ステージ速度(mm/s)を登録します */
	m_dStageSpeed = dStageSpeed;
}

// --------------------------------------------------------------------
// SetDeflection
void CRadiusOfCurvature::SetDeflection(const double* pDeflection, int nOccurence)
{
	TRACE(_T("CRadiusOfCurvature::SetDeflection()\n"));

	/* 変位量（たわみ）を登録します */
	m_DeflectionArray.RemoveAll();
	m_DeflectionArray.SetSize(nOccurence);
	for ( int i = 0; i < nOccurence; i++ )
		m_DeflectionArray.SetAt(i, pDeflection[i] * UNITCONV_MM_TO_UM);
}

// --------------------------------------------------------------------
// SetDeflectionAtZero
void CRadiusOfCurvature::SetDeflectionAtZero(double** pDeflection, int nOccurence)
{
	TRACE(_T("CRadiusOfCurvature::SetDeflectionAtZero()\n"));

	/* 変位量（たわみ）を登録します */
	m_DeflectionAtZeroData.RemoveAll();
	for ( int i = 0; i < nOccurence; i++ )
		m_DeflectionAtZeroData.SetAt(pDeflection[i][0] * UNITCONV_UM_TO_MM, pDeflection[i][1]);
}

// --------------------------------------------------------------------
// Calc
//void CRadiusOfCurvature::Calc(double dDeflStartmm, double dDeflEndmm, double dDeflStepmm)
//{
//	TRACE(_T("CRadiusOfCurvature::Calc()\n"));
//
//// 2009.09.15 K.Matsuo リファレンスと応力測定時の有効ライン不一致ケースへの対応 -->
////	if ( m_DeflectionArray.GetSize() == 0 )
////		return FALSE;
//// 2009.09.15 K.Matsuo リファレンスと応力測定時の有効ライン不一致ケースへの対応 <--
//
//	// 変位量（たわみ）の取得
//	CalcDeflectionData(dDeflStartmm, dDeflEndmm, dDeflStepmm);
//	// 基板表面の傾斜（θ(=dy/dx)）
//	CalcDeflectionTheta(dDeflStartmm, dDeflEndmm, dDeflStepmm);
//	// 基板表面の傾斜から曲率半径を求めるため、1次回帰計算
//	CalcDeflectionThetaLinest(dDeflStartmm, dDeflEndmm, dDeflStepmm);
//}

// --------------------------------------------------------------------
// GetDthetaDx
double CRadiusOfCurvature::GetDthetaDx(double dPosmm)
{
//	TRACE(_T("CRadiusOfCurvature::GetDthetaDx()\n"));

	/* dθ/dxを取得します */
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

	/* Radius を求めます */
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

	/* dθ/dxを求めます */
//	m_DeflectionThetaLineStMap.RemoveAll();

// 2009.10.09 K.Matsuo セクション間でdθ/dx(=1/R)を計算 -->
//	CURVEFITTINGST cf;
//	const int DATANUM = 7;
//
//	double dCoef;
//	double dTemp;
//	double d = dThetaStartmm + dThetaStepmm;
//	while ( d + dThetaStepmm * 6 <= dThetaEndmm - dThetaStepmm ) {
//		cf.datanum = DATANUM;
//		for ( int i = 0; i < DATANUM; i++ ) {
//			cf.point[i] = d + dThetaStepmm * i;
//			if ( !m_DeflectionThetaMap.Lookup(d + dThetaStepmm * i, dTemp) )
//				ASSERT(FALSE);
//			cf.value[i] = dTemp;
//		}
//		m_pDeflectionThetaFitting->SetParamData(cf);
//		dCoef = m_pDeflectionThetaFitting->GetCoefficient(1);				// 係数aの取得（y = ax + b）
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
	dCoef = m_pDeflectionThetaFitting->GetCoefficient(1);				// 係数aの取得（y = ax + b）

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
	dIndex = dIndex * ((1.0 / m_dStageSpeed) / (10.0 / 1000.0));	// (1.0 / m_dStageSpeed) / (サンプリングインターバル10.0 / データ蓄積間隔1000.0)

	iIndex = (int)dIndex;
	ddIndex = dIndex - (double)iIndex;

	// 加重平均計算の関係で、インデックス参照が不正な位置になるので、端の１つ内側までしか利用しないようにする
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

