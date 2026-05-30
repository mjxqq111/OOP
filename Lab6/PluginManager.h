#pragma once

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <vector>

// Forward declaration
class EditorCanvas;

// Function types
typedef void (*InitPluginFunc)(void*, void*);
typedef void (*InitUIFunc)(void*, void*);
typedef const char* (*GetPluginNameFunc)(void);
typedef const char* (*HandleSaveFunc)(const wxString *);
typedef const char* (*HandleLoadFunc)(const wxString*);

// Plugin information structure
struct PluginInfo {
    wxString name;          // Plugin name
    void* handle = nullptr; // DLL handle
    InitUIFunc initUIFunc = nullptr;  // Pointer to initUI function
    HandleSaveFunc handleSaveFunc = nullptr;  // Pointer to handle_save function
    HandleLoadFunc handleLoadFunc = nullptr;  // Pointer to handle_load function
};

// Manages dynamic loading of plugins
class PluginManager {
public:
    // Get singleton instance
    static PluginManager& getInstance();

    // Load a single plugin by file path
    bool loadPluginFromFile(const wxString& dllPath);

    // Unload all plugins
    void unloadPlugins();

    // Initialize UI for all plugins that have initUI function
    void initPluginUI(const PluginInfo& pl, wxWindow* parent, EditorCanvas* canvas);

    // Get list of loaded plugins
    const std::vector<PluginInfo>& getPlugins() const;

private:
    // Constructor
    PluginManager() = default;

    // Destructor
    ~PluginManager();

    // Disable copy
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;    // No copying

    // Unload a single plugin
    void unloadPlugin(PluginInfo& plugin);

    // All plugins
    std::vector<PluginInfo> m_plugins;
};