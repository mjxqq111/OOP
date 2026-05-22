#pragma once

#include "Vehicle.h"

// Helper macro to register vehicle types automatically with the factory
// This macro creates a static object that registers the class on program startup
#define REGISTER_VEHICLE(TypeName) \
    namespace { \
        struct Register##TypeName { \
            Register##TypeName() { \
                Vehicle::registerType(#TypeName, []() -> std::unique_ptr<Vehicle> { \
                    return std::make_unique<TypeName>(); \
                }); \
            } \
        }; \
        static Register##TypeName global_register_##TypeName; \
    }