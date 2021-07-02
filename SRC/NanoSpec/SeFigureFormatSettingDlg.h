#if !defined(AFX_SeFigureFormatSettingDlg_H__97CF82A6_1C35_4679_96AB_B66C89477D7A__INCLUDED_)
#define AFX_SeFigureFormatSettingDlg_H__97CF82A6_1C35_4679_96AB_B66C89477D7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeFigureFormatSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// SeFigureFormatSettingDlg ダイアログ
// ##########################################################################

class SeFigureFormatSettingDlg : public CDialog
{
// コンストラクション
public:
	SeFigureFormatSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(SeFigureFormatSettingDlg)
	enum { IDD = IDD_SR_FIGURE_FORMAT_SETTING_DLG };
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	SR_FIGURE_FORMAT m_SeFigureFormat;

	int m_iHeadType;
	CString m_strCaption;

	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(SeFigureFormatSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(SeFigureFormatSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SeFigureFormatSettingDlg_H__97CF82A6_1C35_4679_96AB_B66C89477D7A__INCLUDED_)
