// NanoPointGraph.cpp : インプリメンテーション ファイル
//
#include "stdafx.h"
#include "NanoPointGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNanoPointGraph

CNanoPointGraph::CNanoPointGraph()
{
	m_PointNum = 0;
	m_SelectedPoint = -1;
	memset(m_PointData,NULL,sizeof(m_PointData));
	memset(&m_Config,NULL,sizeof(m_Config));
	//
	m_bbDC = NULL;
	m_bbBitmap = NULL;
	m_bbOK = FALSE;

	m_bLSStatusMode = FALSE;
	memset(m_LSData,NULL,sizeof(m_LSData));
	m_LSNum = 0;
	m_SelectedLS = -1;

	memset(m_PinData,NULL,sizeof(m_PinData));
	m_PinNum = NULL;

	// 2009.10.14 bagus Distance 追加 --{--
	m_bDistanceMode = FALSE;
	// 2009.10.14 bagus Distance 追加 --}--

	// 2013.01.09 bagus -->
	memset(&m_OriginPointData,NULL,sizeof(m_OriginPointData));
	// 2013.01.09 bagus M--
}

CNanoPointGraph::~CNanoPointGraph()
{
	if(m_bbBitmap){
		m_bbBitmap->DeleteObject();
		delete m_bbBitmap;
	}
	if(m_bbDC){
		m_bbDC->DeleteDC();
		delete m_bbDC;
	}
}


BEGIN_MESSAGE_MAP(CNanoPointGraph, CButton)
	//{{AFX_MSG_MAP(CNanoPointGraph)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		ON_WM_SIZE()
		ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoPointGraph メッセージ ハンドラ

void CNanoPointGraph::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect cButtonRect;
	CRect cTmpRect;
	CRect cClipArea;
	int	  nRet;
	double	dblRateX;
	double	dblRateY;
	int		i;
	int		nDrawX;
	int		nDrawY;

	cButtonRect.CopyRect(&lpDIS->rcItem);

	cTmpRect = cButtonRect;
	cTmpRect.left = cTmpRect.left - NANO_POINT_MAP_EDGE_WIDTH;
	cTmpRect.right = cTmpRect.right - NANO_POINT_MAP_EDGE_WIDTH;
	cTmpRect.top = cTmpRect.top - NANO_POINT_MAP_EDGE_WIDTH;
	cTmpRect.bottom = cTmpRect.bottom - NANO_POINT_MAP_EDGE_WIDTH;

	dblRateX = (cTmpRect.right - cTmpRect.left) / (double)(m_Config.SampleSizeX - 0);
	dblRateY = (cTmpRect.bottom - cTmpRect.top) / (double)(m_Config.SampleSizeY - 0);

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
		for(i = 0;i < m_PointNum;i++){
			//Redraw only changed points
			if(m_PointData[i].NeedRedraw){
				nDrawX = (int)(m_PointData[i].DataX * dblRateX);
				nDrawY = (int)(m_PointData[i].DataY * dblRateY);
				pDC->BitBlt(nDrawX-NANO_POINT_MAP_CROESS_MARK_LENGTH,nDrawY-NANO_POINT_MAP_CROESS_MARK_LENGTH,
							nDrawX+NANO_POINT_MAP_CROESS_MARK_LENGTH,nDrawY+NANO_POINT_MAP_CROESS_MARK_LENGTH,
							m_bbDC,nDrawX-NANO_POINT_MAP_CROESS_MARK_LENGTH,nDrawY-NANO_POINT_MAP_CROESS_MARK_LENGTH,
							SRCCOPY);

			}
		}
	}else{
		pDC->BitBlt(cButtonRect.left,cButtonRect.top,cButtonRect.Width(),cButtonRect.Height(),m_bbDC,cButtonRect.left,cButtonRect.top,SRCCOPY);
		m_bbOK = TRUE;
	}
}

