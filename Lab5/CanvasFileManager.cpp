#include "CanvasFileManager.h"
#include "EditorCanvas.h"
#include "ShapeFactory.h"
#include "XMLSerializer.h"
#include <wx/xml/xml.h>
#include <wx/textfile.h>
#include <sstream>
#include <fstream>

// Constructor
CanvasFileManager::CanvasFileManager() {}

// Save canvas to file
bool CanvasFileManager::saveToFile(EditorCanvas* canvas, const wxString& filename) {
    if (!canvas) return false;

    // Getting XML String
    wxString xmlContent = XMLSerializer::serialize(canvas->getFigures());
    if (xmlContent.IsEmpty()) return false;

    // Passing the XML if needed
    wxString content;
    if (saveHandler != nullptr) {
        content = saveHandler(xmlContent);
    }
    else {
        content = xmlContent;
    }


    std::ofstream file(filename.ToStdString());
    if (!file.is_open()) return false;

    file << content.ToStdString();
    file.close();

    return true;
}

// Load canvas from file
bool CanvasFileManager::loadFromFile(EditorCanvas* canvas, const wxString& filename) {
    if (!canvas) return false;

    std::ifstream file(filename.ToStdString());
    if (!file.is_open()) return false;

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    // Passing content to processing if needed
    wxString content = wxString(ss.str());
    wxString xmlContent;
    if (loadHandler != nullptr) {
        xmlContent = loadHandler(content);
        wxMessageBox(xmlContent);
    }
    else {
        xmlContent = content;
    }

    std::vector<std::shared_ptr<fig::Figure>> figures;
    if (XMLSerializer::deserialize(xmlContent, figures)) {
        canvas->clearAll();
        std::vector<wxString> unknownShapes;
        for (auto& figure : figures) {
        //    auto it = std::find(ShapeFactory::getAvailableTypes().begin(), ShapeFactory::getAvailableTypes().end(), figure.get()->getTypeName());
        //    if (it == ShapeFactory::getAvailableTypes().end()) {
        //        unknownShapes.push_back(figure.get()->getTypeName());
        //        wxMessageBox("Shape '" + figure.get()->getTypeName() + "' is not registered and will not be displayed", "Warning", wxOK | wxICON_WARNING);
        //    }
            canvas->addFigure(figure);
        }
        canvas->Refresh();
        return true;
    }
    return false;
}