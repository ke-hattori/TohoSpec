#if !defined(AFX_ALARMHISTORYOPERATIONVIEW_H__04900304_423B_45EB_9032_FC6127ED0BCA__INCLUDED_)
#define AFX_ALARMHISTORYOPERATIONVIEW_H__04900304_423B_45EB_9032_FC6127ED0BCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmHistoryOperationView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmHistoryOperationView フォーム ビュー								//
//																			//
/////////////////////////////////////////////////////////////////////////////

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

/////////////////////////////////////////////////////////////////////////////
//
class CAlarmHistoryOperationView : public CNanoUI
{
protected:
	CAlarmHistoryOperationView();
	virtual ~CAlarmHistoryOperationView();
	DECLARE_DYNCREATE(CAlarmHistoryOperationView)

protected:
	//{{AFX_MSG(CAlarmHistoryOperationView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//{{AFX_DATA(CAlarmHistoryOperationView)
	enum { IDD = IDD_ALARM_HISTORY_OPERATION_FORM };
	//}}AFX_DATA
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ALARMHISTORYOPERATIONVIEW_H__04900304_423B_45EB_9032_FC6127ED0BCA__INCLUDED_)
