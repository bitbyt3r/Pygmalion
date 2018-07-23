#include <Python.h>
#include "ptp.h"
#include "protocol.h"
#include <glib.h>

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
    {
        "operation",
        ptp_operation,
        METH_VARARGS,
        "Run a usb operation"
    },
    {
        "open_camera",
        ptp_open_camera,
        METH_VARARGS,
        "Open a connection to a given camera"
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
    GHashTable *camera_table;
    camera_table = ptp_usb_list_cameras();
    GHashTableIter iter;
    g_hash_table_iter_init (&iter, camera_table);
    PyObject *list;
    list = Py_BuildValue("[]");
    uint32_t *val;
    uint32_t *key;
    while (g_hash_table_iter_next(&iter, (gpointer) &key, (gpointer) &val)) {
        PyObject *dict;
        dict = Py_BuildValue("{s:i,s:i,s:i}", "vendor", ((camera*)val)->desc->idVendor, "product", ((camera*)val)->desc->idProduct, "device", ((camera*)val)->dev);
        PyList_Append(list, dict);
    }
    return list;
}

static PyObject * ptp_stop(PyObject *self, PyObject *args) {
    ptp_usb_stop();
    Py_RETURN_NONE;
}

static PyObject * ptp_operation(PyObject *self, PyObject *args) {
    command *cmd = (command*)malloc(sizeof(command));
    int opcode;
    PyArg_ParseTuple(args, "i", &opcode);
    cmd->opcode = opcode;
    ptp_usb_operation(cmd);
    Py_RETURN_NONE;
}

static PyObject * ptp_open_camera(PyObject *self, PyObject *args) {
    int dev;
    PyArg_ParseTuple(args, "i", &dev);
    printf("Opening camera %d\n", dev);
    ptp_usb_open_camera(dev);
    Py_RETURN_NONE;
}