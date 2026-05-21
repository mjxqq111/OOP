#pragma once

#include "Point.hpp"
#include <wx/wx.h>

namespace fig {

    // Abstract base class for shapes
    class Figure {
    public:
        virtual ~Figure() = default;

        virtual void setStart(const Point& p) = 0;
        virtual void setEnd(const Point& p) = 0;
        virtual Point getStart() const = 0;
        virtual Point getEnd() const = 0;

        virtual wxRect getBounds() const = 0;
        virtual wxString getTypeName() const = 0;
    };

}