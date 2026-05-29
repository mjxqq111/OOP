#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include "Line.hpp"
#include <algorithm>

// Self-registration for Line
namespace {

    // Drawing function
    void drawLine(wxDC& dc, const fig::Figure& figure) {
        const fig::Line& line = static_cast<const fig::Line&>(figure);

        int x1 = (int)line.p1.x;
        int y1 = (int)line.p1.y;
        int x2 = (int)line.p2.x;
        int y2 = (int)line.p2.y;

        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(x1, y1, x2, y2);
    }

    // Self-registration for both creation and drawing
    struct LineRegistrar {
        // Constructor registers both creation and drawing
        LineRegistrar() {
            ShapeFactory::registerShape("Line", []() {
                return std::make_shared<fig::Line>();
                });
            ShapeRenderer::registerDrawFunction("Line", drawLine);
        }
    };

    static LineRegistrar s_lineRegistrar;

}