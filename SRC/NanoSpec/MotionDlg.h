#if !defined(AFX_MOTIONDLG_H__D417222E_2E24_4FF7_8081_EFF9ADFA0C05__INCLUDED_)
#define AFX_MOTIONDLG_H__D417222E_2E24_4FF7_8081_EFF9ADFA0C05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionDlg.h : ヘッダー ファイル
//

// 2009.11.09 bagus MS 修正 --{--
#define CHECK_CYLINDER_TIMER		(10)
// 2009.11.09 bagus MS 修正 --}--

// ##########################################################################
// CMotionDlg ダイアログ
// ##########################################################################

class CMotionDlg : public CDialog
{
// コンストラクション
public:
	CMotionDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMotionDlg)
	enum { IDD = IDD_MOTION_DLG };
	//}}AFX_DATA

//Operation
public:
	CShadeButtonST	m_InitStageButton, m_InitAutoFocusButton, m_InitAutoTurretButton;
	CShadeButtonST	m_FilterOpenButton, m_FilterCloseButton, m_Filter1Button, m_Filter2Button, m_Filter3Button;
	CShadeButtonST	m_VacuumOnButton, m_VacuumOffButton;
	CShadeButtonST	m_CompEASEShutterOpenButton, m_CompEASEShutterCloseButton;
	CShadeButtonST	m_MaintenancePosition1Button, m_MaintenancePosition2Button, m_MaintenancePosition3Button, m_MaintenancePosition4Button;
	CShadeButtonST	m_SampleLoadingShutterOpenButton, m_SampleLoadingShutterCloseButton;
	CShadeButtonST	m_SampleLifterUpButton, m_SampleLifterDownButton, m_SampleLifterAlignmentButton;
	CShadeButtonST	m_SampleLoadSequenceLoadButton, m_SampleLoadSequenceUnloadButton;
	CShadeButtonST	m_SampleClampSequenceOpenButton, m_SampleClampSequenceCloseButton;
	CShadeButtonST	m_LoadSequenceCancelButton, m_ClampSequenceCancelButton;
	CShadeButtonST	m_CloseButton;
	CStatic			m_ctlWarningMessage;
// 2009.10.29 bagus MS 追加修正 --{--
	CShadeButtonST	m_MSMoveToPosButton, m_MSSTDModeButton, m_MSJogInchingButton;
// 2009.10.29 bagus MS 追加修正 --}--

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
//2009.11.10 bagus MS --{--
	SYSTEM_CONFIG m_SystemConfig;
//2009.11.10 bagus MS --}--

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMotionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMotionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnInitStageButton();
	afx_msg void OnInitAutoFocusButton();
	afx_msg void OnInitAutoTurretButton();
	afx_msg void OnFilterOpenButton();
	afx_msg void OnFilterCloseButton();
	afx_msg void OnFilter1Button();
	afx_msg void OnFilter2Button();
	afx_msg void OnFilter3Button();
	afx_msg void OnVacuumOnButton();
	afx_msg void OnVacuumOffButton();
	afx_msg void OnCompEASEShutterOpenButton();
	afx_msg void OnCompEASEShutterCloseButton();
	afx_msg void OnMaintenancePosition1Button();
	afx_msg void OnMaintenancePosition2Button();
	afx_msg void OnMaintenancePosition3Button();
	afx_msg void OnMaintenancePosition4Button();
	afx_msg void OnSampleLifterUpButton();
	afx_msg void OnSampleLifterAlignmentButton();
	afx_msg void OnSampleLifterDownButton();
	afx_msg void OnSampleLoadSequenceLoadButton();
	afx_msg void OnSampleLoadSequenceUnloadButton();
	afx_msg void OnSampleClampSequenceOpenButton();
	afx_msg void OnSampleClampSequenceCloseButton();
	afx_msg void OnLoadSequenceCancelButton();
	afx_msg void OnClampSequenceCancelButton();
	afx_msg void OnCloseButton();
	afx_msg void OnClose();
	afx_msg void OnSampleLoadingShutterOpenButton();
	afx_msg void OnSampleLoadingShutterCloseButton();
	virtual void OnCancel();
	afx_msg void OnMicroScopeMove();
	afx_msg void OnMicroScopeStdMode();
	afx_msg void OnMicroScopeJogIncing();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnNextraResponse(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

private:
	void SetOperationLog(LPCTSTR msg);
	BOOL m_bSampleLoadingShutterOpen;
	BOOL m_bSampleLoadingShutterClose;

	MAINTENANCE_POSITION m_TempMaintenancePosition[MAINTENANCE_POSITION_MAX];
	void SetWarningMessageText(int idsno);

// 2009.10.29 bagus MS 追加修正 --{--
	void InitCombo_Lens();
// 2009.10.29 bagus MS 追加修正 --}--

// 2009.11.09 bagus MS 修正 --{--
	MS_CONFIG m_MsConfig;
	int		m_nSelLens;
	BOOL	m_bMSModeLampFilcker;
// 2009.11.09 bagus MS 修正 --}--
// matsuhisa 2009.12.26 added >>>
	int		m_nLensSensorItem;
// matsuhisa 2009.12.26 added <<<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MOTIONDLG_H__D417222E_2E24_4FF7_8081_EFF9ADFA0C05__INCLUDED_)
