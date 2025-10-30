#if !defined(AFX_RECIPEOPERATIONVIEW_H__9D2FB97A_5098_4435_8B29_0C78C97B04AF__INCLUDED_)
#define AFX_RECIPEOPERATIONVIEW_H__9D2FB97A_5098_4435_8B29_0C78C97B04AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeOperationView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeOperationView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\\..\\INC\\Globals.hxx"
#include "NanoUI.h"

class CRecipeOperationView : public CNanoUI
{
protected:
	CRecipeOperationView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeOperationView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipeOperationView)
	enum { IDD = IDD_RECIPE_OPERATION_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	int	m_iCurrEditMode;
	CShadeButtonST m_MainRecipeButton;
	CShadeButtonST m_MultiRecipeButton;
	CShadeButtonST m_MeasurementProgramButton;
	CShadeButtonST m_StageProgramButton;
	CShadeButtonST m_RecalibrationProgramButton;
	CShadeButtonST m_DeskewProgramButton;
	CShadeButtonST m_PointDeskewProgramButton;
	SYSTEM_CONFIG m_SystemConfig;

	void SetMenuButton(int iEditMode);
	void SelectMenuButton(int iSelect);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeOperationView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeOperationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeOperationView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMainRecipeButton();
	afx_msg void OnMultiRecipeButton();
	afx_msg void OnMeasurementProgramButton();
	afx_msg void OnStageProgramButton();
	afx_msg void OnRecalibrationProgramButton();
	afx_msg void OnPointDeskewProgramButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEOPERATIONVIEW_H__9D2FB97A_5098_4435_8B29_0C78C97B04AF__INCLUDED_)
