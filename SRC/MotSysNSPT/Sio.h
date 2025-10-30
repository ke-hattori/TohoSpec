// Sio.h: interface for the CSio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIO_H__91E0DF41_C375_11D5_8505_005004B7C8A8__INCLUDED_)
#define AFX_SIO_H__91E0DF41_C375_11D5_8505_005004B7C8A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSio
{
private:
	DWORD m_dwTimeoutCount;
	char m_Buff[1024];
	int m_ChannelNo;
	HANDLE m_Handle;
	int m_Index;
	DWORD m_StartTime;

public:
	BOOL EndComm(void);
	void ClearLog(void);
	BOOL m_LogEnable;
	BOOL m_Indicate_Recv;
	BOOL m_Indicate_Send;
//	void CommLogging(char *LogData, int Dir);
	void CommLogging(char *LogData, int Dir, DWORD Length);
	int m_StopBits;
	int m_Parity;
	int m_ByteSize;
	int m_BaudRate;
	char m_sChannelNo[10];
	COMMTIMEOUTS m_CommTimeOuts;
//	int CommCommand(char *Send, char *Recv);
	int CommCommand(char *Send, char *Recv, DWORD *Length);
	unsigned char CalcCheckSum(char *data, short mode);
	int SioSendData(char* buff, int len);
	CSio(int Channel);
	virtual ~CSio();
	int Initialize(	int BaudRate,
					int ByteSize,
					int Parity,
					int StopBits);

};

#endif // !defined(AFX_SIO_H__91E0DF41_C375_11D5_8505_005004B7C8A8__INCLUDED_)
