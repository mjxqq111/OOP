#pragma once

#include "Point.hpp"
#include "Figure.hpp"

namespace fig {

    // Rectangle defined by two opposite corners
    class Rectangle : public Figure {
    public:
        Point p1, p2;  // First and second corners

        // Default constructor (creates zero-sized rectangle at origin)
        Rectangle();

        // Parameterized constructor (creates rectangle from two opposite corners)
        Rectangle(double x1, double y1, double x2, double y2);

        // Set the first corner (mouse down position)
        void setStart(const Point& p) override;

        // Set the opposite corner (mouse up/drag position)
        void setEnd(const Point& p) override;

        // Get the first corner
        Point getStart() const override;

        // Get the opposite corner
        Point getEnd() const override;

        // Calculate bounding rectangle with proper ordering
        wxRect getBounds() const override;

        // Return shape type identifier "Rectangle"
        wxString getTypeName() const override;
    };

}