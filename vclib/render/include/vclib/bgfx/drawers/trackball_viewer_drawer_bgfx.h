// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWERS_TRACKBALL_VIEWER_DRAWER_BGFX_H
#define VCL_BGFX_DRAWERS_TRACKBALL_VIEWER_DRAWER_BGFX_H

#include "viewer_drawer_bgfx.h"

#include <vclib/bgfx/drawable/drawable_directional_light.h>
#include <vclib/bgfx/drawable/drawable_trackball.h>
#include <vclib/render/drawers/trackball_event_drawer.h>

namespace vcl {

template<typename DerivedRenderApp>
class TrackBallViewerDrawerBGFX :
        public ViewerDrawerBGFX<TrackBallEventDrawer<DerivedRenderApp>>
{
    using ParentViewer =
        ViewerDrawerBGFX<TrackBallEventDrawer<DerivedRenderApp>>;

    DrawableTrackBall        mDrawTrackBall;
    DrawableDirectionalLight mDrawableDirectionalLight;

    std::function<void(void)> mCustomShortcutToggleTrackballCallback =
        [this]() {
            toggleTrackBallVisibility();
        };

public:
    using ParentViewer::ParentViewer;

    void onInit(uint viewId) override
    {
        ParentViewer::onInit(viewId);
        mDrawTrackBall.init();
        mDrawableDirectionalLight.init();
    }

    void onDraw(uint viewId) override
    {
        DrawObjectSettings settings;
        settings.viewId = viewId;

        ParentViewer::onDraw(viewId);

        setDirectionalLightVisibility(
            ParentViewer::currentMotion() ==
            ParentViewer::TrackBallType::DIR_LIGHT_ARC);

        if (mDrawTrackBall.isVisible()) {
            mDrawTrackBall.draw(settings);
        }

        if (mDrawableDirectionalLight.isVisible()) {
            mDrawableDirectionalLight.draw(settings);
        }
    }

    void onDrawContent(uint viewId) override
    {
        ParentViewer::onDrawContent(viewId);

        updateDrawableTrackball();
        updateDrawableDirectionalLight();
    }

    // events
    bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        bool block = ParentViewer::onKeyPress(key, modifiers);
        if (!block) {
            switch (key) {
            case Key::T:
                if (modifiers[KeyModifier::NO_MODIFIER])
                    mCustomShortcutToggleTrackballCallback();
                break;
            default: break;
            }
        }
        return block;
    }

    bool isTrackBallVisible() const { return mDrawTrackBall.isVisible(); }

    void toggleTrackBallVisibility()
    {
        mDrawTrackBall.setVisibility(!mDrawTrackBall.isVisible());
    }

    /**
     * @brief Sets the callback function that will be called when the user
     * presses the shortcut to toggle the trackball visibility (by default, the
     * shortcut is T).
     *
     * This is useful when the user wants to execute some custom code when the
     * trackball visibility is toggled trough the shortcut.
     *
     * @param callback
     */
    void setShortcutToggleTrackballCallback(std::function<void(void)> callback)
    {
        mCustomShortcutToggleTrackballCallback = callback;
    }

private:
    void updateDrawableTrackball()
    {
        auto v = ParentViewer::gizmoMatrix();
        mDrawTrackBall.setTransform(v);
        mDrawTrackBall.updateDragging(ParentViewer::isDragging());
    }

    bool isDirectionalLightVisible() const
    {
        return mDrawableDirectionalLight.isVisible();
    }

    void setDirectionalLightVisibility(bool b)
    {
        mDrawableDirectionalLight.setVisibility(b);
    }

    void updateDrawableDirectionalLight()
    {
        auto v = ParentViewer::lightGizmoMatrix();
        mDrawableDirectionalLight.updateRotation(v);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_TRACKBALL_VIEWER_DRAWER_BGFX_H
