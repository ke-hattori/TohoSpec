// JogDirSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "JogDirSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)

static UINT StageCoordDir_ID[] = {
	IDC_DIR_1,		// 右
	IDC_DIR_2,		// 上
	IDC_DIR_3,		// 左
	IDC_DIR_4,		// 下
};

static LPCTSTR StageCoordDir_Item[] = {
	"  ",
	"+X",
	"+Y",
};

static int STAGE_COORD_DIR_COMBO_MAX = sizeof(StageCoordDir_ID) / sizeof(StageCoordDir_ID[0]);
static int STAGE_COORD_DIR_ITEM_MAX = sizeof(StageCoordDir_Item) / sizeof(StageCoordDir_Item[0]);

/////////////////////////////////////////////////////////////////////////////
// CJogDirSettingDlg ダイアログ


CJogDirSettingDlg::CJogDirSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJogDirSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJogDirSettingDlg)
	//}}AFX_DATA_INIT
	m_bXySwap = FALSE;
	m_bChangeXSign = FALSE;
	m_bChangeYSign = FALSE;
}


void CJogDirSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJogDirSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CJogDirSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CJogDirSettingDlg)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_DIR_1, IDC_DIR_4, OnStageCoordDirComboBox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJogDirSettingDlg メッセージ ハンドラ

