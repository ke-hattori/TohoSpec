// ChifRcvMailThread.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "ChiefView.h"
#include "ChifRcvMailThread.h"
#include "..\\..\\INC\\PifComm.hxx"
#include "..\\..\\INC\\NEXIF.HXX"
#include "ChiefExports.h"
#include "..\\..\\INC\\Active.hxx"
#include "ChifTransiMaster.h"
#include "ChifTransiSeq.h"
#include "NanoSpecDoc.h"
//#include "MainFrm.h"
/* added 2009.08.03 hmenjo ストレス機能追加(12) ---------- { ---------- */
#include "ChifTransiStress.h"
/* added 2009.08.03 hmenjo ストレス機能追加(12) ---------- } ---------- */
/* added 2009.08.07 hmenjo ストレス機能追加(35) ---------- { ---------- */
#include "ChiefPFuncs.h"
/* added 2009.08.07 hmenjo ストレス機能追加(35) ---------- } ---------- */
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
#include "..\\..\\inc\\NEXIOBASE.HXX"
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChiefRcvMailThread

IMPLEMENT_DYNCREATE(CChiefRcvMailThread, CWinThread)

CChiefRcvMailThread::CChiefRcvMailThread()
{
	TRACE(_T("CChiefRcvMailThread::CChiefRcvMailThread() \n"));
}

CChiefRcvMailThread::~CChiefRcvMailThread()
{
	TRACE(_T("CChiefRcvMailThread::~CChiefRcvMailThread() \n"));
}

BOOL CChiefRcvMailThread::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefRcvMailThread::InitInstance() \n"));
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefRcvMailThread."));

	m_bShutDown = FALSE;
//	m_bAutoDelete = FALSE;			// スレッド起動処理に移動(その方が判りやすいので)
	m_mailslot.Create(MAILSLOT_CHIEF);
	m_strRcvCmd = "";

	return TRUE;
}

