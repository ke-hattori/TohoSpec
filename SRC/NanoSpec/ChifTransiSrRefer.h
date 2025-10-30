#if !defined(AFX_CHIFTRANSISRREFER_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
#define AFX_CHIFTRANSISRREFER_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChifTransiSrRefer.h : ヘッダー ファイル
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です

//--------------------------------------------------------------------------//
//	状態定義
//--------------------------------------------------------------------------//
#define	_ST_SREF_DISP(x)	_T(#x)
#define	ST_SREF_DISP(x)	_ST_SREF_DISP(x)
#define	ST_SREF_(x)		ST_SREF_##x
// 状態の名称を定義→
//		ここを追加削除した場合は ST_xxx_n の番号に対応した
//		下記の ST_xxx_n と ST_xxx_DISP_n も合わせてください．
// 2013.11.07 Bagus Add (TohoSpec対応) -->
#if 0
// #define	ST_SREF_0		ST_SREF_(INIT)			// 初期状態
// #define	ST_SREF_1		ST_SREF_(IDLE)			// アイドル
// #define	ST_SREF_2		ST_SREF_(N_MOVE)		// SR リファレンス位置へ移動完了待ち	(WAIT 状態無しの場合有り)
// #define	ST_SREF_3		ST_SREF_(N_ZMOVE)		// SR リファレンス位置Ｚへ移動完了待ち	(WAIT 状態無し)
// #define	ST_SREF_4		ST_SREF_(N_ZMOVE_DONE)	// SR リファレンス位置Ｚへ移動完了		(WAIT 状態無し)
// #define	ST_SREF_5		ST_SREF_(AF)			// AF 完了待ち							(WAIT 状態無しの場合有り)
// #define	ST_SREF_6		ST_SREF_(AF_DONE)		// AF 完了								(WAIT 状態無し)
// #define	ST_SREF_7		ST_SREF_(N_MEAS)		// SR リファレンス測定完了待ち			(WAIT 状態無し)
// #define	ST_SREF_8		ST_SREF_(D_MOVE)		// SR ダーク位置へ移動完了待ち			(WAIT 状態無しの場合有り)
// #define	ST_SREF_9		ST_SREF_(D_ZMOVE)		// SR ダーク位置Ｚへ移動完了待ち		(WAIT 状態無し)
// #define	ST_SREF_10		ST_SREF_(D_MEAS)		// SR ダーク測定完了待ち				(WAIT 状態無し)
#else
#define	ST_SREF_0		ST_SREF_(INIT)					// 初期状態
#define	ST_SREF_1		ST_SREF_(IDLE)					// アイドル
#define	ST_SREF_2		ST_SREF_(N_PRECHECK_MOVE)		//
#define	ST_SREF_3		ST_SREF_(NO_REF)				//
#define	ST_SREF_4		ST_SREF_(NO_REF_DONE)			//
#define	ST_SREF_5		ST_SREF_(N_PRE_CONFIRM_EXPIRE)	//
#define	ST_SREF_6		ST_SREF_(N_CONFIRM_EXPIRE)		//
#define	ST_SREF_7		ST_SREF_(N_EXPIRE_YES)			//
#define	ST_SREF_8		ST_SREF_(N_EXPIRE_NO)			//
#define	ST_SREF_9		ST_SREF_(N_PRE_CONFIRM_MOVE)	//
#define	ST_SREF_10		ST_SREF_(N_CONFIRM_MOVE)		//
#define	ST_SREF_11		ST_SREF_(N_MOVE_OK)				//
#define	ST_SREF_12		ST_SREF_(N_MOVE_CANCEL)			//
#define	ST_SREF_13		ST_SREF_(N_MOVE)				// SR リファレンス位置へ移動完了待ち	(WAIT 状態無しの場合有り)
#define	ST_SREF_14		ST_SREF_(N_ZMOVE)				// SR リファレンス位置Ｚへ移動完了待ち	(WAIT 状態無し)
#define	ST_SREF_15		ST_SREF_(N_ZMOVE_DONE)			// SR リファレンス位置Ｚへ移動完了		(WAIT 状態無し)
#define	ST_SREF_16		ST_SREF_(AF)					// AF 完了待ち							(WAIT 状態無しの場合有り)
#define	ST_SREF_17		ST_SREF_(AF_DONE)				// AF 完了								(WAIT 状態無し)
#define	ST_SREF_18		ST_SREF_(N_MEAS)				// SR リファレンス測定完了待ち			(WAIT 状態無し)
#define	ST_SREF_19		ST_SREF_(D_PRECHECK_MOVE)		//
#define	ST_SREF_20		ST_SREF_(D_CONFIRM_MOVE)		//
#define	ST_SREF_21		ST_SREF_(D_MOVE_OK)				//
#define	ST_SREF_22		ST_SREF_(D_MOVE_CANCEL)			//
#define	ST_SREF_23		ST_SREF_(D_MOVE)				// SR ダーク位置へ移動完了待ち			(WAIT 状態無しの場合有り)
#define	ST_SREF_24		ST_SREF_(D_ZMOVE)				// SR ダーク位置Ｚへ移動完了待ち		(WAIT 状態無し)
#define	ST_SREF_25		ST_SREF_(D_MEAS)				// SR ダーク測定完了待ち				(WAIT 状態無し)
// bagus add 2014.01.10 (Stage None対応) -->
#define	ST_SREF_26		ST_SREF_(STD_ZMOVE)				// SR リファレンス位置Ｚへ移動完了待ち	(WAIT 状態無し)
#define	ST_SREF_27		ST_SREF_(STD_ZMOVE_DONE)		// SR リファレンス位置Ｚへ移動完了		(WAIT 状態無し)
#define	ST_SREF_28		ST_SREF_(STD_AF)				// AF 完了待ち							(WAIT 状態無しの場合有り)
#define	ST_SREF_29		ST_SREF_(STD_AF_DONE)			// AF 完了								(WAIT 状態無し)
#define	ST_SREF_30		ST_SREF_(STD_MEAS)				// SR リファレンス測定完了待ち			(WAIT 状態無し)
#define	ST_SREF_31		ST_SREF_(DRK_ZMOVE)				// SR ダーク位置Ｚへ移動完了待ち		(WAIT 状態無し)
#define	ST_SREF_32		ST_SREF_(DRK_MEAS)				// SR ダーク測定完了待ち				(WAIT 状態無し)
// bagus add 2014.01.10 (Stage None対応) <--
#endif
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// 状態の名称を定義←
enum STATES_SR_REFER
{
	// 構築時は０になっています．
	// ので，最初は０として定義された状態です．
	// しかし，０の状態に対応する関数は自動的に実行されませんので，
	// 必要があれば InitInstance() などから実行する必要があります．

