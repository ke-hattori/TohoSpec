// MotSysDlg.cpp : �C���v�������e�[�V���� �t�@�C��
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

UINT MOTDRV_SHOWALARM = RegisterWindowMessage("MOTDRV_SHOWALARM");	// �A���[���\���p���b�Z�[�W��`
UINT MOTDRV_SRVINITCOMP = RegisterWindowMessage("MOTDRV_SRVINITCOMP");	// �T�[�{�������������b�Z�[�W��`

CSio *g_pCSio;
CLogFile *g_pComLogFile;
CWinThread* g_pCCThread;	// Thread pointer

#define	ID_MSD_BASE_TIMER	101	// OnTimer ID
#define	MSD_BASE_TIME		200	// [ms]


/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
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

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	// ���̃_�C�A���O�p�̃A�C�R����ݒ�
	SetIcon(((CMotSysDlg*) (this->GetParent()))->m_hMainIcon, TRUE);	// �傫���A�C�R����ݒ�

	if (g_pCSio != NULL) {
		if (g_pCSio->m_LogEnable == FALSE) {
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Disable"));
		} else {
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Enable"));
		}
	}

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
				  // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// ���O�N���A�{�^��
void CAboutDlg::OnBtnComLogClear()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if (g_pCSio != NULL) {
		GetDlgItem(IDC_BTN_COMLOGENA)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_COMLOGCLR)->EnableWindow(FALSE);
		g_pCSio->ClearLog();
		GetDlgItem(IDC_BTN_COMLOGCLR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_COMLOGENA)->EnableWindow(TRUE);
	}
}