int CChiefRcvMailThread::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefRcvMailThread::ExitInstance() \n"));
	((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefRcvMailThread."));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefRcvMailThread, CWinThread)
	//{{AFX_MSG_MAP(CChiefRcvMailThread)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefRcvMailThread メッセージ ハンドラ

/*
 *	スレッドループ
 */
int CChiefRcvMailThread::Run()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	TRACE(_T("CChiefRcvMailThread::Run() \n"));

	DWORD l_dwnNumberOfBytesRead;

	const int l_inMessageMaxLen = 256;
	TCHAR l_szRecvBuff[l_inMessageMaxLen];

	while (!m_bShutDown) {
		if (0 != m_mailslot.Recieve(l_szRecvBuff, &l_dwnNumberOfBytesRead)) {
			((CChiefView*) m_pcChiefView)->LogChief(l_szRecvBuff);
			CString l_strRcvCmd = l_szRecvBuff;
			m_strRcvCmd = l_strRcvCmd;
			m_pcChiefView->PostMessage(WM_CHIF_RECV_PIFCMD, 0, 0);		// Pif コマンド受信を通知
			if (		l_strRcvCmd == PIFCOMM_P201) {
				EventProcessP201();						// 時刻合わせ要求
// 2009.02.05 K.Matsuo delete -->
//			} else if ( l_strRcvCmd == PIFCOMM_P306) {
//				EventProcessP306();						// トレースデータの取得要求
// 2009.02.05 K.Matsuo delete <--
			} else if ( l_strRcvCmd == PIFCOMM_P401) {
				EventProcessP401();						// 着工レシピ指示要求
/* added 2009.09.10 hmenjo P411 コマンド追加 ---------- { ---------- */
			} else if ( l_strRcvCmd == PIFCOMM_P411) {
				EventProcessP411();						// 着工レシピ指示要求
/* added 2009.09.10 hmenjo P411 コマンド追加 ---------- } ---------- */
			} else if ( l_strRcvCmd == PIFCOMM_P501) {
				EventProcessP501();						// 測定開始指示要求
/* added 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
			} else if ( l_strRcvCmd == PIFCOMM_P511) {
				EventProcessP511();						// 測定開始指示要求
/* added 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
// 2014.01.07 bagus Add(Stage None対応) -->
			} else if ( l_strRcvCmd == PIFCOMM_P514) {
				EventProcessP514();						// Si リファレンス取得開始指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P516) {
				EventProcessP516();						// Dark リファレンス取得開始指示要求
// 2014.01.07 bagus Add(Stage None対応) <--
			} else if ( l_strRcvCmd == PIFCOMM_P601) {
				EventProcessP601();						// ロードポジション移行指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P602) {
				EventProcessP602();						// 汎用ポジション移行指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P603) {
				EventProcessToNextra(603);				// バキューム ON 指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P604) {
				EventProcessToNextra(604);				// バキューム OFF 指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P605) {
				EventProcessToNextra(605);				// アライメント LOAD 指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P606) {
				EventProcessToNextra(606);				// アライメント UNLOAD 指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P607) {
				EventProcessToNextra(607);				// PINUP 指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P608) {
				EventProcessToNextra(608);				// PINDOWN 指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P609) {
				EventProcessToNextra(609);				// シャッタ OPEN 指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P610) {
				EventProcessToNextra(610);				// シャッタ CLOSE 指示要求
			} else if ( l_strRcvCmd == PIFCOMM_P611) {
				EventProcessP611();						// 測定キャンセル指示要求
			} else {
				// Pif 側でチェック後，このスレッドに渡されるのでココには来ないはず・・・
				ASSERT(FALSE);
			}
		}
		::Sleep(10);
	}

	::PostQuitMessage(0);

	return CWinThread::Run();
}

/*
 *	スレッド終了させる
 */
void CChiefRcvMailThread::ShutDown()
{
	m_bShutDown = TRUE;
}

/*
 *	Pif から受信したコマンドを返す
 */
CString CChiefRcvMailThread::GetPifRcvCmd()
{
	CString	l_strRc;
	TCHAR	l_szTemp[8] = "";

	// 例外処理を書きたくなかったので，一旦，型変換してます．
	_tcsncpy(l_szTemp, m_strRcvCmd, 4);
	l_strRc = l_szTemp;

	return l_strRc;
}

/*
 *	コマンド毎の処理関数群 ---------------------------------------------------
 */
// Nextra 指示専用コマンド処理
void CChiefRcvMailThread::EventProcessToNextra(
		DWORD dwCmdCode		// Pxxx の xxx を数値で指定します．
	)
{
	// 最初にインタロック
	DWORD l_dwEMO = ((CChiefView*) m_pcChiefView)->CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
//		PifComm_DoStateDone(60 + l_dwEMO);	// EMO，ドアインタロック，装置パワーオフ
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ----------			   */
		if (ALID_SafetyPlugOpen != l_dwEMO) {
			PifComm_DoStateDone(60 + l_dwEMO);	/* EMO，ドアインタロック，装置パワーオフ	*/
		} else {
			PifComm_DoStateDone(69);	/* セーフティプラグ	*/
		}
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
		return;
	}
	if ((610 == dwCmdCode) && (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsRobotArmON())) {
		PifComm_DoStateDone(64);	// ロボットアーム検出では，シャッタ閉は禁止
		return;
	}
	if (
		(0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW)
	 && ((0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterClose) || (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterOpen))
	) {
		// 東朋メンテがオフで，かつ，シャッタ CLOSE がオフ，か，シャッタ OPEN がオン の場合
		switch (dwCmdCode) {
		case 603:	// バキューム ON 指示要求
		case 604:	// バキューム OFF 指示要求
		case 605:	// アライメント LOAD 指示要求
		case 606:	// アライメント UNLOAD 指示要求
		case 607:	// PINUP 指示要求
		case 608:	// PINDOWN 指示要求
/* modified 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- { ---------- */
//			PifComm_DoStateDone(67);
//			return;
/* modified 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- 			 */
			if (0 == ((CChiefView*) m_pcChiefView)->m_DioIgnoreSW.bPinShutterILInvalid) {
				PifComm_DoStateDone(67);
				return;
			}
/* modified 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- } ---------- */
			break;
		default:
			break;
		}
	}
	int l_iProcStatus = ((CChiefView*) m_pcChiefView)->ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		PifComm_DoStateDone(32);	// Wait，Down 以外だった
		return;
	}
/* modified 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- { ---------- */
//#if 0	// ターレット動作中をフラグに追加したため，ターレットは除外
//	if (0 != ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
//#else	// ターレット動作中をフラグに追加したため，ターレットは除外
//	DWORD l_dwActFlags = ACTUATE_XYSTAGE | ACTUATE_ZAXIS | ACTUATE_PIN | ACTUATE_SHUTTER | ACTUATE_WORKGUIDE;
//	if (0 != (l_dwActFlags & ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll())) {
//#endif	// ターレット動作中をフラグに追加したため，ターレットは除外
/* modified 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- 			 */
	DWORD l_dwActFlags = ACTUATE_XYSTAGE | ACTUATE_ZAXIS;
	if (0 == ((CChiefView*) m_pcChiefView)->m_DioIgnoreSW.bPinShutterILInvalid) {
		l_dwActFlags |= ACTUATE_PIN | ACTUATE_SHUTTER | ACTUATE_WORKGUIDE;
	}
	if (0 != (l_dwActFlags & ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll())) {
/* modified 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- } ---------- */
		PifComm_DoStateDone(41);	// 動作中フラグがオンだった
		return;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		PifComm_DoStateDone(51);	// 画面モードが「測定モード」か「データモード」でなかった
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == ((CChiefView*) m_pcChiefView)->IsCtaILPI()) {
		switch (dwCmdCode) {
		case 605:	/* アライメント LOAD 指示要求	*/
		case 606:	/* アライメント UNLOAD 指示要求	*/
		case 607:	/* PINUP 指示要求	*/
		case 608:	/* PINDOWN 指示要求	*/
			PifComm_DoStateDone(72);	/* CTAILPI がオフ	*/
			return;
			break;
		default:
			break;
		}
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = ((CChiefView*) m_pcChiefView)->CheckResistIL();
	if (0 != iResistStatus) {
		int iAckCode = 74 + iResistStatus - 1;		// 74,75,76
		switch (dwCmdCode) {
		case 605:	/* アライメント LOAD 指示要求	*/
		case 606:	/* アライメント UNLOAD 指示要求	*/
		case 607:	/* PINUP 指示要求	*/
		case 608:	/* PINDOWN 指示要求	*/
			PifComm_DoStateDone(iAckCode);	/* Resist上昇端センサー がオフ	*/
			return;
			break;
		default:
			break;
		}
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- } ---------- */
// 2009.11.12 bagus MS 下端位置2チェック --{--
	if (0 == ((CChiefView*) m_pcChiefView)->IsMSILPI()) {
		switch (dwCmdCode) {
		case 605:	/* アライメント LOAD 指示要求	*/
		case 606:	/* アライメント UNLOAD 指示要求	*/
		case 607:	/* PINUP 指示要求	*/
		case 608:	/* PINDOWN 指示要求	*/
			PifComm_DoStateDone(77);	/* 下端位置２センサーON*/
			return;
			break;
		default:
			break;
		}
	}
// 2009.11.12 bagus MS 下端位置2チェック --}--
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (609 == dwCmdCode) {
			PifComm_DoStateDone(42);	// シャッタ開は禁止
			return;
		}
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			// さらに，東朋メンテ SW がオフの場合・・・
			if (605 == dwCmdCode) {
				PifComm_DoStateDone(43);	// アライメント LOAD は禁止
				return;
			}
			if (606 == dwCmdCode) {
				PifComm_DoStateDone(43);	// アライメント UNLOAD は禁止
				return;
			}
		}
	}
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bLoadPos) {
		// ロードポジションでない場合・・・
		if ((605 == dwCmdCode) || (606 == dwCmdCode)) {
			// アライメントロード/アンロードは禁止
			PifComm_DoStateDone(44);	// アライメント LOAD/UNLOAD は禁止
			return;
		}
	}

/* added 2010.05.21 hmenjo Nextra 指示前に正常応答する ---------- { ---------- */
	PifComm_DoStateDone(0);	/* 正常応答送信	*/
	::Sleep(500);	/* 0.5s 待ちます．	*/
/* added 2010.05.21 hmenjo Nextra 指示前に正常応答する ---------- } ---------- */
	int		l_iResult = 0;
	BOOL	l_bNexResult;
	switch (dwCmdCode) {
	case 603:	l_bNexResult = nexifVacuumOn(m_pcChiefView->m_hWnd);		break;	// バキューム ON 指示要求
	case 604:	l_bNexResult = nexifVacuumOff(m_pcChiefView->m_hWnd);		break;	// バキューム OFF 指示要求
	case 605:	l_bNexResult = nexifLoad(m_pcChiefView->m_hWnd);			break;	// アライメント LOAD 指示要求
	case 606:	l_bNexResult = nexifUnload(m_pcChiefView->m_hWnd);			break;	// アライメント UNLOAD 指示要求
	case 607:	l_bNexResult = nexifMoveToUpper(m_pcChiefView->m_hWnd);		break;	// PINUP 指示要求
	case 608:	l_bNexResult = nexifMoveToLower(m_pcChiefView->m_hWnd);		break;	// PINDOWN 指示要求
	case 609:	l_bNexResult = nexifOpenShutter(m_pcChiefView->m_hWnd);		break;	// シャッタ OPEN 指示要求
	case 610:	l_bNexResult = nexifCloseShutter(m_pcChiefView->m_hWnd);	break;	// シャッタ CLOSE 指示要求
	default:
		l_iResult = -1;
		break;
	}
	if (-1 != l_iResult) {
		if (0 != l_bNexResult) {
			l_iResult = 0;
			// 動作中フラグをオン
			switch (dwCmdCode) {
			case 605:
			case 606:
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_WORKGUIDE, TRUE);
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_PIN, TRUE);
				break;
			case 607:
			case 608:
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_PIN, TRUE);
				break;
			case 609:
			case 610:
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_SHUTTER, TRUE);
				break;
			default:
				break;
			}
		} else {
			l_iResult = 1;
/* added 2010.05.21 hmenjo Nextra 指示前に正常応答する ---------- { ---------- */
			switch(dwCmdCode) {
			case 605:	PifComm_AlignmentLoadResultReport(l_iResult);	break;
			case 606:	PifComm_AlignmentUnloadResultReport(l_iResult);	break;
			case 607:	PifComm_PinUpResultReport(l_iResult);			break;
			case 608:	PifComm_PinDownResultReport(l_iResult);			break;
			case 609:	PifComm_ShutterOpenResultReport(l_iResult);		break;
			case 610:	PifComm_ShutterCloseResultReport(l_iResult);	break;
			}
/* added 2010.05.21 hmenjo Nextra 指示前に正常応答する ---------- } ---------- */
		}
