#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include "Triangle.hpp"
#include <algorithm>

// Self-registration for triangle
namespace {

    // Drawing function
    void drawTriangle(wxDC& dc, const fig::Figure& figure) {
        const fig::Triangle& triangle = static_cast<const fig::Triangle&>(figure);

        // Get the three vertices of the triangle
        wxPoint points[3];
        points[0] = wxPoint((int)triangle.getBaseLeft().x, (int)triangle.getBaseLeft().y);
        points[1] = wxPoint((int)triangle.getBaseRight().x, (int)triangle.getBaseRight().y);
        points[2] = wxPoint((int)triangle.getApex().x, (int)triangle.getApex().y);

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawPolygon(3, points);
    }

    // Self-registration for both creation and drawing
    struct TriangleRegistrar {
        // Constructor registers both creation and drawing
        TriangleRegistrar() {
            ShapeFactory::registerShape("Triangle", []() {
                return std::make_shared<fig::Triangle>();
                });
            ShapeRenderer::registerDrawFunction("Triangle", drawTriangle);
        }
    };

    static TriangleRegistrar s_triangleRegistrar;

}