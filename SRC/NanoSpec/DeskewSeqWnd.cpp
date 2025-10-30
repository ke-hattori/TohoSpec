// DeskewSeqWnd.cpp : implementation file
//

#include "stdafx.h"
#include "nanospec.h"
#include "MainFrm.h"
#include "ChiefView.h"
#include "ChifTransiSeq.h"
#include "..\\..\\INC\\Deskew.hxx"
#include "..\\..\\INC\\NSStage.hxx"
#include "DeskewSeq.h"
#include "DeskewSeqWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CDeskewManualSeqWnd, CWnd)
	//{{AFX_MSG_MAP(CDeskewManualSeqWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DESKEWSEQ_DESKEWMANUAL_SITE1_OK, OnDeskewManualSite1OkEnd)
	ON_MESSAGE(WM_DESKEWSEQ_DESKEWMANUAL_SITE2_OK, OnDeskewManualSite2OkEnd)
	ON_MESSAGE(WM_DESKEWSEQ_DESKEWMANUAL_CANCEL, OnDeskewManualCancelEnd)
	ON_MESSAGE(WM_DESKEWSEQ_DESKEWMANUAL_NONE, OnDeskewManualNoneEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeskewManualSeqWnd message handlers

//---------------------------------------------------------------------------
// OnCreate
int CDeskewManualSeqWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

// 2009.06.11 K.Matsuo ポーズ対応 -->
	while ( 1 ) {
		if ( ((CDeskewSeq*)AfxGetThread())->m_bShutDown )
			break;
		if ( ST_SEQ_PAUSE == ((CChiefTransiSeq*) ((CChiefView*) ((CDeskewSeq*)AfxGetThread())->m_pChiefView)->m_pcChiefTransiSeq)->GetCurrentState() ) {
			::Sleep(1000);
			continue;
		}
		else
			break;
	}

	if ( ((CDeskewSeq*)AfxGetThread())->m_bShutDown ) {
		PostMessage(WM_DESKEWSEQ_DESKEWMANUAL_CANCEL, 0L, 0L);
	}
	else {
		// DeskewSiteManual 1
		((CMainFrame*) (((CChiefView*) ((CDeskewSeq*)AfxGetThread())->m_pChiefView)->m_pcMainFrame))->PostMessage(WM_DESKEWSEQ_DESKEWMANUAL_START, 0L, 0L);
	}
// 2009.06.11 K.Matsuo ポーズ対応 <--

	return 0;
}

//---------------------------------------------------------------------------
// PostNcDestroy
void CDeskewManualSeqWnd::PostNcDestroy()
{
	delete this;
}

//---------------------------------------------------------------------------
// OnDeskewManualSite1OkEnd
LRESULT CDeskewManualSeqWnd::OnDeskewManualSite1OkEnd(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CDeskewManualSeqWnd::OnDeskewManualSite1OkEnd(WPARAM wParam, LPARAM lParam)\n"));

	((CDeskewSeq*)AfxGetThread())->MatchLoc[0].lX = (long)wParam;
	((CDeskewSeq*)AfxGetThread())->MatchLoc[0].lY = (long)lParam;

// 2009.06.11 K.Matsuo ポーズ対応 -->
	while ( 1 ) {
		if ( ((CDeskewSeq*)AfxGetThread())->m_bShutDown )
			break;
		if ( ST_SEQ_PAUSE == ((CChiefTransiSeq*) ((CChiefView*) ((CDeskewSeq*)AfxGetThread())->m_pChiefView)->m_pcChiefTransiSeq)->GetCurrentState() ) {
			::Sleep(1000);
			continue;
		}
		else
			break;
	}

	if ( ((CDeskewSeq*)AfxGetThread())->m_bShutDown ) {
		PostMessage(WM_DESKEWSEQ_DESKEWMANUAL_CANCEL, 0L, 0L);
	}
	else {
		// DeskewSiteManual 2
		((CMainFrame*) (((CChiefView*) ((CDeskewSeq*)AfxGetThread())->m_pChiefView)->m_pcMainFrame))->PostMessage(WM_DESKEWSEQ_DESKEWMANUAL_START, 1L, 0L);
	}
// 2009.06.11 K.Matsuo ポーズ対応 <--

	return 0L;
}

//---------------------------------------------------------------------------
// OnDeskewManualSite2OkEnd
LRESULT CDeskewManualSeqWnd::OnDeskewManualSite2OkEnd(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CDeskewManualSeqWnd::OnDeskewManualSite2OkEnd(WPARAM wParam, LPARAM lParam)\n"));

	((CDeskewSeq*)AfxGetThread())->MatchLoc[1].lX = (long)wParam;
	((CDeskewSeq*)AfxGetThread())->MatchLoc[1].lY = (long)lParam;

	((CDeskewSeq*)AfxGetThread())->m_iEvent = EV_DSKW_MANU_OK;

	// Deskew計算は、マシン座標で行う
	STAGE_COORD DeskewSiteLoc1, DeskewSiteLoc2, MatchLoc1, MatchLoc2;
	DeskewSiteLoc1 = ((CDeskewSeq*)AfxGetThread())->m_pStageProgInfoHdr->DeskewSiteManual[0].Loc;
	DeskewSiteLoc2 = ((CDeskewSeq*)AfxGetThread())->m_pStageProgInfoHdr->DeskewSiteManual[1].Loc;
	MatchLoc1 = ((CDeskewSeq*)AfxGetThread())->MatchLoc[0];
	MatchLoc2 = ((CDeskewSeq*)AfxGetThread())->MatchLoc[1];

	NS_ConvertToXyAxisCoord(&DeskewSiteLoc1);
	NS_ConvertToXyAxisCoord(&DeskewSiteLoc2);
	NS_ConvertToXyAxisCoord(&MatchLoc1);
	NS_ConvertToXyAxisCoord(&MatchLoc2);

	DeskewSet(DeskewSiteLoc1,
			  DeskewSiteLoc2,
			  MatchLoc1,
			  MatchLoc2);

	// Deskew Manual 終わり
	PostMessage(WM_CLOSE, 0L, 0L);

	return 0L;
}

//---------------------------------------------------------------------------
// OnDeskewManualCancelEnd
LRESULT CDeskewManualSeqWnd::OnDeskewManualCancelEnd(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CDeskewManualSeqWnd::OnDeskewManualCancelEnd(WPARAM wParam, LPARAM lParam)\n"));

	((CDeskewSeq*)AfxGetThread())->m_iEvent = EV_DSKW_MANU_CANCEL;

	// Deskew Manual 終わり
	PostMessage(WM_CLOSE, 0L, 0L);

	return 0L;
}

//---------------------------------------------------------------------------
// OnDeskewManualNoneEnd
LRESULT CDeskewManualSeqWnd::OnDeskewManualNoneEnd(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CDeskewManualSeqWnd::OnDeskewManualNoneEnd(WPARAM wParam, LPARAM lParam)\n"));

	((CDeskewSeq*)AfxGetThread())->m_iEvent = EV_DSKW_MANU_NONE;

	// Deskew Manual 終わり
	PostMessage(WM_CLOSE, 0L, 0L);

	return 0L;
}

