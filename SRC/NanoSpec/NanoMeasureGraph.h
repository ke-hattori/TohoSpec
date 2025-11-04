#if !defined(AFX_NANOMEASUREGRAPH_H__75636F0B_F7E0_4CF3_A378_FA6AC8D9C5C8__INCLUDED_)
#define AFX_NANOMEASUREGRAPH_H__75636F0B_F7E0_4CF3_A378_FA6AC8D9C5C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NanoMeasureGraph.h : �w�b�_�[ �t�@�C��
//
#include <afxwin.h> 		// MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h> 		// MFC �̊g������
#include <afxdisp.h>		// MFC �̃I�[�g���[�V���� �N���X
#include <afxtempl.h>

//debug
#include <math.h>

#include "NanoLineSelect.h"

#define NANO_MEASURE_MAX_GRAPH_LINE	(10)
#define NANO_MAX_TITLE_LEN			(100)
#define NANO_MAX_LINENAME_LEN		(50)
//2009.09.10 bagus stress --{--
//#define NANO_MEASURE_MAX_POINT_DATA	(1024)//(5000)
//�X�g���X�̃f�[�^��1���C������1000�𒴂��Ă��܂��̂Ŋg������
#define NANO_MEASURE_MAX_POINT_DATA	(5000)
//2009.09.10 bagus stress --}--
#define NANO_CALIBRATION_LINE_MAX	(10)

/////////////////////////////////////////////////////////////////////////////
// CNanoMeasureGraph �E�B���h�E

class CNanoMeasureGraph : public CButton
{
public:

	typedef struct {
		long X_Origin;		//X���J�n�l
		long Y_Origin;		//Y���J�n�u
		long X_Step;		//X���f�[�^�Ԋu
		long Y_Step;		//Y���f�[�^�Ԋu�i�_����Y�����\���Ԋu�j
		long X_Max;			//�g��Ȃ�
		long Y_Max;			//Y���ő�l
		int  PointNum;		//����_��
		char UpperTitle[NANO_MAX_TITLE_LEN+1];
		char LowerTitle[NANO_MAX_TITLE_LEN+1];
		char X_AxisTitle[NANO_MAX_TITLE_LEN+1];
		char Y_AxisTitle[NANO_MAX_TITLE_LEN+1];
		//��ʏ㕔�̐��̕\��/��\����؂�ւ���`�F�b�N�{�b�N�X���g�p���邩�ǂ���
		BOOL UseLineSelect;
		//�L�����u���[�V�����֌W
		BOOL UseCalibration;		//�L�����u���[�V�����@�\���g�p���邩�ǂ���
		int  CalibrationLineNum;	//�L�����u���[�V�����̐����ő�l
		//�F�ݒ�
		COLORREF CanvasBackColor;	//�`��G���A�S�̂̔w�i�F
		COLORREF GraphBackColor;	//�O���t�̕����̔w�i�F
		COLORREF GraphBorderColor;	//�O���t�̘g���̐F
		COLORREF GraphStepLineColor;//�O���t���̓_���̐F
		COLORREF TitleTextColor;	//�^�C�g���̐F
		COLORREF UnitTextColor;		//���̃e�L�X�g�F
//		COLORREF EdgeColor;
		COLORREF SelectingCalibrationColor;		//�L�����u���[�V�����F �m��O
		COLORREF SelectedCalibrationColor;		//�L�����u���[�V�����F �m���
		//
		int		 CanvasMargin;					//�]��(pixel)
	} GraphConfig;

	//�e�f�[�^�̐��̐ݒ�
	typedef struct {
		COLORREF Color;										//���̐F
		int		PenStyle;									//����
		BOOL	UseFlag;									//TRUE:�g�p����,FALSE:�g�p���Ȃ�
		BOOL	Visible;									//�����p�r�i���ݕ\������Ă��邩�ۂ��j
		int		DataIndex;									//���Ԗڂ̃f�[�^���g����
		char	LineName[NANO_MAX_LINENAME_LEN+1];			//���̖��O
	} LineConfig;

