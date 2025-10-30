// ServoParam.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MotSysCfg.h"
#include "ServoParam.h"
#include "MotSysCfgDlg.h"

// 軸定義(必ず連番にしてください．欠番禁止．順序は不問)
#define	CC_AXIS_X	0	// X 軸 (1)
#define	CC_AXIS_Y	1	// Y 軸 (2)
#define	CC_AXIS_Z	2	// Z 軸 (3)		(Nano では未使用)
#define	CC_AXIS_T	3	// T 軸 (4)		(Nano では未使用)
#define	CC_AXIS_NUM	4	// 最大軸数

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CServoParam ダイアログ


CServoParam::CServoParam(CWnd* pParent /*=NULL*/)
	: CDialog(CServoParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServoParam)
	m_ABS_AccelTimeS_T = 0;
	m_ABS_AccelTimeS_X = 0;
	m_ABS_AccelTimeS_Y = 0;
	m_ABS_AccelTimeS_Z = 0;
	m_ABS_AccelTimeT_T = 0;
	m_ABS_AccelTimeT_X = 0;
	m_ABS_AccelTimeT_Y = 0;
	m_ABS_AccelTimeT_Z = 0;
	m_HP_AccelTimeS_T = 0;
	m_HP_AccelTimeS_X = 0;
	m_HP_AccelTimeS_Y = 0;
	m_HP_AccelTimeS_Z = 0;
	m_HP_AccelTimeT_T = 0;
	m_HP_AccelTimeT_X = 0;
	m_HP_AccelTimeT_Y = 0;
	m_HP_AccelTimeT_Z = 0;
	m_HP_BaseBand_T = 0;
	m_HP_BaseBand_X = 0;
	m_HP_BaseBand_Y = 0;
	m_HP_BaseBand_Z = 0;
	m_INC_AccelTimeS_T = 0;
	m_INC_AccelTimeS_X = 0;
	m_INC_AccelTimeS_Y = 0;
	m_INC_AccelTimeS_Z = 0;
	m_INC_AccelTimeT_T = 0;
	m_INC_AccelTimeT_X = 0;
	m_INC_AccelTimeT_Y = 0;
	m_INC_AccelTimeT_Z = 0;
	m_ORG1_AccelTimeS_T = 0;
	m_ORG1_AccelTimeS_X = 0;
	m_ORG1_AccelTimeS_Y = 0;
	m_ORG1_AccelTimeS_Z = 0;
	m_ORG1_AccelTimeT_T = 0;
	m_ORG1_AccelTimeT_X = 0;
	m_ORG1_AccelTimeT_Y = 0;
	m_ORG1_AccelTimeT_Z = 0;
	m_ORG1_Speed_T = 0;
	m_ORG1_Speed_X = 0;
	m_ORG1_Speed_Y = 0;
	m_ORG1_Speed_Z = 0;
	m_ORG2_AccelTimeS_T = 0;
	m_ORG2_AccelTimeS_X = 0;
	m_ORG2_AccelTimeS_Y = 0;
	m_ORG2_AccelTimeS_Z = 0;
	m_ORG2_AccelTimeT_T = 0;
	m_ORG2_AccelTimeT_X = 0;
	m_ORG2_AccelTimeT_Y = 0;
	m_ORG2_AccelTimeT_Z = 0;
	m_ORG2_Speed_T = 0;
	m_ORG2_Speed_X = 0;
	m_ORG2_Speed_Y = 0;
	m_ORG2_Speed_Z = 0;
	m_ORG3_AccelTimeS_T = 0;
	m_ORG3_AccelTimeS_X = 0;
	m_ORG3_AccelTimeS_Z = 0;
	m_ORG3_AccelTimeT_T = 0;
	m_ORG3_AccelTimeT_X = 0;
	m_ORG3_AccelTimeS_Y = 0;
	m_ORG3_AccelTimeT_Y = 0;
	m_ORG3_AccelTimeT_Z = 0;
	m_ORG3_Speed_T = 0;
	m_ORG3_Speed_X = 0;
	m_ORG3_Speed_Y = 0;
	m_ORG3_Speed_Z = 0;
	m_ABS_Speed_T = 0;
	m_ABS_Speed_X = 0;
	m_ABS_Speed_Y = 0;
	m_ABS_Speed_Z = 0;
	m_HP_Speed_T = 0;
	m_HP_Speed_X = 0;
	m_HP_Speed_Y = 0;
	m_HP_Speed_Z = 0;
	m_INC_Speed_T = 0;
	m_INC_Speed_X = 0;
	m_INC_Speed_Y = 0;
	m_INC_Speed_Z = 0;
	//}}AFX_DATA_INIT
}


void CServoParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServoParam)
	DDX_Text(pDX, IDC_EDIT_ABS_ATS_T, m_ABS_AccelTimeS_T);
	DDV_MinMaxLong(pDX, m_ABS_AccelTimeS_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_ATS_X, m_ABS_AccelTimeS_X);
	DDV_MinMaxLong(pDX, m_ABS_AccelTimeS_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_ATS_Y, m_ABS_AccelTimeS_Y);
	DDV_MinMaxLong(pDX, m_ABS_AccelTimeS_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_ATS_Z, m_ABS_AccelTimeS_Z);
	DDV_MinMaxLong(pDX, m_ABS_AccelTimeS_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_ATT_T, m_ABS_AccelTimeT_T);
	DDV_MinMaxLong(pDX, m_ABS_AccelTimeT_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_ATT_X, m_ABS_AccelTimeT_X);
	DDV_MinMaxLong(pDX, m_ABS_AccelTimeT_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_ATT_Y, m_ABS_AccelTimeT_Y);
	DDV_MinMaxLong(pDX, m_ABS_AccelTimeT_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_ATT_Z, m_ABS_AccelTimeT_Z);
	DDV_MinMaxLong(pDX, m_ABS_AccelTimeT_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_ATS_T, m_HP_AccelTimeS_T);
	DDV_MinMaxLong(pDX, m_HP_AccelTimeS_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_ATS_X, m_HP_AccelTimeS_X);
	DDV_MinMaxLong(pDX, m_HP_AccelTimeS_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_ATS_Y, m_HP_AccelTimeS_Y);
	DDV_MinMaxLong(pDX, m_HP_AccelTimeS_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_ATS_Z, m_HP_AccelTimeS_Z);
	DDV_MinMaxLong(pDX, m_HP_AccelTimeS_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_ATT_T, m_HP_AccelTimeT_T);
	DDV_MinMaxLong(pDX, m_HP_AccelTimeT_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_ATT_X, m_HP_AccelTimeT_X);
	DDV_MinMaxLong(pDX, m_HP_AccelTimeT_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_ATT_Y, m_HP_AccelTimeT_Y);
	DDV_MinMaxLong(pDX, m_HP_AccelTimeT_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_ATT_Z, m_HP_AccelTimeT_Z);
	DDV_MinMaxLong(pDX, m_HP_AccelTimeT_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_BASE_T, m_HP_BaseBand_T);
	DDV_MinMaxLong(pDX, m_HP_BaseBand_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_BASE_X, m_HP_BaseBand_X);
	DDV_MinMaxLong(pDX, m_HP_BaseBand_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_BASE_Y, m_HP_BaseBand_Y);
	DDV_MinMaxLong(pDX, m_HP_BaseBand_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_BASE_Z, m_HP_BaseBand_Z);
	DDV_MinMaxLong(pDX, m_HP_BaseBand_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_ATS_T, m_INC_AccelTimeS_T);
	DDV_MinMaxLong(pDX, m_INC_AccelTimeS_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_ATS_X, m_INC_AccelTimeS_X);
	DDV_MinMaxLong(pDX, m_INC_AccelTimeS_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_ATS_Y, m_INC_AccelTimeS_Y);
	DDV_MinMaxLong(pDX, m_INC_AccelTimeS_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_ATS_Z, m_INC_AccelTimeS_Z);
	DDV_MinMaxLong(pDX, m_INC_AccelTimeS_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_ATT_T, m_INC_AccelTimeT_T);
	DDV_MinMaxLong(pDX, m_INC_AccelTimeT_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_ATT_X, m_INC_AccelTimeT_X);
	DDV_MinMaxLong(pDX, m_INC_AccelTimeT_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_ATT_Y, m_INC_AccelTimeT_Y);
	DDV_MinMaxLong(pDX, m_INC_AccelTimeT_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_ATT_Z, m_INC_AccelTimeT_Z);
	DDV_MinMaxLong(pDX, m_INC_AccelTimeT_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_ATS_T, m_ORG1_AccelTimeS_T);
	DDV_MinMaxLong(pDX, m_ORG1_AccelTimeS_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_ATS_X, m_ORG1_AccelTimeS_X);
	DDV_MinMaxLong(pDX, m_ORG1_AccelTimeS_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_ATS_Y, m_ORG1_AccelTimeS_Y);
	DDV_MinMaxLong(pDX, m_ORG1_AccelTimeS_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_ATS_Z, m_ORG1_AccelTimeS_Z);
	DDV_MinMaxLong(pDX, m_ORG1_AccelTimeS_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_ATT_T, m_ORG1_AccelTimeT_T);
	DDV_MinMaxLong(pDX, m_ORG1_AccelTimeT_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_ATT_X, m_ORG1_AccelTimeT_X);
	DDV_MinMaxLong(pDX, m_ORG1_AccelTimeT_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_ATT_Y, m_ORG1_AccelTimeT_Y);
	DDV_MinMaxLong(pDX, m_ORG1_AccelTimeT_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_ATT_Z, m_ORG1_AccelTimeT_Z);
	DDV_MinMaxLong(pDX, m_ORG1_AccelTimeT_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_SPD_T, m_ORG1_Speed_T);
	DDV_MinMaxLong(pDX, m_ORG1_Speed_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_SPD_X, m_ORG1_Speed_X);
	DDV_MinMaxLong(pDX, m_ORG1_Speed_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_SPD_Y, m_ORG1_Speed_Y);
	DDV_MinMaxLong(pDX, m_ORG1_Speed_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG1_SPD_Z, m_ORG1_Speed_Z);
	DDV_MinMaxLong(pDX, m_ORG1_Speed_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_ATS_T, m_ORG2_AccelTimeS_T);
	DDV_MinMaxLong(pDX, m_ORG2_AccelTimeS_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_ATS_X, m_ORG2_AccelTimeS_X);
	DDV_MinMaxLong(pDX, m_ORG2_AccelTimeS_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_ATS_Y, m_ORG2_AccelTimeS_Y);
	DDV_MinMaxLong(pDX, m_ORG2_AccelTimeS_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_ATS_Z, m_ORG2_AccelTimeS_Z);
	DDV_MinMaxLong(pDX, m_ORG2_AccelTimeS_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_ATT_T, m_ORG2_AccelTimeT_T);
	DDV_MinMaxLong(pDX, m_ORG2_AccelTimeT_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_ATT_X, m_ORG2_AccelTimeT_X);
	DDV_MinMaxLong(pDX, m_ORG2_AccelTimeT_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_ATT_Y, m_ORG2_AccelTimeT_Y);
	DDV_MinMaxLong(pDX, m_ORG2_AccelTimeT_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_ATT_Z, m_ORG2_AccelTimeT_Z);
	DDV_MinMaxLong(pDX, m_ORG2_AccelTimeT_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_SPD_T, m_ORG2_Speed_T);
	DDV_MinMaxLong(pDX, m_ORG2_Speed_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_SPD_X, m_ORG2_Speed_X);
	DDV_MinMaxLong(pDX, m_ORG2_Speed_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_SPD_Y, m_ORG2_Speed_Y);
	DDV_MinMaxLong(pDX, m_ORG2_Speed_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG2_SPD_Z, m_ORG2_Speed_Z);
	DDV_MinMaxLong(pDX, m_ORG2_Speed_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_ATS_T, m_ORG3_AccelTimeS_T);
	DDV_MinMaxLong(pDX, m_ORG3_AccelTimeS_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_ATS_X, m_ORG3_AccelTimeS_X);
	DDV_MinMaxLong(pDX, m_ORG3_AccelTimeS_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_ATS_Z, m_ORG3_AccelTimeS_Z);
	DDV_MinMaxLong(pDX, m_ORG3_AccelTimeS_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_ATT_T, m_ORG3_AccelTimeT_T);
	DDV_MinMaxLong(pDX, m_ORG3_AccelTimeT_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_ATT_X, m_ORG3_AccelTimeT_X);
	DDV_MinMaxLong(pDX, m_ORG3_AccelTimeT_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_ATS_Y, m_ORG3_AccelTimeS_Y);
	DDV_MinMaxLong(pDX, m_ORG3_AccelTimeS_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_ATT_Y, m_ORG3_AccelTimeT_Y);
	DDV_MinMaxLong(pDX, m_ORG3_AccelTimeT_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_ATT_Z, m_ORG3_AccelTimeT_Z);
	DDV_MinMaxLong(pDX, m_ORG3_AccelTimeT_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_SPD_T, m_ORG3_Speed_T);
	DDV_MinMaxLong(pDX, m_ORG3_Speed_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_SPD_X, m_ORG3_Speed_X);
	DDV_MinMaxLong(pDX, m_ORG3_Speed_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_SPD_Y, m_ORG3_Speed_Y);
	DDV_MinMaxLong(pDX, m_ORG3_Speed_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ORG3_SPD_Z, m_ORG3_Speed_Z);
	DDV_MinMaxLong(pDX, m_ORG3_Speed_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_SPD_T, m_ABS_Speed_T);
	DDV_MinMaxLong(pDX, m_ABS_Speed_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_SPD_X, m_ABS_Speed_X);
	DDV_MinMaxLong(pDX, m_ABS_Speed_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_SPD_Y, m_ABS_Speed_Y);
	DDV_MinMaxLong(pDX, m_ABS_Speed_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_ABS_SPD_Z, m_ABS_Speed_Z);
	DDV_MinMaxLong(pDX, m_ABS_Speed_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_SPD_T, m_HP_Speed_T);
	DDV_MinMaxLong(pDX, m_HP_Speed_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_SPD_X, m_HP_Speed_X);
	DDV_MinMaxLong(pDX, m_HP_Speed_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_SPD_Y, m_HP_Speed_Y);
	DDV_MinMaxLong(pDX, m_HP_Speed_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_HP_SPD_Z, m_HP_Speed_Z);
	DDV_MinMaxLong(pDX, m_HP_Speed_Z, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_SPD_T, m_INC_Speed_T);
	DDV_MinMaxLong(pDX, m_INC_Speed_T, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_SPD_X, m_INC_Speed_X);
	DDV_MinMaxLong(pDX, m_INC_Speed_X, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_SPD_Y, m_INC_Speed_Y);
	DDV_MinMaxLong(pDX, m_INC_Speed_Y, 0, 8388607);
	DDX_Text(pDX, IDC_EDIT_INC_SPD_Z, m_INC_Speed_Z);
	DDV_MinMaxLong(pDX, m_INC_Speed_Z, 0, 8388607);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServoParam, CDialog)
	//{{AFX_MSG_MAP(CServoParam)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_LOAD, OnBtnLoad)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServoParam メッセージ ハンドラ

