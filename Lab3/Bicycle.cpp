#include "Bicycle.h"
#include "RegistrationHelper.h"
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs a bicycle with default values
Bicycle::Bicycle()
    : WheeledVehicle(25, 1.7, 0.6, 1.0, 15, 1, 2) {
}

// Constructs a bicycle with specified maximum speed and mass
Bicycle::Bicycle(double maxSpeed, double mass)
    : WheeledVehicle(maxSpeed, 1.7, 0.6, 1.0, mass, 1, 2) {
}

// Writes the bicycle data to an output stream in XML format
void Bicycle::serializeToXML(std::ostream& out, int indentLevel) const {
    std::string indent(indentLevel * 2, ' ');
    out << indent << "<Bicycle>\n";
    out << indent << "  <maxSpeed>" << std::to_string(maxSpeed) << "</maxSpeed>\n";
    out << indent << "  <length>" << std::to_string(length) << "</length>\n";
    out << indent << "  <width>" << std::to_string(width) << "</width>\n";
    out << indent << "  <height>" << std::to_string(height) << "</height>\n";
    out << indent << "  <mass>" << std::to_string(mass) << "</mass>\n";
    out << indent << "  <capacity>" << std::to_string(capacity) << "</capacity>\n";
    out << indent << "  <wheelCount>" << std::to_string(wheelCount) << "</wheelCount>\n";
    out << indent << "</Bicycle>\n";
}

// Loads bicycle data from an XML element
void Bicycle::loadFromXML(const XMLElement* element) {
    loadCommonProperties(element);
    loadWheeledProperties(element);
}

// Creates a deep copy of the bicycle as a unique pointer
std::unique_ptr<Vehicle> Bicycle::clone() const {
    return std::make_unique<Bicycle>(*this);
}

// Returns the string name of the vehicle type
std::string Bicycle::getTypeName() const {
    return "Bicycle";
}

// Returns a list of all editable properties for this vehicle
// Capacity is not editable because bicycle always carries only the driver (1 person)
// Wheel Count is not editable because bicycle always has 2 wheels
std::vector<Property> Bicycle::getEditableProperties() const {
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
void Bicycle::setProperty(const std::string& name, const std::string& value) {
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
REGISTER_VEHICLE(Bicycle)