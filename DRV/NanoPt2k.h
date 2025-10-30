//=============================================================================
//  Windows NT ISA Bus Kernel Mode Device Driver
//      File Name = NanoPort.h
//
//              2000/12/23
//              NANOmetrics Japan  R&D  Kohji Sumiyoshi
//
//=============================================================================

#include <wdm.h>
//#include <string.h>
//#include <devioctl.h>

#define NANO_DEVICE_TYPE 0x8001

// The IOCTL function codes from 0x800 to 0xFFF are for customer use.
// The IOCTL Device Type codes from 0x8000 to 0xFFFF are for customer use.

#define IO_READ_UCHAR \
    CTL_CODE( NANO_DEVICE_TYPE, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IO_READ_USHORT \
    CTL_CODE( NANO_DEVICE_TYPE, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IO_READ_ULONG \
    CTL_CODE( NANO_DEVICE_TYPE, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IO_WRITE_UCHAR \
    CTL_CODE( NANO_DEVICE_TYPE, 0x810, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IO_WRITE_USHORT \
    CTL_CODE( NANO_DEVICE_TYPE, 0x811, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IO_WRITE_ULONG \
    CTL_CODE( NANO_DEVICE_TYPE, 0x812, METHOD_BUFFERED, FILE_ANY_ACCESS )

// driver local data structure specific to each device object

typedef struct _LOCAL_DEVICE_INFO {
    PVOID               PortBase;       // base port address
    ULONG               PortCount;      // Count of I/O addresses used.
    ULONG               PortMemoryType; // HalTranslateBusAddress MemoryType
    PDEVICE_OBJECT      DeviceObject;   // The Gpd device object.
    PDEVICE_OBJECT      NextLowerDriver;     // The top of the stack
    BOOLEAN             Started;
    BOOLEAN             Removed;
    BOOLEAN             PortWasMapped;  // If TRUE, we have to unmap on unload  
    BOOLEAN             Filler[1];      //bug fix
    IO_REMOVE_LOCK      RemoveLock;
} LOCAL_DEVICE_INFO, *PLOCAL_DEVICE_INFO;


// Function Prototypes

NTSTATUS    DriverEntry(        IN  PDRIVER_OBJECT DriverObject,
                                IN  PUNICODE_STRING RegistryPath );

NTSTATUS    PortDispatch(       IN  PDEVICE_OBJECT pDO,
                                IN  PIRP pIrp                    );

NTSTATUS    PortIoctlReadPort(  IN  PLOCAL_DEVICE_INFO pLDI,
                                IN  PIRP pIrp,
                                IN  PIO_STACK_LOCATION IrpStack,
                                IN  ULONG IoctlCode              );

NTSTATUS    PortIoctlWritePort( IN  PLOCAL_DEVICE_INFO pLDI,
                                IN  PIRP pIrp,
                                IN  PIO_STACK_LOCATION IrpStack,
                                IN  ULONG IoctlCode              );

VOID        PortUnload(         IN  PDRIVER_OBJECT DriverObject );

NTSTATUS    PortAddDevice(      IN PDRIVER_OBJECT DriverObject,
                                IN PDEVICE_OBJECT PhysicalDeviceObject    );

NTSTATUS    PortDispatchPnp(    IN PDEVICE_OBJECT DeviceObject,
                                IN PIRP Irp    );

NTSTATUS    PortStartDevice(    IN PDEVICE_OBJECT    DeviceObject,
                                IN PIRP              Irp    );

NTSTATUS    PortDispatchPower(  IN PDEVICE_OBJECT    DeviceObject,
                                IN PIRP              Irp    );

NTSTATUS    PortDispatchSystemControl(
                                IN PDEVICE_OBJECT    DeviceObject,
                                IN PIRP              Irp    );

PCHAR       PnPMinorFunctionString (
                                UCHAR MinorFunction);
