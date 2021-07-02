// NanoLineSelect.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoLineSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNanoLineSelect

CNanoLineSelect::CNanoLineSelect()
{
	m_BackColor = RGB(0xFF,0xFF,0xFF);
	m_LineColor = RGB(0xFF,0x00,0x00);
	m_PenStyle	= PS_SOLID;
}

CNanoLineSelect::~CNanoLineSelect()
{
}


BEGIN_MESSAGE_MAP(CNanoLineSelect, CButton)
	//{{AFX_MSG_MAP(CNanoLineSelect)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoLineSelect メッセージ ハンドラ

void CNanoLineSelect::OnClicked()
{
	CWnd *pWnd;

	this->SetCheck(!this->GetCheck());
	pWnd = this->GetParent();
	if(pWnd){
		TRACE("Checked=%d,ID=%d\r\n",(WPARAM)this->GetCheck(),this->GetDlgCtrlID());
		pWnd->PostMessage(WM_LINE_SELECT_CHANGE,(WPARAM)this->GetCheck(),this->GetDlgCtrlID());
	}
	Invalidate();
}

void CNanoLineSelect::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	UINT	uState = lpDIS->itemState;
	CRect	cFocusRect;
	CRect	cButtonRect;
	CRect	cTmpRect;


	cFocusRect.CopyRect(&lpDIS->rcItem);
	cButtonRect.CopyRect(&lpDIS->rcItem);

	ClearArea(pDC,cButtonRect);
	// +------------------------+
	// | V ----  Text           |
	// +------------------------+
	//Draw CheckBox State (V)
	DrawCheckBoxState(pDC,cButtonRect);
	//Draw Pen Guidence  (----)
	DrawPenGuidence(pDC,cButtonRect);
	//Draw Text (Text)
	DrawLineText(pDC,cButtonRect);
}

//--------------------------------------------------
//		チェックボックス描画エリア 初期化
//--------------------------------------------------
void CNanoLineSelect::ClearArea(CDC *pDC,RECT *pScreenSize)
{
	CBrush cCanvasColor(m_BackColor);
	CBrush *pOldBrush;

	pOldBrush = pDC->SelectObject(&cCanvasColor);
	pDC->FillRect(pScreenSize,&cCanvasColor);

	pDC->SelectObject(pOldBrush);
}

//--------------------------------------------------
//		チェックボックス描画
//--------------------------------------------------
//   +------------------------------------------------+
//   |                      U1                  |     |
//   |     +------------------------------------|     |
//   |     |                U2            |     |     |
//   |     |     +------------------------|     |     |
//   |     |     | BackArea               |     |     |
//   |     |     |                        |     |     |
//   |     |     |                        |     |     |
//   |     |     |         V (Check)      |     |     |
//   |     |     |                        |     |     |
//   |     |     |                        |     |     |
//   |     |     |                        |     |     |
//   |     |------------------------------+     |     |
//   |     |                L2                  |     |
//   +------------------------------------------+     |
//   |                      L1                        |
//   +------------------------------------------------+
void CNanoLineSelect::DrawCheckBoxState(CDC *pDC,RECT *pScreenSize)
{
	CBrush *pOldBrush;
	CPen* pOldPen;

	int	nCheckState;

	int nScreenHeight;
	int nScreenVCenter;

	RECT pCheckBox;

	RECT pBackArea;

	nCheckState = this->GetCheck();

	//描画エリアの垂直中心座標を算出
	nScreenHeight = pScreenSize->bottom - pScreenSize->top;
	nScreenVCenter = (int)(((nScreenHeight) / 2) + pScreenSize->top);

	pCheckBox.top = nScreenVCenter - (int)(CHECK_BOX_WIDTH / 2);
	pCheckBox.bottom = nScreenVCenter + (int)(CHECK_BOX_WIDTH / 2);
	pCheckBox.left = pScreenSize->left + CHECK_BOX_OFFSET;
	pCheckBox.right = pScreenSize->left + CHECK_BOX_OFFSET + CHECK_BOX_WIDTH;


	CPen cBoxPenU1(PS_SOLID,1,RGB(0x80,0x80,0x80));		//外周：上辺・左辺
	CPen cBoxPenU2(PS_SOLID,1,RGB(0x00,0x00,0x00));		//内周：上辺・左辺
	CPen cBoxPenL1(PS_SOLID,1,RGB(0xFF,0xFF,0xFF));		//外周：下辺・右辺
	CPen cBoxPenL2(PS_SOLID,1,RGB(0xC0,0xC0,0xC0));		//内周：下辺・右辺
	CBrush cBackArea(RGB(0xFF,0xFF,0xFF));				//背景
	CPen cCheckPen(PS_SOLID,2,RGB(0x00,0x00,0x00));		//チェック

	//外周：上左
	pOldPen = pDC->SelectObject(&cBoxPenU1);
	pDC->MoveTo(pCheckBox.left,pCheckBox.top);
	pDC->LineTo(pCheckBox.right-1,pCheckBox.top);
	pDC->MoveTo(pCheckBox.left,pCheckBox.top);
	pDC->LineTo(pCheckBox.left,pCheckBox.bottom-1);
	pDC->SelectObject(pOldPen);

	//内周：上左
	pOldPen = pDC->SelectObject(&cBoxPenU2);
	pDC->MoveTo(pCheckBox.left+1,pCheckBox.top+1);
	pDC->LineTo(pCheckBox.right-2,pCheckBox.top+1);
	pDC->MoveTo(pCheckBox.left+1,pCheckBox.top+1);
	pDC->LineTo(pCheckBox.left+1,pCheckBox.bottom-2);
	pDC->SelectObject(pOldPen);

	//外周：下右
	pOldPen = pDC->SelectObject(&cBoxPenL1);
	pDC->MoveTo(pCheckBox.left,pCheckBox.bottom-1);
	pDC->LineTo(pCheckBox.right,pCheckBox.bottom-1);
	pDC->MoveTo(pCheckBox.right-1,pCheckBox.top);
	pDC->LineTo(pCheckBox.right-1,pCheckBox.bottom);
	pDC->SelectObject(pOldPen);

	//内周：下右
	pOldPen = pDC->SelectObject(&cBoxPenL2);
	pDC->MoveTo(pCheckBox.left+1,pCheckBox.bottom-2);
	pDC->LineTo(pCheckBox.right-2,pCheckBox.bottom-2);
	pDC->MoveTo(pCheckBox.right-2,pCheckBox.top);
	pDC->LineTo(pCheckBox.right-2,pCheckBox.bottom-1);
	pDC->SelectObject(pOldPen);

	//背景
	pBackArea.top = pCheckBox.top + 2;
	pBackArea.bottom = pCheckBox.bottom - 2;
	pBackArea.left = pCheckBox.left + 2;
	pBackArea.right = pCheckBox.right - 2;

	pOldBrush = pDC->SelectObject(&cBackArea);
	pDC->FillRect(&pBackArea,&cBackArea);
	pDC->SelectObject(pOldBrush);

	//チェック
	if (nCheckState) {
		pOldPen = pDC->SelectObject(&cCheckPen);
		pDC->MoveTo(pCheckBox.left+2,pCheckBox.top+5);
		pDC->LineTo(pCheckBox.left+5,pCheckBox.top+8);
		pDC->LineTo(pCheckBox.left+9,pCheckBox.top+3);
		pDC->SelectObject(pOldPen);
	}
}

