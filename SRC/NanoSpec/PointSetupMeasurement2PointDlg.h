#if !defined(AFX_POINTSETUPMEASUREMENT2POINTDLG_H__2A356BEC_3875_44D6_910D_33BE589CA4F8__INCLUDED_)
#define AFX_POINTSETUPMEASUREMENT2POINTDLG_H__2A356BEC_3875_44D6_910D_33BE589CA4F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupMeasurement2PointDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPointSetupMeasurement2PointDlg ダイアログ

class CPointSetupMeasurement2PointDlg : public COverlayDialog
{
// コンストラクション
public:
	CPointSetupMeasurement2PointDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ

	int	m_iTeachingMode;			// 自動/手動モード

	long	m_l1stX, m_l1stY;		// 設定した1st
	long	m_l2ndX, m_l2ndY;		// 設定した2nd

	BOOL	m_b2nd;					// 2nd設定フラグ

	BOOL	m_bComp;				// Compボタン押下フラグ

public:
	typedef enum _TEACHING_MODE
	{
		MODE_AUTO_MEAS = 0,
		MODE_MANUAL_MEAS,
		MODE_MAX,
	} TEACHING_MODE;


// ダイアログ データ
	//{{AFX_DATA(CPointSetupMeasurement2PointDlg)
	enum { IDD = IDD_POINT_SETUP_MEASUREMENT_2POINT_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CBrush m_Brush_Point_No;
	CBrush m_Brush_Point1st;
	CBrush m_Brush_Point2nd;
	CBrush m_Brush_Position_X1;
	CBrush m_Brush_Position_Y1;
	CBrush m_Brush_Position_X2;
	CBrush m_Brush_Position_Y2;
// 2010.01.06 bagus Distance --{--
	CBrush m_Brush_Position_X3;
	CBrush m_Brush_Position_Y3;
// 2010.01.06 bagus Distance --}--
	CBrush m_Brush_Distance;

	CShadeButtonST m_Meas1stButton;
	CShadeButtonST m_Meas2ndButton;
	CShadeButtonST m_MeasCompButton;

	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;			// STAGE_PROG_INFO_HDR構造体
	int	m_iCurrPoint;								// 現在設定しようとしているポイント
	STAGE_COORD m_ScanPoint[SCAN_POINT_MAX * 2];	// Teachingポイント

	int m_iProcessStatusOld;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPointSetupMeasurement2PointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointSetupMeasurement2PointDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHomePositionButton();
	afx_msg void OnMeas1stButton();
	afx_msg void OnMeas2ndButton();
	afx_msg void OnMeasCompButton();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	virtual void Refresh();

private:
	void SetPointData();
	void SoftJoyStickXyzEnable(BOOL bEnable);
	//2009.11.29 bagus 2point-distance --{--
	void AdjustCrossPointPos();
	//2009.11.29 bagus 2point-distance --}--

	BOOL	m_bDrawCross;			// CrossMarkを付けたフラグ
	int		m_iCrossX, m_iCrossY;

	void	CalcPointData(long lX, long lY);

	BOOL	m_bDraw2nd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTSETUPMEASUREMENT2POINTDLG_H__2A356BEC_3875_44D6_910D_33BE589CA4F8__INCLUDED_)
