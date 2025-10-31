#if !defined( _COORD_HPP_ )
#define _COORD_HPP_

#pragma once

#include <active.hxx>
#include <nexmisc.hxx>
#include <nexif.hxx>
#include <algorithm>
#include <functional>

//#define __STATEPROC_DEBUG
/* added 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- { ---------- */
#ifdef NEXPROC_MAS
    #define NEXPROC_EXT
#else
    #define NEXPROC_EXT extern
#endif
NEXPROC_EXT TCHAR g_tszLogPath[_MAX_PATH]
#ifdef NEXPROC_MAS
= _T("")
#endif
;
extern TCHAR g_tszProcDir[_MAX_PATH];       /* 呼出しプロセスのディレクトリ('\'付き)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];       /* 基準ディレクトリ('\'付き)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- } ---------- */

// #########################################################################
// DEFINITIONS
// #########################################################################
typedef void (__stdcall *PFNEXECPROC)   (BOOL);
typedef BOOL (__stdcall *PFNWAITPROC)   (void);
typedef void (__stdcall *PFNEXECPROCEX) (unsigned short, BOOL);
typedef BOOL (__stdcall *PFNWAITPROCEX) (unsigned short);

// ----------------------------------------------------------------------------
// Macro DECLARE_STATEPROC_MAP
// ----------------------------------------------------------------------------
#ifdef __STATEPROC_DEBUG
#define STATEPROC_LOGFILE_NAME (LOG_DIR "NEXCOORD.LOG")
#define DECLARE_STATEPROC_MAPEX(class_name) \
private: \
    typedef void (class_name::*P_STATE_PROC)(const cEventParams* const); \
    class STATE_PROC_DEF \
    { \
    public: \
        int             state; \
        char*           state_descr; \
        P_STATE_PROC    proc; \
    }; \
    static STATE_PROC_DEF   StateProcMap[]; \
    static int              nStateProcMap; \
protected: \
    virtual void EnterState (const cEventParams* const params); \
    int CheckStateProcMap(void);
#define END_STATEPROC_MAPEX END_STATEPROC_MAP

