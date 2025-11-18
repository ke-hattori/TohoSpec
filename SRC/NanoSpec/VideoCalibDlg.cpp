// VideoCalibDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <math.h>
#include <active.hxx>
#include "System.h"
#include "NanoSpec.h"
#include "Misc.h"
#include "VideoCalibDlg.h"
#include "NanoSpecDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SLOWMOVE_EXTENT 		 1
#define MEDIUMSLOWMOVE_EXTENT	10
#define MEDIUMFASTMOVE_EXTENT  100
#define FASTMOVE_EXTENT		   300

#define NUM_MOVE_SPEEDS 4
enum MOVESPEEDS{ SLOWMOVE, MEDIUMSLOWMOVE, MEDIUMFASTMOVE, FASTMOVE };

#define STAGE_MOVE_DELAY 1000

static long GetMoveExtent(MOVESPEEDS nIndex)
{
	long nMoveSpeed[NUM_MOVE_SPEEDS];	  // four speeds
	nMoveSpeed[SLOWMOVE]	   = SLOWMOVE_EXTENT;
	nMoveSpeed[MEDIUMSLOWMOVE] = MEDIUMSLOWMOVE_EXTENT;
	nMoveSpeed[MEDIUMFASTMOVE] = MEDIUMFASTMOVE_EXTENT;
	nMoveSpeed[FASTMOVE]	   = FASTMOVE_EXTENT;

	nIndex = (MOVESPEEDS)__min(NUM_MOVE_SPEEDS - 1, nIndex);
	nIndex = (MOVESPEEDS)__max(0, nIndex);
	return nMoveSpeed[nIndex];
}


//==========================================================================
//==========================  CVideoCalibDlg  ===============================
//==========================================================================
//


//
//	STATES
//
enum VIDEO_CALIB_STATES
{
	// Initialization. It has no actions. It's constructor
	// will call "Event(EV_NEXT);" only.
	//
	ST_INIT,
	ST_START,
	ST_STORE_FIRST_SITE,
	ST_MOVE_STAGE,
	ST_STORE_SECOND_SITE,
	ST_VIDEO_CALIB_DONE,
};

//
// EVENTS
//
enum VIDEO_CALIB_EVENTS
{
	EV_START,
	EV_LBUTTON_DOWN,	// user start the teaching
	EV_NEXT,		// go to the next state in the state transitation table
};

//
// STATE TRANSITION TABLE
//
// Notes: ST_VIDEO_CALIB_DONE has no next state, Application will end.
//
const Transition VideoCalibTransTable[] =
{
	// OldState,				EventId,			NewState
	//
	{ ST_INIT,					EV_START,			ST_START				},
	{ ST_START, 				EV_LBUTTON_DOWN,	ST_STORE_FIRST_SITE 	},
	{ ST_STORE_FIRST_SITE,		EV_NEXT,			ST_MOVE_STAGE			},
	{ ST_MOVE_STAGE,			EV_LBUTTON_DOWN,	ST_STORE_SECOND_SITE	},
	{ ST_STORE_SECOND_SITE, 	EV_NEXT,			ST_VIDEO_CALIB_DONE 	},
	{ ST_VIDEO_CALIB_DONE,		EV_NEXT,			ST_START				},
	END_OF_TRANSITION_TABLE
};


