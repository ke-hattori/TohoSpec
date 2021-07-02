#if !defined( _SIO_H_ )
#define _SIO_H_

#pragma once

using namespace nano;

// =========================================================================
// class : CSio
// =========================================================================

class CSio : protected nano::basic_sio
{
public:
	// constructor / destructor
	CSio ();
	virtual ~ CSio ();

	inline BOOL Open (PCSTR pszDeviceName, DWORD dwBaudRate = CBR_19200, DWORD dwTimeout = T1_TIMEOUT)
	{
		timer.set(dwTimeout);
		return open (pszDeviceName, dwBaudRate);
	}
	inline BOOL Open (sio_config_t cfg, DWORD dwTimeout = T1_TIMEOUT) { return Open(cfg.port, cfg.baudrate, dwTimeout); }
	inline BOOL Open (WORD wPortNo, DWORD dwBaudRate = CBR_19200, DWORD dwTimeout = T1_TIMEOUT)
	{
		TCHAR szDeviceName[256];
		sprintf(szDeviceName, "COM%d", wPortNo);

		return Open (szDeviceName, dwBaudRate, dwTimeout);
	}

	DWORD Write (PCSTR pszBuffer);
	DWORD Read (PSTR pszBuffer);
	inline BOOL IsValid (void) const { return is_valid(); }
};

#endif // _SIO_H_
