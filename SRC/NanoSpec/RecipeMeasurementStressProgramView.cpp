
// RecipeMeasurementStressProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "TestMeasurementDlg.h"
#include "RecipeMeasurementStressProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementStressProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementStressProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMeasurementStressProgramView::CRecipeMeasurementStressProgramView()
	: CNanoRecipeUI(CRecipeMeasurementStressProgramView::IDD)
{
	CString l_strBuffer;
//2009.10.21 bagus Stress 修正 --{--
//	LoadStringML(IDS_CAPTION_SR_THICKNESS, l_strBuffer, "SR THICKNESS");
	LoadStringML(IDS_CAPTION_STRESS, l_strBuffer, "STRESS");
//2009.10.21 bagus Stress 修正 --}--
	m_strCaption = l_strBuffer;

	m_strLens = _T("");

	//{{AFX_DATA_INIT(CRecipeMeasurementStressProgramView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementStressProgramView::~CRecipeMeasurementStressProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementStressProgramView)
	DDX_Control(pDX, IDC_XMP_MEASUREMENT_ITEM_LIST, m_XmpMeasurementItemListCtrl);
	DDX_Control(pDX, IDC_TEST_BUTTON, m_TestButton);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_THICKNESS_PROG_NAME, m_StressMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_STRESS_THICK_STRATEGY, m_StressMeas.ScanParams._SR.XMPDesc.szStrategyEntry, STRATEGY_NAME_LEN + 1);
	DDX_Text(pDX, IDC_MEAS_THICKNESS_INTEGRATION_TIME, m_StressMeas.ScanParams._SR.dIntegTime);
	DDX_CBStringExact(pDX, IDC_STRESS_THICK_LENS, m_strLens);
	DDX_CBStringExact(pDX, IDC_STRESS_THICK_OPT_FILTER, m_strOptFilter);
	DDX_Check(pDX, IDC_ENHANCED_UV, m_StressMeas.ScanParams._SR.bEnhancedUv);
	DDX_Text(pDX, IDC_START_WAVELENGTH, (short&)m_StressMeas.ScanParams._SR.WavelenRange.wStart);
	DDX_Text(pDX, IDC_END_WAVELENGTH, (short&)m_StressMeas.ScanParams._SR.WavelenRange.wEnd);
	DDX_Check(pDX, IDC_ADD_REFLECTANCE_MEASUREMENT, m_StressMeas.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG1, m_strSpecificWavelen[0]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG2, m_strSpecificWavelen[1]);
	DDX_Text(pDX, IDC_MEAS_REFL_WAVE_LENG3, m_strSpecificWavelen[2]);
	DDX_Check(pDX, IDC_STRESS_THICK_REF_LIFETIME_CHECK, m_StressMeas.Ref.hdr.bWarnFlag);
	DDX_Text(pDX, IDC_STRESS_THICK_REF_LIFETIME, m_StressMeas.Ref.hdr.dLifeTime);
	DDX_Text(pDX, IDC_STRESS_THICK_REF_MATERIAL, m_StressMeas.Ref.OptDesc.szRefMaterial, _MAX_PATH + 1);
	DDX_Text(pDX, IDC_COMMENT, m_StressMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

BEGIN_MESSAGE_MAP(CRecipeMeasurementStressProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementStressProgramView)
	ON_CBN_SELCHANGE(IDC_STRESS_THICK_LENS, OnSelchangeStressThickLens)
	ON_BN_CLICKED(IDC_TEST_BUTTON, OnTestButton)
	ON_BN_CLICKED(IDC_ADD_REFLECTANCE_MEASUREMENT, OnAddReflectanceMeasurement)
	ON_CBN_SELCHANGE(IDC_STRESS_THICK_OPT_FILTER, OnSelchangeStressThickOptFilter)
	ON_BN_CLICKED(IDC_STRESS_THICK_REF_LIFETIME_CHECK, OnCheckReferenceLifeTime)
	ON_CBN_SELCHANGE(IDC_STRESS_THICK_STRATEGY, OnSelchangeStressThickStrategy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CRecipeMeasurementStressProgramView メッセージ ハンドラ
// =========================================================================
//
void CRecipeMeasurementStressProgramView::OnInitialUpdate()
{
	CString l_strBuffer, l_strTitle;

	///// Load File /////
//	ConfigFile_GetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	ConfigFile_GetNanoSpecIni(m_srFilter, CONFIG_FILE_SR_FILTER);
	LoadRecipeData();

	m_strLens = m_SrTurret[m_StressMeas.ScanParams._SR.iLens].szName;

	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// 作成に失敗
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, 20, SWP_NOMOVE);

	///// Static Control /////
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_THICKNESS_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// Control Initialize /////
	InitCombo_StressThickStrategy();
	InitCombo_StressThickReferenceMaterial();
	InitCombo_StressThickOpticalFilter();
	InitCombo_Lens();
	if ( !m_SrConfig.bUv )
		GetDlgItem(IDC_ENHANCED_UV)->ShowWindow(SW_HIDE);

	OnCheckReferenceLifeTime();
	OnSelchangeStressThickOptFilter();

	/// Test Button ///
	m_TestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TestButton.DrawFlatFocus(TRUE);
	m_TestButton.ShowWindow(SW_HIDE);

// 2009.10.22 bagus Stress 修正 --{--
#if 0
	///// CalcDataInterval /////
	CString	strBuffer;
	strBuffer.Format("%ld", m_StressMeas.ScanParams._SR.dwCalcDataInterval);
	SetDlgItemText(IDC_MEAS_STRESS_CALCULATION_DATA_INTERVAL, strBuffer);
#endif
// 2009.10.22 bagus Stress 修正 --}--

	///// 測定反射率波長設定 /////
	int IDC_MEAS_REFL_WAVE_LENG[] = {IDC_MEAS_REFL_WAVE_LENG1, IDC_MEAS_REFL_WAVE_LENG2, IDC_MEAS_REFL_WAVE_LENG3};
	for ( int i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
		if (m_StressMeas.ScanParams._SR.iSpecificWavelen[i] != 0)
			m_strSpecificWavelen[i].Format("%d", m_StressMeas.ScanParams._SR.iSpecificWavelen[i]);
		else
			m_strSpecificWavelen[i].Empty();

		SetDlgItemText(IDC_MEAS_REFL_WAVE_LENG[i], m_strSpecificWavelen[i]);
	}

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_THICKNESS_PROG_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_THICKNESS_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEST_BUTTON)->ShowWindow(SW_HIDE);
	}

	InitItemList();
	OnSelchangeStressThickStrategy();
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);
	OnAddReflectanceMeasurement();

	if ((IsDataChanged() == TRUE) && (m_nOpenMode == modeNormal))
	{
		//ストラテジィを書き換えた場合、
		//レシピを読込んだ値と画面表示値(光学フィルタ・波長)に差がでてしまうことがあるので、レシピを強制的に書換える。
		SaveRecipeData();
		LoadStringML(IDS_SAVE_OPTICAL_FILTER_WAVELENG, l_strBuffer, "The optical filter and the wavelength were saved\n by the change in the strategy.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
	}
}

