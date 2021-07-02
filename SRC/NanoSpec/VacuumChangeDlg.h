#if !defined(AFX_VACUUMCHANGEDLG_H__B8CF4765_2787_4FD1_BEED_D601A74BCA8A__INCLUDED_)
#define AFX_VACUUMCHANGEDLG_H__B8CF4765_2787_4FD1_BEED_D601A74BCA8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VacuumChangeDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CVacuumChangeDlg ダイアログ

class CVacuumChangeDlg : public CDialog
{
// コンストラクション
public:
	CVacuumChangeDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CVacuumChangeDlg)
	enum { IDD = IDD_VACUUM_CHANGE_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// Operation
	///// Variable /////
	CShadeButtonST m_VacuumOnButton;
	CShadeButtonST m_VacuumOffButton;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CVacuumChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CVacuumChangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnVacuumOnButton();
	afx_msg void OnVacuumOffButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VACUUMCHANGEDLG_H__B8CF4765_2787_4FD1_BEED_D601A74BCA8A__INCLUDED_)
