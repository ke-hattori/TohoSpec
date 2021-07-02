// MotCfg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MotSysCfg.h"
#include "MotCfg.h"
#include "MotSysCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct MOT_AXIS_DATAtag {
	WORD	wHctlPort;		// HCTL アドレス
	WORD	wHomePort;		// 原点センサアドレス
	WORD	wHomeIntPort;	// 原点センサ割込アドレス
	WORD	wXstopPort;		// 停止指令アドレス
	BYTE	byHomeMask;		// 原点センサビット位置
	BYTE	byHomeIntMask;	// 原点センサ割込ビット
	BYTE	byXstopMask;	// 停止指令ビット
	BYTE	byUnused;		// 未使用
} MOT_AXIS_DATA;

static MOT_AXIS_DATA axis_mot_data[8];
WORD motion_base_port[2];
short alloc_axis = 8;
char szAxisName[8][10];
static char szCfgName[12] = "mot_cfg";

/////////////////////////////////////////////////////////////////////////////
// CMotCfg ダイアログ


CMotCfg::CMotCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotCfg)
	m_BaseAddr1 = _T("");
	m_BaseAddr2 = _T("");
	m_AxisNum = 0;
	m_AxisNameBigX = _T("");
	m_AxisNameBigY = _T("");
	m_AxisNameLevel = _T("");
	m_AxisNameTheta = _T("");
	m_AxisNameXFast = _T("");
	m_AxisNameXSlow = _T("");
	m_AxisNameY = _T("");
	m_AxisNameZ = _T("");
	m_HCTLPortBigX = _T("");
	m_HCTLPortBigY = _T("");
	m_HCTLPortLevel = _T("");
	m_HCTLPortTheta = _T("");
	m_HCTLPortXFast = _T("");
	m_HCTLPortXSlow = _T("");
	m_HCTLPortY = _T("");
	m_HCTLPortZ = _T("");
	m_HomeBitBigX = _T("");
	m_HomeBitBigY = _T("");
	m_HomeBitLevel = _T("");
	m_HomeBitTheta = _T("");
	m_HomeBitXFast = _T("");
	m_HomeBitXSlow = _T("");
	m_HomeBitY = _T("");
	m_HomeBitZ = _T("");
	m_HomeIntBitBigX = _T("");
	m_HomeIntBitBigY = _T("");
	m_HomeIntBitLevel = _T("");
	m_HomeIntBitTheta = _T("");
	m_HomeIntBitXFast = _T("");
	m_HomeIntBitXSlow = _T("");
	m_HomeIntBitY = _T("");
	m_HomeIntBitZ = _T("");
	m_HomeIntPortBigX = _T("");
	m_HomeIntPortBigY = _T("");
	m_HomeIntPortLevel = _T("");
	m_HomeIntPortTheta = _T("");
	m_HomeIntPortXFast = _T("");
	m_HomeIntPortXSlow = _T("");
	m_HomeIntPortY = _T("");
	m_HomeIntPortZ = _T("");
	m_HomePortBigX = _T("");
	m_HomePortBigY = _T("");
	m_HomePortLevel = _T("");
	m_HomePortTheta = _T("");
	m_HomePortXFast = _T("");
	m_HomePortXSlow = _T("");
	m_HomePortY = _T("");
	m_HomePortZ = _T("");
	m_StopBitBigX = _T("");
	m_StopBitBigY = _T("");
	m_StopBitLevel = _T("");
	m_StopBitTheta = _T("");
	m_StopBitXFast = _T("");
	m_StopBitXSlow = _T("");
	m_StopBitY = _T("");
	m_StopBitZ = _T("");
	m_UnusedBigX = _T("");
	m_UnusedBigY = _T("");
	m_UnusedLevel = _T("");
	m_UnusedTheta = _T("");
	m_UnusedXFast = _T("");
	m_UnusedXSlow = _T("");
	m_UnusedY = _T("");
	m_UnusedZ = _T("");
	m_StopPortBigX = _T("");
	m_StopPortBigY = _T("");
	m_StopPortLevel = _T("");
	m_StopPortTheta = _T("");
	m_StopPortXFast = _T("");
	m_StopPortXSlow = _T("");
	m_StopPortY = _T("");
	m_StopPortZ = _T("");
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CMotCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotCfg)
	DDX_Text(pDX, IDC_EDIT_BASE_ADDR1, m_BaseAddr1);
	DDV_MaxChars(pDX, m_BaseAddr1, 4);
	DDX_Text(pDX, IDC_EDIT_BASE_ADDR2, m_BaseAddr2);
	DDV_MaxChars(pDX, m_BaseAddr2, 4);
	DDX_Text(pDX, IDC_EDIT_BASE_AXISNUM, m_AxisNum);
	DDX_Text(pDX, IDC_EDIT_AXIS_NAME_BIGX, m_AxisNameBigX);
	DDX_Text(pDX, IDC_EDIT_AXIS_NAME_BIGY, m_AxisNameBigY);
	DDX_Text(pDX, IDC_EDIT_AXIS_NAME_LEVEL, m_AxisNameLevel);
	DDX_Text(pDX, IDC_EDIT_AXIS_NAME_THETA, m_AxisNameTheta);
	DDX_Text(pDX, IDC_EDIT_AXIS_NAME_XFAST, m_AxisNameXFast);
	DDX_Text(pDX, IDC_EDIT_AXIS_NAME_XSLOW, m_AxisNameXSlow);
	DDX_Text(pDX, IDC_EDIT_AXIS_NAME_Y, m_AxisNameY);
	DDX_Text(pDX, IDC_EDIT_AXIS_NAME_Z, m_AxisNameZ);
	DDX_Text(pDX, IDC_EDIT_HCTLPORT_BIGX, m_HCTLPortBigX);
	DDX_Text(pDX, IDC_EDIT_HCTLPORT_BIGY, m_HCTLPortBigY);
	DDX_Text(pDX, IDC_EDIT_HCTLPORT_LEVEL, m_HCTLPortLevel);
	DDX_Text(pDX, IDC_EDIT_HCTLPORT_THETA, m_HCTLPortTheta);
	DDX_Text(pDX, IDC_EDIT_HCTLPORT_XFAST, m_HCTLPortXFast);
	DDX_Text(pDX, IDC_EDIT_HCTLPORT_XSLOW, m_HCTLPortXSlow);
	DDX_Text(pDX, IDC_EDIT_HCTLPORT_Y, m_HCTLPortY);
	DDX_Text(pDX, IDC_EDIT_HCTLPORT_Z, m_HCTLPortZ);
	DDX_Text(pDX, IDC_EDIT_HOMEBIT_BIGX, m_HomeBitBigX);
	DDX_Text(pDX, IDC_EDIT_HOMEBIT_BIGY, m_HomeBitBigY);
	DDX_Text(pDX, IDC_EDIT_HOMEBIT_LEVEL, m_HomeBitLevel);
	DDX_Text(pDX, IDC_EDIT_HOMEBIT_THETA, m_HomeBitTheta);
	DDX_Text(pDX, IDC_EDIT_HOMEBIT_XFAST, m_HomeBitXFast);
	DDX_Text(pDX, IDC_EDIT_HOMEBIT_XSLOW, m_HomeBitXSlow);
	DDX_Text(pDX, IDC_EDIT_HOMEBIT_Y, m_HomeBitY);
	DDX_Text(pDX, IDC_EDIT_HOMEBIT_Z, m_HomeBitZ);
	DDX_Text(pDX, IDC_EDIT_HOMEINTBIT_BIGX, m_HomeIntBitBigX);
	DDX_Text(pDX, IDC_EDIT_HOMEINTBIT_BIGY, m_HomeIntBitBigY);
	DDX_Text(pDX, IDC_EDIT_HOMEINTBIT_LEVEL, m_HomeIntBitLevel);
	DDX_Text(pDX, IDC_EDIT_HOMEINTBIT_THETA, m_HomeIntBitTheta);
	DDX_Text(pDX, IDC_EDIT_HOMEINTBIT_XFAST, m_HomeIntBitXFast);
	DDX_Text(pDX, IDC_EDIT_HOMEINTBIT_XSLOW, m_HomeIntBitXSlow);
	DDX_Text(pDX, IDC_EDIT_HOMEINTBIT_Y, m_HomeIntBitY);
	DDX_Text(pDX, IDC_EDIT_HOMEINTBIT_Z, m_HomeIntBitZ);
	DDX_Text(pDX, IDC_EDIT_HOMEINTPORT_BIGX, m_HomeIntPortBigX);
	DDX_Text(pDX, IDC_EDIT_HOMEINTPORT_BIGY, m_HomeIntPortBigY);
	DDX_Text(pDX, IDC_EDIT_HOMEINTPORT_LEVEL, m_HomeIntPortLevel);
	DDX_Text(pDX, IDC_EDIT_HOMEINTPORT_THETA, m_HomeIntPortTheta);
	DDX_Text(pDX, IDC_EDIT_HOMEINTPORT_XFAST, m_HomeIntPortXFast);
	DDX_Text(pDX, IDC_EDIT_HOMEINTPORT_XSLOW, m_HomeIntPortXSlow);
	DDX_Text(pDX, IDC_EDIT_HOMEINTPORT_Y, m_HomeIntPortY);
	DDX_Text(pDX, IDC_EDIT_HOMEINTPORT_Z, m_HomeIntPortZ);
	DDX_Text(pDX, IDC_EDIT_HOMEPORT_BIGX, m_HomePortBigX);
	DDX_Text(pDX, IDC_EDIT_HOMEPORT_BIGY, m_HomePortBigY);
	DDX_Text(pDX, IDC_EDIT_HOMEPORT_LEVEL, m_HomePortLevel);
	DDX_Text(pDX, IDC_EDIT_HOMEPORT_THETA, m_HomePortTheta);
	DDX_Text(pDX, IDC_EDIT_HOMEPORT_XFAST, m_HomePortXFast);
	DDX_Text(pDX, IDC_EDIT_HOMEPORT_XSLOW, m_HomePortXSlow);
	DDX_Text(pDX, IDC_EDIT_HOMEPORT_Y, m_HomePortY);
	DDX_Text(pDX, IDC_EDIT_HOMEPORT_Z, m_HomePortZ);
	DDX_Text(pDX, IDC_EDIT_STOPBIT_BIGX, m_StopBitBigX);
	DDX_Text(pDX, IDC_EDIT_STOPBIT_BIGY, m_StopBitBigY);
	DDX_Text(pDX, IDC_EDIT_STOPBIT_LEVEL, m_StopBitLevel);
	DDX_Text(pDX, IDC_EDIT_STOPBIT_THETA, m_StopBitTheta);
	DDX_Text(pDX, IDC_EDIT_STOPBIT_XFAST, m_StopBitXFast);
	DDX_Text(pDX, IDC_EDIT_STOPBIT_XSLOW, m_StopBitXSlow);
	DDX_Text(pDX, IDC_EDIT_STOPBIT_Y, m_StopBitY);
	DDX_Text(pDX, IDC_EDIT_STOPBIT_Z, m_StopBitZ);
	DDX_Text(pDX, IDC_EDIT_UNUSED_BIGX, m_UnusedBigX);
	DDX_Text(pDX, IDC_EDIT_UNUSED_BIGY, m_UnusedBigY);
	DDX_Text(pDX, IDC_EDIT_UNUSED_LEVEL, m_UnusedLevel);
	DDX_Text(pDX, IDC_EDIT_UNUSED_THETA, m_UnusedTheta);
	DDX_Text(pDX, IDC_EDIT_UNUSED_XFAST, m_UnusedXFast);
	DDX_Text(pDX, IDC_EDIT_UNUSED_XSLOW, m_UnusedXSlow);
	DDX_Text(pDX, IDC_EDIT_UNUSED_Y, m_UnusedY);
	DDX_Text(pDX, IDC_EDIT_UNUSED_Z, m_UnusedZ);
	DDX_Text(pDX, IDC_EDIT_STOPPORT_BIGX, m_StopPortBigX);
	DDX_Text(pDX, IDC_EDIT_STOPPORT_BIGY, m_StopPortBigY);
	DDX_Text(pDX, IDC_EDIT_STOPPORT_LEVEL, m_StopPortLevel);
	DDX_Text(pDX, IDC_EDIT_STOPPORT_THETA, m_StopPortTheta);
	DDX_Text(pDX, IDC_EDIT_STOPPORT_XFAST, m_StopPortXFast);
	DDX_Text(pDX, IDC_EDIT_STOPPORT_XSLOW, m_StopPortXSlow);
	DDX_Text(pDX, IDC_EDIT_STOPPORT_Y, m_StopPortY);
	DDX_Text(pDX, IDC_EDIT_STOPPORT_Z, m_StopPortZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMotCfg, CDialog)
	//{{AFX_MSG_MAP(CMotCfg)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_LOAD, OnBtnLoad)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotCfg メッセージ ハンドラ

