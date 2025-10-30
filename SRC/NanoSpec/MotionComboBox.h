#if !defined(AFX_MOTIONCOMBOBOX_H__401EBC8C_AAC5_48D1_B996_03601C391263__INCLUDED_)
#define AFX_MOTIONCOMBOBOX_H__401EBC8C_AAC5_48D1_B996_03601C391263__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionComboBox.h : ヘッダー ファイル
//
#include <list>

/////////////////////////////////////////////////////////////////////////////
// CMotionComboBox ウィンドウ
class CTypeComboBox : public CComboBox
{
public:
	CTypeComboBox();
	virtual ~CTypeComboBox();

protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL CheckIL();
	afx_msg void OnSelchange();
	afx_msg void OnSelendcancel();
	afx_msg LRESULT OnSelectHeadTypeNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	static std::list<HWND> m_hWndList;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	BOOL m_bSelChanged;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

private:
   	HEAD_TYPE_CONFIG m_HeadType;
};

class CMotionComboBox : public CComboBox
{
public:
	CMotionComboBox();
	virtual ~CMotionComboBox();

	UINT GetMode(void);

protected:
	//{{AFX_VIRTUAL(CMotionComboBox)
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL CheckIL();
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CMotionComboBox)
	afx_msg void OnSelchange();
	//}}AFX_MSG
	afx_msg LRESULT OnSelectNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	static std::list<HWND> m_hWndList;

private:
	UINT m_nMode;
	UINT m_nModeOld;
	SYSTEM_CONFIG m_SystemConfig1;
};

class CMotionElevComboBox : public CComboBox
{
public:
	CMotionElevComboBox();
	virtual ~CMotionElevComboBox();

	UINT GetElevMode(void);
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL CheckIL();
	afx_msg void OnSelchange();

	DECLARE_MESSAGE_MAP()

protected:
	static std::list<HWND> m_hWndListElev;

private:
	UINT m_nElevMode;
	SYSTEM_CONFIG m_SystemConfig2;
};

//#define JOY_RADIO_MAX	2
//#define JOY_RADIO1		0
//#define JOY_RADIO2		1
class CJoyStickRadioButton : public CButton
{
public:
	CJoyStickRadioButton(int index);
	virtual ~CJoyStickRadioButton();

	afx_msg void Checked();
	afx_msg void UnChecked();

protected:
	//{{AFX_VIRTUAL(CJoyStickRadioButton)
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL CheckChangeMode();
	virtual BOOL CheckIL();
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CJoyStickRadioButton)
	afx_msg void OnJoyStick();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
//	static std::list<HWND> m_hWndList[JOY_RADIO_MAX];
	static std::list<HWND> m_hWndList[JOY_MODE_MAX];
	int	m_nIndex;

private:


};

// 2013.02.22 bagus Substrate thickness setting -->
class CSubstrateThicknessComboBox : public CComboBox
{
public:
	CSubstrateThicknessComboBox();
	virtual ~CSubstrateThicknessComboBox();

protected:
	//{{AFX_VIRTUAL(CSubstrateThicknessComboBox)
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL CheckIL();
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CSubstrateThicknessComboBox)
	afx_msg void OnSelchange();
	//}}AFX_MSG
	afx_msg LRESULT OnSelectSubstrateThicknessNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	double MilliToMicro(double dMilli);
	double MicroToMilli(double dMicro);

public:
	static std::list<HWND> m_hWndList;
};
// 2013.02.22 bagus Substrate thickness setting <--

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MOTIONCOMBOBOX_H__401EBC8C_AAC5_48D1_B996_03601C391263__INCLUDED_)
