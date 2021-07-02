#if !defined(AFX_USERSETTINGOPERATIONCOMMONSETTINGDLG_H__B21BC6A1_5B56_491B_BBBF_82DBAC713629__INCLUDED_)
#define AFX_USERSETTINGOPERATIONCOMMONSETTINGDLG_H__B21BC6A1_5B56_491B_BBBF_82DBAC713629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingOperationCommonSettingDlg.h : ヘッダー ファイル
//
#include "Misc.h"

// ##########################################################################
// CUserSettingOperationCommonSettingDlg ダイアログ
// ##########################################################################

class CUserSettingOperationCommonSettingDlg : public CDialog
{
// コンストラクション
public:
	CUserSettingOperationCommonSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CUserSettingOperationCommonSettingDlg)
	enum { IDD = IDD_USER_SETTING_OPERATION_COMMON_DLG };
	//}}AFX_DATA

//Opration
protected:
	CShadeButtonST m_ManualLoadPositionSettingButton;
	CShadeButtonST m_UserAccountSettingButton;
	CShadeButtonST m_AccessLevelSettingButton;
	CShadeButtonST m_DeleteDataPeriodSettingButton;
	CShadeButtonST m_DefaultHeadTypeSettingButton;
	CShadeButtonST m_StageUnitSettingButton;
	CShadeButtonST m_MaintenancePositionSettingButton;
	CShadeButtonST m_MaintenanceDateButton;
	//2009.12.10 bagus Recipe Backup --{--
	CShadeButtonST m_BackupDataButton;
	//2009.12.10 bagus Recipe Backup --}--


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationCommonSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationCommonSettingDlg)
	afx_msg void OnManualLoadPositionSettingButton();
	afx_msg void OnUserAccountSettingButton();
	afx_msg void OnAccessLevelSettingButton();
	afx_msg void OnDeleteDataPeriodSettingButton();
	afx_msg void OnDefaultHeadTypeSettingButton();
	afx_msg void OnBackupDataButton();
	afx_msg void OnStageUnitSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnMaintenancePositionSettingButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMaintenanceDateButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERSETTINGOPERATIONCOMMONSETTINGDLG_H__B21BC6A1_5B56_491B_BBBF_82DBAC713629__INCLUDED_)
