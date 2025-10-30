#if !defined(AFX_SAMPLESETTINGDLG_H__67685B6C_A842_4E5E_B3DE_53183A9946FD__INCLUDED_)
#define AFX_SAMPLESETTINGDLG_H__67685B6C_A842_4E5E_B3DE_53183A9946FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SampleSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSampleSettingDlg ダイアログ
// ##########################################################################

class CSampleSettingDlg : public CDialog
{
// コンストラクション
public:
	CSampleSettingDlg(BOOL bNew, LPCTSTR lpszName, CWnd* pParent = NULL);

	char m_szName[RECIPE_NAME_LEN + 1];

// 2009.11.09 bagus Stress 追加変更 --{--
	// 2009.08.26 bagus stress --{--
//	STRESS_ELASTIC_TABLE	m_ElasticTable;
	// 2009.08.26 bagus stress --}--
// 2009.11.09 bagus Stress 追加変更 --}--

protected:
// ダイアログ データ
	//{{AFX_DATA(CSampleSettingDlg)
	enum { IDD = IDD_SAMPLE_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_TeachingButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSampleSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSampleSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTeachingButton();
	afx_msg void OnSelchangeSampleShape();
// 2009.11.09 bagus Stress 追加変更 --{--
//	afx_msg void OnSelchangeMaterialElasticModulus();
// 2009.11.09 bagus Stress 追加変更 --}--
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
// 2009.11.09 bagus Stress 追加変更 --{--
//	void InitCombo_MaterialElasticModulus();
// 2009.11.09 bagus Stress 追加変更 --}--
	void InitCombo_SampleShape();
	BOOL CheckData();

	SAMPLEINFO m_SampleInfo;
	STAGE_CONFIG m_StageConfig;
	BOOL m_bNew;
	// 2009.08.26 bagus stress --{--
	SYSTEM_CONFIG m_SystemConfig;
	// 2009.08.26 bagus stress --}--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SAMPLESETTINGDLG_H__67685B6C_A842_4E5E_B3DE_53183A9946FD__INCLUDED_)
