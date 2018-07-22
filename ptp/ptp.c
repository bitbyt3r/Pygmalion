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
    printf("Initializing ptp!\n");
    ptp_usb_start();
    Py_RETURN_NONE;
}
