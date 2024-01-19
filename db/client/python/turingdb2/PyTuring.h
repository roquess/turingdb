#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

bool addTuringType(PyObject* mod);
