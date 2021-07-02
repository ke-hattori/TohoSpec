// DeskewSeq.cpp : implementation file
//

#include "stdafx.h"
#include "System.h"
#include "nanospec.h"
#include "MainFrm.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\PatRec.hxx"
#include "..\\..\\INC\\Deskew.hxx"
#include "..\\..\\INC\\NSStage.hxx"
#include "ChifTransiSeq.h"
#include "GropeObject.h"
#include "DeskewSeq.h"
#include "DeskewSeqWnd.h"
#include "NanoSpecDoc.h"

#define STAGE_MOVE_DELAY		(1000)

#define OVERLAY_CENTER_X (644 / 2)	// COverlayDialog::m_rcOverlay.Width() / 2
#define OVERLAY_CENTER_Y (483 / 2)	// COverlayDialog::m_rcOverlay.Hight() / 2

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// レンズオフセット対応
//#define STAGEMOVE_ORIGINAL		// Stage.dllのオリジナル座標で移動


/////////////////////////////////////////////////////////////////////////////
// CDeskewSeq

IMPLEMENT_DYNCREATE(CDeskewSeq, CWinThread)

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CDeskewSeq, CWinThread)
	//{{AFX_MSG_MAP(CDeskewSeq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeskewSeq message handlers

/////////////////////////////////////////////////////////////////////////////
// CDeskewAutoSeq

IMPLEMENT_DYNCREATE(CDeskewAutoSeq, CDeskewSeq)

