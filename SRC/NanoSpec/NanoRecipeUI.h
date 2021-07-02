// NanoRecipeUI.h : ヘッダー ファイル
//

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoUI.h"


// ##########################################################################
//
// ##########################################################################

class CNanoRecipeUI : public CNanoUI
{
	DECLARE_DYNAMIC(CNanoRecipeUI)

public:
	enum nOpenMode{modeNormal = 0, modeNew, modeDefaultSetting};

	CNanoRecipeUI(LPCTSTR lpszTemplateName);
	CNanoRecipeUI(UINT nIDTemplate);
	virtual ~CNanoRecipeUI();
	virtual BOOL Exit();
/* added 2016.08.12 hmenjo レシピ編集 menu バグ2 ---------- { ---------- */
	virtual BOOL IsAccessPrivilege()=0;	// 派生クラスで定義
/* added 2016.08.12 hmenjo レシピ編集 menu バグ2 ---------- } ---------- */

protected:
	virtual BOOL Save();
	virtual BOOL SaveAs();
	virtual void Discard();
	virtual BOOL CheckData()=0;			// 派生クラスで定義
	virtual BOOL IsDataChanged()=0;		// 派生クラスで定義
	virtual BOOL SaveRecipeData()=0;	// 派生クラスで定義
/* deleted 2016.08.12 hmenjo レシピ編集 menu バグ2 ---------- { ---------- */
//	virtual BOOL IsAccessPrivilege()=0;	// 派生クラスで定義
/* deleted 2016.08.12 hmenjo レシピ編集 menu バグ2 ---------- } ---------- */
	virtual int SaveAsDlgInfo()=0;		// 派生クラスで定義
/* added 2015.04.24 hmenjo Ref File 削除1 ---------- { ---------- */
	virtual void DelRefFileByUseMeasPgm(LPCTSTR ptszMeasPgmName);
/* added 2015.04.24 hmenjo Ref File 削除1 ---------- } ---------- */

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNanoRecipeUI)
	afx_msg void OnSave();
	afx_msg void OnSaveAs();
	afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	UINT m_nOpenMode;
	char m_szRecipeName[RECIPE_NAME_LEN + 1];
	CString m_strCaption;
};
