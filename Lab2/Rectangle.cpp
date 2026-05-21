#include "Rectangle.hpp"
#include <algorithm>
#include <cmath>

namespace fig {

    Rectangle::Rectangle() : p1(0, 0), p2(0, 0) {}
    Rectangle::Rectangle(double x1, double y1, double x2, double y2)
        : p1(x1, y1), p2(x2, y2) {
    }

    void Rectangle::setStart(const Point& p) { p1 = p; }
    void Rectangle::setEnd(const Point& p) { p2 = p; }
    Point Rectangle::getStart() const { return p1; }
    Point Rectangle::getEnd() const { return p2; }

    // Calculate bounding box with proper ordering
    wxRect Rectangle::getBounds() const {
        int x = std::min((int)p1.x, (int)p2.x);
        int y = std::min((int)p1.y, (int)p2.y);
        int w = std::abs((int)(p1.x - p2.x));
        int h = std::abs((int)(p1.y - p2.y));
        return wxRect(x, y, w, h);
    }

    wxString Rectangle::getTypeName() const { return "Rectangle"; }

}