// PatRecDeskewDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <math.h>
#include <patrec.hxx>
#include <active.hxx>
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "BitMapStatic.h"
//#include "SaveAsDlg.h"
#include "PatRecDeskewDlg.h"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//==========================================================================
//==========================	CPatRecDeskewDlg	========================
//==========================================================================
//

//
//	STATES
//
enum PATREC_DESKEW__TEACH_STATES
{
	// Initialization. It has no actions. It's constructor
	// will call "Event(EV_NEXT);" only.
	//
	ST_INIT,				// initial fall through state no processing

	ST_START,				// get ready

	ST_VERIFY_TEACH,		// verify the teaching result

	ST_DRAW_START,			// lbutton is pressed, start drawing the rectangle
	ST_ERASE_OLD_DRAW_NEW,	// when mouse moved, rect is enlarged, erase old rect and draw new
	ST_DRAW_END,			// rbutton is pressed, finishing the rect teach
	ST_DO_PAT_REC,			// do pattern rec.
};

//
// EVENTS
//
enum PATREC_DESKEW_TEACH_EVENTS
{
	EV_START,
	EV_NEXT,				// go to the next state in the state transitation table
	EV_LBUTTON_DOWN,
	EV_MOUSE_MOVE,
	EV_LBUTTON_UP,
	EV_POINT_TAUGHT,		// the rect taught is either a point or a line
	EV_AREA_TAUGHT, 		// the rect taught is a rect.
	EV_VERIFY,				// verify the pattern
};

//
// STATE TRANSITION TABLE
//
const Transition PatRecDeskewTeachTransTable[] =
{
	// OldState,				EventId,			NewState
	//
	{ ST_INIT,					EV_START,		ST_START				},

	{ ST_START, 				EV_LBUTTON_DOWN, ST_DRAW_START			},
	{ ST_START, 				EV_VERIFY,		ST_VERIFY_TEACH 		},

	{ ST_VERIFY_TEACH,			EV_NEXT,			ST_START			},

	{ ST_DRAW_START,			EV_MOUSE_MOVE,	ST_ERASE_OLD_DRAW_NEW	},
	{ ST_DRAW_START,			EV_LBUTTON_UP,	ST_DRAW_END 			},

	{ ST_ERASE_OLD_DRAW_NEW,	EV_MOUSE_MOVE,	ST_ERASE_OLD_DRAW_NEW	},
	{ ST_ERASE_OLD_DRAW_NEW,	EV_LBUTTON_UP,	ST_DRAW_END 			},

	{ ST_DRAW_END,				EV_POINT_TAUGHT, ST_START				},
	{ ST_DRAW_END,				EV_AREA_TAUGHT, ST_DO_PAT_REC			},

	{ ST_DO_PAT_REC,			EV_NEXT,			ST_START			},

	END_OF_TRANSITION_TABLE
};

// =========================================================================
//
CPatRecDeskewDlg::CPatRecDeskewDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CPatRecDeskewDlg::IDD, pParent), cActive(PatRecDeskewTeachTransTable)
{
	//{{AFX_DATA_INIT(CPatRecDeskewDlg)
	//}}AFX_DATA_INIT

	// init the rects
	m_rcNew.SetRectEmpty();
	m_rcOld.SetRectEmpty();

	m_OffsetPos.lX = 0;
	m_OffsetPos.lY = 0;
}

CPatRecDeskewDlg::CPatRecDeskewDlg(int iMode, int iLens, DESKEW_SITE* pDeskewSite, CWnd* pParent /*=NULL*/)
	: m_iMode(iMode), m_iLens(iLens), m_pDeskewSite(pDeskewSite),
	COverlayDialog(CPatRecDeskewDlg::IDD, pParent), cActive(PatRecDeskewTeachTransTable)
{
	//{{AFX_DATA_INIT(CPatRecDeskewDlg)
	//}}AFX_DATA_INIT
	// init the rects
	m_rcNew.SetRectEmpty();
	m_rcOld.SetRectEmpty();

	m_OffsetPos.lX = 0;
	m_OffsetPos.lY = 0;
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
void CPatRecDeskewDlg::enter_ST_INIT(const cEventParams* const /* params */ )
{
	// do nothing
}

// =========================================================================
//
// ST_START
//
// Actions:
//
void CPatRecDeskewDlg::enter_ST_START(const cEventParams* const /* params */ )
{
//	m_cbLens.SetCurSel(m_iLens);
}

// =========================================================================
//
// ST_VERIFY_TEACH
//
// Actions: Move the taught pattern to the center of the overlay and do
//			a match and draw the matched rectangle.
//
void CPatRecDeskewDlg::enter_ST_VERIFY_TEACH(const cEventParams* const /* params */ )
{
// 2009.06.09 K.Matsuo -->
	CString strMsg;
	CString strTitle;

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
		MessageBox(strMsg, strTitle, MB_OK);
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
		Event(EV_NEXT);
		return;
	}
// 2009.06.09 K.Matsuo <--

	CCursor::BeginWaitCursor();

	PatRecSecondMatch();

	CCursor::EndWaitCursor();

// 2009.06.09 K.Matsuo -->
	// 必ずJoyStickの状態を戻すこと
	m_pDoc->JoyStickStatusRestore();
// 2009.06.09 K.Matsuo <--

	Event(EV_NEXT);
}

