#if !defined(AFX_LOGONDLG_H__602B7A69_9CAF_4338_9718_A705111ADC93__INCLUDED_)
#define AFX_LOGONDLG_H__602B7A69_9CAF_4338_9718_A705111ADC93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogOnDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CLogOnDlg ダイアログ
// ##########################################################################

class CLogOnDlg : public CDialog
{
// コンストラクション
public:
	CLogOnDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CLogOnDlg)
	enum { IDD = IDD_LOG_ON_DLG };
	CString m_strUserName;
	CString m_strUserPassword;
	//}}AFX_DATA

//Operation
public:
	///// Variable /////
	CShadeButtonST m_OkButton, m_CancelButton;
	USER_ACCOUNT m_UserAccount;

	///// Function /////
	USER_ACCOUNT GetUser(){ return m_UserAccount; };

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CLogOnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CLogOnDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_LOGONDLG_H__602B7A69_9CAF_4338_9718_A705111ADC93__INCLUDED_)
