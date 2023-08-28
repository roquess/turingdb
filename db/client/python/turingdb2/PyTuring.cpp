#include "PyTuring.h"

#include "TuringClient.h"
#include "PyExceptions.h"
#include "PyUtils.h"

namespace {

struct TuringWrapper {
    PyObject_HEAD
    turing::db::client::TuringClient* _turing {nullptr};

    static PyObject* create(PyTypeObject* subtype, PyObject* args, PyObject* kwargs) {
        PyObject* wrapper = subtype->tp_alloc(subtype, 0);
        if (!wrapper) {
            return NULL;
        }
        ((TuringWrapper*)wrapper)->_turing = new turing::db::client::TuringClient();
        return wrapper;
    }

    static void destroy(TuringWrapper* wrapper) {
        delete wrapper->_turing;
        wrapper->_turing = NULL;
        Py_TYPE(wrapper)->tp_free(wrapper);
    }

    static PyObject* connect(TuringWrapper* self, PyObject* Py_UNUSED(ignored)) {
        const bool connectRes = self->_turing->connect();
        if (!connectRes) {
            const auto& config = self->_turing->getConfig();
            const std::string msg = "failed to connect to Turing database at "
                + config.getAddress() + ":" + std::to_string(config.getPort());
            PyErr_SetString(getConnectError(), msg.c_str());
        }

        return Py_None;
    }

    static PyObject* exec(TuringWrapper* self, PyObject* arg) {
        if (PyUnicode_READY(arg) != 0) {
            setError("Failed to get canonical representation of argument string");
            return Py_None;
        }

        const char* data = (const char*)PyUnicode_DATA(arg);
        if (!data) {
            setError("Failed to get argument string");
            return Py_None;
        }

        const bool execRes = self->_turing->exec(data);
        if (!execRes) {
            const std::string msg = "Execution of query failed";
            PyErr_SetString(getExecuteError(), msg.c_str());
        }
        return Py_None;
    }
};

PyMethodDef TuringWrapper_methods[] = {
    {"connect", (PyCFunction)TuringWrapper::connect, METH_NOARGS,
        "Start the connection to the Turing platform"},
    {"exec", (PyCFunction)TuringWrapper::exec, METH_O,
        "Execute a query string"},
    {NULL} 
};

PyMemberDef TuringWrapper_members[] = {
    {NULL}
};

PyTypeObject turingType;

}

bool addTuringType(PyObject* mod) {
    memset(&turingType, 0, sizeof(turingType));
    turingType.tp_name = "turingdb2.Turing";
    turingType.tp_basicsize = sizeof(TuringWrapper);
    turingType.tp_dealloc = (destructor)TuringWrapper::destroy;
    turingType.tp_flags = Py_TPFLAGS_DEFAULT;
    turingType.tp_new = (newfunc)TuringWrapper::create;
    turingType.tp_members = TuringWrapper_members;
    turingType.tp_methods = TuringWrapper_methods;

    if (PyType_Ready(&turingType) < 0) {
        return false;
    }

    if (PyModule_AddObject(mod, "Turing", (PyObject*)&turingType) < 0) {
        return false;
    }

    return true;
}