BOOL CMotCfg::Create(CWnd *pWnd)
{
	m_pParent = pWnd;

	BOOL l_rc = CDialog::Create(CMotCfg::IDD, m_pParent);
	if (l_rc == TRUE) {
		this->ShowWindow(SW_SHOW);
	}

	return l_rc;
}


void CMotCfg::PostNcDestroy()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	((CMotSysCfgDlg*) m_pParent)->m_chkMotCfg = FALSE;
	((CButton*) (((CMotSysCfgDlg*) m_pParent)->GetDlgItem(IDC_CHK_MOTCFG)))->SetCheck(0);
	if (m_pParent != NULL) {
		((CMotSysCfgDlg*) m_pParent)->EndMotCfgDlg();
		delete this;
	}

	CDialog::PostNcDestroy();
}

void CMotCfg::OnClose()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	if (m_pParent != NULL) {
		DestroyWindow();
	} else {
		CDialog::OnClose();
	}
}

int CMotCfg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	// メインウィンドウのアイコンを設定
	SetIcon(((CMotSysCfgDlg*) m_pParent)->m_hMainIcon, TRUE);
	SetIcon(((CMotSysCfgDlg*) m_pParent)->m_hMainIcon, FALSE);

	return 0;
}

void CMotCfg::OnOK()
{
//	CDialog::OnOK();
}

