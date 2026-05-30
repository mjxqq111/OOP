#include <wx/wx.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/msgdlg.h>
#include "LoggerAdapter.h"

#define EXPORT __declspec(dllexport)

// Global variables
static LoggerAdapter* g_logger = nullptr;
static wxTextCtrl* g_logDisplay = nullptr;
static wxCheckBox* g_screenCheck = nullptr;
static wxTextCtrl* g_filePathCtrl = nullptr;

// 'Log Message' button click handler 
static void onLogMessageClick(wxCommandEvent& event) {
    if (!g_logger || !g_logDisplay) return;

    wxString message = g_logDisplay->GetValue();
    if (!message.IsEmpty()) {
        wxDateTime now = wxDateTime::Now();
        wxString date = now.Format("%Y-%m-%d %H:%M:%S");

        g_logger->printLog(date, message);
        g_logDisplay->Clear();
    }
}

// 'Clear Logs' button click handler
static void onClearLogsClick(wxCommandEvent& event) {
    if (g_logger) {
        g_logger->clearLogs();
    }
}

// Checkbox handler. Changes the logger mode
static void onToggleMode(wxCommandEvent& event) {
    if (!g_logger || !g_screenCheck || !g_filePathCtrl) return;

    wxString method = g_screenCheck->IsChecked() ? "screen" : "file";
    wxString filePath = g_filePathCtrl->GetValue();

    if (method == "file" && filePath.IsEmpty()) {
        wxMessageBox("Please specify file path for file logging!",
            "Logger", wxOK | wxICON_WARNING);
        g_screenCheck->SetValue(true);
        return;
    }

    delete g_logger;
    g_logger = new LoggerAdapter();

    if (!g_logger->initialize(method, filePath)) {
        wxMessageBox("Failed to initialize logger!",
            "Logger", wxOK | wxICON_ERROR);
    }
}

// Adds UI to control panel
static void addLoggerUI(wxWindow* controlPanel) {
    if (!controlPanel) return;

    wxSizer* sizer = controlPanel->GetSizer();
    if (!sizer) return;

    wxStaticText* title = new wxStaticText(controlPanel, wxID_ANY, "Logger Adapter");
    wxFont titleFont = title->GetFont();
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    title->SetFont(titleFont);
    sizer->Add(title, 0, wxALL, 5);

    g_screenCheck = new wxCheckBox(controlPanel, wxID_ANY, "Screen Mode");
    g_screenCheck->SetValue(true);
    sizer->Add(g_screenCheck, 0, wxALL, 5);

    wxStaticText* fileLabel = new wxStaticText(controlPanel, wxID_ANY, "File:");
    sizer->Add(fileLabel, 0, wxALIGN_CENTER | wxLEFT, 10);

    g_filePathCtrl = new wxTextCtrl(controlPanel, wxID_ANY, "logs.txt");
    sizer->Add(g_filePathCtrl, 1, wxEXPAND | wxALL, 5);

    g_logDisplay = new wxTextCtrl(controlPanel, wxID_ANY, "",
        wxDefaultPosition, wxDefaultSize);
    sizer->Add(g_logDisplay, 1, wxALL, 5);

    wxButton* logBtn = new wxButton(controlPanel, wxID_ANY, "Log Message");
    sizer->Add(logBtn, 0, wxALL, 5);

    wxButton* clearBtn = new wxButton(controlPanel, wxID_ANY, "Clear Logs");
    sizer->Add(clearBtn, 0, wxALIGN_CENTER | wxALL, 5);

    controlPanel->Layout();

    // Binding events
    logBtn->Bind(wxEVT_BUTTON, onLogMessageClick);
    clearBtn->Bind(wxEVT_BUTTON, onClearLogsClick);
    g_screenCheck->Bind(wxEVT_CHECKBOX, onToggleMode);
}

// Plugin initialization
extern "C" EXPORT void initPlugin(void* registerShapeFunc, void* registerDrawFunc) {
    g_logger = new LoggerAdapter();
    g_logger->initialize("screen", "");
}

// UI initialization
extern "C" EXPORT void initUI(void* controlPanel, void* canvas) {
    addLoggerUI((wxWindow*)controlPanel);
}

// Get the plugin name
extern "C" EXPORT const char* getPluginName() {
    return "LoggerAdapter";
}