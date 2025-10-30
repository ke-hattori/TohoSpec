// AxisDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MotSysCfg.h"
#include "AxisDlg.h"
#include "MotSysCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma pack(push, 1)
typedef struct MOT_PARAMStag {
	short	nLowGain;			// K value during trapezoidal moves
	short	nHighGain;			// K value for position hold after moves
	short	nZero;
	short	nPole;				// A, B - values for the digital filter
	short	nSampleTime;		// T - the filter's sample period (HCTL reg R0F)
	BYTE	byPwmMax;			// pwm safety limit to trigger motor overload
	BYTE	byPwmFilterCount;	// consecutive counts > limit to trigger failure
	short	nMode;				// operation mode of the axis - values are
	LONG	lDestination;		// destination location for the axis
	short	nMaxVelocity;		// max velocity for trapezoidal moves
	short	nIntegVelocity;		// command (integral) velocity
	short	nPropVelocity;		// command (proportional) velocity
	short	nAcceleration;		// current acceleration
} MOT_PARAMS;
#pragma pack(pop)

typedef MOT_PARAMS *LPMOT_PARAMS;

#pragma pack(push, 1)
typedef struct MOT_DEF_PARAMStag {
	BYTE	byStatusReg;		// status reg value for HCTL chip
	BYTE	byUnused;			// needed for alignment
	LONG	lMinusLimit;
	LONG	lPlusLimit;			// from Home
// BOOL は int で定義されている
// Win32 では int は 32 ビットなので 16 ビットに変更する
	short	bInvertedSense;		// true -> motions & positions inverted
	short	bForwardForHome;	// true -> go forward to home if home sensor is
//	BOOL	bInvertedSense;		// true -> motions & positions inverted
//	BOOL	bForwardForHome;	// true -> go forward to home if home sensor is
								//	 on low side of home
	short	nHomeVelocity;		// high velocity to do a home move
	short	nHomeAccel;			// acceleration to do a home move
	LPMOT_PARAMS	lpDefaultMotParams;
} MOT_DEF_PARAMS;
#pragma pack(pop)

typedef MOT_DEF_PARAMS *LPMOT_DEF_PARAMS;

static MOT_PARAMS mot_data[8];
static MOT_DEF_PARAMS axis_default_data[8];

extern char szAxisName[8][10];

static const BYTE pwm_filter_count = 4;

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg ダイアログ


