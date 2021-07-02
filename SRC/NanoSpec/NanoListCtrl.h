#if !defined(AFX_CNANOLISTCTRL_H__28E6B317_D5D0_4C55_B164_5A7FE4CBBAA0__INCLUDED_)
#define AFX_CNANOLISTCTRL_H__28E6B317_D5D0_4C55_B164_5A7FE4CBBAA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NanoListCtrl.h : ヘッダー ファイル
//

// ##########################################################################
// CNanoListCtrl ウィンドウ
// ##########################################################################

class CNanoListCtrl : public CListCtrl
{
// コンストラクション
public:
	CNanoListCtrl();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	//{{AFX_VIRTUAL(CNanoListCtrl)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CNanoListCtrl();
	SetViewStyle();

protected:
	//{{AFX_MSG(CNanoListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void FontCreation();

	CFont m_listFont;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CNANOLISTCTRL_H__28E6B317_D5D0_4C55_B164_5A7FE4CBBAA0__INCLUDED_)
