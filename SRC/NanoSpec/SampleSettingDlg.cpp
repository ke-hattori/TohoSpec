// SampleSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SaveAsDlg.h"
#include "OverlayDialog.h"
#include "PointSetupTeachingDlg.h"
#include "../../INC/NSStage.hxx"
#include "SampleSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Define                                                                  //
//                                                                         //
// ==========================================================================
//#define SAMPLESETTING_CAPTION		"SAMPLE SETTING"
//#define SAMPLESETTING_MESSAGE		"SAMPLE SETTING"
//#define SAMPLESETTING_CAPTION       "サンプル設定"
//#define SAMPLESETTING_MESSAGE       "サンプル設定"

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

// #########################################################################
// CSampleSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSampleSettingDlg::CSampleSettingDlg(BOOL bNew, LPCTSTR lpszName, CWnd* pParent /*=NULL*/)
	: CDialog(CSampleSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSampleSettingDlg)
	//}}AFX_DATA_INIT

	m_bNew = bNew;
	strcpy(m_szName, lpszName);
}

// =========================================================================
//
void CSampleSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_SAMPLE_NAME, m_SampleInfo.szName, RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_SAMPLE_THICKNESS, m_SampleInfo.dThickness);
	DDX_Text(pDX, IDC_SAMPLE_SIZE_X, m_SampleInfo.Size.dx);
	DDX_Text(pDX, IDC_SAMPLE_SIZE_Y, m_SampleInfo.Size.dy);
	DDX_Text(pDX, IDC_SAMPLE_ORIGIN_X, m_SampleInfo.Origin.lX);
	DDX_Text(pDX, IDC_SAMPLE_ORIGIN_Y, m_SampleInfo.Origin.lY);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_TEACHING_BUTTON, m_TeachingButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSampleSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSampleSettingDlg)
	ON_BN_CLICKED(IDC_TEACHING_BUTTON, OnTeachingButton)
	ON_CBN_SELCHANGE(IDC_SAMPLE_SHAPE, OnSelchangeSampleShape)
// 2009.11.09 bagus Stress 追加変更 --{--
//	ON_CBN_SELCHANGE(IDC_MATERIAL_ELASTIC_MODULUS, OnSelchangeMaterialElasticModulus)
// 2009.11.09 bagus Stress 追加変更 --}--
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSampleSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSampleSettingDlg::OnInitDialog()
{
	///// Load File /////
	m_bNew = !RecipeFile_LoadRecipe(&m_SampleInfo, m_szName, RECIPE_FILE_SAMPLE);

	CDialog::OnInitDialog();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_SAMPLE_NAME))->SetLimitText(RECIPE_NAME_LEN);

	///// Ok Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Teaching Button /////
	m_TeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TeachingButton.DrawFlatFocus(TRUE);

// 2009.11.09 bagus Stress 追加変更 --{--
#if 0
	// 2009.08.26 bagus stress --{--
	memset(&m_ElasticTable, 0, sizeof(m_ElasticTable));
	ConfigFile_GetNanoSpecIni(&m_ElasticTable, CONFIG_FILE_STRESS_MODULUS_CONFIG);
#endif
// 2009.11.09 bagus Stress 追加変更 --}--
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// 2009.08.26 bagus stress --}--

	///// Initialize /////
	InitCombo_SampleShape();

// 2009.11.09 bagus Stress 追加変更 --{--
#if 0
	// 2009.08.26 bagus stress --{--
	InitCombo_MaterialElasticModulus();
	if (m_SystemConfig.HeadType.bStress) {
		GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS_LABEL)->ShowWindow(TRUE);
		GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS)->ShowWindow(TRUE);
	} else {
		GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS_LABEL)->ShowWindow(FALSE);
		GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS)->ShowWindow(FALSE);
	}
	// 2009.08.26 bagus stress --}--
