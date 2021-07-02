#pragma once

#include "PifMessageBehavior.h"

#define AddP9xxMessage							AddRespMessage

#define GETSETITEMINT(idx, dvname)																\
	int GetItem##dvname() const 																\
	{																							\
		TCHAR szDvValue[DVVALUELEN + 1];														\
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + FORMATIDLEN + POINTNOLEN;				\
		nPos += (VARTYPELEN + DVNAMELEN + DVVALUELEN) * idx;									\
		if ( m_pszRequMessage[nPos] != _TCHAR('I') )											\
			ASSERT( FALSE );																	\
		if ( _tcsncmp(&m_pszRequMessage[nPos + VARTYPELEN], #dvname, _tcslen(#dvname)) != 0 )	\
			ASSERT( FALSE );																	\
		_tcsncpy(szDvValue, &m_pszRequMessage[nPos + VARTYPELEN + DVNAMELEN], DVVALUELEN);		\
		szDvValue[DVVALUELEN] = _TCHAR('\0');													\
		return _ttoi(szDvValue);																\
	}																							\
																								\
	void SetItem##dvname(int i##dvname) 														\
	{																							\
		int iLen;																				\
		stProcessDataSet[idx].chVarType = _TCHAR('I');											\
		_tcscpy(stProcessDataSet[idx].szDvName, #dvname);										\
		iLen = _tcslen(stProcessDataSet[idx].szDvName); 										\
		::FillMemory(&stProcessDataSet[idx].szDvName[iLen], DVNAMELEN - iLen,_TCHAR(' '));		\
		stProcessDataSet[idx].szDvName[DVNAMELEN] = '\0';										\
		_stprintf(stProcessDataSet[idx].szDvValue, _T("%d"), i##dvname);						\
		iLen = _tcslen(stProcessDataSet[idx].szDvValue);										\
		::FillMemory(&stProcessDataSet[idx].szDvValue[iLen], DVVALUELEN - iLen,_TCHAR(' '));	\
		stProcessDataSet[idx].szDvValue[DVVALUELEN] = '\0'; 									\
	}

#define GETSETITEMASCII(idx, dvname)															\
	void GetItem##dvname(LPTSTR psz##dvname) const												\
	{																							\
		TCHAR szDvValue[DVVALUELEN + 1];														\
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + FORMATIDLEN + POINTNOLEN;				\
		nPos += (VARTYPELEN + DVNAMELEN + DVVALUELEN) * idx;									\
		if ( m_pszRequMessage[nPos] != _TCHAR('A') )											\
			ASSERT( FALSE );																	\
		if ( _tcsncmp(&m_pszRequMessage[nPos + VARTYPELEN], #dvname, _tcslen(#dvname)) != 0 )	\
			ASSERT( FALSE );																	\
		_tcsncpy(szDvValue, &m_pszRequMessage[nPos + VARTYPELEN + DVNAMELEN], DVVALUELEN);		\
		szDvValue[DVVALUELEN] = _TCHAR('\0');													\
		_tcscpy(psz##dvname, szDvValue);														\
	}																							\
																								\
	void SetItem##dvname(LPCTSTR psz##dvname)													\
	{																							\
		int iLen;																				\
		stProcessDataSet[idx].chVarType = _TCHAR('A');											\
		_tcscpy(stProcessDataSet[idx].szDvName, #dvname);										\
		iLen = _tcslen(stProcessDataSet[idx].szDvName); 										\
		::FillMemory(&stProcessDataSet[idx].szDvName[iLen], DVNAMELEN - iLen,_TCHAR(' '));		\
		stProcessDataSet[idx].szDvName[DVNAMELEN] = '\0';										\
		_tcscpy(stProcessDataSet[idx].szDvValue, psz##dvname);									\
		iLen = _tcslen(stProcessDataSet[idx].szDvValue);										\
		::FillMemory(&stProcessDataSet[idx].szDvValue[iLen], DVVALUELEN - iLen,_TCHAR(' '));	\
		stProcessDataSet[idx].szDvValue[DVVALUELEN] = '\0'; 									\
	}

#define GETSETITEMDOUBLE(idx, dvname)															\
	double GetItem##dvname() const																\
	{																							\
		TCHAR szDvValue[DVVALUELEN + 1];														\
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + FORMATIDLEN + POINTNOLEN;				\
		nPos += (VARTYPELEN + DVNAMELEN + DVVALUELEN) * idx;									\
		if ( m_pszRequMessage[nPos] != _TCHAR('D') )											\
			ASSERT( FALSE );																	\
		if ( _tcsncmp(&m_pszRequMessage[nPos + VARTYPELEN], #dvname, _tcslen(#dvname)) != 0 )	\
			ASSERT( FALSE );																	\
		_tcsncpy(szDvValue, &m_pszRequMessage[nPos + VARTYPELEN + DVNAMELEN], DVVALUELEN);		\
		szDvValue[DVVALUELEN] = _TCHAR('\0');													\
		return atof(szDvValue); 																\
	}																							\
																								\
	void SetItem##dvname(double d##dvname)														\
	{																							\
		int iLen;																				\
		stProcessDataSet[idx].chVarType = _TCHAR('D');											\
		_tcscpy(stProcessDataSet[idx].szDvName, #dvname);										\
		iLen = _tcslen(stProcessDataSet[idx].szDvName); 										\
		::FillMemory(&stProcessDataSet[idx].szDvName[iLen], DVNAMELEN - iLen,_TCHAR(' '));		\
		stProcessDataSet[idx].szDvName[DVNAMELEN] = '\0';										\
		d##dvname = d##dvname <  99999999.999999 ? d##dvname : 99999999.999999; 				\
		d##dvname = d##dvname < -99999999.999999 ? -99999999.999999 : d##dvname;				\
		_stprintf(stProcessDataSet[idx].szDvValue, _T("%lf"), d##dvname);						\
		iLen = _tcslen(stProcessDataSet[idx].szDvValue);										\
		::FillMemory(&stProcessDataSet[idx].szDvValue[iLen], DVVALUELEN - iLen,_TCHAR(' '));	\
		stProcessDataSet[idx].szDvValue[DVVALUELEN] = '\0'; 									\
	}

//---------------------------------------------------------------------------
// CPifMessage
class CPifMessage
{
public:
	enum { ENQLEN		   = 01,
		   MESSAGELENLEN   = 05,
		   COMMANDLEN	   = 04,
		   CRLEN		   = 01 };

	enum { ACKLEN		   = 02 };

	enum { MESSAGEMAXLEN   = 16*16*16*16*16 };

	static TCHAR ENQ;
	static TCHAR CR;

	CPifMessage() : m_pszRequMessage(NULL) {}

	virtual ~CPifMessage()
	{
		if ( m_pszRequMessage ) {
			delete [] m_pszRequMessage;
			m_pszRequMessage = NULL;
		}
	}

// Recv
	void Recv(LPCTSTR pszMessage, size_t count)
	{
		m_iRecvLength = count;
		m_pszRequMessage = new TCHAR[count + 1];

		_tcscpy(m_pszRequMessage, pszMessage);
		_tcsncpy(m_szCommandId, &m_pszRequMessage[ENQLEN + MESSAGELENLEN], COMMANDLEN);
		m_szCommandId[COMMANDLEN] = _TCHAR('\0');
	}

	LPCTSTR GetItemRecvCommandId() const {return m_szCommandId;}

	int GetItemRecvAck()
	{
		_tcsncpy(m_szAckCode, &m_pszRequMessage[ENQLEN + MESSAGELENLEN + COMMANDLEN], ACKLEN);
		m_szAckCode[ACKLEN] = _TCHAR('\0');
		return _ttoi(m_szAckCode);
	}

	LPCTSTR GetRecvMessagePtr() const {return m_pszRequMessage;}

// Resp Ack
	void SetItemAckCode(int iCode)
	{
		_stprintf(m_szAckCode, _T("%02d"), iCode);
	}

	void SetItemAckCode(LPCTSTR psz)
	{
		_tcscpy(m_szAckCode, psz);
	}

// SendResp
	void SendResp() 																	// Sxxx
	{
		_stprintf(m_szRespMessage, _T("%c%05X%c%s%s%c"), ENQ, COMMANDLEN + ACKLEN, _TCHAR('S'), &m_szCommandId[1], m_szAckCode, CR);
		AddRespMessage();
	}

protected:
// SendRequ
	void SendRequ(LPCTSTR pszCommandId, LPCTSTR pszMsgBody = NULL, size_t count = 0)	// Pxxx
	{
		m_pszRequMessage = new TCHAR[ENQLEN + MESSAGELENLEN + COMMANDLEN + count + CRLEN + 1];
		if ( !pszMsgBody )
			_stprintf(m_pszRequMessage, _T("%c%05X%s%c"), ENQ, COMMANDLEN, pszCommandId, CR);
		else
			_stprintf(m_pszRequMessage, _T("%c%05X%s%s%c"), ENQ, COMMANDLEN + count, pszCommandId, pszMsgBody, CR);
		AddRequMessage();
		if ( m_pszRequMessage ) {
			delete [] m_pszRequMessage;
			m_pszRequMessage = NULL;
		}
	}

	void AddRequMessage() const
	{
		CPifMessageBehavior::AddRequMessage(m_pszRequMessage);
	}

	void AddRespMessage() const
	{
		CPifMessageBehavior::AddRespMessage(m_szRespMessage);
	}

protected:
	TCHAR* m_pszRequMessage;
	TCHAR m_szRespMessage[ENQLEN + MESSAGELENLEN + COMMANDLEN + ACKLEN + CRLEN + 1];
	TCHAR m_szCommandId[COMMANDLEN + 1];
	TCHAR m_szAckCode[ACKLEN + 1];
	int m_iRecvLength;
};


//---------------------------------------------------------------------------
// CPifMessage101
class CPifMessage101 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P101"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage102
class CPifMessage102 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P102"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage103
class CPifMessage103 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P103"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage104
class CPifMessage104 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P104"));
	}
};

// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
//---------------------------------------------------------------------------
// CPifMessage105
class CPifMessage105 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P105"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage106
class CPifMessage106 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P106"));
	}
};
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--

//---------------------------------------------------------------------------
// CPifMessage201 時刻合わせ要求
class CPifMessage201 : public CPifMessage
{
public:
	enum { YYYYLEN	= 04,
		   MMLEN	= 02,
		   DDLEN	= 02,
		   HHLEN	= 02,
		   MILEN	= 02,
		   SSLEN	= 02,
		   CCCLEN	= 03 };

	void SetItemDateTime(LPCTSTR pszDateTime)
	{
		_tcscpy(m_szSystemTime, pszDateTime);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P201"), m_szSystemTime, _tcslen(m_szSystemTime));
	}

	void GetItemDateTime(SYSTEMTIME* lpSystemTime) const
	{
		TCHAR szYear[]		   = _T("yyyy");
		TCHAR szMonth[] 	   = _T("mm");
		TCHAR szDay[]		   = _T("dd");
		TCHAR szHour[]		   = _T("hh");
		TCHAR szMinute[]	   = _T("mm");
		TCHAR szSecond[]	   = _T("ss");
		TCHAR szMilliSeconds[] = _T("ccc");

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szYear,		 &m_pszRequMessage[nPos], YYYYLEN);   nPos += YYYYLEN;
		_tcsncpy(szMonth,		 &m_pszRequMessage[nPos], MMLEN);	  nPos += MMLEN;
		_tcsncpy(szDay, 		 &m_pszRequMessage[nPos], DDLEN);	  nPos += DDLEN;
		_tcsncpy(szHour,		 &m_pszRequMessage[nPos], HHLEN);	  nPos += HHLEN;
		_tcsncpy(szMinute,		 &m_pszRequMessage[nPos], MILEN);	  nPos += MILEN;
		_tcsncpy(szSecond,		 &m_pszRequMessage[nPos], SSLEN);	  nPos += SSLEN;
		_tcsncpy(szMilliSeconds, &m_pszRequMessage[nPos], CCCLEN);

		::ZeroMemory(lpSystemTime, sizeof(SYSTEMTIME));
		lpSystemTime->wYear 		= _ttoi(szYear);
		lpSystemTime->wMonth		= _ttoi(szMonth);
		lpSystemTime->wDay			= _ttoi(szDay);
		lpSystemTime->wHour 		= _ttoi(szHour);
		lpSystemTime->wMinute		= _ttoi(szMinute);
		lpSystemTime->wSecond		= _ttoi(szSecond);
		lpSystemTime->wMilliseconds = _ttoi(szMilliSeconds);
	}

private:
	TCHAR m_szSystemTime[256];
};

//---------------------------------------------------------------------------
// CPifMessage301 装置モニター情報報告
class CPifMessage301 : public CPifMessage
{
public:
	enum { LOCALREMOTELEN		= 1,
		   EQPROCESSSTATUSLEN	= 1,
		   ALARMSSTATUSLEN		= 1,
		   GLASSEXISTLEN		= 1,
		   LOADPOSITIONLEN		= 1,
		   VACCUM1LEN			= 1,
		   SHUTTEROPENLEN		= 1,
		   SHUTTERCLOSELEN		= 1,
		   PINUPLEN 			= 1,
		   PINDOWNLEN			= 1,
		   ARMSENSORLEN 		= 1,
		   DOORINTERLOCK		= 1,
		   FILLER				= 8 };

	void SetItemLocalRemote(int iLocalRemote) {m_iLocalRemote = iLocalRemote;}

	void SetItemEqProcessStatus(int iEqProcessStatus) {m_iEqProcessStatus = iEqProcessStatus;}

	void SetItemAlarmStatus(int iAlarmStatus) {m_iAlarmStatus = iAlarmStatus;}

	void SetItemGlassExist(int iGlassExist) {m_iGlassExist = iGlassExist;}

	void SetItemLoadPosition(int iLoadPos) {m_iLoadPos = iLoadPos;}

	void SetItemVaccum1(int iVaccum1) {m_iVaccum1 = iVaccum1;}

	void SetItemShutterOpen(int iShutterOpen) {m_iShutterOpen = iShutterOpen;}

	void SetItemShutterClose(int iShutterClose) {m_iShutterClose = iShutterClose;}

	void SetItemPinUp(int iPinUp) {m_iPinUp = iPinUp;}

	void SetItemPinDown(int iPinDown) {m_iPinDown = iPinDown;}

	void SetItemArmSensor(int iArmSensor) {m_iArmSensor = iArmSensor;}

	void SetItemDoorInterlock(int iDoorInterlock) {m_iDoorInterlock = iDoorInterlock;}

	void SendRequ()
	{
		TCHAR szBuff[256];
		_stprintf(szBuff, _T("%d%d%d%d%d%d%d%d%d%d%d%d********"), m_iLocalRemote, m_iEqProcessStatus, m_iAlarmStatus, m_iGlassExist, m_iLoadPos, m_iVaccum1, m_iShutterOpen, m_iShutterClose, m_iPinUp, m_iPinDown, m_iArmSensor, m_iDoorInterlock);
		CPifMessage::SendRequ(_T("P301"), szBuff, _tcslen(szBuff));
	}

	int GetItemLocalRemote() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 0;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemEqProcessStatus() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 1;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemAlarmStatus() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 2;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemGlassExist() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 3;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemLoadPosition() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 4;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemVaccum1() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 5;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemShutterOpen() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 6;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemShutterClose() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 7;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemPinUp() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 8;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemPinDown() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 9;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemArmSensor() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 10;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

	int GetItemDoorInterlock() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + 11;
		return (int)(m_pszRequMessage[nPos] - _TCHAR('0'));
	}

private:
	int m_iLocalRemote;
	int m_iEqProcessStatus;
	int m_iAlarmStatus;
	int m_iGlassExist;
	int m_iLoadPos;
	int m_iVaccum1;
	int m_iShutterOpen;
	int m_iShutterClose;
	int m_iPinUp;
	int m_iPinDown;
	int m_iArmSensor;
	int m_iDoorInterlock;
};

//---------------------------------------------------------------------------
// CPifMessage302 装置モニター情報取得要求
class CPifMessage302 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P302"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage303 レシピ一覧取得要求
class CPifMessage303 : public CPifMessage
{
public:
	enum { KINDOFRECIPELEN = 02 };

	void SetItemKindOfRecipe(int iKindOfRecipe)
	{
		_stprintf(m_szKindOfRecipe, _T("%02d"), iKindOfRecipe);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P303"), m_szKindOfRecipe, _tcslen(m_szKindOfRecipe));
	}

	int GetItemKindOfRecipe() const
	{
		TCHAR szBuff[KINDOFRECIPELEN + 1];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], KINDOFRECIPELEN);
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szKindOfRecipe[KINDOFRECIPELEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage304 レシピ一覧報告
class CPifMessage304 : public CPifMessage
{
public:
	enum { KINDOFRECIPELEN	= 02,
		   RECIPENAMEEXTLEN = 28,
		   DATETIMELEN		= 14 };

	void SetItemKindOfRecipe(int iKindOfRecipe)
	{
		_stprintf(m_szKindOfRecipe, _T("%02d"), iKindOfRecipe);
	}

	void SetItemRecipe(LPCTSTR pszRecipeName, const SYSTEMTIME* lpSystemTime)
	{
		char szBuff[RECIPENAMEEXTLEN + 1];
		sprintf(szBuff, "%-28.28s", pszRecipeName);
		m_strRecipeNameList.AddTail(szBuff);

		sprintf(szBuff, _T("%04d%02d%02d%02d%02d%02d"), lpSystemTime->wYear, lpSystemTime->wMonth, lpSystemTime->wDay,
														lpSystemTime->wHour, lpSystemTime->wMinute, lpSystemTime->wSecond);
		m_strRecipeDateTimeList.AddTail(szBuff);
	}

	void SendRequ()
	{
		int count;
		count  = KINDOFRECIPELEN;
		count += m_strRecipeNameList.GetCount() * RECIPENAMEEXTLEN;
		count += m_strRecipeDateTimeList.GetCount() * DATETIMELEN;

		TCHAR* pszBuff = new TCHAR[count + 1];
		_tcscpy(pszBuff, m_szKindOfRecipe);
		POSITION pos1;
		POSITION pos2;
		pos1 = m_strRecipeNameList.GetHeadPosition();
		pos2 = m_strRecipeDateTimeList.GetHeadPosition();
		while ( pos1 && pos2 ) {
			_tcscat(pszBuff, m_strRecipeNameList.GetNext(pos1));
			_tcscat(pszBuff, m_strRecipeDateTimeList.GetNext(pos2));
		}

		CPifMessage::SendRequ(_T("P304"), pszBuff, count);

		delete [] pszBuff;
		pszBuff = NULL;
	}

	int GetItemKindOfRecipe() const
	{
		TCHAR szBuff[KINDOFRECIPELEN + 1];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], KINDOFRECIPELEN);
		return _ttoi(szBuff);
	}

	LPCTSTR GetItemRecipeListPtr() const	// recipename + datetime を一組として配列＋終端文字CRの文字列の先頭ポインタを返す ex. "recipe...datetime...recipe...datetime...[CR]"
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + KINDOFRECIPELEN;
		return &m_pszRequMessage[nPos];
	}

private:
	TCHAR m_szKindOfRecipe[KINDOFRECIPELEN + 1];
	CStringList m_strRecipeNameList;
	CStringList m_strRecipeDateTimeList;
};

//---------------------------------------------------------------------------
// CPifMessage305 レシピ変更報告
class CPifMessage305 : public CPifMessage
{
public:
	enum { KINDOFRECIPELEN	   = 02,
		   RECIPECHANGETYPELEN = 01,
		   RECIPENAMEEXTLEN    = 28,
		   DATETIMELEN		   = 14 };

	void SetItemKindOfRecipe(int iKindOfRecipe)
	{
		_stprintf(m_szKindOfRecipe, _T("%02d"), iKindOfRecipe);
	}

	void SetItemRecipe(TCHAR chRecipeChangeType, LPCTSTR pszRecipeName, const SYSTEMTIME* lpSystemTime)
	{
		m_chRecipeChangeType = chRecipeChangeType;
		sprintf(m_szRecipeName, "%-28.28s", pszRecipeName);
		_stprintf(m_szRecipeDateTime, _T("%04d%02d%02d%02d%02d%02d"), lpSystemTime->wYear, lpSystemTime->wMonth, lpSystemTime->wDay,
																	  lpSystemTime->wHour, lpSystemTime->wMinute, lpSystemTime->wSecond);

	}
	void SendRequ()
	{
		TCHAR szBuff[256];
		_stprintf(szBuff, _T("%s%c%s%s"), m_szKindOfRecipe, m_chRecipeChangeType, m_szRecipeName, m_szRecipeDateTime);
		CPifMessage::SendRequ(_T("P305"), szBuff, _tcslen(szBuff));
	}

	int GetItemKindOfRecipe() const
	{
		TCHAR szBuff[KINDOFRECIPELEN + 1];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], KINDOFRECIPELEN);
		return _ttoi(szBuff);
	}

	TCHAR GetItemRecipeChangeType() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + KINDOFRECIPELEN;
		return m_pszRequMessage[nPos];
	}

	void GetItemRecipeName(LPTSTR pszRecipeName, LPTSTR pszRecipeDateTime) const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + KINDOFRECIPELEN + RECIPECHANGETYPELEN;
		_tcsncpy(pszRecipeName, &m_pszRequMessage[nPos], RECIPENAMEEXTLEN);
		pszRecipeName[RECIPENAMEEXTLEN] = _TCHAR('\0');
		nPos += RECIPENAMEEXTLEN;
		_tcsncpy(pszRecipeDateTime, &m_pszRequMessage[nPos], DATETIMELEN);
		pszRecipeDateTime[DATETIMELEN] = _TCHAR('\0');
	}

private:
	TCHAR m_szKindOfRecipe[KINDOFRECIPELEN + 1];
	TCHAR m_chRecipeChangeType;
	TCHAR m_szRecipeName[RECIPENAMEEXTLEN + 1];
	TCHAR m_szRecipeDateTime[DATETIMELEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage306 トレースデータ取得要求
class CPifMessage306 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P306"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage307 トレースデータ報告
class CPifMessage307 : public CPifMessage
{
public:
	enum { DATETIMELEN		= 14,
		   DATETIMESPANLEN	= 14, };	// レングス数は、DATETIMELEN == DATETIMESPANLENにしておく（オフセット位置算出が面倒なので。）

	void SetItemSrVisReflectLampLastExchange(const SYSTEMTIME* pSystemTime)
	{
		SystemTimeToString(m_szSrVisReflectLampLastExchange, pSystemTime);
	}

	void SetItemSrVisReflectLampElapse(long lSecs)
	{
		sprintf(m_szSrVisReflectLampElapse, "%14ld", lSecs);
	}

	void SetItemSrVisReflectLampLifeTime(long lSecs)
	{
		sprintf(m_szSrVisReflectLampLifeTime, "%14ld", lSecs);
	}

	void SetItemSrVisTransmitLampLastExchange(const SYSTEMTIME* pSystemTime)
	{
		SystemTimeToString(m_szSrVisTransmitLampLastExchange, pSystemTime);
	}

	void SetItemSrVisTransmitLampElapse(long lSecs)
	{
		sprintf(m_szSrVisTransmitLampElapse, "%14ld", lSecs);
	}

	void SetItemSrVisTransmitLampLifeTime(long lSecs)
	{
		sprintf(m_szSrVisTransmitLampLifeTime, "%14ld", lSecs);
	}

	void SetItemSrUvLampLastExchange(const SYSTEMTIME* pSystemTime)
	{
		SystemTimeToString(m_szSrUvLampLastExchange, pSystemTime);
	}

	void SetItemSrUvLampElapse(long lSecs)
	{
		sprintf(m_szSrUvLampElapse, "%14ld", lSecs);
	}

	void SetItemSrUvLampLifeTime(long lSecs)
	{
		sprintf(m_szSrUvLampLifeTime, "%14ld", lSecs);
	}

	void SetItemSeVisReflectLampLastExchange(const SYSTEMTIME* pSystemTime)
	{
		SystemTimeToString(m_szSeVisReflectLampLastExchange, pSystemTime);
	}

	void SetItemSeVisReflectLampElapse(long lSecs)
	{
		sprintf(m_szSeVisReflectLampElapse, "%14ld", lSecs);
	}

	void SetItemSeVisReflectLampLifeTime(long lSecs)
	{
		sprintf(m_szSeVisReflectLampLifeTime, "%14ld", lSecs);
	}

	void SetItemSeUvReflectLampLastExchange(const SYSTEMTIME* pSystemTime)
	{
		SystemTimeToString(m_szSeUvLampLastExchange, pSystemTime);
	}

	void SetItemSeUvReflectLampElapse(long lSecs)
	{
		sprintf(m_szSeUvLampElapse, "%14ld", lSecs);
	}

	void SetItemSeUvReflectLampLifeTime(long lSecs)
	{
		sprintf(m_szSeUvLampLifeTime, "%14ld", lSecs);
	}

	void GetItemSrVisReflectLampLastExchange(SYSTEMTIME* pSystemTime)
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 0);
		StringToSystemTime(pSystemTime, &m_pszRequMessage[nPos]);
	}

	long GetItemSrVisReflectLampElapse()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 1);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	long GetItemSrVisReflectLampLifeTime()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 2);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	void GetItemSrVisTransmitLampLastExchange(SYSTEMTIME* pSystemTime)
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 3);
		StringToSystemTime(pSystemTime, &m_pszRequMessage[nPos]);
	}

	long GetItemSrVisTransmitLampElapse()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 4);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	long GetItemSrVisTransmitLampLifeTime()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 5);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	void GetItemSrUvLampLastExchange(SYSTEMTIME* pSystemTime)
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 6);
		StringToSystemTime(pSystemTime, &m_pszRequMessage[nPos]);
	}

	long GetItemSrUvLampElapse()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 7);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	long GetItemSrUvLampLifeTime()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 8);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	void GetItemSeVisReflectLampLastExchange(SYSTEMTIME* pSystemTime)
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 9);
		StringToSystemTime(pSystemTime, &m_pszRequMessage[nPos]);
	}

	long GetItemSeVisReflectLampElapse()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 10);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	long GetItemSeVisReflectLampLifeTime()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 11);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	void GetItemSeUvReflectLampLastExchange(SYSTEMTIME* pSystemTime)
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 12);
		StringToSystemTime(pSystemTime, &m_pszRequMessage[nPos]);
	}

	long GetItemSeUvReflectLampElapse()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 13);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	long GetItemSeUvReflectLampLifeTime()
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + (DATETIMELEN * 14);
		return StringToLong(&m_pszRequMessage[nPos]);
	}

	void SendRequ()
	{
		TCHAR szBuff[256];
		_stprintf(szBuff, _T("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s"),
						  m_szSrVisReflectLampLastExchange, m_szSrVisReflectLampElapse, m_szSrVisReflectLampLifeTime,
						  m_szSrVisTransmitLampLastExchange, m_szSrVisTransmitLampElapse, m_szSrVisTransmitLampLifeTime,
						  m_szSrUvLampLastExchange, m_szSrUvLampElapse, m_szSrUvLampLifeTime,
						  m_szSeVisReflectLampLastExchange, m_szSeVisReflectLampElapse, m_szSeVisReflectLampLifeTime,
						  m_szSeUvLampLastExchange, m_szSeUvLampElapse, m_szSeUvLampLifeTime);
		CPifMessage::SendRequ(_T("P307"), szBuff, _tcslen(szBuff));
	}

