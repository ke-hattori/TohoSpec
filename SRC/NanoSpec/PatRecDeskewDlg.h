#if !defined( _PATRECDESKEW_H_ )
#define _PATRECDESKEW_H_

#pragma once

#include <patrec.hxx>
#include "EventPar.h"
#include "OverlayDialog.h"

#define PAT_REC_WIDTH_MIN	24.0
#define PAT_REC_WIDTH_MAX	256.0
#define PAT_REC_HEIGHT_MIN	24.0
#define PAT_REC_HEIGHT_MAX	240.0

//===========================================================================
// class : CPatRecDeskewDlg
//===========================================================================

class CPatRecDeskewDlg : public COverlayDialog, cActive
{
// コンストラクション
public:
	CPatRecDeskewDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	CPatRecDeskewDlg(int iMode, int iLens, DESKEW_SITE* pDeskewSite, CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPatRecDeskewDlg)
	enum { IDD = IDD_OVERLAY_DLG };
	//}}AFX_DATA
	CBitmapStatic m_stcModel;
	CShadeButtonST m_btnDeskewVerify;
	CShadeButtonST m_btnOffsetApply;

// Operation
public:
	CPoint m_ptFirst, m_ptSecond;
	CRect m_rcNew, m_rcOld;		// used for delimiting the pat. rec. area

	char m_szPatternName[RECIPE_NAME_LEN + 1];
	int m_iMode;
	int m_iLens;
	DESKEW_SITE* m_pDeskewSite;
	STAGE_COORD m_OffsetPos;		// オフセット座標

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPatRecDeskewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//{{AFX_MSG(CPatRecDeskewDlg)
	afx_msg void OnOK();
	afx_msg void OnClose();
	afx_msg void OnCancel();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnVerify();
	afx_msg void OnPaint();
	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStartFiniteStateMachine();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnOffsetApply();
	//}}AFX_MSG
	LRESULT OnStageMoveDone(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	BOOL TeachPatternRec(void);
	BOOL PatRecFirstMatch(void);
	void MovePatternToCenter(void);
	BOOL PatRecSecondMatch(void);
	void UpdateRect(CPoint point);

	inline void DrawModel(void) { m_stcModel.SetBitmap(DESKEW_IMG_TEMP); }

	// state processing functions
	//
	void enter_ST_INIT				(const cEventParams* const);
	void enter_ST_START				(const cEventParams* const);
	void enter_ST_VERIFY_TEACH		(const cEventParams* const);
	void enter_ST_DRAW_START			(const cEventParams* const evp);
	void enter_ST_ERASE_OLD_DRAW_NEW (const cEventParams* const evp);
	void enter_ST_DRAW_END			(const cEventParams* const evp);
	void enter_ST_DO_PAT_REC			(const cEventParams* const);

	DECLARE_STATEPROC_MAP(CPatRecDeskewDlg);

	DESKEW_RESULT m_DeskewResult;

	SR_TURRET srTurret[SR_LENS_MAX];
	CString	m_szLens;
	STAGE_COORD m_CenterPos;		// ターゲット検出時のセンター座標
};
#endif // !defined(_PATRECDESKEW_H_)
