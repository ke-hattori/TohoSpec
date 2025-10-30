// NanoListUI.h : ヘッダー ファイル
//

#pragma once

// ##########################################################################
// CNanoListUI フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoUI.h"
#include "NanoListCtrl.h"

class CNanoListUI : public CNanoUI
{
protected:
	CNanoListUI();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CNanoListUI();
	DECLARE_DYNAMIC(CNanoListUI)

// フォーム データ
public:
	//{{AFX_DATA(CNanoListUI)
	enum { IDD = IDD_RECIPE_MAIN_LIST_FORM };
	//}}AFX_DATA
	CNanoListCtrl m_lstCtrl;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMainListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNanoRecipeListUI)
	afx_msg void OnColumnclickRecipeMainList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// オペレーション
protected:
	virtual void List_HeaderSet()=0;	// 派生クラスで定義
	virtual void List_DataSet()=0;		// 派生クラスで定義
	void SortColumn(int iCol);

	CListCtrl& rfListCtrl() {return m_lstCtrl;}

private:
	void RenumberItem();

	BOOL m_bSort[256];
};