	//�L�����u���[�V�����f�[�^
	typedef struct {
		int		DataNum;									//�m�肵���L�����u���[�V������
		int		PointIndex[NANO_CALIBRATION_LINE_MAX];		//���W�f�[�^��X�� (X������)
	} CalibrationData;


// �R���X�g���N�V����
public:
	CNanoMeasureGraph();

// �A�g���r���[�g
public:
// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CNanoMeasureGraph)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	CalibrationData GetCalibrationData();
	void MoveCalibrationLine(int nStep);
	virtual BOOL  PreTranslateMessage(MSG *pMsg);
	void Initialize();
	void AddLineData(int nLineNo,double dXValue,double dYValue);//2008-10-10OkabeAdd
	void AddLineData(int nLineNo,double value);
	void AddLineDataAll(int nLineNo,double *pValues,int nPointNum);
	BOOL SetGraphConfig(GraphConfig tConfig);
	BOOL GetGraphConfig(GraphConfig* pConfig);
	BOOL SetLineConfig(int nIndex, LineConfig tConfig);
	BOOL GetLineConfig(int nIndex, LineConfig *pConfig);
	void ClearAllData();
	void ClearGraphData();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual ~CNanoMeasureGraph();

	void ResetCalibrationData();

	//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
	void SetAutoScaleMode(BOOL bMode);
	BOOL GetAutoScaleMode();
	//2009.09.03 bagus se �I�[�g�X�P�[�� --}--

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CNanoMeasureGraph)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	afx_msg LRESULT OnLineSelectChange(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsValidLineNo(int nLineNo);
	//�����p�r
	typedef struct {
		RECT GraphArea;
		double RateX;
		double RateY;
		double DivX;
	} DrawGraphInfo;

	//2008-10-10OkabeAdd X�AY��doublu�l�O���t�p�r
	typedef struct {
		double dXValue;
		double dYValue;
	} GraphXYDataValue;

	void InitBackBuffer();
	void DrawItemMain(LPDRAWITEMSTRUCT lpDIS);
	SIZE GetTextSize(CDC *pDC,CFont *pFont,CString strText);
	int ArrangeLineSelect(CRect cArea);
	void DrawEdge(CDC *pDC,RECT *pArea);
	void DrawAxisTitleX(CDC *pDC, CString strTitle, RECT *pArea);
	void DrawAxisTitleY(CDC *pDC,CString strTitle,RECT *pArea);
	void DrawTitle(CDC *pDC,CString strTitle,RECT *pArea);
	void ClearGraph(CDC *pDC,RECT *pScreenSize);
	void AddCalibrationData(int nPointIndex);
	void ChangeCalibrationData(int nPointIndex);
	void DrawGraph(CDC *pDC,RECT *pArea);
	void CutGraphArea(CDC *pDC, RECT *pArea, RECT *pCutArea);
	//Drawing Colors
	LineConfig m_LineConfig[NANO_MEASURE_MAX_GRAPH_LINE];

	//Drawing Font
	CFont*	m_pTitleFont;
	CFont*	m_pAxisXFont;
	CFont*	m_pAxisYFont;
	CFont*	m_pMemFont;
	//Graph Config
	GraphConfig		m_Config;
	//CArray<MeasureValue_t,MeasureValue_t&> m_LineData[NANO_MEASURE_MAX_GRAPH_LINE];
	double m_LineData[NANO_MEASURE_MAX_GRAPH_LINE][NANO_MEASURE_MAX_POINT_DATA];		//�����l
	//2009.09.20 bagus stress --{--
//	GraphXYDataValue m_LineXYData[1][NANO_MEASURE_MAX_POINT_DATA];		//2008-10-10OkabeAdd
	GraphXYDataValue m_LineXYData[NANO_MEASURE_MAX_GRAPH_LINE][NANO_MEASURE_MAX_POINT_DATA];		//2008-10-10OkabeAdd
	//2009.09.20 bagus stress --}--
	int	m_LinePointNum[NANO_MEASURE_MAX_GRAPH_LINE];

	//2008-10-10OkabeAdd �g�p�O���t�ؑփt���O
	//�i0:m_LineData�O���t	1:m_LineXYData�O���t�j
	int m_iGrapgSelectFlg;

	//
	CNanoLineSelect*	m_pLineSelect[NANO_MEASURE_MAX_GRAPH_LINE];
	CalibrationData		m_CalibrationData;
	BOOL				m_bNowCalibration;

	DrawGraphInfo		m_GraphInfo;

	//BackBuffer
	CDC					*m_bbDC;
	CBitmap				*m_bbBitmap;
	BOOL				m_bbOK;
	BOOL				m_bReDrawGraph;

	//2009.09.03 bagus se �I�[�g�X�P�[�� --{--
	BOOL	m_bAutoScaleMode;
	double	m_dYMin;
	double	m_dYMax;
	//2009.09.03 bagus se �I�[�g�X�P�[�� --}--
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_NANOMEASUREGRAPH_H__75636F0B_F7E0_4CF3_A378_FA6AC8D9C5C8__INCLUDED_)
