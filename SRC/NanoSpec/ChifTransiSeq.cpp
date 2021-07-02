// ChifTransiSeq.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "..\\..\\INC\\Active.hxx"
/* modified 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- { ---------- */
//#include "ChifTransiSeq.h"
//#include "ChiefExports.h"
/* modified 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ----------				*/
#include "ChiefExports.h"
#include "ChifTransiSeq.h"
/* modified 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- } ---------- */
#include "nanospec.h"
#include "MainFrm.h"
#include "Deskew.hxx"
#include "ChiefView.h"
#include "ChifTransiSrRefer.h"
#include "ChifTransiDeskew.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "ChifTransiSr1Point.h"
#include "ChifTransiMaster.h"
/* added 2009.08.03 hmenjo ストレス機能追加(15) ---------- { ---------- */
#include "ChifTransiStress.h"
/* added 2009.08.03 hmenjo ストレス機能追加(15) ---------- } ---------- */
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
#include "NanoSpecDoc.h"
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	マクロ定義
 */


/*
 *	構造体定義
 */


/*
 *	ローカルグローバル定義
 */
UINT WM_CHIF_TRSEQ_SETEVENT = RegisterWindowMessage("WM_CHIF_TRSEQ_SETEVENT");	// トランジションイベント発行用ウィンドウメッセージ
UINT WM_CHIF_TRSEQ_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TRSEQ_HEARTBEAT");	// HeartBeat ウィンドウメッセージ


// ##########################################################################
// class CChiefTransiSeq : シーケンス測定 トランジション
// ##########################################################################

// =========================================================================//
//	トランジションテーブル
// =========================================================================//
const Transition TransTable_Seq[] =
{
	//	OldState,				EventId,				NewState

	{	ST_SEQ_INIT,			EV_SEQ_DO_INIT,			ST_SEQ_INIT				},	// 初期化処理を実行(すぐ EV_SEQ_INIT_DONE で)
	{	ST_SEQ_INIT,			EV_SEQ_INIT_DONE,		ST_SEQ_IDLE				},	// 初期化完了してアイドルへ

	{	ST_SEQ_IDLE,			EV_SEQ_DO_MEAS,			ST_SEQ_REFER			},	// リファレンス測定完了待ち へ

	{	ST_SEQ_REFER,			EV_SEQ_REFER_DONE,		ST_SEQ_REFER_DONE		},	// リファレンス測定完了処理 へ
	{	ST_SEQ_REFER,			EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ
	{	ST_SEQ_REFER,			EV_SEQ_REFDSKW_PASS,	ST_SEQ_DESKEW_DONE		},	// リファレンス測定，デスキューをパスして 測定ポイント移動完了待ち へ

	{	ST_SEQ_REFER_DONE,		EV_SEQ_REFER_OK,		ST_SEQ_DESKEW			},	// デスキュー完了待ち へ
	{	ST_SEQ_REFER_DONE,		EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	{	ST_SEQ_REFER_DONE,		EV_SEQ_PAUSE,			ST_SEQ_PAUSE			},	// シーケンス一時停止(ポーズ)，ポーズ解除待ち へ
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */

	{	ST_SEQ_DESKEW,			EV_SEQ_DESKEW_DONE,		ST_SEQ_DESKEW_DONE		},	// デスキュー完了処理 へ
	{	ST_SEQ_DESKEW,			EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	{	ST_SEQ_DESKEW,			EV_SEQ_PAUSE,			ST_SEQ_PAUSE			},	// シーケンス一時停止(ポーズ)，ポーズ解除待ち へ
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */

	{	ST_SEQ_DESKEW_DONE,		EV_SEQ_DESKEW_OK,		ST_SEQ_POINT_MOVE		},	// 測定ポイント移動完了待ち へ
	{	ST_SEQ_DESKEW_DONE,		EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	{	ST_SEQ_DESKEW_DONE,		EV_SEQ_PAUSE,			ST_SEQ_PAUSE			},	// シーケンス一時停止(ポーズ)，ポーズ解除待ち へ
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */

	{	ST_SEQ_POINT_MOVE,		EV_SEQ_POINT_MOVE_DONE,	ST_SEQ_1POINT			},	// 移動完了で１ポイント測定完了待ち へ
	{	ST_SEQ_POINT_MOVE,		EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ

	{	ST_SEQ_1POINT,			EV_SEQ_1P_DONE,			ST_SEQ_1P_DONE			},	// １ポイント測定完了
	{	ST_SEQ_1POINT,			EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ

	{	ST_SEQ_1P_DONE,			EV_SEQ_1P_NEXT,			ST_SEQ_POINT_MOVE		},	// (次の)測定ポイント移動完了待ち へ
	{	ST_SEQ_1P_DONE,			EV_SEQ_1P_ALLDONE,		ST_SEQ_WAITCOMPLETE		},	// COMPLETE イベント待ち へ
	{	ST_SEQ_1P_DONE,			EV_SEQ_PAUSE,			ST_SEQ_PAUSE			},	// シーケンス一時停止(ポーズ)，ポーズ解除待ち へ
	{	ST_SEQ_1P_DONE,			EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
	{	ST_SEQ_1P_DONE,			EV_SEQ_DO_DREFER,		ST_SEQ_DREFER			},	// ダークリファレンス測定完了待ち へ

	{	ST_SEQ_DREFER,			EV_SEQ_REFER_DONE,		ST_SEQ_DREFER_DONE		},	// ダークリファレンス測定完了処理 へ
	{	ST_SEQ_DREFER,			EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ

	{	ST_SEQ_DREFER_DONE,		EV_SEQ_1P_ALLDONE,		ST_SEQ_WAITCOMPLETE		},	// COMPLETE イベント待ち へ
	{	ST_SEQ_DREFER_DONE,		EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */

/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
//	{	ST_SEQ_PAUSE,			EV_SEQ_RESUME,			ST_SEQ_POINT_MOVE		},	// (次の)測定ポイント移動完了待ち へ
/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- 			 */
	{	ST_SEQ_PAUSE,			EV_SEQ_RESUME,			ST_SEQ_PRE_RESUME		},	// レジューム前処理 へ
	{	ST_SEQ_PAUSE,			EV_SEQ_DESKEW_DONE,		ST_SEQ_PAUSE			},	// シーケンス一時停止(ポーズ)のまま
/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
	{	ST_SEQ_PAUSE,			EV_SEQ_CANCEL,			ST_SEQ_WAITABORT		},	// Abort 停止待ち へ

/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	{	ST_SEQ_PRE_RESUME,		EV_SEQ_REFER_OK,		ST_SEQ_DESKEW			},	// デスキュー完了待ち へ
	{	ST_SEQ_PRE_RESUME,		EV_SEQ_DESKEW_DONE,		ST_SEQ_DESKEW_DONE		},	// デスキュー完了処理 へ
	{	ST_SEQ_PRE_RESUME,		EV_SEQ_1P_NEXT,			ST_SEQ_POINT_MOVE		},	// (次の)測定ポイント移動完了待ち へ
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */

	{	ST_SEQ_WAITCOMPLETE,	EV_SEQ_COMPLETE,		ST_SEQ_COMPLETE			},	// Complete 時間待ち へ

	{	ST_SEQ_COMPLETE,		EV_SEQ_COMPLETE_DONE,	ST_SEQ_IDLE				},	// アイドルへ

	{	ST_SEQ_WAITABORT,		EV_SEQ_CANCEL_DONE,		ST_SEQ_IDLE				},	// アイドルへ

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};

// =========================================================================//
//	状態別処理関数テーブル
// =========================================================================//
BEGIN_STATEPROC_MAP(CChiefTransiSeq)
	/*	0 */	ENTER_STATE(ST_SEQ_INIT,			enter_ST_SEQ_INIT),
	/*	1 */	ENTER_STATE(ST_SEQ_IDLE,			enter_ST_SEQ_IDLE),
	/*	2 */	ENTER_STATE(ST_SEQ_REFER,			enter_ST_SEQ_REFER),				// (WAIT 状態無しの場合有り)
	/*	3 */	ENTER_STATE(ST_SEQ_REFER_DONE,		enter_ST_SEQ_REFER_DONE),			// (WAIT 状態無し)
	/*	4 */	ENTER_STATE(ST_SEQ_DESKEW,			enter_ST_SEQ_DESKEW),				// (WAIT 状態無しの場合有り)
	/*	5 */	ENTER_STATE(ST_SEQ_DESKEW_DONE,		enter_ST_SEQ_DESKEW_DONE),			// (WAIT 状態無し)
	/*	6 */	ENTER_STATE(ST_SEQ_POINT_MOVE,		enter_ST_SEQ_POINT_MOVE),			// (WAIT 状態無しの場合有り)
	/*	7 */	ENTER_STATE(ST_SEQ_1POINT,			enter_ST_SEQ_1POINT),				// (WAIT 状態無しの場合有り)
	/*	8 */	ENTER_STATE(ST_SEQ_1P_DONE,			enter_ST_SEQ_1P_DONE),				// (WAIT 状態無し)
	/*	9 */	ENTER_STATE(ST_SEQ_WAITCOMPLETE,	enter_ST_SEQ_WAITCOMPLETE),
	/* 10 */	ENTER_STATE(ST_SEQ_COMPLETE,		enter_ST_SEQ_COMPLETE),				// (WAIT 状態無し)
	/* 11 */	ENTER_STATE(ST_SEQ_WAITABORT,		enter_ST_SEQ_WAITABORT),			// (WAIT 状態無し)
	/* 12 */	ENTER_STATE(ST_SEQ_PAUSE,			enter_ST_SEQ_PAUSE),				// (WAIT 状態無しの場合有り)
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	/* 13 */	ENTER_STATE(ST_SEQ_PRE_RESUME,		enter_ST_SEQ_PRE_RESUME),			// (WAIT 状態無し)
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
	/* 14 */	ENTER_STATE(ST_SEQ_DREFER,			enter_ST_SEQ_DREFER),				// (WAIT 状態無しの場合有り)
	/* 15 */	ENTER_STATE(ST_SEQ_DREFER_DONE,		enter_ST_SEQ_DREFER_DONE),			// (WAIT 状態無し)
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */

END_STATEPROC_MAP(CChiefTransiSeq)


/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSeq

IMPLEMENT_DYNCREATE(CChiefTransiSeq, CWinThread)

CChiefTransiSeq::CChiefTransiSeq(CWnd* pParent /*=NULL*/) : cActive(TransTable_Seq)
{
	TRACE(_T("CChiefTransiSeq::CChiefTransiSeq() \n"));

	m_pcChiefView = (CView*) pParent;

	//	スレッドの起動待ち
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiSeq."));

	//	オブジェクトの自動破棄を禁止
	m_bAutoDelete = FALSE;

}

CChiefTransiSeq::~CChiefTransiSeq()
{
	TRACE(_T("CChiefTransiSeq::~CChiefTransiSeq() \n"));

	//	スレッドの消滅待ち
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiSeq."));
		}
	}
}

BOOL CChiefTransiSeq::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiSeq::InitInstance() \n"));

	// コンストラクタの待ちを解除します
	//		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．
	m_cSyncEvent.SetEvent();

	// 初期状態処理用イベント発行
	TransiEvent(0, 0);

	// 変数初期化
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiSeq::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiSeq::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiSeq, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiSeq)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRSEQ_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRSEQ_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSeq メッセージ ハンドラ

/*
 *	トランジションイベント発行 メッセージハンドラ
 */
void CChiefTransiSeq::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	// トランジション間タイミング調整(とりあえずだけど恒久かも・・)

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat メッセージハンドラ
 */
void CChiefTransiSeq::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	トランジションイベント発行用関数
 */
void CChiefTransiSeq::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
	switch (iEventNo) {
	case EV_SEQ_CANCEL:
		m_bCancel = TRUE;
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_ABRT);	// ここでもあらかじめ ABORT にしておきます
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ----------			   */
		if (TRANSI_MASTER == m_dwParentTransi) {
			((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_ABRT);	// ここでもあらかじめ ABORT にしておきます
		}
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- } ---------- */
		break;
	case EV_SEQ_PAUSE:
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
		{
			int l_iCurrentState = GetCurrentState();
			if (
				(ST_SEQ_PAUSE == l_iCurrentState)
			 || (ST_SEQ_PRE_RESUME == l_iCurrentState)
			 || (TRUE == m_bPause)
				) {
				return;
			}
			m_iPauseNextState = l_iCurrentState;
		}
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
		m_bPause = TRUE;
		break;
	case EV_SEQ_POINT_MOVE_DONE:
		if (0 != m_bPreMove) {
			m_bPreMoved = TRUE;
		}
		break;
	}

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TRSEQ_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TRSEQ_SETEVENT, _T("WM_CHIF_TRSEQ_SETEVENT"), 6, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
}

/*
 *	HeartBeat 発行用関数
 */
void CChiefTransiSeq::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TRSEQ_HEARTBEAT, 0, 0);
}

/*
 *	現在の状態が「アイドル」かどうかをチェック
 *		戻り値：	true	アイドルである
 *					false	アイドルでない
 */
bool CChiefTransiSeq::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_SEQ_IDLE) {
		l_isRc = true;
	} else {
		l_isRc = false;
	}

	return l_isRc;
}

