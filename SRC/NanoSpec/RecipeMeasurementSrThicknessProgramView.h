#if !defined(AFX_RECIPEMEASUREMENTSRTHICKNESSPROGRAMVIEW_H__2EABB3D5_0FEE_415C_BF19_4FF5C6B4B6FC__INCLUDED_)
#define AFX_RECIPEMEASUREMENTSRTHICKNESSPROGRAMVIEW_H__2EABB3D5_0FEE_415C_BF19_4FF5C6B4B6FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementSrThicknessProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementSrThicknessProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoListCtrl.h"

class CRecipeMeasurementSrThicknessProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementSrThicknessProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementSrThicknessProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementSrThicknessProgramView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMeasurementSrThicknessProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_SR_THICKNESS_PROGRAM_FORM };
	CNanoListCtrl	m_XmpMeasurementItemListCtrl;
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementSrThicknessProgramView)
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
	//{{AFX_MSG(CRecipeMeasurementSrThicknessProgramView)
	afx_msg void OnCheckReferenceLifeTime();
	afx_msg void OnSelchangeSrThickOptFilter();
	afx_msg void OnAddReflectanceMeasurement();
	afx_msg void OnSelchangeSrThickStrategy();
	afx_msg void OnSelchangeSrThickLens();
	afx_msg void OnTestButton();
/* added 2015.06.01 hmenjo XMP 表示/非表示２ ---------- { ---------- */
	afx_msg void OnBtnShowXmp();
	afx_msg void OnTimer(UINT nIDEvent);
/* added 2015.06.01 hmenjo XMP 表示/非表示２ ---------- } ---------- */
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_SrThickStrategy();
	void InitCombo_SrThickReferenceMaterial();
	void InitCombo_SrThickOpticalFilter();
	void InitItemList();
	void InitCombo_Lens();
	CString m_strSpecificWavelen[3];
	SR_CONFIG m_SrConfig;
	MEAS_PROG_INFO m_ThickMeas;
	MEAS_PROG_INFO m_OldThickMeas;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	SR_FILTER m_srFilter[SR_FILTER_MAX];
	CString m_strLens;
	CString m_strOptFilter;
	CShadeButtonST m_TestButton;
/* added 2015.06.01 hmenjo XMP 表示/非表示２ ---------- { ---------- */
	CShadeButtonST m_BtnShowXmp;
/* added 2015.06.01 hmenjo XMP 表示/非表示２ ---------- } ---------- */

private:
	BOOL MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName);
	BOOL MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas);
	BOOL TempRecipeDocSet(LPCTSTR pszRecipeName);
	BOOL GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType);
	BOOL FileOrDirExists(LPCTSTR pszFileName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTSRTHICKNESSPROGRAMVIEW_H__2EABB3D5_0FEE_415C_BF19_4FF5C6B4B6FC__INCLUDED_)
