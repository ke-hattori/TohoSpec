// SpecifyCoordinateDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "SpecifyCoordinateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSpecifyCoordinateDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSpecifyCoordinateDlg::CSpecifyCoordinateDlg(int iTypeDlg, double dSampleSizeX, double dSampleSizeY, CWnd* pParent /*=NULL*/)
	: CDialog(CSpecifyCoordinateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecifyCoordinateDlg)
	//}}AFX_DATA_INIT
	m_iType = iTypeDlg;
	m_dSampleSizeX = dSampleSizeX;
	m_dSampleSizeY = dSampleSizeY;
//	m_nSampleMode = 0;
// Goto add test --->
	m_lX = m_lY = 0;
// Goto add test <---
}

// =========================================================================
//
void CSpecifyCoordinateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecifyCoordinateDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSpecifyCoordinateDlg, CDialog)
	//{{AFX_MSG_MAP(CSpecifyCoordinateDlg)
	ON_CBN_SELCHANGE(IDC_COORDINATE, OnSelchangeCoordinate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSpecifyCoordinateDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSpecifyCoordinateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pCombo;
	int i;
	//Saiki 20090601 Add ----->
	CString strBuffer;
	//Saiki 20090601 Add <-----

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Load Stage Config File /////
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);

	pCombo = (CComboBox *)GetDlgItem(IDC_COORDINATE);
	pCombo->ResetContent();
	if(m_nSampleMode == 0){
		// Kojika 20090528 Change
		//pCombo->AddString(g_lpszSpecifyCoordinate[0]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSpecifyCoordinate_ENU[0]);
		}else{
			pCombo->AddString(g_lpszSpecifyCoordinate_JPN[0]);
		}
		// Kojika 20090528 Change End
	}
	else{
		for(i = 0; i < SPECIFY_COORDINATE_MAX; i++){
		// Kojika 20090528 Change
		//pCombo->AddString(g_lpszSpecifyCoordinate[i]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSpecifyCoordinate_ENU[i]);
		}else{
			pCombo->AddString(g_lpszSpecifyCoordinate_JPN[i]);
		}
		// Kojika 20090528 Change End
		}
	}
	pCombo->SetCurSel(0);

	switch(m_iType){
	case DEFAULT_WND:
	default:
		pCombo->SetCurSel(0);
		break;
	case RECIPE_STAGE_WND:
		pCombo->SetCurSel(1);
		//GetDlgItem(IDC_COORDINATE_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COORDINATE)->EnableWindow(FALSE);
		//GetDlgItem(IDC_COORDINATE_NAME)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_COORDINATE)->ShowWindow(SW_HIDE);
		//SetWindowText("ADD POINT");
		//Saiki 20090601 Change ----->
		//SetWindowText("ポイント追加");
		LoadStringML(IDS_ADD_POINT, strBuffer, "ADD POINT");
		SetWindowText(strBuffer);
		//Saiki 20090601 Change <-----
		break;
	}

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSpecifyCoordinateDlg::OnOK()
{
	CString csBuff;

//OkabeAdd20090612--->
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COORDINATE);
	m_iType = pCombo->GetCurSel();
//OkabeAdd20090612<---

	if(!CheckData()){
			return;
		}
	SetOperationLog("OK Button was push.");

	GetDlgItemText(IDC_COORDINATE_X, csBuff);
	m_lX = static_cast<long>(atof(csBuff) * MICROMETRE);
	GetDlgItemText(IDC_COORDINATE_Y, csBuff);
	m_lY = static_cast<long>(atof(csBuff) * MICROMETRE);
	m_iMoveMode = m_iType;
	CDialog::OnOK();
}

// =========================================================================
//
void CSpecifyCoordinateDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。
	SetOperationLog("CANCEL Button was push.");

	CDialog::OnCancel();
}

