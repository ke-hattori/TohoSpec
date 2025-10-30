// ChifTransiSr1Point.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <Active.hxx>
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- { ---------- */
//#include "ChifTransiSr1Point.h"
//#include "ChiefExports.h"
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ----------				*/
#include "ChiefExports.h"
#include "ChifTransiSr1Point.h"
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- } ---------- */
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "ChiefView.h"
#include "ChifTransiMaster.h"
#include "ChifTransiSeq.h"
#include "ChifTransiAF.h"
#include "SitePtSeq.h"
#include "..\\..\\INC\\NSStage.hxx"
/* added 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- { ---------- */
#include "..\\..\\INC\\NEXIOBASE.HXX"
/* added 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- } ---------- */

// XMP用
#define DDE_SERVICE 			 		_T("jaw_adap")
#define DDE_TOPIC				 		_T("dde")

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
UINT WM_CHIF_TR1P_SETEVENT = RegisterWindowMessage("WM_CHIF_TR1P_SETEVENT");	// トランジションイベント発行用ウィンドウメッセージ
UINT WM_CHIF_TR1P_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TR1P_HEARTBEAT");	// HeartBeat ウィンドウメッセージ


// ##########################################################################
// class CChiefTransiSr1Point : １ポイント測定 トランジション
// ##########################################################################

// =========================================================================//
//	トランジションテーブル
// =========================================================================//
const Transition TransTable_Sr1Point[] =
{
	//	OldState,					EventId,					NewState

	{	ST_SR1P_INIT,				EV_SR1P_DO_INIT,			ST_SR1P_INIT			},	// 初期化処理を実行(すぐ EV_SR1P_INIT_DONE で)
	{	ST_SR1P_INIT,				EV_SR1P_INIT_DONE,			ST_SR1P_IDLE			},	// 初期化完了してアイドルへ

// 2009.05.11 K.Matsuo SitePR -->
//	{	ST_SR1P_IDLE, 				EV_SR1P_START,				ST_SR1P_ZMOVE				},	// 測定ポイントＺ移動完了待ち へ
	{	ST_SR1P_IDLE, 				EV_SR1P_START,				ST_SR1P_SITEPR_START		},
	{	ST_SR1P_SITEPR_START, 		EV_SR1P_SITEPR_NONE,		ST_SR1P_ZMOVE				},	// 測定ポイントＺ移動完了待ち へ
	{	ST_SR1P_SITEPR_START, 		EV_SR1P_SITEPR_AUTO,		ST_SR1P_SITEPR_AUTO			},	//

	{	ST_SR1P_SITEPR_AUTO, 		EV_SR1P_SITEPR_AUTO_OK,		ST_SR1P_SITEPR_AUTO_OK		},	//
	{	ST_SR1P_SITEPR_AUTO_OK, 	EV_SR1P_SITEPR_PREP_MEAS,	ST_SR1P_SITEPR_PREP_MEAS	},	//
	{	ST_SR1P_SITEPR_AUTO_OK, 	EV_SR1P_AF_START,			ST_SR1P_AF					},	// レシピレンズに戻してAF（AF機能にレシピフィルタ戻し付き）
	{	ST_SR1P_SITEPR_AUTO, 		EV_SR1P_SITEPR_AUTO_PROCEED,ST_SR1P_SITEPR_AUTO_PROCEED	},	// 強制測定（AFした後、ステージ移動なし）
	{	ST_SR1P_SITEPR_AUTO_PROCEED,EV_SR1P_SITEPR_PREP_MEAS,	ST_SR1P_SITEPR_PREP_MEAS	},	//
	{	ST_SR1P_SITEPR_AUTO_PROCEED,EV_SR1P_AF_START,			ST_SR1P_AF					},	//
	{	ST_SR1P_SITEPR_AUTO, 		EV_SR1P_SITEPR_AUTO_PROCEED_PRESTEP,						// 強制測定（AFした後、ステージ移動あり）
																ST_SR1P_SITEPR_AUTO_PROCEED_PRESTEP	},	//
	{	ST_SR1P_SITEPR_AUTO_PROCEED_PRESTEP,
									EV_SR1P_AF_START,			ST_SR1P_AF					},	//
	{	ST_SR1P_SITEPR_AUTO, 		EV_SR1P_SITEPR_AUTO_CANCEL,	ST_SR1P_SITEPR_AUTO_CANCEL	},	//
	{	ST_SR1P_SITEPR_AUTO_CANCEL,	EV_SR1P_IDLE,				ST_SR1P_IDLE				},	//
	{	ST_SR1P_SITEPR_AUTO, 		EV_SR1P_SITEPR_AUTO_MANU,	ST_SR1P_SITEPR_MANU			},	//

	{	ST_SR1P_SITEPR_MANU, 		EV_SR1P_SITEPR_MANU_OK,		ST_SR1P_SITEPR_MANU_OK		},	//
	{	ST_SR1P_SITEPR_MANU_OK,		EV_SR1P_SITEPR_PREP_MEAS,	ST_SR1P_SITEPR_PREP_MEAS	},	//
	{	ST_SR1P_SITEPR_MANU_OK,		EV_SR1P_AF_START,			ST_SR1P_AF					},	//
	{	ST_SR1P_SITEPR_MANU, 		EV_SR1P_SITEPR_MANU_CANCEL,	ST_SR1P_SITEPR_MANU_CANCEL	},	//
	{	ST_SR1P_SITEPR_MANU_CANCEL,	EV_SR1P_IDLE,				ST_SR1P_IDLE				},	//

	{	ST_SR1P_SITEPR_PREP_MEAS, 	EV_SR1P_MEAS_START,			ST_SR1P_MEAS			},	// SitePR終了 → 測定前の準備処理（レシピカラーフィルタに戻すなど）
// 2009.05.11 K.Matsuo SitePR <--

	{	ST_SR1P_ZMOVE, 				EV_SR1P_MEAS_START,			ST_SR1P_MEAS			},	// ハードシミュレートのため 測定完了待ち へ
	{	ST_SR1P_ZMOVE, 				EV_SR1P_ZMOVE_FAIL,			ST_SR1P_IDLE			},	// 測定ポイントＺ軸への移動開始失敗
	{	ST_SR1P_ZMOVE, 				EV_SR1P_ZMOVE_DONE,			ST_SR1P_MEAS			},	// 測定ポイントＺ軸移動完了
	{	ST_SR1P_ZMOVE, 				EV_SR1P_AF_START,			ST_SR1P_AF				},	// AF 有りのため AF 完了待ち へ

	{	ST_SR1P_AF, 				EV_SR1P_AF_DONE,			ST_SR1P_AF_DONE			},	// AF 完了
	{	ST_SR1P_AF, 				EV_SR1P_IDLE,				ST_SR1P_IDLE			},	// AF トランジションがビジーのため異常終了

	{	ST_SR1P_AF_DONE,			EV_SR1P_AF_OK,				ST_SR1P_MEAS			},	// OK なら，測定完了待ち へ
	{	ST_SR1P_AF_DONE,			EV_SR1P_AF_NG,				ST_SR1P_IDLE			},	// NG なら，アイドルへ

	{	ST_SR1P_MEAS, 				EV_SR1P_MEAS_ERR,			ST_SR1P_IDLE			},	// 測定データ処理完了で，測定エラー発生
	{	ST_SR1P_MEAS, 				EV_SR1P_ANA_ERR,			ST_SR1P_IDLE			},	// 測定データ処理完了で，解析エラー発生
	{	ST_SR1P_MEAS, 				EV_SR1P_MEAS_DONE,			ST_SR1P_ANA				},	// 測定データ処理完了待ち へ

//2009.10.28 bagus 2point-distance --{--
// 2点間距離測定関連トランジション
	{	ST_SR1P_MEAS,				EV_SR1P_DISTANCE,			ST_SR1P_DISTANCE		},	// 2点間距離測定（ポップアップ要求）
	{	ST_SR1P_DISTANCE,			EV_SR1P_MEAS_DONE,			ST_SR1P_ANA				},	// 2点間距離測定完了（画面OK押下）
	{	ST_SR1P_DISTANCE,			EV_SR1P_MEAS_ERR,			ST_SR1P_DISTANCE_CANCEL	},	// 2点間距離測定キャンセル（画面CANCEL押下）
	{	ST_SR1P_DISTANCE,			EV_SR1P_IDLE,				ST_SR1P_ANA_DONE		},	//	マニュアル時の1点処理
	{	ST_SR1P_DISTANCE_CANCEL,	EV_SR1P_IDLE,				ST_SR1P_IDLE			},	//
//2009.10.28 bagus 2point-distance --}--
//2009.11.03 bagus MS --{--
	{	ST_SR1P_MEAS,				EV_SR1P_MS,					ST_SR1P_MS				},	//
	{	ST_SR1P_MS,					EV_SR1P_MEAS_DONE,			ST_SR1P_MS_DONE			},	//
	{	ST_SR1P_MS,					EV_SR1P_MEAS_ERR,			ST_SR1P_MS_CANCEL		},	//
	{	ST_SR1P_MS_DONE,			EV_SR1P_IDLE,				ST_SR1P_ANA				},	//
	{	ST_SR1P_MS_DONE,			EV_SR1P_MS_NO_ANA,			ST_SR1P_ANA_DONE		},	//マニュアル測定時
	{	ST_SR1P_MS_CANCEL,			EV_SR1P_IDLE,				ST_SR1P_IDLE			},	//

	//2009.11.03 bagus MS --}--

	{	ST_SR1P_ANA, 				EV_SR1P_ANA_DONE,			ST_SR1P_ANA_DONE		},	// 測定データ処理完了

	{	ST_SR1P_ANA_DONE, 			EV_SR1P_IDLE,				ST_SR1P_IDLE			},	// アイドルへ

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};

// =========================================================================//
//	状態別処理関数テーブル
// =========================================================================//
BEGIN_STATEPROC_MAP(CChiefTransiSr1Point)
	/*	0 */	ENTER_STATE(ST_SR1P_INIT,			enter_ST_SR1P_INIT),
	/*	1 */	ENTER_STATE(ST_SR1P_IDLE,			enter_ST_SR1P_IDLE),