BEGIN_MESSAGE_MAP(CVideoCalibDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CVideoCalibDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
// State-Processing-Table Definitions
//
#define ENTER_STATE_NULL	{ NULL, NULL }
#pragma warning(disable: 4204)
BEGIN_STATEPROC_MAP(CVideoCalibDlg)
	ENTER_STATE_NULL(ST_INIT),
	ENTER_STATE(ST_START,enter_ST_START),
	ENTER_STATE(ST_STORE_FIRST_SITE, enter_ST_STORE_FIRST_SITE),
	ENTER_STATE(ST_MOVE_STAGE, enter_ST_MOVE_STAGE),
	ENTER_STATE(ST_STORE_SECOND_SITE, enter_ST_STORE_SECOND_SITE),
	ENTER_STATE(ST_VIDEO_CALIB_DONE, enter_ST_VIDEO_CALIB_DONE)
END_STATEPROC_MAP(CVideoCalibDlg)
#pragma warning(default: 4204)

// =========================================================================
//
CVideoCalibDlg::CVideoCalibDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(COverlayDialog::IDD, pParent), cActive(VideoCalibTransTable)
{
	//{{AFX_DATA_INIT(CVideoCalibDlg)
	//}}AFX_DATA_INIT
	m_MoveX = 0;
	m_MoveY = 0;
	m_dXPixelToMicron = 0.0;
	m_dYPixelToMicron = 0.0;
	m_dPosX1 = 0.0;
	m_dPosY1 = 0.0;
	m_dPosX2 = 0.0;
	m_dPosY2 = 0.0;
}

// =========================================================================
//-----------------STATE PROCESSING------------------------------------------
// =========================================================================


// =========================================================================
//
// ST_START
//
// Actions:
//
void CVideoCalibDlg::enter_ST_START(const cEventParams* const /* params */ )
{
	// Kojika 20090604 Add
	CString l_strBuffer;
	// Kojika 20090604 Add End

	// Kojika 20090604 Change
	//m_ctlMessage.SetWindowText("Teach First Site");
	LoadStringML(IDS_TEACH_FIRST_SITE, l_strBuffer, "Teach First Site");
	m_ctlMessage.SetWindowText(l_strBuffer);
	// Kojika 20090604 Change End
}

// =========================================================================
//
// ST_STORE_FIRST_SITE
//
// Actions:
//
void CVideoCalibDlg::enter_ST_STORE_FIRST_SITE(const cEventParams* const evp )
{
	m_dPosX1 = ((cPointEventParams *)evp)->Point.x;
	m_dPosY1 = ((cPointEventParams *)evp)->Point.y;

	Event(EV_NEXT);
}

void CVideoCalibDlg::MoveStage()
{
	// Kojika 20090604 Add
	CString l_strBuffer;
	// Kojika 20090604 Add End

//	m_MoveX = m_MoveY = GetMoveExtent(MEDIUMFASTMOVE_EXTENT);
	m_MoveX = m_MoveY = GetMoveExtent((MOVESPEEDS)m_cbMoveExtent.GetCurSel());

	// Now move the stage to a default distance base on the teaching mode
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	StageMoveRelative(X, -m_MoveX);
	StageMoveRelative(Y, m_MoveY);
	((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	CCursor::BeginWaitCursor();
	Delay(STAGE_MOVE_DELAY);		// wait for stage to settle down
	CCursor::EndWaitCursor();

	// Kojika 20090604 Change
	//m_ctlMessage.SetWindowText("Teach Second Site");
	LoadStringML(IDS_TEACH_SECOND_SITE, l_strBuffer, "Teach Second Site");
	m_ctlMessage.SetWindowText(l_strBuffer);
	// Kojika 20090604 Change End
}

// =========================================================================
//
// ST_MOVE_STAGE
//
// Actions:
//
void CVideoCalibDlg::enter_ST_MOVE_STAGE(const cEventParams* const evp )
{
	MoveStage();

	Event(EV_NEXT);
}

// =========================================================================
//
// ST_STORE_SECOND_SITE
//
// Actions:
//
void CVideoCalibDlg::enter_ST_STORE_SECOND_SITE(const cEventParams* const evp )
{
	m_dPosX2 = ((cPointEventParams *)evp)->Point.x;
	m_dPosY2 = ((cPointEventParams *)evp)->Point.y;

	Event(EV_NEXT);
}

// =========================================================================
//
// ST_VIDEO_CALIB_DONE
//
// Actions:
//
void CVideoCalibDlg::enter_ST_VIDEO_CALIB_DONE(const cEventParams* const evp )
{
//Kojika 20090603 Add
	CString strBuffer, strBuffer2, strBuffer3;
//Kojika 20090603 Add End

	char szBuff[256];

	if( (m_dPosX1 != m_dPosX2) && (m_dPosY1 != m_dPosY2) )
	{
		// calculate the relationship between stage micron and screen pixel
		// store the result in the document, so in OnOK the document will store
		// this result into calibcom.dll for other people to use
		m_dXPixelToMicron = fabs(m_MoveX) / fabs(m_dPosX1 - m_dPosX2);
		m_dYPixelToMicron = fabs(m_MoveY) / fabs(m_dPosY1 - m_dPosY2);

		double temp_x = Convert_RoundForDisplay( m_dXPixelToMicron, VIDEO_CALIB_FACTOR_DISP_DIGITS );
		double temp_y = Convert_RoundForDisplay( m_dYPixelToMicron, VIDEO_CALIB_FACTOR_DISP_DIGITS );
		//Kojika 20090603 Change
		//sprintf(szBuff, "%s%G%s%G%s", " 1 Pixel(X) = ", temp_x, "um\n 1 Pixel(Y) = ", temp_y, "um");
		LoadStringML(IDS_1PIXELX_1PIXELY1, strBuffer, " 1 Pixel(X) = ");
		LoadStringML(IDS_1PIXELX_1PIXELY2, strBuffer2, "um\n 1 Pixel(Y) = ");
		LoadStringML(IDS_1PIXELX_1PIXELY3, strBuffer3, "um");
		sprintf(szBuff, "%s%G%s%G%s", strBuffer, temp_x, strBuffer2, temp_y, strBuffer3);
		//Kojika 20090603 Change End
	}
	else
	{
//		strcpy(szBuff, "Teach is not valid. Please teach again.");
//Kojika 20090525 Change
		LoadStringML(IDS_TEACH_NOT_VALID, strBuffer, "Teach is not valid. Please teach again.");
		strcpy(szBuff, strBuffer);
//Kojika 20090525 Change End
	}

	AfxMessageBox(szBuff);

	Event(EV_NEXT); 	// start another teaching process again
}

// =========================================================================
//
void CVideoCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoCalibDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_VIDEOCALIB_COMBO_MOVEEXTENT, m_cbMoveExtent);
}

/*---------------------------------------------------------------------------
 *	CVideoCalibDlg::OnLButtonDown ()
 *
 *	Description:
 *		Event that causes the rectangle to be updated.
 *
 */
void CVideoCalibDlg::OnLButtonDown(UINT nFlags, CPoint point)
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
 *	CVideoCalibDlg::OnInitDialog ()
 *
 *	Description:
 *		Create Dialog Items.
 *
 */
BOOL CVideoCalibDlg::OnInitDialog()
{
//	  GROUPBOX		  "MOVE EXTENT",IDC_STATIC,300,360,75,30
//	  COMBOBOX		  IDC_COMBO_MOVEEXTENT,305,370,65,90,CBS_DROPDOWNLIST |
//					  WS_VSCROLL | WS_TABSTOP

	//Kojika 20090603 Add
	CString l_strBuffer;
	//Kojika 20090603 Add End

	// Kojika 20090603 Change
	//CreateDialogItem("BUTTON", 		"MOVE EXTENT",	BS_GROUPBOX, 								CRect(CPoint(300,360), CSize(75,30)), IDC_STATIC);
	LoadStringML(IDS_MOVE_EXTENT_BUTTON, l_strBuffer, "MOVE EXTENT");
	CreateDialogItem("BUTTON", 		l_strBuffer,	BS_GROUPBOX, 								CRect(CPoint(300,360), CSize(75,30)), IDC_STATIC);
	// Kojika 20090603 Change End
	CreateDialogItem("COMBOBOX", 	"",				CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, CRect(CPoint(305,370), CSize(65,90)), IDC_VIDEOCALIB_COMBO_MOVEEXTENT);

	// Kojika 20090603 Change
	// SetCaption("VIDEO CALIBRATION SETUP");
	LoadStringML(IDS_VIDEO_CALIBRATION_SETUP_CAPTION, l_strBuffer, "VIDEO CALIBRATION SETUP");
	SetCaption(l_strBuffer);
	// Kojika 20090603 Change End

	COverlayDialog::OnInitDialog();

	Event(EV_START);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*---------------------------------------------------------------------------
 *	CVideoCalibDlg::OnOK ()
 *
 *	Description:
 *
 *
 */
void CVideoCalibDlg::OnOK()
{
	COverlayDialog::OnOK();
}

/*---------------------------------------------------------------------------
 *	CVideoCalibDlg::OnCancel ()
 *
 *	Description:
 *
 *
 */
void CVideoCalibDlg::OnCancel()
{
	COverlayDialog::OnCancel();
}
