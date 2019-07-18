#ifndef PTP_CAMERA
#define PTP_CAMERA

#include "Python.h"

#define CameraObject_Check(v) (Py_TYPE(v) == &Camera_Type)

PyObject *ErrorObject;

typedef struct CameraObject {
    PyObject_HEAD
    PyObject *x_attr;
} CameraObject;

PyTypeObject Camera_Type;
CameraObject *newCameraObject(PyObject *arg);
void Camera_dealloc(CameraObject *self);
PyObject *Camera_open(CameraObject *self, PyObject *args);
PyObject *Camera_getattro(CameraObject *self, PyObject *name);
int Camera_setattr(CameraObject *self, const char *name, PyObject *v);
void write_cb(struct libusb_transfer *transfer);
void read_cb(struct libusb_transfer *transfer);
int increment_transaction(CameraObject *self);

#include "usb.h"

#endif