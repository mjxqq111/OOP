#include "EditorFrame.h"
#include "ShapeFactory.h"
#include "PluginManager.h"
#include <wx/filedlg.h>
#include <wx/xml/xml.h>
#include <wx/textfile.h>
#include <wx/stream.h>

// Main window
EditorFrame::EditorFrame()
    : wxFrame(nullptr, wxID_ANY, "Graphics editor",
        wxDefaultPosition, wxSize(800, 600))
{
    // Setting up plugin settings window
    m_pluginSettingsFrame = new PluginSettingsFrame(this);

    // Adding a menu bar
    m_menuBar = new wxMenuBar();
    m_menu = new wxMenu();
    m_menuItem = new wxMenuItem(m_menu, ID_OPTIONS_PLUGIN_SETTINGS, "Plugin Settings");
    m_menu->Append(m_menuItem);
    m_menuBar->Append(m_menu, "Options");
    SetMenuBar(m_menuBar);

    // Bind menu event
    Bind(wxEVT_MENU, &EditorFrame::onMenuClick, this, m_menuItem->GetId());

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

    // Save and load buttons
    m_saveBtn = new wxButton(m_controlPanel, wxID_ANY, "Save Canvas");
    controlSizer->Add(m_saveBtn, 0, wxALL, 3);

    m_loadBtn = new wxButton(m_controlPanel, wxID_ANY, "Load Canvas");
    controlSizer->Add(m_loadBtn, 0, wxALL, 3);

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
    m_saveBtn->Bind(wxEVT_BUTTON, &EditorFrame::onSaveCanvasClick, this);
    m_loadBtn->Bind(wxEVT_BUTTON, &EditorFrame::onLoadCanvasClick, this);

    // Creating shape buttons
    updateShapeButtons();
}

// Seleting menu element handler
void EditorFrame::onMenuClick(wxCommandEvent& evt) {
    switch (evt.GetId()) {
    case ID_OPTIONS_PLUGIN_SETTINGS:
        m_pluginSettingsFrame->Show();
        m_pluginSettingsFrame->Raise();
    }
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

        // Initializing plugin UI and more
        if (PluginManager::getInstance().getPlugins().empty()) return;

        const PluginInfo& last_plugin = PluginManager::getInstance().getPlugins().back();

        // Setting save handler
        if (last_plugin.handleSaveFunc) {
            m_manager.saveHandler = last_plugin.handleSaveFunc;
        }

        // Setting load handler
        if (last_plugin.handleLoadFunc) {
            m_manager.loadHandler = last_plugin.handleLoadFunc;
        }

        // Loading UI
        if (last_plugin.handleSaveFunc || last_plugin.handleLoadFunc) {
            if (last_plugin.initUIFunc) {
                PluginManager::getInstance().initPluginUI(last_plugin, m_pluginSettingsFrame->m_panel, nullptr);
            }
        }
        else {
            if (last_plugin.initUIFunc) {
                PluginManager::getInstance().initPluginUI(last_plugin, m_controlPanel, m_canvas);
            }
        }
        wxMessageBox("Plugin loaded!", "Success", wxOK | wxICON_INFORMATION);
    }
}

// 'Save Canvas' button click handler
void EditorFrame::onSaveCanvasClick(wxCommandEvent& WXUNUSED(evt)) {
    wxFileDialog saveDialog(this, "Save Canvas", "", "",
        "XML files (*.xml)|*.xml",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveDialog.ShowModal() == wxID_CANCEL) return;

    wxString filename = saveDialog.GetPath();

    if (m_manager.saveToFile(m_canvas, filename)) {
        wxMessageBox("Canvas saved successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Failed to save canvas!", "Error", wxOK | wxICON_ERROR);
    }
}

// 'Load Canvas' button click handler
void EditorFrame::onLoadCanvasClick(wxCommandEvent & WXUNUSED(evt)) {
    wxFileDialog openDialog(this, "Load Canvas", "", "",
        "XML files (*.xml)|*.xml",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openDialog.ShowModal() == wxID_CANCEL) return;

    wxString filename = openDialog.GetPath();
    if (m_manager.loadFromFile(m_canvas, filename)) {
        wxMessageBox("Canvas loaded successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
    else {
        wxMessageBox("Failed to load canvas!", "Error", wxOK | wxICON_ERROR);
    }
}

// Close window event handler
void EditorFrame::OnClose(wxCloseEvent& evt) {
    if (m_pluginSettingsFrame) {
        m_pluginSettingsFrame->Destroy();
        m_pluginSettingsFrame = nullptr;
    }
    evt.Skip();
}