/////////////////////////////////////////////////////////////////////////////
// MainFrm.cpp : CMainFrame �N���X�̓���̒�`���s���܂��B                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stageint.hxx>
#include <patrec.hxx>
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "NanoSpecView.h"
#include "RecipeMainListView.h"
#include "MainMenuView.h"
#include "ViewWindowView.h"
#include "MeasurementOperationView.h"
#include "DataListOperationView.h"
#include "UserSettingOperationTabView.h"
#include "MaintenanceOperationView.h"
#include "MeasurementProcessView.h"
#include "ManualMeasurementProcessView.h"
#include "ManualMeasurementFormView.h"
#include "TestMeasurementOperationView.h"
#include "RecipeOperationView.h"
#include "DataReviewView.h"
#include "DataReviewOperationView.h"
#include "MeasurementTabView.h"
#include "SystemOperationTabView.h"
#include "RecipeMultiListView.h"
#include "RecipeMeasurementProgramListView.h"
#include "RecipeRecalibrationProgramListView.h"
#include "RecipeStageProgramListView.h"
#include "DataListView.h"
#include "RecipePointDeskewProgramListView.h"
#include "UserSettingView.h"
#include "MaintenanceView.h"
#include "SystemView.h"
#include "RecipeMainSrView.h"
//2009.08.16 bagus stress --{--
#include "RecipeMainStressView.h"
#include "RecipeMeasurementStressProgramView.h"
#include "RecipeStressStageProgramView.h"
//2009.08.16 bagus stress --}--
//2009.09.03 bagus se --{--
#include "RecipeMeasurementSeThicknessProgramView.h"
//2009.09.03 bagus se --}--
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
#include "RecipeMeasurementCompEASEThicknessProgramView.h"
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.02 bagus Gantry --{--
#include "RecipeGantryStageProgramView.h"
// 2009.10.02 bagus Gantry --}--
// 2009.10.07 bagus CTA �ǉ� --{--
#include "RecipeMeasurementCTAProgramView.h"
// 2009.10.07 bagus CTA �ǉ� --}--
// 2009.10.14 bagus Distance �ǉ� --{--
#include "RecipeMeasurementSrDistanceProgramView.h"
#include "RecipeDistanceStageProgramView.h"
// 2009.10.14 bagus Distance �ǉ� --}--
//2009.10.20 bagus MS �ǉ� --{--
#include "RecipeMeasurementMSProgramView.h"
//2009.10.20 bagus MS �ǉ� --}--
#include "RecipeMultiView.h"
#include "RecipeStageProgramView.h"
#include "RecipeRecalibrationProgramView.h"
#include "RecipePointDeskewProgramView.h"
#include "RecipeMeasurementResistanceProgramView.h"
#include "RecipeMeasurementSrCieReflectanceProgramView.h"
#include "RecipemeasurementSrcieTransmittanceProgramView.h"
#include "RecipeMeasurementSrOdProgramView.h"
#include "RecipeMeasurementSrReflectanceProgramView.h"
#include "RecipeMeasurementSrTransmittanceProgramView.h"
#include "RecipeMeasurementSrThicknessProgramView.h"
#include "StatusView.h"
#include "AlarmHistoryListView.h"
#include "AlarmHistoryOperationView.h"
#include "UserSettingOperationSrSettingDlg.h"
#include "DebugDlg.h"
#include "Alarm.h"
#include "LogOnDlg.h"
#include "LogOffDlg.h"
#include "AlarmLogDlg.h"
#include "OverlayDialog.h"
#include "AutoFocusSettingDlg.h"
#include "ManualStageDlg.h"
#include "ManualDeskewDlg.h"
#include "ManualSitePtDlg.h"
#include "ChiefThread.h"
#include "ChiefExports.h"
#include "ExecutionDlg.h"
#include "MeasurementListDlg.h"
#include "MeasurementGraphDlg.h"
#include "TestMeasurementDlg.h"
// 2010.01.06 bagus Gantry --{--
#include "TestMeasurementGantryDlg.h"
// 2010.01.06 bagus Gantry --}--
#include "MaintenanceNoticeDlg.h"
#include "MessageDlg.h"
#include "InterlockDlg.h"
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
#include "ConfirmDlg.h"
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
#include "..\\..\\INC\\PifComm.hxx"
#include "..\\..\\INC\\AlarmIf.hxx"
#include "..\\..\\INC\\Title.hxx"
#include "..\\..\\INC\\NexIoBase.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "..\\..\\INC\\NSStage.hxx"
//2009.08.31 bagus stress --{--

#include "..\\..\\INC\\MeaSys.hxx"

//for DBL_MIN,DBL_MAX
#include <float.h>
//2009.08.31 bagus stress --}--
//2009.10.29 bagus 2point-distance --{--
#include "PointSetupMeasurement2PointDlg.h"
//2009.10.29 bagus 2point-distance --}--
//2009.11.03 bagus MS --{--
#include "PointSetupTeachingMSDlg.h"
//2009.11.03 bagus MS --}--
//Saiki 20091211 Add ----->
#include "FileRetentionPeriod.h"
//Saiki 20091211 Add <-----
/* added 2016.08.05 hmenjo ���V�s�ҏW menu �o�O ---------- { ---------- */
#include "NanoListUI.h"
/* added 2016.08.05 hmenjo ���V�s�ҏW menu �o�O ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Define																   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
#define STATUS_TIMER						1		// 0.1 sec timer
#define STATUS_TIME 						100
#define COUNTER_TIMER						2		// 0.1 sec timer
#define COUNTER_TIME						100
#define REFRESH_TIMER						3
#define REFRESH_TIME						100
#define MAINTENANCE_WATCH_TIMER 			4		// 0.1 sec timer
#define MAINTENANCE_WATCH_TIME				100
#define APPEXIT_WAIT_TIMER					5		// 15sec timer
#define APPEXIT_WAIT_TIME					15000


#define MAIN_WINDOW_ORIGIN_X				0
#define MAIN_WINDOW_ORIGIN_Y				0
#define MAIN_WINDOW_WIDTH					1278
#define MAIN_WINDOW_HEIGHT					988
#define MAIN_WINDOW_TASKBAR 				24

#define SPLIT_WIDTH 						200
#define SPLIT_MAINMENU_HIGHT				192
#define SPLIT_VIEWWINDOW_HEIGHT 			240
#define SPLIT_EXEWIN_HEIGHT 				828

#define ANY_MODE							(-1)
#define ANY_EXEWND							(-1)

// NanoPifSock
#define NANOPIFSOCK_EXE_NAME				_T("NanoPifSock.exe")

//MaintenanceMode
#define MAINFRM_MAINTE_MODE 				1
#define MAINFRM_NORMAL_MODE 				2
#define MAINFRM_QUIT_MODE					3

//Saiki 20091211 Add ----->
char* g_pszFileRetentionList[FILE_RETENTION_MAX] =
{
	g_szResult_Dir,
};
//Saiki 20091211 Add <-----
//Saiki 20100517 Add ----->
#define DIOLOG_EXE_NAME				_T("DioLog.exe")
//Saiki 20100517 Add <-----
CRecipeOperationView* pRecipeOperationView;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

//
// EVENTS
//
enum MEASUREMENT_MAIN_EVENTS
{
	EV_DO_INIT,
	EV_DO_SHUTDOWN,

	EV_DONE,					// fall-thru state
	EV_ABORT,					// failure, user selected abort
	EV_RETRY,					// ReTry
};

//
//	STATES
//
enum MEASUREMENT_MAIN_STATES
{
	ST_IDLE,
	ST_INIT,
	ST_SHUTDOWN,
	ST_DONE,
	ST_ABORT,
	ST_RETRY,
};

/////////////////////////////////////////////////////////////////////////////
// MEASUREMENT_MAIN TRANSITION											   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
const Transition MeasurementMainTransTable[] =
{
	// OldState,				EventId,			NewState

	// initialize
	{	ST_IDLE,				EV_DO_INIT, 				ST_INIT 			},
	{	ST_INIT,				EV_DONE,					ST_DONE 			},
	{	ST_INIT,				EV_ABORT,					ST_ABORT			},
	{	ST_INIT,				EV_RETRY,					ST_RETRY			},

	// termination of current request
	{	ST_DONE,				EV_DONE,					ST_IDLE 			},
	{	ST_ABORT,				EV_DONE,					ST_IDLE 			},
	{	ST_RETRY,				EV_DONE,					ST_IDLE 			},

	// shutdown
	{	ST_IDLE,				EV_DO_SHUTDOWN, 			ST_SHUTDOWN 		},
	{	ST_SHUTDOWN,			EV_DONE,					ST_DONE 			},

	// -----------------------------------------------------------------------
	// terminate the transition table
	// -----------------------------------------------------------------------
	END_OF_TRANSITION_TABLE
};

/////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_MEASUREMENT_START, OnMeasurementStart)
	ON_COMMAND(IDM_MEASUREMENT, OnMenuMeasurement)
	ON_COMMAND(IDM_RECIPE_SETTING, OnMenuEditRecipe)
	ON_COMMAND(IDM_DATA, OnMenuData)
	ON_COMMAND(IDM_USER_SETTING, OnMenuUserSetting)
	ON_COMMAND(IDM_MAINTENANCE, OnMenuMaintenance)
	ON_COMMAND(IDM_SYSTEM, OnMenuSystem)
	ON_COMMAND(IDM_LOCAL, OnMenuLocal)
	ON_COMMAND(IDM_REMOTE, OnMenuRemote)
	ON_COMMAND(IDM_EXIT, OnMenuExit)
	ON_UPDATE_COMMAND_UI(IDM_SAVE, OnUpdateMenuSave)
	ON_COMMAND(ID_AUTO_FOCUS, OnAutoFocus)
	ON_WM_CLOSE()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(IDM_USER_SETTING, OnUpdateMenuUserSetting)
	ON_UPDATE_COMMAND_UI(IDM_MAINTENANCE, OnUpdateMenuMaintenance)
	ON_COMMAND(IDM_LOGON, OnMenuLogon)
	ON_COMMAND(IDM_ALARM, OnMenuAlarm)
	ON_UPDATE_COMMAND_UI(IDM_DATA, OnUpdateMenuData)
	ON_UPDATE_COMMAND_UI(IDM_SYSTEM, OnUpdateSystem)
	ON_WM_COPYDATA()
	ON_UPDATE_COMMAND_UI(IDM_REMOTE, OnUpdateRemote)
	ON_UPDATE_COMMAND_UI(IDM_LOCAL, OnUpdateLocal)
	ON_WM_SHOWWINDOW()
	ON_UPDATE_COMMAND_UI(IDM_MEASUREMENT, OnUpdateMeasurement)
	ON_UPDATE_COMMAND_UI(IDM_RECIPE_SETTING, OnUpdateRecipeSetting)
	ON_UPDATE_COMMAND_UI(IDM_LOGON, OnUpdateLogon)
	ON_WM_SYSCOLORCHANGE()
	ON_UPDATE_COMMAND_UI(IDM_MEASUREMENT_START, OnUpdateMeasurementStart)
	ON_COMMAND(IDM_HANDLER_LOAD_POSITION, OnHandlerLoadPosition)
	ON_UPDATE_COMMAND_UI(IDM_HANDLER_LOAD_POSITION, OnUpdateHandlerLoadPosition)
	ON_COMMAND(IDM_MANUAL_STAGE, OnManualStage)
	ON_UPDATE_COMMAND_UI(IDM_MANUAL_STAGE, OnUpdateManualStage)
	ON_COMMAND(IDM_VACUUM, OnVacuum)
	ON_UPDATE_COMMAND_UI(IDM_VACUUM, OnUpdateVacuum)
	ON_COMMAND(IDM_REFERENCE, OnReference)
	ON_UPDATE_COMMAND_UI(IDM_REFERENCE, OnUpdateReference)
	ON_COMMAND(IDM_MEASUREMENT_CANCEL, OnMeasurementCancel)
	ON_UPDATE_COMMAND_UI(IDM_MEASUREMENT_CANCEL, OnUpdateMeasurementCancel)
	ON_COMMAND(IDM_MEASUREMENT_PAUSE, OnMeasurementPause)
	ON_UPDATE_COMMAND_UI(IDM_MEASUREMENT_PAUSE, OnUpdateMeasurementPause)
	ON_COMMAND(IDM_REMEASUREMENT, OnRemeasurement)
	ON_UPDATE_COMMAND_UI(IDM_REMEASUREMENT, OnUpdateRemeasurement)
	ON_COMMAND(IDM_MANUAL_MEASUREMENT_START, OnManualMeasurementStart)
	ON_UPDATE_COMMAND_UI(IDM_MANUAL_MEASUREMENT_START, OnUpdateManualMeasurementStart)
	ON_COMMAND(IDM_MEASUREMENT_COMPLETE, OnMeasurementComplete)
	ON_UPDATE_COMMAND_UI(IDM_MEASUREMENT_COMPLETE, OnUpdateMeasurementComplete)
	ON_COMMAND(IDM_POINT_TEACHING, OnPointTeaching)
	ON_UPDATE_COMMAND_UI(IDM_POINT_TEACHING, OnUpdatePointTeaching)
	ON_COMMAND(IDM_MEASUREMENT_RESUME, OnMeasurementResume)
	ON_UPDATE_COMMAND_UI(IDM_MEASUREMENT_RESUME, OnUpdateMeasurementResume)
	ON_COMMAND(IDM_MANUAL_MEASUREMENT, OnManualMeasurement)
	ON_UPDATE_COMMAND_UI(IDM_MANUAL_MEASUREMENT, OnUpdateManualMeasurement)
	ON_COMMAND(IDM_SEQMEAS_EXIT, OnSeqmeasExit)
	ON_UPDATE_COMMAND_UI(IDM_SEQMEAS_EXIT, OnUpdateSeqmeasExit)
	ON_COMMAND(IDM_MANUMEAS_EXIT, OnManumeasExit)
	ON_UPDATE_COMMAND_UI(IDM_MANUMEAS_EXIT, OnUpdateManumeasExit)
	ON_COMMAND(IDM_ADUJST_REMEASUREMENT, OnAdujstRemeasurement)
	ON_UPDATE_COMMAND_UI(IDM_ADUJST_REMEASUREMENT, OnUpdateAdujstRemeasurement)
	ON_COMMAND(IDM_MEASUREMENT_START, OnMeasurementStart)
	ON_UPDATE_COMMAND_UI(IDM_DEBUG, OnUpdateDebug)
/* added 2016.05.14 hmenjo �蓮 menu �o�O ---------- { ---------- */
	ON_COMMAND(IDM_SAVE_AS_RESULT_DATA, OnSaveAs)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_AS_RESULT_DATA, OnUpdateSaveAs)
/* added 2016.05.14 hmenjo �蓮 menu �o�O ---------- } ---------- */
/* added 2016.08.05 hmenjo ���V�s�ҏW menu �o�O ---------- { ---------- */
	ON_COMMAND(IDM_NEW, OnNew)
	ON_UPDATE_COMMAND_UI(IDM_NEW, OnUpdateNew)
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_UPDATE_COMMAND_UI(IDM_DELETE, OnUpdateDelete)
/* added 2016.08.05 hmenjo ���V�s�ҏW menu �o�O ---------- } ---------- */
/* added 2016.08.12 hmenjo ���V�s�ҏW menu �o�O2 ---------- { ---------- */
	ON_COMMAND(IDM_SAVE, OnRecipeSave)
	ON_COMMAND(IDM_SAVE_AS, OnRecipeSaveAs)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_AS, OnUpdateRecipeSaveAs)
/* added 2016.08.12 hmenjo ���V�s�ҏW menu �o�O2 ---------- } ---------- */
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_EXE_WND_EXIT, OnExeWndExit)
	ON_MESSAGE(WM_DO_MEASURE, OnDoMeasure)
	ON_MESSAGE(WM_DO_CANCEL, OnDoCancel)
	ON_MESSAGE(WM_DO_PAUSE, OnDoPause)
	ON_MESSAGE(WM_DO_RESUME, OnDoResume)
	ON_MESSAGE(WM_ALARMIF_SET, OnAlarmIf_Set)
	ON_MESSAGE(WM_ALARMIF_RESET, OnAlarmIf_Reset)
	ON_MESSAGE(WM_MEAS_COMP_EVENT, OnSetMeasurementData)
	ON_MESSAGE(WM_PROCESS_INIT, OnProcessInit)
	ON_MESSAGE(WM_UNIT_INIT_RETRY, OnUnitInitRetry)
	ON_MESSAGE(WM_EXE_DLG_DISPLAY, OnExeDlgDisplay)

	ON_MESSAGE(WM_CHIF_1POINT_END, OnScanDataStockProc)
//	ON_MESSAGE(WM_CHIF_MEAS_START, OnSeqStart)
	ON_MESSAGE(WM_CHIF_MEAS_END, OnOneSheetEnd)
	ON_MESSAGE(WM_CHIF_MOVE_SETPOS_END, OnMoveSetposEnd)
	ON_MESSAGE(WM_CHIF_VACUUM_END, OnVacuumEnd)
	ON_MESSAGE(WM_CHIF_AF_POPUP, OnAfPopup)
	ON_MESSAGE(WM_CHIF_SRREF_END, OnSrrefEnd)
	ON_MESSAGE(WM_DESKEWSEQ_DESKEWMANUAL_START, OnManualDeskewDlgPopup)
// 2009.05.16 myanagida -->
	ON_MESSAGE(WM_SITEPTSEQ_SITEPTMANUAL_START, OnManualSitePtDlgPopup)
// 2009.05.16 myanagida <--
/* added 2009.06.02 hmenjo �h�A�J�ő���|�[�Y ---------- { ---------- */
	ON_MESSAGE(WM_CHIF_NOTIFY_MEAS_PAUSE, OnNotifyMeasPause)
/* added 2009.06.02 hmenjo �h�A�J�ő���|�[�Y ---------- } ---------- */

	ON_MESSAGE(WM_MESSAGE_SHOW, OnMessageShow)

	ON_MESSAGE(WM_DEB_MESSAGE, OnDebMessageRecieve)

	//2009.07.27 bagus stress �X�g���X�Ή� --{--
	ON_MESSAGE(WM_CHIF_LINE_END, OnLineEnd)
	ON_MESSAGE(WM_CHIF_LINE_START,OnLineStart)
	ON_MESSAGE(WM_CHIF_STRS_MEAS_END,OnStrsMeasEnd)
	//2009.07.27 bagus stress �X�g���X�Ή� --}--
	//2009.10.28 bagus 2point-distance --{--
	ON_MESSAGE(WM_CHIF_DISTANCE_POPUP, OnDistancePopup)
	//2009.10.28 bagus 2point-distance --}--
	//2009.11.03 bagus MS --{--
	ON_MESSAGE(WM_CHIF_MS_POPUP, OnMSPopup)
	//2009.11.03 bagus MS --}--
	// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	ON_MESSAGE(WM_DISP_CONFIRM_POPUP, OnConfirmPopup)
	// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	// bagus 2014.01.10 Add(Stage None�Ή�) -->
	ON_MESSAGE(WM_PIF_REPORT_SI_REF_COMP, OnCompleteSiReferenceDoneReport)
	ON_MESSAGE(WM_PIF_REPORT_DARK_REF_COMP, OnCompleteDarkReferenceDoneReport)
	// bagus 2014.01.10 Add(Stage None�Ή�) <--
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//

static UINT Indicators[] =
{
	ID_SEPARATOR,						// �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_INDICATOR_VACUUM_SENSOR, 		// Vaccum Sensor
	ID_INDICATOR_SAMPLE_SENSOR, 		// Sample Sensor
	ID_INDICATOR_LOAD_POSITION_SENSOR,	// Load Position
};

// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
static UINT Indicators_T3100[] =
{
	ID_SEPARATOR,						// �X�e�[�^�X ���C�� �C���W�P�[�^
};
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

BEGIN_STATEPROC_MAP(CMainFrame)
// ========================================================================//
// MEASUREMENT_MAIN State-Processing-Table Definitions                     //
// ========================================================================//
	ENTER_STATE(ST_IDLE,						enter_ST_IDLE),
	ENTER_STATE(ST_INIT,						enter_ST_INIT),
	ENTER_STATE(ST_SHUTDOWN,					enter_ST_SHUTDOWN),
	ENTER_STATE(ST_DONE,						enter_ST_DONE),
	ENTER_STATE(ST_ABORT,						enter_ST_ABORT),
	ENTER_STATE(ST_RETRY,						enter_ST_RETRY),

END_STATEPROC_MAP(CMainFrame)

/////////////////////////////////////////////////////////////////////////////
//
CMainFrame::CMainFrame() : cActive(MeasurementMainTransTable)
{
	m_pApp					= NULL;
	m_pDoc					= NULL;

	m_iCurrMode 			= OP_MEASUREMENT;
	m_iCurrExeWnd			= RECIPE_MAIN_LIST_WND;

	m_iSelectHeadType		= HEAD_TYPE_SR;
	m_iSelectMeasType		= MEAS_PROG_TYPE_SR_THICKNESS;
	memset(m_szSelectListName, 0, sizeof(m_szSelectListName));
	memset(m_szSampleTypeName, 0, sizeof(m_szSampleTypeName));
	m_bTransmittance		= FALSE;
	m_iSplit_OpePanelHeight = 527;
	m_bAutoFocusInProgress	= FALSE;
	m_bDefaultSetting		= FALSE;

/* modified 2015.12.03 hmenjo �J�����r�f�I�t�H�[�}�b�g�ݒ艻 ---------- { ---------- */
//	OverlayInitInstance();
/* modified 2015.12.03 hmenjo �J�����r�f�I�t�H�[�}�b�g�ݒ艻 ----------              */
	OverlayInitInstance(g_szVideoFmtSize);
/* modified 2015.12.03 hmenjo �J�����r�f�I�t�H�[�}�b�g�ݒ艻 ---------- } ---------- */
	m_pCurrExeView			= NULL;
	m_pAlarm				= NULL;
	m_pcChiefThread 		= NULL;
	m_bVacuumSensor 		= OFF;
	m_bVacuumValve			= OFF;
	m_bSampleSensor 		= OFF;
	m_Old_bVacuumSensor 	= OFF;
	m_Old_bVacuumValve		= OFF;
	m_Old_bSampleSensor 	= OFF;

	m_bInitRetry			= FALSE;

	m_bGetColorInf			= GetOrginalSystemColor();		//�N�����̃V�X�e���J���[����荞��

	m_pTestMeasDlgObj = NULL;
// 2010.01.06 bagus Gantry --{--
	m_pTestMeasGantryDlgObj = NULL;
// 2010.01.06 bagus Gantry --}--

	m_hEvTabView = ::CreateEvent(0, TRUE, FALSE, 0);
	m_pMntNoticeDlg = NULL;
	m_bOrgSystemColorChangeExecFlg = TRUE;

	m_DbgCount = 0;
/* added 2009.06.02 hmenjo �h�A�J�ő���|�[�Y ---------- { ---------- */
	m_bClosedPauseMsgBox = FALSE;
/* added 2009.06.02 hmenjo �h�A�J�ő���|�[�Y ---------- } ---------- */

// 2009.11.25 K.Matsuo GTr Reference -->
	m_bGTRref = FALSE;
	m_iGTRrefNowPointNo = 0;
// 2009.11.25 K.Matsuo GTr Reference <--

// 2009.12.04 K.Matsuo OnCreate����ړ� -->
	m_nCameraSelect = 1;							// �J�����I��
	m_nJoyStickMode = 0;							// �W���C�X�e�B�N�I��
// 2009.12.04 K.Matsuo OnCreate����ړ� <--
}

/////////////////////////////////////////////////////////////////////////////
//
CMainFrame::~CMainFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message){
	case WM_KEYDOWN:
		switch(pMsg->wParam){
		case VK_ESCAPE:
			PostMessage(WM_EXE_WND_EXIT, 0, 0);
			break;
		}
		break;
/* added 2015.03.18 hmenjo FWXGA �Ή� ---------- { ---------- */
	case WM_MOUSEWHEEL:
		if (g_lModelType == MODEL_T3100) {
			BOOL l_bFound = FALSE;
			HWND l_hwnd = ::GetParent(pMsg->hwnd);
			while ((l_hwnd != 0) && (l_hwnd != this->m_hWnd)) {
				if (l_hwnd == m_wndSplitter1.m_hWnd) {
					l_bFound = TRUE;
					break;
				} else {
					l_hwnd = ::GetParent(l_hwnd);
				}
			}
			if (FALSE != l_bFound) {
				return TRUE;
			}
		}
		break;
/* added 2015.03.18 hmenjo FWXGA �Ή� ---------- { ---------- */
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
//
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//Kojika 20090526 Add
	CString strBuffer;
//Kojika 20090526 Add End

	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pApp = (CNanoSpecApp*)AfxGetApp();
	m_pDoc = (CNanoSpecDoc*)GetActiveDocument();
/* added 2014.11.21 hmenjo 3100 �A�C�R��(MAIN) ---------- { ---------- */
	if (g_lModelType == MODEL_T3100) {
#if 0
		/* ���\�[�X�ւ̃A�C�R���̓o�^���K�v�ł��D	*/
		HICON l_hIcon = 0;
		l_hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_3100));
		if (0 != l_hIcon) {
			this->SetIcon(l_hIcon, TRUE);			/* �傫���A�C�R����ݒ�	*/
			/* �������A�C�R���́C�V�X�e���������ŒT���Ă���܂��D	*/
		}
#else
		/* �A�C�R�����t�@�C������Ǎ��݂܂��D	*/
		CString l_strPathIcon = g_szBin_Dir;
		l_strPathIcon += _T("3100.ico");
		HICON l_hIcon32 = 0;
		HICON l_hIcon16 = 0;
		l_hIcon32 = (HICON) ::LoadImage(
								NULL,
								l_strPathIcon,
								IMAGE_ICON,
								32, 32,
								LR_LOADFROMFILE
							);
		l_hIcon16 = (HICON) ::LoadImage(
								NULL,
								l_strPathIcon,
								IMAGE_ICON,
								16, 16,
								LR_LOADFROMFILE
							);
		if ((0 != l_hIcon32) && (0 != l_hIcon16)) {
			this->SetIcon(l_hIcon32, TRUE);			/* �傫���A�C�R����ݒ�	*/
			this->SetIcon(l_hIcon16, FALSE);		/* �������A�C�R����ݒ�	*/
		}
#endif
	}
/* added 2014.11.21 hmenjo 3100 �A�C�R��(MAIN) ---------- } ---------- */

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND);
	}

	///// Status Bar /////
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
	SYSTEM_CONFIG systemConfig;

	ConfigFile_GetNanoSpecIni(&systemConfig, CONFIG_FILE_SYSTEM_CONFIG);
//	if(g_lModelType == MODEL_T3100){
	if(systemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if(!m_wndStatusBar.Create(this) ||
			!m_wndStatusBar.SetIndicators(Indicators_T3100,
				sizeof(Indicators_T3100)/sizeof(UINT))){
			TRACE0("Failed to create status bar\n");
			return -1;		// �쐬�Ɏ��s
		}
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
		if(!m_wndStatusBar.Create(this) ||
			!m_wndStatusBar.SetIndicators(Indicators,
				sizeof(Indicators)/sizeof(UINT))){
			TRACE0("Failed to create status bar\n");
			return -1;		// �쐬�Ɏ��s
		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	if(systemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		m_wndStatusBar.Initialize(ID_INDICATOR_VACUUM_SENSOR, DARK_GRAY_COLOR, BLACK_COLOR);
		m_wndStatusBar.Initialize(ID_INDICATOR_SAMPLE_SENSOR, DARK_GRAY_COLOR, BLACK_COLOR);
		m_wndStatusBar.Initialize(ID_INDICATOR_LOAD_POSITION_SENSOR, DARK_GRAY_COLOR, BLACK_COLOR);
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

	///// Init Finished Flag /////
	m_pDoc->SetInitFinished(FALSE);

	///// Alarm Thread /////
	m_pAlarm = (CAlarm*)AfxBeginThread(
								RUNTIME_CLASS(CAlarm),
								THREAD_PRIORITY_NORMAL,
								0,
								CREATE_SUSPENDED,
								NULL);

	// �I�����ɏI���R�[�h���m�F����̂Ŏ����I�ɍ폜���Ȃ��悤�ɂ���
	m_pAlarm->m_bAutoDelete = FALSE;

	m_pAlarm->SetMainFrame(this);
	m_pAlarm->SetNanoSpecDoc(m_pDoc);
	CStatusView* pView = (CStatusView*)m_wndSplitter2.GetPane(1, 0);
	m_pAlarm->SetStatusView(pView);

	m_pAlarm->ResumeThread();

	///// Alarm IF Initialize /////
	AlarmIf_Init(m_hWnd);

	//Saiki 20091211 Add ----->
	///// File Retention Period Thread /////
	//Saiki 20100305 Change ----->
	/*�����t�H���_��Result�݂̂Ȃ̂łP�����X���b�h�𗧂Ă�*/
//	  for(int i = 0 ; i < FILE_RETENTION_MAX ; i++){
//		  g_pFileRetentionPeriod[i] = NULL;
//		  if(g_pszFileRetentionList[i] != NULL){
//			  g_pFileRetentionPeriod[i] = (CFileRetentionPeriod*)AfxBeginThread(
//										  RUNTIME_CLASS(CFileRetentionPeriod),
//										  THREAD_PRIORITY_NORMAL,
//										  0,
//										  CREATE_SUSPENDED,
//										  NULL);
//			  g_pFileRetentionPeriod[i]->SetPath(g_pszFileRetentionList[i]);
//			  g_pFileRetentionPeriod[i]->ResumeThread();
//		  }
//	  }
	g_pFileRetentionPeriod[0] = NULL;
	if(g_pszFileRetentionList[0] != NULL){
		g_pFileRetentionPeriod[0] = (CFileRetentionPeriod*)AfxBeginThread(
										RUNTIME_CLASS(CFileRetentionPeriod),
										THREAD_PRIORITY_NORMAL,
										0,
										CREATE_SUSPENDED,
										NULL);
		g_pFileRetentionPeriod[0]->SetPath(g_pszFileRetentionList[0]);
		/*�X���b�h�̎��s��Initial������ɍs�Ȃ�*/
	}

	//Saiki 20100305 Change <-----
	//Saiki 20091211 Add <-----


	///// Load File /////
// 2009.05.15 k-matsuo -->
	ConfigFile_GetNanoSpecIni(&m_srConfig, CONFIG_FILE_SR_CONFIG);
	if ( m_srConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
		m_iMaxWaveLength = MAX_TARGET_WAVELENGTH;
	else
		m_iMaxWaveLength = MAX_TARGET_WAVELENGTH_800;
// 2009.05.15 k-matsuo <--

	ConfigFile_GetNanoSpecIni(&m_pMaintenanceDate, CONFIG_FILE_MAINTENANCE_DATE);
	m_pDoc->SetMaintenanceDate( m_pMaintenanceDate );
	mNextDate.ParseDateTime(m_pMaintenanceDate.NextMaintenanceDate, LOCALE_NOUSEROVERRIDE);

	ConfigFile_GetNanoSpecIni(&m_pMaintenanceLamp, CONFIG_FILE_MAINTENANCE_LAMP);
	m_pDoc->SetMaintenanceLamp( m_pMaintenanceLamp );
	m_pPassageHour.m_span = m_pMaintenanceLamp.MaintenanceHour;
//	m_pDoc->SetPassageHour(m_pPassageHour.m_span);
	COleDateTimeSpan m_1s(0,0,0,1);
	m_100ms.m_span = m_1s.m_span / 10;

	///// MessageDlg�𐶐����� /////
	g_pMessageDlg = new CMessageDlg();
	g_pMessageDlg->Create(CMessageDlg::IDD);

	///// Timer /////
	SetTimer(STATUS_TIMER, STATUS_TIME, NULL);
	SetTimer(COUNTER_TIMER, COUNTER_TIME, NULL);	// 0.1 sec timer
	SetTimer(REFRESH_TIMER, REFRESH_TIME, NULL);
	SetTimer(MAINTENANCE_WATCH_TIMER, MAINTENANCE_WATCH_TIME, NULL);

	m_MeasMenuEnableFlg = TRUE; //������
	m_MeasMenuEnableFlg2 = TRUE; //������

	m_iNowPoint = 0;
	//2009.08.21 bagus strss --{--
	m_iNowStressLine = 0;
	//2009.08.21 bagus strss --}--

	m_nMotionStage	= STAGE_MOTION_HIGH;			// XY�̐ݒ�l
	m_nMotionElev	= STAGE_MOTION_HIGH;			// Z�̐ݒ�l

	TestModeTempRcpDelete();
	//Saiki 20100305 Delete ----->
	/*�폜�X���b�h�ƃo�b�e�B���O���āA�N�����x���Ȃ�(�\��������)����*/
	//OldMeasDataDelete(); //�Â�����f�[�^�t�@�C�����폜
	//Saiki 20100305 Delete <-----
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 5);
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_MESSAGE2, TITLE_MSG2_LOAD_DATA);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	if(bShow == FALSE) return;

	if(m_pDoc->GetInitFinished()) return;

	PostMessage(WM_PROCESS_INIT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	int SysX=0;
	int SysY=0;

//	SysX	= GetSystemMetrics(SM_CXFULLSCREEN);
//	SysY	= GetSystemMetrics(SM_CYFULLSCREEN);
	SysX	= GetSystemMetrics(SM_CXSCREEN);
	SysY	= GetSystemMetrics(SM_CYSCREEN);
	cs.cx	= MAIN_WINDOW_WIDTH;
	cs.cy	= MAIN_WINDOW_HEIGHT;
	cs.x	= (SysX - cs.cx) / 2;
	cs.y	= ((SysY - MAIN_WINDOW_TASKBAR) - cs.cy) / 2;
	int iX, iY;
	iX = cs.x;
	iY = cs.y;
	cs.style &= ~(FWS_PREFIXTITLE | FWS_ADDTOTITLE);
//	cs.style |= WS_MAXIMIZE;

	// �^�C�g���́u����v���폜
//	cs.style ^= FWS_ADDTOTITLE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// OnSysCommand
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	CFrameWnd::OnSysCommand(nID, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���b�Z�[�W �n���h��

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
//Kojika 20090526 Add
	CString strBuffer;
//Kojika 20090526 Add End

	///// (1) �e 1�s 2�� �ɕ��� /////
	if ( !m_wndSplitter.CreateStatic(this, 1, 2) )
		return FALSE;

	///// (2) �e��0�s�ڂ� 3�s 1�� �ɕ��� /////
	// m_wndSplitter(�e)��0�s�ڂ�m_wndSplitter1(�q1)�𓖂Ă͂߂āA3�s1��ɂ���
	// �s��̔ԍ��́A0����J�n
/* modified 2015.03.18 hmenjo FWXGA �Ή� ---------- { ---------- */
//	if ( !m_wndSplitter1.CreateStatic(&m_wndSplitter, 3, 1, WS_CHILD | WS_VISIBLE | WS_BORDER,	m_wndSplitter.IdFromRowCol(0, 0)) ) {
/* modified 2015.03.18 hmenjo FWXGA �Ή� ----------              */
	CRect rcDesktop;
	this->GetDesktopWindow()->GetClientRect(rcDesktop);
	DWORD l_dwWnd1Styl;
	if ((g_lModelType == MODEL_T3100) && (rcDesktop.bottom < 1024)) {
		/* ���L�X�N���[���o�[�ɂ��邽�߂� WS_VSCROLL ��ǉ�	*/
		l_dwWnd1Styl = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL;
	} else {
		l_dwWnd1Styl = WS_CHILD | WS_VISIBLE | WS_BORDER;
	}
	if ( !m_wndSplitter1.CreateStatic(&m_wndSplitter, 3, 1, l_dwWnd1Styl, m_wndSplitter.IdFromRowCol(0, 0)) ) {
/* modified 2015.03.18 hmenjo FWXGA �Ή� ---------- } ---------- */
		TRACE("m_wndSplitter1 createstatic failed\n");
		return FALSE;
	}

	///// (3) �e��1�s�ڂ� 2�s 1�� �ɕ��� /////
	// m_wndSplitter(�e)��1�s�ڂ�m_wndSplitter2(�q2)�𓖂Ă͂߂āA2�s1��ɂ���
	// �s��̔ԍ��́A0����J�n
#ifndef SPLTR_SHRSCRL	/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
	if(!m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE | WS_BORDER, m_wndSplitter.IdFromRowCol(0, 1)) ) {
#else					/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ----------              */
	DWORD l_dwWnd2Styl;
	if ((g_lModelType == MODEL_T3100) && (rcDesktop.bottom < 1024)) {
		/* ���L�X�N���[���o�[�ɂ��邽�߂� WS_VSCROLL ��ǉ�	*/
		l_dwWnd2Styl = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL;
	} else {
		l_dwWnd2Styl = WS_CHILD | WS_VISIBLE | WS_BORDER;
	}
	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, l_dwWnd2Styl, m_wndSplitter.IdFromRowCol(0, 1))) {
#endif					/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */
/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
//		TRACE("m_wndSplitter1 createstatic failed\n");
/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ----------              */
		TRACE("m_wndSplitter2 createstatic failed\n");
/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */
		return FALSE;
	}

/* added 2015.03.18 hmenjo FWXGA �Ή� ---------- { ---------- */
	if ((g_lModelType == MODEL_T3100) && (rcDesktop.bottom < 1024)) {
		/* ���L�X�N���[���o�[��\�����Ȃ�	*/
		m_wndSplitter1.SetScrollStyle(0);
#ifdef SPLTR_SHRSCRL	/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
		m_wndSplitter2.SetScrollStyle(0);
#endif					/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */
	}
/* added 2015.03.18 hmenjo FWXGA �Ή� ---------- } ---------- */
	RECT rect;
	GetClientRect(&rect);
	m_iSplit_OpePanelHeight = rect.bottom - SPLIT_VIEWWINDOW_HEIGHT - SPLIT_MAINMENU_HIGHT;

	// �匳�̃X�v���b�g�E�B���h�E�̃T�C�Y�̎w��(0, 0)
	m_wndSplitter.SetColumnInfo(0, SPLIT_WIDTH, 50);
	//m_wndSplitter.SetColumnInfo(0, SPLIT_WIDTH, 100);

	///// (Pane1) m_wndSplitter1(�q1)��0�s��,0��� /////
	if(!m_wndSplitter1.CreateView(0, 0, RUNTIME_CLASS(CMainMenuView), CSize(SPLIT_WIDTH, SPLIT_MAINMENU_HIGHT), pContext) ) {
		TRACE("Mode view create failed\n");
		return FALSE;
	}

	///// (Pane2) m_wndSplitter1(�q1)��1�s��,0��� /////
	if(!m_wndSplitter1.CreateView(1, 0, RUNTIME_CLASS(CMeasurementOperationView), CSize(SPLIT_WIDTH, m_iSplit_OpePanelHeight), pContext) ) {
		TRACE("Mode view create failed\n");
		return FALSE;
	}

	///// (Pane3) m_wndSplitter1(�q1)��2�s��,0��� /////
	if(!m_wndSplitter1.CreateView(2, 0, RUNTIME_CLASS(CViewWindowView), CSize(SPLIT_WIDTH, SPLIT_VIEWWINDOW_HEIGHT), pContext) ) {
		TRACE("Mode view create failed\n");
		return FALSE;
	}

	///// (Pane4) m_wndSplitter2(�q2)��0�s��,0��� /////
/* modified 2015.03.18 hmenjo FWXGA �Ή� ---------- { ---------- */
//	CRect rcDesktop;
//	GetDesktopWindow()->GetClientRect(rcDesktop);
//	if ( rcDesktop == CRect(0, 0, 1024, 768) ) {
//		if(!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CRecipeMainListView), CSize(0, 600), pContext) ) {
/* modified 2015.03.18 hmenjo FWXGA �Ή� ----------              */
	if (rcDesktop.bottom < 1024) {
		/* SXGA �̏c�������̏ꍇ	*/
		if(!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CRecipeMainListView), CSize(0, rcDesktop.bottom - (768 - 600)), pContext) ) {
/* modified 2015.03.18 hmenjo FWXGA �Ή� ---------- } ---------- */
			TRACE("Mode view create failed\n");
			return FALSE;
		}
	}
	else {
		if(!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CRecipeMainListView), CSize(0, SPLIT_EXEWIN_HEIGHT), pContext) ) {
			TRACE("Mode view create failed\n");
			return FALSE;
		}
	}

	m_pCurrExeView = (CView *)m_wndSplitter2.GetPane(0, 0);

// 2013.11.11 Bagus Add (TohoSpec�Ή�) -->
	SetWindowTitle();
// 2013.11.11 Bagus Add (TohoSpec�Ή�) <--

	///// (Pane5) m_wndSplitter2(�q2)��1�s��,0��� /////
	if(!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CStatusView), CSize(0, 0), pContext) ) {
		TRACE("Mode view create failed\n");
		return FALSE;
	}

	SetActiveView((CView *)m_wndSplitter1.GetPane(1, 0));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnClose()
{
//Kojika 20090526 Add
	CString strBuffer;
//Kojika 20090526 Add End

	DWORD dwStatus = 0;

	ProcessQuit();

// �j�����Y��
	if( m_pMntNoticeDlg != NULL ){
		delete m_pMntNoticeDlg;
		m_pMntNoticeDlg = NULL;
	}
// �j�����Y��

	///// Maintenance Lamp Save /////
	m_pMaintenanceLamp.MaintenanceHour = m_pPassageHour.m_span;
	ConfigFile_SetNanoSpecIni(&m_pMaintenanceLamp, CONFIG_FILE_MAINTENANCE_LAMP);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

	///// Chief Thread /////
	if(m_pcChiefThread != NULL){
// hmenjo ChiefLog -->
		m_pDoc->ChiefLogging(_T("Deleting ChiefThread... (CMainFrame)"));
// hmenjo ChiefLog <--
		HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);

		if(0 != l_hChiefDlg){
			::PostMessage(l_hChiefDlg, WM_CLOSE, 0, 0);
		}
		delete m_pcChiefThread;
		m_pcChiefThread = NULL;
// hmenjo ChiefLog -->
		m_pDoc->ChiefLogging(_T("Deleted  ChiefThread. (CMainFrame)"));
// hmenjo ChiefLog <--

		MEAS_Shutdown();

	}
    //Saiki 20100517 Add ----->
    if(g_pFileRetentionPeriod[0] != NULL){
// 2013.11.13 Bagus Mod (Bug Fix) -->
//        delete g_pFileRetentionPeriod[0];
//        g_pFileRetentionPeriod[0] = NULL;
		GetExitCodeThread(g_pFileRetentionPeriod[0]->m_hThread, &dwStatus);
		if(dwStatus != STILL_ACTIVE){
			delete g_pFileRetentionPeriod[0];
			g_pFileRetentionPeriod[0] = NULL;
		}
// 2013.11.13 Bagus Mod (Bug Fix) <--
    }
    //Saiki 20100517 Add <-----

	///// Alarm Thread /////
	if(m_pAlarm != NULL){
		GetExitCodeThread(m_pAlarm->m_hThread, &dwStatus);
		if(dwStatus != STILL_ACTIVE){
			delete m_pAlarm;
			m_pAlarm = NULL;
		}
	}

	if( m_pAlarm == NULL &&
		m_pcChiefThread == NULL
// 2013.11.13 Bagus Add (Bug Fix) -->
		&& g_pFileRetentionPeriod[0] == NULL
// 2013.11.13 Bagus Add (Bug Fix) <--
	){
		CFrameWnd::OnClose();
	}
	else{
		Sleep(100);
		PostMessage(WM_CLOSE, 0, 0);
	}

//	CFrameWnd::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnAppExit()
{
//Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
//Kojika 20090526 Add End

	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	if(m_pMntNoticeDlg != NULL){
		m_pMntNoticeDlg->ChangeTopMost(FALSE);
	}
//Kojika 20090526 Change
	//if(IDOK == MessageBox(	"Are you sure you want to quit NanoSpec ?",
	//						"QUIT",
	//if(IDOK == MessageBox(  "NanoSpec���I�����Ă���낵���ł����H",
	//						  "�I��",
		LoadStringML(IDS_QUIT_NANOSPEC , strBuffer1, "Are you sure you want to quit NanoSpec ?");
		LoadStringML(IDS_TITLE_QUIT , strBuffer2, "QUIT");
	if(IDOK == MessageBox(	strBuffer1,
							strBuffer2,
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//							  MB_ICONQUESTION|MB_OKCANCEL)
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
							MB_ICONQUESTION | MB_OKCANCEL | MB_DEFBUTTON2)
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
	){
// 2013.11.15 Bagus Add (TohoSpec�Ή�) -->
//		m_pDoc->OperationLogging("NanoSpec Quit - OK Button was pushed.");
		char szBuff[_MAX_PATH];
		strcpy(szBuff, "NanoSpec Quit - OK Button was pushed.");
		if(g_lAppNameType != APP_NAME_NANO){
			CString strMsg;
			strMsg = szBuff;
			strMsg.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
			strcpy(szBuff, strMsg);
		}
		m_pDoc->OperationLogging(szBuff);
// 2013.11.15 Bagus Add (TohoSpec�Ή�) <--
//Kojika 20090526 Change End


#if 0 //NanoSpec�ȊO�̐F��ς��Ă��܂��̂Ŕp�~
	ChangeMaintenanceModeColor(MAINFRM_QUIT_MODE); //
#endif
		if(m_pDoc->GetProcessStatus() == PROCESS_PROC){
			AfxGetMainWnd()->PostMessage(WM_DO_CANCEL, 0, 0); //���蒆�~
			SetTimer(APPEXIT_WAIT_TIMER, APPEXIT_WAIT_TIME, NULL);
			CExecutionDlg dlg;
//Kojika 20090526
//			dlg.SetCaption(IDS_EXECUTION_EXIT_CAPTION);
//			dlg.SetMessage(IDS_EXECUTION_EXIT_TEXT);
			LoadStringML(IDS_EXECUTION_EXIT_CAPTION , strBuffer1, "Quit Process");
			LoadStringML(IDS_NANOSPEC_QUIT_PROCESSING , strBuffer2, "NanoSpec Quit Processing");
			dlg.SetCaption(strBuffer1);
			dlg.SetMessage(strBuffer2);
//Kojika 20090526
			dlg.DoModal(); //CMainFrame::OnTimer()���ŏ���
		}
		else{
			ProcessQuit();
		}
	}

	if(m_pMntNoticeDlg != NULL){
		m_pMntNoticeDlg->ChangeTopMost(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnTimer(UINT nIDEvent)
{
	// Kojika 20090528 Add
	CString l_strBuffer;
	// Kojika 20090528 Add End

	switch(nIDEvent){
	case STATUS_TIMER:
		KillTimer(STATUS_TIMER);
		WatchDoc();
		SetTimer(STATUS_TIMER, STATUS_TIME, NULL);
		break;
	case COUNTER_TIMER:
		KillTimer(COUNTER_TIMER);
		MaintenanceCheck();
		SetTimer(COUNTER_TIMER, COUNTER_TIME, NULL);
		break;
	case REFRESH_TIMER:
		KillTimer(REFRESH_TIMER);
		RefreshDlg();
		SetTimer(REFRESH_TIMER, REFRESH_TIME, NULL);
		break;
	case MAINTENANCE_WATCH_TIMER:
		KillTimer(MAINTENANCE_WATCH_TIMER);
		MaintenanceDlgOpen();
		SetTimer(MAINTENANCE_WATCH_TIMER, MAINTENANCE_WATCH_TIME, NULL);
		break;
	case APPEXIT_WAIT_TIMER:
		KillTimer(APPEXIT_WAIT_TIMER);
		HWND hWnd;
		// Kojika 20090528 Change
		//hWnd = ::FindWindow(NULL, IDS_EXECUTION_EXIT_CAPTION);
		LoadStringML(IDS_EXECUTION_EXIT_CAPTION , l_strBuffer, "QUIT PROCESS");
		hWnd = ::FindWindow(NULL, l_strBuffer);
		// Kojika 20090528 Change End

		if(hWnd != NULL)
		{
			::/*PostMessage*/SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		}
		ProcessQuit();
	default:
		break;
	}

	CFrameWnd::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::ProcessQuit()
{
	static BOOL Done = FALSE;

	if(Done == TRUE) return;

	KillTimer(STATUS_TIMER);
	KillTimer(COUNTER_TIMER);
	KillTimer(REFRESH_TIMER);
	KillTimer(MAINTENANCE_WATCH_TIMER);

	if(m_pAlarm){m_pAlarm->Quit();}
//Saiki 20100517 Add ----->
    if(g_pFileRetentionPeriod[0]){g_pFileRetentionPeriod[0]->Quit();}
//Saiki 20100517 Add <-----
	//Saiki 20090411 Add ------>
	////// Quit NanoMap /////
//	  if(g_ProcessInfo.hProcess != NULL){
//		  TerminateProcess(g_ProcessInfo.hProcess,0);
//		  ::CloseHandle(g_ProcessInfo.hThread);
//		  ::CloseHandle(g_ProcessInfo.hProcess);
//	  }
		// �R�[���o�b�N�֐��̌Ăяo���B
		EnumWindows(QuitExternalApp, (LPARAM)&g_ProcessInfo);

		// Nanomap���I������܂őҋ@���A����5�b�҂��Ă��I���Ȃ����͋����I��������B
		if(::WaitForSingleObject(g_ProcessInfo.hProcess, 5000) == WAIT_TIMEOUT)
		{
				TerminateProcess(g_ProcessInfo.hProcess, 0);
		}

		::CloseHandle(g_ProcessInfo.hThread);
		::CloseHandle(g_ProcessInfo.hProcess);
		//Saiki 20090411 Add <------
/* deleted 2010.10.19 hmenjo DioLog.exe �N�����Ȃ����� ---------- { ---------- */
//        //Saiki 20100517 Add ----->
//        // Quit DioLog.exe
//        EnumWindows(QuitExternalApp, (LPARAM)&m_prInfo);
//
//	    // Nanomap���I������܂őҋ@���A����5�b�҂��Ă��I���Ȃ����͋����I��������B
//	    if(::WaitForSingleObject(m_prInfo.hProcess, 5000) == WAIT_TIMEOUT)
//	    {
//			    TerminateProcess(m_prInfo.hProcess, 0);
//	    }
//
//	    ::CloseHandle(m_prInfo.hThread);
//	    ::CloseHandle(m_prInfo.hProcess);
//
//        //Saiki 20100517 Add <-----
/* deleted 2010.10.19 hmenjo DioLog.exe �N�����Ȃ����� ---------- } ---------- */

	///// Quit Nextra.exe /////
	m_pApp->NextraExe(NEXTRA_EXE_QUIT);

	///// Alarm Log Dlg /////
	g_pAlarmLogDlg->DestroyWindow();
	if(g_pAlarmLogDlg)		delete g_pAlarmLogDlg;

	///// Interlock Dlg /////
	g_pInterlockDlg->DestroyWindow();
	if( g_pInterlockDlg )	delete g_pInterlockDlg;

	///// Debug Dlg /////
	if(m_pApp->m_pDebugDlg != NULL){
		delete m_pApp->m_pDebugDlg;
		m_pApp->m_pDebugDlg = NULL;
	}

	///// Message Dlg /////
	g_pMessageDlg->DestroyWindow();
	if(g_pMessageDlg)	   delete g_pMessageDlg;

	///// Shut Down /////
	Event(EV_DO_SHUTDOWN);

	Done = TRUE;

	PostMessage(WM_CLOSE, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Initialize Event                                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnProcessInit(WPARAM wparam, LPARAM lparam)
{
//Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
//Kojika 20090526 Add End

	HWND hWnd;
	BOOL bRet;
	char szPath[MAX_PATH];
// 2009.11.09 K.Matsuo Resist Interlock -->
	BOOL bMeasInit = FALSE;
// 2009.11.09 K.Matsuo Resist Interlock <--

	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 10);

	//----- NEXIO Initialize ------------------------------------------------
	nexioInitialize(this->m_hWnd);

	//----- Pif Initialize --------------------------------------------------
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.wShowWindow = SW_MINIMIZE;
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process.
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
//	sprintf(szPath, "%s%s", g_szBin_Dir, NANOPIFSOCK_EXE_NAME);
	if(g_lModelType != MODEL_T3100){
		strBuffer1 = NANOPIFSOCK_EXE_NAME;
		if(g_lAppNameType != APP_NAME_NANO){
			strBuffer1.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
		}
		sprintf(szPath, "%s%s", g_szBin_Dir, (LPCTSTR)strBuffer1);
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--
		if(!CreateProcess(NULL, szPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//			m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526
//			MessageBox("NanoPifSock CreateProcess failed.", "NanoSpec", MB_OK);
//			MessageBox("NanoPifSock CreateProcess ���s���܂���.", "NanoSpec", MB_OK);
			LoadStringML(IDS_NANOPIFSOCK_CREATE_FAILED , strBuffer1, "NanoPifSock CreateProcess failed.");
			LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
			MessageBox(strBuffer1, strBuffer2, MB_OK);
//Kojika 20090526
		}
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--

	// Close process and thread handles.
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	PifComm_Init();

	PifComm_GetEqMonitorPtr()->iGlassExist = nexioIsGlassExist();
	PifComm_GetEqMonitorPtr()->iLoadPosition = nexioIsStageLoadPos();
	PifComm_GetEqMonitorPtr()->iVaccumOn1 = nexioIsVacuumOn();
	PifComm_GetEqMonitorPtr()->iShutterOpen = nexioIsShutterOpen();
	PifComm_GetEqMonitorPtr()->iShutterClose = nexioIsShutterClose();
	PifComm_GetEqMonitorPtr()->iPinUp = nexioIsPinUpperPos();
	PifComm_GetEqMonitorPtr()->iPinDown = nexioIsPinDownPos();
	PifComm_GetEqMonitorPtr()->iArmSensor = nexioIsRobotArmDetect();
	PifComm_GetEqMonitorPtr()->iDoorInterlock = nexioIsDoorInterlock();
	PifComm_GetEqMonitorPtr()->iVaccumOn2 = nexioIsVacuumOn();

	m_pMainFrmMailRecvThread = (CMainFrmMailRecvThread*)AfxBeginThread(RUNTIME_CLASS(CMainFrmMailRecvThread),
																	THREAD_PRIORITY_NORMAL,
																	0,
																	CREATE_SUSPENDED,
																	NULL);
	m_pMainFrmMailRecvThread->SetMainFrame(this);
	m_pMainFrmMailRecvThread->SetNanoSpecDoc(m_pDoc);
	m_pMainFrmMailRecvThread->SetInterlockDlg(g_pInterlockDlg);
	m_pMainFrmMailRecvThread->ResumeThread();

//	//----- Chief Initialize ------------------------------------------------
//	m_pcChiefThread = new CChiefThread((CWnd*) this);
//	if(0 == m_pcChiefThread->m_hThread){
//		// �X���b�h�̐������s
//		m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
//		::MessageBox(0, _T("Failed to start Chief Thread."), _T("NanoSpec MainFrame"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
//	}

	//----- Pattern Rec Initialze -------------------------------------------
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	if(( m_SystemConfig.nPRMethod >0 && m_SystemConfig.bSamplePatternRec ) ||
		(m_SystemConfig.nPRMethod >0 && m_SystemConfig.bSitePatternRec)){
		if(!PatRecInit()){
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//			m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526
//			MessageBox("PatternRec Initialize Error", "NanoSpec", MB_OK);
//			MessageBox("�p�^�[���F���̏������Ɏ��s���܂���", "NanoSpec", MB_OK);
			LoadStringML(IDS_PATREC_INIT_ERROR , strBuffer1, "PatternRec Initialize Error");
			LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
			MessageBox(strBuffer1, strBuffer2, MB_OK);
//Kojika 20090526
		}
	}

	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 20);
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_MESSAGE2, TITLE_MSG2_INIT_MEAS_HEAD);

	// ----- Head Initialize ------------------------------------------------
/* added 2013.06.04 hmenjo SubCon PwrOFF ---------- { ---------- */
	if (0 != g_lSBCPwr_OffTime) {
		/* �T�u�R���d���ē���	*/
		nexioSubConPowerOFF(TRUE);		/* �T�u�R���d�� �I�t	*/
		::Sleep(g_lSBCPwr_OffTime * 1000);			/* �I�t����	*/
		nexioSubConPowerOFF(FALSE);		/* �T�u�R���d�� �I��	*/
		::Sleep(g_lSBCPwr_WaitAfterOn * 1000);		/* �I����N���҂�����	*/
	}
/* added 2013.06.04 hmenjo SubCon PwrOFF ---------- } ---------- */
// 2009.11.09 K.Matsuo Resist Interlock -->
//	if(!MEAS_Initialize(g_bHWS))){
	if((bMeasInit = MEAS_Initialize(g_bHWS)) == FALSE){
// 2009.11.09 K.Matsuo Resist Interlock <--
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//		m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526
//		MessageBox("Measys Initialize Error", "NanoSpec", MB_OK | MB_ICONSTOP);
//		MessageBox("Measys�̏������Ɏ��s���܂���", "NanoSpec", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_MEASYS_INIT_ERROR , strBuffer1, "Measys Initialize Error");
		LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONSTOP);
//Kojika 20090526
	}

	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 30);

	//----- Chief Initialize ------------------------------------------------
// hmenjo ChiefLog -->
	m_pDoc->ChiefLogging(_T("Requested to start ChiefThread. (CMainFrame)"));
// hmenjo ChiefLog <--
	m_pcChiefThread = new CChiefThread((CWnd*) this);
	if(0 == m_pcChiefThread->m_hThread){
		// �X���b�h�̐������s
// hmenjo ChiefLog -->
		m_pDoc->ChiefLogging(_T("Failed to start ChiefThread. (CMainFrame)"));
// hmenjo ChiefLog <--
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//		m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526
//		::MessageBox(0, _T("Failed to start Chief Thread."), _T("NanoSpec MainFrame"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
//		::MessageBox(0, _T("Chief Thread�̃X�^�[�g�Ɏ��s���܂���."), _T("NanoSpec MainFrame"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
		LoadStringML(IDS_FAILED_START_CHIEF_THREAD , strBuffer1, "Failed to start Chief Thread.");
		LoadStringML(IDS_TITLE_NANO_MAINFRM , strBuffer2, "NanoSpec MainFrame");
		::MessageBox(0, strBuffer1, strBuffer2, MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
//Kojika 20090526
	}

	ShowWindow(SW_SHOWMAXIMIZED);

	//----- Unit  Initialze -------------------------------------------------
//	Event(EV_DO_INIT);
// 2009.11.09 K.Matsuo Resist Interlock -->
// MeaSys�̒��ŁA���胆�j�b�g�iCTA�⃍���X�^�j�̃C���^�[���b�N�֌W���݂Ă���B
// �������G���[�̏ꍇ�A�댯�ȏ�ԂȂ̂ŁA���������̃X�e�[�W�ړ��́A�X�L�b�v����悤�ɂ����B
	if ( bMeasInit == TRUE ) {
	Event(EV_DO_INIT);
	}
// 2009.11.09 K.Matsuo Resist Interlock <--


	//----- Screen	Initialze -----------------------------------------------
	///// Screen Initialize /////
	// ADAP�ɂ���ĕύX���ꂽ�V�X�e���J���[��߂� => OnSysColorChange()
//	if(m_bGetColorInf) BOOL bRet = SetOrginalSystemColor();

//	//----- Hide Nextra.exe -------------------------------------------------
//	m_pApp->NextraExe(NEXTRA_EXE_HIDE);

	//----- Hide Adap.exe ---------------------------------------------------
	hWnd = ::FindWindow(NULL, ADAP_WINDOW_NAME);
	if(hWnd != NULL){
		bRet = ::ShowWindow(hWnd, SW_HIDE);
	}

	//----- Quit NanoSpec Title.exe -----------------------------------------
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_CLOSE);

/* deleted 2010.10.19 hmenjo DioLog.exe �N�����Ȃ����� ---------- { ---------- */
//    //Saiki 20100517 add ----->
//    //----- Start DioLog.exe ------------------------------------------------
//	STARTUPINFO StartInfo;
//	PROCESS_INFORMATION ProcessInfo;
//	ZeroMemory(&StartInfo,sizeof(StartInfo));
//	ZeroMemory(&ProcessInfo,sizeof(ProcessInfo));
//
//    sprintf(szPath, "%s%s", g_szBin_Dir, DIOLOG_EXE_NAME);
//	if(!CreateProcess(NULL, szPath, NULL, NULL, FALSE, 0, NULL, NULL, &StartInfo, &ProcessInfo)){
//		LoadStringML(IDS_NANOPIFSOCK_CREATE_FAILED , strBuffer1, "DioLog CreateProcess failed.");
//		LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
//		MessageBox(strBuffer1, strBuffer2, MB_OK);
//	}
//    else{
//        m_prInfo = ProcessInfo;
//    }
//    //Saiki 20100517 add <-----
/* deleted 2010.10.19 hmenjo DioLog.exe �N�����Ȃ����� ---------- } ---------- */

	//Saiki 20100305 Add ----->
	//----- File Retention Period Thread Start ------------------------------
	g_pFileRetentionPeriod[0]->ResumeThread();
	//Saiki 20100305 Add <-----

	//----- Init Finished Flag ----------------------------------------------
	m_pDoc->SetInitFinished(TRUE);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnUnitInitRetry(WPARAM wparam, LPARAM lparam)
{
	Event(EV_DO_INIT);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Transition                                                              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// ==========================================================================
// MEASUREMENT_MAIN Definitions
void CMainFrame::enter_ST_IDLE(const cEventParams* const)
{
	m_pDoc->SetProcessStatus(PROCESS_WAIT);

	///// Unit Retry /////
	if(m_bInitRetry){
		m_bInitRetry = FALSE;
		PostMessage(WM_UNIT_INIT_RETRY, 0, 0);
	}
}

// ==========================================================================
//
void CMainFrame::enter_ST_INIT(const cEventParams* const)
{
//Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
//Kojika 20090526 Add End
	m_pDoc->SetProcessStatus(PROCESS_INIT);
	TRACE(_T("CMainFrame::enter_ST_INIT(const cEventParams* const)\n"));
	BOOL bNextraInit = FALSE;
	BOOL bStageInit = FALSE;
	SYSTEM_CONFIG systemConfig;

	ConfigFile_GetNanoSpecIni(&systemConfig, CONFIG_FILE_SYSTEM_CONFIG);

/* added 2009.11.20 hmenjo GTR �����v�������� Close ---------- { ---------- */
	/* GTR �����v�V���b�^ �N���[�Y	*/
	m_pDoc->SetTransmittanceLamp(FILTER_DARK);
/* added 2009.11.20 hmenjo GTR �����v�������� Close ---------- } ---------- */

	if(g_bHWS){
		m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 100);
		Event(EV_DONE);
		return;
	}

	///// Show Title /////
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_SHOW);

	///// Send to Alarm Thread /////
	m_pAlarm->AlarmAllReset();

	// ----- Check Equipment Power ON ---------------------------------------
	if(nexioIsEquipmentPower() == OFF){
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//		m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526 Change
		//if(MessageBox(	"EQ Power is OFF.\nDo you want to retry Unit Initialization?",
		//if(MessageBox(  "EQ�d����OFF�ł�.\n���j�b�g�̏��������Ď��s���܂����H",
		//				  "NanoSpec",
		LoadStringML(IDS_EQ_POWER_OFF , strBuffer1, "EQ Power is OFF.\nDo you want to retry Unit Initialization?");
		LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
		if(MessageBox(	strBuffer1, strBuffer2, MB_OKCANCEL | MB_ICONQUESTION) == IDOK){
//Kojika 20090526 Change
			Event(EV_RETRY);
			return;
		}
		else{
			Event(EV_ABORT);
			return;
		}
	}

/* added 2009.06.02 hmenjo Stage �������O�� RobotArm ---------- { ---------- */
	// ----- ���{�b�g�A�[�����o ---------------------------------------------
	if (0 == nexioIsRobotArmDetect()) {
		// �A�[���������Ă��܂��D
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//		m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE); 	// �^�C�g���_�C�A���O�������܂��D
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
		LoadStringML(IDS_ROBOT_ARM, strBuffer1, _T("Robot Arms in stage area.\nReturn Robot Arms."));
		LoadStringML(IDS_TITLE_NANOSPEC, strBuffer2, "NanoSpec");
		if (IDOK == MessageBox(
							strBuffer1,
							strBuffer2,
							MB_OKCANCEL | MB_ICONQUESTION)
						){
			Event(EV_RETRY);
			return;
		} else {
			Event(EV_ABORT);
			return;
		}
	}
/* added 2009.06.02 hmenjo Stage �������O�� RobotArm ---------- } ---------- */

	// ----- Check Door Open ------------------------------------------------
	if(!(nexioIsMaintenanceSwitch() == OFF && nexioIsEngineerMaintenanceSwitch() == ON)){
/* modified hmenjo 2009.05.20 �h�A�C���^���b�N�������Ή� -------- { -------- */
//		if(nexioIsDoorInterlock() == OFF){ */
/* modified hmenjo 2009.05.20 �h�A�C���^���b�N�������Ή� --------			 */
		if(m_pDoc->Rap_IsDoorInterlock() == OFF){
/* modified hmenjo 2009.05.20 �h�A�C���^���b�N�������Ή� -------- } -------- */
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//			m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526 Change
			//if(MessageBox(	"Door Open.\nDo you want to retry Unit Initialization?",
			//if(MessageBox(  "�h�A���J���Ă܂�\n���j�b�g�̏��������Ď��s���܂����H",
			//				  "NanoSpec",
			LoadStringML(IDS_DOOR_OPEN , strBuffer1, "Door Open.\nDo you want to retry Unit Initialization?");
			LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
			if(MessageBox(	strBuffer1,
							strBuffer2,
//Kojika 20090526 Change
							MB_OKCANCEL | MB_ICONQUESTION) == IDOK){
				Event(EV_RETRY);
				return;
			}
			else{
				Event(EV_ABORT);
				return;
			}
		}
	}

// matsuhisa 20091209 deleted >>>
//	  // ----- Check Shutter Open ------------------------------------------------
//	  if(!(nexioIsMaintenanceSwitch() == OFF && nexioIsEngineerMaintenanceSwitch() == ON)){
//		  if(nexioIsShutterClose() == OFF){
///* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
////		  m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
///* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
////Kojika 20090526 Change
//			  //if(MessageBox(	  "Shutter Open.\nDo you want to retry Unit Initialization?",
//			  //if(MessageBox(	"�V���b�^�[���J���Ă܂�\n���j�b�g�̏��������Ď��s���܂����H",
//			  //				"NanoSpec",
//			  LoadStringML(IDS_SHUTTER_OPEN , strBuffer1, "Shutter Open.\nDo you want to retry Unit Initialization?");
//			  LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
//			  if(MessageBox(  strBuffer1,
//							  strBuffer2,
////Kojika 20090526 Change
//							  MB_OKCANCEL | MB_ICONQUESTION) == IDOK){
//				  Event(EV_RETRY);
//				  return;
//			  }
//			  else{
//				  Event(EV_ABORT);
//				  return;
//			  }
//		  }
//	  }
// matsuhisa 20091209 deleted <<<

	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 40);
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_MESSAGE2, TITLE_MSG2_INIT_ALIGNMENT);

	// ----- NEXTRA - Alignment Initialize ----------------------------------
//	if (systemConfig.bSampleAligner == TRUE){
//		nexioEquipmentStatusRun(TRUE);		// EQ ON
//		if(NEXIF_OK == nexifInitialize(this->m_hWnd)){
//			bNextraInit = TRUE;
//		}
//		nexioEquipmentStatusRun(FALSE); 	// EQ OFF
//	}
	if(systemConfig.bDisableNextra){
		bNextraInit = TRUE;
	}
	else{
		if (systemConfig.bSampleAligner == TRUE){
			nexioEquipmentStatusRun(TRUE);		// EQ ON
			if(NEXIF_OK == nexifInitialize(this->m_hWnd)){
				bNextraInit = TRUE;
			}
			nexioEquipmentStatusRun(FALSE); 	// EQ OFF
		}
	}

	// ----- Confirm to be continue -----------------------------------------
	if(bNextraInit == FALSE){
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//		m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526 Change
		//if(MessageBox(	"Alignment Unit Initialization was failed.\nDo you want to execute Stage Initialization?",
		//if(MessageBox(  "�A���C�����g���j�b�g�̏������Ɏ��s���܂���\n�X�e�[�W�̏����������s���܂����H",
		//				  "NanoSpec",
		LoadStringML(IDS_ALIGNMENT_INIT_FAILED , strBuffer1, "Alignment Unit Initialization was failed.\nDo you want to execute Stage Initialization?");
		LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
		if(MessageBox(	strBuffer1,
						strBuffer2,
//Kojika 20090526 Change
						MB_OKCANCEL | MB_ICONQUESTION) == IDOK){
			bNextraInit = TRUE;
		}
	}

	// ----- Check Alignment Pin Down ---------------------------------------
	if (systemConfig.bSampleAligner == TRUE){
		if(bNextraInit == TRUE){
			if(nexioIsPinDownPos() == OFF){
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//				m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526 Change
				//if(MessageBox(	"Alignment Pin is not Lower Position.\nDo you want to retry Unit Initialization?",
				//if(MessageBox(  "�A���C�����g�̃s�������[�ʒu�ɂ���܂���\n���j�b�g�̏��������Ď��s���܂����H",
				//				  "NanoSpec",
				LoadStringML(IDS_ALIGNMENT_PIN_NOT_LOWER_POS , strBuffer1, "Alignment Pin is not Lower Position.\nDo you want to retry Unit Initialization?");
				LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
				if(MessageBox(	strBuffer1,
								strBuffer2,
//Kojika 20090526 Change
								MB_OKCANCEL | MB_ICONQUESTION) == IDOK){
					Event(EV_RETRY);
					return;
				}
				else{
					Event(EV_ABORT);
					return;
				}
			}
		}
	}

	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 50);
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_MESSAGE2, TITLE_MSG2_INIT_STAGE);

	// ----- Stage Initialize -----------------------------------------------
// added hmenjo 2009.05.15 �V Stage.dll �Ή� ---------- { ----------
	// �X�e�[�W��ʂ�ݒ�
	int l_iStageType;
	switch (systemConfig.nStageType) {
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	case STAGE_TYPE_NONE:
		l_iStageType = STAGETYPE_NONE;
		break;
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	case STAGE_TYPE_SPT:
		l_iStageType = STAGETYPE_SPT;
		break;
	case STAGE_TYPE_TOHO:
	case STAGE_TYPE_NTN:
	default:
		l_iStageType = STAGETYPE_STD;
		break;
	}
	int l_iBackStageType = StageSelector(l_iStageType);
// added hmenjo 2009.05.15 �V Stage.dll �Ή� ---------- } ----------
	if(bNextraInit == TRUE){
#if 0	// ���������͑��x��������
		if (0 != nexioIsMaintenanceSwitch()) {
			// �����e�i���X���[�h�łȂ�����
			// ���x�������Ȃ�
			StageDisableSpeedLimit();
		} else {
			// �����e�i���X���[�h������
			// ���x��������
			StageEnableSpeedLimit();
		}
#else	// ���������͑��x�����͂���
// modified hmenjo 2009.05.12 ���x�ύX���t���O�ǉ� ---------- { ----------
//		StageEnableSpeedLimit();
// modified hmenjo 2009.05.12 ���x�ύX���t���O�ǉ� ----------
		if (TRUE == g_bIL_SpeedDown) {
			StageEnableSpeedLimit();
		}
// modified hmenjo 2009.05.12 ���x�ύX���t���O�ǉ� ---------- } ----------
#endif	// ���������͑��x�����͂���
		nexioEquipmentStatusRun(TRUE);		// EQ ���쒆�I��

		m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 60);

// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
		if(systemConfig.nStageType != STAGE_TYPE_NONE){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
			bStageInit = StageInitialize();
			if(!bStageInit){
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
//				m_pApp->NanoSpecTitle(NANOSPEC_TITLE_HIDE);
/* deleted 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
//Kojika 20090526 Change
//				MessageBox("Stage Initialize Error", "NanoSpec", MB_OK | MB_ICONSTOP);
//				MessageBox("�X�e�[�W�������G���[", "NanoSpec", MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_STAGE_INIT_ERROR , strBuffer1, "Stage Initialize Error");
				LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
				MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONSTOP);
//Kojika 20090526 Change
			}
#if 1	// �R�R�ő��x������������
			else {
				if (0 != nexioIsMaintenanceSwitch()) {
					// �����e�i���X���[�h�łȂ�����
					// ���x�������Ȃ�
					StageDisableSpeedLimit();
				} else {
					// �����e�i���X���[�h������
					// ���x��������
// modified hmenjo 2009.05.12 ���x�ύX���t���O�ǉ� ---------- { ----------
//					StageEnableSpeedLimit();
// modified hmenjo 2009.05.12 ���x�ύX���t���O�ǉ� ----------
					if (TRUE == g_bIL_SpeedDown) {
						StageEnableSpeedLimit();
					}
// modified hmenjo 2009.05.12 ���x�ύX���t���O�ǉ� ---------- } ----------
				}
			}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
#if 0
		::Sleep(1000);		// ���œ���Ă���܂��DStage.dll ���ő΍�o����Εs�v�ł��D
#endif
#endif	// �R�R�ő��x������������
		nexioEquipmentStatusRun(FALSE); 	// EQ ���쒆�I�t
	}

	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 80);
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_MESSAGE2, TITLE_MSG2_INIT_TURRET);

// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	if(systemConfig.nStageType != STAGE_TYPE_NONE){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
		// 2009.07.21 K.Matsuo NSSTAGE -->
		if(bStageInit == TRUE){
			if(bNextraInit == TRUE){
				nexioEquipmentStatusRun(TRUE);		// EQ ���쒆�I��
				NS_StageInitialize();				// Turret Backlash �������A���̊֐��̒��ōs���Ă���
				nexioEquipmentStatusRun(FALSE); 	// EQ ���쒆�I�t
			}
		}
	// 2009.07.21 K.Matsuo NSSTAGE <--
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 90);

	// ----- Lens Initialize ------------------------------------------------

	// �����Y���������B�����l���ǂ��ɂ��邩�s���ł���B
	// �����Y�I�����P�x������Ă��Ȃ��iStageSelectLens()�j�ꍇ�A�J�����g�����Y�s���B
	// stage.dll�́A���̏ꍇ�A-1��ԋp���Ă���B
	// ����āA�o�O�̂��ƂɂȂ�̂ŁA�����I������B
	// �iViewWindowView�̃C�j�V�����Ŏg�p�ibEnable�j�̈�ԏ�������ݒ肵�Ă���B�j
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	if(systemConfig.nStageType != STAGE_TYPE_NONE){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
		if(bStageInit == TRUE){
			if(bNextraInit == TRUE){
				SR_TURRET srTurret[SR_LENS_MAX];
				ConfigFile_GetNanoSpecIni(srTurret, CONFIG_FILE_SR_TURRET);
				nexioEquipmentStatusRun(TRUE);		// EQ ���쒆�I��
				for(int i = 0; i < SR_LENS_MAX; i++){
					if(srTurret[i].bEnable){
						StageSelectLensNotify(i);
						break;
					}
				}
				nexioEquipmentStatusRun(FALSE); 	// EQ ���쒆�I�t
			}
		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_PROGRESS, 100);

/* added 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_CLOSE);
/* added 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
	Event(EV_DONE);
}

// ==========================================================================
//
void CMainFrame::enter_ST_SHUTDOWN(const cEventParams* const)
{
//	MEAS_Shutdown();

	OverlayExitInstance();

	if(m_pMainFrmMailRecvThread){
		m_pMainFrmMailRecvThread->ShutDown();
		::WaitForSingleObject(m_pMainFrmMailRecvThread->m_hThread, INFINITE);
		delete m_pMainFrmMailRecvThread;
		m_pMainFrmMailRecvThread = NULL;
	}

	HOST_ADDRESS hostAddress;
	ConfigFile_GetNanoSpecIni(&hostAddress, CONFIG_FILE_HOST);

	CWnd* pWnd;
	if((pWnd = CWnd::FindWindow(NULL, hostAddress.szNanoPifSockWindowTitle)) != NULL){
		pWnd->PostMessage(WM_CLOSE);
	}

/* modified 2012.01.23 hmenjo [�W]PatRecEnd()���� ---------- { ---------- */
//	if(!g_bHWS){
//		PatRecEnd();
//	}
/* modified 2012.01.23 hmenjo [�W]PatRecEnd()���� ----------              */
//Saiki 20121212 Change ----->
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	if(( m_SystemConfig.nPRMethod >0 && m_SystemConfig.bSamplePatternRec ) ||
		(m_SystemConfig.nPRMethod >0 && m_SystemConfig.bSitePatternRec)){
		PatRecEnd();
	}
//Saiki 20121212 Change <-----

/* modified 2012.01.23 hmenjo [�W]PatRecEnd()���� ---------- } ---------- */

	Event(EV_DONE);
}

// ==========================================================================
//
void CMainFrame::enter_ST_DONE(const cEventParams* const)
{
	Event(EV_DONE);
}

// ==========================================================================
//
void CMainFrame::enter_ST_ABORT(const cEventParams* const)
{
/* added 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- { ---------- */
	m_pApp->NanoSpecTitle(NANOSPEC_TITLE_CLOSE);
/* added 2009.06.05 hmenjo NanoSpecTitle �����Ȃ��΍� ---------- } ---------- */
	Event(EV_DONE);
}

// ==========================================================================
//
void CMainFrame::enter_ST_RETRY(const cEventParams* const)
{
	m_bInitRetry = TRUE;

	Event(EV_DONE);
}

/////////////////////////////////////////////////////////////////////////////
// Event                                                                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuMeasurement()
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	///// ActuateFlag�`�F�b�N /////
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//if(m_pDoc->GetDispStatus() == MAIN_MENU_MEASUREMENT)
	if(m_pDoc->GetProcessStatus() == PROCESS_PROC ||
		m_pDoc->GetProcessStatus() == PROCESS_COMP){
		if(m_iCurrMode != OP_MEASUREMENT_PROCESS){
			ChangeOperationPanel(OP_MEASUREMENT_PROCESS);
		}
		if(m_iCurrExeWnd != MEAS_WND){
			ChangeExeWnd(MEAS_WND);
		}
	}

#if 0 //
	else{

//		  if(m_iCurrExeWnd != RECIPE_MEAS_LIST_WND){
			if ( !MenuExit() )
				return;
			ChangeExeWnd(RECIPE_MAIN_LIST_WND);
//		  }
		if(m_iCurrMode != OP_MEASUREMENT){
			ChangeOperationPanel(OP_MEASUREMENT);
		}
	}
#else //�ߋ��̂ɖ߂��ĉ���
	else{
		if(m_iCurrMode != OP_MEASUREMENT){
			//Saiki 20090609 Add ----->
			if(MenuExit() == FALSE){
				return;
			}
			//Saiki 20090609 Add <-----
			ChangeOperationPanel(OP_MEASUREMENT);
		}
/// 	   if(m_iCurrExeWnd != RECIPE_MEAS_LIST_WND){
			ChangeExeWnd(RECIPE_MAIN_LIST_WND);
/// 	   }
	}
#endif

	// ��ʃX�e�[�^�X���Z�b�g
	m_pDoc->SetDispStatus(MAIN_MENU_MEASUREMENT);

	ChangeMainMenuStatus();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuEditRecipe()
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	//ActuateFlag�`�F�b�N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER))	{
		LoadStringML(IDS_LOAD_SHUTTER_WORKING, strMsg, "Sample Loading Shutter is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //���[�f�B���O�V���b�^�[�����쒆�ł�
	//Saiki 20090601 Change <-----

	//if(m_pDoc->GetDispStatus() == MAIN_MENU_RECIPE_SETTING)
	//Saiki 20090609 Change ----->
	//if(m_iCurrMode == OP_RECIPE_SETTING)
	//	return;
	if(m_iCurrMode == OP_RECIPE_SETTING){
		return;
	}
	else{
		if(MenuExit() == FALSE)
			return;
	}
	//Saiki 20090609 Change <-----

	ChangeOperationPanel(OP_RECIPE_SETTING);
	ChangeExeWnd(RECIPE_MAIN_LIST_WND);
	// ��ʃX�e�[�^�X���Z�b�g
	m_pDoc->SetDispStatus(MAIN_MENU_RECIPE_SETTING);
	ChangeMainMenuStatus();
}

/////////////////////////////////////////////////////////////////////////////
//
/* added 2014.11.22 hmenjo TohoRecall �֕ύX ---------- { ---------- */
typedef struct {
	HWND	hwndFound;
	TCHAR	tszFind[2][128];
	TCHAR	tszFound[128];
} FINDNANOMAPWIN, *LPFINDNANOMAPWIN;
BOOL CALLBACK EnumWindowsProcNanoMap(
	HWND	hwnd,		/* �e�E�B���h�E�̃n���h��	*/
	LPARAM	lParam		/* �A�v���P�[�V������`�̒l	*/
)
{
	if (0 == lParam) {
		return FALSE;
	}

	LPFINDNANOMAPWIN l_pFindNanoMapWin = (LPFINDNANOMAPWIN) lParam;

	int l_iRc = ::GetWindowText(
						hwnd,
						((LPFINDNANOMAPWIN) lParam)->tszFound,
						sizeof(((LPFINDNANOMAPWIN) lParam)->tszFound) - 1
					);
	if (0 != l_iRc) {
		for (int i = 0; i < 2; i++) {
			if (0 != _tcsstr(l_pFindNanoMapWin->tszFound, l_pFindNanoMapWin->tszFind[i])) {
				l_pFindNanoMapWin->hwndFound = hwnd;
				return TRUE;
				break;
			}
		}
	}

	return TRUE;
}
/* added 2014.11.22 hmenjo TohoRecall �֕ύX ---------- } ---------- */
void CMainFrame::OnMenuData()
{
	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	HWND hWnd;
	char szPath[MAX_PATH];

/* modified 2014.11.22 hmenjo TohoRecall �֕ύX ---------- { ---------- */
//	hWnd = ::FindWindow(NULL, NANOMAP_WINDOW_NAME);
/* modified 2014.11.22 hmenjo TohoRecall �֕ύX ----------              */
	FINDNANOMAPWIN l_FindNanoMapWin;
	memset(&l_FindNanoMapWin, 0, sizeof(l_FindNanoMapWin));
	strcpy(l_FindNanoMapWin.tszFind[0], NANOMAP_WINDOW_NAME_NEW);
	CString l_strFindWin = NANOMAP_WINDOW_NAME;
	if (g_lAppNameType == APP_NAME_TOHO) {
		l_strFindWin.Replace(g_lpszAppPrefix4[0], g_lpszAppPrefix4[1]);
	}
	strcpy(l_FindNanoMapWin.tszFind[1], l_strFindWin);
	hWnd = 0;
	BOOL l_bRc = EnumWindows(EnumWindowsProcNanoMap, (LPARAM) &l_FindNanoMapWin);
	hWnd = l_FindNanoMapWin.hwndFound;
/* modified 2014.11.22 hmenjo TohoRecall �֕ύX ---------- } ---------- */
	if(hWnd == NULL){
		STARTUPINFO StartInfo;
		PROCESS_INFORMATION ProcessInfo;
		ZeroMemory(&StartInfo,sizeof(StartInfo));
		ZeroMemory(&ProcessInfo,sizeof(ProcessInfo));
		StartInfo.wShowWindow = SW_SHOWNORMAL;
		StartInfo.cb = sizeof(StartInfo);
/* modified 2014.05.22 hmenjo �蓮�� NanoMap ---------- { ---------- */
//		sprintf(szPath, "%s%s", g_szBin_Dir, NANOMAP_EXE_NAME);
/* modified 2014.05.22 hmenjo �蓮�� NanoMap ----------              */
		CString l_strNanomapExeName = NANOMAP_EXE_NAME;
		if (g_lAppNameType == APP_NAME_TOHO) {
			l_strNanomapExeName.Replace(g_lpszAppPrefix4[0], g_lpszAppPrefix4[1]);
		}
		sprintf(szPath, "%s%s", g_szBin_Dir, l_strNanomapExeName);
/* modified 2014.05.22 hmenjo �蓮�� NanoMap ---------- } ---------- */
/* added 2014.11.22 hmenjo TohoRecall �֕ύX ---------- { ---------- */
		char l_szPathNanoMapOrg[MAX_PATH];
		strcpy(l_szPathNanoMapOrg, szPath);		/* NanoMap32 �ł̖��O��ۑ�	*/
		/* �V NanoMap32 ��	*/
		sprintf(szPath, "%s%s", g_szBin_Dir, NANOMAP_EXE_NAME_NEW);
		DWORD l_dwRc = ::GetFileAttributes(szPath);
		switch (l_dwRc) {
		case -1:
		case FILE_ATTRIBUTE_DIRECTORY:
			/* ���������D	*/
			l_dwRc = ::GetFileAttributes(l_szPathNanoMapOrg);
			switch (l_dwRc) {
			case -1:
			case FILE_ATTRIBUTE_DIRECTORY:
				/* �����Ƃ����������D	*/
				return;
				break;
			default:
				/* ���݂��܂����D	*/
				strcpy(szPath, l_szPathNanoMapOrg);
				break;
			}
			break;
		default:
			/* ���݂��܂����D	*/
			break;
		}
/* added 2014.11.22 hmenjo TohoRecall �֕ύX ---------- } ---------- */
		int ret = CreateProcess(	NULL,
									szPath, 							// BIN_DIR "\\" NANOMAP_EXE_NAME,
									NULL,
									NULL,
									FALSE,
									0,
									NULL,
									".",
									&StartInfo,
									&ProcessInfo
								);
/* added 2014.11.22 hmenjo TohoRecall �֕ύX ---------- { ---------- */
		g_ProcessInfo = ProcessInfo;
/* added 2014.11.22 hmenjo TohoRecall �֕ύX ---------- } ---------- */
	}
	else{
		::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
}
/* added 2014.11.22 hmenjo TohoRecall �֕ύX ---------- { ---------- */
void CMainFrame::OnMenuDataFromExt()
{
	this->OnMenuData();
}
/* added 2014.11.22 hmenjo TohoRecall �֕ύX ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuUserSetting()
{
	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	//if(m_pDoc->GetDispStatus() == MAIN_MENU_USER_SETTING)
	//Saiki 20090609 Change ----->
	//if(m_iCurrMode == OP_USER_SETTING)
	//	return;
	if(m_iCurrMode == OP_USER_SETTING){
		return;
	}
	else{
		if(MenuExit() == FALSE)
		return;
	}
	//Saiki 20090609 Change <-----

	ChangeOperationPanel(OP_USER_SETTING);
	ChangeExeWnd(USERCONF_WND);
	// ��ʃX�e�[�^�X���Z�b�g
	m_pDoc->SetDispStatus(MAIN_MENU_USER_SETTING);
	ChangeMainMenuStatus();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuMaintenance()
{
	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	//if(m_pDoc->GetDispStatus() == MAIN_MENU_MAINTENANCE)
	//Saiki 20090609 Change ----->
	//if(m_iCurrMode == OP_MAINTENANCE)
	//	return;
	if(m_iCurrMode == OP_MAINTENANCE){
		return;
	}
	else{
		if(MenuExit() == FALSE)
		return;
	}
	//Saiki 20090609 Change <-----

	ChangeOperationPanel(OP_MAINTENANCE);
	ChangeExeWnd(MAINTE_WND);
	// ��ʃX�e�[�^�X���Z�b�g
	m_pDoc->SetDispStatus(MAIN_MENU_MAINTENANCE);
	ChangeMainMenuStatus();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuSystem()
{
	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	//if(m_pDoc->GetDispStatus() == MAIN_MENU_SYSTEM)
	//Saiki 20090609 Change ----->
	//if(m_iCurrMode == OP_SYSTEM)
	//	return;
	if(m_iCurrMode == OP_SYSTEM){
		return;
	}
	else{
		if(MenuExit() == FALSE)
		return;
	}
	//Saiki 20090609 Change <-----

	ChangeOperationPanel(OP_SYSTEM);
	ChangeExeWnd(SYSCONF_WND);
	// ��ʃX�e�[�^�X���Z�b�g
	m_pDoc->SetDispStatus(MAIN_MENU_SYSTEM);
	ChangeMainMenuStatus();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuLocal()
{
	ChangeHostMode(HOST_LOCAL);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuRemote()
{
	ChangeHostMode(HOST_REMOTE);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuExit()
{
	MenuExit();
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnExeWndExit(WPARAM wparam, LPARAM lparam)
{
	MenuExit();

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnDoMeasure(WPARAM wparam, LPARAM lparam)
{
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnDoCancel(WPARAM wparam, LPARAM lparam)
{
	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return 0L;
		}
	}

	// ChiefDlg �փ��b�Z�[�W�𓊂���
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);

	if(0 != l_hChiefDlg){
		return ::SendMessage(l_hChiefDlg, WM_DISP_CANCEL_SEQ, 0, 0);
	}
	else{
		return -1;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnDoPause(WPARAM wparam, LPARAM lparam)
{
	// ChiefDlg �փ��b�Z�[�W�𓊂���
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		return ::SendMessage(l_hChiefDlg, WM_DISP_PAUSE_SEQ, 0, 0);
	}
	else{
		return -1;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnDoResume(WPARAM wparam, LPARAM lparam)
{
	// ChiefDlg �փ��b�Z�[�W�𓊂���
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(0 != l_hChiefDlg){
		return ::SendMessage(l_hChiefDlg, WM_DISP_RESUME_SEQ, 0, 0);
	}
	else{
		return -1;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnAlarmIf_Set(WPARAM wparam, LPARAM lparam)
{
	int Id = wparam;

	if ( m_pAlarm )
		m_pAlarm->AlarmSet(Id, NULL);

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnAlarmIf_Reset(WPARAM wparam, LPARAM lparam)
{
	int Id = wparam;

	if ( m_pAlarm )
		m_pAlarm->AlarmReset(Id);

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMenuUserSetting(CCmdUI* pCmdUI)
{
	if(m_MeasMenuEnableFlg2 == FALSE){
		pCmdUI->Enable(FALSE);
	}
	else{
		///// User Access Privilege /////
		pCmdUI->Enable(m_pDoc->User_Access(ACCESS_USERSETTING));
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMenuMaintenance(CCmdUI* pCmdUI)
{
	if(m_MeasMenuEnableFlg2 == FALSE){
		pCmdUI->Enable(FALSE);
	}
	else{
		///// User Access Privilege /////
		pCmdUI->Enable(m_pDoc->User_Access(ACCESS_MAINTENANCE));
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateSystem(CCmdUI* pCmdUI)
{
	if(m_MeasMenuEnableFlg == FALSE){
		pCmdUI->Enable(FALSE);
	}
	else{
		///// User Access Privilege /////
		pCmdUI->Enable(m_pDoc->User_Access(ACCESS_SYSTEMCONFIG));
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuLogon()
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	//ActuateFlag�`�F�b�N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER))	{
		LoadStringML(IDS_LOAD_SHUTTER_WORKING, strMsg, "Sample Loading Shutter is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //���[�f�B���O�V���b�^�[�����쒆�ł�
	//Saiki 20090601 Change <-----

	if(m_pDoc->User_Is() == FALSE){
		CLogOnDlg dlg;
		if(dlg.DoModal() == IDOK){
			m_pDoc->User_Entry(dlg.GetUser());
		}
	}
	else{
		CLogOffDlg dlg;
		if(dlg.DoModal() == IDOK){
			m_pDoc->User_Remove();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMenuAlarm()
{
	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	m_pDoc->OperationLogging("ALARM HISTORY Button was pushed.");

	RECT Rect;
	//typedef struct tagRECT {
	//	 LONG left;
	//	 LONG top;
	//	 LONG right;
	//	 LONG bottom;
	//} RECT;

	GetWindowRect(&Rect);

	g_pAlarmLogDlg->ShowWindow(SW_HIDE);
	g_pAlarmLogDlg->ShowWindow(SW_SHOWNORMAL);
/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
//	g_pAlarmLogDlg->SetWindowPos(&CWnd::wndTop, Rect.left + 270, Rect.top + 150, 0, 0, SWP_NOSIZE);
/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ----------              */
	if (g_lModelType == MODEL_T3100) {
		RECT l_rectAlmLogDlg;	g_pAlarmLogDlg->GetWindowRect(&l_rectAlmLogDlg);
		long l_lTop = Rect.top + (((Rect.bottom - Rect.top) - (l_rectAlmLogDlg.bottom - l_rectAlmLogDlg.top)) / 2);
		if (l_lTop < 0) {
			l_lTop = 0;
		}
		g_pAlarmLogDlg->SetWindowPos(&CWnd::wndTop, Rect.left + 270, l_lTop, 0, 0, SWP_NOSIZE);
	} else {
		g_pAlarmLogDlg->SetWindowPos(&CWnd::wndTop, Rect.left + 270, Rect.top + 150, 0, 0, SWP_NOSIZE);
	}
/* modified 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */

//	// ��ʃX�e�[�^�X���Z�b�g
//	m_pDoc->SetDispStatus(MAIN_MENU_ALARM_HISTORY);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMenuData(CCmdUI* pCmdUI)
{
	///// User Access Privilege /////
	pCmdUI->Enable(m_pDoc->User_Access(ACCESS_DATA));
}

/////////////////////////////////////////////////////////////////////////////
// ����i�s�󋵁i���݂̑���|�C���g / ����\�萔�j
LRESULT CMainFrame::OnSetMeasurementData(WPARAM wparam, LPARAM lparam)
{
	int iPoint = wparam;	// ����|�C���g���Ƃ̉�
	CString strBuffer;

	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0))->PostMessage(WM_MEAS_COMP_EVENT, iPoint, 0); // ����|�C���g�񐔂�WParam

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnAutoFocus()
{
	//Saiki 20090603 Add ----->
	CString strMsg, strCap;
	//Saiki 20090603 Add <-----
	if(!m_bAutoFocusInProgress){
		m_bAutoFocusInProgress = TRUE;
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
/* modified 2009.09.08 hmenjo AF ���������Y�ؑ֒u�� ---------- { ---------- */
//		if(StageDoAutoFocus()==FALSE){
/* modified 2009.09.08 hmenjo AF ���������Y�ؑ֒u�� ----------				*/
		if (FALSE == NS_StageDoAutoFocus()) {
/* modified 2009.09.08 hmenjo AF ���������Y�ؑ֒u�� ---------- } ---------- */
			m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
			CManualStageDlg dlg;

			//Saiki 20090603 Change ----->
			//dlg.SetCaption(AUTOFOCUSSETTING_CAPTION);
			//dlg.SetMessage(AUTOFOCUSSETTING_MESSAGE);
			LoadStringML(IDS_AUTOFOCUSSETTING_CAPTION, strCap, "AUTOFOCUS SETTING");
			LoadStringML(IDS_AUTOFOCUSSETTING_MESSAGE, strMsg, "AUTOFOCUS SETTING");
			dlg.SetCaption(strCap);
			dlg.SetMessage(strMsg);
			//Saiki 20090603 Change <-----
// 2009.10.30 bagus Stage-Sample �C�� --{--
			RCP_DATA rcp_data;
			m_pDoc->GetRcpData(&rcp_data);
			BOOL bMain,bStage;

			bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
			bStage = RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);

			if((bMain == FALSE) || (bStage == FALSE)){
				//���V�s���ǂ߂Ȃ������̂ŃT���v���\���͂Ȃ�
				dlg.m_nSampleMode = 0;
			}else{
				dlg.m_nSampleMode = 1;
			}

			strcpy(dlg.m_szRecipeName, rcp_data.StageProgInfoHdr.SampleInfo.szName);
// 2009.10.30 bagus Stage-Sample �C�� --}--
// 2013.02.22 bagus Substrate thickness setting -->
			if ( dlg.m_nSampleMode != 0 ) {
				BOOL bSelected;
				m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
				bSelected = SelectSubThickNotify(rcp_data.StageProgInfoHdr.SampleInfo.dThickness);
				m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

				if ( !bSelected ) {
					AlarmIf_Set(ALID_SubstrateThicknessError);
				}
			}
// 2013.02.22 bagus Substrate thickness setting <--
			dlg.DoModal();
		}
		else{
			m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
		}
		m_bAutoFocusInProgress = FALSE;
	}
	else{
		StageAbortAutoFocus();
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
		m_bAutoFocusInProgress = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnExeDlgDisplay(WPARAM wparam, LPARAM lparam)
{
	char szMessage[MAX_PATH + 1];

	memset(szMessage, 0, sizeof(szMessage));
	strncpy(szMessage, (char*) wparam, MAX_PATH);

	CExecutionDlg dlg;

	// Kojika 20090528 Change
	CString l_strBuffer;
	//dlg.SetCaption(IDS_EXECUTION_CAPTION);
	LoadStringML(IDS_EXECUTION_CAPTION , l_strBuffer, "EXECUTION");
	dlg.SetCaption(l_strBuffer);
	// Kojika 20090528 Change End

	dlg.SetMessage(szMessage);
	dlg.DoModal();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Chief Event                                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Chief Event
// ����f�[�^�i�[����
// (1��Ă΂���1�|�C���g���蕪�̃f�[�^���i�[)
// ������dat�t�@�C���͂����ō������̂��g�p����B
LRESULT CMainFrame::OnScanDataStockProc(WPARAM wparam, LPARAM lparam)
{

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// Chief Event
// 1������I�����b�Z�[�W��M
// (Seq�g�����W�V�������f�[�^�������W���[��)WM_CHIF_MEAS_END
LRESULT CMainFrame::OnOneSheetEnd(WPARAM wparam, LPARAM lparam)
{
	m_pDoc->OperationLogging("1������I�����b�Z�[�W��M");

// 2009.11.25 K.Matsuo GTr Reference -->
	if (TRUE == m_bGTRref) {
		///// �`�[�t�փ��b�Z�[�W�𑗐M /////
		HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
		if(l_hChiefDlg == NULL){
			AlarmIf_Set(ALID_MeasurementStartFailed);
// 2009.11.30 K.Matsuo GTr Reference -->
			m_bGTRref = FALSE;
			m_iGTRrefNowPointNo = 0;
// 2009.11.30 K.Matsuo GTr Reference <--
			return 0L;
		}
		::PostMessage(l_hChiefDlg, WM_DATA_MEAS_END, 0, 0);
		if(m_pDoc->GetHostMode() == HOST_REMOTE){
			PifComm_CompleteReferenceAllPointsReport(m_iGTRrefNowPointNo);
		}
// 2009.11.30 K.Matsuo GTr Reference -->
		m_bGTRref = FALSE;
		m_iGTRrefNowPointNo = 0;
// 2009.11.30 K.Matsuo GTr Reference <--
// 2010.01.13 bagus GTR --{--
		if(m_pTestMeasGantryDlgObj){
			m_pTestMeasGantryDlgObj->Invalidate();
		}
// 2010.01.13 bagus GTR --}--
		return 0L;
	}
// 2009.11.25 K.Matsuo GTr Reference <--

// 2009.11.30 K.Matsuo �P�|�C���g���莸�s���̃G���[�I���ʒm -->
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){		//�}�j���A������
		m_pManuMeaFormViewObj->ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
	}
// 2009.11.30 K.Matsuo �P�|�C���g���莸�s���̃G���[�I���ʒm <--

	SYSTEMTIME syEndDateTime;	  //����I������
	GetLocalTime(&syEndDateTime); //����I�������X�V
	m_pDoc->SetEndDateTime(&syEndDateTime);

	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);

	//�ꖇ�I���̏����֐���call(EXIT�{�^���̂��)
	if(m_pDoc->GetHostMode() == HOST_REMOTE){
// 2009.09.17 K.Matsuo �X�g���X�̖�������I�����́A�f�[�^�ۑ����Ȃ� -->
		// �X�g���X����̖�������́A�����������C�����肪����̂ŁA�񍐂��Ȃ�
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType != HEAD_TYPE_STRESS){
			MeasEnd();
		}
// 2009.09.17 K.Matsuo �X�g���X�̖�������I�����́A�f�[�^�ۑ����Ȃ� <--
	}

	///// �`�[�t�փ��b�Z�[�W�𓊂��� /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
	m_pDoc->OperationLogging("1������I���������M");
		::PostMessage(l_hChiefDlg, WM_DATA_MEAS_END, 0, 0); 				// 1������I������
	}
	else{
			AlarmIf_Set(ALID_OneSampledMeasurementFailed);
	}

	//�V�[�P���X���胂�[�h�t���O�ݒ�
	m_pDoc->SetSeqMeasMode(SEQ_NOT_MEASURE);								// ���蒆�łȂ��ɐݒ�

	m_pDoc->SetReMeasurePointCnt(0);

// 2009.09.17 K.Matsuo �X�g���X�̖������蒆�́A��ʑJ�ڂ����Ȃ� -->
#if 0
//������ 090326 hibino add
#define PROCESS_RETRY_TIME	10000	//(ms)�v���Z�X���̃��g���C���M�܂ł̑҂�����(�኱�̌덷�L��)
#define ROOP_WAIT			10		//(ms)������� (0�͋֎~)
	if(m_pDoc->GetHostMode() == HOST_REMOTE){
		for(int i=0; i < (PROCESS_RETRY_TIME/ROOP_WAIT)*2 +1; i++){
			int iProcessStatus = m_pDoc->GetProcessStatus();

			if(iProcessStatus == PROCESS_DOWN){
				m_pDoc->OperationLogging("�����[�g����I����A��ʑJ�ڏ������ɃX�e�[�^�X���wDOWN�x");
// 2009.06.04 K.Matsuo -->
//				//�_�E�����͑����ʂɂƂǂ܂�B
				// ���d�l�ύX�F�_�E���������C����ʂɖ߂�
				ChangeOperationPanel(OP_MEASUREMENT);
				ChangeExeWnd(RECIPE_MAIN_LIST_WND);
				break;
// 2009.06.04 K.Matsuo <--
			}

			if((iProcessStatus == PROCESS_COMP)||(iProcessStatus == PROCESS_ABRT)||(iProcessStatus == PROCESS_WAIT)){
				ChangeOperationPanel(OP_MEASUREMENT);
				ChangeExeWnd(RECIPE_MAIN_LIST_WND);
				m_pDoc->OperationLogging("�����[�g����I����A��ʑJ�ڏ�������");
				break;
			}
			///// �v���Z�X�� (or�C�j�V����) /////
			else{
				//��ʐؑւ����ɃX�e�[�^�X���v���Z�X���������ꍇ�A��莞�ԑ҂�����A������I�����������g���C����
				//����ł��v���Z�X��Ԃ������ꍇ�̓A���[���𔭐�������

				Sleep(ROOP_WAIT);

				//���g���C
				if((i%(PROCESS_RETRY_TIME/ROOP_WAIT) == 0)&&(i != 0)){
					//���g���C��̑҂����ԏ�����
					if(i>(PROCESS_RETRY_TIME/ROOP_WAIT)){
						m_pDoc->SetProcessStatus(PROCESS_DOWN);
						AlarmIf_Set(ALID_StatusTransitionError);
						break;
					}

					//�`�[�t�փ��b�Z�[�W�𓊂���
					HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
					if(0 != l_hChiefDlg){
						m_pDoc->OperationLogging("�����[�g����I����A��ʑJ�ڏ������w1������I�������x���g���C���s");
						::PostMessage(l_hChiefDlg, WM_DATA_MEAS_END, 0, 0); //1������I������
					}
					else{
						AlarmIf_Set(ALID_OneSampledMeasurementFailed);
					}
				}
			}
		}
	}

//������ 090326 hibino add
#endif
	if(m_pDoc->GetHostMode() == HOST_REMOTE){
		// �X�g���X����̖�������́A�����������C�����肪����̂ŁA��ʑJ�ڂ����Ȃ�
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType != HEAD_TYPE_STRESS){
			ChangeOperationPanel(OP_MEASUREMENT);
			ChangeExeWnd(RECIPE_MAIN_LIST_WND);
		}
	}
// 2009.09.17 K.Matsuo �X�g���X�̖������蒆�́A��ʑJ�ڂ����Ȃ� <--

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// Chief Event
// �ėp�|�W�V�����ړ��������b�Z�[�W��M
// (AAF�g�����W�V��������ʃ��W���[��)WM_CHIF_MOVE_SETPOS_END
LRESULT CMainFrame::OnMoveSetposEnd(WPARAM wparam, LPARAM lparam)
{
	CView* pView;
	pView = (CView*)(((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_wndSplitter1.GetPane(1, 0));
	((CMeasurementOperationView*) pView)->PostMessage(WM_CHIF_MOVE_SETPOS_END, wparam, lparam);

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// Chief Event
// �o�L���[���w�ߊ������b�Z�[�W��M
// (MSG�n���h������ʃ��W���[��)WM_CHIF_VACUUM_END
LRESULT CMainFrame::OnVacuumEnd(WPARAM wparam, LPARAM lparam)
{

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// Chief Event
// AF�|�b�v�A�b�v�w�߃��b�Z�[�W��M
// (AAF�g�����W�V��������ʃ��W���[��)WM_CHIF_AF_POPUP
LRESULT CMainFrame::OnAfPopup(WPARAM wparam, LPARAM lparam)
{
	CAutoFocusSettingDlg dlg;
	//Saiki 20090603 Add ----->
	CString strMsg, strCap;
	//Saiki 20090603 Add <-----

	//2009.11.30 bagus �C�� --{--
	//�}�j���A�����莞�ɂ͂����œǂݍ���
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
		RecipeFile_LoadRecipe(&dlg.m_StageProgInfoHdr, rcp.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
	}else{
		dlg.m_StageProgInfoHdr = rcp.StageProgInfoHdr;
	}
	//2009.11.30 bagus �C�� --}--

	switch( wparam ){
	case 0:
		//Saiki 20090603 Change ----->
		//dlg.SetCaption(AUTOFOCUSSETTING_CAPTION);
		//dlg.SetMessage(AUTOFOCUSSETTING_MESSAGE);
		LoadStringML(IDS_AUTOFOCUSSETTING_CAPTION, strCap, "AUTOFOCUS SETTING");
		LoadStringML(IDS_AUTOFOCUSSETTING_MESSAGE, strMsg, "AUTOFOCUS SETTING");
		dlg.SetCaption(strCap);
		dlg.SetMessage(strMsg);
		//Saiki 20090603 Change <-----
		break;
// ---- 20090223 hmenjo 1/2 ���t ------ { -----
//	case 1:
//		dlg.SetCaption(AUTOFOCUS_POSITIONADJUST_CAPTION);
//		dlg.SetMessage(AUTOFOCUS_POSITIONADJUST_MESSAGE);
//		break;
//	case 2:
//		dlg.SetCaption(POSITIONADJUST_CAPTION);
//		dlg.SetMessage(POSITIONADJUST_MESSAGE);
//		break;
// ---- 20090223 hmenjo 1/2 ���t ------
	case 1:
		//Saiki 20090603 Change ----->
		//dlg.SetCaption(POSITIONADJUST_CAPTION);
		//dlg.SetMessage(POSITIONADJUST_MESSAGE);

		LoadStringML(IDS_POSITIONADJUST_CAPTION, strCap, "POSITION ADJUST");
		LoadStringML(IDS_POSITIONADJUST_MESSAGE, strMsg, "POSITION ADJUST");
		dlg.SetCaption(strCap);
		dlg.SetMessage(strMsg);
		//Saiki 20090603 Change <-----
		break;
	case 2:
		//dlg.SetCaption(AUTOFOCUS_POSITIONADJUST_CAPTION);
		//dlg.SetMessage(AUTOFOCUS_POSITIONADJUST_MESSAGE);
		LoadStringML(IDS_AUTOFOCUS_POSITIONADJUST_CAPTION, strCap, "AUTOFOCUS SETTING & POSITION ADJUST");
		LoadStringML(IDS_AUTOFOCUS_POSITIONADJUST_MESSAGE, strMsg, "AUTOFOCUS SETTING & POSITION ADJUST");
		dlg.SetCaption(strCap);
		dlg.SetMessage(strMsg);
		break;
// ---- 20090223 hmenjo 1/2 ���t ------ } -----
	default:
		// AF�|�b�v�A�b�v���Ȃ�
		return	0L;
		break;
	}

// ADD 2009.06.02
	// �K��JoyStick�̏�Ԃ�߂�����
	m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.02

// DEL 2009.06.03
// �ʏ푪��ɉe������̂ō폜����
//	dlg.m_joyContinue = TRUE;					// JoyStick�̏�Ԃ��p��������
// DEL 2009.06.03

	dlg.DoModal();

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnManualDeskewDlgPopup(WPARAM wparam, LPARAM lparam)
{
	// �}�j���A���f�X�L���[���s

	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);

	int iSite = (int)wparam;
	UINT message;

	CManualDeskewDlg dlg(&rcp.StageProgInfoHdr.DeskewSiteManual[iSite]);
	//Sample�Z�b�g
	dlg.SetFileName(rcp.StageProgInfoHdr.SampleInfo.szName);
	dlg.SetSampleMode(1);

	int iRet = dlg.DoModal();
	switch ( iRet ) {
	case IDOK:
		if ( iSite == 0 )
			message = WM_DESKEWSEQ_DESKEWMANUAL_SITE1_OK;
		else
			message = WM_DESKEWSEQ_DESKEWMANUAL_SITE2_OK;
		break;
	case IDCANCEL:
		message = WM_DESKEWSEQ_DESKEWMANUAL_CANCEL;
		break;
	default:
		message = WM_DESKEWSEQ_DESKEWMANUAL_NONE;
		break;
	}

	//���b�Z�[�W�𓊂���
	HWND hWnd = ::FindWindow(0, DESKEW_MANUAL_SEQ_WINDOW_NAME);
	if ( hWnd )
		::PostMessage(hWnd, message, dlg.m_lX, dlg.m_lY);

	return 0L;
}

// 2009.05.13 myanagida -->
/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnManualSitePtDlgPopup(WPARAM wparam, LPARAM lparam)
{
	// �}�j���A���T�C�g�p�^�[�����s

	int iSite = (int)wparam;
	UINT message;

//->>>> myan �ύX�K�v�ȓ_
	SITEPT_SITE_MANUAL m_SitePtManual;

	CManualSitePtDlg dlg(&m_SitePtManual);

	//Sample�Z�b�g
	dlg.SetFileName("");
	dlg.SetSampleMode(1);
//<<<<- myan �ύX�K�v�ȓ_

	int iRet = dlg.DoModal();
	switch ( iRet ) {
	case IDOK:
		message = WM_SITEPTSEQ_SITEPTMANUAL_OK;
		break;
	case IDCANCEL:
		message = WM_SITEPTSEQ_SITEPTMANUAL_CANCEL;
		break;
	default:
		message = WM_SITEPTSEQ_SITEPTMANUAL_NONE;
		break;
	}

	//���b�Z�[�W�𓊂���
	HWND hWnd = ::FindWindow(0, SITEPT_MANUAL_SEQ_WINDOW_NAME);
	if ( hWnd )
		::PostMessage(hWnd, message, dlg.m_lX, dlg.m_lY);

	return 0L;
}
// 2009.05.13 myanagida <--

/////////////////////////////////////////////////////////////////////////////
// Chief Event
// ���t�@�����X����I�����b�Z�[�W��M
LRESULT CMainFrame::OnSrrefEnd(WPARAM wparam, LPARAM lparam)
{
	HWND hWnd;
	BOOL bRet;

	// Kojika 20090528 Add
	CString l_strBuffer;
	// Kojika 20090528 Add End

	// Kojika 20090528 Change
	//hWnd = ::FindWindow(NULL, IDS_EXECUTION_CAPTION);
	LoadStringML(IDS_EXECUTION_CAPTION , l_strBuffer, "EXECUTION");
	hWnd = ::FindWindow(NULL, l_strBuffer);
	// Kojika 20090528 Change End
	if(hWnd != NULL){
		bRet = ::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
	}

	// Kojika 20090528 Change
	//hWnd = ::FindWindow(NULL, "���t�@�����X ���j���[");
	LoadStringML(IDS_REFERENCE_CAPTION , l_strBuffer, "REFERENCE MENU");
	hWnd = ::FindWindow(NULL, l_strBuffer);
	// Kojika 20090528 Change End
	if(hWnd != NULL){
		bRet = ::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
	}

	//���t�@�����X�A�_�[�N���t�@�����X�i�[
	if(m_pDoc->GetMeasMode() == MEASMODE_TEST) //�e�X�g���[�h
	{
		TEST_MODE_DATA TestModeData;
		memset(&TestModeData, 0, sizeof(TestModeData));

// 2014.04.04 bagus wavelength step modified -->
		TestModeData.dMinTargetWave = (double)MIN_TARGET_WAVELENGTH;
		TestModeData.dMaxTargetWave = (double)m_iMaxWaveLength;
		switch(m_srConfig.nWavelengthStep){
		case SR_WAVELENGTH_STEP_05_NM:
			TestModeData.dWavelengthStep = 0.5;
			break;
		case SR_WAVELENGTH_STEP_1_NM:
		default:
			TestModeData.dWavelengthStep = 1.0;
			break;
		}
// 2014.04.04 bagus wavelength step modified <--

		int iPoint = m_pDoc->GetStoreCount();
//		m_pDoc->GetTestModeData(&TestModeData, iPoint); //�ŐV�|�C���g+1�̂Ƃ���ɏ������ނ̂ŕs�v

#if 0 // 2014.04.04 bagus wavelength step modified -->
// // 2009.05.15 k-matsuo -->
// //		for(int i = MIN_TARGET_WAVELENGTH; i <= MAX_TARGET_WAVELENGTH ; i++)
// 		for(int i = MIN_TARGET_WAVELENGTH; i <= m_iMaxWaveLength ; i++)
// // 2009.05.15 k-matsuo <--
// 		{
// 			TestModeData.dScan1stReferenceData[i-1] = MEAS_GetScan1stReferenceData(i);
// 			TestModeData.dScanDark_T1Data[i-1] = MEAS_GetScanDark_T1Data(i);
// 		}
#else
		ASSERT(TestModeData.dWavelengthStep > 0.0);

		double dTargetWave = TestModeData.dMinTargetWave;
		double dData;
		while(dTargetWave <= TestModeData.dMaxTargetWave)
		{
			dData = MEAS_GetScan1stReferenceData(dTargetWave);
			TestModeData.SetScan1stReferenceData(dTargetWave, dData);
			dData = MEAS_GetScanDark_T1Data(dTargetWave);
			TestModeData.SetScanDark_T1Data(dTargetWave, dData);

			dTargetWave += TestModeData.dWavelengthStep;
		}
#endif // 2014.04.04 bagus wavelength step modified <--

		m_pDoc->SetTestModeData(&TestModeData, iPoint + 1); //�ŐV�|�C���g+1�Ɋi�[(�����[0]�Ɋi�[)

		//���t�@�����X����I������ʂ֒ʒm
// 2010.01.06 bagus Gantry --{--
		if(m_pTestMeasDlgObj && m_pTestMeasDlgObj->GetSafeHwnd() != NULL)
			m_pTestMeasDlgObj->PostMessage(WM_TESTMODE_REF_END, (WPARAM)0, (LPARAM)0);
		if(m_pTestMeasGantryDlgObj && m_pTestMeasGantryDlgObj->GetSafeHwnd() != NULL)
			m_pTestMeasGantryDlgObj->PostMessage(WM_TESTMODE_REF_END, (WPARAM)0, (LPARAM)0);
// 2010.01.06 bagus Gantry --}--

		m_pDoc->SetProcessStatus(PROCESS_WAIT);

		// H/W�L���Ȃ�
		if( GetJoyStickMode() != 0 ){
			BOOL bJoyStk= m_pDoc->ChangeJoyStick(TRUE); //�W���C�X�e�B�b�N���g�p�ɐݒ�
			TRACE("CMainFrame::OnSrrefEnd()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
		}
// ADD 2009.06.01
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.01
	}
//Saiki 20130123 add --->
	else if(m_pDoc->GetMeasMode() == MEASMODE_REFERENCE){
		m_pDoc->SetProcessStatus(PROCESS_WAIT);
	}
//Saiki 20130123 add <---

	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //�}�j���A�����胂�[�h
	{
		m_pManuMeaFormViewObj->ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
// 		if(g_lModelType == MODEL_T3100){
// 			// ���b�Z�[�W�{�b�N�X�̕\���i����ʒu�ֈړ����Ă��������j
// 			PostMessage(WM_DISP_CONFIRM_POPUP, 20, 0);
// 		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	}

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// Chief Event
// AAF�I�����b�Z�[�W��M
// (MSG�n���h������ʃ��W���[��)WM_CHIF_AAF_END
LRESULT CMainFrame::OnAafEnd(WPARAM wparam, LPARAM lparam)
{


	return 0L;
}

// bagus 2014.01.10 Add(Stage None�Ή�) -->
/////////////////////////////////////////////////////////////////////////////
// ��ʂւ�Si���t�@�������X�擾������(Pif P515)
LRESULT CMainFrame::OnCompleteSiReferenceDoneReport(WPARAM wparam, LPARAM lparam)
{
	DWORD l_ErrCode = (DWORD)wparam;

	PifComm_CompleteSiReferenceDoneReport(l_ErrCode);	// P515

	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// ��ʂւ̃_�[�N���t�@�������X�擾������(Pif P517)
LRESULT CMainFrame::OnCompleteDarkReferenceDoneReport(WPARAM wparam, LPARAM lparam)
{
	DWORD l_ErrCode = (DWORD)wparam;

	PifComm_CompleteDarkReferenceDoneReport(l_ErrCode);	// P517

	return 0L;
}
// bagus 2014.01.10 Add(Stage None�Ή�) <--

/////////////////////////////////////////////////////////////////////////////
// Function                                                                //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// MenuExit Exit���j���[���Ă΂ꂽ��́A�߂��ʂ̕\���ɂ��Ē�`����
BOOL CMainFrame::MenuExit()
{
	CNanoUI* pView = (CNanoUI*)m_wndSplitter2.GetPane(0, 0);

	if ( !pView->Exit() )
		return FALSE;

	CMap<int, int, int, int> wndMap;
	wndMap[RECIPE_MAIN_SR_WND]						= RECIPE_MAIN_LIST_WND;
	// 2009.09.19 bagus SE --{--
	wndMap[RECIPE_MAIN_SE_WND]						= RECIPE_MAIN_LIST_WND;
	// 2009.09.19 bagus SE --}--
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	wndMap[RECIPE_MAIN_COMPEASE_WND]				= RECIPE_MAIN_LIST_WND;
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	//2009.08.16 bagus stress --{--
	wndMap[RECIPE_MAIN_STRESS_WND]					= RECIPE_MAIN_LIST_WND;
	//2009.08.16 bagus stress --}--
	// 2009.12.01 K.Matsuo 4PP �ǉ� --{--
	wndMap[RECIPE_MAIN_4PP_WND] 					= RECIPE_MAIN_LIST_WND;
	// 2009.12.01 K.Matsuo 4PP �ǉ� --}--
	// 2009.10.08 bagus CTA �ǉ� --{--
	wndMap[RECIPE_MAIN_CTA_WND] 					= RECIPE_MAIN_LIST_WND;
	// 2009.10.08 bagus CTA �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	wndMap[RECIPE_MAIN_MS_WND]						= RECIPE_MAIN_LIST_WND;
	//2009.10.20 bagus MS �ǉ� --}--
	wndMap[RECIPE_MULTI_WND]						= RECIPE_MULTI_LIST_WND;
	wndMap[RECIPE_MEAS_SR_THICKNESS_WND]			= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_REFLECTANCE_WND]			= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_TRANSMITTANCE_WND]		= RECIPE_MEAS_LIST_WND;
// 2010.01.07 bagus Gantry --{--
	wndMap[RECIPE_MEAS_SR_GANTRY_WND]				= RECIPE_MEAS_LIST_WND;
// 2010.01.07 bagus Gantry --}--
	wndMap[RECIPE_MEAS_SR_CIE_REFLECTANCE_WND]		= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND]	= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_OD_WND]					= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_4PP_WND] 					= RECIPE_MEAS_LIST_WND;
	//2009.08.16 bagus stress --{--
	wndMap[RECIPE_MEAS_STRESS_WND]					= RECIPE_MEAS_LIST_WND;
	//2009.08.16 bagus stress --}--
	//2009.09.03 bagus se --{--
	wndMap[RECIPE_MEAS_SE_THICKNESS_WND]			= RECIPE_MEAS_LIST_WND;
	//2009.09.03 bagus se --}--
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	wndMap[RECIPE_MEAS_COMPEASE_THICKNESS_WND]		= RECIPE_MEAS_LIST_WND;
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	// 2009.10.08 bagus CTA �ǉ� --{--
	wndMap[RECIPE_MEAS_CTA_WND] 					= RECIPE_MEAS_LIST_WND;
	// 2009.10.08 bagus CTA �ǉ� --}--
	// 2009.10.13 bagus Distance �ǉ� --{--
	wndMap[RECIPE_MEAS_SR_DISTANCE_WND] 			= RECIPE_MEAS_LIST_WND;
	// 2009.10.13 bagus Distance �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	wndMap[RECIPE_MEAS_MS_WND]						= RECIPE_MEAS_LIST_WND;
	//2009.10.20 bagus MS �ǉ� --}--
	wndMap[RECIPE_STAGE_WND]						= RECIPE_STAGE_LIST_WND;
	// 2009.09.04 bagus stress Stage Stress�ǉ� --{--
	wndMap[RECIPE_STAGE_STRESS_WND] 				= RECIPE_STAGE_LIST_WND;
	// 2009.09.04 bagus stress Stage Stress�ǉ� --}--
	// 2009.10.02 bagus Gantry --{--
	wndMap[RECIPE_STAGE_GANTRY_WND] 				= RECIPE_STAGE_LIST_WND;
	// 2009.10.02 bagus Gantry --}--
	// 2009.10.13 bagus Distance �ǉ� --{--
	wndMap[RECIPE_STAGE_SR_DISTANCE_WND]			= RECIPE_STAGE_LIST_WND;
	// 2009.10.13 bagus Distance �ǉ� --}--
	wndMap[RECIPE_RECALIBRATION_WND]				= RECIPE_RECALIBRATION_LIST_WND;
	wndMap[RECIPE_POINT_DESKEW_WND] 				= RECIPE_POINT_DESKEW_LIST_WND;
	wndMap[USERCONF_DEF_MAIN_RECIPE_WND]			= USERCONF_WND;
	wndMap[USERCONF_DEF_SR_THICKNESS_WND]			= USERCONF_WND;
	wndMap[USERCONF_DEF_SR_REFLECTANCE_WND] 		= USERCONF_WND;
	wndMap[USERCONF_DEF_SR_TRANSMITTANCE_WND]		= USERCONF_WND;
	wndMap[USERCONF_DEF_SR_CIE_REFLECTANCE_WND] 	= USERCONF_WND;
	wndMap[USERCONF_DEF_SR_CIE_TRANSMITTANCE_WND]	= USERCONF_WND;
	wndMap[USERCONF_DEF_SR_OD_WND]					= USERCONF_WND;
	wndMap[USERCONF_DEF_SR_4PP_WND] 				= USERCONF_WND;
	//2009.09.07 bagus se --{--
	wndMap[USERCONF_DEF_SE_THICKNESS_WND]			= USERCONF_WND;
	//2009.09.07 bagus se --}--
	// 2009.09.29 bagus SE --{--
	wndMap[USERCONF_DEF_SE_MAIN_RECIPE_WND] 		= USERCONF_WND;
	// 2009.09.29 bagus SE --}--
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	wndMap[USERCONF_DEF_COMPEASE_THICKNESS_WND]		= USERCONF_WND;
	wndMap[USERCONF_DEF_COMPEASE_MAIN_RECIPE_WND] 	= USERCONF_WND;
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	// 2009.11.21 K.Matsuo 4PP �ǉ� --{--
	wndMap[USERCONF_DEF_4PP_MAIN_RECIPE_WND]		= USERCONF_WND;
	wndMap[USERCONF_DEF_4PP_WND]					= USERCONF_WND;
	// 2009.11.21 K.Matsuo 4PP �ǉ� --}--
	// 2009.10.08 bagus CTA �ǉ� --{--
	wndMap[USERCONF_DEF_CTA_MAIN_RECIPE_WND]		= USERCONF_WND;
	wndMap[USERCONF_DEF_CTA_WND]					= USERCONF_WND;
	// 2009.10.08 bagus CTA �ǉ� --}--
	// 2009.10.13 bagus Distance �ǉ� --{--
	wndMap[USERCONF_DEF_SR_DISTANCE_WND]			= USERCONF_WND;
	// 2009.10.13 bagus Distance �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	wndMap[USERCONF_DEF_MS_MAIN_RECIPE_WND] 		= USERCONF_WND;
	wndMap[USERCONF_DEF_MS_WND] 					= USERCONF_WND;
	//2009.10.20 bagus MS �ǉ� --}--
	//2009.10.21 bagus Stress �ǉ� --{--
	wndMap[USERCONF_DEF_STRESS_MAIN_RECIPE_WND] 	= USERCONF_WND;
	wndMap[USERCONF_DEF_STRESS_WND] 				= USERCONF_WND;
	//2009.10.21 bagus Stress �ǉ� --}--

	int wnd;
	if ( wndMap.Lookup(m_iCurrExeWnd, wnd) )
		ChangeExeWnd(wnd);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
/* added 2016.08.12 hmenjo ���V�s�ҏW menu �o�O2 ---------- { ---------- */
void CMainFrame::OnRecipeSave()
{
	if (m_iCurrMode == OP_RECIPE_SETTING) {
		/* ���V�s�ҏW���[�h	*/
		(((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0))->PostMessage(WM_COMMAND, (WPARAM) MAKEWPARAM(IDM_SAVE, 0), (LPARAM) 0);
	}
}
/* added 2016.08.12 hmenjo ���V�s�ҏW menu �o�O2 ---------- } ---------- */
void CMainFrame::OnUpdateMenuSave(CCmdUI* pCmdUI)
{
//	BOOL bEnable = TRUE;
//	switch(m_iCurrExeWnd){
//	case RECIPE_SR_MAIN_RECIPE_WND:
//	case RECIPE_MULTI_WND:
//	case RECIPE_STAGE_WND:
//	case RECIPE_RECALIBRATION_WND:
//	case RECIPE_POINT_DESKEW_WND:
//	case RECIPE_MEAS_SR_THICKNESS_WND:
//	case RECIPE_MEAS_SR_REFLECTANCE_WND:
//	case RECIPE_MEAS_SR_TRANSMITTANCE_WND:
// 2010.01.07 bagus Gantry --{--
//	case RECIPE_MEAS_SR_GANTRY_WND:
// 2010.01.07 bagus Gantry --}--
//	case RECIPE_MEAS_SR_CIE_REFLECTANCE_WND:
//	case RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND:
//	case RECIPE_MEAS_SR_OD_WND:
//	case RECIPE_MEAS_4PP_WND:
//		bEnable = strcmp(m_szSelectListName, _TEXT("")) != 0;
//		break;
//	}
//
//	pCmdUI->Enable(bEnable);
/* added 2016.08.12 hmenjo ���V�s�ҏW menu �o�O2 ---------- { ---------- */
	BOOL l_bRc = FALSE;
	if (m_iCurrMode == OP_RECIPE_SETTING) {
		/* ���V�s�ҏW���[�h	*/
		l_bRc = ((CNanoRecipeUI*) (((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0)))->IsAccessPrivilege();
	}
	pCmdUI->Enable(l_bRc);
/* added 2016.08.12 hmenjo ���V�s�ҏW menu �o�O2 ---------- } ---------- */
}
/* added 2016.08.12 hmenjo ���V�s�ҏW menu �o�O2 ---------- { ---------- */
void CMainFrame::OnRecipeSaveAs()
{
	if (m_iCurrMode == OP_RECIPE_SETTING) {
		/* ���V�s�ҏW���[�h	*/
		(((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0))->PostMessage(WM_COMMAND, (WPARAM) MAKEWPARAM(IDM_SAVE_AS, 0), (LPARAM) 0);
	}
}
void CMainFrame::OnUpdateRecipeSaveAs(CCmdUI* pCmdUI)
{
	BOOL l_bRc = FALSE;
	if (m_iCurrMode == OP_RECIPE_SETTING) {
		/* ���V�s�ҏW���[�h	*/
		l_bRc = ((CNanoRecipeUI*) (((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0)))->IsAccessPrivilege();
	}
	pCmdUI->Enable(l_bRc);
}
/* added 2016.08.12 hmenjo ���V�s�ҏW menu �o�O2 ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::ChangeHostMode(int iMode)
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	///// �����e�i���X���[�h�`�F�b�N /////
	if(nexioIsEngineerMaintenanceSwitch() == ON){
		if( nexioIsMaintenanceSwitch() != OFF ){
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			return;
		}
	}

	//ActuateFlag�`�F�b�N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER))	{
		LoadStringML(IDS_LOAD_SHUTTER_WORKING, strMsg, "Sample Loading Shutter is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //���[�f�B���O�V���b�^�[�����쒆�ł�
	//Saiki 20090601 Change <-----

	// Main Menu View
	CView* pView = (CView *)m_wndSplitter1.GetPane(0, 0);
	((CMainMenuView *)pView)->ChangeHostMode(iMode);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::ChangeMainMenuStatus()
{
	// Main Menu View
	CView* pView = (CView *)m_wndSplitter1.GetPane(0, 0);
	((CMainMenuView *)pView)->UpdateButtonStatus(m_iCurrMode);
}

/////////////////////////////////////////////////////////////////////////////
// �o�L���[���̃X�e�[�^�X��ύX����
void CMainFrame::ChangeVacuumStatus()
{
}

/////////////////////////////////////////////////////////////////////////////
// ���u�̃X�e�[�^�X��ύX����
void CMainFrame::ChangeEqStatus()
{
}

/////////////////////////////////////////////////////////////////////////////
// CheckSelectMainRecipe ���V�s�ꗗ���烌�V�s���I������Ă��邩�ǂ���
BOOL CMainFrame::CheckSelectMainRecipe()
{
	// Main Recipe List
	CNanoRecipeListUI* pView = (CNanoRecipeListUI*)m_wndSplitter2.GetPane(0, 0);
	return pView->CheckSelectRecipe();
}

/////////////////////////////////////////////////////////////////////////////
// ChangeOperationPanel �����̉�ʐؑւ��B�r���[�̍폜�ƍ쐬���s��
void CMainFrame::ChangeOperationPanel(int iMode, int iEditMode /* = EDIT_MAIN_RECIPE */)
{
	// ���݂Ɠ������[�h�Ȃ珈�����Ȃ�
	// ������Measurement Mode, Edit Mode, Config Mode, System Mode���ɂ̓��W�I�{�^���̏��������s��
	if(m_iCurrMode == iMode){
		if(m_iCurrMode == OP_MEASUREMENT	||
			m_iCurrMode == OP_RECIPE_SETTING){
			((CView *)m_wndSplitter1.GetPane(1, 0))->OnInitialUpdate();
		}
		return;
	}

	m_iCurrMode = iMode;

	CRuntimeClass* pNewViewClass;
	CCreateContext Context;

	switch(iMode){
	case OP_MEASUREMENT:
		pNewViewClass = RUNTIME_CLASS(CMeasurementOperationView);
		m_pDoc->SetDispStatus(MAIN_MENU_MEASUREMENT);
		break;
	case OP_MEASUREMENT_PROCESS:
		pNewViewClass = RUNTIME_CLASS(CMeasurementProcessView);
		m_pDoc->SetDispStatus(MAIN_MENU_MEASUREMENT);
		break;
	case OP_MANUAL_MEASUREMENT_PROCESS:
		pNewViewClass = RUNTIME_CLASS(CManualMeasurementProcessView);
		m_pDoc->SetDispStatus(MAIN_MENU_MEASUREMENT);
		break;
	case OP_RECIPE_SETTING:
		pNewViewClass = RUNTIME_CLASS(CRecipeOperationView);
		m_pDoc->SetDispStatus(MAIN_MENU_RECIPE_SETTING);
		break;
	case OP_DATA:
		pNewViewClass = RUNTIME_CLASS(CDataListOperationView);
		m_pDoc->SetDispStatus(MAIN_MENU_DATA);
		break;
	case OP_DATA_REVIEW:
		pNewViewClass = RUNTIME_CLASS(CDataReviewOperationView);
		m_pDoc->SetDispStatus(MAIN_MENU_DATA);
		break;
	case OP_USER_SETTING:
		pNewViewClass = RUNTIME_CLASS(CUserSettingOperationTabView);
		m_pDoc->SetDispStatus(MAIN_MENU_USER_SETTING);
		break;
	case OP_MAINTENANCE:
		pNewViewClass = RUNTIME_CLASS(CMaintenanceOperationView);
		m_pDoc->SetDispStatus(MAIN_MENU_MAINTENANCE);
		break;
	case OP_SYSTEM:
		pNewViewClass = RUNTIME_CLASS(CSystemOperationTabView);
		m_pDoc->SetDispStatus(MAIN_MENU_SYSTEM);
		break;
	case OP_ALARM_HISTORY:
		pNewViewClass = RUNTIME_CLASS(CAlarmHistoryOperationView);
		m_pDoc->SetDispStatus(MAIN_MENU_ALARM_HISTORY);
		break;
	case OP_MANUAL_MEASUREMENT_FORM:
		pNewViewClass = RUNTIME_CLASS(CManualMeasurementFormView);
//������ 090119 hibino �b��
		m_pDoc->SetDispStatus(MAIN_MENU_MANUAL_MEASUREMENT); //��
//		m_pDoc->SetDispStatus(MAIN_MENU_MEASUREMENT); //��
//�}�j���A�����莞�́AMAIN_MENU_MANUAL_MEASUREMENT�X�e�[�^�X�Ƃ��܂����A
//�`�[�t�̓��쐧���Ή����{�����܂ł́A�b��I��MAIN_MENU_MEASUREMENT�X�e�[�^�X�Ƃ��܂��B
//������ 090119 hibino �b��
		break;
	default:
		pNewViewClass = RUNTIME_CLASS(CMeasurementOperationView);
		m_pDoc->SetDispStatus(MAIN_MENU_MEASUREMENT);
		break;
	}

	m_wndSplitter1.SetActivePane(1, 0);

	Context.m_pNewViewClass 	= pNewViewClass;
	Context.m_pCurrentDoc		= GetActiveDocument();
	Context.m_pCurrentFrame 	= this;
	Context.m_pNewDocTemplate	= Context.m_pCurrentDoc->GetDocTemplate();
	Context.m_pLastView 		= (CView *)m_wndSplitter1.GetPane(1,0);

	m_wndSplitter1.DeleteView(1, 0);
	m_wndSplitter1.CreateView(1, 0, pNewViewClass, CSize(SPLIT_WIDTH, m_iSplit_OpePanelHeight), &Context);

	CView *pView = (CView*)m_wndSplitter1.GetPane(1, 0);
	pView->GetParentFrame()->RecalcLayout();
	m_wndSplitter1.RecalcLayout();
	pView->OnInitialUpdate();
	m_wndSplitter1.SetActivePane(1, 0);

	// Edit Mode�Ń��V�s�ҏW��ʂ���߂��Ă����ꍇ�ɂ́C���O�̃��X�g��\������
	if(iMode == OP_RECIPE_SETTING){
		((CRecipeOperationView *)pView)->SetMenuButton(iEditMode);
	}

	ChangeMenu();


	//���C����ʈȊO�́A�펞[���O�I��/���O�I�t]�{�^���𖳌��ɂ���
	BOOL bButtonEnable;
	bButtonEnable = (m_pDoc->GetDispStatus() == MAIN_MENU_MEASUREMENT);

	if(m_pDoc->GetHostMode() != HOST_REMOTE) //���[�J����
	{
		m_wndSplitter1.GetPane(0, 0)->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(bButtonEnable);
	}
}

/////////////////////////////////////////////////////////////////////////////
// ChangeExeWnd �E���̉�ʐؑւ��B�r���[�̍폜�ƍ쐬���s��
void CMainFrame::ChangeExeWnd(int iExeWnd)
{
	CRuntimeClass* pNewViewClass;
	int iNewRecipeHeadType = HEAD_TYPE_SR;

	// ���݂Ɠ����Ȃ珈�����Ȃ�
	if(m_iCurrExeWnd == iExeWnd){
		SetWindowTitle();
		// MainRecipeList�CMultiRecipeList�̏ꍇ�CMeasurementMode�CEditMode�ɉ����ăc�[���o�[�̕\���C��\�����s��
		if ( iExeWnd == RECIPE_MAIN_LIST_WND || iExeWnd == RECIPE_MULTI_LIST_WND )
			((CNanoRecipeListUI*)m_wndSplitter2.GetPane(0, 0))->LayoutToolBar();
		return;
	}

	m_pCurrExeView = NULL;

	m_iCurrExeWnd = iExeWnd;

	CCreateContext Context;

	switch(iExeWnd){
	case MEAS_WND:
		pNewViewClass = RUNTIME_CLASS(CMeasurementTabView);
		break;
	case MANUAL_MEAS_WND:
		pNewViewClass = RUNTIME_CLASS(CMeasurementTabView);
		break;
	case RECIPE_MAIN_LIST_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainListView);
		break;
	case RECIPE_MULTI_LIST_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMultiListView);
		break;
	case RECIPE_MEAS_LIST_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementProgramListView);
		break;
	case RECIPE_STAGE_LIST_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeStageProgramListView);
		break;
	case RECIPE_RECALIBRATION_LIST_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeRecalibrationProgramListView);
		break;
	case RECIPE_POINT_DESKEW_LIST_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipePointDeskewProgramListView);
		break;
	case DATA_LIST_WND:
		pNewViewClass = RUNTIME_CLASS(CDataListView);
		break;
	case RECIPE_MAIN_SR_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		break;
// 2009.09.19 bagus SE --{--
	case RECIPE_MAIN_SE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_SE;
		break;
// 2009.09.19 bagus SE --}--
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case RECIPE_MAIN_COMPEASE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_COMPEASE;
		break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	//2009.08.16 bagus stress --{--
	case RECIPE_MAIN_STRESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_STRESS;
		break;
	//2009.08.16 bagus stress --}--
	// 2009.10.08 bagus CTA �ǉ� --{--
	case RECIPE_MAIN_CTA_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_CTA;
		break;
	// 2009.10.08 bagus CTA �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	case RECIPE_MAIN_MS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_MS;
		break;
	//2009.10.20 bagus MS �ǉ� --}--
	// 2009.11.04 bagus RS �ǉ� --{--
	case RECIPE_MAIN_4PP_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_4PP;
		break;
	// 2009.11.04 bagus RS �ǉ� --}--
	case RECIPE_MULTI_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMultiView);
		break;
	case RECIPE_MEAS_SR_THICKNESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrThicknessProgramView);
		break;
	case RECIPE_MEAS_SR_REFLECTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrReflectanceProgramView);
		break;
	case RECIPE_MEAS_SR_TRANSMITTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrTransmittanceProgramView);
		break;
// 2010.01.07 bagus Gantry --{--
	case RECIPE_MEAS_SR_GANTRY_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrTransmittanceProgramView);
		break;
// 2010.01.07 bagus Gantry --}--
	case RECIPE_MEAS_SR_CIE_REFLECTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrCieReflectanceProgramView);
		break;
	case RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrCieTransmittanceProgramView);
		break;
	case RECIPE_MEAS_SR_OD_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrOdProgramView);
		break;
	//2009.09.03 bagus se --{--
	// SE�����ʂւ̕���ǉ�
	case RECIPE_MEAS_SE_THICKNESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSeThicknessProgramView);
		break;
	//2009.09.03 bagus se --{--
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case RECIPE_MEAS_COMPEASE_THICKNESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementCompEASEThicknessProgramView);
		break;
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	//2009.08.16 bagus stress --{--
	case RECIPE_MEAS_STRESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementStressProgramView);
		break;
	//2009.08.16 bagus stress --}--
	case RECIPE_MEAS_4PP_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementResistanceProgramView);
		break;
	case RECIPE_STAGE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeStageProgramView);
		break;
	//2009.08.28 bagus stress --{--
	case RECIPE_STAGE_STRESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeStressStageProgramView);
		break;
	//2009.08.28 bagus stress --}--
	// 2009.10.02 bagus Gantry --{--
	case RECIPE_STAGE_GANTRY_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeGantryStageProgramView);
		break;
	// 2009.10.02 bagus Gantry --}--
	// 2009.10.13 bagus Distance �ǉ� --{--
	case RECIPE_STAGE_SR_DISTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeDistanceStageProgramView);
		break;
	// 2009.10.13 bagus Distance �ǉ� --}--
	// 2009.10.07 bagus CTA �ǉ� --{--
	case RECIPE_MEAS_CTA_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementCTAProgramView);
		break;
	// 2009.10.07 bagus CTA �ǉ� --}--
	// 2009.10.13 bagus Distance �ǉ� --{--
	case RECIPE_MEAS_SR_DISTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrDistanceProgramView);
		break;
	// 2009.10.13 bagus Distance �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	case RECIPE_MEAS_MS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementMSProgramView);
		break;
	//2009.10.20 bagus MS �ǉ� --}--
	case RECIPE_RECALIBRATION_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeRecalibrationProgramView);
		break;
	case RECIPE_POINT_DESKEW_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipePointDeskewProgramView);
		break;
	case DATA_SR_WND:
		pNewViewClass = RUNTIME_CLASS(CDataReviewView);
		break;
	case USERCONF_WND:
		pNewViewClass = RUNTIME_CLASS(CUserSettingView);
		break;
	case USERCONF_DEF_MAIN_RECIPE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		break;
	case USERCONF_DEF_SR_THICKNESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrThicknessProgramView);
		break;
	case USERCONF_DEF_SR_REFLECTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrReflectanceProgramView);
		break;
	case USERCONF_DEF_SR_TRANSMITTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrTransmittanceProgramView);
		break;
	case USERCONF_DEF_SR_OD_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrOdProgramView);
		break;
	case USERCONF_DEF_SR_CIE_REFLECTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrCieReflectanceProgramView);
		break;
	case USERCONF_DEF_SR_CIE_TRANSMITTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrCieTransmittanceProgramView);
		break;
	case USERCONF_DEF_SR_4PP_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementResistanceProgramView);
		break;
	// 2009.09.29 bagus SE --{--
	case USERCONF_DEF_SE_MAIN_RECIPE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_SE;
		break;
	// 2009.09.29 bagus SE --}--
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case USERCONF_DEF_COMPEASE_MAIN_RECIPE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_COMPEASE;
		break;
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	// 2009.10.08 bagus CTA �ǉ� --{--
	case USERCONF_DEF_CTA_MAIN_RECIPE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_CTA;
		break;
	// 2009.10.08 bagus CTA �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	case USERCONF_DEF_MS_MAIN_RECIPE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_MS;
		break;
	//2009.10.20 bagus MS �ǉ� --}--
	// 2009.11.04 bagus RS �ǉ� --{--
	case USERCONF_DEF_4PP_MAIN_RECIPE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_4PP;
		break;
	// 2009.11.04 bagus RS �ǉ� --}--
	//2009.10.21 bagus Stress �ǉ� --{--
	case USERCONF_DEF_STRESS_MAIN_RECIPE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMainSrView);
		iNewRecipeHeadType = HEAD_TYPE_STRESS;
		break;
	//2009.10.21 bagus Stress �ǉ� --}--
	//2009.09.07 bagus se --{--
	case USERCONF_DEF_SE_THICKNESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSeThicknessProgramView);
		break;
	//2009.09.07 bagus se --}--
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case USERCONF_DEF_COMPEASE_THICKNESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementCompEASEThicknessProgramView);
		break;
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	// 2009.10.08 bagus CTA �ǉ� --{--
	case USERCONF_DEF_CTA_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementCTAProgramView);
		break;
	// 2009.10.08 bagus CTA �ǉ� --}--
	// 2009.10.13 bagus Distance �ǉ� --{--
	case USERCONF_DEF_SR_DISTANCE_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementSrDistanceProgramView);
		break;
	// 2009.10.13 bagus Distance �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	case USERCONF_DEF_MS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementMSProgramView);
		break;
	//2009.10.20 bagus MS �ǉ� --}--
	// 2009.11.04 bagus RS �ǉ� --{--
	case USERCONF_DEF_4PP_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementResistanceProgramView);
		break;
	// 2009.11.04 bagus RS �ǉ� --}--
	//2009.10.21 bagus Stress �ǉ� --{--
	case USERCONF_DEF_STRESS_WND:
		pNewViewClass = RUNTIME_CLASS(CRecipeMeasurementStressProgramView);
		break;
	//2009.10.21 bagus Stress �ǉ� --}--
	case MAINTE_WND:
		pNewViewClass = RUNTIME_CLASS(CMaintenanceView);
		break;
	case SYSCONF_WND:
		pNewViewClass = RUNTIME_CLASS(CSystemView);
		break;
	case SYSCONF_Z_AXIS_OFFSET_MEAS_WND:
		pNewViewClass = RUNTIME_CLASS(CMeasurementTabView);
		break;
	case ALARM_HISTORY_LIST_WND:
		pNewViewClass = RUNTIME_CLASS(CAlarmHistoryListView);
		break;
	case MANUAL_MEAS_FORM_WND:
		pNewViewClass = RUNTIME_CLASS(CMeasurementTabView);
		break;
	default:
		m_iCurrExeWnd = iExeWnd;
		return;
		break;
	}

	m_wndSplitter2.SetActivePane(0, 0);

	Context.m_pNewViewClass 	= pNewViewClass;
	Context.m_pCurrentDoc		= GetActiveDocument();
	Context.m_pCurrentFrame 	= this;
	Context.m_pNewDocTemplate	= Context.m_pCurrentDoc->GetDocTemplate();
	Context.m_pLastView 		= (CView *)m_wndSplitter2.GetPane(0, 0);

	BOOL bRet;
	m_wndSplitter2.DeleteView(0, 0);

	CRect rcDesktop;
	GetDesktopWindow()->GetClientRect(rcDesktop);
/* modified 2015.03.18 hmenjo FWXGA �Ή� ---------- { ---------- */
//	if ( rcDesktop == CRect(0, 0, 1024, 768) ) {
//		bRet = m_wndSplitter2.CreateView(0, 0, pNewViewClass, CSize(0, 600), &Context);
/* modified 2015.03.18 hmenjo FWXGA �Ή� ----------              */
	if (rcDesktop.bottom < 1024) {
		/* SXGA �̏c�������̏ꍇ	*/
		bRet = m_wndSplitter2.CreateView(0, 0, pNewViewClass, CSize(0, rcDesktop.bottom - (768 - 600)), &Context);
/* modified 2015.03.18 hmenjo FWXGA �Ή� ---------- } ---------- */
	}
	else {
		bRet = m_wndSplitter2.CreateView(0, 0, pNewViewClass, CSize(0, SPLIT_EXEWIN_HEIGHT), &Context);
	}

	CView *pView = (CView *)m_wndSplitter2.GetPane(0, 0);
	if(pView == NULL){
		TRACE("ERR=%ld",GetLastError());
		return;
	}
	pView->GetParentFrame()->RecalcLayout();
	m_wndSplitter2.RecalcLayout();
	//2009.08.28 bagus stress --{--
	//��ʂ�SR/STRESS���p�Ȃ̂łǂ�����Ӑ}���ČĂяo���ꂽ����n���Ă���


// 2009.09.29 bagus SE --{--
#if 0
// 2009.09.19 bagus SE --{--
//	if(iExeWnd == RECIPE_MAIN_SR_WND
//	|| iExeWnd == RECIPE_MAIN_STRESS_WND){
	if(iExeWnd == RECIPE_MAIN_SR_WND
	|| iExeWnd == RECIPE_MAIN_SE_WND
	|| iExeWnd == RECIPE_MAIN_STRESS_WND){
// 2009.09.19 bagus SE --}--
		((CRecipeMainSrView *)pView)->m_iNewHeadType = iNewRecipeHeadType;
	}
#else
	switch (iExeWnd)
	{
	case RECIPE_MAIN_SR_WND:
	case RECIPE_MAIN_SE_WND:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case RECIPE_MAIN_COMPEASE_WND:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	case RECIPE_MAIN_STRESS_WND:
	//2009.10.21 bagus Stress �ǉ� --{--
	case USERCONF_DEF_STRESS_MAIN_RECIPE_WND:
	//2009.10.21 bagus Stress �ǉ� --}--
	case USERCONF_DEF_MAIN_RECIPE_WND:
	case USERCONF_DEF_SE_MAIN_RECIPE_WND:
	// 2009.10.08 bagus CTA �ǉ� --{--
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case USERCONF_DEF_COMPEASE_MAIN_RECIPE_WND:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	case RECIPE_MAIN_CTA_WND:
	case USERCONF_DEF_CTA_MAIN_RECIPE_WND:
	// 2009.10.08 bagus CTA �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	case RECIPE_MAIN_MS_WND:
	case USERCONF_DEF_MS_MAIN_RECIPE_WND:
	//2009.10.20 bagus MS �ǉ� --}--
	// 2009.11.04 bagus RS �ǉ� --{--
	case RECIPE_MAIN_4PP_WND:
	case USERCONF_DEF_4PP_MAIN_RECIPE_WND:
	// 2009.11.04 bagus RS �ǉ� --}--
		((CRecipeMainSrView *)pView)->m_iNewHeadType = iNewRecipeHeadType;
		break;
// 2010.01.07 bagus Gantry --{--
	case RECIPE_MEAS_SR_THICKNESS_WND:
		break;
	case RECIPE_MEAS_SR_REFLECTANCE_WND:
		break;
	case RECIPE_MEAS_SR_TRANSMITTANCE_WND:
		((CRecipeMeasurementSrTransmittanceProgramView *)pView)->m_iNewHeadType = HEAD_TYPE_SR;
		((CRecipeMeasurementSrTransmittanceProgramView *)pView)->m_iNewScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE;
		break;
	case RECIPE_MEAS_SR_GANTRY_WND:
		((CRecipeMeasurementSrTransmittanceProgramView *)pView)->m_iNewHeadType = HEAD_TYPE_SR;
		((CRecipeMeasurementSrTransmittanceProgramView *)pView)->m_iNewScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE_G;
		break;
	case RECIPE_MEAS_SR_CIE_REFLECTANCE_WND:
		break;
	case RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND:
		break;
	case RECIPE_MEAS_SR_OD_WND:
		break;
	case RECIPE_MEAS_SE_THICKNESS_WND:
		break;
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case RECIPE_MEAS_COMPEASE_THICKNESS_WND:
		break;
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
//	case RECIPE_MEAS_IRSE_THICKNESS_WND:
//		break;
	case RECIPE_MEAS_MS_WND:
		break;
	case RECIPE_MEAS_STRESS_WND:
		break;
	case RECIPE_MEAS_4PP_WND:
		break;
	case RECIPE_MEAS_CTA_WND:
		break;
	case RECIPE_MEAS_SR_DISTANCE_WND:
		break;
// 2010.01.07 bagus Gantry --}--
	default:
		break;
	}
#endif
// 2009.09.29 bagus SE --}--

	//2009.08.28 bagus stress --}--
	pView->OnInitialUpdate();
	m_wndSplitter2.SetActivePane(0, 0);

	ChangeMenu();
	SetWindowTitle();

	m_pCurrExeView = pView;
}

/////////////////////////////////////////////////////////////////////////////
// ChangeMenu ��ʂɉ��������j���[�o�[��ݒ肷��
void CMainFrame::ChangeMenu()
{
	typedef struct _MENUSEARCH {
		int iCurrMode;
		int iCurrExeWnd;
		UINT nIDResource;
	} MENUSEARCH, *PMENUSEARCH;

	MENUSEARCH MenuSearchTable[] =
	{
		{ OP_MEASUREMENT,					ANY_EXEWND, 							IDR_MEASUREMENT_MENU					},
		{ OP_MEASUREMENT_PROCESS,			ANY_EXEWND, 							IDR_MEASUREMENT_SEQ_MENU				},
		{ OP_MANUAL_MEASUREMENT_PROCESS,	ANY_EXEWND, 							NULL									},
		{ OP_RECIPE_SETTING,				RECIPE_MAIN_SR_WND, 					IDR_EDIT_RECIPE_MENU					},
		// 2009.09.19 bagus SE --{--
		{ OP_RECIPE_SETTING,				RECIPE_MAIN_SE_WND, 					IDR_EDIT_RECIPE_MENU					},
		// 2009.09.19 bagus SE --}--
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		{ OP_RECIPE_SETTING,				RECIPE_MAIN_COMPEASE_WND, 				IDR_EDIT_RECIPE_MENU					},
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		// 2009.09.04 bagus stress MainRecipeStress�ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_MAIN_STRESS_WND, 				IDR_EDIT_RECIPE_MENU					},
		// 2009.09.04 bagus stress MainRecipeStress�ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_MULTI_WND,						IDR_EDIT_RECIPE_MENU					},
		{ OP_RECIPE_SETTING,				RECIPE_STAGE_WND,						IDR_EDIT_RECIPE_MENU					},
		// 2009.09.04 bagus stress Stage Stress�ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_STAGE_STRESS_WND,				IDR_EDIT_RECIPE_MENU					},
		// 2009.09.04 bagus stress Stage Stress�ǉ� --}--
		// 2009.10.02 bagus Gantry --{--
		{ OP_RECIPE_SETTING,				RECIPE_STAGE_GANTRY_WND,				IDR_EDIT_RECIPE_MENU					},
		// 2009.10.02 bagus Gantry --}--
		// 2009.10.13 bagus Distance �ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_STAGE_SR_DISTANCE_WND,			IDR_EDIT_RECIPE_MENU					},
		// 2009.10.13 bagus Distance �ǉ� --}--
		// 2009.10.08 bagus CTA �ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_MAIN_CTA_WND,					IDR_EDIT_RECIPE_MENU					},
		// 2009.10.08 bagus CTA �ǉ� --}--
		//2009.10.20 bagus MS �ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_MAIN_MS_WND, 					IDR_EDIT_RECIPE_MENU					},
		//2009.10.20 bagus MS �ǉ� --}--
		// 2009.11.04 bagus RS �ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_MAIN_4PP_WND,					IDR_EDIT_RECIPE_MENU					},
		// 2009.11.04 bagus RS �ǉ� --}--
		{ OP_RECIPE_SETTING,				RECIPE_RECALIBRATION_WND,				IDR_EDIT_RECIPE_MENU					},
		{ OP_RECIPE_SETTING,				RECIPE_POINT_DESKEW_WND,				IDR_EDIT_RECIPE_MENU					},
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SR_THICKNESS_WND,			IDR_EDIT_RECIPE_MENU					},
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SR_REFLECTANCE_WND, 		IDR_EDIT_RECIPE_MENU					},
		// 2010.01.07 bagus Gantry --{--
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SR_GANTRY_WND,				IDR_EDIT_RECIPE_MENU					},
		// 2010.01.07 bagus Gantry --}--
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SR_TRANSMITTANCE_WND,		IDR_EDIT_RECIPE_MENU					},
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SR_CIE_REFLECTANCE_WND, 	IDR_EDIT_RECIPE_MENU					},
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND,	IDR_EDIT_RECIPE_MENU					},
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SR_OD_WND,					IDR_EDIT_RECIPE_MENU					},
		//2009.09.01 bagus stress --{--
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_STRESS_WND, 				IDR_EDIT_RECIPE_MENU					},
		//2009.09.01 bagus stress --}--
		//2009.09.03 bagus se --{--
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SE_THICKNESS_WND,			IDR_EDIT_RECIPE_MENU					},
		//2009.09.03 bagus se --}--
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_COMPEASE_THICKNESS_WND,		IDR_EDIT_RECIPE_MENU					},
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		// 2009.10.07 bagus CTA �ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_CTA_WND,					IDR_EDIT_RECIPE_MENU					},
		// 2009.10.07 bagus CTA �ǉ� --}--
		// 2009.10.13 bagus Distance �ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_SR_DISTANCE_WND,			IDR_EDIT_RECIPE_MENU					},
		// 2009.10.13 bagus Distance �ǉ� --}--
		//2009.10.20 bagus MS �ǉ� --{--
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_MS_WND, 					IDR_EDIT_RECIPE_MENU					},
		//2009.10.20 bagus MS �ǉ� --}--
		{ OP_RECIPE_SETTING,				RECIPE_MEAS_4PP_WND,					IDR_EDIT_RECIPE_MENU					},
		{ OP_RECIPE_SETTING,				ANY_EXEWND, 							IDR_RECIPE_SETTING_MENU 				},
		{ OP_DATA,							ANY_EXEWND, 							IDR_DATA_MENU							},
		{ OP_DATA_REVIEW,					ANY_EXEWND, 							NULL									},
		{ OP_USER_SETTING,					USERCONF_DEF_MAIN_RECIPE_WND,			IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_SR_THICKNESS_WND,			IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_SR_REFLECTANCE_WND,		IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_SR_TRANSMITTANCE_WND,		IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_SR_CIE_REFLECTANCE_WND,	IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_SR_CIE_TRANSMITTANCE_WND,	IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_SR_OD_WND, 				IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_SR_4PP_WND,				IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		//2009.09.07 bagus se --{--
		{ OP_USER_SETTING,					USERCONF_DEF_SE_THICKNESS_WND,			IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		//2009.09.07 bagus se --}--
		// 2009.09.29 bagus SE --{--
		{ OP_USER_SETTING,					USERCONF_DEF_SE_MAIN_RECIPE_WND,		IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		// 2009.09.29 bagus SE --}--
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		{ OP_USER_SETTING,					USERCONF_DEF_COMPEASE_THICKNESS_WND,	IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_COMPEASE_MAIN_RECIPE_WND,	IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		// 2009.10.08 bagus CTA �ǉ� --{--
		{ OP_USER_SETTING,					USERCONF_DEF_CTA_MAIN_RECIPE_WND,		IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_CTA_WND,					IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		// 2009.10.08 bagus CTA �ǉ� --}--
		// 2009.10.13 bagus Distance �ǉ� --{--
		{ OP_USER_SETTING,					USERCONF_DEF_SR_DISTANCE_WND,			IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		// 2009.10.13 bagus Distance �ǉ� --}--
		//2009.10.20 bagus MS �ǉ� --{--
		{ OP_USER_SETTING,					USERCONF_DEF_MS_MAIN_RECIPE_WND,		IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_MS_WND,					IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		//2009.10.20 bagus MS �ǉ� --}--
		//2009.10.21 bagus Stress �ǉ� --{--
		{ OP_USER_SETTING,					USERCONF_DEF_STRESS_MAIN_RECIPE_WND,	IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_STRESS_WND,				IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		//2009.10.21 bagus Stress �ǉ� --}--
		// 2009.11.04 bagus RS �ǉ� --{--
		{ OP_USER_SETTING,					USERCONF_DEF_4PP_MAIN_RECIPE_WND,		IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		{ OP_USER_SETTING,					USERCONF_DEF_4PP_WND,					IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU	},
		// 2009.11.04 bagus RS �ǉ� --}--
		{ OP_USER_SETTING,					ANY_EXEWND, 							IDR_USER_SETTING_MENU					},
		{ OP_MAINTENANCE,					ANY_EXEWND, 							IDR_MAINTENANCE_MENU					},
		{ OP_SYSTEM,						ANY_EXEWND, 							IDR_SYSTEM_MENU 						},
		{ OP_ALARM_HISTORY, 				ANY_EXEWND, 							NULL									},
		{ OP_MANUAL_MEASUREMENT_FORM,		ANY_EXEWND, 							IDR_MEASUREMENT_MANUAL_MENU 			},
	};

	PMENUSEARCH pMenuSearch = MenuSearchTable;
	UINT nTableCntMax = sizeof(MenuSearchTable) / sizeof(MenuSearchTable[0]);
	UINT nIDResource = NULL;

	for(int i = 0 ; i < nTableCntMax ; i++ ){
		if(pMenuSearch->iCurrMode == m_iCurrMode){
			if(pMenuSearch->iCurrExeWnd != ANY_EXEWND){
				if(pMenuSearch->iCurrExeWnd == m_iCurrExeWnd){
					nIDResource = pMenuSearch->nIDResource;
					break;
				}
			}
			else{
				nIDResource = pMenuSearch->nIDResource;
				break;
			}
		}
		pMenuSearch++;
	}

	if(!nIDResource) return;

	//�V�X�e���R���t�B�O�ݒ�ǂݍ���
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	// ���j���[�ύX
	CMenu mnu;
	mnu.LoadMenu(nIDResource);
	mnu.DeleteMenu(IDM_LOGON, MF_BYCOMMAND);
// 2013.11.11 Bagus Add (TohoSpec�Ή�) -->
	if(g_lModelType == MODEL_T3100){
		if(nIDResource == IDR_MEASUREMENT_MANUAL_MENU){
			mnu.DeleteMenu(2, MF_BYPOSITION );

			CMenu* pSubMenu = mnu.GetSubMenu(0);
/* deleted 2014.05.22 hmenjo �蓮�� NanoMap ---------- { ---------- */
//			pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);
/* deleted 2014.05.22 hmenjo �蓮�� NanoMap ---------- } ---------- */

			pSubMenu = mnu.GetSubMenu(1);
			pSubMenu->DeleteMenu(IDM_POINT_TEACHING, MF_BYCOMMAND);
		}
		else if(nIDResource == IDR_MEASUREMENT_MENU){
			mnu.DeleteMenu(2, MF_BYPOSITION);

			CMenu* pSubMenu = mnu.GetSubMenu(0);
/* deleted 2014.05.22 hmenjo �蓮�� NanoMap ---------- { ---------- */
//			pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);
/* deleted 2014.05.22 hmenjo �蓮�� NanoMap ---------- } ---------- */

			pSubMenu = mnu.GetSubMenu(1);

			CString str;
			pSubMenu->GetMenuString(IDM_MEASUREMENT_START, str, MF_BYCOMMAND);
			pSubMenu->ModifyMenu(IDM_MANUAL_MEASUREMENT, MF_BYCOMMAND, IDM_MANUAL_MEASUREMENT, str);

			pSubMenu->DeleteMenu(IDM_MEASUREMENT_START, MF_BYCOMMAND);
			pSubMenu->DeleteMenu(IDM_HANDLER_LOAD_POSITION, MF_BYCOMMAND);
			pSubMenu->DeleteMenu(IDM_REFERENCE, MF_BYCOMMAND);
			pSubMenu->DeleteMenu(IDM_VACUUM, MF_BYCOMMAND);
			pSubMenu->DeleteMenu(IDM_MANUAL_STAGE, MF_BYCOMMAND);

			pSubMenu->DeleteMenu(1, MF_BYPOSITION);
		}
		else if(nIDResource == IDR_MEASUREMENT_SEQ_MENU){
			mnu.DeleteMenu(2, MF_BYPOSITION);

/* deleted 2014.05.22 hmenjo �蓮�� NanoMap ---------- { ---------- */
//			CMenu* pSubMenu = mnu.GetSubMenu(0);
//			pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);
/* deleted 2014.05.22 hmenjo �蓮�� NanoMap ---------- } ---------- */
		}
		else if(nIDResource == IDR_DATA_MENU ||
			nIDResource == IDR_EDIT_RECIPE_MENU ||
			nIDResource == IDR_MAINTENANCE_MENU ||
			nIDResource == IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU ||
			nIDResource == IDR_RECIPE_SETTING_MENU ||
			nIDResource == IDR_SYSTEM_MENU ||
			nIDResource == IDR_USER_SETTING_MENU){
/* deleted 2014.05.22 hmenjo �蓮�� NanoMap ---------- { ---------- */
//			CMenu* pSubMenu = mnu.GetSubMenu(0);
//			pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);
/* deleted 2014.05.22 hmenjo �蓮�� NanoMap ---------- } ---------- */
		}
	}
	else{
// 2013.11.11 Bagus Add (TohoSpec�Ή�) <--
// 2014.01.17 Bagus Add (Stage None�Ή�) -->
		if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
			if(nIDResource == IDR_MEASUREMENT_MANUAL_MENU){
				if(!m_SystemConfig.bHost){
					mnu.DeleteMenu(2, MF_BYPOSITION );
				}

				CMenu* pSubMenu = mnu.GetSubMenu(0);
				pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);

				pSubMenu = mnu.GetSubMenu(1);
				pSubMenu->DeleteMenu(IDM_POINT_TEACHING, MF_BYCOMMAND);
			}
			else if(nIDResource == IDR_MEASUREMENT_MENU){
				if(!m_SystemConfig.bHost){
					mnu.DeleteMenu(2, MF_BYPOSITION);
				}

				CMenu* pSubMenu = mnu.GetSubMenu(0);
				pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);

				pSubMenu = mnu.GetSubMenu(1);

				CString str;
				pSubMenu->GetMenuString(IDM_MEASUREMENT_START, str, MF_BYCOMMAND);
				pSubMenu->ModifyMenu(IDM_MANUAL_MEASUREMENT, MF_BYCOMMAND, IDM_MANUAL_MEASUREMENT, str);

				pSubMenu->DeleteMenu(IDM_MEASUREMENT_START, MF_BYCOMMAND);
				pSubMenu->DeleteMenu(IDM_HANDLER_LOAD_POSITION, MF_BYCOMMAND);
				pSubMenu->DeleteMenu(IDM_REFERENCE, MF_BYCOMMAND);
				pSubMenu->DeleteMenu(IDM_VACUUM, MF_BYCOMMAND);
				pSubMenu->DeleteMenu(IDM_MANUAL_STAGE, MF_BYCOMMAND);

				pSubMenu->DeleteMenu(1, MF_BYPOSITION);
			}
			else if(nIDResource == IDR_MEASUREMENT_SEQ_MENU){
				if(!m_SystemConfig.bHost){
					mnu.DeleteMenu(2, MF_BYPOSITION);
				}

				CMenu* pSubMenu = mnu.GetSubMenu(0);
				pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);
			}
			else if(nIDResource == IDR_DATA_MENU ||
					nIDResource == IDR_EDIT_RECIPE_MENU ||
					nIDResource == IDR_MAINTENANCE_MENU ||
					nIDResource == IDR_RECIPE_DEFAULT_VALUE_SETTING_MENU ||
					nIDResource == IDR_RECIPE_SETTING_MENU ||
					nIDResource == IDR_SYSTEM_MENU ||
					nIDResource == IDR_USER_SETTING_MENU){
				CMenu* pSubMenu = mnu.GetSubMenu(0);
				pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);
			}
		}
		else{
			if(nIDResource == IDR_MEASUREMENT_MENU && !m_SystemConfig.bHost){
				mnu.DeleteMenu(2, MF_BYPOSITION );
			}
		}
// 2013.11.11 Bagus Add (TohoSpec�Ή�) <--
// 2014.01.17 Bagus Add (Stage None�Ή�) <--
	}
// 2013.11.11 Bagus Add (TohoSpec�Ή�) <--

	SetMenu(NULL);
	::DestroyMenu(m_hMenuDefault);
	SetMenu(&mnu);
	DrawMenuBar();
	m_hMenuDefault = mnu.GetSafeHmenu();
}

/////////////////////////////////////////////////////////////////////////////
// SetWindowTitle ��ʂɉ������^�C�g����ҏW����
void CMainFrame::SetWindowTitle()
{
	TRACE(_T("CMainFrame::SetWindowTitle()\n"));

	typedef struct _TITLESEARCH{
		int iCurrExeWnd;
		int iCurrMode;
		LPCTSTR szTitle;
	}TITLESEARCH, *PTITLESEARCH;

	TITLESEARCH TitleSearchTable[] =
	{
		{ MEAS_WND, 							ANY_MODE,			"NanoSpec - MEASUREMENT"												},
		{ MANUAL_RECIPE_WND,					ANY_MODE,			"NanoSpec - MANUAL MEASUREMENT - MAIN RECIPE"						},
		{ MANUAL_MEAS_WND,						ANY_MODE,			"NanoSpec - MANUAL MEASUREMENT" 										},
		{ RECIPE_MAIN_LIST_WND, 				OP_MEASUREMENT, 	"NanoSpec - MEASUREMENT - MAIN RECIPE LIST" 							},
		{ RECIPE_MAIN_LIST_WND, 				ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE LIST"							},
		{ RECIPE_MULTI_LIST_WND,				OP_MEASUREMENT, 	"NanoSpec - MEASUREMENT - MULTI RECIPE LIST"							},
		{ RECIPE_MULTI_LIST_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING MODE - MULTI RECIPE LIST"					},
		{ RECIPE_MEAS_LIST_WND, 				ANY_MODE,			"NanoSpec - RECIPE SETTING - MEASUREMENT PROGRAM LIST"					},
		{ RECIPE_STAGE_LIST_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING - STAGE PROGRAM LIST"						},
		{ RECIPE_RECALIBRATION_LIST_WND,		ANY_MODE,			"NanoSpec - RECIPE SETTING - RECALIBRATION PROGRAM LIST"				},
		{ RECIPE_POINT_DESKEW_LIST_WND, 		ANY_MODE,			"NanoSpec - RECIPE SETTING - POINT DESKEW PROGRAM LIST" 				},
		{ RECIPE_MAIN_SR_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE"								},
		{ RECIPE_MAIN_SE_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE"								},
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		{ RECIPE_MAIN_COMPEASE_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE"								},
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.19 bagus MS �ǉ� --{--
//		{ RECIPE_MAIN_IRSE_WND, 				ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE"								},
		{ RECIPE_MAIN_MS_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE"								},
// 2009.10.19 bagus MS �ǉ� --}--
		{ RECIPE_MAIN_STRESS_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE"								},
		{ RECIPE_MAIN_4PP_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE"								},
		{ RECIPE_MAIN_CTA_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - MAIN RECIPE"								},
		{ RECIPE_MULTI_WND, 					ANY_MODE,			"NanoSpec - RECIPE SETTING - MULTI RECIPE"								},
		{ RECIPE_MEAS_SR_THICKNESS_WND, 		ANY_MODE,			"NanoSpec - RECIPE SETTING - SR THICKNESS"								},
		{ RECIPE_MEAS_SR_REFLECTANCE_WND,		ANY_MODE,			"NanoSpec - RECIPE SETTING - SR REFLECTANCE"							},
		{ RECIPE_MEAS_SR_TRANSMITTANCE_WND, 	ANY_MODE,			"NanoSpec - RECIPE SETTING - SR TRANSMITTANCE"							},
// 2010.01.07 bagus Gantry --{--
		{ RECIPE_MEAS_SR_GANTRY_WND,			ANY_MODE,			"NanoSpec - RECIPE SETTING - SR TRANSMITTANCE"							  },
// 2010.01.07 bagus Gantry --}--
		{ RECIPE_MEAS_SR_CIE_REFLECTANCE_WND,	ANY_MODE,			"NanoSpec - RECIPE SETTING - SR REFLECTANCE CIE"						},
		{ RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND, ANY_MODE,			"NanoSpec - RECIPE SETTING - SR TRANSMITTANCE CIE"						},
		{ RECIPE_MEAS_SR_OD_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING - SR OPTICAL DENSITY"						},
		//2009.08.16 bagus stress --{--
		{ RECIPE_MEAS_STRESS_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING - STRESS"									},
		//2009.08.16 bagus stress --}--
		//2009.08.16 bagus stress --{--
		{ RECIPE_MEAS_SE_THICKNESS_WND, 		ANY_MODE,			"NanoSpec - RECIPE SETTING - SE THICKNESS"								},
		//2009.08.16 bagus stress --}--
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		{ RECIPE_MEAS_COMPEASE_THICKNESS_WND, 	ANY_MODE,			"NanoSpec - RECIPE SETTING - EASE THICKNESS"							},
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		// 2009.11.04 bagus RS �ǉ� --{--
		//{ RECIPE_MEAS_4PP_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - 4PP"										},
		{ RECIPE_MEAS_4PP_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - RESISTANCE"								},
		// 2009.11.04 bagus RS �ǉ� --}--
		// 2009.10.07 bagus CTA �ǉ� --{--
		{ RECIPE_MEAS_CTA_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - CONTACT ANGLE" 							},
		// 2009.10.07 bagus CTA �ǉ� --}--
		// 2009.10.13 bagus Distance �ǉ� --{--
		{ RECIPE_MEAS_SR_DISTANCE_WND,			ANY_MODE,			"NanoSpec - RECIPE SETTING - SR DISTANCE"								},
		// 2009.10.13 bagus Distance �ǉ� --}--
		{ RECIPE_STAGE_WND, 					ANY_MODE,			"NanoSpec - RECIPE SETTING - STAGE PROGRAM" 							},
		{ RECIPE_STAGE_STRESS_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING - STAGE PROGRAM STRESS"						},
		// 2009.10.02 bagus Gantry --{--
		{ RECIPE_STAGE_GANTRY_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING - STAGE PROGRAM GANTRY"						},
		// 2009.10.02 bagus Gantry --}--
		// 2009.10.13 bagus Distance �ǉ� --{--
		{ RECIPE_STAGE_SR_DISTANCE_WND, 		ANY_MODE,			"NanoSpec - RECIPE SETTING - STAGE PROGRAM DISTANCE"					},
		// 2009.10.13 bagus Distance �ǉ� --}--
		//2009.10.20 bagus MS �ǉ� --{--
		{ RECIPE_MEAS_MS_WND,					ANY_MODE,			"NanoSpec - RECIPE SETTING - MICRO SCOPE"								},
		//2009.10.20 bagus MS �ǉ� --}--
		{ RECIPE_RECALIBRATION_WND, 			ANY_MODE,			"NanoSpec - RECIPE SETTING - RECALIBRATION PROGRAM" 					},
		{ RECIPE_POINT_DESKEW_WND,				ANY_MODE,			"NanoSpec - RECIPE SETTING - POINT DESKEW PROGRAM"						},
		{ DATA_LIST_WND,						ANY_MODE,			"NanoSpec - MEASUREMENT DATA LIST"										},
		{ DATA_SR_WND,							ANY_MODE,			"NanoSpec - MEASUREMENT DATA"											},
		{ USERCONF_WND, 						ANY_MODE,			"NanoSpec - USER SETTING"												},
		{ USERCONF_DEF_MAIN_RECIPE_WND, 		ANY_MODE,			"NanoSpec - USER SETTING - MAIN RECIPE DEFAULT VALUE SETTING"			},
		{ USERCONF_DEF_SR_THICKNESS_WND,		ANY_MODE,			"NanoSpec - USER SETTING - SR THICKNESS DEFAULT VALUE SETTING"			},
		{ USERCONF_DEF_SR_REFLECTANCE_WND,		ANY_MODE,			"NanoSpec - USER SETTING - SR REFLECTANCE DEFAULT VALUE SETTING"		},
		{ USERCONF_DEF_SR_TRANSMITTANCE_WND,	ANY_MODE,			"NanoSpec - USER SETTING - SR TRANSMITTANCE DEFAULT VALUE SETTING"		},
		{ USERCONF_DEF_SR_CIE_REFLECTANCE_WND,	ANY_MODE,			"NanoSpec - USER SETTING - SR REFLECTANCE CIE DEFAULT VALUE SETTING"	},
		{ USERCONF_DEF_SR_CIE_TRANSMITTANCE_WND,ANY_MODE,			"NanoSpec - USER SETTING - SR TRANSMITTANCE CIE DEFAULT VALUE SETTING"	},
		{ USERCONF_DEF_SR_OD_WND,				ANY_MODE,			"NanoSpec - USER SETTING - SR OPTICAL DENSITY DEFAULT VALUE SETTING"	},
		// 2009.11.04 bagus RS �ǉ� --{--
		//{ USERCONF_DEF_SR_4PP_WND,				ANY_MODE,			"NanoSpec - USER SETTING - 4PP DEFAULT VALUE SETTING"					},
		{ USERCONF_DEF_SR_4PP_WND,				ANY_MODE,			"NanoSpec - USER SETTING - RESISTANCE DEFAULT VALUE SETTING"			},
		// 2009.11.04 bagus RS �ǉ� --}--
		//2009.09.07 bagus se --{--
		{ USERCONF_DEF_SE_THICKNESS_WND,		ANY_MODE,			"NanoSpec - USER SETTING - SE THICKNESS DEFAULT VALUE SETTING"			},
		//2009.09.07 bagus se --}--
		// 2009.09.29 bagus SE --{--
		{ USERCONF_DEF_SE_MAIN_RECIPE_WND,		ANY_MODE,			"NanoSpec - USER SETTING - MAIN RECIPE DEFAULT VALUE SETTING"			},
		// 2009.09.29 bagus SE --}--
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		{ USERCONF_DEF_COMPEASE_THICKNESS_WND,	ANY_MODE,			"NanoSpec - USER SETTING - EASE THICKNESS DEFAULT VALUE SETTING"		},
		{ USERCONF_DEF_COMPEASE_MAIN_RECIPE_WND,ANY_MODE,			"NanoSpec - USER SETTING - MAIN RECIPE DEFAULT VALUE SETTING"			},
		// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		// 2009.10.08 bagus CTA �ǉ� --{--
		{ USERCONF_DEF_CTA_MAIN_RECIPE_WND, 	ANY_MODE,			"NanoSpec - USER SETTING - MAIN RECIPE DEFAULT VALUE SETTING"			},
		{ USERCONF_DEF_CTA_WND, 				ANY_MODE,			"NanoSpec - USER SETTING - CONTACT ANGLE DEFAULT VALUE SETTING" 		},
		// 2009.10.08 bagus CTA �ǉ� --}--
		// 2009.10.13 bagus Distance �ǉ� --{--
		{ USERCONF_DEF_SR_DISTANCE_WND, 		ANY_MODE,			"NanoSpec - USER SETTING - SR DISTANCE DEFAULT VALUE SETTING"			},
		// 2009.10.13 bagus Distance �ǉ� --}--
		//2009.10.20 bagus MS �ǉ� --{--
		{ USERCONF_DEF_MS_MAIN_RECIPE_WND,		ANY_MODE,			"NanoSpec - USER SETTING - MAIN RECIPE DEFAULT VALUE SETTING"			},
		{ USERCONF_DEF_MS_WND,					ANY_MODE,			"NanoSpec - USER SETTING - MICRO SCOPE DEFAULT VALUE SETTING"			},
		//2009.10.20 bagus MS �ǉ� --}--
		//2009.10.21 bagus Stress �ǉ� --{--
		{ USERCONF_DEF_STRESS_MAIN_RECIPE_WND,	ANY_MODE,			"NanoSpec - USER SETTING - MAIN RECIPE DEFAULT VALUE SETTING"			},
		{ USERCONF_DEF_STRESS_WND,				ANY_MODE,			"NanoSpec - USER SETTING - STRESS DEFAULT VALUE SETTING"			},
		//2009.10.21 bagus Stress �ǉ� --}--
		// 2009.11.04 bagus RS �ǉ� --{--
		{ USERCONF_DEF_4PP_MAIN_RECIPE_WND, 	ANY_MODE,			"NanoSpec - USER SETTING - MAIN RECIPE DEFAULT VALUE SETTING"			},
		{ USERCONF_DEF_4PP_WND, 				ANY_MODE,			"NanoSpec - USER SETTING - RESISTANCE DEFAULT VALUE SETTING"			},
		// 2009.11.04 bagus RS �ǉ� --}--
		{ MAINTE_WND,							ANY_MODE,			"NanoSpec - SYSTEM CHECK"												},
		{ SYSCONF_WND,							ANY_MODE,			"NanoSpec - SYSTEM CONFIG"												},
		{ SYSCONF_Z_AXIS_OFFSET_MEAS_WND,		ANY_MODE,			"NanoSpec - SYSTEM CONFIG - ZAXIS OFFSET SETTING"						},
		{ ALARM_HISTORY_LIST_WND,				ANY_MODE,			"NanoSpec - ALARM HISTORY"												},
		{ MANUAL_MEAS_FORM_WND, 				ANY_MODE,			"NanoSpec - MANUAL MEASUREMENT" 										},
	};

	PTITLESEARCH pTitleSearch = TitleSearchTable;
	UINT nTableCntMax = sizeof(TitleSearchTable) / sizeof(TitleSearchTable[0]);
	LPCTSTR psz = NULL;

	for(int i = 0 ; i < nTableCntMax ; i++ ){
		if(pTitleSearch->iCurrExeWnd == m_iCurrExeWnd){
			if(pTitleSearch->iCurrMode != ANY_MODE){
				if(pTitleSearch->iCurrMode == m_iCurrMode){
					psz = pTitleSearch->szTitle;
					break;
				}
			}
			else{
				psz = pTitleSearch->szTitle;
				break;
			}
		}
		pTitleSearch++;
	}

	if(!psz){
		ASSERT( FALSE );	// �^�C�g���̓o�^�R��ł��I�I
		return;
	}

	// �^�C�g���ύX
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
// 	char szBuff[256];
// 	if(HardwareSimulation()){
// 		sprintf(szBuff, "%s [HWS]", psz);
// 		SetWindowText(szBuff);
// 	}
// 	else{
// 		SetWindowText(psz);
// 	}

	CString strBuffer(psz);

	if(g_lAppNameType != APP_NAME_NANO){
		strBuffer.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
	}

	if(HardwareSimulation()){
		strBuffer += " [HWS]";
	}

	SetWindowText(strBuffer);
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--
}

/////////////////////////////////////////////////////////////////////////////
//
int CMainFrame::GetCurrOperationPanel()
{
	return m_iCurrMode;
}

/////////////////////////////////////////////////////////////////////////////
//
int CMainFrame::GetCurrExeWnd()
{
	return m_iCurrExeWnd;
}

/////////////////////////////////////////////////////////////////////////////
// GetSelectListName �e���V�s���X�g�ꗗ���ƂɁA�I������Ă������V�s�����擾����
void CMainFrame::GetSelectListName(LPTSTR pszName)
{
	int iIndex;
	iIndex = SelectListNameIndex();
	if ( iIndex == -1 )
		return;
	strcpy(pszName, m_szSelectListName[iIndex]);
}

/////////////////////////////////////////////////////////////////////////////
// SetSelectListName �e���V�s���X�g�ꗗ���ƂɁA�I������Ă��郌�V�s����ۑ�����
void CMainFrame::SetSelectListName(LPCTSTR pszName)
{
	int iIndex;
	iIndex = SelectListNameIndex();
	if ( iIndex == -1 )
		return;
	strcpy(m_szSelectListName[iIndex], pszName);
}

/////////////////////////////////////////////////////////////////////////////
// SelectListNameIndex �e���V�s�ҏW���
// �i�q�j�̃��V�s���X�g�ꗗ�i�e�j�ւ̕R�t���B
// ���V�s���X�g�ꗗ�̃C���f�b�N�X��ԋp����
int CMainFrame::SelectListNameIndex()
{
	CMap<int, int, int, int> wndMap;
	//////// �q�@////////// 						//////// �e�@//////////
	wndMap[MANUAL_MEAS_WND] 						= MANUAL_MEAS_WND;
	wndMap[MEAS_WND]								= RECIPE_MAIN_LIST_WND;
	wndMap[RECIPE_MAIN_LIST_WND]					= RECIPE_MAIN_LIST_WND;
	wndMap[RECIPE_MAIN_SR_WND]						= RECIPE_MAIN_LIST_WND;
	wndMap[RECIPE_MAIN_SE_WND]						= RECIPE_MAIN_LIST_WND;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	wndMap[RECIPE_MAIN_COMPEASE_WND]				= RECIPE_MAIN_LIST_WND;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.19 bagus MS �ǉ� --{--
//	wndMap[RECIPE_MAIN_IRSE_WND]					= RECIPE_MAIN_LIST_WND;
	wndMap[RECIPE_MAIN_MS_WND]						= RECIPE_MAIN_LIST_WND;
// 2009.10.19 bagus MS �ǉ� --}--
	wndMap[RECIPE_MAIN_STRESS_WND]					= RECIPE_MAIN_LIST_WND;
	wndMap[RECIPE_MAIN_4PP_WND] 					= RECIPE_MAIN_LIST_WND;
	wndMap[RECIPE_MAIN_CTA_WND] 					= RECIPE_MAIN_LIST_WND;
	wndMap[RECIPE_MULTI_LIST_WND]					= RECIPE_MULTI_LIST_WND;
	wndMap[RECIPE_MULTI_WND]						= RECIPE_MULTI_LIST_WND;
	wndMap[RECIPE_MEAS_LIST_WND]					= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_THICKNESS_WND]			= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_REFLECTANCE_WND]			= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_TRANSMITTANCE_WND]		= RECIPE_MEAS_LIST_WND;
// 2010.01.07 bagus Gantry --{--
	wndMap[RECIPE_MEAS_SR_GANTRY_WND]				= RECIPE_MEAS_LIST_WND;
// 2010.01.07 bagus Gantry --}--
	wndMap[RECIPE_MEAS_SR_CIE_REFLECTANCE_WND]		= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND]	= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SR_OD_WND]					= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_SE_THICKNESS_WND]			= RECIPE_MEAS_LIST_WND;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	wndMap[RECIPE_MEAS_COMPEASE_THICKNESS_WND]		= RECIPE_MEAS_LIST_WND;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.19 bagus MS �ǉ� --{--
//	wndMap[RECIPE_MEAS_IRSE_THICKNESS_WND]			= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_MS_WND]						= RECIPE_MEAS_LIST_WND;
// 2009.10.19 bagus MS �ǉ� --}--
	wndMap[RECIPE_MEAS_STRESS_WND]					= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_4PP_WND] 					= RECIPE_MEAS_LIST_WND;
	wndMap[RECIPE_MEAS_CTA_WND] 					= RECIPE_MEAS_LIST_WND;
	// 2009.10.14 bagus Distance �ǉ� --{--
	wndMap[RECIPE_MEAS_SR_DISTANCE_WND] 			= RECIPE_MEAS_LIST_WND;
	// 2009.10.14 bagus Distance �ǉ� --}--
	//2009.10.20 bagus MS �ǉ� --{--
	wndMap[RECIPE_MEAS_MS_WND]						= RECIPE_MEAS_LIST_WND;
	//2009.10.20 bagus MS �ǉ� --}--
	wndMap[RECIPE_STAGE_LIST_WND]					= RECIPE_STAGE_LIST_WND;
	wndMap[RECIPE_STAGE_WND]						= RECIPE_STAGE_LIST_WND;
	//2009.08.31 bagus stress --{--
	wndMap[RECIPE_STAGE_STRESS_WND] 				= RECIPE_STAGE_LIST_WND;
	//2009.08.31 bagus stress --}--
	// 2009.10.02 bagus Gantry --{--
	wndMap[RECIPE_STAGE_GANTRY_WND] 				= RECIPE_STAGE_LIST_WND;
	// 2009.10.02 bagus Gantry --}--
	// 2009.10.13 bagus Distance �ǉ� --{--
	wndMap[RECIPE_STAGE_SR_DISTANCE_WND]			= RECIPE_STAGE_LIST_WND;
	// 2009.10.13 bagus Distance �ǉ� --}--
	wndMap[RECIPE_RECALIBRATION_LIST_WND]			= RECIPE_RECALIBRATION_LIST_WND;
	wndMap[RECIPE_RECALIBRATION_WND]				= RECIPE_RECALIBRATION_LIST_WND;
	wndMap[RECIPE_POINT_DESKEW_LIST_WND]			= RECIPE_POINT_DESKEW_LIST_WND;
	wndMap[RECIPE_POINT_DESKEW_WND] 				= RECIPE_POINT_DESKEW_LIST_WND;
	wndMap[DATA_LIST_WND]							= DATA_LIST_WND;
	wndMap[DATA_SR_WND] 							= DATA_LIST_WND;
	wndMap[DATA_SE_WND] 							= DATA_LIST_WND;
// 2009.10.19 bagus MS �ǉ� --{--
//	wndMap[DATA_IRSE_WND]							= DATA_LIST_WND;
	wndMap[DATA_MS_WND] 							= DATA_LIST_WND;
// 2009.10.19 bagus MS �ǉ� --}--
	wndMap[DATA_STRESS_WND] 						= DATA_LIST_WND;
	wndMap[DATA_4PP_WND]							= DATA_LIST_WND;
	wndMap[DATA_CONTACT_ANGLE_WND]					= DATA_LIST_WND;

	int wnd = -1;
	wndMap.Lookup(m_iCurrExeWnd, wnd);

	return wnd;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::GetSelectSampleTypeName(LPTSTR pszName)
{
	strcpy(pszName, m_szSampleTypeName);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::SetSelectSampleTypeName(LPCTSTR pszName)
{
	strcpy(m_szSampleTypeName, pszName);
}

/////////////////////////////////////////////////////////////////////////////
//
int CMainFrame::GetSelectHeadType()
{
	return m_iSelectHeadType;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::SetSelectHeadType(int iHeadType)
{
	m_iSelectHeadType = iHeadType;
}

/////////////////////////////////////////////////////////////////////////////
//
int CMainFrame::GetSelectMeasurementType()
{
	return m_iSelectMeasType;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::SetSelectMeasurementType(int iMeasType)
{
	m_iSelectMeasType = iMeasType;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::GetMethodOfMeasurement()
{
	return m_bTransmittance;
}

// Test --->
/////////////////////////////////////////////////////////////////////////////
// Name 	  : EndSrMeasurement
// Purpose	  : SR����I���̒ʒm
// Parameters : bResult ---> SR���肪����I���������ǂ����̕ϐ�
//
// Returns	  : �Ȃ�
void CMainFrame::EndSrMeasurement(BOOL bResult)
{
	TRACE(bResult ? "CMainFrame::EndSrMeasurement(TRUE)\n" :
					"CMainFrame::EndSrMeasurement(FALSE)\n");
	bResult ? Event(EV_DONE) : Event(EV_ABORT);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : EndSrFocus
// Purpose	  : SR�̃t�H�[�J�X�I���̒ʒm
// Parameters : bResult ---> SR�̃t�H�[�J�X������I���������ǂ����̕ϐ�
//
// Returns	  : �Ȃ�
void CMainFrame::EndSrFocus(BOOL bResult)
{
	TRACE(bResult ? "CMainFrame::EndSrFocus(TRUE)\n" :
					"CMainFrame::EndSrFocus(FALSE)\n");
}
// Test <---

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::HardwareSimulation()
{
	return g_bHWS;
}

/////////////////////////////////////////////////////////////////////////////
//
CView* CMainFrame::GetCurrentExeView()
{
	CView* pView = (CView*)m_wndSplitter2.GetPane(0, 0);

	return pView;
}

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����N�����̃V�X�e���J���[���擾
BOOL CMainFrame::GetOrginalSystemColor()
{
	for (int iIndex = 0 ; iIndex < 50 ; iIndex++ ){
//		if (iIndex != 25){
//			(m_OrgSystemColor[iIndex] = (COLORREF)GetSysColor(iIndex));
//		}
		m_OrgSystemColor[iIndex] = (COLORREF)GetSysColor(iIndex);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����N�����̃V�X�e���J���[��ݒ�
// ADAP�ɂ��V�X�e���J���[�ύX���L�����Z�����邽�߁j
BOOL CMainFrame::SetOrginalSystemColor()
{
	// OnSysColorChange�ōs���̂ŁA�p�~
	return TRUE;

	for (int iIndex = 0 ; iIndex < 50 ; iIndex++ ){
		if (iIndex != 25){
			if(SetSysColors(1,&iIndex,&m_OrgSystemColor[iIndex]) == FALSE){
				return FALSE;
			}
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// �X�e�[�^�X�o�[�\�����e�X�V
void CMainFrame::PutStatusBarUpdate(UINT nIDFind, LPCTSTR str)
{
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
		int n = m_wndStatusBar.CommandToIndex(nIDFind);
		if(n >= 0){
			m_wndStatusBar.SetPaneText(n,str);
		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
}

/////////////////////////////////////////////////////////////////////////////
// Refresh Dialog
void CMainFrame::RefreshDlg(BOOL bInit /* = FALSE */)
{
	int CtrlIndex = 0;
	int ButtonIndex = 0;
	BOOL flag = FALSE;
	CString sBuff;

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	/////////////////////////////////////////////////////////////////////////
	// Screen

	///// Status Bar - Vacuum Sensor /////
	m_bVacuumSensor = nexioIsVacuumOn();
	m_bVacuumValve	= nexioIsVacuumStatus();
	if(m_bVacuumSensor != m_Old_bVacuumSensor || m_bVacuumValve != m_Old_bVacuumValve){
		// Kojika 20090528 Change
		// sBuff = VACUUM_SENSOR_DISP[m_bVacuumSensor];
		if(l_SystemConfig.nLanguage == 0){
			sBuff = VACUUM_SENSOR_DISP_ENU[m_bVacuumValve];
		}
		else{
			sBuff = VACUUM_SENSOR_DISP_JPN[m_bVacuumValve];
		}
		// Kojika 20090528 Change End
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
		if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			m_wndStatusBar.ChangeText(ID_INDICATOR_VACUUM_SENSOR, sBuff);
			m_wndStatusBar.ChangeColor(ID_INDICATOR_VACUUM_SENSOR, VACUUM_SENSOR_TEXT_COLOR[m_bVacuumSensor], m_bVacuumValve && !m_bVacuumSensor ? RED_COLOR : VACUUM_SENSOR_COLOR[m_bVacuumSensor]);
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	}
	m_Old_bVacuumSensor = m_bVacuumSensor;
	m_Old_bVacuumValve	= m_bVacuumValve;

	///// Sample Sensor /////
	m_bSampleSensor = nexioIsGlassExist();
	if(m_bSampleSensor != m_Old_bSampleSensor){
		// Kojika 20090528 Change
		//sBuff = SAMPLE_SENSOR_DISP[m_bSampleSensor];
		if(l_SystemConfig.nLanguage == 0){
			sBuff = SAMPLE_SENSOR_DISP_ENU[m_bSampleSensor];
		}
		else{
			sBuff = SAMPLE_SENSOR_DISP_JPN[m_bSampleSensor];
		}
		// Kojika 20090528 Change End
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
		if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			m_wndStatusBar.ChangeText(ID_INDICATOR_SAMPLE_SENSOR, sBuff);
			m_wndStatusBar.ChangeColor(ID_INDICATOR_SAMPLE_SENSOR, SAMPLE_SENSOR_TEXT_COLOR[m_bSampleSensor], SAMPLE_SENSOR_COLOR[m_bSampleSensor]);
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	}
	m_Old_bSampleSensor = m_bSampleSensor;

	///// Load Position Sensor /////
	m_bStageLoadPos = nexioIsStageLoadPos();
	if(m_bStageLoadPos != m_Old_bStageLoadPos){
		// Kojika 20090528 Change
		//sBuff = LOAD_POSITION_SENSOR_DISP[m_bStageLoadPos];
		if(l_SystemConfig.nLanguage == 0){
			sBuff = LOAD_POSITION_SENSOR_DISP_ENU[m_bStageLoadPos];
		}
		else{
			sBuff = LOAD_POSITION_SENSOR_DISP_JPN[m_bStageLoadPos];
		}
		// Kojika 20090528 Change End
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Add (Stage None�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
		if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None�Ή�) <--
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
			m_wndStatusBar.ChangeText(ID_INDICATOR_LOAD_POSITION_SENSOR, sBuff);
			m_wndStatusBar.ChangeColor(ID_INDICATOR_LOAD_POSITION_SENSOR, LOAD_POSITION_SENSOR_TEXT_COLOR[m_bStageLoadPos], LOAD_POSITION_SENSOR_COLOR[m_bStageLoadPos]);
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	}
	m_Old_bStageLoadPos = m_bStageLoadPos;
}

//2009.11.04 bagus CA --{--
// ADAP����擾������̂Ƃ����łȂ����̂����ʉ�
int CMainFrame::ScanDataLabelGet(WORD wHeadType,char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid)
{
	int nRet = 0;
	switch(wHeadType){
	case HEAD_TYPE_SR:
	case HEAD_TYPE_SE:
	case HEAD_TYPE_STRESS:
		nRet = ScanDataLabelGet_ADAP(szLabel,szLabelOri,dData,iNowPoint,afValid);
		break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case HEAD_TYPE_COMPEASE:
		nRet = ScanDataLabelGet_COMPEASE(szLabel,szLabelOri,dData,iNowPoint,afValid);
		break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	case HEAD_TYPE_MS:
		//�������̏ꍇ�ɂ̓f�[�^�͂Ȃ�
		nRet = 0;
		break;
	case HEAD_TYPE_CTA:
		nRet = ScanDataLabelGet_CA(szLabel,szLabelOri,dData,iNowPoint,afValid);
		break;
	case HEAD_TYPE_4PP:
		nRet = ScanDataLabelGet_RS(szLabel,szLabelOri,dData,iNowPoint,afValid);
		break;
	default:
		nRet = 0;

	}
	return nRet;
}
int CMainFrame::ScanDataLabelGet_RS(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid)
{
	int nLabelCount = 0;
	RESISTRESULT tResult;
	memset(&tResult,NULL,sizeof(tResult));
	m_pDoc->GetOneResistData(&tResult,iNowPoint);

	//2009.11.17 bagus RS --{--
	strncpy(szLabel[nLabelCount],"DATA",ADAPRESULTSTRINGLENMAX);
	nLabelCount++;
	//2009.11.17 bagus RS --}--
	strncpy(szLabel[nLabelCount],MEAS_LIST_RS_ITEM[nLabelCount-1],ADAPRESULTSTRINGLENMAX);
	dData[nLabelCount-1] = tResult.dResistance;
	nLabelCount++;
	strncpy(szLabel[nLabelCount],MEAS_LIST_RS_ITEM[nLabelCount-1],ADAPRESULTSTRINGLENMAX);
	dData[nLabelCount-1] = tResult.dSurfaceResistivity;
	nLabelCount++;
	strncpy(szLabel[nLabelCount],MEAS_LIST_RS_ITEM[nLabelCount-1],ADAPRESULTSTRINGLENMAX);
	dData[nLabelCount-1] = tResult.dVolumeResistivity;
	nLabelCount++;
	strncpy(szLabel[nLabelCount],MEAS_LIST_RS_ITEM[nLabelCount-1],ADAPRESULTSTRINGLENMAX);
	dData[nLabelCount-1] = tResult.dConductivity;
	nLabelCount++;

	//2009.11.17 bagus RS --{--
	//��R����̏ꍇ�ɂ�OverRange/OverLoad������
	if(tResult.chDataValid != _T('N')){
		nLabelCount = 0;
	}
	//2009.11.17 bagus RS --}--
	return nLabelCount;
}
int CMainFrame::ScanDataLabelGet_CA(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid)
{
	CTARESULT tResult;
	RCP_DATA rcp;
	int nLabelCount = 0;

	m_pDoc->GetRcpData(&rcp);
	memset(&tResult,NULL,sizeof(tResult));
	m_pDoc->GetOneContactAngleData(&tResult,iNowPoint);

	//�ڐG�p�i�K��TRUE�ɂȂ��Ă���͂��j
	if(rcp.MeasProgInfo.ScanParams._CA.bContactAngle){
		dData[nLabelCount] = tResult.dCAngle;
		strncpy(szLabel[nLabelCount],MEAS_LIST_CA_ITEM[0],ADAPRESULTSTRINGLENMAX);
		nLabelCount++;
	}
	//���a
	if(rcp.MeasProgInfo.ScanParams._CA.bRadius){
		dData[nLabelCount] = tResult.dRadius;
		strncpy(szLabel[nLabelCount],MEAS_LIST_CA_ITEM[1],ADAPRESULTSTRINGLENMAX);
		nLabelCount++;
	}
	//�t�̑̐�
	if(rcp.MeasProgInfo.ScanParams._CA.bLiquidVolume){
		dData[nLabelCount] = tResult.dLiquid;
		strncpy(szLabel[nLabelCount],MEAS_LIST_CA_ITEM[2],ADAPRESULTSTRINGLENMAX);
		nLabelCount++;
	}

	return nLabelCount;
}
//2009.11.04 bagus CA --}--
/////////////////////////////////////////////////////////////////////////////
// ����f�[�^���x����(����ѐ�)�擾
//2009.11.04 bagus CA --{--
//ADAP�Ƃ���ȊO�̂��̂����ʉ������邽�߂ɖ��̂̕ύX
//int CMainFrame::ScanDataLabelGet(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid)
int CMainFrame::ScanDataLabelGet_ADAP(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid)
//2009.11.04 bagus CA --}--
{
	//Kojika 20090525 Add
	CString strbuf;
	//Kojika 20090525 Add End
	CSharedMemory<ADAPRESULTDATABASE>* m_pAdapResultDataBase;

	m_pAdapResultDataBase = new CSharedMemory<ADAPRESULTDATABASE>;
	if ( !m_pAdapResultDataBase->Open(_T(ADAPRESULTFILEMAPPINGNAME)) ){
//	AfxMessageBox(_T("AdapResultDataBase Open Error"));
		//AfxMessageBox(_T("AdapResultDataBase �I�[�v���G���["));
		//Kojika 20090525 Change
		LoadStringML(IDS_ADAP_RESULTDB_OPEN_ERROR , strbuf, "AdapResultDataBase Open Error");
		AfxMessageBox(strbuf);
	}
	//Kojika 20090525 Change End

	CString strBuffer;
	int iCol = 0;

	//���x���i�[�����擾
	do{
		if((afValid.ucResultAF == (BYTE)0)
		||(afValid.ucResultAF == (BYTE)1)
		||(afValid.ucResultAF == (BYTE)2)
		||((afValid.ucResultAF == (BYTE)4)&&(afValid.ucResultPopup == (BYTE)2))){
			//���肠��
			dData[iCol] = m_pAdapResultDataBase->GetSharedMemoryPtr()->dData[iCol];
		}
		else{
			//�����Ȃ�
		}

		strBuffer = m_pAdapResultDataBase->GetSharedMemoryPtr()->szLabel[iCol];
		iCol++;
	}while( !strBuffer.IsEmpty() );
//	memcpy(szLabel, m_pAdapResultDataBase->GetSharedMemoryPtr()->szLabel, sizeof(m_pAdapResultDataBase->GetSharedMemoryPtr()->szLabel)); //���x���擾
	memcpy(szLabelOri, m_pAdapResultDataBase->GetSharedMemoryPtr()->szLabelOri, sizeof(m_pAdapResultDataBase->GetSharedMemoryPtr()->szLabelOri)); //DDE�̃��x���擾

	delete m_pAdapResultDataBase;
	m_pAdapResultDataBase = NULL;

	return iCol-1; //���x���i�[����ԋp
}

// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
/////////////////////////////////////////////////////////////////////////////
// ����f�[�^���x����(����ѐ�)�擾
int CMainFrame::ScanDataLabelGet_COMPEASE(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid)
{
	CString strbuf;
	CSharedMemory<ADAPRESULTDATABASE>* pAdapResultDataBase;
	CSharedMemory<COMPEASERESULT_HELPER>* pCompEASEResultHelper;

	pAdapResultDataBase = new CSharedMemory<ADAPRESULTDATABASE>;
	if ( !pAdapResultDataBase->Open(_T(ADAPRESULTFILEMAPPINGNAME)) ){
		LoadStringML(IDS_ADAP_RESULTDB_OPEN_ERROR , strbuf, "AdapResultDataBase Open Error");
		AfxMessageBox(strbuf);
	}

	pCompEASEResultHelper = new CSharedMemory<COMPEASERESULT_HELPER>;
	if ( !pCompEASEResultHelper->Open(_T(COMPEASERESULT_HELPER_FILEMAPPINGNAME)) ){
		LoadStringML(IDS_ADAP_RESULTDB_OPEN_ERROR , strbuf, "CompEASEResultHelperDataBase Open Error");
		AfxMessageBox(strbuf);
	}

	//���x���i�[�����擾
	int iDispLabelCount = 0;

	// CompleteEASE�̌��ʂɊ܂܂��\�������郉�x����ADAP_DATABASE�ɐݒ肳��Ă���̂ŁA
	// ������SR,SE�����ƌ݊������������邽�߁A���ۂɕԋp����Ă������x���������𒊏o����
	const PCOMPEASERESULT_HELPER pCompEASEData = pCompEASEResultHelper->GetSharedMemoryPtr();
	int iCount = sizeof(pCompEASEData->bItemEnable) / sizeof(pCompEASEData->bItemEnable[0]);
	int iDispPos = 0;
	int i;
	for ( i = 0; i < iCount; i++ ) {
		if ( pCompEASEData->bItemEnable[i] ) {
			iDispPos = pCompEASEData->iDispIndex[i];
			if ( iDispPos < ADAPRESULT_COLS_MAX ) {
				memcpy(szLabel[iDispPos],    pAdapResultDataBase->GetSharedMemoryPtr()->szLabel[i],    sizeof(szLabel[iDispPos]));
				memcpy(szLabelOri[iDispPos], pAdapResultDataBase->GetSharedMemoryPtr()->szLabelOri[i], sizeof(szLabelOri[iDispPos]));
				dData[iDispPos] = pAdapResultDataBase->GetSharedMemoryPtr()->dData[i];
				iDispLabelCount++;
			}
		}
		if ( iDispLabelCount >= pCompEASEData->iItemEnableCount ||
			 iDispLabelCount >= ADAPRESULT_COLS_MAX ) {
			break;
		}
	}

	char szUnit[DISP_UNIT_NAME_LEN + 1];	//	���茋�ʒP�ʖ�

	memset(szUnit, 0, sizeof(szUnit));
	m_pDoc->GetUnitName(szUnit);//	���茋�ʒP�ʖ��̎擾

	//�u������
	for(i = 0; szLabel[i][0] != '\0'; i++){
		//Thick*�ɒP�ʂ�t��
		if(strstr(szLabel[i], "Thick") != NULL){
			char szUnitPlus[ADAPRESULTSTRINGLENMAX + 1];
			memset(szUnitPlus, 0, sizeof(szUnitPlus));
			sprintf(szUnitPlus, "%s%s%s", "[", szUnit, "]");
			strcat(szLabel[i], szUnitPlus);
		}

		//�wMSE��Fit�x�̒u��
		if(strcmp(szLabel[i], "MSE") == 0){
			strcpy(szLabel[i], "Fit");
		}
	}

	//
	m_pDoc->SetDispLabel(szLabel);
	m_pDoc->SetDispLabelCount(iDispLabelCount);

	delete pAdapResultDataBase;
	pAdapResultDataBase = NULL;
	delete pCompEASEResultHelper;
	pCompEASEResultHelper = NULL;

	return iDispLabelCount; //���x���i�[����ԋp
}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--

/////////////////////////////////////////////////////////////////////////////
// ���v�v�Z
void CMainFrame::GlassDataStatistics(STATISTICS* Statistics, int iLabelCount, UINT nFinalFocusFailDataValidOption/*, AF_VALID afValid*/)
{
	ONE_POINT_DATA OnePointData;
	memset(&OnePointData, 0, sizeof(OnePointData));

	int Point, Type;
	int ValidPointCount = 0;
	double Min, Max, Ave, Std, Range, Tickness;
	double ThicknessSum;
	double Sigma;
	BOOL First;

	for(Type = 0 ; Type < iLabelCount ; Type++){
		First = TRUE;
		ThicknessSum = 0.0;
		ValidPointCount = 0;

		Min = Max = Ave = Std = Range = 0.0;

		///// Min / Max /////
		for(Point = 0 ; Point < SCAN_POINT_MAX ; Point++){
		m_pDoc->GetOnePointData(&OnePointData, Point+1);
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
			if (m_iNowPoint <= Point) {
				/* ���萔����	*/
				/*		�{���Ȃ�Cm_pDoc->GetStoreCount() ���g���ׂ��ł����C
						m_pDoc->SetStoreCount() �� GlassDataStatistics() ��
						��Ɏ��s����Ă��邽�߁CGlassDataStatistics() ���ł�
						���ۂ̑��萔�ł��� m_iNowPoint �g�킴��𓾂܂���D	*/
				break;
			}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
			if(OnePointData.Valid == FALSE){
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
				m_pMeaTabViewObj->MRCS_SettingStatusSet(Point + 1, m_pMeaTabViewObj->MRCS_SettingStatusGet(Point + 1) | 0x4000);
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
				break;
			}

			if(nFinalFocusFailDataValidOption == 1){ //FanalFocusFailDataValidOption(���C�����V�s��)��INVALID
				if(
					// �I�[�g�t�H�[�J�X�Ȃ�
					(OnePointData.afValid.ucResultAF == (BYTE)0)

					// AF����
					||(OnePointData.afValid.ucResultAF == (BYTE)1)

					//�|�b�v�A�b�v���� ���� Z�}�j���A������A�� ���� AF�}�j���A������i�V
					||((OnePointData.afValid.ucResultAF == (BYTE)4)&&(OnePointData.afValid.bit1ManuZ == 1)&&(OnePointData.afValid.bit1ManuAF == 0))

					//�|�b�v�A�b�v���� ���� Z�}�j���A������i�V ���� AF�}�j���A������A�� ���� �}�j���A���t�H�[�J�X���쐬��
					||((OnePointData.afValid.ucResultAF == (BYTE)4)&&(OnePointData.afValid.bit1ManuZ == 0)&&(OnePointData.afValid.bit1ManuAF == 1)&&(OnePointData.afValid.bit1ManuAfRslt == 0))){

					//���̏����̏ꍇ�͓��v�v�Z�ɉ�����
				}
				else{
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
					m_pMeaTabViewObj->MRCS_SettingStatusSet(Point + 1, m_pMeaTabViewObj->MRCS_SettingStatusGet(Point + 1) | 0x2000);
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
					continue;
				}
			}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
			if (2 == m_pMeaTabViewObj->MRCS_SettingGet(Point + 1)) {
				/* �폜�ςݍs�ł����D	*/
				continue;
			} else if (0 < m_pMeaTabViewObj->MRCS_IncludeCountGet()) {
				/* �w��s�݂�	*/
				if (1 != m_pMeaTabViewObj->MRCS_SettingGet(Point + 1)) {
					/* �w��s�łȂ�	*/
					continue;
				}
			}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

			Tickness = OnePointData.dData[Type];
			if(First){
				Min = Max = Tickness;
				First = FALSE;
			}
			else{
				///// Glass Minimum /////
				if(Min > Tickness){
					Min = Tickness;
				}

				///// Glass Maximum /////
				if(Max < Tickness){
					Max = Tickness;
				}
			}

			ThicknessSum += Tickness;

			ValidPointCount++;
		}

		if(ValidPointCount > 0){
			///// Glass Average /////
			Ave = (float)(ThicknessSum / (double)ValidPointCount);

			///// Glass Standard Deviation /////
			Std = 0.0;
			Sigma = 0.0;
			if(ValidPointCount >= 2){
				for(Point = 0 ; Point < SCAN_POINT_MAX ; Point++){
					m_pDoc->GetOnePointData(&OnePointData, Point+1/*iNowPoint*/);
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
					if (m_iNowPoint <= Point) {
						/* ���萔����	*/
						break;
					}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
					if(OnePointData.Valid == FALSE){
						break;
					}
/* added 2016.06.02 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
					if (0 != (m_pMeaTabViewObj->MRCS_SettingStatusGet(Point + 1) & 0x2000)) {
						/* AF �ُ�ł����D	*/
						continue;
					} else if (2 == m_pMeaTabViewObj->MRCS_SettingGet(Point + 1)) {
						/* �폜�ςݍs�ł����D	*/
						continue;
					} else if (0 < m_pMeaTabViewObj->MRCS_IncludeCountGet()) {
						/* �w��s�݂�	*/
						if (1 != m_pMeaTabViewObj->MRCS_SettingGet(Point + 1)) {
							/* �w��s�łȂ�	*/
							continue;
						}
					}
/* added 2016.06.02 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

					Sigma += pow((OnePointData.dData[Type] - Ave), 2.0);
				}

				Std = (float)sqrt((Sigma / (double)(ValidPointCount - 1)));
			}

			////// Glass Range /////
			Range = Max-Min;

			Statistics[Type].dMinimum			= Min;
			Statistics[Type].dMaximum			= Max;
			Statistics[Type].dAverage			= Ave;
			Statistics[Type].dStandardDeviation = Std;
			Statistics[Type].dRange 			= Range;
			Statistics[Type].Valid			   = TRUE;
		}
		else{
			Statistics[Type].dMinimum			= 0;
			Statistics[Type].dMaximum			= 0;
			Statistics[Type].dAverage			= 0;
			Statistics[Type].dStandardDeviation = 0;
			Statistics[Type].dRange 			= 0;
			Statistics[Type].Valid			   = FALSE;
		}
	}

	m_pDoc->SetStatisticsValidPointCount(ValidPointCount);
}

// 2009.09.18 K.Matsuo SE�Ή����S�̓I�Ɍ����� -->
/////////////////////////////////////////////////////////////////////////////
// XMP���茋�ʃf�[�^�����t�H���_�ɃR�s�[����B
// �R�s�[��̃t�@�C�����ɂ́A�A�ԁi����|�C���g�ԍ��j��t������B
void CMainFrame::MeasPointDatFileTempCopy(int iNowPoint)
{
	SR_XMP srxmp;
	SE_XMP sexmp;
	char* pszXmpDataPath;			// DataPath
	char* pszXmpFittingDataPath;	// FittingDataPath
	char szXmpDataFileName[_MAX_FNAME]; 		// Data�t�@�C����
	char szXmpDataExt[_MAX_EXT];				// Data�g���q
	char szDataFilePath[MAX_PATH];				// �R�s�[���Data�t�@�C�����i�A�ԕt���j
	char szXmpFittingDataFileName[_MAX_FNAME];	// FittingData�t�@�C����
	char szXmpFittingDataExt[_MAX_EXT]; 		// FittingData�g���q
	char szFittingDataFilePath[MAX_PATH];		// �R�s�[���FittingData�t�@�C�����i�A�ԕt���j

	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);

	//2009.10.30 bagus 2point-distance --{--
	if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&&	 rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		//2�_�ԋ����̏ꍇ�ɂ̓f�[�^�͐�������Ȃ��̂łȂɂ����Ȃ�
		return;
	}
	//2009.10.30 bagus 2point-distance --}--
	//2009.11.04 bagus MS,CA,RS --{--
	else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS
		||	rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
		||	rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA
		){
		//�ȏ�̃w�b�h�^�C�v�ɐ��f�[�^�͂Ȃ��̂łȂɂ����Ȃ�
		return;
	}
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE){
		return;
	}
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	//2009.11.04 bagus MS,CA,RS --}--
	//�R�s�[���̃t�@�C���p�X�擾
	if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR ||
			rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS ) {
		// SR����܂��́A�X�g���X���̖�������
		ConfigFile_GetNanoSpecIni(&srxmp, CONFIG_FILE_SR_XMP);
		pszXmpDataPath = srxmp.szDataPath;
		pszXmpFittingDataPath = srxmp.szFittingDataPath;
	}
	else {
		// SE����
		ConfigFile_GetNanoSpecIni(&sexmp, CONFIG_FILE_SE_XMP);
		pszXmpDataPath = sexmp.szDataPath;
		pszXmpFittingDataPath = sexmp.szFittingDataPath;
	}

	//�t�@�C���p�X����t�@�C�����擾
	_splitpath(pszXmpDataPath, NULL, NULL, szXmpDataFileName, szXmpDataExt);
	_splitpath(pszXmpFittingDataPath, NULL, NULL, szXmpFittingDataFileName, szXmpFittingDataExt);

	::CreateDirectory(g_szData_Meas_Dat_Temp_Copy_Dir, NULL);

	//�t�@�C�����ɑ���|�C���g�ԍ���ǉ�
	sprintf(szDataFilePath, "%s%s_%05d%s", g_szData_Meas_Dat_Temp_Copy_Dir, szXmpDataFileName, iNowPoint, szXmpDataExt);
	sprintf(szFittingDataFilePath, "%s%s_%05d%s", g_szData_Meas_Dat_Temp_Copy_Dir, szXmpFittingDataFileName, iNowPoint, szXmpFittingDataExt);

	::CopyFile(pszXmpDataPath, szDataFilePath, FALSE);				// �����t�@�C���������Ă��A�㏑������
	if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR && rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS ||
			rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS ||
			rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SE ) {
		::CopyFile(pszXmpFittingDataPath, szFittingDataFilePath, FALSE);	// �����t�@�C���������Ă��A�㏑������
	}
}
// 2009.09.18 K.Matsuo SE�Ή����S�̓I�Ɍ����� <--

/////////////////////////////////////////////////////////////////////////////
// �����񒆂̂���P�o�C�g�ɂ��āA�P�o�C�g�������Q�o�C�g�����̈ꕔ�ł��邩�𔻒肷��B
// *********************************************************
//	������ str �� str[nPos] �ɂ��āA
//	  �O �c�c �P�o�C�g����
//	  �P �c�c �Q�o�C�g�����̈ꕔ�i��P�o�C�g�j
//	  �Q �c�c �Q�o�C�g�����̈ꕔ�i��Q�o�C�g�j
//	�̂����ꂩ��Ԃ��B
//*********************************************************
#define jms1(c) (((0x81 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0x9F)) || ((0xE0 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xFC) ))
#define jms2(c) ((0x7F != (unsigned char)(c)) && (0x40 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xFC))
int CMainFrame::isJMS( const char *str, int nPos )
{
	int i;
	int state; // { 0, 1, 2 }

	// 0 = �P�o�C�g����
	// 1 = �Q�o�C�g�����̑�P�o�C�g
	// 2 = �Q�o�C�g�����̑�Q�o�C�g
	state = 0;
	for(i = 0; str[i] != '\0'; i++){
		if		( ( state == 0 ) && ( jms1( str[i] ) ) ) state = 1; // 0 -> 1
		else if ( ( state == 1 ) && ( jms2( str[i] ) ) ) state = 2; // 1 -> 2
		else if ( ( state == 2 ) && ( jms1( str[i] ) ) ) state = 1; // 2 -> 1
		else											 state = 0; // 2 -> 0, ���̑�

		// str[nPos] �ł̏�Ԃ�Ԃ��B
		if ( i == nPos ) return state;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	ONE_POINT_DATA OnePointData;										//1�|�C���g���̑���f�[�^
	STATISTICS Statistics[ADAPRESULT_COLS_MAX]; 						//���v�f�[�^
	TEST_MODE_DATA TestModeData;										//// �e�X�g���[�h�p�f�[�^
	char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1]; 					//�O���b�h�̃^�C�g��(X,Y,Z)
	char szTitleAf[AF_HEADER_LEN + 1];									//�O���b�h�̃^�C�g��(AF)
	char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];		//�O���b�h�̃^�C�g�� X,Y,Z,AF�͊܂܂�
	char szLabelOri[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];	// DDE��M�̃^�C�g��
	int iLabelCount;													//�O���b�h�̃^�C�g���� X,Y,Z,AF�͊܂܂�
	int iSetPointCount; 												//�i�[�σ|�C���g��
	BOOL bGetSamplID;
	SYSTEMTIME syStartDateTime; 	//����J�n����
	char szSampleId[SAMPLE_ID_LEN + 1];
	HWND l_hChiefDlg;
	RCP_DATA rcp_data;
	int iFormatId;
	int i =0;
	STAGE_COORD_XYZ l_MeasPosXYZ; //�X�e�[�W�ʒu�擾�p(�}�j���A������E�e�X�g���[�h�̂�)

	UINT nFinalFocusFailDataValidOption = 0;

	memset(&OnePointData, 0, sizeof(OnePointData));
	memset(Statistics, 0, sizeof(Statistics));
	memset(&TestModeData, 0, sizeof(TestModeData));
	memset(szTitleXyz, 0, sizeof(szTitleXyz));
	memset(szTitleAf, 0, sizeof(szTitleAf));
	memset(szLabel, 0, sizeof(szLabel));
	memset(szLabelOri, 0, sizeof(szLabelOri));
	memset(szSampleId, 0, sizeof(szSampleId));
	memset(&rcp_data, 0, sizeof(rcp_data));

// 2014.04.04 bagus wavelength step modified -->
	TestModeData.dMinTargetWave = (double)MIN_TARGET_WAVELENGTH;
	TestModeData.dMaxTargetWave = (double)m_iMaxWaveLength;
	switch(m_srConfig.nWavelengthStep){
	case SR_WAVELENGTH_STEP_05_NM:
		TestModeData.dWavelengthStep = 0.5;
		break;
	case SR_WAVELENGTH_STEP_1_NM:
	default:
		TestModeData.dWavelengthStep = 1.0;
		break;
	}
// 2014.04.04 bagus wavelength step modified <--

	m_pDoc->GetRcpData(&rcp_data);

	switch (pCopyDataStruct->dwData) {
	case WMCD_CHIF_1POINT_END:
// 2009.11.25 K.Matsuo GTr Reference -->
		if (TRUE == m_bGTRref) {
			///// �`�[�t�փ��b�Z�[�W�𑗐M /////
			l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
			if(l_hChiefDlg == NULL){
				AlarmIf_Set(ALID_MeasurementStartFailed);
				break;
			}
// 2010.01.06 bagus Gantry --{--
			m_iGTRrefNowPointNo++;
// 2010.01.06 bagus Gantry --}--
			if(m_pDoc->GetHostMode() == HOST_REMOTE){
			//	  m_iGTRrefNowPointNo++;
				PifComm_CompleteReferencePointReport(m_iGTRrefNowPointNo);
			}
			//2009.11.29 bagus GTR --{--
			if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){		//�}�j���A������
				//����I������ʂ֒ʒm
				m_pManuMeaFormViewObj->PostMessage(WM_TESTMODE_ONEPOINT_END, (WPARAM)0, (LPARAM)0);
			}
			//2009.11.29 bagus GTR --}--
// 2010.01.06 bagus Gantry --{--
			if(m_pDoc->GetMeasMode() != MEASMODE_TEST){
				::PostMessage(l_hChiefDlg, WM_DATA_1POINT_END, 0, 0);
				break;
			}else{
			//	MEAS_ReadGTrRefFile(rcp_data.szRecipeName,m_iGTRrefNowPointNo);
			}
// 2010.01.06 bagus Gantry --}--
//			return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);


		}
// 2009.11.25 K.Matsuo GTr Reference <--
			///// �e�X�g���[�h /////
		if(m_pDoc->GetMeasMode() == MEASMODE_TEST){
			static AF_VALID afValid;
			afValid.dwValid =((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->dwAFValid;

			//2010.01.11 bagus GTR --{--
			//GTR�̃e�X�g���[�h
			//2010.01.11 bagus GTR --}--

			if((afValid.ucResultAF == (BYTE)0)
			||(afValid.ucResultAF == (BYTE)1)
			||(afValid.ucResultAF == (BYTE)2)
			||((afValid.ucResultAF == (BYTE)4)&&(afValid.ucResultPopup == (BYTE)2))){

				//���肠��

				m_iNowPoint++;
				m_pDoc->SetStoreCount(m_iNowPoint);

				NS_StageGetPos(&l_MeasPosXYZ);

				//2009.10.29 2point-Distance --{--
				if( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
				&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
				//2�_�ԋ����̏ꍇ�ɂ̓w�b�h�̈ʒu������ʒu�ł͂Ȃ�
					STAGE_COORD tCoord = m_pDoc->GetLastDistancePoint();
					OnePointData.xyz.lX = tCoord.lX;
					OnePointData.xyz.lY = tCoord.lY;
					OnePointData.xyz.lZ = 0;
				}else{
					OnePointData.xyz.lX = l_MeasPosXYZ.lX;
					OnePointData.xyz.lY = l_MeasPosXYZ.lY;
					OnePointData.xyz.lZ = l_MeasPosXYZ.lZ;
				}
				//2009.10.29 2point-Distance --}--

// 2009.09.30 K.Matsuo Delete <--
//				///// MeaSys���瑪��l�擾 /////
//				if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS){
// 2009.05.15 k-matsuo -->
//					for(i = MIN_TARGET_WAVELENGTH; i <= MAX_TARGET_WAVELENGTH ; i++){
//					for(i = MIN_TARGET_WAVELENGTH; i <= m_iMaxWaveLength ; i++){
// 2009.05.15 k-matsuo <--
//						OnePointData.dAnalysisGenData[i-1] = MEAS_GetAnalysisGenData(i) * 100;
//					}
//				}
// 2009.09.30 K.Matsuo Delete <--
// 2009.09.30 K.Matsuo Add -->
				MeasPointDatFileTempCopy(m_iNowPoint);
// 2009.09.30 K.Matsuo Add <--
				//2009.11.04 bagus CA --{--
				//�R���^�N�g�A���O�����茋�ʕۑ�
				if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
					CTARESULT tResult;
					memset(&tResult,NULL,sizeof(tResult));
					GetCTAMeasureResult(&tResult);
					m_pDoc->SetOneContactAngleData(&tResult,m_iNowPoint);
				}
				//2009.11.04 bagus CA --}--
				//2009.11.10 bagus RS --{--
				if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP){
					RESISTRESULT tResult;
					memset(&tResult,NULL,sizeof(tResult));
					GetResistMeasureResult(&tResult);
					m_pDoc->SetOneResistData(&tResult,m_iNowPoint);
				}
				//2009.11.10 bagus RS --}--

				//����
#if 0 // 2014.04.04 bagus wavelength step modified -->
// // 2009.05.15 k-matsuo -->
// //				for(i = MIN_TARGET_WAVELENGTH; i <= MAX_TARGET_WAVELENGTH ; i++){
// 				for(i = MIN_TARGET_WAVELENGTH; i <= m_iMaxWaveLength ; i++){
// // 2009.05.15 k-matsuo <--
// // 2009.09.30 K.Matsuo Delete -->
// //					OnePointData.dAnalysisData[i-1] = MEAS_GetAnalysisData(i) * 100;
// // 2009.09.30 K.Matsuo Delete <--
// 					OnePointData.Valid = TRUE;
// //					if(m_bGTRref == TRUE && m_pDoc->GetMeasMode() == MEASMODE_TEST){
// 					if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR &&
// 					   rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G &&
// 						m_pDoc->GetMeasMode() == MEASMODE_TEST){
// 						//TestModeData.dScanSampleData[i-1] = MEAS_GetScanSampleData(i);
// 						//TestModeData.dScan1stReferenceData[i-1] = MEAS_GetScan1stReferenceData(i);
// 						//TestModeData.dScanDark_T1Data[i-1] = MEAS_GetScanDark_T1Data(i);
// 						TestModeData.dScanSampleData[i-1] = MEAS_GetScanGTrReferenceData(((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->dwPointNo,i);
// 						//�A�����̃^�C�~���O�ł͂܂��_�[�N�𑪒肵�Ă��Ȃ��̂Ő������f�[�^�͓����Ă��Ȃ��B
// 						TestModeData.dScan1stReferenceData[i-1] = MEAS_GetScanGTrReferenceData(0,i);
// 						TestModeData.dScanDark_T1Data[i-1] = 0.0;
// 					}else{
// 						TestModeData.dScanSampleData[i-1] = MEAS_GetScanSampleData(i);
// 						TestModeData.dScan1stReferenceData[i-1] = MEAS_GetScan1stReferenceData(i);
// 						TestModeData.dScanDark_T1Data[i-1] = MEAS_GetScanDark_T1Data(i);
// 					}
// 				}
#else
				ASSERT(TestModeData.dWavelengthStep > 0.0);

				double dTargetWave = TestModeData.dMinTargetWave;
				double dData;
				while(dTargetWave <= TestModeData.dMaxTargetWave)
				{
					OnePointData.Valid = TRUE;
					if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR &&
					   rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G &&
						m_pDoc->GetMeasMode() == MEASMODE_TEST){
						dData = MEAS_GetScanGTrReferenceData(((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->dwPointNo, dTargetWave);
						TestModeData.SetScanSampleData(dTargetWave, dData);
						//�A�����̃^�C�~���O�ł͂܂��_�[�N�𑪒肵�Ă��Ȃ��̂Ő������f�[�^�͓����Ă��Ȃ��B
						dData = MEAS_GetScanGTrReferenceData(0, dTargetWave);
						TestModeData.SetScan1stReferenceData(dTargetWave, dData);
						dData = 0.0;
						TestModeData.SetScanDark_T1Data(dTargetWave, dData);
					}
					else{
						dData = MEAS_GetScanSampleData(dTargetWave);
						TestModeData.SetScanSampleData(dTargetWave, dData);
						dData = MEAS_GetScan1stReferenceData(dTargetWave);
						TestModeData.SetScan1stReferenceData(dTargetWave, dData);
						dData = MEAS_GetScanDark_T1Data(dTargetWave);
						TestModeData.SetScanDark_T1Data(dTargetWave, dData);
					}

					dTargetWave += TestModeData.dWavelengthStep;
				}
#endif // 2014.04.04 bagus wavelength step modified -->

				///// ����l(���x���܂�)�擾 /////
				//2009.11.04 bagus CA --{--
				//�f�[�^�ƃ��x���̎擾�����̋��ʉ��iADAP����̃f�[�^�Ƃ���ȊO�iCA�Ȃǁj
				//iLabelCount = ScanDataLabelGet(,szLabel, szLabelOri, OnePointData.dData, 1, OnePointData.afValid);
				iLabelCount = ScanDataLabelGet(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType,szLabel, szLabelOri, OnePointData.dData, 1, OnePointData.afValid);
				//2009.11.04 bagus CA --}--
				//2009.11.17 bagus RS --{--
				//���v�v�Z�Ɋ܂߂Ȃ��悤��OverRange/OverLoad�̏ꍇ�ɂ�Valid�𗎂Ƃ�
				if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
				&& iLabelCount == 0){
					OnePointData.Valid = FALSE;
				}
				//2009.11.17 bagus RS --}--

				//����f�[�^�ǋL(�����Y�A���w�t�B���^�[�A���莞��)
				MAIN_RCP_INFO l_MainRcpInfo;
				MEAS_PROG_INFO l_Meas;
				memset(&l_MainRcpInfo, 0, sizeof(l_MainRcpInfo));
				memset(&l_Meas, 0, sizeof(l_Meas));
				RecipeFile_LoadRecipe(&l_MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
				RecipeFile_LoadRecipe(&l_Meas, l_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);

				TestModeData.MeasRcpCon.iLens = l_Meas.ScanParams._SR.iLens;						// ���背���Y
				TestModeData.MeasRcpCon.wOpticsFilterType = l_Meas.ScanParams._SR.wOpticsFilterType;// ���w�t�B���^
				TestModeData.MeasRcpCon.dIntegTime = l_Meas.ScanParams._SR.dIntegTime;				// ���莞��

				if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS){
					TestModeData.MeasRcpCon.wStart = l_Meas.ScanParams._SR.WavelenRange.wStart;
					TestModeData.MeasRcpCon.wEnd = l_Meas.ScanParams._SR.WavelenRange.wEnd;
				}
				else{	//( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					TestModeData.MeasRcpCon.wStart = MIN_TARGET_WAVELENGTH;
// 2009.05.15 k-matsuo -->
//					TestModeData.MeasRcpCon.wStart = MAX_TARGET_WAVELENGTH; 	// wStart�́A�o�O�Ȃ̂ł́I�H
					TestModeData.MeasRcpCon.wEnd = m_iMaxWaveLength;
// 2009.05.15 k-matsuo <--
				}

				///// AF VALID�f�[�^�擾 /////
				OnePointData.afValid.dwValid =((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->dwAFValid;
				AfValidResultJudge(OnePointData.afValid, OnePointData.szShowAfValid, OnePointData.szAfP502);

				//NanoSpecDoc�ɓo�^
				m_pDoc->SetOnePointData(&OnePointData, m_iNowPoint);
				m_pDoc->SetTestModeData(&TestModeData, m_iNowPoint);

/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
				switch (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
				case HEAD_TYPE_SR:
				case HEAD_TYPE_4PP:
				case HEAD_TYPE_CTA:
					if ((HEAD_TYPE_SR == rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType)
					 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G != rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
					 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE != rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)) {
						break;
					}
					if (0x80000000 != m_pDoc->m_lZ_AF) {
						m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// ���쒆�t���O(Z ��)���I��
						if (0 == StageElevatorMove(m_pDoc->m_lZ_AF)) {
							/* �ړ��J�n���s	*/
							AlarmIf_Set(ALID_Z_AxisMoveFailed);
						}
						m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// ���쒆�t���O(Z ��)���I�t
					}
					break;
				case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
				case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
				case HEAD_TYPE_IRSE:
				case HEAD_TYPE_STRESS:
				case HEAD_TYPE_MS:
				default:
					break;
				}
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
				//����I������ʂ֒ʒm
// 2010.01.06 bagus Gantry --{--
				if(m_pTestMeasDlgObj && m_pTestMeasDlgObj->GetSafeHwnd() != NULL)
					m_pTestMeasDlgObj->PostMessage(WM_TESTMODE_ONEPOINT_END, (WPARAM)0, (LPARAM)0);
				if(m_pTestMeasGantryDlgObj && m_pTestMeasGantryDlgObj->GetSafeHwnd() != NULL)
					m_pTestMeasGantryDlgObj->PostMessage(WM_TESTMODE_ONEPOINT_END, (WPARAM)0, (LPARAM)0);
// 2010.01.06 bagus Gantry --}--

			}

			///// �`�[�t�փ��b�Z�[�W�𑗐M /////
			l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
			if(l_hChiefDlg == NULL){
				AlarmIf_Set(ALID_MeasurementStartFailed);
				break;
			}
			::PostMessage(l_hChiefDlg, WM_DATA_1POINT_END, 0, 0);

			//2010.01.19 bagus �C�� --{--
			if(m_bGTRref != TRUE){
				m_pDoc->SetProcessStatus(PROCESS_WAIT);
			}
			//2010.01.19 bagus �C�� --}--

// CHG 2009.06.02 H.Kawamura
#if 0
			// H/W�L���Ȃ�
			if( GetJoyStickMode() != 0 ){
				BOOL bJoyStk= m_pDoc->ChangeJoyStick(TRUE); //�W���C�X�e�B�b�N���g�p�ɐݒ�
				TRACE("CMainFrame::OnCopyData()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
			}
#else
			// �K��JoyStick�̏�Ԃ�߂�����
			m_pDoc->JoyStickStatusRestore();
#endif
// CHG 2009.06.02 H.Kawamura

			break;
		}

		// ==================================================================
		// �ȉ��A�ʏ푪��(�����W���[�܂�)�A�}�j���A������ �̏���

		::WaitForSingleObject(m_hEvTabView, INFINITE); //TabView���ł���܂ő҂�

		if(m_pDoc->GetMeasMode() == MEASMODE_SEQ_OR_REMEASURE){ //�ʏ푪��or�����W���[
			///// �ŐV�|�C���g�ԍ� /////
			m_iNowPoint = ((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->dwPointNo;

			if(m_pDoc->GetSeqMeasMode() == SEQ_ON_REMEASURE){	//�����W���[��
				//����L�����Z�����ɁA�O�̒l�����̂܂܎g�p����חp�Ɏ擾
				m_pDoc->GetOnePointData(&OnePointData, m_iNowPoint);
			}
		}
		//2009.11.04 bagus CA --{--
		//�R���^�N�g�A���O�����茋�ʕۑ�
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
			CTARESULT tResult;
			memset(&tResult,NULL,sizeof(tResult));
			GetCTAMeasureResult(&tResult);
			//2009.11.29 bagus CA --{--
			//m_pDoc->SetOneContactAngleData(&tResult,m_iNowPoint);
			if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
//				m_pDoc->SetOneContactAngleData(&tResult,m_iNowPoint + 1);
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ----------              */
				if (0 < m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
					/* �đ���	*/
					m_pDoc->SetOneContactAngleData(&tResult, m_pMeaTabViewObj->MRCS_ReMeasureNoGet());
				} else {
					/* �V�K����	*/
					m_pDoc->SetOneContactAngleData(&tResult, m_iNowPoint + 1);
				}
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
			}else{
				m_pDoc->SetOneContactAngleData(&tResult,m_iNowPoint);
			}
			//2009.11.29 bagus CA --}--
		}
		//2009.11.04 bagus CA --}--
		//2009.11.10 bagus RS --{--
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP){
			RESISTRESULT tResult;
			memset(&tResult,NULL,sizeof(tResult));
			GetResistMeasureResult(&tResult);
			//2009.11.29 bagus CA --{--
			if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
				m_pDoc->SetOneResistData(&tResult,m_iNowPoint + 1);
			}else{
				m_pDoc->SetOneResistData(&tResult,m_iNowPoint);
			}
			//2009.11.29 bagus CA --}--
		}
		//2009.11.10 bagus RS --}--

		///// XYZ���擾 /////
		// �ʏ푪��or�����W���[
		if(m_pDoc->GetMeasMode() == MEASMODE_SEQ_OR_REMEASURE){

			//2009.10.29 2point-Distance --{--
			if( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
			&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			//2�_�ԋ����̏ꍇ�ɂ̓w�b�h�̈ʒu������ʒu�ł͂Ȃ�
				STAGE_COORD tCoord = m_pDoc->GetLastDistancePoint();
				OnePointData.xyz.lX = tCoord.lX;
				OnePointData.xyz.lY = tCoord.lY;
				OnePointData.xyz.lZ = 0;
			}else{
				OnePointData.xyz.lX = ((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->lX;
				OnePointData.xyz.lY = ((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->lY;
				OnePointData.xyz.lZ = ((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->lZ;
			}
			//2009.10.29 2point-Distance --}--
		}
		//�}�j���A������
		else{
			NS_StageGetPos(&l_MeasPosXYZ);
			//2009.10.29 2point-Distance --{--
			if( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
			&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			//2�_�ԋ����̏ꍇ�ɂ̓w�b�h�̈ʒu������ʒu�ł͂Ȃ�
				STAGE_COORD tCoord = m_pDoc->GetLastDistancePoint();
				OnePointData.xyz.lX = tCoord.lX;
				OnePointData.xyz.lY = tCoord.lY;
				OnePointData.xyz.lZ = 0;
			}else{
			//2009.10.29 2point-Distance --}--
				OnePointData.xyz.lX = l_MeasPosXYZ.lX;
				OnePointData.xyz.lY = l_MeasPosXYZ.lY;
				OnePointData.xyz.lZ = l_MeasPosXYZ.lZ;
			}
		}

		///// AF VALID�f�[�^�擾 /////
		OnePointData.afValid.dwValid =((MEAS_1POINT_RESULT*)(pCopyDataStruct->lpData))->dwAFValid;
		AfValidResultJudge(OnePointData.afValid, OnePointData.szShowAfValid, OnePointData.szAfP502);
		// 2009.11.12 K.Matsuo OnePointData.Valid�́A�K���ݒ� -->
		OnePointData.Valid = TRUE;
		// 2009.11.12 K.Matsuo OnePointData.Valid�́A�K���ݒ� <--


		///// ����l(���x���܂�)�擾 /////
		//�ʏ푪��or�����W���[
		if(m_pDoc->GetMeasMode() == MEASMODE_SEQ_OR_REMEASURE){

			//2009.11.04 bagus CA --{--
			//�f�[�^�ƃ��x���̎擾�����̋��ʉ��iADAP����̃f�[�^�Ƃ���ȊO�iCA�Ȃǁj
			//iLabelCount = ScanDataLabelGet(szLabel, szLabelOri, OnePointData.dData, m_iNowPoint, OnePointData.afValid);
			iLabelCount = ScanDataLabelGet(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType,szLabel, szLabelOri, OnePointData.dData, m_iNowPoint, OnePointData.afValid);
			//2009.11.04 bagus CA --}--
			//2009.11.17 bagus RS --{--
			//���v�v�Z�Ɋ܂߂Ȃ��悤��OverRange/OverLoad�̏ꍇ�ɂ�Valid�𗎂Ƃ�
			if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
			&& iLabelCount == 0){
				OnePointData.Valid = FALSE;
			}
			//2009.11.17 bagus RS --}--
		}
		//�}�j���A������
		else{
			//2009.11.04 bagus CA --{--
			//iLabelCount = ScanDataLabelGet(szLabel, szLabelOri, OnePointData.dData, 1, OnePointData.afValid);
			iLabelCount = ScanDataLabelGet(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType,szLabel, szLabelOri, OnePointData.dData, 1, OnePointData.afValid);
			//2009.11.04 bagus CA --}--
			//2009.11.17 bagus RS --{--
			//���v�v�Z�Ɋ܂߂Ȃ��悤��OverRange/OverLoad�̏ꍇ�ɂ�Valid�𗎂Ƃ�
			if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
			&& iLabelCount == 0){
				OnePointData.Valid = FALSE;
			}
			//2009.11.17 bagus RS --}--
		}

		if(((OnePointData.afValid.ucResultAF == (BYTE)0)
			||(OnePointData.afValid.ucResultAF == (BYTE)1)
			||(OnePointData.afValid.ucResultAF == (BYTE)2)
			||(OnePointData.afValid.ucResultAF == (BYTE)3)
			||((OnePointData.afValid.ucResultAF == (BYTE)4)&&(OnePointData.afValid.ucResultPopup == (BYTE)2)))
			&&(!((OnePointData.afValid.bit2PopupKind == (BYTE)1)&&(OnePointData.afValid.ucResultPopup == (BYTE)4)))){
			//���肠��

			//�}�j���A������
			if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
//				m_iNowPoint++;
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ----------              */
				if (0 < m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
					/* �đ���	*/
					/*		m_iNowPoint ���C���N�������g���܂���D
							�Ȃ̂ŁC�ȍ~�̏�����
								m_pDoc->GetStoreCount()
								m_pDoc->GetReMeasurePointCnt()
							���ω����܂���D	*/
				} else {
					/* �V�K����	*/
					m_iNowPoint++;
				}
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
			}


/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
//			m_pDoc->SetOnePointData(&OnePointData, m_iNowPoint); //OnePointData.dData �����v�v�Z�ɕK�v�ƂȂ�̂ŉ��o�^�B
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ----------              */
			/* OnePointData.dData �����v�v�Z�ɕK�v�ƂȂ�̂ŉ��o�^	*/
			if (0 < m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
				/* �đ���	*/
				m_pDoc->SetOnePointData(&OnePointData, m_pMeaTabViewObj->MRCS_ReMeasureNoGet());
			} else {
				/* �V�K����	*/
				m_pDoc->SetOnePointData(&OnePointData, m_iNowPoint);
			}
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
			if (0 < m_pMeaTabViewObj->MRCS_IncludeCountGet()) {
				/* �w��s�����łɂ���ꍇ�́C�w��s�ɐݒ肵�܂��D	*/
				if (0 < m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
					/* �đ���	*/
					m_pMeaTabViewObj->MRCS_IncludeAdd(m_pMeaTabViewObj->MRCS_ReMeasureNoGet());
				} else {
					/* �V�K����	*/
					m_pMeaTabViewObj->MRCS_IncludeAdd(m_iNowPoint);
				}
			}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
			///// ���v�v�Z /////
			nFinalFocusFailDataValidOption = rcp_data.MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption;
			//2009.10.28 bagus 2point-distance --{--
			if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
			&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
				//2�_�ԋ�������̓��v�����v�Z����B
				DistanceDataStatistics(Statistics);
			}else{
				GlassDataStatistics(Statistics, iLabelCount, nFinalFocusFailDataValidOption/*, OnePointData.afValid*/);
			}
			//2009.10.28 bagus 2point-distance --}--

			///// �i�[�σ|�C���g�� /////
			iSetPointCount = m_iNowPoint;

			///// ����|�C���g�f�[�^�����t�H���_�ɃR�s�[���āA�K�؂ȃt�@�C�����ɕύX /////
//			if(m_pDoc->GetMeasMode() == MEASMODE_SEQ_OR_REMEASURE) //�ʏ푪��or�����W���[
//			{
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
//			MeasPointDatFileTempCopy(m_iNowPoint);
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ----------              */
			if (0 < m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
				/* �đ���	*/
				MeasPointDatFileTempCopy(m_pMeaTabViewObj->MRCS_ReMeasureNoGet());
			} else {
				/* �V�K����	*/
				MeasPointDatFileTempCopy(m_iNowPoint);
			}
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
//			}
//			else //�}�j���A������
//			{
//				MeasPointDatFileTempCopy(1);
//			}

			//�f�[�^���Z�[�u��Ԕ���l
			m_pMeaTabViewObj->m_bToolbarSaveCompFlg = FALSE;
			m_pMeaTabViewObj->m_bExitSaveCompFlg = FALSE;

			///// Doc�֊i�[ /////
			if(m_pDoc->GetLabelFlag() == FALSE){
				//�ŏ���1��ڂ̌Ăяo�����̂ݎ��s�������
				m_pDoc->SetLabel(szTitleXyz, szTitleAf, szLabel, szLabelOri, iLabelCount); //��1��ŉ�
				m_pDoc->SetLabelFlag(TRUE);
			}

/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
//			m_pDoc->SetOnePointAndStatistics(&OnePointData, Statistics, m_iNowPoint);
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ----------              */
			if (0 < m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
				/* �đ���	*/
				m_pDoc->SetOnePointAndStatistics(&OnePointData, Statistics, m_pMeaTabViewObj->MRCS_ReMeasureNoGet());
			} else {
				/* �V�K����	*/
				m_pDoc->SetOnePointAndStatistics(&OnePointData, Statistics, m_iNowPoint);
			}
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

			if(iSetPointCount > m_pDoc->GetStoreCount())
			{
				m_pDoc->SetStoreCount(iSetPointCount);
				if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //�}�j���A������
				{
					m_pDoc->SetMeasurePointCount(iSetPointCount);
				}
			}
		}

		if(m_pDoc->GetMeasMode() == MEASMODE_SEQ_OR_REMEASURE) //�ʏ푪��or�����W���[
		{
			if(m_pDoc->GetSeqMeasMode() == SEQ_ON_REMEASURE) //�����W���[��
			{
				m_pDoc->SetReMeasurePointCnt(m_pDoc->GetReMeasurePointCnt() + 1);
			}
		}

/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
		if (0 < m_pMeaTabViewObj->MRCS_IncludeCountGet()) {
			/* �w��s�݂�̏ꍇ�C�w��s�F�ԍ����Z�b�g	*/
			if (0 < m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
				/* �đ���	*/
				m_pMeaTabViewObj->MRCS_IncludeColorSetNoSet(m_pMeaTabViewObj->MRCS_ReMeasureNoGet());
			} else {
				/* �V�K����	*/
				m_pMeaTabViewObj->MRCS_IncludeColorSetNoSet(iSetPointCount);
			}
		}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
//		m_pDoc->SetCurrentSetPoint(iSetPointCount); //��ԍŌ�Ƀf�[�^���i�[(������)�����|�C���g�ԍ��̊i�[
//													//�����W���[���[�h�J�n����0���Z�b�g���邱��!
//													//(�ʏ푪�莞��GetStoreCount()�œ�����l�Ɠ���)
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ----------              */
		/*	��ԍŌ�Ƀf�[�^���i�[(������)�����|�C���g�ԍ��̊i�[
			�����W���[���[�h�J�n����0���Z�b�g���邱��!
			(�ʏ푪�莞��GetStoreCount()�œ�����l�Ɠ���)	*/
		if (0 < m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
			/* �đ���	*/
			WORD l_wRemeasureNo = m_pMeaTabViewObj->MRCS_ReMeasureNoGet();
			m_pMeaTabViewObj->MRCS_ReMeasureNoClr();	/* �đ���ԍ��N���A	*/
			/* ����ԍ����Z�b�g	*/
			m_pDoc->SetCurrentSetPoint(l_wRemeasureNo);
			/* �\���ς݃|�C���g�̋L��	*/
			WORD l_wList, l_wGraph;
			m_pMeaTabViewObj->MRCS_ShowCompletionPointSnap(&l_wList, &l_wGraph);
			/* �\���ς݃|�C���g���đ���ԍ��|�P�ɏ��������܂��D	*/
			m_pDoc->SetShowCompPointGraph(l_wRemeasureNo - 1);
			m_pDoc->SetShowCompPointList(l_wRemeasureNo - 1);
		} else {
			/* �V�K����	*/
			m_pDoc->SetCurrentSetPoint(iSetPointCount);
		}
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //�}�j���A������
		{
			m_pMeaTabViewObj->m_pMeasurementListDlg->SetTimer(SHOW_LIST_TIMER, 500, NULL);
			m_pMeaTabViewObj->m_pMeasurementGraphDlg->SetTimer(SHOW_GRAPH_TIMER, 500, NULL);
		}

		//��ʂւ̕�
		if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR ) {
			switch ( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType ) {
			case MEAS_PROG_TYPE_SR_THICKNESS:
				iFormatId = 1;
				break;
			case MEAS_PROG_TYPE_SR_REFLECTANCE:
				iFormatId = 2;
				break;
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
				iFormatId = 3;
				break;
			case MEAS_PROG_TYPE_SR_DISTANCE:
				iFormatId = 0;
				break;
			default:
				iFormatId = 0;
				break;
			}
			PifComm_CompleteMeasurePointReport(iFormatId, m_iNowPoint, OnePointData.szAfP502, &OnePointData.xyz, szLabelOri, OnePointData.dData, rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen);
		}
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SE ) {
			iFormatId = 4;
			PifComm_CompleteMeasurePointReport(iFormatId, m_iNowPoint, OnePointData.szAfP502, &OnePointData.xyz, szLabelOri, OnePointData.dData, rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen);
		}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE ) {
			iFormatId = 7;
			PifComm_CompleteMeasurePointReport(iFormatId, m_iNowPoint, OnePointData.szAfP502, &OnePointData.xyz, szLabelOri, OnePointData.dData, rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen);
		}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP ) {
			RESISTRESULT tResult;
			memset(&tResult,NULL,sizeof(tResult));
			m_pDoc->GetOneResistData(&tResult, m_iNowPoint);
			PifComm_CompleteMeasureResistPointReport(m_iNowPoint, OnePointData.szAfP502, &OnePointData.xyz, tResult.chDataValid, tResult.szResistance, tResult.szSurfaceResistivity, tResult.szVolumeResistivity, tResult.szConductivity);
		}
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA ) {
			CTARESULT tResult;
			memset(&tResult,NULL,sizeof(tResult));
			m_pDoc->GetOneContactAngleData(&tResult, m_iNowPoint);
			PifComm_CompleteMeasureContactAnglePointReport(m_iNowPoint, OnePointData.szAfP502, &OnePointData.xyz, tResult.dCAngle, tResult.dRadius, tResult.dLiquid);
		}
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS ) {
			// �X�g���X������̖�������́A�񍐂��܂���
			;
		}
		else {
			// ���Ή��I�H
			;
		}

		///// �`�[�t�փ��b�Z�[�W�𑗐M /////
		l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
		if(l_hChiefDlg == NULL){
			AlarmIf_Set(ALID_MeasurementStartFailed);
			break;
		}
		::PostMessage(l_hChiefDlg, WM_DATA_1POINT_END, 0, 0);

		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){		//�}�j���A������
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
			switch (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
			case HEAD_TYPE_SR:
			case HEAD_TYPE_4PP:
			case HEAD_TYPE_CTA:
				if ((HEAD_TYPE_SR == rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType)
				 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G != rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
				 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE != rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)) {
					break;
				}
				if (0x80000000 != m_pDoc->m_lZ_AF) {
					m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// ���쒆�t���O(Z ��)���I��
					if (0 == StageElevatorMove(m_pDoc->m_lZ_AF)) {
						/* �ړ��J�n���s	*/
						AlarmIf_Set(ALID_Z_AxisMoveFailed);
					}
					m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// ���쒆�t���O(Z ��)���I�t
				}
				break;
			case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
			case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
			case HEAD_TYPE_IRSE:
			case HEAD_TYPE_STRESS:
			case HEAD_TYPE_MS:
			default:
				break;
			}
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
			//����I������ʂ֒ʒm
			m_pManuMeaFormViewObj->PostMessage(WM_TESTMODE_ONEPOINT_END, (WPARAM)0, (LPARAM)0);
		}

// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
// 		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){		//�}�j���A������
// 			if(g_lModelType == MODEL_T3100){
// 				// �P������I����ʒm����悤�ɂ���
// 				PostMessage(WM_DISP_CONFIRM_POPUP, 20, 0);
// 			}
// 		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
		break;

	case WMCD_CHIF_MEAS_START:

// 2009.11.25 K.Matsuo GTr Reference -->
		m_iGTRrefNowPointNo = 0;
		if (MEASMODE_GTRREF == ((SEQ_MEAS_START*) (pCopyDataStruct->lpData))->enMeasMode) {
			m_bGTRref = TRUE;
		} else {
			m_bGTRref = FALSE;
		}
// 2009.11.25 K.Matsuo GTr Reference <--
		///// �T���v��ID�擾(�����[�g���̂�) /////
		bGetSamplID = ((SEQ_MEAS_START*)(pCopyDataStruct->lpData))->bGetSamplID;	// FALSE�F�T���v�� ID �̎w��͖���(��ʂ���̃V�[�P���X����J�n�̂���)
																			// TRUE �F�T���v�� ID �̎w�肪�L��(Pif ����̃V�[�P���X����J�n)
// 2009.11.25 K.Matsuo GTr Reference -->
		if (FALSE == m_bGTRref) {
// 2009.11.25 K.Matsuo GTr Reference <--
			if(bGetSamplID == TRUE){	//�����[�g��
// 2014.01.17 Bagus Add (Stage None�Ή�) -->
				SYSTEM_CONFIG l_SystemConfig;
				ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
				if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
					///// �������[������ /////
					m_pMeaOpeViewObj->MeasStart(rcp_data.StageProgInfoHdr.wNumScans);
					m_pDoc->SetLastMeasPoint(rcp_data.StageProgInfoHdr.wNumScans); //�ŏI����|�C���g���i�[
				}
				else{
					///// �������[������ /////
					m_pMeaOpeViewObj->MeasStart(1);
					m_pDoc->SetLastMeasPoint(1); //�ŏI����|�C���g���i�[
				}
// 2014.01.17 Bagus Add (Stage None�Ή�) <--

				//�V�[�P���X���胂�[�h�t���O�ݒ�
				m_pDoc->SetSeqMeasMode(SEQ_ON_NORMALMEASURE); //�ʏ푪�蒆�ɐݒ�

				//���胂�[�h�t���O�ݒ�
				m_pDoc->SetMeasMode(MEASMODE_SEQ_OR_REMEASURE); //�ʏ푪��or�����W���[

				///// �����ʂ֐ؑ� /////
				ChangeOperationPanel(OP_MEASUREMENT_PROCESS);
				ChangeExeWnd(MEAS_WND);

				strcpy( szSampleId, ((SEQ_MEAS_START*)(pCopyDataStruct->lpData))->szSampleID );
				m_pDoc->SetSampleId(szSampleId);
			}
		}

		///// ����J�n�����擾 /////
		GetLocalTime(&syStartDateTime);
		m_pDoc->SetStartDateTime(&syStartDateTime);
		break;

	case WMCD_CHIF_SET_RECIPE: //���H���V�s�w��

		///// �������[������ /////
		m_pDoc->InitMeasData();

		strcpy(rcp_data.szRecipeName, (const char*)((COPYDATASTRUCT*)pCopyDataStruct->lpData)); //���V�s���擾
		///// Recipe �擾 /////
		RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
		RecipeFile_LoadRecipe(&rcp_data.MeasProgInfo, rcp_data.MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
		RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
		//2009.08.21 bagus stress --{--
		//�X�g���X�w�b�_�Ȃ̂ŃX�g���X�p�̏���ʓr�擾����
		if(rcp_data.StageProgInfoHdr.wHeadType == HEAD_TYPE_STRESS){
			RecipeFile_LoadRecipe(&rcp_data.StressStageProgInfo,rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM_STRESS);
		}else{
			memset(&rcp_data.StressStageProgInfo,NULL,sizeof(rcp_data.StressStageProgInfo));
		}
		//2009.08.21 bagus stress --}--
		RecipeFile_LoadPointList(rcp_data.ScanPoint, rcp_data.StageProgInfoHdr.wNumScans, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage);

		///// ���V�s�Z�b�g /////
		m_pDoc->SetRcpData(&rcp_data);
		break;

	default:
		break;
	}

	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateRemote(CCmdUI* pCmdUI)
{
	BOOL bMenuEnable = FALSE;

	if ( strcmp(m_wndSplitter1.GetPane(1, 0)->GetRuntimeClass()->m_lpszClassName, "CMeasurementOperationView") == 0 &&
			(m_pDoc->GetProcessStatus() == PROCESS_WAIT || m_pDoc->GetProcessStatus() == PROCESS_DOWN) ) {
		bMenuEnable = TRUE;
	}
	pCmdUI->Enable(bMenuEnable);

	//�`�F�b�N�}�[�N�\���ؑւ�
	pCmdUI->SetCheck(m_pDoc->GetHostMode() == HOST_REMOTE);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateLocal(CCmdUI* pCmdUI)
{
	BOOL bMenuEnable;
	bMenuEnable = (m_pDoc->GetProcessStatus() == PROCESS_WAIT || m_pDoc->GetProcessStatus() == PROCESS_DOWN);
	pCmdUI->Enable(bMenuEnable);

	//�`�F�b�N�}�[�N�\���ؑւ�
	pCmdUI->SetCheck(m_pDoc->GetHostMode() == HOST_LOCAL);
}

/////////////////////////////////////////////////////////////////////////////
// ���茋�ʉ�ʗpAF_VALID�\���l�E��ʑ��M�pAF VALID �擾
void CMainFrame::AfValidResultJudge(AF_VALID afValid, char* szShowAfValid, char* szAfP502)
{
//������ 081226 hibino add(TRACE)
TRACE("[DATA PROCESSING] ucResultAF:%d ucResultPopup:%d bit1ManuXY:%d bit1ManuZ:%d bit1ManuAF:%d bit1ManuAfRslt:%d \n",
		afValid.ucResultAF, afValid.ucResultPopup, afValid.bit1ManuXY, afValid.bit1ManuZ, afValid.bit1ManuAF, afValid.bit1ManuAfRslt);
//������ 081226 hibino add(TRACE)

	//������
	strcpy(szShowAfValid, g_lpszAfValidResultShow[8]); //"-(--)" �z��O
	strcpy(szAfP502, g_lpszAfValidResultP502[8]); //"NG" �z��O

	if((afValid.ucResultAF == (BYTE)0)	//AF����
	||(afValid.ucResultAF == (BYTE)1)	//AF����
	||(afValid.ucResultAF == (BYTE)2)	//AF���s(������������)
	||(afValid.ucResultAF == (BYTE)4))	//AF���s(�|�b�v�A�b�v)
	{
		if(afValid.ucResultPopup == 0)	//�|�b�v�A�b�v����
		{
			if(afValid.ucResultAF == (BYTE)0)		//AF����
			{
				strcpy(szShowAfValid, g_lpszAfValidResultShow[0]);	//"-----" �I�[�g�t�H�[�J�X�Ȃ�
				strcpy(szAfP502, g_lpszAfValidResultP502[0]);		//"NO"
				return;
			}
			else if(afValid.ucResultAF == (BYTE)1)	//AF���� (���w�|�W�V�����␳���Ȃ��x�̏������ȗ�)
			{
				strcpy(szShowAfValid, g_lpszAfValidResultShow[1]);	//"G(AF)" AF����
				strcpy(szAfP502, g_lpszAfValidResultP502[1]);		//"AF" AF����
				return;
			}
			else if(afValid.ucResultAF == (BYTE)2)	//AF���s(������������) (���w�|�W�V�����␳���Ȃ��x�̏������ȗ�)
			{
				strcpy(szShowAfValid, g_lpszAfValidResultShow[2]);	//"N(AF)" �I�[�g�t�H�[�J�X�ُ�
				strcpy(szAfP502, g_lpszAfValidResultP502[2]);		//"NG" �I�[�g�t�H�[�J�X�ُ�
				return;
			}
		}
		else if(afValid.ucResultPopup == 2) //AF OK�{�^��
		{
			if((afValid.bit1ManuZ == 1)&&(afValid.bit1ManuAF == 0)) //Z�}�j���A������A�� ���� AF�}�j���A������i�V
			{
				strcpy(szShowAfValid, g_lpszAfValidResultShow[3]); //"G(MZ)" Z�}�j���A������A��
				strcpy(szAfP502, g_lpszAfValidResultP502[3]); //"MZ" Z�}�j���A������A��
				return;
			}
			else if((afValid.bit1ManuZ == 0)&&(afValid.bit1ManuAF == 1)&&(afValid.bit1ManuAfRslt == 0)) //Z�}�j���A������i�V ���� AF�}�j���A������A�� ���� �}�j���A���t�H�[�J�X���쐬��
			{
				strcpy(szShowAfValid, g_lpszAfValidResultShow[4]); //"G(MF)" �}�j���A���t�H�[�J�X���쐬��
				strcpy(szAfP502, g_lpszAfValidResultP502[4]); //"MF" �}�j���A���t�H�[�J�X���쐬��
				return;
			}
			else if((afValid.bit1ManuZ == 0)&&(afValid.bit1ManuAF == 1)&&(afValid.bit1ManuAfRslt == 1)) //Z�}�j���A������i�V ���� AF�}�j���A������A�� ���� �}�j���A���t�H�[�J�X���쎸�s
			{
				strcpy(szShowAfValid, g_lpszAfValidResultShow[5]); //"N(MF)" �}�j���A���t�H�[�J�X���쎸�s
				strcpy(szAfP502, g_lpszAfValidResultP502[5]); //"NG"
				return;
			}
			else if((afValid.bit1ManuZ == 0)&&(afValid.bit1ManuAF == 0)) //Z�}�j���A������i�V ���� AF�}�j���A������i�V
			{

				if(afValid.ucResultAF == (BYTE)0)//AF����
				{
					if(afValid.bit2PopupKind == (BYTE)1)	//�|�C���g�ʒu�␳���
					{
						strcpy(szShowAfValid, g_lpszAfValidResultShow[0]);	//"-----" �I�[�g�t�H�[�J�X�Ȃ�
						strcpy(szAfP502, g_lpszAfValidResultP502[0]);		//"NO"
						return;
					}
				}
				else if(afValid.ucResultAF == (BYTE)1)//AF����
				{
					if(afValid.bit2PopupKind == (BYTE)1)	//�|�C���g�ʒu�␳���
					{
						strcpy(szShowAfValid, g_lpszAfValidResultShow[1]); //"G(AF)" AF����
						strcpy(szAfP502, g_lpszAfValidResultP502[1]); //"AF" AF����
						return;
					}
				}
				else if(afValid.ucResultAF == (BYTE)2)//AF���s(������������)
				{
					if(afValid.bit2PopupKind == (BYTE)1)	//�|�C���g�ʒu�␳���
					{
						strcpy(szShowAfValid, g_lpszAfValidResultShow[2]); //"N(AF)" �I�[�g�t�H�[�J�X�ُ�
						strcpy(szAfP502, g_lpszAfValidResultP502[2]); //"N(AF)" �I�[�g�t�H�[�J�X�ُ�
						return;
					}
				}
				else if(afValid.ucResultAF == (BYTE)4)//AF���s(�|�b�v�A�b�v)
				{
					if(afValid.bit2PopupKind == (BYTE)0)		//�t�H�[�J�X�}�j���A���������
					{
						strcpy(szShowAfValid, g_lpszAfValidResultShow[2]); //"N(AF)" �I�[�g�t�H�[�J�X�ُ�
						strcpy(szAfP502, g_lpszAfValidResultP502[2]); //"N(AF)" �I�[�g�t�H�[�J�X�ُ�
						return;
					}
					else if(afValid.bit2PopupKind == (BYTE)2)	//�������
					{
						strcpy(szShowAfValid, g_lpszAfValidResultShow[2]); //"N(AF)" �I�[�g�t�H�[�J�X�ُ�
						strcpy(szAfP502, g_lpszAfValidResultP502[2]); //"N(AF)" �I�[�g�t�H�[�J�X�ُ�
						return;
					}
				}
			}
			else if((afValid.bit1ManuZ == 1)&&(afValid.bit1ManuAF == 1)) //Z�}�j���A������A�� ���� AF�}�j���A������A��
			{
//������ debug�p hibino (�\�t�g�����[�X���O�܂Œu���Ă���)
(CMainFrame*)AfxGetMainWnd()->MessageBox("afValid.bit1ManuZ == 1 )&&(afValid.bit1ManuAF == 1))", "Measurement Data View hibino", MB_OK);
//������ debug�p hibino
//				  strcpy(szShowAfValid, g_lpszAfValidResultShow[8]); //"-(--)" �z��O
//				  strcpy(szAfP502, g_lpszAfValidResultP502[8]); //"NG"
//				  return;
			}
		}
		else if(afValid.ucResultPopup == 3) //�X�L�b�v�{�^��
		{
			strcpy(szShowAfValid, g_lpszAfValidResultShow[0]);	//"-----" �I�[�g�t�H�[�J�X�Ȃ�
			strcpy(szAfP502, g_lpszAfValidResultP502[0]);		//"NO"
			return;
		}
		else if(afValid.ucResultPopup == 4) //�L�����Z��
		{
			strcpy(szShowAfValid, g_lpszAfValidResultShow[6]); //"C(MC)" �L�����Z��
			strcpy(szAfP502, g_lpszAfValidResultP502[6]); //"MC" �L�����Z��
			return;
		}

	}
	else if(afValid.ucResultAF == (BYTE)3)	//AF���s(�����X�L�b�v)
	{
		strcpy(szShowAfValid, g_lpszAfValidResultShow[0]); //"-----" �I�[�g�t�H�[�J�X�Ȃ�
		strcpy(szAfP502, g_lpszAfValidResultP502[0]); //"NO"
		return;
	}
	else if(afValid.ucResultAF == (BYTE)5)	//AF���s(���V�s�I��)
	{
		strcpy(szShowAfValid, g_lpszAfValidResultShow[7]); //"C(AC)" ���V�s�I��
		strcpy(szAfP502, g_lpszAfValidResultP502[7]); //"AC" ���V�s�I��
		return;
	}

	return;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMainFrame::JudgeChiefResult(int Value, LPCTSTR lpszCaption)
{
	//Saiki 20090527 Add ----->
	CString strBuffer;
	//Saiki 20090527 Add <-----
// 2009.11.20 K.Matsuo -->
	CString strBuffer2;
// 2009.11.20 K.Matsuo <--
	if(Value == 0) return TRUE;

	switch(Value){
	case 31:	// EMO
		AlarmIf_Set(ALID_EMOSwitchOn);
		break;;
	case 32:	// Door Interlock
		AlarmIf_Set(ALID_DoorOpen);
		break;;
	case 33:	// EQ Power
		AlarmIf_Set(ALID_EquipmentPowerOff);
		break;;
	case 34:	// ���{�b�g�A�[�����o
		AlarmIf_Set(ALID_RobotArmDetected);
		break;;
	case 35:	// �s���_�E�����I�t
		AlarmIf_Set(ALID_LifterLowestPositionSensorOff);
		break;;
	case 36:	// �G�A���͒ቺ���I��
		AlarmIf_Set(ALID_AirPressureDown);
		break;;
	case 37:	// �V���b�^�[�ُ�
		AlarmIf_Set(ALID_ShutterCloseError);
		break;;
//Kojika 20090526 Change
//	case 2L:	// ���[�J���łȂ�����
//		MessageBox("Host Mode is not Local.", lpszCaption, MB_OK|MB_ICONWARNING);
//		break;;
//	case 5L:	// Wait �łȂ�
//		MessageBox("Process Status is not Wait.", lpszCaption, MB_OK|MB_ICONWARNING);
//		break;;
//	case 6L:	// ���쒆�t���O���I��������
//		MessageBox("Actuater is moving now.", lpszCaption, MB_OK|MB_ICONWARNING);
//		break;;
//	case 7L:	// ��ʃ��[�h���u���胂�[�h�v�łȂ�����
//		MessageBox("Screen is not Measurement Screen.", lpszCaption, MB_OK|MB_ICONWARNING);
//		break;;
//	case 15:	// �����e�i���X SW �I�� && ���������e SW �I�t�̏ꍇ
//		MessageBox("Maintenance Switch is ON & Toho Maintenance Switch is OFF.", lpszCaption, MB_OK|MB_ICONWARNING);
//		break;;
//	default:	// ���̑��̃G���[
//		MessageBox("Measurement was failed.", lpszCaption, MB_OK|MB_ICONWARNING);
//		break;
	case 2L:	// ���[�J���łȂ�����
//		MessageBox("�z�X�g���[�h�����[�J���ł͂���܂���B", lpszCaption, MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_HOST_MODE_NOT_LOCAL, strBuffer, "Host Mode is not Local.");
		MessageBox(strBuffer, lpszCaption, MB_OK|MB_ICONWARNING);
		break;;
	case 5L:	// Wait �łȂ�
//		MessageBox("�A�C�h����Ԃł͂���܂���B", lpszCaption, MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_PROCESS_STATUS_NOT_WAIT, strBuffer, "Process Status is not Wait.");
		MessageBox(strBuffer, lpszCaption, MB_OK|MB_ICONWARNING);
		break;;
	case 6L:	// ���쒆�t���O���I��������
//		MessageBox("�A�N�`���G�[�^�����쒆�ł�", lpszCaption, MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_ACTUATER_MOVING, strBuffer, "Actuater is moving now.");
		MessageBox(strBuffer, lpszCaption, MB_OK|MB_ICONWARNING);
		break;;
	case 7L:	// ��ʃ��[�h���u���胂�[�h�v�łȂ�����
//		MessageBox("��ʂ����胂�[�h�ɂȂ��Ă��܂���", lpszCaption, MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_SCREEN_NOT_MEASUREMENT, strBuffer, "Screen is not Measurement Screen.");
		MessageBox(strBuffer, lpszCaption, MB_OK|MB_ICONWARNING);
		break;;
	case 15:	// �����e�i���X SW �I�� && ���������e SW �I�t�̏ꍇ
//		MessageBox("�����e�i���X�X�C�b�`��ON ���� ���������e�i���X�X�C�b�`��OFF�ɂȂ��Ă܂�", lpszCaption, MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_MAINTE_SWITCH_ON_AND_TOHO_OFF, strBuffer, "Maintenance Switch is ON & Toho Maintenance Switch is OFF.");
		MessageBox(strBuffer, lpszCaption, MB_OK|MB_ICONWARNING);
		break;;
//2009.07.30 sawada -->
// �w�b�h�^�C�v�ُ�
	case 16:
		LoadStringML(IDS_HEAD_TYPE_ERROR,strBuffer, "Illegal Head Type.");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
//2009.07.30 sawada <--
// 2009.11.25 K.Matsuo -->
	case 48:
		LoadStringML(IDS_REFFILE_ELAPSED_TIMEOUT, strBuffer, "Reference File No Exist or Elapsed TimeOut.");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
	case 49:
		LoadStringML(IDS_TR_LAMP_POWER_OFF, strBuffer, "Transmittance Lamp Power Off.");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
	case 50:
		LoadStringML(IDS_CTA_UNIT_EXECUTING, strBuffer, "Contact Angle Unit Executing.(CTA IL PI Off.)");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
	case 52:
		LoadStringML(IDS_RS_UNIT_EXECUTING, strBuffer, "Resist Unit Executing.(Probe Not Upper Position.)");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
	case 53:
		LoadStringML(IDS_RS_PROBE_SENSOR_ERR1, strBuffer, "Resist Unit Probe Position Sensor Error.(Upper On. Lower On.)");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
	case 54:
		LoadStringML(IDS_RS_PROBE_SENSOR_ERR2, strBuffer, "Resist Unit Probe Position Sensor Error.(Upper Off. Lower Off.)");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
	case 55:
		LoadStringML(IDS_MS_UNIT_SENSOR_ERR, strBuffer, "MicroScope Unit Position Error.(Lower2 On)");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
	case 76:
		LoadStringML(IDS_SAFETY_PLUG_OPEN, strBuffer, "�Z�[�t�e�B�v���O�J");
		MessageBox(strBuffer,lpszCaption,MB_OK|MB_ICONWARNING);
		break;
// 2009.11.25 K.Matsuo <--
	default:	// ���̑��̃G���[
//		MessageBox("����Ɏ��s���܂���", lpszCaption, MB_OK|MB_ICONWARNING);
// 2009.11.20 K.Matsuo -->
//		LoadStringML(IDS_MEASUREMENT_FAILED, strBuffer, "Measurement was failed.");
		LoadStringML(IDS_MEASUREMENT_FAILED, strBuffer2, "Measurement was failed.");
		strBuffer.Format("%s (ErrCode : %02d)", strBuffer2, Value);
// 2009.11.20 K.Matsuo <--
		MessageBox(strBuffer, lpszCaption, MB_OK|MB_ICONWARNING);
		break;
//Kojika 20090526 Change End
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMeasurement(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg2);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateRecipeSetting(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg2);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateLogon(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnSysColorChange()
{
	CFrameWnd::OnSysColorChange();

	if(m_bOrgSystemColorChangeExecFlg == FALSE){
		return;
	}

	for (int iIndex = 0 ; iIndex < 50 ; iIndex++ )
		if (m_OrgSystemColor[iIndex] != ::GetSysColor(iIndex))
			SetSysColors(1,&iIndex,&m_OrgSystemColor[iIndex]);
}

// ==========================================================================
//�t�@�C�����݃`�F�b�N
BOOL CMainFrame::FileOrDirExists(LPCTSTR pszFileName)
{
	return GetFileAttributes(pszFileName) != 0xffffffff;
}

// ==========================================================================
// �֐����FBOOL DeleteDirectory(LPCTSTR lpPathName)
// ���@���FlpPathName�@�폜����f�B���N�g���̃p�X����������NULL�ŏI��镶����B
// �߂�l�F�֐�����������ƁA0�ȊO�̒l���Ԃ�A�֐������s����ƁA0���Ԃ�܂��B
// ���@���F�f�B���N�g�����ۂ��ƍ폜����B
BOOL CMainFrame::DeleteDirectory(LPCTSTR lpPathName)
{
	CFileFind fnd;
	CString strPathName = lpPathName;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.*");
	if(fnd.FindFile(strPathName, 0)){
		int i = 1;
		while(i){
			i = fnd.FindNextFile();

			// �t�@�C������"."��".."�̏ꍇ�͎�������
			if(fnd.IsDots())
				continue;

			// �폜����t�@�C�����擾
			// GetFilePath()�ɂ̓o�O�����萳�m�Ɏ擾�ł��Ȃ��ꍇ������̂Ŏg��Ȃ�
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// �t�H���_�������ꍇ�A�ċA�Ăяo���ł��̃t�H���_���폜
			if(fnd.IsDirectory())
				DeleteDirectory(strDeleteFile);

			// �t�@�C���̍폜
			else
				::DeleteFile(strDeleteFile);
		}
		fnd.Close();

		// �t�H���_�̍폜
		return ::RemoveDirectory(lpPathName);
	}
	return FALSE;
}

// ==========================================================================
//
void  CMainFrame::GetStrategyhead(const char* szStrategyEntry, char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], int *iDispLabelCount)
{
	int iDummyStartWaveLen;
	int iDummyEndWaveLen;
	int iDummyOpticalFilter;
	RCP_DATA rcp_data;
	int iHeadCount = 0;
	TCHAR szDdeTextItem[1024];
	int i =0;
	char szWave[ADAPRESULTSTRINGLENMAX + 1];
	char *token;
	char szUnit[DISP_UNIT_NAME_LEN + 1];	//	���茋�ʒP�ʖ�

	memset(&rcp_data, 0, sizeof(rcp_data));
	memset(szDdeTextItem, 0, sizeof(szDdeTextItem));
	memset(szWave, 0, sizeof(szWave));
	memset(szUnit, 0, sizeof(szUnit));

	m_pDoc->GetRcpData(&rcp_data);
	m_pDoc->GetUnitName(szUnit);//	���茋�ʒP�ʖ��̎擾

	//�X�g���e�W�B��
	switch(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType){
		case MEAS_PROG_TYPE_SR_THICKNESS:

			// �X�g���e�W�[���̑��荀�ڃC���f�b�N�X��ǉ�
			if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, szStrategyEntry) ){
				token = strtok(szDdeTextItem, _T(","));
				for (iHeadCount=0; token != NULL ; iHeadCount++)
				{
					strcpy(szDispLabel[iHeadCount], token);
					token = strtok(NULL, _T(","));
				}
			}

			if(rcp_data.MeasProgInfo.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag == TRUE){
				for(i=0; i<3; i++){
					if(rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen[i] != 0){
//						sprintf(szWave, "%s%d%s", "[", rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen[i], "nm]");
						sprintf(szWave, "%d%s", rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen[i], "nm[%]");
						memcpy(szDispLabel[iHeadCount], szWave, sizeof(szWave));
						iHeadCount++;
					}
				}
			}

			//�u������
			for(i = 0; szDispLabel[i][0] != '\0'; i++){
				//Thick1�`3�ɒP�ʂ�t��
				if(strstr(szDispLabel[i], "Thick") != NULL){
					char szUnitPlus[ADAPRESULTSTRINGLENMAX + 1];
					memset(szUnitPlus, 0, sizeof(szUnitPlus));
					sprintf(szUnitPlus, "%s%s%s", "[", szUnit, "]");
					strcat(szDispLabel[i], szUnitPlus);
				}

				//�wMSE��Fit�x�̒u��
				if(strcmp(szDispLabel[i], "MSE") == 0){
					strcpy(szDispLabel[i], "Fit");
				}
			}

			break;

		case MEAS_PROG_TYPE_SR_REFLECTANCE:
// 2009.10.26 K.Matsuo -->
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2009.10.26 K.Matsuo <--
			for(i = 0; i < 3; i++){
				if(rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen[i] != 0){
//					sprintf(szWave, "%s%d%s", "[", rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen[i], "nm]");
					sprintf(szWave, "%d%s", rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen[i], "nm[%]");
					memcpy(szDispLabel[iHeadCount], szWave, sizeof(szWave));
					iHeadCount++;
				}
			}
			break;

		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
		case MEAS_PROG_TYPE_SR_MAX:
		default:
			//�쐬���_�̎d�l�ł͕s�v
			break;
	}
	*iDispLabelCount = iHeadCount;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::MaintenanceCheck()
{
	// Maintenance Date�̃`�F�b�N
	COleDateTime mDate;
	mDate = COleDateTime::GetCurrentTime();

	m_pMaintenanceDate = m_pDoc->GetMaintenanceDate();
	if(m_pMaintenanceDate.nAlarmFlag){
		if(mDate.m_dt > mNextDate.m_dt){
			// �����̃A���[������������
			AlarmIf_Set(ALID_MaintenancePeriodElapsed);
//			MessageBox("�����e�i���X�������o�߂��܂��� !!!.", "", MB_OK|MB_ICONWARNING);
		}
	}

	// SR Reflectance Lamp�̎g�p���ԉ��Z
	m_pMaintenanceLamp = m_pDoc->GetMaintenanceLamp();
	m_pPassageHour.m_span = m_pMaintenanceLamp.MaintenanceHour;
	m_pPassageHour.operator +=( m_100ms );
	m_pMaintenanceLamp.MaintenanceHour = m_pPassageHour.m_span;
	m_pDoc->SetMaintenanceLamp( m_pMaintenanceLamp );

	COleDateTimeSpan m_500Hour(0,500,0,0);
	COleDateTimeSpan m_1000Hour(0,1000,0,0);
	COleDateTimeSpan m_1500Hour(0,1500,0,0);
	COleDateTimeSpan m_2000Hour(0,2000,0,0);
	COleDateTimeSpan m_2500Hour(0,2500,0,0);
	COleDateTimeSpan m_3000Hour(0,3000,0,0);
	COleDateTimeSpan mHour;

	// SR Reflectance Lamp�̃`�F�b�N����������
	switch( m_pMaintenanceLamp.nHourSpan ){
	case HOURS_500:
		mHour = m_500Hour;
		break;
	case HOURS_1000:
		mHour = m_1000Hour;
		break;
	case HOURS_1500:
		mHour = m_1500Hour;
		break;
	case HOURS_2000:
		mHour = m_2000Hour;
		break;
	case HOURS_2500:
		mHour = m_2500Hour;
		break;
	case HOURS_3000:
		mHour = m_3000Hour;
		break;
	}

	if( m_pMaintenanceLamp.nAlarmFlag ){
		if( m_pPassageHour.m_span > mHour.m_span ){
			// �����̃A���[������������
			AlarmIf_Set(ALID_LampExchangePeriodElapsed);
//			MessageBox("�����v�����������o�߂��܂��� !!!.", "", MB_OK|MB_ICONWARNING);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
int CMainFrame::MeasEnd()
{
// Kojika 20090526 Add
	CString strBuffer;
// Kojika 20090526 Add End
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	CString strBuffer2;
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

	int i;
	int iWnd;
	RCP_DATA rcp_data;
	int iFormatId = 0;

	char szLabelOri[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	STATISTICS statistics[ADAPRESULT_COLS_MAX];
	STATISTICSDATESET pifIf[ADAPRESULT_COLS_MAX];
	::ZeroMemory(szLabelOri, sizeof(szLabelOri));
	::ZeroMemory(statistics, sizeof(statistics));
	::ZeroMemory(pifIf, sizeof(pifIf));

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	iWnd = pMainFrame->GetCurrExeWnd();
	m_pDoc->GetRcpData(&rcp_data);
	m_pDoc->SetShowCompPointList(0);
	m_pDoc->SetShowCompPointGraph(0);

	int iSetPointCount; //�i�[�ό���(���|�C���g���肵�����̐�)
	iSetPointCount = m_pDoc->GetStoreCount();


	BOOL bSampleID = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSampleID; //�T���v��ID���g�p�H
	BOOL bSaveMeasResult = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult; //�Z�[�u�t�@�C�����ɃT���v��ID���g�p?(bSampleID==TRUE���̂ݗL��)
	BOOL bSaveOverwrite = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite; //�Z�[�u�t�@�C���������t�@�C�����������ꍇ�A�㏑���H

	BOOL bExitSaveflg = FALSE; //EXIT�{�^���������Z�[�u���s�t���O

	//2009.11.07 bagus MS --{--
	// ����������̎��ɂ͕ۑ����Ȃ�
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		bExitSaveflg = TRUE;
	}else{
	//2009.11.07 bagus MS --}--

	//EXIT�{�^���������ɃZ�[�u���Ă��Ȃ����̂�����ꍇ�̏���
	if((m_pMeaTabViewObj->m_bToolbarSaveCompFlg == TRUE) //�c�[���o�[�ɂ�鑪��f�[�^�ۑ��L
	&&(!((bSampleID == TRUE)&&(bSaveMeasResult == TRUE)))) //���� �T���v��ID�ɂ�鎩���ۑ��ݒ�łȂ�
	{
		TRACE("%s%s\n", "(CMainFrame::MeasEnd) :", "�c�[���o�[�ɂ�鑪��f�[�^�ۑ��L ���� �T���v��ID�ɂ�鎩���ۑ��ݒ�łȂ�"); //�����̃g���[�X�͍폜�� hibino
		//���ɕۑ��ς݂Ȃ̂ŁAEXIT���ɂ͕ۑ����Ȃ��B
	}
	else if(/*m_pMainFrame->*/m_pMeaTabViewObj->m_bExitSaveCompFlg == FALSE) //����f�[�^���ۑ�
	{
		if( (m_pDoc->GetHostMode() != HOST_LOCAL)
			||((bSampleID == TRUE)&&(bSaveMeasResult == TRUE)) )
		{
			bExitSaveflg = TRUE;
		}
		else
		{
			int iRtn;
			LoadStringML(IDS_SAVE_THIS_DATA, strBuffer, "Do you save this data?");
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
//			iRtn = MessageBox(strBuffer, "NanoSpec", MB_YESNOCANCEL); //�m�F�_�C�A���O
			LoadStringML(IDS_TITLE_NANOSPEC , strBuffer2, "NanoSpec");
			iRtn = MessageBox(strBuffer, strBuffer2, MB_YESNOCANCEL); //�m�F�_�C�A���O
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--
			if(iRtn == IDYES)
			{
				bExitSaveflg = TRUE;
			}
			else if(iRtn == IDNO)
			{
				bExitSaveflg = FALSE;
			}
			else //�L�����Z��
			{
				return IDCANCEL; //EXIT�{�^���Ŕ������A�����ʂ̂܂�
			}

		}

		if(bExitSaveflg == TRUE) //���ۑ��f�[�^�ۑ���
		{
			int iCheck = /*m_pMainFrame->*/m_pMeaTabViewObj->SaveMeasConAddRevi(rcp_data.szRecipeName, (int)MEASUREMENT_EXIT_SAVE);

			if(iCheck == 0) //�ۑ����s
			{
				AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //���茋�ʕۑ����s
				if ( m_pDoc->GetHostMode() == HOST_LOCAL ) {
					//�Z�[�u���s
					LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
					MessageBox(strBuffer, "NanoSpec", MB_OK);
				}
			}

			if((iCheck == 0)||(iCheck == 3)) //�ۑ����s OR �ۑ��L�����Z��
			{
				//�����ʂɂƂǂ܂�B
				return IDCANCEL;
			}
		}
	}
	//2009.11.07 bagus MS --{--
	}
	//2009.11.07 bagus MS --}--

	///// ����f�[�^���t�H���_���c���Ă�����폜 /////
	if ( FileOrDirExists(g_szData_Measured_Data_Temp_Dir) ) {
		/*m_pMainFrame->*/DeleteDirectory(g_szData_Measured_Data_Temp_Dir);
	}


	if(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) //�ʏ푪��E�����W���[
	{
		m_pMeaTabViewObj->ButtonEnableChange(MEAS_TAB_VIEW_DELETE);
	}

	//���胂�[�h�t���O�ݒ�
	m_pDoc->SetMeasMode(MEASMODE_NOT_MEASURE); //���蒆�łȂ�(���茋�ʉ�ʂ�WAIT���͊܂܂Ȃ�)


	::ResetEvent(m_hEvTabView);


	switch(iWnd){
	case SYSCONF_Z_AXIS_OFFSET_MEAS_WND:
		pMainFrame->ChangeOperationPanel(OP_SYSTEM);
		pMainFrame->ChangeExeWnd(SYSCONF_WND);
		break;


	case MANUAL_MEAS_FORM_WND:
		HookKeyUnhook();
	//[��]break�͕s�v

	case MEAS_WND:
	case MANUAL_MEAS_WND: //�����g�p�H

		//��ʂւ̕�
		if ( iSetPointCount > 0 ) {
			m_pDoc->GetStatisticsData(statistics/*, iSetPointCount*/);
			m_pDoc->GetLabelOri(szLabelOri);
			for ( i = 0; i < ADAPRESULT_COLS_MAX; i++ ) {
				pifIf[i].dMin = statistics[i].dMinimum;
				pifIf[i].dMax = statistics[i].dMaximum;
				pifIf[i].dAve = statistics[i].dAverage;
				pifIf[i].dSd = statistics[i].dStandardDeviation;
				pifIf[i].dUnif = 0.0;								// �[���ł悢�BUniformity�́A�e�Ђł��낢��Ȍv�Z���W�b�N������炵���BProcon���Ōv�Z����B
			}
		}

		if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR ) {
			switch ( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType ) {
			case MEAS_PROG_TYPE_SR_THICKNESS:
				iFormatId = 1;
				break;
			case MEAS_PROG_TYPE_SR_REFLECTANCE:
				iFormatId = 2;
				break;
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
				iFormatId = 3;
				break;
			case MEAS_PROG_TYPE_SR_DISTANCE:
				iFormatId = 0;
				break;
			default:
				iFormatId = 0;
				break;
			}
			PifComm_CompleteMeasureAllPointsReport(iFormatId, iSetPointCount, szLabelOri, pifIf, rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen);
		}
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SE ) {
			iFormatId = 4;
			PifComm_CompleteMeasureAllPointsReport(iFormatId, iSetPointCount, szLabelOri, pifIf, rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen);
		}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE ) {
			iFormatId = 7;
			PifComm_CompleteMeasureAllPointsReport(iFormatId, iSetPointCount, szLabelOri, pifIf, rcp_data.MeasProgInfo.ScanParams._SR.iSpecificWavelen);
		}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP ) {
			PifComm_CompleteMeasureResistAllPointsReport(iSetPointCount, pifIf);
		}
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA ) {
			PifComm_CompleteMeasureContactAngleAllPointsReport(iSetPointCount, pifIf);
		}
		else if ( rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS ) {
			// �X�g���X������̖�������́A�񍐂��܂���
			;
		}
		else {
			// ���Ή��I�H
			;
		}
		break;
	default:
		m_pDoc->OperationLogging("CMainFrame::MeasEnd()�� default�ʉ�");
		pMainFrame->ChangeOperationPanel(OP_MEASUREMENT);
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_LIST_WND);
		break;
	}

	return bExitSaveflg ? IDYES : IDNO;
}

/////////////////////////////////////////////////////////////////////////////
//���C�����V�s�I����ʂ̃��j���[�o�[��OPERATION��
void CMainFrame::OnMeasurementStart()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MEASUREMENT_START, (WPARAM)WM_DISPMENU_MEASUREMENT_START, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMeasurementStart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnManualMeasurement()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MANUAL_MEASUREMENT, (WPARAM)WM_DISPMENU_MANUAL_MEASUREMENT, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateManualMeasurement(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnHandlerLoadPosition()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_HANDLER_LOAD_POSITION, (WPARAM)WM_DISPMENU_HANDLER_LOAD_POSITION, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateHandlerLoadPosition(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnManualStage()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MANUAL_STAGE, (WPARAM)WM_DISPMENU_MANUAL_STAGE, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateManualStage(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnVacuum()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_VACUUM, (WPARAM)WM_DISPMENU_VACUUM, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateVacuum(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnReference()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_REFERENCE, (WPARAM)WM_DISPMENU_REFERENCE, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateReference(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//�ʏ푪���ʂ̃��j���[�o�[��OPERATION��

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMeasurementCancel()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MEASUREMENT_CANCEL, (WPARAM)WM_DISPMENU_MEASUREMENT_CANCEL, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMeasurementCancel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMeasurementPause()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MEASUREMENT_PAUSE, (WPARAM)WM_DISPMENU_MEASUREMENT_PAUSE, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMeasurementPause(CCmdUI* pCmdUI)
{
	if(m_MeasMenuEnableFlg == TRUE)
	{
		pCmdUI->Enable(!(m_pDoc->GetPauseFlg()));
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMeasurementResume()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MEASUREMENT_RESUME, (WPARAM)WM_DISPMENU_MEASUREMENT_RESUME, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMeasurementResume(CCmdUI* pCmdUI)
{
//	BOOL asd = m_pDoc->GetPauseFlg();

	if(m_MeasMenuEnableFlg == TRUE)
	{
		pCmdUI->Enable(m_pDoc->GetPauseFlg());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnRemeasurement()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_REMEASUREMENT, (WPARAM)WM_DISPMENU_REMEASUREMENT, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateRemeasurement(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnAdujstRemeasurement()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_ADUJST_REMEASUREMENT, (WPARAM)WM_DISPMENU_ADUJST_REMEASUREMENT, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateAdujstRemeasurement(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnSeqmeasExit()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_SEQMEAS_EXIT, (WPARAM)WM_DISPMENU_SEQMEAS_EXIT, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateSeqmeasExit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//�}�j���A�������ʂ̃��j���[�o�[��OPERATION��

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnManualMeasurementStart()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MANUAL_MEASUREMENT_START, (WPARAM)WM_DISPMENU_MANUAL_MEASUREMENT_START, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateManualMeasurementStart(CCmdUI* pCmdUI)
{
/* modified 2016.05.14 hmenjo �蓮 menu �o�O ---------- { ---------- */
//	pCmdUI->Enable(m_ManualMeasMenuEnableFlg);
/* modified 2016.05.14 hmenjo �蓮 menu �o�O ----------              */
	if (m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) {
		pCmdUI->Enable(m_ManualMeasMenuEnableFlg);
	} else {
		if (m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP) {
			pCmdUI->Enable(FALSE);
		} else {
			pCmdUI->Enable(m_ManualMeasMenuEnableFlg);
		}
	}
/* modified 2016.05.14 hmenjo �蓮 menu �o�O ---------- } ---------- */
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnMeasurementComplete()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MEASUREMENT_COMPLETE, (WPARAM)WM_DISPMENU_MEASUREMENT_COMPLETE, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateMeasurementComplete(CCmdUI* pCmdUI)
{
/* modified 2016.05.14 hmenjo �蓮 menu �o�O ---------- { ---------- */
//	pCmdUI->Enable(m_ManualMeasMenuEnableFlg);
/* modified 2016.05.14 hmenjo �蓮 menu �o�O ----------              */
	if (m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) {
		pCmdUI->Enable(m_ManualMeasMenuEnableFlg);
	} else {
		if (m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP) {
			pCmdUI->Enable(FALSE);
		} else {
			pCmdUI->Enable(m_ManualMeasMenuEnableFlg);
		}
	}
/* modified 2016.05.14 hmenjo �蓮 menu �o�O ---------- } ---------- */
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnPointTeaching()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_POINT_TEACHING, (WPARAM)WM_DISPMENU_POINT_TEACHING, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdatePointTeaching(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ManualMeasMenuEnableFlg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnManumeasExit()
{
	(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->PostMessage(WM_DISPMENU_MANUMEAS_EXIT, (WPARAM)WM_DISPMENU_MANUMEAS_EXIT, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateManumeasExit(CCmdUI* pCmdUI)
{
	if((m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)&&(m_pDoc->GetManuMeasMode() == MANUMODE_ON_MEASURE))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(m_ManualMeasMenuEnableFlg);
	}
}
/* added 2016.05.14 hmenjo �蓮 menu �o�O ---------- { ---------- */
void CMainFrame::OnSaveAs()
{
//������͎g���܂���	m_pMeaTabViewObj->PostMessage(WM_COMMAND, (WPARAM) MAKEWPARAM(IDM_SAVE_AS_RESULT_DATA, 0), (LPARAM) 0);
	(((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0))->PostMessage(WM_COMMAND, (WPARAM) MAKEWPARAM(IDM_SAVE_AS_RESULT_DATA, 0), (LPARAM) 0);
}
void CMainFrame::OnUpdateSaveAs(CCmdUI* pCmdUI)
{
	if (m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) {
//	if (0) {
		/* �蓮���胂�[�h	*/
		if(m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP) {
			pCmdUI->Enable(TRUE);
		} else {
			pCmdUI->Enable(FALSE);
		}
	}
}
/* added 2016.05.14 hmenjo �蓮 menu �o�O ---------- } ---------- */
/* added 2016.08.05 hmenjo ���V�s�ҏW menu �o�O ---------- { ---------- */
void CMainFrame::OnNew()
{
	if (m_iCurrMode == OP_RECIPE_SETTING) {
		/* ���V�s�ҏW���[�h	*/
		(((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0))->PostMessage(WM_COMMAND, (WPARAM) MAKEWPARAM(IDM_NEW, 0), (LPARAM) 0);
	}
}
void CMainFrame::OnUpdateNew(CCmdUI* pCmdUI)
{
	BOOL l_bRc = FALSE;
	if (m_iCurrMode == OP_RECIPE_SETTING) {
		/* ���V�s�ҏW���[�h	*/
		/* ���[�U�̎��s���`�F�b�N	*/
/* modified 2016.08.15 hmenjo ���V�s�ҏW menu �o�O3 ---------- { ---------- */
//		l_bRc = m_pDoc->User_Access(ACCESS_RECIPE);
/* modified 2016.08.15 hmenjo ���V�s�ҏW menu �o�O3 ----------              */
		l_bRc = ((CNanoRecipeListUI*) (((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0)))->IsAccessPrivilege();
/* modified 2016.08.15 hmenjo ���V�s�ҏW menu �o�O3 ---------- } ---------- */
		if (FALSE != l_bRc) {
			/* ���s�\	*/
			int l_iItemCount = ((CNanoListUI*) (((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0)))->m_lstCtrl.GetItemCount();
			if (RCP_PGM_NUM_MAX <= l_iItemCount) {
				/* ���X�g�̍ő吔�ɒB���Ă���̂Œǉ��s�\	*/
				l_bRc = FALSE;
			}
		}
	}
	pCmdUI->Enable(l_bRc);
}
void CMainFrame::OnDelete()
{
	if (m_iCurrMode == OP_RECIPE_SETTING) {
		/* ���V�s�ҏW���[�h	*/
		(((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0))->PostMessage(WM_COMMAND, (WPARAM) MAKEWPARAM(IDM_DELETE, 0), (LPARAM) 0);
	}
}
void CMainFrame::OnUpdateDelete(CCmdUI* pCmdUI)
{
	BOOL l_bRc = FALSE;
	if (m_iCurrMode == OP_RECIPE_SETTING) {
		/* ���V�s�ҏW���[�h	*/
		/* ���[�U�̎��s���`�F�b�N	*/
/* modified 2016.08.15 hmenjo ���V�s�ҏW menu �o�O3 ---------- { ---------- */
//		l_bRc = m_pDoc->User_Access(ACCESS_RECIPE);
/* modified 2016.08.15 hmenjo ���V�s�ҏW menu �o�O3 ----------              */
		l_bRc = ((CNanoRecipeListUI*) (((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0)))->IsAccessPrivilege();
/* modified 2016.08.15 hmenjo ���V�s�ҏW menu �o�O3 ---------- } ---------- */
		if (FALSE != l_bRc) {
			/* ���s�\	*/
			UINT l_uiSelectedCount = ((CNanoListUI*) (((CMainFrame*) AfxGetMainWnd())->m_wndSplitter2.GetPane(0, 0)))->m_lstCtrl.GetSelectedCount();
			if (1 != l_uiSelectedCount) {
				/* �I���s�������C���邢�́C�����Ȃ̂Ŏ��s�s�\	*/
				l_bRc = FALSE;
			}
		}
	}
	pCmdUI->Enable(l_bRc);
}
/* added 2016.08.05 hmenjo ���V�s�ҏW menu �o�O ---------- } ---------- */

// =========================================================================
//�e�X�g���[�h�p�����V�s�폜(�e�X�g���[�h���ُ�I�����āA�c���Ă��܂��Ă��������V�s���폜)
void CMainFrame::TestModeTempRcpDelete()
{
	char szDeleteRcpPathMain[255 + 1];
	char szDeleteRcpPathMeas[255 + 1];
	char szDeleteRcpPathRef[255 + 1];
	//2010.01.13 bagus GTR --{--
	char szDeleteRcpPathStage[255 + 1];
	//2010.01.13 bagus GTR --}--
	memset(szDeleteRcpPathMain, 0, sizeof(szDeleteRcpPathMain));
	memset(szDeleteRcpPathMeas, 0, sizeof(szDeleteRcpPathMeas));
	memset(szDeleteRcpPathRef, 0, sizeof(szDeleteRcpPathRef));
	//2010.01.13 bagus GTR --{--
	memset(szDeleteRcpPathStage, 0, sizeof(szDeleteRcpPathStage));
	//2010.01.13 bagus GTR --}--

	sprintf(szDeleteRcpPathMain, "%s%c%s%s", g_szDb_Main_Recipe_Dir, SYSTEM_RECIPE_BEGINNING_CHAR, TESTMODE_TEMP_RECIPE_NAME, MAINRECIPE_EXT);
	sprintf(szDeleteRcpPathMeas, "%s%c%s%s", g_szDb_Measurement_Program_Dir, SYSTEM_RECIPE_BEGINNING_CHAR, TESTMODE_TEMP_RECIPE_NAME, MEASUREMENTPGM_EXT);
	sprintf(szDeleteRcpPathRef, "%s%c%s%s", g_szData_Ref_Dir, SYSTEM_RECIPE_BEGINNING_CHAR, TESTMODE_TEMP_RECIPE_NAME, DAT_EXT);
	//2010.01.13 bagus GTR --{--
	sprintf(szDeleteRcpPathStage, "%s%c%s%s", g_szDb_Stage_Program_Dir, SYSTEM_RECIPE_BEGINNING_CHAR, TESTMODE_TEMP_RECIPE_NAME, STAGEPGM_EXT);
	//2010.01.13 bagus GTR --}--

	DeleteFile( szDeleteRcpPathMain );
	DeleteFile( szDeleteRcpPathMeas );
	DeleteFile( szDeleteRcpPathRef );
	//2010.01.13 bagus GTR --{--
	DeleteFile( szDeleteRcpPathStage );
	//2010.01.13 bagus GTR --}--

	return;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::MaintenanceDlgOpen()
{
	if(m_pDoc->GetInitFinished() == FALSE) return;

	static BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	static BOOL sbNexMntSwOld = TRUE;
	static BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();
	static BOOL sbNexEngiMntSwOld = FALSE;

	sbNexMntSw = nexioIsMaintenanceSwitch();
	sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if((sbNexMntSw != sbNexMntSwOld)||(sbNexEngiMntSw != sbNexEngiMntSwOld)){
		if(((sbNexMntSw == OFF)||(sbNexEngiMntSw == ON)) && (m_pMntNoticeDlg == NULL)){
			m_pMntNoticeDlg = new CMaintenanceNoticeDlg();
			m_pMntNoticeDlg->Create(CMaintenanceNoticeDlg::IDD);
			m_pMntNoticeDlg->ShowWindow(SW_SHOWNORMAL);
			AfxGetMainWnd()->SetForegroundWindow();
		}
		else if(((sbNexMntSw == ON)&&(sbNexEngiMntSw == OFF)) && (m_pMntNoticeDlg != NULL)){
			m_pMntNoticeDlg->DestroyWindow();
			delete m_pMntNoticeDlg;
			m_pMntNoticeDlg = NULL;
		}
		else{
		}
	}

	sbNexMntSwOld = sbNexMntSw;
	sbNexEngiMntSwOld = sbNexEngiMntSw;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::ChangeMaintenanceModeColor(int iMode)
{
	static COLORREF crBackup[5]; //
	int iElements[] = {COLOR_ACTIVECAPTION, COLOR_ACTIVEBORDER, COLOR_MENU, COLOR_WINDOWFRAME, COLOR_SCROLLBAR,};
	COLORREF crRgbValues[] = {RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 0, 0),};
	//�A�N�e�B�u�E�B���h�E�̃^�C�g���o�[�̐F, �A�N�e�B�u�E�B���h�E�̋��E�̐F, ���j���[�̔w�i�F, �E�B���h�E�̘g�̐F, �X�N���[���o�[�̎��̐F

	int i = 0;

	switch(iMode){
		case MAINFRM_MAINTE_MODE:
			m_bOrgSystemColorChangeExecFlg = FALSE; 	// �F�����ɖ߂���Ȃ��悤�ɐݒ�
			for(i=0; i<5; i++)
			{
				crBackup[i] = GetSysColor(iElements[i]);
			}
			SetSysColors(5, iElements, crRgbValues);
			m_bOrgSystemColorChangeExecFlg = TRUE;
			break;

		case MAINFRM_NORMAL_MODE:
			m_bOrgSystemColorChangeExecFlg = FALSE; 	// �F�����ɖ߂���Ȃ��悤�ɐݒ�
			SetSysColors(5, iElements, crBackup);
			m_bOrgSystemColorChangeExecFlg = TRUE;
			break;

		case MAINFRM_QUIT_MODE: //
			m_bOrgSystemColorChangeExecFlg = TRUE;
			SetSysColors(5, iElements, crBackup);
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OldMeasDataDelete()
{
	SYSTEMTIME stDeleteBaseTime;
	char szDefaultCurrentPath[1024+1];
	memset(&stDeleteBaseTime, 0 , sizeof(stDeleteBaseTime));
	memset(szDefaultCurrentPath, 0, sizeof(szDefaultCurrentPath));

	//�f�[�^�폜����擾
	BOOL bRtnChk = GetDeleteDate(&stDeleteBaseTime);

	if(bRtnChk == FALSE){ //�폜�ݒ�Ȃ�
		//�ȍ~�����Ȃ�
		return;
	}

	//�J�����g�f�B���N�g������
	GetCurrentDirectory(sizeof(szDefaultCurrentPath), szDefaultCurrentPath); //������Ɍ��̃J�����g�f�B���N�g���ɖ߂�ׂ̂���
	SetCurrentDirectory(g_szResult_Dir);//�J�����g�f�B���N�g���������Ώۃt�H���_�Ɉړ�

	//�폜�Ώۃt�@�C���̌����E�폜
	DeleteOldFile(stDeleteBaseTime);

	//�J�����g�f�B���N�g������(���ɖ߂�)
	SetCurrentDirectory(szDefaultCurrentPath); //�J�����g�f�B���N�g������(�f�t�H���g�̃J�����g)�ɖ߂�
}

/////////////////////////////////////////////////////////////////////////////
// �t�@�C�����폜���������擾
// [�߂�l] TRUE:�폜�Ώۃt�@�C�������K�v, FALSE:�폜�̕K�v�Ȃ��ɐݒ肳��Ă���B
//
// [���̊֐��̒��ӓ_]
// �j�����͎g�p�֎~(�j�����͐������l�������Ȃ�)�B
BOOL CMainFrame::GetDeleteDate(SYSTEMTIME *stDeleteBaseTime)
{

	int iDeleteDataPeriod;
	ConfigFile_GetNanoSpecIni(&iDeleteDataPeriod, CONFIG_FILE_DELETE_DATA_PERIOD);

	typedef struct _PERIODSEARCH{
		int nSelect;
		int iYear;
		int iMonth;
	}PERIODSEARCH, *PPERIODSEARCH;

	PERIODSEARCH PeriodSearchTable[] ={
		{ DELETE_DATA_PERIOD_NONE,				0,	0	},
		{ DELETE_DATA_PERIOD_ONE_MONTH, 		0,	1	},
		{ DELETE_DATA_PERIOD_THREE_MONTHS,		0,	3	},
		{ DELETE_DATA_PERIOD_SIX_MONTHS,		0,	6	},
		{ DELETE_DATA_PERIOD_ONE_YEAR,			1,	0	},
		{ DELETE_DATA_PERIOD_ONE_HALF_YEARS,	1,	6	},
		{ DELETE_DATA_PERIOD_TWO_YEARS, 		2,	0	},
		{ DELETE_DATA_PERIOD_TWO_HALF_YEARS,	2,	6	},
		{ DELETE_DATA_PERIOD_THREE_YEARS,		3,	0	},
		{ DELETE_DATA_PERIOD_THREE_HALF_YEARS,	3,	6	},
		{ DELETE_DATA_PERIOD_FOUR_YEARS,		4,	0	},
		{ DELETE_DATA_PERIOD_FOUR_HALF_YEARS,	4,	6	},
		{ DELETE_DATA_PERIOD_FIVE_YEARS,		5,	0	},
	};


	int iYear = 0;
	int iMonth = 0;

	for(int i = 0; i < DELETE_DATA_PERIOD_MAX; i++){
		if(iDeleteDataPeriod == PeriodSearchTable[0].nSelect){
			return FALSE; //�폜�Ȃ��ɐݒ肳��Ă���B
		}

		if(iDeleteDataPeriod == PeriodSearchTable[i].nSelect){
			iYear = PeriodSearchTable[i].iYear;
			iMonth = PeriodSearchTable[i].iMonth;
			break;
		}
	}

	SYSTEMTIME stSystemTime;
	::GetLocalTime(&stSystemTime);

	if((int)stSystemTime.wMonth > iMonth){
		stSystemTime.wYear = (int)stSystemTime.wYear - iYear;
		stSystemTime.wMonth = (int)stSystemTime.wMonth - iMonth;
	}
	else{
		stSystemTime.wYear = ((int)stSystemTime.wYear - 1) - iYear;
		stSystemTime.wMonth = ((int)stSystemTime.wMonth + 12) - iMonth;
	}

	//2,4,6,9,11���̓��t����
	if((stSystemTime.wMonth == 4)||(stSystemTime.wMonth == 6)||(stSystemTime.wMonth == 9)||(stSystemTime.wMonth == 11)){
	//4,6,9,11��
		if(stSystemTime.wDay == 31){
			stSystemTime.wDay = 30;
		}
	}
	else if(stSystemTime.wMonth == 2){	//2��
		if(stSystemTime.wDay > 28){
			//���邤�N
			if((((stSystemTime.wYear%4)==0)&&((stSystemTime.wYear%100)!=0))
			||((stSystemTime.wYear%400)==0)){
				stSystemTime.wDay = 29;
			}
			//�񂤂邤�N
			else{
				stSystemTime.wDay = 28;
			}
		}
	}
	else{	//1,3,5,7,8,10,12��
		//�����Ȃ�
	}

	*stDeleteBaseTime = stSystemTime;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// �t�@�C�����폜�������ȑO�̂��̂��폜
// [�֐��̐���]
// 1.�w.dat�t�@�C���x����сw���ɑΉ�����RAW�f�[�^����t�H���_�x������
// 2.��L1.�ō폜����Ȃ������t�H���_(�f�[�^���ޗp�t�H���_)�̒��փJ�����g�f�B���N�g�����ڂ��A�Ăт��̊֐����Ă�(�ċA)
void CMainFrame::DeleteOldFile(const SYSTEMTIME stDeleteBaseTime)
{
	CString csFileTitle;	//�g���q�Ȃ��t�@�C�����擾
	CString csFilePath; 	//�t�@�C���p�X�擾
	CTime ctSearchFileTime;
	CTime ctDeleteBaseTime(stDeleteBaseTime.wYear, stDeleteBaseTime.wMonth, stDeleteBaseTime.wDay,
//						   stDeleteBaseTime.wHour, stDeleteBaseTime.wMinute, stDeleteBaseTime.wSecond);
							0, 0, 0); //�N�����Ŕ�r����̂�0��ݒ�
	char szDrive[16+1];
	char szFilePath[256+1];
	char szDirName[256+1];
	char szFileName[128+1];
	char szExtension[16+1];
	char szRawDataDirPath[256+1]; //.dat�t�@�C���ɑΉ�����RAW�f�[�^�i�[�t�H���_�̃p�X
	memset(szDrive, 0, sizeof(szDrive));
	memset(szFilePath, 0, sizeof(szFilePath));
	memset(szDirName, 0, sizeof(szDirName));
	memset(szFileName, 0, sizeof(szFileName));
	memset(szExtension, 0, sizeof(szExtension));
	memset(szRawDataDirPath, 0, sizeof(szRawDataDirPath));

//	g_pFileRetentionPeriod[0]->FileRetentionLogging("DeleteOldFile Function");

	// ----------------------------------------------------------------------
	//[1]�w.dat�t�@�C���x����сw���ɑΉ�����RAW�f�[�^����t�H���_�x������
	BOOL bFileEnd;
	CFileFind cfFilefind;
	//Saiki 20091214 Add ----->
	CFileFind cfChkStressData;
	BOOL bStressData = FALSE;
	//Saiki 20100517 Add ----->
	char szBuff[256];
	//Saiki 20100517 Add <-----
	//Saiki 20100517 Change ----->
//	if(cfChkStressData.FindFile()){
//		do{
//			bFileEnd = cfChkStressData.FindNextFile();
//			csFilePath = cfChkStressData.GetFilePath(); //�t�@�C���p�X�擾
//			// "." , ".."�𖳎�
//			if(cfChkStressData.IsDots()){
//				continue;
//			}
//
//			//�������ʂ̔���
//			if(cfChkStressData.IsDirectory()){
//				//�t�H���_�͌�Ō���B
//				//�w�q�t�H���_�����t�H���_�x�͌�ōċA�Ō���B
//				continue;
//			}
//
//			strcpy(szFilePath, csFilePath);
//			_tsplitpath(szFilePath, szDrive, szDirName, szFileName, szExtension); //�p�X�𕪉�
//			if(strcmp(szFileName, STRESS_REFERENCE_NAME)==0){
//				//�t�@�C������%StressReference%�̏ꍇ�͈ȍ~�̏��������Ȃ�
//				bStressData = TRUE;
//			}
//		} while( bFileEnd );
//		cfChkStressData.Close();
//	}
	try{
		if(cfChkStressData.FindFile()){
			do{
				bFileEnd = cfChkStressData.FindNextFile();
				csFilePath = cfChkStressData.GetFilePath(); //�t�@�C���p�X�擾
				// "." , ".."�𖳎�
				if(cfChkStressData.IsDots()){
					continue;
				}

				//�������ʂ̔���
				if(cfChkStressData.IsDirectory()){
					//�t�H���_�͌�Ō���B
					//�w�q�t�H���_�����t�H���_�x�͌�ōċA�Ō���B
					continue;
				}

				strcpy(szFilePath, csFilePath);
				_tsplitpath(szFilePath, szDrive, szDirName, szFileName, szExtension); //�p�X�𕪉�
				if(strcmp(szFileName, STRESS_REFERENCE_NAME)==0){
					//�t�@�C������%StressReference%�̏ꍇ�͈ȍ~�̏��������Ȃ�
					bStressData = TRUE;
				}
			} while( bFileEnd );
			cfChkStressData.Close();
		}
	}
	catch(CFileException* fe){
            long ErrorCode = fe->m_lOsError;
            sprintf(szBuff, "(cfChkStressData) %s, ErrorCode = %ld", "File Invalid Error", ErrorCode);
			g_pFileRetentionPeriod[0]->FileRetentionLogging(szBuff);
	}

	//Saiki 20100517 Change <-----

	//Saiki 20091214 Add <-----
	//Saiki 20100517 Change ----->
//	if(bStressData != TRUE){
//		if(cfFilefind.FindFile()){
//			do{
//				bFileEnd = cfFilefind.FindNextFile();
//				csFilePath = cfFilefind.GetFilePath(); //�t�@�C���p�X�擾
//				// "." , ".."�𖳎�
//				if(cfFilefind.IsDots()){
//					continue;
//				}
//
//				//�������ʂ̔���
//				if(cfFilefind.IsDirectory()){
//					//�t�H���_�͌�Ō���B
//					//�w�q�t�H���_�����t�H���_�x�͌�ōċA�Ō���B
//					continue;
//				}
//
//				strcpy(szFilePath, csFilePath);
//				_tsplitpath(szFilePath, szDrive, szDirName, szFileName, szExtension); //�p�X�𕪉�
//				sprintf(szRawDataDirPath, "%s%s%s", szDrive, szDirName, szFileName); //.dat�t�@�C���ɑΉ�����RAW�f�[�^�i�[�t�H���_�̃p�X���쐬
//
//				if(strcmp(szExtension, DAT_EXT)!=0){
//					//�g���q��".dat"�łȂ����̂͌��Ȃ�
//					continue;
//				}
//
//				memset(&ctSearchFileTime, 0, sizeof(ctSearchFileTime));
//				cfFilefind.GetLastWriteTime(ctSearchFileTime);	//�t�@�C���̍X�V�������擾
//
//				//�폜������Â��t�@�C�����ǂ����̃`�F�b�N
//				if(ctSearchFileTime < ctDeleteBaseTime){
//					if(FileOrDirExists(szRawDataDirPath)==TRUE){	//RAW�f�[�^����f�B���N�g������
//						//�w.dat�t�@�C���x����сw���ɑΉ�����RAW�f�[�^����t�H���_�x���Z�b�g�ő��݂���ꍇ�폜����B
//						DeleteFile(szFilePath);
//						/*m_pMainFrame->*/DeleteDirectory(_T(szRawDataDirPath)); //�Ώۃf�B���N�g�����ċA�I�ɍ폜
//					}
//						//Saiki 20091217 Add ---->
//						/*.dat�t�@�C���ɑΉ�����RAW�f�[�^����t�H���_�����݂��Ă��Ȃ��Ă��A�����̉߂���.dat�͏���*/
//						else{
//							DeleteFile(szFilePath);
//						}
//						//Saiki 20091217 Add <----
//				}
//			} while( bFileEnd );
//
//			cfFilefind.Close();
//		}
//	}
	if(bStressData != TRUE){
		try{
			if(cfFilefind.FindFile()){
				do{
					bFileEnd = cfFilefind.FindNextFile();
					csFilePath = cfFilefind.GetFilePath(); //�t�@�C���p�X�擾
					// "." , ".."�𖳎�
					if(cfFilefind.IsDots()){
						continue;
					}

					//�������ʂ̔���
					if(cfFilefind.IsDirectory()){
						//�t�H���_�͌�Ō���B
						//�w�q�t�H���_�����t�H���_�x�͌�ōċA�Ō���B
						continue;
					}

					strcpy(szFilePath, csFilePath);
					_tsplitpath(szFilePath, szDrive, szDirName, szFileName, szExtension); //�p�X�𕪉�
					sprintf(szRawDataDirPath, "%s%s%s", szDrive, szDirName, szFileName); //.dat�t�@�C���ɑΉ�����RAW�f�[�^�i�[�t�H���_�̃p�X���쐬

					if(strcmp(szExtension, DAT_EXT)!=0){
						//�g���q��".dat"�łȂ����̂͌��Ȃ�
						continue;
					}

					memset(&ctSearchFileTime, 0, sizeof(ctSearchFileTime));
					cfFilefind.GetLastWriteTime(ctSearchFileTime);	//�t�@�C���̍X�V�������擾

					//�폜������Â��t�@�C�����ǂ����̃`�F�b�N
					if(ctSearchFileTime < ctDeleteBaseTime){
						if(FileOrDirExists(szRawDataDirPath)==TRUE){	//RAW�f�[�^����f�B���N�g������
							//�w.dat�t�@�C���x����сw���ɑΉ�����RAW�f�[�^����t�H���_�x���Z�b�g�ő��݂���ꍇ�폜����B
							sprintf(szBuff, "%s File Delete(EXIST RAW DATA DIR)", szFilePath);
							g_pFileRetentionPeriod[0]->FileRetentionLogging(szBuff);
							DeleteFile(szFilePath);

							sprintf(szBuff, "%s File Delete", szRawDataDirPath);
							g_pFileRetentionPeriod[0]->FileRetentionLogging(szBuff);
							DeleteDirectory(_T(szRawDataDirPath)); //�Ώۃf�B���N�g�����ċA�I�ɍ폜
						}
						//Saiki 20091217 Add ---->
						/*.dat�t�@�C���ɑΉ�����RAW�f�[�^����t�H���_�����݂��Ă��Ȃ��Ă��A�����̉߂���.dat�͏���*/
						else{
							DeleteFile(szFilePath);
						}
						//Saiki 20091217 Add <----
					}
				} while( bFileEnd );

				cfFilefind.Close();
			}
		}
        catch(CFileException* fe){
            long ErrorCode = fe->m_lOsError;
            sprintf(szBuff, "(cfFilefind) %s, ErrorCode = %ld", "File Invalid Error", ErrorCode);
			g_pFileRetentionPeriod[0]->FileRetentionLogging(szBuff);
        }
	}

	//Saiki 20100517 Change <-----

	// ----------------------------------------------------------------------
	//[2]��L1.�ō폜����Ȃ������t�H���_(�f�[�^���ޗp�t�H���_)�̒��փJ�����g�f�B���N�g�����ڂ��A�Ăт��̊֐����Ă�(�ċA)
	char szCurrentPath[1024+1];
	memset(szCurrentPath, 0, sizeof(szCurrentPath));

	BOOL bFileEnd2;
	CFileFind cfFilefind2;
	if(bStressData != TRUE){
//Saiki 20100517 Change ----->
//		if(cfFilefind2.FindFile()){
//			do{
//				bFileEnd2 = cfFilefind2.FindNextFile();
//				csFilePath = cfFilefind2.GetFilePath(); 							// �t�@�C���p�X�擾
//				// "." , ".."�𖳎�
//				if(cfFilefind2.IsDots()){
//					continue;
//				}
//
//				//�������ʂ̔���
//				if(cfFilefind2.IsDirectory()){
//					GetCurrentDirectory(sizeof(szCurrentPath), szCurrentPath);		// �J�����g�p�X�擾(������Ɍ��̃J�����g�f�B���N�g���ɖ߂�ׂ̂���)
//					SetCurrentDirectory(csFilePath);								// �J�����g�f�B���N�g���������Ώۃt�H���_�Ɉړ�
//
//					DeleteOldFile(stDeleteBaseTime);								// �ċA
//
//					SetCurrentDirectory(szCurrentPath); 							// �J�����g�f�B���N�g�������ɖ߂�
//				}
//
//			} while( bFileEnd2 );
//
//			cfFilefind2.Close();
//		}
		try{
			if(cfFilefind2.FindFile()){
				do{
					bFileEnd2 = cfFilefind2.FindNextFile();
					csFilePath = cfFilefind2.GetFilePath(); 							// �t�@�C���p�X�擾
					// "." , ".."�𖳎�
					if(cfFilefind2.IsDots()){
						continue;
					}
					//�������ʂ̔���
					if(cfFilefind2.IsDirectory()){
						GetCurrentDirectory(sizeof(szCurrentPath), szCurrentPath);		// �J�����g�p�X�擾(������Ɍ��̃J�����g�f�B���N�g���ɖ߂�ׂ̂���)
						SetCurrentDirectory(csFilePath);								// �J�����g�f�B���N�g���������Ώۃt�H���_�Ɉړ�
						DeleteOldFile(stDeleteBaseTime);								// �ċA
						SetCurrentDirectory(szCurrentPath); 							// �J�����g�f�B���N�g�������ɖ߂�
					}
				} while( bFileEnd2 );
				cfFilefind2.Close();
			}
		}
        catch(CFileException* fe){
            long ErrorCode = fe->m_lOsError;
            sprintf(szBuff, "(cfFilefind2) %s, ErrorCode = %ld", "File Invalid Error", ErrorCode);
			g_pFileRetentionPeriod[0]->FileRetentionLogging(szBuff);
        }
//Saiki 20100517 Change <-----
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::WatchDoc()
{
	static BOOL bNexMovo2AlarmOld = nexioIsMovo2Alarm();
	BOOL bNexMovo2Alarm = nexioIsMovo2Alarm();

//	if(((bNexMovo2Alarm | bNexMovo2AlarmOld) ^ bNexMovo2Alarm) == TRUE) //����������o
	if(((bNexMovo2Alarm | bNexMovo2AlarmOld) ^ bNexMovo2AlarmOld) == TRUE){ //�����オ������o
		// ���u�d���I�t�F�o��
		AlarmIf_Set(ALID_StageError); //�X�e�[�W�G���[
		nexioEquipmentPowerOFF(TRUE);
	}

	if(nexioIsEquipmentPower() == OFF){
		// ���u�d���I�t�o�͂��I�t
		nexioEquipmentPowerOFF(FALSE);
	}

	bNexMovo2AlarmOld = bNexMovo2Alarm;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateDebug(CCmdUI* pCmdUI)
{
	///// User Access Privilege /////
	pCmdUI->Enable(m_pDoc->User_Access(ACCESS_SYSTEMCONFIG));
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnMessageShow(WPARAM wparam, LPARAM lparam)
{
	int m_MsgID = wparam;

	// ���b�Z�[�WDialog��ʂ�\������
	g_pMessageDlg->SetMessage(m_MsgID);
	g_pMessageDlg->ShowWindow(SW_SHOWNORMAL);
	g_pMessageDlg->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}

//Saiki 20090411 Add ----->
/////////////////////////////////////////////////////////////////////////////
//
BOOL CALLBACK CMainFrame::QuitExternalApp(HWND hWnd, LPARAM lParam)
{
	// CreateProcess()�Ŏ擾����PROCESS_INFORMATION�\���̂̃|�C���^���擾
	PROCESS_INFORMATION* pi = (PROCESS_INFORMATION*)lParam;

	// �E�C���h�E���쐬�����v���Z�XID���擾�B
	DWORD lpdwProcessId = 0;
	::GetWindowThreadProcessId(hWnd, &lpdwProcessId);

	// CreateProcess�ŋN�������A�v���̃v���Z�XID�ƃ��C���E�B���h�E��
	// �쐬�����v���Z�XID�������ꍇ�A�N�������A�v�����I��������B
	if(pi->dwProcessId == lpdwProcessId)
	{
		::PostMessage(hWnd, WM_CLOSE, 0, 0);
		return FALSE;
	}
	return TRUE;

}
//Saiki 20090411 Add <-----

// =========================================================================
// Name�F DoEvents
void CMainFrame::DoEvents(void)
{
	MSG 	msg;
	BOOL	bRet;

	// dispatch window messages for our client
	while ( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
		bRet = ::GetMessage(&msg, NULL, 0, 0);
		if( bRet != -1 ){
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnDebMessageRecieve(WPARAM wparam, LPARAM lparam)
{
	CTime time = CTime::GetCurrentTime();
	CHAR szDate[80];
	CHAR szTime[80];
	CString szBuf;

	///// Date /////
	strcpy(szDate, time.Format("%Y/%m/%d"));

	///// Time /////
	strcpy(szTime, time.Format("%H:%M:%S"));

	m_DbgCount = wparam;
	TRACE("CMainFrame::OnDebMessageRecieve() RECV=%s %s COUNT=%d\n", szDate, szTime, m_DbgCount);

	return TRUE;
}

//Saiki 20090530 Add ----->
/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::MessageShow(CString strBuffer)
{
	// ���b�Z�[�WDialog��ʂ�\������
	g_pMessageDlg->SetMessageString(strBuffer);
	g_pMessageDlg->ShowWindow(SW_SHOWNORMAL);
	g_pMessageDlg->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
//Saiki 20090530 Add <-----

/* added 2009.06.02 hmenjo �h�A�J�ő���|�[�Y ---------- { ---------- */
/////////////////////////////////////////////////////////////////////////////
// Chief Event
// ����|�[�Y��ʒm(WM_CHIF_NOTIFY_MEAS_PAUSE)
LRESULT CMainFrame::OnNotifyMeasPause(WPARAM wparam, LPARAM lparam)
{
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- { ---------- */
	TCHAR l_tszLogMsg[256];
	_stprintf(l_tszLogMsg, _T("Received WM_CHIF_NOTIFY_MEAS_PAUSE (WPARAM = 0x%08x, LPARAM = 0x%08x)"), wparam, lparam);
	m_pDoc->OperationLogging(l_tszLogMsg);
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- } ---------- */
	int l_iReason = (int) wparam;		/* �|�[�Y���R(�|�[�Y�������͖����ł�)	*/
	BOOL l_bRelease = (BOOL) lparam;	/* TRUE�F�|�[�Y���� */

	// �|�[�Y�ʒm���b�Z�[�W�{�b�N�X�̑��݂��m�F���܂��D
	CString l_strPauseMsgTitle;
	LoadStringML(IDS_PAUSEMSGBOX_TITLE, l_strPauseMsgTitle, _T("NanoSpec Measurement Pause"));
	HWND l_hPauseMsgBoxWnd = ::FindWindow(0, l_strPauseMsgTitle);

	if (0 == l_bRelease) {
		// �|�[�Y�ʒm
		if (0 == l_hPauseMsgBoxWnd) {
			// ���b�Z�[�W�{�b�N�X��\�����Ă��Ȃ��ꍇ�̂ݏ������܂��D
/* added 2009.06.08 hmenjo �|�[�Y�O�� Resume �s��΍� ---------- { ---------- */
			CString l_strPauseMsgBase;
			LoadStringML(IDS_PAUSEMSGBOX_BASE, l_strPauseMsgBase, _T("Measurement was paused, because %s.\n%s"));
/* added 2009.06.08 hmenjo �|�[�Y�O�� Resume �s��΍� ---------- } ---------- */
			CString l_strPauseMsgReason;
			switch (l_iReason) {
			case 1:
				LoadStringML(IDS_PAUSEMSGBOX_01, l_strPauseMsgReason, _T("EQ doors were opened"));
/* modified 2009.06.05 hmenjo �h�A�|�[�Y�A���[����ALID 2 ---------- { ---------- */
//				AlarmIf_Set(1041); break;
/* modified 2009.06.05 hmenjo �h�A�|�[�Y�A���[����ALID 2 ---------- 			 */
				AlarmIf_Set(ALID_DoorOpen); break;
/* modified 2009.06.05 hmenjo �h�A�|�[�Y�A���[����ALID 2 ---------- } ---------- */
/* added 2009.06.08 hmenjo �|�[�Y�O�� Resume �s��΍� ---------- { ---------- */
			case -2:
				l_strPauseMsgBase = _T("%s.\n%s");
				LoadStringML(IDS_PAUSEMSGBOX_M02, l_strPauseMsgReason, _T("Cycle stopping by paused measurement NOW!!!")); break;
/* added 2009.06.08 hmenjo �|�[�Y�O�� Resume �s��΍� ---------- } ---------- */
			default:
				LoadStringML(IDS_PAUSEMSGBOX_00, l_strPauseMsgReason, _T("unknown error was detected"));
//				AlarmIf_Set(1040); break;
				AlarmIf_Set(ALID_MeasPauseError); break;
			// Pif �ւ̒ʒm�������͕K�v�����H�H�H
			}
/* deleted 2009.06.08 hmenjo �|�[�Y�O�� Resume �s��΍� ---------- { ---------- */
//			CString l_strPauseMsgBase;
//			LoadStringML(IDS_PAUSEMSGBOX_BASE, l_strPauseMsgBase, _T("Measurement was paused, because %s.\n%s"));
/* deleted 2009.06.08 hmenjo �|�[�Y�O�� Resume �s��΍� ---------- } ---------- */
			CString l_strPauseMsgQMsg;
			LoadStringML(IDS_PAUSEMSGBOX_QMSG, l_strPauseMsgQMsg, _T("Resume measurement ??"));
			CString l_strPauseMsg;
			l_strPauseMsg.Format(l_strPauseMsgBase, l_strPauseMsgReason, l_strPauseMsgQMsg);
			int l_iRet = MessageBox(l_strPauseMsg, l_strPauseMsgTitle, MB_OKCANCEL | MB_ICONWARNING);
			if (0 != m_bClosedPauseMsgBox) {
				m_bClosedPauseMsgBox = FALSE;
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- { ---------- */
				m_pDoc->OperationLogging(_T("Pause MsgBox : Auto closed."));
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- } ---------- */
			} else {
				switch (l_iRet) {
				case IDOK:	// Resume(�ĊJ)���܂��D
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- { ---------- */
					m_pDoc->OperationLogging(_T("Pause MsgBox : Clicked 'OK' button."));
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- } ---------- */
					{
						LRESULT l_LResult = OnDoResume(0, 0);
						switch (l_LResult) {
						case 0: 	/* ����� Resume(�ĊJ)�o���܂����D	*/
							break;
						case 32:	/* �h�A�C���^���b�N */
							PostMessage(WM_CHIF_NOTIFY_MEAS_PAUSE, (WPARAM) 1, 0); break;
/* added 2009.06.08 hmenjo �|�[�Y�O�� Resume �s��΍� ---------- { ---------- */
						case -2:	/* �܂��|�[�Y�O(�T�C�N����~�O)�ł� */
							PostMessage(WM_CHIF_NOTIFY_MEAS_PAUSE, (WPARAM) -2, 0); break;
/* added 2009.06.08 hmenjo �|�[�Y�O�� Resume �s��΍� ---------- } ---------- */
						case 34:	/* ���{�b�g�A�[�����o	*/
						case 35:	/* �s���_�E�����Ă��Ȃ� */
						case 31:	/* EMO	*/
						case 33:	/* ���u�p���[�I�t	*/
//						case 2: 	/* ���[�J���łȂ�����	*/���g�p�ɂȂ�܂���(20090603)
						case 3: 	/* Processing �łȂ�	*/
						case 4: 	/* ��ʃ��[�h���u���胂�[�h�v���u�f�[�^���[�h�v�łȂ�����	*/
						case -1:	/* Chief ��������Ȃ� */
						default:	/* ����`�G���[ */
							OnDoCancel(0, 0);	// �L�����Z�����܂��D
							break;
						}
					}
					break;
				default:	// �L�����Z�����܂��D
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- { ---------- */
					m_pDoc->OperationLogging(_T("Pause MsgBox : Clicked 'Cancel' button."));
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- } ---------- */
					OnDoCancel(0, 0);
					break;
				}
			}
		}
	} else {
		// �|�[�Y�����ʒm
		if (0 != l_hPauseMsgBoxWnd) {
			if (0 == m_bClosedPauseMsgBox) {
				// ���b�Z�[�W�{�b�N�X��\�����Ă���ꍇ�̂ݏ������܂��D
				// ���ׂẴ��b�Z�[�W�{�b�N�X����܂��D
				m_bClosedPauseMsgBox = TRUE;
				BOOL l_bRet = ::PostMessage(l_hPauseMsgBoxWnd, WM_CLOSE, 0, 0);
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- { ---------- */
				m_pDoc->OperationLogging(_T("Pause MsgBox : Sent WM_CLOSE to Pause MsgBox."));
/* added 2009.06.09 hmenjo �ُ펞����|�[�Y ���O�ǉ� ---------- } ---------- */
				PostMessage(WM_CHIF_NOTIFY_MEAS_PAUSE, 0, (LPARAM) TRUE);
			}
		}
	}

	return 0L;
}
/* added 2009.06.02 hmenjo �h�A�J�ő���|�[�Y ---------- } ---------- */
//2009.07.27 bagus stress �X�g���X�Ή� --{--
/////////////////////////////////////////////////////////////////////////////
// Chief Event
// Stress 1���C�����芮��
afx_msg LRESULT CMainFrame::OnLineEnd(WPARAM wparam,LPARAM lparam)
{
	DWORD dwRefMeas;
	DWORD dwLineNo;
	HWND l_hChiefDlg;
	CSharedMemory<STRESSRESULT>* pStressLineData;
	CString strBuf;

	//0: ���t�@�����X����, 0�ȊO:������
	dwRefMeas = (DWORD)lparam;
	//���C��No
	dwLineNo = (DWORD)wparam;

	//���ۑ���Ԃ�
	m_pMeaTabViewObj->m_bExitSaveCompFlg = FALSE;
	m_pMeaTabViewObj->m_bToolbarSaveCompFlg = FALSE;

	pStressLineData = new CSharedMemory<STRESSRESULT>;
	if(!pStressLineData->Open(_T(STRESSFILEMAPPINGNAME))){
		LoadStringML(IDS_STRESS_RESULT_GET_ERROR, strBuf, "Can't get stress result.");
		AfxMessageBox(strBuf);
	}

	m_pDoc->SetOneStressLineData(pStressLineData->GetSharedMemoryPtr(),dwLineNo);
	m_pDoc->SetOneStressLineDataValid(dwLineNo,TRUE);
	m_pDoc->SetOneStressLineDataRef(dwLineNo,!dwRefMeas);

// 2009.09.17 Stress ��ʕ� -->
	//��ʂւ̕�
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	PifComm_CompleteMeasureStressLineReport(dwRefMeas, dwLineNo, &rcp.StressStageProgInfo, pStressLineData->GetSharedMemoryPtr()->dStressSection);
// 2009.09.17 Stress ��ʕ� <--

	//�ꎞ�f�B���N�g�����쐬����
	CreateDirectory(g_szData_Meas_Dat_Temp_Copy_Dir, NULL);
	//����f�[�^���ꎞ�t�@�C���ɕۑ�����

	for(int nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
		OutputStressLSRawData(NULL,rcp.szRecipeName,dwLineNo,nSection+1,!dwRefMeas);
	}
	//2009.08.31 bagus stress --{--
	//���v���̌v�Z(���C�����j
	STRESSRESULT *pResult = pStressLineData->GetSharedMemoryPtr();
	STATISTICS tStat;
	CalcStressLineStatitcs(pResult,&tStat,rcp.StressStageProgInfo.Line[dwLineNo-1].bScanValid);
	m_pDoc->SetStressLineStatisticsData(dwLineNo,&tStat);
	//���v���̌v�Z(�S�̕�)
	CalcStressStatistics(&tStat,&rcp);
	m_pDoc->SetStressStatisticsData(&tStat);
	//2009.08.31 bagus stress --{--

	///// �`�[�t�։������b�Z�[�W�𑗐M /////
	l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementStartFailed);
		return 0L;
	}
	::PostMessage(l_hChiefDlg, WM_DATA_LINE_END, 0, 0);

	if ( pStressLineData ) {
		delete pStressLineData;
		pStressLineData = NULL;
	}
	m_pDoc->SetShowCompStressList(dwLineNo-1);
	m_pDoc->SetShowCompStressGraph(dwLineNo-1);

	return 0L;
}
/////////////////////////////////////////////////////////////////////////////
// Chief Event
// Stress 1���C������J�n
afx_msg LRESULT CMainFrame::OnLineStart(WPARAM wparam,LPARAM lparam)
{
	DWORD dwRefMeas;
	DWORD dwLineNo;

	//0: ���t�@�����X����, 0�ȊO:������
	dwRefMeas = (DWORD)lparam;
	//���C��No
	dwLineNo = (DWORD)wparam;

	//m_pDoc->SetShowCompStressList(dwLineNo-1);

	m_pDoc->SetCurrentMeasureStressLine(dwLineNo);

	//���ۑ���Ԃ�
	m_pMeaTabViewObj->m_bExitSaveCompFlg = FALSE;
	return 0L;
}
/////////////////////////////////////////////////////////////////////////////
// Chief Event
// Stress 1������I��
afx_msg LRESULT CMainFrame::OnStrsMeasEnd(WPARAM wparam,LPARAM lparam)
{
	DWORD dwRefMeas;
	DWORD dwNormalEnd;
	HWND l_hChiefDlg;
	int nLine;

	//0: ���t�@�����X����, 0�ȊO:������
	dwRefMeas = (DWORD)lparam;
	//���C��No
	dwNormalEnd = (DWORD)wparam;
	//2009.09.17 bagus stress --{--
	SYSTEMTIME syEndDateTime;	  //����I������
	GetLocalTime(&syEndDateTime); //����I�������X�V
	m_pDoc->SetEndDateTime(&syEndDateTime);
	//2009.09.17 bagus stress --}--

	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);

// 2009.09.17 Stress ��ʕ� -->
	BOOL bStressLineReport = FALSE;
	for ( int i = 0; i < STRESS_LINES_MAX; i++ ) {
		bStressLineReport = m_pDoc->GetOneStressLineDataValid(i + 1);
		if ( bStressLineReport )
			break;
	}

	//��ʂւ̕�
	if ( bStressLineReport ) {
// 2009.10.30 K.Matsuo Bug Fix -->
		STATISTICS tStat;
		STATISTICSDATESET pifIf[STRESS_LINES_MAX + 1];

		for ( nLine = 0; nLine < STRESS_LINES_MAX; nLine++ ) {
			::ZeroMemory(&tStat, sizeof(tStat));
			if ( m_pDoc->GetOneStressLineDataValid(nLine + 1) ) {
				m_pDoc->GetStressLineStatisticsData(nLine + 1, &tStat);
				pifIf[nLine].dMin = tStat.dMinimum;
				pifIf[nLine].dMax = tStat.dMaximum;
				pifIf[nLine].dAve = tStat.dAverage;
				pifIf[nLine].dSd  = tStat.dStandardDeviation;
				pifIf[nLine].dUnif = 0.0;			// �[���ł悢�BUniformity�́A�e�Ђł��낢��Ȍv�Z���W�b�N������炵���BProcon���Ōv�Z����B
			}
		}

		// �S�̓��v���
		::ZeroMemory(&tStat, sizeof(tStat));
		m_pDoc->GetStressStatisticsData(&tStat);
		pifIf[nLine].dMin = tStat.dMinimum;
		pifIf[nLine].dMax = tStat.dMaximum;
		pifIf[nLine].dAve = tStat.dAverage;
		pifIf[nLine].dSd  = tStat.dStandardDeviation;
		pifIf[nLine].dUnif = 0.0;			// �[���ł悢�BUniformity�́A�e�Ђł��낢��Ȍv�Z���W�b�N������炵���BProcon���Ōv�Z����B

		PifComm_CompleteMeasureStressAllLinesReport(dwRefMeas, &rcp.StressStageProgInfo, pifIf);
// 2009.10.30 K.Matsuo Bug Fix <--
	}
// 2009.09.17 Stress ��ʕ� <--

	//���ۑ���Ԃ�
	m_pMeaTabViewObj->m_bToolbarSaveCompFlg = FALSE;
	m_pMeaTabViewObj->m_bExitSaveCompFlg = FALSE;

	///// �`�[�t�։������b�Z�[�W�𑗐M /////
	l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementStartFailed);
		return 0L;
	}
	::PostMessage(l_hChiefDlg, WM_DATA_STRS_MEAS_END, 0, 0);

/* deleted 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
//hdebdeb	m_pDoc->SetProcessStatus(PROCESS_WAIT);
/* deleted 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
	//�ꎞ�f�B���N�g�����쐬����
	CreateDirectory(g_szData_Meas_Dat_Temp_Copy_Dir, NULL);
	//����t�@�C���̏o��
	OutputStressRawData(NULL,rcp.szRecipeName,!dwRefMeas);
	OutputStressData(NULL,rcp.szRecipeName,!dwRefMeas);

	if(m_pDoc->GetHostMode() == HOST_REMOTE){
		MeasEnd();
	}

// 2009.09.17 Stress ��ʕ� -->
	// �����[�g���́A�����ŉ�ʑJ�ڂ���
	if(m_pDoc->GetHostMode() == HOST_REMOTE){
		ChangeOperationPanel(OP_MEASUREMENT);
		ChangeExeWnd(RECIPE_MAIN_LIST_WND);
	}
// 2009.09.17 Stress ��ʕ� -->

	return 0L;
}
//2009.07.27 bagus stress �X�g���X�Ή� --}--
//2009.08.27 bagus stress --{--
BOOL CMainFrame::OutputStressData(LPCTSTR lpszOutputPath,LPCTSTR lpszRecipeName,BOOL bRef)
{
	/*
	//�f�[�^������
	m_pMeaTabViewObj->SaveMeasConRevi(lpszRecipeName);

	//�f�[�^�ǋL
	m_pMeaTabViewObj->SaveMeasConAdd(lpszRecipeName); //����f�[�^�ȂǒǋL

	char szTo[MAX_PATH+1];
	char szFrom[MAX_PATH+1];
	if(bRef){
		sprintf(szTo,"%s\\%s%s",g_szData_Meas_Dat_Temp_Copy_Dir,STRESS_REFERENCE_NAME,DAT_EXT);
		sprintf(szFrom,"%s\\%s%s",g_szData_Meas_Dat_Temp_Copy_Dir,lpszRecipeName,DAT_EXT);
		MoveFileEx(szFrom,szTo,MOVEFILE_REPLACE_EXISTING);
	}else{
	}
	*/

	return TRUE;
}

BOOL CMainFrame::OutputStressRawData(LPCTSTR lpszOutputPath,LPCTSTR lpszRecipeName,BOOL bRef)
{
	RCP_DATA rcp;
	CString strBuffer;
	m_pDoc->GetRcpData(&rcp);
	STAGE_PROG_STRESS& stressStage = rcp.StressStageProgInfo;
	STRESS_CONFIG l_StressConfig;

	CString strOutputPath;
	if(bRef){
		strOutputPath.Format("%s\\%s%s",g_szData_Meas_Dat_Temp_Copy_Dir,"StressRefRawData",DAT_EXT);
	}else{
		strOutputPath.Format("%s\\%s%s",g_szData_Meas_Dat_Temp_Copy_Dir,"StressRawData",DAT_EXT);
	}

	ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);

	DeleteFile(strOutputPath);
	//[MainRecipe]
	RECIPE_FILE MainRecipeDesc[] =
	{
		//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
		{"MainRecipe",		"Name", 		'C',		"0",		rcp.szRecipeName		},
	};

	if(bRef){
		m_pMeaTabViewObj->SaveMeasConRevi2(&MainRecipeDesc[0],sizeof(MainRecipeDesc)/sizeof(MainRecipeDesc[0]),_T("StressRefRawData"));
	}else{
		m_pMeaTabViewObj->SaveMeasConRevi2(&MainRecipeDesc[0],sizeof(MainRecipeDesc)/sizeof(MainRecipeDesc[0]),_T("StressRawData"));
	}

	//
	int n;
	for(n = 0;n < STRESS_LINES_MAX;n++){
		char szKey[1024+1];
		sprintf(szKey,"Line%dValidScan",n+1);
		//[ValidLine]
		//2009.09.17 bagus stress --{--
		BOOL bValid = m_pDoc->GetOneStressLineDataValid(n+1);
		//2009.09.24 bagus stress --{--
		//if(!bValid) continue;
		if(!l_StressConfig.Line[n].bValidLine) continue;
		//2009.09.24 bagus stress --}--
		//2009.09.17 bagus stress --}--
		RECIPE_FILE ValidLineDesc[] =
		{
			//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
			//{"ValidScanLine", 	szKey,		'B',		FALSE,		&stressStage.Line[n].bValidLine 	   },
			{"ValidScanLine",		szKey,		'b',		FALSE,		&bValid 								},
		};
		if(bRef){
			m_pMeaTabViewObj->SaveMeasConRevi2(&ValidLineDesc[0],sizeof(ValidLineDesc)/sizeof(ValidLineDesc[0]),_T("StressRefRawData"));
		}else{
			m_pMeaTabViewObj->SaveMeasConRevi2(&ValidLineDesc[0],sizeof(ValidLineDesc)/sizeof(ValidLineDesc[0]),_T("StressRawData"));
		}

	}

	CStdioFile outFile;
	if(!outFile.Open(strOutputPath,CFile::modeWrite | CFile::typeText)){
		return FALSE;
	}
	outFile.Seek(0,CFile::end);

	for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
		//2009.09.17 bagus stress --{--
		STRESSRESULT tRes;
		BOOL bValid = m_pDoc->GetOneStressLineDataValid(nLine+1);
		if(!bValid) continue;
		//2009.09.17 bagus stress --}--

		strBuffer.Format("[Line%d]\n",nLine+1);
		outFile.WriteString(strBuffer);
		strBuffer.Format("ScanPosition[%s]\tHeight[%s]\n","um","um");
		outFile.WriteString(strBuffer);
		//2009.09.17 bagus stress --{--
		//STRESSRESULT tRes;
		//BOOL bValid = m_pDoc->GetOneStressLineDataValid(nLine+1);
		//if(!bValid)	continue;
		//2009.09.17 bagus stress --}--

		if(m_pDoc->GetOneStressLineData(&tRes,nLine+1))
		{
// 2009.09.09 bagus stres -->
//			for(n = 0;n < tRes.nStressLineOccurrence;n++){
// 2009.08.28 K.Matsuo -->
//				strBuffer.Format("%.3f\t%.3F\n",tRes.StressRaw[n].dPosum,tRes.StressRaw[n].dStress);
//				strBuffer.Format("%.3lf\t%.3lf\n",tRes.StressRaw[n].dPosum,tRes.StressRaw[n].dStress);
//
//// 2009.08.28 K.Matsuo <--
//				outFile.WriteString(strBuffer);
//			}
			for(n = 0;n < tRes.nDeflectionOccurence[!bRef];n++){
				strBuffer.Format("%.3lf\t%.3lf\n",tRes.DeflectionRaw[!bRef][n].dPosum,tRes.DeflectionRaw[!bRef][n].dDefl);
				outFile.WriteString(strBuffer);
			}
// 2009.09.09 bagus stres <--
		}

		//
		strBuffer.Format("\n");
		outFile.WriteString(strBuffer);
	}

	outFile.Close();

	return TRUE;

}
BOOL CMainFrame::OutputStressLSRawData(LPCTSTR lpszOutputPath,LPCTSTR lpszRecipeName,int nLine,int nSection,BOOL bRef)
{
	RCP_DATA rcp;
	CString strBuffer;
	CString strOutputPath;
	m_pDoc->GetRcpData(&rcp);
	STAGE_PROG_STRESS& stressStage = rcp.StressStageProgInfo;
	STRESSRESULT tRes;
	STRESS_CONFIG l_StressConfig;
	int n;

	if(nLine < 1 || nLine > STRESS_LINES_MAX){
		return FALSE;
	}
	if(nSection < 1 || nSection > STRESS_SECTIONS_MAX){
		return FALSE;
	}

	//���V�s�Ƃ��ėL���ȃ��C���łȂ���Ε\�����Ȃ�
	if(!stressStage.Line[nLine-1].bValidLine){
		return FALSE;
	}
	if(!stressStage.Line[nLine-1].bScanValid[nSection-1]){
		return FALSE;
	}

	if(!m_pDoc->GetOneStressLineData(&tRes,nLine)){
		return FALSE;
	}

	ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);

	strBuffer.Format("StressL%dS%dRawData",nLine,nSection);
	strOutputPath.Format("%s\\%s%s",g_szData_Meas_Dat_Temp_Copy_Dir,strBuffer,DAT_EXT);
	DeleteFile(strOutputPath);


	char szKeyValid[1024+1];
	char szKeyStartX[1024+1];
	char szKeyEndX[1024+1];
	char szKeyY[1024+1];
	//[INFO]
	sprintf(szKeyValid,"L%dS%d_SanValid",nLine,nSection);
	sprintf(szKeyStartX,"L%dS%d_ScanStart_X",nLine,nSection);
	sprintf(szKeyEndX,"L%dS%d_ScanEnd_X",nLine,nSection);
	sprintf(szKeyY,"L%dS%d_Y",nLine,nSection);
	RECIPE_FILE InfoDesc[] =
	{
		//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
		{"INFO",		szKeyValid, 		'b',		"FALSE",	&stressStage.Line[nLine-1].bScanValid[nSection-1]				},
		{"",			szKeyStartX,		'L',		"0",		&stressStage.Line[nLine-1].SectPos[nSection-1].lScanStartPosX	},
		{"",			szKeyEndX,			'L',		"0",		&stressStage.Line[nLine-1].SectPos[nSection-1].lScanEndPosX 	},
		{"",			szKeyY, 			'L',		"0",		&stressStage.Line[nLine-1].SectPos[nSection-1].lScanPosY		},
	};
	m_pMeaTabViewObj->SaveMeasConRevi2(&InfoDesc[0],sizeof(InfoDesc)/sizeof(InfoDesc[0]),strBuffer);

	CStdioFile outFile;

	if(!outFile.Open(strOutputPath,CFile::modeWrite | CFile::typeText)){
		return FALSE;
	}
	outFile.Seek(0,CFile::end);
	outFile.WriteString("\n");

	//Reference
	strBuffer.Format("[L%dS%d_REFERENCE]\n",nLine,nSection);
	outFile.WriteString(strBuffer);
	strBuffer.Format("ScanPosition[%s]\tHeight[%s]\n","um","um");
	outFile.WriteString(strBuffer);
	for(n = 0;n < tRes.nDeflectionOccurence[0];n++){
		strBuffer.Format("%.3lf\t%.3lf\n",tRes.DeflectionRaw[0][n].dPosum,tRes.DeflectionRaw[0][n].dDefl);
		outFile.WriteString(strBuffer);
	}
	outFile.WriteString("\n");
	//Sample
	strBuffer.Format("[L%dS%d_SAMPLE]\n",nLine,nSection);
	outFile.WriteString(strBuffer);
	strBuffer.Format("ScanPosition[%s]\tHeight[%s]\n","um","um");
	outFile.WriteString(strBuffer);
	for(n = 0;n < tRes.nDeflectionOccurence[1];n++){
		strBuffer.Format("%.3lf\t%.3lf\n",tRes.DeflectionRaw[1][n].dPosum,tRes.DeflectionRaw[1][n].dDefl);
		outFile.WriteString(strBuffer);
	}
	outFile.WriteString("\n");
	//Theta
	strBuffer.Format("[L%dS%d_Theta]\n",nLine,nSection);
	outFile.WriteString(strBuffer);
// 2009.10.23 bagus Stress �C�� --{--
//	strBuffer.Format("ScanPosition[%s]\tTheta[%s]\n","um","deg");
	strBuffer.Format("ScanPosition[%s]\tTheta[%s]\n","um","mRad");
// 2009.10.23 bagus Stress �C�� --}--
	outFile.WriteString(strBuffer);
	for(n = 0;n < tRes.nDyDxOccurence;n++){
		strBuffer.Format("%.3lf\t%.3lf\n",tRes.DyDxRaw[n].dPosum,tRes.DyDxRaw[n].dTheta);
		outFile.WriteString(strBuffer);
	}
	outFile.WriteString("\n");
// 2009.10.23 bagus Stress �C�� --{--
#if 0
	//Stress
	strBuffer.Format("[L%dS%d_Stress]\n",nLine,nSection);
	outFile.WriteString(strBuffer);
	strBuffer.Format("ScanPosition[%s]\tStress[%s]\n","um","MPa");
	outFile.WriteString(strBuffer);
	//2009.09.17 bagus stress --{--
	//for(n = 0;n < tRes.nSectionOccurrence;n++){
	for(n = 0;n < tRes.nStressLineOccurrence;n++){
	//2009.09.17 bagus stress --}--
		strBuffer.Format("%.3lf\t%.3lf\n",tRes.StressRaw[n].dPosum,tRes.StressRaw[n].dStress);
		outFile.WriteString(strBuffer);
	}
	outFile.WriteString("\n");
#endif
// 2009.10.23 bagus Stress �C�� --}--

	outFile.Close();

	/*
	char szTo[MAX_PATH+1];
	if(bRef){
		sprintf(szTo,"%s\\%s",g_szData_Meas_Dat_Temp_Copy_Dir,STRESS_REFERENCE_NAME);
	CreateDirectory(szTo,NULL);
	strBuffer.Format("StressL%dS%dRawData",nLine+1,nSection+1);
	sprintf(szTo,"%s\\%s\\%s%s",g_szData_Meas_Dat_Temp_Copy_Dir,STRESS_REFERENCE_NAME,(LPCSTR)strBuffer,DAT_EXT);
		MoveFileEx(strOutputPath,szTo,MOVEFILE_REPLACE_EXISTING);
	}else{
	sprintf(szTo,"%s\\%s",g_szData_Meas_Dat_Temp_Copy_Dir,lpszRecipeName);
	CreateDirectory(szTo,NULL);
	sprintf(szTo,"%s\\%s\\%s%s",g_szData_Meas_Dat_Temp_Copy_Dir,lpszRecipeName,"StressRawData",(LPCSTR)strBuffer,DAT_EXT);
		MoveFileEx(strOutputPath,szTo,MOVEFILE_REPLACE_EXISTING);
	}
	*/

	return TRUE;

}
//2009.08.27 bagus stress --}--
//2009.08.31 bagus stress --{--
void CMainFrame::CalcStressLineStatitcs(STRESSRESULT *pResult,STATISTICS *pStat,BOOL *pScanValid)
{
#if 0
struct STATISTICS
{
	BOOL Valid;
	double dMinimum;
	double dMaximum;
	double dAverage;
	double dStandardDeviation;
	double dRange;
}
#endif
	double dMin,dMax,dAve,dRange;
	double dSum = 0.0;
	int nValidNum = 0;
	int nSect;
	dMin = DBL_MAX;
	dMax = DBL_MIN;
	dAve = dRange = 0.0;

	for(nSect = 0;nSect < STRESS_SECTIONS_MAX;nSect++){
		if(pScanValid[nSect]){
			nValidNum++;
			dSum += pResult->dStressSection[nSect];
			if(pResult->dStressSection[nSect] < dMin){
				dMin = pResult->dStressSection[nSect];
			}
			if(pResult->dStressSection[nSect] > dMax){
				dMax = pResult->dStressSection[nSect];
			}


		}
	}
	if(nValidNum){
		pStat->Valid = TRUE;
		pStat->dMaximum = dMax;
		pStat->dMinimum = dMin;
		pStat->dAverage = dSum / (double)nValidNum;
		pStat->dRange = dMax - dMin;
		double dSigma = 0.0;
		if(nValidNum >= 2){
			for(nSect = 0;nSect < STRESS_SECTIONS_MAX;nSect++){
				if(pScanValid[nSect]){
					dSigma += pow(pResult->dStressSection[nSect] - pStat->dAverage,2.0);
				}
			}
			pStat->dStandardDeviation = sqrt(dSigma / (double)(nValidNum -1));
		}else{
			pStat->dStandardDeviation = 0.0;
		}
	}else{
		pStat->dMaximum = 0.0;
		pStat->dMinimum = 0.0;
		pStat->dAverage = 0.0;
		pStat->dRange = 0.0;
		pStat->dStandardDeviation = 0.0;
		pStat->Valid = FALSE;
	}

}
void CMainFrame::CalcStressStatistics(STATISTICS *pStat,RCP_DATA *pRcp)
{
	double dMin,dMax,dAve,dRange;
	double dSum = 0.0;
	int nValidNum = 0;
	dMin = DBL_MAX;
	dMax = DBL_MIN;
	dAve = dRange = 0.0;
	STRESSRESULT tResult;
	int nSect;
	int nLine;

	for(nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
		if(m_pDoc->GetOneStressLineDataValid(nLine+1)){
			m_pDoc->GetOneStressLineData(&tResult,nLine+1);
			for(nSect = 0;nSect < STRESS_SECTIONS_MAX;nSect++){
				if(pRcp->StressStageProgInfo.Line[nLine].bScanValid[nSect]){
					nValidNum++;
					dSum += tResult.dStressSection[nSect];
					if(tResult.dStressSection[nSect] < dMin){
						dMin = tResult.dStressSection[nSect];
					}
					if(tResult.dStressSection[nSect] > dMax){
						dMax = tResult.dStressSection[nSect];
					}
				}
			}
		}
	}

	if(nValidNum){
		pStat->Valid = TRUE;
		pStat->dMaximum = dMax;
		pStat->dMinimum = dMin;
		pStat->dAverage = dSum / (double)nValidNum;
		pStat->dRange = dMax - dMin;
		double dSigma = 0.0;
		if(nValidNum >= 2){
			for(nSect = 0;nSect < STRESS_SECTIONS_MAX;nSect++){
				if(pRcp->StressStageProgInfo.Line[nLine].bScanValid[nSect]){
					dSigma += pow(tResult.dStressSection[nSect] - pStat->dAverage,2.0);
				}
			}
			for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
				if(m_pDoc->GetOneStressLineDataValid(nLine+1)){
					m_pDoc->GetOneStressLineData(&tResult,nLine+1);
					for(int nSect = 0;nSect < STRESS_SECTIONS_MAX;nSect++){
						if(pRcp->StressStageProgInfo.Line[nLine].bScanValid[nSect]){
							dSigma += pow(tResult.dStressSection[nSect] - pStat->dAverage,2.0);
						}
					}
				}
			}
			pStat->dStandardDeviation = sqrt(dSigma / (double)(nValidNum -1));
		}else{
			pStat->dStandardDeviation = 0.0;
		}
	}else{
		pStat->dMaximum = 0.0;
		pStat->dMinimum = 0.0;
		pStat->dAverage = 0.0;
		pStat->dRange = 0.0;
		pStat->dStandardDeviation = 0.0;
		pStat->Valid = FALSE;
	}

}
//2009.08.31 bagus stress --}--
//2009.09.15 bagus se --{--

/////////////////////////////////////////////////////////////////////////////
// SplitString
//	������𕪊����Ċi�[����
// pStrData: ��������
// pStrArray: ����������������i�[����
// chSplitter: �������镶��
// bTrim: �i�[����ۂɗ����g�������s�����ǂ���
/////////////////////////////////////////////////////////////////////////////
void CMainFrame::SplitString(CString *pStrData,CStringArray *pStrArray,TCHAR chSplitter,BOOL bTrim /*= FALSE*/)
{
	int nPos;
	int nStart;
	BOOL bData;

	pStrArray->RemoveAll();

	for(nPos = 0,bData= FALSE,nStart = 0;
		nPos < pStrData->GetLength();
		nPos++){

		if(pStrData->GetAt(nPos) == chSplitter){
			//�����̋�؂�ɓ��B����
			if(bData){
				CString s = pStrData->Mid(nStart,nPos - nStart);
				if(bTrim){
					s.TrimLeft();
					s.TrimRight();
				}
				pStrArray->Add(s);
				bData = FALSE;
			}
		}else{
			if(!bData)
				nStart = nPos;
			bData = TRUE;
		}
	}
	//�c��
	if(bData){
		CString s = pStrData->Mid(nStart);
		if(bTrim){
			s.TrimLeft();
			s.TrimRight();
		}
		pStrArray->Add(s);
		bData = FALSE;
	}

}
////////////////////////////////////////////////////////////////////////////////
// LoadSERawData
//	SE�̐��f�[�^���擾����
////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::LoadSERawDataByPointNo(int nPointNo,CSERawDataList *pList)
{
	CString strFilePath;
	BOOL bRet = TRUE;
	//2009.09.24 bagus se --{--
	SE_XMP sexmp;
	char szXmpDataFileName[_MAX_FNAME]; 		// Data�t�@�C����
	char szXmpDataExt[_MAX_EXT];				// Data�g���q
	char szXmpFittingDataFileName[_MAX_FNAME];	// FittingData�t�@�C����
	char szXmpFittingDataExt[_MAX_EXT]; 		// FittingData�g���q

	ConfigFile_GetNanoSpecIni(&sexmp, CONFIG_FILE_SE_XMP);
	//�t�@�C���p�X����t�@�C�����擾
	_splitpath(sexmp.szDataPath, NULL, NULL, szXmpDataFileName, szXmpDataExt);
	_splitpath(sexmp.szFittingDataPath, NULL, NULL, szXmpFittingDataFileName, szXmpFittingDataExt);
	//2009.09.24 bagus se --}--


	pList->RemoveAll();
	//�v���l
	//���菈�����͑��茋�ʂ̈ꎞ�ۑ��f�B���N�g���ɐ��f�[�^������̂ł��̃p�X���쐬����B
	//2009.09.24 bagus se --{--
	//strFilePath.Format("%s%s_%05d%s",g_szData_Measured_Data_Temp_Dir,"sedata",nPointNo,DAT_EXT);
	strFilePath.Format("%s%s_%05d%s",g_szData_Measured_Data_Temp_Dir,szXmpDataFileName,nPointNo,DAT_EXT);
	bRet = bRet && LoadSERawData(strFilePath.GetBuffer(0),pList,TRUE);
	//���f���f�[�^�͂�߂Ȃ��Ă��\������
	//strFilePath.Format("%s%s_%05d%s",g_szData_Measured_Data_Temp_Dir,"gendata",nPointNo,DAT_EXT);
	strFilePath.Format("%s%s_%05d%s",g_szData_Measured_Data_Temp_Dir,szXmpFittingDataFileName,nPointNo,DAT_EXT);
	//2009.09.24 bagus se --}--
	LoadSERawData(strFilePath.GetBuffer(0),pList,FALSE);

	return bRet;
}
////////////////////////////////////////////////////////////////////////////////
// LoadSERawData
//	SE�̐��f�[�^���擾����
////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::LoadSERawData(LPCTSTR lpszFilePath,CSERawDataList *pList,BOOL bRawData)
{
	BOOL bRet = FALSE;
	int nHeaderSkipNum = 3;
	int nMinDataNum = 6;
	CStringArray aryElem;
	CStdioFile inFile;
	CString strLine;
	SE_RAW_ONE_DATA oneRawData;
	int nLineNo;
	BOOL bEB = FALSE;


	ASSERT(pList != NULL);
	ASSERT(lpszFilePath != NULL);

	//�N���A
	//pList->RemoveAll();
	if(!inFile.Open(lpszFilePath,CFile::modeRead)){
		return FALSE;
	}

	nLineNo = 0;
	while(inFile.ReadString(strLine)){
		//�ŏ��̃w�b�_�̓X�L�b�v����
		if(nLineNo < nHeaderSkipNum){
			nLineNo++;
			continue;
		}
		//��s�̃f�[�^��v�f�ɕ�������
		SplitString(&strLine,&aryElem,_T('\t'),TRUE);

		if(aryElem.GetSize() < 0){
			//��s�ɂԂ�����
			break;
		}
		memset(&oneRawData,NULL,sizeof(oneRawData));
		CString s = aryElem.GetAt(0);
		int nDataOffset = 0;
		if(s == "Eb"){
			bEB = TRUE;
			nDataOffset = 1;
			if(aryElem.GetSize() < 7){
				//�v�f������Ȃ�
				continue;
			}
		}else if(s == "dpolE" || s == "dpolEb"){
			//�擾����K�v�̂Ȃ��f�[�^
			break;
		}else{
			oneRawData.bEb = FALSE;
			if(aryElem.GetSize() < 6){
				//�v�f������Ȃ�
				continue;
			}

		}
		//�f�[�^�̎擾
		double dWaveLength = _tcstod(aryElem.GetAt(nDataOffset+0),NULL);
		if(pList->Lookup(dWaveLength,oneRawData)){
			if(bRawData){
				oneRawData.dPsi 			= _tcstod(aryElem.GetAt(nDataOffset+2),NULL);
				oneRawData.dDelta			= _tcstod(aryElem.GetAt(nDataOffset+3),NULL);
				oneRawData.dPsiSD			= _tcstod(aryElem.GetAt(nDataOffset+4),NULL);
				oneRawData.dDeltaSD 		= _tcstod(aryElem.GetAt(nDataOffset+5),NULL);
			}else{
				oneRawData.bValidModelData	= TRUE;
				oneRawData.dPsiModel		= _tcstod(aryElem.GetAt(nDataOffset+2),NULL);
				oneRawData.dDeltaModel		= _tcstod(aryElem.GetAt(nDataOffset+3),NULL);
				oneRawData.dPsiSDModel		= _tcstod(aryElem.GetAt(nDataOffset+4),NULL);
				oneRawData.dDeltaSDModel	= _tcstod(aryElem.GetAt(nDataOffset+5),NULL);
			}
			pList->SetAt(dWaveLength,oneRawData);
		}else{
			if(bRawData){
				oneRawData.dWaveLength		= _tcstod(aryElem.GetAt(nDataOffset+0),NULL);
				oneRawData.dLightInputAngle = _tcstod(aryElem.GetAt(nDataOffset+1),NULL);
				oneRawData.dPsi 			= _tcstod(aryElem.GetAt(nDataOffset+2),NULL);
				oneRawData.dDelta			= _tcstod(aryElem.GetAt(nDataOffset+3),NULL);
				oneRawData.dPsiSD			= _tcstod(aryElem.GetAt(nDataOffset+4),NULL);
				oneRawData.dDeltaSD 		= _tcstod(aryElem.GetAt(nDataOffset+5),NULL);
				pList->SetAt(dWaveLength,oneRawData);
			}else{
				//�Ȃɂ����Ȃ�
			}

		}

		nLineNo++;
	}

	inFile.Close();

	return TRUE;
}
//2009.09.15 bagus se --}--
//2009.10.28 bagus 2point-distance --{--
LRESULT CMainFrame::OnDistancePopup(WPARAM wparam, LPARAM lparam)
{
	CPointSetupMeasurement2PointDlg dlg;
	RCP_DATA rcp;
	DWORD dwPointNo = (DWORD)lparam;
	CString l_strCap, l_strMsg;
	int bOK;

#if 1
	m_pDoc->GetRcpData(&rcp);
	memcpy(dlg.m_ScanPoint,rcp.ScanPoint,sizeof(rcp.ScanPoint));
	//2009.11.12 bagus 2point-distance --{--
	//�}�j���A�����莞�ɂ͂����œǂݍ���
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
		RecipeFile_LoadRecipe(&dlg.m_StageProgInfoHdr, rcp.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
	}else{
		dlg.m_StageProgInfoHdr = rcp.StageProgInfoHdr;
	}
	//2009.11.12 bagus 2point-distance --}--

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(dlg.m_StageProgInfoHdr.SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	LoadStringML(IDS_TWO_POINT_DISTANCE_POS_SETTING_CAPTION, l_strCap, "2POINT DISTANCE MEASUREMENT POSITION SETTING");
	LoadStringML(IDS_TWO_POINT_DISTANCE_POS_SETTING_MESSAGE, l_strMsg, "2POINT DISTANCE MEASUREMENT POSITION SETTING");
	dlg.SetCaption(l_strCap);
	dlg.SetMessage(l_strMsg);


	if(dwPointNo % 2 == 0){
		//�I�_�̐ݒ�Ȃ̂Ŏn�_������������
		STAGE_COORD tCoord;
		tCoord = m_pDoc->GetLastDistancePoint();
		dlg.m_ScanPoint[dwPointNo-2].lX = tCoord.lX;
		dlg.m_ScanPoint[dwPointNo-2].lY = tCoord.lY;
		dlg.m_b2nd = TRUE;
//	}
//	dlg.m_iCurrPoint = dwPointNo / 2 + 1;
		dlg.m_iCurrPoint = dwPointNo / 2;
	} else {
		dlg.m_iCurrPoint = (dwPointNo + 1) / 2;
	}
	dlg.m_iTeachingMode = m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE ? CPointSetupMeasurement2PointDlg::MODE_AUTO_MEAS :  CPointSetupMeasurement2PointDlg::MODE_MANUAL_MEAS;

	if(dlg.DoModal() == IDOK){
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
			//�}�j���A������
			if(dlg.m_bComp){
				bOK = 2;
			}else{
				bOK = 3;
			}
		}else{
		STAGE_COORD tCoord;
		tCoord.lX = dwPointNo % 2 == 1 ? dlg.m_l1stX : dlg.m_l2ndX;
		tCoord.lY = dwPointNo % 2 == 1 ? dlg.m_l1stY : dlg.m_l2ndY;
		m_pDoc->SetLastDistancePoint(tCoord);
			bOK = 1;
		}
	}else{
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
			//2009.11.27 bagus 2point-Distance --{--
			//bOK = 2;
			bOK = 3;
			//2009.11.27 bagus 2point-Distance --}--
		}else{
			bOK = 0;
		}
	}
#else
	STAGE_COORD tCoord;
	m_pDoc->GetRcpData(&rcp);
	tCoord.lX = rcp.ScanPoint[dwPointNo-1].lX;
	tCoord.lY = rcp.ScanPoint[dwPointNo-1].lY;
	m_pDoc->SetLastDistancePoint(tCoord);
	bOK = TRUE;
#endif

	//�`�[�t�փ��b�Z�[�W�𓊂���
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		// AF�|�b�v�A�b�v�I������
		::PostMessage(l_hChiefDlg, WM_DISP_DISTANCE_POPUP_END, bOK, 0);
		if(bOK == 2){
			//�����ʂ����B
			m_wndSplitter1.GetPane(1,0)->PostMessage(WM_DISPMENU_MEASUREMENT_COMPLETE,WM_DISPMENU_MEASUREMENT_COMPLETE,0);
		}else if(bOK == 3){
			m_wndSplitter1.GetPane(1,0)->PostMessage(WM_TESTMODE_ONEPOINT_END,WM_TESTMODE_ONEPOINT_END,0);
		}
	} else {
		//�G���[�����L�q���邱�ƁI
	}
	return TRUE;
}
double CMainFrame::Calc2PointDistance(long lStartX,long lStartY,long lEndX,long lEndY)
{
	double dDiffX;
	double dDiffY;

	dDiffX = lStartX - lEndX;
	dDiffY = lStartY - lEndY;

	dDiffX *= dDiffX;
	dDiffY *= dDiffY;

	return sqrt(dDiffX + dDiffY);
}
void CMainFrame::DistanceDataStatistics(STATISTICS* Statistics)
{
	ONE_POINT_DATA OnePointData;
	ONE_POINT_DATA OnePointData2;
	memset(&OnePointData, 0, sizeof(OnePointData));
	memset(&OnePointData2, 0, sizeof(OnePointData2));

	int Point, Type;
	int ValidPointCount = 0;
	double Min, Max, Ave, Std, Range;
	double DistanceSum;
	double Sigma;
	BOOL First;
	double dData;

	for(Type = 0;Type < 3;Type++){
		Min = Max = Ave = Std = Range = 0.0;
		First = TRUE;
		DistanceSum = 0.0;
		ValidPointCount = 0;

		//

		///// Min / Max /////
		for(Point = 0 ; Point < SCAN_POINT_MAX ; Point += 2){
			m_pDoc->GetOnePointData(&OnePointData, Point+1);
			m_pDoc->GetOnePointData(&OnePointData2, Point+2);
			if(OnePointData.Valid == FALSE || OnePointData2.Valid == FALSE){
				break;
			}
			switch(Type){
			case 0: dData = Calc2PointDistance(OnePointData.xyz.lX,OnePointData.xyz.lY,OnePointData2.xyz.lX,OnePointData2.xyz.lY); break;
			case 1: dData = fabs(OnePointData.xyz.lX - OnePointData2.xyz.lX); break;
			case 2: dData = fabs(OnePointData.xyz.lY - OnePointData2.xyz.lY); break;
			}

			if(First){
				Min = Max = dData;
				First = FALSE;
			}
			else{
				///// Glass Minimum /////
				if(Min > dData){
					Min = dData;
				}

				///// Glass Maximum /////
				if(Max < dData){
					Max = dData;
				}
			}

			DistanceSum += dData;

			ValidPointCount++;
		}

		if(ValidPointCount > 0){
			///// Glass Average /////
			Ave = (float)(DistanceSum / (double)ValidPointCount);

			///// Glass Standard Deviation /////
			Std = 0.0;
			Sigma = 0.0;
			if(ValidPointCount >= 2){
				for(Point = 0 ; Point < SCAN_POINT_MAX ; Point += 2){
					m_pDoc->GetOnePointData(&OnePointData, Point+1/*iNowPoint*/);
					m_pDoc->GetOnePointData(&OnePointData2, Point+2/*iNowPoint*/);
					if(OnePointData.Valid == FALSE || OnePointData2.Valid == FALSE){
						break;
					}
					switch(Type){
					case 0: dData = Calc2PointDistance(OnePointData.xyz.lX,OnePointData.xyz.lY,OnePointData2.xyz.lX,OnePointData2.xyz.lY); break;
					case 1: dData = fabs(OnePointData.xyz.lX - OnePointData2.xyz.lX); break;
					case 2: dData = fabs(OnePointData.xyz.lY - OnePointData2.xyz.lY); break;
					}
					//Distance = Calc2PointDistance(OnePointData.xyz.lX,OnePointData.xyz.lY,OnePointData2.xyz.lX,OnePointData2.xyz.lY);

					//Sigma += pow((Distance - Ave), 2.0);
					Sigma += pow((dData - Ave), 2.0);
				}

				Std = (float)sqrt((Sigma / (double)(ValidPointCount - 1)));
			}

			////// Glass Range /////
			Range = Max-Min;

			Statistics[Type].dMinimum			= Min;
			Statistics[Type].dMaximum			= Max;
			Statistics[Type].dAverage			= Ave;
			Statistics[Type].dStandardDeviation = Std;
			Statistics[Type].dRange 			= Range;
			Statistics[Type].Valid			   = TRUE;
		}
		else{
			Statistics[Type].dMinimum			= 0;
			Statistics[Type].dMaximum			= 0;
			Statistics[Type].dAverage			= 0;
			Statistics[Type].dStandardDeviation = 0;
			Statistics[Type].dRange 			= 0;
			Statistics[Type].Valid			   = FALSE;
		}
	}
	m_pDoc->SetStatisticsValidPointCount(ValidPointCount);
}
//2009.10.28 bagus 2point-distance --}--
//2009.11.03 bagus MS --{--
// �������p�̃|�b�v�A�b�v��ʂ̏���
LRESULT CMainFrame::OnMSPopup(WPARAM wparam,LPARAM lparam)
{
	CPointSetupTeachingMSDlg dlg;
	DWORD dwPointNo = (DWORD)lparam;
	int nRet = 0;
	RCP_DATA rcp;
	CString l_strCap, l_strMsg, strMsg;

	dlg.m_iTeachingMode = m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE ? CPointSetupTeachingMSDlg::MODE_MANUAL_MEAS : CPointSetupTeachingMSDlg::MODE_AUTO_MEAS;
	m_pDoc->GetRcpData(&rcp);
	//2009.11.12 bagus 2point-distance --{--
	//�}�j���A�����莞�ɂ͂����œǂݍ���
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
		RecipeFile_LoadRecipe(&dlg.m_StageProgInfoHdr, rcp.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
	}else{
		dlg.m_StageProgInfoHdr = rcp.StageProgInfoHdr;
	}
	//2009.11.12 bagus 2point-distance --}--

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(dlg.m_StageProgInfoHdr.SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	dlg.m_iCurrPoint = dwPointNo + 1;
	LoadStringML(IDS_MANUAL_MEASUREMENT_POINT_TEACHING_CAPTION, l_strCap, "MANUAL MEASUREMENT - POINT TEACHING");
	LoadStringML(IDS_MANUAL_MEASUREMENT_POINT_TEACHING_MESSAGE, l_strMsg, "MANUAL MEASUREMENT - POINT TEACHING");
	dlg.SetCaption(l_strCap);
	dlg.SetMessage(l_strMsg);

	if(dlg.DoModal() == IDOK){
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
			//�蓮���蒆
			nRet = 0;
		}else{
			//�������蒆
			if(dlg.m_bMoveNext){
				nRet = 1;
			}else{
				//�P��OK�������ꂽ�̂ŉ�ʂ����̂�
				return TRUE;
			}
		}
	}else{
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
			//�蓮���蒆
			nRet = 0;
		}else{
			nRet = 0;
		}
	}

	//�`�[�t�փ��b�Z�[�W�𓊂���
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		// AF�|�b�v�A�b�v�I������
		::PostMessage(l_hChiefDlg, WM_DISP_MS_POPUP_END, nRet, 0);
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){
			//2009.11.26 bagus MS --{--
			//m_wndSplitter1.GetPane(1,0)->PostMessage(WM_TESTMODE_ONEPOINT_END,WM_TESTMODE_ONEPOINT_END,0);
			m_wndSplitter1.GetPane(1,0)->PostMessage(WM_DISPMENU_MANUMEAS_EXIT,WM_DISPMENU_MANUMEAS_EXIT,0);
			//2009.11.26 bagus MS --}--
		}
	} else {
		//�G���[�����L�q���邱�ƁI
	}
	return TRUE;
}
//2009.11.03 bagus MS --}--

//2009.11.04 bagus CA --{--
BOOL CMainFrame::GetCTAMeasureResult(CTARESULT *pResult)
{
	CSharedMemory<CTARESULT>* pCTAData;
	CString strBuf;

	pCTAData = new CSharedMemory<CTARESULT>;
	if(!pCTAData->Open(_T(CTAFILEMAPPINGNAME))){
		LoadStringML(IDS_CA_RESULT_GET_ERROR, strBuf, "Can't get Contact Angle result.");
		AfxMessageBox(strBuf);
		return FALSE;
	}
	*pResult = *pCTAData->GetSharedMemoryPtr();

// 2009.11.05 K.Matsuo -->
	if ( pCTAData ) {
		delete pCTAData;
		pCTAData = NULL;
	}
// 2009.11.05 K.Matsuo <--

	return TRUE;

}
//2009.11.04 bagus CA --{--
//2009.11.10 bagus RS --{--
BOOL CMainFrame::GetResistMeasureResult(RESISTRESULT *pResult)
{
	CSharedMemory<RESISTRESULT>* pResistData;
	CString strBuf;

	pResistData = new CSharedMemory<RESISTRESULT>;
	if(!pResistData->Open(_T(RESISTFILEMAPPINGNAME))){
		LoadStringML(IDS_RESIST_RESULT_GET_ERROR, strBuf, "Can't get Resistance result.");
		AfxMessageBox(strBuf);
		return FALSE;
	}

	*pResult = *pResistData->GetSharedMemoryPtr();

	if ( pResistData ) {
		delete pResistData;
		pResistData = NULL;
	}

	return TRUE;
}
//2009.11.10 bagus RS --}--

// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
LRESULT CMainFrame::OnConfirmPopup(WPARAM wparam,LPARAM lparam)
{
	int iSrcCmdId = (int)wparam;
	CConfirmDlg dlg;
	UINT nButtonStyle = MB_OKCANCEL;
	UINT nIconType = MB_ICONQUESTION;
	CString l_strCaption;
	CString l_strMessageText;
	CString l_strFormat;
	int iDlgResult;
	SYSTEM_CONFIG l_SystemConfig;
	SR_TURRET l_srTurret[SR_LENS_MAX];
	SR_FILTER l_srFilter[SR_FILTER_MAX];
	LPCTSTR pszFilterName[LANGUAGE_MAX][OPT_FILTER_MAX] = {
		{OPTICAL_FILTER_TYPE_ITEM_ENU[0], OPTICAL_FILTER_TYPE_ITEM_ENU[1], OPTICAL_FILTER_TYPE_ITEM_ENU[2], l_srFilter[0].szName, l_srFilter[1].szName, l_srFilter[2].szName},	// Enu
		{OPTICAL_FILTER_TYPE_ITEM_JPN[0], OPTICAL_FILTER_TYPE_ITEM_JPN[1], OPTICAL_FILTER_TYPE_ITEM_JPN[2], l_srFilter[0].szName, l_srFilter[1].szName, l_srFilter[2].szName},	// Jpn
	};

	switch(iSrcCmdId){
	case 0: // set lens and filter.
		{
			int iLens = (int)LOWORD(lparam);
			int iFilter = (int)HIWORD(lparam);
			LoadStringML(IDS_SET_LENS_CAPTION, l_strCaption, "Set Lens and filter");
			LoadStringML(IDS_SET_LENS_MESSAGE, l_strFormat, "Please set lens and filter.\nLens:%s\nFilter:%s\nReference:%s");

			ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
			ConfigFile_GetNanoSpecIni(l_srTurret, CONFIG_FILE_SR_TURRET);
			ConfigFile_GetNanoSpecIni(l_srFilter, CONFIG_FILE_SR_FILTER);

			RCP_DATA rcp;
			m_pDoc->GetRcpData(&rcp);

			struct tm tmFileTime;
			int timeoutRes = MEAS_CheckRefFileElapsedTimeOut(rcp.szRecipeName, rcp.MeasProgInfo.Ref.hdr.dLifeTime,tmFileTime);

			char szTime[256] = "";
			
			// 0 : ����i�������L���j
			// 1 : �G���[�i�t�@�C�������݂��Ȃ��j
			// 2 : �G���[�i�����؂�j
			CString l_strExist;
			switch(timeoutRes)
			{
				case 0:
					LoadStringML(IDS_EXIST, l_strExist, "Exist");
					sprintf(szTime,"(%d/%02d/%02d %02d:%02d:%02d)",tmFileTime.tm_year + 1900,tmFileTime.tm_mon + 1,tmFileTime.tm_mday,tmFileTime.tm_hour,tmFileTime.tm_min,tmFileTime.tm_sec);
					l_strExist += szTime;
					break;
				case 1:
					LoadStringML(IDS_NOEXIST, l_strExist, "No Exist");
					break;
				case 2:
					LoadStringML(IDS_EXPIRED, l_strExist, "Expired");
					sprintf(szTime,"(%d/%02d/%02d %02d:%02d:%02d)",tmFileTime.tm_year + 1900,tmFileTime.tm_mon + 1,tmFileTime.tm_mday,tmFileTime.tm_hour,tmFileTime.tm_min,tmFileTime.tm_sec);
					l_strExist += szTime;
					break;
				default:
					break;
			}


			l_strMessageText.Format(l_strFormat, l_srTurret[iLens].szName, pszFilterName[l_SystemConfig.nLanguage][iFilter],l_strExist);
			nButtonStyle = MB_OK;
			nIconType = MB_ICONINFORMATION;
			break;
		}
	case 1: // normal reference expiration.
		LoadStringML(IDS_N_REF_EXPIRE_CAPTION, l_strCaption, "Reference expiration");
		LoadStringML(IDS_N_REF_EXPIRE_MESSAGE, l_strMessageText, "A term of validity of reference has expired. Is reference data re-acquired?");
		nButtonStyle = MB_YESNO;
		nIconType = MB_ICONQUESTION;
		break;
	case 10: // move normal reference position.
		LoadStringML(IDS_MOVE_N_REF_POS_CAPTION, l_strCaption, "Move to reference position");
		LoadStringML(IDS_MOVE_N_REF_POS_MESSAGE, l_strMessageText, "Please move to reference position.");
		nButtonStyle = MB_OKCANCEL;
		nIconType = MB_ICONQUESTION;
		break;
	case 11: // move dark reference position.
		LoadStringML(IDS_MOVE_D_REF_POS_CAPTION, l_strCaption, "Move to dark reference position");
		LoadStringML(IDS_MOVE_D_REF_POS_MESSAGE, l_strMessageText, "Please move to dark reference position.");
		nButtonStyle = MB_OKCANCEL;
		nIconType = MB_ICONQUESTION;
		break;
	case 20: // move measurement point position.
		LoadStringML(IDS_MOVE_MEAS_POS_CAPTION, l_strCaption, "Move to measurement point position");
		LoadStringML(IDS_MOVE_MEAS_POS_MESSAGE, l_strMessageText, "Please move to measurement point position.");
		nButtonStyle = MB_OK;
		nIconType = MB_ICONINFORMATION;
		break;
	default:
		break;
	}

	dlg.SetButtonStyle(nButtonStyle);
	dlg.SetMessageText(l_strMessageText, l_strCaption, nIconType);
	iDlgResult = dlg.DoModal();

	//�`�[�t�փ��b�Z�[�W�𓊂���
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		::PostMessage(l_hChiefDlg, WM_DISP_CONFIRM_POPUP_END, wparam, iDlgResult);

		// ���t�@�����X�L�����Z�������͑���I���Ɠ��������Ƃ���
		if(iSrcCmdId == 10 || iSrcCmdId == 11){
			if(iDlgResult == IDCANCEL){
				m_wndSplitter1.GetPane(1,0)->PostMessage(WM_DISPMENU_MANUMEAS_EXIT,WM_DISPMENU_MANUMEAS_EXIT,0);
			}
		}
	} else {
		//�G���[�����L�q���邱�ƁI
	}

	return 0L;
}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
