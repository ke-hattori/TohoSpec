// RecipeMainStressView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "RecipeMainStressView.h"
#include "..\\..\\INC\\PifComm.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecipeMainStressView

IMPLEMENT_DYNCREATE(CRecipeMainStressView, CNanoRecipeUI)

CRecipeMainStressView::CRecipeMainStressView()
	: CNanoRecipeUI(CRecipeMainStressView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeMainStressView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します
	//}}AFX_DATA_INIT
}

CRecipeMainStressView::~CRecipeMainStressView()
{
}

void CRecipeMainStressView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMainStressView)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しを追加します
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecipeMainStressView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMainStressView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecipeMainStressView 診断

/////////////////////////////////////////////////////////////////////////////
// CRecipeMainStressView メッセージ ハンドラ


void CRecipeMainStressView::LoadRecipeData()
{
}
BOOL CRecipeMainStressView::CheckData()
{
	return FALSE;
}
BOOL CRecipeMainStressView::IsDataChanged()
{
	return FALSE;
}
BOOL CRecipeMainStressView::SaveRecipeData()
{
	return TRUE;
}
BOOL CRecipeMainStressView::IsAccessPrivilege()
{
	return TRUE;
}
int CRecipeMainStressView::SaveAsDlgInfo()
{
	return SAVE_AS_MAIN_RECIPE;
}
