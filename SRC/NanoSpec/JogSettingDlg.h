#if !defined(AFX_JOGSETTINGDLG_H__D80773C8_D85D_43E3_91C7_E99D5D7CB748__INCLUDED_)
#define AFX_JOGSETTINGDLG_H__D80773C8_D85D_43E3_91C7_E99D5D7CB748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JogSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CJogSettingDlg ダイアログ

class CJogSettingDlg : public CDialog
{
// コンストラクション
public:
	CJogSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CJogSettingDlg)
	enum { IDD = IDD_JOG_SETTING_DLG };
	//}}AFX_DATA

// Operation
public:
	///// Variable /////
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	STAGE_MOTION_JOG_DATA m_StageMotionJogData[MAX_AXIS];

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CJogSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL
	void DDX_CustomJogDataText(CDataExchange* pDX, int nIDC, int& value);
	void DDV_CustomXYJogDataCheck(CDataExchange* pDX, int& value);
	void DDV_CustomZJogDataCheck(CDataExchange* pDX, int& value);

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CJogSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetOperationLog(LPCTSTR pszLog);

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_JOGSETTINGDLG_H__D80773C8_D85D_43E3_91C7_E99D5D7CB748__INCLUDED_)
