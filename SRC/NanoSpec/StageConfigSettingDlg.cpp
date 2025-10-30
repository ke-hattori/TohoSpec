// StageConfigSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "PointSetupTeachingDlg.h"
#include "StageConfigSettingDlg.h"
#include "..\\..\\INC\\StageInt.hxx"

// NanoSpec.iniファイル
//#define INISECTION_STAGESIZE			_T("STAGESIZE")//HWS時のサブコンからのデータ読み込みの代わり
//#define INIKEY_HWSTAGESIMULATIN			_T("HWStageSimulation")
//#define INIKEY_HWS_STAGESIZEX 		_T("StageSizeX")
//#define INIKEY_HWS_STAGESIZEY 		_T("StageSizeY")
//#define INIKEY_HWS_STAGEEDGESIZEX 	_T("EdgeSizeX")
//#define INIKEY_HWS_STAGEEDGESIZEY 	_T("EdgeSizeY")
//#define INIKEY_HWS_STAGEORIGINOFFSETX _T("OriginOffsetX")
//#define INIKEY_HWS_STAGEORIGINOFFSETY _T("OriginOffsetY")

//サブコンステージ設定可能サイズ
#define SUBCON_STAGE_MAX_SIZE	(99999)
#define SUBCON_STAGE_MIN_SIZE	(0)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT StageMachineOriginPos_ID[] = {
	IDC_MACHINE_ORG_POS_1,	// 中心
	IDC_MACHINE_ORG_POS_2,	// 左上
	IDC_MACHINE_ORG_POS_3,	// 左下
	IDC_MACHINE_ORG_POS_4,	// 右下
	IDC_MACHINE_ORG_POS_5,	// 右上
};

static UINT StageOriginPos_ID[] = {
	IDC_ORG_POS_1,	// 中心
	IDC_ORG_POS_2,	// 左上
	IDC_ORG_POS_3,	// 左下
	IDC_ORG_POS_4,	// 右下
	IDC_ORG_POS_5,	// 右上
};

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


// #########################################################################
// CStageConfigSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CStageConfigSettingDlg::CStageConfigSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStageConfigSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStageConfigSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CStageConfigSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStageConfigSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_SAMPLE_SIZE_X, m_StageConfig.Size.dX);
	DDX_Text(pDX, IDC_SAMPLE_SIZE_Y, m_StageConfig.Size.dY);
	DDX_Text(pDX, IDC_STAGE_EDGE_X, m_StageConfig.Edge.dX);
	DDX_Text(pDX, IDC_STAGE_EDGE_Y, m_StageConfig.Edge.dY);
	DDX_Text(pDX, IDC_ORIGIN_X, m_StageConfig.Origin.lX);
	DDX_Text(pDX, IDC_ORIGIN_Y, m_StageConfig.Origin.lY);
//Saiki 20091113 Add ----->
	DDX_Text(pDX, IDC_COORD_UP, m_StageConfig.StageMaxCoord.dUpY);
	DDX_Text(pDX, IDC_COORD_DOWN, m_StageConfig.StageMaxCoord.dDownY);
	DDX_Text(pDX, IDC_COORD_LEFT, m_StageConfig.StageMaxCoord.dLeftX);
	DDX_Text(pDX, IDC_COORD_RIGHT, m_StageConfig.StageMaxCoord.dRightX);
//Saiki 20091113 Add <-----
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_TEACHING_BUTTON, m_TeachingButton);
	DDX_Control(pDX, IDC_SET_MACHINE_ORIGIN_BUTTON, m_SetButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CStageConfigSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CStageConfigSettingDlg)
	ON_BN_CLICKED(IDC_TEACHING_BUTTON, OnTeachingButton)
	ON_EN_CHANGE(IDC_STAGE_EDGE_X, OnChangeStageEdgeX)
	ON_EN_CHANGE(IDC_STAGE_EDGE_Y, OnChangeStageEdgeY)
	ON_BN_CLICKED(IDC_SET_MACHINE_ORIGIN_BUTTON, OnSetMachineOriginButton)
	ON_CBN_SELCHANGE(IDC_ROTATE_VIEW_COMBO, OnSelchangeRotateViewCombo)
	//}}AFX_MSG_MAP
// 2009.05.25 K.Matsuo StageConfigSetting SPT <--
//	ON_CONTROL_RANGE(BN_CLICKED, IDC_MACHINE_ORG_POS_1, IDC_MACHINE_ORG_POS_5, OnStageOrgPosRadioButton)
// 2009.05.25 K.Matsuo StageConfigSetting SPT <--
	ON_CONTROL_RANGE(BN_CLICKED, IDC_ORG_POS_1, IDC_ORG_POS_5, OnStageOrgPosRadioButton)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_DIR_1, IDC_DIR_4, OnStageCoordDirComboBox)
END_MESSAGE_MAP()

