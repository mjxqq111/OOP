#pragma once

#include <wx/wx.h>
#include <vector>
#include <memory>
#include "Figure.hpp"

// Mouse-controlled drawing area
class EditorCanvas : public wxPanel {
public:
    EditorCanvas(wxWindow* parent, wxWindowID id = wxID_ANY);   // Constructor

    void setCurrentShapeType(const wxString& type);  // Set active shape type
    void clearAll();                                  // Clear all shapes

    std::vector<std::shared_ptr<fig::Figure>>& getFigures();    // Access to figures (for plugins) 
    void refreshCanvas();       // Refreshes the canvas

    void setDrawCallback(void (*callback)(wxDC&, void*), void* data);   // Set plugin draw callback
    void clearDrawCallback();   // Clear plugin draw callback

    // Check if currently drawing a shape
    bool isDragging() const;

protected:
    void onPaint(wxPaintEvent& evt);         // Handle paint events
    void onMouseDown(wxMouseEvent& evt);     // Handle mouse press
    void onMouseUp(wxMouseEvent& evt);       // Handle mouse release
    void onMouseMove(wxMouseEvent& evt);     // Handle mouse drag

private:
    void addFigure(std::shared_ptr<fig::Figure> figure);  // Add shape to collection

    std::vector<std::shared_ptr<fig::Figure>> m_figures;  // All completed shapes
    std::shared_ptr<fig::Figure> m_currentFigure;         // Shape being drawn
    wxString m_currentType;                               // Currently selected shape
    wxPoint m_dragStart;                                  // Mouse drag start position
    bool m_dragging;                                      // Is user dragging?

    // Plugin draw callback
    void (*m_drawCallback)(wxDC&, void*);
    void* m_drawData;
};