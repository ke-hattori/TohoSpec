#pragma once

#include <ddeml.h>

#define DDE_TIMEOUT 				 (10000)

static HDDEDATA CALLBACK DdeCallback(
	UINT uType,
	UINT uFmt,
	HCONV hconv,
	HSZ hsz1,
	HSZ hsz2,
	HDDEDATA hdata,
	DWORD dwData1,
	DWORD dwData2
)
{
	return (HDDEDATA)NULL;
}

class CDdeClient
{
public:
	CDdeClient() : m_dwInst(0), m_hConv(0L) {}

	virtual ~CDdeClient()
	{
		Disconnect();
		Uninit();
	}

	BOOL Init() {return DdeInitialize(&m_dwInst, DdeCallback, APPCMD_CLIENTONLY, 0) == DMLERR_NO_ERROR;}

	BOOL Uninit() {return ( !m_dwInst ) ? FALSE : DdeUninitialize(m_dwInst);}

	BOOL Connect(LPCTSTR pszService, LPCTSTR pszTopic)		// pszService, pszTopicの文字列は最大255文字まで（システム制限）
	{
		if ( !m_dwInst )
			return FALSE;
		HSZ hszService = DdeCreateStringHandle(m_dwInst, pszService, CP_WINANSI);
		HSZ hszTopic = DdeCreateStringHandle(m_dwInst, pszTopic, CP_WINANSI);
		m_hConv = DdeConnect(m_dwInst, hszService, hszTopic, NULL);
		if ( !m_hConv )
		{
			DdeFreeStringHandle(m_dwInst, hszService);
			DdeFreeStringHandle(m_dwInst, hszTopic);
			return FALSE;
		}
		DdeFreeStringHandle(m_dwInst, hszService);
		DdeFreeStringHandle(m_dwInst, hszTopic);
		return TRUE;
	}

	BOOL Disconnect() {return ( !m_hConv ) ? FALSE : DdeDisconnect(m_hConv);}

	BOOL Execute(LPCTSTR pszCmd)
	{
		if ( !m_hConv )
			return FALSE;
		HDDEDATA hRet = DdeClientTransaction((LPBYTE)pszCmd, lstrlen(pszCmd)+1, m_hConv, 0L, 0, XTYP_EXECUTE, DDE_TIMEOUT, NULL);
		if ( !hRet )
			return FALSE;
		if ( DdeGetLastError(m_dwInst) != DMLERR_NO_ERROR )
		{
			DdeFreeDataHandle(hRet);
			return FALSE;
		}
		DdeFreeDataHandle(hRet);
		return TRUE;
	}

	BOOL DataCopy(LPTSTR pszData, LPCTSTR pszItem)			// pszDataが指すバッファは4096バイト以上確保すること。pszItemの文字列は最大255文字まで（システム制限）
	{
		if ( !m_hConv )
			return FALSE;
		TCHAR szBuff[4096];
		HSZ hszItem = DdeCreateStringHandle(m_dwInst, pszItem, CP_WINANSI);
		HDDEDATA hRet = DdeClientTransaction(NULL, 0, m_hConv, hszItem, CF_TEXT, XTYP_REQUEST, DDE_TIMEOUT, NULL);
		if ( !hRet )
		{
			DdeFreeStringHandle(m_dwInst, hszItem);
			return FALSE;
		}
		if ( DdeGetLastError(m_dwInst) != DMLERR_NO_ERROR )
		{
			DdeFreeStringHandle(m_dwInst, hszItem);
			DdeFreeDataHandle(hRet);
			return FALSE;
		}
		DdeGetData(hRet, (LPBYTE)szBuff, sizeof(szBuff), 0);
		DdeFreeStringHandle(m_dwInst, hszItem);
		DdeFreeDataHandle(hRet);
		lstrcpy(pszData, szBuff);
		return TRUE;
	}

private:
	DWORD m_dwInst;
	HCONV m_hConv;
};
