#pragma once

#include "Command.h"
#include "EditorCanvas.h"
#include <algorithm>

class AddFigureCommand : public UndoableCommand {
private:
    EditorCanvas* m_canvas;                     // Pointer to canvas
    std::shared_ptr<fig::Figure> m_figure;      // Figure to add

public:
    // Constructor (stores canvas and figure)
    AddFigureCommand(EditorCanvas* canvas, std::shared_ptr<fig::Figure> figure)
        : m_canvas(canvas), m_figure(figure) {
    }

    // Add figure to canvas
    void execute() override {
        m_canvas->getFigures().push_back(m_figure);
        m_canvas->refreshCanvas();
    }

    // Remove figure from canvas (undo operation)
    void undo() override {
        auto& figures = m_canvas->getFigures();
        auto it = std::find(figures.begin(), figures.end(), m_figure);
        if (it != figures.end()) {
            figures.erase(it);
        }
        m_canvas->refreshCanvas();
    }
};