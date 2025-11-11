// ChifTransiStress.cpp : implementation file
//

#include "stdafx.h"
#include <Active.hxx>
#include "nanospec.h"
#include "ChifTransiStress.h"
#include "ChiefExports.h"
#include "ChiefView.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "ChifTransiSeq.h"
#include "ChifTransiMaster.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "..\\..\\INC\\NEXIF.HXX"
#include "..\\..\\INC\\NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	マクロ定義
 */
#define	STRS_HWS_TIME	1000	/* [ms] HWS モード Sleep 時間	*/


/*
 *	構造体定義
 */


/*
 *	ローカルグローバル定義
 */
UINT WM_CHIF_TRSTRS_SETEVENT = RegisterWindowMessage("WM_CHIF_TRSTRS_SETEVENT");	/* トランジションイベント発行用ウィンドウメッセージ	*/
UINT WM_CHIF_TRSTRS_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TRSTRS_HEARTBEAT");	/* HeartBeat ウィンドウメッセージ	*/


/* ##########################################################################
 * class CChiefTransiStress : ストレス トランジション
 * ########################################################################*/

/* =========================================================================//
 *	トランジションテーブル
 * =========================================================================*/
const Transition TransTable_Stress[] =
{
	//	OldState,				EventId,				NewState

	{	ST_STRS_INIT,			EV_STRS_DO_INIT,		ST_STRS_INIT			},	/* 初期化処理を実行(すぐ EV_AAF_INIT_DONE で)			*/
	{	ST_STRS_INIT,			EV_STRS_INIT_DONE,		ST_STRS_IDLE			},	/* 初期化完了してアイドルへ								*/

	{	ST_STRS_IDLE,			EV_STRS_START,			ST_STRS_START			},	/* リファレンス or 実測定 判定 へ						*/

	{	ST_STRS_START,			EV_STRS_MODE_MEAS,		ST_STRS_SRMEAS			},	/* 実測定モードで：SR 測定シーケンス開始 & 完了待ち へ	*/
	{	ST_STRS_START,			EV_STRS_MODE_REF,		ST_STRS_PRELDPOS		},	/* リファレンスモード：測定前 ロード Pos 完了待ち へ	*/
//Saiki 20110308 Add ----->
	{	ST_STRS_START,			EV_STRS_MODE_FIX_MEAS,	ST_STRS_PRELDPOS		},	/* 実測定モード(固定膜厚値)：測定前 ロード Pos 完了待ち へ	*/
//Saiki 20110308 Add <-----
	{	ST_STRS_START,			EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* 異常，キャンセルで，Abort 停止待ち へ				*/

	{	ST_STRS_SRMEAS,			EV_STRS_SRMEAS_DONE,	ST_STRS_SRMEAS_DONE		},	/* SR 測定シーケンス完了処理 へ							*/
	{	ST_STRS_SRMEAS,			EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* 異常，キャンセルで，Abort 停止待ち へ				*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- { ---------- */
	{	ST_STRS_SRMEAS,			EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* ポーズ解除待ち へ									*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- } ---------- */

	{	ST_STRS_SRMEAS_DONE,	EV_STRS_MODE_MEAS,		ST_STRS_PRELDPOS		},	/* 測定前 ロード Pos 完了待ち へ						*/
	{	ST_STRS_SRMEAS_DONE,	EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* 異常，キャンセルで，Abort 停止待ち へ				*/

	{	ST_STRS_PRELDPOS,		EV_STRS_LDPOS_DONE,		ST_STRS_PINALIGN		},	/* ピン アライメント待ち へ								*/
	{	ST_STRS_PRELDPOS,		EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* 異常，キャンセルで，Abort 停止待ち へ				*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- { ---------- */
	{	ST_STRS_PRELDPOS,		EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* ポーズ解除待ち へ									*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- } ---------- */

	{	ST_STRS_PINALIGN,		EV_STRS_PIN_DONE,		ST_STRS_LMVSTAPOS		},	/* ライン動作開始位置 移動 & 完了待ち へ				*/
	{	ST_STRS_PINALIGN,		EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* 異常，キャンセルで，Abort 停止待ち へ				*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- { ---------- */
	{	ST_STRS_PINALIGN,		EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* ポーズ解除待ち へ									*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- } ---------- */

	{	ST_STRS_LMVSTAPOS,		EV_STRS_LINEMOVE_DONE,	ST_STRS_LMVSTAPOS		},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_LMVSTAPOS,		EV_STRS_HEPA_STOP,		ST_STRS_LMVSTAPOS		},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_LMVSTAPOS,		EV_STRS_TO_LMENDPOS,	ST_STRS_LMVENDPOS		},	/* ライン動作終了位置 移動 & 完了待ち へ				*/
	{	ST_STRS_LMVSTAPOS,		EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* 異常，キャンセルで，Abort 停止待ち へ				*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- { ---------- */
	{	ST_STRS_LMVSTAPOS,		EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* ポーズ解除待ち へ									*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- } ---------- */

	{	ST_STRS_LMVENDPOS,		EV_STRS_LINEMOVE_DONE,	ST_STRS_LMVENDPOS		},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_LINEMEAS_DONE,	ST_STRS_LMVENDPOS		},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_LINEDATA_DONE,	ST_STRS_LMVENDPOS		},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_LINE_NEXT,		ST_STRS_LMVSTAPOS		},	/* 次の ライン動作開始位置 移動 & 完了待ち へ			*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_LINE_ALLDONE,	ST_STRS_COMPLDPOS		},	/* 完了後 ロード Pos 完了待ち へ						*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* 異常，キャンセルで，Abort 停止待ち へ				*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- { ---------- */
	{	ST_STRS_LMVENDPOS,		EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* ポーズ解除待ち へ									*/
/* added 2009.08.20 hmenjo ストレス機能追加(49) ---------- } ---------- */

	{	ST_STRS_COMPLDPOS,		EV_STRS_LDPOS_DONE,		ST_STRS_PINDOWNCOMP		},	/* 完了後 ピン ダウン & Complete 待ち へ				*/
// 2009.08.22 K.Matsuo -->
	{	ST_STRS_COMPLDPOS,		EV_STRS_COMPLETE,		ST_STRS_COMPLDPOS		},	/* 状態記憶のみでそのまま								*/
// 2009.08.22 K.Matsuo -->

	{	ST_STRS_PINDOWNCOMP,	EV_STRS_PIN_DONE,		ST_STRS_PINDOWNCOMP		},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_PINDOWNCOMP,	EV_STRS_COMPLETE,		ST_STRS_PINDOWNCOMP		},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_PINDOWNCOMP,	EV_STRS_PINCMP_DONE,	ST_STRS_COMPLETEDELAY	},	/* Complete 時間待ち へ									*/
/* added 2009.10.19 hmenjo Stress トランジションテーブル追加 ---------- { ---------- */
	{	ST_STRS_PINDOWNCOMP,	EV_STRS_CANCEL,			ST_STRS_PINDOWNCOMP		},	/* (ピン失敗だけど)状態記憶のみでそのまま				*/
/* added 2009.10.19 hmenjo Stress トランジションテーブル追加 ---------- } ---------- */

	{	ST_STRS_COMPLETEDELAY,	EV_STRS_COMPLETE_DONE,	ST_STRS_IDLE			},	/* アイドル へ											*/

	{	ST_STRS_WAITABORT,		EV_STRS_CANCEL_DONE,	ST_STRS_ABORTLDPOS		},	/* Abort 後 ロード Pos 完了待ち へ						*/

	{	ST_STRS_ABORTLDPOS,		EV_STRS_LDPOS_DONE,		ST_STRS_ABORTPINDOWN	},	/* Abort 後 ピン ダウン待ち へ							*/

	{	ST_STRS_ABORTPINDOWN,	EV_STRS_PIN_DONE,		ST_STRS_ABORTDELAY		},	/* Abort 時間待ち へ									*/
/* added 2009.10.19 hmenjo Stress トランジションテーブル追加 ---------- { ---------- */
	{	ST_STRS_ABORTPINDOWN,	EV_STRS_CANCEL,			ST_STRS_ABORTDELAY		},	/* (ピン失敗だけど)Abort 時間待ち へ					*/
/* added 2009.10.19 hmenjo Stress トランジションテーブル追加 ---------- } ---------- */

	{	ST_STRS_ABORTDELAY,		EV_STRS_ABORT_DONE,		ST_STRS_IDLE			},	/* アイドル へ											*/

	{	ST_STRS_PAUSE,			EV_STRS_RESUME,			ST_STRS_PRE_RESUME		},	/* レジューム前処理 へ									*/
	{	ST_STRS_PAUSE,			EV_STRS_SRMEAS_DONE,	ST_STRS_PAUSE			},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_PAUSE,			EV_STRS_LDPOS_DONE,		ST_STRS_PAUSE			},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_PAUSE,			EV_STRS_PIN_DONE,		ST_STRS_PAUSE			},	/* 状態記憶のみでそのまま								*/
	{	ST_STRS_PAUSE,			EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* 異常，キャンセルで，Abort 停止待ち へ				*/

	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_SRMEAS,		ST_STRS_SRMEAS			},	/* SR 測定シーケンス開始 & 完了待ち へ					*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_SRMEASED,	ST_STRS_SRMEAS_DONE		},	/* SR 測定シーケンス完了処理 へ							*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_PRELDPOS,	ST_STRS_PRELDPOS		},	/* 測定前 ロード Pos 完了待ち へ						*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_PINALIGN,	ST_STRS_PINALIGN		},	/* ピン アライメント待ち へ								*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_LMVSTAPOS,	ST_STRS_LMVSTAPOS		},	/* ライン動作開始位置 移動 & 完了待ち へ				*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_LMVENDPOS,	ST_STRS_LMVENDPOS		},	/* ライン動作終了位置 移動 & 完了待ち へ				*/

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};

/* =========================================================================//
 *	状態別処理関数テーブル
 * =========================================================================*/
BEGIN_STATEPROC_MAP(CChiefTransiStress)
	/*	0 */	ENTER_STATE(ST_STRS_INIT,			enter_ST_STRS_INIT),
	/*	1 */	ENTER_STATE(ST_STRS_IDLE,			enter_ST_STRS_IDLE),
	/*	2 */	ENTER_STATE(ST_STRS_START,			enter_ST_STRS_START),				/* (WAIT 状態無し)			*/
	/*	3 */	ENTER_STATE(ST_STRS_SRMEAS,			enter_ST_STRS_SRMEAS),				/* (WAIT 状態無しの場合有り)*/
	/*	4 */	ENTER_STATE(ST_STRS_SRMEAS_DONE,	enter_ST_STRS_SRMEAS_DONE),			/* (WAIT 状態無し)			*/
	/*	5 */	ENTER_STATE(ST_STRS_PRELDPOS,		enter_ST_STRS_PRELDPOS),			/* (WAIT 状態無しの場合有り)*/
	/*	6 */	ENTER_STATE(ST_STRS_PINALIGN,		enter_ST_STRS_PINALIGN),			/* (WAIT 状態無しの場合有り)*/
	/*	7 */	ENTER_STATE(ST_STRS_LMVSTAPOS,		enter_ST_STRS_LMVSTAPOS),			/* (WAIT 状態無しの場合有り)*/
	/*	8 */	ENTER_STATE(ST_STRS_LMVENDPOS,		enter_ST_STRS_LMVENDPOS),			/* (WAIT 状態無しの場合有り)*/
	/*	9 */	ENTER_STATE(ST_STRS_COMPLDPOS,		enter_ST_STRS_COMPLDPOS),			/* (WAIT 状態無しの場合有り)*/
	/*	10 */	ENTER_STATE(ST_STRS_PINDOWNCOMP,	enter_ST_STRS_PINDOWNCOMP),			/* (WAIT 状態無しの場合有り)*/
	/*	11 */	ENTER_STATE(ST_STRS_COMPLETEDELAY,	enter_ST_STRS_COMPLETEDELAY),		/* (WAIT 状態無し)			*/
	/*	12 */	ENTER_STATE(ST_STRS_WAITABORT,		enter_ST_STRS_WAITABORT),			/* (WAIT 状態無し)			*/
	/*	13 */	ENTER_STATE(ST_STRS_ABORTLDPOS,		enter_ST_STRS_ABORTLDPOS),			/* (WAIT 状態無しの場合有り)*/
	/*	14 */	ENTER_STATE(ST_STRS_ABORTPINDOWN,	enter_ST_STRS_ABORTPINDOWN),		/* (WAIT 状態無しの場合有り)*/
	/*	15 */	ENTER_STATE(ST_STRS_ABORTDELAY,		enter_ST_STRS_ABORTDELAY),			/* (WAIT 状態無し)			*/
	/*	16 */	ENTER_STATE(ST_STRS_PAUSE,			enter_ST_STRS_PAUSE),				/* (WAIT 状態無しの場合有り)*/
	/*	17 */	ENTER_STATE(ST_STRS_PRE_RESUME,		enter_ST_STRS_PRE_RESUME),			/* (WAIT 状態無し)			*/

END_STATEPROC_MAP(CChiefTransiStress)

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiStress

IMPLEMENT_DYNCREATE(CChiefTransiStress, CWinThread)

CChiefTransiStress::CChiefTransiStress(CWnd* pParent /*=NULL*/) : cActive(TransTable_Stress)
{
	TRACE(_T("CChiefTransiStress::CChiefTransiStress() \n"));

	m_pcChiefView = (CView*) pParent;

	/*	スレッドの起動待ち	*/
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiStress."));

	/*	オブジェクトの自動破棄を禁止	*/
	m_bAutoDelete = FALSE;

}

CChiefTransiStress::~CChiefTransiStress()
{
	TRACE(_T("CChiefTransiStress::~CChiefTransiStress() \n"));

	/*	スレッドの消滅待ち	*/
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiStress."));
		}
	}
}

