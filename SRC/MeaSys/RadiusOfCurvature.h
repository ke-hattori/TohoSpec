#pragma once

#include "CurveFitting.hpp"

class CRadiusOfCurvature
{
public:
	CRadiusOfCurvature();
	~CRadiusOfCurvature();

	void SetStageSpeed(double dStageSpeed);
	void SetDeflection(const double* pDeflection, int nOccurence);
	void SetDeflectionAtZero(double** pDeflection, int nOccurence);
//	void Calc(double dDeflStartmm, double dDeflEndmm, double dDeflStepmm);
	double GetDthetaDx(double dPosmm);
	double GetRadius(double dPosmm);

//	void DeflectionStartAndEndAtZero(double dSectionStartX, double dSectionEndX, double dDeflStepmm, double dLineStartX, double dLineEndX, BOOL bFirstSection, BOOL bLastSection);

	void CalcDeflectionData(double dDeflStartmm, double dDeflEndmm, double dDeflStepmm);
	void CalcDeflectionStartAndEndAtZero(double dSectionStartX, double dSectionEndX, double dDeflStepmm, double dLineStartX, double dLineEndX, BOOL bFirstSection, BOOL bLastSection);

//private:
	void CalcDeflectionTheta(double dThetaStartmm, double dThetaEndmm, double dThetaStepmm);
	void CalcDeflectionThetaLinest(double dThetaStartmm, double dThetaEndmm, double dThetaStepmm);
private:
	double GetDeflectionByPos(double dPosmm, double dOffsetmm);

private:
	double m_dStageSpeed;
	CCurveFitting* m_pDeflectionThetaFitting;

public:
	CArray<double, const double&> m_DeflectionArray;
	CMap<double, const double&, double, const double&> m_DeflectionData;
	CMap<double, const double&, double, const double&> m_DeflectionAtZeroData;
	CMap<double, const double&, double, const double&> m_DeflectionThetaMap;
	CMap<double, const double&, double, const double&> m_DeflectionThetaLineStMap;
};
