#if !defined(AFX_NANOPOINTGRAPH_H__D2641646_8B5F_4981_BDF7_B40B8E2F85A8__INCLUDED_)
#define AFX_NANOPOINTGRAPH_H__D2641646_8B5F_4981_BDF7_B40B8E2F85A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NanoPointGraph.h : ヘッダー ファイル
//
#include <afxwin.h> 		// MFC のコアおよび標準コンポーネント
#include <afxext.h> 		// MFC の拡張部分
#include <afxdisp.h>		// MFC のオートメーション クラス

#include "Globals.hxx"

#define NANO_POINT_MAP_DATA_MAX		(10000)
#define NANO_POINT_MAP_EDGE_WIDTH	(0)	//(2)

//2009.09.10 bagus stress --{--
//#define	NANO_POINT_MAP_LINE_WIDTH	(1)				// Line(Section)の線幅
#define	NANO_POINT_MAP_LINE_WIDTH	(3)				// Line(Section)の線幅
//2009.09.10 bagus stress --}--

// 2009.10.15 bagus Distance 追加 --{--
#define	NANO_POINT_MAP_DISTANCE_LINE_WIDTH	(2)		// Line(Distance)の線幅
// 2009.10.15 bagus Distance 追加 --}--

#define NANO_POINT_MAP_CROESS_MARK_LENGTH	(5)
#define NANO_POINT_MAP_CROESS_MARK_WIDTH	(3)

#define MAX_POINT_MAP_SECTION_MAX		(STRESS_SECTIONS_MAX)
#define MAX_POINT_MAP_LINE_MAX			(STRESS_LINES_MAX)
// 2009.09.11 bagus stress 修正 --{--
//#define MAX_POINT_MAP_PIN_MAX			(STRESS_PINS_MAX)
#define MAX_POINT_MAP_PIN_MAX			(STRESS_PINS_MAX * STRESS_LINES_MAX)
// 2009.09.11 bagus stress 修正 --}--

/////////////////////////////////////////////////////////////////////////////
// CNanoPointGraph ウィンドウ

class CNanoPointGraph : public CButton
{
// コンストラクション
public:
	CNanoPointGraph();

// アトリビュート
public:
	typedef enum {
		NOT_MEASURED = 0,
		MEASURING = 1,
		MEASURED = 2,
		SELECTED = 3,
		STATUS_MAX = 4
	} PointStatus;
	typedef enum {
		SECTION_NOT_MEASURED = 0,
		SECTION_MEASURING = 1,
		SECTION_MEASURED = 2,
		SECTION_SELECTED = 3,
		SECTION_NOT_USED = 4,
		SECTION_STATUS_MAX = 5
	} SectionStatus;
	typedef enum {
		EDGE_UPPER = 0,							//影の色 (Top & Left)
		EDGE_LOWER = 1,							//影の色 (Bottom & Right)
		EDGE_MAX = 2,							//
	} EdgeStatus;
	typedef struct {
		//Sampleパネルサイズ[um]
		double	SampleSizeX;					//パネルの横幅 (グラフ最大値)
		double	SampleSizeY;					//パネルの縦幅 (グラフ最大値)
		//
		COLORREF	StatusColor[STATUS_MAX];				//ステータスの色
		COLORREF	SectionStatusColor[SECTION_STATUS_MAX];	//ステータスの色(Line/Section)
		COLORREF	EdgeColor[EDGE_MAX];					//グラフの縁取り色
		COLORREF	BackColor;								//背景色
		COLORREF	PinColor;								//ピンの色
		// 2009.10.14 bagus Distance 追加 --{--
		COLORREF	DistanceStartColor;						//2点間の始点のの色
		COLORREF	DistanceEndColor;						//2点間の終点の色
		COLORREF	DistanceLineColor;						//2点間の線の色
		// 2009.10.14 bagus Distance 追加 --}--
		// 2013.01.09 bagus -->
		COLORREF	OriginColor;							//原点の色
		// 2013.01.09 bagus <--
	} GraphConfig;
	typedef struct {
		long DataX;								//実データ
		long DataY;								//実データ
		PointStatus	Status;						//座標ステータス
		BOOL NeedRedraw;
	} PointData;

	//2009.09.10 bagus stress --{--
	typedef struct {
		long DataX;								//実データ
		long DataY;								//実データ
		BOOL NeedRedraw;
	} PinData;
	//2009.09.10 bagus stress --}--

	typedef struct {
		long StartX;							//実データ
		long StartY;							//実データ
		long EndX;								//実データ
		long EndY;								//実データ
		SectionStatus	Status;					//座標ステータス
		//2009.08.30 -->
		int SourceLineNo;						//元ラインNo
		int SourceSectionNo;					//元セクションNo
		//2009.08.30 <--
		BOOL NeedRedraw;
	} LSData;