// =========================================================================
// CStageConfigSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CStageConfigSettingDlg::OnInitDialog()
{
	// Kojika 20090603 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090603 Add End

	//Saiki 20091120 Delete <Teachingからオフセット位置を取得> ----->
	////未対応機能disable
	//GetDlgItem(IDC_TEACHING_BUTTON)->ShowWindow(SW_HIDE);
	//Saiki 20091120 Delete <Teachingからオフセット位置を取得> <-----

	///// Load Stage Coordinate /////
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
/* added hmenjo 2009.05.20 SPT ステージ用設定対応 -------- { -------- */
	//Saiki 20091210 Change ----->
	//SYSTEM_CONFIG l_SystemConfig;
	//ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//if (STAGE_TYPE_SPT == l_SystemConfig.nStageType) {
	ConfigFile_GetNanoSpecIni(&ml_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	if (STAGE_TYPE_SPT == ml_SystemConfig.nStageType) {
	//Saiki 20091210 Change <-----
		CString l_strDlgStr;
		// Kojika 20090603 Change
		//l_strDlgStr = _T("This size is stage size by the stage controller. \n(P509, P510, P511, P512)");
		//LoadStringML(IDS_THIS_SIZE_IS_STAGE_SIZE, l_strBuffer, "This size is stage size by the stage controller. \n(P509, P510, P511, P512)");
		LoadStringML(IDS_THIS_SIZE_IS_STAGE_SIZE, l_strBuffer, "These values are givrn by stage controller.");
		l_strDlgStr = l_strBuffer;
		SetDlgItemText(IDC_STATIC_STGSIZE, l_strDlgStr);
		//l_strDlgStr = _T("(This size is invalid area from the stage edge  by the stage controller.) unused with SPT stage.");
		//LoadStringML(IDS_THIS_SIZE_IS_INVALID_AREA, l_strBuffer, "This size is invalid area from the stage edge  by the stage controller.) unused with SPT stage.");
		LoadStringML(IDS_THIS_SIZE_IS_INVALID_AREA, l_strBuffer, "This size is invalid area from the stage edge.");
		l_strDlgStr = l_strBuffer;
		SetDlgItemText(IDC_STATIC_EDGESIZE, l_strDlgStr);
		//l_strDlgStr = _T("This is offset value to adjust logical origin position.");
		LoadStringML(IDS_THIS_IS_OFFSET_VALUE, l_strBuffer, "This is offset value to adjust logical origin position.");
		l_strDlgStr = l_strBuffer;
		// Kojika 20090603 Change End
		SetDlgItemText(IDC_STATIC_STGOFFSET, l_strDlgStr);
/* added 2011.11.14 hmenjo [６]StageCfg画面 ---------- { ---------- */
	} else {
		this->GetDlgItem(IDC_SET_MACHINE_ORIGIN_BUTTON)->EnableWindow(FALSE);
/* added 2011.11.14 hmenjo [６]StageCfg画面 ---------- } ---------- */
	}
/* added hmenjo 2009.05.20 SPT ステージ用設定対応 -------- } -------- */

	CDialog::OnInitDialog();

	InitCombo_StageCoord();
	InitRadio_StageOrgPos();

// 2013.01.17 bagus rotate xy view -->
	InitCombo_RotateViewCoord();
// 2013.01.17 bagus rotate xy view <--

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Teaching Button /////
	m_TeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TeachingButton.DrawFlatFocus(TRUE);

	///// Set Button /////
	m_SetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SetButton.DrawFlatFocus(TRUE);

	TCHAR szBuff[256];
	EEPROMDATA pEEPromData;
	CString strBuffer;
	long lStageSizeX;	//ステージサイズを受け取る（S28）
	long lStageSizeY;	//ステージサイズを受け取る（S29）
	long lEdgeSizeX;	//ステージサイズを受け取る（S36）
	long lEdgeSizeY;	//ステージサイズを受け取る（S37）
	long lOffsetX;		//ステージサイズを受け取る（S40）
	long lOffsetY;		//ステージサイズを受け取る（S41）
	//Saiki 20091113 Add ----->
	long lMachineOriginUp;		//(P509)
	long lMachineOriginDown;	//(P510)
	long lMachineOriginRight;	//(P511)
	long lMachineOriginLeft;	//(P512)
	//Saiki 20091113 Add <-----

	//Saiki 20091210 Change ----->
	//if (g_bHWStageSimulation){
	//	  lStageSizeX = g_lHWStageSizeX / (MICROMETRE / 10);
	//	  lStageSizeY = g_lHWStageSizeY / (MICROMETRE / 10);
	//	  lEdgeSizeX = g_lHWEdgeSizeX / (MICROMETRE / 10);
	//	  lEdgeSizeY = g_lHWEdgeSizeY / (MICROMETRE / 10);
	//	  lOffsetX = g_lHWOriginOffsetX;
	//	  lOffsetY = g_lHWOriginOffsetY;
	//}
	if(g_bHWS && STAGE_TYPE_SPT == ml_SystemConfig.nStageType){/*シミュレーションモード　かつ　ステージがSPT*/

		lStageSizeX = static_cast<long>(m_StageConfig.Size.dX);
		lStageSizeY = static_cast<long>(m_StageConfig.Size.dY);
	}
	else if(g_bHWStageSimulation && STAGE_TYPE_SPT != ml_SystemConfig.nStageType){/*ステージシミュレーションモード　かつ　ステージがTOHOかNTN*/
			/*サブコン対応時に要確認すること*/
			lStageSizeX = g_lHWStageSizeX / (MICROMETRE / 10);
			lStageSizeY = g_lHWStageSizeY / (MICROMETRE / 10);
			lEdgeSizeX = g_lHWEdgeSizeX / (MICROMETRE / 10);
			lEdgeSizeY = g_lHWEdgeSizeY / (MICROMETRE / 10);
			lOffsetX = g_lHWOriginOffsetX;
			lOffsetY = g_lHWOriginOffsetY;
	}
	//Saiki 20091210 Change <-----
	else{
		StageGetEEPromData(&pEEPromData);

/* modified hmenjo 2009.05.20 SPT ステージ用設定対応 -------- { -------- */
//		if ((pEEPromData.AutoStage.Size.x <= 0) || (pEEPromData.AutoStage.Size.y <= 0)
//			|| (pEEPromData.AutoStage.Edge.x < 0) || (pEEPromData.AutoStage.Edge.y < 0)
//			|| (pEEPromData.AutoStage.Origin.lX < 0) || (pEEPromData.AutoStage.Origin.lY < 0)){
//			sprintf(szBuff, "Can not get the parameter of the sub controller.");
//			MessageBox(szBuff, "STAGE CONFIG", MB_OK | MB_ICONSTOP);
//			return TRUE;
//		}
/* modified hmenjo 2009.05.20 SPT ステージ用設定対応 --------			 */
		BOOL l_bRet = StageGetStageSize(&lStageSizeX, &lStageSizeY);
		BOOL l_bRet2 = StageGetOrgLoc(&lOffsetX, &lOffsetY);
		//Saiki 20091210 Change ----->
		//if (STAGE_TYPE_SPT == l_SystemConfig.nStageType) {
		if (STAGE_TYPE_SPT == ml_SystemConfig.nStageType) {
		//Saiki 20091210 Change <-----
			/* ステージタイプは，SPT */
			if (
				(FALSE == l_bRet)
			 || (FALSE == l_bRet2)
			 || ((lStageSizeX <= 0) || (lStageSizeY <= 0))
			 || ((lOffsetX <= 0) || (lOffsetY <= 0))
				) {
				// Kojika 20090603 Change
				//sprintf(szBuff, "Can not get the parameter of the MotSys Driver for SPT stage.");
				//MessageBox(szBuff, "STAGE CONFIG", MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_CANNOT_GET_PARA_MOTSYS_DRIVER, l_strBuffer, "Can not get the parameter of the MotSys Driver for SPT stage.");
				LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
				sprintf(szBuff, l_strBuffer);
				MessageBox(szBuff, l_strTitle, MB_OK | MB_ICONSTOP);
				// Kojika 20090603 Change End
				return TRUE;
			}
			lEdgeSizeX = 0;
			lEdgeSizeY = 0;
			//Saiki 20091113 Add ----->
			/*SPTパラメータから、SPT稼動範囲を計算*/
			lMachineOriginLeft = (lOffsetX * (-1));
			lMachineOriginDown = (lOffsetY * (-1));
			lMachineOriginRight = (lOffsetX * (-1)) + lStageSizeX;
			lMachineOriginUp = (lOffsetY * (-1)) + lStageSizeY;
			//Saiki 20091113 Add <-----
		} else {
			/* ステージタイプは，TOHO あるいは NTN */
			if (
				(FALSE == l_bRet)
			 || (FALSE == l_bRet2)
			 || ((lStageSizeX <= 0) || (lStageSizeY <= 0))
			 || ((pEEPromData.AutoStage.Edge.x < 0) || (pEEPromData.AutoStage.Edge.y < 0))
			 || ((lOffsetX <= 0) || (lOffsetY <= 0))
				){
				// Kojika 20090603 Change
				//sprintf(szBuff, "Can not get the parameter of the sub controller.");
				//MessageBox(szBuff, "STAGE CONFIG", MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_CANNOT_GET_PARA_SUB_CONTROLLER, l_strBuffer, "Can not get the parameter of the sub controller.");
				LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
				sprintf(szBuff, l_strBuffer);
				MessageBox(szBuff, l_strTitle, MB_OK | MB_ICONSTOP);
				// Kojika 20090603 Change End
				return TRUE;
			}
			lStageSizeX *= (MICROMETRE / 10);
			lStageSizeY *= (MICROMETRE / 10);
			lEdgeSizeX = pEEPromData.AutoStage.Edge.x * (MICROMETRE / 10);
			lEdgeSizeY = pEEPromData.AutoStage.Edge.y * (MICROMETRE / 10);
			/*ステージサイズとオフセットから、稼動範囲を計算*/
			lMachineOriginLeft = (lOffsetX * (-1));
			lMachineOriginDown = (lOffsetY * (-1));
			lMachineOriginRight = (lOffsetX * (-1)) + lStageSizeX;
			lMachineOriginUp = (lOffsetY * (-1)) + lStageSizeY;
			//Saiki 20091113 Add <-----
		}
/* modified hmenjo 2009.05.20 SPT ステージ用設定対応 -------- } -------- */

		// Stage Size X,Y /////
		((CEdit*)GetDlgItem(IDC_SAMPLE_SIZE_X))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_SAMPLE_SIZE_Y))->SetReadOnly();

/* modified 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- { ---------- */
//		// Stage Edge X,Y /////
//		((CEdit*)GetDlgItem(IDC_STAGE_EDGE_X))->SetReadOnly();
//		((CEdit*)GetDlgItem(IDC_STAGE_EDGE_Y))->SetReadOnly();
/* modified 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ----------			   */
		// Stage Edge X,Y /////
		//Saiki 20091210 Change ----->
		//if (STAGE_TYPE_SPT != l_SystemConfig.nStageType) {
		if (STAGE_TYPE_SPT != ml_SystemConfig.nStageType) {
		//Saiki 20091210 Change <-----
			((CEdit*)GetDlgItem(IDC_STAGE_EDGE_X))->SetReadOnly();
			((CEdit*)GetDlgItem(IDC_STAGE_EDGE_Y))->SetReadOnly();
		}
/* modified 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- } ---------- */

//Saiki 20091113 Delete <Stage Config修正により入力可能にする> ----->
		// Stage Offset X,Y /////
		//((CEdit*)GetDlgItem(IDC_ORIGIN_X))->SetReadOnly();
		//((CEdit*)GetDlgItem(IDC_ORIGIN_Y))->SetReadOnly();
//Saiki 20091113 Delete <-----

//Saiki 20091113 Change <Stage Config修正により装置稼動範囲を表示> ----->
		// Stage Max Coordnate X,Y /////
		//((CEdit*)GetDlgItem(IDC_COORD_LEFT_UP))->SetReadOnly();
		//((CEdit*)GetDlgItem(IDC_COORD_LEFT_DOWN))->SetReadOnly();
		//((CEdit*)GetDlgItem(IDC_COORD_RIGHT_UP))->SetReadOnly();
		//((CEdit*)GetDlgItem(IDC_COORD_RIGHT_DOWN))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_EDIT_MACHIN_ORIGIN_UP))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_EDIT_MACHIN_ORIGIN_LEFT))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_EDIT_MACHIN_ORIGIN_RIGHT))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_EDIT_MACHIN_ORIGIN_DOWN))->SetReadOnly();
//Saiki 20091113 Change <-----

/* deleted 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- { ---------- */
///* added hmenjo 2009.05.20 SPT ステージ用設定対応 -------- { -------- */
//		if (STAGE_TYPE_SPT == l_SystemConfig.nStageType) {
//			/* ステージタイプは，SPT */
//			GetDlgItem(IDC_STAGE_EDGE_X)->EnableWindow(FALSE);
//			GetDlgItem(IDC_STAGE_EDGE_Y)->EnableWindow(FALSE);
//			GetDlgItem(IDC_STATIC_EDGEGRP)->EnableWindow(FALSE);
//			GetDlgItem(IDC_STATIC_EDGE_X)->EnableWindow(FALSE);
//			GetDlgItem(IDC_STATIC_EDGE_Y)->EnableWindow(FALSE);
//			GetDlgItem(IDC_STATIC_EDGESIZE)->EnableWindow(FALSE);
//		}
///* added hmenjo 2009.05.20 SPT ステージ用設定対応 -------- } -------- */
/* deleted 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- } ---------- */

/* deleted hmenjo 2009.05.20 SPT ステージ用設定対応 -------- { -------- */
//		lStageSizeX = pEEPromData.AutoStage.Size.x;
//		lStageSizeY = pEEPromData.AutoStage.Size.y;
//		lEdgeSizeX =  pEEPromData.AutoStage.Edge.x;
//		lEdgeSizeY =  pEEPromData.AutoStage.Edge.y;
//		lOffsetX =	pEEPromData.AutoStage.Origin.lX;
//		lOffsetY =	pEEPromData.AutoStage.Origin.lY;
/* deleted hmenjo 2009.05.20 SPT ステージ用設定対応 -------- } -------- */
	}

