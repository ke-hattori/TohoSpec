#if !defined(AFX_SCANDATAGRAPHDLG_H__8EBE37D4_8D32_4DF8_92DE_16B07A8ECF3C__INCLUDED_)
#define AFX_SCANDATAGRAPHDLG_H__8EBE37D4_8D32_4DF8_92DE_16B07A8ECF3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanDataGraphDlg.h : ヘッダー ファイル
//

#include "SharedMemory.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用

/////////////////////////////////////////////////////////////////////////////
// CScanDataGraphDlg ダイアログ

class CScanDataGraphDlg : public CDialog
{
// コンストラクション
public:
	CScanDataGraphDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CScanDataGraphDlg();

// ダイアログ データ
	//{{AFX_DATA(CScanDataGraphDlg)
	enum { IDD = IDD_SCAN_DATA_GRAPH_DLG };
	CNanoMeasureGraph*	m_pScanDataGraph;
	//}}AFX_DATA

public:
	CGridCtrl m_ScanDataGrid;

	int m_iStartWaveLength;
	int m_iEndWaveLength;
	int m_nOpticalFilter;
	//2010.01.13 bagus GTR --{--
	//int m_nIntegrationTime;
	double m_dIntegrationTime;
	//2010.01.13 bagus GTR --}--
	int m_bTransmittanceShutterOpen;

	void ScanDataGraph_Init();
	void ScanDataGrid_Init();
	void AddLightIntensityListAndGraph();

// 2010.01.21 bagus Gantry --{--
	BOOL ScanData();

	void InitDialogData();
// 2010.01.21 bagus Gantry --}--

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CScanDataGraphDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CScanDataGraphDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg void OnGraphAutoScale();
	DECLARE_MESSAGE_MAP()

private:
// 2010.01.21 bagus Gantry --{--
//	BOOL ScanData();
// 2010.01.21 bagus Gantry --}--

// 2010.01.08 bagus Gantry --{--
	int	m_nSysVScrollWidth;
// 2010.01.08 bagus Gantry --}--

	SR_CONFIG m_srConfig;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SCANDATAGRAPHDLG_H__8EBE37D4_8D32_4DF8_92DE_16B07A8ECF3C__INCLUDED_)
