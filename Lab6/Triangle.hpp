#pragma once

#include "Point.hpp"
#include "Figure.hpp"

namespace fig {

    // Isosceles triangle inscribed in a bounding box
    // Flips direction based on drag direction
    class Triangle : public Figure {
    public:
        Point p1, p2;  // First and second corners of bounding box

        // Default constructor (creates zero-sized triangle)
        Triangle();

        // Parameterized constructor (creates triangle from bounding box corners)
        Triangle(double x1, double y1, double x2, double y2);

        // Set first corner (mouse down)
        void setStart(const Point& p) override;

        // Set second corner (mouse up/drag)
        void setEnd(const Point& p) override;

        // Get first corner
        Point getStart() const override;

        // Get second corner
        Point getEnd() const override;

        // Calculate bounding box for screen invalidation
        wxRect getBounds() const override;

        // Return shape type identifier "Triangle"
        wxString getTypeName() const override;

        // Helper methods for bounding box
        double getLeft() const { return std::min(p1.x, p2.x); }
        double getRight() const { return std::max(p1.x, p2.x); }
        double getTop() const { return std::min(p1.y, p2.y); }
        double getBottom() const { return std::max(p1.y, p2.y); }

        // Check if triangle points up (apex at top) or down (apex at bottom)
        bool pointsUp() const { return p1.y <= p2.y; }

        // Get the left endpoint of the triangle's base
        Point getBaseLeft() const {
            if (pointsUp()) {
                return Point(getLeft(), getBottom());
            }
            else {
                return Point(getLeft(), getTop());
            }
        }

        // Get the right endpoint of the triangle's base
        Point getBaseRight() const {
            if (pointsUp()) {
                return Point(getRight(), getBottom());
            }
            else {
                return Point(getRight(), getTop());
            }
        }

        // Get the apex (top if pointing up, bottom if pointing down)
        Point getApex() const {
            if (pointsUp()) {
                return Point((getLeft() + getRight()) / 2.0, getTop());
            }
            else {
                return Point((getLeft() + getRight()) / 2.0, getBottom());
            }
        }
    };

}