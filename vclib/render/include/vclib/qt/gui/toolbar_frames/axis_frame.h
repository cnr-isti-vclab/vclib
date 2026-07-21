// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_AXIS_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_AXIS_FRAME_H

#include "generic_editor_frame.h"

namespace vcl::qt {

template<typename ViewerType>
class AxisFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    ViewerType& mViewer;

public:
    explicit AxisFrame(ViewerType& viewer, QWidget* parent = nullptr) :
            GenericEditorFrame(parent), mViewer(viewer)
    {
        QIcon ic(":/icons/show_axis.png");

        QPushButton* editorButton = Base::addButton(ic);

        editorButton->setToolTip("Show Axis");

        Base::hideSettingsButton();

        mViewer.setShortcutToggleAxisCallback([editorButton]() {
            editorButton->click();
        });

        connect(editorButton, &QPushButton::clicked, this, [this]() {
            mViewer.toggleAxisVisibility();
        });
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_AXIS_FRAME_H
