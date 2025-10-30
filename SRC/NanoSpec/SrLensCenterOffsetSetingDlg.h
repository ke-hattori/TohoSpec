#if !defined(AFX_SRLENSCENTEROFFSETSETINGDLG_H__4244EC21_22B3_45FA_B31B_13668CE0CF9B__INCLUDED_)
#define AFX_SRLENSCENTEROFFSETSETINGDLG_H__4244EC21_22B3_45FA_B31B_13668CE0CF9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrLensCenterOffsetSetingDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"

// ##########################################################################
// CSrLensCenterOffsetSetingDlg ダイアログ
// ##########################################################################

class CSrLensCenterOffsetSetingDlg : public CDialog
{
// コンストラクション
public:
	CSrLensCenterOffsetSetingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrLensCenterOffsetSetingDlg)
	enum { IDD = IDD_SR_LENS_CENTER_OFFSET_SETTING_DLG };
	//}}AFX_DATA


//opration
public:
	CColorStatic m_basePos_X, m_basePos_Y, m_basePos_Z;
	CColorStatic m_stcLens1_X, m_stcLens1_Y, m_stcLens1_Z;
	CColorStatic m_stcLens2_X, m_stcLens2_Y, m_stcLens2_Z;
	CColorStatic m_stcLens3_X, m_stcLens3_Y, m_stcLens3_Z;
	CColorStatic m_stcLens4_X, m_stcLens4_Y, m_stcLens4_Z;
	CShadeButtonST m_Lens1SetupButton;
	CShadeButtonST m_Lens2SetupButton;
	CShadeButtonST m_Lens3SetupButton;
	CShadeButtonST m_Lens4SetupButton;
	CShadeButtonST m_AllClearButton;
	CShadeButtonST m_CheckButton;
	CShadeButtonST m_ApplyButton;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	SR_LENS_CENTER_OFFSET m_LensOffset;
	SR_LENS_CENTER_OFFSET m_LensOffsetBackUp;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	int m_iBaseLens;

	void EnableButtons(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrLensCenterOffsetSetingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrLensCenterOffsetSetingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAllClear();
	afx_msg void OnCheck();
	afx_msg void OnApply();
	afx_msg void OnSelchangeBaseLens();
	afx_msg void OnLens1SetupButton();
	afx_msg void OnLens2SetupButton();
	afx_msg void OnLens3SetupButton();
	afx_msg void OnLens4SetupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetOperationLog(LPCTSTR pszLog);

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRLENSCENTEROFFSETSETINGDLG_H__4244EC21_22B3_45FA_B31B_13668CE0CF9B__INCLUDED_)
