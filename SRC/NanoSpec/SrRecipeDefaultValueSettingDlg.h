#if !defined(AFX_SRRECIPEDEFAULTVALUESETTINGDLG_H__7417C7FF_75E5_4D06_A532_E27046BC7C8E__INCLUDED_)
#define AFX_SRRECIPEDEFAULTVALUESETTINGDLG_H__7417C7FF_75E5_4D06_A532_E27046BC7C8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrRecipeDefaultValueSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSrRecipeDefaultValueSettingDlg ダイアログ
// ##########################################################################

class CSrRecipeDefaultValueSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrRecipeDefaultValueSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrRecipeDefaultValueSettingDlg)
	enum { IDD = IDD_SR_RECIPE_DEFAULT_VALUE_SETTING_DLG };
	//}}AFX_DATA

//Operation
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_MainRecipeButton;
	CShadeButtonST m_MeasurementProgramButton;
	int			   m_DefaultHeadType;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrRecipeDefaultValueSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrRecipeDefaultValueSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMainRecipeButton();
	afx_msg void OnMeasurementProgramButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRRECIPEDEFAULTVALUESETTINGDLG_H__7417C7FF_75E5_4D06_A532_E27046BC7C8E__INCLUDED_)
