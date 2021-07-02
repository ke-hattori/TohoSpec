// NanoSpecView.h : CNanoSpecView クラスの宣言およびインターフェイスの定義をします。
//
// ##########################################################################

#if !defined(AFX_NANOSPECVIEW_H__B2ED7EBA_A79C_4BCC_8E82_0E7144987B1D__INCLUDED_)
#define AFX_NANOSPECVIEW_H__B2ED7EBA_A79C_4BCC_8E82_0E7144987B1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NanoUI.h"

class CNanoSpecView : public CNanoUI
{
protected: // シリアライズ機能のみから作成します。
	CNanoSpecView();
	DECLARE_DYNCREATE(CNanoSpecView)

public:
	//{{AFX_DATA(CNanoSpecView)
	enum{ IDD = IDD_NANOSPEC_FORM };
	//}}AFX_DATA

// アトリビュート
public:
	CNanoSpecDoc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNanoSpecView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	virtual void OnInitialUpdate(); // 構築後の最初の１度だけ呼び出されます。
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CNanoSpecView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CNanoSpecView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// NanoSpecView.cpp ファイルがデバッグ環境の時使用されます。
inline CNanoSpecDoc* CNanoSpecView::GetDocument()
	{ return (CNanoSpecDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NANOSPECVIEW_H__B2ED7EBA_A79C_4BCC_8E82_0E7144987B1D__INCLUDED_)
