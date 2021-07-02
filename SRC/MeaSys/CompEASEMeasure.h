#pragma once

#include <afxmt.h>
#include "../../INC/Globals.hxx"
#include "../../INC/ConfigFile.hxx"
#include "CompEASEHead.h"
#include "CompEASEPollingStatus.h"
#include "TcpAsyncPort.h"

class CCompEASEMeasure
{
public:
	CCompEASEMeasure();
	virtual ~CCompEASEMeasure();
	virtual BOOL InitInstance();
	virtual void ExitInstance();
	//2013.02.19 -->
	//virtual BOOL Measure(LPCTSTR pszSampleId, const MEAS_PROG_INFO* pMeasProgInfo, const MAIN_RCP_INFO* pMainRcpInfo);
	virtual BOOL Measure(DWORD dwPointNo, LPCTSTR pszSampleId, const MEAS_PROG_INFO* pMeasProgInfo, const MAIN_RCP_INFO* pMainRcpInfo);
	//2013.02.19 <--
	virtual BOOL ListRecipes(CStringArray* pListRecipes);

	int Status();
	BOOL OpenShutter();
	BOOL CloseShutter();

private:
	BOOL SetToAdapResultDataBase(DWORD dwPointNo, LPCTSTR pszMeasResult, const MAIN_RCP_INFO* pMainRcpInfo);
	void Split(CString strRecord, TCHAR chDel, CStringArray* pStrTsvDatas);
	int IsTargetItem(LPCTSTR pszKey);

private:
	TCHAR m_szListRecipBuff[(COMPEASE_RECIPE_NAME_LEN + 1) * 10000];

	CCompEASEHead* m_pCompEASEHead;
	CCompEASEPollingStatus* m_pCompEASEPolling;
	CTcpAsyncPort* m_pTcpAsyncPort;

	CCriticalSection m_CriticalSection;
};
