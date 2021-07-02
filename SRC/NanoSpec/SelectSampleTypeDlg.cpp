// SelectSampleTypeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "SelectSampleTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSelectSampleTypeDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSelectSampleTypeDlg::CSelectSampleTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectSampleTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectSampleTypeDlg)
	//}}AFX_DATA_INIT
	//2009.08.28 bagus stress --{--
// 2009.10.09 bagus StagePGM 共通化 --{--
//	m_iHeadType = HEAD_TYPE_SR;
	m_iHeadType = STAGE_PGM_TYPE_STANDARD;
// 2009.10.09 bagus StagePGM 共通化 --}--
	//2009.08.28 bagus stress --}--
}

// =========================================================================
//
void CSelectSampleTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectSampleTypeDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSelectSampleTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectSampleTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSelectSampleTypeDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSelectSampleTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	memset(m_szName, 0, sizeof(m_szName));

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	if(!InitCombo_Sample()){
		EndDialog(IDCANCEL);
		return FALSE;
	}

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSelectSampleTypeDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_SAMPLE);
	int iSelect = pCombo->GetCurSel();

	if(iSelect == CB_ERR){
		return;
	}

	memset(m_szName, 0, sizeof(m_szName));
	pCombo->GetLBText(iSelect, m_szName);

	CDialog::OnOK();
}

// =========================================================================
//
BOOL CSelectSampleTypeDlg::InitCombo_Sample()
{
	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_SAMPLE);
	CFileFind Finder;
	char szDirPath[_MAX_PATH];
	CString csName;
	BOOL bLoop;
	//Saiki 20090601 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090601 Add <-----

	pCombo->ResetContent();

	sprintf(szDirPath, "%s*%s", g_szCfg_System_Sample_Dir, DAT_EXT);
	bLoop = Finder.FindFile(szDirPath);
	//2009.08.28 bagus stress --{--
	//データの絞込み用のためにストレスコンフィグを読み込む
	STRESS_CONFIG l_StressConfig;
	memset(&l_StressConfig,NULL,sizeof(l_StressConfig));
// 2009.10.09 bagus StagePGM 共通化 --{--
//	if(m_iHeadType == HEAD_TYPE_STRESS){
	if(m_iHeadType == STAGE_PGM_TYPE_STRESS){
// 2009.10.09 bagus StagePGM 共通化 --}--
		ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	}
	//2009.08.28 bagus stress --}--

	while(bLoop){
		bLoop = Finder.FindNextFile();

		if(Finder.IsDots()){
			continue;
		}

		csName = Finder.GetFileTitle();
		csName.Insert(RECIPE_NAME_LEN, '\0');
		//2009.08.28 bagus stress --{--
		//ストレスの場合にはStress Line/Section Settingで指定されたサンプルの大きさ、原点座標と同じものしか表示しない
// 2009.10.09 bagus StagePGM 共通化 --{--
#if 0
		switch(m_iHeadType){
		case HEAD_TYPE_SR:
		default:
			break;
		case HEAD_TYPE_STRESS:
			{
				SAMPLEINFO l_SampleInfo;
				memset(&l_SampleInfo,NULL,sizeof(l_SampleInfo));
				if ( RecipeFile_LoadRecipe(&l_SampleInfo, csName.GetBuffer(0), RECIPE_FILE_SAMPLE) ){
					if(l_SampleInfo.Size.dx == l_StressConfig.Size.dx
					&& l_SampleInfo.Size.dy == l_StressConfig.Size.dy
					&& l_SampleInfo.Origin.lX == l_StressConfig.Origin.lX
					&& l_SampleInfo.Origin.lY == l_StressConfig.Origin.lY){
						//一致
					}else{
						//不一致なのでリストに追加しない
						continue;
					}

				}else{
					//サンプルの情報が読み込めないので何もしない
					continue;
				}

			}
			break;
		}
#else
		switch(m_iHeadType){
		case STAGE_PGM_TYPE_STRESS:
			{
				SAMPLEINFO l_SampleInfo;
				memset(&l_SampleInfo,NULL,sizeof(l_SampleInfo));
				if ( RecipeFile_LoadRecipe(&l_SampleInfo, csName.GetBuffer(0), RECIPE_FILE_SAMPLE) ){
// 2009.12.14 bagus 修正 --{--
#if 0
					if(l_SampleInfo.Size.dx == l_StressConfig.Size.dx
					&& l_SampleInfo.Size.dy == l_StressConfig.Size.dy
					&& l_SampleInfo.Origin.lX == l_StressConfig.Origin.lX
					&& l_SampleInfo.Origin.lY == l_StressConfig.Origin.lY){
#else
					if (strcmp(l_SampleInfo.szName, csName.GetBuffer(0)) == 0) {
#endif
// 2009.12.14 bagus 修正 --}--
						//一致
					}else{
						//不一致なのでリストに追加しない
						continue;
					}

				}else{
					//サンプルの情報が読み込めないので何もしない
					continue;
				}

			}
			break;
		default:
			break;
		}
#endif
// 2009.10.09 bagus StagePGM 共通化 --}--
		//2009.08.28 bagus stress --}--

		pCombo->AddString(csName);
	}

	if(pCombo->GetCount() <= 0){
		//MessageBox("Please Sample Setting", "SELECT SAMPLE TYPE", MB_OK | MB_ICONSTOP);
		//Saiki 20090601 Change ----->
		//MessageBox("サンプル設定を行ってください", "SELECT SAMPLE TYPE", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_SAMPLE_SETTING, strBuffer, "Please Sample Setting");
		LoadStringML(IDS_SELECT_SAMPLE_TYPE, strTitle, "SELECT SAMPLE TYPE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
		//Saiki 20090601 Change <-----
		return FALSE;
	}

	pCombo->SetCurSel(0);

	return TRUE;
}
