#if !defined(AFX_MESSAGEDLG_H__B14429F0_6A6E_4388_8659_C8E9C3A23124__INCLUDED_)
#define AFX_MESSAGEDLG_H__B14429F0_6A6E_4388_8659_C8E9C3A23124__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageDlg.h : ヘッダー ファイル
//
#define MESSAGE_LIST_MAX			2000
#define MESSAGE_TEXT_LEN			80
#define MESSAGE_FILE_NAME_J			"StringList_J.ini"
#define MESSAGE_FILE_NAME_E			"StringList_E.ini"

typedef struct{
	int Id;
	char Text[MESSAGE_TEXT_LEN + 1];
}MESSAGE_LIST;

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg ダイアログ

class CMessageDlg : public CDialog
{
// コンストラクション
public:
	CMessageDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMessageDlg)
	enum { IDD = IDD_MESSAGE_DLG };
	CEdit		m_ctlMessage;
	//}}AFX_DATA

	void MessageListLoading();
	void SetCaption(LPCTSTR pszCaption);
	void SetMessage(int Id);
	int MessageSearchIndex(int Id);
	//Saiki 20090530 Add ----->
	void SetMessageString(CString strBuffer);
	//Saiki 20090530 Add <-----
	MESSAGE_LIST m_MessageList[MESSAGE_LIST_MAX];
	int m_MessageMax;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL



// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMessageDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MESSAGEDLG_H__B14429F0_6A6E_4388_8659_C8E9C3A23124__INCLUDED_)