BOOL CChiefTransiStress::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	TRACE(_T("CChiefTransiStress::InitInstance() \n"));

	/* コンストラクタの待ちを解除します
	 *		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．	*/
	m_cSyncEvent.SetEvent();

	/* 初期状態処理用イベント発行	*/
	TransiEvent(0, 0);

	/* 変数初期化	*/
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiStress::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here

	TRACE(_T("CChiefTransiStress::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiStress, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiStress)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRSTRS_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRSTRS_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiStress message handlers

/*
 *	トランジションイベント発行 メッセージハンドラ
 */
void CChiefTransiStress::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	/* トランジション間タイミング調整(とりあえずだけど恒久かも・・)	*/

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat メッセージハンドラ
 */
void CChiefTransiStress::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	トランジションイベント発行用関数
 */
void CChiefTransiStress::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
	switch (iEventNo) {
	case EV_STRS_CANCEL:
		m_bCancel = TRUE;
		/* ここでもあらかじめ ABORT にしておきます	*/
		if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_ABRT);}
		break;
	case EV_STRS_HEPA_STOP:
		m_bHepaStopped = TRUE;
		break;
	case EV_STRS_PAUSE:
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ---------- { ---------- */
//		{
//			int l_iCurrentState = this->GetCurrentState();
//			if (
//				(ST_STRS_PAUSE == l_iCurrentState)
//			 || (ST_STRS_PRE_RESUME == l_iCurrentState)
//			 || (TRUE == m_bPause)
//			 || (ST_STRS_LMVSTAPOS == l_iCurrentState)
//			 || (ST_STRS_LMVENDPOS == l_iCurrentState)
//				) {
//				return;
//			}
//			m_iPausePreState = l_iCurrentState;
//		}
//		m_bPause = TRUE;
//		if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
//			/* シーケンス測定 トランジションへポーズ指令	*/
//			((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_PAUSE);
//		}
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ----------			   */
		{
			int l_iCurrentState = this->GetCurrentState();
			switch (l_iCurrentState) {
			case ST_STRS_PAUSE:
			case ST_STRS_PRE_RESUME:
				return;
				break;
			default:
				if (TRUE == m_bPause) {
					return;
				}
				break;
			}
			m_iPausePreState = l_iCurrentState;
			m_bPause = TRUE;
			if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
				/* シーケンス測定 トランジションへポーズ指令	*/
				((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_PAUSE);
			}
			if (
				(ST_STRS_LMVSTAPOS == l_iCurrentState)
			 || (ST_STRS_LMVENDPOS == l_iCurrentState)
				) {
				/*	enter_ST_STRS_LMVSTAPOS()，enter_ST_STRS_LMVENDPOS() 内で，
				 *	直接，Event() を使いますので，WM_CHIF_TRSTRS_SETEVENT は発行しません．	*/
				return;
			}
		}
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ---------- } ---------- */
		break;
	}

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TRSTRS_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TRSTRS_SETEVENT, _T("WM_CHIF_TRSTRS_SETEVENT"), 9, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
}

/*
 *	HeartBeat 発行用関数
 */
void CChiefTransiStress::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TRSTRS_HEARTBEAT, 0, 0);
}

/*
 *	現在の状態が「アイドル」かどうかをチェック
 *		戻り値：	true	アイドルである
 *					false	アイドルでない
 */
