// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_ACTIONS_UNDO_REDO_STACK_H
#define VCL_RENDER_ACTIONS_UNDO_REDO_STACK_H

#include <memory>
#include <vector>

#include "action.h"

namespace vcl {

/**
 * @brief Manages the history of undoable actions.
 *
 * The stack holds two separate containers for the undo and redo history.
 * Pushing a new action clears the redo history.
 */
class UndoRedoStack
{
    std::vector<std::unique_ptr<UndoRedoAction>> mUndoStack;
    std::vector<std::unique_ptr<UndoRedoAction>> mRedoStack;

public:
    UndoRedoStack() = default;

    /**
     * @brief Pushes a new action onto the undo stack and clears the redo stack.
     * @param[in] action: The action to push.
     */
    void pushAction(std::unique_ptr<UndoRedoAction> action)
    {
        if (action) {
            mUndoStack.push_back(std::move(action));
            mRedoStack.clear();
        }
    }

    /**
     * @brief Undoes the top action on the undo stack and pushes it to the redo
     * stack.
     * @return true if an action was undone, false if the undo stack was empty.
     */
    bool undo()
    {
        if (mUndoStack.empty())
            return false;

        auto action = std::move(mUndoStack.back());
        mUndoStack.pop_back();
        action->undo();
        mRedoStack.push_back(std::move(action));
        return true;
    }

    /**
     * @brief Redoes the top action on the redo stack and pushes it to the undo
     * stack.
     * @return true if an action was redone, false if the redo stack was empty.
     */
    bool redo()
    {
        if (mRedoStack.empty())
            return false;

        auto action = std::move(mRedoStack.back());
        mRedoStack.pop_back();
        action->redo();
        mUndoStack.push_back(std::move(action));
        return true;
    }

    /**
     * @brief Clears both the undo and redo stacks.
     */
    void clear()
    {
        mUndoStack.clear();
        mRedoStack.clear();
    }

    /**
     * @brief Checks if there is any action that can be undone.
     * @return true if the undo stack is not empty, false otherwise.
     */
    bool canUndo() const { return !mUndoStack.empty(); }

    /**
     * @brief Checks if there is any action that can be redone.
     * @return true if the redo stack is not empty, false otherwise.
     */
    bool canRedo() const { return !mRedoStack.empty(); }
};

} // namespace vcl

#endif // VCL_RENDER_ACTIONS_UNDO_REDO_STACK_H
