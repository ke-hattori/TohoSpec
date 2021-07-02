// ChiefFrame.h : CChiefFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CChiefFrame : public CFrameWnd
{
public:
	CChiefFrame();
protected: // シリアライズ機能のみから作成します。
	DECLARE_DYNCREATE(CChiefFrame)

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext );/* added 2014.11.21 hmenjo 3100 アイコン(MAIN)*/
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CChiefFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:	// コントロール バー用メンバ
	CStatusBar	m_wndStatusBar;
	CToolBar	m_wndToolBar;

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CChiefFrame)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnHeartBeat(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnNextraResponse(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDispRecvPifCmd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnUpdateStatesOnDlg(WPARAM wparam, LPARAM lparam);
// 2009.02.05 K.Matsuo delete -->
//	afx_msg LRESULT OnSendTraceData(WPARAM wparam, LPARAM lparam);
// 2009.02.05 K.Matsuo delete <--
	afx_msg LRESULT OnPifDiRefresh(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnEQRunCheck(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnReportAlarms(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMoveSetPosFromPif(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMoveSetPosFromDisp(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStageStop(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnVacuumFromDisp(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetRecipe(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnAFPopupEnd(WPARAM wparam, LPARAM lparam);
// 2009.07.29 K.Matsuo -->
	afx_msg LRESULT OnManualSitePtPopupEnd(WPARAM wparam, LPARAM lparam);
// 2009.07.29 K.Matsuo <--
	afx_msg LRESULT OnMeasRecvData(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnData1PointEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDataMeasEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStartAAF(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStartDispSeq(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStart1Point(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSeqPause(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSeqResume(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSeqCancel(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStartSrRefer(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDlgShowSW(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnResetAlarm(WPARAM wparam, LPARAM lparam);
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- { ---------- */
	afx_msg LRESULT OnMeasLineEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDataLineEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDataStressMeasEnd(WPARAM wparam, LPARAM lparam);
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- } ---------- */
//2009.10.28 bagus 2point-distance --{--
	afx_msg LRESULT OnDistancePopupEnd(WPARAM wparam, LPARAM lparam);
//2009.10.28 bagus 2point-distance --}--
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- { ---------- */
	afx_msg LRESULT OnMeasAlarmCTA(WPARAM wparam, LPARAM lparam);
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- } ---------- */
//2009.11.03 bagus MS --{--
	afx_msg LRESULT OnMSPopupEnd(WPARAM wparam, LPARAM lparam);
//2009.11.03 bagus MS --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	afx_msg LRESULT OnMeasCompEASEStatus(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMeasCompEASEError(WPARAM wparam, LPARAM lparam);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	afx_msg LRESULT OnConfirmPopupEnd(WPARAM wparam, LPARAM lparam);
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	DECLARE_MESSAGE_MAP()
};
