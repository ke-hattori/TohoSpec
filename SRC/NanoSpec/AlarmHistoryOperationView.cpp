// AlarmHistoryOperationView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "NanoSpec.h"
#include "AlarmHistoryOperationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmHistoryOperationView                                               //
//                                                                          //
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CAlarmHistoryOperationView, CNanoUI)

/////////////////////////////////////////////////////////////////////////////
//
CAlarmHistoryOperationView::CAlarmHistoryOperationView()
	: CNanoUI(CAlarmHistoryOperationView::IDD)
{
	//{{AFX_DATA_INIT(CAlarmHistoryOperationView)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
CAlarmHistoryOperationView::~CAlarmHistoryOperationView()
{
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CAlarmHistoryOperationView, CNanoUI)
	//{{AFX_MSG_MAP(CAlarmHistoryOperationView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
