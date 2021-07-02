#if !defined(AFX_CCDCALIBRATIONDLG_H__37578E62_8781_4335_8D19_EA3D9C0E76DB__INCLUDED_)
#define AFX_CCDCALIBRATIONDLG_H__37578E62_8781_4335_8D19_EA3D9C0E76DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CcdCalibrationDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CCcdCalibrationDlg ダイアログ
// ##########################################################################

class CCcdCalibrationDlg : public CDialog
{
// コンストラクション
public:
	CCcdCalibrationDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCcdCalibrationDlg)
	enum { IDD = IDD_CCD_CALIBRATION_DLG };
	//}}AFX_DATA

// Operation
public:
	int m_iSelect;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCcdCalibrationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCcdCalibrationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CCDCALIBRATIONDLG_H__37578E62_8781_4335_8D19_EA3D9C0E76DB__INCLUDED_)
