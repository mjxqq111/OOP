#include "EraserMark.h"
#include <algorithm>
#include <cmath>

namespace fig {

    // Constructor (all zeroes)
    EraserMark::EraserMark() : center(0, 0), radius(0) {}

    // Constructor with parameters
    EraserMark::EraserMark(Point center, double radius)
        : center(center), radius(radius) {
    }

    // Sets the first corner
    void EraserMark::setStart(const Point& p) {
        center = p;
        radius = 0;
    }

    // Sets the opposite corner
    void EraserMark::setEnd(const Point& p) {
        double dx = std::abs(p.x - center.x);
        double dy = std::abs(p.y - center.y);
        radius = std::max(dx, dy);
    }

    // Returns the first corner
    Point EraserMark::getStart() const {
        return center;
    }

    // Returns the opposite corner
    Point EraserMark::getEnd() const {
        return Point(center.x + radius, center.y + radius);
    }

    // Returns bounds of the figure
    wxRect EraserMark::getBounds() const {
        int x = (int)(center.x - radius);
        int y = (int)(center.y - radius);
        int size = (int)(radius * 2);
        return wxRect(x, y, size, size);
    }

    // Returns type name of the figure
    wxString EraserMark::getTypeName() const {
        return "EraserMark";
    }

    // Returns radius of the mark
    double EraserMark::getRadius() const { 
        return radius; 
    }

    // Returns center of the mark
    Point EraserMark::getCenter() const { 
        return center; 
    }

}