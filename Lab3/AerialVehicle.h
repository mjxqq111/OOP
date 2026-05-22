#pragma once

#include "Vehicle.h"

// Abstract class for aerial vehicles
class AerialVehicle : public Vehicle {
public:
    // Constructs an aerial vehicle with default zero values
    AerialVehicle();

    // Constructs an aerial vehicle with specified parameters including maximum altitude
    AerialVehicle(double maxSpeed, double length, double width, double height,
        double mass, int capacity, double maxAltitude);

    // Virtual destructor for proper cleanup of derived classes
    virtual ~AerialVehicle() = default;

    // Returns the maximum operating altitude in meters
    double getMaxAltitude() const;

    // Sets the maximum operating altitude in meters
    void setMaxAltitude(double altitude);

    // Loads aerial vehicle data from an XML element
    void loadFromXML(const tinyxml2::XMLElement* element) override;

    // Returns a list of all editable properties including max altitude
    std::vector<Property> getEditableProperties() const override;

    // Updates a property value by its name including max altitude
    void setProperty(const std::string& name, const std::string& value) override;

protected:
    double maxAltitude; // Maximum operating altitude in meters

    // Helper method to load max altitude from XML
    void loadAerialProperties(const tinyxml2::XMLElement* element);
};