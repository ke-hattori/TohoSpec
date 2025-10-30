// SitePtSeq.h : header file
//

#pragma once

#include "System.h"
#include "Active.hxx"
#include "NanoSpec.h"
#include "ChiefView.h"
#include "ChifTransiSr1Point.h"
#include "LogFile.h"

#define SITEPR_LOG_NAME		"SitePt"
#define SITEPR_LOG_SIZE		(1000000)
#define SITEPR_LOG_MAX		(30)

/////////////////////////////////////////////////////////////////////////////
//
class CSitePtSeq : public CWinThread
{
    DECLARE_DYNCREATE(CSitePtSeq)
protected:
    CSitePtSeq() {}

// Operations
public:
    virtual ~CSitePtSeq() {}
	virtual void ShutDown() {};	/* implementしないとコンパイル通らない	*/

// Overrides
    //{{AFX_VIRTUAL(CSitePtSeq)
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CSitePtSeq)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    ///// Variable /////
public:
	CChiefView* m_pChiefView;							/* 便利そうなので、もらっておく	*/
    CChiefTransiSr1Point* m_pChiefTransiSr1Point;		/* Event発行用に、もらっておく	*/
	LPSTAGE_PROG_INFO_HDR m_pStageProgInfoHdr;
	LPSITE_PATTERN m_pSitePattern;
	int m_iEvent;

	STAGE_COORD		MatchLoc[2];		/* 測定位置			*/
	STAGE_COORD		PrePosition;		/* サーチ開始前位置	*/
	
protected:
    BOOL m_bShutDown;
};


/////////////////////////////////////////////////////////////////////////////
//
class CSitePtAutoSeq : public CSitePtSeq
{
    DECLARE_DYNCREATE(CSitePtAutoSeq)
protected:
    CSitePtAutoSeq() {}

// Operations
public:
    virtual ~CSitePtAutoSeq() {}
	virtual void ShutDown();

// Overrides
    //{{AFX_VIRTUAL(CSitePtAutoSeq)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual int Run();
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CSitePtAutoSeq)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

	void DoPatRecMatchWithVerify( int Site, DESKEW_RESULT* pSitePtResult, STAGE_COORD* pMatchLoc);
	BOOL DoPatternRecMatch(DESKEW_RESULT* pSitePtResult, STAGE_COORD* pMatchLoc, int iLens);
	void PixelToMicron(double xPos, double yPos, STAGE_COORD* pMatchLoc, int iLens);
// 2013.01.09 bagus -->
	void ToClientXyStageCoord(STAGE_COORD* lpPoint);
// 2013.01.09 bagus <--
// 2013.01.17 bagus rotate xy view -->
	long RotateXyAxisDir(long lDir);
// 2013.01.17 bagus rotate xy view <--

	void AddTime(char* lpszBuff);
	void SitePtLogging(TCHAR* pszLogText);

protected:
	SR_PATTERN_REC m_patternRec;
	DESKEW_RESULT SitePtResult[2];						//

	SR_VIDEO_CALIB m_SrVideoCalib[SR_LENS_MAX];
//	SR_TURRET m_SrTurret[SR_LENS_MAX];

// 2013.01.09 bagus -->
	STAGE_CONFIG		m_StageConfig;
// 2013.01.09 bagus <--

    CLogFile m_SitePtLogFile;
	char szLogText[1024];

private:
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- { ---------- */
	SPR_SUB_INFO m_SprSubInfo[SITE_PATTERN_MAX];
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- } ---------- */
};


/////////////////////////////////////////////////////////////////////////////
//
class CSitePtManualSeq : public CSitePtSeq
{
    DECLARE_DYNCREATE(CSitePtManualSeq)
protected:
    CSitePtManualSeq() {}

// Operations
public:
    virtual ~CSitePtManualSeq() {}
	virtual void ShutDown();

// Overrides
    //{{AFX_VIRTUAL(CSitePtManualSeq)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CSitePtManualSeq)
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};