private:
	void SystemTimeToString(LPTSTR pszSystemTime, const SYSTEMTIME* pSystemTime)
	{
		sprintf(pszSystemTime, _T("%04d%02d%02d%02d%02d%02d"),
				pSystemTime->wYear,
				pSystemTime->wMonth,
				pSystemTime->wDay,
				pSystemTime->wHour,
				pSystemTime->wMinute,
				pSystemTime->wSecond);
	}

	void StringToSystemTime(SYSTEMTIME* pSystemTime, LPCTSTR pszSystemTime)
	{
		enum { YYYYLEN	= 04,
			   MMLEN	= 02,
			   DDLEN	= 02,
			   HHLEN	= 02,
			   MILEN	= 02,
			   SSLEN	= 02,};

		TCHAR szYear[]			= _T("yyyy");
		TCHAR szMonth[] 		= _T("mm");
		TCHAR szDay[]			= _T("dd");
		TCHAR szHour[]			= _T("hh");
		TCHAR szMinute[]		= _T("mm");
		TCHAR szSecond[]		= _T("ss");

		int nPos = 0;
		_tcsncpy(szYear,		&pszSystemTime[nPos], YYYYLEN);   nPos += YYYYLEN;
		_tcsncpy(szMonth,		&pszSystemTime[nPos], MMLEN);	  nPos += MMLEN;
		_tcsncpy(szDay, 		&pszSystemTime[nPos], DDLEN);	  nPos += DDLEN;
		_tcsncpy(szHour,		&pszSystemTime[nPos], HHLEN);	  nPos += HHLEN;
		_tcsncpy(szMinute,		&pszSystemTime[nPos], MILEN);	  nPos += MILEN;
		_tcsncpy(szSecond,		&pszSystemTime[nPos], SSLEN);

		::ZeroMemory(pSystemTime, sizeof(SYSTEMTIME));
		pSystemTime->wYear		= _ttoi(szYear);
		pSystemTime->wMonth 	= _ttoi(szMonth);
		pSystemTime->wDay		= _ttoi(szDay);
		pSystemTime->wHour		= _ttoi(szHour);
		pSystemTime->wMinute	= _ttoi(szMinute);
		pSystemTime->wSecond	= _ttoi(szSecond);
	}

	long StringToLong(LPCTSTR psz)
	{
		TCHAR szBuff[DATETIMESPANLEN + 1];

		strncpy(szBuff, psz, DATETIMESPANLEN);
		szBuff[DATETIMESPANLEN] = '\0';

		return atol(szBuff);
	}

private:
	TCHAR m_szSrVisReflectLampLastExchange[DATETIMELEN + 1];
	TCHAR m_szSrVisReflectLampElapse[DATETIMESPANLEN + 1];
	TCHAR m_szSrVisReflectLampLifeTime[DATETIMESPANLEN + 1];
	TCHAR m_szSrVisTransmitLampLastExchange[DATETIMELEN + 1];
	TCHAR m_szSrVisTransmitLampElapse[DATETIMESPANLEN + 1];
	TCHAR m_szSrVisTransmitLampLifeTime[DATETIMESPANLEN + 1];
	TCHAR m_szSrUvLampLastExchange[DATETIMELEN + 1];
	TCHAR m_szSrUvLampElapse[DATETIMESPANLEN + 1];
	TCHAR m_szSrUvLampLifeTime[DATETIMESPANLEN + 1];
	TCHAR m_szSeVisReflectLampLastExchange[DATETIMELEN + 1];
	TCHAR m_szSeVisReflectLampElapse[DATETIMESPANLEN + 1];
	TCHAR m_szSeVisReflectLampLifeTime[DATETIMESPANLEN + 1];
	TCHAR m_szSeUvLampLastExchange[DATETIMELEN + 1];
	TCHAR m_szSeUvLampElapse[DATETIMESPANLEN + 1];
	TCHAR m_szSeUvLampLifeTime[DATETIMESPANLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage308 サンプルサイズデータ取得要求
class CPifMessage308 : public CPifMessage
{
public:
	enum { RECIPENAMELEN   = 24 };

	void SetItemRecipeName(LPCTSTR pszRecipeName)
	{
		sprintf(m_szRecipeName, "%-24.24s", pszRecipeName);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P308"), m_szRecipeName, _tcslen(m_szRecipeName));
	}

	void GetItemRecipeName(LPTSTR pszRecipeName) const
	{
		char szRecipeName[_MAX_FNAME];
		CString strRecipeName;

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szRecipeName, &m_pszRequMessage[nPos], RECIPENAMELEN);
		szRecipeName[RECIPENAMELEN] = _TCHAR('\0');

		strRecipeName = szRecipeName;
		strRecipeName.TrimRight();
		strcpy(pszRecipeName, strRecipeName);
	}

private:
	TCHAR m_szRecipeName[RECIPENAMELEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage309 サンプルサイズデータ報告
class CPifMessage309 : public CPifMessage
{
public:
	enum { SAMPLESIZEXLEN = 16,
		   SAMPLESIZEYLEN = 16 };

	void SetItemSampleSizeX(double dX)
	{
		char szBuff[256];
		sprintf(szBuff, _T("%.3lf"), dX);
		sprintf(m_szSampleSizeX, "%-16.16s", szBuff);
	}

	void SetItemSampleSizeY(double dY)
	{
		char szBuff[256];
		sprintf(szBuff, _T("%.3lf"), dY);
		sprintf(m_szSampleSizeY, "%-16.16s", szBuff);
	}

	void SendRequ()
	{
		TCHAR szBuff[256];
		_stprintf(szBuff, _T("%s%s"), m_szSampleSizeX, m_szSampleSizeY);
		CPifMessage::SendRequ(_T("P309"), szBuff, _tcslen(szBuff));
	}

	double GetItemSampleSizeX() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], SAMPLESIZEXLEN);
		szBuff[SAMPLESIZEXLEN] = _TCHAR('\0');
		return atof(szBuff);
	}

	double GetItemSampleSizeY() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + SAMPLESIZEXLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], SAMPLESIZEYLEN);
		szBuff[SAMPLESIZEYLEN] = _TCHAR('\0');
		return atof(szBuff);
	}

private:
	TCHAR m_szSampleSizeX[SAMPLESIZEXLEN + 1];
	TCHAR m_szSampleSizeY[SAMPLESIZEYLEN + 1];
};

// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
//---------------------------------------------------------------------------
// CPifMessage310 リファレンスデータ確認要求
class CPifMessage310 : public CPifMessage
{
public:
	enum { RECIPENAMELEN   = 24 };

	void SetItemRecipeName(LPCTSTR pszRecipeName)
	{
		sprintf(m_szRecipeName, "%-24.24s", pszRecipeName);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P310"), m_szRecipeName, _tcslen(m_szRecipeName));
	}

	void GetItemRecipeName(LPTSTR pszRecipeName) const
	{
		char szRecipeName[_MAX_FNAME];
		CString strRecipeName;

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szRecipeName, &m_pszRequMessage[nPos], RECIPENAMELEN);
		szRecipeName[RECIPENAMELEN] = _TCHAR('\0');

