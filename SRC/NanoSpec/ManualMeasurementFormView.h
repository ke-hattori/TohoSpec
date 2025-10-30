#if !defined(AFX_MANUALMEASUREMENTFORMVIEW_H__6FC13CEA_2E40_4241_A264_8E4D0429D56C__INCLUDED_)
#define AFX_MANUALMEASUREMENTFORMVIEW_H__6FC13CEA_2E40_4241_A264_8E4D0429D56C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ManualMeasurementFormView.h : ヘッダー ファイル
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\\..\\INC\\Globals.hxx"
#include "NanoUI.h"
#include "BtnST.h"
#include "Misc.h"
#include "MotionComboBox.h"
#include "ColorStatic.h"

// 2009.11.09 bagus MS 修正 --{--
#define CHECK_CYLINDER_TIMER		(10)
#define CYLINDER_MODE_LAMP_TIMER	(11)
// 2009.11.09 bagus MS 修正 --}--

class CAdditionalMeasurementDlg; //#include "AdditionalMeasurementDlg.h"


class CManualMeasurementFormView : public CNanoUI
{
protected:
	CManualMeasurementFormView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CManualMeasurementFormView)

// インプリメンテーション
	virtual ~CManualMeasurementFormView();

// フォーム データ
public:
	//{{AFX_DATA(CManualMeasurementFormView)
	enum { IDD = IDD_MANUAL_MEASUREMENT_OPERATION_FORM };
	//}}AFX_DATA

public:
	CShadeButtonST	m_MeasurementButton;
	CShadeButtonST	m_MeasurementCompleteButton;
	CShadeButtonST	m_PointTeachingButton;
	CShadeButtonST	m_ExitButton;
	CMotionButton	m_XY_UpButton;
	CMotionButton	m_XY_DownButton;
	CMotionButton	m_XY_RightButton;
	CMotionButton	m_XY_LeftButton;
	CMotionComboBox m_cbMotionStage;
	CJoyStickRadioButton*	m_joyStick1;
	CJoyStickRadioButton*	m_joyStick2;
//Saiki 20090924 Add ----->
	CColorStatic m_stcMeasurementPointX;
	CColorStatic m_stcMeasurementPointY;
//Saiki 20090924 Add <-----
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
	BOOL m_bNexMntSwOld;
	BOOL m_bNexEngiMntSwOld;
	int m_iProcessStatusOld;
//Saiki 20090924 Add ----->
	SR_CONFIG m_SrConfig;
//Saiki 20090924 Add <-----
	CAdditionalMeasurementDlg* m_pAddMeasDlgObj; //CAdditionalMeasurementDlgのオブジェクトのポインタ格納用


	void ClearReference(char* szRecipeName);
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//	void ManuMeas(void);
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
	BOOL ManuMeas(void);
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	BOOL GetReferenceManuStart(void);
	void ButtonEnableChange(int Select);
	void JoyStickXyEnable(BOOL bEnable);
//	void SoftJoyStickXyEnable(BOOL bEnable);
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 -->
	void JoyStickEnableCheck(void);
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 <--
//Saiki 20090924 Add ----->
	void SetPointData(SR_REFERENCE_POSITION, int);
	void Init_SR_XYStageMenu();
	void Init_SR_Transmittance_StageMenu();
//Saiki 20090924 Add <-----

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CManualMeasurementFormView)
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL


protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CManualMeasurementFormView)
	afx_msg void OnDestroy();
	afx_msg void OnMeasurementButton();
	afx_msg void OnMeasurementCompleteButton();
	afx_msg void OnPointTeachingButton();
	afx_msg void OnExitButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
/* deleted 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
//	afx_msg void OnJoyStick();
/* deleted 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- } ---------- */
	afx_msg void OnSelchangeComboMeasPoint();
	afx_msg void OnSelendokMicroScopeFocusPosition();
	//}}AFX_MSG
	LRESULT OnDoStageMove(WPARAM, LPARAM);
	LRESULT OnStageMoveDone(WPARAM, LPARAM);
	afx_msg LRESULT OnManuMeaFormFuncCall(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnManuModeOnePointEnd(WPARAM wparam, LPARAM lparam);
/* added 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
	LRESULT OnJoyStick(WPARAM, LPARAM);
/* added 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- } ---------- */

// 2009.11.09 bagus MS 修正 --{--
	afx_msg LRESULT OnNexResponse(WPARAM wparam, LPARAM lparam);
// 2009.11.09 bagus MS 修正 --}--
	DECLARE_MESSAGE_MAP()

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

private:
	SYSTEM_CONFIG m_SystemConfig;
// 2009.05.28 K.Matsuo SPT <--
	BOOL m_bIsMoveAtSpeed;
// 2009.05.28 K.Matsuo SPT -->

// 2009.10.30 bagus MS 追加 --{--
	void InitCombo_Lens();
// 2009.10.30 bagus MS 追加 --}--

// 2009.11.02 bagus MS 追加 --{--
	MS_CONFIG m_MsConfig;
	BOOL m_bMSModeLampFilcker;
// 2009.11.02 bagus MS 追加 --}--

// 2009.11.09 bagus MS 修正 --{--
	int		m_nSelLens;
// 2009.11.09 bagus MS 修正 --}--
// matsuhisa 2009.12.26 added >>>
	int		m_nLensSensorItem;
// matsuhisa 2009.12.26 added <<<

// 2013.01.11 bagus jog direction -->
	JOG_DIR_SETTING m_JogDirSetting;
// 2013.01.11 bagus jog direction -->
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MANUALMEASUREMENTFORMVIEW_H__6FC13CEA_2E40_4241_A264_8E4D0429D56C__INCLUDED_)
