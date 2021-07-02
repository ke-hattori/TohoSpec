// ChifTransiDeskew.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <Active.hxx>
#include "ChifTransiDeskew.h"
#include "ChiefExports.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "..\\..\\INC\\Deskew.hxx"
#include "DeskewSeq.h"
#include "ChiefView.h"
#include "ChifTransiSeq.h"
#include "ChifTransiAF.h"
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
#include "..\\..\\INC\\NSStage.hxx"
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */

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
UINT WM_CHIF_TRDSK_SETEVENT = RegisterWindowMessage("WM_CHIF_TRDSK_SETEVENT");	// トランジションイベント発行用ウィンドウメッセージ
UINT WM_CHIF_TRDSK_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TRDSK_HEARTBEAT");	// HeartBeat ウィンドウメッセージ


// ##########################################################################
// class CChiefTransiDeskew : デスキュー トランジション
// ##########################################################################

// =========================================================================//
//	トランジションテーブル
// =========================================================================//
const Transition TransTable_Deskew[] =
{
	//	OldState,				EventId,				NewState

	{	ST_DSKW_INIT,			EV_DSKW_DO_INIT,		ST_DSKW_INIT			},	// 初期化処理を実行(すぐ EV_DSKW_INIT_DONE で)
	{	ST_DSKW_INIT, 			EV_DSKW_INIT_DONE,		ST_DSKW_IDLE			},	// 初期化完了してアイドルへ

	{	ST_DSKW_IDLE, 			EV_DSKW_START,			ST_DSKW_START			},	// 開始前処理へ

	{	ST_DSKW_START, 			EV_DSKW_IDLE,			ST_DSKW_IDLE			},	// デスキュー無し設定なので完了処理後(すぐ EV_DSKW_IDLE で)アイドルへ
	{	ST_DSKW_START, 			EV_DSKW_MANU,			ST_DSKW_MANU			},	// マニュアルデスキュー完了待ちへ
// 2009.03.11 K.Matsuo change -->
//	{	ST_DSKW_START, 			EV_DSKW_AF,				ST_DSKW_AF				},	// AF 完了待ちへ

//	{	ST_DSKW_AF,				EV_DSKW_AF_DONE,		ST_DSKW_AF_DONE			},	// AF 完了
//	{	ST_DSKW_AF_DONE,		EV_DSKW_AF_OK,			ST_DSKW_AUTO			},	// AF OK なので オートデスキュー完了待ち へ
//	{	ST_DSKW_AF_DONE,		EV_DSKW_AF_NG,			ST_DSKW_MANU			},	// AF NG なので マニュアルデスキュー完了待ち へ
//	{	ST_DSKW_AF_DONE,		EV_DSKW_AF_CANCEL,		ST_DSKW_IDLE			},	// AF ポップアップでキャンセルなので アイドルへ
	{	ST_DSKW_START, 			EV_DSKW_AUTO,			ST_DSKW_AUTO			},	// オートデスキュー完了待ちへ
// 2009.03.11 K.Matsuo change <--

	{	ST_DSKW_AUTO,			EV_DSKW_AUTO_OK,		ST_DSKW_AUTO_OK			},	// オートデスキュー OK 完了(すぐ EV_DSKW_IDLE で)
	{	ST_DSKW_AUTO_OK, 		EV_DSKW_IDLE,			ST_DSKW_IDLE			},	// 完了処理実行してアイドルへ
	{	ST_DSKW_AUTO,			EV_DSKW_AUTO_PROCEED,	ST_DSKW_AUTO_PROCEED	},	// オートデスキュー NG(続行) 完了(すぐ EV_DSKW_IDLE で)
	{	ST_DSKW_AUTO_PROCEED, 	EV_DSKW_IDLE,			ST_DSKW_IDLE			},	// 完了処理実行してアイドルへ
	{	ST_DSKW_AUTO,			EV_DSKW_AUTO_CANCEL,	ST_DSKW_AUTO_CANCEL		},	// オートデスキュー NG(レシピ終了) 完了(すぐ EV_DSKW_IDLE で)
	{	ST_DSKW_AUTO_CANCEL,	EV_DSKW_IDLE,			ST_DSKW_IDLE			},	// 完了処理実行してアイドルへ
	{	ST_DSKW_AUTO,			EV_DSKW_AUTO_MANU,		ST_DSKW_MANU			},	// オートデスキュー NG(マニュアルデスキュー) 完了，マニュアルデスキュー完了待ちへ
// 2009.04.10 K.Matsuo Add -->
	{	ST_DSKW_AUTO,			EV_DSKW_AUTO_PROCEED_PMA, ST_DSKW_AUTO_PROCEED_PMA},// オートデスキュー NG(続行・位置補正付き測定) 完了(すぐ EV_DSKW_IDLE で)
	{	ST_DSKW_AUTO_PROCEED_PMA,EV_DSKW_IDLE,			ST_DSKW_IDLE			},	// 完了処理実行してアイドルへ
// 2009.04.10 K.Matsuo Add <--

	{	ST_DSKW_MANU, 			EV_DSKW_MANU_OK,		ST_DSKW_MANU_OK			},	// マニュアルデスキュー OK 完了(すぐ EV_DSKW_IDLE で)
	{	ST_DSKW_MANU_OK, 		EV_DSKW_IDLE,			ST_DSKW_IDLE			},	// 完了処理実行してアイドルへ
	{	ST_DSKW_MANU, 			EV_DSKW_MANU_CANCEL,	ST_DSKW_MANU_CANCEL		},	// マニュアルデスキュー NG(レシピ終了) 完了(すぐ EV_DSKW_IDLE で)
	{	ST_DSKW_MANU_CANCEL, 	EV_DSKW_IDLE,			ST_DSKW_IDLE			},	// 完了処理実行してアイドルへ
	{	ST_DSKW_MANU, 			EV_DSKW_MANU_NONE,		ST_DSKW_MANU_NONE		},	// マニュアルデスキュー NG(しない) 完了(すぐ EV_DSKW_IDLE で)
	{	ST_DSKW_MANU_NONE, 		EV_DSKW_IDLE,			ST_DSKW_IDLE			},	// 完了処理実行してアイドルへ

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};

