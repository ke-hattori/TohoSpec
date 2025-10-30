// Sio.cpp: implementation of the CSio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"
#include "SioDef.h"
#include "Sio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CLogFile *g_pComLogFile;

CRITICAL_SECTION	g_CritSec_COM;	// クリティカルセクション

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSio::CSio(int Channel)
{
	// COM ポート番号
	m_ChannelNo = Channel;
	sprintf(m_sChannelNo, "\\\\.\\COM%d", m_ChannelNo);
	// ハンドルを無効にしておく
	m_Handle = INVALID_HANDLE_VALUE;
	// 受信バッファをクリア
	memset(m_Buff, 0, sizeof(m_Buff));
	// 受信数をクリア
	m_Index = 0;
	// クリティカルセクションを定義
	InitializeCriticalSection(&g_CritSec_COM);

	m_Indicate_Send = FALSE;
	m_Indicate_Recv = FALSE;
	m_LogEnable = FALSE;
}

CSio::~CSio()
{
	// クリティカルセクションを開放
	EnterCriticalSection(&g_CritSec_COM);	// 取得されたままかも知れないので待つ
	LeaveCriticalSection(&g_CritSec_COM);	// 開放する
	DeleteCriticalSection(&g_CritSec_COM);

	if (m_Handle != FALSE) {
		CloseHandle(m_Handle);
	}
}

// ポートの初期化
int CSio::Initialize(		int BaudRate,
							int ByteSize,
							int Parity,
							int StopBits
						)
{
	// COM ポートをオープン
	m_Handle = CreateFile(
							m_sChannelNo,
							GENERIC_READ | GENERIC_WRITE,	// アクセスモード：ReadWrite
							0,	// 共有モード：共有なし
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL
						);

	if (m_Handle == INVALID_HANDLE_VALUE) {
		// オープン失敗
		m_Handle = NULL;
		return FALSE;
	}

	m_BaudRate = BaudRate;
	m_ByteSize = ByteSize;
	m_Parity = Parity;
	m_StopBits = StopBits;

	// デバイス制御ブロックを設定
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);	// デバイス制御ブロックのバイト数
	GetCommState(m_Handle, &dcb);	// 現在の設定を取得

	dcb.BaudRate = BaudRate;	// ボーレート
	dcb.ByteSize = ByteSize;	// 8bit or 7bit
	dcb.Parity	 = Parity;		// パリティ
	dcb.StopBits = StopBits;	// ストップビット
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;	// DTR は常に ON
	dcb.fOutxCtsFlow = 0;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;	// RTS は常に ON
	dcb.fInX = 0;
	dcb.fOutX = 0;

	BOOL ret = SetCommState(m_Handle, &dcb);	// 書換え

	// タイムアウトを設定(タイムアウトは設定しない)
	m_CommTimeOuts.ReadIntervalTimeout = 0;
	m_CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	m_CommTimeOuts.ReadTotalTimeoutConstant = 0;
	m_CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	m_CommTimeOuts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(m_Handle, &m_CommTimeOuts);

	return TRUE;
}

// 送信処理
int CSio::SioSendData(char *buff, int len)
{
	DWORD writesize;
	char SendBuff[256];	// 送信バッファ

	if (m_Handle == NULL) return 0;

	EnterCriticalSection(&g_CritSec_COM);	// クリティカルセクション取得

	// 送信バッファを０クリア
	memset(SendBuff, 0, sizeof(SendBuff));

	// 送信データ長が -1 ならバッファ長を送信データ長にする
	if (len == -1) {
		len = strlen(buff);
	}
	// バッファから送信バッファにコピー
	memcpy(SendBuff, buff, len);

	// 送信する
	if (WriteFile(m_Handle, SendBuff, len, &writesize, NULL) == 0) {
		// 送信失敗
		LeaveCriticalSection(&g_CritSec_COM);	// クリティカルセクション開放
		return -1;
	}

	LeaveCriticalSection(&g_CritSec_COM);	// クリティカルセクション開放
	return (int) writesize;
}

