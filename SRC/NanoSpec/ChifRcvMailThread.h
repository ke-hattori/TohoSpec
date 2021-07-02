#if !defined(AFX_CHIFRCVMAILTHREAD_H__571D72EB_00AE_43CA_9200_0285B55DFD74__INCLUDED_)
#define AFX_CHIFRCVMAILTHREAD_H__571D72EB_00AE_43CA_9200_0285B55DFD74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChifRcvMailThread.h : ヘッダー ファイル
//

#include <Mailslot.h>

/////////////////////////////////////////////////////////////////////////////
// CChiefRcvMailThread スレッド

class CChiefRcvMailThread : public CWinThread
{
	DECLARE_DYNCREATE(CChiefRcvMailThread)
protected:
	CChiefRcvMailThread();			 // 動的生成に使用されるプロテクト コンストラクタ

// アトリビュート
public:

// オペレーション
public:
	CView* m_pcChiefView;
	CString GetPifRcvCmd(void);
	void ShutDown(void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefRcvMailThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void EventProcessP201(void);
// 2009.02.05 K.Matsuo delete -->
//	void EventProcessP306(void);
// 2009.02.05 K.Matsuo delete <--
/* modified 2009.09.10 hmenjo P411 コマンド追加 ---------- { ---------- */
//	void EventProcessP401(void);
/* modified 2009.09.10 hmenjo P411 コマンド追加 ----------				*/
	void EventProcessP401(BOOL bGTRRefer = FALSE);
	void EventProcessP411(void);
/* modified 2009.09.10 hmenjo P411 コマンド追加 ---------- } ---------- */
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
//	void EventProcessP501(void);
/* modified 2009.09.10 hmenjo P511 コマンド追加 ----------				*/
	void EventProcessP501(BOOL bReferMode = FALSE);
	void EventProcessP511(void);
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
// 2014.01.07 bagus Add(Stage None対応) -->
	void EventProcessP514(void);
	void EventProcessP516(void);
// 2014.01.07 bagus Add(Stage None対応) <--
	void EventProcessP601(void);
	void EventProcessP602(void);
	void EventProcessP611(void);
	virtual ~CChiefRcvMailThread();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefRcvMailThread)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CString m_strRcvCmd;
	void EventProcessToNextra(DWORD dwCmdCode);
	CMailslot m_mailslot;
	BOOL m_bShutDown;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHIFRCVMAILTHREAD_H__571D72EB_00AE_43CA_9200_0285B55DFD74__INCLUDED_)
