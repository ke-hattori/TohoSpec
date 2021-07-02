#if !defined( _TYPEA_HPP_ )
#define _TYPEA_HPP_

#pragma once

#include <alarmif.hxx>
#include <nexui.hxx>
#include <nexproc.hxx>

// =========================================================================
// class : cResistenceCoord
// =========================================================================
class cTypeA : public cCoord, protected cActive
{
public:
    // Constructor/destructor
    cTypeA (void);
    virtual ~ cTypeA (void);

    // Action requests from client
    virtual LRESULT Initialize (void);
    virtual void Finalize (void);

    // Status inquiries
    virtual BOOL IsIdle (void) const;

    virtual void Cancel (void)
    {
        cCoord::Cancel();

        // 非常停止発生した場合は送らない！
        if ( !m_emergency ) {
            nexaxCancel();
        }
    }

protected:
    // state processing
    void enter_ST_IDLE (const cEventParams* const params);
    void enter_ST_INIT (const cEventParams* const params);
    void enter_ST_INIT_SERVO_ON (const cEventParams* const params);
    void enter_ST_INIT_MOVE_HOME (const cEventParams* const params);
    void enter_ST_INIT_DONE (const cEventParams* const params);
    void enter_ST_MOVE_UPPER (const cEventParams* const params);
    void enter_ST_MOVE_ALIGN (const cEventParams* const params);
    void enter_ST_MOVE_LOWER (const cEventParams* const params);
    void enter_ST_AFLOAD_FLOAT_ON (const cEventParams* const params);
    void enter_ST_AFLOAD_FLOAT_OFF (const cEventParams* const params);
    void enter_ST_CLAMP1 (const cEventParams* const params);
    void enter_ST_CLAMP2 (const cEventParams* const params);
    void enter_ST_UNCLAMP1 (const cEventParams* const params);
    void enter_ST_UNCLAMP2 (const cEventParams* const params);
    void enter_ST_PREP_UNCLAMP (const cEventParams* const params);
    void enter_ST_VAC_ON (const cEventParams* const params);
    void enter_ST_VAC_OFF (const cEventParams* const params);
    void enter_ST_WAIT_VAC_ON (const cEventParams* const params);
    void enter_ST_WAIT_VAC_OFF (const cEventParams* const params);
#ifdef __NEXPROC_NO_PIN_UP_CHECK
    void enter_ST_PARSE_NEXT(const cEventParams* const params);
#endif
    void enter_ST_ERROR (const cEventParams* const params);
    void enter_ST_CANCEL (const cEventParams* const params);
    void enter_ST_DONE (const cEventParams* const params);
    void enter_ST_ABORT (const cEventParams* const params);

    DECLARE_STATEPROC_MAPEX(cTypeA)

    virtual void ParseEvent (void);
    unsigned int start_address (void);

private:
    inline BOOL MoveToUpperPos (void)
    {
        if ( nexaxMoveToUpperPos(nexax_axis_1) != NEXAXERR_NONE ) {
            return FALSE;
        }
        if ( nexaxMoveToUpperPos(nexax_axis_2) != NEXAXERR_NONE ) {
            return FALSE;
        }
        return TRUE;
    }
    inline BOOL MoveToAlignmentPos (void)
    {
        if ( nexaxMoveToAlignmentPos(nexax_axis_1) != NEXAXERR_NONE ) {
            return FALSE;
        }
        if ( nexaxMoveToAlignmentPos(nexax_axis_2) != NEXAXERR_NONE ) {
            return FALSE;
        }
        return TRUE;
    }
    inline BOOL MoveToLowerPos (void)
    {
        if ( nexaxMoveToLowerPos(nexax_axis_1) != NEXAXERR_NONE ) {
            return FALSE;
        }
        if ( nexaxMoveToLowerPos(nexax_axis_2) != NEXAXERR_NONE ) {
            return FALSE;
        }
        return TRUE;
    }
    inline BOOL MoveToHomePos (void) const
    {
        if ( nexaxMoveToHomePos(nexax_axis_1) != NEXAXERR_NONE ) {
            return FALSE;
        }
        if ( nexaxMoveToHomePos(nexax_axis_2) != NEXAXERR_NONE ) {
            return FALSE;
        }
        return TRUE;
    }
    void WaitForMotionFinished (void);

    inline BOOL IsExistServoAlarm (void) const { return nexioIsServoAlarmEx(nexax_axis_1) || nexioIsServoAlarmEx(nexax_axis_2); }
    inline BOOL IsServoReady      (void) const { return nexioIsServoOnEx(nexax_axis_1)    || nexioIsServoOnEx(nexax_axis_2);    }
    inline void ServoOn (BOOL bOn) const { nexioServoOnEx(nexax_axis_1, bOn); nexioServoOnEx(nexax_axis_2, bOn); }
    inline void ServoBrake (BOOL bOn) const { nexioServoBrakeEx(nexax_axis_1, bOn); nexioServoBrakeEx(nexax_axis_2, bOn); }

    // クランプ・アンクランプ
    virtual BOOL DoClampWaitForFinished (PFNEXECPROCEX pExecProcEx, PFNWAITPROCEX pWaitProcEx, unsigned short num, unsigned long msec)
    {
        BOOL ret = FALSE;

    RETRY:
        ret = cCoord::DoClampWaitForFinished(pExecProcEx, pWaitProcEx, num, msec);

        // タイムアウト時にはメッセージ表示・次動作確認
        if ( !ret && m_params.System.ConfirmOperator ) {
            TCHAR szTitleMsg[256] = {0}, szErrorMsg[256] = {0};
            ::LoadString(m_hModule, IDS_NEXTRA_APP_TITLE, szTitleMsg, sizeof(szTitleMsg));
            ::LoadString(m_hModule, IDS_NEXTRA_CLAMP_FAILED, szErrorMsg, sizeof(szErrorMsg));
            if ( nexuiMessageBox(NULL, szErrorMsg, szTitleMsg,
                     MB_RETRYCANCEL | MB_SYSTEMMODAL | MB_ICONQUESTION) == IDRETRY ) {
                AlarmIf_Reset(NEXALID_ClampTimeouts);
                goto RETRY;
            }
        }
        return ret;
    }
protected:
    // class Vacuum TypeA
    class cVacuumTypeA : public cCoord::cVacuum
    {
    public:
        cVacuumTypeA (void) : cVacuum() { Initialize(); }
        virtual ~ cVacuumTypeA (void) {}

        virtual void Initialize (void)
        {
            // 電磁弁1, 2 登録
            for ( unsigned short valve = 0; valve < VACUUM_SOLENOID_VALVE_MAX; valve++ )
                m_valve.push_back(valve);
        }
    };
    // class AirFloat TypeA 
    class cAirFloatTypeA : public cCoord::cAirFloat
    {
    public:
        cAirFloatTypeA (void) : cAirFloat() { Initialize(); }
        virtual ~ cAirFloatTypeA (void) {}

        virtual void Initialize (void)
        {
            // 電磁弁1, 2 登録
            for ( unsigned short valve = 0; valve < VACUUM_SOLENOID_VALVE_MAX; valve++ )
                m_valve.push_back(valve);
        }
    };

    virtual void EmergencyStop (void);
};

#endif // _TYPEA_HPP_