// チェックサムを計算
unsigned char CSio::CalcCheckSum(
									char *data,	// データのポインタ(NULL で終端)
									short mode	// モード：1=add, 2=xor, 3=7bit add, 4=7bit xor
								)
{
	unsigned char	l_sum = 0;

	while (*data != 0x00) {
		switch (mode) {
		case 1:		// add
		case 3:		// add, 7bit, MSB=1
		default:
			l_sum += *data++; break;
		case 2:		// xor
		case 4:		// xor, 7bit, MSB=1
			l_sum ^= *data++; break;
		}
	}
	switch (mode) {
	case 3:		// add, 7bit, MSB=1
	case 4:		// xor, 7bit, MSB=1
		l_sum |= 0x80; break;
	}

	return l_sum;
}

// コマンド通信
//int CSio::CommCommand(char *Send, char *Recv)
int CSio::CommCommand(char *Send, char *Recv, DWORD *Length)
{
	int		l_prc = 0;
	int		l_rc = 0;					// 結果
	char	l_sendbuf[BUFSIZE] = "";	// 送信バッファ
	char	l_recvbuf[BUFSIZE] = "";	// 受信バッファ
	char	l_recvdat[BUFSIZE] = "";	// 受信データ
	DWORD	dwErrors;					// エラー情報
	COMSTAT	ComStat;					// デバイスの状態
	DWORD	dwReadCount;				// 読出したバイト数
//	DWORD	EvtMask;					// 発生イベント
	unsigned char	l_rcvtmp;			// 受信した１バイトデータ
	int		l_SendLen;					// 送信バイト数
	unsigned char	l_Sum;				// チェックサム
	BOOL	l_SendCancel = FALSE;		// キャンセル送信フラグ
	BOOL	l_RecvOne = TRUE;			// １バイト目受信フラグ
	int 	i = 0;
	DWORD	l_RecvLen = 0;				// 受信バイト数
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
	DWORD	l_dwTimeoutPeriod = 100;	// タイムアウト最小値[ms]
#endif

	EnterCriticalSection(&g_CritSec_COM);	// クリティカルセクション取得

#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
	m_dwTimeoutCount = 0;		// タイムアウトカウント
#endif

	while (l_rc == 0) {
		switch (l_prc) {
		case 0:		// 既に受信していたデータがあれば読み捨てる
			m_CommTimeOuts.ReadTotalTimeoutConstant = 0;
			SetCommTimeouts(m_Handle, &m_CommTimeOuts);
			if (ClearCommError(m_Handle, &dwErrors, &ComStat) == 0) {
				// 関数失敗
				l_rc = MSC_PRE_CHECK_FAIL;
			} else {
				if (ComStat.cbInQue != 0) {
					// 受信データがあれば読み捨て
					if (ReadFile(m_Handle, &l_recvbuf, ComStat.cbInQue, &dwReadCount, NULL) == 0) {
						// 関数失敗
						l_rc = MSC_PRE_READ_FAIL;
					} else {
						l_prc = 1;
					}
				} else {
					l_prc = 1;
				}
			}
			break;
		case 1:		// コマンド送信
//			SetCommMask(m_Handle, EV_RXCHAR);	// 監視イベントを設定：1文字受信
			l_Sum = CalcCheckSum(Send, 1);	// チェックサム計算
			strcpy(l_sendbuf, Send);
			l_SendLen = strlen(l_sendbuf);
			l_sendbuf[l_SendLen] = COM_SO;	// 送信デリミタを追加
			// 送信
//			CommLogging(l_sendbuf, 0);
			CommLogging(l_sendbuf, 0, l_SendLen + 1);
			if (SioSendData(l_sendbuf, l_SendLen + 1) == -1) {
				// 送信失敗
				l_rc = MSC_COMMAND_SEND_FAIL;
			} else {
				l_prc = 2;
			}
m_Indicate_Send = TRUE;
			break;
		case 2:		// 確認コードまち(チェックサム[1byte]を受信)
			// １バイト受信タイムアウト ReadTotalTimeoutConstant + ReadTotalTimeoutMultiplier * 1byte
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
			m_CommTimeOuts.ReadTotalTimeoutConstant = l_dwTimeoutPeriod;
			m_dwTimeoutCount = R1TIMEOUT / l_dwTimeoutPeriod;
#else
			m_CommTimeOuts.ReadTotalTimeoutConstant = R1TIMEOUT;
#endif
			SetCommTimeouts(m_Handle, &m_CommTimeOuts);
//			WaitCommEvent(m_Handle, &EvtMask, NULL);
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
READ_RETRY1:
#endif
			if (ReadFile(m_Handle, &l_recvbuf, 1, &dwReadCount, NULL) == 0) {
				// 関数失敗
				l_rc = MSC_READ_FAIL;
			} else {
				if (dwReadCount == 0) {
					// １バイト受信タイムアウト
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
					if (0 != m_dwTimeoutCount) {
						m_dwTimeoutCount--;
						goto READ_RETRY1;
					}
#endif
					l_rc = MSC_CONFIRM_TIMEOUT;
					l_SendCancel = TRUE;
//					CommLogging("Timeout", 1);
					CommLogging("Timeout", 1, 7);
				} else {
m_Indicate_Recv = TRUE;
					l_rcvtmp = l_recvbuf[0]; l_recvbuf[1] = 0x00;
//					CommLogging(l_recvbuf, 1);
					CommLogging(l_recvbuf, 1, 1);
					// １バイト以上受信していないかチェック
					m_CommTimeOuts.ReadTotalTimeoutConstant = 0;
					SetCommTimeouts(m_Handle, &m_CommTimeOuts);
					if (ClearCommError(m_Handle, &dwErrors, &ComStat) == 0) {
						// 関数失敗
						l_rc = MSC_CONFIRM_CHECK_FAIL;
					} else {
						if (ComStat.cbInQue != 0) {
							// 受信データがあれば読み捨て，不正データ受信となる
							if (ReadFile(m_Handle, &l_recvbuf, ComStat.cbInQue, &dwReadCount, NULL) == 0) {
								// 関数失敗
								l_rc = MSC_CONFIRM_READ_FAIL;
							} else {
								// 不正データを受信
m_Indicate_Recv = TRUE;
								l_rc = MSC_CONFIRM_FORMAT_ERROR;
								l_SendCancel = TRUE;
								l_recvbuf[dwReadCount] = 0x00;
//								CommLogging(l_recvbuf, 1);
								CommLogging(l_recvbuf, 1, dwReadCount);
							}
						} else {
							if (l_Sum != l_rcvtmp) {
								// チェックサムエラー
								l_rc = MSC_CONFIRM_SUM_ERROR;
								l_SendCancel = TRUE;
							} else {
								l_prc = 3;
							}
						}
					}
				}
			}
			break;
		case 3:		// 実行コードを送信(cr)
			l_sendbuf[0] = COM_CR; l_sendbuf[1] = 0x00;
//			CommLogging(l_sendbuf, 0);
			CommLogging(l_sendbuf, 0, 1);
			if (SioSendData(l_sendbuf, 1) == -1) {
				// 送信失敗
				l_rc = MSC_GO_COMMAND_FAIL;
			} else {
				l_prc = 4;
			}
m_Indicate_Send = TRUE;
			break;
		case 4:		// 応答データを受信
			// 多バイト受信タイムアウト ReadTotalTimeoutConstant + ReadTotalTimeoutMultiplier * 1byte
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
			m_CommTimeOuts.ReadTotalTimeoutConstant = l_dwTimeoutPeriod;
			m_dwTimeoutCount = R1TIMEOUT / l_dwTimeoutPeriod;
#else
			m_CommTimeOuts.ReadTotalTimeoutConstant = R1TIMEOUT;
#endif
			SetCommTimeouts(m_Handle, &m_CommTimeOuts);
			l_rcvtmp = 0x00;
			i = 0;
//			memset(l_recvbuf, 0x00, sizeof(l_recvbuf));
//			while (l_rcvtmp != COM_ACK) {
			while (1) {
				// ACK 受信までループ
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
READ_RETRY2:
#endif
				if (ReadFile(m_Handle, &l_recvbuf, 1, &dwReadCount, NULL) == 0) {
					// 関数失敗
					l_rc = MSC_RESPONSE_READ_FAIL;
					break;
				} else {
					if (dwReadCount == 0) {
						// 受信タイムアウト
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
						if (0 != m_dwTimeoutCount) {
							m_dwTimeoutCount--;
							goto READ_RETRY2;
						}
#endif
						l_rc = MSC_RESPONSE_TIMEOUT;
						l_SendCancel = TRUE;
//						CommLogging("Timeout", 1);
						CommLogging("Timeout", 1, 7);
						break;
					} else {
m_Indicate_Recv = TRUE;
						if ((i == 0) && (l_recvbuf[0] == COM_BEL)) {
							// １バイト目が BEL なら中断
							l_rc = MSC_RESPONSE_BEL;
//							l_RecvOne = FALSE;
							l_recvbuf[1] = 0x00;
//							CommLogging(l_recvbuf, 1);
							CommLogging(l_recvbuf, 1, 1);
							// 残りのデータがあれば読み捨てる
							m_CommTimeOuts.ReadTotalTimeoutConstant = 0;
							SetCommTimeouts(m_Handle, &m_CommTimeOuts);
							if (ClearCommError(m_Handle, &dwErrors, &ComStat) == 0) {
								// 関数失敗
								l_rc = MSC_RESPONSE_BEL_CHECK_FAIL;
							} else {
								if (ComStat.cbInQue != 0) {
									// 受信データがあれば読み捨て
									if (ReadFile(m_Handle, &l_recvbuf, ComStat.cbInQue, &dwReadCount, NULL) == 0) {
										// 関数失敗
										l_rc = MSC_RESPONSE_BEL_CHKREAD_FAIL;
									} else {
										// 不正データを受信
m_Indicate_Recv = TRUE;
										l_rc = MSC_RESPONSE_BEL_FORMAT_ERROR;
										l_SendCancel = TRUE;
										l_recvbuf[dwReadCount] = 0x00;
//										CommLogging(l_recvbuf, 1);
										CommLogging(l_recvbuf, 1, dwReadCount);
									}
								}
							}
							break;
						} else if ((i == 0) && (l_recvbuf[0] == COM_ACK)) {
							// １バイト目が ACK なら終了
//							l_RecvOne = FALSE;
							l_rcvtmp = l_recvbuf[0];
							l_recvdat[i++] = l_rcvtmp;
							l_recvdat[i] = 0x00;	// 毎回，終端を書込む
							break;
						} else {
							l_rcvtmp = l_recvbuf[0];
							l_recvdat[i++] = l_rcvtmp;
							l_recvdat[i] = 0x00;	// 毎回，終端を書込む
							if ((BUFSIZE - 1) < i) {
								l_rc = MSC_RESPONSE_OVERFLOW;
								break;
							}
							if ((4 <= i) && (l_recvdat[i - 3] == COM_CR) && (l_rcvtmp == COM_ACK)) {
								// ACK を正規の位置で認識のため終了
								break;
							}
						}
					}
				}
			}
			if (l_rcvtmp == COM_ACK) {
				// 残り１バイトを受信(チェックサム)
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
				m_dwTimeoutCount = R1TIMEOUT / l_dwTimeoutPeriod;
READ_RETRY3:
#endif
				if (ReadFile(m_Handle, &l_recvbuf, 1, &dwReadCount, NULL) == 0) {
					// 関数失敗
					l_rc = MSC_RESPONSE_SUM_READ_FAIL;
				} else {
					if (dwReadCount == 0) {
						// １バイト受信タイムアウト
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
						if (0 != m_dwTimeoutCount) {
							m_dwTimeoutCount--;
							goto READ_RETRY3;
						}
#endif
						l_rc = MSC_RESPONSE_SUM_TIMEOUT;
						l_SendCancel = TRUE;
//						CommLogging("Timeout", 1);
						CommLogging("Timeout", 1, 7);
					} else {
m_Indicate_Recv = TRUE;
						l_rcvtmp = l_recvbuf[0];
						l_recvdat[i++] = l_rcvtmp;
						l_recvdat[i] = 0x00;	// 終端を追加
//						CommLogging(l_recvdat, strlen(l_recvdat));
						CommLogging(l_recvdat, 1, i);
						// これ以降受信していないかチェック
						m_CommTimeOuts.ReadTotalTimeoutConstant = 0;
						SetCommTimeouts(m_Handle, &m_CommTimeOuts);
						if (ClearCommError(m_Handle, &dwErrors, &ComStat) == 0) {
							// 関数失敗
							l_rc = MSC_RESPONSE_SUM_CHECK_FAIL;
						} else {
							if (ComStat.cbInQue != 0) {
								// 受信データがあれば読み捨て
								if (ReadFile(m_Handle, &l_recvbuf, ComStat.cbInQue, &dwReadCount, NULL) == 0) {
									// 関数失敗
									l_rc = MSC_RESPONSE_SUM_CHKREAD_FAIL;
								} else {
									// 不正データを受信
m_Indicate_Recv = TRUE;
									l_rc = MSC_RESPONSE_SUM_FORMAT_ERROR;
									l_SendCancel = TRUE;
									l_recvbuf[dwReadCount] = 0x00;
//									CommLogging(l_recvbuf, 1);
									CommLogging(l_recvbuf, 1, dwReadCount);
								}
							} else {
								if (l_Sum != l_rcvtmp) {
									// チェックサムエラー
									l_rc = MSC_RESPONSE_SUM_ERROR;
									l_SendCancel = TRUE;
								} else {
									// 通信成功
									l_rc = MSC_COMM_OK;
								}
							}
						}
					}
				}
			}
			break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		default:
			break;
		}
	}

	// キャンセル送信
	if (l_SendCancel == TRUE) {
		l_SendCancel = FALSE;
		l_sendbuf[0] = COM_CAN; l_sendbuf[1] = 0x00;
//		CommLogging(l_sendbuf, 0);
		CommLogging(l_sendbuf, 0, 1);
m_Indicate_Send = TRUE;
		if (SioSendData(l_sendbuf, 1) == -1) {
			// 送信失敗
			l_rc |= MSC_CANSEL_SEND_FAIL;
		} else {
			// キャンセル受信待ち
			// １バイト受信タイムアウト ReadTotalTimeoutConstant + ReadTotalTimeoutMultiplier * 1byte
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
			m_CommTimeOuts.ReadTotalTimeoutConstant = l_dwTimeoutPeriod;
			m_dwTimeoutCount = R1TIMEOUT / l_dwTimeoutPeriod;
#else
			m_CommTimeOuts.ReadTotalTimeoutConstant = R1TIMEOUT;
#endif
			SetCommTimeouts(m_Handle, &m_CommTimeOuts);
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
READ_RETRY4:
#endif
			if (ReadFile(m_Handle, &l_recvbuf, 1, &dwReadCount, NULL) == 0) {
				// 関数失敗
				l_rc |= MSC_CANCEL_READ_FAIL;
			} else {
				if (dwReadCount == 0) {
					// １バイト受信タイムアウト
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
					if (0 != m_dwTimeoutCount) {
						m_dwTimeoutCount--;
						goto READ_RETRY4;
					}
#endif
					l_rc |= MSC_CANCEL_TIMEOUT;
//					CommLogging("Timeout", 1);
					CommLogging("Timeout", 1, 7);
				} else {
m_Indicate_Recv = TRUE;
					l_rcvtmp = l_recvbuf[0];
					l_recvbuf[1] = 0x00;
//					CommLogging(l_recvbuf, 1);
					CommLogging(l_recvbuf, 1, 1);
					// １バイト以上受信していないかチェック
					m_CommTimeOuts.ReadTotalTimeoutConstant = 0;
					SetCommTimeouts(m_Handle, &m_CommTimeOuts);
					if (ClearCommError(m_Handle, &dwErrors, &ComStat) == 0) {
						// 関数失敗
						l_rc = MSC_CANCEL_CHECK_FAIL;
					} else {
						if (ComStat.cbInQue != 0) {
							// 受信データがあれば読み捨て，不正データ受信となる
							if (ReadFile(m_Handle, &l_recvbuf, ComStat.cbInQue, &dwReadCount, NULL) == 0) {
								// 関数失敗
								l_rc = MSC_CANCEL_CHKREAD_FAIL;
							} else {
								// 不正データを受信
m_Indicate_Recv = TRUE;
								l_rc = MSC_CANCEL_FORMAT_ERROR;
								l_recvbuf[dwReadCount] = 0x00;
//								CommLogging(l_recvbuf, 1);
								CommLogging(l_recvbuf, 1, dwReadCount);
							}
						} else {
							if (COM_CAN != l_rcvtmp) {
								// キャンセルでなかった
								l_rc = MSC_CANCEL_NOT_CANCELCODE;
							} else {
								;	// キャンセル受信成功
							}
						}
					}
				}
			}
		}
	}

	// 受信データコピー
	if (l_recvdat[0] != 0x00) {
		// 受信データが１バイトでもあればコピーしておく
//		strcpy(Recv, l_recvdat);
		memcpy(Recv, l_recvdat, i);
		Recv[i] = 0x00;
		*Length = i;
	} else {
		*Length = 0;
	}

	LeaveCriticalSection(&g_CritSec_COM);	// クリティカルセクション開放
	return l_rc;
}

