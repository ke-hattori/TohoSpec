// NanoMeasureGraph.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <math.h>
#include "System.h"
#include "NanoMeasureGraph.h"
//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
#include <float.h>
//2009.09.03 bagus se �I�[�g�X�P�[�� --}--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNanoMeasureGraph

CNanoMeasureGraph::CNanoMeasureGraph()
{

	//Initialize
	memset(&m_Config,NULL,sizeof(m_Config));
	memset(m_LineConfig,NULL,sizeof(m_LineConfig));
	memset(m_pLineSelect,NULL,sizeof(m_pLineSelect));
	m_bbDC = NULL;
	m_bbBitmap = NULL;
	m_bbOK = FALSE;

	//Fonts
	m_pTitleFont = new CFont();
	m_pTitleFont->CreateFont(12,0,	0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_SCRIPT,"MS UI Gothic");
	m_pAxisXFont = new CFont();
	m_pAxisXFont->CreateFont(12,0,	0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_SCRIPT,"MS UI Gothic");
	m_pAxisYFont = new CFont();
	m_pAxisYFont->CreateFont(12,5,900,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_SCRIPT,"MS UI Gothic");
	m_pMemFont = new CFont();
	m_pMemFont->CreateFont(10,0,  0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_SCRIPT,"MS UI Gothic");

	// Default Choose Normal Graph
	m_iGrapgSelectFlg=0;

//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
	m_bAutoScaleMode = FALSE;
	m_dYMin = DBL_MAX;
	m_dYMax = DBL_MIN;
//2009.09.03 bagus se �I�[�g�X�P�[�� --}--
}

CNanoMeasureGraph::~CNanoMeasureGraph()
{
	delete m_pTitleFont;
	delete m_pAxisXFont;
	delete m_pAxisYFont;
	delete m_pMemFont;

	for(int i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
		if(m_pLineSelect[i] != NULL)
			delete m_pLineSelect[i];
	}

	if(m_bbDC){
		delete m_bbDC;
	}
	if(m_bbBitmap){
		delete m_bbBitmap;
	}
}


BEGIN_MESSAGE_MAP(CNanoMeasureGraph, CButton)
	//{{AFX_MSG_MAP(CNanoMeasureGraph)
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_CHAR()
	ON_WM_SYSKEYUP()
	ON_WM_KEYDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_LINE_SELECT_CHANGE,OnLineSelectChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoMeasureGraph ���b�Z�[�W �n���h��

//------------------------------------------------------------------------
// DrawItem
//	Draw graph.
//  Screen
//
//   +------------------------------------------------------------------+
//   |                        UpperTitle                                |
//   +------------------------------------------------------------------+
//   |                        UpperTitle                                |
//   +-----+------------------------------------------------------------+
//   |  Y  |                                                            |
//   | Axis|                                                            |
//   | Tit |                                                            |
//   |     |                Graph Body                                  |
//   |     |                                                            |
//   |     |                                                            |
//   |     |                                                            |
//   +-----+------------------------------------------------------------+
//   |     |                   LowerTitle(X-Axis)                       |
//   +------------------------------------------------------------------+
//   |                         LowerTitle                               |
//   +------------------------------------------------------------------+
//------------------------------------------------------------------------
void CNanoMeasureGraph::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect cButtonRect;
	CRect cClipArea;
	int	  nRet;
	if(!m_bbDC || !m_bbBitmap){
		InitBackBuffer();
	}

	cButtonRect.CopyRect(&lpDIS->rcItem);
	DrawItemMain(lpDIS);

	if(m_bbOK){
		nRet = pDC->GetClipBox(cClipArea);
		if(nRet == ERROR || nRet == NULLREGION){
			return;
		}
		pDC->BitBlt(cClipArea.left,cClipArea.top,cClipArea.Width(),cClipArea.Height(),m_bbDC,cClipArea.left,cClipArea.top,SRCCOPY);
		if(m_bReDrawGraph){
			m_bReDrawGraph = FALSE;

			pDC->BitBlt(m_GraphInfo.GraphArea.left,m_GraphInfo.GraphArea.top,m_GraphInfo.GraphArea.right - m_GraphInfo.GraphArea.left,m_GraphInfo.GraphArea.bottom - m_GraphInfo.GraphArea.top,m_bbDC,m_GraphInfo.GraphArea.left,m_GraphInfo.GraphArea.top,SRCCOPY);
		}
	}else{
		pDC->BitBlt(cButtonRect.left,cButtonRect.top,cButtonRect.Width(),cButtonRect.Height(),m_bbDC,cButtonRect.left,cButtonRect.top,SRCCOPY);
		m_bbOK = TRUE;
	}
}
void CNanoMeasureGraph::DrawItemMain(LPDRAWITEMSTRUCT lpDIS)
{
//	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CDC* pDC = m_bbDC;

	UINT	uState = lpDIS->itemState;
	CRect	cFocusRect;
	CRect	cButtonRect;
	CRect	cTmpRect;
	SIZE	tFontSizeX;
	SIZE	tFontSizeY;
	SIZE	tFontSizeUpperTitle;
	SIZE	tFontSizeLowerTitle;
	SIZE	tSize;
	CString strTmp;
	int		nSelectorHeight;
	int		nPadding = 2;

	cFocusRect.CopyRect(&lpDIS->rcItem);
	cButtonRect.CopyRect(&lpDIS->rcItem);

	//Clear Canvas
	ClearGraph(pDC,&cButtonRect);

	if(m_Config.CanvasMargin > 0){
		cButtonRect.left += m_Config.CanvasMargin;
		cButtonRect.top += m_Config.CanvasMargin;
		cButtonRect.right -= m_Config.CanvasMargin;
		cButtonRect.bottom -= m_Config.CanvasMargin;
	}


	//�e�L�X�g�`�掞�̃T�C�Y���v�Z
	tFontSizeX = GetTextSize(pDC,m_pAxisXFont,m_Config.X_AxisTitle);
	tFontSizeY = GetTextSize(pDC,m_pAxisYFont,m_Config.Y_AxisTitle);
	tFontSizeUpperTitle = GetTextSize(pDC,m_pTitleFont,m_Config.UpperTitle);
	tFontSizeLowerTitle = GetTextSize(pDC,m_pTitleFont,m_Config.LowerTitle);

	cTmpRect = cButtonRect;

	//Mem Size
	strTmp.Format("%d ",m_Config.Y_Max);
	tSize = GetTextSize(pDC, m_pMemFont, strTmp);

	cTmpRect.top = cTmpRect.top + tFontSizeUpperTitle.cy + nPadding;
	cTmpRect.left = cTmpRect.left + (tFontSizeY.cy + nPadding) + tSize.cx + tSize.cx / 2;
	nSelectorHeight = ArrangeLineSelect(cTmpRect);


	//Draw Graph Body
	cTmpRect = cButtonRect;
	//cTmpRect.left = cTmpRect.left + 20;
	cTmpRect.left = cTmpRect.left + (tFontSizeY.cy + nPadding);
	//cTmpRect.top = cTmpRect.top + 20 + nSelectorHeight ;
	cTmpRect.top = cTmpRect.top + (tFontSizeUpperTitle.cy + nPadding)+ nSelectorHeight ;
	//cTmpRect.bottom = cTmpRect.bottom - 40;
	cTmpRect.bottom = cTmpRect.bottom
					- (tFontSizeLowerTitle.cy + nPadding)
					- (tFontSizeX.cy + nPadding)
					;
	//2008.12.10
	// Clip drawing area to prohibit drawing outside of graph.
	CRgn rgnClip;
	rgnClip.CreateRectRgn(cTmpRect.left,cTmpRect.top,cTmpRect.right,cTmpRect.bottom);
	pDC->SelectClipRgn(&rgnClip);
	DrawGraph(pDC,cTmpRect);
	pDC->SelectClipRgn(NULL);
	rgnClip.DeleteObject();
	//<--

	//Cut
	CutGraphArea(pDC,&cButtonRect,cTmpRect);


	//Draw Y-Axis Title
	cTmpRect = cButtonRect;
	//cTmpRect.right = cTmpRect.left + 15;
	cTmpRect.right = cTmpRect.left + tFontSizeY.cy + nPadding;
	cTmpRect.top = cTmpRect.top + (tFontSizeUpperTitle.cy + nPadding)+ nSelectorHeight ;
	cTmpRect.bottom = cTmpRect.bottom
					- (tFontSizeLowerTitle.cy + nPadding)
					- (tFontSizeX.cy + nPadding)
					;
	DrawAxisTitleY(pDC,m_Config.Y_AxisTitle,&cTmpRect);
	//Draw X-Axis Title
	cTmpRect = cButtonRect;
	//cTmpRect.left = cTmpRect.left + 15;
	cTmpRect.left = cTmpRect.left + tFontSizeY.cy + nPadding;
	//cTmpRect.top = cTmpRect.bottom - 40;
	cTmpRect.top	= cTmpRect.bottom
					- (tFontSizeLowerTitle.cy + nPadding)
					- (tFontSizeX.cy + nPadding);
	cTmpRect.bottom = cTmpRect.top + tFontSizeX.cy + nPadding;
	DrawAxisTitleX(pDC,m_Config.X_AxisTitle,&cTmpRect);
	//Draw Graph Title (Upper)
	cTmpRect = cButtonRect;
	//cTmpRect.left = cTmpRect.left + 15;
	cTmpRect.left = cTmpRect.left + tFontSizeY.cy + nPadding;
	//cTmpRect.top = 0; //cTmpRect.top; //+ nSelectorHeight ;
	//cTmpRect.bottom = cTmpRect.top + 20;
	cTmpRect.bottom = cTmpRect.top + (tFontSizeUpperTitle.cy + nPadding);
	DrawTitle(pDC,m_Config.UpperTitle,cTmpRect);
	//Draw Graph Title (Lower)
	cTmpRect = cButtonRect;
	cTmpRect.left = cTmpRect.left + tFontSizeY.cy + nPadding;
	cTmpRect.top = cTmpRect.bottom - (tFontSizeLowerTitle.cy + nPadding);
	cTmpRect.bottom = cTmpRect.top + (tFontSizeLowerTitle.cy + nPadding);
	DrawTitle(pDC,m_Config.LowerTitle,cTmpRect);
}
//------------------------------------------------------------------------
// OnKeyUp
//------------------------------------------------------------------------
void CNanoMeasureGraph::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(!m_Config.UseCalibration)
		return;
	switch(nChar){
	case VK_RETURN:
		if(m_bNowCalibration){
			m_bNowCalibration = FALSE;
			m_CalibrationData.DataNum++;
			GetParent()->PostMessage(WM_NANO_MEASURE_GRAPH_CALIBRATION_SET,m_CalibrationData.DataNum,0);
			if(m_CalibrationData.DataNum >= m_Config.CalibrationLineNum){
				//AfxMessageBox("Calibration DEBUG");
				GetParent()->PostMessage(WM_NANO_MEASURE_GRAPH_CALIBRATION_COMP,0,0);
			}
			Invalidate(FALSE);
			m_bReDrawGraph = TRUE;
		}
		break;
	default:
		break;
	}
	CButton::OnKeyUp(nChar, nRepCnt, nFlags);
}

