#include "EditorCanvas.h"
#include "ShapeFactory.h"
#include "ShapeRenderer.h"

EditorCanvas::EditorCanvas(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
    m_dragging(false),
    m_currentType("Rectangle")
{
    SetBackgroundColour(*wxWHITE);

    Bind(wxEVT_PAINT, &EditorCanvas::onPaint, this);
    Bind(wxEVT_LEFT_DOWN, &EditorCanvas::onMouseDown, this);
    Bind(wxEVT_LEFT_UP, &EditorCanvas::onMouseUp, this);
    Bind(wxEVT_MOTION, &EditorCanvas::onMouseMove, this);
}

void EditorCanvas::setCurrentShapeType(const wxString& type) {
    m_currentType = type;
}

void EditorCanvas::clearAll() {
    m_figures.clear();
    Refresh();
}

void EditorCanvas::addFigure(std::shared_ptr<fig::Figure> figure) {
    m_figures.push_back(figure);
    Refresh();
}

// Handle paint event - draw all completed shapes and the current temporary shape
void EditorCanvas::onPaint(wxPaintEvent& WXUNUSED(evt)) {
    wxPaintDC dc(this);
    dc.Clear();

    ShapeRenderer renderer;

    for (auto& figure : m_figures) {
        renderer.render(dc, figure);
    }

    if (m_dragging && m_currentFigure) {
        renderer.render(dc, m_currentFigure);
    }
}

// Handle mouse down - start creating a new shape
void EditorCanvas::onMouseDown(wxMouseEvent& evt) {
    m_dragStart = evt.GetPosition();
    m_currentFigure = ShapeFactory::createShape(m_currentType);

    if (m_currentFigure) {
        m_currentFigure->setStart(Point(m_dragStart.x, m_dragStart.y));
        m_currentFigure->setEnd(Point(m_dragStart.x, m_dragStart.y));
        m_dragging = true;
    }
}

void EditorCanvas::onMouseUp(wxMouseEvent& evt) {
    if (m_dragging && m_currentFigure) {
        wxPoint endPos = evt.GetPosition();
        m_currentFigure->setEnd(Point(endPos.x, endPos.y));

        Point start = m_currentFigure->getStart();
        Point end = m_currentFigure->getEnd();

        if (start.x != end.x || start.y != end.y) {
            addFigure(m_currentFigure);
        }

        m_currentFigure.reset();
        m_dragging = false;
    }
}

void EditorCanvas::onMouseMove(wxMouseEvent& evt) {
    if (m_dragging && m_currentFigure) {
        wxPoint curPos = evt.GetPosition();
        m_currentFigure->setEnd(Point(curPos.x, curPos.y));
        Refresh();
    }
}