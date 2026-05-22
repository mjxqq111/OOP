#pragma once

#include "AerialVehicle.h"

// Represents a helicopter as a rotary wing aerial vehicle
class Helicopter : public AerialVehicle {
public:
    // Constructs a helicopter with default values
    Helicopter();

    // Constructs a helicopter with specified parameters
    Helicopter(double maxSpeed, double length, double width, double height,
        double mass, int capacity, double maxAltitude);

    // Virtual destructor
    virtual ~Helicopter() = default;

    // Writes the helicopter data to an output stream in XML format
    void serializeToXML(std::ostream& out, int indentLevel = 0) const override;

    // Loads helicopter data from an XML element
    void loadFromXML(const tinyxml2::XMLElement* element) override;

    // Creates a deep copy of the helicopter as a unique pointer
    std::unique_ptr<Vehicle> clone() const override;

    // Returns the string name of the vehicle type
    std::string getTypeName() const override;

    // Returns a list of all editable properties for this vehicle
    std::vector<Property> getEditableProperties() const override;

    // Updates a property value by its name
    void setProperty(const std::string& name, const std::string& value) override;
};