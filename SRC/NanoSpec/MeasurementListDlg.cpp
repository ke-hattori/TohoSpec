// MeasurementListDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "Math.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MultiLangHelper.h"
#include "MeasurementTabView.h"
#include "MeasurementListDlg.h"
#include "NanoSpecDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MEASUREMENTGRIDROWMAX	(31)//(2000)
#define MEASUREMENTGRIDCOLMAX	(50)
#define STATISTICSGRIDROWMAX	(5)
#define STATISTICSGRIDCOLMAX	(18)

#define MEASUREMENTGRIDCOLMIN	(10)
//2009.09.14 bagus stress --{--
//#define STATISTICSGRIDCOLMIN	  (11)
#define STATISTICSGRIDCOLMIN	(10)
//2009.09.14 bagus stress --}--

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

// Helper Fucntion
// �w���\�L�֕ϊ�����֐��iprintf������%E���������A�w������2���\���ɂ��������ߎ���֐���p�Ӂj
static void ConvertToExpNumber(double dNumber, LPTSTR pszExpNumber)
{
	sprintf(pszExpNumber, "%.3E", dNumber);

	// Two Digit Exponent
	pszExpNumber[strlen(pszExpNumber) - 3] = pszExpNumber[strlen(pszExpNumber) - 2];
	pszExpNumber[strlen(pszExpNumber) - 2] = pszExpNumber[strlen(pszExpNumber) - 1];
	pszExpNumber[strlen(pszExpNumber) - 1] = '\0';
}


// #########################################################################
// CMeasurementListDlg �_�C�A���O
// #########################################################################

// =========================================================================
//
CMeasurementListDlg::CMeasurementListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasurementListDlg::IDD, pParent)
{
	//Kojika 20090525 Add
	CString strBuffer;
	//Kojika 20090525 Add End
	//{{AFX_DATA_INIT(CMeasurementListDlg)
	//}}AFX_DATA_INIT

	// LOGFONT
	ZeroMemory(&m_logFont, sizeof(m_logFont));
	m_logFont.lfHeight = 11;
	m_logFont.lfWeight = FW_NORMAL;
	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = FIXED_PITCH | FF_SCRIPT;
	strcpy(m_logFont.lfFaceName, "MS UI Gothic");

//	memset(m_szName, 0, sizeof(m_szName));
	memset(m_dBuff, 0, sizeof(m_dBuff));
	memset(m_iPointStates, 0, sizeof(m_iPointStates));

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;

}

// =========================================================================
//
CMeasurementListDlg::~CMeasurementListDlg()
{
	m_pDoc->SetShowCompPointList(0);
}

// =========================================================================
//
void CMeasurementListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasurementListDlg)
	DDX_Control(pDX, IDC_POINT_GRAPH, m_PointGraph);
	DDX_Control(pDX, IDC_MEASUREMENT_LIST_GRID, m_MeasurementListGrid);
	DDX_Control(pDX, IDC_STATISTICS_LIST_GRID, m_StatisticsListGrid);
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CMeasurementListDlg, CDialog)
	//{{AFX_MSG_MAP(CMeasurementListDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	//ON_NOTIFY(NM_CLICK, IDC_MEASUREMENT_LIST_GRID, OnGridClick)
	ON_NOTIFY(GVN_SELCHANGING, IDC_MEASUREMENT_LIST_GRID, OnSelChangeing)
//	ON_MESSAGE(WM_MEAS_COMP_EVENT, SetMeasurementData)

END_MESSAGE_MAP()

// =========================================================================
// CMeasurementListDlg ���b�Z�[�W �n���h��

// =========================================================================
//
BOOL CMeasurementListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

// 2014.01.17 Bagus Add (Stage None�Ή�) -->
	//�V�X�e���R���t�B�O�ݒ�ǂݍ���
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2014.01.17 Bagus Add (Stage None�Ή�) <--

	///// Get Selected Recipe Name /////
//	((CMainFrame*)AfxGetMainWnd())->GetSelectListName(m_szName);

	///// Load Data /////
	LoadData();

	///// Set Data /////
	SetData();

	///// Static Control /////
	m_stcMainRecipeName.SubclassWindow(GetDlgItem(IDC_MAIN_RECIPE_NAME)->GetSafeHwnd());
	m_stcMainRecipeName.SetBkColor(WATER_COLOR);
	m_stcHeadType.SubclassWindow(GetDlgItem(IDC_HEAD_TYPE)->GetSafeHwnd());
	m_stcHeadType.SetBkColor(WATER_COLOR);
	m_stcMeasurementItem.SubclassWindow(GetDlgItem(IDC_MEASUREMENT_ITEM)->GetSafeHwnd());
	m_stcMeasurementItem.SetBkColor(WATER_COLOR);
	m_stcMeasurementPoint.SubclassWindow(GetDlgItem(IDC_MEASUREMENT_POINT)->GetSafeHwnd());
	m_stcMeasurementPoint.SetBkColor(WATER_COLOR);
	//2009.07.30 bagus stress �X�g���X�@�\�ǉ� --{--
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // �w�b�h�^�C�v
	m_stcMeasurementLS.SubclassWindow(GetDlgItem(IDC_MEASUREMENT_LS)->GetSafeHwnd());
	m_stcMeasurementLS.SetBkColor(WATER_COLOR);
	if(wHeadType != HEAD_TYPE_STRESS){
		//2009.09.08 bagus stress --{--
		//���͑���ȊO��LS�̕\�����Ȃ��̂Ń|�C���g�\���̑傫�������ǂ�
		RECT tRect1,tRect2;
		m_stcMeasurementPoint.GetWindowRect(&tRect1);
		m_stcMeasurementLS.GetWindowRect(&tRect2);
		this->ScreenToClient(&tRect1);
		this->ScreenToClient(&tRect2);
// 2010.01.13 bagus Stress --{--
//		  m_stcMeasurementPoint.MoveWindow(tRect1.left ,tRect1.top,tRect2.right - (tRect1.left ),tRect2.bottom - tRect2.top);
		m_stcMeasurementPoint.MoveWindow(tRect1.left, tRect1.top, 122, tRect2.bottom - tRect2.top);
// 2010.01.13 bagus Stress --}--
		//2009.09.08 bagus stress --}--
		m_stcMeasurementLS.ShowWindow(SW_HIDE);
	}else{
		CString strBuffer;
		LoadStringML(IDS_STRESS_MEAS_POINT_LABEL,strBuffer,"Meas Point&LS");
		SetDlgItemText(IDC_MEASUREMENT_POINT_LABEL,strBuffer);
	}
	//2009.07.30 bagus stress �X�g���X�@�\�ǉ� --}--

	///// Grid Control Initialize /////
	MeasurementListGrid_InitGrid();
	StatisticsListGrid_InitGrid();
	MeasurementListGrid_HeaderSet();
	StatisticsListGrid_HeaderSet();

// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--

	///// Point Graph Initialize /////
	PointGraph_DataSet();
	m_PointGraph.Measure(0);

	SetTimer(SHOW_LIST_TIMER, 500, NULL);

	m_bCellSelectedFlg = FALSE; //�I���s�̗L���t���O

	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //�}�j���A������
	{
		GetDlgItem(IDC_SAMPLE_SIZE)->ShowWindow(SW_HIDE);
	}

/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
	CWnd* l_pcwndParent = this->GetParent();
	RECT l_rectParentAR;	l_pcwndParent->GetWindowRect(&l_rectParentAR);
	/* �e�E�B���h�E���^�u�ƌ��ߕt���������ł��D	*/
	((CTabCtrl*) l_pcwndParent)->AdjustRect(FALSE, &l_rectParentAR);
	RECT l_rectThisDlg;	this->GetWindowRect(&l_rectThisDlg);
	if ((g_lModelType == MODEL_T3100) &&
		((l_rectParentAR.bottom - l_rectParentAR.top) < (l_rectThisDlg.bottom - l_rectThisDlg.top))) {
		/* �I�t�Z�b�g�v�Z	*/
		long l_lDeltaY = (l_rectThisDlg.bottom - l_rectThisDlg.top) - (l_rectParentAR.bottom - l_rectParentAR.top);
		l_lDeltaY += 1;
		this->ChgSizeCtrl(-l_lDeltaY);
	}
/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */

	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
					// ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// =========================================================================
// OnGridClick MeasurementListGrid�I���s�̃X�L�����|�C���g�ɂ��āA�}�b�v�̃X�L�����|�C���g��I��F�ɐݒ肷��
//void CMeasurementListDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
/*{
	///// Draw SelectPoint /////
	CCellID CellID;
	CellID = m_MeasurementListGrid.GetFocusCell();
	if ( !m_MeasurementListGrid.IsValid(CellID) )
		return;
	//PointGraph�I���|�C���g�F�ύX
	m_PointGraph.Select(CellID.row-1);
}
*/

// =========================================================================
// OnSelChangeing MeasurementListGrid�I���s�̃X�L�����|�C���g�ɂ��āA�}�b�v�̃X�L�����|�C���g��I��F�ɐݒ肷��
void CMeasurementListDlg::OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	///// Draw SelectPoint /////
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID CellID;
	CellID.row=pItem->iRow;
	CellID.col=pItem->iColumn;
	if ( !m_MeasurementListGrid.IsValid(CellID) )
		return;
	//PointGraph�I���|�C���g�F�ύX
	//2009.08.31 bagus stress --{--
	if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		//2009.12.03 bagus stress --{--
		//��U�I������������
		m_PointGraph.Select(-1);
		m_PointGraph.SelectSection(-1);
		//2009.12.03 bagus stress --}--
		if(CellID.row -1 < m_pDoc->GetStoreCount()){
			//��������
			m_PointGraph.Select(CellID.row-1);
		}else{
			m_PointGraph.SelectSection(CellID.row - 1 - m_pDoc->GetStoreCount());
		}
	}else{
		m_PointGraph.Select(CellID.row-1);
	}
	//2009.08.31 bagus stress --}--
}


//// =========================================================================
//// �X�L�����|�C���g���̑��肪�I�������Ƃ��ɑ��茋�ʂ�ҏW����
//LRESULT CMeasurementListDlg::SetMeasurementData(WPARAM wparam, LPARAM lparam)
//{
//MessageBox("�����ɂ͗��Ȃ��͂� hibino", "WARNING!", MB_OK);
////������SetMeasurementData2�ֈړ� hibino
//	return 0L;
//}



// =========================================================================
//
//[�߂�l]
//-1:�������s
// 0:��������(�\����������)
// 1:��������(�\�������Ȃ�)
// 2:��������(�\����������(�ŐV�}�����ւ̈ړ��Ȃ�))
int CMeasurementListDlg::SetMeasurementData2()
{

	char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1]; 	//�O���b�h�̃^�C�g��(X,Y,Z)
	char szTitleAf[AF_HEADER_LEN + 1];					//�O���b�h�̃^�C�g��(AF)
	char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];	//�O���b�h�̃^�C�g��(�擪��Thick1?) ��X,Y,Z,AF�͊܂܂�
	int iLabelCount=0;									//�O���b�h�̃^�C�g����(�擪��Thick1?) ��X,Y,Z,AF�͊܂܂�
	int iCurrentSetPoint = 0; //��ԍŌ�Ƀf�[�^���i�[(������)�����|�C���g�ԍ�

	int iDispLabelCount=0;
	ONE_POINT_DATA OnePointData;						//1�|�C���g���̑���f�[�^
	STATISTICS Statistics[ADAPRESULT_COLS_MAX]; 		//���v�f�[�^
	int iMeasureNum=0;									//����|�C���g��
	int iPoint=0;	// ����|�C���g���Ƃ̉�
	int iShowCompletionPointList; //��ʂɕ\���ς݂̃|�C���g��
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	///// ScanType��Paramaeter /////
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // �w�b�h�^�C�v
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;
	int nTotalSressLineNum = 0;
	//2009.10.28 bagus 2point-distance --{--
	BOOL bMeasDistance = wHeadType == HEAD_TYPE_SR && wScanType == MEAS_PROG_TYPE_SR_DISTANCE;
	ONE_POINT_DATA DistanceOnePointData;					//2�_�ԋ����̎n�_�p�f�[�^
	int nItemCount = 0;
	//2009.10.28 bagus 2point-distance --}--
	//2009.11.10 bagus RS --{--
	RESISTRESULT	ResistOnePointData;
	//2009.11.10 bagus RS --}--
	//2009.11.17 bagus RS --{--
	BOOL bResitValidData = FALSE;
	//2009.11.17 bagus RS --}--

/* added 2017.04.15 hmenjo �����\���� ---------- { ---------- */
	int l_iUnitIndexThickInt = -1;		/* -1 �͐����\�������Ȃ�	*/
	SR_FIGURE_FORMAT l_SrFigureFormat;
	ConfigFile_GetNanoSpecIni(&l_SrFigureFormat,
					CONFIG_FILE_SR_FIGURE_FORMAT);
	if (0 != l_SrFigureFormat.iThicknessIntOnly) {
		l_iUnitIndexThickInt = 3;		/* -1 �ȊO�̓C���f�b�N�X�l	*/
	}
