#include "EditorFrame.h"
#include "ShapeFactory.h"

// Main window
EditorFrame::EditorFrame()
    : wxFrame(nullptr, wxID_ANY, "Graphics editor",
        wxDefaultPosition, wxSize(800, 600))
{
    wxPanel* mainPanel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Toolbar
    wxPanel* toolBar = new wxPanel(mainPanel);
    wxBoxSizer* toolSizer = new wxBoxSizer(wxHORIZONTAL);

    toolSizer->Add(new wxStaticText(toolBar, wxID_ANY, "Shape:"),
        0, wxALIGN_CENTER | wxALL, 5);

    m_shapeCombo = new wxComboBox(toolBar, wxID_ANY, "Rectangle",
        wxDefaultPosition, wxSize(100, -1),
        ShapeFactory::getAvailableTypes(),
        wxCB_READONLY);
    toolSizer->Add(m_shapeCombo, 0, wxALL, 5);

    wxButton* clearBtn = new wxButton(toolBar, wxID_ANY, "Clear All");
    toolSizer->Add(clearBtn, 0, wxALL, 5);

    toolBar->SetSizer(toolSizer);

    // Canvas
    m_canvas = new EditorCanvas(mainPanel);

    // Layout
    sizer->Add(toolBar, 0, wxEXPAND);
    sizer->Add(m_canvas, 1, wxEXPAND);
    mainPanel->SetSizer(sizer);

    // Bind events
    m_shapeCombo->Bind(wxEVT_COMBOBOX, &EditorFrame::onSelectShape, this);
    clearBtn->Bind(wxEVT_BUTTON, &EditorFrame::onClear, this);

    m_canvas->setCurrentShapeType("Rectangle");
}

// When we select a shape
void EditorFrame::onSelectShape(wxCommandEvent& evt) {
    m_canvas->setCurrentShapeType(evt.GetString());
}

// When we press 'Clear All'
void EditorFrame::onClear(wxCommandEvent& WXUNUSED(evt)) {
    m_canvas->clearAll();
}