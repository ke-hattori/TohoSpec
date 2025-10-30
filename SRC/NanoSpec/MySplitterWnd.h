#if !defined(AFX_MYSPLITTERWND_H__94F17219_434D_43D7_BDC2_5206EA4DA3BA__INCLUDED_)
#define AFX_MYSPLITTERWND_H__94F17219_434D_43D7_BDC2_5206EA4DA3BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySplitterWnd.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd ウィンドウ                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
class CMySplitterWnd : public CSplitterWnd
{
// コンストラクション
public:
	CMySplitterWnd();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CMySplitterWnd)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMySplitterWnd();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMySplitterWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYSPLITTERWND_H__94F17219_434D_43D7_BDC2_5206EA4DA3BA__INCLUDED_)
