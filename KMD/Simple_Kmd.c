
#include "ntddk.h"
#include "dbgmsg.h"

#define IOCTL_TEST_CMD \
CTL_CODE(FILE_DEVICE_RK, 0x801, METHOD_BUFFERED, FILE_READ_DATA|FILE_WRITE_DATA)

const WCHAR DeviceNameBuffer[] = L"\\Device\\msnetdiag";                    // prefisso L = Unicode
PDEVICE_OBJECT MSNetDiagDeviceObject;

NTSTATUS RegisterDriverDeviceName(
    IN PDRIVER_OBJECT pDriverObject
){
    NTSTATUS ntStatus;
    UNICODE_STRING unicodeString;

    RtlInitUnicodeString(&unicodeString, DeviceNameBuffer);

    ntStatus = IoCreateDevice(  
        pDriverObject,                      // Puntatore al driver object
        0,                                  // bytes allocati per l'estensione del dispositivo
        &unicodeString,                     // stringa unicode contenente il nome del dispositivo
        FILE_DEVICE_RK,                     // tipo di driver (definito dal costruttore)
        0,                                  // costanti definite dal sistema
        TRUE,                               // il device object e`un dispositivo esclusivo
        &MSNetDiagDeviceObject              // puntatore al device object globale
    );

    return (ntStatus);
}

VOID Unload(IN PDRIVER_OBJECT pDriverObject){
    DBG_TRACE("OnUnload","Received signal to unload the driver");
    return;
}

NTSTATUS dispatchIOControl(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP pIRP
){
    PIO_STACK_LOCATION  irpStack;
    PVOID               inputBuffer;
    PVOID               outputBuffer;
    ULONG               inputBufferLength;
    ULONG               outputBufferLength;
    ULONG               ioctrlcode;
    NTSTATUS            ntStatus;

    ntStatus = STATUS_SUCCESS;
    ((*pIRP).IoStatus).Status = STATUS_SUCCESS;
    ((*pIRP).IoStatus).Information = 0;

    inputBuffer = (*pIRP).AssociatedIrp.SystemBuffer;
    outputBuffer = (*pIRP).AssociatedIrp.SystemBuffer;

    // Ottiene un puntatore alla locazione dello stack del chiamante nell'IRP dato
    irpStack = IoGetCurrentIrpStackLocation(pIRP);
    inputBufferLength = (*irpStack).Parameters.DeviceIoControl.InputBufferLength;
    outputBufferLength = (*irpStack).Parameters.DeviceIoControl.OutputBufferLength;
    ioctrlcode = (*irpStack).Parameters.DeviceIoControl.IoControlCode;

    DBG_TRACE("dispatchIOControl", "Received a command");

    switch(ioctrlcode){
        case IOCTL_TEST_CMD:
            TestCommand(inputBuffer, outputBuffer, inputBufferLength, outputBufferLength);
            ((*pIRP).IoStatus).Information = outputBufferLength;
            break;
        default:
            DBG_TRACE("dispatchIOControl", "control code not recognized");
            break;
    } 

    IoCompleteRequest(pIRP, IO_NO_INCREMENT);
    
    return (ntStatus);
}

// Setta il campo "information" del membro IoStatus dell'IRP al numero di bytes trasferiti con successo e quindi completa l'IRP in modo che l'I/O manager possa
// smaltire l"IRP e reportare indietro all'applicazione che ha iniziato l'intero processo.
NTSTATUS defaultDispatch(
    IN PDEVICE_OBJECT pDeviceObject,            // puntatore alla struttura Device Object
    IN PIRP pIRP                                // puntatore alla struttura I/O Request Packet
){
    ((*pIRP).IoStatus).Status = STATUS_SUCCESS;
    ((*pIRP).IoStatus).Information = 0;
    IoCompleteRequest(pIRP, IO_NO_INCREMENT);

    return (STATUS_SUCCES);
}

// Eseguita quando il KMD viene caricato per la prima volta dentro lo spazio del kernel.
// Il parametro pDriverObject rappresenza l'immagine di memoria del KMD.
// Il parametro regPath descrive il percorso alla chiave del KMD nel registro.
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING regPath){          // IN indica che sono parametri di input a differenza di OUT che indica parametri di output.
    int i;
    NTSTATUS ntStatus;

    for(i=0; i<IRP_MJ_MAXIMUM_FUNCTION; i++){
        (*pDriverObject).MajorFunction[i] = defaultDispatch;
    }

    (*pDriverObject).MajorFunction[IRP_MJ_DEVICE_CONTROL]= dispatchIOControl;

    (*pDriverObject).DriverUnload = Unload;

    DriverObjectRef = pDriverObject;                                                       //set global reference variable

    DBG_TRACE("Driver Entry", "Registering driver's device name");

    ntStatus = RegisterDriverDeviceName(pDriverObject);
    
    if(!NT_SUCCESS(ntStatus)){
        DBG_TRACE("Driver Entry", "Failed to create device");
        return ntStatus;
    }

    DBG_TRACE("Driver Entry", "Registering driver's symbolic link");

    ntStatus = RegisterDriverDeviceLink();

    if(!NT_SUCCESS(ntStatus)){
        DBG_TRACE("Driver Entry", "Failed to create symbolic link");
    }

    return (STATUS_SUCCESS);
}