bool CChiefTransiStress::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_STRS_IDLE) {
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
int CChiefTransiStress::GetCurrentState()
{
	return CurrState;
}

/*
 *	現在の測定ポイント番号を取得
 *		戻り値：	測定ポイント番号
 */
DWORD CChiefTransiStress::GetCurrentLineNo()
{
	return m_dwLineNo;
}

/*
 *	現在のポーズ状態を取得
 *		戻り値：	ポーズ状態
 */
bool CChiefTransiStress::IsPauseSeq()
{
	return (0 == m_bPause)? false : true;
}

/*
 *	現在，測定中かどうかを取得
 *		戻り値：	測定状態
 */
bool CChiefTransiStress::IsMeasuring()
{
	return (0 == m_bMeasStress)? false : true;
}

/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
bool CChiefTransiStress::IsPinAligningHWS()
{
	return (0 == m_bPinAligning)? false : true;
}
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */

/*
 *	ハンドラ ロードポジションへ移動
 */
BOOL CChiefTransiStress::MoveToLdPos()
{
	/* 座標を取得します．	*/
	STAGE_COORD l_SetPosition;
	ConfigFile_GetNanoSpecIni(&l_SetPosition, CONFIG_FILE_HANDLER_LOAD_POSITION);

/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	/* SR ヘッドにします．	*/
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */

	return NS_StageMoveAbsoluteEx(&l_SetPosition);
}

/*
 *	測定ポイント先行移動
 */
void CChiefTransiStress::PreMove()
{
	if (0 != m_bPause) {
		return;		/* ポーズ指令時は先行移動しません．	*/
	}

	if (2 == m_dwLineMoveToEndPos) {
		/* ライン動作終了位置へ移動済だった．	*/
		if (0 != ((CChiefView*) m_pcChiefView)->m_bPreMoveSW) {
			/* 先行移動設定がイネーブル	*/
			if (
				(ST_STRS_LMVENDPOS == CurrState)	/* ライン動作終了位置 移動 & 完了待ち	*/
				) {
				/* 先行移動が有効	*/
				STRESS_CONFIG l_StressConfig;
				ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
				DWORD l_dwLineNo = m_dwLineNo + 1;
				if (((CChiefView*) m_pcChiefView)->GetNextLineNo(&l_dwLineNo) <= l_StressConfig.dwLiftPinNumberOfLine) {
					/* 次の測定対象ラインがあった．	*/
					NS_RestoreStageSpeed();	/* 速度を戻します．	*/
					/*
					 *	ココでは，ピン位置と Z 軸位置はチェックしません．
					 */
					/* ライン座標を読出し	*/
					STAGE_COORD l_SetPosition;
					STRESS_CONFIG l_StressConfig;
					ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
					l_SetPosition.lX = l_StressConfig.lLineStartPosX;		/* ライン動作開始座標 X	*/
					LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgStress;
/* modified 2009.08.21 hmenjo ストレス機能追加(52) ---------- { ---------- */
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
////					l_SetPosition.lY = l_pStageProgStress->Line[l_dwLineNo - 1].LineSec.SectPos[0].lScanPosY;	/* ライン動作開始座標 Y	*/
////																					/*	セクション１の Y 座標を使います．	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------				*/
//					l_SetPosition.lY = l_pStageProgStress->Line[l_dwLineNo - 1].SectPos[0].lScanPosY;	/* ライン動作開始座標 Y	*/
//																					/*	セクション１の Y 座標を使います．	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
/* modified 2009.08.21 hmenjo ストレス機能追加(52) ----------			   */
					for (int i = 0; i < l_StressConfig.Line[l_dwLineNo - 1].dwSectionNum; i++) {
						if (0 != l_pStageProgStress->Line[l_dwLineNo - 1].bScanValid[i]) {
							break;
						}
					}
					if (l_StressConfig.Line[l_dwLineNo - 1].dwSectionNum <= i) {
						TCHAR l_tszLog[128];
						_stprintf(l_tszLog, _T("No Sections in Line No.%d."), l_dwLineNo);
						((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
						/* キャンセルをポスト	*/
						this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_NOSECTION);
					} else {
					l_SetPosition.lY = l_pStageProgStress->Line[l_dwLineNo - 1].SectPos[i].lScanPosY;	/* ライン動作開始座標 Y	*/
/* modified 2009.08.21 hmenjo ストレス機能追加(52) ---------- } ---------- */
					TCHAR l_tszLogText[256];
					_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d. (PreMove)"), l_dwLineNo);
					((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//					((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_STAPOS_PREMOVING);  // Status Bar
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
// 2009.08.27 K.Matsuo -->
					TCHAR l_szMainRcpName[256];
					((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// 2009.08.27 K.Matsuo <--
					if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
						/* HWS です．	*/
						::Sleep(STRS_HWS_TIME);
						m_dwLineMoveToStartPos = 1;	/* ライン動作開始位置：移動中	*/
						m_bPreMoving = TRUE;	/* 先行移動中	*/
						this->TransiEvent(EV_STRS_LINEMOVE_DONE, (cEventParams*) EV_STRS_LINEMOVE_DONE);
					} else
// 2009.08.27 K.Matsuo -->
//					if (0 == NS_StageMoveAbsoluteEx(&l_SetPosition)) {
					if (0 == NS_SampleCoordStageMoveAbsoluteEx(&l_SetPosition, l_szMainRcpName, HEAD_TYPE_STRESS)) {
// 2009.08.27 K.Matsuo <--
						/* 移動開始失敗	*/
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed_PreMotion));
						/* キャンセルをポスト	*/
						this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_LINEMOVE_FAIL);
					} else {
						/* 移動開始成功	*/
						m_dwLineMoveToStartPos = 1;	/* ライン動作開始位置：移動中	*/
						m_bPreMoving = TRUE;	/* 先行移動中	*/
						((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 73;	/* ライン動作 移動中	*/
						((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
					}
/* added 2009.08.21 hmenjo ストレス機能追加(52) ---------- { ---------- */
					}
/* added 2009.08.21 hmenjo ストレス機能追加(52) ---------- } ---------- */
				}
			}
		}
	}
}

/*--------------------------------------------------------------------------//
 *	状態別処理関数群
 *--------------------------------------------------------------------------*/
/*
 *	初期状態
 */
void CChiefTransiStress::enter_ST_STRS_INIT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[0], (DWORD) pEvParams);

	m_bPinAligning = FALSE;		/* ピンのアライメント位置中(HWS 用)	*//* added 2009.08.20 hmenjo ストレス機能追加(46) */
}

/*
 *	アイドル
 */
void CChiefTransiStress::enter_ST_STRS_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[1], (DWORD) pEvParams);

	m_bHepaStopped = FALSE;
	m_dwLineMoveToStartPos = 0;
	m_dwLineMoveToEndPos = 0;
	m_bCompleted = FALSE;
	m_iCancelStat = 0;
	m_bCancel = FALSE;
	m_bPreMoving = FALSE;
	m_bPause = FALSE;
	m_iPausePreState = 0;
	memset(&m_PauseStat, 0, sizeof(m_PauseStat));
	m_bMeasStress = FALSE;
	m_bPinAligning = FALSE;		/* ピンのアライメント位置中(HWS 用)	*//* added 2009.08.20 hmenjo ストレス機能追加(46) */

	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);}

	NS_RestoreStageSpeed();	/* 速度を戻します．	*/

	/* WAIT 状態にするため処理はありません．	*/
}

/*
 * リファレンス or 実測定 判定(WAIT 状態無し)
 */
