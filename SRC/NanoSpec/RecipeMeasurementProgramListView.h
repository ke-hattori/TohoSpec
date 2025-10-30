#if !defined(AFX_RECIPEMEASUREMENTPROGRAMLISTVIEW_H__61C53245_C247_415B_BDA8_A8A0CFFEE82F__INCLUDED_)
#define AFX_RECIPEMEASUREMENTPROGRAMLISTVIEW_H__61C53245_C247_415B_BDA8_A8A0CFFEE82F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementProgramListView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementProgramListView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeListUI.h"

class CRecipeMeasurementProgramListView : public CNanoRecipeListUI
{
protected:
	CRecipeMeasurementProgramListView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementProgramListView();
	DECLARE_DYNCREATE(CRecipeMeasurementProgramListView)

public:
	virtual void RecipeEntry(BOOL bNew);

protected:
	//{{AFX_MSG(CRecipeMeasurementProgramListView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual BOOL DeleteRecipeData(LPCTSTR pszRecipeName);
	virtual BOOL IsAccessPrivilege();
private:
	void List_Add(char* szName, int iHeadType, int iMeasType, char* szDate, char* szComment);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTPROGRAMLISTVIEW_H__61C53245_C247_415B_BDA8_A8A0CFFEE82F__INCLUDED_)
