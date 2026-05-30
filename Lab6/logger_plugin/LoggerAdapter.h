#pragma once

#include <wx/wx.h>
#include <string>

class LoggerAdapter {
public:
    // Constructor
    LoggerAdapter();

    // Destructor
    ~LoggerAdapter();

    // Logger initialization
    bool initialize(const wxString& method, const wxString& file = "");

    // Logs a message with a date
    void printLog(const wxString& date, const wxString& message);

    // Clears logs (for files)
    void clearLogs();
private:
    void* m_pyInstance;
    bool m_initialized;
    void* m_pyModule;
    void* m_pyClass;
    void* m_printLogMethod;
    void* m_clearLogsMethod;
};