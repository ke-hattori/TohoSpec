#if !defined(AFX_SIGNALTOWERSETTINGDLG_H__92FB2A29_0B7A_480F_BC1F_DF2BCB76E26F__INCLUDED_)
#define AFX_SIGNALTOWERSETTINGDLG_H__92FB2A29_0B7A_480F_BC1F_DF2BCB76E26F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SignalTowerSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSignalTowerSettingDlg ダイアログ
// ##########################################################################

class CSignalTowerSettingDlg : public CDialog
{
// コンストラクション
public:
	CSignalTowerSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CSignalTowerSettingDlg)
	enum { IDD = IDD_SIGNAL_TOWER_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_InitializeTestButton;
	CShadeButtonST m_IdleTestButton;
	CShadeButtonST m_RunTestButton;
	CShadeButtonST m_AlarmTestButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSignalTowerSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//{{AFX_MSG(CSignalTowerSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnInitializeTestButton();
	afx_msg void OnIdleTestButton();
	afx_msg void OnRunTestButton();
	afx_msg void OnAlarmTestButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	///// Functions /////
	void InitCombo_SignalTowerLight(UINT nID, int iSelect);
	void InitCombo_SignalTowerBuzzer(UINT nID, int iSelect);

	SIGNAL_TOWER m_SignalTower;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SIGNALTOWERSETTINGDLG_H__92FB2A29_0B7A_480F_BC1F_DF2BCB76E26F__INCLUDED_)
