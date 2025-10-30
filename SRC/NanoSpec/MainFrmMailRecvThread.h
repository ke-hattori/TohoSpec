#pragma once

#include "../../INC/PifComm.hxx"
#include "Mailslot.h"

class CMainFrame;
class CNanoSpecDoc;
class CStatusView;
class CInterlockDlg;

class CMainFrmMailRecvThread : public CWinThread
{
	DECLARE_DYNCREATE(CMainFrmMailRecvThread)
protected:
	CMainFrmMailRecvThread();
public:
	virtual ~CMainFrmMailRecvThread();
	void ShutDown();
	void SetMainFrame(CMainFrame* pMainFrame){m_pMainFrame = pMainFrame;}
	void SetNanoSpecDoc(CNanoSpecDoc* pDoc){m_pDoc = pDoc;}
	void SetInterlockDlg(CInterlockDlg* pInterlockDlg){m_pInterlockDlg = pInterlockDlg;}

	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();

protected:
	DECLARE_MESSAGE_MAP()

// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
	void EventProcessP105();
	void EventProcessP106();
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--
	void EventProcessP301();
	void EventProcessP302();
	void EventProcessP303();
	void EventProcessP306();
	void EventProcessP308();
	void EventProcessP310();

// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
	BOOL CallLock(BOOL order);
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--

private:
	CMailslot m_mailslot;
	BOOL m_bShutDown;

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
	CInterlockDlg* m_pInterlockDlg;

	EQMONITORSET m_eqMonitor;
	EQMONITORSET m_eqMonitorOld;
};
