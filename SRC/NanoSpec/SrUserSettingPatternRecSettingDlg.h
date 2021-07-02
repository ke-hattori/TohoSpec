#if !defined(AFX_SRUSERSETTINGPATTERNRECSETTINGDLG_H__84897FFC_6DD3_418F_86EA_3BAE2382EC2C__INCLUDED_)
#define AFX_SRUSERSETTINGPATTERNRECSETTINGDLG_H__84897FFC_6DD3_418F_86EA_3BAE2382EC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrUserSettingPatternRecSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSrUserSettingPatternRecSettingDlg ダイアログ
// ##########################################################################

class CSrUserSettingPatternRecSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrUserSettingPatternRecSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrUserSettingPatternRecSettingDlg)
	enum { IDD = IDD_SR_USER_SETTING_PATTERN_REC_SETTING_DLG };
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	SR_PATTERN_REC m_SrUserSettingPatternRec;

	void UpDate(BOOL);
	BOOL CheckData();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrUserSettingPatternRecSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrUserSettingPatternRecSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRUSERSETTINGPATTERNRECSETTINGDLG_H__84897FFC_6DD3_418F_86EA_3BAE2382EC2C__INCLUDED_)