// 通信ログ
void CSio::CommLogging(
					   char *LogData,	// データ
					   int Dir,			// 0：送信，1：受信
					   DWORD Length		// データ長
					   )
{
	char	l_LogBuff[1024];
	char	l_cBuff[1024];

	l_LogBuff[0] = 0x00; l_cBuff[0] = 0x00;

	// 日付，時刻を設定
	g_pComLogFile->AddTime(l_LogBuff, 2);
	// 方向を設定
	if (Dir == 0) {
		strcat(l_LogBuff, " PC -> CTRL : ");
	} else {
		strcat(l_LogBuff, " PC <- CTRL : ");
	}
	// バイナリを HEX に変換
//	g_pComLogFile->ConvBinToHex(LogData, l_cBuff);
	g_pComLogFile->ConvBinToHex(LogData, l_cBuff, Length);
	strcat(l_LogBuff, l_cBuff);
	// ログ書き込み
	if (m_LogEnable == TRUE) {
		g_pComLogFile->Logging(l_LogBuff);
	}
}

// ログをクリア
void CSio::ClearLog()
{
	BOOL	l_Enable;

	// ログをディセーブルにしておいてクリア
	l_Enable = m_LogEnable;
	if (l_Enable == TRUE) {
		EnterCriticalSection(&g_CritSec_COM);	// クリティカルセクション取得
		m_LogEnable = FALSE;
		LeaveCriticalSection(&g_CritSec_COM);	// クリティカルセクション開放
	}

	g_pComLogFile->ClearLog();

	m_LogEnable = l_Enable;

}

BOOL CSio::EndComm()
{
#ifdef COM_TIMEOUT_MODE	//-------------------------------------------------------------------------
	m_dwTimeoutCount = 0;
#endif

	return TRUE;
}
