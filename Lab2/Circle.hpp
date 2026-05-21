#pragma once

#include "Ellipse.hpp"

namespace fig {

    // Perfect circle (special case of ellipse with equal width and height)
    class Circle : public Ellipse {
    public:
        // Default constructor
        Circle();

        // Constructor from center point and radius (creates a circle with given center and radius)
        Circle(Point center, double radius);

        // Override setEnd to force square bounding box (keeps circle perfect)
        void setEnd(const Point& p) override;

        // Calculate and return the radius of the circle
        double getRadius() const;
    };

}