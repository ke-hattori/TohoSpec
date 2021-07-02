#if !defined( _OVERLAYDIALOG_H_ )
#define _OVERLAYDIALOG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BtnST.h"
#include "ColorStatic.h"
#include "System.h"
#include "Misc.h"
#include "MainFrm.h"
#include "LensComboBox.h"
#include "MotionComboBox.h"
// 2010.01.21 bagus Gantry --{--
#include "ScanDataGraphDlg.h"
// 2010.01.21 bagus Gantry --}--

#ifdef REFRESH_TIMER_ID
	#undef REFRESH_TIMER_ID
#endif
#define REFRESH_TIMER_ID		(1)
#define OVERLAY_BUTTON_ENABLECHANGE_TIMER	(2)
#define CHECK_ABORT_TIMER		(3)


#ifdef REFRESH_TIMER_ELAPSE
	#undef REFRESH_TIMER_ELAPSE
#endif
#define REFRESH_TIMER_ELAPSE	(100L)

#ifdef OVERLAY_VIEW_OFFSET_X
	#undef OVERLAY_VIEW_OFFSET_X
#endif
#define OVERLAY_VIEW_OFFSET_X	(10)

#ifdef OVERLAY_VIEW_OFFSET_Y
	#undef OVERLAY_VIEW_OFFSET_Y
#endif
#define OVERLAY_VIEW_OFFSET_Y	(39)

#ifdef STAGE_MAP_OFFSET_X
	#undef STAGE_MAP_OFFSET_X
#endif
#define STAGE_MAP_OFFSET_X		(20)

#ifdef STAGE_MAP_OFFSET_Y
	#undef STAGE_MAP_OFFSET_Y
#endif
#define STAGE_MAP_OFFSET_Y		(20)

#define SAMPLE_MAX		100 				// 暫定で１００にしておく

// ##########################################################################
// class : COverlayDialog
// ##########################################################################
class COverlayStatic : public CStatic
{
public:
	COverlayStatic();

protected:
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
private:
	RECT m_rcOverlay;
};

// ##########################################################################
// class : COverlayDialog
// ##########################################################################
class COverlayDialog : public CDialog
{
public:
	COverlayDialog(UINT uResourceID = IDD_OVERLAY_DLG, CWnd* pParent = NULL);

	long m_lX, m_lY, m_lZ;
// 2013.01.22 bagus stage origin offset -->
	long m_lTeachPosX, m_lTeachPosY; // fot StageConfig stage offset
// 2013.01.22 bagus stage origin offset <--
	BOOL m_StagePositionSetting;
// 2009.11.16 K.Matsuo ヘッドオフセット対応 -->
	BOOL m_bOriginalPositionSetting;
// 2009.11.16 K.Matsuo ヘッドオフセット対応 <--
	double m_dXPixelToMicron, m_dYPixelToMicron;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	BOOL m_bProgress1PMeas;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

	template<class T>
	void GetTeachPos(T* pPos) { ::CopyMemory(pPos, &m_CurrPos, sizeof(T)); }
	template<class T>
	BOOL IsTeachPosValid(T* pPos) { return memcmp(pPos, &m_CurrPos, sizeof(T)) == 0; }

	void SetCaption(LPCTSTR pszCaption);
	void SetMessage(LPCTSTR pszMessage);
	void SetFileName(LPCTSTR pszFileName);
	void SetOperationLog(LPCTSTR pszLog);
	void SetSampleMode(int nSampleMode)
	{ m_nSampleMode = nSampleMode;
		return;
	}

	BOOL m_joyContinue;				// TRUEの場合は、前の状態を引き継ぐ

// 2010.01.07 bagus Gantry --{--
	BOOL	m_bGantry;
// 2010.01.07 bagus Gantry --}--
// 2010.01.13 bagus Gantry --{--
	BOOL	m_bUseCheckScan;
// 2010.01.13 bagus Gantry --}--

protected:

