#if !defined(AFX_SELECTSAMPLETYPEDLG_H__6AD7F704_0A4C_43F2_B39C_CECF5FFCD6CB__INCLUDED_)
#define AFX_SELECTSAMPLETYPEDLG_H__6AD7F704_0A4C_43F2_B39C_CECF5FFCD6CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectSampleTypeDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSelectSampleTypeDlg ダイアログ
// ##########################################################################

class CSelectSampleTypeDlg : public CDialog
{
// コンストラクション
public:
	CSelectSampleTypeDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSelectSampleTypeDlg)
	enum { IDD = IDD_SELECT_SAMPLE_TYPE_DLG };
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton, m_CancelButton;
	char m_szName[RECIPE_NAME_LEN + 1];
	//2009.08.28 bagus stress --{--
	//サンプル形状の絞り込みように要求するヘッドタイプを渡しておく
	int m_iHeadType;
	//2009.08.28 bagus stress --}--

	BOOL InitCombo_Sample();


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSelectSampleTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSelectSampleTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SELECTSAMPLETYPEDLG_H__6AD7F704_0A4C_43F2_B39C_CECF5FFCD6CB__INCLUDED_)
