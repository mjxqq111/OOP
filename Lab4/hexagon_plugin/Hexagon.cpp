#include "Hexagon.h"
#include <algorithm>
#include <cmath>

namespace fig {
    // Constructor (all zeroes)
    Hexagon::Hexagon() : p1(0, 0), p2(0, 0) {}

    // Constructor with parameters
    Hexagon::Hexagon(double x1, double y1, double x2, double y2)
        : p1(x1, y1), p2(x2, y2) {
    }

    void Hexagon::setStart(const Point& p) { p1 = p; p2 = p; }  // Setting the start point
    void Hexagon::setEnd(const Point& p) { p2 = p; }    // Setting the end point
    Point Hexagon::getStart() const { return p1; }  // Getting the start point
    Point Hexagon::getEnd() const { return p2; }    // Getting the end point

    // Getting the bounds of the area
    wxRect Hexagon::getBounds() const {
        int x = (int)std::min(p1.x, p2.x);
        int y = (int)std::min(p1.y, p2.y);
        int w = (int)std::abs(p1.x - p2.x);
        int h = (int)std::abs(p1.y - p2.y);
        return wxRect(x, y, w, h);
    }

    // Getting the type name of the hexagon
    wxString Hexagon::getTypeName() const { return "Hexagon"; }

    // Getting the vertices of the hexagon
    std::vector<Point> Hexagon::getVertices() const {
        std::vector<Point> vertices;

        // Using first point as center
        double cx = p1.x;
        double cy = p1.y;

        double dx = std::abs(p2.x - p1.x);
        double dy = std::abs(p2.y - p1.y);
        double radius = std::sqrt(dx * dx + dy * dy);

        // Minimum radius to avoid bad shape
        if (radius < 1.0) radius = 1.0;

        const double PI = 3.1415926535898;

        // Generating 6 vertices of a regular hexagon
        for (int i = 0; i < 6; ++i) {
            double angle = i * 60.0 * PI / 180.0;
            double x = cx + radius * cos(angle);
            double y = cy + radius * sin(angle);
            vertices.push_back(Point(x, y));
        }

        return vertices;
    }

    double Hexagon::getLeft() const { return std::min(p1.x, p2.x); }    // Getting the smallest X
    double Hexagon::getRight() const { return std::max(p1.x, p2.x); }   // Getting the largest X
    double Hexagon::getTop() const { return std::min(p1.y, p2.y); } // Getting the smallest Y
    double Hexagon::getBottom() const { return std::max(p1.y, p2.y); }  // Getting the largest Y

}