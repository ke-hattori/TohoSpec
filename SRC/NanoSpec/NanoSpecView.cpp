// NanoSpecView.cpp : CNanoSpecView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "NanoSpecView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CNanoSpecView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CNanoSpecView, CNanoUI)

// =========================================================================
//
BEGIN_MESSAGE_MAP(CNanoSpecView, CNanoUI)
	//{{AFX_MSG_MAP(CNanoSpecView)
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

// =========================================================================
// CNanoSpecView クラスの構築/消滅

// =========================================================================
//
CNanoSpecView::CNanoSpecView()
	: CNanoUI(CNanoSpecView::IDD)
{
	//{{AFX_DATA_INIT(CNanoSpecView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CNanoSpecView::~CNanoSpecView()
{
}

// =========================================================================
//
void CNanoSpecView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNanoSpecView)
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BOOL CNanoSpecView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//	修正してください。

	return CNanoUI::PreCreateWindow(cs);
}

// =========================================================================
//
void CNanoSpecView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

// =========================================================================
// CNanoSpecView クラスの印刷

// =========================================================================
//
BOOL CNanoSpecView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

// =========================================================================
//
void CNanoSpecView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

// =========================================================================
//
void CNanoSpecView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

// =========================================================================
//
void CNanoSpecView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷用のコードをここに追加してください。
}

// =========================================================================
// CNanoSpecView クラスの診断

#ifdef _DEBUG
// =========================================================================
//
void CNanoSpecView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CNanoSpecView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}

// =========================================================================
//
CNanoSpecDoc* CNanoSpecView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNanoSpecDoc)));
	return (CNanoSpecDoc*)m_pDocument;
}
#endif //_DEBUG

// =========================================================================
// CNanoSpecView クラスのメッセージ ハンドラ
