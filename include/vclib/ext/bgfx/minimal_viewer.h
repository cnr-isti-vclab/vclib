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

#ifndef VCL_EXT_BGFX_MINIMAL_VIEWER_H
#define VCL_EXT_BGFX_MINIMAL_VIEWER_H

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/interfaces/event_manager_i.h>
#include <vclib/render/viewer/desktop_trackball.h>

#include "drawable_axis.h"
#include "drawable_directional_light.h"
#include "drawable_trackball.h"
#include "uniforms/camera_uniforms.h"
#include "uniforms/directional_light_uniforms.h"
#include "uniforms/mesh_render_settings_uniforms.h"

namespace vcl::bgf {

class MinimalViewer :
        public vcl::DesktopTrackBall<float>,
        public virtual vcl::EventManagerInterface
{
    // this Viewer does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> mDrawList;

    CameraUniforms             mCameraUniforms;
    DirectionalLightUniforms   mDirectionalLightUniforms;
    MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

    DrawableAxis             mAxis;
    DrawableDirectionalLight mDirectionalLight;
    DrawableTrackBall        mTrackBall;

protected:
    using DTB = vcl::DesktopTrackBall<float>;

public:
    MinimalViewer(uint width = 1024, uint height = 768);

    MinimalViewer(
        std::shared_ptr<DrawableObjectVector> v,
        uint                                  width  = 1024,
        uint                                  height = 768);

    const DrawableObjectVector& drawableObjectVector() const;

    void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v);

    uint pushDrawableObject(const DrawableObjectI& obj);

    void fitScene();

    void draw(uint viewId);

    void toggleAxisVisibility() { mAxis.setVisibility(!mAxis.isVisible()); }

    void toggleTrackballVisibility()
    {
        mTrackBall.setVisibility(!mTrackBall.isVisible());
    }

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

    // events

    void onResize(unsigned int width, unsigned int height) override;

    void onKeyPress(Key::Enum key) override;

    void onKeyRelease(Key::Enum key) override;

    void onMouseMove(double x, double y) override;

    void onMousePress(MouseButton::Enum button) override;

    void onMouseRelease(MouseButton::Enum button) override;

    void onMouseScroll(double dx, double dy) override;

private:
    void initDrawableObject(DrawableObjectI& obj);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_MINIMAL_VIEWER_H
