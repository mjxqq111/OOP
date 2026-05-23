#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include "Square.hpp"
#include <algorithm>

// Self-registration for Square
namespace {

    // Drawing function
    void drawSquare(wxDC& dc, const fig::Figure& figure) {
        const fig::Square& square = static_cast<const fig::Square&>(figure);

        int x = (int)std::min(square.p1.x, square.p2.x);
        int y = (int)std::min(square.p1.y, square.p2.y);
        int size = (int)std::abs(square.p1.x - square.p2.x);

        if (size <= 0) return;

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawRectangle(x, y, size, size);
    }

    // Self-registration for both creation and drawing
    struct SquareRegistrar {
        // Constructor registers both creation and drawing
        SquareRegistrar() {
            ShapeFactory::registerShape("Square", []() {
                return std::make_shared<fig::Square>();
                });
            ShapeRenderer::registerDrawFunction("Square", drawSquare);
        }
    };

    static SquareRegistrar s_squareRegistrar;

}