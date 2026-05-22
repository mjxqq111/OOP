#include "AerialVehicle.h"
#include "tinyxml2.h"

using namespace tinyxml2;

// Constructs an aerial vehicle with default zero values
AerialVehicle::AerialVehicle()
    : Vehicle(), maxAltitude(0) {
}

// Constructs an aerial vehicle with specified parameters including maximum altitude
AerialVehicle::AerialVehicle(double maxSpeed, double length, double width, double height,
    double mass, int capacity, double maxAltitude)
    : Vehicle(maxSpeed, length, width, height, mass, capacity), maxAltitude(maxAltitude) {
}

// Returns the maximum operating altitude in meters
double AerialVehicle::getMaxAltitude() const { return maxAltitude; }

// Sets the maximum operating altitude in meters
void AerialVehicle::setMaxAltitude(double altitude) { maxAltitude = altitude; }

// Loads aerial vehicle data from an XML element
void AerialVehicle::loadFromXML(const XMLElement* element) {
    loadCommonProperties(element);
    loadAerialProperties(element);
}

// Helper method to load max altitude from XML
void AerialVehicle::loadAerialProperties(const XMLElement* element) {
    if (!element) return;

    const XMLElement* field = element->FirstChildElement("maxAltitude");
    if (field && field->GetText()) {
        maxAltitude = std::stod(field->GetText());
    }
}

// Returns a list of all editable properties including max altitude
std::vector<Property> AerialVehicle::getEditableProperties() const {
    std::vector<Property> props = Vehicle::getEditableProperties();
    props.push_back({ "Max Altitude (m)", std::to_string(maxAltitude), "double" });
    return props;
}

// Updates a property value by its name including max altitude
void AerialVehicle::setProperty(const std::string& name, const std::string& value) {
    if (name == "Max Altitude (m)") {
        maxAltitude = std::stod(value);
    }
    else {
        Vehicle::setProperty(name, value);
    }
}