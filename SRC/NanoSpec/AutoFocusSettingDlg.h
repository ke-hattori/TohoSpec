#if !defined(AFX_AUTOFOCUSSETTINGDLG_H__8FD26CF4_973A_4783_B7A7_63BAE57EB35C__INCLUDED_)
#define AFX_AUTOFOCUSSETTINGDLG_H__8FD26CF4_973A_4783_B7A7_63BAE57EB35C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoFocusSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAutoFocusSettingDlg ダイアログ

class CAutoFocusSettingDlg : public COverlayDialog
{
// コンストラクション
public:
	CAutoFocusSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CAutoFocusSettingDlg)
	enum { IDD = IDD_OVERLAY_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	long m_lX;
	long m_lY;
	long m_lZ;

	CBrush m_Brush_Position_X;
	CBrush m_Brush_Position_Y;
//2009.09.24 bagus stress 機能追加 --{--
	CBrush m_Brush_Position_Z;
	CShadeButtonST m_SettingPointButton;

	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;		// STAGE_PROG_INFO_HDR構造体
//2009.09.24 bagus stress 機能追加 --}--

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAutoFocusSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAutoFocusSettingDlg)
	afx_msg BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnSettingPointButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
//2009.09.24 bagus stress 機能追加 --{--
	void SetPointData();

	WORD m_wHeadType;															// ヘッドタイプ
//2009.09.24 bagus stress 機能追加 --}--

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_AUTOFOCUSSETTINGDLG_H__8FD26CF4_973A_4783_B7A7_63BAE57EB35C__INCLUDED_)