	// ここを変更した場合は直後にあるステータス文字列の定義も合わせてください．

	ST_SREF_0 = 0,
	ST_SREF_1,
	ST_SREF_2,
	ST_SREF_3,
	ST_SREF_4,
	ST_SREF_5,
	ST_SREF_6,
	ST_SREF_7,
	ST_SREF_8,
	ST_SREF_9,
	ST_SREF_10,
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	ST_SREF_11,
	ST_SREF_12,
	ST_SREF_13,
	ST_SREF_14,
	ST_SREF_15,
	ST_SREF_16,
	ST_SREF_17,
	ST_SREF_18,
	ST_SREF_19,
	ST_SREF_20,
	ST_SREF_21,
	ST_SREF_22,
	ST_SREF_23,
	ST_SREF_24,
	ST_SREF_25,
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// bagus add 2014.01.10 (Stage None対応) -->
	ST_SREF_26,
	ST_SREF_27,
	ST_SREF_28,
	ST_SREF_29,
	ST_SREF_30,
	ST_SREF_31,
	ST_SREF_32,
// bagus add 2014.01.10 (Stage None対応) <--
};
const LPSTR STATES_SR_REFER_DISP[] = {
	ST_SREF_DISP(ST_SREF_0),
	ST_SREF_DISP(ST_SREF_1),
	ST_SREF_DISP(ST_SREF_2),
	ST_SREF_DISP(ST_SREF_3),
	ST_SREF_DISP(ST_SREF_4),
	ST_SREF_DISP(ST_SREF_5),
	ST_SREF_DISP(ST_SREF_6),
	ST_SREF_DISP(ST_SREF_7),
	ST_SREF_DISP(ST_SREF_8),
	ST_SREF_DISP(ST_SREF_9),
	ST_SREF_DISP(ST_SREF_10),
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	ST_SREF_DISP(ST_SREF_11),
	ST_SREF_DISP(ST_SREF_12),
	ST_SREF_DISP(ST_SREF_13),
	ST_SREF_DISP(ST_SREF_14),
	ST_SREF_DISP(ST_SREF_15),
	ST_SREF_DISP(ST_SREF_16),
	ST_SREF_DISP(ST_SREF_17),
	ST_SREF_DISP(ST_SREF_18),
	ST_SREF_DISP(ST_SREF_19),
	ST_SREF_DISP(ST_SREF_20),
	ST_SREF_DISP(ST_SREF_21),
	ST_SREF_DISP(ST_SREF_22),
	ST_SREF_DISP(ST_SREF_23),
	ST_SREF_DISP(ST_SREF_24),
	ST_SREF_DISP(ST_SREF_25),
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// bagus add 2014.01.10 (Stage None対応) -->
	ST_SREF_DISP(ST_SREF_26),
	ST_SREF_DISP(ST_SREF_27),
	ST_SREF_DISP(ST_SREF_28),
	ST_SREF_DISP(ST_SREF_29),
	ST_SREF_DISP(ST_SREF_30),
	ST_SREF_DISP(ST_SREF_31),
	ST_SREF_DISP(ST_SREF_32),
// bagus add 2014.01.10 (Stage None対応) <--
} ;

