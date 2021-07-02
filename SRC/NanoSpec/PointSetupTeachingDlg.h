#if !defined(AFX_POINTSETUPTEACHINGDLG_H__E5226ECE_066A_4EA9_9DBE_096798063CF9__INCLUDED_)
#define AFX_POINTSETUPTEACHINGDLG_H__E5226ECE_066A_4EA9_9DBE_096798063CF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupTeachingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingDlg ダイアログ

class CPointSetupTeachingDlg : public COverlayDialog
{
// コンストラクション
public:
	CPointSetupTeachingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// 2009.09.14 bagus stress Stress対応 --{--
	WORD	m_wHeadType;
	long	m_lYPoint;
	int		m_nSelectedPoint;
// 2009.09.14 bagus stress Stress対応 --}--

// ダイアログ データ
	//{{AFX_DATA(CPointSetupTeachingDlg)
	enum { IDD = IDD_POINT_SETUP_TEACHING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CBrush m_Brush_PointNo;
	CBrush m_Brush_Position_X;
	CBrush m_Brush_Position_Y;
	CShadeButtonST m_NextPointButton;
	CShadeButtonST m_PreviousPointButton;
	CShadeButtonST m_ApplyButton;
	CShadeButtonST m_MoveButton;

	long m_lX;
	long m_lY;
	long m_lZ;
	int m_iNumScans;							// Teachingされているポイント数
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;		// STAGE_PROG_INFO_HDR構造体
	STAGE_COORD m_ScanPoint[SCAN_POINT_MAX];	// Teachingポイント

	int m_iProcessStatusOld;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPointSetupTeachingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointSetupTeachingDlg)
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
// 2009.09.14 bagus stress Stress対応 --{--
	BOOL CheckInputCurrPoint();
// 2009.09.14 bagus stress Stress対応 --}--
	void SetPointData();
	void SoftJoyStickXyzEnable(BOOL bEnable);

	int m_iCurrPoint;						// 現在設定しようとしているポイント

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTSETUPTEACHINGDLG_H__E5226ECE_066A_4EA9_9DBE_096798063CF9__INCLUDED_)