/* modified hmenjo 2009.05.20 SPT ステージ用設定対応 -------- { -------- */
//	strBuffer.Format("%d", lStageSizeX * (MICROMETRE / 10));		// convert nm to um
//	SetDlgItemText(IDC_SAMPLE_SIZE_X, strBuffer);
//	strBuffer.Format("%d", lStageSizeY * (MICROMETRE / 10));		// convert nm to um
//	SetDlgItemText(IDC_SAMPLE_SIZE_Y, strBuffer);
//
//	strBuffer.Format("%d", lEdgeSizeX * (MICROMETRE / 10)); 	// convert nm to um
//	SetDlgItemText(IDC_STAGE_EDGE_X, strBuffer);
//	strBuffer.Format("%d", lEdgeSizeY * (MICROMETRE / 10)); 	// convert nm to um
//	SetDlgItemText(IDC_STAGE_EDGE_Y, strBuffer);
/* modified hmenjo 2009.05.20 SPT ステージ用設定対応 --------			 */
	strBuffer.Format("%d", lStageSizeX);	// um
	SetDlgItemText(IDC_SAMPLE_SIZE_X, strBuffer);
	strBuffer.Format("%d", lStageSizeY);	// um
	SetDlgItemText(IDC_SAMPLE_SIZE_Y, strBuffer);