	//{{AFX_DATA(COverlayDialog)
	enum { IDD = IDD_OVERLAY_DLG };
	CStatic					m_ctlMessage;
	COverlayStatic			m_stcOverlay;
	CShadeButtonST			m_btnOk;
	CShadeButtonST			m_btnCancel;
	CShadeButtonST			m_btnAutoFocus;
	CShadeButtonST			m_btnCapture;
	CShadeButtonST			m_btnSpecifyCoord;
	CShadeButtonST			m_btnJogSetting;
// 2010.01.06 bagus Gantry --{--
	CShadeButtonST			m_btnCheckScan;
// 2010.01.06 bagus Gantry --}--
	CMotionButton			m_XY_UpButton;
	CMotionButton			m_XY_DownButton;
	CMotionButton			m_XY_RightButton;
	CMotionButton			m_XY_LeftButton;
	CMotionButton			m_Z_UpButton;
	CMotionButton			m_Z_DownButton;
//Saiki 20091009 Add ----->
	CMotionButton			m_Z_HomePosButton;
//Saiki 20091009 Add <-----
	CTypeComboBox			m_cbHeadType;
	CLensComboBox			m_cbLens;
	CCameraComboBox			m_cbCamera;
	CMotionComboBox			m_cbMotionStage;
	CMotionElevComboBox		m_cbMotionElev;
	CJoyStickRadioButton*	m_joyStick1;
	CJoyStickRadioButton*	m_joyStick2;
// 2013.02.22 bagus Substrate thickness setting -->
	CSubstrateThicknessComboBox		m_cbSubThick;
// 2013.02.22 bagus Substrate thickness setting <--
	//}}AFX_DATA

	CShadeButtonST m_btnStageMove;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverlayDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(COverlayDialog)
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAutoFocus();
	afx_msg void OnCapture();
	afx_msg void OnSpecifyCoord();
	afx_msg void OnJogSetting();
/* deleted 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
//	afx_msg void OnJoyStick();
/* deleted 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- } ---------- */
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
// INS 2009.07.06 by kawamura
	afx_msg void OnStageMove();
// INS 2009.07.06 by kawamura
// 2010.01.07 bagus Gantry --{--
	afx_msg void OnCheckScan();
	afx_msg void OnSelchangeSrOptFilter();
	afx_msg void OnCheckTransmittanceShutter();
// 2010.01.07 bagus Gantry --}--
//Saiki 20091016 Add ---->
	afx_msg void OnHomePositionButton();
//Saiki 20091016 Add <----
// 2010.01.29 bagus Gantry --{--
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
// 2010.01.29 bagus Gantry --}--
	//}}AFX_MSG
// 2009.05.28 K.Matsuo SPT -->
	LRESULT OnKeyHook(WPARAM, LPARAM);
// 2009.05.28 K.Matsuo SPT <--
	LRESULT OnDoStageMove(WPARAM, LPARAM);
	LRESULT OnStageMoveDone(WPARAM, LPARAM);
	LRESULT OnDoElevatorMove(WPARAM, LPARAM);
/* added 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
	LRESULT OnJoyStick(WPARAM, LPARAM);
/* added 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- } ---------- */

	DECLARE_MESSAGE_MAP()

	//2010.01.07 bagus 拡張ウィンドウスタイルをセットできるように修正 --{--
	//void CreateDialogItem(LPCTSTR lpClassName, LPCTSTR lpszCaption, DWORD dwStyle, RECT rect, UINT nID);
	void CreateDialogItem(LPCTSTR lpClassName, LPCTSTR lpszCaption, DWORD dwStyle, RECT rect, UINT nID,DWORD dwStyleEx = 0);
	//2010.01.07 bagus --}--
	void ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color);
	void DrawLocationMarker(CONST POINT* lpPoint, BOOL bDrawColor = TRUE);
	void SetupStageMap(void);
	void SetupSampleMap(void);
// 2013.01.09 bagus -->
//	void DrawSampleMap(void);
	void DrawSampleMap(long lMapDrawSampleOriginCoordX, long lMapDrawSampleOriginCoordY);
