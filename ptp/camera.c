#include "Python.h"
#include "camera.h"

CameraObject *newCameraObject(PyObject *arg)
{
    CameraObject *self;
    self = PyObject_New(CameraObject, &Camera_Type);
    if (self == NULL)
        return NULL;
    self->x_attr = NULL;
    int vendor;
    int product;
    int device;
    PyArg_ParseTuple(arg, "iii", &vendor, &product, &device);
    PyObject *vendorObj = PyLong_FromLong(vendor);
    PyObject *productObj = PyLong_FromLong(product);
    PyObject *deviceObj = PyLong_FromLong(device);
    Camera_setattr(self, "vendor", vendorObj);
    Camera_setattr(self, "product", productObj);
    Camera_setattr(self, "device", deviceObj);
    return self;
}

void Camera_dealloc(CameraObject *self)
{
    Py_XDECREF(self->x_attr);
    PyObject_Del(self);
};

PyObject *Camera_open(CameraObject *self, PyObject *args)
{
    printf("Opening camera\n");
    Py_RETURN_NONE;
};

PyObject *Camera_register_event_cb(CameraObject *self, PyObject *args) {
    printf("Registering callback\n");
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
    int device = PyLong_AsLong(PyDict_GetItemString(self->x_attr, "device"));
    return PyUnicode_FromFormat("Camera %04x:%04x (%d)", vendor, product, device);
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
