#include "EditorCanvas.h"
#include "ShapeFactory.h"
#include "ShapeRenderer.h"
#include <wx/dcbuffer.h>

// Constructor for canvas
EditorCanvas::EditorCanvas(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
    m_dragging(false),
    m_currentType("Rectangle"),
    m_drawCallback(nullptr),
    m_drawData(nullptr)
{
    SetDoubleBuffered(true);
    SetBackgroundColour(*wxWHITE);

    // Binding events
    Bind(wxEVT_PAINT, &EditorCanvas::onPaint, this);
    Bind(wxEVT_LEFT_DOWN, &EditorCanvas::onMouseDown, this);
    Bind(wxEVT_LEFT_UP, &EditorCanvas::onMouseUp, this);
    Bind(wxEVT_MOTION, &EditorCanvas::onMouseMove, this);
}

// Choosing the shape type
void EditorCanvas::setCurrentShapeType(const wxString& type) {
    m_currentType = type;
}

// Clearing the canvas
void EditorCanvas::clearAll() {
    m_figures.clear();
    m_currentFigure.reset();
    m_dragging = false;
    Refresh();
}

// Returns list of all figures on a canvas (for plugins)
std::vector<std::shared_ptr<fig::Figure>>& EditorCanvas::getFigures() { 
    return m_figures;
}

// Refreshes the canvas
void EditorCanvas::refreshCanvas() {
    Refresh();
}

// Adding a figure to canvas
void EditorCanvas::addFigure(std::shared_ptr<fig::Figure> figure) {
    m_figures.push_back(figure);
    Refresh();
}

// Setting draw callback
void EditorCanvas::setDrawCallback(void (*callback)(wxDC&, void*), void* data) {
    m_drawCallback = callback;
    m_drawData = data;
}

// Clearing draw callback
void EditorCanvas::clearDrawCallback() {
    m_drawCallback = nullptr;
    m_drawData = nullptr;
    Refresh();
}

// Is user dragging? (true/false)
bool EditorCanvas::isDragging() const {
    return m_dragging;
}

// Handle paint event (draw all completed shapes and the current temporary shape)
void EditorCanvas::onPaint(wxPaintEvent& WXUNUSED(evt)) {
    wxBufferedPaintDC dc(this);
    dc.Clear();

    ShapeRenderer renderer;

    // Draw all completed figures
    for (auto& figure : m_figures) {
        renderer.render(dc, figure);
    }

    // Draw custom plugin overlay if callback is set
    if (m_drawCallback && m_drawData) {
        m_drawCallback(dc, m_drawData);
    }

    // Draw temporary figure while dragging (preview)
    if (m_dragging && m_currentFigure) {
        renderer.render(dc, m_currentFigure);
    }

}

// Handle mouse down (start creating a new shape)
void EditorCanvas::onMouseDown(wxMouseEvent& evt) {
    wxPoint pos = evt.GetPosition();
    m_dragStart = pos;

    // Create temporary figure for preview
    m_currentFigure = ShapeFactory::createShape(m_currentType);

    if (m_currentFigure) {
        m_currentFigure->setStart(Point(pos.x, pos.y));
        m_currentFigure->setEnd(Point(pos.x, pos.y));
        m_dragging = true;
        Refresh();
    }
}

// Handle mouse up
void EditorCanvas::onMouseUp(wxMouseEvent& evt) {
    if (m_dragging && m_currentFigure) {
        wxPoint endPos = evt.GetPosition();
        m_currentFigure->setEnd(Point(endPos.x, endPos.y));

        Point start = m_currentFigure->getStart();
        Point end = m_currentFigure->getEnd();

        // Only add if shape has non-zero size
        if (start.x != end.x || start.y != end.y) {
            addFigure(m_currentFigure);
        }

        m_currentFigure.reset();
        m_dragging = false;
        Refresh();
    }
}

// Handling mouse movement
void EditorCanvas::onMouseMove(wxMouseEvent& evt) {
    if (m_dragging && m_currentFigure) {
        wxPoint pos = evt.GetPosition();
        m_currentFigure->setEnd(Point(pos.x, pos.y));
        Refresh();
    }
}