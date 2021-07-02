#if !defined(AFX_STAGECONFIGSETTINGDLG_H__80040020_9000_4BD6_BED4_3002F8552D9C__INCLUDED_)
#define AFX_STAGECONFIGSETTINGDLG_H__80040020_9000_4BD6_BED4_3002F8552D9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StageConfigSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CStageConfigSettingDlg ダイアログ
// ##########################################################################

class CStageConfigSettingDlg : public CDialog
{
// コンストラクション
public:
	CStageConfigSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CStageConfigSettingDlg)
	enum { IDD = IDD_STAGE_CONFIG_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_TeachingButton;
	CShadeButtonST m_SetButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStageConfigSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStageConfigSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTeachingButton();
	afx_msg void OnChangeStageEdgeX();
	afx_msg void OnChangeStageEdgeY();
	afx_msg void OnSetMachineOriginButton();
	afx_msg void OnSelchangeRotateViewCombo();
	//}}AFX_MSG
	afx_msg void OnStageOrgPosRadioButton(UINT nID);
	afx_msg void OnStageCoordDirComboBox(UINT nID);
	DECLARE_MESSAGE_MAP()

private:
	void InitRadio_StageOrgPos();
	void InitCombo_StageCoord();
// 2013.01.17 bagus rotate xy view -->
	void InitCombo_RotateViewCoord();
// 2013.01.17 bagus rotate xy view <--
	BOOL CheckData();
	void SetCoordRange();

	STAGE_CONFIG m_StageConfig;
	SYSTEM_CONFIG ml_SystemConfig;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STAGECONFIGSETTINGDLG_H__80040020_9000_4BD6_BED4_3002F8552D9C__INCLUDED_)
