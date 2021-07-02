#if !defined(AFX_ADDITIONALMEASUREMENTDLG_H__EEF4D301_D1D8_4F5B_9B19_DADA4C15581E__INCLUDED_)
#define AFX_ADDITIONALMEASUREMENTDLG_H__EEF4D301_D1D8_4F5B_9B19_DADA4C15581E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdditionalMeasurementDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAdditionalMeasurementDlg ダイアログ

class CAdditionalMeasurementDlg : public COverlayDialog
{
// コンストラクション
public:
	CAdditionalMeasurementDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// 2009.10.30 bagus Stage-Sample 修正 --{--
	char m_szRecipeName[RECIPE_NAME_LEN + 1];
	int m_nSampleMode;
// 2009.10.30 bagus Stage-Sample 修正 --}--

// ダイアログ データ
	//{{AFX_DATA(CAdditionalMeasurementDlg)
	enum { IDD = IDD_OVERLAY_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_MeasurementButton;

	long m_lX;
	long m_lY;
	long m_lZ;

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
	int m_iProcessStatusOld;
	int m_iManuMeasModeOld;
	//2009.11.29 bagus GTR --{--
	BOOL m_bGTR;
	CComboBox m_cbMeasurePoint;
	CColorStatic m_stcMeasurementPointX;
	CColorStatic m_stcMeasurementPointY;
	CBrush m_Brush_PositionX;
	CBrush m_Brush_PositionY;
	//2009.11.29 bagus GTR --}--

	void ButtonEnableChange(int Select);
	//2009.11.29 bagus GTR --{--
	void SetPointData(SR_REFERENCE_POSITION SrReferencePosition, int iSelectMeasPt);
	//2009.11.29 bagus GTR --}--

protected:
	void SoftJoyStickXyzEnable(BOOL bEnable);
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	void ButtonEnableCheck(void);
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

public:


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAdditionalMeasurementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAdditionalMeasurementDlg)
	afx_msg BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMeasurementButton();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeComboMeasPoint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnManuModeOnePointEnd(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ADDITIONALMEASUREMENTDLG_H__EEF4D301_D1D8_4F5B_9B19_DADA4C15581E__INCLUDED_)
