#include "ShapeRenderer.h"
#include "Ellipse.hpp"
#include <algorithm>

namespace {

    void drawEllipse(wxDC& dc, const fig::Figure& figure) {
        const fig::Ellipse& ellipse = static_cast<const fig::Ellipse&>(figure);

        int x = (int)std::min(ellipse.p1.x, ellipse.p2.x);
        int y = (int)std::min(ellipse.p1.y, ellipse.p2.y);
        int w = (int)std::abs(ellipse.p1.x - ellipse.p2.x);
        int h = (int)std::abs(ellipse.p1.y - ellipse.p2.y);

        if (w <= 0 || h <= 0) return;

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawEllipse(x, y, w, h);
    }

    // Self-registration for Ellipse drawing
    struct EllipseDrawRegistrar {
        EllipseDrawRegistrar() {
            ShapeRenderer::registerDrawFunction("Ellipse", drawEllipse);
        }
    };

    static EllipseDrawRegistrar s_ellipseDrawRegistrar;

}