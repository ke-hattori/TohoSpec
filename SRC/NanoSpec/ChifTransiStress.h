#if !defined(AFX_CHIFTRANSISTRESS_H__BF38E61A_F1AB_4369_A5EA_CFB1C06FA103__INCLUDED_)
#define AFX_CHIFTRANSISTRESS_H__BF38E61A_F1AB_4369_A5EA_CFB1C06FA103__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChifTransiStress.h : header file
//

#include <afxmt.h>	/* CEvent と CSingleLock で必要です	*/

/*--------------------------------------------------------------------------//
//	状態定義
//--------------------------------------------------------------------------*/
#define	_ST_STRS_DISP(x)	_T(#x)
#define	ST_STRS_DISP(x)		_ST_STRS_DISP(x)
#define	ST_STRS_(x)			ST_STRS_##x
/* 状態の名称を定義→
 *		ここを追加削除した場合は ST_xxx_n の番号に対応した
 *		下記の ST_xxx_n と ST_xxx_DISP_n も合わせてください．*/
#define	ST_STRS_0		ST_STRS_(INIT)			/* 初期状態															*/
#define	ST_STRS_1		ST_STRS_(IDLE)			/* アイドル															*/
#define	ST_STRS_2		ST_STRS_(START)			/* リファレンス or 実測定 判定			(WAIT 状態無し)				*/
#define	ST_STRS_3		ST_STRS_(SRMEAS)		/* SR 測定シーケンス開始 & 完了待ち		(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_4		ST_STRS_(SRMEAS_DONE)	/* SR 測定シーケンス完了処理			(WAIT 状態無し)				*/
#define	ST_STRS_5		ST_STRS_(PRELDPOS)		/* 測定前 ロード Pos 完了待ち			(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_6		ST_STRS_(PINALIGN)		/* ピン アライメント待ち				(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_7		ST_STRS_(LMVSTAPOS)		/* ライン動作開始位置 移動 & 完了待ち	(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_8		ST_STRS_(LMVENDPOS)		/* ライン動作終了位置 移動 & 完了待ち	(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_9		ST_STRS_(COMPLDPOS)		/* 完了後 ロード Pos 完了待ち			(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_10		ST_STRS_(PINDOWNCOMP)	/* 完了後 ピン ダウン & Complete 待ち	(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_11		ST_STRS_(COMPLETEDELAY)	/* Complete 時間待ち					(WAIT 状態無し)				*/
#define	ST_STRS_12		ST_STRS_(WAITABORT)		/* Abort 停止待ち						(WAIT 状態無し)				*/
#define	ST_STRS_13		ST_STRS_(ABORTLDPOS)	/* Abort 後 ロード Pos 完了待ち			(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_14		ST_STRS_(ABORTPINDOWN)	/* Abort 後 ピン ダウン待ち				(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_15		ST_STRS_(ABORTDELAY)	/* Abort 時間待ち						(WAIT 状態無し)				*/
#define	ST_STRS_16		ST_STRS_(PAUSE)			/* ポーズ解除待ち						(WAIT 状態無しの場合有り)	*/
#define	ST_STRS_17		ST_STRS_(PRE_RESUME)	/* レジューム前処理						(WAIT 状態無し)				*/
/* 状態の名称を定義←	*/
enum STATES_STRS
{
	/* 構築時は０になっています．
	 * ので，最初は０として定義された状態です．
	 * しかし，０の状態に対応する関数は自動的に実行されませんので，
	 * 必要があれば InitInstance() などから実行する必要があります．*/

	/* ここを変更した場合は直後にあるステータス文字列の定義も合わせてください．	*/

	ST_STRS_0 = 0,
	ST_STRS_1,
	ST_STRS_2,
	ST_STRS_3,
	ST_STRS_4,
	ST_STRS_5,
	ST_STRS_6,
	ST_STRS_7,
	ST_STRS_8,
	ST_STRS_9,
	ST_STRS_10,
	ST_STRS_11,
	ST_STRS_12,
	ST_STRS_13,
	ST_STRS_14,
	ST_STRS_15,
	ST_STRS_16,
	ST_STRS_17,
};
const LPSTR STATES_STRS_DISP[] = {
	ST_STRS_DISP(ST_STRS_0),
	ST_STRS_DISP(ST_STRS_1),
	ST_STRS_DISP(ST_STRS_2),
	ST_STRS_DISP(ST_STRS_3),
	ST_STRS_DISP(ST_STRS_4),
	ST_STRS_DISP(ST_STRS_5),
	ST_STRS_DISP(ST_STRS_6),
	ST_STRS_DISP(ST_STRS_7),
	ST_STRS_DISP(ST_STRS_8),
	ST_STRS_DISP(ST_STRS_9),
	ST_STRS_DISP(ST_STRS_10),
	ST_STRS_DISP(ST_STRS_11),
	ST_STRS_DISP(ST_STRS_12),
	ST_STRS_DISP(ST_STRS_13),
	ST_STRS_DISP(ST_STRS_14),
	ST_STRS_DISP(ST_STRS_15),
	ST_STRS_DISP(ST_STRS_16),
	ST_STRS_DISP(ST_STRS_17),
};