	// 2009.10.14 bagus Distance 追加 --{--
	typedef struct {
		long DataX;								//実データ
		long DataY;								//実データ
	} DistanceData;
	// 2009.10.14 bagus Distance 追加 --}--
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNanoPointGraph)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void Select(int nPoint);
	void Finish(int nPoint);
	void Measure(int nPoint);
	void AddPoint(long x,long y,PointStatus status = NOT_MEASURED);
	void AddPoint(PointData *pData);
	void AddPointArray(int nPoint,PointData *pData);
	void SetConfig(GraphConfig *pConfig);
	void GetConfig(GraphConfig *pConfig);
	void Reset();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual ~CNanoPointGraph();

	//@@@ bagus stress 測定ポイントグラフ対応 --{--
	void SetLineStatusMode(BOOL bMode);
	BOOL GetLineStatusMode();
	void AddLineData(LSData *pData);
	void SelectSection(int nNo);
	void FinishSection(int nNo);
	void MeasureSection(int nNo);
	void ResetSection();
	BOOL IsValidLine(int nNo);
	void ChangeSectionStatus(int nNo, SectionStatus status);
	//
	void SelectSectionByLSNo(int nLineNo,int nSectionNo = 0 );
	void FinishSectionByLSNo(int nLineNo,int nSectionNo = 0);
	void MeasureSectionByLSNo(int nLineNo,int nSectionNo = 0);
	BOOL IsValidLSNo(int nLineNo,int nSectionNo = 0);
	//@@@ bagus stress 測定ポイントグラフ対応 --}--

	// 2009.09.10 bagus stress --{--
	// ストレスピン表示対応
	void ResetPin();
	void AddPinData(long x,long y);
	// 2009.09.10 bagus stress --}--

	// 2009.10.14 bagus Distance 追加 --{--
	void SetDistanceMode(BOOL bMode);
	BOOL GetDistanceMode();
	void SelectDistance(int nNo);
	// 2009.10.14 bagus Distance 追加 --}--

// 2009.09.11 bagus stress --{--
	void ResetAll();
// 2009.09.11 bagus stress --}--

	// 2013.01.09 bagus -->
	void SetOriginPointData(long x,long y);
	// 2013.01.09 bagus <--

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CNanoPointGraph)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	//2009.09.10 bagus stress --{--
	void DrawLines(CDC *pDC,double dblRateX,double dblRateY);
	void DrawPins(CDC *pDC,double dblRateX,double dblRateY);
	//2009.09.10 bagus stress --}--

	void InitBackBuffer();
	BOOL IsValidPoint(int nPoint);
	void ChangeStatus(int nPoint,PointStatus status);
	void ClearGraph(CDC *pDC,RECT *pScreenSize);
	// 2013.01.09 bagus -->
	void DrawOriginPoint(CDC *pDC,double dblRateX,double dblRateY);
	// 2013.01.09 bagus <--
	void DrawMeasuredPoints(CDC *pDC,double dblRateX,double dblRateY);
	void DrawNotMeasuredPoints(CDC *pDC,double dblRateX,double dblRateY);
	void DrawMeasuringPoints(CDC *pDC,double dblRateX,double dblRateY);
	void DrawSelectedPoint(CDC *pDC,double dblRateX,double dblRateY);
	// 2009.10.14 bagus Distance 追加 --{--
	void Draw2PointDistance(CDC *pDC,double dblRateX,double dblRateY);
	// 2009.10.14 bagus Distance 追加 --}--
	void DrawEdge(CDC *pDC,RECT *pScreenSize);

	void DrawPointDot(CDC *pDC,int nX,int nY);
	void DrawPointCross(CDC *pDC,int nX,int nY);

	void DrawItemMain(LPDRAWITEMSTRUCT lpDIS);
	void ReDraw();

	GraphConfig	m_Config;									//グラフ領域データ
	PointData	m_PointData[NANO_POINT_MAP_DATA_MAX];		//座標データ
	int			m_PointNum;									//有効ポイント数 ( = 実際の配列最大値)
	int			m_SelectedPoint;							//現在選択されている座標データの配列番号 (-1は選択無し)
	//BackBuffer
	CDC					*m_bbDC;
	CBitmap				*m_bbBitmap;
	BOOL				m_bbOK;

	//@@@ bagus stress 測定ポイントグラフ対応 --{--
	BOOL				m_bLSStatusMode;
	LSData				m_LSData[MAX_POINT_MAP_LINE_MAX * MAX_POINT_MAP_SECTION_MAX];		//座標データ
	int					m_LSNum;									//有効ポイント数 ( = 実際の配列最大値)
	int					m_SelectedLS;							//現在選択されている座標データの配列番号 (-1は選択無し)
	//@@@ bagus stress 測定ポイントグラフ対応 --}--
	//2009.09.10 bagus stress --{--
	PinData				m_PinData[MAX_POINT_MAP_PIN_MAX];
	int					m_PinNum;
	//2009.09.10 bagus stress --}--

	// 2009.10.14 bagus Distance 追加 --{--
	BOOL				m_bDistanceMode;
	int					m_SelectedDistance;
	// 2009.10.14 bagus Distance 追加 --}--

	//2013.01.09 bagus -->
	PointData			m_OriginPointData;					//原点座標データ
	//2013.01.09 bagus <--
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NANOPOINTGRAPH_H__D2641646_8B5F_4981_BDF7_B40B8E2F85A8__INCLUDED_)
