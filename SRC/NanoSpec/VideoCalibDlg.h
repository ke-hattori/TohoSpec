#if !defined( _VIDEOCALIBDLG_H_ )
#define _VIDEOCALIBDLG_H_

#pragma once

#include "EventPar.h"
#include "OverlayDialog.h"

//===========================================================================
// class : CVideoCalibDlg
//===========================================================================

#define VIDEO_CALIB_FACTOR_DISP_DIGITS		(3)

class CVideoCalibDlg : public COverlayDialog, cActive
{
// コンストラクション
public:
	CVideoCalibDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CVideoCalibDlg)
	//}}AFX_DATA

public:
	class CComboBoxMoveExtent : public CComboBox
	{
	public:
		void PreSubclassWindow()
		{
			ResetContent();
			AddString("SLOW");
			AddString("MEDIUM-SLOW");
			AddString("MEDIUM-FAST");
			AddString("FAST");

			SetCurSel(2);
		}
	};

	CComboBoxMoveExtent m_cbMoveExtent;

// Operation
public:
	double GetXPixelToMicron() const {return m_dXPixelToMicron;}
	double GetYPixelToMicron() const {return m_dYPixelToMicron;}
	void SetXPixelToMicron(double dXPixelToMicron) {m_dXPixelToMicron = dXPixelToMicron;}
	void SetYPixelToMicron(double dYPixelToMicron) {m_dYPixelToMicron = dYPixelToMicron;}

	static double Convert_RoundForDisplay( double Val, WORD NumDecimDigts )
	{
		if( NumDecimDigts > 20 )
			return 0;

		double Fct	= pow(10, NumDecimDigts);
		long	Temp = (long) (Val * Fct + 0.5);
		return Temp / Fct;
	}

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CVideoCalibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//{{AFX_MSG(CVideoCalibDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// state processing functions
	//
	void enter_ST_START(const cEventParams* const);
	void enter_ST_STORE_FIRST_SITE(const cEventParams* const);
	void enter_ST_MOVE_STAGE(const cEventParams* const);
	void enter_ST_STORE_SECOND_SITE(const cEventParams* const);
	void enter_ST_VIDEO_CALIB_DONE(const cEventParams* const);
	DECLARE_STATEPROC_MAP(CVideoCalibDlg);

	void MoveStage();

	long m_MoveX;
	long m_MoveY;
	double m_dPosX1;
	double m_dPosY1;
	double m_dPosX2;
	double m_dPosY2;
};
#endif // !defined(_VIDEOCALIBDLG_H_)
