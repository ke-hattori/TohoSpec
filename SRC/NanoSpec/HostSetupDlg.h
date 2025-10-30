#if !defined(AFX_HOSTSETUPDLG_H__76F72B54_E885_4D96_ADE0_53CD3AAED179__INCLUDED_)
#define AFX_HOSTSETUPDLG_H__76F72B54_E885_4D96_ADE0_53CD3AAED179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HostSetupDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CHostSetupDlg ダイアログ

class CHostSetupDlg : public CDialog
{
// コンストラクション
public:
	CHostSetupDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CHostSetupDlg)
	enum { IDD = IDD_SYSTEM_HOST_SETUP_DLG };
	CIPAddressCtrl	m_localIpAddress;
	UINT	m_nPortNumber;
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHostSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CHostSetupDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HOST_ADDRESS m_HostAddress;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HOSTSETUPDLG_H__76F72B54_E885_4D96_ADE0_53CD3AAED179__INCLUDED_)
