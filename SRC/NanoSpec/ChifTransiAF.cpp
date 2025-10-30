// ChifTransiAF.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <Active.hxx>
#include "ChifTransiAF.h"
#include "ChiefExports.h"
#include "nanospec.h"
#include "ChiefView.h"
#include "MainFrm.h"
#include "ChifTransiMaster.h"
#include "ChifTransiSrRefer.h"
#include "ChifTransiDeskew.h"
#include "ChifTransiSr1Point.h"
#include "NanoSpecDoc.h"

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
UINT WM_CHIF_TRAF_SETEVENT = RegisterWindowMessage("WM_CHIF_TRAF_SETEVENT");	// トランジションイベント発行用ウィンドウメッセージ
UINT WM_CHIF_TRAF_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TRAF_HEARTBEAT");	// HeartBeat ウィンドウメッセージ


// ##########################################################################
// class CChiefTransiAF : AF トランジション
// ##########################################################################

// =========================================================================//
//	トランジションテーブル
// =========================================================================//
const Transition TransTable_AF[] =
{
	//	OldState,				EventId,				NewState

	{	ST_AAF_INIT,			EV_AAF_DO_INIT,			ST_AAF_INIT				},	// 初期化処理を実行(すぐ EV_AAF_INIT_DONE で)
	{	ST_AAF_INIT,			EV_AAF_INIT_DONE,		ST_AAF_IDLE				},	// 初期化完了してアイドルへ

	{	ST_AAF_IDLE,			EV_AAF_START,			ST_AAF_AF				},	// AF 実行して，AF 完了待ちへ

	{	ST_AAF_AF,				EV_AAF_OK,				ST_AAF_IDLE				},	// AF OK 完了(すぐ EV_AAF_IDLE で)アイドルへ
	{	ST_AAF_AF,				EV_AAF_RO_PROCEED,		ST_AAF_IDLE				},	// AF NG(自動強制測定) 完了(すぐ EV_AAF_IDLE で)アイドルへ
	{	ST_AAF_AF,				EV_AAF_RO_SKIP,			ST_AAF_IDLE				},	// AF NG(自動スキップ) 完了(すぐ EV_AAF_IDLE で)アイドルへ
	{	ST_AAF_AF,				EV_AAF_RO_CANCEL,		ST_AAF_IDLE				},	// AF NG(レシピ終了) 完了(すぐ EV_AAF_IDLE で)アイドルへ
	{	ST_AAF_AF,				EV_AAF_RO_POPUP,		ST_AAF_POPUP			},	// AF NG(ポップアップ) 完了，ポップアップ発行してポップアップ終了待ちへ

	{	ST_AAF_POPUP,			EV_AAF_POPUP_RETRY,		ST_AAF_POPUP_RETRY		},	// リトライボタン(すぐ EV_AAF_START で)
	{	ST_AAF_POPUP_RETRY,		EV_AAF_START,			ST_AAF_AF				},	// 完了処理実行して AF 完了待ちへ
	{	ST_AAF_POPUP,			EV_AAF_POPUP_IGNORE,	ST_AAF_POPUP_IGNORE		},	// AF 無視ボタン(すぐ EV_AAF_IDLE で)
	{	ST_AAF_POPUP_IGNORE,	EV_AAF_IDLE,			ST_AAF_IDLE				},	// 完了処理実行してアイドルへ
	{	ST_AAF_POPUP,			EV_AAF_POPUP_SKIP,		ST_AAF_POPUP_SKIP		},	// スキップボタン(すぐ EV_AAF_IDLE で)
	{	ST_AAF_POPUP_SKIP,		EV_AAF_IDLE,			ST_AAF_IDLE				},	// 完了処理実行してアイドルへ
	{	ST_AAF_POPUP,			EV_AAF_POPUP_CANCEL,	ST_AAF_POPUP_CANCEL		},	// キャンセルボタン(すぐ EV_AAF_IDLE で)
	{	ST_AAF_POPUP_CANCEL,	EV_AAF_IDLE,			ST_AAF_IDLE				},	// 完了処理実行してアイドルへ

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};

