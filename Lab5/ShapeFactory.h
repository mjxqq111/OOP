#pragma once

#include <memory>
#include <wx/arrstr.h>
#include <wx/string.h>
#include <map>
#include <functional>
#include "Figure.hpp"

// Self-registering factory for creating shapes by name
class ShapeFactory {
public:
    // Function type for shape creators
    using Creator = std::function<std::shared_ptr<fig::Figure>()>;

    // Create a shape by its type name
    static std::shared_ptr<fig::Figure> createShape(const wxString& typeName);

    // Get list of all registered shape type names (for UI dropdown)
    static wxArrayString getAvailableTypes();

    // Register a shape type with its creator function
    static void registerShape(const wxString& typeName, Creator creator);

private:
    // Get the static registry map
    static std::map<wxString, Creator>& getRegistry();
};