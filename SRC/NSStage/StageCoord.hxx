#pragma once

#include "globals.hxx"
#include "ConfigFile.hxx"


#ifndef _DEBUG
// Release
#pragma comment(lib, "TnsConfigFile.lib")
#else
// Debug
#pragma comment(lib, "dtnsConfigfile.lib")
#endif // _DEBUG

/*
// CStageCoord について
//
// CStageCoord クラスは、軸座標（STAGEAPI用）とクライアントステージ座標（UI画面用）の変換を目的としています。
// 座標変換情報を取得するため、クラス内部では ConfigAPI を使用しています。
//
//
// CStageCoord クラス の使用方法について
//
// CStageCoord クラスは、globals.hxx の STAGE_COORD 構造体と同様のものです。
// CStageCoord クラスも、CStageCoord オブジェクトや globals.hxx の STAGE_COORD 構造体を操作するメンバ関数を持っています。
//
// CStageCoord オブジェクトは、関数の引数に STAGE_COORD 構造体や、STAGE_COORD*、const STAGE_COORD* が渡せるところにはどこにでも使用できます。
//
// オーバーロードされた CStageCoord 演算子を使うときは、最初のオペランドは必ず CStageCoord でなければいけません。
// 2 番目は STAGE_COORD 構造体でも CStageCoord オブジェクトでもかまいません。
//
//
// CStageCoord クラス メンバ について
//
// CStageCoord::CStageCoord()                           CStageCoord オブジェクトを構築します。（X,Y座標の初期値Zero）
// CStageCoord::CStageCoord(STAGE_COORD coord)          CStageCoord オブジェクトを構築します。（X,Y座標の初期値設定）
// CStageCoord::CStageCoord(long lX, long lY)           CStageCoord オブジェクトを構築します。（X,Y座標の初期値設定（個別））
// CStageCoord::CStageCoord(const STAGE_COORD* pCoord)  CStageCoord オブジェクトを構築します。（X,Y座標の初期値設定）
// void CStageCoord::ToApiCoord()                       STAGEAPI用（軸座標）の座標に変換します。
// void CStageCoord::ToUiDisplayCoord()                 UI画面用（クライアントステージ座標）の座標に変換します。
//
//   上記以外は、ソース参照のこと。
*/

class CStageCoord : public _STAGE_COORD
{
public:
	// Constructors
	CStageCoord() {::ZeroMemory(this, sizeof(*this));}					// X,Y座標の初期値Zero
	CStageCoord(const STAGE_COORD& srcCoord) {CopyCoord(&srcCoord);}	// X,Y座標の初期値設定
	CStageCoord(long lX, long lY) {this->lX = lX; this->lY = lY;}		// X,Y座標の初期値設定（個別）
	CStageCoord(const STAGE_COORD* pSrcCoord) {CopyCoord(pSrcCoord);} 	// X,Y座標の初期値設定

	// STAGEAPI用（変換後、クライアントステージ原点の軸座標のオフセットを加算）
	void ToApiCoord() {ToXyAxisCoord(); *this += XyAxisClientXyStageOriginCoord();}

	// UI画面用（クライアントステージ原点の軸座標のオフセットを減算後、変換）
	void ToUiDisplayCoord() {*this -= XyAxisClientXyStageOriginCoord(); ToClientXyStageCoord();}

	// X,Y各座標の取得
	long GetCoordX() const {return this->lX;}
	long GetCoordY() const {return this->lY;}

	// キャスト
	operator STAGE_COORD*() {return this;}
	operator const STAGE_COORD*() const {return this;}

	// 演算子
	void operator +=(const STAGE_COORD& coord) {AddCoord(&coord);}
	void operator -=(const STAGE_COORD& coord) {SubtractCoord(&coord);}
	CStageCoord& operator =(const STAGE_COORD& coord) {CopyCoord(&coord); return *this;}
	CStageCoord operator +(const STAGE_COORD& coord2) const {CStageCoord coord(this); coord.AddCoord(&coord2); return coord;}
	CStageCoord operator -(const STAGE_COORD& coord2) const {CStageCoord coord(this); coord.SubtractCoord(&coord2); return coord;}

private:
	enum DIR {DIR_UNKNOWN = 0, DIR_RIGHT, DIR_TOP, DIR_LEFT, DIR_BOTTOM, DIR_MAX};

