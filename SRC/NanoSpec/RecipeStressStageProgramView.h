#if !defined(AFX_RECIPESTRESSSTAGEPROGRAMVIEW_H__6EBAB248_4CD5_43AB_91E7_8EEB1848EDC1__INCLUDED_)
#define AFX_RECIPESTRESSSTAGEPROGRAMVIEW_H__6EBAB248_4CD5_43AB_91E7_8EEB1848EDC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeStressStageProgramView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeStressStageProgramView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用
#include "RecipeStageProgramMeasurementPointGrid.h"

class CRecipeStressStageProgramView : public CNanoRecipeUI
{
protected:
	CRecipeStressStageProgramView();		   // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeStressStageProgramView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipeStressStageProgramView)
	enum { IDD = IDD_RECIPE_STAGE_STRESS_PROGRAM_FORM };
	CNanoPointGraph	m_SamplePointGraph;
	//}}AFX_DATA
	CColorStatic m_stcStageProgramName;
	CColorStatic m_stcStageProgramNumScan;
	CColorStatic m_stcStageProgramLineSection;
	CColorStatic m_stcSampleName;
	CColorStatic m_stcSampleShape;
	CColorStatic m_stcSampleThickness;
	CColorStatic m_stcSampleSizeX;
	CColorStatic m_stcSampleSizeY;
	CColorStatic m_stcSampleOriginX;
	CColorStatic m_stcSampleOriginY;
// 2009.09.04 bagus stress デスキュー削除 --{--
//	CColorStatic m_stcDeskewMode;
// 2009.09.04 bagus stress デスキュー削除 --}--
	CRecipeStageProgramMeasurementPointGrid m_MeasurementPointGrid;
	CRecipeStageProgramMeasurementPointGrid m_MeasurementPointGrid2;

	CShadeButtonST m_AllCheckButton;
	CShadeButtonST m_AllClearButton;
	CShadeButtonST m_TeachingButton;
	CShadeButtonST m_RedrawPointButton;
// 2009.09.04 bagus stress デスキュー削除 --{--
//	CShadeButtonST m_DeskewButton;
// 2009.09.04 bagus stress デスキュー削除 --}--


// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeStressStageProgramView)
	public:
	virtual void OnInitialUpdate();
	// 2009.09.04 bagus stress Escｷｰ処理 --{--
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 2009.09.04 bagus stress Escｷｰ処理 --}--
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeStressStageProgramView();
	virtual BOOL IsDataChanged();
	virtual BOOL SaveRecipeData();
	virtual BOOL IsAccessPrivilege();
	virtual BOOL CheckData();
	virtual int SaveAsDlgInfo();
	virtual void Discard();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeStressStageProgramView)
	afx_msg void OnAllcheckButton();
	afx_msg void OnAllClearButton();
	afx_msg void OnTeachingButton();
	afx_msg void OnRedrawPointMap();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeMaterialElasticModulus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// Operation
protected:
	CToolBar m_wndEditBar;

	BOOL LoadRecipeData();

	void DrawSample(CDC* pDC, int Org, int DirX, int DirY);
	void DrawAxisCoordinate(int iOrgPos, int DirX, int DirY);
	void DrawPointMaker(int Origin, int PosX, int PosY, int DirX, int DirY, int iType);
// 2013.01.09 bagus -->
//	void DrawOriginMaker(int Origin);
// 2013.01.09 bagus <--
	void SamplePointGraph_DataSet();
	void SamplePointGraph_Init();

private:
// 2009.09.11 bagus stress 修正 --{--
	BOOL LoadScanPoint();
// 2009.09.11 bagus stress 修正 --}--
	BOOL CheckMeasurementLineSectionData();
// 2009.09.07 bagus stress Line/Section数修正 --{--
	WORD CheckLineSectionNum();
// 2009.09.07 bagus stress Line/Section数修正 --}--
	SYSTEM_CONFIG		m_SystemConfig;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;
	STAGE_PROG_INFO_HDR m_OldStageProgInfoHdr;
	STAGE_PROG_STRESS	m_StageStressProg;
	STAGE_PROG_STRESS	m_OldStageStressProg;

	STAGE_CONFIG		m_StageConfig;
	STRESS_CONFIG		m_StressConfig;
// 2009.10.05 bagus SE --{--
//	SAMPLEINFO			m_SampleInfo;
// 2009.10.05 bagus SE --}--

	CRgn				m_StageRgn;
	CRgn				m_WaferRgn;
	CBrush* m_oldBrush;
	CPen* m_oldPen;

	int m_iNumScans;
	int m_iWnd;

	void MeasurementLineSectionListGrid_Init();
	void SetMeasurementLineSectionData();

	void MeasurementPointListGrid_Init();
	void SetMeasurementPointData();

	void AdjustValidCheckBoxPos();

	void SetData();
	void UpDate();

	double m_dDrawStartStagePosX;
	double m_dDrawEndStagePosX;
	double m_dDrawStartStagePosY;
	double m_dDrawEndStagePosY;
	double m_dPointBuff[3][SCAN_POINT_MAX];

	STAGE_COORD		m_ScanPoint[SCAN_POINT_MAX];
	STAGE_COORD		m_OldScanPoint[SCAN_POINT_MAX];
	CCellID m_CurrentCellID;

	WORD			m_wNumSite;
	SITE_PATTERN	m_SitePoint[SCAN_POINT_MAX];
	SITE_PATTERN	m_OldSitePoint[SCAN_POINT_MAX];

	//2009.09.01 bagus stress --{--
	//グリッド上にチェックボックスを置くための処理
	//struct {
	//	CGridCtrl *Grid;
	//	CButton *CheckBox[STRESS_LINES_MAX*STRESS_SECTIONS_MAX];
	//} m_LineCheckBox;
	//void SetLSValidCheckBox();
	//void ClearLSValidCheckBox();
	//2009.09.01 bagus stress --}--

// 2009.09.10 bagus stress 修正 --{--
	BOOL	m_bFirst;
	BOOL	m_bUseLine[STRESS_LINES_MAX];
// 2009.09.10 bagus stress 修正 --}--

// 2009.09.17 bagus stress Grid修正 --{--
	int	m_nSysVScrollWidth;
// 2009.09.17 bagus stress Grid修正 --}--

// 2009.11.09 bagus Stress 追加変更 --{--
	STRESS_ELASTIC_TABLE	m_ElasticTable;
	void InitCombo_MaterialElasticModulus();
// 2009.11.09 bagus Stress 追加変更 --}--

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPESTRESSSTAGEPROGRAMVIEW_H__6EBAB248_4CD5_43AB_91E7_8EEB1848EDC1__INCLUDED_)
