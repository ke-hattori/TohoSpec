#if !defined(AFX_TESTMEASUREMENTGANTRYDLG_H__B84DD2CB_1670_4A80_88E8_248F4BBB0C55__INCLUDED_)
#define AFX_TESTMEASUREMENTGANTRYDLG_H__B84DD2CB_1670_4A80_88E8_248F4BBB0C55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestMeasurementGantryDlg.h : ヘッダー ファイル
//
#include "GridCtrl.h"
#include "BtnST.h"
#include "Misc.h"
#include "MotionComboBox.h"
#include "ColorStatic.h"

class CMeasurementGraphDlg2;

/////////////////////////////////////////////////////////////////////////////
// CTestMeasurementGantryDlg ダイアログ

class CTestMeasurementGantryDlg : public CDialog
{
// コンストラクション
public:
	CTestMeasurementGantryDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTestMeasurementGantryDlg)
	enum { IDD = IDD_TEST_MEASUREMENT_GANTRY_DLG };
	CShadeButtonST	m_ReferenceButton;
	CShadeButtonST	m_OnePointReferenceButton;
	CShadeButtonST	m_ReferenceCancelButton;
	//}}AFX_DATA
	CColorStatic m_stcMeasurementPointX;
	CColorStatic m_stcMeasurementPointY;

public:
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	CTabCtrl m_MeasurementTab;
	CMeasurementGraphDlg2* m_pMeasurementGraphDlg1;
	CMeasurementGraphDlg2* m_pMeasurementGraphDlg2;

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	int m_iConditionFlg;
	int m_iConditionFlgOld;	//前回値
	int m_iProcessStatus;
	int m_iProcessStatusOld; //前回値
	BOOL m_AfExecuteFlg;
	BOOL m_AfExecuteFlgOld; //前回値
	int m_iAlarmStatus;
	int m_iAlarmStatusOld; //前回値

	// 外部との引渡し変数
	CString m_strLens;
	CString m_strOpticalFilter;
	double	m_dIntegTime;
	WORD	m_wStart;
	WORD	m_wEnd;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTestMeasurementGantryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTestMeasurementGantryDlg)
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeMeasurementTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReferenceButton();
	afx_msg void OnOnePointReferenceButton();
	afx_msg void OnReferenceCancelButton();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillfocusWavelengthStart();
	afx_msg void OnKillfocusMeasIntegrationTime();
	afx_msg void OnKillfocusWavelengthEnd();
	afx_msg void OnSetfocusSrLens();
	afx_msg void OnSetfocusSrOptFilter();
	afx_msg void OnKillfocusSrOptFilter();
	afx_msg void OnSetfocusMeasIntegrationTime();
	afx_msg void OnSetfocusWavelengthStart();
	afx_msg void OnSetfocusWavelengthEnd();
	afx_msg void OnKillfocusSrLens();
	afx_msg void OnSelchangeComboMeasPoint();
	//}}AFX_MSG
	afx_msg LRESULT OnTestModeOnePointEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTestModeRefEnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void MeasurementTab_Init();
	void LoadRecipeData();
	void InitCombo_Lens();
	void OnSelchangeSrLens();
	void InitCombo_SrOpticalFilter();
	void OnSelchangeSrOptFilter();
	BOOL MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName, int iRecipeTipe);
	BOOL MakeTempSrGantryStageRecipe(LPCTSTR pszRecipeName);
	BOOL DummyRcpDelete();
	void StartTestModeMeas();
	BOOL CheckData();

	void SetPointData(SR_REFERENCE_POSITION, int);

private:
	void TabChange(int Index);
	void ClearReference(char* szRecipeName);

	MAIN_RCP_INFO m_MainRcpInfo;
	CFont m_Font;
	SR_CONFIG m_SrConfig;
	MEAS_PROG_INFO m_Meas;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	SYSTEM_CONFIG m_SystemConfig;

	BOOL m_bIsMoveAtSpeed;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TESTMEASUREMENTGANTRYDLG_H__B84DD2CB_1670_4A80_88E8_248F4BBB0C55__INCLUDED_)
