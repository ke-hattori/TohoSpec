// DeskewSeq.h : header file
//

#pragma once

#include "System.h"
#include "Active.hxx"
#include "NanoSpec.h"
#include "ChiefView.h"
#include "ChifTransiDeskew.h"

/////////////////////////////////////////////////////////////////////////////
//
class CDeskewSeq : public CWinThread
{
	DECLARE_DYNCREATE(CDeskewSeq)
protected:
	CDeskewSeq() {}

// Operations
public:
	virtual ~CDeskewSeq() {}
	virtual void ShutDown() {};	// implementしないとコンパイル通らない

// Overrides
	//{{AFX_VIRTUAL(CDeskewSeq)
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDeskewSeq)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	///// Variable /////
public:
	CChiefView* m_pChiefView;							// 便利そうなので、もらっておく
	CChiefTransiDeskew* m_pChiefTransiDeskew;			// Event発行用に、もらっておく
	LPSTAGE_PROG_INFO_HDR m_pStageProgInfoHdr;
	int m_iEvent;
	STAGE_COORD MatchLoc[2];

// 2009.06.12 K.Matsuo -->
	// CDeskewManualSeqWndから、参照したいのでpublicに変更
	// ShutDown()で設定すること！
//protected:
public:
	BOOL m_bShutDown;
// 2009.06.12 K.Matsuo <--
};


/////////////////////////////////////////////////////////////////////////////
//
class CDeskewAutoSeq : public CDeskewSeq
{
	DECLARE_DYNCREATE(CDeskewAutoSeq)
protected:
	CDeskewAutoSeq() {}

// Operations
public:
	virtual ~CDeskewAutoSeq() {}
	virtual void ShutDown();

// Overrides
	//{{AFX_VIRTUAL(CDeskewAutoSeq)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDeskewAutoSeq)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void DoPatRecMatchWithVerify( int Site, DESKEW_RESULT* pDeskewResult, STAGE_COORD* pMatchLoc);
	BOOL DoPatternRecMatch(DESKEW_RESULT* pDeskewResult, STAGE_COORD* pMatchLoc);
	void PixelToMicron(double xPos, double yPos, STAGE_COORD* pMatchLoc);
// 2013.01.09 bagus -->
	void ToClientXyStageCoord(STAGE_COORD* lpPoint);
// 2013.01.09 bagus <--
// 2013.01.17 bagus rotate xy view -->
	long RotateXyAxisDir(long lDir);
// 2013.01.17 bagus rotate xy view <--

protected:
	SR_PATTERN_REC m_patternRec;
	DESKEW_RESULT DeskewResult[2];

	SR_VIDEO_CALIB m_SrVideoCalib[SR_LENS_MAX];
//	SR_TURRET m_SrTurret[SR_LENS_MAX];
// 2013.01.09 bagus -->
	STAGE_CONFIG		m_StageConfig;
// 2013.01.09 bagus <--
};


/////////////////////////////////////////////////////////////////////////////
//
class CDeskewManualSeq : public CDeskewSeq
{
	DECLARE_DYNCREATE(CDeskewManualSeq)
protected:
	CDeskewManualSeq() {}

// Operations
public:
	virtual ~CDeskewManualSeq() {}
	virtual void ShutDown();

// Overrides
	//{{AFX_VIRTUAL(CDeskewManualSeq)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDeskewManualSeq)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
