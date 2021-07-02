#pragma once

#include "../../INC/Globals.hxx"
#include "../../INC/ConfigFile.hxx"
#include "RsHead.h"


class CRsMeasure
{
public:
	CRsMeasure();
	virtual ~CRsMeasure();
	virtual BOOL InitInstance();
	virtual void ExitInstance();
	virtual BOOL Measure(const MEAS_PROG_INFO* pMeasProgInfo);

private:
	BOOL CheckParams(const MEAS_PROG_INFO* pMeasProgInfo);
	BOOL SetParams(const MEAS_PROG_INFO* pMeasProgInfo);
	BOOL MoveProbeHeadLowerPos(BOOL bLowerPos = TRUE);

private:
	CRsHead* m_pRsHead;
	RS_CONFIG m_RsConfig;

	static LPCTSTR m_pszRange[];
	static LPCTSTR m_pszThickUnit[];
};
