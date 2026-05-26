#include "PluginManager.h"

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

// Function types
typedef void (*InitUIFunc)(void*, void*);
typedef const char* (*GetToolNameFunc)(void);

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
    void* tempHandle = DLOPEN(dllPath.c_str());
    if (!tempHandle) {
        wxLogError("Failed to open DLL: %s", dllPath);
        return false;
    }

    // Trying to get tool name for tool plugins
    GetToolNameFunc getToolName = reinterpret_cast<GetToolNameFunc>(DLSYM(tempHandle, "getToolName"));

    wxString pluginName;
    bool isToolPlugin = (getToolName != nullptr);

    if (isToolPlugin) {
        pluginName = getToolName();
    }
    else {
        wxLogError("Plugin has no getToolName: %s", dllPath);
        DLCLOSE(tempHandle);
        return false;
    }

    // Checking if plugin with this name is already loaded
    for (const auto& plugin : m_plugins) {
        if (plugin.name == pluginName) {
            wxMessageBox("Plugin '" + pluginName + "' already loaded!",
                "Plugin Error", wxOK | wxICON_WARNING);
            DLCLOSE(tempHandle);
            return false;
        }
    }

    // Closing temporary handle and load DLL for real
    DLCLOSE(tempHandle);

    void* handle = DLOPEN(dllPath.c_str());
    if (!handle) {
        wxLogError("Failed to load DLL: %s", dllPath);
        return false;
    }

    // Storing plugin info
    PluginInfo info;
    info.name = pluginName;
    info.handle = handle;
    info.initUIFunc = reinterpret_cast<void*>(DLSYM(handle, "initUI"));

    m_plugins.push_back(info);

    wxLogMessage("Plugin loaded: %s (%s)", pluginName, dllPath);

    return true;
}

// Initialize UI for all plugins that have initUI function
void PluginManager::initPluginUIs(wxWindow* parent, EditorCanvas* canvas) {
    for (auto& plugin : m_plugins) {
        if (plugin.initUIFunc) {
            typedef void (*InitUIFuncType)(void*, void*);
            InitUIFuncType initUI = reinterpret_cast<InitUIFuncType>(plugin.initUIFunc);

            if (initUI) {
                initUI((void*)parent, (void*)canvas);
                wxLogMessage("UI initialized for plugin: %s", plugin.name);
            }
        }
    }
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