#include "ShapeFactory.h"
#include "Circle.hpp"

// Self-registration for Circle
namespace {
    struct CircleRegistrar {
        CircleRegistrar() {
            ShapeFactory::registerShape("Circle", []() {
                return std::make_shared<fig::Circle>();
                });
        }
    };
    static CircleRegistrar s_circleRegistrar;
}