void CMotCfg::OnCancel()
{
//	CDialog::OnCancel();
}

void CMotCfg::OnBtnLoad()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	int		fd;
	char	szPathName[128];
	int		i, j;
	char	l_cText[255];

	/* Windows ディレクトリのパスを取得 */
	GetWindowsDirectory(szPathName, 128);
	lstrcat(szPathName, "\\");
	lstrcat(szPathName, szCfgName);
	lstrcat(szPathName, ".dat");

	if ((fd = _lopen(szPathName, OF_READ)) >= 0) {
		/* ファイル「mot_cfg.dat」オープン出来た*/
		/* motion_base_port[0],[1]を設定 */
		_lread(fd, (LPSTR) motion_base_port, 2 * sizeof(WORD));
		/* 軸数 alloc_axis を設定 */
		_lread(fd, (LPSTR) &alloc_axis, sizeof(short));
		/* 軸毎のデータ読出し */
		for (i = 0; i < alloc_axis; i++) {
			/* 軸名を読出す[８文字](BIGX など) */
			_lread(fd, (LPSTR) szAxisName[i], 8);
			/* 軸名の終端に 0x00 を入れる */
			for (j = 0; j < 8; j++) {
				char c;
				if ((c = szAxisName[i][j]) == ' ' || c == '\0')
				   break;
			}
			szAxisName[i][j] = 0;
			/* 軸データを読出す */
			_lread(fd, (LPSTR) &axis_mot_data[i], sizeof(MOT_AXIS_DATA));
		}
		_lclose(fd);
		// データ表示
		DispData();
	} else {
		// ファイルオープンエラー
		sprintf(l_cText, "Failed to open(read) file[%s]", szPathName);
		::MessageBox(NULL, l_cText, "Motion Config Error", MB_OK | MB_SYSTEMMODAL);
	}
}