/////////////////////////////////////////////////////////////////////////////
//
BOOL CDeskewAutoSeq::InitInstance()
{
	// delete this します
	m_bAutoDelete = TRUE;

	m_bShutDown = FALSE;

	DeskewClear();

	if ( !m_pStageProgInfoHdr->DeskewSite[0].bDefined ||
		 !m_pStageProgInfoHdr->DeskewSite[1].bDefined ) {
		// レシピ情報取得失敗
//		::MessageBox(NULL, "レシピ情報Deskewサイト未定義\n測定を中止します", "CDeskewAutoSeq", MB_ICONEXCLAMATION);
		// デスキュー処理失敗－測定キャンセル
		m_iEvent = EV_DSKW_AUTO_CANCEL;
		return FALSE;
	}

	::ZeroMemory(DeskewResult, sizeof(DeskewResult));
	::ZeroMemory(MatchLoc, sizeof(MatchLoc));

	ConfigFile_GetNanoSpecIni(m_SrVideoCalib, CONFIG_FILE_SR_VIDEO_CALIBRATION);
	ConfigFile_GetNanoSpecIni(&m_patternRec, CONFIG_FILE_SR_PATTERN_REC);
	// 2013.01.09 bagus -->
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
	// 2013.01.09 bagus <--

	((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(m_pStageProgInfoHdr->iLens);
	((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
int CDeskewAutoSeq::ExitInstance()
{
	TRACE(_T("CDeskewAutoSeq::ExitInstance()\n"));

	m_pChiefTransiDeskew->TransiEvent(m_iEvent, 0L);

	return CDeskewSeq::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CDeskewAutoSeq, CDeskewSeq)
	//{{AFX_MSG_MAP(CDeskewAutoSeq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeskewAutoSeq message handlers

/////////////////////////////////////////////////////////////////////////////
//
int CDeskewAutoSeq::Run()
{
	BOOL bAutoFocus;

	// get the current deskew site and go there

// 2009.06.11 K.Matsuo ポーズ対応 -->
//	for ( int iSite = 0; iSite < 2; iSite++ ) {
	int iSite = 0;
	while ( iSite < 2 ) {
		if ( m_bShutDown )
			break;
		if ( ST_SEQ_PAUSE == ((CChiefTransiSeq*) ((CChiefView*) m_pChiefView)->m_pcChiefTransiSeq)->GetCurrentState() ) {
			::Sleep(1000);
			continue;
		}
// 2009.06.11 K.Matsuo ポーズ対応 <--

		STAGE_COORD coord =	m_pStageProgInfoHdr->DeskewSite[iSite].Loc;
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		NS_StageMoveAbsolute(&coord);
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
		bAutoFocus = NS_StageDoAutoFocus();
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
		if ( !bAutoFocus )
			break;

		if ( m_bShutDown )
			break;
		Delay(STAGE_MOVE_DELAY);
		if ( m_bShutDown )
			break;
		DoPatRecMatchWithVerify(iSite, &DeskewResult[iSite], &MatchLoc[iSite]);
		if ( DeskewResult[iSite].Score < m_patternRec.dLowestMatchScore ) // pat rec is NG
			break;
// 2009.06.11 K.Matsuo ポーズ対応 -->
		iSite++;
// 2009.06.11 K.Matsuo ポーズ対応 <--
	}


	// デスキュー処理終了
	if ( m_bShutDown ) {
		// キャンセル終了
		m_iEvent = EV_DSKW_AUTO_CANCEL;
	}
	else {
		if ( DeskewResult[0].Score >= m_patternRec.dLowestMatchScore &&
			 DeskewResult[1].Score >= m_patternRec.dLowestMatchScore ) // pat rec is OK
		{
			// デスキュー処理正常
			m_iEvent = EV_DSKW_AUTO_OK;

			// Deskew計算は、マシン座標で行う
			STAGE_COORD DeskewSiteLoc1, DeskewSiteLoc2, MatchLoc1, MatchLoc2;
			DeskewSiteLoc1 = m_pStageProgInfoHdr->DeskewSite[0].Loc;
			DeskewSiteLoc2 = m_pStageProgInfoHdr->DeskewSite[1].Loc;
			MatchLoc1 = MatchLoc[0];
			MatchLoc2 = MatchLoc[1];

			NS_ConvertToXyAxisCoord(&DeskewSiteLoc1);
			NS_ConvertToXyAxisCoord(&DeskewSiteLoc2);
			NS_ConvertToXyAxisCoord(&MatchLoc1);
			NS_ConvertToXyAxisCoord(&MatchLoc2);

			DeskewSet(DeskewSiteLoc1,
					  DeskewSiteLoc2,
					  MatchLoc1,
					  MatchLoc2);
		}
		else
		{
			switch ( m_pStageProgInfoHdr->nDeskewFailOption )
			{
			case DESKEW_FAIL_OPTION_PROCEED_MEAS:								// 強制測定（そのまま測定開始）
				m_iEvent = EV_DSKW_AUTO_PROCEED;
				break;
			case DESKEW_FAIL_OPTION_CANCEL_RECIPE:								// 測定は行わず，レシピ終了
				m_iEvent = EV_DSKW_AUTO_CANCEL;
				break;
			case DESKEW_FAIL_OPTION_MANUAL_DESKEW:								// Manual Deskewに移行する
				m_iEvent = EV_DSKW_AUTO_MANU;
				break;
// 2009.04.10 K.Matsuo Add -->
			case DESKEW_FAIL_OPTION_PROCEED_MEAS_PMA:							// 強制測定（そのまま位置補正付き測定（PMA使用））
				m_iEvent = EV_DSKW_AUTO_PROCEED_PMA;
				break;
// 2009.04.10 K.Matsuo Add <--
			}
		}
	}

	::PostQuitMessage(0);

	return CDeskewSeq::Run();
}

/////////////////////////////////////////////////////////////////////////////
//
void CDeskewAutoSeq::DoPatRecMatchWithVerify( int Site, 		// first or second deskew site
											  DESKEW_RESULT* pDeskewResult,
											  STAGE_COORD* pMatchLoc)
{
	// perform a pattern rec match, if success, move the match location
	// to the center of the overlay and do another match to verify.

	// Score = Model coverage x (1 - (Fit error weighting factor x Normalized Fit Error))
	// Target Score = Target coverage x (1 - (Fit error weighting factor x Normalized Fit Error))

	// The model coverage and target coverage are defined as follows:
	//
	// Model coverage.
	// The model coverage is the percentage of the total length of the model's active edges found in the occurrence. 100% indicates that for each of the model's active edges, a corresponding edge was found in the occurrence.
	// Target coverage.
	// The target coverage is the percentage of the total length of the model's active edges found in the occurrence, divided by the total length of edges present within the occurrence's bounding box.
	// Thus, a target coverage score of 100% means that no extra edges were found. Lower scores indicate that features or edges found in the target (result occurrence) are not present in the model.

	const int PatRecTargetScore= 40.0;

	Delay(STAGE_MOVE_DELAY);

	// should block after restore is done
	char szBuff[MAX_PATH];
	sprintf(szBuff, "%s%s.D%d", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName, Site + 1);
	PatRecRestoreModel(szBuff);


	// keep track of the site of the highest score
	DESKEW_RESULT bestResult;
// 2009.06.12 K.Matsuo Bug Fix -->
	STAGE_COORD	bestMatchLoc;
// 2009.06.12 K.Matsuo Bug Fix <--

	// do the first match
	DoPatternRecMatch(pDeskewResult, pMatchLoc);

	// update the best result so far
	bestResult = *pDeskewResult;
// 2009.06.12 K.Matsuo Bug Fix -->
	bestMatchLoc = *pMatchLoc;
// 2009.06.12 K.Matsuo Bug Fix <--

	// if the first score is less than the match score and pat rec groping is enabled, do groping
	if( pDeskewResult->Score < m_patternRec.dStopGropingScore && m_patternRec.nNumLayers > 0)
	{
		const int StepGropeLayers[] = { 2, 3, 4 };
		long DeltaX = long( OVERLAY_CENTER_X * m_SrVideoCalib[m_pStageProgInfoHdr->iLens].dXPixelToMicron * 2 / StepGropeLayers[0] );
		long DeltaY = long( OVERLAY_CENTER_Y * m_SrVideoCalib[m_pStageProgInfoHdr->iLens].dYPixelToMicron * 2 / StepGropeLayers[0] );

		short dX, dY;	 // stage translation based on retry site number

		// do groping retry if feature not found in the previous try
		//
		CGropeObject GropObj(m_patternRec.nNumLayers);	 // object which will tell where to grope

		STAGE_COORD Position;
		const int nMaxRetries = 2;
		int nRetries = nMaxRetries;
		while ( pDeskewResult->Score < m_patternRec.dStopGropingScore && !GropObj.Finished() ) {
			if ( m_bShutDown )
				return;

			// get the current stage position
			NS_StageGetPos(&Position);

			while ( PatRecTargetScore <= pDeskewResult->TargetScore && nRetries-- > 0 ) {
				if ( m_bShutDown )
					return;

				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				NS_StageMoveAbsolute(pMatchLoc);
				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				Delay(STAGE_MOVE_DELAY);

				// retry the match at this new site
				DoPatternRecMatch(pDeskewResult, pMatchLoc);
				if ( pDeskewResult->Score > bestResult.Score ) {
					// update the best score so far
					bestResult = *pDeskewResult;
// 2009.06.12 K.Matsuo Bug Fix -->
					bestMatchLoc = *pMatchLoc;
// 2009.06.12 K.Matsuo Bug Fix <--
				}
				if ( pDeskewResult->Score >= m_patternRec.dStopGropingScore ) goto SUCCESS;
			}
			nRetries = nMaxRetries;

			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			NS_StageMoveAbsolute(&Position);
			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

			GropObj.GetNextMove( &dX, &dY );   // get the next movement amount and direction

			// ask stage to move to the new site
			STAGE_COORD stageCoord;
			stageCoord.lX = dX * DeltaX;
			stageCoord.lY = dY * DeltaY;
			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			StageMoveRelative(&stageCoord);
			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			Delay(STAGE_MOVE_DELAY);		   // wait long enough for stage to settle down

			// retry the match at this new site
			DoPatternRecMatch(pDeskewResult, pMatchLoc);
			if ( pDeskewResult->Score > bestResult.Score ) {
				// update the best score so far
				bestResult = *pDeskewResult;
// 2009.06.12 K.Matsuo Bug Fix -->
				bestMatchLoc = *pMatchLoc;
// 2009.06.12 K.Matsuo Bug Fix <--
			}
		}
	}

SUCCESS:

	// update the score and match location with the best or the first score > 55
	*pDeskewResult = bestResult;
// 2009.06.12 K.Matsuo Bug Fix -->
	*pMatchLoc = bestMatchLoc;
// 2009.06.12 K.Matsuo Bug Fix <--

	// if pattern rec is OK, we want to move the matched location to the center
	// and do another match to minimize the error caused by video calibration factor.
	if( pDeskewResult->Score >= m_patternRec.dLowestMatchScore ) // pat rec is OK
	{
		// move to the last matched location, delay to make sure stage is settled
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		NS_StageMoveAbsolute(pMatchLoc);
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		Delay(STAGE_MOVE_DELAY);

		// do the second match
		DoPatternRecMatch(pDeskewResult, pMatchLoc);
	}
}

BOOL CDeskewAutoSeq::DoPatternRecMatch(DESKEW_RESULT* pDeskewResult, STAGE_COORD* pMatchLoc)
{
	PatRecMatch(pDeskewResult);

	// map center from screen (0, 0) to screen(m_OverlayCenter.x, m_OverlayCenter.y)
	pDeskewResult->XPos -= OVERLAY_CENTER_X;
	pDeskewResult->YPos -= OVERLAY_CENTER_Y;

	pDeskewResult->YPos *= -1;							// now invert the y axis

	PixelToMicron(pDeskewResult->XPos, pDeskewResult->YPos, pMatchLoc);

// 2013.01.09 bagus -->
//	NS_ConvertToStageGetPosCoord(pMatchLoc);
// 2013.01.09 bagus <--

	return TRUE;
}

// 2013.01.17 bagus rotate xy view -->
// ==========================================================================
//
long CDeskewAutoSeq::RotateXyAxisDir(long lDir)
{
	long lRet;

	lRet = lDir;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
		break;
	case 90:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_TOP:
			lRet = DIR_RIGHT;
			break;
		case DIR_LEFT:
			lRet = DIR_TOP;
			break;
		case DIR_BOTTOM:
			lRet = DIR_LEFT;
			break;
		default:
			break;
		}
		break;
	case 180:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_LEFT;
			break;
		case DIR_TOP:
			lRet = DIR_BOTTOM;
			break;
		case DIR_LEFT:
			lRet = DIR_RIGHT;
			break;
		case DIR_BOTTOM:
			lRet = DIR_TOP;
			break;
		default:
			break;
		}
		break;
	case 270:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_TOP;
			break;
		case DIR_TOP:
			lRet = DIR_LEFT;
			break;
		case DIR_LEFT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_BOTTOM:
			lRet = DIR_RIGHT;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return lRet;
}
// 2013.01.17 bagus rotate xy view <--

// 2013.01.09 bagus -->
// クライアントステージ座標（UI画面用）に変換
// カメラは、StageConfigで設定されたView方向で設置されている
void CDeskewAutoSeq::ToClientXyStageCoord(STAGE_COORD* lpPoint)
{
// 2013.01.17 bagus rotate xy view -->
	long lDirX = RotateXyAxisDir(m_StageConfig.Dir.X);
	long lDirY = RotateXyAxisDir(m_StageConfig.Dir.Y);
// 2013.01.17 bagus rotate xy view <--

	// X: RIGHT
	if ( lDirX == DIR_RIGHT ) {
		switch ( lDirY ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: LEFT
	if ( lDirX == DIR_LEFT ) {
		lpPoint->lX *= -1;
		switch ( lDirY ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: TOP
	if ( lDirX == DIR_TOP ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lX = ptTemp.lY;
		switch ( lDirY ) {
		case DIR_RIGHT:
			lpPoint->lY = ptTemp.lX;
			break;
		case DIR_LEFT:
			lpPoint->lY = ptTemp.lX * -1;
			break;
		default:
			break;
		}
	}

	// X: BOTTOM
	if ( lDirX == DIR_BOTTOM ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lX = ptTemp.lY * -1;
		switch ( lDirY ) {
		case DIR_RIGHT:
			lpPoint->lY = ptTemp.lX;
			break;
		case DIR_LEFT:
			lpPoint->lY = ptTemp.lX * -1;
			break;
		default:
			break;
		}
	}
}
// 2013.01.09 bagus <--

/*---------------------------------------------------------------------------
 *	 PixelToMicron: convert pixels relative to overlay center to stage coordinate
 *
 *	 Description:
 *		Convert the pixels in doubles to stage microns.
 */

void CDeskewAutoSeq::PixelToMicron(double xPos, double yPos, STAGE_COORD* pMatchLoc)
{
// 2013.01.09 bagus -->
// 	// get the current coordinate of the chuck
// 	STAGE_COORD MatchCtr;
// 	StageGetPos(&MatchCtr);
//
// 	// calculate the absolute position of each point on stage
// 	double x = xPos * m_SrVideoCalib[m_pStageProgInfoHdr->iLens].dXPixelToMicron;
// 	double y = yPos * m_SrVideoCalib[m_pStageProgInfoHdr->iLens].dYPixelToMicron;
// 	pMatchLoc->lX = long( x >= 0 ? x + 0.5 : x - 0.5 ) + MatchCtr.lX;
// 	pMatchLoc->lY = long( y >= 0 ? y + 0.5 : y - 0.5 ) + MatchCtr.lY;

	// get the current coordinate of the chuck
	STAGE_COORD MatchCtr;
	NS_StageGetPos(&MatchCtr);

	// calculate the absolute position of each point on stage
	double x = xPos * m_SrVideoCalib[m_pStageProgInfoHdr->iLens].dXPixelToMicron;
	double y = yPos * m_SrVideoCalib[m_pStageProgInfoHdr->iLens].dYPixelToMicron;

	STAGE_COORD XyViewNsClientCoord;

	XyViewNsClientCoord.lX = long( x >= 0 ? x + 0.5 : x - 0.5 );
	XyViewNsClientCoord.lY = long( y >= 0 ? y + 0.5 : y - 0.5 );

	ToClientXyStageCoord(&XyViewNsClientCoord);

	pMatchLoc->lX = XyViewNsClientCoord.lX + MatchCtr.lX;
	pMatchLoc->lY = XyViewNsClientCoord.lY + MatchCtr.lY;
// 2013.01.09 bagus <--
}

/////////////////////////////////////////////////////////////////////////////
//
void CDeskewAutoSeq::ShutDown()
{
	m_bShutDown = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CDeskewManualSeq

IMPLEMENT_DYNCREATE(CDeskewManualSeq, CDeskewSeq)

/////////////////////////////////////////////////////////////////////////////
//
BOOL CDeskewManualSeq::InitInstance()
{
	// delete this します
	m_bAutoDelete = TRUE;

	m_bShutDown = FALSE;

	DeskewClear();

	if ( !m_pStageProgInfoHdr->DeskewSiteManual[0].bDefined ||
		 !m_pStageProgInfoHdr->DeskewSiteManual[1].bDefined ) {
		// レシピ情報取得失敗
//		::MessageBox(NULL, "レシピ情報Deskewサイト未定義\n測定を中止します", "DeskewManualSeq", MB_ICONEXCLAMATION);
		// デスキュー処理失敗－測定キャンセル
		m_iEvent = EV_DSKW_MANU_CANCEL;
		return FALSE;
	}

	::ZeroMemory(MatchLoc, sizeof(MatchLoc));

	m_pMainWnd = new CDeskewManualSeqWnd();
	m_pMainWnd->CreateEx(0, AfxRegisterWndClass(0), DESKEW_MANUAL_SEQ_WINDOW_NAME, WS_OVERLAPPEDWINDOW, CRect(0, 0, 0, 0), NULL, 0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
int CDeskewManualSeq::ExitInstance()
{
	TRACE(_T("CDeskewManualSeq::ExitInstance()\n"));

	m_pChiefTransiDeskew->TransiEvent(m_iEvent, 0L);

	return CDeskewSeq::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CDeskewManualSeq, CDeskewSeq)
	//{{AFX_MSG_MAP(CDeskewManualSeq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeskewManualSeq message handlers

/////////////////////////////////////////////////////////////////////////////
//
void CDeskewManualSeq::ShutDown()
{
	m_bShutDown = TRUE;
//	m_pMainWnd->PostMessage(WM_DESKEWSEQ_DESKEWMANUAL_CANCEL);
}

