#if !defined( _ADAX_H_ )
#define _ADAX_H_

#pragma once

#include <crtdbg.h>

using namespace std;

// #########################################################################
// MACRO
// #########################################################################
#ifndef countof
	#define countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#define BEGIN_NEXAX_RET_VAL_ERR() \
	basic_timer::do_events(1L); \
	unsigned long retval = NEXAXERR_NONE;
#define NEXAX_RET_VAL_ERR(ecode) \
	retval = ecode; \
	if ( retval != NEXAXERR_NONE ) \
		return retval;
#define END_NEXAX_RET_VAL_ERR() \
	return retval;

// =========================================================================
// ADAX COMMAND ID
// =========================================================================
#define ADAXCMDID_RESPONSE_OK					0x00
#define ADAXCMDID_RESPONSE_NG					0x01
#define ADAXCMDID_PARAMS_DOWN_BEGIN 			0x02
#define ADAXCMDID_PARAMS_DOWN_REQUEST			0x03
#define ADAXCMDID_PARAMS_UP_BEGIN				0x04
#define ADAXCMDID_PARAMS_UP_REQUEST 			0x05
#define ADAXCMDID_PARAMS_SET_BEGIN				0x06
#define ADAXCMDID_PARAMS_SET					0x07
#define ADAXCMDID_PARAMS_REFERENCE_BEGIN		0x08
#define ADAXCMDID_PARAMRS_EFERENCE				0x09
#define ADAXCMDID_EEPROM_INITIAL				0x0A
#define ADAXCMDID_EEPROM_WRITE_ENABLE			0x0B
#define ADAXCMDID_EEPROM_WRITE					0x0C
#define ADAXCMDID_NORMAL_JOG					0x10
#define ADAXCMDID_HOME							0x11
#define ADAXCMDID_PULSE_JOG 					0x12
#define ADAXCMDID_FEED_PULSE_SET				0x13
#define ADAXCMDID_ANALOG_SET_BEGIN				0x14
#define ADAXCMDID_ANALOG_SET					0x15
#define ADAXCMDID_ANALOG_REFERENCE_BEGIN		0x16
#define ADAXCMDID_ANALOG_REFERENCE				0x17
#define ADAXCMDID_COMM_PARAMS_SET				0x18
#define ADAXCMDID_COMM_PARAMS_REFERENCE 		0x19
#define ADAXCMDID_TRIAL_RUN_STATE_SET			0x1A
#define ADAXCMDID_HOME_ACCEL_DECEL_TIME_SET 	0x1B
#define ADAXCMDID_CURRENT_GAIN_RESET			0x1C
#define ADAXCMDID_MACHINE_DIAG_RESET			0x1D
#define ADAXCMDID_MACHINE_DIAG_BEGIN			0x1E
#define ADAXCMDID_TRACE_CONDITION_SET			0x20
#define ADAXCMDID_TRACE_OBJECT_REGIST			0x21
#define ADAXCMDID_TRACE_TRIGGER_SET 			0x22
#define ADAXCMDID_TRACE_CMD 					0x23
#define ADAXCMDID_TRACE_DATA_UP_BEGIN			0x24
#define ADAXCMDID_TRACE_DATA_UP_REQUEST 		0x25
#define ADAXCMDID_PROGRAM_DOWN_BEGIN			0x30
#define ADAXCMDID_PROGRAM_DOWN_REQUEST			0x31
#define ADAXCMDID_PROGRAM_UP_BEGIN				0x32
#define ADAXCMDID_PROGRAM_UP_REQUEST			0x33
#define ADAXCMDID_BREAK_POINT					0x34
#define ADAXCMDID_PROGRAM_CMD					0x35
#define ADAXCMDID_PROGRAM_EEPROM				0x36
#define ADAXCMDID_VARIABLE_REFERENCE			0x37
#define ADAXCMDID_POST_EACH_NO_SET				0x38
#define ADAXCMDID_POST_EACH_DATA_SET			0x39
#define ADAXCMDID_PROGRAM_DOWN_INTERRUPTION 	0x3A
#define ADAXCMDID_SOFT_VER_REFERENCE			0x40
#define ADAXCMDID_HARD_VER_REFERENCE			0x41
#define ADAXCMDID_OFFLINE_PARAMS_REFERENCE		0x50
#define ADAXCMDID_OFFLINE_PARAMS_SET			0x51
#define ADAXCMDID_OFFLINE_CMD					0x52
#define ADAXCMDID_OFFLINE_TRACE_DATA_UP_BEGIN	0x53
#define ADAXCMDID_OFFLINE_TRACE_DATA_UP_REQUEST 0x54
#define ADAXCMDID_OFFLINE_INERTIAMOMENT_SET 	0x55
#define ADAXCMDID_OFFLINE_TRACE_OBJECT_REGIST	0x56
#define ADAXCMDID_ACK							0x80
#define ADAXCMDID_CURRENT_POS					0x81
#define ADAXCMDID_REFERENCE 					0x82
#define ADAXCMDID_REMOTE_COMMON 				0x90
#define ADAXCMDID_REMOTE_DATA					0x91
#define ADAXCMDID_REMOTE_PULSE_JOG				0x92

