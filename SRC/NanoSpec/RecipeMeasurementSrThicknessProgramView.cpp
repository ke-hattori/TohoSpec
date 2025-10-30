// RecipeMeasurementSrThicknessProgramView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "TestMeasurementDlg.h"
#include "RecipeMeasurementSrThicknessProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementSrThicknessProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementSrThicknessProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMeasurementSrThicknessProgramView::CRecipeMeasurementSrThicknessProgramView()
	: CNanoRecipeUI(CRecipeMeasurementSrThicknessProgramView::IDD)
{
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SR_THICKNESS, l_strBuffer, "SR THICKNESS");
	m_strCaption = l_strBuffer;

	m_strLens = _T("");

	//{{AFX_DATA_INIT(CRecipeMeasurementSrThicknessProgramView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementSrThicknessProgramView::~CRecipeMeasurementSrThicknessProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementSrThicknessProgramView)
	DDX_Control(pDX, IDC_XMP_MEASUREMENT_ITEM_LIST, m_XmpMeasurementItemListCtrl);
	DDX_Control(pDX, IDC_TEST_BUTTON, m_TestButton);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_THICKNESS_PROG_NAME, m_ThickMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_SR_THICK_STRATEGY, m_ThickMeas.ScanParams._SR.XMPDesc.szStrategyEntry, STRATEGY_NAME_LEN + 1);
	DDX_Text(pDX, IDC_MEAS_THICKNESS_INTEGRATION_TIME, m_ThickMeas.ScanParams._SR.dIntegTime);
	DDX_CBStringExact(pDX, IDC_SR_THICK_LENS, m_strLens);
	DDX_CBStringExact(pDX, IDC_SR_THICK_OPT_FILTER, m_strOptFilter);
	DDX_Check(pDX, IDC_ENHANCED_UV, m_ThickMeas.ScanParams._SR.bEnhancedUv);
	DDX_Text(pDX, IDC_START_WAVELENGTH, (short&)m_ThickMeas.ScanParams._SR.WavelenRange.wStart);
	DDX_Text(pDX, IDC_END_WAVELENGTH, (short&)m_ThickMeas.ScanParams._SR.WavelenRange.wEnd);
	DDX_Check(pDX, IDC_ADD_REFLECTANCE_MEASUREMENT, m_ThickMeas.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG1, m_strSpecificWavelen[0]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG2, m_strSpecificWavelen[1]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG3, m_strSpecificWavelen[2]);
	DDX_Check(pDX, IDC_SR_THICK_REF_LIFETIME_CHECK, m_ThickMeas.Ref.hdr.bWarnFlag);
	DDX_Text(pDX, IDC_SR_THICK_REF_LIFETIME, m_ThickMeas.Ref.hdr.dLifeTime);
	DDX_Text(pDX, IDC_SR_THICK_REF_MATERIAL, m_ThickMeas.Ref.OptDesc.szRefMaterial, _MAX_PATH + 1);
	DDX_Text(pDX, IDC_COMMENT, m_ThickMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
/* added 2015.06.01 hmenjo XMP �\��/��\���Q ---------- { ---------- */
	DDX_Control(pDX, IDC_BTN_SHOWXMP, m_BtnShowXmp);
/* added 2015.06.01 hmenjo XMP �\��/��\���Q ---------- } ---------- */
}

BEGIN_MESSAGE_MAP(CRecipeMeasurementSrThicknessProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementSrThicknessProgramView)
	ON_BN_CLICKED(IDC_SR_THICK_REF_LIFETIME_CHECK, OnCheckReferenceLifeTime)
	ON_CBN_SELCHANGE(IDC_SR_THICK_OPT_FILTER, OnSelchangeSrThickOptFilter)
	ON_BN_CLICKED(IDC_ADD_REFLECTANCE_MEASUREMENT, OnAddReflectanceMeasurement)
	ON_CBN_SELCHANGE(IDC_SR_THICK_STRATEGY, OnSelchangeSrThickStrategy)
	ON_CBN_SELCHANGE(IDC_SR_THICK_LENS, OnSelchangeSrThickLens)
	ON_BN_CLICKED(IDC_TEST_BUTTON, OnTestButton)
/* added 2015.06.01 hmenjo XMP �\��/��\���Q ---------- { ---------- */
	ON_BN_CLICKED(IDC_BTN_SHOWXMP, OnBtnShowXmp)
	ON_WM_TIMER()
/* added 2015.06.01 hmenjo XMP �\��/��\���Q ---------- } ---------- */
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CRecipeMeasurementSrThicknessProgramView ���b�Z�[�W �n���h��
// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::OnInitialUpdate()
{
	CString l_strBuffer, l_strTitle;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	ConfigFile_GetNanoSpecIni(m_srFilter, CONFIG_FILE_SR_FILTER);
	LoadRecipeData();

	m_strLens = m_SrTurret[m_ThickMeas.ScanParams._SR.iLens].szName;

	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// �쐬�Ɏ��s
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, 20, SWP_NOMOVE);

	///// Static Control /////
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_THICKNESS_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// Control Initialize /////
	InitCombo_SrThickStrategy();
	InitCombo_SrThickReferenceMaterial();
	InitCombo_SrThickOpticalFilter();
	InitCombo_Lens();
	if ( !m_SrConfig.bUv )
		GetDlgItem(IDC_ENHANCED_UV)->ShowWindow(SW_HIDE);

// 2013.11.11 Bagus Mod (TohoSpec�Ή�) -->
//	OnCheckReferenceLifeTime();
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_SR_THICK_REF_LIFETIME_CHECK)->ShowWindow(SW_HIDE);
	}
	else{
// bagus add 2014.01.10 (Stage None�Ή�) -->
//		OnCheckReferenceLifeTime();

		SYSTEM_CONFIG l_SystemConfig;
		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

		if(l_SystemConfig.nStageType == STAGE_TYPE_NONE){
			m_ThickMeas.Ref.hdr.bWarnFlag = FALSE;
			UpdateData(FALSE);
			GetDlgItem(IDC_SR_THICK_REF_LIFETIME_CHECK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_SR_THICK_REF_LIFETIME)->EnableWindow(TRUE);
		}
		else{
			OnCheckReferenceLifeTime();
		}
// bagus add 2014.01.10 (Stage None�Ή�) <--
	}
