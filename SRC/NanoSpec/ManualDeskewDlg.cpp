// ManualDeskewDlg.cpp : インプリメンテーション ファイル
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
#include "ManualDeskewDlg.h"
#include "NanoSpecDoc.h"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//==========================================================================
//==========================    CManualDeskewDlg      ======================
//==========================================================================
//

//
//	STATES
//
enum MANUAL_DESKEW_STATES
{
	// Initialization. It has no actions. It's constructor
	// will call "Event(EV_NEXT);" only.
	//
	ST_INIT,				// initial fall through state no processing

	ST_MOVE_TO_DESKEW_SITE, // move to the current deskew site if it's defined

	ST_FOCUS,				// do focusing

	ST_START,				// get ready

	ST_STORE_SITE,			// store the first location

	ST_STAGE_MOVED
};

//
// EVENTS
//
enum MANUAL_DESKEW_EVENTS
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
const Transition ManualDeskewTeachTransTable[] =
{
	// OldState,				EventId,		NewState
	//
	{ ST_INIT,					EV_START,		ST_MOVE_TO_DESKEW_SITE	},

	{ ST_MOVE_TO_DESKEW_SITE,	EV_NEXT,		ST_FOCUS				},

	{ ST_FOCUS, 				EV_FOCUS_DONE,	ST_START				},

	{ ST_START, 				EV_LBUTTON_DOWN,ST_STORE_SITE			},
	{ ST_START, 				EV_STAGE_MOVED, ST_STAGE_MOVED			},

	{ ST_STORE_SITE,			EV_NEXT,		ST_START				},

	{ ST_STAGE_MOVED,			EV_NEXT,		ST_START				},

	END_OF_TRANSITION_TABLE
};

// =========================================================================
//
CManualDeskewDlg::CManualDeskewDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(COverlayDialog::IDD, pParent), cActive(ManualDeskewTeachTransTable)
{
	//{{AFX_DATA_INIT(CManualDeskewDlg)
	//}}AFX_DATA_INIT
}

CManualDeskewDlg::CManualDeskewDlg(DESKEW_SITE_MANUAL* pDeskewSiteManual, CWnd* pParent /*=NULL*/)
	: m_pDeskewSiteManual(pDeskewSiteManual),
	COverlayDialog(COverlayDialog::IDD, pParent), cActive(ManualDeskewTeachTransTable)
{
	//{{AFX_DATA_INIT(CManualDeskewDlg)
	//}}AFX_DATA_INIT
//	m_logFile.Create("C:\\NANOSPEC\\LOG", "DESKEW");
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
void CManualDeskewDlg::enter_ST_INIT(const cEventParams* const /* params */ )
{
	// do nothing
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("enter_ST_INIT");
	//Saiki 20100630 Add <-----
}

// =========================================================================
//
// ST_MOVE_TO_DESKEW_SITE
//
// Actions:
//
void CManualDeskewDlg::enter_ST_MOVE_TO_DESKEW_SITE(const cEventParams* const /* params */ )
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:enter_ST_MOVE_TO_DESKEW_SITE");
	//Saiki 20100630 Add <-----
	if ( m_pDeskewSiteManual->bDefined ) {
		CCursor::BeginWaitCursor();
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:NS_StageMoveAbsolute Process Start");
		NS_StageMoveAbsolute(&m_pDeskewSiteManual->Loc);
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:NS_StageMoveAbsolute Process Complete");
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		CCursor::EndWaitCursor();
	}
	// use the current stage position as the taught position
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:StageGetPos Process Start");
	//Saiki 20100630 Add <-----
	STAGE_COORD_XYZ CurrPos;
	StageGetPos(&CurrPos);
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:StageGetPos Process Complete");
	//Saiki 20100630 Add <-----

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
void CManualDeskewDlg::enter_ST_FOCUS(const cEventParams* const /* params */ )
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:enter_ST_FOCUS");
	//Saiki 20100630 Add <-----

	CCursor::BeginWaitCursor();

	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:NS_StageDoAutoFocus Process Start");
	NS_StageDoAutoFocus();
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:NS_StageDoAutoFocus Process Complete");
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
void CManualDeskewDlg::enter_ST_START(const cEventParams* const /* params */ )
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:enter_ST_START");
	//Saiki 20100630 Add <-----
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
void CManualDeskewDlg::enter_ST_STORE_SITE(const cEventParams* const evp )
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:enter_ST_STORE_SITE");
	//Saiki 20100630 Add <-----
	// erase the old location
	//DrawLocationMarker(&m_ptTeach, FALSE);
	// get the taught point in device coordinates
	m_ptTeach = ((cPointEventParams*) evp)->Point;

	// convert device coordinates to stage coordinates
	ClientToStage(&m_ptTeach, &m_TeachPos);

	// now move the stage such that the point taught is at the center
	//Saiki 20100701 Change ----->
//	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd()))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
//	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:StageMoveAbsolute Process Start");
//	StageMoveAbsolute(&m_TeachPos);
//	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:StageMoveAbsolute Process Complete");
//	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd()))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:StageMoveAbsolute Process Start");
// 2013.01.09 bagus -->
//	StageMoveAbsolute(&m_TeachPos);
	NS_StageMoveAbsolute(&m_TeachPos);
// 2013.01.09 bagus <--
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:StageMoveAbsolute Process Complete");
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	//Saiki 20100701 Change <-----

	// get the new position in client window
	StageToClient(&m_TeachPos, &m_ptTeach);
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:WM_STAGE_MOVE_DONE PostMessage Start");
	PostMessage(WM_STAGE_MOVE_DONE, 0L, 0L);
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:WM_STAGE_MOVE_DONE PostMessage Complete");

	Event(EV_NEXT);
}