/*
 *	現在の状態を取得
 *		戻り値：	現在の状態
 */
int CChiefTransiSeq::GetCurrentState()
{
	return CurrState;
}
/* added 2009.08.03 hmenjo ストレス機能追加(16) ---------- { ---------- */
/*
 *	呼出元トランジションに完了を通知
 */
void CChiefTransiSeq::NotifyComplete(const cEventParams *const pEvParams)
{
	switch (m_dwParentTransi) {
	case TRANSI_AF:
	case TRANSI_SR_REFER:
	case TRANSI_DESKEW:
	case TRANSI_SR_1POINT:
	default:
		/* 何もしません．	*///
		break;
	case TRANSI_MASTER:
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_SEQ_DONE, pEvParams);
		break;
	case TRANSI_STRESS_SEQ:
		((CChiefTransiStress*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress)->TransiEvent(EV_STRS_SRMEAS_DONE, pEvParams);
		break;
	}
}
/* added 2009.08.03 hmenjo ストレス機能追加(16) ---------- } ---------- */

/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
/*
 *	現在の測定ポイント番号を取得
 *		戻り値：	測定ポイント番号
 */
DWORD CChiefTransiSeq::GetCurrentPointNo()
{
	return m_dwPointNo;
}
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */

/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
/*
 *	現在のポーズ状態を取得
 *		戻り値：	ポーズ状態
 */
bool CChiefTransiSeq::IsPauseSeq()
{
	return (0 == m_bPause)? false : true;
}
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */

/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- { ---------- */
/*
 *	現在，測定中かどうかを取得
 *		戻り値：	測定状態
 */
bool CChiefTransiSeq::IsMeasuring()
{
	return (0 == m_bMeasSeq)? false : true;
}
/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- } ---------- */

/*
 *	測定ポイント先行移動
 */
