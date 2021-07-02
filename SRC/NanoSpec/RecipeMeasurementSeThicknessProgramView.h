#if !defined(AFX_RecipeMeasurementSeThicknessProgramView_H__2EABB3D5_0FEE_415C_BF19_4FF5C6B4B6FC__INCLUDED_)
#define AFX_RecipeMeasurementSeThicknessProgramView_H__2EABB3D5_0FEE_415C_BF19_4FF5C6B4B6FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementSeThicknessProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementSeThicknessProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoListCtrl.h"

class CRecipeMeasurementSeThicknessProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementSeThicknessProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementSeThicknessProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementSeThicknessProgramView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMeasurementSeThicknessProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_SE_THICKNESS_PROGRAM_FORM };
	CNanoListCtrl	m_XmpMeasurementItemListCtrl;
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementSeThicknessProgramView)
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
	//{{AFX_MSG(CRecipeMeasurementSeThicknessProgramView)
	afx_msg void OnSelchangeSeThickStrategy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_SeThickStrategy();
	void InitItemList();
	MEAS_PROG_INFO m_ThickMeas;
	MEAS_PROG_INFO m_OldThickMeas;

private:
	BOOL MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName);
	BOOL MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas);
	BOOL TempRecipeDocSet(LPCTSTR pszRecipeName);
	BOOL GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType);
	BOOL FileOrDirExists(LPCTSTR pszFileName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RecipeMeasurementSeThicknessProgramView_H__2EABB3D5_0FEE_415C_BF19_4FF5C6B4B6FC__INCLUDED_)