/* added 2017.04.15 hmenjo �����\���� ---------- } ---------- */

	memset(szTitleXyz, 0, sizeof(szTitleXyz));
	memset(szTitleAf, 0, sizeof(szTitleAf));
	memset(szLabel, 0, sizeof(szLabel));
	memset(&OnePointData, 0, sizeof(OnePointData));
	memset(Statistics, 0, sizeof(Statistics));
	memset(szDispLabel, 0, sizeof(szDispLabel));


	m_pDoc->GetDispLabel(szDispLabel);

	int i;
	CString strBuffer;


	iShowCompletionPointList = m_pDoc->GetShowCompPointList();

	if(m_pDoc->GetSeqMeasMode() == SEQ_ON_REMEASURE) //�����W���[
	{
		if(iShowCompletionPointList == 0)
		{
			for(i=0; i < m_pDoc->GetLastMeasPoint(); i++)
			{
				if(m_pDoc->m_bDoPointMeasFlag[i] == TRUE)
				{
					m_PointGraph.Measure(i);
					break;
				}
			}
		}
	}


	m_pDoc->GetLabel(szTitleXyz, szTitleAf, szLabel,&iLabelCount);
	iDispLabelCount = m_pDoc->GetDispLabelCount();
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	if ( wHeadType == HEAD_TYPE_COMPEASE ) {
		if ( m_MeasurementListGrid.GetColumnCount() < iDispLabelCount + 3 + 1 ) {
			MeasurementListGrid_InitGrid();
			StatisticsListGrid_InitGrid();
		}
		MeasurementListGrid_HeaderSet();
		StatisticsListGrid_HeaderSet();
	}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	iMeasureNum = m_pDoc->GetMeasurePointCount();
	iPoint = iCurrentSetPoint = m_pDoc->GetCurrentSetPoint();

	for(i=0; i < iShowCompletionPointList; i++)
	{
		//2009.09.14 bagus stress --{--
		if(i + 1>= m_MeasurementListGrid.GetRowCount()){
			break;
		}
		//2009.09.14 bagus stress --}--
		int iFocus = (m_MeasurementListGrid.GetItemState(i+1, 1) & GVIS_SELECTED);
		//Saiki 20090608 Add ----->
		if((i == 0) && (iFocus > 0)){
			m_bCellSelectedFlg = TRUE;
		}
		//Saiki 20090608 Add <----
		if((iFocus != 0)&&(m_bCellSelectedFlg == FALSE)) //�s���I������Ă���(���I�����m�F�����ƈȍ~�͍ŐV�s��\�����Ȃ�)
		{
			m_bCellSelectedFlg = TRUE;
/* modified 2014.10.28 hmenjo ���茋�� Auto Scroll ---------- { ---------- */
//			m_MeasurementListGrid.EnsureVisible((i+1), TRUE);
/* modified 2014.10.28 hmenjo ���茋�� Auto Scroll ----------              */
			if (
				(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) &&
				(m_pDoc->GetMeasMode() != MEASMODE_TEST)
				) {
				m_MeasurementListGrid.EnsureVisible((i + 1), TRUE);
			}
/* modified 2014.10.28 hmenjo ���茋�� Auto Scroll ---------- } ---------- */
			break;
		}
	}


	int iCount =0;
	if(iCurrentSetPoint == 0)
	{
		return 1;
	}

	if(iShowCompletionPointList < iCurrentSetPoint)
	{
		iPoint = iCurrentSetPoint;	//�w���[�v���̏I���|�C���g���x���w�i�[�ς݃|�C���g�x�l�ɐݒ�B
		iCount = iShowCompletionPointList;	//���[�v�J�n�_��ݒ�B
	}
	else
	{
		return 1;	//��������(�\�������Ȃ�)
	}

/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
	WORD l_wIncludeColorSetNo = 0;
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

	while ( ++iCount <= iPoint )
	{
		CCellID CellID;
		CellID.row = iCount;

		if (m_MeasurementListGrid.IsValid(CellID) )
			break;

		m_pDoc->GetOnePointAndStatistics(&OnePointData, Statistics, iCount); //(1�|�C���g�ځ�1)

		//2009.11.17 bagus RS --{--
		//��R����̏ꍇ�ɂ�OnePointData.Valid=FALSE�����ɓ����Ă�����
		if(wHeadType == HEAD_TYPE_4PP){
			if(!m_pDoc->GetOneResistData(&ResistOnePointData,iCount)){
				break;
			}
		//2009.11.10 bagus RS --}--
		}else{
			if(OnePointData.Valid == FALSE)
			{
				break;
			}
		}
		//2009.11.17 bagus RS --}--


		int iIndex = 0;
		char szXyzFormat[255+1];
		double dPositionX;
		double dPositionY;
		double dPositionZ;
		int iCol;
		char szDecimalPointBuff[255+1];
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
		char szDecimalPointBuffThickSD[255+1];
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */

		//2009.07.27 bagus stress SR�ȊO�̃w�b�h�^�C�v�ɂ��Ή�
		//if(wHeadType == HEAD_TYPE_SR)
		switch(wHeadType){
		case HEAD_TYPE_SR:

			//2009.10.28 bagus 2point-Distance --{--
			//2�_�ԋ�����SR�w�b�h�����傫���\�����ڂ��Ⴄ
			switch(wScanType){
			default:
				//char szXyzFormat[255+1];
				memset(szXyzFormat, 0, sizeof(szXyzFormat));
				sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
				///// Point Position X //////
				iIndex = 0;
				iIndex++;
				dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
				strBuffer.Format(szXyzFormat, dPositionX);
				AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

				///// Point Position Y //////
				dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
				strBuffer.Format(szXyzFormat, dPositionY);
				AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

				/// ///// Point Position Z //////
				dPositionZ = (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0]) / 10;
				strBuffer.Format(szXyzFormat, dPositionZ);
				AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);


				//// �I�[�g�t�H�[�J�X���藓 ////
				ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
				if(m_SrConfig.bAutoFocus)
				{
					strBuffer.Format("%s", OnePointData.szShowAfValid);
					AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);
				}

				iCol = 0;
				for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+3]:XYZ, [+1]:AF
				{
					int iUnitIndex = 0;
					int iUnit = m_pDoc->GetUnitIndex();
					if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
					else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
					if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
						iUnitIndex = 0;
					}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

					char szDecimalPointBuff[255+1];
					memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
					if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
					{
/* modified 2017.04.15 hmenjo �����\���� ---------- { ---------- */
//						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
/* modified 2017.04.15 hmenjo �����\���� ----------              */
						if (l_iUnitIndexThickInt < 0) {
							sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
						} else {
							sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "f");
						}
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
						sprintf(szDecimalPointBuffThickSD, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */
/* modified 2017.04.15 hmenjo �����\���� ---------- } ---------- */
					}
					else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,K�l�n
					{
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]�Œ�
					}
// 2009.11.12 K.Matsuo ADAP�̕񍐍��ڂ̐���Ή��B2�ڂ�N1[XXX],K1[XXX]���̂��Ƃ�A�ʍ��ڂ̒l��� -->
					else if(strncmp(szDispLabel[i], "Const", strlen("Const")) == 0 ||
							strncmp(szDispLabel[i], "Eg", strlen("Eg")) == 0 ||
							strncmp(szDispLabel[i], "Fraction", strlen("Fraction")) == 0)
					{
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]�Œ�
					}
// 2009.11.12 K.Matsuo ADAP�̕񍐍��ڂ̐���Ή��B2�ڂ�N1[XXX],K1[XXX]���̂��Ƃ�A�ʍ��ڂ̒l��� <--
					else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%�n, Fit
					{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]�Œ�
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
						if ((0 != strstr(szDispLabel[i], "Fit")) &&
							(1 == g_lXmpType)) {
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- { ---------- */
////							sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f"); //[.##]�Œ�
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ----------              */
//							strcpy(szDecimalPointBuff, "%.3e");
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
#if FIT_DISP_SCOUT == 3
							strcpy(szDecimalPointBuff, "");
#elif FIT_DISP_SCOUT == 2
							strcpy(szDecimalPointBuff, "%.2e");
#elif FIT_DISP_SCOUT == 1
							strcpy(szDecimalPointBuff, "%.3e");
#else
							sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f");
#endif
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
						} else {
							sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]�Œ�
						}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
					}
					else //���̑�
					{
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
					}

					if(strstr(szDispLabel[i], "Thick") != NULL) //���x������Thick*
					{

						//MeasurementData
						strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * g_dUnitConvert[iUnitIndex]);  //
						AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
						//���v�f�[�^
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum * g_dUnitConvert[iUnitIndex]);	//MAX
						AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum * g_dUnitConvert[iUnitIndex]);	//MIN
						AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange * g_dUnitConvert[iUnitIndex]);	//RANGE
						AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage * g_dUnitConvert[iUnitIndex]);	//MEAN
						AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
/* modified 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
//						strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation * g_dUnitConvert[iUnitIndex]); //S.D.
/* modified 2017.04.29 hmenjo �����\���� SD ���O ----------              */
						strBuffer.Format(szDecimalPointBuffThickSD, Statistics[i].dStandardDeviation * g_dUnitConvert[iUnitIndex]); //S.D.
/* modified 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */
						AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
						iCol++;
					}
					else if(strstr(szDispLabel[i], "%") != NULL) //���x������%�n
					{
						//MeasurementData
						strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * 100);	//
						AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
						//���v�f�[�^
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum * 100);  //MAX
						AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum * 100);  //MIN
						AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange * 100);	 //RANGE
						AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage * 100);  //MEAN
						AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation * 100); //S.D.
						AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
						iCol++;
					}
					else //
					{
						//MeasurementData
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
//						strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);  //
//						AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
//						//���v�f�[�^
//						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum);  //MAX
//						AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
//						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum);  //MIN
//						AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
//						strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange);    //RANGE
//						AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
//						strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage);  //MEAN
//						AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
//						strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation); //S.D.
//						AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
						if (0 == strlen(szDecimalPointBuff)) {
							long l_lExp = m_pDoc->GetExpDouble(OnePointData.dData[i]);
							strBuffer.Format("%.2fe%+03d", OnePointData.dData[i] / pow(10, l_lExp), l_lExp);
							AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
							/* ���v�f�[�^	*/
							l_lExp = m_pDoc->GetExpDouble(Statistics[i].dMaximum);
							strBuffer.Format("%.2fe%+03d", Statistics[i].dMaximum / pow(10, l_lExp), l_lExp);
							AddStatisticsListGrid_Data(1, iCol+1, strBuffer );				/* MAX	*/
							l_lExp = m_pDoc->GetExpDouble(Statistics[i].dMinimum);
							strBuffer.Format("%.2fe%+03d", Statistics[i].dMinimum / pow(10, l_lExp), l_lExp);
							AddStatisticsListGrid_Data(2, iCol+1, strBuffer );				/* MIN	*/
							l_lExp = m_pDoc->GetExpDouble(Statistics[i].dRange);
							strBuffer.Format("%.2fe%+03d", Statistics[i].dRange / pow(10, l_lExp), l_lExp);
							AddStatisticsListGrid_Data(3, iCol+1, strBuffer );				/* RANGE	*/
							l_lExp = m_pDoc->GetExpDouble(Statistics[i].dAverage);
							strBuffer.Format("%.2fe%+03d", Statistics[i].dAverage / pow(10, l_lExp), l_lExp);
							AddStatisticsListGrid_Data(4, iCol+1, strBuffer );				/* MEAN	*/
							l_lExp = m_pDoc->GetExpDouble(Statistics[i].dStandardDeviation);
							strBuffer.Format("%.2fe%+03d", Statistics[i].dStandardDeviation / pow(10, l_lExp), l_lExp);
							AddStatisticsListGrid_Data(5, iCol+1, strBuffer );				/* S.D.	*/
						} else {
							strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
							AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
							/* ���v�f�[�^	*/
							strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum);	/* MAX	*/
							AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
							strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum);	/* MIN	*/
							AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
							strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange);		/* RANGE	*/
							AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
							strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage);	/* MEAN	*/
							AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
							strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation);	/* S.D.	*/
							AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
						}
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
						iCol++;
					}
				}
				break;
			case MEAS_PROG_TYPE_SR_DISTANCE:

				//2009.10.28 bagus 2point-Distance --{--
				//2�_�ԋ����̏ꍇ�ɂ͊�Ԃ̂Ƃ��̂ݍX�V
				if(bMeasDistance && iCount % 2 == 1){
					break;
				}
				//2009.10.28 bagus 2point-Distance --}--

				nItemCount = iCount / 2;
				m_pDoc->GetOnePointAndStatistics(&OnePointData, Statistics,iCount); //(1�|�C���g�ځ�1)
				m_pDoc->GetOnePointAndStatistics(&DistanceOnePointData, Statistics, iCount-1); //(1�|�C���g�ځ�1)

				//char szXyzFormat[255+1];
				memset(szXyzFormat, 0, sizeof(szXyzFormat));
				sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
				///// Point Position X (Start)//////
				iIndex = 0;
				iIndex++;
				dPositionX = DistanceOnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
				strBuffer.Format(szXyzFormat, dPositionX);
				AddMeasurementListGrid_Data(nItemCount, iIndex++, strBuffer);

				///// Point Position Y (Start)//////
				dPositionY = DistanceOnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
				strBuffer.Format(szXyzFormat, dPositionY);
				AddMeasurementListGrid_Data(nItemCount, iIndex++, strBuffer);

				///// Point Position X (End)//////
				dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
				strBuffer.Format(szXyzFormat, dPositionX);
				AddMeasurementListGrid_Data(nItemCount, iIndex++, strBuffer);

				///// Point Position Y (End)//////
				dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
				strBuffer.Format(szXyzFormat, dPositionY);
				AddMeasurementListGrid_Data(nItemCount, iIndex++, strBuffer);

				///// Distance /////
				dPositionX = m_pMainFrame->Calc2PointDistance(DistanceOnePointData.xyz.lX,DistanceOnePointData.xyz.lY,OnePointData.xyz.lX,OnePointData.xyz.lY);
				dPositionX = dPositionX * g_dXyzUnitConvertCoefficient[0];
				strBuffer.Format(szXyzFormat, dPositionX);
				AddMeasurementListGrid_Data(nItemCount, iIndex++, strBuffer);

				//2009.11.26 bagus 2point-Distance --{--
				// dx,dy���ڂ̒ǉ�
				//dx
				dPositionX = DistanceOnePointData.xyz.lX - OnePointData.xyz.lX;
				//2009.11.29 bagus 2point-distance --{--
				//dPositionX = dPositionX * g_dXyzUnitConvertCoefficient[0];
				dPositionX = fabs(dPositionX * g_dXyzUnitConvertCoefficient[0]);
				//2009.11.29 bagus 2point-distance --}--
				strBuffer.Format(szXyzFormat, dPositionX);
				AddMeasurementListGrid_Data(nItemCount, iIndex++, strBuffer);

				//dy
				dPositionX = DistanceOnePointData.xyz.lY - OnePointData.xyz.lY;
				//2009.11.29 bagus 2point-distance --{--
				//dPositionX = dPositionX * g_dXyzUnitConvertCoefficient[0];
				dPositionX = fabs(dPositionX * g_dXyzUnitConvertCoefficient[0]);
				//2009.11.29 bagus 2point-distance --}--
				strBuffer.Format(szXyzFormat, dPositionX);
				AddMeasurementListGrid_Data(nItemCount, iIndex++, strBuffer);
				//2009.11.26 bagus 2point-Distance --}--

				///// StatisTics /////
				iCol = 0;
				for(iCol = 0;iCol < 3;iCol++){
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]�Œ�
					strBuffer.Format(szDecimalPointBuff, Statistics[iCol].dMaximum* g_dXyzUnitConvertCoefficient[0]);  //MAX
					AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[iCol].dMinimum* g_dXyzUnitConvertCoefficient[0]);  //MIN
					AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[iCol].dRange* g_dXyzUnitConvertCoefficient[0]);    //RANGE
					AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[iCol].dAverage* g_dXyzUnitConvertCoefficient[0]);  //MEAN
					AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[iCol].dStandardDeviation* g_dXyzUnitConvertCoefficient[0]); //S.D.
					AddStatisticsListGrid_Data(5, iCol+1, strBuffer );

				}
				//	iCol++;

				break;
			}
			//2009.10.28 bagus 2point-Distance
			break;
		case HEAD_TYPE_STRESS:
			//Stress �w�b�h�̃f�[�^�������ꍇ�̏���������
			memset(szXyzFormat, 0, sizeof(szXyzFormat));
			sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
			///// X-Start/X-End/Stress
			iIndex = 0;
			AddMeasurementListGrid_Data(iCount, 1+iIndex++, _T("--"));
			AddMeasurementListGrid_Data(iCount, 1+iIndex++, _T("--"));
			AddMeasurementListGrid_Data(iCount, 1+iIndex++, _T("--"));
			AddMeasurementListGrid_Data(iCount, 1+iIndex++, _T("--"));
			///// Point Position X //////
			iIndex++;
			dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionX);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			///// Point Position Y //////
			dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionY);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			/// ///// Point Position Z //////
			dPositionZ = (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0]) / 10;
			strBuffer.Format(szXyzFormat, dPositionZ);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			//// �I�[�g�t�H�[�J�X���藓 ////
			strBuffer.Format("%s", OnePointData.szShowAfValid);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			//2009.08.30 bagus stress --{--
			iCol = 0;
