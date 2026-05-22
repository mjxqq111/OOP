#pragma once

#include "WheeledVehicle.h"

// Represents a bus with 4 wheels and high passenger capacity
class Bus : public WheeledVehicle {
public:
    // Constructs a bus with default values
    Bus();

    // Constructs a bus with specified physical parameters and capacity
    Bus(double maxSpeed, double length, double width, double height,
        double mass, int capacity);

    // Virtual destructor
    virtual ~Bus() = default;

    // Writes the bus data to an output stream in XML format
    void serializeToXML(std::ostream& out, int indentLevel = 0) const override;

    // Loads bus data from an XML element
    void loadFromXML(const tinyxml2::XMLElement* element) override;

    // Creates a deep copy of the bus as a unique pointer
    std::unique_ptr<Vehicle> clone() const override;

    // Returns the string name of the vehicle type
    std::string getTypeName() const override;

    // Returns a list of all editable properties for this vehicle
    std::vector<Property> getEditableProperties() const override;

    // Updates a property value by its name
    void setProperty(const std::string& name, const std::string& value) override;
};