/* deleted 2010.05.21 hmenjo Nextra 指示前に正常応答する ---------- { ---------- */
//		PifComm_DoStateDone(l_iResult);	// 応答送信
/* deleted 2010.05.21 hmenjo Nextra 指示前に正常応答する ---------- } ---------- */
	} else {
		// Chief の対 Nextra 用として未定義コマンドでコールされた場合はココに来ます．
		// 2008.09.18(Thu) 現在の仕様では報知の必要はありません．
		m_pcChiefView->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(2, CHRAMSG_OK)), MAKELPARAM(16, 0));
	}

}

//	時刻合わせ要求
void CChiefRcvMailThread::EventProcessP201()
{
	SYSTEMTIME l_systemTime;
	PifComm_GetSetTime(&l_systemTime);	// 指定時刻を取得

	// システム時刻を設定
	//		Administrator での実行を想定していますので，
	//		セキュリティ特権の変更はしていません．
	int l_iResult;
	if (0 == SetLocalTime(&l_systemTime)) {
		// 設定失敗
		l_iResult = 1;
//		m_pParent->PostMessage(WM_CHIF_SETTIME_FAIL, 0, 0);
			/*	Pif からの指示で異常時は Pif に異常ステータスで返すので
			 *	メッセージボックスなどの表示は不要としています．	*/
	} else {
		l_iResult = 0;
	}
	PifComm_DoStateDone(l_iResult);	// 応答送信
}

// 2009.02.05 K.Matsuo delete -->
////	トレースデータの取得要求
//void CChiefRcvMailThread::EventProcessP306()
//{
//	// トレースデータ送信要求
//	m_pcChiefView->PostMessage(WM_CHIF_SENDTRACEDATA, 0, 0);
//	PifComm_DoStateDone(0);	// 応答送信
//}
// 2009.02.05 K.Matsuo delete <--

//	着工レシピ指示要求
/* modified 2009.09.10 hmenjo P411 コマンド追加 ---------- { ---------- */
//void CChiefRcvMailThread::EventProcessP401()
/* modified 2009.09.10 hmenjo P411 コマンド追加 ----------				*/
void CChiefRcvMailThread::EventProcessP401(BOOL bGTRRefer/* = FALSE*/)
/* modified 2009.09.10 hmenjo P411 コマンド追加 ---------- } ---------- */
{
	// 最初にインタロック
	if (HOST_REMOTE != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		PifComm_DoStateDone(20);	// リモートでなかった
		return;
	}
	int l_iProcStatus = ((CChiefView*) m_pcChiefView)->ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		PifComm_DoStateDone(32);	// Wait，Down 以外だった
		return;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		PifComm_DoStateDone(51);	// 画面モードが「測定モード」か「データモード」でなかった
		return;
	}

	// Pif からレシピ名を取得
	TCHAR l_szRecipeName[RECIPE_NAME_LEN + 1];
	PifComm_GetRecipeName(l_szRecipeName);

	// 指定されたレシピが存在するか確認
	BOOL l_bRecipeExist = FALSE;
	MAIN_RCP_INFO l_MainRcpInfo;
	l_bRecipeExist = RecipeFile_LoadRecipe(&l_MainRcpInfo, l_szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	/*		ここはスレッド内なので時間のかかるファイルアクセスはしない方がよいですが，
	 *		ここで Pif への応答を返すべきなので，ここでレシピをファイルから検索しています．	*/
	if (0 != l_bRecipeExist) {
		// 指定されたレシピがあった
/* added 2009.09.10 hmenjo P411 コマンド追加 ---------- { ---------- */
		MEAS_PROG_INFO l_MeasProgInfo;
		if (0 == RecipeFile_LoadRecipe(&l_MeasProgInfo, l_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM)) {
			/* 測定プログラムの読込み異常です．	*/
			PifComm_DoStateDone(1);	// 異常応答送信
		} else
/* added 2009.09.10 hmenjo P411 コマンド追加 ---------- } ---------- */
		if (0 == ((CChiefView*) m_pcChiefView)->HeadTypeCheck(l_MainRcpInfo.MainRcpParam.hdr.wHeadType)) {
			// ヘッドタイプが無効です．
			PifComm_DoStateDone(61);	// 異常応答送信
/* deleted 2009.12.25 hmenjo P511 別ヘッド対応 修正 ---------- { ---------- */
///* added 2009.09.10 hmenjo P411 コマンド追加 ---------- { ---------- */
//		} else if ((TRUE == bGTRRefer)
//				&& ((HEAD_TYPE_SR != l_MeasProgInfo.ScanParams.hdr.wHeadType)
//				 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G != l_MeasProgInfo.ScanParams.hdr.wScanType))) {
//// 2009.12.24 K.Matsuo -->
////			/* ガントリ透過率リファレンス用レシピなのに，SR ヘッドでないか，スキャンタイプがガントリでない．	*/
////			PifComm_DoStateDone(61);	// 異常応答送信
//			/* リファレンスモードで，GTR 以外は，
//				何もせず正常完了を見せかけます．	*/
//			((CChiefView*) m_pcChiefView)->m_bGotRecipeFromPif = TRUE;
//			PifComm_DoStateDone(0);	/* 正常応答	*/
//// 2009.12.24 K.Matsuo <--
///* added 2009.09.10 hmenjo P411 コマンド追加 ---------- } ---------- */
/* deleted 2009.12.25 hmenjo P511 別ヘッド対応 修正 ---------- } ---------- */
		} else {
			// ヘッドタイプが有効
/* deleted 2009.09.10 hmenjo P401 セット結果チェック追加 ---------- { ---------- */
//			PifComm_DoStateDone(0);	// 正常応答送信
//			((CChiefView*) m_pcChiefView)->m_bGotRecipeFromPif = TRUE;
/* deleted 2009.09.10 hmenjo P401 セット結果チェック追加 ---------- } ---------- */
			// Chief ダイアログに通知
			COPYDATASTRUCT l_CopyData;
			l_CopyData.dwData = WMCD_CHIF_RECIPE_NOTIFY;
			l_CopyData.cbData = sizeof(l_szRecipeName);
			l_CopyData.lpData = l_szRecipeName;
/* modified 2009.09.10 hmenjo P401 セット結果チェック追加 ---------- { ---------- */
//			m_pcChiefView->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);	// ウィンドウハンドルは０にしています．
/* modified 2009.09.10 hmenjo P401 セット結果チェック追加 ----------			  */
			BOOL l_bRet = m_pcChiefView->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);	// ウィンドウハンドルは０にしています．
			if (1 == l_bRet) {
				((CChiefView*) m_pcChiefView)->m_bGotRecipeFromPif = TRUE;
				PifComm_DoStateDone(0);	// 正常応答送信
			} else {
				/* レシピを通知出来ませんでした．	*/
				PifComm_DoStateDone(1);	// 異常応答送信
			}
/* modified 2009.09.10 hmenjo P401 セット結果チェック追加 ---------- } ---------- */
		}
	} else {
		// 指定されたレシピが無かった
		PifComm_DoStateDone(1);	// 異常応答送信
	}
}

