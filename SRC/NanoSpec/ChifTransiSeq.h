#if !defined(AFX_CHIFTRANSISEQ_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
#define AFX_CHIFTRANSISEQ_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChifTransiSeq.h : ヘッダー ファイル
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です

//--------------------------------------------------------------------------//
//	状態定義
//--------------------------------------------------------------------------//
#define	_ST_SEQ_DISP(x)	_T(#x)
#define	ST_SEQ_DISP(x)	_ST_SEQ_DISP(x)
#define	ST_SEQ_(x)		ST_SEQ_##x
// 状態の名称を定義→
//		ここを追加削除した場合は ST_xxx_n の番号に対応した
//		下記の ST_xxx_n と ST_xxx_DISP_n も合わせてください．
#define	ST_SEQ_0		ST_SEQ_(INIT)			// 初期状態
#define	ST_SEQ_1		ST_SEQ_(IDLE)			// アイドル
#define	ST_SEQ_2		ST_SEQ_(REFER)			// SR リファレンス測定完了待ち			(WAIT 状態無しの場合有り)
#define	ST_SEQ_3		ST_SEQ_(REFER_DONE)		// SR リファレンス測定完了処理			(WAIT 状態無し)
#define	ST_SEQ_4		ST_SEQ_(DESKEW)			// デスキュー完了待ち					(WAIT 状態無しの場合有り)
#define	ST_SEQ_5		ST_SEQ_(DESKEW_DONE)	// デスキュー完了処理					(WAIT 状態無し)
#define	ST_SEQ_6		ST_SEQ_(POINT_MOVE)		// 測定ポイント移動完了待ち				(WAIT 状態無しの場合有り)
#define	ST_SEQ_7		ST_SEQ_(1POINT)			// SR １ポイント測定完了待ち			(WAIT 状態無しの場合有り)
#define	ST_SEQ_8		ST_SEQ_(1P_DONE)		// SR １ポイント測定完了				(WAIT 状態無し)
#define	ST_SEQ_9		ST_SEQ_(WAITCOMPLETE)	// Complete イベント待ち
#define	ST_SEQ_10		ST_SEQ_(COMPLETE)		// Complete 時間待ち					(WAIT 状態無しの場合有り)
#define	ST_SEQ_11		ST_SEQ_(WAITABORT)		// Abort 停止待ち						(WAIT 状態無し)
#define	ST_SEQ_12		ST_SEQ_(PAUSE)			// ポーズ解除待ち						(WAIT 状態無しの場合有り)
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
#define	ST_SEQ_13		ST_SEQ_(PRE_RESUME)		// レジューム前処理						(WAIT 状態無し)
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
#define	ST_SEQ_14		ST_SEQ_(DREFER)			// SR ダークリファレンス測定完了待ち	(WAIT 状態無しの場合有り)
#define	ST_SEQ_15		ST_SEQ_(DREFER_DONE)	// SR ダークリファレンス測定完了処理	(WAIT 状態無し)
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */
// 状態の名称を定義←
enum STATES_SEQ
{
	// 構築時は０になっています．
	// ので，最初は０として定義された状態です．
	// しかし，０の状態に対応する関数は自動的に実行されませんので，
	// 必要があれば InitInstance() などから実行する必要があります．

	// ここを変更した場合は直後にあるステータス文字列の定義も合わせてください．

	ST_SEQ_0 = 0,
	ST_SEQ_1,
	ST_SEQ_2,
	ST_SEQ_3,
	ST_SEQ_4,
	ST_SEQ_5,
	ST_SEQ_6,
	ST_SEQ_7,
	ST_SEQ_8,
	ST_SEQ_9,
	ST_SEQ_10,
	ST_SEQ_11,
	ST_SEQ_12,
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	ST_SEQ_13,
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
	ST_SEQ_14,
	ST_SEQ_15,
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */
};
const LPSTR STATES_SEQ_DISP[] = {
	ST_SEQ_DISP(ST_SEQ_0),
	ST_SEQ_DISP(ST_SEQ_1),
	ST_SEQ_DISP(ST_SEQ_2),
	ST_SEQ_DISP(ST_SEQ_3),
	ST_SEQ_DISP(ST_SEQ_4),
	ST_SEQ_DISP(ST_SEQ_5),
	ST_SEQ_DISP(ST_SEQ_6),
	ST_SEQ_DISP(ST_SEQ_7),
	ST_SEQ_DISP(ST_SEQ_8),
	ST_SEQ_DISP(ST_SEQ_9),
	ST_SEQ_DISP(ST_SEQ_10),
	ST_SEQ_DISP(ST_SEQ_11),
	ST_SEQ_DISP(ST_SEQ_12),
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	ST_SEQ_DISP(ST_SEQ_13),
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
	ST_SEQ_DISP(ST_SEQ_14),
	ST_SEQ_DISP(ST_SEQ_15),
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */
} ;

