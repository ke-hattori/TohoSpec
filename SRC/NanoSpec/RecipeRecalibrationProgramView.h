#if !defined(AFX_RECIPERECALIBRATIONPROGRAMVIEW_H__FE2C3AFF_2594_44A4_A6C6_0642856D8C5A__INCLUDED_)
#define AFX_RECIPERECALIBRATIONPROGRAMVIEW_H__FE2C3AFF_2594_44A4_A6C6_0642856D8C5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeRecalibrationProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeRecalibrationProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用

class CRecipeRecalibrationProgramView : public CNanoRecipeUI
{
protected:
	CRecipeRecalibrationProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeRecalibrationProgramView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeRecalibrationProgramView)
	enum { IDD = IDD_RECIPE_RECALIBRATION_PROGRAM_FORM };
	CNanoMeasureGraph	m_RecalibGraph;
	//}}AFX_DATA
	CColorStatic m_stcRecalibrationProgramName;

//Operations
protected:
	CToolBar m_wndToolBar;
	CGridCtrl m_RecalibrationGrid;

	BOOL RecalibrationGrid_Graph_Init();
	void Graph_Init();
	void Graph_Frame_Init(int dX_Min =0, int dX_Max =100, int dY_Min =0, int dY_Max =100, int iPointNum = 1);
	void LoadRecipeData();
	void SetData();
	void UpDate();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeRecalibrationProgramView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeRecalibrationProgramView();
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
	//{{AFX_MSG(CRecipeRecalibrationProgramView)
	afx_msg void OnDrawRecalibGraphButton();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	RECALIB_PROG_INFO m_RecalibProgInfo;
	RECALIB_PROG_INFO m_OldRecalibProgInfo;
	CShadeButtonST m_DrawGraphButton;

//2009.09.01 bagus stress --{--
private:
	int		m_ValidNum;
//2009.09.01 bagus stress --}--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPERECALIBRATIONPROGRAMVIEW_H__FE2C3AFF_2594_44A4_A6C6_0642856D8C5A__INCLUDED_)