#endif
// 2009.11.09 bagus Stress 追加変更 --}--

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSampleSettingDlg::OnOK()
{
	///// Save /////
	if(!CheckData()) return;

	//2009.12.01 bagus NANOMAP --{--
	STAGE_CONFIG	l_StatgeConfig;
	ConfigFile_GetNanoSpecIni(&l_StatgeConfig,CONFIG_FILE_STAGE_CONFIG);
	m_SampleInfo.StageConfig = l_StatgeConfig;
	//2009.12.01 bagus NANOMAP --}--

	RecipeFile_SaveRecipe(&m_SampleInfo, m_SampleInfo.szName, RECIPE_FILE_SAMPLE);

	CDialog::OnOK();
}

// =========================================================================
//
void CSampleSettingDlg::OnTeachingButton()
{
	COverlayDialog dlgOverlay;
	char szBuff[256];
	//Saiki 20090603 Add ----->
	CString strMsg, strCap;
	//Saiki 20090603 Add <-----

	dlgOverlay.m_lX = m_SampleInfo.Origin.lX;
	dlgOverlay.m_lY = m_SampleInfo.Origin.lY;

	//Sampleセット
	dlgOverlay.SetFileName(m_SampleInfo.szName);
	dlgOverlay.SetSampleMode(1);
	//Saiki 20090603 Change ----->
	//dlgOverlay.SetCaption(SAMPLESETTING_CAPTION);
	//dlgOverlay.SetMessage(SAMPLESETTING_MESSAGE);
	LoadStringML(IDS_SAMPLESETTING_CAPTION, strCap, "SAMPLE SETTING");
	LoadStringML(IDS_SAMPLESETTING_MESSAGE, strMsg, "SAMPLE SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);
	//Saiki 20090603 Change <-----
	if( dlgOverlay.DoModal() == IDOK ){
		m_SampleInfo.Origin.lX = dlgOverlay.m_lX;
		m_SampleInfo.Origin.lY = dlgOverlay.m_lY;

		sprintf(szBuff, "%ld", m_SampleInfo.Origin.lX );
		SetDlgItemText(IDC_SAMPLE_ORIGIN_X, szBuff);

		sprintf(szBuff, "%ld", m_SampleInfo.Origin.lY );
		SetDlgItemText(IDC_SAMPLE_ORIGIN_Y, szBuff);
	}
}

// =========================================================================
//
void CSampleSettingDlg::OnSelchangeSampleShape()
{
	m_SampleInfo.nShape = ((CComboBox*)GetDlgItem(IDC_SAMPLE_SHAPE))->GetCurSel();
}

// =========================================================================
//
void CSampleSettingDlg::InitCombo_SampleShape()
{
	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_SAMPLE_SHAPE);
	pCombo->ResetContent();
	for ( int i = 0; i < SAMPLE_SHAPE_MAX; i++ ){
		// Kojika 20090528 Change
		//pCombo->AddString(g_lpszSampleShape[i]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszSampleShape_ENU[i]);
		}else{
			pCombo->AddString(g_lpszSampleShape_JPN[i]);
		}
		// Kojika 20090528 Change End
	}
	pCombo->SetCurSel(m_SampleInfo.nShape);
}

