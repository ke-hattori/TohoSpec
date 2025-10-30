#if !defined(AFX_RECIPEPOINTDESKEWPROGRAMLISTVIEW_H__0FC2940D_0C71_41EE_A4F5_317923846158__INCLUDED_)
#define AFX_RECIPEPOINTDESKEWPROGRAMLISTVIEW_H__0FC2940D_0C71_41EE_A4F5_317923846158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipePointDeskewProgramListView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipePointDeskewProgramListView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeListUI.h"

class CRecipePointDeskewProgramListView : public CNanoRecipeListUI
{
protected:
	CRecipePointDeskewProgramListView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipePointDeskewProgramListView();
	DECLARE_DYNCREATE(CRecipePointDeskewProgramListView)

public:
	virtual void RecipeEntry(BOOL bNew);

protected:
	//{{AFX_MSG(CRecipePointDeskewProgramListView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual BOOL DeleteRecipeData(LPCTSTR pszRecipeName);
	virtual BOOL IsAccessPrivilege();
private:
	void List_Add(char* szName, int iHeadType, char* szDate, char* szComment);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEPOINTDESKEWPROGRAMLISTVIEW_H__0FC2940D_0C71_41EE_A4F5_317923846158__INCLUDED_)
