// ChifTransiMaster.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <Active.hxx>
#include "ChifTransiMaster.h"
#include "ChiefExports.h"
#include "nanospec.h"
#include "ChiefView.h"
#include <PatRec.hxx>
#include "MainFrm.h"
#include "ChifTransiSeq.h"
#include "ChifTransiSrRefer.h"
#include "ChifTransiDeskew.h"
#include "ChifTransiAF.h"
#include "ChifTransiSr1Point.h"
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- { ---------- */
#include "ChifTransiStress.h"
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- } ---------- */
/* added 2009.08.07 hmenjo ストレス機能追加(35) ---------- { ---------- */
#include "ChiefPFuncs.h"
/* added 2009.08.07 hmenjo ストレス機能追加(35) ---------- } ---------- */
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
UINT WM_CHIF_TRMAS_SETEVENT = RegisterWindowMessage("WM_CHIF_TRMAS_SETEVENT");	// トランジションイベント発行用ウィンドウメッセージ
UINT WM_CHIF_TRMAS_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TRMAS_HEARTBEAT");	// HeartBeat ウィンドウメッセージ


// ##########################################################################
// class CChiefTransiMaster : マスタ トランジション
// ##########################################################################

// =========================================================================//
//	トランジションテーブル
// =========================================================================//
const Transition TransTable_Master[] =
{
	//	OldState,				EventId,				NewState

	{	ST_MAS_INIT,			EV_MAS_DO_INIT, 		ST_MAS_INIT 			},	// 初期化処理を実行(すぐ EV_MAS_INIT_DONE で)
	{	ST_MAS_INIT,			EV_MAS_INIT_DONE,		ST_MAS_IDLE 			},	// 初期化完了してアイドルへ
	{	ST_MAS_INIT,			EV_MAS_END, 			ST_MAS_END				},	// システム終了処理実行してトランジション終了へ

	{	ST_MAS_IDLE,			EV_MAS_END, 			ST_MAS_END				},	// システム終了処理実行してトランジション終了へ
	{	ST_MAS_IDLE,			EV_MAS_DO_UISEQ,		ST_MAS_UISEQ			},	// 画面 SEQ 測定へ
	{	ST_MAS_IDLE,			EV_MAS_DO_UI1POINT, 	ST_MAS_UI1POINT 		},	// 画面１ポイント測定へ
	{	ST_MAS_IDLE,			EV_MAS_DO_UIAAF,		ST_MAS_UIAAF			},	// 画面 AF へ
	{	ST_MAS_IDLE,			EV_MAS_DO_PIFSEQ,		ST_MAS_PIFSEQ			},	// Pif SEQ 測定へ
	{	ST_MAS_IDLE,			EV_MAS_DO_UISREF,		ST_MAS_UISREF			},	// 画面リファレンス測定へ
// bagus add 2014.01.10 (Stage None対応) -->
	{	ST_MAS_IDLE,			EV_MAS_DO_STDREF,		ST_MAS_STDREF			},	// スタンダードリファレンス測定へ
	{	ST_MAS_IDLE,			EV_MAS_DO_DRKREF,		ST_MAS_DRKREF			},	// ダークリファレンス測定へ
// bagus add 2014.01.10 (Stage None対応) <--

	{	ST_MAS_UISEQ,			EV_MAS_SEQ_DONE,		ST_MAS_UISEQ_DONE		},	// 画面 SEQ 測定完了(すぐ EV_MAS_IDLE で)
	{	ST_MAS_UISEQ_DONE,		EV_MAS_IDLE,			ST_MAS_IDLE 			},	// 完了処理実行してアイドルへ

	{	ST_MAS_UI1POINT,		EV_MAS_UI1POINT_DONE,	ST_MAS_UI1POINT_DONE	},	// 画面１ポイント測定完了(すぐ EV_MAS_IDLE で)
	{	ST_MAS_UI1POINT_DONE,	EV_MAS_IDLE,			ST_MAS_IDLE 			},	// 完了処理実行してアイドルへ
/* added 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
	{	ST_MAS_UI1POINT_DONE,	EV_MAS_DO_UISREF,		ST_MAS_UISREF			},	// 画面リファレンス測定へ
/* added 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */

	{	ST_MAS_UIAAF,			EV_MAS_UIAAF_DONE,		ST_MAS_UIAAF_DONE		},	// 画面 AF 完了(すぐ EV_MAS_IDLE で)
	{	ST_MAS_UIAAF_DONE,		EV_MAS_IDLE,			ST_MAS_IDLE 			},	// 完了処理実行してアイドルへ

	{	ST_MAS_PIFSEQ,			EV_MAS_SEQ_DONE,		ST_MAS_PIFSEQ_DONE		},	// Pif SEQ 測定完了(すぐ EV_MAS_IDLE で)
	{	ST_MAS_PIFSEQ_DONE, 	EV_MAS_IDLE,			ST_MAS_IDLE 			},	// 完了処理実行してアイドルへ

	{	ST_MAS_UISREF,			EV_MAS_UISREF_DONE, 	ST_MAS_UISREF_DONE		},	// 画面リファレンス測定完了(すぐ EV_MAS_IDLE で)
	{	ST_MAS_UISREF_DONE, 	EV_MAS_IDLE,			ST_MAS_IDLE 			},	// 完了処理実行してアイドルへ

// bagus add 2014.01.10 (Stage None対応) -->
	{	ST_MAS_STDREF,			EV_MAS_STDREF_DONE, 	ST_MAS_STDREF_DONE		},	// スタンダードリファレンス測定完了(すぐ EV_MAS_IDLE で)
	{	ST_MAS_STDREF_DONE, 	EV_MAS_IDLE,			ST_MAS_IDLE 			},	// 完了処理実行してアイドルへ

	{	ST_MAS_DRKREF,			EV_MAS_DRKREF_DONE, 	ST_MAS_DRKREF_DONE		},	// ダークリファレンス測定完了(すぐ EV_MAS_IDLE で)
	{	ST_MAS_DRKREF_DONE, 	EV_MAS_IDLE,			ST_MAS_IDLE 			},	// 完了処理実行してアイドルへ
// bagus add 2014.01.10 (Stage None対応) <--

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};

