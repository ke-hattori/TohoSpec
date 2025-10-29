// SaveNameOverlapDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "nanospec.h"
#include "SaveNameOverlapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveNameOverlapDlg �_�C�A���O


CSaveNameOverlapDlg::CSaveNameOverlapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveNameOverlapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveNameOverlapDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT

	memset(m_szSpecifiedFileName, 0, sizeof(m_szSpecifiedFileName));

}


void CSaveNameOverlapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveNameOverlapDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveNameOverlapDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveNameOverlapDlg)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_SAVE_AS, OnSaveAs)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveNameOverlapDlg ���b�Z�[�W �n���h��


BOOL CSaveNameOverlapDlg::OnInitDialog()
{
	// Kojika 20090601 Add
	CString l_strBuffer;
	// Kojika 20090601 Add End

	CDialog::OnInitDialog();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	char l_szMessage[1024+1];
	memset(l_szMessage, 0, sizeof(l_szMessage));
	// Kojika 20090601 Change
/*	sprintf(l_szMessage, "%s%s", m_szSpecifiedFileName, "\n already exists, Do you want to replace it?"); */
/*	sprintf(l_szMessage, "%s%s", m_szSpecifiedFileName, "\n���̃t�@�C���͊��ɑ��݂��܂��B �㏑�����܂����H"); */
//	sprintf(l_szMessage, "%s%s", m_szSpecifiedFileName, "\n���̃t�@�C���͊��ɑ��݂��܂��B �㏑�����܂����H\n(�ۑ��ɐ��\�b������ꍇ������܂��B)");
	LoadStringML(IDS_FILE_ALREADY_EXIST, l_strBuffer, "\n already exists, Do you want to replace it?");
	sprintf(l_szMessage, "%s%s", m_szSpecifiedFileName, (LPCSTR)l_strBuffer);
	// Kojika 20090601 Change End
	GetDlgItem(IDC_SAVE_NAME_OVERLAP_TEXT)->SetWindowText(l_szMessage);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
				  // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}



void CSaveNameOverlapDlg::OnSave()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EndDialog(SAVEOVERLAP_SAVE); //�㏑���ۑ�(SAVE)
}

void CSaveNameOverlapDlg::OnCancel()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EndDialog(SAVEOVERLAP_NOSAVE); //�ۑ����Ȃ�(CANCEL)
}

void CSaveNameOverlapDlg::OnSaveAs()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EndDialog(SAVEOVERLAP_SAVEAS); //���O��t���ĕۑ�(SAVE AS)
}