// 2009.05.11 K.Matsuo SitePR -->
	/*	2 */	ENTER_STATE(ST_SR1P_SITEPR_START,					enter_ST_SR1P_SITEPR_START),					// (WAIT 状態無し)
	/*	3 */	ENTER_STATE(ST_SR1P_SITEPR_AUTO,					enter_ST_SR1P_SITEPR_AUTO),
	/*	4 */	ENTER_STATE(ST_SR1P_SITEPR_AUTO_OK,					enter_ST_SR1P_SITEPR_AUTO_OK),					// (WAIT 状態無し)
	/*	5 */	ENTER_STATE(ST_SR1P_SITEPR_AUTO_PROCEED,			enter_ST_SR1P_SITEPR_AUTO_PROCEED),				// (WAIT 状態無し)
	/*	6 */	ENTER_STATE(ST_SR1P_SITEPR_AUTO_PROCEED_PRESTEP,	enter_ST_SR1P_SITEPR_AUTO_PROCEED_PRESTEP),		// (WAIT 状態無し)
	/*	7 */	ENTER_STATE(ST_SR1P_SITEPR_AUTO_CANCEL,				enter_ST_SR1P_SITEPR_AUTO_CANCEL),				// (WAIT 状態無し)
	/*	8 */	ENTER_STATE(ST_SR1P_SITEPR_MANU,					enter_ST_SR1P_SITEPR_MANU),
	/*	9 */	ENTER_STATE(ST_SR1P_SITEPR_MANU_OK,					enter_ST_SR1P_SITEPR_MANU_OK),					// (WAIT 状態無し)
	/*	10 */	ENTER_STATE(ST_SR1P_SITEPR_MANU_CANCEL,				enter_ST_SR1P_SITEPR_MANU_CANCEL),				// (WAIT 状態無し)
	/*	11 */	ENTER_STATE(ST_SR1P_SITEPR_PREP_MEAS,				enter_ST_SR1P_SITEPR_PREP_MEAS),				// (WAIT 状態無し)
// 2009.05.11 K.Matsuo SitePR <--
	/*	12 */	ENTER_STATE(ST_SR1P_ZMOVE,			enter_ST_SR1P_ZMOVE),		// (WAIT 状態無し)
	/*	13 */	ENTER_STATE(ST_SR1P_AF,				enter_ST_SR1P_AF),			// (WAIT 状態無しの場合有り)
	/*	14 */	ENTER_STATE(ST_SR1P_AF_DONE,		enter_ST_SR1P_AF_DONE),		// (WAIT 状態無し)
	/*	15 */	ENTER_STATE(ST_SR1P_MEAS,			enter_ST_SR1P_MEAS),		// (WAIT 状態無し)
	/*	16 */	ENTER_STATE(ST_SR1P_ANA,			enter_ST_SR1P_ANA),
	/*	17 */	ENTER_STATE(ST_SR1P_ANA_DONE,		enter_ST_SR1P_ANA_DONE),	// (WAIT 状態無し)
//2009.10.28 bagus 2point-distance --{--
	/*	18 */	ENTER_STATE(ST_SR1P_DISTANCE,		enter_ST_SR1P_DISTANCE),		// (WAIT 状態あり)
	/*	19 */	ENTER_STATE(ST_SR1P_DISTANCE_CANCEL,enter_ST_SR1P_DISTANCE_CANCEL),	// (WAIT 状態あり)
//2009.10.28 bagus 2point-distance --}--
//2009.11.03 bagus MS --{--
	/*	20 */	ENTER_STATE(ST_SR1P_MS,				enter_ST_SR1P_MS),				// (WAIT 状態あり)
	/*	21 */	ENTER_STATE(ST_SR1P_MS_CANCEL,		enter_ST_SR1P_MS_CANCEL),		// (WAIT 状態あり)
	/*	22 */	ENTER_STATE(ST_SR1P_MS_DONE,		enter_ST_SR1P_MS_DONE),			// (WAIT 状態あり)
//2009.11.03 bagus MS --}--

END_STATEPROC_MAP(CChiefTransiSr1Point)


/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSr1Point

IMPLEMENT_DYNCREATE(CChiefTransiSr1Point, CWinThread)

CChiefTransiSr1Point::CChiefTransiSr1Point(CWnd* pParent /*=NULL*/) : cActive(TransTable_Sr1Point)
{
	TRACE(_T("CChiefTransiSr1Point::CChiefTransiSr1Point() \n"));

	m_pcChiefView = (CView*) pParent;

	//	スレッドの起動待ち
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiSr1Point."));

	//	オブジェクトの自動破棄を禁止
	m_bAutoDelete = FALSE;

}

CChiefTransiSr1Point::~CChiefTransiSr1Point()
{
	TRACE(_T("CChiefTransiSr1Point::~CChiefTransiSr1Point() \n"));

	//	スレッドの消滅待ち
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiSr1Point."));
		}
	}
}

BOOL CChiefTransiSr1Point::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiSr1Point::InitInstance() \n"));

	// コンストラクタの待ちを解除します
	//		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．
	m_cSyncEvent.SetEvent();

	// ADAP, WVASE の初期化
	m_xmpDde.Init();
	m_xmpDde.Connect(DDE_SERVICE, DDE_TOPIC);

	// 初期状態処理用イベント発行
	TransiEvent(0, 0);

	// 変数初期化
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiSr1Point::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiSr1Point::ExitInstance() \n"));

	// ADAP, WVASE の終了
	m_xmpDde.Disconnect();
	m_xmpDde.Uninit();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiSr1Point, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiSr1Point)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TR1P_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TR1P_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSr1Point メッセージ ハンドラ

/*
 *	トランジションイベント発行 メッセージハンドラ
 */
void CChiefTransiSr1Point::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	// トランジション間タイミング調整(とりあえずだけど恒久かも・・)

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat メッセージハンドラ
 */
void CChiefTransiSr1Point::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	トランジションイベント発行用関数
 */
void CChiefTransiSr1Point::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TR1P_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TR1P_SETEVENT, _T("WM_CHIF_TR1P_SETEVENT"), 7, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
}

/*
 *	HeartBeat 発行用関数
 */
void CChiefTransiSr1Point::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TR1P_HEARTBEAT, 0, 0);
}

/*
 *	現在の状態が「アイドル」かどうかをチェック
 *		戻り値：	true	アイドルである
 *					false	アイドルでない
 */
bool CChiefTransiSr1Point::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_SR1P_IDLE) {
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
int CChiefTransiSr1Point::GetCurrentState()
{
	return CurrState;
}

/*
 *	呼出元トランジションに完了を通知
 */
void CChiefTransiSr1Point::NotifyComplete(const cEventParams *const pEvParams)
{
	switch (m_dwParentTransi) {
	case TRANSI_AF:
	case TRANSI_SR_REFER:
	case TRANSI_DESKEW:
	case TRANSI_SR_1POINT:
	default:
		// 何もしません．
		break;
	case TRANSI_MASTER:
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_UI1POINT_DONE, pEvParams);
		break;
	case TRANSI_SEQ:
		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_1P_DONE, pEvParams);
		break;
	}
}

/*
 *	ポイントマニュアルアジャスト 有無 判定
 */
BOOL CChiefTransiSr1Point::IsPointManualAdjust(DWORD dwPointNo)
{
	BOOL l_bRc = FALSE;

	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	DWORD l_dwPMA_Mode = l_pMainRcpInfo->MainRcpParam.hdr.nPointManualAdjustment;

#if 0	// ラベルを使うようにしました
	switch (l_dwPMA_Mode) {
	case 0:		// しない
		l_bRc = FALSE;
		break;
	case 1:		// １ポイント目だけ実施
		if (1 == dwPointNo) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case 2:		// 全ポイントで実施
	default:
		l_bRc = TRUE;
		break;
	}
#else	// ラベルを使うようにしました
	switch (l_dwPMA_Mode) {
	case POINT_MANUAL_ADJUSTMENT_NONE:							// しない
		l_bRc = FALSE;
		break;
//	case POINT_MANUAL_ADJUSTMENT_ONLY_FIRST_POINT:				// １ポイント目だけ実施
//		if (1 == dwPointNo) {
//			l_bRc = TRUE;
//		} else {
//			l_bRc = FALSE;
//		}
//		break;
	case POINT_MANUAL_ADJUSTMENT_ALL_POINT_EVERY_ADJUSTMENT:	// 全ポイントで実施
	default:
		l_bRc = TRUE;
		break;
	}
#endif	// ラベルを使うようにしました

	return l_bRc;
}

/*
 *	X，Y，Z 座標を取得
 */
void CChiefTransiSr1Point::GetMeasPosXYZ(MEASPOS *pMeasPosXYZ)
{
	// XYZ 軸座標を取得
	STAGE_COORD_XYZ l_MeasPosXYZ;
	NS_StageGetPos(&l_MeasPosXYZ);

/* modified 2009.09.30 hmenjo 単独起動時，実位置(X/Y)取得 ---------- { ---------- */
//#if 0	// 実位置の場合はこの取得が必要です
//	pMeasPosXYZ->lX = l_MeasPosXYZ.lX;
//	if (0xcdcdcdcd == pMeasPosXYZ->lX) {pMeasPosXYZ->lX = 0;}
//	pMeasPosXYZ->lY = l_MeasPosXYZ.lY;
//	if (0xcdcdcdcd == pMeasPosXYZ->lY) {pMeasPosXYZ->lY = 0;}
//#endif
/* modified 2009.09.30 hmenjo 単独起動時，実位置(X/Y)取得 ----------			  */
	if (TRANSI_MASTER == m_dwParentTransi) {
		/* 単独起動時は X/Y の実位置も代入します．	*/
		pMeasPosXYZ->lX = l_MeasPosXYZ.lX;
		if (0xcdcdcdcd == pMeasPosXYZ->lX) {pMeasPosXYZ->lX = 0;}
		pMeasPosXYZ->lY = l_MeasPosXYZ.lY;
		if (0xcdcdcdcd == pMeasPosXYZ->lY) {pMeasPosXYZ->lY = 0;}
	}
/* modified 2009.09.30 hmenjo 単独起動時，実位置(X/Y)取得 ---------- } ---------- */
	pMeasPosXYZ->lZ = l_MeasPosXYZ.lZ;
	if (0xcdcdcdcd == pMeasPosXYZ->lZ) {pMeasPosXYZ->lZ = 0;}
}

/*
 *	SR １ポイント測定 トランジションをキャンセル終了させます．
 */
void CChiefTransiSr1Point::CancelSr1Point()
{
	m_bCancelReq = TRUE;

// 2009.05.28 myanagida -->
	switch (CurrState) {
	case ST_SR1P_SITEPR_AUTO:
		// サイトパターンをキャンセルします
		/*
		 *	ココにサイトパターンのキャンセル処理を入れてください．
		 */
		m_pSitePtSeq->ShutDown();
		break;
	case ST_SR1P_SITEPR_MANU:
		// マニュアルデスキューをキャンセルします
		/*
		 *	ココにマニュアルデスキューのキャンセル処理を入れてください．
		 */
		m_pSitePtSeq->ShutDown();
		break;
	//2009.11.07 bagus MS --{--
	case ST_SR1P_MS:
		//顕微鏡測定の場合には何もすることがないので強制的にIDLEにする
		CurrState = ST_SR1P_IDLE;
		break;
	//2009.11.07 bagus MS --}--
	default:
		// 何もしません
		break;
	}
// 2009.05.28 myanagida <--

	// AF トランジションをキャンセル終了させます．
	if (false == ((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->IsIdle()) {
		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->CancelAF();
	}
}

// 2009.05.11 K.Matsuo SitePR -->
/*
 *	カレントレンズとレシピレンズをチェック
 */
BOOL CChiefTransiSr1Point::IsCurrLensEqualsRecipeLens()
{
// 2009.05.26 K.Matsuo SitePR -->
//	LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);
//	return l_pStageProgInfoHdr->iLens == StageGetCurLens();
	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
	return l_pMeasProgInfo->ScanParams._SR.iLens == StageGetCurLens();
// 2009.05.26 K.Matsuo SitePR <--

}
// 2009.05.11 K.Matsuo SitePR <--

//--------------------------------------------------------------------------//
//	状態別処理関数群
//--------------------------------------------------------------------------//
/*
 *	初期状態
 */
void CChiefTransiSr1Point::enter_ST_SR1P_INIT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[0], (DWORD) pEvParams);

/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
	m_lZ_AF = 0x80000000;
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */

	// 初期化処理はありません．
}