		strRecipeName = szRecipeName;
		strRecipeName.TrimRight();
		strcpy(pszRecipeName, strRecipeName);
	}

private:
	TCHAR m_szRecipeName[RECIPENAMELEN + 1];
};
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--

//---------------------------------------------------------------------------
// CPifMessage401 着工レシピ指示要求
class CPifMessage401 : public CPifMessage
{
public:
	enum { RECIPENAMELEN   = 24 };

	void SetItemRecipeName(LPCTSTR pszRecipeName)
	{
		sprintf(m_szRecipeName, "%-24.24s", pszRecipeName);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P401"), m_szRecipeName, _tcslen(m_szRecipeName));
	}

	void GetItemRecipeName(LPTSTR pszRecipeName) const
	{
		char szRecipeName[_MAX_FNAME];
		CString strRecipeName;

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szRecipeName, &m_pszRequMessage[nPos], RECIPENAMELEN);
		szRecipeName[RECIPENAMELEN] = _TCHAR('\0');
		strRecipeName = szRecipeName;
		strRecipeName.TrimRight();
		strcpy(pszRecipeName, strRecipeName);
	}

private:
	TCHAR m_szRecipeName[RECIPENAMELEN + 1];
};

// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
//---------------------------------------------------------------------------
// CPifMessage411 リファレンス取得レシピ指示要求
class CPifMessage411 : public CPifMessage
{
public:
	enum { RECIPENAMELEN   = 24 };

	void SetItemRecipeName(LPCTSTR pszRecipeName)
	{
		sprintf(m_szRecipeName, "%-24.24s", pszRecipeName);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P411"), m_szRecipeName, _tcslen(m_szRecipeName));
	}

	void GetItemRecipeName(LPTSTR pszRecipeName) const
	{
		char szRecipeName[_MAX_FNAME];
		CString strRecipeName;

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szRecipeName, &m_pszRequMessage[nPos], RECIPENAMELEN);
		szRecipeName[RECIPENAMELEN] = _TCHAR('\0');
		strRecipeName = szRecipeName;
		strRecipeName.TrimRight();
		strcpy(pszRecipeName, strRecipeName);
	}

private:
	TCHAR m_szRecipeName[RECIPENAMELEN + 1];
};
// 2009.09.10 K.Matsuo リファレンス取得機能追加 <--

//---------------------------------------------------------------------------
// CPifMessage501 測定開始指示要求
class CPifMessage501 : public CPifMessage
{
public:
	enum { SAMPLEIDLEN = 32,
		   LOTIDLEN = 32 };			// Add Item 2009.10.29

	void SetItemSampleId(LPCTSTR pszSampleId)
	{
		sprintf(m_szSampleId, "%-32.32s", pszSampleId);
	}

	void SetItemLotId(LPCTSTR pszLotId)
	{
		sprintf(m_szLotId, "%-32.32s", pszLotId);
	}

	void SendRequ()
	{
		CString strMsg;

		strMsg = m_szSampleId;
		strMsg += m_szLotId;

		CPifMessage::SendRequ(_T("P501"), strMsg, strMsg.GetLength());
	}

	void GetItemSampleId(LPTSTR pszSampleId) const
	{
		char szSampleId[_MAX_FNAME];
		CString strSampleId;

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szSampleId, &m_pszRequMessage[nPos], SAMPLEIDLEN);
		szSampleId[SAMPLEIDLEN] = _TCHAR('\0');
		strSampleId = szSampleId;
		strSampleId.TrimRight();
		strcpy(pszSampleId, strSampleId);
	}

	void GetItemLotId(LPTSTR pszLotId) const
	{
		char szLotId[_MAX_FNAME];
		CString strLotId;

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + SAMPLEIDLEN;

		// Check Message Version
		if ( m_iRecvLength < nPos || m_pszRequMessage[nPos] == CR ) {
			// old version
			strcpy(pszLotId, "");
			return;
		}

		_tcsncpy(szLotId, &m_pszRequMessage[nPos], LOTIDLEN);
		szLotId[LOTIDLEN] = _TCHAR('\0');
		strLotId = szLotId;
		strLotId.TrimRight();
		strcpy(pszLotId, strLotId);
	}

