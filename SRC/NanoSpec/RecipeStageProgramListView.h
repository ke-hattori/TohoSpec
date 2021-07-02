#if !defined(AFX_RECIPESTAGEPROGRAMLISTVIEW_H__C8487174_49F3_40C4_9136_BE143B2F8205__INCLUDED_)
#define AFX_RECIPESTAGEPROGRAMLISTVIEW_H__C8487174_49F3_40C4_9136_BE143B2F8205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeStageProgramListView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeStageProgramListView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeListUI.h"

class CRecipeStageProgramListView : public CNanoRecipeListUI
{
protected:
	CRecipeStageProgramListView();			 // 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeStageProgramListView();
	DECLARE_DYNCREATE(CRecipeStageProgramListView)

public:
	virtual void RecipeEntry(BOOL bNew);

protected:
	//{{AFX_MSG(CRecipeStageProgramListView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual BOOL DeleteRecipeData(LPCTSTR pszRecipeName);
	virtual BOOL IsAccessPrivilege();
private:
	void List_Add(char* szName, int iPoint, char* szSample, char* szDate, char* szComment);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPESTAGEPROGRAMLISTVIEW_H__C8487174_49F3_40C4_9136_BE143B2F8205__INCLUDED_)
