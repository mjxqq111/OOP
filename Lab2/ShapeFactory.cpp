#include "ShapeFactory.h"
#include <map>

// Static map: typeName -> creator function
std::map<wxString, ShapeFactory::Creator>& ShapeFactory::getRegistry() {
    static std::map<wxString, Creator> registry;
    return registry;
}

void ShapeFactory::registerShape(const wxString& typeName, Creator creator) {
    getRegistry()[typeName] = creator;
}

// Create a shape instance by looking up its creator function
std::shared_ptr<fig::Figure> ShapeFactory::createShape(const wxString& typeName) {
    auto& registry = getRegistry();
    auto it = registry.find(typeName);
    if (it != registry.end()) {
        return it->second();
    }
    return nullptr;
}

wxArrayString ShapeFactory::getAvailableTypes() {
    wxArrayString types;
    for (const auto& pair : getRegistry()) {
        types.Add(pair.first);
    }
    return types;
}