//--------------------------------------------------------------------------//
//	イベント定義
//--------------------------------------------------------------------------//
enum EVENTS_SR_REFER
{
	EV_SREF_DO_INIT = 0,		// 初期化開始
	EV_SREF_INIT_DONE,			// 初期化終了
	EV_SREF_IDLE,				// アイドルへ
	EV_SREF_START,				// 開始指令
// bagus add 2014.01.10 (Stage None対応) -->
	EV_SREF_STD_START,			// 開始指令（スタンダード）
	EV_SREF_DRK_START,			// 開始指令（ダーク）
// bagus add 2014.01.10 (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	EV_SREF_N_PRECHECK_MOVE_NO_CONFIRM,
	EV_SREF_N_SIMULATION,
	EV_SREF_N_PRECHECK_NO_REF,
	EV_SREF_N_PRECHECK_NEED_CONFIRM_EXPIRE,
	EV_SREF_N_PRECHECK_NEED_CONFIRM_MOVE,
	EV_SREF_N_SHOW_INFO_DLG_DONE,
	EV_SREF_N_CONFIRM_EXPIRE_YES,
	EV_SREF_N_CONFIRM_EXPIRE_NO,
	EV_SREF_N_CONFIRM_NEXT,
	EV_SREF_N_CONFIRM_MOVE_OK,
	EV_SREF_N_CONFIRM_MOVE_CANCEL,
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	EV_SREF_N_MOVE_DONE,		// リファレンス位置移動完了
	EV_SREF_N_MOVE_FAIL,		// リファレンス位置移動開始失敗
	EV_SREF_N_ZMOVE_DONE,		// リファレンス位置Ｚ移動完了
	EV_SREF_N_ZMOVE_FAIL,		// リファレンス位置Ｚ移動開始失敗
	EV_SREF_AF,					// AF 有り
	EV_SREF_NOAF,				// AF 無し
	EV_SREF_AF_DONE,			// AF 完了
	EV_SREF_AF_OK,				// AF OK
	EV_SREF_AF_NG,				// AF NG
	EV_SREF_N_MEAS_DONE,		// リファレンス測定完了
	EV_SREF_N_MEAS_ERR,			// リファレンス測定異常終了
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	EV_SREF_D_PRECHECK_MOVE_NO_CONFIRM,
	EV_SREF_D_SIMULATION,
	EV_SREF_D_PRECHECK_MOVE_CONFIRM_NEED,
	EV_SREF_D_CONFIRM_NEXT,
	EV_SREF_D_CONFIRM_MOVE_OK,
	EV_SREF_D_CONFIRM_MOVE_CANCEL,
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	EV_SREF_D_MOVE_DONE,		// ダークリファレンス位置移動完了
	EV_SREF_D_MOVE_FAIL,		// ダークリファレンス位置移動完了
	EV_SREF_D_ZMOVE_DONE,		// ダークリファレンス位置Ｚ移動完了
	EV_SREF_D_ZMOVE_FAIL,		// ダークリファレンス位置Ｚ移動完了
	EV_SREF_D_MEAS_DONE,		// ダークリファレンス測定完了
	EV_SREF_D_MEAS_ERR,			// ダークリファレンス測定異常終了
// bagus add 2014.01.10 (Stage None対応) -->
	EV_SREF_STD_SIMULATION,		// 
	EV_SREF_STD_ZMOVE_DONE,		// リファレンス位置Ｚ移動完了
	EV_SREF_STD_ZMOVE_FAIL,		// リファレンス位置Ｚ移動開始失敗
	EV_SREF_STD_AF,				// AF 有り
	EV_SREF_STD_NOAF,			// AF 無し
	EV_SREF_STD_AF_DONE,		// AF 完了
	EV_SREF_STD_AF_OK,			// AF OK
	EV_SREF_STD_AF_NG,			// AF NG
	EV_SREF_STD_MEAS_DONE,		// リファレンス測定完了
	EV_SREF_STD_MEAS_ERR,		// リファレンス測定異常終了
	EV_SREF_DRK_SIMULATION,		// 
	EV_SREF_DRK_ZMOVE_DONE,		// ダークリファレンス位置Ｚ移動完了
	EV_SREF_DRK_ZMOVE_FAIL,		// ダークリファレンス位置Ｚ移動完了
	EV_SREF_DRK_MEAS_DONE,		// ダークリファレンス測定完了
	EV_SREF_DRK_MEAS_ERR,		// ダークリファレンス測定異常終了
// bagus add 2014.01.10 (Stage None対応) <--
};