// 2013.11.11 Bagus Mod (TohoSpec�Ή�) <--
	OnSelchangeSrThickOptFilter();

	/// Test Button ///
	m_TestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TestButton.DrawFlatFocus(TRUE);

	///// ���蔽�˗��g���ݒ� /////
	int IDC_MEAS_REFL_WAVE_LENG[] = {IDC_MEAS_REFL_WAVE_LENG1, IDC_MEAS_REFL_WAVE_LENG2, IDC_MEAS_REFL_WAVE_LENG3};
	for ( int i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
		if (m_ThickMeas.ScanParams._SR.iSpecificWavelen[i] != 0)
			m_strSpecificWavelen[i].Format("%d", m_ThickMeas.ScanParams._SR.iSpecificWavelen[i]);
		else
			m_strSpecificWavelen[i].Empty();

		SetDlgItemText(IDC_MEAS_REFL_WAVE_LENG[i], m_strSpecificWavelen[i]);
	}

	// �R�����g����������
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_THICKNESS_PROG_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_THICKNESS_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT)->ShowWindow(SW_HIDE);
		//Saiki 20090410 Add ----->
		GetDlgItem(IDC_TEST_BUTTON)->ShowWindow(SW_HIDE);
		//Saiki 20090410 Add <-----
	}

	InitItemList();
	OnSelchangeSrThickStrategy();
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);
	OnAddReflectanceMeasurement();

	if ((IsDataChanged() == TRUE) && (m_nOpenMode == modeNormal)) {
		//�X�g���e�W�B�������������ꍇ�A
		//���V�s��Ǎ��񂾒l�Ɖ�ʕ\���l(���w�t�B���^�E�g��)�ɍ����łĂ��܂����Ƃ�����̂ŁA���V�s�������I�ɏ�������B
		SaveRecipeData();
		LoadStringML(IDS_SAVE_OPTICAL_FILTER_WAVELENG, l_strBuffer, "The optical filter and the wavelength were saved\n by the change in the strategy.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
	}
/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
	RECT l_rectThisView;	this->GetWindowRect(&l_rectThisView);
	RECT l_rectComment;	this->GetDlgItem(IDC_COMMENT)->GetWindowRect(&l_rectComment);
	RECT l_rectThisDlg = {0, 0, 605, 551};
	((CDialog*) this)->MapDialogRect(&l_rectThisDlg);
	long l_lViewBottom = l_rectThisView.top + (l_rectThisDlg.bottom - l_rectThisDlg.top + 1);
	long l_lSpaceCommentBottom = l_lViewBottom - l_rectComment.bottom;
	if ((g_lModelType == MODEL_T3100) && (l_rectThisView.bottom <= (l_rectComment.bottom + l_lSpaceCommentBottom))) {
		long l_ldeltaY = l_rectThisView.bottom - l_lSpaceCommentBottom - (l_rectComment.bottom - l_rectComment.top);
		l_ldeltaY = l_rectComment.top - l_ldeltaY;
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
		POINT l_ptStart = {0, 0};
		/* �R�����g����	*/
		RECT l_rectCommentStatic;	this->GetDlgItem(IDC_COMMENT_STATIC)->GetWindowRect(&l_rectCommentStatic);
		l_ptStart.x = l_rectCommentStatic.left;
		l_ptStart.y = l_rectCommentStatic.top - l_ldeltaY;
		this->ScreenToClient(&l_ptStart);
		this->GetDlgItem(IDC_COMMENT_STATIC)->SetWindowPos(0, l_ptStart.x, l_ptStart.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		/* �R�����g��	*/
		l_ptStart.x = l_rectComment.left;
		l_ptStart.y = l_rectComment.top - l_ldeltaY;
		this->ScreenToClient(&l_ptStart);
		this->GetDlgItem(IDC_COMMENT)->SetWindowPos(0, l_ptStart.x, l_ptStart.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		/* ����g���F�O���[�v�{�b�N�X	*/
		RECT l_rectWaveLenStatic = {145, 300, 135, 70};
//		RECT l_rectWaveLenStatic = {50, 323, 140, 62};
		((CDialog*) this)->MapDialogRect(&l_rectWaveLenStatic);
		this->GetDlgItem(IDC_STATIC_WAVELEN)->SetWindowPos(0,
											l_rectWaveLenStatic.left, l_rectWaveLenStatic.top,
											l_rectWaveLenStatic.right, l_rectWaveLenStatic.bottom,
											SWP_NOZORDER);
		/* �R���g���[�����ړ�	*/
		struct {
			RECT	rectCnvPos;
			int		iID;
		} l_CnvPosId[] = {
				{{ 150, 310,  85,  12}, IDC_STATIC_WAVELEN1},
				{{ 150, 330,  85,  12}, IDC_STATIC_WAVELEN2},
				{{ 150, 350,  85,  12}, IDC_STATIC_WAVELEN3},
				{{ 240, 310,  35,  12}, IDC_MEAS_REFL_WAVE_LENG1},
				{{ 240, 330,  35,  12}, IDC_MEAS_REFL_WAVE_LENG2},
				{{ 240, 350,  35,  12}, IDC_MEAS_REFL_WAVE_LENG3},
				{{  10, 335,  50,  15}, IDC_TEST_BUTTON},
//				{{  55, 335,  85,  12}, IDC_STATIC_WAVELEN1},
//				{{  55, 350,  85,  12}, IDC_STATIC_WAVELEN2},
//				{{  55, 365,  85,  12}, IDC_STATIC_WAVELEN3},
//				{{ 145, 335,  35,  12}, IDC_MEAS_REFL_WAVE_LENG1},
//				{{ 145, 350,  35,  12}, IDC_MEAS_REFL_WAVE_LENG2},
//				{{ 145, 365,  35,  12}, IDC_MEAS_REFL_WAVE_LENG3},
//				{{ 210, 360,  50,  15}, IDC_TEST_BUTTON},
				{{0, 0, 0, 0}, 0},
			};
		for (int i = 0; 0 != l_CnvPosId[i].iID; i++) {
			((CDialog*) this)->MapDialogRect(&l_CnvPosId[i].rectCnvPos);
			this->GetDlgItem(l_CnvPosId[i].iID)->SetWindowPos(0,
							l_CnvPosId[i].rectCnvPos.left,
							l_CnvPosId[i].rectCnvPos.top,
							0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
	}
/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */
/* added 2015.06.01 hmenjo XMP �\��/��\���Q ---------- { ---------- */
	if (1 == g_lXmpType) {
		BOOL l_bTextShow = TRUE;
		HWND l_hWnd = ::FindWindow(0, ADAP_WINDOW_NAME);
		if (0 != l_hWnd) {
			if (FALSE != ::IsWindowVisible(l_hWnd)) {
				l_bTextShow = FALSE;
			}
		}
		m_BtnShowXmp.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_BtnShowXmp.DrawFlatFocus(TRUE);
		this->SetDlgItemText(IDC_BTN_SHOWXMP, (FALSE != l_bTextShow)? _T("Show Xmp") : _T("Hide Xmp"));
		this->GetDlgItem(IDC_BTN_SHOWXMP)->ShowWindow(SW_SHOW);
		this->SetTimer(TMR_CHKXMPSHOW_ID, TMR_CHKXMPSHOW_TIME, 0);
	}
/* added 2015.06.01 hmenjo XMP �\��/��\���Q ---------- } ---------- */
}

// =========================================================================
// ���t�@�����X�^�C�� �`�F�b�N�I����
//
void CRecipeMeasurementSrThicknessProgramView::OnCheckReferenceLifeTime()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_SR_THICK_REF_LIFETIME)->EnableWindow(m_ThickMeas.Ref.hdr.bWarnFlag);
}

// =========================================================================
// �I�v�e�B�J���t�B���^�[ �R���{�I����
//
void CRecipeMeasurementSrThicknessProgramView::OnSelchangeSrThickOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_THICK_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_ThickMeas.ScanParams._SR.wOpticsFilterType = static_cast<WORD>(pCombo->GetItemData(nSelect));
}

// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::LoadRecipeData()
{
	CString l_strBuffer;

	switch ( m_nOpenMode )
	{
	case modeNew:				// ���V�s�V�K�쐬
	case modeDefaultSetting:	// �R���t�B�O��ʂ���J�ڂ��Ă����P�[�X
		if ( !RecipeFile_LoadRecipe(&m_ThickMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_THICKNESS) ) {
			m_ThickMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
			m_ThickMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_THICKNESS;
		}
		break;
	default:					// ���V�s���C�����X�g��ʂ���J�ڂ��Ă����P�[�X
		if ( !RecipeFile_LoadRecipe(&m_ThickMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {			// RECIPE_FILE_MEASUREMENT_PROGRAM �Ő������炵��...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ThickMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_THICKNESS) ) {
				m_ThickMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
				m_ThickMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_THICKNESS;
			}
		}
		break;
	}
	::CopyMemory(&m_OldThickMeas, &m_ThickMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_ThickMeas.hdr.szName, l_strBuffer);
	}
}
// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::InitCombo_SrThickStrategy()
{
	CString l_strBuffer;
// 2009.09.18 K.Matsuo �X�g���e�W�[�̑���^�C�v���� -->
	BOOL bSr, bDummy, bDummy2;
// 2009.09.18 K.Matsuo �X�g���e�W�[�̑���^�C�v���� <--

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_THICK_STRATEGY);

	pCombo->ResetContent();

	SR_XMP SrXmp;
	ConfigFile_GetNanoSpecIni(&SrXmp, CONFIG_FILE_SR_XMP);
	CString strBuffer;

	TRY
	{
		CStdioFile stdioFile(SrXmp.szStrategyListPath, CFile::modeRead);
// 2009.09.18 K.Matsuo �X�g���e�W�[�̑���^�C�v���� -->
		while ( stdioFile.ReadString(strBuffer) ) {
			MEAS_GetStratetyType(bSr, bDummy, bDummy2, strBuffer);
			if ( bSr )
				pCombo->AddString(strBuffer);
		}
// 2009.09.18 K.Matsuo �X�g���e�W�[�̑���^�C�v���� <--
		stdioFile.Close();
	}
	CATCH(CFileException, e)
	{
		CString strCause;
		LoadStringML(IDS_CAUSE_CODE, l_strBuffer, "CauseCode:%d\n%s");
		strCause.Format(l_strBuffer, e->m_cause, e->m_strFileName);

		switch ( e->m_cause )
		{
		case CFileException::fileNotFound :
			LoadStringML(IDS_FILE_NOT_FOUND, l_strBuffer, " FileNotFound");
			strCause += l_strBuffer;
			break;
		case CFileException::badPath :
			LoadStringML(IDS_BAD_PATH, l_strBuffer, " BadPath");
			strCause += l_strBuffer;
			break;
		default :
			break;
		}
		MessageBox(strCause, NULL, MB_ICONERROR | MB_OK);
	}
	END_CATCH

	int nIndex = pCombo->FindStringExact(-1, m_ThickMeas.ScanParams._SR.XMPDesc.szStrategyEntry);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::InitCombo_SrThickReferenceMaterial()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_THICK_REF_MATERIAL);
	pCombo->ResetContent();

	CString strDirPath;
	strDirPath.Format("%s*%s", g_szCfg_System_Sr_Reference_Material_Dir, DAT_EXT);
	CFileFind Finder;
	if ( !Finder.FindFile(strDirPath) )
		return;

	//Saiki 20090410 Change ----->
	//BOOL bLoop;
	BOOL bLoop = TRUE;
	//Saiki 20090410 Change <-----
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		pCombo->AddString(Finder.GetFileTitle());
	}

	int nIndex = pCombo->FindStringExact(0, m_ThickMeas.Ref.OptDesc.szRefMaterial);
	//Saiki 20090406Change ----->
	//pCombo->SetCurSel(nIndex);
	if(!(nIndex == CB_ERR)){
		pCombo->SetCurSel(nIndex);
	}
	else{
		if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
			SaveRecipeData();
			LoadStringML(IDS_REFER_STRATEGY_ZERO, l_strBuffer, "Invalid for the specified item, Reference strategy is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
	//Saiki 20090406 Change <----
}

// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::InitCombo_SrThickOpticalFilter()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_THICK_OPT_FILTER);

// 2013.12.18 Bagus Add (TohoSpec�Ή�) -->
	BOOL bUseFilter;

	if(g_lModelType != MODEL_T3100){
		bUseFilter = m_SrConfig.bAutoFilter;	//�I�[�g�t�B���^�̃R���t�B�O�ݒ�̓��e
	}
	else{
		bUseFilter = TRUE;	// T3100�ł́A�t�B���^�ݒ����Ɏg�p�i�\���j����
	}
// 2013.12.18 Bagus Add (TohoSpec�Ή�) <--

// 2013.12.18 Bagus Mod (TohoSpec�Ή�) -->
// 	//�I�[�g�t�B���^����̏ꍇ
// 	if (m_SrConfig.bAutoFilter){
	// �t�B���^�ݒ���g�p����ꍇ
	if (bUseFilter){
// 2013.12.18 Bagus Mod (TohoSpec�Ή�) <--
// 2009.12.08 K.Matsuo -->
//		pCombo->EnableWindow(FALSE);
// 2009.12.08 K.Matsuo <--
		pCombo->ResetContent();

		int nIndex;
		int iFilterIndex;

		SYSTEM_CONFIG l_SystemConfig;
		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

		LPCTSTR pszFilterName[LANGUAGE_MAX][OPT_FILTER_MAX] = {
			{OPTICAL_FILTER_TYPE_ITEM_ENU[0], OPTICAL_FILTER_TYPE_ITEM_ENU[1], OPTICAL_FILTER_TYPE_ITEM_ENU[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Enu
			{OPTICAL_FILTER_TYPE_ITEM_JPN[0], OPTICAL_FILTER_TYPE_ITEM_JPN[1], OPTICAL_FILTER_TYPE_ITEM_JPN[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Jpn
		};

		for ( int iOptItem = 0; iOptItem < OPT_FILTER_MAX; iOptItem++ ) {
			switch ( iOptItem ) {
			case OPT_FILTER_UNKNOWN:
				break;
			case OPT_FILTER_OPEN:
				nIndex = pCombo->AddString(pszFilterName[l_SystemConfig.nLanguage][iOptItem]);
				pCombo->SetItemData(nIndex, iOptItem);
				break;
			case OPT_FILTER_DARK:
				break;
			case OPT_FILTER_POS1:
			case OPT_FILTER_POS2:
			case OPT_FILTER_POS3:
				iFilterIndex = iOptItem - 3;
				if ( m_srFilter[iFilterIndex].bEnable ) {
					nIndex = pCombo->AddString(m_srFilter[iFilterIndex].szName);
					pCombo->SetItemData(nIndex, iOptItem);
				}
				break;
			default:
				break;
			}
		}

		// 2009.08.25 bagus stress --{--
		//nIndex = pCombo->FindStringExact(-1, pszFilterName[m_ThickMeas.ScanParams._SR.wOpticsFilterType]);
		if ((m_ThickMeas.ScanParams._SR.wOpticsFilterType < 0) || (m_ThickMeas.ScanParams._SR.wOpticsFilterType >= OPT_FILTER_MAX)) {
			nIndex = 0;
		} else {
			nIndex = pCombo->FindStringExact(-1, pszFilterName[l_SystemConfig.nLanguage][m_ThickMeas.ScanParams._SR.wOpticsFilterType]);
		}
		// 2009.08.25 bagus stress --}--

		//Saiki 20090406 Change ----->
		//pCombo->SetCurSel(nIndex);
		if(!(nIndex == CB_ERR)){
			pCombo->SetCurSel(nIndex);
		}
		else{
			if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
				nIndex = 0;
				pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus SR --{--
				m_ThickMeas.ScanParams._SR.wOpticsFilterType = static_cast<WORD>(pCombo->GetItemData(0));
// 2009.12.14 bagus SR --}--
			}
			else{
				nIndex = 0;
				pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus SR --{--
				m_ThickMeas.ScanParams._SR.wOpticsFilterType = static_cast<WORD>(pCombo->GetItemData(0));
// 2009.12.09 bagus SR --}--
				SaveRecipeData();
				LoadStringML(IDS_OPTICAL_FILTER_ZERO, l_strBuffer, "Invalid for the specified item, Optical filter is saved as an INDEX to 0.");
				LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
				MessageBox(l_strBuffer, l_strTitle, MB_OK);
			}
		}
		//Saiki 20090406 Change <-----
	}

	//�I�[�g�t�B���^�Ȃ��̏ꍇ
	else{
		m_ThickMeas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_THICK_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_THICK_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
//�����Y �R���{�C�j�V��������
//
void CRecipeMeasurementSrThicknessProgramView::InitCombo_Lens()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_THICK_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
// 2009.11.24 bagus SR �C�� --{--
//		if ( m_SrTurret[i].bEnable ) {
		if ( m_SrTurret[i].bEnable && m_SrTurret[i].bAnalysis ) {
// 2009.11.24 bagus SR �C�� --}--
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	nIndex = pCombo->FindStringExact(-1, m_strLens);
	//Saiki 20090406 Change ----->
	//pCombo->SetCurSel(nIndex);
	if(!(nIndex == CB_ERR)){
		pCombo->SetCurSel(nIndex);
	}
	else{
		if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus SR --{--
			m_ThickMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.14 bagus SR --}--
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus SR --{--
			m_ThickMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.09 bagus SR --}--
			SaveRecipeData();
			LoadStringML(IDS_LENS_ZERO, l_strBuffer, "Invalid for the specified item, Lens is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
	//Saiki 20090406 Change <-----
}

// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::InitItemList()
{
	LV_COLUMN listcol;
	const int ItemMax = 2;

	int Width[] = {
		40, 	// (0) Index
		79, 	// (1) Item
	};

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	LPTSTR pszItem[LANGUAGE_MAX][ItemMax] = {
		{"Index",	"Item",		},		// Enu
		{"Index",	"�A�C�e��",	},		// Jpn
	};

	int Fmt[] = { LVCFMT_CENTER, LVCFMT_CENTER, };

	m_XmpMeasurementItemListCtrl.SetViewStyle();

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		listcol.pszText = pszItem[l_SystemConfig.nLanguage][i];
		m_XmpMeasurementItemListCtrl.InsertColumn(i,&listcol);
	}

	m_XmpMeasurementItemListCtrl.DeleteAllItems();
}

// =========================================================================
//
BOOL CRecipeMeasurementSrThicknessProgramView::CheckData()
{
	CString l_strBuffer, l_strTitle;

	//2010.01.15 bagus --{--
	//�`�F�b�N�ł͂������ꍇ�ɂ͎��ɐi�܂Ȃ�
	//UpdateData(TRUE);
	if(!UpdateData(TRUE)){
		return FALSE;
	}
	//2010.01.15 bagus --}--

	CString strBuffer;
	CString strCaption;
	int MinWaveLength;
// 2009.05.15 k-matsuo -->
	int MaxWaveLength;
// 2009.05.15 k-matsuo <--
	int iValue;
	BOOL bValidWaveLen = FALSE;
	const WORD wStart = m_ThickMeas.ScanParams._SR.WavelenRange.wStart;
	const WORD wEnd = m_ThickMeas.ScanParams._SR.WavelenRange.wEnd;
	int i;
	int j;

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		LoadStringML(IDS_CAPTION_DEF_SETTING, l_strBuffer, "DEFAULT SETTING - ");
		strCaption = l_strBuffer;
		break;
	default:
		LoadStringML(IDS_CAPTION_MEASURE_PROG, l_strBuffer, "MEASUREMET PROGRAM - ");
		strCaption = l_strBuffer;
		break;
	}
	strCaption += m_strCaption;

	///// Strategy /////
	if ( strlen(m_ThickMeas.ScanParams._SR.XMPDesc.szStrategyEntry) == 0 )
	{
		LoadStringML(IDS_STRATEGY_EMPTY, l_strBuffer, "Strategy is empty.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	///// Integration Time /////
	if ( m_ThickMeas.ScanParams._SR.dIntegTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_ThickMeas.ScanParams._SR.dIntegTime )
	{
		LoadStringML(IDS_INTEGRATIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.3f - %.3f)");
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_MEAS_THICKNESS_INTEGRATION_TIME)->SetFocus();
		return FALSE;
	}

	///// Lens /////
	if ( m_strLens.IsEmpty() ) {
		LoadStringML(IDS_SELECT_LENS, l_strBuffer, "Please Select Lens.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_SR_THICK_LENS)->SetFocus();
		return FALSE;
	}

	///// Optical Filter /////
	
	if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH512 ) {
		// �W���w�b�h�w�莞�́ANDFilter���p����ƁA���ʂ������邽�߁A�g�p���Ȃ��悤�x��
		if ( m_ThickMeas.ScanParams._SR.wOpticsFilterType == OPT_FILTER_POS2 ||
			  m_ThickMeas.ScanParams._SR.wOpticsFilterType == OPT_FILTER_POS3)
		{
			LoadStringML(IDS_NOMALHEAD_FILTER, l_strBuffer, "If LAH512 head, select Yellow Filter or Open Necessary.");
			MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_SR_THICK_OPT_FILTER)->SetFocus();
			return FALSE;
		}
	} else if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS ) {
		// ������\�w�b�h�̓����Y�{���ɂ��A�g�p�t�B���^�𐧌�����悤�x��
		if ( m_strLens == "5 X" || m_strLens == "10 X" ) {
			if ( m_ThickMeas.ScanParams._SR.wOpticsFilterType != OPT_FILTER_POS2 &&
				 m_ThickMeas.ScanParams._SR.wOpticsFilterType != OPT_FILTER_POS3)
			{
				LoadStringML(IDS_HIGHRESO_HEAD_LOWMAGNI_LENS_FILTER, l_strBuffer, "If Select 5X or 10X Lens, select ND30 Filter or ND + Yellow Filter Necessary.");
				MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
				GetDlgItem(IDC_SR_THICK_OPT_FILTER)->SetFocus();
				return FALSE;
			}
		} else {
			if ( m_ThickMeas.ScanParams._SR.wOpticsFilterType == OPT_FILTER_POS2 ||
				 m_ThickMeas.ScanParams._SR.wOpticsFilterType == OPT_FILTER_POS3)
			{
				LoadStringML(IDS_HIGHRESO_HEAD_HIGHMAGNI_LENS_FILTER, l_strBuffer, "If Select 50X or 100X Lens, select Yellow Filter or Open Necessary.");
				MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
				GetDlgItem(IDC_SR_THICK_OPT_FILTER)->SetFocus();
				return FALSE;
			}
		}
	}

	///// Wave Length /////
	if ( wStart >= wEnd ) {
		LoadStringML(IDS_START_WAVE_LARGER, l_strBuffer, "Start Wavelength is Larger than End Wavelength.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_WAVELENGTH_START)->SetFocus();
		return FALSE;
	}

	if ( m_ThickMeas.ScanParams._SR.bEnhancedUv )
		MinWaveLength = MIN_ENHANCED_UV_WAVELENGTH;
	else
		MinWaveLength = MIN_TARGET_WAVELENGTH;

// 2009.05.15 k-matsuo -->
	if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
		MaxWaveLength = MAX_TARGET_WAVELENGTH;
	else
		MaxWaveLength = MAX_TARGET_WAVELENGTH_800;
// 2009.05.15 k-matsuo <--


// 2009.05.15 k-matsuo change -->
//	if ( wStart < MinWaveLength || MAX_TARGET_WAVELENGTH < wStart )
	if ( wStart < MinWaveLength || MaxWaveLength < wStart ) {
		LoadStringML(IDS_START_WAVE_OUT_RANGE, l_strBuffer, "Start Wavelength is out of range.""\n(%d - %d)");
		strBuffer.Format(l_strBuffer, MinWaveLength, MaxWaveLength);
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_WAVELENGTH_START)->SetFocus();
		return FALSE;
	}

//	if ( wEnd < MinWaveLength || MAX_TARGET_WAVELENGTH < wEnd )
	if ( wEnd < MinWaveLength || MaxWaveLength < wEnd )	{
		LoadStringML(IDS_END_WAVE_OUT_RANGE, l_strBuffer, "End Wavelength is out of range.""\n(%d - %d)");
		strBuffer.Format(l_strBuffer, MinWaveLength, MaxWaveLength);
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_WAVELENGTH_END)->SetFocus();
		return FALSE;
	}
// 2009.05.15 k-matsuo change <--

	///// ���蔽�˗��g���ݒ� /////
	///// Reflectance Measurement /////
	///// 1st Target WaveLength /////
	///// 2nd Target WaveLength /////
	///// 3rd Target WaveLength /////
	int IDC_MEAS_REFL_WAVE_LENG[] = {IDC_MEAS_REFL_WAVE_LENG1, IDC_MEAS_REFL_WAVE_LENG2, IDC_MEAS_REFL_WAVE_LENG3};
	int IDS_WAVELENGTH[] = {IDS_FIRST_WAVE_OUT_RANGE, IDS_SECOND_WAVE_OUT_RANGE, IDS_THIRD_WAVE_OUT_RANGE};
	LPCTSTR pszWaveLengthString[] = {"1st Target WaveLength is out of range.\n(%d - %d)", "2nd Target WaveLength is out of range.\n(%d - %d)", "3rd Target WaveLength is out of range.\n(%d - %d)"};

	if (m_ThickMeas.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag) {
		//�R�̐ݒ�l�̏�ʂ����ݒ�̏ꍇ�͏�ʂɋl�߂�B
		CStringArray strSpecificWavelen;
		strSpecificWavelen.SetSize(SPECIFIED_WAVELENGTH_MAX);
		for ( i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
			strSpecificWavelen[i] = m_strSpecificWavelen[i];
			strSpecificWavelen[i].TrimLeft();
			strSpecificWavelen[i].TrimRight();
		}

		// ��ʂɋl�߂�
		j = 0;
		for ( i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
			if ( !strSpecificWavelen[i].IsEmpty() )
				m_strSpecificWavelen[j++] = strSpecificWavelen[i];
		}
		// CleanUp
		for ( ; j < SPECIFIED_WAVELENGTH_MAX; j++ ) {
			m_strSpecificWavelen[j].Empty();
		}
		for ( i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
			SetDlgItemText(IDC_MEAS_REFL_WAVE_LENG[i], m_strSpecificWavelen[i]);
		}

		// Check
		for ( i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
			if ( !m_strSpecificWavelen[i].IsEmpty() )	{
				iValue = atoi(m_strSpecificWavelen[i]);
				if ((!CheckTransData(iValue,m_strSpecificWavelen[i])) || (iValue < wStart) || (wEnd < iValue )) { /**�����ŁA�^�[�Q�b�g���L���͈͂ɐݒ肳��Ă��邩�H*/
					if(CheckTransData(iValue,m_strSpecificWavelen[i])) {
						LoadStringML(IDS_WAVELENGTH[i], l_strBuffer, pszWaveLengthString[i]);
						strBuffer.Format(l_strBuffer, wStart, wEnd );
					}
					else {
						LoadStringML(IDS_SET_INTERVAL_NM, l_strBuffer, "Please set it at intervals of 1 nm.");
						strBuffer.Format(l_strBuffer);
					}
					MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
					GetDlgItem(IDC_MEAS_REFL_WAVE_LENG[i])->SetFocus();
					return FALSE;
				}
				else {
					bValidWaveLen = TRUE;
				}
			}
		}

		if ( !bValidWaveLen ) {
			LoadStringML(IDS_INPUT_WAVELENGTH, l_strBuffer, "Please Input at Least One or More Wavelength.");
			MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
		else{
			for ( int i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
				m_ThickMeas.ScanParams._SR.iSpecificWavelen[i] = atoi(m_strSpecificWavelen[i]);
			}
		}
	}

	///// Life Time /////
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	if(g_lModelType == MODEL_T3100){
		if ( m_ThickMeas.Ref.hdr.dLifeTime < MIN_LIFE_TIME || MAX_LIFE_TIME < m_ThickMeas.Ref.hdr.dLifeTime ) {
			LoadStringML(IDS_LIFETIME_OUT_RANGE, l_strBuffer, "Life Time is out of range.""\n(%.1f - %.1f)");
			strBuffer.Format(l_strBuffer, MIN_LIFE_TIME, MAX_LIFE_TIME);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_SR_THICK_REF_LIFETIME)->SetFocus();
			return FALSE;
		}
		if((m_ThickMeas.Ref.hdr.dLifeTime - (int)m_ThickMeas.Ref.hdr.dLifeTime) != 0) { //�����łȂ�
			LoadStringML(IDS_SET_INTERVAL_MIN, l_strBuffer, "Please set it at intervals of 1 min.");
			strBuffer.Format(l_strBuffer);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_SR_THICK_REF_LIFETIME)->SetFocus();
			return FALSE;
		}
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
		if ( m_ThickMeas.Ref.hdr.bWarnFlag ) {
			if ( m_ThickMeas.Ref.hdr.dLifeTime < MIN_LIFE_TIME || MAX_LIFE_TIME < m_ThickMeas.Ref.hdr.dLifeTime ) {
				LoadStringML(IDS_LIFETIME_OUT_RANGE, l_strBuffer, "Life Time is out of range.""\n(%.1f - %.1f)");
				strBuffer.Format(l_strBuffer, MIN_LIFE_TIME, MAX_LIFE_TIME);
				MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
				GetDlgItem(IDC_SR_THICK_REF_LIFETIME)->SetFocus();
				return FALSE;
			}
			if((m_ThickMeas.Ref.hdr.dLifeTime - (int)m_ThickMeas.Ref.hdr.dLifeTime) != 0) { //�����łȂ�
				LoadStringML(IDS_SET_INTERVAL_MIN, l_strBuffer, "Please set it at intervals of 1 min.");
				strBuffer.Format(l_strBuffer);
				MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
				GetDlgItem(IDC_SR_THICK_REF_LIFETIME)->SetFocus();
				return FALSE;
			}
		}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

	///// Reference Strategy /////
	if ( strlen(m_ThickMeas.Ref.OptDesc.szRefMaterial) == 0 ) {
		LoadStringML(IDS_REFER_STRATEGY_EMPTY, l_strBuffer, "Reference Strategy is empty.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_SR_THICK_REF_MATERIAL)->SetFocus();
		return FALSE;
	}

	if (m_ThickMeas.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag == FALSE) { //AddReflectanceMeasureFlag
		m_ThickMeas.ScanParams._SR.iSpecificWavelen[0] = 0; //FirstWaveLength
		m_ThickMeas.ScanParams._SR.iSpecificWavelen[1] = 0; //SecondWaveLength
		m_ThickMeas.ScanParams._SR.iSpecificWavelen[2] = 0; //ThirdWaveLength
	}


	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrThicknessProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldThickMeas, &m_ThickMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrThicknessProgramView::SaveRecipeData()
{
// 2009.10.09 K.Matsuo Delete -->
// 2009.09.19 bagus SE --{--
//	SR_CONFIG	SrConfig;

//	memset(&SrConfig, 0, sizeof(SrConfig));
//	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

//	m_ThickMeas.ScanParams._SR.iLens = SrConfig.iDefaultMeasLensNo;
//	m_ThickMeas.ScanParams._SR.wOpticsFilterType = SrConfig.wDefaultMeasFilter;
// 2009.09.19 bagus SE --}--
// 2009.10.09 K.Matsuo Delete <--

// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	if(g_lModelType == MODEL_T3100){
		m_ThickMeas.Ref.hdr.bWarnFlag = FALSE;
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_ThickMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_THICKNESS) )
			return FALSE;
		break;
	default:
		// �V�������V�s����ݒ�iSave, SaveAs���p�j
		strcpy(m_ThickMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ThickMeas, m_szRecipeName, RECIPE_FILE_SR_THICKNESS) )
			return FALSE;
/* added 2015.04.24 hmenjo Ref File �폜1 ---------- { ---------- */
		if ((g_lModelType == MODEL_T3100) &&
			(
				(m_nOpenMode == modeNew) ||
				((m_nOpenMode == modeNormal) && (
							(m_ThickMeas.ScanParams._SR.iLens      != m_OldThickMeas.ScanParams._SR.iLens) ||
							(m_ThickMeas.ScanParams._SR.dIntegTime != m_OldThickMeas.ScanParams._SR.dIntegTime)
						))
			)
		) {
			/*	�V�K(modeNew)�ł͖������ɍ폜
				�ʏ�(modeNormal)�ł́C�����Y or �~�ώ���(�I������)���ύX����Ă���	*/
			CNanoRecipeUI::DelRefFileByUseMeasPgm(m_ThickMeas.hdr.szName);
		}
/* added 2015.04.24 hmenjo Ref File �폜1 ---------- } ---------- */
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldThickMeas, &m_ThickMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrThicknessProgramView::IsAccessPrivilege()
{
	int iAccessItem;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( m_nOpenMode == modeDefaultSetting )
		iAccessItem = ACCESS_USERSETTING;
	else
		iAccessItem = ACCESS_PROGRAM;

	return pDoc->User_Access(iAccessItem);
}

// =========================================================================
//
int CRecipeMeasurementSrThicknessProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::OnAddReflectanceMeasurement()
{
	UpdateData(TRUE);

	// ���˗�����̒ǉ��ݒ�G�f�B�b�g�{�b�N�X�̗L�������̐؂�ւ�
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG1)->EnableWindow(IsDlgButtonChecked(IDC_ADD_REFLECTANCE_MEASUREMENT) == BST_CHECKED);
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG2)->EnableWindow(IsDlgButtonChecked(IDC_ADD_REFLECTANCE_MEASUREMENT) == BST_CHECKED);
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG3)->EnableWindow(IsDlgButtonChecked(IDC_ADD_REFLECTANCE_MEASUREMENT) == BST_CHECKED);

}

// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::OnSelchangeSrThickStrategy()
{
	UpdateData(TRUE);

	TCHAR szBuff[256];
	TCHAR szDdeTextItem[1024];
	LPTSTR token;
	int iItem;
	int iStartWaveLen = 400;
	int iEndWaveLen = 800;
	int iOpticalFilter = 0;

	m_XmpMeasurementItemListCtrl.DeleteAllItems();

	if ( !MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iStartWaveLen, iEndWaveLen, iOpticalFilter, m_ThickMeas.ScanParams._SR.XMPDesc.szStrategyEntry) ) {
		// �f�t�H���g�l�ݒ�
		m_ThickMeas.ScanParams._SR.WavelenRange.wStart = iStartWaveLen;
		m_ThickMeas.ScanParams._SR.WavelenRange.wEnd = iEndWaveLen;
		UpdateData(FALSE);
//		m_ThickMeas.ScanParams._SR.wOpticsFilterType = iOpticalFilter;
//		InitCombo_SrThickOpticalFilter();
		return;
	}

	m_ThickMeas.ScanParams._SR.WavelenRange.wStart = iStartWaveLen;
	m_ThickMeas.ScanParams._SR.WavelenRange.wEnd = iEndWaveLen;
	UpdateData(FALSE);
// 2009.12.08 K.Matsuo -->
//	m_ThickMeas.ScanParams._SR.wOpticsFilterType = iOpticalFilter;
//	InitCombo_SrThickOpticalFilter();
// 2009.12.08 K.Matsuo <--

	token = _tcstok(szDdeTextItem, _T(","));
	while ( token ) {
		iItem = m_XmpMeasurementItemListCtrl.GetItemCount();
		_stprintf(szBuff, _T("%d"), iItem + 1);
		m_XmpMeasurementItemListCtrl.InsertItem(iItem, szBuff);

		if(strcmp(token, "MSE") == 0)
		{
			//�wMSE��Fit�x�̒u��
			m_XmpMeasurementItemListCtrl.SetItem(iItem, 1, LVIF_TEXT, /*token*/"Fit", 0, 0, 0, NULL);
		}
		else
		{
			m_XmpMeasurementItemListCtrl.SetItem(iItem, 1, LVIF_TEXT, token, 0, 0, 0, NULL);
		}

		token = _tcstok(NULL, _T(","));
	}
}

// =========================================================================
//
BOOL CRecipeMeasurementSrThicknessProgramView::CheckTransData(int iData , CString strData)
{
	CString strTransiData;

	strTransiData.Format("%d",iData);
	return strTransiData == strData;
}

// =========================================================================
// �����Y �R���{�I����
//
void CRecipeMeasurementSrThicknessProgramView::OnSelchangeSrThickLens()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_THICK_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_ThickMeas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	UpdateData(FALSE);
}