BOOL CJogDirSettingDlg::OnInitDialog() 
{
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Get Data /////
	ConfigFile_GetNanoSpecIni(&m_JogDirectionSetting, CONFIG_FILE_JOG_DIR);

	m_bXySwap = m_JogDirectionSetting.bSwapXyDir;
	m_bChangeXSign = m_JogDirectionSetting.bChangeXSign;
	m_bChangeYSign = m_JogDirectionSetting.bChangeYSign;

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	InitCombo_StageCoord();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogDirSettingDlg::OnOK() 
{
	if ( !UpdateData() )
		return;

	if ( !CheckData() )
		return;

	SetOperationLog("OK Button was push.");

	MakeSaveData();

	m_JogDirectionSetting.bSwapXyDir = m_bXySwap;
	m_JogDirectionSetting.bChangeXSign = m_bChangeXSign;
	m_JogDirectionSetting.bChangeYSign = m_bChangeYSign;

	ConfigFile_SetNanoSpecIni(&m_JogDirectionSetting, CONFIG_FILE_JOG_DIR);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CJogDirSettingDlg::CheckData()
{
	CString strBuffer;
	BOOL bDirectX = FALSE;
	BOOL bDirectY = FALSE;

	// Kojika 20090603 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090603 Add End

	///// StageCoordinate Direction /////
	for ( int i = 0; i < STAGE_COORD_DIR_COMBO_MAX; i++ ) {
		GetDlgItemText(StageCoordDir_ID[i], strBuffer);
		///// +Xが２つ以上ないか確認 /////
		if ( strBuffer == StageCoordDir_Item[1] ) {
			if ( !bDirectX ) {
				bDirectX = TRUE;
			}
			else {
				// Kojika 20090603 Change
				//MessageBox("Two or More +X Exists.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_TWO_MORE_X_EXIST, l_strBuffer, "Two or More +X Exists.");
				LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
				MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
				// Kojika 20090603 Change End
				return FALSE;
			}
		}

		///// +Yが２つ以上ないか確認 /////
		if ( strBuffer == StageCoordDir_Item[2] ) {
			if ( !bDirectY ) {
				bDirectY = TRUE;
			}
			else {
				// Kojika 20090603 Change
				//MessageBox("Two or More +Y Exists.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_TWO_MORE_Y_EXIST, l_strBuffer, "Two or More +Y Exists.");
				LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
				MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
				// Kojika 20090603 Change End
				return FALSE;
			}
		}
	}

	///// +X +Y が存在しない /////
	if ( !bDirectX && !bDirectY ) {
		// Kojika 20090603 Change
		//MessageBox("Please Setup +X and +Y", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_SETUP_X_AND_Y, l_strBuffer, "Please Setup +X and +Y");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// +X +Y が対でない /////
	if ( !bDirectX ) {
		// Kojika 20090603 Change
		//MessageBox("Please Setup +X.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_SETUP_X, l_strBuffer, "Please Setup +X.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		return FALSE;
	}
	if ( !bDirectY ) {
		// Kojika 20090603 Change
		//MessageBox("Please Setup +Y.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_SETUP_Y, l_strBuffer, "Please Setup +Y.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// X,Yの方向が一直線でないこと /////
	if ( (m_SelDirX + m_SelDirY) % 2 == 0 ) {
		// Kojika 20090603 Change
		//MessageBox("Please Correct.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_CORRECT, l_strBuffer, "Please Correct.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogDirSettingDlg::OnCancel() 
{
	SetOperationLog("CANCEL Button was push.");

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//
void CJogDirSettingDlg::SetOperationLog(LPCTSTR pszLog)
{
	char strmsg[1024];

	// 操作ログを取得
	memset( strmsg, 0, sizeof(strmsg));
	strcpy( strmsg, "[");
	::GetWindowText(this->m_hWnd, &strmsg[1], 1000);
	_tcscat( strmsg, "] ");
	_tcscat( strmsg, pszLog);

	m_pDoc->OperationLogging(strmsg);
}

// =========================================================================
//
void CJogDirSettingDlg::InitCombo_StageCoord()
{
	CComboBox* pCombo;
	int nSelect;
	int DirX;
	int DirY;
	int iTemp;

	for ( int i = 0; i < STAGE_COORD_DIR_COMBO_MAX; i++ ) {
		pCombo = (CComboBox*)GetDlgItem(StageCoordDir_ID[i]);
		pCombo->ResetContent();
		for ( int j = 0; j < STAGE_COORD_DIR_ITEM_MAX; j++ ) {
			pCombo->AddString(StageCoordDir_Item[j]);
		}
	}

	DirX = DIR_LEFT;
	DirY = DIR_TOP;
	if ( m_bXySwap ) {
		iTemp = DirX;
		DirX = DirY;
		DirY = iTemp;
	}

	if ( m_bChangeXSign ) {
		switch ( DirX ) {
		case DIR_RIGHT:
			DirX = DIR_LEFT;
			break;
		case DIR_TOP:
			DirX = DIR_BOTTOM;
			break;
		case DIR_LEFT:
			DirX = DIR_RIGHT;
			break;
		case DIR_BOTTOM:
			DirX = DIR_TOP;
			break;
		default:
			break;
		}
	}

	if ( m_bChangeYSign ) {
		switch ( DirY ) {
		case DIR_RIGHT:
			DirY = DIR_LEFT;
			break;
		case DIR_TOP:
			DirY = DIR_BOTTOM;
			break;
		case DIR_LEFT:
			DirY = DIR_RIGHT;
			break;
		case DIR_BOTTOM:
			DirY = DIR_TOP;
			break;
		default:
			break;
		}
	}

	pCombo = (CComboBox*)GetDlgItem(StageCoordDir_ID[DirX - 1]);
	nSelect = pCombo->FindStringExact(-1, StageCoordDir_Item[1]);
	pCombo->SetCurSel(nSelect);

	pCombo = (CComboBox*)GetDlgItem(StageCoordDir_ID[DirY - 1]);
	nSelect = pCombo->FindStringExact(-1, StageCoordDir_Item[2]);
	pCombo->SetCurSel(nSelect);
}

// =========================================================================
//
void CJogDirSettingDlg::MakeSaveData()
{
	int OverlayJogDirX;
	int OverlayJogDirY;

	// init value.
	m_bXySwap = FALSE;
	m_bChangeXSign = FALSE;
	m_bChangeYSign = FALSE;

	OverlayJogDirX = DIR_LEFT;
	OverlayJogDirY = DIR_TOP;

	// check xy swap.
	if ( OverlayJogDirX == DIR_LEFT ||
		 OverlayJogDirX == DIR_RIGHT ) {
		if ( m_SelDirX == DIR_TOP ||
			 m_SelDirX == DIR_BOTTOM ) {
			m_bXySwap = TRUE;
		}
	}
	else {
		if ( m_SelDirX == DIR_LEFT ||
			 m_SelDirX == DIR_RIGHT ) {
			m_bXySwap = TRUE;
		}
	}

	// check X Sign
	if ( m_bXySwap == FALSE ) {
		if ( m_SelDirX != OverlayJogDirX ) {
			m_bChangeXSign = TRUE;
		}
	}
	else {
		if ( m_SelDirX != OverlayJogDirY ) {
			m_bChangeXSign = TRUE;
		}
	}

	// check Y Sign
	if ( m_bXySwap == FALSE ) {
		if ( m_SelDirY != OverlayJogDirY ) {
			m_bChangeYSign = TRUE;
		}
	}
	else {
		if ( m_SelDirY != OverlayJogDirX ) {
			m_bChangeYSign = TRUE;
		}
	}
}

// =========================================================================
// OnStageCoordDirComboBox ステージ座標の方向設定
void CJogDirSettingDlg::OnStageCoordDirComboBox(UINT nID)
{
	CString strBuffer;

	for ( int i = 0; i < STAGE_COORD_DIR_COMBO_MAX; i++ ) {
		GetDlgItemText(StageCoordDir_ID[i], strBuffer);
		if ( strBuffer == StageCoordDir_Item[1] )
			m_SelDirX = StageCoordDir_ID[i] - IDC_DIR_1 + 1;
		if ( strBuffer == StageCoordDir_Item[2] )
			m_SelDirY = StageCoordDir_ID[i] - IDC_DIR_1 + 1;
	}
}
