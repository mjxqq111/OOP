#pragma once

#include "WheeledVehicle.h"

// Represents a motorcycle with 2 wheels and capacity for 1 person
class Motorcycle : public WheeledVehicle {
public:
    // Constructs a motorcycle with default values
    Motorcycle();

    // Constructs a motorcycle with specified maximum speed and mass
    Motorcycle(double maxSpeed, double mass);

    // Virtual destructor
    virtual ~Motorcycle() = default;

    // Writes the motorcycle data to an output stream in XML format
    void serializeToXML(std::ostream& out, int indentLevel = 0) const override;

    // Loads motorcycle data from an XML element
    void loadFromXML(const tinyxml2::XMLElement* element) override;

    // Creates a deep copy of the motorcycle as a unique pointer
    std::unique_ptr<Vehicle> clone() const override;

    // Returns the string name of the vehicle type
    std::string getTypeName() const override;

    // Returns a list of all editable properties for this vehicle
    std::vector<Property> getEditableProperties() const override;

    // Updates a property value by its name
    void setProperty(const std::string& name, const std::string& value) override;
};