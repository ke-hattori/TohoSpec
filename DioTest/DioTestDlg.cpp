// DioTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DioTest.h"
#include "DioTestDlg.h"
#include <nexif.hxx>
#include <nexiobase.hxx>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "dtnsnexio.lib")
#pragma comment(lib, "dtnsnexif.lib")

PCSTR NextraEventsEnumString[] =
{
	"nexInitialize",
    "nexCancel",
    "nexReturnToOrigin",
    "nexLoad",
    "nexUnload",
    "nexMoveToUpper",
    "nexMoveToAlignment",
    "nexMoveToDown",
    "nexOpenWorkGuide",
    "nexCloseWorkGuide",
    "nexOpenShutter",
    "nexCloseShutter",
    "nexVacuumOn",
    "nexVacuumOff",
    "nexSetSampleSize",
};

unsigned long hatoi (const char* s, unsigned long n)
{
	return (unsigned long) strtol(string(s).substr(0, n).c_str(), NULL, 16);
}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDioTestDlg dialog

CDioTestDlg::CDioTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDioTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDioTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDioTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDioTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDioTestDlg, CDialog)
	//{{AFX_MSG_MAP(CDioTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INITIAL, OnInitial)
	ON_BN_CLICKED(IDC_UPPER, OnUpper)
	ON_BN_CLICKED(IDC_SHUTTER_OPEN, OnShutterOpen)
	ON_BN_CLICKED(IDC_SHUTTER_CLOSE, OnShutterClose)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_UNLOAD, OnUnload)
	ON_BN_CLICKED(IDC_ALIGNMENT, OnAlignment)
	ON_BN_CLICKED(IDC_CLAMP, OnClamp)
	ON_BN_CLICKED(IDC_UNCLAMP, OnUnclamp)
	ON_BN_CLICKED(IDC_VACUUM_ON, OnVacuumOn)
	ON_BN_CLICKED(IDC_VACUUM_OFF, OnVacuumOff)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDioTestDlg message handlers

BOOL CDioTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//nexioIsGlassExist();
//	BOOL b = nexioIsPinUpperPosEx(1);
#define TYPEA_NUM1_GUIDEOP_MK   0x00AA

	int mask = TYPEA_NUM1_GUIDEOP_MK;
	int m = 2;
	BOOL bret = m & mask;
	long i = hatoi("0F", 2);
	nexioInitialize(m_hWnd);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDioTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ( (nID & 0xFFF0) == IDM_ABOUTBOX ) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDioTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDioTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CDioTestDlg::OnNexResponse(WPARAM wp, LPARAM lp)
{
	TRACE("%s=%s\n", NextraEventsEnumString[wp], lp ? "SUCCESS" : "FAILED");
	return 0L;
}

void CDioTestDlg::OnInitial() 
{
	BOOL b = nexifInitialize(m_hWnd);
	TRACE("Call nexifInitialize\n");
}

void CDioTestDlg::OnUpper() 
{
	nexifMoveToUpper(m_hWnd);	
	TRACE("Call nexifMoveToUpper\n");
}

void CDioTestDlg::OnShutterOpen() 
{
	nexifOpenShutter(m_hWnd);
	TRACE("Call nexifOpenShutter\n");
}

void CDioTestDlg::OnShutterClose() 
{
	nexifClooseShutter(m_hWnd);	
	TRACE("Call nexifClooseShutter\n");
}

void CDioTestDlg::OnLoad() 
{
	nexifLoad(m_hWnd);	
	TRACE("Call nexifLoad\n");
}

void CDioTestDlg::OnUnload() 
{
	nexifUnload(m_hWnd);
	TRACE("Call nexifUnload\n");
}

void CDioTestDlg::OnAlignment() 
{
	nexifMoveToAlignment(m_hWnd);	
	TRACE("Call nexifMoveToAlignment\n");

}

void CDioTestDlg::OnClamp() 
{
	nexifCloseWorkGuide(m_hWnd);	
	TRACE("Call nexifCloseWorkGuide\n");

}

void CDioTestDlg::OnUnclamp() 
{
	nexifOpenWorkGuide(m_hWnd);	
	TRACE("Call nexifOpenWorkGuide\n");
	
}

void CDioTestDlg::OnVacuumOn() 
{
	nexifVacuumOn(m_hWnd);	
	TRACE("Call nexifVacuumOn\n");

}

void CDioTestDlg::OnVacuumOff() 
{
	nexifVacuumOff(m_hWnd);	
	TRACE("Call nexifVacuumOff\n");

}

void CDioTestDlg::OnDown() 
{
//	nexifMoveToDown(m_hWnd);	
	TRACE("Call nexifMoveToDown\n");
	
}

void CDioTestDlg::OnButton1() 
{
	nexioMonitor(0, 0);
	BOOL b = nexioIsEmergencyStop();
}
