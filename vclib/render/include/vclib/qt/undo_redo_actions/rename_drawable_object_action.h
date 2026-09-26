// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_UNDO_REDO_ACTIONS_RENAME_DRAWABLE_OBJECT_ACTION_H
#define VCL_QT_UNDO_REDO_ACTIONS_RENAME_DRAWABLE_OBJECT_ACTION_H

#include <vclib/qt/mesh_viewer.h>
#include <vclib/render/undo_redo/undo_redo_action.h>

namespace vcl::qt {

/**
 * @brief Action for undoing/redoing the renaming of a drawable object.
 */
class RenameDrawableObjectAction : public vcl::UndoRedoAction
{
    MeshViewer*                          mViewer;
    std::shared_ptr<vcl::DrawableObject> mObj;
    std::string                          mStateName;

public:
    RenameDrawableObjectAction(
        MeshViewer*                          viewer,
        std::shared_ptr<vcl::DrawableObject> obj,
        const std::string&                   oldName) :
            mViewer(viewer), mObj(obj), mStateName(oldName)
    {
    }

    void undo() override { swapState(); }

    void redo() override { swapState(); }

    std::string name() const override { return "Rename Drawable Object"; }

private:
    void swapState()
    {
        if (mObj && mViewer) {
            std::swap(mObj->name(), mStateName);
            mViewer->updateGUI();
        }
    }
};

} // namespace vcl::qt

#endif // VCL_QT_UNDO_REDO_ACTIONS_RENAME_DRAWABLE_OBJECT_ACTION_H
