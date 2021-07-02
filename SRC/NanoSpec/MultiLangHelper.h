// MultiLangHelper.h : ヘッダー ファイル
//

#pragma once

// =========================================================================
//
class CMultiLangHelper
{
public:
	CMultiLangHelper();
	virtual ~CMultiLangHelper();

	virtual int GetItemCount()=0;
	virtual CString& GetItemText(int index)=0;
	virtual CString& operator[](int index);

protected:
	UINT m_nLanguage;
	CString m_string;
};


// =========================================================================
//
class CMLScanType : public CMultiLangHelper
{
public:
	CMLScanType(int iHeadType);
	virtual ~CMLScanType();

	int GetItemCount();
	CString& GetItemText(int index);

private:
	int m_iHeadType;
};
