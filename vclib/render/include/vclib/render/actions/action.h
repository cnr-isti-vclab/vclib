// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_ACTIONS_ACTION_H
#define VCL_RENDER_ACTIONS_ACTION_H

#include <string>

namespace vcl {

/**
 * @brief Base class for all undoable/redoable actions.
 *
 * Implements the Command pattern. An action encapsulates all the information
 * needed to perform, undo, and redo a specific modification to the document or
 * scene state.
 */
class UndoRedoAction
{
public:
    virtual ~UndoRedoAction() = default;

    /**
     * @brief Reverts the changes made by this action.
     */
    virtual void undo() = 0;

    /**
     * @brief Re-applies the changes made by this action.
     */
    virtual void redo() = 0;

    /**
     * @brief Returns a human-readable name for this action.
     * @return The name of the action.
     */
    virtual std::string name() const { return "Unknown Action"; }
};

} // namespace vcl

#endif // VCL_RENDER_ACTIONS_ACTION_H
