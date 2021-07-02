#if !defined(AFX_CTASIO_H__BA3B6842_4531_49D8_9A28_0AAD6B8635FD__INCLUDED_)
#define AFX_CTASIO_H__BA3B6842_4531_49D8_9A28_0AAD6B8635FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtaSio.h : header file
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です

/*
 *	マクロ定義
 */
#define	PKT_DATA_LEN_MAX	(1024)					/* 最大データ長	*/
/* エラー定義	*/
#define	CTASIO_ERR_UNINIT		(-1)	/* 01:未初期化です	*/
#define	CTASIO_ERR_RCVBUFFULL	(-2)	/* 02:受信バッファフル	*/
#define	CTASIO_ERR_SEND_FAIL	(-3)	/* 03:送信失敗	*/
#define	CTASIO_ERR_PARAM		(-4)	/* 04:パラメタエラー	*/
#define	CTASIO_ERR_NORCVDATA	(-5)	/* 05:受信データはありません	*/
#define	CTASIO_ERR_INVALID_PKT	(-6)	/* 06:パケットコードが範囲外です	*/
#define	CTASIO_ERR_INVALID_CMD	(-7)	/* 07:コマンドコードが範囲外です	*/
#define	CTASIO_ERR_INVALID_EVT	(-8)	/* 08:イベントコードが範囲外です	*/
#define	CTASIO_ERR_NOLF			(-9)	/* 09:LF がありません	*/
#define	CTASIO_ERR_NOCR			(-10)	/* 10:CR がありません	*/
#define	CTASIO_ERR_SUM			(-11)	/* 11:チェックサム不一致	*/
#define	CTASIO_ERR_TOOSHORT		(-12)	/* 12:受信データ長が短すぎます	*/
#define	CTASIO_ERR_CTRLCODE		(-13)	/* 13:制御コードがあった	*/
#define	CTASIO_ERR_INVALIDLEN	(-14)	/* 14:受信データ長異常	*/
#define	CTASIO_ERR_RCVFAIL		(-15)	/* 15:受信処理失敗	*/
/* パケットコード	*/
enum PACKET_CODE {
	PKT_CMD = 0,	/* コマンド	*/
	PKT_EVT,		/* イベントレポート	*/
	PKT_RES,		/* レスポンス	*/
	PKT_ERR,		/* エラーレスポンス	*/
	PKT_MAX,
};
/* コマンドコード	*/
enum COMMAND_CODE {
	CMD_STA = 0,	/* コマンド：状態要求			*/
	CMD_ORG,		/* コマンド：原点復帰			*/
	CMD_MOD,		/* コマンド：測定モードセット	*/
	CMD_CID,		/* コマンド：カセット情報セット	*/
	CMD_PID,		/* コマンド：基板情報セット		*/
	CMD_SPT,		/* コマンド：基板厚み情報セット	*/
	CMD_MST,		/* コマンド：測定開始			*/
	CMD_MPE,		/* コマンド：基板終了通知		*/
	CMD_MCE,		/* コマンド：カセット終了通知	*/
	CMD_RST,		/* コマンド：トラブルリセット	*/
	CMD_END,		/* コマンド：FAMAS 終了			*/
	CMD_MAX,
	EVT_CMO,		/* イベント：移動完了			*/
	EVT_CST,		/* イベント：設定完了			*/
	EVT_CMP,		/* イベント：１ポイント測定完了	*/
	EVT_CMA,		/* イベント：全ポイント測定完了	*/
	EVT_REP,		/* イベント：エラー発生			*/
	EVT_MAX,
};


/////////////////////////////////////////////////////////////////////////////
// CCtaSio thread

class CCtaSio : public CWinThread
{
	DECLARE_DYNCREATE(CCtaSio)
public:
	CCtaSio(DWORD dwCreateFlags = 0);	// protected constructor used by dynamic creation
	virtual ~CCtaSio();
protected:

// Attributes
public:

// Operations
public:
	DWORD ResumeSIO(DWORD dwParentThreadID);
	int IsExistRcvData(void);
	int GetRcvData(PACKET_CODE* penumPacketCode, COMMAND_CODE* penumCmdCode, char* pszRcvData);
	int CmdSend(const PACKET_CODE enumPacketCode, const COMMAND_CODE enumdwCmdCode, const char* pszData);
	int PacketSend(const char* pszSendPkt);
// 2009.11.18 K.Matsuo -->
//	BOOL Initialize(int iComNo, int iBaudRate, int iByteSize, int iParity, int iStopBits, BOOL bHWS);
	BOOL Initialize(LPCTSTR pszCommNo, LPCTSTR pszBaudRate, LPCTSTR pszByteSize, LPCTSTR pszParity, LPCTSTR pszStopBits, BOOL bHWS);
// 2009.11.18 K.Matsuo <--

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtaSio)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
protected:
//	virtual ~CCtaSio();

	// Generated message map functions
	//{{AFX_MSG(CCtaSio)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void HWS_RecvLoop(void);
	char* m_pszCmdBufHWS;
	BOOL HWS_RcvBufSet(const PACKET_CODE enumPacketCode, const COMMAND_CODE enumdwCmdCode, const char* pszData);
	BOOL m_bHWS;
	void Logging(LPCTSTR strLog);
	void RecvLoop(void);
	BOOL NotifyParent(UINT uiMsg, WPARAM wParam, LPARAM lParam);
	void LoggingSIO(BOOL bMode, LPCSTR pszLogData, DWORD dwLen);
	int m_iIdxWr;
	int m_iIdxRd;
	BOOL m_bInited;
	DWORD m_dwParentThreadID;
	LPVOID m_pvRcvBufs;
	int RcvBufGet(char* pszRcvData);
	int RcvBufSet(char* pszRcvData);
	int CalcCheckSum(const char* pszData, char* pszChkSum);
	COMMTIMEOUTS m_CommTimeOuts;
	CRITICAL_SECTION m_csRcvBuf;
	CRITICAL_SECTION m_csSend;
	CRITICAL_SECTION m_csComDev;
	HANDLE m_hCom;
	BOOL m_bInitedInstance;
	DWORD m_dwCreateFlags;
	CEvent m_cSyncEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTASIO_H__BA3B6842_4531_49D8_9A28_0AAD6B8635FD__INCLUDED_)