// =========================================================================
// リファレンスタイム チェック選択時
//
void CRecipeMeasurementStressProgramView::OnCheckReferenceLifeTime()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_STRESS_THICK_REF_LIFETIME)->EnableWindow(m_StressMeas.Ref.hdr.bWarnFlag);
}

// =========================================================================
// オプティカルフィルター コンボ選択時
//
void CRecipeMeasurementStressProgramView::OnSelchangeStressThickOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STRESS_THICK_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_StressMeas.ScanParams._SR.wOpticsFilterType = pCombo->GetItemData(nSelect);
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::LoadRecipeData()
{
	CString l_strBuffer;

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_StressMeas, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS) ) {
			m_StressMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_STRESS;
			m_StressMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_STRESS_MEAS;
		}
		//2009.12.23 bagus stress --{--
		//必ずヘッドタイプはストレスにならなければいけない
		m_StressMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_STRESS;
		m_StressMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_STRESS_MEAS;
		//2009.12.23 bagus stress --}--
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_StressMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) { 		 // RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_StressMeas, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS) ) {
				m_StressMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_STRESS;
				m_StressMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_STRESS_MEAS;
			}
			//2009.12.23 bagus stress --{--
			//必ずヘッドタイプはストレスにならなければいけない
			m_StressMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_STRESS;
			m_StressMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_STRESS_MEAS;
			//2009.12.23 bagus stress --}--
		}
		break;
	}
	::CopyMemory(&m_OldStressMeas, &m_StressMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_StressMeas.hdr.szName, l_strBuffer);
	}
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::InitCombo_StressThickStrategy()
{
	CString l_strBuffer;
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 -->
	BOOL bStress, bDummy, bDummy2;
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 <--

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STRESS_THICK_STRATEGY);

	pCombo->ResetContent();

	SR_XMP SrXmp;
	ConfigFile_GetNanoSpecIni(&SrXmp, CONFIG_FILE_SR_XMP);
	CString strBuffer;

	TRY
	{
		CStdioFile stdioFile(SrXmp.szStrategyListPath, CFile::modeRead);
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 -->
		while ( stdioFile.ReadString(strBuffer) ) {
			MEAS_GetStratetyType(bDummy, bDummy2, bStress, strBuffer);
			if ( bStress )
				pCombo->AddString(strBuffer);
		}
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 <--
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

	int nIndex = pCombo->FindStringExact(-1, m_StressMeas.ScanParams._SR.XMPDesc.szStrategyEntry);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::InitCombo_StressThickReferenceMaterial()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STRESS_THICK_REF_MATERIAL);
	pCombo->ResetContent();

	CString strDirPath;
	strDirPath.Format("%s*%s", g_szCfg_System_Sr_Reference_Material_Dir, DAT_EXT);
	CFileFind Finder;
	if ( !Finder.FindFile(strDirPath) )
		return;

	BOOL bLoop = TRUE;
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		pCombo->AddString(Finder.GetFileTitle());
	}

	int nIndex = pCombo->FindStringExact(0, m_StressMeas.Ref.OptDesc.szRefMaterial);
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
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::InitCombo_StressThickOpticalFilter()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STRESS_THICK_OPT_FILTER);

	//オートフィルタありの場合
	if (m_SrConfig.bAutoFilter){
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

		if ((m_StressMeas.ScanParams._SR.wOpticsFilterType < 0) || (m_StressMeas.ScanParams._SR.wOpticsFilterType >= OPT_FILTER_MAX)) {
			nIndex = 0;
		} else {
			nIndex = pCombo->FindStringExact(-1, pszFilterName[l_SystemConfig.nLanguage][m_StressMeas.ScanParams._SR.wOpticsFilterType]);
		}

		if(!(nIndex == CB_ERR)){
			pCombo->SetCurSel(nIndex);
		}
		else{
			if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
				nIndex = 0;
				pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus Stress --{--
				m_StressMeas.ScanParams._SR.wOpticsFilterType = pCombo->GetItemData(0);
// 2009.12.14 bagus Stress --}--
			}
			else{
				nIndex = 0;
				pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus Stress --{--
				m_StressMeas.ScanParams._SR.wOpticsFilterType = pCombo->GetItemData(0);
// 2009.12.09 bagus Stress --}--
				SaveRecipeData();
				LoadStringML(IDS_OPTICAL_FILTER_ZERO, l_strBuffer, "Invalid for the specified item, Optical filter is saved as an INDEX to 0.");
				LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
				MessageBox(l_strBuffer, l_strTitle, MB_OK);
			}
		}
	}

	//オートフィルタなしの場合
	else{
		m_StressMeas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_STRESS_THICK_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STRESS_THICK_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
//レンズ コンボイニシャル処理
//
void CRecipeMeasurementStressProgramView::InitCombo_Lens()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STRESS_THICK_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
// 2009.11.24 bagus SR 修正 --{--
//		if ( m_SrTurret[i].bEnable ) {
		if ( m_SrTurret[i].bEnable && m_SrTurret[i].bAnalysis ) {
// 2009.11.24 bagus SR 修正 --}--
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	nIndex = pCombo->FindStringExact(-1, m_strLens);
	if(!(nIndex == CB_ERR)){
		pCombo->SetCurSel(nIndex);
	}
	else{
		if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus Stress --{--
			m_StressMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.14 bagus Stress --}--
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus Stress --{--
			m_StressMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.09 bagus Stress --}--
			SaveRecipeData();
			LoadStringML(IDS_LENS_ZERO, l_strBuffer, "Invalid for the specified item, Lens is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::InitItemList()
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
		{"Index",	"アイテム",	},		// Jpn
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
BOOL CRecipeMeasurementStressProgramView::CheckData()
{
	CString l_strBuffer, l_strTitle;

	UpdateData(TRUE);

	CString strBuffer;
	CString strCaption;
	int MinWaveLength;
	int MaxWaveLength;
	int iValue;
	BOOL bValidWaveLen = FALSE;
	const WORD wStart = m_StressMeas.ScanParams._SR.WavelenRange.wStart;
	const WORD wEnd = m_StressMeas.ScanParams._SR.WavelenRange.wEnd;

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

// 2009.10.22 bagus Stress 修正 --{--
#if 0
	///// CalcDataInterval /////
	long	dwBuffer;
	GetDlgItemText(IDC_MEAS_STRESS_CALCULATION_DATA_INTERVAL, strBuffer);
	dwBuffer = strtol(strBuffer, NULL, 10);
	if ((dwBuffer < 1) || (dwBuffer > 20)) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:データ間隔(1-20)");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_CALCULATION_DATA_INTERVAL, strBuffer, "Please input data interval.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return ( FALSE );
	} else {
		m_StressMeas.ScanParams._SR.dwCalcDataInterval = dwBuffer;
	}
#endif
// 2009.10.22 bagus Stress 修正 --}--

	///// Strategy /////
	if ( strlen(m_StressMeas.ScanParams._SR.XMPDesc.szStrategyEntry) == 0 )
	{
		LoadStringML(IDS_STRATEGY_EMPTY, l_strBuffer, "Strategy is empty.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	///// Integration Time /////
	if ( m_StressMeas.ScanParams._SR.dIntegTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_StressMeas.ScanParams._SR.dIntegTime )
	{
		LoadStringML(IDS_INTEGRATIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.3f - %.3f)");
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_MEAS_THICKNESS_INTEGRATION_TIME)->SetFocus();
		return FALSE;
	}

	///// Lens /////
	if ( m_strLens.IsEmpty() )
	{
		LoadStringML(IDS_SELECT_LENS, l_strBuffer, "Please Select Lens.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_STRESS_THICK_LENS)->SetFocus();
		return FALSE;
	}

// 2009.12.08 K.Matsuo PKI Special Logic -->
	///// Optical Filter /////
	// スモールスポットは、レンズ5Xを選択したとき、オプティカルフィルターは、ND-30であること（サチュレーションしないためにです）
	if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS ) {
		if ( m_strLens == "5 X" || m_strLens == "10 X" ) {
			if ( m_StressMeas.ScanParams._SR.wOpticsFilterType != OPT_FILTER_POS3 ) {
				LoadStringML(IDS_SMALLSPOT_FILTER, l_strBuffer, "If Select 5X or 10X Lens, select ND-30 Filter Necessary.");
				MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
				GetDlgItem(IDC_SR_THICK_OPT_FILTER)->SetFocus();
				return FALSE;
			}
		}
	}
// 2009.12.08 K.Matsuo PKI Special Logic <--

	///// Wave Length /////
	if ( wStart >=	wEnd)
	{
		LoadStringML(IDS_START_WAVE_LARGER, l_strBuffer, "Start Wavelength is Larger than End Wavelength.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_WAVELENGTH_START)->SetFocus();
		return FALSE;
	}

	if ( m_StressMeas.ScanParams._SR.bEnhancedUv )
		MinWaveLength = MIN_ENHANCED_UV_WAVELENGTH;
	else
		MinWaveLength = MIN_TARGET_WAVELENGTH;

	if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
		MaxWaveLength = MAX_TARGET_WAVELENGTH;
	else
		MaxWaveLength = MAX_TARGET_WAVELENGTH_800;

	if ( wStart < MinWaveLength || MaxWaveLength < wStart )
	{
		LoadStringML(IDS_START_WAVE_OUT_RANGE, l_strBuffer, "Start Wavelength is out of range.""\n(%d - %d)");
		strBuffer.Format(l_strBuffer, MinWaveLength, MaxWaveLength);
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_WAVELENGTH_START)->SetFocus();
		return FALSE;
	}

	if ( wEnd < MinWaveLength || MaxWaveLength < wEnd )
	{
		LoadStringML(IDS_END_WAVE_OUT_RANGE, l_strBuffer, "End Wavelength is out of range.""\n(%d - %d)");
		strBuffer.Format(l_strBuffer, MinWaveLength, MaxWaveLength);
		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_WAVELENGTH_END)->SetFocus();
		return FALSE;
	}

	///// 測定反射率波長設定 /////
	///// Reflectance Measurement /////
	///// 1st Target WaveLength /////
	///// 2nd Target WaveLength /////
	///// 3rd Target WaveLength /////
	int IDC_MEAS_REFL_WAVE_LENG[] = {IDC_MEAS_REFL_WAVE_LENG1, IDC_MEAS_REFL_WAVE_LENG2, IDC_MEAS_REFL_WAVE_LENG3};
	int IDS_WAVELENGTH[] = {IDS_FIRST_WAVE_OUT_RANGE, IDS_SECOND_WAVE_OUT_RANGE, IDS_THIRD_WAVE_OUT_RANGE};
	LPCTSTR pszWaveLengthString[] = {"1st Target WaveLength is out of range.\n(%d - %d)", "2nd Target WaveLength is out of range.\n(%d - %d)", "3rd Target WaveLength is out of range.\n(%d - %d)"};

	if (m_StressMeas.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag) {
		//３つの設定値の上位が未設定の場合は上位に詰める。
		CStringArray strSpecificWavelen;
		strSpecificWavelen.SetSize(SPECIFIED_WAVELENGTH_MAX);
		for ( int i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
			strSpecificWavelen[i] = m_strSpecificWavelen[i];
			strSpecificWavelen[i].TrimLeft();
			strSpecificWavelen[i].TrimRight();
		}

		// 上位に詰める
		int j = 0;
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
				if ((!CheckTransData(iValue,m_strSpecificWavelen[i])) || (iValue < wStart) || (wEnd < iValue )) { /**整数で、ターゲットが有効範囲に設定されているか？*/
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
				m_StressMeas.ScanParams._SR.iSpecificWavelen[i] = atoi(m_strSpecificWavelen[i]);
			}
		}
	}

	///// Life Time /////
	if( m_StressMeas.Ref.hdr.bWarnFlag )
	{
		if ( m_StressMeas.Ref.hdr.dLifeTime < MIN_LIFE_TIME || MAX_LIFE_TIME < m_StressMeas.Ref.hdr.dLifeTime )
		{
			LoadStringML(IDS_LIFETIME_OUT_RANGE, l_strBuffer, "Life Time is out of range.""\n(%.1f - %.1f)");
			strBuffer.Format(l_strBuffer, MIN_LIFE_TIME, MAX_LIFE_TIME);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_STRESS_THICK_REF_LIFETIME)->SetFocus();
			return FALSE;
		}
		if((m_StressMeas.Ref.hdr.dLifeTime - (int)m_StressMeas.Ref.hdr.dLifeTime) != 0) //整数でない
		{
			LoadStringML(IDS_SET_INTERVAL_MIN, l_strBuffer, "Please set it at intervals of 1 min.");
			strBuffer.Format(l_strBuffer);
			MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
			GetDlgItem(IDC_STRESS_THICK_REF_LIFETIME)->SetFocus();
			return FALSE;
		}
	}

	///// Reference Strategy /////
	if ( strlen(m_StressMeas.Ref.OptDesc.szRefMaterial) == 0)
	{
		LoadStringML(IDS_REFER_STRATEGY_EMPTY, l_strBuffer, "Reference Strategy is empty.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_STRESS_THICK_REF_MATERIAL)->SetFocus();
		return FALSE;
	}

	if(m_StressMeas.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag == FALSE) //AddReflectanceMeasureFlag
	{
		m_StressMeas.ScanParams._SR.iSpecificWavelen[0] = 0; //FirstWaveLength
		m_StressMeas.ScanParams._SR.iSpecificWavelen[1] = 0; //SecondWaveLength
		m_StressMeas.ScanParams._SR.iSpecificWavelen[2] = 0; //ThirdWaveLength
	}


	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementStressProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldStressMeas, &m_StressMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementStressProgramView::SaveRecipeData()
{
// 2009.10.09 K.Matsuo Delete -->
// 2009.09.19 bagus SE --{--
//	SR_CONFIG	SrConfig;
//
//	memset(&SrConfig, 0, sizeof(SrConfig));
//	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);
//
//	m_StressMeas.ScanParams._SR.iLens = SrConfig.iDefaultMeasLensNo;
//	m_StressMeas.ScanParams._SR.wOpticsFilterType = SrConfig.wDefaultMeasFilter;
// 2009.09.19 bagus SE --}--
// 2009.10.09 K.Matsuo Delete <--

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_StressMeas, STRESS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_STRESS) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_StressMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_StressMeas, m_szRecipeName, RECIPE_FILE_STRESS) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldStressMeas, &m_StressMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementStressProgramView::IsAccessPrivilege()
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
int CRecipeMeasurementStressProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::OnAddReflectanceMeasurement()
{
	UpdateData(TRUE);

	// 反射率測定の追加設定エディットボックスの有効無効の切り替え
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG1)->EnableWindow(IsDlgButtonChecked(IDC_ADD_REFLECTANCE_MEASUREMENT) == BST_CHECKED);
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG2)->EnableWindow(IsDlgButtonChecked(IDC_ADD_REFLECTANCE_MEASUREMENT) == BST_CHECKED);
	GetDlgItem(IDC_MEAS_REFL_WAVE_LENG3)->EnableWindow(IsDlgButtonChecked(IDC_ADD_REFLECTANCE_MEASUREMENT) == BST_CHECKED);
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::OnSelchangeStressThickStrategy()
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

	if ( !MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iStartWaveLen, iEndWaveLen, iOpticalFilter, m_StressMeas.ScanParams._SR.XMPDesc.szStrategyEntry) ) {
		// デフォルト値設定
		m_StressMeas.ScanParams._SR.WavelenRange.wStart = iStartWaveLen;
		m_StressMeas.ScanParams._SR.WavelenRange.wEnd = iEndWaveLen;
		UpdateData(FALSE);
//		m_StressMeas.ScanParams._SR.wOpticsFilterType = iOpticalFilter;
//		InitCombo_StressThickOpticalFilter();
		return;
	}

	m_StressMeas.ScanParams._SR.WavelenRange.wStart = iStartWaveLen;
	m_StressMeas.ScanParams._SR.WavelenRange.wEnd = iEndWaveLen;
	UpdateData(FALSE);
// 2009.12.08 K.Matsuo -->
//	m_StressMeas.ScanParams._SR.wOpticsFilterType = iOpticalFilter;
//	InitCombo_StressThickOpticalFilter();
// 2009.12.08 K.Matsuo <--

	token = _tcstok(szDdeTextItem, _T(","));
	while ( token ) {
		iItem = m_XmpMeasurementItemListCtrl.GetItemCount();
		_stprintf(szBuff, _T("%d"), iItem + 1);
		m_XmpMeasurementItemListCtrl.InsertItem(iItem, szBuff);

		if(strcmp(token, "MSE") == 0)
		{
			//『MSE→Fit』の置換
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
BOOL CRecipeMeasurementStressProgramView::CheckTransData(int iData , CString strData)
{
	CString strTransiData;

	strTransiData.Format("%d",iData);
	return strTransiData == strData;
}

// =========================================================================
// レンズ コンボ選択時
//
void CRecipeMeasurementStressProgramView::OnSelchangeStressThickLens()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STRESS_THICK_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_StressMeas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	UpdateData(FALSE);
}

// =========================================================================
//
void CRecipeMeasurementStressProgramView::OnTestButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください


	//仮メインレシピ作成
	if(CheckData() == FALSE) //レシピデータの整合性がとれていない
	{
		//エラーメッセージはCheckData()内で出す。
		return; //テストモード開始せず
	}
	char szRecipeNameMain[RECIPE_NAME_LEN + 1];
	char szRecipeNameMeas[RECIPE_NAME_LEN + 1];

	if ( !GetTestModeRecipeName(szRecipeNameMain, SAVE_AS_MAIN_RECIPE) )
		return;
	if ( !GetTestModeRecipeName(szRecipeNameMeas, SAVE_AS_MEASUREMENT_PROGRAM) )
		return;

	BOOL bMainRcpMakeFlg = MakeTempSrMainRecipe(szRecipeNameMain, szRecipeNameMeas); //仮メインレシピ作成
	BOOL bMeasRcpMakeFlg = MakeTempSrMeasurementRecipe(szRecipeNameMeas); //仮メジャーメントプログラム作成

	//『メインレシピ名』『メインレシピ』『メジャーメントレシピ』『表示用ラベル』『単位名』のみ格納
	//および仮メジャーメントプログラムのWave範囲
	TempRecipeDocSet(szRecipeNameMain);

	//ボタン押下ログ
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	pDoc->OperationLogging("Measurement Operation - TEST Button was pushed.");

	pMainFrame->m_iNowPoint = 0;
	pDoc->SetMeasMode(MEASMODE_TEST); //テストモード

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
	pCombo = (CComboBox*)GetDlgItem(IDC_STRESS_THICK_OPT_FILTER);
	nIndex = pCombo->GetCurSel();
	dlg.m_strOpticalFilter = pszFilterName[l_SystemConfig.nLanguage][pCombo->GetItemData(nIndex)];

	// Lens
	pCombo = (CComboBox*)GetDlgItem(IDC_STRESS_THICK_LENS);
	nIndex = pCombo->GetCurSel();
//	  dlg.m_strLens = m_SrTurret[nIndex].szName; //←×
	pCombo->GetLBText(nIndex, dlg.m_strLens);
	// Integration Time
	dlg.m_dIntegTime = m_StressMeas.ScanParams._SR.dIntegTime;
	dlg.m_wStart = m_StressMeas.ScanParams._SR.WavelenRange.wStart;
	dlg.m_wEnd = m_StressMeas.ScanParams._SR.WavelenRange.wEnd;

	if( dlg.DoModal() == IDOK ){
		UpdateData(TRUE);

		m_strLens = dlg.m_strLens;
		m_strOptFilter = dlg.m_strOpticalFilter;
		m_StressMeas.ScanParams._SR.dIntegTime = dlg.m_dIntegTime;
		m_StressMeas.ScanParams._SR.WavelenRange.wStart = dlg.m_wStart;
		m_StressMeas.ScanParams._SR.WavelenRange.wEnd = dlg.m_wEnd;

		UpdateData(FALSE);
	}
}

// =========================================================================
//ダミーメジャーメントプログラムを作成
BOOL CRecipeMeasurementStressProgramView::MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName)
{
	return RecipeFile_SaveRecipe(&m_StressMeas, pszRecipeName, RECIPE_FILE_SR_THICKNESS);
}

