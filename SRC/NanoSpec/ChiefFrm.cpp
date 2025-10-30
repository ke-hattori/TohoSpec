// ChiefFrame.cpp : CChiefFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "resource.h"
#include "system.h"
#include "ChiefView.h"
#include "ChiefFrm.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "ChifRcvMailThread.h"
#include "ChiefView.h"
#include <NEXIF.HXX>
#include <PIFCOMM.HXX>
//#include <NEXIO.HXX>		hmenjo 使用禁止
#include <NEXIOBASE.HXX>
#include "ChifTransiAF.h"
#include "ChifTransiDeskew.h"
#include "ChifTransiMaster.h"
#include "ChifTransiSeq.h"
#include "ChifTransiSr1Point.h"
#include "ChifTransiSrRefer.h"
#include "ChiefExports.h"
#include "ChiefThread.h"
#include "LensComboBox.h"
#include "..\\..\\INC\\MeaSys.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CChiefView*	g_pcChiefView;	// static 用クラスポインタ

/////////////////////////////////////////////////////////////////////////////
// CChiefFrame

IMPLEMENT_DYNCREATE(CChiefFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CChiefFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CChiefFrame)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHIF_HEARTBEAT, OnHeartBeat)
	ON_MESSAGE(WM_NEX_RESPONSE, OnNextraResponse)
	ON_MESSAGE(WM_CHIF_RECV_PIFCMD, OnDispRecvPifCmd)
	ON_MESSAGE(WM_CHIF_UPDATESTATESDLG, OnUpdateStatesOnDlg)
// 2009.02.05 K.Matsuo delete -->
//	ON_MESSAGE(WM_CHIF_SENDTRACEDATA, OnSendTraceData)
// 2009.02.05 K.Matsuo delete <--
	ON_MESSAGE(WM_CHIF_PIFDIREFRESH, OnPifDiRefresh)
	ON_MESSAGE(WM_CHIF_EQRUNCHECK, OnEQRunCheck)
	ON_MESSAGE(WM_CHIF_REPORTALARM, OnReportAlarms)
	ON_MESSAGE(WM_CHIF_MOVE_SETPOS, OnMoveSetPosFromPif)
	ON_MESSAGE(WM_DISP_MOVE_SETPOS, OnMoveSetPosFromDisp)
	ON_MESSAGE(WM_CHIF_STAGE_STOP, OnStageStop)
	ON_MESSAGE(WM_DISP_VACUUM_ONOFF, OnVacuumFromDisp)
	ON_MESSAGE(WM_CHIF_SET_RECIPE, OnSetRecipe)
	ON_MESSAGE(WM_DISP_AF_POPUP_END, OnAFPopupEnd)
// 2009.07.29 K.Matsuo -->
	ON_MESSAGE(WM_MANUALSITE_POPUP_END, OnManualSitePtPopupEnd)
// 2009.07.29 K.Matsuo <--
	ON_MESSAGE(WM_MEAS_RECVDATA, OnMeasRecvData)
	ON_MESSAGE(WM_DATA_1POINT_END, OnData1PointEnd)
	ON_MESSAGE(WM_DATA_MEAS_END, OnDataMeasEnd)
	ON_MESSAGE(WM_DISP_START_AAF, OnStartAAF)
	ON_MESSAGE(WM_DISP_START_SEQ, OnStartDispSeq)
	ON_MESSAGE(WM_DISP_START_1POINT, OnStart1Point)
	ON_MESSAGE(WM_DISP_PAUSE_SEQ, OnSeqPause)
	ON_MESSAGE(WM_DISP_RESUME_SEQ, OnSeqResume)
	ON_MESSAGE(WM_DISP_CANCEL_SEQ, OnSeqCancel)
	ON_MESSAGE(WM_DISP_START_SRREF, OnStartSrRefer)
	ON_MESSAGE(WM_CHIF_SHOWSW, OnDlgShowSW)
	ON_MESSAGE(WM_CHIF_RESET_ALARM, OnResetAlarm)
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- { ---------- */
	ON_MESSAGE(WM_MEAS_LINE_END, OnMeasLineEnd)
	ON_MESSAGE(WM_DATA_LINE_END, OnDataLineEnd)
	ON_MESSAGE(WM_DATA_STRS_MEAS_END, OnDataStressMeasEnd)
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- } ---------- */
//2009.10.28 bagus 2point-distance --{--
	ON_MESSAGE(WM_DISP_DISTANCE_POPUP_END, OnDistancePopupEnd)
