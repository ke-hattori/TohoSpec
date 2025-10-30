#if !defined(AFX_SRCCDARRAYSETTINGDLG_H__7DA12EFD_7C10_4ACE_9BB4_93EECD993452__INCLUDED_)
#define AFX_SRCCDARRAYSETTINGDLG_H__7DA12EFD_7C10_4ACE_9BB4_93EECD993452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrCcdArraySettingDlg.h : ヘッダー ファイル
//

#include "SharedMemory.h"
#include "NanoGraphLib.h"	//折れ線・マッピンググラフ用


// ##########################################################################
// CSrCcdArraySettingDlg ダイアログ
// ##########################################################################

class CSrCcdArraySettingDlg : public CDialog
{
// コンストラクション
public:
	CSrCcdArraySettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	~CSrCcdArraySettingDlg();
// ダイアログ データ
	//{{AFX_DATA(CSrCcdArraySettingDlg)
	enum { IDD = IDD_SR_CCD_ARRAY_SETTING_DLG };
	CNanoMeasureGraph*	m_pCCDCalibGraph;
	double	m_CCDCalibIntegrationTime;
	//}}AFX_DATA
//Opration

public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_ScanButton;
	CShadeButtonST m_ClearCalibrationButton;

	CGridCtrl m_CCDIntensityListGrid;
	CGridCtrl m_CCDCalibListGrid;
	SR_CONFIG m_SrConfig;
	SR_CCD_ARRAY m_SrCcdArray[SR_CCD_ARRAY_ITEM_MAX];
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	CString m_strLens;

	CFont m_Font;
	int m_iStartWaveLength;
	int m_iEndWaveLength;
	BOOL m_bScan_StopFlg;

	void SetData();
	BOOL CheckData();
	void LightIntensityListGrid_Init();
	void CalibPixelListGrid_Init();
	void AddLightIntensityListAndGraph(int iPointIndex);
	void DrawLightIntensityGraph_Init();
	void InitCombo_Lens();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrCcdArraySettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrCcdArraySettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnScanButton();
	afx_msg void OnCalibClearButton();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT SetMeasurementData(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT CalibrationSet(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT CalibrationComplete(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRCCDARRAYSETTINGDLG_H__7DA12EFD_7C10_4ACE_9BB4_93EECD993452__INCLUDED_)