// =========================================================================
// ADAX ERROR CODE
// =========================================================================
#define ADAXERR_NONE							0x00
#define ADAXERR_PARITY							0x01
#define ADAXERR_BCC 							0x02
#define ADAXERR_FRAMING 						0x03
#define ADAXERR_OVERRUN 						0x04
#define ADAXERR_PROTOCOL						0x05
#define ADAXERR_ASCIICODE						0x06
#define ADAXERR_BUFFEROVERFLOW					0x07
#define ADAXERR_TIMEOUT 						0x08
#define ADAXERR_ABNORMALCMD 					0x11
#define ADAXERR_RUNIMPROPRIETY					0x13
#define ADAXERR_ABNORMALPARAM					0x16
#define ADAXERR_ABNORMALDATASIZE				0x17
#define ADAXERR_ABNORMALCOMMUNICATE 			0x18

#define ADAX_EX 								TEXT("01")
#define ADAXSYNC_KERNELSTR					   TEXT("ADAX_KERNELSTR")

typedef BOOL (__stdcall *PFNADAXWAITPROC)(unsigned short);

enum adax_status_enum {
	DOWN = 0,
	IDLE,
	RUN,
};

enum connect_mode_enum {
	OFFLINE = 0,
	ONLINE,
};

struct adax_info_t {
	BOOL			   initilized;
	BOOL			   established;
	adax_status_enum   status;
	connect_mode_enum  connect;
	unsigned short	   alid;
	struct {
		unsigned short axis;
		location_enum  loc;
		long		   pos;
	} curr;
};

class cAdaxServo : nano::thread
{
public:
	// constructor / destructor
	cAdaxServo (unsigned short axis);
	virtual ~ cAdaxServo ();

	inline BOOL IsIdle (void) const { return m_info.status == IDLE; }
	inline BOOL IsReady (void) const
	{
		if ( m_mode != FA22_Pro || IsHardwareSimulation() ) return TRUE;
		return nexioIsServoOnEx(m_info.curr.axis) && !IsAlarmExist();
	}
	inline BOOL IsAlarmExist (void) const
	{
		if ( IsHardwareSimulation() )
			return FALSE;
		return nexioIsServoAlarmEx(m_info.curr.axis);
	}
	inline BOOL IsInitialized (void) const { return m_info.initilized; }
	void Initialize (void);
	void Finalize (void);

	// ADAX専用アラーム
	inline void AlarmSet (unsigned long alid) { nexaxAlarmSet(alid, static_cast<unsigned short>(m_info.curr.axis)); }

	// ADAXメッセージ
	class message_packet : public adax_data_t
	{
	public:
		friend std::ostream& operator << (ostream& os, message_packet& rhs) { return os << setw(14) << setfill('0') << string(rhs.ex).substr(0, 14).c_str(); }

		message_packet () : received(false) { clear(); }
		message_packet (const unsigned short command, const long data)
			: received(false) { clear(); create(command, data); }
		message_packet (const unsigned short command, const short low, const short high)
			: received(false) { clear(); create(command, MAKELONG(low, high)); }
		message_packet (const message_packet* pmsg)
		{
			value = pmsg->value;
			strcpy(buffer, pmsg->buffer);
		}

		operator const char* () { return &buffer[0]; }
		operator	   char* () { return &buffer[0]; }
		message_packet& operator = (const message_packet& msg)
		{
			value = msg.value;
			strcpy(buffer, (const char*) &msg);
			return *this;
		}