//------------------------------------------------------------------------
// OnLButtonUp
//------------------------------------------------------------------------
void CNanoMeasureGraph::OnLButtonUp(UINT nFlags, CPoint point)
{
	long lDataX;			//�����l
	int i;
	int nPointIndex;


	//�L�����u���[�V�������Ȃ��ꍇ�͉������Ȃ�
	if(!m_Config.UseCalibration){
		CButton::OnLButtonUp(nFlags, point);
		return;
	}
	//����ȏ���͒u���Ȃ��B
	if(!m_bNowCalibration
	&& m_CalibrationData.DataNum >= m_Config.CalibrationLineNum){
		CButton::OnLButtonUp(nFlags, point);
		return;
	}

	//�ŋߖT�̃f�[�^�ɐ���������
	//	X�����̕`��G���A�O����
	if (point.x < m_GraphInfo.GraphArea.left) {
		CButton::OnLButtonUp(nFlags, point);
		return;
	} else if (point.x > m_GraphInfo.GraphArea.right) {
		CButton::OnLButtonUp(nFlags, point);
		return;
	}

	//	Y�����̕`��G���A�O����
	if (point.y < m_GraphInfo.GraphArea.top) {
		CButton::OnLButtonUp(nFlags, point);
		return;
	} else if (point.y > m_GraphInfo.GraphArea.bottom) {
		CButton::OnLButtonUp(nFlags, point);
		return;
	}

	//	�����l�̋t�Z
	if (m_GraphInfo.RateX == 0) {
		lDataX = 0;
	} else {
		lDataX = (long)((double)(point.x - m_GraphInfo.GraphArea.left));// / m_GraphInfo.RateX);
	}

	//�ߎ��l�̎Z�o
	for (i=0; i<m_Config.PointNum; i++) {
		if (((i * m_GraphInfo.DivX) <= lDataX) && (lDataX < (i * m_GraphInfo.DivX + m_GraphInfo.DivX / 2.0))) {
			//lDataX = (long)(i * m_GraphInfo.DivX);
			nPointIndex = i;
			break;
		}
		if (((i * m_GraphInfo.DivX + m_GraphInfo.DivX / 2.0) <= lDataX) && (lDataX < ((i+1) * m_GraphInfo.DivX ))) {
			//lDataX = (long)(i * m_GraphInfo.DivX);
			nPointIndex = i+1;
			break;
		}
	}

	if(m_bNowCalibration){
		ChangeCalibrationData(nPointIndex);
	}else{
		AddCalibrationData(nPointIndex);
		m_bNowCalibration = TRUE;
	}
	CButton::OnLButtonUp(nFlags, point);

}

//------------------------------------------------------------------------
// OnSize
//------------------------------------------------------------------------
void CNanoMeasureGraph::OnSize(UINT nType, int cx, int cy)
{
	CRect cArea;
	CButton::OnSize(nType, cx, cy);

	GetClientRect(cArea);
	//ArrangeLineSelect(cArea);
	InitBackBuffer();
	Invalidate(TRUE);
}

void CNanoMeasureGraph::ClearGraph(CDC *pDC,RECT *pScreenSize)
{
	CBrush cCanvasColor(m_Config.CanvasBackColor);
	CBrush *pOldBrush;

	pOldBrush = pDC->SelectObject(&cCanvasColor);
	pDC->FillRect(pScreenSize,&cCanvasColor);

	pDC->SelectObject(pOldBrush);
}

void CNanoMeasureGraph::DrawAxisTitleY(CDC *pDC, CString strTitle, RECT *pArea)
{
	CBrush cAxisTitleColor(m_Config.TitleTextColor);
	CFont *pOldFont;

	CRect	cTempRect;
	int nOldBkMode;

	cTempRect = pArea;

	SIZE tTitle;

	tTitle = GetTextSize(pDC, m_pAxisYFont, strTitle);

	DrawEdge(pDC,pArea);
	pOldFont = pDC->SelectObject(m_pAxisYFont);
	pDC->SetTextColor(m_Config.TitleTextColor);
	nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor (m_Config.CanvasBackColor);

	cTempRect.bottom += tTitle.cx;

	pDC->DrawText(strTitle,cTempRect, DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nOldBkMode);
}