	// クライアントステージ原点の軸座標
	CStageCoord XyAxisClientXyStageOriginCoord()	// 軸座標（STAGEAPI用）
	{
		STAGE_COORD originCoord;
		STAGE_CONFIG stageConfig;
		ConfigFile_GetNanoSpecIni(&stageConfig, CONFIG_FILE_STAGE_CONFIG);
		ConfigDataCheck(&stageConfig);

/*		switch ( stageConfig.OrgPos ) {
		case 1: // 中心（サブコン設定とクライアントステージ原点が一致。つまりオフセットなし）
			originCoord.lX = 0L;
			originCoord.lY = 0L;
			break;
		case 2: // 左上
			originCoord.lX = (long)stageConfig.StageMaxCoord.dLeftX;
			originCoord.lY = (long)stageConfig.StageMaxCoord.dUpY;
			break;
		case 3: // 左下
			originCoord.lX = (long)stageConfig.StageMaxCoord.dLeftX;
			originCoord.lY = (long)stageConfig.StageMaxCoord.dDownY;
			break;
		case 4: // 右下
			originCoord.lX = (long)stageConfig.StageMaxCoord.dRightX;
			originCoord.lY = (long)stageConfig.StageMaxCoord.dDownY;
			break;
		case 5: // 右上
			originCoord.lX = (long)stageConfig.StageMaxCoord.dRightX;
			originCoord.lY = (long)stageConfig.StageMaxCoord.dUpY;
			break;
		}
*/
		//サブコン設定とクライアントステージ原点のオフセットはステージオフセットから使用する(ステージの原点位置に拘らず)
		//中心-> 0, 0
		//左上-> OffsetX(minus), OffsetY
		//左下-> OffsetX(minus), OffsetY(minus)
		//右下-> OffsetX, OffsetY(minus)
		//右上-> OffsetX, OffsetY
		originCoord.lX = (long)(stageConfig.Origin.lX * -1);
		originCoord.lY = (long)(stageConfig.Origin.lY * -1);

		return CStageCoord(originCoord.lX, originCoord.lY);
	}

//	// クライアントステージ原点
//	CStageCoord ClientXyStageOriginCoord() 			// クライアントステージ座標（UI画面用）
//	{
//		return CStageCoord(0L, 0L);
//	}

public:
	// 軸座標（STAGEAPI用）に変換
	void ToXyAxisCoord()
	{
// 2013.01.09 bagus -->
		STAGE_CONFIG stageConfig;
		ConfigFile_GetNanoSpecIni(&stageConfig, CONFIG_FILE_STAGE_CONFIG);
		ConfigDataCheck(&stageConfig);

// 	// 画面表示・軸方向StageConfigの軸設定をプラス方向にする
// 		// LEFT
// 		if ( stageConfig.Dir.X == DIR_LEFT )
// 			this->lX *= -1;
// 		else if ( stageConfig.Dir.Y == DIR_LEFT )
// 			this->lX *= -1;
//
// 		// BOTTOM
// 		if ( stageConfig.Dir.X == DIR_BOTTOM )
// 			this->lY *= -1;
// 		else if ( stageConfig.Dir.Y == DIR_BOTTOM )
// 			this->lY *= -1;

	// 画面表示・軸方向StageConfigの軸設定をプラス方向にする
		// X: RIGHT
		if ( stageConfig.Dir.X == DIR_RIGHT ) {
			switch ( stageConfig.Dir.Y ) {
			case DIR_TOP:
				break;
			case DIR_BOTTOM:
				this->lY *= -1;
				break;
			default:
				break;
			}
		}

		// X: LEFT
		if ( stageConfig.Dir.X == DIR_LEFT ) {
			this->lX *= -1;
			switch ( stageConfig.Dir.Y ) {
			case DIR_TOP:
				break;
			case DIR_BOTTOM:
				this->lY *= -1;
				break;
			default:
				break;
			}
		}

		// X: TOP
		if ( stageConfig.Dir.X == DIR_TOP ) {
			STAGE_COORD tempCoord;

			tempCoord = *this;

			this->lY = tempCoord.lX;
			switch ( stageConfig.Dir.Y ) {
			case DIR_RIGHT:
				this->lX = tempCoord.lY;
				break;
			case DIR_LEFT:
				this->lX = tempCoord.lY * -1;
				break;
			default:
				break;
			}
		}

		// X: BOTTOM
		if ( stageConfig.Dir.X == DIR_BOTTOM ) {
			STAGE_COORD tempCoord;

			tempCoord = *this;

			this->lY = tempCoord.lX * -1;
			switch ( stageConfig.Dir.Y ) {
			case DIR_RIGHT:
				this->lX = tempCoord.lY;
				break;
			case DIR_LEFT:
				this->lX = tempCoord.lY * -1;
				break;
			default:
				break;
			}
		}
// 2013.01.09 bagus <--
	}