//2009.10.28 bagus 2point-distance --}--
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- { ---------- */
	ON_MESSAGE(WM_MEAS_CTA_ALARM, OnMeasAlarmCTA)
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- } ---------- */
//2009.11.03 bagus 2point-distance --{--
	ON_MESSAGE(WM_DISP_MS_POPUP_END, OnMSPopupEnd)
//2009.11.03 bagus 2point-distance --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	ON_MESSAGE(WM_MEAS_COMPEASE_STATUS, OnMeasCompEASEStatus)
	ON_MESSAGE(WM_MEAS_COMPEASE_ERROR, OnMeasCompEASEError)
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	ON_MESSAGE(WM_DISP_CONFIRM_POPUP_END, OnConfirmPopupEnd)
// 2013.11.07 Bagus Add (TohoSpec対応) <--
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,			// ステータス ライン インジケータ
	ID_INDICATOR_KANA,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CChiefFrame クラスの構築/消滅

CChiefFrame::CChiefFrame()
{
	// TODO: この位置にメンバの初期化処理コードを追加してください。

}

CChiefFrame::~CChiefFrame()
{
}

BOOL CChiefFrame::PreTranslateMessage(MSG* pMsg)
{
	switch ( pMsg->message ) {
	case WM_SYSKEYDOWN:
		if ( pMsg->wParam == VK_F4 )
			return TRUE;
		break;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

BOOL CChiefFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して、Window クラスやスタイルを
	//		 修正してください。
	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_SYSMENU;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CChiefFrame クラスの診断

#ifdef _DEBUG
void CChiefFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CChiefFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChiefFrame メッセージ ハンドラ

LRESULT CChiefFrame::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_HEARTBEAT, wparam, lparam);
}

LRESULT CChiefFrame::OnNextraResponse(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_NEX_RESPONSE, wparam, lparam);
}

LRESULT CChiefFrame::OnDispRecvPifCmd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_RECV_PIFCMD, wparam, lparam);
}

LRESULT CChiefFrame::OnUpdateStatesOnDlg(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_UPDATESTATESDLG, wparam, lparam);
}

LRESULT CChiefFrame::OnPifDiRefresh(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_PIFDIREFRESH, wparam, lparam);
}

LRESULT CChiefFrame::OnEQRunCheck(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_EQRUNCHECK, wparam, lparam);
}

LRESULT CChiefFrame::OnReportAlarms(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_REPORTALARM, wparam, lparam);
}

LRESULT CChiefFrame::OnMoveSetPosFromPif(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_MOVE_SETPOS, wparam, lparam);
}

LRESULT CChiefFrame::OnMoveSetPosFromDisp(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_MOVE_SETPOS, wparam, lparam);
}

LRESULT CChiefFrame::OnStageStop(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_STAGE_STOP, wparam, lparam);
}

LRESULT CChiefFrame::OnVacuumFromDisp(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_VACUUM_ONOFF, wparam, lparam);
}

LRESULT CChiefFrame::OnSetRecipe(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_SET_RECIPE, wparam, lparam);
}

LRESULT CChiefFrame::OnAFPopupEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_AF_POPUP_END, wparam, lparam);
}

// 2009.07.29 K.Matsuo -->
LRESULT CChiefFrame::OnManualSitePtPopupEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_MANUALSITE_POPUP_END, wparam, lparam);
}
// 2009.07.29 K.Matsuo <--

LRESULT CChiefFrame::OnMeasRecvData(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_MEAS_RECVDATA, wparam, lparam);
}

LRESULT CChiefFrame::OnData1PointEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DATA_1POINT_END, wparam, lparam);
}

LRESULT CChiefFrame::OnDataMeasEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DATA_MEAS_END, wparam, lparam);
}

