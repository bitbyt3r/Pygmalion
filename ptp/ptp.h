#include <Python.h>

#ifndef PTP
#define PTP

static PyObject * ptp_init(PyObject *self, PyObject *args, PyObject *kw);
static PyObject * ptp_stop(PyObject *self, PyObject *args);

static struct PyModuleDef ptp_module;

PyMODINIT_FUNC PyInit_ptp(void);

#endif