		inline size_t length (void) const { return strlen(buffer); }

		inline message_packet& create (const unsigned short command, const long data)
		{
			sprintf(buffer, "%s%s%02X%08X", STX, ADAX_EX, command, data);
			_snprintf(bcc, sizeof(bcc), "%02X", calc_bcc());
			delim = CRCHAR; buffer[MAX_BUFFER_LENGTH] = NULLCHAR;
			analysis();
			return *this;
		}
		inline message_packet& create (const unsigned short command, const char* fmt, ...)
		{
			sprintf(buffer, "%s%s%02X", STX, ADAX_EX, command);

			va_list args;
			va_start(args, fmt);
			_vsnprintf(data, sizeof(data), fmt, args);
			va_end(args);

			_snprintf(bcc, sizeof(bcc), "%02X", calc_bcc());
			delim = CRCHAR; buffer[MAX_BUFFER_LENGTH] = NULLCHAR;
			analysis();
			return *this;
		}
		inline message_packet& create (const unsigned short command) { return create(command, value); }
		inline void analysis (void) { value = hatol(data, 8); }
		inline void clear  (void) { value = 0; buffer[0] = NULLCHAR; }
		inline bool empty  (void) const { return length() == 0; }
		inline unsigned long parse_command (void) { return hatol(command, 2); }
		inline unsigned long error (void) { return hatol(data, 2); }

		// BCC計算
		inline unsigned short calc_bcc (void)
		{
			short retval = 0;
			for ( int i = 1; i < 13; i++ ) {
				retval ^= buffer[i];
			}
			return retval;
		}

		inline bool is_match_bcc (void) { return hatol(bcc, 2) == calc_bcc(); }

	protected:
		inline string substr (const char* s, string::size_type pos, string::size_type n = string::npos) { return string(s).substr(pos, n); }

		// 16進文字列変換
		long hatol (const char* s, unsigned long n) { return hatol(substr(s, 0, n).c_str()); }
		long hatol (const char* s)
		{
			long retval = 0;
			BOOL first	= FALSE;

			struct hex_dec_t {
				char ch;
				long value;
			};
			static const hex_dec_t table [16] =
			{
				{ '0',	0 },	{ '1',	1 },	{ '2',	2 },	{ '3',	3 },
				{ '4',	4 },	{ '5',	5 },	{ '6',	6 },	{ '7',	7 },
				{ '8',	8 },	{ '9',	9 },	{ 'A', 10 },	{ 'B', 11 },
				{ 'C', 12 },	{ 'D', 13 },	{ 'E', 14 },	{ 'F', 15 }
			};

			char* str = _strupr(_strdup(s));
			char* pch = str;

			if ( *pch == '0' && *(pch + 1) == 'X' )
				pch += 2;

			while ( *pch != NULLCHAR ) {
				BOOL found = FALSE;
				for ( int i = 0; i < countof(table); i++ ) {
					if ( *pch == table[i].ch ) {
						if ( !first )
							retval <<= 4;
						retval |= table[i].value;
						found = TRUE;
						break;
					}
				}
				if ( !found ) break;
				pch++;
				first = FALSE;
			}
			free(str);
			return retval;
		}
		union {
			char buffer[MAX_BUFFER_LENGTH + 1];

			struct {
				char stx;
				char ex[2];
				char command[2];
				char data[8];
				char bcc[2];
				char delim;
			};
		};
		bool		  received;
	};

	class params_packet : public message_packet
	{
	public:
		params_packet () : message_packet() {}
		params_packet (const message_packet& msg) : message_packet(msg) {}
		params_packet (const message_packet* pmsg) : message_packet(pmsg) {}

		int compare (const char* s) { return strcmp(s, substr(data, 0, 4).c_str()); }
		inline long tol (string::size_type n) { return atol(c_str(n).c_str()); }
	protected:
		virtual const string c_str (string::size_type n) { return substr(data, 0, n); }
	};
	class jog_packet : public params_packet
	{
	public:
		jog_packet () : params_packet() {}
		jog_packet (short speed) : params_packet() { create(speed); }
		jog_packet (const message_packet& msg) : params_packet(msg) {}
		jog_packet& operator = (const message_packet& msg)
		{
			message_packet::operator = (msg);
			return *this;
		}