// =========================================================================//
//	状態別処理関数テーブル
// =========================================================================//
BEGIN_STATEPROC_MAP(CChiefTransiDeskew)
	/*	0 */	ENTER_STATE(ST_DSKW_INIT,			enter_ST_DSKW_INIT),
	/*	1 */	ENTER_STATE(ST_DSKW_IDLE,			enter_ST_DSKW_IDLE),
	/*	2 */	ENTER_STATE(ST_DSKW_START,			enter_ST_DSKW_START),				// (WAIT 状態無し)
	/*	3 */	ENTER_STATE(ST_DSKW_AF,				enter_ST_DSKW_AF),
	/*	4 */	ENTER_STATE(ST_DSKW_AF_DONE,		enter_ST_DSKW_AF_DONE),				// (WAIT 状態無し)
	/*	5 */	ENTER_STATE(ST_DSKW_AUTO,			enter_ST_DSKW_AUTO),
	/*	6 */	ENTER_STATE(ST_DSKW_AUTO_OK,		enter_ST_DSKW_AUTO_OK),				// (WAIT 状態無し)
	/*	7 */	ENTER_STATE(ST_DSKW_AUTO_PROCEED,	enter_ST_DSKW_AUTO_PROCEED),		// (WAIT 状態無し)
	/*	8 */	ENTER_STATE(ST_DSKW_AUTO_CANCEL,	enter_ST_DSKW_AUTO_CANCEL),			// (WAIT 状態無し)
// 2009.04.10 K.Matsuo Change -->
	/*	9 */	ENTER_STATE(ST_DSKW_AUTO_PROCEED_PMA,	enter_ST_DSKW_AUTO_PROCEED_PMA),	// (WAIT 状態無し)
	/* 10 */	ENTER_STATE(ST_DSKW_MANU,			enter_ST_DSKW_MANU),
	/* 11 */	ENTER_STATE(ST_DSKW_MANU_OK,		enter_ST_DSKW_MANU_OK),				// (WAIT 状態無し)
	/* 12 */	ENTER_STATE(ST_DSKW_MANU_CANCEL,	enter_ST_DSKW_MANU_CANCEL),			// (WAIT 状態無し)
	/* 13 */	ENTER_STATE(ST_DSKW_MANU_NONE,		enter_ST_DSKW_MANU_NONE),			// (WAIT 状態無し)
// 2009.04.10 K.Matsuo Change <--

END_STATEPROC_MAP(CChiefTransiDeskew)


/////////////////////////////////////////////////////////////////////////////
// CChiefTransiDeskew

