#include "Car.h"
#include "RegistrationHelper.h"
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs a car with default values
Car::Car()
    : WheeledVehicle(180, 4.5, 1.8, 1.5, 1200, 4, 4) {
}

// Constructs a car with specified physical parameters
Car::Car(double maxSpeed, double length, double width, double height, double mass)
    : WheeledVehicle(maxSpeed, length, width, height, mass, 4, 4) {
}

// Writes the car data to an output stream in XML format
void Car::serializeToXML(std::ostream& out, int indentLevel) const {
    std::string indent(indentLevel * 2, ' ');
    out << indent << "<Car>\n";
    out << indent << "  <maxSpeed>" << std::to_string(maxSpeed) << "</maxSpeed>\n";
    out << indent << "  <length>" << std::to_string(length) << "</length>\n";
    out << indent << "  <width>" << std::to_string(width) << "</width>\n";
    out << indent << "  <height>" << std::to_string(height) << "</height>\n";
    out << indent << "  <mass>" << std::to_string(mass) << "</mass>\n";
    out << indent << "  <capacity>" << std::to_string(capacity) << "</capacity>\n";
    out << indent << "  <wheelCount>" << std::to_string(wheelCount) << "</wheelCount>\n";
    out << indent << "</Car>\n";
}

// Loads car data from an XML element
void Car::loadFromXML(const XMLElement* element) {
    loadCommonProperties(element);
    loadWheeledProperties(element);
}

// Creates a deep copy of the car as a unique pointer
std::unique_ptr<Vehicle> Car::clone() const {
    return std::make_unique<Car>(*this);
}

// Returns the string name of the vehicle type
std::string Car::getTypeName() const {
    return "Car";
}

// Returns a list of all editable properties for this vehicle
// Capacity is not editable because car always has fixed capacity of 4 persons
// Wheel Count is not editable because car always has 4 wheels
std::vector<Property> Car::getEditableProperties() const {
    std::vector<Property> props;
    props.push_back({ "Max Speed (km/h)", std::to_string(maxSpeed), "double" });
    props.push_back({ "Length (m)", std::to_string(length), "double" });
    props.push_back({ "Width (m)", std::to_string(width), "double" });
    props.push_back({ "Height (m)", std::to_string(height), "double" });
    props.push_back({ "Mass (kg)", std::to_string(mass), "double" });
    // Capacity is omitted (always 4)
    // Wheel Count is omitted (always 4)
    return props;
}

// Updates a property value by its name
void Car::setProperty(const std::string& name, const std::string& value) {
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
    // Capacity is not included because it cannot be changed
    // Wheel Count is not included because it cannot be changed
}

// Automatic registration with the factory
REGISTER_VEHICLE(Car)