void CMotCfg::OnBtnSave()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	int		fd;
	char	szPathName[128];
	int		i;
	char	l_cText[255];

	/* Windows ディレクトリのパスを取得 */
	GetWindowsDirectory(szPathName, 128);
	lstrcat(szPathName, "\\");
	lstrcat(szPathName, szCfgName);
	lstrcat(szPathName, ".dat");

	// データ取得
	GetData();

	if ((fd = _lcreat(szPathName, 0)) >= 0) {
		/* ファイル「mot_cfg.dat」が作成出来た */
		/* motion_base_port[0],[1] を書込み */
		_lwrite(fd, (LPSTR) motion_base_port, 2 * sizeof(WORD));
		/* 軸数を書込み */
		_lwrite(fd, (LPSTR) &alloc_axis, sizeof(short));
		/* 軸毎の書込み */
		for (i = 0; i < alloc_axis; i++) {
			/* 軸名を書込み */
			_lwrite(fd, (LPSTR) szAxisName[i], 8);
			/* 軸データを書込み */
			_lwrite(fd, (LPSTR) &axis_mot_data[i], sizeof(MOT_AXIS_DATA));
		}
		_lclose(fd);
		// データ表示
		DispData();
	} else {
		// ファイルオープンエラー
		sprintf(l_cText, "Failed to open(write) file[%s]", szPathName);
		::MessageBox(NULL, l_cText, "Motion Config Error", MB_OK | MB_SYSTEMMODAL);
	}
}

