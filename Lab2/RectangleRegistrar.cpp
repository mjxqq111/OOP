#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include "Rectangle.hpp"
#include <algorithm>

// Self-registration for Rectangle
namespace {

    // Drawing function
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

    // Self-registration for both creation and drawing
    struct RectangleRegistrar {
        RectangleRegistrar() {
            ShapeFactory::registerShape("Rectangle", []() {
                return std::make_shared<fig::Rectangle>();
                });
            ShapeRenderer::registerDrawFunction("Rectangle", drawRectangle);
        }
    };

    static RectangleRegistrar s_rectangleRegistrar;

}