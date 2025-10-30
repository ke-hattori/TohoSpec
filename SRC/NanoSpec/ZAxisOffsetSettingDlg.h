#if !defined(AFX_ZAXISOFFSETSETTINGDLG_H__CB7AA3E0_C3BB_4FE0_BAFD_89FA0137F132__INCLUDED_)
#define AFX_ZAXISOFFSETSETTINGDLG_H__CB7AA3E0_C3BB_4FE0_BAFD_89FA0137F132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZAxisOffsetSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CZAxisOffsetSettingDlg ダイアログ
// ##########################################################################

class CZAxisOffsetSettingDlg : public CDialog
{
// コンストラクション
public:
	CZAxisOffsetSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CZAxisOffsetSettingDlg)
	enum { IDD = IDD_ZAXIS_OFFSET_SETTING_DLG };
	CString m_strLens;
	//}}AFX_DATA
	CShadeButtonST m_StartButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	//{{AFX_VIRTUAL(CZAxisOffsetSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CZAxisOffsetSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnStartButton();
	afx_msg void OnSelchangeLens();
	afx_msg void OnSelchangeDefaultDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void LoadRecipeData();
	void InitCombo_StageProgram();
	void InitCombo_Lens();
	void InitCombo_DefaultDisplay();
	BOOL CheckData();

	SR_TURRET m_SrTurret[SR_LENS_MAX];
	MAIN_RCP_INFO m_MainRcpInfo;
	MEAS_PROG_INFO m_MeasProgInfo;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ZAXISOFFSETSETTINGDLG_H__CB7AA3E0_C3BB_4FE0_BAFD_89FA0137F132__INCLUDED_)