void CNanoPointGraph::DrawItemMain(LPDRAWITEMSTRUCT lpDIS){
	//CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CDC* pDC = m_bbDC;

	UINT	uState = lpDIS->itemState;
	CRect	cFocusRect;
	CRect	cButtonRect;
	CRect	cTmpRect;

	double	dblRateX;
	double	dblRateY;

	cFocusRect.CopyRect(&lpDIS->rcItem);
	cButtonRect.CopyRect(&lpDIS->rcItem);

	cTmpRect = cButtonRect;
	cTmpRect.left = cTmpRect.left - NANO_POINT_MAP_EDGE_WIDTH;
	cTmpRect.right = cTmpRect.right - NANO_POINT_MAP_EDGE_WIDTH;
	cTmpRect.top = cTmpRect.top - NANO_POINT_MAP_EDGE_WIDTH;
	cTmpRect.bottom = cTmpRect.bottom - NANO_POINT_MAP_EDGE_WIDTH;

	dblRateX = (cTmpRect.right - cTmpRect.left) / (double)(m_Config.SampleSizeX - 0);
	dblRateY = (cTmpRect.bottom - cTmpRect.top) / (double)(m_Config.SampleSizeY - 0);

	//Draw background
	ClearGraph(pDC,cButtonRect);

	//Draw Lines
	if (GetLineStatusMode()) {
		//Line/Section
		DrawLines(pDC, dblRateX, dblRateY);
		//Pins
		DrawPins(pDC, dblRateX, dblRateY);
	}

// 2013.01.09 bagus -->
	DrawOriginPoint(pDC, dblRateX, dblRateY);
// 2013.01.09 bagus <--

	//Draw MEASURED points
	DrawMeasuredPoints(pDC, dblRateX, dblRateY);

	//Draw NOT_MEASURED points
	DrawNotMeasuredPoints(pDC, dblRateX, dblRateY);

	//Draw MEASURING point
	DrawMeasuringPoints(pDC, dblRateX, dblRateY);


	// 2009.10.14 bagus Distance 追加 --{--
	//Draw Selected point
//	DrawSelectedPoint(pDC, dblRateX, dblRateY);
	if (GetDistanceMode()) {
		// Draw 2 Point Distance
		Draw2PointDistance(pDC, dblRateX, dblRateY);
	} else {
		//Draw Selected point
		DrawSelectedPoint(pDC, dblRateX, dblRateY);
	}
	// 2009.10.14 bagus Distance 追加 --}--

	//Draw Edge
	DrawEdge(pDC, cButtonRect);

}

void CNanoPointGraph::Reset()
{
	m_PointNum = 0;
	m_SelectedPoint = -1;
	memset(m_PointData,NULL,sizeof(m_PointData));
	Invalidate(TRUE);
}

void CNanoPointGraph::AddPoint(long x, long y, PointStatus status)
{
	PointData tData;

	tData.DataX = x;
	tData.DataY = y;
	tData.Status = status;
	tData.NeedRedraw = TRUE;
	AddPoint(&tData);
}
void CNanoPointGraph::AddPoint(PointData *pData)
{
	//Data is full.
	if(m_PointNum >= NANO_POINT_MAP_DATA_MAX){
		return;
	}
	m_PointData[m_PointNum] = *pData;
	m_PointData[m_PointNum].NeedRedraw = TRUE;
	m_PointNum++;
	Invalidate(FALSE);

}
void CNanoPointGraph::AddPointArray(int nPoint,PointData *pData)
{
	int i;

	for(i = 0;i < nPoint;i++){
		AddPoint(&pData[i]);
	}
}

void CNanoPointGraph::Finish(int nPoint)
{
	for(int i = 0;i < nPoint;i++){
		ChangeStatus(i,PointStatus::MEASURED);
	}
	ChangeStatus(nPoint,PointStatus::MEASURED);
	Invalidate(FALSE);

}
void CNanoPointGraph::Select(int nPoint)
{
	//2009.10.28 bagus 2point Distance --{--
	if(GetDistanceMode()){
		SelectDistance(nPoint*2);
		return;
	}
	//2009.10.28 bagus 2point Distance --}--

	//OkabeChange--->
	if(IsValidPoint(m_SelectedPoint)){
		m_PointData[m_SelectedPoint].NeedRedraw = TRUE;
	}
	if(IsValidPoint(nPoint)){
		m_PointData[nPoint].NeedRedraw = TRUE;
		m_SelectedPoint = nPoint;
	}
	else{
		m_SelectedPoint = -1;
	}
	Invalidate(FALSE);

	/*
	if(!IsValidPoint(nPoint)){
		return;
	}
	if(IsValidPoint(m_SelectedPoint)){
		m_PointData[m_SelectedPoint].NeedRedraw = TRUE;
	}
	m_PointData[nPoint].NeedRedraw = TRUE;
	m_SelectedPoint = nPoint;
	Invalidate(FALSE);
	}
	*/
}
void CNanoPointGraph::Measure(int nPoint)
{
	for(int i = 0;i < nPoint;i++){
		ChangeStatus(i,PointStatus::MEASURED);
	}
	ChangeStatus(nPoint,PointStatus::MEASURING);
	Invalidate(FALSE);
}

