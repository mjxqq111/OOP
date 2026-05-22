#pragma once

#include "Motorcycle.h"

// Represents a motorcycle with a sidecar that adds cargo capacity
class SidecarMotorcycle : public Motorcycle {
public:
    // Constructs a sidecar motorcycle with default values
    SidecarMotorcycle();

    // Constructs a sidecar motorcycle with specified parameters and cargo capacity
    SidecarMotorcycle(double maxSpeed, double mass, double cargoCapacity);

    // Virtual destructor
    virtual ~SidecarMotorcycle() = default;

    // Returns the cargo capacity of the sidecar in kilograms
    double getCargoCapacity() const;

    // Sets the cargo capacity of the sidecar in kilograms
    void setCargoCapacity(double cap);

    // Writes the sidecar motorcycle data to an output stream in XML format
    void serializeToXML(std::ostream& out, int indentLevel = 0) const override;

    // Loads sidecar motorcycle data from an XML element
    void loadFromXML(const tinyxml2::XMLElement* element) override;

    // Creates a deep copy of the sidecar motorcycle as a unique pointer
    std::unique_ptr<Vehicle> clone() const override;

    // Returns the string name of the vehicle type
    std::string getTypeName() const override;

    // Returns a list of all editable properties for this vehicle
    std::vector<Property> getEditableProperties() const override;

    // Updates a property value by its name
    void setProperty(const std::string& name, const std::string& value) override;

private:
    double cargoCapacity; // Cargo capacity in kilograms

    // Helper method to load cargo capacity from XML
    void loadSidecarProperties(const tinyxml2::XMLElement* element);
};