#include "PyExceptions.h"

namespace {

PyObject* connectErrorType = NULL;
PyObject* executeErrorType = NULL;

}

bool addExceptions() {
    connectErrorType = PyErr_NewException("turingdb.ConnectionError",
                                          PyExc_Exception,
                                          NULL);
    if (!connectErrorType) {
        return false;
    }

    executeErrorType = PyErr_NewException("turingdb.ExecuteError",
                                          PyExc_Exception,
                                          NULL);
    if (!executeErrorType) {
        return false;
    }

    return true;
}

PyObject* getConnectError() {
    return connectErrorType;
}

PyObject* getExecuteError() {
    return executeErrorType;
}
