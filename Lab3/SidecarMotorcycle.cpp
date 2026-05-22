#include "SidecarMotorcycle.h"
#include "RegistrationHelper.h"
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs a sidecar motorcycle with default values
SidecarMotorcycle::SidecarMotorcycle()
    : Motorcycle(150, 280), cargoCapacity(100) {
}

// Constructs a sidecar motorcycle with specified parameters and cargo capacity
SidecarMotorcycle::SidecarMotorcycle(double maxSpeed, double mass, double cargoCapacity)
    : Motorcycle(maxSpeed, mass), cargoCapacity(cargoCapacity) {
}

// Returns the cargo capacity of the sidecar in kilograms
double SidecarMotorcycle::getCargoCapacity() const { return cargoCapacity; }

// Sets the cargo capacity of the sidecar in kilograms
void SidecarMotorcycle::setCargoCapacity(double cap) { cargoCapacity = cap; }

// Writes the sidecar motorcycle data to an output stream in XML format
void SidecarMotorcycle::serializeToXML(std::ostream& out, int indentLevel) const {
    std::string indent(indentLevel * 2, ' ');
    out << indent << "<SidecarMotorcycle>\n";
    out << indent << "  <maxSpeed>" << std::to_string(maxSpeed) << "</maxSpeed>\n";
    out << indent << "  <length>" << std::to_string(length) << "</length>\n";
    out << indent << "  <width>" << std::to_string(width) << "</width>\n";
    out << indent << "  <height>" << std::to_string(height) << "</height>\n";
    out << indent << "  <mass>" << std::to_string(mass) << "</mass>\n";
    out << indent << "  <capacity>" << std::to_string(capacity) << "</capacity>\n";
    out << indent << "  <wheelCount>" << std::to_string(wheelCount) << "</wheelCount>\n";
    out << indent << "  <cargoCapacity>" << std::to_string(cargoCapacity) << "</cargoCapacity>\n";
    out << indent << "</SidecarMotorcycle>\n";
}

// Loads sidecar motorcycle data from an XML element
void SidecarMotorcycle::loadFromXML(const XMLElement* element) {
    loadCommonProperties(element);
    loadWheeledProperties(element);
    loadSidecarProperties(element);
}

// Helper method to load cargo capacity from XML
void SidecarMotorcycle::loadSidecarProperties(const XMLElement* element) {
    if (!element) return;

    const XMLElement* field = element->FirstChildElement("cargoCapacity");
    if (field && field->GetText()) {
        cargoCapacity = std::stod(field->GetText());
    }
}

// Creates a deep copy of the sidecar motorcycle as a unique pointer
std::unique_ptr<Vehicle> SidecarMotorcycle::clone() const {
    return std::make_unique<SidecarMotorcycle>(*this);
}

// Returns the string name of the vehicle type
std::string SidecarMotorcycle::getTypeName() const {
    return "SidecarMotorcycle";
}

// Returns a list of all editable properties for this vehicle
std::vector<Property> SidecarMotorcycle::getEditableProperties() const {
    // Get properties from parent class (Motorcycle)
    std::vector<Property> props = Motorcycle::getEditableProperties();

    // Remove Wheel Count if present
    for (auto it = props.begin(); it != props.end(); ++it) {
        if (it->name == "Wheel Count") {
            props.erase(it);
            break;
        }
    }

    // Add Cargo Capacity property
    props.push_back({ "Cargo Capacity (kg)", std::to_string(cargoCapacity), "double" });

    return props;
}

// Updates a property value by its name
void SidecarMotorcycle::setProperty(const std::string& name, const std::string& value) {
    if (name == "Cargo Capacity (kg)") {
        cargoCapacity = std::stod(value);
    }
    else if (name == "Max Speed (km/h)") {
        maxSpeed = std::stod(value);
    }
    else if (name == "Length (m)") {
        length = std::stod(value);
    }
    else if (name == "Width (m)") {
        width = std::stod(value);
    }
    else if (name == "Height (m)") {
        height = std::stod(value);
    }
    else if (name == "Mass (kg)") {
        mass = std::stod(value);
    }
    // Capacity and Wheel Count are not editable
}

// Automatic registration with the factory
REGISTER_VEHICLE(SidecarMotorcycle)