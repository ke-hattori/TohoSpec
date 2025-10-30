#if !defined(AFX_SRTRANSMITTANCESETTINGDLG_H__96C5A96C_53C0_4BCC_9FF6_78FF06DDD34B__INCLUDED_)
#define AFX_SRTRANSMITTANCESETTINGDLG_H__96C5A96C_53C0_4BCC_9FF6_78FF06DDD34B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrTransmittanceSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSrTransmittanceSettingDlg ダイアログ

class CSrTransmittanceSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrTransmittanceSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrTransmittanceSettingDlg)
	enum { IDD = IDD_SR_TRANSMITTANCE_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	SR_TRANSMIT m_SrTransmittance;

	BOOL CheckData();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrTransmittanceSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrTransmittanceSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChkDonotCheckMeasPoint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRTRANSMITTANCESETTINGDLG_H__96C5A96C_53C0_4BCC_9FF6_78FF06DDD34B__INCLUDED_)
