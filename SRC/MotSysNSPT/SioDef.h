
#define	COM_TIMEOUT_MODE

// 制御コード定義

#define	COM_SO	0x0e
#define	COM_CR	0x0d
#define	COM_ACK	0x06
#define	COM_BEL	0x07
#define	COM_CAN	0x18
#define	BUFSIZE	1024	// 送信/受信バッファサイズ
#define	R1TIMEOUT	5000	// １バイト受信タイムアウト[ms]
#define	RATIMEOUT	1000	// 多バイト受信タイムアウト[ms] 
#define	RETRY_TIMES	3		// リトライ回数

// 通信エラー定義
#define	MSC_COMM_OK						0x00000001	//  1:通信成功
#define	MSC_PRE_CHECK_FAIL				0x80000002	//  2:送信前の受信済みデータチェック時の関数[ClearCommError]の失敗
#define	MSC_PRE_READ_FAIL				0x80000003	//  3:送信前の受信済みデータ読出し時の関数[ReadFile]の失敗
#define	MSC_COMMAND_SEND_FAIL			0x80000004	//  4:コマンド送信での送信関数[WriteFile]の失敗
#define	MSC_READ_FAIL					0x80000005	//  5:確認コードの受信データ読出し時の関数[ReadFile]の失敗
#define	MSC_CONFIRM_TIMEOUT				0x00000006	//  6:確認コードの受信の１バイト受信タイムアウト
#define	MSC_CONFIRM_CHECK_FAIL			0x80000007	//  7:確認コードの受信の１バイト以上受信のデータチェック関数[ClearCommError]の失敗
#define	MSC_CONFIRM_READ_FAIL			0x80000008	//  8:確認コードの受信の１バイト以上受信の読出し関数[ReadFile]の失敗
#define	MSC_CONFIRM_FORMAT_ERROR		0x00000009	//  9:確認コードの受信時の１バイト以上の受信(不正データ扱い)
#define	MSC_CONFIRM_SUM_ERROR			0x0000000a	// 10:確認コードの受信時のチェックサムエラー
#define	MSC_GO_COMMAND_FAIL				0x8000000b	// 11:実行コード送信時の送信関数[WriteFile]の失敗
#define	MSC_RESPONSE_READ_FAIL			0x8000000c	// 12:応答データ受信時のデータ読出し関数[ReadFile]の失敗
#define	MSC_RESPONSE_TIMEOUT			0x0000000d	// 13:応答データ受信時の１バイト受信タイムアウト
#define	MSC_RESPONSE_BEL				0x0000000e	// 14:応答データ受信時の BEL 受信
#define	MSC_RESPONSE_BEL_CHECK_FAIL		0x8000000f	// 15:BEL 受信後の１バイト以上受信のデータチェック関数[ClearCommError]の失敗
#define	MSC_RESPONSE_BEL_CHKREAD_FAIL	0x80000010	// 16:BEL 受信後の１バイト以上受信の読出し関数[ReadFile]の失敗
#define	MSC_RESPONSE_BEL_FORMAT_ERROR	0x00000011	// 17:BEL 受信後の１バイト以上の受信(不正データ扱い)
#define	MSC_RESPONSE_SUM_READ_FAIL		0x80000012	// 18:応答データのチェックサムのデータ読出し関数[ReadFile]の失敗
#define	MSC_RESPONSE_SUM_TIMEOUT		0x80000013	// 19:応答データのチェックサムの１バイト受信タイムアウト
#define	MSC_RESPONSE_SUM_CHECK_FAIL		0x00000014	// 20:応答データのチェックサムの１バイト以上受信データチェック関数[ClearCommError]の失敗
#define	MSC_RESPONSE_SUM_CHKREAD_FAIL	0x80000015	// 21:応答データのチェックサムの１バイト以上受信の読出し関数[ReadFile]の失敗
#define	MSC_RESPONSE_SUM_FORMAT_ERROR	0x00000016	// 22:応答データのチェックサムの１バイト以上の受信(不正データ扱い)
#define	MSC_RESPONSE_SUM_ERROR			0x00000017	// 23:応答データのチェックサムエラー
#define	MSC_RESPONSE_OVERFLOW			0x00000018	// 24:受信バッファオーバーフロー
#define	MSC_CANSEL_SEND_FAIL			0xc0010000	//  1:キャンセル送信での送信関数[WriteFile]の失敗
#define	MSC_CANCEL_READ_FAIL			0xc0020000	//  2:キャンセル受信での読出し関数[ReadFile]の失敗
#define	MSC_CANCEL_TIMEOUT				0x00030000	//  3:キャンセル受信での１バイト受信タイムアウト
#define	MSC_CANCEL_CHECK_FAIL			0xc0040000	//  4:キャンセル受信での１バイト以上受信データチェック関数[ClearCommError]の失敗
#define	MSC_CANCEL_CHKREAD_FAIL			0xc0050000	//  5:キャンセル受信での１バイト以上受信データチェック関数[ClearCommError]の失敗
#define	MSC_CANCEL_FORMAT_ERROR			0x00060000	//  6:キャンセル受信での１バイト以上の受信(不正データ扱い)
#define	MSC_CANCEL_NOT_CANCELCODE		0x00070000	//  7:キャンセル受信でのキャンセル以外のデータ受信
