#if !defined(AFX_ALARMLOGDLG_H__25098580_49AA_4F0C_9959_340765D1124D__INCLUDED_)
#define AFX_ALARMLOGDLG_H__25098580_49AA_4F0C_9959_340765D1124D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmLogDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmLogDlg ダイアログ

class CAlarmLogDlg : public CDialog
{
// コンストラクション
public:
	CAlarmLogDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CAlarmLogDlg)
	enum { IDD = IDD_ALARM_LOG_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// Operation
public:
	///// Variable /////
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	CShadeButtonST m_OkButton, m_CancelButton;

	///// Functions /////
	void InitList();
	void AddAlarmHis(int Index, ALARM_HIS* pAlarmHis);
	void SetCursorToLast();
	void Restore(char* Path, char* filename,int LogMax,int HisMax);
	void AlarmHis_StrToHis(char* pszStr, ALARM_HIS* pAlarmHis);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAlarmLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAlarmLogDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ALARMLOGDLG_H__25098580_49AA_4F0C_9959_340765D1124D__INCLUDED_)
