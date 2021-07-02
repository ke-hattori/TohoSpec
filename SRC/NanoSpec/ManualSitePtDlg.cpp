// ManualSitePtDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <math.h>
#include <patrec.hxx>
#include <active.hxx>
#include "System.h"
#include "NanoSpec.h"
#include "MainFrm.h"
#include "BitMapStatic.h"
#include "Misc.h"
#include "OverlayDialog.h"
#include "ManualStageDlg.h"
#include "ManualSitePtDlg.h"
#include "NanoSpecDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//==========================================================================
//==========================	CManualSitePtDlg	  ======================
//==========================================================================
//

//
//	STATES
//
enum MANUAL_SITEPT_STATES
{
	// Initialization. It has no actions. It's constructor
	// will call "Event(EV_NEXT);" only.
	//
	ST_INIT,				// initial fall through state no processing

	ST_MOVE_TO_SITEPT,		// move to the current SitePattern if it's defined

	ST_FOCUS,				// do focusing

	ST_START,				// get ready

	ST_STORE_SITE,			// store the first location

	ST_STAGE_MOVED
};

//
// EVENTS
//
enum MANUAL_SITEPT_EVENTS
{
	EV_START,
	EV_NEXT,			// go to the next state in the state transitation table
	EV_LBUTTON_DOWN,	// user start the teaching
	EV_FOCUS_DONE,
	EV_STAGE_MOVED
};

//
// STATE TRANSITION TABLE
//
const Transition ManualSitePtTeachTransTable[] =
{
	// OldState,				EventId,		NewState
	//
	{ ST_INIT,					EV_START,		ST_MOVE_TO_SITEPT		},

	{ ST_MOVE_TO_SITEPT,		EV_NEXT,		ST_FOCUS				},

	{ ST_FOCUS, 				EV_FOCUS_DONE,	ST_START				},

	{ ST_START, 				EV_LBUTTON_DOWN,ST_STORE_SITE			},
	{ ST_START, 				EV_STAGE_MOVED, ST_STAGE_MOVED			},

	{ ST_STORE_SITE,			EV_NEXT,		ST_START				},

	{ ST_STAGE_MOVED,			EV_NEXT,		ST_START				},

	END_OF_TRANSITION_TABLE
};

// =========================================================================
//
CManualSitePtDlg::CManualSitePtDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(COverlayDialog::IDD, pParent), cActive(ManualSitePtTeachTransTable)
{
	//{{AFX_DATA_INIT(CManualSitePtDlg)
	//}}AFX_DATA_INIT
}


CManualSitePtDlg::CManualSitePtDlg(SITEPT_SITE_MANUAL* pSitePtSiteManual, CWnd* pParent /*=NULL*/)
	: m_pSitePtSiteManual(pSitePtSiteManual),
	COverlayDialog(COverlayDialog::IDD, pParent), cActive(ManualSitePtTeachTransTable)
{
	//{{AFX_DATA_INIT(CManualSitePtDlg)
	//}}AFX_DATA_INIT
//	m_logFile.Create("C:\\NANOSPEC\\LOG", "SITEPT");
}

// =========================================================================
//-----------------STATE PROCESSING------------------------------------------
// =========================================================================


// =========================================================================
//
// ST_INIT
//
// Actions:
//
void CManualSitePtDlg::enter_ST_INIT(const cEventParams* const /* params */ )
{
	// do nothing
}

// =========================================================================
//
// ST_MOVE_TO_SITEPT
//
// Actions:
//
void CManualSitePtDlg::enter_ST_MOVE_TO_SITEPT(const cEventParams* const /* params */ )
{
	// use the current stage position as the taught position
	STAGE_COORD_XYZ CurrPos;
	StageGetPos(&CurrPos);

	m_TeachPos.lX = CurrPos.lX;
	m_TeachPos.lY = CurrPos.lY;

	StageToClient(&m_TeachPos, &m_ptTeach); 	// convert to client coord

	// enable the OK button
	SetValidBit(TRUE);

	Event(EV_NEXT);
}

// =========================================================================
//
// ST_FOCUS
//
// Actions: Do focus.
//
void CManualSitePtDlg::enter_ST_FOCUS(const cEventParams* const /* params */ )
{
	CCursor::BeginWaitCursor();

	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	//Saiki 20090728 Add ----->
	//StageDoAutoFocus();
	if ( NS_StageDoAutoFocus() )
		m_AutoFocusResult = 0;	// AutoFocus Result OK
	else
		m_AutoFocusResult = 1;	// AutoFocus Result NG
	//Saiki 20090728 Add <-----
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	CCursor::EndWaitCursor();

	Event(EV_FOCUS_DONE);
}

// =========================================================================
//
// ST_START
//
// Actions: Tell the teach can be started.
//
void CManualSitePtDlg::enter_ST_START(const cEventParams* const /* params */ )
{
	// draw the deskew location on screen
	//DrawLocationMarker(&m_ptTeach);

	// wait for the user to teach the deskew point
}

// =========================================================================
//
// ST_DRAW_START
//
// Actions:
//
void CManualSitePtDlg::enter_ST_STORE_SITE(const cEventParams* const evp )
{
	// erase the old location
	//DrawLocationMarker(&m_ptTeach, FALSE);
	// get the taught point in device coordinates
	m_ptTeach = ((cPointEventParams*) evp)->Point;

	// convert device coordinates to stage coordinates
	ClientToStage(&m_ptTeach, &m_TeachPos);

	// now move the stage such that the point taught is at the center
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
// 2013.01.09 bagus -->
//	StageMoveAbsolute(&m_TeachPos);
	NS_StageMoveAbsolute(&m_TeachPos);
// 2013.01.09 bagus <--
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	// get the new position in client window
	StageToClient(&m_TeachPos, &m_ptTeach);

	PostMessage(WM_STAGE_MOVE_DONE, 0L, 0L);

	Event(EV_NEXT);
}