// =========================================================================
//
void CRecipeMeasurementSrThicknessProgramView::OnTestButton()
{

	//�����C�����V�s�쐬
	if(CheckData() == FALSE) //���V�s�f�[�^�̐��������Ƃ�Ă��Ȃ�
	{
		//�G���[���b�Z�[�W��CheckData()���ŏo���B
		return; //�e�X�g���[�h�J�n����
	}
	char szRecipeNameMain[RECIPE_NAME_LEN + 1];
	char szRecipeNameMeas[RECIPE_NAME_LEN + 1];

	if ( !GetTestModeRecipeName(szRecipeNameMain, SAVE_AS_MAIN_RECIPE) )
		return;
	if ( !GetTestModeRecipeName(szRecipeNameMeas, SAVE_AS_MEASUREMENT_PROGRAM) )
		return;

	BOOL bMainRcpMakeFlg = MakeTempSrMainRecipe(szRecipeNameMain, szRecipeNameMeas); //�����C�����V�s�쐬
	BOOL bMeasRcpMakeFlg = MakeTempSrMeasurementRecipe(szRecipeNameMeas); //�����W���[�����g�v���O�����쐬

	//�w���C�����V�s���x�w���C�����V�s�x�w���W���[�����g���V�s�x�w�\���p���x���x�w�P�ʖ��x�̂݊i�[
	//����щ����W���[�����g�v���O������Wave�͈�
	TempRecipeDocSet(szRecipeNameMain);

	//�{�^���������O
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	pDoc->OperationLogging("Measurement Operation - TEST Button was pushed.");

	pMainFrame->m_iNowPoint = 0;
	pDoc->SetMeasMode(MEASMODE_TEST); //�e�X�g���[�h

	CTestMeasurementDlg dlg;
	CComboBox* pCombo;
	int nIndex;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	LPCTSTR pszFilterName[LANGUAGE_MAX][OPT_FILTER_MAX] = {
		{OPTICAL_FILTER_TYPE_ITEM_ENU[0], OPTICAL_FILTER_TYPE_ITEM_ENU[1], OPTICAL_FILTER_TYPE_ITEM_ENU[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Enu
		{OPTICAL_FILTER_TYPE_ITEM_JPN[0], OPTICAL_FILTER_TYPE_ITEM_JPN[1], OPTICAL_FILTER_TYPE_ITEM_JPN[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Jpn
	};

	// Optical Filter
	pCombo = (CComboBox*)GetDlgItem(IDC_SR_THICK_OPT_FILTER);
	nIndex = pCombo->GetCurSel();
// 2013.11.07 Bagus bug fix. -->
	if(nIndex == CB_ERR){
		nIndex = 0;
	}
// 2013.11.07 Bagus bug fix. <--
	dlg.m_strOpticalFilter = pszFilterName[l_SystemConfig.nLanguage][pCombo->GetItemData(nIndex)];

	// Lens
	pCombo = (CComboBox*)GetDlgItem(IDC_SR_THICK_LENS);
	nIndex = pCombo->GetCurSel();
//	  dlg.m_strLens = m_SrTurret[nIndex].szName; //���~
	pCombo->GetLBText(nIndex, dlg.m_strLens);
	// Integration Time
	dlg.m_dIntegTime = m_ThickMeas.ScanParams._SR.dIntegTime;
	dlg.m_wStart = m_ThickMeas.ScanParams._SR.WavelenRange.wStart;
	dlg.m_wEnd = m_ThickMeas.ScanParams._SR.WavelenRange.wEnd;

	if( dlg.DoModal() == IDOK ){
		UpdateData(TRUE);

		m_strLens = dlg.m_strLens;
		m_strOptFilter = dlg.m_strOpticalFilter;
		m_ThickMeas.ScanParams._SR.dIntegTime = dlg.m_dIntegTime;
		m_ThickMeas.ScanParams._SR.WavelenRange.wStart = dlg.m_wStart;
		m_ThickMeas.ScanParams._SR.WavelenRange.wEnd = dlg.m_wEnd;

		UpdateData(FALSE);
	}
}

// =========================================================================
//�_�~�[���W���[�����g�v���O�������쐬
BOOL CRecipeMeasurementSrThicknessProgramView::MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName)
{
	return RecipeFile_SaveRecipe(&m_ThickMeas, pszRecipeName, RECIPE_FILE_SR_THICKNESS);
}

// =========================================================================
//�_�~�[���C���v���O�������쐬
BOOL CRecipeMeasurementSrThicknessProgramView::MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas)
{
	MAIN_RCP_INFO MainRcpInfo_Temp;
	memset(&MainRcpInfo_Temp, 0, sizeof(MainRcpInfo_Temp));


#if 0 //�ݒ肪�K�v�Ȃ�ǉ����邱��
	MainRcpInfo_Temp.hdr.szComment;
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szStage, "---");
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szPointDeskew, "---");
	MainRcpInfo_Temp.MainRcpParam.hdr.nPointManualAdjustment;
	MainRcpInfo_Temp.MainRcpParam.hdr.bSampleID = FALSE;
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szSampleID, "---");
	MainRcpInfo_Temp.MainRcpParam.hdr.bSaveMeasResult = FALSE;
	MainRcpInfo_Temp.MainRcpParam.hdr.bSaveOverwrite = FALSE;
	MainRcpInfo_Temp.MainRcpParam._SR.RecalibItem[0] = 0; //
	strcpy(MainRcpInfo_Temp.MainRcpParam._SR.szRecalib[0], "---"); //
	MainRcpInfo_Temp.MainRcpParam._SR.nAutoFocusFailOption = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.nFinalFocusFailDataValidOption = 0;

	MainRcpInfo_Temp.MainRcpParam._SR.nDefaultDisplay = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.bMeasRef = FALSE;
