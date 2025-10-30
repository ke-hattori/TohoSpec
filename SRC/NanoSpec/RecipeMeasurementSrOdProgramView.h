#if !defined(AFX_RECIPEMEASUREMENTSRODPROGRAMVIEW_H__E8AA376D_4015_48B6_89B4_EB13A0A42F42__INCLUDED_)
#define AFX_RECIPEMEASUREMENTSRODPROGRAMVIEW_H__E8AA376D_4015_48B6_89B4_EB13A0A42F42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementSrOdProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementSrOdProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"

class CRecipeMeasurementSrOdProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementSrOdProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementSrOdProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementSrOdProgramView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMeasurementSrOdProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_SR_OD_PROGRAM_FORM };
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementSrOdProgramView)
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
	virtual BOOL CheckTransData(int iData , CString strData);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeMeasurementSrOdProgramView)
	afx_msg void OnCheckSeconReferenceMeasurement();
	afx_msg void OnSelchangeMeasReflLens();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_ReferenceMaterial(CComboBox* pCombo, LPCTSTR pszMaterial);
	void InitCombo_Lens();
	CString m_strSpecificWavelen[3];
	SR_XMP m_SrXmp;
	MEAS_PROG_INFO m_ODMeas;
	MEAS_PROG_INFO m_OldODMeas;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	CString m_strLens;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTSRODPROGRAMVIEW_H__E8AA376D_4015_48B6_89B4_EB13A0A42F42__INCLUDED_)