private:
	TCHAR m_szSampleId[SAMPLEIDLEN + 1];
	TCHAR m_szLotId[LOTIDLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage502
class CPifMessage502 : public CPifMessage
{
public:
	enum { FORMATIDLEN = 03,
		   POINTNOLEN  = 05,
		   VARTYPELEN  = 01,
		   DVNAMELEN   = 16,
		   DVVALUELEN  = 16,
		   PROCESSDATAOCCURENCEMAX07 = 7,
		   PROCESSDATAOCCURENCEMAX09 = 9,
		   PROCESSDATAOCCURENCEMAX66 = 66,
		   PROCESSDATAOCCURENCEMAX122 = 122,};

	typedef struct tagPROCESSDATASET
	{
		TCHAR chVarType;
		TCHAR szDvName[DVNAMELEN + 1];
		TCHAR szDvValue[DVVALUELEN + 1];
	} PROCESSDATASET;

	CPifMessage502()
	{
	}

	void SetItemFormatId(int iFormatId)
	{
		_stprintf(m_szFormatId, _T("%03d"), iFormatId);

		// 初期値編集
		switch ( iFormatId ) {
		case 1:	// SR膜厚
		case 2:	// SR反射率
		case 3:	// SR透過率
		case 4:	// SE
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case 7:	// EASE
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			SetItemAF_ST("");
			SetItemXCOORD(0.0);
			SetItemYCOORD(0.0);
			SetItemZCOORD(0.0);
			SetItemLAYERFLG(_T("00000000"));
			SetItemTHICK1(0.0);
			SetItemTHICK2(0.0);
			SetItemTHICK3(0.0);
			SetItemTHICK4(0.0);
			SetItemTHICK5(0.0);
			SetItemTHICK6(0.0);
			SetItemTHICK7(0.0);
			SetItemTHICK8(0.0);
			SetItemN1(0.0);
			SetItemN2(0.0);
			SetItemN3(0.0);
			SetItemN4(0.0);
			SetItemN5(0.0);
			SetItemN6(0.0);
			SetItemN7(0.0);
			SetItemN8(0.0);
			SetItemK1(0.0);
			SetItemK2(0.0);
			SetItemK3(0.0);
			SetItemK4(0.0);
			SetItemK5(0.0);
			SetItemK6(0.0);
			SetItemK7(0.0);
			SetItemK8(0.0);
			SetItemCONST1(0.0);
			SetItemCONST2(0.0);
			SetItemCONST3(0.0);
			SetItemCONST4(0.0);
			SetItemCONST5(0.0);
			SetItemCONST6(0.0);
			SetItemCONST7(0.0);
			SetItemCONST8(0.0);
			SetItemFIT(0.0);
			SetItemREFLECT1_WAVE(0.0);
			SetItemREFLECT2_WAVE(0.0);
			SetItemREFLECT3_WAVE(0.0);
			SetItemREFLECT1(0.0);
			SetItemREFLECT2(0.0);
			SetItemREFLECT3(0.0);
			SetItemTRANSMIT1_WAVE(0.0);
			SetItemTRANSMIT2_WAVE(0.0);
			SetItemTRANSMIT3_WAVE(0.0);
			SetItemTRANSMIT1(0.0);
			SetItemTRANSMIT2(0.0);
			SetItemTRANSMIT3(0.0);
			SetItemEG1(0.0);
			SetItemEG2(0.0);
			SetItemEG3(0.0);
			SetItemEG4(0.0);
			SetItemEG5(0.0);
			SetItemEG6(0.0);
			SetItemEG7(0.0);
			SetItemEG8(0.0);
			SetItemFRACTION1(0.0);
			SetItemFRACTION2(0.0);
			SetItemFRACTION3(0.0);
			SetItemFRACTION4(0.0);
			SetItemFRACTION5(0.0);
			SetItemFRACTION6(0.0);
			SetItemFRACTION7(0.0);
			SetItemFRACTION8(0.0);
			break;
		case 5:	// RS(Resist)
			SetItemAF_ST("");		// Filler項目（AF未使用）
			SetItemXCOORD(0.0);
			SetItemYCOORD(0.0);
			SetItemZCOORD(0.0);
			SetItemRSDATAVALID(" ");
			SetItemRSRESIST("0.000E+00");
			SetItemRSSURFACERESIST("0.000E+00");
			SetItemRSVOLUMERESIST("0.000E+00");
			SetItemRSCONDUCTIVITY("0.000E+00");
			break;
		case 6:	// CA(ContactAngle)
			SetItemAF_ST("");		// Filler項目（AF未使用）
			SetItemXCOORD(0.0);
			SetItemYCOORD(0.0);
			SetItemZCOORD(0.0);
			SetItemCACONTACTANGLE(0.0);
			SetItemCARADIUS(0.0);
			SetItemCALIQUID(0.0);
			break;
		case 101:
// 2009.09.17 K.Matsuo リファレンス・応力測定モード -->
			SetItemSTRMEASMODE(0);
// 2009.09.17 K.Matsuo リファレンス・応力測定モード <--
			SetItemSTRLx_Valid(0);
			SetItemSTRLxS1_Valid(0);
			SetItemSTRLxS1_StartX(0.0);
			SetItemSTRLxS1_StartY(0.0);
			SetItemSTRLxS1_EndX(0.0);
			SetItemSTRLxS1_EndY(0.0);
			SetItemSTRLxS1_Stress(0.0);
			SetItemSTRLxS2_Valid(0);
			SetItemSTRLxS2_StartX(0.0);
			SetItemSTRLxS2_StartY(0.0);
			SetItemSTRLxS2_EndX(0.0);
			SetItemSTRLxS2_EndY(0.0);
			SetItemSTRLxS2_Stress(0.0);
			SetItemSTRLxS3_Valid(0);
			SetItemSTRLxS3_StartX(0.0);
			SetItemSTRLxS3_StartY(0.0);
			SetItemSTRLxS3_EndX(0.0);
			SetItemSTRLxS3_EndY(0.0);
			SetItemSTRLxS3_Stress(0.0);
			SetItemSTRLxS4_Valid(0);
			SetItemSTRLxS4_StartX(0.0);
			SetItemSTRLxS4_StartY(0.0);
			SetItemSTRLxS4_EndX(0.0);
			SetItemSTRLxS4_EndY(0.0);
			SetItemSTRLxS4_Stress(0.0);
			SetItemSTRLxS5_Valid(0);
			SetItemSTRLxS5_StartX(0.0);
			SetItemSTRLxS5_StartY(0.0);
			SetItemSTRLxS5_EndX(0.0);
			SetItemSTRLxS5_EndY(0.0);
			SetItemSTRLxS5_Stress(0.0);
			SetItemSTRLxS6_Valid(0);
			SetItemSTRLxS6_StartX(0.0);
			SetItemSTRLxS6_StartY(0.0);
			SetItemSTRLxS6_EndX(0.0);
			SetItemSTRLxS6_EndY(0.0);
			SetItemSTRLxS6_Stress(0.0);
			SetItemSTRLxS7_Valid(0);
			SetItemSTRLxS7_StartX(0.0);
			SetItemSTRLxS7_StartY(0.0);
			SetItemSTRLxS7_EndX(0.0);
			SetItemSTRLxS7_EndY(0.0);
			SetItemSTRLxS7_Stress(0.0);
			SetItemSTRLxS8_Valid(0);
			SetItemSTRLxS8_StartX(0.0);
			SetItemSTRLxS8_StartY(0.0);
			SetItemSTRLxS8_EndX(0.0);
			SetItemSTRLxS8_EndY(0.0);
			SetItemSTRLxS8_Stress(0.0);
			SetItemSTRLxS9_Valid(0);
			SetItemSTRLxS9_StartX(0.0);
			SetItemSTRLxS9_StartY(0.0);
			SetItemSTRLxS9_EndX(0.0);
			SetItemSTRLxS9_EndY(0.0);
			SetItemSTRLxS9_Stress(0.0);
			SetItemSTRLxS10_Valid(0);
			SetItemSTRLxS10_StartX(0.0);
			SetItemSTRLxS10_StartY(0.0);
			SetItemSTRLxS10_EndX(0.0);
			SetItemSTRLxS10_EndY(0.0);
			SetItemSTRLxS10_Stress(0.0);
			SetItemSTRLxS11_Valid(0);
			SetItemSTRLxS11_StartX(0.0);
			SetItemSTRLxS11_StartY(0.0);
			SetItemSTRLxS11_EndX(0.0);
			SetItemSTRLxS11_EndY(0.0);
			SetItemSTRLxS11_Stress(0.0);
			SetItemSTRLxS12_Valid(0);
			SetItemSTRLxS12_StartX(0.0);
			SetItemSTRLxS12_StartY(0.0);
			SetItemSTRLxS12_EndX(0.0);
			SetItemSTRLxS12_EndY(0.0);
			SetItemSTRLxS12_Stress(0.0);
			SetItemSTRLxS13_Valid(0);
			SetItemSTRLxS13_StartX(0.0);
			SetItemSTRLxS13_StartY(0.0);
			SetItemSTRLxS13_EndX(0.0);
			SetItemSTRLxS13_EndY(0.0);
			SetItemSTRLxS13_Stress(0.0);
			SetItemSTRLxS14_Valid(0);
			SetItemSTRLxS14_StartX(0.0);
			SetItemSTRLxS14_StartY(0.0);
			SetItemSTRLxS14_EndX(0.0);
			SetItemSTRLxS14_EndY(0.0);
			SetItemSTRLxS14_Stress(0.0);
			SetItemSTRLxS15_Valid(0);
			SetItemSTRLxS15_StartX(0.0);
			SetItemSTRLxS15_StartY(0.0);
			SetItemSTRLxS15_EndX(0.0);
			SetItemSTRLxS15_EndY(0.0);
			SetItemSTRLxS15_Stress(0.0);
			SetItemSTRLxS16_Valid(0);
			SetItemSTRLxS16_StartX(0.0);
			SetItemSTRLxS16_StartY(0.0);
			SetItemSTRLxS16_EndX(0.0);
			SetItemSTRLxS16_EndY(0.0);
			SetItemSTRLxS16_Stress(0.0);
			SetItemSTRLxS17_Valid(0);
			SetItemSTRLxS17_StartX(0.0);
			SetItemSTRLxS17_StartY(0.0);
			SetItemSTRLxS17_EndX(0.0);
			SetItemSTRLxS17_EndY(0.0);
			SetItemSTRLxS17_Stress(0.0);
			SetItemSTRLxS18_Valid(0);
			SetItemSTRLxS18_StartX(0.0);
			SetItemSTRLxS18_StartY(0.0);
			SetItemSTRLxS18_EndX(0.0);
			SetItemSTRLxS18_EndY(0.0);
			SetItemSTRLxS18_Stress(0.0);
			SetItemSTRLxS19_Valid(0);
			SetItemSTRLxS19_StartX(0.0);
			SetItemSTRLxS19_StartY(0.0);
			SetItemSTRLxS19_EndX(0.0);
			SetItemSTRLxS19_EndY(0.0);
			SetItemSTRLxS19_Stress(0.0);
			SetItemSTRLxS20_Valid(0);
			SetItemSTRLxS20_StartX(0.0);
			SetItemSTRLxS20_StartY(0.0);
			SetItemSTRLxS20_EndX(0.0);
			SetItemSTRLxS20_EndY(0.0);
			SetItemSTRLxS20_Stress(0.0);
			break;
		}
	}

	void SetItemPointNo(int iPointNo)
	{
		_stprintf(m_szPointNo, _T("%05d"), iPointNo);
	}

	void SetItemLineNo(int iLineNo)
	{
		SetItemPointNo(iLineNo);
	}

	// FormatId 001,002,003,004,007
	GETSETITEMASCII(0, AF_ST)
	GETSETITEMDOUBLE(1, XCOORD)
	GETSETITEMDOUBLE(2, YCOORD)
	GETSETITEMDOUBLE(3, ZCOORD)
	GETSETITEMASCII(4, LAYERFLG)
	GETSETITEMDOUBLE(5, THICK1)
	GETSETITEMDOUBLE(6, THICK2)
	GETSETITEMDOUBLE(7, THICK3)
	GETSETITEMDOUBLE(8, THICK4)
	GETSETITEMDOUBLE(9, THICK5)
	GETSETITEMDOUBLE(10, THICK6)
	GETSETITEMDOUBLE(11, THICK7)
	GETSETITEMDOUBLE(12, THICK8)
	GETSETITEMDOUBLE(13, N1)
	GETSETITEMDOUBLE(14, N2)
	GETSETITEMDOUBLE(15, N3)
	GETSETITEMDOUBLE(16, N4)
	GETSETITEMDOUBLE(17, N5)
	GETSETITEMDOUBLE(18, N6)
	GETSETITEMDOUBLE(19, N7)
	GETSETITEMDOUBLE(20, N8)
	GETSETITEMDOUBLE(21, K1)
	GETSETITEMDOUBLE(22, K2)
	GETSETITEMDOUBLE(23, K3)
	GETSETITEMDOUBLE(24, K4)
	GETSETITEMDOUBLE(25, K5)
	GETSETITEMDOUBLE(26, K6)
	GETSETITEMDOUBLE(27, K7)
	GETSETITEMDOUBLE(28, K8)
	GETSETITEMDOUBLE(29, CONST1)
	GETSETITEMDOUBLE(30, CONST2)
	GETSETITEMDOUBLE(31, CONST3)
	GETSETITEMDOUBLE(32, CONST4)
	GETSETITEMDOUBLE(33, CONST5)
	GETSETITEMDOUBLE(34, CONST6)
	GETSETITEMDOUBLE(35, CONST7)
	GETSETITEMDOUBLE(36, CONST8)
	GETSETITEMDOUBLE(37, FIT)
	GETSETITEMDOUBLE(38, REFLECT1_WAVE)
	GETSETITEMDOUBLE(39, REFLECT2_WAVE)
	GETSETITEMDOUBLE(40, REFLECT3_WAVE)
	GETSETITEMDOUBLE(41, REFLECT1)
	GETSETITEMDOUBLE(42, REFLECT2)
	GETSETITEMDOUBLE(43, REFLECT3)
	GETSETITEMDOUBLE(44, TRANSMIT1_WAVE)
	GETSETITEMDOUBLE(45, TRANSMIT2_WAVE)
	GETSETITEMDOUBLE(46, TRANSMIT3_WAVE)
	GETSETITEMDOUBLE(47, TRANSMIT1)
	GETSETITEMDOUBLE(48, TRANSMIT2)
	GETSETITEMDOUBLE(49, TRANSMIT3)
	GETSETITEMDOUBLE(50, EG1)
	GETSETITEMDOUBLE(51, EG2)
	GETSETITEMDOUBLE(52, EG3)
	GETSETITEMDOUBLE(53, EG4)
	GETSETITEMDOUBLE(54, EG5)
	GETSETITEMDOUBLE(55, EG6)
	GETSETITEMDOUBLE(56, EG7)
	GETSETITEMDOUBLE(57, EG8)
	GETSETITEMDOUBLE(58, FRACTION1)
	GETSETITEMDOUBLE(59, FRACTION2)
	GETSETITEMDOUBLE(60, FRACTION3)
	GETSETITEMDOUBLE(61, FRACTION4)
	GETSETITEMDOUBLE(62, FRACTION5)
	GETSETITEMDOUBLE(63, FRACTION6)
	GETSETITEMDOUBLE(64, FRACTION7)
	GETSETITEMDOUBLE(65, FRACTION8)

	// FormatId 005
//	GETSETITEMASCII(0, AF_ST)			// Filler項目（AF未使用）
//	GETSETITEMDOUBLE(1, XCOORD)			// 既に定義済み
//	GETSETITEMDOUBLE(2, YCOORD)			// 既に定義済み
//	GETSETITEMDOUBLE(3, ZCOORD)			// 既に定義済み
	GETSETITEMASCII(4, RSDATAVALID)
	GETSETITEMASCII(5, RSRESIST)
	GETSETITEMASCII(6, RSSURFACERESIST)
	GETSETITEMASCII(7, RSVOLUMERESIST)
	GETSETITEMASCII(8, RSCONDUCTIVITY)

	// FormatId 006
//	GETSETITEMASCII(0, AF_ST)			// Filler項目（AF未使用）
//	GETSETITEMDOUBLE(1, XCOORD)			// 既に定義済み
//	GETSETITEMDOUBLE(2, YCOORD)			// 既に定義済み
//	GETSETITEMDOUBLE(3, ZCOORD)			// 既に定義済み
	GETSETITEMDOUBLE(4, CACONTACTANGLE)
	GETSETITEMDOUBLE(5, CARADIUS)
	GETSETITEMDOUBLE(6, CALIQUID)

	// FormatId 101
// 2009.09.17 K.Matsuo リファレンス・応力測定モード -->
	GETSETITEMINT(0, STRMEASMODE)
// 2009.09.17 K.Matsuo リファレンス・応力測定モード <--
	GETSETITEMINT(1, STRLx_Valid)
	GETSETITEMINT(2, STRLxS1_Valid)
	GETSETITEMDOUBLE(3, STRLxS1_StartX)
	GETSETITEMDOUBLE(4, STRLxS1_StartY)
	GETSETITEMDOUBLE(5, STRLxS1_EndX)
	GETSETITEMDOUBLE(6, STRLxS1_EndY)
	GETSETITEMDOUBLE(7, STRLxS1_Stress)
	GETSETITEMINT(8, STRLxS2_Valid)
	GETSETITEMDOUBLE(9, STRLxS2_StartX)
	GETSETITEMDOUBLE(10, STRLxS2_StartY)
	GETSETITEMDOUBLE(11, STRLxS2_EndX)
	GETSETITEMDOUBLE(12, STRLxS2_EndY)
	GETSETITEMDOUBLE(13, STRLxS2_Stress)
	GETSETITEMINT(14, STRLxS3_Valid)
	GETSETITEMDOUBLE(15, STRLxS3_StartX)
	GETSETITEMDOUBLE(16, STRLxS3_StartY)
	GETSETITEMDOUBLE(17, STRLxS3_EndX)
	GETSETITEMDOUBLE(18, STRLxS3_EndY)
	GETSETITEMDOUBLE(19, STRLxS3_Stress)
	GETSETITEMINT(20, STRLxS4_Valid)
	GETSETITEMDOUBLE(21, STRLxS4_StartX)
	GETSETITEMDOUBLE(22, STRLxS4_StartY)
	GETSETITEMDOUBLE(23, STRLxS4_EndX)
	GETSETITEMDOUBLE(24, STRLxS4_EndY)
	GETSETITEMDOUBLE(25, STRLxS4_Stress)
	GETSETITEMINT(26, STRLxS5_Valid)
	GETSETITEMDOUBLE(27, STRLxS5_StartX)
	GETSETITEMDOUBLE(28, STRLxS5_StartY)
	GETSETITEMDOUBLE(29, STRLxS5_EndX)
	GETSETITEMDOUBLE(30, STRLxS5_EndY)
	GETSETITEMDOUBLE(31, STRLxS5_Stress)
	GETSETITEMINT(32, STRLxS6_Valid)
	GETSETITEMDOUBLE(33, STRLxS6_StartX)
	GETSETITEMDOUBLE(34, STRLxS6_StartY)
	GETSETITEMDOUBLE(35, STRLxS6_EndX)
	GETSETITEMDOUBLE(36, STRLxS6_EndY)
	GETSETITEMDOUBLE(37, STRLxS6_Stress)
	GETSETITEMINT(38, STRLxS7_Valid)
	GETSETITEMDOUBLE(39, STRLxS7_StartX)
	GETSETITEMDOUBLE(40, STRLxS7_StartY)
	GETSETITEMDOUBLE(41, STRLxS7_EndX)
	GETSETITEMDOUBLE(42, STRLxS7_EndY)
	GETSETITEMDOUBLE(43, STRLxS7_Stress)
	GETSETITEMINT(44, STRLxS8_Valid)
	GETSETITEMDOUBLE(45, STRLxS8_StartX)
	GETSETITEMDOUBLE(46, STRLxS8_StartY)
	GETSETITEMDOUBLE(47, STRLxS8_EndX)
	GETSETITEMDOUBLE(48, STRLxS8_EndY)
	GETSETITEMDOUBLE(49, STRLxS8_Stress)
	GETSETITEMINT(50, STRLxS9_Valid)
	GETSETITEMDOUBLE(51, STRLxS9_StartX)
	GETSETITEMDOUBLE(52, STRLxS9_StartY)
	GETSETITEMDOUBLE(53, STRLxS9_EndX)
	GETSETITEMDOUBLE(54, STRLxS9_EndY)
	GETSETITEMDOUBLE(55, STRLxS9_Stress)
	GETSETITEMINT(56, STRLxS10_Valid)
	GETSETITEMDOUBLE(57, STRLxS10_StartX)
	GETSETITEMDOUBLE(58, STRLxS10_StartY)
	GETSETITEMDOUBLE(59, STRLxS10_EndX)
	GETSETITEMDOUBLE(60, STRLxS10_EndY)
	GETSETITEMDOUBLE(61, STRLxS10_Stress)
	GETSETITEMINT(62, STRLxS11_Valid)
	GETSETITEMDOUBLE(63, STRLxS11_StartX)
	GETSETITEMDOUBLE(64, STRLxS11_StartY)
	GETSETITEMDOUBLE(65, STRLxS11_EndX)
	GETSETITEMDOUBLE(66, STRLxS11_EndY)
	GETSETITEMDOUBLE(67, STRLxS11_Stress)
	GETSETITEMINT(68, STRLxS12_Valid)
	GETSETITEMDOUBLE(69, STRLxS12_StartX)
	GETSETITEMDOUBLE(70, STRLxS12_StartY)
	GETSETITEMDOUBLE(71, STRLxS12_EndX)
	GETSETITEMDOUBLE(72, STRLxS12_EndY)
	GETSETITEMDOUBLE(73, STRLxS12_Stress)
	GETSETITEMINT(74, STRLxS13_Valid)
	GETSETITEMDOUBLE(75, STRLxS13_StartX)
	GETSETITEMDOUBLE(76, STRLxS13_StartY)
	GETSETITEMDOUBLE(77, STRLxS13_EndX)
	GETSETITEMDOUBLE(78, STRLxS13_EndY)
	GETSETITEMDOUBLE(79, STRLxS13_Stress)
	GETSETITEMINT(80, STRLxS14_Valid)
	GETSETITEMDOUBLE(81, STRLxS14_StartX)
	GETSETITEMDOUBLE(82, STRLxS14_StartY)
	GETSETITEMDOUBLE(83, STRLxS14_EndX)
	GETSETITEMDOUBLE(84, STRLxS14_EndY)
	GETSETITEMDOUBLE(85, STRLxS14_Stress)
	GETSETITEMINT(86, STRLxS15_Valid)
	GETSETITEMDOUBLE(87, STRLxS15_StartX)
	GETSETITEMDOUBLE(88, STRLxS15_StartY)
	GETSETITEMDOUBLE(89, STRLxS15_EndX)
	GETSETITEMDOUBLE(90, STRLxS15_EndY)
	GETSETITEMDOUBLE(91, STRLxS15_Stress)
	GETSETITEMINT(92, STRLxS16_Valid)
	GETSETITEMDOUBLE(93, STRLxS16_StartX)
	GETSETITEMDOUBLE(94, STRLxS16_StartY)
	GETSETITEMDOUBLE(95, STRLxS16_EndX)
	GETSETITEMDOUBLE(96, STRLxS16_EndY)
	GETSETITEMDOUBLE(97, STRLxS16_Stress)
	GETSETITEMINT(98, STRLxS17_Valid)
	GETSETITEMDOUBLE(99, STRLxS17_StartX)
	GETSETITEMDOUBLE(100, STRLxS17_StartY)
	GETSETITEMDOUBLE(101, STRLxS17_EndX)
	GETSETITEMDOUBLE(102, STRLxS17_EndY)
	GETSETITEMDOUBLE(103, STRLxS17_Stress)
	GETSETITEMINT(104, STRLxS18_Valid)
	GETSETITEMDOUBLE(105, STRLxS18_StartX)
	GETSETITEMDOUBLE(106, STRLxS18_StartY)
	GETSETITEMDOUBLE(107, STRLxS18_EndX)
	GETSETITEMDOUBLE(108, STRLxS18_EndY)
	GETSETITEMDOUBLE(109, STRLxS18_Stress)
	GETSETITEMINT(110, STRLxS19_Valid)
	GETSETITEMDOUBLE(111, STRLxS19_StartX)
	GETSETITEMDOUBLE(112, STRLxS19_StartY)
	GETSETITEMDOUBLE(113, STRLxS19_EndX)
	GETSETITEMDOUBLE(114, STRLxS19_EndY)
	GETSETITEMDOUBLE(115, STRLxS19_Stress)
	GETSETITEMINT(116, STRLxS20_Valid)
	GETSETITEMDOUBLE(117, STRLxS20_StartX)
	GETSETITEMDOUBLE(118, STRLxS20_StartY)
	GETSETITEMDOUBLE(119, STRLxS20_EndX)
	GETSETITEMDOUBLE(120, STRLxS20_EndY)
	GETSETITEMDOUBLE(121, STRLxS20_Stress)

	int GetItemFormatId() const
	{
		TCHAR szFormatId[FORMATIDLEN + 1];

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szFormatId, &m_pszRequMessage[nPos], FORMATIDLEN);
		szFormatId[FORMATIDLEN] = _TCHAR('\0');
		return _ttoi(szFormatId);
	}

	int GetItemPointNo() const
	{
		TCHAR szPointNo[POINTNOLEN + 1];

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + FORMATIDLEN;
		_tcsncpy(szPointNo, &m_pszRequMessage[nPos], POINTNOLEN);
		szPointNo[POINTNOLEN] = _TCHAR('\0');
		return _ttoi(szPointNo);
	}

	int GetItemLineNo() const
	{
		return GetItemPointNo();
	}

	void SendRequ()
	{
		CString strMsg;

		if ( _tcscmp(m_szFormatId, _T("001")) == 0 ||
			 _tcscmp(m_szFormatId, _T("002")) == 0 ||
			 _tcscmp(m_szFormatId, _T("003")) == 0 ||
			 _tcscmp(m_szFormatId, _T("004")) == 0 ||
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			 _tcscmp(m_szFormatId, _T("007")) == 0 ) {
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
			for ( int i = 0; i < PROCESSDATAOCCURENCEMAX66; i++ )
				AddProcessDataItem(i, strMsg);
		}
		else if ( _tcscmp(m_szFormatId, _T("005")) == 0 ) {
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
			for ( int i = 0; i < PROCESSDATAOCCURENCEMAX09; i++ )
				AddProcessDataItem(i, strMsg);
		}
		else if ( _tcscmp(m_szFormatId, _T("006")) == 0 ) {
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
			for ( int i = 0; i < PROCESSDATAOCCURENCEMAX07; i++ )
				AddProcessDataItem(i, strMsg);
		}
		else if ( _tcscmp(m_szFormatId, _T("101")) == 0 ) {
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
			for ( int i = 0; i < PROCESSDATAOCCURENCEMAX122; i++ )
				AddProcessDataItem(i, strMsg);
		}
		else {
			ASSERT( FALSE );
		}
		CPifMessage::SendRequ(_T("P502"), strMsg, strMsg.GetLength());
	}

protected:
	void AddProcessDataItem(int i, CString& strMsg)
	{
		char szBuff[256];
		strMsg += stProcessDataSet[i].chVarType;
		sprintf(szBuff, "%-16.16s", stProcessDataSet[i].szDvName);
		strMsg += szBuff;
		sprintf(szBuff, "%-16.16s", stProcessDataSet[i].szDvValue);
		strMsg += szBuff;
	}

private:
	TCHAR m_szFormatId[FORMATIDLEN + 1];
	TCHAR m_szPointNo[POINTNOLEN + 1];
	PROCESSDATASET stProcessDataSet[PROCESSDATAOCCURENCEMAX122];
};

//---------------------------------------------------------------------------
// CPifMessage503
class CPifMessage503 : public CPifMessage
{
public:
	enum { FORMATIDLEN = 03,
		   POINTNOLEN  = 05,
		   VARTYPELEN  = 01,
		   DVNAMELEN  = 16,
		   DVVALUELEN = 16,
		   PROCESSDATAOCCURENCEMAX15 = 15,
		   PROCESSDATAOCCURENCEMAX20 = 20,
		   PROCESSDATAOCCURENCEMAX108 = 108,
		   PROCESSDATAOCCURENCEMAX277 = 277 };

	typedef struct tagPROCESSDATASET
	{
		TCHAR chVarType;
		TCHAR szDvName[DVNAMELEN + 1];
		TCHAR szDvValue[DVVALUELEN + 1];
	} PROCESSDATASET;

	CPifMessage503()
	{
	}