// =========================================================================//
//	状態別処理関数テーブル
// =========================================================================//
BEGIN_STATEPROC_MAP(CChiefTransiMaster)
	/*	0 */	ENTER_STATE(ST_MAS_INIT,			enter_ST_MAS_INIT),
	/*	1 */	ENTER_STATE(ST_MAS_IDLE,			enter_ST_MAS_IDLE),
	/*	2 */	ENTER_STATE(ST_MAS_UISEQ,			enter_ST_MAS_UISEQ),				// (WAIT 状態無しの場合有り)
	/*	3 */	ENTER_STATE(ST_MAS_UISEQ_DONE,		enter_ST_MAS_UISEQ_DONE),			// (WAIT 状態無し)
	/*	4 */	ENTER_STATE(ST_MAS_UI1POINT,		enter_ST_MAS_UI1POINT), 			// (WAIT 状態無しの場合有り)
	/*	5 */	ENTER_STATE(ST_MAS_UI1POINT_DONE,	enter_ST_MAS_UI1POINT_DONE),		// (WAIT 状態無し)
	/*	6 */	ENTER_STATE(ST_MAS_UIAAF,			enter_ST_MAS_UIAAF),				// (WAIT 状態無しの場合有り)
	/*	7 */	ENTER_STATE(ST_MAS_UIAAF_DONE,		enter_ST_MAS_UIAAF_DONE),			// (WAIT 状態無し)
	/*	8 */	ENTER_STATE(ST_MAS_PIFSEQ,			enter_ST_MAS_PIFSEQ),
	/*	9 */	ENTER_STATE(ST_MAS_PIFSEQ_DONE, 	enter_ST_MAS_PIFSEQ_DONE),			// (WAIT 状態無し)
	/* 10 */	ENTER_STATE(ST_MAS_UISREF,			enter_ST_MAS_UISREF),				// (WAIT 状態無しの場合有り)
	/* 11 */	ENTER_STATE(ST_MAS_UISREF_DONE, 	enter_ST_MAS_UISREF_DONE),			// (WAIT 状態無し)
	/* 12 */	ENTER_STATE(ST_MAS_END, 			enter_ST_MAS_END),
// bagus add 2014.01.10 (Stage None対応) -->
	/* 13 */	ENTER_STATE(ST_MAS_STDREF,			enter_ST_MAS_STDREF),				// (WAIT 状態無しの場合有り)
	/* 14 */	ENTER_STATE(ST_MAS_STDREF_DONE, 	enter_ST_MAS_STDREF_DONE),			// (WAIT 状態無し)
	/* 15 */	ENTER_STATE(ST_MAS_DRKREF,			enter_ST_MAS_DRKREF),				// (WAIT 状態無しの場合有り)
	/* 16 */	ENTER_STATE(ST_MAS_DRKREF_DONE, 	enter_ST_MAS_DRKREF_DONE),			// (WAIT 状態無し)
// bagus add 2014.01.10 (Stage None対応) <--
END_STATEPROC_MAP(CChiefTransiMaster)


/////////////////////////////////////////////////////////////////////////////
// CChiefTransiMaster

IMPLEMENT_DYNCREATE(CChiefTransiMaster, CWinThread)

CChiefTransiMaster::CChiefTransiMaster(CWnd* pParent /*=NULL*/) : cActive(TransTable_Master)
{
	TRACE(_T("CChiefTransiMaster::CChiefTransiMaster() \n"));

	m_pcChiefView = (CView*) pParent;

	//	スレッドの起動待ち
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiMaster."));

	//	オブジェクトの自動破棄を禁止
	m_bAutoDelete = FALSE;

}

CChiefTransiMaster::~CChiefTransiMaster()
{
	TRACE(_T("CChiefTransiMaster::~CChiefTransiMaster() \n"));

	//	スレッドの消滅待ち
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiMaster."));
		}
	}
}

BOOL CChiefTransiMaster::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiMaster::InitInstance() \n"));

	// コンストラクタの待ちを解除します
	//		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．
	m_cSyncEvent.SetEvent();

	// 初期状態処理用イベント発行
	TransiEvent(0, 0);

	// 変数初期化
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiMaster::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiMaster::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiMaster, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiMaster)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRMAS_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRMAS_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiMaster メッセージ ハンドラ

/*
 *	トランジションイベント発行 メッセージハンドラ
 */
void CChiefTransiMaster::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	// トランジション間タイミング調整(とりあえずだけど恒久かも・・)

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat メッセージハンドラ
 */
void CChiefTransiMaster::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	トランジションイベント発行用関数
 */
void CChiefTransiMaster::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TRMAS_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TRMAS_SETEVENT, _T("WM_CHIF_TRMAS_SETEVENT"), 5, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
}

