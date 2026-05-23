#include "Square.hpp"
#include <algorithm>
#include <cmath>

namespace fig {

    // Default constructor (all zeroes)
    Square::Square() : Rectangle(), a(0) {}

    // Constructor from top-left corner and side length
    Square::Square(Point p, double a)
        : Rectangle(p.x, p.y, p.x + a, p.y + a), a(a) {
    }

    // Force square shape (width == height)
    void Square::setEnd(const Point& p) {
        // Calculate distances from start point
        double dx = std::abs(p.x - p1.x);
        double dy = std::abs(p.y - p1.y);

        // Take the larger distance to ensure square shape
        double size = std::max(dx, dy);

        // Preserve the dragging direction
        int signX = (p.x >= p1.x) ? 1 : -1;
        int signY = (p.y >= p1.y) ? 1 : -1;

        // Set opposite corner to create perfect square
        p2 = Point(p1.x + signX * size, p1.y + signY * size);
        a = size;
    }

    // Return side length
    double Square::getSide() const {
        return a;
    }

}