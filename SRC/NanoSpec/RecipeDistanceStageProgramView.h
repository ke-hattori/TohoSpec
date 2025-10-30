#if !defined(AFX_RECIPEDISTANCESTAGEPROGRAMVIEW_H__49AF6761_082B_48A6_8BBE_F256A9051D8B__INCLUDED_)
#define AFX_RECIPEDISTANCESTAGEPROGRAMVIEW_H__49AF6761_082B_48A6_8BBE_F256A9051D8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeDistanceStageProgramView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRecipeDistanceStageProgramView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用
#include "RecipeStageProgramMeasurementPointGrid.h"

class CRecipeDistanceStageProgramView : public CNanoRecipeUI
{
protected:
	CRecipeDistanceStageProgramView();			 // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeDistanceStageProgramView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipeDistanceStageProgramView)
	enum { IDD = IDD_RECIPE_STAGE_DISTANCE_PROGRAM_FORM };
	CNanoPointGraph	m_SamplePointGraph;
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:

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
	CColorStatic m_stcDeskewMode;

	CShadeButtonST m_DispositionButton;
	CShadeButtonST m_TeachingButton;
	CShadeButtonST m_AllClearButton;
	CShadeButtonST m_DeskewButton;
	CShadeButtonST m_AddButton;
	CShadeButtonST m_DeleteButton;
	CShadeButtonST m_RedrawPointButton;
	CRecipeStageProgramMeasurementPointGrid m_MeasurementPointGrid;

	void ClearMeasurementPoint();
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
//	void DrawOriginMaker(int Origin);
// 2013.01.09 bagus <--
	void SamplePointGraph_DataSet();
	void SamplePointGraph_Init();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeDistanceStageProgramView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeDistanceStageProgramView();
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
	//{{AFX_MSG(CRecipeDistanceStageProgramView)
	afx_msg void OnDispositionButton();
	afx_msg void OnTeachingButton();
	afx_msg void OnAllClearButton();
	afx_msg void OnDeskewButton();
	afx_msg void OnPaint();
	afx_msg void OnAddButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnRedrawPointMap();
	//}}AFX_MSG

	afx_msg void OnEndLabelEdit(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	afx_msg void OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	afx_msg void OnSelChanged(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);

	DECLARE_MESSAGE_MAP()

private:
	SYSTEM_CONFIG		m_SystemConfig;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;
	STAGE_PROG_INFO_HDR m_OldStageProgInfoHdr;
	STAGE_CONFIG		m_StageConfig;

	CRgn				m_StageRgn;
	CRgn				m_WaferRgn;
	CBrush* m_oldBrush;
	CPen* m_oldPen;

	int m_iNumScans;
	int m_iWnd;

	double m_dDrawStartStagePosX;
	double m_dDrawEndStagePosX;
	double m_dDrawStartStagePosY;
	double m_dDrawEndStagePosY;
	double m_dPointBuff[3][SCAN_POINT_MAX * 2];

	STAGE_COORD		m_ScanPoint[SCAN_POINT_MAX * 2];
	STAGE_COORD		m_OldScanPoint[SCAN_POINT_MAX * 2];
	CCellID m_CurrentCellID;

	void MeasurementPointListGrid_Init();
	void SetMeasurementPointData();

	int	m_nSysVScrollWidth;

// 2009.10.24 bagus 2点間 修正 --{--
	BOOL	CheckValidPointX(long lPointX);
	BOOL	CheckValidPointY(long lPointY);
// 2009.10.24 bagus 2点間 修正 --}--

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEDISTANCESTAGEPROGRAMVIEW_H__49AF6761_082B_48A6_8BBE_F256A9051D8B__INCLUDED_)
