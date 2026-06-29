// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_OPENGL2_DRAWERS_TRACKBALL_VIEWER_DRAWER_OPENGL2_H
#define VCL_OPENGL2_DRAWERS_TRACKBALL_VIEWER_DRAWER_OPENGL2_H

#include "viewer_drawer_opengl2.h"

#include <vclib/render/drawers/trackball_event_drawer.h>

namespace vcl {

template<typename DerivedRenderApp>
class TrackBallViewerDrawerOpenGL2 :
        public ViewerDrawerOpenGL2<TrackBallEventDrawer<DerivedRenderApp>>
{
    using ParentViewer =
        ViewerDrawerOpenGL2<TrackBallEventDrawer<DerivedRenderApp>>;

public:
    using ParentViewer::ParentViewer;

    bool isTrackBallVisible() const { return false; }

    void toggleTrackBallVisibility() {}

    void setShortcutToggleTrackballCallback(std::function<void(void)> callback)
    {
    }
};

} // namespace vcl

#endif // VCL_OPENGL2_DRAWERS_TRACKBALL_VIEWER_DRAWER_OPENGL2_H
