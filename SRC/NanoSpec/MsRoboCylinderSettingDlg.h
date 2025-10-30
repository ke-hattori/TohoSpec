#if !defined(AFX_MSROBOCYLINDERSETTINGDLG_H__B583E088_C645_44A5_9318_E5F54FFEE8E4__INCLUDED_)
#define AFX_MSROBOCYLINDERSETTINGDLG_H__B583E088_C645_44A5_9318_E5F54FFEE8E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsRoboCylinderSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMsRoboCylinderSettingDlg ダイアログ

class CMsRoboCylinderSettingDlg : public CDialog
{
// コンストラクション
public:
	CMsRoboCylinderSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMsRoboCylinderSettingDlg)
	enum { IDD = IDD_MS_ROBO_CYLINDER_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	MS_CONFIG m_MsConfig;
	int m_iCurrNo;
	CString m_strName;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	BOOL CheckData();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMsRoboCylinderSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMsRoboCylinderSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeIndex();
	virtual void OnOK();
	afx_msg void OnCheckEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MSROBOCYLINDERSETTINGDLG_H__B583E088_C645_44A5_9318_E5F54FFEE8E4__INCLUDED_)