CAxisDlg::CAxisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAxisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAxisDlg)
	m_Acceleration = 0;
	m_DefaultMotParams = _T("");
	m_Destination = 0;
	m_FWD_Home = 0;
	m_HCTLStatusReg = 0;
	m_HighGain = 0;
	m_HomeAccel = 0;
	m_HomeVelocity = 0;
	m_IntegVelocity = 0;
	m_InvertedSense = 0;
	m_LowGain = 0;
	m_MaxVelocity = 0;
	m_MinusLimit = 0;
	m_Mode = 0;
	m_PlusLimit = 0;
	m_Pole = 0;
	m_PropVelocity = 0;
	m_PwmFilterCount = 0;
	m_PwmMax = 0;
	m_SampleTime = 0;
	m_Unused = 0;
	m_Zero = 0;
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CAxisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxisDlg)
	DDX_Text(pDX, IDC_EDIT_ACCEL, m_Acceleration);
	DDX_Text(pDX, IDC_EDIT_DEFMOTP, m_DefaultMotParams);
	DDX_Text(pDX, IDC_EDIT_DISTINATION, m_Destination);
	DDX_Text(pDX, IDC_EDIT_FWD_HOME, m_FWD_Home);
	DDX_Text(pDX, IDC_EDIT_HCTL_STAT, m_HCTLStatusReg);
	DDX_Text(pDX, IDC_EDIT_HIGHGAIN, m_HighGain);
	DDX_Text(pDX, IDC_EDIT_HOME_ACCEL, m_HomeAccel);
	DDX_Text(pDX, IDC_EDIT_HOME_VELOCITY, m_HomeVelocity);
	DDX_Text(pDX, IDC_EDIT_INTEG_VELOCITY, m_IntegVelocity);
	DDX_Text(pDX, IDC_EDIT_INVERT, m_InvertedSense);
	DDX_Text(pDX, IDC_EDIT_LOWGAIN, m_LowGain);
	DDX_Text(pDX, IDC_EDIT_MAX_VELOCITY, m_MaxVelocity);
	DDX_Text(pDX, IDC_EDIT_MLIMIT, m_MinusLimit);
	DDX_Text(pDX, IDC_EDIT_MODE, m_Mode);
	DDX_Text(pDX, IDC_EDIT_PLIMIT, m_PlusLimit);
	DDX_Text(pDX, IDC_EDIT_POLE, m_Pole);
	DDX_Text(pDX, IDC_EDIT_PROP_VELOCITY, m_PropVelocity);
	DDX_Text(pDX, IDC_EDIT_PWMFILTER, m_PwmFilterCount);
	DDX_Text(pDX, IDC_EDIT_PWMMAX, m_PwmMax);
	DDX_Text(pDX, IDC_EDIT_SAMPLETIME, m_SampleTime);
	DDX_Text(pDX, IDC_EDIT_UNUSED, m_Unused);
	DDX_Text(pDX, IDC_EDIT_ZERO, m_Zero);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxisDlg, CDialog)
	//{{AFX_MSG_MAP(CAxisDlg)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_LOAD, OnBtnLoad)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg メッセージ ハンドラ

BOOL CAxisDlg::Create(CWnd *pWnd, int AxisNo)
{
	m_pParent = pWnd;
	m_AxisNo = AxisNo;

	BOOL l_rc = CDialog::Create(CAxisDlg::IDD, m_pParent);
	if (l_rc == TRUE) {
		this->ShowWindow(SW_SHOW);
	}
	switch (m_AxisNo) {
	case 0:	SetWindowText("BigX.dat"); break;
	case 1:	SetWindowText("BigY.dat"); break;
	case 2:	SetWindowText("Z.dat"); break;
	case 3:	SetWindowText("XSlow.dat"); break;
	case 4:	SetWindowText("Theta.dat"); break;
	case 5:	SetWindowText("Level.dat"); break;
	case 6:	SetWindowText("XFast.dat"); break;
	case 7:	SetWindowText("Y.dat"); break;
	}

	return l_rc;
}

void CAxisDlg::OnClose()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}

int CAxisDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	// メインウィンドウのアイコンを設定
	SetIcon(((CMotSysCfgDlg*) m_pParent)->m_hMainIcon, TRUE);
	SetIcon(((CMotSysCfgDlg*) m_pParent)->m_hMainIcon, FALSE);

	return 0;
}