/*
 *	HeartBeat 発行用関数
 */
void CChiefTransiMaster::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TRMAS_HEARTBEAT, 0, 0);
}

/*
 *	現在の状態が「アイドル」かどうかをチェック
 *		戻り値：	true	アイドルである
 *					false	アイドルでない
 */
bool CChiefTransiMaster::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_MAS_IDLE) {
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
int CChiefTransiMaster::GetCurrentState()
{
	return CurrState;
}

//--------------------------------------------------------------------------//
//	状態別処理関数群
//--------------------------------------------------------------------------//
/*
 *	初期状態
 */
void CChiefTransiMaster::enter_ST_MAS_INIT(const cEventParams* const pEvParams)
{
// deleted hmenjo 2009.01.23 ---- { ----
//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_INIT);
// deleted hmenjo 2009.01.23 ---- } ----
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[0], (DWORD) pEvParams);

	BOOL	l_bInitialized = TRUE;
	CMainFrame* l_pcMainFrame = (CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame);

#if 0		// ここの初期化処理があると NanoSpec.exe 正常に終了出来ないため，元々の MainFrame に戻しました --------------------
	// XMP 起動(Adap 起動) ----------------------------------------------------
	STARTUPINFO l_StartupInfoi;
	PROCESS_INFORMATION l_ProcessInfo;
	memset(&l_StartupInfoi, 0, sizeof(l_StartupInfoi));
	l_StartupInfoi.cb = sizeof(l_StartupInfoi);
	l_StartupInfoi.wShowWindow = SW_MINIMIZE;		// 最小化で起動
	memset(&l_ProcessInfo, 0, sizeof(l_ProcessInfo));
	// 子プロセスを起動
	if (0 == ::CreateProcess(NULL, ADAP_EXE_PATH, NULL, NULL, FALSE, 0, NULL, NULL, &l_StartupInfoi, &l_ProcessInfo)) {
		// 起動失敗
		l_bInitialized = FALSE;
		m_pcChiefView->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(5, CHRAMSG_YESNO)), MAKELPARAM(19, 0));
		// return;	ココで return する必要があるか？？
	}
	// 子プロセスのハンドル(プロセスとスレッド)をクローズ
	CloseHandle(l_ProcessInfo.hProcess);
	CloseHandle(l_ProcessInfo.hThread);

	// パターン認識を初期化 -----------------------------------------------
	SYSTEM_CONFIG *l_SystemConfig = &(((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_SystemConfig);
	ConfigFile_GetNanoSpecIni(l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	if (( l_SystemConfig->nPRMethod > 0 && l_SystemConfig->bSamplePatternRec )
	 || ( l_SystemConfig->nPRMethod > 0 && l_SystemConfig->bSitePatternRec)) {
		if (0 == PatRecInit()) {
			// 初期化失敗
			l_bInitialized = FALSE;
			m_pcChiefView->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(5, CHRAMSG_YESNO)), MAKELPARAM(20, 0));
			// return;	ココで return する必要があるか？？
		}
	}

	// 実機の場合のみ起動 -----------------------------------------------------
	if (0 == l_pcMainFrame->HardwareSimulation()) {
#if 0	// hmenjo-deleted-->
		// hmenjo 20081024 パターン認識はハードシミュレーションでも動作するようになったようなので上記に移動です．
		// パターン認識を初期化 -----------------------------------------------
		if (0 == PatRecInit()) {
			// 初期化失敗
			l_bInitialized = FALSE;
			m_pcChiefView->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(5, CHRAMSG_YESNO)), MAKELPARAM(20, 0));
			// return;	ココで return する必要があるか？？
		}
#endif	// hmenjo-deleted<--
		// ステージモジュールを初期化 -----------------------------------------
		if (0 == StageInitialize()) {
			// 初期化失敗
			l_bInitialized = FALSE;
			m_pcChiefView->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(5, CHRAMSG_YESNO)), MAKELPARAM(21, 0));
			// return;	ココで return する必要があるか？？
		} else {
			((CChiefView*) m_pcChiefView)->m_dwModuleState_Stage = 1;	// 初期化完了(Initialized)
		}
		// ターレットを初期設定 -----------------------------------------------
		//		レンズ初期化未。初期値がどこにくるか不明である。
		//		レンズ選択が１度もされていない（StageSelectLens()）場合、カレントレンズ不明。
		//		stage.dllは、この場合、-1を返却してくる。
		//		よって、バグのもとになるので、強制選択する。
		SR_TURRET l_srTurret[SR_LENS_MAX];
		ConfigFile_GetNanoSpecIni(l_srTurret, CONFIG_FILE_SR_TURRET);
		for ( int i = 0; i < SR_LENS_MAX; i++ ) {
			if ( l_srTurret[i].bEnable ) {
				StageSelectLensNotify(i);
				break;
			}
		}
	}

	// オーバレイモジュールを初期化 -------------------------------------------
	OverlayInitInstance();

	// 測定モジュールを初期化 -------------------------------------------------
	if (0 == MEAS_Initialize(l_pcMainFrame->HardwareSimulation())) {
		// 初期化失敗
		l_bInitialized = FALSE;
		m_pcChiefView->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(5, CHRAMSG_YESNO)), MAKELPARAM(22, 0));
		// return;	ココで return する必要があるか？？
	} else {
		((CChiefView*) m_pcChiefView)->m_dwModuleState_SR_Meas = 1; // 初期化完了(Initialized)
	}
