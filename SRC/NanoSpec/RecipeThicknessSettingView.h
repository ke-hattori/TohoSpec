#if !defined(AFX_RECIPETHICKNESSSETTINGVIEW_H__CEA972A0_713A_465A_A84F_5A75382EF20E__INCLUDED_)
#define AFX_RECIPETHICKNESSSETTINGVIEW_H__CEA972A0_713A_465A_A84F_5A75382EF20E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeThicknessSettingView.h : header file
//

#include "ColorStatic.h"
#include "GridCtrl.h"
#include "NanoPointGraph.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用

/////////////////////////////////////////////////////////////////////////////
// CRecipeThicknessSettingView dialog

class CRecipeThicknessSettingView : public CDialog
{
// Construction
public:
	CRecipeThicknessSettingView(CWnd* pParent = NULL);	 // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecipeThicknessSettingView)
	enum { IDD = IDD_RECIPE_THICK_SETTING_FORM };
	CNanoPointGraph m_StageMapGraph;
	//}}AFX_DATA
public:
	CMainFrame* 		m_pMainFrame;
	CNanoSpecDoc*		m_pDoc;
	STRESS_PARAM_INFO	m_StressParamInfo;
	STRESS_PARAM_INFO	m_OldStressParamInfo;
	MAIN_RCP_INFO 		m_MainRcpInfo;
	STAGE_PROG_STRESS	m_StageStressProg;
	UINT				m_nOpenMode;
	enum nOpenMode{modeNormal = 0, modeNew, modeDefaultSetting};

protected:
	CGridCtrl			m_MeasurementPointForStressListGrid;

	CShadeButtonST		m_ThicknessCopyButton;
	CShadeButtonST		m_ThicknessClearButton;
	CShadeButtonST		m_OkButton;
	CShadeButtonST		m_CancelButton;

	STRESS_CONFIG		m_StressConfig;
	STAGE_CONFIG		m_StageConfig;
	SAMPLEINFO			m_SampleInfo;
	STAGE_COORD 		m_ScanPoint[SCAN_POINT_MAX * 2];

	CCellID 			m_CurrentCellID;

	void SamplePointGraph_Init();
	void SamplePointGraph_DataSet();

	void DrawSample(CDC* pDC, int Org, int DirX, int DirY);
// 2013.01.09 bagus -->
//	void DrawOriginMaker(int Origin);
// 2013.01.09 bagus <--
	void DrawAxisCoordinate(int iOrgPos, int DirX, int DirY);
	void LoadThicknessData();
	BOOL CheckData();
	void Update(STRESS_PARAM_INFO* tempStressInfo);
	BOOL CheckInputValueLong(CString strVal, long *lValue);
	BOOL CheckInputValueDouble(CString strVal, double *dValue);

private:
	void MeasurementPointForStressListGrid_Init();
	int m_nSysVScrollWidth;
	char m_szName[RECIPE_NAME_LEN + 1];

	CBrush* 			m_oldBrush;
	CPen*				m_oldPen;

	double m_dDrawStartStagePosX;
	double m_dDrawEndStagePosX;
	double m_dDrawStartStagePosY;
	double m_dDrawEndStagePosY;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecipeThicknessSettingView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecipeThicknessSettingView)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnButtonThicknessCopy();
	afx_msg void OnButtonThicknessClear();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG

	afx_msg void OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECIPETHICKNESSSETTINGVIEW_H__CEA972A0_713A_465A_A84F_5A75382EF20E__INCLUDED_)