		inline message_packet& create (const unsigned short command, const long data) { return message_packet::create(command, data); }
		inline message_packet& create (short speed = 0)
		{
			unsigned char mode = jogStop;
			if ( speed > 0 ) mode = jogForward;
			else
			if ( speed < 0 ) mode = jogReverse;
			return message_packet::create(ADAXCMDID_NORMAL_JOG, "%02hX%04hX%02hX", mode, abs(speed), 0);
		}
		short speed (void) { return static_cast<short>(hatol(substr(data, 2, 4).c_str())); }
		unsigned char mode (void) { return static_cast<unsigned char>(tol(2)); }
		const char* c_str (void)
		{
			static const char* table [4] = { "stop", "fwd", "rev", "unknown" };
			return table[mode()];
		}
		enum JogModeEnum{ jogStop/*stop*/, jogForward/*forward*/, jogReverse/*reverse*/ };
	protected:
		virtual const string c_str (string::size_type n) { return substr(data, 0, n); }
	};
	// 位置決め完了待ち
	inline unsigned long WaitForMotionFinished (void)
	{
		unsigned long retval = NEXAXERR_INVALID_PARAMETER;

		if ( m_pAdaxWaitProc == NULL ) return retval;

		// タイムアウト設定
		nano::basic_timer wait(m_params.MotionTimeouts.TeachLoc);
		while ( 1 ) {
			if ( m_pAdaxWaitProc(m_info.curr.axis) ) {
				// 正常終了 ステータスアイドル移行
				m_info.status = IDLE;
				retval = NEXAXERR_NONE;
				break;
			}
			else
			if ( m_cancel ) {
				// キャンセル処理
				retval = NEXAXERR_CANCELED;
				break;
			}
			else
			if ( IsAlarmExist() ) {
				// ADAXアラーム入力
				AlarmSet(NEXALID_ServoAlarm);
				GetAlarmId();
				retval = NEXAXERR_MOTION_FAILURE;
				break;
			}
			else
			if ( wait.is_timeout() ) {
				// タイムアウトエラー
				AlarmSet(NEXALID_MotionTimeouts);
				retval = NEXAXERR_MOTION_TIMEOUTS;
				break;
			}
			basic_timer::do_events(1L);
		}
		m_pAdaxWaitProc = NULL;

		return retval;
	}
	// 上昇端移動
	inline unsigned long MoveToUpperPos (BOOL wait)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		if ( !MoveToTeachLocation(loc_upper) ) return NEXAXERR_COMMUNICATION_FAILURE;

		if ( !wait )
			m_pAdaxWaitProc = nexioIsPinUpperPosEx;