void CNanoPointGraph::ChangeStatus(int nPoint, PointStatus status)
{
	if(!IsValidPoint(nPoint)){
		return;
	}
	m_PointData[nPoint].Status = status;
	m_PointData[nPoint].NeedRedraw = TRUE;
}

BOOL CNanoPointGraph::IsValidPoint(int nPoint)
{
	if(nPoint < 0 || nPoint >= NANO_POINT_MAP_DATA_MAX){
		return FALSE;
	}
	if(nPoint >= m_PointNum){
		return FALSE;
	}
	return TRUE;
}

//--------------------------------------------------
//		ポイントマッピンググラフ領域クリア
//--------------------------------------------------
void CNanoPointGraph::ClearGraph(CDC *pDC,RECT *pScreenSize)
{
	CBrush cCanvasColor(m_Config.BackColor);
	CBrush *pOldBrush;

	pOldBrush = pDC->SelectObject(&cCanvasColor);
	pDC->FillRect(pScreenSize,&cCanvasColor);

	pDC->SelectObject(pOldBrush);
}

//--------------------------------------------------
//		原点座標 描画
//--------------------------------------------------
void CNanoPointGraph::DrawOriginPoint(CDC *pDC,double dblRateX,double dblRateY)
{
	CPen* pOldPen;

	int nDrawX;
	int nDrawY;

	CPen cLinePen(PS_SOLID, 1, m_Config.OriginColor);
	pOldPen = pDC->SelectObject(&cLinePen);

	nDrawX = (int)(m_OriginPointData.DataX * dblRateX);
	nDrawY = (int)(m_OriginPointData.DataY * dblRateY);

	DrawPointDot(pDC, nDrawX, nDrawY);

	pDC->SelectObject(pOldPen);
}

//--------------------------------------------------
//		測定終了座標 描画
//--------------------------------------------------
void CNanoPointGraph::DrawMeasuredPoints(CDC *pDC,double dblRateX,double dblRateY)
{
	int	i;
	CPen* pOldPen;

	int nDrawX;
	int nDrawY;

	CPen cLinePen(PS_SOLID, 1, m_Config.StatusColor[MEASURED]);
	pOldPen = pDC->SelectObject(&cLinePen);
	for (i=0; i<m_PointNum; i++) {
		if (m_PointData[i].Status != MEASURED) {
			continue;
		}

		nDrawX = (int)(m_PointData[i].DataX * dblRateX);
		nDrawY = (int)(m_PointData[i].DataY * dblRateY);

		DrawPointDot(pDC, nDrawX, nDrawY);
	}
	pDC->SelectObject(pOldPen);
}

//--------------------------------------------------
//		測定前座標 描画
//--------------------------------------------------
void CNanoPointGraph::DrawNotMeasuredPoints(CDC *pDC,double dblRateX,double dblRateY)
{
	int	i;
	CPen* pOldPen;

	int nDrawX;
	int nDrawY;

	CPen cLinePen(PS_SOLID, 1, m_Config.StatusColor[NOT_MEASURED]);
	pOldPen = pDC->SelectObject(&cLinePen);
	for (i=0; i<m_PointNum; i++) {
		if (m_PointData[i].Status != NOT_MEASURED) {
			continue;
		}

		nDrawX = (int)(m_PointData[i].DataX * dblRateX);
		nDrawY = (int)(m_PointData[i].DataY * dblRateY);

		DrawPointDot(pDC, nDrawX, nDrawY);
	}
	pDC->SelectObject(pOldPen);
}