// =========================================================================
//
// ST_DRAW_ERASE_OLD_DRAW_NEW
//
// Actions:
//
void CManualDeskewDlg::enter_ST_STAGE_MOVED(const cEventParams* const evp )
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:enter_ST_STAGE_MOVED");
	//Saiki 20100630 Add <-----
	// erase the old location
	//DrawLocationMarker (&m_ptTeach, FALSE);

	// get the new stage position and use it as the new deskew point
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:StageGetPos Process Start");
	//Saiki 20100630 Add <-----
	STAGE_COORD_XYZ CurrPos;
	StageGetPos(&CurrPos);
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:StageGetPos Process End");
	//Saiki 20100630 Add <-----

	m_TeachPos.lX = CurrPos.lX;
	m_TeachPos.lY = CurrPos.lY;

	// get the new position in client window
	StageToClient(&m_TeachPos, &m_ptTeach);

	Event(EV_NEXT);
}

// =========================================================================
//
void CManualDeskewDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualDeskewDlg)
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CManualDeskewDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CManualDeskewDlg)
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
BEGIN_STATEPROC_MAP(CManualDeskewDlg)
	ENTER_STATE(ST_INIT, enter_ST_INIT),
	ENTER_STATE(ST_MOVE_TO_DESKEW_SITE, enter_ST_MOVE_TO_DESKEW_SITE),
	ENTER_STATE(ST_FOCUS, enter_ST_FOCUS),
	ENTER_STATE(ST_START, enter_ST_START),
	ENTER_STATE(ST_STORE_SITE, enter_ST_STORE_SITE),
	ENTER_STATE(ST_STAGE_MOVED, enter_ST_STAGE_MOVED)
END_STATEPROC_MAP(CManualDeskewDlg)
#pragma warning(default: 4204)

/*---------------------------------------------------------------------------
 *	CManualDeskewDlg::OnCreate ()
 *
 *	Description:
 */
int CManualDeskewDlg::OnCreate (LPCREATESTRUCT lpcs)
{
	COverlayDialog::OnCreate (lpcs);

	return TRUE;
}

/*---------------------------------------------------------------------------
 *	CManualDeskewDlg::OnPaint ()
 *
 *	Description:
 *		Update the view.
 *
 */
void CManualDeskewDlg::OnPaint ()
{
	COverlayDialog::OnPaint();	// base class handler
}

/*---------------------------------------------------------------------------
 *	CManualDeskewDlg::OnLButtonDown ()
 *
 *	Description:
 *		Event that causes the rectangle to be updated.
 *
 */
void CManualDeskewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:OnLButtonDown");
	//Saiki 20100630 Add <-----
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
BOOL CManualDeskewDlg::OnInitDialog()
{
	// Kojika 20090529 Add
	CString l_strCap, l_strMsg;
	// Kojika 20090529 Add End

	// Kojika 20090529 Change
//	SetCaption("MANUAL DESKEW SETUP");
//	SetCaption("CLICK CORRECT POSITION");
	//Saiki 20090410 Change ----->
	//SetCaption("手動 傾き補正設定");
	//SetCaption("マニュアル デスキュー設定");
	//Saiki 20090410 Change <-----
	//SetMessage("クリックして位置をあわせてください");
	LoadStringML(IDS_MANUAL_DESKEW_SETUP, l_strCap, "MANUAL DESKEW SETUP");
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
void CManualDeskewDlg::OnOK()
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:OnOK");
	//Saiki 20100630 Add <-----
//	if ( !IsTeachPosValid(&m_TeachPos) ) AfxMessageBox("HOGE");

	COverlayDialog::OnOK();
}

// =========================================================================
//
void CManualDeskewDlg::OnCancel()
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:OnCancel");
	//Saiki 20100630 Add <-----
	COverlayDialog::OnCancel();
}

LRESULT CManualDeskewDlg::OnStageMoveDone(WPARAM wParam, LPARAM lParam)
{
	//Saiki 20100630 Add ----->
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->OperationLogging("CManualDeskewDlg:OnStageMoveDone");
	//Saiki 20100630 Add <-----
	Event(EV_STAGE_MOVED);

	return 0L;
}

