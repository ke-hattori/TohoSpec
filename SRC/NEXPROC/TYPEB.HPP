#if !defined( _TYPEB_HPP_ )
#define _TYPEB_HPP_

#pragma once

#include <alarmif.hxx>
#include <nexui.hxx>

// =========================================================================
// class : cResistenceCoord
// =========================================================================
class cTypeB : public cCoord, protected cActive
{
public:
    // Constructor/destructor
    cTypeB (void);
    virtual ~ cTypeB (void);

    // Action requests from client
    virtual LRESULT Initialize (void);
    virtual void Finalize (void);

    // Status inquiries
    virtual BOOL IsIdle (void) const;

    virtual void Cancel (void)
    {
        cCoord::Cancel();
    }

protected:
    // state processing
    void enter_ST_IDLE (const cEventParams* const params);
    void enter_ST_INIT (const cEventParams* const params);
    void enter_ST_INIT_DONE (const cEventParams* const params);
    void enter_ST_MOVE_UPPER (const cEventParams* const params);
    void enter_ST_MOVE_ALIGN (const cEventParams* const params);
    void enter_ST_MOVE_LOWER (const cEventParams* const params);
    void enter_ST_CLAMP1 (const cEventParams* const params);
    void enter_ST_CLAMP2 (const cEventParams* const params);
    void enter_ST_UNCLAMP1 (const cEventParams* const params);
    void enter_ST_UNCLAMP2 (const cEventParams* const params);
    void enter_ST_PREP_UNCLAMP (const cEventParams* const params);
    void enter_ST_VAC_ON (const cEventParams* const params);
    void enter_ST_VAC_OFF (const cEventParams* const params);
    void enter_ST_WAIT_VAC_ON (const cEventParams* const params);
    void enter_ST_WAIT_VAC_OFF (const cEventParams* const params);
    void enter_ST_ERROR (const cEventParams* const params);
    void enter_ST_CANCEL (const cEventParams* const params);
    void enter_ST_DONE (const cEventParams* const params);
    void enter_ST_ABORT (const cEventParams* const params);

    DECLARE_STATEPROC_MAPEX(cTypeB)

    virtual void ParseEvent (void);
    unsigned int start_address (void);

private:
    inline BOOL MoveToUpperPos (void)
    {
        return DoMoveToLocWaitForFinished(nexioPinUpEx, nexioIsPinUpperPos);
    }
    inline BOOL MoveToAlignmentPos (void)
    {
        return DoMoveToLocWaitForFinished(nexioPinAlignmentEx, nexioIsPinAlignmentPos);
    }
    inline BOOL MoveToLowerPos (void)
    {
        return DoMoveToLocWaitForFinished(nexioPinDownEx, nexioIsPinDownPos);
    }
    BOOL DoMoveToLocWaitForFinished (PFNEXECPROCEX pExecProcEx, PFNWAITPROC pWaitProc)
    {
        BOOL ret = FALSE;

        pExecProcEx(0, ON);

        nano::basic_timer wait(m_params.MotionTimeouts.TeachLoc);
        // キャンセルされるまでループ
        while ( !is_cancel() ) {
            if ( pWaitProc() ) {
                ret = TRUE;
                break;
            }
            else
            if ( wait.is_timeout() ) {
                nexifAlarmSet(NEXALID_MotionTimeouts);
                break;
            }
            basic_timer::do_events(1L);
        }

        pExecProcEx(0, OFF);

        return ret;
    }

protected:
    // class Vacuum TypeA 
    class cVacuumTypeB : public cCoord::cVacuum
    {
    public:
        cVacuumTypeB (void) : cVacuum() { Initialize(); }
        virtual ~ cVacuumTypeB (void) {}

        virtual void Initialize (void)
        {
            // 電磁弁1 登録
            m_valve.push_back(0);
        }
    };

    virtual void EmergencyStop (void);
};

#endif // _TYPEB_HPP_