	void SetItemFormatId(int iFormatId)
	{
		_stprintf(m_szFormatId, _T("%03d"), iFormatId);

		// 初期値編集
		switch ( iFormatId ) {
		case 1:	// SR膜厚
		case 2:	// SR反射率
		case 3:	// SR透過率
		case 4:	// SE
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case 7:	// EASE
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			SetItemLAYERFLG(_T("00000000"));
			SetItemTHICK1_MIN(0.0);
			SetItemTHICK2_MIN(0.0);
			SetItemTHICK3_MIN(0.0);
			SetItemTHICK4_MIN(0.0);
			SetItemTHICK5_MIN(0.0);
			SetItemTHICK6_MIN(0.0);
			SetItemTHICK7_MIN(0.0);
			SetItemTHICK8_MIN(0.0);
			SetItemTHICK1_MAX(0.0);
			SetItemTHICK2_MAX(0.0);
			SetItemTHICK3_MAX(0.0);
			SetItemTHICK4_MAX(0.0);
			SetItemTHICK5_MAX(0.0);
			SetItemTHICK6_MAX(0.0);
			SetItemTHICK7_MAX(0.0);
			SetItemTHICK8_MAX(0.0);
			SetItemTHICK1_AVE(0.0);
			SetItemTHICK2_AVE(0.0);
			SetItemTHICK3_AVE(0.0);
			SetItemTHICK4_AVE(0.0);
			SetItemTHICK5_AVE(0.0);
			SetItemTHICK6_AVE(0.0);
			SetItemTHICK7_AVE(0.0);
			SetItemTHICK8_AVE(0.0);
			SetItemTHICK1_SD(0.0);
			SetItemTHICK2_SD(0.0);
			SetItemTHICK3_SD(0.0);
			SetItemTHICK4_SD(0.0);
			SetItemTHICK5_SD(0.0);
			SetItemTHICK6_SD(0.0);
			SetItemTHICK7_SD(0.0);
			SetItemTHICK8_SD(0.0);
			SetItemTHICK1_UNIF(0.0);
			SetItemTHICK2_UNIF(0.0);
			SetItemTHICK3_UNIF(0.0);
			SetItemTHICK4_UNIF(0.0);
			SetItemTHICK5_UNIF(0.0);
			SetItemTHICK6_UNIF(0.0);
			SetItemTHICK7_UNIF(0.0);
			SetItemTHICK8_UNIF(0.0);
			SetItemN1_MIN(0.0);
			SetItemN2_MIN(0.0);
			SetItemN3_MIN(0.0);
			SetItemN4_MIN(0.0);
			SetItemN5_MIN(0.0);
			SetItemN6_MIN(0.0);
			SetItemN7_MIN(0.0);
			SetItemN8_MIN(0.0);
			SetItemN1_MAX(0.0);
			SetItemN2_MAX(0.0);
			SetItemN3_MAX(0.0);
			SetItemN4_MAX(0.0);
			SetItemN5_MAX(0.0);
			SetItemN6_MAX(0.0);
			SetItemN7_MAX(0.0);
			SetItemN8_MAX(0.0);
			SetItemN1_AVE(0.0);
			SetItemN2_AVE(0.0);
			SetItemN3_AVE(0.0);
			SetItemN4_AVE(0.0);
			SetItemN5_AVE(0.0);
			SetItemN6_AVE(0.0);
			SetItemN7_AVE(0.0);
			SetItemN8_AVE(0.0);
			SetItemN1_SD(0.0);
			SetItemN2_SD(0.0);
			SetItemN3_SD(0.0);
			SetItemN4_SD(0.0);
			SetItemN5_SD(0.0);
			SetItemN6_SD(0.0);
			SetItemN7_SD(0.0);
			SetItemN8_SD(0.0);
			SetItemN1_UNIF(0.0);
			SetItemN2_UNIF(0.0);
			SetItemN3_UNIF(0.0);
			SetItemN4_UNIF(0.0);
			SetItemN5_UNIF(0.0);
			SetItemN6_UNIF(0.0);
			SetItemN7_UNIF(0.0);
			SetItemN8_UNIF(0.0);
			SetItemK1_MIN(0.0);
			SetItemK2_MIN(0.0);
			SetItemK3_MIN(0.0);
			SetItemK4_MIN(0.0);
			SetItemK5_MIN(0.0);
			SetItemK6_MIN(0.0);
			SetItemK7_MIN(0.0);
			SetItemK8_MIN(0.0);
			SetItemK1_MAX(0.0);
			SetItemK2_MAX(0.0);
			SetItemK3_MAX(0.0);
			SetItemK4_MAX(0.0);
			SetItemK5_MAX(0.0);
			SetItemK6_MAX(0.0);
			SetItemK7_MAX(0.0);
			SetItemK8_MAX(0.0);
			SetItemK1_AVE(0.0);
			SetItemK2_AVE(0.0);
			SetItemK3_AVE(0.0);
			SetItemK4_AVE(0.0);
			SetItemK5_AVE(0.0);
			SetItemK6_AVE(0.0);
			SetItemK7_AVE(0.0);
			SetItemK8_AVE(0.0);
			SetItemK1_SD(0.0);
			SetItemK2_SD(0.0);
			SetItemK3_SD(0.0);
			SetItemK4_SD(0.0);
			SetItemK5_SD(0.0);
			SetItemK6_SD(0.0);
			SetItemK7_SD(0.0);
			SetItemK8_SD(0.0);
			SetItemK1_UNIF(0.0);
			SetItemK2_UNIF(0.0);
			SetItemK3_UNIF(0.0);
			SetItemK4_UNIF(0.0);
			SetItemK5_UNIF(0.0);
			SetItemK6_UNIF(0.0);
			SetItemK7_UNIF(0.0);
			SetItemK8_UNIF(0.0);
			SetItemCONST1_MIN(0.0);
			SetItemCONST2_MIN(0.0);
			SetItemCONST3_MIN(0.0);
			SetItemCONST4_MIN(0.0);
			SetItemCONST5_MIN(0.0);
			SetItemCONST6_MIN(0.0);
			SetItemCONST7_MIN(0.0);
			SetItemCONST8_MIN(0.0);
			SetItemCONST1_MAX(0.0);
			SetItemCONST2_MAX(0.0);
			SetItemCONST3_MAX(0.0);
			SetItemCONST4_MAX(0.0);
			SetItemCONST5_MAX(0.0);
			SetItemCONST6_MAX(0.0);
			SetItemCONST7_MAX(0.0);
			SetItemCONST8_MAX(0.0);
			SetItemCONST1_AVE(0.0);
			SetItemCONST2_AVE(0.0);
			SetItemCONST3_AVE(0.0);
			SetItemCONST4_AVE(0.0);
			SetItemCONST5_AVE(0.0);
			SetItemCONST6_AVE(0.0);
			SetItemCONST7_AVE(0.0);
			SetItemCONST8_AVE(0.0);
			SetItemCONST1_SD(0.0);
			SetItemCONST2_SD(0.0);
			SetItemCONST3_SD(0.0);
			SetItemCONST4_SD(0.0);
			SetItemCONST5_SD(0.0);
			SetItemCONST6_SD(0.0);
			SetItemCONST7_SD(0.0);
			SetItemCONST8_SD(0.0);
			SetItemCONST1_UNIF(0.0);
			SetItemCONST2_UNIF(0.0);
			SetItemCONST3_UNIF(0.0);
			SetItemCONST4_UNIF(0.0);
			SetItemCONST5_UNIF(0.0);
			SetItemCONST6_UNIF(0.0);
			SetItemCONST7_UNIF(0.0);
			SetItemCONST8_UNIF(0.0);
			SetItemREFLECT1_WAVE(0.0);
			SetItemREFLECT2_WAVE(0.0);
			SetItemREFLECT3_WAVE(0.0);
			SetItemREFLECT1_MIN(0.0);
			SetItemREFLECT2_MIN(0.0);
			SetItemREFLECT3_MIN(0.0);
			SetItemREFLECT1_MAX(0.0);
			SetItemREFLECT2_MAX(0.0);
			SetItemREFLECT3_MAX(0.0);
			SetItemREFLECT1_AVE(0.0);
			SetItemREFLECT2_AVE(0.0);
			SetItemREFLECT3_AVE(0.0);
			SetItemREFLECT1_SD(0.0);
			SetItemREFLECT2_SD(0.0);
			SetItemREFLECT3_SD(0.0);
			SetItemREFLECT1_UNIF(0.0);
			SetItemREFLECT2_UNIF(0.0);
			SetItemREFLECT3_UNIF(0.0);
			SetItemTRANSMIT1_WAVE(0.0);
			SetItemTRANSMIT2_WAVE(0.0);
			SetItemTRANSMIT3_WAVE(0.0);
			SetItemTRANSMIT1_MIN(0.0);
			SetItemTRANSMIT2_MIN(0.0);
			SetItemTRANSMIT3_MIN(0.0);
			SetItemTRANSMIT1_MAX(0.0);
			SetItemTRANSMIT2_MAX(0.0);
			SetItemTRANSMIT3_MAX(0.0);
			SetItemTRANSMIT1_AVE(0.0);
			SetItemTRANSMIT2_AVE(0.0);
			SetItemTRANSMIT3_AVE(0.0);
			SetItemTRANSMIT1_SD(0.0);
			SetItemTRANSMIT2_SD(0.0);
			SetItemTRANSMIT3_SD(0.0);
			SetItemTRANSMIT1_UNIF(0.0);
			SetItemTRANSMIT2_UNIF(0.0);
			SetItemTRANSMIT3_UNIF(0.0);
			SetItemEG1_MIN(0.0);
			SetItemEG2_MIN(0.0);
			SetItemEG3_MIN(0.0);
			SetItemEG4_MIN(0.0);
			SetItemEG5_MIN(0.0);
			SetItemEG6_MIN(0.0);
			SetItemEG7_MIN(0.0);
			SetItemEG8_MIN(0.0);
			SetItemEG1_MAX(0.0);
			SetItemEG2_MAX(0.0);
			SetItemEG3_MAX(0.0);
			SetItemEG4_MAX(0.0);
			SetItemEG5_MAX(0.0);
			SetItemEG6_MAX(0.0);
			SetItemEG7_MAX(0.0);
			SetItemEG8_MAX(0.0);
			SetItemEG1_AVE(0.0);
			SetItemEG2_AVE(0.0);
			SetItemEG3_AVE(0.0);
			SetItemEG4_AVE(0.0);
			SetItemEG5_AVE(0.0);
			SetItemEG6_AVE(0.0);
			SetItemEG7_AVE(0.0);
			SetItemEG8_AVE(0.0);
			SetItemEG1_SD(0.0);
			SetItemEG2_SD(0.0);
			SetItemEG3_SD(0.0);
			SetItemEG4_SD(0.0);
			SetItemEG5_SD(0.0);
			SetItemEG6_SD(0.0);
			SetItemEG7_SD(0.0);
			SetItemEG8_SD(0.0);
			SetItemEG1_UNIF(0.0);
			SetItemEG2_UNIF(0.0);
			SetItemEG3_UNIF(0.0);
			SetItemEG4_UNIF(0.0);
			SetItemEG5_UNIF(0.0);
			SetItemEG6_UNIF(0.0);
			SetItemEG7_UNIF(0.0);
			SetItemEG8_UNIF(0.0);
			SetItemFRACTION1_MIN(0.0);
			SetItemFRACTION2_MIN(0.0);
			SetItemFRACTION3_MIN(0.0);
			SetItemFRACTION4_MIN(0.0);
			SetItemFRACTION5_MIN(0.0);
			SetItemFRACTION6_MIN(0.0);
			SetItemFRACTION7_MIN(0.0);
			SetItemFRACTION8_MIN(0.0);
			SetItemFRACTION1_MAX(0.0);
			SetItemFRACTION2_MAX(0.0);
			SetItemFRACTION3_MAX(0.0);
			SetItemFRACTION4_MAX(0.0);
			SetItemFRACTION5_MAX(0.0);
			SetItemFRACTION6_MAX(0.0);
			SetItemFRACTION7_MAX(0.0);
			SetItemFRACTION8_MAX(0.0);
			SetItemFRACTION1_AVE(0.0);
			SetItemFRACTION2_AVE(0.0);
			SetItemFRACTION3_AVE(0.0);
			SetItemFRACTION4_AVE(0.0);
			SetItemFRACTION5_AVE(0.0);
			SetItemFRACTION6_AVE(0.0);
			SetItemFRACTION7_AVE(0.0);
			SetItemFRACTION8_AVE(0.0);
			SetItemFRACTION1_SD(0.0);
			SetItemFRACTION2_SD(0.0);
			SetItemFRACTION3_SD(0.0);
			SetItemFRACTION4_SD(0.0);
			SetItemFRACTION5_SD(0.0);
			SetItemFRACTION6_SD(0.0);
			SetItemFRACTION7_SD(0.0);
			SetItemFRACTION8_SD(0.0);
			SetItemFRACTION1_UNIF(0.0);
			SetItemFRACTION2_UNIF(0.0);
			SetItemFRACTION3_UNIF(0.0);
			SetItemFRACTION4_UNIF(0.0);
			SetItemFRACTION5_UNIF(0.0);
			SetItemFRACTION6_UNIF(0.0);
			SetItemFRACTION7_UNIF(0.0);
			SetItemFRACTION8_UNIF(0.0);
			break;
		case 5:
			SetItemRSRESIST_MIN("0.000E+00");
			SetItemRSRESIST_MAX("0.000E+00");
			SetItemRSRESIST_AVE("0.000E+00");
			SetItemRSRESIST_STD(0.0);
			SetItemRSRESIST_UNIF(0.0);
			SetItemRSSURFACE_MIN("0.000E+00");
			SetItemRSSURFACE_MAX("0.000E+00");
			SetItemRSSURFACE_AVE("0.000E+00");
			SetItemRSSURFACE_STD(0.0);
			SetItemRSSURFACE_UNIF(0.0);
			SetItemRSVOLUME_MIN("0.000E+00");
			SetItemRSVOLUME_MAX("0.000E+00");
			SetItemRSVOLUME_AVE("0.000E+00");
			SetItemRSVOLUME_STD(0.0);
			SetItemRSVOLUME_UNIF(0.0);
			SetItemRSCONDUCT_MIN("0.000E+00");
			SetItemRSCONDUCT_MAX("0.000E+00");
			SetItemRSCONDUCT_AVE("0.000E+00");
			SetItemRSCONDUCT_STD(0.0);
			SetItemRSCONDUCT_UNIF(0.0);
			break;
		case 6:
			SetItemCAANGLE_MIN(0.0);
			SetItemCAANGLE_MAX(0.0);
			SetItemCAANGLE_AVE(0.0);
			SetItemCAANGLE_STD(0.0);
			SetItemCAANGLE_UNIF(0.0);
			SetItemCARADIUS_MIN(0.0);
			SetItemCARADIUS_MAX(0.0);
			SetItemCARADIUS_AVE(0.0);
			SetItemCARADIUS_STD(0.0);
			SetItemCARADIUS_UNIF(0.0);
			SetItemCALIQUID_MIN(0.0);
			SetItemCALIQUID_MAX(0.0);
			SetItemCALIQUID_AVE(0.0);
			SetItemCALIQUID_STD(0.0);
			SetItemCALIQUID_UNIF(0.0);
			break;
		case 101:
// 2009.09.17 K.Matsuo リファレンス・応力測定モード -->
			SetItemSTRMEASMODE(0);
// 2009.09.17 K.Matsuo リファレンス・応力測定モード <--
			SetItemSTRLxValidFLG0(_T("0000000000000000"));
			SetItemSTRLxValidFLG1(_T("0000"));
			SetItemSTRL1_MIN(0.0);
			SetItemSTRL2_MIN(0.0);
			SetItemSTRL3_MIN(0.0);
			SetItemSTRL4_MIN(0.0);
			SetItemSTRL5_MIN(0.0);
			SetItemSTRL6_MIN(0.0);
			SetItemSTRL7_MIN(0.0);
			SetItemSTRL8_MIN(0.0);
			SetItemSTRL9_MIN(0.0);
			SetItemSTRL10_MIN(0.0);
			SetItemSTRL11_MIN(0.0);
			SetItemSTRL12_MIN(0.0);
			SetItemSTRL13_MIN(0.0);
			SetItemSTRL14_MIN(0.0);
			SetItemSTRL15_MIN(0.0);
			SetItemSTRL16_MIN(0.0);
			SetItemSTRL17_MIN(0.0);
			SetItemSTRL18_MIN(0.0);
			SetItemSTRL19_MIN(0.0);
			SetItemSTRL20_MIN(0.0);
			SetItemSTRWA_MIN(0.0);
			SetItemSTRL1_MAX(0.0);
			SetItemSTRL2_MAX(0.0);
			SetItemSTRL3_MAX(0.0);
			SetItemSTRL4_MAX(0.0);
			SetItemSTRL5_MAX(0.0);
			SetItemSTRL6_MAX(0.0);
			SetItemSTRL7_MAX(0.0);
			SetItemSTRL8_MAX(0.0);
			SetItemSTRL9_MAX(0.0);
			SetItemSTRL10_MAX(0.0);
			SetItemSTRL11_MAX(0.0);
			SetItemSTRL12_MAX(0.0);
			SetItemSTRL13_MAX(0.0);
			SetItemSTRL14_MAX(0.0);
			SetItemSTRL15_MAX(0.0);
			SetItemSTRL16_MAX(0.0);
			SetItemSTRL17_MAX(0.0);
			SetItemSTRL18_MAX(0.0);
			SetItemSTRL19_MAX(0.0);
			SetItemSTRL20_MAX(0.0);
			SetItemSTRWA_MAX(0.0);
			SetItemSTRL1_AVE(0.0);
			SetItemSTRL2_AVE(0.0);
			SetItemSTRL3_AVE(0.0);
			SetItemSTRL4_AVE(0.0);
			SetItemSTRL5_AVE(0.0);
			SetItemSTRL6_AVE(0.0);
			SetItemSTRL7_AVE(0.0);
			SetItemSTRL8_AVE(0.0);
			SetItemSTRL9_AVE(0.0);
			SetItemSTRL10_AVE(0.0);
			SetItemSTRL11_AVE(0.0);
			SetItemSTRL12_AVE(0.0);
			SetItemSTRL13_AVE(0.0);
			SetItemSTRL14_AVE(0.0);
			SetItemSTRL15_AVE(0.0);
			SetItemSTRL16_AVE(0.0);
			SetItemSTRL17_AVE(0.0);
			SetItemSTRL18_AVE(0.0);
			SetItemSTRL19_AVE(0.0);
			SetItemSTRL20_AVE(0.0);
			SetItemSTRWA_AVE(0.0);
			SetItemSTRL1_SD(0.0);
			SetItemSTRL2_SD(0.0);
			SetItemSTRL3_SD(0.0);
			SetItemSTRL4_SD(0.0);
			SetItemSTRL5_SD(0.0);
			SetItemSTRL6_SD(0.0);
			SetItemSTRL7_SD(0.0);
			SetItemSTRL8_SD(0.0);
			SetItemSTRL9_SD(0.0);
			SetItemSTRL10_SD(0.0);
			SetItemSTRL11_SD(0.0);
			SetItemSTRL12_SD(0.0);
			SetItemSTRL13_SD(0.0);
			SetItemSTRL14_SD(0.0);
			SetItemSTRL15_SD(0.0);
			SetItemSTRL16_SD(0.0);
			SetItemSTRL17_SD(0.0);
			SetItemSTRL18_SD(0.0);
			SetItemSTRL19_SD(0.0);
			SetItemSTRL20_SD(0.0);
			SetItemSTRWA_SD(0.0);
			SetItemSTRL1_UNIF(0.0);
			SetItemSTRL2_UNIF(0.0);
			SetItemSTRL3_UNIF(0.0);
			SetItemSTRL4_UNIF(0.0);
			SetItemSTRL5_UNIF(0.0);
			SetItemSTRL6_UNIF(0.0);
			SetItemSTRL7_UNIF(0.0);
			SetItemSTRL8_UNIF(0.0);
			SetItemSTRL9_UNIF(0.0);
			SetItemSTRL10_UNIF(0.0);
			SetItemSTRL11_UNIF(0.0);
			SetItemSTRL12_UNIF(0.0);
			SetItemSTRL13_UNIF(0.0);
			SetItemSTRL14_UNIF(0.0);
			SetItemSTRL15_UNIF(0.0);
			SetItemSTRL16_UNIF(0.0);
			SetItemSTRL17_UNIF(0.0);
			SetItemSTRL18_UNIF(0.0);
			SetItemSTRL19_UNIF(0.0);
			SetItemSTRL20_UNIF(0.0);
			SetItemSTRWA_UNIF(0.0);
			break;
		}
	}