// =========================================================================
//ダミーメインプログラムを作成
BOOL CRecipeMeasurementStressProgramView::MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas)
{
	MAIN_RCP_INFO MainRcpInfo_Temp;
	memset(&MainRcpInfo_Temp, 0, sizeof(MainRcpInfo_Temp));


#if 0 //設定が必要なら追加すること
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
//仮レシピをNanoSpecDocへ格納
BOOL CRecipeMeasurementStressProgramView::TempRecipeDocSet(LPCTSTR pszRecipeName)
{
//『メインレシピ名』『メインレシピ』『メジャーメントレシピ』『表示用ラベル』『単位名』のみ格納
//および『仮メジャーメントプログラムのWave範囲』、『測定種別』

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	///// メモリー初期化 /////
	pDoc->InitMeasData();

	//測定種別
	rcp_data.MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_THICKNESS;

	//Wave範囲セット
	rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wStart = (int)GetDlgItemInt(IDC_WAVELENGTH_START, NULL, FALSE);
	rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd = (int)GetDlgItemInt(IDC_WAVELENGTH_END, NULL, FALSE);


	strcpy(rcp_data.szRecipeName, pszRecipeName);
	//レシピロード＆NanoSpecDocへの格納
	BOOL bMain = FALSE;
	BOOL bMeas = FALSE;
	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bMeas = RecipeFile_LoadRecipe(&rcp_data.MeasProgInfo, rcp_data.MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
	if((bMain == FALSE) || (bMeas == FALSE))
	{
		return FALSE;
	}
	pDoc->SetRcpData(&rcp_data);


	///// 単位名取得 /////
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


	///// 画面表示(データ保存)用タイトル /////
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];		//画面表示(データ保存)用タイトル ←X,Y,Z,AFは含まず
	int iDispLabelCount;
	memset(szDispLabel, 0, sizeof(szDispLabel)); //

	pMainFrame->GetStrategyhead(m_StressMeas.ScanParams._SR.XMPDesc.szStrategyEntry, szDispLabel, &iDispLabelCount);
	pDoc->SetDispLabel(szDispLabel);
	pDoc->SetDispLabelCount(iDispLabelCount);


	return TRUE;
}

// =========================================================================
//レシピ名作成
BOOL CRecipeMeasurementStressProgramView::GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType)
{
	CString l_strBuffer, l_strTitle;

	char szRcpName[RECIPE_NAME_LEN + 1];
	char szRcpNamePath[MAX_PATH + 1];

	sprintf(szRcpName, "%c%s", SYSTEM_RECIPE_BEGINNING_CHAR, TESTMODE_TEMP_RECIPE_NAME); //作成レシピ名

	//作成するレシピ名が重複していないか調べる為のパスを作成
	if ( iRcpType == SAVE_AS_MAIN_RECIPE ) { //メインレシピ
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Main_Recipe_Dir, szRcpName, MAINRECIPE_EXT); //重複レシピ名検索用
	}
	else if(iRcpType == SAVE_AS_MEASUREMENT_PROGRAM) { //メジャーメントプログラム
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Measurement_Program_Dir, szRcpName, MEASUREMENTPGM_EXT); //重複レシピ名検索用
	}

	//作成しようとするレシピの重複チェック
	if ( FileOrDirExists(szRcpNamePath)==TRUE ) { //作成しようとしているファイル名に重複あり
		//ソフト起動時に仮レシピを消すので、ここには来ないはず。
		LoadStringML(IDS_CAPTION_SR_TRANSMIT, l_strBuffer, "Temporary recipe for the test mode remains, \nthe test mode cannot be begun.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		return FALSE; //ファイル名重複(ソフト起動時、仮レシピ消去ミス)
	}

	strcpy(pszFileName, szRcpName);
	return TRUE;
}

// =========================================================================
//ファイル存在チェック(ファイル存在時はTRUEを返す)
BOOL CRecipeMeasurementStressProgramView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}
