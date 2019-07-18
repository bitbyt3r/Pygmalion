#include "Python.h"
#include <iconv.h>
#include "usb.h"

CameraObject *newCameraObject(PyObject *arg)
{
    CameraObject *self;
    self = PyObject_New(CameraObject, &Camera_Type);
    if (self == NULL)
        return NULL;
    self->x_attr = NULL;
    int vendor;
    int product;
    PyObject *device;
    PyArg_ParseTuple(arg, "iiO", &vendor, &product, &device);
    PyObject *vendorObj = PyLong_FromLong(vendor);
    PyObject *productObj = PyLong_FromLong(product);
    PyObject *transactionObj = PyLong_FromLong(0);
    PyObject *sessionObj = PyLong_FromLong(0);
    Camera_setattr(self, "vendor", vendorObj);
    Camera_setattr(self, "product", productObj);
    Camera_setattr(self, "device", device);
    Camera_setattr(self, "transaction", transactionObj);
    Camera_setattr(self, "session", sessionObj);
    return self;
}

void Camera_dealloc(CameraObject *self)
{
    Py_XDECREF(self->x_attr);
    PyObject_Del(self);
};

void print_deviceinfo(unsigned char *buf) {
    size_t nconv, srclen, destlen;
    char *iconvIn;
    char *iconvOut;
    iconv_t utfconv = iconv_open("ASCII", "UCS-2");

    int offset = 12;
    int length = 0;
    printf("Current offset: %04x (%d)\n", offset, offset);
    uint16_t standard_version;
    standard_version = buf[0] + (buf[1]<<8);
    offset = offset + 2;
    printf("Standard Version: %04x\n", standard_version);

    printf("Current offset: %04x (%d)\n", offset, offset);
    uint32_t standard_extension_id;
    standard_extension_id = buf[offset] + (buf[offset+1]<<8) + (buf[offset+2]<<16) + (buf[offset+3]<<24);
    offset = offset + 4;
    printf("Standard Extension ID: %08x\n", standard_extension_id);

    printf("Current offset: %04x (%d)\n", offset, offset);
    uint16_t standard_extension_version;
    standard_extension_version = buf[offset] + (buf[offset+1]<<8);
    offset = offset + 2;
    printf("Standard Extension Version: %04x\n", standard_extension_version);
    
    printf("Current offset: %04x (%d)\n", offset, offset);
    char *vendor_extension_desc;
    length = buf[offset]*2;
    srclen = buf[offset];
    offset = offset + 1;
    iconvIn = buf[offset];
    iconvOut = (char *)vendor_extension_desc;
    nconv = iconv(utfconv, &iconvIn, &srclen, &iconvOut, &length);
    printf("Vendor Extension Description Length: %d\n", length);
    vendor_extension_desc = malloc(length);
    memcpy(vendor_extension_desc, &buf[offset], length);
    offset = offset + length;
    printf("Vendor Extension Description: %s\n", vendor_extension_desc);

    printf("Current offset: %04x (%d)\n", offset, offset);
    uint16_t functional_mode;
    functional_mode = buf[offset] + buf[offset+1]<<8;
    offset = offset + 2;
    printf("Functional Mode: %d\n", functional_mode);

    printf("Current offset: %04x (%d)\n", offset, offset);
    uint32_t num_operations_codes;
    num_operations_codes = buf[offset] + (buf[offset+1]<<8) + (buf[offset+2]<<16) + (buf[offset+3]<<24);
    offset = offset + 4;
    printf("Found %d Operational Codes\n", num_operations_codes);

    uint16_t *operations_supported;
    operations_supported = malloc(num_operations_codes*sizeof(uint16_t));
    for (int i=0; i<num_operations_codes; i++) {
        operations_supported[i] = buf[offset+i*2] + buf[offset+1+i*2]<<8;
    }
    offset = offset + num_operations_codes*2;

    printf("Current offset: %04x (%d)\n", offset, offset);
    uint32_t num_event_codes;
    num_event_codes = buf[offset] + (buf[offset+1]<<8) + (buf[offset+2]<<16) + (buf[offset+3]<<24);
    offset = offset + 4;
    printf("Found %d Event Codes\n", num_event_codes);

    uint16_t *events_supported;
    events_supported = malloc(num_event_codes*sizeof(uint16_t));
    for (int i=0; i<num_event_codes; i++) {
        events_supported[i] = buf[offset+i*2] + buf[offset+1+i*2]<<8;
    }
    offset = offset + num_event_codes*2;

    printf("Current offset: %04x (%d)\n", offset, offset);
    uint32_t num_device_prop_codes;
    num_device_prop_codes = buf[offset] + (buf[offset+1]<<8) + (buf[offset+2]<<16) + (buf[offset+3]<<24);
    offset = offset + 4;
    printf("Found %d Device Property Codes\n");

    uint16_t *device_properties_supported;
    device_properties_supported = malloc(num_device_prop_codes*sizeof(uint16_t));
    for (int i=0; i<num_device_prop_codes; i++) {
        device_properties_supported[i] = buf[offset+i*2] + buf[offset+1+i*2]<<8;
    }
    offset = offset + num_device_prop_codes*2;

    printf("Current offset: %04x (%d)\n", offset, offset);
    uint32_t num_capture_formats;
    num_capture_formats = buf[offset] + (buf[offset+1]<<8) + (buf[offset+2]<<16) + (buf[offset+3]<<24);
    offset = offset + 4;
    printf("Found %d Capture Formats\n", num_capture_formats);

    uint16_t *capture_formats;
    capture_formats = malloc(num_capture_formats*sizeof(uint16_t));
    for (int i=0; i<num_capture_formats; i++) {
        capture_formats[i] = buf[offset+i*2] + buf[offset+1+i*2]<<8;
    }
    offset = offset + num_capture_formats*2;

    printf("Current offset: %04x (%d)\n", offset, offset);
    uint32_t num_image_formats;
    num_image_formats = buf[offset] + (buf[offset+1]<<8) + (buf[offset+2]<<16) + (buf[offset+3]<<24);
    offset = offset + 4;
    printf("Found %d Image Formats\n", num_image_formats);

    uint16_t *image_formats;
    image_formats = malloc(num_image_formats*sizeof(uint16_t));
    for (int i=0; i<num_image_formats; i++) {
        image_formats[i] = buf[offset+2*i] + buf[offset+1+i*2]<<8;
    }
    offset = offset + num_image_formats*2;
    
    srclen = buf[offset]*2;
    length = buf[offset];
    destlen = length;
    offset = offset + 1;
    char *manufacturer = malloc(length);
    iconvOut = (char*)manufacturer;
    iconvIn = &buf[offset];
    nconv = iconv(utfconv, &iconvIn, &srclen, &iconvOut, &destlen);
    offset = offset + length*2;
    printf("Manufacturer: %s\n", manufacturer);

    srclen = buf[offset]*2;
    length = buf[offset];
    destlen = length;
    offset = offset + 1;
    char *model = malloc(length);
    iconvOut = (char*)model;
    iconvIn = &buf[offset];
    nconv = iconv(utfconv, &iconvIn, &srclen, &iconvOut, &destlen);
    offset = offset + length*2;
    printf("Model: %s\n", model);

    srclen = buf[offset]*2;
    length = buf[offset];
    destlen = length;
    offset = offset + 1;
    char *device_version = malloc(length);
    iconvOut = (char*)device_version;
    iconvIn = &buf[offset];
    nconv = iconv(utfconv, &iconvIn, &srclen, &iconvOut, &destlen);
    offset = offset + length*2;
    printf("Device Version: %s\n", device_version);

    srclen = buf[offset]*2;
    length = buf[offset];
    destlen = length;
    offset = offset + 1;
    char *serial_number = malloc(length);
    iconvOut = (char*)serial_number;
    iconvIn = &buf[offset];
    nconv = iconv(utfconv, &iconvIn, &srclen, &iconvOut, &destlen);
    offset = offset + length*2;
    printf("Serial Number: %s\n", serial_number);
}

