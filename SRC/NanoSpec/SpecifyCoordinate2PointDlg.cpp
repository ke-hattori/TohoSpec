// SpecifyCoordinate2PointDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "SpecifyCoordinate2PointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecifyCoordinate2PointDlg ダイアログ


CSpecifyCoordinate2PointDlg::CSpecifyCoordinate2PointDlg(int iTypeDlg, double dSampleSizeX, double dSampleSizeY, CWnd* pParent /*=NULL*/)
	: CDialog(CSpecifyCoordinate2PointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecifyCoordinate2PointDlg)
	//}}AFX_DATA_INIT
	m_iType = iTypeDlg;
	m_dSampleSizeX = dSampleSizeX;
	m_dSampleSizeY = dSampleSizeY;
	m_lX1 = m_lY1 = 0;
	m_lX2 = m_lY2 = 0;
}


void CSpecifyCoordinate2PointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecifyCoordinate2PointDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CSpecifyCoordinate2PointDlg, CDialog)
	//{{AFX_MSG_MAP(CSpecifyCoordinate2PointDlg)
	ON_CBN_SELCHANGE(IDC_COORDINATE, OnSelchangeCoordinate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecifyCoordinate2PointDlg メッセージ ハンドラ

BOOL CSpecifyCoordinate2PointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pCombo;
	int i;
	CString strBuffer;

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

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
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSpecifyCoordinate_ENU[0]);
		}else{
			pCombo->AddString(g_lpszSpecifyCoordinate_JPN[0]);
		}
	}
	else{
		for(i = 0; i < SPECIFY_COORDINATE_MAX; i++){
			if(l_SystemConfig.nLanguage == 0){
				pCombo->AddString(g_lpszSpecifyCoordinate_ENU[i]);
			}else{
				pCombo->AddString(g_lpszSpecifyCoordinate_JPN[i]);
			}
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
		GetDlgItem(IDC_COORDINATE)->EnableWindow(FALSE);
		LoadStringML(IDS_ADD_POINT, strBuffer, "ADD POINT");
		SetWindowText(strBuffer);
		break;
	}

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSpecifyCoordinate2PointDlg::OnOK()
{
	CString csBuff;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COORDINATE);
	m_iType = pCombo->GetCurSel();

	if(!CheckData()){
		return;
	}
	SetOperationLog("OK Button was push.");

	GetDlgItemText(IDC_COORDINATE_X1, csBuff);
	m_lX1 = atof(csBuff) * MICROMETRE;
	GetDlgItemText(IDC_COORDINATE_Y1, csBuff);
	m_lY1 = atof(csBuff) * MICROMETRE;

	GetDlgItemText(IDC_COORDINATE_X2, csBuff);
	m_lX2 = atof(csBuff) * MICROMETRE;
	GetDlgItemText(IDC_COORDINATE_Y2, csBuff);
	m_lY2 = atof(csBuff) * MICROMETRE;

	m_iMoveMode = m_iType;
	CDialog::OnOK();
}

// =========================================================================
//
void CSpecifyCoordinate2PointDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。
	SetOperationLog("CANCEL Button was push.");

	CDialog::OnCancel();
}

// =========================================================================
//
BOOL CSpecifyCoordinate2PointDlg::CheckData()
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
			coordLeftTop.lX = m_dSampleSizeX / 2 * -1;
			coordLeftTop.lY = m_dSampleSizeY / 2;
			coordRightBottom.lX = m_dSampleSizeX / 2;
			coordRightBottom.lY = m_dSampleSizeY / 2 * -1;
			break;
		case 2: // 左上
			coordLeftTop.lX = 0;
			coordLeftTop.lY = 0;
			coordRightBottom.lX = m_dSampleSizeX;
			coordRightBottom.lY = m_dSampleSizeY * -1;
			break;
		case 3: // 左下
			coordLeftTop.lX = 0;
			coordLeftTop.lY = m_dSampleSizeY;
			coordRightBottom.lX = m_dSampleSizeX;
			coordRightBottom.lY = 0;
			break;
		case 4: // 右下
			coordLeftTop.lX = m_dSampleSizeX * -1;
			coordLeftTop.lY = m_dSampleSizeY;
			coordRightBottom.lX = 0;
			coordRightBottom.lY = 0;
			break;
		case 5: // 右上
			coordLeftTop.lX = m_dSampleSizeX * -1;
			coordLeftTop.lY = 0;
			coordRightBottom.lX = 0;
			coordRightBottom.lY = m_dSampleSizeY * -1;
			break;
		}

		// ステージコンフィグの軸設定と合わせる
		NS_ConvertToStageConfigXyAxis(&coordLeftTop);
		NS_ConvertToStageConfigXyAxis(&coordRightBottom);

		break;
	///// 指定座標が、ステージ稼動範囲であるか
	case SPECIFY_COORDINATE_STAGE:
	default:
		coordLeftTop.lX = m_StageConfig.StageMaxCoord.dLeftX;
		coordLeftTop.lY = m_StageConfig.StageMaxCoord.dUpY;
		coordRightBottom.lX = m_StageConfig.StageMaxCoord.dRightX;
		coordRightBottom.lY = m_StageConfig.StageMaxCoord.dDownY;

		NS_ConvertToStageGetPosCoord(&coordLeftTop);
		NS_ConvertToStageGetPosCoord(&coordRightBottom);

		break;
	}

	///// X /////
	GetDlgItemText(IDC_COORDINATE_X1, csBuff);
	//センター原点
	if(CheckMinMaxDouble(csBuff, min(coordLeftTop.lX, coordRightBottom.lX) / MICROMETRE, max(coordLeftTop.lX, coordRightBottom.lX) / MICROMETRE) != CHECK_DATA_OK){
		return FALSE;
	}
	lPointX = atof(csBuff) * MICROMETRE;

	///// Y /////
	GetDlgItemText(IDC_COORDINATE_Y1, csBuff);
	if(CheckMinMaxDouble(csBuff, min(coordLeftTop.lY, coordRightBottom.lY) / MICROMETRE, max(coordLeftTop.lY, coordRightBottom.lY) / MICROMETRE) != CHECK_DATA_OK){
		return FALSE;
	}
	lPointY = atof(csBuff) * MICROMETRE;

	///// X /////
	GetDlgItemText(IDC_COORDINATE_X2, csBuff);
	//センター原点
	if(CheckMinMaxDouble(csBuff, min(coordLeftTop.lX, coordRightBottom.lX) / MICROMETRE, max(coordLeftTop.lX, coordRightBottom.lX) / MICROMETRE) != CHECK_DATA_OK){
		return FALSE;
	}
	lPointX = atof(csBuff) * MICROMETRE;

	///// Y /////
	GetDlgItemText(IDC_COORDINATE_Y2, csBuff);
	if(CheckMinMaxDouble(csBuff, min(coordLeftTop.lY, coordRightBottom.lY) / MICROMETRE, max(coordLeftTop.lY, coordRightBottom.lY) / MICROMETRE) != CHECK_DATA_OK){
		return FALSE;
	}
	lPointY = atof(csBuff) * MICROMETRE;

	return TRUE;
}

void CSpecifyCoordinate2PointDlg::OnSelchangeCoordinate()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COORDINATE);

	m_iType = pCombo->GetCurSel();
}

// =========================================================================
//
void CSpecifyCoordinate2PointDlg::SetOperationLog(LPCTSTR pszLog)
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
