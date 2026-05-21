#pragma once

#include "Point.hpp"
#include "Figure.hpp"

namespace fig {

    // Line defined by two endpoints
    class Line : public Figure {
    public:
        Point p1, p2;  // Start and end points

        // Default constructor (creates zero-length line at origin)
        Line();

        // Parameterized constructor (creates line from two points)
        Line(double x1, double y1, double x2, double y2);

        // Set the first endpoint (mouse down position)
        void setStart(const Point& p) override;

        // Set the second endpoint (mouse up/drag position)
        void setEnd(const Point& p) override;

        // Get the first endpoint
        Point getStart() const override;

        // Get the second endpoint
        Point getEnd() const override;

        // Calculate bounding box for screen invalidation
        wxRect getBounds() const override;

        // Return shape type identifier "Line"
        wxString getTypeName() const override;
    };

}