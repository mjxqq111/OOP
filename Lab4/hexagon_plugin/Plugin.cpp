#include "../ShapeFactory.h"
#include "../ShapeRenderer.h"
#include "Hexagon.h"
#include <vector>

#define EXPORT __declspec(dllexport)

// Drawing function for Hexagon
static void drawHexagon(wxDC& dc, const fig::Figure& figure) {
    const fig::Hexagon& hexagon = static_cast<const fig::Hexagon&>(figure);
    std::vector<Point> vertices = hexagon.getVertices();

    if (vertices.size() < 3) return;

    std::vector<wxPoint> wxVertices;
    for (const auto& p : vertices) {
        wxVertices.push_back(wxPoint((int)p.x, (int)p.y));
    }

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawPolygon((int)wxVertices.size(), wxVertices.data());
}

// Creator function
static std::shared_ptr<fig::Hexagon> createHexagon() {
    return std::make_shared<fig::Hexagon>();
}

// Register shape and draw function
extern "C" EXPORT void initPlugin(void* registerShapeFunc, void* registerDrawFunc) {
    typedef void (*RegisterShapeFuncType)(const char*, void*);
    typedef void (*RegisterDrawFuncType)(const char*, void*);

    RegisterShapeFuncType registerShape = (RegisterShapeFuncType)registerShapeFunc;
    RegisterDrawFuncType registerDraw = (RegisterDrawFuncType)registerDrawFunc;

    if (registerShape) {
        registerShape("Hexagon", (void*)createHexagon);
    }

    if (registerDraw) {
        registerDraw("Hexagon", (void*)drawHexagon);
    }
}

// Get shape name
extern "C" EXPORT const char* getShapeName() {
    return "Hexagon";
}

// Get plugin name
extern "C" EXPORT const char* getPluginName() {
    return "Hexagon";
}