// 2013.01.09 bagus -->
	void DrawMarker(void);
	void DrawMarkerInWaferColor();
	void DrawCenterMarker(LPCRECT lpRect, CDC* pDC);
	void DrawGridLine(WORD wAxis, int iInc, int iInterval, LPCRECT lpRect, CDC* pDC);
	void DrawDashLine(short Axis, int Start, short Inc, short Length, int Bound, int OtherAxisVal, int iEmptySpace, CDC *pDC );
	void DrawOneLine(short Axis, int Start, int End, int OtherAxisVal, CDC *pDC);
	void UpdateStagePosMarker(void);
// 2009.05.27 K.Matsuo ダブルバッファリング -->
	void CreateStageMapBackBuffer();
// 2009.05.27 K.Matsuo ダブルバッファリング <--

	void ClientToStage(LPPOINT lpPoint, STAGE_COORD* pStage);
// 2013.01.09 bagus -->
	void PixelToMicron(LPPOINT lpPoint, STAGE_COORD* pStage);
// 2013.01.09 bagus <--
	void StageToClient(STAGE_COORD* pStage, LPPOINT lpPoint);
	void MicronToPixel(STAGE_COORD* pLogical, LPPOINT lpPoint);
	virtual void Refresh(void);
	virtual void ButtonEnableCheck(void);
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 -->
	void JoyStickEnableCheck(void);
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 <--
// 2013.01.09 bagus -->
	void ToClientXyStageCoord(STAGE_COORD* lpPoint);
	void ToXyAxisCoord(STAGE_COORD* lpPoint);
// 2013.01.09 bagus <--
// 2013.01.17 bagus rotate xy view -->
	long RotateXyAxisDir(long lDir);
// 2013.01.17 bagus rotate xy view <--

	BOOL OnLButtonDown_CheckIL();

	inline void SetValidBit(BOOL bOn)	{ m_btnOk.EnableWindow(bOn);		}
	inline void SetCancelBit(BOOL bOn) { m_btnCancel.EnableWindow(bOn); }

// 2010.01.07 bagus Gantry Transmittance --{--
	void InitCombo_SrOpticalFilter();
// 2010.01.07 bagus Gantry Transmittance --}--

	// ----------------------------------------------------------------------
	//
	inline int GetMotionJog(WORD wAxis, UINT nMode)
	{
		if(m_SystemConfig.nStageType == STAGE_TYPE_SPT){
			STAGE_MOTION_SPEED_DATA StageMotionSpeedData[MAX_AXIS];
			ConfigFile_GetNanoSpecIni(&StageMotionSpeedData[0], CONFIG_FILE_STAGE_MOTION_SPEED);

			if(nMode >= STAGE_MOTION_JOG_LOW){
				return StageMotionSpeedData[wAxis].Jog;
			}else{
				return StageMotionSpeedData[wAxis].Speed[nMode];
			}
		}else{
			STAGE_MOTION_JOG_DATA StageMotionJogData[MAX_AXIS];
			ConfigFile_GetNanoSpecIni(&StageMotionJogData[0], CONFIG_FILE_STAGE_MOTION_JOG);

			return StageMotionJogData[wAxis].Jog[nMode];
		}
	}

	// ----------------------------------------------------------------------
	//
	inline double GetMotionElevSpeed(WORD wAxis, UINT nElevMode)
	{
		double* pdZSpeed;
		StageGetElevatorSpeed(&pdZSpeed);
		return *(pdZSpeed + nElevMode);
	}

	CBrush m_Brush_MessageLabel;
	CBrush m_Brush_Coord_X;
	CBrush m_Brush_Coord_Y;
	CBrush m_Brush_Coord_Z;
	CBrush m_Brush_Sample_X;
	CBrush m_Brush_Sample_Y;
	CBrush m_Brush_Sample_Z;
	CBrush m_Brush_StagePosition_X;
	CBrush m_Brush_StagePosition_Y;
	STAGE_COORD m_TeachPos;
	STAGE_COORD m_OriginPos;
	CRect m_rcOverlay;
	CRect m_rcClient;
	CRect m_rcStage;

	SR_VIDEO_CALIB m_SrVideoCalib[SR_LENS_MAX];

	double m_dStageMaxSizeX, m_dStageMaxSizeY;
	STAGE_COORD m_StageOriginCoord;
	double m_dStageSizeX, m_dStageSizeY;
	STAGE_COORD m_SampleOriginCoord;
	double m_dSampleSizeX, m_dSampleSizeY;
