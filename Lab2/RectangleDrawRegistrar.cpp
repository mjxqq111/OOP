#include "ShapeRenderer.h"
#include "Rectangle.hpp"
#include <algorithm>

namespace {

    void drawRectangle(wxDC& dc, const fig::Figure& figure) {
        const fig::Rectangle& rect = static_cast<const fig::Rectangle&>(figure);
        Point start = rect.getStart();
        Point end = rect.getEnd();

        int x = std::min((int)start.x, (int)end.x);
        int y = std::min((int)start.y, (int)end.y);
        int w = std::abs((int)(start.x - end.x));
        int h = std::abs((int)(start.y - end.y));

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawRectangle(x, y, w, h);
    }

    // Self-registration for Rectangle drawing
    struct RectangleDrawRegistrar {
        RectangleDrawRegistrar() {
            ShapeRenderer::registerDrawFunction("Rectangle", drawRectangle);
        }
    };

    static RectangleDrawRegistrar s_rectangleDrawRegistrar;

}