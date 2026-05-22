#include "WheeledVehicle.h"
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs a wheeled vehicle with default zero values
WheeledVehicle::WheeledVehicle()
    : Vehicle(), wheelCount(0) {
}

// Constructs a wheeled vehicle with specified parameters including wheel count
WheeledVehicle::WheeledVehicle(double maxSpeed, double length, double width, double height,
    double mass, int capacity, int wheelCount)
    : Vehicle(maxSpeed, length, width, height, mass, capacity), wheelCount(wheelCount) {
}

// Returns the number of wheels on the vehicle
int WheeledVehicle::getWheelCount() const { return wheelCount; }

// Sets the number of wheels on the vehicle
void WheeledVehicle::setWheelCount(int count) { wheelCount = count; }

// Loads wheeled vehicle data from an XML element
void WheeledVehicle::loadFromXML(const XMLElement* element) {
    loadCommonProperties(element);
    loadWheeledProperties(element);
}

// Helper method to load wheel count from XML
void WheeledVehicle::loadWheeledProperties(const XMLElement* element) {
    if (!element) return;

    const XMLElement* field = element->FirstChildElement("wheelCount");
    if (field && field->GetText()) {
        wheelCount = std::stoi(field->GetText());
    }
}

// Returns a list of all editable properties including wheel count
std::vector<Property> WheeledVehicle::getEditableProperties() const {
    std::vector<Property> props = Vehicle::getEditableProperties();
    props.push_back({ "Wheel Count", std::to_string(wheelCount), "int" });
    return props;
}

// Updates a property value by its name including wheel count
void WheeledVehicle::setProperty(const std::string& name, const std::string& value) {
    if (name == "Wheel Count") {
        wheelCount = std::stoi(value);
    }
    else {
        Vehicle::setProperty(name, value);
    }
}