// =========================================================================
//
BOOL CSpecifyCoordinateDlg::CheckData()
{
	CString csBuff;
	long lPointX, lPointY;
	STAGE_COORD coordLeftTop;
	STAGE_COORD coordRightBottom;

	switch(m_iType){
	///// 指定座標が、サンプル範囲であるか
	case SPECIFY_COORDINATE_SAMPLE:
	case RECIPE_STAGE_WND:

		switch ( m_StageConfig.OrgPos ) {
		case 1: // 中心
			coordLeftTop.lX = static_cast<long>(m_dSampleSizeX / 2 * -1);
			coordLeftTop.lY = static_cast<long>(m_dSampleSizeY / 2);
			coordRightBottom.lX = static_cast<long>(m_dSampleSizeX / 2);
			coordRightBottom.lY = static_cast<long>(m_dSampleSizeY / 2 * -1);
			break;
		case 2: // 左上
			coordLeftTop.lX = 0;
			coordLeftTop.lY = 0;
			coordRightBottom.lX = static_cast<long>(m_dSampleSizeX);
			coordRightBottom.lY = static_cast<long>(m_dSampleSizeY * -1);
			break;
		case 3: // 左下
			coordLeftTop.lX = 0;
			coordLeftTop.lY = static_cast<long>(m_dSampleSizeY);
			coordRightBottom.lX = static_cast<long>(m_dSampleSizeX);
			coordRightBottom.lY = 0;
			break;
		case 4: // 右下
			coordLeftTop.lX = static_cast<long>(m_dSampleSizeX * -1);
			coordLeftTop.lY = static_cast<long>(m_dSampleSizeY);
			coordRightBottom.lX = 0;
			coordRightBottom.lY = 0;
			break;
		case 5: // 右上
			coordLeftTop.lX = static_cast<long>(m_dSampleSizeX * -1);
			coordLeftTop.lY = 0;
			coordRightBottom.lX = 0;
			coordRightBottom.lY = static_cast<long>(m_dSampleSizeY * -1);
			break;
		}

		// ステージコンフィグの軸設定と合わせる
		NS_ConvertToStageConfigXyAxis(&coordLeftTop);
		NS_ConvertToStageConfigXyAxis(&coordRightBottom);

		break;
	///// 指定座標が、ステージ稼動範囲であるか
	case SPECIFY_COORDINATE_STAGE:
	default:
		coordLeftTop.lX = static_cast<long>(m_StageConfig.StageMaxCoord.dLeftX);
		coordLeftTop.lY = static_cast<long>(m_StageConfig.StageMaxCoord.dUpY);
		coordRightBottom.lX = static_cast<long>(m_StageConfig.StageMaxCoord.dRightX);
		coordRightBottom.lY = static_cast<long>(m_StageConfig.StageMaxCoord.dDownY);

		NS_ConvertToStageGetPosCoord(&coordLeftTop);
		NS_ConvertToStageGetPosCoord(&coordRightBottom);

		break;
	}

	///// X /////
	GetDlgItemText(IDC_COORDINATE_X, csBuff);
	//センター原点
	if(CheckMinMaxDouble(csBuff, min(coordLeftTop.lX, coordRightBottom.lX) / MICROMETRE, max(coordLeftTop.lX, coordRightBottom.lX) / MICROMETRE) != CHECK_DATA_OK){
		return FALSE;
	}
	lPointX = static_cast<long>(atof(csBuff) * MICROMETRE);

	///// Y /////
	GetDlgItemText(IDC_COORDINATE_Y, csBuff);
	if(CheckMinMaxDouble(csBuff, min(coordLeftTop.lY, coordRightBottom.lY) / MICROMETRE, max(coordLeftTop.lY, coordRightBottom.lY) / MICROMETRE) != CHECK_DATA_OK){
		return FALSE;
	}
	lPointY = static_cast<long>(atof(csBuff) * MICROMETRE);

	return TRUE;
}

void CSpecifyCoordinateDlg::OnSelchangeCoordinate()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COORDINATE);

	m_iType = pCombo->GetCurSel();
}

// =========================================================================
//
void CSpecifyCoordinateDlg::SetOperationLog(LPCTSTR pszLog)
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