// =========================================================================
//
// ST_DRAW_START
//
// Actions: erase the old rectangle and define the starting point of the new rect
//
void CPatRecDeskewDlg::enter_ST_DRAW_START(const cEventParams* const evp )
{
	// disable the verify button
#if 0
//	m_btnDeskewVerify.EnableWindow(FALSE);
//	m_btnOffsetApply.EnableWindow(FALSE);
#endif

	// erase the old rect
	OverlayDrawRectangle (m_rcOld, FALSE);		// don't care the old rect exists or not
#if 0
//	m_stcModel.DeleteObject();
#endif

	// define the starting point of the new rect
	m_ptFirst = ((cPointEventParams*) evp)->Point;

	// init the new rect
	m_rcOld.left = m_rcOld.top = m_rcOld.right = m_rcOld.bottom = 0;
	m_rcNew.left = m_rcNew.top = m_rcNew.right = m_rcNew.bottom = 0;
}

// =========================================================================
//
// ST_DRAW_ERASE_OLD_DRAW_NEW
//
// Actions:
//
void CPatRecDeskewDlg::enter_ST_ERASE_OLD_DRAW_NEW(const cEventParams* const evp )
{
#if 1
	// erase the old bmp
	m_stcModel.DeleteObject();
#endif

	UpdateRect (((cPointEventParams *)evp)->Point);
}

