// SitePtSeqWnd.cpp : implementation file
//

#include "stdafx.h"
#include "nanospec.h"
#include "MainFrm.h"
#include "ChiefView.h"
#include "SitePtSeq.h"
#include "SitePtSeqWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CSitePtManualSeqWnd, CWnd)
	//{{AFX_MSG_MAP(CSitePtManualSeqWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SITEPTSEQ_SITEPTMANUAL_OK, OnSitePtManualOkEnd)
	ON_MESSAGE(WM_SITEPTSEQ_SITEPTMANUAL_CANCEL, OnSitePtManualCancelEnd)
	ON_MESSAGE(WM_SITEPTSEQ_SITEPTMANUAL_NONE, OnSitePtManualNoneEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSitePtManualSeqWnd message handlers

//---------------------------------------------------------------------------
// OnCreate
int CSitePtManualSeqWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// SitePtSiteManual
	((CMainFrame*) (((CChiefView*) ((CSitePtSeq*)AfxGetThread())->m_pChiefView)->m_pcMainFrame))->PostMessage(WM_SITEPTSEQ_SITEPTMANUAL_START, 0L, 0L);

	return 0;
}

//---------------------------------------------------------------------------
// PostNcDestroy
void CSitePtManualSeqWnd::PostNcDestroy()
{
	delete this;
}

//---------------------------------------------------------------------------
// OnSitePtManualOkEnd
LRESULT CSitePtManualSeqWnd::OnSitePtManualOkEnd(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CSitePtManualSeqWnd::OnSitePtManualOkEnd(WPARAM wParam, LPARAM lParam)\n"));

	((CSitePtSeq*)AfxGetThread())->m_iEvent = EV_SR1P_SITEPR_MANU_OK;

	/* SitePt Manual 終わり */
	PostMessage(WM_CLOSE, 0L, 0L);

	return 0L;
}

//---------------------------------------------------------------------------
// OnSitePtManualCancelEnd
LRESULT CSitePtManualSeqWnd::OnSitePtManualCancelEnd(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CSitePtManualSeqWnd::OnSitePtManualCancelEnd(WPARAM wParam, LPARAM lParam)\n"));

	((CSitePtSeq*)AfxGetThread())->m_iEvent = EV_SR1P_SITEPR_MANU_CANCEL;

	/* SitePt Manual 終わり */
	PostMessage(WM_CLOSE, 0L, 0L);

	return 0L;
}

//---------------------------------------------------------------------------
// OnSitePtManualNoneEnd
LRESULT CSitePtManualSeqWnd::OnSitePtManualNoneEnd(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CSitePtManualSeqWnd::OnSitePtManualNoneEnd(WPARAM wParam, LPARAM lParam)\n"));

	((CSitePtSeq*)AfxGetThread())->m_iEvent = EV_SR1P_SITEPR_MANU_CANCEL;

// myan 一応キャンセルを送るようにするが 実質の確認が必要（なんのボタン(Event)に関連しているのか 要確認）
//	((CSitePtSeq*)AfxGetThread())->m_iEvent = EV_SR1P_SITEPR_MANU_NONE;

	// SitePt Manual 終わり
	PostMessage(WM_CLOSE, 0L, 0L);

	return 0L;
}

