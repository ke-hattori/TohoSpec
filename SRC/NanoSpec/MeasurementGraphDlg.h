#if !defined(AFX_MEASUREMENTGRAPHDLG_H__8A7DCBC9_D781_47AA_AC53_C61DA9402589__INCLUDED_)
#define AFX_MEASUREMENTGRAPHDLG_H__8A7DCBC9_D781_47AA_AC53_C61DA9402589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasurementGraphDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"
#include "GridCtrl.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用


// ##########################################################################
// CMeasurementGraphDlg ダイアログ
// ##########################################################################

class CMeasurementGraphDlg : public CDialog
{
// コンストラクション
public:
	CMeasurementGraphDlg(int m_mode, UINT uResourceID = IDD_MEASUREMENT_GRAPH_DLG, CWnd* pParent = NULL);
	virtual ~CMeasurementGraphDlg();

// ダイアログ データ
	//{{AFX_DATA(CMeasurementGraphDlg)
	enum { IDD = IDD_MEASUREMENT_GRAPH_DLG };
	CNanoMeasureGraph	m_MeasureGraph;
	//}}AFX_DATA

protected:
	CGridCtrl m_StatisticsListGrid;
	CColorStatic m_stcMainRecipeName;
	CColorStatic m_stcHeadType;
	CColorStatic m_stcMeasurementItem;
	CColorStatic m_stcMeasurementPoint;
	CColorStatic m_stcMeasurementLS;

//Operation
public:
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
// 2014.01.17 Bagus Add (Stage None対応) -->
	SYSTEM_CONFIG m_SystemConfig;
// 2014.01.17 Bagus Add (Stage None対応) <--
	CGridCtrl m_MeasurementListGrid;
	CTabCtrl m_MeasureGraphTab;

private:
	RCP_DATA m_rcp_data;
	int m_iGraphDlgMode;
	BOOL m_bCellSelectedFlg;

public:

	// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMeasurementGraphDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMeasurementGraphDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeMeasureGraphTab(NMHDR* pNMHDR, LRESULT* pResult);
	//2009.09.03 bagus se オートスケール --{--
	afx_msg void OnGraphAutoScale();
	//2009.09.03 bagus se オートスケール --}--
	//}}AFX_MSG
	//afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	afx_msg void OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	afx_msg void OnDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg LRESULT SetMeasurementData(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetMeasurementData_TestMode(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetRefData_TestMode(WPARAM wparam, LPARAM lparam);


	DECLARE_MESSAGE_MAP()

	//2009.09.13 bagus stress --{--
	int	 m_nGraphRow;
	int	 m_nGraphTab;
	//2009.09.13 bagus stress --}--
public:
	void MeasurementListGrid_UnSelectAll();
/* added 2015.03.10 hmenjo テスト測定保存 ---------- { ---------- */
	void TestMeasGetGrid(CGridCtrl **pMeasListGrid, CGridCtrl **pIntListGrid) {
			if (0 != pMeasListGrid) {
				*pMeasListGrid = &m_MeasurementListGrid;
			}
			if (0 != pIntListGrid) {
				*pIntListGrid = &m_LightIntensityListGrid;
			}
		};
	int TestMeasGetGraphRow() {
			return m_nGraphRow;
		};
	void TestMeasChangeSelect(int iPoint) {
			NM_GRIDVIEW l_gvDmy;
			l_gvDmy.iColumn = 0;
			l_gvDmy.iRow = iPoint;
			this->OnSelChangeing((NMHDR*) &l_gvDmy, 0);
		};
/* added 2015.03.10 hmenjo テスト測定保存 ---------- } ---------- */
/* added 2015.03.19 hmenjo FWXGA 対応 ---------- { ---------- */
	void ChgSizeCtrl(long ldY);
/* added 2015.03.19 hmenjo FWXGA 対応 ---------- } ---------- */
private:
	void FontCreation();
	void MeasurementListGrid_Init();
	void MeasurementListGrid_HeaderSet();
	void LoadData();
	void SetData();
	void AddMeasurementListGrid_Data(int iRow, int iCol, LPCTSTR pszValue);
	void AddLightIntensityListAndGraph(int iPointIndex);
	void AddLightIntensityListAndGraph_Ref(int iPointIndex);
	//2009.09.15 bagus se --{--
	void AddPsiAndDeltaListAndGraph(int iPointIndex);
	//2009.09.15 bagus se --}--
	//2009.08.25 bagus stess --{--
	void AddStressDataListAndGraph(int nLine,int nSection,int nType);
	//2009.08.25 bagus stess --}--
	void LightIntensityListGrid_Init();
	void DrawLightIntensityGraph_Init();
	//2009.08.31 bagus stress --{--
	void DrawLightIntensityGraph_Init_SR();
	//void DrawLightIntensityGraph_Init_Stress();
	void DrawLightIntensityGraph_Init_Stress(int nLineNo,int nSectionNo);
	//2009.08.31 bagus stress --}--
	//2009.09.15 bagus se --{--
	void DrawLightIntensityGraph_Init_SE();
	//2009.09.15 bagus se --}--
	int SetMeasurementData2();
	//2009.08.22 bagus stress --{--
	int SetMeasurementLSData(int nStartRow);
	int SetMeasurementLSData_One(int nStressLine,int nRow,STRESSRESULT *pOneData);
	//2009.08.22 bagus stress --}--
	//2009.08.12 bagus stress --{--
	void Tab_Init();
	void TabChange(int Index);
	int GetCurrentTab();
	//2009.08.12 bagus stress --}--
	//2009.09.19 bagus stress --{--
	void SetBlankLightIntensityListGrid();
	//2009.09.19 bagus stress --}--


private:
	SR_CONFIG m_SrConfig;
	STRESS_CONFIG m_StressConfig;
	MAIN_RCP_INFO m_MainRcpInfo;
	MEAS_PROG_INFO m_MeasProgInfo;
	MEASUREMENT_DATA m_MeasurementData;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;
	STAGE_COORD m_ScanPoint[SCAN_POINT_MAX];
	STAGE_COORD_XYZ m_CurrPos[SCAN_POINT_MAX];
	CFont m_Font;
	CGridCtrl m_LightIntensityListGrid;
	char m_szName[RECIPE_NAME_LEN + 1];
	CBrush* m_oldBrush;
	CPen* m_oldPen;
	int m_iStepX;
	double m_dStepY;
	int m_iStartWaveLength;
	int m_iEndWaveLength;
	int m_iMinResult;
	int m_iMaxResult;

	int m_iModeStatus;
	int m_iDispPointNo;
	int m_nDispLine;
	int m_nDispSection;
	double* m_dAnalysisData;						// 反射率データ(index0: 380nm)
	double* m_dAnalysisGenData;						// XmpからのGenData(index0: 380nm)
	SE_RAW_ONE_DATA* m_seRawData;
	int m_nDataIndex;
	class CLightIntensityListGridHeader : public CStringArray
	{
	public:
		//2010.01.11 bagus GTR --{--
		//void GetHeader(WORD wHeadType, int iGraphDlgMode, int iTab);
		void GetHeader(WORD wHeadType, int iGraphDlgMode, int iTab,WORD wScanType);
		//2010.01.11 bagus GTR --}--
	};
	CLightIntensityListGridHeader m_strHeader;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEASUREMENTGRAPHDLG_H__8A7DCBC9_D781_47AA_AC53_C61DA9402589__INCLUDED_)
