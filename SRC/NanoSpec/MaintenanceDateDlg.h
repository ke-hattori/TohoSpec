#if !defined(AFX_MAINTENANCEDATEDLG_H__0CE79CAE_F4A3_48AA_8CA0_6A2880475CC7__INCLUDED_)
#define AFX_MAINTENANCEDATEDLG_H__0CE79CAE_F4A3_48AA_8CA0_6A2880475CC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaintenanceDateDlg.h : ヘッダー ファイル
//

#ifdef REFRESH_TIMER_ID
	#undef REFRESH_TIMER_ID
#endif
#define REFRESH_TIMER_ID		(1)

#ifdef REFRESH_TIMER_ELAPSE
	#undef REFRESH_TIMER_ELAPSE
#endif
#define REFRESH_TIMER_ELAPSE	(100L)

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDateDlg ダイアログ

class CMaintenanceDateDlg : public CDialog
{
// コンストラクション
public:
	CMaintenanceDateDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMaintenanceDateDlg)
	enum { IDD = IDD_MAINTENANCE_DATE_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	CComboBox	m_DaysComboBox;
	CComboBox	m_AlarmMainteComboBox;
	CComboBox	m_LifeTimeComboBox;
	CComboBox	m_AlarmLampComboBox;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMaintenanceDateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMaintenanceDateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeMaintenanceLifetimeCombo();
	afx_msg void OnMaintenanceDataSetButton();
	afx_msg void OnLampMaintenanceDataSetButton();
	afx_msg void OnCloseButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void MakeTotalTimeString( COleDateTimeSpan pstime, char* csBuff );
	void SetOperationLog(LPCTSTR pszLog);

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	MAINTENANCE_DATE m_pMaintenanceDate;
	MAINTENANCE_LAMP m_pMaintenanceLamp;
	COleDateTimeSpan m_pPassageHour;		// 経過時間

	CShadeButtonST m_MainteDataSetButton;
	CShadeButtonST m_LampMainteDataSetButton;
	CShadeButtonST m_CloseButton;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINTENANCEDATEDLG_H__0CE79CAE_F4A3_48AA_8CA0_6A2880475CC7__INCLUDED_)
