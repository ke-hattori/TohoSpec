#if !defined(AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_)
#define AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupTeachingMSDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingMSDlg ダイアログ

class CPointSetupTeachingMSDlg : public COverlayDialog
{
// コンストラクション
public:
	CPointSetupTeachingMSDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	int	m_iTeachingMode;			// 自動/手動モード

public:
	typedef enum _TEACHING_MODE
	{
		MODE_AUTO_MEAS = 0,
		MODE_MANUAL_MEAS,
		MODE_MAX,
	} TEACHING_MODE;


// ダイアログ データ
	//{{AFX_DATA(CPointSetupTeachingMSDlg)
	enum { IDD = IDD_POINT_SETUP_TEACHING_MS_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_NextPointButton;

	int m_iCurrPoint;							// 現在設定しようとしているポイント
	BOOL m_bMoveNext;							// MoveNext押下フラグ

	long m_lX;
	long m_lY;
	long m_lZ;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;		// STAGE_PROG_INFO_HDR構造体

	int m_iProcessStatusOld;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPointSetupTeachingMSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointSetupTeachingMSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHomePositionButton();
	afx_msg void OnSelendokMicroScopeFocusPosition();
	afx_msg void OnNextPointButton();
	//}}AFX_MSG
// 2009.11.09 bagus MS 修正 --{--
	afx_msg LRESULT OnNexResponse(WPARAM wparam, LPARAM lparam);
// 2009.11.09 bagus MS 修正 --}--
	DECLARE_MESSAGE_MAP()

	virtual void ButtonEnableCheck(void);
private:
	void InitCombo_Lens();
	void SoftJoyStickXyzEnable(BOOL bEnable);

	MS_CONFIG m_MsConfig;

// 2009.11.09 bagus MS 修正 --{--
	int		m_nSelLens;
	BOOL	m_bMSModeLampFlicker;
// 2009.11.09 bagus MS 修正 --}--
// matsuhisa 2009.12.26 added >>>
	int		m_nLensSensorItem;
// matsuhisa 2009.12.26 added <<<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_)
