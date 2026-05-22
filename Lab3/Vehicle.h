#pragma once

#include <string>
#include <memory>
#include <map>
#include <functional>
#include <ostream>
#include <vector>

namespace tinyxml2 {
    class XMLElement;
}

// Structure representing an editable property with name, value and type
struct Property {
    std::string name;
    std::string value;
    std::string type;  // "double", "int"
};

// Abstract base class for all vehicles
class Vehicle {
public:
    // Constructs a vehicle with default zero values
    Vehicle();

    // Constructs a vehicle with specified physical parameters
    Vehicle(double maxSpeed, double length, double width, double height,
        double mass, int capacity);

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Vehicle() = default;

    double getMaxSpeed() const;                // Returns the maximum speed in kilometers per hour
    double getLength() const;                  // Returns the vehicle length in meters
    double getWidth() const;                   // Returns the vehicle width in meters
    double getHeight() const;                  // Returns the vehicle height in meters
    double getMass() const;                    // Returns the vehicle mass in kilograms
    int getCapacity() const;                   // Returns the passenger capacity including the driver

    void setMaxSpeed(double speed);            // Sets the maximum speed in kilometers per hour
    void setLength(double len);                // Sets the vehicle length in meters
    void setWidth(double wid);                 // Sets the vehicle width in meters
    void setHeight(double hei);                // Sets the vehicle height in meters
    void setMass(double m);                    // Sets the vehicle mass in kilograms
    void setCapacity(int cap);                 // Sets the passenger capacity including the driver

    // Writes the vehicle data to an output stream in XML format
    virtual void serializeToXML(std::ostream& out, int indentLevel = 0) const = 0;

    // Loads vehicle data from an XML element
    virtual void loadFromXML(const tinyxml2::XMLElement* element) = 0;

    // Creates a deep copy of the vehicle as a unique pointer
    virtual std::unique_ptr<Vehicle> clone() const = 0;

    // Returns the string name of the vehicle type
    virtual std::string getTypeName() const = 0;

    // Returns a list of all editable properties for this vehicle
    virtual std::vector<Property> getEditableProperties() const;

    // Updates a property value by its name
    virtual void setProperty(const std::string& name, const std::string& value);

    // Factory registration function type
    using Creator = std::function<std::unique_ptr<Vehicle>()>;

    // Registers a vehicle type with the factory using a creator function
    static void registerType(const std::string& typeName, Creator creator);

    // Creates a vehicle instance of the specified type using the factory
    static std::unique_ptr<Vehicle> create(const std::string& typeName);

    // Returns a list of all registered vehicle type names
    static std::vector<std::string> getRegisteredTypes();

protected:
    double maxSpeed;    // Maximum speed in kilometers per hour
    double length;      // Length in meters
    double width;       // Width in meters
    double height;      // Height in meters
    double mass;        // Mass in kilograms
    int capacity;       // Number of persons including driver

    // Helper method to load common properties from XML
    void loadCommonProperties(const tinyxml2::XMLElement* element);

private:
    // Returns the static registry map that stores type creators
    static std::map<std::string, Creator>& getRegistry();
};