BOOL CServoParam::Create(CWnd *pWnd)
{
	m_pParent = pWnd;

	BOOL l_rc = CDialog::Create(CServoParam::IDD, m_pParent);
	if (l_rc == TRUE) {
		this->ShowWindow(SW_SHOW);
	}

	return l_rc;
}

int CServoParam::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	// メインウィンドウのアイコンを設定
	SetIcon(((CMotSysCfgDlg*) m_pParent)->m_hMainIcon, TRUE);
	SetIcon(((CMotSysCfgDlg*) m_pParent)->m_hMainIcon, FALSE);

	return 0;
}

void CServoParam::PostNcDestroy()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	((CMotSysCfgDlg*) m_pParent)->m_chkServo = FALSE;
	((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_SERVO)))->SetCheck(0);
	if (m_pParent != NULL) {
		((CMotSysCfgDlg*) m_pParent)->EndServoParam();
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CServoParam::OnOK()
{
//	CDialog::OnOK();
}

void CServoParam::OnCancel()
{
//	CDialog::OnCancel();
}

void CServoParam::OnClose()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}

typedef struct {
	long	ORG1_AccelTimeT;	// 原点復帰 第１加減速時間
	long	ORG1_AccelTimeS;	//				Ｓ字加減速時間
	long	ORG1_Speed;			//				移動速度
	long	ORG2_AccelTimeT;	//			第２加減速時間
	long	ORG2_AccelTimeS;	//				Ｓ字加減速時間
	long	ORG2_Speed;			//				移動速度
	long	ORG3_AccelTimeT;	//			第３加減速時間
	long	ORG3_AccelTimeS;	//				Ｓ字加減速時間
	long	ORG3_Speed;			//				移動速度
	long	HP_AccelTimeT;		// HP 移動	加減速時間
	long	HP_AccelTimeS;		//			Ｓ字加減速時間
	long	HP_BaseBand;		//			移動済判断基準
	long	HP_Speed;			//			速度
	long	ABS_AccelTimeT;		// ABS 移動	加減速時間
	long	ABS_AccelTimeS;		//			Ｓ字加減速時間
	long	ABS_Speed;			//			速度
	long	INC_AccelTimeT;		// INC 移動	加減速時間
	long	INC_AccelTimeS;		//			Ｓ字加減速時間
	long	INC_Speed;			//			速度
} SERVO_PARAM;

