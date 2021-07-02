#if !defined(AFX_MANUREMEASRECALCSTATDLG_H__A6DE4A76_3048_45D7_A0D0_741314E642FA__INCLUDED_)
#define AFX_MANUREMEASRECALCSTATDLG_H__A6DE4A76_3048_45D7_A0D0_741314E642FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManuReMeasReCalcStatDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CManuReMeasReCalcStatDlg ダイアログ

class CManuReMeasReCalcStatDlg : public CDialog
{
// コンストラクション
public:
	BOOL ParamGet(LPARAM* plParam = 0);
	BOOL ParamSet(LPARAM lParam = 0);
	CManuReMeasReCalcStatDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CManuReMeasReCalcStatDlg)
	enum { IDD = IDD_MANUREMEASRECALCSTAT_DLG };
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CManuReMeasReCalcStatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CManuReMeasReCalcStatDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnBtnMRCS_Run();
	afx_msg void OnBtnMRCS_Cancel();
	afx_msg void OnRadioMRCS_1();
	afx_msg void OnRadioMRCS_2();
	afx_msg void OnRadioMRCS_3();
	afx_msg void OnRadioMRCS_4();
	afx_msg void OnRadioMRCS_5();
	afx_msg void OnRadioMRCS_13();
	afx_msg void OnRadioMRCS_14();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetRunButtonText(long lNo);
	WORD m_wSelectNo;
	WORD m_wEnableSelect;
	WORD m_wClickLine;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MANUREMEASRECALCSTATDLG_H__A6DE4A76_3048_45D7_A0D0_741314E642FA__INCLUDED_)
