#if !defined(AFX_CTACTRL_H__D58029DF_132B_4B1C_805E_F41DF6198581__INCLUDED_)
#define AFX_CTACTRL_H__D58029DF_132B_4B1C_805E_F41DF6198581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CtaCtrl.h : header file
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です

/*
 *	マクロ定義
 */
/* シーケンスパターン	*/
enum SEQ_PATTERN {
	SEQPTN_NONE = 0,	/* 無し	*/
	SEQPTN_INIT,		/* 初期化	*/
	SEQPTN_SETINF,		/* 情報設定	*/
	SEQPTN_MEASSTA,		/* 測定開始	*/
	SEQPTN_MEASEND,		/* 測定終了	*/
	SEQPTN_GETSTAT,		/* 状態取得	*/
	SEQPTN_MAX,
};
/* エラー定義	*/
#define	CTACTL_ERR_DATANONE		(-101)	/* 01:データはありません．	*/
#define	CTACTL_ERR_DATAFORMAT	(-102)	/* 02:データ(フォーマット)エラー	*/
#define	CTACTL_ERR_PKTCMD		(-103)	/* 03:パケット/コマンド 整合性エラー	(未使用)*/
#define	CTACTL_ERR_PARAMERR		(-104)	/* 04:パラメタエラー	*/
#define	CTACTL_ERR_EQBUSY		(-105)	/* 05:測定状態が待機中でない	*/
#define	CTACTL_ERR_EQERR		(-106)	/* 06:装置状態がエラー	*/
#define	CTACTL_ERR_TIMEOUT		(-107)	/* 07:タイムアウト	*/
#define CTACTL_ERR_ILPO			(-108)	/* 08:PO インタロック異常	*/
#define CTACTL_ERR_ILPI			(-109)	/* 09:PI インタロック異常	*/
#define CTACTL_ERR_FORCEEND		(-110)	/* 10:シーケンス強制終了	*/
#define	CTACTL_ERR_RECALIB		(-111)	/* 11:リキャリブエラー	*/
/* CTA ユニット エラーコード	*/
#define	CTACTL_ERCD_RST				(000)	/* 01:エラーリセット	*/
#define	CTACTL_ERCD_HEAD			(100)	/* 02:ヘッダエラー	*/
#define	CTACTL_ERCD_CMD				(101)	/* 03:コマンドエラー	*/
#define	CTACTL_ERCD_SUM				(102)	/* 04:チェックサムエラー	*/
#define	CTACTL_ERCD_LEN				(103)	/* 05:受信データ長エラー	*/
#define	CTACTL_ERCD_IL				(200)	/* 06:インタロックエラー	*/
#define	CTACTL_ERCD_CTRLR			(201)	/* 07:コントローラエラー	*/
#define	CTACTL_ERCD_NO_MEAS			(300)	/* 08:測定動作不可エラー	*/
#define	CTACTL_ERCD_MEASING			(301)	/* 09:測定中エラー	*/
#define	CTACTL_ERCD_NO_OPE			(302)	/* 10:操作不可エラー	*/
#define	CTACTL_ERCD_UNSAVE			(303)	/* 11:未セーブデータ有りエラー	*/
#define	CTACTL_ERCD_MODE			(304)	/* 12:モード指定エラー	*/
#define	CTACTL_ERCD_DUP_ID			(306)	/* 13:ID 重複エラー	*/
#define	CTACTL_ERCD_SET_VALUE		(307)	/* 14:設定値エラー	*/
#define	CTACTL_ERCD_PWR				(210)	/* 15:電源状態変化エラー	*/
#define	CTACTL_ERCD_Z_CTRLR_COM		(212)	/* 16:Z 軸コントローラ通信エラー	*/
#define	CTACTL_ERCD_GENDROP			(213)	/* 17:液滴作成エラー	*/
#define	CTACTL_ERCD_HOST_COM		(214)	/* 18:ホスト通信エラー	*/
#define	CTACTL_ERCD_MOVE_TIMEOUT	(215)	/* 19:指定動作タイムアウトエラー	*/
#define	CTACTL_ERCD_ORG				(216)	/* 20:原点復帰エラー	*/
#define	CTACTL_ERCD_Z_CTRLR			(220)	/* 21:Z 軸コントローラエラー	*/
#define	CTACTL_ERCD_PLC_COM			(222)	/* 22:PLC 通信エラー	*/