// =========================================================================
//
// 2013.01.22 bagus 原点有効範囲内・サンプルサイズオーバー順序でチェックするように変更
BOOL CSampleSettingDlg::CheckData()
{
	if(UpdateData(TRUE) == FALSE) return FALSE;

	CString strBuffer;
	int iValue;
	double dValue;
	double dStageSizeX;
	double dStageSizeY;
	//Saiki 20090603 Add ----->
	CString strMsg, strTitle;
	//Saiki 20090603 Add <-----

	SR_LENS_CENTER_OFFSET currLensOffset;
	SR_LENS_CENTER_OFFSET zeroLensOffset;

	ConfigFile_GetNanoSpecIni(&currLensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);			// 参照のみ

	STAGE_COORD coordLeftTop;
	STAGE_COORD coordRightBottom;

	long lStageXEdgeMin;
	long lStageXEdgeMax;
	long lSampleXEdgeMin;
	long lSampleXEdgeMax;
	long lStageYEdgeMin;
	long lStageYEdgeMax;
	long lSampleYEdgeMin;
	long lSampleYEdgeMax;

	coordLeftTop.lX = static_cast<long>(m_StageConfig.StageMaxCoord.dLeftX + m_StageConfig.Edge.dX);
	coordLeftTop.lY = static_cast<long>(m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.Edge.dY);
	coordRightBottom.lX = static_cast<long>(m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.Edge.dX);
	coordRightBottom.lY = static_cast<long>(m_StageConfig.StageMaxCoord.dDownY + m_StageConfig.Edge.dY);

	// 現在のレンズOffset情報消去（座標変換にレンズオフセットを加味しない）
	::ZeroMemory(&zeroLensOffset, sizeof(zeroLensOffset));
	ConfigFile_SetNanoSpecIni(&zeroLensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);

	NS_ConvertToStageGetPosCoord(&coordLeftTop);
	NS_ConvertToStageGetPosCoord(&coordRightBottom);

	// 現在のレンズOffset復元
	ConfigFile_SetNanoSpecIni(&currLensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);

	///// Sample Origin X /////
	lStageXEdgeMin = min(coordLeftTop.lX, coordRightBottom.lX);
	lStageXEdgeMax = max(coordLeftTop.lX, coordRightBottom.lX);
	if(( m_SampleInfo.Origin.lX < lStageXEdgeMin ) || ( lStageXEdgeMax < m_SampleInfo.Origin.lX )){
		//Saiki 20090603 Change ----->
		//MessageBox("X axis origin coordinate is out of range the stage.", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("Ｘ軸の原点座標がステージの範囲外に設定されています", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_X_AXIS_ORIGIN_OUT_RANGE, strMsg, "X axis origin coordinate is out of range the stage.");
		MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change <-----
		GetDlgItem(IDC_SAMPLE_ORIGIN_X)->SetFocus();
		return FALSE;
	}

	///// Sample Origin Y /////
	lStageYEdgeMin = min(coordLeftTop.lY, coordRightBottom.lY);
	lStageYEdgeMax = max(coordLeftTop.lY, coordRightBottom.lY);
	if(( m_SampleInfo.Origin.lY < lStageYEdgeMin ) || ( lStageYEdgeMax < m_SampleInfo.Origin.lY )){
		//Saiki 20090603 Change ----->
		//MessageBox("Y axis origin coordinate is out of range the stage.", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("Ｙ軸の原点座標がステージの範囲外に設定されています", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_Y_AXIS_ORIGIN_OUT_RANGE, strMsg, "Y axis origin coordinate is out of range the stage.");
		MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change <-----
		GetDlgItem(IDC_SAMPLE_ORIGIN_Y)->SetFocus();
		return FALSE;
	}

	///// ステージ原点ﾎﾟｼﾞｼｮﾝによる有効エリアチェック /////
	//	  現行センター中心のみに対応
// 2009.12.08 bagus --{--
#if 0
// 	switch ( m_StageConfig.OrgPos ) {
// 	case 1: // 中心
// 		///// Sample Origin X + Sample Size X　有効エリアチェック/////
// 		if( (( m_SampleInfo.Origin.lX - (m_SampleInfo.Size.dx / 2)) < min(coordLeftTop.lX, coordRightBottom.lX) ) || ( max(coordLeftTop.lX, coordRightBottom.lX) < (m_SampleInfo.Origin.lX + (m_SampleInfo.Size.dx / 2))) ){
// 			//Saiki 20090603 Change ----->
// 			//MessageBox("X axis origin coordinate is out of range the stage.", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
// 			//MessageBox("Ｘ軸の原点座標がステージの範囲外に設定されています", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
// 			LoadStringML(IDS_X_AXIS_ORIGIN_OUT_RANGE, strMsg, "X axis origin coordinate is out of range the stage.");
// 			MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
// 			//Saiki 20090603 Change <-----
// 			GetDlgItem(IDC_SAMPLE_ORIGIN_X)->SetFocus();
// 			return FALSE;
// 		}
//
// 		///// Sample Origin Y + Sample Size Y　有効エリアチェック/////
// 		if( (( m_SampleInfo.Origin.lY - (m_SampleInfo.Size.dy / 2)) < min(coordLeftTop.lY, coordRightBottom.lY) ) || ( max(coordLeftTop.lY, coordRightBottom.lY) < (m_SampleInfo.Origin.lY + (m_SampleInfo.Size.dy / 2))) ){
// 			//Saiki 20090603 Change ----->
// 			//MessageBox("Y axis origin coordinate is out of range the stage.", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
// 			//MessageBox("Ｙ軸の原点座標がステージの範囲外に設定されています", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
// 			LoadStringML(IDS_Y_AXIS_ORIGIN_OUT_RANGE, strMsg, "Y axis origin coordinate is out of range the stage.");
// 			MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
// 			//Saiki 20090603 Change <-----
// 			GetDlgItem(IDC_SAMPLE_ORIGIN_Y)->SetFocus();
// 			return FALSE;
// 		}
// 		break;
//
// 	case 2: // 左上
// 	case 3: // 左下
// 	case 4: // 右下
// 	case 5: // 右上
// 		break;
// 	}
#else
	switch ( m_StageConfig.OrgPos ) {
	case 1: // 中心
	default:
		///// Sample Origin X + Sample Size X　有効エリアチェック/////
		lStageXEdgeMin = min(coordLeftTop.lX, coordRightBottom.lX);
		lStageXEdgeMax = max(coordLeftTop.lX, coordRightBottom.lX);
		lSampleXEdgeMin = static_cast<long>(m_SampleInfo.Origin.lX - (m_SampleInfo.Size.dx / 2));
		lSampleXEdgeMax = static_cast<long>(m_SampleInfo.Origin.lX + (m_SampleInfo.Size.dx / 2));

		///// Sample Origin Y + Sample Size Y　有効エリアチェック/////
		lStageYEdgeMin = min(coordLeftTop.lY, coordRightBottom.lY);
		lStageYEdgeMax = max(coordLeftTop.lY, coordRightBottom.lY);
		lSampleYEdgeMin = static_cast<long>(m_SampleInfo.Origin.lY - (m_SampleInfo.Size.dy / 2));
		lSampleYEdgeMax = static_cast<long>(m_SampleInfo.Origin.lY + (m_SampleInfo.Size.dy / 2));
		break;
	case 2: // 左上
		///// Sample Origin X + Sample Size X　有効エリアチェック/////
		lStageXEdgeMin = min(coordLeftTop.lX, coordRightBottom.lX);
		lStageXEdgeMax = max(coordLeftTop.lX, coordRightBottom.lX);
		if ( m_StageConfig.Dir.X == DIR_RIGHT ||
			 m_StageConfig.Dir.X == DIR_BOTTOM ) {
			lSampleXEdgeMin = m_SampleInfo.Origin.lX;
			lSampleXEdgeMax = static_cast<long>(m_SampleInfo.Origin.lX + m_SampleInfo.Size.dx);
		}
		else {
			lSampleXEdgeMin = static_cast<long>(m_SampleInfo.Origin.lX - m_SampleInfo.Size.dx);
			lSampleXEdgeMax = m_SampleInfo.Origin.lX;
		}

		///// Sample Origin Y + Sample Size Y　有効エリアチェック/////
		lStageYEdgeMin = min(coordLeftTop.lY, coordRightBottom.lY);
		lStageYEdgeMax = max(coordLeftTop.lY, coordRightBottom.lY);
		if ( m_StageConfig.Dir.Y == DIR_RIGHT ||
			 m_StageConfig.Dir.Y == DIR_BOTTOM ) {
			lSampleYEdgeMin = m_SampleInfo.Origin.lY;
			lSampleYEdgeMax = static_cast<long>(m_SampleInfo.Origin.lY + m_SampleInfo.Size.dy);
		}
		else {
			lSampleYEdgeMin = static_cast<long>(m_SampleInfo.Origin.lY - m_SampleInfo.Size.dy);
			lSampleYEdgeMax = m_SampleInfo.Origin.lY;
		}
		break;
	case 3: // 左下
		///// Sample Origin X + Sample Size X　有効エリアチェック/////
		lStageXEdgeMin = min(coordLeftTop.lX, coordRightBottom.lX);
		lStageXEdgeMax = max(coordLeftTop.lX, coordRightBottom.lX);
		if ( m_StageConfig.Dir.X == DIR_RIGHT ||
			 m_StageConfig.Dir.X == DIR_TOP ) {
			lSampleXEdgeMin = m_SampleInfo.Origin.lX;
			lSampleXEdgeMax = static_cast<long>(m_SampleInfo.Origin.lX + m_SampleInfo.Size.dx);
		}
		else {
			lSampleXEdgeMin = static_cast<long>(m_SampleInfo.Origin.lX - m_SampleInfo.Size.dx);
			lSampleXEdgeMax = m_SampleInfo.Origin.lX;
		}

		///// Sample Origin Y + Sample Size Y　有効エリアチェック/////
		lStageYEdgeMin = min(coordLeftTop.lY, coordRightBottom.lY);
		lStageYEdgeMax = max(coordLeftTop.lY, coordRightBottom.lY);
		if ( m_StageConfig.Dir.Y == DIR_RIGHT ||
			 m_StageConfig.Dir.Y == DIR_TOP ) {
			lSampleYEdgeMin = m_SampleInfo.Origin.lY;
			lSampleYEdgeMax = static_cast<long>(m_SampleInfo.Origin.lY + m_SampleInfo.Size.dy);
		}
		else {
			lSampleYEdgeMin = static_cast<long>(m_SampleInfo.Origin.lY - m_SampleInfo.Size.dy);
			lSampleYEdgeMax = m_SampleInfo.Origin.lY;
		}
		break;
	case 4: // 右下
		///// Sample Origin X + Sample Size X　有効エリアチェック/////
		lStageXEdgeMin = min(coordLeftTop.lX, coordRightBottom.lX);
		lStageXEdgeMax = max(coordLeftTop.lX, coordRightBottom.lX);
		if ( m_StageConfig.Dir.X == DIR_LEFT ||
			 m_StageConfig.Dir.X == DIR_TOP ) {
			lSampleXEdgeMin = m_SampleInfo.Origin.lX;
			lSampleXEdgeMax = static_cast<long>(m_SampleInfo.Origin.lX + m_SampleInfo.Size.dx);
		}
		else {
			lSampleXEdgeMin = static_cast<long>(m_SampleInfo.Origin.lX - m_SampleInfo.Size.dx);
			lSampleXEdgeMax = m_SampleInfo.Origin.lX;
		}

		///// Sample Origin Y + Sample Size Y　有効エリアチェック/////
		lStageYEdgeMin = min(coordLeftTop.lY, coordRightBottom.lY);
		lStageYEdgeMax = max(coordLeftTop.lY, coordRightBottom.lY);
		if ( m_StageConfig.Dir.Y == DIR_LEFT ||
			 m_StageConfig.Dir.Y == DIR_TOP ) {
			lSampleYEdgeMin = m_SampleInfo.Origin.lY;
			lSampleYEdgeMax = static_cast<long>(m_SampleInfo.Origin.lY + m_SampleInfo.Size.dy);
		}
		else {
			lSampleYEdgeMin = static_cast<long>(m_SampleInfo.Origin.lY - m_SampleInfo.Size.dy);
			lSampleYEdgeMax = m_SampleInfo.Origin.lY;
		}
		break;
	case 5: // 右上
		///// Sample Origin X + Sample Size X　有効エリアチェック/////
		lStageXEdgeMin = min(coordLeftTop.lX, coordRightBottom.lX);
		lStageXEdgeMax = max(coordLeftTop.lX, coordRightBottom.lX);
		if ( m_StageConfig.Dir.X == DIR_LEFT ||
			 m_StageConfig.Dir.X == DIR_BOTTOM ) {
			lSampleXEdgeMin = m_SampleInfo.Origin.lX;
			lSampleXEdgeMax = static_cast<long>(m_SampleInfo.Origin.lX + m_SampleInfo.Size.dx);
		}
		else {
			lSampleXEdgeMin = static_cast<long>(m_SampleInfo.Origin.lX - m_SampleInfo.Size.dx);
			lSampleXEdgeMax = m_SampleInfo.Origin.lX;
		}

		///// Sample Origin Y + Sample Size Y　有効エリアチェック/////
		lStageYEdgeMin = min(coordLeftTop.lY, coordRightBottom.lY);
		lStageYEdgeMax = max(coordLeftTop.lY, coordRightBottom.lY);
		if ( m_StageConfig.Dir.Y == DIR_LEFT ||
			 m_StageConfig.Dir.Y == DIR_BOTTOM ) {
			lSampleYEdgeMin = m_SampleInfo.Origin.lY;
			lSampleYEdgeMax = static_cast<long>(m_SampleInfo.Origin.lY + m_SampleInfo.Size.dy);
		}
		else {
			lSampleYEdgeMin = static_cast<long>(m_SampleInfo.Origin.lY - m_SampleInfo.Size.dy);
			lSampleYEdgeMax = m_SampleInfo.Origin.lY;
		}
		break;

	}

	if( ( lSampleXEdgeMin < lStageXEdgeMin ) || ( lStageXEdgeMax < lSampleXEdgeMax ) ){
		LoadStringML(IDS_XSIZE_STAGE_OUT_RANGE, strMsg, "X size is out of range the stage.");
		MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
		GetDlgItem(IDC_SAMPLE_SIZE_X)->SetFocus();
		return FALSE;
	}

	if( ( lSampleYEdgeMin < lStageYEdgeMin ) || ( lStageYEdgeMax < lSampleYEdgeMax ) ){
		LoadStringML(IDS_YSIZE_STAGE_OUT_RANGE, strMsg, "Y size is out of range the stage.");
		MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
		GetDlgItem(IDC_SAMPLE_SIZE_Y)->SetFocus();
		return FALSE;
	}
#endif

// 2009.11.09 bagus Stress 追加変更 --{--
#if 0
// 	// 2009.08.26 bagus stress --{--
// 	///// Material Elastic Modulus /////
// 	if (m_SystemConfig.HeadType.bStress) {
// 		CComboBox* pCombo;
// 		pCombo = (CComboBox*)GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS);
//
// 		int nIndex = pCombo->GetCurSel();
// 		if (nIndex < 0) {
// // 2009.09.09 bagus stress StringTable使用 --{--
// //			AfxMessageBox("Err:Material Elastic Modulus");
// 			CString	strBuffer;
// 			LoadStringML(IDS_STRESS_SELECT_MATERIAL_ELASTIC_MODULUS, strBuffer, "Please select material elastic modulus.");
// 			AfxMessageBox(strBuffer);
// // 2009.09.09 bagus stress StringTable使用 --}--
// 			return FALSE;
// 		}
// 	}
// 	// 2009.08.26 bagus stress --}--
#endif
// 2009.11.09 bagus Stress 追加変更 --}--

	// 2013.01.09 bagus -->
// 	dStageSizeX = (m_StageConfig.Size.dX) - 2*(m_StageConfig.Edge.dX);
// 	dStageSizeY = (m_StageConfig.Size.dY) - 2*(m_StageConfig.Edge.dY);
	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dStageSizeX = (m_StageConfig.Size.dX) - 2*(m_StageConfig.Edge.dX);
		dStageSizeY = (m_StageConfig.Size.dY) - 2*(m_StageConfig.Edge.dY);
	}
	else {
		dStageSizeX = (m_StageConfig.Size.dY) - 2*(m_StageConfig.Edge.dY);
		dStageSizeY = (m_StageConfig.Size.dX) - 2*(m_StageConfig.Edge.dX);
	}
	// 2013.01.09 bagus <--

	//Saiki 20090603 Add ----->
	LoadStringML(IDS_TITLE_SAMPLE_SETTING, strTitle, "SAMPLE SETTING");
	//Saiki 20090603 Add <-----

	///// Sample Name /////
	if ( strlen(m_SampleInfo.szName) == 0 ) {
		//MessageBox("Please Enter a Name", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change ----->
		//MessageBox("名前を入力してください", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_ENTER_A_NAME, strMsg, "Please Enter a Name");
		MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change <-----
		GetDlgItem(IDC_SAMPLE_NAME)->SetFocus();
		return FALSE;
	}

	///// Sample Shape /////
	iValue = m_SampleInfo.nShape;
	if(iValue < 0){
		//MessageBox("Please Select a Sample Shape", "SAMPLE SETTIN", MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change ----->
		//MessageBox("サンプルの形状を選択してください", "SAMPLE SETTIN", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_SELECT_SAMPLE_SHAPE, strMsg, "Please Select a Sample Shape");
		MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change <-----
		return FALSE;
	}

	///// Sample Thickness /////
	dValue = m_SampleInfo.dThickness;
	if(!(dValue >= MIN_FILM_THICKNESS  &&  dValue <= MAX_FILM_THICKNESS)){
		//Saiki 20090603 Change ----->
		//strBuffer.Format("Sample thickness is out of range.\n(%d - %d)", MIN_FILM_THICKNESS, MAX_FILM_THICKNESS);
		//strBuffer.Format("サンプルの膜厚が範囲外に設定されています\n(%d - %d)", MIN_FILM_THICKNESS, MAX_FILM_THICKNESS);
		//MessageBox(strBuffer, "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_SAMPLE_THICKNESS_OUT_RANGE, strMsg, "Sample thickness is out of range.\n(%d - %d)");
		strBuffer.Format(strMsg, MIN_FILM_THICKNESS, MAX_FILM_THICKNESS);
		MessageBox(strBuffer, strTitle, MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change <-----
		GetDlgItem(IDC_SAMPLE_THICKNESS)->SetFocus();
		return FALSE;
	}

	///// Sample Size X /////
	if( dStageSizeX < m_SampleInfo.Size.dx ){
		//Saiki 20090603 Change ----->
		//MessageBox("X size is out of range the stage.", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("Ｘ軸側のサンプルサイズがステージの範囲外に設定されています", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_XSIZE_STAGE_OUT_RANGE, strMsg, "X size is out of range the stage.");
		MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change <-----
		GetDlgItem(IDC_SAMPLE_SIZE_X)->SetFocus();
		return FALSE;
	}

	///// Sample Size Y /////
	if( dStageSizeY < m_SampleInfo.Size.dy ){
		//Saiki 20090603 Change ----->
		//MessageBox("Y size is out of range the stage.", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		//MessageBox("Ｙ軸側のサンプルサイズがステージの範囲外に設定されています", "SAMPLE SETTING", MB_OK|MB_ICONSTOP);
		LoadStringML(IDS_YSIZE_STAGE_OUT_RANGE, strMsg, "Y size is out of range the stage.");
		MessageBox(strMsg, strTitle, MB_OK|MB_ICONSTOP);
		//Saiki 20090603 Change <-----
		GetDlgItem(IDC_SAMPLE_SIZE_Y)->SetFocus();
		return FALSE;
	}

	return TRUE;
}

