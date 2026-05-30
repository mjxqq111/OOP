#include "PluginSettingsFrame.h"

// Constructor
PluginSettingsFrame::PluginSettingsFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, "Plugin Settings")
{
    Bind(wxEVT_CLOSE_WINDOW, &PluginSettingsFrame::OnClose, this);

    m_panel = new wxPanel(this);
    m_panel->SetBackgroundColour(*wxWHITE);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_panel->SetSizer(sizer);
}

// Close window event handler
void PluginSettingsFrame::OnClose(wxCloseEvent& evt) {
    Hide();
}