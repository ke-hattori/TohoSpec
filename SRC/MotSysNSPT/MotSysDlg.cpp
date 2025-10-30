// MotSysDlg.cpp : ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//

#include <io.h>
#include "stdafx.h"
#include "Sio.h"
#include "LogFile.h"
#include "MotSysDef.h"
#define MotMsgON
#include "..\\..\\inc\\MotsysMsg.h"
#include "MotSysDlg.h"
#include "CtrlCmd.h"
#include "MotionDef.h"
#include "Motion.h"
#include "FileNameDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT MOTDRV_SHOWALARM = RegisterWindowMessage("MOTDRV_SHOWALARM");	// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½pï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½`
UINT MOTDRV_SRVINITCOMP = RegisterWindowMessage("MOTDRV_SRVINITCOMP");	// ï¿½Tï¿½[ï¿½{ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½`

CSio *g_pCSio;
CLogFile *g_pComLogFile;
CWinThread* g_pCCThread;	// Thread pointer

#define	ID_MSD_BASE_TIMER	101	// OnTimer ID
#define	MSD_BASE_TIME		200	// [ms]


/////////////////////////////////////////////////////////////////////////////
// ï¿½Aï¿½vï¿½ï¿½ï¿½Pï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Ìƒoï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å�gï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ CAboutDlg ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O ï¿½fï¿½[ï¿½^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard ï¿½Í‰ï¿½ï¿½zï¿½Ö�ï¿½ï¿½ÌƒIï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½hï¿½ğ�¶�ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ï¿½ÌƒTï¿½|ï¿½[ï¿½g
	//}}AFX_VIRTUAL

// ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnBtnComLogClear();
	afx_msg void OnBtnComLogEnable();
	virtual BOOL OnInitDialog();
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

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½É�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì•â‘«ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	// ï¿½ï¿½ï¿½Ìƒ_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½pï¿½ÌƒAï¿½Cï¿½Rï¿½ï¿½ï¿½ï¿½İ’ï¿½
	SetIcon(((CMotSysDlg*) (this->GetParent()))->m_hMainIcon, TRUE);	// ï¿½å‚«ï¿½ï¿½ï¿½Aï¿½Cï¿½Rï¿½ï¿½ï¿½ï¿½İ’ï¿½

	if (g_pCSio != NULL) {
		if (g_pCSio->m_LogEnable == FALSE) {
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Disable"));
		} else {
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Enable"));
		}
	}

	return TRUE;  // ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Éƒtï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½İ’è‚µï¿½È‚ï¿½ï¿½Æ‚ï¿½ï¿½Aï¿½ß‚ï¿½lï¿½ï¿½ TRUE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
				  // ï¿½ï¿½O: OCX ï¿½vï¿½ï¿½ï¿½pï¿½eï¿½B ï¿½yï¿½[ï¿½Wï¿½Ì–ß‚ï¿½lï¿½ï¿½ FALSE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
}

// ï¿½ï¿½ï¿½Oï¿½Nï¿½ï¿½ï¿½Aï¿½{ï¿½^ï¿½ï¿½
void CAboutDlg::OnBtnComLogClear()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	if (g_pCSio != NULL) {
		GetDlgItem(IDC_BTN_COMLOGENA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_COMLOGCLR)->EnableWindow(FALSE);
		g_pCSio->ClearLog();
		GetDlgItem(IDC_BTN_COMLOGCLR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_COMLOGENA)->EnableWindow(TRUE);
	}
}