	void SetItemPointNo(int iPointNo)
	{
		_stprintf(m_szPointNo, _T("%05d"), iPointNo);
	}

	void SetItemLineNo(int iLineNo)
	{
		SetItemPointNo(iLineNo);
	}

	// FormatId 001,002,003,004,007
	GETSETITEMASCII(0, LAYERFLG)
	GETSETITEMDOUBLE(1, THICK1_MIN)
	GETSETITEMDOUBLE(2, THICK2_MIN)
	GETSETITEMDOUBLE(3, THICK3_MIN)
	GETSETITEMDOUBLE(4, THICK4_MIN)
	GETSETITEMDOUBLE(5, THICK5_MIN)
	GETSETITEMDOUBLE(6, THICK6_MIN)
	GETSETITEMDOUBLE(7, THICK7_MIN)
	GETSETITEMDOUBLE(8, THICK8_MIN)
	GETSETITEMDOUBLE(9, THICK1_MAX)
	GETSETITEMDOUBLE(10, THICK2_MAX)
	GETSETITEMDOUBLE(11, THICK3_MAX)
	GETSETITEMDOUBLE(12, THICK4_MAX)
	GETSETITEMDOUBLE(13, THICK5_MAX)
	GETSETITEMDOUBLE(14, THICK6_MAX)
	GETSETITEMDOUBLE(15, THICK7_MAX)
	GETSETITEMDOUBLE(16, THICK8_MAX)
	GETSETITEMDOUBLE(17, THICK1_AVE)
	GETSETITEMDOUBLE(18, THICK2_AVE)
	GETSETITEMDOUBLE(19, THICK3_AVE)
	GETSETITEMDOUBLE(20, THICK4_AVE)
	GETSETITEMDOUBLE(21, THICK5_AVE)
	GETSETITEMDOUBLE(22, THICK6_AVE)
	GETSETITEMDOUBLE(23, THICK7_AVE)
	GETSETITEMDOUBLE(24, THICK8_AVE)
	GETSETITEMDOUBLE(25, THICK1_SD)
	GETSETITEMDOUBLE(26, THICK2_SD)
	GETSETITEMDOUBLE(27, THICK3_SD)
	GETSETITEMDOUBLE(28, THICK4_SD)
	GETSETITEMDOUBLE(29, THICK5_SD)
	GETSETITEMDOUBLE(30, THICK6_SD)
	GETSETITEMDOUBLE(31, THICK7_SD)
	GETSETITEMDOUBLE(32, THICK8_SD)
	GETSETITEMDOUBLE(33, THICK1_UNIF)
	GETSETITEMDOUBLE(34, THICK2_UNIF)
	GETSETITEMDOUBLE(35, THICK3_UNIF)
	GETSETITEMDOUBLE(36, THICK4_UNIF)
	GETSETITEMDOUBLE(37, THICK5_UNIF)
	GETSETITEMDOUBLE(38, THICK6_UNIF)
	GETSETITEMDOUBLE(39, THICK7_UNIF)
	GETSETITEMDOUBLE(40, THICK8_UNIF)
	GETSETITEMDOUBLE(41, N1_MIN)
	GETSETITEMDOUBLE(42, N2_MIN)
	GETSETITEMDOUBLE(43, N3_MIN)
	GETSETITEMDOUBLE(44, N4_MIN)
	GETSETITEMDOUBLE(45, N5_MIN)
	GETSETITEMDOUBLE(46, N6_MIN)
	GETSETITEMDOUBLE(47, N7_MIN)
	GETSETITEMDOUBLE(48, N8_MIN)
	GETSETITEMDOUBLE(49, N1_MAX)
	GETSETITEMDOUBLE(50, N2_MAX)
	GETSETITEMDOUBLE(51, N3_MAX)
	GETSETITEMDOUBLE(52, N4_MAX)
	GETSETITEMDOUBLE(53, N5_MAX)
	GETSETITEMDOUBLE(54, N6_MAX)
	GETSETITEMDOUBLE(55, N7_MAX)
	GETSETITEMDOUBLE(56, N8_MAX)
	GETSETITEMDOUBLE(57, N1_AVE)
	GETSETITEMDOUBLE(58, N2_AVE)
	GETSETITEMDOUBLE(59, N3_AVE)
	GETSETITEMDOUBLE(60, N4_AVE)
	GETSETITEMDOUBLE(61, N5_AVE)
	GETSETITEMDOUBLE(62, N6_AVE)
	GETSETITEMDOUBLE(63, N7_AVE)
	GETSETITEMDOUBLE(64, N8_AVE)
	GETSETITEMDOUBLE(65, N1_SD)
	GETSETITEMDOUBLE(66, N2_SD)
	GETSETITEMDOUBLE(67, N3_SD)
	GETSETITEMDOUBLE(68, N4_SD)
	GETSETITEMDOUBLE(69, N5_SD)
	GETSETITEMDOUBLE(70, N6_SD)
	GETSETITEMDOUBLE(71, N7_SD)
	GETSETITEMDOUBLE(72, N8_SD)
	GETSETITEMDOUBLE(73, N1_UNIF)
	GETSETITEMDOUBLE(74, N2_UNIF)
	GETSETITEMDOUBLE(75, N3_UNIF)
	GETSETITEMDOUBLE(76, N4_UNIF)
	GETSETITEMDOUBLE(77, N5_UNIF)
	GETSETITEMDOUBLE(78, N6_UNIF)
	GETSETITEMDOUBLE(79, N7_UNIF)
	GETSETITEMDOUBLE(80, N8_UNIF)
	GETSETITEMDOUBLE(81, K1_MIN)
	GETSETITEMDOUBLE(82, K2_MIN)
	GETSETITEMDOUBLE(83, K3_MIN)
	GETSETITEMDOUBLE(84, K4_MIN)
	GETSETITEMDOUBLE(85, K5_MIN)
	GETSETITEMDOUBLE(86, K6_MIN)
	GETSETITEMDOUBLE(87, K7_MIN)
	GETSETITEMDOUBLE(88, K8_MIN)
	GETSETITEMDOUBLE(89, K1_MAX)
	GETSETITEMDOUBLE(90, K2_MAX)
	GETSETITEMDOUBLE(91, K3_MAX)
	GETSETITEMDOUBLE(92, K4_MAX)
	GETSETITEMDOUBLE(93, K5_MAX)
	GETSETITEMDOUBLE(94, K6_MAX)
	GETSETITEMDOUBLE(95, K7_MAX)
	GETSETITEMDOUBLE(96, K8_MAX)
	GETSETITEMDOUBLE(97, K1_AVE)
	GETSETITEMDOUBLE(98, K2_AVE)
	GETSETITEMDOUBLE(99, K3_AVE)
	GETSETITEMDOUBLE(100, K4_AVE)
	GETSETITEMDOUBLE(101, K5_AVE)
	GETSETITEMDOUBLE(102, K6_AVE)
	GETSETITEMDOUBLE(103, K7_AVE)
	GETSETITEMDOUBLE(104, K8_AVE)
	GETSETITEMDOUBLE(105, K1_SD)
	GETSETITEMDOUBLE(106, K2_SD)
	GETSETITEMDOUBLE(107, K3_SD)
	GETSETITEMDOUBLE(108, K4_SD)
	GETSETITEMDOUBLE(109, K5_SD)
	GETSETITEMDOUBLE(110, K6_SD)
	GETSETITEMDOUBLE(111, K7_SD)
	GETSETITEMDOUBLE(112, K8_SD)
	GETSETITEMDOUBLE(113, K1_UNIF)
	GETSETITEMDOUBLE(114, K2_UNIF)
	GETSETITEMDOUBLE(115, K3_UNIF)
	GETSETITEMDOUBLE(116, K4_UNIF)
	GETSETITEMDOUBLE(117, K5_UNIF)
	GETSETITEMDOUBLE(118, K6_UNIF)
	GETSETITEMDOUBLE(119, K7_UNIF)
	GETSETITEMDOUBLE(120, K8_UNIF)
	GETSETITEMDOUBLE(121, CONST1_MIN)
	GETSETITEMDOUBLE(122, CONST2_MIN)
	GETSETITEMDOUBLE(123, CONST3_MIN)
	GETSETITEMDOUBLE(124, CONST4_MIN)
	GETSETITEMDOUBLE(125, CONST5_MIN)
	GETSETITEMDOUBLE(126, CONST6_MIN)
	GETSETITEMDOUBLE(127, CONST7_MIN)
	GETSETITEMDOUBLE(128, CONST8_MIN)
	GETSETITEMDOUBLE(129, CONST1_MAX)
	GETSETITEMDOUBLE(130, CONST2_MAX)
	GETSETITEMDOUBLE(131, CONST3_MAX)
	GETSETITEMDOUBLE(132, CONST4_MAX)
	GETSETITEMDOUBLE(133, CONST5_MAX)
	GETSETITEMDOUBLE(134, CONST6_MAX)
	GETSETITEMDOUBLE(135, CONST7_MAX)
	GETSETITEMDOUBLE(136, CONST8_MAX)
	GETSETITEMDOUBLE(137, CONST1_AVE)
	GETSETITEMDOUBLE(138, CONST2_AVE)
	GETSETITEMDOUBLE(139, CONST3_AVE)
	GETSETITEMDOUBLE(140, CONST4_AVE)
	GETSETITEMDOUBLE(141, CONST5_AVE)
	GETSETITEMDOUBLE(142, CONST6_AVE)
	GETSETITEMDOUBLE(143, CONST7_AVE)
	GETSETITEMDOUBLE(144, CONST8_AVE)
	GETSETITEMDOUBLE(145, CONST1_SD)
	GETSETITEMDOUBLE(146, CONST2_SD)
	GETSETITEMDOUBLE(147, CONST3_SD)
	GETSETITEMDOUBLE(148, CONST4_SD)
	GETSETITEMDOUBLE(149, CONST5_SD)
	GETSETITEMDOUBLE(150, CONST6_SD)
	GETSETITEMDOUBLE(151, CONST7_SD)
	GETSETITEMDOUBLE(152, CONST8_SD)
	GETSETITEMDOUBLE(153, CONST1_UNIF)
	GETSETITEMDOUBLE(154, CONST2_UNIF)
	GETSETITEMDOUBLE(155, CONST3_UNIF)
	GETSETITEMDOUBLE(156, CONST4_UNIF)
	GETSETITEMDOUBLE(157, CONST5_UNIF)
	GETSETITEMDOUBLE(158, CONST6_UNIF)
	GETSETITEMDOUBLE(159, CONST7_UNIF)
	GETSETITEMDOUBLE(160, CONST8_UNIF)
	GETSETITEMDOUBLE(161, REFLECT1_WAVE)
	GETSETITEMDOUBLE(162, REFLECT2_WAVE)
	GETSETITEMDOUBLE(163, REFLECT3_WAVE)
	GETSETITEMDOUBLE(164, REFLECT1_MIN)
	GETSETITEMDOUBLE(165, REFLECT2_MIN)
	GETSETITEMDOUBLE(166, REFLECT3_MIN)
	GETSETITEMDOUBLE(167, REFLECT1_MAX)
	GETSETITEMDOUBLE(168, REFLECT2_MAX)
	GETSETITEMDOUBLE(169, REFLECT3_MAX)
	GETSETITEMDOUBLE(170, REFLECT1_AVE)
	GETSETITEMDOUBLE(171, REFLECT2_AVE)
	GETSETITEMDOUBLE(172, REFLECT3_AVE)
	GETSETITEMDOUBLE(173, REFLECT1_SD)
	GETSETITEMDOUBLE(174, REFLECT2_SD)
	GETSETITEMDOUBLE(175, REFLECT3_SD)
	GETSETITEMDOUBLE(176, REFLECT1_UNIF)
	GETSETITEMDOUBLE(177, REFLECT2_UNIF)
	GETSETITEMDOUBLE(178, REFLECT3_UNIF)
	GETSETITEMDOUBLE(179, TRANSMIT1_WAVE)
	GETSETITEMDOUBLE(180, TRANSMIT2_WAVE)
	GETSETITEMDOUBLE(181, TRANSMIT3_WAVE)
	GETSETITEMDOUBLE(182, TRANSMIT1_MIN)
	GETSETITEMDOUBLE(183, TRANSMIT2_MIN)
	GETSETITEMDOUBLE(184, TRANSMIT3_MIN)
	GETSETITEMDOUBLE(185, TRANSMIT1_MAX)
	GETSETITEMDOUBLE(186, TRANSMIT2_MAX)
	GETSETITEMDOUBLE(187, TRANSMIT3_MAX)
	GETSETITEMDOUBLE(188, TRANSMIT1_AVE)
	GETSETITEMDOUBLE(189, TRANSMIT2_AVE)
	GETSETITEMDOUBLE(190, TRANSMIT3_AVE)
	GETSETITEMDOUBLE(191, TRANSMIT1_SD)
	GETSETITEMDOUBLE(192, TRANSMIT2_SD)
	GETSETITEMDOUBLE(193, TRANSMIT3_SD)
	GETSETITEMDOUBLE(194, TRANSMIT1_UNIF)
	GETSETITEMDOUBLE(195, TRANSMIT2_UNIF)
	GETSETITEMDOUBLE(196, TRANSMIT3_UNIF)
	GETSETITEMDOUBLE(197, EG1_MIN)
	GETSETITEMDOUBLE(198, EG2_MIN)
	GETSETITEMDOUBLE(199, EG3_MIN)
	GETSETITEMDOUBLE(200, EG4_MIN)
	GETSETITEMDOUBLE(201, EG5_MIN)
	GETSETITEMDOUBLE(202, EG6_MIN)
	GETSETITEMDOUBLE(203, EG7_MIN)
	GETSETITEMDOUBLE(204, EG8_MIN)
	GETSETITEMDOUBLE(205, EG1_MAX)
	GETSETITEMDOUBLE(206, EG2_MAX)
	GETSETITEMDOUBLE(207, EG3_MAX)
	GETSETITEMDOUBLE(208, EG4_MAX)
	GETSETITEMDOUBLE(209, EG5_MAX)
	GETSETITEMDOUBLE(210, EG6_MAX)
	GETSETITEMDOUBLE(211, EG7_MAX)
	GETSETITEMDOUBLE(212, EG8_MAX)
	GETSETITEMDOUBLE(213, EG1_AVE)
	GETSETITEMDOUBLE(214, EG2_AVE)
	GETSETITEMDOUBLE(215, EG3_AVE)
	GETSETITEMDOUBLE(216, EG4_AVE)
	GETSETITEMDOUBLE(217, EG5_AVE)
	GETSETITEMDOUBLE(218, EG6_AVE)
	GETSETITEMDOUBLE(219, EG7_AVE)
	GETSETITEMDOUBLE(220, EG8_AVE)
	GETSETITEMDOUBLE(221, EG1_SD)
	GETSETITEMDOUBLE(222, EG2_SD)
	GETSETITEMDOUBLE(223, EG3_SD)
	GETSETITEMDOUBLE(224, EG4_SD)
	GETSETITEMDOUBLE(225, EG5_SD)
	GETSETITEMDOUBLE(226, EG6_SD)
	GETSETITEMDOUBLE(227, EG7_SD)
	GETSETITEMDOUBLE(228, EG8_SD)
	GETSETITEMDOUBLE(229, EG1_UNIF)
	GETSETITEMDOUBLE(230, EG2_UNIF)
	GETSETITEMDOUBLE(231, EG3_UNIF)
	GETSETITEMDOUBLE(232, EG4_UNIF)
	GETSETITEMDOUBLE(233, EG5_UNIF)
	GETSETITEMDOUBLE(234, EG6_UNIF)
	GETSETITEMDOUBLE(235, EG7_UNIF)
	GETSETITEMDOUBLE(236, EG8_UNIF)
	GETSETITEMDOUBLE(237, FRACTION1_MIN)
	GETSETITEMDOUBLE(238, FRACTION2_MIN)
	GETSETITEMDOUBLE(239, FRACTION3_MIN)
	GETSETITEMDOUBLE(240, FRACTION4_MIN)
	GETSETITEMDOUBLE(241, FRACTION5_MIN)
	GETSETITEMDOUBLE(242, FRACTION6_MIN)
	GETSETITEMDOUBLE(243, FRACTION7_MIN)
	GETSETITEMDOUBLE(244, FRACTION8_MIN)
	GETSETITEMDOUBLE(245, FRACTION1_MAX)
	GETSETITEMDOUBLE(246, FRACTION2_MAX)
	GETSETITEMDOUBLE(247, FRACTION3_MAX)
	GETSETITEMDOUBLE(248, FRACTION4_MAX)
	GETSETITEMDOUBLE(249, FRACTION5_MAX)
	GETSETITEMDOUBLE(250, FRACTION6_MAX)
	GETSETITEMDOUBLE(251, FRACTION7_MAX)
	GETSETITEMDOUBLE(252, FRACTION8_MAX)
	GETSETITEMDOUBLE(253, FRACTION1_AVE)
	GETSETITEMDOUBLE(254, FRACTION2_AVE)
	GETSETITEMDOUBLE(255, FRACTION3_AVE)
	GETSETITEMDOUBLE(256, FRACTION4_AVE)
	GETSETITEMDOUBLE(257, FRACTION5_AVE)
	GETSETITEMDOUBLE(258, FRACTION6_AVE)
	GETSETITEMDOUBLE(259, FRACTION7_AVE)
	GETSETITEMDOUBLE(260, FRACTION8_AVE)
	GETSETITEMDOUBLE(261, FRACTION1_SD)
	GETSETITEMDOUBLE(262, FRACTION2_SD)
	GETSETITEMDOUBLE(263, FRACTION3_SD)
	GETSETITEMDOUBLE(264, FRACTION4_SD)
	GETSETITEMDOUBLE(265, FRACTION5_SD)
	GETSETITEMDOUBLE(266, FRACTION6_SD)
	GETSETITEMDOUBLE(267, FRACTION7_SD)
	GETSETITEMDOUBLE(268, FRACTION8_SD)
	GETSETITEMDOUBLE(269, FRACTION1_UNIF)
	GETSETITEMDOUBLE(270, FRACTION2_UNIF)
	GETSETITEMDOUBLE(271, FRACTION3_UNIF)
	GETSETITEMDOUBLE(272, FRACTION4_UNIF)
	GETSETITEMDOUBLE(273, FRACTION5_UNIF)
	GETSETITEMDOUBLE(274, FRACTION6_UNIF)
	GETSETITEMDOUBLE(275, FRACTION7_UNIF)
	GETSETITEMDOUBLE(276, FRACTION8_UNIF)

