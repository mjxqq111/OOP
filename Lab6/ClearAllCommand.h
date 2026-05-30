#pragma once

#include "Command.h"
#include "EditorCanvas.h"

class ClearAllCommand : public UndoableCommand {
private:
    EditorCanvas* m_canvas;                            // Pointer to canvas
    std::vector<std::shared_ptr<fig::Figure>> m_oldFigures;  // Saved figures for undo

public:
    // Constructor (saves current figures for undo)
    ClearAllCommand(EditorCanvas* canvas)
        : m_canvas(canvas) {
        m_oldFigures = m_canvas->getFigures();
    }

    // Clear all figures from canvas
    void execute() override {
        m_canvas->getFigures().clear();
        m_canvas->refreshCanvas();
    }

    // Restore all saved figures (undo operation)
    void undo() override {
        m_canvas->getFigures() = m_oldFigures;
        m_canvas->refreshCanvas();
    }
};