//	double x_um_pixs, y_um_pixs;		// map stage coordinates to pixels
	double x_pixs_um, y_pixs_um;		// map pixels to stage ums

	int m_iMaxSizeX, m_iMaxSizeY;		// Stage Max Size
	int m_iStageSizeX, m_iStageSizeY;	// Stage Size

	STAGE_MOTION_JOG_DATA m_StageMotionJogData[MAX_AXIS];
	STAGE_MOTION_SPEED_DATA m_StageMotionSpeedData[MAX_AXIS];
// 2009.05.28 K.Matsuo SPT -->
	BOOL m_bIsMoveAtSpeed;
// 2009.05.28 K.Matsuo SPT <--

	CPoint m_ptStageMarker, m_ptOldStageMarker;
	CDC m_memStageMap;
	CDC m_memGridLine;
	CBitmap m_bmStageMap;
	CBitmap m_bmGridLine;
	int m_iOrgPos;

	COLORREF m_crBkColor;
	CLogFile m_logFile;

	STAGE_COORD_XYZ m_CurrPos;
	STAGE_COORD_XYZ m_CurrSamplePos;
	STAGE_COORD_XYZ m_old_CurrPos;
	STAGE_COORD_XYZ m_old_CurrSamplePos;
// 2013.01.22 bagus stage origin offset -->
	STAGE_COORD_XYZ m_CurrTeachPos;
// 2013.01.22 bagus stage origin offset <--

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	BYTE		m_XY_ButtonUseHistry;		// XY Button Use ?
	BYTE		m_Z_ButtonUseHistry;		// Z Button Use ?
	BYTE		m_AutoFocusUseHistry;		// AutoFocus Use ?
	BYTE		m_AutoFocusResult;			// AutoFocus Result OK ? or NG ?

	CString m_sCaption;
	CString m_sMessage;
	CString m_sFileName;

private:
	void DrawStageView(void);
	void GetSampleData(void);
	void DrawGrid(void);
	BOOL CheckStageMoveEnable(STAGE_COORD* pos);
	CRect m_dStageMaxMap;
	CRect m_dStageMap;

	CBrush* m_oldBrush;
	CPen* m_oldPen;
	long m_lDrawStartStagePosX;
	long m_lDrawEndStagePosX;
	long m_lDrawStartStagePosY;
	long m_lDrawEndStagePosY;
	long m_lDrawStartSamplePosX;
	long m_lDrawEndSamplePosX;
	long m_lDrawStartSamplePosY;
	long m_lDrawEndSamplePosY;

	CPoint	m_lCenterPos;					// オフセットした中心座標（対ステージ）
	CPoint	m_lAbsolutePos; 				// オフセットした中心座標（対ウィンド）

	STAGE_CONFIG m_StageConfig;
	SYSTEM_CONFIG m_SystemConfig;

	int		m_nSampleMode;					// 0:指定なし, 1:指定あり
	int		m_iSampleMax;
	char	m_sSampleName[SAMPLE_MAX][RECIPE_NAME_LEN + 1];

// 2009.05.26 K.Matsuo ダブルバッファリング -->
	CDC* m_bbDC;
	CBitmap* m_bbBitmap;
// 2009.05.26 K.Matsuo ダブルバッファリング <--
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	STAGE_COORD_XYZ m_HeadOffset;
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */

// 2010.01.21 bagus Gantry --{--
	CScanDataGraphDlg	m_ScanDataGraphDlg;
// 2010.01.21 bagus Gantry --{--

// 2013.01.11 bagus jog direction -->
	JOG_DIR_SETTING m_JogDirSetting;
// 2013.01.11 bagus jog direction -->
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined( _OVERLAYDIALOG_H_ )
