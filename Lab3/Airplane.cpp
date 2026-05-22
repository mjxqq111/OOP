#include "Airplane.h"
#include "RegistrationHelper.h"
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs an airplane with default values
Airplane::Airplane()
    : AerialVehicle(900, 70, 60, 18, 500000, 200, 12000) {
}

// Constructs an airplane with specified parameters
Airplane::Airplane(double maxSpeed, double length, double width, double height,
    double mass, int capacity, double maxAltitude)
    : AerialVehicle(maxSpeed, length, width, height, mass, capacity, maxAltitude) {
}

// Writes the airplane data to an output stream in XML format
void Airplane::serializeToXML(std::ostream& out, int indentLevel) const {
    std::string indent(indentLevel * 2, ' ');
    out << indent << "<Airplane>\n";
    out << indent << "  <maxSpeed>" << std::to_string(maxSpeed) << "</maxSpeed>\n";
    out << indent << "  <length>" << std::to_string(length) << "</length>\n";
    out << indent << "  <width>" << std::to_string(width) << "</width>\n";
    out << indent << "  <height>" << std::to_string(height) << "</height>\n";
    out << indent << "  <mass>" << std::to_string(mass) << "</mass>\n";
    out << indent << "  <capacity>" << std::to_string(capacity) << "</capacity>\n";
    out << indent << "  <maxAltitude>" << std::to_string(maxAltitude) << "</maxAltitude>\n";
    out << indent << "</Airplane>\n";
}

// Loads airplane data from an XML element
void Airplane::loadFromXML(const XMLElement* element) {
    loadCommonProperties(element);
    loadAerialProperties(element);
}

// Creates a deep copy of the airplane as a unique pointer
std::unique_ptr<Vehicle> Airplane::clone() const {
    return std::make_unique<Airplane>(*this);
}

// Returns the string name of the vehicle type
std::string Airplane::getTypeName() const {
    return "Airplane";
}

// Returns a list of all editable properties for this vehicle
std::vector<Property> Airplane::getEditableProperties() const {
    std::vector<Property> props;
    props.push_back({ "Max Speed (km/h)", std::to_string(maxSpeed), "double" });
    props.push_back({ "Length (m)", std::to_string(length), "double" });
    props.push_back({ "Width (m)", std::to_string(width), "double" });
    props.push_back({ "Height (m)", std::to_string(height), "double" });
    props.push_back({ "Mass (kg)", std::to_string(mass), "double" });
    props.push_back({ "Capacity", std::to_string(capacity), "int" });
    props.push_back({ "Max Altitude (m)", std::to_string(maxAltitude), "double" });
    return props;
}

// Updates a property value by its name
void Airplane::setProperty(const std::string& name, const std::string& value) {
    if (name == "Max Speed (km/h)") maxSpeed = std::stod(value);
    else if (name == "Length (m)") length = std::stod(value);
    else if (name == "Width (m)") width = std::stod(value);
    else if (name == "Height (m)") height = std::stod(value);
    else if (name == "Mass (kg)") mass = std::stod(value);
    else if (name == "Capacity") capacity = std::stoi(value);
    else if (name == "Max Altitude (m)") maxAltitude = std::stod(value);
}

// Automatic registration with the factory
REGISTER_VEHICLE(Airplane)