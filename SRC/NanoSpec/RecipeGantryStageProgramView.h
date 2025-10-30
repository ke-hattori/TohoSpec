#if !defined(AFX_RECIPEGANTRYSTAGEPROGRAMVIEW_H__73E4DF5C_8BBB_4BA6_A5F3_C5212052735C__INCLUDED_)
#define AFX_RECIPEGANTRYSTAGEPROGRAMVIEW_H__73E4DF5C_8BBB_4BA6_A5F3_C5212052735C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeGantryStageProgramView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeGantryStageProgramView ダイアログ

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用
#include "RecipeStageProgramMeasurementPointGrid.h"

class CRecipeGantryStageProgramView : public CNanoRecipeUI
{
// コンストラクション
protected:
	CRecipeGantryStageProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeGantryStageProgramView)

// ダイアログ データ
public:
	//{{AFX_DATA(CRecipeGantryStageProgramView)
	enum { IDD = IDD_RECIPE_STAGE_GANTRY_PROGRAM_FORM };
	CNanoPointGraph	m_SamplePointGraph;
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// Operation
protected:
	CToolBar m_wndEditBar;
	CColorStatic m_stcStageProgramName;
	CColorStatic m_stcStageProgramNumScan;
	CColorStatic m_stcSampleName;
	CColorStatic m_stcSampleShape;
	CColorStatic m_stcSampleThickness;
	CColorStatic m_stcSampleSizeX;
	CColorStatic m_stcSampleSizeY;
	CColorStatic m_stcSampleOriginX;
	CColorStatic m_stcSampleOriginY;

	CShadeButtonST m_TeachingButton;

	CRecipeStageProgramMeasurementPointGrid m_MeasurementPointGrid;

	BOOL LoadRecipeData();
	void SetData();
	void UpDate();

// 2013.01.09 bagus -->
	void ToXyAxisCoord(STAGE_COORD* lpPoint);
// 2013.01.09 bagus <--
	void DrawSample(CDC* pDC, int Org, int DirX, int DirY);
	void DrawAxisCoordinate(int iOrgPos, int DirX, int DirY);
	void DrawPointMaker(int Origin, int PosX, int PosY, int DirX, int DirY, int iType);
// 2013.01.09 bagus -->
// 	void DrawOriginMaker(int Origin);
// 2013.01.09 bagus <--
	void SamplePointGraph_DataSet();
	void SamplePointGraph_Init();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeGantryStageProgramView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeGantryStageProgramView();
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
	//{{AFX_MSG(CRecipeGantryStageProgramView)
	afx_msg void OnTeachingButton();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

// 2010.01.29 bagus Gantry 修正 --{--
	afx_msg void OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	afx_msg void OnSelChanged(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
// 2010.01.29 bagus Gantry 修正 --}--

	DECLARE_MESSAGE_MAP()

private:
	SYSTEM_CONFIG		m_SystemConfig;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;
	STAGE_PROG_INFO_HDR m_OldStageProgInfoHdr;
	STAGE_CONFIG		m_StageConfig;
// 2009.10.05 bagus SE --{--
//	SAMPLEINFO			m_SampleInfo;
// 2009.10.05 bagus SE --}--

	SR_CONFIG	m_SrConfig;
	SR_REFERENCE_POSITION m_SrReferencePosition;

	CRgn	m_StageRgn;
	CRgn	m_WaferRgn;
	CBrush* m_oldBrush;
	CPen* m_oldPen;

	int m_iNumScans;
	int m_iWnd;

	double m_dDrawStartStagePosX;
	double m_dDrawEndStagePosX;
	double m_dDrawStartStagePosY;
	double m_dDrawEndStagePosY;

	STAGE_COORD		m_ScanPoint[SCAN_POINT_MAX];
	STAGE_COORD		m_OldScanPoint[SCAN_POINT_MAX];

	BOOL			m_ValidPoint[SCAN_POINT_MAX];
	BOOL			m_OldValidPoint[SCAN_POINT_MAX];

	void MeasurementPointListGrid_Init();
	void SetMeasurementPointData();

	BOOL CheckMeasurementPointData();

	void CheckValidPoint();

	int	m_nSysVScrollWidth;

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEGANTRYSTAGEPROGRAMVIEW_H__73E4DF5C_8BBB_4BA6_A5F3_C5212052735C__INCLUDED_)
