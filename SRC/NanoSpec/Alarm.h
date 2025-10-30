#if !defined(AFX_ALARM_H__45E45540_93B7_4239_8DE7_397850E363AF__INCLUDED_)
#define AFX_ALARM_H__45E45540_93B7_4239_8DE7_397850E363AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Alarm.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// Define                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
#ifndef _MASTER_
#define _EXTERN	extern
#else
#define	_EXTERN
#endif	// _MASTER_

#define ALARM_LIST_MAX			2000

#define ALARM_LOG_NAME			"Alarm"
#define ALARM_LOG_MAX			30
#define ALARM_LOG_SIZE			300000
#define	SUBITEM_LEN				100
#define	ALARM_TIME_LEN			14				// 20080805154030

#define ALARM_FILE_NAME			"AlarmList.ini"
// K.Matsuo 2009.08.03 言語別アラームリスト読込み -->
#define ALARM_FILE_E_NAME		"AlarmList_E.ini"
#define ALARM_FILE_J_NAME		"AlarmList_J.ini"
// K.Matsuo 2009.08.03 言語別アラームリスト読込み <--

#define ALARM_BUFF_LEN			20
#define ALARM_BUFF_MAX			100

/////////////////////////////////////////////////////////////////////////////
// Structure                                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
typedef struct{
	int Id;
	int Alcd;
	char Level;
	char Text[ALARM_LOG_TEXT_LEN + 1];
}ALARM_LIST;

typedef struct{
	int Status;
	int Old;
	int Up;
	int Down;
	char SubItem[SUBITEM_LEN + 1];
	char OccurrenceTime[ALARM_TIME_LEN + 1];
	char ReleaseTime[ALARM_TIME_LEN + 1];
}ALARM_STATUS;

typedef struct{
	int Status;
	char SubItem[SUBITEM_LEN + 1];
}ALARM_SET;

/////////////////////////////////////////////////////////////////////////////
// Class Define                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
class CMainFrame;
class CNanoSpecDoc;
class CStatusView;

/////////////////////////////////////////////////////////////////////////////
// CAlarm スレッド                                                         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
class CAlarm : public CWinThread
{
	DECLARE_DYNCREATE(CAlarm)
protected:
	CAlarm();			// 動的生成に使用されるプロテクト コンストラクタ

// アトリビュート
public:

// オペレーション
public:
	///// Variable //////////////////////////////////////////////////////////
	BOOL m_Run;

	CLogFile* m_pAlarmLogFile;
	ALARM_LIST m_AlarmList[ALARM_LIST_MAX];
	int m_AlarmMax;
	ALARM_STATUS* m_AlarmStatus;
	ALARM_SET* m_AlarmSet;

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
	CStatusView* m_pStatusView;

	///// Function //////////////////////////////////////////////////////////
	virtual ~CAlarm();
	void Quit(void){m_Run = FALSE;};
	void AlarmListLoading();
	int GetAlarmMax(){return m_AlarmMax;};
	void AlarmProcessing(void);
	void AlarmOccurrence(int Index);
	void AlarmRelease(int Index);
	void AlarmSet(int ID, char* subitem = "\0");
	void AlarmReset(int ID);
	void AlarmAllReset();
	int AlarmSearchIndex(int Id);
	void DeleteAlarmList(int Index);
	void AlarmCheck();
	int AlarmStatusCheck(int Id);

	void SetMainFrame(CMainFrame* pMainFrame){m_pMainFrame = pMainFrame;};
	void SetNanoSpecDoc(CNanoSpecDoc* pDoc){m_pDoc = pDoc;};
	void SetStatusView(CStatusView* pStatusView){m_pStatusView = pStatusView;};

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAlarm)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAlarm)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ALARM_H__45E45540_93B7_4239_8DE7_397850E363AF__INCLUDED_)