#endif

	strcpy(MainRcpInfo_Temp.hdr.szName, pszRecipeNameMain);
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szMeas, pszRecipeNameMeas);

	MainRcpInfo_Temp.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
	MainRcpInfo_Temp.MainRcpParam._SR.nFocus = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.bRefWithAF = TRUE;

	return RecipeFile_SaveRecipe(&MainRcpInfo_Temp, pszRecipeNameMain, RECIPE_FILE_SR_MAIN_RECIPE);
}

// =========================================================================
//�����V�s��NanoSpecDoc�֊i�[
BOOL CRecipeMeasurementSrThicknessProgramView::TempRecipeDocSet(LPCTSTR pszRecipeName)
{
//�w���C�����V�s���x�w���C�����V�s�x�w���W���[�����g���V�s�x�w�\���p���x���x�w�P�ʖ��x�̂݊i�[
//����сw�����W���[�����g�v���O������Wave�͈́x�A�w�����ʁx

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	///// �������[������ /////
	pDoc->InitMeasData();

	//������
	rcp_data.MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_THICKNESS;

	//Wave�͈̓Z�b�g
	rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wStart = (int)GetDlgItemInt(IDC_WAVELENGTH_START, NULL, FALSE);
	rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd = (int)GetDlgItemInt(IDC_WAVELENGTH_END, NULL, FALSE);


	strcpy(rcp_data.szRecipeName, pszRecipeName);
	//���V�s���[�h��NanoSpecDoc�ւ̊i�[
	BOOL bMain = FALSE;
	BOOL bMeas = FALSE;
	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bMeas = RecipeFile_LoadRecipe(&rcp_data.MeasProgInfo, rcp_data.MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
	if((bMain == FALSE) || (bMeas == FALSE))
	{
		return FALSE;
	}
	pDoc->SetRcpData(&rcp_data);


	///// �P�ʖ��擾 /////
	SR_FIGURE_FORMAT SrFigureFormat;
	char szUnit[MAX_PATH];
	memset(szUnit, 0, sizeof(szUnit));

	ConfigFile_GetNanoSpecIni(&SrFigureFormat, CONFIG_FILE_SR_FIGURE_FORMAT);

	int nUnit = SrFigureFormat.nThicknessUnit;
	if(nUnit >= 0 && nUnit < FIGURE_UNIT_SR_THICKNESS_MAX){
		strcpy(szUnit, g_lpszFigureUnitSrThickness[nUnit]);
	}
	else{
		strcpy(szUnit, " ");
	}
	pDoc->SetUnitName(szUnit);
	pDoc->SetUnitIndex(nUnit);


	///// ��ʕ\��(�f�[�^�ۑ�)�p�^�C�g�� /////
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];		//��ʕ\��(�f�[�^�ۑ�)�p�^�C�g�� ��X,Y,Z,AF�͊܂܂�
	int iDispLabelCount;
	memset(szDispLabel, 0, sizeof(szDispLabel)); //

	pMainFrame->GetStrategyhead(m_ThickMeas.ScanParams._SR.XMPDesc.szStrategyEntry, szDispLabel, &iDispLabelCount);
	pDoc->SetDispLabel(szDispLabel);
	pDoc->SetDispLabelCount(iDispLabelCount);


	return TRUE;
}