/*
 *	アイドル
 */
void CChiefTransiSr1Point::enter_ST_SR1P_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[1], (DWORD) pEvParams);

	m_dwAFValid = 0;
	m_bCancelReq = FALSE;
	// WAIT 状態にするため処理はありません．
}

// 2009.05.11 K.Matsuo SitePR -->
/*
 *	SitePR 開始処理前(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_START(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOBYTE(LOWORD(pEvParams))：呼出元トランジション
/*				HIBYTE(LOWORD(pEvParams))：測定モード added 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17)
/*				HIWORD(pEvParams)：測定ポイント番号
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[2], (DWORD) pEvParams);

// 2009.05.21 K.Matsuo -->
	// 呼出元トランジションを保存
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//	m_dwParentTransi = LOWORD((DWORD) pEvParams);
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
	m_dwParentTransi = LOBYTE(LOWORD(pEvParams));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
	// 測定ポイント番号を保存
	m_dwPointNo = HIWORD((DWORD) pEvParams);
// 2009.05.21 K.Matsuo <--
/* added 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- { ---------- */
	m_enMeasMode = (MEASMODE_ID) HIBYTE(LOWORD(pEvParams));
/* added 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- } ---------- */

/* added 2009.06.08 hmenjo SitePR XY 座標値表示不具合 ---------- { ---------- */
	// XY 座標位置記憶
	m_MeasPosXYZ.lX = m_MeasPosbufXYZ.lX;
	m_MeasPosXYZ.lY = m_MeasPosbufXYZ.lY;
/* added 2009.06.08 hmenjo SitePR XY 座標値表示不具合 ---------- } ---------- */

// 2009.05.16 myanagida -->

	// サイトパターン ini ファイルチェック
	SYSTEM_CONFIG *l_SystemConfig = &(((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_SystemConfig);
	ConfigFile_GetNanoSpecIni(l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
/* modified 2009.06.05 hmenjo １ポイント測定単体動作時は SitePR 禁止 ---------- { ---------- */
//	if (l_SystemConfig->bSitePatternRec != TRUE) {
/* modified 2009.06.05 hmenjo １ポイント測定単体動作時は SitePR 禁止 ---------- 			 */
//	if ((l_SystemConfig->bSitePatternRec != TRUE) || (TRANSI_MASTER == m_dwParentTransi)) {
/* modified 2009.06.05 hmenjo １ポイント測定単体動作時は SitePR 禁止 ---------- } ---------- */
// 2009.06.10 K.Matsuo -->
	if ((l_SystemConfig->nPRMethod <=0 || l_SystemConfig->bSitePatternRec != TRUE) || (TRANSI_MASTER == m_dwParentTransi)) {
// 2009.06.10 K.Matsuo <--
		// サイトパターン処理実施しない
		TransiEvent(EV_SR1P_SITEPR_NONE, pEvParams);

		return;
	}

	// ハードシミュレートモード判定 -------------------------------------------
	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		// ハードシミュレートモードだったので，サイトパターンパス
		// トランジション更新
		TransiEvent(EV_SR1P_SITEPR_NONE, pEvParams);

		return;
	}

	// メインレシピを取得
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);

	// ステージプログラム
	LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr);

	// サイトパターン テーブル
	LPSITE_PATTERN l_pSitePattern = (LPSITE_PATTERN) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pSitePattern);

	// 画面からの動作有無指定チェック
	//	 サイトパターン実施するか？ 情報をステージＰＧＭから取得する
	switch(l_pStageProgInfoHdr->nSitePatternMode) {
	case SITE_PR_MODE_USE_PR:					// correct with use pattern recognition

		// 測定ポイントとしての測定有無チェック(する/しない かつ 測定パターンの登録有無)
		if((l_pSitePattern[m_dwPointNo-1].bEnable == TRUE)
		&& ((strlen(l_pSitePattern[m_dwPointNo-1].PatternInfo[0].szSitePatternName) != 0)
		 || (strlen(l_pSitePattern[m_dwPointNo-1].PatternInfo[1].szSitePatternName) != 0))){
			// サイトパターン処理実施する
			TransiEvent(EV_SR1P_SITEPR_AUTO, pEvParams);
		}
		else {
			// サイトパターン処理実施しない
			TransiEvent(EV_SR1P_SITEPR_NONE, pEvParams);
		}
		break;

	case SITE_PR_MODE_NO_PR:					// correct with no pattern recognition
	default:
		// サイトパターン処理実施しない
		TransiEvent(EV_SR1P_SITEPR_NONE);

		break;
	}
// 2009.05.16 myanagida <--
}

/*
 *	SitePR オート完了待ち
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_AUTO(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[3], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SITEPR_AUTO_ING);	// Status Bar

	/*
	 *	ココにSitePRオートの起動処理を入れてください．
	 */
// 2009.05.16 myanagida -->
	// サイトパターン テーブル
	LPSITE_PATTERN l_pSitePattern = (LPSITE_PATTERN) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pSitePattern);

	m_pSitePtSeq = (CSitePtSeq*)AfxBeginThread(RUNTIME_CLASS(CSitePtAutoSeq), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_pSitePtSeq->m_pChiefView			= (CChiefView*)m_pcChiefView;
	m_pSitePtSeq->m_pChiefTransiSr1Point= this;
	m_pSitePtSeq->m_pStageProgInfoHdr 	= (LPSTAGE_PROG_INFO_HDR)((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
	m_pSitePtSeq->m_pSitePattern 		= &l_pSitePattern[m_dwPointNo-1];
	m_pSitePtSeq->ResumeThread();
// 2009.05.16 myanagida <--

	// WAIT 状態にするため処理はありません．
}

/*
 *	SitePR オート OK 完了(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_AUTO_OK(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[4], (DWORD) pEvParams);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SITEPR_AUTO_COMPLETE);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// ターレット制御(サイトパターン用：メインレシピの Lens，あるいは，ステージプログラムの Lens)
	//		ここではメインレシピの Lens を使います
	//		前回のAF成功／失敗結果を優先し、なるべくAFしないようにする。
	//		よって、事前にカレントレンズとのチェックをしておく
	//		・・・つまり、前回AF成功していれば、再AFして失敗になるのを防ぎたい
	//					  前回AF失敗していれば、再AFしても失敗になる可能性大なので、無駄になるAF処理をしたくないため
	if ( IsCurrLensEqualsRecipeLens() ) {
		// カレントレンズとレシピレンズが一致
		TransiEvent(EV_SR1P_SITEPR_PREP_MEAS);
	}
	else {
// 2009.05.26 K.Matsuo SitePR -->
		// ターレット制御(測定用：測定プログラムの Lens)
		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ----------				*/
		((CChiefView*) m_pcChiefView)->SelectLens(((CChiefView*) m_pcChiefView)->GetMeasLens(l_pMeasProgInfo->ScanParams.hdr.wHeadType));
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- } ---------- */
// 2009.05.26 K.Matsuo SitePR <--

		TransiEvent(EV_SR1P_AF_START, (cEventParams*) 0);
	}
}

/*
 *	SitePR オート失敗のまま続行(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_AUTO_PROCEED(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[5], (DWORD) pEvParams);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SITEPR_AUTO_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// ターレット制御(サイトパターン用：メインレシピの Lens，あるいは，ステージプログラムの Lens)
	//		ここではメインレシピの Lens を使います
	//		前回のAF成功／失敗結果を優先し、なるべくAFしないようにする。
	//		よって、事前にカレントレンズとのチェックをしておく
	//		・・・つまり、前回AF成功していれば、再AFして失敗になるのを防ぎたい
	//					  前回AF失敗していれば、再AFしても失敗になる可能性大なので、無駄になるAF処理をしたくないため
	if ( IsCurrLensEqualsRecipeLens() ) {
		// カレントレンズとレシピレンズが一致
		TransiEvent(EV_SR1P_SITEPR_PREP_MEAS);
	}
	else {
// 2009.05.26 K.Matsuo SitePR -->
		// ターレット制御(測定用：測定プログラムの Lens)
		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ----------				*/
		((CChiefView*) m_pcChiefView)->SelectLens(((CChiefView*) m_pcChiefView)->GetMeasLens(l_pMeasProgInfo->ScanParams.hdr.wHeadType));
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- } ---------- */
// 2009.05.26 K.Matsuo SitePR <--

		TransiEvent(EV_SR1P_AF_START, (cEventParams*) 0);
	}
}

/*
 *	SitePR オート失敗のまま続行（前ステップ位置）(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_AUTO_PROCEED_PRESTEP(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[6], (DWORD) pEvParams);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SITEPR_AUTO_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

// 2009.06.30 K.Matsuo SitePR Delete -->
//	// AutoFocus後、ステージ移動しているので、再AFが必要な扱いとする。
//	// なので、無条件（カレントレンズとレシピレンズのチェックはしない）で、AF処理を実施する。
// 2009.06.30 K.Matsuo SitePR Delete <--

// 2009.06.30 K.Matsuo SitePR Add -->
// レシピレンズに変更した上で、必ずAF処理を実施する。
	if ( IsCurrLensEqualsRecipeLens() ) {
		// カレントレンズとレシピレンズが一致
		;
	}
	else {
		// ターレット制御(測定用：測定プログラムの Lens)
		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ----------				*/
		((CChiefView*) m_pcChiefView)->SelectLens(((CChiefView*) m_pcChiefView)->GetMeasLens(l_pMeasProgInfo->ScanParams.hdr.wHeadType));
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- } ---------- */
	}
// 2009.06.30 K.Matsuo SitePR Add <--

	TransiEvent(EV_SR1P_AF_START, (cEventParams*) 0);

}

/*
 *	SitePR オートレシピ終了(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_AUTO_CANCEL(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[7], (DWORD) pEvParams);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SITEPR_AUTO_CANCEL);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	TransiEvent(EV_SR1P_IDLE);

// 2009.05.25 K.Matsuo SitePR -->
	// 呼出元トランジションに完了を通知
	NotifyComplete((cEventParams*) TR_SR1P_MEAS_ERR);
// 2009.05.25 K.Matsuo SitePR <--
}

/*
 *	SitePR マニュアル完了待ち
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_MANU(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[8], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SITEPR_MANU_ING);	// Status Bar

	/*
	 *	ココにSitePRマニュアルの起動処理を入れてください．
	 */
