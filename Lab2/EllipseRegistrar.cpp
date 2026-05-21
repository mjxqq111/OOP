#include "ShapeFactory.h"
#include "Ellipse.hpp"

// Self-registration for Ellipse
namespace {
    struct EllipseRegistrar {
        EllipseRegistrar() {
            ShapeFactory::registerShape("Ellipse", []() {
                return std::make_shared<fig::Ellipse>();
                });
        }
    };
    static EllipseRegistrar s_ellipseRegistrar;
}