//			iCol++; 	//STRESS[MPa]
			for(i = 0; i < STRESS_LINES_MAX;i++){
				if(m_rcp_data.StressStageProgInfo.Line[i].bValidLine){
// 2009.09.07 K.Matsuo Delete -->
//					iCol++;
// 2009.09.07 K.Matsuo Delete <--
					nTotalSressLineNum++;
				}
			}
			//2009.08.30 bagus stress --}--
			//for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + 4 + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+4]LS+STRESS, [+3]:XYZ, [+1]:AF
			for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + nTotalSressLineNum + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+4]LS+STRESS, [+3]:XYZ, [+1]:AF
			{
				int iUnitIndex = 0;
				int iUnit = m_pDoc->GetUnitIndex();
				if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
				else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
				if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
					iUnitIndex = 0;
				}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
				{
/* modified 2017.04.15 hmenjo �����\���� ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
/* modified 2017.04.15 hmenjo �����\���� ----------              */
					if (l_iUnitIndexThickInt < 0) {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "f");
					}
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
					sprintf(szDecimalPointBuffThickSD, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */
/* modified 2017.04.15 hmenjo �����\���� ---------- } ---------- */
				}
				else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,K�l�n
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]�Œ�
				}
// 2009.11.12 K.Matsuo ADAP�̕񍐍��ڂ̐���Ή��B2�ڂ�N1[XXX],K1[XXX]���̂��Ƃ�A�ʍ��ڂ̒l��� -->
				else if(strncmp(szDispLabel[i], "Const", strlen("Const")) == 0 ||
						strncmp(szDispLabel[i], "Eg", strlen("Eg")) == 0 ||
						strncmp(szDispLabel[i], "Fraction", strlen("Fraction")) == 0)
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]�Œ�
				}
// 2009.11.12 K.Matsuo ADAP�̕񍐍��ڂ̐���Ή��B2�ڂ�N1[XXX],K1[XXX]���̂��Ƃ�A�ʍ��ڂ̒l��� <--
				else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%�n, Fit
				{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]�Œ�
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
					if ((0 != strstr(szDispLabel[i], "Fit")) &&
						(1 == g_lXmpType)) {
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- { ---------- */
////						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f"); //[.##]�Œ�
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ----------              */
//						strcpy(szDecimalPointBuff, "%.3e");
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
#if FIT_DISP_SCOUT == 3
						strcpy(szDecimalPointBuff, "");
#elif FIT_DISP_SCOUT == 2
						strcpy(szDecimalPointBuff, "%.2e");
#elif FIT_DISP_SCOUT == 1
						strcpy(szDecimalPointBuff, "%.3e");
#else
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f");
#endif
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]�Œ�
					}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
				}
				else //���̑�
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
				}

				if(strstr(szDispLabel[i], "Thick") != NULL) //���x������Thick*
				{

					//MeasurementData
					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * g_dUnitConvert[iUnitIndex]);  //
					//AddMeasurementListGrid_Data(iCount, iCol+nTotalSressLineNum  + 1, strBuffer );
					AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer );
					//���v�f�[�^
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum * g_dUnitConvert[iUnitIndex]);	//MAX
					AddStatisticsListGrid_Data(1, nTotalSressLineNum + iCol+2, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum * g_dUnitConvert[iUnitIndex]);	//MIN
					AddStatisticsListGrid_Data(2, nTotalSressLineNum +iCol+2, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange * g_dUnitConvert[iUnitIndex]);	//RANGE
					AddStatisticsListGrid_Data(3, nTotalSressLineNum +iCol+2, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage * g_dUnitConvert[iUnitIndex]);	//MEAN
					AddStatisticsListGrid_Data(4, nTotalSressLineNum +iCol+2, strBuffer );
/* modified 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation * g_dUnitConvert[iUnitIndex]); //S.D.
/* modified 2017.04.29 hmenjo �����\���� SD ���O ----------              */
					strBuffer.Format(szDecimalPointBuffThickSD, Statistics[i].dStandardDeviation * g_dUnitConvert[iUnitIndex]); //S.D.
/* modified 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */
					AddStatisticsListGrid_Data(5, nTotalSressLineNum +iCol+2, strBuffer );
					iCol++;
				}
				else if(strstr(szDispLabel[i], "%") != NULL) //���x������%�n
				{
					//MeasurementData
					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * 100);	//
					AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer );
					//���v�f�[�^
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum * 100);  //MAX
					AddStatisticsListGrid_Data(1, nTotalSressLineNum +iCol+2, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum * 100);  //MIN
					AddStatisticsListGrid_Data(2, nTotalSressLineNum +iCol+2, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange * 100);	 //RANGE
					AddStatisticsListGrid_Data(3, nTotalSressLineNum +iCol+2, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage * 100);  //MEAN
					AddStatisticsListGrid_Data(4, nTotalSressLineNum +iCol+2, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation * 100); //S.D.
					AddStatisticsListGrid_Data(5, nTotalSressLineNum +iCol+2, strBuffer );
					iCol++;
				}
				else //
				{
					//MeasurementData
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
//					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);  //
//					AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer );
//					//���v�f�[�^
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum);  //MAX
//					AddStatisticsListGrid_Data(1, nTotalSressLineNum + iCol+2, strBuffer );
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum);  //MIN
//					AddStatisticsListGrid_Data(2, nTotalSressLineNum + iCol+2, strBuffer );
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange);    //RANGE
//					AddStatisticsListGrid_Data(3, nTotalSressLineNum + iCol+2, strBuffer );
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage);  //MEAN
//					AddStatisticsListGrid_Data(4, nTotalSressLineNum + iCol+2, strBuffer );
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation); //S.D.
//					AddStatisticsListGrid_Data(5, nTotalSressLineNum + iCol+2, strBuffer );
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
					if (0 == strlen(szDecimalPointBuff)) {
						long l_lExp = m_pDoc->GetExpDouble(OnePointData.dData[i]);
						strBuffer.Format("%.2fe%+03d", OnePointData.dData[i] / pow(10, l_lExp), l_lExp);
						AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer );
						/* ���v�f�[�^	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dMaximum);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dMaximum / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(1, nTotalSressLineNum + iCol+2, strBuffer );	/* MAX	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dMinimum);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dMinimum / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(2, nTotalSressLineNum + iCol+2, strBuffer );	/* MIN	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dRange);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dRange / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(3, nTotalSressLineNum + iCol+2, strBuffer );	/* RANGE	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dAverage);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dAverage / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(4, nTotalSressLineNum + iCol+2, strBuffer );	/* MEAN	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dStandardDeviation);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dStandardDeviation / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(5, nTotalSressLineNum + iCol+2, strBuffer );	/* S.D.	*/
					} else {
						strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
						AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer );
						/* ���v�f�[�^	*/
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum);	/* MAX	*/
						AddStatisticsListGrid_Data(1, nTotalSressLineNum + iCol+2, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum);	/* MIN	*/
						AddStatisticsListGrid_Data(2, nTotalSressLineNum + iCol+2, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange);		/* RANGE	*/
						AddStatisticsListGrid_Data(3, nTotalSressLineNum + iCol+2, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage);	/* MEAN	*/
						AddStatisticsListGrid_Data(4, nTotalSressLineNum + iCol+2, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation);	/* S.D.	*/
						AddStatisticsListGrid_Data(5, nTotalSressLineNum + iCol+2, strBuffer );
					}
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
					iCol++;
				}
			}
			break;
		//2009.09.15 bagus se --{--
		//SR����R�s�[
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
			//char szXyzFormat[255+1];
			memset(szXyzFormat, 0, sizeof(szXyzFormat));
			sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
			///// Point Position X //////
			iIndex = 0;
			iIndex++;
			dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionX);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			///// Point Position Y //////
			dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionY);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			/// ///// Point Position Z //////
			dPositionZ = (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0]) / 10;
			strBuffer.Format(szXyzFormat, dPositionZ);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);


			//// �I�[�g�t�H�[�J�X���藓 ////
			ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
			if(m_SrConfig.bAutoFocus)
			{
				strBuffer.Format("%s", OnePointData.szShowAfValid);
				AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);
			}

			iCol = 0;
			for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+3]:XYZ, [+1]:AF
			{
				int iUnitIndex = 0;
				int iUnit = m_pDoc->GetUnitIndex();
				if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
				else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
				if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
					iUnitIndex = 0;
				}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

				char szDecimalPointBuff[255+1];
				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
				}
				else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,K�l�n
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]�Œ�
				}
// 2009.11.12 K.Matsuo ADAP�̕񍐍��ڂ̐���Ή��B2�ڂ�N1[XXX],K1[XXX]���̂��Ƃ�A�ʍ��ڂ̒l��� -->
				else if(strncmp(szDispLabel[i], "Const", strlen("Const")) == 0 ||
						strncmp(szDispLabel[i], "Eg", strlen("Eg")) == 0 ||
						strncmp(szDispLabel[i], "Fraction", strlen("Fraction")) == 0)
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[4], "f"); //[.####]�Œ�
				}
// 2009.11.12 K.Matsuo ADAP�̕񍐍��ڂ̐���Ή��B2�ڂ�N1[XXX],K1[XXX]���̂��Ƃ�A�ʍ��ڂ̒l��� <--
				else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%�n, Fit
				{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]�Œ�
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
					if ((0 != strstr(szDispLabel[i], "Fit")) &&
						(1 == g_lXmpType)) {
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- { ---------- */
////						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f"); //[.##]�Œ�
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ----------              */
//						strcpy(szDecimalPointBuff, "%.3e");
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
#if FIT_DISP_SCOUT == 3
						strcpy(szDecimalPointBuff, "");
#elif FIT_DISP_SCOUT == 2
						strcpy(szDecimalPointBuff, "%.2e");
#elif FIT_DISP_SCOUT == 1
						strcpy(szDecimalPointBuff, "%.3e");
#else
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[6], "f"); //[.##]�Œ�
#endif
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
					} else {
						sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]�Œ�
					}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
				}
				else //���̑�
				{
					sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "f"); //�I��Unit�ŕϓ�
				}

				if(strstr(szDispLabel[i], "Thick") != NULL) //���x������Thick*
				{

					//MeasurementData
					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * g_dUnitConvert[iUnitIndex]);  //
					AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
					//���v�f�[�^
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum * g_dUnitConvert[iUnitIndex]);	//MAX
					AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum * g_dUnitConvert[iUnitIndex]);	//MIN
					AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange * g_dUnitConvert[iUnitIndex]);	//RANGE
					AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage * g_dUnitConvert[iUnitIndex]);	//MEAN
					AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation * g_dUnitConvert[iUnitIndex]); //S.D.
					AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
					iCol++;
				}
				else if(strstr(szDispLabel[i], "%") != NULL) //���x������%�n
				{
					//MeasurementData
					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i] * 100);	//
					AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
					//���v�f�[�^
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum * 100);  //MAX
					AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum * 100);  //MIN
					AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange * 100);	 //RANGE
					AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage * 100);  //MEAN
					AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation * 100); //S.D.
					AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
					iCol++;
				}
				else //
				{
					//MeasurementData
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
//					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);  //
//					AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
//					//���v�f�[�^
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum);  //MAX
//					AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum);  //MIN
//					AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange);    //RANGE
//					AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage);  //MEAN
//					AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
//					strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation); //S.D.
//					AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
					if (0 == strlen(szDecimalPointBuff)) {
						long l_lExp = m_pDoc->GetExpDouble(OnePointData.dData[i]);
						strBuffer.Format("%.2fe%+03d", OnePointData.dData[i] / pow(10, l_lExp), l_lExp);
						AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
						/* ���v�f�[�^	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dMaximum);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dMaximum / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(1, iCol+1, strBuffer);				/* MAX	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dMinimum);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dMinimum / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(2, iCol+1, strBuffer);				/* MIN	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dRange);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dRange / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(3, iCol+1, strBuffer);				/* RANGE	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dAverage);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dAverage / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(4, iCol+1, strBuffer);				/* MEAN	*/
						l_lExp = m_pDoc->GetExpDouble(Statistics[i].dStandardDeviation);
						strBuffer.Format("%.2fe%+03d", Statistics[i].dStandardDeviation / pow(10, l_lExp), l_lExp);
						AddStatisticsListGrid_Data(5, iCol+1, strBuffer);				/* S.D.	*/
					} else {
						strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);
						AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
						/* ���v�f�[�^	*/
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMaximum);	/* MAX	*/
						AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dMinimum);	/* MIN	*/
						AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange);		/* RANGE	*/
						AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dAverage);	/* MEAN	*/
						AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
						strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation);	/* S.D.	*/
						AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
					}
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
					iCol++;
				}
			}
			break;
		//2009.09.15 bagus --}--
