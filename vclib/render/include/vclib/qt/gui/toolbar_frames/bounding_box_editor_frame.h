// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_BOUNDING_BOX_EDITOR_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_BOUNDING_BOX_EDITOR_FRAME_H

#include "generic_editor_frame.h"
#include "settings/bounding_box_editor_settings_frame.h"

#include <vclib/render/editors/bounding_box_editor.h>

namespace vcl::qt {

template<typename ViewerType>
class BoundingBoxEditorFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    std::shared_ptr<vcl::BoundingBoxEditor<ViewerType>> mBoundingBoxEditor;

public:
    explicit BoundingBoxEditorFrame(
        std::shared_ptr<vcl::BoundingBoxEditor<ViewerType>> ptr,
        QWidget* parent = nullptr) : GenericEditorFrame(parent)
    {
        mBoundingBoxEditor = ptr;

        QIcon ic(":/icons/bbox.png");

        QPushButton* editorButton = Base::addButton(ic);

        editorButton->setToolTip("Show Bounding Box");

        connect(editorButton, &QPushButton::clicked, this, [this]() {
            if (mBoundingBoxEditor) {
                mBoundingBoxEditor->setActive(!mBoundingBoxEditor->isActive());
            }
        });

        BoundingBoxEditorSettingsFrame* sf =
            Base::setSettingsFrame<BoundingBoxEditorSettingsFrame>(
                mBoundingBoxEditor->settings());

        connect(sf, SIGNAL(settingsUpdated()), this, SLOT(refreshSettings()));
    }

private slots:

    void refreshSettings() override
    {
        if (mBoundingBoxEditor) {
            mBoundingBoxEditor->refreshSettings();
        }
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_BOUNDING_BOX_EDITOR_FRAME_H
