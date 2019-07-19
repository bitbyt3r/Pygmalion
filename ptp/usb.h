#ifndef PTP_USB
#define PTP_USB

#include "libusb.h"
#include "camera.h"

enum ContainerType {
    CommandBlock  = 0x0001,
    DataBlock     = 0x0002,
    ResponseBlock = 0x0003,
    EventBlock    = 0x0004,
};

enum Operation {
    GetDeviceInfo               = 0x1001,
    OpenSession                 = 0x1002,
    CloseSession                = 0x1003,
    GetStorageIDs               = 0x1004,
    GetStorageInfo              = 0x1005,
    GetNumObjects               = 0x1006,
    GetObjectHandles            = 0x1007,
    GetObjectInfo               = 0x1008,
    OpGetObject                 = 0x1009,
    GetThumb                    = 0x100A,
    OpDeleteObject              = 0x100B,
    SendObjectInfo              = 0x100C,
    SendObject                  = 0x100D,
    InitiateCapture             = 0x100E,
    FormatStore                 = 0x100F,
    ResetDevice                 = 0x1010,
    SelfTest                    = 0x1011,
    SetObjectProtection         = 0x1012,
    PowerDown                   = 0x1013,
    GetDevicePropDesc           = 0x1014,
    GetDevicePropValue          = 0x1015,
    SetDevicePropValue          = 0x1016,
    ResetDevicePropValue        = 0x1017,
    TerminateOpenCapture        = 0x1018,
    MoveObject                  = 0x1019,
    CopyObject                  = 0x101A,
    GetPartialObject            = 0x101B,
    InitiateOpenCapture         = 0x101C,
    CanonSetRemoteMode          = 0x9114,
    CanonSetEventMode           = 0x9115,
    CanonGetEvent               = 0x9116,
    CanonSetRequestOLCInfoGroup = 0x913D,
};

enum Response {
    OK                                     = 0x2001,
    GeneralError                           = 0x2002,
    SessionNotOpen                         = 0x2003,
    InvalidTransactionID                   = 0x2004,
    OperationNotSupported                  = 0x2005,
    ParameterNotSupported                  = 0x2006,
    IncompleteTransfer                     = 0x2007,
    InvalidStorageID                       = 0x2008,
    InvalidObjectHandle                    = 0x2009,
    DevicePropNotSupported                 = 0x200A,
    InvalidObjectFormatCode                = 0x200B,
    RespStoreFull                          = 0x200C,
    ObjectWriteProtected                   = 0x200D,
    StoreReadOnly                          = 0x200E,
    AccessDenied                           = 0x200F,
    NoThumbnailPresent                     = 0x2010,
    SelfTestFailed                         = 0x2011,
    PartialDeletion                        = 0x2012,
    StoreNotAvailable                      = 0x2013,
    SpecificationByFormatUnsupported       = 0x2014,
    NoValidObjectInfo                      = 0x2015,
    InvalidCodeFormat                      = 0x2016,
    UnknownVendorCode                      = 0x2017,
    CaptureAlreadyTerminated               = 0x2018,
    DeviceBusy                             = 0x2019,
    InvalidParentObject                    = 0x201A,
    InvalidDevicePropFormat                = 0x201B,
    InvalidDevicePropValue                 = 0x201C,
    InvalidParameter                       = 0x201D,
    SessionAlreadyOpen                     = 0x201E,
    TransactionCancelled                   = 0x201F,
    SpecificationofDestinationUnsupported  = 0x2020,
};

enum Event {
    CancelTransaction     = 0x4001,
    ObjectAdded           = 0x4002,
    ObjectRemoved         = 0x4003,
    StoreAdded            = 0x4004,
    StoreRemoved          = 0x4005,
    DevicePropChanged     = 0x4006,
    ObjectInfoChanged     = 0x4007,
    DeviceInfoChanged     = 0x4008,
    RequestObjectTransfer = 0x4009,
    EvtStoreFull          = 0x400A,
    DeviceReset           = 0x400B,
    StorageInfoChanged    = 0x400C,
    CaptureComplete       = 0x400D,
    UnreportedStatus      = 0x400E,
};

typedef struct container {
    uint32_t length;
    uint16_t container_type;
    uint16_t code;
    uint32_t session;
    uint32_t transaction;
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint32_t param4;
    uint32_t param5;
    CameraObject *camera;
} container;

int ptp_usb_start(void *camera_added_cb, void *camera_removed_cb, void *camera_test_cb);
int ptp_usb_stop(void);
libusb_device_handle *ptp_usb_open(libusb_device *dev);

#endif