// =========================================================================
//
// ST_DRAW_END
//
// Actions:
//
void CPatRecDeskewDlg::enter_ST_DRAW_END(const cEventParams* const /* params */ )
{
	//Kojika 20090525 Add
	CString strBuffer;
	//Kojika 20090525 Add End
	// check the taught area: cannot be too small
	if ( (m_rcNew.right - m_rcNew.left) < PAT_REC_WIDTH_MIN ||
			(m_rcNew.bottom - m_rcNew.top) < PAT_REC_HEIGHT_MIN ) {
		if ( m_rcNew.right - m_rcNew.left > 3 ||
				m_rcNew.bottom - m_rcNew.top > 3 ) {
			//AfxMessageBox(IDS_REC_TOO_SMALL);
			//Kojika 20090525 Change
			LoadStringML(IDS_REC_TOO_SMALL, strBuffer, "The taught rectangle is too SMALL. Please teach again!");
			AfxMessageBox(strBuffer);
			//Kojika 20090525 Change End

// 2009.06.11 K.Matsuo -->
			OverlayDrawRectangle (m_rcNew, FALSE);	// erase the rect

			m_rcOld.left = m_rcOld.top = m_rcOld.right = m_rcOld.bottom = 0;
			m_rcNew.left = m_rcNew.top = m_rcNew.right = m_rcNew.bottom = 0;

			Event(EV_POINT_TAUGHT);
			return;
// 2009.06.11 K.Matsuo <--
		}

		OverlayDrawRectangle (m_rcNew, FALSE);	// erase the rect

		m_rcOld.left = m_rcOld.top = m_rcOld.right = m_rcOld.bottom = 0;
		m_rcNew.left = m_rcNew.top = m_rcNew.right = m_rcNew.bottom = 0;

// 2009.06.10 K.Matsuo -->
		CString strMsg;
		CString strTitle;

		if( !m_pDoc->JoyStickChangeDisable() ){
			LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
			LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
			MessageBox(strMsg, strTitle, MB_OK);
		}
		else {
			// move the first point taught to the center of the overlay
			STAGE_COORD TeachPos;					// chuck coord of the first point taught
			ClientToStage(&m_ptFirst, &TeachPos);	// convert client point to chuck point

			((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
// 2013.01.09 bagus -->
//			StageMoveAbsolute(&TeachPos);
			NS_StageMoveAbsolute(&TeachPos);
// 2013.01.09 bagus <--
			((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		}

		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
// 2009.06.10 K.Matsuo <--

		Event(EV_POINT_TAUGHT);
	}
	else if ( (m_rcNew.right - m_rcNew.left) > PAT_REC_WIDTH_MAX ||
				(m_rcNew.bottom - m_rcNew.top) > PAT_REC_HEIGHT_MAX) {
		// the taught area is too big
		//AfxMessageBox(IDS_REC_TOO_BIG);
		//Kojika 20090525 Change
		LoadStringML(IDS_REC_TOO_BIG, strBuffer, "The taught rectangle is too BIG. Please teach again!\n");
		AfxMessageBox(strBuffer);
		//Kojika 20090525 Change End

		OverlayDrawRectangle (m_rcNew, FALSE);	// erase the rect
		m_rcOld.left = m_rcOld.top = m_rcOld.right = m_rcOld.bottom = 0;
		m_rcNew.left = m_rcNew.top = m_rcNew.right = m_rcNew.bottom = 0;

		Event(EV_POINT_TAUGHT);
	}
	else {	// a legal area has been taught
		Event(EV_AREA_TAUGHT);
	}
}

// =========================================================================
//
// ST_DO_PAT_REC
//
// Actions:
//
void CPatRecDeskewDlg::enter_ST_DO_PAT_REC(const cEventParams* const /* params */ )
{
// 2009.06.09 K.Matsuo -->
	CString strMsg;
	CString strTitle;

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
		MessageBox(strMsg, strTitle, MB_OK);
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
		Event(EV_NEXT);
		return;
	}
// 2009.06.09 K.Matsuo <--

	CCursor::BeginWaitCursor();

	// first teach the pattern and then do a match immdeiately after
	if ( !TeachPatternRec() || ! PatRecFirstMatch() ) {
		CCursor::EndWaitCursor();
// 2009.06.09 K.Matsuo -->
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
		Event(EV_NEXT);
		return;
// 2009.06.09 K.Matsuo <--
	}

	// オフセットをクリアする
	TCHAR szBuffer[256];
	m_OffsetPos.lX = 0;
	m_OffsetPos.lY = 0;
	_stprintf(szBuffer, "%.3lf", (double)(m_OffsetPos.lX)/MICROMETRE);
	SetDlgItemText(IDC_OFFSET_X, szBuffer);
	_stprintf(szBuffer, "%.3lf", (double)(m_OffsetPos.lY)/MICROMETRE);
	SetDlgItemText(IDC_OFFSET_Y, szBuffer);

	// move the matched pattern to the center of the overlay
	MovePatternToCenter();

	// perform another match
	if ( !PatRecSecondMatch() ) {
		CCursor::EndWaitCursor();
// 2009.06.09 K.Matsuo -->
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
		Event(EV_NEXT);
		return;
// 2009.06.09 K.Matsuo <--
	}

	PatRecSaveModel(DESKEW_IMG_TEMP);

	m_iLens = StageGetCurLens();
	m_szLens = srTurret[m_iLens].szName;

	// enable the verify button
	m_btnDeskewVerify.EnableWindow(TRUE);

	DrawModel();
	Invalidate();

	UpdateData(FALSE);

	CCursor::EndWaitCursor();

// 2009.06.09 K.Matsuo -->
	// 必ずJoyStickの状態を戻すこと
	m_pDoc->JoyStickStatusRestore();
// 2009.06.09 K.Matsuo <--

	Event(EV_NEXT);
}

// =========================================================================
//
void CPatRecDeskewDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatRecDeskewDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DESKEW_VERIFY_BUTTON, m_btnDeskewVerify);
	DDX_Control(pDX, IDC_DESKEW_MODEL, m_stcModel);
	DDX_Text(pDX, IDC_LENS, m_szLens);
	DDX_Control(pDX, IDC_OFFSET_APPLY_BUTTON, m_btnOffsetApply);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CPatRecDeskewDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CPatRecDeskewDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DESKEW_VERIFY_BUTTON, OnVerify)
	ON_BN_CLICKED(IDC_OFFSET_APPLY_BUTTON, OnOffsetApply)
	//}}AFX_MSG_MAP
// 2009.06.11 K.Matsuo キーフック -->
//	ON_MESSAGE(WM_STAGE_MOVE_DONE, OnStageMoveDone)
// 2009.06.11 K.Matsuo キーフック <--
END_MESSAGE_MAP()

//
// State-Processing-Table Definitions
//
#pragma warning(disable: 4204)
BEGIN_STATEPROC_MAP(CPatRecDeskewDlg)
	ENTER_STATE(ST_INIT, enter_ST_INIT),
	ENTER_STATE(ST_START, enter_ST_START),
	ENTER_STATE(ST_VERIFY_TEACH, enter_ST_VERIFY_TEACH),
	ENTER_STATE(ST_DRAW_START, enter_ST_DRAW_START),
	ENTER_STATE(ST_ERASE_OLD_DRAW_NEW, enter_ST_ERASE_OLD_DRAW_NEW),
	ENTER_STATE(ST_DRAW_END, enter_ST_DRAW_END),
	ENTER_STATE(ST_DO_PAT_REC, enter_ST_DO_PAT_REC),
END_STATEPROC_MAP(CPatRecDeskewDlg)
#pragma warning(default: 4204)

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::OnCreate ()
 *
 *	Description:
 */
int CPatRecDeskewDlg::OnCreate (LPCREATESTRUCT lpcs)
{
	COverlayDialog::OnCreate (lpcs);

	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(m_iLens);
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

// 2009.07.29 K.Matsuo Add -->
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
// 2009.07.29 K.Matsuo Add <--

	return TRUE;
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::OnPaint ()
 *
 *	Description:
 *		Update the view.
 *
 */
void CPatRecDeskewDlg::OnPaint()
{
	COverlayDialog::OnPaint (); // base class handler

	// Update the rectangle if there is one
	//
	OverlayDrawRectangle(m_rcNew);
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::OnLButtonDown ()
 *
 *	Description:
 *		Event that causes the rectangle to be updated.
 *
 */
void CPatRecDeskewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( m_rcOverlay.PtInRect(point) ) {
		cPointEventParams evp(point);
		Event(EV_LBUTTON_DOWN, &evp);
	}
	else {
		COverlayDialog::OnLButtonDown(nFlags, point);
	}
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::OnLButtonUp ()
 *
 *	Description:
 *		Event that causes the rectangle to be updated.
 *
 */
void CPatRecDeskewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	Event(EV_LBUTTON_UP);
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::OnMouseMove ()
 *
 *	Description:
 *		Event that causes the rectangle to be updated.
 *
 */
void CPatRecDeskewDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	COverlayDialog::OnMouseMove(nFlags, point);

	if ( !CCursor::m_bWaitCursor ) {
		point.y = min( point.y, m_rcOverlay.bottom - 1 );
		cPointEventParams evp(point);
		Event(EV_MOUSE_MOVE, &evp);
	}
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::TeachPatternRec ()
 *
 *	Description:
 *		Perform pattern recognition teach on the indicated area
 *
 */
BOOL CPatRecDeskewDlg::TeachPatternRec(void)
{
	// convert the taught rect to video overlay coordinate
	RECT rcTeach = m_rcNew;
//	ClientToScreen(&rcTeach);	// convert from client to screen coordinate

	// save the teaching rect into
	BOOL bRet = PatRecTeach(&rcTeach);
	return bRet;
//	return TRUE;
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::PatRecFirstMatch ()
 *
 *	Description:
 *		match the taught model and find the center of the matched rect.
 *	Generate appropriate event base on the matching score.
 */
BOOL CPatRecDeskewDlg::PatRecFirstMatch()
{
	PatRecMatch(&m_DeskewResult);
	TCHAR szBuffer[256];
	_stprintf(szBuffer, "%.2lf", m_DeskewResult.Score);
	SetDlgItemText(IDC_DESKEW_MATCH_SCORE, szBuffer);

	return TRUE;
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::MovePatternToCenter ()
 *
 *	Description:
 *		Move the stage such that the pattern taught is at the
 *		center of the screen.
 */
void CPatRecDeskewDlg::MovePatternToCenter(void)
{
	STAGE_COORD CenterPos;			// chuck coord of the first point taught
	CPoint ptResult(m_DeskewResult.XPos, m_DeskewResult.YPos);
	ptResult.Offset(m_rcOverlay.TopLeft());

	ClientToStage(&ptResult, &CenterPos);	// convert client point to chuck point

	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
// 2013.01.09 bagus -->
//	StageMoveAbsolute(&CenterPos);
	NS_StageMoveAbsolute(&CenterPos);
// 2013.01.09 bagus <--
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::PatRecSecondMatch ()
 *
 *	Description:
 *		mattch the taught model and find the center of the matched rect.
 *		Generate apporopriate event base	on the matching score.
 */
BOOL CPatRecDeskewDlg::PatRecSecondMatch(void)
{
	Sleep(4000);

	PatRecMatch(&m_DeskewResult);

	TCHAR szBuffer[256];
	_stprintf(szBuffer, "%.2lf", m_DeskewResult.Score);
	SetDlgItemText(IDC_DESKEW_MATCH_SCORE, szBuffer);

	if ( m_DeskewResult.Score < 40 ) {
		return FALSE;
	}

	CPoint NewCenter;
	NewCenter.x = int(m_DeskewResult.XPos + 0.5); NewCenter.y = int(m_DeskewResult.YPos + 0.5);
	NewCenter.Offset(m_rcOverlay.TopLeft());

	// draw the rect at a new position
	OverlayDrawRectangle (m_rcOld, FALSE);	// erase the old rect

	// draw the new rectangle
	POINT OldCenter;
	OldCenter.x = (m_rcNew.right - m_rcNew.left) / 2 + m_rcNew.left;
	OldCenter.y = (m_rcNew.bottom - m_rcNew.top) / 2 + m_rcNew.top;
	int off_x = NewCenter.x - OldCenter.x;
	int off_y = NewCenter.y - OldCenter.y;
	m_rcNew.left += off_x; m_rcNew.top	+= off_y;
	m_rcNew.right += off_x; m_rcNew.bottom += off_y;

	STAGE_COORD CenterPos;
	ClientToStage(&NewCenter, &CenterPos);

	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
// 2013.01.09 bagus -->
//	StageMoveAbsolute(&CenterPos);
	NS_StageMoveAbsolute(&CenterPos);
// 2013.01.09 bagus <--
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	// 2009.07.21 K.Matsuo NSSTAGE -->
	// センター座標を一時記憶
	m_CenterPos = CenterPos;
	NS_ConvertToStageGetPosCoord(&m_CenterPos);
	// 2009.07.21 K.Matsuo NSSTAGE <--

	m_btnOffsetApply.EnableWindow(TRUE);

	POINT TeachPt;
	RECT TempR = m_rcNew;
	StageToClient(&CenterPos, &TeachPt);

	m_rcNew.left = TeachPt.x - (TempR.right - TempR.left) / 2;
	m_rcNew.right = TeachPt.x + (TempR.right - TempR.left) / 2;
	m_rcNew.top = TeachPt.y - (TempR.bottom - TempR.top) / 2;
	m_rcNew.bottom = TeachPt.y + (TempR.bottom - TempR.top) / 2;

	OverlayDrawRectangle(m_rcNew); // draw the new rect

	m_rcOld = m_rcNew;

	return TRUE;
}

/*---------------------------------------------------------------------------
 *	CPatRecDeskewDlg::UpdateRect ()
 *
 *	Description:
 *		Update the rect taught on the screen with the new point.
 *	Erase the old rect and draw the new rect.
 */

void CPatRecDeskewDlg::UpdateRect (CPoint point)
{
	// record this point as the second point that decides the rect.
	m_ptSecond = point;

	m_rcNew.left = __min (m_ptFirst.x, m_ptSecond.x); m_rcNew.right = __max (m_ptFirst.x, m_ptSecond.x);
	m_rcNew.top = __min (m_ptFirst.y, m_ptSecond.y); m_rcNew.bottom = __max (m_ptFirst.y, m_ptSecond.y);

	m_rcNew.OffsetRect(-m_rcOverlay.TopLeft());
	OverlayDrawRectangle (m_rcOld, FALSE);	// erase the old rect

	OverlayDrawCenter();

	OverlayDrawRectangle(m_rcNew);	// draw the new rect

	m_rcOld = m_rcNew;			// update
}

// =========================================================================
//
BOOL CPatRecDeskewDlg::OnInitDialog()
{
char	m_szTitle[81];
char	m_szMessage[81];
char	m_szGroup[81];
//Saiki 20090530 Add ----->
CString strTitle, strMsg, strGrp, strBuffer;
//Saiki 20090530 Add <-----

	LoadStringML(IDS_OFFSET_X, strBuffer, "Offset X(mm)");
	CreateDialogItem("STATIC", strBuffer,		SS_CENTERIMAGE, CRect(CPoint(499,372), CSize(42,12)),	IDC_OFFSET_X_LABEL);
	CreateDialogItem("STATIC", "",				SS_SUNKEN | SS_RIGHT,	CRect(CPoint(548,372), CSize(35,12)),	IDC_OFFSET_X);
	LoadStringML(IDS_OFFSET_Y, strBuffer, "Offset Y(mm)");
	CreateDialogItem("STATIC", strBuffer,		SS_CENTERIMAGE, CRect(CPoint(499,389), CSize(42,12)),	IDC_OFFSET_Y_LABEL);
	CreateDialogItem("STATIC", "",				SS_SUNKEN | SS_RIGHT,	CRect(CPoint(548,389), CSize(35,12)),	IDC_OFFSET_Y);
	LoadStringML(IDS_BUTTON_OFFSET_APPLY, strBuffer, "Offset Apply");
	CreateDialogItem("BUTTON", strBuffer,		BS_PUSHBUTTON,	CRect(CPoint(499,409), CSize(94,24)),	IDC_OFFSET_APPLY_BUTTON);

	memset(m_szTitle, '\0', sizeof(m_szTitle));
	memset(m_szMessage, '\0', sizeof(m_szMessage));
	memset(m_szGroup, '\0', sizeof(m_szGroup));
	switch( m_iMode ){
	case SITEPR_MODE:
		//Saiki 20090530 Change ----->
		//sprintf(m_szTitle, "%s",	"サイトパターン設定");
		//sprintf(m_szMessage, "%s",	"パターンを囲ってください");
		//sprintf(m_szGroup, "%s",	"サイトパターン");
		LoadStringML(IDS_SITE_PATTERN_SET, strTitle, "Site Pattern Setting");
		LoadStringML(IDS_PATTERN_TEACH_RECT, strMsg, "Pattern Teaching Rect");
		LoadStringML(IDS_SITE_PATTERN_GRP, strGrp, "Site Pattern");
		sprintf(m_szTitle, "%s",	(LPCTSTR)strTitle);
		sprintf(m_szMessage, "%s",	(LPCTSTR)strMsg);
		sprintf(m_szGroup, "%s",	(LPCTSTR)strGrp);
		//Saiki 20090530 Change <-----

		GetDlgItem(IDC_OFFSET_X_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OFFSET_X)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OFFSET_Y_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OFFSET_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OFFSET_APPLY_BUTTON)->ShowWindow(SW_SHOW);
		break;
	case DESKEW_MODE:
	default:
		//Saiki 20090530 Change ----->
		//sprintf(m_szTitle, "%s",	"デスキュー設定");
		//sprintf(m_szMessage, "%s",	"パターンマークを囲ってください");
		//sprintf(m_szGroup, "%s",	"デスキュー");
		LoadStringML(IDS_DESKEW_SETTING, strTitle, "Deskew Setting");
		LoadStringML(IDS_PATTERN_MARK_TEACH_RECT, strMsg, "Pattern Mark Teaching Rect");
		LoadStringML(IDS_DESKEW_GRP, strGrp, "Deskew");
		sprintf(m_szTitle, "%s",	(LPCTSTR)strTitle);
		sprintf(m_szMessage, "%s",	(LPCTSTR)strMsg);
		sprintf(m_szGroup, "%s",	(LPCTSTR)strGrp);
		//Saiki 20090530 Change <-----

		GetDlgItem(IDC_OFFSET_X_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OFFSET_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OFFSET_Y_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OFFSET_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OFFSET_APPLY_BUTTON)->ShowWindow(SW_HIDE);
		break;
	}

//	  PUSHBUTTON	  "VERIFY",IDC_DESKEW_VERIFY_BUTTON,499,487,94,24
//	  GROUPBOX		  "Deskew",IDC_STATIC,295,362,320,155
//	  LTEXT 		  "Source",IDC_STATIC,300,372,40,8,SS_CENTERIMAGE
//	  LTEXT 		  "Match Score",IDC_STATIC,499,467,40,12,SS_CENTERIMAGE
//	  LTEXT 		  "",IDC_DESKEW_MATCH_SCORE2,544,467,35,12,SS_SUNKEN
//	  LTEXT 		  "%",IDC_STATIC,584,467,8,12,SS_CENTERIMAGE
//	  CONTROL		  "",IDC_DESKEW_MODEL,"Static",SS_BLACKFRAME,300,382,110,130

//	CreateDialogItem("BUTTON", "VERIFY",		BS_PUSHBUTTON,	CRect(CPoint(499,487), CSize(94,24)),	IDC_DESKEW_VERIFY_BUTTON);
//	CreateDialogItem("BUTTON", "Deskew",		BS_GROUPBOX,	CRect(CPoint(295,362), CSize(320,155)), IDC_STATIC);
//	CreateDialogItem("STATIC", "Source",		SS_CENTERIMAGE, CRect(CPoint(300,372), CSize(40,8)),	IDC_STATIC);
//	CreateDialogItem("STATIC", "Match Score",	SS_CENTERIMAGE, CRect(CPoint(499,467), CSize(40,12)),	IDC_STATIC);
//	CreateDialogItem("STATIC", "",				SS_SUNKEN,		CRect(CPoint(544,467), CSize(35,12)),	IDC_DESKEW_MATCH_SCORE);
//	CreateDialogItem("STATIC", "%", 			SS_CENTERIMAGE, CRect(CPoint(584,467), CSize(8,12)),	IDC_STATIC);
//	CreateDialogItem("STATIC", "",				SS_BLACKFRAME,	CRect(CPoint(300,382), CSize(110,130)), IDC_DESKEW_MODEL);
//	CreateDialogItem("STATIC", "Lens",			SS_CENTERIMAGE, CRect(CPoint(499,450), CSize(40,12)),	IDC_STATIC);
//	CreateDialogItem("STATIC", "",				SS_SUNKEN,		CRect(CPoint(544,450), CSize(35,12)),	IDC_LENS);
//
//	SetCaption("DESKEW SETUP");
//	SetMessage("Enclose the pattern mark");
	//Saiki 20090530 Change ----->
//	CreateDialogItem("BUTTON", "照合",			BS_PUSHBUTTON,	CRect(CPoint(499,487), CSize(94,24)),	IDC_DESKEW_VERIFY_BUTTON);
////	CreateDialogItem("BUTTON", "デスキュー",	BS_GROUPBOX,	CRect(CPoint(295,362), CSize(320,155)), IDC_STATIC);
//	CreateDialogItem("BUTTON", m_szGroup,		BS_GROUPBOX,	CRect(CPoint(295,362), CSize(320,155)), IDC_STATIC);
//	CreateDialogItem("STATIC", "ソース",		SS_CENTERIMAGE, CRect(CPoint(300,372), CSize(40,8)),	IDC_STATIC);
//	CreateDialogItem("STATIC", "認識スコア",	SS_CENTERIMAGE, CRect(CPoint(499,467), CSize(40,12)),	IDC_STATIC);
//	CreateDialogItem("STATIC", "",				SS_SUNKEN,		CRect(CPoint(544,467), CSize(35,12)),	IDC_DESKEW_MATCH_SCORE);
//	CreateDialogItem("STATIC", "%", 			SS_CENTERIMAGE, CRect(CPoint(584,467), CSize(8,12)),	IDC_STATIC);
//	CreateDialogItem("STATIC", "",				SS_BLACKFRAME,	CRect(CPoint(300,382), CSize(110,130)), IDC_DESKEW_MODEL);
//	CreateDialogItem("STATIC", "レンズ",		SS_CENTERIMAGE, CRect(CPoint(499,450), CSize(40,12)),	IDC_STATIC);
//	CreateDialogItem("STATIC", "",				SS_SUNKEN,		CRect(CPoint(544,450), CSize(35,12)),	IDC_LENS);
	LoadStringML(IDS_BUTTON_VERIFY, strBuffer, "VERIFY");
	CreateDialogItem("BUTTON", strBuffer,		BS_PUSHBUTTON,	CRect(CPoint(499,487), CSize(94,24)),	IDC_DESKEW_VERIFY_BUTTON);
//	CreateDialogItem("BUTTON", "デスキュー",	BS_GROUPBOX,	CRect(CPoint(295,362), CSize(320,155)), IDC_STATIC);
	CreateDialogItem("BUTTON", m_szGroup,		BS_GROUPBOX,	CRect(CPoint(295,362), CSize(320,155)), IDC_STATIC);
	LoadStringML(IDS_STATIC_SOURCE, strBuffer, "Source");
	CreateDialogItem("STATIC", strBuffer,		SS_CENTERIMAGE, CRect(CPoint(300,372), CSize(40,8)),	IDC_STATIC);
	LoadStringML(IDS_STATIC_MATCH_SCORE, strBuffer, "Match Score");
	CreateDialogItem("STATIC", strBuffer,	SS_CENTERIMAGE, CRect(CPoint(499,467), CSize(40,12)),	IDC_STATIC);
	CreateDialogItem("STATIC", "",				SS_SUNKEN,		CRect(CPoint(548,467), CSize(35,12)),	IDC_DESKEW_MATCH_SCORE);
	CreateDialogItem("STATIC", "%", 			SS_CENTERIMAGE, CRect(CPoint(586,467), CSize(8,12)),	IDC_STATIC);
	CreateDialogItem("STATIC", "",				SS_BLACKFRAME,	CRect(CPoint(300,382), CSize(110,130)), IDC_DESKEW_MODEL);
	LoadStringML(IDS_STATIC_LENS, strBuffer, "Lens");
	CreateDialogItem("STATIC", strBuffer,		SS_CENTERIMAGE, CRect(CPoint(499,450), CSize(40,12)),	IDC_STATIC);
	CreateDialogItem("STATIC", "",				SS_SUNKEN,		CRect(CPoint(548,450), CSize(35,12)),	IDC_LENS);
	//Saiki 20090530 Change <-----

//	SetCaption("デスキュー 設定");
//	SetMessage("パターンマークを囲ってください");
	SetCaption(m_szTitle);
	SetMessage(m_szMessage);

	COverlayDialog::OnInitDialog();

	m_btnDeskewVerify.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnDeskewVerify.DrawFlatFocus(TRUE);
	m_btnOffsetApply.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnOffsetApply.DrawFlatFocus(TRUE);

	///// Lens Magnification /////
	ConfigFile_GetNanoSpecIni(srTurret, CONFIG_FILE_SR_TURRET);

	// disable the verify button initially
	m_btnDeskewVerify.EnableWindow(FALSE);
	m_btnOffsetApply.EnableWindow(FALSE);

	if ( m_pDeskewSite->bDefined ) {

		CCursor::BeginWaitCursor();
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		NS_StageMoveAbsolute(&m_pDeskewSite->Loc);
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		CCursor::EndWaitCursor();

		if ( ::GetFileAttributes(DESKEW_IMG_TEMP) != 0xffffffff ) {
			DrawModel();
			//---> Add Kawamura 2009.05.28
			PatRecRestoreModel(DESKEW_IMG_TEMP);
			m_btnDeskewVerify.EnableWindow(TRUE);
			//<--- Add Kawamura 2009.05.28
		}
	}

	TCHAR szBuffer[256];
	_stprintf(szBuffer, "%.3lf", (double)(m_OffsetPos.lX)/MICROMETRE);
	SetDlgItemText(IDC_OFFSET_X, szBuffer);
	_stprintf(szBuffer, "%.3lf", (double)(m_OffsetPos.lY)/MICROMETRE);
	SetDlgItemText(IDC_OFFSET_Y, szBuffer);

	Event(EV_START);

	// 2009.07.17 K.Matsuo -->
	m_szLens = srTurret[m_iLens].szName;
	UpdateData(FALSE);
	// 2009.07.17 K.Matsuo <--

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CPatRecDeskewDlg::OnTimer(UINT nIDEvent)
{
	COverlayDialog::OnTimer(nIDEvent);
}

// =========================================================================
//
void CPatRecDeskewDlg::OnClose()
{
	COverlayDialog::OnClose();
}

// =========================================================================
//
void CPatRecDeskewDlg::OnOK()
{
	//Kojika 20090525 Add
	CString strBuffer;
	//Kojika 20090525 Add End
	if ( !m_stcModel.IsValid() ) {
		//AfxMessageBox("未登録のため、キャンセル終了します。");
		//Kojika 20090525 Change
		LoadStringML(IDS_CANCELLATION_END_UNREGIST , strBuffer, "It makes it to the cancellation end for unregistration.");
		AfxMessageBox(strBuffer);
		//Kojika 20090525 Change End
		OnCancel();
		return;
	}

//	m_iLens = StageGetCurLens();

#if 0
	if( m_iMode == SITEPR_MODE ){
		CSaveAsDlg dlg(SAVE_AS_SITE_PATTERN_IMG);
		dlg.m_iLens = m_iLens;
		if ( dlg.DoModal() == IDOK ) {
			// 名前の保存
			memset(m_szPatternName, '\0', sizeof(m_szPatternName));
			sprintf(m_szPatternName, "%s", dlg.m_strName);
			// ファイルの保存
			::CopyFile(DESKEW_IMG_TEMP, dlg.m_szFilePath, FALSE);

			COverlayDialog::OnOK();
		}
	}
	else{
		COverlayDialog::OnOK();
	}
#else
	COverlayDialog::OnOK();
#endif
}

// =========================================================================
//
void CPatRecDeskewDlg::OnCancel()
{
	COverlayDialog::OnCancel();
}

// =========================================================================
//
void CPatRecDeskewDlg::OnVerify()
{
	if ( !m_stcModel.IsValid() ) return;

	Event(EV_VERIFY);
}

// =========================================================================
//
void CPatRecDeskewDlg::OnOffsetApply()
{
	STAGE_COORD CurrPos;

	// 2009.07.21 K.Matsuo NSSTAGE -->
//	StageGetPos(&CurrPos);
	NS_StageGetPos(&CurrPos);
	// 2009.07.21 K.Matsuo NSSTAGE <--

	m_OffsetPos.lX = CurrPos.lX - m_CenterPos.lX;
	m_OffsetPos.lY = CurrPos.lY - m_CenterPos.lY;

	TCHAR szBuffer[256];
	_stprintf(szBuffer, "%.3lf", (double)(m_OffsetPos.lX)/MICROMETRE);
	SetDlgItemText(IDC_OFFSET_X, szBuffer);
	_stprintf(szBuffer, "%.3lf", (double)(m_OffsetPos.lY)/MICROMETRE);
	SetDlgItemText(IDC_OFFSET_Y, szBuffer);
}

// =========================================================================
//
LRESULT CPatRecDeskewDlg::OnStageMoveDone(WPARAM wParam, LPARAM lParam)
{
	OverlayDrawRectangle (m_rcOld, FALSE);	// erase the old rect

	return 0L;
}