/*
 *	マクロ定義
 */
// SR リファレンス終了ステータス
#define	TR_SREF_OK					0			// OK
#define	TR_SREF_TRAAF_BUSY			0xffff0305	// AF トランジションがビジーでした
#define	TR_SREF_N_MOVESTART_FAIL	0xffff0311	// リファレンス測定位置へ移動開始失敗
#define	TR_SREF_N_ZMOVESTART_FAIL	0xffff0312	// リファレンス測定位置Ｚへ移動開始失敗
#define	TR_SREF_N_1STMEAS_FAIL		0xffff0321	// ファースト リファレンス測定 失敗
#define	TR_SREF_N_2NDMEAS_T1_FAIL	0xffff0322	// セカンド リファレンス測定 T1 失敗
#define	TR_SREF_N_2NDMEAS_T2_FAIL	0xffff0323	// セカンド リファレンス測定 T2 失敗
#define	TR_SREF_D_MOVESTART_FAIL	0xffff0331	// ダークリファレンス測定位置へ移動開始失敗
#define	TR_SREF_D_ZMOVESTART_FAIL	0xffff0332	// ダークリファレンス測定位置へ移動開始失敗
#define	TR_SREF_D_MEAS_T1_FAIL		0xffff0341	// ダークリファレンス測定 T1 失敗
#define	TR_SREF_D_MEAS_T2_FAIL		0xffff0342	// ダークリファレンス測定 T2 失敗
#define	TR_SREF_REFDATA_WR_ERR		0xffff0351	// リファレンスデータ書込み失敗
#define	TR_SREF_REFDATAT1_WR_ERR	0xffff0352	// リファレンスデータ T1 書込み失敗
#define	TR_SREF_REFDATAT2_WR_ERR	0xffff0353	// リファレンスデータ T2 書込み失敗
#define	TR_SREF_AF_NG				0xffff0361	// AF NG
#define	TR_SREF_CANCEL				0xffff0371	// キャンセル
// リファレンス測定位置
typedef struct {
	long	lX;
	long	lY;
	long	lZ;
} REFPOS;

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSrRefer スレッド

