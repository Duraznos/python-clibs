#include <Python/Python.h>

#define PY_PRINTF(o) \
    PyObject_Print(o, stdout, 0); printf("\n");

static PyObject *
count_keys(PyObject *self, PyObject *args) {
    PyObject * list = NULL, *item = NULL, *iter = NULL, *agg_dict = NULL;
    PyObject * key = NULL, *val = NULL;
    PyObject * const_one = NULL;
    Py_ssize_t pos = 0;
    PyObject * result = NULL;

    const_one = PyLong_FromLong(1l);
    Py_INCREF(const_one);
    agg_dict = PyDict_New();
    if (agg_dict == NULL) {
        goto error;
    }

    if (!PyArg_ParseTuple(args, "O", &list)) {
        goto error;
    }

    if (!PyList_Check(list)) {
        goto error;
    }

    iter = PyObject_GetIter(list);
    if (!iter) {
        goto error;
    }

    while ((item = PyIter_Next(iter)) != NULL) {
        if (!PyDict_Check(item)) {
            Py_XDECREF(item);
            goto error;
        }
        while (PyDict_Next(item, &pos, &key, &val)) {
            if (!PyMapping_HasKey(agg_dict, key)) {
                if (PyObject_SetItem(agg_dict, key, const_one) < 0) {
                    Py_XDECREF(item);
                    goto error;
                }
            } else {
                PyObject * current = PyObject_GetItem(agg_dict, key);
                if (PyObject_SetItem(agg_dict, key, PyNumber_Add(PyNumber_Long(current), const_one)) < 0) {
                    Py_XDECREF(item);
                    goto error;
                }
                Py_XDECREF(current);
            }
        }
        Py_XDECREF(item);
        pos = 0;
    }
    result = agg_dict;

    error:
    Py_XDECREF(iter);
    Py_XDECREF(const_one);
    return result;
}

static PyObject *
agg_keys(PyObject *self, PyObject *args) {
    PyObject * list = NULL, *item = NULL, *iter = NULL, *agg_dict = NULL;
    PyObject * key = NULL, *val = NULL;
    PyObject * agg_list = NULL;
    Py_ssize_t pos = 0;
    PyObject * result = NULL;

    agg_dict = PyDict_New();
    if (agg_dict == NULL) {
        goto error;
    }

    if (!PyArg_ParseTuple(args, "O", &list)) {
        goto error;
    }

    if (!PyList_Check(list)) {
        goto error;
    }

    iter = PyObject_GetIter(list);
    if (!iter) {
        goto error;
    }

    while ((item = PyIter_Next(iter)) != NULL) {
        if (!PyDict_Check(item)) {
            Py_XDECREF(item);
            goto error;
        }
        while (PyDict_Next(item, &pos, &key, &val)) {
            if (!PyMapping_HasKey(agg_dict, key)) {
                agg_list = PyList_New(1);
                Py_INCREF(val);
                if (agg_list == NULL) {
                    Py_XDECREF(val);
                    goto error;
                }
                if (PyList_SET_ITEM(agg_list, 0, val) < 0) {
                    Py_XDECREF(agg_list);
                    Py_XDECREF(item);
                    Py_XDECREF(val);
                    goto error;
                }
                if (PyDict_SetItem(agg_dict, key, agg_list) < 0) {
                    Py_XDECREF(item);
                    Py_XDECREF(agg_list);
                    Py_XDECREF(val);
                    goto error;
                }
            } else {
                agg_list = PyObject_GetItem(agg_dict, key);
                if (PyList_Append(agg_list, val) < 0) {
                    Py_XDECREF(agg_list);
                    Py_XDECREF(item);
                    Py_XDECREF(val);
                    goto error;
                }
                if (PyDict_SetItem(agg_dict, key, agg_list) < 0) {
                    Py_XDECREF(item);
                    Py_XDECREF(agg_list);
                    Py_XDECREF(val);
                    goto error;
                }
                Py_XDECREF(agg_list);
                Py_XDECREF(val);
            }
        }
        pos = 0;
        Py_XDECREF(item);
    }
    result = agg_dict;

    error:
    Py_XDECREF(iter);
    return result;
}


static PyMethodDef methods[] = {
        {"count_keys", (PyCFunction) count_keys, METH_VARARGS, NULL},
        {"agg_keys", (PyCFunction) agg_keys, METH_VARARGS, NULL},
        {NULL, NULL, 0,                                        NULL}
};

static struct PyModuleDef module = {
        PyModuleDef_HEAD_INIT, "utils", NULL, -1, methods
};

PyMODINIT_FUNC PyInit_utils(void) {
    return PyModule_Create(&module);
}