// =========================================================================//
//	状態別処理関数テーブル
// =========================================================================//
BEGIN_STATEPROC_MAP(CChiefTransiAF)
	/*	0 */	ENTER_STATE(ST_AAF_INIT,			enter_ST_AAF_INIT),
	/*	1 */	ENTER_STATE(ST_AAF_IDLE,			enter_ST_AAF_IDLE),
	/*	2 */	ENTER_STATE(ST_AAF_AF,				enter_ST_AAF_AF),					// (WAIT 状態無し)
	/*	3 */	ENTER_STATE(ST_AAF_POPUP,			enter_ST_AAF_POPUP),
	/*	4 */	ENTER_STATE(ST_AAF_POPUP_RETRY,		enter_ST_AAF_POPUP_RETRY),			// (WAIT 状態無し)
	/*	5 */	ENTER_STATE(ST_AAF_POPUP_IGNORE,	enter_ST_AAF_POPUP_IGNORE),			// (WAIT 状態無し)
	/*	6 */	ENTER_STATE(ST_AAF_POPUP_SKIP,		enter_ST_AAF_POPUP_SKIP),			// (WAIT 状態無し)
	/*	7 */	ENTER_STATE(ST_AAF_POPUP_CANCEL,	enter_ST_AAF_POPUP_CANCEL),			// (WAIT 状態無し)

END_STATEPROC_MAP(CChiefTransiAF)


/////////////////////////////////////////////////////////////////////////////
// CChiefTransiAF

IMPLEMENT_DYNCREATE(CChiefTransiAF, CWinThread)

CChiefTransiAF::CChiefTransiAF(CWnd* pParent /*=NULL*/) : cActive(TransTable_AF)
{
	TRACE(_T("CChiefTransiAF::CChiefTransiAF() \n"));

	m_pcChiefView = (CView*) pParent;

	//	スレッドの起動待ち
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiAF."));

	//	オブジェクトの自動破棄を禁止
	m_bAutoDelete = FALSE;

}

CChiefTransiAF::~CChiefTransiAF()
{
	TRACE(_T("CChiefTransiAF::~CChiefTransiAF() \n"));

	//	スレッドの消滅待ち
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiAF."));
		}
	}
}

BOOL CChiefTransiAF::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiAF::InitInstance() \n"));

	// コンストラクタの待ちを解除します
	//		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．
	m_cSyncEvent.SetEvent();

	// 初期状態処理用イベント発行
	TransiEvent(0, 0);

	// 変数初期化
	m_dwAFValid = 0;
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiAF::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiAF::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiAF, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiAF)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRAF_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRAF_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiAF メッセージ ハンドラ

/*
 *	トランジションイベント発行 メッセージハンドラ
 */
void CChiefTransiAF::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	// トランジション間タイミング調整(とりあえずだけど恒久かも・・)

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat メッセージハンドラ
 */
void CChiefTransiAF::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	トランジションイベント発行用関数
 */
void CChiefTransiAF::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TRAF_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TRAF_SETEVENT, _T("WM_CHIF_TRAF_SETEVENT"), 3, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
}

/*
 *	HeartBeat 発行用関数
 */
void CChiefTransiAF::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TRAF_HEARTBEAT, 0, 0);
}

/*
 *	現在の状態が「アイドル」かどうかをチェック
 *		戻り値：	true	アイドルである
 *					false	アイドルでない
 */