// 2009.10.19 bagus MS �ǉ� --{--
//		case HEAD_TYPE_IRSE:
		case HEAD_TYPE_MS:
			memset(szXyzFormat, 0, sizeof(szXyzFormat));
			sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
			///// Point Position X //////
			iIndex = 0;
			iIndex++;
			dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionX);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			///// Point Position Y //////
			dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionY);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);
			break;
// 2009.10.19 bagus MS �ǉ� --}--
// 2009.11.12 K.Matsuo Resist �w���\���ɏC�� -->
		case HEAD_TYPE_4PP:
			memset(szXyzFormat, 0, sizeof(szXyzFormat));
			sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
			///// Point Position X //////
			iIndex = 0;
			iIndex++;
			dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionX);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			///// Point Position Y //////
			dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionY);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			/// ///// Point Position Z //////
			dPositionZ = (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0]) / 10;
			strBuffer.Format(szXyzFormat, dPositionZ);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);


// 2009.12.17 bagus RS --{--
#if 0
			//// �I�[�g�t�H�[�J�X���藓 ////
			ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
			if(m_SrConfig.bAutoFocus)
			{
				strBuffer.Format("%s", OnePointData.szShowAfValid);
				AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);
			}
#endif
// 2009.12.17 bagus RS --}--

			iCol = 0;
			char szBuff[255+1];
			char szDecimalPointBuff[255+1];
			sprintf(szDecimalPointBuff, "%s%s%s", "%", g_lpszDecimalPoint[2], "f"); //[.##]�Œ�

			bResitValidData = FALSE;
			//2009.11.17 RS --{--
			// Over Range, Over Load,�Ȃǒǉ�
			// Over Range / Over load �̏ꍇ�ɂ̓f�[�^�͕\�����Ȃ�
			switch(ResistOnePointData.chDataValid){
			case _T('N'):
				bResitValidData = TRUE;
				strBuffer.Format(_T("%s"),_T("VALID"));
				break;
			case _T('O'):
				bResitValidData = FALSE;
				strBuffer.Format(_T("%s"),_T("OV.RG"));
				break;
			case _T('L'):
				bResitValidData = FALSE;
				strBuffer.Format(_T("%s"),_T("OV.LD"));
				break;
			default:
				bResitValidData = FALSE;
				strBuffer.Format(_T("%s"),_T("UNKNOWN"));
				break;
			}
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);
			for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+3]:XYZ, [+1]:AF
			{
				//MeasurementData
				switch(i){
				case 0: strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szResistance : _T(""));		break;
				case 1: strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szSurfaceResistivity : _T(""));	break;
				case 2: strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szVolumeResistivity : _T(""));	break;
				case 3: strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szConductivity : _T(""));		break;
				//2009.11.17 RS --}--
				}
				AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
				//���v�f�[�^
				ConvertToExpNumber(Statistics[i].dMaximum, szBuff);  //MAX
				AddStatisticsListGrid_Data(1, iCol+1, szBuff);
				ConvertToExpNumber(Statistics[i].dMinimum, szBuff);  //MIN
				AddStatisticsListGrid_Data(2, iCol+1, szBuff);
				strBuffer.Format(szDecimalPointBuff, Statistics[i].dRange);    //RANGE
				AddStatisticsListGrid_Data(3, iCol+1, strBuffer);
				ConvertToExpNumber(Statistics[i].dAverage, szBuff);  //MEAN
				AddStatisticsListGrid_Data(4, iCol+1, szBuff);
				strBuffer.Format(szDecimalPointBuff, Statistics[i].dStandardDeviation); //S.D.
				AddStatisticsListGrid_Data(5, iCol+1, strBuffer);
				iCol++;

			}
			break;
// 2009.11.12 K.Matsuo Resist �w���\���ɏC�� <--
		case HEAD_TYPE_CTA:
		//2009.11.04 bagus CA --{--
		//SE����R�s�[
			//char szXyzFormat[255+1];
			memset(szXyzFormat, 0, sizeof(szXyzFormat));
			sprintf(szXyzFormat, "%s%s%s", "%", g_lpszXyzDecimalPoint[0], "lf");
			///// Point Position X //////
			iIndex = 0;
			iIndex++;
			dPositionX = OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionX);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			///// Point Position Y //////
			dPositionY = OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0];
			strBuffer.Format(szXyzFormat, dPositionY);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);

			/// ///// Point Position Z //////
			dPositionZ = (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0]) / 10;
			strBuffer.Format(szXyzFormat, dPositionZ);
			AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);


// 2009.12.17 bagus CA --{--
#if 0
			//// �I�[�g�t�H�[�J�X���藓 ////
			ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
			if(m_SrConfig.bAutoFocus)
			{
				strBuffer.Format("%s", OnePointData.szShowAfValid);
				AddMeasurementListGrid_Data(iCount, iIndex++, strBuffer);
			}
#endif
// 2009.12.17 bagus CA --}--

			iCol = 0;
			for(i = 0; ( (i < iDispLabelCount) && (iDispLabelCount + 3 + 1 < GRID_COL_MAX) ); i++ ) //[+3]:XYZ, [+1]:AF
			{

				//2009.11.11 CA --{--
				//�\�������̒���
				char szDecimalPointBuff[255+1];
				char szDecimalPointBuffStat[255+1];
				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				memset(szDecimalPointBuffStat, 0, sizeof(szDecimalPointBuffStat));
				{
					switch(i){
					case 0: strncpy(szDecimalPointBuff,"%3.1lf",255);strncpy(szDecimalPointBuffStat,"%3.2lf",255); break;
					case 1: strncpy(szDecimalPointBuff,"%4.1lf",255);strncpy(szDecimalPointBuffStat,"%4.2lf",255); break;
					case 2: strncpy(szDecimalPointBuff,"%2.2lf",255);strncpy(szDecimalPointBuffStat,"%2.3lf",255); break;
					default: strncpy(szDecimalPointBuff,"%.3lf",255);strncpy(szDecimalPointBuffStat,"%.3lf",255); break;
					}

					//MeasurementData
					strBuffer.Format(szDecimalPointBuff, OnePointData.dData[i]);  //
					AddMeasurementListGrid_Data(iCount, iCol+iIndex, strBuffer );
					//���v�f�[�^
					strBuffer.Format(szDecimalPointBuffStat, Statistics[i].dMaximum);  //MAX
					AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuffStat, Statistics[i].dMinimum);  //MIN
					AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuffStat, Statistics[i].dRange);    //RANGE
					AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuffStat, Statistics[i].dAverage);  //MEAN
					AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
					strBuffer.Format(szDecimalPointBuffStat, Statistics[i].dStandardDeviation); //S.D.
					AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
					iCol++;
				}
				//2009.11.11 CA --}--
			}
			break;
		//2009.11.04 bagus CA --}--
		default:
			break;
		}
		//2009.07.27 bagus stress SR�ȊO�̃w�b�h�ɂ��Ή� --}--


		//2010.01.01.15 bagus 2point-Distance --{--
		//2�_�ԋ����͔���
		int iDiv = 1;
		if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			iDiv = 2;
		}

		if(m_pDoc->GetSeqMeasMode() == SEQ_ON_NORMALMEASURE) //�ʏ푪�蒆
		{
			strBuffer.Format("%5d / %5d ", iPoint / iDiv, iMeasureNum / iDiv);	//�����萔(����)���������ꍇ�ɂ��Ή����邱�ƁI
		}
		else if(m_pDoc->GetSeqMeasMode() == SEQ_ON_REMEASURE) //�����W���[��
		{
			strBuffer.Format("%5d / %5d ", m_pDoc->GetReMeasurePointCnt() / iDiv/*iPoint*/, m_pDoc->GetReMeasNumScans() / iDiv/*iMeasureNum*/);
		}
		else //���蒆�łȂ�
		{
			strBuffer.Format("%5d / %5d ", m_pDoc->GetStoreCount() / iDiv/*iPoint*/, iMeasureNum / iDiv);
		}
		//2010.01.01.15 bagus 2point-Distance --}--

		SetDlgItemText(IDC_MEASUREMENT_POINT, strBuffer);

/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
		if (0 < m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSetNoGet()) {
			if (iCount == m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSetNoGet()) {
				l_wIncludeColorSetNo = iCount;
			}
		}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
	}

	iShowCompletionPointList = iCurrentSetPoint; //��ʕ\���ςݐ����X�V
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
//	m_pDoc->SetShowCompPointList(iShowCompletionPointList);
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ----------              */
	WORD l_wShowCompletionPointList = 0;
	m_pMainFrame->m_pMeaTabViewObj->MRCS_ShowCompletionPointSnap(&l_wShowCompletionPointList, 0);
	if (0 == l_wShowCompletionPointList) {
		m_pDoc->SetShowCompPointList(iShowCompletionPointList);
	} else {
		/* �\���ς݃|�C���g�̋L���݂�(�܂�đ���)	*/
		m_pDoc->SetShowCompPointList(l_wShowCompletionPointList);
		/* ����ԍ����߂��܂��D	*/
		if (l_wShowCompletionPointList == m_pDoc->GetShowCompPointGraph()) {
			m_pDoc->SetCurrentSetPoint(l_wShowCompletionPointList);
		}
	}
	if (0 < l_wIncludeColorSetNo) {
		/* �w��s�F�ԍ��݂�	*/
		m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSet(l_wIncludeColorSetNo, TRUE, FALSE);
	}
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */


	if(m_bCellSelectedFlg == FALSE) //�I���s�Ȃ�
	{
		//�ʉ�ʎ��́A�ǉ��s�Ɉړ����Ȃ��悤����(�\�t�g�������Ă��܂���)
		CWnd* pFocusWnd = GetFocus();
		if(pFocusWnd == NULL)
		{
			return 2;
		}
		//2009.09.14 bagus stress --{--
		if(iShowCompletionPointList < m_MeasurementListGrid.GetRowCount()){
/* modified 2014.10.28 hmenjo ���茋�� Auto Scroll ---------- { ---------- */
//			m_MeasurementListGrid.EnsureVisible(iShowCompletionPointList, TRUE);
/* modified 2014.10.28 hmenjo ���茋�� Auto Scroll ----------              */
			if (
				(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) &&
				(m_pDoc->GetMeasMode() != MEASMODE_TEST)
				) {
				m_MeasurementListGrid.EnsureVisible(iShowCompletionPointList, TRUE);
			}
/* modified 2014.10.28 hmenjo ���茋�� Auto Scroll ---------- } ---------- */
		}
		//2009.09.14 bagus stress --{--
	}

	return 1;
}


