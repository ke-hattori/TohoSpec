#if !defined(AFX_SERVOPARAM_H__A1F8E018_B57A_49BD_A1BF_55B24D92928D__INCLUDED_)
#define AFX_SERVOPARAM_H__A1F8E018_B57A_49BD_A1BF_55B24D92928D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServoParam.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CServoParam ダイアログ

class CServoParam : public CDialog
{
// コンストラクション
public:
	void GetData(void);
	void DispData(void);
	void SetDefaultData(void);
	BOOL Create(CWnd *pWnd);
	CWnd* m_pParent;
	CServoParam(CWnd* pParent = NULL);	 // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CServoParam)
	enum { IDD = IDD_MOTCTRLR_DLG };
	long	m_ABS_AccelTimeS_T;
	long	m_ABS_AccelTimeS_X;
	long	m_ABS_AccelTimeS_Y;
	long	m_ABS_AccelTimeS_Z;
	long	m_ABS_AccelTimeT_T;
	long	m_ABS_AccelTimeT_X;
	long	m_ABS_AccelTimeT_Y;
	long	m_ABS_AccelTimeT_Z;
	long	m_HP_AccelTimeS_T;
	long	m_HP_AccelTimeS_X;
	long	m_HP_AccelTimeS_Y;
	long	m_HP_AccelTimeS_Z;
	long	m_HP_AccelTimeT_T;
	long	m_HP_AccelTimeT_X;
	long	m_HP_AccelTimeT_Y;
	long	m_HP_AccelTimeT_Z;
	long	m_HP_BaseBand_T;
	long	m_HP_BaseBand_X;
	long	m_HP_BaseBand_Y;
	long	m_HP_BaseBand_Z;
	long	m_INC_AccelTimeS_T;
	long	m_INC_AccelTimeS_X;
	long	m_INC_AccelTimeS_Y;
	long	m_INC_AccelTimeS_Z;
	long	m_INC_AccelTimeT_T;
	long	m_INC_AccelTimeT_X;
	long	m_INC_AccelTimeT_Y;
	long	m_INC_AccelTimeT_Z;
	long	m_ORG1_AccelTimeS_T;
	long	m_ORG1_AccelTimeS_X;
	long	m_ORG1_AccelTimeS_Y;
	long	m_ORG1_AccelTimeS_Z;
	long	m_ORG1_AccelTimeT_T;
	long	m_ORG1_AccelTimeT_X;
	long	m_ORG1_AccelTimeT_Y;
	long	m_ORG1_AccelTimeT_Z;
	long	m_ORG1_Speed_T;
	long	m_ORG1_Speed_X;
	long	m_ORG1_Speed_Y;
	long	m_ORG1_Speed_Z;
	long	m_ORG2_AccelTimeS_T;
	long	m_ORG2_AccelTimeS_X;
	long	m_ORG2_AccelTimeS_Y;
	long	m_ORG2_AccelTimeS_Z;
	long	m_ORG2_AccelTimeT_T;
	long	m_ORG2_AccelTimeT_X;
	long	m_ORG2_AccelTimeT_Y;
	long	m_ORG2_AccelTimeT_Z;
	long	m_ORG2_Speed_T;
	long	m_ORG2_Speed_X;
	long	m_ORG2_Speed_Y;
	long	m_ORG2_Speed_Z;
	long	m_ORG3_AccelTimeS_T;
	long	m_ORG3_AccelTimeS_X;
	long	m_ORG3_AccelTimeS_Z;
	long	m_ORG3_AccelTimeT_T;
	long	m_ORG3_AccelTimeT_X;
	long	m_ORG3_AccelTimeS_Y;
	long	m_ORG3_AccelTimeT_Y;
	long	m_ORG3_AccelTimeT_Z;
	long	m_ORG3_Speed_T;
	long	m_ORG3_Speed_X;
	long	m_ORG3_Speed_Y;
	long	m_ORG3_Speed_Z;
	long	m_ABS_Speed_T;
	long	m_ABS_Speed_X;
	long	m_ABS_Speed_Y;
	long	m_ABS_Speed_Z;
	long	m_HP_Speed_T;
	long	m_HP_Speed_X;
	long	m_HP_Speed_Y;
	long	m_HP_Speed_Z;
	long	m_INC_Speed_T;
	long	m_INC_Speed_X;
	long	m_INC_Speed_Y;
	long	m_INC_Speed_Z;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CServoParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CServoParam)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBtnLoad();
	afx_msg void OnBtnSave();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SERVOPARAM_H__A1F8E018_B57A_49BD_A1BF_55B24D92928D__INCLUDED_)