//--------------------------------------------------------------------------//
//	イベント定義
//--------------------------------------------------------------------------//
enum EVENTS_SEQ
{
	EV_SEQ_DO_INIT = 0,			// 初期化開始
	EV_SEQ_INIT_DONE,			// 初期化終了
	EV_SEQ_DO_MEAS,				// 測定開始
	EV_SEQ_REFER_DONE,			// SR リファレンス測定完了
	EV_SEQ_REFER_OK,			// SR リファレンス測定 OK 完了
	EV_SEQ_DESKEW_DONE,			// デスキュー 完了
	EV_SEQ_DESKEW_OK,			// デスキュー OK 完了
	EV_SEQ_POINT_MOVE_DONE,		// 測定ポイント移動完了
	EV_SEQ_1P_DONE,				// １ポイント測定完了
	EV_SEQ_1P_NEXT,				// 次ポイント測定開始
	EV_SEQ_1P_ALLDONE,			// 全ポイント測定完了
	EV_SEQ_COMPLETE,			// Complete
	EV_SEQ_CANCEL,				// キャンセル
	EV_SEQ_CANCEL_DONE,			// キャンセル完了
	EV_SEQ_COMPLETE_DONE,		// Complete タイムアップ
	EV_SEQ_PAUSE,				// ポーズ
	EV_SEQ_RESUME,				// レジューム
	EV_SEQ_REFDSKW_PASS,		// リファレンス測定，デスキュー パス
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
	EV_SEQ_DO_DREFER,			/* リファレンス起動	*/
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */
};

/*
 *	マクロ定義
 */
// シーケンス測定終了ステータス
#define	TR_SEQ_OK				0			// シーケンス測定 OK
#define	TR_SEQ_TRSREF_BUSY		0xffff0203	// SR リファレンス測定 トランジションがビジーでした
#define	TR_SEQ_TRDSKW_BUSY		0xffff0204	// デスキュー トランジションがビジーでした
#define	TR_SEQ_TRSR1P_BUSY		0xffff0206	// １ポイント測定 トランジションがビジーでした
#define	TR_SEQ_MOVE_FAIL		0xffff0211	// 測定ポイントへの移動開始が失敗しました
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
#define	TR_SEQ_CTAUNIT_SETERR	0xffff0221	/* CTA 情報設定エラー	*/
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSeq スレッド

class CChiefTransiSeq : public CWinThread, public cActive
{
	DECLARE_DYNCREATE(CChiefTransiSeq)
public:
	CChiefTransiSeq(CWnd* pParent = NULL);	// 動的生成に使用されるプロテクト コンストラクタ
protected:

// アトリビュート
public:

// オペレーション
public:
/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- { ---------- */
	BOOL m_bMeasSeq;				/* Seq 測定中	*/
	bool IsMeasuring();
/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- } ---------- */
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
	bool IsPauseSeq(void);
	DWORD GetCurrentPointNo(void);
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
	DWORD m_dwHeartBeat;
	void HeartBeatPulse(void);
	void PreMove(void);
	void TransiEvent(const int iEventNo, const cEventParams* const pEvParams = 0);
	int GetCurrentState(void);
	bool IsIdle();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefTransiSeq)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CChiefTransiSeq();

	// 状態別の処理関数
	void enter_ST_SEQ_INIT(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_IDLE(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_REFER(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_REFER_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_DESKEW(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_DESKEW_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_POINT_MOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_1POINT(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_1P_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_WAITCOMPLETE(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_COMPLETE(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_WAITABORT(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_PAUSE(const cEventParams* const pEvParams = 0);
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	void enter_ST_SEQ_PRE_RESUME(const cEventParams* const pEvParams = 0);
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
	void enter_ST_SEQ_DREFER(const cEventParams* const pEvParams = 0);
	void enter_ST_SEQ_DREFER_DONE(const cEventParams* const pEvParams = 0);
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */

	DECLARE_STATEPROC_MAP(CChiefTransiSeq);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefTransiSeq)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	afx_msg void OnTransiEvent(WPARAM wparam, LPARAM lparam);
	afx_msg void OnHeartBeat(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
private:
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- { ---------- */
	MEASMODE_ID m_enMeasMode;
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- } ---------- */
/* added 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
	BOOL m_bReferMode;
/* added 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
/* added 2009.08.03 hmenjo ストレス機能追加(16) ---------- { ---------- */
	void NotifyComplete(const cEventParams *const pEvParams);
/* added 2009.08.03 hmenjo ストレス機能追加(16) ---------- } ---------- */
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	int m_iPauseNextState;
	int m_iDeskewing;
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
	BOOL m_bPreMoved;
	BOOL m_bPause;
	BOOL m_bCancel;
	DWORD m_dwPointNo;
	DWORD m_dwParentTransi;
	BOOL m_bPreMove;
	CView* m_pcChiefView;
	CEvent m_cSyncEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHIFTRANSISEQ_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
