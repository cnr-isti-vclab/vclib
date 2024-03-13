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

#include <vclib/gui/desktop_trackball.h>
#include <vclib/render/drawable_object_vector.h>

#include "drawable_axis.h"
#include "drawable_directional_light.h"
#include "uniforms/camera_uniforms.h"
#include "uniforms/directional_light_uniforms.h"
#include "uniforms/mesh_render_settings_uniforms.h"

namespace vcl::bgf {

class MinimalViewer : public vcl::DesktopTrackBall<float>
{
    // this Viewer does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> drawList;

    CameraUniforms             cameraUniforms;
    DirectionalLightUniforms   directionalLightUniforms;
    MeshRenderSettingsUniforms meshRenderSettingsUniforms;

    DrawableAxis             axis;
    DrawableDirectionalLight directionalLight;

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

    void toggleAxisVisibility() { axis.setVisibility(!axis.isVisible()); }

    bool isDirectionalLightVisible() const
    {
        return directionalLight.isVisible();
    }

    void setDirectionalLightVisibility(bool b)
    {
        directionalLight.setVisibility(b);
    }

    void updateDirectionalLight()
    {
        directionalLight.update(light());
        directionalLightUniforms.updateLight(light());
    }

private:
    void initDrawableObject(DrawableObjectI& obj);
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_MINIMAL_VIEWER_H