	// FormatId 005
	GETSETITEMASCII(0, RSRESIST_MIN)
	GETSETITEMASCII(1, RSRESIST_MAX)
	GETSETITEMASCII(2, RSRESIST_AVE)
	GETSETITEMDOUBLE(3, RSRESIST_STD)
	GETSETITEMDOUBLE(4, RSRESIST_UNIF)
	GETSETITEMASCII(5, RSSURFACE_MIN)
	GETSETITEMASCII(6, RSSURFACE_MAX)
	GETSETITEMASCII(7, RSSURFACE_AVE)
	GETSETITEMDOUBLE(8, RSSURFACE_STD)
	GETSETITEMDOUBLE(9, RSSURFACE_UNIF)
	GETSETITEMASCII(10, RSVOLUME_MIN)
	GETSETITEMASCII(11, RSVOLUME_MAX)
	GETSETITEMASCII(12, RSVOLUME_AVE)
	GETSETITEMDOUBLE(13, RSVOLUME_STD)
	GETSETITEMDOUBLE(14, RSVOLUME_UNIF)
	GETSETITEMASCII(15, RSCONDUCT_MIN)
	GETSETITEMASCII(16, RSCONDUCT_MAX)
	GETSETITEMASCII(17, RSCONDUCT_AVE)
	GETSETITEMDOUBLE(18, RSCONDUCT_STD)
	GETSETITEMDOUBLE(19, RSCONDUCT_UNIF)

	// FormatId 006
	GETSETITEMDOUBLE(0, CAANGLE_MIN)
	GETSETITEMDOUBLE(1, CAANGLE_MAX)
	GETSETITEMDOUBLE(2, CAANGLE_AVE)
	GETSETITEMDOUBLE(3, CAANGLE_STD)
	GETSETITEMDOUBLE(4, CAANGLE_UNIF)
	GETSETITEMDOUBLE(5, CARADIUS_MIN)
	GETSETITEMDOUBLE(6, CARADIUS_MAX)
	GETSETITEMDOUBLE(7, CARADIUS_AVE)
	GETSETITEMDOUBLE(8, CARADIUS_STD)
	GETSETITEMDOUBLE(9, CARADIUS_UNIF)
	GETSETITEMDOUBLE(10, CALIQUID_MIN)
	GETSETITEMDOUBLE(11, CALIQUID_MAX)
	GETSETITEMDOUBLE(12, CALIQUID_AVE)
	GETSETITEMDOUBLE(13, CALIQUID_STD)
	GETSETITEMDOUBLE(14, CALIQUID_UNIF)

	// FormatId 101
// 2009.09.17 K.Matsuo リファレンス・応力測定モード -->
	GETSETITEMINT(0, STRMEASMODE)
// 2009.09.17 K.Matsuo リファレンス・応力測定モード <--
	GETSETITEMASCII(1, STRLxValidFLG0)
	GETSETITEMASCII(2, STRLxValidFLG1)
	GETSETITEMDOUBLE(3, STRL1_MIN)
	GETSETITEMDOUBLE(4, STRL2_MIN)
	GETSETITEMDOUBLE(5, STRL3_MIN)
	GETSETITEMDOUBLE(6, STRL4_MIN)
	GETSETITEMDOUBLE(7, STRL5_MIN)
	GETSETITEMDOUBLE(8, STRL6_MIN)
	GETSETITEMDOUBLE(9, STRL7_MIN)
	GETSETITEMDOUBLE(10, STRL8_MIN)
	GETSETITEMDOUBLE(11, STRL9_MIN)
	GETSETITEMDOUBLE(12, STRL10_MIN)
	GETSETITEMDOUBLE(13, STRL11_MIN)
	GETSETITEMDOUBLE(14, STRL12_MIN)
	GETSETITEMDOUBLE(15, STRL13_MIN)
	GETSETITEMDOUBLE(16, STRL14_MIN)
	GETSETITEMDOUBLE(17, STRL15_MIN)
	GETSETITEMDOUBLE(18, STRL16_MIN)
	GETSETITEMDOUBLE(19, STRL17_MIN)
	GETSETITEMDOUBLE(20, STRL18_MIN)
	GETSETITEMDOUBLE(21, STRL19_MIN)
	GETSETITEMDOUBLE(22, STRL20_MIN)
	GETSETITEMDOUBLE(23, STRWA_MIN)
	GETSETITEMDOUBLE(24, STRL1_MAX)
	GETSETITEMDOUBLE(25, STRL2_MAX)
	GETSETITEMDOUBLE(26, STRL3_MAX)
	GETSETITEMDOUBLE(27, STRL4_MAX)
	GETSETITEMDOUBLE(28, STRL5_MAX)
	GETSETITEMDOUBLE(29, STRL6_MAX)
	GETSETITEMDOUBLE(30, STRL7_MAX)
	GETSETITEMDOUBLE(31, STRL8_MAX)
	GETSETITEMDOUBLE(32, STRL9_MAX)
	GETSETITEMDOUBLE(33, STRL10_MAX)
	GETSETITEMDOUBLE(34, STRL11_MAX)
	GETSETITEMDOUBLE(35, STRL12_MAX)
	GETSETITEMDOUBLE(36, STRL13_MAX)
	GETSETITEMDOUBLE(37, STRL14_MAX)
	GETSETITEMDOUBLE(38, STRL15_MAX)
	GETSETITEMDOUBLE(39, STRL16_MAX)
	GETSETITEMDOUBLE(40, STRL17_MAX)
	GETSETITEMDOUBLE(41, STRL18_MAX)
	GETSETITEMDOUBLE(42, STRL19_MAX)
	GETSETITEMDOUBLE(43, STRL20_MAX)
	GETSETITEMDOUBLE(44, STRWA_MAX)
	GETSETITEMDOUBLE(45, STRL1_AVE)
	GETSETITEMDOUBLE(46, STRL2_AVE)
	GETSETITEMDOUBLE(47, STRL3_AVE)
	GETSETITEMDOUBLE(48, STRL4_AVE)
	GETSETITEMDOUBLE(49, STRL5_AVE)
	GETSETITEMDOUBLE(50, STRL6_AVE)
	GETSETITEMDOUBLE(51, STRL7_AVE)
	GETSETITEMDOUBLE(52, STRL8_AVE)
	GETSETITEMDOUBLE(53, STRL9_AVE)
	GETSETITEMDOUBLE(54, STRL10_AVE)
	GETSETITEMDOUBLE(55, STRL11_AVE)
	GETSETITEMDOUBLE(56, STRL12_AVE)
	GETSETITEMDOUBLE(57, STRL13_AVE)
	GETSETITEMDOUBLE(58, STRL14_AVE)
	GETSETITEMDOUBLE(59, STRL15_AVE)
	GETSETITEMDOUBLE(60, STRL16_AVE)
	GETSETITEMDOUBLE(61, STRL17_AVE)
	GETSETITEMDOUBLE(62, STRL18_AVE)
	GETSETITEMDOUBLE(63, STRL19_AVE)
	GETSETITEMDOUBLE(64, STRL20_AVE)
	GETSETITEMDOUBLE(65, STRWA_AVE)
	GETSETITEMDOUBLE(66, STRL1_SD)
	GETSETITEMDOUBLE(67, STRL2_SD)
	GETSETITEMDOUBLE(68, STRL3_SD)
	GETSETITEMDOUBLE(69, STRL4_SD)
	GETSETITEMDOUBLE(70, STRL5_SD)
	GETSETITEMDOUBLE(71, STRL6_SD)
	GETSETITEMDOUBLE(72, STRL7_SD)
	GETSETITEMDOUBLE(73, STRL8_SD)
	GETSETITEMDOUBLE(74, STRL9_SD)
	GETSETITEMDOUBLE(75, STRL10_SD)
	GETSETITEMDOUBLE(76, STRL11_SD)
	GETSETITEMDOUBLE(77, STRL12_SD)
	GETSETITEMDOUBLE(78, STRL13_SD)
	GETSETITEMDOUBLE(79, STRL14_SD)
	GETSETITEMDOUBLE(80, STRL15_SD)
	GETSETITEMDOUBLE(81, STRL16_SD)
	GETSETITEMDOUBLE(82, STRL17_SD)
	GETSETITEMDOUBLE(83, STRL18_SD)
	GETSETITEMDOUBLE(84, STRL19_SD)
	GETSETITEMDOUBLE(85, STRL20_SD)
	GETSETITEMDOUBLE(86, STRWA_SD)
	GETSETITEMDOUBLE(87, STRL1_UNIF)
	GETSETITEMDOUBLE(88, STRL2_UNIF)
	GETSETITEMDOUBLE(89, STRL3_UNIF)
	GETSETITEMDOUBLE(90, STRL4_UNIF)
	GETSETITEMDOUBLE(91, STRL5_UNIF)
	GETSETITEMDOUBLE(92, STRL6_UNIF)
	GETSETITEMDOUBLE(93, STRL7_UNIF)
	GETSETITEMDOUBLE(94, STRL8_UNIF)
	GETSETITEMDOUBLE(95, STRL9_UNIF)
	GETSETITEMDOUBLE(96, STRL10_UNIF)
	GETSETITEMDOUBLE(97, STRL11_UNIF)
	GETSETITEMDOUBLE(98, STRL12_UNIF)
	GETSETITEMDOUBLE(99, STRL13_UNIF)
	GETSETITEMDOUBLE(100, STRL14_UNIF)
	GETSETITEMDOUBLE(101, STRL15_UNIF)
	GETSETITEMDOUBLE(102, STRL16_UNIF)
	GETSETITEMDOUBLE(103, STRL17_UNIF)
	GETSETITEMDOUBLE(104, STRL18_UNIF)
	GETSETITEMDOUBLE(105, STRL19_UNIF)
	GETSETITEMDOUBLE(106, STRL20_UNIF)
	GETSETITEMDOUBLE(107, STRWA_UNIF)

	int GetItemFormatId() const
	{
		TCHAR szFormatId[FORMATIDLEN + 1];

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szFormatId, &m_pszRequMessage[nPos], FORMATIDLEN);
		szFormatId[FORMATIDLEN] = _TCHAR('\0');
		return _ttoi(szFormatId);
	}

	int GetItemPointNo() const
	{
		TCHAR szPointNo[POINTNOLEN + 1];

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + FORMATIDLEN;
		_tcsncpy(szPointNo, &m_pszRequMessage[nPos], POINTNOLEN);
		szPointNo[POINTNOLEN] = _TCHAR('\0');
		return _ttoi(szPointNo);
	}

	int GetItemLineNo() const
	{
		return GetItemPointNo();
	}

	void SendRequ()
	{
		CString strMsg;

		if ( _tcscmp(m_szFormatId, _T("001")) == 0 ||
			 _tcscmp(m_szFormatId, _T("002")) == 0 ||
			 _tcscmp(m_szFormatId, _T("003")) == 0 ||
			 _tcscmp(m_szFormatId, _T("004")) == 0 ||
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			 _tcscmp(m_szFormatId, _T("007")) == 0 ) {
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
			for ( int i = 0; i < PROCESSDATAOCCURENCEMAX277; i++ )
				AddProcessDataItem(i, strMsg);
		}
		else if ( _tcscmp(m_szFormatId, _T("005")) == 0 ) {
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
			for ( int i = 0; i < PROCESSDATAOCCURENCEMAX20; i++ )
				AddProcessDataItem(i, strMsg);
		}
		else if ( _tcscmp(m_szFormatId, _T("006")) == 0 ) {
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
			for ( int i = 0; i < PROCESSDATAOCCURENCEMAX15; i++ )
				AddProcessDataItem(i, strMsg);
		}
		else if ( _tcscmp(m_szFormatId, _T("101")) == 0 ) {
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
			for ( int i = 0; i < PROCESSDATAOCCURENCEMAX108; i++ )
				AddProcessDataItem(i, strMsg);
		}
		else {
			ASSERT( FALSE );
		}
		CPifMessage::SendRequ(_T("P503"), strMsg, strMsg.GetLength());
	}

protected:
	void AddProcessDataItem(int i, CString& strMsg)
	{
		char szBuff[256];
		strMsg += stProcessDataSet[i].chVarType;
		sprintf(szBuff, "%-16.16s", stProcessDataSet[i].szDvName);
		strMsg += szBuff;
		sprintf(szBuff, "%-16.16s", stProcessDataSet[i].szDvValue);
		strMsg += szBuff;
	}

private:
	TCHAR m_szFormatId[FORMATIDLEN + 1];
	TCHAR m_szPointNo[POINTNOLEN + 1];
	PROCESSDATASET stProcessDataSet[PROCESSDATAOCCURENCEMAX277];
};

// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
//---------------------------------------------------------------------------
// CPifMessage511 リファレンス取得開始指示要求
class CPifMessage511 : public CPifMessage
{
public:
	enum { SAMPLEIDLEN = 32,
		   LOTIDLEN = 32 };			// Add Item 2009.10.29

	void SetItemSampleId(LPCTSTR pszSampleId)
	{
		sprintf(m_szSampleId, "%-32.32s", pszSampleId);
	}

	void SetItemLotId(LPCTSTR pszLotId)
	{
		sprintf(m_szLotId, "%-32.32s", pszLotId);
	}

	void SendRequ()
	{
		CString strMsg;

		strMsg = m_szSampleId;
		strMsg += m_szLotId;

		CPifMessage::SendRequ(_T("P511"), strMsg, strMsg.GetLength());
	}