//--------------------------------------------------
//		測定中座標 描画
//--------------------------------------------------
void CNanoPointGraph::DrawMeasuringPoints(CDC *pDC,double dblRateX,double dblRateY)
{
	int	i;
	CPen* pOldPen;

	int nDrawX;
	int nDrawY;

	CPen cLinePen(PS_SOLID, NANO_POINT_MAP_CROESS_MARK_WIDTH, m_Config.StatusColor[MEASURING]);
	pOldPen = pDC->SelectObject(&cLinePen);

	for (i=0; i<m_PointNum; i++) {
		if (m_PointData[i].Status != MEASURING) {
			continue;
		}

		nDrawX = (int)(m_PointData[i].DataX * dblRateX);
		nDrawY = (int)(m_PointData[i].DataY * dblRateY);

		DrawPointCross(pDC, nDrawX, nDrawY);
	}
	pDC->SelectObject(pOldPen);
}

//--------------------------------------------------
//		選択座標 描画
//--------------------------------------------------
void CNanoPointGraph::DrawSelectedPoint(CDC *pDC,double dblRateX,double dblRateY)
{
	CPen* pOldPen;

	int nDrawX;
	int nDrawY;

	if (IsValidPoint(m_SelectedPoint) != TRUE) {
		return;
	}

	CPen cLinePen(PS_SOLID, NANO_POINT_MAP_CROESS_MARK_WIDTH, m_Config.StatusColor[SELECTED]);
	pOldPen = pDC->SelectObject(&cLinePen);

	nDrawX = (int)(m_PointData[m_SelectedPoint].DataX * dblRateX);
	nDrawY = (int)(m_PointData[m_SelectedPoint].DataY * dblRateY);

	DrawPointCross(pDC, nDrawX, nDrawY);

	pDC->SelectObject(pOldPen);
}

//--------------------------------------------------
//		ポイントマッピンググラフ領域縁取り
//--------------------------------------------------
void CNanoPointGraph::DrawEdge(CDC *pDC,RECT *pScreenSize)
{
	CBrush cEdgeTopLeft(m_Config.EdgeColor[EDGE_UPPER]);
	CBrush cEdgeBottomRight(m_Config.EdgeColor[EDGE_LOWER]);
	RECT cCutArea;

	//Edge (Left)
	cCutArea.top = pScreenSize->top;
	cCutArea.left = pScreenSize->left;
	cCutArea.bottom = pScreenSize->bottom;
	cCutArea.right = pScreenSize->left + NANO_POINT_MAP_EDGE_WIDTH;
	pDC->FillRect(&cCutArea, &cEdgeTopLeft);
	//Edge (Right)
	cCutArea.top = pScreenSize->top;
	cCutArea.left = pScreenSize->right - NANO_POINT_MAP_EDGE_WIDTH;
	cCutArea.bottom = pScreenSize->bottom;
	cCutArea.right = pScreenSize->right;
	pDC->FillRect(&cCutArea, &cEdgeBottomRight);
	//Edge (Top)
	cCutArea.top = pScreenSize->top;
	cCutArea.left = pScreenSize->left;
	cCutArea.bottom = pScreenSize->top + NANO_POINT_MAP_EDGE_WIDTH;
	cCutArea.right = pScreenSize->right;
	pDC->FillRect(&cCutArea, &cEdgeTopLeft);
	//Edge (Bottom)
	cCutArea.top = pScreenSize->bottom - NANO_POINT_MAP_EDGE_WIDTH;
	cCutArea.left = pScreenSize->left;
	cCutArea.bottom = pScreenSize->bottom;
	cCutArea.right = pScreenSize->right;
	pDC->FillRect(&cCutArea, &cEdgeBottomRight);
}