void read_cb(struct libusb_transfer *transfer) {
    command *cmd = malloc(sizeof(command));
    memset(cmd, 0, sizeof(command));
    cmd->length = 24;
    cmd->packet_type = 1;
    cmd->opcode = ((command *)transfer->user_data)->opcode;
    cmd->camera = ((command *)transfer->user_data)->camera;
    cmd->transaction = increment_transaction(cmd->camera);
    free(transfer->user_data);

    if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
        printf("Transfer Failed %d\n", cmd->opcode);
        return;
    }
    
    printf("Received %d bytes:\n", transfer->length);
    uint32_t length = unpack32(transfer->buffer);
    printf("Reported size %d bytes\n", length);
    uint16_t container_type = unpack16(transfer->buffer+4);
    printf("Container_type: %d\n", container_type);
    uint16_t response_code = unpack16(transfer->buffer+6);
    uint32_t session_id = unpack32(transfer->buffer+8);
    uint32_t transaction_id = unpack32(transfer->buffer+12);
    printf("Container: %04X, Response: %04X, Session: %08X, Transaction: %08X\n", container_type, response_code, session_id, transaction_id);
    for (int i=0; i<length; i++) {
        printf("%02X ", transfer->buffer[i]);
    }
    printf("\n");

    if (cmd->opcode == OpenSession) { // OpenSession
        printf("Session Opened\n");
        cmd->opcode = GetDeviceInfo;
        cmd->param1 = 1;
        cmd->param2 = 0;
        ptp_usb_transaction(cmd, transfer->dev_handle, write_cb);
    } else if (cmd->opcode == GetDeviceInfo) {
        printf("Got Device Info\n");
        //print_deviceinfo(transfer->buffer);
        cmd->opcode = CloseSession;
        ptp_usb_transaction(cmd, transfer->dev_handle, write_cb);
    } else if (cmd->opcode == ResetDevice) {
        printf("Finished resetting device\n");
        cmd->opcode = OpenSession;
        cmd->length = 16;
        cmd->param1 = 0xDEADBEEF;
        cmd->transaction = 0;
        ptp_usb_transaction(cmd, transfer->dev_handle, write_cb);
    } else if (cmd->opcode == CloseSession) {
        printf("Closed session\n");
    }
}

