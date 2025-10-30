#if !defined(AFX_POINTCONFIRMATIONDLG_H__F86BF613_3841_411D_BCF0_831E44C7520B__INCLUDED_)
#define AFX_POINTCONFIRMATIONDLG_H__F86BF613_3841_411D_BCF0_831E44C7520B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointConfirmationDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPointConfirmationDlg ダイアログ

class CPointConfirmationDlg : public COverlayDialog
{
// コンストラクション
public:
	CPointConfirmationDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	WORD	m_wHeadType;
	long	m_lYPoint;
	int		m_nSelectedPoint;

// ダイアログ データ
	//{{AFX_DATA(CPointConfirmationDlg)
	enum { IDD = IDD_POINT_CONFIRMATION_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CBrush m_Brush_PointNo;
	CBrush m_Brush_Position_X;
	CBrush m_Brush_Position_Y;
	CShadeButtonST m_NextPointButton;
	CShadeButtonST m_PreviousPointButton;
	CShadeButtonST m_MoveButton;

	long m_lX;
	long m_lY;
	long m_lZ;
	int m_iNumScans;							// Teachingされているポイント数
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;		// STAGE_PROG_INFO_HDR構造体
	STAGE_COORD m_ScanPoint[SCAN_POINT_MAX];	// Teachingポイント

	BOOL			m_ValidPoint[SCAN_POINT_MAX];

	int m_iProcessStatusOld;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPointConfirmationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointConfirmationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNextPointButton();
	afx_msg void OnPreviousPointButton();
	afx_msg void OnCheckValidPoint();
	//}}AFX_MSG
	afx_msg void OnMoveButton();

	DECLARE_MESSAGE_MAP()

private:
	BOOL CheckInputCurrPoint();
	void SetPointData();
	void SoftJoyStickXyzEnable(BOOL bEnable);

	int m_iCurrPoint;						// 現在設定しようとしているポイント

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTCONFIRMATIONDLG_H__F86BF613_3841_411D_BCF0_831E44C7520B__INCLUDED_)
