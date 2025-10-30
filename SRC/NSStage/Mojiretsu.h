#pragma once

class CMojiretsu
{
public:
	CMojiretsu() {}
	~CMojiretsu() {}

	void SetResourceHandle(HINSTANCE hInstance)
	{
		m_hInstance = hInstance;
	}

	CMojiretsu& LoadString(UINT nID)
	{
		int iRet;

		m_szBuff[0] = '\0';

		iRet = ::LoadString(m_hInstance, nID, m_szBuff, sizeof(m_szBuff));

		ASSERT(iRet > 0);

		return *this;
	}

	// ƒLƒƒƒXƒg
	operator LPCTSTR() {return m_szBuff;}

private:
	HINSTANCE m_hInstance;
	TCHAR m_szBuff[256];
};