/*
 *	構造体定義
 */
typedef struct {			/* 各シーケンスへの受信通知 --------------------*/
	BOOL	bRecved;			/* 受信有り	*/
	BOOL	bAnaed;				/* 解析済み＆応答済み(イベントの場合のみ)	*/
	int		iRslt;				/* 解析結果	*/
	DWORD	dwErrCode;			/* エラー発生イベントコード	*/
	LPDWORD	pdwPktCode;			/* パケット コード	*/
	LPDWORD	pdwCmdCode;			/* コマンド/イベント コード	*/
	LPCSTR	pszRcvData;			/* データ	*/
} CTARCVDATA, *LPCTARCVDATA;
/* データ部の構造 ----------------------------------------------------------*/
typedef union {
	struct {				/* エラーレスポンス -------------------*/
		DWORD	dwErrRes;	/* エラーコード	*/
	} ResERR;
	struct {				/* STA コマンド -----------------------*/
		DWORD	dwZState;	/* Z 軸位置		1：原点位置
											2：待機位置
											3：その他の位置	*/
		DWORD	dwStatus1;	/* 測定状態		1：測定中
											2：待機中
											3：操作不可	*/
		DWORD	dwStatus2;	/* 装置状態		1：エラー無し
											3：ヘッドエラー
											4：通信異常	*/
	} CmdSTA;
	struct {				/* REP イベント -----------------------*/
		DWORD	dwErrCode;	/* エラーコード*/
	} EvtREP;
	struct {				/* CMP イベント -----------------------*/
		int		iPointNo;	/* ポイント No.	*/
		int		iCAngle;	/* 接触角[0.1degree]	*/
		int		iRadius;	/* 半径[0.1um]	*/
		int		iLiquid;	/* 液量[0.01uL]	*/
	} EvtCMP;
} CTADATAINF, *LPCTADATAINF;
typedef const CTADATAINF *LPCCTADATAINF;
typedef struct {			/* 測定情報 ------------------------------------*/
	int		iCtaMode;			/* 測定モード	*/
	char	szCstID[80 + 2];	/* カセット ID	：ASCII 80 文字	*/
	char	szSampleID[80 + 2];	/* 基板 ID		：ASCII 80 文字	*/
	DWORD	dwThick;			/* 基板厚み情報[0.1mm]，範囲(0.0〜10.0mm)	*/
	TCHAR	tszRecalibCA[64];	/* リキャリブ PGM 名	*/
	TCHAR	tszRecalibRad[64];	/* リキャリブ PGM 名	*/
} CTAMEASINF, *LPCTAMEASINF;
typedef const CTAMEASINF *LPCCTAMEASINF;
typedef struct {			/* 測定番号 ------------------------------------*/
	int		i_ScanCnt;			/* 測定番号	*/
	long	lScanX;				/* 測定座標 X	*/
	long	lScanY;				/* 測定座標 Y	*/
	long	lScanZ;				/* 測定座標 Z	*/
	/* 測定データ	*/
} CTAMEASDATA, *LPCTAMEASDATA;
typedef const CTAMEASDATA *LPCCTAMEASDATA;
typedef struct {			/* １ポイントデータ ----------------------------*/
	int		iPointNo;	/* ポイント No.	*/
	double	dCAngle;	/* 接触角[degree]	*/
	double	dRadius;	/* 半径[um]	*/
	double	dLiquid;	/* 液量[uL]	*/
} CTAPOINTDATA, *LPCTAPOINTDATA;
typedef const CTAPOINTDATA *LPCCTAPOINTDATA;


/////////////////////////////////////////////////////////////////////////////
// CCtaCtrl thread

