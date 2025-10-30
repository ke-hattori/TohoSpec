#if !defined(AFX_RECIPEMEASUREMENTRESISTANCEPROGRAMVIEW_H__E26FC3D6_0250_4B82_B039_0D276998D83F__INCLUDED_)
#define AFX_RECIPEMEASUREMENTRESISTANCEPROGRAMVIEW_H__E26FC3D6_0250_4B82_B039_0D276998D83F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementResistanceProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementResistanceProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"

class CRecipeMeasurementResistanceProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementResistanceProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementResistanceProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementResistanceProgramView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMeasurementResistanceProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_RESISTANCE_PROGRAM_FORM };
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementResistanceProgramView)
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
	//{{AFX_MSG(CRecipeMeasurementResistanceProgramView)
	afx_msg void OnSelchangeThicknessUnit();
	afx_msg void OnSelchangeLimitVoltage();
	afx_msg void OnSelchangeStartRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_ThicknessUnit();
	void InitCombo_LimitVoltage();
	void InitCombo_StartRange();

	MEAS_PROG_INFO m_ResisMeas;
	MEAS_PROG_INFO m_OldResisMeas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTRESISTANCEPROGRAMVIEW_H__E26FC3D6_0250_4B82_B039_0D276998D83F__INCLUDED_)