#endif		// ここの初期化処理があると NanoSpec.exe 正常に終了出来ないため，元々の MainFrame に戻しました --------------------

// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 2013.02.22 bagus Substrate thickness setting -->
		if (((CChiefView*) m_pcChiefView)->SelectSubstrateThickIndex(0)) {	// 板厚設定には、リファレンスを指定する
			((CChiefView*) m_pcChiefView)->LogChief(_T("Select Reference Sub Thickness failed."));
		}
// 2013.02.22 bagus Substrate thickness setting <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	// トランジション更新 -----------------------------------------------------
	if (0 != l_bInitialized) {
		// 他のトランジションに初期化終了イベントを発行します -----------------
		// シーケンス測定 トランジション
		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_INIT_DONE);
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- { ---------- */
		// ストレス トランジション
		((CChiefTransiStress*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress)->TransiEvent(EV_STRS_INIT_DONE);
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- } ---------- */
		// リファレンス測定 トランジション
		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_INIT_DONE);
		// デスキュー トランジション
		((CChiefTransiDeskew*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew)->TransiEvent(EV_DSKW_INIT_DONE);
		// AF トランジション
		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_INIT_DONE);
		// １ポイント測定 トランジション
		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_INIT_DONE);

		// IDLE へ遷移 --------------------------------------------------------
// deleted hmenjo 2009.01.23 ---- { ----
//		((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);
// deleted hmenjo 2009.01.23 ---- } ----
		TransiEvent(EV_MAS_INIT_DONE);
	}

	// マスタトランジション初期化完了イベントをセット -------------------------
	BOOL l_bRc = ::SetEvent(((CChiefView*) m_pcChiefView)->m_hEvTrMaster);

}

/*
 *	アイドル
 */
void CChiefTransiMaster::enter_ST_MAS_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[1], (DWORD) pEvParams);

/* added 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
	m_bGTR1PRefer = FALSE;
/* added 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */

	// WAIT 状態にするため処理はありません．
}

/*
 *	画面 SEQ 測定(WAIT 状態無しの場合有り)
 */
void CChiefTransiMaster::enter_ST_MAS_UISEQ(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOWORD(pEvParams)：リファレンス測定，デスキュー，パスモード(リメジャーモード)
/*										＝０：パスしない
/*										≠０：パスする
/*				HIWORD(pEvParams)：リファレンスモード
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[2], (DWORD) pEvParams);

/* added 2009.09.11 hmenjo 画面へガントリ透過率 Refer 結果通知 ---------- { ---------- */
	m_bReferMode = (0 != HIWORD(pEvParams))? TRUE : FALSE;
/* added 2009.09.11 hmenjo 画面へガントリ透過率 Refer 結果通知 ---------- } ---------- */

/* modified 2009.08.03 hmenjo ストレス機能追加(10) ---------- { ---------- */
//	if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
//		// SEQ トランジションがビジーでした．
//		// トランジション更新
//		TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) TR_MAS_TRDSKW_BUSY);
//	} else {
//		// シーケンス測定 発行
//		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(TRANSI_MASTER, pEvParams));
//		// WAIT 状態にするためトランジションを更新しません．
//	}
/* modified 2009.08.03 hmenjo ストレス機能追加(10) ----------			   */
	CHIEF_PFUNCS l_ChiefPFuncs;
	WORD l_wHeadType;
	if (0 == PFC_FuncSet((CChiefView*) m_pcChiefView, &l_ChiefPFuncs, &l_wHeadType)) {
		this->TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) TR_MAS_INVALID_HEAD);
		return;
	}
	int l_iErrCode = 0;
	switch (l_wHeadType) {
	case HEAD_TYPE_SR:		l_iErrCode = TR_MAS_TRSEQ_BUSY; 	break;
	case HEAD_TYPE_STRESS:	l_iErrCode = TR_MAS_TRSTRS_BUSY;	break;
/* modified 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- { ---------- */
//	case HEAD_TYPE_SE:
/* modified 2009.08.26 hmenjo SE Seq 起動条件追加 ----------			  */
	case HEAD_TYPE_SE:		l_iErrCode = TR_MAS_TRSEQ_BUSY; 	break;
/* modified 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- } ---------- */
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:l_iErrCode = TR_MAS_TRSEQ_BUSY; 	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	case HEAD_TYPE_IRSE:
	case HEAD_TYPE_MS:
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:
	case HEAD_TYPE_CTA:
	default:
		//2009.11.02 bagus MS --{--
			l_iErrCode = TR_MAS_TRSEQ_BUSY;
		//2009.11.02 bagus MS --}--
		break;
	}
	if (false == (*l_ChiefPFuncs.IsIdle)((CChiefView*) m_pcChiefView)) {
		/* SEQ トランジションがビジーでした．	*/
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiMaster::enter_ST_MAS_UISEQ() - SR or Stress Seq is busy."));
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		/* トランジション更新	*/
		this->TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) l_iErrCode);
	} else {
		/* シーケンス測定 発行	*/
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
///* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
////		(*l_ChiefPFuncs.TransiEvent)((CChiefView*) m_pcChiefView, l_ChiefPFuncs.Event.iSTART, (cEventParams*) MAKELONG(TRANSI_MASTER, pEvParams));
///* modified 2009.09.09 hmenjo 引数変更 ---------- 			 */
//		(*l_ChiefPFuncs.TransiEvent)((CChiefView*) m_pcChiefView, l_ChiefPFuncs.Event.iSTART, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, 0), pEvParams));
///* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
/* modified 2009.09.10 hmenjo P511 コマンド追加 ----------				*/
		(*l_ChiefPFuncs.TransiEvent)((CChiefView*) m_pcChiefView, l_ChiefPFuncs.Event.iSTART, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, m_bReferMode), LOWORD(pEvParams)));
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
		/* WAIT 状態にするためトランジションを更新しません．	*/
	}
