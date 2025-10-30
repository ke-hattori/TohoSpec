#if !defined(AFX_SRVIEWRANGESETTINGDLG_H__ACF5D269_9D5D_4C16_BE92_FD9D4D88EF8A__INCLUDED_)
#define AFX_SRVIEWRANGESETTINGDLG_H__ACF5D269_9D5D_4C16_BE92_FD9D4D88EF8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrViewRangeSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSrViewRangeSettingDlg ダイアログ
// ##########################################################################

class CSrViewRangeSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrViewRangeSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CSrViewRangeSettingDlg)
	enum { IDD = IDD_SR_VIEW_RANGE_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrViewRangeSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrViewRangeSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	SR_VIEW_RANGE m_SrViewRange;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRVIEWRANGESETTINGDLG_H__ACF5D269_9D5D_4C16_BE92_FD9D4D88EF8A__INCLUDED_)
