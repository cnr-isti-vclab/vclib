/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_BGFX_DRAWERS_TRACKBALL_VIEWER_DRAWER_BGFX_H
#define VCL_BGFX_DRAWERS_TRACKBALL_VIEWER_DRAWER_BGFX_H

#include "viewer_drawer.h"

#include <vclib/bgfx/drawable/drawable_axis.h>
#include <vclib/bgfx/drawable/drawable_trackball.h>
#include <vclib/render/drawers/trackball_event_drawer.h>

namespace vcl {

template<typename DerivedRenderApp>
class TrackBallViewerDrawerBGFX :
        public ViewerDrawerBGFX<TrackBallEventDrawer, DerivedRenderApp>
{
    using ParentViewer =
        ViewerDrawerBGFX<TrackBallEventDrawer, DerivedRenderApp>;

    DrawableAxis             mAxis;
    DrawableTrackBall        mDrawTrackBall;

public:
    using ParentViewer::ParentViewer;

    void onInit(uint viewId) override
    {
        ParentViewer::onInit(viewId);
        mAxis.init();
        mDrawTrackBall.init();
    }

    void onDraw(uint viewId) override
    {
        ParentViewer::onDraw(viewId);

        if (mAxis.isVisible()) {
            mAxis.draw(viewId);
        }

        if (mDrawTrackBall.isVisible()) {
            mDrawTrackBall.draw(viewId);
        }
    }

    void onDrawContent(uint viewId) override
    {
        ParentViewer::onDrawContent(viewId);

        updateDrawableTrackball();
    }

    // events
    void onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        ParentViewer::onKeyPress(key, modifiers);

        switch (key) {
        case Key::A: toggleAxisVisibility(); break;

        case Key::T: toggleTrackBallVisibility(); break;

        default: break;
        }
    }

    void toggleAxisVisibility()
    {
        mAxis.setVisibility(!mAxis.isVisible());
    }

    void toggleTrackBallVisibility()
    {
        mDrawTrackBall.setVisibility(!mDrawTrackBall.isVisible());
    }

private:
    void updateDrawableTrackball()
    {
        auto v = ParentViewer::gizmoMatrix();
        mDrawTrackBall.setTransform(v);
        mDrawTrackBall.updateDragging(ParentViewer::isDragging());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_TRACKBALL_VIEWER_DRAWER_BGFX_H