// =========================================================================
//���V�s���쐬
BOOL CRecipeMeasurementSrThicknessProgramView::GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType)
{
	CString l_strBuffer, l_strTitle;

	char szRcpName[RECIPE_NAME_LEN + 1];
	char szRcpNamePath[MAX_PATH + 1];

	sprintf(szRcpName, "%c%s", SYSTEM_RECIPE_BEGINNING_CHAR, TESTMODE_TEMP_RECIPE_NAME); //�쐬���V�s��

	//�쐬���郌�V�s�����d�����Ă��Ȃ������ׂ�ׂ̃p�X���쐬
	if ( iRcpType == SAVE_AS_MAIN_RECIPE ) { //���C�����V�s
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Main_Recipe_Dir, szRcpName, MAINRECIPE_EXT); //�d�����V�s�������p
	}
	else if(iRcpType == SAVE_AS_MEASUREMENT_PROGRAM) { //���W���[�����g�v���O����
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Measurement_Program_Dir, szRcpName, MEASUREMENTPGM_EXT); //�d�����V�s�������p
	}

	//�쐬���悤�Ƃ��郌�V�s�̏d���`�F�b�N
	if ( FileOrDirExists(szRcpNamePath)==TRUE ) { //�쐬���悤�Ƃ��Ă���t�@�C�����ɏd������
		//�\�t�g�N�����ɉ����V�s�������̂ŁA�����ɂ͗��Ȃ��͂��B
		LoadStringML(IDS_CAPTION_SR_TRANSMIT, l_strBuffer, "Temporary recipe for the test mode remains, \nthe test mode cannot be begun.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		return FALSE; //�t�@�C�����d��(�\�t�g�N�����A�����V�s�����~�X)
	}

	strcpy(pszFileName, szRcpName);
	return TRUE;
}