void CChiefTransiStress::enter_ST_STRS_START(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOBYTE(LOWORD(pEvParams))：呼出元トランジション
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[2], (DWORD) pEvParams);

	/* 呼出し元トランジションを保存	*/
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//	m_dwParentTransi = LOWORD(pEvParams);
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
	m_dwParentTransi = LOBYTE(LOWORD(pEvParams));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
	/* 測定ライン番号を初期化	*/
	m_dwLineNo = 0;

	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);}

	/* HEPA 停止	*/
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stopping HEPA..."));
	char l_cHepaAlLevel;
	if (TRUE != ((CChiefView*) m_pcChiefView)->HepaOnOff(FALSE, &l_cHepaAlLevel)) {
		/* 停止異常でした．	*/
		AlarmIf_Set(ALID_HepaStopFailed);
		if ('W' != l_cHepaAlLevel) {
			/* アラームレベルが警告以外だったので終了します．	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to stop HEPA(alarm)."));
			/* トランジション更新	*/
			this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_HEPASTP_FAIL);
			return;
		} else {
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to stop HEPA(warning)."));
		}
	}

	/* ストレス シーケンス測定開始を画面モジュールへ通知	*/
	SEQ_MEAS_START l_SeqMeasStart;
	if ((0x0d == ((CChiefView*) m_pcChiefView)->m_szSampleID[0]) && (0x00 == ((CChiefView*) m_pcChiefView)->m_szSampleID[1])) {
		l_SeqMeasStart.bGetSamplID = FALSE;
	} else {
		l_SeqMeasStart.bGetSamplID = TRUE;
	}
	_tcscpy(l_SeqMeasStart.szSampleID, ((CChiefView*) m_pcChiefView)->m_szSampleID);
	COPYDATASTRUCT l_CopyData;
	l_CopyData.dwData = WMCD_CHIF_MEAS_START;
	l_CopyData.cbData = sizeof(SEQ_MEAS_START);
	l_CopyData.lpData = &l_SeqMeasStart;
	if (TRANSI_MASTER == m_dwParentTransi) {
/* added 2009.08.20 hmenjo ストレス機能追加(48) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WMCD_CHIF_MEAS_START to Data module."));
/* added 2009.08.20 hmenjo ストレス機能追加(48) ---------- } ---------- */
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);
	}

	/* サンプル ID を取得	*/
	TCHAR l_tszSampleID[SAMPLE_ID_LEN + 1];
	if (TRUE == l_SeqMeasStart.bGetSamplID) {
		_tcscpy(l_tszSampleID, ((CChiefView*) m_pcChiefView)->m_szSampleID);
	} else {
		//2009.08.12 bagus stress --{--
		//((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView))->GetSampleId(l_tszSampleID);
		((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetSampleId(l_tszSampleID);
		//2009.08.12 bagus stress --}--
	}
	/* 測定モード判定	*/
	/*		[サンプル ID].dat の有無を確認して判定します．	*/
	TCHAR l_RefFilePath[_MAX_PATH];
	_stprintf(l_RefFilePath, _T("%s%s.dat"), g_szData_StressRef_Dir, l_tszSampleID);
	m_iMeasMode = 1;	/* 実測定	*/
	if (-1 == ::GetFileAttributes(l_RefFilePath)) {
		m_iMeasMode = 0;	/* (ファイルが無かったので)リファレンス測定	*/
	}
	/* サンプル ID を測定モジュールへ通知	*/
	// 2009.08.18 K.Matsuo ストレス対応 -->
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo;
	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo;
// 2009.10.06 K.Matsuo -->
	LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
	LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgStress;
//Saiki 20110308 Change ----->
//	BOOL l_bRet = MEAS_PrepareForStressMeasure(m_iMeasMode, l_tszSampleID, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgStress);
//	BOOL l_bRet = MEAS_PrepareForStressMeasure(m_iMeasMode, l_tszSampleID, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgInfoHdr, l_pStageProgStress);
	LPSTRESS_PARAM_INFO l_pStressParamInfo = (LPSTRESS_PARAM_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStressParamInfo;
	BOOL l_bRet = MEAS_PrepareForStressMeasure(m_iMeasMode, l_tszSampleID, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgInfoHdr, l_pStageProgStress, l_pStressParamInfo);
//Saiki 20110308 Change <-----
// 2009.10.06 K.Matsuo <--
	// 2009.08.18 K.Matsuo ストレス対応 <--
	if (TRUE != l_bRet) {
		/* 測定モジュールから異常が返りました．	*/
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_MEASPRE_FAIL);
	} else {
		if (0 == m_iMeasMode) {
			((CChiefView*) m_pcChiefView)->LogChief(_T("Start reference mode."));
			this->TransiEvent(EV_STRS_MODE_REF);	/* リファレンス測定	*/
		} else {
//Saiki 20110308 Change ----->
//			((CChiefView*) m_pcChiefView)->LogChief(_T("Start stress mode."));
//			this->TransiEvent(EV_STRS_MODE_MEAS);	/* 実測定	*/
			if(TRUE == l_pStressParamInfo->bPriority){
				((CChiefView*) m_pcChiefView)->LogChief(_T("Start stress fixed mode."));
				this->TransiEvent(EV_STRS_MODE_FIX_MEAS);	/* 実測定(固定値)	*/
			}
			else{
				((CChiefView*) m_pcChiefView)->LogChief(_T("Start stress actual mode."));
				this->TransiEvent(EV_STRS_MODE_MEAS);	/* 実測定(実測値)	*/
			}
//Saiki 20110308 Change <-----
		}
	}
}

/*
 * SR 測定シーケンス開始 & 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_SRMEAS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[3], (DWORD) pEvParams);

	if (1 == m_PauseStat.dwSrMeasuring) {
		/* 測定中で，ポーズから復帰のため．*/
		m_PauseStat.dwSrMeasuring = 0;
		/* WAIT 状態にするためトランジションを更新しません．	*/
		return;
	}

	/* SR シーケンス測定 開始	*/
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(6, 9)) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		/* SEQ トランジションがビジーでした．	*/
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->LogChief(_T("SR Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiStress::enter_ST_STRS_SRMEAS() - SR Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		/* トランジション更新	*/
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_TRSEQ_BUSY);
	} else {
		/* シーケンス測定 発行	*/
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(TRANSI_STRESS_SEQ, pEvParams));
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_STRESS_SEQ, 0), pEvParams));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
		/* WAIT 状態にするためトランジションを更新しません．	*/
	}
}

/*
 * SR 測定シーケンス完了処理(WAIT 状態無し)
 */
void CChiefTransiStress::enter_ST_STRS_SRMEAS_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[4], (DWORD) pEvParams);

	/* SR シーケンス測定 結果確認	*/
	if (TR_SEQ_OK != (DWORD) pEvParams) {
		/* 異常終了なのでキャンセル扱いにします．	*/
		((CChiefView*) m_pcChiefView)->LogChief(_T("SR seq is abnormal end."));
		/* トランジション更新	*/
		this->TransiEvent(EV_STRS_CANCEL, pEvParams);
	} else if (0 != m_bCancel) {
		/* キャンセル指令	*/
		this->TransiEvent(EV_STRS_CANCEL);
	} else {
		/* トランジション更新	*/
		this->TransiEvent(EV_STRS_MODE_MEAS);
	}
}

/*
 * 測定前 ロード Pos 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_PRELDPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[5], (DWORD) pEvParams);

	if (1 == m_PauseStat.dwPreLoadingPos) {
		/* 移動中で，ポーズから復帰のため．*/
		m_PauseStat.dwPreLoadingPos = 0;
		/* WAIT 状態にするためトランジションを更新しません．	*/
/* added 2009.08.21 hmenjo ストレス機能追加(53) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
/* added 2009.08.21 hmenjo ストレス機能追加(53) ---------- } ---------- */
		return;
	}

	/* ロード Pos へ移動 -------------------------------------- */
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to LD Pos. (before-meas)"));
	if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
		/* HWS です．	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
		::Sleep(STRS_HWS_TIME);
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else
	if (0 == this->MoveToLdPos()) {
		/* 移動開始失敗	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LoadPositionMoveStartFailed));
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to LD Pos. (before-meas)"));
		/* キャンセルをポスト	*/
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_HLDPOSMOVE_FAIL);
	} else {
		/* 移動開始成功	*/
		((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 72;	/* ハンドラ ロード ポジション 移動中	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
		/* WAIT 状態にするため処理はありません．	*/
	}
}

/*
 * ピン アライメント待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_PINALIGN(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[6], (DWORD) pEvParams);

/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	/* Stress ヘッドにします．	*/
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_STRESS, FALSE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */

	if (1 == m_PauseStat.dwPinAligning) {
		/* 動作中で，ポーズから復帰のため．*/
		m_PauseStat.dwPinAligning = 0;
		/* WAIT 状態にするためトランジションを更新しません．	*/
/* added 2009.08.21 hmenjo ストレス機能追加(53) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_LIFT_ALIGN);	// Status Bar
/* added 2009.08.21 hmenjo ストレス機能追加(53) ---------- } ---------- */
		return;
	}

	/* Z 軸を干渉外位置へ移動	*/
	((CChiefView*) m_pcChiefView)->LogChief(_T("Retracting Z axis..."));
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		/* 動作中フラグ(Z 軸)をオン	*/
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_Z_MOVING);	// Status Bar
/* modified 2009.09.29 hmenjo Stress Z 軸スキャン位置 ---------- { ---------- */
//	if ((0 == StageElevatorMove(Z_RETRACT_POS)) && (0 == ((CChiefView*) m_pcChiefView)->IsHWS())) {
/* modified 2009.09.29 hmenjo Stress Z 軸スキャン位置 ----------			  */
	STRESS_CONFIG l_StressConfig;
	ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	if ((0 == StageElevatorMove(l_StressConfig.lScanZAxisPosition)) && (0 == ((CChiefView*) m_pcChiefView)->IsHWS())) {
/* modified 2009.09.29 hmenjo Stress Z 軸スキャン位置 ---------- } ---------- */
		/* 移動開始失敗	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		/* 動作中フラグ(Z 軸)をオフ	*/
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to Z retract position."));
		/* キャンセルをポスト	*/
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_ZMOVE_FAIL);
	} else {
		if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
			/* HWS です．	*/
			::Sleep(STRS_HWS_TIME);
		}
		/* 移動完了	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		/* 動作中フラグ(Z 軸)をオフ	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_Z_MOVED);	// Status Bar
		((CChiefView*) m_pcChiefView)->LogChief(_T("Moved to Z retract position."));
		/* ピンをアライメント位置へ移動 ----------------------------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_LIFT_ALIGN);	// Status Bar
		((CChiefView*) m_pcChiefView)->LogChief(_T("Moving Lifter to alignment pos..."));
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bLoadPos) {
			/* ロードポジションではなかった．	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Not LD position."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
			/* キャンセルをポスト	*/
			this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_NO_HLDPOS);
		} else {
			BOOL l_bNexResult;
			if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
				/* HWS です．	*/
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
				m_bPinAligning = TRUE;
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
				::Sleep(STRS_HWS_TIME);
				this->TransiEvent(EV_STRS_PIN_DONE);
			} else
			if (0 == (l_bNexResult = nexifMoveToAlignment(m_pcChiefView->m_hWnd))) {
				/* 失敗	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move Lifter to alignment pos."));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
				/* キャンセルをポスト	*/
				this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_PINMOVE_FAIL);
			} else {
				/* 成功	*/
				((CChiefView*) m_pcChiefView)->m_dwPinMoveState = 71;	/* ピン移動中	*/
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_PIN, TRUE);
				/* WAIT 状態にするため処理はありません．	*/
			}
		}
	}
}

