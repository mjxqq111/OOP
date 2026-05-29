#include "Triangle.hpp"
#include <algorithm>
#include <cmath>

namespace fig {

    // Default constructor (all zeroes)
    Triangle::Triangle() : p1(0, 0), p2(0, 0) {}

    // Parameterized constructor (creates triangle from two corners of bounding box)
    Triangle::Triangle(double x1, double y1, double x2, double y2)
        : p1(x1, y1), p2(x2, y2) {
    }

    // Set top-left corner (mouse down)
    void Triangle::setStart(const Point& p) {
        p1 = p;
        p2 = p;
    }

    // Set bottom-right corner (mouse up/drag)
    void Triangle::setEnd(const Point& p) {
        p2 = p;
    }

    // Get top-left corner
    Point Triangle::getStart() const { return p1; }

    // Get bottom-right corner
    Point Triangle::getEnd() const { return p2; }

    // Calculate bounding box (same as for a rectangle)
    wxRect Triangle::getBounds() const {
        int x = (int)std::min(p1.x, p2.x);
        int y = (int)std::min(p1.y, p2.y);
        int w = (int)std::abs(p1.x - p2.x);
        int h = (int)std::abs(p1.y - p2.y);
        return wxRect(x, y, w, h);
    }

    // Return shape type identifier
    wxString Triangle::getTypeName() const { return "Triangle"; }

}