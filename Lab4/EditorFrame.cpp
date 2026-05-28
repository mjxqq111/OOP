#include "EditorFrame.h"
#include "ShapeFactory.h"
#include "PluginManager.h"

// Main window
EditorFrame::EditorFrame()
    : wxFrame(nullptr, wxID_ANY, "Graphics editor",
        wxDefaultPosition, wxSize(800, 600))
{
    // Creating main panel
    m_mainPanel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Creating shape panel for shape selection buttons
    m_shapePanel = new wxPanel(m_mainPanel, wxID_ANY);
    m_shapePanel->SetBackgroundColour(wxColour(220, 220, 220));
    mainSizer->Add(m_shapePanel, 0, wxEXPAND);

    // Creating control panel
    m_controlPanel = new wxPanel(m_mainPanel, wxID_ANY);
    m_controlPanel->SetName("controlPanel");
    m_controlPanel->SetBackgroundColour(wxColour(220, 220, 220));
    wxBoxSizer* controlSizer = new wxBoxSizer(wxHORIZONTAL);

    m_loadPluginBtn = new wxButton(m_controlPanel, wxID_ANY, "Load Plugin", wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    controlSizer->Add(m_loadPluginBtn, 0, wxALL, 3);

    m_pluginsBtn = new wxButton(m_controlPanel, wxID_ANY, "Plugins");
    controlSizer->Add(m_pluginsBtn, 0, wxALL, 3);

    m_clearBtn = new wxButton(m_controlPanel, wxID_ANY, "Clear All");
    controlSizer->Add(m_clearBtn, 0, wxALL, 3);

    m_controlPanel->SetSizer(controlSizer);
    
    mainSizer->Add(m_controlPanel, 0, wxEXPAND);

    // Canvas
    m_canvas = new EditorCanvas(m_mainPanel);
    m_canvas->SetBackgroundColour(*wxWHITE);
    mainSizer->Add(m_canvas, 1, wxEXPAND);

    m_mainPanel->SetSizer(mainSizer);

    // Bind events
    m_loadPluginBtn->Bind(wxEVT_BUTTON, &EditorFrame::onLoadPluginClick, this);
    m_pluginsBtn->Bind(wxEVT_BUTTON, &EditorFrame::onPluginsClick, this);
    m_clearBtn->Bind(wxEVT_BUTTON, &EditorFrame::onClearAllClick, this);

    // Creating shape buttons
    updateShapeButtons();
}

// Highlights a shape button
void EditorFrame::highlightButton(wxButton* btn) {
    // Reset previous button color
    if (m_currentSelectedBtn) {
        m_currentSelectedBtn->SetBackgroundColour(btn_color);
    }

    // Highlight new button
    m_currentSelectedBtn = btn;
    m_currentSelectedBtn->SetBackgroundColour(btn_select_color);
    m_currentSelectedBtn->Refresh();
}

// Shape button click handler
void EditorFrame::onShapeButtonClick(wxCommandEvent& evt) {
    wxButton* btn = static_cast<wxButton*>(evt.GetEventObject());
    highlightButton(btn);
    m_canvas->setCurrentShapeType(btn->GetLabel());
}

// Removes shape selection buttons
void EditorFrame::removeShapeButtons() {
    for (auto* btn : m_shapeButtons) {
        btn->Unbind(wxEVT_BUTTON, &EditorFrame::onShapeButtonClick, this);
        shapeSizer->Detach(btn);
        btn->Destroy();
    }
    m_shapeButtons.clear();
    m_currentSelectedBtn = nullptr;
}

// Updates shape selection buttons
void EditorFrame::updateShapeButtons() {
    removeShapeButtons();
    wxArrayString shapes = ShapeFactory::getAvailableTypes();
    shapeSizer = new wxBoxSizer(wxHORIZONTAL);

    // Adding shape buttons
    for (const auto& shape : shapes) {
        wxButton* btn = new wxButton(m_shapePanel, wxID_ANY, shape, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
        btn->SetWindowStyleFlag(btn->GetWindowStyleFlag() | wxBU_EXACTFIT);
        btn->SetBackgroundColour(btn_color);
        btn->SetForegroundColour(*wxBLACK);

        btn->SetMinSize(wxSize(65, 25));
        shapeSizer->Add(btn, 0, wxALL, 3);
        btn->Bind(wxEVT_BUTTON, &EditorFrame::onShapeButtonClick, this);
        m_shapeButtons.push_back(btn);
    }

    m_shapePanel->SetSizer(shapeSizer);
    m_shapePanel->Layout();

    // Set default selection
    if (!m_shapeButtons.empty() && !m_currentSelectedBtn) {
        highlightButton(m_shapeButtons[0]);
        m_canvas->setCurrentShapeType(m_shapeButtons[0]->GetLabel());
    }

    // Refresh layout
    m_shapePanel->GetParent()->Layout();
}

// 'Clear All' button click handler
void EditorFrame::onClearAllClick(wxCommandEvent& WXUNUSED(evt)) {
    m_canvas->clearAll();
}

// 'Plugins' button click handler
void EditorFrame::onPluginsClick(wxCommandEvent& WXUNUSED(evt)) {
    wxString msg = "Loaded plugins:\n\n";
    auto& plugins = PluginManager::getInstance().getPlugins();

    if (plugins.empty()) {
        msg += "No plugins loaded";
    }
    else {
        for (const auto& p : plugins) {
            msg += "- " + p.name + "\n";
        }
    }

    wxMessageBox(msg, "Plugins", wxOK | wxICON_INFORMATION, this);
}

// 'Load Plugin' button click handler
void EditorFrame::onLoadPluginClick(wxCommandEvent& WXUNUSED(evt)) {
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
        updateShapeButtons();

        // Initializing plugin UI
        if (!PluginManager::getInstance().getPlugins().empty()) {
            const PluginInfo& last_plugin = PluginManager::getInstance().getPlugins().back();
            if (last_plugin.initUIFunc) {
                PluginManager::getInstance().initPluginUI(last_plugin, m_controlPanel, m_canvas);
            }

            wxMessageBox("Plugin loaded!", "Success", wxOK | wxICON_INFORMATION);
        }
    }
}