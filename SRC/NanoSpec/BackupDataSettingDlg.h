#if !defined(AFX_BACKUPDATASETTINGDLG_H__86A2F955_0E48_4101_AB80_8828D7547FEB__INCLUDED_)
#define AFX_BACKUPDATASETTINGDLG_H__86A2F955_0E48_4101_AB80_8828D7547FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackupDataSettingDlg.h : ヘッダー ファイル
//
#include "Globals.hxx"

/////////////////////////////////////////////////////////////////////////////
// CBackupDataSettingDlg ダイアログ

class CBackupDataSettingDlg : public CDialog
{
// コンストラクション
public:
	CBackupDataSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CBackupDataSettingDlg)
	enum { IDD = IDD_BACKUP_DATA_SETTIG_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CBackupDataSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CBackupDataSettingDlg)
	afx_msg void OnBackupNowButton();
	afx_msg void OnRestoreButton();
	afx_msg void OnRefBackupPath();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnXmpBackupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int MakeDirectory(char* dir);
	BACKUP_SETTING	m_BackupSetting;
	BACKUP_SETTING	m_OldBackupSetting;

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_BackupNowButton;
	CShadeButtonST m_RestoreButton;
	CShadeButtonST m_XMPBackupButton;
	CShadeButtonST m_RefButton;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BACKUPDATASETTINGDLG_H__86A2F955_0E48_4101_AB80_8828D7547FEB__INCLUDED_)
