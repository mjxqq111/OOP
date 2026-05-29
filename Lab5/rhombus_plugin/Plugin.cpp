#include "Rhombus.h"
#include <vector>

#define EXPORT __declspec(dllexport)

// Drawing function for Rhombus
static void drawRhombus(wxDC& dc, const fig::Figure& figure) {
    const fig::Rhombus& rhombus = static_cast<const fig::Rhombus&>(figure);
    std::vector<Point> vertices = rhombus.getVertices();

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
static std::shared_ptr<fig::Rhombus> createRhombus() {
    return std::make_shared<fig::Rhombus>();
}

// Register shape and draw function
extern "C" EXPORT void initPlugin(void* registerShapeFunc, void* registerDrawFunc) {
    typedef void (*RegisterShapeFuncType)(const char*, void*);
    typedef void (*RegisterDrawFuncType)(const char*, void*);

    RegisterShapeFuncType registerShape = (RegisterShapeFuncType)registerShapeFunc;
    RegisterDrawFuncType registerDraw = (RegisterDrawFuncType)registerDrawFunc;

    if (registerShape) {
        registerShape("Rhombus", (void*)createRhombus);
    }

    if (registerDraw) {
        registerDraw("Rhombus", (void*)drawRhombus);
    }
}

// Get shape name
extern "C" EXPORT const char* getShapeName() {
    return "Rhombus";
}

// Get plugin name
extern "C" EXPORT const char* getPluginName() {
    return "Rhombus";
}