void CChiefTransiSeq::PreMove()
{
/* deleted 2009.06.05 hmenjo ポーズ時先行移動禁止 削除 ---------- { ---------- */
///* added 2009.06.05 hmenjo ポーズ時先行移動禁止 ---------- { ---------- */
//	if (0 != m_bPause) {
//		return;
//	}
///* added 2009.06.05 hmenjo ポーズ時先行移動禁止 ---------- } ---------- */
/* deleted 2009.06.05 hmenjo ポーズ時先行移動禁止 削除 ---------- } ---------- */
/* added 2009.06.10 hmenjo ポーズ時先行移動禁止 ---------- { ---------- */
	if (0 != m_bPause) {
		return;
	}
/* added 2009.06.10 hmenjo ポーズ時先行移動禁止 ---------- } ---------- */
	if (
//			(ST_SEQ_POINT_MOVE == CurrState)	不要(先行移動禁止)
			(ST_SEQ_1POINT == CurrState)
		 || (ST_SEQ_1P_DONE == CurrState)
		) {
		// １ポイント測定完了待ち のときのみ有効です
		if (0 != ((CChiefView*) m_pcChiefView)->m_bPreMoveSW) {
			// 先行移動設定有り
			LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
			//2009.10.28 bagus 2point-Distance --{--
			WORD wNumScans = l_pStageProgInfoHdr->wNumScans;
			if(l_pStageProgInfoHdr->wHeadType == HEAD_TYPE_SR && l_pStageProgInfoHdr->wScanType == SCAN_TYPE_SR_DISTANCE){
				wNumScans *= 2;
			}
#ifdef CHIEF_REMEASURE_ON	// リメジャー対応 20081225
			DWORD l_dwPointNo = m_dwPointNo + 1;
			//if (((CChiefView*) m_pcChiefView)->GetNextPointNo(&l_dwPointNo) <= l_pStageProgInfoHdr->wNumScans) {
			if (((CChiefView*) m_pcChiefView)->GetNextPointNo(&l_dwPointNo) <= wNumScans ){
#else
			//if ((m_dwPointNo + 1) <= l_pStageProgInfoHdr->wNumScans) {
			if ((m_dwPointNo + 1) <= wNumScans) {
				DWORD l_dwPointNo = m_dwPointNo + 1;
#endif
			//2009.10.28 bagus 2point-Distance --}--
				// 残りの測定ポイントがある
				// 次の測定ポイントを取得
				LPSTAGE_COORD l_pScanPoint = (LPSTAGE_COORD) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pScanPoint);
				STAGE_COORD l_ScanPointStage;
				 l_ScanPointStage = l_pScanPoint[l_dwPointNo - 1];
				// X,Y 座標を記憶
				((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lX = l_ScanPointStage.lX;
				((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lY = l_ScanPointStage.lY;
				// 補正
				TCHAR	l_szMainRcpName[256];
				((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// K.Matsuo 2009.09.01 -->
#ifndef CHIEF_STAGE_ON
				NS_ConvertToStageMoveCoord(&l_ScanPointStage, l_szMainRcpName);

				// 20090123 Deskew K.Matsuo ---->
				STAGE_COORD stageCoordTemp;
				DeskewCorrectToCurrent(l_ScanPointStage, stageCoordTemp);
				l_ScanPointStage = stageCoordTemp;
				// 20090123 Deskew K.Matsuo <----
#endif
// K.Matsuo 2009.09.01 <--

				// 移動処理
				TCHAR l_tszLogText[256];
/* modified 2009.06.10 hmenjo 先行移動時のログ文字列変更 ---------- { ---------- */
//				_stprintf(l_tszLogText, _T("Stage is moving to Point No.%5d."), l_dwPointNo);
/* modified 2009.06.10 hmenjo 先行移動時のログ文字列変更 ---------- 			 */
				_stprintf(l_tszLogText, _T("Stage is moving to Point No.%5d. (PreMove)"), l_dwPointNo);
/* modified 2009.06.10 hmenjo 先行移動時のログ文字列変更 ---------- } ---------- */
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_STAGE_PREMOVING);	// Status Bar
				((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
#ifndef CHIEF_STAGE_ON
//				if (0 == StageMoveAbsoluteEx(&l_ScanPointStage, CHIEF_STGMVABS_MODE)) {		Stage.dll が対応するまでは下記の呼出し方法になります．
				if (0 == StageMoveAbsolute(&l_ScanPointStage)) {
#else
// K.Matsuo 2009.09.01 -->
//				if (0 == NS_StageMoveAbsoluteAsync(&l_ScanPointStage)) {
				LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
				WORD wHeadType = l_pMeasProgInfo->ScanParams.hdr.wHeadType;
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//				// ストレス測定内の膜厚測定
//				if (wHeadType == HEAD_TYPE_STRESS) {
//					wHeadType = HEAD_TYPE_SR;
///* added 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- { ---------- */
//				} else if (HEAD_TYPE_SE == wHeadType) {
//					SE_SETTING l_SeSetting;
//					ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
//					LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
////					if (
////						(TRUE == l_SeSetting.bDoNotMoveShutter)
////					 && ((0 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) || ((1 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) && (1 != l_dwPointNo)))
////					 && (0 == ((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->IsPointManualAdjust(l_dwPointNo))
////						) {
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ----------				*/
//					HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
//					((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, wHeadType, &l_HeadMainRcpInf);
//					if (
//						(TRUE == l_SeSetting.bDoNotMoveShutter)
//					 && ((0 == l_HeadMainRcpInf.nFocus) || ((1 == l_HeadMainRcpInf.nFocus) && (1 != l_dwPointNo)))
//					 && (0 == ((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->IsPointManualAdjust(l_dwPointNo))
//						) {
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
//						/*	運用モードで，かつ，(AF 無し，あるいは，１ポイント目のみ AF 有りで２ポイント目以降)，
//						 *	かつ，PMA 無し，の場合は SE の位置とします．つまり，SE ヘッドオフセット有効．	*/
//						wHeadType = HEAD_TYPE_SE;
//					} else {
//						wHeadType = HEAD_TYPE_SR;
//					}
///* added 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- } ---------- */
//				}
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
				/* ストレス測定内の膜厚測定 判定	*/
				if (wHeadType == HEAD_TYPE_STRESS) {
					wHeadType = HEAD_TYPE_SR;
				}
				LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
				SE_SETTING l_SeSetting;
				ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
				COMPEASE_SETTING l_EASESetting;
				ConfigFile_GetNanoSpecIni(&l_EASESetting, CONFIG_FILE_COMPEASE_SETTING);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
				/* AF 有無 確認		*/
				HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
				((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, wHeadType, &l_HeadMainRcpInf);
				BOOL l_bAF = ((1 == l_HeadMainRcpInf.nFocus) && (1 == l_dwPointNo)) || (2 == l_HeadMainRcpInf.nFocus);
				/* PMA 有無 確認	*/
				BOOL l_bPMA = ((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->IsPointManualAdjust(l_dwPointNo);
				/* SitePR 有無 確認	*/
				BOOL l_bSitePR = FALSE;
				SYSTEM_CONFIG l_SystemConfig;
				ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
				if ((0 < l_SystemConfig.nPRMethod) && (TRUE == l_SystemConfig.bSitePatternRec)) {
					if (SITE_PR_MODE_USE_PR == l_pStageProgInfoHdr->nSitePatternMode) {
						LPSITE_PATTERN l_pSitePattern = (LPSITE_PATTERN) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pSitePattern);
						if ((l_pSitePattern[l_dwPointNo - 1].bEnable == TRUE)
						 && ((strlen(l_pSitePattern[l_dwPointNo - 1].PatternInfo[0].szSitePatternName) != 0)
						  || (strlen(l_pSitePattern[l_dwPointNo - 1].PatternInfo[1].szSitePatternName) != 0))) {
							l_bSitePR = TRUE;
						}
					}
				}
				/* ヘッドタイプ判定	*/
				if (0 != l_bAF) {
					/* AF 有りは，SR	*/
					wHeadType = HEAD_TYPE_SR;
				} else if (0 != l_bSitePR) {
					/* SitePR 有りは，SR	*/
					wHeadType = HEAD_TYPE_SR;
				} else if ((0 == l_bAF) && (0 != l_bPMA) && (HEAD_TYPE_MS != wHeadType)) {
					/* PMA のみで MS 以外は，SR	*/
					wHeadType = HEAD_TYPE_SR;
				} else if ((HEAD_TYPE_SE == wHeadType) && (TRUE != l_SeSetting.bDoNotMoveShutter)) {
					/* SE で DoNotMoveShutter == FALSE は，SR	*/
					wHeadType = HEAD_TYPE_SR;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
				} else if ((HEAD_TYPE_COMPEASE == wHeadType) && (TRUE != l_EASESetting.bDoNotMoveShutter)) {
					/* EASE で DoNotMoveShutter == FALSE は，SR	*/
					wHeadType = HEAD_TYPE_SR;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
				} else {
					;	/* その他の場合は，レシピヘッド	*/
				}
				((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
				if (0 == NS_SampleCoordStageMoveAbsoluteEx(&l_ScanPointStage, l_szMainRcpName, wHeadType)) {
// K.Matsuo 2009.09.01 <--
#endif
					// 移動開始失敗
					((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(6, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed_PreMotion));
					// キャンセルをポスト
					TransiEvent(EV_SEQ_CANCEL, (cEventParams*) TR_SEQ_MOVE_FAIL);
				} else {
					// 移動開始成功
					((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 32;
					// 先行移動フラグをセット
					m_bPreMove = TRUE;
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
					if (FALSE != l_bAF) {
						TCHAR l_tszIniPath[_MAX_PATH];
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//						_stprintf(l_tszIniPath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
						_tcscpy(l_tszIniPath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
						BOOL l_bPreAF = ::GetPrivateProfileInt(_T("PreMove"), _T("PreAFSW"), 0, l_tszIniPath);
						if (0 != l_bPreAF) {
							((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove = 1;
							((CChiefView*) m_pcChiefView)->LogChief(_T("PreAF - m_lPreAFafterPreMove = 1"));
						}
					}
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
					((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
				}
			}
		}
	}
}

//--------------------------------------------------------------------------//
//	状態別処理関数群
//--------------------------------------------------------------------------//
/*
 *	初期状態
 */
void CChiefTransiSeq::enter_ST_SEQ_INIT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[0], (DWORD) pEvParams);

	// 変数を初期化
	m_bPreMove = FALSE;
	m_bPause = FALSE;
	m_bCancel = FALSE;
	m_bPreMoved = FALSE;
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	m_iPauseNextState = 0;
	m_iDeskewing = 0;
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
}

/*
 *	アイドル
 */
void CChiefTransiSeq::enter_ST_SEQ_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[1], (DWORD) pEvParams);

	m_bPreMove = FALSE;
	m_bCancel = FALSE;
	m_bPause = FALSE;
	m_bPreMoved = FALSE;
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	m_iPauseNextState = 0;
	m_iDeskewing = 0;
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
/* added 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
	m_bReferMode = FALSE;
/* added 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- { ---------- */
	m_enMeasMode = MEASMODE_NORM;
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- } ---------- */
/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- { ---------- */
	m_bMeasSeq = FALSE;
/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- } ---------- */

/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- { ---------- */
//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ----------			   */
	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);}
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- } ---------- */
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
	((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_lZ_AF = 0x80000000;
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */

	// WAIT 状態にするため処理はありません．
}

/*
 *	SR リファレンス測定完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSeq::enter_ST_SEQ_REFER(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOBYTE(LOWORD(pEvParams))：呼出元トランジション
/*				HIBYTE(LOWORD(pEvParams))：リファレンスモード
/*				HIWORD(pEvParams)：リファレンス測定，デスキュー，パスモード(リメジャーモード)
/*										＝０：パスしない
/*										≠０：パスする
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[2], (DWORD) pEvParams);

	// 呼出元トランジションを保存
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
////	m_dwParentTransi = (DWORD) pEvParams;
//	m_dwParentTransi = LOWORD(pEvParams);
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
	m_dwParentTransi = LOBYTE(LOWORD(pEvParams));
	m_bReferMode = (0 != HIBYTE(LOWORD(pEvParams)))? TRUE : FALSE;
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
	// 「リファレンス測定」と「デスキュー」をパスするモード(リメジャーモード)
	WORD l_wPassRefDeskew = HIWORD(pEvParams);
	// 測定ポイント番号を初期化
	m_dwPointNo = 0;

/* added 2009.08.26 hmenjo SE ランプ制御 ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* SE ランプフィルタ CLOSE	*/
/* added 2009.08.26 hmenjo SE ランプ制御 ---------- } ---------- */

/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- { ---------- */
//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ----------			   */
	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);}
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- } ---------- */

	// シーケンス測定開始を通知
	SEQ_MEAS_START l_SeqMeasStart;
	if ((0x0d == ((CChiefView*) m_pcChiefView)->m_szSampleID[0]) && (0x00 == ((CChiefView*) m_pcChiefView)->m_szSampleID[1])) {
		l_SeqMeasStart.bGetSamplID = FALSE;
	} else {
		l_SeqMeasStart.bGetSamplID = TRUE;
	}
	_tcscpy(l_SeqMeasStart.szSampleID, ((CChiefView*) m_pcChiefView)->m_szSampleID);
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- { ---------- */
	_tcscpy(l_SeqMeasStart.tszLotID, ((CChiefView*) m_pcChiefView)->m_szLotID);
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- } ---------- */
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- { ---------- */
	/* 測定モードを判定	*/
	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
	switch (l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
	case HEAD_TYPE_SR:
		if ((0 != m_bReferMode)
		 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
			m_enMeasMode = MEASMODE_GTRREF;	/* ガントリ透過率リファレンス測定	*/
		//2009.10.28 bagus 2point-distance --{--
		//} else if(MEAS_PROG_TYPE_SR_DISTANCE == l_pMeasProgInfo->ScanParams.hdr.wScanType){
		//	m_enMeasMode = MEASMODE_DISTANCE;
		//2009.10.28 bagus 2point-distance --}--
		} else {
			m_enMeasMode = MEASMODE_NORM;	/* 通常測定	*/
		}
		break;
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
	case HEAD_TYPE_CTA:
		m_enMeasMode = MEASMODE_CTA;	/* CTA 測定	*/
		break;
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */
	//2009.11.03 bagus MS --{--
	case HEAD_TYPE_MS:
		m_enMeasMode = MEASMODE_MS;
		break;
	//2009.11.03 bagus MS --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		m_enMeasMode = MEASMODE_COMPEASE;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	default:
		m_enMeasMode = MEASMODE_NORM;	/* 通常測定	*/
		break;
	}
	l_SeqMeasStart.enMeasMode = m_enMeasMode;
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- } ---------- */
	COPYDATASTRUCT l_CopyData;
	l_CopyData.dwData = WMCD_CHIF_MEAS_START;
	l_CopyData.cbData = sizeof(SEQ_MEAS_START);
	l_CopyData.lpData = &l_SeqMeasStart;
/* modified 2009.08.03 hmenjo ストレス機能追加(15) ---------- { ---------- */
//	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);
/* modified 2009.08.03 hmenjo ストレス機能追加(15) ----------			   */
	if (TRANSI_MASTER == m_dwParentTransi) {
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);
	}
/* modified 2009.08.03 hmenjo ストレス機能追加(15) ---------- } ---------- */

/* deleted 2009.10.29 hmenjo SE デスキューは有効 ---------- { ---------- */
///* added 2009.08.27 hmenjo SE SR リファレンス動作禁止 ---------- { ---------- */
///* deleted 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- { ---------- */
////	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
///* deleted 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- } ---------- */
//	if (HEAD_TYPE_SE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//		// トランジション更新
//		TransiEvent(EV_SEQ_REFDSKW_PASS, (cEventParams*) TR_DSKW_OK);
//	} else
///* added 2009.08.27 hmenjo SE SR リファレンス動作禁止 ---------- } ---------- */
/* deleted 2009.10.29 hmenjo SE デスキューは有効 ---------- } ---------- */
#ifdef CHIEF_REMEASURE_ON	// リメジャー対応 20081225
	if (0 != l_wPassRefDeskew) {
		// 「リファレンス測定」と「デスキュー」をパスするモード(リメジャーモード)
		// トランジション更新
		TransiEvent(EV_SEQ_REFDSKW_PASS, (cEventParams*) TR_DSKW_OK);
	} else
#endif
/* added 2009.09.09 hmenjo 透過率 Seq から Refer 実行不可 ---------- { ---------- */
	if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
	 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
		/* SR ガントリ透過率では，リファレンスを自動実行しません．
		 *	仕様により，デスキューもパスします．	*/
		/* また，現状は，通常透過率でもリファレンスをパスします．	*/
		/* トランジション更新	*/
		TransiEvent(EV_SEQ_REFDSKW_PASS, (cEventParams*) TR_DSKW_OK);
	//2009.10.30 bagus 2point-distance --{--
	} else if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
	 && (MEAS_PROG_TYPE_SR_DISTANCE == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
		//2点間距離の場合にはリファレンス測定を行わないがデスキューは行う。
		TransiEvent(EV_SEQ_REFER_DONE,(cEventParams *)TRUE);
	//2009.10.30 bagus 2point-distance --{--
	//2009.11.03 bagus MS --{--
	} else if(HEAD_TYPE_MS == l_pMeasProgInfo->ScanParams.hdr.wHeadType){
		//顕微鏡の場合にはリファレンス測定を行わないがデスキューは行う。
		TransiEvent(EV_SEQ_REFER_DONE,(cEventParams *)TRUE);
	//2009.11.03 bagus MS --}--
	} else
/* added 2009.09.09 hmenjo 透過率 Seq から Refer 実行不可 ---------- } ---------- */
	// リファレンス測定指令 発行
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(8, 6)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiSeq::enter_ST_SEQ_REFER() - SrRefer Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// SR リファレンス測定 トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_SEQ_CANCEL, (cEventParams*) TR_SEQ_TRSREF_BUSY);
	} else {
// bagus 2014.01.10 Add(Stage None対応) -->
		SYSTEM_CONFIG l_SystemConfig;
		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

		if (l_SystemConfig.nStageType == STAGE_TYPE_NONE) {
			TransiEvent(EV_SEQ_REFDSKW_PASS, (cEventParams*) TR_DSKW_OK);
			return;
		}
// bagus 2014.01.10 Add(Stage None対応) <--

/* deleted 2009.07.02 hmenjo リファレンス用ターレット&フィルタ切替え移動 ---------- { ---------- */
/*		// ターレット制御(測定用：測定プログラムの Lens)
//		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
//		((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
//		// カラーフィルタ制御(リファレンス測定用：測定プログラムの OpticalFilter)
//		if (0 == MEAS_SrHead_ChangeCcdShutter(l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType)) {
//			// 異常時でも無視します．
//			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSeq::enter_ST_SEQ_REFER()."));
//		}
*/
/* deleted 2009.07.02 hmenjo リファレンス用ターレット&フィルタ切替え移動 ---------- } ---------- */

/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//		((CChiefTransiSrRefer*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer))->TransiEvent(EV_SREF_START, (cEventParams*) TRANSI_SEQ);
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
		((CChiefTransiSrRefer*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer))->TransiEvent(EV_SREF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_SEQ, 0), 0));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	SR リファレンス測定完了処理(WAIT 状態無し)
 */
void CChiefTransiSeq::enter_ST_SEQ_REFER_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[3], (DWORD) pEvParams);

	if (((long) pEvParams) < 0) {
		// 異常完了
		// トランジション更新
		TransiEvent(EV_SEQ_CANCEL, pEvParams);
	} else {
		// 正常完了
		// トランジション更新
/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
//		TransiEvent(EV_SEQ_REFER_OK);
/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- 			 */
		if (0 != m_bPause) {
			m_bPause = FALSE;
			TransiEvent(EV_SEQ_PAUSE, (cEventParams*) ST_SEQ_DESKEW);
		} else {
			TransiEvent(EV_SEQ_REFER_OK);
		}
/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
	}
}

/*
 *	デスキュー完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSeq::enter_ST_SEQ_DESKEW(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[4], (DWORD) pEvParams);
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	if (0 != pEvParams) {
		// ポーズから解除されてココに来たため，そのまま待機し，デスキューの完了を待ちます．
		return;
	}
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */

	// デスキュー指令 発行
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiDeskew*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(4, 6)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiSeq::enter_ST_SEQ_DESKEW() - Deskew Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// デスキュー トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_SEQ_CANCEL, (cEventParams*) TR_SEQ_TRDSKW_BUSY);
	} else {
#if 0		// デスキュートランジションへ移動しました
		// ターレット制御(デスキュー用：メインレシピの Lens，あるいは，ステージプログラムの Lens)
		//		ここではステージプログラムの Lens を使います
		LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
		((CChiefView*) m_pcChiefView)->SelectLens(l_pStageProgInfoHdr->iLens);
		// カラーフィルタ制御(デスキュー用：OPEN にします)
		if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
			// 異常時でも無視します．
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSeq::enter_ST_SEQ_DESKEW()."));
		}
#endif
		((CChiefTransiDeskew*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew))->TransiEvent(EV_DSKW_START, (cEventParams*) TRANSI_SEQ);
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	デスキュー完了処理(WAIT 状態無し)
 */
void CChiefTransiSeq::enter_ST_SEQ_DESKEW_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[5], (DWORD) pEvParams);

	switch ((DWORD) pEvParams) {
	case TR_DSKW_OK:
	case TR_DSKW_AUTO_PROCEED:
	case TR_DSKW_MANU_PROCEED:
		// 正常完了
		{
			// １ポイント目のみ実行されます
			// ターレット制御(測定用：測定プログラムの Lens)
			LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- { ---------- */
//// 2009.09.04 K.Matsuo -->
//			int l_iLens;
//			if (HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//				l_iLens = l_pMeasProgInfo->ScanParams._SR.iLens;
//			}
//			else if (HEAD_TYPE_SE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//				l_iLens = l_pMeasProgInfo->ScanParams._SE.iLens;
//			}
//			else if (HEAD_TYPE_IRSE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//				l_iLens = l_pMeasProgInfo->ScanParams._IR.iLens;
//			}
//			else if (HEAD_TYPE_4PP == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//				l_iLens = l_pMeasProgInfo->ScanParams._RS.iLens;
//			}
//			else if (HEAD_TYPE_CTA == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//				l_iLens = l_pMeasProgInfo->ScanParams._CA.iLens;
//			}
//			else if (HEAD_TYPE_STRESS == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//				l_iLens = l_pMeasProgInfo->ScanParams._SR.iLens;
//			}
//			else {
//				l_iLens = l_pMeasProgInfo->ScanParams._SR.iLens;
//			}
//			((CChiefView*) m_pcChiefView)->SelectLens(l_iLens);
//// 2009.09.04 K.Matsuo <--
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ----------				*/
			((CChiefView*) m_pcChiefView)->SelectLens(((CChiefView*) m_pcChiefView)->GetMeasLens(l_pMeasProgInfo->ScanParams.hdr.wHeadType));
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- } ---------- */
			// カラーフィルタ制御(測定用：測定プログラムの OpticalFilter)
/* modified 2009.08.25 hmenjo SE ランプ制御 ---------- { ---------- */
//			if (0 == MEAS_SrHead_ChangeCcdShutter(l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType)) {
/* modified 2009.08.25 hmenjo SE ランプ制御 ----------				*/
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- { ---------- */
//// 2009.09.04 K.Matsuo -->
//			WORD l_wFilter;
//			if (HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//				l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
//			}
//			else if (HEAD_TYPE_SE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
////				l_wFilter = FILTER_OPEN;
//				l_wFilter = l_pMeasProgInfo->ScanParams._SE.wOpticsFilterType;
//			}
//			else if (HEAD_TYPE_IRSE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
////				l_wFilter = FILTER_OPEN;
//				l_wFilter = l_pMeasProgInfo->ScanParams._IR.wOpticsFilterType;
//			}
//			else if (HEAD_TYPE_4PP == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
////				l_wFilter = FILTER_OPEN;
//				l_wFilter = l_pMeasProgInfo->ScanParams._RS.wOpticsFilterType;
//			}
//			else if (HEAD_TYPE_CTA == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
////				l_wFilter = FILTER_OPEN;
//				l_wFilter = l_pMeasProgInfo->ScanParams._CA.wOpticsFilterType;
//			}
//			else if (HEAD_TYPE_STRESS == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//				l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
//			}
//			else {
//				l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
//			}
//			if (0 == MEAS_SrHead_ChangeCcdShutter(l_wFilter)) {
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ----------			  */
			if (0 == MEAS_SrHead_ChangeCcdShutter(((CChiefView*) m_pcChiefView)->GetMeasFilter(l_pMeasProgInfo->ScanParams.hdr.wHeadType))) {
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- } ---------- */
/* modified 2009.08.25 hmenjo SE ランプ制御 ---------- } ---------- */
				// 異常時でも無視します．
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSeq::enter_ST_SEQ_DESKEW_DONE()."));
			}
// 2009.09.04 K.Matsuo <--
		}
/* modified 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
//		// トランジション更新
///* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
////		TransiEvent(EV_SEQ_DESKEW_OK);
///* modified 2009.06.10 hmenjo ポーズ機能拡張 ----------			   */
//		if (0 != m_bPause) {
//			m_bPause = FALSE;
//			TransiEvent(EV_SEQ_PAUSE, (cEventParams*) ST_SEQ_POINT_MOVE);
//		} else {
//			TransiEvent(EV_SEQ_DESKEW_OK);
//		}
///* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
/* modified 2009.10.29 hmenjo CTA Seq API 追加 ----------			   */
		{	/* CTA ヘッドの場合のみ実施	*/
			BOOL l_bCtaRslt = TRUE;	/* CTA 設定結果	*/
			if (HEAD_TYPE_CTA == ((LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.wHeadType) {
				/* CTA 情報設定 ----------------------------------------------------*/
				/* サンプル ID，ロット ID 取得	*/
				char l_szSampleID[SAMPLE_ID_LEN + 1];
				char l_szLotID[LOT_ID_LEN + 1];
				if ((0x0d == ((CChiefView*) m_pcChiefView)->m_szSampleID[0])
				 && (0x00 == ((CChiefView*) m_pcChiefView)->m_szSampleID[1])) {
					/* 画面から設定された	*/
					((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetSampleId(l_szSampleID);
					((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetLotId(l_szLotID);
				} else {
					/* Pif から設定された	*/
					strcpy(l_szSampleID, ((CChiefView*) m_pcChiefView)->m_szSampleID);
					strcpy(l_szLotID, ((CChiefView*) m_pcChiefView)->m_szLotID);
				}
				MEAS_CtaModeSet(MEAS_CTA_MODE_1);
				l_bCtaRslt =	MEAS_CtaSetCstSample(
										l_szLotID,
										l_szSampleID,
										(LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo
									);
				DWORD l_dwThick;	/* 基板厚み[0.1mm]	*/
				double l_dThick;	/* 基板厚み[um]	*/
				l_dThick = ((LPSTAGE_PROG_INFO_HDR) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr)->SampleInfo.dThickness;
				l_dwThick = (DWORD) (l_dThick / 100.0);
				if (0 != l_bCtaRslt) {
					l_bCtaRslt = MEAS_CtaSetThick(l_dwThick);
				}
			}
			/* トランジション更新 ----------------------------------------------*/
			if (0 == l_bCtaRslt) {
				/* CTA 情報設定エラー	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to set inf to CTA.(MEAS_CtaSetThick)"));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(6, CHRAMSG_OK)), MAKELPARAM(0, ALID_CTA_Setinf));
				TransiEvent(EV_SEQ_CANCEL, (cEventParams*) TR_SEQ_CTAUNIT_SETERR);
			} else if (0 != m_bPause) {
				m_bPause = FALSE;
				TransiEvent(EV_SEQ_PAUSE, (cEventParams*) ST_SEQ_POINT_MOVE);
			} else {
				TransiEvent(EV_SEQ_DESKEW_OK);
			}
		}
/* modified 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		{
			if (HEAD_TYPE_COMPEASE == ((LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.wHeadType) {
				MEAS_CompEASEHead_OpenLampShutter();
			}
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		break;
	default:
		// 異常完了
		// トランジション更新
		TransiEvent(EV_SEQ_CANCEL, pEvParams);
		break;
	}
}

/*
 *	測定ポイント移動完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSeq::enter_ST_SEQ_POINT_MOVE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[6], (DWORD) pEvParams);

	// 測定ポイント番号を更新
#ifdef CHIEF_REMEASURE_ON	// リメジャー対応 20081225
	m_dwPointNo++;
	((CChiefView*) m_pcChiefView)->GetNextPointNo(&m_dwPointNo);
#else
	m_dwPointNo++;
#endif

// bagus 2014.01.10 Add(Stage None対応) -->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	if (l_SystemConfig.nStageType == STAGE_TYPE_NONE) {
		// ステージレシピ指定なし
		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lX = 0;
		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lY = 0;
		// トランジション更新
		TransiEvent(EV_SEQ_POINT_MOVE_DONE);
		return;
	}
// bagus 2014.01.10 Add(Stage None対応) <--

	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		// ハードシミュレートモードの場合は，そのまま移動完了とします．
		// X,Y 座標を暫定で設定
		LPSTAGE_COORD l_pScanPoint = (LPSTAGE_COORD) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pScanPoint);
		STAGE_COORD l_ScanPointStage;
		l_ScanPointStage = l_pScanPoint[m_dwPointNo - 1];
		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lX = l_ScanPointStage.lX;
		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lY = l_ScanPointStage.lY;
		// トランジション更新
		TransiEvent(EV_SEQ_POINT_MOVE_DONE);
	} else {
		// 測定ポイントへ移動
		if (0 != m_bPreMove) {
			// 先行移動済
			if (0 != m_bPreMoved) {
				// すでに先行移動が停止していた
				// トランジション更新
				TransiEvent(EV_SEQ_POINT_MOVE_DONE);
			} else {
				// WAIT 状態にするためトランジションを更新しません．
			}
		} else {
			// 測定ポイントを取得
			LPSTAGE_COORD l_pScanPoint = (LPSTAGE_COORD) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pScanPoint);
			STAGE_COORD l_ScanPointStage;
			l_ScanPointStage = l_pScanPoint[m_dwPointNo - 1];
			// X,Y 座標を記憶
			((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lX = l_ScanPointStage.lX;
			((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lY = l_ScanPointStage.lY;
			// 補正
			TCHAR	l_szMainRcpName[256];
			((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// K.Matsuo 2009.09.01 -->
#ifndef CHIEF_STAGE_ON
			NS_ConvertToStageMoveCoord(&l_ScanPointStage, l_szMainRcpName);

			// 20090123 Deskew K.Matsuo ---->
			STAGE_COORD stageCoordTemp;
			DeskewCorrectToCurrent(l_ScanPointStage, stageCoordTemp);
			l_ScanPointStage = stageCoordTemp;
			// 20090123 Deskew K.Matsuo <----
#endif
// K.Matsuo 2009.09.01 <--

			TCHAR l_tszLogText[256];
			_stprintf(l_tszLogText, _T("Stage is moving to Point No.%5d."), m_dwPointNo);
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_STAGE_MOVING);	// Status Bar
			((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
#ifndef CHIEF_STAGE_ON
//			if (0 == StageMoveAbsoluteEx(&l_ScanPointStage, CHIEF_STGMVABS_MODE)) {		Stage.dll が対応するまでは下記の呼出し方法になります．
			if (0 == StageMoveAbsolute(&l_ScanPointStage)) {
#else
// K.Matsuo 2009.09.01 -->
//			if (0 == NS_StageMoveAbsoluteAsync(&l_ScanPointStage)) {
			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
			WORD wHeadType = l_pMeasProgInfo->ScanParams.hdr.wHeadType;
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//			// ストレス測定内の膜厚測定
//			if (wHeadType == HEAD_TYPE_STRESS) {
//				wHeadType = HEAD_TYPE_SR;
///* added 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- { ---------- */
//			} else if (HEAD_TYPE_SE == wHeadType) {
//				SE_SETTING l_SeSetting;
//				ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
//				LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
////				if (
////					(TRUE == l_SeSetting.bDoNotMoveShutter)
////				 && ((0 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) || ((1 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) && (1 != m_dwPointNo)))
////				 && (0 == ((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->IsPointManualAdjust(m_dwPointNo))
////					) {
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ----------				*/
//				HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
//				((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, wHeadType, &l_HeadMainRcpInf);
//				if (
//					(TRUE == l_SeSetting.bDoNotMoveShutter)
//				 && ((0 == l_HeadMainRcpInf.nFocus) || ((1 == l_HeadMainRcpInf.nFocus) && (1 != m_dwPointNo)))
//				 && (0 == ((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->IsPointManualAdjust(m_dwPointNo))
//					) {
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
//					/*	運用モードで，かつ，(AF 無し，あるいは，１ポイント目のみ AF 有りで２ポイント目以降)，
//					 *	かつ，PMA 無し，の場合は SE の位置とします．つまり，SE ヘッドオフセット有効．	*/
//					wHeadType = HEAD_TYPE_SE;
//				} else {
//					wHeadType = HEAD_TYPE_SR;
//				}
///* added 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- } ---------- */
//			}
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
			/* ストレス測定内の膜厚測定 判定	*/
			if (wHeadType == HEAD_TYPE_STRESS) {
				wHeadType = HEAD_TYPE_SR;
			}
			LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
			SE_SETTING l_SeSetting;
			ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			COMPEASE_SETTING l_EASESetting;
			ConfigFile_GetNanoSpecIni(&l_EASESetting, CONFIG_FILE_COMPEASE_SETTING);
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			/* AF 有無 確認		*/
			HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
			((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, wHeadType, &l_HeadMainRcpInf);
			BOOL l_bAF = ((1 == l_HeadMainRcpInf.nFocus) && (1 == m_dwPointNo)) || (2 == l_HeadMainRcpInf.nFocus);
			/* PMA 有無 確認	*/
			BOOL l_bPMA = ((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->IsPointManualAdjust(m_dwPointNo);
			/* SitePR 有無 確認	*/
			BOOL l_bSitePR = FALSE;
			SYSTEM_CONFIG l_SystemConfig;
			ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
			if ((0 < l_SystemConfig.nPRMethod) && (TRUE == l_SystemConfig.bSitePatternRec)) {
				LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
				if (SITE_PR_MODE_USE_PR == l_pStageProgInfoHdr->nSitePatternMode) {
					LPSITE_PATTERN l_pSitePattern = (LPSITE_PATTERN) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pSitePattern);
					if ((l_pSitePattern[m_dwPointNo - 1].bEnable == TRUE)
					 && ((strlen(l_pSitePattern[m_dwPointNo - 1].PatternInfo[0].szSitePatternName) != 0)
					  || (strlen(l_pSitePattern[m_dwPointNo - 1].PatternInfo[1].szSitePatternName) != 0))) {
						l_bSitePR = TRUE;
					}
				}
			}
			/* ヘッドタイプ判定	*/
			if (0 != l_bAF) {
				/* AF 有りは，SR	*/
				wHeadType = HEAD_TYPE_SR;
			} else if (0 != l_bSitePR) {
				/* SitePR 有りは，SR	*/
				wHeadType = HEAD_TYPE_SR;
			} else if ((0 == l_bAF) && (0 != l_bPMA) && (HEAD_TYPE_MS != wHeadType)) {
				/* PMA のみで MS 以外は，SR	*/
				wHeadType = HEAD_TYPE_SR;
			} else if ((HEAD_TYPE_SE == wHeadType) && (TRUE != l_SeSetting.bDoNotMoveShutter)) {
				/* SE で DoNotMoveShutter == FALSE は，SR	*/
				wHeadType = HEAD_TYPE_SR;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			} else if ((HEAD_TYPE_COMPEASE == wHeadType) && (TRUE != l_EASESetting.bDoNotMoveShutter)) {
				/* EASE で DoNotMoveShutter == FALSE は，SR	*/
				wHeadType = HEAD_TYPE_SR;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			} else {
				;	/* その他の場合は，レシピヘッド	*/
			}
			((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
			if (0 == NS_SampleCoordStageMoveAbsoluteEx(&l_ScanPointStage, l_szMainRcpName, wHeadType)) {
// K.Matsuo 2009.09.01 <--
#endif
				// 移動開始失敗
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(6, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
				// トランジション更新
				TransiEvent(EV_SEQ_CANCEL, (cEventParams*) TR_SEQ_MOVE_FAIL);
			} else {
				// 移動開始成功
				((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 31;
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
				// WAIT 状態にするためトランジションを更新しません．
			}
		}
	}
}

/*
 *	SR １ポイント測定完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSeq::enter_ST_SEQ_1POINT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[7], (DWORD) pEvParams);

	// 測定開始前のヘッド揺れ収束待ち(2008.12.18 追加)
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_SWAYSETTLING);	// Status Bar
	((CChiefView*) m_pcChiefView)->WaitTimeBeforeMeas(1);

	m_bPreMoved = FALSE;
/* added 2009.06.08 hmenjo 先行移動時のポーズバグ修正 ---------- { ---------- */
	m_bPreMove = FALSE;
/* added 2009.06.08 hmenjo 先行移動時のポーズバグ修正 ---------- } ---------- */

	// １ポイント測定指令 発行
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(7, 6)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiSeq::enter_ST_SEQ_1POINT() - SR1point Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// １ポイント測定 トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_SEQ_CANCEL, (cEventParams*) TR_SEQ_TRSR1P_BUSY);
	} else {
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- { ---------- */
///* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
////		DWORD l_dwParam = MAKELONG(TRANSI_SEQ, m_dwPointNo);
///* modified 2009.09.09 hmenjo 引数変更 ---------- 			 */
//		DWORD l_dwParam = MAKELONG(MAKEWORD(TRANSI_SEQ, 0), m_dwPointNo);
///* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ----------				*/
		DWORD l_dwParam = MAKELONG(MAKEWORD(TRANSI_SEQ, m_enMeasMode), m_dwPointNo);
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- } ---------- */
// 2009.09.25 K.Matsuo CTA -->
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// 		LPCTSTR l_pszLotId = ((CChiefView*) m_pcChiefView)->m_szLotID;
// 		LPCTSTR l_pszSampleID = ((CChiefView*) m_pcChiefView)->m_szSampleID;
		TCHAR l_szLotID[256];
		TCHAR l_szSampleID[256];
		if ((0x0d == ((CChiefView*) m_pcChiefView)->m_szSampleID[0])
			&& (0x00 == ((CChiefView*) m_pcChiefView)->m_szSampleID[1])) {
			/* 画面から設定された	*/
			((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetSampleId(l_szSampleID);
			((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetLotId(l_szLotID);
		} else {
			/* Pif から設定された	*/
			strcpy(l_szSampleID, ((CChiefView*) m_pcChiefView)->m_szSampleID);
			strcpy(l_szLotID, ((CChiefView*) m_pcChiefView)->m_szLotID);
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		LPCTSTR l_pszMainRecipeName = ((CChiefView*) m_pcChiefView)->GetMainRecipeNamePtr();
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo;
		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo;
		LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//		MEAS_PrepareForMeasure(l_pszLotId, l_pszSampleID, l_pszMainRecipeName, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgInfoHdr);
		MEAS_PrepareForMeasure(l_szLotID, l_szSampleID, l_pszMainRecipeName, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgInfoHdr);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.09.25 K.Matsuo CTA <--
		((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->TransiEvent(EV_SR1P_START, (cEventParams*) l_dwParam);
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	SR １ポイント測定完了(WAIT 状態無し)
 */
void CChiefTransiSeq::enter_ST_SEQ_1P_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[8], (DWORD) pEvParams);

	AF_VALID l_AFValid;
	if (0 < ((long) pEvParams)) {
		l_AFValid.dwValid = (DWORD) pEvParams;
	} else {
		l_AFValid.dwValid = 0;
	}

	// 測定キャンセルをチェック
	if (0 != m_bCancel) {
		// キャンセル処理
		m_bCancel = FALSE;
		// トランジション更新
		TransiEvent(EV_SEQ_CANCEL);
	} else {
		if (((long) pEvParams) < 0) {
			// １ポイント測定が異常終了だった
			// トランジション更新
			TransiEvent(EV_SEQ_CANCEL, pEvParams);
#if 0	// AF 結果判定を変更 -------- { --------
//		} else if ((4 == l_AFValid.ucResultAF) && (4 == l_AFValid.ucResultPopup)) {
//			// AF ポップアップでキャンセルだった
		} else if (((4 == l_AFValid.ucResultAF) && (4 == l_AFValid.ucResultPopup)) || (5 == l_AFValid.ucResultAF)) {
			// AF ポップアップでキャンセルだった，か，レシピ終了だった
#else	// AF 結果判定を変更 --------
		} else if ((4 == l_AFValid.ucResultPopup) || (5 == l_AFValid.ucResultAF)) {
			// AF ポップアップでキャンセルだった，か，レシピ終了だった
#endif	// AF 結果判定を変更 -------- } --------
			((CChiefView*) m_pcChiefView)->m_bReqCancelComplete = TRUE;	// 測定キャンセル完了報告指令有り
			if (5 != l_AFValid.ucResultAF) {
				// レシピ終了でない場合は報告する
				PifComm_PressMeasureCancelButtonReport();	// 測定キャンセルボタン押下報告
			}
			TransiEvent(EV_SEQ_CANCEL, pEvParams);
		} else {
			// 最終ポイントだったかチェック
			LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
			//2009.10.28 bagus 2point-Distance --{--
// bagus 2014.01.10 Mod(Stage None対応) -->
//			WORD wNumScans = l_pStageProgInfoHdr->wNumScans;
			WORD wNumScans;
			SYSTEM_CONFIG l_SystemConfig;
			ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

			if (l_SystemConfig.nStageType == STAGE_TYPE_NONE) {
				wNumScans = 1;
			}
			else{
				wNumScans = l_pStageProgInfoHdr->wNumScans;
			}
// bagus 2014.01.10 Mod(Stage None対応) <--
			if(l_pStageProgInfoHdr->wHeadType == HEAD_TYPE_SR
			&& l_pStageProgInfoHdr->wScanType == SCAN_TYPE_SR_DISTANCE){
				wNumScans *= 2;
			}
#ifdef CHIEF_REMEASURE_ON	// リメジャー対応 20081225
			DWORD l_dwPointNo = m_dwPointNo + 1;
			//if (
			//	(l_pStageProgInfoHdr->wNumScans <= m_dwPointNo)
			// || (l_pStageProgInfoHdr->wNumScans < ((CChiefView*) m_pcChiefView)->GetNextPointNo(&l_dwPointNo))
			//	) {
			if (
				(wNumScans <= m_dwPointNo)
			 || (wNumScans < ((CChiefView*) m_pcChiefView)->GetNextPointNo(&l_dwPointNo))
				) {
#else
			//if (l_pStageProgInfoHdr->wNumScans <= m_dwPointNo) {
			if (wNumScans <= m_dwPointNo) {
#endif
			//2009.10.28 bagus 2point-Distance --}--
				// 最終ポイントだった
/* modified 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
//				TransiEvent(EV_SEQ_1P_ALLDONE);
/* modified 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ----------			  */
				if (0 != m_bReferMode) {
					/* リファレンスモード	*/
					TransiEvent(EV_SEQ_DO_DREFER);
				} else {
					/* 測定モード	*/
					TransiEvent(EV_SEQ_1P_ALLDONE);
				}
/* modified 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */
			} else {
				// ポーズをチェック
				if (0 != m_bPause) {
					// ポーズ指令 有り
					m_bPause = FALSE;
/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
//					TransiEvent(EV_SEQ_PAUSE);
/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- 			 */
					TransiEvent(EV_SEQ_PAUSE, (cEventParams*) ST_SEQ_POINT_MOVE);
/* modified 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
				} else {
					// 次の測定を行います
					// １ポイント測定完了待ちへ
					TransiEvent(EV_SEQ_1P_NEXT);
				}
			}
		}
	}
}

/*
 *	Complete イベント待ち
 */
void CChiefTransiSeq::enter_ST_SEQ_WAITCOMPLETE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[9], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_DATA_PROCING);	// Status Bar

/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->SentMeasPause(0, TRUE);	// ポーズ解除通知
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */

/* added 2009.08.26 hmenjo SE ランプ制御 ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* SE ランプフィルタ CLOSE	*/
/* added 2009.08.26 hmenjo SE ランプ制御 ---------- } ---------- */
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	if (HEAD_TYPE_COMPEASE == ((LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.wHeadType) {
		MEAS_CompEASEHead_CloseLampShutter();
	}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
/* added 2009.11.24 hmenjo 自動測定終了時は SR フィルタ OPEN ---------- { ---------- */
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/* added 2009.11.24 hmenjo 自動測定終了時は SR フィルタ OPEN ---------- } ---------- */
	// １枚測定終了を データ処理 へ通知(正常)
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_MEAS_END, 0, 0);

	((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_MEAS_END with OK."));

	// WAIT 状態にするためトランジションを更新しません．
}

/*
 *	Complete 時間待ち(WAIT 状態無し)
 */
void CChiefTransiSeq::enter_ST_SEQ_COMPLETE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[10], (DWORD) pEvParams);

/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
	if (HEAD_TYPE_CTA == ((LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.wHeadType) {
		/* CTA ヘッドの場合のみ実施	*/
/* modified 2009.11.06 hmenjo CTA 終了処理失敗では強制終了 ---------- { ---------- */
//		MEAS_CtaMeasEnd();
/* modified 2009.11.06 hmenjo CTA 終了処理失敗では強制終了 ----------			   */
		if (0 == MEAS_CtaMeasEnd()) {
			MEAS_CtaForceEnd();		/* 強制終了	*/
		}
/* modified 2009.11.06 hmenjo CTA 終了処理失敗では強制終了 ---------- } ---------- */
	}
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */

/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- { ---------- */
	m_bMeasSeq = FALSE;
/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- } ---------- */

/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */

/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- { ---------- */
//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_COMP);
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ----------			   */
	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_COMP);}
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- } ---------- */

	// 時間待ち
	::Sleep(TIMER_DELAY_COMPLETE);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_DATA_PROC_COMPLETE);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// トランジション更新
	TransiEvent(EV_SEQ_COMPLETE_DONE);

	// 呼出元トランジションに完了を通知
/* modified 2009.08.03 hmenjo ストレス機能追加(16) ---------- { ---------- */
//	((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_SEQ_DONE, TR_SEQ_OK);
/* modified 2009.08.03 hmenjo ストレス機能追加(16) ----------			   */
	NotifyComplete((cEventParams*) TR_SEQ_OK);
/* modified 2009.08.03 hmenjo ストレス機能追加(16) ---------- } ---------- */
}

/*
 *	Abort 停止待ち(WAIT 状態無し)
 */
void CChiefTransiSeq::enter_ST_SEQ_WAITABORT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[11], (DWORD) pEvParams);

	DWORD l_dwErrCode = (DWORD) pEvParams;	// エラーコード保存

/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
	if (HEAD_TYPE_CTA == ((LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.wHeadType) {
		/* CTA ヘッドの場合のみ実施	*/
/* modified 2009.11.06 hmenjo CTA Abort 時は異常リセット ---------- { ---------- */
//		MEAS_CtaMeasEnd();
/* modified 2009.11.06 hmenjo CTA Abort 時は異常リセット ---------- 			 */
		MEAS_CtaForceEnd();
/* modified 2009.11.06 hmenjo CTA Abort 時は異常リセット ---------- } ---------- */
	}
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */

/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- { ---------- */
//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_ABRT);
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ----------			   */
	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_ABRT);}
/* modified 2009.08.03 hmenjo ストレス機能追加(14) ---------- } ---------- */

/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->SentMeasPause(0, TRUE);	// ポーズ解除通知
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
/* deleted 2009.10.01 hmenjo SE 排他対策(ランプ制御) ---------- { ---------- */
///* added 2009.08.26 hmenjo SE ランプ制御 ---------- { ---------- */
//	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* SE ランプフィルタ CLOSE	*/
///* added 2009.08.26 hmenjo SE ランプ制御 ---------- } ---------- */
/* deleted 2009.10.01 hmenjo SE 排他対策(ランプ制御) ---------- } ---------- */

// deleted hmenjo 2009.01.25 ---- { ----
//	// １枚測定終了を データ処理 へ通知(異常)
//	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_MEAS_END, 1, 0);
// deleted hmenjo 2009.01.25 ---- } ----

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_CANCELING);	// Status Bar
// deleted hmenjo 2009.01.25 ---- { ----
//	((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_MEAS_END with NG(or Cancel)."));
// deleted hmenjo 2009.01.25 ---- } ----

	m_bCancel = FALSE;

	// 各トランジションに停止指令を発行 ---------------------------------------
	BOOL l_bNeedStop = FALSE;
	//		各トランジションの状態を見て決めます．
	// リファレンス測定 トランジション
	if (false == ((CChiefTransiSrRefer*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer))->IsIdle()) {
		// リファレンス測定 トランジションが動作中
		l_bNeedStop = TRUE;
		// 終了させる
		((CChiefTransiSrRefer*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer))->CancelSrRefer();
	}
	// デスキュー トランジション
	if (false == ((CChiefTransiDeskew*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew))->IsIdle()) {
		// デスキュー トランジションが動作中
		l_bNeedStop = TRUE;
		// 終了させる
		// 各デスキューモジュールを終了させます
		((CChiefTransiDeskew*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew))->CancelDeskew();
	}
	// １ポイント測定 トランジション
	if (false == ((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->IsIdle()) {
		// １ポイント測定 トランジションが動作中
		l_bNeedStop = TRUE;
		// 終了させる
		((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->CancelSr1Point();
	}

	// 軸が動作中の場合はココで停止させる -------------------------------------
	// XY 軸を停止
#ifndef CHIEF_STAGE_ON
	if (0 == StageIsIdle()) {
#else
	if (0 == NS_IsIdleStageMoveAsync()) {
#endif
		StageStop();	// 戻り値は無視しています
	}
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 動作中フラグ(XY ステージ)をオフ
	// AF を停止
	StageAbortAutoFocus();	// 戻り値は無視しています
	// Z 軸を停止
//	StageElevatorStop();	// 戻り値は無視しています
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ

	if (0 != l_bNeedStop) {
		// 以下のすべてのトランジションがアイドルになるまで待ちます
		while (
			(false == ((CChiefTransiSrRefer*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer))->IsIdle())
		 || (false == ((CChiefTransiDeskew*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew))->IsIdle())
		 || (false == ((CChiefTransiSr1Point*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point))->IsIdle())
			) {
			::Sleep(1);
		}
	}

/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- { ---------- */
	m_bMeasSeq = FALSE;
/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- } ---------- */

/* added 2009.10.01 hmenjo SE 排他対策(ランプ制御) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* SE ランプフィルタ CLOSE	*/
/* added 2009.10.01 hmenjo SE 排他対策(ランプ制御) ---------- } ---------- */
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	if (HEAD_TYPE_COMPEASE == ((LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.wHeadType) {
		MEAS_CompEASEHead_CloseLampShutter();
	}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
/* added 2009.11.24 hmenjo 自動測定終了時は SR フィルタ OPEN ---------- { ---------- */
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/* added 2009.11.24 hmenjo 自動測定終了時は SR フィルタ OPEN ---------- } ---------- */
// added hmenjo 2009.01.25 ---- { ----
	// １枚測定終了を データ処理 へ通知(異常)
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_MEAS_END, 1, 0);
	((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_MEAS_END with NG(or Cancel)."));
// added hmenjo 2009.01.25 ---- } ----
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */

	// 時間待ち
	::Sleep(TIMER_DELAY_ABORT);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_CANCELED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// 測定キャンセル完了を Pif へ報告
	((CChiefView*) m_pcChiefView)->ReportCancelComplete(0);

	// トランジション更新
	TransiEvent(EV_SEQ_CANCEL_DONE);

	// 呼出元トランジションに完了を通知
/* modified 2009.08.03 hmenjo ストレス機能追加(16) ---------- { ---------- */
//	((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) l_dwErrCode);
/* modified 2009.08.03 hmenjo ストレス機能追加(16) ----------			   */
	NotifyComplete((cEventParams*) l_dwErrCode);
/* modified 2009.08.03 hmenjo ストレス機能追加(16) ---------- } ---------- */
}

/*
 *	ポーズ解除待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSeq::enter_ST_SEQ_PAUSE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[12], (DWORD) pEvParams);

	m_bPause = FALSE;

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_PAUSING);	// Status Bar
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
	if (0 != m_iDeskewing) {
		/*	デスキュー中にポーズイベントが発生したあとのポーズイベントなので，
			EV_SEQ_DESKEW_DONE が発生したと判断します．*/
		switch ((DWORD) pEvParams) {
		case TR_DSKW_OK:
		case TR_DSKW_AUTO_PROCEED:
		case TR_DSKW_MANU_PROCEED:
			// 正常完了
			m_iPauseNextState = ST_SEQ_DESKEW_DONE;
			m_iDeskewing = 2;	// デスキュートランジション正常完了
			break;
		default:
			// 異常完了
			m_iDeskewing = 0;
			// トランジション更新
			TransiEvent(EV_SEQ_CANCEL, pEvParams);
			break;
		}
	} else {
		if (false == ((CChiefTransiDeskew*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew))->IsIdle()) {
			// デスキュー中にポーズイベント発生しました．
			m_iDeskewing = 1;	// デスキュートランジション実行中
		}
		if (0 != pEvParams) {
			m_iPauseNextState = (int) pEvParams;
		}
	}
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */

	// WAIT 状態にするため処理はありません．
}

/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- { ---------- */
/*
 *	レジューム前処理(WAIT 状態無し)
 */
void CChiefTransiSeq::enter_ST_SEQ_PRE_RESUME(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[13], (DWORD) pEvParams);

	DWORD l_dwDeskewing = this->m_iDeskewing;
	this->m_iDeskewing = 0;

	int l_iEventNo;
	DWORD l_dwEvParam;
	switch (m_iPauseNextState) {
	case ST_SEQ_DESKEW:
		l_iEventNo = EV_SEQ_REFER_OK;
		l_dwEvParam = (1 == l_dwDeskewing)? 1 : 0;
		break;
	case ST_SEQ_DESKEW_DONE:
		l_iEventNo = EV_SEQ_DESKEW_DONE;
		l_dwEvParam = TR_DSKW_OK;
		break;
	case ST_SEQ_POINT_MOVE:
		l_iEventNo = EV_SEQ_1P_NEXT;
		l_dwEvParam = 0;
		break;
	default:
		l_iEventNo = EV_SEQ_REFER_OK;
		l_dwEvParam = 0;
		break;
	}

	this->TransiEvent(l_iEventNo, (cEventParams*) l_dwEvParam);
}
/* added 2009.06.10 hmenjo ポーズ機能拡張 ---------- } ---------- */
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- { ---------- */
/*
 *	SR ダークリファレンス測定完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSeq::enter_ST_SEQ_DREFER(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[14], (DWORD) pEvParams);

	// リファレンス測定指令 発行
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(8, 6)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiSeq::enter_ST_SEQ_DREFER() - SrRefer Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// SR リファレンス測定 トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_SEQ_CANCEL, (cEventParams*) TR_SEQ_TRSREF_BUSY);
	} else {
		((CChiefTransiSrRefer*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer))->TransiEvent(EV_SREF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_SEQ, 0), 0));
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	SR ダークリファレンス測定完了処理(WAIT 状態無し)
 */
void CChiefTransiSeq::enter_ST_SEQ_DREFER_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SEQ_DISP[15], (DWORD) pEvParams);

	if (((long) pEvParams) < 0) {
		// 異常完了
		// トランジション更新
		TransiEvent(EV_SEQ_CANCEL, pEvParams);
	} else {
		// 正常完了
		// トランジション更新
		TransiEvent(EV_SEQ_1P_ALLDONE);
	}
}
/* added 2009.09.11 hmenjo Seq で Refer モードでダーク実施 ---------- } ---------- */