IMPLEMENT_DYNCREATE(CChiefTransiDeskew, CWinThread)

CChiefTransiDeskew::CChiefTransiDeskew(CWnd* pParent /*=NULL*/) : cActive(TransTable_Deskew)
{
	TRACE(_T("CChiefTransiDeskew::CChiefTransiDeskew() \n"));

	m_pcChiefView = (CView*) pParent;

	//	スレッドの起動待ち
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiDeskew."));

	//	オブジェクトの自動破棄を禁止
	m_bAutoDelete = FALSE;

// 2009.03.18 K.Matsuo Add -->
	m_pDeskewSeq = NULL;
// 2009.03.18 K.Matsuo Add <--

}

CChiefTransiDeskew::~CChiefTransiDeskew()
{
	TRACE(_T("CChiefTransiDeskew::~CChiefTransiDeskew() \n"));

	//	スレッドの消滅待ち
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiDeskew."));
		}
	}
}

BOOL CChiefTransiDeskew::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiDeskew::InitInstance() \n"));

	// コンストラクタの待ちを解除します
	//		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．
	m_cSyncEvent.SetEvent();

	// 初期状態処理用イベント発行
	TransiEvent(0, 0);

	// 変数初期化
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiDeskew::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiDeskew::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiDeskew, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiDeskew)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRDSK_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRDSK_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiDeskew メッセージ ハンドラ

/*
 *	トランジションイベント発行 メッセージハンドラ
 */
void CChiefTransiDeskew::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	// トランジション間タイミング調整(とりあえずだけど恒久かも・・)

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat メッセージハンドラ
 */
void CChiefTransiDeskew::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	トランジションイベント発行用関数
 */
void CChiefTransiDeskew::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TRDSK_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TRDSK_SETEVENT, _T("WM_CHIF_TRDSK_SETEVENT"), 4, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
}

/*
 *	HeartBeat 発行用関数
 */
void CChiefTransiDeskew::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TRDSK_HEARTBEAT, 0, 0);
}

/*
 *	現在の状態が「アイドル」かどうかをチェック
 *		戻り値：	true	アイドルである
 *					false	アイドルでない
 */
bool CChiefTransiDeskew::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_DSKW_IDLE) {
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
int CChiefTransiDeskew::GetCurrentState()
{
	return CurrState;
}

/*
 *	各デスキューモジュールをキャンセル終了させます．
 */
void CChiefTransiDeskew::CancelDeskew()
{
	switch (CurrState) {
	case ST_DSKW_AUTO:
		// オートデスキューをキャンセルします
		/*
		 *	ココにオートデスキューのキャンセル処理を入れてください．
		 */
		m_pDeskewSeq->ShutDown();
		break;
	case ST_DSKW_MANU:
		// マニュアルデスキューをキャンセルします
		/*
		 *	ココにマニュアルデスキューのキャンセル処理を入れてください．
		 */
		m_pDeskewSeq->ShutDown();
		break;
	default:
		// 何もしません
		break;
	}
}

/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
void CChiefTransiDeskew::NotifyComplete(const cEventParams *const pEvParams)
{
	/* 以前のヘッドに戻します．	*/
	((CChiefView*) m_pcChiefView)->SelectHead(m_wHeadPre, FALSE);

	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, pEvParams);
}
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */

//--------------------------------------------------------------------------//
//	状態別処理関数群
//--------------------------------------------------------------------------//
/*
 *	初期状態
 */
void CChiefTransiDeskew::enter_ST_DSKW_INIT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[0], (DWORD) pEvParams);

	// 初期化処理はありません．
}

/*
 *	アイドル
 */
void CChiefTransiDeskew::enter_ST_DSKW_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[1], (DWORD) pEvParams);

	// WAIT 状態にするため処理はありません．
}

/*
 *	開始前処理(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_START(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[2], (DWORD) pEvParams);

	DWORD	l_dwErrCode = TR_DSKW_OK;

/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	m_wHeadPre = NS_GetCurrentHeadType();
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);		/* SR ヘッドにします．	*/
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
// 2013.02.22 bagus Substrate thickness setting -->
	LPSTAGE_PROG_INFO_HDR	pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
	if (((CChiefView*) m_pcChiefView)->SelectSubstrateThick(pStageProgInfoHdr->SampleInfo.dThickness)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("Select Substrate Thickness failed."));
	}
