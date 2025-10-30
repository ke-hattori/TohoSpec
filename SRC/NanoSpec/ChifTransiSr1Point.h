#if !defined(AFX_CHIFTRANSISR1POINT_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
#define AFX_CHIFTRANSISR1POINT_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChifTransiSr1Point.h : ヘッダー ファイル
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です
#include "DdeClient.hxx"

class CSitePtSeq;

//--------------------------------------------------------------------------//
//	状態定義
//--------------------------------------------------------------------------//
#define	_ST_SR1P_DISP(x)	_T(#x)
#define	ST_SR1P_DISP(x)	_ST_SR1P_DISP(x)
#define	ST_SR1P_(x)		ST_SR1P_##x
// 状態の名称を定義→
//		ここを追加削除した場合は ST_xxx_n の番号に対応した
//		下記の ST_xxx_n と ST_xxx_DISP_n も合わせてください．
#define	ST_SR1P_0		ST_SR1P_(INIT)			// 初期状態
#define	ST_SR1P_1		ST_SR1P_(IDLE)			// アイドル
// 2009.05.11 K.Matsuo SitePR -->
#define	ST_SR1P_2		ST_SR1P_(SITEPR_START)					// SitePR 開始処理前							(WAIT 状態無し)
#define	ST_SR1P_3		ST_SR1P_(SITEPR_AUTO)					// SitePR オート完了待ち
#define	ST_SR1P_4		ST_SR1P_(SITEPR_AUTO_OK)				// SitePR オート OK 完了						(WAIT 状態無し)
#define	ST_SR1P_5		ST_SR1P_(SITEPR_AUTO_PROCEED)			// SitePR オート失敗のまま続行					(WAIT 状態無し)
#define	ST_SR1P_6		ST_SR1P_(SITEPR_AUTO_PROCEED_PRESTEP)	// SitePR オート失敗のまま続行（前ステップ位置）(WAIT 状態無し)
#define	ST_SR1P_7		ST_SR1P_(SITEPR_AUTO_CANCEL)			// SitePR オートレシピ終了						(WAIT 状態無し)
#define	ST_SR1P_8		ST_SR1P_(SITEPR_MANU)					// SitePR マニュアル完了待ち
#define	ST_SR1P_9		ST_SR1P_(SITEPR_MANU_OK)				// SitePR マニュアル OK 完了					(WAIT 状態無し)
#define	ST_SR1P_10		ST_SR1P_(SITEPR_MANU_CANCEL)			// SitePR マニュアルレシピ終了					(WAIT 状態無し)
#define	ST_SR1P_11		ST_SR1P_(SITEPR_PREP_MEAS)				// SitePR 終了 → 測定前の準備処理				(WAIT 状態無し)
// 2009.05.11 K.Matsuo SitePR <--
#define	ST_SR1P_12		ST_SR1P_(ZMOVE)			// 測定ポイントＺ軸移動完了待ち	(WAIT 状態無し)
#define	ST_SR1P_13		ST_SR1P_(AF)			// AF 完了待ち					(WAIT 状態無しの場合有り)
#define	ST_SR1P_14		ST_SR1P_(AF_DONE)		// AF 完了						(WAIT 状態無し)
#define	ST_SR1P_15		ST_SR1P_(MEAS)			// 測定完了待ち					(WAIT 状態無し)
#define	ST_SR1P_16		ST_SR1P_(ANA)			// 測定データ処理完了待ち
#define	ST_SR1P_17		ST_SR1P_(ANA_DONE)		// 測定データ処理完了			(WAIT 状態無し)
//2009.10.28 bagus 2point-distance --{--
#define ST_SR1P_18		ST_SR1P_(DISTANCE)			// 2点間距離測定
#define ST_SR1P_19		ST_SR1P_(DISTANCE_CANCEL)	// 2点間距離測定キャンセル
//2009.10.28 bagus 2point-distance --}--
//2009.10.31 bagus MS --{--
#define ST_SR1P_20		ST_SR1P_(MS)				// 顕微鏡測定
#define ST_SR1P_21		ST_SR1P_(MS_CANCEL)			// 顕微鏡測定
#define ST_SR1P_22		ST_SR1P_(MS_DONE)			// 顕微鏡測定
//2009.10.31 bagus MS --}--
// 状態の名称を定義←
enum STATES_SR_1POINT
{
	// 構築時は０になっています．
	// ので，最初は０として定義された状態です．
	// しかし，０の状態に対応する関数は自動的に実行されませんので，
	// 必要があれば InitInstance() などから実行する必要があります．

	// ここを変更した場合は直後にあるステータス文字列の定義も合わせてください．

