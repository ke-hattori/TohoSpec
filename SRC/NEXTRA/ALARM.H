// ALARM.H: interface for the cAlarm class.
//
// ----------------------------------------------------------------------------

#if !defined(_ALARM_H_)
#define _ALARM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------
// FORWARD DECLARATIONS
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// DEFINITIONS
// ----------------------------------------------------------------------------
#define ALARM_LOG_NAME		"Align_Alarm"
#define ALARM_LOG_MAX		30
#define ALARM_LOG_SIZE		300000
#define SUBITEM_LEN 		20

// ----------------------------------------------------------------------------
// CONSTANT
// ----------------------------------------------------------------------------
enum
{
	ALARM_RELEASE,
	ALARM_OCCURR,
};

enum
{
	WARNING_RELEASE,
	WARNING_OCCURR,
};

enum
{
	ALCD_Servo,
	ALCD_Alignment,
};

class cAlarm
{
public:
	cAlarm();
	virtual ~cAlarm();

public:
	void Initialize (void);
	inline void Attach (HWND hwnd) { m_hwnd = hwnd; }
	void Set (unsigned long alid);
	void Reset (void);
	void Reset(unsigned long alid);

private:
	struct alarm_desc_t {
		unsigned char flag;
		unsigned long alcd;
		string		  level;
		string		  text;
		basic_timer   time;
	};
	class alarm_data
	{
	public:
		alarm_data () : alid(0) { basic_interface::zeromemory(&desc); }
		unsigned long alid;
		alarm_desc_t  desc;

		friend istringstream& operator >> (istringstream& iss, alarm_data& rhs)
		{
			std::string token;
			if ( std::getline(iss, token, COMMACHAR) ) {
				rhs.alid = _ttol(token.c_str());
			}
			if ( std::getline(iss, token, COMMACHAR) ) {
				rhs.desc.alcd = _ttol(token.c_str());
			}
			std::getline(iss, rhs.desc.level, COMMACHAR);
			std::getline(iss, rhs.desc.text, COMMACHAR);
			return iss;
		}

		friend ostream& operator << (ostream& os, const alarm_data& rhs)
		{
			return os << rhs.desc.time
			   << setw(4) << setfill('0') << rhs.alid
			   << SPACECHAR
			   << LBRACKET
			   << rhs.desc.level
			   << RBRACKET
			   << SPACECHAR
			   << (rhs.desc.flag == ON ? "<SET>" : "<RST>")
			   << SPACECHAR
			   << rhs.desc.text;
		}

	};
	typedef std::vector<alarm_data> alarm_table;

	void SetItemDesc (const alarm_table::iterator iter);
	void ResetItemDesc (const alarm_table::iterator iter);

	alarm_table 		m_table;
	critical_section	m_wait;
	lfstream			m_lfs;
	HWND				m_hwnd;

};

#endif // !defined(_ALARM_H_)
