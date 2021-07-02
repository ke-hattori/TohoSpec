#pragma once

#include <math.h>

//#define MAXDATANUM						(512)
#define MAXDATANUM						  (512 * 10)
#define MAXCOEF 						  (7)		// n-1次式

typedef struct _CURVEFITTINGST{
	int datanum;
	double point[MAXDATANUM];
	double value[MAXDATANUM];
} CURVEFITTINGST, *LPCURVEFITTINGST, CCDCALIB, *LPCCDCALIB;

/* 回帰近似曲線の計算 */
	// 下記をWebなどで検索してください
	//	 最小２乗法、ｎ次多項式、ガウスの消去法
	//	 [XL97]近似曲線「多項式近似曲線」について http://support.microsoft.com/kb/405463/ja

	// ストレスでの利用（最大6次式 y = ax^6 + bx^5 + cx^4 + dx^3 + ex^2 + fx + g）
	// 7ポイントの膜厚測定結果からのマッピング


	// Sr膜厚測定での利用（最大5次式）
	// Srヘッド・ccdポジションのマッピング
	// ディテクタと使用波長間のデータの変換を行うための、変換式の構築
	// 最大5次式を作成し、その係数をm_dCoefficient(i)に代入している

class CCurveFitting
{
public:
	enum { COEF1 = 1,
		   COEF2 = 2,		// 1次回帰
		   COEF3 = 3,
		   COEF4 = 4,
		   COEF5 = 5,
		   COEF6 = 6,
		   COEF7 = 7, };

	CCurveFitting(int nMaxCoef = MAXCOEF) : m_nMaxCoef(nMaxCoef)
	{
		ASSERT(m_nMaxCoef <= MAXCOEF);

		m_nCoef = 0;
		::ZeroMemory(m_dCoefficient, sizeof(m_dCoefficient));

		matrix = NULL;
	}

	~CCurveFitting() {}

	BOOL SetParamData(const CURVEFITTINGST& param)
	{
		int p, q;

		// n-1次式の近似曲線を求めるのですか！？
		if ( param.datanum > m_nMaxCoef )
			p = m_nMaxCoef;
		else
			p = param.datanum;
		q = p + 1;

		// 行列テーブルの作成
		AllocMatrix(p, q);

		// 行列テーブルのデータ初期化
		InitMatrix(p, q, param);

		// ガウスの消去法（Gaussian elimination）
		Eliminate(p, q);

		// 導出した係数をメンバー変数に代入しておく
		Substitute(p, q);

		// 行列テーブルの破棄
		FreeMatrix(p);

		return TRUE;
	}

	double GetFitValue(double dX)
	{
//		// 6次式の場合、下記の計算内容と同じ
//		// y = ax^6 + bx^5 + cx^4 + dx^3 + ex^2 + fx + g 
//		double dSum = 0.0;
//		dSum += m_dCoefficient[6] * pow(dX, 6);
//		dSum += m_dCoefficient[5] * pow(dX, 5);
//		dSum += m_dCoefficient[4] * pow(dX, 4);
//		dSum += m_dCoefficient[3] * pow(dX, 3);
//		dSum += m_dCoefficient[2] * pow(dX, 2);
//		dSum += m_dCoefficient[1] * pow(dX, 1);
//		dSum += m_dCoefficient[0];
//		return dSum;

		double dSum = 0.0;

		for ( int i = m_nCoef-1; i >= 0; i-- ) {
			dSum = dSum * dX + m_dCoefficient[i];
		}
		return dSum;
	}

	double GetCoefficient(int nPolyPower)
	{
		ASSERT(nPolyPower <= m_nCoef - 1);

		return m_dCoefficient[nPolyPower];
	}

private:
	void AllocMatrix(int p, int q)
	{
		matrix = new double*[p];
		for ( int i = 0; i < p; i++ ) {
			matrix[i] = new double[q];
			::ZeroMemory(matrix[i], sizeof(double) * q);
		}
	}

	void FreeMatrix(int p)
	{
		for ( int i = 0; i < p; i++ )
			delete [] matrix[i];
		delete [] matrix;
		matrix = NULL;
	}

	void InitMatrix(int p, int q, const CURVEFITTINGST& param)
	{
		int i, j, k;

		for ( i = 0; i < p; i++ )
		{
			for ( j = 0; j < p; j++ )
			{
				for ( k = 0; k < param.datanum; k++ )
				{
					matrix[i][j] += pow(param.point[k], (i + j));
				}
			}
			for ( k = 0; k < param.datanum; k++ )
			{
				matrix[i][j] += param.value[k] * pow(param.point[k], i);
			}
		}
	}

	void Eliminate(int p, int q)
	{
		int i, j, k;

		for ( i = 0; i < p; i++ )
		{
			for ( j = i + 1; j < q; j++ )
			{
				matrix[i][j] = matrix[i][j] / matrix[i][i];
			}
			for ( j = 0; j < p; j++)
			{
				if ( j != i )
				{
					for ( k = i + 1; k < q; k++ )
					{
						matrix[j][k] = matrix[j][k] - (matrix[j][i] * matrix[i][k]);
					}
				}
			}
		}

	}

	void Substitute(int p, int q)
	{
		m_nCoef = p;
		for ( int i = 0; i < p; i++ )
		{
			m_dCoefficient[i] = matrix[i][q - 1];
		}
	}

private:
	const int m_nMaxCoef;
	int m_nCoef;
	double m_dCoefficient[MAXCOEF];

	double** matrix;
};