//2009.08.25 bagus stress --{--
int CMeasurementListDlg::SetMeasurementLSData_One(int nStressLine,int nRow,STRESSRESULT *pOneData)
{
	STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;
	CString strBuffer;
	int nSection;
	int nValidCount = 0;

	if(nStressLine < 1 || nStressLine > STRESS_LINES_MAX){
		return 0;
	}

	//���V�s�Ƃ��ėL���ȃ��C���łȂ���Ε\�����Ȃ�
	if(!stressStage.Line[nStressLine-1].bValidLine){
		return 0;
	}

	for(nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
		//�L���łȂ��Z�N�V�����͂Ȃɂ����Ȃ�
		if(!stressStage.Line[nStressLine-1].bScanValid[nSection]){
			continue;
		}
		//LmSn
		strBuffer.Format("L%dS%d",nStressLine,nSection+1);
		AddMeasurementListGrid_Data(nRow + nValidCount, 0, strBuffer);
		//X-Start[mm]
		//2009.09.09 bagus stres --{--
		//um -> mm
		//strBuffer.Format("%ld",stressStage.Line[nStressLine-1].SectPos[nSection].lScanStartPosX);
		strBuffer.Format("%.3lf",stressStage.Line[nStressLine-1].SectPos[nSection].lScanStartPosX / (double)MICROMETRE);
		AddMeasurementListGrid_Data(nRow + nValidCount, 1, strBuffer);
		//X-End[mm]
		//strBuffer.Format("%ld",stressStage.Line[nStressLine-1].SectPos[nSection].lScanEndPosX);
		strBuffer.Format("%.3lf",stressStage.Line[nStressLine-1].SectPos[nSection].lScanEndPosX / (double)MICROMETRE);
		AddMeasurementListGrid_Data(nRow + nValidCount, 2, strBuffer);
		//Y[mm]
		//strBuffer.Format("%ld",stressStage.Line[nStressLine-1].SectPos[nSection].lScanPosY);
		strBuffer.Format("%.3lf",stressStage.Line[nStressLine-1].SectPos[nSection].lScanPosY / (double)MICROMETRE);
		AddMeasurementListGrid_Data(nRow + nValidCount, 3, strBuffer);
		//2009.09.09 bagus stres --}--
		//Stress
		strBuffer.Format("%.3lf",pOneData->dStressSection[nSection]);
		AddMeasurementListGrid_Data(nRow + nValidCount, 4, strBuffer);
		//
		strBuffer.Format("---");
		for(int nRestCol = 5;nRestCol < m_MeasurementListGrid.GetColumnCount();nRestCol++){
			AddMeasurementListGrid_Data(nRow + nValidCount, nRestCol, strBuffer);
		}
		nValidCount++;
	}
	return nValidCount;
}
//2009.08.25 bagus stress --}--
//2009.08.22 bagus stress --{--
// LS�f�[�^��List�O���b�h�փZ�b�g����ׂ̊֐�
int CMeasurementListDlg::SetMeasurementLSData(int nStartRow)
{
	int nShowStressData = 0;
	STRESSRESULT tResult;
	int nShowRowNum;
	int nTotalNum;
	int nProcessedNum;
	CString strBuffer;
	STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;
	STATISTICS	Statistics;
	int iCol = 0;
	int nShowCompLine = 0;

	nShowRowNum = 0;
	nTotalNum = 0;
	nProcessedNum = 0;

	if(m_pDoc->GetCurrentMeasureStressLine() <= m_pDoc->GetShowCompStressList()){
		return 0;
	}

	m_PointGraph.MeasureSectionByLSNo(m_pDoc->GetCurrentMeasureStressLine());
	memset(&Statistics,NULL,sizeof(Statistics));
	//�S�̓��v���̃O���b�h�\��
	m_pDoc->GetStressStatisticsData(&Statistics);
	//���v�f�[�^
	char szDecimalPointBuff[1024+1];
	strcpy(szDecimalPointBuff,"%.3lf");
	strBuffer.Format(szDecimalPointBuff, Statistics.dMaximum);	//MAX
	AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
	strBuffer.Format(szDecimalPointBuff, Statistics.dMinimum);	//MIN
	AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
	strBuffer.Format(szDecimalPointBuff, Statistics.dRange);	//RANGE
	AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
	strBuffer.Format(szDecimalPointBuff, Statistics.dAverage);	//MEAN
	AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
	strBuffer.Format(szDecimalPointBuff, Statistics.dStandardDeviation); //S.D.
	AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
	iCol++;
	//
	for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
		if(m_pDoc->GetOneStressLineDataValid(nLine+1)){
			m_PointGraph.FinishSectionByLSNo(nLine+1);
			m_pDoc->GetOneStressLineData(&tResult,nLine+1);
			nShowRowNum += SetMeasurementLSData_One(nLine+1,nShowRowNum+nStartRow+1,&tResult);
			m_pDoc->GetStressLineStatisticsData(nLine+1,&Statistics);
			//���v�f�[�^
			strBuffer.Format(szDecimalPointBuff, Statistics.dMaximum);	//MAX
			AddStatisticsListGrid_Data(1, iCol+1, strBuffer );
			strBuffer.Format(szDecimalPointBuff, Statistics.dMinimum);	//MIN
			AddStatisticsListGrid_Data(2, iCol+1, strBuffer );
			strBuffer.Format(szDecimalPointBuff, Statistics.dRange);	//RANGE
			AddStatisticsListGrid_Data(3, iCol+1, strBuffer );
			strBuffer.Format(szDecimalPointBuff, Statistics.dAverage);	//MEAN
			AddStatisticsListGrid_Data(4, iCol+1, strBuffer );
			strBuffer.Format(szDecimalPointBuff, Statistics.dStandardDeviation); //S.D.
			AddStatisticsListGrid_Data(5, iCol+1, strBuffer );
			iCol++;
			nShowCompLine = nLine+1;
		}else if(m_pDoc->GetCurrentMeasureStressLine() == nLine+1){
			m_PointGraph.MeasureSectionByLSNo(nLine+1);
		}
		if(stressStage.Line[nLine].bValidLine){
			for(int nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
				if(stressStage.Line[nLine].bScanValid[nSection]){
					nTotalNum++;
				}
			}
		}

	}
	nProcessedNum = nShowRowNum;
	//m_PointGraph.FinishSection(nShowRowNum);
	strBuffer.Format("%5d / %5d",nProcessedNum,nTotalNum);
	//2009.12.03 bagus --{--
	//SetDlgItemText(IDC_MEASUREMENT_LS,strBuffer);
	CString strTmp;
	GetDlgItemText(IDC_MEASUREMENT_LS,strTmp);
	if(strTmp != strBuffer){
		SetDlgItemText(IDC_MEASUREMENT_LS,strBuffer);
	}
	//2009.12.03 bagus --}--
	m_pDoc->SetShowCompStressList(nShowCompLine);

	return 1;
}
//2009.08.22 bagus stress --}--


// =========================================================================
// MeasurementListGrid_InitGrid �O���b�h�̐F�E���E�T�C�Y�E�X�^�C���t�H�[�}�b�g�E�t�H���g��ݒ肷��
void CMeasurementListDlg::MeasurementListGrid_InitGrid()
{

	int iDispCol = 0; //�L��Col��
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // �w�b�h�^�C�v

	//2009.08.21 bagus stress --{--
	//�����I�Ȃ��Ƃ���������Switch�ŕ���
	//iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
	switch(wHeadType){
	case HEAD_TYPE_SR:
		//2009.10.28 bagus 2point-Distance --{--
		if(m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			//2�_�ԋ���
			//2009.11.26 bagus 2point-distance --{--
			//dx,dy���ڂ̒ǉ�
			//iDispCol = m_pDoc->GetDispLabelCount() + 4; //Start X/Y, End X/Y
			iDispCol = m_pDoc->GetDispLabelCount() + 4 + 2; //Start X/Y, End X/Y
			//2009.11.26 bagus 2point-distance --}--
		}else{
			//�ʏ푪��
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
		}
		//2009.10.28 bagus 2point-Distance --{--
		break;
	//2009.09.15 bagus se --{--
	case HEAD_TYPE_SE:
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
		break;
	//2009.09.15 bagus se --}--
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case HEAD_TYPE_COMPEASE:
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
		break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	case HEAD_TYPE_STRESS:
		iDispCol = m_pDoc->GetDispLabelCount() + 4 + 3 + 1; //4: LS+STRESS, 3:XYZ, 1:AF
		//iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1 + 1; //4: LS+STRESS, 3:XYZ, 1:AF
		break;
	//2009.11.04 bagus CA --{--
	case HEAD_TYPE_CTA:
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
		break;
	//2009.11.04 bagus CA --}--
	//2009.11.04 bagus RS --{--
	case HEAD_TYPE_4PP:
		//2009.11.17 bagus RS --{--
		//iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1 + 1; //3:XYZ, 1:AF 1:DATA
		//2009.11.17 bagus RS --}--
		break;
	//2009.11.04 bagus RS --}--
	//2009.11.07 bagus MS --{--
		//�����������X,Y�̂�
		iDispCol = 2;
		break;
	//2009.11.07 bagus MS --{--
	default:
		iDispCol = m_pDoc->GetDispLabelCount() + 3 + 1; //3:XYZ, 1:AF
		break;
	}
	//2009.08.21 bagus stress --}--

	if(iDispCol < MEASUREMENTGRIDCOLMIN)
	{
		iDispCol = MEASUREMENTGRIDCOLMIN;
	}

	int Rows = MEASUREMENTGRIDROWMAX + 1;
	int Cols = iDispCol + 1;
	int FixRows = 1;
	int FixCols = 1;

	///// Grid Initialize /////
	m_MeasurementListGrid.SetEditable(FALSE);
	m_MeasurementListGrid.EnableDragAndDrop(TRUE);
	m_MeasurementListGrid.SetTextBkColor(WHITE_COLOR);
	m_MeasurementListGrid.SetRowResize(0);
// 2013.11.13 Bagus Add (TohoSpec�Ή�) -->
	m_MeasurementListGrid.SetColumnResize(0);
// 2013.11.13 Bagus Add (TohoSpec�Ή�) <--

	m_MeasurementListGrid.EnableSelection(TRUE);
	m_MeasurementListGrid.SetTrackFocusCell(FALSE);
	m_MeasurementListGrid.SetFrameFocusCell(FALSE);
	m_MeasurementListGrid.SetFixedColumnSelection(FALSE);
	m_MeasurementListGrid.SetFixedRowSelection(TRUE);
	m_MeasurementListGrid.SetListMode(TRUE);

	TRY{
		m_MeasurementListGrid.SetRowCount(Rows);
		m_MeasurementListGrid.SetColumnCount(Cols);
		m_MeasurementListGrid.SetFixedRowCount(FixRows);
		m_MeasurementListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	///// Grid Size Initialize /////
	// Height
	int iRow = 0;
	m_MeasurementListGrid.SetRowHeight(iRow++, 36); 		// Row 0
	while ( iRow < m_MeasurementListGrid.GetRowCount() )	// Row 1 �`
		m_MeasurementListGrid.SetRowHeight(iRow++, 18);

	// Width
	int iCol = 0;
	m_MeasurementListGrid.SetColumnWidth(iCol++, 40);		// Col 0
	while ( iCol < m_MeasurementListGrid.GetColumnCount() ) // Col 1 �`
		m_MeasurementListGrid.SetColumnWidth(iCol++, 68);
// 2013.11.13 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if(m_SrConfig.bAutoFocus){
			for (iCol = 1 ; iCol < 5; iCol++)	// XYZ, AF
			{
				m_MeasurementListGrid.SetColumnWidth(iCol, 0);
			}
		}
		else{
			for (iCol = 1 ; iCol < 4; iCol++)	// XYZ
			{
				m_MeasurementListGrid.SetColumnWidth(iCol, 0);
			}
		}
	}
// 2013.11.13 Bagus Add (TohoSpec�Ή�) <--

	///// Grid Format /////
	///// Font Initialize /////
	DWORD dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

	for ( iRow = 0; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ ) {

		if(iRow == 0)
		{
			m_logFont.lfHeight = 12;
		}
		else
		{
			m_logFont.lfHeight = 11;
		}

		for ( iCol = 0; iCol < m_MeasurementListGrid.GetColumnCount(); iCol++ ) {
			m_MeasurementListGrid.SetItemFont(iRow, iCol, &m_logFont);
			m_MeasurementListGrid.SetItemFormat(iRow, iCol, dwTextStyle);
		}
	}
}

// =========================================================================
// MeasurementListGrid_HeaderSet �s�ԍ��A�񖼁i�X�L�����|�W�V�����Ƒ��薈�̍��ږ��j��ݒ肷��
void CMeasurementListDlg::MeasurementListGrid_HeaderSet()
{
	int i;
	char szXyz[255+1];
	memset(szXyz, 0, sizeof(szXyz));

	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // �w�b�h�^�C�v
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];

	//2009.07.25 bagus stress �X�g���X�@�\�ǉ� --{--
	// if ( wHeadType == HEAD_TYPE_SR ) {
	int iCol = 0;
	int iRow = 0;
	switch( wHeadType ){
	case HEAD_TYPE_SR:
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		//2009.10.28 bagus 2point-Distance --{--
		switch(wScanType){
		default:
			///// Title Column /////
			for  (iCol = 0; iCol < MEAS_LIST_POSITION_MAX; iCol++ ) 			   // �X�L�����|�W�V����
			{
				if(iCol == 0)
				{
					sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]��mm
				}
				else
				{
					sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
				}
				m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
		//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
			}
			//// �I�[�g�t�H�[�J�X���藓 ////
			ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
			if(m_SrConfig.bAutoFocus){
				m_MeasurementListGrid.SetItemText(0, iCol++, MEAS_LIST_AF_CHECK_ITEM[0]);
			}
			m_pDoc->GetDispLabel(szDispLabel);
			for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
			{
				m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel[i]);
				iCol++;
			}
			break;
		case MEAS_PROG_TYPE_SR_DISTANCE:
			///// Title Column /////
			sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[0]); //g_lpszXyzUnit[0]��mm
			m_MeasurementListGrid.SetItemText(0, 0, szXyz);
			for  (iCol = 0; iCol < 4; iCol++ )				  // �X�L�����|�W�V����
			{
				//2009.11.26 bagus 2point-distance --{--
				//sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol % 2 + 1], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
				sprintf(szXyz, "%s%s", MEAS_LIST_DISTANCE_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
				//2009.11.26 bagus 2point-distance --}--
				m_MeasurementListGrid.SetItemText(0, iCol + 1, szXyz);
		//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
			}
			m_pDoc->GetDispLabel(szDispLabel);
			for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
			{
				m_MeasurementListGrid.SetItemText(0, iCol + 1, szDispLabel[i]);
				iCol++;
			}
			break;
		}
		//2009.10.28 bagus 2point-Distance --}--
		break;
	case HEAD_TYPE_STRESS:
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		///// Title Column /////
		for  (iCol = 0; iCol < MEAS_LIST_STRESS_POSITION_MAX; iCol++ )				  // �X�L�����|�W�V����
		{
			if(iCol == 0)
			{
				sprintf(szXyz, "%s", MEAS_LIST_STRESS_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]��mm
			}
			else if(iCol > 4)
			{
				sprintf(szXyz, "%s%s", MEAS_LIST_STRESS_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
			}
			else {
				sprintf(szXyz, "%s", MEAS_LIST_STRESS_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]��mm
			}
			m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
	//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
		}
		//// �I�[�g�t�H�[�J�X���藓 ////
		m_MeasurementListGrid.SetItemText(0, iCol++, MEAS_LIST_AF_CHECK_ITEM[0]);


		char szDispLabel2[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
		m_pDoc->GetDispLabel(szDispLabel2);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel2[i]);
			iCol++;
		}
		break;
	//2009.09.15 bagus se --{--
	//SR����R�s�[
	case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		///// Title Column /////
		for  (iCol = 0; iCol < MEAS_LIST_POSITION_MAX; iCol++ ) 			   // �X�L�����|�W�V����
		{
			if(iCol == 0)
			{
				sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]��mm
			}
			else
			{
				sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
			}
			m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
	//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
		}
		//// �I�[�g�t�H�[�J�X���藓 ////
		ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
		if(m_SrConfig.bAutoFocus){
			m_MeasurementListGrid.SetItemText(0, iCol++, MEAS_LIST_AF_CHECK_ITEM[0]);
		}


		m_pDoc->GetDispLabel(szDispLabel);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel[i]);
			iCol++;
		}
		break;
	//2009.09.15 bagus se --}--
