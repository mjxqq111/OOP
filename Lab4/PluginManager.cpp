#include "PluginManager.h"
#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include <windows.h>

// Register a shape creator function
extern "C" void Bridge_RegisterShape(const char* typeName, void* creatorFunc) {
    if (!typeName || !creatorFunc) {
        return;  // Invalid parameters
    }

    wxString name(typeName);

    // Convert void* to actual function pointer
    auto creator = reinterpret_cast<std::shared_ptr<fig::Figure>(*)()>(creatorFunc);

    // Register in the main program's ShapeFactory
    ShapeFactory::registerShape(name, [creator]() {
        return creator();
    });
}

// Register a draw function for a shape
extern "C" void Bridge_RegisterDrawFunction(const char* typeName, void* drawFunc) {
    if (!typeName || !drawFunc) {
        return;  // Invalid parameters
    }

    wxString name(typeName);

    // Convert void* to actual function pointer
    auto drawer = reinterpret_cast<void(*)(wxDC&, const fig::Figure&)>(drawFunc);

    // Register in the main program's ShapeRenderer
    ShapeRenderer::registerDrawFunction(name, [drawer](wxDC& dc, const fig::Figure& fig) {
        drawer(dc, fig);
    });
}

// Get singleton instance
PluginManager& PluginManager::getInstance() {
    static PluginManager instance;
    return instance;
}

// Destructor
PluginManager::~PluginManager() {
    unloadPlugins();
}

// Load a single plugin DLL
bool PluginManager::loadPluginFromFile(const wxString& dllPath) {
    // Openning DLL temporarily to get name
    void* tempHandle = LoadLibraryA(dllPath.c_str());
    if (!tempHandle) {
        wxLogError("Failed to open DLL: %s", dllPath);
        return false;
    }

    // Trying to get plugin name
    GetPluginNameFunc getPluginName = reinterpret_cast<GetPluginNameFunc>(GetProcAddress((HMODULE)tempHandle, "getPluginName"));

    wxString pluginName;
    if (getPluginName != nullptr) {
        pluginName = getPluginName();
    }
    else {
        wxLogError("Plugin has no getPluginName: %s", dllPath);
        FreeLibrary((HMODULE)tempHandle);
        return false;
    }

    // Checking if plugin with this name is already loaded
    for (const auto& plugin : m_plugins) {
        if (plugin.name == pluginName) {
            wxMessageBox("Plugin '" + pluginName + "' already loaded!",
                "Plugin Error", wxOK | wxICON_WARNING);
            FreeLibrary((HMODULE)tempHandle);
            return false;
        }
    }

    // Closing temporary handle and load DLL for real
    FreeLibrary((HMODULE)tempHandle);

    void* handle = LoadLibraryA(dllPath.c_str());
    if (!handle) {
        wxLogError("Failed to load DLL: %s", dllPath);
        return false;
    }

    // Initiliazing plugin
    InitPluginFunc initPlugin = (InitPluginFunc)GetProcAddress((HMODULE)handle, "initPlugin");
    if (initPlugin) {
        initPlugin((void*)Bridge_RegisterShape, (void*)Bridge_RegisterDrawFunction);
    }

    // Storing plugin info
    PluginInfo info;
    info.name = pluginName;
    info.handle = handle;
    info.initUIFunc = reinterpret_cast<InitUIFunc>(GetProcAddress((HMODULE)handle, "initUI"));

    m_plugins.push_back(info);
    return true;
}

// Initializing plugin UI
void PluginManager::initPluginUI(const PluginInfo& pl, wxWindow* parent, EditorCanvas* canvas) {
    if (pl.initUIFunc) {
        pl.initUIFunc((void*)parent, (void*)canvas);
    }
}

// Returns a list of plugins
const std::vector<PluginInfo>& PluginManager::getPlugins() const {
    return m_plugins;
}

// Unload all plugins
void PluginManager::unloadPlugins() {
    for (auto& plugin : m_plugins) {
        unloadPlugin(plugin);
    }
    m_plugins.clear();
}

// Unload a single plugin
void PluginManager::unloadPlugin(PluginInfo& plugin) {
    if (plugin.handle) {
        FreeLibrary((HMODULE)plugin.handle);
        plugin.handle = nullptr;
    }
}