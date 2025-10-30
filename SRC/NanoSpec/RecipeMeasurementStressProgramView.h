#if !defined(AFX_RECIPEMEASUREMENTSTRESSPROGRAMVIEW_H__8EFA3375_6EE2_4EF5_9657_D1DEAA35208E__INCLUDED_)
#define AFX_RECIPEMEASUREMENTSTRESSPROGRAMVIEW_H__8EFA3375_6EE2_4EF5_9657_D1DEAA35208E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementStressProgramView.h : ヘッダー ファイル
//

#include "NanoRecipeUI.h"
#include "ColorStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementStressProgramView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoListCtrl.h"

class CRecipeMeasurementStressProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementStressProgramView();			 // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeMeasurementStressProgramView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipeMeasurementStressProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_STRESS_PROGRAM_FORM };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA

	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;
	CNanoListCtrl	m_XmpMeasurementItemListCtrl;

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementStressProgramView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeMeasurementStressProgramView();

	virtual void LoadRecipeData();
	virtual BOOL CheckData();
	virtual BOOL IsDataChanged();
	virtual BOOL SaveRecipeData();
	virtual BOOL IsAccessPrivilege();
	virtual int SaveAsDlgInfo();
	virtual BOOL CheckTransData(int iData , CString strData);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeMeasurementStressProgramView)
	afx_msg void OnCheckReferenceLifeTime();
	afx_msg void OnSelchangeStressThickOptFilter();
	afx_msg void OnAddReflectanceMeasurement();
	afx_msg void OnSelchangeStressThickStrategy();
	afx_msg void OnSelchangeStressThickLens();
	afx_msg void OnTestButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_StressThickStrategy();
	void InitCombo_StressThickReferenceMaterial();
	void InitCombo_StressThickOpticalFilter();
	void InitItemList();
	void InitCombo_Lens();

//	STRESS_CONFIG	m_StressConfig;
	MEAS_PROG_INFO m_StressMeas;
	MEAS_PROG_INFO m_OldStressMeas;

	CString m_strSpecificWavelen[3];
	SR_CONFIG m_SrConfig;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	SR_FILTER m_srFilter[SR_FILTER_MAX];
	CString m_strLens;
	CString m_strOptFilter;
	CShadeButtonST m_TestButton;

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

#endif // !defined(AFX_RECIPEMEASUREMENTSTRESSPROGRAMVIEW_H__8EFA3375_6EE2_4EF5_9657_D1DEAA35208E__INCLUDED_)
