#pragma once

#include "DdeClient.hxx"

class CXmp
{
public:
	CXmp();
	~CXmp();
	BOOL InitInstance();
	void ExitInstance();
	BOOL DoXmp(LPTSTR pszXmpDdeText, LPCTSTR pszCmd, CDdeClient* pXmpDde);
/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
	BOOL ShowXmp(int iShow);
/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */

/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
private:
	BOOL m_bSC_Adap;
/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */

private:
	BOOL RunXmp();
	void ShutDownXmp();
};
