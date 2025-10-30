#if !defined(AFX_MAINTENANCEPOSITIONSETTINGDLG_H__D4947BC3_6815_4EDE_B88A_BBD96EB03CB2__INCLUDED_)
#define AFX_MAINTENANCEPOSITIONSETTINGDLG_H__D4947BC3_6815_4EDE_B88A_BBD96EB03CB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorStatic.h"

// MaintenancePositionSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CMaintenancePositionSettingDlg ダイアログ
// ##########################################################################

class CMaintenancePositionSettingDlg : public CDialog
{
// コンストラクション
public:
	CMaintenancePositionSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CMaintenancePositionSettingDlg)
	enum { IDD = IDD_MAINTENANCE_POSITION_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_MaintenancePositionSetupButton;
	CColorStatic m_stcMaintenancePosX;
	CColorStatic m_stcMaintenancePosY;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMaintenancePositionSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMaintenancePositionSettingDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeMaintenancePositionNo();
	afx_msg void OnMaintenancePositionSetupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	MAINTENANCE_POSITION m_TempMaintenancePosition[MAINTENANCE_POSITION_MAX];

private:
	void SetOperationLog(LPCTSTR pszLog);

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	void InitCombo_MaintenancePosNo();

	int m_iCurrNo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINTENANCEPOSITIONSETTINGDLG_H__D4947BC3_6815_4EDE_B88A_BBD96EB03CB2__INCLUDED_)
