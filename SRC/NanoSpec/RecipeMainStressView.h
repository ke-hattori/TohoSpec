#if !defined(AFX_RECIPEMAINSTRESSVIEW_H__5725A9B7_1C56_4DCB_B54D_EC8E858F1194__INCLUDED_)
#define AFX_RECIPEMAINSTRESSVIEW_H__5725A9B7_1C56_4DCB_B54D_EC8E858F1194__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMainStressView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeMainStressView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"

class CRecipeMainStressView : public CNanoRecipeUI
{
protected:
	CRecipeMainStressView();		   // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeMainStressView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipeMainStressView)
	enum { IDD = IDD_RECIPE_MAIN_STRESS_FORM };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMainStressView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeMainStressView();

	virtual void LoadRecipeData();
	virtual BOOL CheckData();
	virtual BOOL IsDataChanged();
	virtual BOOL SaveRecipeData();
	virtual BOOL IsAccessPrivilege();
	virtual int SaveAsDlgInfo();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeMainStressView)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMAINSTRESSVIEW_H__5725A9B7_1C56_4DCB_B54D_EC8E858F1194__INCLUDED_)
