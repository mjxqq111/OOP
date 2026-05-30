#pragma once

#include "Rectangle.hpp"

namespace fig {

    // Square is a special case of rectangle with equal width and height
    class Square : public Rectangle {
    public:
        double a;  // Side length

        // Default constructor (creates zero-sized square at origin)
        Square();

        // Constructor from top-left corner and side length
        Square(Point p, double a);

        // Override setEnd to force square shape (width == height)
        void setEnd(const Point& p) override;

        // Get side length of the square
        double getSide() const;
    };

}