//Saiki 20091113 Chage <Stage Offset, Edge Size, 稼動範囲はファイルから読み出す>----->
//	strBuffer.Format("%d", lEdgeSizeX); 	// um
//	SetDlgItemText(IDC_STAGE_EDGE_X, strBuffer);
//	strBuffer.Format("%d", lEdgeSizeY); 	// um
//	SetDlgItemText(IDC_STAGE_EDGE_Y, strBuffer);
///* modified hmenjo 2009.05.20 SPT ステージ用設定対応 -------- } -------- */
//
//	strBuffer.Format("%d", lOffsetX );		// um
//	SetDlgItemText(IDC_ORIGIN_X, strBuffer);
//	strBuffer.Format("%d", lOffsetY );		// um
//	SetDlgItemText(IDC_ORIGIN_Y, strBuffer);

	//Saiki 20091210 Change ----->
	//strBuffer.Format("%ld", lMachineOriginUp);
	//SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_UP, strBuffer);
	//strBuffer.Format("%ld", lMachineOriginDown);
	//SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_DOWN, strBuffer);
	//strBuffer.Format("%ld", lMachineOriginLeft);
	//SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_LEFT, strBuffer);
	//strBuffer.Format("%ld", lMachineOriginRight);
	//SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_RIGHT, strBuffer);

	//strBuffer.Format("%ld", m_StageConfig.Edge.dX);	   // um
	//SetDlgItemText(IDC_STAGE_EDGE_X, strBuffer);
	//strBuffer.Format("%ld", m_StageConfig.Edge.dY);	   // um
	//SetDlgItemText(IDC_STAGE_EDGE_Y, strBuffer);
//Saiki 20130809 Change ---->
//	if(g_bHWS && STAGE_TYPE_SPT != ml_SystemConfig.nStageType){
//		strBuffer.Format("%ld", lMachineOriginUp);
//		SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_UP, strBuffer);
//		strBuffer.Format("%ld", lMachineOriginDown);
//		SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_DOWN, strBuffer);
//		strBuffer.Format("%ld", lMachineOriginLeft);
//		SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_LEFT, strBuffer);
//		strBuffer.Format("%ld", lMachineOriginRight);
//		SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_RIGHT, strBuffer);
//	}
		strBuffer.Format("%ld", lMachineOriginUp);
		SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_UP, strBuffer);
		strBuffer.Format("%ld", lMachineOriginDown);
		SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_DOWN, strBuffer);
		strBuffer.Format("%ld", lMachineOriginLeft);
		SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_LEFT, strBuffer);
		strBuffer.Format("%ld", lMachineOriginRight);
		SetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_RIGHT, strBuffer);
//Saiki 20130809 Change <----

	strBuffer.Format("%.3lf", m_StageConfig.Edge.dX);	   // um
	SetDlgItemText(IDC_STAGE_EDGE_X, strBuffer);
	strBuffer.Format("%.3lf", m_StageConfig.Edge.dY);	   // um
	SetDlgItemText(IDC_STAGE_EDGE_Y, strBuffer);

//Saiki 20091210 Change <-----

	strBuffer.Format("%ld", m_StageConfig.Origin.lX );		 // um
	SetDlgItemText(IDC_ORIGIN_X, strBuffer);
	strBuffer.Format("%ld", m_StageConfig.Origin.lY );		 // um
	SetDlgItemText(IDC_ORIGIN_Y, strBuffer);

	strBuffer.Format("%lf", m_StageConfig.StageMaxCoord.dUpY ); 	 // um
	SetDlgItemText(IDC_COORD_UP, strBuffer);
	strBuffer.Format("%lf", m_StageConfig.StageMaxCoord.dDownY );		 // um
	SetDlgItemText(IDC_COORD_DOWN, strBuffer);
	strBuffer.Format("%lf", m_StageConfig.StageMaxCoord.dLeftX );		 // um
	SetDlgItemText(IDC_COORD_LEFT, strBuffer);
	strBuffer.Format("%lf", m_StageConfig.StageMaxCoord.dRightX );		 // um
	SetDlgItemText(IDC_COORD_RIGHT, strBuffer);


//Saiki 20091113 Chage <Stage Offset, Edge Size, 稼動範囲はファイルから読み出す><-----

	UpdateData(TRUE);
	SetCoordRange();
/* added 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- { ---------- */
	this->GetDlgItem(IDOK)->EnableWindow(TRUE);
