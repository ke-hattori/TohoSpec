#if !defined(AFX_SELECTSTAGEPROGRAMTYPEDLG_H__135431FB_58DD_49D1_8D49_BB19488903B3__INCLUDED_)
#define AFX_SELECTSTAGEPROGRAMTYPEDLG_H__135431FB_58DD_49D1_8D49_BB19488903B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectStageProgramTypeDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSelectStageProgramTypeDlg ダイアログ

class CSelectStageProgramTypeDlg : public CDialog
{
// コンストラクション
public:
	CSelectStageProgramTypeDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSelectStageProgramTypeDlg)
	enum { IDD = IDD_SELECT_STAGE_PROGRAM_TYPE_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_OkButton, m_CancelButton;
	int m_iHeadType;
// 2009.10.09 bagus StagePGM 共通化 --{--
	int m_iScanType;
// 2009.10.09 bagus StagePGM 共通化 --}--


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSelectStageProgramTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSelectStageProgramTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SELECTSTAGEPROGRAMTYPEDLG_H__135431FB_58DD_49D1_8D49_BB19488903B3__INCLUDED_)
