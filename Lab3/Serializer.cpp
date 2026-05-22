#include "Serializer.h"
#include <fstream>
#include "tinyxml2.h"
#include <wx/msgdlg.h>

using namespace tinyxml2;

// Writes a collection of vehicles to an XML file
void Serializer::serializeToFile(const std::vector<std::unique_ptr<Vehicle>>& vehicles,
    const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        wxMessageBox("Cannot open file for writing: " + filename,
            "Save Error",
            wxOK | wxICON_ERROR);
        return;
    }

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<VehicleList>\n";

    for (const auto& v : vehicles) {
        v->serializeToXML(file, 1);
    }

    file << "</VehicleList>\n";
    file.close();
}

// Reads a collection of vehicles from an XML file
std::vector<std::unique_ptr<Vehicle>> Serializer::deserializeFromFile(const std::string& filename) {
    std::vector<std::unique_ptr<Vehicle>> result;

    // Create an XML document object
    tinyxml2::XMLDocument doc;

    // Load the XML file
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        wxMessageBox("Failed to load XML file: " + filename,
            "Load Error",
            wxOK | wxICON_ERROR);
        return result;
    }

    // Get the root element
    XMLElement* root = doc.FirstChildElement("VehicleList");
    if (!root) {
        wxMessageBox("Invalid XML format: missing VehicleList root element",
            "Parse Error",
            wxOK | wxICON_ERROR);
        return result;
    }

    // Iterate through each vehicle element
    for (XMLElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        std::string typeName = elem->Name();

        // Create a vehicle of the appropriate type using the factory
        auto vehicle = Vehicle::create(typeName);
        if (!vehicle) {
            wxMessageBox("Unknown vehicle type: " + typeName,
                "Parse Error",
                wxOK | wxICON_ERROR);
            continue;
        }

        // Load vehicle data from XML using virtual method
        vehicle->loadFromXML(elem);

        result.push_back(std::move(vehicle));
    }

    wxMessageBox("Loaded " + std::to_string(result.size()) + " vehicles from " + filename,
        "Load Success",
        wxOK | wxICON_INFORMATION);

    return result;
}