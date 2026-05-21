#include "ShapeFactory.h"
#include "Rectangle.hpp"

// Self-registration for Rectangle
namespace {
    struct RectangleRegistrar {
        RectangleRegistrar() {
            ShapeFactory::registerShape("Rectangle", []() {
                return std::make_shared<fig::Rectangle>();
                });
        }
    };
    static RectangleRegistrar s_rectangleRegistrar;
}