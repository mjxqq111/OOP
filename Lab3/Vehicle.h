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

    // Getters
    double getMaxSpeed() const;
    double getLength() const;
    double getWidth() const;
    double getHeight() const;
    double getMass() const;
    int getCapacity() const;

    // Setters
    void setMaxSpeed(double speed);
    void setLength(double len);
    void setWidth(double wid);
    void setHeight(double hei);
    void setMass(double m);
    void setCapacity(int cap);

    // Serialization interface
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

    // Factory registration
    using Creator = std::function<std::unique_ptr<Vehicle>()>;
    static void registerType(const std::string& typeName, Creator creator);
    static std::unique_ptr<Vehicle> create(const std::string& typeName);
    static std::vector<std::string> getRegisteredTypes();

protected:
    double maxSpeed;
    double length;
    double width;
    double height;
    double mass;
    int capacity;

    // Helper method to load common properties from XML
    void loadCommonProperties(const tinyxml2::XMLElement* element);

private:
    // Returns the static registry map that stores type creators
    static std::map<std::string, Creator>& getRegistry();
};