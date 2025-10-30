// ComSetupDlg.h : ヘッダー ファイル
//

#pragma once

#include "ComSetupGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CComSetupDlg ダイアログ

class CComSetupDlg : public CDialog
{
// コンストラクション
public:
	CComSetupDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual ~CComSetupDlg();

// ダイアログ データ
	//{{AFX_DATA(CComSetupDlg)
	enum { IDD = IDD_SYSTEM_COM_SETUP_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CComSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	BOOL UpdateComPortData(int iRow, LPCTSTR pszHeadName, COM_PORT* pComPort, BOOL bSaveAndValidate = TRUE);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CComSetupDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	SYSTEM_CONFIG m_SystemConfig;
	COM_SETTING m_ComSetting;
	COM_SETTING m_OldComSetting;

	CComSetupGridCtrl m_ComGridCtrl;

// 2013.11.07 Bagus Add (TohoSpec対応) -->
	CString m_strMsgBoxCaption;
// 2013.11.07 Bagus Add (TohoSpec対応) <--
};
