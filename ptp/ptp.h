#include <Python.h>

#ifndef PTP
#define PTP

static PyObject * ptp_init(PyObject *self, PyObject *args);
static PyObject * ptp_list_cameras(PyObject *self, PyObject *args);
static PyObject * ptp_stop(PyObject *self, PyObject *args);

static PyMethodDef ptp_methods[];

static struct PyModuleDef ptp_module;

PyMODINIT_FUNC PyInit_ptp(void);

#endif
