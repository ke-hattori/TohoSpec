#if !defined(AFX_MEASUREMENTOPERATIONVIEW_H__4652614C_9749_4E16_9843_96D5CD9F9F72__INCLUDED_)
#define AFX_MEASUREMENTOPERATIONVIEW_H__4652614C_9749_4E16_9843_96D5CD9F9F72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasurementOperationView.h : ヘッダー ファイル
//

// ##########################################################################
// CMeasurementOperationView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\\..\\INC\\Globals.hxx"
#include "NanoUI.h"

class CMeasurementOperationView : public CNanoUI
{
public:
	CMeasurementOperationView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CMeasurementOperationView)

// フォーム データ
public:
	//{{AFX_DATA(CMeasurementOperationView)
	enum { IDD = IDD_MEASUREMENT_OPERATION_FORM };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	///// Variable //////////////////////////////////////////////////////////
	int m_CurrRecipe;
	CShadeButtonST m_MainRecipeButton;
	CShadeButtonST m_MultiRecipeButton;
	CShadeButtonST m_StartButton;
	CShadeButtonST m_HandlerLoadPositionButton;
	CShadeButtonST m_ManualLoadPositionButton;
	CShadeButtonST m_VacuumButton;
	CShadeButtonST m_GlassLifterButton;
	CShadeButtonST m_ReferenceButton;
	CShadeButtonST m_ManualStageButton;
	CShadeButtonST m_ManualMeasurementButton;
	CShadeButtonST m_SampleLoadButton;
	CShadeButtonST m_SampleUnLoadButton;

	SYSTEM_CONFIG m_SystemConfig;
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	///// Function //////////////////////////////////////////////////////////
	void SelectRecipeButton(int Select);
	void ButtonEnableChange(int Select);
	void MeasStart(WORD wNumScans);

	/////////////////////////////////////////////////////////////////////////////
	// strtrim
	LPSTR strtrim(LPSTR str)
	{
		static char buff[STRFUNC_BUF_LEN+1];
		CString string;

		memset(buff, NULL, sizeof(buff));
		string = str;
		string.TrimLeft();
		string.TrimRight();
		strncpy(buff, string.GetBuffer(0), STRFUNC_BUF_LEN);

		return buff;
	};


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMeasurementOperationView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CMeasurementOperationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMeasurementOperationView)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStartButton();
	afx_msg void OnHandlerLoadPositionButton();
	afx_msg void OnManualLoadPositionButton();
	afx_msg void OnVacuumButton();
	afx_msg void OnGlassLifterButton();
	afx_msg void OnReferenceButton();
	afx_msg void OnManualMeasurementButton();
	afx_msg void OnMainRecipeButton();
	afx_msg void OnMultiRecipeButton();
	afx_msg void OnSampleLoadButton();
	afx_msg void OnSampleUnloadButton();
	afx_msg void OnManualStageButton();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	afx_msg LRESULT OnMoveSetposEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMeaOpeFuncCall(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

	//Saiki 20091226 Add ----->
	MS_CONFIG m_MsConfig;
	//Saiki 20091226 Add <-----

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEASUREMENTOPERATIONVIEW_H__4652614C_9749_4E16_9843_96D5CD9F9F72__INCLUDED_)
