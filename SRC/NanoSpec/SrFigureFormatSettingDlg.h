#if !defined(AFX_SRFIGUREFORMATSETTINGDLG_H__97CF82A6_1C35_4679_96AB_B66C89477D7A__INCLUDED_)
#define AFX_SRFIGUREFORMATSETTINGDLG_H__97CF82A6_1C35_4679_96AB_B66C89477D7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrFigureFormatSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// SrFigureFormatSettingDlg ダイアログ
// ##########################################################################

class SrFigureFormatSettingDlg : public CDialog
{
// コンストラクション
public:
	SrFigureFormatSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(SrFigureFormatSettingDlg)
	enum { IDD = IDD_SR_FIGURE_FORMAT_SETTING_DLG };
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	SR_FIGURE_FORMAT m_SrFigureFormat;

	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(SrFigureFormatSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(SrFigureFormatSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRFIGUREFORMATSETTINGDLG_H__97CF82A6_1C35_4679_96AB_B66C89477D7A__INCLUDED_)
