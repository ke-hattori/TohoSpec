// SaveAsDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include <RecipeFile.hxx>
#include <PatternFile.hxx>
#include "NanoSpec.h"
#include "SaveAsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg ダイアログ												   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CSaveAsDlg::CSaveAsDlg(int iMode, CWnd* pParent /*=NULL*/)
	: CDialog(CSaveAsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveAsDlg)
	//}}AFX_DATA_INIT
	m_iMode = iMode;
	m_strName = "";
	LoadStringML(IDS_TITLE_SAVE_AS, m_strCaption, "SAVE AS");
}

/////////////////////////////////////////////////////////////////////////////
//
void CSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveAsDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, RECIPE_NAME_LEN);
	DDV_CustomRecipeNameEmptyCheck(pDX, m_strName);
	DDV_CustomCheckNameCheck(pDX, m_strName);
	DDV_CustomSystemRecipeBeginningCharCheck(pDX, m_strName);
	DDV_CustomUntitledNameCheck(pDX, m_strName);
}

/////////////////////////////////////////////////////////////////////////////
//
void CSaveAsDlg::DDV_CustomRecipeNameEmptyCheck(CDataExchange* pDX, const CString& value)
{
	/* 名前の入力なし */

	CString strBuffer;

	if ( pDX->m_bSaveAndValidate ) {
		if ( value.IsEmpty() ) {
			LoadStringML(IDS_ENTER_A_NAME, strBuffer, "Please enter a name");
			MessageBox(strBuffer, m_strCaption, MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CSaveAsDlg::DDV_CustomCheckNameCheck(CDataExchange* pDX, const CString& value)
{
	/* ファイル名に含むことをできない文字を付けて保存することを禁止する */

	CString strBuffer;

	if ( pDX->m_bSaveAndValidate ) {
		if ( !CheckName(value, RECIPE_NAME_LEN) ) {
			LoadStringML(IDS_NAME_NOT_CONTAIN_SPECE, strBuffer, "Name can not contain spaces or punctuation characters");
			MessageBox(strBuffer, m_strCaption, MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CSaveAsDlg::DDV_CustomSystemRecipeBeginningCharCheck(CDataExchange* pDX, const CString& value)
{
	/* アンダーバーで始まる名前を付けて保存することを禁止する */

	CString strBuffer;

	if ( pDX->m_bSaveAndValidate ) {
		if ( value[0] == SYSTEM_RECIPE_BEGINNING_CHAR ) {
			LoadStringML(IDS_UNDER_BAR_NOT_SPECIFIED, strBuffer, "_ (under bar) cannot be specified for the beginning character of the name");
			MessageBox(strBuffer, m_strCaption, MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
}

// 2009.06.12 K.Matsuo -->
/////////////////////////////////////////////////////////////////////////////
//
void CSaveAsDlg::DDV_CustomUntitledNameCheck(CDataExchange* pDX, const CString& value)
{
	/* 新規作成（"Untitled"、"タイトルなし"など）時と同じ名前を付けて保存することを禁止する */

	CString strBuffer;
	CString strUntitled;

	if ( pDX->m_bSaveAndValidate ) {
		LoadStringML(IDS_UNTITLE, strUntitled, "Untitled");
		if ( value == strUntitled ) {
			LoadStringML(IDS_NAME_NOT_ENTER, strBuffer, "This name cannot be entered. Please enter a different name");
			MessageBox(strBuffer, m_strCaption, MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
}
// 2009.06.12 K.Matsuo <--

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CSaveAsDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveAsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSaveAsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CSaveAsDlg::OnOK()
{
	BOOL bExist = FALSE;
	TCHAR szName[RECIPE_NAME_LEN + 1];
	TCHAR szMessage[256];
	SYSTEMTIME lastWriteSystemTime;
	CString strBuffer;

	if ( !UpdateData() )
		return;

	memset(m_szFilePath, 0, sizeof(m_szFilePath));
	strncpy(szName, m_strName, RECIPE_NAME_LEN);
	szName[RECIPE_NAME_LEN] = '\0';
	// それぞれのモードで同じ名前のファイルが存在するかを調べる
	switch(m_iMode){
	case SAVE_AS_MAIN_RECIPE:
		bExist = RecipeFile_ExistRecipe(szName, &lastWriteSystemTime, RECIPE_FILE_MAIN_RECIPE);
		break;
	case SAVE_AS_MULTI_RECIPE:
		bExist = RecipeFile_ExistRecipe(szName, &lastWriteSystemTime, RECIPE_FILE_MULTI_RECIPE);
		break;
	case SAVE_AS_MEASUREMENT_PROGRAM:
		bExist = RecipeFile_ExistRecipe(szName, &lastWriteSystemTime, RECIPE_FILE_MEASUREMENT_PROGRAM);
		break;
	case SAVE_AS_STAGE_PROGRAM:
		bExist = RecipeFile_ExistRecipe(szName, &lastWriteSystemTime, RECIPE_FILE_STAGE_PROGRAM);
		break;
	case SAVE_AS_RECALIBRATION_PROGRAM:
		bExist = RecipeFile_ExistRecipe(szName, &lastWriteSystemTime, RECIPE_FILE_RECALIBRATION_PROGRAM);
		break;
	case SAVE_AS_POINT_DESKEW_PROGRAM:
		bExist = RecipeFile_ExistRecipe(szName, &lastWriteSystemTime, RECIPE_FILE_POINT_DESKEW_PROGRAM);
		break;
	case SAVE_AS_SITE_PATTERN:
		PatternFile_MakePatternFilePath(m_szFilePath, szName);
		bExist = PatternFile_ExistPatternFilePath(m_szFilePath, &lastWriteSystemTime);
		break;
	case SAVE_AS_SITE_PATTERN_IMG:
//		PatternFile_MakePatternImageFilePath(m_szFilePath, szName, m_iLens);
		sprintf(m_szFilePath, "%s%s%s", g_szDb_Pattern_Img_Dir, szName, PATTERN_IMAGE_EXT);
		bExist = PatternFile_ExistPatternFilePath(m_szFilePath, &lastWriteSystemTime);
		break;
	case SAVE_AS_OVERLAY_CAPTURE:
		sprintf(m_szFilePath, "%s%s%s", g_szResult_Dir, szName, BMP_EXT);
		bExist = CheckFileExists(m_szFilePath, &lastWriteSystemTime);
		break;
	default:
		return;
	}

	// ファイルが存在している場合はOverwriteするかを確認する
	if ( bExist) {
		LoadStringML(IDS_NAME_ALREADY_EXIST, strBuffer, "Name ""%s"" already exists, overwrite?");
		sprintf(szMessage, strBuffer, m_strName);
		if ( MessageBox(szMessage, m_strCaption, MB_OKCANCEL) == IDOK ) {
			EndDialog(IDOK);
		}
		else{
			GetDlgItem(IDC_NAME)->SetFocus();
		}
	}
	else{
		EndDialog(IDOK);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSaveAsDlg::CheckFileExists(LPCTSTR pszFileName, SYSTEMTIME* lpLastWriteSystemTime)
{
	/* ファイルが存在するかを確認する関数（存在する場合、あわせて最終更新日時を設定する） */

	WIN32_FILE_ATTRIBUTE_DATA win32Data;
	FILETIME localFileTime;

	if ( !GetFileAttributesEx(pszFileName, GetFileExInfoStandard, &win32Data) )
		return FALSE;
	::FileTimeToLocalFileTime(&win32Data.ftLastWriteTime, &localFileTime);
	::FileTimeToSystemTime(&localFileTime, lpLastWriteSystemTime);
	return TRUE;
}
