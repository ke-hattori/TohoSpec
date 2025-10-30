#pragma once

#include "../../INC/Globals.hxx"

class CScanRange
{
public:
	CScanRange(const MEAS_PROG_INFO* pMeasProgInfo);
	virtual ~CScanRange();

	enum { MIN_UV_SCAN	= 200,
		   MIN_VS_SCANL = 380,
		   MAX_CS_SCAN	= 800,
		   MAX_SS_SCAN	= 850,
		   CIE_MINWAVE	= 380,	// CIE/ODílç≈è¨îgí∑
		   CIE_MAXWAVE	= 780 };

	int GetScanStartRange() const {return m_iScanStart;}
	int GetScanEndRange() const {return m_iScanEnd;}
	BOOL IsUvMeasure() const {return m_bUvMeasure;}
	BOOL IsVisMeasure() const {return m_bVisMeasure;}

protected:
	BOOL IsDiscrete(WORD wScanType);

	int m_iScanStart;
	int m_iScanEnd;
	BOOL m_bUvMeasure;
	BOOL m_bVisMeasure;
};

