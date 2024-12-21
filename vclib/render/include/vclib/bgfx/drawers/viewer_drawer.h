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
    ViewerDrawerBGFX(
        uint  width     = 1024,
        uint  height    = 768);

    ViewerDrawerBGFX(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width     = 1024,
        uint                                         height    = 768);

    void toggleAxisVisibility() override
    {
        mAxis.setVisibility(!mAxis.isVisible());
    }

    void toggleTrackBallVisibility() override
    {
        mDrawTrackBall.setVisibility(!mDrawTrackBall.isVisible());
    }

    void onDraw(uint) override;

    void onDrawContent(uint) override;

    // events
    // void onMouseDoubleClick(
    //     MouseButton::Enum   button,
    //     double              x,
    //     double              y,
    //     const KeyModifiers& modifiers) override;

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