// 2009.11.09 bagus Stress 追加変更 --{--
#if 0
// 2009.08.26 bagus stress --{--
void CSampleSettingDlg::InitCombo_MaterialElasticModulus()
{
	int	i;
	CString	strItem;
	CString	strSample;

	strSample.Format("%e %s", m_SampleInfo.dElasticModulusValue, m_SampleInfo.tszElasticModulusName);

	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS);
	pCombo->ResetContent();
	for (i=0; i<m_ElasticTable.uiTableNum; i++) {
		strItem.Format("%e %s", m_ElasticTable.ElasticModulus[i].dElasticModulusValue, m_ElasticTable.ElasticModulus[i].tszElasticModulusName);
		pCombo->AddString(strItem);
	}
	int nIndex = pCombo->FindStringExact(-1, strSample);
	pCombo->SetCurSel(nIndex);
}

void CSampleSettingDlg::OnSelchangeMaterialElasticModulus()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS);

	int nIndex = pCombo->GetCurSel();

	strcpy(m_SampleInfo.tszElasticModulusName, m_ElasticTable.ElasticModulus[nIndex].tszElasticModulusName);
	m_SampleInfo.dElasticModulusValue = m_ElasticTable.ElasticModulus[nIndex].dElasticModulusValue;
}
// 2009.08.26 bagus stress --}--
#endif
// 2009.11.09 bagus Stress 追加変更 --}--
