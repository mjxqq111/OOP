#pragma once

#include "Ellipse.hpp"

namespace fig {

    // Circle (equal width and height)
    class Circle : public Ellipse {
    public:
        Circle();
        Circle(Point center, double radius);

        void setEnd(const Point& p) override;
        double getRadius() const;
    };

}