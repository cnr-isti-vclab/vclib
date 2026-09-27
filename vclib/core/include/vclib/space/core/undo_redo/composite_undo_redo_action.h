// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_CORE_UNDO_REDO_COMPOSITE_UNDO_REDO_ACTION_H
#define VCL_SPACE_CORE_UNDO_REDO_COMPOSITE_UNDO_REDO_ACTION_H

#include <memory>
#include <string>
#include <vector>

#include "undo_redo_action.h"

namespace vcl {

/**
 * @brief Combines several UndoRedoAction objects into a single one.
 *
 * Implements the Composite pattern on top of UndoRedoAction: it allows
 * grouping multiple independently produced actions (e.g. one that reverts a
 * geometric modification and one that reverts a scene modification) into a
 * single entry of an UndoRedoStack, so that a single undo()/redo() call
 * reverts/reapplies all of them together.
 *
 * Sub-actions are undone in reverse order (the last one applied is the first
 * one undone) and redone in the original order, which is the correct
 * behavior when sub-actions depend on each other.
 */
class CompositeUndoRedoAction : public UndoRedoAction
{
    std::vector<std::unique_ptr<UndoRedoAction>> mActions;
    std::string                                  mName;

public:
    /**
     * @brief Creates an empty CompositeUndoRedoAction with the given name.
     * @param[in] name: The human-readable name of this composite action.
     */
    explicit CompositeUndoRedoAction(std::string name) : mName(std::move(name))
    {
    }

    /**
     * @brief Appends a sub-action to this composite.
     *
     * The action is appended at the end, i.e. it is considered the last one
     * applied: it will be the first one reverted by undo(), and the last one
     * reapplied by redo(). A null action is silently ignored.
     *
     * @param[in] action: The sub-action to append.
     */
    void addAction(std::unique_ptr<UndoRedoAction> action)
    {
        if (action)
            mActions.push_back(std::move(action));
    }

    /**
     * @brief Reverts all the sub-actions, in reverse order.
     */
    void undo() override
    {
        for (auto it = mActions.rbegin(); it != mActions.rend(); ++it)
            (*it)->undo();
    }

    /**
     * @brief Re-applies all the sub-actions, in the order they were added.
     */
    void redo() override
    {
        for (auto& action : mActions)
            action->redo();
    }

    /**
     * @brief Returns a human-readable name for this action.
     * @return The name of the action.
     */
    std::string name() const override { return mName; }
};

} // namespace vcl

#endif // VCL_SPACE_CORE_UNDO_REDO_COMPOSITE_UNDO_REDO_ACTION_H
