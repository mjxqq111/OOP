#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include "Circle.hpp"
#include <algorithm>

// Self-registration for Circle
namespace {

    // Drawing function
    void drawCircle(wxDC& dc, const fig::Figure& figure) {
        const fig::Circle& circle = static_cast<const fig::Circle&>(figure);

        int x = (int)std::min(circle.p1.x, circle.p2.x);
        int y = (int)std::min(circle.p1.y, circle.p2.y);
        int size = (int)std::abs(circle.p1.x - circle.p2.x);

        if (size <= 0) return;

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawEllipse(x, y, size, size);
    }

    // Self-registration for both creation and drawing
    struct CircleRegistrar {
        CircleRegistrar() {
            ShapeFactory::registerShape("Circle", []() {
                return std::make_shared<fig::Circle>();
                });
            ShapeRenderer::registerDrawFunction("Circle", drawCircle);
        }
    };

    static CircleRegistrar s_circleRegistrar;

}