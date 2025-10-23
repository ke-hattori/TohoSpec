#if !defined(AFX_NANOMEASUREGRAPH_H__75636F0B_F7E0_4CF3_A378_FA6AC8D9C5C8__INCLUDED_)
#define AFX_NANOMEASUREGRAPH_H__75636F0B_F7E0_4CF3_A378_FA6AC8D9C5C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NanoMeasureGraph.h : ヘッダー ファイル
//
#include <afxwin.h> 		// MFC のコアおよび標準コンポーネント
#include <afxext.h> 		// MFC の拡張部分
#include <afxdisp.h>		// MFC のオートメーション クラス
#include <afxtempl.h>

//debug
#include <math.h>

#include "NanoLineSelect.h"

#define NANO_MEASURE_MAX_GRAPH_LINE	(10)
#define NANO_MAX_TITLE_LEN			(100)
#define NANO_MAX_LINENAME_LEN		(50)
//2009.09.10 bagus stress --{--
//#define NANO_MEASURE_MAX_POINT_DATA	(1024)//(5000)
//ストレスのデータの1ライン分は1000を超えてしまうので拡張する
#define NANO_MEASURE_MAX_POINT_DATA	(5000)
//2009.09.10 bagus stress --}--
#define NANO_CALIBRATION_LINE_MAX	(10)

/////////////////////////////////////////////////////////////////////////////
// CNanoMeasureGraph ウィンドウ

class CNanoMeasureGraph : public CButton
{
public:

	typedef struct {
		long X_Origin;		//X軸開始値
		long Y_Origin;		//Y軸開始置
		long X_Step;		//X軸データ間隔
		long Y_Step;		//Y軸データ間隔（点線のY方向表示間隔）
		long X_Max;			//使わない
		long Y_Max;			//Y軸最大値
		int  PointNum;		//測定点数
		char UpperTitle[NANO_MAX_TITLE_LEN+1];
		char LowerTitle[NANO_MAX_TITLE_LEN+1];
		char X_AxisTitle[NANO_MAX_TITLE_LEN+1];
		char Y_AxisTitle[NANO_MAX_TITLE_LEN+1];
		//画面上部の線の表示/非表示を切り替えるチェックボックスを使用するかどうか
		BOOL UseLineSelect;
		//キャリブレーション関係
		BOOL UseCalibration;		//キャリブレーション機能を使用するかどうか
		int  CalibrationLineNum;	//キャリブレーションの線数最大値
		//色設定
		COLORREF CanvasBackColor;	//描画エリア全体の背景色
		COLORREF GraphBackColor;	//グラフの部分の背景色
		COLORREF GraphBorderColor;	//グラフの枠線の色
		COLORREF GraphStepLineColor;//グラフ内の点線の色
		COLORREF TitleTextColor;	//タイトルの色
		COLORREF UnitTextColor;		//軸のテキスト色
//		COLORREF EdgeColor;
		COLORREF SelectingCalibrationColor;		//キャリブレーション色 確定前
		COLORREF SelectedCalibrationColor;		//キャリブレーション色 確定後
		//
		int		 CanvasMargin;					//余白(pixel)
	} GraphConfig;

	//各データの線の設定
	typedef struct {
		COLORREF Color;										//線の色
		int		PenStyle;									//線種
		BOOL	UseFlag;									//TRUE:使用する,FALSE:使用しない
		BOOL	Visible;									//内部用途（現在表示されているか否か）
		int		DataIndex;									//何番目のデータを使うか
		char	LineName[NANO_MAX_LINENAME_LEN+1];			//線の名前
	} LineConfig;

