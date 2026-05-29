#pragma once

#include "Figure.hpp"
#include "Point.hpp"

namespace fig {

    // Ellipse inscribed in a bounding box defined by two corners
    class Ellipse : public Figure {
    public:
        Point p1, p2;  // Top-left and bottom-right corners of bounding box

        // Default constructor
        Ellipse();

        // Parameterized constructor (creates ellipse from two opposite corners)
        Ellipse(double x1, double y1, double x2, double y2);

        // Set the top-left corner when mouse is pressed
        void setStart(const Point& p) override;

        // Set the bottom-right corner as mouse is dragged
        void setEnd(const Point& p) override;

        // Get the top-left corner (start point)
        Point getStart() const override;

        // Get the bottom-right corner (end point)
        Point getEnd() const override;

        // Calculate bounding box for screen invalidation
        wxRect getBounds() const override;

        // Return shape type identifier "Ellipse"
        wxString getTypeName() const override;

        // Get minimum X coordinate
        double getX() const { return std::min(p1.x, p2.x); }

        // Get minimum Y coordinate
        double getY() const { return std::min(p1.y, p2.y); }

        // Get width of bounding box
        double getWidth() const { return std::abs(p1.x - p2.x); }

        // Get height of bounding box
        double getHeight() const { return std::abs(p1.y - p2.y); }
    };

}