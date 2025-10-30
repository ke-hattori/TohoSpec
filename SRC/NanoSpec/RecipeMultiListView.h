#if !defined(AFX_RECIPEMULTILISTVIEW_H__D4E6BC28_3CB6_486B_BBBB_80A74640ABE4__INCLUDED_)
#define AFX_RECIPEMULTILISTVIEW_H__D4E6BC28_3CB6_486B_BBBB_80A74640ABE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMultiListView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMultiListView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeListUI.h"

class CRecipeMultiListView : public CNanoRecipeListUI
{
protected:
	CRecipeMultiListView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMultiListView();
	DECLARE_DYNCREATE(CRecipeMultiListView)

public:
	virtual void RecipeEntry(BOOL bNew);

protected:
	//{{AFX_MSG(CRecipeMultiListView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual BOOL DeleteRecipeData(LPCTSTR pszRecipeName);
	virtual BOOL IsAccessPrivilege();
private:
	void List_Add(char* szName, char* szDate, char* szComment);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMULTILISTVIEW_H__D4E6BC28_3CB6_486B_BBBB_80A74640ABE4__INCLUDED_)