SERVO_PARAM	ServoParam[9];
static TCHAR tszServoParamName[MAX_PATH] = _T("..\\cfg\\ServoParam.dat");

void CServoParam::OnBtnLoad()
{
	TCHAR	tszPathName[MAX_PATH];
	FILE	*fp;
	TCHAR	l_tszText[256];

	_tcscpy(tszPathName, tszServoParamName);

	if ((fp = _tfopen(tszPathName, "r")) != NULL) {
		fread(ServoParam, sizeof(ServoParam), 1, fp);
		fclose(fp);
		DispData();
	} else {
		// ファイルが無かったので作成する
		_stprintf(l_tszText, _T("Create default data file[%s]."), tszPathName);
		::MessageBox(NULL, l_tszText, _T("Motion System Configure"), MB_OK | MB_SYSTEMMODAL);
		SetDefaultData();
		DispData();
		if ((fp = fopen(tszPathName, "w")) != NULL) {
			fwrite(ServoParam, sizeof(ServoParam), 1, fp);
			fclose(fp);
		} else {
			// デフォルトも作成できなかった
			_stprintf(l_tszText, _T("Could not create default data file[%s]."), tszPathName);
			::MessageBox(NULL, l_tszText, _T("Motion System Configure"), MB_OK | MB_SYSTEMMODAL);
		}
	}

}
void CServoParam::SetDefaultData()
{
	memset(ServoParam, 0, sizeof(ServoParam));
	// X 軸
	ServoParam[CC_AXIS_X].ORG1_AccelTimeT = 500;
	ServoParam[CC_AXIS_X].ORG1_AccelTimeS = 250;
	ServoParam[CC_AXIS_X].ORG1_Speed	  = 800;
	ServoParam[CC_AXIS_X].ORG2_AccelTimeT = 100;
	ServoParam[CC_AXIS_X].ORG2_AccelTimeS = 50;
	ServoParam[CC_AXIS_X].ORG2_Speed	  = 4;
	ServoParam[CC_AXIS_X].ORG3_AccelTimeT = 250;
	ServoParam[CC_AXIS_X].ORG3_AccelTimeS = 125;
	ServoParam[CC_AXIS_X].ORG3_Speed	  = 400;
	ServoParam[CC_AXIS_X].HP_AccelTimeT   = 2000;
	ServoParam[CC_AXIS_X].HP_AccelTimeS   = 1000;
	ServoParam[CC_AXIS_X].HP_BaseBand	  = 10;
	ServoParam[CC_AXIS_X].HP_Speed		  = 2000;
	ServoParam[CC_AXIS_X].ABS_AccelTimeT  = 2000;
	ServoParam[CC_AXIS_X].ABS_AccelTimeS  = 1000;
	ServoParam[CC_AXIS_X].ABS_Speed 	  = 2000;
	ServoParam[CC_AXIS_X].INC_AccelTimeT  = 2000;
	ServoParam[CC_AXIS_X].INC_AccelTimeS  = 1000;
	ServoParam[CC_AXIS_X].INC_Speed 	  = 2000;
	// Y 軸
	ServoParam[CC_AXIS_Y].ORG1_AccelTimeT = 500;
	ServoParam[CC_AXIS_Y].ORG1_AccelTimeS = 250;
	ServoParam[CC_AXIS_Y].ORG1_Speed	  = 600;
	ServoParam[CC_AXIS_Y].ORG2_AccelTimeT = 100;
	ServoParam[CC_AXIS_Y].ORG2_AccelTimeS = 50;
	ServoParam[CC_AXIS_Y].ORG2_Speed	  = 40;
	ServoParam[CC_AXIS_Y].ORG3_AccelTimeT = 250;
	ServoParam[CC_AXIS_Y].ORG3_AccelTimeS = 125;
	ServoParam[CC_AXIS_Y].ORG3_Speed	  = 400;
	ServoParam[CC_AXIS_Y].HP_AccelTimeT   = 500;
	ServoParam[CC_AXIS_Y].HP_AccelTimeS   = 250;
	ServoParam[CC_AXIS_Y].HP_BaseBand	  = 10;
	ServoParam[CC_AXIS_Y].HP_Speed		  = 2000;
	ServoParam[CC_AXIS_Y].ABS_AccelTimeT  = 500;
	ServoParam[CC_AXIS_Y].ABS_AccelTimeS  = 250;
	ServoParam[CC_AXIS_Y].ABS_Speed 	  = 2000;
	ServoParam[CC_AXIS_Y].INC_AccelTimeT  = 500;
	ServoParam[CC_AXIS_Y].INC_AccelTimeS  = 250;
	ServoParam[CC_AXIS_Y].INC_Speed 	  = 2000;
	// Z 軸
	ServoParam[CC_AXIS_Z].ORG1_AccelTimeT = 250;
	ServoParam[CC_AXIS_Z].ORG1_AccelTimeS = 125;
	ServoParam[CC_AXIS_Z].ORG1_Speed	  = 4;
	ServoParam[CC_AXIS_Z].ORG2_AccelTimeT = 100;
	ServoParam[CC_AXIS_Z].ORG2_AccelTimeS = 50;
	ServoParam[CC_AXIS_Z].ORG2_Speed	  = 2;
	ServoParam[CC_AXIS_Z].ORG3_AccelTimeT = 200;
	ServoParam[CC_AXIS_Z].ORG3_AccelTimeS = 100;
	ServoParam[CC_AXIS_Z].ORG3_Speed	  = 3;
	ServoParam[CC_AXIS_Z].HP_AccelTimeT   = 500;
	ServoParam[CC_AXIS_Z].HP_AccelTimeS   = 250;
	ServoParam[CC_AXIS_Z].HP_BaseBand	  = 2;
	ServoParam[CC_AXIS_Z].HP_Speed		  = 8;
	ServoParam[CC_AXIS_Z].ABS_AccelTimeT  = 500;
	ServoParam[CC_AXIS_Z].ABS_AccelTimeS  = 250;
	ServoParam[CC_AXIS_Z].ABS_Speed 	  = 8;
	ServoParam[CC_AXIS_Z].INC_AccelTimeT  = 500;
	ServoParam[CC_AXIS_Z].INC_AccelTimeS  = 250;
	ServoParam[CC_AXIS_Z].INC_Speed 	  = 8;
	// T 軸
	ServoParam[CC_AXIS_T].ORG1_AccelTimeT = 250;
	ServoParam[CC_AXIS_T].ORG1_AccelTimeS = 125;
	ServoParam[CC_AXIS_T].ORG1_Speed	  = 8;
	ServoParam[CC_AXIS_T].ORG2_AccelTimeT = 100;
	ServoParam[CC_AXIS_T].ORG2_AccelTimeS = 50;
	ServoParam[CC_AXIS_T].ORG2_Speed	  = 2;
	ServoParam[CC_AXIS_T].ORG3_AccelTimeT = 250;
	ServoParam[CC_AXIS_T].ORG3_AccelTimeS = 125;
	ServoParam[CC_AXIS_T].ORG3_Speed	  = 8;
	ServoParam[CC_AXIS_T].HP_AccelTimeT   = 500;
	ServoParam[CC_AXIS_T].HP_AccelTimeS   = 250;
	ServoParam[CC_AXIS_T].HP_BaseBand	  = 2;
	ServoParam[CC_AXIS_T].HP_Speed		  = 16;
	ServoParam[CC_AXIS_T].ABS_AccelTimeT  = 500;
	ServoParam[CC_AXIS_T].ABS_AccelTimeS  = 250;
	ServoParam[CC_AXIS_T].ABS_Speed 	  = 16;
	ServoParam[CC_AXIS_T].INC_AccelTimeT  = 500;
	ServoParam[CC_AXIS_T].INC_AccelTimeS  = 250;
	ServoParam[CC_AXIS_T].INC_Speed 	  = 16;
}

