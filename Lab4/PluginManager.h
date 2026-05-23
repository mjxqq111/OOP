#pragma once

#include <wx/wx.h>
#include <vector>
#include <functional>
#include <memory>

// Plugin function types
typedef void (*RegisterShapeFunc)(void);
typedef const char* (*GetShapeNameFunc)(void);

// Plugin information structure
struct PluginInfo {
    wxString name;                  // Plugin name
    void* handle;                   // DLL handle
    RegisterShapeFunc registerFunc; // Function to register shapes
};

// Manages dynamic loading of shape plugins
class PluginManager {
public:
    // Get singleton instance
    static PluginManager& getInstance();

    // Load all plugins from plugins directory
    void loadPlugins(const wxString& pluginDir = "plugins");

    // Unload all plugins
    void unloadPlugins();

    // Get list of loaded plugins
    const std::vector<PluginInfo>& getPlugins() const { return m_plugins; }

private:
    PluginManager() = default;
    ~PluginManager();

    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    // Load a single plugin DLL
    bool loadPlugin(const wxString& dllPath);

    // Unload a single plugin
    void unloadPlugin(PluginInfo& plugin);

    std::vector<PluginInfo> m_plugins;
};