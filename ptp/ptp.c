#include <Python.h>
#include "ptp.h"
#include "usb.h"
#include <stdbool.h>
#include "camera.h"

static PyObject *camera_added_cb = NULL;
static PyObject *camera_removed_cb = NULL;
static PyObject *camera_test_cb = NULL;

static PyMethodDef ptp_methods[] = {
    {
        "init",
        ptp_init,
        METH_VARARGS | METH_KEYWORDS,
        "Initialize the ptp module"
    },
    {
        "stop",
        ptp_stop,
        METH_NOARGS,
        "Close the usb connections"
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef ptp_module = {
    PyModuleDef_HEAD_INIT,
    "ptp",
    "A simple implementation of ptp over USB",
    -1,
    ptp_methods
};

PyMODINIT_FUNC PyInit_ptp(void) {
    Py_Initialize();
    return PyModule_Create(&ptp_module);
}

void camera_added(libusb_device *dev, struct libusb_device_descriptor *desc) {
    PyGILState_STATE gstate;

    if (camera_added_cb == NULL) {
        return;
    }
    gstate = PyGILState_Ensure();
    PyObject *devObj = PyCapsule_New((void*)dev, NULL, NULL);
    PyObject *argList = Py_BuildValue("iiO", desc->idVendor, desc->idProduct, devObj);
    PyObject *obj = (PyObject *)newCameraObject(argList);
    Py_DECREF(argList);
    PyObject *args = Py_BuildValue("(O)", obj);
    PyEval_CallObject(camera_added_cb, args);
    PyGILState_Release(gstate);
}

void camera_removed(libusb_device *dev, struct libusb_device_descriptor *desc) {
    PyGILState_STATE gstate;

    if (camera_removed_cb == NULL) {
        return;
    }
    PyObject *args;
    args = Py_BuildValue("({s:i,s:i,s:i})", "vendor", desc->idVendor, "product", desc->idProduct, "device", dev);
    gstate = PyGILState_Ensure();
    PyEval_CallObject(camera_removed_cb, args);
    Py_DECREF(args);
    PyGILState_Release(gstate);
}

bool camera_test(struct libusb_device_descriptor *desc) {
    PyGILState_STATE gstate;

    if (camera_test_cb == NULL) {
        return true;
    }
    PyObject *args;
    args = Py_BuildValue("({s:i,s:i})", "vendor", desc->idVendor, "product", desc->idProduct);
    gstate = PyGILState_Ensure();
    PyObject *result = PyEval_CallObject(camera_test_cb, args);
    Py_DECREF(args);
    bool bool_result = PyObject_IsTrue(result);
    Py_DECREF(result);
    PyGILState_Release(gstate);
    return bool_result;
}

static PyObject * ptp_init(PyObject *self, PyObject *args, PyObject *kw) {
    static char *kwlist[] = {"camera_added", "camera_removed", "camera_test", NULL};
    PyObject *cam_add_func;
    PyObject *cam_rem_func;
    PyObject *cam_test_func;
    PyArg_ParseTupleAndKeywords(args, kw, "|OOO", kwlist, &cam_add_func, &cam_rem_func, &cam_test_func);
    if (PyCallable_Check(cam_add_func)) {
        Py_XDECREF(camera_added_cb);
        Py_XINCREF(cam_add_func);
        camera_added_cb = cam_add_func;
    }
    if (PyCallable_Check(cam_rem_func)) {
        Py_XDECREF(camera_removed_cb);
        Py_XINCREF(cam_rem_func);
        camera_removed_cb = cam_rem_func;
    }
    if (PyCallable_Check(cam_test_func)) {
        Py_XDECREF(camera_test_cb);
        Py_XINCREF(cam_test_func);
        camera_test_cb = cam_test_func;
    }
    ptp_usb_start(camera_added, camera_removed, camera_test);
    Py_RETURN_NONE;
}

static PyObject * ptp_stop(PyObject *self, PyObject *args) {
    ptp_usb_stop();
    Py_RETURN_NONE;
}