void CServoParam::OnBtnSave()
{
	TCHAR	tszPathName[MAX_PATH];
	FILE	*fp;
	TCHAR	l_tszText[256];

	_tcscpy(tszPathName, tszServoParamName);

	if ((fp = _tfopen(tszPathName, "w")) != NULL) {
		GetData();
		fwrite(ServoParam, sizeof(ServoParam), 1, fp);
		fclose(fp);
		DispData();
	} else {
		// ファイルが無かった
		_stprintf(l_tszText, _T("Could not open data file[%s]."), tszPathName);
		::MessageBox(NULL, l_tszText, _T("Motion System Configure"), MB_OK | MB_SYSTEMMODAL);
	}
}

void CServoParam::DispData()
{
	// X 軸
	m_ORG1_AccelTimeT_X = ServoParam[CC_AXIS_X].ORG1_AccelTimeT;
	m_ORG1_AccelTimeS_X = ServoParam[CC_AXIS_X].ORG1_AccelTimeS;
	m_ORG1_Speed_X		= ServoParam[CC_AXIS_X].ORG1_Speed	   ;
	m_ORG2_AccelTimeT_X = ServoParam[CC_AXIS_X].ORG2_AccelTimeT;
	m_ORG2_AccelTimeS_X = ServoParam[CC_AXIS_X].ORG2_AccelTimeS;
	m_ORG2_Speed_X		= ServoParam[CC_AXIS_X].ORG2_Speed	   ;
	m_ORG3_AccelTimeT_X = ServoParam[CC_AXIS_X].ORG3_AccelTimeT;
	m_ORG3_AccelTimeS_X = ServoParam[CC_AXIS_X].ORG3_AccelTimeS;
	m_ORG3_Speed_X		= ServoParam[CC_AXIS_X].ORG3_Speed	   ;
	m_HP_AccelTimeT_X	= ServoParam[CC_AXIS_X].HP_AccelTimeT  ;
	m_HP_AccelTimeS_X	= ServoParam[CC_AXIS_X].HP_AccelTimeS  ;
	m_HP_BaseBand_X 	= ServoParam[CC_AXIS_X].HP_BaseBand    ;
	m_HP_Speed_X		= ServoParam[CC_AXIS_X].HP_Speed	   ;
	m_ABS_AccelTimeT_X	= ServoParam[CC_AXIS_X].ABS_AccelTimeT ;
	m_ABS_AccelTimeS_X	= ServoParam[CC_AXIS_X].ABS_AccelTimeS ;
	m_ABS_Speed_X		= ServoParam[CC_AXIS_X].ABS_Speed	   ;
	m_INC_AccelTimeT_X	= ServoParam[CC_AXIS_X].INC_AccelTimeT ;
	m_INC_AccelTimeS_X	= ServoParam[CC_AXIS_X].INC_AccelTimeS ;
	m_INC_Speed_X		= ServoParam[CC_AXIS_X].INC_Speed	   ;
	// Y 軸
	m_ORG1_AccelTimeT_Y = ServoParam[CC_AXIS_Y].ORG1_AccelTimeT;
	m_ORG1_AccelTimeS_Y = ServoParam[CC_AXIS_Y].ORG1_AccelTimeS;
	m_ORG1_Speed_Y		= ServoParam[CC_AXIS_Y].ORG1_Speed	   ;
	m_ORG2_AccelTimeT_Y = ServoParam[CC_AXIS_Y].ORG2_AccelTimeT;
	m_ORG2_AccelTimeS_Y = ServoParam[CC_AXIS_Y].ORG2_AccelTimeS;
	m_ORG2_Speed_Y		= ServoParam[CC_AXIS_Y].ORG2_Speed	   ;
	m_ORG3_AccelTimeT_Y = ServoParam[CC_AXIS_Y].ORG3_AccelTimeT;
	m_ORG3_AccelTimeS_Y = ServoParam[CC_AXIS_Y].ORG3_AccelTimeS;
	m_ORG3_Speed_Y		= ServoParam[CC_AXIS_Y].ORG3_Speed	   ;
	m_HP_AccelTimeT_Y	= ServoParam[CC_AXIS_Y].HP_AccelTimeT  ;
	m_HP_AccelTimeS_Y	= ServoParam[CC_AXIS_Y].HP_AccelTimeS  ;
	m_HP_BaseBand_Y 	= ServoParam[CC_AXIS_Y].HP_BaseBand    ;
	m_HP_Speed_Y		= ServoParam[CC_AXIS_Y].HP_Speed	   ;
	m_ABS_AccelTimeT_Y	= ServoParam[CC_AXIS_Y].ABS_AccelTimeT ;
	m_ABS_AccelTimeS_Y	= ServoParam[CC_AXIS_Y].ABS_AccelTimeS ;
	m_ABS_Speed_Y		= ServoParam[CC_AXIS_Y].ABS_Speed	   ;
	m_INC_AccelTimeT_Y	= ServoParam[CC_AXIS_Y].INC_AccelTimeT ;
	m_INC_AccelTimeS_Y	= ServoParam[CC_AXIS_Y].INC_AccelTimeS ;
	m_INC_Speed_Y		= ServoParam[CC_AXIS_Y].INC_Speed	   ;
	// Z 軸
	m_ORG1_AccelTimeT_Z = ServoParam[CC_AXIS_Z].ORG1_AccelTimeT;
	m_ORG1_AccelTimeS_Z = ServoParam[CC_AXIS_Z].ORG1_AccelTimeS;
	m_ORG1_Speed_Z		= ServoParam[CC_AXIS_Z].ORG1_Speed	   ;
	m_ORG2_AccelTimeT_Z = ServoParam[CC_AXIS_Z].ORG2_AccelTimeT;
	m_ORG2_AccelTimeS_Z = ServoParam[CC_AXIS_Z].ORG2_AccelTimeS;
	m_ORG2_Speed_Z		= ServoParam[CC_AXIS_Z].ORG2_Speed	   ;
	m_ORG3_AccelTimeT_Z = ServoParam[CC_AXIS_Z].ORG3_AccelTimeT;
	m_ORG3_AccelTimeS_Z = ServoParam[CC_AXIS_Z].ORG3_AccelTimeS;
	m_ORG3_Speed_Z		= ServoParam[CC_AXIS_Z].ORG3_Speed	   ;
	m_HP_AccelTimeT_Z	= ServoParam[CC_AXIS_Z].HP_AccelTimeT  ;
	m_HP_AccelTimeS_Z	= ServoParam[CC_AXIS_Z].HP_AccelTimeS  ;
	m_HP_BaseBand_Z 	= ServoParam[CC_AXIS_Z].HP_BaseBand    ;
	m_HP_Speed_Z		= ServoParam[CC_AXIS_Z].HP_Speed	   ;
	m_ABS_AccelTimeT_Z	= ServoParam[CC_AXIS_Z].ABS_AccelTimeT ;
	m_ABS_AccelTimeS_Z	= ServoParam[CC_AXIS_Z].ABS_AccelTimeS ;
	m_ABS_Speed_Z		= ServoParam[CC_AXIS_Z].ABS_Speed	   ;
	m_INC_AccelTimeT_Z	= ServoParam[CC_AXIS_Z].INC_AccelTimeT ;
	m_INC_AccelTimeS_Z	= ServoParam[CC_AXIS_Z].INC_AccelTimeS ;
	m_INC_Speed_Z		= ServoParam[CC_AXIS_Z].INC_Speed	   ;
	// T 軸
	m_ORG1_AccelTimeT_T = ServoParam[CC_AXIS_T].ORG1_AccelTimeT;
	m_ORG1_AccelTimeS_T = ServoParam[CC_AXIS_T].ORG1_AccelTimeS;
	m_ORG1_Speed_T		= ServoParam[CC_AXIS_T].ORG1_Speed	   ;
	m_ORG2_AccelTimeT_T = ServoParam[CC_AXIS_T].ORG2_AccelTimeT;
	m_ORG2_AccelTimeS_T = ServoParam[CC_AXIS_T].ORG2_AccelTimeS;
	m_ORG2_Speed_T		= ServoParam[CC_AXIS_T].ORG2_Speed	   ;
	m_ORG3_AccelTimeT_T = ServoParam[CC_AXIS_T].ORG3_AccelTimeT;
	m_ORG3_AccelTimeS_T = ServoParam[CC_AXIS_T].ORG3_AccelTimeS;
	m_ORG3_Speed_T		= ServoParam[CC_AXIS_T].ORG3_Speed	   ;
	m_HP_AccelTimeT_T	= ServoParam[CC_AXIS_T].HP_AccelTimeT  ;
	m_HP_AccelTimeS_T	= ServoParam[CC_AXIS_T].HP_AccelTimeS  ;
	m_HP_BaseBand_T 	= ServoParam[CC_AXIS_T].HP_BaseBand    ;
	m_HP_Speed_T		= ServoParam[CC_AXIS_T].HP_Speed	   ;
	m_ABS_AccelTimeT_T	= ServoParam[CC_AXIS_T].ABS_AccelTimeT ;
	m_ABS_AccelTimeS_T	= ServoParam[CC_AXIS_T].ABS_AccelTimeS ;
	m_ABS_Speed_T		= ServoParam[CC_AXIS_T].ABS_Speed	   ;
	m_INC_AccelTimeT_T	= ServoParam[CC_AXIS_T].INC_AccelTimeT ;
	m_INC_AccelTimeS_T	= ServoParam[CC_AXIS_T].INC_AccelTimeS ;
	m_INC_Speed_T		= ServoParam[CC_AXIS_T].INC_Speed	   ;

	UpdateData(FALSE);
}

