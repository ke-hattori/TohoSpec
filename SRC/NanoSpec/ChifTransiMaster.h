#if !defined(AFX_CHIFTRANSIMASTER_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
#define AFX_CHIFTRANSIMASTER_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChifTransiMaster.h : ヘッダー ファイル
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です

//--------------------------------------------------------------------------//
//	状態定義
//--------------------------------------------------------------------------//
#define	_ST_MAS_DISP(x)	_T(#x)
#define	ST_MAS_DISP(x)	_ST_MAS_DISP(x)
#define	ST_MAS_(x)		ST_MAS_##x
// 状態の名称を定義→
//		ここを追加削除した場合は ST_xxx_n の番号に対応した
//		下記の ST_xxx_n と ST_xxx_DISP_n も合わせてください．
#define	ST_MAS_0		ST_MAS_(INIT)			// 初期状態
#define	ST_MAS_1		ST_MAS_(IDLE)			// アイドル
#define	ST_MAS_2		ST_MAS_(UISEQ)			// 画面 SEQ 測定						(WAIT 状態無しの場合有り)
#define	ST_MAS_3		ST_MAS_(UISEQ_DONE)		// 画面 SEQ 測定完了					(WAIT 状態無し)
#define	ST_MAS_4		ST_MAS_(UI1POINT)		// 画面１ポイント測定					(WAIT 状態無しの場合有り)
#define	ST_MAS_5		ST_MAS_(UI1POINT_DONE)	// 画面１ポイント測定完了				(WAIT 状態無し)
#define	ST_MAS_6		ST_MAS_(UIAAF)			// 画面 AF								(WAIT 状態無しの場合有り)
#define	ST_MAS_7		ST_MAS_(UIAAF_DONE)		// 画面 AF 完了							(WAIT 状態無し)
#define	ST_MAS_8		ST_MAS_(PIFSEQ)			// Pif SEQ 測定
#define	ST_MAS_9		ST_MAS_(PIFSEQ_DONE)	// Pif SEQ 測定完了						(WAIT 状態無し)
#define	ST_MAS_10		ST_MAS_(UISREF)			// 画面リファレンス測定					(WAIT 状態無しの場合有り)
#define	ST_MAS_11		ST_MAS_(UISREF_DONE)	// 画面リファレンス測定完了				(WAIT 状態無し)
#define	ST_MAS_12		ST_MAS_(END)			// トランジション終了
// bagus add 2014.01.10 (Stage None対応) -->
#define	ST_MAS_13		ST_MAS_(STDREF)			// スタンダードリファレンス測定
#define	ST_MAS_14		ST_MAS_(STDREF_DONE)	// スタンダードリファレンス測定完了
#define	ST_MAS_15		ST_MAS_(DRKREF)			// ダークリファレンス測定
#define	ST_MAS_16		ST_MAS_(DRKREF_DONE)	// ダークリファレンス測定完了
// bagus add 2014.01.10 (Stage None対応) <--
// 状態の名称を定義←
enum STATES_MASTER
{
	// 構築時は０になっています．
	// ので，最初は０として定義された状態です．
	// しかし，０の状態に対応する関数は自動的に実行されませんので，
	// 必要があれば InitInstance() などから実行する必要があります．

	// ここを変更した場合は直後にあるステータス文字列の定義も合わせてください．

	ST_MAS_0 = 0,
	ST_MAS_1,
	ST_MAS_2,
	ST_MAS_3,
	ST_MAS_4,
	ST_MAS_5,
	ST_MAS_6,
	ST_MAS_7,
	ST_MAS_8,
	ST_MAS_9,
	ST_MAS_10,
	ST_MAS_11,
	ST_MAS_12,
// bagus add 2014.01.10 (Stage None対応) -->
	ST_MAS_13,
	ST_MAS_14,
	ST_MAS_15,
	ST_MAS_16,
// bagus add 2014.01.10 (Stage None対応) <--
};
const LPSTR STATES_MASTER_DISP[] = {
	ST_MAS_DISP(ST_MAS_0),
	ST_MAS_DISP(ST_MAS_1),
	ST_MAS_DISP(ST_MAS_2),
	ST_MAS_DISP(ST_MAS_3),
	ST_MAS_DISP(ST_MAS_4),
	ST_MAS_DISP(ST_MAS_5),
	ST_MAS_DISP(ST_MAS_6),
	ST_MAS_DISP(ST_MAS_7),
	ST_MAS_DISP(ST_MAS_8),
	ST_MAS_DISP(ST_MAS_9),
	ST_MAS_DISP(ST_MAS_10),
	ST_MAS_DISP(ST_MAS_11),
	ST_MAS_DISP(ST_MAS_12),
// bagus add 2014.01.10 (Stage None対応) -->
	ST_MAS_DISP(ST_MAS_13),
	ST_MAS_DISP(ST_MAS_14),
	ST_MAS_DISP(ST_MAS_15),
	ST_MAS_DISP(ST_MAS_16),
// bagus add 2014.01.10 (Stage None対応) <--
};

