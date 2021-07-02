#if !defined(AFX_MEASUREMENTPROCESSVIEW_H__CC07C5A1_5F1F_4C88_AD4C_F5A450948A84__INCLUDED_)
#define AFX_MEASUREMENTPROCESSVIEW_H__CC07C5A1_5F1F_4C88_AD4C_F5A450948A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasurementProcessView.h : ヘッダー ファイル
//

// ##########################################################################
// CMeasurementProcessView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

// 2009.10.30 bagus MS 追加 --{--
#include "..\\..\\INC\\Globals.hxx"
// 2009.10.30 bagus MS 追加 --}--
#include "NanoUI.h"
// 2009.10.30 bagus MS 追加 --{--
#include "BtnST.h"
#include "Misc.h"
#include "MotionComboBox.h"
#include "ColorStatic.h"
// 2009.10.30 bagus MS 追加 --}--

// 2009.11.09 bagus MS 修正 --{--
#define CHECK_CYLINDER_TIMER		(10)
// 2009.11.09 bagus MS 修正 --}--

class CMeasurementProcessView : public CNanoUI
{
protected:
	CMeasurementProcessView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CMeasurementProcessView)

// フォーム データ
public:
	//{{AFX_DATA(CMeasurementProcessView)
	enum { IDD = IDD_MEASUREMENT_PROCESS_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CShadeButtonST m_MeasurementCancelButton;
	CShadeButtonST m_MeasurementPauseButton;
	CShadeButtonST m_RemeasurementButton;
	CShadeButtonST m_AdujstRemeasurementButton;
	CShadeButtonST m_ValuesToDisplayButton;
	CShadeButtonST m_SaveButton;
	CShadeButtonST m_PrintButton;
	CShadeButtonST m_ExitButton;
// 2009.10.30 bagus MS 追加 --{--
	CShadeButtonST m_NextPointButton;

	CShadeButtonST	m_PointTeachingButton;
	CMotionButton	m_XY_UpButton;
	CMotionButton	m_XY_DownButton;
	CMotionButton	m_XY_RightButton;
	CMotionButton	m_XY_LeftButton;
	CMotionComboBox m_cbMotionStage;
	CJoyStickRadioButton*	m_joyStick1;
	CJoyStickRadioButton*	m_joyStick2;
	CColorStatic m_stcMeasurementPointX;
	CColorStatic m_stcMeasurementPointY;
// 2009.10.30 bagus MS 追加 --}--
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	BOOL StartRemeasurement(int iPma);

// 2009.10.30 bagus MS 追加 --{--
	void Init_SR_XYStageMenu();
// 2009.10.30 bagus MS 追加 --}--

// 2009.12.17 bagus MS 修正 --{--
	void ChangeOperationEnable(BOOL bEnable);
// 2009.12.17 bagus MS 修正 --}--

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMeasurementProcessView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CMeasurementProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMeasurementProcessView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMeasurementCancelButton();
	afx_msg void OnMeasurementPauseButton();
	afx_msg void OnExitButton();
	afx_msg void OnRemeasurementButton();
	afx_msg void OnSaveButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAdujstRemeasurementButton();
	afx_msg void OnNextPointButton();
	afx_msg void OnPointTeachingButton();
	afx_msg void OnDestroy();
	afx_msg void OnSelendokMicroScopeFocusPosition();
	//}}AFX_MSG

	afx_msg LRESULT OnMeaProcFuncCall(WPARAM wparam, LPARAM lparam);

// 2009.10.30 bagus MS 追加 --{--
	LRESULT OnJoyStick(WPARAM, LPARAM);
// 2009.10.30 bagus MS 追加 --}--

// 2009.11.09 bagus MS 修正 --{--
	afx_msg void OnNexResponse(WPARAM wparam, LPARAM lparam);
// 2009.11.09 bagus MS 修正 --}--
	DECLARE_MESSAGE_MAP()

// 2009.10.30 bagus MS 追加 --{--
private:
	SYSTEM_CONFIG m_SystemConfig;

	void InitCombo_Lens();
// 2009.10.30 bagus MS 追加 --}--

// 2009.11.02 bagus MS 追加 --{--
	MS_CONFIG m_MsConfig;
// 2009.11.02 bagus MS 追加 --}--

// 2009.11.09 bagus MS 修正 --{--
	int		m_nSelLens;
	BOOL	m_bMSModeLampFilcker;
// 2009.11.09 bagus MS 修正 --}--
// matsuhisa 2009.12.26 added >>>
	int		m_nLensSensorItem;
// matsuhisa 2009.12.26 added <<<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEASUREMENTPROCESSVIEW_H__CC07C5A1_5F1F_4C88_AD4C_F5A450948A84__INCLUDED_)