/*
 * ライン動作開始位置 移動 & 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_LMVSTAPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[7], (DWORD) pEvParams);

	BOOL l_bCancelReq = FALSE;
	DWORD l_dwErrCode = TR_STRS_OK;

	m_bMeasStress = TRUE;	/* ストレス測定中	*/

	/* ライン動作移動完了 イベント チェック	*/
	if ((1 == m_dwLineMoveToStartPos) && (EV_STRS_LINEMOVE_DONE == LOWORD(pEvParams))) {
		m_dwLineMoveToStartPos = 2;		/* 移動済	*/
		if (TRUE == m_bPreMoving) {
			m_bPreMoving = FALSE;
			/* ライン番号を更新	*/
			m_dwLineNo++;
			((CChiefView*) m_pcChiefView)->GetNextLineNo(&m_dwLineNo);
		}
	}

	if (0 == m_dwLineMoveToStartPos) {
		if (TRUE == m_bPause) {
			/* ポーズ指令有った．	*/
			m_iPausePreState = ST_STRS_LMVSTAPOS;
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ---------- { ---------- */
//			this->TransiEvent(EV_STRS_PAUSE);
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ----------			   */
			Event(EV_STRS_PAUSE);
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ---------- } ---------- */
			return;
		}
		/* ライン動作開始位置へ移動	*/
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//		if (0 == nexioIsPinAlignmentPos()) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bPinAlign) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
			/* ピンがアライメント位置でなかった．	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Not Lifter is alignment position."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterLowestPositionSensorOff));
			/* キャンセルをポスト	*/
			l_bCancelReq = TRUE;
			l_dwErrCode = TR_STRS_NO_PIN_ALIGN;
		} else if (0 == nexioIsRetractPosZ()) {
			/* ストレス用 Z 軸位置でなかった．	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Z retract position is Invalid."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_PositionRetractFailed));
			/* キャンセルをポスト	*/
			l_bCancelReq = TRUE;
			l_dwErrCode = TR_STRS_NO_ZPOS;
		} else {
			NS_RestoreStageSpeed();	/* 速度を戻します．	*/
			/* ライン番号を更新	*/
			m_dwLineNo++;
			((CChiefView*) m_pcChiefView)->GetNextLineNo(&m_dwLineNo);
			/* ライン座標を読出し	*/
			STAGE_COORD l_SetPosition;
			STRESS_CONFIG l_StressConfig;
			ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
			l_SetPosition.lX = l_StressConfig.lLineStartPosX;		/* ライン動作開始座標 X	*/
			LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgStress;
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ---------- { ---------- */
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
////			l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].LineSec.SectPos[0].lScanPosY;	/* ライン動作開始座標 Y	*/
////																			/*	セクション１の Y 座標を使います．	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------				*/
//			l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].SectPos[0].lScanPosY;	/* ライン動作開始座標 Y	*/
//																			/*	セクション１の Y 座標を使います．	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ----------			   */
			for (int i = 0; i < l_StressConfig.Line[m_dwLineNo - 1].dwSectionNum; i++) {
				if (0 != l_pStageProgStress->Line[m_dwLineNo - 1].bScanValid[i]) {
					break;
				}
			}
			if (l_StressConfig.Line[m_dwLineNo - 1].dwSectionNum <= i) {
				TCHAR l_tszLog[128];
				_stprintf(l_tszLog, _T("No Sections in Line No.%d."), m_dwLineNo);
				((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
				/* キャンセルをポスト	*/
				l_bCancelReq = TRUE;
				l_dwErrCode = TR_STRS_NOSECTION;
			} else {
			l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].SectPos[i].lScanPosY;	/* ライン動作開始座標 Y	*/
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ---------- } ---------- */
			TCHAR l_tszLogText[256];
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ---------- { ---------- */
//			_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d start position."), m_dwLineNo);
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ----------			   */
			_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d start position. [%10d, %10d]"), m_dwLineNo, l_SetPosition.lX, l_SetPosition.lY);
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ---------- } ---------- */
			((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
			TCHAR l_szMainRcpName[256];
			((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
			if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
				/* HWS です．	*/
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
				CString l_strStatBar; l_strStatBar.Format(IDS_CHIF_STRS_STAPOS_MOVING, m_dwLineNo);
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(l_strStatBar);  // Status Bar
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
				::Sleep(STRS_HWS_TIME);
				m_dwLineMoveToStartPos = 1;		/* 移動中	*/
				this->TransiEvent(EV_STRS_LINEMOVE_DONE, (cEventParams*) EV_STRS_LINEMOVE_DONE);
			} else
			// 2009.08.27 K.Matsuo -->
			if (0 == NS_SampleCoordStageMoveAbsoluteEx(&l_SetPosition, l_szMainRcpName, HEAD_TYPE_STRESS)) {
			// 2009.08.27 K.Matsuo <--
				/* 移動開始失敗	*/
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to line start position."));
				/* キャンセルをポスト	*/
				l_bCancelReq = TRUE;
				l_dwErrCode = TR_STRS_LINEMOVE_FAIL;
			} else {
				/* 移動開始成功	*/
				m_dwLineMoveToStartPos = 1;		/* 移動中	*/
				((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 73;	/* ライン動作 移動中	*/
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_STAPOS_MOVING);	// Status Bar
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
				CString l_strStatBar; l_strStatBar.Format(IDS_CHIF_STRS_STAPOS_MOVING, m_dwLineNo);
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(l_strStatBar);  // Status Bar
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
				/* WAIT 状態にするため処理はありません．	*/
			}
/* added 2009.08.21 hmenjo ストレス機能追加(51) ---------- { ---------- */
			}
/* added 2009.08.21 hmenjo ストレス機能追加(51) ---------- } ---------- */
		}
	} else if ((0 != m_bHepaStopped) && (2 == m_dwLineMoveToStartPos)) {
		/* HEPA 停止済 ＆ 移動済	*/
		m_dwLineMoveToStartPos = 0;
		/* トランジション更新	*/
		this->TransiEvent(EV_STRS_TO_LMENDPOS);
	} else {
		/* WAIT 状態にするため処理はありません．	*/
	}

	/* キャンセル処理(あれば・・・)	*/
	if (TRUE == l_bCancelReq) {
		m_dwLineMoveToStartPos = 0;
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) l_dwErrCode);
	}
}

/*
 * ライン動作終了位置 移動 & 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_LMVENDPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[8], (DWORD) pEvParams);

	BOOL l_bCancelReq = FALSE;
	DWORD l_dwErrCode = TR_STRS_OK;

	static BOOL ls_bMeasEnd = FALSE;	/* １ライン測定完了(測定モジュールより)	*/
	static BOOL ls_AnaEndReply = FALSE;		/* １ラインデータ処理完了応答(データ処理モジュールから)	*/

	/* イベント チェック	*/
	switch (LOWORD(pEvParams)) {
	case EV_STRS_LINEMOVE_DONE:		/* ライン動作移動完了	*/
		if (1 == m_dwLineMoveToEndPos) {
			m_dwLineMoveToEndPos = 2;		/* 移動済	*/
		}
/* added 2009.08.21 hmenjo ストレス機能追加(53) ---------- { ---------- */
		if ((1 == m_dwLineMoveToStartPos) && (TRUE == m_bPreMoving)) {
			/* 先行移動の完了	*/
			m_dwLineMoveToStartPos = 2;
		}
/* added 2009.08.21 hmenjo ストレス機能追加(53) ---------- } ---------- */
		break;
	case EV_STRS_LINEMEAS_DONE:		/* １ライン測定完了	*/
		ls_bMeasEnd = TRUE;
		if (0 != HIWORD(pEvParams)) {
			/* 異常通知でした．	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Measurement was failure.."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementError));
			l_bCancelReq = TRUE;
			l_dwErrCode = TR_STRS_MEASEND_ERR;
		} else {
			/* 「データ処理モジュール」へ１ライン測定完了を通知します．	*/
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_CHIF_LINE_END, m_dwLineNo, m_iMeasMode);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_LINE_END to Data module."));
		}
		break;
	case EV_STRS_LINEDATA_DONE:		/* １ラインデータ処理完了応答	*/
		ls_AnaEndReply = TRUE;
		break;
	default:
		;	/* 何もしません．	*/
		break;
	}

	if (TRUE != l_bCancelReq) {
		if (0 == m_dwLineMoveToEndPos) {
			if (TRUE == m_bPause) {
				/* ポーズ指令有った．	*/
				m_iPausePreState = ST_STRS_LMVENDPOS;
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ---------- { ---------- */
//				this->TransiEvent(EV_STRS_PAUSE);
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ----------			   */
				Event(EV_STRS_PAUSE);
/* modified 2009.08.20 hmenjo ストレス機能追加(49) ---------- } ---------- */
				return;
			}
			/* ライン動作開始位置へ移動	*/
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//			if (0 == nexioIsPinAlignmentPos()) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
			if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bPinAlign) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
				/* ピンがアライメント位置でなかった．	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Not Lifter is alignment position."));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterLowestPositionSensorOff));
				/* キャンセルをポスト	*/
				l_bCancelReq = TRUE;
				l_dwErrCode = TR_STRS_NO_PIN_ALIGN;
			} else if (0 == nexioIsRetractPosZ()) {
				/* ストレス用 Z 軸位置でなかった．	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Z retract position is Invalid."));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_PositionRetractFailed));
				/* キャンセルをポスト	*/
				l_bCancelReq = TRUE;
				l_dwErrCode = TR_STRS_NO_ZPOS;
			} else {
				/* 「データ処理モジュール」へライン動作開始を通知します．	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Start Measurement."));
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_CHIF_LINE_START, m_dwLineNo, m_iMeasMode);
				/* 「測定モジュール」へライン動作開始を通知します．	*/
				BOOL l_bRet = MEAS_StressMeasure(m_dwLineNo);
				if (TRUE != l_bRet) {
					/* 「測定モジュール」開始エラーでした．	*/
					((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to run MEAS_StressMeasure()."));
					((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementError));
					/* キャンセルをポスト	*/
					l_bCancelReq = TRUE;
					l_dwErrCode = TR_STRS_MEASSTA_FAIL;
				} else {
					/* ライン座標を読出し	*/
					STAGE_COORD l_SetPosition;
					STRESS_CONFIG l_StressConfig;
					ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
					l_SetPosition.lX = l_StressConfig.lLineEndPosX;		/* ライン動作終了座標 X	*/
					/* 測定速度 設定 [mm/s]=[um/ms]	*/
					NS_SetStageSpeed(l_StressConfig.dwScanSpeed);
					LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgStress;
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ---------- { ---------- */
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
////					l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].LineSec.SectPos[0].lScanPosY;	/* ライン動作開始座標 Y	*/
////																					/*	セクション１の Y 座標を使います．	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------				*/
//					l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].SectPos[0].lScanPosY;	/* ライン動作開始座標 Y	*/
//																					/*	セクション１の Y 座標を使います．	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ----------			   */
					for (int i = 0; i < l_StressConfig.Line[m_dwLineNo - 1].dwSectionNum; i++) {
						if (0 != l_pStageProgStress->Line[m_dwLineNo - 1].bScanValid[i]) {
							break;
						}
					}
					if (l_StressConfig.Line[m_dwLineNo - 1].dwSectionNum <= i) {
						TCHAR l_tszLog[128];
						_stprintf(l_tszLog, _T("No Sections in Line No.%d."), m_dwLineNo);
						((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
						/* キャンセルをポスト	*/
						l_bCancelReq = TRUE;
						l_dwErrCode = TR_STRS_NOSECTION;
					} else {
					l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].SectPos[i].lScanPosY;	/* ライン動作開始座標 Y	*/
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ---------- } ---------- */
					TCHAR l_tszLogText[256];
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ---------- { ---------- */
//					_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d end position."), m_dwLineNo);
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ----------			   */
					_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d end position. [%10d, %10d]"), m_dwLineNo, l_SetPosition.lX, l_SetPosition.lY);
/* modified 2009.08.21 hmenjo ストレス機能追加(51) ---------- } ---------- */
					((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
					TCHAR l_szMainRcpName[256];
					((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
					if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
						/* HWS です．	*/
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
						CString l_strStatBar; l_strStatBar.Format(IDS_CHIF_STRS_ENDPOS_MOVING, m_dwLineNo);
						((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(l_strStatBar);  // Status Bar
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
						::Sleep(STRS_HWS_TIME);
						m_dwLineMoveToEndPos = 1;		/* 移動中	*/
						this->TransiEvent(EV_STRS_LINEMOVE_DONE, (cEventParams*) EV_STRS_LINEMOVE_DONE);
					} else
					// 2009.08.27 K.Matsuo -->
					if (0 == NS_SampleCoordStageMoveAbsoluteEx(&l_SetPosition, l_szMainRcpName, HEAD_TYPE_STRESS)) {
					// 2009.08.27 K.Matsuo <--
						/* 移動開始失敗	*/
						((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
						((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to line end position."));
						/* キャンセルをポスト	*/
						l_bCancelReq = TRUE;
						l_dwErrCode = TR_STRS_LINEMOVE_FAIL;
					} else {
						/* 移動開始成功	*/
						m_dwLineMoveToEndPos = 1;		/* 移動中	*/
						((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 73;	/* ライン動作 移動中	*/
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//						((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_ENDPOS_MOVING);	// Status Bar
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
						CString l_strStatBar; l_strStatBar.Format(IDS_CHIF_STRS_ENDPOS_MOVING, m_dwLineNo);
						((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(l_strStatBar);  // Status Bar
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
						((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
						/* WAIT 状態にするため処理はありません．	*/
					}
/* added 2009.08.21 hmenjo ストレス機能追加(51) ---------- { ---------- */
					}
/* added 2009.08.21 hmenjo ストレス機能追加(51) ---------- } ---------- */
				}
			}
		} else if ((2 == m_dwLineMoveToEndPos) && (TRUE == ls_bMeasEnd) && (TRUE == ls_AnaEndReply)) {
			/* 移動済 ＆ 測定済 ＆ データ処理済	*/
			m_dwLineMoveToEndPos = 0;
			ls_bMeasEnd = FALSE;
			ls_AnaEndReply = FALSE;
			STRESS_CONFIG l_StressConfig;
			ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
			DWORD l_dwLineNo = m_dwLineNo + 1;
			if ((l_StressConfig.dwLiftPinNumberOfLine <= m_dwLineNo)
			 || (l_StressConfig.dwLiftPinNumberOfLine < ((CChiefView*) m_pcChiefView)->GetNextLineNo(&l_dwLineNo))) {
				/* 最終ライン番号だったので全終了	*/
				/* トランジション更新	*/
				NS_RestoreStageSpeed();	/* 速度を戻します．	*/
				this->TransiEvent(EV_STRS_LINE_ALLDONE);
			} else {
//				/* ポーズチェック	*/
//				if (0) {	/* ポーズ系のデバッグ終了後，本当に不要なら削除すること．	*/
//					/**/
//				} else {
					/* 次のラインへ	*/
/* added 2009.08.21 hmenjo ストレス機能追加(53) ---------- { ---------- */
					if ((2 == m_dwLineMoveToStartPos) && (TRUE == m_bPreMoving)) {
						m_bPreMoving = FALSE;
						/* ライン番号を更新	*/
						m_dwLineNo++;
						((CChiefView*) m_pcChiefView)->GetNextLineNo(&m_dwLineNo);
					}
/* added 2009.08.21 hmenjo ストレス機能追加(53) ---------- } ---------- */
					/* トランジション更新	*/
					this->TransiEvent(EV_STRS_LINE_NEXT);
//				}
			}
		} else {
			/* WAIT 状態にするため処理はありません．	*/
		}
	}

	/* キャンセル処理(あれば・・・)	*/
	if (TRUE == l_bCancelReq) {
		m_dwLineMoveToEndPos = 0;
		ls_bMeasEnd = FALSE;
		ls_AnaEndReply = FALSE;
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) l_dwErrCode);
	}
}

/*
 * 完了後 ロード Pos 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_COMPLDPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[9], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->SentMeasPause(0, TRUE);	// ポーズ解除通知

	m_bMeasStress = FALSE;	/* ストレス測定中 解除	*/

// 2009.08.22 K.Matsuo -->
	/* イベントチェック	*/
	switch (LOWORD(pEvParams)) {
	case EV_STRS_COMPLETE:	/* １枚測定完了 応答	*/
		m_bCompleted = TRUE;
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_DATA_PROC_COMPLETE);	// Status Bar
//		break;
		return;
	default:
		;	/* 何もしません．	*/
		break;
	}
// 2009.08.22 K.Matsuo <--

	if (TRUE != m_bCompleted) {
		/* 「データ処理モジュール」へ１枚測定完了を通知	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_CHIF_STRS_MEAS_END, 0, m_iMeasMode);
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_DATA_PROCING);  // Status Bar
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_STRS_MEAS_END to Data module."));
	}

	/* ロード Pos へ移動 -------------------------------------- */
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to LD Pos. (after-meas)"));
	if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
		/* HWS です．	*/
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
		::Sleep(STRS_HWS_TIME);
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else
	if (0 == this->MoveToLdPos()) {
		/* 移動開始失敗	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LoadPositionMoveStartFailed));
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to LD Pos. (after-meas)"));
		/* 次のトランジションへ(ロード Pos へ移動せずに)	*/
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else {
		/* 移動開始成功	*/
		((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 72;	/* ハンドラ ロード ポジション 移動中	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
		/* WAIT 状態にするため処理はありません．	*/
	}
}

/*
 * 完了後 ピン ダウン & Complete 待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_PINDOWNCOMP(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[10], (DWORD) pEvParams);

	static BOOL ls_bPinDown = FALSE;	/* ピンダウン	*/

	/* イベントチェック	*/
	switch (LOWORD(pEvParams)) {
	case EV_STRS_COMPLETE:	/* １枚測定完了 応答	*/
		m_bCompleted = TRUE;
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_DATA_PROC_COMPLETE);	// Status Bar
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
		break;
	case EV_STRS_PIN_DONE:	/* ピンダウン完了	*/
/* added 2009.10.19 hmenjo Stress トランジションテーブル追加 ---------- { ---------- */
	case TR_STRS_PINMOVE_FAIL:	/* ピン失敗	*/
/* added 2009.10.19 hmenjo Stress トランジションテーブル追加 ---------- } ---------- */
		ls_bPinDown = TRUE;
		break;
	default:
		;	/* 何もしません．	*/
		break;
	}

	if ((TRUE == ls_bPinDown) && (TRUE == m_bCompleted)) {
		/* ピンダウン ＆ １枚測定完了 応答	*/
		m_bCompleted = FALSE;
		ls_bPinDown = FALSE;
		/* トランジション更新	*/
		this->TransiEvent(EV_STRS_PINCMP_DONE);
	} else if (TRUE != ls_bPinDown) {
		/* ピンダウンでないので，ピンを下降端へ移動	*/
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bLoadPos) {
			/* ロードポジションではなかった．	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Not LD position."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
			/* ピンダウン出来ないので，次のトランジションへ(ピンダウンせずに)	*/
			this->TransiEvent(EV_STRS_PINCMP_DONE);
		} else {
			BOOL l_bNexResult;
			if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
				/* HWS です．	*/
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_LIFT_LOWER);  // Status Bar
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
				::Sleep(STRS_HWS_TIME);
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
				m_bPinAligning = FALSE;
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
				this->TransiEvent(EV_STRS_PINCMP_DONE);
			} else
/* added 2009.10.19 hmenjo 下降端ではピンダウンしない ---------- { ---------- */
			if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bPinDown) {
				/* ピンがすでに下降端なので次のトランジションへ	*/
				this->TransiEvent(EV_STRS_PINCMP_DONE);
			} else
/* added 2009.10.19 hmenjo 下降端ではピンダウンしない ---------- { ---------- */
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- { ---------- */
//			if (0 == (l_bNexResult = nexifMoveToLower(m_pcChiefView->m_hWnd))) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ----------			  */
			if (0 == (l_bNexResult = nexifLoad(m_pcChiefView->m_hWnd))) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- } ---------- */
				/* 失敗	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move Lifter to lower pos."));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
				/* 次のトランジションへ(ピンダウンせずに)	*/
				this->TransiEvent(EV_STRS_PINCMP_DONE);
			} else {
				/* 成功	*/
				((CChiefView*) m_pcChiefView)->m_dwPinMoveState = 71;	/* ピン移動中	*/
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_PIN, TRUE);
				/* WAIT 状態にするため処理はありません．	*/
			}
		}
	} else {
		/* WAIT 状態にするため処理はありません．	*/
	}
}

/*
 * Complete 時間待ち(WAIT 状態無し)
 */
void CChiefTransiStress::enter_ST_STRS_COMPLETEDELAY(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[11], (DWORD) pEvParams);

/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	/* SR ヘッドに戻します．	*/
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */

	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_COMP);}

	/* HEPA 起動	*/
	((CChiefView*) m_pcChiefView)->HepaOnOff(TRUE);

	/* 時間待ち	*/
	::Sleep(TIMER_DELAY_COMPLETE);

/* added 2009.08.18 hmenjo ストレス機能追加(46) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
/* added 2009.08.18 hmenjo ストレス機能追加(46) ---------- } ---------- */

	/* トランジション更新	*/
	TransiEvent(EV_STRS_COMPLETE_DONE);

	/* 呼出元トランジションに完了を通知	*/
	((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_SEQ_DONE, TR_STRS_OK);
}

/*
 * Abort 停止待ち(WAIT 状態無し)
 */
void CChiefTransiStress::enter_ST_STRS_WAITABORT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[12], (DWORD) pEvParams);

	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_ABRT);}

	m_iCancelStat = (DWORD) pEvParams;

	NS_RestoreStageSpeed();	/* 速度を戻します．	*/

	m_bCancel = FALSE;

	/* ポーズ解除通知	*/
	((CChiefView*) m_pcChiefView)->SentMeasPause(0, TRUE);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_CANCELING);	// Status Bar

	/* 測定モジュールにキャンセルを通知	*/
	((CChiefView*) m_pcChiefView)->LogChief(_T("Cancel Measurement."));
	MEAS_StressCancel();

	/* シーケンス測定 トランジションを停止	*/
	BOOL l_bNeedStop = FALSE;
/* modified 2009.08.18 hmenjo ストレス機能追加(46) ---------- { ---------- */
//	if (false == ((CChiefTransiStress*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress))->IsIdle()) {
//		/* １ポイント測定 トランジションが動作中	*/
//		l_bNeedStop = TRUE;
//		/* 終了させる	*/
//		((CChiefTransiStress*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress))->TransiEvent(EV_STRS_CANCEL);
//	}
/* modified 2009.08.18 hmenjo ストレス機能追加(46) ----------			   */
	if (false == ((CChiefTransiSeq*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq))->IsIdle()) {
		/* SR シーケンス測定 トランジションが動作中	*/
		l_bNeedStop = TRUE;
		/* 終了させる	*/
		((CChiefTransiSeq*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq))->TransiEvent(EV_SEQ_CANCEL);
	}
/* modified 2009.08.18 hmenjo ストレス機能追加(46) ---------- } ---------- */

	/* 軸が動作中の場合はココで停止させます．	*/
	if (0 == NS_IsIdleStageMoveAsync()) {
		l_bNeedStop = TRUE;	/*hdebdeb20090824*/
		StageStop();	/* 戻り値は無視しています．	*/
	}

	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/

	if (0 != l_bNeedStop) {
		/* 以下のすべてのトランジションがアイドルになるまで待ちます．	*/
		while (
/* modified 2009.08.18 hmenjo ストレス機能追加(46) ---------- { ---------- */
//			(false == ((CChiefTransiStress*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress))->IsIdle())
/* modified 2009.08.18 hmenjo ストレス機能追加(46) ----------			   */
			(false == ((CChiefTransiSeq*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq))->IsIdle())
		 || (0 == NS_IsIdleStageMoveAsync())	/*hdebdeb20090824*/
/* modified 2009.08.18 hmenjo ストレス機能追加(46) ---------- } ---------- */
			) {
			::Sleep(1);
		}
	}

	m_bMeasStress = FALSE;	/* ストレス測定中 解除	*/

	::Sleep(2000);	/* ステージ停止待ちを待ってみる	*/

	/* １枚測定終了を データ処理 へ通知(異常)	*/
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_CHIF_STRS_MEAS_END, 1, m_iMeasMode);
	((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_STRS_MEAS_END with NG(or Cancel)."));

	/* トランジション更新	*/
	TransiEvent(EV_STRS_CANCEL_DONE);
}

/*
 * Abort 後 ロード Pos 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_ABORTLDPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[13], (DWORD) pEvParams);

	/* ロード Pos へ移動 -------------------------------------- */
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to LD Pos. (abort)"));
	if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
		/* HWS です．	*/
		::Sleep(STRS_HWS_TIME);
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else
	if (0 == this->MoveToLdPos()) {
		/* 移動開始失敗	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LoadPositionMoveStartFailed));
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to LD Pos. (abort)"));
		/* 次のトランジションへ(ロード Pos へ移動せずに)	*/
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else {
		/* 移動開始成功	*/
		((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 72;	/* ハンドラ ロード ポジション 移動中	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
		/* WAIT 状態にするため処理はありません．	*/
	}
}

/*
 * Abort 後 ピン ダウン待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_ABORTPINDOWN(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[14], (DWORD) pEvParams);

	/* ピンを下降端へ移動	*/
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bLoadPos) {
		/* ロードポジションではなかった．	*/
		((CChiefView*) m_pcChiefView)->LogChief(_T("Not LD position."));
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
		/* ピンダウン出来ないので，次のトランジションへ(ピンダウンせずに)	*/
		this->TransiEvent(EV_STRS_PIN_DONE);
	} else {
		BOOL l_bNexResult;
		if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
			/* HWS です．	*/
			::Sleep(STRS_HWS_TIME);
			this->TransiEvent(EV_STRS_PIN_DONE);
		} else
/* added 2009.10.19 hmenjo 下降端ではピンダウンしない ---------- { ---------- */
			if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bPinDown) {
				/* ピンがすでに下降端なので次のトランジションへ	*/
				this->TransiEvent(EV_STRS_PIN_DONE);
			} else
/* added 2009.10.19 hmenjo 下降端ではピンダウンしない ---------- { ---------- */
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- { ---------- */
//		if (0 == (l_bNexResult = nexifMoveToLower(m_pcChiefView->m_hWnd))) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ----------			  */
		if (0 == (l_bNexResult = nexifLoad(m_pcChiefView->m_hWnd))) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- } ---------- */
			/* 失敗	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move Lifter to lower pos."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
			/* 次のトランジションへ(ピンダウンせずに)	*/
			this->TransiEvent(EV_STRS_PIN_DONE);
		} else {
			/* 成功	*/
			((CChiefView*) m_pcChiefView)->m_dwPinMoveState = 71;	/* ピン移動中	*/
			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_PIN, TRUE);
			/* WAIT 状態にするため処理はありません．	*/
		}
	}
}

