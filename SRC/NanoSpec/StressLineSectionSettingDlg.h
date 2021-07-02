#if !defined(AFX_STRESSLINESECTIONSETTINGDLG_H__A4AA5361_A3A8_45E7_A5F4_B731887F7855__INCLUDED_)
#define AFX_STRESSLINESECTIONSETTINGDLG_H__A4AA5361_A3A8_45E7_A5F4_B731887F7855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StressLineSectionSettingDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"
#include "GridCtrl.h"
#include "NanoPointGraph.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用

/////////////////////////////////////////////////////////////////////////////
// CStressLineSectionSettingDlg ダイアログ

class CStressLineSectionSettingDlg : public CDialog
{
// コンストラクション
public:
	CStressLineSectionSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CStressLineSectionSettingDlg();

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_GridSetButton;
	CShadeButtonST m_SetLineSectionButton;
	// 2009.09.04 bagus stress ライン数設定ボタン追加 --{--
	CShadeButtonST m_LineSetButton;
	// 2009.09.04 bagus stress ライン数設定ボタン追加 --}--

	CString m_strSampleShape;
	CString m_strSampleSizeX;
	CString m_strSampleSizeY;
	CString m_strSampleOriginX;
	CString m_strSampleOriginY;

	CString m_strStressValidRange;
	CString m_strInvalidRangeEdge;
	CString m_strMeasPointOffsetX;
	CString m_strMeasPointOffsetY;

	CColorStatic m_stcSampleShape;
	CColorStatic m_stcSampleSizeX;
	CColorStatic m_stcSampleSizeY;
	CColorStatic m_stcSampleOriginX;
	CColorStatic m_stcSampleOriginY;

	CNanoPointGraph	m_SamplePointGraph;

	char m_szName[RECIPE_NAME_LEN + 1];

// ダイアログ データ
	//{{AFX_DATA(CStressLineSectionSettingDlg)
	enum { IDD = IDD_STRESS_LINE_SECTION_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
protected:
	CGridCtrl m_LineListGrid;
	CGridCtrl m_PinListGrid;
	CGridCtrl m_LineValidListGrid;
	CGridCtrl m_StressMeasurementLineSectionListGrid;
	CGridCtrl m_NecessaryThicknessMeasurementListGrid;

	void SamplePointGraph_Init();
	void SamplePointGraph_DataSet();

	void DrawSample(CDC* pDC, int Org, int DirX, int DirY);
// 2013.01.09 bagus -->
//	void DrawOriginMaker(int Origin);
// 2013.01.09 bagus <--
	void DrawAxisCoordinate(int iOrgPos, int DirX, int DirY);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStressLineSectionSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStressLineSectionSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnKillfocusNumberOfLine();
	afx_msg void OnGridsetButton();
	afx_msg void OnSetLineSectionButton();
	afx_msg void OnKillfocusInvalidRangeEdge();
	afx_msg void OnKillfocusThickMeasurementPointOffsetX();
	virtual void OnOK();
	afx_msg void OnLinesetButton();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	STAGE_CONFIG	m_StageConfig;
	STRESS_CONFIG	m_StressConfig;
	SAMPLEINFO		m_SampleInfo;

private:
// 2009.09.11 bagus stress チェック追加 --{--
	BOOL CheckStressMeasurementLineSectionData();
	BOOL CheckNecessaryThicknessMeasurementData();
// 2009.09.11 bagus stress チェック追加 --}--
// 2009.09.11 bagus stress 原点位置対応 --{--
	void CheckPinMaxPos(double *dX, double *dY);
// 2009.09.11 bagus stress 原点位置対応 --}--
// 2009.09.08 bagus stress LineNumのチェック --{--
	BOOL CheckLineNum();
// 2009.09.08 bagus stress LineNumのチェック --}--
// 2009.09.08 bagus stress 原点位置対応 --{--
	void CheckPinMinPos(double *dX, double *dY);
	BOOL CheckPinPos(long lX, long lY);
// 2009.09.08 bagus stress 原点位置対応 --}--
	BOOL CheckStressConfig_NumOfLineData();

	BOOL CheckStressConfig_LineData();
	BOOL CheckLineData();
	void SetLineData();

	BOOL CheckStressConfig_PinData();
	BOOL CheckPinData();
	void SetPinData();
	void SetDefaultPinData();

	// 2009.09.04 bagus stress 数値チェック追加 --{--
	BOOL CheckStressValidRangeData();
	BOOL CheckInvalidRangeEdgeData();
	BOOL CheckThinkMeasurementPointOffsetXData();
	// 2009.09.04 bagus stress 数値チェック追加 --}--

	BOOL CheckLineValidData();
	void SetLineValidData();

	void SetStressMeasurementLineSectionData();
	void SetDefaultStressMeasurementLineSectionData();

	void SetNecessaryThicknessMeasurementData();
	void SetDefaultNecessaryThicknessMeasurementData();

	BOOL IsValidLine(int nLine);

	CBrush* m_oldBrush;
	CPen* m_oldPen;

	double m_dDrawStartStagePosX;
	double m_dDrawEndStagePosX;
	double m_dDrawStartStagePosY;
	double m_dDrawEndStagePosY;

	void NecessaryThicknessMeasurementListGrid_Init();
	void StressMeasurementLineSectionListGrid_Init();
	void LineValidListGrid_Init();
	void LineListGrid_Init();
	void PinListGrid_Init();
	//2009.09.14 bagus stress --{--
	BOOL SetAllInputData();
	//2009.09.14 bagus stress --}--
	//2009.09.14 bagus stress --{--
	BOOL m_bOldLineValid[STRESS_LINES_MAX];
	//2009.09.14 bagus stress --}--

// 2009.09.15 bagus stress 修正 --{--
	BOOL CheckInputValueLong(CString strVal, long *lValue);
	BOOL CheckInputValueDouble(CString strVal, double *dValue);
// 2009.09.15 bagus stress 修正 --}--

// 2009.09.17 bagus stress Grid修正 --{--
	int	m_nSysVScrollWidth;
// 2009.09.17 bagus stress Grid修正 --}--

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRESSLINESECTIONSETTINGDLG_H__A4AA5361_A3A8_45E7_A5F4_B731887F7855__INCLUDED_)
