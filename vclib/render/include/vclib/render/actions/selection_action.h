// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_ACTIONS_SELECTION_ACTION_H
#define VCL_RENDER_ACTIONS_SELECTION_ACTION_H

#include <vclib/render/actions/action.h>
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
    struct MeshSelectionState {
        std::weak_ptr<vcl::DrawableObject> obj;
        vcl::BitVector<true>               vertexSelection;
        vcl::BitVector<true>               faceSelection;
    };

private:
    std::vector<MeshSelectionState> mStates;

public:
    SelectionUndoRedoAction(std::vector<MeshSelectionState> states) :
            mStates(std::move(states))
    {
    }

    void undo() override { swapState(); }

    void redo() override { swapState(); }

    std::string name() const override { return "Selection Changed"; }

private:
    void swapState()
    {
        for (auto& state : mStates) {
            if (auto lock = state.obj.lock()) {
                if (auto* mesh =
                    dynamic_cast<AbstractDrawableMesh*>(lock.get())) {
                    if (!state.vertexSelection.empty()) {
                        auto current = mesh->vertexSelectionBitVector();
                        mesh->setVertexSelectionBitVector(
                            state.vertexSelection);
                        state.vertexSelection = std::move(current);
                    }
                    if (!state.faceSelection.empty()) {
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

#endif // VCL_RENDER_ACTIONS_SELECTION_ACTION_H
