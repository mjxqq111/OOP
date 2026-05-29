#include "EditorFrame.h"
#include "ShapeFactory.h"
#include "PluginManager.h"
#include <wx/filedlg.h>
#include <wx/xml/xml.h>
#include <wx/textfile.h>
#include <wx/stream.h>
#include <sstream>

#include <stack>
#include <cctype>
#include <iomanip>

#include <wx/mstream.h>
#include <wx/sstream.h>

wxString handle1(const wxString& xmlContent);
wxString handle2(const wxString& json_str);


// Main window
EditorFrame::EditorFrame()
    : wxFrame(nullptr, wxID_ANY, "Graphics editor",
        wxDefaultPosition, wxSize(800, 600))
{
    ////////////
    m_manager.saveHandler = handle1;
    m_manager.loadHandler = handle2;
    ////////////

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

// 'Save Canvas' button click handler
void EditorFrame::onSaveCanvasClick(wxCommandEvent& WXUNUSED(evt)) {
    wxFileDialog saveDialog(this, "Save Canvas", "", "",
        "XML files (*.xml)|*.xml",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveDialog.ShowModal() == wxID_CANCEL) return;

    wxString filename = saveDialog.GetPath();

    // Getting XML
    //...

    // Forming output file content
    // ...

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

/////////////////////////////

wxString handle1(const wxString& xmlContent) {
    if (xmlContent.IsEmpty()) return "{}";

    wxStringInputStream stream(xmlContent);

    wxXmlDocument doc;
    if (!doc.Load(stream)) {
        return "{\"error\": \"Invalid XML format\"}";
    }

    wxXmlNode* root = doc.GetRoot();
    if (!root) {
        return "{\"error\": \"Empty XML document\"}";
    }

    std::stringstream json;
    json << "{\n";

    // Process root element
    json << "  \"" << root->GetName().ToStdString() << "\": {\n";

    // Process children
    wxXmlNode* child = root->GetChildren();
    bool firstChild = true;

    while (child) {
        if (!firstChild) {
            json << ",\n";
        }
        firstChild = false;

        // Get element name
        std::string elemName = child->GetName().ToStdString();

        // Check if it's a Figure element
        if (child->GetName() == "Figure") {
            json << "    \"" << elemName << "\": {\n";

            // Get Type attribute
            wxString type = child->GetAttribute("Type");
            json << "      \"Type\": \"" << type.ToStdString() << "\",\n";

            // Process Start and End elements
            wxXmlNode* prop = child->GetChildren();
            bool firstProp = true;

            while (prop) {
                if (!firstProp) {
                    json << ",\n";
                }
                firstProp = false;

                std::string propName = prop->GetName().ToStdString();
                json << "      \"" << propName << "\": {";

                // Get attributes
                wxXmlAttribute* attr = prop->GetAttributes();
                bool firstAttr = true;

                while (attr) {
                    if (!firstAttr) {
                        json << ", ";
                    }
                    firstAttr = false;

                    std::string attrName = attr->GetName().ToStdString();
                    std::string attrValue = attr->GetValue().ToStdString();

                    json << "\"" << attrName << "\": " << attrValue;

                    attr = attr->GetNext();
                }

                json << "}";
                prop = prop->GetNext();
            }

            json << "\n    }";
        }
        else {
            // Handle other elements
            json << "    \"" << elemName << "\": \"" << child->GetNodeContent().ToStdString() << "\"";
        }

        child = child->GetNext();
    }

    json << "\n  }\n}";

    return wxString(json.str());
}

wxString handle2(const wxString& json_str) {
    std::string json = json_str.ToStdString();
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Canvas>\n";

    size_t pos = 0;
    while (true) {
        size_t figurePos = json.find("\"Figure\"", pos);
        if (figurePos == std::string::npos) break;

        // Находим объект Figure целиком
        size_t openBrace = json.find("{", figurePos);
        if (openBrace == std::string::npos) break;

        int braceCount = 1;
        size_t closeBrace = openBrace + 1;
        while (braceCount > 0 && closeBrace < json.length()) {
            if (json[closeBrace] == '{') braceCount++;
            else if (json[closeBrace] == '}') braceCount--;
            closeBrace++;
        }

        if (braceCount != 0) break;
        std::string figure = json.substr(openBrace, closeBrace - openBrace);

        // Извлечение Type
        size_t typePos = figure.find("\"Type\"");
        if (typePos == std::string::npos) { pos = closeBrace; continue; }
        size_t typeColon = figure.find(":", typePos);
        if (typeColon == std::string::npos) { pos = closeBrace; continue; }
        size_t typeQuote1 = figure.find("\"", typeColon + 1);
        if (typeQuote1 == std::string::npos) { pos = closeBrace; continue; }
        size_t typeQuote2 = figure.find("\"", typeQuote1 + 1);
        if (typeQuote2 == std::string::npos) { pos = closeBrace; continue; }
        std::string type = figure.substr(typeQuote1 + 1, typeQuote2 - typeQuote1 - 1);

        // Извлечение Start
        size_t startPos = figure.find("\"Start\"");
        if (startPos == std::string::npos) { pos = closeBrace; continue; }
        size_t startColon = figure.find(":", startPos);
        if (startColon == std::string::npos) { pos = closeBrace; continue; }
        size_t startBrace = figure.find("{", startColon);
        if (startBrace == std::string::npos) { pos = closeBrace; continue; }

        int startBraceCount = 1;
        size_t startEndBrace = startBrace + 1;
        while (startBraceCount > 0 && startEndBrace < figure.length()) {
            if (figure[startEndBrace] == '{') startBraceCount++;
            else if (figure[startEndBrace] == '}') startBraceCount--;
            startEndBrace++;
        }

        std::string startObj = figure.substr(startBrace, startEndBrace - startBrace);

        double startX = 0, startY = 0;
        size_t xPos = startObj.find("\"x\"");
        if (xPos != std::string::npos) {
            size_t xColon = startObj.find(":", xPos);
            if (xColon != std::string::npos) {
                size_t xStart = xColon + 1;
                while (xStart < startObj.length() && (startObj[xStart] == ' ' || startObj[xStart] == '\n')) xStart++;
                std::string xNum;
                while (xStart < startObj.length() && (isdigit(startObj[xStart]) || startObj[xStart] == '.' || startObj[xStart] == '-')) {
                    xNum += startObj[xStart++];
                }
                if (!xNum.empty()) startX = std::stod(xNum);
            }
        }

        size_t yPos = startObj.find("\"y\"");
        if (yPos != std::string::npos) {
            size_t yColon = startObj.find(":", yPos);
            if (yColon != std::string::npos) {
                size_t yStart = yColon + 1;
                while (yStart < startObj.length() && (startObj[yStart] == ' ' || startObj[yStart] == '\n')) yStart++;
                std::string yNum;
                while (yStart < startObj.length() && (isdigit(startObj[yStart]) || startObj[yStart] == '.' || startObj[yStart] == '-')) {
                    yNum += startObj[yStart++];
                }
                if (!yNum.empty()) startY = std::stod(yNum);
            }
        }

        // Извлечение End
        size_t endPos = figure.find("\"End\"");
        if (endPos == std::string::npos) { pos = closeBrace; continue; }
        size_t endColon = figure.find(":", endPos);
        if (endColon == std::string::npos) { pos = closeBrace; continue; }
        size_t endBrace = figure.find("{", endColon);
        if (endBrace == std::string::npos) { pos = closeBrace; continue; }

        int endBraceCount = 1;
        size_t endEndBrace = endBrace + 1;
        while (endBraceCount > 0 && endEndBrace < figure.length()) {
            if (figure[endEndBrace] == '{') endBraceCount++;
            else if (figure[endEndBrace] == '}') endBraceCount--;
            endEndBrace++;
        }

        std::string endObj = figure.substr(endBrace, endEndBrace - endBrace);

        double endX = 0, endY = 0;
        xPos = endObj.find("\"x\"");
        if (xPos != std::string::npos) {
            size_t xColon = endObj.find(":", xPos);
            if (xColon != std::string::npos) {
                size_t xStart = xColon + 1;
                while (xStart < endObj.length() && (endObj[xStart] == ' ' || endObj[xStart] == '\n')) xStart++;
                std::string xNum;
                while (xStart < endObj.length() && (isdigit(endObj[xStart]) || endObj[xStart] == '.' || endObj[xStart] == '-')) {
                    xNum += endObj[xStart++];
                }
                if (!xNum.empty()) endX = std::stod(xNum);
            }
        }

        yPos = endObj.find("\"y\"");
        if (yPos != std::string::npos) {
            size_t yColon = endObj.find(":", yPos);
            if (yColon != std::string::npos) {
                size_t yStart = yColon + 1;
                while (yStart < endObj.length() && (endObj[yStart] == ' ' || endObj[yStart] == '\n')) yStart++;
                std::string yNum;
                while (yStart < endObj.length() && (isdigit(endObj[yStart]) || endObj[yStart] == '.' || endObj[yStart] == '-')) {
                    yNum += endObj[yStart++];
                }
                if (!yNum.empty()) endY = std::stod(yNum);
            }
        }

        // Формирование XML
        auto formatDouble = [](double val) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << val;
            return oss.str();
            };

        xml += "  <Figure Type=\"" + type + "\">\n";
        xml += "    <Start x=\"" + formatDouble(startX) + "\" y=\"" + formatDouble(startY) + "\"/>\n";
        xml += "    <End x=\"" + formatDouble(endX) + "\" y=\"" + formatDouble(endY) + "\"/>\n";
        xml += "  </Figure>\n";

        pos = closeBrace;
    }

    xml += "</Canvas>";
    return wxString(xml);
}