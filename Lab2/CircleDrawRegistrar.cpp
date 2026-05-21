#include "ShapeRenderer.h"
#include "Circle.hpp"
#include <algorithm>

namespace {

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

    // Self-registration for Circle drawing
    struct CircleDrawRegistrar {
        CircleDrawRegistrar() {
            ShapeRenderer::registerDrawFunction("Circle", drawCircle);
        }
    };

    static CircleDrawRegistrar s_circleDrawRegistrar;

}