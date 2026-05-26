#include "EditorFrame.h"
#include "ShapeFactory.h"
#include "PluginManager.h"

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

    // Load Plugin button
    wxButton* loadPluginBtn = new wxButton(toolBar, wxID_ANY, "Load Plugin");
    toolSizer->Add(loadPluginBtn, 0, wxALL, 5);

    // Plugins info button
    wxButton* pluginsBtn = new wxButton(toolBar, wxID_ANY, "Plugins");
    toolSizer->Add(pluginsBtn, 0, wxALL, 5);

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
    loadPluginBtn->Bind(wxEVT_BUTTON, &EditorFrame::onLoadPlugin, this);
    pluginsBtn->Bind(wxEVT_BUTTON, &EditorFrame::onShowPlugins, this);
    clearBtn->Bind(wxEVT_BUTTON, &EditorFrame::onClear, this);

    m_canvas->setCurrentShapeType("Rectangle");
}

void EditorFrame::updateShapeCombo() {
    // Save current selection
    wxString current = m_shapeCombo->GetValue();

    // Get updated shape list
    wxArrayString shapeTypes = ShapeFactory::getAvailableTypes();

    // Update combo box
    m_shapeCombo->Clear();
    m_shapeCombo->Append(shapeTypes);

    // Restore selection if still exists
    int index = m_shapeCombo->FindString(current);
    if (index != wxNOT_FOUND) {
        m_shapeCombo->SetSelection(index);
    }
    else if (shapeTypes.GetCount() > 0) {
        m_shapeCombo->SetSelection(0);
        m_canvas->setCurrentShapeType(shapeTypes[0]);
    }
}

void EditorFrame::onLoadPlugin(wxCommandEvent& WXUNUSED(evt)) {
    // Create file dialog for selecting DLL
    wxFileDialog openFileDialog(
        this,
        "Select Plugin DLL",           // Title
        "",                            // Default directory
        "",                            // Default filename
        "DLL files (*.dll)|*.dll|All files (*.*)|*.*",  // File filters
        wxFD_OPEN | wxFD_FILE_MUST_EXIST
    );

    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;  // User cancelled
    }

    wxString dllPath = openFileDialog.GetPath();

    // Trying to load the plugin
    if (PluginManager::getInstance().loadPluginFromFile(dllPath)) {
        // Update the shape combo box with new shapes
        updateShapeCombo();

        // Show message about the new shape
        wxMessageBox("Plugin loaded!\n\nNew shape available in the list.",
            "Success", wxOK | wxICON_INFORMATION);
    }
}

// When we select a shape
void EditorFrame::onSelectShape(wxCommandEvent& evt) {
    m_canvas->setCurrentShapeType(evt.GetString());
}

// When we press 'Clear All'
void EditorFrame::onClear(wxCommandEvent& WXUNUSED(evt)) {
    m_canvas->clearAll();
}

void EditorFrame::onShowPlugins(wxCommandEvent& WXUNUSED(evt)) {
    wxString msg = "Loaded plugins:\n\n";
    auto& plugins = PluginManager::getInstance().getPlugins();

    if (plugins.empty()) {
        msg += "No plugins loaded.";
    }
    else {
        for (const auto& p : plugins) {
            msg += "• " + p.name + "\n";
        }
    }

    wxMessageBox(msg, "Plugins", wxOK | wxICON_INFORMATION, this);
}