// 2013.02.22 bagus Substrate thickness setting <--

	// デスキューモードを判定 -------------------------------------------------
	int l_iEventNext;
	// ハードシミュレートモード判定
	if (0 == ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		// 実機の場合 -----------------------------------------------------
		LPSTAGE_PROG_INFO_HDR	l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
		switch (l_pStageProgInfoHdr->nDeskewMode) {
		case DESKEW_MODE_MANUAL_DESKEW:		// マニュアルデスキュー
			l_iEventNext = EV_DSKW_MANU;
			// 次のトランジションでマニュアルデスキューを発行しています．
			break;
		case DESKEW_MODE_PATTERN_DESKEW:	// オートデスキュー
			// AF 発行
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//			if (false == ((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
			if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(3, 4)) {
				((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiDeskew::enter_ST_DSKW_START() - AF Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
				// AF トランジションがビジーでした．
				l_iEventNext = EV_DSKW_IDLE;
				l_dwErrCode = TR_DSKW_TRAF_BUSY;
			} else {
// 2009.03.11 K.Matsuo change -->
//				// 次のトランジションで AF 指令を発行しています．
//				l_iEventNext = EV_DSKW_AF;
				l_iEventNext = EV_DSKW_AUTO;
// 2009.03.11 K.Matsuo change <--
			}
			break;
		case DESKEW_MODE_NO_DESKEW:			// デスキュー無し
		default:
			// Deskew Clear
			DeskewClear();
			l_iEventNext = EV_DSKW_IDLE;
			break;
		}
	} else {
		// ハードシミュレートモードの場合はデスキュー無しにする -----------
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_HWS_DESKEWING);	// Status Bar
#ifdef CHIEF_USE_SLEEP
		::Sleep(2000);
#endif
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_HWS_DESKEWED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		l_iEventNext = EV_DSKW_IDLE;
	}

	if (EV_DSKW_IDLE != l_iEventNext) {
		// ターレット制御(デスキュー用：メインレシピの Lens，あるいは，ステージプログラムの Lens)
		//		ここではステージプログラムの Lens を使います
		// 2009.01.30 k.matsuo delete ----->
		// ここではダークリファレンス位置の可能性大なので、オートフォーカスエラーする。デスキューポジションで行なうにする。
//		LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
//		((CChiefView*) m_pcChiefView)->SelectLens(l_pStageProgInfoHdr->iLens);
		// 2009.01.30 k.matsuo delete <-----
		// カラーフィルタ制御(デスキュー用：OPEN にします)
		if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
			// 異常時でも無視します．
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiDeskew::enter_ST_DSKW_START()."));
		}
	}

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	// 呼出元(SEQ のみのはず)トランジションに完了を通知
	if (EV_DSKW_IDLE == l_iEventNext) {
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) l_dwErrCode);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
		this->NotifyComplete((cEventParams*) l_dwErrCode);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
	}
}

/*
 *	AF 完了待ち
 */
void CChiefTransiDeskew::enter_ST_DSKW_AF(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[3], (DWORD) pEvParams);

	// AF 発行
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_FOCUSING);	// Status Bar
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//	((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(TRANSI_DESKEW, 0));
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
	((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_DESKEW, 0), 0));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */

	// WAIT 状態にするためトランジションを更新しません．
}