// 2009.10.19 bagus MS �ǉ� --{--
//	case HEAD_TYPE_IRSE:
//	case HEAD_TYPE_MS:
// 2009.10.19 bagus MS �ǉ� --}--
	//2009.11.04 bagus CA --{--
	//SE����R�s�[
	case HEAD_TYPE_CTA:
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		///// Title Column /////
		for  (iCol = 0; iCol < MEAS_LIST_POSITION_MAX; iCol++ ) 			   // �X�L�����|�W�V����
		{
			if(iCol == 0)
			{
				sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]��mm
			}
			else
			{
				sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
			}
			m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
	//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
		}
// 2009.12.17 bagus CA --{--
#if 0
		//// �I�[�g�t�H�[�J�X���藓 ////
		ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
		if(m_SrConfig.bAutoFocus){
			m_MeasurementListGrid.SetItemText(0, iCol++, MEAS_LIST_AF_CHECK_ITEM[0]);
		}
#endif
// 2009.12.17 bagus CA --}--

		m_pDoc->GetDispLabel(szDispLabel);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel[i]);
			iCol++;
		}
		break;
	//2009.11.04 bagus CA --}--
	//2009.11.04 bagus RS --{--
	//SE����R�s�[
	case HEAD_TYPE_4PP:
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		///// Title Column /////
		for  (iCol = 0; iCol < MEAS_LIST_POSITION_MAX; iCol++ ) 			   // �X�L�����|�W�V����
		{
			if(iCol == 0)
			{
				sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]��mm
			}
			else
			{
				sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
			}
			m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
	//		m_MeasurementListGrid.SetItemText(0, iCol, MEAS_LIST_POSITION_ITEM[iCol]);
		}
// 2009.12.17 bagus RS --{--
#if 0
		//// �I�[�g�t�H�[�J�X���藓 ////
		ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
		if(m_SrConfig.bAutoFocus){
			m_MeasurementListGrid.SetItemText(0, iCol++, MEAS_LIST_AF_CHECK_ITEM[0]);
		}
#endif
// 2009.12.17 bagus RS --}--

		//2009.11.17 bagus RS --{--
		m_MeasurementListGrid.SetItemText(0, iCol++, "DATA");
		//2009.11.17 bagus RS --}--

		m_pDoc->GetDispLabel(szDispLabel);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_MeasurementListGrid.SetItemText(0, iCol, szDispLabel[i]);
			iCol++;
		}
		break;
	//2009.11.04 bagus RS --}--
	//2009.11.07 bagus MS --{--
	case HEAD_TYPE_MS:
		///// Title Row /////
		for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ )
			m_MeasurementListGrid.SetItemTextFmt(iRow, 0, "%d", iRow);

		///// Title Column /////
		for  (iCol = 0; iCol < MEAS_LIST_POSITION_MAX - 1; iCol++ ) 			   // �X�L�����|�W�V����
		{
			if(iCol == 0)
			{
				sprintf(szXyz, "%s", MEAS_LIST_POSITION_ITEM[iCol]); //g_lpszXyzUnit[0]��mm
			}
			else
			{
				sprintf(szXyz, "%s%s", MEAS_LIST_POSITION_ITEM[iCol], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
			}
			m_MeasurementListGrid.SetItemText(0, iCol, szXyz);
		}
		break;
	//2009.11.07 bagus MS --}--
	default:
		break;
	}
	//2009.07.25 bagus stress �X�g���X�@�\�ǉ� --}--
}

// =========================================================================
// AddMeasurementListGrid_Data �w��ʒu�i�s�E��j�ɕ������ݒ肷��
void CMeasurementListDlg::AddMeasurementListGrid_Data(int iRow, int iCol, LPCTSTR psz)
{
	DWORD dwTextStyle;

// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
// 	//2009.09.10 bagus stress --{--
// 	if(iCol >= m_MeasurementListGrid.GetColumnCount()){
// 		return;
// 	}
// 	//2009.09.10 bagus stress --}--
	if(iCol >= m_MeasurementListGrid.GetColumnCount()){
		m_MeasurementListGrid.SetColumnCount(iCol + 1);
	}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--

	if (iRow==0)
		dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	else{
		//dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
		dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	}
//2009.08.25 bagus stress --{--
//2�ȏ��щz�����ꍇ���l��
//	if(iRow>m_MeasurementListGrid.GetRowCount()-1){
//		DWORD dwTextStyleColIndex = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
//		CString strBuffer;
//		strBuffer.Format("%d", m_MeasurementListGrid.GetRowCount());
//		m_MeasurementListGrid.InsertRow(strBuffer);
//		m_MeasurementListGrid.SetItemFormat(iRow, 0, dwTextStyleColIndex);
//	}
		while(m_MeasurementListGrid.GetRowCount() <= iRow){
			DWORD dwTextStyleColIndex = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
			CString strBuffer;
			strBuffer.Format("%d", m_MeasurementListGrid.GetRowCount());
			m_MeasurementListGrid.InsertRow(strBuffer);
			//m_MeasurementListGrid.SetItemFormat(iRow, 0, dwTextStyleColIndex);
			m_MeasurementListGrid.SetRowHeight (m_MeasurementListGrid.GetRowCount()-1,18);
			m_MeasurementListGrid.SetItemFormat(m_MeasurementListGrid.GetRowCount()-1, 0, dwTextStyle);
		}
//2009.08.25 bagus stress --}--
	m_MeasurementListGrid.SetItemText(iRow, iCol, psz);
	m_MeasurementListGrid.SetItemFormat(iRow, iCol, dwTextStyle);
	//PointGraph���芮���|�C���g�F�ύX
	m_PointGraph.Finish(iRow-1);
	//PointGraph���蒆�|�C���g�F�ύX

	if(m_pDoc->GetSeqMeasMode() == SEQ_ON_REMEASURE) //�����W���[
	{
		for(int i=0; i < m_pDoc->GetLastMeasPoint(); i++)
		{
			if(m_pDoc->m_bDoPointMeasFlag[iRow+i] == TRUE)
			{
				m_PointGraph.Measure(iRow+i);
				break;
			}
		}
	}
	else
	{
		m_PointGraph.Measure(iRow);
	}
/* added 2014.10.28 hmenjo ���茋�� Auto Scroll ---------- { ---------- */
	if (
		(
			(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) ||
			(m_pDoc->GetMeasMode() == MEASMODE_TEST)
			) &&
		(1 == iCol)
		) {
		this->SetFocus();
		m_MeasurementListGrid.EnsureVisible(iRow, TRUE);
	}
/* added 2014.10.28 hmenjo ���茋�� Auto Scroll ---------- } ---------- */

	//�t�H���g�T�C�Y�ύX
	for ( iRow = 1; iRow < m_MeasurementListGrid.GetRowCount(); iRow++ ) {
			m_logFont.lfHeight = 11;
		for ( iCol = 0; iCol < m_MeasurementListGrid.GetColumnCount(); iCol++ ) {
			m_MeasurementListGrid.SetItemFont(iRow, iCol, &m_logFont);
		}
	}


	m_MeasurementListGrid.Invalidate();
}

// =========================================================================
// StatisticsListGrid_InitGrid �O���b�h�̐F�E���E�T�C�Y�E�X�^�C���t�H�[�}�b�g�E�t�H���g��ݒ肷��
void CMeasurementListDlg::StatisticsListGrid_InitGrid()
{

	int iDispCol = 0; //�L��Col��
	iDispCol = m_pDoc->GetDispLabelCount(); //
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // �w�b�h�^�C�v
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;

	if(iDispCol < STATISTICSGRIDCOLMIN)
	{
		iDispCol = STATISTICSGRIDCOLMIN;
	}


	///// Grid Initialize /////
	m_StatisticsListGrid.SetEditable(FALSE);
	m_StatisticsListGrid.EnableDragAndDrop(TRUE);
	m_StatisticsListGrid.SetTextBkColor(WHITE_COLOR);
	m_StatisticsListGrid.SetRowResize(0);
// 2013.11.13 Bagus Add (TohoSpec�Ή�) -->
	m_StatisticsListGrid.SetColumnResize(0);
// 2013.11.13 Bagus Add (TohoSpec�Ή�) <--

	m_StatisticsListGrid.EnableSelection(FALSE);
	m_StatisticsListGrid.SetTrackFocusCell(FALSE);
	m_StatisticsListGrid.SetFrameFocusCell(FALSE);
	//m_StatisticsListGrid.SetFixedColumnSelection(FALSE);
	//m_StatisticsListGrid.SetFixedRowSelection(TRUE);

	TRY{
		int Rows = STATISTICSGRIDROWMAX + 1;

		int Cols = m_pDoc->GetDispLabelCount() + 1;
		//2009.08.30 bagus stress --{--
		int nLine;
		//�X�g���X�p�̃J�����̌���ǉ�����
		switch(wHeadType){
		case HEAD_TYPE_STRESS:
			Cols++;
			for(nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
				STGPGM_STRESS_LINE& LineInfo = m_rcp_data.StressStageProgInfo.Line[nLine];
				if(LineInfo.bValidLine){
					Cols++;
				}
			}
			if(Cols < STATISTICSGRIDCOLMIN)
			{
				Cols = STATISTICSGRIDCOLMIN;
			}
			break;
		default:
			if(Cols < STATISTICSGRIDCOLMIN)
			{
				Cols = STATISTICSGRIDCOLMIN;
			}
			break;
		}
		//2009.08.30 bagus stress --}--

		int FixRows = 1;
		int FixCols = 1;
		m_StatisticsListGrid.SetRowCount(Rows);
		m_StatisticsListGrid.SetColumnCount(Cols);
		m_StatisticsListGrid.SetFixedRowCount(FixRows);
		m_StatisticsListGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	///// Grid Size Initialize /////
	// Height
	int iRow = 0;
	m_StatisticsListGrid.SetRowHeight(iRow++, 36);			// Row 0
	while ( iRow < m_StatisticsListGrid.GetRowCount() ) 	// Row 1 �`
		m_StatisticsListGrid.SetRowHeight(iRow++, 18);


	// Width
	int iCol = 0;
	m_StatisticsListGrid.SetColumnWidth(iCol++, 40);		// Col 0
//	while ( iCol < m_StatisticsListGrid.GetColumnCount())	// Col 1 �`
//		m_StatisticsListGrid.SetColumnWidth(iCol++, 68);
	while ( iCol < m_StatisticsListGrid.GetColumnCount())	// Col 1 �`
		m_StatisticsListGrid.SetColumnWidth(iCol++, 80);

	///// Grid Format /////
	///// Font Initialize /////
	DWORD dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

	for ( iRow = 0; iRow < m_StatisticsListGrid.GetRowCount(); iRow++ ) {

		if(iRow == 0)
		{
			m_logFont.lfHeight = 12;
		}
		else
		{
			m_logFont.lfHeight = 11;
		}

		for ( iCol = 0; iCol < m_StatisticsListGrid.GetColumnCount(); iCol++ ) {
			m_StatisticsListGrid.SetItemFont(iRow, iCol, &m_logFont);
			m_StatisticsListGrid.SetItemFormat(iRow, iCol, dwTextStyle);
		}
	}

}

// =========================================================================
// StatisticsListGrid_HeaderSet �s�ԍ��A�񖼁i�X�L�����|�W�V�����Ƒ��薈�̍��ږ��j��ݒ肷��
void CMeasurementListDlg::StatisticsListGrid_HeaderSet()
{
	int i;

	///// Title Row /////
	//Saiki 20090603 Change ----->
	//for ( int iRow = 1; iRow < m_StatisticsListGrid.GetRowCount(); iRow++ )
	//	  m_StatisticsListGrid.SetItemText(iRow, 0, MEAS_LIST_PARAM_ITEM[iRow-1]);
	for ( int iRow = 1; iRow < m_StatisticsListGrid.GetRowCount(); iRow++ ){
		SYSTEM_CONFIG l_SystemConfig;
		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
		if(l_SystemConfig.nLanguage == 0){
			m_StatisticsListGrid.SetItemText(iRow, 0, MEAS_LIST_PARAM_ITEM_ENU[iRow-1]);
		}
		else{
			m_StatisticsListGrid.SetItemText(iRow, 0, MEAS_LIST_PARAM_ITEM_JPN[iRow-1]);
		}
	}
	//Saiki 20090603 Change <-----

	///// Title Column /////
	int iCol = 1;

	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // �w�b�h�^�C�v
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;

	//2009.08.30 bagus stress --{--
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	//�X�g���X�̓��v���ɑΉ�
	switch(wHeadType){
	case HEAD_TYPE_SR:
	//2009.09.15 bagus se --{--
	case HEAD_TYPE_SE:
	//2009.09.15 bagus se --}--
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	default:
		m_pDoc->GetDispLabel(szDispLabel);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_StatisticsListGrid.SetItemText(0, iCol, szDispLabel[i]);
			iCol++;
		}
		break;
	case HEAD_TYPE_STRESS:
		//Stress[MPa],Stree L1,....Stress Ln,ADAP(�����j
		//Valid�ɂȂ��Ă���X�g���X�̃��C�������擾����
		m_StatisticsListGrid.SetItemText(0, iCol, "Stress[MPa]");
		iCol++;
		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			STGPGM_STRESS_LINE& LineInfo = m_rcp_data.StressStageProgInfo.Line[nLine];
			if(LineInfo.bValidLine){
				CString strBuff;
				strBuff.Format("Stress L%d",nLine+1);
				m_StatisticsListGrid.SetItemText(0, iCol, strBuff.GetBuffer(0));
				iCol++;
			}
		}
		//
		m_pDoc->GetDispLabel(szDispLabel);
		for(i = 0 ; szDispLabel[i][0] != '\0'; i++)
		{
			m_StatisticsListGrid.SetItemText(0, iCol, szDispLabel[i]);
			iCol++;
		}
		break;

	}
	//2009.08.30 bagus stress --}--


}

