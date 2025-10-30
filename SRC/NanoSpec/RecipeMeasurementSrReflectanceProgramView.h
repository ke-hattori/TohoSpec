#if !defined(AFX_RECIPEMEASUREMENTSRREFLECTANCEPROGRAMVIEW_H__D66A7569_094C_4AB4_B86E_6D8E966F828E__INCLUDED_)
#define AFX_RECIPEMEASUREMENTSRREFLECTANCEPROGRAMVIEW_H__D66A7569_094C_4AB4_B86E_6D8E966F828E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementSrReflectanceProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementSrReflectanceProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"

class CRecipeMeasurementSrReflectanceProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementSrReflectanceProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementSrReflectanceProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementSrReflectanceProgramView)

// 2010.01.07 bagus Gantry --{--
public:
	int m_iNewHeadType;
	int m_iNewScanType;
// 2010.01.07 bagus Gantry --}--

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMeasurementSrReflectanceProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_SR_REFLECTANCE_PROGRAM_FORM };
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementSrReflectanceProgramView)
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
	//{{AFX_MSG(CRecipeMeasurementSrReflectanceProgramView)
	afx_msg void OnCheckReferenceLifeTime();
	afx_msg void OnSelchangeSrMeasReflOptFilter();
	afx_msg void OnSelchangeMeasReflLens();
	afx_msg void OnTestButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_SrReflOpticalFilter();
	void InitCombo_Lens();
	void InitCombo_MeasReflReferenceMaterial();
	CString m_strSpecificWavelen[3];
	SR_CONFIG m_SrConfig;
	MEAS_PROG_INFO m_ReflecMeas;
	MEAS_PROG_INFO m_OldReflecMeas;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	SR_FILTER m_srFilter[SR_FILTER_MAX];
	CString m_strLens;
	CString m_strOptFilter;
	CShadeButtonST m_TestButton;

private:
	BOOL MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName);
	//2010.01.11 bagus Gantry --{--
	//BOOL MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas);
	BOOL MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas, LPCTSTR pszRecipeNameStage);
	BOOL MakeTempSrGantryStageRecipe(LPCTSTR pszRecipeName);
	//2010.01.11 bagus Gantry --}--
	BOOL TempRecipeDocSet(LPCTSTR pszRecipeName);
	BOOL GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType);
	BOOL FileOrDirExists(LPCTSTR pszFileName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTSRREFLECTANCEPROGRAMVIEW_H__D66A7569_094C_4AB4_B86E_6D8E966F828E__INCLUDED_)