void CServoParam::GetData()
{
	UpdateData(TRUE);

	// X 軸
	ServoParam[CC_AXIS_X].ORG1_AccelTimeT = m_ORG1_AccelTimeT_X;
	ServoParam[CC_AXIS_X].ORG1_AccelTimeS = m_ORG1_AccelTimeS_X;
	ServoParam[CC_AXIS_X].ORG1_Speed	  = m_ORG1_Speed_X;
	ServoParam[CC_AXIS_X].ORG2_AccelTimeT = m_ORG2_AccelTimeT_X;
	ServoParam[CC_AXIS_X].ORG2_AccelTimeS = m_ORG2_AccelTimeS_X;
	ServoParam[CC_AXIS_X].ORG2_Speed	  = m_ORG2_Speed_X;
	ServoParam[CC_AXIS_X].ORG3_AccelTimeT = m_ORG3_AccelTimeT_X;
	ServoParam[CC_AXIS_X].ORG3_AccelTimeS = m_ORG3_AccelTimeS_X;
	ServoParam[CC_AXIS_X].ORG3_Speed	  = m_ORG3_Speed_X;
	ServoParam[CC_AXIS_X].HP_AccelTimeT   = m_HP_AccelTimeT_X;
	ServoParam[CC_AXIS_X].HP_AccelTimeS   = m_HP_AccelTimeS_X;
	ServoParam[CC_AXIS_X].HP_BaseBand	  = m_HP_BaseBand_X;
	ServoParam[CC_AXIS_X].HP_Speed		  = m_HP_Speed_X;
	ServoParam[CC_AXIS_X].ABS_AccelTimeT  = m_ABS_AccelTimeT_X;
	ServoParam[CC_AXIS_X].ABS_AccelTimeS  = m_ABS_AccelTimeS_X;
	ServoParam[CC_AXIS_X].ABS_Speed 	  = m_ABS_Speed_X;
	ServoParam[CC_AXIS_X].INC_AccelTimeT  = m_INC_AccelTimeT_X;
	ServoParam[CC_AXIS_X].INC_AccelTimeS  = m_INC_AccelTimeS_X;
	ServoParam[CC_AXIS_X].INC_Speed 	  = m_INC_Speed_X;
	// Y 軸
	ServoParam[CC_AXIS_Y].ORG1_AccelTimeT = m_ORG1_AccelTimeT_Y;
	ServoParam[CC_AXIS_Y].ORG1_AccelTimeS = m_ORG1_AccelTimeS_Y;
	ServoParam[CC_AXIS_Y].ORG1_Speed	  = m_ORG1_Speed_Y;
	ServoParam[CC_AXIS_Y].ORG2_AccelTimeT = m_ORG2_AccelTimeT_Y;
	ServoParam[CC_AXIS_Y].ORG2_AccelTimeS = m_ORG2_AccelTimeS_Y;
	ServoParam[CC_AXIS_Y].ORG2_Speed	  = m_ORG2_Speed_Y;
	ServoParam[CC_AXIS_Y].ORG3_AccelTimeT = m_ORG3_AccelTimeT_Y;
	ServoParam[CC_AXIS_Y].ORG3_AccelTimeS = m_ORG3_AccelTimeS_Y;
	ServoParam[CC_AXIS_Y].ORG3_Speed	  = m_ORG3_Speed_Y;
	ServoParam[CC_AXIS_Y].HP_AccelTimeT   = m_HP_AccelTimeT_Y;
	ServoParam[CC_AXIS_Y].HP_AccelTimeS   = m_HP_AccelTimeS_Y;
	ServoParam[CC_AXIS_Y].HP_BaseBand	  = m_HP_BaseBand_Y;
	ServoParam[CC_AXIS_Y].HP_Speed		  = m_HP_Speed_Y;
	ServoParam[CC_AXIS_Y].ABS_AccelTimeT  = m_ABS_AccelTimeT_Y;
	ServoParam[CC_AXIS_Y].ABS_AccelTimeS  = m_ABS_AccelTimeS_Y;
	ServoParam[CC_AXIS_Y].ABS_Speed 	  = m_ABS_Speed_Y;
	ServoParam[CC_AXIS_Y].INC_AccelTimeT  = m_INC_AccelTimeT_Y;
	ServoParam[CC_AXIS_Y].INC_AccelTimeS  = m_INC_AccelTimeS_Y;
	ServoParam[CC_AXIS_Y].INC_Speed 	  = m_INC_Speed_Y;
	// Z 軸
	ServoParam[CC_AXIS_Z].ORG1_AccelTimeT = m_ORG1_AccelTimeT_Z;
	ServoParam[CC_AXIS_Z].ORG1_AccelTimeS = m_ORG1_AccelTimeS_Z;
	ServoParam[CC_AXIS_Z].ORG1_Speed	  = m_ORG1_Speed_Z;
	ServoParam[CC_AXIS_Z].ORG2_AccelTimeT = m_ORG2_AccelTimeT_Z;
	ServoParam[CC_AXIS_Z].ORG2_AccelTimeS = m_ORG2_AccelTimeS_Z;
	ServoParam[CC_AXIS_Z].ORG2_Speed	  = m_ORG2_Speed_Z;
	ServoParam[CC_AXIS_Z].ORG3_AccelTimeT = m_ORG3_AccelTimeT_Z;
	ServoParam[CC_AXIS_Z].ORG3_AccelTimeS = m_ORG3_AccelTimeS_Z;
	ServoParam[CC_AXIS_Z].ORG3_Speed	  = m_ORG3_Speed_Z;
	ServoParam[CC_AXIS_Z].HP_AccelTimeT   = m_HP_AccelTimeT_Z;
	ServoParam[CC_AXIS_Z].HP_AccelTimeS   = m_HP_AccelTimeS_Z;
	ServoParam[CC_AXIS_Z].HP_BaseBand	  = m_HP_BaseBand_Z;
	ServoParam[CC_AXIS_Z].HP_Speed		  = m_HP_Speed_Z;
	ServoParam[CC_AXIS_Z].ABS_AccelTimeT  = m_ABS_AccelTimeT_Z;
	ServoParam[CC_AXIS_Z].ABS_AccelTimeS  = m_ABS_AccelTimeS_Z;
	ServoParam[CC_AXIS_Z].ABS_Speed 	  = m_ABS_Speed_Z;
	ServoParam[CC_AXIS_Z].INC_AccelTimeT  = m_INC_AccelTimeT_Z;
	ServoParam[CC_AXIS_Z].INC_AccelTimeS  = m_INC_AccelTimeS_Z;
	ServoParam[CC_AXIS_Z].INC_Speed 	  = m_INC_Speed_Z;
	// T 軸
	ServoParam[CC_AXIS_T].ORG1_AccelTimeT = m_ORG1_AccelTimeT_T;
	ServoParam[CC_AXIS_T].ORG1_AccelTimeS = m_ORG1_AccelTimeS_T;
	ServoParam[CC_AXIS_T].ORG1_Speed	  = m_ORG1_Speed_T;
	ServoParam[CC_AXIS_T].ORG2_AccelTimeT = m_ORG2_AccelTimeT_T;
	ServoParam[CC_AXIS_T].ORG2_AccelTimeS = m_ORG2_AccelTimeS_T;
	ServoParam[CC_AXIS_T].ORG2_Speed	  = m_ORG2_Speed_T;
	ServoParam[CC_AXIS_T].ORG3_AccelTimeT = m_ORG3_AccelTimeT_T;
	ServoParam[CC_AXIS_T].ORG3_AccelTimeS = m_ORG3_AccelTimeS_T;
	ServoParam[CC_AXIS_T].ORG3_Speed	  = m_ORG3_Speed_T;
	ServoParam[CC_AXIS_T].HP_AccelTimeT   = m_HP_AccelTimeT_T;
	ServoParam[CC_AXIS_T].HP_AccelTimeS   = m_HP_AccelTimeS_T;
	ServoParam[CC_AXIS_T].HP_BaseBand	  = m_HP_BaseBand_T;
	ServoParam[CC_AXIS_T].HP_Speed		  = m_HP_Speed_T;
	ServoParam[CC_AXIS_T].ABS_AccelTimeT  = m_ABS_AccelTimeT_T;
	ServoParam[CC_AXIS_T].ABS_AccelTimeS  = m_ABS_AccelTimeS_T;
	ServoParam[CC_AXIS_T].ABS_Speed 	  = m_ABS_Speed_T;
	ServoParam[CC_AXIS_T].INC_AccelTimeT  = m_INC_AccelTimeT_T;
	ServoParam[CC_AXIS_T].INC_AccelTimeS  = m_INC_AccelTimeS_T;
	ServoParam[CC_AXIS_T].INC_Speed 	  = m_INC_Speed_T;
}

