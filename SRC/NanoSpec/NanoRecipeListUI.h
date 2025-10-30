// NanoListUI.h : ヘッダー ファイル
//

#pragma once

// ##########################################################################
// CNanoRecipeListUI フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoListUI.h"

class CNanoRecipeListUI : public CNanoListUI
{
protected:
	CNanoRecipeListUI();
	virtual ~CNanoRecipeListUI();
	DECLARE_DYNCREATE(CNanoRecipeListUI)

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMainListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNanoRecipeListUI)
	afx_msg void OnNew();
	afx_msg void OnDelete();
	afx_msg void OnClickRecipeMainList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkRecipeMainList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);

	//}}AFX_MSG
	afx_msg LRESULT OnReturnRecipeMainList(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

public:
	void LayoutToolBar();
	virtual void RecipeEntry(BOOL bNew);
	BOOL CheckSelectRecipe();
/* added 2016.08.15 hmenjo レシピ編集 menu バグ3 ---------- { ---------- */
	virtual BOOL IsAccessPrivilege();
/* added 2016.08.15 hmenjo レシピ編集 menu バグ3 ---------- } ---------- */

protected:
	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual BOOL DeleteRecipeData(LPCTSTR pszRecipeName);
/* deleted 2016.08.15 hmenjo レシピ編集 menu バグ3 ---------- { ---------- */
//	virtual BOOL IsAccessPrivilege();
/* deleted 2016.08.15 hmenjo レシピ編集 menu バグ3 ---------- } ---------- */
	int SetCursorPosition(int iCol, LPCTSTR psz);

	CToolBar m_wndToolBar;
};