		return wait ? WaitForMotionFinished() : NEXAXERR_NONE;
	}

	// アライメント移動
	inline unsigned long MoveToAlignmentPos (BOOL wait)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		if ( !MoveToTeachLocation(loc_alignment) ) return NEXAXERR_COMMUNICATION_FAILURE;

		m_pAdaxWaitProc = nexioIsPinAlignmentPosEx;

		return wait ? WaitForMotionFinished() : NEXAXERR_NONE;
	}
	// 下降端移動
	inline unsigned long MoveToLowerPos (BOOL wait)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		if ( !MoveToTeachLocation(loc_lower) ) return NEXAXERR_COMMUNICATION_FAILURE;

		if ( !wait )
			m_pAdaxWaitProc = nexioIsPinLowerPosEx;

		return wait ? WaitForMotionFinished() : NEXAXERR_NONE;
	}

	// 位置決め完了待ち
	inline unsigned long WaitForHomeMotionFinished (void)
	{
		unsigned long retval;

		// タイムアウト設定
		nano::basic_timer wait(m_params.MotionTimeouts.Homing);
		while (1) {
			// 正常終了：ステータスアイドル及び位置決め完了信号ON
			if ( IsAlarmExist() ) {
				// ADAXアラーム入力
				AlarmSet(NEXALID_ServoAlarm);
				GetAlarmId();
				retval = NEXAXERR_MOTION_FAILURE;
				break;
			}
			else
			if ( m_info.status == IDLE && nexioIsPinPosCompEx(m_info.curr.axis) ) {
				retval = NEXAXERR_NONE;
				break;
			}
			else
			if ( wait.is_timeout() ) {
				AlarmSet(NEXALID_MotionTimeouts);
				retval = NEXAXERR_MOTION_TIMEOUTS;
				break;
			}
			basic_timer::do_events(1L);
		}
		// 初期化失敗
		if ( retval != NEXAXERR_NONE )
			AlarmSet(NEXALID_ServoInitializeFailure);

		finalize();

		return retval;
	}
	// 原点復帰
	inline unsigned long MoveToHomePos (void)
	{
		unsigned long retval = NEXAXERR_NONE;

		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		// スレッド開始
		begin();

		return retval;
	}

	// ソフトHP移動
	inline unsigned long MoveToSoftHomePos (void)
	{
		unsigned long retval = NEXAXERR_MOTION_TIMEOUTS;

		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		message_packet msg;
		msg.high = rcmd_softhome;
		msg.create(ADAXCMDID_REMOTE_COMMON);
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;
		return WaitForHomeMotionFinished();
	}
	inline BOOL Cancel (void)
	{
		m_cancel = TRUE;
		return MotionStop() == NEXAXERR_NONE;
	}
	// ソフトウェアバージョン情報
	inline BOOL SoftwareVersionReference (void)
	{
		message_packet msg;
		msg.create(ADAXCMDID_SOFT_VER_REFERENCE);
		return SendMsgWaitAck(msg);
	}
	// ハードウェアバージョン情報(通信確立として用いる)
	inline BOOL HardwareVersionReference (void)
	{
		message_packet msg;
		msg.create(ADAXCMDID_HARD_VER_REFERENCE);
		return SendMsgWaitAck(msg);
	}
	// モード選択
	inline BOOL SelectMode (unsigned char mode)
	{
		message_packet msg;
		msg.create(ADAXCMDID_PARAMS_SET_BEGIN, "FA%02d%04hX", 22, 2);
		if ( !SendMsgWaitAck(msg) ) return FALSE;
		msg.create(ADAXCMDID_PARAMS_SET, "%02hX%06hX", mode, 0);
		if ( !SendMsgWaitAck(msg) ) return FALSE;
		return TRUE;
	}
	// モード取得
	inline BOOL GetCurrMode (void)
	{
		message_packet msg;
		msg.create(ADAXCMDID_PARAMS_REFERENCE_BEGIN, "FA%02d%04hX", 22, 2);
		return SendMsgWaitAck(msg);
	}
#if 0
	inline BOOL MoveToHomePos (void)
	{
		message_packet msg;
		msg.high = rcmd_home;
		msg.create(ADAXCMDID_REMOTE_COMMON);
		return SendMsgWaitAck(msg);
	}
	inline BOOL MoveToSoftHomePos (void)
	{
		message_packet msg;
		msg.high = rcmd_softhome;
		msg.create(ADAXCMDID_REMOTE_COMMON);
		return SendMsgWaitAck(msg);
	}
#endif
	// ジョグ運転
	inline unsigned long NormalJog (short speed)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		jog_packet msg;
		if ( m_mode == FA22_PLS ) {
			msg.create(speed);
		}
		else {
			msg.create(ADAXCMDID_REMOTE_COMMON, MAKELONG(speed, rcmd_normal_jog));
		}
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;
		return NEXAXERR_NONE;
	}
	// パルスジョグ運転
	inline unsigned long PulseJog (long pls)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		message_packet msg;
		msg.value = pls;
		msg.create(ADAXCMDID_REMOTE_PULSE_JOG);
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;
		return NEXAXERR_NONE;
	}
	// 軸移動停止
	inline unsigned long MotionStop (void)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}
#if 0
		message_packet msg;
		msg.high = rcmd_motion_stop;
		msg.create(ADAXCMDID_REMOTE_COMMON);
#else
		jog_packet msg;

		if ( m_mode == FA22_PLS ) {
			msg.create();
		}
		else {
			msg.create(ADAXCMDID_REMOTE_COMMON, MAKELONG(0, rcmd_motion_stop));
		}