void write_cb(struct libusb_transfer *transfer) {
    if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
        printf("Transfer successful\n");
    } else {
        printf("Transfer failed: %d\n", transfer->status);
    }
    unsigned char *buf;
    buf = malloc(1024);
    int length=1024;
    printf("Write finished for opcode %p\n", (void*)((command*)transfer->user_data)->opcode);
    libusb_fill_bulk_transfer(transfer, transfer->dev_handle, 0x81, buf, length, read_cb, transfer->user_data, 0);
    int ret;
    printf("Submitting transfer\n");
    ret = libusb_submit_transfer(transfer);
    if (LIBUSB_SUCCESS != ret) {
        printf("Couldn't submit transfer: %s\n", libusb_error_name(ret));
        return;
    }
    printf("Read submitted\n");
}

int increment_transaction(CameraObject *self) {
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    int id = PyLong_AsLong(PyDict_GetItemString(self->x_attr, "transaction"));
    id++;
    PyDict_SetItemString(self->x_attr, "transaction", PyLong_FromLong(id));
    PyGILState_Release(gstate);
    return id;
}

PyObject *Camera_open(CameraObject *self, PyObject *args) {
    PyGILState_STATE gstate;
    libusb_device *dev;
    gstate = PyGILState_Ensure();
    PyObject *device = PyDict_GetItemString(self->x_attr, "device");
    dev = (libusb_device*)PyCapsule_GetPointer(device, NULL);
    libusb_device_handle *handle = ptp_usb_open(dev);
    PyObject *handleObj = PyCapsule_New((void*)handle, NULL, NULL);
    PyDict_SetItemString(self->x_attr, "session", PyLong_FromLong(0xDEADBEEF));
    PyDict_SetItemString(self->x_attr, "transaction", PyLong_FromLong(0));
    PyGILState_Release(gstate);
    Camera_setattr(self, "handle", handleObj);
    command *cmd = malloc(sizeof(command));
    cmd->opcode = OpenSession;
    cmd->length = 24;
    cmd->packet_type = 1;
    cmd->param1 = 0xDEADBEEF;
    cmd->param2 = 0;
    cmd->param3 = 0;
    cmd->transaction = 0;
    cmd->camera = self;
    ptp_usb_transaction(cmd, handle, write_cb);
    Py_RETURN_NONE;
}

