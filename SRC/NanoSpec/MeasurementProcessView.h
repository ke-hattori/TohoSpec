#if !defined(AFX_MEASUREMENTPROCESSVIEW_H__CC07C5A1_5F1F_4C88_AD4C_F5A450948A84__INCLUDED_)
#define AFX_MEASUREMENTPROCESSVIEW_H__CC07C5A1_5F1F_4C88_AD4C_F5A450948A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasurementProcessView.h : �w�b�_�[ �t�@�C��
//

// ##########################################################################
// CMeasurementProcessView �t�H�[�� �r���[
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

// 2009.10.30 bagus MS �ǉ� --{--
#include "..\\..\\INC\\Globals.hxx"
// 2009.10.30 bagus MS �ǉ� --}--
#include "NanoUI.h"
// 2009.10.30 bagus MS �ǉ� --{--
#include "BtnST.h"
#include "Misc.h"
#include "MotionComboBox.h"
#include "ColorStatic.h"
// 2009.10.30 bagus MS �ǉ� --}--

// 2009.11.09 bagus MS �C�� --{--
#define CHECK_CYLINDER_TIMER		(10)
// 2009.11.09 bagus MS �C�� --}--

class CMeasurementProcessView : public CNanoUI
{
protected:
	CMeasurementProcessView();			// ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^�B
	DECLARE_DYNCREATE(CMeasurementProcessView)

// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CMeasurementProcessView)
	enum { IDD = IDD_MEASUREMENT_PROCESS_FORM };
	//}}AFX_DATA

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	CShadeButtonST m_MeasurementCancelButton;
	CShadeButtonST m_MeasurementPauseButton;
	CShadeButtonST m_RemeasurementButton;
	CShadeButtonST m_AdujstRemeasurementButton;
	CShadeButtonST m_ValuesToDisplayButton;
	CShadeButtonST m_SaveButton;
	CShadeButtonST m_PrintButton;
	CShadeButtonST m_ExitButton;
// 2009.10.30 bagus MS �ǉ� --{--
	CShadeButtonST m_NextPointButton;

	CShadeButtonST	m_PointTeachingButton;
	CMotionButton	m_XY_UpButton;
	CMotionButton	m_XY_DownButton;
	CMotionButton	m_XY_RightButton;
	CMotionButton	m_XY_LeftButton;
	CMotionComboBox m_cbMotionStage;
	CJoyStickRadioButton*	m_joyStick1;
	CJoyStickRadioButton*	m_joyStick2;
	CColorStatic m_stcMeasurementPointX;
	CColorStatic m_stcMeasurementPointY;
// 2009.10.30 bagus MS �ǉ� --}--
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	BOOL StartRemeasurement(int iPma);

// 2009.10.30 bagus MS �ǉ� --{--
	void Init_SR_XYStageMenu();
// 2009.10.30 bagus MS �ǉ� --}--

// 2009.12.17 bagus MS �C�� --{--
	void ChangeOperationEnable(BOOL bEnable);
// 2009.12.17 bagus MS �C�� --}--

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMeasurementProcessView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CMeasurementProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMeasurementProcessView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMeasurementCancelButton();
	afx_msg void OnMeasurementPauseButton();
	afx_msg void OnExitButton();
	afx_msg void OnRemeasurementButton();
	afx_msg void OnSaveButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAdujstRemeasurementButton();
	afx_msg void OnNextPointButton();
	afx_msg void OnPointTeachingButton();
	afx_msg void OnDestroy();
	afx_msg void OnSelendokMicroScopeFocusPosition();
	//}}AFX_MSG

	afx_msg LRESULT OnMeaProcFuncCall(WPARAM wparam, LPARAM lparam);

// 2009.10.30 bagus MS �ǉ� --{--
	afx_msg LRESULT OnJoyStick(WPARAM, LPARAM);
// 2009.10.30 bagus MS �ǉ� --}--

// 2009.11.09 bagus MS �C�� --{--
	afx_msg void OnNexResponse(WPARAM wparam, LPARAM lparam);
// 2009.11.09 bagus MS �C�� --}--
	DECLARE_MESSAGE_MAP()

// 2009.10.30 bagus MS �ǉ� --{--
private:
	SYSTEM_CONFIG m_SystemConfig;

	void InitCombo_Lens();
// 2009.10.30 bagus MS �ǉ� --}--

// 2009.11.02 bagus MS �ǉ� --{--
	MS_CONFIG m_MsConfig;
// 2009.11.02 bagus MS �ǉ� --}--

// 2009.11.09 bagus MS �C�� --{--
	int		m_nSelLens;
	BOOL	m_bMSModeLampFilcker;
// 2009.11.09 bagus MS �C�� --}--
// matsuhisa 2009.12.26 added >>>
	int		m_nLensSensorItem;
// matsuhisa 2009.12.26 added <<<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MEASUREMENTPROCESSVIEW_H__CC07C5A1_5F1F_4C88_AD4C_F5A450948A84__INCLUDED_)
