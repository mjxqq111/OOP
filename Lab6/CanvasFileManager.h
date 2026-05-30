#pragma once

#include <wx/wx.h>
#include <vector>
#include <memory>
#include "Figure.hpp"

class EditorCanvas;

class CanvasFileManager {
public:
    // Constructor
    CanvasFileManager();

    // Save canvas to file
    bool saveToFile(EditorCanvas* canvas, const wxString& filename);

    // Load canvas from file
    bool loadFromFile(EditorCanvas* canvas, const wxString& filename);

    // Handlers
    const char *(*saveHandler)(const wxString*) = nullptr;
    const char *(*loadHandler)(const wxString*) = nullptr;
};