PyObject *Camera_close(CameraObject *self, PyObject *args) {
    libusb_device *dev;
    PyObject *device = PyDict_GetItemString(self->x_attr, "device");
    dev = (libusb_device*)PyCapsule_GetPointer(device, NULL);
    libusb_device_handle *handle = ptp_usb_open(dev);
    PyObject *handleObj = PyCapsule_New((void*)handle, NULL, NULL);
    Camera_setattr(self, "handle", handleObj);
    command *cmd = malloc(sizeof(command));
    cmd->opcode = CloseSession;
    cmd->length = 24;
    cmd->packet_type = 1;
    cmd->param1 = 0;
    cmd->param2 = 0;
    cmd->param3 = 0;
    cmd->transaction = 0;
    ptp_usb_transaction(cmd, handle, write_cb);
    Py_RETURN_NONE;
}

PyObject *Camera_register_event_cb(CameraObject *self, PyObject *args) {
    Py_RETURN_NONE;
}

PyMethodDef Camera_methods[] = {
    {"open", (PyCFunction)Camera_open, METH_VARARGS, ""},
    {"register_event_cb", (PyCFunction)Camera_register_event_cb, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}
};

PyObject *Camera_getattro(CameraObject *self, PyObject *name)
{
    if (self->x_attr != NULL) {
        PyObject *v = PyDict_GetItem(self->x_attr, name);
        if (v != NULL) {
            Py_INCREF(v);
            return v;
        }
    }
    return PyObject_GenericGetAttr((PyObject *)self, name);
}

int Camera_setattr(CameraObject *self, const char *name, PyObject *v)
{
    if (self->x_attr == NULL) {
        self->x_attr = PyDict_New();
        if (self->x_attr == NULL)
            return -1;
    }
    if (v == NULL) {
        int rv = PyDict_DelItemString(self->x_attr, name);
        if (rv < 0)
            PyErr_SetString(PyExc_AttributeError,
                "delete non-existing Camera attribute");
        return rv;
    }
    else {
        return PyDict_SetItemString(self->x_attr, name, v);
    }
}

PyObject *Camera_repr(CameraObject *self) {
    int vendor = PyLong_AsLong(PyDict_GetItemString(self->x_attr, "vendor"));
    int product = PyLong_AsLong(PyDict_GetItemString(self->x_attr, "product"));
    PyObject *device = PyDict_GetItemString(self->x_attr, "device");
    return PyUnicode_FromFormat("Camera %04x:%04x", vendor, product);
}

PyTypeObject Camera_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "ptp.Camera",             /*tp_name*/
    sizeof(CameraObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Camera_dealloc,    /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)0,             /*tp_getattr*/
    (setattrfunc)Camera_setattr,   /*tp_setattr*/
    0,                          /*tp_reserved*/
    (reprfunc)Camera_repr,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    (getattrofunc)Camera_getattro, /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    0,                          /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    0,                          /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    0,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    Camera_methods,                /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
    0,                          /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    0,                          /*tp_init*/
    0,                          /*tp_alloc*/
    0,                          /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};
