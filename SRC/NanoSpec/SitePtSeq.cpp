// SitePtSeq.cpp : implementation file
//

#include "stdafx.h"
#include "System.h"
#include "nanospec.h"
#include "MainFrm.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\PatRec.hxx"
#include "GropeObject.h"
#include "SitePtSeq.h"
#include "SitePtSeqWnd.h"
#include "NanoSpecDoc.h"
#include "Deskew.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#define STAGE_MOVE_DELAY		(1000)

#define OVERLAY_CENTER_X (644 / 2)	// COverlayDialog::m_rcOverlay.Width() / 2
#define OVERLAY_CENTER_Y (483 / 2)	// COverlayDialog::m_rcOverlay.Hight() / 2

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* �����Y�I�t�Z�b�g�Ή�	*/
//#define STAGEMOVE_ORIGINAL		/* Stage.dll�̃I���W�i�����W�ňړ�	*/


/////////////////////////////////////////////////////////////////////////////
// CSitePtSeq

IMPLEMENT_DYNCREATE(CSitePtSeq, CWinThread)

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CSitePtSeq, CWinThread)
	//{{AFX_MSG_MAP(CSitePtSeq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSitePtSeq message handlers

/////////////////////////////////////////////////////////////////////////////
// CSitePtAutoSeq