/*
 *	AF 完了(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_AF_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[4], (DWORD) pEvParams);

	AF_VALID	l_AFValid;

	// AF 結果判定
	int l_iEventNext;
	l_AFValid.dwValid = (DWORD) pEvParams;
#if 0	// AF 結果判定を変更 -------- { --------
	switch (l_AFValid.ucResultAF) {
	case 0:		// AF 無し
	case 1:		// AF 正常
	case 2:		// 自動強制測定
		l_iEventNext = EV_DSKW_AF_OK;
		break;
	case 4:		// AF 失敗(元ポップアップ)
		switch (l_AFValid.ucResultPopup) {
		case 2:		// 閉じる(OK)(元 AF 無視)ボタン
			l_iEventNext = EV_DSKW_AF_OK;
			break;
		case 4:		// キャンセルボタン
			l_iEventNext = EV_DSKW_AF_CANCEL;
			break;
		case 0:		// ポップアップ無し
		case 1:		// リトライボタン
		case 3:		// スキップボタン
		default:
			l_iEventNext = EV_DSKW_AF_NG;
			break;
		}
		break;
	case 5:		// レシピ終了
		l_iEventNext = EV_DSKW_AF_CANCEL;
		break;
	case 3:		// 自動スキップ
	default:
		l_iEventNext = EV_DSKW_AF_NG;
		break;
	}
#else	// AF 結果判定を変更 --------
	switch (l_AFValid.ucResultPopup) {
	case 0:		// ポップアップ無し
	case 1:		// リトライボタン
		switch (l_AFValid.ucResultAF) {
		case 0:		// AF 無し
		case 1:		// AF 正常
		case 2:		// 自動強制測定
			l_iEventNext = EV_DSKW_AF_OK;
			break;
		case 3:		// 自動スキップ
		case 4:		// AF 失敗(ココでこの状態は存在しないはず)
		case 5:		// レシピ終了(デスキューではキャンセルさせず NG とし，マニュアルデスキューに移行させます)
		default:
			l_iEventNext = EV_DSKW_AF_NG;
			break;
		}
		break;
	case 2:		// 閉じる(OK)(元 AF 無視)ボタン
		l_iEventNext = EV_DSKW_AF_OK;
		break;
	case 4:		// キャンセルボタン
		l_iEventNext = EV_DSKW_AF_CANCEL;
		break;
	case 3:		// スキップボタン
	default:
		l_iEventNext = EV_DSKW_AF_NG;
		break;
	}
#endif	// AF 結果判定を変更 -------- } --------

	// 次のトランジションでオート/マニュアルデスキューを発行しています．

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	if (EV_DSKW_AF_CANCEL == l_iEventNext) {
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_FOCUS_CANCEL);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		// 呼出元(SEQ のみのはず)トランジションに完了を通知
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) TR_DSKW_AF_CANCEL);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
		this->NotifyComplete((cEventParams*) TR_DSKW_AF_CANCEL);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
	}
}

/*
 *	オートデスキュー完了待ち
 */
