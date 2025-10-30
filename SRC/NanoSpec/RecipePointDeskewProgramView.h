#if !defined(AFX_RECIPEPOINTDESKEWPROGRAMVIEW_H__1D05B3A9_8BE5_4D7F_9309_D5E7AEC1BCA2__INCLUDED_)
#define AFX_RECIPEPOINTDESKEWPROGRAMVIEW_H__1D05B3A9_8BE5_4D7F_9309_D5E7AEC1BCA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipePointDeskewProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipePointDeskewProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"

class CRecipePointDeskewProgramView : public CNanoRecipeUI
{
protected:
	CRecipePointDeskewProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipePointDeskewProgramView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipePointDeskewProgramView)
	enum { IDD = IDD_RECIPE_POINT_DESKEW_PROGRAM_FORM };
	//}}AFX_DATA

//Operation
protected:
	CToolBar m_wndToolBar;
	CBrush m_Brush_PointDeskewProgramName;
	CBrush m_Brush_AutoDeskew_X;
	CBrush m_Brush_AutoDeskew_Y;
	CShadeButtonST m_AutoDeskewSetupButton;

	void ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color);
	void LoadRecipeData();
	void SetData();
	void UpDate();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipePointDeskewProgramView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipePointDeskewProgramView();
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
	//{{AFX_MSG(CRecipePointDeskewProgramView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnAutoDeskewSetupButton();
	afx_msg void OnSelchangeDeskewExecutionMode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	POINT_DESKEW_PROG_INFO m_PointDeskewProgInfo;
	POINT_DESKEW_PROG_INFO m_OldPointDeskewProgInfo;
	BOOL m_bDeskewDefined;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEPOINTDESKEWPROGRAMVIEW_H__1D05B3A9_8BE5_4D7F_9309_D5E7AEC1BCA2__INCLUDED_)
