#if !defined(AFX_TESTMEASUREMENTDLG_H__80166F9D_ACA2_4BC6_8E44_1D5D1896CC2B__INCLUDED_)
#define AFX_TESTMEASUREMENTDLG_H__80166F9D_ACA2_4BC6_8E44_1D5D1896CC2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestMeasurementDlg.h : ヘッダー ファイル
//
#include "GridCtrl.h"
#include "BtnST.h"
#include "Misc.h"
#include "MotionComboBox.h"
#include "ColorStatic.h"

class CMeasurementGraphDlg2;

/////////////////////////////////////////////////////////////////////////////
// CTestMeasurementDlg ダイアログ

class CTestMeasurementDlg : public CDialog
{
// コンストラクション
public:
	CTestMeasurementDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTestMeasurementDlg)
	enum { IDD = IDD_TEST_MEASUREMENT_DLG };
	CShadeButtonST	m_AutoFocusButton;
	CShadeButtonST	m_ReferenceButton;
	CShadeButtonST	m_MeasurementButton;
	CShadeButtonST	m_AfMeasurementButton;
	CMotionButton	m_XY_UpButton;
	CMotionButton	m_XY_DownButton;
	CMotionButton	m_XY_RightButton;
	CMotionButton	m_XY_LeftButton;
	CMotionComboBox m_cbMotionStage;
	CMotionButton	m_Z_UpButton;
	CMotionButton	m_Z_DownButton;
	CMotionButton	m_Z_HomePosButton;
	CMotionElevComboBox m_cbMotionElev;
	//}}AFX_DATA
	CColorStatic m_stcMeasurementPointX;
	CColorStatic m_stcMeasurementPointY;
public:
	CJoyStickRadioButton*	m_joyStick1;
	CJoyStickRadioButton*	m_joyStick2;

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	CTabCtrl m_MeasurementTab;
	CMeasurementGraphDlg2* m_pMeasurementGraphDlg1;
	CMeasurementGraphDlg2* m_pMeasurementGraphDlg2;

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
/* added 2015.03.10 hmenjo テスト測定保存 ---------- { ---------- */
	CShadeButtonST m_BtnSave;
/* added 2015.03.10 hmenjo テスト測定保存 ---------- } ---------- */

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
	//{{AFX_VIRTUAL(CTestMeasurementDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTestMeasurementDlg)
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeMeasurementTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAutoFocusButton();
	afx_msg void OnReferenceButton();
	afx_msg void OnMeasurementButton();
	afx_msg void OnChangeMeasIntegrationTime();
	afx_msg void OnChangeWavelengthStart();
	afx_msg void OnChangeWavelengthEnd();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAfAndMeasurementButton();
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
/* deleted 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
//	afx_msg void OnJoyStick();
/* deleted 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- } ---------- */
	afx_msg void OnSelchangeComboMeasPoint();
	afx_msg void OnHomePositonButton();
/* added 2015.03.10 hmenjo テスト測定保存 ---------- { ---------- */
	afx_msg void OnBtnSave();
/* added 2015.03.10 hmenjo テスト測定保存 ---------- } ---------- */
	//}}AFX_MSG
	LRESULT OnDoStageMove(WPARAM, LPARAM);
	LRESULT OnStageMoveDone(WPARAM, LPARAM);
	LRESULT OnDoElevatorMove(WPARAM, LPARAM);
	afx_msg LRESULT OnTestModeOnePointEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTestModeRefEnd(WPARAM wParam, LPARAM lParam);
/* added 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
	LRESULT OnJoyStick(WPARAM, LPARAM);
/* added 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- } ---------- */
	DECLARE_MESSAGE_MAP()

//	virtual void ButtonEnableCheck(void);

private:
	void MeasurementTab_Init();
	void LoadRecipeData();
	void InitCombo_Lens();
	void OnSelchangeSrLens();
	void InitCombo_SrOpticalFilter();
	void OnSelchangeSrOptFilter();
	BOOL MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName, int iRecipeTipe);
	BOOL DummyRcpDelete();
	void StartTestModeMeas();
	BOOL CheckData();
	void SoftJoyStickXyzEnable(BOOL bEnable);
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 -->
	void JoyStickEnableCheck(void);
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 <--
//Saiki 20090908 Add ----->
	void Init_SR_XYStageMenu();
	void Init_SR_Transmittance_StageMenu();
	void SetPointData(SR_REFERENCE_POSITION, int);
//Saiki 20090908 Add <-----

private:
	void TabChange(int Index);
	void ClearReference(char* szRecipeName);


	MAIN_RCP_INFO m_MainRcpInfo;
	CFont m_Font;
	SR_CONFIG m_SrConfig;
	MEAS_PROG_INFO m_Meas;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	SYSTEM_CONFIG m_SystemConfig;

	// ----------------------------------------------------------------------
	//
	inline int GetMotionJog(WORD wAxis, UINT nMode)
	{
		if(m_SystemConfig.nStageType == STAGE_TYPE_SPT){
			STAGE_MOTION_SPEED_DATA StageMotionSpeedData[MAX_AXIS];
			ConfigFile_GetNanoSpecIni(&StageMotionSpeedData[0], CONFIG_FILE_STAGE_MOTION_SPEED);

			if(nMode >= STAGE_MOTION_JOG_LOW){
				return StageMotionSpeedData[wAxis].Jog;
			}else{
				return StageMotionSpeedData[wAxis].Speed[nMode];
			}
		}else{
			STAGE_MOTION_JOG_DATA StageMotionJogData[MAX_AXIS];
			ConfigFile_GetNanoSpecIni(&StageMotionJogData[0], CONFIG_FILE_STAGE_MOTION_JOG);

			return StageMotionJogData[wAxis].Jog[nMode];
		}
	}

// 2009.05.28 K.Matsuo SPT -->
	BOOL m_bIsMoveAtSpeed;
// 2009.05.28 K.Matsuo SPT <--

// 2013.01.11 bagus jog direction -->
	JOG_DIR_SETTING m_JogDirSetting;
// 2013.01.11 bagus jog direction -->
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TESTMEASUREMENTDLG_H__80166F9D_ACA2_4BC6_8E44_1D5D1896CC2B__INCLUDED_)
