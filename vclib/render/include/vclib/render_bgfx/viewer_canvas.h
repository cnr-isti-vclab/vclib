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

#ifndef VCL_BGFX_RENDER_VIEWER_CANVAS_H
#define VCL_BGFX_RENDER_VIEWER_CANVAS_H

#include <vclib/render/interfaces/viewer_i.h>

#include <vclib/render_bgfx/canvas.h>
#include <vclib/render_bgfx/drawable/drawable_axis.h>
#include <vclib/render_bgfx/drawable/drawable_directional_light.h>
#include <vclib/render_bgfx/drawable/drawable_trackball.h>
#include <vclib/render_bgfx/drawable/uniforms/camera_uniforms.h>
#include <vclib/render_bgfx/drawable/uniforms/directional_light_uniforms.h>
#include <vclib/render_bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>

namespace vcl {

class ViewerCanvas : public Canvas, public ViewerI
{
    CameraUniforms             mCameraUniforms;
    DirectionalLightUniforms   mDirectionalLightUniforms;
    MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

    DrawableAxis             mAxis;
    DrawableDirectionalLight mDirectionalLight;
    DrawableTrackBall        mTrackBall;

public:
    ViewerCanvas(
        void* winId,
        uint  width     = 1024,
        uint  height    = 768,
        void* displayId = nullptr);

    ViewerCanvas(
        void*                                        winId,
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width     = 1024,
        uint                                         height    = 768,
        void*                                        displayId = nullptr);

    void toggleAxisVisibility() override
    {
        mAxis.setVisibility(!mAxis.isVisible());
    }

    void toggleTrackBallVisibility() override
    {
        mTrackBall.setVisibility(!mTrackBall.isVisible());
    }

protected:
    void draw() override;

    // events
    void onResize(unsigned int width, unsigned int height) override;

    void onKeyPress(Key::Enum key) override;

    void onKeyRelease(Key::Enum key) override;

    void onMouseMove(double x, double y) override;

    void onMousePress(MouseButton::Enum button) override;

    void onMouseRelease(MouseButton::Enum button) override;

    void onMouseScroll(double dx, double dy) override;

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
        mDirectionalLightUniforms.updateLight(light());
    }

    void updateDrawableTrackball()
    {
        auto v = DTB::gizmoMatrix();
        mTrackBall.updateRotation(v);
        mTrackBall.updateDragging(isDragging());
    }
};

} // namespace vcl

#endif // VCL_BGFX_RENDER_VIEWER_CANVAS_H