//--------------------------------------------------------------------------//
//	イベント定義
//--------------------------------------------------------------------------//
enum EVENTS_MASTER
{

	EV_MAS_DO_INIT = 0,		// 初期化開始
	EV_MAS_INIT_DONE,		// 初期化終了
	EV_MAS_IDLE,			// アイドルへ
	EV_MAS_END,				// トランジション終了
	EV_MAS_DO_UISEQ,		// 画面 SEQ 測定開始
	EV_MAS_SEQ_DONE,		// SEQ 測定終了
	EV_MAS_DO_UI1POINT,		// 画面１ポイント測定開始
	EV_MAS_UI1POINT_DONE,	// 画面１ポイント測定終了
	EV_MAS_DO_UIAAF,		// 画面 AF 開始
	EV_MAS_UIAAF_DONE,		// 画面 AF 終了
	EV_MAS_DO_PIFSEQ,		// Pif SEQ 測定開始
	EV_MAS_DO_UISREF,		// 画面リファレンス測定開始
	EV_MAS_UISREF_DONE,		// 画面リファレンス測定終了
// bagus add 2014.01.10 (Stage None対応) -->
	EV_MAS_DO_STDREF,		// スタンダードリファレンス測定開始
	EV_MAS_STDREF_DONE,		// スタンダードリファレンス測定終了
	EV_MAS_DO_DRKREF,		// ダークリファレンス測定開始
	EV_MAS_DRKREF_DONE,		// ダークリファレンス測定終了
// bagus add 2014.01.10 (Stage None対応) <--
};

/*
 *	マクロ定義
 */
// マスタ終了ステータス
#define	TR_MAS_TRSEQ_BUSY		0xffff0102	// シーケンス測定 トランジションがビジーでした
#define	TR_MAS_TRSREF_BUSY		0xffff0103	// リファレンス測定 トランジションがビジーでした
#define	TR_MAS_TRAAF_BUSY		0xffff0105	// AF トランジションがビジーでした
#define	TR_MAS_TRSR1P_BUSY		0xffff0106	// １ポイント測定 トランジションがビジーでした
/* added 2009.07.31 hmenjo ストレス機能追加(10) ---------- { ---------- */
#define	TR_MAS_TRSTRS_BUSY		0xffff0107	// シーケンス測定 トランジションがビジーでした
#define	TR_MAS_INVALID_HEAD		0xffff0111	/* 無効なヘッドタイプです．	*/
/* added 2009.07.31 hmenjo ストレス機能追加(10) ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiMaster スレッド

class CChiefTransiMaster : public CWinThread, public cActive
{
	DECLARE_DYNCREATE(CChiefTransiMaster)
public:
	CChiefTransiMaster(CWnd* pParent = NULL);	// 動的生成に使用されるプロテクト コンストラクタ
protected:

// アトリビュート
public:

// オペレーション
public:
	DWORD m_dwHeartBeat;
	void HeartBeatPulse(void);
	void TransiEvent(const int iEventNo, const cEventParams* const pEvParams = 0);
	int GetCurrentState(void);
	bool IsIdle();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefTransiMaster)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CChiefTransiMaster();

	// 状態別の処理関数
	void enter_ST_MAS_INIT(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_IDLE(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_UISEQ(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_UISEQ_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_UI1POINT(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_UI1POINT_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_UIAAF(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_UIAAF_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_PIFSEQ(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_PIFSEQ_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_UISREF(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_UISREF_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_END(const cEventParams* const pEvParams = 0);
// bagus add 2014.01.10 (Stage None対応) -->
	void enter_ST_MAS_STDREF(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_STDREF_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_DRKREF(const cEventParams* const pEvParams = 0);
	void enter_ST_MAS_DRKREF_DONE(const cEventParams* const pEvParams = 0);
// bagus add 2014.01.10 (Stage None対応) <--

	DECLARE_STATEPROC_MAP(CChiefTransiMaster);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefTransiMaster)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	afx_msg void OnTransiEvent(WPARAM wparam, LPARAM lparam);
	afx_msg void OnHeartBeat(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
private:
/* added 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
	BOOL m_bGTR1PRefer;
/* added 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */
/* added 2009.09.11 hmenjo 画面へガントリ透過率 Refer 結果通知 ---------- { ---------- */
	BOOL m_bReferMode;
/* added 2009.09.11 hmenjo 画面へガントリ透過率 Refer 結果通知 ---------- } ---------- */
	CView* m_pcChiefView;
	CEvent m_cSyncEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHIFTRANSIMASTER_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
