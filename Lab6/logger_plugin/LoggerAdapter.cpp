#include "LoggerAdapter.h"
#include <Python.h>

// Constructor
LoggerAdapter::LoggerAdapter()
    : m_pyInstance(nullptr), m_initialized(false),
    m_pyModule(nullptr), m_pyClass(nullptr),
    m_printLogMethod(nullptr), m_clearLogsMethod(nullptr)
{
    Py_Initialize();
}

// Destructor
LoggerAdapter::~LoggerAdapter() {
    if (m_initialized && m_pyInstance) {
        Py_DECREF((PyObject*)m_pyInstance);
    }
    if (m_pyClass) {
        Py_DECREF((PyObject*)m_pyClass);
    }
    if (m_pyModule) {
        Py_DECREF((PyObject*)m_pyModule);
    }
    Py_Finalize();
}

// Initialization of the logger
bool LoggerAdapter::initialize(const wxString& method, const wxString& file) {
    PyRun_SimpleString("import sys\nsys.path.append('.')");

    // Importing module
    m_pyModule = PyImport_ImportModule("logger");
    if (!m_pyModule) {
        PyErr_Print();
        return false;
    }

    m_pyClass = PyObject_GetAttrString((PyObject*)m_pyModule, "Logger");
    if (!m_pyClass) {
        PyErr_Print();
        return false;
    }

    PyObject* args = PyTuple_New(2);
    PyTuple_SetItem(args, 0, PyUnicode_FromString(method.ToStdString().c_str()));

    if (method == "file") {
        PyTuple_SetItem(args, 1, PyUnicode_FromString(file.ToStdString().c_str()));
    }
    else {
        PyTuple_SetItem(args, 1, PyUnicode_FromString(""));
    }

    m_pyInstance = PyObject_CallObject((PyObject*)m_pyClass, args);
    Py_DECREF(args);

    if (!m_pyInstance) {
        PyErr_Print();
        return false;
    }

    m_printLogMethod = PyObject_GetAttrString((PyObject*)m_pyInstance, "print_log");
    m_clearLogsMethod = PyObject_GetAttrString((PyObject*)m_pyInstance, "clear_logs");

    m_initialized = true;
    return true;
}

// Logs a message
void LoggerAdapter::printLog(const wxString& date, const wxString& message) {
    if (!m_initialized || !m_printLogMethod) return;

    PyObject* args = PyTuple_New(2);
    PyTuple_SetItem(args, 0, PyUnicode_FromString(date.ToStdString().c_str()));
    PyTuple_SetItem(args, 1, PyUnicode_FromString(message.ToStdString().c_str()));

    PyObject_CallObject((PyObject*)m_printLogMethod, args);
    Py_DECREF(args);
}

// Clears logs (from file)
void LoggerAdapter::clearLogs() {
    if (!m_initialized || !m_clearLogsMethod) return;

    PyObject_CallObject((PyObject*)m_clearLogsMethod, nullptr);
}