bool CChiefTransiAF::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_AAF_IDLE) {
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
int CChiefTransiAF::GetCurrentState()
{
	return CurrState;
}

/*
 *	呼出元トランジションに完了を通知
 */
void CChiefTransiAF::NotifyComplete(const cEventParams *const pEvParams)
{
	switch (m_dwParentTransi) {
	case TRANSI_SEQ:
	case TRANSI_AF:
// bagus add 2014.01.10 -->
	case TRANSI_MASTER_STD:
	case TRANSI_MASTER_DRK:
// bagus add 2014.01.10 <--
	default:
		// 何もしません．
		break;
	case TRANSI_MASTER:
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_UIAAF_DONE, pEvParams);
		break;
	case TRANSI_SR_REFER:
		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_AF_DONE, pEvParams);
		break;
	case TRANSI_DESKEW:
		((CChiefTransiDeskew*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew)->TransiEvent(EV_DSKW_AF_DONE, pEvParams);
		break;
	case TRANSI_SR_1POINT:
		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_AF_DONE, pEvParams);
		break;
// bagus add 2014.01.10 -->
	case TRANSI_SR_STD_REFER:
		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_STD_AF_DONE, pEvParams);
		break;
// bagus add 2014.01.10 <--
	}
}

/*
 *	AF トランジションをキャンセル終了させます．
 */
void CChiefTransiAF::CancelAF()
{
	m_bCancelReq = TRUE;
}

//--------------------------------------------------------------------------//
//	状態別処理関数群
//--------------------------------------------------------------------------//
/*
 *	初期状態
 */
void CChiefTransiAF::enter_ST_AAF_INIT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[0], (DWORD) pEvParams);

	// 初期化処理はありません．
}

/*
 *	アイドル
 */
void CChiefTransiAF::enter_ST_AAF_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[1], (DWORD) pEvParams);

	m_bCancelReq = FALSE;
	// WAIT 状態にするため処理はありません．
}

/*
 *	AF 完了待ち(WAIT 状態無し)
 */
