#if !defined(AFX_RECIPEMEASUREMENTMSPROGRAMVIEW_H__11D8EF38_42FB_4D90_A027_8B36DB905E42__INCLUDED_)
#define AFX_RECIPEMEASUREMENTMSPROGRAMVIEW_H__11D8EF38_42FB_4D90_A027_8B36DB905E42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementMSProgramView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementMSProgramView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"

class CRecipeMeasurementMSProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementMSProgramView();			 // 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementMSProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementMSProgramView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipeMeasurementMSProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_MS_PROGRAM_FORM };
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
	//{{AFX_VIRTUAL(CRecipeMeasurementMSProgramView)
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
	//{{AFX_MSG(CRecipeMeasurementMSProgramView)
	afx_msg void OnSelchangeMsLens();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_Lens();
	MS_CONFIG m_MsConfig;
	MEAS_PROG_INFO m_MsMeas;
	MEAS_PROG_INFO m_OldMsMeas;
	CString m_strLens;

private:
	BOOL FileOrDirExists(LPCTSTR pszFileName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTMSPROGRAMVIEW_H__11D8EF38_42FB_4D90_A027_8B36DB905E42__INCLUDED_)
