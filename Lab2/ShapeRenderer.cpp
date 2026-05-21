#include "ShapeRenderer.h"
#include <map>
#include <functional>

// Static registry of drawing functions
std::map<wxString, ShapeRenderer::DrawFunction>& ShapeRenderer::getDrawRegistry() {
    static std::map<wxString, DrawFunction> registry;
    return registry;
}

// Register a drawing function for a shape type
void ShapeRenderer::registerDrawFunction(const wxString& typeName, DrawFunction func) {
    getDrawRegistry()[typeName] = func;
}

// Render a shape by dispatching to its registered drawing function
void ShapeRenderer::render(wxDC& dc, const std::shared_ptr<fig::Figure>& figure) {
    if (!figure) return;

    wxString typeName = figure->getTypeName();
    auto& registry = getDrawRegistry();
    auto it = registry.find(typeName);

    if (it != registry.end()) {
        it->second(dc, *figure);
    }
}