// DeskewDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "BitmapStatic.h"
#include "SaveModifiedDlg.h"
#include "PatRecDeskewDlg.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "DeskewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CDeskewDlg ダイアログ
// #########################################################################

static UINT DeskewImageCtrlIDs[] =
{
	IDC_DESKEW_SITE1,
	IDC_DESKEW_SITE2,
};

// =========================================================================
//
CDeskewDlg::CDeskewDlg(STAGE_PROG_INFO_HDR* pStageProgInfoHdr, CWnd* pParent /*=NULL*/)
	:	m_pStageProgInfoHdr(pStageProgInfoHdr) , CDialog(CDeskewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeskewDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CDeskewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeskewDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AUTO_DESKEW1_X, m_stcAutoDeskew1_X);
	DDX_Control(pDX, IDC_AUTO_DESKEW1_Y, m_stcAutoDeskew1_Y);
	DDX_Control(pDX, IDC_AUTO_DESKEW2_X, m_stcAutoDeskew2_X);
	DDX_Control(pDX, IDC_AUTO_DESKEW2_Y, m_stcAutoDeskew2_Y);
	DDX_Control(pDX, IDC_MANUAL_DESKEW1_X, m_stcManualDeskew1_X);
	DDX_Control(pDX, IDC_MANUAL_DESKEW1_Y, m_stcManualDeskew1_Y);
	DDX_Control(pDX, IDC_MANUAL_DESKEW2_X, m_stcManualDeskew2_X);
	DDX_Control(pDX, IDC_MANUAL_DESKEW2_Y, m_stcManualDeskew2_Y);
	DDX_Control(pDX, IDC_AUTO_DESKEW1_SETUP_BUTTON, m_AutoDeskew1SetupButton);
	DDX_Control(pDX, IDC_AUTO_DESKEW2_SETUP_BUTTON, m_AutoDeskew2SetupButton);
	DDX_Control(pDX, IDC_MANUAL_DESKEW1_SETUP_BUTTON, m_ManualDeskew1SetupButton);
	DDX_Control(pDX, IDC_MANUAL_DESKEW2_SETUP_BUTTON, m_ManualDeskew2SetupButton);
	DDX_Control(pDX, IDC_DESKEW_IMG1, m_stcDeskewImg[0]);
	DDX_Control(pDX, IDC_DESKEW_IMG2, m_stcDeskewImg[1]);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDV_CustomComboExecutionModeCheck(pDX);
	DDV_CustomComboFailOptionCheck(pDX);
	DDV_CustomAutoDeskewLensCheck(pDX);
	DDX_CustomLocText(pDX, IDC_AUTO_DESKEW1_X, m_pStageProgInfoHdr->DeskewSite[0].Loc.lX);
	DDX_CustomLocText(pDX, IDC_AUTO_DESKEW1_Y, m_pStageProgInfoHdr->DeskewSite[0].Loc.lY);
	DDX_CustomLocText(pDX, IDC_AUTO_DESKEW2_X, m_pStageProgInfoHdr->DeskewSite[1].Loc.lX);
	DDX_CustomLocText(pDX, IDC_AUTO_DESKEW2_Y, m_pStageProgInfoHdr->DeskewSite[1].Loc.lY);
	DDX_CustomLocText(pDX, IDC_MANUAL_DESKEW1_X, m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lX);
	DDX_CustomLocText(pDX, IDC_MANUAL_DESKEW1_Y, m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lY);
	DDX_CustomLocText(pDX, IDC_MANUAL_DESKEW2_X, m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lX);
	DDX_CustomLocText(pDX, IDC_MANUAL_DESKEW2_Y, m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lY);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDeskewDlg::DDX_CustomLocText(CDataExchange* pDX, int nIDC, long& value)
{
	// um→mm表示対応

	TCHAR szBuff[256];

//	表示用のコントロールなので、コメントアウトしておく
//	pDX->PrepareEditCtrl(nIDC);

	if ( pDX->m_bSaveAndValidate ) {
//		表示用のコントロールなので、コメントアウトしておく
//		GetDlgItemText(nIDC, szBuff, sizeof(szBuff));
//		value = (long)(atof(szBuff) * MICROMETRE);
	}
	else {
		_stprintf(szBuff, "%.3lf", ((double)value) / MICROMETRE);
		SetDlgItemText(nIDC, szBuff);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CDeskewDlg::DDV_CustomComboExecutionModeCheck(CDataExchange* pDX)
{
	CString strBuffer;

	pDX->PrepareCtrl(IDC_DESKEW_EXECUTION_MODE);

	if ( pDX->m_bSaveAndValidate ) {
		switch ( m_pStageProgInfoHdr->nDeskewMode ) {
		case DESKEW_MODE_NO_DESKEW:
			break;
		case DESKEW_MODE_PATTERN_DESKEW:
			if ( !m_pStageProgInfoHdr->DeskewSite[0].bDefined ) {
				LoadStringML(IDS_TEACH_AUTO_DESKEW1 , strBuffer, "Please Teach Auto Deskew1");
				AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
				pDX->Fail();
			}
			if ( !m_pStageProgInfoHdr->DeskewSite[1].bDefined ) {
				LoadStringML(IDS_TEACH_AUTO_DESKEW2 , strBuffer, "Please Teach Auto Deskew2");
				AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
				pDX->Fail();
			}
			break;
		case DESKEW_MODE_MANUAL_DESKEW:
			if ( !m_pStageProgInfoHdr->DeskewSiteManual[0].bDefined ) {
				LoadStringML(IDS_TEACH_MANUAL_DESKEW1 , strBuffer, "Please Teach Manual Deskew1");
				AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
				pDX->Fail();
			}
			if ( !m_pStageProgInfoHdr->DeskewSiteManual[1].bDefined ) {
				LoadStringML(IDS_TEACH_MANUAL_DESKEW2 , strBuffer, "Please Teach Manual Deskew2");
				AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
				pDX->Fail();
			}
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CDeskewDlg::DDV_CustomComboFailOptionCheck(CDataExchange* pDX)
{
	CString strBuffer;

	pDX->PrepareCtrl(IDC_DESKEW_FAIL_OPTION);

	if ( pDX->m_bSaveAndValidate ) {
		if ( m_pStageProgInfoHdr->nDeskewMode == DESKEW_MODE_PATTERN_DESKEW ) {
			if ( m_pStageProgInfoHdr->nDeskewFailOption == DESKEW_FAIL_OPTION_MANUAL_DESKEW ) {
				if ( !m_pStageProgInfoHdr->DeskewSiteManual[0].bDefined ) {
					LoadStringML(IDS_TEACH_MANUAL_DESKEW1 , strBuffer, "Please Teach Manual Deskew1");
					AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
					pDX->Fail();
				}
				if ( !m_pStageProgInfoHdr->DeskewSiteManual[1].bDefined ) {
					LoadStringML(IDS_TEACH_MANUAL_DESKEW2 , strBuffer, "Please Teach Manual Deskew2");
					AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
					pDX->Fail();
				}
			}
		}
	}
}

// 2009.05.07 AutoDeskewのレンズを、一致させる対応
/////////////////////////////////////////////////////////////////////////////
//
void CDeskewDlg::DDV_CustomAutoDeskewLensCheck(CDataExchange* pDX)
{
	CString strBuffer;

	pDX->PrepareCtrl(IDC_AUTO_DESKEW1_SETUP_BUTTON);

	if ( pDX->m_bSaveAndValidate ) {
		if ( m_pStageProgInfoHdr->nDeskewMode == DESKEW_MODE_PATTERN_DESKEW ) {
			if ( m_iAutoDeskewLensCheck[0] != m_iAutoDeskewLensCheck[1] ) {
				LoadStringML(IDS_LENS_SETTING_DISCORDS , strBuffer, "Auto Deskew1 and Auto Deskew2 lens setting discords");
				AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
				pDX->Fail();
			}
		}
	}
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CDeskewDlg, CDialog)
	//{{AFX_MSG_MAP(CDeskewDlg)
	ON_BN_CLICKED(IDC_AUTO_DESKEW1_SETUP_BUTTON, OnAutoDeskew1SetupButton)
	ON_BN_CLICKED(IDC_AUTO_DESKEW2_SETUP_BUTTON, OnAutoDeskew2SetupButton)
	ON_BN_CLICKED(IDC_MANUAL_DESKEW1_SETUP_BUTTON, OnManualDeskew1SetupButton)
	ON_BN_CLICKED(IDC_MANUAL_DESKEW2_SETUP_BUTTON, OnManualDeskew2SetupButton)
	ON_CBN_SELCHANGE(IDC_DESKEW_EXECUTION_MODE, OnSelchangeDeskewExecutionMode)
	ON_CBN_SELCHANGE(IDC_DESKEW_FAIL_OPTION, OnSelchangeDeskewFailOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CDeskewDlg メッセージ ハンドラ

BOOL CDeskewDlg::OnInitDialog()
{
	TCHAR szFilePath[MAX_PATH];
	TCHAR szFilePath2[MAX_PATH];
	CString strBuffer;

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// BackUp Recipe Data /////
	memcpy(&m_OldStageProgInfoHdr, m_pStageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));

// 2013.02.22 bagus Substrate thickness setting -->
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
// 2013.02.22 bagus Substrate thickness setting <--

	CDialog::OnInitDialog();

	///// Static Control /////
	m_stcAutoDeskew1_X.SetBkColor(REFERENCE_COLOR);
	m_stcAutoDeskew1_Y.SetBkColor(REFERENCE_COLOR);
	m_stcAutoDeskew2_X.SetBkColor(REFERENCE_COLOR);
	m_stcAutoDeskew2_Y.SetBkColor(REFERENCE_COLOR);
	m_stcManualDeskew1_X.SetBkColor(REFERENCE_COLOR);
	m_stcManualDeskew1_Y.SetBkColor(REFERENCE_COLOR);
	m_stcManualDeskew2_X.SetBkColor(REFERENCE_COLOR);
	m_stcManualDeskew2_Y.SetBkColor(REFERENCE_COLOR);

	m_AutoDeskew1SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AutoDeskew1SetupButton.DrawFlatFocus(TRUE);
	m_AutoDeskew2SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AutoDeskew2SetupButton.DrawFlatFocus(TRUE);
	m_ManualDeskew1SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ManualDeskew1SetupButton.DrawFlatFocus(TRUE);
	m_ManualDeskew2SetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ManualDeskew2SetupButton.DrawFlatFocus(TRUE);
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	::DeleteFile(DESKEW_IMG_TEMP);
	::DeleteFile(DESKEW_IMG_1);
	::DeleteFile(DESKEW_IMG_2);

// 2009.06.11 K.Matsuo -->
	// 読込み異常時は、リセット
	switch ( m_pStageProgInfoHdr->nDeskewMode ) {
	case DESKEW_MODE_PATTERN_DESKEW:
		_stprintf(szFilePath,  "%s%s.D1", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		_stprintf(szFilePath2, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		// DeskewImage ファイル存在チェック
		if ( ::GetFileAttributes(szFilePath) == 0xffffffff || ::GetFileAttributes(szFilePath2) == 0xffffffff ) {
			LoadStringML(IDS_DESKEW_IMAGE_NOT_FOUND, strBuffer, "An image file of deskew is not found. The content of deskew setting is cleared.");
			AfxMessageBox(strBuffer, MB_ICONEXCLAMATION);
			::DeleteFile(szFilePath);
			::DeleteFile(szFilePath2);

			m_pStageProgInfoHdr->iLens = 0;									// レンズ
			m_pStageProgInfoHdr->nDeskewMode = 0;							// Deskewの種類
			m_pStageProgInfoHdr->nDeskewFailOption = 0;						// パターン認識失敗時の設定
			::ZeroMemory(m_pStageProgInfoHdr->DeskewSite, sizeof(m_pStageProgInfoHdr->DeskewSite));					// パターン認識
			::ZeroMemory(m_pStageProgInfoHdr->DeskewSiteManual,  sizeof(m_pStageProgInfoHdr->DeskewSiteManual));		// Manual Deskew
			UpdateData(FALSE);
		}
		break;
	default:
		break;
	}
// 2009.06.11 K.Matsuo <--

	switch ( m_pStageProgInfoHdr->nDeskewMode ) {
	case DESKEW_MODE_NO_DESKEW:
	case DESKEW_MODE_PATTERN_DESKEW:
	case DESKEW_MODE_MANUAL_DESKEW:
		_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::CopyFile(szFilePath, DESKEW_IMG_1, FALSE);
		m_stcDeskewImg[0].SetBitmap(DESKEW_IMG_1);
		_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::CopyFile(szFilePath, DESKEW_IMG_2, FALSE);
		m_stcDeskewImg[1].SetBitmap(DESKEW_IMG_2);
		break;
	case DESKEW_MODE_MAX:
		_ASSERT(FALSE);
		break;
	}

	// 2009.05.07 AutoDeskewのレンズを、一致させる対応
	m_iAutoDeskewLensCheck[0] = m_pStageProgInfoHdr->iLens;
	m_iAutoDeskewLensCheck[1] = m_pStageProgInfoHdr->iLens;

	///// Deskew Execution Mode /////
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_DESKEW_EXECUTION_MODE);
	pCombo->ResetContent();
	for ( int i = 0; i < DESKEW_MODE_MAX; i++ ){
		if(l_SystemConfig.nLanguage == LANGUAGE_ENGLISH)
			pCombo->AddString(g_lpszDeskewMode_ENU[i]);
		else
			pCombo->AddString(g_lpszDeskewMode_JPN[i]);
	}
	pCombo->SetCurSel(m_pStageProgInfoHdr->nDeskewMode);

	///// Deskew Fail Option /////
	pCombo = (CComboBox*)GetDlgItem(IDC_DESKEW_FAIL_OPTION);
	pCombo->ResetContent();
	for ( i = 0; i < DESKEW_FAIL_OPTION_MAX; i++ ){
		if(l_SystemConfig.nLanguage == LANGUAGE_ENGLISH)
			pCombo->AddString(g_lpszDeskewFailOption_ENU[i]);
		else
			pCombo->AddString(g_lpszDeskewFailOption_JPN[i]);
	}
	pCombo->SetCurSel(m_pStageProgInfoHdr->nDeskewFailOption);

	OnSelchangeDeskewExecutionMode();
	OnSelchangeDeskewFailOption();

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CDeskewDlg::OnOK()
{
	TCHAR szFilePath[MAX_PATH];

	if ( !UpdateData() )
		return;

	CDialog::OnOK();

#if 0
	switch ( m_pStageProgInfoHdr->nDeskewMode ) {
	case DESKEW_MODE_NO_DESKEW:
		// CleanUp 定義済みフラグ等の消しこみ
		m_pStageProgInfoHdr->DeskewSite[0].bDefined = FALSE;
		m_pStageProgInfoHdr->DeskewSite[0].Loc.lX = 0;
		m_pStageProgInfoHdr->DeskewSite[0].Loc.lY = 0;
		_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::DeleteFile(szFilePath);
		m_pStageProgInfoHdr->DeskewSite[1].bDefined = FALSE;
		m_pStageProgInfoHdr->DeskewSite[1].Loc.lX = 0;
		m_pStageProgInfoHdr->DeskewSite[1].Loc.lY = 0;
		_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::DeleteFile(szFilePath);
		m_pStageProgInfoHdr->DeskewSiteManual[0].bDefined = FALSE;
		m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lX = 0;
		m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lY = 0;
		m_pStageProgInfoHdr->DeskewSiteManual[1].bDefined = FALSE;
		m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lX = 0;
		m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lY = 0;
		break;
	case DESKEW_MODE_PATTERN_DESKEW:
		if ( m_pStageProgInfoHdr->nDeskewFailOption != DESKEW_FAIL_OPTION_MANUAL_DESKEW ) {
			// CleanUp 定義済みフラグ等の消しこみ
			m_pStageProgInfoHdr->DeskewSiteManual[0].bDefined = FALSE;
			m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lX = 0;
			m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lY = 0;
			m_pStageProgInfoHdr->DeskewSiteManual[1].bDefined = FALSE;
			m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lX = 0;
			m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lY = 0;
		}
		// パターン認識画像の保存
		_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::CopyFile(DESKEW_IMG_1, szFilePath, FALSE);
		_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::CopyFile(DESKEW_IMG_2, szFilePath, FALSE);
		break;
	case DESKEW_MODE_MANUAL_DESKEW:
		// CleanUp 定義済みフラグ等の消しこみ
		m_pStageProgInfoHdr->DeskewSite[0].bDefined = FALSE;
		m_pStageProgInfoHdr->DeskewSite[0].Loc.lX = 0;
		m_pStageProgInfoHdr->DeskewSite[0].Loc.lY = 0;
		_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::DeleteFile(szFilePath);
		m_pStageProgInfoHdr->DeskewSite[1].bDefined = FALSE;
		m_pStageProgInfoHdr->DeskewSite[1].Loc.lX = 0;
		m_pStageProgInfoHdr->DeskewSite[1].Loc.lY = 0;
		_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::DeleteFile(szFilePath);
		break;
	case DESKEW_MODE_MAX:
		_ASSERT(FALSE);
		break;
	}
#endif // 0

	switch ( m_pStageProgInfoHdr->nDeskewMode ) {
	case DESKEW_MODE_PATTERN_DESKEW:
		// パターン認識画像の保存
		_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::CopyFile(DESKEW_IMG_1, szFilePath, FALSE);
		_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_pStageProgInfoHdr->hdr.szName);
		::CopyFile(DESKEW_IMG_2, szFilePath, FALSE);
		break;
	}
}

// =========================================================================
//
void CDeskewDlg::OnCancel()
{
	CString strBuffer;

	CSaveModifiedDlg dlg;

	if ( memcmp(m_pStageProgInfoHdr, &m_OldStageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR)) != 0 ) {
		LoadStringML(IDS_DESKEW_PROGRAM , strBuffer, "DESKEW PROGRAM");
		dlg.m_strModified = strBuffer;

		if ( dlg.DoModal() == IDCANCEL )
			return;

		if ( dlg.m_iModified == MODIFIED_SAVE ) {
			if ( !UpdateData() )
				return;
			OnOK();
		}
		else {
			///// Restore Data /////
			memcpy(m_pStageProgInfoHdr, &m_OldStageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
		}
	}

	CDialog::OnCancel();
}

// =========================================================================
//
void CDeskewDlg::OnSelchangeDeskewExecutionMode()
{
	int iSel = ((CComboBox*)GetDlgItem(IDC_DESKEW_EXECUTION_MODE))->GetCurSel();

	m_pStageProgInfoHdr->nDeskewMode = iSel;

	GetDlgItem(IDC_AUTO_DESKEW1_SETUP_BUTTON)->EnableWindow(iSel == DESKEW_MODE_PATTERN_DESKEW);
	GetDlgItem(IDC_AUTO_DESKEW2_SETUP_BUTTON)->EnableWindow(iSel == DESKEW_MODE_PATTERN_DESKEW);
	GetDlgItem(IDC_MANUAL_DESKEW1_SETUP_BUTTON)->EnableWindow(iSel == DESKEW_MODE_MANUAL_DESKEW);
	GetDlgItem(IDC_MANUAL_DESKEW2_SETUP_BUTTON)->EnableWindow(iSel == DESKEW_MODE_MANUAL_DESKEW);

	/* DeskewFailOption コンボボックスは、DeskewExecutionMode コンボボックスが DESKEW_MODE_PATTERN_DESKEW のとき選択可能 */
	GetDlgItem(IDC_DESKEW_FAIL_OPTION)->EnableWindow(iSel == DESKEW_MODE_PATTERN_DESKEW);
}

// =========================================================================
//
void CDeskewDlg::OnSelchangeDeskewFailOption()
{
	// DeskewFailOption コンボボックスは、DeskewExecutionMode コンボボックスが DESKEW_MODE_PATTERN_DESKEW のとき選択可能だが、
	// 初期値編集で呼ばれるので、ガードしておく
	if ( m_pStageProgInfoHdr->nDeskewMode != DESKEW_MODE_PATTERN_DESKEW )
		return;

	int iSel = ((CComboBox*)GetDlgItem(IDC_DESKEW_FAIL_OPTION))->GetCurSel();

	m_pStageProgInfoHdr->nDeskewFailOption = iSel;

	// DeskewFailOption で マニュアルDeskewが選択されたとき、マニュアルセットアップボタンの使用を許可する
	GetDlgItem(IDC_MANUAL_DESKEW1_SETUP_BUTTON)->EnableWindow(iSel == DESKEW_FAIL_OPTION_MANUAL_DESKEW);
	GetDlgItem(IDC_MANUAL_DESKEW2_SETUP_BUTTON)->EnableWindow(iSel == DESKEW_FAIL_OPTION_MANUAL_DESKEW);
}

// =========================================================================
//
void CDeskewDlg::OnAutoDeskew1SetupButton()
{
	::CopyFile(DESKEW_IMG_1, DESKEW_IMG_TEMP, FALSE);

	CPatRecDeskewDlg dlg(DESKEW_MODE, m_pStageProgInfoHdr->iLens, &m_pStageProgInfoHdr->DeskewSite[0]);
	//Sampleセット
	dlg.SetFileName(m_pStageProgInfoHdr->SampleInfo.szName);
	dlg.SetSampleMode(1);

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(m_pStageProgInfoHdr->SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	if ( dlg.DoModal() == IDOK ) {
		m_pStageProgInfoHdr->iLens = dlg.m_iLens;
		// 2009.05.07 AutoDeskewのレンズを、一致させる対応
		m_iAutoDeskewLensCheck[0] = m_pStageProgInfoHdr->iLens;
		m_pStageProgInfoHdr->DeskewSite[0].bDefined = TRUE;
		m_pStageProgInfoHdr->DeskewSite[0].Loc.lX = dlg.m_lX;
		m_pStageProgInfoHdr->DeskewSite[0].Loc.lY = dlg.m_lY;
		UpdateData(FALSE);

		::CopyFile(DESKEW_IMG_TEMP, DESKEW_IMG_1, FALSE);
		m_stcDeskewImg[0].SetBitmap(DESKEW_IMG_1);
	}
}

// =========================================================================
//
void CDeskewDlg::OnAutoDeskew2SetupButton()
{
	::CopyFile(DESKEW_IMG_2, DESKEW_IMG_TEMP, FALSE);

	CPatRecDeskewDlg dlg(DESKEW_MODE, m_pStageProgInfoHdr->iLens, &m_pStageProgInfoHdr->DeskewSite[1]);
	//Sampleセット
	dlg.SetFileName(m_pStageProgInfoHdr->SampleInfo.szName);
	dlg.SetSampleMode(1);

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(m_pStageProgInfoHdr->SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	if ( dlg.DoModal() == IDOK ) {
		m_pStageProgInfoHdr->iLens = dlg.m_iLens;
		// 2009.05.07 AutoDeskewのレンズを、一致させる対応
		m_iAutoDeskewLensCheck[1] = m_pStageProgInfoHdr->iLens;
		m_pStageProgInfoHdr->DeskewSite[1].bDefined = TRUE;
		m_pStageProgInfoHdr->DeskewSite[1].Loc.lX = dlg.m_lX;
		m_pStageProgInfoHdr->DeskewSite[1].Loc.lY = dlg.m_lY;
		UpdateData(FALSE);

		::CopyFile(DESKEW_IMG_TEMP, DESKEW_IMG_2, FALSE);
		m_stcDeskewImg[1].SetBitmap(DESKEW_IMG_2);
	}
}

// =========================================================================
//
void CDeskewDlg::OnManualDeskew1SetupButton()
{
	CString strBuffer;

	COverlayDialog dlg;

	LoadStringML(IDS_MANUAL_DESKEW_SETUP , strBuffer, "MANUAL DESKEW SETUP");
	dlg.SetCaption(strBuffer);
	LoadStringML(IDS_CLICK_CORRECT_POS , strBuffer, "CLICK CORRECT POSITION");
	dlg.SetMessage(strBuffer);

	//Sampleセット
	dlg.SetFileName(m_pStageProgInfoHdr->SampleInfo.szName);
	dlg.SetSampleMode(1);

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(m_pStageProgInfoHdr->SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	dlg.m_lX = m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lX;
	dlg.m_lY = m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lY;

	if ( m_pStageProgInfoHdr->DeskewSiteManual[0].bDefined ) {
		STAGE_COORD coord =	m_pStageProgInfoHdr->DeskewSiteManual[0].Loc;
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		NS_StageMoveAbsolute(&coord);
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}

// 2009.07.29 K.Matsuo Add -->
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
// 2009.07.29 K.Matsuo Add <--

	if ( dlg.DoModal() == IDOK ) {
		m_pStageProgInfoHdr->DeskewSiteManual[0].bDefined = TRUE;
		m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lX = dlg.m_lX;
		m_pStageProgInfoHdr->DeskewSiteManual[0].Loc.lY = dlg.m_lY;
		UpdateData(FALSE);
	}
}

// =========================================================================
//
void CDeskewDlg::OnManualDeskew2SetupButton()
{
	CString strBuffer;

	COverlayDialog dlg;

	LoadStringML(IDS_MANUAL_DESKEW_SETUP , strBuffer, "MANUAL DESKEW SETUP");
	dlg.SetCaption(strBuffer);
	LoadStringML(IDS_CLICK_CORRECT_POS , strBuffer, "CLICK CORRECT POSITION");
	dlg.SetMessage(strBuffer);

	//Sampleセット
	dlg.SetFileName(m_pStageProgInfoHdr->SampleInfo.szName);
	dlg.SetSampleMode(1);

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(m_pStageProgInfoHdr->SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	dlg.m_lX = m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lX;
	dlg.m_lY = m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lY;

	if ( m_pStageProgInfoHdr->DeskewSiteManual[1].bDefined ) {
		STAGE_COORD coord =	m_pStageProgInfoHdr->DeskewSiteManual[1].Loc;
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		NS_StageMoveAbsolute(&coord);
		((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->GetActiveDocument())->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}

// 2009.07.29 K.Matsuo Add -->
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
// 2009.07.29 K.Matsuo Add <--

	if ( dlg.DoModal() == IDOK ) {
		m_pStageProgInfoHdr->DeskewSiteManual[1].bDefined = TRUE;
		m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lX = dlg.m_lX;
		m_pStageProgInfoHdr->DeskewSiteManual[1].Loc.lY = dlg.m_lY;
		UpdateData(FALSE);
	}
}
