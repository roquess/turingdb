#include "PyUtils.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

void setError(const std::string& str) {
    PyErr_SetString(PyExc_RuntimeError, str.c_str());
}
