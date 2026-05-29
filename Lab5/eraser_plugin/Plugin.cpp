#include <wx/wx.h>
#include <wx/checkbox.h>
#include "../EditorCanvas.h"
#include "EraserMark.h"
#include <cmath>

#define EXPORT __declspec(dllexport)

// Eraser state
struct EraserState {
    bool enabled;
    wxPoint position;
    double radius;
    wxCheckBox* checkbox;
    EditorCanvas* canvas;
};

static EraserState g_state = {
    false,
    wxPoint(0, 0),
    20.0,
    nullptr,
    nullptr
};

// Drawing function for eraser mark
static void drawEraserMark(wxDC& dc, const fig::Figure& figure) {
    const fig::EraserMark& mark = static_cast<const fig::EraserMark&>(figure);

    int x = (int)(mark.getCenter().x - mark.getRadius());
    int y = (int)(mark.getCenter().y - mark.getRadius());
    int size = (int)(mark.getRadius() * 2);

    if (size <= 0) return;

    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawEllipse(x, y, size, size);
}

// Draw white circle with black outline (eraser preview)
static void drawEraserPreview(wxDC& dc, void* data) {
    EraserState* state = (EraserState*)data;

    if (!state->enabled) return;

    int x = (int)(state->position.x - state->radius);
    int y = (int)(state->position.y - state->radius);
    int size = (int)(state->radius * 2);

    if (size <= 0) return;

    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawEllipse(x, y, size, size);
}

// Adds eraser mark as a figure
static void addEraserMark(const wxPoint& pos, double radius) {
    if (!g_state.canvas) return;

    // Create eraser mark as a figure
    auto mark = std::make_shared<fig::EraserMark>();
    mark->setStart(Point(pos.x, pos.y));
    mark->setEnd(Point(pos.x + radius, pos.y + radius));

    // Add to canvas figures
    g_state.canvas->getFigures().push_back(mark);
    g_state.canvas->refreshCanvas();
}

// Adds line of eraser marks
static void addEraserLine(const wxPoint& from, const wxPoint& to, double radius) {
    if (!g_state.canvas) return;

    double distance = std::sqrt(std::pow(to.x - from.x, 2) + std::pow(to.y - from.y, 2));
    int steps = (int)(distance / (radius / 2)) + 1;

    for (int i = 0; i <= steps; ++i) {
        double t = (double)i / steps;
        wxPoint pos(
            (int)(from.x + t * (to.x - from.x)),
            (int)(from.y + t * (to.y - from.y))
        );

        auto mark = std::make_shared<fig::EraserMark>();
        mark->setStart(Point(pos.x, pos.y));
        mark->setEnd(Point(pos.x + radius, pos.y + radius));

        g_state.canvas->getFigures().push_back(mark);
    }

    g_state.canvas->refreshCanvas();
}

// Mouse down event handler
static void onMouseDown(wxMouseEvent& event) {
    if (!g_state.enabled) {
        event.Skip();
        return;
    }

    g_state.position = event.GetPosition();
    addEraserMark(g_state.position, g_state.radius);
}

// Mouse up event handler
static void onMouseUp(wxMouseEvent& event) {
    if (!g_state.enabled) {
        event.Skip();
        return;
    }

    g_state.canvas->refreshCanvas();
}

// Mouse move event handler
static void onMouseMove(wxMouseEvent& event) {
    if (!g_state.enabled) {
        event.Skip();
        return;
    }

    static wxPoint lastPos = event.GetPosition();
    wxPoint currentPos = event.GetPosition();

    if (event.LeftIsDown()) {
        addEraserLine(lastPos, currentPos, g_state.radius);
    }

    g_state.position = currentPos;
    lastPos = currentPos;
    g_state.canvas->refreshCanvas();
}

// Checkbox handler
static void onCheckboxToggled(wxCommandEvent& event) {
    g_state.enabled = g_state.checkbox->IsChecked();
    g_state.canvas->refreshCanvas();
}

// Adds checkbox to toolbar
static void addCheckboxToUI(wxWindow* controlPanel) {
    if (!controlPanel) return;

    g_state.checkbox = new wxCheckBox(controlPanel, wxID_ANY, "Eraser Mode");

    wxSizer* sizer = controlPanel->GetSizer();
    if (sizer) {
        sizer->Add(g_state.checkbox, 0, wxALIGN_CENTER | wxALL, 5);
        sizer->Layout();
    }
    else {
        return;
    }

    g_state.checkbox->Bind(wxEVT_CHECKBOX, onCheckboxToggled);
    controlPanel->Layout();
}

// Plugin initialization
extern "C" EXPORT void initPlugin(void*, void* bridge_register_draw) {
    ((void(*)(const char*, void*))bridge_register_draw)("EraserMark", drawEraserMark);
}

// Adding elements to UI
extern "C" EXPORT void initUI(void* parentWindow, void* canvas) {
    g_state.canvas = (EditorCanvas*)canvas;
    if (!g_state.canvas) return;

    // Set draw callback for eraser preview
    g_state.canvas->setDrawCallback(drawEraserPreview, &g_state);

    // Add checkbox to toolbar
    addCheckboxToUI((wxWindow*)parentWindow);

    // Bind mouse events to canvas
    g_state.canvas->Bind(wxEVT_LEFT_DOWN, onMouseDown);
    g_state.canvas->Bind(wxEVT_LEFT_UP, onMouseUp);
    g_state.canvas->Bind(wxEVT_MOTION, onMouseMove);
}

// Getting the name of the plugin
extern "C" EXPORT const char* getPluginName() {
    return "Eraser";
}