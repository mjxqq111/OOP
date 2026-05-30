#pragma once

#include <wx/wx.h>
#include "PluginSettingsFrame.h"
#include "EditorCanvas.h"
#include "CanvasFileManager.h"

// Menu IDs
enum {
    ID_OPTIONS_PLUGIN_SETTINGS = 1001
};

// Main application window containing toolbar and drawing area
class EditorFrame : public wxFrame {
public:
    // Constructor. Builds the main window layout and initializes UI components
    EditorFrame();

private:
    void onMenuClick(wxCommandEvent& evt);  // Menu bar handler
    void onShapeButtonClick(wxCommandEvent& evt);   // Shape button click handler
    void onClearAllClick(wxCommandEvent& evt);  // 'Clear All' button click handler
    void onPluginsClick(wxCommandEvent& evt);    // 'Plugins' button click handler
    void onLoadPluginClick(wxCommandEvent& evt); // 'Load Plugin' button click handler
    void onSaveCanvasClick(wxCommandEvent& evt);   // 'Save Canvas' button click handler
    void onLoadCanvasClick(wxCommandEvent& evt);   // 'Load Canvas' button click handler

    void updateShapeButtons();  // Updates shape buttons
    void removeShapeButtons();   // Removes shape buttons
    void highlightButton(wxButton* btn);    // Highlights button when pressed

    void OnClose(wxCloseEvent& evt);    // Handling close window event

    PluginSettingsFrame* m_pluginSettingsFrame; // Plugin Settings window

    EditorCanvas* m_canvas; // Drawing area where shapes are rendered
    wxPanel* m_mainPanel;
    wxPanel* m_controlPanel;
    wxPanel* m_shapePanel;
    wxBoxSizer* shapeSizer;

    // Menu bar items
    wxMenuBar* m_menuBar;
    wxMenu* m_menu;
    wxMenuItem* m_menuItem;
    
    std::vector<wxButton*> m_shapeButtons;
    wxButton* m_currentSelectedBtn;
    
    wxButton* m_loadPluginBtn;
    wxButton* m_pluginsBtn;
    wxButton* m_clearBtn;
    wxButton* m_saveBtn;
    wxButton* m_loadBtn;

    CanvasFileManager m_manager;

    const wxColour btn_color = wxColour(*wxWHITE);
    const wxColour btn_select_color = wxColour(0, 100, 200);
};