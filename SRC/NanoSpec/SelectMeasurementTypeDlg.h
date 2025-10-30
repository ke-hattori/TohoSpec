#if !defined(AFX_SELECTMEASUREMENTTYPEDLG_H__820F1FB5_6EF8_45B2_8F14_29604D16F810__INCLUDED_)
#define AFX_SELECTMEASUREMENTTYPEDLG_H__820F1FB5_6EF8_45B2_8F14_29604D16F810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectMeasurementTypeDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSelectMeasurementTypeDlg ダイアログ
// ##########################################################################

class CSelectMeasurementTypeDlg : public CDialog
{
// コンストラクション
public:
	CSelectMeasurementTypeDlg(BOOL bRecipeDefaultValueSetting = FALSE,
		int iDefaultValueSettingHeadType = HEAD_TYPE_SR, CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSelectMeasurementTypeDlg)
	enum { IDD = IDD_SELECT_MEASUREMENT_TYPE_DLG };
	CListBox	m_lbMeasTypeList;
	CComboBox	m_cbHeadType;
	//}}AFX_DATA

//Opration
public:
	int m_iHeadType;
	int m_iMeasType;
// 2009.10.29 bagus 2点間 修正 --{--
	BOOL m_bRecalibration;
// 2009.10.29 bagus 2点間 修正 --}--

private:
	int m_iDefaultValueSettingHeadType;
	BOOL m_bRecipeDefaultValueSetting;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	HEAD_TYPE_CONFIG m_HeadType;
	DEFAULT_HEAD_TYPE m_DefaultHeadType;

	void SaveMeasurementType();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSelectMeasurementTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSelectMeasurementTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkMeasurementTypeList();
	afx_msg void OnSelchangeHeadType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SELECTMEASUREMENTTYPEDLG_H__820F1FB5_6EF8_45B2_8F14_29604D16F810__INCLUDED_)