//--------------------------------------------------
//		測定点：ドット 描画
//--------------------------------------------------
void CNanoPointGraph::DrawPointDot(CDC *pDC,int nX,int nY)
{
/*
	//Top
	pDC->MoveTo(nX-1, nY-1);
	pDC->LineTo(nX+2, nY-1);
	//Middle
	pDC->MoveTo(nX-1, nY);
	pDC->LineTo(nX+2, nY);
	//Bottom
	pDC->MoveTo(nX-1, nY+1);
	pDC->LineTo(nX+2, nY+1);
*/

	if (m_bLSStatusMode) {
		// --- for DrawLine ---
		pDC->MoveTo(nX-1, nY-1);	// Top
		pDC->LineTo(nX+2, nY-1);
		pDC->MoveTo(nX-1, nY);		// Middle
		pDC->LineTo(nX+2, nY);
		pDC->MoveTo(nX-1, nY+1);	// Bottom
		pDC->LineTo(nX+2, nY+1);
	} else {
		// --- for DrawPoint ---
		pDC->MoveTo(nX-1, nY-1);	// Top
		pDC->LineTo(nX+2, nY-1);
		pDC->MoveTo(nX-1, nY);		// Middle
		pDC->LineTo(nX+2, nY);
		pDC->MoveTo(nX-1, nY+1);	// Bottom
		pDC->LineTo(nX+2, nY+1);
	}
}

//--------------------------------------------------
//		測定点：クロス 描画
//--------------------------------------------------
void CNanoPointGraph::DrawPointCross(CDC *pDC,int nX,int nY)
{
/*
	// |
	pDC->MoveTo(nX, nY - NANO_POINT_MAP_CROESS_MARK_LENGTH);
	pDC->LineTo(nX, nY + NANO_POINT_MAP_CROESS_MARK_LENGTH + 1);
	// ―
	pDC->MoveTo(nX - NANO_POINT_MAP_CROESS_MARK_LENGTH, nY);
	pDC->LineTo(nX + NANO_POINT_MAP_CROESS_MARK_LENGTH + 1, nY);
*/

	if (m_bLSStatusMode) {
		// --- for DrawLine ---
		pDC->MoveTo(nX, nY - NANO_POINT_MAP_CROESS_MARK_LENGTH);			// |
		pDC->LineTo(nX, nY + NANO_POINT_MAP_CROESS_MARK_LENGTH + 1);
		pDC->MoveTo(nX - NANO_POINT_MAP_CROESS_MARK_LENGTH, nY);			// ->
		pDC->LineTo(nX + NANO_POINT_MAP_CROESS_MARK_LENGTH + 1, nY);
	} else {
		// --- for DrawPoint ---
		pDC->MoveTo(nX, nY - NANO_POINT_MAP_CROESS_MARK_LENGTH);			// |
		pDC->LineTo(nX, nY + NANO_POINT_MAP_CROESS_MARK_LENGTH + 1);
		pDC->MoveTo(nX - NANO_POINT_MAP_CROESS_MARK_LENGTH, nY);			// ->
		pDC->LineTo(nX + NANO_POINT_MAP_CROESS_MARK_LENGTH + 1, nY);
	}
}

void CNanoPointGraph::SetConfig(GraphConfig *pConfig)
{
	m_Config = *pConfig;
}
void CNanoPointGraph::GetConfig(GraphConfig *pConfig)
{
	*pConfig = m_Config;
}
void CNanoPointGraph::ReDraw()
{
	if(this->GetSafeHwnd()){
		Invalidate(TRUE);
	}
}
//------------------------------------------------------------------------
// OnSize
//------------------------------------------------------------------------
void CNanoPointGraph::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	InitBackBuffer();
}
void CNanoPointGraph::InitBackBuffer()
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

