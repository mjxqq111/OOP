#include "VehicleManager.h"
#include "Serializer.h"

// Adds a new vehicle to the collection
void VehicleManager::addVehicle(std::unique_ptr<Vehicle> vehicle) {
    vehicles.push_back(std::move(vehicle));
}

// Removes a vehicle at the specified index from the collection
void VehicleManager::removeVehicle(int index) {
    if (index >= 0 && index < static_cast<int>(vehicles.size())) {
        vehicles.erase(vehicles.begin() + index);
    }
}

// Replaces a vehicle at the specified index with a new vehicle
void VehicleManager::editVehicle(int index, std::unique_ptr<Vehicle> newData) {
    if (index >= 0 && index < static_cast<int>(vehicles.size())) {
        vehicles[index] = std::move(newData);
    }
}

// Saves the current vehicle collection to an XML file
void VehicleManager::saveToFile(const std::string& filename) {
    Serializer::serializeToFile(vehicles, filename);
}

// Loads a vehicle collection from an XML file
void VehicleManager::loadFromFile(const std::string& filename) {
    auto loaded = Serializer::deserializeFromFile(filename);
    if (!loaded.empty()) {
        vehicles = std::move(loaded);
    }
}

// Returns the number of vehicles in the collection
int VehicleManager::getCount() const {
    return static_cast<int>(vehicles.size());
}

// Returns a copy of the vehicle at the specified index
std::unique_ptr<Vehicle> VehicleManager::getVehicleCopy(int index) const {
    if (index >= 0 && index < static_cast<int>(vehicles.size())) {
        return vehicles[index]->clone();
    }
    return nullptr;
}

// Returns a const pointer to the vehicle at the specified index
const Vehicle* VehicleManager::getVehicleConst(int index) const {
    if (index >= 0 && index < static_cast<int>(vehicles.size())) {
        return vehicles[index].get();
    }
    return nullptr;
}