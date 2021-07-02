#if !defined( _PORT_NT_HXX_ )
#define _PORT_NT_HXX_

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _PORTNTDLL_ )
	#define PORTNTAPI __declspec(dllimport)
#else
	#define PORTNTAPI __declspec(dllexport)
#endif

/*
			Driver Link Name
	"\\\\.\\NanoPtIo" -> "NanoPtIo.sys"
	"\\\\.\\NanoPt2k" -> "NanoPt2k.sys"
*/
#define 	DrvName 				"\\\\.\\NanoPtIo"
#define 	DrvName2k				"\\\\.\\NanoPt2k"	

#define 	NANO_DEVICE_TYPE		0x8001

/*
			I/O Control Code
*/
#define 	IO_READ_UCHAR \
			CTL_CODE( NANO_DEVICE_TYPE, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define 	IO_READ_USHORT \
			CTL_CODE( NANO_DEVICE_TYPE, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define 	IO_READ_ULONG \
			CTL_CODE( NANO_DEVICE_TYPE, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define 	IO_WRITE_UCHAR \
			CTL_CODE( NANO_DEVICE_TYPE, 0x810, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define 	IO_WRITE_USHORT \
			CTL_CODE( NANO_DEVICE_TYPE, 0x811, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define 	IO_WRITE_ULONG \
			CTL_CODE( NANO_DEVICE_TYPE, 0x812, METHOD_BUFFERED, FILE_ANY_ACCESS )

typedef struct _PortBuffData{
	ULONG	PortNumber; 	// Port # to write to
	union	{				// Data to be output to port
		ULONG	LongData;
		USHORT	ShortData;
		UCHAR	CharData;
	};		
}PortBuffData;

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================
UCHAR PORTNTAPI InByte(ULONG, USHORT);
USHORT PORTNTAPI InWord(ULONG, USHORT);
int PORTNTAPI OutByte(ULONG, USHORT, UCHAR);
int PORTNTAPI OutWord(ULONG, USHORT, USHORT);
long PORTNTAPI Open_NtPta(char*);
long PORTNTAPI Close_NtPta(ULONG);
long PORTNTAPI GetNtPtaError();

#ifdef __cplusplus
}
#endif

#endif // _STAGEINT_HXX_