void CNanoPointGraph::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	Invalidate(TRUE);
}
//@@@ bagus stress 測定ポイントグラフ対応 --}--
void CNanoPointGraph::SetLineStatusMode(BOOL bMode)
{
	m_bLSStatusMode = bMode;
}
BOOL CNanoPointGraph::GetLineStatusMode()
{
	return m_bLSStatusMode;
}
void CNanoPointGraph::AddLineData(LSData *pData)
{
	//Data is full.
	if(m_LSNum >= (MAX_POINT_MAP_LINE_MAX * MAX_POINT_MAP_SECTION_MAX)){
		return;
	}
	m_LSData[m_LSNum] = *pData;
	m_LSNum++;
	Invalidate(FALSE);
}
void CNanoPointGraph::SelectSection(int nNo)
{
	if(IsValidLine(nNo)){
		m_SelectedLS = nNo;
	}
	else{
		m_SelectedLS = -1;
	}
	Invalidate(FALSE);
}
void CNanoPointGraph::FinishSection(int nNo)
{
	int	i;

	for(i=0; i<nNo; i++){
		if (m_LSData[i].Status != SectionStatus::SECTION_NOT_USED) {
			ChangeSectionStatus(i, SectionStatus::SECTION_MEASURED);
		}
	}
	if (m_LSData[nNo].Status != SectionStatus::SECTION_NOT_USED) {
		ChangeSectionStatus(nNo, SectionStatus::SECTION_MEASURED);
	}
	Invalidate(FALSE);
}
void CNanoPointGraph::MeasureSection(int nNo)
{
	int	i;

	for(i=0; i<nNo; i++){
		if (m_LSData[i].Status != SectionStatus::SECTION_NOT_USED) {
			ChangeSectionStatus(i, SectionStatus::SECTION_MEASURED);
		}
	}
	if (m_LSData[nNo].Status != SectionStatus::SECTION_NOT_USED) {
		ChangeSectionStatus(nNo, SectionStatus::SECTION_MEASURING);
	}
	Invalidate(FALSE);
}
void CNanoPointGraph::ResetSection()
{
	m_LSNum = 0;
	m_SelectedLS = -1;
	memset(m_LSData,NULL,sizeof(m_LSData));
	Invalidate(TRUE);
}
BOOL CNanoPointGraph::IsValidLine(int nNo)
{
	//if(nNo < 0 || nNo >= (MAX_POINT_MAP_LINE_MAX * MAX_POINT_MAP_SECTION_MAX)){
	//	return FALSE;
	//}
	if(nNo >= m_LSNum){
		return FALSE;
	}

	return TRUE;
}

void CNanoPointGraph::ChangeSectionStatus(int nNo,SectionStatus status)
{
	if(!IsValidLine(nNo)){
		return;
	}
	m_LSData[nNo].Status = status;
}