	void GetItemSampleId(LPTSTR pszSampleId) const
	{
		char szSampleId[_MAX_FNAME];
		CString strSampleId;

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szSampleId, &m_pszRequMessage[nPos], SAMPLEIDLEN);
		szSampleId[SAMPLEIDLEN] = _TCHAR('\0');
		strSampleId = szSampleId;
		strSampleId.TrimRight();
		strcpy(pszSampleId, strSampleId);
	}

	void GetItemLotId(LPTSTR pszLotId) const
	{
		char szLotId[_MAX_FNAME];
		CString strLotId;

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + SAMPLEIDLEN;

		// Check Message Version
		if ( m_iRecvLength < nPos || m_pszRequMessage[nPos] == CR ) {
			// old version
			strcpy(pszLotId, "");
			return;
		}

		_tcsncpy(szLotId, &m_pszRequMessage[nPos], LOTIDLEN);
		szLotId[LOTIDLEN] = _TCHAR('\0');
		strLotId = szLotId;
		strLotId.TrimRight();
		strcpy(pszLotId, strLotId);
	}

private:
	TCHAR m_szSampleId[SAMPLEIDLEN + 1];
	TCHAR m_szLotId[LOTIDLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage512
class CPifMessage512 : public CPifMessage
{
public:
	enum { FORMATIDLEN = 03,
		   POINTNOLEN  = 05,};

	CPifMessage512()
	{
		// 001:汎用コード
		SetItemFormatId(1);
	}

	void SetItemPointNo(int iPointNo)
	{
		_stprintf(m_szPointNo, _T("%05d"), iPointNo);
	}

	int GetItemFormatId() const
	{
		TCHAR szFormatId[FORMATIDLEN + 1];

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szFormatId, &m_pszRequMessage[nPos], FORMATIDLEN);
		szFormatId[FORMATIDLEN] = _TCHAR('\0');
		return _ttoi(szFormatId);
	}

	int GetItemPointNo() const
	{
		TCHAR szPointNo[POINTNOLEN + 1];

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + FORMATIDLEN;
		_tcsncpy(szPointNo, &m_pszRequMessage[nPos], POINTNOLEN);
		szPointNo[POINTNOLEN] = _TCHAR('\0');
		return _ttoi(szPointNo);
	}

	void SendRequ()
	{
		CString strMsg;

		if ( _tcscmp(m_szFormatId, _T("001")) == 0 ) {
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
		}
		else {
			ASSERT( FALSE );
		}
		CPifMessage::SendRequ(_T("P512"), strMsg, strMsg.GetLength());
	}

private:
	// for the future.
	void SetItemFormatId(int iFormatId)
	{
		_stprintf(m_szFormatId, _T("%03d"), iFormatId);

	}


private:
	TCHAR m_szFormatId[FORMATIDLEN + 1];
	TCHAR m_szPointNo[POINTNOLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage513
class CPifMessage513 : public CPifMessage
{
public:
	enum { FORMATIDLEN = 03,
		   POINTNOLEN  = 05,};

	CPifMessage513()
	{
		// 001:汎用コード
		SetItemFormatId(1);
	}

	void SetItemPointNo(int iPointNo)
	{
		_stprintf(m_szPointNo, _T("%05d"), iPointNo);
	}

	int GetItemFormatId() const
	{
		TCHAR szFormatId[FORMATIDLEN + 1];

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szFormatId, &m_pszRequMessage[nPos], FORMATIDLEN);
		szFormatId[FORMATIDLEN] = _TCHAR('\0');
		return _ttoi(szFormatId);
	}

	int GetItemPointNo() const
	{
		TCHAR szPointNo[POINTNOLEN + 1];

		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + FORMATIDLEN;
		_tcsncpy(szPointNo, &m_pszRequMessage[nPos], POINTNOLEN);
		szPointNo[POINTNOLEN] = _TCHAR('\0');
		return _ttoi(szPointNo);
	}

	void SendRequ()
	{
		CString strMsg;

		if ( _tcscmp(m_szFormatId, _T("001")) == 0 ) {
			strMsg = m_szFormatId;
			strMsg += m_szPointNo;
		}
		else {
			ASSERT( FALSE );
		}
		CPifMessage::SendRequ(_T("P513"), strMsg, strMsg.GetLength());
	}

private:
	// for the future.
	void SetItemFormatId(int iFormatId)
	{
		_stprintf(m_szFormatId, _T("%03d"), iFormatId);
	}

private:
	TCHAR m_szFormatId[FORMATIDLEN + 1];
	TCHAR m_szPointNo[POINTNOLEN + 1];
};
// 2009.10.29 K.Matsuo リファレンス取得機能追加 <--

//---------------------------------------------------------------------------
// CPifMessage504
class CPifMessage504 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P504"));
	}
};

// 2014.01.07 bagus Add(Stage None対応) -->
//---------------------------------------------------------------------------
// CPifMessage514
class CPifMessage514 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P514"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage515
class CPifMessage515 : public CPifMessage
{
public:
	enum { REFERENCERESULTLEN = 02 };

	void SetItemReferenceResult(int iReferenceResult)
	{
		_stprintf(m_szReferenceResult, _T("%02d"), iReferenceResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P515"), m_szReferenceResult, _tcslen(m_szReferenceResult));
	}

	int GetItemReferenceResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], REFERENCERESULTLEN);
		szBuff[REFERENCERESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szReferenceResult[REFERENCERESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage516
class CPifMessage516 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P516"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage517
class CPifMessage517 : public CPifMessage
{
public:
	enum { REFERENCERESULTLEN = 02 };

	void SetItemReferenceResult(int iReferenceResult)
	{
		_stprintf(m_szReferenceResult, _T("%02d"), iReferenceResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P517"), m_szReferenceResult, _tcslen(m_szReferenceResult));
	}

	int GetItemReferenceResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], REFERENCERESULTLEN);
		szBuff[REFERENCERESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szReferenceResult[REFERENCERESULTLEN + 1];
};
// 2014.01.07 bagus Add(Stage None対応) <--

//---------------------------------------------------------------------------
// CPifMessage601
class CPifMessage601 : public CPifMessage
{
public:
	enum { SAMPLESIZEXLEN = 16,
		   SAMPLESIZEYLEN = 16 };

	void SetItemSampleSizeX(double dX)
	{
		_stprintf(m_szSampleSizeX, _T("%.3lf"), dX);
	}

	void SetItemSampleSizeY(double dY)
	{
		_stprintf(m_szSampleSizeY, _T("%.3lf"), dY);
	}

	void SendRequ()
	{
		TCHAR szBuff[256];
		_stprintf(szBuff, _T("%-16.16s%-16.16s"), m_szSampleSizeX, m_szSampleSizeY);
		CPifMessage::SendRequ(_T("P601"), szBuff, _tcslen(szBuff));
	}

	double GetItemSampleSizeX() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], SAMPLESIZEXLEN);
		szBuff[SAMPLESIZEXLEN] = _TCHAR('\0');
		return atof(szBuff);
	}

	double GetItemSampleSizeY() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + SAMPLESIZEXLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], SAMPLESIZEYLEN);
		szBuff[SAMPLESIZEYLEN] = _TCHAR('\0');
		return atof(szBuff);
	}

private:
	TCHAR m_szSampleSizeX[SAMPLESIZEXLEN + 1];
	TCHAR m_szSampleSizeY[SAMPLESIZEYLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage602
class CPifMessage602 : public CPifMessage
{
public:
	enum { MOVEPOSIDLEN = 2 };

	void SetItemMovePosId(int iMovePosId)
	{
		_stprintf(m_szMovePosId, "%02d", iMovePosId);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P602"), m_szMovePosId, _tcslen(m_szMovePosId));
	}

	int GetItemMovePosId() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEPOSIDLEN);
		szBuff[MOVEPOSIDLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovePosId[MOVEPOSIDLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage603
class CPifMessage603 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P603"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage604
class CPifMessage604 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P604"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage605
class CPifMessage605 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P605"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage606
class CPifMessage606 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P606"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage607
class CPifMessage607 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P607"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage608
class CPifMessage608 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P608"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage609
class CPifMessage609 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P609"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage610
class CPifMessage610 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P610"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage611
class CPifMessage611 : public CPifMessage
{
public:
	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P611"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage701
class CPifMessage701 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P701"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage702
class CPifMessage702 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P702"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage703
class CPifMessage703 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P703"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage704
class CPifMessage704 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P704"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage705
class CPifMessage705 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P705"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage706
class CPifMessage706 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P706"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage707
class CPifMessage707 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P707"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage708
class CPifMessage708 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P708"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage709
class CPifMessage709 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P709"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage710
class CPifMessage710 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P710"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage711
class CPifMessage711 : public CPifMessage
{
public:
	enum { MOVEMENTRESULTLEN = 02 };

	void SetItemMovementResult(int iMovementResult)
	{
		_stprintf(m_szMovementResult, _T("%02d"), iMovementResult);
	}

	void SendRequ()
	{
		CPifMessage::SendRequ(_T("P711"), m_szMovementResult, _tcslen(m_szMovementResult));
	}

	int GetItemMovementResult() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], MOVEMENTRESULTLEN);
		szBuff[MOVEMENTRESULTLEN] = _TCHAR('\0');
		return _ttoi(szBuff);
	}

private:
	TCHAR m_szMovementResult[MOVEMENTRESULTLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage801
class CPifMessage801 : public CPifMessage
{
public:
	enum { ALARMEVENTTYPELEN = 01,
		   ALARMCODELEN 	 = 04,
		   ALARMIDLEN		 = 04,
		   ALARMLEVELLEN	 = 01,
		   ALARMMESSAGELEN	 = 80 };

	void SetItemAlarmEventType(TCHAR chAlarmEventType)
	{
		m_chAlarmEventType = chAlarmEventType;
	}

	void SetItemAlarmCode(int iAlarmCode)
	{
		_stprintf(m_szAlarmCode, _T("%04d"), iAlarmCode);
	}

	void SetItemAlarmId(int iAlarmId)
	{
		_stprintf(m_szAlarmId, _T("%04d"), iAlarmId);
	}

	void SetItemAlarmLevel(TCHAR chAlarmLevel)
	{
		m_chAlarmLevel = chAlarmLevel;
	}

	void SetItemAlarmMessage(LPCTSTR pszAlarmMessage)
	{
		sprintf(m_szAlarmMessage, "%-80.80s", pszAlarmMessage);
	}

	void SendRequ()
	{
		TCHAR szBuff[256];
		_stprintf(szBuff, _T("%c%s%s%c%s"), m_chAlarmEventType, m_szAlarmCode, m_szAlarmId, m_chAlarmLevel, m_szAlarmMessage);
		CPifMessage::SendRequ(_T("P801"), szBuff, _tcslen(szBuff));
	}

	TCHAR GetItemAlarmEventType() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN;
		return m_pszRequMessage[nPos];
	}

	int GetItemAlarmCode() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + ALARMEVENTTYPELEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], ALARMCODELEN);
		szBuff[ALARMCODELEN] = _TCHAR('\0');
		return _ttoi(szBuff);

	}

	int GetItemAlarmId() const
	{
		TCHAR szBuff[256];
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + ALARMEVENTTYPELEN + ALARMCODELEN;
		_tcsncpy(szBuff, &m_pszRequMessage[nPos], ALARMIDLEN);
		szBuff[ALARMIDLEN] = _TCHAR('\0');
		return _ttoi(szBuff);

	}

	TCHAR GetItemAlarmLevel() const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + ALARMEVENTTYPELEN + ALARMCODELEN + ALARMIDLEN;
		return m_pszRequMessage[nPos];
	}

	void GetItemAlarmMessage(LPTSTR pszAlarmMessage) const
	{
		int nPos = ENQLEN + MESSAGELENLEN + COMMANDLEN + ALARMEVENTTYPELEN + ALARMCODELEN + ALARMIDLEN + ALARMLEVELLEN;
		_tcsncpy(pszAlarmMessage, &m_pszRequMessage[nPos], ALARMMESSAGELEN);
		pszAlarmMessage[ALARMMESSAGELEN] = _TCHAR('\0');
	}

private:
	TCHAR m_chAlarmEventType;
	TCHAR m_szAlarmCode[ALARMCODELEN + 1];
	TCHAR m_szAlarmId[ALARMIDLEN + 1];
	TCHAR m_chAlarmLevel;
	TCHAR m_szAlarmMessage[ALARMMESSAGELEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage901 未登録コマンド
class CPifMessage901 : public CPifMessage
{
public:
	void SetItemIllegalCommandId(LPCTSTR pszIllegalCommandId)
	{
		_tcscpy(m_szRecvCommandId, pszIllegalCommandId);
	}

	void Send()
	{
		_tcscpy(m_szCommandId, _T("P901"));
		_stprintf(m_szRespMessage, _T("%c%05X%s%s%c"), ENQ, COMMANDLEN + COMMANDLEN, m_szCommandId, m_szRecvCommandId, CR);
		AddP9xxMessage();
	}

private:
	TCHAR m_szRecvCommandId[COMMANDLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage902 データレングスエラー
class CPifMessage902 : public CPifMessage
{
public:
	void SetItemRecvCommandId(LPCTSTR pszRecvCommandId)
	{
		_tcscpy(m_szRecvCommandId, pszRecvCommandId);
	}

	void Send()
	{
		_tcscpy(m_szCommandId, _T("P902"));
		_stprintf(m_szRespMessage, _T("%c%05X%s%s%c"), ENQ, COMMANDLEN + COMMANDLEN, m_szCommandId, m_szRecvCommandId, CR);
		AddP9xxMessage();
	}
private:
	TCHAR m_szRecvCommandId[COMMANDLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage903 トランザクションタイマタイムアウト
class CPifMessage903 : public CPifMessage
{
public:
	void SetItemSendCommandId(LPCTSTR pszSendCommandId)
	{
		_tcscpy(m_szSendCommandId, pszSendCommandId);
	}

	void Send()
	{
		_tcscpy(m_szCommandId, _T("P903"));
		_stprintf(m_szRespMessage, _T("%c%05X%s%s%c"), ENQ, COMMANDLEN + COMMANDLEN, m_szCommandId, m_szSendCommandId, CR);
		AddP9xxMessage();
	}
private:
	TCHAR m_szSendCommandId[COMMANDLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage904 会話タイマタイムアウト
class CPifMessage904 : public CPifMessage
{
public:
	void SetItemRecvCommandId(LPCTSTR pszRecvCommandId)
	{
		_tcscpy(m_szRecvCommandId, pszRecvCommandId);
	}

	void Send()
	{
		_tcscpy(m_szCommandId, _T("P904"));
		_stprintf(m_szRespMessage, _T("%c%05X%s%s%c"), ENQ, COMMANDLEN + COMMANDLEN, m_szCommandId, m_szRecvCommandId, CR);
		AddP9xxMessage();
	}
private:
	TCHAR m_szRecvCommandId[COMMANDLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage905 シーケンスエラー
class CPifMessage905 : public CPifMessage
{
public:
	void SetItemRecvCommandId(LPCTSTR pszRecvCommandId)
	{
		_tcscpy(m_szRecvCommandId, pszRecvCommandId);
	}

	void Send()
	{
		_tcscpy(m_szCommandId, _T("P905"));
		_stprintf(m_szRespMessage, _T("%c%05X%s%s%c"), ENQ, COMMANDLEN + COMMANDLEN, m_szCommandId, m_szRecvCommandId, CR);
		AddP9xxMessage();
	}
private:
	TCHAR m_szRecvCommandId[COMMANDLEN + 1];
};

//---------------------------------------------------------------------------
// CPifMessage997 データレングス不正（デバッグ確認用）
class CPifMessage996 : public CPifMessage
{
public:
	void Send()
	{
		LPCTSTR pszCommandId = _T("P996");
		LPCTSTR pszMsgBody = _T("Hellow World");
		int count = _tcslen(_T("Hellow World"));
		m_pszRequMessage = new TCHAR[ENQLEN + MESSAGELENLEN + COMMANDLEN + count + CRLEN + 1];
		count += 20;	// *****
		_stprintf(m_pszRequMessage, _T("%c%05X%s%s%c"), ENQ, COMMANDLEN + count, pszCommandId, pszMsgBody, CR);
		AddRequMessage();
	}
};

//---------------------------------------------------------------------------
// CPifMessage997 データレングス不正（デバッグ確認用）
class CPifMessage997 : public CPifMessage
{
public:
	void Send()
	{
		LPCTSTR pszCommandId = _T("P997");
		LPCTSTR pszMsgBody = _T("Hellow World");
		int count = _tcslen(_T("Hellow World"));
		m_pszRequMessage = new TCHAR[ENQLEN + MESSAGELENLEN + COMMANDLEN + count + CRLEN + 1];
		count -= 5; 	// *****
		_stprintf(m_pszRequMessage, _T("%c%05X%s%s%c"), ENQ, COMMANDLEN + count, pszCommandId, pszMsgBody, CR);
		AddRequMessage();
	}
};

//---------------------------------------------------------------------------
// CPifMessage998 トランザクションタイマタイムアウトコマンド（デバッグ確認用）
class CPifMessage998 : public CPifMessage
{
public:
	void Send()
	{
		CPifMessage::SendRequ(_T("P998"));
	}
};

//---------------------------------------------------------------------------
// CPifMessage999 不正コマンド（デバッグ確認用）
class CPifMessage999 : public CPifMessage
{
public:
	void Send()
	{
		CPifMessage::SendRequ(_T("P999"));
	}
};
//
