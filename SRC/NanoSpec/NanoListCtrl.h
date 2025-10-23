#if !defined(AFX_CNANOLISTCTRL_H__28E6B317_D5D0_4C55_B164_5A7FE4CBBAA0__INCLUDED_)
#define AFX_CNANOLISTCTRL_H__28E6B317_D5D0_4C55_B164_5A7FE4CBBAA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NanoListCtrl.h : �w�b�_�[ �t�@�C��
//

// ##########################################################################
// CNanoListCtrl �E�B���h�E
// ##########################################################################

class CNanoListCtrl : public CListCtrl
{
// �R���X�g���N�V����
public:
	CNanoListCtrl();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	//{{AFX_VIRTUAL(CNanoListCtrl)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CNanoListCtrl();
	void SetViewStyle();

protected:
	//{{AFX_MSG(CNanoListCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void FontCreation();

	CFont m_listFont;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CNANOLISTCTRL_H__28E6B317_D5D0_4C55_B164_5A7FE4CBBAA0__INCLUDED_)
