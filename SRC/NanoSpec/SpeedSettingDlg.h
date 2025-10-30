#if !defined(AFX_SPEEDSETTINGDLG_H__80EF8F27_4900_4DE4_8756_C96C5DB28037__INCLUDED_)
#define AFX_SPEEDSETTINGDLG_H__80EF8F27_4900_4DE4_8756_C96C5DB28037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpeedSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSpeedSettingDlg ダイアログ

class CSpeedSettingDlg : public CDialog
{
// コンストラクション
public:
	CSpeedSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSpeedSettingDlg)
	enum { IDD = IDD_SPEED_SETTING_DLG };
	//}}AFX_DATA

// Operation
public:
	///// Variable /////
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	STAGE_MOTION_SPEED_DATA m_StageMotionSpeedData[MAX_AXIS];

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSpeedSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL
	void DDX_CustomSpeedDataText(CDataExchange* pDX, int nIDC, int& value);
	void DDV_CustomXYSpeedDataCheck(CDataExchange* pDX, int& value);
	void DDV_CustomZSpeedDataCheck(CDataExchange* pDX, int& value);
	void DDV_CustomJogDataCheck(CDataExchange* pDX, int& value);

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSpeedSettingDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetOperationLog(LPCTSTR pszLog);

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SPEEDSETTINGDLG_H__80EF8F27_4900_4DE4_8756_C96C5DB28037__INCLUDED_)
