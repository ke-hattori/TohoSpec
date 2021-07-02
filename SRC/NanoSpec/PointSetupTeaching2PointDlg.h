#if !defined(AFX_POINTSETUPTEACHING2POINTDLG_H__8C1C5805_7BA7_4074_A118_8042E47BF972__INCLUDED_)
#define AFX_POINTSETUPTEACHING2POINTDLG_H__8C1C5805_7BA7_4074_A118_8042E47BF972__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupTeaching2PointDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeaching2PointDlg ダイアログ

class CPointSetupTeaching2PointDlg : public COverlayDialog
{
// コンストラクション
public:
	CPointSetupTeaching2PointDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// 2009.10.24 bagus 2点間 修正 --{--
//	BOOL	m_bTeachingMode;
// 2009.10.24 bagus 2点間 修正 --}--


// ダイアログ データ
	//{{AFX_DATA(CPointSetupTeaching2PointDlg)
	enum { IDD = IDD_POINT_SETUP_TEACHING_2POINT_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CBrush m_Brush_Point1st;
	CBrush m_Brush_Point2nd;
	CBrush m_Brush_Position_X1;
	CBrush m_Brush_Position_Y1;
	CBrush m_Brush_Position_X2;
	CBrush m_Brush_Position_Y2;
	CBrush m_Brush_Distance;
	CShadeButtonST m_NextPointButton;
	CShadeButtonST m_PreviousPointButton;
	CShadeButtonST m_ApplyButton;
	CShadeButtonST m_MoveButton;

	long m_lX;
	long m_lY;
	long m_lZ;
	int m_iNumScans;								// Teachingされているポイント数
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;			// STAGE_PROG_INFO_HDR構造体
	STAGE_COORD m_ScanPoint[SCAN_POINT_MAX * 2];	// Teachingポイント

	int m_iProcessStatusOld;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPointSetupTeaching2PointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointSetupTeaching2PointDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNextPointButton();
	afx_msg void OnPreviousPointButton();
	afx_msg void OnApplyButton();
	afx_msg void OnHomePositionButton();
	//}}AFX_MSG
	afx_msg void OnMoveButton();

	DECLARE_MESSAGE_MAP()

	virtual void ButtonEnableCheck(void);
private:
	BOOL CheckInputCurrPoint();
	void SetPointData();
	void SoftJoyStickXyzEnable(BOOL bEnable);

	int	m_iCurrPoint;						// 現在設定しようとしているポイント
	int	m_iCurrIndex;
// 2009.10.24 bagus 2点間 修正 --{--
	int	m_iMaxIndex;

	int		m_iPosX;
	int		m_iPosY;
// 2009.10.24 bagus 2点間 修正 --}--

// 2009.10.28 bagus 2点間 修正 --{--
	BOOL	m_b2nd;
// 2009.10.28 bagus 2点間 修正 --}--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTSETUPTEACHING2POINTDLG_H__8C1C5805_7BA7_4074_A118_8042E47BF972__INCLUDED_)