// =========================================================================
//
// ST_DRAW_ERASE_OLD_DRAW_NEW
//
// Actions:
//
void CManualSitePtDlg::enter_ST_STAGE_MOVED(const cEventParams* const evp )
{
	// erase the old location
	//DrawLocationMarker (&m_ptTeach, FALSE);

	// get the new stage position and use it as the new deskew point
	STAGE_COORD_XYZ CurrPos;
	StageGetPos(&CurrPos);

	m_TeachPos.lX = CurrPos.lX;
	m_TeachPos.lY = CurrPos.lY;

	// get the new position in client window
	StageToClient(&m_TeachPos, &m_ptTeach);

	Event(EV_NEXT);
}

// =========================================================================
//
void CManualSitePtDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualSitePtDlg)
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CManualSitePtDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CManualSitePtDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
// 2009.06.11 K.Matsuo キーフック -->
//	ON_MESSAGE(WM_STAGE_MOVE_DONE, OnStageMoveDone)
// 2009.06.11 K.Matsuo キーフック <--
END_MESSAGE_MAP()

//
// State-Processing-Table Definitions
//
#pragma warning(disable: 4204)
BEGIN_STATEPROC_MAP(CManualSitePtDlg)
	ENTER_STATE(ST_INIT, enter_ST_INIT),
	ENTER_STATE(ST_MOVE_TO_SITEPT, enter_ST_MOVE_TO_SITEPT),
	ENTER_STATE(ST_FOCUS, enter_ST_FOCUS),
	ENTER_STATE(ST_START, enter_ST_START),
	ENTER_STATE(ST_STORE_SITE, enter_ST_STORE_SITE),
	ENTER_STATE(ST_STAGE_MOVED, enter_ST_STAGE_MOVED)
END_STATEPROC_MAP(CManualSitePtDlg)
#pragma warning(default: 4204)

/*---------------------------------------------------------------------------
 *	CManualSitePtDlg::OnCreate ()
 *
 *	Description:
 */
int CManualSitePtDlg::OnCreate (LPCREATESTRUCT lpcs)
{
	COverlayDialog::OnCreate (lpcs);

	return TRUE;
}

/*---------------------------------------------------------------------------
 *	CManualSitePtDlg::OnPaint ()
 *
 *	Description:
 *		Update the view.
 *
 */
void CManualSitePtDlg::OnPaint ()
{
	COverlayDialog::OnPaint();	// base class handler
}

/*---------------------------------------------------------------------------
 *	CManualSitePtDlg::OnLButtonDown ()
 *
 *	Description:
 *		Event that causes the rectangle to be updated.
 *
 */
void CManualSitePtDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( m_rcOverlay.PtInRect(point) ) {
		cPointEventParams evp(point);
		Event(EV_LBUTTON_DOWN, &evp);
	}
	else {
		COverlayDialog::OnLButtonDown(nFlags, point);
	}
}

// =========================================================================
//
BOOL CManualSitePtDlg::OnInitDialog()
{
	// Kojika 20090529 Add
	CString l_strCap, l_strMsg;
	// Kojika 20090529 Add End

	// Kojika 20090529 Change
//	SetCaption("MANUAL SITE PATTERN SETUP");
//	SetCaption("CLICK CORRECT POSITION");
	//SetCaption("手動 サイトパターン設定");
	//SetCaption("マニュアル サイトパターン設定");
	//SetMessage("クリックして位置をあわせてください");
	LoadStringML(IDS_MANUAL_SITE_PATTRN_SETUP, l_strCap, "MANUAL SITE PATTERN SETUP");
	LoadStringML(IDS_CLICK_CORRECT_POSITION, l_strMsg, "CLICK CORRECT POSITION");
	SetCaption(l_strCap);
	SetMessage(l_strMsg);
	// Kojika 20090529 Change End

	COverlayDialog::OnInitDialog();

	Event(EV_START);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CManualSitePtDlg::OnOK()
{
//	if ( !IsTeachPosValid(&m_TeachPos) ) AfxMessageBox("HOGE");

	//Saiki 20090728 Add ----->
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		::PostMessage(l_hChiefDlg, WM_MANUALSITE_POPUP_END, 1, MAKELPARAM(MAKEWORD(m_XY_ButtonUseHistry,m_Z_ButtonUseHistry),MAKEWORD(m_AutoFocusUseHistry,m_AutoFocusResult)));
	} else {
		//エラー処理記述すること！
	}
	//Saiki 20090728 Add <-----
	COverlayDialog::OnOK();
}

// =========================================================================
//
void CManualSitePtDlg::OnCancel()
{
	//Saiki 20090728 Add ----->
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		::PostMessage(l_hChiefDlg, WM_MANUALSITE_POPUP_END, 2, MAKELPARAM(MAKEWORD(m_XY_ButtonUseHistry,m_Z_ButtonUseHistry),MAKEWORD(m_AutoFocusUseHistry,m_AutoFocusResult)));
	} else {
		//エラー処理記述すること！
	}
	//Saiki 20090728 Add <-----
	COverlayDialog::OnCancel();
}

LRESULT CManualSitePtDlg::OnStageMoveDone(WPARAM wParam, LPARAM lParam)
{
	Event(EV_STAGE_MOVED);

	return 0L;
}