/*
 * Abort 時間待ち(WAIT 状態無し)
 */
void CChiefTransiStress::enter_ST_STRS_ABORTDELAY(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[15], (DWORD) pEvParams);

/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	/* SR ヘッドに戻します．	*/
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */

	/* HEPA 起動	*/
	((CChiefView*) m_pcChiefView)->HepaOnOff(TRUE);

	/* 時間待ち	*/
	::Sleep(TIMER_DELAY_ABORT);

	/* 測定キャンセル完了を Pif へ報告	*/
	((CChiefView*) m_pcChiefView)->ReportCancelComplete(0);

/* added 2009.08.18 hmenjo ストレス機能追加(46) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
/* added 2009.08.18 hmenjo ストレス機能追加(46) ---------- } ---------- */

	/* トランジション更新	*/
	TransiEvent(EV_STRS_ABORT_DONE);

	/* 呼出元トランジションに完了を通知	*/
	((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) m_iCancelStat);
}

/*
 * ポーズ解除待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiStress::enter_ST_STRS_PAUSE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[16], (DWORD) pEvParams);

	m_bPause = FALSE;

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_PAUSING);	// Status Bar

	switch (m_iPausePreState) {
	case ST_STRS_SRMEAS:
		switch (m_PauseStat.dwSrMeasuring) {
		case 0:	m_PauseStat.dwSrMeasuring = 1;	break;	/* 測定中	*/
		case 1:	m_PauseStat.dwSrMeasuring = 2;			/* 測定済	*/
			switch ((DWORD) pEvParams) {
			case TR_SEQ_OK:	/* 正常完了	*/
				break;
			default:
				/* 異常完了	*/
				/* キャンセルをポスト	*/
				this->TransiEvent(EV_STRS_CANCEL, pEvParams);
				return;
				break;
			}
			break;
		default:
			break;
		}
		break;
	case ST_STRS_PRELDPOS:
		switch (m_PauseStat.dwPreLoadingPos) {
		case 0:	m_PauseStat.dwPreLoadingPos = 1;	break;	/* 移動中	*/
		case 1:	m_PauseStat.dwPreLoadingPos = 2;	break;	/* 移動済	*/
		default:	break;
		}
		break;
	case ST_STRS_PINALIGN:
		switch (m_PauseStat.dwPinAligning) {
		case 0:	m_PauseStat.dwPinAligning = 1;	break;	/* 動作中	*/
		case 1:	m_PauseStat.dwPinAligning = 2;	break;	/* 動作済	*/
		default:	break;
		}
		break;
	case ST_STRS_LMVSTAPOS:
	case ST_STRS_LMVENDPOS:
	default:
		break;
	}

	/* WAIT 状態にするため処理はありません．	*/
}