// 2009.06.30 K.Matsuo SitePR Add -->
// レシピレンズに変更した上で、必ずAF処理を実施する。
	if ( IsCurrLensEqualsRecipeLens() ) {
		// カレントレンズとレシピレンズが一致
		;
	}
	else {
		// ターレット制御(測定用：測定プログラムの Lens)
		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ----------				*/
		((CChiefView*) m_pcChiefView)->SelectLens(((CChiefView*) m_pcChiefView)->GetMeasLens(l_pMeasProgInfo->ScanParams.hdr.wHeadType));
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- } ---------- */
	}
// 2009.06.30 K.Matsuo SitePR Add <--
// 2009.05.16 myanagida -->
	// サイトパターン テーブル
	LPSITE_PATTERN l_pSitePattern = (LPSITE_PATTERN) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pSitePattern);

	m_pSitePtSeq = (CSitePtSeq*)AfxBeginThread(RUNTIME_CLASS(CSitePtManualSeq), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_pSitePtSeq->m_pChiefView			= (CChiefView*)m_pcChiefView;
	m_pSitePtSeq->m_pChiefTransiSr1Point= this;
	m_pSitePtSeq->m_pStageProgInfoHdr 	= (LPSTAGE_PROG_INFO_HDR)((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
	m_pSitePtSeq->m_pSitePattern 		= &l_pSitePattern[m_dwPointNo-1];
	m_pSitePtSeq->ResumeThread();

	// ポップアップをアラームで通知します．
	if (HOST_LOCAL != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_ManualSitePatternRequired));
	}

// 2009.05.16 myanagida <--

	// WAIT 状態にするため処理はありません．
}

/*
 *	SitePR マニュアル OK 完了(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_MANU_OK(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[9], (DWORD) pEvParams);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SITEPR_MANU_COMPLETE);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// ターレット制御(サイトパターン用：メインレシピの Lens，あるいは，ステージプログラムの Lens)
	//		ここではメインレシピの Lens を使います
	//		前回のAF成功／失敗結果を優先し、なるべくAFしないようにする。
	//		よって、事前にカレントレンズとのチェックをしておく
	//		・・・つまり、前回AF成功していれば、再AFして失敗になるのを防ぎたい
	//					  前回AF失敗していれば、再AFしても失敗になる可能性大なので、無駄になるAF処理をしたくないため
	if ( IsCurrLensEqualsRecipeLens() ) {
		// カレントレンズとレシピレンズが一致
		TransiEvent(EV_SR1P_SITEPR_PREP_MEAS);
	}
	else {
// 2009.05.26 K.Matsuo SitePR -->
		// ターレット制御(測定用：測定プログラムの Lens)
		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ----------				*/
		((CChiefView*) m_pcChiefView)->SelectLens(((CChiefView*) m_pcChiefView)->GetMeasLens(l_pMeasProgInfo->ScanParams.hdr.wHeadType));
/* modified 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- } ---------- */
// 2009.05.26 K.Matsuo SitePR <--

		TransiEvent(EV_SR1P_AF_START, (cEventParams*) 0);
	}
}

/*
 *	SitePR マニュアルレシピ終了(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_MANU_CANCEL(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[10], (DWORD) pEvParams);

#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SITEPR_MANU_CANCEL);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	TransiEvent(EV_SR1P_IDLE);

// 2009.05.25 K.Matsuo SitePR -->
	// 呼出元トランジションに完了を通知
	NotifyComplete((cEventParams*) TR_SR1P_MEAS_ERR);
// 2009.05.25 K.Matsuo SitePR <--
}

/*
 *	SitePR 終了 → 測定前の準備処理(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_SITEPR_PREP_MEAS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[11], (DWORD) pEvParams);

	// カラーフィルタ制御(測定用：測定プログラムの OpticalFilter)
	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- { ---------- */
//	if (0 == MEAS_SrHead_ChangeCcdShutter(l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType)) {
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ----------			  */
	if (0 == MEAS_SrHead_ChangeCcdShutter(((CChiefView*) m_pcChiefView)->GetMeasFilter(l_pMeasProgInfo->ScanParams.hdr.wHeadType))) {
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- } ---------- */
		// 異常時でも無視します．
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSr1Point::enter_ST_SR1P_AF_DONE()."));
	}

	TransiEvent(EV_SR1P_MEAS_START);
}

// 2009.05.11 K.Matsuo SitePR <--

/*
 *	測定ポイントＺ軸移動完了待ち(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_ZMOVE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[12], (DWORD) pEvParams);

#if 0 // 2009.05.21 K.Matsuo 移動しました（ enter_ST_SR1P_SITEPR_START ）
	// 呼出元トランジションを保存
	m_dwParentTransi = LOWORD((DWORD) pEvParams);
	// 測定ポイント番号を保存
	m_dwPointNo = HIWORD((DWORD) pEvParams);
#endif // 0

	// このブロックは仮変数の定義です→ -----------------------------------------------------------------------------------------------
		BOOL	l_bAF = TRUE;	// Z 軸移動する(AF しない設定の場合)
	//	long	l_lZPos = 10;		// Z 軸位置
	// このブロックは仮変数の定義です← -----------------------------------------------------------------------------------------------
	EEPROMDATA	l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z 軸の初期化停止位置
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu;	// Z 軸の初期化停止位置ファクタ

	// XY 座標位置記憶
	m_MeasPosXYZ.lX = m_MeasPosbufXYZ.lX;
	m_MeasPosXYZ.lY = m_MeasPosbufXYZ.lY;

	// ハードシミュレートモード判定 -------------------------------------------
	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		// ハードシミュレートモードだったので，測定完了待ち へ
		m_MeasPosXYZ.lZ = 0;
		// トランジション更新
		TransiEvent(EV_SR1P_MEAS_START);
	} else {
		// 実機の場合 ---------------------------------------------------------
		// AF 要/不要判定
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
/* modified 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- { ---------- */
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
////		if ((0 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) || ((1 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) && (1 != m_dwPointNo))) {
////			// 不要
////			m_dwAFValid = 0;	// AF 無し
////			// (全点 AF 無しの場合)Z 軸移動が必要かチェック
//////			if ((0 != l_pMainRcpInfo->MainRcpParam._SR.nFocus) || (0 == l_bAF)) {
////			if ((0 != l_pMainRcpInfo->MainRcpParam._SR.nFocus) || (0 == l_bAF) || (TRANSI_MASTER == m_dwParentTransi)) {
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ----------				*/
//		HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
//		((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, l_pMainRcpInfo->MainRcpParam.hdr.wHeadType, &l_HeadMainRcpInf);
//		if ((0 == l_HeadMainRcpInf.nFocus) || ((1 == l_HeadMainRcpInf.nFocus) && (1 != m_dwPointNo))) {
//			// 不要
//			m_dwAFValid = 0;	// AF 無し
//			// (全点 AF 無しの場合)Z 軸移動が必要かチェック
//			if ((0 != l_HeadMainRcpInf.nFocus) || (0 == l_bAF) || (TRANSI_MASTER == m_dwParentTransi)) {
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
/* modified 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ----------				*/
		HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
		((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, l_pMainRcpInfo->MainRcpParam.hdr.wHeadType, &l_HeadMainRcpInf);
		if ((0 == l_HeadMainRcpInf.nFocus)
		 || ((1 == l_HeadMainRcpInf.nFocus) && (1 != m_dwPointNo))
/* deleted 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//		 || (HEAD_TYPE_CTA == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
//		 || (HEAD_TYPE_4PP == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
/* deleted 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
		 //2009.11.10 bagus MS --{-- Z軸退避
		 || (HEAD_TYPE_MS == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
		 //2009.11.10 bagus MS --}-- Z軸退避
		 ) {
			// 不要
			m_dwAFValid = 0;	// AF 無し
			// (全点 AF 無しの場合)Z 軸移動が必要かチェック
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//			if ((HEAD_TYPE_CTA != l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
//			 && (HEAD_TYPE_4PP != l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
//			//2009.11.10 bagus MS --{-- Z軸退避
//			&& (HEAD_TYPE_MS != l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
//			//2009.11.10 bagus MS --}-- Z軸退避
//			 && ((0 != l_HeadMainRcpInf.nFocus) || (0 == l_bAF) || (TRANSI_MASTER == m_dwParentTransi))) {
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
			if (
				(HEAD_TYPE_4PP == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
			 || (HEAD_TYPE_CTA == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
			 || ((HEAD_TYPE_SR == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
			  && ((MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
			   || (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_pMeasProgInfo->ScanParams.hdr.wScanType)))
				) {
				l_bAF = FALSE;
			}
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
			if (
				((0 != l_HeadMainRcpInf.nFocus) || (0 == l_bAF) || (TRANSI_MASTER == m_dwParentTransi))
			 && (HEAD_TYPE_MS != l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
				) {
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
/* modified 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- } ---------- */
				// 不要
				// トランジション更新
				if (0 != IsPointManualAdjust(m_dwPointNo)) {
					// PMA のみが必要
					TransiEvent(EV_SR1P_AF_START, (cEventParams*) 1);
				} else {
					TransiEvent(EV_SR1P_MEAS_START);
				}
			} else {
				// 必要(仕様上必要なら)
				// Ｚ軸移動
/* deleted 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- { ---------- */
//				TCHAR	l_szMainRcpName[256];
//				((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
/* deleted 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- } ---------- */
				// 初期化停止位置[pulse]を[0.1um]に変換します
				LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
				l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// 初期化停止位置[pulse]÷ファクタ[pulse/mm]
				l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]に変換する
				long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));

				NS_ConvertToStageMoveCoord(&l_lZ);		// 補正

