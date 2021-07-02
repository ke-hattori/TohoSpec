#if !defined(AFX_RECIPEMAINSRVIEW_H__11723BD0_0DEA_427E_AD85_81D1977E868B__INCLUDED_)
#define AFX_RECIPEMAINSRVIEW_H__11723BD0_0DEA_427E_AD85_81D1977E868B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMainSrView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMainSrView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoPointGraph.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用

class CRecipeMainSrView : public CNanoRecipeUI
{
protected:
	CRecipeMainSrView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMainSrView();
	DECLARE_DYNCREATE(CRecipeMainSrView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMainSrView)
	enum { IDD = IDD_RECIPE_MAIN_SR_FORM };
	CString m_strHeadType;
	CString m_strMeasHeadType;
	CString m_strPointDeskewHeadType;
	CString m_strPointDeskewLens;
	CString m_strRecalib1HeadType;
	CString m_strRecalib2HeadType;
	CString m_strRecalib3HeadType;
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcMainRecipeName;
	CColorStatic m_stcHeadType;
	CColorStatic m_stcMeasHeadType;
	CColorStatic m_stcPointDeskewLens;
	CColorStatic m_stcPointDeskewHeadType;
	CColorStatic m_stcRecalib1HeadType;
	CColorStatic m_stcRecalib2HeadType;
	CColorStatic m_stcRecalib3HeadType;

	SR_TURRET m_SrTurret[SR_LENS_MAX];
	MAIN_RCP_INFO m_MainRcpInfo;
	MAIN_RCP_INFO m_OldMainRcpInfo;
	MEAS_PROG_INFO m_MeasProgInfo;
	SYSTEM_CONFIG m_SystemConfig;
	SR_CONFIG m_SrConfig;

	//2009.09.02 bagus se 機能拡張 --{--
	CString m_strStageHeadType;
	CColorStatic m_stcStageHeadType;
	CNanoPointGraph	m_SamplePointGraph;
	//2009.09.02 bagus se 機能拡張 --}--

//Saiki 20110207 Add ----->
	CShadeButtonST m_ThickSettingButton;
//Saiki 20110207 Add <-----

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMainSrView)
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

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeMainSrView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeMeasurementProgram();
	afx_msg void OnSelchangePointDeskewProgram();
	afx_msg void OnSelchangeRecalibration1();
	afx_msg void OnSelchangeRecalibration2();
	afx_msg void OnSelchangeRecalibration3();
	afx_msg void OnSelchangePointManualAdjustment();
	afx_msg void OnSelchangeFocus();
	afx_msg void OnSelchangeFocusIfFail();
	afx_msg void OnSelchangeDefaultDisplay();
	afx_msg void OnSelchangeSampleId();
	afx_msg void OnSelchangeRecalibrationItem1();
	afx_msg void OnSelchangeRecalibrationItem2();
	afx_msg void OnSelchangeRecalibrationItem3();
	afx_msg void OnSelchangeFinalFocus();
	afx_msg void OnSelchangeStageProgram();
	afx_msg void OnMeasurementResultAutoSave();
	afx_msg void OnThickSettingButton();
	afx_msg void OnRadioThicknessTypeForStressValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void DefaultSetting_Init();
	void InitCombo_MeasurementProgram();
	void InitCombo_StageProgram();
	void InitCombo_PointDeskewProgram();
	void InitCombo_RecalibrationProgram(CComboBox* pCombo, LPCTSTR pszRecalib);
	void InitCombo_RecalibItemSetting(int iRicalibSetIndex1=0,int iRicalibSetIndex2=0,int iRicalibSetIndex3=0);
	void InitCombo_PointManualAdjustment();
	void InitCombo_Focus();
	void InitCombo_FocusIfFail();
	void InitCombo_DefaultDisplay();
	void RecalibItemSetting();
//Saiki 20110201 Add ----->
	void InitThickOptionsForStress(BOOL bEnable=FALSE) ;
	void EnableRecalibrationForStress(BOOL bEnable=TRUE);
	void OnSelectThickOptionsForStress(BOOL bEnable=FALSE);
	BOOL CheckStressThicknessData(MAIN_RCP_INFO);
	BOOL CheckInputValueLong(CString strVal, long *lValue);
	BOOL CheckInputValueDouble(CString strVal, double *dValue);
//Saiki 20110201 Add <-----
//2009.09.02 bagus se 機能拡張 --{--
	void SamplePointGraph_Init();
	void SamplePointGraph_DataSet();
// 2013.01.17 bagus rotate xy view -->
	long RotateXyAxisDir(long lDir);
// 2013.01.17 bagus rotate xy view <--

// 2013.01.09 bagus -->
	void ToXyAxisCoord(STAGE_COORD* lpPoint);
// 2013.01.09 bagus <--
	void DrawSample(CDC* pDC, int Org, int DirX, int DirY);
// 2013.01.09 bagus -->
//	void DrawOriginMaker(int Origin);
// 2013.01.09 bagus <--
//2009.09.02 bagus se 機能拡張 --}--

public:
	//2009.08.28 bagus stress --{--
	//新規作成時のHEAD TYPE区別用の為の変数
	int m_iNewHeadType;
	//2009.08.28 bagus stress --}--

//2009.09.02 bagus se 機能拡張 --{--
protected:
	STAGE_CONFIG		m_StageConfig;
	STRESS_CONFIG		m_StressConfig;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;
// 2009.10.28 bagus 2点間 --{--
//	STAGE_COORD			m_ScanPoint[SCAN_POINT_MAX];
	STAGE_COORD			m_ScanPoint[SCAN_POINT_MAX * 2];
// 2009.10.28 bagus 2点間 --}--
	STAGE_PROG_STRESS	m_StageStressProg;
//Saiki 20110215 Add ----->
	STRESS_PARAM_INFO	m_StressParamInfo;
	STRESS_PARAM_INFO	m_OldStressParamInfo;
//Saiki 20110215 Add <-----
private:
	CBrush* m_oldBrush;
	CPen* m_oldPen;

	double m_dDrawStartStagePosX;
	double m_dDrawEndStagePosX;
	double m_dDrawStartStagePosY;
	double m_dDrawEndStagePosY;
//2009.09.02 bagus se 機能拡張 --}--

// 2009.10.02 bagus Gantry --{--
	WORD	m_SRScanType;

	BOOL	m_bChage;
// 2009.10.02 bagus Gantry --}--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMAINSRVIEW_H__11723BD0_0DEA_427E_AD85_81D1977E868B__INCLUDED_)