/* added 2009.09.10 hmenjo P411 コマンド追加 ---------- { ---------- */
/*	着工レシピ指示要求：透過率リファレンス用	*/
/*		現状はガントリ透過率にのみ使っています．
 *		通常透過率の仕様が未決定のためです．	*/
void CChiefRcvMailThread::EventProcessP411()
{
	EventProcessP401(TRUE);
}
/* added 2009.09.10 hmenjo P411 コマンド追加 ---------- } ---------- */

//	測定開始指示要求
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
//void CChiefRcvMailThread::EventProcessP501()
/* modified 2009.09.10 hmenjo P511 コマンド追加 ----------				*/
void CChiefRcvMailThread::EventProcessP501(BOOL bReferMode/* = FALSE*/)
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
{
	// 最初にインタロック
	DWORD l_dwEMO = ((CChiefView*) m_pcChiefView)->CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
//		PifComm_DoStateDone(60 + l_dwEMO);	// EMO，ドアインタロック，装置パワーオフ
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ----------			   */
		if (ALID_SafetyPlugOpen != l_dwEMO) {
			PifComm_DoStateDone(60 + l_dwEMO);	/* EMO，ドアインタロック，装置パワーオフ	*/
		} else {
			PifComm_DoStateDone(69);	/* セーフティプラグ	*/
		}
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsRobotArmON()) {
		PifComm_DoStateDone(64);	// ロボットアーム検出
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsPinDownOFF()) {
		PifComm_DoStateDone(65);	// ピンダウンがオフ
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsAirPressureLowON()) {
		PifComm_DoStateDone(66);	// エア圧力低下がオフ
		return;
	}
	if ((0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterClose) || (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterOpen)) {
		PifComm_DoStateDone(67);	// シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == ((CChiefView*) m_pcChiefView)->IsCtaILPI()) {
		PifComm_DoStateDone(72);	/* CTAILPI がオフ	*/
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = ((CChiefView*) m_pcChiefView)->CheckResistIL();
	if (0 != iResistStatus) {
		int iAckCode = 74 + iResistStatus - 1;		// 74,75,76
		PifComm_DoStateDone(iAckCode);	/* Resist上昇端センサー がオフ	*/
		return;
	}
// 2009.11.12 bagus MS 下端位置2チェック --{--
	if (0 == ((CChiefView*) m_pcChiefView)->IsMSILPI()) {
		PifComm_DoStateDone(77);	/* 下端位置２センサーON*/
		return;
	}
// 2009.11.12 bagus MS 下端位置2チェック --}--
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- } ---------- */
	if (HOST_REMOTE != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		PifComm_DoStateDone(20);	// リモートでなかった
		return;
	}
	int l_iProcStatus = ((CChiefView*) m_pcChiefView)->ProcStatusGet();
	if (PROCESS_WAIT != l_iProcStatus) {
		PifComm_DoStateDone(33);	// Wait 以外だった
		return;
	}
	if (0 != ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		PifComm_DoStateDone(41);	// 動作中フラグがオンだった
		return;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		PifComm_DoStateDone(51);	// 画面モードが「測定モード」か「データモード」でなかった
		return;
	}
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			PifComm_DoStateDone(43);	// さらに，東朋メンテ SW がオフの場合，禁止
			return;
		}
#if 1	// メンテでなく，東朋メンテの場合に動作禁止にします
	} else {
		// メンテナンス SW がオフ時は・・・
		if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			// 東朋メンテ SW がオンの場合，禁止
			PifComm_DoStateDone(43);
			return;
		}
