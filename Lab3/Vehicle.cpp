#include "Vehicle.h"
#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs a vehicle with default zero values
Vehicle::Vehicle()
    : maxSpeed(0), length(0), width(0), height(0), mass(0), capacity(0) {
}

// Constructs a vehicle with specified physical parameters
Vehicle::Vehicle(double maxSpeed, double length, double width, double height,
    double mass, int capacity)
    : maxSpeed(maxSpeed), length(length), width(width), height(height),
    mass(mass), capacity(capacity) {
}

// Returns the maximum speed in kilometers per hour
double Vehicle::getMaxSpeed() const { return maxSpeed; }

// Returns the vehicle length in meters
double Vehicle::getLength() const { return length; }

// Returns the vehicle width in meters
double Vehicle::getWidth() const { return width; }

// Returns the vehicle height in meters
double Vehicle::getHeight() const { return height; }

// Returns the vehicle mass in kilograms
double Vehicle::getMass() const { return mass; }

// Returns the passenger capacity including the driver
int Vehicle::getCapacity() const { return capacity; }

// Sets the maximum speed in kilometers per hour
void Vehicle::setMaxSpeed(double speed) { maxSpeed = speed; }

// Sets the vehicle length in meters
void Vehicle::setLength(double len) { length = len; }

// Sets the vehicle width in meters
void Vehicle::setWidth(double wid) { width = wid; }

// Sets the vehicle height in meters
void Vehicle::setHeight(double hei) { height = hei; }

// Sets the vehicle mass in kilograms
void Vehicle::setMass(double m) { mass = m; }

// Sets the passenger capacity including the driver
void Vehicle::setCapacity(int cap) { capacity = cap; }

// Helper method to load common properties from XML
void Vehicle::loadCommonProperties(const XMLElement* element) {
    if (!element) return;

    const XMLElement* field = element->FirstChildElement("maxSpeed");
    if (field && field->GetText()) {
        maxSpeed = std::stod(field->GetText());
    }

    field = element->FirstChildElement("length");
    if (field && field->GetText()) {
        length = std::stod(field->GetText());
    }

    field = element->FirstChildElement("width");
    if (field && field->GetText()) {
        width = std::stod(field->GetText());
    }

    field = element->FirstChildElement("height");
    if (field && field->GetText()) {
        height = std::stod(field->GetText());
    }

    field = element->FirstChildElement("mass");
    if (field && field->GetText()) {
        mass = std::stod(field->GetText());
    }

    field = element->FirstChildElement("capacity");
    if (field && field->GetText()) {
        capacity = std::stoi(field->GetText());
    }
}

// Returns a list of all editable properties for this vehicle
std::vector<Property> Vehicle::getEditableProperties() const {
    std::vector<Property> props;
    props.push_back({ "Max Speed (km/h)", std::to_string(maxSpeed), "double" });
    props.push_back({ "Length (m)", std::to_string(length), "double" });
    props.push_back({ "Width (m)", std::to_string(width), "double" });
    props.push_back({ "Height (m)", std::to_string(height), "double" });
    props.push_back({ "Mass (kg)", std::to_string(mass), "double" });
    props.push_back({ "Capacity", std::to_string(capacity), "int" });
    return props;
}

// Updates a property value by its name
void Vehicle::setProperty(const std::string& name, const std::string& value) {
    if (name == "Max Speed (km/h)") maxSpeed = std::stod(value);
    else if (name == "Length (m)") length = std::stod(value);
    else if (name == "Width (m)") width = std::stod(value);
    else if (name == "Height (m)") height = std::stod(value);
    else if (name == "Mass (kg)") mass = std::stod(value);
    else if (name == "Capacity") capacity = std::stoi(value);
}

// Returns the static registry map that stores type creators
std::map<std::string, Vehicle::Creator>& Vehicle::getRegistry() {
    static std::map<std::string, Creator> registry;
    return registry;
}

// Registers a vehicle type with the factory using a creator function
void Vehicle::registerType(const std::string& typeName, Creator creator) {
    getRegistry()[typeName] = creator;
}

// Creates a vehicle instance of the specified type using the factory
std::unique_ptr<Vehicle> Vehicle::create(const std::string& typeName) {
    auto& registry = getRegistry();
    auto it = registry.find(typeName);
    if (it != registry.end()) {
        return it->second();
    }
    return nullptr;
}

// Returns a list of all registered vehicle type names
std::vector<std::string> Vehicle::getRegisteredTypes() {
    std::vector<std::string> types;
    for (auto& pair : getRegistry()) {
        types.push_back(pair.first);
    }
    return types;
}