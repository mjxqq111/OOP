#include "Rhombus.h"
#include <algorithm>
#include <cmath>

namespace fig {
    // Constructor (all zeroes)
    Rhombus::Rhombus() : p1(0, 0), p2(0, 0) {}

    // Constructor with parameters
    Rhombus::Rhombus(double x1, double y1, double x2, double y2)
        : p1(x1, y1), p2(x2, y2) {
    }
    
    void Rhombus::setStart(const Point& p) { p1 = p; p2 = p; }  // Setting the start point  
    void Rhombus::setEnd(const Point& p) { p2 = p; }    // Setting the end point
    Point Rhombus::getStart() const { return p1; }  // Getting the start point
    Point Rhombus::getEnd() const { return p2; }    // Getting the end point

    // Getting the bounds of the area
    wxRect Rhombus::getBounds() const {
        int x = (int)std::min(p1.x, p2.x);
        int y = (int)std::min(p1.y, p2.y);
        int w = (int)std::abs(p1.x - p2.x);
        int h = (int)std::abs(p1.y - p2.y);
        return wxRect(x, y, w, h);
    }

    // Getting the type name
    wxString Rhombus::getTypeName() const {
        return "Rhombus";
    }

    // Getting the vertices
    std::vector<Point> Rhombus::getVertices() const {
        std::vector<Point> vertices;

        double left = getLeft();
        double right = getRight();
        double top = getTop();
        double bottom = getBottom();

        double centerX = (left + right) / 2.0;
        double centerY = (top + bottom) / 2.0;

        // Rhombus vertices: top, right, bottom, left
        vertices.push_back(Point(centerX, top));      // Top
        vertices.push_back(Point(right, centerY));    // Right
        vertices.push_back(Point(centerX, bottom));   // Bottom
        vertices.push_back(Point(left, centerY));     // Left

        return vertices;
    }

    double Rhombus::getLeft() const   { return std::min(p1.x, p2.x); }  // Getting the smallest X
    double Rhombus::getRight() const  { return std::max(p1.x, p2.x); }  // Getting the largest X
    double Rhombus::getTop() const    { return std::min(p1.y, p2.y); }  // Getting the smallest Y
    double Rhombus::getBottom() const { return std::max(p1.y, p2.y); }  // Getting the largest Y

}