/*--------------------------------------------------------------------------//
 *	イベント定義
 *--------------------------------------------------------------------------*/
enum EVENTS_STRS
{
	EV_STRS_DO_INIT = 0,		/* 初期化開始	*/
	EV_STRS_INIT_DONE,			/* 初期化終了	*/
	EV_STRS_IDLE,				/* アイドルへ	*/
	EV_STRS_START,				/* 開始指令	*/
	EV_STRS_MODE_REF,			/* リファレンスモード	*/
	EV_STRS_MODE_MEAS,			/* 実測定モード	*/
	EV_STRS_SRMEAS_DONE,		/* SR 測定完了*/
	EV_STRS_LDPOS_DONE,			/* ロード POS 移動完了	*/
	EV_STRS_PIN_DONE,			/* ピン完了	*/
	EV_STRS_LINEMOVE_DONE,		/* ライン動作移動完了	*/
	EV_STRS_HEPA_STOP,			/* HEPA 停止	*/
	EV_STRS_TO_LMENDPOS,		/* ライン終了位置移動へ	*/
	EV_STRS_LINEMEAS_DONE,		/* １ライン測定完了	*/
	EV_STRS_LINEDATA_DONE,		/* １ラインデータ処理完了応答	*/
	EV_STRS_LINE_NEXT,			/* 次ラインへ	*/
	EV_STRS_LINE_ALLDONE,		/* 全ライン終了	*/
	EV_STRS_COMPLETE,			/* Complete	*/
	EV_STRS_PINCMP_DONE,		/* ピンダウン，Complete 成立	*/
	EV_STRS_COMPLETE_DONE,		/* Complete タイムアップ	*/
	EV_STRS_CANCEL,				/* キャンセル	*/
	EV_STRS_CANCEL_DONE,		/* キャンセル完了	*/
	EV_STRS_ABORT_DONE,			/* Abort タイムアップ	*/
	EV_STRS_PAUSE,				/* ポーズ	*/
	EV_STRS_RESUME,				/* レジューム	*/
	EV_STRS_RSM_SRMEAS,			/* レジューム：SR 測定中	*/
	EV_STRS_RSM_SRMEASED,		/* レジューム：SR 測定済	*/
	EV_STRS_RSM_PRELDPOS,		/* レジューム：測定前 ロード Pos 移動中	*/
	EV_STRS_RSM_PINALIGN,		/* レジューム：ピン アライメント	*/
	EV_STRS_RSM_LMVSTAPOS,		/* レジューム：ライン動作開始位置	*/
	EV_STRS_RSM_LMVENDPOS,		/* レジューム：ライン動作終了位置	*/
//Saiki 20110308 Add ----->
	EV_STRS_MODE_FIX_MEAS,		/* 実測定モード(固定膜厚値使用)		*/
//Saiki 20110308 Add <-----
};

/*
 *	マクロ定義
 */
/* シーケンス測定終了ステータス	*/
#define	TR_STRS_OK				0			// シーケンス測定 OK．									*/
#define	TR_STRS_TRSEQ_BUSY		0xffff0702	/* シーケンス測定 トランジションがビジーでした			*/
#define	TR_STRS_HEPASTP_FAIL	0xffff0711	/* HEPA 停止異常でした．								*/
#define	TR_STRS_HEPASTA_FAIL	0xffff0712	/* HEPA 開始異常でした．								*/
#define	TR_STRS_NO_HLDPOS		0xffff0714	/* ハンドラ ロード ポジションでは無かった．				*/
#define	TR_STRS_PINMOVE_FAIL	0xffff0715	/* ピン動作に失敗しました．								*/
#define	TR_STRS_NO_PIN_ALIGN	0xffff0716	/* ピンがアライメント位置でなかった．					*/
#define	TR_STRS_NO_ZPOS			0xffff0717	/* ストレス用 Z 軸位置でなかった．						*/
#define	TR_STRS_MEASPRE_FAIL	0xffff0721	/* 測定モジュール準備で異常．							*/
#define	TR_STRS_MEASSTA_FAIL	0xffff0722	/* 測定モジュールで開始エラー．							*/
#define	TR_STRS_MEASEND_ERR		0xffff0723	/* 測定モジュールで異常完了．							*/
#define	TR_STRS_HLDPOSMOVE_FAIL	0xffff0731	/* ハンドラ ロード ポジションへの移動で失敗しました．	*/
#define	TR_STRS_LINEMOVE_FAIL	0xffff0732	/* ライン動作開始 失敗									*/
#define	TR_STRS_ZMOVE_FAIL		0xffff0733	/* Z 軸移動開始失敗										*/
/* added 2009.08.21 hmenjo ストレス機能追加(51) ---------- { ---------- */
#define	TR_STRS_NOSECTION		0xffff0734	/* 有効なセクションがありません．						*/
/* added 2009.08.21 hmenjo ストレス機能追加(51) ---------- } ---------- */