/* added 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- } ---------- */

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CStageConfigSettingDlg::OnOK()
{
	///// Save /////
	if(!CheckData()) return;

	ConfigFile_SetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CStageConfigSettingDlg::OnTeachingButton()
{
	COverlayDialog dlg;
	//Saiki 20091120 Change ----->
	//dlg.DoModal();
	CString strBuff;
	CString l_strCap, l_strMsg;

	LoadStringML(IDS_STAGE_OFFSET_TEACHING_CAPTION, l_strCap, "STAGE OFFSET - TEACHING");
	LoadStringML(IDS_STAGE_OFFSET_TEACHING_MESSAGE, l_strMsg, "STAGE OFFSET - TEACHING");
	dlg.SetCaption(l_strCap);
	dlg.SetMessage(l_strMsg);


	if(dlg.DoModal() == IDOK){
// 2013.01.22 bagus stage origin offset -->
//		strBuff.Format("%ld", (m_StageConfig.Origin.lX - dlg.m_lX));
		strBuff.Format("%ld", (m_StageConfig.Origin.lX - dlg.m_lTeachPosX));
// 2013.01.22 bagus stage origin offset <--
		SetDlgItemText(IDC_ORIGIN_X, strBuff);

// 2013.01.22 bagus stage origin offset -->
//		strBuff.Format("%ld", (m_StageConfig.Origin.lY - dlg.m_lY));
		strBuff.Format("%ld", (m_StageConfig.Origin.lY - dlg.m_lTeachPosY));
// 2013.01.22 bagus stage origin offset <--
		SetDlgItemText(IDC_ORIGIN_Y, strBuff);
	}
}

// =========================================================================
//Saiki 20091113 Change ----->
//// OnStageOrgPosRadioButton ステージ原点のラジオボタンチェック 原点座標の目安値を設定する
// OnStageOrgPosRadioButton サンプル原点のラジオボタンチェック
void CStageConfigSettingDlg::OnStageOrgPosRadioButton(UINT nID)
{
// 2009.05.25 K.Matsuo StageConfigSetting SPT <--
//	if(IDC_MACHINE_ORG_POS_1 <= nID && nID <=IDC_MACHINE_ORG_POS_5){
//		m_StageConfig.MachineOrgSensorLoc = nID - IDC_MACHINE_ORG_POS_1 + 1;
//		return;
//	}
// 2009.05.25 K.Matsuo StageConfigSetting SPT -->

	if(IDC_ORG_POS_1 <= nID && nID <=IDC_ORG_POS_5){
		m_StageConfig.OrgPos = nID - IDC_ORG_POS_1 + 1;
//Saiki 20091113 Delete <サンプル原点は設定のみとする>----->
//		UpdateData(TRUE);
//		SetCoordRange();
//Saiki 20091113 Delete <サンプル原点は設定のみとする><-----
/* added 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- { ---------- */
		this->GetDlgItem(IDOK)->EnableWindow(TRUE);
/* added 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- } ---------- */
	}
}

// =========================================================================
// OnStageCoordDirComboBox ステージ座標の方向設定
void CStageConfigSettingDlg::OnStageCoordDirComboBox(UINT nID)
{
	CString strBuffer;
	for ( int i = 0; i < STAGE_COORD_DIR_COMBO_MAX; i++ ) {
		GetDlgItemText(StageCoordDir_ID[i], strBuffer);
		if ( strBuffer == StageCoordDir_Item[1] )
			m_StageConfig.Dir.X = StageCoordDir_ID[i] - IDC_DIR_1 + 1;
		if ( strBuffer == StageCoordDir_Item[2] )
			m_StageConfig.Dir.Y = StageCoordDir_ID[i] - IDC_DIR_1 + 1;
	}
}

// =========================================================================
//
void CStageConfigSettingDlg::InitRadio_StageOrgPos()
{
// 2009.05.25 K.Matsuo StageConfigSetting SPT -->

	// StageConfigSettingのm_StageConfig.Originの取扱いについて説明しておく
	//	 修正前：MachineOrgSensorLoc （ラジオボタンで左下などを指定）の基準位置からのオフセット値。基準位置により、オフセット値の加算・減算を行う
	//	 修正後：MachineOrgSensorLocは、廃止。左下基準（固定）とし、マシーンステージ原点ロケーションの座標位置として利用する

//	CheckDlgButton(StageMachineOriginPos_ID[m_StageConfig.MachineOrgSensorLoc - 1], BST_CHECKED);
	CheckDlgButton(IDC_MACHINE_ORG_POS_3, BST_CHECKED);

// 2009.05.25 K.Matsuo StageConfigSetting SPT <--

	//ステージ原点からサンプル原点に機能を変更
	CheckDlgButton(StageOriginPos_ID[m_StageConfig.OrgPos - 1], BST_CHECKED);
	UpdateData(TRUE);
}

// =========================================================================
//
void CStageConfigSettingDlg::InitCombo_StageCoord()
{
	CComboBox* pCombo;
	int nSelect;

	for ( int i = 0; i < STAGE_COORD_DIR_COMBO_MAX; i++ ) {
		pCombo = (CComboBox*)GetDlgItem(StageCoordDir_ID[i]);
		pCombo->ResetContent();
		for ( int j = 0; j < STAGE_COORD_DIR_ITEM_MAX; j++ ) {
			pCombo->AddString(StageCoordDir_Item[j]);
		}
	}

	pCombo = (CComboBox*)GetDlgItem(StageCoordDir_ID[m_StageConfig.Dir.X - 1]);
	nSelect = pCombo->FindStringExact(-1, StageCoordDir_Item[1]);
	pCombo->SetCurSel(nSelect);

	pCombo = (CComboBox*)GetDlgItem(StageCoordDir_ID[m_StageConfig.Dir.Y - 1]);
	nSelect = pCombo->FindStringExact(-1, StageCoordDir_Item[2]);
	pCombo->SetCurSel(nSelect);
}

// 2013.01.17 bagus rotate xy view -->
// =========================================================================
//
void CStageConfigSettingDlg::InitCombo_RotateViewCoord()
{
	CComboBox* pCombo;
	int nSelect;
	CString strItem;

	pCombo = (CComboBox*)GetDlgItem(IDC_ROTATE_VIEW_COMBO);
	pCombo->ResetContent();
	LoadStringML(IDS_ROTATE_VIEW_0, strItem, "0 degree");
	pCombo->AddString(strItem);
	LoadStringML(IDS_ROTATE_VIEW_90, strItem, "90 degree");
	pCombo->AddString(strItem);
	LoadStringML(IDS_ROTATE_VIEW_180, strItem, "180 degree");
	pCombo->AddString(strItem);
	LoadStringML(IDS_ROTATE_VIEW_270, strItem, "270 degree");
	pCombo->AddString(strItem);

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		nSelect = 0;
		break;
	case 90:
		nSelect = 1;
		break;
	case 180:
		nSelect = 2;
		break;
	case 270:
		nSelect = 3;
		break;
	}

	pCombo->SetCurSel(nSelect);
}
// 2013.01.17 bagus rotate xy view <--

