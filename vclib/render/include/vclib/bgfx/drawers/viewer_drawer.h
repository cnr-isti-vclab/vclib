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

#ifndef VCL_BGFX_DRAWERS_VIEWER_DRAWER_H
#define VCL_BGFX_DRAWERS_VIEWER_DRAWER_H

#include <vclib/render/drawers/abstract_viewer_drawer.h>

#include <vclib/bgfx/drawable/drawable_axis.h>
#include <vclib/bgfx/drawable/drawable_directional_light.h>
#include <vclib/bgfx/drawable/drawable_trackball.h>
#include <vclib/bgfx/drawable/uniforms/camera_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>

namespace vcl {

template<typename DerivedRenderApp>
class ViewerDrawerBGFX : public AbstractViewerDrawer<DerivedRenderApp>
{
    using ParentViewer = AbstractViewerDrawer<DerivedRenderApp>;
    using DTB          = ParentViewer::DTB;

    CameraUniforms           mCameraUniforms;
    DirectionalLightUniforms mDirectionalLightUniforms;

    DrawableAxis             mAxis;
    DrawableDirectionalLight mDirectionalLight;
    DrawableTrackBall        mDrawTrackBall;

    // flags
    bool mStatsEnabled = false;

public:
    ViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
        mCameraUniforms.updateCamera(DTB::camera());
        mDirectionalLightUniforms.updateLight(DTB::light());
    }

    ViewerDrawerBGFX(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width = 1024,
        uint height = 768) : ViewerDrawerBGFX(width, height)
    {
        ParentViewer::setDrawableObjectVector(v);
    }

    void onInit(uint viewId) override
    {
        ParentViewer::onInit(viewId);
        mAxis.init();
        mDirectionalLight.init();
        mDrawTrackBall.init();
    }

    void onDraw(uint viewId) override
    {
        onDrawContent(viewId);

        if (mAxis.isVisible()) {
            mAxis.draw(viewId);
        }

        if (mDirectionalLight.isVisible()) {
            mDirectionalLight.draw(viewId);
        }

        if (mDrawTrackBall.isVisible()) {
            mDrawTrackBall.draw(viewId);
        }
    }

    void onDrawContent(uint viewId) override
    {
        setDirectionalLightVisibility(
            DTB::currentMotion() == DTB::TrackBallType::DIR_LIGHT_ARC);
        updateDirectionalLight();
        updateDrawableTrackball();

        bgfx::setViewTransform(
            viewId, DTB::viewMatrix().data(), DTB::projectionMatrix().data());

        mCameraUniforms.updateCamera(DTB::camera());
        mCameraUniforms.bind();

        mDirectionalLightUniforms.bind();

        ParentViewer::drawableObjectVector().draw(viewId);
    }

    void onDrawId(uint viewId) override
    {
        bgfx::setViewTransform(
            viewId, DTB::viewMatrix().data(), DTB::projectionMatrix().data());

        mCameraUniforms.updateCamera(DTB::camera());
        mCameraUniforms.bind();

        ParentViewer::drawableObjectVector().drawId(viewId, ParentViewer::id());
    }

    void onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        if (key == Key::F1) {
            if (mStatsEnabled) {
                mStatsEnabled = false;
                bgfx::setDebug(BGFX_DEBUG_NONE);
            }
            else {
                mStatsEnabled = true;
                bgfx::setDebug(BGFX_DEBUG_STATS);
            }
        }
        ParentViewer::onKeyPress(key, modifiers);
    }

    void onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        ParentViewer::onMouseDoubleClick(button, x, y, modifiers);
        
        if (button == MouseButton::LEFT) {
            const bool homogeneousNDC =
                Context::instance().capabilites().homogeneousDepth;

            ParentViewer::readDepthRequest(x, y, homogeneousNDC);
        }
    }

    void toggleAxisVisibility() override
    {
        mAxis.setVisibility(!mAxis.isVisible());
    }

    void toggleTrackBallVisibility() override
    {
        mDrawTrackBall.setVisibility(!mDrawTrackBall.isVisible());
    }

private:
    bool isDirectionalLightVisible() const
    {
        return mDirectionalLight.isVisible();
    }

    void setDirectionalLightVisibility(bool b)
    {
        mDirectionalLight.setVisibility(b);
    }

    void updateDirectionalLight()
    {
        auto v = DTB::lightGizmoMatrix();
        mDirectionalLight.updateRotation(v);
        mDirectionalLightUniforms.updateLight(DTB::light());
    }

    void updateDrawableTrackball()
    {
        auto v = DTB::gizmoMatrix();
        mDrawTrackBall.setTransform(v);
        mDrawTrackBall.updateDragging(DTB::isDragging());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_H
