
/*-------------------------------------------------------------------
 *	Chief トランジション関数ポインタ 定義ファイル
 *-----------------------------------------------------------------*/

#ifdef CHIEF_PFUNCS_MAS
	#define	CHIFPF_EXT
#else
	#define	CHIFPF_EXT	extern
#endif

#ifdef CHIEF_PFUNCS_MAS
/*	IsIdle() -------------------------------------------------- */
static bool PFC_IsIdleSeq(CChiefView* pcChiefView)
	{return ((CChiefTransiSeq*) pcChiefView->m_pcChiefTransiSeq)->IsIdle();}
static bool PFC_IsIdleStress(CChiefView* pcChiefView)
	{return ((CChiefTransiStress*) pcChiefView->m_pcChiefTransiStress)->IsIdle();}
/*	TransiEvent() --------------------------------------------- */
static void PFC_TransiEventSeq(CChiefView* pcChiefView, const int iEventNo, const cEventParams *const pEvParams)
	{((CChiefTransiSeq*) pcChiefView->m_pcChiefTransiSeq)->TransiEvent(iEventNo, pEvParams);}
static void PFC_TransiEventStress(CChiefView* pcChiefView, const int iEventNo, const cEventParams *const pEvParams)
	{((CChiefTransiStress*) pcChiefView->m_pcChiefTransiStress)->TransiEvent(iEventNo, pEvParams);}
/*	GetCurrentState() ----------------------------------------- */
static int PFC_GetCurrentStateSeq(CChiefView* pcChiefView)
	{return ((CChiefTransiSeq*) pcChiefView->m_pcChiefTransiSeq)->GetCurrentState();}
static int PFC_GetCurrentStateStress(CChiefView* pcChiefView)
	{return ((CChiefTransiStress*) pcChiefView->m_pcChiefTransiStress)->GetCurrentState();}
/*	IsPauseSeq() ---------------------------------------------- */
static bool PFC_IsPauseSeqSeq(CChiefView* pcChiefView)
	{return ((CChiefTransiSeq*) pcChiefView->m_pcChiefTransiSeq)->IsPauseSeq();}
static bool PFC_IsPauseSeqStress(CChiefView* pcChiefView)
	{return ((CChiefTransiStress*) pcChiefView->m_pcChiefTransiStress)->IsPauseSeq();}
#endif

/* アクセス用構造体	*/
typedef struct {
	bool	(*IsIdle)(CChiefView*);
	void	(*TransiEvent)(CChiefView*, const int, const cEventParams *const);
	int		(*GetCurrentState)(CChiefView*);
	bool	(*IsPauseSeq)(CChiefView*);
	struct {
		int		iPAUSE;
		int		iRESUME;
		int		iCANCEL;
		int		iSTART;
	} Event;
	struct {
		int		iPAUSE;
	} State;
} CHIEF_PFUNCS, *LPCHIEF_PFUNCS;

/*
 *	設定関数
 *		関数ポインタとパラメタを設定します．
 */
CHIFPF_EXT BOOL PFC_FuncSet(CChiefView* pcChiefView, LPCHIEF_PFUNCS pChiefPFuncs, LPWORD pwHeadType)
#ifndef CHIEF_PFUNCS_MAS
;
#else
{
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) pcChiefView->m_ChiefRecipes.pMainRcpInfo;
	if (0 == l_pMainRcpInfo) {
		if (0 != pwHeadType) {*pwHeadType = -1;}
		return FALSE;	/* メインレシピ未読込み	*/
	}
	if (0 != pwHeadType) {*pwHeadType = l_pMainRcpInfo->MainRcpParam.hdr.wHeadType;}
	switch (l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
	case HEAD_TYPE_SR:
/* added 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- { ---------- */
	case HEAD_TYPE_SE:
/* added 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- } ---------- */
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	case HEAD_TYPE_CTA:
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq CTAILPI チェック ---------- { ---------- */
	case HEAD_TYPE_4PP:
/* added 2009.11.06 K.Matsuo RS Seq CTAILPI チェック ---------- } ---------- */
// 2009.11.03 bagus MS 顕微鏡もSRの流れでおこなう --{--
	case HEAD_TYPE_MS:
// 2009.11.03 bagus MS	--}--
		pChiefPFuncs->IsIdle = PFC_IsIdleSeq;
		pChiefPFuncs->TransiEvent = PFC_TransiEventSeq;
		pChiefPFuncs->GetCurrentState = PFC_GetCurrentStateSeq;
		pChiefPFuncs->IsPauseSeq = PFC_IsPauseSeqSeq;
		pChiefPFuncs->Event.iPAUSE = EV_SEQ_PAUSE;
		pChiefPFuncs->Event.iRESUME = EV_SEQ_RESUME;
		pChiefPFuncs->Event.iCANCEL = EV_SEQ_CANCEL;
		pChiefPFuncs->Event.iSTART = EV_SEQ_DO_MEAS;
		pChiefPFuncs->State.iPAUSE = ST_SEQ_PAUSE;
		break;
	case HEAD_TYPE_STRESS:
		pChiefPFuncs->IsIdle = PFC_IsIdleStress;
		pChiefPFuncs->TransiEvent = PFC_TransiEventStress;
		pChiefPFuncs->GetCurrentState = PFC_GetCurrentStateStress;
		pChiefPFuncs->IsPauseSeq = PFC_IsPauseSeqStress;
		pChiefPFuncs->Event.iPAUSE = EV_STRS_PAUSE;
		pChiefPFuncs->Event.iRESUME = EV_STRS_RESUME;
		pChiefPFuncs->Event.iCANCEL = EV_STRS_CANCEL;
		pChiefPFuncs->Event.iSTART = EV_STRS_START;
		pChiefPFuncs->State.iPAUSE = ST_STRS_PAUSE;
		break;
/* deleted 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- { ---------- */
//	case HEAD_TYPE_SE:
/* deleted 2009.08.26 hmenjo SE Seq 起動条件追加 ---------- } ---------- */
// 2009.10.19 bagus MS 追加 --{--
//	case HEAD_TYPE_IRSE:
// 2009.10.19 bagus MS 追加 --}--
/* deleted 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
//	case HEAD_TYPE_CTA:
/* deleted 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
// 2009.10.19 bagus MS 追加 --{--
// 2009.11.03 bagus MS 削除 --{--
//	case HEAD_TYPE_MS:
// 2009.11.03 bagus MS 削除 --}--
// 2009.10.19 bagus MS 追加 --}--
	default:
		return FALSE;	/* ヘッドタイプ異常	*/
		break;
	}

	return TRUE;
}
#endif

/*
 *	アイドル状態をチェックします．
 */
CHIFPF_EXT bool PFC_IsIdleAll(CChiefView* pcChiefView)
#ifndef CHIEF_PFUNCS_MAS
;
#else
{
	if (false == PFC_IsIdleSeq(pcChiefView)) {
		return false;
	}
	if (false == PFC_IsIdleStress(pcChiefView)) {
		return false;
	}

	return true;
}
#endif