// =========================================================================
//
BOOL CStageConfigSettingDlg::CheckData()
{
	if(UpdateData(TRUE) == FALSE) return FALSE;

	CString strBuffer;
	BOOL bDirectX = FALSE;
	BOOL bDirectY = FALSE;
	long lValue;
	//Saiki 20091113 Add ----->
	long lMachineOriginUp;		//(P509)
	long lMachineOriginDown;	//(P510)
	long lMachineOriginRight;	//(P511)
	long lMachineOriginLeft;	//(P512)
	//Saiki 20091113 Add <-----

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
	if ( (m_StageConfig.Dir.X + m_StageConfig.Dir.Y) % 2 == 0 ) {
		// Kojika 20090603 Change
		//MessageBox("Please Correct.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_CORRECT, l_strBuffer, "Please Correct.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		return FALSE;
	}

	///// Stage Size X /////
	if ( ( m_StageConfig.Size.dX <= SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < m_StageConfig.Size.dX	) ) {
		// Kojika 20090603 Change
		//MessageBox("Stage X Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_STAGE_X_SIZE_OUT_RANGE, l_strBuffer, "Stage X Size is out of Range.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		GetDlgItem(IDC_SAMPLE_SIZE_X)->SetFocus();
		return FALSE;
	}

	///// Stage Size Y /////
	if ( ( m_StageConfig.Size.dY <= SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < m_StageConfig.Size.dY	) ) {
		// Kojika 20090603 Change
		//MessageBox("Stage Y Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_STAGE_Y_SIZE_OUT_RANGE, l_strBuffer, "Stage Y Size is out of Range.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		GetDlgItem(IDC_SAMPLE_SIZE_Y)->SetFocus();
		return FALSE;
	}

	////// Machine Coordinate  Up(P509) /////
	////// Machine Coordinate  Down(P510) /////
	////// Machine Coordinate  Right(P511) /////
	////// Machine Coordinate  Left(P512) /////

	////// Stage Max Coord Up /////
	GetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_UP, strBuffer);
	lMachineOriginUp = atoi(strBuffer);
	lValue = static_cast<long>(m_StageConfig.StageMaxCoord.dUpY);
	//Saiki 20091210 Change ----->
	//if ( ( lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue  ) || lMachineOriginUp < lValue) {
	//	  // Kojika 20090603 Change
	//	  //MessageBox("Stage Max Coord Up Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
	//	  LoadStringML(IDS_COORD_UP_SIZE_OUT_RANGE, l_strBuffer, "Stage Max Coord Up Size is out of Range.");
	//	  LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
	//	  MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
	//	  // Kojika 20090603 Change End
	//	  GetDlgItem(IDC_COORD_UP)->SetFocus();
	//	  return FALSE;
	//}
	if(!(g_bHWS && STAGE_TYPE_SPT == ml_SystemConfig.nStageType)){
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ---------- { ---------- */
//		if ( ( lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue  ) || lMachineOriginUp < lValue) {
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ----------              */
		if ((lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10)) ||
			(SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue) ||
			(m_StageConfig.StageMaxCoord.dUpY < 0) ||
			((STAGE_TYPE_SPT == ml_SystemConfig.nStageType) && (lMachineOriginUp < lValue))
		) {
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ---------- } ---------- */
			// Kojika 20090603 Change
			//MessageBox("Stage Max Coord Up Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_COORD_UP_SIZE_OUT_RANGE, l_strBuffer, "Stage Max Coord Up Size is out of Range.");
			LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
			MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
			// Kojika 20090603 Change End
			GetDlgItem(IDC_COORD_UP)->SetFocus();
			return FALSE;
		}
	}
	//Saiki 20091210 Change <-----

	////// Stage Max Coord Down /////
	GetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_DOWN, strBuffer);
	lMachineOriginDown = atoi(strBuffer);
	lValue = static_cast<long>(m_StageConfig.StageMaxCoord.dDownY);
	//Saiki 20091210 Change ----->
	//if ( ( lValue > SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue  ) || lMachineOriginDown > lValue) {
	//	  // Kojika 20090603 Change
	//	  //MessageBox("Stage Max Coord Up Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
	//	  LoadStringML(IDS_COORD_DOWN_SIZE_OUT_RANGE, l_strBuffer, "Stage Max Coord Down Size is out of Range.");
	//	  LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
	//	  MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
	//	  // Kojika 20090603 Change End
	//	  GetDlgItem(IDC_COORD_DOWN)->SetFocus();
	//	  return FALSE;
	//}
	if(!(g_bHWS && STAGE_TYPE_SPT == ml_SystemConfig.nStageType)){
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ---------- { ---------- */
//		if ( ( lValue > SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue  ) || lMachineOriginDown > lValue) {
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ----------              */
		lMachineOriginDown = labs(lMachineOriginDown);
		lValue = labs(lValue);
		if ((lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10)) ||
			(SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue) ||
			(0 <= m_StageConfig.StageMaxCoord.dDownY) ||
			((STAGE_TYPE_SPT == ml_SystemConfig.nStageType) && (lMachineOriginDown < lValue))
		) {
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ---------- } ---------- */
			// Kojika 20090603 Change
			//MessageBox("Stage Max Coord Up Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_COORD_DOWN_SIZE_OUT_RANGE, l_strBuffer, "Stage Max Coord Down Size is out of Range.");
			LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
			MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
			// Kojika 20090603 Change End
			GetDlgItem(IDC_COORD_DOWN)->SetFocus();
			return FALSE;
		}
	}
	//Saiki 20091210 Change <-----

	////// Stage Max Coord Right /////
	GetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_RIGHT, strBuffer);
	lMachineOriginRight = atoi(strBuffer);
	lValue = static_cast<long>(m_StageConfig.StageMaxCoord.dRightX);
	//Saiki 20091210 Change ----->
	//if ( ( lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue  ) || lMachineOriginRight < lValue) {
	//	  // Kojika 20090603 Change
	//	  //MessageBox("Stage Max Coord Right Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
	//	  LoadStringML(IDS_COORD_RIGHT_SIZE_OUT_RANGE, l_strBuffer, "Stage Max Coord Right Size is out of Range.");
	//	  LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
	//	  MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
	//	  // Kojika 20090603 Change End
	//	  GetDlgItem(IDC_COORD_RIGHT)->SetFocus();
	//	  return FALSE;
	//}
	if(!(g_bHWS && STAGE_TYPE_SPT == ml_SystemConfig.nStageType)){
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ---------- { ---------- */
//		if ( ( lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue  ) || lMachineOriginRight < lValue) {
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ----------              */
		if ((lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10)) ||
			(SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue) ||
			(m_StageConfig.StageMaxCoord.dRightX < 0) ||
			((STAGE_TYPE_SPT == ml_SystemConfig.nStageType) && (lMachineOriginRight < lValue))
		) {
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ---------- } ---------- */
			// Kojika 20090603 Change
			//MessageBox("Stage Max Coord Right Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_COORD_RIGHT_SIZE_OUT_RANGE, l_strBuffer, "Stage Max Coord Right Size is out of Range.");
			LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
			MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
			// Kojika 20090603 Change End
			GetDlgItem(IDC_COORD_RIGHT)->SetFocus();
			return FALSE;
		}
	}
	//Saiki 20091210 Change <-----

	////// Stage Max Coord Left /////
	GetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_LEFT, strBuffer);
	lMachineOriginLeft = atoi(strBuffer);
	lValue = static_cast<long>(m_StageConfig.StageMaxCoord.dLeftX);
	//Saiki 20091210 Change ----->
	//if ( ( lValue > SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue  ) || lMachineOriginLeft > lValue) {
	//	  // Kojika 20090603 Change
	//	  //MessageBox("Stage Max Coord Left Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
	//	  LoadStringML(IDS_COORD_LEFT_SIZE_OUT_RANGE, l_strBuffer, "Stage Max Coord Left Size is out of Range.");
	//	  LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
	//	  MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
	//	  // Kojika 20090603 Change End
	//	  GetDlgItem(IDC_COORD_LEFT)->SetFocus();
	//	  return FALSE;
	//}
	if(!(g_bHWS && STAGE_TYPE_SPT == ml_SystemConfig.nStageType)){
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ---------- { ---------- */
//		if ( ( lValue > SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue  ) || lMachineOriginLeft > lValue) {
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ----------              */
		lMachineOriginLeft = labs(lMachineOriginLeft);
		lValue = labs(lValue);
		if ((lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10)) ||
			(SUBCON_STAGE_MAX_SIZE * (MICROMETRE / 10) < lValue) ||
			(0 <= m_StageConfig.StageMaxCoord.dLeftX) ||
			((STAGE_TYPE_SPT == ml_SystemConfig.nStageType) && (lMachineOriginLeft < lValue))
		) {
/* modified 2011.11.14 hmenjo [６]StageCfg画面 ---------- } ---------- */
			// Kojika 20090603 Change
			//MessageBox("Stage Max Coord Left Size is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_COORD_LEFT_SIZE_OUT_RANGE, l_strBuffer, "Stage Max Coord Left Size is out of Range.");
			LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
			MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
			// Kojika 20090603 Change End
			GetDlgItem(IDC_COORD_LEFT)->SetFocus();
			return FALSE;
		}
	}
	//Saiki 20091210 Change <-----

	///// Stage Edge X /////
	lValue = static_cast<long>(m_StageConfig.Edge.dX);
	//Saiki 20091113 Change ----->
	///*Edgeの範囲は稼動範囲より小さいこと*/
	//if ( ( lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( m_StageConfig.Size.dX < lValue ) ) {
	if ( ( lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( abs(m_StageConfig.StageMaxCoord.dLeftX) < lValue ) || ( abs(m_StageConfig.StageMaxCoord.dRightX) < lValue )) {
	//Saiki 20091113 Change <-----
		// Kojika 20090603 Change
		//MessageBox("Stage X Edge Coordinate is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_STAGE_X_EDGE_COORDINATE_OUT_RANGE, l_strBuffer, "Stage X Edge Coordinate is out of Range.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		GetDlgItem(IDC_STAGE_EDGE_X)->SetFocus();
		return FALSE;
	}

	///// Stage Edge Y /////
	lValue = static_cast<long>(m_StageConfig.Edge.dY);
	//Saiki 20091113 Change ----->
	///*Edgeの範囲は稼動範囲より小さいこと*/
	//if ( ( lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( m_StageConfig.Size.dY < lValue ) ) {
	if ( ( lValue < SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) ) || ( abs(m_StageConfig.StageMaxCoord.dUpY) < lValue ) || ( abs(m_StageConfig.StageMaxCoord.dDownY) < lValue )) {
	//Saiki 20091113 Change <-----
		// Kojika 20090603 Change
		//MessageBox("Stage Y Edge Coordinate is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_STAGE_Y_EDGE_COORDINATE_OUT_RANGE, l_strBuffer, "Stage Y Edge Coordinate is out of Range.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		GetDlgItem(IDC_STAGE_EDGE_Y)->SetFocus();
		return FALSE;
	}

	///// Stage Origin X /////
	lValue = m_StageConfig.Origin.lX;
	//Saiki 20091113 Change ---->
	///*ステージオフセットは、稼動範囲からEdge分内側内に存在しなければならない*/
	//if ( ( lValue < ( SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) + m_StageConfig.Edge.dX ) ) || ( (m_StageConfig.Size.dX - m_StageConfig.Edge.dX ) < lValue ) ) {
	if ( (m_StageConfig.StageMaxCoord.dLeftX + m_StageConfig.Edge.dX > lValue) || (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.Edge.dX < lValue)) {
		// Kojika 20090603 Change
		//MessageBox("Stage X Origin Coordinate is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_STAGE_X_ORIGIN_COORDINATE_OUT_RANGE, l_strBuffer, "Stage X Origin Coordinate is out of Range.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		GetDlgItem(IDC_ORIGIN_X)->SetFocus();
		return FALSE;
	}

	///// Stage Origin Y /////
	lValue = m_StageConfig.Origin.lY;
	///*ステージオフセットは、稼動範囲からEdge分内側内に存在しなければならない*/
