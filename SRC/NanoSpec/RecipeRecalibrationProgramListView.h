#if !defined(AFX_RECIPERECALIBRATIONPROGRAMLISTVIEW_H__BDAF693C_2B40_4967_81E6_FD5035AFD32B__INCLUDED_)
#define AFX_RECIPERECALIBRATIONPROGRAMLISTVIEW_H__BDAF693C_2B40_4967_81E6_FD5035AFD32B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeRecalibrationProgramListView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeRecalibrationProgramListView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeListUI.h"

class CRecipeRecalibrationProgramListView : public CNanoRecipeListUI
{
protected:
	CRecipeRecalibrationProgramListView();			 // 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeRecalibrationProgramListView();
	DECLARE_DYNCREATE(CRecipeRecalibrationProgramListView)

public:
	virtual void RecipeEntry(BOOL bNew);

protected:
	//{{AFX_MSG(CRecipeRecalibrationProgramListView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual BOOL DeleteRecipeData(LPCTSTR pszRecipeName);
	virtual BOOL IsAccessPrivilege();
private:
	void List_Add(char* szName, int iHeadType, int iMeasType, char* szDate, char* szComment);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPERECALIBRATIONPROGRAMLISTVIEW_H__BDAF693C_2B40_4967_81E6_FD5035AFD32B__INCLUDED_)
