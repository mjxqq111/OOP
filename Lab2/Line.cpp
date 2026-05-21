#include "Line.hpp"
#include <algorithm>
#include <cmath>

namespace fig {

    // Default constructor (all zeroes)
    Line::Line() : p1(0, 0), p2(0, 0) {}

    // Parameterized constructor (creates line from two points)
    Line::Line(double x1, double y1, double x2, double y2)
        : p1(x1, y1), p2(x2, y2) {
    }

    // Set the first endpoint (mouse down)
    void Line::setStart(const Point& p) { p1 = p; }

    // Set the second endpoint (mouse up/drag)
    void Line::setEnd(const Point& p) { p2 = p; }

    // Get the first endpoint
    Point Line::getStart() const { return p1; }

    // Get the second endpoint
    Point Line::getEnd() const { return p2; }

    // Calculate bounding box that contains the line
    wxRect Line::getBounds() const {
        int x = (int)std::min(p1.x, p2.x);
        int y = (int)std::min(p1.y, p2.y);
        int w = (int)std::abs(p1.x - p2.x);
        int h = (int)std::abs(p1.y - p2.y);
        // Ensure minimum size for visibility
        if (w == 0 && h == 0) {
            return wxRect(x, y, 1, 1);
        }
        return wxRect(x, y, w, h);
    }

    // Return shape type identifier
    wxString Line::getTypeName() const { return "Line"; }

}