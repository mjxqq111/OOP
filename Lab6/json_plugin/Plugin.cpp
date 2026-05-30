#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <sstream>
#include <iomanip>
#include <wx/sstream.h>
#include <wx/checkbox.h>
#include <cstdlib>

#define EXPORT __declspec(dllexport)

// Plugin state
struct State {
    bool enabled;
    wxCheckBox* checkbox;
};

static State g_state = {
    true,
    nullptr
};

// Plugin initialization
extern "C" EXPORT void initPlugin(void* registerShapeFunc, void* registerDrawFunc) {}

// Updates global state
void updateState() {
    g_state.enabled = g_state.checkbox->IsChecked();
}

// Checkbox handler
static void onCheckboxToggled(wxCommandEvent& event) {
    updateState();
}

// Adding elements to UI
extern "C" EXPORT void initUI(void* parentWindow, void* canvas) {
    if (!parentWindow) return;

    wxWindow* parent = (wxWindow*)parentWindow;

    g_state.checkbox = new wxCheckBox(parent, wxID_ANY, "XML <-> JSON");
    g_state.checkbox->SetValue(true);
    updateState();

    wxSizer* sizer = parent->GetSizer();
    if (sizer) {
        sizer->Add(g_state.checkbox, 0, wxALL, 5);
        sizer->Layout();
    }
    else {
        return;
    }

    g_state.checkbox->Bind(wxEVT_CHECKBOX, onCheckboxToggled);
    parent->Layout();
}


// Transforms XML to JSON (called when canvas is being saved)
extern "C" EXPORT const char *handle_save(const wxString* xmlContent) {
    if (!g_state.enabled) {
        std::string str = xmlContent->ToStdString();
        char* buf = (char*)std::malloc(str.length() + 1);
        if (!buf) return NULL;
        std::memcpy((void*)buf, str.c_str(), str.length());
        buf[str.length()] = '\0';

        return buf;
    }

    if (xmlContent->IsEmpty()) return "{}";

    wxStringInputStream stream(*xmlContent);

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

    // Allocating memory
    std::string str = json.str();
    char* buf = (char *)std::malloc(str.length() + 1);
    if (!buf) return NULL;
    std::memcpy((void*)buf, str.c_str(), str.length());
    buf[str.length()] = '\0';

    return buf;
}

// Transforms JSON to XML (called when canvas is being loaded)
extern "C" EXPORT const char *handle_load(const wxString* json_str) {
    if (!g_state.enabled) {
        std::string str = json_str->ToStdString();
        char* buf = (char*)std::malloc(str.length() + 1);
        if (!buf) return NULL;
        std::memcpy((void*)buf, str.c_str(), str.length());
        buf[str.length()] = '\0';

        return buf;
    }

    std::string json = json_str->ToStdString();
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Canvas>\n";

    size_t pos = 0;
    while (true) {
        size_t figurePos = json.find("\"Figure\"", pos);
        if (figurePos == std::string::npos) break;

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

        // Extracting Type
        size_t typePos = figure.find("\"Type\"");
        if (typePos == std::string::npos) { pos = closeBrace; continue; }
        size_t typeColon = figure.find(":", typePos);
        if (typeColon == std::string::npos) { pos = closeBrace; continue; }
        size_t typeQuote1 = figure.find("\"", typeColon + 1);
        if (typeQuote1 == std::string::npos) { pos = closeBrace; continue; }
        size_t typeQuote2 = figure.find("\"", typeQuote1 + 1);
        if (typeQuote2 == std::string::npos) { pos = closeBrace; continue; }
        std::string type = figure.substr(typeQuote1 + 1, typeQuote2 - typeQuote1 - 1);

        // Extracting Start
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

        // Extracting End
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

        // Forming XML
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

    // Allocating memory
    char* buf = (char*)std::malloc(xml.length() + 1);
    if (!buf) return NULL;
    std::memcpy((void*)buf, xml.c_str(), xml.length());
    buf[xml.length()] = '\0';

    return buf;
}

// Get plugin name
extern "C" EXPORT const char* getPluginName() {
    return "JSON support plugin";
}