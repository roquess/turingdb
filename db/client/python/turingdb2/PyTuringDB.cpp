#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "PyTuring.h"
#include "PyUtils.h"
#include "PyExceptions.h"

namespace {

PyMethodDef turingdb_methods[] = {
    {NULL, NULL, 0, NULL}
};

struct PyModuleDef turingDBModule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "turingdb2",
    .m_doc = "Python client for Turing Biosystems platform",
    .m_size = -1,
    .m_methods = turingdb_methods,
    .m_slots = NULL
};

}

PyMODINIT_FUNC PyInit_turingdb2(void) {
    PyObject* mod = PyModule_Create(&turingDBModule);
    if (!mod) {
        setError("ERROR: failed to initialize turingdb python module.");
        return NULL;
    }

    if (!addTuringType(mod)) {
        setError("ERROR: failed to create Turing class.\n");
        return NULL;
    }

    if (!addExceptions()) {
        setError("ERROR: failed to register turingdb exceptions.");
        return NULL;
    }

    return mod;
}