/*
 * レジューム前処理(WAIT 状態無し)
 */
void CChiefTransiStress::enter_ST_STRS_PRE_RESUME(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[17], (DWORD) pEvParams);

	m_bPause = FALSE;

	int l_iEventNo = 0;
	DWORD l_dwEvParam;
	switch (m_iPausePreState) {
	case ST_STRS_SRMEAS:
		if (1 == m_PauseStat.dwSrMeasuring) {
			l_iEventNo = EV_STRS_RSM_SRMEAS;	/* 測定中：ST_STRS_SRMEAS へ		*/
			l_dwEvParam = 0;
/* added 2009.08.21 hmenjo ストレス機能追加(50) ---------- { ---------- */
			if (ST_SEQ_PAUSE == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->GetCurrentState()) {
				/* シーケンス測定 トランジションへレジューム指令	*/
				((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_RESUME);
			}
/* added 2009.08.21 hmenjo ストレス機能追加(50) ---------- } ---------- */
		} else {
			l_iEventNo = EV_STRS_RSM_SRMEASED;	/* 測定済：ST_STRS_SRMEAS_DONE へ	*/
			l_dwEvParam = 0;
			m_PauseStat.dwSrMeasuring = 0;
		}
		break;
	case ST_STRS_PRELDPOS:
		if (1 == m_PauseStat.dwPreLoadingPos) {
			l_iEventNo = EV_STRS_RSM_PRELDPOS;	/* 移動中：ST_STRS_PRELDPOS へ	*/
			l_dwEvParam = 0;
		} else {
			l_iEventNo = EV_STRS_RSM_PINALIGN;	/* 移動済：ST_STRS_PINALIGN へ	*/
			l_dwEvParam = 0;
			m_PauseStat.dwPreLoadingPos = 0;
		}
		break;
	case ST_STRS_PINALIGN:
		if (1 == m_PauseStat.dwPinAligning) {
			l_iEventNo = EV_STRS_RSM_PINALIGN;	/* 動作中：ST_STRS_PINALIGN へ	*/
			l_dwEvParam = 0;
		} else {
			l_iEventNo = EV_STRS_RSM_LMVSTAPOS;	/* 動作済：ST_STRS_LMVSTAPOS へ	*/
			l_dwEvParam = 0;
			m_PauseStat.dwPinAligning = 0;
		}
		break;
	case ST_STRS_LMVSTAPOS:
		l_iEventNo = EV_STRS_RSM_LMVSTAPOS;		/* ST_STRS_LMVSTAPOS へ	*/
		l_dwEvParam = 0;
		break;
	case ST_STRS_LMVENDPOS:
		l_iEventNo = EV_STRS_RSM_LMVENDPOS;		/* ST_STRS_LMVENDPOS へ	*/
		l_dwEvParam = 0;
		break;
	}

	if (0 != l_iEventNo) {
		this->TransiEvent(l_iEventNo, (cEventParams*) l_dwEvParam);
	}
}
