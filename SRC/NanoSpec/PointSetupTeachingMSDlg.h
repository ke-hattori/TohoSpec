#if !defined(AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_)
#define AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupTeachingMSDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingMSDlg �_�C�A���O

class CPointSetupTeachingMSDlg : public COverlayDialog
{
// �R���X�g���N�V����
public:
	CPointSetupTeachingMSDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	int	m_iTeachingMode;			// ����/�蓮���[�h

public:
	typedef enum _TEACHING_MODE
	{
		MODE_AUTO_MEAS = 0,
		MODE_MANUAL_MEAS,
		MODE_MAX,
	} TEACHING_MODE;


// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPointSetupTeachingMSDlg)
	enum { IDD = IDD_POINT_SETUP_TEACHING_MS_DLG };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

public:
	CShadeButtonST m_NextPointButton;

	int m_iCurrPoint;							// ���ݐݒ肵�悤�Ƃ��Ă���|�C���g
	BOOL m_bMoveNext;							// MoveNext�����t���O

	long m_lX;
	long m_lY;
	long m_lZ;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;		// STAGE_PROG_INFO_HDR�\����

	int m_iProcessStatusOld;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPointSetupTeachingMSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPointSetupTeachingMSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHomePositionButton();
	afx_msg void OnSelendokMicroScopeFocusPosition();
	afx_msg void OnNextPointButton();
	//}}AFX_MSG
// 2009.11.09 bagus MS �C�� --{--
	afx_msg LRESULT OnNexResponse(WPARAM wparam, LPARAM lparam);
// 2009.11.09 bagus MS �C�� --}--
	DECLARE_MESSAGE_MAP()

	virtual void ButtonEnableCheck(void);
private:
	void InitCombo_Lens();
	void SoftJoyStickXyzEnable(BOOL bEnable);

	MS_CONFIG m_MsConfig;

// 2009.11.09 bagus MS �C�� --{--
	int		m_nSelLens;
	BOOL	m_bMSModeLampFlicker;
// 2009.11.09 bagus MS �C�� --}--
// matsuhisa 2009.12.26 added >>>
	int		m_nLensSensorItem;
// matsuhisa 2009.12.26 added <<<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_)