#endif
	}

	if (0 == ((CChiefView*) m_pcChiefView)->m_bGotRecipeFromPif) {
		// レシピを受信していません．
		PifComm_DoStateDone(7);	// 異常応答送信
	} else {
		// Pif からサンプル ID を取得
		TCHAR l_szSampleId[SAMPLE_ID_LEN + 2];
		PifComm_GetSampleId(l_szSampleId);
		// サンプル ID の文字数をチェック(範囲：1～32)
		DWORD l_dwIdLen = _tcslen(l_szSampleId);
/* added 2009.09.10 hmenjo P511 サンプル ID スペース化 ---------- { ---------- */
		if ((TRUE == bReferMode) && (0 == l_dwIdLen)) {
			/* リファレンスモードの場合に，サンプル ID が null ならスペースで埋めます．	*/
			memset(l_szSampleId, 0, sizeof(l_szSampleId));
//			_tcsnset(l_szSampleId, _T(' '), SAMPLE_ID_LEN);
			for (int i = 0; i < SAMPLE_ID_LEN; i++) {
				l_szSampleId[i] = _T(' ');
			}
			l_dwIdLen = _tcslen(l_szSampleId);
		}
/* added 2009.09.10 hmenjo P511 サンプル ID スペース化 ---------- } ---------- */
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- { ---------- */
		/* Pif からロット ID を取得します．	*/
		TCHAR l_szLotId[LOT_ID_LEN + 2];
		PifComm_GetLotId(l_szLotId);
		/* ロット ID の文字数をチェック(範囲：1～32)	*/
		DWORD l_dwLotIdLen = _tcslen(l_szLotId);
		if ((TRUE == bReferMode) && (0 == l_dwLotIdLen)) {
			/* リファレンスモードの場合に，ロット ID が null ならスペースで埋めます．	*/
			memset(l_szLotId, 0, sizeof(l_szLotId));
			_tcsnset(l_szLotId, _T(' '), LOT_ID_LEN);
//			for (int i = 0; i < SAMPLE_ID_LEN; i++) {
//				l_szSampleId[i] = _T(' ');
//			}
			l_dwLotIdLen = _tcslen(l_szLotId);
		}
		if ((l_dwLotIdLen < 0) || (LOT_ID_LEN < l_dwLotIdLen)) {
			/* ロット ID の文字数が範囲外：1～32	*/
			PifComm_DoStateDone(6);	// 異常応答送信
		} else
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- } ---------- */
		if ((l_dwIdLen < 1) || (SAMPLE_ID_LEN < l_dwIdLen)) {
			// サンプル ID の文字数が範囲外：1～32
			PifComm_DoStateDone(6);	// 異常応答送信
		} else {
			// サンプル ID を保存
			_tcscpy(((CChiefView*) m_pcChiefView)->m_szSampleID, l_szSampleId);
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- { ---------- */
			/* ロット ID を保存	*/
			_tcscpy(((CChiefView*) m_pcChiefView)->m_szLotID, l_szLotId);
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- } ---------- */
			// メインレシピを読込み
			TCHAR l_szMainRcpName[256];
			((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
			DWORD l_Result = ((CChiefView*) m_pcChiefView)->RecipesGet(l_szMainRcpName, 0);
			if (0 != l_Result) {
				// レシピ読込み異常
				PifComm_DoStateDone(l_Result);	// 異常応答送信
			} else {
/* modified 2009.08.03 hmenjo ストレス機能追加(12) ---------- { ---------- */
//#ifdef CHIEF_REMEASURE_ON	// リメジャー対応 20081225
//				LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
//				// --> Pif が対応するまで仮で，測定する/しない フラグをセットします
//				{
//					for (int i = 0; i < l_pStageProgInfoHdr->wNumScans; i++) {
//						((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->m_bDoPointMeasFlag[i] = TRUE;
//					}
//				}
//				// <-- Pif が対応するまで仮で，測定する/しない フラグをセットします
//				DWORD l_dwSearchStartNo = 1;
//				if (l_pStageProgInfoHdr->wNumScans < ((CChiefView*) m_pcChiefView)->GetNextPointNo(&l_dwSearchStartNo)) {
//					// 測定する/しない フラグがすべて「しない」になってます
//					PifComm_DoStateDone(68);	// 異常応答送信
//				} else {
//					// シーケンストランジションを起動 ---------------------------------
//					if (false == ((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->IsIdle()) {
//						// マスタ トランジションがビジーでした
//						PifComm_DoStateDone(9);	// 異常応答送信
//					} else if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
//						// シーケンス測定 トランジションがビジーでした
//						PifComm_DoStateDone(10);	// 異常応答送信
//					} else {
//						// 測定開始
//						((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_PIFSEQ, 0);
//						PifComm_DoStateDone(0);	// 応答送信
//					}
//				}
//#else
//				// シーケンストランジションを起動 ---------------------------------
//				if (false == ((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->IsIdle()) {
//					// マスタ トランジションがビジーでした
//					PifComm_DoStateDone(9);	// 異常応答送信
//				} else if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
//					// シーケンス測定 トランジションがビジーでした
//					PifComm_DoStateDone(10);	// 異常応答送信
//				} else {
//					// 測定開始
//					((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_PIFSEQ);
//					PifComm_DoStateDone(0);	// 応答送信
//				}
//#endif
/* modified 2009.08.03 hmenjo ストレス機能追加(12) ----------			   */
/* added 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
				LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo;
/* added 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
				LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
				// --> Pif が対応するまで仮で，測定する/しない フラグをセットします
				{
					for (int i = 0; i < l_pStageProgInfoHdr->wNumScans; i++) {
						((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->m_bDoPointMeasFlag[i] = TRUE;
					}
				}
				// <-- Pif が対応するまで仮で，測定する/しない フラグをセットします
				DWORD l_dwSearchStartNo = 1;
				if (l_pStageProgInfoHdr->wNumScans < ((CChiefView*) m_pcChiefView)->GetNextPointNo(&l_dwSearchStartNo)) {
					// 測定する/しない フラグがすべて「しない」になってます
					PifComm_DoStateDone(68);	// 異常応答送信
// 2009.12.24 K.Matsuo Delete -->
///* added 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
//				} else if ((TRUE == bReferMode)
//						&& ((HEAD_TYPE_SR != l_pMeasProgInfo->ScanParams.hdr.wHeadType)
//						 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G != l_pMeasProgInfo->ScanParams.hdr.wScanType))) {
//					/* ガントリ透過率リファレンスなのに，SR ヘッドでないか，スキャンタイプがガントリでない．	*/
//					PifComm_DoStateDone(61);	// 異常応答送信
///* added 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
// 2009.12.24 K.Matsuo Delete <--
/* added 2009.09.11 hmenjo 測定前リファレンスデータ確認 ---------- { ---------- */
				} else if ((FALSE == bReferMode)
						&& (HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
						&& (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
						&& (1 == ((CChiefView*) m_pcChiefView)->CheckReferenceData(	l_szMainRcpName,
																					l_pMeasProgInfo->Ref.hdr.dLifeTime,
																					l_pMeasProgInfo->Ref2nd.bMeasure))) {
					/* ガントリ透過率測定で，リファレンスデータ異常
						(リファレンスデータファイルが存在しない)	*/
					PifComm_DoStateDone(70);	// 異常応答送信
/* added 2009.09.11 hmenjo 測定前リファレンスデータ確認 ---------- } ---------- */
/* added 2009.09.11 hmenjo 測定前ガントリ透過率ランプ確認 ---------- { ---------- */
				} else if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
						&& (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
						&& (TRUE != ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->IsGTRLampOn())) {
					/* ガントリ透過率測定(リファレンス含む)で，ガントリ透過率用ハロゲンランプがオフ	*/
					PifComm_DoStateDone(71);	// 異常応答送信
/* added 2009.09.11 hmenjo 測定前ガントリ透過率ランプ確認 ---------- } ---------- */
/* added 2009.10.30 hmenjo P511 別ヘッド対応 ---------- { ---------- */
				} else if ((TRUE == bReferMode)
						&& (0 == (	(HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
								&& (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)))) {
					/* リファレンスモードで，GTR 以外は，
						何もせず正常完了を見せかけます．	*/
					PifComm_DoStateDone(0);	/* 正常応答	*/
					PifComm_CompleteReferenceAllPointsReport(0);	/* 完了送信	*/
/* added 2009.10.30 hmenjo P511 別ヘッド対応 ---------- } ---------- */
				} else {
					CHIEF_PFUNCS l_ChiefPFuncs;
					if (0 == PFC_FuncSet((CChiefView*) m_pcChiefView, &l_ChiefPFuncs, 0)) {
						/* ヘッドタイプ異常	*/
						PifComm_DoStateDone(1);	/* 異常応答送信	*/
						return;
					}
					/* シーケンストランジションを起動 --------------------------------- */
					if (false == ((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->IsIdle()) {
						/* マスタ トランジションがビジーでした	*/
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
						((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefRcvMailThread::EventProcessP501() - Master Seq is busy."));
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
						PifComm_DoStateDone(9);	/* 異常応答送信	*/
					} else if (false == (*l_ChiefPFuncs.IsIdle)((CChiefView*) m_pcChiefView)) {
						/* 測定 トランジションがビジーでした	*/
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
						((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefRcvMailThread::EventProcessP501() - SR or Stress Seq is busy."));
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
						PifComm_DoStateDone(10);	/* 異常応答送信	*/
					} else {
						/* 測定開始	*/
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
//						((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_PIFSEQ, 0);
/* modified 2009.09.10 hmenjo P511 コマンド追加 ----------				*/
						((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_PIFSEQ, (cEventParams*) MAKELONG(0, bReferMode));
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
						PifComm_DoStateDone(0);	/* 応答送信	*/
					}
				}
/* modified 2009.08.03 hmenjo ストレス機能追加(12) ---------- } ---------- */
			}
		}
	}
}

/* added 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
/*	測定開始指示要求：透過率用	*/
/*		現状はガントリ透過率にのみ使っています．
 *		通常透過率の仕様が未決定のためです．	*/
void CChiefRcvMailThread::EventProcessP511()
{
	EventProcessP501(TRUE);	/* リファレンスモードで起動	*/
}
/* added 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */

// 2014.01.07 bagus Add(Stage None対応) -->
// Si リファレンス取得開始指示要求
void CChiefRcvMailThread::EventProcessP514()
{
	// 最初にインタロック
	DWORD l_dwEMO = ((CChiefView*) m_pcChiefView)->CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
//		PifComm_DoStateDone(60 + l_dwEMO);	// EMO，ドアインタロック，装置パワーオフ
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ----------			   */
		if (ALID_SafetyPlugOpen != l_dwEMO) {
			PifComm_DoStateDone(60 + l_dwEMO);	/* EMO，ドアインタロック，装置パワーオフ	*/
		} else {
			PifComm_DoStateDone(69);	/* セーフティプラグ	*/
		}
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsRobotArmON()) {
		PifComm_DoStateDone(64);	// ロボットアーム検出
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsPinDownOFF()) {
		PifComm_DoStateDone(65);	// ピンダウンがオフ
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsAirPressureLowON()) {
		PifComm_DoStateDone(66);	// エア圧力低下がオフ
		return;
	}
	if ((0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterClose) || (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterOpen)) {
		PifComm_DoStateDone(67);	// シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == ((CChiefView*) m_pcChiefView)->IsCtaILPI()) {
		PifComm_DoStateDone(72);	/* CTAILPI がオフ	*/
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = ((CChiefView*) m_pcChiefView)->CheckResistIL();
	if (0 != iResistStatus) {
		int iAckCode = 74 + iResistStatus - 1;		// 74,75,76
		PifComm_DoStateDone(iAckCode);	/* Resist上昇端センサー がオフ	*/
		return;
	}
// 2009.11.12 bagus MS 下端位置2チェック --{--
	if (0 == ((CChiefView*) m_pcChiefView)->IsMSILPI()) {
		PifComm_DoStateDone(77);	/* 下端位置２センサーON*/
		return;
	}
// 2009.11.12 bagus MS 下端位置2チェック --}--
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- } ---------- */
	if (HOST_REMOTE != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		PifComm_DoStateDone(20);	// リモートでなかった
		return;
	}
	int l_iProcStatus = ((CChiefView*) m_pcChiefView)->ProcStatusGet();
	if (PROCESS_WAIT != l_iProcStatus) {
		PifComm_DoStateDone(33);	// Wait 以外だった
		return;
	}
	if (0 != ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		PifComm_DoStateDone(41);	// 動作中フラグがオンだった
		return;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		PifComm_DoStateDone(51);	// 画面モードが「測定モード」か「データモード」でなかった
		return;
	}
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			PifComm_DoStateDone(43);	// さらに，東朋メンテ SW がオフの場合，禁止
			return;
		}
#if 1	// メンテでなく，東朋メンテの場合に動作禁止にします
	} else {
		// メンテナンス SW がオフ時は・・・
		if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			// 東朋メンテ SW がオンの場合，禁止
			PifComm_DoStateDone(43);
			return;
		}
#endif
	}

	if (0 == ((CChiefView*) m_pcChiefView)->m_bGotRecipeFromPif) {
		// レシピを受信していません．
		PifComm_DoStateDone(7);	// 異常応答送信
	} else {
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_STDREF, 0);
		PifComm_DoStateDone(0);	/* 応答送信	*/
	}
}

// Dark リファレンス取得開始指示要求
void CChiefRcvMailThread::EventProcessP516()
{
	// 最初にインタロック
	DWORD l_dwEMO = ((CChiefView*) m_pcChiefView)->CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
//		PifComm_DoStateDone(60 + l_dwEMO);	// EMO，ドアインタロック，装置パワーオフ
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ----------			   */
		if (ALID_SafetyPlugOpen != l_dwEMO) {
			PifComm_DoStateDone(60 + l_dwEMO);	/* EMO，ドアインタロック，装置パワーオフ	*/
		} else {
			PifComm_DoStateDone(69);	/* セーフティプラグ	*/
		}
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsRobotArmON()) {
		PifComm_DoStateDone(64);	// ロボットアーム検出
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsPinDownOFF()) {
		PifComm_DoStateDone(65);	// ピンダウンがオフ
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsAirPressureLowON()) {
		PifComm_DoStateDone(66);	// エア圧力低下がオフ
		return;
	}
	if ((0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterClose) || (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterOpen)) {
		PifComm_DoStateDone(67);	// シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == ((CChiefView*) m_pcChiefView)->IsCtaILPI()) {
		PifComm_DoStateDone(72);	/* CTAILPI がオフ	*/
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = ((CChiefView*) m_pcChiefView)->CheckResistIL();
	if (0 != iResistStatus) {
		int iAckCode = 74 + iResistStatus - 1;		// 74,75,76
		PifComm_DoStateDone(iAckCode);	/* Resist上昇端センサー がオフ	*/
		return;
	}
// 2009.11.12 bagus MS 下端位置2チェック --{--
	if (0 == ((CChiefView*) m_pcChiefView)->IsMSILPI()) {
		PifComm_DoStateDone(77);	/* 下端位置２センサーON*/
		return;
	}
// 2009.11.12 bagus MS 下端位置2チェック --}--
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- } ---------- */
	if (HOST_REMOTE != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		PifComm_DoStateDone(20);	// リモートでなかった
		return;
	}
	int l_iProcStatus = ((CChiefView*) m_pcChiefView)->ProcStatusGet();
	if (PROCESS_WAIT != l_iProcStatus) {
		PifComm_DoStateDone(33);	// Wait 以外だった
		return;
	}
	if (0 != ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		PifComm_DoStateDone(41);	// 動作中フラグがオンだった
		return;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		PifComm_DoStateDone(51);	// 画面モードが「測定モード」か「データモード」でなかった
		return;
	}
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			PifComm_DoStateDone(43);	// さらに，東朋メンテ SW がオフの場合，禁止
			return;
		}
#if 1	// メンテでなく，東朋メンテの場合に動作禁止にします
	} else {
		// メンテナンス SW がオフ時は・・・
		if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			// 東朋メンテ SW がオンの場合，禁止
			PifComm_DoStateDone(43);
			return;
		}
#endif
	}

	if (0 == ((CChiefView*) m_pcChiefView)->m_bGotRecipeFromPif) {
		// レシピを受信していません．
		PifComm_DoStateDone(7);	// 異常応答送信
	} else {
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_DRKREF, 0);
		PifComm_DoStateDone(0);	/* 応答送信	*/
	}
}
// 2014.01.07 bagus Add(Stage None対応) <--

