#if !defined(AFX_RECIPEMULTIVIEW_H__D250083C_B6CE_4267_A2D9_571295D6B983__INCLUDED_)
#define AFX_RECIPEMULTIVIEW_H__D250083C_B6CE_4267_A2D9_571295D6B983__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMultiView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMultiView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "NanoListCtrl.h"
#include "ColorStatic.h"

class CRecipeMultiView : public CNanoRecipeUI
{
protected:
	CRecipeMultiView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeMultiView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMultiView)
	enum { IDD = IDD_RECIPE_MULTI_FORM };
	CNanoListCtrl	m_mainListCtrl;
	CNanoListCtrl	m_multiListCtrl;
	//}}AFX_DATA
	CColorStatic m_stcMultiRecipeName;

// アトリビュート
protected:

// オペレーション
protected:
	CToolBar m_wndToolBar;
	CButtonST m_MultiRecipeAddButton;
	CButtonST m_MultiRecipeDeleteButton;

	void MainRecipeList_Init();
	void MainRecipeList_Add(LPTSTR lpszName, int iHeadType, int iMeasType);
	void MainRecipeList_DataSet();
	void MultiRecipeList_Init();
	void MultiRecipeList_Add(LPTSTR lpszName);
	void MultiRecipeList_DataSet();
	void LoadRecipeData();
	void SetData();
	void UpDate();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMultiView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeMultiView();
	virtual BOOL IsDataChanged();
	virtual BOOL SaveRecipeData();
	virtual BOOL IsAccessPrivilege();
	virtual BOOL CheckData();
	virtual int SaveAsDlgInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeMultiView)
	afx_msg void OnMultiRecipeAddButton();
	afx_msg void OnMultiRecipeDeleteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	MULTI_RCP_INFO_HDR m_MultiRcpInfoHdr;
	MULTI_RCP_INFO_HDR m_OldMultiRcpInfoHdr;
	MULTI_RCP_MAIN_RCP_LIST m_MultiRcpMainRcpList[MULTI_RCP_MAIN_RCP_MAX];
	MULTI_RCP_MAIN_RCP_LIST m_OldMultiRcpMainRcpList[MULTI_RCP_MAIN_RCP_MAX];

	int m_iNumMainRecipe;
	int m_iWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMULTIVIEW_H__D250083C_B6CE_4267_A2D9_571295D6B983__INCLUDED_)
