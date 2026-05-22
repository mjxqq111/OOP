#include "Bus.h"
#include "RegistrationHelper.h"
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs a bus with default values
Bus::Bus()
    : WheeledVehicle(120, 12, 2.5, 3.0, 15000, 50, 4) {
}

// Constructs a bus with specified physical parameters and capacity
Bus::Bus(double maxSpeed, double length, double width, double height,
    double mass, int capacity)
    : WheeledVehicle(maxSpeed, length, width, height, mass, capacity, 4) {
}

// Writes the bus data to an output stream in XML format
void Bus::serializeToXML(std::ostream& out, int indentLevel) const {
    std::string indent(indentLevel * 2, ' ');
    out << indent << "<Bus>\n";
    out << indent << "  <maxSpeed>" << std::to_string(maxSpeed) << "</maxSpeed>\n";
    out << indent << "  <length>" << std::to_string(length) << "</length>\n";
    out << indent << "  <width>" << std::to_string(width) << "</width>\n";
    out << indent << "  <height>" << std::to_string(height) << "</height>\n";
    out << indent << "  <mass>" << std::to_string(mass) << "</mass>\n";
    out << indent << "  <capacity>" << std::to_string(capacity) << "</capacity>\n";
    out << indent << "  <wheelCount>" << std::to_string(wheelCount) << "</wheelCount>\n";
    out << indent << "</Bus>\n";
}

// Loads bus data from an XML element
void Bus::loadFromXML(const XMLElement* element) {
    loadCommonProperties(element);
    loadWheeledProperties(element);
}

// Creates a deep copy of the bus as a unique pointer
std::unique_ptr<Vehicle> Bus::clone() const {
    return std::make_unique<Bus>(*this);
}

// Returns the string name of the vehicle type
std::string Bus::getTypeName() const {
    return "Bus";
}

// Returns a list of all editable properties for this vehicle
// Wheel Count is removed because bus always has 4 wheels
// Capacity is kept editable because buses can have different passenger capacities
std::vector<Property> Bus::getEditableProperties() const {
    std::vector<Property> props = WheeledVehicle::getEditableProperties();

    // Remove only Wheel Count from the list
    for (auto it = props.begin(); it != props.end(); ++it) {
        if (it->name == "Wheel Count") {
            props.erase(it);
            break;
        }
    }
    return props;
}

// Updates a property value by its name
void Bus::setProperty(const std::string& name, const std::string& value) {
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
    else if (name == "Capacity") {
        capacity = std::stoi(value);
    }
    // Wheel Count is not editable for bus
}

// Automatic registration with the factory
REGISTER_VEHICLE(Bus)