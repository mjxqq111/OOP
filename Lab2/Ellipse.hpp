#pragma once

#include "Figure.hpp"
#include "Point.hpp"

namespace fig {

    // Ellipse inscribed in a bounding box
    class Ellipse : public Figure {
    public:
        Point p1, p2;

        Ellipse();
        Ellipse(double x1, double y1, double x2, double y2);

        void setStart(const Point& p) override;
        void setEnd(const Point& p) override;
        Point getStart() const override;
        Point getEnd() const override;

        wxRect getBounds() const override;
        wxString getTypeName() const override;

        double getX() const { return std::min(p1.x, p2.x); }
        double getY() const { return std::min(p1.y, p2.y); }
        double getWidth() const { return std::abs(p1.x - p2.x); }
        double getHeight() const { return std::abs(p1.y - p2.y); }
    };

}