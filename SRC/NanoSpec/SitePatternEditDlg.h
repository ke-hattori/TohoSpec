#if !defined(AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_)
#define AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SitePatternEditDlg.h : �w�b�_�[ �t�@�C��
//
#include "NanoListCtrl.h"
#include "ColorStatic.h"
#include "BitmapStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CSitePatternEditDlg �_�C�A���O

class CSitePatternEditDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSitePatternEditDlg(CWnd* pParent = NULL);	 // �W���̃R���X�g���N�^
	~CSitePatternEditDlg();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSitePatternEditDlg)
	enum { IDD = IDD_SITE_PATTERN_EDIT_DLG };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

	CBitmapStatic m_stcSitePrImg;
	CShadeButtonST m_DeleteButton;
	CShadeButtonST m_DeleteAllButton;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	CNanoListCtrl m_lstCtrl;
	CListCtrl& rfListCtrl() {return m_lstCtrl;}

	int		m_iMode;
	int		m_SelectItem;
	CString m_SelectName;
	SPR_SUB_INFO m_pPatSubInfo;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSitePatternEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSitePatternEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickPatternList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickPatternList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteButton();
	afx_msg void OnDeleteAllButton();
	//}}AFX_MSG
	LRESULT OnExecutionStart(WPARAM wparam, LPARAM lparam);
	LRESULT OnExecutionEnd(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual void List_Add(const char* Name);
	virtual void SortColumn(int iCol);
	virtual void DispPattern();
	virtual void SetCursorPosition(int iItem);
	virtual void InitialDataSet();

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;


private:
	void RenumberItem();

	SR_TURRET srTurret[SR_LENS_MAX];
	BOOL	m_bSort[256];

	int m_ExecuteSeq;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_)
