#if !defined(AFX_MEASUREMENTGRAPHDLG2_H__0C87C494_7CFF_4AE6_B7E3_6F5596AC23A3__INCLUDED_)
#define AFX_MEASUREMENTGRAPHDLG2_H__0C87C494_7CFF_4AE6_B7E3_6F5596AC23A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasurementGraphDlg2.h : ヘッダー ファイル
//
#include "ColorStatic.h"
#include "GridCtrl.h"
#include "SharedMemory.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用

/////////////////////////////////////////////////////////////////////////////
// CMeasurementGraphDlg2 ダイアログ

class CMeasurementGraphDlg2 : public CMeasurementGraphDlg
{
// コンストラクション
public:
	CMeasurementGraphDlg2(int m_mode, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMeasurementGraphDlg2)
	enum { IDD = IDD_MEASUREMENT_GRAPH_DLG2 };
	CNanoMeasureGraph	m_MeasureGraph;
	//}}AFX_DATA

//Operation
public:

private:
	RCP_DATA m_rcp_data;
//	CMainFrame* m_pMainFrame;
//	CNanoSpecDoc* m_pDoc;



// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMeasurementGraphDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL



// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMeasurementGraphDlg2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEASUREMENTGRAPHDLG2_H__0C87C494_7CFF_4AE6_B7E3_6F5596AC23A3__INCLUDED_)
