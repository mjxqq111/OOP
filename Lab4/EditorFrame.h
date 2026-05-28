#pragma once

#include <wx/wx.h>
#include "EditorCanvas.h"

// Main application window containing toolbar and drawing area
class EditorFrame : public wxFrame {
public:
    // Constructor. Builds the main window layout and initializes UI components
    EditorFrame();

private:
    void onShapeButtonClick(wxCommandEvent& evt);   // Shape button click handler
    void onClearAllClick(wxCommandEvent& evt);  // 'Clear All' button click handler
    void onPluginsClick(wxCommandEvent& evt);    // 'Plugins' button click handler
    void onLoadPluginClick(wxCommandEvent& evt); // 'Load Plugin' button click handler

    void updateShapeButtons();  // Updates shape buttons
    void removeShapeButtons();   // Removes shape buttons
    void highlightButton(wxButton* btn);    // Highlights button when pressed

    EditorCanvas* m_canvas; // Drawing area where shapes are rendered
    wxPanel* m_mainPanel;
    wxPanel* m_controlPanel;
    wxPanel* m_shapePanel;
    wxBoxSizer* shapeSizer;
    
    std::vector<wxButton*> m_shapeButtons;
    wxButton* m_currentSelectedBtn;
    
    wxButton* m_loadPluginBtn;
    wxButton* m_pluginsBtn;
    wxButton* m_clearBtn;

    const wxColour btn_color = wxColour(*wxWHITE);
    const wxColour btn_select_color = wxColour(0, 100, 200);
};