#define	SERVO_INI	_T("..\\cfg\\Servo.ini")
BOOL CServoParam::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	// Servo.ini による使用/未使用の表示
	TCHAR l_tszText[256];
	GetPrivateProfileString(_T("Axis"), _T("EnableAxisZ"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	if (0 != _tcscmp(l_tszText, _T("1"))) {
		GetDlgItem(IDC_STATIC_LBL_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG1_ATT_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG1_ATS_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG1_SPD_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG2_ATT_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG2_ATS_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG2_SPD_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG3_ATT_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG3_ATS_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG3_SPD_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HP_ATT_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HP_ATS_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HP_BASE_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HP_SPD_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ABS_ATT_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ABS_ATS_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ABS_SPD_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INC_ATT_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INC_ATS_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INC_SPD_Z)->EnableWindow(FALSE);
	}
	GetPrivateProfileString(_T("Axis"), _T("EnableAxisT"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	if (0 != _tcscmp(l_tszText, _T("1"))) {
		GetDlgItem(IDC_STATIC_LBL_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG1_ATT_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG1_ATS_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG1_SPD_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG2_ATT_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG2_ATS_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG2_SPD_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG3_ATT_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG3_ATS_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ORG3_SPD_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HP_ATT_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HP_ATS_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HP_BASE_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HP_SPD_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ABS_ATT_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ABS_ATS_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ABS_SPD_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INC_ATT_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INC_ATS_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INC_SPD_T)->EnableWindow(FALSE);
	}

	// 最初にロードしておく
	OnBtnLoad();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
