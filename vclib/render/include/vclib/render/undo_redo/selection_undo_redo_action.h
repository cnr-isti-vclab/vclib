// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_UNDO_REDO_SELECTION_UNDO_REDO_ACTION_H
#define VCL_RENDER_UNDO_REDO_SELECTION_UNDO_REDO_ACTION_H

#include "undo_redo_action.h"

#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/space/core.h>

#include <memory>
#include <vector>

namespace vcl {

/**
 * @brief An action that encapsulates a change in the selection state of a mesh.
 *
 * This action stores the previous and new selection state of a mesh, allowing
 * the user to undo and redo selection changes. It supports both vertex and face
 * selections.
 */
class SelectionUndoRedoAction : public UndoRedoAction
{
public:
    /**
     * @brief Represents the saved selection state of a single object.
     */
    struct MeshState
    {
        std::weak_ptr<vcl::DrawableObject> obj;
        vcl::BitVector<true>               vertexSelection;
        vcl::BitVector<true>               faceSelection;
    };

private:
    std::vector<MeshState> mStates;

public:
    /**
     * @brief Constructs a new selection action with the given pre-selection
     * states.
     * @param[in] states: The list of mesh states prior to the selection
     * operation.
     */
    SelectionUndoRedoAction(std::vector<MeshState> states) :
            mStates(std::move(states))
    {
    }

    /// @copydoc UndoRedoAction::undo()
    void undo() override { swapState(); }

    /// @copydoc UndoRedoAction::redo()
    void redo() override { swapState(); }

    /// @copydoc UndoRedoAction::name()
    std::string name() const override { return "Selection Changed"; }

private:
    void swapState()
    {
        for (auto& state : mStates) {
            if (auto lock = state.obj.lock()) {
                if (auto* mesh =
                        dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                    if (!state.vertexSelection.empty() ||
                        !mesh->vertexSelectionBitVector().empty()) {
                        auto current = mesh->vertexSelectionBitVector();
                        mesh->setVertexSelectionBitVector(
                            state.vertexSelection);
                        state.vertexSelection = std::move(current);
                    }
                    if (!state.faceSelection.empty() ||
                        !mesh->faceSelectionBitVector().empty()) {
                        auto current = mesh->faceSelectionBitVector();
                        mesh->setFaceSelectionBitVector(state.faceSelection);
                        state.faceSelection = std::move(current);
                    }
                }
            }
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_UNDO_REDO_SELECTION_UNDO_REDO_ACTION_H