class CCtaCtrl : public CWinThread
{
	DECLARE_DYNCREATE(CCtaCtrl)
public:
	CCtaCtrl(DWORD dwCreateFlags = 0);	// protected constructor used by dynamic creation
	virtual ~CCtaCtrl();
protected:

// Attributes
public:

// Operations
public:
	BOOL Cta_GetStatus(long* plStatus);
	BOOL Cta_ForceEnd(void);
	BOOL Cta_MeasEnd(void);
	BOOL Cta_Measure(int iScanCnt, const STAGE_COORD_XYZ* pStageCoordXyz);
	BOOL Cta_SetThick(DWORD dwThick);
	BOOL Cta_SetCstSample(LPCSTR pstrCstID, LPCSTR pstrSampleID,
							const MAIN_RCP_INFO* pMainRcpInfo);
	BOOL Cta_ModeSet(int iCtaMode);
	BOOL Cta_Initialize(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtaCtrl)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
protected:
//	virtual ~CCtaCtrl();

	// Generated message map functions
	//{{AFX_MSG(CCtaCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	afx_msg void OnCtaCtrlSeqKick(WPARAM wparam, LPARAM lparam);
	afx_msg void OnCtaCtrlTimer(WPARAM wparam, LPARAM lparam);
	afx_msg void OnCtaSioRecved(WPARAM wparam, LPARAM lparam);
	afx_msg void OnCtaSioRcvBufFull(WPARAM wparam, LPARAM lparam);
	afx_msg void OnCtaSioRcvFail(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

private:
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- { ---------- */
	UINT m_uiDelay_ILPO;
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- } ---------- */
/* added 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- { ---------- */
	BOOL m_bErrRep;
/* added 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- } ---------- */
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- { ---------- */
	UINT m_uiTimeout_ILPI;
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- } ---------- */
	UINT m_uiTimeout_MPE_Wait;
	UINT m_uiTimeout_Retry;
	UINT m_uiTimeout_ILPO;
	UINT m_uiTimeout_T9_AllEnd;
	UINT m_uiTimeout_T9_Meas;
	long m_lCtaStatus;
	BOOL m_bHWS;
	BOOL m_bForceEnd;
	BOOL NotifyAlarm(int iAlarmCode);
	CTAMEASDATA m_CtaMeasData;
	BOOL m_bRecvedCMA;
	CTAMEASINF m_CtaMeasInf;
	int SendEvtRes(LPCTARCVDATA pCtaRcvData);
	int SendErrRes(int iResult);
	DWORD m_dwRetryCnt;
	int m_iSeqResult;
	int IsStrAlNum(LPCSTR pstrSrc);
	int IsStrDecimal(LPCSTR pstrSrc);
	int IsStrDecimalSign(LPCSTR pstrSrc);
	int AnaRcvData(LPCTARCVDATA pCtaRcvData, LPCTADATAINF pCtaDataInf);
	BOOL SetTimeOut(BOOL bMode, UINT uiElapse);
	BOOL KickCtrlSeq(int iKickNo = -1);
	int SeqInitCTA(LPCTARCVDATA pCtaRcvData);
	int SeqSetInf(LPCTARCVDATA pCtaRcvData);
	int SeqMeasStart(LPCTARCVDATA pCtaRcvData);
	int SeqMeasEnd(LPCTARCVDATA pCtaRcvData);
	int SeqGetStat(LPCTARCVDATA pCtaRcvData);
	HANDLE m_hEvWaitEndSeq;
	BOOL WaitSeqEndStart(SEQ_PATTERN enumSeqPattern);
	CRITICAL_SECTION m_csCtrlBusy;
	DWORD m_dwSeqPattern;
	int CtrlSeq(void);
	BOOL m_bRcvData;
	BOOL m_bTimeOut;
	void Logging_WinMsg(LPCTSTR strLog, WPARAM wParam, LPARAM lParam);
	void Logging(LPCTSTR strLog);
	BOOL InitializeSIO(void);
	CWinThread* m_pclsCCtaSio;
	BOOL m_bInitedInstance;
	DWORD m_dwCreateFlags;
	CEvent m_cSyncEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTACTRL_H__D58029DF_132B_4B1C_805E_F41DF6198581__INCLUDED_)
