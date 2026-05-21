#include "Rectangle.hpp"
#include <algorithm>
#include <cmath>

namespace fig {

    // Default constructor (all zeroes)
    Rectangle::Rectangle() : p1(0, 0), p2(0, 0) {}

    // Parameterized constructor (creates rectangle from two opposite corners)
    Rectangle::Rectangle(double x1, double y1, double x2, double y2)
        : p1(x1, y1), p2(x2, y2) {
    }

    // Set the first corner (mouse down position)
    void Rectangle::setStart(const Point& p) { p1 = p; }

    // Set the opposite corner (mouse up/drag position)
    void Rectangle::setEnd(const Point& p) { p2 = p; }

    // Get the first corner
    Point Rectangle::getStart() const { return p1; }

    // Get the opposite corner
    Point Rectangle::getEnd() const { return p2; }

    // Calculate bounding rectangle with proper ordering (handles negative drag)
    wxRect Rectangle::getBounds() const {
        int x = std::min((int)p1.x, (int)p2.x);
        int y = std::min((int)p1.y, (int)p2.y);
        int w = std::abs((int)(p1.x - p2.x));
        int h = std::abs((int)(p1.y - p2.y));
        return wxRect(x, y, w, h);
    }

    // Return shape type identifier used for dispatch in ShapeRenderer
    wxString Rectangle::getTypeName() const { return "Rectangle"; }

}