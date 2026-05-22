#pragma once

#include "WheeledVehicle.h"

// Represents a bicycle with 2 wheels and capacity for 1 person
class Bicycle : public WheeledVehicle {
public:
    // Constructs a bicycle with default values
    Bicycle();

    // Constructs a bicycle with specified maximum speed and mass
    Bicycle(double maxSpeed, double mass);

    // Virtual destructor
    virtual ~Bicycle() = default;

    // Writes the bicycle data to an output stream in XML format
    void serializeToXML(std::ostream& out, int indentLevel = 0) const override;

    // Loads bicycle data from an XML element
    void loadFromXML(const tinyxml2::XMLElement* element) override;

    // Creates a deep copy of the bicycle as a unique pointer
    std::unique_ptr<Vehicle> clone() const override;

    // Returns the string name of the vehicle type
    std::string getTypeName() const override;

    // Returns a list of all editable properties for this vehicle
    std::vector<Property> getEditableProperties() const override;

    // Updates a property value by its name
    void setProperty(const std::string& name, const std::string& value) override;
};