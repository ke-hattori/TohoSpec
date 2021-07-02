#pragma once

// RecipeMeasurementCompEASEThicknessProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementCompEASEThicknessProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoListCtrl.h"

class CRecipeMeasurementCompEASEThicknessProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementCompEASEThicknessProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementCompEASEThicknessProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementCompEASEThicknessProgramView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMeasurementCompEASEThicknessProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_COMPEASE_THICKNESS_PROGRAM_FORM };
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementCompEASEThicknessProgramView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual void LoadRecipeData();
	virtual BOOL CheckData();
	virtual BOOL IsDataChanged();
	virtual BOOL SaveRecipeData();
	virtual BOOL IsAccessPrivilege();
	virtual int SaveAsDlgInfo();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeMeasurementCompEASEThicknessProgramView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_CompEASERecipe();
	void DDX_CompEASERecipeText(CDataExchange* pDX, int nIDC, LPTSTR value, int nMaxLen);

	MEAS_PROG_INFO m_ThickMeas;
	MEAS_PROG_INFO m_OldThickMeas;
};