// =========================================================================
//�t�@�C�����݃`�F�b�N(�t�@�C�����ݎ���TRUE��Ԃ�)
BOOL CRecipeMeasurementSrThicknessProgramView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}

/* added 2015.06.01 hmenjo XMP �\��/��\���Q ---------- { ---------- */
void CRecipeMeasurementSrThicknessProgramView::OnBtnShowXmp()
{
	BOOL l_bTextShow = TRUE;

	HWND l_hWnd = ::FindWindow(0, ADAP_WINDOW_NAME);
	if (0 != l_hWnd) {
		BOOL l_bVisible = ::IsWindowVisible(l_hWnd);
		int l_iVisible = 0;
		if (FALSE != l_bVisible) {
			l_iVisible = 1;
		} else {
			l_bTextShow = FALSE;
			l_iVisible = 2;
		}
		MEAS_ShowXmp(l_iVisible);
	}

	this->SetDlgItemText(IDC_BTN_SHOWXMP, (FALSE != l_bTextShow)? _T("Show Xmp") : _T("Hide Xmp"));
}
void CRecipeMeasurementSrThicknessProgramView::OnTimer(UINT nIDEvent)
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	switch (nIDEvent) {
	case TMR_CHKXMPSHOW_ID:
		{	/* XMP �̕\����Ԃ��`�F�b�N���C�{�^���\�����X�V���܂��D	*/
			BOOL l_bTextShow = TRUE;
			HWND l_hWnd = ::FindWindow(0, ADAP_WINDOW_NAME);
			if (0 != l_hWnd) {
				if (FALSE != ::IsWindowVisible(l_hWnd)) {
					l_bTextShow = FALSE;
				}
			}
			this->SetDlgItemText(IDC_BTN_SHOWXMP, (FALSE != l_bTextShow)? _T("Show Xmp") : _T("Hide Xmp"));
		}
		break;
	}

	CNanoRecipeUI::OnTimer(nIDEvent);
}
/* added 2015.06.01 hmenjo XMP �\��/��\���Q ---------- } ---------- */