void CAxisDlg::PostNcDestroy()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	switch (m_AxisNo) {
	case 0:
		((CMotSysCfgDlg*) m_pParent)->m_chkBigX = FALSE;
		((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_BIGX)))->SetCheck(0);
		break;
	case 1:
		((CMotSysCfgDlg*) m_pParent)->m_chkBigY = FALSE;
		((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_BIGY)))->SetCheck(0);
		break;
	case 2:
		((CMotSysCfgDlg*) m_pParent)->m_chkZ = FALSE;
		((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_Z)))->SetCheck(0);
		break;
	case 3:
		((CMotSysCfgDlg*) m_pParent)->m_chkXSlow = FALSE;
		((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_XSLOW)))->SetCheck(0);
		break;
	case 4:
		((CMotSysCfgDlg*) m_pParent)->m_chkTheta = FALSE;
		((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_THETA)))->SetCheck(0);
		break;
	case 5:
		((CMotSysCfgDlg*) m_pParent)->m_chkLevel = FALSE;
		((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_LEVEL)))->SetCheck(0);
		break;
	case 6:
		((CMotSysCfgDlg*) m_pParent)->m_chkXFast = FALSE;
		((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_XFAST)))->SetCheck(0);
		break;
	case 7:
		((CMotSysCfgDlg*) m_pParent)->m_chkY = FALSE;
		((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_Y)))->SetCheck(0);
		break;
	}

	if (m_pParent != NULL) {
		((CMotSysCfgDlg*) m_pParent)->EndAxisDlg(m_AxisNo);
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CAxisDlg::OnOK()
{
//	CDialog::OnOK();
}

void CAxisDlg::OnCancel()
{
//	CDialog::OnCancel();
}

void CAxisDlg::OnBtnLoad()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	short	l_axis = m_AxisNo;
	int		fd;
	char	szPathName[128];
	LPMOT_DEF_PARAMS	lpDef = &(axis_default_data[l_axis]);
	LPMOT_PARAMS		lpMot = &(mot_data[l_axis]);
	char	l_cText[255];

	/* Windows ディレクトリのパスを取得 */
	GetWindowsDirectory(szPathName, 128);
	lstrcat(szPathName, "\\");
	lstrcat(szPathName, szAxisName[l_axis]);	/* ReadConfig()で読出した値 */
	lstrcat(szPathName, ".dat");

	if ((fd = _lopen (szPathName, OF_READ)) >= 0) {
		/* ファイル「軸名.dat」がオープン出来た */
		/* モーションパラメタを読出し */
		_lread(fd, (LPSTR) lpMot, sizeof(MOT_PARAMS));
		/* モーションデフォルトパラメタを読出し */
		_lread(fd, (LPSTR) lpDef, sizeof(MOT_DEF_PARAMS));
		/* モーションデフォルトパラメタ内にモーションパラメタのアドレスをセット */
		lpDef->lpDefaultMotParams = lpMot;
		_lclose(fd);
		// データ表示
		DispData();
	} else {
		// ファイルオープンエラー
		sprintf(l_cText, "Failed to open(read) file[%s]", szPathName);
		::MessageBox(NULL, l_cText, "Motion Config Error", MB_OK | MB_SYSTEMMODAL);
	}
	/* 連続カウントのしきい値をセット */
	lpMot->byPwmFilterCount = pwm_filter_count;

}

void CAxisDlg::OnBtnSave()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	short	l_axis = m_AxisNo;
	int fd;
	char  szPathName[128];
	LPMOT_DEF_PARAMS lpDef = &(axis_default_data[l_axis]);
	LPMOT_PARAMS lpMot = &(mot_data[l_axis]);
	char	l_cText[255];

	/* Windows ディレクトリのパスを取得 */
	GetWindowsDirectory(szPathName, 128);
	lstrcat(szPathName, "\\");
	lstrcat(szPathName, szAxisName[l_axis]);	/* ReadConfig()で読出した値 */
	lstrcat(szPathName, ".dat");

	// データ取得
	GetData();
	if ((fd = _lcreat(szPathName, 0)) >= 0) {
		/* ファイル「軸名.dat」が作成出来た */
		/* モーションパラメタを書込み */
		_lwrite(fd, (LPSTR) lpMot, sizeof(MOT_PARAMS));
		/* モーションデフォルトパラメタを書込み */
		_lwrite(fd, (LPSTR) lpDef, sizeof(MOT_DEF_PARAMS));
		_lclose(fd);
		// データ表示
		DispData();
	} else {
		// ファイルオープンエラー
		sprintf(l_cText, "Failed to open(read) file[%s]", szPathName);
		::MessageBox(NULL, l_cText, "Motion Config Error", MB_OK | MB_SYSTEMMODAL);
	}
}

