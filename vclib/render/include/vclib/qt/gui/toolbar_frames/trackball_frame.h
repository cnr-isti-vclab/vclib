/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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

        connect(
            editorButton,
            &QPushButton::clicked,
            this,
            [this]() {
                mViewer.toggleTrackBallVisibility();
                mViewer.update();
            });
    }

};

} // namespace vcl::qt

#endif // VCL_QT_GUI_TOOLBAR_FRAMES_TRACKBALL_FRAME_H