//	  if ( ( lValue < ( SUBCON_STAGE_MIN_SIZE * (MICROMETRE / 10) + m_StageConfig.Edge.dY ) ) || ( (m_StageConfig.Size.dY - m_StageConfig.Edge.dY ) < lValue ) ) {
	if ( (m_StageConfig.StageMaxCoord.dDownY + m_StageConfig.Edge.dY > lValue) || (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.Edge.dY < lValue)) {
		// Kojika 20090603 Change
		//MessageBox("Stage Y Origin Coordinate is out of Range.", "STAGE CONFIG", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_STAGE_Y_ORIGIN_COORDINATE_OUT_RANGE, l_strBuffer, "Stage Y Origin Coordinate is out of Range.");
		LoadStringML(IDS_TITLE_STAGE_CONFIG, l_strTitle, "STAGE CONFIG");
		MessageBox(l_strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090603 Change End
		GetDlgItem(IDC_ORIGIN_Y)->SetFocus();
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
// SetCoordRange 設定可能是表範囲を設定
void CStageConfigSettingDlg::SetCoordRange()
{
	CString strBuffer1;
	CString strBuffer2;
	CString strBuffer3;
	CString strBuffer4;

//Saiki 20091113 Change ----->
//ステージは全て、センター中心で考えるため、原点位置に関係なく最大ストロークを入れる

//	switch ( m_StageConfig.OrgPos ) {
//	case 1: // 中心
//		m_StageConfig.StageMaxCoord.dLeftX = (-(m_StageConfig.Origin.lX) + m_StageConfig.Edge.dX);
//		m_StageConfig.StageMaxCoord.dRightX = (m_StageConfig.Size.dX - m_StageConfig.Origin.lX - m_StageConfig.Edge.dX );
//		m_StageConfig.StageMaxCoord.dUpY = (m_StageConfig.Size.dY - m_StageConfig.Origin.lY - m_StageConfig.Edge.dY);
//		m_StageConfig.StageMaxCoord.dDownY = (-(m_StageConfig.Origin.lY) + m_StageConfig.Edge.dY);
//
//		break;
//	case 2: // 左上
//	case 3: // 左下
//	case 4: // 右下
//	case 5: // 右上
//		break;
//	}
//	strBuffer1.Format("( %.0f , %.0f )", m_StageConfig.StageMaxCoord.dLeftX, m_StageConfig.StageMaxCoord.dUpY );
//	SetDlgItemText(IDC_COORD_LEFT_UP, strBuffer1);
//	strBuffer2.Format("( %.0f , %.0f )", m_StageConfig.StageMaxCoord.dLeftX, m_StageConfig.StageMaxCoord.dDownY );
//	SetDlgItemText(IDC_COORD_LEFT_DOWN, strBuffer2);
//	strBuffer3.Format("( %.0f , %.0f )", m_StageConfig.StageMaxCoord.dRightX, m_StageConfig.StageMaxCoord.dUpY );
//	SetDlgItemText(IDC_COORD_RIGHT_UP, strBuffer3);
//	strBuffer4.Format("( %.0f , %.0f )", m_StageConfig.StageMaxCoord.dRightX, m_StageConfig.StageMaxCoord.dDownY );
//	SetDlgItemText(IDC_COORD_RIGHT_DOWN, strBuffer4);

	strBuffer1.Format("%.0f", m_StageConfig.StageMaxCoord.dUpY );
	SetDlgItemText(IDC_COORD_UP, strBuffer1);
	strBuffer2.Format("%.0f", m_StageConfig.StageMaxCoord.dDownY );
	SetDlgItemText(IDC_COORD_DOWN, strBuffer2);
	strBuffer3.Format("%.0f", m_StageConfig.StageMaxCoord.dLeftX);
	SetDlgItemText(IDC_COORD_LEFT, strBuffer3);
	strBuffer4.Format("%.0f", m_StageConfig.StageMaxCoord.dRightX);
	SetDlgItemText(IDC_COORD_RIGHT, strBuffer4);

//Saiki 20091113 Change <-----
}

/* added 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- { ---------- */
void CStageConfigSettingDlg::OnChangeStageEdgeX()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
	//Saiki 20091210 Delete ----->
	/*ステージ座標の仕様変更により、Edgeを変更してもストローク値を消さない*/
	//this->SetDlgItemText(IDC_COORD_LEFT_UP,	  _T(""));
	//this->SetDlgItemText(IDC_COORD_LEFT_DOWN,   _T(""));
	//this->SetDlgItemText(IDC_COORD_RIGHT_UP,	  _T(""));
	//this->SetDlgItemText(IDC_COORD_RIGHT_DOWN,  _T(""));
	//this->GetDlgItem(IDOK)->EnableWindow(FALSE);
	//Saiki 20091210 Delete <-----
}
void CStageConfigSettingDlg::OnChangeStageEdgeY()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code h	 //Saiki 20091210 Delete ----->
	//this->SetDlgItemText(IDC_COORD_LEFT_UP,	  _T(""));
	//this->SetDlgItemText(IDC_COORD_LEFT_DOWN,   _T(""));
	//this->SetDlgItemText(IDC_COORD_RIGHT_UP,	  _T(""));
	//this->SetDlgItemText(IDC_COORD_RIGHT_DOWN,  _T(""));
	//this->GetDlgItem(IDOK)->EnableWindow(FALSE);
	//Saiki 20091210 Delete <-----
}
/* added 2009.07.16 hmenjo SPT ステージ EDGE 入力可能化 ---------- } ---------- */

void CStageConfigSettingDlg::OnSetMachineOriginButton()
{
	CString strBuff;

	GetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_UP, strBuff);
	SetDlgItemText(IDC_COORD_UP, strBuff);

	GetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_DOWN, strBuff);
	SetDlgItemText(IDC_COORD_DOWN, strBuff);

	GetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_LEFT, strBuff);
	SetDlgItemText(IDC_COORD_LEFT, strBuff);

	GetDlgItemText(IDC_EDIT_MACHIN_ORIGIN_RIGHT, strBuff);
	SetDlgItemText(IDC_COORD_RIGHT, strBuff);

}

void CStageConfigSettingDlg::OnSelchangeRotateViewCombo()
{
	CComboBox* pCombo;
	int iSel;

	pCombo = (CComboBox*)GetDlgItem(IDC_ROTATE_VIEW_COMBO);
	iSel = pCombo->GetCurSel();

	switch ( iSel ) {
	case 0:
	default:
		m_StageConfig.RotateXyView = 0;
		break;
	case 1:
		m_StageConfig.RotateXyView = 90;
		break;
	case 2:
		m_StageConfig.RotateXyView = 180;
		break;
	case 3:
		m_StageConfig.RotateXyView = 270;
		break;
	}
}
