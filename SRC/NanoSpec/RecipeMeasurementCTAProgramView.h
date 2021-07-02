#if !defined(AFX_RECIPEMEASUREMENTCTAPROGRAMVIEW_H__E2EFBDCC_5DD1_42D3_8CF8_537ACF5148AF__INCLUDED_)
#define AFX_RECIPEMEASUREMENTCTAPROGRAMVIEW_H__E2EFBDCC_5DD1_42D3_8CF8_537ACF5148AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementCTAProgramView.h : ヘッダー ファイル
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementCTAProgramView ダイアログ

class CRecipeMeasurementCTAProgramView : public CNanoRecipeUI
{
// コンストラクション
public:
	CRecipeMeasurementCTAProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementCTAProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementCTAProgramView)

// ダイアログ データ
	//{{AFX_DATA(CRecipeMeasurementCTAProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_CTA_PROGRAM_FORM };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementCTAProgramView)
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
	//{{AFX_MSG(CRecipeMeasurementCTAProgramView)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	MEAS_PROG_INFO m_CTAMeas;
	MEAS_PROG_INFO m_OldCTAMeas;

private:
	BOOL MakeTempCTAMeasurementRecipe(LPCTSTR pszRecipeName);
	BOOL MakeTempCTAMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas);
	BOOL TempRecipeDocSet(LPCTSTR pszRecipeName);
	BOOL GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType);
	BOOL FileOrDirExists(LPCTSTR pszFileName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTCTAPROGRAMVIEW_H__E2EFBDCC_5DD1_42D3_8CF8_537ACF5148AF__INCLUDED_)
