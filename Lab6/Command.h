#pragma once

#include <memory>
#include <stack>
#include <vector>
#include <algorithm>

// Base command class
class Command {
public:
    virtual ~Command() = default;   // Constructor
    virtual void execute() = 0; // Execute command
};

// Command that can be undone
class UndoableCommand : public Command {
public:
    virtual void undo() = 0;    // Undo method
};

// History of commands
class CommandHistory {
private:
    std::stack<std::unique_ptr<UndoableCommand>> m_undoStack;
    std::stack<std::unique_ptr<UndoableCommand>> m_redoStack;

    CommandHistory() = default; // Constructor
    ~CommandHistory() = default; // Destructor

    CommandHistory(const CommandHistory&) = delete; // No copying
    CommandHistory& operator=(const CommandHistory&) = delete;  // Copying disallowed

public:
    // Returns an single instance
    static CommandHistory& getInstance() {
        static CommandHistory instance;
        return instance;
    }

    // Execute command
    void execute(std::unique_ptr<UndoableCommand> cmd) {
        cmd->execute();
        m_undoStack.push(std::move(cmd));
        while (!m_redoStack.empty()) m_redoStack.pop();
    }

    // Undo the last command
    void undo() {
        if (m_undoStack.empty()) return;
        auto cmd = std::move(m_undoStack.top());
        m_undoStack.pop();
        cmd->undo();
        m_redoStack.push(std::move(cmd));
    }

    // Redo the last undone command
    void redo() {
        if (m_redoStack.empty()) return;
        auto cmd = std::move(m_redoStack.top());
        m_redoStack.pop();
        cmd->execute();
        m_undoStack.push(std::move(cmd));
    }

    // Checks if undo can be done
    bool canUndo() const { return !m_undoStack.empty(); }

    // Checks if redo can be done
    bool canRedo() const { return !m_redoStack.empty(); }
};

// Undo Command (triggers undo in history)
class UndoCommand : public Command {
public:
    // Execute undo operation
    void execute() override {
        CommandHistory::getInstance().undo();
    }
};

// Redo Command (triggers redo in history)
class RedoCommand : public Command {
public:
    // Execute redo operation
    void execute() override {
        CommandHistory::getInstance().redo();
    }
};