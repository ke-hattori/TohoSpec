#if !defined(AFX_JOGDIRSETTINGDLG_H__D5B97CED_4AF6_41AC_B134_B44DE79334C4__INCLUDED_)
#define AFX_JOGDIRSETTINGDLG_H__D5B97CED_4AF6_41AC_B134_B44DE79334C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JogDirSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CJogDirSettingDlg ダイアログ

class CJogDirSettingDlg : public CDialog
{
// コンストラクション
public:
	CJogDirSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	///// Variable /////
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	JOG_DIR_SETTING m_JogDirectionSetting;

// ダイアログ データ
	//{{AFX_DATA(CJogDirSettingDlg)
	enum { IDD = IDD_JOG_DIR_SETTING_DLG };
	//}}AFX_DATA
	BOOL	m_bXySwap;
	BOOL	m_bChangeXSign;
	BOOL	m_bChangeYSign;

	int		m_SelDirX;
	int		m_SelDirY;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CJogDirSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CJogDirSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnStageCoordDirComboBox(UINT nID);

	DECLARE_MESSAGE_MAP()

private:
	void InitCombo_StageCoord();

	void SetOperationLog(LPCTSTR pszLog);
	BOOL CheckData();
	void MakeSaveData();

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_JOGDIRSETTINGDLG_H__D5B97CED_4AF6_41AC_B134_B44DE79334C4__INCLUDED_)
