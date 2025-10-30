#if !defined(AFX_DATAREVIEWOPERATIONVIEW_H__8B8A30CD_11F3_40BB_8F45_F0BD4358DD10__INCLUDED_)
#define AFX_DATAREVIEWOPERATIONVIEW_H__8B8A30CD_11F3_40BB_8F45_F0BD4358DD10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataReviewOperationView.h : ヘッダー ファイル
//

// ##########################################################################
// CDataReviewOperationView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CDataReviewOperationView : public CNanoUI
{
protected:
	CDataReviewOperationView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CDataReviewOperationView)

// フォーム データ
public:
	//{{AFX_DATA(CDataReviewOperationView)
	enum { IDD = IDD_DATA_REVIEW_OPERATION_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CShadeButtonST m_GridParameterSettingButton;
	CShadeButtonST m_ValuesToDisplayButton;
	CShadeButtonST m_SaveButton;
	CShadeButtonST m_SaveBitmapButton;
	CShadeButtonST m_PrintButton;
	CShadeButtonST m_ExitButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDataReviewOperationView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CDataReviewOperationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDataReviewOperationView)
	afx_msg void OnExitButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DATAREVIEWOPERATIONVIEW_H__8B8A30CD_11F3_40BB_8F45_F0BD4358DD10__INCLUDED_)