/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
///* added 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- { ---------- */
//				if (HEAD_TYPE_CTA == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType
//				//2009.11.10 bagus MS --{-- Z軸退避
//				|| HEAD_TYPE_MS == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType
//				//2009.11.10 bagus MS --}-- Z軸退避
//				) {
//// 2009.11.19 K.Matsuo Delete -->
////					/* すでに Z 軸退避位置(センサ オン)なら，Z 軸移動しません．	*/
////					if (0 != nexioIsRetractPosZ()) {
////						TransiEvent(EV_SR1P_ZMOVE_DONE);
////						return;
////					}
//// 2009.11.19 K.Matsuo Delete <--
//					/* CTA 用の Z 軸退避位置を取得します．	*/
///* modified 2009.10.30 hmenjo CTA Z 軸退避位置 ---------- { ---------- */
////					STRESS_CONFIG l_StressConfig;
////					ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
////					l_lZ = l_StressConfig.lScanZAxisPosition;
////l_lZ = 420000L;	/* [0.1um]仮です*/
////2009.11.10 bagus MS --{-- Z軸退避
//					switch(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType){
//					case HEAD_TYPE_CTA:
//						{
////2009.11.10 bagus MS --}-- Z軸退避
///* modified 2009.10.30 hmenjo CTA Z 軸退避位置 ---------- 			 */
//					CA_CONFIG l_CaConfig;
//					ConfigFile_GetNanoSpecIni(&l_CaConfig, CONFIG_FILE_CA_CONFIG);
//					l_lZ = l_CaConfig.lCaStdZAxisRetractPos;
///* modified 2009.10.30 hmenjo CTA Z 軸退避位置 ---------- } ---------- */
//						}
//					break;
//					case HEAD_TYPE_MS:
//						{
//						MS_CONFIG l_MsConfig;
//						ConfigFile_GetNanoSpecIni(&l_MsConfig, CONFIG_FILE_MS_CONFIG);
//						l_lZ = l_MsConfig.lStdZAxisRetractPos;
//						}
//						break;
//					}
////2009.11.10 bagus MS --}-- Z軸退避
//				}
///* added 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- } ---------- */
///* added 2009.11.06 K.Matsuo Resist AF 無し，Z 軸退避位置 ---------- { ---------- */
//				if (HEAD_TYPE_4PP == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
//// 2009.11.18 K.Matsuo Delete -->
////					/* すでに Z 軸退避位置(センサ オン)なら，Z 軸移動しません．	*/
////					if (0 != nexioIsRetractPosZ()) {
////						TransiEvent(EV_SR1P_ZMOVE_DONE);
////						return;
////					}
//// 2009.11.18 K.Matsuo Delete <--
//					/* Resist 用の Z 軸退避位置を取得します．	*/
//					RS_CONFIG l_RsConfig;
//					ConfigFile_GetNanoSpecIni(&l_RsConfig, CONFIG_FILE_RS_CONFIG);
//					l_lZ = l_RsConfig.lRsStdZAxisRetractPos;
//				}
///* added 2009.11.06 K.Matsuo Resist AF 無し，Z 軸退避位置 ---------- } ---------- */
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
				/* ヘッドによっては指定の Z 軸位置がある場合	*/
				switch (l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
				case HEAD_TYPE_MS:
					{
						MS_CONFIG l_MsConfig;
						ConfigFile_GetNanoSpecIni(&l_MsConfig, CONFIG_FILE_MS_CONFIG);
						l_lZ = l_MsConfig.lStdZAxisRetractPos;
					}
					break;
				}
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */

				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
#if 0	//----- Kawashima 2008.12.01 ----->
				TransiEvent(EV_SR1P_ZMOVE_DONE);
