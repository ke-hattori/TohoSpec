// SaveAsDlg.h : ヘッダー ファイル
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg ダイアログ

class CSaveAsDlg : public CDialog
{
// コンストラクション
public:
	CSaveAsDlg(int iMode = 0, CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSaveAsDlg)
	enum { IDD = IDD_SAVE_AS_DLG };
	//}}AFX_DATA

//Operation
public:
	CShadeButtonST m_OkButton, m_CancelButton;
	int		m_iMode;
	int		m_iLens;
	CString m_strName;
	CString m_strCaption;
	TCHAR m_szFilePath[MAX_PATH];
	MEAS_PROG_INFO m_MeasProgInfo;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSaveAsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL
	void DDV_CustomRecipeNameEmptyCheck(CDataExchange* pDX, const CString& value);
	void DDV_CustomCheckNameCheck(CDataExchange* pDX, const CString& value);
	void DDV_CustomSystemRecipeBeginningCharCheck(CDataExchange* pDX, const CString& value);
	void DDV_CustomUntitledNameCheck(CDataExchange* pDX, const CString& value);

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSaveAsDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL CheckFileExists(LPCTSTR pszFileName, SYSTEMTIME* lpLastWriteSystemTime);
};
