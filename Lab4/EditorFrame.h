#pragma once

#include <wx/wx.h>
#include "EditorCanvas.h"

// Main application window containing toolbar and drawing area
class EditorFrame : public wxFrame {
public:
    // Constructor. Builds the main window layout and initializes UI components
    EditorFrame();

private:
    // Event handler for shape selection from dropdown menu
    void onSelectShape(wxCommandEvent& evt);

    // Event handler for clear button click (removes all shapes from the canvas)
    void onClear(wxCommandEvent& evt);

    // Show plugin list
    void onShowPlugins(wxCommandEvent& evt);

    EditorCanvas* m_canvas;      // Drawing area where shapes are rendered
    wxComboBox* m_shapeCombo;    // Dropdown menu for selecting shape type
};