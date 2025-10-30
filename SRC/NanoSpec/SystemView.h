#if !defined(AFX_SYSTEMVIEW_H__6F73C25A_4DF1_4A1D_AD1B_BB693ACC65AF__INCLUDED_)
#define AFX_SYSTEMVIEW_H__6F73C25A_4DF1_4A1D_AD1B_BB693ACC65AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemView.h : ヘッダー ファイル
//

// ##########################################################################
// CSystemView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CSystemView : public CNanoUI
{
protected:
	CSystemView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CSystemView)

// フォーム データ
public:
	//{{AFX_DATA(CSystemView)
	enum { IDD = IDD_SYSTEM_FORM };
	//}}AFX_DATA

// アトリビュート
public:
	CBrush m_Brush_BackColor;

// オペレーション
public:
	void ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemView)
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	public:
	virtual void OnInitialUpdate();
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMVIEW_H__6F73C25A_4DF1_4A1D_AD1B_BB693ACC65AF__INCLUDED_)