	// クライアントステージ座標（UI画面用）に変換
	void ToClientXyStageCoord()
	{
// 2013.01.09 bagus -->
		STAGE_CONFIG stageConfig;
		ConfigFile_GetNanoSpecIni(&stageConfig, CONFIG_FILE_STAGE_CONFIG);
		ConfigDataCheck(&stageConfig);

// 	// 画面表示・軸方向StageConfigの軸設定をプラス方向にする
// 		// LEFT
// 		if ( stageConfig.Dir.X == DIR_LEFT )
// 			this->lX *= -1;
// 		else if ( stageConfig.Dir.Y == DIR_LEFT )
// 			this->lX *= -1;
//
// 		// BOTTOM
// 		if ( stageConfig.Dir.X == DIR_BOTTOM )
// 			this->lY *= -1;
// 		else if ( stageConfig.Dir.Y == DIR_BOTTOM )
// 			this->lY *= -1;

	// 画面表示・軸方向StageConfigの軸設定をプラス方向にする
		// X: RIGHT
		if ( stageConfig.Dir.X == DIR_RIGHT ) {
			switch ( stageConfig.Dir.Y ) {
			case DIR_TOP:
				break;
			case DIR_BOTTOM:
				this->lY *= -1;
				break;
			default:
				break;
			}
		}

		// X: LEFT
		if ( stageConfig.Dir.X == DIR_LEFT ) {
			this->lX *= -1;
			switch ( stageConfig.Dir.Y ) {
			case DIR_TOP:
				break;
			case DIR_BOTTOM:
				this->lY *= -1;
				break;
			default:
				break;
			}
		}

		// X: TOP
		if ( stageConfig.Dir.X == DIR_TOP ) {
			STAGE_COORD tempCoord;

			tempCoord = *this;

			this->lX = tempCoord.lY;
			switch ( stageConfig.Dir.Y ) {
			case DIR_RIGHT:
				this->lY = tempCoord.lX;
				break;
			case DIR_LEFT:
				this->lY = tempCoord.lX * -1;
				break;
			default:
				break;
			}
		}

		// X: BOTTOM
		if ( stageConfig.Dir.X == DIR_BOTTOM ) {
			STAGE_COORD tempCoord;

			tempCoord = *this;

			this->lX = tempCoord.lY * -1;
			switch ( stageConfig.Dir.Y ) {
			case DIR_RIGHT:
				this->lY = tempCoord.lX;
				break;
			case DIR_LEFT:
				this->lY = tempCoord.lX * -1;
				break;
			default:
				break;
			}
		}
// 2013.01.09 bagus <--
	}

private:
	// 座標のOffset設定
	void AddCoord(const STAGE_COORD* pCoord) {this->lX += pCoord->lX; this->lY += pCoord->lY;}
	void SubtractCoord(const STAGE_COORD* pCoord) {this->lX -= pCoord->lX; this->lY -= pCoord->lY;}
	// 座標のコピー
	void CopyCoord(const STAGE_COORD* pCoord) {::CopyMemory(this, pCoord, sizeof(*this));}

	// StageConfigのデータチェック
	void ConfigDataCheck(const STAGE_CONFIG* pStageConfig)
	{
		ASSERT( pStageConfig->Dir.X > DIR_UNKNOWN && pStageConfig->Dir.X < DIR_MAX );	// X方向
		ASSERT( pStageConfig->Dir.Y > DIR_UNKNOWN && pStageConfig->Dir.Y < DIR_MAX );	// Y方向
		ASSERT( pStageConfig->Dir.X != pStageConfig->Dir.Y );					// X,Yが同一でないこと
		ASSERT( (pStageConfig->Dir.X + pStageConfig->Dir.Y) % 2 != 0 ); 		// X,Yの方向が一直線でないこと
	}
};
