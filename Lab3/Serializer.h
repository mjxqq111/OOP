#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Vehicle.h"

// Handles XML serialization and deserialization of vehicle collections
class Serializer {
public:
    // Writes a collection of vehicles to an XML file
    static void serializeToFile(const std::vector<std::unique_ptr<Vehicle>>& vehicles,
        const std::string& filename);

    // Reads a collection of vehicles from an XML file
    static std::vector<std::unique_ptr<Vehicle>> deserializeFromFile(const std::string& filename);
};