#if !defined(AFX_MAINTENANCENOTICEDLG_H__58C45343_C3B7_4276_A7B6_9A07D73C56C0__INCLUDED_)
#define AFX_MAINTENANCENOTICEDLG_H__58C45343_C3B7_4276_A7B6_9A07D73C56C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaintenanceNoticeDlg.h : ヘッダー ファイル
//

#include "Colorstatic.h"

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceNoticeDlg ダイアログ

class CMaintenanceNoticeDlg : public CDialog
{
// コンストラクション
public:
	CMaintenanceNoticeDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMaintenanceNoticeDlg)
	enum { IDD = IDD_MAINTENANCE_NOTICE_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


	void FontCreation();
	BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	void ChangeTopMost(BOOL bTopMost);



// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMaintenanceNoticeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMaintenanceNoticeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFont m_Font;
	CColorStatic m_stcMntNotice;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINTENANCENOTICEDLG_H__58C45343_C3B7_4276_A7B6_9A07D73C56C0__INCLUDED_)