void CNanoPointGraph::DrawLines(CDC *pDC, double dblRateX, double dblRateY)
{
	int	i;
	CPen* pOldPen;

	int nLineSX, nLineEX, nLineSY, nLineEY;
	int nDrawSX, nDrawEX, nDrawSY, nDrawEY, nLengthX, nLengthY;

	CPen cLinePenNotMeasured(PS_SOLID, NANO_POINT_MAP_LINE_WIDTH, m_Config.SectionStatusColor[SECTION_NOT_MEASURED]);
	CPen cLinePenMeasuring(PS_SOLID, NANO_POINT_MAP_LINE_WIDTH, m_Config.SectionStatusColor[SECTION_MEASURING]);
	CPen cLinePenMeasured(PS_SOLID, NANO_POINT_MAP_LINE_WIDTH, m_Config.SectionStatusColor[SECTION_MEASURED]);
	CPen cLinePenSlected(PS_SOLID, NANO_POINT_MAP_LINE_WIDTH, m_Config.SectionStatusColor[SECTION_SELECTED]);

	for (i=0; i<m_LSNum; i++) {
		nLineSX = (int)(m_LSData[i].StartX * dblRateX);
		nLineEX = (int)(m_LSData[i].EndX * dblRateX);
		nLineSY = (int)(m_LSData[i].StartY * dblRateY);
		nLineEY = (int)(m_LSData[i].EndY * dblRateY);

		nLengthX = (int)(abs)(nLineEX - nLineSX);
		nLengthY = (int)(abs)(nLineEY - nLineSY);

		switch ( m_LSData[i].Status )
		{
		case SECTION_NOT_MEASURED:
			pOldPen = pDC->SelectObject(&cLinePenNotMeasured);
			break;
		case SECTION_MEASURING:
			pOldPen = pDC->SelectObject(&cLinePenMeasuring);
			break;
		case SECTION_MEASURED:
			pOldPen = pDC->SelectObject(&cLinePenMeasured);
			break;
		case SECTION_SELECTED:
			pOldPen = pDC->SelectObject(&cLinePenSlected);
			break;
		default:
			continue;
			break;
		}

		nDrawSX = nLineSX;
		nDrawEX = nDrawSX + nLengthX;
		nDrawSY = nLineSY;
		nDrawEY = nDrawSY + nLengthY;

		pDC->MoveTo(nDrawSX, nDrawSY);
		pDC->LineTo(nDrawEX, nDrawEY);

		pDC->SelectObject(pOldPen);
	}
	//2009.08.31 bagus stress --{--
	//選択状態のものは最後にもう一度上書きする
	if(m_SelectedLS >= 0 && m_SelectedLS < m_LSNum){
		pOldPen = pDC->SelectObject(&cLinePenSlected);
		nLineSX = (int)(m_LSData[m_SelectedLS].StartX * dblRateX);
		nLineEX = (int)(m_LSData[m_SelectedLS].EndX * dblRateX);
		nLineSY = (int)(m_LSData[m_SelectedLS].StartY * dblRateY);
		nLineEY = (int)(m_LSData[m_SelectedLS].EndY * dblRateY);
		nLengthX = (int)(abs)(nLineEX - nLineSX);
		nLengthY = (int)(abs)(nLineEY - nLineSY);
		nDrawSX = nLineSX;
		nDrawEX = nDrawSX + nLengthX;
		nDrawSY = nLineSY;
		nDrawEY = nDrawSY + nLengthY;

		pDC->MoveTo(nDrawSX, nDrawSY);
		pDC->LineTo(nDrawEX, nDrawEY);
		pDC->SelectObject(pOldPen);
	}
	//2009.08.31 bagus stress --}--
}
//@@@ bagus stress 測定ポイントグラフ対応 --}--
//2009.08.30 bagus stress --{--
void CNanoPointGraph::SelectSectionByLSNo(int nLineNo,int nSectionNo /*= 0 */)
{
	if(!IsValidLSNo(nLineNo,nSectionNo)){
		return;
	}
	for(int nNo =0;nNo < m_LSNum;nNo++){
		if(m_LSData[nNo].SourceLineNo == nLineNo){
			//if(nSectionNo == 0 || m_LSData[nNo].SourceSectionNo == nSectionNo || m_LSData[nNo].Status != SECTION_NOT_USED){
			if((nSectionNo == 0 || m_LSData[nNo].SourceSectionNo == nSectionNo) && m_LSData[nNo].Status != SECTION_NOT_USED){
				ChangeSectionStatus(nNo,SECTION_SELECTED);
			}
		}
	}
}
void CNanoPointGraph::FinishSectionByLSNo(int nLineNo,int nSectionNo /*= 0*/)
{
	if(!IsValidLSNo(nLineNo,nSectionNo)){
		return;
	}
	for(int nNo =0;nNo < m_LSNum;nNo++){
		if(m_LSData[nNo].SourceLineNo == nLineNo){
			//if(nSectionNo == 0 || m_LSData[nNo].SourceSectionNo == nSectionNo || m_LSData[nNo].Status != SECTION_NOT_USED){
			if((nSectionNo == 0 || m_LSData[nNo].SourceSectionNo == nSectionNo) && m_LSData[nNo].Status != SECTION_NOT_USED){
				ChangeSectionStatus(nNo,SECTION_MEASURED);
			}
		}
	}
}
void CNanoPointGraph::MeasureSectionByLSNo(int nLineNo,int nSectionNo /*= 0*/)
{
	if(!IsValidLSNo(nLineNo,nSectionNo)){
		return;
	}
	for(int nNo =0;nNo < m_LSNum;nNo++){
		if(m_LSData[nNo].SourceLineNo == nLineNo){
			//if(nSectionNo == 0 || m_LSData[nNo].SourceSectionNo == nSectionNo || m_LSData[nNo].Status != SECTION_NOT_USED){
			if((nSectionNo == 0 || m_LSData[nNo].SourceSectionNo == nSectionNo) && m_LSData[nNo].Status != SECTION_NOT_USED){
				ChangeSectionStatus(nNo,SECTION_MEASURING);
			}
		}
	}
}
BOOL CNanoPointGraph::IsValidLSNo(int nLineNo,int nSectionNo /*= 0*/)
{
	if(nLineNo < 1 || nLineNo >= STRESS_LINES_MAX){
		return FALSE;
	}
	if(nSectionNo != 0){
		if(nSectionNo < 1 || nSectionNo >= STRESS_SECTIONS_MAX){
			return FALSE;
		}
	}
	return TRUE;
}
//2009.08.30 bagus stress --}--
//2009.09.10 bagus stress --{--
void CNanoPointGraph::ResetPin()
{
	m_PinNum = 0;
	memset(m_PinData,NULL,sizeof(m_PinData));
}
void CNanoPointGraph::AddPinData(long x,long y)
{
	PinData tPin;
	if(m_PinNum >= MAX_POINT_MAP_PIN_MAX){
		return;
	}
	tPin.DataX = x;
	tPin.DataY = y;
	m_PinData[m_PinNum] = tPin;
	m_PinNum++;
	Invalidate(FALSE);
}
//2009.09.10 bagus stress --}--
void CNanoPointGraph::DrawPins(CDC *pDC, double dblRateX, double dblRateY)
{
	int	i;
	CPen* pOldPen;

	int nDrawX;
	int nDrawY;

	CPen cLinePen(PS_SOLID, 1, m_Config.PinColor);
	pOldPen = pDC->SelectObject(&cLinePen);
	for (i=0; i<m_PinNum; i++) {

		nDrawX = (int)(m_PinData[i].DataX * dblRateX);
		nDrawY = (int)(m_PinData[i].DataY * dblRateY);

		DrawPointDot(pDC,nDrawX,nDrawY);
	}
	pDC->SelectObject(pOldPen);

}