// ï¿½ï¿½ï¿½Oï¿½Ì‹ï¿½ï¿½ï¿½/ï¿½Ö�~
void CAboutDlg::OnBtnComLogEnable()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	if (g_pCSio != NULL) {
		if (g_pCSio->m_LogEnable == FALSE) {
			g_pCSio->m_LogEnable = TRUE;
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Enable"));
			// Servo.ini ï¿½É‹Lï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
			::WritePrivateProfileString(_T("Log"), _T("ComLog"), _T("1"), SERVO_INI);
		} else {
			g_pCSio->m_LogEnable = FALSE;
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Disable"));
			// Servo.ini ï¿½É‹Lï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
			::WritePrivateProfileString(_T("Log"), _T("ComLog"), _T("0"), SERVO_INI);
		}
	}
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_BTN_COMLOGCLR, OnBtnComLogClear)
	ON_BN_CLICKED(IDC_BTN_COMLOGENA, OnBtnComLogEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotSysDlg ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O

CMotSysDlg::CMotSysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotSysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotSysDlg)
		// ï¿½ï¿½ï¿½ï¿½: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ï¿½ ClassWizard ï¿½É‚ï¿½ï¿½ï¿½Äƒï¿½ï¿½ï¿½ï¿½oï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//}}AFX_DATA_INIT
	// ï¿½ï¿½ï¿½ï¿½: LoadIcon ï¿½ï¿½ Win32 ï¿½ï¿½ DestroyIcon ï¿½ÌƒTï¿½uï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMotSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotSysDlg)
		// ï¿½ï¿½ï¿½ï¿½: ï¿½ï¿½ï¿½Ì�ê�Šï¿½É‚ï¿½ ClassWizard ï¿½É‚ï¿½ï¿½ï¿½ï¿½ DDX ï¿½ï¿½ DDV ï¿½ÌŒÄ‚Ñ�oï¿½ï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMotSysDlg, CDialog)
	//{{AFX_MSG_MAP(CMotSysDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SERVO_OFF, OnBTNServoOFF)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ORGtest, OnBTNORGtest)
	ON_BN_CLICKED(IDC_BTN_SERVO_ON, OnBTNServoON)
	ON_BN_CLICKED(IDC_BTN_ORGtestY, OnBTNORGtestY)
	ON_BN_CLICKED(IDC_BTN_ORGtestZ, OnBTNORGtestZ)
	ON_BN_CLICKED(IDC_BTN_ORGtestT, OnBTNORGtestT)
	ON_BN_CLICKED(IDC_BTN_ABStestX, OnBTNABStestX)
	ON_BN_CLICKED(IDC_BTN_ABStestY, OnBTNABStestY)
	ON_BN_CLICKED(IDC_BTN_ABStestZ, OnBTNABStestZ)
	ON_BN_CLICKED(IDC_BTN_ABStestT, OnBTNABStestT)
	ON_BN_CLICKED(IDC_BTN_INCtestX, OnBTNINCtestX)
	ON_BN_CLICKED(IDC_BTN_INCtestY, OnBTNINCtestY)
	ON_BN_CLICKED(IDC_BTN_INCtestZ, OnBTNINCtestZ)
	ON_BN_CLICKED(IDC_BTN_INCtestT, OnBTNINCtestT)
	ON_BN_CLICKED(IDC_BTN_HPtestX, OnBTNHPtestX)
	ON_BN_CLICKED(IDC_BTN_HPtestY, OnBTNHPtestY)
	ON_BN_CLICKED(IDC_BTN_HPtestZ, OnBTNHPtestZ)
	ON_BN_CLICKED(IDC_BTN_HPtestT, OnBTNHPtestT)
	ON_BN_CLICKED(IDC_BTN_ABStestX2, OnBTNABStestX2)
	ON_BN_CLICKED(IDC_BTN_ABStestY2, OnBTNABStestY2)
	ON_BN_CLICKED(IDC_BTN_ABStestZ2, OnBTNABStestZ2)
	ON_BN_CLICKED(IDC_BTN_ABStestT2, OnBTNABStestT2)
	ON_BN_CLICKED(IDC_BTN_INCtestX2, OnBTNINCtestX2)
	ON_BN_CLICKED(IDC_BTN_INCtestY2, OnBTNINCtestY2)
	ON_BN_CLICKED(IDC_BTN_INCtestZ2, OnBTNINCtestZ2)
	ON_BN_CLICKED(IDC_BTN_INCtestT2, OnBTNINCtestT2)
	ON_BN_CLICKED(IDC_BTN_SERVO_PARAM, OnBtnServoParam)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_X, OnBtnDPosPeakClearX)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_YM, OnBtnDPosPeakClearYM)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_YS, OnBtnDPosPeakClearYS)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_Z, OnBtnDPosPeakClearZ)
	ON_BN_CLICKED(IDC_BTN_ORGtestX, OnBTNORGtest)
	ON_BN_CLICKED(IDC_BTN_DPOSP_CLR_T, OnBtnDPosPeakClearT)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(MOTDRV_SHOWINDOW, OnMotsysShowWindow)
	ON_REGISTERED_MESSAGE(MOTDRV_HIDEINDOW, OnMotsysHideWindow)
	ON_REGISTERED_MESSAGE(MOTDRV_SETWINDOW, OnMotdrv_Setwindow)
	ON_REGISTERED_MESSAGE(MOTDRV_SETSOFTHOME, OnMotdrv_Setsofthome)
	ON_REGISTERED_MESSAGE(MOTDRV_EMERGENCYSTOP, OnMotdrv_Emergencystop)
	ON_REGISTERED_MESSAGE(MOTDRV_MOVEATSPEED, OnMotdrv_Moveatspeed)
	ON_REGISTERED_MESSAGE(MOTDRV_MOVETOPOSITION, OnMotdrv_Movetoposition)
	ON_REGISTERED_MESSAGE(MOTDRV_GETPOSITION, OnMotdrv_Getposition)
	ON_REGISTERED_MESSAGE(MOTDRV_GOHOME, OnMotdrv_Gohome)
	ON_REGISTERED_MESSAGE(MOTDRV_MOVEATSPEEDTODEST, OnMotdrv_Moveatspeedtodest)
	ON_REGISTERED_MESSAGE(MOTDRV_ISSTOPPED, OnMotdrv_Isstopped)
	ON_REGISTERED_MESSAGE(MOTDRV_ISJOYMODE, OnMotdrv_IsJoyMode)
	ON_REGISTERED_MESSAGE(MOTDRV_SETJOYMODE, OnMotdrv_SetJoyMode)
	ON_REGISTERED_MESSAGE(MOTDRV_GETMAXSPEED, OnMotdrv_GetMaxSpeed)
	ON_REGISTERED_MESSAGE(MOTDRV_GETMOTIONSIZE, OnMotdrv_GetMotionSize)
	ON_REGISTERED_MESSAGE(MOTDRV_GETELECTRONICGEAR, OnMotdrv_GetElectronicGear)
	ON_REGISTERED_MESSAGE(MOTDRV_GETSOFTLIMIT, OnMotdrv_GetSoftLimit)
	ON_REGISTERED_MESSAGE(MOTDRV_SETJOYSPEED, OnMotdrv_SetJoySpeed)
	ON_REGISTERED_MESSAGE(MOTDRV_SHOWALARM, OnShowAlarm)
	ON_REGISTERED_MESSAGE(MOTDRV_SRVINITCOMP, OnServoInitComp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotSysDlg ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½

BOOL CMotSysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "ï¿½oï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½..." ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ú‚ï¿½ï¿½Vï¿½Xï¿½eï¿½ï¿½ ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½Ö’Ç‰ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B

	// IDM_ABOUTBOX ï¿½ÍƒRï¿½}ï¿½ï¿½ï¿½h ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½Ì”ÍˆÍ‚Å‚È‚ï¿½ï¿½ï¿½Î‚È‚ï¿½Ü‚ï¿½ï¿½ï¿½B
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

	// ï¿½ï¿½ï¿½Ìƒ_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½pï¿½ÌƒAï¿½Cï¿½Rï¿½ï¿½ï¿½ï¿½İ’è‚µï¿½Ü‚ï¿½ï¿½Bï¿½tï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Nï¿½ÍƒAï¿½vï¿½ï¿½ï¿½Pï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½Cï¿½ï¿½
	// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½Å‚È‚ï¿½ï¿½ï¿½ï¿½Í�ï¿½ï¿½ï¿½ï¿½Iï¿½É�İ’è‚µï¿½Ü‚ï¿½ï¿½ï¿½B
	SetIcon(m_hIcon, TRUE);			// ï¿½å‚«ï¿½ï¿½ï¿½Aï¿½Cï¿½Rï¿½ï¿½ï¿½ï¿½İ’ï¿½
//	SetIcon(m_hIcon, FALSE);		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½Cï¿½Rï¿½ï¿½ï¿½ï¿½İ’ï¿½
	// ï¿½ï¿½ï¿½Ì�sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÆƒVï¿½Xï¿½eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½16x16ï¿½Aï¿½Cï¿½Rï¿½ï¿½ï¿½ï¿½Tï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½D
	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½C16x16ï¿½Aï¿½Cï¿½Rï¿½ï¿½ï¿½Í—\ï¿½ß�ì�¬ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D

	// TODO: ï¿½ï¿½ï¿½Ê‚È�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½Í‚ï¿½ï¿½Ì�ê�Šï¿½É’Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B

	// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½Ìƒvï¿½ï¿½ï¿½pï¿½eï¿½Bï¿½ï¿½ï¿½Xï¿½gï¿½É�ï¿½ï¿½Ê�ï¿½ï¿½ï¿½Ç‰ï¿½
	::SetProp(m_hWnd, IDENT_CODE, (HANDLE) 1);

	//ï¿½Ï�ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
	m_bOnClosing = FALSE;
	m_bDIO_Opned = FALSE;
	m_pComLogFile = 0;
	m_pSio = 0;
	g_pCCThread = 0;
	m_bInitFailed = FALSE;
	m_hMainIcon = m_hIcon;
	m_OnClose = FALSE;
	g_CC_bInitialize = FALSE;
	memset(m_dPos, 0, sizeof(m_dPos));
	InitMotionData();

	// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ÌƒCï¿½lï¿½[ï¿½uï¿½ï¿½ï¿½İ’ï¿½
	EnableDlgCtrl();

	TCHAR	l_tszText[32];
	::GetPrivateProfileString(_T("Window"), _T("AutoHide"), _T("2"), l_tszText, sizeof(l_tszText), SERVO_INI);
	m_AutoHide = _ttoi(l_tszText);
	if ((m_AutoHide < 0) || (2 < m_AutoHide)) {
		m_AutoHide = 2;
	}
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- { ---------- */
	/* DIO ï¿½ï¿½ï¿½O ï¿½ï¿½ï¿½ï¿½/ï¿½ï¿½ï¿½È‚ï¿½ï¿½İ’ï¿½Ç�ï¿½ï¿½ï¿½	*/
	::GetPrivateProfileString(_T("DIO"), _T("Log"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	if (0 == _tcscmp(_T("1"), l_tszText)) {
		m_bDioLog = TRUE;
	} else {
		m_bDioLog = FALSE;
	}
	/* DIO ï¿½ï¿½ï¿½O ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½İ’ï¿½	*/
	if (TRUE == m_bDioLog) {
		TCHAR l_tszDrive[_MAX_PATH], l_tszDir[_MAX_DIR], l_tszFName[_MAX_FNAME], l_tszExt[_MAX_EXT];
		TCHAR l_tszFilePath[_MAX_PATH];
		_tsplitpath(MS_DIO_LOG, l_tszDrive, l_tszDir, l_tszFName, l_tszExt);
		_stprintf(l_tszFilePath, _T("%s%s"), l_tszDrive, l_tszDir);
		m_pcDioLog = new CLogFile(l_tszFName);
		if (0 != m_pcDioLog) {
			_tcscpy(m_pcDioLog->m_tszLogFilePath, l_tszFilePath);
		} else {
			::MessageBox(NULL, _T("Failed to create DIO Log class"), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	} else {
		m_pcDioLog = 0;
	}
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- } ---------- */

	SetDlgItemText(IDC_STATIC_STATUS, _T("Initializing..."));

	// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
	{
		TCHAR l_tszDrive[_MAX_PATH], l_tszDir[_MAX_PATH], l_tszFName[_MAX_FNAME], l_tszExt[8];
		TCHAR	l_tszFilePath[_MAX_PATH];
		_tsplitpath(ALARM_LOG, l_tszDrive, l_tszDir, l_tszFName, l_tszExt);
		// ï¿½pï¿½Xï¿½Æƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�¬
		_stprintf(l_tszFilePath, _T("%s%s"), l_tszDrive, l_tszDir);
		m_pAlarmLogFile = new CLogFile(l_tszFName);
		if (0 != m_pAlarmLogFile) {
			_tcscpy(m_pAlarmLogFile->m_tszLogFilePath, l_tszFilePath);
		} else {
			::MessageBox(NULL, _T("Failed to create Alarm Log class"), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	}

	// DIO ï¿½Iï¿½[ï¿½vï¿½ï¿½
	if (0 == m_bInitFailed) {
		if (CC_DioOpen() != 0) {
			// ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½s
			m_bInitFailed = TRUE;
			SetDlgItemText(IDC_STATIC_STATUS, _T("Failed to DIO opened"));
			SetDlgItemText(IDC_STATIC_COMSTAT, _T("Uninitialized"));
			::MessageBox(NULL, _T("Failed to open DIO Driver."), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		} else {
			m_bDIO_Opned = TRUE;
			// ï¿½Vï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Ê�Mï¿½Ìƒï¿½ï¿½Oï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
			if (InitializeCommLog() == FALSE) {
				m_bInitFailed = TRUE;
				SetDlgItemText(IDC_STATIC_STATUS, _T("Uninitialized"));
				SetDlgItemText(IDC_STATIC_COMSTAT, _T("Failed to create Comm. Log"));
				::MessageBox(NULL, _T("Failed to create Comm Log class"), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			} else {
				// ï¿½Vï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Ê�Mï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
				if (InitializeComm() == TRUE) {
					char l_tszText[80], l_tszText2[8], l_tszText3[8];
					switch (m_pSio->m_Parity) {
					case 0: strcpy(l_tszText2, _T("N")); break;
					case 1: strcpy(l_tszText2, _T("O")); break;
					case 2: strcpy(l_tszText2, _T("E")); break;
					case 3: strcpy(l_tszText2, _T("M")); break;
					case 4: strcpy(l_tszText2, _T("S")); break;
					}
					switch (m_pSio->m_StopBits) {
					case 0: strcpy(l_tszText3, _T("1")); break;
					case 1: strcpy(l_tszText3, _T("1.5")); break;
					case 2: strcpy(l_tszText3, _T("2")); break;
					}
					_stprintf(l_tszText, _T("%s,%d,%s,%s"), m_pSio->m_sChannelNo, m_pSio->m_BaudRate, l_tszText2, l_tszText3);
					SetDlgItemText(IDC_STATIC_COMSTAT, l_tszText);
				} else {
					m_bInitFailed = TRUE;
					SetDlgItemText(IDC_STATIC_COMSTAT, _T("Uninitialized"));
					SetDlgItemText(IDC_STATIC_STATUS, _T("Uninitialized"));
				}
			}
		}
	} else {
		SetDlgItemText(IDC_STATIC_STATUS, _T("Failed to open DIO"));
		SetDlgItemText(IDC_STATIC_COMSTAT, _T("Uninitialized"));
	}

	// ï¿½Aï¿½oï¿½Eï¿½gï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ ICON ï¿½ï¿½İ’ï¿½
	((CButton*) GetDlgItem(IDC_BTN_ABOUT))->SetIcon(m_hIcon);

	// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½Å‘Oï¿½Ê‚É‚ï¿½ï¿½ï¿½
#ifndef _DEBUG
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#endif

	// ï¿½ï¿½ï¿½Ìƒvï¿½ï¿½ï¿½Zï¿½Xï¿½Ì—Dï¿½ï¿½xï¿½ï¿½İ’è‚·ï¿½ï¿½
	HANDLE l_hCurProc = ::GetCurrentProcess();
	// ï¿½Oï¿½Ì—Dï¿½ï¿½xï¿½ï¿½ï¿½æ“¾
	DWORD l_oldPriority = ::GetPriorityClass(l_hCurProc);
	ChgToPriorityMsg(l_oldPriority, l_tszText);
	SetDlgItemText(IDC_STATIC_PROCPRI2, l_tszText);
#if 1
//	BOOL l_err = SetPriorityClass(l_hCurProc, REALTIME_PRIORITY_CLASS);
	BOOL l_err = SetPriorityClass(l_hCurProc, HIGH_PRIORITY_CLASS);
	if (l_err == 0) {
		// ï¿½Dï¿½ï¿½xï¿½Ì�Ø‘Ö‚ï¿½ï¿½ï¿½ï¿½s
		DWORD l_errcode = GetLastError();
		TCHAR l_tszErrMsg[255]; ShowLastError(l_errcode, l_tszText);
		_stprintf(l_tszErrMsg, _T("Failed to change process priority.\n%s(0x%08x)"), l_tszText, l_errcode);
		if (0 == m_bOnClosing) {
			::MessageBox(NULL, l_tszErrMsg, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	}
	// ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Dï¿½ï¿½xï¿½ï¿½ï¿½æ“¾
	DWORD l_newPriority = GetPriorityClass(l_hCurProc);
	ChgToPriorityMsg(l_newPriority, l_tszText);
	SetDlgItemText(IDC_STATIC_PROCPRI, l_tszText);
#endif

	// ï¿½^ï¿½Cï¿½}(OnTimer)ï¿½ï¿½ï¿½Nï¿½ï¿½
	SetTimer(ID_MSD_BASE_TIMER, MSD_BASE_TIME, NULL);

	return TRUE;  // TRUE ï¿½ï¿½Ô‚ï¿½ï¿½ÆƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½É�İ’è‚µï¿½ï¿½ï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½Í�ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½B
}

void CMotSysDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½{ï¿½bï¿½Nï¿½Xï¿½É�Å�ï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½Î�Aï¿½Aï¿½Cï¿½Rï¿½ï¿½ï¿½ï¿½`ï¿½æ‚·ï¿½ï¿½
// ï¿½Rï¿½[ï¿½hï¿½ï¿½ï¿½È‰ï¿½ï¿½É‹Lï¿½qï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½BMFC ï¿½Aï¿½vï¿½ï¿½ï¿½Pï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ document/view
// ï¿½ï¿½ï¿½fï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½Íƒtï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Nï¿½É‚ï¿½è�©ï¿½ï¿½ï¿½Iï¿½É�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B

void CMotSysDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ï¿½`ï¿½ï¿½pï¿½Ìƒfï¿½oï¿½Cï¿½X ï¿½Rï¿½ï¿½ï¿½eï¿½Lï¿½Xï¿½g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// ï¿½Nï¿½ï¿½ï¿½Cï¿½Aï¿½ï¿½ï¿½gï¿½Ì‹ï¿½`ï¿½Ìˆï¿½ï¿½ï¿½Ì’ï¿½ï¿½ï¿½
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ï¿½Aï¿½Cï¿½Rï¿½ï¿½ï¿½ï¿½`ï¿½æ‚µï¿½Ü‚ï¿½ï¿½B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Í�Aï¿½ï¿½ï¿½[ï¿½Uï¿½[ï¿½ï¿½ï¿½Å�ï¿½ï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½hï¿½ï¿½ï¿½bï¿½Oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Ô�A
// ï¿½Jï¿½[ï¿½\ï¿½ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½é‚½ï¿½ß‚É‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
HCURSOR CMotSysDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMotSysDlg::OnOK()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½É‚ï¿½ï¿½Ì‘ï¿½ï¿½ÌŒï¿½ï¿½Ø—pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	// "ï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½"ï¿½Lï¿½[ï¿½Å�Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚½ï¿½ï¿½
//	CDialog::OnOK();
}

void CMotSysDlg::OnBtnAbout()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	// ï¿½Aï¿½oï¿½Eï¿½gï¿½ï¿½\ï¿½ï¿½
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CMotSysDlg::OnBtnHide()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½E(ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½xï¿½[ï¿½X)ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½
	ShowWindow(SW_HIDE);
}

LRESULT CMotSysDlg::OnMotsysHideWindow(WPARAM wParam, LPARAM lParam)
{
	// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½E(ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½xï¿½[ï¿½X)ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CMotSysDlg::OnMotsysShowWindow(WPARAM wParam, LPARAM lParam)
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½E(ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½xï¿½[ï¿½X)ï¿½ï¿½\ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½
	ShowWindow(SW_SHOWNORMAL);
	return 0;
}

void CMotSysDlg::OnTimer(UINT nIDEvent)
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½Éƒï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚©ï¿½Ü‚ï¿½ï¿½Íƒfï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	switch (nIDEvent) {
	case ID_MSD_BASE_TIMER:
		{
			static BOOL l_bRunOnce = TRUE;
			if ((0 != l_bRunOnce) && (0 == m_bInitFailed)) {
				l_bRunOnce = FALSE;
				// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ä�ï¿½ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½ï¿½ï¿½Nï¿½ï¿½(ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì’ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
				g_pCCThread = AfxBeginThread(CC_Main_Loop, (CWnd*) this, THREAD_PRIORITY_NORMAL);
				g_pCCThread->m_bAutoDelete = FALSE;
			}
		}
		if (0 == m_bInitFailed) {
			// ï¿½ï¿½ï¿½M/ï¿½ï¿½Mï¿½ï¿½ï¿½ï¿½ï¿½vï¿½Ì�ï¿½ï¿½ï¿½
			static BOOL ls_IndicateS = FALSE;
			static BOOL ls_IndicateR = FALSE;
			if (m_pSio != NULL) {
				if (ls_IndicateS == TRUE) {
					ls_IndicateS = FALSE;
					if (m_pSio->m_Indicate_Send == TRUE) {
						SetDlgItemText(IDC_STATIC_SEND, _T("S"));
						m_pSio->m_Indicate_Send = FALSE;
					}
				} else {
					ls_IndicateS = TRUE;
					SetDlgItemText(IDC_STATIC_SEND, _T("s"));
				}
				if (ls_IndicateR == TRUE) {
					ls_IndicateR = FALSE;
					if (m_pSio->m_Indicate_Recv == TRUE) {
						SetDlgItemText(IDC_STATIC_RECV, _T("R"));
						m_pSio->m_Indicate_Recv = FALSE;
					}
				} else {
					ls_IndicateR = TRUE;
					SetDlgItemText(IDC_STATIC_RECV, _T("r"));
				}
			}
		}
		{
			CC_DispOthers();
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

/*
 *	ï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½
 */
LRESULT CMotSysDlg::OnMotdrv_Setwindow(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_SETWINDOW, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Setsofthome(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_SETSOFTHOME, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Emergencystop(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_EMERGENCYSTOP, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Moveatspeed(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_MOVEATSPEED, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Movetoposition(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_MOVETOPOSITION, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Getposition(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETPOSITION, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Gohome(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GOHOME, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Moveatspeedtodest(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_MOVEATSPEEDTODEST, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_Isstopped(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_ISSTOPPED, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_IsJoyMode(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_ISJOYMODE, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_SetJoyMode(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_SETJOYMODE, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_GetMaxSpeed(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETMAXSPEED, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_GetMotionSize(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETMOTIONSIZE, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_GetElectronicGear(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETELECTRONICGEAR, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_GetSoftLimit(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_GETSOFTLIMIT, wParam, lParam);
}
LRESULT CMotSysDlg::OnMotdrv_SetJoySpeed(WPARAM wParam, LPARAM lParam)
{
	return MotsysDriverProc(MOTDRV_SETJOYSPEED, wParam, lParam);
}

// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½Ö�ï¿½
LRESULT CMotSysDlg::MotsysDriverProc(UINT uMsg, WPARAM lParam1, LPARAM lParam2)
{
	LRESULT	lRes = 0L;

	if (uMsg == 0) {
		lRes = -1L;  // return 0L to FAIL load
	} else {
		lRes = MotsysProc(uMsg, lParam1, lParam2);
	}

	return lRes;
}
// ï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Ö�ï¿½
LRESULT CMotSysDlg::MotsysProc(UINT uMsg, WPARAM lParam1, LPARAM lParam2)
{
	LRESULT	lRes = 0L;
	short axis;

	if		  (uMsg == MOTDRV_SETWINDOW) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_set_response_window(axis, (HWND) lParam2);
	} else if (uMsg == MOTDRV_SETSOFTHOME) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_set_soft_home(axis, lParam2);
	} else if (uMsg == MOTDRV_EMERGENCYSTOP) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_emergency_stop(axis);
	} else if (uMsg == MOTDRV_MOVEATSPEED) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_move_at_speed(axis, HIWORD(lParam1));
	} else if (uMsg == MOTDRV_MOVETOPOSITION) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_move_to_position(axis, lParam2, HIWORD(lParam1));
	} else if (uMsg == MOTDRV_GETPOSITION) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_position(axis);
	} else if (uMsg == MOTDRV_GOHOME) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_go_home(axis);
//	} else if (uMsg == MOTDRV_MOVEATSPEEDTODEST) {	ï¿½ï¿½ï¿½ï¿½ï¿½ MOTDRV_MOVETOPOSITION ï¿½Æ“ï¿½ï¿½ï¿½ï¿½È‚Ì‚Å�gï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Æ‚É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
//		axis = ConvAxis(LOWORD(lParam1));
//		lRes = motion_move_at_speed_to_dest(axis, lParam2, HIWORD(lParam1));
	} else if (uMsg == MOTDRV_ISSTOPPED) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_is_stopped(axis);
	} else if (uMsg == MOTDRV_ISJOYMODE) {
		lRes = motion_is_joymode();
	} else if (uMsg == MOTDRV_SETJOYMODE) {
		lRes = motion_set_joymode(lParam1, lParam2);
	} else if (uMsg == MOTDRV_GETMAXSPEED) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_maxspeed(axis);
	} else if (uMsg == MOTDRV_GETMOTIONSIZE) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_motionsize(axis);
	} else if (uMsg == MOTDRV_GETELECTRONICGEAR) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_electronicgear(axis);
	} else if (uMsg == MOTDRV_GETSOFTLIMIT) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_get_softlimit(axis, HIWORD(lParam1));
	} else if (uMsg == MOTDRV_SETJOYSPEED) {
		axis = ConvAxis(LOWORD(lParam1));
		lRes = motion_set_joyspeed(axis, lParam2, HIWORD(lParam1));
	} else {
		lRes = MS_NO_FUNCTION;
	}

	return lRes;
}

void CMotSysDlg::OnCancel()
{
	if (m_OnClose == TRUE) {
		CDialog::OnCancel();
	}
}

void CMotSysDlg::OnClose()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½Éƒï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚©ï¿½Ü‚ï¿½ï¿½Íƒfï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_bOnClosing = TRUE;

	// ï¿½Tï¿½[ï¿½{ OFFï¿½Cï¿½Sï¿½ï¿½ï¿½ï¿½~
	if (0 != g_pCCThread) {
		CC_ServoOFF_RunOFF();
	}

	/*	ï¿½ï¿½ï¿½Lï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ GetExitCodeThread ï¿½É‚ï¿½ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½Ì�Iï¿½ï¿½ï¿½ÌŠmï¿½Fï¿½ï¿½ï¿½oï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ß�C
		ï¿½ï¿½ï¿½Ì‚æ‚¤ï¿½È�ï¿½ï¿½ï¿½ï¿½É‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½D
		ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä�Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½uï¿½[ï¿½gï¿½ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½D*/

	// ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Ü‚ï¿½ MessageBox ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÎƒNï¿½ï¿½ï¿½[ï¿½Yï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	HWND m_hwnd_MsgBox;
	do {
		if ((m_hwnd_MsgBox = ::FindWindow(NULL, MSD_MESSAGEBOX_TITLE)) != NULL) {
			::PostMessage(m_hwnd_MsgBox, WM_CLOSE, 0, 0);
			// ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚½ï¿½ß‚É�Ä“x WM_CLOSE ï¿½ğ”­�sï¿½ï¿½ï¿½COS ï¿½É�ï¿½ï¿½ï¿½ï¿½nï¿½ï¿½
			PostMessage(WM_CLOSE, 0, 0);
			return;
		}
	} while (m_hwnd_MsgBox != NULL);

	KillTimer(ID_MSD_BASE_TIMER);

	if (0 != g_pCCThread) {
		m_pSio->EndComm();
		g_CC_ThreadLoop = FALSE;
		DWORD l_dwRc = WaitForSingleObject(g_pCCThread->m_hThread, 100);
		switch (l_dwRc) {
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			PostMessage(WM_CLOSE, 0, 0);
			return;
			break;
		case WAIT_ABANDONED:
		case WAIT_FAILED:
		default:
			TerminateThread(g_pCCThread, 0);
			break;
		}
		delete g_pCCThread;
		g_pCCThread = NULL;
	}

	CloseObjects();		// ï¿½eï¿½ï¿½Iï¿½uï¿½Wï¿½Fï¿½Nï¿½gï¿½ï¿½ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	CDialog::OnClose();
}
// ï¿½eï¿½ï¿½Iï¿½uï¿½Wï¿½Fï¿½Nï¿½gï¿½ï¿½ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
void CMotSysDlg::CloseObjects()
{
	m_OnClose = TRUE;

	// DIO ï¿½Nï¿½ï¿½ï¿½[ï¿½Y
	if (0 != m_bDIO_Opned) {
		m_bDIO_Opned = FALSE;
		if (CC_DioClose() != 0) {
			;	// ï¿½Nï¿½ï¿½ï¿½[ï¿½Yï¿½ï¿½ï¿½s
		}
	}

	// ï¿½Vï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Ê�Mï¿½ÌŠJï¿½ï¿½
	if (m_pSio) {
		delete m_pSio;
		m_pSio = NULL;
	}
	if (m_pComLogFile) {
		delete m_pComLogFile;
		m_pComLogFile = NULL;
	}
	if (m_pAlarmLogFile) {
		delete m_pAlarmLogFile;
		m_pAlarmLogFile = NULL;
	}

}

// ï¿½Dï¿½ï¿½xï¿½Rï¿½[ï¿½hï¿½ğ•¶�ï¿½ï¿½ï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½ï¿½
void CMotSysDlg::ChgToPriorityMsg(DWORD code, TCHAR *ptszmsg)
{
	switch (code) {
	case HIGH_PRIORITY_CLASS:
		_stprintf(ptszmsg, _T("HIGH_PRIORITY_CLASS(0x%08x)"), code);
		break;
	case IDLE_PRIORITY_CLASS:
		_stprintf(ptszmsg, _T("IDLE_PRIORITY_CLASS(0x%08x)"), code);
		break;
	case NORMAL_PRIORITY_CLASS:
		_stprintf(ptszmsg, _T("NORMAL_PRIORITY_CLASS(0x%08x)"), code);
		break;
	case REALTIME_PRIORITY_CLASS:
		_stprintf(ptszmsg, _T("REALTIME_PRIORITY_CLASS(0x%08x)"), code);
		break;
	default:
		_stprintf(ptszmsg, _T("Undefined(0x%08x)"), code);
		break;
	}
}

// ï¿½Gï¿½ï¿½ï¿½[ï¿½Rï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½ÌƒGï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½ï¿½
void CMotSysDlg::ShowLastError(DWORD code, TCHAR *ptszErrMsg)
{
	LPVOID lpMsgBuf;
	::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
		);
	_tcscpy(ptszErrMsg, (char*) lpMsgBuf);
	::LocalFree(lpMsgBuf);

	TCHAR *l_ptszText;
	l_ptszText = ptszErrMsg;
	while (1) {
		if ((*l_ptszText == 0x7f) || ((0x00 <= *l_ptszText) && (*l_ptszText <= 0x1f))) {
			*l_ptszText = 0x00;
			break;
		}
		l_ptszText++;
	}
}

// ï¿½Vï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Ê�Mï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
BOOL CMotSysDlg::InitializeComm()
{
	TCHAR	l_tszText[255];
	::GetPrivateProfileString(_T("COM"), _T("No"), _T("3"), l_tszText, sizeof(l_tszText), MOT_INI);	// ï¿½|ï¿½[ï¿½gï¿½Ô�ï¿½ï¿½Ç�oï¿½ï¿½
	int l_PortCh = _ttoi(l_tszText);
	m_pSio = g_pCSio = new CSio(l_PortCh);	// ï¿½Ê�Mï¿½Nï¿½ï¿½ï¿½Xï¿½ì�¬
	if (m_pSio == NULL) {
		return FALSE;
	}
	::GetPrivateProfileString(_T("Log"), _T("ComLog"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);	// ï¿½Ê�Mï¿½ï¿½ï¿½O on/off
	m_pSio->m_LogEnable = _ttoi(l_tszText);
	if (m_pSio->m_LogEnable != 0) {
		m_pSio->m_LogEnable = TRUE;
	} else {
		m_pSio->m_LogEnable = FALSE;
	}
	::GetPrivateProfileString(_T("COM"), _T("BaudRate"), _T("38400"), l_tszText, sizeof(l_tszText), MOT_INI);	// ï¿½{ï¿½[ï¿½ï¿½ï¿½[ï¿½gï¿½Ç�oï¿½ï¿½
	int l_BaudRate = _ttoi(l_tszText);
	::GetPrivateProfileString(_T("COM"), _T("ByteSize"), _T("8"), l_tszText, sizeof(l_tszText), MOT_INI);		// ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Ç�oï¿½ï¿½
	int l_ByteSize = _ttoi(l_tszText);
	::GetPrivateProfileString(_T("COM"), _T("Parity"), _T("0"), l_tszText, sizeof(l_tszText), MOT_INI);		// ï¿½pï¿½ï¿½ï¿½eï¿½Bï¿½Ç�oï¿½ï¿½
	int l_Parity = _ttoi(l_tszText);
	switch (l_Parity) {
	case 0:	l_Parity = NOPARITY; break;
	case 1:	l_Parity = ODDPARITY; break;
	case 2:	l_Parity = EVENPARITY; break;
	case 3:	l_Parity = MARKPARITY; break;
	case 4:	l_Parity = SPACEPARITY; break;
	}
	::GetPrivateProfileString(_T("COM"), _T("StopBits"), _T("0"), l_tszText, sizeof(l_tszText), MOT_INI);		// ï¿½Xï¿½gï¿½bï¿½vï¿½rï¿½bï¿½gï¿½Ç�oï¿½ï¿½
	int l_StopBits = _ttoi(l_tszText);
	switch (l_StopBits) {
	case 0:	l_StopBits = ONESTOPBIT; break;
	case 1:	l_StopBits = ONE5STOPBITS; break;
	case 2:	l_StopBits = TWOSTOPBITS; break;
	}
	if (m_pSio->Initialize(l_BaudRate, l_ByteSize, l_Parity, l_StopBits) != TRUE) {
		_stprintf(l_tszText, _T("Communication port COM%01d is invalid."), l_PortCh);
		if (0 == m_bOnClosing) {
			::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
		return FALSE;
	}

	return TRUE;
}

// ï¿½Vï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½Ê�Mï¿½Ìƒï¿½ï¿½Oï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
BOOL CMotSysDlg::InitializeCommLog()
{
	TCHAR	l_tszText[_MAX_PATH] = _T("");
	FILE	*fp;
	TCHAR	l_tsztmpfile[_MAX_FNAME] = _T("DCXXXXXX");
	BOOL	l_NoFile = TRUE;
	TCHAR	l_tszFilePath[_MAX_PATH];

	// ï¿½ï¿½ï¿½Oï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½æ“¾
	::GetPrivateProfileString(_T("ComLog"), _T("Path"), _T("None"), l_tszText, sizeof(l_tszText), MOT_INI);
	if ((_tcscmp(l_tszText, "None") == 0) || (l_tszText[0] == 0x00)) {
		// ï¿½İ’è‚ªï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½
		l_NoFile = FALSE;
	} else {
		if (_tmktemp(l_tsztmpfile) != NULL) {
			_tcscpy(l_tszFilePath, l_tszText);
			_tcscat(l_tszText, l_tsztmpfile);
			if ((fp = _tfopen(l_tszText, "w")) != NULL) {
				fclose(fp);
				_tremove(l_tszText);
			} else {
				// ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½s
				l_NoFile = FALSE;
			}
		} else {
			// temp ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ì�¬ï¿½ï¿½ï¿½s
			l_NoFile = FALSE;
		}
	}
	if (l_NoFile == FALSE) {
		// MOT_INI ï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ê’uï¿½É�ì�¬
		TCHAR l_tszDrive[8], l_tszDir[_MAX_PATH], l_tszFName[_MAX_FNAME], l_tszExt[8];
		_tsplitpath(MOT_INI, l_tszDrive, l_tszDir, l_tszFName, l_tszExt);
		// ï¿½pï¿½Xï¿½Æƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�¬
		_stprintf(l_tszFilePath, _T("%s%s"), l_tszDrive, l_tszDir);
	}

	// ï¿½ï¿½ï¿½Oï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½æ“¾
	::GetPrivateProfileString(_T("ComLog"), _T("FileName"), COM_LOG_FILE, l_tszText, sizeof(l_tszText), MOT_INI);
	if (_tcscmp(l_tszText, COM_LOG_FILE) != 0) {
		_tcscpy(l_tsztmpfile, l_tszFilePath);
		_tcscat(l_tsztmpfile, l_tszText);
		if ((fp = _tfopen(l_tsztmpfile, "w")) != NULL) {
			fclose(fp);
			_tremove(l_tsztmpfile);
		} else {
			// ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½s
			l_NoFile = FALSE;
			_tcscpy(l_tsztmpfile, l_tszFilePath);
			_tcscat(l_tsztmpfile, COM_LOG_FILE);
		}
	}

	// ï¿½Ê�Mï¿½ï¿½ï¿½Oï¿½Nï¿½ï¿½ï¿½Xï¿½ì�¬
	m_pComLogFile = g_pComLogFile = new CLogFile(l_tszText);
	if (m_pComLogFile == NULL) {
		return FALSE;
	}
	_tcscpy(m_pComLogFile->m_tszLogFilePath, l_tszFilePath);

	return TRUE;
}

// ï¿½Tï¿½[ï¿½{ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½
LRESULT CMotSysDlg::OnServoInitComp(WPARAM wParam, LPARAM lParam)
{
	BOOL l_bRc = (BOOL) wParam;

	if (0 == wParam) {
		// ï¿½ï¿½ï¿½s
		;	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½D
	} else {
		// ï¿½ï¿½ï¿½ï¿½
		switch (m_AutoHide) {
		case 0:
			break;
		case 1:
			ShowWindow(SW_MINIMIZE);
			break;
		case 2:
		default:
			ShowWindow(SW_MINIMIZE);
			ShowWindow(SW_HIDE);
			break;
		}
	}
	return 0;
}

// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
void CMotSysDlg::ShowAlarm(WORD Mode, WORD Axis, WORD Code1, WORD Code2)
{
	WPARAM	l_WParam;
	LPARAM	l_LParam;

	l_WParam = MAKEWPARAM(Axis, Mode);
	l_LParam = MAKELPARAM(Code2, Code1);

	// ï¿½Ù�ï¿½ï¿½Ê’mï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
	send_motion_message(-1, Mode);

	PostMessage(MOTDRV_SHOWALARM, l_WParam, l_LParam);
}
// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½
LRESULT CMotSysDlg::OnShowAlarm(WPARAM wParam, LPARAM lParam)
{
	//							 ----Motion System Driver Error----
	TCHAR	l_tszText0[] = "Alarm\n----------------------------------\n";
	TCHAR	l_tszText1[255];
	TCHAR	l_tszText2[32];
	TCHAR	l_tszText3[32];
/* added 2010.02.24 hmenjo MotSys Y ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- { ---------- */
	TCHAR	l_tszText4[32];
/* added 2010.02.24 hmenjo MotSys Y ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- } ---------- */
	TCHAR	l_tszText5[255];
	WORD	l_Mode	= HIWORD(wParam);
	WORD	l_Axis	= LOWORD(wParam);
	WORD	l_Code1 = HIWORD(lParam);
	WORD	l_Code2 = LOWORD(lParam);
	int		l_MsgID;
	int		l_ret;
	TCHAR	l_tszHomeErrorMessage[255] = _T("Axis can not go home.");
	TCHAR	tszAlarmLog[255] = _T("Motion System Driver Alarm.");

	switch (l_Mode) {
	case 1:		// ï¿½Xï¿½eï¿½[ï¿½Wï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ WD ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		_tcscpy(l_tszText1, l_tszText0);
		_tcscat(l_tszText1, _T("W.D. Error on Controller.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" W.D. Error on Controller."));
		break;
	case 2:		// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½Fï¿½X
		_tcscpy(l_tszText1, l_tszText0);
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" System Error."));
		break;
	case 3:		// ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Fï¿½X
		switch (l_Axis) {
		case AXIS_BIG_X:		_tcscpy(l_tszText2, _T("Big X"));			break;	// Big X
		case AXIS_BIG_Y:		_tcscpy(l_tszText2, _T("Big YM"));			break;	// Big Y
		case AXIS_Z:			_tcscpy(l_tszText2, _T("Z(Elevator)"));		break;	// Z(Elevator)
		case AXIS_SLOW_X:		_tcscpy(l_tszText2, _T("X slow"));			break;	// X slow
		case AXIS_T:			_tcscpy(l_tszText2, _T("Theta"));			break;	// Theta
		case AXIS_L:			_tcscpy(l_tszText2, _T("Level"));			break;	// Level
		case AXIS_FAST_X:		_tcscpy(l_tszText2, _T("X fast"));			break;	// X fast
		default:	_tcscpy(l_tszText2, _T("Undefine"));		break;	// Undefine
		}
		CnvWordToBin(l_Code1, l_tszText3, 2);
		_stprintf(l_tszText1, _T("%sAlarm on Axis[%d] (%s).\nAlarm code : %s"), l_tszText0, l_Axis, l_tszText2, l_tszText3);
		if (l_Axis == AXIS_BIG_Y) {
			// Y ï¿½ï¿½ï¿½Ì�ê�‡ï¿½ï¿½ YS ï¿½ï¿½ï¿½\ï¿½ï¿½
/* modified 2010.02.24 hmenjo MotSys Y ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- { ---------- */
//			CnvWordToBin(l_Code2, l_tszText3, 2);
//			_stprintf(l_tszText5, _T("\nAlarm on Axis[%d] (%s).\nAlarm code : %s"), l_Axis, _T("Big YS"), l_tszText3);
/* modified 2010.02.24 hmenjo MotSys Y ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ----------			  */
			CnvWordToBin(l_Code2, l_tszText4, 2);
			_stprintf(l_tszText5, _T("\nAlarm on Axis[%d] (%s).\nAlarm code : %s"), l_Axis, _T("Big YS"), l_tszText4);
/* modified 2010.02.24 hmenjo MotSys Y ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- } ---------- */
			_tcscat(l_tszText1, l_tszText5);
		}
		l_MsgID = 1;
		// AlarmLog Message
		TCHAR tszTemp[256];
/* modified 2010.02.24 hmenjo MotSys Y ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- { ---------- */
//		  _stprintf(tszTemp, _T(" Alarm on Axis (%s). Alarm code : %s."), l_tszText2, l_tszText3);
/* modified 2010.02.24 hmenjo MotSys Y ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ----------			  */
		_stprintf(tszTemp, _T(" Alarm on Axis (%sYS). Alarm code : %s    %s."), l_tszText2, l_tszText3, l_tszText4);
/* modified 2010.02.24 hmenjo MotSys Y ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- } ---------- */
		_tcscat(tszAlarmLog, tszTemp);
		break;
	case 4:		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½Oï¿½É‚È‚ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½
		_tcscpy(l_tszText1, l_tszText0);
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("Not be initialized on Controller.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" System Error. Not be Initialized on Controller."));
		break;
	case 5:		// DIO ï¿½Gï¿½ï¿½ï¿½[
		_tcscpy(l_tszText1, _T("DIO error.\n----------------------------------\n"));
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("Not work DIO board.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" DIO error."));
		break;
	case 6:		// ï¿½Tï¿½[ï¿½{ ON/OFF ï¿½Gï¿½ï¿½ï¿½[
		_tcscpy(l_tszText1, _T("Servo ON/OFF timeout.\n----------------------------------\n"));
		_tcscat(l_tszText1, _T("System Error.\n"));
		if (l_Code1 == 0) {
			_tcscat(l_tszText1, _T("Not Servo OFF on Controller.\n"));
		} else {
			_tcscat(l_tszText1, _T("Not Servo ON on Controller.\n"));
		}
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" Servo ON/OFF timeout."));
		break;
	case 7:		// ï¿½ï¿½ï¿½ï¿½~ ï¿½Gï¿½ï¿½ï¿½[
		_tcscpy(l_tszText1, _T("EMG error.\n----------------------------------\n"));
		_tcscat(l_tszText1, _T("EMG button.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" EMG error."));
		break;
	case 8: 	// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A ï¿½Gï¿½ï¿½ï¿½[	 ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½sï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½
		switch(l_Code1){
		case MS_PARAMETER_ERROR:
			_tcscat(l_tszHomeErrorMessage, _T(" Parameter error. "));
			break;
		case MS_AXIS_LOCKED:
			_tcscat(l_tszHomeErrorMessage, _T(" Axis locked. "));
			break;
		case MS_AXIS_UNINITIALIZED:
			_tcscat(l_tszHomeErrorMessage, _T(" Uninitialized. "));
			break;
		case MS_FUNCTION_BUSY:
			_tcscat(l_tszHomeErrorMessage, _T(" Function busy. "));
			break;
		default:
			_tcscat(l_tszHomeErrorMessage, _T(" Error. "));
			break;
		}
		_stprintf(l_tszText1, _T("%sAxis=%d."), l_tszHomeErrorMessage, l_Axis);
		l_MsgID = 2;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" Axis can not go home."));
		break;
	default:
		break;
	}
	AlarmLogging(tszAlarmLog);

	switch (l_MsgID) {
	case 0:
		if (0 == m_bOnClosing) {
			static int ls_iMsgID_0 = 0;
			if (0 == ls_iMsgID_0) {ls_iMsgID_0 = 1;} else {break;}
			g_CC_CommAbort = TRUE;
			l_ret = ::MessageBox(NULL, l_tszText1, MSD_MESSAGEBOX_TITLE,
																MB_OK
															  | MB_ICONQUESTION
															  | MB_DEFBUTTON1
															  | MB_SYSTEMMODAL);
			g_CC_CommAbort = FALSE;
			if (l_ret == IDOK) {
				PostMessage(WM_CLOSE, 0, 0);
			} else {
				if (l_Mode == 6) {
					g_CC_ServoONOFFerr = FALSE;
				}
			}
			ls_iMsgID_0 = 0;
		}
		break;
	case 1:
		if (0 == m_bOnClosing) {
/* deleted 2009.06.02 hmenjo ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Å‚ï¿½ COM ï¿½Ê�Mï¿½ï¿½ï¿½fï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//			static ls_iMsgID_1 = 0;
//			if (0 == ls_iMsgID_1) {ls_iMsgID_1 = 1;} else {break;}
//			g_CC_CommAbort = TRUE;
/* deleted 2009.06.02 hmenjo ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Å‚ï¿½ COM ï¿½Ê�Mï¿½ï¿½ï¿½fï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
			l_ret = ::MessageBox(NULL, l_tszText1, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
/* deleted 2009.06.02 hmenjo ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Å‚ï¿½ COM ï¿½Ê�Mï¿½ï¿½ï¿½fï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//			g_CC_CommAbort = FALSE;
/* deleted 2009.06.02 hmenjo ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Å‚ï¿½ COM ï¿½Ê�Mï¿½ï¿½ï¿½fï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
			if (l_Mode == 3) {
				g_CC_bClearAlarm[CC_CnvAxisToSPT(l_Axis)] = TRUE;
			}
/* deleted 2009.06.02 hmenjo ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Å‚ï¿½ COM ï¿½Ê�Mï¿½ï¿½ï¿½fï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//			ls_iMsgID_1 = 0;
/* deleted 2009.06.02 hmenjo ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Å‚ï¿½ COM ï¿½Ê�Mï¿½ï¿½ï¿½fï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
		}
		break;
	default:
		if (0 == m_bOnClosing) {
			static int ls_iMsgID_def = 0;
			if (0 == ls_iMsgID_def) {ls_iMsgID_def = 1;} else {break;}
			g_CC_CommAbort = TRUE;
			l_ret = ::MessageBox(NULL, l_tszText1, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			g_CC_CommAbort = FALSE;
			ls_iMsgID_def = 0;
		}
		break;
	}
	return 0;
}

// ï¿½ï¿½ï¿½[ï¿½hï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Qï¿½iï¿½ï¿½ï¿½\ï¿½Lï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½ï¿½
//	mode ï¿½É‚ï¿½è•ªï¿½ï¿½ï¿½Å‚ï¿½ï¿½ï¿½D
//		=0 : ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
//		=1 : 8 ï¿½rï¿½bï¿½gï¿½ï¿½ï¿½É•ï¿½ï¿½ï¿½
//		=2 : 4 ï¿½rï¿½bï¿½gï¿½ï¿½ï¿½É•ï¿½ï¿½ï¿½
void CMotSysDlg::CnvWordToBin(
		WORD WordData,		// 16 bit ï¿½fï¿½[ï¿½^
		TCHAR *ptszBinary,	// ï¿½ÏŠï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		WORD mode			// ï¿½ï¿½ï¿½[ï¿½h(ï¿½ï¿½Lï¿½Qï¿½ï¿½)
	)
{
	TCHAR	l_tszText1[32];
	TCHAR	l_tszText2[32];

	_ltot(WordData, l_tszText1, 2);
	memset(l_tszText2, 0, sizeof(l_tszText2));
	memcpy(l_tszText2, _T("0000000000000000"), 16 - strlen(l_tszText1));
	_tcscat(l_tszText2, l_tszText1);
	switch (mode) {
	case 1:
		_tcscpy(l_tszText1, _T("00000000 00000000"));
		memcpy(&l_tszText1[0], &l_tszText2[0], 8);
		memcpy(&l_tszText1[9], &l_tszText2[8], 8);
		break;
	case 2:
		_tcscpy(l_tszText1, _T("0000 0000 0000 0000"));
		memcpy(&l_tszText1[0],	&l_tszText2[0],  4);
		memcpy(&l_tszText1[5],	&l_tszText2[4],  4);
		memcpy(&l_tszText1[10], &l_tszText2[8],  4);
		memcpy(&l_tszText1[15], &l_tszText2[12], 4);
		break;
	case 0:
	default:
		_tcscpy(l_tszText1, l_tszText2);
		break;
	}
	_tcscpy(ptszBinary, l_tszText1);
}

BOOL CMotSysDlg::DestroyWindow()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉŒÅ—Lï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚©ï¿½Aï¿½Ü‚ï¿½ï¿½ÍŠï¿½{ï¿½Nï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	// ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½Ìƒvï¿½ï¿½ï¿½pï¿½eï¿½Bï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½ç�¯ï¿½Ê�ï¿½ï¿½ï¿½ï¿½í�œ
	::RemoveProp(m_hWnd, IDENT_CODE);

	return CDialog::DestroyWindow();
}

// ï¿½Tï¿½[ï¿½{ OFF
void CMotSysDlg::OnBTNServoOFF()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	g_CC_ServoOFFReq = TRUE;
}

// ï¿½Tï¿½[ï¿½{ ON
void CMotSysDlg::OnBTNServoON()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	g_CC_ServoONReq = TRUE;
}

// ï¿½Tï¿½[ï¿½{ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Ç�ï¿½ï¿½ï¿½
void CMotSysDlg::OnBtnServoParam()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	CC_LoadServoParam();

}

// X ï¿½ï¿½ ï¿½Ê’uï¿½Î�ï¿½ï¿½sï¿½[ï¿½Nï¿½lï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
void CMotSysDlg::OnBtnDPosPeakClearX()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_BIG_X], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_BIG_X], _T("0"));
	m_dPos[1][AXIS_BIG_X] = m_dPos[2][AXIS_BIG_X] = 0;
}
// YM ï¿½ï¿½ ï¿½Ê’uï¿½Î�ï¿½ï¿½sï¿½[ï¿½Nï¿½lï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
void CMotSysDlg::OnBtnDPosPeakClearYM()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_BIG_Y], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_BIG_Y], _T("0"));
	m_dPos[1][AXIS_BIG_Y] = m_dPos[2][AXIS_BIG_Y] = 0;
}
// YS ï¿½ï¿½ ï¿½Ê’uï¿½Î�ï¿½ï¿½sï¿½[ï¿½Nï¿½lï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
void CMotSysDlg::OnBtnDPosPeakClearYS()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_NUM], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_NUM], _T("0"));
	m_dPos[1][AXIS_NUM] = m_dPos[2][AXIS_NUM] = 0;
}
// Z ï¿½ï¿½ ï¿½Ê’uï¿½Î�ï¿½ï¿½sï¿½[ï¿½Nï¿½lï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
void CMotSysDlg::OnBtnDPosPeakClearZ()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_Z], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_Z], _T("0"));
	m_dPos[1][AXIS_Z] = m_dPos[2][AXIS_Z] = 0;
}
// T ï¿½ï¿½ ï¿½Ê’uï¿½Î�ï¿½ï¿½sï¿½[ï¿½Nï¿½lï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
void CMotSysDlg::OnBtnDPosPeakClearT()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_T], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_T], _T("0"));
	m_dPos[1][AXIS_T] = m_dPos[2][AXIS_T] = 0;
}
// ï¿½eï¿½ï¿½ï¿½Ê’uï¿½Î�ï¿½ï¿½ï¿½\ï¿½ï¿½
void CMotSysDlg::DispdPos(WORD wAxis, long d_Pos)
{
	TCHAR	l_tszText[32];
//	short	l_sAxis = ConvAxis(wAxis);		// ï¿½sï¿½vï¿½Ì‚Í‚ï¿½
	short	l_sAxis = wAxis;

	// ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½lï¿½ï¿½ï¿½Xï¿½V
	m_dPos[0][l_sAxis] = d_Pos;
	_stprintf(l_tszText, _T("%11d"), m_dPos[0][l_sAxis]);
	SetDlgItemText(g_iDlgCtrlID[17][l_sAxis], l_tszText);
	// ï¿½|ï¿½ï¿½ï¿½sï¿½[ï¿½Nï¿½lï¿½ï¿½ï¿½Xï¿½V
	if (m_dPos[0][l_sAxis] < m_dPos[1][l_sAxis]) {
		m_dPos[1][l_sAxis] = m_dPos[0][l_sAxis];
		_stprintf(l_tszText, _T("%11d"), m_dPos[1][l_sAxis]);
		SetDlgItemText(g_iDlgCtrlID[19][l_sAxis], l_tszText);
	}
	// ï¿½{ï¿½ï¿½ï¿½sï¿½[ï¿½Nï¿½lï¿½ï¿½ï¿½Xï¿½V
	if (m_dPos[2][l_sAxis] < m_dPos[0][l_sAxis]) {
		m_dPos[2][l_sAxis] = m_dPos[0][l_sAxis];
		_stprintf(l_tszText, _T("%11d"), m_dPos[2][l_sAxis]);
		SetDlgItemText(g_iDlgCtrlID[20][l_sAxis], l_tszText);
	}
}


// ï¿½È‰ï¿½ï¿½ÌƒRï¿½[ï¿½hï¿½Íƒfï¿½oï¿½bï¿½Oï¿½p

// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A
void CMotSysDlg::OnBTNORGtest()		// ï¿½uX ORGï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGX(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestY()		// ï¿½uY ORGï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGY(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestZ()		// ï¿½uZ ORGï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGZ(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestT()		// ï¿½uT ORGï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGT(0);	//debdeb
}
// ABS ï¿½Ú“ï¿½
void CMotSysDlg::OnBTNABStestX()		// ï¿½uX ABS1ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGX(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestX2()		// ï¿½uX ABS2ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGX(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestY()		// ï¿½uY ABS1ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGY(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestY2()		// ï¿½uY ABS2ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGY(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestZ()		// ï¿½uZ ABS1ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGZ(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestZ2()		// ï¿½uZ ABS2ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGZ(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestT()		// ï¿½uT ABS1ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGT(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestT2()		// ï¿½uT ABS2ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGT(21);	//debdeb
}
// INC ï¿½Ú“ï¿½
void CMotSysDlg::OnBTNINCtestX()		// ï¿½uX INC+ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGX(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestX2()		// ï¿½uX INC-ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGX(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestY()		// ï¿½uY INC+ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGY(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestY2()		// ï¿½uY INC-ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGY(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestZ()		// ï¿½uZ INC+ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGZ(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestZ2()		// ï¿½uZ INC-ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGZ(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestT()		// ï¿½uT INC+ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGT(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestT2()		// ï¿½uT INC-ï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGT(31);	//debdeb
}
// HP ï¿½Ú“ï¿½
void CMotSysDlg::OnBTNHPtestX()		// ï¿½uX HPï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGX(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestY()		// ï¿½uY HPï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGY(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestZ()		// ï¿½uZ HPï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGZ(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestT()		// ï¿½uT HPï¿½vï¿½{ï¿½^ï¿½ï¿½
{
	mmtestORGT(1);	//debdeb
}

// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ Enableï¿½ï¿½ï¿½ï¿½
void CMotSysDlg::EnableDlgCtrl()
{
	int l_iCtrlID[][9] = {	// ï¿½eï¿½ï¿½ï¿½ï¿½ï¿½Æ‚É�Å‘å�€ï¿½Ú�ï¿½ï¿½Í‚Tï¿½Oï¿½Â‚Å‚ï¿½ï¿½D(g_iDlgCtrlID[][]ï¿½Å’ï¿½`)
	/*00*/	IDC_STATIC_LBL_STS_X,	IDC_STATIC_LBL_STS_YM,	IDC_STATIC_LBL_STS_Z,	0,	IDC_STATIC_LBL_STS_T,	0,	0,	0,	IDC_STATIC_LBL_STS_YS,
	/*01*/	IDC_STATIC_AXIS_X,		IDC_STATIC_AXIS_YM,		IDC_STATIC_AXIS_Z,		0,	IDC_STATIC_AXIS_T,		0,	0,	0,	IDC_STATIC_AXIS_YS,
	/*02*/	IDC_STATIC_MODE_X,		IDC_STATIC_MODE_YM,		IDC_STATIC_MODE_Z,		0,	IDC_STATIC_MODE_T,		0,	0,	0,	0,
	/*03*/	IDC_STATIC_POS_X,		IDC_STATIC_POS_YM,		IDC_STATIC_POS_Z,		0,	IDC_STATIC_POS_T,		0,	0,	0,	IDC_STATIC_POS_YS,
	/*04*/	IDC_BTN_HPtestX,		IDC_BTN_HPtestY,		IDC_BTN_HPtestZ,		0,	IDC_BTN_HPtestT,		0,	0,	0,	0,
	/*05*/	IDC_BTN_INCtestX2,		IDC_BTN_INCtestY2,		IDC_BTN_INCtestZ2,		0,	IDC_BTN_INCtestT2,		0,	0,	0,	0,
	/*06*/	IDC_BTN_INCtestX,		IDC_BTN_INCtestY,		IDC_BTN_INCtestZ,		0,	IDC_BTN_INCtestT,		0,	0,	0,	0,
	/*07*/	IDC_BTN_ABStestX2,		IDC_BTN_ABStestY2,		IDC_BTN_ABStestZ2,		0,	IDC_BTN_ABStestT2,		0,	0,	0,	0,
	/*08*/	IDC_BTN_ABStestX,		IDC_BTN_ABStestY,		IDC_BTN_ABStestZ,		0,	IDC_BTN_ABStestT,		0,	0,	0,	0,
	/*09*/	IDC_BTN_ORGtestX,		IDC_BTN_ORGtestY,		IDC_BTN_ORGtestZ,		0,	IDC_BTN_ORGtestT,		0,	0,	0,	0,
	/*10*/	IDC_STATIC_LBL_PTN_X,	IDC_STATIC_LBL_PTN_Y,	IDC_STATIC_LBL_PTN_Z,	0,	IDC_STATIC_LBL_PTN_T,	0,	0,	0,	0,
	/*11*/	IDC_STATIC_PTN_X,		IDC_STATIC_PTN_Y,		IDC_STATIC_PTN_Z,		0,	IDC_STATIC_PTN_T,		0,	0,	0,	0,
	/*12*/	IDC_STATIC_PRC_X,		IDC_STATIC_PRC_Y,		IDC_STATIC_PRC_Z,		0,	IDC_STATIC_PRC_T,		0,	0,	0,	0,
	/*13*/	IDC_STATIC_SOFTHP_X,	IDC_STATIC_SOFTHP_Y,	IDC_STATIC_SOFTHP_Z,	0,	IDC_STATIC_SOFTHP_T,	0,	0,	0,	0,
	/*14*/	IDC_STATIC_SPEED_X,		IDC_STATIC_SPEED_Y,		IDC_STATIC_SPEED_Z,		0,	IDC_STATIC_SPEED_T,		0,	0,	0,	0,
	/*15*/	IDC_STATIC_DESTPOS_X,	IDC_STATIC_DESTPOS_Y,	IDC_STATIC_DESTPOS_Z,	0,	IDC_STATIC_DESTPOS_T,	0,	0,	0,	0,
	/*16*/	IDC_STATIC_LBL_POS_X,	IDC_STATIC_LBL_POS_YM,	IDC_STATIC_LBL_POS_Z,	0,	IDC_STATIC_LBL_POS_T,	0,	0,	0,	IDC_STATIC_LBL_POS_YS,
	/*17*/	IDC_STATIC_DPOSR_X,		IDC_STATIC_DPOSR_YM,	IDC_STATIC_DPOSR_Z,		0,	IDC_STATIC_DPOSR_T,		0,	0,	0,	IDC_STATIC_DPOSR_YS,
	/*18*/	IDC_BTN_DPOSP_CLR_X,	IDC_BTN_DPOSP_CLR_YM,	IDC_BTN_DPOSP_CLR_Z,	0,	IDC_BTN_DPOSP_CLR_T,	0,	0,	0,	IDC_BTN_DPOSP_CLR_YS,
	/*19*/	IDC_STATIC_DPOSPM_X,	IDC_STATIC_DPOSPM_YM,	IDC_STATIC_DPOSPM_Z,	0,	IDC_STATIC_DPOSPM_T,	0,	0,	0,	IDC_STATIC_DPOSPM_YS,
	/*20*/	IDC_STATIC_DPOSPP_X,	IDC_STATIC_DPOSPP_YM,	IDC_STATIC_DPOSPP_Z,	0,	IDC_STATIC_DPOSPP_T,	0,	0,	0,	IDC_STATIC_DPOSPP_YS,
			-1,						-1,						-1,						-1,	-1,						-1,	-1,	-1,	-1,
		};

	int i, j;

	// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ ID ï¿½eï¿½[ï¿½uï¿½ï¿½ï¿½ï¿½İ’ï¿½
	memset(g_iDlgCtrlID, 0, sizeof(g_iDlgCtrlID));
	i = 0;
	while (-1 != l_iCtrlID[i][0]) {
#if CC_DEB_BUTTON==0
		if ((4 <= i) && (i <= 9)) {
			i++;
			continue;
		}
#endif
		g_iDlgCtrlID[i][AXIS_BIG_X]	= l_iCtrlID[i][0];
		g_iDlgCtrlID[i][AXIS_BIG_Y]	= l_iCtrlID[i][1];
		g_iDlgCtrlID[i][AXIS_Z]		= l_iCtrlID[i][2];
		g_iDlgCtrlID[i][AXIS_T]		= l_iCtrlID[i][4];
		g_iDlgCtrlID[i][AXIS_NUM]	= l_iCtrlID[i][8];
		i++;
	}
	for (j = 0; j <= AXIS_NUM; j++) {
		g_iDlgCtrlID[i][j] = -1;
	}

	// Enable ï¿½ï¿½ï¿½ï¿½
	if (0 != motion_data[AXIS_BIG_X].bEnable) {
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_BIG_X]) {
			if (0 != g_iDlgCtrlID[i][AXIS_BIG_X]) {
				GetDlgItem(g_iDlgCtrlID[i][AXIS_BIG_X])->EnableWindow(TRUE);
			}
			i++;
		}
	}
	if (0 != motion_data[AXIS_BIG_Y].bEnable) {
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_BIG_Y]) {
			if (0 != g_iDlgCtrlID[i][AXIS_BIG_Y]) {
				GetDlgItem(g_iDlgCtrlID[i][AXIS_BIG_Y])->EnableWindow(TRUE);
			}
			i++;
		}
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_NUM]) {
			if (0 != g_iDlgCtrlID[i][AXIS_NUM]) {
				GetDlgItem(g_iDlgCtrlID[i][AXIS_NUM])->EnableWindow(TRUE);
			}
			i++;
		}
	}
	if (0 != motion_data[AXIS_Z].bEnable) {
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_Z]) {
			if (0 != g_iDlgCtrlID[i][AXIS_Z]) {
				GetDlgItem(g_iDlgCtrlID[i][2])->EnableWindow(TRUE);
			}
			i++;
		}
	}
	if (0 != motion_data[AXIS_T].bEnable) {
		i = 0;
		while (-1 != g_iDlgCtrlID[i][AXIS_T]) {
			if (0 != g_iDlgCtrlID[i][AXIS_T]) {
				GetDlgItem(g_iDlgCtrlID[i][4])->EnableWindow(TRUE);
			}
			i++;
		}
	}
}

