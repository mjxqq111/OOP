#pragma once

#include <wx/wx.h>
#include <memory>
#include <functional>
#include <map>
#include "Figure.hpp"

// Separate drawing logic from shapes
class ShapeRenderer {
public:
    // Function type for drawing callbacks
    using DrawFunction = std::function<void(wxDC&, const fig::Figure&)>;

    // Render a shape by dispatching to its registered drawing function
    void render(wxDC& dc, const std::shared_ptr<fig::Figure>& figure);

    // Register a drawing function for a shape type
    static void registerDrawFunction(const wxString& typeName, DrawFunction func);

private:
    // Get the static drawing registry map
    static std::map<wxString, DrawFunction>& getDrawRegistry();
};