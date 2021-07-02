// NanoSpecIni.h: interface for the CNanoSpecIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NANOSPECINI_H__DD6F214B_A286_4621_ADF9_933459A81113__INCLUDED_)
#define AFX_NANOSPEC_H__DD6F214B_A286_4621_ADF9_933459A81113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNanoSpecIni
{
public:
	CNanoSpecIni();
	virtual ~CNanoSpecIni();

private:
	int		m_IniCount;

// 2013.11.07 Bagus Add (TohoSpec‘Î‰ž) -->
	CString m_strIniFilePath;
// 2013.11.07 Bagus Add (TohoSpec‘Î‰ž) <--

public:
	void	Get();
	void	Set();
};

#endif // !defined(AFX_NANOSPEC_H__DD6F214B_A286_4621_ADF9_933459A81113__INCLUDED_)