void CChiefTransiDeskew::enter_ST_DSKW_AUTO(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[5], (DWORD) pEvParams);

	// オートデスキュー発行
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_AUTO_ING);	// Status Bar
	// パターン認識フラグをセット
	((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->SetPatRecFlag(TRUE);
	/*
	 *	ココにオートデスキューの起動処理を入れてください．
	 */
	m_pDeskewSeq = (CDeskewSeq*)AfxBeginThread(RUNTIME_CLASS(CDeskewAutoSeq), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_pDeskewSeq->m_pChiefView			= (CChiefView*)m_pcChiefView;
	m_pDeskewSeq->m_pChiefTransiDeskew	= this;
	m_pDeskewSeq->m_pStageProgInfoHdr 	= (LPSTAGE_PROG_INFO_HDR)((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
	m_pDeskewSeq->ResumeThread();


	// WAIT 状態にするためトランジションを更新しません．
}

/*
 *	オートデスキュー OK 完了(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_AUTO_OK(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[6], (DWORD) pEvParams);

// 2009.03.18 K.Matsuo Add -->
	m_pDeskewSeq = NULL;
// 2009.03.18 K.Matsuo Add <--

	// パターン認識フラグをリセット
	((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->SetPatRecFlag(FALSE);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_AUTO_COMPLETE);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_DSKW_IDLE);

	// 呼出元(SEQ のみのはず)トランジションに完了を通知
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) TR_DSKW_OK);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
	this->NotifyComplete((cEventParams*) TR_DSKW_OK);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
}

/*
 *	オートデスキュー失敗のまま続行(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_AUTO_PROCEED(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[7], (DWORD) pEvParams);

// 2009.03.18 K.Matsuo Add -->
	m_pDeskewSeq = NULL;
// 2009.03.18 K.Matsuo Add <--

	// パターン認識フラグをリセット
	((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->SetPatRecFlag(FALSE);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_AUTO_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_DSKW_IDLE);

	// 呼出元(SEQ のみのはず)トランジションに完了を通知
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) TR_DSKW_AUTO_PROCEED);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
	this->NotifyComplete((cEventParams*) TR_DSKW_AUTO_PROCEED);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
}

/*
 *	オートデスキューレシピ終了(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_AUTO_CANCEL(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[8], (DWORD) pEvParams);

// 2009.03.18 K.Matsuo Add -->
	m_pDeskewSeq = NULL;
// 2009.03.18 K.Matsuo Add <--

	// パターン認識フラグをリセット
	((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->SetPatRecFlag(FALSE);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_AUTO_CANCEL);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_DSKW_IDLE);

	// 呼出元(SEQ のみのはず)トランジションに完了を通知
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) TR_DSKW_AUTO_CANCEL);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
		this->NotifyComplete((cEventParams*) TR_DSKW_AUTO_CANCEL);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
}

// 2009.04.10 K.Matsuo Add -->
/*
 *	オートデスキュー失敗のまま続行・位置補正付き測定(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_AUTO_PROCEED_PMA(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[9], (DWORD) pEvParams);

	m_pDeskewSeq = NULL;

	// パターン認識フラグをリセット
	((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->SetPatRecFlag(FALSE);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_AUTO_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// 測定開始時に読み込んだレシピ内容（バッファ）に「PMAを使用する」を強制設定する
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	l_pMainRcpInfo->MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_ALL_POINT_EVERY_ADJUSTMENT;

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_DSKW_IDLE);

	// 呼出元(SEQ のみのはず)トランジションに完了を通知
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) TR_DSKW_AUTO_PROCEED);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
	this->NotifyComplete((cEventParams*) TR_DSKW_AUTO_PROCEED);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
}
// 2009.04.10 K.Matsuo Add <--

/*
 *	マニュアルデスキュー完了待ち
 */
void CChiefTransiDeskew::enter_ST_DSKW_MANU(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[10], (DWORD) pEvParams);

// 2009.03.18 K.Matsuo Add -->
	m_pDeskewSeq = NULL;
// 2009.03.18 K.Matsuo Add <--

	// パターン認識フラグをリセット
	((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->SetPatRecFlag(FALSE);

	// マニュアルデスキュー発行
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_MANU_ING);	// Status Bar
	/*
	 *	ココにマニュアルデスキューの起動処理を入れてください．
	 */
	m_pDeskewSeq = (CDeskewSeq*)AfxBeginThread(RUNTIME_CLASS(CDeskewManualSeq), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_pDeskewSeq->m_pChiefView			= (CChiefView*)m_pcChiefView;
	m_pDeskewSeq->m_pChiefTransiDeskew	= this;
	m_pDeskewSeq->m_pStageProgInfoHdr 	= (LPSTAGE_PROG_INFO_HDR)((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
	m_pDeskewSeq->ResumeThread();

// 2009.04.10 K.Matsuo Add -->
	// ポップアップをアラームで通知します．
	if (HOST_LOCAL != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_ManualDeskewRequired));
	}
// 2009.04.10 K.Matsuo Add <--

	// WAIT 状態にするためトランジションを更新しません．
}

/*
 *	マニュアルデスキュー OK 完了(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_MANU_OK(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[11], (DWORD) pEvParams);

// 2009.03.18 K.Matsuo Add -->
	m_pDeskewSeq = NULL;
// 2009.03.18 K.Matsuo Add <--

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_MANU_COMPLETE);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_DSKW_IDLE);

	// 呼出元(SEQ のみのはず)トランジションに完了を通知
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) TR_DSKW_OK);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
	this->NotifyComplete((cEventParams*) TR_DSKW_OK);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
}

/*
 *	マニュアルデスキューレシピ終了(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_MANU_CANCEL(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[12], (DWORD) pEvParams);

// 2009.03.18 K.Matsuo Add -->
	m_pDeskewSeq = NULL;
// 2009.03.18 K.Matsuo Add <--

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_MANU_CANCEL);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_DSKW_IDLE);

	// 呼出元(SEQ のみのはず)トランジションに完了を通知
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) TR_DSKW_MANU_CANCEL);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
	this->NotifyComplete((cEventParams*) TR_DSKW_MANU_CANCEL);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
}

/*
 *	マニュアルデスキューなしで続行(WAIT 状態無し)
 */
void CChiefTransiDeskew::enter_ST_DSKW_MANU_NONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_DESKEW_DISP[13], (DWORD) pEvParams);

// 2009.03.18 K.Matsuo Add -->
	m_pDeskewSeq = NULL;
// 2009.03.18 K.Matsuo Add <--

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DSKW_MANU_IGNORE);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_DSKW_IDLE);

	// 呼出元(SEQ のみのはず)トランジションに完了を通知
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DESKEW_DONE, (cEventParams*) TR_DSKW_MANU_PROCEED);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
	this->NotifyComplete((cEventParams*) TR_DSKW_MANU_PROCEED);
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
}
