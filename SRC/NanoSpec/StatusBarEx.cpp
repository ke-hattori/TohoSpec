/////////////////////////////////////////////////////////////////////////////
// StatusBarEx.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "StatusBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StatusBarEx

/////////////////////////////////////////////////////////////////////////////
// MODULE  : CStatusBarEx
// ABSTRACT: コンストラクタ.
// FUNCTION:
// RETURN  :
CStatusBarEx::CStatusBarEx()
{
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : ~CStatusBarEx
// ABSTRACT: デストラクタ.
// FUNCTION:
// RETURN  :
CStatusBarEx::~CStatusBarEx()
{
	for(int i=0 ; i<m_Panes.GetSize() ; i++)
		delete m_Panes[i];
	m_Panes.RemoveAll();
}

BEGIN_MESSAGE_MAP(CStatusBarEx, CStatusBar)
	//{{AFX_MSG_MAP(StatusBarEx)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
//	StatusBarEx message handlers

/////////////////////////////////////////////////////////////////////////////
// MODULE  : lFindBufferByIndex
// ABSTRACT: 指定されたペイン番号を持つバッファの位置をペインのインデックスから検索.
// FUNCTION:
// RETURN  : -1 = エラー
int CStatusBarEx::lFindBufferByIndex( int index)
{
	for(int i = 0 ; i < m_Panes.GetSize() ; i++){
		if(m_Panes[i]->nIndex == index) return i;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : lFindBufferByIndex
// ABSTRACT: 指定されたペイン番号を持つバッファの位置を文字列IDから検索.
// FUNCTION:
// RETURN  : -1 = エラー
int CStatusBarEx::lFindBufferByStrId( UINT strId)
{
	for(int i = 0 ; i < m_Panes.GetSize() ; i++){
		if(m_Panes[i]->uStrId == strId) return i;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : DrawItem
// ABSTRACT: オーナー描画処理.
// FUNCTION:
// RETURN  :
void CStatusBarEx::DrawItem( LPDRAWITEMSTRUCT lpDIS)
{
	//	色データの検索;
	int n = lFindBufferByIndex( lpDIS->itemID);

	if(n < 0) return;

	CDC dc;
	dc.Attach( lpDIS->hDC);
	dc.SetBkMode( TRANSPARENT);		// 文字の背景色を透明にする.

	CRect rect( &lpDIS->rcItem);

	dc.FillSolidRect( &rect, m_Panes[n]->BackColor);
	dc.SetTextColor( m_Panes[n]->TextColor);
	dc.DrawText(m_Panes[n]->sText, &rect,DT_CENTER);

	dc.Detach();
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : lInitPane
// ABSTRACT: ペインの初期化共通内部処理.
// FUNCTION:
// RETURN  :
int CStatusBarEx::lInitPane( UINT strId, COLORREF TextColor, COLORREF BackColor)
{
	int index = CommandToIndex( strId);

	if(index < 0) return FALSE;

	PANE_DATA *pPane = new PANE_DATA;

	CString cs = GetPaneText( index);
	GetStatusBarCtrl().SetText( cs, index, SBT_OWNERDRAW);

	pPane->nIndex		= index;
	pPane->uStrId		= strId;
	pPane->TextColor	= TextColor;
	pPane->BackColor	= BackColor;
	pPane->sText		= cs;
	m_Panes.Add( pPane);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : Initialize
// ABSTRACT: ペインの初期化.
// FUNCTION: 色の指定も行う
//		strId = ペインの文字列のID
// RETURN  :
int CStatusBarEx::Initialize( UINT strId, COLORREF TextColor, COLORREF BackColor)
{
	return lInitPane(strId, TextColor, BackColor);
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : Initialize
// ABSTRACT: ペインの初期化.
// FUNCTION: 色の指定は行わない。 色は通常のシステム色になる。
//		strId = ペインの文字列のID
//		bDisable = TRUEの場合、テキスト色が禁止状態の色になる。(デフォルトはFALSE)
// RETURN  :
int CStatusBarEx::Initialize( UINT strId, int bDisable/*=FALSE*/)
{
	COLORREF TextColor, BackColor;

	if(!bDisable)							// 標準テキスト色.
		TextColor = ::GetSysColor( COLOR_WINDOWTEXT);
	else									// 禁止状態のテキスト色.
		TextColor = ::GetSysColor( COLOR_GRAYTEXT);

	BackColor = ::GetSysColor( COLOR_3DFACE);

	return lInitPane( strId, TextColor, BackColor);
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : ChangeColors
// ABSTRACT: 表示色の変更.
// FUNCTION:
// RETURN  :
void CStatusBarEx::ChangeColor( UINT strId, COLORREF TextColor, COLORREF BackColor, int bUpdate/*=TRUE*/)
{
	int n = lFindBufferByStrId( strId);

	if(n < 0) return;

	m_Panes[n]->TextColor = TextColor;
	m_Panes[n]->BackColor = BackColor;

	if(bUpdate) Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : ChangeTextColor
// ABSTRACT: テキスト色の変更.
// FUNCTION:
// RETURN  :
void CStatusBarEx::ChangeTextColor( UINT strId, COLORREF TextColor, int bUpdate/*=TRUE*/)
{
	int n = lFindBufferByStrId( strId);

	if(n < 0) return;

	m_Panes[n]->TextColor = TextColor;

	if(bUpdate) Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : ChangeBackColor
// ABSTRACT: 背景色の変更.
// FUNCTION:
// RETURN  :
void CStatusBarEx::ChangeBackColor( UINT strId, COLORREF BackColor, int bUpdate/*=TRUE*/)
{
	int n = lFindBufferByStrId( strId);

	if(n < 0) return;

	m_Panes[n]->BackColor = BackColor;

	if(bUpdate) Invalidate( FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// MODULE  : ChangeText
// ABSTRACT: 表示文字列の変更
// FUNCTION: 機能はCStatusBarのSetPaneTextと同じだが、文字列IDにより指定する.
//		注意!! CStatusBarのSetPaneTextを使うとOWNERDRAWが解除されてしまう.
// RETURN  :
void CStatusBarEx::ChangeText( UINT strId, LPCTSTR pszNewText, int bUpdate/*=TRUE*/)
{
	int n = lFindBufferByStrId( strId);

	if(n < 0) return;

	m_Panes[n]->sText = pszNewText;

	if(bUpdate) Invalidate( FALSE);
}