// データを表示
void CMotCfg::DispData()
{
	m_BaseAddr1.Format("%04x", motion_base_port[0]);
	m_BaseAddr2.Format("%04x", motion_base_port[1]);
	m_AxisNum = alloc_axis;
	m_AxisNameBigX	= szAxisName[0];
	m_AxisNameBigY	= szAxisName[1];
	m_AxisNameZ 	= szAxisName[2];
	m_AxisNameXSlow = szAxisName[3];
	m_AxisNameTheta = szAxisName[4];
	m_AxisNameLevel = szAxisName[5];
	m_AxisNameXFast = szAxisName[6];
	m_AxisNameY 	= szAxisName[7];
	m_HCTLPortBigX.Format( "%04x", axis_mot_data[0].wHctlPort);
	m_HCTLPortBigY.Format( "%04x", axis_mot_data[1].wHctlPort);
	m_HCTLPortZ.Format(    "%04x", axis_mot_data[2].wHctlPort);
	m_HCTLPortXSlow.Format("%04x", axis_mot_data[3].wHctlPort);
	m_HCTLPortTheta.Format("%04x", axis_mot_data[4].wHctlPort);
	m_HCTLPortLevel.Format("%04x", axis_mot_data[5].wHctlPort);
	m_HCTLPortXFast.Format("%04x", axis_mot_data[6].wHctlPort);
	m_HCTLPortY.Format(    "%04x", axis_mot_data[7].wHctlPort);
	m_HomePortBigX.Format( "%04x", axis_mot_data[0].wHomePort);
	m_HomePortBigY.Format( "%04x", axis_mot_data[1].wHomePort);
	m_HomePortZ.Format(    "%04x", axis_mot_data[2].wHomePort);
	m_HomePortXSlow.Format("%04x", axis_mot_data[3].wHomePort);
	m_HomePortTheta.Format("%04x", axis_mot_data[4].wHomePort);
	m_HomePortLevel.Format("%04x", axis_mot_data[5].wHomePort);
	m_HomePortXFast.Format("%04x", axis_mot_data[6].wHomePort);
	m_HomePortY.Format(    "%04x", axis_mot_data[7].wHomePort);
	m_HomeIntPortBigX.Format( "%04x", axis_mot_data[0].wHomeIntPort);
	m_HomeIntPortBigY.Format( "%04x", axis_mot_data[1].wHomeIntPort);
	m_HomeIntPortZ.Format(	  "%04x", axis_mot_data[2].wHomeIntPort);
	m_HomeIntPortXSlow.Format("%04x", axis_mot_data[3].wHomeIntPort);
	m_HomeIntPortTheta.Format("%04x", axis_mot_data[4].wHomeIntPort);
	m_HomeIntPortLevel.Format("%04x", axis_mot_data[5].wHomeIntPort);
	m_HomeIntPortXFast.Format("%04x", axis_mot_data[6].wHomeIntPort);
	m_HomeIntPortY.Format(	  "%04x", axis_mot_data[7].wHomeIntPort);
	m_StopPortBigX.Format( "%04x", axis_mot_data[0].wXstopPort);
	m_StopPortBigY.Format( "%04x", axis_mot_data[1].wXstopPort);
	m_StopPortZ.Format(    "%04x", axis_mot_data[2].wXstopPort);
	m_StopPortXSlow.Format("%04x", axis_mot_data[3].wXstopPort);
	m_StopPortTheta.Format("%04x", axis_mot_data[4].wXstopPort);
	m_StopPortLevel.Format("%04x", axis_mot_data[5].wXstopPort);
	m_StopPortXFast.Format("%04x", axis_mot_data[6].wXstopPort);
	m_StopPortY.Format(    "%04x", axis_mot_data[7].wXstopPort);
	m_HomeBitBigX.Format( "%04x", axis_mot_data[0].byHomeMask);
	m_HomeBitBigY.Format( "%04x", axis_mot_data[1].byHomeMask);
	m_HomeBitZ.Format(	  "%04x", axis_mot_data[2].byHomeMask);
	m_HomeBitXSlow.Format("%04x", axis_mot_data[3].byHomeMask);
	m_HomeBitTheta.Format("%04x", axis_mot_data[4].byHomeMask);
	m_HomeBitLevel.Format("%04x", axis_mot_data[5].byHomeMask);
	m_HomeBitXFast.Format("%04x", axis_mot_data[6].byHomeMask);
	m_HomeBitY.Format(	  "%04x", axis_mot_data[7].byHomeMask);
	m_HomeIntBitBigX.Format( "%04x", axis_mot_data[0].byHomeIntMask);
	m_HomeIntBitBigY.Format( "%04x", axis_mot_data[1].byHomeIntMask);
	m_HomeIntBitZ.Format(	 "%04x", axis_mot_data[2].byHomeIntMask);
	m_HomeIntBitXSlow.Format("%04x", axis_mot_data[3].byHomeIntMask);
	m_HomeIntBitTheta.Format("%04x", axis_mot_data[4].byHomeIntMask);
	m_HomeIntBitLevel.Format("%04x", axis_mot_data[5].byHomeIntMask);
	m_HomeIntBitXFast.Format("%04x", axis_mot_data[6].byHomeIntMask);
	m_HomeIntBitY.Format(	 "%04x", axis_mot_data[7].byHomeIntMask);
	m_StopBitBigX.Format( "%04x", axis_mot_data[0].byXstopMask);
	m_StopBitBigY.Format( "%04x", axis_mot_data[1].byXstopMask);
	m_StopBitZ.Format(	  "%04x", axis_mot_data[2].byXstopMask);
	m_StopBitXSlow.Format("%04x", axis_mot_data[3].byXstopMask);
	m_StopBitTheta.Format("%04x", axis_mot_data[4].byXstopMask);
	m_StopBitLevel.Format("%04x", axis_mot_data[5].byXstopMask);
	m_StopBitXFast.Format("%04x", axis_mot_data[6].byXstopMask);
	m_StopBitY.Format(	  "%04x", axis_mot_data[7].byXstopMask);
	m_UnusedBigX.Format( "%04x", axis_mot_data[0].byUnused);
	m_UnusedBigY.Format( "%04x", axis_mot_data[1].byUnused);
	m_UnusedZ.Format(	 "%04x", axis_mot_data[2].byUnused);
	m_UnusedXSlow.Format("%04x", axis_mot_data[3].byUnused);
	m_UnusedTheta.Format("%04x", axis_mot_data[4].byUnused);
	m_UnusedLevel.Format("%04x", axis_mot_data[5].byUnused);
	m_UnusedXFast.Format("%04x", axis_mot_data[6].byUnused);
	m_UnusedY.Format(	 "%04x", axis_mot_data[7].byUnused);

	// 更新
	UpdateData(FALSE);
}