//	ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½pï¿½ï¿½ï¿½O
void CMotSysDlg::AlarmLogging(TCHAR *ptszAlarmLog)
{
	if (0 == m_pAlarmLogFile) {
		return;
	}

	TCHAR	l_tszLogBuff[1024] = _T("");

	// ï¿½ï¿½ï¿½tï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½
	m_pAlarmLogFile->AddTime(l_tszLogBuff, 2);
	_tcscat(l_tszLogBuff, _T(" "));
	_tcscat(l_tszLogBuff, ptszAlarmLog);

	m_pAlarmLogFile->Logging(l_tszLogBuff);
}

/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- { ---------- */
void CMotSysDlg::DioLogging(DWORD dwDi, DWORD dwDo)
{
	if (0 == m_pcDioLog) {
		return;
	}

	TCHAR l_tszLogBuff[1024] = _T("");
	WORD l_wDiLData = LOWORD(dwDi);
	WORD l_wDiHData = HIWORD(dwDi);
	WORD l_wDoLData = LOWORD(dwDo);
	WORD l_wDoHData = HIWORD(dwDo);
	TCHAR l_tszDiLData[32];
	TCHAR l_tszDiHData[32];
	TCHAR l_tszDoLData[32];
	TCHAR l_tszDoHData[32];
	this->CnvWordToBin(l_wDiLData, l_tszDiLData, 1);
	this->CnvWordToBin(l_wDiHData, l_tszDiHData, 1);
	this->CnvWordToBin(l_wDoLData, l_tszDoLData, 1);
	this->CnvWordToBin(l_wDoHData, l_tszDoHData, 1);

	/* ï¿½ï¿½ï¿½tï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½	*/
	m_pcDioLog->AddTime(l_tszLogBuff, 4);
	_tcscat(l_tszLogBuff, _T(" "));

	_tcscat(l_tszLogBuff, l_tszDiHData);
	_tcscat(l_tszLogBuff, _T(" "));
	_tcscat(l_tszLogBuff, l_tszDiLData);
	_tcscat(l_tszLogBuff, _T("  "));
	_tcscat(l_tszLogBuff, l_tszDoHData);
	_tcscat(l_tszLogBuff, _T(" "));
	_tcscat(l_tszLogBuff, l_tszDoLData);

	m_pcDioLog->Logging(l_tszLogBuff);
}
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- } ---------- */
