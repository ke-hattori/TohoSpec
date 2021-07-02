// StressCofigurationSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "../../INC/NSStage.hxx"
#include "StressCofigurationSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStressCofigurationSettingDlg ダイアログ


CStressCofigurationSettingDlg::CStressCofigurationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStressCofigurationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStressCofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CStressCofigurationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressCofigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CStressCofigurationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CStressCofigurationSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStressCofigurationSettingDlg メッセージ ハンドラ

void CStressCofigurationSettingDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください
	if (!CheckData())
		return;

	UpDate(TRUE);

	ConfigFile_SetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_STRESS_INI_FILE);

	CDialog::OnOK();
}

BOOL CStressCofigurationSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	memset(&m_StressConfig, 0, sizeof(m_StressConfig));
	ConfigFile_GetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Set Data /////
	UpDate(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CStressCofigurationSettingDlg::CheckData()
{
// K.Matsuo 2009.09.07 -->
	STAGE_CONFIG	StageConfig;
//	double	dStageSizeXMax, dStageSizeXMin, dStageSizeYMax, dStageSizeYMin;

	CString	strBuffer;

	UpdateData(TRUE);

	///// Load Stage Config File /////
	ConfigFile_GetNanoSpecIni(&StageConfig, CONFIG_FILE_STAGE_CONFIG);
//	dStageSizeXMax = StageConfig.Size.dX / 2;
//	dStageSizeXMin = -(dStageSizeXMax);
//	dStageSizeYMax = StageConfig.Size.dY / 2;
//	dStageSizeYMin = -(dStageSizeYMax);

	STAGE_COORD coordLeftTop;
	STAGE_COORD coordRightBottom;

	coordLeftTop.lX = StageConfig.StageMaxCoord.dLeftX;
	coordLeftTop.lY = StageConfig.StageMaxCoord.dUpY;
	coordRightBottom.lX = StageConfig.StageMaxCoord.dRightX;
	coordRightBottom.lY = StageConfig.StageMaxCoord.dDownY;

	NS_ConvertToStageGetPosCoord(&coordLeftTop);
	NS_ConvertToStageGetPosCoord(&coordRightBottom);
// K.Matsuo 2009.09.07 <--

	///// Sampling Interval Time /////
	GetDlgItemText(IDC_SAMPLING_INTERVAL_TIME,strBuffer);
	if(CheckMinMaxLong(strBuffer, 0, 100000) != CHECK_DATA_OK){
		GetDlgItem(IDC_SAMPLING_INTERVAL_TIME)->SetFocus();
		return FALSE;
	}

	///// Data Integration Interval /////
	GetDlgItemText(IDC_DATA_INTEGRATION_INTERVAL,strBuffer);
	if(CheckMinMaxLong(strBuffer, 0, 1000) != CHECK_DATA_OK){
		GetDlgItem(IDC_DATA_INTEGRATION_INTERVAL)->SetFocus();
		return FALSE;
	}

	///// Scan Start Wait Time /////
	GetDlgItemText(IDC_SCAN_START_WAIT_TIME,strBuffer);
	if(CheckMinMaxLong(strBuffer, 0, 1000) != CHECK_DATA_OK){
		GetDlgItem(IDC_SCAN_START_WAIT_TIME)->SetFocus();
		return FALSE;
	}

	///// Scan Speed /////
	GetDlgItemText(IDC_SCAN_SPEED,strBuffer);
	if(CheckMinMaxLong(strBuffer, 0, 100000) != CHECK_DATA_OK){
		GetDlgItem(IDC_SCAN_SPEED)->SetFocus();
		return FALSE;
	}

// K.Matsuo 2009.09.07 -->
	///// Scan Start Position /////
	GetDlgItemText(IDC_SCAN_START_POSITION,strBuffer);
//	if(CheckMinMaxDouble(strBuffer, dStageSizeXMin, dStageSizeXMax) != CHECK_DATA_OK){
	if(CheckMinMaxDouble(strBuffer, min(coordLeftTop.lX, coordRightBottom.lX), max(coordLeftTop.lX, coordRightBottom.lX)) != CHECK_DATA_OK){
		GetDlgItem(IDC_SCAN_START_POSITION)->SetFocus();
		return FALSE;
	}

	///// Scan End Position /////
	GetDlgItemText(IDC_SCAN_END_POSITION,strBuffer);
//	if(CheckMinMaxDouble(strBuffer, dStageSizeXMin, dStageSizeXMax) != CHECK_DATA_OK){
	if(CheckMinMaxDouble(strBuffer, min(coordLeftTop.lX, coordRightBottom.lX), max(coordLeftTop.lX, coordRightBottom.lX)) != CHECK_DATA_OK){
		GetDlgItem(IDC_SCAN_END_POSITION)->SetFocus();
		return FALSE;
	}

//	///// Line Move Start Position /////
//	GetDlgItemText(IDC_LINE_MOVE_START_POSITION,strBuffer);
////	if(CheckMinMaxDouble(strBuffer, dStageSizeXMin, dStageSizeXMax) != CHECK_DATA_OK){
//	if(CheckMinMaxDouble(strBuffer, min(coordLeftTop.lX, coordRightBottom.lX), max(coordLeftTop.lX, coordRightBottom.lX)) != CHECK_DATA_OK){
//		GetDlgItem(IDC_LINE_MOVE_START_POSITION)->SetFocus();
//		return FALSE;
//	}
//
//	///// Line Move End Position /////
//	GetDlgItemText(IDC_LINE_MOVE_END_POSITION,strBuffer);
////	if(CheckMinMaxDouble(strBuffer, dStageSizeXMin, dStageSizeXMax) != CHECK_DATA_OK){
//	if(CheckMinMaxDouble(strBuffer, min(coordLeftTop.lX, coordRightBottom.lX), max(coordLeftTop.lX, coordRightBottom.lX)) != CHECK_DATA_OK){
//		GetDlgItem(IDC_LINE_MOVE_END_POSITION)->SetFocus();
//		return FALSE;
//	}
// K.Matsuo 2009.09.07 <--

// 2009.10.01 bagus Stress --{--
	///// Scan Z Axis Position /////
	GetDlgItemText(IDC_SCAN_Z_AXIS_POSITION,strBuffer);
	if(CheckMinMaxLong(strBuffer, 10000, 50000) != CHECK_DATA_OK){
		GetDlgItem(IDC_SCAN_Z_AXIS_POSITION)->SetFocus();
		return FALSE;
	}
// 2009.10.01 bagus Stress --}--

	return TRUE;
}

void CStressCofigurationSettingDlg::UpDate(BOOL bValid)
{
	CString strBuffer;

	if(bValid){
		//構造体へデータをセットする
		GetDlgItemText(IDC_SAMPLING_INTERVAL_TIME,strBuffer);
		m_StressConfig.dwSamplingInterval = atol(strBuffer);
		GetDlgItemText(IDC_DATA_INTEGRATION_INTERVAL,strBuffer);
		m_StressConfig.dwDataIntegrationInterval = atol(strBuffer);
		GetDlgItemText(IDC_SCAN_START_WAIT_TIME,strBuffer);
		m_StressConfig.dwScanStartWaitTime = atol(strBuffer);
		GetDlgItemText(IDC_SCAN_SPEED,strBuffer);
		m_StressConfig.dwScanSpeed = atol(strBuffer);
// K.Matsuo 2009.09.07 -->
		GetDlgItemText(IDC_SCAN_START_POSITION,strBuffer);
		m_StressConfig.lScanStartPosX = atol(strBuffer);
		GetDlgItemText(IDC_SCAN_END_POSITION,strBuffer);
		m_StressConfig.lScanEndPosX = atol(strBuffer);
// K.Matsuo 2009.09.07 <--
		GetDlgItemText(IDC_LINE_MOVE_START_POSITION,strBuffer);
		m_StressConfig.lLineStartPosX = atol(strBuffer);
		GetDlgItemText(IDC_LINE_MOVE_END_POSITION,strBuffer);
		m_StressConfig.lLineEndPosX = atol(strBuffer);
// 2009.10.01 bagus Stress --{--
		GetDlgItemText(IDC_SCAN_Z_AXIS_POSITION,strBuffer);
		m_StressConfig.lScanZAxisPosition = atol(strBuffer) * 10;
// 2009.10.01 bagus Stress --}--
	}else{
		//構造体にあるデータを画面にセットする
		//// Sampling Interval Time(um)
		strBuffer.Format(_T("%ld"),m_StressConfig.dwSamplingInterval);
		SetDlgItemText(IDC_SAMPLING_INTERVAL_TIME,strBuffer);
		//// Data Integration Interval (times)
		strBuffer.Format(_T("%ld"),m_StressConfig.dwDataIntegrationInterval);
		SetDlgItemText(IDC_DATA_INTEGRATION_INTERVAL,strBuffer);
		//// Scan Start Wait Time(sec)
		strBuffer.Format(_T("%ld"),m_StressConfig.dwScanStartWaitTime);
		SetDlgItemText(IDC_SCAN_START_WAIT_TIME,strBuffer);
		//// Scan Speed(mm/sec)
		strBuffer.Format(_T("%ld"),m_StressConfig.dwScanSpeed);
		SetDlgItemText(IDC_SCAN_SPEED,strBuffer);
// K.Matsuo 2009.09.07 -->
		//// Scan Start Position (um)
		strBuffer.Format(_T("%ld"),m_StressConfig.lScanStartPosX);
		SetDlgItemText(IDC_SCAN_START_POSITION,strBuffer);
		//// Scan End Position (um)
		strBuffer.Format(_T("%ld"),m_StressConfig.lScanEndPosX);
		SetDlgItemText(IDC_SCAN_END_POSITION,strBuffer);
// K.Matsuo 2009.09.07 <--
		//// Line Move Start Position (um)
		strBuffer.Format(_T("%ld"),m_StressConfig.lLineStartPosX);
		SetDlgItemText(IDC_LINE_MOVE_START_POSITION,strBuffer);
		//// Line Move End Position (um)
		strBuffer.Format(_T("%ld"),m_StressConfig.lLineEndPosX);
		SetDlgItemText(IDC_LINE_MOVE_END_POSITION,strBuffer);
// 2009.10.01 bagus Stress --{--
		///// Scan Z Axis Position /////
		strBuffer.Format(_T("%ld"), m_StressConfig.lScanZAxisPosition / 10);
		SetDlgItemText(IDC_SCAN_Z_AXIS_POSITION, strBuffer);
// 2009.10.01 bagus Stress --}--
	}
}
