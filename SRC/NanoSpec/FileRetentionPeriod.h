#if !defined(AFX_FILERETENTIONPERIOD_H__E7306D72_A980_40E9_A306_C20E7080BCD4__INCLUDED_)
#define AFX_FILERETENTIONPERIOD_H__E7306D72_A980_40E9_A306_C20E7080BCD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileRetentionPeriod.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Define                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
#define FRP_TIMER_TBL_MAX				100
//Saiki 20100517 Add ----->
#define FRETEN_LOG_NAME		"FileRetention"
#define FRETEN_LOG_SIZE		(1000000)
#define FRETEN_LOG_MAX		(30)

#define FRETENPRO_LOG_NAME		"FileRetentionProcess"
#define FRETENPRO_LOG_SIZE		(1000000)
#define FRETENPRO_LOG_MAX		(30)
//Saiki 20100517 Add <-----
/////////////////////////////////////////////////////////////////////////////
// Structure                                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Variable                                                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CFileRetentionPeriod thread                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
class CFileRetentionPeriod : public CWinThread
{
	DECLARE_DYNCREATE(CFileRetentionPeriod)
protected:
	CFileRetentionPeriod(); 		  // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	// ======================================================================
	// Variable
	// ======================================================================
	//Saiki 20100517 Add ----->
//	CNanoSpecDoc* m_pDoc;
//	CMainFrame* m_pMainFrame;

	//Saiki 20100517 Add <-----
	CWnd* m_pParent;

	TIMER_STATUS* m_pTimer[FRP_TIMER_TBL_MAX];
	TIMER_STATUS m_MyTimer;
	BOOL m_Run;
	int m_EqId;
	int m_TimerMax;

	///// Process Logging /////
	int* m_pProcessChange[100];
	int m_Old_ProcessChange[100];
	int m_ProcessChangeMax;
	int m_iDeleteDataPeriod;
	__int64 m_iLimitTime;
	struct{
		int DirCheck;
	}m_PROCESS;

	int* m_pProcess;
	CFileFind m_Finder;
	CTime m_CurTime;
	BOOL m_bLoop;

	char m_szPath[MAX_PATH + 1];
	char m_szNextPath[MAX_PATH + 1];
	char m_Process_LogTitle[10240];
	char m_szFilePath[256+1];
	///// Logging /////
	CLogFile m_pProcessLogFile;
	CLogFile m_FRetentionLogFile;



	// ======================================================================
	// Function
	// ======================================================================
	virtual ~CFileRetentionPeriod();
	void Quit(void){m_Run = FALSE;};

	void SetEqId(CWnd* pParent = NULL, int EqId = 0) {
		m_pParent = pParent;
		m_EqId = EqId;
	}

	void VariableInitialize();
	void ProcessInitialize();
	void ProcessChangeInitialize();
	void TimerInitialize();
	void Timer();
	void StartTimer(TIMER_STATUS* Timer, int Value);
	void ResetTimer(TIMER_STATUS* Timer);

	void PROCESS_DirCheck();
	void SetPath(char* pszPath);
    void ProcessLogTitle_Initialize();
    void ProcessLogging();
    void FileRetentionLogging(char* lpszLogText);
    void AddTime(char* lpszBuff);
	BOOL FileOrDirExists(LPCTSTR);
	BOOL DeleteDirectory(LPCTSTR);
	BOOL DeleteAllData(LPCTSTR);
	BOOL CheckDirectory(LPCTSTR);
	BOOL CheckStressResultDir(LPCTSTR);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileRetentionPeriod)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileRetentionPeriod)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILERETENTIONPERIOD_H__E7306D72_A980_40E9_A306_C20E7080BCD4__INCLUDED_)