/* ポーズ中の状態記憶	*/
typedef struct {
	DWORD	dwSrMeasuring;		/* SR シーケンス測定中フラグ		*/
	DWORD	dwPreLoadingPos;	/* 測定前 ロード Pos 移動中フラグ	*/
	DWORD	dwPinAligning;		/* ピン アライメント動作中フラグ	*/
} STRS_PAUSE_STAT;

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiStress thread

class CChiefTransiStress : public CWinThread, public cActive
{
	DECLARE_DYNCREATE(CChiefTransiStress)
public:
	CChiefTransiStress(CWnd* pParent = NULL);			// protected constructor used by dynamic creation
protected:

// Attributes
public:

// Operations
public:
	bool IsPinAligningHWS();	/* added 2009.08.20 hmenjo ストレス機能追加(46) */
	bool IsMeasuring();
	DWORD GetCurrentLineNo();
	bool IsPauseSeq();
	void PreMove();
	DWORD m_dwHeartBeat;
	void HeartBeatPulse(void);
	void TransiEvent(const int iEventNo, const cEventParams* const pEvParams = 0);
	int GetCurrentState(void);
	bool IsIdle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChiefTransiStress)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChiefTransiStress();

	/* 状態別の処理関数	*/
	void enter_ST_STRS_INIT(const cEventParams* const pEvParams = 0);			/*	0 */
	void enter_ST_STRS_IDLE(const cEventParams* const pEvParams = 0);			/*	1 */
	void enter_ST_STRS_START(const cEventParams* const pEvParams = 0);			/*	2 */
	void enter_ST_STRS_SRMEAS(const cEventParams* const pEvParams = 0);			/*	3 */
	void enter_ST_STRS_SRMEAS_DONE(const cEventParams* const pEvParams = 0);	/*	4 */
	void enter_ST_STRS_PRELDPOS(const cEventParams* const pEvParams = 0);		/*	5 */
	void enter_ST_STRS_PINALIGN(const cEventParams* const pEvParams = 0);		/*	6 */
	void enter_ST_STRS_LMVSTAPOS(const cEventParams* const pEvParams = 0);		/*	7 */
	void enter_ST_STRS_LMVENDPOS(const cEventParams* const pEvParams = 0);		/*	8 */
	void enter_ST_STRS_COMPLDPOS(const cEventParams* const pEvParams = 0);		/*	9 */
	void enter_ST_STRS_PINDOWNCOMP(const cEventParams* const pEvParams = 0);	/* 10 */
	void enter_ST_STRS_COMPLETEDELAY(const cEventParams* const pEvParams = 0);	/* 11 */
	void enter_ST_STRS_WAITABORT(const cEventParams* const pEvParams = 0);		/* 12 */
	void enter_ST_STRS_ABORTLDPOS(const cEventParams* const pEvParams = 0);		/* 13 */
	void enter_ST_STRS_ABORTPINDOWN(const cEventParams* const pEvParams = 0);	/* 14 */
	void enter_ST_STRS_ABORTDELAY(const cEventParams* const pEvParams = 0);		/* 15 */
	void enter_ST_STRS_PAUSE(const cEventParams* const pEvParams = 0);			/* 16 */
	void enter_ST_STRS_PRE_RESUME(const cEventParams* const pEvParams = 0);		/* 17 */

	DECLARE_STATEPROC_MAP(CChiefTransiStress);

	// Generated message map functions
	//{{AFX_MSG(CChiefTransiStress)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	afx_msg void OnTransiEvent(WPARAM wparam, LPARAM lparam);
	afx_msg void OnHeartBeat(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
private:
	BOOL MoveToLdPos();
	BOOL m_bPinAligning;			/* ピンのアライメント位置中(HWS 用)	*//* added 2009.08.20 hmenjo ストレス機能追加(46) */
	BOOL m_bMeasStress;				/* ストレス測定中	*/
	STRS_PAUSE_STAT m_PauseStat;	/* ポーズ中の状態記憶	*/
	BOOL m_bPause;					/* ポーズ指令	*/
	int m_iPausePreState;			/* レジューム前状態記憶	*/
	BOOL m_bPreMoving;				/* 先行移動中	*/
	BOOL m_bCancel;					/* キャンセル指令	*/
	DWORD m_iCancelStat;			/* キャンセル ステータス	*/
	BOOL m_bCompleted;				/* １枚測定終了応答 記憶	*/
	DWORD m_dwLineMoveToStartPos;	/* 0：移動前，1:移動中，2：移動済	*/
	DWORD m_dwLineMoveToEndPos;		/* 0：移動前，1:移動中，2：移動済	*/
	BOOL m_bHepaStopped;			/* HEPA 停止 記憶	*/
	DWORD m_dwLineNo;				/* ライン番号(1〜)	*/
	DWORD m_dwParentTransi;			/* 呼出し元トランジション	*/
	int	m_iMeasMode;				/* 測定モード  0：リファレンス，1：実測定	*/
	CView* m_pcChiefView;
	CEvent m_cSyncEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHIFTRANSISTRESS_H__BF38E61A_F1AB_4369_A5EA_CFB1C06FA103__INCLUDED_)