// ���O�̋���/�֎~
void CAboutDlg::OnBtnComLogEnable()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if (g_pCSio != NULL) {
		if (g_pCSio->m_LogEnable == FALSE) {
			g_pCSio->m_LogEnable = TRUE;
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Enable"));
			// Servo.ini �ɋL�����Ă���
			::WritePrivateProfileString(_T("Log"), _T("ComLog"), _T("1"), SERVO_INI);
		} else {
			g_pCSio->m_LogEnable = FALSE;
			SetDlgItemText(IDC_BTN_COMLOGENA, _T("Disable"));
			// Servo.ini �ɋL�����Ă���
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
// CMotSysDlg �_�C�A���O

CMotSysDlg::CMotSysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotSysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotSysDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMotSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotSysDlg)
		// ����: ���̏ꏊ�ɂ� ClassWizard �ɂ���� DDX �� DDV �̌Ăяo�����ǉ�����܂��B
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
	ON_REGISTERED_MESSAGE(MOTDRV_HIDEINDOW, OnBtnHide)
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
// CMotSysDlg ���b�Z�[�W �n���h��

BOOL CMotSysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
//	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	// ���̍s�������ƃV�X�e���������16x16�A�C�R����T���Ă����D
	// �������C16x16�A�C�R���͗\�ߍ쐬���Ă����K�v������D

	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B

	// �E�B���h�E�̃v���p�e�B���X�g�Ɏ��ʏ���ǉ�
	::SetProp(m_hWnd, IDENT_CODE, (HANDLE) 1);

	//�ϐ��̏�����
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

	// �R���g���[���̃C�l�[�u���ݒ�
	EnableDlgCtrl();

	TCHAR	l_tszText[32];
	::GetPrivateProfileString(_T("Window"), _T("AutoHide"), _T("2"), l_tszText, sizeof(l_tszText), SERVO_INI);
	m_AutoHide = _ttoi(l_tszText);
	if ((m_AutoHide < 0) || (2 < m_AutoHide)) {
		m_AutoHide = 2;
	}
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
	/* DIO ���O ����/���Ȃ��ݒ�Ǎ���	*/
	::GetPrivateProfileString(_T("DIO"), _T("Log"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	if (0 == _tcscmp(_T("1"), l_tszText)) {
		m_bDioLog = TRUE;
	} else {
		m_bDioLog = FALSE;
	}
	/* DIO ���O �t�@�C���ݒ�	*/
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
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */

	SetDlgItemText(IDC_STATIC_STATUS, _T("Initializing..."));

	// �A���[�����O�̏�����
	{
		TCHAR l_tszDrive[_MAX_PATH], l_tszDir[_MAX_PATH], l_tszFName[_MAX_FNAME], l_tszExt[8];
		TCHAR	l_tszFilePath[_MAX_PATH];
		_tsplitpath(ALARM_LOG, l_tszDrive, l_tszDir, l_tszFName, l_tszExt);
		// �p�X�ƃt�@�C�������쐬
		_stprintf(l_tszFilePath, _T("%s%s"), l_tszDrive, l_tszDir);
		m_pAlarmLogFile = new CLogFile(l_tszFName);
		if (0 != m_pAlarmLogFile) {
			_tcscpy(m_pAlarmLogFile->m_tszLogFilePath, l_tszFilePath);
		} else {
			::MessageBox(NULL, _T("Failed to create Alarm Log class"), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	}

	// DIO �I�[�v��
	if (0 == m_bInitFailed) {
		if (CC_DioOpen() != 0) {
			// �I�[�v�����s
			m_bInitFailed = TRUE;
			SetDlgItemText(IDC_STATIC_STATUS, _T("Failed to DIO opened"));
			SetDlgItemText(IDC_STATIC_COMSTAT, _T("Uninitialized"));
			::MessageBox(NULL, _T("Failed to open DIO Driver."), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		} else {
			m_bDIO_Opned = TRUE;
			// �V���A���ʐM�̃��O�̏�����
			if (InitializeCommLog() == FALSE) {
				m_bInitFailed = TRUE;
				SetDlgItemText(IDC_STATIC_STATUS, _T("Uninitialized"));
				SetDlgItemText(IDC_STATIC_COMSTAT, _T("Failed to create Comm. Log"));
				::MessageBox(NULL, _T("Failed to create Comm Log class"), MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			} else {
				// �V���A���ʐM�̏�����
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

	// �A�o�E�g�{�^���� ICON ��ݒ�
	((CButton*) GetDlgItem(IDC_BTN_ABOUT))->SetIcon(m_hIcon);

	// �E�B���h�E���őO�ʂɂ���
#ifndef _DEBUG
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#endif

	// ���̃v���Z�X�̗D��x��ݒ肷��
	HANDLE l_hCurProc = ::GetCurrentProcess();
	// �O�̗D��x���擾
	DWORD l_oldPriority = ::GetPriorityClass(l_hCurProc);
	ChgToPriorityMsg(l_oldPriority, l_tszText);
	SetDlgItemText(IDC_STATIC_PROCPRI2, l_tszText);
#if 1
//	BOOL l_err = SetPriorityClass(l_hCurProc, REALTIME_PRIORITY_CLASS);
	BOOL l_err = SetPriorityClass(l_hCurProc, HIGH_PRIORITY_CLASS);
	if (l_err == 0) {
		// �D��x�̐ؑւ����s
		DWORD l_errcode = GetLastError();
		TCHAR l_tszErrMsg[255]; ShowLastError(l_errcode, l_tszText);
		_stprintf(l_tszErrMsg, _T("Failed to change process priority.\n%s(0x%08x)"), l_tszText, l_errcode);
		if (0 == m_bOnClosing) {
			::MessageBox(NULL, l_tszErrMsg, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	}
	// �V�����D��x���擾
	DWORD l_newPriority = GetPriorityClass(l_hCurProc);
	ChgToPriorityMsg(l_newPriority, l_tszText);
	SetDlgItemText(IDC_STATIC_PROCPRI, l_tszText);
#endif

	// �^�C�}(OnTimer)���N��
	SetTimer(ID_MSD_BASE_TIMER, MSD_BASE_TIME, NULL);

	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
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

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CMotSysDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CMotSysDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMotSysDlg::OnOK()
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	// "���^�[��"�L�[�ŏI������̂�������邽��
//	CDialog::OnOK();
}

void CMotSysDlg::OnBtnAbout()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	// �A�o�E�g��\��
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

LRESULT CMotSysDlg::OnBtnHide(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	// �E�B���h�E(�_�C�A���O�x�[�X)���\���ɂ���
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CMotSysDlg::OnMotsysShowWindow(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	// �E�B���h�E(�_�C�A���O�x�[�X)��\���ɂ���
	ShowWindow(SW_SHOWNORMAL);
	return 0;
}

void CMotSysDlg::OnTimer(UINT nIDEvent)
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	switch (nIDEvent) {
	case ID_MSD_BASE_TIMER:
		{
			static BOOL l_bRunOnce = TRUE;
			if ((0 != l_bRunOnce) && (0 == m_bInitFailed)) {
				l_bRunOnce = FALSE;
				// �R���g���[���Ď��X���b�h���N��(�R���g���[������̒��������)
				g_pCCThread = AfxBeginThread(CC_Main_Loop, (CWnd*) this, THREAD_PRIORITY_NORMAL);
				g_pCCThread->m_bAutoDelete = FALSE;
			}
		}
		if (0 == m_bInitFailed) {
			// ���M/��M�����v�̐���
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
 *	���[�V�������b�Z�[�W����
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

// ���C���֐�
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
// ���[�V�����֐�
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
//	} else if (uMsg == MOTDRV_MOVEATSPEEDTODEST) {	����� MOTDRV_MOVETOPOSITION �Ɠ����Ȃ̂Ŏg��Ȃ����Ƃɂ��܂��D
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	m_bOnClosing = TRUE;

	// �T�[�{ OFF�C�S����~
	if (0 != g_pCCThread) {
		CC_ServoOFF_RunOFF();
	}

	/*	���L�̏����� GetExitCodeThread �ɂ��X���b�h�̏I���̊m�F���o���Ȃ��������߁C
		���̂悤�ȏ����ɂ��Ă���D
		���������āC���������[�N���������Ă�����C���u�[�g���K�v�����D*/

	// �\�������܂܂� MessageBox ������΃N���[�Y������
	HWND m_hwnd_MsgBox;
	do {
		if ((m_hwnd_MsgBox = ::FindWindow(NULL, MSD_MESSAGEBOX_TITLE)) != NULL) {
			::PostMessage(m_hwnd_MsgBox, WM_CLOSE, 0, 0);
			// ���b�Z�[�W�����������邽�߂ɍēx WM_CLOSE �𔭍s���COS �ɐ����n��
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

	CloseObjects();		// �e��I�u�W�F�N�g���J������

	CDialog::OnClose();
}
// �e��I�u�W�F�N�g���J������
void CMotSysDlg::CloseObjects()
{
	m_OnClose = TRUE;

	// DIO �N���[�Y
	if (0 != m_bDIO_Opned) {
		m_bDIO_Opned = FALSE;
		if (CC_DioClose() != 0) {
			;	// �N���[�Y���s
		}
	}

	// �V���A���ʐM�̊J��
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

// �D��x�R�[�h�𕶎���ɕϊ�����
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

// �G���[�R�[�h����V�X�e���̃G���[���b�Z�[�W���擾����
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

// �V���A���ʐM�̏�����
BOOL CMotSysDlg::InitializeComm()
{
	TCHAR	l_tszText[255];
	::GetPrivateProfileString(_T("COM"), _T("No"), _T("3"), l_tszText, sizeof(l_tszText), MOT_INI);	// �|�[�g�ԍ��Ǐo��
	int l_PortCh = _ttoi(l_tszText);
	m_pSio = g_pCSio = new CSio(l_PortCh);	// �ʐM�N���X�쐬
	if (m_pSio == NULL) {
		return FALSE;
	}
	::GetPrivateProfileString(_T("Log"), _T("ComLog"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);	// �ʐM���O on/off
	m_pSio->m_LogEnable = _ttoi(l_tszText);
	if (m_pSio->m_LogEnable != 0) {
		m_pSio->m_LogEnable = TRUE;
	} else {
		m_pSio->m_LogEnable = FALSE;
	}
	::GetPrivateProfileString(_T("COM"), _T("BaudRate"), _T("38400"), l_tszText, sizeof(l_tszText), MOT_INI);	// �{�[���[�g�Ǐo��
	int l_BaudRate = _ttoi(l_tszText);
	::GetPrivateProfileString(_T("COM"), _T("ByteSize"), _T("8"), l_tszText, sizeof(l_tszText), MOT_INI);		// �f�[�^���Ǐo��
	int l_ByteSize = _ttoi(l_tszText);
	::GetPrivateProfileString(_T("COM"), _T("Parity"), _T("0"), l_tszText, sizeof(l_tszText), MOT_INI);		// �p���e�B�Ǐo��
	int l_Parity = _ttoi(l_tszText);
	switch (l_Parity) {
	case 0:	l_Parity = NOPARITY; break;
	case 1:	l_Parity = ODDPARITY; break;
	case 2:	l_Parity = EVENPARITY; break;
	case 3:	l_Parity = MARKPARITY; break;
	case 4:	l_Parity = SPACEPARITY; break;
	}
	::GetPrivateProfileString(_T("COM"), _T("StopBits"), _T("0"), l_tszText, sizeof(l_tszText), MOT_INI);		// �X�g�b�v�r�b�g�Ǐo��
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

// �V���A���ʐM�̃��O�̏�����
BOOL CMotSysDlg::InitializeCommLog()
{
	TCHAR	l_tszText[_MAX_PATH] = _T("");
	FILE	*fp;
	TCHAR	l_tsztmpfile[_MAX_FNAME] = _T("DCXXXXXX");
	BOOL	l_NoFile = TRUE;
	TCHAR	l_tszFilePath[_MAX_PATH];

	// ���O�f�B���N�g�����擾
	::GetPrivateProfileString(_T("ComLog"), _T("Path"), _T("None"), l_tszText, sizeof(l_tszText), MOT_INI);
	if ((_tcscmp(l_tszText, "None") == 0) || (l_tszText[0] == 0x00)) {
		// �ݒ肪�Ȃ�����
		l_NoFile = FALSE;
	} else {
		if (_tmktemp(l_tsztmpfile) != NULL) {
			_tcscpy(l_tszFilePath, l_tszText);
			_tcscat(l_tszText, l_tsztmpfile);
			if ((fp = _tfopen(l_tszText, "w")) != NULL) {
				fclose(fp);
				_tremove(l_tszText);
			} else {
				// �I�[�v�����s
				l_NoFile = FALSE;
			}
		} else {
			// temp �t�@�C���쐬���s
			l_NoFile = FALSE;
		}
	}
	if (l_NoFile == FALSE) {
		// MOT_INI �̃f�B���N�g���ʒu�ɍ쐬
		TCHAR l_tszDrive[8], l_tszDir[_MAX_PATH], l_tszFName[_MAX_FNAME], l_tszExt[8];
		_tsplitpath(MOT_INI, l_tszDrive, l_tszDir, l_tszFName, l_tszExt);
		// �p�X�ƃt�@�C�������쐬
		_stprintf(l_tszFilePath, _T("%s%s"), l_tszDrive, l_tszDir);
	}

	// ���O�t�@�C�������擾
	::GetPrivateProfileString(_T("ComLog"), _T("FileName"), COM_LOG_FILE, l_tszText, sizeof(l_tszText), MOT_INI);
	if (_tcscmp(l_tszText, COM_LOG_FILE) != 0) {
		_tcscpy(l_tsztmpfile, l_tszFilePath);
		_tcscat(l_tsztmpfile, l_tszText);
		if ((fp = _tfopen(l_tsztmpfile, "w")) != NULL) {
			fclose(fp);
			_tremove(l_tsztmpfile);
		} else {
			// �I�[�v�����s
			l_NoFile = FALSE;
			_tcscpy(l_tsztmpfile, l_tszFilePath);
			_tcscat(l_tsztmpfile, COM_LOG_FILE);
		}
	}

	// �ʐM���O�N���X�쐬
	m_pComLogFile = g_pComLogFile = new CLogFile(l_tszText);
	if (m_pComLogFile == NULL) {
		return FALSE;
	}
	_tcscpy(m_pComLogFile->m_tszLogFilePath, l_tszFilePath);

	return TRUE;
}

// �T�[�{�����������n���h��
LRESULT CMotSysDlg::OnServoInitComp(WPARAM wParam, LPARAM lParam)
{
	BOOL l_bRc = (BOOL) wParam;

	if (0 == wParam) {
		// ���s
		;	// �������܂���D
	} else {
		// ����
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

// �A���[���\������
void CMotSysDlg::ShowAlarm(WORD Mode, WORD Axis, WORD Code1, WORD Code2)
{
	WPARAM	l_WParam;
	LPARAM	l_LParam;

	l_WParam = MAKEWPARAM(Axis, Mode);
	l_LParam = MAKELPARAM(Code2, Code1);

	// �ُ��ʒm���܂��D
	send_motion_message(-1, Mode);

	PostMessage(MOTDRV_SHOWALARM, l_WParam, l_LParam);
}
// �A���[���\�����b�Z�[�W����
LRESULT CMotSysDlg::OnShowAlarm(WPARAM wParam, LPARAM lParam)
{
	//							 ----Motion System Driver Error----
	TCHAR	l_tszText0[] = "Alarm\n----------------------------------\n";
	TCHAR	l_tszText1[255];
	TCHAR	l_tszText2[32];
	TCHAR	l_tszText3[32];
/* added 2010.02.24 hmenjo MotSys Y ���A���[�����O�C�� ---------- { ---------- */
	TCHAR	l_tszText4[32];
/* added 2010.02.24 hmenjo MotSys Y ���A���[�����O�C�� ---------- } ---------- */
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
	case 1:		// �X�e�[�W�R���g���[���� WD �G���[������
		_tcscpy(l_tszText1, l_tszText0);
		_tcscat(l_tszText1, _T("W.D. Error on Controller.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" W.D. Error on Controller."));
		break;
	case 2:		// �V�X�e���G���[�F�X
		_tcscpy(l_tszText1, l_tszText0);
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" System Error."));
		break;
	case 3:		// ���A���[���F�X
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
			// Y ���̏ꍇ�� YS ���\��
/* modified 2010.02.24 hmenjo MotSys Y ���A���[�����O�C�� ---------- { ---------- */
//			CnvWordToBin(l_Code2, l_tszText3, 2);
//			_stprintf(l_tszText5, _T("\nAlarm on Axis[%d] (%s).\nAlarm code : %s"), l_Axis, _T("Big YS"), l_tszText3);
/* modified 2010.02.24 hmenjo MotSys Y ���A���[�����O�C�� ----------			  */
			CnvWordToBin(l_Code2, l_tszText4, 2);
			_stprintf(l_tszText5, _T("\nAlarm on Axis[%d] (%s).\nAlarm code : %s"), l_Axis, _T("Big YS"), l_tszText4);
/* modified 2010.02.24 hmenjo MotSys Y ���A���[�����O�C�� ---------- } ---------- */
			_tcscat(l_tszText1, l_tszText5);
		}
		l_MsgID = 1;
		// AlarmLog Message
		TCHAR tszTemp[256];
/* modified 2010.02.24 hmenjo MotSys Y ���A���[�����O�C�� ---------- { ---------- */
//		  _stprintf(tszTemp, _T(" Alarm on Axis (%s). Alarm code : %s."), l_tszText2, l_tszText3);
/* modified 2010.02.24 hmenjo MotSys Y ���A���[�����O�C�� ----------			  */
		_stprintf(tszTemp, _T(" Alarm on Axis (%sYS). Alarm code : %s    %s."), l_tszText2, l_tszText3, l_tszText4);
/* modified 2010.02.24 hmenjo MotSys Y ���A���[�����O�C�� ---------- } ---------- */
		_tcscat(tszAlarmLog, tszTemp);
		break;
	case 4:		// �����������t���O���O�ɂȂ�Ȃ�����
		_tcscpy(l_tszText1, l_tszText0);
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("Not be initialized on Controller.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" System Error. Not be Initialized on Controller."));
		break;
	case 5:		// DIO �G���[
		_tcscpy(l_tszText1, _T("DIO error.\n----------------------------------\n"));
		_tcscat(l_tszText1, _T("System Error.\n"));
		_tcscat(l_tszText1, _T("Not work DIO board.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" DIO error."));
		break;
	case 6:		// �T�[�{ ON/OFF �G���[
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
	case 7:		// ����~ �G���[
		_tcscpy(l_tszText1, _T("EMG error.\n----------------------------------\n"));
		_tcscat(l_tszText1, _T("EMG button.\n"));
		_tcscat(l_tszText1, _T("End Motion System Driver?\n"));
		_tcscat(l_tszText1, _T("(then reboot the system)"));
		l_MsgID = 0;
		// AlarmLog Message
		_tcscat(tszAlarmLog, _T(" EMG error."));
		break;
	case 8: 	// ���_���A �G���[	 ���_���A���s���Ȃ�����
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
			static ls_iMsgID_0 = 0;
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
/* deleted 2009.06.02 hmenjo ���A���[���ł� COM �ʐM���f���� ---------- { ---------- */
//			static ls_iMsgID_1 = 0;
//			if (0 == ls_iMsgID_1) {ls_iMsgID_1 = 1;} else {break;}
//			g_CC_CommAbort = TRUE;
/* deleted 2009.06.02 hmenjo ���A���[���ł� COM �ʐM���f���� ---------- } ---------- */
			l_ret = ::MessageBox(NULL, l_tszText1, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
/* deleted 2009.06.02 hmenjo ���A���[���ł� COM �ʐM���f���� ---------- { ---------- */
//			g_CC_CommAbort = FALSE;
/* deleted 2009.06.02 hmenjo ���A���[���ł� COM �ʐM���f���� ---------- } ---------- */
			if (l_Mode == 3) {
				g_CC_bClearAlarm[CC_CnvAxisToSPT(l_Axis)] = TRUE;
			}
/* deleted 2009.06.02 hmenjo ���A���[���ł� COM �ʐM���f���� ---------- { ---------- */
//			ls_iMsgID_1 = 0;
/* deleted 2009.06.02 hmenjo ���A���[���ł� COM �ʐM���f���� ---------- } ---------- */
		}
		break;
	default:
		if (0 == m_bOnClosing) {
			static ls_iMsgID_def = 0;
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

// ���[�h�f�[�^���Q�i���\�L�̕�����ɕϊ�����
//	mode �ɂ�蕪���ł���D
//		=0 : �����Ȃ�
//		=1 : 8 �r�b�g���ɕ���
//		=2 : 4 �r�b�g���ɕ���
void CMotSysDlg::CnvWordToBin(
		WORD WordData,		// 16 bit �f�[�^
		TCHAR *ptszBinary,	// �ϊ���f�[�^������
		WORD mode			// ���[�h(��L�Q��)
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
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

	// �E�B���h�E�̃v���p�e�B���X�g���环�ʏ����폜
	::RemoveProp(m_hWnd, IDENT_CODE);

	return CDialog::DestroyWindow();
}

// �T�[�{ OFF
void CMotSysDlg::OnBTNServoOFF()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	g_CC_ServoOFFReq = TRUE;
}

// �T�[�{ ON
void CMotSysDlg::OnBTNServoON()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	g_CC_ServoONReq = TRUE;
}

// �T�[�{�p�����^�Ǎ���
void CMotSysDlg::OnBtnServoParam()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CC_LoadServoParam();

}

// X �� �ʒu�΍��s�[�N�l���N���A
void CMotSysDlg::OnBtnDPosPeakClearX()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_BIG_X], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_BIG_X], _T("0"));
	m_dPos[1][AXIS_BIG_X] = m_dPos[2][AXIS_BIG_X] = 0;
}
// YM �� �ʒu�΍��s�[�N�l���N���A
void CMotSysDlg::OnBtnDPosPeakClearYM()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_BIG_Y], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_BIG_Y], _T("0"));
	m_dPos[1][AXIS_BIG_Y] = m_dPos[2][AXIS_BIG_Y] = 0;
}
// YS �� �ʒu�΍��s�[�N�l���N���A
void CMotSysDlg::OnBtnDPosPeakClearYS()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_NUM], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_NUM], _T("0"));
	m_dPos[1][AXIS_NUM] = m_dPos[2][AXIS_NUM] = 0;
}
// Z �� �ʒu�΍��s�[�N�l���N���A
void CMotSysDlg::OnBtnDPosPeakClearZ()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_Z], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_Z], _T("0"));
	m_dPos[1][AXIS_Z] = m_dPos[2][AXIS_Z] = 0;
}
// T �� �ʒu�΍��s�[�N�l���N���A
void CMotSysDlg::OnBtnDPosPeakClearT()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetDlgItemText(g_iDlgCtrlID[19][AXIS_T], _T("0"));
	SetDlgItemText(g_iDlgCtrlID[20][AXIS_T], _T("0"));
	m_dPos[1][AXIS_T] = m_dPos[2][AXIS_T] = 0;
}
// �e���ʒu�΍���\��
void CMotSysDlg::DispdPos(WORD wAxis, long d_Pos)
{
	TCHAR	l_tszText[32];
//	short	l_sAxis = ConvAxis(wAxis);		// �s�v�̂͂�
	short	l_sAxis = wAxis;

	// ���A���^�C���l���X�V
	m_dPos[0][l_sAxis] = d_Pos;
	_stprintf(l_tszText, _T("%11d"), m_dPos[0][l_sAxis]);
	SetDlgItemText(g_iDlgCtrlID[17][l_sAxis], l_tszText);
	// �|���s�[�N�l���X�V
	if (m_dPos[0][l_sAxis] < m_dPos[1][l_sAxis]) {
		m_dPos[1][l_sAxis] = m_dPos[0][l_sAxis];
		_stprintf(l_tszText, _T("%11d"), m_dPos[1][l_sAxis]);
		SetDlgItemText(g_iDlgCtrlID[19][l_sAxis], l_tszText);
	}
	// �{���s�[�N�l���X�V
	if (m_dPos[2][l_sAxis] < m_dPos[0][l_sAxis]) {
		m_dPos[2][l_sAxis] = m_dPos[0][l_sAxis];
		_stprintf(l_tszText, _T("%11d"), m_dPos[2][l_sAxis]);
		SetDlgItemText(g_iDlgCtrlID[20][l_sAxis], l_tszText);
	}
}


// �ȉ��̃R�[�h�̓f�o�b�O�p

// ���_���A
void CMotSysDlg::OnBTNORGtest()		// �uX ORG�v�{�^��
{
	mmtestORGX(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestY()		// �uY ORG�v�{�^��
{
	mmtestORGY(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestZ()		// �uZ ORG�v�{�^��
{
	mmtestORGZ(0);	//debdeb
}
void CMotSysDlg::OnBTNORGtestT()		// �uT ORG�v�{�^��
{
	mmtestORGT(0);	//debdeb
}
// ABS �ړ�
void CMotSysDlg::OnBTNABStestX()		// �uX ABS1�v�{�^��
{
	mmtestORGX(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestX2()		// �uX ABS2�v�{�^��
{
	mmtestORGX(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestY()		// �uY ABS1�v�{�^��
{
	mmtestORGY(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestY2()		// �uY ABS2�v�{�^��
{
	mmtestORGY(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestZ()		// �uZ ABS1�v�{�^��
{
	mmtestORGZ(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestZ2()		// �uZ ABS2�v�{�^��
{
	mmtestORGZ(21);	//debdeb
}
void CMotSysDlg::OnBTNABStestT()		// �uT ABS1�v�{�^��
{
	mmtestORGT(2);	//debdeb
}
void CMotSysDlg::OnBTNABStestT2()		// �uT ABS2�v�{�^��
{
	mmtestORGT(21);	//debdeb
}
// INC �ړ�
void CMotSysDlg::OnBTNINCtestX()		// �uX INC+�v�{�^��
{
	mmtestORGX(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestX2()		// �uX INC-�v�{�^��
{
	mmtestORGX(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestY()		// �uY INC+�v�{�^��
{
	mmtestORGY(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestY2()		// �uY INC-�v�{�^��
{
	mmtestORGY(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestZ()		// �uZ INC+�v�{�^��
{
	mmtestORGZ(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestZ2()		// �uZ INC-�v�{�^��
{
	mmtestORGZ(31);	//debdeb
}
void CMotSysDlg::OnBTNINCtestT()		// �uT INC+�v�{�^��
{
	mmtestORGT(3);	//debdeb
}
void CMotSysDlg::OnBTNINCtestT2()		// �uT INC-�v�{�^��
{
	mmtestORGT(31);	//debdeb
}
// HP �ړ�
void CMotSysDlg::OnBTNHPtestX()		// �uX HP�v�{�^��
{
	mmtestORGX(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestY()		// �uY HP�v�{�^��
{
	mmtestORGY(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestZ()		// �uZ HP�v�{�^��
{
	mmtestORGZ(1);	//debdeb
}
void CMotSysDlg::OnBTNHPtestT()		// �uT HP�v�{�^��
{
	mmtestORGT(1);	//debdeb
}

// �R���g���[���� Enable����
void CMotSysDlg::EnableDlgCtrl()
{
	int l_iCtrlID[][9] = {	// �e�����Ƃɍő區�ڐ��͂T�O�ł��D(g_iDlgCtrlID[][]�Œ�`)
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

	// �R���g���[�� ID �e�[�u����ݒ�
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

	// Enable ����
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

//	�A���[���p���O
void CMotSysDlg::AlarmLogging(TCHAR *ptszAlarmLog)
{
	if (0 == m_pAlarmLogFile) {
		return;
	}

	TCHAR	l_tszLogBuff[1024] = _T("");

	// ���t�C������ݒ�
	m_pAlarmLogFile->AddTime(l_tszLogBuff, 2);
	_tcscat(l_tszLogBuff, _T(" "));
	_tcscat(l_tszLogBuff, ptszAlarmLog);

	m_pAlarmLogFile->Logging(l_tszLogBuff);
}

/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
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

	/* ���t�C������ݒ�	*/
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
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */
