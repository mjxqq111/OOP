#pragma once

#include <vector>
#include <memory>
#include "Vehicle.h"

// Manages a collection of vehicles with add, remove, edit and persistence operations
class VehicleManager {
public:
    // Adds a new vehicle to the collection
    void addVehicle(std::unique_ptr<Vehicle> vehicle);

    // Removes a vehicle at the specified index from the collection
    void removeVehicle(int index);

    // Replaces a vehicle at the specified index with a new vehicle
    void editVehicle(int index, std::unique_ptr<Vehicle> newData);

    // Saves the current vehicle collection to an XML file
    void saveToFile(const std::string& filename);

    // Loads a vehicle collection from an XML file
    void loadFromFile(const std::string& filename);

    // Returns the number of vehicles in the collection
    int getCount() const;

    // Returns a copy of the vehicle at the specified index
    std::unique_ptr<Vehicle> getVehicleCopy(int index) const;

    // Returns a const pointer to the vehicle at the specified index
    const Vehicle* getVehicleConst(int index) const;

private:
    std::vector<std::unique_ptr<Vehicle>> vehicles;  // Collection of vehicles
};