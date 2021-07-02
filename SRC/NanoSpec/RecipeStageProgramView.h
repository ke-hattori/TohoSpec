#if !defined(AFX_RECIPESTAGEPROGRAMVIEW_H__9F714F32_CA93_431F_8D3A_1D881C8552B6__INCLUDED_)
#define AFX_RECIPESTAGEPROGRAMVIEW_H__9F714F32_CA93_431F_8D3A_1D881C8552B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeStageProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeStageProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用
#include "RecipeStageProgramMeasurementPointGrid.h"

class CRecipeStageProgramView : public CNanoRecipeUI
{
protected:
	CRecipeStageProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeStageProgramView)

// フォーム データ
public:
	//{{AFX_DATA(CRecipeStageProgramView)
	enum { IDD = IDD_RECIPE_STAGE_PROGRAM_FORM };
	CNanoPointGraph	m_SamplePointGraph;
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
	CColorStatic m_stcDeskewMode;
	CColorStatic m_stcSitePatternMode;

	CShadeButtonST m_DispositionButton;
	CShadeButtonST m_TeachingButton;
	CShadeButtonST m_AllClearButton;
	CShadeButtonST m_DeskewButton;
	CShadeButtonST m_SitePRSetButton;
	CShadeButtonST m_SitePRDelButton;
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
// 2013.01.17 bagus rotate xy view -->
	long RotateXyAxisDir(long lDir);
// 2013.01.17 bagus rotate xy view <--

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeStageProgramView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CRecipeStageProgramView();
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
	//{{AFX_MSG(CRecipeStageProgramView)
	afx_msg void OnDispositionButton();
	afx_msg void OnTeachingButton();
	afx_msg void OnAllClearButton();
	afx_msg void OnDeskewButton();
	afx_msg void OnPaint();
	afx_msg void OnAddButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnRedrawPointMap();
	afx_msg void OnSitePrSetButton();
	afx_msg void OnSitePrDelButton();
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
// 2009.10.05 bagus SE --{--
//	SAMPLEINFO			m_SampleInfo;
// 2009.10.05 bagus SE --}--

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
	double m_dPointBuff[3][SCAN_POINT_MAX];

	STAGE_COORD		m_ScanPoint[SCAN_POINT_MAX];
	STAGE_COORD		m_OldScanPoint[SCAN_POINT_MAX];
	CCellID m_CurrentCellID;

	WORD			m_wNumSite;
	SITE_PATTERN	m_SitePoint[SCAN_POINT_MAX];
	SITE_PATTERN	m_OldSitePoint[SCAN_POINT_MAX];

// 2009.10.24 bagus 2点間 修正 --{--
	BOOL	CheckValidPointX(long lPointX);
	BOOL	CheckValidPointY(long lPointY);
// 2009.10.24 bagus 2点間 修正 --}--

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPESTAGEPROGRAMVIEW_H__9F714F32_CA93_431F_8D3A_1D881C8552B6__INCLUDED_)
