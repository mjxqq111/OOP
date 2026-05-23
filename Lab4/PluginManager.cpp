#include "PluginManager.h"
#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/log.h>

#ifdef _WIN32
#include <windows.h>
#define DLOPEN(path) LoadLibraryA(path)
#define DLSYM(handle, name) GetProcAddress((HMODULE)handle, name)
#define DLCLOSE(handle) FreeLibrary((HMODULE)handle)
#else
#include <dlfcn.h>
#define DLOPEN(path) dlopen(path, RTLD_LAZY)
#define DLSYM(handle, name) dlsym(handle, name)
#define DLCLOSE(handle) dlclose(handle)
#endif

// Get singleton instance
PluginManager& PluginManager::getInstance() {
    static PluginManager instance;
    return instance;
}

// Destructor - unload all plugins
PluginManager::~PluginManager() {
    unloadPlugins();
}

// Load all plugins from directory
void PluginManager::loadPlugins(const wxString& pluginDir) {
    // Create directory if it doesn't exist
    if (!wxDirExists(pluginDir)) {
        wxMkdir(pluginDir);
        return;
    }

    wxDir dir(pluginDir);
    if (!dir.IsOpened()) return;

    wxString filename;
    bool cont = dir.GetFirst(&filename, "*.dll", wxDIR_FILES);

    while (cont) {
        wxString fullPath = pluginDir + wxFileName::GetPathSeparator() + filename;
        loadPlugin(fullPath);
        cont = dir.GetNext(&filename);
    }
}

// Load a single plugin DLL
bool PluginManager::loadPlugin(const wxString& dllPath) {
    void* handle = DLOPEN(dllPath.c_str());
    if (!handle) {
        wxLogError("Failed to load plugin: %s", dllPath);
        return false;
    }

    // Get plugin name
    GetShapeNameFunc getNameFunc = (GetShapeNameFunc)DLSYM(handle, "getShapeName");
    if (!getNameFunc) {
        DLCLOSE(handle);
        return false;
    }

    wxString shapeName = getNameFunc();

    // Register the shape
    RegisterShapeFunc registerFunc = (RegisterShapeFunc)DLSYM(handle, "registerShape");
    if (registerFunc) {
        registerFunc();
    }

    PluginInfo info;
    info.name = shapeName;
    info.handle = handle;
    info.registerFunc = registerFunc;

    m_plugins.push_back(info);

    wxLogMessage("Loaded plugin: %s", shapeName);
    return true;
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
        DLCLOSE(plugin.handle);
        plugin.handle = nullptr;
    }
}