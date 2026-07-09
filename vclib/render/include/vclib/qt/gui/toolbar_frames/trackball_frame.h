// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_GUI_TOOLBAR_FRAMES_TRACKBALL_FRAME_H
#define VCL_QT_GUI_TOOLBAR_FRAMES_TRACKBALL_FRAME_H

#include "generic_editor_frame.h"

namespace vcl::qt {

template<typename ViewerType>
class TrackBallFrame : public GenericEditorFrame
{
    using Base = GenericEditorFrame;

    ViewerType& mViewer;

public:
    explicit TrackBallFrame(ViewerType& viewer, QWidget* parent = nullptr) :
            GenericEditorFrame(parent), mViewer(viewer)
    {
        QIcon ic(":/icons/trackball.png");

        QPushButton* editorButton = Base::addButton(ic);
        editorButton->setChecked(mViewer.isTrackBallVisible());
        editorButton->setToolTip("Show TrackBall");

        mViewer.setShortcutToggleTrackballCallback([editorButton]() {
            editorButton->click();
        });

        Base::hideSettingsButton();

        connect(editorButton, &QPushButton::clicked, this, [this]() {
            mViewer.toggleTrackBallVisibility();
            mViewer.update();
        });
    }
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_TRACKBALL_FRAME_H
