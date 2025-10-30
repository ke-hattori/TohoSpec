#if !defined(AFX_RECIPEMEASUREMENTSRCIEREFLECTANCEPROGRAMVIEW_H__7BAEEE30_AA9E_4DC7_BD33_C852A8B76D83__INCLUDED_)
#define AFX_RECIPEMEASUREMENTSRCIEREFLECTANCEPROGRAMVIEW_H__7BAEEE30_AA9E_4DC7_BD33_C852A8B76D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementSrChromaticityProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementSrCieReflectanceProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoRecipeUI.h"
#include "ColorStatic.h"

class CRecipeMeasurementSrCieReflectanceProgramView : public CNanoRecipeUI
{
protected:
	CRecipeMeasurementSrCieReflectanceProgramView();				// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CRecipeMeasurementSrCieReflectanceProgramView();
	DECLARE_DYNCREATE(CRecipeMeasurementSrCieReflectanceProgramView)

// フォーム データ
protected:
	//{{AFX_DATA(CRecipeMeasurementSrCieReflectanceProgramView)
	enum { IDD = IDD_RECIPE_MEASUREMENT_SR_CHROMATICITY_PROGRAM_FORM };
	//}}AFX_DATA
	CToolBar m_wndToolBar;
	CColorStatic m_stcProgName;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMeasurementSrCieReflectanceProgramView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual void LoadRecipeData();
	virtual BOOL CheckData();
	virtual BOOL IsDataChanged();
	virtual BOOL SaveRecipeData();
	virtual BOOL IsAccessPrivilege();
	virtual int SaveAsDlgInfo();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRecipeMeasurementSrCieReflectanceProgramView)
	afx_msg void OnSelchangeCieLens();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_ColorSystem();
	void InitCombo_IlluminantTable();
	void InitCombo_ReferenceMaterial();
	void InitCombo_Lens();

	MEAS_PROG_INFO m_ChroMeas;
	MEAS_PROG_INFO m_OldChroMeas;
	SR_TURRET m_SrTurret[SR_LENS_MAX];
	CString m_strLens;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTSRCIEREFLECTANCEPROGRAMVIEW_H__7BAEEE30_AA9E_4DC7_BD33_C852A8B76D83__INCLUDED_)