// データ表示
void CAxisDlg::DispData()
{
	// Motion Parameter
	m_LowGain		 = mot_data[m_AxisNo].nLowGain;
	m_HighGain		 = mot_data[m_AxisNo].nHighGain;
	m_Zero			 = mot_data[m_AxisNo].nZero;
	m_Pole			 = mot_data[m_AxisNo].nPole;
	m_SampleTime	 = mot_data[m_AxisNo].nSampleTime;
	m_PwmMax		 = mot_data[m_AxisNo].byPwmMax;
	m_PwmFilterCount = mot_data[m_AxisNo].byPwmFilterCount;
	m_Mode			 = mot_data[m_AxisNo].nMode;
	m_Destination	 = mot_data[m_AxisNo].lDestination;
	m_MaxVelocity	 = mot_data[m_AxisNo].nMaxVelocity;
	m_IntegVelocity  = mot_data[m_AxisNo].nIntegVelocity;
	m_PropVelocity	 = mot_data[m_AxisNo].nPropVelocity;
	m_Acceleration	 = mot_data[m_AxisNo].nAcceleration;

	// Motion Default Parameter
	m_HCTLStatusReg    = axis_default_data[m_AxisNo].byStatusReg;
	m_Unused		   = axis_default_data[m_AxisNo].byUnused;
	m_MinusLimit	   = axis_default_data[m_AxisNo].lMinusLimit;
	m_PlusLimit 	   = axis_default_data[m_AxisNo].lPlusLimit;
	m_InvertedSense    = axis_default_data[m_AxisNo].bInvertedSense;
	m_FWD_Home		   = axis_default_data[m_AxisNo].bForwardForHome;
	m_HomeVelocity	   = axis_default_data[m_AxisNo].nHomeVelocity;
	m_HomeAccel 	   = axis_default_data[m_AxisNo].nHomeAccel;
	m_DefaultMotParams.Format("%08x", axis_default_data[m_AxisNo].lpDefaultMotParams);

	// 更新
	UpdateData(FALSE);
}

// データ取得
void CAxisDlg::GetData()
{
	UpdateData(TRUE);

	// Motion Parameter
	mot_data[m_AxisNo].nLowGain 		= m_LowGain;
	mot_data[m_AxisNo].nHighGain		= m_HighGain;
	mot_data[m_AxisNo].nZero			= m_Zero;
	mot_data[m_AxisNo].nPole			= m_Pole;
	mot_data[m_AxisNo].nSampleTime		= m_SampleTime;
	mot_data[m_AxisNo].byPwmMax 		= m_PwmMax;
	mot_data[m_AxisNo].byPwmFilterCount = m_PwmFilterCount;
	mot_data[m_AxisNo].nMode			= m_Mode;
	mot_data[m_AxisNo].lDestination 	= m_Destination;
	mot_data[m_AxisNo].nMaxVelocity 	= m_MaxVelocity;
	mot_data[m_AxisNo].nIntegVelocity	= m_IntegVelocity;
	mot_data[m_AxisNo].nPropVelocity	= m_PropVelocity;
	mot_data[m_AxisNo].nAcceleration	= m_Acceleration;

	// Motion Default Parameter
	axis_default_data[m_AxisNo].byStatusReg 	   = m_HCTLStatusReg;
	axis_default_data[m_AxisNo].byUnused		   = m_Unused;
	axis_default_data[m_AxisNo].lMinusLimit 	   = m_MinusLimit;
	axis_default_data[m_AxisNo].lPlusLimit		   = m_PlusLimit;
	axis_default_data[m_AxisNo].bInvertedSense	   = m_InvertedSense;
	axis_default_data[m_AxisNo].bForwardForHome    = m_FWD_Home;
	axis_default_data[m_AxisNo].nHomeVelocity	   = m_HomeVelocity;
	axis_default_data[m_AxisNo].nHomeAccel		   = m_HomeAccel;
	axis_default_data[m_AxisNo].lpDefaultMotParams = (LPMOT_PARAMS) strtoul(m_DefaultMotParams, NULL, 16);
}

BOOL CAxisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	// 最初にロードしておく
	OnBtnLoad();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
