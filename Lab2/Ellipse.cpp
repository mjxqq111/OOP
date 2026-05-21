#include "Ellipse.hpp"
#include <algorithm>
#include <cmath>

namespace fig {

    Ellipse::Ellipse() : p1(0, 0), p2(0, 0) {}
    Ellipse::Ellipse(double x1, double y1, double x2, double y2)
        : p1(x1, y1), p2(x2, y2) {
    }

    // Set both corners to the same point when mouse is pressed
    void Ellipse::setStart(const Point& p) { p1 = p; p2 = p; }

    // Update the second corner as mouse is dragged
    void Ellipse::setEnd(const Point& p) { p2 = p; }

    Point Ellipse::getStart() const { return p1; }
    Point Ellipse::getEnd() const { return p2; }

    wxRect Ellipse::getBounds() const {
        int x = (int)std::min(p1.x, p2.x);
        int y = (int)std::min(p1.y, p2.y);
        int w = (int)std::abs(p1.x - p2.x);
        int h = (int)std::abs(p1.y - p2.y);
        return wxRect(x, y, w, h);
    }

    wxString Ellipse::getTypeName() const { return "Ellipse"; }

}