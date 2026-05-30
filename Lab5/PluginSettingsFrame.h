#pragma once

#include <wx/wx.h>

// A window with plugin settings
class PluginSettingsFrame : public wxFrame {
public:
    // Constructor
    PluginSettingsFrame(wxWindow* parent);

    wxPanel* m_panel;
private:
    // Close window event handler
    void OnClose(wxCloseEvent& evt);
};