class CChiefTransiSrRefer : public CWinThread, public cActive
{
	DECLARE_DYNCREATE(CChiefTransiSrRefer)
public:
	CChiefTransiSrRefer(CWnd* pParent = NULL);	// 動的生成に使用されるプロテクト コンストラクタ
protected:

// アトリビュート
public:

// オペレーション
public:
	DWORD m_dwHeartBeat;
	void HeartBeatPulse(void);
	void CancelSrRefer(void);
	void TransiEvent(const int iEventNo, const cEventParams* const pEvParams = 0);
	int GetCurrentState(void);
	bool IsIdle();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefTransiSrRefer)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CChiefTransiSrRefer();

	// 状態別の処理関数
	void enter_ST_SREF_INIT(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_IDLE(const cEventParams* const pEvParams = 0);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	void enter_ST_SREF_N_PRECHECK_MOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_NO_REF(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_NO_REF_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_PRE_CONFIRM_EXPIRE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_CONFIRM_EXPIRE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_EXPIRE_YES(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_EXPIRE_NO(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_PRE_CONFIRM_MOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_CONFIRM_MOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_MOVE_OK(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_MOVE_CANCEL(const cEventParams* const pEvParams = 0);
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	void enter_ST_SREF_N_MOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_ZMOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_ZMOVE_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_AF(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_AF_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_N_MEAS(const cEventParams* const pEvParams = 0);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	void enter_ST_SREF_D_PRECHECK_MOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_D_CONFIRM_MOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_D_MOVE_OK(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_D_MOVE_CANCEL(const cEventParams* const pEvParams = 0);
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	void enter_ST_SREF_D_MOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_D_ZMOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_D_MEAS(const cEventParams* const pEvParams = 0);
// bagus add 2014.01.10 (Stage None対応) -->
	void enter_ST_SREF_STD_ZMOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_STD_ZMOVE_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_STD_AF(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_STD_AF_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_STD_MEAS(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_DRK_ZMOVE(const cEventParams* const pEvParams = 0);
	void enter_ST_SREF_DRK_MEAS(const cEventParams* const pEvParams = 0);
// bagus add 2014.01.10 (Stage None対応) <--

	DECLARE_STATEPROC_MAP(CChiefTransiSrRefer);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefTransiSrRefer)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	afx_msg void OnTransiEvent(WPARAM wparam, LPARAM lparam);
	afx_msg void OnHeartBeat(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
private:
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	WORD m_wHeadPre;
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
	BOOL m_bCancelReq;
	DWORD m_dwAFValid;
	void NotifyComplete(const cEventParams *const pEvParams);
	WORD m_wScanType;
	REFPOS m_DRefPos;
	REFPOS m_NRefPos;
	BOOL m_bRef2ndMeasure;
	double m_dLifeTime;
	BOOL m_bWarnFlag;
	BOOL m_bMeasRef;
	DWORD m_dwRefCounter;
	DWORD m_dwParentTransi;
	CView* m_pcChiefView;
	CEvent m_cSyncEvent;
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
	BOOL m_bDoRefMeas;
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHIFTRANSISRREFER_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
