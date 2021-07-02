#if !defined( _FILEMAP_HXX_ )
#define _FILEMAP_HXX_

#pragma once

//===========================================================================
// class CFileMapping
//===========================================================================
class CFileMapping
{
public:
	CFileMapping();
	CFileMapping(PCSTR pszFileName, DWORD dwDataSize, LPSECURITY_ATTRIBUTES lpAttributes = NULL);
	CFileMapping(PCSTR pszFileName);
	virtual ~CFileMapping();

	void Create(PCSTR pszFileName, DWORD dwDataSize, LPSECURITY_ATTRIBUTES lpAttributes = NULL);
	void Open(PCSTR pszFileName);
	void Close(void);
	virtual void Attach(void);
	virtual void Detach(void);

	LPVOID		  operator->() { return m_lpBuffer; }
	CFileMapping* operator&()  { return (CFileMapping *)this; }
	operator	  void* ()	   { return m_lpBuffer; }

	inline BOOL   IsAlreadyExists(void) 	{ return m_bAlreadyExists; }
	inline BOOL   IsValid(void) const		{ return m_lpBuffer != NULL; }
	inline LPVOID GetBuffer(void) const 	{ return m_lpBuffer; }
	inline DWORD  GetBufferSize(void) const { return m_dwDataSize; }
	inline PCSTR  GetFileName(void) const	{ return m_strFileName.c_str(); }

protected:
	HANDLE m_hFile;
public:
	LPVOID m_lpBuffer;
	BOOL m_bAlreadyExists;
	DWORD m_dwDataSize;
	std::string m_strFileName;
};

//===========================================================================
// template class CFileMappingST
//===========================================================================
template<class TYPE>
class CFileMappingST : public CFileMapping
{
public:
	CFileMappingST();
	CFileMappingST(PCSTR pszFileName) : CFileMapping(pszFileName, sizeof(TYPE)) {}
	virtual ~CFileMappingST() {}

	TYPE *operator->() { return (TYPE*) m_lpBuffer; }
};

#endif // _FILEMAP_HXX_