void CNanoMeasureGraph::DrawAxisTitleX(CDC *pDC, CString strTitle, RECT *pArea)
{
	CBrush cAxisTitleColor(m_Config.TitleTextColor);
	CFont *pOldFont;
	int nOldBkMode;

	DrawEdge(pDC,pArea);
	pOldFont = pDC->SelectObject(m_pAxisXFont);
	pDC->SetTextColor(m_Config.TitleTextColor);
	nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor (m_Config.CanvasBackColor);
	pDC->DrawText(strTitle,pArea,DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(nOldBkMode);
}

void CNanoMeasureGraph::DrawEdge(CDC *pDC, RECT *pArea)
{
	return;

	CBrush cEdgeColor(RGB(0xFF,0x00,0x00));
	CBrush *pOldBrush;

	pOldBrush = pDC->SelectObject(&cEdgeColor);

	pDC->Rectangle(pArea);
	pDC->SelectObject(pOldBrush);
}

void CNanoMeasureGraph::DrawTitle(CDC *pDC, CString strTitle, RECT *pArea)
{
	CFont *pOldFont;

	DrawEdge(pDC,pArea);
	pOldFont = pDC->SelectObject(m_pTitleFont);
	pDC->SetTextColor(m_Config.TitleTextColor);
	pDC->SetBkColor (m_Config.CanvasBackColor);
	pDC->DrawText(strTitle,pArea,DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
}

void CNanoMeasureGraph::DrawGraph(CDC *pDC, RECT *pArea)
{
//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
	double	dScaleYMin, dScaleYMax;
	double	dScaleYStep;

	if (m_bAutoScaleMode) {
		//2010.01.19 bagus --{--
		dScaleYStep = m_Config.Y_Step;
		//2010.01.19 bagus --}--
		//2009.09.11 bagus stress --{--
		//AutoScale���ɂ�Step��1/2�ɂ���
		if((m_Config.Y_Max - m_Config.Y_Origin) > (m_dYMax - m_dYMin)){
			//�A���S�̂�Y���͈̔͂��������Ȃ����Ƃ��̂�
			dScaleYStep = m_Config.Y_Step;
			m_Config.Y_Step = m_Config.Y_Step / 2;
			if(m_Config.Y_Step > 10){
				//10�ȏ�ł��ꂠ��1�̈ʂ�؂�̂��������ɕύX����
				if(m_Config.Y_Step % 10 >= 5){
					m_Config.Y_Step = (m_Config.Y_Step + 5) - (m_Config.Y_Step % 10);
				}else{
					m_Config.Y_Step = m_Config.Y_Step - (m_Config.Y_Step % 10);
				}
			}
		}
		//2010.01.19 bagus --{--
		else{
			m_Config.Y_Step = static_cast<long>((m_dYMax -m_dYMin) / 10);
			if(m_Config.Y_Step > 10){
				//10�ȏ�ł��ꂠ��1�̈ʂ�؂�̂��������ɕύX����
				if(m_Config.Y_Step % 10 >= 5){
					m_Config.Y_Step = (m_Config.Y_Step + 5) - (m_Config.Y_Step % 10);
				}else{
					m_Config.Y_Step = m_Config.Y_Step - (m_Config.Y_Step % 10);
				}
			}
		}
		//2010.01.19 bagus --}--

		//2009.09.11 bagus stress --}--
		dScaleYMin = m_Config.Y_Origin;
		dScaleYMax = m_Config.Y_Max;
		m_Config.Y_Max = static_cast<long>(m_dYMax);
		m_Config.Y_Origin = static_cast<long>(m_dYMin);
	}
//2009.09.03 bagus se �I�[�g�X�P�[�� --}--

	CBrush cGraphColor(m_Config.GraphBackColor);
	CBrush *pOldBrush;
	int i;
	int j;
	CPen* pOldPen;

	CFont *pOldFont;

	int nDataIndex;

	RECT cCutArea;

	RECT cMemArea;

	char szMem[256];
	CString strMem;

	SIZE tSize_X;
	SIZE tSize_Y;

	//Normal Graph Data
	int nTmp_X;
	int nTmp_Y;
	int nTmp_X_before;
	int nTmp_Y_before;

	//XY Graph Data
	double dTmp_X;
	double dTmp_Y;
	double dTmp_X_before;
	double dTmp_Y_before;

	int nDrawPoint_X_bottom;
	int nDrawPoint_X_top;

	double dblA;			// Y = AX + B
	double dblB;
	int nOldBkMode;

	double dStep;
	double dTmp;
	double dIndex;
	double dGraphStepOrigin_Y;
	double dGraphStepOrigin_X;
	double dGraphStep_Y;
	double dGraphStep_X;

	pOldBrush = pDC->SelectObject(&cGraphColor);
	pDC->FillRect(pArea,&cGraphColor);

	pOldFont = pDC->SelectObject(m_pMemFont);
	pDC->SetTextColor(m_Config.UnitTextColor);
	pDC->SetBkColor(m_Config.GraphBackColor);

	//
	if(m_Config.X_Step <= 0){
		m_Config.X_Step = 1;
	}
	if(m_Config.Y_Step <= 0){
		m_Config.Y_Step = 1;
	}
	//Mem Size
	wsprintf(szMem, "%d", m_Config.X_Max);
	strMem = szMem;
	tSize_X = GetTextSize(pDC, m_pMemFont, strMem);
	wsprintf(szMem, "%d ", m_Config.Y_Max);
	strMem = szMem;
	tSize_Y = GetTextSize(pDC, m_pMemFont, strMem);

	//Draw Area (Graph)
	m_GraphInfo.GraphArea.left = pArea->left + tSize_Y.cx + (int)(tSize_Y.cx / 2);
	m_GraphInfo.GraphArea.bottom = pArea->bottom - tSize_X.cy - 3;
	m_GraphInfo.GraphArea.right = pArea->right - (int)(tSize_Y.cx / 2);
	m_GraphInfo.GraphArea.top = pArea->top + tSize_X.cy;
	m_GraphInfo.RateX = (m_GraphInfo.GraphArea.right - m_GraphInfo.GraphArea.left) / (double)(m_Config.X_Max - m_Config.X_Origin);
	m_GraphInfo.RateY = (m_GraphInfo.GraphArea.bottom - m_GraphInfo.GraphArea.top) / (double)(m_Config.Y_Max - m_Config.Y_Origin);
	if (m_Config.PointNum > 1) {
		m_GraphInfo.DivX = (double)((m_GraphInfo.GraphArea.right - m_GraphInfo.GraphArea.left)) / (double)(m_Config.PointNum - 1);
	}

	//Step (X)
	dGraphStep_X = (m_GraphInfo.RateX * m_Config.X_Step);
	if (dGraphStep_X < 1.0) {
		dGraphStep_X = 0.0;
	}
	//Step (Y)
	dGraphStep_Y = (m_GraphInfo.RateY * m_Config.Y_Step);
	if (dGraphStep_Y < 1.0) {
		dGraphStep_Y = 0.0;
	}

	//DrawAxis
	////�⏕���iX���j
	CPen cLinePen(PS_SOLID, 1, RGB(0x00,0x00,0x00));
	CPen cFramePen(PS_SOLID, 1, m_Config.GraphBorderColor);
	CPen cInFramePen(PS_DOT, 1, m_Config.GraphStepLineColor);

	dStep = m_Config.X_Origin % m_Config.X_Step;
	dTmp = m_Config.X_Origin % m_Config.X_Step;
	dTmp = (double)m_Config.X_Origin - dTmp;
	dTmp /= (double)m_Config.X_Step;
	dTmp += 1;
	dTmp *= (double)m_Config.X_Step;
	dTmp -= (double)m_Config.X_Origin;

	dGraphStepOrigin_X = (m_GraphInfo.RateX * dTmp) + m_GraphInfo.GraphArea.left;
	pOldPen = pDC->SelectObject(&cInFramePen);
	j = 1;
	for (dIndex=dGraphStepOrigin_X; dIndex<=m_GraphInfo.GraphArea.right; dIndex+=dGraphStep_X) {
		if (dGraphStep_X < 1) {
			break;
		}

		pDC->MoveTo((int)dIndex, m_GraphInfo.GraphArea.top);
		pDC->LineTo((int)dIndex, m_GraphInfo.GraphArea.bottom);

		if((m_Config.X_Step * j) + (m_Config.X_Origin - dStep) > m_Config.X_Max)
			break;
		j++;
	}
	pDC->SelectObject(pOldPen);

	//�⏕���iY���j
	dGraphStep_Y = (m_GraphInfo.RateY * (double)m_Config.Y_Step);
	if (dGraphStep_Y < 1.0) {
		dGraphStep_Y = 0.0;
	}
	//Axis - X
	dTmp = m_Config.Y_Origin % m_Config.Y_Step;
	dTmp = m_Config.Y_Origin - dTmp;
	dTmp /= m_Config.Y_Step;
	dTmp += 1.0;
	dTmp *= m_Config.Y_Step;
	dTmp -= m_Config.Y_Origin;
	dGraphStepOrigin_Y = m_GraphInfo.GraphArea.bottom - (double)(m_GraphInfo.RateY * dTmp);
	pOldPen = pDC->SelectObject(&cInFramePen);
	j = 1;
	for (dIndex=dGraphStepOrigin_Y; dIndex>=m_GraphInfo.GraphArea.top; dIndex-=dGraphStep_Y) {
		if (dGraphStep_Y < 1) {
			break;
		}

		pDC->MoveTo(m_GraphInfo.GraphArea.left, (int)dIndex);
		pDC->LineTo(m_GraphInfo.GraphArea.right, (int)dIndex);

		j++;
	}
	pDC->SelectObject(pOldPen);


	//2008.08.27 �g�����e�����͈�ԍŌ�Ɉړ�����
	pOldPen = pDC->SelectObject(&cFramePen);
	//����
	pDC->MoveTo(m_GraphInfo.GraphArea.left, m_GraphInfo.GraphArea.top);
	pDC->LineTo(m_GraphInfo.GraphArea.left, m_GraphInfo.GraphArea.bottom);
	//�E��
	pDC->MoveTo(m_GraphInfo.GraphArea.right, m_GraphInfo.GraphArea.top);
	pDC->LineTo(m_GraphInfo.GraphArea.right, m_GraphInfo.GraphArea.bottom);

	//���
	pDC->MoveTo(m_GraphInfo.GraphArea.left, m_GraphInfo.GraphArea.bottom);
	pDC->LineTo(m_GraphInfo.GraphArea.right, m_GraphInfo.GraphArea.bottom);
	//����
	pDC->MoveTo(m_GraphInfo.GraphArea.left, m_GraphInfo.GraphArea.top);
	pDC->LineTo(m_GraphInfo.GraphArea.right, m_GraphInfo.GraphArea.top);
	pDC->SelectObject(pOldPen);


	//NormalGraph
	if (m_iGrapgSelectFlg==0){
		//DrawData
		for(i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
			if(m_LineConfig[i].UseFlag == FALSE
			|| m_LineConfig[i].Visible == FALSE
			|| m_LineConfig[i].DataIndex < 0
			|| m_LineConfig[i].DataIndex >= NANO_MEASURE_MAX_GRAPH_LINE ){
				//Not Use
				continue;
			}
			nDataIndex = m_LineConfig[i].DataIndex;
			CPen cGraphPen(m_LineConfig[i].PenStyle,1,m_LineConfig[i].Color);
			pOldPen = pDC->SelectObject(&cGraphPen);

			//Move to origin
			//2009.09.11 bagus --{--
			//���_��0�ȊO�̍��W�̎��̑Ή�
			//pDC->MoveTo(0 + m_GraphInfo.GraphArea.left, m_GraphInfo.GraphArea.bottom - (int)(m_LineData[nDataIndex][0] * m_GraphInfo.RateY));
			pDC->MoveTo(0 + m_GraphInfo.GraphArea.left, m_GraphInfo.GraphArea.bottom - (int)((m_LineData[nDataIndex][0] - m_Config.Y_Origin)* m_GraphInfo.RateY));
			//2009.09.11 bagus --}--
			for(j = 0;(j < m_Config.PointNum) && (m_Config.PointNum > 1);j++){
				nTmp_X = (int)((double)j * m_GraphInfo.DivX) + m_GraphInfo.GraphArea.left;
				//2009.09.11 bagus --{--
				//���_��0�ȊO�̍��W�̎��̑Ή�
				//nTmp_Y = m_GraphInfo.GraphArea.bottom - (int)(m_LineData[nDataIndex][j] * m_GraphInfo.RateY);
				nTmp_Y = m_GraphInfo.GraphArea.bottom - (int)((m_LineData[nDataIndex][j] - m_Config.Y_Origin) * m_GraphInfo.RateY);
				//2009.09.11 bagus --}--

				pDC->LineTo(nTmp_X, nTmp_Y);

				//�ŏ��̒l (X���_)
				if (j == 0) {
					nTmp_X_before = nTmp_X;
					nTmp_Y_before = nTmp_Y;
					pDC->MoveTo(nTmp_X, nTmp_Y);

					continue;
				}

				//�X�����Ȃ�
				if ((nTmp_X_before - nTmp_X) == 0) {
					nTmp_X_before = nTmp_X;
					nTmp_Y_before = nTmp_Y;
					pDC->MoveTo(nTmp_X, nTmp_Y);

					continue;
				}

				//�X�� => ��_�Z�o
				dblA = (double)(nTmp_Y_before - nTmp_Y) / (double)(nTmp_X_before - nTmp_X);
				dblB = (double)(nTmp_Y - (double)(dblA * nTmp_X));
				nDrawPoint_X_bottom = (int)((m_GraphInfo.GraphArea.bottom - dblB) / dblA);
				nDrawPoint_X_top = (int)((m_GraphInfo.GraphArea.top - dblB) / dblA);

				//�`��
				//	--------L	(�����)
				//	--------U	(������)
				//		*		(�����F�O�̓_�b�E���F���̓_)
				if ((nTmp_Y_before > m_GraphInfo.GraphArea.bottom) && (nTmp_Y < m_GraphInfo.GraphArea.top)) {
					//		*
					//-----/---L
					//	  /
					//---/-----U
					//	*
					//�����I�[�o�[ => ����I�[�o�[
					pDC->MoveTo(nTmp_X_before, m_GraphInfo.GraphArea.bottom);
					pDC->LineTo(nDrawPoint_X_bottom, m_GraphInfo.GraphArea.bottom);
					pDC->MoveTo(nDrawPoint_X_top, m_GraphInfo.GraphArea.top);
					pDC->LineTo(nTmp_X, m_GraphInfo.GraphArea.top);
				} else if ((nTmp_Y_before < m_GraphInfo.GraphArea.top) && (nTmp_Y > m_GraphInfo.GraphArea.bottom)) {
					//	*
					//---\-----L
					//	  \
					//-----\---U
					//		*
					//����I�[�o�[ => �����I�[�o�[
					pDC->MoveTo(nTmp_X_before, m_GraphInfo.GraphArea.top);
					pDC->LineTo(nDrawPoint_X_top, m_GraphInfo.GraphArea.top);
					pDC->MoveTo(nDrawPoint_X_bottom, m_GraphInfo.GraphArea.bottom);
 					pDC->LineTo(nTmp_X, m_GraphInfo.GraphArea.bottom);
				} else if ((nTmp_Y_before > m_GraphInfo.GraphArea.bottom) && (nTmp_Y > m_GraphInfo.GraphArea.bottom)) {
					//
					//-------L
					//	*-*
					//�����I�[�o�[ => �����I�[�o�[
					pDC->MoveTo(nTmp_X_before, m_GraphInfo.GraphArea.bottom);
					pDC->LineTo(nTmp_X, m_GraphInfo.GraphArea.bottom);
				} else if ((nTmp_Y_before > m_GraphInfo.GraphArea.bottom) && (nTmp_Y <= m_GraphInfo.GraphArea.bottom)) {
					//	  *
					//---/---L
					//	*
					//�����I�[�o�[ => �O���t�̈��
					pDC->MoveTo(nTmp_X_before, m_GraphInfo.GraphArea.bottom);
					pDC->LineTo(nDrawPoint_X_bottom, m_GraphInfo.GraphArea.bottom);
				} else if ((nTmp_Y_before <= m_GraphInfo.GraphArea.bottom) && (nTmp_Y > m_GraphInfo.GraphArea.bottom)) {
					//	*
					//---\---L
					//	  *
					//�O���t�̈�� => ����I�[�o�[
					pDC->MoveTo(nDrawPoint_X_bottom, m_GraphInfo.GraphArea.bottom);
 					pDC->LineTo(nTmp_X, m_GraphInfo.GraphArea.bottom);
				} else if ((nTmp_Y_before < m_GraphInfo.GraphArea.top) && (nTmp_Y < m_GraphInfo.GraphArea.top)) {
					//	*-*
					//-------U
					//
					//����I�[�o�[ => ����I�[�o�[
					pDC->MoveTo(nTmp_X_before, m_GraphInfo.GraphArea.top);
					pDC->LineTo(nTmp_X, m_GraphInfo.GraphArea.top);
				} else if ((nTmp_Y_before >= m_GraphInfo.GraphArea.top) && (nTmp_Y < m_GraphInfo.GraphArea.top)) {
					//	  *
					//---/---U
					//	*
					//�O���t�̈�� => ����I�[�o�[
					pDC->MoveTo(nDrawPoint_X_top, m_GraphInfo.GraphArea.top);
					pDC->LineTo(nTmp_X, m_GraphInfo.GraphArea.top);
				} else if ((nTmp_Y_before < m_GraphInfo.GraphArea.top) && (nTmp_Y >= m_GraphInfo.GraphArea.top)) {
					//	*
					//---\---U
					//	  *
					//����I�[�o�[ => �O���t�̈��
					pDC->MoveTo(nTmp_X_before, m_GraphInfo.GraphArea.top);
					pDC->LineTo(nDrawPoint_X_top, m_GraphInfo.GraphArea.top);
				}


				//�l�̋L��
				nTmp_X_before = nTmp_X;
				nTmp_Y_before = nTmp_Y;
				pDC->MoveTo(nTmp_X, nTmp_Y);
			}
			pDC->SelectObject(pOldPen);
		}
	}

	else if (m_iGrapgSelectFlg==1){
		//DrawData
		for(i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
			if(m_LineConfig[i].UseFlag == FALSE
			|| m_LineConfig[i].Visible == FALSE
			|| m_LineConfig[i].DataIndex < 0
			|| m_LineConfig[i].DataIndex >= NANO_MEASURE_MAX_GRAPH_LINE ){
				//Not Use
				continue;
			}
			nDataIndex = m_LineConfig[i].DataIndex;
			CPen cGraphPen(m_LineConfig[i].PenStyle,1,m_LineConfig[i].Color);
			pOldPen = pDC->SelectObject(&cGraphPen);

			//2009.09.20 bagus se --{--
			int nPointNum = m_LinePointNum[nDataIndex];
			//2009.09.20 bagus se --}--
			//Move to origin
			//��_(0,0)
//2009.09.01 bagus stress --{--

			//2009.09.20 bagus se --{--
			//if(m_Config.PointNum <= 1){
			if(nPointNum <= 1){
			//2009.09.20 bagus se --}--
				pDC->MoveTo((m_GraphInfo.GraphArea.left + 0 * m_GraphInfo.RateX), (m_GraphInfo.GraphArea.bottom - 0 * m_GraphInfo.RateY));
			}else{
				pDC->MoveTo((m_GraphInfo.GraphArea.left + (m_LineXYData[nDataIndex][0].dXValue - m_Config.X_Origin) * m_GraphInfo.RateX), (m_GraphInfo.GraphArea.bottom - (m_LineXYData[nDataIndex][0].dYValue - m_Config.Y_Origin) * m_GraphInfo.RateY));
			}
//2009.09.01 bagus stress --}--
			//2009.09.20 bagus se --{--
			//for(j = 0;(j <= m_Config.PointNum) && (m_Config.PointNum >= 1);j++){
			for(j = 0;(j <= nPointNum) && (nPointNum >= 1);j++){
			//2009.09.20 bagus se --}--
				//�I�_(X,Y)�ǂ��炩�̍ő�\���L���l
				//2009.09.20 bagus se --{--
//				if (j == m_Config.PointNum) {
				if (j == nPointNum) {
				//2009.09.20 bagus se --}--
					if (j == 1) {
						//�X��
						dblA = (double)(m_GraphInfo.GraphArea.bottom-dTmp_Y) / (double)(m_GraphInfo.GraphArea.left-dTmp_X);
						//�ؕ�
						dblB = (double)(dTmp_Y - (double)(dblA * dTmp_X));
					}
					//�X��,�ؕ� => ��_�Z�o
					//�w���E�[���W
					dTmp_X = m_GraphInfo.GraphArea.right;
/* added 2015.12.06 hmenjo �O���t�o�O�C�� ---------- { ---------- */
					if (2 <= nPointNum) {
						if (m_LineXYData[nDataIndex][nPointNum - 2].dXValue > m_LineXYData[nDataIndex][nPointNum - 1].dXValue) {
							dTmp_X = m_GraphInfo.GraphArea.left;
						}
					}
/* added 2015.12.06 hmenjo �O���t�o�O�C�� ---------- } ---------- */
					dTmp_Y = dblA * dTmp_X + dblB;
					//X ���E�[���W�ɂ����āA�x����[���W���I�[�o�[���Ă���ꍇ�́A�x����[����W�����߂�
					if (dTmp_Y<m_GraphInfo.GraphArea.top){
						dTmp_Y = m_GraphInfo.GraphArea.top;
						dTmp_X = (dTmp_Y - dblB) / dblA;
					}
				}
				else{
//2009.09.01 bagus stress --{--
//					dTmp_X = m_GraphInfo.GraphArea.left + (double)(m_LineXYData[nDataIndex][j].dXValue	* m_GraphInfo.RateX);
//					dTmp_Y = m_GraphInfo.GraphArea.bottom - (double)(m_LineXYData[nDataIndex][j].dYValue * m_GraphInfo.RateY);
					dTmp_X = m_GraphInfo.GraphArea.left + (double)((m_LineXYData[nDataIndex][j].dXValue - m_Config.X_Origin)  * m_GraphInfo.RateX);
					dTmp_Y = m_GraphInfo.GraphArea.bottom - (double)((m_LineXYData[nDataIndex][j].dYValue - m_Config.Y_Origin) * m_GraphInfo.RateY);
//2009.09.01 bagus stress --}--


				}

				pDC->LineTo(dTmp_X, dTmp_Y);

				//�ݒ�1�_�ڂ̒l
				if (j == 0) {
					dTmp_X_before = dTmp_X;
					dTmp_Y_before = dTmp_Y;
					pDC->MoveTo(dTmp_X, dTmp_Y);

					continue;
				}

				//�X�����Ȃ�
				if ((dTmp_X_before - dTmp_X) == 0) {
					dTmp_X_before = dTmp_X;
					dTmp_Y_before = dTmp_Y;
					pDC->MoveTo(dTmp_X, dTmp_Y);
					continue;
				}

				//�X�� => ��_�Z�o
				dblA = (double)(dTmp_Y_before - dTmp_Y) / (double)(dTmp_X_before - dTmp_X);
				dblB = (double)(dTmp_Y - (double)(dblA * dTmp_X));
				nDrawPoint_X_bottom = (int)((m_GraphInfo.GraphArea.bottom - dblB) / dblA);
				nDrawPoint_X_top = (int)((m_GraphInfo.GraphArea.top - dblB) / dblA);

				//�`��
				//	--------L	(�����)
				//	--------U	(������)
				//		*		(�����F�O�̓_�b�E���F���̓_)
				if ((dTmp_Y_before > m_GraphInfo.GraphArea.bottom) && (dTmp_Y < m_GraphInfo.GraphArea.top)) {
					//		*
					//-----/---L
					//	  /
					//---/-----U
					//	*
					//�����I�[�o�[ => ����I�[�o�[
					pDC->MoveTo(dTmp_X_before, m_GraphInfo.GraphArea.bottom);
					pDC->LineTo(nDrawPoint_X_bottom, m_GraphInfo.GraphArea.bottom);
					pDC->MoveTo(nDrawPoint_X_top, m_GraphInfo.GraphArea.top);
					pDC->LineTo(dTmp_X, m_GraphInfo.GraphArea.top);
				} else if ((dTmp_Y_before < m_GraphInfo.GraphArea.top) && (dTmp_Y > m_GraphInfo.GraphArea.bottom)) {
					//	*
					//---\-----L
					//	  \
					//-----\---U
					//		*
					//����I�[�o�[ => �����I�[�o�[
					pDC->MoveTo(dTmp_X_before, m_GraphInfo.GraphArea.top);
					pDC->LineTo(nDrawPoint_X_top, m_GraphInfo.GraphArea.top);
					pDC->MoveTo(nDrawPoint_X_bottom, m_GraphInfo.GraphArea.bottom);
 					pDC->LineTo(dTmp_X, m_GraphInfo.GraphArea.bottom);
				} else if ((dTmp_Y_before > m_GraphInfo.GraphArea.bottom) && (dTmp_Y > m_GraphInfo.GraphArea.bottom)) {
					//
					//-------L
					//	*-*
					//�����I�[�o�[ => �����I�[�o�[
					pDC->MoveTo(dTmp_X_before, m_GraphInfo.GraphArea.bottom);
					pDC->LineTo(dTmp_X, m_GraphInfo.GraphArea.bottom);
				} else if ((dTmp_Y_before > m_GraphInfo.GraphArea.bottom) && (dTmp_Y <= m_GraphInfo.GraphArea.bottom)) {
					//	  *
					//---/---L
					//	*
					//�����I�[�o�[ => �O���t�̈��
					pDC->MoveTo(dTmp_X_before, m_GraphInfo.GraphArea.bottom);
					pDC->LineTo(nDrawPoint_X_bottom, m_GraphInfo.GraphArea.bottom);
				} else if ((dTmp_Y_before <= m_GraphInfo.GraphArea.bottom) && (dTmp_Y > m_GraphInfo.GraphArea.bottom)) {
					//	*
					//---\---L
					//	  *
					//�O���t�̈�� => ����I�[�o�[
					pDC->MoveTo(nDrawPoint_X_bottom, m_GraphInfo.GraphArea.bottom);
 					pDC->LineTo(dTmp_X, m_GraphInfo.GraphArea.bottom);
				} else if ((dTmp_Y_before < m_GraphInfo.GraphArea.top) && (dTmp_Y < m_GraphInfo.GraphArea.top)) {
					//	*-*
					//-------U
					//
					//����I�[�o�[ => ����I�[�o�[
					pDC->MoveTo(dTmp_X_before, m_GraphInfo.GraphArea.top);
					pDC->LineTo(dTmp_X, m_GraphInfo.GraphArea.top);
				} else if ((dTmp_Y_before >= m_GraphInfo.GraphArea.top) && (dTmp_Y < m_GraphInfo.GraphArea.top)) {
					//	  *
					//---/---U
					//	*
					//�O���t�̈�� => ����I�[�o�[
					pDC->MoveTo(nDrawPoint_X_top, m_GraphInfo.GraphArea.top);
					pDC->LineTo(dTmp_X, m_GraphInfo.GraphArea.top);
				} else if ((dTmp_Y_before < m_GraphInfo.GraphArea.top) && (dTmp_Y >= m_GraphInfo.GraphArea.top)) {
					//	*
					//---\---U
					//	  *
					//����I�[�o�[ => �O���t�̈��
					pDC->MoveTo(dTmp_X_before, m_GraphInfo.GraphArea.top);
					pDC->LineTo(nDrawPoint_X_top, m_GraphInfo.GraphArea.top);
				}


				//�l�̋L��
				dTmp_X_before = dTmp_X;
				dTmp_Y_before = dTmp_Y;
				pDC->MoveTo(dTmp_X, dTmp_Y);
			}
			pDC->SelectObject(pOldPen);
		}
	}

	//Cut
	cCutArea.top = m_GraphInfo.GraphArea.top;
	cCutArea.left = m_GraphInfo.GraphArea.left;
	cCutArea.right = m_GraphInfo.GraphArea.right;
	cCutArea.bottom = m_GraphInfo.GraphArea.bottom;
	CutGraphArea(pDC, pArea, &cCutArea);

	dStep = m_Config.X_Origin % m_Config.X_Step;
	dTmp = m_Config.X_Origin - dStep;
	dTmp /=  (double)m_Config.X_Step;
	dTmp += 1;
	dTmp *= (double)m_Config.X_Step;
	dTmp -= (double)m_Config.X_Origin;
	dGraphStepOrigin_X = (m_GraphInfo.RateX * dTmp) + m_GraphInfo.GraphArea.left;
	j = 1;
	nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	for (dIndex=dGraphStepOrigin_X; dIndex<=(m_GraphInfo.GraphArea.right ); dIndex+=dGraphStep_X) {
		if (dGraphStep_X < 1) {
			break;
		}

		//�������`��
		cMemArea.left = (int)(dIndex - (int)(tSize_X.cx / 2) - 5);
		cMemArea.top = m_GraphInfo.GraphArea.bottom + 1;
		cMemArea.right = (int)(dIndex + (int)(tSize_X.cx / 2) + 5);
		cMemArea.bottom = pArea->bottom;
		if((m_Config.X_Step * j) + (m_Config.X_Origin - dStep) >= m_Config.X_Max)
			break;
		if(cMemArea.right >= m_GraphInfo.GraphArea.right)
			break;
		//2009.10.17 bagus stress --{--
		//�������\���̉��P
		//�����̃������̕\��������Origin/Max�̃������̕`���ɏd�Ȃ�Ȃ��悤�ɂ���B
		if(cMemArea.left <= m_GraphInfo.GraphArea.left + (int)(tSize_X.cx / 2) + 5){
			//X_Origin�Əd�Ȃ�Ȃ��悤�ɁB
/* added 2020.05.30 hmenjo �O���t�ڐ���o�O ---------- { ---------- */
			j++;
/* added 2020.05.30 hmenjo �O���t�ڐ���o�O ---------- } ---------- */
			continue;
		}
		if(cMemArea.right >= m_GraphInfo.GraphArea.right - (int)(tSize_X.cx) - 5){
			//X_Max�Əd�Ȃ�Ȃ��悤�ɁB
/* added 2020.05.30 hmenjo �O���t�ڐ���o�O ---------- { ---------- */
			j++;
/* added 2020.05.30 hmenjo �O���t�ڐ���o�O ---------- } ---------- */
			continue;
		}
		//2009.10.17 bagus stress --}--
		wsprintf(szMem, "%d", (int)((m_Config.X_Step * j) + (m_Config.X_Origin - dStep)));
		strMem = szMem;
		pDC->DrawText(strMem, &cMemArea, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		j++;
	}

	//����
	wsprintf(szMem, "%d", m_Config.X_Origin);
	strMem = szMem;
	cMemArea.left = m_GraphInfo.GraphArea.left - (int)(tSize_X.cx / 2) - 5;
	cMemArea.top = m_GraphInfo.GraphArea.bottom + 1;
	cMemArea.right = m_GraphInfo.GraphArea.left + (int)(tSize_X.cx / 2) + 5;
	cMemArea.bottom = pArea->bottom;
	pDC->DrawText(strMem, &cMemArea, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	//�E��
	//2009.10.17 bagus stress --{--
	//�������\���̉��P
	//cMemArea.left = m_GraphInfo.GraphArea.right - (int)(tSize_X.cx / 2) - 5;
	cMemArea.left = m_GraphInfo.GraphArea.right - (int)(tSize_X.cx) - 5;
	cMemArea.top = m_GraphInfo.GraphArea.bottom + 1;
	//cMemArea.right = cMemArea.left + tSize_X.cx + 5; //pArea->right;
	cMemArea.right = m_GraphInfo.GraphArea.right;
	//2009.10.17 bagus stress --}--
	cMemArea.bottom = pArea->bottom;
	wsprintf(szMem, "%d", m_Config.X_Max);
	strMem = szMem;
	pDC->DrawText(strMem, &cMemArea, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);

	//Axis - Y
	dStep = m_Config.Y_Origin % m_Config.Y_Step;
	dTmp = (double)m_Config.Y_Origin - dStep;
	dTmp /= (double)m_Config.Y_Step;
	dTmp += 1;
	dTmp *= (double)m_Config.Y_Step;
	dTmp -= (double)m_Config.Y_Origin;
	dGraphStepOrigin_Y = (double)m_GraphInfo.GraphArea.bottom - (m_GraphInfo.RateY * dTmp);
	//Step (Y)
	j = 1;
	for (dIndex=dGraphStepOrigin_Y; dIndex>=(m_GraphInfo.GraphArea.top - dGraphStep_Y); dIndex-=dGraphStep_Y) {
		if (dGraphStep_Y < 1) {
			break;
		}

		//�������`��
		cMemArea.left = pArea->left;
		cMemArea.top = (int)dIndex - 5;
		cMemArea.right = m_GraphInfo.GraphArea.left;
		cMemArea.bottom = (int)dIndex + 5;
		if((m_Config.Y_Step * j) + (m_Config.Y_Origin - dStep) >= m_Config.Y_Max)
			break;
		if(cMemArea.top <= m_GraphInfo.GraphArea.top)
			break;
		wsprintf(szMem, "%d ", (int)((m_Config.Y_Step * j) + (m_Config.Y_Origin - dStep)));
		strMem = szMem;
		pDC->DrawText(strMem, &cMemArea, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
		j++;
	}

	//���
	cMemArea.left = pArea->left;
	cMemArea.top = pArea->top;
	cMemArea.right = m_GraphInfo.GraphArea.left;
	cMemArea.bottom = m_GraphInfo.GraphArea.top;
	wsprintf(szMem, "%d ", m_Config.Y_Max);
	strMem = szMem;
	pDC->DrawText(strMem, &cMemArea, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);
	//����
	cMemArea.left = pArea->left;
	cMemArea.top = m_GraphInfo.GraphArea.bottom - tSize_Y.cy;
	cMemArea.right = m_GraphInfo.GraphArea.left;
	cMemArea.bottom = m_GraphInfo.GraphArea.bottom;
	wsprintf(szMem, "%d ", m_Config.Y_Origin);
	strMem = szMem;
	pDC->DrawText(strMem, &cMemArea, DT_VCENTER | DT_RIGHT | DT_SINGLELINE);

	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);


///----------------------



	//�L�����u���[�V������
	if(m_Config.UseCalibration){
		CPen cSelectedCalibrationPen(PS_SOLID,1,m_Config.SelectedCalibrationColor);
		pDC->SelectObject(pOldPen);
		pOldPen = pDC->SelectObject(&cSelectedCalibrationPen);
		//�m��ς݂̐�
		for(i = 0;i < m_CalibrationData.DataNum;i++){
			if(m_CalibrationData.PointIndex[i] >= 0){
				pDC->MoveTo((int)((double)m_CalibrationData.PointIndex[i] * m_GraphInfo.DivX ) + m_GraphInfo.GraphArea.left
					,m_GraphInfo.GraphArea.top);
				pDC->LineTo((int)((double)m_CalibrationData.PointIndex[i] * m_GraphInfo.DivX ) + m_GraphInfo.GraphArea.left
					, m_GraphInfo.GraphArea.bottom);
			}
		}
		pDC->SelectObject(pOldPen);
		CPen cSelectingCalibrationPen(PS_SOLID,1,m_Config.SelectingCalibrationColor);
		pOldPen = pDC->SelectObject(&cSelectingCalibrationPen);
		//���݈ʒu���ߒ��̐�
		if(m_bNowCalibration && m_CalibrationData.DataNum >= 0){
			pDC->MoveTo((int)((double)m_CalibrationData.PointIndex[m_CalibrationData.DataNum ] * m_GraphInfo.DivX ) + m_GraphInfo.GraphArea.left
				,m_GraphInfo.GraphArea.top);
			pDC->LineTo((int)((double)m_CalibrationData.PointIndex[m_CalibrationData.DataNum ] * m_GraphInfo.DivX ) + m_GraphInfo.GraphArea.left
				, m_GraphInfo.GraphArea.bottom);
		}
		pDC->SelectObject(pOldPen);
	}

//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
	if (m_bAutoScaleMode) {
		m_Config.Y_Origin = static_cast<long>(dScaleYMin);
		m_Config.Y_Max = static_cast<long>(dScaleYMax);
		m_Config.Y_Step = static_cast<long>(dScaleYStep);
	}
//2009.09.03 bagus se �I�[�g�X�P�[�� --}--
}

void CNanoMeasureGraph::CutGraphArea(CDC *pDC, RECT *pArea, RECT *pCutArea)
{
	CBrush cGraphColor(m_Config.CanvasBackColor);
	RECT cCutArea;

	//Cut (Top)
	cCutArea.top = pArea->top;
	cCutArea.left = pArea->left;
	cCutArea.bottom = pCutArea->top;
	cCutArea.right = pArea->right;
	pDC->FillRect(&cCutArea, &cGraphColor);
	//Cut (Left)
	cCutArea.top = pArea->top;
	cCutArea.left = pArea->left;
	cCutArea.bottom = pArea->bottom;
	cCutArea.right = pCutArea->left;
	pDC->FillRect(&cCutArea, &cGraphColor);
	//Cut (Right)
	cCutArea.top = pArea->top;
	cCutArea.left = pCutArea->right + 1;
	cCutArea.bottom = pArea->bottom;
	cCutArea.right = pArea->right + 1;
	pDC->FillRect(&cCutArea, &cGraphColor);
	//Cut (Bottom)
	cCutArea.top = pCutArea->bottom + 1;
	cCutArea.left = pArea->left;
	cCutArea.bottom = pArea->bottom + 1;
	cCutArea.right = pArea->right;
	pDC->FillRect(&cCutArea, &cGraphColor);

}

void CNanoMeasureGraph::ClearAllData()
{
	memset(&m_Config,NULL,sizeof(m_Config));

	ResetCalibrationData();
	ClearGraphData();
}

void CNanoMeasureGraph::ClearGraphData()
{
	memset(m_LineData,NULL,sizeof(double) * NANO_MEASURE_MAX_GRAPH_LINE * NANO_MEASURE_MAX_POINT_DATA);
	memset(m_LinePointNum,NULL,sizeof(m_LinePointNum));

//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
	m_dYMin = DBL_MAX;
	m_dYMax = DBL_MIN;
//2009.09.03 bagus se �I�[�g�X�P�[�� --}--
}

/////////////////////////////////////////////////////////////////////////////////
// �f�[�^�𖖔��ɒǉ�
//	nLineNo: 0 - 9
//	dValue: �f�[�^
/////////////////////////////////////////////////////////////////////////////////
void CNanoMeasureGraph::AddLineData(int nLineNo,double dValue)
{
	//Invalid LineNo
	if(!IsValidLineNo(nLineNo)){
		return;
	}
	//Invalid Point Num
	if(m_LinePointNum[nLineNo] < 0)
		m_LinePointNum[nLineNo] = 0;
	//Data is full;
	if(m_LinePointNum[nLineNo] >= NANO_MEASURE_MAX_POINT_DATA)
		return;
	m_LineData[nLineNo][m_LinePointNum[nLineNo]] = dValue;
	m_LinePointNum[nLineNo]++;
	m_iGrapgSelectFlg = 0;

	//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
	if (m_dYMin > dValue)
		m_dYMin = floor(dValue);
	if (m_dYMax < dValue)
		m_dYMax = ceil(dValue);
	//2009.09.03 bagus se �I�[�g�X�P�[�� --}--

	Invalidate(FALSE);
	m_bReDrawGraph = TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// 2008-10-10OkabeAdd
// X���AY������double�l���������O���t�p
// �f�[�^�𖖔��ɒǉ��@�ő�F5000(NANO_MEASURE_MAX_POINT_DATA)
//	nLineNo: 0 - 9
//	nIndex: 0 - 4999
//	dXValue , dYValue: �f�[�^
/////////////////////////////////////////////////////////////////////////////////
void CNanoMeasureGraph::AddLineData(int nLineNo, double dCurrXValue, double dCurrYValue)
{
	//Invalid LineNo
	if(!IsValidLineNo(nLineNo)){
		return;
	}
	//Invalid Point Num
	if(m_LinePointNum[nLineNo] < 0)
		m_LinePointNum[nLineNo] = 0;
	//Data is full;
	if(m_LinePointNum[nLineNo] >= NANO_MEASURE_MAX_POINT_DATA)
		return;

	GraphXYDataValue CurrXYData;
	CurrXYData.dXValue = dCurrXValue;
	CurrXYData.dYValue = dCurrYValue;

	m_LineXYData[nLineNo][m_LinePointNum[nLineNo]] = CurrXYData;
	m_LinePointNum[nLineNo]++;
	m_iGrapgSelectFlg = 1;

	//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
	if (m_dYMin > dCurrYValue)
		m_dYMin = floor(dCurrYValue);
	if (m_dYMax < dCurrYValue)
		m_dYMax = ceil(dCurrYValue);
	//2009.09.03 bagus se �I�[�g�X�P�[�� --}--

	Invalidate(FALSE);
	m_bReDrawGraph = TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// �f�[�^�̈ꊇ����
//	nLineNo: 0 - 9
//	pValues: �f�[�^�̔z��
//	nPointNum: �f�[�^�̌�
/////////////////////////////////////////////////////////////////////////////////
void CNanoMeasureGraph::AddLineDataAll(int nLineNo,double *pValues,int nPointNum)
{
	int i;

	//Invalid LineNo
	if(!IsValidLineNo(nLineNo)){
		return;
	}

	m_LinePointNum[nLineNo] = 0;

	for(i = 0;i < nPointNum;i++){
		AddLineData(nLineNo,pValues[i]);
	}
}


BOOL CNanoMeasureGraph::SetGraphConfig(GraphConfig tConfig)
{
	if (tConfig.X_Step < 1) {
		tConfig.X_Step = 1;
	}

	if (tConfig.Y_Step < 1) {
		tConfig.Y_Step = 1;
	}

	m_Config = tConfig;

	for(int i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
		if(m_pLineSelect[i] == NULL){
			m_pLineSelect[i] = new CNanoLineSelect();
			m_pLineSelect[i]->Create("DDD",WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_OWNERDRAW,CRect(10,10,100,100),this,i);
		}
		m_pLineSelect[i]->SetDisplaySetting(m_Config.CanvasBackColor,m_LineConfig[i].Color, m_LineConfig[i].PenStyle);
	}
	return TRUE;
}

BOOL CNanoMeasureGraph::GetGraphConfig(CNanoMeasureGraph::GraphConfig *pConfig)
{
	if(pConfig){
		*pConfig = m_Config;
		return TRUE;
	}
	return FALSE;
}

BOOL CNanoMeasureGraph::SetLineConfig(int nIndex, LineConfig tConfig)
{
	if (!IsValidLineNo(nIndex)) {
		//NG
		return FALSE;
	}

	m_LineConfig[nIndex] = tConfig;
	if(m_pLineSelect[nIndex] != NULL){
		m_pLineSelect[nIndex]->SetDisplaySetting(m_Config.CanvasBackColor,m_LineConfig[nIndex].Color, m_LineConfig[nIndex].PenStyle);
	}

	return TRUE;
}

BOOL CNanoMeasureGraph::GetLineConfig(int nIndex,CNanoMeasureGraph::LineConfig* pConfig)
{
	if (!IsValidLineNo(nIndex)) {
		//NG
		return FALSE;
	}

	*pConfig = m_LineConfig[nIndex];
	return TRUE;
}

void CNanoMeasureGraph::Initialize()
{


}

LRESULT CNanoMeasureGraph::OnLineSelectChange(WPARAM wparam,LPARAM lparam)
{
	CString strDebug;
	int nIndex;

	nIndex = (int)lparam;
	if(!IsValidLineNo(nIndex)){
		return 0;
	}
	m_LineConfig[nIndex].Visible = ((int)wparam != 0 ? TRUE : FALSE);
	Invalidate(FALSE);
	m_bReDrawGraph = TRUE;
	return 0;
}

int CNanoMeasureGraph::ArrangeLineSelect(CRect cArea)
{
	int i;
	int nColumnWidth = 0;
	int nRowHeight = 0;
	SIZE tSize;
	int nNextWidth;

	nColumnWidth = cArea.left;
	nRowHeight = cArea.top;
	for(i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
		if(m_pLineSelect[i]){
			if(m_Config.UseLineSelect
			&& m_LineConfig[i].UseFlag){
				m_pLineSelect[i]->ShowWindow(SW_NORMAL);
				m_pLineSelect[i]->SetWindowText(m_LineConfig[i].LineName);
				m_pLineSelect[i]->SetCheck(m_LineConfig[i].Visible);
				m_pLineSelect[i]->SetFont(m_pTitleFont);
				if (i >= NANO_MEASURE_MAX_GRAPH_LINE-1) {
					nNextWidth = cArea.left;
				} else {
					tSize = m_pLineSelect[i+1]->GetIdealDispSize();
					nNextWidth = tSize.cx;
				}
				tSize = m_pLineSelect[i]->GetIdealDispSize();
				m_pLineSelect[i]->MoveWindow(CRect(nColumnWidth ,nRowHeight,nColumnWidth +tSize.cx,nRowHeight +tSize.cy));
				m_pLineSelect[i]->SetWindowPos(this,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
				nColumnWidth += tSize.cx;
				if(nColumnWidth >= cArea.right - nNextWidth){
					nRowHeight += tSize.cy;
					nColumnWidth = cArea.left;
				}
			}else{
				m_pLineSelect[i]->ShowWindow(SW_HIDE);
			}
		}
	}
	return m_Config.UseLineSelect ? (nRowHeight + tSize.cy - cArea.top) : 0;
}

void CNanoMeasureGraph::ResetCalibrationData()
{
	m_bNowCalibration = FALSE;
	m_CalibrationData.DataNum = -1;
	memset(&m_CalibrationData.PointIndex,0,sizeof(m_CalibrationData.PointIndex));
}

void CNanoMeasureGraph::AddCalibrationData(int nPointIndex)
{
	if(m_CalibrationData.DataNum < 0)
		m_CalibrationData.DataNum = 0;

	m_CalibrationData.PointIndex[m_CalibrationData.DataNum] = nPointIndex;
}
void CNanoMeasureGraph::ChangeCalibrationData(int nPointIndex)
{
	if(m_CalibrationData.DataNum < 0)
		m_CalibrationData.DataNum = 0;

	m_CalibrationData.PointIndex[m_CalibrationData.DataNum] = nPointIndex;
}

void CNanoMeasureGraph::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CButton::OnChar(nChar, nRepCnt, nFlags);
}

void CNanoMeasureGraph::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CButton::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

SIZE CNanoMeasureGraph::GetTextSize(CDC *pDC, CFont *pFont, CString strText)
{
	SIZE tRet;
	CFont *pOldFont;

	pOldFont = pDC->SelectObject(pFont);
	//2009.10.17 bagus stress --{--
	//GetTextExtentPoint(pDC->GetSafeHdc(),strText,strText.GetLength(),&tRet);
	tRet = pDC->GetOutputTextExtent(strText);
	//2009.10.17 bagus stress --}--
	pDC->SelectObject(pOldFont);

	return tRet;
}

BOOL CNanoMeasureGraph::PreTranslateMessage(MSG *pMsg)
{
	return FALSE;
}

void CNanoMeasureGraph::MoveCalibrationLine(int nStep)
{
	//�L�����u���[�V�������[�h�ł͂Ȃ�
	if(!m_Config.UseCalibration){
		return;
	}
	if(!m_bNowCalibration){
		return;
	}
	if(m_CalibrationData.DataNum < 0 || m_CalibrationData.DataNum >= m_Config.CalibrationLineNum){
		return;
	}
	m_CalibrationData.PointIndex[m_CalibrationData.DataNum] += nStep;
	if(m_CalibrationData.PointIndex[m_CalibrationData.DataNum] < 0){
		m_CalibrationData.PointIndex[m_CalibrationData.DataNum] = 0;
	}else if(m_CalibrationData.PointIndex[m_CalibrationData.DataNum] >= m_Config.PointNum){
		m_CalibrationData.PointIndex[m_CalibrationData.DataNum] = m_Config.PointNum-1;
	}
	Invalidate(FALSE);
	m_bReDrawGraph = TRUE;
}

void CNanoMeasureGraph::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SHORT nLShift;
	SHORT nRShift;

	nLShift = GetKeyState(VK_LSHIFT);
	nRShift = GetKeyState(VK_RSHIFT);

	if(!m_Config.UseCalibration)
		return;
	switch(nChar){
	case VK_LEFT:
		if(m_bNowCalibration){
			if(nLShift < 0 || nRShift < 0){
				MoveCalibrationLine(-5);
			}else{
				MoveCalibrationLine(-1);
			}
		}
		break;
	case VK_RIGHT:
		if(m_bNowCalibration){
			if(nLShift < 0 || nRShift < 0){
				MoveCalibrationLine(5);
			}else{
				MoveCalibrationLine(1);
			}
		}
		break;
	default:
		break;
	}

	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CNanoMeasureGraph::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CButton::OnShowWindow(bShow, nStatus);

}

void CNanoMeasureGraph::InitBackBuffer()
{
	CClientDC dc(this);
	CRect cArea;

	GetClientRect(cArea);

	if(m_bbBitmap){
		m_bbBitmap->DeleteObject();
		delete m_bbBitmap;
	}
	if(m_bbDC){
		m_bbDC->DeleteDC();
		delete m_bbDC;
	}
	m_bbDC = new CDC();
	m_bbDC->CreateCompatibleDC(&dc);
	m_bbBitmap = new CBitmap();
	m_bbBitmap->CreateCompatibleBitmap(&dc,cArea.Width(),cArea.Height());
	m_bbDC->SetBkMode(OPAQUE);
	m_bbDC->SelectObject(m_bbBitmap);
	m_bbOK = FALSE;
}

void CNanoMeasureGraph::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	Invalidate(TRUE);
}

CNanoMeasureGraph::CalibrationData CNanoMeasureGraph::GetCalibrationData()
{
	return m_CalibrationData;
}

BOOL CNanoMeasureGraph::IsValidLineNo(int nLineNo)
{
	if(nLineNo < 0 || nLineNo >= NANO_MEASURE_MAX_GRAPH_LINE){
		return FALSE;
	}
	return TRUE;
}

//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
void CNanoMeasureGraph::SetAutoScaleMode(BOOL bMode)
{
	m_bAutoScaleMode = bMode;
	//2010.01.19 bagus --{--
	Invalidate(TRUE);
	//2010.01.19 bagus --}--
}

BOOL CNanoMeasureGraph::GetAutoScaleMode()
{
	return m_bAutoScaleMode;
}
//2009.09.03 bagus se �I�[�g�X�P�[�� --}--