#else	//----- Kawashima 2008.12.01 -----
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);	// Status Bar
				if (0 == StageElevatorMove(l_lZ)) {
					// 移動開始失敗
					((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
					// トランジション更新
					TransiEvent(EV_SR1P_ZMOVE_FAIL);
					// 呼出元トランジションに完了を通知
					NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
				} else {
					// 移動完了
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
					((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
					((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
					((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to 0 position."));
					// トランジション更新
/* modified 2009.11.09 hmenjo CTA Z 軸退避センサ判定修正 ---------- { ---------- */
///* added 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- { ---------- */
//					/* CTA, RS で Z 軸移動後，Z 軸退避位置(センサ オン)なら正常	*/
//					if ((HEAD_TYPE_CTA == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) || (HEAD_TYPE_4PP == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
//					 && (0 == nexioIsRetractPosZ())) {
///* added 2009.11.06 hmenjo CTA Z 軸退避エラー通知 ---------- { ---------- */
//					((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
///* added 2009.11.06 hmenjo CTA Z 軸退避エラー通知 ---------- } ---------- */
//						/* トランジション更新	*/
//						TransiEvent(EV_SR1P_ZMOVE_FAIL);
//						/* 呼出元トランジションに完了を通知	*/
//						NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
//					} else
///* added 2009.10.29 hmenjo CTA AF 無し，Z 軸退避位置 ---------- } ---------- */
/* modified 2009.11.09 hmenjo CTA Z 軸退避センサ判定修正 ---------- 			 */
					/* CTA, RS で Z 軸移動後，Z 軸退避位置(センサ オン)なら正常	*/
					BOOL l_bZRetract = TRUE;
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
//					if ((HEAD_TYPE_CTA == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
//					 || (HEAD_TYPE_4PP == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
//					 || (HEAD_TYPE_MS  == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
//					) {
//						::Sleep(100);	/* センサ入力遅れ待ち	*/
//						l_bZRetract = nexioIsRetractPosZ();
//					}
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
					switch (l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
					case HEAD_TYPE_MS:
						::Sleep(100);	/* センサ入力遅れ待ち	*/
						l_bZRetract = nexioIsRetractPosZ();
						break;
					}
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
					if (0 == l_bZRetract) {
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
						/* トランジション更新	*/
						TransiEvent(EV_SR1P_ZMOVE_FAIL);
						/* 呼出元トランジションに完了を通知	*/
						NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
					} else
/* modified 2009.11.09 hmenjo CTA Z 軸退避センサ判定修正 ---------- } ---------- */
					if (0 != IsPointManualAdjust(m_dwPointNo)) {
						// PMA のみが必要
						TransiEvent(EV_SR1P_AF_START, (cEventParams*) 1);
					} else {
						TransiEvent(EV_SR1P_ZMOVE_DONE);
					}
				}
#endif	//----- Kawashima 2008.12.01 <-----
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
			}
		} else {
			// 必要
			// 次のトランジションで AF 指令を発行しています．
			// トランジション更新
			if (0 != IsPointManualAdjust(m_dwPointNo)) {
				// AF と PMA が必要
				TransiEvent(EV_SR1P_AF_START, (cEventParams*) 2);
			} else {
				TransiEvent(EV_SR1P_AF_START, (cEventParams*) 0);
			}
		}
	}
}

/*
 *	AF 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_AF(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[13], (DWORD) pEvParams);

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(3, 7)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiSr1Point::enter_ST_SR1P_AF() - AF Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// AF トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_SR1P_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SR1P_TRAAF_BUSY);
	} else {
		// カラーフィルタ制御(デスキュー用：OPEN にします)
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- { ---------- */
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- { ---------- */
////		if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ----------			   */
//		WORD l_wFilter;
//		if (0 == ((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_SystemConfig.bUseOptFilterMeasPgmAF) {
//			l_wFilter = FILTER_OPEN;
//		} else {
//			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
//			l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
//		}
//		if (0 == MEAS_SrHead_ChangeCcdShutter(l_wFilter)) {
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- } ---------- */
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ----------			  */
		WORD l_wFilter;
/* modified 2009.08.25 hmenjo SE ランプ制御 ---------- { ---------- */
//		if (0 == ((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_srConfig.bUseOptFilterMeasPgmAF) {
//			l_wFilter = FILTER_OPEN;
//		} else {
//			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
//			l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
//		}
/* modified 2009.08.25 hmenjo SE ランプ制御 ----------				*/
		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
//		if (HEAD_TYPE_SE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ----------			  */
		if (
			(HEAD_TYPE_SE == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		 || (HEAD_TYPE_COMPEASE == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		 || (HEAD_TYPE_CTA == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
		 || (HEAD_TYPE_4PP == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
		 || ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
		  && ((MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
		   || (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_pMeasProgInfo->ScanParams.hdr.wScanType)))
		 	) {
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
			l_wFilter = FILTER_OPEN;
		} else
		if (0 == ((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_srConfig.bUseOptFilterMeasPgmAF) {
			l_wFilter = FILTER_OPEN;
		} else {
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- { ---------- */
//			l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ----------			  */
			l_wFilter = ((CChiefView*) m_pcChiefView)->GetMeasFilter(l_pMeasProgInfo->ScanParams.hdr.wHeadType);
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- } ---------- */
		}
/* modified 2009.08.25 hmenjo SE ランプ制御 ---------- } ---------- */
		if (0 == MEAS_SrHead_ChangeCcdShutter(l_wFilter)) {
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- } ---------- */
			// 異常時でも無視します．
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSr1Point::enter_ST_SR1P_AF()."));
		}
		// AF 発行
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_FOCUSING);	// Status Bar
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
///* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//////		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) TRANSI_SR_1POINT);
////		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(TRANSI_SR_1POINT, pEvParams));
///* modified 2009.09.09 hmenjo 引数変更 ---------- 			 */
//		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_SR_1POINT, 0), pEvParams));
///* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ----------			  */
		if (((1 == (DWORD) pEvParams) || (2 == (DWORD) pEvParams)) && (HEAD_TYPE_MS != l_pMeasProgInfo->ScanParams.hdr.wHeadType) && (TRANSI_MASTER != m_dwParentTransi)) {
			/* PMA 有り，単独起動でなく，MS ヘッド以外の場合のみ	*/
			if (0x80000000 != m_lZ_AF) {
				/* Z 軸記憶位置が有効なので記憶位置に移動します．	*/
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
				if (0 == StageElevatorMove(m_lZ_AF)) {
					/* 移動開始失敗	*/
					((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
					/* トランジション更新	*/
					TransiEvent(EV_SR1P_IDLE);
					/* 呼出元トランジションに完了を通知	*/
					NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
					return;
				}
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
			} else {
				/* Z 軸記憶位置が無効なので AF させます．	*/
				*((LPLONG) &pEvParams) = 2;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
			}
		}
		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_SR_1POINT, 0), pEvParams));
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */

		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	AF 完了(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_AF_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[14], (DWORD) pEvParams);

	AF_VALID	l_AFValid;

	// AF 結果判定
	int l_iEventNext;
	l_AFValid.dwValid = (DWORD) pEvParams;
#if 0	// AF 結果判定を変更 -------- { --------
	switch (l_AFValid.ucResultAF) {
	case 0:		// AF 無し
	case 1:		// AF 正常
	case 2:		// 自動強制測定
		l_iEventNext = EV_SR1P_AF_OK;
		break;
	case 4:		// AF 失敗(元ポップアップ)
		switch (l_AFValid.ucResultPopup) {
		case 2:		// 閉じる(OK)(元 AF 無視)ボタン
			l_iEventNext = EV_SR1P_AF_OK;
			break;
		case 0:		// ポップアップ無し(この状態は存在しないはず)
		case 1:		// リトライボタン(この状態は存在しないはず)
		case 3:		// スキップボタン
		case 4:		// キャンセルボタン
		default:
			l_iEventNext = EV_SR1P_AF_NG;
			break;
		}
		break;
	case 3:		// 自動スキップ
		l_iEventNext = EV_SR1P_AF_NG;
		break;
	case 5:		// レシピ終了(上記「自動スキップ」とはわざと別にしてあります)
	default:
		l_iEventNext = EV_SR1P_AF_NG;
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
			l_iEventNext = EV_SR1P_AF_OK;
			break;
		case 3:		// 自動スキップ
			l_iEventNext = EV_SR1P_AF_NG;
			break;
		case 4:		// AF 失敗(ココでこの状態は存在しないはず)
		case 5:		// レシピ終了(上記「自動スキップ」とはわざと別にしてあります)
		default:
			l_iEventNext = EV_SR1P_AF_NG;
			break;
		}
		break;
	case 2:		// 閉じる(OK)(元 AF 無視)ボタン
		l_iEventNext = EV_SR1P_AF_OK;
		break;
	case 3:		// スキップボタン
	case 4:		// キャンセルボタン
	default:
		l_iEventNext = EV_SR1P_AF_NG;
		break;
	}
#endif	// AF 結果判定を変更 -------- } --------

	// 軸座標を取得(Z 軸だけ)
	GetMeasPosXYZ(&m_MeasPosXYZ);

	if (EV_SR1P_AF_NG != l_iEventNext) {
		m_dwAFValid = l_AFValid.dwValid;
		// カラーフィルタ制御(測定用：測定プログラムの OpticalFilter)
		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- { ---------- */
///* modified 2009.08.25 hmenjo SE ランプ制御 ---------- { ---------- */
////		if (0 == MEAS_SrHead_ChangeCcdShutter(l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType)) {
///* modified 2009.08.25 hmenjo SE ランプ制御 ----------			  */
//		WORD l_Filter;
//		if (HEAD_TYPE_SE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//			l_Filter = FILTER_OPEN;
//		} else {
//			l_Filter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
//		}
//		if (0 == MEAS_SrHead_ChangeCcdShutter(l_Filter)) {
///* modified 2009.08.25 hmenjo SE ランプ制御 ---------- } ---------- */
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ----------			  */
		WORD l_wFilter;
		if (HEAD_TYPE_SE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
			l_wFilter = FILTER_OPEN;
		} else {
			l_wFilter = ((CChiefView*) m_pcChiefView)->GetMeasFilter(l_pMeasProgInfo->ScanParams.hdr.wHeadType);
		}
		if (0 == MEAS_SrHead_ChangeCcdShutter(l_wFilter)) {
/* modified 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- } ---------- */
			// 異常時でも無視します．
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSr1Point::enter_ST_SR1P_AF_DONE()."));
		}
	}

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	if (EV_SR1P_AF_NG == l_iEventNext) {
		// 測定ポイント番号と VALID データをデータ処理へ通知
		MEAS_1POINT_RESULT l_Meas1PointResult;
		l_Meas1PointResult.dwPointNo = m_dwPointNo;
		l_Meas1PointResult.dwAFValid = l_AFValid.dwValid;
		l_Meas1PointResult.lX = m_MeasPosXYZ.lX;
		l_Meas1PointResult.lY = m_MeasPosXYZ.lY;
		l_Meas1PointResult.lZ = m_MeasPosXYZ.lZ;
		COPYDATASTRUCT l_CopyData;
		l_CopyData.dwData = WMCD_CHIF_1POINT_END;
		l_CopyData.cbData = sizeof(MEAS_1POINT_RESULT);
		l_CopyData.lpData = &l_Meas1PointResult;
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);
		TCHAR l_tszLogText[256];
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_FOCUS_ABORTED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		_stprintf(l_tszLogText, _T("Notify WMCD_CHIF_1POINT_END with AF NG. (Point No.= %5d, AF Valid = 0x%08x)."), m_dwPointNo, l_AFValid.dwValid);
		((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) l_AFValid.dwValid);
	}
}

/*
 *	測定完了待ち(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_MEAS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[15], (DWORD) pEvParams);

/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- { ---------- */
	((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->m_bMeasSeq = TRUE;
/* added 2009.10.29 hmenjo CTA Z 軸退避位置チェック ---------- } ---------- */

	// 軸座標を取得(Z 軸だけ)
	GetMeasPosXYZ(&m_MeasPosXYZ);

	// メインレシピを取得
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	// 測定プログラムを取得
	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* added 2009.12.15 hmenjo PMA 後，測定直前に測定PGMレンズ切替 ---------- { ---------- */
	/*	PMA などでレンズが変更されているかもしれないので，
		測定 PGM のレンズに切替えます．	*/
	((CChiefView*) m_pcChiefView)->SelectLens(((CChiefView*) m_pcChiefView)->GetMeasLens(l_pMeasProgInfo->ScanParams.hdr.wHeadType));
/* added 2009.12.15 hmenjo PMA 後，測定直前に測定PGMレンズ切替 ---------- } ---------- */

	//2009.10.28 bagus 2point-distance --{--
	//if(m_enMeasMode == MEASMODE_DISTANCE){
	if(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& l_pMeasProgInfo->ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		//2点間距離は別関数で処理する
		TransiEvent(EV_SR1P_DISTANCE);
		return;
	}
	//2009.10.28 bagus 2point-distance --}--
	if(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		//顕微鏡は別関数で処理する。
		TransiEvent(EV_SR1P_MS);
		return;
	}
	// ポイント測定
	BOOL l_bMeasErr = FALSE;
	TCHAR l_tszLogText[256];
	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		// ハードシミュレートモードだったので，そのまま解析処理 へ
/* added 2009.08.25 hmenjo SE ランプ制御 ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_OPEN, 1);	/* SE ランプフィルタ OPEN	*/
/* added 2009.08.25 hmenjo SE ランプ制御 ---------- } ---------- */
		// 2008.12.30 matsuo add ----->
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_HWS_MEASING);	// Status Bar
		_stprintf(l_tszLogText, _T("Measuring Point No. %5d."), m_dwPointNo);
		((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
		// 2009.08.18 K.Matsuo ストレス対応 -->
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- { ---------- */
////		MEAS_Measure(l_pMeasProgInfo, m_dwPointNo);
//		MEAS_Measure(l_pMeasProgInfo, m_dwPointNo, &m_MeasPosXYZ);
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ----------				*/
// 2009.09.29 K.Matsuo GTr -->
		switch (m_enMeasMode) {
		case MEASMODE_GTRREF:
			MEAS_GTrReferenceMeasure(m_dwPointNo, &m_MeasPosXYZ);
			break;
		default:
			MEAS_Measure(m_dwPointNo, &m_MeasPosXYZ);
			break;
		}
// 2009.09.29 K.Matsuo GTr <--
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- } ---------- */
/* added 2009.08.25 hmenjo SE ランプ制御 ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 2);	/* SE ランプフィルタ CLOSE(通常のみ)	*/
/* added 2009.08.25 hmenjo SE ランプ制御 ---------- } ---------- */
		// 2009.08.18 K.Matsuo ストレス対応 <--
		// 2008.12.30 matsuo add <-----
		m_MeasPosXYZ.lZ = 0;
#ifdef CHIEF_USE_SLEEP
		::Sleep(2000);
/* added 2009.08.20 hmenjo ストレス機能追加(48) ---------- { ---------- */
#else
		::Sleep(200);
/* added 2009.08.20 hmenjo ストレス機能追加(48) ---------- } ---------- */
#endif
		//		ここには処理はありません．
	} else {
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
///* added 2009.08.25 hmenjo SE ランプ制御 ---------- { ---------- */
///* added 2009.08.31 hmenjo SE ヘッドオフセット対応 ---------- { ---------- */
//		/* ヘッドオフセット移動	*/
///* added 2009.09.30 hmenjo SE ヘッドオフセット手動測定対応 ---------- { ---------- */
//		BOOL l_bRestoreSRHead = FALSE;
///* added 2009.09.30 hmenjo SE ヘッドオフセット手動測定対応 ---------- } ---------- */
//		if (HEAD_TYPE_SE == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
///* modified 2009.08.31 hmenjo SE ヘッドオフセット対応(11) ---------- { ---------- */
////			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
////			BOOL l_bRet =	NS_SampleCoordStageMoveAbsolute(
////									(STAGE_COORD*) &m_MeasPosXYZ,
////									l_pMainRcpInfo->hdr.szName,
////									HEAD_TYPE_SE
////								);
////			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 動作中フラグ(XY ステージ)をオフ
///* modified 2009.08.31 hmenjo SE ヘッドオフセット対応(11) ----------				*/
//			/* SE ヘッド	*/
///* modified 2009.09.01 hmenjo SE 構造体追加(12) ---------- { ---------- */
////			SE_CONFIG l_SeConfig;
////			ConfigFile_GetNanoSpecIni(&l_SeConfig, CONFIG_FILE_SE_CONFIG);
////			if (
////				FALSE ==	(
////								(TRUE == l_SeConfig.bDoNotMoveShutter)
////							 && ((0 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) || ((1 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) && (1 != m_dwPointNo)))
////							 && (0 == this->IsPointManualAdjust(m_dwPointNo))
////							)
////				) {
///* modified 2009.09.01 hmenjo SE 構造体追加(12) ----------			  */
//			SE_SETTING l_SeSetting;
//			ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
////			if (
////				FALSE ==	(
////								(TRUE == l_SeSetting.bDoNotMoveShutter)
////							 && ((0 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) || ((1 == l_pMainRcpInfo->MainRcpParam._SR.nFocus) && (1 != m_dwPointNo)))
////							 && (0 == this->IsPointManualAdjust(m_dwPointNo))
////							)
////				) {
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ----------				*/
//			HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
//			((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, l_pMeasProgInfo->ScanParams.hdr.wHeadType, &l_HeadMainRcpInf);
//			if (
//				FALSE ==	(
//								(TRUE == l_SeSetting.bDoNotMoveShutter)
//							 && ((0 == l_HeadMainRcpInf.nFocus) || ((1 == l_HeadMainRcpInf.nFocus) && (1 != m_dwPointNo)))
//							 && (0 == this->IsPointManualAdjust(m_dwPointNo))
//							)
//				) {
///* modified 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
///* modified 2009.09.01 hmenjo SE 構造体追加(12) ---------- } ---------- */
//				/*	運用モードで，かつ，(AF 無し，あるいは，１ポイント目のみ AF 有りで２ポイント目以降)，
//				 *	かつ，PMA 無し，の場合はヘッドオフセットしません．
//				 *	以下は上記条件が成立しなかった場合に実行されます．	*/
//				((CChiefView*) m_pcChiefView)->LogChief(_T("Moving Head-offset for SE."));
//				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
//				/* ウェイト無しの移動関数のため，移動完了までこのスレッドは待機させられます．	*/
///* modified 2009.10.30 hmenjo CTA Head Offset 不要部削除 ---------- { ---------- */
/////* modified 2009.09.30 hmenjo SE ヘッドオフセット手動測定対応 ---------- { ---------- */
//////				BOOL l_bRet =	NS_SampleCoordStageMoveAbsolute(
//////										(STAGE_COORD*) &m_MeasPosXYZ,
//////										l_pMainRcpInfo->hdr.szName,
//////										HEAD_TYPE_SE
//////									);
/////* modified 2009.09.30 hmenjo SE ヘッドオフセット手動測定対応 ----------			  */
////				BOOL l_bRet;
////				if (0) {
////					l_bRet =	NS_SampleCoordStageMoveAbsolute(
////										(STAGE_COORD*) &m_MeasPosXYZ,
////										l_pMainRcpInfo->hdr.szName,
////										HEAD_TYPE_SE
////									);
////				} else {
////					l_bRestoreSRHead = TRUE;
////					l_bRet = 	SelectHeadTypeNotify(HEAD_TYPE_SE, TRUE);
////				}
/////* modified 2009.09.30 hmenjo SE ヘッドオフセット手動測定対応 ---------- } ---------- */
///* modified 2009.10.30 hmenjo CTA Head Offset 不要部削除 ----------			   */
//				l_bRestoreSRHead = TRUE;
//				BOOL l_bRet = SelectHeadTypeNotify(HEAD_TYPE_SE, TRUE);
///* modified 2009.10.30 hmenjo CTA Head Offset 不要部削除 ---------- } ---------- */
//				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/
///* modified 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- { ---------- */
////				((CChiefView*) m_pcChiefView)->LogChief(_T("Moved Head-offset for SE."));
///* modified 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ----------				*/
//				if (0 == l_bRet) {
//					/* 移動開始失敗	*/
//					((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move Head-offset for SE."));
//					((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
//					// トランジション更新
//					TransiEvent(EV_SR1P_MEAS_ERR);
//					// 呼出元トランジションに完了を通知
//					NotifyComplete((cEventParams*) TR_SR1P_XYMOVE_FAIL);
//					return;
//				} else {
//					((CChiefView*) m_pcChiefView)->LogChief(_T("Moved Head-offset for SE."));
//				}
///* modified 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- } ---------- */
//			}
///* modified 2009.08.31 hmenjo SE ヘッドオフセット対応(11) ---------- } ---------- */
//		}
///* added 2009.08.31 hmenjo SE ヘッドオフセット対応 ---------- } ---------- */
///* modified 2009.11.19 hmenjo CTA 1ポイント単体起動時 Head Offset追加 ---------- { ---------- */
/////* added 2009.11.06 K.Matsuo Resist ヘッドオフセット対応 ---------- { ---------- */
////		if (HEAD_TYPE_4PP == l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
////			/* RS ヘッド	*/
////			HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
////			((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, l_pMeasProgInfo->ScanParams.hdr.wHeadType, &l_HeadMainRcpInf);
////
////			((CChiefView*) m_pcChiefView)->LogChief(_T("Moving Head-offset for RS."));
////			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
////			/* ウェイト無しの移動関数のため，移動完了までこのスレッドは待機させられます．	*/
////			l_bRestoreSRHead = TRUE;
////			BOOL l_bRet = SelectHeadTypeNotify(HEAD_TYPE_4PP, TRUE);
////			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/
////			if (0 == l_bRet) {
////				/* 移動開始失敗	*/
////				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move Head-offset for RS."));
////				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
////				// トランジション更新
////				TransiEvent(EV_SR1P_MEAS_ERR);
////				// 呼出元トランジションに完了を通知
////				NotifyComplete((cEventParams*) TR_SR1P_XYMOVE_FAIL);
////				return;
////			} else {
////				((CChiefView*) m_pcChiefView)->LogChief(_T("Moved Head-offset for RS."));
////			}
////		}
/////* added 2009.11.06 K.Matsuo Resist ヘッドオフセット対応 ---------- } ---------- */
///* modified 2009.11.19 hmenjo CTA 1ポイント単体起動時 Head Offset追加 ----------				*/
//		if ((HEAD_TYPE_4PP == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
//		 || ((HEAD_TYPE_CTA == l_pMeasProgInfo->ScanParams.hdr.wHeadType) && (TRANSI_MASTER == m_dwParentTransi))) {
//			/* RS / CTA ヘッド	*/
//			HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
//			((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, l_pMeasProgInfo->ScanParams.hdr.wHeadType, &l_HeadMainRcpInf);
//			TCHAR l_tszLog[128];
//			TCHAR l_tszHeadName[32];
//			switch (l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
//			case HEAD_TYPE_4PP:		_tcscpy(l_tszHeadName, _T("RS"));	break;
//			case HEAD_TYPE_CTA:		_tcscpy(l_tszHeadName, _T("CTA"));	break;
//			default:
//				ASSERT(0);
//				break;
//			}
//			_stprintf(l_tszLog, _T("Moving Head-offset for %s."), l_tszHeadName);
//			((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
//			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* 動作中フラグ(XY ステージ)をオン	*/
//			/* ウェイト無しの移動関数のため，移動完了までこのスレッドは待機させられます．	*/
//			l_bRestoreSRHead = TRUE;
//			  BOOL l_bRet = SelectHeadTypeNotify(l_pMeasProgInfo->ScanParams.hdr.wHeadType, TRUE);
//			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* 動作中フラグ(XY ステージ)をオフ	*/
//			if (0 == l_bRet) {
//				/* 移動開始失敗	*/
//				_stprintf(l_tszLog, _T("Failed to move Head-offset for %s."), l_tszHeadName);
//				((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
//				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
//				// トランジション更新
//				TransiEvent(EV_SR1P_MEAS_ERR);
//				// 呼出元トランジションに完了を通知
//				NotifyComplete((cEventParams*) TR_SR1P_XYMOVE_FAIL);
//				return;
//			} else {
//				_stprintf(l_tszLog, _T("Moved Head-offset for %s."), l_tszHeadName);
//				((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
//			}
//		}
///* modified 2009.11.19 hmenjo CTA 1ポイント単体起動時 Head Offset追加 ---------- } ---------- */
//		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_OPEN, 1);	/* SE ランプフィルタ OPEN	*/
///* added 2009.08.25 hmenjo SE ランプ制御 ---------- } ---------- */
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------			  */
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
		/* ヘッドオフセット前の Z 軸位置を記憶	*/
		{
			STAGE_COORD_XYZ l_MeasPosXYZ;
			NS_StageGetPos(&l_MeasPosXYZ);
			m_lZ_AF = l_MeasPosXYZ.lZ;
		}
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
		WORD l_wRcpHead = l_pMeasProgInfo->ScanParams.hdr.wHeadType;
		/* ストレス測定内の膜厚測定 判定	*/
		if (l_wRcpHead == HEAD_TYPE_STRESS) {
			l_wRcpHead = HEAD_TYPE_SR;
		}
		SE_SETTING l_SeSetting;
		ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		COMPEASE_SETTING l_EASESetting;
		ConfigFile_GetNanoSpecIni(&l_EASESetting, CONFIG_FILE_COMPEASE_SETTING);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		/* AF 有無 確認	*/
		HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
		((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, l_wRcpHead, &l_HeadMainRcpInf);
		BOOL l_bAF = ((1 == l_HeadMainRcpInf.nFocus) && (1 == m_dwPointNo)) || (2 == l_HeadMainRcpInf.nFocus);
		/* PMA 有無 確認	*/
		BOOL l_bPMA = this->IsPointManualAdjust(m_dwPointNo);
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
		/* ヘッドタイプ判定とヘッドオフセット移動	*/
		WORD l_wOffsetHead = 0xffff;
		if ((HEAD_TYPE_MS != l_wRcpHead) && ((0 != l_bAF) || (0 != l_bPMA))) {
			l_wOffsetHead = l_wRcpHead;		/* MS 以外で AF 有り，または，PMA 有り，はレシピヘッド	*/
		}
		if ((HEAD_TYPE_MS == l_wRcpHead) && (0 != l_bAF) && (0 == l_bPMA)) {
			l_wOffsetHead = l_wRcpHead;		/* MS で AF 有り，かつ，PMA 無し，はレシピヘッド	*/
		}
		if ((HEAD_TYPE_SE == l_wRcpHead) && (TRUE != l_SeSetting.bDoNotMoveShutter)) {
			l_wOffsetHead = l_wRcpHead;		/* SE で DoNotMoveShutter == FALSE は，SE(レシピヘッド)	*/
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if ((HEAD_TYPE_COMPEASE == l_wRcpHead) && (TRUE != l_EASESetting.bDoNotMoveShutter)) {
			l_wOffsetHead = l_wRcpHead;		/* EASE で DoNotMoveShutter == FALSE は，EASE(レシピヘッド)	*/
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		if (0 != l_bSitePR) {
			l_wOffsetHead = l_wRcpHead;		/* SitePR 有りは，レシピヘッド	*/
		}
		if ((TRANSI_MASTER == m_dwParentTransi) && (HEAD_TYPE_MS != l_wRcpHead)) {
			l_wOffsetHead = l_wRcpHead;		/* 単独起動で MS 以外は，レシピヘッド	*/
		}
		if (0xffff != l_wOffsetHead) {
			/* ヘッドオフセット移動	*/
			((CChiefView*) m_pcChiefView)->SelectHead(l_wOffsetHead, TRUE);
		}

		/* Z 軸退避位置へ移動	*/
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
		BOOL l_bTRmode =	(HEAD_TYPE_SR == l_wRcpHead)
						 && ((MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
						  || (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_pMeasProgInfo->ScanParams.hdr.wScanType));
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
		if (
			(HEAD_TYPE_4PP == l_wRcpHead)		/* 4PP の場合	*/
		 || (HEAD_TYPE_CTA == l_wRcpHead)		/* CTA の場合	*/
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
		 || (0 != l_bTRmode)					/* 透過率の場合	*/
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
			) {
			long l_lZRetractPos;
			switch (l_wRcpHead) {
			case HEAD_TYPE_4PP:
				{
					RS_CONFIG l_RsConfig;
					ConfigFile_GetNanoSpecIni(&l_RsConfig, CONFIG_FILE_RS_CONFIG);
					l_lZRetractPos = l_RsConfig.lRsStdZAxisRetractPos;
				}	break;
			case HEAD_TYPE_CTA:
				{
					CA_CONFIG l_CaConfig;
					ConfigFile_GetNanoSpecIni(&l_CaConfig, CONFIG_FILE_CA_CONFIG);
					l_lZRetractPos = l_CaConfig.lCaStdZAxisRetractPos;
				}	break;
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
			case HEAD_TYPE_SR:
				if (0 != l_bTRmode) {
					/* 透過率の場合は初期化停止位置	*/
					EEPROMDATA	l_E2pRomParams;
					StageGetEEPromData(&l_E2pRomParams);
					long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	/* Z 軸の初期化停止位置	*/
					long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu;	/* Z 軸の初期化停止位置ファクタ	*/
					/* 初期化停止位置[pulse]を[0.1um]に変換します	*/
					LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
					l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	/* 初期化停止位置[pulse]÷ファクタ[pulse/mm]	*/
					l_l64_Z = l_l64_Z * ((LONG64) 10000);	/* [0.1um]に変換する	*/
					l_lZRetractPos = (long) (l_l64_Z / ((LONG64) 1000000));
					NS_ConvertToStageMoveCoord(&l_lZRetractPos);		/* 補正	*/
				}
				break;
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
			}
			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		/* 動作中フラグ(Z 軸)をオン	*/
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);	/* Status Bar	*/
			if (0 == StageElevatorMove(l_lZRetractPos)) {
				/* 移動開始失敗	*/
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
				/* トランジション更新	*/
				TransiEvent(EV_SR1P_MEAS_ERR);
				/* 呼出元トランジションに完了を通知	*/
				NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
			} else {
				/* 移動完了	*/
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	/* Status Bar	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to Retract position."));
				/* Z 軸退避位置チェック	*/
				BOOL l_bZRetract = TRUE;
				switch (l_wRcpHead) {
				case HEAD_TYPE_4PP:
				case HEAD_TYPE_CTA:
					::Sleep(100);	/* センサ入力遅れ待ち	*/
					l_bZRetract = nexioIsRetractPosZ();
					break;
				}
				if (0 == l_bZRetract) {
					((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
					/* トランジション更新	*/
					TransiEvent(EV_SR1P_MEAS_ERR);
					/* 呼出元トランジションに完了を通知	*/
					NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
				}
			}
			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		/* 動作中フラグ(Z 軸)をオフ	*/
		}

		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_OPEN, 1);	/* ランプフィルタ OPEN	*/
/* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_MEASING);	// Status Bar
		_stprintf(l_tszLogText, _T("Measuring Point No. %5d."), m_dwPointNo);
		((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
		/* 測定直前の Z 軸位置を画面/データモジュール通知用に取得	*/
		{
			STAGE_COORD_XYZ l_MeasPosXYZ;
			NS_StageGetPos(&l_MeasPosXYZ);
			m_MeasPosXYZ.lZ = l_MeasPosXYZ.lZ;
		}
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- { ---------- */
//		// 2009.08.18 K.Matsuo ストレス対応 -->
////		if (0 == MEAS_Measure(l_pMeasProgInfo, m_dwPointNo)) {
//		if (0 == MEAS_Measure(l_pMeasProgInfo, m_dwPointNo, &m_MeasPosXYZ)) {
//		// 2009.08.18 K.Matsuo ストレス対応 <--
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ----------				*/
		BOOL l_bRetMeas;
// 2009.09.29 K.Matsuo GTr -->
		switch (m_enMeasMode) {
		case MEASMODE_GTRREF:
			l_bRetMeas = MEAS_GTrReferenceMeasure(m_dwPointNo, &m_MeasPosXYZ);
			break;
		default:
			l_bRetMeas = MEAS_Measure(m_dwPointNo, &m_MeasPosXYZ);
			break;
		}
// 2009.09.29 K.Matsuo GTr <--
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
///* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
/////* added 2009.09.30 hmenjo SE ヘッドオフセット手動測定対応 ---------- { ---------- */
////		if (TRUE == l_bRestoreSRHead) {
////			SelectHeadTypeNotify(HEAD_TYPE_SR, FALSE);
////		}
/////* added 2009.09.30 hmenjo SE ヘッドオフセット手動測定対応 ---------- } ---------- */
///* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ----------				*/
//		((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
///* modified 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ----------			  */
		if ((TRANSI_MASTER == m_dwParentTransi) && (0xffff != l_wOffsetHead)) {
			/* 単独起動でヘッドオフセット移動されていた場合は SR オフセットに戻します．	*/
				/*	自動シーケンスの場合は，先行移動があるので「絶対」にここが実行されないようにすること！！！！	*/
			((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, TRUE);
		} else {
			((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
		}
/* modified 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
		if (0 == l_bRetMeas) {
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- } ---------- */
/* added 2009.08.25 hmenjo SE ランプ制御 ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* SE ランプフィルタ CLOSE	*/
/* added 2009.08.25 hmenjo SE ランプ制御 ---------- } ---------- */
			// 測定エラー
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_MEAS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
			l_bMeasErr = TRUE;
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementError));
			// 呼出元トランジションに完了を通知
			NotifyComplete((cEventParams*) TR_SR1P_MEAS_ERR);
			TransiEvent(EV_SR1P_MEAS_ERR);
		} else {
			// 測定正常
/* added 2009.08.25 hmenjo SE ランプ制御 ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 2);	/* SE ランプフィルタ CLOSE(通常のみ)	*/
/* added 2009.08.25 hmenjo SE ランプ制御 ---------- } ---------- */
			//		ここには処理はありません．
		}
	}

	if (0 == l_bMeasErr) {
		// 測定正常の場合
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_ANALYZING);	// Status Bar
		_stprintf(l_tszLogText, _T("Analyzing Point No. %5d."), m_dwPointNo);
		((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- { ---------- */
//		if (0 == MEAS_Analysis(l_pMainRcpInfo, l_pMeasProgInfo, m_dwPointNo, &m_xmpDde)) {
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ----------				*/
		BOOL l_bRetAna;
// 2009.09.29 K.Matsuo GTr -->
		switch (m_enMeasMode) {
		case MEASMODE_GTRREF:
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
		case MEASMODE_CTA:			/* CTA 測定は解析不要	*/
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case MEASMODE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			l_bRetAna = TRUE;			// GTrリファレンス測定なので、解析不要
			break;
		default:
{//hdebdeb
TCHAR l_tszIniPath[_MAX_PATH];
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//_stprintf(l_tszIniPath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
_tcscpy(l_tszIniPath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
DWORD l_dwDT = ::GetPrivateProfileInt(_T("PreMove"), _T("tstdelay"), 0, l_tszIniPath);//hdebdeb
::Sleep(l_dwDT);//hdebdeb
}//hdebdeb
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->LogChief(_T("MEAS_Analysis() before call."));
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- } ---------- */
			l_bRetAna = MEAS_Analysis(m_dwPointNo, &m_xmpDde);
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->LogChief(_T("MEAS_Analysis() after call."));
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- } ---------- */
			break;
		}
// 2009.09.29 K.Matsuo GTr <--
		if (0 == l_bRetAna) {
/* modified 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- } ---------- */
			// 解析エラー
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_ANALYZE_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_AnalysisError));
			// トランジション更新
			TransiEvent(EV_SR1P_ANA_ERR);
			// 呼出元トランジションに完了を通知
			NotifyComplete((cEventParams*) TR_SR1P_ANA_ERR);
		} else {
			// 測定終了
			TransiEvent(EV_SR1P_MEAS_DONE);
		}
	}
}

/*
 *	測定データ処理完了待ち
 */
void CChiefTransiSr1Point::enter_ST_SR1P_ANA(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[16], (DWORD) pEvParams);

	// 測定データ処理発行
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_DATA_PROCING);	// Status Bar
	// 測定ポイント番号と VALID データをデータ処理へ通知
	MEAS_1POINT_RESULT l_Meas1PointResult;
	l_Meas1PointResult.dwPointNo = m_dwPointNo;
	l_Meas1PointResult.dwAFValid = m_dwAFValid;
	l_Meas1PointResult.lX = m_MeasPosXYZ.lX;
	l_Meas1PointResult.lY = m_MeasPosXYZ.lY;
	l_Meas1PointResult.lZ = m_MeasPosXYZ.lZ;
	COPYDATASTRUCT l_CopyData;
	l_CopyData.dwData = WMCD_CHIF_1POINT_END;
	l_CopyData.cbData = sizeof(MEAS_1POINT_RESULT);
	l_CopyData.lpData = &l_Meas1PointResult;
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);
	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("Notify WMCD_CHIF_1POINT_END with OK. (Point No.= %5d, AF Valid = 0x%08x)."), m_dwPointNo, m_dwAFValid);
	((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);

	// WAIT 状態にするためトランジションを更新しません．
}

/*
 *	測定データ処理完了(WAIT 状態無し)
 */
void CChiefTransiSr1Point::enter_ST_SR1P_ANA_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[17], (DWORD) pEvParams);

	// 測定データ処理完了の処理
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SR1P_DATA_PROC_COMPLETE);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

	// トランジション更新
	TransiEvent(EV_SR1P_IDLE);

	// 呼出元トランジションに完了を通知
	NotifyComplete((cEventParams*) m_dwAFValid);

}

//2009.10.28 bagus 2point-distance --{--
// 2点間距離測定
void CChiefTransiSr1Point::enter_ST_SR1P_DISTANCE(const cEventParams* const pEvParams /*= 0*/)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[18], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DISTANCE);	// Status Bar

	//画面にPoint Teachingの画面を出すように指示をだす。
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_DISTANCE_POPUP, (WPARAM) pEvParams, m_dwPointNo);

	//WAIT状態にするためにここではトランジションを変更しない
}
// キャンセル処理
void CChiefTransiSr1Point::enter_ST_SR1P_DISTANCE_CANCEL(const cEventParams* const pEvParams /*= 0*/)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[19], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_DISTANCE_CANCEL);	// Status Bar

	// トランジション更新
	TransiEvent(EV_SR1P_IDLE);

	// 測定キャンセルを上位に通知
	NotifyComplete((cEventParams*) TR_SR1P_MEAS_ERR);
}
//2009.10.28 bagus 2point-distance --}--
//2009.11.03 bagus MS --{--
void CChiefTransiSr1Point::enter_ST_SR1P_MS(const cEventParams* const pEvParams /*= 0*/)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[20], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_MS);	// Status Bar
	//
	if (0 == ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		SelectHeadTypeNotify(HEAD_TYPE_MS, FALSE);
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK,1);
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
		/* MS の場合は必ず SR フィルタを CLOSE	*/
		if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_DARK)) {
			/* 異常時でも無視します．	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSr1Point::enter_ST_SR1P_MS()."));
		}
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	}

	//画面にPoint Teachingの画面を出すように指示をだす。
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_MS_POPUP, (WPARAM) pEvParams, m_dwPointNo);

	//WAIT状態にするためにここではトランジションを変更しない
}
void CChiefTransiSr1Point::enter_ST_SR1P_MS_CANCEL(const cEventParams* const pEvParams /* = 0*/)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_1POINT_DISP[21], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_MS_CANCEL);	// Status Bar

	if (0 == ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		SelectHeadTypeNotify(HEAD_TYPE_SR, FALSE);
/* deleted 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_OPEN,1);
/* deleted 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	}

	// トランジション更新
	TransiEvent(EV_SR1P_IDLE);

	// 測定キャンセルを上位に通知
	NotifyComplete((cEventParams*) TR_SR1P_MEAS_ERR);
}
void CChiefTransiSr1Point::enter_ST_SR1P_MS_DONE(const cEventParams* const pEvParams /*= 0*/)
{
	if (0 == ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		SelectHeadTypeNotify(HEAD_TYPE_SR, FALSE);
/* deleted 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_OPEN,1);
/* deleted 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	}
	//2009.11.12 bagus MS --{--
	//マニュアル測定時にはデータを送らない
	if(((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetMeasMode() == MEASMODE_MANUALMEASURE){
		TransiEvent(EV_SR1P_MS_NO_ANA);
	}else{
		TransiEvent(EV_SR1P_IDLE);
	}
	//2009.11.12 bagus MS --}--
}
//2009.11.03 bagus MS --}--