	ST_SR1P_0 = 0,
	ST_SR1P_1,
// 2009.05.11 K.Matsuo SitePR -->
	ST_SR1P_2,
	ST_SR1P_3,
	ST_SR1P_4,
	ST_SR1P_5,
	ST_SR1P_6,
	ST_SR1P_7,
	ST_SR1P_8,
	ST_SR1P_9,
	ST_SR1P_10,
	ST_SR1P_11,
// 2009.05.11 K.Matsuo SitePR <--
	ST_SR1P_12,
	ST_SR1P_13,
	ST_SR1P_14,
	ST_SR1P_15,
	ST_SR1P_16,
	ST_SR1P_17,
// 2009.10.28 bagus 2point-distance --{--
	ST_SR1P_18,
	ST_SR1P_19,
// 2009.10.28 bagus 2point-distance --}--
// 2009.11.03 bagus MS --{--
	ST_SR1P_20,
	ST_SR1P_21,
	ST_SR1P_22,
// 2009.11.03 bagus MS --}--
};
const LPSTR STATES_SR_1POINT_DISP[] = {
	ST_SR1P_DISP(ST_SR1P_0),
	ST_SR1P_DISP(ST_SR1P_1),
// 2009.05.11 K.Matsuo SitePR -->
	ST_SR1P_DISP(ST_SR1P_2),
	ST_SR1P_DISP(ST_SR1P_3),
	ST_SR1P_DISP(ST_SR1P_4),
	ST_SR1P_DISP(ST_SR1P_5),
	ST_SR1P_DISP(ST_SR1P_6),
	ST_SR1P_DISP(ST_SR1P_7),
	ST_SR1P_DISP(ST_SR1P_8),
	ST_SR1P_DISP(ST_SR1P_9),
	ST_SR1P_DISP(ST_SR1P_10),
	ST_SR1P_DISP(ST_SR1P_11),
// 2009.05.11 K.Matsuo SitePR <--
	ST_SR1P_DISP(ST_SR1P_12),
	ST_SR1P_DISP(ST_SR1P_13),
	ST_SR1P_DISP(ST_SR1P_14),
	ST_SR1P_DISP(ST_SR1P_15),
	ST_SR1P_DISP(ST_SR1P_16),
	ST_SR1P_DISP(ST_SR1P_17),
// 2009.10.28 bagus 2point-distance --{--
	ST_SR1P_DISP(ST_SR1P_18),
	ST_SR1P_DISP(ST_SR1P_19),
// 2009.10.28 bagus 2point-distance --}--
// 2009.11.03 bagus MS --{--
	ST_SR1P_DISP(ST_SR1P_20),
	ST_SR1P_DISP(ST_SR1P_21),
	ST_SR1P_DISP(ST_SR1P_22),
// 2009.11.03 bagus MS --}--
} ;

//--------------------------------------------------------------------------//
//	イベント定義
//--------------------------------------------------------------------------//
enum EVENTS_SR_1POINT
{
	EV_SR1P_DO_INIT = 0,		// 初期化開始
	EV_SR1P_INIT_DONE,			// 初期化終了
// 2009.05.11 K.Matsuo SitePR -->
	EV_SR1P_SITEPR_NONE,		// SitePR 使用なしの場合のイベント
	EV_SR1P_SITEPR_AUTO,
	EV_SR1P_SITEPR_AUTO_OK,
	EV_SR1P_SITEPR_PREP_MEAS,
	EV_SR1P_SITEPR_AUTO_PROCEED,
	EV_SR1P_SITEPR_AUTO_PROCEED_PRESTEP,
	EV_SR1P_SITEPR_AUTO_CANCEL,
	EV_SR1P_SITEPR_AUTO_MANU,
	EV_SR1P_SITEPR_MANU_OK,
	EV_SR1P_SITEPR_MANU_CANCEL,
// 2009.05.11 K.Matsuo SitePR <--
	EV_SR1P_IDLE,				// アイドルへ
	EV_SR1P_START,				// 開始
	EV_SR1P_ZMOVE_DONE,			// 測定位置Ｚ移動完了
	EV_SR1P_ZMOVE_FAIL,			// 測定位置Ｚ移動開始失敗
	EV_SR1P_MEAS_START,			// 測定開始
	EV_SR1P_MEAS_ERR,			// 測定エラー
	EV_SR1P_ANA_ERR,			// 解析エラー
	EV_SR1P_MEAS_DONE,			// 測定完了
	EV_SR1P_AF_START,			// AF 開始
	EV_SR1P_AF_DONE,			// AF 完了
	EV_SR1P_AF_OK,				// AF OK
	EV_SR1P_AF_NG,				// AF NG
	EV_SR1P_ANA_DONE,			// データ処理完了
//2009.10.28 bagus 2point-distance
	EV_SR1P_DISTANCE,			// 2点間距離測定開始
//2009.10.28 bagus 2point-dsitance
//2009.11.03 bagus MS --{--
	EV_SR1P_MS,					// 顕微鏡測定
	EV_SR1P_MS_NO_ANA,			// 顕微鏡測定
//2009.11.03 bagus MS --}--
};

