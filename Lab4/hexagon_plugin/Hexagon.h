#pragma once

#include "../Figure.hpp"
#include "../Point.hpp"
#include <vector>

namespace fig {

    // Hexagon shape inscribed in a bounding box
    class Hexagon : public Figure {
    public:
        Point p1, p2;  // Top-left and bottom-right corners

        // Constructor
        Hexagon();

        // Constructor with parameters
        Hexagon(double x1, double y1, double x2, double y2);

        void setStart(const Point& p) override; // Setting the start point
        void setEnd(const Point& p) override; // Setting the end point
        Point getStart() const override; // Getting the start point
        Point getEnd() const override; // Getting the end point

        wxRect getBounds() const override; // Getting the bounds of the area
        wxString getTypeName() const override; // Getting the type name of the hexagon

        // Geting hexagon vertices (6 points)
        std::vector<Point> getVertices() const;

    private:
        double getLeft() const; // Getting the smallest X
        double getRight() const; // Getting the largest X
        double getTop() const; // Getting the smallest Y
        double getBottom() const; // Getting the largest Y
    };

}