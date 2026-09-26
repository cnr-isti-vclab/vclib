// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_UNDO_REDO_ACTIONS_DELETE_DRAWABLE_OBJECT_ACTION_H
#define VCL_QT_UNDO_REDO_ACTIONS_DELETE_DRAWABLE_OBJECT_ACTION_H

#include <vclib/qt/mesh_viewer.h>
#include <vclib/render/undo_redo/undo_redo_action.h>

namespace vcl::qt {

/**
 * @brief Action for undoing/redoing the deletion of a drawable object.
 */
class DeleteDrawableObjectAction : public vcl::UndoRedoAction
{
    MeshViewer*                          mViewer;
    uint                                 mIndex;
    std::shared_ptr<vcl::DrawableObject> mObj;
    bool                                 mIsDeleted = true;

public:
    DeleteDrawableObjectAction(
        MeshViewer*                          viewer,
        uint                                 index,
        std::shared_ptr<vcl::DrawableObject> obj) :
            mViewer(viewer), mIndex(index), mObj(std::move(obj))
    {
    }

    void undo() override { toggleState(); }

    void redo() override { toggleState(); }

    std::string name() const override { return "Delete Drawable Object"; }

private:
    void toggleState()
    {
        if (!mViewer || !mObj)
            return;

        if (mIsDeleted) {
            mViewer->insertDrawableObject(mIndex, mObj);
            mIsDeleted = false;
        }
        else {
            mViewer->removeDrawableObject(mIndex);
            mIsDeleted = true;
        }
    }
};

} // namespace vcl::qt

#endif // VCL_QT_UNDO_REDO_ACTIONS_DELETE_DRAWABLE_OBJECT_ACTION_H
