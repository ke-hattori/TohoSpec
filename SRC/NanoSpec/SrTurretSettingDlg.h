#if !defined(AFX_SRTURRETSETTINGDLG_H__19F6D106_835F_4539_8CAA_5690AAB8D8CF__INCLUDED_)
#define AFX_SRTURRETSETTINGDLG_H__19F6D106_835F_4539_8CAA_5690AAB8D8CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrTurretSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSrTurretSettingDlg ダイアログ
// ##########################################################################

class CSrTurretSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrTurretSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrTurretSettingDlg)
	enum { IDD = IDD_SR_TURRET_SETTING_DLG };
	//}}AFX_DATA

//Operation
public:
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	int m_iCurrNo;
	CString m_strName;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	void GridDisp(CGridCtrl* pGrid, int row, int col, char* text, DWORD TextStyle);
	void GridEditable(BOOL Editable);
	BOOL CheckData();
	void UpdateXmpAngle();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrTurretSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrTurretSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeNo();
	virtual void OnOK();
	afx_msg void OnCheckEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRTURRETSETTINGDLG_H__19F6D106_835F_4539_8CAA_5690AAB8D8CF__INCLUDED_)
