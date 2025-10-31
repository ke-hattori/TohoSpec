#include <crtdbg.h>
#define __NEXTRA_INTERNAL
#include <NEXMISC.HXX>
#include <NEXIF.HXX>
#include <NEXIOBASE.HXX>
#include <NEXINI.HXX>
#include <NEXAX.HXX>
#include "sio.h"
#include "adax.h"

using namespace nano;

/* added 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- { ---------- */
extern TCHAR g_tszProcDir[_MAX_PATH];		/* 呼出しプロセスのディレクトリ('\'付き)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* 基準ディレクトリ('\'付き)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- } ---------- */
// ===========================================================================
// Constructor/destructor
// ===========================================================================

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
cAdaxServo::cAdaxServo(unsigned short axis)
	: m_wbit(FALSE), m_loop(TRUE), m_mode(FA22_Unknown), m_hThread(NULL), m_pAdaxWaitProc(NULL)
{
	basic_interface::zeromemory (&m_info);
	basic_interface::zeromemory (&m_motparams);
	basic_interface::zeromemory (&m_pid);

	m_info.curr.axis = axis;

	nexiniGetParameter(&m_params);

	char path[MAX_PATH];
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- { ---------- */
//	  sprintf(path, LOG_DIR "NEXAX%02d_.LOG", axis + 1);
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- 			 */
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
	sprintf(path, "%s" LOG_DIR "NEXAX%02d_.LOG", g_tszProcDir, axis + 1);
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexax.dll ---------- } ---------- */
	m_lfs.create(path);

	char name[MAX_PATH];
	sprintf(name, ADAXSYNC_KERNELSTR "%02d", axis + 1);
	m_sync.create(name);
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
cAdaxServo::~cAdaxServo(void)
{
	_RPTF1(_CRT_WARN, "cAdaxServo - Destructor\n", m_info.curr.axis);
}

// ---------------------------------------------------------------------------
// Initialize
// ---------------------------------------------------------------------------
void cAdaxServo::Initialize(void)
{
	// シュミレーションモード判別
	if ( IsHardwareSimulation() ) {
		// シュミレーション時の変数初期化
		m_info.initilized  = TRUE;
		m_info.status	   = IDLE;
		m_info.established = TRUE;
	}
	else
	if ( !m_Sio.Open(m_params.Sio[m_info.curr.axis]) ) {
		// シリアルポートオープン失敗
		AlarmSet(NEXALID_PortInitializeFailure);
	}
	else {
		const int MaxRetries = 3;
			  int Retries	 = 0;
		while (1) {
			// 現在のプログラムモード取得
			// 通信確立
			if ( GetCurrMode() ) {
				// モードが設定と異なる場合は再設定
				if ( m_mode != m_params.System.Mode ) {
					if ( SelectMode(m_params.System.Mode) ) {
						// 再取得
						GetCurrMode();
					}
					else break; 		// 失敗!?
				}
				switch ( m_mode ) {
				case FA22_PLS:			// ジョグモード起動
					// 初期化完了
					m_info.initilized = TRUE;
					m_info.status	  = IDLE;
					break;
				default:
					break;
				}
				Execute();
				m_info.established = TRUE;
				break;
			}
			else
			if ( ++Retries > MaxRetries ) {
				AlarmSet(NEXALID_EstabCommunicationError);
				m_lfs << nano::arrange
					  << nano::datetime
					  << "establish communication retry over";
				m_lfs << endl;
				break;
			}
			else
			if ( !m_loop ) {
				m_lfs << nano::arrange
					  << nano::datetime
					  << "establish communication canceled by user";
				m_lfs << endl;
				break;
			}
			basic_timer::do_events(1L);
		}
	}
}
// ---------------------------------------------------------------------------
// Finalize
// ---------------------------------------------------------------------------
void cAdaxServo::Finalize(void)
{
	// ブレーキ及びサーボオフ
	nexioServoBrakeEx(m_info.curr.axis, OFF);
	nexioServoOnEx(m_info.curr.axis, OFF);

	Exit();
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------

// ===========================================================================
// Helper functions for common processing
// ===========================================================================

// ---------------------------------------------------------------------------
// ThreadProc
// ---------------------------------------------------------------------------
UINT WINAPI cAdaxServo::ThreadProc(LPVOID lpData)
{
	cAdaxServo* pThis = reinterpret_cast<cAdaxServo*>(lpData);

	return pThis->ThreadProc();
}

// ---------------------------------------------------------------------------
// ThreadProc
// ---------------------------------------------------------------------------
UINT cAdaxServo::ThreadProc(void)
{
	// Ack 受信スレッド
	while ( m_loop ) {
		if ( m_wbit ) {
			m_sync.lock();

			WaitAck();

			m_sync.unlock();
		}
		basic_timer::do_events(1L);
	}

	return 0L;
}

// ---------------------------------------------------------------------------
// ReceiveAck
// ---------------------------------------------------------------------------
BOOL cAdaxServo::ReceiveAck(void)
{
	BOOL bRet = FALSE;

	nano::basic_timer timer(m_params.ADAX.TimeoutsT2);
	if ( IsMoveToHomePos() ) timer = m_params.MotionTimeouts.Homing;
	while ( !timer.is_timeout() ) {
		if ( ReceiveMsg() ) {
			bRet = TRUE;
			break;
		}
		Sleep(1L);
	}

	return bRet;
}

// ---------------------------------------------------------------------------
// WaitAck
// ---------------------------------------------------------------------------
BOOL cAdaxServo::WaitAck(message_packet& msg/*=message_packet()*/)
{
	m_wbit = FALSE;

	const int MaxRetries = 3;
		  int Retries	 = 0;

	for (;;) {
		if ( ReceiveAck() )  {
			m_msg.analysis();
			break;
		}
		else
		if ( ++Retries > MaxRetries ) {
			m_lfs << nano::arrange
				  << nano::datetime
				  << "receive ack retry over";
			m_lfs << endl;
			return FALSE;
		}
		else
		if ( m_timerT3.is_timeout() ) {
			m_lfs << nano::arrange
				  << nano::datetime
				  << "receive ack T3 timeout";
			m_lfs << endl;
			return FALSE;
		}
		m_msg.clear();
	}

	if ( ParseCommand(msg) != ADAXERR_NONE ) {
		return FALSE;;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// SendMsg
// 機能：	メッセージ送信処理
// 引数：	msg - 送信メッセージ
//			wbit - Ack 受信待ちフラグ
//
// 戻り値： BOOL - 成功
//
// Notes:	wbit = FALSEの場合はAck 受信スレッドにてAck待ち
// ---------------------------------------------------------------------------
BOOL cAdaxServo::SendMsgWaitAck(message_packet msg, BOOL wbit/*=TRUE*/)
{
	BOOL bRet = TRUE;

	// シュミレーションモード時は成功
	if ( IsHardwareSimulation() ) return TRUE;

	m_sync.lock();

	if ( !SendMsg(msg) ) {
		m_lfs << nano::arrange
			  << nano::datetime
			  << msg
			  << SPACE
			  << "send message failed"
			  << endl;
		goto ERR;
	}

	// Ackは受信スレッドにて待つ
	if ( !wbit ) {
		m_wbit = TRUE;
		goto UNLOCK;
	}

	// Ack受信
	if ( WaitAck(msg) ) goto UNLOCK;

ERR:
	bRet = FALSE;
	m_lfs.close();
	if ( m_info.established )
		AlarmSet(NEXALID_CommunicationFailure);

UNLOCK:
	m_sync.unlock();

	return bRet;
}

// ---------------------------------------------------------------------------
// SendMsg
// ---------------------------------------------------------------------------
BOOL cAdaxServo::SendMsg(message_packet msg)
{
	BOOL bRet = FALSE;

	m_info.connect = ONLINE;

	if ( m_Sio.Write((PCSTR) msg) == SERR_SUCCESS ) bRet = TRUE;

	m_timerT3.set(m_params.ADAX.TimeoutsT3);

	/*
	  原点復帰移動時のタイムアウト設定
	  原点復帰時は完了までAck応答が無い
	*/
	if ( IsMoveToHomePos() ) m_timerT3 = m_params.MotionTimeouts.Homing;

	m_lfs << nano::arrange
		  << nano::datetime
		  << "NEXTRA -> ADAX "
		  << msg
		  << SPACE;

	// 送信コマンド解析
	switch ( msg.parse_command() ) {
	case ADAXCMDID_PARAMS_SET_BEGIN:
		m_lfs << "specified parameter set begin : FA-22";
		break;
	case ADAXCMDID_PARAMS_SET:
		switch ( params_packet(msg).tol(2) ) {
		case FA22_PLS:
			m_lfs << "selection position command : PLS(0)";
			break;
		case FA22_Pro:
			m_lfs << "selection position command : Pro(1)";
			break;
		case FA22_oP:
			m_lfs << "selection position command : oP(2)";
			break;
		}
		break;
	case ADAXCMDID_PARAMS_REFERENCE_BEGIN:
		if ( params_packet(msg).compare(" d08") == 0 )
			m_lfs << "get current position";
		else
			m_lfs << "specified parameter reference begin : FA-22";
		break;
	case ADAXCMDID_NORMAL_JOG:
		{
			// ジョグモード専用処理
			jog_packet pkt = jog_packet(msg);
			switch ( pkt.mode() ) {
			case jog_packet::jogStop:
				// ブレーキ
				nexioServoBrakeEx(m_info.curr.axis, OFF);
				m_info.status = IDLE;
				break;
			case jog_packet::jogForward:
			case jog_packet::jogReverse:
				// ブレーキ解除
				nexioServoBrakeEx(m_info.curr.axis, ON);
				m_info.status = RUN;
				break;
			}
			m_lfs << "jogging operation("
				  << pkt.c_str()
				  << COMMACHAR
				  << pkt.speed()
				  << ")";
		}
		break;
	case ADAXCMDID_SOFT_VER_REFERENCE:
		m_lfs << "software version reference";
		break;
	case ADAXCMDID_HARD_VER_REFERENCE:
		m_lfs << "hardware version reference";
		break;
	case ADAXCMDID_REMOTE_COMMON:
		switch ( msg.high ) {
		case rcmd_teach_loc:
			m_info.status = RUN;
			m_lfs << "move absolute["
				  << eeprom_params_names[msg.low]
				  << "]";
			break;
		case rcmd_home:
			m_info.status = RUN;
			m_lfs << "homing";
			break;
		case rcmd_softhome:
			m_info.status = RUN;
			m_lfs << "soft home position";
			break;
		case rcmd_normal_jog:
			m_info.status = RUN;
			m_lfs << "jogging operation[speed:"
				  << msg.low
				  << "]";
			break;
		case rcmd_motion_stop:
			m_lfs << "motion stop";
			break;
		case rcmd_params_item:
			m_pid.down = msg.low;
			m_lfs << "set parameters";
			break;
		case rcmd_curr_pos:
			m_lfs << "get current position";
			break;
		case rcmd_eeprom_data:
			m_pid.up = msg.low;
			m_lfs << "get "
				  << eeprom_params_names[msg.low];
			break;
		case rcmd_alarm_reset:
			m_lfs << "alarm reset";
			break;
		}
		break;
	case ADAXCMDID_REMOTE_DATA:
		// サーボパラメータ設定
		switch ( m_pid.down ) {
		case eeprom_upper_pos:
		case eeprom_alignment_pos:
		case eeprom_lower_pos:
		case eeprom_alignment_decel_pos:
			m_lfs << "set "
				  <<  eeprom_params_names[m_pid.down]
				  << "teaching data["
				  << setw(12)
				  << setfill('0')
				  << showpos
				  << msg.value
				  <<  " pls]";
			break;
		case eeprom_upper_speed:
			m_lfs << "set "
				  << eeprom_params_names[m_pid.down]
				  << "[alignment >> upper : "
				  << setw(4)
				  << setfill('0')
				  << showpos
				  << msg.low
				  << ", lower >> upper : "
				  << msg.high
				  << "]";
			break;
		case eeprom_alignment_speed:
			m_lfs << "set "
				  << eeprom_params_names[m_pid.down]
				  << "[lower >> alignment : "
				  << setw(4)
				  << setfill('0')
				  << showpos
				  << msg.low
				  << ", upper >> alignment : "
				  << msg.high
				  << "]";
			break;
		case eeprom_lower_speed:
			m_lfs << "set "
				  << eeprom_params_names[m_pid.down]
				  << "[alignment >> lower : "
				  << setw(4)
				  << setfill('0')
				  << showpos
				  << msg.low
				  << ", upper >> lower : "
				  << msg.high
				  << "]";
			break;
		case eeprom_alignment_decel_speed:
			m_lfs << "set "
				  << eeprom_params_names[m_pid.down]
				  << setw(4)
				  << setfill('0')
				  << showpos
				  << " : "
				  << msg.high;
			break;
		case eeprom_teach_accel_decel:
		case eeprom_home_accel_decel:
			m_lfs << "set "
				  << eeprom_params_names[m_pid.down]
				  << "[acc:"
				  << setprecision(2)
				  << msg.low / 100.0f
				  << "(s), dec:"
				  << msg.high  / 100.0f
				  << "(s)]";
			break;
		case eeprom_home_speed:
			m_lfs << "set "
				  << eeprom_params_names[m_pid.down]
				  << "fwd : "
				  << setw(4)
				  << setfill('0')
				  << showpos
				  << msg.low
				  << ", rev : "
				  << msg.high
				  << "]";
			break;
		case eeprom_softhome_speed:
		case eeprom_detect_range:
		case eeprom_jog_speed:
			m_lfs << "set "
				  << eeprom_params_names[m_pid.down]
				  << "["
				  << setw(4)
				  << setfill('0')
				  << msg.high
				  << "]";
			break;
		case eeprom_io_monitor:
			m_lfs << "i/o monitor";
			break;
		case eeprom_status:
			m_lfs << "servo status";
			break;
		}
		break;
	case ADAXCMDID_REMOTE_PULSE_JOG:
		m_info.status = RUN;
		m_lfs << "pulse jogging operation[feed pulse:"
			  << setw(12)
			  << setfill('0')
			  << showpos
			  << msg.value
			  << "]";
		break;
	default:
		break;
	}
	m_lfs << endl;

	return bRet;
}

// ---------------------------------------------------------------------------
// ParseEvent
// ---------------------------------------------------------------------------
unsigned long cAdaxServo::ParseCommand(message_packet& msg/*=message_packet()*/)
{
	m_wbit = FALSE;

	// BCCチェック
	if ( !m_msg.is_match_bcc() ) {
		m_lfs << nano::arrange
			  << nano::datetime
			  << "NEXTRA <- ADAX "
			  << "bcc unmatch error";
		m_lfs << endl;
		return ADAXERR_BCC;
	}

	m_lfs << nano::arrange
		  << nano::datetime
		  << "NEXTRA <- ADAX "
		  << m_msg
		  << SPACE;

	// サーボ応答メッセージ解析
	switch ( m_msg.parse_command() ) {
	case ADAXCMDID_RESPONSE_OK:
		m_lfs << "ack";
		break;
	case ADAXCMDID_RESPONSE_NG:
		m_lfs << GetErrorMsg(m_msg);
		break;
	case ADAXCMDID_PARAMS_SET:
		m_lfs << "selection position command ack";
		break;
	case ADAXCMDID_PARAMS_REFERENCE_BEGIN:
		if ( params_packet(msg).compare("FA22") == 0 ) {
			m_mode = m_msg.value;
			m_lfs << "FA-22="
				  << m_msg.value;
		}
		else {
			m_info.curr.pos = m_msg.value;
			m_lfs << "current position["
				  << setw(12)
				  << setfill('0')
				  << showpos
				  << m_info.curr.pos
				  <<  "]";
		}
		break;
	case ADAXCMDID_SOFT_VER_REFERENCE:
		m_info.established = TRUE;
		m_lfs << "software version reference ack";
		break;
	case ADAXCMDID_HARD_VER_REFERENCE:
		m_info.established = TRUE;
		m_lfs << "hardware version reference ack";
		break;
	case ADAXCMDID_ACK:
		switch ( m_msg.high ) {
		case rcmd_teach_loc:
		case rcmd_normal_jog:
			break;
		case rcmd_softhome:
		case rcmd_home:
			m_info.initilized = TRUE;
			m_info.status	  = IDLE;
			break;
		case rcmd_motion_stop:
			m_info.status = IDLE;
			break;
		}
		m_lfs << "ack";
		break;
	case ADAXCMDID_CURRENT_POS:
		m_info.curr.pos = m_msg.value;
		m_lfs << "current position["
			  << setw(12)
			  << setfill('0')
			  << showpos
			  << m_info.curr.pos
			  <<  "]";
		break;
	case ADAXCMDID_REFERENCE:
		switch ( m_pid.up ) {
		case eeprom_upper_pos:
		case eeprom_alignment_pos:
		case eeprom_lower_pos:
		case eeprom_alignment_decel_pos:
			m_motparams.loc.pos[m_pid.up] = m_msg.value;
			m_lfs <<  eeprom_params_names[m_pid.up]
				  << "teaching data["
				  << setw(12)
				  << setfill('0')
				  << showpos
				  << m_msg.value
				  <<  " pls]";
			break;
		case eeprom_upper_speed:
			m_motparams.loc.speed[loc_upper].value = m_msg.value;
				m_lfs <<eeprom_params_names[m_pid.up]
					  << "[alignment >> upper : "
					  << setw(4)
					  << setfill('0')
					  << showpos
					  << m_msg.low
					  << ", lower >> upper : "
					  << m_msg.high
					  << "]";
			break;
		case eeprom_alignment_speed:
			m_motparams.loc.speed[loc_alignment].value = m_msg.value;
			m_lfs << eeprom_params_names[m_pid.up]
				  << "[lower >> alignment : "
				  << setw(4)
				  << setfill('0')
				  << showpos
				  << m_msg.low
				  << ", upper >> alignment : "
				  << m_msg.high
				  << "]";
			break;
		case eeprom_lower_speed:
			m_motparams.loc.speed[loc_lower].value = m_msg.value;
			m_lfs << eeprom_params_names[m_pid.up]
				  << "[alignment >> lower : "
				  << setw(4)
				  << setfill('0')
				  << showpos
				  << m_msg.low
				  << ", upper >> lower : "
				  << m_msg.high
				  << "]";
			break;
		case eeprom_alignment_decel_speed:
			m_motparams.loc.speed[loc_alignment_decel].value = m_msg.value;
			m_lfs << eeprom_params_names[m_pid.up]
				  << setw(4)
				  << setfill('0')
				  << showpos
				  << " : "
				  << m_msg.high;
			break;
		case eeprom_teach_accel_decel:
			m_motparams.loc.acc = m_msg.low / 100.0f;
			m_motparams.loc.dec = m_msg.high / 100.0f;
			m_lfs << eeprom_params_names[m_pid.up]
				  << "[acc:"
				  << setprecision(2)
				  << m_msg.low / 100.0f
				  << "(s), dec:"
				  << m_msg.high / 100.0f
				  << "(s)]";
			break;
		case eeprom_home_speed:
			m_motparams.home.speed.value = m_msg.value;
			break;
		case eeprom_home_accel_decel:
			m_motparams.home.acc = m_msg.low / 100.0f;
			m_motparams.home.dec = m_msg.high / 100.0f;
			m_lfs << eeprom_params_names[m_pid.up]
				  << "[acc:"
				  << setprecision(2)
				  << m_msg.low / 100.0f
				  << "(s), dec:"
				  << m_msg.high  / 100.0f
				  << "(s)]";
			break;
		case eeprom_softhome_speed:
			m_motparams.softhome_speed.value = m_msg.value;
			m_lfs << eeprom_params_names[m_pid.up]
				  << "["
				  << setw(4)
				  << setfill('0')
				  << m_msg.high
				  << "]";
			break;
		case eeprom_detect_range:
			m_motparams.detect_range = m_msg.low;
			m_lfs << eeprom_params_names[m_pid.up]
				  << "["
				  << setw(4)
				  << setfill('0')
				  << m_msg.high
				  << "]";
			break;
		case eeprom_jog_speed:
			m_motparams.jog_speed.value = m_msg.high;
			m_lfs << eeprom_params_names[m_pid.up]
				  << "["
				  << setw(4)
				  << setfill('0')
				  << m_msg.high
				  << "]";
			break;
		case eeprom_alarm_id:
			m_lfs << eeprom_params_names[m_pid.down]
				  << "["
				  << setw(3)
				  << setfill('0')
				  << m_msg.high
				  << "]";
				  m_info.alid = m_msg.high;
			break;
		case eeprom_io_monitor:
			m_lfs << "input["
				  << setw(4)
				  << hex
				  << uppercase
				  << m_msg.low
				  << "] : output["
				  << m_msg.high
				  << "]";
			break;
		case eeprom_status:
			m_lfs << "homing is"
				  << m_msg.high == 0 ? "completion" : "not completion";
			break;
		}
		break;
	default:
		break;
	}
	m_lfs << endl;

	return ADAXERR_NONE;
}