/*
 *	マクロ定義
 */
// １ポイント測定終了ステータス
#define	TR_SR1P_TRAAF_BUSY		0xffff0605	// AF トランジションがビジーでした
#define	TR_SR1P_ZMOVE_FAIL		0xffff0611	// Z 軸移動開始失敗
/* added 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- { ---------- */
#define	TR_SR1P_XYMOVE_FAIL		0xffff0612	// XY 軸移動開始失敗
/* added 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- } ---------- */
#define	TR_SR1P_MEAS_ERR		0xffff0621	// 測定エラー
#define	TR_SR1P_ANA_ERR			0xffff0622	// 解析エラー
// リファレンス測定位置

// 2009.08.18 K.Matsuo -->
typedef STAGE_COORD_XYZ 		MEASPOS;
// 2009.08.18 K.Matsuo <--

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSr1Point スレッド

class CChiefTransiSr1Point : public CWinThread, public cActive
{
	DECLARE_DYNCREATE(CChiefTransiSr1Point)
public:
	CChiefTransiSr1Point(CWnd* pParent = NULL);	// 動的生成に使用されるプロテクト コンストラクタ
protected:

// アトリビュート
public:

// オペレーション
public:
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
	long m_lZ_AF;
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
/* added 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- { ---------- */
	BOOL IsPointManualAdjust(DWORD dwPointNo);
/* added 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- } ---------- */
	DWORD m_dwHeartBeat;
	void HeartBeatPulse(void);
	void CancelSr1Point(void);
// 2009.05.11 K.Matsuo SitePR -->
	BOOL IsCurrLensEqualsRecipeLens(void);
// 2009.05.11 K.Matsuo SitePR <--
	MEASPOS m_MeasPosXYZ;	// 測定位置 記憶
	MEASPOS m_MeasPosbufXYZ;	// 測定位置 記憶前バッファ
	//Saiki 20090724 Add ----->
	DWORD m_dwAFValid; //Privateより移行
	//Saiki 20090724 Add <-----
	void TransiEvent(const int iEventNo, const cEventParams* const pEvParams = 0);
	int GetCurrentState(void);
	bool IsIdle();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefTransiSr1Point)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CChiefTransiSr1Point();

	// 状態別の処理関数
	void enter_ST_SR1P_INIT(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_IDLE(const cEventParams* const pEvParams = 0);
// 2009.05.11 K.Matsuo SitePR -->
	void enter_ST_SR1P_SITEPR_START(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_AUTO(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_AUTO_OK(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_AUTO_PROCEED(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_AUTO_PROCEED_PRESTEP(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_AUTO_CANCEL(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_MANU(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_MANU_OK(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_MANU_CANCEL(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_SITEPR_PREP_MEAS(const cEventParams* const pEvParams = 0);
// 2009.05.11 K.Matsuo SitePR <--
	void enter_ST_SR1P_ZMOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_AF(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_AF_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_MEAS(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_ANA(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_ANA_DONE(const cEventParams* const pEvParams = 0);
//2009.10.28 bagus 2point-distance --{--
	void enter_ST_SR1P_DISTANCE(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_DISTANCE_CANCEL(const cEventParams* const pEvParams = 0);
//2009.10.28 bagus 2point-distance --}--
//2009.11.03 bagus MS --{--
	void enter_ST_SR1P_MS(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_MS_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SR1P_MS_CANCEL(const cEventParams* const pEvParams = 0);
//2009.11.03 bagus MS --}--

	DECLARE_STATEPROC_MAP(CChiefTransiSr1Point);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefTransiSr1Point)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	afx_msg void OnTransiEvent(WPARAM wparam, LPARAM lparam);
	afx_msg void OnHeartBeat(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
private:
/* added 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- { ---------- */
	MEASMODE_ID m_enMeasMode;
/* added 2009.09.10 hmenjo 透過率 Seq リファレンスモード(17) ---------- } ---------- */
	BOOL m_bCancelReq;
	void GetMeasPosXYZ(MEASPOS *pMeasPosXYZ);
/* deleted 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- { ---------- */
//	BOOL IsPointManualAdjust(DWORD dwPointNo);
/* deleted 2009.09.01 hmenjo SE ヘッドオフセット対応(10) ---------- } ---------- */
	//Saiki 20090724 Delete ----->
	//DWORD m_dwAFValid; //Publicへ移行
	//Saiki 20090724 Delete <-----
	void NotifyComplete(const cEventParams *const pEvParams);
	DWORD m_dwPointNo;
	DWORD m_dwParentTransi;
	CView* m_pcChiefView;
	CEvent m_cSyncEvent;
	CDdeClient m_xmpDde;
// 2009.05.16 myanagida -->
	CSitePtSeq* m_pSitePtSeq;
// 2009.05.16 myanagida <--
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHIFTRANSISR1POINT_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
