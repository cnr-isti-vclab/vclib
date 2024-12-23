/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/render/viewer/abstract_viewer.h>

#include <vclib/bgfx/drawable/drawable_axis.h>
#include <vclib/bgfx/drawable/drawable_directional_light.h>
#include <vclib/bgfx/drawable/drawable_trackball.h>
#include <vclib/bgfx/drawable/uniforms/camera_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>

namespace vcl {

class ViewerDrawerBGFX : public AbstractViewer
{
    CameraUniforms             mCameraUniforms;
    DirectionalLightUniforms   mDirectionalLightUniforms;
    MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

    DrawableAxis             mAxis;
    DrawableDirectionalLight mDirectionalLight;
    DrawableTrackBall        mDrawTrackBall;

public:
    ViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            AbstractViewer(width, height)
    {
        mCameraUniforms.updateCamera(DTB::camera());
        mDirectionalLightUniforms.updateLight(DTB::light());
    }

    ViewerDrawerBGFX(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width = 1024,
        uint height = 768) : ViewerDrawerBGFX(width, height)
    {
        setDrawableObjectVector(v);
    }

    void toggleAxisVisibility() override
    {
        mAxis.setVisibility(!mAxis.isVisible());
    }

    void toggleTrackBallVisibility() override
    {
        mDrawTrackBall.setVisibility(!mDrawTrackBall.isVisible());
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
            currentMotion() == DTB::TrackBallType::DIR_LIGHT_ARC);
        updateDirectionalLight();
        updateDrawableTrackball();

        bgfx::setViewTransform(
            viewId, viewMatrix().data(), projectionMatrix().data());

        mCameraUniforms.updateCamera(DTB::camera());
        mCameraUniforms.bind();

        mDirectionalLightUniforms.bind();

        for (auto obj : drawableObjectVector())
            obj->draw(viewId);
    }

    // events
    // void onMouseDoubleClick(
    //     MouseButton::Enum   button,
    //     double              x,
    //     double              y,
    //     const KeyModifiers& modifiers) override
    // {
    //     using ReadFramebufferRequest = detail::ReadFramebufferRequest;
    //     using CallbackReadBuffer     = ReadFramebufferRequest::CallbackReadBuffer;
    //     using ReadData               = ReadFramebufferRequest::ReadData;

    //     // FIXME: code duplication for both OpenGL2 and BGFX
    //     if (mReadRequested)
    //         return;

    //     // get point
    //     const Point2d p(x, y);

    //     // get the homogeneous NDC flag
    //     const bool homogeneousNDC =
    //         Context::instance().capabilites().homogeneousDepth;

    //     // matrices
    //     const auto proj = projectionMatrix();
    //     const auto view = viewMatrix();
    //     // viewport
    //     const Point4f vp = {.0f, .0f, float(size().x()), float(size().y())};
    //     // create the callback
    //     auto callback = [=, this](const ReadData& data) {
    //         assert(std::holds_alternative<std::vector<float>>(data));
    //         const auto& d  = std::get<std::vector<float>>(data);
    //         mReadRequested = false;

    //         // if the depth is 1.0, the point is not in the scene
    //         const float depth = d[0];
    //         if (depth == 1.0f) {
    //             return;
    //         }

    //         // unproject the point
    //         const Point3f p2d(p.x(), vp[3] - p.y(), depth);
    //         const auto    unproj = unproject(
    //             p2d, Matrix44<ScalarType>(proj * view), vp, homogeneousNDC);

    //         this->focus(unproj);
    //         // this->update(); TODO
    //     };

    //     mReadRequested = this->readDepth(Point2i(p.x(), p.y()), callback);
    //     if (mReadRequested)
    //         update();
    // }

private:
    bool mReadRequested = false;

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
        mDirectionalLightUniforms.updateLight(light());
    }

    void updateDrawableTrackball()
    {
        auto v = DTB::gizmoMatrix();
        mDrawTrackBall.setTransform(v);
        mDrawTrackBall.updateDragging(isDragging());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_H