/* modified 2009.08.03 hmenjo ストレス機能追加(10) ---------- } ---------- */
}

/*
 *	画面 SEQ 測定完了(WAIT 状態無し)
 */
void CChiefTransiMaster::enter_ST_MAS_UISEQ_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[3], (DWORD) pEvParams);

	DWORD l_ErrCode = (DWORD) pEvParams;

/* added 2009.09.11 hmenjo 画面へガントリ透過率 Refer 結果通知 ---------- { ---------- */
	if (0 != m_bReferMode) {
		// 画面へ終了コードを通知
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- { ---------- */
		TCHAR l_tszLogMsg[128];
		_stprintf(l_tszLogMsg, _T("Notify WM_CHIF_SRREF_END with errcode(0x%08x)."), l_ErrCode);
		((CChiefView*) m_pcChiefView)->LogChief(l_tszLogMsg);
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- } ---------- */
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_SRREF_END, l_ErrCode, 0);
	}
/* added 2009.09.11 hmenjo 画面へガントリ透過率 Refer 結果通知 ---------- } ---------- */

	TransiEvent(EV_MAS_IDLE);
}

/*
 *	画面１ポイント測定(WAIT 状態無しの場合有り)
 */
void CChiefTransiMaster::enter_ST_MAS_UI1POINT(const cEventParams* const pEvParams)
/*				LOWORD(pEvParams)：ポイント番号
 *				HIWORD(pEvParams)：リファレンスモード			*/
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[4], (DWORD) pEvParams);

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(7, 5)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiMaster::enter_ST_MAS_UI1POINT() - Sr1Point Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// １ポイント測定 トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_MAS_UI1POINT_DONE, (cEventParams*) TR_MAS_TRSR1P_BUSY);
	} else {
//		((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);
#if 0		// --> １ポイント測定の単独動作時は画面側で行うため不要[2009.01.07]
		// ターレット制御(測定用：測定プログラムの Lens)
		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
		((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
#endif		// <-- １ポイント測定の単独動作時は画面側で行うため不要[2009.01.07]
#if 0		// --> 不要になりました．[2009.01.07]
		// ステージプログラム内のスキャン数を操作します
		LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
		l_pStageProgInfoHdr->wNumScans = 1;
#endif		// <-- 不要になりました．[2009.01.07]
		// １ポイント測定 発行
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
////		DWORD l_dwParam = MAKELONG(TRANSI_MASTER, 1);
////		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_START, (cEventParams*) l_dwParam);
//		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_START, (cEventParams*) MAKELONG(TRANSI_MASTER, 1));
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
// 2009.09.25 K.Matsuo CTA -->
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// 		LPCTSTR l_pszLotId = ((CChiefView*) m_pcChiefView)->m_szLotID;
// 		LPCTSTR l_pszSampleID = ((CChiefView*) m_pcChiefView)->m_szSampleID;
		TCHAR l_szLotID[256];
		TCHAR l_szSampleID[256];
		/* 画面から設定された	*/
		if (( (CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->IsSetSampleIdFromUI() ) {
			((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetSampleId(l_szSampleID);
			((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetLotId(l_szLotID);
		}
		else {
			strcpy(l_szLotID, "");
			strcpy(l_szSampleID, "");
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		LPCTSTR l_pszMainRecipeName = ((CChiefView*) m_pcChiefView)->GetMainRecipeNamePtr();
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo;
		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo;
		LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//		MEAS_PrepareForMeasure(l_pszLotId, l_pszSampleID, l_pszMainRecipeName, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgInfoHdr);
		MEAS_PrepareForMeasure(l_szLotID, l_szSampleID, l_pszMainRecipeName, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgInfoHdr);
// 2009.09.25 K.Matsuo CTA <--
/* added 2009.11.30 hmenjo 単体起動では PMA 無し ---------- { ---------- */
		/* 単体起動(手動)時は PMA を無しにします．	*/
		l_pMainRcpInfo->MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_NONE;
		/* 単体起動(手動)時は AF を無しにします．	*/
		switch (l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
		case HEAD_TYPE_SR:
		case HEAD_TYPE_STRESS:	l_pMainRcpInfo->MainRcpParam._SR.nFocus = 0;		break;
		case HEAD_TYPE_SE:		l_pMainRcpInfo->MainRcpParam._SE.nFocus = 0;		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:l_pMainRcpInfo->MainRcpParam._COMPEASE.nFocus = 0;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		case HEAD_TYPE_MS:		l_pMainRcpInfo->MainRcpParam._MScope.nFocus = 0;	break;
		case HEAD_TYPE_4PP: 	l_pMainRcpInfo->MainRcpParam._RS.nFocus = 0;		break;
		case HEAD_TYPE_CTA: 	l_pMainRcpInfo->MainRcpParam._CA.nFocus = 0;		break;
		}
/* added 2009.11.30 hmenjo 単体起動では PMA 無し ---------- } ---------- */
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
///* modified 2009.11.19 hmenjo CTA 1ポイント単体起動時 Head Offset追加 ---------- { ---------- */
////		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, 0), 1));
///* modified 2009.11.19 hmenjo CTA 1ポイント単体起動時 Head Offset追加 ----------				*/
//		MEASMODE_ID l_enMeasMode;
//		switch (l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//		case HEAD_TYPE_SR:	l_enMeasMode = MEASMODE_NORM;	break;
//		case HEAD_TYPE_CTA: l_enMeasMode = MEASMODE_CTA;	break;
//		case HEAD_TYPE_MS:	l_enMeasMode = MEASMODE_MS; 	break;
//		default:			l_enMeasMode = MEASMODE_NORM;	break;
//		}
///* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- { ---------- */
////		DWORD l_dwParam = MAKELONG(MAKEWORD(TRANSI_MASTER, l_enMeasMode), 1);
///* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- 			 */
//		DWORD l_dwParam = MAKELONG(MAKEWORD(TRANSI_MASTER, l_enMeasMode), pEvParams);
///* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- } ---------- */
//		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_START, (cEventParams*) l_dwParam);
///* modified 2009.11.19 hmenjo CTA 1ポイント単体起動時 Head Offset追加 ---------- } ---------- */
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- 			 */
		MEASMODE_ID l_enMeasMode;
		switch (l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
		case HEAD_TYPE_SR:
			if ((MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType) && (0 != HIWORD(pEvParams))) {
				/* GTR でリファレンスモード指定の場合	*/
				l_enMeasMode = MEASMODE_GTRREF;
				m_bGTR1PRefer = TRUE;
			} else {
				l_enMeasMode = MEASMODE_NORM;
			}
			break;
		case HEAD_TYPE_CTA: l_enMeasMode = MEASMODE_CTA;	break;
		case HEAD_TYPE_MS:	l_enMeasMode = MEASMODE_MS; 	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:l_enMeasMode = MEASMODE_COMPEASE;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		default:			l_enMeasMode = MEASMODE_NORM;	break;
		}
		DWORD l_dwParam = MAKELONG(MAKEWORD(TRANSI_MASTER, l_enMeasMode), LOWORD(pEvParams));
		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_START, (cEventParams*) l_dwParam);
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	画面１ポイント測定完了(WAIT 状態無し)
 */
void CChiefTransiMaster::enter_ST_MAS_UI1POINT_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[5], (DWORD) pEvParams);

	DWORD l_ErrCode = (DWORD) pEvParams;

/* modified 2009.11.30 hmenjo １ポイント測定失敗時のエラー終了通知 ---------- { ---------- */
//	// １枚測定終了は通知しません．
//	//		１ポイント測定完了の通知を使ってください．
/* modified 2009.11.30 hmenjo １ポイント測定失敗時のエラー終了通知 ----------			   */
	if (0 != (0x80000000 & l_ErrCode)) {
		/* １ポイント測定失敗時のエラー終了通知 */
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_MEAS_END, 1, 0);
		((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_MEAS_END with 1 point NG."));
		m_bGTR1PRefer = FALSE;
	}
/* modified 2009.11.30 hmenjo １ポイント測定失敗時のエラー終了通知 ---------- } ---------- */

/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
///* added 2009.11.19 hmenjo 1ポイント単体起動時 SR フィルタ開 ---------- { ---------- */
//	  /* SR フィルタを開にします．	  */
//	  if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
//		  /* 異常時でも無視します．   */
//		  ((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiMaster::enter_ST_MAS_UI1POINT_DONE()."));
//	  }
///* added 2009.11.19 hmenjo 1ポイント単体起動時 SR フィルタ開 ---------- } ---------- */
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	if (HEAD_TYPE_MS != l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
		/* MS ヘッド以外の場合	*/
		/* SR フィルタを開にします．	*/
		if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
			/* 異常時でも無視します．	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiMaster::enter_ST_MAS_UI1POINT_DONE()."));
		}
	}
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);

/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
//	TransiEvent(EV_MAS_IDLE);
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- 			 */
	if (TRUE == m_bGTR1PRefer) {
		/* GTR １ポイントリファレンス用に，ダークを測定するためです．	*/
		TransiEvent(EV_MAS_DO_UISREF);
	} else {
		TransiEvent(EV_MAS_IDLE);
	}
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */
}

/*
 *	画面 AF(WAIT 状態無しの場合有り)
 */
void CChiefTransiMaster::enter_ST_MAS_UIAAF(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[6], (DWORD) pEvParams);

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(3, 5)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiMaster::enter_ST_MAS_UIAAF() - AF Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// AF トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_MAS_UIAAF_DONE, (cEventParams*) TR_MAS_TRAAF_BUSY);
	} else {
		if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
			// ハードシミュレートモードの場合は，何もせず終了
#ifdef CHIEF_USE_SLEEP
			::Sleep(1000);
#endif
			TransiEvent(EV_MAS_UIAAF_DONE, 0);
		} else {
			((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//			((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(TRANSI_MASTER, 0));
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
			((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, 0), 0));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
			// WAIT 状態にするためトランジションを更新しません．
		}
	}
}

/*
 *	画面 AF 完了(WAIT 状態無し)
 */
void CChiefTransiMaster::enter_ST_MAS_UIAAF_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[7], (DWORD) pEvParams);

	DWORD l_ErrCode = (DWORD) pEvParams;

	// 画面へ終了コードを通知
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_AAF_END, l_ErrCode, 0);

	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);

	TransiEvent(EV_MAS_IDLE);
}

/*
 *	Pif SEQ 測定(WAIT 状態無しの場合有り)
 */
void CChiefTransiMaster::enter_ST_MAS_PIFSEQ(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOWORD(pEvParams)：リファレンス測定，デスキュー，パスモード(リメジャーモード)
/*										＝０：パスしない
/*										≠０：パスする
/*				HIWORD(pEvParams)：リファレンスモード
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[8], (DWORD) pEvParams);

/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- { ---------- */
	m_bReferMode = (0 != HIWORD(pEvParams))? TRUE : FALSE;
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- } ---------- */

	// トランジションのビジーチェックは Pif 側で行っています
	// シーケンス測定 発行
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
///* modified 2009.07.31 hmenjo ストレス機能追加(10) ---------- { ---------- */
////	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(TRANSI_MASTER, pEvParams));
///* modified 2009.07.31 hmenjo ストレス機能追加(10) ---------- 			 */
//	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo;
//	switch (l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
//	case HEAD_TYPE_SR:
///* added 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- { ---------- */
//	case HEAD_TYPE_SE:
///* added 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- } ---------- */
///* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
////		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(TRANSI_MASTER, pEvParams));
///* modified 2009.09.09 hmenjo 引数変更 ---------- 			 */
//		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, 0), pEvParams));
///* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
//		break;
//	case HEAD_TYPE_STRESS:
///* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
////		((CChiefTransiStress*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress)->TransiEvent(EV_STRS_START, (cEventParams*) MAKELONG(TRANSI_MASTER, pEvParams));
///* modified 2009.09.09 hmenjo 引数変更 ---------- 			 */
//		((CChiefTransiStress*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress)->TransiEvent(EV_STRS_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, 0), pEvParams));
///* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
//		break;
///* deleted 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- { ---------- */
////	case HEAD_TYPE_SE:
///* deleted 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- } ---------- */
//	case HEAD_TYPE_IRSE:
//	case HEAD_TYPE_4PP:
//	case HEAD_TYPE_CTA:
//	default:
//		this->TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) TR_MAS_INVALID_HEAD);
//		break;
//	}
///* modified 2009.07.31 hmenjo ストレス機能追加(10) ---------- } ---------- */
/* modified 2009.09.10 hmenjo P511 コマンド追加 ----------				*/
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo;
	//2009.11.07 bagus 2point-distacen --{--
	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO)((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo;
	//2009.11.07 bagus 2point-distacen --}--
	switch (l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
	case HEAD_TYPE_SR:
		//2009.11.07 bagus 2point-distance --{--
		//2点間距離はリモートでは行えない
		if(l_pMeasProgInfo->ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			this->TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) TR_MAS_INVALID_HEAD);
			break;
		}
		//2009.11.07 bagus 2point-distance --}--

	case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
/* added 2009.11.09 hmenjo CTA Pif 対応 ---------- { ---------- */
	case HEAD_TYPE_CTA:
/* added 2009.11.09 hmenjo CTA Pif 対応 ---------- } ---------- */
/* added 2009.11.09 K.Matsuo Resist Pif 対応 ---------- { ---------- */
	case HEAD_TYPE_4PP:
/* added 2009.11.09 K.Matsuo Resist Pif 対応 ---------- } ---------- */
		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, HIWORD(pEvParams)), LOWORD(pEvParams)));
		break;
	case HEAD_TYPE_STRESS:
		((CChiefTransiStress*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress)->TransiEvent(EV_STRS_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, 0), LOWORD(pEvParams)));
		break;
