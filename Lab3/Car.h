#pragma once

#include "WheeledVehicle.h"

// Represents a passenger car with 4 wheels and capacity for 4 persons
class Car : public WheeledVehicle {
public:
    // Constructs a car with default values
    Car();

    // Constructs a car with specified physical parameters
    Car(double maxSpeed, double length, double width, double height, double mass);

    // Virtual destructor
    virtual ~Car() = default;

    // Writes the car data to an output stream in XML format
    void serializeToXML(std::ostream& out, int indentLevel = 0) const override;

    // Loads car data from an XML element
    void loadFromXML(const tinyxml2::XMLElement* element) override;

    // Creates a deep copy of the car as a unique pointer
    std::unique_ptr<Vehicle> clone() const override;

    // Returns the string name of the vehicle type
    std::string getTypeName() const override;

    // Returns a list of all editable properties for this vehicle
    std::vector<Property> getEditableProperties() const override;

    // Updates a property value by its name
    void setProperty(const std::string& name, const std::string& value) override;
};