LRESULT CChiefFrame::OnStartAAF(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_START_AAF, wparam, lparam);
}

LRESULT CChiefFrame::OnStartDispSeq(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_START_SEQ, wparam, lparam);
}

LRESULT CChiefFrame::OnStart1Point(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_START_1POINT, wparam, lparam);
}

LRESULT CChiefFrame::OnSeqPause(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_PAUSE_SEQ, wparam, lparam);
}

LRESULT CChiefFrame::OnSeqResume(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_RESUME_SEQ, wparam, lparam);
}

LRESULT CChiefFrame::OnSeqCancel(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_CANCEL_SEQ, wparam, lparam);
}

LRESULT CChiefFrame::OnStartSrRefer(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_START_SRREF, wparam, lparam);
}

LRESULT CChiefFrame::OnDlgShowSW(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_SHOWSW, wparam, lparam);
}

LRESULT CChiefFrame::OnResetAlarm(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_CHIF_RESET_ALARM, wparam, lparam);
}

void CChiefFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	g_pcChiefView->PrepareToDestroy();
}
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- { ---------- */
LRESULT CChiefFrame::OnMeasLineEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_MEAS_LINE_END, wparam, lparam);
}
LRESULT CChiefFrame::OnDataLineEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DATA_LINE_END, wparam, lparam);
}
LRESULT CChiefFrame::OnDataStressMeasEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DATA_STRS_MEAS_END, wparam, lparam);
}
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- } ---------- */
//2009.10.28 bagus 2point-distance --{--
LRESULT CChiefFrame::OnDistancePopupEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_DISTANCE_POPUP_END,wparam,lparam);
}
//2009.10.28 bagus 2point-distance --}--

/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- { ---------- */
LRESULT CChiefFrame::OnMeasAlarmCTA(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_MEAS_CTA_ALARM, wparam, lparam);
}
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- } ---------- */
//2009.11.03 bagus MS --{--
LRESULT CChiefFrame::OnMSPopupEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_MS_POPUP_END,wparam,lparam);
}
//2009.11.03 bagus MS --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
LRESULT CChiefFrame::OnMeasCompEASEStatus(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_MEAS_COMPEASE_STATUS, wparam, lparam);
}

LRESULT CChiefFrame::OnMeasCompEASEError(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_MEAS_COMPEASE_ERROR, wparam, lparam);
}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
LRESULT CChiefFrame::OnConfirmPopupEnd(WPARAM wparam, LPARAM lparam)
{
	return g_pcChiefView->SendMessage(WM_DISP_CONFIRM_POPUP_END,wparam,lparam);
}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

/* added 2014.11.21 hmenjo 3100 アイコン(MAIN) ---------- { ---------- */
BOOL CChiefFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if (g_lModelType == MODEL_T3100) {
#if 0
		/* リソースへのアイコンの登録が必要です．	*/
		HICON l_hIcon = 0;
		l_hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_3100));
		if (0 != l_hIcon) {
			this->SetIcon(l_hIcon, TRUE);			/* 大きいアイコンを設定	*/
			/* 小さいアイコンは，システムが自動で探してくれます．	*/
		}
#else
		/* アイコンをファイルから読込みます．	*/
		CString l_strPathIcon = g_szBin_Dir;
		l_strPathIcon += _T("3100.ico");
		HICON l_hIcon32 = 0;
		HICON l_hIcon16 = 0;
		l_hIcon32 = (HICON) ::LoadImage(
								NULL,
								l_strPathIcon,
								IMAGE_ICON,
								32, 32,
								LR_LOADFROMFILE
							);
		l_hIcon16 = (HICON) ::LoadImage(
								NULL,
								l_strPathIcon,
								IMAGE_ICON,
								16, 16,
								LR_LOADFROMFILE
							);
		if ((0 != l_hIcon32) && (0 != l_hIcon16)) {
			this->SetIcon(l_hIcon32, TRUE);			/* 大きいアイコンを設定	*/
			this->SetIcon(l_hIcon16, FALSE);		/* 小さいアイコンを設定	*/
		}
#endif
	}

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
/* added 2014.11.21 hmenjo 3100 アイコン(MAIN) ---------- } ---------- */
