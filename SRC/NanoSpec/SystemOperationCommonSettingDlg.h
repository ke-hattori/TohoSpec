#if !defined(AFX_SYSTEMOPERATIONCOMMONSETTINGDLG_H__FCEC633C_38D7_4A91_B53D_394E2B23C6DA__INCLUDED_)
#define AFX_SYSTEMOPERATIONCOMMONSETTINGDLG_H__FCEC633C_38D7_4A91_B53D_394E2B23C6DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemOperationCommonSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSystemOperationCommonSettingDlg ダイアログ
// ##########################################################################

class CSystemOperationCommonSettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemOperationCommonSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSystemOperationCommonSettingDlg)
	enum { IDD = IDD_SYSTEM_OPERATION_COMMON_DLG };
	//}}AFX_DATA

//Opration
protected:
	CShadeButtonST m_SystemConfigurationSettingButton;
	CShadeButtonST m_OriginalPositionSettingButton;
	CShadeButtonST m_HandlerLoadPositionSettingButton;
	CShadeButtonST m_GeneralPositionSettingButton;
	CShadeButtonST m_StageCoordinateSettingButton;
	CShadeButtonST m_SampleSettingButton;
	CShadeButtonST m_SubstrateThicknessSettingListButton;
	CShadeButtonST m_EdgeAlignerSettingButton;
	CShadeButtonST m_ZAxisOffsetSettingButton;
	CShadeButtonST m_HostSettingButton;
	CShadeButtonST m_ComSettingButton;
	CShadeButtonST m_SignalTowerSettingButton;
	CShadeButtonST m_JogDirectionSettingButton;

	SYSTEM_CONFIG m_SystemConfig;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationCommonSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationCommonSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOriginalPositionSettingButton();
	afx_msg void OnHandlerLoadPositionSettingButton();
	afx_msg void OnGeneralPositionSettingButton();
	afx_msg void OnSampleSettingButton();
	afx_msg void OnSubstrateThicknessSettingListButton();
	afx_msg void OnEdgeAlignerSettingButton();
	afx_msg void OnZAxisOffsetSettingButton();
	afx_msg void OnHostSettingButton();
	afx_msg void OnComSettingButton();
	afx_msg void OnSignalTowerSettingButton();
	afx_msg void OnStageConfigSettingButton();
	afx_msg void OnSystemConfigurationSettingButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnJogDirectionSettingButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
	void InitButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMOPERATIONCOMMONSETTINGDLG_H__FCEC633C_38D7_4A91_B53D_394E2B23C6DA__INCLUDED_)