#endif
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;
		return NEXAXERR_NONE;
	}
	// 現在値取得
	inline unsigned long GetCurrPos (long& pos)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		message_packet msg;
		if ( m_mode == FA22_PLS ) {
			msg.create(ADAXCMDID_PARAMS_REFERENCE_BEGIN, " d%02d%04hX", 8, 8);
		}
		else {
			msg.high = rcmd_curr_pos;
			msg.create(ADAXCMDID_REMOTE_COMMON, MAKELONG(0, rcmd_curr_pos));
		}
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;

		pos = m_info.curr.pos;

		return NEXAXERR_NONE;
	}
	// モーションパラメータ取得
	unsigned long UploadMotionParams (motion_params_t* params)
	{
		BEGIN_NEXAX_RET_VAL_ERR()
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_upper_pos))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_alignment_pos))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_lower_pos))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_alignment_decel_pos))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_upper_speed))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_alignment_speed))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_lower_speed))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_alignment_decel_speed))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_teach_accel_decel))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_home_speed))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_home_accel_decel))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_softhome_speed))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_detect_range))
			NEXAX_RET_VAL_ERR(EEPromRead(eeprom_jog_speed))
		basic_interface::copymemory(params, &m_motparams);
		END_NEXAX_RET_VAL_ERR()
	}
	// モーションパラメータ設定
	unsigned long DownloadMotionParams (motion_params_t* params)
	{
		basic_interface::copymemory(&m_motparams, params);
		BEGIN_NEXAX_RET_VAL_ERR()
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_upper_pos))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_alignment_pos))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_lower_pos))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_alignment_decel_pos))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_upper_speed))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_alignment_speed))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_lower_speed))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_alignment_decel_speed))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_teach_accel_decel))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_home_speed))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_home_accel_decel))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_softhome_speed))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_detect_range))
			NEXAX_RET_VAL_ERR(EEPromWrite(eeprom_jog_speed))
		END_NEXAX_RET_VAL_ERR()
	}

	unsigned short GetAlarmId (void) { EEPromRead(eeprom_alarm_id); return m_info.alid; }

