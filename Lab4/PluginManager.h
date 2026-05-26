#pragma once

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <vector>

// Forward declaration
class EditorCanvas;

// Plugin information structure
struct PluginInfo {
    wxString name;          // Plugin name
    void* handle;           // DLL handle
    void* initUIFunc;       // Pointer to initUI function
};

// Manages dynamic loading of shape plugins
class PluginManager {
public:
    // Get singleton instance
    static PluginManager& getInstance();

    // Load a single plugin by file path
    bool loadPluginFromFile(const wxString& dllPath);

    // Unload all plugins
    void unloadPlugins();

    // Initialize UI for all plugins that have initUI function
    void initPluginUIs(wxWindow* parent, EditorCanvas* canvas);

    // Get list of loaded plugins
    const std::vector<PluginInfo>& getPlugins() const { return m_plugins; }

private:
    PluginManager() = default;
    ~PluginManager();

    // Disable copy
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    // Unload a single plugin
    void unloadPlugin(PluginInfo& plugin);

    std::vector<PluginInfo> m_plugins;
};