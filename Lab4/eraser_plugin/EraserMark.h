#pragma once

#include "../Figure.hpp"
#include "../Point.hpp"

namespace fig {

    // Eraser mark (white circle without outline)
    class EraserMark : public Figure {
    public:
        Point center;
        double radius;

        EraserMark();   // Constructor
        EraserMark(Point center, double radius);    // Constructor with parameters

        void setStart(const Point& p) override; // Sets the first corner
        void setEnd(const Point& p) override;   // Sets the opposite corner
        Point getStart() const override;    // Returns the first corner
        Point getEnd() const override;  // Returns the opposite corner

        wxRect getBounds() const override;  // Returns bounds of the figure
        wxString getTypeName() const override;  // Returns type name of the figure

        double getRadius() const;   // Returns radius of the mark
        Point getCenter() const;    // Returns center of the mark
    };

}