// 2009.09.11 bagus stress --{--
void CNanoPointGraph::ResetAll()
{
	Reset();
	ResetSection();
	ResetPin();
}
// 2009.09.11 bagus stress --}--

// 2009.10.14 bagus Distance 追加 --{--
void CNanoPointGraph::SetDistanceMode(BOOL bMode)
{
	m_bDistanceMode = bMode;
}

BOOL CNanoPointGraph::GetDistanceMode()
{
	return m_bDistanceMode;
}

void CNanoPointGraph::SelectDistance(int nNo)
{
	int	nRem;

	if(IsValidPoint(nNo)){
		nRem = nNo % 2;
		if (nRem == 0) {
			// 0,2,4,6,8...
			m_SelectedDistance = nNo;
		} else {
			// 1,3,5,7,9...
			m_SelectedDistance = nNo - 1;
		}
	}
	else{
		m_SelectedDistance = -1;
	}
	Invalidate(FALSE);
}

void CNanoPointGraph::Draw2PointDistance(CDC *pDC,double dblRateX,double dblRateY)
{
	if (IsValidPoint(m_SelectedDistance) != TRUE) {
		return;
	}

	if (IsValidPoint(m_SelectedDistance + 1) != TRUE) {
		return;
	}

	CPen* pOldPen;

	int nDrawSX, nDrawEX, nDrawSY, nDrawEY;

	CPen cSLinePen(PS_SOLID, NANO_POINT_MAP_CROESS_MARK_WIDTH, m_Config.DistanceStartColor);
	CPen cELinePen(PS_SOLID, NANO_POINT_MAP_CROESS_MARK_WIDTH, m_Config.DistanceEndColor);
	CPen cLLinePen(PS_SOLID, NANO_POINT_MAP_DISTANCE_LINE_WIDTH, m_Config.DistanceLineColor);

	nDrawSX = (int)(m_PointData[m_SelectedDistance].DataX * dblRateX);
	nDrawSY = (int)(m_PointData[m_SelectedDistance].DataY * dblRateY);
	nDrawEX = (int)(m_PointData[m_SelectedDistance + 1].DataX * dblRateX);
	nDrawEY = (int)(m_PointData[m_SelectedDistance + 1].DataY * dblRateY);

	// 線
	pOldPen = pDC->SelectObject(&cLLinePen);
	pDC->MoveTo(nDrawSX, nDrawSY);
	pDC->LineTo(nDrawEX, nDrawEY);

	// 始点
	pDC->SelectObject(&cSLinePen);
	DrawPointCross(pDC, nDrawSX, nDrawSY);

	// 終点
	pDC->SelectObject(&cELinePen);
	DrawPointCross(pDC, nDrawEX, nDrawEY);


	pDC->SelectObject(pOldPen);
}
// 2009.10.14 bagus Distance 追加 --}--

// 2013.01.09 bagus -->
void CNanoPointGraph::SetOriginPointData(long x, long y)
{
	PointData tData;

	tData.DataX = x;
	tData.DataY = y;
	tData.Status = NOT_MEASURED;
	tData.NeedRedraw = TRUE;

	m_OriginPointData = tData;
}
// 2013.01.09 bagus <--