IMPLEMENT_DYNCREATE(CSitePtAutoSeq, CSitePtSeq)

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSitePtAutoSeq::InitInstance()
{
	/* delete this ���܂�	*/
	m_bAutoDelete = TRUE;

	m_bShutDown = FALSE;

	::ZeroMemory(SitePtResult, sizeof(SitePtResult));
	::ZeroMemory(MatchLoc, sizeof(MatchLoc));

	/* �R���t�B�O�ǎ�	*/
	ConfigFile_GetNanoSpecIni(m_SrVideoCalib, CONFIG_FILE_SR_VIDEO_CALIBRATION);
	ConfigFile_GetNanoSpecIni(&m_patternRec, CONFIG_FILE_SR_PATTERN_REC);
	// 2013.01.09 bagus -->
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
	// 2013.01.09 bagus <--

	// SitePR ���O�t�@�C���̏�����
	m_SitePtLogFile.Create(	g_szLog_Dir, 								// LOG_DIR,
						SITEPR_LOG_NAME,
						SITEPR_LOG_SIZE,
						SITEPR_LOG_MAX);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
int CSitePtAutoSeq::ExitInstance()
{
	TRACE(_T("CSitePtAutoSeq::ExitInstance()\n"));

	m_pChiefTransiSr1Point->TransiEvent(m_iEvent, 0L);

	return CSitePtSeq::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CSitePtAutoSeq, CSitePtSeq)
	//{{AFX_MSG_MAP(CSitePtAutoSeq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSitePtAutoSeq message handlers

/////////////////////////////////////////////////////////////////////////////
//
int CSitePtAutoSeq::Run()
{
	BOOL bAutoFocus = false;
	BOOL bPrePos = false;
	int  i_LastSite = 0;
/* added 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
	STAGE_COORD l_OffsetAdd;
/* added 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */
	int iSite;

	/* �T�C�g�p�^�[���J�n �O�ʒu�擾	*/
	NS_StageGetPos(&PrePosition);		/* �f�t�H���g�|�W�V����(�Ȃɂ����Ȃ��������̈�)	*/

	/* �T�C�g�p�^�[�����s����	*/
	for ( iSite = 0; iSite < 2; iSite++ ) {
		if ( m_bShutDown )
			break;

		/* �T�C�g�p�^�[�� �p�^�[���o�^ �L���m�F	*/
		if(strlen(m_pSitePattern->PatternInfo[iSite].szSitePatternName) == 0){
			/* �p�^�[�����o�^�̓p�X	*/
			continue;
		}
/* added 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
		/* �T�u�����擾	*/
		PatternFile_GetSubInfo(m_pSitePattern->PatternInfo[iSite].szSitePatternName, &(m_SprSubInfo[iSite]));
/* added 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */

		sprintf(szLogText,"== Site: %d / Pattern Image: %s / Score:%3.2f%% / Grop:%3.2f%% \0",iSite+1 , m_pSitePattern->PatternInfo[iSite].szSitePatternName, m_patternRec.dLowestMatchScore, m_patternRec.dStopGropingScore);
		SitePtLogging(szLogText);

		/* �����Y�ؑ�	*/
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//		StageSelectLensNotify(m_pSitePattern->PatternInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */

		StageSelectLensNotify(m_SprSubInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

		/* �J���[�t�B���^����(OPEN �ɂ��܂�)	*/
		if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
			/* �ُ펞�ł��������܂��D	*/
			((CChiefView*) m_pChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CSitePtAutoSeq::Run()."));
		}

/* deleted 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//		/* �T�C�g�p�^�[���J�n �O�ʒu�擾�i�����ς݂̃����Y�ʒu��ێ��j	*/
//		NS_StageGetPos(&PrePosition);
/* deleted 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */

		/* �`�e	*/
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
		bAutoFocus = NS_StageDoAutoFocus();
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

		//Saiki 20090723 Change ----->
		DWORD l_dwAFValid = 0;
		AF_VALID*	l_pAFValid = (AF_VALID*) &l_dwAFValid;

		if ( !bAutoFocus ) {
	   		// AF ���s
			l_pAFValid->ucResultAF = (BYTE)2;	  // ������������
	   	}
	   	else {
			// AF ����
			l_pAFValid->ucResultAF = (BYTE)1;	  // AF ����
		}

		//Saiki 20090727 Add ----->
		((CChiefTransiSr1Point*) ((CChiefView*) m_pChiefView)->m_pcChiefTransiSr1Point)->m_dwAFValid = l_pAFValid->dwValid;
		//Saiki 20090727 Add <-----

		//Saiki 20090723 Change <-----

/* added 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
		/* �T�C�g�p�^�[���J�n �O�ʒu�擾�i�����ς݂̃����Y�ʒu��ێ��j	*/
		NS_StageGetPos(&PrePosition);		/* �ʒu�|�[�����O�Ԋu���ԑ҂��̂��߃R�R�ֈړ��@*/
/* added 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */
		if ( !bAutoFocus )
			break;
		if ( m_bShutDown )
			break;

		/* �X�e�[�W�ړ��҂� */
		Delay(STAGE_MOVE_DELAY);
		if ( m_bShutDown )
			break;

		/* �T�C�g�o�q���s */
		i_LastSite = iSite;
		DoPatRecMatchWithVerify(iSite, &SitePtResult[iSite], &MatchLoc[iSite]);
		if ( SitePtResult[iSite].Score < m_patternRec.dLowestMatchScore ){ // pat rec is NG
			/* �p�^�[����������Ȃ�������A���m�f	*/
			break;
		}
	}

	/* �T�C�g�p�^�[�������I��	*/
	if ( m_bShutDown ) {
		/* �L�����Z���I��	*/
		m_iEvent = EV_SR1P_SITEPR_AUTO_CANCEL;

		sprintf(szLogText,"SitePR Cancel End\0");
		SitePtLogging(szLogText);
	}
	else {
		if ( SitePtResult[i_LastSite].Score >= m_patternRec.dLowestMatchScore ) // pat rec is OK
		{
			/* �T�C�g�p�^�[����������	*/
			m_iEvent = EV_SR1P_SITEPR_AUTO_OK;

			sprintf(szLogText,"SitePR OK complete %3.2f%%\0",SitePtResult[i_LastSite].Score );
			SitePtLogging(szLogText);
		}
		else
		{
			switch ( m_pStageProgInfoHdr->nSitePrFailOption )
			{
			case SITE_PR_FAIL_OPTION_FORCE:					/* ��������i���̂܂ܑ���J�n�j	*/
// 2009.05.26 K.Matsuo SitePR -->
				if(iSite == 0 && !bAutoFocus){
					/* �X�e�[�WPGM���W�̂܂ܗ��܂��Ă����iSitePattern�������ŁA�X�e�[�W�ړ����Ă��Ȃ��j */
					m_iEvent = EV_SR1P_SITEPR_AUTO_PROCEED;				/* ��������(�X�e�[�WPGM���W�̂܂ܗ��܂��Ă���)*/

					sprintf(szLogText,"SitePR Force complete %3.2f%% non move \0",SitePtResult[i_LastSite].Score );
					SitePtLogging(szLogText);
				}else{
					/* �X�e�[�WPGM���W�Ɉړ����� */

					// X,Y ���W���擾�i�����Y�ύX���Ă���\��������̂ŁA�����Y�I�t�Z�b�g���l���������W�����߂�K�v����B
					//				   ����āA�X�e�[�WPGM�̍��W����ēx�v�Z���Ȃ����B�T�C�g�p�^�[���J�n�O�̃X�e�[�W���W�̗��p�́A�s�I�j
					STAGE_COORD l_ScanPointStage;
					l_ScanPointStage.lX = ((CChiefTransiSr1Point*) ((CChiefView*) m_pChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lX;
					l_ScanPointStage.lY = ((CChiefTransiSr1Point*) ((CChiefView*) m_pChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lY;
					// �␳
					TCHAR	l_szMainRcpName[256];
					((CChiefView*) m_pChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// K.Matsuo 2009.09.01 -->
//					NS_ConvertToStageMoveCoord(&l_ScanPointStage, l_szMainRcpName);

//					STAGE_COORD stageCoordTemp;
//					DeskewCorrectToCurrent(l_ScanPointStage, stageCoordTemp);
//					l_ScanPointStage = stageCoordTemp;

					((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
//					StageMoveAbsolute(&l_ScanPointStage);
					NS_SampleCoordStageMoveAbsolute(&l_ScanPointStage, l_szMainRcpName, HEAD_TYPE_SR);
					((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
// K.Matsuo 2009.09.01 <--
					m_iEvent = EV_SR1P_SITEPR_AUTO_PROCEED_PRESTEP;		/* ��������(�X�e�[�WPGM���W�Ɉړ�����)	*/
// 2009.05.26 K.Matsuo SitePR <--

					sprintf(szLogText,"SitePR Force complete %3.2f%% orginal position\0",SitePtResult[i_LastSite].Score );
					SitePtLogging(szLogText);
				}
				break;

			case SITE_PR_FAIL_OPTION_CANCEL:				/* ����L�����Z��	*/
				m_iEvent = EV_SR1P_SITEPR_AUTO_CANCEL;

				sprintf(szLogText,"SitePR Cancel complete %3.2f%%\0",SitePtResult[i_LastSite].Score );
				SitePtLogging(szLogText);
				break;

			case SITE_PR_FAIL_OPTION_MANUAL:				/* ���݈ʒu�Ń}�j���A���T�C�g�p�^�[���Ɉڍs����	*/
				/* �X�e�[�WPGM���W�Ɉړ����� */

				// X,Y ���W���擾�i�����Y�ύX���Ă���\��������̂ŁA�����Y�I�t�Z�b�g���l���������W�����߂�K�v����B
				//				   ����āA�X�e�[�WPGM�̍��W����ēx�v�Z���Ȃ����B�T�C�g�p�^�[���J�n�O�̃X�e�[�W���W�̗��p�́A�s�I�j
				STAGE_COORD l_ScanPointStage;
				l_ScanPointStage.lX = ((CChiefTransiSr1Point*) ((CChiefView*) m_pChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lX;
				l_ScanPointStage.lY = ((CChiefTransiSr1Point*) ((CChiefView*) m_pChiefView)->m_pcChiefTransiSr1Point)->m_MeasPosbufXYZ.lY;
				// �␳
				TCHAR	l_szMainRcpName[256];
				((CChiefView*) m_pChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// K.Matsuo 2009.09.01 -->
//				NS_ConvertToStageMoveCoord(&l_ScanPointStage, l_szMainRcpName);

//				STAGE_COORD stageCoordTemp;
//				DeskewCorrectToCurrent(l_ScanPointStage, stageCoordTemp);
//				l_ScanPointStage = stageCoordTemp;

				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
//				StageMoveAbsolute(&l_ScanPointStage);
				NS_SampleCoordStageMoveAbsolute(&l_ScanPointStage, l_szMainRcpName, HEAD_TYPE_SR);
				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
// K.Matsuo 2009.09.01 <--
				m_iEvent = EV_SR1P_SITEPR_AUTO_MANU;

				sprintf(szLogText,"SitePR to Manual mode %3.2f%% original position\0",SitePtResult[i_LastSite].Score );
				SitePtLogging(szLogText);
				break;

			case SITE_PR_FAIL_OPTION_PRE_FORCE:				/* ��������i�O�̈ʒu�ő���J�n�j	*/
				/* �O�ʒu�ֈړ� */
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
//				NS_StageMoveAbsolute(&PrePosition);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
				if ((0 != _tcslen(m_pSitePattern->PatternInfo[0].szSitePatternName)) && (1 == i_LastSite)) {
					/*	�p�^�[���P�̓o�^���L��C�p�^�[���Q�Ŏ��s�̏ꍇ�C
						�p�^�[���P�̃I�t�Z�b�g���g���܂��D */
					l_OffsetAdd.lX = m_SprSubInfo[0].ptOffset.x;
					l_OffsetAdd.lY = m_SprSubInfo[0].ptOffset.y;
					_stprintf(szLogText, _T("SitePR offset : %s, x = %d, y = %d"), m_pSitePattern->PatternInfo[0].szSitePatternName, l_OffsetAdd.lX, l_OffsetAdd.lY);
					SitePtLogging(szLogText);
				} else {
					l_OffsetAdd.lX = 0;
					l_OffsetAdd.lY = 0;
				}
				l_OffsetAdd.lX += PrePosition.lX;
				l_OffsetAdd.lY += PrePosition.lY;
				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				NS_StageMoveAbsolute(&l_OffsetAdd);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */
				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				Delay(STAGE_MOVE_DELAY);

				m_iEvent = EV_SR1P_SITEPR_AUTO_PROCEED_PRESTEP;		/* ��������(�ŏIAF�� �ړ����Ă���)	*/

				sprintf(szLogText,"SitePR Force complete %3.2f%% Pre position\0",SitePtResult[i_LastSite].Score );
				SitePtLogging(szLogText);
				break;

			case SITE_PR_FAIL_OPTION_PRE_MANUAL:			/* �O�̈ʒu�Ń}�j���A���T�C�g�p�^�[���Ɉڍs����	*/
				/* �O�ʒu�ֈړ� */
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
//				NS_StageMoveAbsolute(&PrePosition);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
				if ((0 != _tcslen(m_pSitePattern->PatternInfo[0].szSitePatternName)) && (1 == i_LastSite)) {
					/*	�p�^�[���P�̓o�^���L��C�p�^�[���Q�Ŏ��s�̏ꍇ�C
						�p�^�[���P�̃I�t�Z�b�g���g���܂��D */
					l_OffsetAdd.lX = m_SprSubInfo[0].ptOffset.x;
					l_OffsetAdd.lY = m_SprSubInfo[0].ptOffset.y;
					_stprintf(szLogText, _T("SitePR offset : %s, x = %d, y = %d"), m_pSitePattern->PatternInfo[0].szSitePatternName, l_OffsetAdd.lX, l_OffsetAdd.lY);
					SitePtLogging(szLogText);
				} else {
					l_OffsetAdd.lX = 0;
					l_OffsetAdd.lY = 0;
				}
				l_OffsetAdd.lX += PrePosition.lX;
				l_OffsetAdd.lY += PrePosition.lY;
				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				NS_StageMoveAbsolute(&l_OffsetAdd);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */
				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				Delay(STAGE_MOVE_DELAY);

				m_iEvent = EV_SR1P_SITEPR_AUTO_MANU;

				sprintf(szLogText,"SitePR to Manual mode %3.2f%% Pre position\0",SitePtResult[i_LastSite].Score );
				SitePtLogging(szLogText);
				break;
			}
		}
	}

	sprintf(szLogText,"--------------------------------------------------------------------\0");
	SitePtLogging(szLogText);

	::PostQuitMessage(0);

	return CSitePtSeq::Run();
}

/////////////////////////////////////////////////////////////////////////////
//
void CSitePtAutoSeq::DoPatRecMatchWithVerify( int iSite,		 // first or second deskew site
											  DESKEW_RESULT* pSitePtResult,
											  STAGE_COORD* pMatchLoc)
{
	// perform a pattern rec match, if success, move the match location
	// to the center of the overlay and do another match to verify.

	// Score = Model coverage x (1 - (Fit error weighting factor x Normalized Fit Error))
	// Target Score = Target coverage x (1 - (Fit error weighting factor x Normalized Fit Error))

	// The model coverage and target coverage are defined as follows:
	//
	// Model coverage.
	// The model coverage is the percentage of the total length of the model's active edges found in the occurrence. 100% indicates that for each of the model's active edges, a corresponding edge was found in the occurrence.
	// Target coverage.
	// The target coverage is the percentage of the total length of the model's active edges found in the occurrence, divided by the total length of edges present within the occurrence's bounding box.
	// Thus, a target coverage score of 100% means that no extra edges were found. Lower scores indicate that features or edges found in the target (result occurrence) are not present in the model.

	int		GropStep = 0;					// loging count only

	const int PatRecTargetScore= static_cast<int>(40.0);

	Delay(STAGE_MOVE_DELAY);

	// should block after restore is done
	char szBuff[MAX_PATH];
	//Saiki 20090602 Add ----->
	CString strMsg1, strMsg2, strBuffer;
	//Saiki 20090602 Add <-----

/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//	sprintf(szBuff, "%s%s.SP%d", g_szDb_Pattern_Img_Dir, m_pSitePattern->PatternInfo[iSite].szSitePatternName, m_pSitePattern->PatternInfo[iSite].iLens);
// myan File�L���m�F�K�v
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
//	PatternFile_MakePatternImageFilePath(szBuff, m_pSitePattern->PatternInfo[iSite].szSitePatternName, m_SprSubInfo[iSite].iLens);
	PatternFile_MakePatternImageFilePath(szBuff, m_pSitePattern->PatternInfo[iSite].szSitePatternName);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */
	PatRecRestoreModel(szBuff);

	// keep track of the site of the highest score
	DESKEW_RESULT bestResult;
	STAGE_COORD	bestMatchLoc;

	// do the first match
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//	  DoPatternRecMatch(pSitePtResult, pMatchLoc, m_pSitePattern->PatternInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
	DoPatternRecMatch(pSitePtResult, pMatchLoc, m_SprSubInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */

	sprintf(szLogText,"Pattern Match : Score:%3.2f%%  Target:%3.2f%% \0", pSitePtResult->Score, pSitePtResult->TargetScore);
	SitePtLogging(szLogText);

	//Saiki 20090602 Change ----->
	//sprintf(szLogText, IDS_CHIF_SITEPR_AUTO_ING "Result %3.2f%%\0", pSitePtResult->Score);
	LoadStringML(IDS_CHIF_SITEPR_AUTO_ING, strMsg1, "Site Pattern:Auto Site Pattern Executing...");
	LoadStringML(IDS_RESULT, strMsg2, "Result %3.2f%%");
	strBuffer = strMsg1 + strMsg2;
	sprintf(szLogText, strBuffer, pSitePtResult->Score);
	//Saiki 20090602 Change <-----
	((CMainFrame*) ((m_pChiefView)->m_pcMainFrame))->SetMessageText(szLogText);	// Status Bar

	// update the best result so far
	bestResult = *pSitePtResult;
	bestMatchLoc = *pMatchLoc;

	// if the first score is less than the match score and pat rec groping is enabled, do groping
	if( pSitePtResult->Score < m_patternRec.dStopGropingScore && m_patternRec.nNumLayers > 0)
	{
		const int StepGropeLayers[] = { 2, 3, 4 };
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//		long DeltaX = long( OVERLAY_CENTER_X * m_SrVideoCalib[m_pSitePattern->PatternInfo[iSite].iLens].dXPixelToMicron * 2 / StepGropeLayers[0] );
//		long DeltaY = long( OVERLAY_CENTER_Y * m_SrVideoCalib[m_pSitePattern->PatternInfo[iSite].iLens].dYPixelToMicron * 2 / StepGropeLayers[0] );
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
		long DeltaX = long( OVERLAY_CENTER_X * m_SrVideoCalib[m_SprSubInfo[iSite].iLens].dXPixelToMicron * 2 / StepGropeLayers[0] );
		long DeltaY = long( OVERLAY_CENTER_Y * m_SrVideoCalib[m_SprSubInfo[iSite].iLens].dYPixelToMicron * 2 / StepGropeLayers[0] );
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */

		short dX, dY;	 // stage translation based on retry site number

		// do groping retry if feature not found in the previous try
		//
		CGropeObject GropObj(m_patternRec.nNumLayers);	 // object which will tell where to grope

		STAGE_COORD Position;
		const int nMaxRetries = 2;
		int nRetries = nMaxRetries;
		while ( pSitePtResult->Score < m_patternRec.dStopGropingScore && !GropObj.Finished() ) {
			if ( m_bShutDown )
				return;

			// get the current stage position
			NS_StageGetPos(&Position);

			while ( PatRecTargetScore <= pSitePtResult->TargetScore && nRetries-- > 0 ) {
				if ( m_bShutDown )
					return;

				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				NS_StageMoveAbsolute(pMatchLoc);
				((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				Delay(STAGE_MOVE_DELAY);

				// retry the match at this new site
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//				  DoPatternRecMatch(pSitePtResult, pMatchLoc, m_pSitePattern->PatternInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
				DoPatternRecMatch(pSitePtResult, pMatchLoc, m_SprSubInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */

				sprintf(szLogText,"Pattern Match : Score:%3.2f%%  Target:%3.2f%%  Target Check %d\0", pSitePtResult->Score, pSitePtResult->TargetScore, nRetries);
				SitePtLogging(szLogText);

				//Saiki 20090602 Change ----->
				//sprintf(szLogText, IDS_CHIF_SITEPR_AUTO_ING "Verify Result %3.2f%%  (Best %3.2f%%)\0", pSitePtResult->Score, bestResult.Score);
				LoadStringML(IDS_CHIF_SITEPR_AUTO_ING, strMsg1, "Site Pattern:Auto Site Pattern Executing...");
				LoadStringML(IDS_VERIFY_RESULT, strMsg2, "Verify Result %3.2f%%  (Best %3.2f%%)");
				strBuffer = strMsg1 + strMsg2;
				sprintf(szLogText, strBuffer , pSitePtResult->Score, bestResult.Score);
				//Saiki 20090602 Change <-----
				((CMainFrame*) ((m_pChiefView)->m_pcMainFrame))->SetMessageText(szLogText);	// Status Bar

				if ( pSitePtResult->Score > bestResult.Score ) {
					// update the best score so far
					bestResult = *pSitePtResult;
					bestMatchLoc = *pMatchLoc;
				}
				if ( pSitePtResult->Score >= m_patternRec.dStopGropingScore ) goto SUCCESS;
			}
			nRetries = nMaxRetries;

			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			NS_StageMoveAbsolute(&Position);
			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

			GropObj.GetNextMove( &dX, &dY );   // get the next movement amount and direction

			// ask stage to move to the new site
			STAGE_COORD stageCoord;
			stageCoord.lX = dX * DeltaX;
			stageCoord.lY = dY * DeltaY;
			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			StageMoveRelative(&stageCoord);
			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			Delay(STAGE_MOVE_DELAY);		   // wait long enough for stage to settle down

			// retry the match at this new site
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//			  DoPatternRecMatch(pSitePtResult, pMatchLoc, m_pSitePattern->PatternInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
			DoPatternRecMatch(pSitePtResult, pMatchLoc, m_SprSubInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */

			GropStep++;
			sprintf(szLogText,"Pattern Match : Score:%3.2f%%  Target:%3.2f%%  Grop Position %d\0", pSitePtResult->Score, pSitePtResult->TargetScore, GropStep);
			SitePtLogging(szLogText);

			//Saiki 20090602 Change ----->
			//sprintf(szLogText, IDS_CHIF_SITEPR_AUTO_ING "Search Result %3.2f%%  (Best %3.2f%%)\0", pSitePtResult->Score, bestResult.Score);
			LoadStringML(IDS_CHIF_SITEPR_AUTO_ING, strMsg1, "Site Pattern:Auto Site Pattern Executing...");
			LoadStringML(IDS_SEARCH_RESULT, strMsg2, "Search Result %3.2f%%  (Best %3.2f%%)");
			strBuffer = strMsg1 + strMsg2;
			sprintf(szLogText, strBuffer, pSitePtResult->Score, bestResult.Score);
			//Saiki 20090602 Change <-----
			((CMainFrame*) ((m_pChiefView)->m_pcMainFrame))->SetMessageText(szLogText);	// Status Bar

			if ( pSitePtResult->Score > bestResult.Score ) {
				// update the best score so far
				bestResult = *pSitePtResult;
				bestMatchLoc = *pMatchLoc;
			}
		}
	}

SUCCESS:

	// update the score and match location with the best or the first score > 55
	*pSitePtResult = bestResult;
	*pMatchLoc = bestMatchLoc;

	// if pattern rec is OK, we want to move the matched location to the center
	// and do another match to minimize the error caused by video calibration factor.
	if( pSitePtResult->Score >= m_patternRec.dLowestMatchScore ) // pat rec is OK
	{
		const int nMaxRetries = 2;
		int nRetries = nMaxRetries;
		while ( nRetries-- > 0 ) {
			if ( m_bShutDown )
				return;

			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			NS_StageMoveAbsolute(pMatchLoc);
			((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			Delay(STAGE_MOVE_DELAY);

			// retry the match at this new site
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//			DoPatternRecMatch(pSitePtResult, pMatchLoc, m_pSitePattern->PatternInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
			DoPatternRecMatch(pSitePtResult, pMatchLoc, m_SprSubInfo[iSite].iLens);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */

			sprintf(szLogText,"Pattern Match : Score:%3.2f%%  Target:%3.2f%%  Last Adjust %d\0", pSitePtResult->Score, pSitePtResult->TargetScore, nRetries);
			SitePtLogging(szLogText);

			//Saiki 20090602 Change ----->
			//sprintf(szLogText, IDS_CHIF_SITEPR_AUTO_ING "Last Result %3.2f%%  (Best %3.2f%%)\0", pSitePtResult->Score, bestResult.Score);
			LoadStringML(IDS_CHIF_SITEPR_AUTO_ING, strMsg1, "Site Pattern:Auto Site Pattern Executing...");
			LoadStringML(IDS_LAST_RESULT, strMsg2, "Last Result %3.2f%%  (Best %3.2f%%)");
			strBuffer = strMsg1 + strMsg2;
			sprintf(szLogText, strBuffer, pSitePtResult->Score, bestResult.Score);
			//Saiki 20090602 Change <-----
			((CMainFrame*) ((m_pChiefView)->m_pcMainFrame))->SetMessageText(szLogText);	// Status Bar

			if ( pSitePtResult->Score >= m_patternRec.dStopGropingScore )	break;
		}

		if( pSitePtResult->Score < m_patternRec.dLowestMatchScore ) // pat rec is NG !!! why?
		{
			sprintf(szLogText,"========= Adjust Match NG       : Score:%3.2f%%  Target:%3.2f%%\0", pSitePtResult->Score, pSitePtResult->TargetScore);
			SitePtLogging(szLogText);

			*pSitePtResult = bestResult;
			*pMatchLoc = bestMatchLoc;

			sprintf(szLogText,"          Best Result overwrite : Score:%3.2f%%  Target:%3.2f%%\0", pSitePtResult->Score, pSitePtResult->TargetScore);
			SitePtLogging(szLogText);

		}

/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- { ---------- */
//		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
//		NS_StageMoveAbsolute(pMatchLoc);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ----------			  */
		STAGE_COORD l_OffsetAdd;
		if (0 == iSite) {
			/* �p�^�[���P�̏ꍇ */
			if (0 == _tcslen(m_pSitePattern->PatternInfo[1].szSitePatternName)) {
				/* �p�^�[���Q�̓o�^�������ꍇ */
				l_OffsetAdd.lX = m_SprSubInfo[0].ptOffset.x;
				l_OffsetAdd.lY = m_SprSubInfo[0].ptOffset.y;
				_stprintf(szLogText, _T("SitePR offset : %s, x = %d, y = %d"), m_pSitePattern->PatternInfo[0].szSitePatternName, l_OffsetAdd.lX, l_OffsetAdd.lY);
				SitePtLogging(szLogText);
			} else {
				l_OffsetAdd.lX = 0;
				l_OffsetAdd.lY = 0;
			}
		} else {
			/* �p�^�[���Q�̏ꍇ */
			l_OffsetAdd.lX = m_SprSubInfo[1].ptOffset.x;
			l_OffsetAdd.lY = m_SprSubInfo[1].ptOffset.y;
			_stprintf(szLogText, _T("SitePR offset : %s, x = %d, y = %d"), m_pSitePattern->PatternInfo[1].szSitePatternName, l_OffsetAdd.lX, l_OffsetAdd.lY);
			SitePtLogging(szLogText);
		}
		l_OffsetAdd.lX += pMatchLoc->lX;
		l_OffsetAdd.lY += pMatchLoc->lY;
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		NS_StageMoveAbsolute(&l_OffsetAdd);
/* modified 2009.06.22 hmenjo SPR �I�t�Z�b�g�Ǎ���/�����݋@�\�ǉ� ---------- } ---------- */
		((CNanoSpecDoc*) ((m_pChiefView)->m_pcNanoSpecDoc))->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		Delay(STAGE_MOVE_DELAY);
	}
}


BOOL CSitePtAutoSeq::DoPatternRecMatch(DESKEW_RESULT* pSitePtResult, STAGE_COORD* pMatchLoc, int iLens)
{
	PatRecMatch(pSitePtResult);

	// map center from screen (0, 0) to screen(m_OverlayCenter.x, m_OverlayCenter.y)
	pSitePtResult->XPos -= OVERLAY_CENTER_X;
	pSitePtResult->YPos -= OVERLAY_CENTER_Y;

	pSitePtResult->YPos *= -1;							// now invert the y axis

	PixelToMicron(pSitePtResult->XPos, pSitePtResult->YPos, pMatchLoc, iLens);

// 2013.01.09 bagus -->
//	NS_ConvertToStageGetPosCoord(pMatchLoc);
// 2013.01.09 bagus <--

	return TRUE;
}

// 2013.01.17 bagus rotate xy view -->
// ==========================================================================
//
long CSitePtAutoSeq::RotateXyAxisDir(long lDir)
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

// 2013.01.09 bagus -->
// �N���C�A���g�X�e�[�W���W�iUI��ʗp�j�ɕϊ�
// �J�����́AStageConfig�Őݒ肳�ꂽView�����Őݒu����Ă���
void CSitePtAutoSeq::ToClientXyStageCoord(STAGE_COORD* lpPoint)
{
// 2013.01.17 bagus rotate xy view -->
	long lDirX = RotateXyAxisDir(m_StageConfig.Dir.X);
	long lDirY = RotateXyAxisDir(m_StageConfig.Dir.Y);
// 2013.01.17 bagus rotate xy view <--

	// X: RIGHT
	if ( lDirX == DIR_RIGHT ) {
		switch ( lDirY ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: LEFT
	if ( lDirX == DIR_LEFT ) {
		lpPoint->lX *= -1;
		switch ( lDirY ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: TOP
	if ( lDirX == DIR_TOP ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lX = ptTemp.lY;
		switch ( lDirY ) {
		case DIR_RIGHT:
			lpPoint->lY = ptTemp.lX;
			break;
		case DIR_LEFT:
			lpPoint->lY = ptTemp.lX * -1;
			break;
		default:
			break;
		}
	}

	// X: BOTTOM
	if ( lDirX == DIR_BOTTOM ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lX = ptTemp.lY * -1;
		switch ( lDirY ) {
		case DIR_RIGHT:
			lpPoint->lY = ptTemp.lX;
			break;
		case DIR_LEFT:
			lpPoint->lY = ptTemp.lX * -1;
			break;
		default:
			break;
		}
	}
}
// 2013.01.09 bagus <--

/*---------------------------------------------------------------------------
 *	 PixelToMicron: convert pixels relative to overlay center to stage coordinate
 *
 *	 Description:
 *		Convert the pixels in doubles to stage microns.
 */

void CSitePtAutoSeq::PixelToMicron(double xPos, double yPos, STAGE_COORD* pMatchLoc, int iLens)
{
// 2013.01.09 bagus -->
// 	// get the current coordinate of the chuck
// 	STAGE_COORD MatchCtr;
// 	StageGetPos(&MatchCtr);
//
// 	// calculate the absolute position of each point on stage
// 	double x = xPos * m_SrVideoCalib[iLens].dXPixelToMicron;
// 	double y = yPos * m_SrVideoCalib[iLens].dYPixelToMicron;
// 	pMatchLoc->lX = long( x >= 0 ? x + 0.5 : x - 0.5 ) + MatchCtr.lX;
// 	pMatchLoc->lY = long( y >= 0 ? y + 0.5 : y - 0.5 ) + MatchCtr.lY;

	// get the current coordinate of the chuck
	STAGE_COORD MatchCtr;
	NS_StageGetPos(&MatchCtr);

	// calculate the absolute position of each point on stage
	double x = xPos * m_SrVideoCalib[iLens].dXPixelToMicron;
	double y = yPos * m_SrVideoCalib[iLens].dYPixelToMicron;

	STAGE_COORD XyViewNsClientCoord;

	XyViewNsClientCoord.lX = long( x >= 0 ? x + 0.5 : x - 0.5 );
	XyViewNsClientCoord.lY = long( y >= 0 ? y + 0.5 : y - 0.5 );

	ToClientXyStageCoord(&XyViewNsClientCoord);

	pMatchLoc->lX = XyViewNsClientCoord.lX + MatchCtr.lX;
	pMatchLoc->lY = XyViewNsClientCoord.lY + MatchCtr.lY;
// 2013.01.09 bagus <--
}

/////////////////////////////////////////////////////////////////////////////
//
void CSitePtAutoSeq::ShutDown()
{
	m_bShutDown = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CSitePtAutoSeq::AddTime(char* lpszBuff)
{
	SYSTEMTIME csTime;

	::GetLocalTime(&csTime);
	sprintf(lpszBuff, "%04d.%02d.%02d %02d:%02d:%02d.%03d",
		csTime.wYear, csTime.wMonth, csTime.wDay,
		csTime.wHour, csTime.wMinute, csTime.wSecond,
		csTime.wMilliseconds);
}

/////////////////////////////////////////////////////////////////////////////
//
void CSitePtAutoSeq::SitePtLogging(TCHAR* pszLogText)
{
	char l_szBuff[1024];

	AddTime(l_szBuff);
	_tcscat(l_szBuff, " ");
	_tcscat(l_szBuff, pszLogText);

	m_SitePtLogFile.Logging(l_szBuff);
}



/////////////////////////////////////////////////////////////////////////////
// CSitePtManualSeq

IMPLEMENT_DYNCREATE(CSitePtManualSeq, CSitePtSeq)

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSitePtManualSeq::InitInstance()
{
	/* delete this ���܂�	*/
	m_bAutoDelete = TRUE;

	m_bShutDown = FALSE;

	::ZeroMemory(MatchLoc, sizeof(MatchLoc));

	m_pMainWnd = new CSitePtManualSeqWnd();
	m_pMainWnd->CreateEx(0, AfxRegisterWndClass(0), SITEPT_MANUAL_SEQ_WINDOW_NAME, WS_OVERLAPPEDWINDOW, CRect(0, 0, 0, 0), NULL, 0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
int CSitePtManualSeq::ExitInstance()
{
	TRACE(_T("CSitePtManualSeq::ExitInstance()\n"));

	m_pChiefTransiSr1Point->TransiEvent(m_iEvent, 0L);

	return CSitePtSeq::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CSitePtManualSeq, CSitePtSeq)
	//{{AFX_MSG_MAP(CSitePtManualSeq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSitePtManualSeq message handlers

/////////////////////////////////////////////////////////////////////////////
//
void CSitePtManualSeq::ShutDown()
{
	m_bShutDown = TRUE;
//	m_pMainWnd->PostMessage(WM_SITEPTSEQ_SITEPTMANUAL_CANCEL);
}

