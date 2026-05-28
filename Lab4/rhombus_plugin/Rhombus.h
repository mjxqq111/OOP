#pragma once

#include "../Figure.hpp"
#include "../Point.hpp"
#include <vector>

namespace fig {

    // Rhombus inscribed in a bounding box
    class Rhombus : public Figure {
    public:
        Point p1, p2;  // Top-left and bottom-right corners

        // Constructor
        Rhombus();

        // Constructor with parameters
        Rhombus(double x1, double y1, double x2, double y2);

        void setStart(const Point& p) override; // Setting the start point
        void setEnd(const Point& p) override; // Setting the end point
        Point getStart() const override;    // Getting the start point
        Point getEnd() const override;  // Getting the end point

        // Getting the bounds of the area
        wxRect getBounds() const override;

        // Getting type name
        wxString getTypeName() const override;

        // Get diamond vertices (4 points)
        std::vector<Point> getVertices() const;

    private:
        double getLeft() const; // Getting the smallest X
        double getRight() const; // Getting the largest X
        double getTop() const; // Getting the smallest Y
        double getBottom() const; // Getting the largest Y
    };

}