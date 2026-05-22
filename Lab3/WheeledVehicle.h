#pragma once

#include "Vehicle.h"

// Abstract class for wheeled ground vehicles
class WheeledVehicle : public Vehicle {
public:
    // Constructs a wheeled vehicle with default zero values
    WheeledVehicle();

    // Constructs a wheeled vehicle with specified parameters including wheel count
    WheeledVehicle(double maxSpeed, double length, double width, double height,
        double mass, int capacity, int wheelCount);

    // Virtual destructor for proper cleanup of derived classes
    virtual ~WheeledVehicle() = default;

    // Returns the number of wheels on the vehicle
    int getWheelCount() const;

    // Sets the number of wheels on the vehicle
    void setWheelCount(int count);

    // Loads wheeled vehicle data from an XML element
    void loadFromXML(const tinyxml2::XMLElement* element) override;

    // Returns a list of all editable properties including wheel count
    std::vector<Property> getEditableProperties() const override;

    // Updates a property value by its name including wheel count
    void setProperty(const std::string& name, const std::string& value) override;

protected:
    int wheelCount;  // Number of wheels

    // Helper method to load wheel count from XML
    void loadWheeledProperties(const tinyxml2::XMLElement* element);
};