// データ取得
void CMotCfg::GetData()
{
	UpdateData(TRUE);

	motion_base_port[0] = (unsigned short) strtoul(m_BaseAddr1, NULL, 16);
	motion_base_port[1] = (unsigned short) strtoul(m_BaseAddr2, NULL, 16);
	alloc_axis = m_AxisNum;
	strcpy(szAxisName[0], m_AxisNameBigX);
	strcpy(szAxisName[1], m_AxisNameBigY);
	strcpy(szAxisName[2], m_AxisNameZ);
	strcpy(szAxisName[3], m_AxisNameXSlow);
	strcpy(szAxisName[4], m_AxisNameTheta);
	strcpy(szAxisName[5], m_AxisNameLevel);
	strcpy(szAxisName[6], m_AxisNameXFast);
	strcpy(szAxisName[7], m_AxisNameY);
	axis_mot_data[0].wHctlPort = (unsigned short) strtoul(m_HCTLPortBigX, NULL, 16);
	axis_mot_data[1].wHctlPort = (unsigned short) strtoul(m_HCTLPortBigY, NULL, 16);
	axis_mot_data[2].wHctlPort = (unsigned short) strtoul(m_HCTLPortZ, NULL, 16);
	axis_mot_data[3].wHctlPort = (unsigned short) strtoul(m_HCTLPortXSlow, NULL, 16);
	axis_mot_data[4].wHctlPort = (unsigned short) strtoul(m_HCTLPortTheta, NULL, 16);
	axis_mot_data[5].wHctlPort = (unsigned short) strtoul(m_HCTLPortLevel, NULL, 16);
	axis_mot_data[6].wHctlPort = (unsigned short) strtoul(m_HCTLPortXFast, NULL, 16);
	axis_mot_data[7].wHctlPort = (unsigned short) strtoul(m_HCTLPortY, NULL, 16);
	axis_mot_data[0].wHomePort = (unsigned short) strtoul(m_HomePortBigX, NULL, 16);
	axis_mot_data[1].wHomePort = (unsigned short) strtoul(m_HomePortBigY, NULL, 16);
	axis_mot_data[2].wHomePort = (unsigned short) strtoul(m_HomePortZ, NULL, 16);
	axis_mot_data[3].wHomePort = (unsigned short) strtoul(m_HomePortXSlow, NULL, 16);
	axis_mot_data[4].wHomePort = (unsigned short) strtoul(m_HomePortTheta, NULL, 16);
	axis_mot_data[5].wHomePort = (unsigned short) strtoul(m_HomePortLevel, NULL, 16);
	axis_mot_data[6].wHomePort = (unsigned short) strtoul(m_HomePortXFast, NULL, 16);
	axis_mot_data[7].wHomePort = (unsigned short) strtoul(m_HomePortY, NULL, 16);
	axis_mot_data[0].wHomeIntPort = (unsigned short) strtoul(m_HomeIntPortBigX, NULL, 16);
	axis_mot_data[1].wHomeIntPort = (unsigned short) strtoul(m_HomeIntPortBigY, NULL, 16);
	axis_mot_data[2].wHomeIntPort = (unsigned short) strtoul(m_HomeIntPortZ, NULL, 16);
	axis_mot_data[3].wHomeIntPort = (unsigned short) strtoul(m_HomeIntPortXSlow, NULL, 16);
	axis_mot_data[4].wHomeIntPort = (unsigned short) strtoul(m_HomeIntPortTheta, NULL, 16);
	axis_mot_data[5].wHomeIntPort = (unsigned short) strtoul(m_HomeIntPortLevel, NULL, 16);
	axis_mot_data[6].wHomeIntPort = (unsigned short) strtoul(m_HomeIntPortXFast, NULL, 16);
	axis_mot_data[7].wHomeIntPort = (unsigned short) strtoul(m_HomeIntPortY, NULL, 16);
	axis_mot_data[0].wXstopPort = (unsigned short) strtoul(m_StopPortBigX, NULL, 16);
	axis_mot_data[1].wXstopPort = (unsigned short) strtoul(m_StopPortBigY, NULL, 16);
	axis_mot_data[2].wXstopPort = (unsigned short) strtoul(m_StopPortZ, NULL, 16);
	axis_mot_data[3].wXstopPort = (unsigned short) strtoul(m_StopPortXSlow, NULL, 16);
	axis_mot_data[4].wXstopPort = (unsigned short) strtoul(m_StopPortTheta, NULL, 16);
	axis_mot_data[5].wXstopPort = (unsigned short) strtoul(m_StopPortLevel, NULL, 16);
	axis_mot_data[6].wXstopPort = (unsigned short) strtoul(m_StopPortXFast, NULL, 16);
	axis_mot_data[7].wXstopPort = (unsigned short) strtoul(m_StopPortY, NULL, 16);
	axis_mot_data[0].byHomeMask = (unsigned char) strtoul(m_HomeBitBigX, NULL, 16);
	axis_mot_data[1].byHomeMask = (unsigned char) strtoul(m_HomeBitBigY, NULL, 16);
	axis_mot_data[2].byHomeMask = (unsigned char) strtoul(m_HomeBitZ, NULL, 16);
	axis_mot_data[3].byHomeMask = (unsigned char) strtoul(m_HomeBitXSlow, NULL, 16);
	axis_mot_data[4].byHomeMask = (unsigned char) strtoul(m_HomeBitTheta, NULL, 16);
	axis_mot_data[5].byHomeMask = (unsigned char) strtoul(m_HomeBitLevel, NULL, 16);
	axis_mot_data[6].byHomeMask = (unsigned char) strtoul(m_HomeBitXFast, NULL, 16);
	axis_mot_data[7].byHomeMask = (unsigned char) strtoul(m_HomeBitY, NULL, 16);
	axis_mot_data[0].byHomeIntMask = (unsigned char) strtoul(m_HomeIntBitBigX, NULL, 16);
	axis_mot_data[1].byHomeIntMask = (unsigned char) strtoul(m_HomeIntBitBigY, NULL, 16);
	axis_mot_data[2].byHomeIntMask = (unsigned char) strtoul(m_HomeIntBitZ, NULL, 16);
	axis_mot_data[3].byHomeIntMask = (unsigned char) strtoul(m_HomeIntBitXSlow, NULL, 16);
	axis_mot_data[4].byHomeIntMask = (unsigned char) strtoul(m_HomeIntBitTheta, NULL, 16);
	axis_mot_data[5].byHomeIntMask = (unsigned char) strtoul(m_HomeIntBitLevel, NULL, 16);
	axis_mot_data[6].byHomeIntMask = (unsigned char) strtoul(m_HomeIntBitXFast, NULL, 16);
	axis_mot_data[7].byHomeIntMask = (unsigned char) strtoul(m_HomeIntBitY, NULL, 16);
	axis_mot_data[0].byXstopMask = (unsigned char) strtoul(m_StopBitBigX, NULL, 16);
	axis_mot_data[1].byXstopMask = (unsigned char) strtoul(m_StopBitBigY, NULL, 16);
	axis_mot_data[2].byXstopMask = (unsigned char) strtoul(m_StopBitZ, NULL, 16);
	axis_mot_data[3].byXstopMask = (unsigned char) strtoul(m_StopBitXSlow, NULL, 16);
	axis_mot_data[4].byXstopMask = (unsigned char) strtoul(m_StopBitTheta, NULL, 16);
	axis_mot_data[5].byXstopMask = (unsigned char) strtoul(m_StopBitLevel, NULL, 16);
	axis_mot_data[6].byXstopMask = (unsigned char) strtoul(m_StopBitXFast, NULL, 16);
	axis_mot_data[7].byXstopMask = (unsigned char) strtoul(m_StopBitY, NULL, 16);
	axis_mot_data[0].byUnused = (unsigned char) strtoul(m_UnusedBigX, NULL, 16);
	axis_mot_data[1].byUnused = (unsigned char) strtoul(m_UnusedBigY, NULL, 16);
	axis_mot_data[2].byUnused = (unsigned char) strtoul(m_UnusedZ, NULL, 16);
	axis_mot_data[3].byUnused = (unsigned char) strtoul(m_UnusedXSlow, NULL, 16);
	axis_mot_data[4].byUnused = (unsigned char) strtoul(m_UnusedTheta, NULL, 16);
	axis_mot_data[5].byUnused = (unsigned char) strtoul(m_UnusedLevel, NULL, 16);
	axis_mot_data[6].byUnused = (unsigned char) strtoul(m_UnusedXFast, NULL, 16);
	axis_mot_data[7].byUnused = (unsigned char) strtoul(m_UnusedY, NULL, 16);
}

BOOL CMotCfg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	// 最初にロードしておく
	OnBtnLoad();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
