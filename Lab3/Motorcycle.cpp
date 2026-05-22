#include "Motorcycle.h"
#include "RegistrationHelper.h"
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs a motorcycle with default values
Motorcycle::Motorcycle()
    : WheeledVehicle(180, 2.1, 0.8, 1.1, 200, 1, 2) {
}

// Constructs a motorcycle with specified maximum speed and mass
Motorcycle::Motorcycle(double maxSpeed, double mass)
    : WheeledVehicle(maxSpeed, 2.1, 0.8, 1.1, mass, 1, 2) {
}

// Writes the motorcycle data to an output stream in XML format
void Motorcycle::serializeToXML(std::ostream& out, int indentLevel) const {
    std::string indent(indentLevel * 2, ' ');
    out << indent << "<Motorcycle>\n";
    out << indent << "  <maxSpeed>" << std::to_string(maxSpeed) << "</maxSpeed>\n";
    out << indent << "  <length>" << std::to_string(length) << "</length>\n";
    out << indent << "  <width>" << std::to_string(width) << "</width>\n";
    out << indent << "  <height>" << std::to_string(height) << "</height>\n";
    out << indent << "  <mass>" << std::to_string(mass) << "</mass>\n";
    out << indent << "  <capacity>" << std::to_string(capacity) << "</capacity>\n";
    out << indent << "  <wheelCount>" << std::to_string(wheelCount) << "</wheelCount>\n";
    out << indent << "</Motorcycle>\n";
}

// Loads motorcycle data from an XML element
void Motorcycle::loadFromXML(const XMLElement* element) {
    loadCommonProperties(element);
    loadWheeledProperties(element);
}

// Creates a deep copy of the motorcycle as a unique pointer
std::unique_ptr<Vehicle> Motorcycle::clone() const {
    return std::make_unique<Motorcycle>(*this);
}

// Returns the string name of the vehicle type
std::string Motorcycle::getTypeName() const {
    return "Motorcycle";
}

// Returns a list of all editable properties for this vehicle
// Capacity is not editable because motorcycle always carries only the driver (1 person)
// Wheel Count is not editable because motorcycle always has 2 wheels
std::vector<Property> Motorcycle::getEditableProperties() const {
    std::vector<Property> props;
    props.push_back({ "Max Speed (km/h)", std::to_string(maxSpeed), "double" });
    props.push_back({ "Length (m)", std::to_string(length), "double" });
    props.push_back({ "Width (m)", std::to_string(width), "double" });
    props.push_back({ "Height (m)", std::to_string(height), "double" });
    props.push_back({ "Mass (kg)", std::to_string(mass), "double" });
    // Capacity is omitted (always 1)
    // Wheel Count is omitted (always 2)
    return props;
}

// Updates a property value by its name
void Motorcycle::setProperty(const std::string& name, const std::string& value) {
    if (name == "Max Speed (km/h)") {
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
    // Capacity is not included because it cannot be changed (always 1)
    // Wheel Count is not included because it cannot be changed (always 2)
}

// Automatic registration with the factory
REGISTER_VEHICLE(Motorcycle)