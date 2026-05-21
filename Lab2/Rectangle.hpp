#pragma once

#include "Point.hpp"
#include "Figure.hpp"

namespace fig {

    // Rectangle defined by two opposite corners
    class Rectangle : public Figure {
    public:
        Point p1, p2;

        Rectangle();
        Rectangle(double x1, double y1, double x2, double y2);

        void setStart(const Point& p) override;
        void setEnd(const Point& p) override;
        Point getStart() const override;
        Point getEnd() const override;

        wxRect getBounds() const override;
        wxString getTypeName() const override;
    };

}