//--------------------------------------------------
//		ペンガイド描画
//--------------------------------------------------
void CNanoLineSelect::DrawPenGuidence(CDC *pDC,RECT *pScreenSize)
{
	CPen* pOldPen;

	int nScreenHeight;
	int nScreenVCenter;
	int nLineLeft;

	nScreenHeight = pScreenSize->bottom - pScreenSize->top;
	nScreenVCenter = (int)(((nScreenHeight) / 2) + pScreenSize->top);

	nLineLeft = pScreenSize->left + CHECK_BOX_OFFSET + CHECK_BOX_WIDTH + CHECK_BOX_OFFSET;

	CPen cGuidencePen(m_PenStyle,1,m_LineColor);

	pOldPen = pDC->SelectObject(&cGuidencePen);

	pDC->MoveTo(nLineLeft,nScreenVCenter);
	pDC->LineTo(nLineLeft+GUIDE_LINE_LENGTH,nScreenVCenter);

	pDC->SelectObject(pOldPen);
}

//--------------------------------------------------
//		テキスト描画
//--------------------------------------------------
void CNanoLineSelect::DrawLineText(CDC *pDC,RECT *pScreenSize)
{
	CString strText;
	COLORREF crOldColor;
	int nOldBKMode;

	RECT pTextArea;

	GetWindowText(strText);

	pTextArea.top = pScreenSize->top;
	pTextArea.bottom = pScreenSize->bottom;
	pTextArea.left = pScreenSize->left + CHECK_BOX_OFFSET + CHECK_BOX_WIDTH + CHECK_BOX_OFFSET + GUIDE_LINE_LENGTH + CHECK_BOX_OFFSET;
	pTextArea.right = pScreenSize->right;

	nOldBKMode = pDC->SetBkMode(TRANSPARENT);
	crOldColor = pDC->SetBkColor(m_BackColor);
	pDC->DrawText(strText,&pTextArea,DT_VCENTER | DT_SINGLELINE);
	pDC->SetBkColor(crOldColor);
	pDC->SetBkMode(nOldBKMode);
}

int CNanoLineSelect::GetCheck()
{
	return m_Checked;
}

void CNanoLineSelect::SetCheck(int nChecked)
{
	m_Checked = nChecked;
}
void CNanoLineSelect::SetDisplaySetting(COLORREF BackColor,COLORREF LineColor,int PenStyle)
{
	m_BackColor = BackColor;
	m_LineColor = LineColor;
	m_PenStyle = PenStyle;
}

SIZE CNanoLineSelect::GetIdealDispSize()
{
	SIZE tSize;
	CString strName;
	CDC *pDC;
	CFont *pFont;

	pFont = this->GetFont ();
	this->GetWindowText(strName);
	pDC = this->GetDC();
	tSize = GetTextSize(pDC,pFont,strName);
	this->ReleaseDC(pDC);
	tSize.cx += (CHECK_BOX_OFFSET + CHECK_BOX_WIDTH + CHECK_BOX_OFFSET + GUIDE_LINE_LENGTH + CHECK_BOX_OFFSET + CHECK_BOX_OFFSET) + 10;
	return tSize;
}

SIZE CNanoLineSelect::GetTextSize(CDC *pDC, CFont *pFont, CString strText)
{
	SIZE tRet;
	CFont *pOldFont;

	pOldFont = pDC->SelectObject(pFont);
	GetTextExtentPoint(pDC->GetSafeHdc(),strText,strText.GetLength(),&tRet);
	pDC->SelectObject(pOldFont);

	return tRet;
}
