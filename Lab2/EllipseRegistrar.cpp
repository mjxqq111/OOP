#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include "Ellipse.hpp"
#include <algorithm>

// Self-registration for Ellipse
namespace {

    // Drawing function
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

    // Self-registration for both creation and drawing
    struct EllipseRegistrar {
        EllipseRegistrar() {
            ShapeFactory::registerShape("Ellipse", []() {
                return std::make_shared<fig::Ellipse>();
                });
            ShapeRenderer::registerDrawFunction("Ellipse", drawEllipse);
        }
    };

    static EllipseRegistrar s_ellipseRegistrar;

}