// 2009.10.19 bagus MS 追加 --{--
//	case HEAD_TYPE_IRSE:
	case HEAD_TYPE_MS:
// 2009.10.19 bagus MS 追加 --}--
/* deleted 2009.11.09 K.Matsuo Resist Pif 対応 ---------- { ---------- */
//	case HEAD_TYPE_4PP:
/* deleted 2009.11.09 K.Matsuo Resist Pif 対応 ---------- } ---------- */
/* deleted 2009.11.09 hmenjo CTA Pif 対応 ---------- { ---------- */
//	case HEAD_TYPE_CTA:
/* deleted 2009.11.09 hmenjo CTA Pif 対応 ---------- } ---------- */
	default:
		this->TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) TR_MAS_INVALID_HEAD);
		break;
	}
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
	// WAIT 状態にするためトランジションを更新しません．
}

/*
 *	Pif SEQ 測定完了(WAIT 状態無し)
 */
void CChiefTransiMaster::enter_ST_MAS_PIFSEQ_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[9], (DWORD) pEvParams);

	DWORD l_ErrCode = (DWORD) pEvParams;

/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- { ---------- */
	if (0 != m_bReferMode) {
		// 画面へ終了コードを通知
		TCHAR l_tszLogMsg[128];
		_stprintf(l_tszLogMsg, _T("Notify WM_CHIF_SRREF_END with errcode(0x%08x)."), l_ErrCode);
		((CChiefView*) m_pcChiefView)->LogChief(l_tszLogMsg);
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_SRREF_END, l_ErrCode, 0);
	}
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- } ---------- */

	TransiEvent(EV_MAS_IDLE);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(""));	// Status Bar
}

