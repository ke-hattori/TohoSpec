#if !defined(AFX_RECIPEMEASUREMENTSRDISTANCEPROGRAMVIEW_H__5874361D_DD50_49F9_A67B_4C959005C754__INCLUDED_)
#define AFX_RECIPEMEASUREMENTSRDISTANCEPROGRAMVIEW_H__5874361D_DD50_49F9_A67B_4C959005C754__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementSrDistanceProgramView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementSrDistanceProgramView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"

class CRecipeMeasurementSrDistanceProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementSrDistanceProgramView();			 // 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementSrDistanceProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementSrDistanceProgramView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipeMeasurementSrDistanceProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_SR_DISTANCE_PROGRAM_FORM };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementSrDistanceProgramView)
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

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeMeasurementSrDistanceProgramView)
	afx_msg void OnSelchangeSrThickLens();
	afx_msg void OnSelchangeOptFilter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_Lens();
	void InitCombo_OpticalFilter();
	SR_CONFIG m_SrConfig;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	SR_FILTER m_srFilter[SR_FILTER_MAX];
	MEAS_PROG_INFO m_DistanceMeas;
	MEAS_PROG_INFO m_OldDistanceMeas;
	CString m_strLens;
	CString m_strOptFilter;

private:
	BOOL MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName);
	BOOL MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas);
	BOOL TempRecipeDocSet(LPCTSTR pszRecipeName);
	BOOL GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType);
	BOOL FileOrDirExists(LPCTSTR pszFileName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTSRDISTANCEPROGRAMVIEW_H__5874361D_DD50_49F9_A67B_4C959005C754__INCLUDED_)
