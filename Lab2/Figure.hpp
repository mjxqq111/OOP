#pragma once

#include "Point.hpp"
#include <wx/wx.h>

namespace fig {

    // Abstract base class for shapes
    class Figure {
    public:
        virtual ~Figure() = default;  // Virtual destructor

        // Set the starting point (mouse down)
        virtual void setStart(const Point& p) = 0;

        // Set the ending point (mouse up/drag)
        virtual void setEnd(const Point& p) = 0;

        // Get the stored start point
        virtual Point getStart() const = 0;

        // Get the stored end point
        virtual Point getEnd() const = 0;

        // Get bounding box for screen invalidation
        virtual wxRect getBounds() const = 0;

        // Return shape type name (e.g., "Rectangle")
        virtual wxString getTypeName() const = 0;
    };

}