/*
 *	画面リファレンス測定(WAIT 状態無しの場合有り)
 */
void CChiefTransiMaster::enter_ST_MAS_UISREF(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[10], (DWORD) pEvParams);

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(8, 5)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiMaster::enter_ST_MAS_UISREF() - SrRefer Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// リファレンス測定 トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_MAS_UISREF_DONE, (cEventParams*) TR_MAS_TRSREF_BUSY);
	} else {
//		((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_START, (cEventParams*) TRANSI_MASTER);
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER, 0), 0));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	画面リファレンス測定完了(WAIT 状態無し)
 */
void CChiefTransiMaster::enter_ST_MAS_UISREF_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[11], (DWORD) pEvParams);

	DWORD l_ErrCode = (DWORD) pEvParams;

	// 画面へ終了コードを通知
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- { ---------- */
	TCHAR l_tszLogMsg[128];
	_stprintf(l_tszLogMsg, _T("Notify WM_CHIF_SRREF_END with errcode(0x%08x)."), l_ErrCode);
	((CChiefView*) m_pcChiefView)->LogChief(l_tszLogMsg);
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- } ---------- */
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_SRREF_END, l_ErrCode, 0);

//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);

	TransiEvent(EV_MAS_IDLE);
}

/*
 *	トランジション終了
 */
