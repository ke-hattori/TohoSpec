#if !defined(AFX_MEASUREMENTLISTDLG_H__726555BC_56F3_4468_B4C7_2E1ACC833AAC__INCLUDED_)
#define AFX_MEASUREMENTLISTDLG_H__726555BC_56F3_4468_B4C7_2E1ACC833AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasurementListDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"
#include "GridCtrl.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用

// ##########################################################################
// CMeasurementListDlg ダイアログ
// ##########################################################################

class CMeasurementListDlg : public CDialog
{
// コンストラクション
public:
	CMeasurementListDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	~CMeasurementListDlg();

// ダイアログ データ
public:
	//{{AFX_DATA(CMeasurementListDlg)
	enum { IDD = IDD_MEASUREMENT_LIST_DLG };
	CNanoPointGraph m_PointGraph;
	//}}AFX_DATA
	CGridCtrl m_MeasurementListGrid;
protected:
	CGridCtrl m_StatisticsListGrid;
	CColorStatic m_stcMainRecipeName;
	CColorStatic m_stcHeadType;
	CColorStatic m_stcMeasurementItem;
	CColorStatic m_stcMeasurementPoint;
	CColorStatic m_stcMeasurementLS;
	CRgn m_WaferRgn;
	CRect m_PictRect;

// Operation
public:
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
// 2014.01.17 Bagus Add (Stage None対応) -->
	SYSTEM_CONFIG m_SystemConfig;
// 2014.01.17 Bagus Add (Stage None対応) <--
private:
	RCP_DATA m_rcp_data;
	BOOL m_bCellSelectedFlg;

	STAGE_CONFIG	m_StageConfig;
public:

	char m_szName[RECIPE_NAME_LEN + 1];
	double m_dBuff[MEAS_LIST_CIE_MAX][SCAN_POINT_MAX];//最大のアイテム数を持つ測定の値を使用
	double m_dStandard;
	int m_iPointStates[SCAN_POINT_MAX];
	void AddMeasurementListGrid_Data(int iRow, int iCol, LPCTSTR psz);
	void AddStatisticsListGrid_Data(int iRow, int iCol, LPCTSTR psz);
	void LoadData();
	void SetData();
	void StartMeasurementInit(int Point);
	void MeasurementError(int Point);
	int SetMeasurementData2();
	//2009.08.22 bagus stress --{--
	int SetMeasurementLSData(int nStartRow);
	int SetMeasurementLSData_One(int nStressLine,int nRow,STRESSRESULT *pOneData);
	//2009.08.22 bagus stress --}--
	void MeasurementListGrid_UnSelectAll();
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	void ChgSizeCtrl(long ldY);
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */

// ------>テスト用関数
	double GetSampleData(int iPoint, int wave);
// <------テスト用関数

// 2013.01.17 bagus rotate xy view -->
	long RotateXyAxisDir(long lDir);
// 2013.01.17 bagus rotate xy view <--

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMeasurementListDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMeasurementListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	//afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	afx_msg void OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
//	afx_msg LRESULT SetMeasurementData(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

private:
	void MeasurementListGrid_InitGrid();
	void StatisticsListGrid_InitGrid();
	void MeasurementListGrid_HeaderSet();
	void StatisticsListGrid_HeaderSet();
	void PointGraph_DataSet();

	LOGFONT m_logFont;
	SR_CONFIG m_SrConfig;
	MAIN_RCP_INFO m_MainRcpInfo;
	MEAS_PROG_INFO m_MeasProgInfo;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;
	MEASUREMENT_DATA m_MeasurementData;
	STAGE_COORD m_ScanPoint[SCAN_POINT_MAX];
	STAGE_COORD_XYZ m_CurrPos[SCAN_POINT_MAX];
	long m_RectLeft;
	long m_RectRight;
	long m_RectTop;
	long m_RectBottom;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEASUREMENTLISTDLG_H__726555BC_56F3_4468_B4C7_2E1ACC833AAC__INCLUDED_)
