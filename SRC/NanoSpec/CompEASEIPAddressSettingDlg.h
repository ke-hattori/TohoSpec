#pragma once

// CompEASEIPAddressSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CCompEASEIPAddressSettingDlg ダイアログ
// ##########################################################################

class CCompEASEIPAddressSettingDlg : public CDialog
{
// コンストラクション
public:
	CCompEASEIPAddressSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CCompEASEIPAddressSettingDlg)
	enum { IDD = IDD_COMPEASE_IPADDRESS_SETTING_DLG };
	//}}AFX_DATA
	CIPAddressCtrl	m_IpAddress;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCompEASEIPAddressSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCompEASEIPAddressSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	COMPEASE_CONFIG m_CompEaseConfig;
};
