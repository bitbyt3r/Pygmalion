#ifndef PTP_PROTOCOL
#define PTP_PROTOCOL

#include "libusb.h"
#include <sys/queue.h>
#include <glib.h>

#define DATA_IN_EP      1
#define DATA_OUT_EP     2
#define INTERRUPT_EP    3
#define CONFIG_NUM      1

enum Operation {
    GetDeviceInfo        = 0x1001,
    OpenSession          = 0x1002,
    CloseSession         = 0x1003,
    GetStorageIDs        = 0x1004,
    GetStorageInfo       = 0x1005,
    GetNumObjects        = 0x1006,
    GetObjectHandles     = 0x1007,
    GetObjectInfo        = 0x1008,
    GetObject            = 0x1009,
    GetThumb             = 0x100A,
    DeleteObject         = 0x100B,
    SendObjectInfo       = 0x100C,
    SendObject           = 0x100D,
    InitiateCapture      = 0x100E,
    FormatStore          = 0x100F,
    ResetDevice          = 0x1010,
    SelfTest             = 0x1011,
    SetObjectProtection  = 0x1012,
    PowerDown            = 0x1013,
    GetDevicePropDesc    = 0x1014,
    GetDevicePropValue   = 0x1015,
    SetDevicePropValue   = 0x1016,
    ResetDevicePropValue = 0x1017,
    TerminateOpenCapture = 0x1018,
    MoveObject           = 0x1019,
    CopyObject           = 0x101A,
    GetPartialObject     = 0x101B,
    InitiateOpenCapture  = 0x101C,
};

typedef struct camera {
    libusb_device *dev;
    struct libusb_device_descriptor *desc;
    libusb_device_handle *handle;
} camera;

typedef struct command {
    uint16_t opcode;
    uint32_t param1;
    uint32_t param2;
    uint32_t param3;
    uint32_t session;
    uint32_t transaction;
    libusb_device_handle *handle;
    STAILQ_ENTRY(command) entry;
} command;

int ptp_usb_start(void);
int ptp_usb_stop(void);
void ptp_usb_operation(command *cmd);
void ptp_usb_open_camera(libusb_device *dev);

GHashTable *ptp_usb_list_cameras(void);

#endif