// =========================================================================
// AddStatisticsListGrid_Data �w��ʒu�i�s�E��j�ɕ������ݒ肷��
void CMeasurementListDlg::AddStatisticsListGrid_Data(int iRow, int iCol, LPCTSTR psz)
{
	DWORD dwTextStyle;

	//2009.09.10 bagus stress --{--
	if(iRow >= m_StatisticsListGrid.GetRowCount()){
		return;
	}
	if(iCol >= m_StatisticsListGrid.GetColumnCount()){
		return;
	}
	//2009.09.10 bagus stress --}--
	if (iRow==0)
		dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	else
		dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

	m_StatisticsListGrid.SetItemText(iRow, iCol, psz);
	m_StatisticsListGrid.SetItemFormat(iRow, iCol, dwTextStyle);
	m_StatisticsListGrid.Invalidate();
}

// =========================================================================
// PreTranslateMessage
BOOL CMeasurementListDlg::PreTranslateMessage(MSG* pMsg)
{
//	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	switch(pMsg->message){
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
	case WM_RBUTTONDBLCLK:
		if (pMsg->hwnd == m_MeasurementListGrid.m_hWnd) {
			POINT l_ptlParam;
			l_ptlParam.x = LOWORD(pMsg->lParam);
			l_ptlParam.y = HIWORD(pMsg->lParam);
			if (FALSE != m_pMainFrame->m_pMeaTabViewObj->MRCS_RDblClk(l_ptlParam, &m_MeasurementListGrid)) {
				return TRUE;
			}
		}
		break;
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
	case WM_KEYDOWN:
		switch(pMsg->wParam){
		case VK_ESCAPE:
/* added 2015.04.12 hmenjo ���茋�ʉ�ʃN���A�΍� ---------- { ---------- */
		case VK_RETURN:
/* added 2015.04.12 hmenjo ���茋�ʉ�ʃN���A�΍� ---------- } ---------- */
//			pMainFrame->ChangeOperationPanel(OP_DATA);
//			pMainFrame->ChangeExeWnd(DATA_LIST_WND);
//			break;
			return TRUE;
		}
		break;
	case WM_SYSKEYDOWN:
		switch(pMsg->wParam){
		case VK_F4:
			return TRUE;
		}
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// =========================================================================
// LoadData
void CMeasurementListDlg::LoadData()
{
	m_pDoc->GetRcpData(&m_rcp_data);

}

// =========================================================================
//
void CMeasurementListDlg::SetData()
{
	CString strBuffer;
//	int iHeadType = m_MainRcpInfo.MainRcpParam.hdr.wHeadType;
	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // �w�b�h�^�C�v
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;
	int iMeasureNum=0;			 //����|�C���g��
	iMeasureNum = m_pDoc->GetMeasurePointCount();

	///// Set Recipe Name /////
	SetDlgItemText(IDC_MAIN_RECIPE_NAME, m_rcp_data.szRecipeName);

	///// Set Recipe Head /////
	SetDlgItemText(IDC_HEAD_TYPE, HEAD_TYPE_ITEM[wHeadType]);

	///// Set Scan Type /////
	CMLScanType mlScanTypeItem(wHeadType);
	SetDlgItemText(IDC_MEASUREMENT_ITEM, mlScanTypeItem[m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType]);

	///// Set Stage Point /////
	//2010.01.01.15 bagus 2point-Distance --{--
	if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		iMeasureNum /= 2;
	}
	//2010.01.01.15 bagus 2point-Distance --}--
	strBuffer.Format("%5d / %5d ", 0, iMeasureNum);
	SetDlgItemText(IDC_MEASUREMENT_POINT, strBuffer);

	//2009.09.10 bagus stress --{--
	if(wHeadType == HEAD_TYPE_STRESS){
		int nTotalNum = 0;
		STAGE_PROG_STRESS& stressStage = m_rcp_data.StressStageProgInfo;
		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			if(stressStage.Line[nLine].bValidLine){
				for(int nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
					if(stressStage.Line[nLine].bScanValid[nSection]){
						nTotalNum++;
					}
				}
			}
		}
		strBuffer.Format("%5d / %5d ", 0, nTotalNum);
		SetDlgItemText(IDC_MEASUREMENT_LS, strBuffer);
	}
	//2009.09.10 bagus stress --}--

}


// =========================================================================
// OnPaint �ĕ`�悷��
void CMeasurementListDlg::OnPaint()
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

	// �`��p���b�Z�[�W�Ƃ��� CDialog::OnPaint() ���Ăяo���Ă͂����܂���

}

// =========================================================================
//
void CMeasurementListDlg::StartMeasurementInit(int Point)
{

}
// =========================================================================
//
void CMeasurementListDlg::MeasurementError(int Point)
{
}

// ------>�e�X�g�p�֐�
double CMeasurementListDlg::GetSampleData(int iPoint, int wave)
{
	//return MEAS_GetScanSampleData(iPoint, wave);	// �X�L�����T���v���f�[�^
//	  return MEAS_GetAnalysisData(iPoint, wave);		// ���˗�����f�[�^
return 0; //�b��
}
// <------�e�X�g�p�֐�

// 2013.01.17 bagus rotate xy view -->
// ==========================================================================
//
long CMeasurementListDlg::RotateXyAxisDir(long lDir)
{
	long lRet;

	lRet = lDir;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
		break;
	case 90:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_TOP:
			lRet = DIR_RIGHT;
			break;
		case DIR_LEFT:
			lRet = DIR_TOP;
			break;
		case DIR_BOTTOM:
			lRet = DIR_LEFT;
			break;
		default:
			break;
		}
		break;
	case 180:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_LEFT;
			break;
		case DIR_TOP:
			lRet = DIR_BOTTOM;
			break;
		case DIR_LEFT:
			lRet = DIR_RIGHT;
			break;
		case DIR_BOTTOM:
			lRet = DIR_TOP;
			break;
		default:
			break;
		}
		break;
	case 270:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_TOP;
			break;
		case DIR_TOP:
			lRet = DIR_LEFT;
			break;
		case DIR_LEFT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_BOTTOM:
			lRet = DIR_RIGHT;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return lRet;
}
// 2013.01.17 bagus rotate xy view <--

// =========================================================================
//

void CMeasurementListDlg::PointGraph_DataSet()
{
	CString strBuffer;
	CNanoPointGraph::GraphConfig tConfig;

	int iMeasureNum=0;			 //����|�C���g��
	iMeasureNum = m_pDoc->GetMeasurePointCount();


	////Point Graph Data Reset
	m_PointGraph.Reset();

	////Back Color
	tConfig.BackColor = RGB(0xEE,0xFF,0xFF);
	////Edge Color
	tConfig.EdgeColor[CNanoPointGraph::EDGE_LOWER] = WHITE_COLOR;
	tConfig.EdgeColor[CNanoPointGraph::EDGE_UPPER] = RGB(0x00,0x00,0x00);
	////Status Color
	tConfig.StatusColor[CNanoPointGraph::NOT_MEASURED] = RGB(0x00,0x00,0xFF);
	tConfig.StatusColor[CNanoPointGraph::MEASURING] = RGB(0x00,0xFF,0x55);
	tConfig.StatusColor[CNanoPointGraph::SELECTED] = RGB(0xFF,0x00,0x00);
	tConfig.StatusColor[CNanoPointGraph::MEASURED] = RGB(0x77,0x77,0x77);
	// 2009.08.16 bagus stress --{--
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_NOT_MEASURED] = RGB(0x00,0x00,0xFF);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_MEASURING] = RGB(0x00,0xFF,0x55);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_MEASURED] = RGB(0x77,0x77,0x77);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_SELECTED] = RGB(0xFF,0x00,0xFF);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_NOT_USED] = RGB(0xFF,0xFF,0xFF);
	// 2009.08.16 bagus stress --}--
	// 2009.10.28 bagus 2point-Distance --{--
	////Distance Line
	tConfig.DistanceStartColor = RGB(0xB2, 0x22, 0x22);
	tConfig.DistanceEndColor = RGB(0x00, 0x80, 0x00);
	tConfig.DistanceLineColor = RGB(0xFF, 0x8C, 0x00);
	// 2009.10.28 bagus 2point-Distance --}--
	// 2013.01.09 bagus -->
	tConfig.OriginColor = GREEN_COLOR;
	// 2013.01.09 bagus -->

	////Sample Size
	char szSprintfBuff[255+1];
	memset(szSprintfBuff, 0, sizeof(szSprintfBuff));
	sprintf(szSprintfBuff, "%s%s%s%s%s%s", "Sample Size X : %",  g_lpszXyzDecimalPoint[0],	"f  Y : %",  g_lpszXyzDecimalPoint[0],	"f ", g_lpszXyzUnit[0]);
	strBuffer.Format(szSprintfBuff, m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx * g_dXyzUnitConvertCoefficient[0], m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy * g_dXyzUnitConvertCoefficient[0]);
	SetDlgItemText(IDC_SAMPLE_SIZE, strBuffer);

	////DLG��̊O�`�R���g���[���T�C�Y(Sample Size)
	WINDOWPLACEMENT placement;
	m_PointGraph.GetWindowPlacement(&placement);
	//�O���t�R���g���[���O���t�̈�T�C�Y
	double dGraphSize_x = (placement.rcNormalPosition.right - placement.rcNormalPosition.left);
	double dGraphSize_y = (placement.rcNormalPosition.bottom - placement.rcNormalPosition.top);

// 2013.01.09 bagus -->
	double dSampleSize_x;
	double dSampleSize_y;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSize_x = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSize_y = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
	}
	else {
		dSampleSize_x = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
		dSampleSize_y = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
	}
// 2013.01.17 bagus rotate xy view <--
	double dTemp;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		break;
	case 90:
	case 270:
		dTemp = dSampleSize_x;
		dSampleSize_x = dSampleSize_y;
		dSampleSize_y = dTemp;
		break;
	}
// 2013.01.17 bagus rotate xy view <--

// 	if ((dGraphSize_y / dGraphSize_x) < ((m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy)/(m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx))){
// 		//X�T�C�Y�ݒ�
// 		double dDeltaGraphSize_x = ((m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy) * (m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx)/(m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy) * (dGraphSize_y / m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy));
// 		//�X�y�[�X�̃Z���^�[���킹�p���W�v�Z
// 		double dGraphCenterOffset_x = (dGraphSize_x - dDeltaGraphSize_x)/2;
// 		placement.rcNormalPosition.left = placement.rcNormalPosition.left + dGraphCenterOffset_x;
// 		placement.rcNormalPosition.right = placement.rcNormalPosition.left + dDeltaGraphSize_x;
// 	}
// 	else{
// 		//Y�T�C�Y�ݒ�
// 		double dDeltaGraphSize_y = ((m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx) * (m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy)/(m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx) * (dGraphSize_x / m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx));
// 		placement.rcNormalPosition.bottom = placement.rcNormalPosition.top + dDeltaGraphSize_y;
// 	}
	if ((dGraphSize_y / dGraphSize_x) < (dSampleSize_y/dSampleSize_x)){
		//X�T�C�Y�ݒ�
		double dDeltaGraphSize_x = dSampleSize_x * (dGraphSize_y / dSampleSize_y);
		//�X�y�[�X�̃Z���^�[���킹�p���W�v�Z
		double dGraphCenterOffset_x = (dGraphSize_x - dDeltaGraphSize_x)/2;
		placement.rcNormalPosition.left = static_cast<long>(placement.rcNormalPosition.left + dGraphCenterOffset_x);
		placement.rcNormalPosition.right = static_cast<long>(placement.rcNormalPosition.left + dDeltaGraphSize_x);
	}
	else{
		//Y�T�C�Y�ݒ�
		double dDeltaGraphSize_y = dSampleSize_y * (dGraphSize_x / dSampleSize_x);
		placement.rcNormalPosition.bottom = static_cast<long>(placement.rcNormalPosition.top + dDeltaGraphSize_y);
	}
// 2013.01.09 bagus <--
	//�ʒu�w��
	m_PointGraph.SetWindowPlacement(&placement);

	////CNanoPointGraph�N���X���T�C�Y(Sample Size)
