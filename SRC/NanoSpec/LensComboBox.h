#if !defined(AFX_LENSCOMBOBOX_H__E63F4D5D_ACBF_4FFC_89C1_837CC03B8B75__INCLUDED_)
#define AFX_LENSCOMBOBOX_H__E63F4D5D_ACBF_4FFC_89C1_837CC03B8B75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LensComboBox.h : ヘッダー ファイル
//
#include <list>

// ##########################################################################
// CLensComboBox ウィンドウ
// ##########################################################################

class CLensComboBox : public CComboBox
{
// コンストラクション
public:
	CLensComboBox();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CLensComboBox)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL CheckIL();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CLensComboBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CLensComboBox)
	afx_msg void OnSelchange();
	afx_msg void OnSelendcancel();
	//}}AFX_MSG
	afx_msg LRESULT OnSelectLensNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	static std::list<HWND> m_hWndList;
};

// ##########################################################################
// CCameraComboBox ウィンドウ
// ##########################################################################

class CCameraComboBox : public CComboBox
{
// コンストラクション
public:
	CCameraComboBox();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCameraComboBox)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL CheckIL();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CCameraComboBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CCameraComboBox)
	afx_msg void OnSelchange();
	//}}AFX_MSG
//	afx_msg LRESULT OnSelectLensNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	static std::list<HWND> m_hWndList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_LENSCOMBOBOX_H__E63F4D5D_ACBF_4FFC_89C1_837CC03B8B75__INCLUDED_)