protected:
	unsigned int start_address (void)
	{
		unsigned long retval = NEXAXERR_MOTION_TIMEOUTS;

		_RPTF1(_CRT_WARN, TEXT("cAdaxServo::ThreadProc - Enter Axis%02d\n"), m_info.curr.axis);

		message_packet msg;
		msg.high = rcmd_home;
		msg.create(ADAXCMDID_REMOTE_COMMON);
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;

		_RPTF1(_CRT_WARN, TEXT("cAdaxServo::ThreadProc - Exit Axis%02d\n"), m_info.curr.axis);

		return retval;
	}
	inline BOOL IsMoveToHomePos (void) const { return is_valid(); }

	inline BOOL Execute (void)
	{
		m_hThread = (HANDLE) _beginthreadex(NULL, 0, ThreadProc, LPVOID(this), 0, &m_nThreadId);
		return m_hThread != NULL;
	}
	inline void Exit (void)
	{
		m_sync.lock();

		m_loop = FALSE;
		if ( m_hThread != NULL ) {
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		m_sync.unlock();
	}
	static UINT WINAPI ThreadProc (LPVOID lpData);
	UINT ThreadProc (void);
	HANDLE m_hThread;
	UINT m_nThreadId;

private:
	inline BOOL IsHardwareSimulation (void) const { return m_params.ADAX.HardwareSimulation; }

	// 位置決め移動
	inline BOOL MoveToTeachLocation (short loc)
	{
		m_cancel = FALSE;
		message_packet msg;
		msg.high = rcmd_teach_loc; msg.low = loc;
		msg.create(ADAXCMDID_REMOTE_COMMON);
		return SendMsgWaitAck(msg, FALSE);
	}

	BOOL ReceiveMsg (void) { return m_Sio.Read(m_msg) == SERR_SUCCESS; }
	BOOL ReceiveAck (void);
	BOOL WaitAck (message_packet& msg = message_packet());
	BOOL SendMsgWaitAck (message_packet msg, BOOL wbit = TRUE);
	BOOL SendMsg (message_packet msg);
	unsigned long ParseCommand (message_packet& msg = message_packet());

	struct adax_error_t {
		unsigned short id;
		const char*    msg;
	};
	// 応答エラーメッセージ
	inline const char* GetErrorMsg (message_packet& msg) const
	{
		static const adax_error_t adax_error_table [] = {
			{	 ADAXERR_PARITY,				"parity error." 			},
			{	 ADAXERR_BCC,					"bcc error."				},
			{	 ADAXERR_FRAMING,				"framing error."			},
			{	 ADAXERR_OVERRUN,				"over run error."			},
			{	 ADAXERR_PROTOCOL,				"protocol error."			},
			{	 ADAXERR_ASCIICODE, 			"ascii code error." 		},
			{	 ADAXERR_BUFFEROVERFLOW,		"receive buffer overflow."	},
			{	 ADAXERR_TIMEOUT,				"receive timeout."			},
			{	 ADAXERR_ABNORMALCMD,			"abnormal command." 		},
			{	 ADAXERR_RUNIMPROPRIETY,		"run impropriety."			},
			{	 ADAXERR_ABNORMALPARAM, 		"abnormal parameter."		},
			{	 ADAXERR_ABNORMALDATASIZE,		"abnormal data size."		},
			{	 ADAXERR_ABNORMALCOMMUNICATE,	"abnormal communicate." 	},
		};

		for ( int index = 0; index < countof(adax_error_table); index++ ) {
			if ( adax_error_table[index].id == msg.error() )
				return adax_error_table[index].msg;
		}
		return "unrecognition error id";
	}

	inline unsigned long EEPromRead (short pid = 0)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		message_packet msg;
		msg.high = rcmd_eeprom_data; msg.low = pid;
		msg.create(ADAXCMDID_REMOTE_COMMON);
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;
		return NEXAXERR_NONE;
	}

	inline unsigned long EEPromWrite (short pid = 0)
	{
		if ( !IsInitialized() ) {
			AlarmSet(NEXALID_ServoNotInitialized);
			return NEXAXERR_NOT_INITIALIZED;
		}
		if ( !IsReady() ) {
			AlarmSet(NEXALID_ServoNotReady);
			return NEXAXERR_NOT_READY;
		}

		message_packet msg;
		msg.high = rcmd_params_item; msg.low = pid;
		msg.create(ADAXCMDID_REMOTE_COMMON);
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;
		switch ( pid ) {
		case eeprom_upper_pos:
		case eeprom_alignment_pos:
		case eeprom_lower_pos:
		case eeprom_alignment_decel_pos:
			msg.value = m_motparams.loc.pos[pid];
			break;
		case eeprom_upper_speed:
			msg.value = m_motparams.loc.speed[loc_upper].value;
			break;
		case eeprom_alignment_speed:
			msg.value = m_motparams.loc.speed[loc_alignment].value;
			break;
		case eeprom_lower_speed:
			msg.value = m_motparams.loc.speed[loc_lower].value;
			break;
		case eeprom_alignment_decel_speed:
			msg.value = m_motparams.loc.speed[loc_alignment_decel].value;
			break;
		case eeprom_teach_accel_decel:
			msg.low  = static_cast<short>(m_motparams.loc.acc * 100.0f);
			msg.high = static_cast<short>(m_motparams.loc.dec * 100.0f);
			break;
		case eeprom_home_speed:
			msg.value = m_motparams.home.speed.value;
			break;
		case eeprom_home_accel_decel:
			msg.low  = static_cast<short>(m_motparams.home.acc * 100.0f);
			msg.high = static_cast<short>(m_motparams.home.dec * 100.0f);
			break;
		case eeprom_softhome_speed:
			msg.value = m_motparams.softhome_speed.value;
			break;
		case eeprom_detect_range:
//			  msg.value = m_motparams.detect_range;
			msg.value = 100L;
			break;
		case eeprom_jog_speed:
			msg.high = static_cast<short>(m_motparams.jog_speed.value);
			break;
		}
		msg.create(ADAXCMDID_REMOTE_DATA);
		if ( !SendMsgWaitAck(msg) ) return NEXAXERR_COMMUNICATION_FAILURE;
		return NEXAXERR_NONE;
	}

	CSio				   m_Sio;				// シリアル通信
	message_packet		   m_msg;				// 受信バッファ
	nano::lfstream		   m_lfs;
	nano::basic_timer	   m_timerT3;
	BOOL				   m_wbit;
	BOOL				   m_loop;
	BOOL				   m_cancel;
	adax_info_t 		   m_info;
	motion_params_t 	   m_motparams;
	mutex				   m_sync;
	unsigned char		   m_mode;
	struct {
		short down; 	  // download params id
		short up;		  // upload params id
	}					   m_pid;

	NEXINIPARAMS		   m_params;
	PFNADAXWAITPROC 	   m_pAdaxWaitProc;
};

#endif // _ADAX_H_