void CChiefTransiMaster::enter_ST_MAS_END(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[12], (DWORD) pEvParams);

#if 0		// ここの初期化処理があると NanoSpec.exe 正常に終了出来ないため，元々の MainFrame に戻しました --------------------
	// SR 測定モジュールを終了 ------------------------------------------------
	MEAS_Shutdown();
	((CChiefView*) m_pcChiefView)->m_dwModuleState_SR_Meas = 9; // 終了(Terminated)

	// 実機の場合のみ終了 -----------------------------------------------------
	if (0 == ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		// パターン認識を終了 -------------------------------------------------
		PatRecEnd();
	}

	// オーバレイモジュールを終了 ---------------------------------------------
	OverlayExitInstance();

	// ステージモジュールを終了 -----------------------------------------------
	//		ステージモジュールの終了処理は不要です．
#endif		// ここの初期化処理があると NanoSpec.exe 正常に終了出来ないため，元々の MainFrame に戻しました --------------------

	// トランジション更新 -----------------------------------------------------
	//		次のトランジションはありません．

	// マスタトランジション終了完了イベントをセット -------------------------
	BOOL l_bRc = ::SetEvent(((CChiefView*) m_pcChiefView)->m_hEvTrMaster);

}

// bagus 2014.01.10 Add(Stage None対応) -->
/*
 *	スタンダードリファレンス測定(WAIT 状態無しの場合有り)
 */
void CChiefTransiMaster::enter_ST_MAS_STDREF(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[13], (DWORD) pEvParams);

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(8, 5)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiMaster::enter_ST_MAS_STDREF() - SrRefer Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// リファレンス測定 トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_MAS_STDREF_DONE, (cEventParams*) TR_MAS_TRSREF_BUSY);
	} else {
//		((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);
		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_STD_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER_STD, 0), 0));
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	スタンダードリファレンス測定完了(WAIT 状態無し)
 */
void CChiefTransiMaster::enter_ST_MAS_STDREF_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[14], (DWORD) pEvParams);

	DWORD l_ErrCode = (DWORD) pEvParams;

	// 画面へ終了コードを通知
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- { ---------- */
	TCHAR l_tszLogMsg[128];
	_stprintf(l_tszLogMsg, _T("Notify WM_CHIF_SRREF_END with errcode(0x%08x)."), l_ErrCode);
	((CChiefView*) m_pcChiefView)->LogChief(l_tszLogMsg);
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- } ---------- */
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_SRREF_END, l_ErrCode, 0);

//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);

	TransiEvent(EV_MAS_IDLE);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(""));	// Status Bar

	//PifComm_CompleteSiReferenceDoneReport(l_ErrCode);	// P515
	if(l_ErrCode != 0){
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_PIF_REPORT_SI_REF_COMP, 1, 0);
	}
	else{
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_PIF_REPORT_SI_REF_COMP, 0, 0);
	}
}

/*
 *	ダークリファレンス測定(WAIT 状態無しの場合有り)
 */
void CChiefTransiMaster::enter_ST_MAS_DRKREF(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[15], (DWORD) pEvParams);

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(8, 5)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiMaster::enter_ST_MAS_DRKREF() - SrRefer Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// リファレンス測定 トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_MAS_DRKREF_DONE, (cEventParams*) TR_MAS_TRSREF_BUSY);
	} else {
//		((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);
		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_DRK_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_MASTER_DRK, 0), 0));
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	ダークリファレンス測定完了(WAIT 状態無し)
 */
void CChiefTransiMaster::enter_ST_MAS_DRKREF_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_MASTER_DISP[16], (DWORD) pEvParams);

	DWORD l_ErrCode = (DWORD) pEvParams;

	// 画面へ終了コードを通知
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- { ---------- */
	TCHAR l_tszLogMsg[128];
	_stprintf(l_tszLogMsg, _T("Notify WM_CHIF_SRREF_END with errcode(0x%08x)."), l_ErrCode);
	((CChiefView*) m_pcChiefView)->LogChief(l_tszLogMsg);
/* added 2009.09.14 hmenjo SrRef リファレンス完了にログ追加 ---------- } ---------- */
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_SRREF_END, l_ErrCode, 0);

//	((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);

	TransiEvent(EV_MAS_IDLE);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(""));	// Status Bar

	//PifComm_CompleteDarkReferenceDoneReport(l_ErrCode);	// P517
	if(l_ErrCode != 0){
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_PIF_REPORT_DARK_REF_COMP, 1, 0);
	}
	else{
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_PIF_REPORT_DARK_REF_COMP, 0, 0);
	}
}
// bagus 2014.01.10 Add(Stage None対応) <--
