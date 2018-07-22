#include <Python.h>
#include "ptp.h"
#include "protocol.h"

static PyMethodDef ptp_methods[] = {
    {
        "init",
        ptp_init,
        METH_NOARGS,
        "Initialize the ptp module"
    },
    {
        "list_cameras",
        ptp_list_cameras,
        METH_NOARGS,
        "List connected cameras"
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

static PyObject * ptp_init(PyObject *self, PyObject *args) {
    ptp_usb_start();
    Py_RETURN_NONE;
}

static PyObject * ptp_list_cameras(PyObject *self, PyObject *args) {
    camera_list *cameras;
    cameras = ptp_usb_list_cameras();
    if (cameras == NULL) {
        Py_RETURN_NONE;
    }
    PyObject *list;
    list = Py_BuildValue("[]");
    while (cameras != NULL) {
        PyObject *dict;
        dict = Py_BuildValue("{s:i,s:i}", "vendor", cameras->vendorId, "product", cameras->deviceId);
        PyList_Append(list, dict);
        cameras = cameras->next;
    }
    return list;
}

static PyObject * ptp_stop(PyObject *self, PyObject *args) {
    ptp_usb_stop();
    Py_RETURN_NONE;
}