void CChiefTransiAF::enter_ST_AAF_AF(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOBYTE(LOWORD(pEvParams))：呼出元トランジション
/*				HIWORD(pEvParams)：AF モード
/*										０：AF のみ
/*										１：PMA のみ
/*										２：AF と PMA 両方
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[2], (DWORD) pEvParams);

	// このブロックは仮変数の定義です→ -----------------------------------------------------------------------------------------------
	// AF 関係の設定変数が不明なためです．
//		DWORD	l_dwRetryTime = 0;	// リトライ回数設定
//		BOOL	l_bPopup = FALSE;	// AF エラー時にポップアップするかどうか
//		DWORD	l_dwNGMethod = 0;	// ポップアップしない場合の終了方法
									//		1：スキップ
									//		2：強制測定(プロシード)(N/A にしない)
									//		3：強制測定(プロシード)(N/A にする)
									//		4：レシピ終了
	// このブロックは仮変数の定義です← -----------------------------------------------------------------------------------------------
	// リトライ回数設定を取得
	DWORD	l_dwRetryTime = 0;	// リトライ回数設定
	ConfigFile_GetNanoSpecIni(&l_dwRetryTime, CONFIG_FILE_SR_AUTOFOCUS_RETRY);
	if (50 < l_dwRetryTime) {	// 最大設定値の制限を５０回にしておきます．
		l_dwRetryTime = 50;
	}
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
/* added 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
	HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
	((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, l_pMainRcpInfo->MainRcpParam.hdr.wHeadType, &l_HeadMainRcpInf);
/* added 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
	// リトライ有無の判断
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
//	switch (l_pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ----------			  */
	switch (l_HeadMainRcpInf.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_PROCEED:
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_CANCEL_RECIPE:
		break;
	case AFOCUS_FAIL_OPTION_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_PROCEED:
	case AFOCUS_FAIL_OPTION_CANCEL_RECIPE:
	default:
		l_dwRetryTime = 0;		// リトライ無しとします
		break;
	}
	// ポップアップ指定を取得
	BOOL	l_bPopup;
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
//	switch (l_pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ----------			  */
	switch (l_HeadMainRcpInf.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_MANUAL_FOCUS:
		l_bPopup = TRUE;		// ポップアップする
		break;
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_PROCEED:
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_CANCEL_RECIPE:
	case AFOCUS_FAIL_OPTION_PROCEED:
	case AFOCUS_FAIL_OPTION_CANCEL_RECIPE:
	default:
		l_bPopup = FALSE;		// ポップアップしない
		break;
	}
	// 終了方法を取得
	DWORD	l_dwNGMethod;
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
//	switch (l_pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ----------			  */
	switch (l_HeadMainRcpInf.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_PROCEED:
	case AFOCUS_FAIL_OPTION_PROCEED:
		l_dwNGMethod = 2;		// 強制測定(プロシード)(N/A にしない)
		break;
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_CANCEL_RECIPE:
	case AFOCUS_FAIL_OPTION_CANCEL_RECIPE:
	default:
		l_dwNGMethod = 4;		// レシピ終了
		break;
	}

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;

	// 初回呼出時は VALID データをクリア
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//	if (0 != LOWORD(pEvParams)) {
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
	if (0 != LOBYTE(LOWORD(pEvParams))) {
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
		m_dwAFValid = 0;
		// 呼出元トランジションを保存
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//		m_dwParentTransi = LOWORD(pEvParams);
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
		m_dwParentTransi = LOBYTE(LOWORD(pEvParams));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
		// モードを保存 (0:AF のみ，1:PMA のみ，2:AF と PMA の両方)
		m_dwAfMode = HIWORD(pEvParams);
	}

	if (0 == ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		// 実機の場合 ---------------------------------------------------------
		BOOL l_bAF_Success = FALSE;
		DWORD l_dwRetryCnt = 0;	// リトライカウンタ
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
		if (3 == ((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove) {
			/* すでに，PreAF で AF 成功しているのでパスします．*/
			((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove = 4;
			l_bAF_Success = TRUE;
			((CChiefView*) m_pcChiefView)->LogChief(_T("PreAF - Pass AF. (m_lPreAFafterPreMove = 4)"));
		} else
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
		if ((1 != m_dwAfMode)) {
			// PMA のみでない場合のみ AF を実行します
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
			if (1 == ((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove) {
				((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove = 2 | 0x80000000;
				l_bPopup = FALSE;	/* ポップアップ無し	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("PreAF - Start AF. (m_lPreAFafterPreMove = 2 | 0x80000000)"));
			}
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
			/* AF は SR ヘッドで行います．	*/
			((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, TRUE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
			do {
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
				if (0 == NS_StageDoAutoFocus()) {
					// AF 失敗
					l_dwRetryCnt++;
				} else {
					// AF 成功
					l_bAF_Success = TRUE;
					((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
					break;		// ループを抜けます
				}
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
// キャンセル対応改造			} while (l_dwRetryCnt <= l_dwRetryTime);
			} while ((l_dwRetryCnt <= l_dwRetryTime) && (0 == m_bCancelReq));
		}
#if 1	// キャンセル対応改造
		if (0 != m_bCancelReq) {
			m_bCancelReq = FALSE;
			// トランジション更新 -----------------
			TransiEvent(EV_AAF_RO_CANCEL);
			// 呼出元トランジションに完了を通知
			l_pAFValid->ucResultAF = 5;		// レシピ終了
			NotifyComplete((cEventParams*) m_dwAFValid);
		} else
#endif
//		if (0 == l_bAF_Success) {
		if ((0 == l_bAF_Success) || (0 != m_dwAfMode)) {
			// AF 失敗，あるいは，AF モードに PMA がある場合
			if ((1 != m_dwAfMode) && (0 == l_bAF_Success)) {
				// AF を行った場合のみ(PMA のみでない場合)
				// リトライオーバをセット
				l_pAFValid->ucRetry = static_cast<long>(l_dwRetryCnt - 1);
			}
//			if (0 == l_bPopup) {
			if ((0 == l_bPopup) && ((0 == m_dwAfMode) || ((2 == m_dwAfMode) && (2 != l_dwNGMethod)))) {
				// ポップアップ無し指定で，モードが AF のみか，AF. のみ以外で自動強制測定以外の場合
				// 終了する ---------------------------
				l_pAFValid->ucResultPopup = 0;	// ポップアップ無し
				int l_iEventNext;
				switch (l_dwNGMethod) {
				case 1:		// スキップ
					l_pAFValid->ucResultAF = 3;		// 自動スキップ
					l_iEventNext = EV_AAF_RO_SKIP;
					break;
				case 2:		// 強制測定(N/A にしない)
				case 3:		// 強制測定(N/A にする)
					l_pAFValid->ucResultAF = 2;		// 自動強制測定
					l_iEventNext = EV_AAF_RO_PROCEED;
					break;
				case 4:		// レシピ終了
				default:
					l_pAFValid->ucResultAF = 5;		// レシピ終了
					l_iEventNext = EV_AAF_RO_CANCEL;
					break;
				}
				// トランジション更新 -----------------
				TransiEvent(l_iEventNext);
				// 呼出元トランジションに完了を通知
				NotifyComplete((cEventParams*) m_dwAFValid);
			} else {
				// ポップアップする -------------------
				//		次のトランジションでポップアップ指令を発行しています．
				// AF 結果を設定
				if (0 != l_bAF_Success) {
					// AF 成功時
					l_pAFValid->ucResultAF = 1;		// AF 正常
				} else {
					// AF 失敗時
					if (1 == m_dwAfMode) {
						// PMA のみの場合は AF していないので
						l_pAFValid->ucResultAF = 0;		// AF 無し
					} else {
						switch (l_dwNGMethod) {
						case 2:		// 強制測定(N/A にしない)
						case 3:		// 強制測定(N/A にする)
							l_pAFValid->ucResultAF = 2;		// 自動強制測定
							break;
						default:
							l_pAFValid->ucResultAF = 4;		// AF 失敗
							break;
						}
					}
				}
				// トランジション更新 -----------------
//				TransiEvent(EV_AAF_RO_POPUP);
				DWORD l_dwPopupMode;
				if (0 == l_bPopup) {
					l_dwPopupMode = 1;			// PMA のみ
				} else if (1 == m_dwAfMode) {
					l_dwPopupMode = 1;			// PMA のみ
				} else if (0 != m_dwAfMode) {
					if (0 != l_bAF_Success) {
						l_dwPopupMode = 1;		// PMA のみ
					} else {
						l_dwPopupMode = 2;		// AF と PMA の両方
					}
				} else {
					l_dwPopupMode = 0;			// AF のみ
				}
				l_pAFValid->bit2PopupKind = (BYTE) l_dwPopupMode;		// ポップアップ種類
				TransiEvent(EV_AAF_RO_POPUP, (cEventParams*) l_dwPopupMode);
			}
		} else {
			// AF 成功 --------------------------------------------------------
			l_pAFValid->ucResultAF = 1;		// AF 正常
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
			if (0 != (((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove & 0x80000000)) {
				((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove &= 0x7fffffff;
				((CChiefView*) m_pcChiefView)->LogChief(_T("PreAF - m_lPreAFafterPreMove = 2"));
			}
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
			// トランジション更新 ---------------------
			TransiEvent(EV_AAF_OK);
			// 呼出元トランジションに完了を通知
			NotifyComplete((cEventParams*) m_dwAFValid);
		}
	} else {
		// ハードシミュレートモードの場合は正常完了にする ---------------------
#ifdef CHIEF_USE_SLEEP
		::Sleep(1000);
/* added 2009.08.20 hmenjo ストレス機能追加(48) ---------- { ---------- */
#else
		::Sleep(200);
/* added 2009.08.20 hmenjo ストレス機能追加(48) ---------- } ---------- */
#endif
		l_pAFValid->ucRetry = 0;
		l_pAFValid->ucResultAF = 0;
		l_pAFValid->ucResultPopup = 0;
		l_pAFValid->bit1ManuXY = 0;
		l_pAFValid->bit1ManuZ = 0;
		l_pAFValid->bit1ManuAF = 0;
		l_pAFValid->bit1ManuAfRslt = 0;
		// トランジション更新 -------------------------
		TransiEvent(EV_AAF_OK);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) m_dwAFValid);
	}
}

/*
 *	ポップアップ終了待ち
 */
void CChiefTransiAF::enter_ST_AAF_POPUP(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[3], (DWORD) pEvParams);

	// ポップアップをアラームで通知します．
	if (HOST_LOCAL != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		WORD l_wAlID;
		switch ((long) pEvParams) {
		case 0:		l_wAlID = ALID_AutoFocusRequired;			break;	// AF のみ
		case 1:		l_wAlID = ALID_StagePositioningRequired;	break;	// PMA のみ
		case 2:
		default:	l_wAlID = ALID_FocusAndPositioningRequired;	break;	// AF と PMA の両方
		}
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, l_wAlID));
	}
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	/* PMA 有りの場合で，レシピが(MS)ヘッドなら，(MS)ヘッドに切替えます．	*/
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	if ((0 != ((long) pEvParams)) && (HEAD_TYPE_MS == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)) {
		if (1 != m_dwAfMode) {
			/* AF を実施した場合のみ	*/
			((CChiefView*) m_pcChiefView)->SelectHead(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType, TRUE);
		}
	}
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
	// ポップアップする -------------------
	// 画面にポップアップ指令メッセージを Post する
//	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_AF_POPUP, 0, 0);
//	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_AF_POPUP, (WPARAM) m_dwAfMode, 0);
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_AF_POPUP, (WPARAM) pEvParams, 0);

	// WAIT 状態にするためトランジションを更新しません．
}

/*
 *	ポップアップ終了 リトライボタン(WAIT 状態無し)
 */
void CChiefTransiAF::enter_ST_AAF_POPUP_RETRY(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[4], (DWORD) pEvParams);

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;
	l_pAFValid->ucResultPopup = 1;		// リトライボタン
	l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(pEvParams)))? 0 : 1;

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_AAF_START);
}

/*
 *	ポップアップ終了 AF 無視ボタン(WAIT 状態無し)
 */
void CChiefTransiAF::enter_ST_AAF_POPUP_IGNORE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[5], (DWORD) pEvParams);

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;
	l_pAFValid->ucResultPopup = 2;		// 閉じる(OK)ボタン
// ココでは設定しません．	l_pAFValid->ucResultAF = 4;
	l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(pEvParams)))? 0 : 1;

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_AAF_IDLE);

	// 呼出元トランジションに完了を通知
	NotifyComplete((cEventParams*) m_dwAFValid);
}

/*
 *	ポップアップ終了 スキップボタン(WAIT 状態無し)
 */
void CChiefTransiAF::enter_ST_AAF_POPUP_SKIP(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[6], (DWORD) pEvParams);

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;
	l_pAFValid->ucResultPopup = 3;		// スキップボタン
// ココでは設定しません．	l_pAFValid->ucResultAF = 4;
	l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(pEvParams)))? 0 : 1;

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_AAF_IDLE);

	// 呼出元トランジションに完了を通知
	NotifyComplete((cEventParams*) m_dwAFValid);
}

/*
 *	ポップアップ終了 キャンセルボタン(WAIT 状態無し)
 */
void CChiefTransiAF::enter_ST_AAF_POPUP_CANCEL(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[7], (DWORD) pEvParams);

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;
	l_pAFValid->ucResultPopup = 4;		// キャンセルボタン
// ココでは設定しません．	l_pAFValid->ucResultAF = 4;
	l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(pEvParams)))? 0 : 1;

	// トランジション更新 -----------------------------------------------------
	TransiEvent(EV_AAF_IDLE);

	// 呼出元トランジションに完了を通知
	NotifyComplete((cEventParams*) m_dwAFValid);
}
