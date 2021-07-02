#if !defined(AFX_RECIPEMAINLISTVIEW_H__FE4783F3_FC74_4A77_B3B4_E5BE054B216C__INCLUDED_)
#define AFX_RECIPEMAINLISTVIEW_H__FE4783F3_FC74_4A77_B3B4_E5BE054B216C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMainListView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMainListView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeListUI.h"

class CRecipeMainListView : public CNanoRecipeListUI
{
protected:
	CRecipeMainListView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMainListView();
	DECLARE_DYNCREATE(CRecipeMainListView)

protected:
	//{{AFX_MSG(CRecipeMainListView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL RecipeStart(LPTSTR pszRecipeName);
	BOOL RecipeHead(LPTSTR pszRecipeHead, LPTSTR pszRecipeScan);


public:
	virtual void RecipeEntry(BOOL bNew);

protected:
	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual BOOL DeleteRecipeData(LPCTSTR pszRecipeName);
	virtual BOOL IsAccessPrivilege();
private:
	void List_Add(char* szName, int iHeadType, int iMeasType, char* szDate, char* szComment);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMAINLISTVIEW_H__FE4783F3_FC74_4A77_B3B4_E5BE054B216C__INCLUDED_)