// 2013.01.09 bagus -->
// 	tConfig.SampleSizeX = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
// 	tConfig.SampleSizeY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
// 2013.01.17 bagus rotate xy view -->
// 	if (((StageConfig.Dir.X == 2) || (StageConfig.Dir.X == 4)) && ((StageConfig.Dir.Y == 1) || (StageConfig.Dir.Y == 3))) {
// 		// X<->Y����ւ�
// 		tConfig.SampleSizeY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
// 		tConfig.SampleSizeX = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
// 	} else if (((StageConfig.Dir.X == 1) || (StageConfig.Dir.X == 3)) && ((StageConfig.Dir.Y == 2) || (StageConfig.Dir.Y == 4))) {
// 		// X-Y���̂܂�
// 		tConfig.SampleSizeX = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
// 		tConfig.SampleSizeY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
// 	} else {
// 		// ���ݒ肪��������
// 	}
	double dSampleSizeX;
	double dSampleSizeY;

	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
		// X<->Y����ւ�
		dSampleSizeY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeX = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
		// X-Y���̂܂�
		dSampleSizeX = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
	} else {
		// ���ݒ肪��������
	}

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		break;
	case 90:
	case 270:
		dTemp = dSampleSizeX;
		dSampleSizeX = dSampleSizeY;
		dSampleSizeY = dTemp;
		break;
	}

	tConfig.SampleSizeX = dSampleSizeX;
	tConfig.SampleSizeY = dSampleSizeY;
// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus <--
	m_PointGraph.SetConfig(&tConfig);

	////Point Data
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
//	double dOffSetValueX = (m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx/2);
//	double dOffSetValueY = (m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy/2);
	double dOffSetValueX;
	double dOffSetValueY;
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
		dOffSetValueX = (m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx/2);
		dOffSetValueY = (m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy/2);
// 2013.01.17 bagus rotate xy view -->
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			break;
		case 90:
			break;
		case 180:
			break;
		case 270:
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 2:
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = 0;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
// //		dOffSetValueY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
// 		dOffSetValueY = 0;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			break;
		case 90:
			dOffSetValueX += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 180:
			dOffSetValueX += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 270:
			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 3:
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = 0;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
// //		dOffSetValueY = 0;
// 		dOffSetValueY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			dOffSetValueX += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 90:
			break;
		case 180:
			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 270:
			dOffSetValueX += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 4:
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
// //		dOffSetValueY = 0;
// 		dOffSetValueY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			dOffSetValueX += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 90:
			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 180:
			break;
		case 270:
			dOffSetValueX += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 5:
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
// //		dOffSetValueY = m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
// 		dOffSetValueY = 0;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
 			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 90:
			dOffSetValueX += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 180:
			dOffSetValueX += m_rcp_data.StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 270:
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	}
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--

// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus -->
// 	if ( StageConfig.Dir.X == DIR_RIGHT ||
// 		 StageConfig.Dir.X == DIR_LEFT ) {
// 		m_PointGraph.SetOriginPointData(dOffSetValueX, dOffSetValueY);
// 	}
// 	else {
// 		m_PointGraph.SetOriginPointData(dOffSetValueY, dOffSetValueX);
// 	}
	double dOriginPointDataX;
	double dOriginPointDataY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dOriginPointDataX = dOffSetValueX;
		dOriginPointDataY = dOffSetValueY;
	}
	else {
		dOriginPointDataX = dOffSetValueY;
		dOriginPointDataY = dOffSetValueX;
	}

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		m_PointGraph.SetOriginPointData(dOriginPointDataX, dOriginPointDataY);
		break;
	case 90:
		m_PointGraph.SetOriginPointData(dOriginPointDataY, dOriginPointDataX);
		dOffSetValueX = dOriginPointDataY;
		dOffSetValueY = dOriginPointDataX;
		break;
	case 180:
		m_PointGraph.SetOriginPointData(dOriginPointDataX, dOriginPointDataY);
		break;
	case 270:
		m_PointGraph.SetOriginPointData(dOriginPointDataY, dOriginPointDataX);
		dOffSetValueX = dOriginPointDataY;
		dOffSetValueY = dOriginPointDataX;
		break;
	}
// 2013.01.17 bagus rotate xy view <--

// 2013.01.09 bagus <--

	double Scan_PointIncOffsetX;
	double Scan_PointIncOffsetY;

	long lDirX = RotateXyAxisDir(m_StageConfig.Dir.X);
	long lDirY = RotateXyAxisDir(m_StageConfig.Dir.Y);

	for(int iPoint= 0; iPoint<iMeasureNum; iPoint++){
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
//		Scan_PointIncOffsetX = m_rcp_data.ScanPoint[iPoint].lX + dOffSetValueX;
//		Scan_PointIncOffsetY = -(m_rcp_data.ScanPoint[iPoint].lY - dOffSetValueY);
		switch (lDirX) {
		case 1: 	// X:��
		default:
			Scan_PointIncOffsetX = dOffSetValueX + m_rcp_data.ScanPoint[iPoint].lX;
			break;
		case 2: 	// X:��
			Scan_PointIncOffsetY = dOffSetValueX - m_rcp_data.ScanPoint[iPoint].lX;
			break;
		case 3: 	// X:��
			Scan_PointIncOffsetX = dOffSetValueX - m_rcp_data.ScanPoint[iPoint].lX;
			break;
		case 4: 	// X:��
			Scan_PointIncOffsetY = dOffSetValueX + m_rcp_data.ScanPoint[iPoint].lX;
			break;
		}

		switch (lDirY) {
		case 1: 	// Y:��
			Scan_PointIncOffsetX = dOffSetValueY + m_rcp_data.ScanPoint[iPoint].lY;
			break;
		case 2: 	// Y:��
		default:
			Scan_PointIncOffsetY = dOffSetValueY - m_rcp_data.ScanPoint[iPoint].lY;
			break;
		case 3: 	// Y:��
			Scan_PointIncOffsetX = dOffSetValueY - m_rcp_data.ScanPoint[iPoint].lY;
			break;
		case 4: 	// Y:��
			Scan_PointIncOffsetY = dOffSetValueY + m_rcp_data.ScanPoint[iPoint].lY;
			break;
		}
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
		m_PointGraph.AddPoint(Scan_PointIncOffsetX, Scan_PointIncOffsetY);
	}

	WORD wHeadType = m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType; // �w�b�h�^�C�v
	WORD wScanType = m_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType;

	if(wHeadType == HEAD_TYPE_STRESS){
		//
		CNanoPointGraph::LSData tLineData;
		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			STGPGM_STRESS_LINE& LineInfo = m_rcp_data.StressStageProgInfo.Line[nLine];
			if(LineInfo.bValidLine){
				for(int nSect = 0;nSect < STRESS_SECTIONS_MAX;nSect++){
					if(LineInfo.bScanValid[nSect]){
						tLineData.Status = CNanoPointGraph::SECTION_NOT_MEASURED;

// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
//						tLineData.StartX = LineInfo.SectPos[nSect].lScanStartPosX + dOffSetValueX;
//						tLineData.StartY = -(LineInfo.SectPos[nSect].lScanPosY - dOffSetValueY);
//						tLineData.EndX = LineInfo.SectPos[nSect].lScanEndPosX + dOffSetValueX;
//						tLineData.EndY = -(LineInfo.SectPos[nSect].lScanPosY - dOffSetValueY);
						switch (lDirX) {
						case 1: 	// X:��
						default:
							tLineData.StartX = static_cast<long>(dOffSetValueX + LineInfo.SectPos[nSect].lScanStartPosX);
							tLineData.EndX = static_cast<long>(dOffSetValueX + LineInfo.SectPos[nSect].lScanEndPosX);
							break;
						case 2: 	// X:��
							tLineData.StartY = static_cast<long>(dOffSetValueX - LineInfo.SectPos[nSect].lScanStartPosX);
							tLineData.EndY = static_cast<long>(dOffSetValueX - LineInfo.SectPos[nSect].lScanEndPosX);
							break;
						case 3: 	// X:��
							tLineData.StartX = static_cast<long>(dOffSetValueX - LineInfo.SectPos[nSect].lScanStartPosX);
							tLineData.EndX = static_cast<long>(dOffSetValueX - LineInfo.SectPos[nSect].lScanEndPosX);
							break;
						case 4: 	// X:��
							tLineData.StartY = static_cast<long>(dOffSetValueX + LineInfo.SectPos[nSect].lScanStartPosX);
							tLineData.EndY = static_cast<long>(dOffSetValueX + LineInfo.SectPos[nSect].lScanEndPosX);
							break;
						}

						switch (lDirY) {
						case 1: 	// Y:��
							tLineData.StartX = static_cast<long>(dOffSetValueY + LineInfo.SectPos[nSect].lScanPosY);
							tLineData.EndX = static_cast<long>(dOffSetValueY + LineInfo.SectPos[nSect].lScanPosY);
							break;
						case 2: 	// Y:��
						default:
							tLineData.StartY = static_cast<long>(dOffSetValueY - LineInfo.SectPos[nSect].lScanPosY);
							tLineData.EndY = static_cast<long>(dOffSetValueY - LineInfo.SectPos[nSect].lScanPosY);
							break;
						case 3: 	// Y:��
							tLineData.StartX = static_cast<long>(dOffSetValueY - LineInfo.SectPos[nSect].lScanPosY);
							tLineData.EndX = static_cast<long>(dOffSetValueY - LineInfo.SectPos[nSect].lScanPosY);
							break;
						case 4: 	// Y:��
							tLineData.StartY = static_cast<long>(dOffSetValueY + LineInfo.SectPos[nSect].lScanPosY);
							tLineData.EndY = static_cast<long>(dOffSetValueY + LineInfo.SectPos[nSect].lScanPosY);
							break;
						}
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--

						//2009.08.30 bagus stress --{--
						tLineData.SourceLineNo = nLine+1;
						tLineData.SourceSectionNo = nSect+1;
						//2009.08.30 bagus stress --}--
						m_PointGraph.AddLineData (&tLineData);
					}
				}
			}
		}
		//�X�g���X����p�̕`���L���ɂ���
		m_PointGraph.SetLineStatusMode(TRUE);
	}
	//2009.10.28 bagus 2point-Distance --{--
	//2�_�ԋ����̕\�����[�h�ɐ؂�ւ���
	else if(wHeadType == HEAD_TYPE_SR && wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		m_PointGraph.SetDistanceMode(TRUE);
	}
	//2009.10.28 bagus 2point-Distance --}--
	//m_PointGraph.Measure(0);
}



void CMeasurementListDlg::OnTimer(UINT nIDEvent)
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	if( m_pDoc->GetLastMeasPoint() == m_pDoc->GetShowCompPointList() ) //�� [����]�ǉ�����̃��W�b�N�ǉ����́A�ǉ������\���ł���悤�ɏC�����K�v�H hibino
	{
		//2009.08.31 bagus stess --{--
		//�v���Z�b�V���O���ł���΂܂��^�C�}���~�߂Ȃ��i�����Ŏ~�߂�ƃX�g���X����̌��ʂ��\���ł��Ȃ��j
		if(m_pDoc->GetProcessStatus() != PROCESS_PROC){
			m_PointGraph.FinishSection(m_pDoc->GetMeasuredStressLSNum());
			KillTimer(SHOW_LIST_TIMER);
			MeasurementListGrid_UnSelectAll();
		}
		//KillTimer(SHOW_LIST_TIMER);
		//MeasurementListGrid_UnSelectAll();
		//2009.08.31 bagus stess --}--
	}
	SetMeasurementData2();
	//2009.08.31 bagus stress --{--
	if(m_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		SetMeasurementLSData(m_pDoc->GetStoreCount());
	}
	//2009.08.31 bagus stress --}--


	if(m_pDoc->GetSeqMeasMode() == SEQ_NOT_MEASURE) //���蒆�łȂ�
	{
		m_PointGraph.Finish(m_pDoc->GetStoreCount());
		//2009.09.10 bagus stress -- {--
		//2009.09.10 bagus stress -- }--
		//2009.12.03 bagus stress �폜 --{--
		//m_PointGraph.FinishSection(m_pDoc->GetMeasuredStressLSNum());
		//2009.12.03 bagus stress --}--
	}


	CDialog::OnTimer(nIDEvent);
}


// =========================================================================
// m_MeasurementListGrid�̑I������Ă���Z����S�ĉ�������
void CMeasurementListDlg::MeasurementListGrid_UnSelectAll()
{
	int iRowCount = m_MeasurementListGrid.GetRowCount();
	int iColumnCount = m_MeasurementListGrid.GetColumnCount();

	for(int iRow=1; iRow<iRowCount; iRow++)
	{
		for(int iCol=1; iCol<iColumnCount; iCol++)
		{
			//�I������
			int iState = m_MeasurementListGrid.GetItemState(iRow, iCol);
			iState = (iState & GVIS_SELECTED) ^ iState;
			BOOL bRtnChk = m_MeasurementListGrid.SetItemState(iRow, iCol, iState);
			m_MeasurementListGrid.Invalidate();
		}
	}
}

/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
void CMeasurementListDlg::ChgSizeCtrl(long ldY)
{
	/* ���胊�X�g �O���b�h ��ӈړ�	*/
	RECT l_rectMeasGrid;	m_MeasurementListGrid.GetWindowRect(&l_rectMeasGrid);
	m_MeasurementListGrid.SetWindowPos(	0,
						0, 0,
						l_rectMeasGrid.right - l_rectMeasGrid.left,
						l_rectMeasGrid.bottom - l_rectMeasGrid.top + ldY,
						SWP_NOZORDER | SWP_NOMOVE);
	/* ���v���X�g �O���b�h �ړ�	*/
	RECT l_rectStatListGrid;	m_StatisticsListGrid.GetWindowRect(&l_rectStatListGrid);
	POINT l_ptStatListGrid = {l_rectStatListGrid.left, l_rectStatListGrid.top + ldY};
	this->ScreenToClient(&l_ptStatListGrid);
	m_StatisticsListGrid.SetWindowPos(0, l_ptStatListGrid.x, l_ptStatListGrid.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	/* �{�̃_�C�A���O ��ӈړ�	*/
	RECT l_rectDlg;	this->GetWindowRect(&l_rectDlg);
	this->SetWindowPos(	0,
						0, 0,
						l_rectDlg.right - l_rectDlg.left,
						l_rectDlg.bottom - l_rectDlg.top + ldY,
						SWP_NOZORDER | SWP_NOMOVE);
}
/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */
