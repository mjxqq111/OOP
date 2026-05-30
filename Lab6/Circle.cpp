#include "Circle.hpp"
#include <algorithm>
#include <cmath>

namespace fig {

    Circle::Circle() : Ellipse() {} // Default constructor
    // Constructor. Creates a square bounding box where width = height = radius * 2
    Circle::Circle(Point center, double radius)
        : Ellipse(center.x - radius, center.y - radius,
            center.x + radius, center.y + radius) {
    }

    // Force square bounding box to keep circle perfect
    void Circle::setEnd(const Point& p) {
        double dx = std::abs(p.x - p1.x);
        double dy = std::abs(p.y - p1.y);
        double size = std::max(dx, dy);
        int signX = (p.x >= p1.x) ? 1 : -1;
        int signY = (p.y >= p1.y) ? 1 : -1;
        p2 = Point(p1.x + signX * size, p1.y + signY * size);
    }

    // Getting radius of a circle
    double Circle::getRadius() const { return std::abs(p2.x - p1.x) / 2.0; }

}