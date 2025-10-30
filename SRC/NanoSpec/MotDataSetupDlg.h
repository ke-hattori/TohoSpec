#if !defined(AFX_MOTDATASETUPDLG_H__32E76E8D_26BE_4A56_9B97_0087A3BFC5AA__INCLUDED_)
#define AFX_MOTDATASETUPDLG_H__32E76E8D_26BE_4A56_9B97_0087A3BFC5AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotDataSetupDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMotDataSetupDlg ダイアログ

class CMotDataSetupDlg : public CDialog
{
// コンストラクション
public:
	CMotDataSetupDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMotDataSetupDlg)
	enum { IDD = IDD_MOTION_DATA_SETUP_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMotDataSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMotDataSetupDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MOTDATASETUPDLG_H__32E76E8D_26BE_4A56_9B97_0087A3BFC5AA__INCLUDED_)