//	ロードポジション移行指示要求
void CChiefRcvMailThread::EventProcessP601()
{
	// 最初にインタロック
	DWORD l_dwEMO = ((CChiefView*) m_pcChiefView)->CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
//		PifComm_DoStateDone(60 + l_dwEMO);	// EMO，ドアインタロック，装置パワーオフ
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ----------			   */
		if (ALID_SafetyPlugOpen != l_dwEMO) {
			PifComm_DoStateDone(60 + l_dwEMO);	/* EMO，ドアインタロック，装置パワーオフ	*/
		} else {
			PifComm_DoStateDone(69);	/* セーフティプラグ	*/
		}
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsRobotArmON()) {
		PifComm_DoStateDone(64);	// ロボットアーム検出
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsPinDownOFF()) {
		PifComm_DoStateDone(65);	// ピンダウンがオフ
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsAirPressureLowON()) {
		PifComm_DoStateDone(66);	// エア圧力低下がオフ
		return;
	}
	if (
		(0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW)
	 && ((0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterClose) || (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterOpen))
	) {
		PifComm_DoStateDone(67);	// 東朋メンテがオフで，かつ，シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == ((CChiefView*) m_pcChiefView)->IsCtaILPI()) {
		PifComm_DoStateDone(72);	/* CTAILPI がオフ	*/
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = ((CChiefView*) m_pcChiefView)->CheckResistIL();
	if (0 != iResistStatus) {
		int iAckCode = 74 + iResistStatus - 1;		// 74,75,76
		PifComm_DoStateDone(iAckCode);	/* Resist上昇端センサー がオフ	*/
		return;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- } ---------- */
// 2009.11.12 bagus MS 下端位置2チェック --{--
	if (0 == ((CChiefView*) m_pcChiefView)->IsMSILPI()) {
		PifComm_DoStateDone(77);	/* 下端位置２センサーON*/
		return;
	}
// 2009.11.12 bagus MS 下端位置2チェック --}--
	int l_iProcStatus = ((CChiefView*) m_pcChiefView)->ProcStatusGet();
// hmenjo 2009.01.22 Complete　でも許可する
//	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
	if (
		(PROCESS_WAIT != l_iProcStatus)
	 && (PROCESS_DOWN != l_iProcStatus)
	 && (PROCESS_COMP != l_iProcStatus)
	 && (PROCESS_ABRT != l_iProcStatus)
	) {
		PifComm_DoStateDone(32);	// Wait，Down 以外だった
		return;
	}
#if 0	// ターレット動作中をフラグに追加したため，ターレットは除外
	if (0 != ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
#else	// ターレット動作中をフラグに追加したため，ターレットは除外
	DWORD l_dwActFlags = ACTUATE_XYSTAGE | ACTUATE_ZAXIS | ACTUATE_PIN | ACTUATE_SHUTTER | ACTUATE_WORKGUIDE;
	if (0 != (l_dwActFlags & ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll())) {
#endif	// ターレット動作中をフラグに追加したため，ターレットは除外
		PifComm_DoStateDone(41);	// 動作中フラグがオンだった
		return;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		PifComm_DoStateDone(51);	// 画面モードが「測定モード」か「データモード」でなかった
		return;
	}
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			PifComm_DoStateDone(43);	// さらに，東朋メンテ SW がオフの場合，禁止
			return;
		}
#if 1	// メンテでなく，東朋メンテの場合に動作禁止にします
	} else {
		// メンテナンス SW がオフ時は・・・
		if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			// 東朋メンテ SW がオンの場合，禁止
			PifComm_DoStateDone(43);
			return;
		}