// ----------------------------------------------------------------------------
// Macro BEGIN_STATEPROC_MAP (debug and release versions)
// ----------------------------------------------------------------------------
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- { ---------- */
//#define BEGIN_STATEPROC_MAPEX(class_name) \
//void class_name::EnterState (const cEventParams* const params) \
//{ \
//    nano::lfstream state_stream(STATEPROC_LOGFILE_NAME); \
//    if (CurrState >= 0 && CurrState < nStateProcMap) \
//    { \
//        if (StateProcMap[CurrState].state_descr != NULL) \
//        { \
//            state_stream << nano::arrange \
//                         << nano::datetime \
//                         << #class_name \
//                         << StateProcMap[CurrState].state_descr; \
//        } \
//        if (StateProcMap[CurrState].proc != NULL) \
//            (this->*(StateProcMap[CurrState].proc))(params); \
//    } \
//} \
//class_name::STATE_PROC_DEF class_name::StateProcMap[] = \
//{
//#else
//#define DECLARE_STATEPROC_MAPEX DECLARE_STATEPROC_MAP
//#define BEGIN_STATEPROC_MAPEX BEGIN_STATEPROC_MAP
//#define END_STATEPROC_MAPEX END_STATEPROC_MAP
//#endif // __STATEPROC_DEBUG
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ----------              */
#define BEGIN_STATEPROC_MAPEX(class_name) \
void class_name::EnterState (const cEventParams* const params) \
{ \
    nano::lfstream state_stream(g_tszLogPath); \
    if (CurrState >= 0 && CurrState < nStateProcMap) \
    { \
        if (StateProcMap[CurrState].state_descr != NULL) \
        { \
            state_stream << nano::arrange \
                         << nano::datetime \
                         << #class_name \
                         << StateProcMap[CurrState].state_descr; \
        } \
        if (StateProcMap[CurrState].proc != NULL) \
            (this->*(StateProcMap[CurrState].proc))(params); \
    } \
} \
class_name::STATE_PROC_DEF class_name::StateProcMap[] = \
{
#else
#define DECLARE_STATEPROC_MAPEX DECLARE_STATEPROC_MAP
#define BEGIN_STATEPROC_MAPEX BEGIN_STATEPROC_MAP
#define END_STATEPROC_MAPEX END_STATEPROC_MAP
#endif // __STATEPROC_DEBUG
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- } ---------- */

// =========================================================================
// class : cCoord
// =========================================================================
class basic_coord
{
public:
    basic_coord (void) : m_run(FALSE), m_cancel(FALSE) { nexiniGetParameter(&m_params); }
    BOOL is_cancel (void) const { return m_cancel; }
    virtual void cancel (void) { m_cancel = TRUE; }
    virtual void ready (void) { m_cancel = FALSE; }
    virtual void run (void) { m_run = TRUE; }
    virtual void done (void) { m_run = FALSE; }
    virtual BOOL is_idle (void) const { return !m_run; }
    virtual BOOL is_valid (void) const { return TRUE; }
    virtual void emergency (void) {}
    virtual void finalize (void) {}
    virtual void reset (void) {}
protected:
    BOOL         m_cancel;
    BOOL         m_run;
    NEXINIPARAMS m_params;
};
class cCoord : public basic_coord, protected nano::thread
{
public:
    // Constructor/destructor
    cCoord (void);
    virtual ~ cCoord (void);

    // ホストイベント結果通知
	inline BOOL PostClientMessage (void) { return PostClientMessage(client.hwnd, client.event, client.result); }
	inline BOOL PostClientMessage (HWND hwnd, WPARAM wp, LPARAM lp = NEXIF_OK)
    {
		m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
        if ( hwnd != NULL ) {
			m_lfs << arrange << datetime << setw(10) << setfill('0') << hex << showbase << "hwnd(" << hwnd << "), wp(" << wp << "), lp(" << lp << ")" << std::endl;
            return ::PostMessage(hwnd, WM_NEX_RESPONSE, wp, lp);
        }
        return FALSE;
    }

    // Action requests from client
    virtual LRESULT Initialize (void) { return 0L; }
    virtual void Finalize (void)
    {
        Cancel();

        list<basic_coord*>::iterator iter(m_list.begin());
        for ( ; iter != m_list.end(); ++iter ) {
            (*iter)->finalize();
        }
    }

    inline void ReturnToOrigin  (HWND hwnd) { Execute(hwnd, nexReturnToOrigin);  }
    inline void Load            (HWND hwnd) { Execute(hwnd, nexLoad);            }
    inline void Unload          (HWND hwnd) { Execute(hwnd, nexUnload);          }
    inline void MoveToUpper     (HWND hwnd) { Execute(hwnd, nexMoveToUpper);     }
    inline void MoveToAlignment (HWND hwnd) { Execute(hwnd, nexMoveToAlignment); }
    inline void MoveToLower     (HWND hwnd) { Execute(hwnd, nexMoveToLower);     }
    inline void OpenWorkGuide   (HWND hwnd) { Execute(hwnd, nexOpenWorkGuide);   }
    inline void CloseWorkGuide  (HWND hwnd) { Execute(hwnd, nexCloseWorkGuide);  }
    inline void OpenShutter     (HWND hwnd) { PostClientMessage(hwnd, nexOpenShutter, m_shutter->Open());          }
    inline void CloseShutter    (HWND hwnd) { PostClientMessage(hwnd, nexCloseShutter, m_shutter->Close());        }
    inline void VacuumOn        (HWND hwnd) { PostClientMessage(hwnd, nexVacuumOn, m_vacuum->ChuckValve());        }
    inline void VacuumOff       (HWND hwnd) { PostClientMessage(hwnd, nexVacuumOff, m_vacuum->UnchuckValve());     }
    inline void AirFloatOn      (HWND hwnd) { PostClientMessage(hwnd, nexAirFloatOn, m_airfloat->ChuckValve());    }
    inline void AirFloatOff     (HWND hwnd) { PostClientMessage(hwnd, nexAirFloatOff, m_airfloat->UnchuckValve()); }
    inline void RC_MoveToTeachPos (HWND hwnd, unsigned char num)
    {
        if ( m_params.Options.RoboCylinder )
            PostClientMessage(hwnd, nexRC_MoveToTeachPos, m_robo->MoveToTeachPos(num));
    }
    inline void RC_JogPlus (HWND hwnd)
    {
        if ( m_params.Options.RoboCylinder )
            PostClientMessage(hwnd, nexRC_JogPlus, m_robo->JogPlus());
    }
    inline void RC_JogMinus (HWND hwnd)
    {
        if ( m_params.Options.RoboCylinder )
            PostClientMessage(hwnd, nexRC_JogMinus, m_robo->JogMinus());
    }
    inline void RC_JogStop (HWND hwnd)
    {
        if ( m_params.Options.RoboCylinder )
            PostClientMessage(hwnd, nexRC_JogStop, m_robo->JogStop());
    }
    inline void RC_SelectMode (HWND hwnd, unsigned char mode)
    {
        if ( m_params.Options.RoboCylinder )
            PostClientMessage(hwnd, nexRC_SelectMode, m_robo->SelectMode(mode));
    }
    virtual void Cancel (void)
    {
        cancel();
        list<basic_coord*>::iterator iter(m_list.begin());
        for ( ; iter != m_list.end(); ++iter ) {
            (*iter)->cancel();
        }
    }

    // Status inquiries
    virtual BOOL IsIdle (void) const { return FALSE; }
    BOOL IsInitialized (void) const { return m_initialized; }

    virtual BOOL DoClampWaitForFinished (PFNEXECPROCEX pExecProcEx, PFNWAITPROCEX pWaitProcEx, unsigned short num, unsigned long msec)
    {
        BOOL ret = FALSE;

        pExecProcEx(num, ON);

        nano::basic_timer wait(msec);
        // キャンセルされるまでループ
        while ( !is_cancel() ) {
            if ( pWaitProcEx(num) ) {
                ret = TRUE;
                break;
            }
            else
            if ( wait.is_timeout() ) {
                nexifAlarmSet(NEXALID_ClampTimeouts);
                break;
            }
            basic_timer::do_events(1L);
        }

        pExecProcEx(num, OFF);

        return ret;
    }
/*
 * バキューム制御クラス
*/
    struct sample_size_t {
        double x;
        double y;
    };
    typedef vector<unsigned short> valve_list;
    struct sample_map_t {
        sample_size_t size;
        valve_list    valve;
    };
    typedef vector<sample_map_t> vacuum_list;
    class cVacuum : public basic_coord, protected basic_interface
    {
    public:
        cVacuum (void) { Initialize(); }
        virtual ~ cVacuum (void) {}

        // バキュームオン
        virtual BOOL ChuckValve (BOOL wbit = TRUE)
        {
            BOOL retval = FALSE;

            ready();

            // サンプル更新
            GetSample();

            // 電磁弁リストが空っぽ
            if ( m_valve.empty() ) return retval;
            valve_list::iterator iter(m_valve.begin());
            // エアー浮上オフ
            for ( ; iter != m_valve.end(); ++iter ) {
                nexioAirFloatEx(*iter, OFF);
            }
            // 電磁弁オン
            for ( iter = m_valve.begin(); iter != m_valve.end(); ++iter ) {
                nexioVacuumOnEx(*iter, ON);
            }
            // 吸着確認を行うか？
            if ( !wbit ) return TRUE;
            basic_timer wait(m_params.MotionTimeouts.VacuumOn);
            // キャンセルされるまでループ
            while ( !is_cancel() ) {
                if ( nexioIsVacuumOn() ) {
                    retval = TRUE;
                    break;
                }
                else
                if ( wait.is_timeout() ) {
                    nexifAlarmSet(NEXALID_VacuumOnTimeouts);
                    break;
                }
                basic_timer::do_events(1L);
            }
            return retval;
        }
        // 非常停止時は電磁弁オフ
        virtual void emergency (void)
        {
            if ( !m_valve.empty() ) {
                valve_list::iterator iter(m_valve.begin());
                for ( iter = m_valve.begin(); iter != m_valve.end(); ++iter ) {
                    nexioVacuumOnEx(*iter, OFF);
                }
            }
        }
        virtual BOOL UnchuckValve (BOOL wbit = TRUE)
        {
            BOOL retval = FALSE;

            ready();

            // サンプル更新
            GetSample();

            // 電磁弁リストが空っぽ
            if ( m_valve.empty() ) return retval;
            valve_list::iterator iter(m_valve.begin());
            // 電磁弁オフ
            for ( ; iter != m_valve.end(); ++iter ) {
                nexioVacuumOnEx(*iter, OFF);
            }
            // 破壊電磁弁オン
            for ( iter = m_valve.begin(); iter != m_valve.end(); ++iter ) {
                nexioVacuumDestructEx(*iter, ON);
            }
            // 破壊遅延処理
            basic_timer timer;
            timer.delay(m_params.MotionDelay.VacuumDestruct);
            for ( iter = m_valve.begin(); iter != m_valve.end(); ++iter ) {
                nexioVacuumDestructEx(*iter, OFF);
            }
            // 確認を行うか？
            if ( !wbit ) return TRUE;
            basic_timer wait(m_params.MotionTimeouts.VacuumOff);
            // キャンセルされるまでループ
            while ( !is_cancel() ) {
                if ( nexioIsVacuumOff() ) {
                    retval = TRUE;
                    break;
                }
                else
                if ( wait.is_timeout() ) {
                    nexifAlarmSet(NEXALID_VacuumOffTimeouts);
                    break;
                }
                basic_timer::do_events(1L);
            }
            return retval;
        }

        virtual void Initialize (void)
        {
            // バキューム設定ファイル読み込み
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- { ---------- */
//            ifstream ifs(VACUUM_CFGFILE_NAME);
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ----------              */
            GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
            TCHAR l_tszVacCfgFPath[_MAX_PATH];
            _stprintf(l_tszVacCfgFPath, _T("%s") VACUUM_CFGFILE_NAME, g_tszProcDir);
            ifstream ifs(l_tszVacCfgFPath);
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexproc.dll ---------- } ---------- */
            string line;
            while ( std::getline(ifs, line) ) {
                istringstream iss(line);
                string token;
                sample_map_t sample;
                if ( std::getline(iss, token, COMMACHAR) )
                    sample.size.x = atof(token.c_str());
                if ( std::getline(iss, token, COMMACHAR) )
                    sample.size.y = atof(token.c_str());
                while ( std::getline(iss, token, COMMACHAR) )
                    sample.valve.push_back(atoi(token.c_str()) - 1);
                m_list.push_back(sample);
            }
        }

    protected:
        virtual void GetSample (void)
        {
            if ( m_list.empty() ) return;
            sample_size_t size;
            // 現在サンプルサイズ取得
            nexifGetSampleSize(&size.x, &size.y);
            m_valve.clear();
            vacuum_list::iterator iter(m_list.begin());
            for ( ; iter != m_list.end(); ++iter ) {
                if ( comparememory(&size, &iter->size) == 0 ) {
                    m_valve = iter->valve;
                    break;
                }
            }
        }
        vacuum_list     m_list;
        valve_list      m_valve;
    }; // cVacuum

/*
 * エア浮上制御クラス
*/
    class cAirFloat : public cVacuum
    {
    public:
        cAirFloat (void) : cVacuum() {}
        virtual ~ cAirFloat (void) {}
        virtual BOOL is_valid (void) const { return m_params.Options.AirFloat; }

        virtual BOOL ChuckValve (BOOL wbit = TRUE)
        {
            if ( !is_valid() ) return FALSE;

            // サンプル更新
            GetSample();

            if ( m_valve.empty() ) return FALSE;
            valve_list::iterator iter(m_valve.begin());
            // バキュームOFF
            for ( ; iter != m_valve.end(); ++iter ) {
                nexioVacuumOnEx(*iter, OFF);
            }
            // Air浮上
            for ( iter = m_valve.begin(); iter != m_valve.end(); ++iter ) {
                nexioAirFloatEx(*iter, ON);
            }
            return TRUE;
        }
        virtual void emergency (void)
        {
            if ( !m_valve.empty() && is_valid() ) {
                valve_list::iterator iter(m_valve.begin());
                for ( iter = m_valve.begin(); iter != m_valve.end(); ++iter ) {
                    nexioAirFloatEx(*iter, OFF);
                }
            }
        }
        virtual BOOL UnchuckValve (BOOL wbit = TRUE)
        {
            if ( !is_valid() ) return FALSE;

            // サンプル更新
            GetSample();

            if ( m_valve.empty() ) return FALSE;
            valve_list::iterator iter(m_valve.begin());
            // Air浮上
            for ( ; iter != m_valve.end(); ++iter ) {
                nexioAirFloatEx(*iter, OFF);
            }
            return TRUE;
        }
    }; // cAirFloat

/*
 * シャッター制御クラス
*/
    class cShutter : public basic_coord
    {
    public:
        cShutter (void) {}
        virtual ~ cShutter (void) {}
        virtual BOOL is_valid (void) const { return m_params.Options.Shutter; }
        virtual void cancel (void)
        {
            if ( m_params.Shutter.Cylinder == CYLINDER_3POS )
                m_cancel = TRUE;
        }

        inline BOOL Open (void)
        {
            BOOL retval = FALSE;

            if ( !is_valid() ) return retval;

            ready();
            basic_timer wait(m_params.MotionTimeouts.ShutterOpen);
            if ( m_params.Shutter.Cylinder == CYLINDER_3POS ) {
                nexioShutterCloseEx(OFF);
            }
            nexioShutterOpenEx(ON);
            // キャンセルされるまでループ
            while ( !is_cancel() ) {
                if ( nexioIsShutterOpen() ) {
                    retval = TRUE;
                    break;
                }
                else
                if ( wait.is_timeout() ) {
                    nexifAlarmSet(NEXALID_ShutterOpenTimeouts);
                    break;
                }
                //basic_timer::do_events(1L);
                Sleep(10);
            }
            if ( m_params.Shutter.Cylinder == CYLINDER_2POS || is_cancel() ) {
                nexioShutterOpenEx(OFF);
            }
            return retval;
        }

        inline BOOL Close (void)
        {
            BOOL retval = FALSE;

            if ( !is_valid() ) return retval;

            ready();
            basic_timer wait(m_params.MotionTimeouts.ShutterClose);
            if ( nexioIsRobotArmDetect() ) {
                if ( m_params.Shutter.Cylinder == CYLINDER_3POS ) {
                    nexioShutterOpenEx(OFF);
                }
                nexioShutterCloseEx(ON);
                // キャンセルされるまでループ
                while ( !is_cancel() ) {
                    // ロボットアーム検出
                    if ( !nexioIsRobotArmDetect() ) {
                        nexifAlarmSet(NEXALID_RobotArmDetected);
                        // オープン処理移行
                        Open();
                        break;
                    }
                    else
                    if ( wait.is_timeout() ) {
                        nexifAlarmSet(NEXALID_ShutterCloseTimeouts);
                        break;
                    }
                    else
                    if ( nexioIsShutterClose() ) {
                        retval = TRUE;
                        break;
                    }
                    //basic_timer::do_events(1L);
					Sleep(10);
                }
                if ( m_params.Shutter.Cylinder == CYLINDER_2POS || is_cancel() ) {
                    nexioShutterCloseEx(OFF);
                }
            }
            return retval;
        }
        // 非常停止時は電磁弁オフ
        inline void emergency (void)
        {
            nexioShutterOpenEx(OFF);
            nexioShutterCloseEx(OFF);
        }
    }; // cShutter

/*
 * ロボシリンダー制御クラス
*/
    class cRoboCylinder : public basic_coord
    {
    public:
        cRoboCylinder (void) {}
        virtual ~ cRoboCylinder (void) {}
        virtual BOOL is_valid (void) const { return m_params.Options.Shutter; }
        virtual void finalize (void) { ServoOff(); }

        inline BOOL Initialize (void)
        {
            BOOL retval = FALSE;

            if ( ServoOn() ) {
                retval = MoveToHomePos() && MoveToTeachPos(m_params.RoboCylinder.InitialPosNo);
            }

            // 下端位置センサーを原点復帰完了後に確認
            return retval && nexioIsMS_LowerPos1() && nexioIsMS_LowerPos2();
        }
        inline BOOL JogPlus (void)
        {
            BOOL retval = FALSE;

            if ( !nexioIsRC_MODES() ) {
                return retval;
            }
            run();

            nexioRC_JOGM(OFF);
            // ↓ボタン押下中なら出力
            if ( nexioIsMS_DownSwitch() ) {
                nexioRC_JOGP(ON);
                _RPTF0(_CRT_WARN, "RC - JOG PLUS\n");
                retval = TRUE;
            }

            done();

            return retval;
        }
        inline BOOL JogMinus (void)
        {
            BOOL retval = FALSE;

            if ( !nexioIsRC_MODES() ) {
                return retval;
            }
            run();

            nexioRC_JOGP(OFF);
            // ↑ボタン押下中なら出力
            if ( nexioIsMS_UpSwitch() ) {
                nexioRC_JOGM(ON);
                _RPTF0(_CRT_WARN, "RC - JOG MINUS\n");
                retval = TRUE;
            }

            done();

            return retval;
        }
        inline BOOL JogStop (void)
        {
            BOOL retval = TRUE;

            if ( !nexioIsRC_MODES() ) {
                return FALSE;;
            }
            run();

            nexioRC_JOGP(OFF);
            nexioRC_JOGM(OFF);

            _RPTF0(_CRT_WARN, "RC - JOG STOP\n");

            done();

            return retval;
        }
        inline BOOL SelectMode (unsigned char mode)
        {
            BOOL retval = FALSE;
            basic_timer wait;

            if ( !is_idle() ) return retval;

            ready();

            switch ( mode ) {
            case RCOPMODE_NORMAL:
                nexioRC_MODE(OFF);
                break;
            case RCOPMODE_JOG:
                nexioRC_MODE(ON);
                nexioRC_JISL(OFF);
                _RPTF0(_CRT_WARN, "RC - RCOPMODE_JOG MODE SELECT\n");
                break;
            case RCOPMODE_INCHING:
                nexioRC_MODE(ON);
                nexioRC_JISL(ON);
                _RPTF0(_CRT_WARN, "RC - RCOPMODE_INCHING MODE SELECT\n");
                break;
            case RCOPMODE_UNKNOWN:
            default:
                break;
            }

            wait.set(5000);

            while ( !is_cancel() ) {
                if ( mode == nexioRC_GetCurrentMode() ) {
                    retval = TRUE;
                    break;
                }
                else
                if ( wait.is_timeout() ) {
                    break;
                }
                basic_timer::do_events(1L);
            }
            return retval;
        }

        inline BOOL AlarmReset (void)
        {
            BOOL retval = FALSE;
            basic_timer wait;

            ready();

            nexioRC_RES(ON);

            // アラームリセット信号（RES）を6msec以上入力
            wait.set(500);

            while ( !is_cancel() ) {
                if ( nexioIsRC_ALM() ) {
                    nexioRC_RES(OFF);
                    retval = TRUE;
                    break;
                }
                else
                if ( wait.is_timeout() ) {
                    break;
                }
                basic_timer::do_events(1L);
            }
            return retval;
        }

        inline BOOL MoveToTeachPos (unsigned char num)
        {
            BOOL retval = FALSE;
            basic_timer wait;

            if ( !nexioIsRC_SV() ) {
                return retval;
            }
            else
            if ( !nexioIsRC_HEND() ) {
                return retval;
            }
            else
            if ( !SelectMode(RCOPMODE_NORMAL) ) {
                
                return retval;
            }

            ready();

            run();

            // 指令ポジション番号
            nexioRC_PC(num);

            // スタート信号(CSTR)のON前に6ms以上の遅延
            wait.delay(500);

            nexioRC_CSTR(ON);

            // タイムアウト
            wait.set(5000);

            // キャンセルされるまでループ
            while ( !is_cancel() ) {
                if ( !nexioIsRC_PEND() ) {
                    retval = TRUE;
                    break;
                }
                else
                if ( wait.is_timeout() ) {
                    nexifAlarmSet(NEXALID_RcMotionTimeouts);
                    break;
                }
                basic_timer::do_events(1L);
            }
            nexioRC_CSTR(OFF);

            // タイムアウト
            wait.set(m_params.RoboCylinder.Timeouts.TeachPos);

            if ( retval ) {
                // キャンセルされるまでループ
                while (1) {
                    if ( nexioIsRC_PEND() && !nexioIsRC_MOVE() ) {
                        break;
                    }
                    else
                    if ( is_cancel() ) {
                        retval = FALSE;
                        break;
                    }
                    else
                    if ( wait.is_timeout() ) {
                        retval = FALSE;
                        nexifAlarmSet(NEXALID_RcMotionTimeouts);
                        break;
                    }
                    basic_timer::do_events(1L);
                }
                if ( !nexioIsRC_ALM() ) {
                    nexifAlarmSet(NEXALID_RcMotionFailure);
                    AlarmReset();
                }
            }

            done();

            return retval;
        }

        // 非常停止時は電磁弁オフ
        inline void emergency (void)
        {
            ServoOff();
        }
    private:
        inline BOOL ServoOn (void)
        {
            BOOL retval = FALSE;
            basic_timer wait;

            if ( nexioIsRC_RMDS() ) {
                return retval;
            }

            ready();

            // 遅延してサーボオンする
            wait.delay(500);

            nexioRC_STP(ON);
            nexioRC_SON(ON);

            // サーボオンタイムアウト
            wait.set(m_params.RoboCylinder.Timeouts.ServoOn);

            while ( !is_cancel() ) {
                if ( nexioIsRC_SV() && nexioIsRC_ALM() ) {
                    retval = TRUE;
                    break;
                }
                else
                if ( wait.is_timeout() ) {
                    break;
                }
                basic_timer::do_events(1);
            }
            // キャンセルまたはタイムアウト時は出力OFFする
            if ( !retval ) {
                nexioRC_STP(OFF);
                nexioRC_SON(OFF);
            }
            return retval;
        }
        inline void ServoOff (void)
        {
            nexioRC_PC(0);
            nexioRC_MODE(OFF);
            nexioRC_JISL(OFF);
            nexioRC_JOGP(OFF);
            nexioRC_JOGM(OFF);
            nexioRC_RMOD(OFF);
            nexioRC_HOME(OFF);
            nexioRC_STP(OFF);
            nexioRC_CSTR(OFF);
            nexioRC_PWRT(OFF);
            nexioRC_RES(OFF);
            nexioRC_SON(OFF);
        }
        inline BOOL MoveToHomePos (void)
        {
            BOOL retval = FALSE;
            basic_timer wait;

            // 原点復帰済みの場合はしない
            if ( nexioIsRC_HEND() ) {
                return TRUE;
            }

            nexioRC_HOME(ON);

            // 原点復帰タイムアウト
            wait.set(m_params.RoboCylinder.Timeouts.Homing);

            while ( !is_cancel() ) {
                if ( nexioIsRC_HEND() &&
                     nexioIsRC_PEND() &&
                    !nexioIsRC_MOVE() ) {
                    retval = TRUE;
                    break;
                }
                else
                if ( wait.is_timeout() ) {
                    nexifAlarmSet(NEXALID_RcMotionTimeouts);
                    break;
                }
                basic_timer::do_events(1);
            }
            // 常時出力OFFする
            nexioRC_HOME(OFF);

            return retval;
        }
    }; // cRoboCylinder

protected:
    virtual void ParseEvent (void) {}
    virtual void EmergencyStop (void)
    {
        m_emergency = TRUE;
        nexifAlarmSet(NEXALID_EmergencyStop);
        m_initialized = FALSE;
        list<basic_coord*>::iterator iter(m_list.begin());
        for ( ; iter != m_list.end(); ++iter ) {
            (*iter)->emergency();
        }
    }

    inline BOOL IsHardwareSimulation (void) const { return m_params.ADAX.HardwareSimulation; }

    inline void Execute (HWND hwnd, NextraEventsEnum event)
    {
        if ( !IsIdle() ) {
			m_lfs << arrange << datetime << "Execute error." << std::endl;
            PostClientMessage(hwnd, event, FALSE);
            return;
        }
		m_lfs << arrange << datetime << setw(10) << setfill('0') << hex << showbase << "hwnd(" << client.hwnd << ")" << std::endl;
        client.hwnd = hwnd; client.result = FALSE; client.event = event;
        ParseEvent();
    }

    BOOL                m_initialized;
    BOOL                m_emergency;                    // 非常停止発生
    HMODULE             m_hModule;
    cVacuum*            m_vacuum;
    cAirFloat*          m_airfloat;
    cShutter*           m_shutter;
    cRoboCylinder*      m_robo;

    list<basic_coord*>  m_list;

    struct clinet_request_t {
        HWND             hwnd;
        BOOL             result;
        NextraEventsEnum event;
    } client;
	lfstream			m_lfs;
};

#endif // _COORD_HPP_