	//キャリブレーションデータ
	typedef struct {
		int		DataNum;									//確定したキャリブレーション数
		int		PointIndex[NANO_CALIBRATION_LINE_MAX];		//座標データのX個目 (X軸方向)
	} CalibrationData;


// コンストラクション
public:
	CNanoMeasureGraph();

// アトリビュート
public:
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNanoMeasureGraph)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CalibrationData GetCalibrationData();
	void MoveCalibrationLine(int nStep);
	virtual BOOL  PreTranslateMessage(MSG *pMsg);
	void Initialize();
	void AddLineData(int nLineNo,double dXValue,double dYValue);//2008-10-10OkabeAdd
	void AddLineData(int nLineNo,double value);
	void AddLineDataAll(int nLineNo,double *pValues,int nPointNum);
	BOOL SetGraphConfig(GraphConfig tConfig);
	BOOL GetGraphConfig(GraphConfig* pConfig);
	BOOL SetLineConfig(int nIndex, LineConfig tConfig);
	BOOL GetLineConfig(int nIndex, LineConfig *pConfig);
	void ClearAllData();
	void ClearGraphData();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual ~CNanoMeasureGraph();

	void ResetCalibrationData();

	//2009.09.03 bagus se オートスケール --{--
	void SetAutoScaleMode(BOOL bMode);
	BOOL GetAutoScaleMode();
	//2009.09.03 bagus se オートスケール --}--

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CNanoMeasureGraph)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	afx_msg LRESULT OnLineSelectChange(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsValidLineNo(int nLineNo);
	//内部用途
	typedef struct {
		RECT GraphArea;
		double RateX;
		double RateY;
		double DivX;
	} DrawGraphInfo;

	//2008-10-10OkabeAdd X、Y軸doublu値グラフ用途
	typedef struct {
		double dXValue;
		double dYValue;
	} GraphXYDataValue;

	void InitBackBuffer();
	void DrawItemMain(LPDRAWITEMSTRUCT lpDIS);
	SIZE GetTextSize(CDC *pDC,CFont *pFont,CString strText);
	int ArrangeLineSelect(CRect cArea);
	void DrawEdge(CDC *pDC,RECT *pArea);
	void DrawAxisTitleX(CDC *pDC, CString strTitle, RECT *pArea);
	void DrawAxisTitleY(CDC *pDC,CString strTitle,RECT *pArea);
	void DrawTitle(CDC *pDC,CString strTitle,RECT *pArea);
	void ClearGraph(CDC *pDC,RECT *pScreenSize);
	void AddCalibrationData(int nPointIndex);
	void ChangeCalibrationData(int nPointIndex);
	void DrawGraph(CDC *pDC,RECT *pArea);
	void CutGraphArea(CDC *pDC, RECT *pArea, RECT *pCutArea);
	//Drawing Colors
	LineConfig m_LineConfig[NANO_MEASURE_MAX_GRAPH_LINE];

	//Drawing Font
	CFont*	m_pTitleFont;
	CFont*	m_pAxisXFont;
	CFont*	m_pAxisYFont;
	CFont*	m_pMemFont;
	//Graph Config
	GraphConfig		m_Config;
	//CArray<MeasureValue_t,MeasureValue_t&> m_LineData[NANO_MEASURE_MAX_GRAPH_LINE];
	double m_LineData[NANO_MEASURE_MAX_GRAPH_LINE][NANO_MEASURE_MAX_POINT_DATA];		//実測値
	//2009.09.20 bagus stress --{--
//	GraphXYDataValue m_LineXYData[1][NANO_MEASURE_MAX_POINT_DATA];		//2008-10-10OkabeAdd
	GraphXYDataValue m_LineXYData[NANO_MEASURE_MAX_GRAPH_LINE][NANO_MEASURE_MAX_POINT_DATA];		//2008-10-10OkabeAdd
	//2009.09.20 bagus stress --}--
	int	m_LinePointNum[NANO_MEASURE_MAX_GRAPH_LINE];

	//2008-10-10OkabeAdd 使用グラフ切替フラグ
	//（0:m_LineDataグラフ	1:m_LineXYDataグラフ）
	int m_iGrapgSelectFlg;

	//
	CNanoLineSelect*	m_pLineSelect[NANO_MEASURE_MAX_GRAPH_LINE];
	CalibrationData		m_CalibrationData;
	BOOL				m_bNowCalibration;

	DrawGraphInfo		m_GraphInfo;

	//BackBuffer
	CDC					*m_bbDC;
	CBitmap				*m_bbBitmap;
	BOOL				m_bbOK;
	BOOL				m_bReDrawGraph;

	//2009.09.03 bagus se オートスケール --{--
	BOOL	m_bAutoScaleMode;
	double	m_dYMin;
	double	m_dYMax;
	//2009.09.03 bagus se オートスケール --}--
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NANOMEASUREGRAPH_H__75636F0B_F7E0_4CF3_A378_FA6AC8D9C5C8__INCLUDED_)