#endif
	}

	double l_dSubstrateSizeX, l_dSubstrateSizeY;	// 基板サイズ
	// Pif から基板サイズを取得
	PifComm_GetSampleSize(&l_dSubstrateSizeX, &l_dSubstrateSizeY);
	// Nextra に設定
	BOOL l_bNexResult = nexifSetSampleSize(l_dSubstrateSizeX, l_dSubstrateSizeY);
	if (0 == l_bNexResult) {
		PifComm_DoStateDone(1);	// 異常応答送信(セット出来ませんでした(Nextra から異常(FALSE)応答))
	} else {
		// ロードポジション移動処理
		PifComm_DoStateDone(0);	// 正常応答送信
		m_pcChiefView->PostMessage(WM_CHIF_MOVE_SETPOS, 0, 0);
	}
}

//	汎用ポジション移行指示要求
void CChiefRcvMailThread::EventProcessP602()
{
	// 最初にインタロック
	DWORD l_dwEMO = ((CChiefView*) m_pcChiefView)->CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
//		PifComm_DoStateDone(60 + l_dwEMO);	// EMO，ドアインタロック，装置パワーオフ
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ----------			   */
		if (ALID_SafetyPlugOpen != l_dwEMO) {
			PifComm_DoStateDone(60 + l_dwEMO);	/* EMO，ドアインタロック，装置パワーオフ	*/
		} else {
			PifComm_DoStateDone(69);	/* セーフティプラグ	*/
		}
/* modified 2009.08.18 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsRobotArmON()) {
		PifComm_DoStateDone(64);	// ロボットアーム検出
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsPinDownOFF()) {
		PifComm_DoStateDone(65);	// ピンダウンがオフ
		return;
	}
	if (0 != ((CChiefView*) m_pcChiefView)->CheckDIO_IsAirPressureLowON()) {
		PifComm_DoStateDone(66);	// エア圧力低下がオフ
		return;
	}
	if (
		(0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW)
	 && ((0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterClose) || (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bShutterOpen))
	) {
		PifComm_DoStateDone(67);	// 東朋メンテがオフで，かつ，シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == ((CChiefView*) m_pcChiefView)->IsCtaILPI()) {
		PifComm_DoStateDone(72);	/* CTAILPI がオフ	*/
		return;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = ((CChiefView*) m_pcChiefView)->CheckResistIL();
	if (0 != iResistStatus) {
		int iAckCode = 74 + iResistStatus - 1;		// 74,75,76
		PifComm_DoStateDone(iAckCode);	/* Resist上昇端センサー がオフ	*/
		return;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- } ---------- */
// 2009.11.12 bagus MS 下端位置2チェック --{--
	if (0 == ((CChiefView*) m_pcChiefView)->IsMSILPI()) {
		PifComm_DoStateDone(77);	/* 下端位置２センサーON*/
		return;
	}
// 2009.11.12 bagus MS 下端位置2チェック --}--
	int l_iProcStatus = ((CChiefView*) m_pcChiefView)->ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		PifComm_DoStateDone(32);	// Wait，Down 以外だった
		return;
	}
