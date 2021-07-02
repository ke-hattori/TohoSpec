#pragma once

/////////////////////////////////////////////////////////////////////////////
// CCompEASEPollingStatus スレッド

class CCompEASEPollingStatus : public CWinThread
{
	DECLARE_DYNCREATE(CCompEASEPollingStatus)
protected:
	CCompEASEPollingStatus();           // 動的生成に使用されるプロテクト コンストラクタ

// アトリビュート
public:

// オペレーション
public:
	virtual ~CCompEASEPollingStatus();
	void Quit(void){m_Run = FALSE;};

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCompEASEPollingStatus)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCompEASEPollingStatus)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	BOOL m_Run;
};
