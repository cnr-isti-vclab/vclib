// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_NOT_EDITING_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_NOT_EDITING_FRAME_H

#include "generic_editor_frame.h"

namespace vcl::qt {

template<typename ViewerType>
class NotEditingFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    ViewerType& mViewer;

public:
    explicit NotEditingFrame(ViewerType& viewer, QWidget* parent = nullptr) :
            GenericEditorFrame(parent), mViewer(viewer)
    {
        QIcon ic(":/icons/not_editing.png");

        QPushButton* editorButton = Base::addButton(ic, true);
        editorButton->setToolTip("Not Editing Mode");
        editorButton->setChecked(!mViewer.isEditorsEventsEnabled());

        Base::hideSettingsButton();

        connect(editorButton, &QPushButton::toggled, this, [this](bool checked) {
            mViewer.setEditorsEventsEnabled(!checked);
        });

        mViewer.setOnEditorsEventsEnabledChangedCallback(
            [editorButton](bool enabled) {
                editorButton->setChecked(!enabled);
            });
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_NOT_EDITING_FRAME_H