#if 0	// ターレット動作中をフラグに追加したため，ターレットは除外
	if (0 != ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
#else	// ターレット動作中をフラグに追加したため，ターレットは除外
	DWORD l_dwActFlags = ACTUATE_XYSTAGE | ACTUATE_ZAXIS | ACTUATE_PIN | ACTUATE_SHUTTER | ACTUATE_WORKGUIDE;
	if (0 != (l_dwActFlags & ((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->ActuateFlagsGetAll())) {
#endif	// ターレット動作中をフラグに追加したため，ターレットは除外
		PifComm_DoStateDone(41);	// 動作中フラグがオンだった
		return;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		PifComm_DoStateDone(51);	// 画面モードが「測定モード」か「データモード」でなかった
		return;
	}
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			PifComm_DoStateDone(43);	// さらに，東朋メンテ SW がオフの場合，禁止
			return;
		}
#if 1	// メンテでなく，東朋メンテの場合に動作禁止にします
	} else {
		// メンテナンス SW がオフ時は・・・
		if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bTHMaintenanceSW) {
			// 東朋メンテ SW がオンの場合，禁止
			PifComm_DoStateDone(43);
			return;
		}
#endif
	}

	// 汎用ポジション位置番号を取得
	int l_iMovePosId;
	l_iMovePosId = PifComm_GetMovePosId();
	if ((l_iMovePosId < 1) || (GENERAL_POSITION_MAX < l_iMovePosId)) {
		// 範囲(1～GENERAL_POSITION_MAX)外エラー
		PifComm_DoStateDone(1);	// 異常応答送信
	} else {
		PifComm_DoStateDone(0);	// 正常応答送信
		m_pcChiefView->PostMessage(WM_CHIF_MOVE_SETPOS, (WPARAM) l_iMovePosId, 0);
	}
}

//	測定キャンセル指示要求
void CChiefRcvMailThread::EventProcessP611()
{
	if (HOST_REMOTE != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		PifComm_DoStateDone(20);	// リモートでない
	} else if (PROCESS_PROC != ((CChiefView*) m_pcChiefView)->ProcStatusGet()) {
		PifComm_DoStateDone(31);	// Processing でない
	} else {
/* modified 2009.08.03 hmenjo ストレス機能追加(12) ---------- { ---------- */
//		if (false == ((CChiefTransiSeq*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq))->IsIdle()) {
//			// キャンセルイベントを発行
//			((CChiefTransiSeq*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq))->TransiEvent(EV_SEQ_CANCEL);
//			((CChiefView*) m_pcChiefView)->m_bReqCancelComplete = TRUE;	// 測定キャンセル完了報告指令有り
//			PifComm_DoStateDone(0);	// 正常応答送信
/* modified 2009.08.03 hmenjo ストレス機能追加(12) ----------			   */
		CHIEF_PFUNCS l_ChiefPFuncs;
		if (0 == PFC_FuncSet((CChiefView*) m_pcChiefView, &l_ChiefPFuncs, 0)) {
			/* ヘッドタイプ異常	*/
			PifComm_DoStateDone(1);	/* 異常応答送信	*/
			return;
		}
		if (false == (*l_ChiefPFuncs.IsIdle)((CChiefView*) m_pcChiefView)) {
			/* キャンセルイベントを発行	*/
			(*l_ChiefPFuncs.TransiEvent)((CChiefView*) m_pcChiefView, l_ChiefPFuncs.Event.iCANCEL, 0);
			((CChiefView*) m_pcChiefView)->m_bReqCancelComplete = TRUE;	/* 測定キャンセル完了報告指令有り	*/
			PifComm_DoStateDone(0);	/* 正常応答送信	*/
/* modified 2009.08.03 hmenjo ストレス機能追加(12) ---------- } ---------- */
		} else {
			// シーケンス測定トランジションが実行中でなかった
			PifComm_DoStateDone(1);	// 異常応答送信
		}
	}
}
