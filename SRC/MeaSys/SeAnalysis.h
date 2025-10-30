#pragma once

#include "DdeClient.hxx"

class CSeAnalysis
{
public:
	CSeAnalysis();
	~CSeAnalysis();

	BOOL InitInstance();
	void ExitInstance();

	BOOL DoXmp(LPTSTR pszXmpDdeText, const MEAS_PROG_INFO* pMeasProgInfo, CDdeClient* pXmpDde);
};
